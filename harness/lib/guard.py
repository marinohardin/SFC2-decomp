"""Decisions for the PreToolUse hook of a function session (harness/hooks/guard.py).

The agent may edit rebuilt source and nothing else, and may run the build-and-verify script and
read-only commands. Everything that judges its work (the harness, the compiler and its flags, the
original exe and its hashes, the analysis exports, generated objects and listings, the queue and
the matched set) is out of reach, as are the hook settings themselves.
"""
import json
import os
import re
import shlex

from . import paths

# Commands a function session may run, by first word. Each segment of a pipeline or a command
# list is checked on its own, and the arguments of the commands that can write a file (sort -o,
# uniq OUT, xxd -r or OUT, find -fprint, sed w/e, git --output, file -C, rg --pre) are checked
# below. awk, less and more are left out: each can run commands or write files.
READ_ONLY = {'ls', 'cat', 'head', 'tail', 'grep', 'egrep', 'rg', 'wc', 'find', 'sort', 'uniq',
             'cut', 'diff', 'cmp', 'xxd', 'od', 'file', 'stat', 'pwd', 'echo', 'printf', 'true',
             'sed', 'tr', 'column', 'nl', 'basename', 'dirname', 'realpath', 'c++filt', 'which',
             'test', '['}
GIT_READ_ONLY = {'diff', 'status', 'log', 'show', 'blame', 'grep', 'ls-files', 'rev-parse'}
VERIFY_REL = ('harness/build-and-verify.sh', './harness/build-and-verify.sh')
SEGMENT_SPLIT = re.compile(r'\|\||&&|\|&|;|\||&|\n')
SAFE_REDIRECTS = re.compile(r'\d?>&\d|\d?>\s*/dev/null')
FIND_ACTIONS = {'-delete', '-exec', '-execdir', '-ok', '-okdir', '-fprint', '-fprint0',
                '-fprintf', '-fls'}
# sed may only print line ranges or matching lines: 10,20p  $p  /re/p  /a/,/b/p
SED_PRINT = re.compile(r'^((\d+|\$)(,(\d+|\$))?|/[^/;]*/(,/[^/;]*/)?)p$')
XXD_VALUE_FLAGS = {'-c', '-cols', '-l', '-len', '-s', '-seek', '-g', '-groupsize', '-o',
                   '-offset', '-n', '-name'}


def protected_roots():
    r = paths.REPO
    return [os.path.join(r, p) for p in
            ('harness', 'toolchain', 'analysis', 'ghidra', 'scripts', 'oracle', '.claude',
             '.githooks', '.github', 'third_party', 'docs', 'AGENTS.md', 'CLAUDE.md')] + [
        paths.STATE, paths.SHARED, paths.DIFFICULT, paths.REFERENCE, paths.TOOLS, paths.MATCHED_SET, paths.UNITS, paths.EXE,
        paths.FUNCTIONS_CSV, paths.HASHES]


def _inside(path, root):
    path, root = os.path.realpath(path), os.path.realpath(root)
    return path == root or path.startswith(root.rstrip(os.sep) + os.sep)


def check_edit(file_path, cwd=None):
    """None to allow, or the reason to deny an Edit/Write of file_path."""
    if not file_path:
        return 'no file path given'
    p = file_path if os.path.isabs(file_path) else os.path.join(cwd or paths.REPO, file_path)
    for root in protected_roots():
        if _inside(p, root):
            return (f'{file_path} is part of what judges your work (harness, toolchain, original '
                    f'binary, analysis exports, matched set, queue or settings); it cannot be '
                    f'edited from a function session')
    if p.lower().endswith(('.asm', '.obj', '.lib', '.exe', '.dll', '.pdb')):
        return f'{file_path} is generated or binary output; change the source instead'
    if not _inside(p, paths.SRC_ROOT):
        return (f'{file_path} is outside the rebuilt source tree '
                f'({os.path.relpath(paths.SRC_ROOT, paths.REPO)}/); edit only source files there')
    return None


