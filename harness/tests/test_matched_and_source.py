"""Never-regress bookkeeping, annotations, and listing-to-symbol mapping."""
import pytest

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


VFTABLE_LISTING = """\
\tTITLE\ttEntity.cpp
\t.386P
;\tCOMDAT ??_7tEntity@@6B@
; File ..\\unsorted/tEntity.h
CONST\tSEGMENT
??_7tEntity@@6B@ DD FLAT:??_R4tEntity@@6B@\t\t; tEntity::`vftable'
CONST\tENDS
;\tCOMDAT ??0tEntity@@QAE@XZ
_TEXT\tSEGMENT
??0tEntity@@QAE@XZ PROC NEAR\t\t\t\t; tEntity::tEntity, COMDAT

; 43   : {

\tret\t0
??0tEntity@@QAE@XZ ENDP
;\tCOMDAT ?f@tEntity@@QAEXXZ
; File ..\\unsorted/tEntity.h
_TEXT\tSEGMENT
?f@tEntity@@QAEXXZ PROC NEAR

; 12   : \tinline void f() {}

\tret\t0
?f@tEntity@@QAEXXZ ENDP
"""


def test_a_file_record_before_a_vftable_does_not_move_the_code_after_it():
    # VC6 names the header that declares a class before its vftable and does not name the unit
    # again before the code that follows; a record heading a code segment still applies.
    assert source.listing_functions(VFTABLE_LISTING) == [
        ('??0tEntity@@QAE@XZ', 'tEntity.cpp', 43),
        ('?f@tEntity@@QAEXXZ', '..\\unsorted/tEntity.h', 12)]


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


# Our own compiler's /FAs listing (game-release flags, trimmed to the code) of this source, whose
# annotations are on lines 14, 17, 20, 23-25 and 28-29:
#
#   struct tCounter
#   {
#       int m_n;
#       tCounter();
#       ~tCounter();
#   };
#
#   struct tPlain
#   {
#       int m_n;
#       tPlain();
#   };
#
#   // FUNCTION: STARFLEETOP 0x00401000
#   tCounter::tCounter() : m_n(0) {}
#
#   // FUNCTION: STARFLEETOP 0x00401010
#   tCounter::~tCounter() { m_n = 0; }
#
#   // FUNCTION: STARFLEETOP 0x00401020
#   tPlain::tPlain() : m_n(1) {}
#
#   // FUNCTION: STARFLEETOP 0x00401050
#   // FUNCTION: STARFLEETOP 0x00401040
#   // FUNCTION: STARFLEETOP 0x00401030
#   tCounter g_tCounter;
#
#   // FUNCTION: STARFLEETOP 0x00401070
#   // FUNCTION: STARFLEETOP 0x00401060
#   tPlain g_tPlain;
GLOBALS_LISTING = """\
\tTITLE\tglobals.cpp
CRT$XCU\tSEGMENT
_$S5\tDD\tFLAT:_$E4
_$S8\tDD\tFLAT:_$E7
CRT$XCU\tENDS
;\tCOMDAT ??0tCounter@@QAE@XZ
_TEXT\tSEGMENT
??0tCounter@@QAE@XZ PROC NEAR\t\t\t\t; tCounter::tCounter, COMDAT

; 15   : tCounter::tCounter() : m_n(0) {}

\tmov\teax, ecx
\tand\tDWORD PTR [eax], 0
\tret\t0
??0tCounter@@QAE@XZ ENDP\t\t\t\t; tCounter::tCounter
_TEXT\tENDS
;\tCOMDAT ??1tCounter@@QAE@XZ
_TEXT\tSEGMENT
??1tCounter@@QAE@XZ PROC NEAR\t\t\t\t; tCounter::~tCounter, COMDAT

; 18   : tCounter::~tCounter() { m_n = 0; }

\tand\tDWORD PTR [ecx], 0
\tret\t0
??1tCounter@@QAE@XZ ENDP\t\t\t\t; tCounter::~tCounter
_TEXT\tENDS
;\tCOMDAT ??0tPlain@@QAE@XZ
_TEXT\tSEGMENT
??0tPlain@@QAE@XZ PROC NEAR\t\t\t\t; tPlain::tPlain, COMDAT

; 21   : tPlain::tPlain() : m_n(1) {}

\tmov\teax, ecx
\tmov\tDWORD PTR [eax], 1
\tret\t0
??0tPlain@@QAE@XZ ENDP\t\t\t\t\t; tPlain::tPlain
_TEXT\tENDS
;\tCOMDAT _$E4
_TEXT\tSEGMENT
_$E4\tPROC NEAR\t\t\t\t\t; COMDAT
\tcall\t_$E1
\tjmp\t_$E3
_$E4\tENDP
_TEXT\tENDS
;\tCOMDAT _$E1
_TEXT\tSEGMENT
_$E1\tPROC NEAR\t\t\t\t\t; COMDAT

; 26   : tCounter g_tCounter;

\tmov\tecx, OFFSET FLAT:?g_tCounter@@3UtCounter@@A ; g_tCounter
\tjmp\t??0tCounter@@QAE@XZ\t\t\t; tCounter::tCounter
_$E1\tENDP
_TEXT\tENDS
;\tCOMDAT _$E3
_TEXT\tSEGMENT
_$E3\tPROC NEAR\t\t\t\t\t; COMDAT
\tpush\tOFFSET FLAT:_$E2
\tcall\t_atexit
\tpop\tecx
\tret\t0
_$E3\tENDP
_TEXT\tENDS
;\tCOMDAT _$E2
_TEXT\tSEGMENT
_$E2\tPROC NEAR\t\t\t\t\t; COMDAT
\tmov\tecx, OFFSET FLAT:?g_tCounter@@3UtCounter@@A ; g_tCounter
\tjmp\t??1tCounter@@QAE@XZ\t\t\t; tCounter::~tCounter
_$E2\tENDP
_TEXT\tENDS
;\tCOMDAT _$E7
_TEXT\tSEGMENT
_$E7\tPROC NEAR\t\t\t\t\t; COMDAT
\tjmp\t_$E6
_$E7\tENDP
_TEXT\tENDS
;\tCOMDAT _$E6
_TEXT\tSEGMENT
_$E6\tPROC NEAR\t\t\t\t\t; COMDAT

; 30   : tPlain g_tPlain;

\tmov\tecx, OFFSET FLAT:?g_tPlain@@3UtPlain@@A\t; g_tPlain
\tjmp\t??0tPlain@@QAE@XZ\t\t\t; tPlain::tPlain
_$E6\tENDP
_TEXT\tENDS
END
"""
GLOBALS_ANNOTATIONS = [14, 17, 20, 23, 24, 25, 28, 29]


