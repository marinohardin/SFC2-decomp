"""Queue order, attempt cap, model tiers, claims, and the loop's JSON plumbing."""
import io
import json
import os
import subprocess
import sys
import types

import pytest

from conftest import make_pe
from harness.lib import coff, loopio, matched, workqueue


def test_tiers_by_default_sonnet_builds_a_small_function_opus_a_larger_one_then_opus():
    t = workqueue.Tiers.from_env({})
    small, large = t.for_size(127), t.for_size(128)
    assert [small.model(n) for n in range(4)] == ['sonnet', 'opus', 'opus', None]
    assert [large.model(n) for n in range(4)] == ['opus', 'opus', 'opus', None]
    assert [small.role(n, small.model(n)) for n in range(3)] == ['build', 'fix', 'build']
    assert [large.role(n, large.model(n)) for n in range(3)] == ['build', 'fix', 'build']
    assert small.cap == large.cap == 3
    # A size that is unknown, zero or not a number runs the regular order.
    for size in (None, 0, '', 'x'):
        assert t.for_size(size).model(0) == 'opus'


def test_the_size_line_and_both_orders_come_from_the_environment():
    t = workqueue.Tiers.from_env({'HARNESS_SMALL_BYTES': '64'})
    assert (t.for_size(63).model(0), t.for_size(64).model(0)) == ('sonnet', 'opus')
    off = workqueue.Tiers.from_env({'HARNESS_SMALL_BYTES': '0'})
    assert off.for_size(10).model(0) == 'opus'
    # HARNESS_TIERS alone sets one order for every size, as before sizes mattered.
    one = workqueue.Tiers.from_env({'HARNESS_TIERS': 'sonnet,opus,opus'})
    assert one.for_size(10).regular == one.for_size(5000).regular == ['sonnet', 'opus', 'opus']
    both = workqueue.Tiers.from_env({'HARNESS_TIERS': 'opus,sonnet',
                                     'HARNESS_SMALL_TIERS': 'sonnet'})
    assert (both.for_size(10).regular, both.for_size(500).regular) == \
        (['sonnet'], ['opus', 'sonnet'])
    with pytest.raises(ValueError):
        workqueue.Tiers.from_env({'HARNESS_SMALL_TIERS': 'fable'})


def test_fable_comes_after_the_default_tiers_and_only_when_enabled():
    t = workqueue.Tiers.from_env({'HARNESS_ALLOW_FABLE': '1'})
    assert [t.for_size(40).model(n) for n in range(5)] == \
        ['sonnet', 'opus', 'opus', 'fable', None]
    assert [t.for_size(400).model(n) for n in range(5)] == \
        ['opus', 'opus', 'opus', 'fable', None]
    assert t.role(3, 'fable') == 'fix'
    assert 'fable' not in workqueue.Tiers.from_env({'HARNESS_ALLOW_FABLE': 'yes'}).sequence
    with pytest.raises(ValueError):
        workqueue.Tiers.from_env({'HARNESS_TIERS': 'fable,opus'})
    assert workqueue.Tiers.from_env({'HARNESS_TIERS': 'opus, sonnet'}).sequence == \
        ['opus', 'sonnet']


def fn(address, size, name, demangled='', source='sdklib-exact', kind='function', lib=False):
    return types.SimpleNamespace(
        address=address, size=size, name=name, demangled=demangled, name_source=source,
        kind=kind, is_library=lib, id=f'0x{address:08x}',
        has_name=not name.startswith('FUN_'), has_prototype='(' in demangled)


