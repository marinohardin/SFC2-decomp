"""The committed symbol table as the harness symbol table's evidence (harness/lib/symtab.py)."""
import pytest

from conftest import FakeIndex
from harness.lib import pack, symtab

HEAD = 'address\tkind\tname\tclass\tsymbol\tsymbol_evidence\tdeclaration\tevidence\n'


def table_file(tmp_path, *rows):
    f = tmp_path / 'table.tsv'
    f.write_text(HEAD + ''.join('\t'.join(r) + '\n' for r in rows))
    return str(f)


def test_the_committed_table_is_read_for_rows_with_a_symbol(tmp_path):
    f = table_file(tmp_path,
                   ('0x00401000', 'function', 'a', '', '?a@@YAXXZ', 'claim', '', 'claim'),
                   ('0x00401100', 'function', 'fn_00401100', '', '', '', '', 'none'),
                   ('0x00401200', 'function', 'c', '', '?c@@YAXXZ', 'callers', '', 'none'),
                   ('0x00401200', 'folded', 'c2', '', '?c2@@YAXXZ', 'add', '', 'none'))
    assert symtab.load_table(f) == {'0x00401000': ('?a@@YAXXZ', 'claim'),
                                    '0x00401200': ('?c@@YAXXZ', 'callers')}
    assert symtab.load_table(str(tmp_path / 'missing.tsv')) == {}


def test_tier_order_is_build_claim_then_table_then_functions_csv_then_sdk():
    index = FakeIndex(known={'?csv@@YAXXZ': [0x401300], '?sdk@@YAXXZ': [0x401400]})
    fn = type('F', (), {'mangled': '?csv@@YAXXZ'})()
    index.by_address = {0x401300: fn}
    claims = {'0x00401000': ('?built@@YAXXZ', 'b.cpp')}
    table = {'0x00401000': ('?table@@YAXXZ', 'claim'), '0x00401300': ('?tab@@YAXXZ', 'add'),
             '0x00401500': ('?only@@YAXXZ', 'mangled')}
    assert symtab.lookup(0x401000, index, claims, table) == ('?built@@YAXXZ', 'claim', 'b.cpp')
    assert symtab.lookup(0x401300, index, claims, table) == ('?tab@@YAXXZ', 'table', 'add')
    assert symtab.lookup(0x401300, index, claims) == ('?csv@@YAXXZ', 'functions.csv', '')
    assert symtab.lookup(0x401400, index, claims, table) == ('?sdk@@YAXXZ', 'sdk', '')
    assert symtab.lookup(0x401500, index, claims, table) == ('?only@@YAXXZ', 'table', 'mangled')


def test_the_pack_requires_a_claim_mangled_or_add_symbol_and_leads_with_the_others():
    assert 'must name `?a@@YAXXZ`' in pack.callee_symbol(('?a@@YAXXZ', 'table', 'add'))
    assert 'must name' in pack.callee_symbol(('?a@@YAXXZ', 'table', 'mangled'))
    assert 'must name' in pack.callee_symbol(('?a@@YAXXZ', 'table', 'claim'))
    assert pack.callee_symbol(('?a@@YAXXZ', 'table', 'callers')).startswith(': probably')


def test_columns_are_found_by_their_header_names_and_a_bad_table_is_an_error(tmp_path):
    f = tmp_path / 'wide.tsv'
    f.write_text('note\t' + HEAD + 'x\t0x00401000\tfunction\ta\t\t?a@@YAXXZ\tadd\t\tnone\n')
    assert symtab.load_table(str(f)) == {'0x00401000': ('?a@@YAXXZ', 'add')}
    f.write_text(HEAD + '0x00401000\tfunction\ta\n')
    with pytest.raises(symtab.TableError, match=':2: 3 cells'):
        symtab.load_table(str(f))
    f.write_text('address\tkind\n')
    with pytest.raises(symtab.TableError, match='lacks symbol, symbol_evidence'):
        symtab.load_table(str(f))
