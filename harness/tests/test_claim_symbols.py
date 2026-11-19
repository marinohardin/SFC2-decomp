"""An exact function's relocations must name the symbol claimed at their target."""
import struct

import pytest

from conftest import FakeIndex, make_pe
from harness.lib import coff, funcs, paths, target, verify

FN = 0x401000          # the exact function
CALLEE = 0x401200      # the address its call reaches in the exe
GLOBAL = 0x00500010

# call ?callee@@YAXXZ ; mov eax, [?g@@3HA] ; ret, with the exe's call going to CALLEE
BASE_CODE = bytes.fromhex('e800000000a100000000c3')
RELOCS = [(1, '?callee@@YAXXZ', coff.REL_REL32), (6, '?g@@3HA', coff.REL_DIR32)]


def exact_caller():
    """The result verify gives the hand-made function: exact, two relocations, both masked."""
    obj = coff.CoffObject(coff.write_object(BASE_CODE, '?f@@YAXXZ', RELOCS))
    code = (b'\xe8' + struct.pack('<I', CALLEE - (FN + 5)) + b'\xa1' + struct.pack('<I', GLOBAL)
            + b'\xc3')
    exe = coff.PeImage(make_pe(code + b'\xcc' * 32))
    t = target.build(FN, 11, obj, '?f@@YAXXZ', exe, FakeIndex())
    assert t['strict']
    return dict(address='0x00401000', unit='a.cpp', exact=True, symbol='?f@@YAXXZ',
                relocs=[r.as_dict() for r in t['relocs']])


def claim(symbol, address='0x00401200', unit='b.cpp'):
    """The result of the claim at `address`, rebuilt in this run; it defines `symbol`."""
    return dict(address=address, unit=unit, exact=True, symbol=symbol, relocs=[])


def recorded(**symbols):
    return {'version': 1, 'functions': {
        k.replace('a', '0x', 1): dict(name='x', symbol=s, unit='b.cpp', size=8, score=100.0,
                                      exact=True, recorded='2026-11-01')
        for k, s in symbols.items()}}


def checked(results, data=None, baseline=(), check_all=False):
    rep = verify.Report()
    rep.results = results
    verify.report_claim_symbols(rep, data or recorded(), list(baseline), check_all)
    return rep


def row(function='0x00401000', offset=1, symbol='?callee@@YAXXZ', target_='0x00401200',
        claimed='?other@@YAXXZ'):
    return dict(function=function, offset=offset, symbol=symbol, target=target_, claimed=claimed)


def test_a_call_naming_the_claimed_symbol_passes():
    rep = checked([exact_caller(), claim('?callee@@YAXXZ')])
    assert rep.claim_breaks == [] and not rep.failed
    assert rep.lines == []


def test_a_call_naming_another_symbol_than_the_claim_fails():
    rep = checked([exact_caller(), claim('?fn_00401200@c_00401100_Object@@QAEXXZ')])
    assert rep.failed
    assert rep.lines == [
        'ERROR 0x00401000 relocation +0x1 names ?callee@@YAXXZ, which reaches 0x00401200, '
        'claimed as ?fn_00401200@c_00401100_Object@@QAEXXZ (a consumer linking the exact '
        'objects cannot resolve it)']
    assert rep.claim_breaks == [dict(function='0x00401000', offset=1, symbol='?callee@@YAXXZ',
                                     target='0x00401200',
                                     claimed='?fn_00401200@c_00401100_Object@@QAEXXZ',
                                     source='rebuilt')]


def test_a_target_nothing_claims_is_ignored():
    rep = checked([exact_caller()], data=recorded(a00401300='?h@@YAXXZ'))
    assert rep.claim_breaks == [] and not rep.failed and rep.lines == []


def test_a_function_that_is_not_exact_is_not_checked():
    caller = dict(exact_caller(), exact=False)
    rep = checked([caller, claim('?other@@YAXXZ')])
    assert rep.claim_breaks == [] and not rep.failed


def test_the_rebuilt_symbol_wins_over_a_stale_recorded_one():
    rep = checked([exact_caller(), claim('?callee@@YAXXZ')],
                  data=recorded(a00401200='?old_name@@YAXXZ'))
    assert rep.claim_breaks == [] and not rep.failed


