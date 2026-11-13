"""ghidra/scripts/name_game_copies.py: naming the game's own copies of library functions.

The rows and names are invented; expected rows are written out by hand.
"""
import csv
import importlib.util
import pathlib

import pytest

ROOT = pathlib.Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('name_game_copies',
                                              ROOT / 'ghidra/scripts/name_game_copies.py')
name_game_copies = importlib.util.module_from_spec(spec)
spec.loader.exec_module(name_game_copies)

FIELDS = ['address', 'size', 'name', 'mangled', 'demangled', 'name_source', 'translation_unit',
          'tu_evidence', 'headers', 'is_library', 'library', 'kind', 'tags']


def row(address, name, mangled='', source='default'):
    r = dict.fromkeys(FIELDS, '')
    r.update(address=address, size='16', name=name, mangled=mangled, name_source=source,
             is_library='0', kind='function')
    return r


def table(tmp_path, text):
    p = tmp_path / 'copies.tsv'
    p.write_text('address\tmangled\tname\tevidence\n' + text)
    return name_game_copies.load_table(str(p))


def test_an_unnamed_copy_takes_the_listed_name(tmp_path):
    rows = [row('00401000', 'FUN_00401000'), row('00402000', 'FUN_00402000')]
    t = table(tmp_path, '0x00401000\t?Go@tThing@@QAEXXZ\ttThing::Go\tcalled as Go by 0x00403000\n')
    assert name_game_copies.apply(rows, t) == ([0x00401000], [])
    assert (rows[0]['name'], rows[0]['mangled'], rows[0]['name_source']) == \
        ('tThing::Go', '?Go@tThing@@QAEXXZ', 'game-copy')
    assert rows[1]['name'] == 'FUN_00402000'
    assert name_game_copies.apply(rows, t) == ([], [])     # a second run changes nothing


def test_a_row_the_analysis_already_names_is_left_alone(tmp_path):
    rows = [row('00401000', 'tOther::Run', '?Run@tOther@@QAEXXZ', 'sdklib-exact')]
    t = table(tmp_path, '0x00401000\t?Go@tThing@@QAEXXZ\ttThing::Go\tcalled as Go\n')
    assert name_game_copies.apply(rows, t) == ([], [0x00401000])
    assert rows[0]['mangled'] == '?Run@tOther@@QAEXXZ'


def test_a_row_without_evidence_or_with_an_unknown_address_is_refused(tmp_path):
    with pytest.raises(ValueError, match='evidence'):
        table(tmp_path, '0x00401000\t?Go@tThing@@QAEXXZ\ttThing::Go\t\n')
    t = table(tmp_path, '0x00409000\t?Go@tThing@@QAEXXZ\ttThing::Go\tcalled as Go\n')
    with pytest.raises(ValueError, match='not a function'):
        name_game_copies.apply([row('00401000', 'FUN_00401000')], t)
