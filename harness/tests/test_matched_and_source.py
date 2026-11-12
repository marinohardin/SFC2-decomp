"""Never-regress bookkeeping, annotations, and listing-to-symbol mapping."""
import pytest

from harness.lib import deleting, matched, source

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


# A unit whose class tShip has an out-of-class virtual destructor at line 40 and a stand-in
# member at line 50, as VC6 lists it: the destructor has line records, the scalar deleting
# destructor VC6 generates from it has none and calls it; tOther's deleting destructor calls
# another destructor and must not be taken for tShip's.
DELETING_LISTING = """\
\tTITLE\tship.cpp
;\tCOMDAT ??1tShip@@UAE@XZ
_TEXT\tSEGMENT
??1tShip@@UAE@XZ PROC NEAR\t\t\t; tShip::~tShip, COMDAT

; 41   : {

\tret\t0
??1tShip@@UAE@XZ ENDP\t\t\t\t; tShip::~tShip
_TEXT\tENDS
;\tCOMDAT ??_GtOther@@UAEPAXI@Z
_TEXT\tSEGMENT
??_GtOther@@UAEPAXI@Z PROC NEAR\t\t\t; tOther::`scalar deleting destructor', COMDAT
\tcall\t??1tOther@@UAE@XZ\t\t\t; tOther::~tOther
\tret\t4
??_GtOther@@UAEPAXI@Z ENDP
_TEXT\tENDS
;\tCOMDAT ??_GtShip@@UAEPAXI@Z
_TEXT\tSEGMENT
___flags$ = 8
??_GtShip@@UAEPAXI@Z PROC NEAR\t\t\t; tShip::`scalar deleting destructor', COMDAT
\tpush\tesi
\tmov\tesi, ecx
\tcall\t??1tShip@@UAE@XZ\t\t\t; tShip::~tShip
\tret\t4
??_GtShip@@UAEPAXI@Z ENDP
_TEXT\tENDS
;\tCOMDAT ?mStandIn@tShip@@QAEPAV1@I@Z
_TEXT\tSEGMENT
?mStandIn@tShip@@QAEPAV1@I@Z PROC NEAR\t\t; tShip::mStandIn, COMDAT

; 51   : {

\tret\t4
?mStandIn@tShip@@QAEPAV1@I@Z ENDP
_TEXT\tENDS
END
"""


def _deleting(line, lines, kind='scalar'):
    return source.symbol_for_annotation(DELETING_LISTING, 'x/ship.cpp', line,
                                        [n for n in lines if n != line], deleting=kind)


def test_a_deleting_destructor_is_read_from_its_name_or_symbol():
    assert deleting.kind(None, '', 'tShip::scalar_deleting_destructor') == 'scalar'
    assert deleting.kind(None, '??_GtShip@@UAEPAXI@Z', 'FUN_00401000') == 'scalar'
    assert deleting.kind(None, '', "tShip::`vector_deleting_destructor'") == 'vector'
    assert deleting.kind(None, '??_EtShip@@UAEPAXI@Z', '') == 'vector'
    assert deleting.kind(None, '??1tShip@@UAE@XZ', 'tShip::~tShip') is None
    # The matched-set entry is evidence too, its symbol first.
    assert deleting.kind({'symbol': '??_EtShip@@UAEPAXI@Z', 'name': 'FUN_00401000'},
                         '', 'FUN_00401000') == 'vector'
    assert deleting.kind({'symbol': '??1tShip@@UAE@XZ',
                          'name': 'tShip::scalar_deleting_destructor'}, '', 'FUN_00401000') == 'scalar'


