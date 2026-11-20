"""Whole-tree checks after a function session (harness/README.md, "Whole-tree checks").

The Stop hook (harness/hooks/stop_verify.py) runs them once the function itself is judged
good, and harness/loop.sh runs them again before it commits the round, so the session that
caused a problem is told while it still has the context. Five checks, each the tool's own:

  verify    build-and-verify.sh --all --json: a regression or an ERROR in any unit
  manifest  tools/manifest.py, written to a scratch file so the tree is left alone
  lint      tools/lint_claims.py: a new finding (a stale baseline row is not the session's)
  tests     tests/test_units.py and tests/test_manifest.py, except the one test that compares the
            committed manifest with the matched set: the loop never regenerates the manifest
  symbols   tools/symbols.py claim (all) then check, on a copy of the symbol table

HARNESS_WHOLETREE=0 turns all of it off. HARNESS_WHOLETREE_BUDGET (seconds, default 600) is the time
for the whole step; each command gets what remains. At its start the loop runs them once on the tree as it stands, reusing its own
startup `--all` (`--verify-log FILE --verify-exit N`), and refuses to start on a failing tree. `python3 -m harness.lib.wholetree` prints the failures
and exits 1, or prints nothing and exits 0.
"""
import json
import os
import re
import subprocess
import sys
import time

from . import paths

MAX_LINES = 15
STALE = re.compile(r'is in the baseline but no longer occurs')
# A partial function's relocation detail is an ERROR line too (verify.py), but not a failure.
RELOC_NOTE = re.compile(r'^ERROR\s+relocation \+0x')
SUMMARY = re.compile(r'^lint_claims: (\d+) new')
ADVICE = ('Fix these in the source of your own function (or undo the edit that caused them) and '
          'stop again; do not edit another function or another lane\'s file.')


def enabled(environ=None):
    return (os.environ if environ is None else environ).get('HARNESS_WHOLETREE', '1') != '0'


BUDGET = 600
TIMEOUT_TEXT = ('%s timed out (the %d s time budget of the whole-tree checks); the build cache '
                'was probably cold (a cold --all took 189 s here). Run it once by hand, then '
                'stop again.')
NOT_RUN_TEXT = '%s not run: the %d s time budget of the whole-tree checks was spent.'


def _now():
    return time.monotonic()


def run_command(argv, env=None, timeout=None):
    """(exit code, stdout, stderr) of argv, run from the repository root. A command that runs
    longer than `timeout` seconds exits 124 with the fixed text on stderr."""
    e = dict(os.environ, PYTHONPATH=paths.REPO, PYTHONDONTWRITEBYTECODE='1')
    e.update(env or {})
    name = ' '.join(os.path.basename(a) for a in argv[:3])
    try:
        p = subprocess.run(argv, capture_output=True, text=True, cwd=paths.REPO, env=e,
                           timeout=timeout)
    except subprocess.TimeoutExpired:
        return 124, '', TIMEOUT_TEXT % (name, timeout)
    return p.returncode, p.stdout, p.stderr


def _trim(lines, keep=None):
    """At most MAX_LINES of lines: those `keep` accepts, else the last ones."""
    lines = [x.rstrip() for x in lines if x.strip()]
    picked = [x for x in lines if keep(x)] if keep else []
    picked = picked or lines
    more = len(picked) - MAX_LINES
    return picked[:MAX_LINES] + ([f'... and {more} more'] if more > 0 else [])


def check_verify(run, done=None):
    """done: (exit code, text) of an `--all` run already made, in place of a new one."""
    if done:
        rc, out, err = done[0], '', done[1]
        lines = err.splitlines()
    else:
        rc, out, err = run([os.path.join(paths.HARNESS, 'build-and-verify.sh'), '--all', '--json'])
        try:
            lines = json.loads(out).get('lines', [])
        except ValueError:
            return None if rc == 0 else _trim((out + err).splitlines()[-MAX_LINES:])
    if rc == 0:
        return None
    if rc == 124:
        return [err.strip()]
    kept = [x for x in lines if x.startswith('ERROR') and not RELOC_NOTE.match(x)]
    return _trim(kept) if kept else [f'verify failed with exit {rc}']


