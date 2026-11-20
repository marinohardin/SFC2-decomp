"""The whole-tree checks after a function session (harness/lib/wholetree.py). The commands are
fakes: nothing is compiled and no tool runs."""
import importlib.util
import io
import json
import os
import sys

import pytest

from harness.lib import paths, wholetree

OK_VERIFY = json.dumps(dict(exit=0, lines=['INFO fine'], results=[]))
SUMMARY_0 = 'lint_claims: 0 new (empty-body 0), 12 in the baseline, 0 stale baseline rows\n'


def fake(**overrides):
    """A runner keyed on what each command is; any command not overridden passes."""
    answers = dict(verify=(0, OK_VERIFY, ''), manifest=(0, '', ''), lint=(0, SUMMARY_0, ''),
                   tests=(0, '3 passed in 1.0s\n', ''))
    answers.update(overrides)
    calls = []

    def run(argv, env=None):
        calls.append(argv)
        key = ('verify' if argv[0].endswith('build-and-verify.sh') else
               'manifest' if argv[1].endswith('manifest.py') else
               'lint' if argv[1].endswith('lint_claims.py') else 'tests')
        return answers[key]
    run.calls = calls
    return run


def test_all_passing_is_silent_and_runs_all_four():
    run = fake()
    assert wholetree.check(run, {}) == (True, '')
    assert len(run.calls) == 4


def test_the_off_switch_runs_nothing():
    run = fake(verify=(1, 'not json', 'boom'))
    assert wholetree.check(run, {'HARNESS_WHOLETREE': '0'}) == (True, '')
    assert run.calls == []


def test_a_verify_error_in_any_unit_fails_with_the_error_lines_only():
    out = json.dumps(dict(exit=2, lines=['INFO a', 'ERROR Tiger/x.cpp: relocation 0x1 names _y',
                                         'WARN b'], results=[]))
    ok, msg = wholetree.check(fake(verify=(2, out, '')), {})
    assert not ok
    assert 'ERROR Tiger/x.cpp: relocation 0x1 names _y' in msg
    assert 'INFO a' not in msg and 'WARN b' not in msg
    assert 'build-and-verify.sh --all' in msg


def test_unparsable_verify_output_shows_its_last_lines():
    ok, msg = wholetree.check(fake(verify=(2, '', 'Traceback\nValueError: x')), {})
    assert not ok and 'ValueError: x' in msg


def test_a_manifest_error_fails_with_its_text():
    ok, msg = wholetree.check(fake(manifest=(2, '', 'manifest: a class is declared twice\n')), {})
    assert not ok
    assert 'manifest: a class is declared twice' in msg


def test_a_new_lint_finding_fails_and_a_stale_baseline_row_does_not():
    new = ('op/A/a.cpp:10: empty-body: 0x00401000 has an empty body\n'
           'lint_claims: 1 new (empty-body 1), 12 in the baseline, 0 stale baseline rows\n')
    ok, msg = wholetree.check(fake(lint=(1, new, '')), {})
    assert not ok and 'op/A/a.cpp:10: empty-body' in msg
    assert 'lint_claims: 1 new' not in msg
    stale = ('op/A/b.cpp: empty-body: 0x1 is in the baseline but no longer occurs; delete its row '
             'from the baseline\n'
             'lint_claims: 0 new (empty-body 0), 11 in the baseline, 1 stale baseline rows\n')
    assert wholetree.check(fake(lint=(1, stale, '')), {}) == (True, '')


def test_a_lint_crash_fails_with_its_text():
    ok, msg = wholetree.check(fake(lint=(2, '', 'lint_claims: no source tree at x\n')), {})
    assert not ok and 'no source tree' in msg


def test_a_failing_test_fails_with_the_failed_lines_and_deselects_the_manifest_comparison():
    out = ('F\n=== short test summary info ===\n'
           'FAILED tests/test_units.py::test_empty_stray - assert 0\n1 failed, 2 passed\n')
    run = fake(tests=(1, out, ''))
    ok, msg = wholetree.check(run, {})
    assert not ok
    assert 'FAILED tests/test_units.py::test_empty_stray' in msg and '1 failed, 2 passed' not in msg
    argv = run.calls[-1]
    assert argv[argv.index('--deselect') + 1] == \
        'tests/test_manifest.py::test_committed_manifest_matches_the_tree'


def test_every_failure_is_reported_at_once_with_one_closing_sentence():
    ok, msg = wholetree.check(fake(manifest=(2, 'manifest: bad\n', ''),
                                   tests=(1, 'FAILED t::x - e\n', '')), {})
    assert not ok
    assert 'manifest (' in msg and 'tests (' in msg and 'lint (' not in msg
    assert msg.endswith(wholetree.ADVICE)


def test_long_output_is_trimmed():
    err = ''.join(f'line {i}\n' for i in range(100))
    ok, msg = wholetree.check(fake(manifest=(2, '', err)), {})
    assert 'line 0' in msg and 'line 14' in msg and 'line 15' not in msg
    assert '... and 85 more' in msg


def load_stop_hook():
    spec = importlib.util.spec_from_file_location(
        'stop_verify', os.path.join(paths.HARNESS, 'hooks', 'stop_verify.py'))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def run_stop_hook(monkeypatch, tmp_path, blocks, whole):
    hook = load_stop_hook()
    claim = tmp_path / 'claim.json'
    claim.write_text(json.dumps(dict(address='0x006fc690', baseline_score=80.0,
                                     baseline_exact=False, max_blocks=3, blocks=blocks)))
    monkeypatch.setenv('HARNESS_FUNCTION_SESSION', '1')
    monkeypatch.setattr(paths, 'CLAIM', str(claim))
    monkeypatch.setattr(sys, 'stdin', io.StringIO('{}'))
    verify = dict(exit=0, lines=[], results=[dict(address='0x006fc690', score=90.0, exact=False)])

    class P:
        stdout = json.dumps(verify)
    monkeypatch.setattr(hook.subprocess, 'run', lambda *a, **k: P)
    monkeypatch.setattr(hook.wholetree, 'check', whole)
    return hook, claim


def test_the_stop_hook_blocks_an_improved_function_when_the_tree_fails(monkeypatch, tmp_path,
                                                                       capsys):
    hook, claim = run_stop_hook(monkeypatch, tmp_path, 0, lambda: (False, 'MANIFEST BROKE'))
    hook.main()
    out = json.loads(capsys.readouterr().out)
    assert out == {'decision': 'block', 'reason': 'MANIFEST BROKE'}
    assert json.loads(claim.read_text())['blocks'] == 1


def test_the_stop_hook_lets_an_improved_function_go_when_the_tree_passes(monkeypatch, tmp_path,
                                                                         capsys):
    hook, claim = run_stop_hook(monkeypatch, tmp_path, 0, lambda: (True, ''))
    hook.main()
    assert 'improved from 80.00% to 90.00%' in json.loads(capsys.readouterr().out)['systemMessage']


def test_the_stop_hook_does_not_run_the_tree_checks_once_refusals_are_used_up(monkeypatch,
                                                                            tmp_path, capsys):
    def boom():
        raise AssertionError('must not run')
    hook, claim = run_stop_hook(monkeypatch, tmp_path, 3, boom)
    hook.main()
    assert 'repeated refusals' in json.loads(capsys.readouterr().out)['systemMessage']
