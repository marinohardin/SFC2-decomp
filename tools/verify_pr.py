#!/usr/bin/env python3
"""Re-verify an outside contributor's pull request with develop's own judge (the build review).

    python3 tools/verify_pr.py <number> --sha <head sha> [--report FILE]

Run by the maintainer from a checkout of develop. Nothing from the pull request is run or
imported: its op/ files are copied in as text and compiled by develop's harness.

1. Fetches develop and pull/<number>/head from the remote, and refuses (exit 3) when the head is
   not --sha.
2. Runs tools/contrib_guard.py's checks on the diff (the same code), and refuses on a failure.
3. Makes a detached worktree of <remote>/develop at $SFCOP_WORK/verify/<number>, and writes each
   changed op/ file into it with `git show <sha>:<path>`, mode 644; deleted files are deleted.
4. In that worktree, under `nice -n 5`, with no cache and a harness state of its own:
   `sh harness/build-and-verify.sh --all --base <remote>/develop <the pull request's addresses>`,
   `python3 tools/lint_claims.py`, and `python3 -m harness.lib.symsync --scratch` (the symbol
   table claimed and checked on a copy, as the landing step will).
5. Writes a Markdown report (default $SFCOP_WORK/verify/<number>-report.md): the SHAs, each
   function the pull request annotates with its old and new score, regressions, each command's
   exit status, and "claimed N exact, verified M" from the evidence block in the description.
   Escape characters are stripped and the home directory masked.
6. Removes the worktree and its harness state (about 4 GB of build output), keeping the report
   and the harness output beside it.

The description and the claims come from `gh` (read-only), or from --body and --claims files.
Exit 0 when every command passed, 1 when one failed or a function regressed, 2 on a usage or git
error, 3 when the pull request is refused.
"""
import argparse
import json
import os
import re
import shutil
import subprocess
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, TOOLS)
import contrib_guard  # noqa: E402
import evidence  # noqa: E402

ESCAPES = re.compile(r'\x1b\[[0-?]*[ -/]*[@-~]|\x1b[@-_]|[\x00-\x08\x0b-\x1f\x7f]')
SCORE_LINE = re.compile(r'^(?:OK|ERROR|WARN)\s+(0x[0-9a-f]{8}) (.+?) (\d+\.\d+)% (exact|partial)\b')
CLAIMED = re.compile(r'Claimed exact:\s*(\d+)(?:\s*\(([^)]*)\))?', re.I)
# Kept from the caller's environment: what picks the compiler and the exe, nothing that moves
# the state, the matched set or the source root.
KEEP = ('HARNESS_COMPILER', 'HARNESS_JOBS', 'HARNESS_EXE')


def clean(text):
    return ESCAPES.sub('', text.replace('\r\n', '\n'))


def git(repo, *args):
    return contrib_guard.git(repo, *args)


def work_dir():
    if os.environ.get('SFCOP_WORK'):
        return os.path.abspath(os.environ['SFCOP_WORK'])
    sys.path.insert(0, os.path.dirname(TOOLS))
    from harness.lib import paths
    return paths.WORK


def harness_env(state):
    """The judge's environment: no harness override but KEEP, and no token (the compiler reads
    the contributor's source)."""
    env = {k: v for k, v in os.environ.items()
           if not (k.startswith('HARNESS_') or k.startswith('SFCOP_LANE') or 'TOKEN' in k
                   or k == 'SFCOP_BASE_REF') or k in KEEP}
    env.update(HARNESS_NO_CACHE='1', HARNESS_STATE=state)
    return env


def commands(base, addresses):
    """[(label, argv)]: the judge's three steps."""
    py = sys.executable or 'python3'
    nice = ['nice', '-n', '5']
    return [
        ('harness/build-and-verify.sh --all --base ' + base,
         nice + ['sh', 'harness/build-and-verify.sh', '--all', '--base', base,
                 *[f'0x{a:08x}' for a in sorted(addresses)]]),
        ('tools/lint_claims.py', nice + [py, 'tools/lint_claims.py']),
        ('harness.lib.symsync --scratch', nice + [py, '-m', 'harness.lib.symsync', '--scratch']),
    ]


