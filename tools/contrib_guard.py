#!/usr/bin/env python3
"""Check an outside contributor's pull request by reading its diff (CONTRIBUTING.md).

    python3 tools/contrib_guard.py <base> <head> --body body.md [--claims issues.json]

It reads git objects only: nothing from the pull request is run or imported. It fails, listing
each reason, when the pull request (the changes from the merge base of <base> and <head> to
<head>):

- changes a path outside op/, or changes op/port/, op/UNITS.tsv or op/manifest.tsv, or a file
  under op/ that is not source (.cpp, .h, .inl: the suffixes op/ holds);
- changes a symlink, a submodule or a file mode, or adds a binary file or one over 256 KB;
- adds an #include that is absolute, names a drive letter, uses `..` to leave op/ or names no
  file (a macro), or adds an #import, a #line, a #pragma include_alias, or a #pragma or _Pragma
  holding a quoted string (a path the compiler could open);
- adds a line ending in a backslash: a splice can join a line to the next one past every check
  here, so a contributor writes a multi-line macro on one line, or asks the maintainer;
- adds, changes or removes a `// FUNCTION:`, `// STUB:` or `// VTABLE:` address outside its
  claim's ranges;
- names no claim: the body needs a line `Claim: #<issue>`, and that issue (open, labelled
  `claim`, opened by the pull request's author) holds one or more lines
  `range: 0x<first>-0x<last>`, each inside one unit of the base's op/UNITS.tsv that the issue
  names (`op/<unit>`), and none overlapping an open `reserved` issue.

What it binds is annotation addresses: it does not see an edit to the body of an unclaimed
function or to a shared header. The maintainer's build review (tools/verify_pr.py, which
rebuilds every claim) and reading of the diff catch those.

The claim issues come from `--claims`, a JSON list of issues as `gh api` prints them, or else
from `gh api repos/$GITHUB_REPOSITORY/issues/<n>`. With `--pr <number>` and `gh` it also warns,
without failing, when another open pull request annotates one of the same addresses.

Exit 0 when nothing fails, 1 when something does, 2 on a usage or git error.
"""
import argparse
import json
import os
import posixpath
import re
import subprocess
import sys

MAX_BYTES = 256 * 1024
FORBIDDEN = ('op/port/',)
FORBIDDEN_FILES = ('op/UNITS.tsv', 'op/manifest.tsv')
SOURCE_SUFFIXES = ('.cpp', '.h', '.inl')
CLAIM_LINE = re.compile(r'^\s*Claim:\s*#(\d+)\s*$', re.M | re.I)
RANGE_LINE = re.compile(r'^\s*range:\s*0x([0-9a-fA-F]{1,8})\s*-\s*0x([0-9a-fA-F]{1,8})\s*$', re.M)
# Any claim line: the same pattern as CLAIM_LINE in harness/lib/guard.py (and ANNOTATION in
# tools/manifest.py), kept here so the guard imports no harness code.
ANNOTATION = re.compile(r'^\s*//\s*(FUNCTION|STUB|VTABLE):\s*(\w+)\s+0x([0-9A-Fa-f]{1,8})\s*$')
INCLUDE = re.compile(r'^\s*#\s*include\b\s*(.*)$')
IMPORT = re.compile(r'^\s*#\s*import\b')
LINE_DIRECTIVE = re.compile(r'^\s*#\s*line\b')
PRAGMA = re.compile(r'^\s*#\s*pragma\b(.*)$|\b_Pragma\s*\((.*)$')
CONTRIBUTING = 'see CONTRIBUTING.md, "Find and claim work"'


class GitError(Exception):
    pass


def git(cwd, *args, binary=False):
    r = subprocess.run(['git', *args], cwd=cwd, capture_output=True, text=not binary)
    if r.returncode != 0:
        err = r.stderr if not binary else r.stderr.decode('utf-8', 'replace')
        raise GitError(f'git {" ".join(args)}: {err.strip()}')
    return r.stdout


class Change:
    """One changed path: git's raw diff fields."""
    def __init__(self, old_mode, new_mode, new_blob, status, path):
        self.old_mode, self.new_mode, self.new_blob = old_mode, new_mode, new_blob
        self.status, self.path = status, path

    @property
    def deleted(self):
        return self.status == 'D'


