#!/usr/bin/env python3
"""Check the claims in op/ for the rule breaks the byte build cannot see.

None of these lowers a score, so the harness and the progress gate pass them; each is a rule
in AGENTS.md or docs/CONVENTIONS.md that a reviewer otherwise has to find by reading. Each break
is printed as `path:line: <check>: <message>`, the path relative to the repository root.

empty-body
    A `// FUNCTION:` or `// STUB:` annotation whose definition has an empty body: `{}`, or only
    whitespace, comments, preprocessor lines and stray `;` between the braces (AGENTS.md, "No
    claim sits on a body that is not the original's code"). A definition that ends in `;` (a
    global object's, claimed by stacked annotations) has no body and is not checked. Allowed:

    - a claim that analysis/results/matched_set.json records exact: `{}` compiling to the
      original's bytes is the evidence that the original's body was empty too. Size is not: of
      the 63 exact empty bodies on develop when this check was written, two are a bare `ret` /
      `ret N` (3 bytes) and the rest are constructors, destructors and functions with a
      by-value parameter, 7 to 432 bytes of code VC6 generates around an empty body (vftable
      stores, base and member destructors, a parameter's destructor), so a size limit would
      reject the original's own code. An empty body that is not exact is a placeholder, however
      small the original;
    - a constructor with a member-initializer list (`A::A() : fB(0) {}`): its code is the list;
    - the deleting-destructor form (harness/SRC_LAYOUT.md): annotations stacked on a class's
      out-of-class destructor, the evidence naming one a scalar or vector deleting destructor
      (harness/lib/deleting.py). The empty destructor is legal when it is the class's real
      destructor. The stack shows that by claiming the destructor too, exact (a claim in the
      stack that is not a deleting destructor, judged by the rule above); or the deleting
      destructor is exact and a `// MATCH-ONLY:` comment directly above the stack says why the
      empty definition is the class's (its destructor implicit, or folded with a base's:
      CONVENTIONS §4.1, "a compiler-generated member written out"). A stack of deleting
      destructors alone on an empty destructor with neither is a break: the body may stand in
      for a destructor the original wrote elsewhere, with code (the case of `~tCountMap()` and
      0x004361cb), and an exact score alone cannot tell, since a scalar deleting destructor
      calls the destructor out of line. Not exact, it is the case of the empty explicit
      specializations on 0x0057ab52 and 0x0057ab6f.

fragment-claim
    A claim on an address that analysis/results/boundary_errors.tsv lists as `body-fragment` or
    `library-instance`: the lanes moved those there because they are not functions of their own.
    Also a claim on an address strictly inside a ghidra/exports/functions.csv row: the analysis
    does not start a function there, so it is the tail of its neighbour. The usual case is a row
    tagged `eh-prolog-joined`, whose exception prolog and body the analysis joined, the body's
    old start now inside it (ghidra/scripts/join_eh_prologs.py).

address-order
    Within one unit (a `.cpp` or `.c` file), the definitions appear in ascending address order
    (docs/CONVENTIONS.md §4.1, harness/SRC_LAYOUT.md "Order"). A stack of annotations on one
    definition counts once, by the address of the definition itself: the lowest of its
    annotations that are not deleting destructors (a global's initializer, sitting directly
    above, is the lowest of its stack in practice; a deleting destructor is emitted where the
    compiler puts it), or of all of them when every one is a deleting destructor. The breaks are
    the definitions outside one longest ascending run, so that one misplaced function is
    reported once, not as every neighbour it sits between. A `// MATCH-ONLY:` comment that names
    a reason, in the comment block directly above the stack, excuses its definition (§4.1: the
    form exists only to make VC6 emit the original bytes). Headers are not units: their claims
    compile in the unit op/UNITS.tsv names, so their order is not checked.

non-source
    A file under op/ that is not a source or header (`.cpp`, `.c`, `.h`, `.inl`), a `README.md`,
    or one of the tables at op/'s root (`UNITS.tsv`, `manifest.tsv`). Names beginning with `.`
    are not read (a desktop's `.DS_Store` is not the tree's).

The baseline, analysis/results/lint_baseline.tsv, lists the breaks develop had when the check
started (columns check, path, address, reason; `-` for a file-level break's address). A break in
it is reported as known and does not fail; one not in it fails. A baseline row that no longer
occurs fails too, until it is deleted, so the baseline only shrinks. The address-order
check sets the baseline's addresses aside before it finds the ascending run, so a new function
cannot shift the blame onto a neighbour, and fixing one listed function stales only its row.
`--write-baseline` rewrites the file to the breaks found now, keeping the reasons of rows that
stay; it is for the integrator, after a fix, not for making a new break pass.

    python3 tools/lint_claims.py                 # check; exit 0 when nothing new and nothing stale
    python3 tools/lint_claims.py --all           # also print the breaks the baseline holds

Exit 0 when every break is in the baseline and every baseline row still occurs, 1 otherwise, 2
when an input cannot be read.
"""
import argparse
import bisect
import csv
import json
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, REPO)
from harness.lib import deleting  # noqa: E402  (the one rule for deleting destructors)

