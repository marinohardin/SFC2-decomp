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


def test_a_symbol_with_no_known_address_is_unchanged_by_its_claim(tmp_path):
    # it was never a mismatch (unverified, and recorded as used for one address), so a claim adds
    # nothing and must not change how the function's other relocations are judged
    idx = index(tmp_path, [])
    idx.add_claims({CALLEE}, table(SYM))
    assert first_status(idx)[0] == 'unverified'
    assert idx.claimed == {}


def test_a_claim_gives_no_credit_to_another_symbol(tmp_path):
    idx = index(tmp_path, [(0x700000, 'folded', SYM), (0x700100, 'other', OTHER)])
    assert idx.add_claims({CALLEE}, table(OTHER)) == 1
    assert first_status(idx)[0] == 'mismatch'
    assert SYM not in idx.claimed and idx.claimed[OTHER] == {CALLEE}


def test_a_claim_does_not_make_a_wrong_address_pass(tmp_path):
    idx = index(tmp_path, [(0x700000, 'folded', SYM)])
    idx.add_claims({CALLEE}, table(SYM))
    assert first_status(idx, callee=0x401300)[0] == 'mismatch'


def test_a_claim_against_the_evidence_gives_no_credit(tmp_path):
    # SYM is known elsewhere too, so only the guard on the names at the address refuses the credit
    idx = index(tmp_path, [(CALLEE, 'real', OTHER), (0x700000, 'folded', SYM)])
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


def test_a_claim_where_the_symbol_is_already_known_is_not_recorded(tmp_path):
    idx = index(tmp_path, [(CALLEE, 'real', SYM)])
    assert idx.add_claims({CALLEE}, table(SYM)) == 0
    assert idx.claimed == {}
    assert first_status(idx) == ('ok', 'known address')


def test_the_credit_list_names_the_pair_the_known_addresses_and_the_uses(tmp_path):
    from harness.lib import verify
    idx = index(tmp_path, [(0x700000, 'folded', SYM)])
    idx.add_claims({CALLEE}, table(SYM))
    results = [{'relocs': [{'detail': 'claimed address', 'symbol': SYM, 'exe_address': f'0x{CALLEE:08x}'},
                           {'detail': 'known address', 'symbol': SYM, 'exe_address': '0x00700000'},
                           {'detail': 'claimed address', 'symbol': SYM, 'exe_address': f'0x{CALLEE:08x}'}]}]
    assert verify.claimed_credits(results, idx) == [(SYM, '0x00401200', '0x00700000', 2)]
    assert verify.claimed_credits([{'relocs': []}], idx) == []     # an unused credit is not listed
