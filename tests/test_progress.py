"""Tests for scripts/progress.py on small invented fixtures; no retail data is read.

The fixture is six ordinary functions, one EH funclet and one thunk, laid out so each status and
name tier occurs. Expected values are worked out by hand in the comments.
"""
import importlib.util
import json
import pathlib

import pytest

ROOT = pathlib.Path(__file__).resolve().parents[1]
NOW = '2026-01-02T03:04:05Z'


def _load():
    spec = importlib.util.spec_from_file_location('progress', ROOT / 'scripts/progress.py')
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


progress = _load()

FUNCTIONS_CSV = """\
address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,headers,is_library,library,kind,tags
00401000,100,tShip::mFire,,,sdklib-exact,,,,0,,function,sdklib-exact
00401100,200,tShip::vfunction3,,,rtti-class-recovery,,,,0,,function,
00401300,50,_strlen,,,ghidra-fid-vs,,,,1,crt,function,
00401400,150,FUN_00401400,,,default,,,,0,,function,
00401500,300,FUN_00401500,,,default,,,,0,,function,
00402000,200,tHull::mRepair,,,sdk-fid,,,,0,,function,sdk-fid
00402100,20,Unwind@00402100,,,eh-analyzer,,,,0,,eh-funclet,
00402200,6,thunk_FUN_00401000,,,default,,,,1,import-thunk,thunk,
"""

# address, size, mangled name. The section-only row must not count as a name.
SDK_EXACT_TSV = """\
00401000\t100\t?mFire@tShip@@QAEXXZ
00401000\t100\t?mFire@tShip@@QAEXXZ
00401500\t300\t.\\Release\\x.obj:.text#4
"""

SDK_FUZZY_TSV = """\
address\tsize\tmangled_name\tlibrary\ttier\tscore\tnotes
00401100\t200\t?mTurn@tShip@@QAEXXZ\tShared.lib\texact+ctx\t0.9\t
00401400\t150\t?mWarp@tShip@@QAEXXZ\tShared.lib\tfuzzy\t0.8\t
00401500\t300\t?mDock@tShip@@QAEXXZ\tShared.lib\tmasked\t0.5\t
"""


def write_fixture(tmp, lifted_ranges=((0x401000, 0x401480), (0x402000, 0x4020C8)),
                  errors=(), stubs=(), matched=None, functional=None, funclet_parents=None):
    """Write the fixture into `tmp` and return the paths for progress.compute."""
    (tmp / 'functions.csv').write_text(FUNCTIONS_CSV)
    if funclet_parents is not None:
        (tmp / 'funclets.tsv').write_text('# invented\nfunclet\tparent\n' + ''.join(
            '%08x\t%08x\n' % fp for fp in funclet_parents.items()))
    (tmp / 'exact.tsv').write_text(SDK_EXACT_TSV)
    (tmp / 'fuzzy.tsv').write_text(SDK_FUZZY_TSV)
    lifter = tmp / 'lifter'
    (lifter / 'cat').mkdir(parents=True)
    (lifter / 'out' / 'gen').mkdir(parents=True)
    catalog = {'functions': [{'address': a, 'end': e} for a, e in lifted_ranges]}
    (lifter / 'cat' / 'functions.json').write_text(json.dumps(catalog))
    (lifter / 'out' / 'lift_stats.json').write_text(json.dumps({'not_lifted_stubs': len(stubs)}))
    (lifter / 'out' / 'lift_errors.txt').write_text(
        ''.join('0x%08X\tValueError: bad\n' % a for a in errors))
    (lifter / 'out' / 'gen' / 'recomp_0000.c').write_text(
        ''.join('void sub_%08X(void) { RECOMP_NOT_LIFTED(0x%08Xu); }\n' % (a, a) for a in stubs))
    paths = {
        'work_dir': tmp,
        'functions_csv': tmp / 'functions.csv',
        'sdk_exact_tsv': tmp / 'exact.tsv',
        'sdk_fuzzy_tsv': tmp / 'fuzzy.tsv',
        'eh_funclet_parents': tmp / 'funclets.tsv',
        'lifter_dir': lifter,
        'matched_set': tmp / 'matched_set.json',
        'functional_set': tmp / 'functional_set.json',
        'objdiff_reports': [],
    }
    if matched is not None:
        (tmp / 'matched_set.json').write_text(json.dumps(matched))
    if functional is not None:
        (tmp / 'functional_set.json').write_text(json.dumps(functional))
    return paths


