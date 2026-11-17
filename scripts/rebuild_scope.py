#!/usr/bin/env python3
"""Whether a change needs the matched-set rebuild in `scripts/check.sh`.

    python3 scripts/rebuild_scope.py BASE

Prints `rebuild` or `skip` for the files that differ between BASE and the working tree, untracked
files included. `skip` only when every changed file is documentation or repository metadata that
no build, harness or gate reads; any other file, or no changed file at all, is `rebuild`.
"""
import fnmatch
import subprocess
import sys

# Paths that never reach a compile, a score or a gate. A pattern without a slash matches the file
# name in any folder; one with a slash matches the whole path from the repository root.
# Images are never read by a build. The progress output is written by scripts/progress.py and
# tools/progress_chart.py and read back only from the base branch, by the progress gate, which runs
# whatever this says; the rebuild never reads it.
DOC_ONLY = (
    '*.md',
    '*.svg',
    '*.png',
    '*.jpg',
    '*.gif',
    'analysis/results/progress/*',
    'docs/*',
    '.claude/*',
    '.github/pull_request_template.md',
    '.github/CODEOWNERS',
    '.github/hotspots.txt',
    'LICENSE*',
)


def is_doc_only(path):
    """True when `path` (relative to the repository root, `/`-separated) needs no rebuild."""
    name = path.rsplit('/', 1)[-1]
    for pattern in DOC_ONLY:
        if '/' in pattern:
            if fnmatch.fnmatchcase(path, pattern):
                return True
        elif pattern.startswith('LICENSE'):
            if '/' not in path and fnmatch.fnmatchcase(path, pattern):
                return True
        elif fnmatch.fnmatchcase(name, pattern):
            return True
    return False


def classify(paths):
    """`skip` when there is at least one path and every one is doc-only, else `rebuild`."""
    paths = [p for p in paths if p]
    if paths and all(is_doc_only(p) for p in paths):
        return 'skip'
    return 'rebuild'


def changed_paths(base):
    def lines(*args):
        out = subprocess.run(['git', *args], check=True, capture_output=True, text=True).stdout
        return [l for l in out.splitlines() if l]
    return lines('diff', '--name-only', base) + lines('ls-files', '--others', '--exclude-standard')


def main(argv):
    if len(argv) != 2:
        print(__doc__.strip(), file=sys.stderr)
        return 2
    print(classify(changed_paths(argv[1])))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
