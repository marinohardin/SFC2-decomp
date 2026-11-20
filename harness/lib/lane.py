"""Is this checkout a grind lane? A lane commits source only (harness/README.md, "Lanes commit
source only"), so its manifest and matched set stay as on develop until the landing step.

One order, used by scripts/check.sh and tests/test_manifest.py alike:
  1. SFCOP_LANE=1 (the loop exports it);
  2. GITHUB_HEAD_REF starts with grind/ (CI checks out a detached merge commit, where the branch
     name reads HEAD; GitHub sets this on pull_request events);
  3. the checked-out branch starts with grind/.

`python3 -m harness.lib.lane` exits 0 on a lane and 1 otherwise.
"""
import os
import subprocess
import sys

PREFIX = 'grind/'


def on_lane(environ=None, branch=None):
    env = os.environ if environ is None else environ
    if env.get('SFCOP_LANE') == '1':
        return True
    if env.get('GITHUB_HEAD_REF', '').startswith(PREFIX):
        return True
    if branch is None:
        repo = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        branch = subprocess.run(['git', '-C', repo, 'rev-parse', '--abbrev-ref', 'HEAD'],
                                capture_output=True, text=True).stdout.strip()
    return branch.startswith(PREFIX)


if __name__ == '__main__':
    sys.exit(0 if on_lane() else 1)