def statuses(report):
    t = report['scopes']['total']['status']
    return {k: (v['functions'], v['bytes']) for k, v in t.items()}


def test_every_ordinary_function_gets_the_best_status_that_applies(tmp_path):
    paths = write_fixture(tmp_path, matched=['00401000', {'address': '0x00401100', 'status': 'functional'}],
                          functional=[0x401400])
    report = progress.compute(paths, NOW, text_bytes=2000)
    # 401000 exact; 401100 and 401400 functional; 401300 and 402000 lifted (inside a range);
    # 401500 starts outside every range, so it is still original x86.
    assert statuses(report) == {
        'exact': (1, 100),
        'functional': (2, 350),
        'lifted': (2, 250),
        'original-x86': (1, 300),
    }
    assert report['exact'] == [{'address': '00401000', 'name': 'tShip::mFire'}]
    assert [e['address'] for e in report['functional']] == ['00401100', '00401400']


def test_funclets_without_a_known_parent_and_thunks_are_counted_but_not_classified(tmp_path):
    report = progress.compute(write_fixture(tmp_path), NOW, text_bytes=2000)
    assert report['scopes']['total']['functions'] == 6
    assert report['scopes']['total']['bytes'] == 1000
    assert report['eh_funclets'] == {'functions': 1, 'bytes': 20, 'with_parent': 0,
                                     'with_parent_bytes': 0, 'without_parent': 1,
                                     'without_parent_bytes': 20}
    assert report['function_kinds'] == {
        'eh-funclet': {'functions': 1, 'bytes': 20},
        'function': {'functions': 6, 'bytes': 1000},
        'thunk': {'functions': 1, 'bytes': 6},
    }


def test_byte_and_count_percentages(tmp_path):
    paths = write_fixture(tmp_path, matched=['00401000'])
    report = progress.compute(paths, NOW, text_bytes=2000)
    exact = report['scopes']['total']['status']['exact']
    assert exact['pct_text'] == 5.0          # 100 of 2000
    assert exact['pct_bytes'] == 10.0        # 100 of 1000
    assert exact['pct_functions'] == 16.6667  # 1 of 6
    lifted = report['scopes']['total']['status']['lifted']
    assert (lifted['bytes'], lifted['pct_text'], lifted['pct_bytes']) == (600, 30.0, 60.0)


def test_game_only_scope_leaves_out_library_functions(tmp_path):
    paths = write_fixture(tmp_path, matched=['00401000'])
    report = progress.compute(paths, NOW, text_bytes=2000)
    game = report['scopes']['game']
    # _strlen (crt, 50 bytes) is out; the import thunk was never an ordinary function.
    assert (game['functions'], game['bytes']) == (5, 950)
    assert game['status']['exact']['pct_bytes'] == 10.5263  # 100 of 950
    assert report['excluded_libraries'] == {'crt': {'functions': 1, 'bytes': 50}}
    assert 'pct_text' not in game['status']['exact']


def test_each_function_gets_the_first_name_tier_that_applies(tmp_path):
    report = progress.compute(write_fixture(tmp_path), NOW)
    named = {k: v['functions'] for k, v in report['scopes']['total']['named'].items()}
    # 401000 sdk-exact; 401100 rtti-class but a trusted fuzzy row (exact+ctx, 0.9) -> sdk-fuzzy;
    # 401300 vs-fid; 401400 fuzzy tier and 401500 low score are leads only -> none;
    # 401500's section-only exact row is not a name; 402000 sdk-fid.
    assert named == {'sdk-exact': 1, 'sdk-fid': 1, 'vs-fid': 1, 'manual': 0, 'sdk-fuzzy': 1, 'string': 0,
                     'rtti-class': 0, 'none': 2}
    assert report['scopes']['total']['named_real_symbol'] == {'functions': 4, 'pct_functions': 66.6667}
    assert report['scopes']['total']['named_with_class'] == {'functions': 4, 'pct_functions': 66.6667}


