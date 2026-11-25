"""Near-miss labels (harness/lib/nearmiss.py): each label from small synthetic comparisons, the
precedence, the fallback, the reference listing and the stored labels."""
import os

from harness.lib import nearmiss, verify

def side_by_side(tmp_path, pairs):
    """A diff.txt written by the scorer's own writer, parsed back. `pairs` are
    (original offset or None, original text, rebuilt text, differs)."""
    left, right = [], []
    for off, a, b, differs in pairs:
        kind = 'DIFF_REPLACE' if differs else 'DIFF_NONE'
        left.append({'instruction': {'address': off, 'formatted': a}, 'diff_kind': kind}
                    if a else {'diff_kind': kind})
        right.append({'instruction': {'address': off or 0, 'formatted': b}, 'diff_kind': kind}
                     if b else {'diff_kind': kind})
    p = tmp_path / 'diff.txt'
    verify.write_side_by_side({'instructions': left}, {'instructions': right}, str(p))
    return nearmiss.parse_side_by_side(p.read_text())


SAME = b'\x55\x8b\xec\xe8\x00\x00\x00\x00\x5d\xc3'


def test_the_listing_is_read_back_as_written(tmp_path):
    rows = side_by_side(tmp_path, [(0, 'push ebp', 'push ebp', False),
                                   (None, '', 'mov ebx, ecx', True)])
    assert rows == [(False, 0, 'push ebp', 'push ebp'), (True, None, '', 'mov ebx, ecx')]


def test_reference_only_when_only_a_mismatched_field_differs(tmp_path):
    rows = side_by_side(tmp_path, [(0, 'push ebp', 'push ebp', False),
                                   (1, 'mov ebp, esp', 'mov ebp, esp', False),
                                   (3, 'call $exe_00401234', 'call ?f@@YAXXZ', True),
                                   (8, 'pop ebp', 'pop ebp', False), (9, 'ret', 'ret', False)])
    original = SAME[:4] + b'\x10\x20\x00\x00' + SAME[8:]
    relocs = [dict(offset=4, status='mismatch')]
    assert nearmiss.classify(rows, SAME, original, relocs) == 'reference-only'


def test_reference_only_when_the_strict_check_passes_but_a_relocation_row_is_counted(tmp_path):
    rows = side_by_side(tmp_path, [(0, 'push ebp', 'push ebp', False),
                                   (3, 'call 0x8', 'call ?f@@YAXXZ', True),
                                   (8, 'pop ebp', 'pop ebp', False)])
    assert nearmiss.classify(rows, SAME, SAME, [dict(offset=4, status='ok')]) == 'reference-only'
    # The same counted row with no relocation in it is not a reference.
    assert nearmiss.classify(rows, SAME, SAME, []) == 'structural'


def test_a_wrong_reference_and_a_body_difference_is_structural(tmp_path):
    rows = side_by_side(tmp_path, [(0, 'push esi', 'push edi', True),
                                   (3, 'call $exe_00401234', 'call ?f@@YAXXZ', True)])
    original = b'\x56' + SAME[1:4] + b'\x10\x20\x00\x00' + SAME[8:]
    relocs = [dict(offset=4, status='mismatch')]
    assert nearmiss.classify(rows, b'\x57' + SAME[1:], original, relocs) == 'structural'


def test_order_when_the_same_instructions_moved(tmp_path):
    rows = side_by_side(tmp_path, [(None, '', 'mov ebx, ecx', True),
                                   (0, 'push esi', 'push esi', False),
                                   (1, 'mov ebx, ecx', '', True)])
    assert nearmiss.classify(rows, b'\x8b\xd9\x56', b'\x56\x8b\xd9', []) == 'order'


def test_registers_when_only_register_names_differ(tmp_path):
    rows = side_by_side(tmp_path, [(0, 'mov esi, ecx', 'mov edi, ecx', True),
                                   (2, 'mov eax, [esi+0x48]', 'mov eax, [edi+0x48]', True)])
    assert nearmiss.classify(rows, b'\x01\x02\x03', b'\x01\x02\x04', []) == 'registers'


def test_stack_when_only_slots_and_frame_size_differ(tmp_path):
    rows = side_by_side(tmp_path, [(0, 'sub esp, 0x10', 'sub esp, 0xc', True),
                                   (3, 'lea eax, [esp+0x1c]', 'lea eax, [esp+0x18]', True)])
    assert nearmiss.classify(rows, b'\x01\x02', b'\x01\x03', []) == 'stack'


def test_precedence_stack_over_registers_and_order_over_both(tmp_path):
    # Slots and registers both differ: stack comes before registers.
    rows = side_by_side(tmp_path, [(0, 'sub esp, 0x10', 'sub esp, 0xc', True),
                                   (3, 'mov dl, [esp+0x13]', 'mov cl, [esp+0x13]', True)])
    assert nearmiss.classify(rows, b'\x01\x02', b'\x01\x03', []) == 'stack'
    # Two swapped instructions fit order and registers: order comes first.
    rows = side_by_side(tmp_path, [(0, 'mov eax, ecx', 'mov ecx, eax', True),
                                   (2, 'mov ecx, eax', 'mov eax, ecx', True)])
    assert nearmiss.classify(rows, b'\x01\x02', b'\x02\x01', []) == 'order'