def test_the_matched_set_alone_can_say_an_annotation_claims_a_deleting_destructor():
    # functions.csv has only a placeholder name and no mangled name; the matched-set entry
    # records ??_GtShip. The claim is the deleting destructor, as tools/manifest.py names the row,
    # not the destructor the plain rule would give.
    recorded = {'symbol': '??_GtShip@@UAEPAXI@Z', 'name': 'FUN_00401000'}
    assert source.symbol_for_claim(DELETING_LISTING, 'x/ship.cpp', 39, [49], recorded=recorded,
                                   mangled='', name='FUN_00401000') == '??_GtShip@@UAEPAXI@Z'
    assert source.symbol_for_claim(DELETING_LISTING, 'x/ship.cpp', 39, [49], recorded=None,
                                   mangled='', name='FUN_00401000') == '??1tShip@@UAE@XZ'


def test_a_deleting_destructor_stacked_on_the_destructor_claims_the_generated_function():
    # Directly above the definition (line 39), or above the destructor's own annotation (38,
    # with 39 the destructor's): both claim ??_GtShip, not the destructor itself.
    assert _deleting(39, [39, 49]) == '??_GtShip@@UAEPAXI@Z'
    assert _deleting(38, [38, 39, 49]) == '??_GtShip@@UAEPAXI@Z'
    # Without the evidence, the plain rule: the destructor.
    assert source.symbol_for_annotation(DELETING_LISTING, 'x/ship.cpp', 39, [49]) == \
        '??1tShip@@UAE@XZ'


def test_a_deleting_destructor_the_unit_did_not_emit_claims_nothing():
    # tShip has no vector deleting destructor in this unit.
    assert _deleting(39, [39, 49], kind='vector') is None


def test_a_deleting_destructor_claimed_on_a_stand_in_keeps_the_plain_rule():
    assert _deleting(49, [39, 49]) == '?mStandIn@tShip@@QAEPAV1@I@Z'


# VC6 writes the template instances it needs after the unit's own functions with no `; File`
# record before them, so the listing gives them the unit's file. Here the vector destructor's
# first line record (line 4 of the header) lands on the same number as Unit::f's first line.
TRAILING_TEMPLATE_LISTING = """\
\tTITLE\tunit.cpp
_TEXT\tSEGMENT
?f@Unit@@QAEXXZ PROC NEAR\t\t; Unit::f, COMDAT

; 4    : \tfield = 1;

\tret\t0
?f@Unit@@QAEXXZ ENDP\t\t\t; Unit::f
_TEXT\tENDS
;\tCOMDAT ??1?$vector@H@std@@QAE@XZ
_TEXT\tSEGMENT
??1?$vector@H@std@@QAE@XZ PROC NEAR\t; std::vector<int>::~vector<int>, COMDAT

; 4    : \t\t{_Destroy(_First, _Last);

\tret\t0
??1?$vector@H@std@@QAE@XZ ENDP\t\t; std::vector<int>::~vector<int>
_TEXT\tENDS
"""


def test_a_template_instance_after_the_unit_is_told_apart_by_the_line_it_quotes(tmp_path):
    unit = tmp_path / 'unit.cpp'
    unit.write_text('// FUNCTION: STARFLEETOP 0x00401000\nvoid Unit::f()\n{\n\tfield = 1;\n}\n')
    assert source.symbol_for_annotation(TRAILING_TEMPLATE_LISTING, str(unit), 1, []) == \
        '?f@Unit@@QAEXXZ'
    # Without the unit's text to compare with, both functions are still on the line.
    assert source.symbol_for_annotation(TRAILING_TEMPLATE_LISTING, str(tmp_path / 'gone' / 'unit.cpp'),
                                        1, []) == ['?f@Unit@@QAEXXZ', '??1?$vector@H@std@@QAE@XZ']


