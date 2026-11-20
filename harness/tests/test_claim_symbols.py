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


def objects_defining(tmp_path, **units):
    """{unit: object file} for units that each define the one symbol given (`a_cpp` is a.cpp)."""
    out = {}
    for unit, symbol in units.items():
        path = tmp_path / (unit + '.obj')
        path.write_bytes(coff.write_object(b'\xc3', symbol, []))
        out[unit.replace('_cpp', '.cpp')] = str(path)
    return out


def checked(results, data=None, baseline=(), check_all=False, objects=None, evidence=None):
    rep = verify.Report()
    rep.results = results
    rep.objects = objects or {}
    verify.report_claim_symbols(rep, data or recorded(), list(baseline), check_all, None,
                                evidence)
    return rep


def row(function='0x00401000', offset=1, symbol='?callee@@YAXXZ', target_='0x00401200',
        claimed='?other@@YAXXZ'):
    return dict(function=function, offset=offset, symbol=symbol, target=target_, claimed=claimed)


def test_a_call_naming_the_claimed_symbol_passes():
    rep = checked([exact_caller(), claim('?callee@@YAXXZ')])
    assert rep.claim_breaks == [] and not rep.failed
    assert rep.lines == []


STANDIN = '?fn_00401200@c_00401100_Object@@QAEXXZ'


def test_a_symbol_defined_in_another_object_is_an_error(tmp_path):
    rep = checked([exact_caller(), claim(STANDIN)],
                  objects=objects_defining(tmp_path, c_cpp='?callee@@YAXXZ'))
    assert rep.failed
    assert rep.lines == [
        'ERROR 0x00401000 relocation +0x1 names ?callee@@YAXXZ, which reaches 0x00401200, '
        f'claimed as {STANDIN}: it is defined in another object (c.cpp): a consumer would bind '
        'the call to that body']
    assert rep.claim_breaks == [dict(function='0x00401000', offset=1, symbol='?callee@@YAXXZ',
                                     target='0x00401200', claimed=STANDIN, source='rebuilt',
                                     **{'class': 'other-object', 'detail': 'c.cpp'})]


def test_a_symbol_the_evidence_puts_at_another_address_is_an_error():
    rep = checked([exact_caller(), claim(STANDIN)],
                  evidence=FakeIndex(known={'?callee@@YAXXZ': [0x401500]}))
    assert rep.failed
    assert rep.lines == [
        'ERROR 0x00401000 relocation +0x1 names ?callee@@YAXXZ, which reaches 0x00401200, '
        f'claimed as {STANDIN}: the evidence has it at another address (0x00401500): a '
        'consumer would bind the call there']


def test_a_target_the_evidence_names_otherwise_is_an_error():
    rep = checked([exact_caller(), claim(STANDIN)],
                  evidence=FakeIndex(known={'?real@@YAXXZ': [0x401200]}))
    assert rep.failed
    assert rep.lines == [
        'ERROR 0x00401000 relocation +0x1 names ?callee@@YAXXZ, which reaches 0x00401200, '
        f'claimed as {STANDIN}: it names another function than the one at this address '
        '(?real@@YAXXZ)']


def test_a_symbol_among_several_evidence_names_of_the_target_is_folded():
    rep = checked([exact_caller(), claim(STANDIN)], evidence=FakeIndex(
        known={'?callee@@YAXXZ': [0x401200], '?real@@YAXXZ': [0x401200]}))
    assert not rep.failed and [b['class'] for b in rep.claim_breaks] == ['folded']
    assert rep.lines[0].startswith('WARN  1 relocations of exact functions name a symbol the '
                                   'evidence gives the target together with other names')
    # the evidence naming the symbol at another address still wins
    rep = checked([exact_caller(), claim(STANDIN)], evidence=FakeIndex(
        known={'?callee@@YAXXZ': [0x401200, 0x401500], '?real@@YAXXZ': [0x401200]}))
    assert rep.failed and [b['class'] for b in rep.claim_breaks] == ['other-address']


def test_a_new_stand_in_of_an_exact_function_is_refused_an_old_one_is_not(tmp_path, monkeypatch):
    monkeypatch.setattr(paths, 'BUILD', str(tmp_path))
    results = [exact_caller(), claim(STANDIN)]
    assert checked(results).standins == []          # no startup file: loosened
    (tmp_path / 'reloc_breaks.tsv').write_text(verify.reloc_baseline_text(
        [dict(row(), **{'class': 'stand-in'})], classes=True))
    assert checked(results).standins == []          # the startup --all had it
    (tmp_path / 'reloc_breaks.tsv').write_text(verify.reloc_baseline_text([], classes=True))
    assert [b['class'] for b in checked(results).standins] == ['stand-in']


def test_a_stand_in_nobody_else_knows_is_a_warning_only():
    rep = checked([exact_caller(), claim(STANDIN)], evidence=FakeIndex(known={'?x@@YAXXZ': [0x1]}))
    assert not rep.failed
    assert [b['class'] for b in rep.claim_breaks] == ['stand-in']
    assert rep.lines == [
        'WARN  1 relocations of exact functions name a stand-in no other object defines and no '
        'evidence places elsewhere: resolved by name or by address (class stand-in); --all '
        'lists them in reloc_breaks.tsv']
    assert rep.standins == []