def test_queue_puts_named_functions_first_then_small_then_similar_to_solved():
    code = bytes.fromhex('8b4104c3') + b'\x90' * 12 + bytes.fromhex('8b4104c3') + \
        b'\x90' * 12 + bytes.fromhex('33c0c3') + b'\x90' * 13 + bytes(0x30 + 5000)
    exe = coff.PeImage(make_pe(code))
    fns = [fn(0x401000, 4, 'A::solved', 'int f(void)'),
           fn(0x401010, 4, 'B::similar', 'int f(void)'),
           fn(0x401020, 5, 'C::other', 'int f(void)'),
           fn(0x401030, 2, 'FUN_00401030', source='default'),
           fn(0x401040, 4, 'D::lib', 'int f(void)', lib=True),
           fn(0x401050, 4, 'E::funclet', 'int f(void)', kind='eh-funclet'),
           fn(0x401060, 5000, 'F::huge', 'int f(void)')]
    index = types.SimpleNamespace(functions=fns, by_address={f.address: f for f in fns})
    done = matched.empty()
    matched.record(done, 0x401000, dict(name='A::solved', score=100.0, exact=True), '2026-11-01')
    q = workqueue.build(index, exe, done, [], workqueue.Tiers())
    # B and C share a size bucket; B is identical to the solved A, so it comes first.
    assert [e['name'] for e in q] == ['B::similar', 'C::other', 'F::huge', 'FUN_00401030']


def test_function_is_moved_to_difficult_after_every_regular_attempt(tmp_path):
    p = types.SimpleNamespace(ATTEMPTS=str(tmp_path / 'a.tsv'), DIFFICULT=str(tmp_path / 'd.tsv'),
                              REVISIT=str(tmp_path / 'r.tsv'))
    tiers = workqueue.Tiers(['opus', 'sonnet'])
    entry = dict(address='0x00401010', name='B::f')
    row = dict(time='2026-11-01T00:00:00Z', address='0x00401010', name='B::f', model='opus',
               outcome='failed', score_before='0.00', score_after='42.00')
    assert workqueue.record(p, entry, row, tiers) is None
    assert workqueue.record(p, entry, dict(row, outcome='limit'), tiers) is None
    assert workqueue.record(p, entry, dict(row, time='2026-11-01T01:00:00Z', model='sonnet',
                                           outcome='improved', score_before='42.00',
                                           score_after='60.00'), tiers) == 'difficult'
    (d,) = workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS)
    assert (d['address'], d['attempts'], d['last_model'], d['best_score']) == \
        ('0x00401010', '2', 'sonnet', '60.00')
    assert d['reason'] == ('not exact after 2 attempts: best not above 90% after 2 attempts; '
                           'best 60.00%')
    assert workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS) == []
    queue = [dict(address='0x00401010', name='B::f'), dict(address='0x00401020', name='C::g')]
    attempts = workqueue.read_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS)
    nxt = workqueue.next_entry(queue, attempts, [d], tiers)
    assert (nxt['address'], nxt['model'], nxt['attempts'], nxt['role']) == \
        ('0x00401020', 'opus', 0, 'build')


def test_next_skips_exact_functions_and_functions_another_loop_holds():
    tiers = workqueue.Tiers(['opus', 'sonnet', 'opus'])
    queue = [dict(address=a, name=a) for a in ('0x00401000', '0x00401010', '0x00401020')]
    attempts = [dict(address='0x00401000', outcome='exact'),
                dict(address='0x00401020', outcome='improved')]
    nxt = workqueue.next_entry(queue, attempts, [], tiers, claim=lambda a: a != '0x00401010')
    assert (nxt['address'], nxt['model'], nxt['role']) == ('0x00401020', 'sonnet', 'fix')
    assert workqueue.next_entry(queue, attempts, [], tiers, claim=lambda a: False) is None


