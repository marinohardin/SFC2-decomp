"""Tests for tools/progress_chart.py: the README's progress badges and treemap.

Fixtures are invented and small; expected values are worked out by hand in the comments.
"""
import importlib.util
import json
import pathlib

import pytest

ROOT = pathlib.Path(__file__).resolve().parents[1]


def _load():
    spec = importlib.util.spec_from_file_location('progress_chart', ROOT / 'tools/progress_chart.py')
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


pc = _load()


def _progress(exact_pct=1.25, functional_pct=0.5, named_pct=7.5, exact=()):
    st = {'exact': {'pct_bytes': exact_pct}, 'functional': {'pct_bytes': functional_pct}}
    return {'scopes': {'game': {'status': st, 'named_real_symbol': {'pct_functions': named_pct}}},
            'exact': list(exact), 'functional': []}


def test_badges_are_the_game_scope_figures_with_functional_including_exact():
    assert pc.badge_values(_progress()) == (1.25, 1.75, 7.5)


def test_badge_shows_label_and_value_and_fits_the_value():
    svg = pc.badge_svg('Exact', '12.34%')
    assert 'aria-label="Exact: 12.34%"' in svg
    assert '>Exact</text>' in svg and '>12.34%</text>' in svg
    # "12.34%" is 4 digits at 7, '.' at 3.9 and '%' at 10 = 41.9, plus 12 padding -> 54.
    assert '<rect x="' in svg and 'width="54"' in svg


def test_squarify_fills_the_box_exactly_with_areas_in_proportion():
    rects = pc.squarify([6, 6, 4, 3, 2, 2, 1], 0, 0, 6, 4)
    assert len(rects) == 7
    # Total area 24 over a 6x4 box: each rectangle's area equals its value.
    for v, (x, y, w, h) in zip([6, 6, 4, 3, 2, 2, 1], rects):
        assert w * h == pytest.approx(v)
        assert x >= -1e-9 and y >= -1e-9 and x + w <= 6 + 1e-9 and y + h <= 4 + 1e-9
    assert sum(w * h for _, _, w, h in rects) == pytest.approx(24)


def test_squarify_of_one_value_is_the_whole_box():
    assert pc.squarify([5], 10, 20, 30, 40) == [(10, 20, 30, 40)]


def test_color_runs_from_dark_to_partial_green_and_bright_when_complete():
    assert pc.color(0.0, False) == '#333633'
    assert pc.color(1.0, False) == '#3d8f3d'
    assert pc.color(0.5, True) == '#00d000'
    assert pc.color(-1, False) == '#333633' and pc.color(2, False) == '#3d8f3d'


def _write_inputs(tmp_path):
    (tmp_path / 'UNITS.tsv').write_text(
        'unit\tfirst\tlast\tfunctions\tevidence\tprofile\theader\n'
        'Tiger/ta.cpp\t0x00401000\t0x0040101f\t2\tassert-path\tsdk-release\t\n'
        'unsorted/fn_00402000.cpp\t0x00402000\t0x0040203f\t3\tpage\tsdk-release\t\n'
        'unsorted/fn_00404000.cpp\t0x00404000\t0x0040400f\t1\tpage\tsdk-release\t\n')
    (tmp_path / 'functions.csv').write_text(
        'address,size,name,kind,is_library\n'
        '00401000,16,A,function,0\n'
        '00401010,16,B,function,0\n'
        '00402000,40,C,function,0\n'
        '00402030,10,D,function,0\n'
        '00402038,8,E,function,1\n'      # library code: left out
        '0040203a,4,F,eh-funclet,0\n'    # not an ordinary function: left out
        '00403000,8,G,function,0\n'      # in the gap between two units: left out
        '00405000,8,H,function,0\n'      # after the last unit: left out
        '00405100,6,I,eh-funclet,0\n'    # funclet of A: counted with A
        '00405200,5,J,eh-funclet,0\n')   # funclet of H, which is outside every unit: left out
    (tmp_path / 'funclets.tsv').write_text(
        '# EH funclet -> parent function\n'
        'funclet\tparent\n'
        '00405100\t00401000\n'
        '00405200\t00405000\n')


def test_units_are_sized_by_game_code_and_shaded_by_matched_bytes(tmp_path):
    _write_inputs(tmp_path)
    # A exact through progress.json, B exact through the matched set; C half matched.
    matched = {'functions': {'0x00401010': {'score': 100.0, 'exact': True},
                             '0x00402000': {'score': 50.0, 'exact': False}}}
    units = pc.read_units(str(tmp_path / 'functions.csv'), str(tmp_path / 'UNITS.tsv'),
                          _progress(exact=['0x00401000']), matched,
                          pc.read_funclet_parents(str(tmp_path / 'funclets.tsv')))
    assert sorted(units) == [
        # A 16 + its funclet I 6, and B 16: 38 bytes, all exact.
        ('Tiger', 'Tiger/ta.cpp', 38, 38.0, True),
        # C 40 bytes at 50% = 20 of 50 bytes (C and D).
        ('unsorted', 'unsorted/fn_00402000.cpp', 50, 20.0, False),
    ]


