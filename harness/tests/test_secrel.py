"""Thread-local variables: relocation type 0x0B (IMAGE_REL_I386_SECREL).

The field of such a relocation holds the variable's offset inside the thread-local block. objdiff
cannot load the type, so it is given copies in which the records read as DIR32, and the harness's
own strict check judges the field: the offset is turned into an address with the exe's TLS
directory and checked like any other relocation's address.
"""
import struct
from types import SimpleNamespace

from conftest import FakeIndex, make_pe
from harness.lib import coff, target, verify

FN = 0x401000
TLS_START = 0x00600000     # where the exe's thread-local data starts
TLS_END = 0x00600010       # raw data ends here
TLS_FILL = 0x10            # then this many zero-fill bytes
TLS_DIR_AT = 0x100         # the directory sits this far into the .text bytes

# Rebuilt function: mov dword ptr [edx + secrel], ecx ; ret   (an addend of 0 in the object)
BASE = bytes.fromhex('898a00000000c3')
RELOCS = [(2, '?tvar@@3HA', coff.REL_SECREL)]


def exe_code(offset):
    return bytes.fromhex('898a') + struct.pack('<I', offset) + b'\xc3'


def tls_pe(code):
    directory = struct.pack('<6I', TLS_START, TLS_END, 0x600100, 0x600104, TLS_FILL, 0)
    text = code.ljust(TLS_DIR_AT, b'\xcc') + directory
    return coff.PeImage(make_pe(text, tls_dir_rva=0x1000 + TLS_DIR_AT))


def build(offset, known=None, base=BASE, exe=None):
    obj = coff.CoffObject(coff.write_object(base, '?f@@YAXXZ', RELOCS))
    exe = exe or tls_pe(exe_code(offset))
    return target.build(FN, 7, obj, '?f@@YAXXZ', exe, FakeIndex(known))


def test_rewrite_changes_only_the_type_of_secrel_records():
    code = bytes.fromhex('898a00000000a100000000c3')
    raw = coff.write_object(code, '?f@@YAXXZ', [(2, '?tvar@@3HA', coff.REL_SECREL),
                                                 (7, '?g@@3HA', coff.REL_REL32)])
    out, n = coff.rewrite_reloc_types(raw)
    assert n == 1 and len(out) == len(raw)
    new, old = coff.CoffObject(out).sections[1], coff.CoffObject(raw).sections[1]
    assert [t for _, _, t in new.relocs] == [coff.REL_DIR32, coff.REL_REL32]
    assert [i for _, i, _ in new.relocs] == [i for _, i, _ in old.relocs]
    assert [o for o, _, _ in new.relocs] == [o for o, _, _ in old.relocs]
    assert new.data == old.data
    assert coff.rewrite_reloc_types(out)[1] == 0


def test_rewrite_refuses_a_truncated_relocation_table_or_an_overflow_flag():
    import pytest
    raw = coff.write_object(BASE, '?f@@YAXXZ', RELOCS)
    with pytest.raises(coff.CoffError):
        coff.rewrite_reloc_types(raw[:20 + 40 + len(BASE) + 4])
    flagged = bytearray(raw)
    struct.pack_into('<H', flagged, 20 + 32, 0xFFFF)
    struct.pack_into('<I', flagged, 20 + 36, 0x01000000 | coff.TEXT_CHARACTERISTICS)
    with pytest.raises(coff.CoffError):
        coff.rewrite_reloc_types(bytes(flagged))


def test_pe_reads_the_thread_local_block_range():
    exe = tls_pe(BASE)
    assert (exe.tls_start, exe.tls_end) == (TLS_START, TLS_END + TLS_FILL)
    exe = coff.PeImage(make_pe(BASE))
    assert (exe.tls_start, exe.tls_end) == (None, None)


def test_offset_resolves_to_an_address_in_the_tls_block():
    t = build(0x8)
    assert [(r.symbol, r.status, r.exe_address) for r in t['relocs']] == \
        [('?tvar@@3HA', 'unverified', TLS_START + 8)]
    assert t['relocs'][0].as_dict()['type'] == 'SECREL'
    assert t['strict'] is True


def test_a_known_variable_is_checked_against_its_address():
    assert build(0x8, known={'?tvar@@3HA': [TLS_START + 8]})['relocs'][0].status == 'ok'
    t = build(0xc, known={'?tvar@@3HA': [TLS_START + 8]})
    assert t['relocs'][0].status == 'mismatch'
    assert t['strict'] is False


