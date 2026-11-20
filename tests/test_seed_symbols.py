"""Tests for analysis/scripts/seed_symbols.py on an invented game of a dozen functions.

The functions, symbols and results are invented; every expected row is written out by hand.
"""
import importlib.util
import json
import os
import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from harness.lib import funcs  # noqa: E402

spec = importlib.util.spec_from_file_location('seed_symbols', ROOT / 'analysis/scripts/seed_symbols.py')
seed_mod = importlib.util.module_from_spec(spec)
spec.loader.exec_module(seed_mod)

HEAD = ('address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,headers,'
        'is_library,library,kind,tags\n')
FIRE, FIRE_CSV = '?Fire@tTestShip@@QAEXXZ', '?Fire@tTestShip@@QAEHXZ'


def fn_row(address, name, mangled='', source='default', kind='function'):
    return f'{address},10,{name},{mangled},,{source},,,,0,,{kind},\n'


FUNCTIONS = (HEAD
             + fn_row('00401000', 'tTestShip::Fire', FIRE_CSV, 'sdklib-exact')
             + fn_row('00401010', 'tTestShip::Hold', '?Hold@tTestShip@@QAEXXZ', 'sdk-fid')
             + fn_row('00401020', 'FUN_00401020')
             + fn_row('00401030', 'FUN_00401030')
             + fn_row('00401040', 'tTestShip::vfunction3', source='rtti-class-recovery')
             + fn_row('00401050', 'FUN_00401050')
             + fn_row('00401060', 'FUN_00401060')
             + fn_row('00401070', 'tTestShip::Dock', source='rtti-class-recovery')
             + fn_row('00401080', 'thunk_FUN_00401080', kind='thunk')
             + fn_row('00401090', 'tTestShip::Dup1', '?Dup@@YAXXZ', 'sdk-fid')
             + fn_row('004010a0', 'tTestShip::Dup2', '?Dup@@YAXXZ', 'sdk-fid')
             + fn_row('004010b0', 'Zap', '?Zap@@YAXXZ', 'sdk-fid')
             + fn_row('004010c0', 'FUN_004010c0_funclet', source='eh-analyzer', kind='eh-funclet')
             + fn_row('004010d0', 'FUN_004010d0'))
SDK_TSV = ('address\tmangled_name\ttier\tscore\n'
           '0x00401060\t?Turn@tTestShip@@QAEXXZ\texact\t1.0\n'
           '0x004010b0\t?Zap@@YAXXZ\texact\t1.0\n'
           '0x004010b0\t?Zap2@@YAXXZ\texact\t1.0\n')
CLAIM_SYMS = {'0x00401000': (FIRE, 'a.cpp'), '0x00402000': ('?Caller@@YAXXZ', 'b.cpp'),
              '0x00402100': ('?Other@@YAXXZ', 'c.cpp')}
VTABLES = [('0x00500000', 'tDerived', '0x00401050'), ('0x00500100', 'tBase', '0x00401050'),
           ('0x00500200', '', '0x004010d0')]
BASES = {'tDerived': {'tBase'}, 'tBase': set()}


def reloc(symbol, target, status='ok', kind='REL32', file_local=False):
    return dict(offset=1, type=kind, symbol=symbol, exe_address=target, status=status,
                detail='', file_local=file_local)


def result(address, unit, relocs, exact=True):
    return dict(address=address, unit=unit, exact=exact, relocs=relocs)


RESULTS = [
    result('0x00402000', 'b.cpp', [
        reloc('?Aim@tTestShip@@QAEXXZ', '0x00401020'),
        reloc('?Lock@tTestShip@@QAEXXZ', '0x00401030'),
        reloc(FIRE, '0x00401000'),                              # a claimed target: not seeded
        reloc('$L1234', '0x00401070'),                          # a label: per unit
        reloc('?Nope@@YAXXZ', '0x00401070', status='mismatch'),
        reloc('?Data@@3HA', '0x00409999'),                      # not a function
    ]),
    result('0x00402100', 'c.cpp', [reloc('?Aim@tTestShip@@QAEXXZ', '0x00401020'),
                                   reloc('?Lock@tOther@@QAEXXZ', '0x00401030')]),
    result('0x00402200', 'd.cpp', [reloc('?Skip@@YAXXZ', '0x00401070')], exact=False),
]


def make(tmp_path):
    f = tmp_path / 'functions.csv'
    f.write_text(FUNCTIONS)
    t = tmp_path / 'sdk.tsv'
    t.write_text(SDK_TSV)
    return funcs.FunctionIndex(str(f), [str(t)])


def run(tmp_path, existing=None, reseed=False, folded=True, results=RESULTS):
    return seed_mod.seed(make(tmp_path), CLAIM_SYMS, {0x401000, 0x402000, 0x402100, 0x401068},
                         VTABLES, BASES, results, existing, reseed, folded)


def by_address(rows):
    return {(r['address'], r['kind']): r for r in rows}


def cells(r):
    return (r['name'], r['class'], r['symbol'], r['evidence'])


