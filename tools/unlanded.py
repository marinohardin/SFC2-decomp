#!/usr/bin/env python3
"""List the local branches that hold exact functions the base branch does not.

A grind lane commits source only: each of its commits for a function it matched carries a
`Harness-Score: <module> <address> exact` trailer (harness/loop.sh), and its matched set lives
outside the repository. Until that branch is merged, the work exists nowhere else, and a branch
that is never turned into a pull request is lost work that nothing reports. This lists every local
branch with a function exact that the base's matched set does not have exact: one its committed
matched set has exact (a branch from before lanes committed source only), or one named by a
trailer in a commit on `<base>..<branch>`. A function has landed once the base's matched set has
it exact, which a squash merge keeps true.

    python3 tools/unlanded.py                      # every local branch, against origin/develop
    python3 tools/unlanded.py --base develop grind/ wp/   # only branches with these prefixes
    python3 tools/unlanded.py --disposed grind/tiger-e     # a branch with a written disposition

Run `git fetch` first: the base is read as it is locally, and a stale `origin/develop` lists
branches that have already landed. A function that was exact on the base and has since been
released there makes every older branch that still holds it look unlanded; that is a false
positive, and the listing names the addresses to check.

A branch named with `--disposed` is one whose unlanded functions have a written disposition
(AGENTS.md, "Rules that are easy to get wrong"); it is still listed, marked as disposed, and does
not count. Exit 0 when no other branch holds unlanded exact functions, 1 when one does (each is
listed), 2 on a usage or git error, or a matched set that is not valid JSON. A batch is closed,
and the next one started, only when this exits 0.
"""

import argparse
import json
import re
import subprocess
import sys

MATCHED_SET = 'analysis/results/matched_set.json'
# A value of a Harness-Score trailer, as git's own parser prints it: `<module> <address> exact`.
EXACT_VALUE = re.compile(r'^\S+\s+(0x[0-9a-fA-F]{1,8})\s+exact\s*$')


def unlanded(base_functions, branch_functions):
    """The addresses exact in `branch_functions` and not exact in `base_functions`, sorted."""
    return sorted(k for k, v in branch_functions.items()
                  if v.get('exact') and not base_functions.get(k, {}).get('exact'))


def git(*args, cwd=None):
    return subprocess.run(['git', *args], cwd=cwd, capture_output=True, text=True)


def functions_at(ref, cwd=None):
    """The matched set's functions at `ref`, or {} when the ref has no matched set."""
    r = git('show', f'{ref}:{MATCHED_SET}', cwd=cwd)
    if r.returncode != 0:
        return {}
    try:
        return json.loads(r.stdout).get('functions', {})
    except ValueError as e:
        raise RuntimeError(f'{ref}:{MATCHED_SET} is not valid JSON ({e})') from e


def trailer_exact(base, branch, cwd=None):
    """The functions (matched-set keys) that commits in `base..branch` record exact in a
    Harness-Score trailer."""
    r = git('log', '--format=%(trailers:key=Harness-Score,valueonly)%x00', f'{base}..{branch}',
            cwd=cwd)
    if r.returncode != 0:
        # Never "no trailers": that would report the lane as landed.
        raise RuntimeError(f'git log {base}..{branch} failed: {r.stderr.strip()}')
    found = (EXACT_VALUE.match(v.strip()) for v in r.stdout.replace('\0', '\n').splitlines())
    return {f'0x{int(m.group(1), 16):08x}': {'exact': True} for m in found if m}


def local_branches(prefixes, cwd=None):
    r = git('for-each-ref', '--format=%(refname:short)', 'refs/heads', cwd=cwd)
    if r.returncode != 0:
        raise RuntimeError(r.stderr.strip())
    names = [n for n in r.stdout.split('\n') if n]
    return [n for n in names if not prefixes or n.startswith(tuple(prefixes))]


def report(base, prefixes, cwd=None, out=None, disposed=()):
    """Print one line per branch with unlanded exact functions; return how many branches, not
    counting those in `disposed`."""
    out = out or sys.stdout
    base_functions = functions_at(base, cwd)
    if not base_functions:
        raise RuntimeError(f'{base} has no {MATCHED_SET}')
    found = 0
    for branch in local_branches(prefixes, cwd):
        claimed = trailer_exact(base, branch, cwd)
        claimed.update({k: v for k, v in functions_at(branch, cwd).items() if v.get('exact')})
        addresses = unlanded(base_functions, claimed)
        if not addresses:
            continue
        held = branch in disposed
        found += not held
        pushed = bool(git('branch', '-r', '--contains', branch, cwd=cwd).stdout.strip())
        shown = ' '.join(addresses[:3]) + (' ...' if len(addresses) > 3 else '')
        print(f'{branch}: {len(addresses)} exact not in {base} '
              f'({"pushed" if pushed else "not pushed"}{", disposed" if held else ""}): {shown}',
              file=out)
    return found


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    p.add_argument('--base', default='origin/develop', help='the branch work lands on')
    p.add_argument('--disposed', action='append', default=[], metavar='BRANCH',
                   help='a branch whose unlanded functions have a written disposition')
    p.add_argument('prefixes', nargs='*', help='only branches whose names start with these')
    a = p.parse_args(argv)
    try:
        found = report(a.base, a.prefixes, disposed=set(a.disposed))
    except RuntimeError as e:
        print(f'unlanded: {e}', file=sys.stderr)
        return 2
    if found:
        print(f'unlanded: {found} branch(es) hold exact functions {a.base} does not have')
        return 1
    print(f'unlanded: no local branch holds exact functions {a.base} does not have')
    return 0


if __name__ == '__main__':
    sys.exit(main())
