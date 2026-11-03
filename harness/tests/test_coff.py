"""COFF writer and reader agree; the PE reader maps addresses to file bytes."""
from conftest import make_pe
from harness.lib import coff

LONG = '?mAddToAllocList@tDynamicAllocator@@QAEXABVtype_info@@PAVtAllocator@@@Z'


def test_written_object_reads_back_with_function_relocations_and_long_names():
    code = bytes.fromhex('e800000000c3')
    raw = coff.write_object(code, LONG, [(1, '??2@YAPAXI@Z', coff.REL_REL32)])
    obj = coff.CoffObject(raw)
    sec, off, size = obj.function(LONG)
    assert (sec.number, off, size) == (1, 0, 6)
    assert sec.data == code
    assert sec.characteristics == 0x60500020
    (r_off, idx, typ), = sec.relocs
    assert (r_off, obj.symbols[idx].name, typ) == (1, '??2@YAPAXI@Z', 0x14)
    assert obj.function_names() == [LONG]


def test_local_symbols_are_defined_in_the_function_section():
    raw = coff.write_object(b'\x90' * 8, '_f', [(4, '$L1', coff.REL_DIR32)], [('$L1', 6)])
    obj = coff.CoffObject(raw)
    sym = obj.by_name['$L1']
    assert (sym.section, sym.value) == (1, 6)


def test_function_ends_at_the_next_function_symbol_in_its_section():
    raw = coff.write_object(b'\xc3' * 4, '_f', [], [])
    obj = coff.CoffObject(raw)
    assert obj.function('_f')[2] == 4


def test_pe_reader_reads_by_virtual_address():
    pe = coff.PeImage(make_pe(bytes(range(16))))
    assert pe.image_base == 0x400000
    assert pe.read(0x401004, 4) == bytes([4, 5, 6, 7])


def test_non_i386_object_is_rejected():
    raw = bytearray(coff.write_object(b'\xc3', '_f', []))
    raw[0:2] = b'\x64\x86'
    try:
        coff.CoffObject(bytes(raw))
    except coff.CoffError as e:
        assert 'not an i386' in str(e)
    else:
        raise AssertionError('accepted an x64 object')
