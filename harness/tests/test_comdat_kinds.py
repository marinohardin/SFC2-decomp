"""One symbol, one COMDAT selection kind: objects the linker cannot merge are reported."""
import struct

from harness.lib import coff, verify


def make_obj(symbols):
    """A COFF object with one section per (name, kind): kind 1 or 2 makes a COMDAT section with
    that selection byte, None a plain section; each section defines one external symbol."""
    sections, syms, strings = [], bytearray(), bytearray(4)
    nsym = 0
    for i, (name, kind) in enumerate(symbols):
        chars = 0x60500020 | (coff.SCN_LNK_COMDAT if kind else 0)
        sections.append(struct.pack('<8sIIIIIIHHI', b'.text', 0, 0, 1, 0, 0, 0, 0, 0, chars))
        aux = struct.pack('<IHHIHB3x', 1, 0, 0, 0, 0, kind or 0)   # selection at aux offset 14
        syms += struct.pack('<8sIhHBB', b'.text', 0, i + 1, 0, coff.SYM_CLASS_STATIC, 1) + aux
        syms += struct.pack('<IIIhHBB', 0, len(strings), 0, i + 1, 0x20, coff.SYM_CLASS_EXTERNAL, 0)
        strings += name.encode() + b'\0'
        nsym += 3
    struct.pack_into('<I', strings, 0, len(strings))
    symoff = 20 + 40 * len(sections)
    head = struct.pack('<HHIIIHH', coff.IMAGE_FILE_MACHINE_I386, len(sections), 0, symoff, nsym, 0, 0)
    return head + b''.join(sections) + bytes(syms) + bytes(strings)


def write(tmp_path, name, symbols):
    path = tmp_path / name
    path.write_bytes(make_obj(symbols))
    return str(path)


DTOR = '??1tDynamicMemoryFreeList@@UAE@XZ'


def test_the_reader_exposes_the_selection_kind():
    obj = coff.CoffObject(make_obj([(DTOR, 1), ('?g@@YAXXZ', 2), ('?h@@YAXXZ', None)]))
    assert [obj.sections[n].selection for n in (1, 2, 3)] == [1, 2, None]


def test_mixed_kinds_for_one_symbol_are_an_error(tmp_path):
    objs = {'unsorted/tDynamicMemoryFreeList.cpp': write(tmp_path, 'a.obj', [(DTOR, 1)]),
            'unsorted/tShipSpecManager.cpp': write(tmp_path, 'b.obj', [(DTOR, 2)])}
    assert verify.comdat_kind_conflicts(objs) == [
        f'{DTOR} is defined with different COMDAT selection kinds: no duplicates (1) in '
        'unsorted/tDynamicMemoryFreeList.cpp; any (2) in unsorted/tShipSpecManager.cpp (no linker '
        'accepts the pair; give it one definition, the same in every unit)']


def test_the_same_kind_in_both_objects_is_fine(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', [(DTOR, 2)]),
            'b.cpp': write(tmp_path, 'b.obj', [(DTOR, 2)])}
    assert verify.comdat_kind_conflicts(objs) == []


def test_a_symbol_defined_in_one_object_only_is_fine(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', [(DTOR, 1)]),
            'b.cpp': write(tmp_path, 'b.obj', [('?other@@YAXXZ', 2)])}
    assert verify.comdat_kind_conflicts(objs) == []


def test_a_symbol_defined_outside_comdats_twice_is_an_error(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', [('_g', None)]),
            'b.cpp': write(tmp_path, 'b.obj', [('_g', None)])}
    assert verify.comdat_kind_conflicts(objs) == [
        '_g is defined outside a COMDAT in more than one object: a.cpp, b.cpp']


def test_a_conflict_fails_the_run_with_an_error_line_at_column_0(tmp_path):
    rep = verify.Report()
    rep.objects = {'a.cpp': write(tmp_path, 'a.obj', [(DTOR, 1)]),
                   'b.cpp': write(tmp_path, 'b.obj', [(DTOR, 2)])}
    verify.report_comdat_kinds(rep)
    assert rep.failed and rep.lines[0].startswith(f'ERROR {DTOR} is defined with different')
