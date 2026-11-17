"""The stop rule, the revisit list, lean sessions, the context pack and handoff notes."""
import contextlib
import json
import os
import subprocess
import sys
import types

import pytest

from conftest import make_pe
from harness.lib import loopio, matched, notes, pack, paths, timebox, workqueue

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
A = '0x00401010'


def att(n, model, before, after, outcome=None, address=A):
    """One counted attempt row; the outcome follows from the scores unless given."""
    if outcome is None:
        outcome = 'improved' if after > before else 'no-change'
    return dict(time=f'2026-11-10T00:0{n}:00Z', address=address, name='B::f', model=model,
                outcome=outcome, score_before=f'{before:.2f}', score_after=f'{after:.2f}')


TIERS = workqueue.Tiers()                       # opus, sonnet, opus; Fable not allowed
FABLE = workqueue.Tiers(allow_fable=True)


# --- the stop rule ----------------------------------------------------------------------------

def test_attempt_one_is_opus_and_a_failed_first_attempt_still_gets_a_fix_round():
    p = workqueue.plan([], TIERS)
    assert (p.model, p.role, p.attempts) == ('opus', 'build', 0)
    p = workqueue.plan([att(1, 'opus', 0, 0, 'failed')], TIERS)
    assert (p.model, p.role) == ('sonnet', 'fix')


def test_a_second_attempt_that_does_not_raise_the_best_ends_the_run():
    rows = [att(1, 'opus', 0, 95.5), att(2, 'sonnet', 95.5, 95.5, 'failed')]
    p = workqueue.plan(rows, TIERS)
    assert (p.model, p.full, p.best) == (None, False, 95.5)
    assert p.note == 'attempt 2 (sonnet) did not raise the best; waits for Fable; best 95.50%'


def test_after_two_attempts_the_run_ends_unless_the_best_is_above_90():
    rows = [att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 80)]
    p = workqueue.plan(rows, FABLE)
    assert (p.model, p.full) == (None, False)
    assert p.note == 'best not above 90% after 2 attempts; best 80.00%'
    rows = [att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 91)]
    assert workqueue.plan(rows, TIERS).model == 'opus'


def test_a_best_of_exactly_90_ends_the_run_but_qualifies_for_fable():
    rows = [att(1, 'opus', 0, 50), att(2, 'sonnet', 50, 90)]
    assert workqueue.plan(rows, TIERS).model is None
    p = workqueue.plan(rows, FABLE)
    assert (p.model, p.role) == ('fable', 'fix')


def test_fable_gets_one_try_after_the_regular_run_then_the_function_had_its_full_chance():
    rows = [att(1, 'opus', 0, 60), att(2, 'sonnet', 60, 93), att(3, 'opus', 93, 97)]
    p = workqueue.plan(rows, TIERS)
    assert (p.model, p.full, p.note) == (None, False,
                                         'all 3 regular attempts used; waits for Fable; '
                                         'best 97.00%')
    assert workqueue.plan(rows, FABLE).model == 'fable'
    p = workqueue.plan(rows + [att(4, 'fable', 97, 97, 'failed')], FABLE)
    assert (p.model, p.full, p.note) == (None, True,
                                         'all 3 regular attempts used; Fable tried; best 97.00%')


def test_fable_after_an_early_stop_does_not_count_as_the_full_chance():
    rows = [att(1, 'opus', 0, 92), att(2, 'sonnet', 92, 92, 'failed'),
            att(3, 'fable', 92, 98)]
    p = workqueue.plan(rows, FABLE)
    assert (p.model, p.full) == (None, False)
    assert p.note == 'attempt 2 (sonnet) did not raise the best; Fable tried; best 98.00%'


def test_a_function_far_from_a_match_never_gets_fable():
    rows = [att(1, 'opus', 0, 30), att(2, 'sonnet', 30, 30, 'failed')]
    assert workqueue.plan(rows, FABLE).model is None


def test_usage_limit_stops_never_count():
    rows = [dict(att(1, 'opus', 0, 0), outcome='limit'), att(2, 'opus', 0, 40),
            dict(att(3, 'sonnet', 40, 40), outcome='limit'),
            dict(att(4, 'sonnet', 40, 40), outcome='error')]
    h = workqueue.history(rows, A)
    assert [r['time'] for r in h] == ['2026-11-10T00:02:00Z']
    assert workqueue.plan(h, TIERS).model == 'sonnet'


def test_the_best_counts_the_score_an_attempt_started_from():
    # Another branch had the function at 70%; this attempt started there and dropped.
    rows = [att(1, 'opus', 70, 10, 'failed'), att(2, 'sonnet', 10, 65)]
    p = workqueue.plan(rows, TIERS)
    assert (p.best, p.model) == (70.0, None)
    assert p.note.startswith('attempt 2 (sonnet) did not raise the best')


