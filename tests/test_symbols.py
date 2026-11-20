"""Tests for tools/symbols.py: check, add and show on an invented table.

Rows, addresses and symbols are invented; expected output is written out by hand.
"""
import importlib.util
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('symbols_tool', ROOT / 'tools/symbols.py')
sym = importlib.util.module_from_spec(spec)
spec.loader.exec_module(sym)

HEAD = 'address\tkind\tname\tclass\tsymbol\tsymbol_evidence\tdeclaration\tevidence\n'
FUNCTIONS = ('address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,'
             'headers,is_library,library,kind,tags\n'
             '00401000,10,tTestShip::Fire,,,rtti-class-recovery,,,,0,,function,\n'
             '00401010,20,FUN_00401010,,,default,,,,0,,function,\n'
             '00401020,5,thunk_FUN_00401020,,,default,,,,0,,thunk,\n'
             '00401030,9,FUN_00401030_funclet,,,eh-analyzer,,,,0,,eh-funclet,\n')
FIRE = '?Fire@tTestShip@@QAEXXZ'
ROW_FIRE = '0x00401000\tfunction\ttTestShip::Fire\ttTestShip\t' + FIRE + '\tmangled\t\trtti'
ROW_FOLDED = '0x00401000\tfolded\ttTestShip::Fire2\ttTestShip\t?Fire2@tTestShip@@QAEXXZ\tadd\t\tsdk'
ROW_PLAIN = '0x00401010\tfunction\tfn_00401010\t\t\t\t\tnone'
ROW_THUNK = '0x00401020\tthunk\tfn_00401020\ttVtbl_00500000\t\t\t\tvtable'
GOOD = HEAD + '\n'.join([ROW_FIRE, ROW_FOLDED, ROW_PLAIN, ROW_THUNK]) + '\n'


def setup(tmp_path, text=GOOD):
    table = tmp_path / 'T.tsv'
    table.write_text(text, newline='')
    functions = tmp_path / 'functions.csv'
    functions.write_text(FUNCTIONS)
    return table, functions


def run(tmp_path, capsys, *args):
    rc = sym.main(['--table', str(tmp_path / 'T.tsv'), *args])
    return rc, capsys.readouterr().out


def check(tmp_path, capsys, text, build=None, src_root=None):
    _, functions = setup(tmp_path, text)
    b = build or tmp_path / 'nobuild'
    return run(tmp_path, capsys, 'check', '--functions', str(functions), '--build', str(b),
               '--src-root', str(src_root or tmp_path / 'noop'))


def test_a_good_table_passes_check(tmp_path, capsys):
    rc, out = check(tmp_path, capsys, GOOD)
    assert (rc, out) == (0, f'ok: 4 rows in {tmp_path / "T.tsv"}\n')


def test_check_reports_a_wrong_header_and_a_bad_column_count(tmp_path, capsys):
    text = GOOD.replace('declaration', 'decl').replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010')
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert 'ERROR line 1: the header is not address\tkind' in out
    assert 'ERROR line 4: 3 columns, expected 8' in out


def test_check_reports_an_address_not_in_0x_lowercase_hex(tmp_path, capsys):
    rc, out = check(tmp_path, capsys, GOOD.replace('0x00401010', '0x0040101A'))
    assert rc == 1
    assert "ERROR line 4: address '0x0040101A' is not 0x followed by 8 lowercase hex digits" in out


def test_check_reports_a_kind_and_an_evidence_outside_the_vocabulary(tmp_path, capsys):
    text = GOOD.replace('\tnone\n', '\tguess\n').replace('0x00401020\tthunk', '0x00401020\tdata')
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert "ERROR line 4: evidence 'guess' is not one of" in out
    assert "ERROR line 5: kind 'data' is not one of function, thunk, folded" in out


def test_check_reports_rows_out_of_order(tmp_path, capsys):
    lines = GOOD.split('\n')
    lines[3], lines[4] = lines[4], lines[3]
    rc, out = check(tmp_path, capsys, '\n'.join(lines))
    assert rc == 1
    assert 'ERROR line 5: 0x00401010 is out of order after 0x00401020' in out


