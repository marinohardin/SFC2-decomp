"""Tests for tools/lint_claims.py.

Each test builds an invented op/ tree and its evidence (matched set, boundary list, functions.csv,
baseline) under pytest's tmp_path; the expected breaks are written out by hand.
"""
import importlib.util
import json
import pathlib
import struct

ROOT = pathlib.Path(__file__).resolve().parents[1]


def _load():
    spec = importlib.util.spec_from_file_location('lint_claims', ROOT / 'tools/lint_claims.py')
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


lint_claims = _load()

CSV_HEADER = ('address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,'
              'headers,is_library,library,kind,tags\n')


def make_repo(tmp_path, files, matched=None, functions=(), boundaries=(), baseline=None):
    """An invented repository: `files` maps paths under op/ to their text; `matched` maps
    addresses to (size, exact); `functions` is [(address, size, name, tags)]; `boundaries` is
    [(address, kind)]; `baseline` is [(check, path, address, reason)] or None for no file."""
    for rel, text in files.items():
        p = tmp_path / 'op' / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)
    res = tmp_path / 'analysis' / 'results'
    res.mkdir(parents=True)
    res.joinpath('matched_set.json').write_text(json.dumps({'version': 1, 'functions': {
        a: {'name': 'TestFn', 'size': size, 'score': 100.0 if exact else 50.0, 'exact': exact}
        for a, (size, exact) in (matched or {}).items()}}))
    res.joinpath('boundary_errors.tsv').write_text(
        'address\tname\tpart_of\tkind\tnote\n'
        + ''.join(f'{a}\tFUN_x\t0x00400000\t{k}\tinvented\n' for a, k in boundaries))
    exports = tmp_path / 'ghidra' / 'exports'
    exports.mkdir(parents=True)
    exports.joinpath('functions.csv').write_text(
        CSV_HEADER + ''.join(f'{a},{size},{name},,,default,,,,0,,function,{tags}\n'
                             for a, size, name, tags in functions))
    if baseline is not None:
        res.joinpath('lint_baseline.tsv').write_text(
            'check\tpath\taddress\treason\n' + ''.join('\t'.join(r) + '\n' for r in baseline))
    return tmp_path


def run(repo, capsys, *extra):
    # No executable unless a test names one: the falls-through rule must not read the real install.
    rc = lint_claims.main(['--repo', str(repo), '--exe', str(repo / 'no-such.exe'), *extra])
    out = capsys.readouterr().out.splitlines()
    return rc, [ln for ln in out if not ln.startswith('lint_claims:')]


EMPTY_FN = """// FUNCTION: STARFLEETOP 0x00401000
void tTestCaptain::Idle()
{
	// nothing to do
}
"""


# --- empty-body ------------------------------------------------------------------------------

def test_an_empty_body_that_is_not_exact_is_a_break(tmp_path, capsys):
    repo = make_repo(tmp_path, {'unsorted/tTestCaptain.cpp': EMPTY_FN},
                     matched={'0x00401000': (14, False)})
    rc, out = run(repo, capsys)
    assert rc == 1
    assert out == ["op/unsorted/tTestCaptain.cpp:1: empty-body: FUNCTION 0x00401000 claims an "
                   "empty body and is not exact: an empty body is allowed only where it compiles "
                   "to the original's bytes"]


def test_an_exact_empty_body_and_an_initializer_list_pass(tmp_path, capsys):
    src = EMPTY_FN + """
// FUNCTION: STARFLEETOP 0x00401010
tTestCaptain::tTestCaptain() : fRank( 0 )
{
}
"""
    repo = make_repo(tmp_path, {'unsorted/tTestCaptain.cpp': src},
                     matched={'0x00401000': (1, True), '0x00401010': (12, False)})
    assert run(repo, capsys) == (0, [])


def test_an_unclaimed_destructor_holding_only_a_deleting_destructor_is_a_break(tmp_path, capsys):
    src = """// FUNCTION: STARFLEETOP 0x00401020
tTestCaptain::~tTestCaptain()
{
}
"""
    repo = make_repo(tmp_path, {'unsorted/tTestCaptain.cpp': src},
                     matched={'0x00401020': (28, True)},
                     functions=[('00401020', 28, 'tTestCaptain::scalar_deleting_destructor', '')])
    rc, out = run(repo, capsys)
    assert rc == 1
    assert [ln.split(' (')[0] for ln in out] == [
        'op/unsorted/tTestCaptain.cpp:1: empty-body: FUNCTION 0x00401020']