def test_a_claim_wins_over_the_mangled_name_and_the_disagreement_is_reported(tmp_path):
    rows, stats = run(tmp_path)
    assert cells(by_address(rows)[('0x00401000', 'function')]) == \
        ('tTestShip::Fire', 'tTestShip', FIRE, 'claim')
    assert stats['disagree'] == [('0x00401000', {'claim': FIRE, 'mangled': FIRE_CSV})]


def test_a_mangled_name_gives_name_class_and_symbol(tmp_path):
    rows, _ = run(tmp_path)
    assert cells(by_address(rows)[('0x00401010', 'function')]) == \
        ('tTestShip::Hold', 'tTestShip', '?Hold@tTestShip@@QAEXXZ', 'mangled')


def test_callers_that_agree_seed_the_symbol_of_an_unclaimed_function(tmp_path):
    rows, stats = run(tmp_path)
    assert cells(by_address(rows)[('0x00401020', 'function')]) == \
        ('fn_00401020', '', '?Aim@tTestShip@@QAEXXZ', 'none')
    assert stats['symbol'] == {'claim': 1, 'mangled': 2, 'callers': 1}


def test_callers_that_disagree_leave_the_symbol_empty_and_are_reported(tmp_path):
    rows, stats = run(tmp_path)
    assert by_address(rows)[('0x00401030', 'function')]['symbol'] == ''
    assert stats['conflicts'] == [('0x00401030', {'?Lock@tOther@@QAEXXZ': ['0x00402100'],
                                                  '?Lock@tTestShip@@QAEXXZ': ['0x00402000']})]
    assert '0x00401030\n    ?Lock@tOther@@QAEXXZ  called by 0x00402100\n' in \
        seed_mod.report_text(rows, stats)


def test_a_relocation_of_a_claimed_label_mismatch_or_inexact_caller_is_not_a_source(tmp_path):
    rows, _ = run(tmp_path)
    assert by_address(rows)[('0x00401070', 'function')]['symbol'] == ''


def test_a_vfunction_name_is_not_a_name_but_gives_the_class(tmp_path):
    rows, _ = run(tmp_path)
    assert cells(by_address(rows)[('0x00401040', 'function')]) == \
        ('fn_00401040', 'tTestShip', '', 'vtable')


def test_a_function_in_vtables_of_a_base_and_a_derived_class_belongs_to_the_base(tmp_path):
    rows, _ = run(tmp_path)
    assert cells(by_address(rows)[('0x00401050', 'function')]) == \
        ('fn_00401050', 'tBase', '', 'vtable')


def test_a_vtable_without_a_class_name_gives_the_placeholder_class(tmp_path):
    rows, _ = run(tmp_path)
    assert cells(by_address(rows)[('0x004010d0', 'function')]) == \
        ('fn_004010d0', 'tVtbl_00500200', '', 'vtable')


def test_an_unambiguous_sdk_label_names_the_function_but_is_not_seeded_as_a_symbol(tmp_path):
    rows, _ = run(tmp_path)
    assert cells(by_address(rows)[('0x00401060', 'function')]) == \
        ('tTestShip::Turn', 'tTestShip', '', 'sdk')


def test_an_rtti_name_is_a_name_and_a_thunk_without_evidence_keeps_its_kind(tmp_path):
    rows, _ = run(tmp_path)
    got = by_address(rows)
    assert cells(got[('0x00401070', 'function')]) == ('tTestShip::Dock', 'tTestShip', '', 'rtti')
    assert (got[('0x00401080', 'thunk')]['name'], got[('0x00401080', 'thunk')]['evidence']) == \
        ('fn_00401080', 'none')


def test_an_exception_funclet_gets_no_row(tmp_path):
    rows, _ = run(tmp_path)
    assert '0x004010c0' not in {r['address'] for r in rows}
    assert len(rows) == 14      # 13 functions and thunks, 1 folded


def test_a_symbol_at_two_addresses_is_left_empty_at_both_and_reported(tmp_path):
    rows, stats = run(tmp_path)
    got = by_address(rows)
    assert got[('0x00401090', 'function')]['symbol'] == got[('0x004010a0', 'function')]['symbol'] == ''
    assert stats['duplicates'] == [('?Dup@@YAXXZ', ['0x00401090', '0x004010a0'])]


def test_a_second_sdk_name_at_one_address_is_a_folded_row_after_the_primary(tmp_path):
    rows, _ = run(tmp_path)
    at = [(r['kind'], r['name'], r['symbol'], r['evidence']) for r in rows if r['address'] == '0x004010b0']
    assert at == [('function', 'Zap', '?Zap@@YAXXZ', 'mangled'),
                  ('folded', 'Zap2', '?Zap2@@YAXXZ', 'sdk')]
    rows, _ = run(tmp_path, folded=False)
    assert len([r for r in rows if r['address'] == '0x004010b0']) == 1


def test_the_seed_is_deterministic(tmp_path):
    a = seed_mod.tool.format_rows(run(tmp_path)[0])
    b = seed_mod.tool.format_rows(run(tmp_path, results=list(reversed(RESULTS)))[0])
    assert a == b and a.startswith('address\tkind\tname\tclass\tsymbol\tdeclaration\tevidence\n')


