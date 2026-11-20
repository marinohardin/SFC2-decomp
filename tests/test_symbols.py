"""Tests for tools/symbols.py: check, add and show on an invented table.

Rows, addresses and symbols are invented; expected output is written out by hand.
"""
import importlib.util
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('symbols_tool', ROOT / 'tools/symbols.py')
sym = importlib.util.module_from_spec(spec)
spec.loader.exec_module(sym)

HEAD = 'address\tkind\tname\tclass\tsymbol\tdeclaration\tevidence\n'
FUNCTIONS = ('address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,'
             'headers,is_library,library,kind,tags\n'
             '00401000,10,tTestShip::Fire,,,rtti-class-recovery,,,,0,,function,\n'
             '00401010,20,FUN_00401010,,,default,,,,0,,function,\n'
             '00401020,5,thunk_FUN_00401020,,,default,,,,0,,thunk,\n'
             '00401030,9,FUN_00401030_funclet,,,eh-analyzer,,,,0,,eh-funclet,\n')
FIRE = '?Fire@tTestShip@@QAEXXZ'
ROW_PLAIN = '0x00401010\tfunction\tfn_00401010\t\t\t\tnone'
GOOD = (HEAD
        + '0x00401000\tfunction\ttTestShip::Fire\ttTestShip\t' + FIRE + '\t\trtti\n'
        + '0x00401000\tfolded\ttTestShip::Fire2\ttTestShip\t?Fire2@tTestShip@@QAEXXZ\t\tsdk\n'
        + ROW_PLAIN + '\n'
        + '0x00401020\tthunk\tfn_00401020\ttVtbl_00500000\t\t\tvtable\n')


def setup(tmp_path, text=GOOD):
    table = tmp_path / 'T.tsv'
    table.write_text(text, newline='')
    functions = tmp_path / 'functions.csv'
    functions.write_text(FUNCTIONS)
    return table, functions


def run(tmp_path, capsys, *args):
    rc = sym.main(['--table', str(tmp_path / 'T.tsv'), *args])
    return rc, capsys.readouterr().out


def check(tmp_path, capsys, text, build=None):
    _, functions = setup(tmp_path, text)
    b = build or tmp_path / 'nobuild'
    return run(tmp_path, capsys, 'check', '--functions', str(functions), '--build', str(b))


def test_a_good_table_passes_check(tmp_path, capsys):
    rc, out = check(tmp_path, capsys, GOOD)
    assert (rc, out) == (0, f'ok: 4 rows in {tmp_path / "T.tsv"}\n')


def test_check_reports_a_wrong_header_and_a_bad_column_count(tmp_path, capsys):
    text = GOOD.replace('declaration', 'decl').replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010')
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert 'ERROR line 1: the header is not address\tkind' in out
    assert 'ERROR line 4: 3 columns, expected 7' in out


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
    text = GOOD.replace(ROW_PLAIN, '0x00401015\tfunction\tfn_00401015\t\t\t\tnone')
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


def test_check_reports_an_implausible_and_a_per_unit_symbol(tmp_path, capsys):
    text = GOOD.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\tFire(void)\t\tnone')
    text = text.replace('?Fire2@tTestShip@@QAEXXZ', '??_C@_05abc@hello@')
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert "ERROR line 4: symbol 'Fire(void)' is not a plausible MSVC symbol" in out
    assert "ERROR line 3: symbol '??_C@_05abc@hello@' is per unit and is never recorded" in out


def test_check_reports_one_symbol_at_two_addresses(tmp_path, capsys):
    text = GOOD.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\t' + FIRE + '\t\tnone')
    rc, out = check(tmp_path, capsys, text)
    assert rc == 1
    assert f'ERROR symbol {FIRE} is at 2 addresses: 0x00401000, 0x00401010' in out


def test_check_warns_when_the_last_build_compiled_another_symbol(tmp_path, capsys):
    build = tmp_path / 'build'
    build.mkdir()
    (build / 'symbols.tsv').write_text('address\tsymbol\tfile\n'
                                       '0x00401000\t?Fire@tTestShip@@QAEHXZ\ta.cpp\n')
    rc, out = check(tmp_path, capsys, GOOD, build=build)
    assert rc == 0
    assert f'WARN 0x00401000: the last build compiled ?Fire@tTestShip@@QAEHXZ, the table has {FIRE}' in out


def test_add_fills_an_empty_symbol_and_changes_only_that_line(tmp_path, capsys):
    table, _ = setup(tmp_path)
    rc, out = run(tmp_path, capsys, 'add', '0x00401010', '?Hold@tTestShip@@QAEXXZ',
                  '--declaration', 'void Hold(void)')
    assert (rc, out) == (0, '0x00401010: ?Hold@tTestShip@@QAEXXZ\n')
    want = GOOD.replace(ROW_PLAIN, '0x00401010\tfunction\tfn_00401010\t\t?Hold@tTestShip@@QAEXXZ'
                                   '\tvoid Hold(void)\tnone')
    assert table.read_bytes() == want.encode()


def test_add_of_the_same_symbol_is_a_no_op_and_a_different_one_is_refused(tmp_path, capsys):
    table, _ = setup(tmp_path)
    rc, out = run(tmp_path, capsys, 'add', '0x00401000', FIRE)
    assert (rc, out) == (0, '0x00401000: unchanged\n')
    rc, out = run(tmp_path, capsys, 'add', '0x00401000', '?Fire@tTestShip@@QAEHXZ')
    assert rc == 1
    assert f'already has symbol {FIRE}; first writer wins' in out
    assert table.read_bytes() == GOOD.encode()


def test_add_refuses_a_symbol_that_is_at_another_address(tmp_path, capsys):
    table, _ = setup(tmp_path)
    rc, out = run(tmp_path, capsys, 'add', '0x00401010', FIRE)
    assert rc == 1
    assert f'symbol {FIRE} is already at 0x00401000' in out
    assert table.read_bytes() == GOOD.encode()


def test_add_refuses_an_implausible_symbol_a_per_unit_one_and_an_unknown_address(tmp_path, capsys):
    table, _ = setup(tmp_path)
    for address, symbol in (('0x00401010', 'tTestShip::Hold'), ('0x00401010', '??_C@_05abc@hello@'),
                            ('0x00409999', '?Hold@tTestShip@@QAEXXZ')):
        rc, out = run(tmp_path, capsys, 'add', address, symbol)
        assert rc == 1 and out.startswith('ERROR')
    assert table.read_bytes() == GOOD.encode()


def test_show_prints_the_row_and_its_folded_rows(tmp_path, capsys):
    setup(tmp_path)
    rc, out = run(tmp_path, capsys, 'show', '401000', 'fn_00401010')
    assert rc == 0
    assert out == (HEAD
                   + '0x00401000\tfunction\ttTestShip::Fire\ttTestShip\t' + FIRE + '\t\trtti\n'
                   + '0x00401000\tfolded\ttTestShip::Fire2\ttTestShip\t?Fire2@tTestShip@@QAEXXZ\t\tsdk\n'
                   + ROW_PLAIN + '\n')


def test_show_of_an_address_without_a_row_exits_1(tmp_path, capsys):
    setup(tmp_path)
    rc, _ = run(tmp_path, capsys, 'show', '0x00409999')
    assert rc == 1


def test_format_rows_sorts_by_address_and_puts_folded_after_the_primary():
    rows = [dict(zip(sym.COLUMNS, line.split('\t'))) for line in GOOD.split('\n')[1:-1]]
    assert sym.format_rows(reversed(rows)) == GOOD