def test_check_reports_an_address_that_appears_twice_without_folded(tmp_path, capsys):
    rc, out = check(tmp_path, capsys, GOOD.replace('0x00401000\tfolded', '0x00401000\tfunction'))
    assert rc == 1
    assert 'ERROR line 3: address 0x00401000 appears twice' in out


def test_check_reports_a_folded_row_without_a_primary_above_it(tmp_path, capsys):
    rc, out = check(tmp_path, capsys, GOOD.replace('0x00401000\tfolded', '0x00401005\tfolded'))
    assert rc == 1
    assert 'ERROR line 3: a folded row must directly follow a row at 0x00401005' in out


def test_check_reports_an_unknown_address_and_a_missing_function(tmp_path, capsys):
    text = GOOD.replace(ROW_PLAIN, '0x00401015\tfunction\tfn_00401015\t\t\t\t\tnone')
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert 'ERROR line 4: 0x00401015 is not a function or thunk of functions.csv' in out
    assert 'ERROR 1 functions of functions.csv have no row, first 0x00401010' in out


def test_check_reports_a_row_kind_that_differs_from_functions_csv(tmp_path, capsys):
    rc, out = check(tmp_path, capsys, GOOD.replace('0x00401020\tthunk', '0x00401020\tfunction'))
    assert rc == 1
    assert 'ERROR line 5: 0x00401020 is a thunk in functions.csv, the row says function' in out


def test_check_reports_a_placeholder_name_that_disagrees_with_its_evidence(tmp_path, capsys):
    rc, out = check(tmp_path, capsys, GOOD.replace('fn_00401010', 'tTestShip::Hold'))
    assert rc == 1
    assert "ERROR line 4: name 'tTestShip::Hold' and evidence 'none' disagree" in out


def test_check_rejects_evidence_other_than_none_or_vtable_on_a_placeholder_name(tmp_path, capsys):
    text = GOOD.replace('tTestShip::Fire\t', 'tTestShip::fn_00401000\t')
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert "ERROR line 2: name 'tTestShip::fn_00401000' has a placeholder component and is not evidence" in out
    text = GOOD.replace('0x00401000\tfunction\ttTestShip::Fire', '0x00401000\tfunction\ttObject_00401000::Fire')
    rc, out = check(tmp_path, capsys, text)
    assert "ERROR line 2: name 'tObject_00401000::Fire' has a placeholder component and is not evidence" in out


def test_check_rejects_a_placeholder_in_class_except_a_vtable_class(tmp_path, capsys):
    rc, out = check(tmp_path, capsys, GOOD)
    assert rc == 0                                  # tVtbl_00500000 on the thunk is allowed
    rc, out = check(tmp_path, capsys, GOOD.replace('tVtbl_00500000', 'tObject_00500000'))
    assert rc == 1
    assert "ERROR line 5: class 'tObject_00500000' has a placeholder component" in out
    rc, out = check(tmp_path, capsys, GOOD.replace('tVtbl_00500000', 'tVtbl_005'))
    assert rc == 0


def test_check_reports_an_implausible_and_a_per_unit_symbol(tmp_path, capsys):
    text = GOOD.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\tFire(void)\tadd\t\tnone')
    text = text.replace('?Fire2@tTestShip@@QAEXXZ', '??_C@_05abc@hello@')
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert "ERROR line 4: symbol 'Fire(void)' is not a plausible MSVC symbol" in out
    assert "ERROR line 3: symbol '??_C@_05abc@hello@' is per unit and is never recorded" in out


def test_check_reports_symbol_and_symbol_evidence_that_do_not_go_together(tmp_path, capsys):
    text = GOOD.replace(ROW_FIRE, ROW_FIRE.replace('\tmangled\t', '\t\t'))
    text = text.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\t\tcallers\t\tnone')
    text = text.replace(ROW_THUNK, ROW_THUNK.replace('\t\t\t\tvtable', '\t\tguess\t\tvtable'))
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert 'ERROR line 2: symbol and symbol_evidence must both be set or both empty' in out
    assert 'ERROR line 4: symbol and symbol_evidence must both be set or both empty' in out
    assert "ERROR line 5: symbol_evidence 'guess' is not one of claim, mangled, callers, add or empty" in out