def test_the_evidence_naming_the_symbol_at_this_address_is_a_warning_only():
    rep = checked([exact_caller(), claim(STANDIN)],
                  evidence=FakeIndex(known={'?callee@@YAXXZ': [0x401200]}))
    assert not rep.failed and [b['class'] for b in rep.claim_breaks] == ['stand-in']


def test_a_symbol_the_callers_own_object_defines_is_its_own_warning(tmp_path):
    rep = checked([exact_caller(), claim(STANDIN)],
                  objects=objects_defining(tmp_path, a_cpp='?callee@@YAXXZ'))
    assert not rep.failed
    assert [b['class'] for b in rep.claim_breaks] == ['own-object']
    assert rep.lines == [
        'WARN  1 relocations of exact functions name a symbol their own object defines (an '
        'inline or template instance): unresolved, whether a consumer binds it to that body is '
        'open (class own-object); --all lists them in reloc_breaks.tsv']


def test_a_definition_in_another_object_beats_the_own_object_class(tmp_path):
    rep = checked([exact_caller(), claim(STANDIN)], objects=objects_defining(
        tmp_path, a_cpp='?callee@@YAXXZ', c_cpp='?callee@@YAXXZ'))
    assert rep.failed and [b['class'] for b in rep.claim_breaks] == ['other-object']


def test_a_functions_own_stand_in_is_still_a_stand_in_until_it_is_exact():
    partial_caller = dict(exact_caller(), exact=False)
    rep = checked([partial_caller, claim(STANDIN)])
    assert [b['class'] for b in rep.standins] == ['stand-in']
    rep = checked([exact_caller(), claim(STANDIN)])
    assert rep.standins == []


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


def test_an_error_class_break_against_a_recorded_symbol_only_warns(tmp_path):
    # The claiming unit was not built in this run: its matched-set symbol may be stale.
    rep = checked([exact_caller()], data=recorded(a00401200='?other@@YAXXZ'),
                  objects=objects_defining(tmp_path, c_cpp='?callee@@YAXXZ'))
    assert not rep.failed
    assert len(rep.lines) == 1 and rep.lines[0].startswith(
        'WARN  0x00401000 relocation +0x1 names ?callee@@YAXXZ, which reaches 0x00401200, '
        'claimed as ?other@@YAXXZ: it is defined in another object (c.cpp)')
    assert rep.lines[0].endswith('(the claim is from matched_set.json, not rebuilt in this '
                                 'run; the recorded symbol may be stale; --all decides)')


def test_a_break_the_baseline_lists_is_tolerated(tmp_path):
    rep = checked([exact_caller(), claim('?other@@YAXXZ')], baseline=[row(offset=0x40)],
                  objects=objects_defining(tmp_path, c_cpp='?callee@@YAXXZ'))
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


def test_a_baseline_row_that_is_only_a_warning_now_is_stale_under_all():
    results = [exact_caller(), claim('?other@@YAXXZ')]
    rep = checked(results, baseline=[row()], check_all=True)
    assert not rep.failed
    assert len(rep.lines) == 2 and rep.lines[1].endswith(
        'which is now only a warning (stand-in): remove the row')
    # a single-function run may not know every object, so it does not ask for the removal
    assert all('remove the row' not in line for line in checked(results, baseline=[row()]).lines)


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
    # the file an --all run writes has the class as a sixth column, which the reader ignores
    classed = verify.reloc_baseline_text([dict(b, **{'class': 'stand-in'}) for b in breaks],
                                         classes=True)
    assert classed.splitlines()[0] == 'function\toffset\tsymbol\ttarget\tclaimed\tclass'
    assert classed.splitlines()[1].endswith('\t?other@@YAXXZ\tstand-in')
    p.write_text(classed)
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

    def breaks(rep, data, baseline, check_all, table=None, evidence=None):
        calls.append(check_all)
        rep.results = [exact_caller()]
        rep.claim_breaks = [dict(row(), source='rebuilt', **{'class': 'stand-in'})]
        rep.add('ERROR', 'a relocation names another symbol than the claim')
        rep.failed = True
    monkeypatch.setattr(verify, 'report_claim_symbols', breaks)
    assert verify.main(['--all']) == verify.EXIT_FAILED
    assert calls == [True]
    assert (tmp_path / 'build' / 'reloc_breaks.tsv').read_text() == (
        'function\toffset\tsymbol\ttarget\tclaimed\tclass\n'
        '0x00401000\t0x1\t?callee@@YAXXZ\t0x00401200\t?other@@YAXXZ\tstand-in\n')


def test_defined_symbols_reads_external_definitions_per_unit(tmp_path):
    objects = objects_defining(tmp_path, a_cpp='?f@@YAXXZ', c_cpp='?f@@YAXXZ')
    objects['gone.cpp'] = str(tmp_path / 'missing.obj')
    assert verify.defined_symbols(objects) == {'?f@@YAXXZ': {'a.cpp', 'c.cpp'}}
