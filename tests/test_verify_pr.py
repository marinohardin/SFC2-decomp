"""tools/verify_pr.py rebuilds a pull request's op/ files in a worktree of develop.

A throwaway "origin" repository holds develop (with a stub harness/build-and-verify.sh that prints
a fixed result) and refs/pull/5/head; a clone of it is the maintainer's checkout. No network, no
gh, no compiler. Expected text is written out by hand.
"""
import importlib.util
import os
import pathlib
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]
_spec = importlib.util.spec_from_file_location('verify_pr', ROOT / 'tools/verify_pr.py')
verify_pr = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(verify_pr)

ISSUES = [{'number': 7, 'state': 'open', 'labels': [{'name': 'claim'}], 'user': {'login': 'ann'},
           'body': 'op/a.cpp\nrange: 0x00401000-0x004010ff\n'}]
BODY = 'Claim: #7\n\n- Claimed exact: 2 (0x00401010, 0x00401020)\n'
STUB = r'''#!/bin/sh
printf '\033[32mOK\033[0m    0x00401010 tFoo::Bar 100.00%% exact (8 bytes original, 8 rebuilt; 0 relocations, 0 unverified); matched_set.json has 50.00%%\n'
echo "INFO  args: $*"
echo "INFO  no cache: $HARNESS_NO_CACHE"
grep -c FUNCTION op/a.cpp | sed 's/^/INFO  annotations: /'
[ -e op/gone.cpp ] && echo "INFO  gone.cpp still there"
echo "RESULT OK"
'''


def _git(repo, *args):
    return subprocess.run(['git', '-c', 'user.name=Test', '-c', 'user.email=test@example.invalid',
                           *args], cwd=repo, check=True, capture_output=True, text=True).stdout


def _write(repo, rel, text):
    p = repo / rel
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(text)


def _setup(tmp_path, monkeypatch, change):
    origin = tmp_path / 'origin'
    origin.mkdir()
    _git(origin, 'init', '-q', '-b', 'develop')
    _write(origin, 'harness/build-and-verify.sh', STUB)
    _write(origin, 'harness/lib/symsync.py', 'print("ok: symbols")\n')
    _write(origin, 'tools/lint_claims.py', 'print("ok: claims")\n')
    _write(origin, 'analysis/results/matched_set.json',
           '{"version": 1, "functions": {"0x00401010": {"score": 50.0, "exact": false}}}\n')
    _write(origin, 'op/a.cpp', 'void f() {}\n')
    _write(origin, 'op/UNITS.tsv', 'unit\tfirst\tlast\n'
           'a.cpp\t0x00401000\t0x004010ff\ngone.cpp\t0x00402000\t0x004020ff\n')
    _write(origin, 'op/gone.cpp', 'void g() {}\n')
    _git(origin, 'add', '-A')
    _git(origin, 'commit', '-q', '-m', 'base')
    _git(origin, 'checkout', '-q', '-b', 'contrib/7-a')
    change(origin)
    _git(origin, 'add', '-A')
    _git(origin, 'commit', '-q', '-m', 'feat(op): match tFoo::Bar (0x00401010)')
    sha = _git(origin, 'rev-parse', 'HEAD').strip()
    _git(origin, 'update-ref', 'refs/pull/5/head', sha)
    _git(origin, 'checkout', '-q', 'develop')
    _git(tmp_path, 'clone', '-q', str(origin), 'main')
    monkeypatch.setenv('SFCOP_WORK', str(tmp_path / 'work'))
    return tmp_path / 'main', sha


def _source_change(origin):
    _write(origin, 'op/a.cpp', '// FUNCTION: STARFLEETOP 0x00401010\nvoid f() {}\n'
           '// FUNCTION: STARFLEETOP 0x00401020\nvoid h() {}\n')
    (origin / 'op/gone.cpp').unlink()