def test_check_reports_one_symbol_at_two_addresses_folded_rows_included(tmp_path, capsys):
    text = GOOD.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\t' + FIRE + '\tadd\t\tnone')
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert f'ERROR symbol {FIRE} appears 2 times: 0x00401000 function, 0x00401010 function' in out
    text = GOOD.replace('?Fire2@tTestShip@@QAEXXZ', FIRE)
    rc, out = check(tmp_path, capsys, text)
    assert f'ERROR symbol {FIRE} appears 2 times: 0x00401000 function, 0x00401000 folded' in out


def build_folder(tmp_path):
    build = tmp_path / 'build'
    build.mkdir()
    (build / 'symbols.tsv').write_text('address\tsymbol\tfile\n'
                                       '0x00401000\t?Fire@tTestShip@@QAEHXZ\ta.cpp\n'
                                       '0x00401010\t?Stale@@YAXXZ\ta.cpp\n')
    src = tmp_path / 'op'
    src.mkdir()
    (src / 'a.cpp').write_text('// FUNCTION: STARFLEETOP 0x00401000\nvoid f() {}\n')
    return build, src


def test_check_fails_when_the_last_build_compiled_another_symbol_of_a_current_claim(tmp_path, capsys):
    build, src = build_folder(tmp_path)
    rc, out = check(tmp_path, capsys, GOOD, build=build, src_root=src)
    assert rc == 1
    assert out.startswith(f'ERROR 0x00401000: the last build compiled ?Fire@tTestShip@@QAEHXZ, '
                          f'the table has {FIRE}; run `python3 tools/symbols.py claim`\n')
    assert '0x00401010' not in out              # not a current claim: its row is stale


def test_a_build_difference_is_an_error_when_the_level_is_error():
    known = {'0x00401000': 'function'}
    text = HEAD + ROW_FIRE + '\n'
    got = sym.check_text(text, known, {'0x00401000': '?Other@@YAXXZ'}, 'ERROR')
    assert got == [f'ERROR 0x00401000: the last build compiled ?Other@@YAXXZ, the table has {FIRE}; '
                   'run `python3 tools/symbols.py claim`']


def test_add_fills_an_empty_symbol_marks_it_add_and_changes_only_that_line(tmp_path, capsys):
    table, _ = setup(tmp_path)
    rc, out = run(tmp_path, capsys, 'add', '0x00401010', '?Hold@tTestShip@@QAEXXZ',
                  '--declaration', 'void Hold(void)')
    assert (rc, out) == (0, '0x00401010: ?Hold@tTestShip@@QAEXXZ\n')
    want = GOOD.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\t?Hold@tTestShip@@QAEXXZ'
                                   '\tadd\tvoid Hold(void)\tnone')
    assert table.read_bytes() == want.encode()


def test_add_of_the_same_symbol_is_a_no_op_and_a_different_one_is_refused(tmp_path, capsys):
    table, _ = setup(tmp_path)
    rc, out = run(tmp_path, capsys, 'add', '0x00401000', FIRE)
    assert (rc, out) == (0, '0x00401000: unchanged\n')
    rc, out = run(tmp_path, capsys, 'add', '0x00401000', '?Fire@tTestShip@@QAEHXZ')
    assert rc == 1
    assert f'already has symbol {FIRE}; first writer wins' in out
    assert table.read_bytes() == GOOD.encode()


def test_add_on_a_claimed_row_is_refused_because_the_claim_defines_the_symbol(tmp_path, capsys):
    text = GOOD.replace(ROW_FIRE, ROW_FIRE.replace('\tmangled\t', '\tclaim\t'))
    table, _ = setup(tmp_path, text)
    rc, out = run(tmp_path, capsys, 'add', '0x00401000', FIRE)
    assert rc == 1
    assert f'0x00401000 is claimed: its claim defines {FIRE}' in out
    assert table.read_bytes() == text.encode()