def test_an_offset_outside_the_tls_block_is_a_mismatch():
    t = build(0x20)    # raw data 0x10 plus zero fill 0x10: 0x20 is the first byte past the block
    assert t['relocs'][0].status == 'mismatch'
    assert 'outside' in t['relocs'][0].detail
    assert t['strict'] is False
    assert build(0x1f)['relocs'][0].status == 'unverified'


def test_an_exe_without_a_tls_directory_is_a_mismatch():
    t = build(8, exe=coff.PeImage(make_pe(exe_code(8) + b'\xcc' * 32)))
    assert t['relocs'][0].status == 'mismatch'
    assert 'no TLS directory' in t['relocs'][0].detail
    assert t['strict'] is False


def test_the_objects_addend_is_subtracted_before_the_address_is_formed():
    t = build(0xc, base=bytes.fromhex('898a04000000c3'))   # the object holds 4, the exe 0xc
    assert t['relocs'][0].exe_address == TLS_START + 8


def test_other_bytes_must_still_match():
    t = build(0x8, base=bytes.fromhex('898b00000000c3'))
    assert t['strict'] is False and t['first_diff'] == 1


def test_the_target_object_for_objdiff_carries_dir32_while_the_result_keeps_secrel():
    t = build(0x8)
    assert t['rewritten'] == 1
    tobj = coff.CoffObject(t['object'])
    assert [typ for _, _, typ in tobj.sections[1].relocs] == [coff.REL_DIR32]
    assert [r.type for r in t['relocs']] == [coff.REL_SECREL]


# --- score_function with a faked objdiff ------------------------------------------------------

def _score(tmp_path, monkeypatch, relocs, base_code):
    monkeypatch.setenv('HARNESS_NO_CACHE', '1')
    seen = {}

    def fake_objdiff(t, b, sym, out_dir):
        seen['base'] = b
        seen['base_types'] = [typ for _, _, typ in coff.CoffObject(open(b, 'rb').read())
                              .sections[1].relocs]
        ins = [{'diff_kind': 'DIFF_NONE', 'instruction': {'address': 0, 'formatted': 'ret'}}]
        return {'match_percent': 100.0, 'instructions': ins}, {'instructions': ins}, ''
    monkeypatch.setattr(verify, 'objdiff', fake_objdiff)
    obj_path = tmp_path / 'u.obj'
    obj_path.write_bytes(coff.write_object(base_code, '_f', relocs))
    b = dict(obj=str(obj_path), listing=None, out_dir=str(tmp_path))
    fn = SimpleNamespace(address=FN, name='f', size=len(base_code), mangled='_f')
    unit = dict(rel='u.cpp', path='u.cpp', file='u.cpp', line=1, other_lines=[])
    exe = tls_pe(exe_code(8) if relocs else base_code)
    res = verify.score_function(fn, unit, b, exe, FakeIndex(), symbol_hint='_f')
    return res, seen, obj_path


def test_objdiff_receives_the_rewritten_object_when_the_object_has_secrel(tmp_path, monkeypatch):
    res, seen, obj_path = _score(tmp_path, monkeypatch, RELOCS, BASE)
    assert seen['base'] == str(tmp_path / '0x00401000' / 'base.obj')
    assert seen['base_types'] == [coff.REL_DIR32]
    assert coff.CoffObject(obj_path.read_bytes()).sections[1].relocs[0][2] == coff.REL_SECREL
    assert res['exact'] is True
    assert res['objdiff_rewrites'] == 2     # one record in the target object, one in the base
    assert res['relocs'][0]['type'] == 'SECREL'
    rep = verify.Report()
    verify._report_function(rep, dict(functions={}), SimpleNamespace(address=FN, name='f'),
                            dict(res, claimed=True))
    assert any(ln.startswith('INFO') and 'thread-local (SECREL)' in ln for ln in rep.lines)


def test_objdiff_receives_the_original_object_when_it_has_no_secrel(tmp_path, monkeypatch):
    res, seen, obj_path = _score(tmp_path, monkeypatch, [], b'\xc3')
    assert seen['base'] == str(obj_path)
    assert not (tmp_path / '0x00401000' / 'base.obj').exists()
    assert res['objdiff_rewrites'] == 0