# --- the revisit list and the queue -----------------------------------------------------------

def _paths(tmp_path):
    return types.SimpleNamespace(ATTEMPTS=str(tmp_path / 'attempts.tsv'),
                                 DIFFICULT=str(tmp_path / 'difficult.tsv'),
                                 REVISIT=str(tmp_path / 'revisit.tsv'))


def test_a_run_the_rule_ends_early_goes_to_the_revisit_list(tmp_path):
    p = _paths(tmp_path)
    entry = dict(address=A, name='B::f')
    assert workqueue.record(p, entry, att(1, 'opus', 0, 40), TIERS) is None
    assert workqueue.record(p, entry, att(2, 'sonnet', 40, 40, 'failed'), TIERS) == 'revisit'
    (r,) = workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)
    assert r == dict(address=A, name='B::f', best_score='40.00', attempts='2',
                     last_model='sonnet', listed='2026-11-10T00:02:00Z',
                     note='attempt 2 (sonnet) did not raise the best; best 40.00%')
    assert workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS) == []
    # A late row for a listed function changes nothing outside a revisit run.
    assert workqueue.record(p, entry, att(3, 'opus', 40, 41), TIERS) is None


def test_the_queue_skips_revisit_listed_functions_unless_told_to_take_them(tmp_path):
    queue = [dict(address=A, name='B::f'), dict(address='0x00401020', name='C::g')]
    attempts = [att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 40, 'failed')]
    revisit = [dict(address=A, listed='2026-11-10T00:02:00Z')]
    nxt = workqueue.next_entry(queue, attempts, [], TIERS, revisit=revisit)
    assert nxt['address'] == '0x00401020'
    nxt = workqueue.next_entry(queue, attempts, [], TIERS, revisit=revisit, take_revisit=True)
    # A revisit run starts over: only attempts after the listing count.
    assert (nxt['address'], nxt['model'], nxt['attempts'], nxt['best']) == (A, 'opus', 0, 0.0)


def test_a_function_whose_logged_run_is_over_is_skipped_even_when_unlisted():
    queue = [dict(address=A, name='B::f'), dict(address='0x00401020', name='C::g')]
    attempts = [att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 40, 'failed')]
    assert workqueue.next_entry(queue, attempts, [], TIERS)['address'] == '0x00401020'


def test_revisit_takes_a_function_another_checkout_listed_with_a_new_run():
    # The run ended short of its full chance in the shared log; this checkout has no row for it.
    queue = [dict(address=A, name='B::f')]
    attempts = [att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 40, 'failed')]
    assert workqueue.next_entry(queue, attempts, [], TIERS) is None
    nxt = workqueue.next_entry(queue, attempts, [], TIERS, take_revisit=True)
    assert (nxt['model'], nxt['role'], nxt['attempts']) == ('opus', 'build', 0)
    # One attempt into the new run, the next is its Sonnet fix round.
    attempts.append(att(3, 'opus', 40, 55))
    nxt = workqueue.next_entry(queue, attempts, [], TIERS, take_revisit=True)
    assert (nxt['model'], nxt['attempts']) == ('sonnet', 1)


def test_revisit_leaves_unlisted_functions_that_used_their_full_chance_or_wait_for_fable():
    queue = [dict(address=A, name='B::f')]
    full = [att(1, 'opus', 0, 50), att(2, 'sonnet', 50, 92), att(3, 'opus', 92, 95),
            att(4, 'fable', 95, 96)]
    assert workqueue.next_entry(queue, full, [], FABLE, take_revisit=True) is None
    waiting = [att(1, 'opus', 0, 92), att(2, 'sonnet', 92, 92, 'failed')]
    assert workqueue.next_entry(queue, waiting, [], TIERS, take_revisit=True) is None
    nxt = workqueue.next_entry(queue, waiting, [], FABLE, take_revisit=True)
    assert (nxt['model'], nxt['attempts']) == ('fable', 2)


def test_a_new_run_of_an_unlisted_function_is_listed_here_when_it_ends(tmp_path):
    p = _paths(tmp_path)
    entry = dict(address=A, name='B::f')
    for r in (att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 40, 'failed')):
        workqueue.append_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS, r)
    assert workqueue.record(p, entry, att(3, 'opus', 40, 45), TIERS, take_revisit=True) is None
    got = workqueue.record(p, entry, att(4, 'sonnet', 45, 45, 'failed'), TIERS,
                           take_revisit=True)
    assert got == 'revisit'
    (r,) = workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)
    assert (r['attempts'], r['best_score'], r['listed']) == ('2', '45.00', '2026-11-10T00:04:00Z')