def test_the_seeded_table_passes_check(tmp_path):
    text = seed_mod.tool.format_rows(run(tmp_path)[0])
    known = {f'0x{int(r.split(",")[0], 16):08x}': r.split(',')[11] for r in FUNCTIONS.split('\n')[1:-1]
             if r.split(',')[11] != 'eh-funclet'}
    assert seed_mod.tool.check_text(text, known) == []


def test_reseeding_keeps_a_symbol_and_declaration_added_by_hand(tmp_path):
    rows, _ = run(tmp_path)
    existing = {r['address']: dict(r) for r in rows if r['address'] in ('0x00401000', '0x00401030')}
    existing['0x00401030'].update(symbol='?Lock@tTestShip@@QAEXXZ', declaration='void Lock(void)')
    existing['0x00401000'].update(symbol='?Fire@tTestShip@@QAEXXZ', declaration='void Fire(void)')
    again, stats = run(tmp_path, existing=existing)
    got = by_address(again)
    assert (got[('0x00401030', 'function')]['symbol'], got[('0x00401030', 'function')]['declaration']) \
        == ('?Lock@tTestShip@@QAEXXZ', 'void Lock(void)')
    assert got[('0x00401000', 'function')]['declaration'] == 'void Fire(void)'
    assert stats['symbol']['existing'] == 2


def test_an_existing_symbol_that_differs_from_the_seed_is_kept_and_reported(tmp_path):
    rows, _ = run(tmp_path)
    existing = {r['address']: dict(r) for r in rows if r['kind'] != 'folded'}
    existing['0x00401010']['symbol'] = '?Hold2@tTestShip@@QAEXXZ'
    again, stats = run(tmp_path, existing=existing)
    assert by_address(again)[('0x00401010', 'function')]['symbol'] == '?Hold2@tTestShip@@QAEXXZ'
    assert stats['kept'] == [('0x00401010', '?Hold2@tTestShip@@QAEXXZ', '?Hold@tTestShip@@QAEXXZ')]


def test_reseed_takes_the_seeds_values_and_drops_declarations(tmp_path):
    rows, _ = run(tmp_path)
    existing = {r['address']: dict(r) for r in rows if r['kind'] != 'folded'}
    existing['0x00401010'].update(symbol='?Hold2@tTestShip@@QAEXXZ', declaration='int Hold(void)')
    again, _ = run(tmp_path, existing=existing, reseed=True)
    got = by_address(again)[('0x00401010', 'function')]
    assert (got['symbol'], got['declaration']) == ('?Hold@tTestShip@@QAEXXZ', '')


def test_decode_symbol_reads_plain_member_constructor_destructor_and_operator_symbols():
    d = seed_mod.decode_symbol
    assert d('?Fire@tTestShip@@QAEXXZ') == ('tTestShip::Fire', 'tTestShip')
    assert d('?Run@tInner@tOuter@@QAEXXZ') == ('tOuter::tInner::Run', 'tOuter::tInner')
    assert d('??0tTestShip@@QAE@XZ') == ('tTestShip::tTestShip', 'tTestShip')
    assert d('??1tTestShip@@UAE@XZ') == ('tTestShip::~tTestShip', 'tTestShip')
    assert d('??8tTestShip@@UAEJXZ') == ('tTestShip::operator==', 'tTestShip')
    assert d('?Zap@@YAXXZ') == ('Zap', '')
    assert d('??_GtTestShip@@UAEPAXI@Z') is None
    assert d('??$Tpl@H@@YAXXZ') is None
    assert d('_memcpy') is None


def test_the_seed_refuses_a_missing_or_stale_build(tmp_path, capsys):
    src = tmp_path / 'op'
    src.mkdir()
    (src / 'a.cpp').write_text('// FUNCTION: STARFLEETOP 0x00401000\n')
    build = tmp_path / 'build'
    build.mkdir()
    args = ['--report', str(tmp_path / 'r.txt'), '--out', str(tmp_path / 'T.tsv'),
            '--build', str(build), '--src-root', str(src)]
    assert seed_mod.main(args) == 1
    assert 'run harness/build-and-verify.sh --all first' in capsys.readouterr().err
    (build / 'symbols.tsv').write_text('address\tsymbol\tfile\n')
    os.utime(build / 'symbols.tsv', (1, 1))
    assert seed_mod.main(args) == 1
    assert 'is older than the sources' in capsys.readouterr().err
    assert not (tmp_path / 'T.tsv').exists()


def test_load_results_reads_each_function_folder_of_each_unit_in_order(tmp_path):
    for unit, address in (('b-1', '0x00402000'), ('a-1', '0x00401000')):
        d = tmp_path / unit / address
        d.mkdir(parents=True)
        (d / 'result.json').write_text(json.dumps({'address': address}))
    (tmp_path / 'a-1' / 'build.key').write_text('x')
    assert [r['address'] for r in seed_mod.load_results(str(tmp_path))] == ['0x00401000', '0x00402000']