def changes(cwd, base, head):
    """The changed paths from the merge base of `base` and `head` to `head`, renames split."""
    mb = git(cwd, 'merge-base', base, head).strip()
    out = git(cwd, 'diff', '--raw', '-z', '--no-renames', '--no-abbrev', mb, head)
    parts = out.split('\0')
    result = []
    for i in range(0, len(parts) - 1, 2):
        meta, path = parts[i], parts[i + 1]
        old_mode, new_mode, _old, new_blob, status = meta.lstrip(':').split()
        result.append(Change(old_mode, new_mode, new_blob, status[0], path))
    return mb, result


def binary_paths(cwd, mb, head):
    out = git(cwd, 'diff', '--numstat', '-z', '--no-renames', mb, head)
    return {p.split('\t', 2)[2] for p in out.split('\0') if p.startswith('-\t-\t')}


def diff_lines(cwd, mb, head, path):
    """The (added, removed) lines of one file's diff."""
    out = git(cwd, 'diff', '-U0', '--no-renames', mb, head, '--', path, binary=True)
    added, removed = [], []
    header = True   # a file's header lines come before its first @@; +++ and --- are only there
    for raw in out.decode('utf-8', 'replace').split('\n'):
        if raw.startswith('diff --git '):
            header = True
        elif raw.startswith('@@'):
            header = False
        if header:
            continue
        if raw.startswith('+'):
            added.append(raw[1:])
        elif raw.startswith('-'):
            removed.append(raw[1:])
    return added, removed


def include_problem(path, line):
    """Why an added line is refused, or None."""
    if line.rstrip('\r').endswith('\\'):
        return ('adds a line ending in a backslash (a line splice); write a multi-line macro on '
                'one line in a contributor pull request, or ask the maintainer')
    if IMPORT.match(line):
        return 'adds an #import'
    if LINE_DIRECTIVE.match(line):
        return 'adds a #line directive; ask the maintainer'
    m = PRAGMA.search(line)
    if m:
        rest = m.group(1) if m.group(1) is not None else m.group(2)
        if 'include_alias' in rest or '"' in rest:
            return f'adds a #pragma that can name a file: {line.strip()!r}'
    m = INCLUDE.match(line)
    if not m:
        return None
    spec = m.group(1).strip()
    q = re.match(r'^(?:"([^"]*)"|<([^>]*)>)', spec)
    if not q:
        return f'adds an #include that names no file: {spec!r}'
    name = (q.group(1) if q.group(1) is not None else q.group(2)).replace('\\', '/')
    if name.startswith('/'):
        return f'adds an absolute #include: {name!r}'
    if re.match(r'^[A-Za-z]:', name):
        return f'adds an #include with a drive letter: {name!r}'
    if '..' in name.split('/'):
        where = posixpath.normpath(posixpath.join(posixpath.dirname(path), name))
        if not where.startswith('op/'):
            return f'adds an #include that leaves op/: {name!r}'
    return None


def is_source(path):
    """A path a contributor may change: a source file under op/."""
    return path.startswith('op/') and path.endswith(SOURCE_SUFFIXES)


def parse_ranges(body):
    """The (first, last) address ranges of `range:` lines in an issue body."""
    return [(int(a, 16), int(b, 16)) for a, b in RANGE_LINE.findall(body or '')]


def claim_numbers(body):
    return [int(n) for n in CLAIM_LINE.findall(body or '')]


def labels(issue):
    return {(x['name'] if isinstance(x, dict) else x) for x in issue.get('labels', [])}


def issue_author(issue):
    """The login that opened the issue: `user` from gh api, `author` from gh issue list."""
    who = issue.get('user') or issue.get('author') or {}
    return who.get('login') if isinstance(who, dict) else None


def base_units(cwd, base):
    """[(first, last, 'op/<unit>')] from the base's op/UNITS.tsv (never the pull request's)."""
    try:
        text = git(cwd, 'show', f'{base}:op/UNITS.tsv')
    except GitError:
        return []
    out = []
    for line in text.splitlines()[1:]:
        cells = line.split('\t')
        if len(cells) >= 3:
            out.append((int(cells[1], 16), int(cells[2], 16), 'op/' + cells[0]))
    return out