def test_an_exact_revisit_takes_the_function_off_the_list(tmp_path):
    p = _paths(tmp_path)
    entry = dict(address=A, name='B::f')
    workqueue.record(p, entry, att(1, 'opus', 0, 40), TIERS)
    workqueue.record(p, entry, att(2, 'sonnet', 40, 40, 'failed'), TIERS)
    got = workqueue.record(p, entry, att(3, 'opus', 40, 100, 'exact'), TIERS, take_revisit=True)
    assert got == 'cleared'
    assert workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS) == []


def test_a_revisit_run_that_ends_again_replaces_the_row_and_one_with_its_full_chance_is_difficult(
        tmp_path):
    p = _paths(tmp_path)
    entry = dict(address=A, name='B::f')
    for r in (att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 40, 'failed')):
        workqueue.record(p, entry, r, TIERS)
    # The revisit run: three attempts after the listing, every regular tier, best above 90.
    run = [att(3, 'opus', 40, 92), att(4, 'sonnet', 92, 95), att(5, 'opus', 95, 96)]
    got = [workqueue.record(p, entry, r, TIERS, take_revisit=True) for r in run]
    assert got == [None, None, 'revisit']
    (r,) = workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)
    assert (r['attempts'], r['best_score'], r['listed']) == ('3', '96.00', '2026-11-10T00:05:00Z')
    assert r['note'] == 'all 3 regular attempts used; waits for Fable; best 96.00%'
    got = workqueue.record(p, entry, att(6, 'fable', 96, 96, 'failed'), FABLE, take_revisit=True)
    assert got == 'difficult'
    assert workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS) == []
    (d,) = workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS)
    # The Fable try continued the whole history: five regular attempts and Fable.
    assert (d['attempts'], d['last_model'], d['best_score']) == ('6', 'fable', '96.00')


def test_a_fable_try_in_a_revisit_run_is_final_so_the_loop_never_takes_the_function_again(
        tmp_path):
    # Stopped early at 99.78% and listed to wait for Fable; a --revisit --allow-fable loop gives
    # it Fable, which changes nothing. It goes to the difficult list and is not taken again.
    p = _paths(tmp_path)
    entry = dict(address=A, name='B::f')
    for r in (att(1, 'opus', 99.78, 99.78), att(2, 'sonnet', 99.78, 99.78)):
        workqueue.record(p, entry, r, TIERS)
    (r,) = workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)
    assert 'waits for Fable' in r['note']
    got = workqueue.record(p, entry, att(3, 'fable', 99.78, 99.78), FABLE, take_revisit=True)
    assert got == 'difficult'
    assert workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS) == []
    attempts = workqueue.read_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS)
    difficult = workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS)
    assert workqueue.next_entry([entry], attempts, difficult, FABLE, take_revisit=True) is None


def test_a_revisit_run_that_stops_short_again_goes_to_the_difficult_list(tmp_path):
    p = _paths(tmp_path)
    entry = dict(address=A, name='B::f')
    for r in (att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 40, 'failed')):
        workqueue.record(p, entry, r, TIERS)
    run = [att(3, 'opus', 40, 50), att(4, 'sonnet', 50, 50, 'failed')]
    got = [workqueue.record(p, entry, r, FABLE, take_revisit=True) for r in run]
    assert got == [None, 'difficult']
    assert workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS) == []


def test_a_revisit_of_a_function_waiting_for_fable_gives_it_fable_or_leaves_it_waiting():
    attempts = [att(1, 'opus', 0, 92), att(2, 'sonnet', 92, 92, 'failed')]
    queue = [dict(address=A, name='B::f')]
    revisit = [dict(address=A, listed='2026-11-10T00:02:00Z')]
    nxt = workqueue.next_entry(queue, attempts, [], FABLE, revisit=revisit, take_revisit=True)
    assert (nxt['model'], nxt['attempts'], nxt['best']) == ('fable', 2, 92.0)
    # Without --allow-fable it keeps waiting: no new Opus run over a 92% history.
    assert workqueue.next_entry(queue, attempts, [], TIERS, revisit=revisit,
                                take_revisit=True) is None


def test_sweep_lists_functions_whose_run_ended_before_the_rule_existed(tmp_path):
    p = _paths(tmp_path)
    rows = [att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 40, 'failed'),
            att(1, 'opus', 0, 100, 'exact', address='0x00401020'),
            att(1, 'opus', 0, 10, address='0x00401030')]
    for r in rows:
        workqueue.append_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS, r)
    assert workqueue.sweep(p, TIERS, '2026-11-11T00:00:00Z') == {A: 'revisit'}
    (r,) = workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)
    assert (r['address'], r['listed']) == (A, '2026-11-11T00:00:00Z')
    assert workqueue.sweep(p, TIERS, '2026-11-12T00:00:00Z') == {}