# Any claim line, in any module: a session may add or change only its own function's.
CLAIM_LINE = re.compile(r'^\s*//\s*(FUNCTION|STUB|VTABLE):\s*(\w+)\s+0x([0-9A-Fa-f]{1,8})\s*$')
ANNOTATED_EXTS = ('.cpp', '.c', '.cxx', '.h', '.hpp', '.inl')


def claimed_address():
    """The function this session works on, from $HARNESS_STATE/claim.json, as an int or None."""
    try:
        with open(paths.CLAIM) as f:
            return int(json.load(f)['address'], 16)
    except (OSError, ValueError, KeyError, TypeError):
        return None


def other_claims(text, claimed):
    """Claim lines in text for any address but the claimed one: {(kind, module, address): count}."""
    out = {}
    for line in (text or '').splitlines():
        m = CLAIM_LINE.match(line)
        if m and int(m.group(3), 16) != claimed:
            k = (m.group(1), m.group(2), int(m.group(3), 16))
            out[k] = out.get(k, 0) + 1
    return out


def _apply(text, old, new, replace_all=False):
    """The file text after one Edit, as the Edit tool applies it (None if old is not found)."""
    if not old or old not in text:
        return None
    return text.replace(old, new) if replace_all else text.replace(old, new, 1)


def check_claims(tool, file_path, ti, cwd=None):
    """None, or the reason an Edit/Write/MultiEdit would remove or change the claim line of a
    function other than the session's own. The edit is applied to the whole file and the file's
    claim lines compared, so an edit of part of a line is seen too (the loop checks the whole
    tree again after the session)."""
    p = file_path if os.path.isabs(file_path) else os.path.join(cwd or paths.REPO, file_path)
    if not os.path.realpath(p).lower().endswith(ANNOTATED_EXTS) and not p.lower().endswith(ANNOTATED_EXTS):
        return None
    claimed = claimed_address()
    try:
        with open(p, encoding='latin1', newline='') as f:
            old = f.read()
    except OSError:
        old = None
    if tool == 'Write':
        if old is None:
            return None
        new = ti.get('content', '')
    else:
        edits = ti.get('edits') if tool == 'MultiEdit' else [ti]
        if old is None:
            # no file to apply to: compare each edit's own text
            old = ''.join(e.get('old_string', '') for e in edits or [])
            new = ''.join(e.get('new_string', '') for e in edits or [])
        else:
            new = old
            for e in edits or []:
                applied = _apply(new, e.get('old_string', ''), e.get('new_string', ''),
                                 bool(e.get('replace_all')))
                if applied is None:
                    return None  # the Edit tool refuses it anyway
                new = applied
    before, after = other_claims(old, claimed), other_claims(new, claimed)
    lost = sorted(k for k, n in before.items() if after.get(k, 0) < n)
    if lost:
        names = ', '.join(f'{k} {m} 0x{a:08x}' for k, m, a in lost)
        return (f'{file_path}: this edit removes or changes the claim line of another '
                f'function ({names}). Only your own function\'s line may change; if another '
                f'claim is in the way, leave it and say so in your final message')
    return None


# The owner's personal data: never read by a session, by any tool. Lean sessions do not load
# the project's permission rules, so this hook is what keeps them out.
PRIVATE_MARKERS = re.compile(r'(?i)(work/private|wine/prefix|testkey-data|\.reg\b|\.env\b|'
                             r'(^|/)private(/|$))')
PRIVATE_DENY = ("the owner's private files (work/private, the Wine prefix's .reg files, the test "
                "key data, any .reg or .env file) are never read")


def private_roots():
    w = paths.WORK
    return [os.path.join(w, 'private'), os.path.join(w, 'wine', 'prefix'),
            os.path.join(w, 'testkey-data')]


def check_read(path, cwd=None):
    """None to allow, or the reason to deny reading, searching or listing `path`: inside a
    private root, an ancestor of one (a search would reach it), or a .reg/.env file outside the
    repository."""
    if not path:
        return None
    p = path if os.path.isabs(path) else os.path.join(cwd or paths.REPO, path)
    wild = re.search(r'[*?\[{]', p)
    if wild:  # a pattern: judge the folder it starts from, which it may reach below
        p = os.path.dirname(p[:wild.start()]) or os.sep
    for root in private_roots():
        if _inside(p, root) or _inside(root, p):
            return PRIVATE_DENY
    if p.lower().endswith(('.reg', '.env')) and not _inside(p, paths.REPO):
        return PRIVATE_DENY
    if PRIVATE_MARKERS.search(path) and not _inside(p, paths.REPO):
        return PRIVATE_DENY
    return None