def test_rtti_class_counts_with_class_but_not_as_a_real_symbol(tmp_path):
    paths = write_fixture(tmp_path)
    paths['sdk_fuzzy_tsv'] = None
    report = progress.compute(paths, NOW)
    total = report['scopes']['total']
    assert total['named']['rtti-class']['functions'] == 1
    assert total['named_real_symbol']['functions'] == 3
    assert total['named_with_class']['functions'] == 4


def test_lift_errors_and_stubs_are_not_lifted(tmp_path):
    paths = write_fixture(tmp_path, lifted_ranges=[(0x401000, 0x401064), (0x401100, 0x4011C8),
                                                   (0x401300, 0x401332), (0x402000, 0x4020C8)],
                          errors=[0x401100], stubs=[0x402000])
    report = progress.compute(paths, NOW)
    assert statuses(report)['lifted'] == (2, 150)  # 401000 and 401300
    assert report['lift']['excluded_errors'] == 1
    assert report['lift']['excluded_stubs'] == 1


def test_nothing_counts_as_lifted_without_a_lift_run(tmp_path):
    paths = write_fixture(tmp_path)
    (paths['lifter_dir'] / 'out' / 'lift_stats.json').unlink()
    report = progress.compute(paths, NOW)
    assert statuses(report)['lifted'] == (0, 0)
    assert statuses(report)['original-x86'] == (6, 1000)


def test_missing_inputs_count_as_zero(tmp_path):
    absent = tmp_path / 'absent'
    paths = {k: absent / k for k in ('functions_csv', 'sdk_exact_tsv', 'sdk_fuzzy_tsv',
                                      'eh_funclet_parents', 'lifter_dir', 'matched_set',
                                      'functional_set')}
    paths['objdiff_reports'] = [absent / 'report.json']
    report = progress.compute(paths, NOW)
    total = report['scopes']['total']
    assert (total['functions'], total['bytes']) == (0, 0)
    assert all(v['pct_text'] == 0.0 for v in total['status'].values())
    assert total['named_real_symbol'] == {'functions': 0, 'pct_functions': 0.0}
    assert all(not v['present'] and v['sha256'] is None for v in report['inputs'].values())
    progress.summary_line(report)  # must not divide by zero


def test_missing_harness_files_leave_exact_and_functional_at_zero(tmp_path):
    report = progress.compute(write_fixture(tmp_path), NOW)
    assert statuses(report)['exact'] == (0, 0)
    assert statuses(report)['functional'] == (0, 0)
    assert report['inputs']['matched_set']['present'] is False


def test_objdiff_report_at_100_percent_counts_as_exact(tmp_path):
    paths = write_fixture(tmp_path)
    report_json = {'units': [{'name': 'ship', 'functions': [
        {'name': 'mFire', 'fuzzy_match_percent': 100.0, 'metadata': {'virtual_address': '4198400'}},
        {'name': 'vfunction3', 'fuzzy_match_percent': 99.5, 'metadata': {'virtual_address': '4198656'}},
        {'name': 'unmatched', 'metadata': {'virtual_address': '4199424'}},
    ]}]}
    (tmp_path / 'objdiff.json').write_text(json.dumps(report_json))
    paths['objdiff_reports'] = [tmp_path / 'objdiff.json']
    report = progress.compute(paths, NOW)
    assert report['exact'] == [{'address': '00401000', 'name': 'tShip::mFire'}]


def test_objdiff_function_without_virtual_address_is_skipped_not_misplaced(tmp_path, capsys):
    # Its "address" is a section offset that happens to equal a real function's address; it must
    # not mark that function exact.
    paths = write_fixture(tmp_path)
    report_json = {'units': [{'name': 'ship', 'functions': [
        {'name': 'mRepair', 'fuzzy_match_percent': 100.0, 'address': '4198400'},
    ]}]}
    (tmp_path / 'objdiff.json').write_text(json.dumps(report_json))
    paths['objdiff_reports'] = [tmp_path / 'objdiff.json']
    report = progress.compute(paths, NOW)
    assert report['exact'] == []
    assert report['objdiff_skipped_without_address'] == 1
    assert 'ship/mRepair has no metadata.virtual_address' in capsys.readouterr().err


