"""Build paths in data sections and clashing copies of a claimed function (harness/lib/objclash.py)."""
import struct

from harness.lib import coff, objclash

CODE = 0x60500020 | coff.SCN_LNK_COMDAT     # a code COMDAT
DATA = 0xC0301040 | coff.SCN_LNK_COMDAT     # an initialized, writable data COMDAT
GET = '?Get@tBuffer@nDataStore@@QBEEK@Z'
STRING_SYM = '??_C@_0BB@abc@'


def make_obj(sections):
    """A COFF object. Each section is (symbol, data, chars, kind, relocs): the section holds
    `data`, defines one external symbol `symbol` (a function when chars has the code flag), and
    carries relocations as (offset, target symbol name, type) against undefined externals."""
    syms, strings, index = bytearray(), bytearray(4), {}
    nsym = 0

    def add_name(n):
        off = len(strings)
        strings.extend(n.encode() + b'\0')
        return off
    for i, (sym, data, chars, kind, relocs) in enumerate(sections):
        syms += struct.pack('<8sIhHBB', b'.text', 0, i + 1, 0, coff.SYM_CLASS_STATIC, 1)
        syms += struct.pack('<IHHIHB3x', len(data), 0, 0, 0, 0, kind)   # selection at offset 14
        typ = 0x20 if chars & 0x20 else 0
        syms += struct.pack('<IIIhHBB', 0, add_name(sym), 0, i + 1, typ,
                            coff.SYM_CLASS_EXTERNAL, 0)
        index[sym] = nsym + 2
        nsym += 3
    for sym, data, chars, kind, relocs in sections:
        for _, tgt, _ in relocs:
            if tgt not in index:
                syms += struct.pack('<IIIhHBB', 0, add_name(tgt), 0, 0, 0,
                                    coff.SYM_CLASS_EXTERNAL, 0)
                index[tgt] = nsym
                nsym += 1
    off = 20 + 40 * len(sections)
    body, headers = bytearray(), []
    for sym, data, chars, kind, relocs in sections:
        dataoff = off
        body += data
        off += len(data)
        reloff = off
        for r_off, tgt, typ in relocs:
            body += struct.pack('<IIH', r_off, index[tgt], typ)
            off += 10
        headers.append(struct.pack('<8sIIIIIIHHI', b'.text', 0, 0, len(data), dataoff,
                                   reloff if relocs else 0, 0, len(relocs), 0, chars))
    struct.pack_into('<I', strings, 0, len(strings))
    head = struct.pack('<HHIIIHH', coff.IMAGE_FILE_MACHINE_I386, len(sections), 0, off, nsym, 0, 0)
    return head + b''.join(headers) + bytes(body) + bytes(syms) + bytes(strings)


def write(tmp_path, name, sections):
    p = tmp_path / name
    p.write_bytes(make_obj(sections))
    return str(p)


def test_the_helper_round_trips():
    obj = coff.CoffObject(make_obj([(GET, b'\x55\xc3', CODE, 2, [(1, 'helper', coff.REL_REL32)])]))
    sec = obj.sections[1]
    assert sec.data == b'\x55\xc3' and sec.selection == 2
    assert [(o, obj.symbols[i].name, t) for o, i, t in sec.relocs] == [(1, 'helper', 0x14)]


def used(text, sym=STRING_SYM):
    """Sections: a string `text` and the function GET that refers to it."""
    return [(sym, text, DATA, 2, []),
            (GET, b'\x68\0\0\0\0\xc3', CODE, 2, [(1, sym, coff.REL_DIR32)])]


def test_a_dot_segment_path_used_by_a_function_is_reported(tmp_path):
    path = b'C:\\work\\checkout\\op\\.\\Common\\DataStore\\DataStoreBuffer.h\0'
    objs = {'unsorted/a.cpp': write(tmp_path, 'a.obj', used(path))}
    assert objclash.build_path_lines(objs) == [
        f"unsorted/a.cpp: {GET} uses the build path "
        "'C:\\\\work\\\\checkout\\\\op\\\\.\\\\Common\\\\DataStore\\\\DataStoreBuffer.h': an inline body "
        "that uses __FILE__ was emitted here, and the exe never has that string (give the body "
        "its original file and line with #line, or keep this unit from emitting it)"]
    assert objclash.build_path_lines(objs, allowed={('unsorted/a.cpp', GET)}) == []