# Claims in a header. ship.h, written by hand below, declares tShip and its subclass tShipX and
# ends with tShip's destructor written inline, with the claim of its scalar deleting destructor
# on it:
#
#   10  // FUNCTION: STARFLEETOP 0x00401500
#   11  inline tShip::~tShip()
#   12  {
#   13      mClear();
#   14  }
#
# and, at line 6, a part binding in tShip's body with its claim at line 5.
SHIP_H = ('class tShip : public tPart {\n'        # 1
          'public:\n'                             # 2
          '\tvirtual ~tShip();\n'                 # 3
          '\tvirtual void mClear();\n'            # 4
          '\t// FUNCTION: STARFLEETOP 0x00401600\n'  # 5
          '\tGenAbstractBinding( tShip );\n'      # 6
          '};\n'                                  # 7
          'class tShipX : public tShip { public: virtual ~tShipX() {} };\n'  # 8
          '\n'                                    # 9
          '// FUNCTION: STARFLEETOP 0x00401500\n'  # 10
          'inline tShip::~tShip()\n'              # 11
          '{\n'                                   # 12
          '\tmClear();\n'                         # 13
          '}\n')                                  # 14
SHIP_CPP = ('#include "Test/ship.h"\n'
            '\n'
            '// FUNCTION: STARFLEETOP 0x00401000\n'
            'void fn_00401000()\n'
            '{\n'
            '}\n')

# The unit's listing as VC6 writes it when it inlines the destructor everywhere: the `; File`
# record for the header heads the vftable, not code; no ??1tShip is emitted, and the deleting
# destructors have no line records. tShipX's deleting destructor inlines tShip's destructor too.
INLINED_LISTING = """\
\tTITLE\tunit.cpp
; File C:\\game\\.\\Test/ship.h
CONST\tSEGMENT
??_7tShip@@6B@ DD FLAT:??_EtShip@@UAEPAXI@Z
CONST\tENDS
;\tCOMDAT ?fn_00401000@@YAXXZ
_TEXT\tSEGMENT
?fn_00401000@@YAXXZ PROC NEAR\t\t\t; fn_00401000, COMDAT

; 5    : {

\tret\t0
?fn_00401000@@YAXXZ ENDP\t\t\t; fn_00401000
_TEXT\tENDS
;\tCOMDAT ??_GtShipX@@UAEPAXI@Z
_TEXT\tSEGMENT
??_GtShipX@@UAEPAXI@Z PROC NEAR\t\t; tShipX::`scalar deleting destructor', COMDAT
\tcall\t?mClear@tShip@@UAEXXZ\t\t; tShip::mClear
\tret\t4
??_GtShipX@@UAEPAXI@Z ENDP
_TEXT\tENDS
;\tCOMDAT ??_GtShip@@UAEPAXI@Z
_TEXT\tSEGMENT
??_GtShip@@UAEPAXI@Z PROC NEAR\t\t; tShip::`scalar deleting destructor', COMDAT
\tmov\tDWORD PTR [esi], OFFSET FLAT:??_7tShip@@6B@
\tcall\t?mClear@tShip@@UAEXXZ\t\t; tShip::mClear
\tret\t4
??_GtShip@@UAEPAXI@Z ENDP
_TEXT\tENDS
END
"""

# The same with the header's line records inside the deleting destructors: tShip's at 12 and 13,
# and tShipX's, which inlines tShip's destructor, at 8, 12 and 13; and the part binding's two
# members, whose first line record is the binding's line, 6.
RECORDED_LISTING = """\
\tTITLE\tunit.cpp
_TEXT\tSEGMENT
??_GtShipX@@UAEPAXI@Z PROC NEAR\t\t; tShipX::`scalar deleting destructor', COMDAT
; File C:\\game\\.\\Test/ship.h

; 8    : class tShipX : public tShip { public: virtual ~tShipX() {} };

; 12   : {
; 13   : \tmClear();

\tret\t4
??_GtShipX@@UAEPAXI@Z ENDP
_TEXT\tENDS
_TEXT\tSEGMENT
??_GtShip@@UAEPAXI@Z PROC NEAR\t\t; tShip::`scalar deleting destructor', COMDAT
; File C:\\game\\.\\Test/ship.h

; 12   : {
; 13   : \tmClear();

\tret\t4
??_GtShip@@UAEPAXI@Z ENDP
_TEXT\tENDS
_TEXT\tSEGMENT
?Search_tShip@tShip@@SAPAV1@XZ PROC NEAR\t; tShip::Search_tShip, COMDAT
; File C:\\game\\.\\Test/ship.h

; 6    : \tGenAbstractBinding( tShip );

\tret\t0
?Search_tShip@tShip@@SAPAV1@XZ ENDP
_TEXT\tENDS
_TEXT\tSEGMENT
?Test_tShip@tShip@@SAEXXZ PROC NEAR\t; tShip::Test_tShip, COMDAT

; 6    : \tGenAbstractBinding( tShip );

\tret\t0
?Test_tShip@tShip@@SAEXXZ ENDP
_TEXT\tENDS
END
"""