def test_funclet_counts_under_its_parent_status_and_scope(tmp_path):
    # Unwind@00402100 (20 bytes) belongs to tHull::mRepair at 00402000 (200 bytes, exact).
    paths = write_fixture(tmp_path, matched=['00402000'], funclet_parents={0x402100: 0x402000})
    report = progress.compute(paths, NOW, text_bytes=2000)
    exact = report['scopes']['total']['status']['exact']
    assert (exact['functions'], exact['function_bytes'], exact['funclet_bytes'], exact['bytes']) == (
        1, 200, 20, 220)
    assert exact['pct_text'] == 11.0     # 220 of 2000
    assert exact['pct_bytes'] == 21.5686  # 220 of 1020
    assert report['scopes']['game']['funclet_bytes'] == 20
    assert report['eh_funclets']['with_parent'] == 1
    assert report['eh_funclets']['without_parent'] == 0


def test_funclet_whose_parent_is_not_an_ordinary_function_stays_unclassified(tmp_path):
    paths = write_fixture(tmp_path, funclet_parents={0x402100: 0x402200})  # 402200 is a thunk
    report = progress.compute(paths, NOW)
    assert report['scopes']['total']['funclet_bytes'] == 0
    assert report['eh_funclets']['without_parent_bytes'] == 20


def test_manual_names_are_a_real_symbol_tier(tmp_path):
    paths = write_fixture(tmp_path)
    csv_text = FUNCTIONS_CSV.replace('00401100,200,tShip::vfunction3,,,rtti-class-recovery',
                                     '00401100,200,tShip::mTurn,,,manual')
    paths['functions_csv'].write_text(csv_text)
    report = progress.compute(paths, NOW)
    named = report['scopes']['total']['named']
    # 401100 also has a trusted fuzzy row; the applied manual name wins.
    assert named['manual']['functions'] == 1
    assert named['sdk-fuzzy']['functions'] == 0
    assert report['scopes']['total']['named_real_symbol']['functions'] == 4
    assert report['unmapped_name_sources'] == {}


def test_matched_set_accepts_an_object_with_status_lists(tmp_path):
    paths = write_fixture(tmp_path, matched={'exact': ['00401000'], 'functional': ['00402000']})
    report = progress.compute(paths, NOW)
    assert statuses(report)['exact'] == (1, 100)
    assert statuses(report)['functional'] == (1, 200)


def test_input_hashes_and_timestamp_are_recorded(tmp_path):
    report = progress.compute(write_fixture(tmp_path), NOW)
    assert report['generated_at'] == NOW
    entry = report['inputs']['functions_csv']
    assert entry['present'] is True
    assert entry['path'] == '$SFCOP_WORK/functions.csv'
    assert len(entry['sha256']) == 64


def run_cli(tmp_path, *extra):
    args = ['--functions', str(tmp_path / 'functions.csv'), '--sdk-exact', str(tmp_path / 'exact.tsv'),
            '--sdk-fuzzy', str(tmp_path / 'fuzzy.tsv'), '--lifter', str(tmp_path / 'lifter'),
            '--funclet-parents', str(tmp_path / 'funclets.tsv'),
            '--matched-set', str(tmp_path / 'matched_set.json'),
            '--functional-set', str(tmp_path / 'functional_set.json'), '--objdiff', str(tmp_path / 'none.json'),
            '--text-bytes', '2000', '--out', str(tmp_path / 'out' / 'progress.json'), '--now', NOW]
    return progress.main(args + list(extra))


def gate_against(tmp_path, previous):
    prev = tmp_path / 'previous.json'
    prev.write_text(json.dumps(previous))
    return run_cli(tmp_path, '--gate', str(prev))


def test_gate_refuses_a_function_that_lost_exact_status(tmp_path, capsys):
    write_fixture(tmp_path, matched=['00401000'])
    previous = {'exact': [{'address': '00401000', 'name': 'tShip::mFire'},
                          {'address': '00402000', 'name': 'tHull::mRepair'}], 'functional': []}
    assert gate_against(tmp_path, previous) == 1
    out = capsys.readouterr().out
    assert 'REGRESSION 00402000: exact -> not replaced' in out
    assert '00401000:' not in out
    assert not (tmp_path / 'out' / 'progress.json').exists()  # the gate writes nothing


