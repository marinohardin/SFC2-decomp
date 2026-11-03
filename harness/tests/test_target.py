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