def _queue_cli(tmp_path, *args, **env):
    e = dict(os.environ, HARNESS_STATE=str(tmp_path / 'state'),
             HARNESS_DIFFICULT=str(tmp_path / 'difficult.tsv'),
             HARNESS_REVISIT_LIST=str(tmp_path / 'revisit.tsv'), **env)
    for k in ('HARNESS_SELECTION', 'HARNESS_TAKE_REVISIT', 'HARNESS_ALLOW_FABLE'):
        if k not in env:
            e.pop(k, None)
    return subprocess.run([sys.executable, os.path.join(HERE, 'queue.py'), *args], env=e,
                          capture_output=True, text=True)


def test_queue_cli_next_takes_revisit_listed_functions_only_with_the_flag(tmp_path):
    workqueue.save_queue(str(tmp_path / 'state' / 'queue.json'),
                         [dict(address=A, name='B::f', size=4, named=True, similarity=0.0)], 't')
    (tmp_path / 'revisit.tsv').write_text(
        'address\tname\tbest_score\tattempts\tlast_model\tlisted\tnote\n'
        f'{A}\tB::f\t40.00\t2\tsonnet\t2026-11-10T00:02:00Z\tstopped\n')
    r = _queue_cli(tmp_path, 'next')
    assert (r.returncode, r.stdout) == (0, '')
    r = _queue_cli(tmp_path, 'next', HARNESS_TAKE_REVISIT='1')
    assert json.loads(r.stdout)['address'] == A
    _queue_cli(tmp_path, 'release', A)
    assert '1 to revisit' in _queue_cli(tmp_path, 'show').stdout


# --- lean sessions ----------------------------------------------------------------------------

def test_lean_session_command_line_carries_the_lean_flags(tmp_path):
    if not (os.path.exists(paths.EXE) and os.path.exists(paths.FUNCTIONS_CSV)):
        pytest.skip('the original exe or the Ghidra exports are not here')
    import csv
    recorded = matched.load(paths.MATCHED_SET).get('functions', {})
    with open(paths.FUNCTIONS_CSV, newline='') as f:
        address = next(a for a in (f'0x{int(r["address"], 16):08x}' for r in csv.DictReader(f)
                                   if r['kind'] == 'function' and r['is_library'] != '1'
                                   and int(r['size'] or 0) > 16)
                       if a not in recorded)
    lean = tmp_path / 'lean.md'
    lean.write_text('Lean rules.\n')
    with _loop_harness(tmp_path) as harness:
        r = _loop(tmp_path, '--once', '--lean', str(lean), '--address', address,
                  harness=harness, HARNESS_PACK='0',
                  HARNESS_DIFFICULT=str(tmp_path / 'difficult.tsv'),
                  HARNESS_REVISIT_LIST=str(tmp_path / 'revisit.tsv'))
    assert r.returncode == 0, r.stderr + r.stdout
    (cmd,) = (tmp_path / 'state' / 'attempts').glob('*.command.txt')
    text = cmd.read_text()
    for flag in ("'--strict-mcp-config'", "'--setting-sources' 'user'",
                 "'--append-system-prompt' <" + str(tmp_path / 'state' / 'lean.md') + '>',
                 "'--settings'"):
        assert flag in text, (flag, text)
    assert '--mcp-config' not in text


def _loop(tmp_path, *args, harness=HERE, **env):
    e = dict(os.environ, HARNESS_STATE=str(tmp_path / 'state'), **env)
    for k in ('HARNESS_LEAN_INSTRUCTIONS', 'HARNESS_NO_LEAN'):
        if k not in env:
            e.pop(k, None)
    return subprocess.run(['sh', os.path.join(harness, 'loop.sh'), '--dry-run', *args],
                          capture_output=True, text=True, env=e)


@contextlib.contextmanager
def _loop_harness(tmp_path):
    """The harness folder of a checkout the loop will run in.

    The loop refuses main and develop, and a fresh clone sits on develop, so there the loop is run
    from a detached worktree of the same commit; on any other branch, from this checkout. The
    worktree holds the committed loop.sh, so on main or develop an uncommitted edit to it is not
    what runs; work on the loop happens on a work branch, where the checkout's own copy runs.
    """
    repo = os.path.dirname(HERE)
    branch = subprocess.run(['git', '-C', repo, 'rev-parse', '--abbrev-ref', 'HEAD'],
                            capture_output=True, text=True).stdout.strip()
    if branch not in ('main', 'develop'):
        yield HERE
        return
    tree = str(tmp_path / 'detached')
    subprocess.run(['git', '-C', repo, 'worktree', 'add', '-q', '--detach', tree, 'HEAD'],
                   check=True, capture_output=True)
    try:
        yield os.path.join(tree, 'harness')
    finally:
        subprocess.run(['git', '-C', repo, 'worktree', 'remove', '--force', tree],
                       capture_output=True)