def test_next_chooses_each_function_s_order_by_its_size(tmp_path):
    tiers = workqueue.Tiers.from_env({})
    queue = [dict(address='0x00401000', name='a', size=40),
             dict(address='0x00401100', name='b', size=300)]
    nxt = workqueue.next_entry(queue, [], [], tiers)
    assert (nxt['address'], nxt['model'], nxt['role']) == ('0x00401000', 'sonnet', 'build')
    nxt = workqueue.next_entry(queue, [], [], tiers, claim=lambda a: a != '0x00401000')
    assert (nxt['address'], nxt['model'], nxt['role']) == ('0x00401100', 'opus', 'build')
    # The second attempt of a small function is Opus, in a fix round.
    first = dict(time='2026-11-10T00:01:00Z', address='0x00401000', name='a', model='sonnet',
                 outcome='improved', score_before='0.00', score_after='60.00')
    nxt = workqueue.next_entry(queue, [first], [], tiers)
    assert (nxt['model'], nxt['role'], nxt['attempts']) == ('opus', 'fix', 1)


def test_a_claim_is_held_by_one_checkout_until_released(tmp_path):
    d = str(tmp_path / 'claims')
    assert workqueue.acquire(d, '0x00401000', '/wt/a', os.getpid())
    assert workqueue.acquire(d, '0x00401000', '/wt/a', os.getpid())
    assert not workqueue.acquire(d, '0x00401000', '/wt/b', os.getpid())
    workqueue.release(d, '0x00401000', '/wt/b')  # not b's to release
    assert not workqueue.acquire(d, '0x00401000', '/wt/b', os.getpid())
    workqueue.release(d, '0x00401000', '/wt/a')
    assert workqueue.acquire(d, '0x00401000', '/wt/b', os.getpid())


def test_a_claim_left_by_a_dead_loop_is_taken_over(tmp_path):
    d = str(tmp_path / 'claims')
    p = subprocess.Popen([sys.executable, '-c', 'pass'])
    p.wait()
    assert workqueue.acquire(d, '0x00401000', '/wt/a', p.pid)
    assert workqueue.acquire(d, '0x00401000', '/wt/b', os.getpid())
    with open(os.path.join(d, '0x00401000.lock')) as f:
        assert json.load(f)['owner'] == '/wt/b'


def test_attempt_rows_from_several_writers_all_land(tmp_path):
    path = str(tmp_path / 'shared' / 'attempts.tsv')
    for i in range(3):
        workqueue.append_tsv(path, workqueue.ATTEMPT_FIELDS,
                             dict(time='2026-11-01T00:00:00Z', address=f'0x{i:08x}'))
    rows = workqueue.read_tsv(path, workqueue.ATTEMPT_FIELDS)
    assert [r['address'] for r in rows] == ['0x00000000', '0x00000001', '0x00000002']


def test_login_status_is_read_from_standard_input(monkeypatch):
    monkeypatch.setattr(sys, 'stdin', io.StringIO('{"loggedIn": false}'))
    assert loopio.load('-') == {'loggedIn': False}
    monkeypatch.setattr(sys, 'stdin', io.StringIO('not json'))
    assert loopio.load('-') is None


def test_usage_limit_is_told_apart_from_a_failed_task():
    assert loopio.limited(dict(is_error=True, result='Claude usage limit reached. Resets at 5pm'))
    assert not loopio.limited(dict(is_error=False, subtype='success', result='done'))
    assert not loopio.limited(dict(is_error=True, subtype='error_max_turns', result=''))


def test_the_five_hour_limit_message_is_a_usage_limit():
    # The claude CLI's result when the plan's 5-hour window is used up: the run reports
    # subtype 'success' with is_error set, and the 429 sits in its own field.
    message = "You've hit your 5-hour limit \u00b7 resets 11am (America/Los_Angeles)"
    assert loopio.limited(dict(is_error=True, subtype='success', result=message))
    assert loopio.limited(dict(is_error=True, subtype='success', result='',
                               terminal_reason='api_error', api_error_status=429))
    assert not loopio.limited(dict(is_error=True, subtype='success', result='',
                                   terminal_reason='api_error', api_error_status=500))