def test_a_deleting_destructor_on_its_real_empty_destructor_passes(tmp_path, capsys):
    claimed = """// FUNCTION: STARFLEETOP 0x00401020
// FUNCTION: STARFLEETOP 0x00401040
tTestCaptain::~tTestCaptain()
{
}
"""
    explained = """// MATCH-ONLY: tTestShip's destructor is implicit; written out so the compiler emits
// the scalar deleting destructor from it.
// FUNCTION: STARFLEETOP 0x00402020
tTestShip::~tTestShip()
{
}
"""
    repo = make_repo(tmp_path, {'unsorted/tTestCaptain.cpp': claimed,
                                'unsorted/tTestShip.cpp': explained},
                     matched={'0x00401020': (28, True), '0x00401040': (7, True),
                              '0x00402020': (28, True)},
                     functions=[('00401020', 28, 'tTestCaptain::scalar_deleting_destructor', ''),
                                ('00401040', 7, 'tTestCaptain::~tTestCaptain', ''),
                                ('00402020', 28, 'tTestShip::scalar_deleting_destructor', '')])
    assert run(repo, capsys) == (0, [])


# --- fragment-claim --------------------------------------------------------------------------

def test_a_claim_on_a_listed_fragment_or_inside_a_joined_function_is_a_break(tmp_path, capsys):
    src = """// FUNCTION: STARFLEETOP 0x00401000
void fn_00401000()
{
	g_count++;
}

// FUNCTION: STARFLEETOP 0x0040105a
void fn_0040105a()
{
	g_count--;
}
"""
    repo = make_repo(tmp_path, {'unsorted/fn_00401000.cpp': src},
                     matched={'0x00401000': (20, True), '0x0040105a': (30, False)},
                     functions=[('00401000', 20, 'fn_00401000', ''),
                                ('00401050', 60, 'tTestCaptain::Fire', 'eh-prolog-joined')],
                     boundaries=[('0x00401000', 'body-fragment')])
    rc, out = run(repo, capsys)
    assert rc == 1
    assert out == [
        'op/unsorted/fn_00401000.cpp:1: fragment-claim: FUNCTION 0x00401000 is listed as '
        'body-fragment in boundary_errors.tsv: not a function of its own',
        'op/unsorted/fn_00401000.cpp:7: fragment-claim: FUNCTION 0x0040105a lies inside '
        '0x00401050 (eh-prolog-joined: its exception prolog and body joined) in functions.csv: '
        'the tail of its neighbour',
    ]


def test_a_claim_on_a_function_start_or_another_boundary_kind_passes(tmp_path, capsys):
    src = """// FUNCTION: STARFLEETOP 0x00401050
void tTestCaptain::Fire()
{
	g_count++;
}
"""
    repo = make_repo(tmp_path, {'unsorted/tTestCaptain.cpp': src},
                     matched={'0x00401050': (60, True)},
                     functions=[('00401050', 60, 'tTestCaptain::Fire', 'eh-prolog-joined')],
                     boundaries=[('0x00401050', 'thunk')])
    assert run(repo, capsys) == (0, [])


# --- address-order ---------------------------------------------------------------------------

def fn(address, name):
    return f'// FUNCTION: STARFLEETOP 0x{address}\nvoid {name}()\n{{\n\tg_count++;\n}}\n\n'


def test_a_function_out_of_address_order_is_a_break(tmp_path, capsys):
    src = (fn('00401000', 'fn_a') + fn('00401300', 'fn_misplaced') + fn('00401100', 'fn_b')
           + fn('00401200', 'fn_c'))
    repo = make_repo(tmp_path, {'unsorted/fn_00401000.cpp': src})
    rc, out = run(repo, capsys)
    assert rc == 1
    assert out == ['op/unsorted/fn_00401000.cpp:7: address-order: 0x00401300 is out of address '
                   'order: it sits after 0x00401000 (line 1) and before 0x00401100 (line 13)']