def test_gate_refuses_exact_falling_to_functional(tmp_path, capsys):
    write_fixture(tmp_path, matched=[{'address': '00401000', 'status': 'functional'}])
    assert gate_against(tmp_path, {'exact': [{'address': '00401000', 'name': 'x'}]}) == 1
    assert 'REGRESSION 00401000: exact -> functional' in capsys.readouterr().out


def test_gate_compares_by_address_so_a_rename_cannot_hide_a_regression(tmp_path, capsys):
    # The function at 00401100 was exact under another name; the name tShip::mFire is still
    # exact, but at a different address.
    write_fixture(tmp_path, matched=['00401000'])
    previous = {'exact': [{'address': '00401100', 'name': 'tShip::mFire'}]}
    assert gate_against(tmp_path, previous) == 1
    assert 'REGRESSION 00401100: exact -> not replaced' in capsys.readouterr().out


def test_gate_passes_when_nothing_regressed(tmp_path, capsys):
    write_fixture(tmp_path, matched=['00401000', '00402000'])
    previous = {'exact': [{'address': '00401000', 'name': 'old name'}],
                'functional': [{'address': '0x402000', 'name': 'tHull::mRepair'}]}
    assert gate_against(tmp_path, previous) == 0  # functional -> exact is an improvement
    assert 'gate: ok' in capsys.readouterr().out


@pytest.mark.parametrize('previous', [[], {'exact': [{'name': 'no address'}]},
                                      {'exact': ['00401000']}, {'functional': 'x'}])
def test_gate_against_a_malformed_file_is_an_error_not_a_crash(tmp_path, previous):
    write_fixture(tmp_path)
    assert gate_against(tmp_path, previous) == 2


def test_gate_against_a_missing_file_is_an_error(tmp_path):
    write_fixture(tmp_path)
    assert run_cli(tmp_path, '--gate', str(tmp_path / 'nope.json')) == 2


def test_markdown_keeps_history_and_appends_one_line_per_run(tmp_path):
    write_fixture(tmp_path, matched=['00401000'])
    md = tmp_path / 'out' / 'PROGRESS.md'
    assert run_cli(tmp_path, '--markdown', '--markdown-out', str(md)) == 0
    assert run_cli(tmp_path, '--markdown', '--markdown-out', str(md), '--now', '2026-01-03T00:00:00Z') == 0
    text = md.read_text()
    history = text.split('## History', 1)[1].strip().splitlines()
    assert history == [
        '- 2026-01-02T03:04:05Z: exact 5.00%, functional 0.00%, lifted 30.00% of .text; '
        'real symbol 66.67%, with class 66.67% of 6 functions; game-only exact 10.53%, functional 0.00%',
        '- 2026-01-03T00:00:00Z: exact 5.00%, functional 0.00%, lifted 30.00% of .text; '
        'real symbol 66.67%, with class 66.67% of 6 functions; game-only exact 10.53%, functional 0.00%',
    ]
    assert '| exact | 1 | 100 | 0 | 5.00 | 10.00 | 1 | 100 | 10.53 |' in text
    written = json.loads((tmp_path / 'out' / 'progress.json').read_text())
    assert written['generated_at'] == '2026-01-03T00:00:00Z'


@pytest.mark.parametrize('value,expected', [('00401000', 0x401000), ('0x00401000', 0x401000),
                                            (4198400, 0x401000), ('', None), ('zz', None), (True, None)])
def test_hex_addresses_parse_in_every_accepted_form(value, expected):
    assert progress.parse_hex_address(value) == expected