def test_score_reads_the_claimed_function_and_treats_unscored_as_zero():
    v = dict(exit=0, results=[dict(address='0x00401000', score=97.5, exact=False, relocs=[])])
    assert loopio.score(v, '0x00401000') == (97.5, False, 0)
    assert loopio.score(dict(exit=2, results=[]), '0x00401000') == (0.0, False, 2)
    assert loopio.score(None, '0x00401000') == (0.0, False, 2)


def test_attempt_row_carries_tokens_and_cost():
    out = dict(total_cost_usd=0.42, num_turns=7,
               usage=dict(input_tokens=10, output_tokens=20, cache_read_input_tokens=30,
                          cache_creation_input_tokens=40))
    r = loopio.row(dict(address='0x1', name='f', model='opus'), 'improved', '10', '20.5', out,
                   '61', '2026-11-01T00:00:00Z')
    assert (r['input_tokens'], r['output_tokens'], r['cache_read_tokens'],
            r['cache_write_tokens'], r['cost_usd'], r['turns']) == (10, 20, 30, 40, 0.42, 7)
    assert (r['score_before'], r['score_after']) == ('10.00', '20.50')


# --- --unit / --address selection -------------------------------------------------------------

UNIT_MAP = [(0x00401000, 0x0040101f, 'Tiger/tsay.cpp'),
            (0x00401020, 0x0040103f, 'unsorted/fn_00401020.cpp')]


def test_select_takes_a_units_open_functions_in_address_order():
    # The queue's own order (named first, small first) is not the batch's order.
    queue = [dict(address='0x00401018'), dict(address='0x00401030'), dict(address='0x00401004')]
    assert workqueue.select(queue, UNIT_MAP, units=['Tiger/tsay.cpp']) == \
        ['0x00401004', '0x00401018']


def test_select_adds_single_addresses_and_drops_ones_not_in_the_queue():
    queue = [dict(address='0x00401018'), dict(address='0x00401030')]
    # 0x00401004 is exact or difficult already, so it is not in the queue.
    assert workqueue.select(queue, UNIT_MAP, addresses=['0x00401030', '0x00401004']) == \
        ['0x00401030']
    assert workqueue.select(queue, UNIT_MAP, units=['Tiger/tsay.cpp'],
                            addresses=['0x401030']) == ['0x00401018', '0x00401030']


def test_select_refuses_a_unit_not_in_the_map_and_a_malformed_address():
    with pytest.raises(ValueError, match='Tiger/nosuch.cpp'):
        workqueue.select([], UNIT_MAP, units=['Tiger/tsay.cpp', 'Tiger/nosuch.cpp'])
    for bad in ('zz', '-5', '1_0', '0x123456789', ''):
        with pytest.raises(ValueError, match='not an address'):
            workqueue.select([], UNIT_MAP, addresses=[bad])


def test_select_includes_both_ends_of_a_unit_range_and_nothing_past_them():
    queue = [dict(address=a) for a in ('0x00400fff', '0x00401000', '0x0040101f', '0x00401020')]
    assert workqueue.select(queue, UNIT_MAP, units=['Tiger/tsay.cpp']) == \
        ['0x00401000', '0x0040101f']


def test_select_lists_a_function_named_twice_once():
    queue = [dict(address='0x00401004'), dict(address='0x00401030')]
    assert workqueue.select(queue, UNIT_MAP, units=['Tiger/tsay.cpp'],
                            addresses=['0x00401004', '0x401004', '0x00401030']) == \
        ['0x00401004', '0x00401030']


def test_restrict_keeps_the_selection_order():
    queue = [dict(address='0x00401018', name='b'), dict(address='0x00401004', name='a'),
             dict(address='0x00401030', name='c')]
    assert [e['name'] for e in workqueue.restrict(queue, ['0x00401004', '0x00401018'])] == \
        ['a', 'b']


