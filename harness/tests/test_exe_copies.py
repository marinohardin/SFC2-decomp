"""Copies of an exact claim in other objects, compared with the original (harness/lib/objclash.py
exe_copy_findings, harness/lib/verify.py report_exe_copies)."""
import types

from conftest import FakeIndex, make_pe
from harness.lib import coff, objclash, verify
from test_objclash import CODE, make_obj, write

FN = 0x401000                     # the claimed address in the fake exe
SYM = '?_Freeze@tString@@AAEXXZ'
CALLEE = '?_Grow@tString@@AAE_NI@Z'
EXE = bytes.fromhex('558bece8f8010000') + b'\x5d\xc3'   # push ebp; mov ebp,esp; call 0x401200; pop ebp; ret
SAME = bytes.fromhex('558bece800000000') + b'\x5d\xc3'
LONGER = bytes.fromhex('558bece800000000') + b'\x90\x5d\xc3'
CALL = [(4, CALLEE, coff.REL_REL32)]


def fixture(known=None, size=10):
    exe = coff.PeImage(make_pe(EXE + b'\xcc' * 32))
    index = FakeIndex(known if known is not None else {CALLEE: [0x401200]})
    index.by_address = {FN: types.SimpleNamespace(size=size)}
    return exe, index


def findings(tmp_path, copies, claims=None, known=None, baseline=frozenset()):
    """copies: {unit: code}; the claim is FN in a.cpp unless `claims` says otherwise."""
    objs = {u: coff.CoffObject(make_obj([(SYM, code, CODE, 2, CALL)])) for u, code in copies.items()}
    exe, index = fixture(known)
    return objclash.exe_copy_check(objs, claims or {SYM: (FN, 'a.cpp')},
                                   verify.exe_copy_compare(exe, index), baseline)


def test_a_copy_equal_to_the_original_passes(tmp_path):
    assert findings(tmp_path, {'a.cpp': SAME, 'b.cpp': SAME}) == ([], [], [])


def test_a_longer_copy_is_an_error_naming_unit_symbol_address_and_sizes(tmp_path):
    errors, stale, found = findings(tmp_path, {'a.cpp': SAME, 'b.cpp': LONGER})
    assert errors == [
        f"b.cpp: its copy of {SYM} differs from the original at 0x00401000, which a.cpp claims "
        "exact: 11 bytes against the original's 10, first differing byte +0x8 (a linker keeps "
        "the first copy it sees, so a consumer that links the objects may place this one; give "
        "the unit the definition the claim matches)"]
    assert stale == [] and found == [('b.cpp', SYM)]


def test_a_copy_that_calls_another_function_is_an_error(tmp_path):
    # the bytes agree once the call field is masked, but the exe's call reaches 0x401200, which
    # the evidence knows under another name
    errors, _, _ = findings(tmp_path, {'b.cpp': SAME}, known={'?Other@@YAXXZ': [0x401200]})
    assert errors == [
        f"b.cpp: its copy of {SYM} differs from the original at 0x00401000, which a.cpp claims "
        "exact: 10 bytes against the original's 10, first differing byte +0x4, mismatched "
        "relocations: 1, the first +0x4 "
        f"{CALLEE}: the exe address is known as ?Other@@YAXXZ (a linker keeps the first copy it "
        "sees, so a consumer that links the objects may place this one; give the unit the "
        "definition the claim matches)"]


def test_the_claiming_units_copy_and_unclaimed_symbols_are_not_compared(tmp_path):
    # a.cpp is the claim's own unit: its copy is the one scored
    assert findings(tmp_path, {'a.cpp': LONGER}) == ([], [], [])
    assert findings(tmp_path, {'b.cpp': LONGER}, claims={'?Else@@YAXXZ': (FN, 'a.cpp')}) == (
        [], [], [])


def test_a_function_outside_a_comdat_is_not_read(tmp_path):
    objs = {'b.cpp': coff.CoffObject(make_obj([(SYM, LONGER, CODE & ~coff.SCN_LNK_COMDAT, 0,
                                                CALL)]))}
    exe, index = fixture()
    assert objclash.exe_copy_findings(objs, {SYM: (FN, 'a.cpp')},
                                      verify.exe_copy_compare(exe, index)) == []


def test_the_baseline_hides_a_listed_copy_names_stale_rows_and_a_new_unit_is_an_error(tmp_path):
    base = {('b.cpp', SYM), ('c.cpp', SYM)}
    errors, stale, found = findings(tmp_path, {'b.cpp': LONGER, 'd.cpp': LONGER}, baseline=base)
    assert [e.split(':')[0] for e in errors] == ['d.cpp']
    assert stale == [f'c.cpp\t{SYM}']
    assert found == [('b.cpp', SYM), ('d.cpp', SYM)]