# The Bash side of the private-file rule. It refuses on sight, without trying to work out what
# a command would really touch: over-refusal is deliberate. A false refusal costs a grind session
# one turn; a leak of the owner's personal data is not acceptable. Refused:
# - any of these substrings anywhere in the command, in any letter case, as typed and again
#   after shell quoting is removed;
# - `$` in any form (variables, $'...' escapes) and a leading `~`, which could spell a path;
# - a glob or brace pattern (* ? [ {) that could expand to one of the private names, or that
#   points outside the repository (absolute, or climbing with ..);
# - any path argument that is a private folder or an ancestor of one (`grep -r key ..`).
PRIVATE_SUBSTRINGS = ('private', 'testkey', 'wine/prefix', '.reg', '.env')
PRIVATE_NAMES = ('private', 'testkey', 'testkey-data', 'wine', 'prefix', 'x.reg', 'x.env')
GLOB_CHARS = re.compile(r'[*?\[{]')
MANIFEST_CHECK = ['python3', 'tools/manifest.py', '--check']


def _expand_braces(word, limit=64):
    """The words a shell brace pattern expands to (a{b,c}d -> abd, acd), capped."""
    m = re.search(r'\{([^{}]*)\}', word)
    if not m or ',' not in m.group(1):
        return [word]
    out = []
    for alt in m.group(1).split(','):
        out += _expand_braces(word[:m.start()] + alt + word[m.end():], limit)
        if len(out) >= limit:
            break
    return out[:limit]


def _glob_could_reach_private(word):
    import fnmatch
    for w in _expand_braces(word):
        if any(x in w.lower() for x in PRIVATE_SUBSTRINGS):
            return True
        if not GLOB_CHARS.search(w):
            continue
        if os.path.isabs(w) or '..' in w.split('/'):
            return True
        for part in w.split('/'):
            if GLOB_CHARS.search(part) and any(fnmatch.fnmatch(n, part.lower())
                                               for n in PRIVATE_NAMES):
                return True
    return False


def check_private_bash(command):
    """PRIVATE_DENY if the command could read the owner's private files, else None."""
    low = command.lower()
    if any(x in low for x in PRIVATE_SUBSTRINGS) or '$' in command:
        return PRIVATE_DENY
    try:
        words = shlex.split(command)
    except ValueError:
        return PRIVATE_DENY
    for w in words:
        if any(x in w.lower() for x in PRIVATE_SUBSTRINGS) or w.startswith('~'):
            return PRIVATE_DENY
        if (GLOB_CHARS.search(w) or '{' in w) and _glob_could_reach_private(w):
            return PRIVATE_DENY
        if w.startswith('-'):
            continue
        p = w if os.path.isabs(w) else os.path.join(paths.REPO, w)
        if any(_inside(p, root) or _inside(root, p) for root in private_roots()):
            return PRIVATE_DENY
    return None


def check_bash(command):
    """None to allow, or the reason to deny a Bash command."""
    reason = check_private_bash(command)
    if reason:
        return reason
    if 'ANTHROPIC_API_KEY' in command or 'ANTHROPIC_AUTH_TOKEN' in command:
        return 'API credentials are not used in function sessions'
    if '`' in command or '$(' in command or '<(' in command or '>(' in command:
        return 'command and process substitution are not allowed; run the commands one at a time'
    stripped = SAFE_REDIRECTS.sub('', command)
    if '>' in stripped:
        return 'redirecting output to a file is not allowed; use the Edit or Write tool on source'
    for seg in SEGMENT_SPLIT.split(stripped):
        seg = seg.strip()
        if not seg:
            continue
        try:
            words = shlex.split(seg)
        except ValueError:
            return f'cannot parse {seg!r}'
        if not words:
            continue
        if re.match(r'^[A-Za-z_][A-Za-z0-9_]*=', words[0]):
            return 'setting environment variables is not allowed'
        reason = _check_words(words)
        if reason:
            return reason
    return None


