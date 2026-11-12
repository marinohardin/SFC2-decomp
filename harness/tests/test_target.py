"""The target object: original bytes, relocations masked only when the exe agrees."""
import struct

from conftest import FakeIndex, make_pe
from harness.lib import coff, target

FN = 0x401000          # function address in the fake exe
CALLEE = 0x401200      # where the exe's call goes
GLOBAL = 0x00500010    # the global the exe reads

# Rebuilt function: call ?callee@@YAXXZ ; mov eax, [?g@@3HA] ; ret
# e8 <rel32> ; a1 <dir32> ; c3   (relocation fields hold the in-place addends, all zero)
BASE_CODE = bytes.fromhex('e8 00000000 a1 00000000 c3'.replace(' ', ''))
RELOCS = [(1, '?callee@@YAXXZ', coff.REL_REL32), (6, '?g@@3HA', coff.REL_DIR32)]


def exe_code(callee=CALLEE, glob=GLOBAL, tail=b''):
    rel = (callee - (FN + 5)) & 0xFFFFFFFF
    return b'\xe8' + struct.pack('<I', rel) + b'\xa1' + struct.pack('<I', glob) + b'\xc3' + tail


def build(code, known, size=11, starts=()):
    obj = coff.CoffObject(coff.write_object(BASE_CODE, '?f@@YAXXZ', RELOCS))
    exe = coff.PeImage(make_pe(code + b'\xcc' * 32))
    return target.build(FN, size, obj, '?f@@YAXXZ', exe, FakeIndex(known, starts))


def statuses(t):
    return [(r.symbol, r.status) for r in t['relocs']]


def test_matching_function_with_known_callee_is_strictly_equal():
    t = build(exe_code(), {'?callee@@YAXXZ': [CALLEE]})
    assert statuses(t) == [('?callee@@YAXXZ', 'ok'), ('?g@@3HA', 'unverified')]
    assert t['strict'] is True
    assert t['target'] == BASE_CODE


def test_wrong_callee_is_a_mismatch_even_though_the_opcodes_agree():
    t = build(exe_code(callee=0x401300), {'?callee@@YAXXZ': [CALLEE]})
    assert statuses(t)[0] == ('?callee@@YAXXZ', 'mismatch')
    assert t['strict'] is False
    tobj = coff.CoffObject(t['object'])
    names = [tobj.symbols[i].name for _, i, _ in tobj.sections[1].relocs]
    assert names == ['$exe_00401300', '?g@@3HA']


def test_calling_an_address_known_under_another_name_is_a_mismatch():
    t = build(exe_code(), {'?other@@YAXXZ': [CALLEE]})
    assert statuses(t)[0] == ('?callee@@YAXXZ', 'mismatch')
    assert 'known as ?other@@YAXXZ' in t['relocs'][0].detail


def test_trailing_padding_is_not_compared():
    t = build(exe_code(), {}, size=16)
    assert len(t['target']) == 11
    assert t['strict'] is True


def test_different_instruction_bytes_are_not_strict():
    code = bytearray(exe_code())
    code[10] = 0xC2  # ret imm16 instead of ret
    t = build(bytes(code), {})
    assert t['first_diff'] == 10
    assert t['strict'] is False


def test_rebuilt_longer_than_analysis_size_is_extended_up_to_the_next_function():
    t = build(exe_code(), {}, size=6, starts=[FN, FN + 8])
    assert len(t['target']) == 8  # stops at the next function start, so the rebuild is longer
    assert t['strict'] is False


def test_same_unknown_symbol_must_resolve_to_one_address():
    base = bytes.fromhex('a100000000a100000000c3')
    relocs = [(1, '?g@@3HA', coff.REL_DIR32), (6, '?g@@3HA', coff.REL_DIR32)]
    obj = coff.CoffObject(coff.write_object(base, '?f@@YAXXZ', relocs))
    code = b'\xa1' + struct.pack('<I', GLOBAL) + b'\xa1' + struct.pack('<I', GLOBAL + 4) + b'\xc3'
    exe = coff.PeImage(make_pe(code))
    t = target.build(FN, 11, obj, '?f@@YAXXZ', exe, FakeIndex())
    assert [r.status for r in t['relocs']] == ['unverified', 'mismatch']


# fld dword ptr [__real@...] ; push offset ??_C@... ; ret
# d9 05 <dir32> ; 68 <dir32> ; c3
CONST_CODE = bytes.fromhex('d90500000000' '6800000000' 'c3')
REAL = '__real@4@3ffccccccd0000000000'      # 0.2f
STRING = '??_C@_08IBMI@A3_FLEET?$AA@'
REAL_AT, STRING_AT = 0x401100, 0x401108     # where the fake exe keeps them