def _ship_tree(tmp_path, units='Test/unit.cpp\t0x00401000\t0x004016ff\n', cpp=SHIP_CPP):
    game = tmp_path / 'game'
    (game / 'Test').mkdir(parents=True)
    (game / 'Test' / 'ship.h').write_text(SHIP_H)
    (game / 'Test' / 'unit.cpp').write_text(cpp)
    (game / 'UNITS.tsv').write_text('unit\tfirst\tlast\n' + units)
    return game


def test_the_listing_gives_every_line_record_of_each_proc_with_its_file():
    assert source.listing_line_records(RECORDED_LISTING) == [
        ('??_GtShipX@@UAEPAXI@Z', [('C:\\game\\.\\Test/ship.h', 8), ('C:\\game\\.\\Test/ship.h', 12),
                                   ('C:\\game\\.\\Test/ship.h', 13)]),
        ('??_GtShip@@UAEPAXI@Z', [('C:\\game\\.\\Test/ship.h', 12), ('C:\\game\\.\\Test/ship.h', 13)]),
        ('?Search_tShip@tShip@@SAPAV1@XZ', [('C:\\game\\.\\Test/ship.h', 6)]),
        ('?Test_tShip@tShip@@SAEXXZ', [('C:\\game\\.\\Test/ship.h', 6)])]
    assert source.listing_line_records(INLINED_LISTING) == [
        ('?fn_00401000@@YAXXZ', [('unit.cpp', 5)]),
        ('??_GtShipX@@UAEPAXI@Z', []),
        ('??_GtShip@@UAEPAXI@Z', [])]


def test_a_deleting_destructor_on_an_inline_destructor_is_found_by_its_class(tmp_path):
    header = str(_ship_tree(tmp_path) / 'Test' / 'ship.h')
    assert source.symbol_for_annotation(INLINED_LISTING, header, 10, [5],
                                        deleting='scalar') == '??_GtShip@@UAEPAXI@Z'
    # Not emitted in this unit: nothing.
    assert source.symbol_for_annotation(INLINED_LISTING, header, 10, [5], deleting='vector') is None


def test_a_deleting_destructor_on_an_inline_destructor_is_found_by_its_line_records(tmp_path):
    header = str(_ship_tree(tmp_path) / 'Test' / 'ship.h')
    # Both deleting destructors have records at 12 and 13; tShip's is the one its class names.
    assert source.symbol_for_annotation(RECORDED_LISTING, header, 10, [5],
                                        deleting='scalar') == '??_GtShip@@UAEPAXI@Z'
    # With no deleting destructor named for tShip, neither is taken.
    assert source.symbol_for_annotation(RECORDED_LISTING.replace('??_GtShip@', '??_GtRenamed@'),
                                        header, 10, [5], deleting='scalar') is None
    # When only one has records between the claim and the next annotation, the records decide:
    # here tShipX's keeps only line 8, before the claim.
    one = RECORDED_LISTING.replace("; 8    : class tShipX : public tShip { public: virtual ~tShipX() {} };\n\n"
                                   "; 12   : {\n; 13   : \tmClear();\n",
                                   "; 8    : class tShipX : public tShip { public: virtual ~tShipX() {} };\n")
    assert one != RECORDED_LISTING
    assert source.symbol_for_annotation(one.replace('??_GtShip@', '??_GtRenamed@'), header, 10, [5],
                                        deleting='scalar') == '??_GtRenamed@@UAEPAXI@Z'