def test_loop_refuses_to_start_without_a_lean_file_or_the_opt_out(tmp_path):
    r = _loop(tmp_path)
    assert r.returncode == 2 and 'no lean instruction file' in r.stderr and '--no-lean' in r.stderr
    assert not (tmp_path / 'state').exists()


def test_loop_refuses_a_lean_file_inside_the_worktree_given_either_way(tmp_path):
    inside = os.path.join(HERE, 'README.md')
    r = _loop(tmp_path, '--lean', inside)
    assert r.returncode == 2 and 'lean instruction file must lie outside this worktree' in r.stderr
    r = _loop(tmp_path, HARNESS_LEAN_INSTRUCTIONS=inside)
    assert r.returncode == 2 and 'outside this worktree' in r.stderr
    empty = tmp_path / 'lean.md'
    empty.write_text('\n')
    r = _loop(tmp_path, '--lean', str(empty))
    assert r.returncode == 2 and 'is empty' in r.stderr


def test_loop_passes_the_lean_guard_with_a_file_outside_or_the_opt_out(tmp_path):
    lean = tmp_path / 'lean.md'
    lean.write_text('Lean rules.\n')
    # The next check refuses an API key; reaching it means the lean guard let the loop on.
    for args, env in ((['--lean', str(lean)], {}), (['--no-lean'], {}),
                      ([], {'HARNESS_NO_LEAN': '1'})):
        r = _loop(tmp_path, *args, ANTHROPIC_API_KEY='sk-test-not-real', **env)
        assert r.returncode == 2 and 'ANTHROPIC_API_KEY is set' in r.stderr, (args, r.stderr)


# --- the context pack -------------------------------------------------------------------------

def _fn(address, size, name, demangled=''):
    return types.SimpleNamespace(address=address, size=size, name=name, demangled=demangled,
                                 id=f'0x{address:08x}', has_prototype='(' in demangled)


def _pack_inputs(tmp_path):
    # 0x401000 B::f calls 0x401010 B::g and 0x401020 FUN_00401020, then returns.
    code = bytes.fromhex('e80b000000' 'e816000000' 'c3') + b'\x90' * 5 + \
        bytes.fromhex('c3') + b'\x90' * 15 + bytes.fromhex('c3')
    exe = types.SimpleNamespace(read=lambda a, n: make_pe(code)[0x200 + a - 0x401000:][:n])
    fns = [_fn(0x401000, 11, 'B::f', 'void __thiscall B::f(void)'),
           _fn(0x401010, 1, 'B::g', 'int __thiscall B::g(int)'),
           _fn(0x401020, 1, 'FUN_00401020')]
    index = types.SimpleNamespace(by_address={f.address: f for f in fns})
    src = tmp_path / 'op'
    (src / 'unsorted').mkdir(parents=True)
    (src / 'unsorted' / 'B.h').write_text(
        'class B;\n\nclass B : public A {\npublic:\n\tvoid f();\n\tint g(int);\n};\n')
    (src / 'unsorted' / 'B.cpp').write_text(
        '#include "B.h"\n\n// FUNCTION: STARFLEETOP 0x00401010\nint B::g(int x)\n{\n'
        '\treturn x;\n}\n')
    decomp = tmp_path / 'decomp'
    decomp.mkdir()
    (decomp / '0x00401000.c').write_text('\nvoid __thiscall B::f(B *this)\n{\n  g(this, 1);\n}\n')
    ms = matched.empty()
    matched.record(ms, 0x401010, dict(name='B::g', score=100.0, exact=True), '2026-11-10')
    return index, exe, ms, str(src), str(decomp)


