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


def test_a_dot_segment_path_in_a_data_section_is_reported(tmp_path):
    path = b'C:\\work\\checkout\\op\\.\\Common\\DataStore\\DataStoreBuffer.h\0'
    objs = {'unsorted/a.cpp': write(tmp_path, 'a.obj', [(STRING_SYM, path, DATA, 2, [])])}
    assert objclash.build_path_lines(objs) == [
        f"unsorted/a.cpp: {STRING_SYM} uses the build path "
        "'C:\\\\work\\\\checkout\\\\op\\\\.\\\\Common\\\\DataStore\\\\DataStoreBuffer.h': an inline body "
        "that uses __FILE__ was emitted here, and the exe never has that string (give the body "
        "its original file and line with #line, or keep this unit from emitting it)"]


def test_a_unix_path_with_a_dot_segment_and_the_checkout_root_are_reported(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', [('s1', b'/src/op/.\\X.h\0', DATA, 2, [])]),
            'b.cpp': write(tmp_path, 'b.obj', [('s2', b'build\\op\\Y.cpp\0', DATA, 2, [])])}
    assert [x.split(' uses')[0] for x in objclash.build_path_lines(objs)] == ['a.cpp: s1']
    lines = objclash.build_path_lines(objs, roots=['build'])
    assert [x.split(' uses')[0] for x in lines] == ['a.cpp: s1', 'b.cpp: s2']


def test_the_finding_names_the_function_that_uses_the_string(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', [
        (STRING_SYM, b'/src/op/.\\Common\\X.h\0', DATA, 2, []),
        (GET, b'\x68\0\0\0\0\xc3', CODE, 2, [(1, STRING_SYM, coff.REL_DIR32)])])}
    lines = objclash.build_path_lines(objs)
    assert len(lines) == 1 and lines[0].startswith(f'a.cpp: {GET} uses the build path ')
    assert objclash.build_path_lines(objs, allowed={('a.cpp', GET)}) == []


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


def test_an_allowed_pair_is_not_reported(tmp_path):
    objs = {'a.cpp': write(tmp_path, 'a.obj', [('s1', b'/src/./X.h\0', DATA, 2, [])])}
    assert objclash.build_path_lines(objs, allowed={('a.cpp', 's1')}) == []


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
    objs = {'a.cpp': write(tmp_path, 'a.obj', clash_a + [('s1', b'/src/./X.h\0', DATA, 2, [])]),
            'b.cpp': write(tmp_path, 'b.obj', clash_b)}
    errors, stale = objclash.check(objs, {GET})
    assert len(errors) == 2 and stale == []
    base = tmp_path / 'base.tsv'
    base.write_text(f'kind\tunit\tsymbol\n# a comment\npath\ta.cpp\ts1\ncopy\t-\t{GET}\n'
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
    results = [{'address': '0x0042a31c', 'symbol': GET}]
    rep = Rep(objs, results)
    verify.report_object_clashes(rep, str(tmp_path / 'none.tsv'))
    assert rep.failed and [lv for lv, _ in rep.lines] == ['ERROR', 'ERROR']
    rep = Rep({'b.cpp': objs['b.cpp']}, results)
    verify.report_object_clashes(rep, str(tmp_path / 'none.tsv'))
    assert not rep.failed and rep.lines == []


def test_a_missing_baseline_is_no_debt_and_a_bad_row_is_refused(tmp_path):
    assert objclash.load_baseline(str(tmp_path / 'none.tsv')) == (set(), set())
    bad = tmp_path / 'bad.tsv'
    bad.write_text('other\tx\ty\n')
    try:
        objclash.load_baseline(str(bad))
    except ValueError as e:
        assert 'expected kind' in str(e)
    else:
        raise AssertionError('a malformed row was accepted')
