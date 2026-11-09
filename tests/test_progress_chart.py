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
        'unsorted/fn_00402000.cpp\t0x00402000\t0x0040203f\t3\tpage\tsdk-release\t\n')
    (tmp_path / 'functions.csv').write_text(
        'address,size,name,kind,is_library\n'
        '00401000,16,A,function,0\n'
        '00401010,16,B,function,0\n'
        '00402000,40,C,function,0\n'
        '00402030,10,D,function,0\n'
        '00402038,8,E,function,1\n'      # library code: left out
        '0040203a,4,F,eh-funclet,0\n'    # not an ordinary function: left out
        '00403000,8,G,function,0\n')     # outside every unit: left out


def test_units_are_sized_by_game_code_and_shaded_by_matched_bytes(tmp_path):
    _write_inputs(tmp_path)
    # A exact through progress.json, B exact through the matched set; C half matched.
    matched = {'functions': {'0x00401010': {'score': 100.0, 'exact': True},
                             '0x00402000': {'score': 50.0, 'exact': False}}}
    units = pc.read_units(str(tmp_path / 'functions.csv'), str(tmp_path / 'UNITS.tsv'),
                          _progress(exact=['0x00401000']), matched)
    assert sorted(units) == [
        ('Tiger', 'Tiger/ta.cpp', 32, 32.0, True),
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
            '--matched-set', str(tmp_path / 'matched.json')]
    names = ['badge-exact.svg', 'badge-functional.svg', 'badge-named.svg', 'treemap.svg']
    pc.main(args + ['--out', str(tmp_path / 'a')])
    pc.main(args + ['--out', str(tmp_path / 'b')])
    for n in names:
        assert (tmp_path / 'a' / n).read_bytes() == (tmp_path / 'b' / n).read_bytes()
    tm = (tmp_path / 'a' / 'treemap.svg').read_text()
    assert tm.startswith('<svg') and tm.rstrip().endswith('</svg>')
    assert 'Tiger/ta.cpp: 50.0% of 32 bytes' in tm