def test_pack_holds_the_pseudo_c_callees_class_and_an_exact_neighbour(tmp_path, monkeypatch):
    index, exe, ms, src, decomp = _pack_inputs(tmp_path)
    monkeypatch.setattr(pack.paths, 'REPO', str(tmp_path))
    text = pack.build(dict(address='0x00401000', name='B::f'), index, exe, ms, src_root=src,
                      decomp=decomp)
    assert text == '''## Context pack

Gathered by the harness before this session so you need not look it up. Ghidra's output is a \
reading aid with rough types and names; the original instructions from build-and-verify.sh are \
what you match.

### Ghidra's pseudo-C

```c
void __thiscall B::f(B *this)
{
  g(this, 1);
}
```

### Direct callees

- `0x00401010` `int __thiscall B::g(int)` (in `unsorted/B.cpp`)
- `0x00401020` `FUN_00401020`

### Class declarations in the headers

`unsorted/B.h`:

```cpp
class B : public A {
public:
\tvoid f();
\tint g(int);
};
```


### An exact function from the same class or unit

`0x00401010` B::g, in `unsorted/B.cpp`:

```cpp
// FUNCTION: STARFLEETOP 0x00401010
int B::g(int x)
{
\treturn x;
}
```
'''


def test_exact_sample_of_an_unnamed_function_is_shown_by_its_address(tmp_path):
    from harness.lib import source
    index, exe, ms, src, decomp = _pack_inputs(tmp_path)
    ms = matched.empty()
    matched.record(ms, 0x401030, dict(name='', score=100.0, exact=True), '2026-11-10')
    with open(os.path.join(src, 'unsorted', 'B.cpp'), 'a') as f:
        f.write('\n// FUNCTION: STARFLEETOP 0x00401030\nint h()\n{\n\treturn 0;\n}\n')
    fn = index.by_address[0x401000]
    got = pack.exact_sample(fn, index, ms, src, [(0x401000, 0x401fff, 'unsorted/B.cpp')],
                            source.scan_annotations(src, 'STARFLEETOP'))
    assert got == ('0x00401030', 'fn_00401030', 'unsorted/B.cpp',
                   '// FUNCTION: STARFLEETOP 0x00401030\nint h()\n{\n\treturn 0;\n}')


def test_pack_says_what_is_missing_and_respects_its_cap(tmp_path, monkeypatch):
    index, exe, ms, src, decomp = _pack_inputs(tmp_path)
    monkeypatch.setattr(pack.paths, 'REPO', str(tmp_path))
    os.remove(os.path.join(decomp, '0x00401000.c'))
    os.remove(os.path.join(src, 'unsorted', 'B.h'))
    text = pack.build(dict(address='0x00401000', name='B::f'), index, None, matched.empty(),
                      src_root=src, decomp=decomp)
    assert '(not exported: run ghidra/scripts/export_decomp.sh)' in text
    assert '(the exe is not available)' in text
    assert 'Not declared in any header: `B`. Search `op/` before declaring one' in text
    assert text.endswith('### An exact function from the same class or unit\n\n(none yet)\n')
    short = pack.build(dict(address='0x00401000', name='B::f'), index, None, matched.empty(),
                       src_root=src, decomp=decomp, limit=200)
    assert len(short) <= 240 and short.rstrip().endswith('more lines of the pack cut)')


def test_cut_stops_at_a_line_end_and_counts_what_it_left_out():
    assert pack.cut('a\nbb\nccc\n', 100) == 'a\nbb\nccc'
    assert pack.cut('a\nbb\nccc\ndddd', 6) == 'a\nbb\n... (2 more lines cut)'


def test_a_long_ghidra_plate_comment_is_cut_to_three_lines():
    text = '\n/* m inherited by A\n   m inherited by B\n   m inherited by C\n   m inherited by D */\n\nvoid f(void)\n'
    assert pack.short_plate(text) == ("/* m inherited by A\n   m inherited by B\n"
                                      "   ... (2 more lines of Ghidra's comment cut) */\n\n"
                                      "void f(void)\n")
    assert pack.short_plate('/* one */\nvoid f(void)\n') == '/* one */\nvoid f(void)\n'


def test_class_of_takes_the_class_part_of_a_qualified_name():
    assert pack.class_of('tPanelMgr::~tPanelMgr') == 'tPanelMgr'
    assert pack.class_of('nDatabase::tFileDatabaseMgr::Load') == 'tFileDatabaseMgr'
    assert pack.class_of('FUN_00401000') is None
    assert pack.class_of('FUN_00401000::x') is None
    assert pack.class_of('A<int(*)(int)>::f') is None
    assert pack.class_of('tList<int>::Add') == 'tList'
    assert pack.class_of('<lambda>::f') is None


def test_call_targets_follow_rel32_calls_once_each():
    # 0x401000 call 0x401010; 0x401005 call 0x401020; 0x40100a call 0x401010; ret
    code = bytes.fromhex('e80b000000' 'e816000000' 'e801000000' 'c3')
    assert pack.call_targets(code, 0x401000) == [0x401010, 0x401020]


# --- handoff notes ----------------------------------------------------------------------------

