"""New stand-in classes: the whole-tree check that a session did not invent a placeholder class.

A stand-in class is a class or struct declared in `op/` under a name the original does not have: a
placeholder built from an address (`fn_00674250_Owner`, `c_0067b2b0`, `c_tMachine_Field04`,
`tObject_00410fd4`, `tChunk_0042dd9b`), or a `t...` class name that no row of the symbol table, no
SDK library match and no Ghidra export knows. A session must use the real class, ask for its
header, or say in its final message why none exists. Stand-ins already in the base are tolerated
debt; only the ones this tree adds are refused. Free `fn_<8 hex>` function names are the sanctioned
placeholder for an unclaimed callee and are not looked at: only class/struct declarations are.

    python3 -m harness.lib.standins [--base REF]          # refuse (exit 1) on a new stand-in
    python3 -m harness.lib.standins --report [--base REF] # landing: count and list, always exit 0
    python3 -m harness.lib.standins --record FILE         # collect the allowances of a session's
                                                          # final message (claude --output-format json)

The base is `HARNESS_STANDIN_BASE`, else the merge base of HEAD and origin/develop (or develop).
Names are compared as sets, so a declaration that moves to another file is not new. The scan is
text only: `git grep` at the base, a walk of `op/` now. No compile.

An allowance is a line `STAND-IN: <name> <address> <reason>` in the session's final message; it is
kept in `$HARNESS_STATE/standins.tsv` (the lane's state), so the refusal does not come back, and
the landing step lists it. A `HEADER REQUEST: <class> <why>` line is kept in
`$HARNESS_STATE/header_requests.txt` for the integrator.
"""
import csv
import glob
import json
import os
import re
import subprocess
import sys

from . import paths

SOURCE_EXTS = ('.h', '.hpp', '.inl', '.c', '.cpp')
# `class X`, `struct X` at the start of a line (a definition or a forward declaration), not a
# template parameter, a friend declaration or a variable (`struct X x;`).
DECL = re.compile(r'^\s*(?:template\s*<[^>]*>\s*)?(?:class|struct|union)\s+(?:__declspec\s*\([^)]*\)\s*)?'
                  r'([A-Za-z_]\w*)\s*(?:[:{;]|$)')
PATTERNS = (('fn_<address>_<Word>', re.compile(r'^fn_[0-9a-fA-F]{8}_\w+$')),
            ('c_<...>', re.compile(r'^[cC]_\w+$')),
            ('<name>_<address>', re.compile(r'^\w*_[0-9a-fA-F]{8}(?:_\w+)?$')))
PROJECT_CLASS = re.compile(r't[A-Z]\w*$')
ALLOW = re.compile(r'^\s*STAND-IN:\s*(\S+)\s+(0x[0-9a-fA-F]{1,8})\s+(\S.*?)\s*$', re.M)
HEADER = re.compile(r'^\s*HEADER REQUEST:\s*(\S.*?)\s*$', re.M)
# A class name inside a name or a mangled symbol: not after a lowercase letter, so `getTable` and
# `Contract` are not read as tTable and tContract.
TOKEN = re.compile(r'(?<![a-z])t[A-Z][A-Za-z0-9_]*')
ADVICE = ('A new stand-in class is a refusal. Do this instead:\n'
          '  1. Look the address or vtable up: python3 tools/symbols.py (the symbol table, '
          'analysis/symbols/STARFLEETOP.tsv, has the real class and symbol).\n'
          '  2. Use the real class if the table has it, and include its header.\n'
          '  3. If the real class\'s header is outside your file list, write a line '
          '`HEADER REQUEST: <class> <why>` in your final message, leave the function unclaimed, and '
          'do not declare a local copy of the class.\n'
          '  4. If the original truly has no such class, say so in your final message with one line '
          '`STAND-IN: <name> <address> <reason>` per name; the check then accepts it and the landing '
          'step lists it.')


def state_file(name):
    return os.path.join(paths.STATE, name)


# --- what a declaration looks like ------------------------------------------------------------

def declared(text):
    """[(name, line number)] of the class/struct/union declarations in text."""
    out = []
    for n, line in enumerate(text.splitlines(), 1):
        m = DECL.match(line)
        if m:
            out.append((m.group(1), n))
    return out