def test_groups_are_the_top_directory_or_two_levels_under_source_and_meta():
    assert pc.group_of('Tiger/tsay.cpp') == 'Tiger'
    assert pc.group_of('Source/entity/entity_physical.cpp') == 'Source/entity'
    assert pc.group_of('Meta/Servers/x/y.cpp') == 'Meta/Servers'
    assert pc.group_of('top.cpp') == '(root)'


def test_running_twice_writes_the_same_files(tmp_path):
    _write_inputs(tmp_path)
    (tmp_path / 'progress.json').write_text(json.dumps(_progress(exact=['0x00401000'])))
    (tmp_path / 'matched.json').write_text('{"functions": {}}')
    args = ['--progress', str(tmp_path / 'progress.json'), '--functions',
            str(tmp_path / 'functions.csv'), '--units', str(tmp_path / 'UNITS.tsv'),
            '--matched-set', str(tmp_path / 'matched.json'),
            '--funclets', str(tmp_path / 'funclets.tsv')]
    names = ['badge-exact.svg', 'badge-functional.svg', 'badge-named.svg', 'treemap.svg']
    pc.main(args + ['--out', str(tmp_path / 'a')])
    pc.main(args + ['--out', str(tmp_path / 'b')])
    for n in names:
        assert (tmp_path / 'a' / n).read_bytes() == (tmp_path / 'b' / n).read_bytes()
    tm = (tmp_path / 'a' / 'treemap.svg').read_text()
    assert tm.startswith('<svg') and tm.rstrip().endswith('</svg>')
    # A and its funclet (22 bytes) are exact, B (16) is not: 22 / 38 = 57.9%.
    assert 'Tiger/ta.cpp: 57.9% of 38 bytes' in tm


def test_a_complete_unit_is_drawn_bright_green():
    tm = pc.treemap_svg([('Tiger', 'Tiger/ta.cpp', 10, 10.0, True),
                         ('Tiger', 'Tiger/tb.cpp', 10, 5.0, False)])
    assert 'fill="#00d000"><title>Tiger/ta.cpp: 100.0% of 10 bytes' in tm
    assert 'fill="#00d000"><title>Tiger/tb.cpp' not in tm


def test_a_missing_matched_set_or_funclet_map_still_writes_the_files(tmp_path):
    _write_inputs(tmp_path)
    (tmp_path / 'progress.json').write_text(json.dumps(_progress()))
    pc.main(['--progress', str(tmp_path / 'progress.json'), '--functions',
             str(tmp_path / 'functions.csv'), '--units', str(tmp_path / 'UNITS.tsv'),
             '--matched-set', str(tmp_path / 'absent.json'),
             '--funclets', str(tmp_path / 'absent.tsv'), '--out', str(tmp_path / 'o')])
    # Nothing exact and no funclets: A and B are 32 bytes, none matched.
    assert 'Tiger/ta.cpp: 0.0% of 32 bytes' in (tmp_path / 'o' / 'treemap.svg').read_text()


def test_badge_text_is_escaped():
    svg = pc.badge_svg('A<B', '1&2')
    assert '>A&lt;B</text>' in svg and '>1&amp;2</text>' in svg and '<B' not in svg


def test_warns_when_the_matched_set_is_ahead_of_progress_or_units_miss_game_code():
    progress = _progress(exact=['0x00401000'])
    progress['scopes']['game']['bytes'] = 100
    matched = {'functions': {'0x00401000': {'score': 100.0, 'exact': True},
                             '0x00401010': {'score': 100.0, 'exact': True}}}
    w = pc.warnings(progress, matched, 90)
    assert len(w) == 2
    assert w[0].startswith('1 exact function(s) in the matched set')
    assert '90 bytes of the 100 bytes' in w[1]
    assert pc.warnings(progress, {'functions': {}}, 100) == []


def test_not_started_writes_three_badges_and_no_treemap(tmp_path):
    assert pc.main(['--not-started', '--out', str(tmp_path)]) == 0
    assert sorted(p.name for p in tmp_path.iterdir()) == [
        'badge-exact.svg', 'badge-functional.svg', 'badge-named.svg']
    assert 'Exact: not started' in (tmp_path / 'badge-exact.svg').read_text()