VERIFY = dict(lines=[
    'OK    build unsorted/B.cpp (1.0 s, profile game-release)',
    'OK    0x00401000 B::f 87.50% partial (16 bytes original, 16 rebuilt; 0 relocations, '
    '0 unverified); not in matched_set.json',
    'INFO      +0x3 op       original: mov eax, ecx                  rebuilt: mov eax, edx',
    'INFO    side by side: /x/diff.txt',
    'OK    0x00401010 B::g 100.00% exact (1 bytes original, 1 rebuilt; 0 relocations, '
    '0 unverified); matched_set.json has 100.00%'])


def test_handoff_section_ends_at_the_next_heading():
    out = dict(result='HANDOFF:\n- tried a local\n\n## Summary\nLong summary.\n')
    assert notes.session_account(out) == '- tried a local'


def test_note_carries_the_remaining_diff_and_the_sessions_handoff(tmp_path):
    out = dict(result='Done what I could.\n\nHANDOFF:\n- tried a local for this\n'
                      '- suspect the register choice comes from argument order\n')
    entry = dict(address='0x00401000', model='opus', attempts=0)
    notes.write(entry, 'improved', '0', '87.5', out, VERIFY, '2026-11-10T00:01:00Z',
                directory=str(tmp_path))
    assert notes.latest('0x00401000', directory=str(tmp_path)) == '''\
### Attempt 1 (opus, 2026-11-10T00:01:00Z): improved, 0.00% -> 87.50%

Remaining differences (verifier):

    OK    0x00401000 B::f 87.50% partial (16 bytes original, 16 rebuilt; 0 relocations, 0 unverified); not in matched_set.json
    INFO      +0x3 op       original: mov eax, ecx                  rebuilt: mov eax, edx

The session's handoff:

- tried a local for this
- suspect the register choice comes from argument order'''


def test_note_falls_back_to_the_end_of_the_last_message_and_keeps_the_newest_two(tmp_path):
    d = str(tmp_path)
    for n in range(3):
        notes.write(dict(address='0x00401000', model='sonnet', attempts=n), 'failed', '50', '50',
                    dict(result=f'message {n}'), None, f't{n}', directory=d)
    text = notes.latest('0x00401000', directory=d)
    assert text.count('### Attempt') == 2 and text.startswith('### Attempt 2 (sonnet, t1)')
    assert '(no HANDOFF section; the end of its last message)\nmessage 2' in text
    assert '(no verifier output)' in text
    assert notes.latest('0x00401000', directory=d, limit=150).count('### Attempt') == 1
    assert notes.latest('0x00409999', directory=d) == ''


def test_prompt_carries_pack_then_notes_then_brief(tmp_path, monkeypatch):
    template = tmp_path / 'function.md'
    template.write_text('Function {{ADDRESS}}.\n')
    brief = tmp_path / 'brief.md'
    brief.write_text('Brief.\n')
    monkeypatch.setattr(loopio.paths, 'CLAIM', str(tmp_path / 'no-claim.json'))
    monkeypatch.setenv('HARNESS_BRIEF', str(brief))
    text = loopio.prompt(dict(address='0x00401000', name='A'), template_path=str(template),
                         pack_text='## Context pack\n\nP.\n', notes_text='### Attempt 1\n')
    assert text == ('Function 0x00401000.\n\n---\n\n## Context pack\n\nP.\n\n---\n\n'
                    '## Notes from earlier attempts on this function\n\n### Attempt 1\n\n---\n\n'
                    'Brief.\n')


# --- the time box -----------------------------------------------------------------------------

def test_timebox_returns_the_commands_status_or_124_when_it_runs_over():
    assert timebox.run(5, [sys.executable, '-c', 'import sys; sys.exit(3)']) == 3
    assert timebox.run(0.3, [sys.executable, '-c', 'import time; time.sleep(30)'],
                       grace=1) == 124


@pytest.mark.parametrize('sig', ['SIGINT', 'SIGTERM', 'SIGHUP'])
def test_timebox_passes_a_signal_to_the_session_and_waits_for_it(tmp_path, sig):
    import signal
    import time
    pidfile = tmp_path / 'child.pid'
    child = ('import os, sys, time; open(sys.argv[1], "w").write(str(os.getpid())); '
             'time.sleep(60)')
    tb = subprocess.Popen([sys.executable, '-m', 'harness.lib.timebox', '0', sys.executable, '-c',
                           child, str(pidfile)], cwd=os.path.dirname(HERE),
                          env=dict(os.environ, HARNESS_TIMEBOX_GRACE='2'))
    for _ in range(100):
        if pidfile.exists() and pidfile.read_text():
            break
        time.sleep(0.05)
    pid = int(pidfile.read_text())
    tb.send_signal(getattr(signal, sig))
    assert tb.wait(timeout=10) == 128 + getattr(signal, sig)
    with pytest.raises(ProcessLookupError):
        os.kill(pid, 0)


