"""Whole-tree checks after a function session (harness/README.md, "Whole-tree checks").

The Stop hook (harness/hooks/stop_verify.py) runs them once the function itself is judged
good, and harness/loop.sh runs them again before it commits the round, so the session that
caused a problem is told while it still has the context. Four checks, each the tool's own:

  verify    build-and-verify.sh --all --json: a regression or an ERROR in any unit
  manifest  tools/manifest.py, written to a scratch file so the tree is left alone
  lint      tools/lint_claims.py: a new finding (a stale baseline row is not the session's)
  tests     tests/test_units.py and tests/test_manifest.py, except the one test that compares the
            committed manifest with the matched set: the loop never regenerates the manifest

HARNESS_WHOLETREE=0 turns all of it off. `python3 -m harness.lib.wholetree` prints the failures
and exits 1, or prints nothing and exits 0.
"""
import json
import os
import re
import subprocess
import sys

from . import paths

MAX_LINES = 15
STALE = re.compile(r'is in the baseline but no longer occurs')
SUMMARY = re.compile(r'^lint_claims: (\d+) new')
ADVICE = ('Fix these in the source of your own function (or undo the edit that caused them) and '
          'stop again; do not edit another function or another lane\'s file.')


def enabled(environ=None):
    return (os.environ if environ is None else environ).get('HARNESS_WHOLETREE', '1') != '0'


def run_command(argv, env=None):
    """(exit code, stdout, stderr) of argv, run from the repository root."""
    e = dict(os.environ, PYTHONPATH=paths.REPO, PYTHONDONTWRITEBYTECODE='1')
    e.update(env or {})
    p = subprocess.run(argv, capture_output=True, text=True, cwd=paths.REPO, env=e)
    return p.returncode, p.stdout, p.stderr


def _trim(lines, keep=None):
    """At most MAX_LINES of lines: those `keep` accepts, else the last ones."""
    lines = [x.rstrip() for x in lines if x.strip()]
    picked = [x for x in lines if keep(x)] if keep else []
    picked = picked or lines
    more = len(picked) - MAX_LINES
    return picked[:MAX_LINES] + ([f'... and {more} more'] if more > 0 else [])


def check_verify(run):
    rc, out, err = run([os.path.join(paths.HARNESS, 'build-and-verify.sh'), '--all', '--json'])
    try:
        lines = json.loads(out).get('lines', [])
    except ValueError:
        lines = (out + err).splitlines()[-MAX_LINES:]
    if rc == 0:
        return None
    return _trim(lines, lambda x: x.startswith('ERROR'))


def check_manifest(run):
    rc, out, err = run([sys.executable, os.path.join(paths.REPO, 'tools', 'manifest.py'),
                        '--manifest', os.path.join(paths.STATE, 'wholetree-manifest.tsv')])
    return None if rc == 0 else _trim((out + err).splitlines())


def check_lint(run):
    rc, out, err = run([sys.executable, os.path.join(paths.REPO, 'tools', 'lint_claims.py')])
    lines = out.splitlines()
    found = [x for x in lines if x.strip() and not STALE.search(x) and not SUMMARY.match(x)]
    new = next((int(m.group(1)) for m in map(SUMMARY.match, lines) if m), None)
    if rc == 2 or new is None and rc != 0:
        return _trim((out + err).splitlines())
    return _trim(found) if new else None


def check_tests(run):
    rc, out, err = run([sys.executable, '-m', 'pytest', 'tests/test_units.py',
                        'tests/test_manifest.py', '-q', '-p', 'no:cacheprovider', '-rfE',
                        '--deselect', 'tests/test_manifest.py::test_committed_manifest_matches_the_tree'])
    return None if rc == 0 else _trim((out + err).splitlines(),
                                      lambda x: x.startswith(('FAILED', 'ERROR')))


CHECKS = (('verify (harness/build-and-verify.sh --all)', check_verify),
          ('manifest (tools/manifest.py)', check_manifest),
          ('lint (tools/lint_claims.py)', check_lint),
          ('tests (tests/test_units.py, tests/test_manifest.py)', check_tests))


def check(run=run_command, environ=None):
    """(ok, message): message is empty when everything passes or the checks are off."""
    if not enabled(environ):
        return True, ''
    parts = []
    for name, fn in CHECKS:
        lines = fn(run)
        if lines:
            parts.append(f'{name} failed:\n' + '\n'.join('  ' + x for x in lines))
    if not parts:
        return True, ''
    return False, 'The whole-tree checks failed.\n' + '\n'.join(parts) + '\n' + ADVICE


def main():
    ok, message = check()
    if message:
        print(message)
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