def test_stacks_and_a_match_only_function_do_not_break_the_order(tmp_path, capsys):
    src = (fn('00401000', 'fn_a')
           # a global's stack: the initializer directly above the definition, its atexit
           # registration and destructor above that, at higher addresses
           + '// FUNCTION: STARFLEETOP 0x00401900\n// FUNCTION: STARFLEETOP 0x00401800\n'
           '// FUNCTION: STARFLEETOP 0x00401100\ntTestCaptain g_tTestCaptain;\n\n'
           + '// MATCH-ONLY: defined first so the compiler inlines it into fn_c.\n'
           + fn('00401500', 'fn_helper') + fn('00401200', 'fn_c'))
    repo = make_repo(tmp_path, {'unsorted/fn_00401000.cpp': src})
    assert run(repo, capsys) == (0, [])


# --- non-source ------------------------------------------------------------------------------

def test_a_stray_file_under_op_is_a_break(tmp_path, capsys):
    repo = make_repo(tmp_path, {'unsorted/tTestCaptain.cpp.part': ''})
    rc, out = run(repo, capsys)
    assert rc == 1
    assert out == ["op/unsorted/tTestCaptain.cpp.part:1: non-source: not a source, header, "
                   "README.md or one of op/'s tables"]


def test_sources_headers_readmes_and_the_root_tables_pass(tmp_path, capsys):
    repo = make_repo(tmp_path, {'README.md': '# op\n', 'UNITS.tsv': 'unit\n',
                                'manifest.tsv': 'address\n', 'Tiger/README.md': '# Tiger\n',
                                'Tiger/tTest.h': '', 'Tiger/tTest.inl': '', 'Tiger/tTest.cpp': '',
                                '.DS_Store': ''})
    assert run(repo, capsys) == (0, [])


def test_the_named_library_header_passes_at_the_root_only(tmp_path, capsys):
    repo = make_repo(tmp_path, {'xtree': '', 'xstring': '', 'Tiger/xtree': '',
                                'Tiger/xstring': '', 'map': ''})
    rc, out = run(repo, capsys)
    assert rc == 1
    assert out == ["op/Tiger/xstring:1: non-source: not a source, header, README.md or one of "
                   "op/'s tables",
                   "op/Tiger/xtree:1: non-source: not a source, header, README.md or one of "
                   "op/'s tables",
                   "op/map:1: non-source: not a source, header, README.md or one of op/'s tables"]


# --- the baseline ----------------------------------------------------------------------------

def test_a_break_in_the_baseline_passes_and_a_new_one_still_fails(tmp_path, capsys):
    src = (fn('00401000', 'fn_a') + fn('00401300', 'fn_old') + fn('00401100', 'fn_b')
           + fn('00401200', 'fn_c'))
    path = 'op/unsorted/fn_00401000.cpp'
    repo = make_repo(tmp_path, {'unsorted/fn_00401000.cpp': src, 'unsorted/x.bak': ''},
                     baseline=[('address-order', path, '0x00401300', 'before the check')])
    rc, out = run(repo, capsys)
    assert rc == 1
    assert out == ["op/unsorted/x.bak:1: non-source: not a source, header, README.md or one of "
                   "op/'s tables"]
    (repo / 'op' / 'unsorted' / 'x.bak').unlink()
    assert run(repo, capsys) == (0, [])


def test_a_new_function_cannot_move_the_blame_off_a_baseline_entry(tmp_path, capsys):
    # The baseline holds 0x00401300. A new 0x00401350 written after it, ahead of 0x00401100, is
    # reported itself; it does not make 0x00401300 look in order and 0x00401100 out of it.
    src = (fn('00401000', 'fn_a') + fn('00401300', 'fn_old') + fn('00401350', 'fn_new')
           + fn('00401100', 'fn_b') + fn('00401400', 'fn_c'))
    path = 'op/unsorted/fn_00401000.cpp'
    repo = make_repo(tmp_path, {'unsorted/fn_00401000.cpp': src},
                     baseline=[('address-order', path, '0x00401300', 'before the check')])
    rc, out = run(repo, capsys)
    assert rc == 1
    assert out == ['op/unsorted/fn_00401000.cpp:13: address-order: 0x00401350 is out of address '
                   'order: it sits after 0x00401000 (line 1) and before 0x00401100 (line 19)']