def test_the_baseline_file_loads_and_a_bad_row_is_refused(tmp_path):
    f = tmp_path / 'b.tsv'
    f.write_bytes(b'# note\r\nunit\tsymbol\r\n\r\nunsorted/fn_00402000.cpp\t?f@@YAXXZ\r\n')
    assert objclash.load_exe_copy_baseline(str(f)) == {('unsorted/fn_00402000.cpp', '?f@@YAXXZ')}
    assert objclash.load_exe_copy_baseline(str(tmp_path / 'none.tsv')) == set()
    bad = tmp_path / 'bad.tsv'
    bad.write_text('copy\t-\t?f@@YAXXZ\n')
    try:
        objclash.load_exe_copy_baseline(str(bad))
    except ValueError as e:
        assert 'expected unit and symbol' in str(e)
    else:
        raise AssertionError('a malformed row was accepted')


def test_only_exact_still_claimed_symbols_of_one_claim_are_compared():
    results = [
        {'address': '0x00401000', 'symbol': SYM, 'unit': 'a.cpp', 'exact': True},
        {'address': '0x00401100', 'symbol': '?near@@YAXXZ', 'unit': 'a.cpp', 'exact': False},
        {'address': '0x00401200', 'symbol': '?gone@@YAXXZ', 'unit': 'a.cpp', 'exact': True},
        {'address': '0x00401300', 'symbol': '_$E21', 'unit': 'a.cpp', 'exact': True},
        {'address': '0x00401400', 'symbol': '?two@@YAXXZ', 'unit': 'a.cpp', 'exact': True},
        {'address': '0x00401500', 'symbol': '?two@@YAXXZ', 'unit': 'b.cpp', 'exact': True},
        {'address': '0x00401600', 'unit': 'b.cpp', 'exact': False}]
    keys = {'0x00401000', '0x00401100', '0x00401300', '0x00401400', '0x00401500', '0x00401600'}
    assert verify.exact_claims(results, keys) == {SYM: (0x401000, 'a.cpp')}


def test_the_run_fails_on_a_new_copy_and_keeps_every_finding(tmp_path, monkeypatch):
    class Rep:
        def __init__(self, objects, results):
            self.objects, self.results, self.lines, self.failed = objects, results, [], False
            self.exe_copies = None

        def add(self, level, text):
            self.lines.append((level, text))
    objs = {'a.cpp': write(tmp_path, 'a.obj', [(SYM, SAME, CODE, 2, CALL)]),
            'b.cpp': write(tmp_path, 'b.obj', [(SYM, LONGER, CODE, 2, CALL)])}
    results = [{'address': '0x00401000', 'symbol': SYM, 'unit': 'a.cpp', 'exact': True}]
    monkeypatch.setattr(verify, 'claimed_keys', lambda: {'0x00401000'})
    exe, index = fixture()
    rep = Rep(objs, results)
    verify.report_exe_copies(rep, exe, index, str(tmp_path / 'none.tsv'))
    assert rep.failed and [lv for lv, _ in rep.lines] == ['ERROR', 'INFO']
    assert rep.exe_copies == [('b.cpp', SYM)]
    assert rep.lines[1][1].startswith('1 copies of exact claims in other objects differ from the '
                                      'original (0 on the baseline); copies of 1 exact claims '
                                      'compared in ')
    base = tmp_path / 'base.tsv'
    base.write_text(f'unit\tsymbol\nb.cpp\t{SYM}\nc.cpp\t{SYM}\n')
    rep = Rep(objs, results)
    verify.report_exe_copies(rep, exe, index, str(base))
    assert not rep.failed and [lv for lv, _ in rep.lines] == ['WARN', 'INFO']
    assert f'the row "c.cpp\t{SYM}" no longer occurs' in rep.lines[0][1]


def test_an_unreadable_object_is_named_in_a_warning(tmp_path, monkeypatch):
    class Rep:
        def __init__(self, objects):
            self.objects, self.results, self.lines, self.failed = objects, [], [], False
            self.exe_copies = None

        def add(self, level, text):
            self.lines.append((level, text))
    bad = tmp_path / 'b.obj'
    bad.write_bytes(b'not an object')
    monkeypatch.setattr(verify, 'claimed_keys', lambda: set())
    exe, index = fixture()
    rep = Rep({'b.cpp': str(bad)})
    verify.report_exe_copies(rep, exe, index, str(tmp_path / 'none.tsv'))
    assert not rep.failed and [lv for lv, _ in rep.lines] == ['WARN', 'INFO']
    assert rep.lines[0][1] == ('b.cpp: its object cannot be read (file too short for a COFF '
                               'header), so its copies are not compared with the original and '
                               'its baseline rows read as gone')
