"""Tests for tools/rtti_symbols.py on a hand-built image of invented classes (no retail bytes).

The image is one .data section at 0x401000. Every address and expected row below is written out by
hand from the layout in `build_image`.
"""
import hashlib
import importlib.util
import pathlib
import struct
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
spec = importlib.util.spec_from_file_location('rtti_tool', ROOT / 'tools/rtti_symbols.py')
tool = importlib.util.module_from_spec(spec)
spec.loader.exec_module(tool)

BASE = 0x400000
SEC = 0x401000
TYPE_INFO = 0x00404000          # the vftable pointer every descriptor starts with (not in the image)


def va(offset):
    return SEC + offset


def build_image():
    """Descriptors at 0x000..0x240 (0x40 apart), base class records from 0x300, class
    hierarchies at 0x440.., ??_R4 records at 0x500.., vftables (a pointer to their ??_R4, then a
    slot) from 0x600. tDerived has a vftable at offset 0 and one at offset 8 (base tBaseB), tTwin
    one at offset 0 and one at offset 8 where two bases (tX, tY) sit."""
    d = bytearray(0x700)

    def put(at, *words):
        struct.pack_into('<%di' % len(words), d, at, *words)

    def descriptor(at, name, vptr=TYPE_INFO):
        put(at, vptr, 0)
        d[at + 8:at + 8 + len(name) + 1] = name + b'\0'

    names = {0x000: b'.?AVtAlpha@@', 0x040: b'.?AVtDerived@@', 0x080: b'.?AVtBaseA@@',
             0x0C0: b'.?AVtBaseB@@', 0x100: b'.?AVtTwin@@', 0x140: b'.?AVtX@@', 0x180: b'.?AVtY@@',
             0x1C0: b'.H', 0x200: b'.?AW4eMode@nX@@'}
    for at, name in names.items():
        descriptor(at, name)
    descriptor(0x240, b'.?AVtNoise@@', vptr=0x00404ABC)     # another vptr: not a type descriptor
    # ??_R1: type descriptor, bases contained, mdisp, pdisp, vdisp, attributes, hierarchy
    r3 = {'alpha': va(0x440), 'der': va(0x460), 'twin': va(0x480)}
    put(0x300, va(0x000), 0, 0, -1, 0, 0, r3['alpha'])
    put(0x320, va(0x040), 2, 0, -1, 0, 0, r3['der'])
    put(0x340, va(0x080), 0, 0, -1, 0, 0, r3['der'])
    put(0x360, va(0x0C0), 0, 8, -1, 0, 0, r3['der'])
    put(0x380, va(0x100), 2, 0, -1, 0, 0, r3['twin'])
    put(0x3A0, va(0x140), 0, 8, -1, 0, 0, r3['twin'])
    put(0x3C0, va(0x180), 0, 8, -1, 0, 0, r3['twin'])
    put(0x400, va(0x300))                                    # ??_R2 of tAlpha
    put(0x410, va(0x320), va(0x340), va(0x360))              # tDerived
    put(0x420, va(0x380), va(0x3A0), va(0x3C0))              # tTwin
    put(0x440, 0, 0, 1, va(0x400))                           # ??_R3: signature, attributes, count, array
    put(0x460, 0, 0, 3, va(0x410))
    put(0x480, 0, 0, 3, va(0x420))
    # ??_R4: signature, offset, cdOffset, type descriptor, hierarchy
    put(0x500, 0, 0, 0, va(0x000), r3['alpha'])
    put(0x520, 0, 0, 0, va(0x040), r3['der'])
    put(0x540, 0, 8, 0, va(0x040), r3['der'])
    put(0x560, 0, 0, 0, va(0x100), r3['twin'])
    put(0x580, 0, 8, 0, va(0x100), r3['twin'])
    for at, r4 in ((0x600, 0x500), (0x610, 0x520), (0x620, 0x540), (0x630, 0x560), (0x640, 0x580)):
        put(at, va(r4), BASE + 0x1000)                       # the pointer to ??_R4, then slot 0
    raw = bytearray(0x200)
    raw[0:2] = b'MZ'
    struct.pack_into('<I', raw, 0x3C, 0x40)
    raw[0x40:0x44] = b'PE\0\0'
    struct.pack_into('<HHIIIHH', raw, 0x44, 0x14C, 1, 0, 0, 0, 0xE0, 0x102)
    struct.pack_into('<H', raw, 0x58, 0x10B)
    struct.pack_into('<I', raw, 0x58 + 28, BASE)
    struct.pack_into('<8sIIII', raw, 0x58 + 0xE0, b'.data', len(d), 0x1000, len(d), 0x200)
    return bytes(raw) + bytes(d)


EXPECTED = [
    (0x401000, 'type_descriptor', '??_R0?AVtAlpha@@@8'),
    (0x401040, 'type_descriptor', '??_R0?AVtDerived@@@8'),
    (0x401080, 'type_descriptor', '??_R0?AVtBaseA@@@8'),
    (0x4010C0, 'type_descriptor', '??_R0?AVtBaseB@@@8'),
    (0x401100, 'type_descriptor', '??_R0?AVtTwin@@@8'),
    (0x401140, 'type_descriptor', '??_R0?AVtX@@@8'),
    (0x401180, 'type_descriptor', '??_R0?AVtY@@@8'),
    (0x4011C0, 'type_descriptor', '??_R0H@8'),
    (0x401200, 'type_descriptor', '??_R0?AW4eMode@nX@@@8'),
    (0x401604, 'vftable', '??_7tAlpha@@6B@'),
    (0x401614, 'vftable', '??_7tDerived@@6B@'),
    (0x401624, 'vftable', '??_7tDerived@@6BtBaseB@@@'),
    (0x401634, 'vftable', '??_7tTwin@@6B@'),
]