def test_a_baseline_row_that_no_longer_occurs_fails_as_stale(tmp_path, capsys):
    src = fn('00401000', 'fn_a') + fn('00401100', 'fn_b') + fn('00401300', 'fn_moved')
    path = 'op/unsorted/fn_00401000.cpp'
    repo = make_repo(tmp_path, {'unsorted/fn_00401000.cpp': src},
                     baseline=[('address-order', path, '0x00401300', 'before the check'),
                               ('non-source', 'op/unsorted/gone.part', '-', 'removed since')])
    rc, out = run(repo, capsys)
    assert rc == 1
    assert out == [
        'op/unsorted/fn_00401000.cpp: address-order: 0x00401300 is in the baseline but no longer '
        'occurs; delete its row from the baseline',
        'op/unsorted/gone.part: non-source: - is in the baseline but no longer occurs; delete '
        'its row from the baseline',
    ]


def test_a_global_with_a_brace_initializer_has_no_body(tmp_path, capsys):
    src = """// FUNCTION: STARFLEETOP 0x00401000
tTestTable g_testTable = {};
"""
    repo = make_repo(tmp_path, {'unsorted/tTestTable.cpp': src},
                     matched={'0x00401000': (8, False)})
    assert run(repo, capsys) == (0, [])


def test_an_empty_assignment_operator_is_still_a_body(tmp_path, capsys):
    src = """// FUNCTION: STARFLEETOP 0x00401000
tTestCaptain &tTestCaptain::operator=(const tTestCaptain &other)
{
}
"""
    repo = make_repo(tmp_path, {'unsorted/tTestCaptain.cpp': src},
                     matched={'0x00401000': (14, False)})
    rc, out = run(repo, capsys)
    assert rc == 1
    assert out == ["op/unsorted/tTestCaptain.cpp:1: empty-body: FUNCTION 0x00401000 claims an "
                   "empty body and is not exact: an empty body is allowed only where it compiles "
                   "to the original's bytes"]


def test_a_written_baseline_keeps_its_reasons_and_checks_clean(tmp_path, capsys):
    other = EMPTY_FN.replace('0x00401000', '0x00402000').replace('tTestCaptain', 'tTestPilot')
    repo = make_repo(tmp_path, {'unsorted/tTestCaptain.cpp': EMPTY_FN,
                                'unsorted/tTestPilot.cpp': other},
                     matched={'0x00401000': (14, False), '0x00402000': (14, False)},
                     baseline=[('empty-body', 'op/unsorted/tTestCaptain.cpp', '0x00401000',
                                'kept by hand')])
    rc, _ = run(repo, capsys, '--write-baseline')
    assert rc == 0
    rows = (repo / 'analysis' / 'results' / 'lint_baseline.tsv').read_text().splitlines()
    assert rows == [
        'check\tpath\taddress\treason',
        'empty-body\top/unsorted/tTestCaptain.cpp\t0x00401000\tkept by hand',
        'empty-body\top/unsorted/tTestPilot.cpp\t0x00402000\tFUNCTION 0x00402000 claims an '
        "empty body and is not exact: an empty body is allowed only where it compiles to the "
        "original's bytes",
    ]
    assert run(repo, capsys) == (0, [])


def line_fn(address, body, pre=''):
    return f"// FUNCTION: STARFLEETOP 0x{address:08x}\n{pre}void tTestShip::Check()\n{{\n{body}}}\n"


def warnings_of(repo, capsys):
    rc = lint_claims.main(['--repo', str(repo)])
    out = capsys.readouterr().out.splitlines()
    return rc, [ln for ln in out if 'line-macro' in ln], out[-2]


def test_a_claimed_function_using_a_line_macro_without_a_directive_is_flagged(tmp_path, capsys):
    repo = make_repo(tmp_path, {'a.cpp': line_fn(0x401000, '\tfn_x(__FILE__, __LINE__);\n')})
    rc, warns, summary = warnings_of(repo, capsys)
    assert rc == 0
    assert len(warns) == 1 and warns[0].startswith('op/a.cpp:4: line-macro: warning')
    assert '0x00401000' in warns[0]
    assert '1 claimed functions use one, 0 governed by #line, 1 flagged' in summary