def _queue_cli(tmp_path, *args, selection=False):
    env = dict(os.environ, HARNESS_STATE=str(tmp_path / 'state'),
               HARNESS_UNITS_TSV=str(tmp_path / 'UNITS.tsv'),
               HARNESS_DIFFICULT=str(tmp_path / 'difficult.tsv'))
    env.pop('HARNESS_SELECTION', None)
    if selection:
        env['HARNESS_SELECTION'] = str(tmp_path / 'state' / 'selection.json')
    script = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'queue.py')
    return subprocess.run([sys.executable, script, *args], env=env, capture_output=True,
                          text=True)


def _write_queue_inputs(tmp_path):
    (tmp_path / 'UNITS.tsv').write_text(
        'unit\tfirst\tlast\tfunctions\tevidence\tprofile\theader\n'
        'Tiger/tsay.cpp\t0x00401000\t0x0040101f\t2\tassert-path\tsdk-release\t\n'
        'unsorted/fn_00401020.cpp\t0x00401020\t0x0040103f\t1\tpage\tsdk-release\t\n')
    entries = [dict(address=a, name=n, size=4, named=True, similarity=0.0)
               for a, n in (('0x00401030', 'C'), ('0x00401018', 'B'), ('0x00401004', 'A'))]
    workqueue.save_queue(str(tmp_path / 'state' / 'queue.json'), entries, 'test')


def test_queue_cli_next_follows_the_selection_only_when_the_loop_names_it(tmp_path):
    _write_queue_inputs(tmp_path)
    r = _queue_cli(tmp_path, 'select', '--unit', 'Tiger/tsay.cpp')
    assert r.returncode == 0, r.stderr
    assert json.loads((tmp_path / 'state' / 'selection.json').read_text())['entries'] == \
        ['0x00401004', '0x00401018']
    r = _queue_cli(tmp_path, 'next', selection=True)
    assert json.loads(r.stdout)['name'] == 'A'
    _queue_cli(tmp_path, 'release', '0x00401004')
    # Without HARNESS_SELECTION a leftover selection file restricts nothing.
    r = _queue_cli(tmp_path, 'next')
    assert json.loads(r.stdout)['name'] == 'C'


def test_queue_cli_select_fails_on_an_unknown_unit_and_writes_nothing(tmp_path):
    _write_queue_inputs(tmp_path)
    r = _queue_cli(tmp_path, 'select', '--unit', 'Tiger/nosuch.cpp')
    assert r.returncode == 2 and 'Tiger/nosuch.cpp' in r.stderr
    assert not (tmp_path / 'state' / 'selection.json').exists()
    assert _queue_cli(tmp_path, 'select', '--unit').returncode == 2


def test_queue_cli_refuses_an_unreadable_selection_for_next_but_still_releases(tmp_path):
    _write_queue_inputs(tmp_path)
    r = _queue_cli(tmp_path, 'next', selection=True)
    assert r.returncode == 2 and 'selection' in r.stderr
    # The loop's exit trap releases its claim even if the selection file is gone.
    assert _queue_cli(tmp_path, 'release', '0x00401004', selection=True).returncode == 0


def test_queue_cli_select_with_no_open_function_is_an_error(tmp_path):
    _write_queue_inputs(tmp_path)
    r = _queue_cli(tmp_path, 'select', '--address', '0x00402000')
    assert r.returncode == 2 and 'no open function' in r.stderr
    assert not (tmp_path / 'state' / 'selection.json').exists()


def test_loop_refuses_a_unit_value_with_shell_characters(tmp_path):
    loop = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'loop.sh')
    for bad in (['--unit', 'Tiger/a b.cpp'], ['--unit', '$(true)'], ['--address'],
                ['--unit', '']):
        r = subprocess.run(['sh', loop, *bad], capture_output=True, text=True,
                           env=dict(os.environ, HARNESS_STATE=str(tmp_path / 'state')))
        assert r.returncode == 2 and ('ERROR' in r.stderr or 'usage' in r.stderr), bad
    assert not (tmp_path / 'state').exists()