def test_scan_finds_every_descriptor_and_the_unambiguous_vftables():
    rows, _ = tool.scan(build_image())
    assert rows == EXPECTED


def test_scan_counts_what_it_wrote_and_what_it_left_out():
    _, stats = tool.scan(build_image())
    assert dict(stats) == {'type_descriptors': 9, 'vftable_one_per_class': 1, 'vftable_offset_zero': 2,
                           'vftable_with_base': 1, 'left_out_base_not_unique': 1}


def test_a_vftable_whose_record_two_places_point_at_is_left_out():
    raw = bytearray(build_image())
    struct.pack_into('<I', raw, 0x200 + 0x680, va(0x500))    # a second word pointing at tAlpha's ??_R4
    rows, stats = tool.scan(bytes(raw))
    assert (0x401604, 'vftable', '??_7tAlpha@@6B@') not in rows
    assert stats['left_out_not_one_vftable_reference'] == 1


def test_a_nonzero_vftable_over_a_virtual_base_is_left_out():
    raw = bytearray(build_image())
    struct.pack_into('<i', raw, 0x200 + 0x36C, 4)            # tBaseB's base record gets a virtual-base displacement
    rows, _ = tool.scan(bytes(raw))
    assert 0x401624 not in [a for a, _, _ in rows]


def test_text_has_two_comment_lines_the_header_and_one_row_per_symbol():
    out = tool.text(EXPECTED[:1] + EXPECTED[-1:]).split('\n')
    assert out[0].startswith('# ') and out[1].startswith('# ')
    assert out[2:] == ['address\tkind\tsymbol\tevidence',
                       '0x00401000\ttype_descriptor\t??_R0?AVtAlpha@@@8\trtti',
                       '0x00401634\tvftable\t??_7tTwin@@6B@\trtti', '']


def committed(tmp_path, rows=EXPECTED):
    exe = tmp_path / 'game.exe'
    exe.write_bytes(build_image())
    file = tmp_path / 'data.tsv'
    file.write_text(tool.text(rows), encoding='utf-8', newline='')
    return exe, file


def test_check_passes_when_the_file_is_what_the_executable_gives(tmp_path, capsys):
    exe, file = committed(tmp_path)
    rc = tool.main(['check', '--exe', str(exe), '--file', str(file), '--no-hash'])
    assert rc == 0 and capsys.readouterr().out.startswith(f'ok: 13 rows in {file}')


def test_check_fails_when_the_file_differs(tmp_path, capsys):
    exe, file = committed(tmp_path, EXPECTED[:-1])
    rc = tool.main(['check', '--exe', str(exe), '--file', str(file), '--no-hash'])
    assert rc == 1 and 'differs from the executable at line 16: run python3 tools/rtti_symbols.py generate' \
        in capsys.readouterr().out


def test_check_skips_with_an_info_line_when_there_is_no_executable(tmp_path, capsys):
    rc = tool.main(['check', '--exe', str(tmp_path / 'none.exe'), '--file', str(tmp_path / 'f.tsv')])
    assert rc == 0 and capsys.readouterr().out.startswith('INFO no executable at ')


def test_generate_writes_the_file_check_accepts(tmp_path, capsys):
    exe, file = committed(tmp_path, [])
    assert tool.main(['generate', '--exe', str(exe), '--out', str(file), '--no-hash']) == 0
    assert file.read_text(encoding='utf-8') == tool.text(EXPECTED)
    assert tool.main(['check', '--exe', str(exe), '--file', str(file), '--no-hash']) == 0


def test_the_executable_is_held_to_the_retail_hash(tmp_path, capsys):
    exe, file = committed(tmp_path)
    hashes = tmp_path / 'hashes.sha256'
    hashes.write_text('0' * 64 + '  game/StarFleetOP.exe\n')
    assert tool.exe_hash_problem(str(exe), str(hashes), 'game/StarFleetOP.exe') \
        .endswith('is not the retail file (sha256 ' + hashlib.sha256(exe.read_bytes()).hexdigest() + ')')
    hashes.write_text(hashlib.sha256(exe.read_bytes()).hexdigest() + '  game/StarFleetOP.exe\n')
    assert tool.exe_hash_problem(str(exe), str(hashes), 'game/StarFleetOP.exe') is None
    assert 'not in hashes.sha256' in tool.exe_hash_problem(str(exe), str(hashes), 'other.exe')


def test_the_committed_file_is_well_formed_and_agrees_with_the_vtable_export():
    lines = (ROOT / 'analysis/symbols/STARFLEETOP.data.tsv').read_text(encoding='utf-8').split('\n')
    assert lines[-1] == '' and lines[0].startswith('# ') and lines[1].startswith('# ')
    assert lines[2] == 'address\tkind\tsymbol\tevidence'
    rows = [l.split('\t') for l in lines[3:-1]]
    assert all(len(r) == 4 and r[3] == 'rtti' for r in rows)
    addresses = [int(r[0], 16) for r in rows]
    assert addresses == sorted(set(addresses))
    symbols = [r[2] for r in rows]
    assert len(symbols) == len(set(symbols))
    assert all(r[2].startswith('??_R0') and r[2].endswith('@8') for r in rows if r[1] == 'type_descriptor')
    assert all(r[2].startswith('??_7') and '6B' in r[2] for r in rows if r[1] == 'vftable')
    export = {l.split(',')[0] for l in (ROOT / 'ghidra/exports/vtables.csv').read_text().split('\n')[1:] if l}
    assert all(r[0][2:] in export for r in rows if r[1] == 'vftable')