def check_manifest(run):
    rc, out, err = run([sys.executable, os.path.join(paths.REPO, 'tools', 'manifest.py'),
                        '--manifest', os.path.join(paths.STATE, 'wholetree-manifest.tsv')])
    return None if rc == 0 else _trim((out + err).splitlines())


def check_lint(run):
    rc, out, err = run([sys.executable, os.path.join(paths.REPO, 'tools', 'lint_claims.py')])
    lines = out.splitlines()
    found = [x for x in lines if x.strip() and not STALE.search(x) and not SUMMARY.match(x)]
    new = next((int(m.group(1)) for m in map(SUMMARY.match, lines) if m), None)
    if rc == 124 or rc == 2 or new is None and rc != 0:
        return _trim((out + err).splitlines())
    return _trim(found) if new else None


def check_symbols(run):
    """The symbol table, synced to the claims the verify step just rebuilt and then checked, on
    a copy in the state folder so the tree is left alone (harness/lib/symsync.py --scratch; the
    loop commits the table through symsync and the landing step syncs all of it)."""
    rc, out, err = run([sys.executable, '-m', 'harness.lib.symsync', '--scratch'])
    return None if rc == 0 else _trim((out + err).splitlines(),
                                      lambda x: x.startswith(('ERROR', 'WARN')))


def check_tests(run):
    rc, out, err = run([sys.executable, '-m', 'pytest', 'tests/test_units.py',
                        'tests/test_manifest.py', '-q', '-p', 'no:cacheprovider', '-rfE',
                        '--deselect', 'tests/test_manifest.py::test_committed_manifest_matches_the_tree'])
    return None if rc == 0 else _trim((out + err).splitlines(),
                                      lambda x: x.startswith(('FAILED', 'ERROR')))


CHECKS = (('verify (harness/build-and-verify.sh --all)', check_verify),
          ('manifest (tools/manifest.py)', check_manifest),
          ('lint (tools/lint_claims.py)', check_lint),
          ('tests (tests/test_units.py, tests/test_manifest.py)', check_tests),
          ('symbols (tools/symbols.py claim, check)', check_symbols))


def check(run=None, environ=None, verify_done=None):
    """(ok, message): message is empty when everything passes or the checks are off. `run` is
    called as run(argv, env=None, timeout=seconds_left); once HARNESS_WHOLETREE_BUDGET seconds
    are spent, the checks still to run are reported as not run."""
    if not enabled(environ):
        return True, ''
    run = run or run_command
    budget = int((os.environ if environ is None else environ).get('HARNESS_WHOLETREE_BUDGET',
                                                                  BUDGET))
    start = _now()
    inner = run

    def run(argv, env=None):
        left = budget - (_now() - start)
        if left <= 0:
            return 124, '', NOT_RUN_TEXT % (' '.join(os.path.basename(a) for a in argv[:3]),
                                            budget)
        return inner(argv, env, timeout=left)
    parts = []
    for name, fn in CHECKS:
        lines = fn(run, verify_done) if fn is check_verify else fn(run)
        if lines:
            parts.append(f'{name} failed:\n' + '\n'.join('  ' + x for x in lines))
    if not parts:
        return True, ''
    return False, 'The whole-tree checks failed.\n' + '\n'.join(parts) + '\n' + ADVICE


def main(argv=None):
    a = list(sys.argv[1:] if argv is None else argv)
    done = None
    if a[:1] == ['--verify-log'] and a[2:3] == ['--verify-exit']:
        try:
            with open(a[1], errors='replace') as f:
                done = (int(a[3]), f.read())
        except (OSError, ValueError, IndexError):
            done = None
    ok, message = check(verify_done=done)
    if message:
        print(message)
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