def test_a_line_directive_before_the_use_governs_it(tmp_path, capsys):
    body = '#line 3504\n\tfn_x(__FILE__, __LINE__);\n#line 7\n'
    text = line_fn(0x401000, body) + line_fn(0x401100, '\tfn_y();\n', '#line 900\n') \
        + line_fn(0x401200, '\tassert(1);\n', '#line 12\n')
    repo = make_repo(tmp_path, {'a.cpp': text})
    rc, warns, summary = warnings_of(repo, capsys)
    assert rc == 0 and warns == []
    assert '2 claimed functions use one, 2 governed by #line, 0 flagged' in summary


def test_a_directive_in_an_earlier_function_does_not_govern_a_later_one(tmp_path, capsys):
    text = line_fn(0x401000, '#line 5\n\tassert(1);\n') + line_fn(0x401100, '\tassert(2);\n')
    repo = make_repo(tmp_path, {'a.cpp': text})
    rc, warns, summary = warnings_of(repo, capsys)
    assert len(warns) == 1 and '0x00401100' in warns[0]
    assert '2 claimed functions use one, 1 governed by #line, 1 flagged' in summary


def test_a_project_macro_is_recognised_through_other_macros(tmp_path, capsys):
    header = ('#define WHERE_ __LINE__\n'
              '#define SAY(a) \\\n   fn_say(a, WHERE_)\n'
              '#define SAYMORE(a) { SAY(a); }\n')
    repo = make_repo(tmp_path, {'h.h': header,
                                'a.cpp': line_fn(0x401000, '\tSAYMORE("x");\n')})
    rc, warns, summary = warnings_of(repo, capsys)
    assert rc == 0 and len(warns) == 1
    assert '1 claimed functions use one' in summary


def test_uses_in_comments_and_strings_are_ignored(tmp_path, capsys):
    body = '\t// assert(x) and __LINE__\n\tfn_x("assert __FILE__");\n\t/* assert(y) */\n'
    repo = make_repo(tmp_path, {'a.cpp': line_fn(0x401000, body)})
    rc, warns, summary = warnings_of(repo, capsys)
    assert warns == []
    assert '0 claimed functions use one' in summary


def test_an_unclaimed_function_is_not_reported(tmp_path, capsys):
    text = 'void tTestShip::Check()\n{\n\tassert(1);\n}\n'
    repo = make_repo(tmp_path, {'a.cpp': text})
    rc, warns, summary = warnings_of(repo, capsys)
    assert rc == 0 and warns == []
    assert '0 claimed functions use one' in summary


def test_warnings_do_not_reach_the_baseline_or_the_exit_status(tmp_path, capsys):
    repo = make_repo(tmp_path, {'a.cpp': line_fn(0x401000, '\tassert(1);\n')}, baseline=[])
    assert lint_claims.main(['--repo', str(repo), '--write-baseline']) == 0
    capsys.readouterr()
    assert (tmp_path / 'analysis/results/lint_baseline.tsv').read_text() == \
        'check\tpath\taddress\treason\n'
    assert lint_claims.main(['--repo', str(repo)]) == 0


def test_a_project_assert_without_a_line_is_not_flagged(tmp_path, capsys):
    repo = make_repo(tmp_path, {'t.h': '#define assert(x) { if (!(x)) throw 1; }\n',
                                'a.cpp': '#include "t.h"\n' + line_fn(0x401000, '\tassert(1);\n')})
    rc, warns, summary = warnings_of(repo, capsys)
    assert warns == [] and '0 claimed functions use one' in summary


def test_a_project_assert_with_a_line_is_flagged(tmp_path, capsys):
    repo = make_repo(tmp_path, {'t.h': '#define assert(x) fn_fail(__LINE__)\n',
                                'a.cpp': '#include "t.h"\n' + line_fn(0x401000, '\tassert(1);\n')})
    rc, warns, summary = warnings_of(repo, capsys)
    assert len(warns) == 1 and '1 claimed functions use one' in summary


def test_assert_with_no_project_definition_is_the_built_in(tmp_path, capsys):
    repo = make_repo(tmp_path, {'a.cpp': line_fn(0x401000, '\tassert(1);\n')})
    rc, warns, summary = warnings_of(repo, capsys)
    assert len(warns) == 1