def test_queue_cli_select_reports_a_missing_unit_map(tmp_path):
    _write_queue_inputs(tmp_path)
    (tmp_path / 'UNITS.tsv').unlink()
    r = _queue_cli(tmp_path, 'select', '--unit', 'Tiger/tsay.cpp')
    assert r.returncode == 2 and 'ERROR' in r.stderr


# --- --brief ----------------------------------------------------------------------------------

def _prompt(tmp_path, monkeypatch, brief=None):
    template = tmp_path / 'function.md'
    template.write_text('Function {{ADDRESS}}.\n')
    monkeypatch.setattr(loopio.paths, 'CLAIM', str(tmp_path / 'no-claim.json'))
    if brief is None:
        monkeypatch.delenv('HARNESS_BRIEF', raising=False)
    else:
        monkeypatch.setenv('HARNESS_BRIEF', brief)
    return loopio.prompt(dict(address='0x00401000', name='A'), template_path=str(template))


def test_prompt_appends_the_batch_brief_after_a_rule(tmp_path, monkeypatch):
    b = tmp_path / 'brief.md'
    b.write_text('Notes on the class.\n\n')
    assert _prompt(tmp_path, monkeypatch, str(b)) == \
        'Function 0x00401000.\n\n---\n\nNotes on the class.\n'
    assert _prompt(tmp_path, monkeypatch) == 'Function 0x00401000.\n'


def test_prompt_stops_when_the_brief_cannot_be_read(tmp_path, monkeypatch):
    with pytest.raises(SystemExit, match='batch brief'):
        _prompt(tmp_path, monkeypatch, str(tmp_path / 'missing.md'))


def test_loop_refuses_a_missing_brief_or_one_inside_the_worktree(tmp_path):
    here = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    loop = os.path.join(here, 'loop.sh')
    env = dict(os.environ, HARNESS_STATE=str(tmp_path / 'state'))
    r = subprocess.run(['sh', loop, '--dry-run', '--brief', str(tmp_path / 'missing.md')],
                       capture_output=True, text=True, env=env)
    assert r.returncode == 2 and 'no readable brief' in r.stderr
    r = subprocess.run(['sh', loop, '--dry-run', '--brief', os.path.join(here, 'README.md')],
                       capture_output=True, text=True, env=env)
    assert r.returncode == 2 and 'outside this worktree' in r.stderr
    assert subprocess.run(['sh', loop, '--brief'], capture_output=True, text=True,
                          env=env).returncode == 2


def _loop(tmp_path, *args, cwd=None):
    here = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    return subprocess.run(['sh', os.path.join(here, 'loop.sh'), '--dry-run', *args],
                          capture_output=True, text=True, cwd=cwd,
                          env=dict(os.environ, HARNESS_STATE=str(tmp_path / 'state')))


def test_loop_refuses_an_empty_brief(tmp_path):
    b = tmp_path / 'brief.md'
    b.write_text(' \n\t\n')
    r = _loop(tmp_path, '--brief', str(b))
    assert r.returncode == 2 and 'is empty' in r.stderr


def test_loop_takes_a_relative_brief_from_the_callers_directory(tmp_path):
    r = _loop(tmp_path, '--brief', 'notes.md', cwd=str(tmp_path))
    assert r.returncode == 2 and str(tmp_path / 'notes.md') in r.stderr


def test_loop_refuses_a_link_outside_that_points_into_the_worktree(tmp_path):
    here = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    link = tmp_path / 'readme.md'
    link.symlink_to(os.path.join(here, 'README.md'))
    r = _loop(tmp_path, '--brief', str(link))
    assert r.returncode == 2 and 'outside this worktree' in r.stderr


def test_loop_refuses_a_worktree_path_spelled_in_other_letter_case(tmp_path):
    here = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    other = os.path.join(here.swapcase(), 'README.md')
    if not os.path.exists(other):
        pytest.skip('case-sensitive file system')
    r = _loop(tmp_path, '--brief', other)
    assert r.returncode == 2 and 'outside this worktree' in r.stderr
