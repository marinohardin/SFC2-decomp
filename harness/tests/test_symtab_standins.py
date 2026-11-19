"""The symbol table, and the stand-in check built on it: a session's function must reach a
claimed function through the claim's own symbol."""
import pytest

from conftest import FakeIndex
from test_claim_symbols import claim, exact_caller, recorded
from harness.lib import loopio, stophook, symtab, verify

STANDIN = '?fn_00401200@c_00401100_Object@@QAEXXZ'


@pytest.mark.parametrize('symbol, conv', [
    ('?f@@YAXXZ', 'cdecl'),
    ('?f@@YGXH@Z', 'stdcall'),
    ('?f@@YIXH@Z', 'fastcall'),
    ('?mGet@tDir@@QAEXXZ', 'thiscall'),
    ('??0tPath@@QAE@ABV0@@Z', 'thiscall'),
    ('?vfunction8@tScrollBar@@UAEXPBUtRect@@@Z', 'thiscall'),
    ('?Create@tFoo@@SGPAV1@XZ', 'stdcall'),
    ('?g@@3HA', ''),
    ('_main', ''),
])
def test_convention_is_read_from_the_mangled_name(symbol, conv):
    assert symtab.convention(symbol) == conv


def test_the_table_round_trips_and_a_claim_beats_other_evidence(tmp_path):
    out = tmp_path / 'symbols.tsv'
    symtab.write_claims(str(out), [('0x00401200', '?callee@@YAXXZ', 'b.cpp')])
    claims = symtab.load_claims(str(out))
    assert claims == {'0x00401200': ('?callee@@YAXXZ', 'b.cpp')}
    index = FakeIndex(known={'?other@@YAXXZ': [0x401200], '?sdk@@YAXXZ': [0x401300]})
    index.by_address = {}
    assert symtab.lookup(0x401200, index, claims) == ('?callee@@YAXXZ', 'claim', 'b.cpp')
    assert symtab.lookup(0x401300, index, claims) == ('?sdk@@YAXXZ', 'sdk', '')
    assert symtab.lookup(0x401400, index, claims) is None


def test_a_missing_table_is_empty(tmp_path):
    assert symtab.load_claims(str(tmp_path / 'none.tsv')) == {}


def checked(results, data=None, table=None):
    rep = verify.Report()
    rep.results = results
    verify.report_claim_symbols(rep, data or recorded(), [], False, table)
    return rep


def partial(result):
    return dict(result, exact=False)


def test_a_partial_function_calling_a_rebuilt_claim_through_a_stand_in_is_found():
    rep = checked([partial(exact_caller()), claim(STANDIN)])
    assert rep.claim_breaks == []
    assert [(b['function'], b['symbol'], b['claimed']) for b in rep.standins] == [
        ('0x00401000', '?callee@@YAXXZ', STANDIN)]
    assert rep.lines == [
        f'WARN  stand-in: 0x00401000 reaches 0x00401200 as ?callee@@YAXXZ, but 0x00401200 is '
        f'claimed as {STANDIN}; declare it as its claim does (the class and header that define '
        'it), not again under another name']


def test_the_table_stands_for_a_claim_this_run_did_not_rebuild():
    rep = checked([partial(exact_caller())], table={'0x00401200': (STANDIN, 'b.cpp')})
    assert [b['source'] for b in rep.standins] == ['table']


def test_a_recorded_symbol_alone_is_not_a_stand_in():
    """The matched set's symbol field can be stale, so it never refuses a session."""
    rep = checked([partial(exact_caller())], data=recorded(a00401200=STANDIN))
    assert rep.standins == []


def test_the_claims_own_symbol_is_not_a_stand_in():
    rep = checked([partial(exact_caller())], table={'0x00401200': ('?callee@@YAXXZ', 'b.cpp')})
    assert rep.standins == []


CLAIM = {'address': '0x00401000', 'baseline_score': 10.0, 'baseline_exact': False,
         'max_blocks': 3}


def verify_json(standins):
    return {'exit': 0, 'lines': [], 'standins': standins,
            'results': [{'address': '0x00401000', 'score': 100.0, 'exact': True, 'relocs': []}]}


def test_the_stop_hook_refuses_a_stand_in_even_when_exact():
    b = dict(function='0x00401000', offset=1, symbol='?callee@@YAXXZ', target='0x00401200',
             claimed=STANDIN, source='table')
    allow, why = stophook.decide(CLAIM, verify_json([b]), 0)
    assert not allow
    assert f'0x00401200 is claimed as {STANDIN}; this function names ?callee@@YAXXZ' in why


def test_the_stop_hook_ignores_another_functions_stand_in():
    b = dict(function='0x00409999', offset=1, symbol='?x@@YAXXZ', target='0x00401200',
             claimed=STANDIN, source='table')
    allow, _ = stophook.decide(CLAIM, verify_json([b]), 0)
    assert allow


def test_the_loop_does_not_commit_a_function_holding_a_stand_in():
    b = dict(function='0x00401000', offset=1, symbol='?callee@@YAXXZ', target='0x00401200',
             claimed=STANDIN, source='table')
    assert loopio.score(verify_json([b]), '0x00401000') == (100.0, True, loopio.EXIT_STANDIN)
    assert loopio.score(verify_json([]), '0x00401000') == (100.0, True, 0)