def fetch_reserved(repo=None):
    repo = repo or os.environ.get('GITHUB_REPOSITORY')
    r = subprocess.run(['gh', 'issue', 'list', '--repo', repo, '--state', 'open', '--label',
                        'reserved', '--limit', '200', '--json', 'number,body,labels,state'],
                       capture_output=True, text=True)
    if r.returncode != 0:
        raise GitError(f'gh issue list --label reserved: {r.stderr.strip()}')
    return json.loads(r.stdout)


def fetch_issue(number, repo=None):
    repo = repo or os.environ.get('GITHUB_REPOSITORY')
    if not repo:
        raise GitError('no --claims file and no GITHUB_REPOSITORY to fetch the claim from')
    r = subprocess.run(['gh', 'api', f'repos/{repo}/issues/{number}'], capture_output=True,
                       text=True)
    if r.returncode != 0:
        return None
    return json.loads(r.stdout)


class Result:
    def __init__(self):
        self.failures, self.warnings = [], []
        self.changes = []
        self.added, self.removed = set(), set()
        self.merge_base = None

    def fail(self, text):
        self.failures.append(text)


def check(cwd, base, head, body, issues=None, repo=None, author=None):
    """Check the pull request `base`...`head` whose description is `body`, opened by `author`.
    `issues` is a list of issue objects (the claims and the open `reserved` issues); None
    fetches them with gh. Returns a Result."""
    res = Result()
    mb, chs = changes(cwd, base, head)
    res.merge_base, res.changes = mb, chs
    binaries = binary_paths(cwd, mb, head)
    for c in chs:
        p = c.path
        if not p.startswith('op/'):
            res.fail(f'{p}: outside op/')
            continue
        if p.startswith(FORBIDDEN) or p in FORBIDDEN_FILES:
            res.fail(f'{p}: op/port/, op/UNITS.tsv and op/manifest.tsv are the maintainer\'s')
            continue
        if not is_source(p):
            res.fail(f'{p}: not a source file ({", ".join(SOURCE_SUFFIXES)})')
            continue
        modes = {c.old_mode, c.new_mode} - {'000000'}
        if '120000' in modes:
            res.fail(f'{p}: a symlink')
            continue
        if '160000' in modes:
            res.fail(f'{p}: a submodule')
            continue
        if modes != {'100644'}:
            res.fail(f'{p}: file mode {c.old_mode} -> {c.new_mode} (only 100644 files)')
        if c.deleted:
            _, removed = diff_lines(cwd, mb, head, p)
            res.removed.update(_addresses(removed))
            continue
        if p in binaries:
            res.fail(f'{p}: a binary file')
            continue
        size = int(git(cwd, 'cat-file', '-s', c.new_blob).strip())
        if size > MAX_BYTES:
            res.fail(f'{p}: {size} bytes, over the {MAX_BYTES} byte limit')
            continue
        added, removed = diff_lines(cwd, mb, head, p)
        for line in added:
            why = include_problem(p, line)
            if why:
                res.fail(f'{p}: {why}')
        res.added.update(_addresses(added))
        res.removed.update(_addresses(removed))
    _check_claims(res, body, issues, repo, author, base_units(cwd, base))
    return res


def _addresses(lines):
    return {int(m.group(3), 16) for m in map(ANNOTATION.match, lines) if m}