def test_assert_defined_two_ways_follows_the_included_header(tmp_path, capsys):
    repo = make_repo(tmp_path, {'t.h': '#define assert(x) fn_fail(__LINE__)\n',
                                'u.h': '#define assert(x) { }\n',
                                'a.cpp': '#include "u.h"\n' + line_fn(0x401000, '\tassert(1);\n'),
                                'b.cpp': '#include "t.h"\n' + line_fn(0x401100, '\tassert(1);\n'),
                                'c.cpp': line_fn(0x401200, '\tassert(1);\n')})
    rc, warns, summary = warnings_of(repo, capsys)
    assert len(warns) == 1 and 'op/b.cpp' in warns[0]


# --- falls-through -----------------------------------------------------------------------------
# A hand-built PE image: one .text section at 0x401000 (image base 0x400000, section at 0x1000,
# raw data at file offset 0x200, 0x90 bytes). Offsets below are from 0x401000; every byte is
# written out. Gaps are 0xCC.
#
#   0x00  33 C0 C3                  f1, 3 bytes, ends in ret
#   0x10  6A 00 E8 E9 FF FF FF      f2, 7 bytes, ends in call (to f1); 0x17 is called by f4: ok
#   0x17  C3                        f3, reached by f4's call
#   0x20  E8 F2 FF FF FF C3         f4, 6 bytes, ends in ret; its call goes to 0x17
#   0x30  6A 00 E8 E0 FF FF FF      f5, 7 bytes, ends in call (to 0x17); 0x37 refers to nothing
#   0x37  C3                        f5's unreferenced continuation
#   0x40  6A 00 E8 B9 FF FF FF 90 90 90   f6, 10 bytes with three nops of padding, ends in call
#   0x4A  C3                        f6's unreferenced continuation
#   0x50  6A 00 E8 A9 FF FF FF      f7, 7 bytes, ends in call; CC CC CC, then 0x5A is referenced
#   0x5A  C3                        f8, referenced by the dword at 0x60
#   0x60  5A 10 40 00               a pointer to f8
#   0x70  6A 00 E8 89 FF FF FF      f9, 7 bytes, ends in call; 0x77 is referenced by the dword
#   0x77  C3                        at 0x80
#   0x80  77 10 40 00               a pointer to 0x77
IMAGE_PIECES = {
    0x00: '33C0C3',
    0x10: '6A00E8E9FFFFFF',
    0x17: 'C3',
    0x20: 'E8F2FFFFFFC3',
    0x30: '6A00E8E0FFFFFF',
    0x37: 'C3',
    0x40: '6A00E8B9FFFFFF909090',
    0x4A: 'C3',
    0x50: '6A00E8A9FFFFFF',
    0x5A: 'C3',
    0x60: '5A104000',
    0x70: '6A00E889FFFFFF',
    0x77: 'C3',
    0x80: '77104000',
}
# (address, size) of the claims, as functions.csv would give them
FALL_CLAIMS = {'f1': (0x401000, 3), 'f2': (0x401010, 7), 'f4': (0x401020, 6),
               'f5': (0x401030, 7), 'f6': (0x401040, 10), 'f7': (0x401050, 7),
               'f9': (0x401070, 7)}


def build_pe(path):
    text = bytearray(b'\xcc' * 0x90)
    for off, hexbytes in IMAGE_PIECES.items():
        b = bytes.fromhex(hexbytes)
        text[off:off + len(b)] = b
    head = bytearray(0x200)
    head[0:2] = b'MZ'
    struct.pack_into('<I', head, 0x3C, 0x40)
    head[0x40:0x44] = b'PE\0\0'
    struct.pack_into('<HHIIIHH', head, 0x44, 0x14C, 1, 0, 0, 0, 0xE0, 0x102)
    struct.pack_into('<I', head, 0x58 + 28, 0x400000)             # ImageBase
    struct.pack_into('<8sIIII', head, 0x58 + 0xE0, b'.text', 0x90, 0x1000, 0x90, 0x200)
    path.write_bytes(bytes(head) + bytes(text))
    return path