def test_a_deleting_destructor_on_an_inline_destructor_emitted_out_of_line(tmp_path):
    # A unit that emits tShip::~tShip itself (from the header, line records 12 to 14): the
    # deleting destructor that calls it, as for an out-of-class destructor in a unit.
    header = str(_ship_tree(tmp_path) / 'Test' / 'ship.h')
    listing = ('\tTITLE\tunit.cpp\n'
               '_TEXT\tSEGMENT\n'
               '??1tShip@@UAE@XZ PROC NEAR\t\t; tShip::~tShip, COMDAT\n'
               '; File C:\\game\\.\\Test/ship.h\n\n'
               '; 12   : {\n\n'
               '\tret\t0\n'
               '??1tShip@@UAE@XZ ENDP\n'
               '_TEXT\tENDS\n'
               '_TEXT\tSEGMENT\n'
               '??_GtShip@@UAEPAXI@Z PROC NEAR\t\t; tShip::`scalar deleting destructor\', COMDAT\n'
               '\tcall\t??1tShip@@UAE@XZ\t\t; tShip::~tShip\n'
               '\tret\t4\n'
               '??_GtShip@@UAEPAXI@Z ENDP\n'
               '_TEXT\tENDS\n'
               'END\n')
    assert source.symbol_for_annotation(listing, header, 10, [5], deleting='scalar') == \
        '??_GtShip@@UAEPAXI@Z'
    # Without the evidence that it is a deleting destructor, the plain rule: the destructor.
    assert source.symbol_for_annotation(listing, header, 10, [5]) == '??1tShip@@UAE@XZ'


def test_a_part_binding_in_a_header_claims_by_the_plain_rule(tmp_path):
    header = str(_ship_tree(tmp_path) / 'Test' / 'ship.h')
    # Both members the binding defines start on its line; the harness picks the mangled name.
    assert source.symbol_for_annotation(RECORDED_LISTING, header, 5, [10]) == \
        ['?Search_tShip@tShip@@SAPAV1@XZ', '?Test_tShip@tShip@@SAEXXZ']
    # The unit's own lines are not the header's: the unit's annotation at line 3 claims its own.
    unit = str(tmp_path / 'game' / 'Test' / 'unit.cpp')
    assert source.symbol_for_annotation(INLINED_LISTING, unit, 3, []) == '?fn_00401000@@YAXXZ'


def test_scan_reads_headers_and_a_header_claim_is_compiled_in_its_units_unit(tmp_path):
    game = _ship_tree(tmp_path)
    anns = source.scan_annotations(str(game), 'STARFLEETOP')
    assert sorted((hex(a), [(x.path.rsplit('/', 1)[1], x.line) for x in v]) for a, v in anns.items()) == [
        ('0x401000', [('unit.cpp', 3)]), ('0x401500', [('ship.h', 10)]), ('0x401600', [('ship.h', 5)])]
    unit = str(game / 'Test' / 'unit.cpp')
    assert source.claim_unit(anns[0x401500][0], str(game)) == unit
    assert source.claim_unit(anns[0x401600][0], str(game)) == unit
    assert source.claim_unit(anns[0x401000][0], str(game)) == unit