def _claim(line, listing=GLOBALS_LISTING, lines=GLOBALS_ANNOTATIONS):
    return source.symbol_for_annotation(listing, 'x/globals.cpp', line,
                                        [n for n in lines if n != line])


def test_annotations_stacked_above_a_global_claim_its_generated_functions_upwards():
    assert _claim(25) == '_$E1'
    assert _claim(24) == '_$E3'
    assert _claim(23) == '_$E2'


def test_a_global_with_only_a_constructor_has_only_an_initializer_to_claim():
    assert _claim(29) == '_$E6'
    with pytest.raises(source.SourceError, match='registers no destructor'):
        _claim(28)


def test_a_fourth_annotation_above_a_global_is_reported():
    with pytest.raises(source.SourceError, match='at most 3'):
        _claim(22, lines=GLOBALS_ANNOTATIONS + [22])


def test_annotations_above_functions_keep_the_nearest_function_rule():
    assert _claim(14) == '??0tCounter@@QAE@XZ'
    assert _claim(17) == '??1tCounter@@QAE@XZ'
    assert _claim(20) == '??0tPlain@@QAE@XZ'
    # An annotation stacked above a function's annotation still claims nothing.
    assert _claim(13, lines=GLOBALS_ANNOTATIONS + [13]) is None


def test_generated_functions_are_read_from_the_startup_table_and_atexit_call():
    assert source.global_generated_functions(GLOBALS_LISTING, '_$E1') == ['_$E1', '_$E3', '_$E2']
    assert source.global_generated_functions(GLOBALS_LISTING, '_$E6') == ['_$E6']
    assert source.global_generated_functions(GLOBALS_LISTING, '??0tPlain@@QAE@XZ') is None
