"""objdiff's per-function report is read once and not kept: a full run's build folder stays small."""
import json
import os
import stat

from harness.lib import paths, verify


def fake_objdiff(tmp_path, report):
    """An objdiff that writes `report` to the -o file (the 7th argument)."""
    src = tmp_path / 'report.json'
    src.write_text(json.dumps(report))
    exe = tmp_path / 'objdiff'
    exe.write_text(f'#!/bin/sh\ncp {src} "$7"\n')
    exe.chmod(exe.stat().st_mode | stat.S_IXUSR)
    return str(exe)


def run(tmp_path, monkeypatch, percent):
    left = {'name': '?f@@YAXXZ', 'match_percent': percent, 'target_symbol': 0,
            'instructions': [{'diff_kind': 'DIFF_NONE' if percent == 100.0 else 'DIFF_REPLACE',
                              'instruction': {'address': 0, 'formatted': 'ret'}}]}
    right = {'name': '?f@@YAXXZ', 'instructions': [{'instruction': {'address': 0, 'formatted': 'ret'}}]}
    report = {'left': {'symbols': [left]}, 'right': {'symbols': [right]}}
    monkeypatch.setattr(paths, 'OBJDIFF', fake_objdiff(tmp_path, report))
    return verify.objdiff('a.obj', 'b.obj', '?f@@YAXXZ', str(tmp_path))


def test_an_exact_function_leaves_no_report_behind(tmp_path, monkeypatch):
    left, right, err = run(tmp_path, monkeypatch, 100.0)
    assert (left['match_percent'], err) == (100.0, '')
    assert not os.path.exists(tmp_path / 'objdiff.json')


def test_a_function_that_differs_still_gives_the_rows_a_session_is_shown(tmp_path, monkeypatch):
    left, right, err = run(tmp_path, monkeypatch, 50.0)
    assert err == ''
    assert verify.diff_rows(left, right) == ([('+0x0', 'ret', 'ret', 'replace')], 1)
    assert not os.path.exists(tmp_path / 'objdiff.json')