MODULE = 'STARFLEETOP'
ANNOTATION = re.compile(r'^\s*//\s*(FUNCTION|STUB):\s*(\w+)\s+0x([0-9A-Fa-f]{1,8})\s*$')
MATCH_ONLY = re.compile(r'//\s*MATCH-ONLY:\s*\S')
SOURCE_EXTS = ('.cpp', '.c')
HEADER_EXTS = ('.h', '.inl')
ROOT_TABLES = ('UNITS.tsv', 'manifest.tsv')
FRAGMENT_KINDS = ('body-fragment', 'library-instance')
JOINED_TAG = 'eh-prolog-joined'
BASELINE_COLUMNS = ('check', 'path', 'address', 'reason')
CHECKS = ('empty-body', 'fragment-claim', 'address-order', 'non-source')

csv.field_size_limit(1 << 24)


class Break:
    __slots__ = ('check', 'path', 'line', 'address', 'message')

    def __init__(self, check, path, line, address, message):
        self.check, self.path, self.line = check, path, line
        self.address, self.message = address, message

    @property
    def key(self):
        return (self.check, self.path, self.address)

    def __str__(self):
        return f'{self.path}:{self.line}: {self.check}: {self.message}'


class Stack:
    """The annotations stacked on one definition, and what follows them."""
    __slots__ = ('anns', 'first_line', 'def_line', 'body', 'header', 'match_only')

    def __init__(self, anns, first_line):
        self.anns = anns              # [(kind, address, line)]
        self.first_line = first_line  # line of the first annotation
        self.def_line = None          # line the definition starts on
        self.body = None              # None: no body (`;`); else the body's code, comments blanked
        self.header = ''              # the definition's code before its body
        self.match_only = False       # a MATCH-ONLY: comment directly above the stack


# --- reading C++ -----------------------------------------------------------------------------

def blank_comments(text):
    """`text` with comments and the contents of string and character literals replaced by spaces,
    newlines kept, so offsets and line numbers stay those of the original."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if c == '/' and text.startswith('//', i):
            j = text.find('\n', i)
            j = n if j < 0 else j
            out.append(' ' * (j - i))
            i = j
        elif c == '/' and text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r'[^\n]', ' ', text[i:j]))
            i = j
        elif c in '"\'':
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            j = min(j, n - 1)
            out.append(c + re.sub(r'[^\n]', ' ', text[i + 1:j]) + text[j])
            i = j + 1
        else:
            out.append(c)
            i += 1
    return ''.join(out)


# The end of a header that is naming an operator: its `=` is part of the name, not an
# initializer (operator=, operator==, operator<=, operator+=, ...).
OPERATOR_TAIL = re.compile(r'\boperator\s*[=!<>+\-*/%&|^]*$')


def _match(code, i, opener, closer):
    """The index of the `closer` that closes the `opener` at code[i], or -1."""
    depth = 0
    for j in range(i, len(code)):
        if code[j] == opener:
            depth += 1
        elif code[j] == closer:
            depth -= 1
            if depth == 0:
                return j
    return -1


def _is_empty(body):
    lines = [ln for ln in body.split('\n') if not ln.strip().startswith('#')]
    return not ''.join(lines).replace(';', '').strip()


def _has_initializer_list(header):
    i = header.find('(')
    if i < 0:
        return False
    j = _match(header, i, '(', ')')
    return j >= 0 and re.search(r'(?<!:):(?!:)', header[j + 1:]) is not None


def is_destructor(header):
    return re.search(r'::\s*~\s*\w+\s*\(', header) is not None


def read_stacks(text):
    """[Stack] for the FUNCTION/STUB annotations of MODULE in a file's text, in file order."""
    lines = text.split('\n')
    code = blank_comments(text)
    starts, pos = [], 0
    for ln in lines:
        starts.append(pos)
        pos += len(ln) + 1
    code_lines = code.split('\n')
    stacks, cur = [], None
    for n, raw in enumerate(lines, 1):
        m = ANNOTATION.match(raw)
        if m and m.group(2) == MODULE:
            if cur is None:
                cur = Stack([], n)
                stacks.append(cur)
            cur.anns.append((m.group(1), int(m.group(3), 16), n))
        elif cur is not None and not (raw.strip().startswith('//')
                                      and not code_lines[n - 1].strip()):
            cur = None
    for s in stacks:
        s.match_only = any(MATCH_ONLY.search(c) for c in comment_block_above(lines, s.first_line))
        last = s.anns[-1][2]
        k = next((k for k in range(last, len(lines)) if code_lines[k].strip()), None)
        if k is None:
            continue
        s.def_line = k + 1
        i = starts[k]
        depth = 0
        while i < len(code):
            c = code[i]
            if c == '(':
                depth += 1
            elif c == ')':
                depth -= 1
            elif depth == 0 and c in ';}':
                break
            elif depth == 0 and c == '=' and not OPERATOR_TAIL.search(code[starts[k]:i]):
                break  # a global's initializer, `T g = {...};`: no body
            elif depth == 0 and c == '{':
                j = _match(code, i, '{', '}')
                if j >= 0:
                    s.header = code[starts[k]:i]
                    s.body = code[i + 1:j]
                break
            i += 1
    return stacks