def scores(text):
    """{address: (name, score, exact)} from a harness output."""
    out = {}
    for line in text.splitlines():
        m = SCORE_LINE.match(line)
        if m:
            out[int(m.group(1), 16)] = (m.group(2), float(m.group(3)), m.group(4) == 'exact')
    return out


def claimed(body):
    """(N, [addresses]) from the evidence block's `Claimed exact:` line, or (None, [])."""
    m = CLAIMED.search(body or '')
    if not m:
        return None, []
    addrs = [int(x, 16) for x in re.findall(r'0x([0-9a-fA-F]{1,8})', m.group(2) or '')]
    return int(m.group(1)), addrs


def base_functions(repo, base):
    try:
        text = git(repo, 'show', f'{base}:analysis/results/matched_set.json')
        return json.loads(text).get('functions', {})
    except (contrib_guard.GitError, ValueError):
        return {}


def report(number, sha, base_sha, mb, res, runs, old, body):
    """The report's Markdown text."""
    out = [f'## Build review of #{number}', '',
           f'- Head: {sha}', f'- develop: {base_sha}', f'- Merge base: {mb}', '']
    build = runs[0][2] if runs else ''
    new = scores(build)
    out += ['| Address | Name | Old | New | Exact |', '| --- | --- | --- | --- | --- |']
    for a in sorted(res.added):
        o = old.get(f'0x{a:08x}')
        name, score, exact = new.get(a, ('(not scored)', None, False))
        was = '%.2f%%' % o['score'] if o else '-'
        now = '%.2f%%' % score if score is not None else '-'
        out.append(f'| 0x{a:08x} | {name} | {was} | {now} | {"yes" if exact else "no"} |')
    for a in sorted(res.removed - res.added):
        out.append(f'| 0x{a:08x} | (annotation removed) | - | - | - |')
    regress = [l for l in build.splitlines() if 'REGRESSION' in l]
    out += ['', f'Regressions: {len(regress)}'] + [f'    {l}' for l in regress[:50]]
    out += ['', '| Command | Exit |', '| --- | --- |']
    for label, code, _ in runs:
        out.append(f'| `{label}` | {code} |')
    result = [l for l in build.splitlines() if l.startswith('RESULT ')]
    out.append('')
    out.append(result[-1] if result else 'RESULT line: none')
    n, addrs = claimed(body)
    verified = sorted(a for a in res.added if new.get(a, (0, 0, False))[2])
    if n is None:
        out.append(f'Claimed: no `Claimed exact:` line in the description; verified {len(verified)} exact')
    else:
        out.append(f'Claimed {n} exact, verified {len(verified)}')
        short = [a for a in addrs if a not in verified]
        if short:
            out.append('Claimed exact but not verified: ' + ', '.join(f'0x{a:08x}' for a in short))
    return evidence.mask(clean('\n'.join(out) + '\n'))


def remove_worktree(repo, tree, state):
    subprocess.run(['git', 'worktree', 'remove', '--force', tree], cwd=repo, capture_output=True)
    shutil.rmtree(tree, ignore_errors=True)
    shutil.rmtree(state, ignore_errors=True)
    subprocess.run(['git', 'worktree', 'prune'], cwd=repo, capture_output=True)


def gh_body(number, repo):
    cmd = ['gh', 'pr', 'view', str(number), '--json', 'body', '-q', '.body']
    if repo:
        cmd += ['--repo', repo]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        raise contrib_guard.GitError(f'gh pr view {number}: {r.stderr.strip()}')
    return r.stdout