def pattern_of(name):
    """The placeholder form `name` has, or ''."""
    for label, rx in PATTERNS:
        if rx.match(name):
            return label
    return ''


def known_tokens(files):
    """The project class names (t + capital) the symbol evidence mentions: the class, name and
    symbol columns of the table, SDK library matches, Ghidra's exports. Mangled names are split
    at their punctuation (`??0tWinTimer@@...`, `?AVtThread@@`)."""
    out = set()
    for f in files:
        try:
            with open(f, errors='replace') as fh:
                text = fh.read()
        except OSError:
            continue
        out.update(TOKEN.findall(text))
    return out


def default_known_files():
    return ([paths.SYMBOL_TABLE] + list(paths.SYMBOL_TSVS) + [
        os.path.join(paths.REPO, 'analysis', 'results', n) for n in (
            'StarFleetOP_2552_sdklib_matches.tsv', 'StarFleetOP_2500_sdklib_matches.tsv')] +
        [paths.FUNCTIONS_CSV, os.path.join(paths.REPO, 'ghidra', 'exports', 'classes.csv'),
         os.path.join(paths.REPO, 'ghidra', 'exports', 'vtables.csv')])


def kind_of(name, known):
    """Why `name` is a stand-in: the pattern it matches, or 'unknown t-class', or ''."""
    label = pattern_of(name)
    if label:
        return 'placeholder name ' + label
    if PROJECT_CLASS.match(name) and name not in known:
        return 'a t-class no symbol table row, SDK match or export knows'
    return ''


# --- the base and the tree ---------------------------------------------------------------------

def _git(args, repo):
    p = subprocess.run(['git'] + args, cwd=repo, capture_output=True, text=True)
    return p.returncode, p.stdout


def base_ref(repo=None, environ=None):
    """The commit the tree is compared with, or None when there is none to find."""
    repo = repo or paths.REPO
    env = os.environ if environ is None else environ
    given = env.get('HARNESS_STANDIN_BASE')
    if given:
        return given
    for upstream in ('origin/develop', 'develop'):
        rc, out = _git(['merge-base', 'HEAD', upstream], repo)
        if rc == 0 and out.strip():
            return out.strip()
    return None


def base_names(base, repo=None):
    """The set of class/struct names declared under op/ at `base`."""
    repo = repo or paths.REPO
    rc, out = _git(['grep', '-h', '-E', '(class|struct|union)[[:space:]]+[A-Za-z_]', base, '--', 'op'],
                   repo)
    names = set()
    if rc in (0, 1):
        for name, _ in declared(out):
            names.add(name)
    return names


def tree_declarations(repo=None, subdir='op'):
    """{name: (relative path, line)} of the first declaration of each name in the working tree."""
    repo = repo or paths.REPO
    root = os.path.join(repo, subdir)
    found = {}
    for here, dirs, files in os.walk(root):
        dirs[:] = sorted(d for d in dirs if not d.startswith('.'))
        for fn in sorted(files):
            if not fn.endswith(SOURCE_EXTS):
                continue
            path = os.path.join(here, fn)
            try:
                with open(path, errors='replace') as f:
                    text = f.read()
            except OSError:
                continue
            for name, line in declared(text):
                found.setdefault(name, (os.path.relpath(path, repo), line))
    return found


def load_allowed(state=None):
    """{name: (address, reason)} the lane state accepts."""
    out = {}
    try:
        with open(state or state_file('standins.tsv'), newline='') as f:
            for row in csv.reader(f, delimiter='\t'):
                if len(row) >= 3:
                    out[row[0]] = (row[1], row[2])
    except OSError:
        pass
    return out


def findings(repo=None, base=None, known=None, allowed=None, environ=None):
    """[(name, path, line, why)] of the stand-in classes declared now and not at the base, minus
    the allowed ones; [] when there is no base to compare with."""
    repo = repo or paths.REPO
    base = base or base_ref(repo, environ)
    if base is None:
        return []
    old = base_names(base, repo)
    known = known_tokens(default_known_files()) if known is None else known
    allowed = load_allowed() if allowed is None else allowed
    out = []
    for name, (path, line) in sorted(tree_declarations(repo).items()):
        if name in old or name in allowed:
            continue
        why = kind_of(name, known)
        if why:
            out.append((name, path, line, why))
    return out