def comment_block_above(lines, line):
    """The comment-only lines directly above line `line` (1-based), nearest last."""
    out, k = [], line - 2
    while k >= 0 and lines[k].strip().startswith('//'):
        out.append(lines[k])
        k -= 1
    return out[::-1]


# --- the evidence ----------------------------------------------------------------------------

def load_matched(path):
    with open(path) as f:
        return {int(k, 16): v for k, v in json.load(f).get('functions', {}).items()}


def load_boundaries(path):
    """{address: kind} from boundary_errors.tsv, or {} when it is absent."""
    if not os.path.isfile(path):
        return {}
    with open(path, newline='') as f:
        return {int(r['address'], 16): r.get('kind', '') for r in csv.DictReader(f, delimiter='\t')
                if r.get('address')}


def load_functions(path):
    """{address: (size, mangled, name, tags)} from functions.csv, or {} when it is absent."""
    if not os.path.isfile(path):
        return {}
    out = {}
    with open(path, newline='') as f:
        for r in csv.DictReader(f):
            out[int(r['address'], 16)] = (int(r.get('size') or 0), r.get('mangled') or '',
                                          r.get('name') or '',
                                          tuple(t for t in (r.get('tags') or '').split(';') if t))
    return out


def load_baseline(path):
    """{(check, path, address): reason}; {} when the file is absent."""
    if not os.path.isfile(path):
        return {}
    with open(path, newline='') as f:
        return {(r['check'], r['path'], r['address']): r.get('reason', '')
                for r in csv.DictReader(f, delimiter='\t') if r.get('check')}


class Evidence:
    def __init__(self, matched, boundaries, functions):
        self.matched, self.boundaries, self.functions = matched, boundaries, functions
        self.rows = sorted((a, v[0]) for a, v in functions.items())

    def kind(self, address):
        row = self.functions.get(address, (0, '', '', ()))
        return deleting.kind(self.matched.get(address), row[1], row[2])

    def exact(self, address):
        return bool((self.matched.get(address) or {}).get('exact'))

    def container(self, address):
        """The functions.csv row that holds `address` strictly inside it, or None."""
        i = bisect.bisect_left(self.rows, (address, -1)) - 1
        if i >= 0:
            start, size = self.rows[i]
            if start < address < start + size:
                return start
        return None


# --- the checks ------------------------------------------------------------------------------

def hx(a):
    return f'0x{a:08x}'