@pytest.mark.parametrize('units,cpp,message', [
    ('Test/unit.cpp\t0x00401000\t0x004014ff\n', SHIP_CPP,
     '0x00401500 is claimed in the header Test/ship.h, but no row of game/UNITS.tsv holds that '
     'address'),
    ('Test/gone.cpp\t0x00401000\t0x004016ff\n', SHIP_CPP,
     'game/UNITS.tsv assigns it to Test/gone.cpp, which does not exist'),
    ('Test/unit.cpp\t0x00401000\t0x004016ff\n', SHIP_CPP.replace('"Test/ship.h"', '<string.h>'),
     'game/UNITS.tsv assigns it to Test/unit.cpp, which does not include that header'),
])
def test_a_header_claim_without_a_unit_that_includes_it_is_an_error(tmp_path, units, cpp, message):
    game = _ship_tree(tmp_path, units=units, cpp=cpp)
    ann = source.scan_annotations(str(game), 'STARFLEETOP')[0x401500][0]
    with pytest.raises(source.SourceError, match=message.replace('(', r'\(')):
        source.claim_unit(ann, str(game))


def test_a_header_reached_through_another_header_counts_as_included(tmp_path):
    game = _ship_tree(tmp_path, cpp=SHIP_CPP.replace('"Test/ship.h"', '"fleet.h"'))
    # fleet.h, beside the unit, includes the header by its path from the source root.
    (game / 'Test' / 'fleet.h').write_text('#include <windows.h>\n#include "Test/ship.h"\n')
    ann = source.scan_annotations(str(game), 'STARFLEETOP')[0x401500][0]
    assert source.claim_unit(ann, str(game)) == str(game / 'Test' / 'unit.cpp')


# The part binding's member as VC6 lists it when it emits the header's inline code after the
# unit's own: no `; File` record before it (the header's record heads a data segment), so the
# listing gives it the unit's file. Its first line record quotes the header's line 6.
TRAILING_BINDING_LISTING = """\
\tTITLE\tunit.cpp
_TEXT\tSEGMENT
?fn_00401000@@YAXXZ PROC NEAR\t\t\t; fn_00401000, COMDAT

; 5    : {

\tret\t0
?fn_00401000@@YAXXZ ENDP\t\t\t; fn_00401000
_TEXT\tENDS
; File C:\\game\\.\\Test/ship.h
_BSS\tSEGMENT
?partCachePtr@?1??Search_tShip@tShip@@SAPAV2@XZ@4PAV2@A DD 01H DUP (?)
_BSS\tENDS
_TEXT\tSEGMENT
?Search_tShip@tShip@@SAPAV1@XZ PROC NEAR\t; tShip::Search_tShip, COMDAT

; 6    : \tGenAbstractBinding( tShip );

\tret\t0
?Search_tShip@tShip@@SAPAV1@XZ ENDP
_TEXT\tENDS
END
"""


def test_a_header_function_listed_under_the_units_file_is_told_apart_by_its_text(tmp_path):
    game = _ship_tree(tmp_path)
    header, unit = str(game / 'Test' / 'ship.h'), str(game / 'Test' / 'unit.cpp')
    assert source.listing_functions(TRAILING_BINDING_LISTING)[1] == \
        ('?Search_tShip@tShip@@SAPAV1@XZ', 'unit.cpp', 6)
    assert source.symbol_for_annotation(TRAILING_BINDING_LISTING, header, 5, [10], unit=unit) == \
        '?Search_tShip@tShip@@SAPAV1@XZ'
    # Without the unit to compare with, only the header's own records count.
    assert source.symbol_for_annotation(TRAILING_BINDING_LISTING, header, 5, [10]) is None
    # The unit's own claim is unaffected.
    assert source.symbol_for_annotation(TRAILING_BINDING_LISTING, unit, 3, [], unit=unit) == \
        '?fn_00401000@@YAXXZ'
    # When the unit has the same text at line 6, the function may be the unit's: not taken.
    (game / 'Test' / 'unit.cpp').write_text('#include "Test/ship.h"\n\n\n\n\n'
                                            '\tGenAbstractBinding( tShip );\n')
    assert source.symbol_for_annotation(TRAILING_BINDING_LISTING, header, 5, [10], unit=unit) is None