def lines_of(found):
    return [f'{path}:{line}: {name} ({why})' for name, path, line, why in found]


# --- the session's final message ---------------------------------------------------------------

def allowances_in(text):
    """[(name, address, reason)] of each well-formed STAND-IN: line."""
    return [(n, a.lower(), r) for n, a, r in ALLOW.findall(text or '')]


def record(text, state=None, header_state=None):
    """Keep the allowances and header requests of a final message in the lane state; returns the
    allowances that were new."""
    state = state or state_file('standins.tsv')
    have = load_allowed(state)
    new = [a for a in allowances_in(text) if a[0] not in have]
    if new:
        os.makedirs(os.path.dirname(state), exist_ok=True)
        with open(state, 'a', newline='') as f:
            w = csv.writer(f, delimiter='\t', lineterminator='\n')
            for row in new:
                w.writerow(row)
    asks = HEADER.findall(text or '')
    if asks:
        path = header_state or state_file('header_requests.txt')
        try:
            with open(path) as f:
                seen = set(f.read().splitlines())
        except OSError:
            seen = set()
        fresh = [a for a in dict.fromkeys(asks) if a not in seen]
        if fresh:
            os.makedirs(os.path.dirname(path), exist_ok=True)
            with open(path, 'a') as f:
                f.write(''.join(a + '\n' for a in fresh))
    return new


def last_message(payload):
    """The session's last assistant text from a Stop hook's input: `last_assistant_message`, else
    the end of the transcript file."""
    if not isinstance(payload, dict):
        return ''
    text = payload.get('last_assistant_message')
    if isinstance(text, str) and text:
        return text
    try:
        with open(payload.get('transcript_path') or '') as f:
            rows = [json.loads(x) for x in f if x.strip()]
    except (OSError, ValueError):
        return ''
    for row in reversed(rows):
        msg = row.get('message') if isinstance(row, dict) else None
        if isinstance(msg, dict) and msg.get('role') == 'assistant':
            content = msg.get('content')
            if isinstance(content, str):
                return content
            parts = [c.get('text', '') for c in content or [] if isinstance(c, dict)
                     and c.get('type') == 'text']
            if parts:
                return '\n'.join(parts)
    return ''


# --- the whole-tree check and the landing report ---------------------------------------------

def check_lines(told=(), **kw):
    """The refusal for wholetree: a header line, the findings not already told, the advice. None
    when there is nothing to say. `told`: names a refusal already named once."""
    found = [f for f in findings(**kw) if f[0] not in set(told)]
    if not found:
        return None
    return ['new stand-in classes (not in the base, not in the symbol table):'] + \
        lines_of(found) + ADVICE.splitlines()


def pending_names(**kw):
    return [f[0] for f in findings(**kw)]


def landing_lines(base, repo=None):
    """The landing report: new stand-ins on this tree against `base`, with the allowance each has
    in any lane's state."""
    found = findings(repo=repo, base=base, allowed={})
    allowed = {}
    for f in glob.glob(os.path.join(paths.WORK, 'harness', '*', 'standins.tsv')):
        allowed.update(load_allowed(f))
    allowed.update(load_allowed())
    out = [f'new stand-in classes against {base}: {len(found)}']
    for name, path, line, why in found:
        a = allowed.get(name)
        out.append(f'  {path}:{line}: {name} ({why})' +
                   (f'; allowed at {a[0]}: {a[1]}' if a else '; no STAND-IN allowance'))
    return out


def main(argv=None):
    a = list(sys.argv[1:] if argv is None else argv)
    base = None
    if '--base' in a:
        i = a.index('--base')
        base = a[i + 1]
    if '--record' in a:
        try:
            with open(a[a.index('--record') + 1]) as f:
                result = (json.load(f) or {}).get('result') or ''
        except (OSError, ValueError, IndexError):
            return 0
        for n, ad, r in record(result if isinstance(result, str) else ''):
            print(f'INFO  stand-in allowed: {n} {ad} {r}')
        return 0
    if '--report' in a:
        print('\n'.join(landing_lines(base or base_ref() or 'origin/develop')))
        return 0
    lines = check_lines(base=base)
    if lines:
        print('\n'.join(lines))
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
