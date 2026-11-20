"""The loop keeps the symbol table current by tool (harness/lib/symsync.py): the claims of the
rebuilt units are synced, and an unclaimed callee with no symbol gets the one its relocation
names, when the seed's rule accepts it."""
import json

from harness.lib import paths, symsync, symtab

HEAD = 'address\tkind\tname\tclass\tsymbol\tsymbol_evidence\tdeclaration\tevidence\n'


def row(address, name, symbol='', ev='', evidence='none', kind='function'):
    return '\t'.join((address, kind, name, '', symbol, ev, '', evidence))


def reloc(addr, symbol, status='ok', typ='REL32'):
    return dict(type=typ, symbol=symbol, exe_address=addr, status=status, offset=1)


def tree(tmp_path, rows, relocs):
    src = tmp_path / 'op'
    src.mkdir()
    (src / 'a.cpp').write_text('// FUNCTION: STARFLEETOP 0x00401000\nvoid f() {}\n')
    table = tmp_path / 'table.tsv'
    table.write_text(HEAD + '\n'.join(rows) + '\n')
    out = tmp_path / 'after.json'
    out.write_text('noise before the JSON\n' + json.dumps(dict(results=[
        dict(address='0x00401000', unit='a.cpp', symbol='?new@@YAXXZ', relocs=relocs)])))
    return str(out), str(table), str(src)


def rows_of(table):
    return {c[0]: c for c in (x.split('\t') for x in open(table).read().split('\n')[1:] if x)}


def test_the_claim_is_synced_and_an_acceptable_callee_symbol_is_added(tmp_path):
    out, table, src = tree(tmp_path, [
        row('0x00401000', 'f', '?old@@YAXXZ', 'claim', 'claim'),
        row('0x00401200', 'fn_00401200')],
        [reloc('0x00401200', '?fn_00401200@@YAXXZ')])
    lines = symsync.sync(out, '0x00401000', table, src)
    rows = rows_of(table)
    assert rows['0x00401000'][4:6] == ['?new@@YAXXZ', 'claim']
    assert rows['0x00401200'][4:6] == ['?fn_00401200@@YAXXZ', 'add']
    assert not [x for x in lines if x.startswith('WARN')]
    assert any('0x00401200 gets ?fn_00401200@@YAXXZ (add)' in x for x in lines)


def test_a_symbol_that_fails_the_rule_is_not_written_and_warns_once(tmp_path):
    out, table, src = tree(tmp_path, [
        row('0x00401000', 'f', '?old@@YAXXZ', 'claim', 'claim'),
        row('0x00401300', 'fn_00401300'),
        row('0x00401400', 'fn_00401400')],
        [reloc('0x00401300', '?fn_00409999@@YAXXZ'),          # another address's placeholder
         reloc('0x00401400', '?mGo@c_00401400_Object@@QAEXXZ')])   # a placeholder class
    lines = symsync.sync(out, '0x00401000', table, src)
    rows = rows_of(table)
    assert rows['0x00401300'][4:6] == ['', ''] and rows['0x00401400'][4:6] == ['', '']
    warns = [x for x in lines if x.startswith('WARN')]
    assert len(warns) == 2
    assert 'fn-name-of-another-address' in warns[0]


def test_a_row_with_another_mangled_or_add_symbol_is_never_replaced(tmp_path):
    out, table, src = tree(tmp_path, [
        row('0x00401000', 'f', '?old@@YAXXZ', 'claim', 'claim'),
        row('0x00401500', 'g', '?g@@YAXXZ', 'mangled', 'mangled'),
        row('0x00401600', 'h', '?h@@YAXXZ', 'callers')],
        [reloc('0x00401500', '?other@@YAXXZ'), reloc('0x00401600', '?other2@@YAXXZ')])
    lines = symsync.sync(out, '0x00401000', table, src)
    rows = rows_of(table)
    assert rows['0x00401500'][4:6] == ['?g@@YAXXZ', 'mangled']
    assert rows['0x00401600'][4:6] == ['?h@@YAXXZ', 'callers']
    assert len([x for x in lines if x.startswith('WARN')]) == 1


def test_a_claimed_target_a_mismatch_and_a_per_unit_symbol_are_left_alone(tmp_path):
    out, table, src = tree(tmp_path, [
        row('0x00401000', 'f', '?old@@YAXXZ', 'claim', 'claim'),
        row('0x00401700', 'fn_00401700'), row('0x00401800', 'fn_00401800'),
        row('0x00401900', 'fn_00401900')],
        [reloc('0x00401700', '?fn_00401700@@YAXXZ', status='mismatch'),
         reloc('0x00401800', '$L123'), reloc('0x00401000', '?self@@YAXXZ'),
         reloc('0x00401900', '?fn_00401900@@YAXXZ', typ='SECREL')])
    symsync.sync(out, '0x00401000', table, src)
    rows = rows_of(table)
    assert [rows[a][4] for a in ('0x00401700', '0x00401800', '0x00401900')] == ['', '', '']
    assert rows['0x00401000'][4] == '?new@@YAXXZ'


def test_a_build_output_without_the_function_changes_nothing(tmp_path):
    out, table, src = tree(tmp_path, [row('0x00401000', 'f', '?old@@YAXXZ', 'claim', 'claim')], [])
    before = open(table).read()
    lines = symsync.sync(out, '0x00409999', table, src)
    assert open(table).read() == before and lines[0].startswith('WARN')


def test_scratch_check_syncs_a_copy_and_leaves_the_committed_table_alone(tmp_path, monkeypatch):
    table = tmp_path / 'table.tsv'
    table.write_text(HEAD + row('0x00401000', 'f', '?old@@YAXXZ', 'claim', 'claim') + '\n')
    monkeypatch.setattr(paths, 'SYMBOL_TABLE', str(table))
    monkeypatch.setattr(paths, 'STATE', str(tmp_path / 'state'))
    monkeypatch.setattr(paths, 'BUILD', str(tmp_path / 'state' / 'build'))
    monkeypatch.setattr(paths, 'SRC_ROOT', str(tmp_path / 'op'))
    (tmp_path / 'f.csv').write_text('address,kind\n00401000,function\n')
    monkeypatch.setattr(paths, 'FUNCTIONS_CSV', str(tmp_path / 'f.csv'))
    (tmp_path / 'op').mkdir()      # nothing is claimed: the old claim row is reset on the copy
    before = table.read_text()
    build = tmp_path / 'state' / 'build'
    build.mkdir(parents=True)
    symtab.write_claims(str(build / 'symbols.tsv'), [])
    assert symsync.scratch_check() == 0
    assert table.read_text() == before
    copy = (tmp_path / 'state' / 'wholetree-symbols.tsv').read_text()
    assert copy.split('\n')[1].split('\t')[4:6] == ['', '']