def verify(repo, number, sha, remote='origin', body=None, issues=None, gh_repo=None,
           report_path=None, keep=False, log=print):
    """Steps 1 to 6; returns the exit status."""
    base = f'{remote}/develop'
    git(repo, 'fetch', '-q', remote, f'+refs/heads/develop:refs/remotes/{remote}/develop')
    git(repo, 'fetch', '-q', remote, f'pull/{number}/head')
    head = git(repo, 'rev-parse', 'FETCH_HEAD').strip()
    if head != sha:
        log(f'verify: refused: pull/{number}/head is {head}, not {sha}')
        return 3
    if body is None:
        body = gh_body(number, gh_repo)
    res = contrib_guard.check(repo, base, sha, body, issues, gh_repo)
    if res.failures:
        for f in res.failures:
            log(f'FAIL {f}')
        log('verify: refused by the guard')
        return 3
    work = os.path.join(work_dir(), 'verify')
    os.makedirs(work, exist_ok=True)
    tree = os.path.join(work, str(number))
    state = tree + '.state'
    report_path = report_path or tree + '-report.md'
    remove_worktree(repo, tree, state)
    git(repo, 'worktree', 'add', '-q', '--detach', tree, base)
    try:
        for c in res.changes:
            dest = os.path.join(tree, c.path)
            if not os.path.realpath(dest).startswith(os.path.realpath(tree) + os.sep):
                raise contrib_guard.GitError(f'{c.path}: outside the worktree')
            if c.deleted:
                if os.path.lexists(dest):
                    os.remove(dest)
                continue
            data = subprocess.run(['git', 'show', f'{sha}:{c.path}'], cwd=repo,
                                  capture_output=True, check=True).stdout
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            with open(dest, 'wb') as f:
                f.write(data)
            os.chmod(dest, 0o644)
        env = harness_env(state)
        runs = []
        for label, cmd in commands(base, res.added):
            log('verify: ' + label)
            r = subprocess.run(cmd, cwd=tree, env=env, capture_output=True, text=True,
                               errors='replace')
            runs.append((label, r.returncode, clean(r.stdout + r.stderr)))
        with open(os.path.splitext(report_path)[0] + '-harness.log', 'w') as f:
            f.write(evidence.mask('\n'.join(f'$ {label}\n{o}' for label, _, o in runs)))
        text = report(number, sha, git(repo, 'rev-parse', base).strip(), res.merge_base, res,
                      runs, base_functions(repo, base), body)
        with open(report_path, 'w') as f:
            f.write(text)
        log(text)
        log(f'verify: report in {evidence.mask(report_path)}')
        failed = any(code != 0 for _, code, _ in runs)
        return 1 if failed else 0
    finally:
        if not keep:
            remove_worktree(repo, tree, state)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('number', type=int)
    ap.add_argument('--sha', required=True, help='the head commit you reviewed')
    ap.add_argument('--report', help='where to write the report')
    ap.add_argument('--remote', default='origin')
    ap.add_argument('--repo', help='owner/name for gh (default: the current repository)')
    ap.add_argument('--body', help='a file holding the description, instead of gh')
    ap.add_argument('--claims', help='a JSON file of claim issues, instead of gh')
    ap.add_argument('--keep', action='store_true', help='keep the worktree afterwards')
    a = ap.parse_args(argv)
    if not re.fullmatch(r'[0-9a-f]{40}', a.sha):
        ap.error('--sha must be a full 40-character commit SHA')
    body = issues = None
    if a.body:
        with open(a.body, encoding='utf-8', errors='replace') as f:
            body = f.read()
    if a.claims:
        with open(a.claims, encoding='utf-8') as f:
            issues = json.load(f)
    repo = git(os.getcwd(), 'rev-parse', '--show-toplevel').strip()
    try:
        return verify(repo, a.number, a.sha, a.remote, body, issues, a.repo,
                      os.path.abspath(a.report) if a.report else None, a.keep)
    except (contrib_guard.GitError, subprocess.CalledProcessError) as e:
        print(f'verify: {e}')
        return 2


if __name__ == '__main__':
    sys.exit(main())
