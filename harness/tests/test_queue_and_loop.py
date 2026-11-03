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


def test_tiers_default_to_opus_build_two_sonnet_fix_rounds_then_opus():
    t = workqueue.Tiers.from_env({})
    assert [t.model(n) for n in range(5)] == ['opus', 'sonnet', 'sonnet', 'opus', None]
    assert [t.role(n, t.model(n)) for n in range(4)] == ['build', 'fix', 'fix', 'build']
    assert t.cap == 4


def test_fable_runs_only_after_the_default_tiers_and_only_when_enabled():
    t = workqueue.Tiers.from_env({'HARNESS_ALLOW_FABLE': '1'})
    assert [t.model(n) for n in range(6)] == ['opus', 'sonnet', 'sonnet', 'opus', 'fable', None]
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


def test_function_is_moved_to_difficult_after_the_attempt_cap(tmp_path):
    p = types.SimpleNamespace(ATTEMPTS=str(tmp_path / 'a.tsv'), DIFFICULT=str(tmp_path / 'd.tsv'))
    tiers = workqueue.Tiers(['opus', 'sonnet'])
    entry = dict(address='0x00401010', name='B::f')
    row = dict(time='2026-11-01T00:00:00Z', address='0x00401010', name='B::f', model='opus',
               outcome='failed', score_before='0.00', score_after='42.00')
    assert workqueue.record(p, entry, row, tiers) is False
    assert workqueue.record(p, entry, dict(row, outcome='limit'), tiers) is False
    assert workqueue.record(p, entry, dict(row, model='sonnet', outcome='tampered',
                                           score_after='60.00'), tiers) is True
    (d,) = workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS)
    assert (d['address'], d['attempts'], d['last_model'], d['best_score']) == \
        ('0x00401010', '2', 'sonnet', '60.00')
    queue = [dict(address='0x00401010', name='B::f'), dict(address='0x00401020', name='C::g')]
    attempts = workqueue.read_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS)
    nxt = workqueue.next_entry(queue, attempts, [d], tiers)
    assert (nxt['address'], nxt['model'], nxt['attempts'], nxt['role']) == \
        ('0x00401020', 'opus', 0, 'build')


def test_next_skips_exact_functions_and_functions_another_loop_holds():
    tiers = workqueue.Tiers()
    queue = [dict(address=a, name=a) for a in ('0x00401000', '0x00401010', '0x00401020')]
    attempts = [dict(address='0x00401000', outcome='exact'),
                dict(address='0x00401020', outcome='improved')]
    nxt = workqueue.next_entry(queue, attempts, [], tiers, claim=lambda a: a != '0x00401010')
    assert (nxt['address'], nxt['model'], nxt['role']) == ('0x00401020', 'sonnet', 'fix')
    assert workqueue.next_entry(queue, attempts, [], tiers, claim=lambda a: False) is None


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