def test_a_boxed_session_is_logged_and_noted_as_a_timeout_with_unknown_cost(tmp_path):
    # The loop's own commands after a time box: timebox (124, cut-off result), the outcome,
    # the attempt-log row and the handoff note, then the log counts it.
    root = os.path.dirname(HERE)
    out = tmp_path / 'claude.json'
    with open(out, 'w') as f:
        rc = subprocess.run([sys.executable, '-m', 'harness.lib.timebox', '0.5', 'sh', '-c',
                             'printf \'{"type":"res\'; sleep 30'], cwd=root, stdout=f,
                            env=dict(os.environ, HARNESS_TIMEBOX_GRACE='1')).returncode
    assert rc == 124 and out.read_text() == '{"type":"res'

    def loopio_cli(*args):
        return subprocess.run([sys.executable, '-m', 'harness.lib.loopio', *args], cwd=root,
                              capture_output=True, text=True, check=True).stdout.strip()
    assert loopio_cli('outcome', 'improved', '124') == 'timeout'
    assert loopio_cli('outcome', 'no-change', '124') == 'timeout'
    assert loopio_cli('outcome', 'exact', '124') == 'exact'
    assert loopio_cli('outcome', 'improved', '0') == 'improved'

    entry = tmp_path / 'entry.json'
    entry.write_text(json.dumps(dict(address='0x00401000', name='B::f', model='fable',
                                     attempts=2)))
    row = json.loads(loopio_cli('row', str(entry), 'timeout', '91', '93.5', str(out), '1800',
                                '2026-11-10T00:03:00Z'))
    assert row == dict(time='2026-11-10T00:03:00Z', address='0x00401000', name='B::f',
                       model='fable', outcome='timeout', score_before='91.00',
                       score_after='93.50', input_tokens='', output_tokens='',
                       cache_read_tokens='', cache_write_tokens='', cost_usd='', turns='',
                       seconds='1800')

    after = tmp_path / 'after.json'
    after.write_text(json.dumps(VERIFY))
    notes.write(json.loads(entry.read_text()), 'timeout', '91', '93.5', loopio.load(str(out)),
                VERIFY, '2026-11-10T00:03:00Z', directory=str(tmp_path / 'notes'))
    note = notes.latest('0x00401000', directory=str(tmp_path / 'notes'))
    assert note.startswith('### Attempt 3 (fable, 2026-11-10T00:03:00Z): timeout, 91.00% -> '
                           '93.50%\n')

    p = _paths(tmp_path)
    workqueue.record(p, dict(address='0x00401000', name='B::f'), row, FABLE)
    (logged,) = workqueue.read_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS)
    assert (logged['outcome'], logged['cost_usd'], logged['input_tokens']) == ('timeout', '', '')
    assert workqueue.counted_attempts(workqueue.read_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS),
                                      '0x00401000') == 1


def test_queue_never_takes_a_listed_boundary_error():
    class Fn:
        def __init__(self, a):
            self.address, self.id, self.name, self.size = a, f'0x{a:08x}', f'FUN_{a:08x}', 10
            self.kind, self.is_library, self.has_name, self.has_prototype = 'function', False, False, False

    class Index:
        functions = [Fn(0x401000), Fn(0x401010)]
        by_address = {f.address: f for f in functions}

    q = workqueue.build(Index(), None, {'version': 1, 'functions': {}}, [], TIERS,
                        boundaries={'0x00401010'})
    assert [e['address'] for e in q] == ['0x00401000']


def test_sweep_skips_and_unlists_functions_the_matched_set_records_exact(tmp_path):
    p = _paths(tmp_path)
    for r in [att(1, 'opus', 0, 40), att(2, 'sonnet', 40, 40, 'failed'),
              att(1, 'opus', 0, 10, address='0x00401030'),
              att(2, 'sonnet', 10, 10, 'failed', address='0x00401030')]:
        workqueue.append_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS, r)
    workqueue.append_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS, dict(
        address='0x00401040', name='C::g', attempts='3', last_model='sonnet', best_score='5.00',
        last_attempt='2026-11-09T00:00:00Z', reason='not exact after 3 attempts'))
    got = workqueue.sweep(p, TIERS, '2026-11-11T00:00:00Z', exact={'0x00401030', '0x00401040'})
    assert got == {A: 'revisit', '0x00401040': 'cleared'}
    assert workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS) == []
    assert [r['address'] for r in workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)] == [A]