def _is_verify(word):
    """The real build-and-verify.sh, not a look-alike elsewhere (the session runs in the repo)."""
    return word in VERIFY_REL or word == os.path.join(paths.HARNESS, 'build-and-verify.sh')


def _positional(args, value_flags=()):
    out, skip = [], False
    for a in args:
        if skip:
            skip = False
        elif a in value_flags:
            skip = True
        elif not a.startswith('-') or a == '-':
            out.append(a)
    return out


def _check_words(words):
    cmd, args = words[0], words[1:]
    if cmd in ('sh', 'bash') and args and _is_verify(args[0]):
        cmd, args = args[0], args[1:]
    if words == MANIFEST_CHECK:  # read-only: reports, writes nothing
        return None
    if _is_verify(cmd):
        if any(a == '--record' for a in args):
            return 'only the harness records results'
        return None
    if any(a.startswith(('--output', '--pre', '--hostname-bin')) for a in args):
        return f'{cmd}: options that write files or run programs are not allowed'
    if cmd == 'git':
        if any(a == '-c' or a.startswith(('-O', '--open-files-in-pager', '--ext-diff', '--exec'))
               for a in args):
            return 'git options that run programs or change configuration are not allowed'
        sub = next((a for a in args if not a.startswith('-')), '')
        if sub in GIT_READ_ONLY:
            return None
        return f'git {sub} is not allowed; the loop commits and resets for you'
    if cmd not in READ_ONLY:
        return (f'{cmd} is not allowed in a function session: run harness/build-and-verify.sh '
                f'<function> to build and score, and read-only commands to look around')
    if cmd == 'find' and any(a in FIND_ACTIONS for a in args):
        return 'find may only list files here'
    if cmd == 'sort' and any(a.startswith('-') and not a.startswith('--') and 'o' in a
                             for a in args):
        return 'sort -o writes a file; it is not allowed'
    if cmd == 'uniq' and len(_positional(args, ('-f', '-s', '-w'))) > 1:
        return 'uniq with an output file is not allowed'
    if cmd == 'xxd':
        if any(a.startswith('-r') for a in args):
            return 'xxd -r writes binary; it is not allowed'
        if len(_positional(args, XXD_VALUE_FLAGS)) > 1:
            return 'xxd with an output file is not allowed'
    if cmd == 'file' and any(a in ('-C', '--compile') for a in args):
        return 'file -C writes a file; it is not allowed'
    if cmd == 'sed':
        return _check_sed(args)
    return None


def _check_sed(args):
    """sed only as a pager: -n with one print-only script (use the Read tool for anything else)."""
    deny = 'sed may only print lines here (sed -n 10,20p FILE); use the Read tool or Edit instead'
    if '-n' not in args or any(a.startswith(('-i', '--in-place', '-f', '--file', '-s', '-E',
                                             '-r', '-z', '-u', '-l'))
                               for a in args if a != '-n'):
        return deny
    scripts = [args[i + 1] for i, a in enumerate(args[:-1]) if a == '-e']
    if not scripts:
        rest = [a for a in args if a != '-n']
        scripts = rest[:1]
    if not scripts or any(not SED_PRINT.match(sc.replace(' ', '')) for sc in scripts):
        return deny
    return None


def decide(event):
    """PreToolUse event dict -> None (no objection) or a deny reason."""
    tool = event.get('tool_name', '')
    ti = event.get('tool_input') or {}
    if tool in ('Edit', 'Write', 'MultiEdit'):
        return (check_edit(ti.get('file_path'), event.get('cwd'))
                or check_claims(tool, ti.get('file_path'), ti, event.get('cwd')))
    if tool in ('Read', 'Grep', 'Glob'):
        return (check_read(ti.get('file_path') or ti.get('path'), event.get('cwd'))
                or (check_read(ti['pattern'], event.get('cwd'))
                    if tool == 'Glob' and ti.get('pattern') else None))
    if tool == 'NotebookEdit':
        return 'notebooks are not part of the rebuilt source'
    if tool == 'Bash':
        return check_bash(ti.get('command', ''))
    return None
