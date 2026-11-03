"""Never-regress bookkeeping, annotations, and listing-to-symbol mapping."""
from harness.lib import matched, source

LISTING = """\
; File DynamicAllocator.cpp
?Instance@tDynamicAllocator@@SAPAV1@XZ PROC NEAR\t; tDynamicAllocator::Instance, COMDAT

; 126  : {

\tpush\t-1
; File ..\\Shared\\Source\\Taldren\\Common\\CleanUpGlobalPointer.h
; 40   : \tinline stuff
?Instance@tDynamicAllocator@@SAPAV1@XZ ENDP\t\t; tDynamicAllocator::Instance
; File DynamicAllocator.cpp
??0tDynamicAllocator@@AAE@XZ PROC NEAR\t\t; tDynamicAllocator::tDynamicAllocator, COMDAT

; 138  : {

\tret\t0
??0tDynamicAllocator@@AAE@XZ ENDP
"""


def test_lower_score_than_recorded_is_a_regression_and_equal_is_not():
    data = matched.empty()
    assert matched.record(data, 0x6FC690, dict(name='f', score=97.5, exact=False), '2026-11-01')
    assert matched.is_regression(data, 0x6FC690, 97.0)
    assert not matched.is_regression(data, 0x6FC690, 97.5)
    assert not matched.is_regression(data, 0x401000, 0.0)


def test_record_keeps_the_best_and_its_date():
    data = matched.empty()
    matched.record(data, 0x6FC690, dict(name='f', score=100.0, exact=True), '2026-11-01')
    assert not matched.record(data, 0x6FC690, dict(name='g', score=90.0, exact=False), '2026-11-02')
    assert data['functions']['0x006fc690'] == dict(name='f', score=100.0, exact=True,
                                                   recorded='2026-11-01')


def test_matched_set_round_trips_through_its_file(tmp_path):
    p = str(tmp_path / 'm.json')
    data = matched.empty()
    matched.record(data, 0x10, dict(name='a', score=50.0, exact=False), '2026-11-01')
    matched.save(p, data)
    assert matched.load(p)['functions'] == {'0x00000010': dict(name='a', score=50.0, exact=False,
                                                                recorded='2026-11-01')}


def test_annotation_claims_the_next_function_defined_in_the_file():
    funcs = source.listing_functions(LISTING)
    assert funcs == [('?Instance@tDynamicAllocator@@SAPAV1@XZ', 'DynamicAllocator.cpp', 126),
                     ('??0tDynamicAllocator@@AAE@XZ', 'DynamicAllocator.cpp', 138)]
    assert source.symbol_for_annotation(LISTING, 'x/DynamicAllocator.cpp', 124, [136]) == \
        '?Instance@tDynamicAllocator@@SAPAV1@XZ'
    assert source.symbol_for_annotation(LISTING, 'x/DynamicAllocator.cpp', 136, [124]) == \
        '??0tDynamicAllocator@@AAE@XZ'


def test_annotation_with_another_annotation_before_any_function_claims_nothing():
    assert source.symbol_for_annotation(LISTING, 'x/DynamicAllocator.cpp', 120, [124]) is None


def test_scan_finds_function_and_stub_annotations(tmp_path):
    d = tmp_path / 'Source'
    d.mkdir()
    (d / 'A.cpp').write_text('// FUNCTION: STARFLEETOP 0x006fc690\nint f() { return 0; }\n'
                             '//STUB: STARFLEETOP 0x00401000\nvoid g() {}\n'
                             '// FUNCTION: OTHER 0x00401010\n')
    found = source.scan_annotations(str(tmp_path), 'STARFLEETOP')
    assert sorted(found) == [0x401000, 0x6FC690]
    assert (found[0x6FC690][0].kind, found[0x6FC690][0].line) == ('FUNCTION', 1)
    assert found[0x401000][0].kind == 'STUB'


def test_flags_drop_output_and_browse_options_and_put_includes_first():
    profile = dict(flags='/nologo /O2 /Fr /YX /FD /Fosomewhere/ /c', include_first=['STL'])
    assert source._flags(profile, '/src') == ['/I', '/src/STL', '/nologo', '/O2', '/c']