def test_a_break_against_a_recorded_symbol_only_warns():
    # The claiming unit was not built in this run: its matched-set symbol may be stale.
    rep = checked([exact_caller()], data=recorded(a00401200='?other@@YAXXZ'))
    assert not rep.failed
    assert len(rep.lines) == 1 and rep.lines[0].startswith(
        'WARN  0x00401000 relocation +0x1 names ?callee@@YAXXZ, which reaches 0x00401200, '
        'claimed as ?other@@YAXXZ in matched_set.json, not rebuilt in this run')


def test_a_break_the_baseline_lists_is_tolerated():
    rep = checked([exact_caller(), claim('?other@@YAXXZ')], baseline=[row(offset=0x40)])
    assert not rep.failed
    assert len(rep.claim_breaks) == 1
    assert rep.lines == [f'INFO  1 relocations name another symbol than the claim at their '
                         f'target, as {verify._shown(verify.RELOC_BASELINE)} lists']


def test_a_baseline_row_that_no_longer_breaks_is_reported_stale():
    rep = checked([exact_caller(), claim('?callee@@YAXXZ')], baseline=[row()])
    assert not rep.failed
    assert len(rep.lines) == 1
    assert rep.lines[0].startswith('WARN ') and rep.lines[0].endswith(
        'lists 0x00401000 +0x1 ?callee@@YAXXZ -> 0x00401200, which no longer breaks: remove the row')


def test_rows_of_functions_not_scored_are_stale_only_under_all():
    other = row(function='0x00409000')
    assert checked([exact_caller(), claim('?callee@@YAXXZ')], baseline=[other]).lines == []
    rep = checked([exact_caller(), claim('?callee@@YAXXZ')], baseline=[other], check_all=True)
    assert len(rep.lines) == 1 and 'lists 0x00409000 +0x1' in rep.lines[0]


def test_the_baseline_reads_back_what_the_breaks_write(tmp_path):
    breaks = [dict(row(), source='rebuilt'), dict(row(function='0x00400000', offset=0x2c),
                                                   source='recorded')]
    text = verify.reloc_baseline_text(breaks)
    assert text == ('function\toffset\tsymbol\ttarget\tclaimed\n'
                    '0x00400000\t0x2c\t?callee@@YAXXZ\t0x00401200\t?other@@YAXXZ\n'
                    '0x00401000\t0x1\t?callee@@YAXXZ\t0x00401200\t?other@@YAXXZ\n')
    p = tmp_path / 'reloc_baseline.tsv'
    p.write_text(text)
    assert verify.load_reloc_baseline(str(p)) == [row(function='0x00400000', offset=0x2c), row()]
    assert verify.load_reloc_baseline(str(tmp_path / 'absent.tsv')) == []


def test_a_malformed_baseline_row_stops_the_run(tmp_path):
    p = tmp_path / 'reloc_baseline.tsv'
    p.write_text('function\toffset\tsymbol\ttarget\tclaimed\n0x00401000\t0x1\t?f@@YAXXZ\n')
    with pytest.raises(verify.Untrusted, match=r':2: expected 5 tab-separated fields'):
        verify.load_reloc_baseline(str(p))


def test_run_checks_claim_symbols_and_all_writes_the_breaks(tmp_path, monkeypatch):
    monkeypatch.setattr(paths, 'SRC_ROOT', str(tmp_path))
    monkeypatch.setattr(paths, 'MATCHED_SET', str(tmp_path / 'matched_set.json'))
    monkeypatch.setattr(paths, 'BUILD', str(tmp_path / 'build'))
    monkeypatch.setattr(verify, 'RELOC_BASELINE', str(tmp_path / 'reloc_baseline.tsv'))
    monkeypatch.setattr(funcs, 'FunctionIndex', lambda *a: FakeIndex())
    monkeypatch.setattr(verify, 'check_exe', lambda rep: None)   # no retail file is read
    calls = []

    def breaks(rep, data, baseline, check_all, table=None):
        calls.append(check_all)
        rep.results = [exact_caller()]
        rep.claim_breaks = [dict(row(), source='rebuilt')]
        rep.add('ERROR', 'a relocation names another symbol than the claim')
        rep.failed = True
    monkeypatch.setattr(verify, 'report_claim_symbols', breaks)
    assert verify.main(['--all']) == verify.EXIT_FAILED
    assert calls == [True]
    assert (tmp_path / 'build' / 'reloc_breaks.tsv').read_text() == (
        'function\toffset\tsymbol\ttarget\tclaimed\n'
        '0x00401000\t0x1\t?callee@@YAXXZ\t0x00401200\t?other@@YAXXZ\n')
