"""A session's function must reach an unclaimed function through the symbol its table row has
(mangled or add: refused; callers: a warning), and exact functions already in the tree are never
judged by it."""
import pytest

from conftest import FakeIndex
from test_claim_symbols import exact_caller
from harness.lib import paths, verify

CALLEE = '0x00401200'
OWN = {'0x00401000'}


@pytest.fixture(autouse=True)
def _own_build_folder(monkeypatch, tmp_path):
    monkeypatch.setattr(paths, 'BUILD', str(tmp_path / 'build'))


def run(rows, caller=None, check_all=False, own=OWN, evidence=None, claims=None, objects=None):
    rep = verify.Report()
    rep.results = [caller or exact_caller()]
    rep.objects = objects or {}
    verify.report_table_breaks(rep, claims or {}, rows, check_all, own,
                               evidence or FakeIndex())
    return rep


def text(rep):
    return '\n'.join(rep.lines)


def test_another_symbol_for_a_mangled_row_with_the_name_in_the_evidence_is_refused():
    evidence = FakeIndex(known={'?other@@YAXXZ': [0x401200]})
    rep = run({CALLEE: ('?other@@YAXXZ', 'mangled')}, evidence=evidence)
    assert rep.failed
    assert [b['class'] for b in rep.table_breaks] == ['wrong-function']
    assert rep.lines[0].startswith('ERROR')
    assert 'the symbol table has ?other@@YAXXZ at 0x00401200 (mangled)' in rep.lines[0]
    assert 'declare the callee so it mangles to ?other@@YAXXZ' in rep.lines[0]


def test_a_stand_in_against_an_add_row_goes_to_the_stop_hook_not_to_an_error():
    rep = run({CALLEE: ('?added@@YAXXZ', 'add')})
    assert not rep.failed
    assert [(b['function'], b['claimed']) for b in rep.standins] == \
        [('0x00401000', '?added@@YAXXZ')]
    assert rep.lines[0].startswith('WARN')


def test_a_callers_row_only_warns():
    evidence = FakeIndex(known={'?other@@YAXXZ': [0x401200]})
    rep = run({CALLEE: ('?other@@YAXXZ', 'callers')}, evidence=evidence)
    assert not rep.failed and rep.standins == []
    assert len(rep.lines) == 1 and rep.lines[0].startswith('WARN')
    assert 'provisional' in rep.lines[0]


def test_the_symbol_the_row_has_is_no_break_and_an_empty_row_is_not_judged():
    assert run({CALLEE: ('?callee@@YAXXZ', 'add')}).lines == []
    assert run({}).lines == []


def test_a_claimed_target_and_a_claim_row_are_left_to_the_claim_check():
    assert run({CALLEE: ('?other@@YAXXZ', 'add')}, claims={CALLEE: ('?x@@YAXXZ', 'recorded')}
               ).lines == []
    assert run({CALLEE: ('?other@@YAXXZ', 'claim')}).lines == []


def test_a_function_that_is_not_the_sessions_own_is_not_judged():
    rep = run({CALLEE: ('?other@@YAXXZ', 'add')}, own={'0x00409999'})
    assert rep.lines == [] and not rep.failed


def test_under_all_nothing_fails_and_the_breaks_are_only_counted():
    evidence = FakeIndex(known={'?other@@YAXXZ': [0x401200]})
    rep = run({CALLEE: ('?other@@YAXXZ', 'mangled')}, check_all=True, evidence=evidence)
    assert not rep.failed and rep.standins == []
    assert len(rep.table_breaks) == 1
    assert len(rep.lines) == 1 and rep.lines[0].startswith('WARN')
    assert '1 relocations of exact functions' in rep.lines[0] and '1 wrong-function' in rep.lines[0]
    # a partial function is not counted under --all
    assert run({CALLEE: ('?other@@YAXXZ', 'mangled')}, check_all=True, evidence=evidence,
               caller=dict(exact_caller(), exact=False)).table_breaks == []


def test_a_folded_body_is_a_warning_even_against_a_mangled_row():
    evidence = FakeIndex(known={'?callee@@YAXXZ': [0x401200], '?other@@YAXXZ': [0x401200]})
    rep = run({CALLEE: ('?other@@YAXXZ', 'mangled')}, evidence=evidence)
    assert not rep.failed and rep.standins == []
    assert rep.lines[0].startswith('WARN') and 'class folded' in rep.lines[0]


def test_a_stand_in_already_there_at_the_start_does_not_refuse_an_exact_function(tmp_path):
    build = tmp_path / 'build'
    build.mkdir()
    (build / 'table_breaks.tsv').write_text(
        'function\toffset\tsymbol\ttarget\tclaimed\tclass\n'
        '0x00401000\t0x1\t?callee@@YAXXZ\t0x00401200\t?added@@YAXXZ\tstand-in\n')
    rep = run({CALLEE: ('?added@@YAXXZ', 'add')})
    assert rep.standins == [] and 'already so when the session started' in rep.lines[0]
    # a function that is not exact is refused all the same
    rep = run({CALLEE: ('?added@@YAXXZ', 'add')}, caller=dict(exact_caller(), exact=False))
    assert len(rep.standins) == 1
