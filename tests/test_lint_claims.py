"""Tests for tools/lint_claims.py.

Each test builds an invented op/ tree and its evidence (matched set, boundary list, functions.csv,
baseline) under pytest's tmp_path; the expected breaks are written out by hand.
"""
import importlib.util
import json
import pathlib

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
    rc = lint_claims.main(['--repo', str(repo), *extra])
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
