"""Remove the build folders of git worktrees that no longer exist.

    python3 -m harness.lib.clean [--dry-run] [--unrecorded] [--root <work/harness>]

Every checkout keeps its state in work/harness/wt-<name>-<hash>/ and its build output, several GB
after a full run, in the build/ folder there. Nothing removes it when the worktree goes. The
harness records the worktree's path in each state folder (worktree.txt, `record_worktree`); this
deletes only the build/ folder of a state folder whose recorded worktree is gone. attempts/,
queue.json, cache/ and everything else stay. A folder with no record is left alone unless
--unrecorded is given. Nothing calls this by itself.
"""
import argparse
import os
import shutil
import sys

from . import paths

RECORD = 'worktree.txt'


def record_worktree(state=None, repo=None):
    """Write the path of the worktree using `state` into its record file, if it is not there."""
    state, repo = state or paths.STATE, repo or paths.REPO
    path = os.path.join(state, RECORD)
    try:
        with open(path) as f:
            if f.read().strip() == repo:
                return
    except OSError:
        pass
    try:
        os.makedirs(state, exist_ok=True)
        with open(path, 'w') as f:
            f.write(repo + '\n')
    except OSError:
        pass


def tree_size(path):
    total = 0
    for d, _, files in os.walk(path):
        for n in files:
            try:
                total += os.lstat(os.path.join(d, n)).st_size
            except OSError:
                pass
    return total


def recorded_worktree(folder):
    try:
        with open(os.path.join(folder, RECORD)) as f:
            return f.read().strip() or None
    except OSError:
        return None


def orphan_builds(root, unrecorded=False):
    """(state folder, its build folder, why) for each build/ that may be removed."""
    found = []
    for name in sorted(os.listdir(root)) if os.path.isdir(root) else []:
        folder = os.path.join(root, name)
        build = os.path.join(folder, 'build')
        if not name.startswith('wt-') or not os.path.isdir(folder):
            continue
        if os.path.islink(build) or not os.path.isdir(build):
            continue
        wt = recorded_worktree(folder)
        if wt is None:
            if unrecorded:
                found.append((folder, build, 'no record'))
        elif not os.path.exists(wt):
            found.append((folder, build, f'worktree gone: {wt}'))
    return found


def clean(root, dry_run=False, unrecorded=False, out=sys.stdout):
    """Remove (or with dry_run, list) the orphaned build folders; returns the bytes freed."""
    freed = 0
    for folder, build, why in orphan_builds(root, unrecorded):
        size = tree_size(build)
        out.write(f"{'would remove' if dry_run else 'removed'} {build} ({size} bytes) {why}\n")
        if not dry_run:
            shutil.rmtree(build, ignore_errors=True)
        freed += size
    out.write(f"{'would free' if dry_run else 'freed'} {freed} bytes\n")
    return freed


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--dry-run', action='store_true', help='list what would go, remove nothing')
    ap.add_argument('--unrecorded', action='store_true',
                    help='also remove build/ of folders that have no worktree record')
    ap.add_argument('--root', default=os.path.join(paths.WORK, 'harness'),
                    help='the folder holding the wt-* state folders')
    a = ap.parse_args(argv)
    clean(a.root, a.dry_run, a.unrecorded)
    return 0


if __name__ == '__main__':
    sys.exit(main())
