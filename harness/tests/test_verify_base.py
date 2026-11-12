"""Holding a run to the base branch's matched set, and trusting the original exe."""
import hashlib
import os
import subprocess

import pytest

from conftest import FakeIndex, make_pe
from harness.lib import funcs, matched, paths, verify


def entry(score, exact=False):
    return dict(name='A::f', symbol='?f@A@@QAEXXZ', unit='A.cpp', size=16, score=score,
                exact=exact, recorded='2026-11-01')


def matched_set(**entries):
    return {'version': 1, 'functions': {k.replace('a', '0x'): v for k, v in entries.items()}}


@pytest.fixture
def empty_tree(tmp_path, monkeypatch):
    src = tmp_path / 'game'
    src.mkdir()
    monkeypatch.setattr(paths, 'SRC_ROOT', str(src))
    monkeypatch.setattr(paths, 'MATCHED_SET', str(tmp_path / 'matched_set.json'))
    monkeypatch.setattr(paths, 'BOUNDARIES', str(tmp_path / 'boundary_errors.tsv'))
    monkeypatch.setattr(funcs, 'FunctionIndex', lambda *a: FakeIndex())
    return tmp_path


def list_boundaries(tmp_path, *addresses):
    rows = ''.join(f'{a}\tFUN_{a[2:]}\t0x00401000\teh-prolog\tsplit from its body\n'
                   for a in addresses)
    (tmp_path / 'boundary_errors.tsv').write_text('address\tname\tpart_of\tkind\tnote\n' + rows)


def test_a_listed_boundary_error_that_was_never_exact_may_be_dropped(empty_tree):
    matched.save(paths.MATCHED_SET, matched_set(a00401000=entry(99.0)))
    list_boundaries(empty_tree, '0x00401010')
    rep = verify.run([], base=(matched_set(a00401000=entry(95.0), a00401010=entry(20.0)), 'base'))
    assert rep.regressions == [] and not rep.failed
    assert any('0x00401010 dropped from matched_set.json: on the boundary-error list' in ln
               for ln in rep.lines)


def test_a_listed_address_game_still_claims_may_not_be_dropped(empty_tree):
    base = matched_set(a00401000=entry(99.0), a00401010=entry(40.0))
    head = matched_set(a00401000=entry(99.0))
    got, released = matched.release_listed(base, head, {'0x00401010'}, claimed={'0x00401010'})
    assert released == [] and got == base
    (empty_tree / 'game' / 'unit.cpp').write_text('// FUNCTION: STARFLEETOP 0x00401010\nvoid f() {}\n'
                                                 '// STUB: STARFLEETOP 0x00401020\nvoid g() {}\n')
    assert verify.claimed_keys({'module': 'STARFLEETOP'}) == {'0x00401010'}


def test_an_unlisted_or_exact_entry_still_may_not_be_dropped(empty_tree):
    matched.save(paths.MATCHED_SET, matched_set())
    list_boundaries(empty_tree, '0x00401020')
    base = matched_set(a00401010=entry(20.0), a00401020=entry(100.0, True))
    rep = verify.run([], base=(base, 'base'))
    assert sorted(rep.regressions) == [0x401010, 0x401020]


def test_a_listed_entry_that_is_kept_is_still_held_to_its_score(empty_tree):
    matched.save(paths.MATCHED_SET, matched_set(a00401010=entry(10.0)))
    list_boundaries(empty_tree, '0x00401010')
    rep = verify.run([], base=(matched_set(a00401010=entry(20.0)), 'base'))
    assert rep.regressions == [0x401010]


def test_entries_the_head_drops_or_lowers_are_regressions(empty_tree):
    head = matched_set(a00401000=entry(90.0), a00401020=entry(100.0, True))
    matched.save(paths.MATCHED_SET, head)
    base = matched_set(a00401000=entry(95.0), a00401010=entry(50.0),
                       a00401020=entry(100.0, True))
    rep = verify.run([], base=(base, 'origin/develop'))
    assert sorted(rep.regressions) == [0x401000, 0x401010]
    assert any('0x00401010' in ln and 'drops' in ln for ln in rep.lines)
    assert any('0x00401000' in ln and 'lowers to 90.00%' in ln for ln in rep.lines)


def test_an_exact_entry_that_is_no_longer_exact_is_a_regression(empty_tree):
    matched.save(paths.MATCHED_SET, matched_set(a00401000=entry(100.0, False)))
    rep = verify.run([], base=(matched_set(a00401000=entry(100.0, True)), 'base'))
    assert rep.regressions == [0x401000]


def test_a_head_that_only_adds_and_raises_passes(empty_tree):
    matched.save(paths.MATCHED_SET, matched_set(a00401000=entry(99.0), a00401010=entry(10.0)))
    rep = verify.run([], base=(matched_set(a00401000=entry(95.0)), 'base'))
    assert rep.regressions == [] and not rep.failed