def _synthetic_pe():
    """A two-section PE32 with C++ EH tables for two functions, laid out by hand.

    .text at 0x401000: F (0x401000, 0x20 bytes) pushes handler stub S1 in its prologue; G
    (0x401020, 0x20) loads stub S2 with mov eax before the EH prologue call. Funclets U1-U4 sit at
    0x401080-0x40109f. Stubs at 0x401100 (S1 -> FI1), 0x40110a (S2 -> FI2), 0x401114 (S3 -> FI3,
    referenced by no function). .rdata at 0x402000 holds FI1 (unwind U1, one try block whose catch
    handler is U2), FI2 (unwind U3) and FI3 (unwind U4).
    """
    u32 = lambda v: v.to_bytes(4, 'little')
    text = bytearray(0x200)
    text[0x00:0x0a] = b'\x55\x8b\xec\x6a\xff\x68' + u32(0x401100)
    text[0x20:0x28] = b'\x55\x8b\xec\xb8' + u32(0x40110a)
    for off, fi in ((0x100, 0x402000), (0x10a, 0x402020), (0x114, 0x402040)):
        text[off:off + 10] = b'\xb8' + u32(fi) + b'\xe9' + u32(0)
    rdata = bytearray(0x100)
    magic = 0x19930520
    rdata[0x00:0x14] = b''.join(map(u32, (magic, 1, 0x402060, 1, 0x402080)))
    rdata[0x20:0x34] = b''.join(map(u32, (magic, 1, 0x402068, 0, 0)))
    rdata[0x40:0x54] = b''.join(map(u32, (magic, 1, 0x402070, 0, 0)))
    rdata[0x60:0x68] = u32(0xffffffff) + u32(0x401080)
    rdata[0x68:0x70] = u32(0xffffffff) + u32(0x401090)
    rdata[0x70:0x78] = u32(0xffffffff) + u32(0x401098)
    rdata[0x80:0x94] = b''.join(map(u32, (0, 0, 0, 1, 0x4020a0)))
    rdata[0xa0:0xb0] = b''.join(map(u32, (0, 0, 0, 0x401088)))
    head = bytearray(0x200)
    head[0:2] = b'MZ'
    head[0x3c:0x40] = u32(0x40)
    head[0x40:0x44] = b'PE\0\0'
    head[0x46:0x48] = (2).to_bytes(2, 'little')       # sections
    head[0x54:0x56] = (0xe0).to_bytes(2, 'little')    # optional header size
    head[0x58 + 28:0x58 + 32] = u32(0x400000)          # image base
    for i, (name, va, raw, size) in enumerate(((b'.text', 0x1000, 0x200, 0x200),
                                               (b'.rdata', 0x2000, 0x400, 0x100))):
        o = 0x58 + 0xe0 + 40 * i
        head[o:o + 8] = name.ljust(8, b'\0')
        head[o + 8:o + 24] = u32(size) + u32(va) + u32(size) + u32(raw)
    return bytes(head + text + rdata)


def _synthetic_functions():
    F = progress.Function
    return [F(0x401000, 0x20, 'F', 'default', 'function', False, ''),
            F(0x401020, 0x20, 'G', 'default', 'function', False, ''),
            F(0x401080, 8, 'Unwind@00401080', 'eh-analyzer', 'eh-funclet', False, ''),
            F(0x401088, 8, 'Catch@00401088', 'eh-analyzer', 'eh-funclet', False, ''),
            F(0x401090, 8, 'Unwind@00401090', 'eh-analyzer', 'eh-funclet', False, ''),
            F(0x401098, 8, 'Unwind@00401098', 'eh-analyzer', 'eh-funclet', False, '')]


def test_funclet_map_follows_the_eh_tables_from_both_prologue_forms():
    mapping, stats = progress.build_funclet_map(_synthetic_pe(), _synthetic_functions())
    # U1 and U2 (unwind and catch) belong to F through push; U3 to G through mov eax; U4's stub is
    # referenced by no function, so it has no parent.
    assert mapping == {0x401080: 0x401000, 0x401088: 0x401000, 0x401090: 0x401020}
    assert stats == {'handler_stubs': 3, 'stubs_with_one_parent': 2, 'funclets': 4, 'mapped': 3,
                     'conflicting': 0}


def test_funclet_map_round_trips_through_its_file(tmp_path):
    out = tmp_path / 'map.tsv'
    progress.write_funclet_map(out, {0x401080: 0x401000, 0x401090: 0x401020}, 'ab' * 32)
    loaded = progress.load_funclet_parents(out, progress.InputLog(None))
    assert loaded == {0x401080: 0x401000, 0x401090: 0x401020}