def test_a_clean_pull_request_is_rebuilt_and_reported(tmp_path, monkeypatch):
    main, sha = _setup(tmp_path, monkeypatch, _source_change)
    lines = []
    assert verify_pr.verify(str(main), 5, sha, body=BODY, issues=ISSUES, log=lines.append,
                            author='ann') == 0
    work = tmp_path / 'work' / 'verify'
    assert not (work / '5').exists() and not (work / '5.state').exists()
    assert subprocess.run(['git', 'worktree', 'list'], cwd=main, capture_output=True,
                          text=True).stdout.count('\n') == 1
    report = (work / '5-report.md').read_text()
    assert '| 0x00401010 | tFoo::Bar | 50.00% | 100.00% | yes |' in report
    assert '| 0x00401020 | (not scored) | - | - | no |' in report
    assert 'Regressions: 0' in report
    assert 'RESULT OK' in report
    assert 'Claimed 2 exact, verified 1' in report
    assert 'Claimed exact but not verified: 0x00401020' in report
    assert '\x1b' not in report
    log = (work / '5-report-harness.log').read_text()
    assert 'INFO  args: --all --base origin/develop 0x00401010 0x00401020' in log
    assert 'INFO  no cache: 1' in log
    assert 'INFO  annotations: 2' in log
    assert 'gone.cpp still there' not in log
    assert 'ok: claims' in log and 'ok: symbols' in log


def test_a_head_that_is_not_the_reviewed_sha_is_refused(tmp_path, monkeypatch):
    main, sha = _setup(tmp_path, monkeypatch, _source_change)
    lines = []
    other = '0' * 40
    assert verify_pr.verify(str(main), 5, other, body=BODY, issues=ISSUES,
                            log=lines.append, author='ann') == 3
    assert lines == [f'verify: refused: pull/5/head is {sha}, not {other}']
    assert not (tmp_path / 'work' / 'verify').exists()


def test_a_pull_request_the_guard_fails_is_refused_before_any_build(tmp_path, monkeypatch):
    def change(origin):
        _source_change(origin)
        _write(origin, 'harness/build-and-verify.sh', '#!/bin/sh\necho planted\n')
    main, sha = _setup(tmp_path, monkeypatch, change)
    lines = []
    assert verify_pr.verify(str(main), 5, sha, body=BODY, issues=ISSUES, log=lines.append,
                            author='ann') == 3
    assert lines == ['FAIL harness/build-and-verify.sh: outside op/',
                     'verify: refused by the guard']
    assert not (tmp_path / 'work' / 'verify').exists()


def test_clean_strips_escapes_and_control_characters():
    assert verify_pr.clean('\x1b[31merror\x1b[0m\r\nnext\x07 \x1b]0;t\x07ok\ttab') == \
        'error\nnext 0;tok\ttab'


def test_claimed_reads_the_evidence_line():
    assert verify_pr.claimed('- Claimed exact: 3 (0x00401000, 0x401010)') == \
        (3, [0x401000, 0x401010])
    assert verify_pr.claimed('- Claimed exact: 0') == (0, [])
    assert verify_pr.claimed('nothing') == (None, [])


def test_the_harness_environment_drops_overrides(monkeypatch):
    monkeypatch.setenv('HARNESS_MATCHED_SET', '/elsewhere.json')
    monkeypatch.setenv('HARNESS_COMPILER', 'native')
    monkeypatch.setenv('SFCOP_LANE', '1')
    monkeypatch.setenv('GH_TOKEN', 'not-a-real-token')
    env = verify_pr.harness_env('/w/verify/5.state')
    assert 'HARNESS_MATCHED_SET' not in env and 'SFCOP_LANE' not in env
    assert 'GH_TOKEN' not in env
    assert env['HARNESS_COMPILER'] == 'native'
    assert env['HARNESS_NO_CACHE'] == '1' and env['HARNESS_STATE'] == '/w/verify/5.state'
    assert os.environ['HARNESS_MATCHED_SET'] == '/elsewhere.json'
