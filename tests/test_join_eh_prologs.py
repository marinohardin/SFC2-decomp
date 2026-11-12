"""ghidra/scripts/join_eh_prologs.py: joining functions split after their exception prolog.

The bytes are made up here: a fake helper with __EH_prolog's instruction bytes and fake
prologs that call it. No retail file is read.
"""
import csv
import importlib.util
import io
import pathlib
import struct

ROOT = pathlib.Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('join_eh_prologs',
                                              ROOT / 'ghidra/scripts/join_eh_prologs.py')
join_eh_prologs = importlib.util.module_from_spec(spec)
spec.loader.exec_module(join_eh_prologs)

FIELDS = ['address', 'size', 'name', 'mangled', 'demangled', 'name_source', 'translation_unit',
          'tu_evidence', 'headers', 'is_library', 'library', 'kind', 'tags']
HELPER = 0x00402000
EH_PROLOG = bytes.fromhex('6aff5064a100000000508b44240c64892500000000896c240c8d6c240c50c3')


def prolog(at, target=HELPER):
    return b'\xb8\x00\x10\x70\x00\xe8' + struct.pack('<i', target - (at + 10))


def memory(*pieces):
    mem = dict(pieces)

    def read(va, n):
        for start, data in mem.items():
            if start <= va and va + n <= start + len(data):
                return data[va - start:va - start + n]
        return b''
    return read


def row(address, size, name='', source='default', kind='function', **extra):
    r = dict.fromkeys(FIELDS, '')
    r.update(address=f'{address:08x}', size=str(size), name=name or f'FUN_{address:08x}',
             name_source=source, kind=kind, is_library='0')
    r.update(extra)
    return r


def test_a_prolog_and_its_body_become_one_function_at_the_entry():
    read = memory((0x401000, prolog(0x401000)), (HELPER, EH_PROLOG))
    rows = [row(0x401000, 10, 'A::~A', 'rtti-class-recovery'),
            row(0x40100a, 40, 'B::B', 'rtti-class-recovery', translation_unit='a.cpp'),
            row(0x401032, 8)]
    out, moved = join_eh_prologs.join(rows, read)
    assert moved == {0x40100a: 0x401000}
    assert [(r['address'], r['size'], r['name']) for r in out] == [
        ('00401000', '50', 'A::~A'), ('00401032', '8', 'FUN_00401032')]
    assert out[0]['translation_unit'] == 'a.cpp'
    assert out[0]['tags'] == 'eh-prolog-joined'


def test_the_body_name_is_kept_when_the_entry_has_none_from_class_recovery():
    read = memory((0x401000, prolog(0x401000)), (HELPER, EH_PROLOG))
    out, _ = join_eh_prologs.join([row(0x401000, 10), row(0x40100a, 40, 'B::f', 'ghidra-fid-vs',
                                                          is_library='1', library='stl')], read)
    assert (out[0]['name'], out[0]['name_source'], out[0]['is_library'], out[0]['library']) == \
        ('B::f', 'ghidra-fid-vs', '1', 'stl')


def test_rows_that_only_look_like_a_split_are_left_alone():
    other = 0x00403000
    read = memory((0x401000, prolog(0x401000, other)), (other, b'\xc3' * 40),
                  (0x401100, prolog(0x401100)), (0x401200, prolog(0x401200)),
                  (HELPER, EH_PROLOG))
    rows = [row(0x401000, 10), row(0x40100a, 20),                  # calls something else
            row(0x401100, 10), row(0x40110a, 20, kind='eh-funclet'),  # next row is no function
            row(0x401200, 12), row(0x40120c, 20)]                  # not 10 bytes
    out, moved = join_eh_prologs.join(rows, read)
    assert moved == {} and out == rows


def test_a_body_that_looks_like_a_prolog_is_not_also_joined_to_what_follows():
    # Two prologs in a row: the first takes the second as its body, and the row after the second
    # is kept, so no bytes leave functions.csv.
    read = memory((0x401000, prolog(0x401000) + prolog(0x40100a)), (HELPER, EH_PROLOG))
    rows = [row(0x401000, 10), row(0x40100a, 10), row(0x401014, 30)]
    out, moved = join_eh_prologs.join(rows, read)
    assert moved == {0x40100a: 0x401000}
    assert [(r['address'], r['size']) for r in out] == [('00401000', '20'), ('00401014', '30')]


def test_joining_twice_changes_nothing_more():
    read = memory((0x401000, prolog(0x401000)), (HELPER, EH_PROLOG))
    once, _ = join_eh_prologs.join([row(0x401000, 10), row(0x40100a, 40)], read)
    twice, moved = join_eh_prologs.join([dict(r) for r in once], read)
    assert moved == {} and twice == once


def test_the_csvs_that_name_a_body_follow_the_joined_function(tmp_path, monkeypatch):
    def write(name, fields, rows):
        with open(tmp_path / name, 'w', newline='') as f:
            w = csv.DictWriter(f, fieldnames=fields, lineterminator='\n')
            w.writeheader()
            w.writerows(rows)
    write('functions.csv', FIELDS, [row(0x401000, 10, 'A::f', 'rtti-class-recovery'),
                                    row(0x40100a, 40), row(0x401040, 16)])
    write('strings_to_functions.csv', ['string_address', 'function_address', 'function', 'via',
                                       'string'],
          [dict(string_address='00795000', function_address='0040100a',
                function='FUN_0040100a', via='direct', string='TestCaptain')])
    write('vtables.csv', ['vtable_address', 'class', 'vtable_name', 'slot', 'function_address',
                          'function'],
          [dict(vtable_address='007b0000', **{'class': 'A'}, vtable_name='vftable', slot='0',
                function_address='00401000', function='FUN_00401000')])
    write('unnamed_regions.csv', ['start', 'end', 'functions', 'bytes', 'named_before',
                                  'named_after'],
          [dict(start='0040100a', end='0040104f', functions='2', bytes='56',
                named_before='00401000 A::f', named_after='')])
    read = memory((0x401000, prolog(0x401000)), (HELPER, EH_PROLOG))
    monkeypatch.setattr(join_eh_prologs, 'pe_reader', lambda image: read)
    exe = tmp_path / 'fake.exe'
    exe.write_bytes(b'')
    join_eh_prologs.main(['join_eh_prologs.py', str(tmp_path), str(exe)])
    text = (tmp_path / 'strings_to_functions.csv').read_text()
    assert list(csv.DictReader(io.StringIO(text)))[0]['function_address'] == '00401000'
    assert list(csv.DictReader(io.StringIO(text)))[0]['function'] == 'A::f'
    funcs = list(csv.DictReader(open(tmp_path / 'functions.csv')))
    assert [(r['address'], r['size']) for r in funcs] == [('00401000', '50'), ('00401040', '16')]
    vt = list(csv.DictReader(open(tmp_path / 'vtables.csv')))
    assert vt[0]['function'] == 'A::f'
    # The joined body was the first of two unnamed functions; one is left, the region keeps its end.
    regions = list(csv.DictReader(open(tmp_path / 'unnamed_regions.csv')))
    assert [(r['start'], r['end'], r['functions'], r['bytes'], r['named_before']) for r in regions] \
        == [('00401040', '0040104f', '1', '16', '00401000 A::f')]
