"""A claim is found by the symbol the committed symbol table records for it; the listing's line
records are the fallback for a claim the table has no current symbol for. Invented listings and
sources; expected values written out by hand."""
from types import SimpleNamespace

from harness.lib import source, verify

A = '?FnA@@YAXXZ'
B = '?FnB@@YAXXZ'

# FnA is annotated at line 10 and FnB at line 20, but the compiler saw FnA's body at line 21 and
# FnB's at line 11: the functions swapped places after the annotations were written.
SWAPPED = """\
; File u.cpp
?FnA@@YAXXZ PROC NEAR\t; FnA

; 21   : {

\tret\t0
?FnA@@YAXXZ ENDP
?FnB@@YAXXZ PROC NEAR\t; FnB

; 11   : {

\tret\t0
?FnB@@YAXXZ ENDP
"""

# Two functions whose first line record is the same line (an inline function opening on a bare
# `{` that a macro or a header expands twice): the line rule cannot tell them apart.
SAME_LINE = """\
; File u.cpp
?FnA@@YAXXZ PROC NEAR

; 11   : {

\tret\t0
?FnA@@YAXXZ ENDP
?FnB@@YAXXZ PROC NEAR

; 11   : {

\tret\t0
?FnB@@YAXXZ ENDP
"""


def obj(*names):
    return SimpleNamespace(by_name={n: SimpleNamespace(section=1) for n in names})


def fn(mangled=''):
    return SimpleNamespace(mangled=mangled, name='f')


def unit(line, others):
    return dict(path='op/u.cpp', file='op/u.cpp', line=line, other_lines=others)


def test_moved_lines_attach_the_claim_to_the_wrong_function_by_line_and_the_right_one_by_symbol():
    o = obj(A, B)
    # by line, as before the table: FnA's claim (line 10) reaches FnB, which starts at line 11
    assert verify.claim_symbol(fn(), unit(10, [20]), o, SWAPPED) == (B, 'line')
    # by symbol: the table says what the claim compiled
    assert verify.claim_symbol(fn(), unit(10, [20]), o, SWAPPED, table_symbol=A) == (A, 'symbol')
    assert verify.claim_symbol(fn(), unit(20, [10]), o, SWAPPED, table_symbol=B) == (B, 'symbol')


def test_functions_on_one_line_are_ambiguous_by_line_and_exact_by_symbol():
    o = obj(A, B)
    sym, how = verify.claim_symbol(fn(), unit(10, []), o, SAME_LINE)
    assert (sym, how) == ([A, B], 'line')
    assert verify.claim_symbol(fn(), unit(10, []), o, SAME_LINE, table_symbol=B) == (B, 'symbol')


def test_a_symbol_the_unit_no_longer_produces_falls_back_to_the_line():
    # the session re-signed FnA: the table still says A, the unit now defines ?FnA@@YAHXZ
    new = '?FnA@@YAHXZ'
    listing = SWAPPED.replace(A, new)
    o = obj(new, B)
    assert verify.claim_symbol(fn(), unit(20, [10]), o, listing, table_symbol=A) == (new, 'line')


def test_a_symbol_without_a_section_is_not_a_definition():
    o = SimpleNamespace(by_name={A: SimpleNamespace(section=0), B: SimpleNamespace(section=1)})
    assert verify.claim_symbol(fn(), unit(10, [20]), o, SWAPPED, table_symbol=A) == (B, 'line')


def test_a_hint_beats_the_table():
    o = obj(A, B)
    assert verify.claim_symbol(fn(), unit(10, [20]), o, SWAPPED, table_symbol=A,
                               symbol_hint=B) == (B, 'hint')


def test_a_function_found_by_line_is_told_in_the_report():
    rep = verify.Report()
    data = dict(functions={})
    f = SimpleNamespace(address=0x401000, name='f')
    res = dict(address='0x00401000', error='x', claimed=True, found_by='line')
    verify._report_function(rep, data, f, res)
    assert any(ln.startswith('INFO') and 'found by line (table has no current symbol)' in ln
               for ln in rep.lines)
    rep2 = verify.Report()
    verify._report_function(rep2, data, f, dict(res, found_by='symbol'))
    assert not any('found by line' in ln for ln in rep2.lines)


def test_the_listing_rule_is_unchanged():
    assert source.symbol_for_claim(SWAPPED, 'op/u.cpp', 10, [20]) == B
