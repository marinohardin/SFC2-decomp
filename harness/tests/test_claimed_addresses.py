"""An address claimed in the source counts for the symbol its table row carries, and only that."""
from conftest import make_pe
from harness.lib import coff, funcs, target

from test_target import BASE_CODE, CALLEE, FN, RELOCS, exe_code

SYM = '?callee@@YAXXZ'
OTHER = '?other@@YAXXZ'
HEADER = 'address,size,name,mangled,demangled,name_source,translation_unit,is_library,kind\n'


def index(tmp_path, rows):
    f = tmp_path / 'functions.csv'
    f.write_text(HEADER + ''.join(f'{a:08x},16,{n},{m},,x,,0,function\n' for a, n, m in rows))
    return funcs.FunctionIndex(str(f))


def first_status(idx, callee=CALLEE):
    obj = coff.CoffObject(coff.write_object(BASE_CODE, '?f@@YAXXZ', RELOCS))
    exe = coff.PeImage(make_pe(exe_code(callee=callee) + b'\xcc' * 32))
    t = target.build(FN, 11, obj, '?f@@YAXXZ', exe, idx)
    return t['relocs'][0].status, t['relocs'][0].detail


def table(sym, evidence='claim', address=CALLEE):
    return {f'0x{address:08x}': (sym, evidence)}


def test_a_claimed_address_is_accepted_for_the_claimed_symbol(tmp_path):
    # the symbol is known only at the folded copy, 0x00700000; the claim is at CALLEE
    idx = index(tmp_path, [(0x700000, 'folded', SYM)])
    assert first_status(idx)[0] == 'mismatch'
    assert idx.add_claims({CALLEE}, table(SYM)) == 1
    assert first_status(idx) == ('ok', 'claimed address')


def test_the_folded_address_keeps_working(tmp_path):
    idx = index(tmp_path, [(0x700000, 'folded', SYM)])
    idx.add_claims({CALLEE}, table(SYM))
    assert first_status(idx, callee=0x700000) == ('ok', 'known address')


def test_a_claim_gives_no_credit_to_another_symbol(tmp_path):
    idx = index(tmp_path, [])
    idx.add_claims({CALLEE}, table(OTHER))
    assert first_status(idx)[0] == 'unverified'
    assert SYM not in idx.claimed and idx.claimed[OTHER] == {CALLEE}


def test_a_claim_does_not_make_a_wrong_address_pass(tmp_path):
    idx = index(tmp_path, [(0x700000, 'folded', SYM)])
    idx.add_claims({CALLEE}, table(SYM))
    assert first_status(idx, callee=0x401300)[0] == 'mismatch'


def test_a_claim_against_the_evidence_gives_no_credit(tmp_path):
    idx = index(tmp_path, [(CALLEE, 'real', OTHER)])
    assert idx.add_claims({CALLEE}, table(SYM)) == 0
    assert idx.claimed == {}
    assert first_status(idx)[0] == 'mismatch'


def test_only_claim_rows_of_claimed_addresses_count(tmp_path):
    idx = index(tmp_path, [])
    for evidence in ('mangled', 'add', 'callers'):
        assert idx.add_claims({CALLEE}, table(SYM, evidence)) == 0
    assert idx.add_claims(set(), table(SYM)) == 0           # no annotation claims the address
    assert idx.add_claims({CALLEE}, table('fn_00401200')) == 0   # not a decorated symbol
    assert idx.claimed == {}