def test_add_refuses_a_symbol_that_is_at_another_address(tmp_path, capsys):
    table, _ = setup(tmp_path)
    rc, out = run(tmp_path, capsys, 'add', '0x00401010', FIRE)
    assert rc == 1
    assert f'symbol {FIRE} is already at 0x00401000' in out
    assert table.read_bytes() == GOOD.encode()


def test_add_refuses_what_is_not_a_symbol_a_function_may_be_called_by(tmp_path, capsys):
    table, _ = setup(tmp_path)
    for address, symbol in (('0x00401010', 'tTestShip::Hold'),            # not mangled
                            ('0x00401010', '??_C@_05abc@hello@'),          # per unit
                            ('0x00401010', '__imp__Sleep@4'),               # an import
                            ('0x00401010', '?Hold@tObject_00401010@@QAEXXZ'),   # stand-in class
                            ('0x00401010', '?fn_00401010@tTestShip@@QAEXXZ'),   # placeholder name
                            ('0x00409999', '?Hold@tTestShip@@QAEXXZ')):   # no such row
        rc, out = run(tmp_path, capsys, 'add', address, symbol)
        assert rc == 1 and out.startswith('ERROR'), symbol
    assert table.read_bytes() == GOOD.encode()


def test_show_prints_the_row_and_its_folded_rows(tmp_path, capsys):
    setup(tmp_path)
    rc, out = run(tmp_path, capsys, 'show', '401000', 'fn_00401010')
    assert rc == 0
    assert out == HEAD + '\n'.join([ROW_FIRE, ROW_FOLDED, ROW_PLAIN]) + '\n'


def test_show_of_an_address_without_a_row_exits_1(tmp_path, capsys):
    setup(tmp_path)
    rc, _ = run(tmp_path, capsys, 'show', '0x00409999')
    assert rc == 1


def test_format_rows_sorts_by_address_and_puts_folded_after_the_primary():
    rows = [dict(zip(sym.COLUMNS, line.split('\t'))) for line in GOOD.split('\n')[1:-1]]
    assert sym.format_rows(reversed(rows)) == GOOD


def test_the_committed_table_passes_check():
    table = ROOT / 'analysis/symbols/STARFLEETOP.tsv'
    known = sym.known_functions(str(ROOT / 'ghidra/exports/functions.csv'))
    assert sym.check_text(table.read_text(encoding='utf-8'), known) == []


def test_check_is_quiet_when_the_build_symbol_of_a_claim_equals_the_tables(tmp_path, capsys):
    build, src = build_folder(tmp_path)
    (build / 'symbols.tsv').write_text(f'address\tsymbol\tfile\n0x00401000\t{FIRE}\ta.cpp\n')
    rc, out = check(tmp_path, capsys, GOOD, build=build, src_root=src)
    assert (rc, out) == (0, f'ok: 4 rows in {tmp_path / "T.tsv"}\n')


def test_check_fails_for_a_claim_row_whose_address_is_no_longer_claimed(tmp_path, capsys):
    build, src = build_folder(tmp_path)
    (build / 'symbols.tsv').write_text(f'address\tsymbol\tfile\n0x00401000\t{FIRE}\ta.cpp\n')
    text = GOOD.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\t?Hold@tTestShip@@QAEXXZ\tclaim\t\tnone')
    rc, out = check(tmp_path, capsys, text, build=build, src_root=src)
    assert rc == 1
    assert out.startswith('ERROR 0x00401010: symbol_evidence is claim but the address is no longer '
                          'claimed; run `python3 tools/symbols.py claim` to reset the row\n')


def claim_run(tmp_path, capsys, *args):
    return run(tmp_path, capsys, 'claim', '--build', str(tmp_path / 'build'),
               '--src-root', str(tmp_path / 'op'), *args)