def check_empty(rel, stack, ev):
    if stack.body is None or not _is_empty(stack.body):
        return []
    if _has_initializer_list(stack.header):
        return []
    out = []
    dtor = is_destructor(stack.header)
    own = [(k, a, n) for k, a, n in stack.anns if not (dtor and ev.kind(a))]
    dels = [(k, a, n) for k, a, n in stack.anns if dtor and ev.kind(a)]
    for kind, a, n in own:
        if not ev.exact(a):
            out.append(Break('empty-body', rel, n, hx(a),
                             f'{kind} {hx(a)} claims an empty body and is not exact: an empty '
                             'body is allowed only where it compiles to the original\'s bytes'))
    if dels and not any(ev.exact(a) for _, a, _ in own):
        for kind, a, n in dels:
            if stack.match_only and ev.exact(a):
                continue
            out.append(Break('empty-body', rel, n, hx(a),
                             f'{kind} {hx(a)} ({ev.kind(a)} deleting destructor) sits on an empty '
                             'destructor the stack does not claim exact, and is not itself exact '
                             'under a MATCH-ONLY comment: the body may stand in for the '
                             "class's real destructor"))
    return out


def check_fragments(rel, stack, ev):
    out = []
    for kind, a, n in stack.anns:
        bk = ev.boundaries.get(a)
        if bk in FRAGMENT_KINDS:
            out.append(Break('fragment-claim', rel, n, hx(a),
                             f'{kind} {hx(a)} is listed as {bk} in boundary_errors.tsv: '
                             'not a function of its own'))
            continue
        host = ev.container(a)
        if host is not None:
            joined = JOINED_TAG in ev.functions[host][3]
            out.append(Break('fragment-claim', rel, n, hx(a),
                             f'{kind} {hx(a)} lies inside {hx(host)}'
                             + (f' ({JOINED_TAG}: its exception prolog and body joined)'
                                if joined else '')
                             + ' in functions.csv: the tail of its neighbour'))
    return out


def stack_address(stack, ev, dtor):
    own = [a for _, a, _ in stack.anns if not (dtor and ev.kind(a))]
    return min(own or [a for _, a, _ in stack.anns])


def longest_ascending(values):
    """Indices of one longest strictly ascending subsequence of `values` (the earliest-ending
    one on ties), in order."""
    tails, tail_idx, prev = [], [], [None] * len(values)
    for i, v in enumerate(values):
        j = bisect.bisect_left(tails, v)
        if j == len(tails):
            tails.append(v)
            tail_idx.append(i)
        else:
            tails[j] = v
            tail_idx[j] = i
        prev[i] = tail_idx[j - 1] if j else None
    out, k = [], tail_idx[-1] if tail_idx else None
    while k is not None:
        out.append(k)
        k = prev[k]
    return out[::-1]


def check_order(rel, stacks, ev, known=()):
    """The address-order breaks among `stacks` of the unit `rel`. `known` holds the addresses the
    baseline lists for this unit: they are set aside first, so the run of the others, and with
    it what a new break is, does not move as the baseline shrinks. A known address is still a
    break while it sits out of that run's order, and stops being one (the baseline row goes
    stale) once it is moved into place."""
    entries = []
    for s in stacks:
        if s.def_line is None or s.match_only:
            continue
        dtor = s.body is not None and is_destructor(s.header)
        entries.append((stack_address(s, ev, dtor), s))
    rest = [i for i, (a, _) in enumerate(entries) if hx(a) not in known]
    keep = {rest[k] for k in longest_ascending([entries[i][0] for i in rest])}
    out = []
    for i, (a, s) in enumerate(entries):
        if i in keep:
            continue
        before = [entries[j] for j in sorted(keep) if j < i]
        after = [entries[j] for j in sorted(keep) if j > i]
        if hx(a) in known and (not before or before[-1][0] < a) and (not after or a < after[0][0]):
            continue
        where = []
        if before:
            where.append(f'after {hx(before[-1][0])} (line {before[-1][1].first_line})')
        if after:
            where.append(f'before {hx(after[0][0])} (line {after[0][1].first_line})')
        out.append(Break('address-order', rel, s.first_line, hx(a),
                         f'{hx(a)} is out of address order: it sits ' + ' and '.join(where)))
    return out


def check_files(op_root, repo):
    out = []
    for d, dirs, files in os.walk(op_root):
        dirs[:] = sorted(x for x in dirs if not x.startswith('.'))
        for name in sorted(files):
            if name.startswith('.'):
                continue
            p = os.path.join(d, name)
            if name.lower().endswith(SOURCE_EXTS + HEADER_EXTS) or name == 'README.md':
                continue
            if d == op_root and name in ROOT_TABLES:
                continue
            rel = os.path.relpath(p, repo).replace(os.sep, '/')
            out.append(Break('non-source', rel, 1, '-',
                             'not a source, header, README.md or one of op/\'s tables'))
    return out