def test_a_path_no_function_uses_is_an_error_that_cannot_be_baselined(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', [(STRING_SYM, b'/src/op/.\\X.h\0', DATA, 2, [])])}
    expected = ["a.cpp: the object holds the build path '/src/op/.\\\\X.h' and no function refers "
                "to it, so it cannot be baselined (find what emits it)"]
    assert objclash.build_path_lines(objs) == expected
    assert objclash.build_path_lines(objs, allowed={('a.cpp', STRING_SYM)}) == expected


def test_the_checkout_root_matches_whole_segments_only(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', used(b'/work/op/X.h\0')),
            'b.cpp': write(tmp_path, 'b.obj', used(b'/work2/op/X.h\0')),
            'c.cpp': write(tmp_path, 'c.obj', used(b'/home/work/op/X.h\0')),
            'd.cpp': write(tmp_path, 'd.obj', used(b'\\work\\op\\X.h\0'))}
    lines = objclash.build_path_lines(objs, roots=['/work'])
    assert [x.split(':')[0] for x in lines] == ['a.cpp', 'd.cpp']


def test_two_paths_in_one_section_are_both_found_and_a_string_without_nul_is_not(tmp_path):
    two = b'/a/./X.h\0/b/./Y.h\0'
    objs = {'a.cpp': write(tmp_path, 'a.obj', used(two)),
            'b.cpp': write(tmp_path, 'b.obj', used(b'/a/./X.h'))}
    obj = coff.CoffObject(make_obj(used(two)))
    assert [t for _, _, t in objclash.build_path_strings(obj)] == ['/a/./X.h', '/b/./Y.h']
    # one finding per (unit, function), not per string; a string cut off by the section end has no
    # terminating NUL, so it is not read as a string
    assert [x.split(':')[0] for x in objclash.build_path_lines(objs)] == ['a.cpp']


def test_the_originals_own_paths_and_other_strings_pass(tmp_path):
    ok = [('s1', b'..\\Common\\DataStore\\DataStoreBuffer.cpp\0', DATA, 2, []),
          ('s2', b'tBuffer\0', DATA, 2, []),
          ('s3', b'D:\\work\\Studio\\Core\\gfx\\view.cpp\0', DATA, 2, []),
          ('s4', b'../Common/Database/Database.h\0', DATA, 2, [])]
    objs = {'a.cpp': write(tmp_path, 'a.obj', ok)}
    assert objclash.build_path_lines(objs) == []


def test_code_sections_are_not_read_as_strings(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', [(GET, b'C:\\a\\b.h\0', CODE, 2, [])])}
    assert objclash.build_path_lines(objs) == []


def test_two_different_copies_of_a_claimed_function_are_an_error(tmp_path):
    a = [(GET, b'\x55\x8b\xec\xc3', CODE, 1, [(2, 'strA', coff.REL_DIR32)])]
    b = [(GET, b'\x55\x8b\xec\xc3', CODE, 2, [(2, 'strB', coff.REL_DIR32)])]
    objs = {'unsorted/fn_0042A000.cpp': write(tmp_path, 'a.obj', a),
            'unsorted/tClientCharacter.cpp': write(tmp_path, 'b.obj', b)}
    assert objclash.clashing_copies(objs, {GET}) == [
        f'{GET} is a claimed function that objects define differently: '
        'unsorted/fn_0042A000.cpp (4 bytes, 1 relocations); unsorted/tClientCharacter.cpp '
        '(4 bytes, 1 relocations) (a linker keeps the first copy it sees; give it one definition)']


def test_different_code_is_a_clash_and_equal_copies_are_not(tmp_path):
    same = [(GET, b'\x55\xc3', CODE, 2, [])]
    other = [(GET, b'\x55\x90\xc3', CODE, 2, [])]
    objs = {'a.cpp': write(tmp_path, 'a.obj', same), 'b.cpp': write(tmp_path, 'b.obj', same)}
    assert objclash.clashing_copies(objs, {GET}) == []
    objs['c.cpp'] = write(tmp_path, 'c.obj', other)
    assert len(objclash.clashing_copies(objs, {GET})) == 1


def test_an_unclaimed_symbol_may_differ(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', [(GET, b'\x55\xc3', CODE, 2, [])]),
            'b.cpp': write(tmp_path, 'b.obj', [(GET, b'\x90\xc3', CODE, 2, [])])}
    assert objclash.clashing_copies(objs, set()) == []


def test_the_baseline_hides_listed_findings_and_names_stale_rows(tmp_path):
    clash_a = [(GET, b'\x55\xc3', CODE, 2, [])]
    clash_b = [(GET, b'\x90\xc3', CODE, 2, [])]
    objs = {'a.cpp': write(tmp_path, 'a.obj', clash_a + [('s1', b'/src/./X.h\0', DATA, 2, []),
                                                  ('?f@@YAXXZ', b'\xc3', CODE, 2, [(0, 's1', coff.REL_DIR32)])]),
            'b.cpp': write(tmp_path, 'b.obj', clash_b)}
    errors, stale = objclash.check(objs, {GET})
    assert len(errors) == 2 and stale == []
    base = tmp_path / 'base.tsv'
    base.write_text(f'kind\tunit\tsymbol\n# a comment\npath\ta.cpp\t?f@@YAXXZ\ncopy\t-\t{GET}\n'
                    'copy\t-\t?gone@@YAXXZ\npath\tc.cpp\ts9\n')
    errors, stale = objclash.check(objs, {GET}, baseline=objclash.load_baseline(str(base)))
    assert errors == []
    assert stale == ['path\tc.cpp\ts9', 'copy\t-\t?gone@@YAXXZ']


def test_the_run_fails_on_a_build_path_and_on_a_clashing_claimed_copy(tmp_path, monkeypatch):
    from harness.lib import verify

    class Rep:
        def __init__(self, objects, results):
            self.objects, self.results, self.lines, self.failed = objects, results, [], False

        def add(self, level, text):
            self.lines.append((level, text))
    objs = {'a.cpp': write(tmp_path, 'a.obj', [(GET, b'\x55\xc3', CODE, 2, []),
                                               ('s1', b'/src/./X.h\0', DATA, 2, [])]),
            'b.cpp': write(tmp_path, 'b.obj', [(GET, b'\x90\xc3', CODE, 2, [])])}
    monkeypatch.setattr(verify, 'claimed_keys', lambda: {'0x0042a31c'})
    results = [{'address': '0x0042a31c', 'symbol': GET, 'unit': 'a.cpp'}]
    rep = Rep(objs, results)
    verify.report_object_clashes(rep, str(tmp_path / 'none.tsv'), table={})
    assert rep.failed and [lv for lv, _ in rep.lines] == ['ERROR', 'ERROR']
    rep = Rep({'b.cpp': objs['b.cpp']}, results)
    verify.report_object_clashes(rep, str(tmp_path / 'none.tsv'), table={})
    assert not rep.failed and rep.lines == []


def test_a_missing_baseline_is_no_debt_and_a_bad_row_is_refused(tmp_path):
    assert objclash.load_baseline(str(tmp_path / 'none.tsv')) == (set(), set(), set())
    bad = tmp_path / 'bad.tsv'
    bad.write_text('other\tx\ty\n')
    try:
        objclash.load_baseline(str(bad))
    except ValueError as e:
        assert 'expected kind' in str(e)
    else:
        raise AssertionError('a malformed row was accepted')


def test_baseline_matching_does_not_read_the_message_text(tmp_path, monkeypatch):
    objs = {'a.cpp': write(tmp_path, 'a.obj', used(b'/a/./X.h\0') + [(GET + 'x', b'\x90', CODE, 2, [])]),
            'b.cpp': write(tmp_path, 'b.obj', [(GET + 'x', b'\xc3', CODE, 2, [])])}
    base = (frozenset({('a.cpp', GET)}), frozenset({GET + 'x'}), frozenset())
    assert objclash.check(objs, {GET + 'x'}, baseline=base) == ([], [])
    real_copies, real_paths = objclash.copy_findings, objclash.path_findings
    monkeypatch.setattr(objclash, 'copy_findings',
                        lambda o, c: [(s, 'reworded ' + t) for s, t in real_copies(o, c)])
    monkeypatch.setattr(objclash, 'path_findings',
                        lambda o, r=(): [(u, s, x, 'reworded') for u, s, x, _ in real_paths(o, r)])
    assert objclash.check(objs, {GET + 'x'}, baseline=base) == ([], [])
    errors, _ = objclash.check(objs, {GET + 'x'})
    assert len(errors) == 2 and errors[0] == 'reworded' and errors[1].startswith('reworded ')


def test_a_baseline_file_with_crlf_and_comments_loads(tmp_path):
    f = tmp_path / 'b.tsv'
    f.write_bytes(b'# note\r\nkind\tunit\tsymbol\r\n\r\npath\ta.cpp\t?f@@YAXXZ\r\n'
                  b'# another\r\ncopy\t-\t?g@@YAXXZ\r\n')
    assert objclash.load_baseline(str(f)) == ({('a.cpp', '?f@@YAXXZ')}, {'?g@@YAXXZ'}, set())


CTOR = '??0tPhysicalEntity@@QAE@XZ'
OTHER = '?Other@tPhysicalEntity@@QAEXXZ'


def unclaimed(tmp_path, sections, addresses, claims, unit='a.cpp', baseline=None):
    objs = {unit: write(tmp_path, 'u.obj', sections)}
    return objclash.check(objs, set(), baseline=baseline or (frozenset(),) * 3,
                          symbol_addresses=addresses, unit_claims=claims)


def test_a_body_under_an_originals_symbol_without_a_claim_is_an_error(tmp_path):
    errors, stale = unclaimed(tmp_path, [(CTOR, b'\xc3', CODE, 1, [])],
                              {CTOR: {0x412f67}}, {'a.cpp': {0x401000}})
    assert stale == [] and errors == [
        f"a.cpp: {CTOR} is defined here, and the symbol table places it at 0x00412f67, but no "
        "// FUNCTION: claim in this unit covers that address, so the body is not held to the "
        "original's bytes while it carries the original's symbol: claim the function as a real "
        "attempt at the original's code, remove the body, or give it a placeholder name (not the "
        "original's symbol)"]


def test_a_claim_in_the_unit_at_any_address_of_the_symbol_covers_it(tmp_path):
    body = [(CTOR, b'\xc3', CODE, 1, [])]
    # the symbol is known at a folded copy and at the claimed address
    assert unclaimed(tmp_path, body, {CTOR: {0x412f67, 0x500000}}, {'a.cpp': {0x500000}}) == ([], [])
    # a claim of that address in another unit does not cover this unit's body
    errors, _ = unclaimed(tmp_path, body, {CTOR: {0x412f67}}, {'b.cpp': {0x412f67}})
    assert len(errors) == 1 and '0x00412f67' in errors[0]


def test_comdat_copies_undecorated_and_unknown_symbols_are_not_read(tmp_path):
    secs = [(CTOR, b'\xc3', CODE, 2, []),                       # inline or template copy (selection any)
            ('_main', b'\xc3', CODE, 1, []),   # not decorated
            (OTHER, b'\xc3', CODE, 1, []),     # not in the table
            ('data', b'\x00', DATA, 1, [])]    # not a function
    addrs = {CTOR: {0x412f67}, '_main': {0x401000}, 'data': {0x500000}}
    assert unclaimed(tmp_path, secs, addrs, {}) == ([], [])


def test_the_unclaimed_baseline_hides_a_row_and_a_gone_row_is_stale(tmp_path):
    body = [(CTOR, b'\xc3', CODE, 1, [])]
    base = (frozenset(), frozenset(), frozenset({('a.cpp', CTOR), ('a.cpp', OTHER)}))
    assert unclaimed(tmp_path, body, {CTOR: {0x412f67}}, {}, baseline=base) == (
        [], [f'unclaimed\ta.cpp\t{OTHER}'])
    # the same symbol in another unit is a new case
    errors, _ = unclaimed(tmp_path, body, {CTOR: {0x412f67}}, {}, unit='b.cpp', baseline=base)
    assert len(errors) == 1


def test_the_unclaimed_baseline_loads_and_the_check_is_off_without_a_table(tmp_path):
    f = tmp_path / 'b.tsv'
    f.write_text(f'kind\tunit\tsymbol\nunclaimed\ta.cpp\t{CTOR}\n')
    assert objclash.load_baseline(str(f)) == (set(), set(), {('a.cpp', CTOR)})
    objs = {'a.cpp': write(tmp_path, 'a.obj', [(CTOR, b'\xc3', CODE, 1, [])])}
    assert objclash.check(objs, set(), baseline=objclash.load_baseline(str(f))) == ([], [])


def test_the_inputs_come_from_the_table_and_the_results():
    from harness.lib import verify
    table = {'0x00412f67': (CTOR, 'mangled'), '0x00500000': (OTHER, 'callers'),
             '0x00600000': ('?Stale@@YAXXZ', 'claim'), '0x00700000': ('?Cl@@YAXXZ', 'claim'),
             '0x00800000': ('?Add@@YAXXZ', 'add')}
    results = [{'address': '0x00700000', 'symbol': '?Cl@@YAXXZ', 'unit': 'a.cpp'},
               {'address': '0x00900000', 'unit': 'a.cpp'},
               {'address': '0x00a00000', 'symbol': '?New@@YAXXZ', 'unit': 'b.cpp'}]
    addrs, units = verify.unclaimed_inputs(results, table)
    assert addrs == {CTOR: {0x412f67}, '?Cl@@YAXXZ': {0x700000}, '?Add@@YAXXZ': {0x800000},
                     '?New@@YAXXZ': {0xa00000}}
    assert units == {'a.cpp': {0x700000, 0x900000}, 'b.cpp': {0xa00000}}


def test_the_run_fails_on_an_unclaimed_original(tmp_path, monkeypatch):
    from harness.lib import verify

    class Rep:
        def __init__(self, objects, results):
            self.objects, self.results, self.lines, self.failed = objects, results, [], False

        def add(self, level, text):
            self.lines.append((level, text))
    objs = {'a.cpp': write(tmp_path, 'a.obj', [(CTOR, b'\xc3', CODE, 1, [])])}
    monkeypatch.setattr(verify, 'claimed_keys', lambda: set())
    table = {'0x00412f67': (CTOR, 'mangled')}
    rep = Rep(objs, [])
    verify.report_object_clashes(rep, str(tmp_path / 'none.tsv'), table=table)
    assert rep.failed and [lv for lv, _ in rep.lines] == ['ERROR']
    base = tmp_path / 'base.tsv'
    base.write_text(f'unclaimed\ta.cpp\t{CTOR}\nunclaimed\ta.cpp\t{OTHER}\n')
    rep = Rep(objs, [])
    verify.report_object_clashes(rep, str(base), table=table)
    assert not rep.failed and [lv for lv, _ in rep.lines] == ['WARN']


def test_a_function_outside_a_comdat_is_read_too(tmp_path):
    errors, _ = unclaimed(tmp_path, [(CTOR, b'\xc3', CODE & ~coff.SCN_LNK_COMDAT, 0, [])],
                          {CTOR: {0x412f67}}, {})
    assert len(errors) == 1