def build_with_constants(real_bytes, string_bytes, exe_real, exe_string):
    relocs = [(2, REAL, coff.REL_DIR32), (7, STRING, coff.REL_DIR32)]
    obj = coff.CoffObject(coff.write_object(CONST_CODE, '?f@@YAXXZ', relocs,
                                            data=[(REAL, real_bytes), (STRING, string_bytes)]))
    code = (bytes.fromhex('d905') + struct.pack('<I', REAL_AT) + b'\x68'
            + struct.pack('<I', STRING_AT) + b'\xc3')
    text = bytearray(code + b'\xcc' * 0x200)
    text[REAL_AT - FN:REAL_AT - FN + len(exe_real)] = exe_real
    text[STRING_AT - FN:STRING_AT - FN + len(exe_string)] = exe_string
    exe = coff.PeImage(make_pe(bytes(text)))
    return target.build(FN, 12, obj, '?f@@YAXXZ', exe, FakeIndex())


POINT_TWO = bytes.fromhex('cdcc4c3e')       # 0.2f
POINT_ONE = bytes.fromhex('cdcccc3d')       # 0.1f


def test_constants_holding_the_exes_values_stay_exact():
    t = build_with_constants(POINT_TWO, b'A3_FLEET\0', POINT_TWO, b'A3_FLEET\0')
    assert statuses(t) == [(REAL, 'unverified'), (STRING, 'unverified')]
    assert t['strict'] is True


def test_a_constant_with_another_value_is_a_mismatch():
    t = build_with_constants(POINT_ONE, b'A3_FLEET\0', POINT_TWO, b'A3_FLEET\0')
    assert statuses(t) == [(REAL, 'mismatch'), (STRING, 'unverified')]
    assert t['relocs'][0].detail == 'the exe holds cdcc4c3e there, the rebuilt constant is cdcccc3d'
    assert t['strict'] is False


def test_a_stand_in_string_is_a_mismatch():
    t = build_with_constants(POINT_TWO, b'a\0', POINT_TWO, b'A3_FLEET\0')
    assert statuses(t) == [(REAL, 'unverified'), (STRING, 'mismatch')]
    assert t['relocs'][1].detail == 'the exe holds 4133 there, the rebuilt constant is 6100'
    assert t['strict'] is False


def test_a_constant_the_object_does_not_define_is_not_compared():
    relocs = [(2, REAL, coff.REL_DIR32), (7, STRING, coff.REL_DIR32)]
    obj = coff.CoffObject(coff.write_object(CONST_CODE, '?f@@YAXXZ', relocs))
    code = (bytes.fromhex('d905') + struct.pack('<I', REAL_AT) + b'\x68'
            + struct.pack('<I', STRING_AT) + b'\xc3')
    exe = coff.PeImage(make_pe(code + b'\xcc' * 0x200))
    t = target.build(FN, 12, obj, '?f@@YAXXZ', exe, FakeIndex())
    assert statuses(t) == [(REAL, 'unverified'), (STRING, 'unverified')]


def test_a_constant_address_outside_the_exe_is_not_compared():
    # Misaligned code reads bytes that are no address in the exe; that says nothing of the value.
    relocs = [(2, REAL, coff.REL_DIR32)]
    obj = coff.CoffObject(coff.write_object(CONST_CODE, '?f@@YAXXZ', relocs,
                                            data=[(REAL, POINT_TWO)]))
    code = bytes.fromhex('d905') + struct.pack('<I', 0xdf007968) + b'\x68' + b'\0' * 4 + b'\xc3'
    exe = coff.PeImage(make_pe(code + b'\xcc' * 0x200))
    t = target.build(FN, 12, obj, '?f@@YAXXZ', exe, FakeIndex())
    assert [r.status for r in t['relocs']][0] != 'mismatch'


def test_a_source_file_name_near_the_end_of_the_section_is_accepted():
    path = b'D:\\build\\Tiger\\tsay.cpp\0'
    relocs = [(2, REAL, coff.REL_DIR32), (7, STRING, coff.REL_DIR32)]
    obj = coff.CoffObject(coff.write_object(CONST_CODE, '?f@@YAXXZ', relocs,
                                            data=[(REAL, POINT_TWO), (STRING, b'tsay.cpp\0')]))
    code = (bytes.fromhex('d905') + struct.pack('<I', REAL_AT) + b'\x68'
            + struct.pack('<I', STRING_AT) + b'\xc3')
    text = bytearray(code + b'\xcc' * (STRING_AT - FN - len(code)) + path + b'\xcc' * 3)
    text[REAL_AT - FN:REAL_AT - FN + 4] = POINT_TWO
    exe = coff.PeImage(make_pe(bytes(text)))
    t = target.build(FN, 12, obj, '?f@@YAXXZ', exe, FakeIndex())
    assert statuses(t) == [(REAL, 'unverified'), (STRING, 'unverified')]


def test_a_source_file_name_under_the_original_build_folder_is_accepted():
    t = build_with_constants(POINT_TWO, b'tsay.cpp\0', POINT_TWO, b'D:\\build\\Tiger\\tsay.cpp\0')
    assert statuses(t) == [(REAL, 'unverified'), (STRING, 'unverified')]
    assert t['relocs'][1].detail == 'the source file name; the exe holds the original build path'
    assert t['strict'] is True


def test_a_different_file_name_is_still_a_mismatch():
    t = build_with_constants(POINT_TWO, b'tsay.cpp\0', POINT_TWO, b'D:\\build\\Tiger\\tpart.cpp\0')
    assert statuses(t)[1] == (STRING, 'mismatch')