def lint(repo, op_root, ev, baseline=()):
    """Every break under op_root, sorted by path and line. `baseline` holds the baseline's
    (check, path, address) keys, which check_order sets aside."""
    out = check_files(op_root, repo)
    for d, dirs, files in os.walk(op_root):
        dirs[:] = sorted(x for x in dirs if not x.startswith('.'))
        for name in sorted(files):
            low = name.lower()
            if name.startswith('.') or not low.endswith(SOURCE_EXTS + HEADER_EXTS):
                continue
            p = os.path.join(d, name)
            rel = os.path.relpath(p, repo).replace(os.sep, '/')
            with open(p, encoding='latin1') as f:
                text = f.read()
            stacks = read_stacks(text)
            for s in stacks:
                out += check_empty(rel, s, ev)
                out += check_fragments(rel, s, ev)
            if low.endswith(SOURCE_EXTS):
                known = {k[2] for k in baseline if k[0] == 'address-order' and k[1] == rel}
                out += check_order(rel, stacks, ev, known)
    return sorted(out, key=lambda b: (b.path, b.line, b.check))


def baseline_text(breaks, old):
    rows = ['\t'.join(BASELINE_COLUMNS)]
    seen = set()
    for b in sorted(breaks, key=lambda b: (b.check, b.path, b.address)):
        if b.key in seen:
            continue
        seen.add(b.key)
        reason = old.get(b.key) or b.message
        rows.append('\t'.join((b.check, b.path, b.address, reason.replace('\t', ' '))))
    return '\n'.join(rows) + '\n'


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--repo', default=REPO, help='repository root (default: this checkout)')
    ap.add_argument('--op', help='source tree (default: <repo>/op)')
    ap.add_argument('--matched-set', help='default: <repo>/analysis/results/matched_set.json')
    ap.add_argument('--boundaries', help='default: <repo>/analysis/results/boundary_errors.tsv')
    ap.add_argument('--functions', help='default: <repo>/ghidra/exports/functions.csv')
    ap.add_argument('--baseline', help='default: <repo>/analysis/results/lint_baseline.tsv')
    ap.add_argument('--all', action='store_true', help='also print the breaks the baseline holds')
    ap.add_argument('--write-baseline', action='store_true',
                    help='rewrite the baseline to the breaks found now, keeping existing reasons')
    args = ap.parse_args(argv)
    repo = os.path.abspath(args.repo)
    res = os.path.join(repo, 'analysis', 'results')
    op_root = os.path.abspath(args.op or os.path.join(repo, 'op'))
    baseline_path = args.baseline or os.path.join(res, 'lint_baseline.tsv')
    try:
        ev = Evidence(load_matched(args.matched_set or os.environ.get('HARNESS_MATCHED_SET')
                                   or os.path.join(res, 'matched_set.json')),
                      load_boundaries(args.boundaries or os.path.join(res, 'boundary_errors.tsv')),
                      load_functions(args.functions
                                     or os.path.join(repo, 'ghidra', 'exports', 'functions.csv')))
        baseline = load_baseline(baseline_path)
    except (OSError, ValueError, KeyError) as e:
        print(f'lint_claims: cannot read an input: {e}', file=sys.stderr)
        return 2
    if not os.path.isdir(op_root):
        print(f'lint_claims: no source tree at {op_root}', file=sys.stderr)
        return 2
    breaks = lint(repo, op_root, ev, () if args.write_baseline else baseline)
    if args.write_baseline:
        with open(baseline_path, 'w', newline='') as f:
            f.write(baseline_text(breaks, baseline))
        print(f'lint_claims: wrote {len({b.key for b in breaks})} rows to {baseline_path}')
        return 0
    found = {b.key for b in breaks}
    new = [b for b in breaks if b.key not in baseline]
    known = [b for b in breaks if b.key in baseline]
    stale = sorted(k for k in baseline if k not in found)
    for b in new:
        print(b)
    if args.all:
        for b in known:
            print(f'{b} (in the baseline)')
    for check, path, address in stale:
        print(f'{path}: {check}: {address} is in the baseline but no longer occurs; '
              'delete its row from the baseline')
    counts = ', '.join(f'{c} {sum(1 for b in new if b.check == c)}' for c in CHECKS)
    print(f'lint_claims: {len(new)} new ({counts}), {len(known)} in the baseline, '
          f'{len(stale)} stale baseline rows')
    return 1 if new or stale else 0


if __name__ == '__main__':
    sys.exit(main())