def fall_repo(tmp_path, names=tuple(FALL_CLAIMS), matched=None, baseline=None):
    files = {'fall.c': ''.join(
        f'// FUNCTION: STARFLEETOP 0x{FALL_CLAIMS[n][0]:08x}\nvoid {n}() {{ x(); }}\n\n'
        for n in sorted(names, key=lambda n: FALL_CLAIMS[n]))}
    return make_repo(tmp_path, files, matched=matched, baseline=baseline,
                     functions=[(f'{FALL_CLAIMS[n][0]:08x}', FALL_CLAIMS[n][1], n, '')
                                for n in names])


def fall_run(tmp_path, capsys, *extra):
    exe = build_pe(tmp_path / 'image.exe')
    return run(tmp_path, capsys, '--exe', str(exe), '--no-hash', *extra)


def fall_addresses(out):
    return sorted(ln.split('FUNCTION ')[1].split(' ')[0] for ln in out if 'falls-through:' in ln)


def test_a_claim_ending_in_a_call_with_nothing_after_it_is_a_break(tmp_path, capsys):
    fall_repo(tmp_path)
    rc, out = fall_run(tmp_path, capsys)
    # f5: 0x401037 is referenced by nothing; f6: padding stripped, ends in a call, 0x40104a the same
    assert fall_addresses(out) == ['0x00401030', '0x00401040']
    assert rc == 1
    assert out[0].startswith('op/fall.c:') and 'ends in a call and nothing refers to 0x00401037' in out[0]
    assert 'nothing refers to 0x0040104a' in out[1]


def test_ret_a_referenced_next_and_padding_before_a_referenced_next_pass(tmp_path, capsys):
    # f1 ends in ret; f2 in a call with its next (0x401017) the target of f4's call; f7 ends in a
    # call, three CC follow and the function after them is a pointer's target; f9's next is a
    # pointer's target directly
    fall_repo(tmp_path, ('f1', 'f2', 'f4', 'f7', 'f9'))
    rc, out = fall_run(tmp_path, capsys)
    assert out == [] and rc == 0


def test_an_exact_claim_that_falls_through_is_not_excused_by_the_baseline(tmp_path, capsys):
    fall_repo(tmp_path, ('f5', 'f6'), matched={'0x00401030': (7, True), '0x00401040': (10, False)},
              baseline=[('falls-through', 'op/fall.c', '0x00401030', 'wrongly baselined'),
                        ('falls-through', 'op/fall.c', '0x00401040', 'known')])
    rc, out = fall_run(tmp_path, capsys)
    assert fall_addresses(out) == ['0x00401030']    # f6 is known, f5 is exact and still fails
    assert 'it is exact, so the baseline cannot excuse it' in out[0]
    assert rc == 1


def test_a_written_baseline_leaves_out_an_exact_fall_through(tmp_path, capsys):
    fall_repo(tmp_path, ('f5', 'f6'), matched={'0x00401030': (7, True), '0x00401040': (10, False)},
              baseline=[])
    rc, _ = fall_run(tmp_path, capsys, '--write-baseline')
    rows = (tmp_path / 'analysis/results/lint_baseline.tsv').read_text().splitlines()
    assert rc == 0 and len(rows) == 2 and rows[1].split('\t')[:3] == [
        'falls-through', 'op/fall.c', '0x00401040']


def test_without_an_executable_the_rule_is_skipped_with_an_info_line(tmp_path, capsys):
    fall_repo(tmp_path, ('f5',), baseline=[('falls-through', 'op/fall.c', '0x00401030', 'known')])
    rc = lint_claims.main(['--repo', str(tmp_path), '--exe', str(tmp_path / 'absent.exe')])
    out = capsys.readouterr().out.splitlines()
    assert out[0].startswith('lint_claims: INFO no executable at ') and 'skipped' in out[0]
    assert not any('falls-through:' in ln for ln in out)    # no break, and the row is not stale
    assert rc == 0


def test_an_executable_that_is_not_the_retail_file_is_refused(tmp_path, capsys):
    fall_repo(tmp_path, ('f5',))
    exe = build_pe(tmp_path / 'image.exe')
    rc = lint_claims.main(['--repo', str(tmp_path), '--exe', str(exe)])
    assert rc == 2 and 'is not the retail file' in capsys.readouterr().err
