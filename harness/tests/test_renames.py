"""A function exact at the base keeps its symbol (harness/lib/renames.py)."""
import json
import subprocess

import pytest

from harness.lib import paths, renames, verify

A, B, C, D = '0x00401000', '0x00401010', '0x00401020', '0x00401030'
TABLE = ('address\tkind\tname\tclass\tsymbol\tsymbol_evidence\tdeclaration\tevidence\n'
         f'{A}\tfunction\tf\tt\t?Fire@tShip@@UAEXXZ\tclaim\t\tclaim\n'
         f'{B}\tfunction\tg\tt\t?Hold@tShip@@UAEXXZ\tclaim\t\tclaim\n'
         f'{C}\tfunction\th\tt\t?Warp@tShip@@UAEXXZ\tclaim\t\tclaim\n'
         f'{D}\tfunction\ti\tt\t?Cloak@tShip@@UAEXXZ\tclaim\t\tclaim\n')
MATCHED = {'version': 1, 'functions': {
    A: {'exact': True}, B: {'exact': True}, C: {'exact': False}, D: {'exact': True}}}


@pytest.fixture
def base(tmp_path, monkeypatch):
    repo = tmp_path / 'repo'
    (repo / 'analysis' / 'results').mkdir(parents=True)
    (repo / 'analysis' / 'symbols').mkdir(parents=True)
    (repo / 'analysis' / 'results' / 'matched_set.json').write_text(json.dumps(MATCHED))
    (repo / 'analysis' / 'symbols' / 'STARFLEETOP.tsv').write_text(TABLE)
    git = ['git', '-C', str(repo), '-c', 'user.name=t', '-c', 'user.email=t@example.com']
    subprocess.run(['git', 'init', '-q', str(repo)], check=True)
    subprocess.run(git + ['add', '-A'], check=True)
    subprocess.run(git + ['commit', '-qm', 'base'], check=True)
    monkeypatch.setattr(paths, 'REPO', str(repo))
    monkeypatch.setattr(renames, 'BASELINE', str(tmp_path / 'none.tsv'))
    return subprocess.run(git + ['rev-parse', 'HEAD'], capture_output=True, text=True).stdout.strip()


SAME = {A: '?Fire@tShip@@UAEXXZ', B: '?Hold@tShip@@UAEXXZ', C: '?Warp@tShip@@UAEXXZ'}


def test_an_unchanged_symbol_passes(base):
    assert renames.findings(SAME, base) == []


def test_a_renamed_exact_function_is_a_finding_that_names_both_symbols(base):
    built = dict(SAME, **{B: '?Wait@tShip@@UAEXXZ'})
    found = renames.findings(built, base)
    assert found == [(B, '?Hold@tShip@@UAEXXZ', '?Wait@tShip@@UAEXXZ')]
    text = renames.line(found[0], base)
    assert text.startswith(f'{B}: exact at {base[:12]} as ?Hold@tShip@@UAEXXZ, but this tree '
                           'compiles ?Wait@tShip@@UAEXXZ; keep the base name')
    assert 'HEADER REQUEST:' in text


def test_a_renamed_function_that_was_not_exact_at_the_base_passes(base):
    assert renames.findings(dict(SAME, **{C: '?Other@tShip@@UAEXXZ'}), base) == []


def test_an_exact_function_with_no_symbol_in_the_build_is_not_an_error(base):
    assert renames.findings({A: SAME[A]}, base) == []      # B and D: their unit was not built


def test_no_base_is_not_an_error(base):
    assert renames.findings({A: '?X@@YAXXZ'}, None) == []


def test_a_listed_rename_is_allowed_only_for_that_exact_triple(base, tmp_path):
    f = tmp_path / 'rename_baseline.tsv'
    f.write_text('address\tbase symbol\tnew symbol\n'
                 f'{B}\t?Hold@tShip@@UAEXXZ\t?Wait@tShip@@UAEXXZ\n')
    allowed = renames.load_baseline(str(f))
    assert renames.findings(dict(SAME, **{B: '?Wait@tShip@@UAEXXZ'}), base, allowed) == []
    assert len(renames.findings(dict(SAME, **{B: '?Other@tShip@@UAEXXZ'}), base, allowed)) == 1
    assert renames.load_baseline(str(tmp_path / 'absent.tsv')) == set()


def test_the_all_report_fails_the_run_on_a_rename_and_not_otherwise(base):
    rep = verify.Report()
    rep.results = [dict(address=B, symbol='?Wait@tShip@@UAEXXZ', unit='a.cpp'),
                   dict(address=A, symbol=SAME[A], unit='a.cpp')]
    verify.report_renamed_exact(rep, base)
    assert rep.failed and len(rep.lines) == 1 and rep.lines[0].startswith('ERROR ' + B)
    ok = verify.Report()
    ok.results = [dict(address=A, symbol=SAME[A], unit='a.cpp')]
    verify.report_renamed_exact(ok, base)
    assert not ok.failed and ok.lines == []


def test_no_base_is_a_warning_that_the_guard_did_not_run(monkeypatch):
    from harness.lib import standins
    monkeypatch.setattr(standins, 'base_ref', lambda *a, **k: None)
    rep = verify.Report()
    rep.results = [dict(address=B, symbol='?Wait@tShip@@UAEXXZ', unit='a.cpp')]
    verify.report_renamed_exact(rep)
    assert not rep.failed and len(rep.lines) == 1
    assert rep.lines[0].startswith('WARN ') and 'rename guard' in rep.lines[0]


def test_a_malformed_baseline_row_is_a_warning_and_comments_are_ignored(tmp_path):
    f = tmp_path / 'rename_baseline.tsv'
    f.write_text('# why these\naddress\tbase symbol\tnew symbol\n\n'
                 f'{B}\t?Hold@tShip@@UAEXXZ\t?Wait@tShip@@UAEXXZ\n'
                 f'{A}\t?Fire@tShip@@UAEXXZ\n'
                 f'{C}\ta\tb\tc\n')
    warn = []
    assert renames.load_baseline(str(f), warn) == {(B, '?Hold@tShip@@UAEXXZ', '?Wait@tShip@@UAEXXZ')}
    assert warn == ['rename_baseline.tsv:5: expected 3 tab-separated fields (address, base '
                    'symbol, new symbol), got 2; row ignored',
                    'rename_baseline.tsv:6: expected 3 tab-separated fields (address, base '
                    'symbol, new symbol), got 4; row ignored']


def test_main_refuses_a_build_of_another_tree(base, tmp_path, monkeypatch, capsys):
    build = tmp_path / 'build'
    build.mkdir()
    (tmp_path / 'op').mkdir()
    (tmp_path / 'op' / 'a.cpp').write_text('x')
    monkeypatch.setattr(paths, 'BUILD', str(build))
    monkeypatch.setattr(paths, 'SRC_ROOT', str(tmp_path / 'op'))     # no symbols.id: not current
    assert renames.main(['--base', base]) == 1
    assert 'not made from the current source tree' in capsys.readouterr().out