def test_anything_else_is_structural(tmp_path):
    # An extra instruction.
    rows = side_by_side(tmp_path, [(0, 'push esi', 'push esi', False),
                                   (None, '', 'xor eax, eax', True)])
    assert nearmiss.classify(rows, b'\x56\x33\xc0', b'\x56', []) == 'structural'
    # Another operand that is neither a register nor a stack slot.
    rows = side_by_side(tmp_path, [(0, 'mov eax, [ecx+0x8]', 'mov eax, [ecx+0xc]', True)])
    assert nearmiss.classify(rows, b'\x01', b'\x02', []) == 'structural'
    # Nothing counted at all.
    rows = side_by_side(tmp_path, [(0, 'ret', 'ret', False)])
    assert nearmiss.classify(rows, b'\xc3', b'\xc3', []) == 'structural'


def test_the_register_mask_leaves_symbol_names_alone():
    assert nearmiss.mask_registers('call ?dl@@YAXXZ') == 'call ?dl@@YAXXZ'
    assert nearmiss.mask_registers('mov al, [ebx+0x4]') == 'mov R, [R+0x4]'


ROWS = {
    '0x00402000': dict(address='0x00402000', kind='function', name='tFoo::Bar', **{'class': 'tFoo'},
                       symbol='?Bar@tFoo@@QAEXXZ', symbol_evidence='mangled',
                       declaration='void tFoo::Bar()', evidence='mangled'),
    '0x00403000': dict(address='0x00403000', kind='function', name='fn_00403000', **{'class': ''},
                       symbol='', symbol_evidence='', declaration='', evidence='none'),
}


def ref(addr, symbol='?Other@@YAXXZ'):
    return dict(offset=0x15, type='REL32', symbol=symbol, exe_address=addr, status='mismatch',
                detail='the exe address is known as ?Bar@tFoo@@QAEXXZ')


def test_a_reference_to_a_full_row():
    r = nearmiss.reference(ref('0x00402000'), ROWS, {}, {})
    assert (r['kind'], r['level'], r['name'], r['cls'], r['table_symbol'], r['declaration']) == \
        ('function', 'full', 'tFoo::Bar', 'tFoo', '?Bar@tFoo@@QAEXXZ', 'void tFoo::Bar()')
    assert nearmiss.describe(r) == (
        '+0x15: the source names `?Other@@YAXXZ`; the original reaches 0x00402000; table: '
        '`tFoo::Bar`, class `tFoo`, symbol `?Bar@tFoo@@QAEXXZ`, declared `void tFoo::Bar()` '
        '(scorer: the exe address is known as ?Bar@tFoo@@QAEXXZ)')
    assert nearmiss.waits([r]) == ''


def test_a_reference_to_a_name_only_row_waits():
    r = nearmiss.reference(ref('0x00403000'), ROWS, {}, {})
    assert (r['kind'], r['level']) == ('function', 'name')
    assert 'the row has only a name (no symbol yet)' in nearmiss.describe(r)
    assert nearmiss.waits([r]) == 'unnamed: no symbol yet at 0x00403000'


def test_a_reference_with_no_row_waits():
    r = nearmiss.reference(ref('0x00404000'), ROWS, {}, {})
    assert (r['kind'], r['level']) == ('data', 'none')
    assert 'no table row and no name' in nearmiss.describe(r)
    assert nearmiss.waits([r]) == 'unnamed: no symbol yet at 0x00404000'


def test_a_reference_that_already_names_the_table_symbol_is_for_the_integrator():
    r = nearmiss.reference(ref('0x00402000', '?Bar@tFoo@@QAEXXZ'), ROWS, {}, {})
    assert nearmiss.waits([r]).startswith('evidence: ')


def test_labels_are_stored_per_address(tmp_path):
    path = str(tmp_path / 'lane' / 'nearmiss_labels.tsv')
    rec = dict(address='0x00401000', name='f', unit='a.cpp', label='reference-only', score=99.5,
               differing=1, references=[{}], waits='')
    nearmiss.store([rec, dict(rec, address='0x00401010', label='order', references=[])],
                   everything=True, path=path, now='T')
    got = nearmiss.load_labels(path)
    assert (got['0x00401000']['label'], got['0x00401000']['wrong_refs'], got['0x00401010']['label']) \
        == ('reference-only', '1', 'order')
    # A function that is exact now (no record) loses its row; the others stay.
    nearmiss.store([], ['0x00401000'], path=path)
    assert list(nearmiss.load_labels(path)) == ['0x00401010']