def test_claim_makes_a_claimed_row_carry_the_builds_symbol_and_touches_nothing_else(tmp_path, capsys):
    table, _ = setup(tmp_path)
    build_folder(tmp_path)
    rc, out = claim_run(tmp_path, capsys)
    assert rc == 0
    assert out == (f'0x00401000: {FIRE} (mangled) -> ?Fire@tTestShip@@QAEHXZ (claim)\n'
                   '1 rows changed\n')
    want = GOOD.replace(ROW_FIRE, ROW_FIRE.replace(FIRE, '?Fire@tTestShip@@QAEHXZ').replace('mangled', 'claim'))
    assert table.read_bytes() == want.encode()
    rc, out = claim_run(tmp_path, capsys)
    assert (rc, out) == (0, '0 rows changed\n')
    assert table.read_bytes() == want.encode()


def test_claim_resets_a_claim_row_that_is_no_longer_claimed_but_only_without_addresses(tmp_path, capsys):
    text = GOOD.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\t?Hold@tTestShip@@QAEXXZ\tclaim\t\tnone')
    table, _ = setup(tmp_path, text)
    build_folder(tmp_path)
    rc, out = claim_run(tmp_path, capsys, '0x00401000')
    assert 'no longer claimed' not in out
    rc, out = claim_run(tmp_path, capsys)
    assert '0x00401010: ?Hold@tTestShip@@QAEXXZ (claim) -> no symbol (no longer claimed)\n' in out
    assert ROW_PLAIN in table.read_text()


def test_claim_refuses_a_missing_build_and_an_address_the_build_lacks(tmp_path, capsys):
    setup(tmp_path)
    rc, out = claim_run(tmp_path, capsys)
    assert rc == 1 and out.startswith('ERROR no symbols.tsv in')
    build_folder(tmp_path)
    rc, out = claim_run(tmp_path, capsys, '0x00401020')
    assert (rc, out) == (1, 'ERROR the build has no symbol for 0x00401020\n')


def test_add_may_replace_a_callers_symbol_but_not_a_mangled_or_add_one(tmp_path, capsys):
    text = GOOD.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\t?Old@@YAXXZ\tcallers\t\tnone')
    table, _ = setup(tmp_path, text)
    rc, out = run(tmp_path, capsys, 'add', '0x00401010', '?New@@YAXXZ')
    assert rc == 0
    assert out == '0x00401010: replacing the callers symbol ?Old@@YAXXZ with ?New@@YAXXZ\n' \
                  '0x00401010: ?New@@YAXXZ\n'
    assert '0x00401010\tfunction\tfn_00401010\t\t?New@@YAXXZ\tadd\t\tnone' in table.read_text()
    rc, out = run(tmp_path, capsys, 'add', '0x00401010', '?Newer@@YAXXZ')
    assert rc == 1 and 'first writer wins' in out


def test_add_refuses_a_symbol_whose_name_disagrees_with_a_named_row(tmp_path, capsys):
    row = '0x00401010\tfunction\ttTestShip::Hold\ttTestShip\t\t\t\trtti'
    setup(tmp_path, GOOD.replace(ROW_PLAIN, row))
    rc, out = run(tmp_path, capsys, 'add', '0x00401010', '?Drop@tTestShip@@QAEXXZ')
    assert (rc, out) == (1, 'ERROR 0x00401010 is named tTestShip::Hold, the symbol '
                            '?Drop@tTestShip@@QAEXXZ decodes to tTestShip::Drop\n')
    rc, out = run(tmp_path, capsys, 'add', '0x00401010', '?Hold@tTestShip@@QAEXXZ')
    assert rc == 0


def test_a_failed_write_leaves_no_temporary_file(tmp_path, monkeypatch):
    target = tmp_path / 'T.tsv'
    target.write_text('old\n')

    def boom(*a):
        raise OSError('disk full')
    monkeypatch.setattr(sym.os, 'replace', boom)
    try:
        sym.write_atomic(str(target), 'new\n')
    except OSError:
        pass
    assert target.read_text() == 'old\n'
    assert not (tmp_path / 'T.tsv.tmp').exists()
