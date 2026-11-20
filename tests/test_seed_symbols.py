"""Tests for analysis/scripts/seed_symbols.py on an invented game of two dozen functions.

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
tool = seed_mod.tool

HEAD = ('address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,headers,'
        'is_library,library,kind,tags\n')
FIRE, FIRE_CSV = '?Fire@tTestShip@@QAEXXZ', '?Fire@tTestShip@@QAEHXZ'
TABLE_HEAD = 'address\tkind\tname\tclass\tsymbol\tsymbol_evidence\tdeclaration\tevidence\n'


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
             + fn_row('00401090', 'Dup', '?Dup@@YAXXZ', 'sdk-fid')
             + fn_row('004010a0', 'Dup', '?Dup@@YAXXZ', 'sdk-fid')
             + fn_row('004010b0', 'Zap', '?Zap@@YAXXZ', 'sdk-fid')
             + fn_row('004010c0', 'FUN_004010c0_funclet', source='eh-analyzer', kind='eh-funclet')
             + fn_row('004010d0', 'FUN_004010d0')
             + fn_row('004010e0', 'tTestShip::Guess', source='ghidra-analysis')
             + fn_row('004010f0', 'FUN_004010f0')
             + fn_row('00401100', 'FUN_00401100')
             + fn_row('00401110', 'FUN_00401110')
             + fn_row('00401120', 'FUN_00401120')
             + fn_row('00401130', 'FUN_00401130')
             + fn_row('00401140', 'FUN_00401140')
             + fn_row('00401150', 'tTestShip::Foo', '?Bar@tTestShip@@QAEXXZ', 'sdk-fid'))
SDK_TSV = ('address\tmangled_name\ttier\tscore\n'
           '0x00401060\t?Turn@tTestShip@@QAEXXZ\texact\t1.0\n')
STANDIN = '?fn_004010f0@tObject_004010f0@@QAEXXZ'
CLAIM_SYMS = {'0x00401000': (FIRE, 'a.cpp'),
              '0x004010f0': (STANDIN, 'e.cpp'),
              '0x00401100': ('?fn_00401100@tTestShip@@QAEXXZ', 'e.cpp'),
              '0x00401010': ('?Stale@@YAXXZ', 'z.cpp'),         # no longer a claim
              '0x00402000': ('?Caller@@YAXXZ', 'b.cpp'),
              '0x00402100': ('?Other@@YAXXZ', 'c.cpp')}
CLAIMED = {0x401000, 0x4010f0, 0x401100, 0x402000, 0x402100}
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
        reloc('?fn_00401110@tObject_00401110@@QAEXXZ', '0x00401110'),
        reloc('?fn_00401110@tTestShip@@QAEXXZ', '0x00401120'),
        reloc('?fn_00401130@tTestShip@@QAEXXZ', '0x00401130'),
        reloc('?fn_00401140@@YAXXZ', '0x00401140'),
    ]),
    result('0x00402100', 'c.cpp', [reloc('?Aim@tTestShip@@QAEXXZ', '0x00401020'),
                                   reloc('?Lock@tOther@@QAEXXZ', '0x00401030')]),
    result('0x00402200', 'd.cpp', [reloc('?Skip@@YAXXZ', '0x00401070')], exact=False),
]

# Every row of the seed on this game, by hand: address, kind, name, class, symbol,
# symbol_evidence, declaration, evidence.
EXPECTED = TABLE_HEAD + ''.join(line + '\n' for line in (
    f'0x00401000\tfunction\ttTestShip::Fire\ttTestShip\t{FIRE}\tclaim\t\tclaim',
    '0x00401010\tfunction\ttTestShip::Hold\ttTestShip\t?Hold@tTestShip@@QAEXXZ\tmangled\t\tmangled',
    '0x00401020\tfunction\tfn_00401020\t\t?Aim@tTestShip@@QAEXXZ\tcallers\t\tnone',
    '0x00401030\tfunction\tfn_00401030\t\t\t\t\tnone',
    '0x00401040\tfunction\tfn_00401040\ttTestShip\t\t\t\tvtable',
    '0x00401050\tfunction\tfn_00401050\ttBase\t\t\t\tvtable',
    '0x00401060\tfunction\ttTestShip::Turn\ttTestShip\t\t\t\tsdk',
    '0x00401070\tfunction\ttTestShip::Dock\ttTestShip\t\t\t\trtti',
    '0x00401080\tthunk\tfn_00401080\t\t\t\t\tnone',
    '0x00401090\tfunction\tDup\t\t\t\t\tmangled',
    '0x004010a0\tfunction\tDup\t\t\t\t\tmangled',
    '0x004010b0\tfunction\tZap\t\t?Zap@@YAXXZ\tmangled\t\tmangled',
    '0x004010d0\tfunction\tfn_004010d0\ttVtbl_00500200\t\t\t\tvtable',
    '0x004010e0\tfunction\tfn_004010e0\t\t\t\t\tnone',
    f'0x004010f0\tfunction\tfn_004010f0\t\t{STANDIN}\tclaim\t\tnone',
    '0x00401100\tfunction\tfn_00401100\ttTestShip\t?fn_00401100@tTestShip@@QAEXXZ\tclaim\t\tnone',
    '0x00401110\tfunction\tfn_00401110\t\t\t\t\tnone',
    '0x00401120\tfunction\tfn_00401120\t\t\t\t\tnone',
    '0x00401130\tfunction\tfn_00401130\t\t?fn_00401130@tTestShip@@QAEXXZ\tcallers\t\tnone',
    '0x00401140\tfunction\tfn_00401140\t\t?fn_00401140@@YAXXZ\tcallers\t\tnone',
    '0x00401150\tfunction\tfn_00401150\t\t\t\t\tnone'))


def make(tmp_path):
    f = tmp_path / 'functions.csv'
    f.write_text(FUNCTIONS)
    t = tmp_path / 'sdk.tsv'
    t.write_text(SDK_TSV)
    return funcs.FunctionIndex(str(f), [str(t)])


def run(tmp_path, existing=None, reseed=False, results=RESULTS):
    return seed_mod.seed(make(tmp_path), CLAIM_SYMS, CLAIMED, VTABLES, BASES, results, existing, reseed)


def table(rows):
    return tool.format_rows(rows)


def by_address(rows):
    return {r['address']: r for r in rows}


def test_the_seed_gives_exactly_these_rows(tmp_path):
    rows, _ = run(tmp_path)
    assert table(rows) == EXPECTED


def test_the_seed_reports_counts_conflicts_and_reasons(tmp_path):
    rows, stats = run(tmp_path)
    assert stats['symbol'] == {'claim': 3, 'mangled': 2, 'callers': 3}
    assert stats['name'] == {'claim': 1, 'mangled': 4, 'sdk': 1, 'rtti': 1, 'vtable': 3, 'none': 11}
    assert stats['callers_single'] == 5
    assert stats['callers_rejected'] == {'placeholder-class': 1, 'fn-name-of-another-address': 1}
    assert stats['conflicts'] == [('0x00401030', {'?Lock@tOther@@QAEXXZ': ['0x00402100'],
                                                  '?Lock@tTestShip@@QAEXXZ': ['0x00402000']})]
    assert stats['disagree'] == [('0x00401000', {'claim': FIRE, 'mangled': FIRE_CSV})]
    assert stats['duplicates'] == [('?Dup@@YAXXZ', ['0x00401090', '0x004010a0'])]
    assert stats['mangled_names'] == [('0x00401150', '?Bar@tTestShip@@QAEXXZ', 'tTestShip::Foo')]
    text = seed_mod.report_text(rows, stats)
    assert '0x00401030\n    ?Lock@tOther@@QAEXXZ  called by 0x00402100\n' in text
    assert 'placeholder-class' in text and 'fn-name-of-another-address' in text


def test_a_symbol_from_a_row_that_is_not_a_current_claim_is_not_used(tmp_path):
    rows, _ = run(tmp_path)
    got = by_address(rows)['0x00401010']
    assert (got['symbol'], got['symbol_evidence']) == ('?Hold@tTestShip@@QAEXXZ', 'mangled')


def test_a_claim_with_a_placeholder_function_name_is_not_a_name_and_keeps_a_real_class(tmp_path):
    got = by_address(run(tmp_path)[0])
    assert (got['0x004010f0']['name'], got['0x004010f0']['class'], got['0x004010f0']['evidence']) \
        == ('fn_004010f0', '', 'none')
    assert (got['0x00401100']['name'], got['0x00401100']['class'], got['0x00401100']['evidence']) \
        == ('fn_00401100', 'tTestShip', 'none')


def test_ghidra_analysis_names_are_not_evidence(tmp_path):
    got = by_address(run(tmp_path)[0])['0x004010e0']
    assert (got['name'], got['evidence']) == ('fn_004010e0', 'none')


def test_a_claim_replaces_an_earlier_symbol_and_the_report_lists_it(tmp_path):
    rows, _ = run(tmp_path)
    existing = {r['address']: dict(r) for r in rows if r['address'] == '0x00401000'}
    existing['0x00401000'].update(symbol='?Old@@YAXXZ', symbol_evidence='add')
    again, stats = run(tmp_path, existing=existing)
    got = by_address(again)['0x00401000']
    assert (got['symbol'], got['symbol_evidence']) == (FIRE, 'claim')
    assert stats['replaced'] == [('0x00401000', '?Old@@YAXXZ', 'add', FIRE)]
    assert '0x00401000  was ?Old@@YAXXZ (add)  now ' + FIRE in seed_mod.report_text(again, stats)


def test_a_claim_symbol_follows_a_rename_on_a_reseed(tmp_path):
    rows, _ = run(tmp_path)
    existing = {r['address']: dict(r) for r in rows if r['address'] == '0x00401000'}
    renamed = dict(CLAIM_SYMS, **{'0x00401000': ('?Shoot@tTestShip@@QAEXXZ', 'a.cpp')})
    again, stats = seed_mod.seed(make(tmp_path), renamed, CLAIMED, VTABLES, BASES, RESULTS, existing)
    got = by_address(again)['0x00401000']
    assert (got['name'], got['symbol'], got['symbol_evidence']) == \
        ('tTestShip::Shoot', '?Shoot@tTestShip@@QAEXXZ', 'claim')
    assert stats['replaced'] == [('0x00401000', FIRE, 'claim', '?Shoot@tTestShip@@QAEXXZ')]


def test_an_existing_non_claim_symbol_is_kept_and_a_different_seed_value_reported(tmp_path):
    rows, _ = run(tmp_path)
    existing = {r['address']: dict(r) for r in rows if r['address'] in ('0x00401010', '0x00401030')}
    existing['0x00401010'].update(symbol='?Hold2@tTestShip@@QAEXXZ', symbol_evidence='add',
                                  declaration='int Hold(void)')
    existing['0x00401030'].update(symbol='?Lock@tTestShip@@QAEXXZ', symbol_evidence='add')
    again, stats = run(tmp_path, existing=existing)
    got = by_address(again)
    assert (got['0x00401010']['symbol'], got['0x00401010']['symbol_evidence'],
            got['0x00401010']['declaration']) == ('?Hold2@tTestShip@@QAEXXZ', 'add', 'int Hold(void)')
    assert got['0x00401030']['symbol'] == '?Lock@tTestShip@@QAEXXZ'
    assert stats['kept'] == [('0x00401010', '?Hold2@tTestShip@@QAEXXZ', '?Hold@tTestShip@@QAEXXZ')]


def test_reseed_takes_the_seeds_values_and_drops_declarations(tmp_path):
    rows, _ = run(tmp_path)
    existing = {r['address']: dict(r) for r in rows if r['address'] == '0x00401010'}
    existing['0x00401010'].update(symbol='?Hold2@tTestShip@@QAEXXZ', symbol_evidence='add',
                                  declaration='int Hold(void)')
    again, _ = run(tmp_path, existing=existing, reseed=True)
    got = by_address(again)['0x00401010']
    assert (got['symbol'], got['symbol_evidence'], got['declaration']) == \
        ('?Hold@tTestShip@@QAEXXZ', 'mangled', '')


def test_the_seed_is_deterministic(tmp_path):
    assert table(run(tmp_path)[0]) == table(run(tmp_path, results=list(reversed(RESULTS)))[0])


def test_folded_rows_are_never_seeded(tmp_path):
    assert 'folded' not in {r['kind'] for r in run(tmp_path)[0]}


def test_a_vtable_class_comes_before_the_vfunction_prefix(tmp_path):
    functions = FUNCTIONS.replace('tTestShip::vfunction3', 'tPrefix::vfunction3')
    (tmp_path / 'functions.csv').write_text(functions)
    index = funcs.FunctionIndex(str(tmp_path / 'functions.csv'))
    vt = [('0x00500300', 'tOwner', '0x00401040')]
    rows, _ = seed_mod.seed(index, CLAIM_SYMS, CLAIMED, vt, {}, [])
    assert by_address(rows)['0x00401040']['class'] == 'tOwner'


def test_decode_symbol_reads_member_constructor_destructor_operator_and_free_symbols():
    d = tool.decode_symbol
    assert d('?Fire@tTestShip@@QAEXXZ') == ('tTestShip::Fire', 'tTestShip')
    assert d('?Run@tInner@tOuter@@QAEXXZ') == ('tOuter::tInner::Run', 'tOuter::tInner')
    assert d('??0tTestShip@@QAE@XZ') == ('tTestShip::tTestShip', 'tTestShip')
    assert d('??1tTestShip@@UAE@XZ') == ('tTestShip::~tTestShip', 'tTestShip')
    assert d('??8tTestShip@@UAEJXZ') == ('tTestShip::operator==', 'tTestShip')
    assert d('?Zap@@YAXXZ') == ('Zap', '')
    assert d('?Zap@nTest@@YAXXZ') == ('nTest::Zap', '')
    assert d('?_Construct@std@@YAXPAU?$pair@H@@@Z') == ('std::_Construct', '')   # a template only in the arguments
    assert d('??_GtTestShip@@UAEPAXI@Z') is None
    assert d('??$Tpl@H@@YAXXZ') is None
    assert d('_memcpy') is None


def test_decode_symbol_refuses_a_scope_that_is_only_digits(tmp_path):
    assert tool.decode_symbol('?Fire@12345@@QAEXXZ') is None
    assert tool.decode_symbol('?1234@tTestShip@@QAEXXZ') is None


def test_the_placeholder_predicate_covers_each_shape():
    p = tool.is_placeholder
    for name in ('fn_00401000', 'FUN_00401000', 'thunk_FUN_00401000', 'fn_00401000_Field2',
                 'tObject_00401000', 'tObject_00401000_x', 'c_00401000', 'C_0040ABCD',
                 'c_tShip_Field3', 'vfunction13', '~tObject_00401000'):
        assert p(name), name
    for name in ('tShip', 'fn_0040', 'Fire', 'vfunctions', 'tObject', 'cShip'):
        assert not p(name), name
    assert tool.has_placeholder('tTestShip::fn_00401000')
    assert not tool.has_placeholder('nTest::tTestShip::Fire')


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


def test_the_table_is_written_through_a_temporary_file(tmp_path):
    target = tmp_path / 'T.tsv'
    target.write_text('old\n')
    tool.write_atomic(str(target), 'new\n')
    assert target.read_text() == 'new\n'
    assert not (tmp_path / 'T.tsv.tmp').exists()


def test_load_results_reads_each_function_folder_of_each_unit_in_order(tmp_path):
    for unit, address in (('b-1', '0x00402000'), ('a-1', '0x00401000')):
        d = tmp_path / unit / address
        d.mkdir(parents=True)
        (d / 'result.json').write_text(json.dumps({'address': address}))
    (tmp_path / 'a-1' / 'build.key').write_text('x')
    assert [r['address'] for r in seed_mod.load_results(str(tmp_path))] == ['0x00401000', '0x00402000']