def _check_claims(res, body, issues, repo, author, units):
    numbers = claim_numbers(body)
    if not numbers:
        res.fail(f'the description names no claim (a line `Claim: #<issue>`); {CONTRIBUTING}')
        return
    by_number = {int(i['number']): i for i in (issues or [])}
    ranges = []
    for n in numbers:
        issue = by_number.get(n) if issues is not None else fetch_issue(n, repo)
        if not issue:
            res.fail(f'claim #{n}: no such issue')
            continue
        if issue.get('state', 'open').lower() != 'open':
            res.fail(f'claim #{n}: the issue is closed')
            continue
        if 'claim' not in labels(issue):
            res.fail(f'claim #{n}: the issue is not labelled `claim`')
            continue
        if not author or issue_author(issue) != author:
            res.fail(f'claim #{n}: opened by {issue_author(issue) or "unknown"}, not by the pull '
                     f'request\'s author ({author or "unknown"})')
            continue
        got = parse_ranges(issue.get('body'))
        if not got:
            res.fail(f'claim #{n}: the issue has no `range: 0x<first>-0x<last>` line')
        named = set(re.findall(r'(?<![\w/])op/\S+', issue.get('body') or ''))
        for lo, hi in got:
            unit = [u for first, last, u in units if first <= lo and hi <= last]
            if not unit:
                res.fail(f'claim #{n}: range 0x{lo:08x}-0x{hi:08x} is not inside one unit of '
                         'op/UNITS.tsv')
            elif unit[0] not in named:
                res.fail(f'claim #{n}: range 0x{lo:08x}-0x{hi:08x} is in {unit[0]}, which the '
                         'issue does not name')
        ranges += got
    if not ranges:
        return
    reserved = issues if issues is not None else fetch_reserved(repo)
    for r in reserved:
        if 'reserved' not in labels(r) or str(r.get('state', 'open')).lower() != 'open':
            continue
        for lo, hi in parse_ranges(r.get('body')):
            for a, b in ranges:
                if a <= hi and lo <= b:
                    res.fail(f'range 0x{a:08x}-0x{b:08x} overlaps reserved issue #{r["number"]} '
                             f'(0x{lo:08x}-0x{hi:08x})')
    for a in sorted(res.added | res.removed):
        if not any(lo <= a <= hi for lo, hi in ranges):
            what = 'adds or changes' if a in res.added else 'removes'
            res.fail(f'0x{a:08x}: the pull request {what} this annotation, outside the claimed '
                     'ranges')


def overlap_warnings(number, addresses, pull_requests):
    """WARN lines for other open pull requests that annotate one of `addresses`.
    `pull_requests` is [(number, set of annotated addresses)]."""
    out = []
    for n, theirs in pull_requests:
        both = sorted(addresses & theirs)
        if n != number and both:
            out.append(f'pull request #{n} also annotates '
                       + ', '.join(f'0x{a:08x}' for a in both))
    return out


def open_pull_requests(repo):
    """[(number, annotated addresses)] of the open pull requests, through gh."""
    r = subprocess.run(['gh', 'pr', 'list', '--repo', repo, '--state', 'open', '--limit', '200',
                        '--json', 'number'], capture_output=True, text=True)
    if r.returncode != 0:
        return []
    out = []
    for pr in json.loads(r.stdout):
        d = subprocess.run(['gh', 'pr', 'diff', str(pr['number']), '--repo', repo],
                           capture_output=True, text=True)
        if d.returncode == 0:
            lines = [x[1:] for x in d.stdout.splitlines() if x.startswith('+')]
            out.append((pr['number'], _addresses(lines)))
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('base')
    ap.add_argument('head')
    ap.add_argument('--body', help='a file holding the pull request description')
    ap.add_argument('--claims', help='a JSON file: the claim issues, as gh api prints them')
    ap.add_argument('--repo', help='owner/name (default $GITHUB_REPOSITORY)')
    ap.add_argument('--pr', type=int, help='this pull request, for the overlap warning')
    ap.add_argument('--author', help="the pull request author's login")
    a = ap.parse_args(argv)
    body = ''
    if a.body:
        with open(a.body, encoding='utf-8', errors='replace') as f:
            body = f.read()
    issues = None
    if a.claims:
        with open(a.claims, encoding='utf-8') as f:
            issues = json.load(f)
    try:
        res = check(os.getcwd(), a.base, a.head, body, issues, a.repo, a.author)
    except GitError as e:
        print(f'guard: {e}')
        return 2
    repo = a.repo or os.environ.get('GITHUB_REPOSITORY')
    if a.pr and repo:
        res.warnings += overlap_warnings(a.pr, res.added, open_pull_requests(repo))
    for line in res.failures:
        print(f'FAIL {line}')
    for line in res.warnings:
        print(f'WARN {line}')
    if res.failures:
        print(f'guard: {len(res.failures)} problems; {CONTRIBUTING}')
        return 1
    print(f'guard: ok ({len(res.changes)} files, {len(res.added)} annotations added or changed)')
    return 0


if __name__ == '__main__':
    sys.exit(main())