def test_merged_bar_is_the_higher_of_base_and_head():
    m = matched.merged(matched_set(a00401000=entry(90.0), a00401020=entry(70.0)),
                       matched_set(a00401000=entry(95.0), a00401010=entry(50.0)))
    assert {k: v['score'] for k, v in m['functions'].items()} == \
        {'0x00401000': 95.0, '0x00401010': 50.0, '0x00401020': 70.0}


def git(repo, *args):
    subprocess.run(['git', '-C', str(repo), '-c', 'user.name=Test', '-c',
                    'user.email=test@example.invalid', *args], check=True, capture_output=True)


def test_base_is_read_from_a_git_ref_or_a_file(tmp_path, monkeypatch):
    repo = tmp_path / 'repo'
    (repo / 'analysis' / 'results').mkdir(parents=True)
    git(repo, 'init', '-q')
    (repo / 'README').write_text('x\n')
    git(repo, 'add', '-A')
    git(repo, 'commit', '-q', '-m', 'before the matched set')
    m = repo / 'analysis' / 'results' / 'matched_set.json'
    matched.save(str(m), matched_set(a00401000=entry(95.0)))
    git(repo, 'add', '-A')
    git(repo, 'commit', '-q', '-m', 'matched set')
    monkeypatch.setattr(paths, 'REPO', str(repo))
    monkeypatch.setattr(paths, 'MATCHED_SET', str(m))
    data, label = verify.load_base('HEAD')
    assert label == 'HEAD:analysis/results/matched_set.json'
    assert data['functions']['0x00401000']['score'] == 95.0
    data, label = verify.load_base('HEAD~1')
    assert data == matched.empty() and 'no analysis/results/matched_set.json' in label
    data, _ = verify.load_base(str(m))
    assert '0x00401000' in data['functions']
    with pytest.raises(verify.Untrusted):
        verify.load_base('no-such-ref')


@pytest.fixture
def exe(tmp_path, monkeypatch):
    raw = make_pe(b'\xc3')
    path = tmp_path / 'StarFleetOP.exe'
    path.write_bytes(raw)
    hashes = tmp_path / 'hashes'
    monkeypatch.setattr(paths, 'EXE', str(path))
    monkeypatch.setattr(paths, 'HASHES', str(hashes))
    monkeypatch.delenv('HARNESS_EXE_UNVERIFIED', raising=False)
    return hashes, hashlib.sha256(raw).hexdigest()


def test_the_exe_is_checked_against_its_hash(exe):
    hashes, digest = exe
    hashes.write_text(f'{digest}  {paths.EXE_REL}\n')
    rep = verify.Report()
    assert verify.check_exe(rep) is not None and rep.lines == []


@pytest.mark.parametrize('line', ['', f'{"0" * 64}  {paths.EXE_REL}\n',
                                  f'{"0" * 64}  some/other.exe\n'])
def test_a_missing_hash_or_another_exe_stops_the_run(exe, line):
    hashes, _ = exe
    hashes.write_text(line)
    with pytest.raises(verify.Untrusted):
        verify.check_exe(verify.Report())


def test_another_exe_is_scored_only_when_asked_for_and_then_warned_about(exe, monkeypatch):
    hashes, _ = exe
    hashes.write_text(f'{"0" * 64}  {paths.EXE_REL}\n')
    monkeypatch.setenv('HARNESS_EXE_UNVERIFIED', '1')
    rep = verify.Report()
    verify.check_exe(rep)
    assert rep.lines and rep.lines[0].startswith('WARN') and 'NOT the original exe' in rep.lines[0]


def test_the_boundary_list_reads_keys_and_skips_blank_rows(tmp_path):
    p = tmp_path / 'boundary_errors.tsv'
    p.write_text('address\tname\tpart_of\tkind\tnote\n'
                 '0x0040ab7e\tFUN_0040ab7e\t0x0040ab74\tbody-fragment\tx\n'
                 '\t\t\t\t\n'
                 '0x401010\tFUN_00401010\t\tthunk\ty\n')
    assert matched.boundary_listed(str(p)) == {'0x0040ab7e', '0x00401010'}
    assert matched.boundary_listed(str(tmp_path / 'missing.tsv')) == set()


def test_a_bad_address_on_the_boundary_list_names_the_file_and_line(tmp_path):
    p = tmp_path / 'boundary_errors.tsv'
    p.write_text('address\tname\tpart_of\tkind\tnote\n'
                 '0x0040ab7e\tFUN_0040ab7e\t\tthunk\tx\n'
                 '4198400\tFUN_00401000\t\tthunk\ty\n')
    with pytest.raises(ValueError, match=r'boundary_errors\.tsv:3: address \'4198400\''):
        matched.boundary_listed(str(p))
