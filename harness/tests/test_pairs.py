"""A deleting destructor and its real destructor are one piece of work for the loop:
the pairing lookup (harness/lib/pairs.py), the queue (workqueue.next_entry, park,
settle_partners, acquire_group), the claim the session may make (guard, claim snapshot, stop
hook) and the prompt."""
import json
import os
import struct
import types

from harness.lib import guard, loopio, pairs, stophook, workqueue

# 0x401000 deleting destructor of tA, 0x401020 its real destructor, 0x401040 deleting destructor
# of tB whose class symbol is missing (only its bytes say which destructor), 0x401060 tB's
# destructor, 0x401080 a deleting destructor with nothing to pair with.
DEL_A, REAL_A, DEL_B, REAL_B, LOOSE = 0x401000, 0x401020, 0x401040, 0x401060, 0x401080


def fn(address, name, mangled='', size=30, kind='function', named=True):
    return types.SimpleNamespace(address=address, name=name, mangled=mangled, size=size, kind=kind,
                                 is_library=False, has_name=named)


def index(*functions):
    return types.SimpleNamespace(functions=list(functions),
                                 by_address={f.address: f for f in functions})


def call_to(at, target):
    """push esi; mov esi, ecx; call target: the start of a deleting destructor at `at`."""
    return b'\x56\x8b\xf1\xe8' + struct.pack('<i', target - (at + 8)) + b'\x90' * 20


def world(read=True):
    idx = index(
        fn(DEL_A, "tA::`scalar deleting destructor'", '??_GtA@@UAEPAXI@Z'),
        fn(REAL_A, 'tA::~tA', '??1tA@@UAE@XZ', size=60),
        fn(DEL_B, 'fn_00401040', '', named=False),
        fn(REAL_B, 'fn_00401060', '', size=40, named=False),
        fn(LOOSE, "tC::`vector deleting destructor'", '??_EtC@@UAEPAXI@Z'))
    code = {DEL_A: call_to(DEL_A, REAL_A), DEL_B: call_to(DEL_B, REAL_B),
            LOOSE: b'\x90' * 30}
    reader = (lambda a, n: code.get(a, b'\x90' * n)[:n]) if read else None
    symbols = {a: f.mangled for a, f in idx.by_address.items() if f.mangled}
    return pairs.Resolver(idx, symbols, reader)


def test_classify_reads_the_role_kind_and_class_from_the_symbol_or_the_name():
    assert pairs.classify('??_GtThreadEvent@@UAEPAXI@Z', '') == ('deleting', 'scalar',
                                                                'tThreadEvent')
    assert pairs.classify('??_EtX@Ns@@UAEPAXI@Z', '') == ('deleting', 'vector', 'tX@Ns')
    assert pairs.classify('??1tThreadEvent@@UAE@XZ', '') == ('real', None, 'tThreadEvent')
    # The older named-member form is a deleting destructor of its class too.
    assert pairs.classify('?scalar_deleting_destructor@tVisGroup@@QAEPAV1@E@Z', '') == \
        ('deleting', 'scalar', 'tVisGroup')
    # No symbol: the name says it.
    assert pairs.classify('', 'tA::~tA') == ('real', None, 'tA')
    assert pairs.classify('', "B::A::`vector deleting destructor'") == ('deleting', 'vector',
                                                                        'A@B')
    assert pairs.classify('??0tA@@QAE@XZ', 'tA::tA') == (None, None, '')


def test_a_deleting_destructor_pairs_with_the_destructor_of_its_class():
    w = world()
    p = w.pair_of(DEL_A)
    assert (p.role, p.kind, p.partners) == ('deleting', 'scalar', [REAL_A])
    assert p.as_dict() == {'role': 'deleting', 'kind': 'scalar',
                           'how': 'class symbols and bytes', 'partners': ['0x00401020']}


def test_a_destructor_pairs_with_the_deleting_destructor_that_calls_it():
    p = world().pair_of(REAL_A)
    assert (p.role, p.partners) == ('real', [DEL_A])


def test_the_call_in_the_bytes_names_the_destructor_when_no_symbol_does():
    # Neither address has a symbol, so the class of neither is known: the bytes decide only when
    # the deleting destructor is classified; here it is named by its name.
    idx = index(fn(DEL_B, "tB::`scalar deleting destructor'", '', named=True),
                fn(REAL_B, 'fn_00401060', '', size=40, named=False))
    r = pairs.Resolver(idx, {}, lambda a, n: call_to(DEL_B, REAL_B)[:n])
    p = r.pair_of(DEL_B)
    assert (p.partners, p.how) == ([REAL_B], 'the call in its bytes')


def test_a_deleting_destructor_with_no_identifiable_destructor_is_unpaired_with_a_reason():
    got = world().pair_of(LOOSE)
    assert isinstance(got, pairs.Unpaired)
    assert 'no real destructor of its class is named' in got.reason
    assert 'no call near its start reaches a function' in got.reason
    # Without the original's bytes the reason says so.
    assert 'bytes were not read' in world(read=False).pair_of(LOOSE).reason


def test_a_class_and_a_call_that_disagree_are_unpaired():
    idx = index(fn(DEL_A, "tA::`scalar deleting destructor'", '??_GtA@@UAEPAXI@Z'),
                fn(REAL_A, 'tA::~tA', '??1tA@@UAE@XZ'),
                fn(REAL_B, 'tB::~tB', '??1tB@@UAE@XZ'))
    r = pairs.Resolver(idx, {DEL_A: '??_GtA@@UAEPAXI@Z', REAL_A: '??1tA@@UAE@XZ',
                             REAL_B: '??1tB@@UAE@XZ'}, lambda a, n: call_to(DEL_A, REAL_B)[:n])
    got = r.pair_of(DEL_A)
    assert isinstance(got, pairs.Unpaired) and 'the bytes call 0x00401060' in got.reason


def test_a_call_to_another_named_function_is_not_a_destructor():
    idx = index(fn(DEL_A, "tA::`scalar deleting destructor'", '??_GtA@@UAEPAXI@Z'),
                fn(REAL_B, 'tB::Run', '?Run@tB@@QAEXXZ'))
    r = pairs.Resolver(idx, {DEL_A: '??_GtA@@UAEPAXI@Z'}, lambda a, n: call_to(DEL_A, REAL_B)[:n])
    got = r.pair_of(DEL_A)
    assert isinstance(got, pairs.Unpaired) and 'is not a destructor' in got.reason


def test_a_destructor_without_a_deleting_destructor_and_other_functions_have_no_pair():
    idx = index(fn(REAL_A, 'tA::~tA', '??1tA@@UAE@XZ'), fn(0x1, 'tA::Run', '?Run@tA@@QAEXXZ'))
    r = pairs.Resolver(idx, {}, None)
    assert r.pair_of(REAL_A) is None and r.pair_of(0x1) is None and r.pair_of(0x99) is None


def test_the_deleting_destructors_of_one_class_are_one_group():
    idx = index(fn(DEL_A, "tA::`scalar deleting destructor'", '??_GtA@@UAEPAXI@Z'),
                fn(DEL_B, "tA::`vector deleting destructor'", '??_EtA@@UAEPAXI@Z'),
                fn(REAL_A, 'tA::~tA', '??1tA@@UAE@XZ'))
    r = pairs.Resolver(idx, {}, None)
    assert r.pair_of(DEL_A).partners == [REAL_A, DEL_B]
    assert r.pair_of(REAL_A).partners == [DEL_A, DEL_B]


# ---- the queue ----------------------------------------------------------------------------

TIERS = workqueue.Tiers.from_env({})


def q(*addrs_sizes):
    return [dict(address=f'0x{a:08x}', name=f'n{a:x}', size=s) for a, s in addrs_sizes]


def test_the_queue_hands_a_pair_to_one_session_whichever_member_comes_first():
    w = world()
    queue = q((DEL_A, 30), (REAL_A, 60))
    e = workqueue.next_entry(queue, [], [], TIERS, pairs=lambda a: w.pair_of(int(a, 16)))
    assert e['address'] == '0x00401000'
    assert e['pair']['partners'] == ['0x00401020'] and e['pair']['role'] == 'deleting'
    # The destructor's own entry waits behind the lead: nothing else is handed out.
    assert workqueue.next_entry(queue[1:], [], [], TIERS,
                                pairs=lambda a: w.pair_of(int(a, 16))) ['pair']['role'] == 'real'
    claimed = []
    e = workqueue.next_entry(queue, [], [], TIERS, claim=lambda a: claimed.append(a) or False,
                             pairs=lambda a: w.pair_of(int(a, 16)))
    assert e is None and claimed == ['0x00401000']  # the destructor was never offered
    # With the destructor first in the queue, it leads and the deleting destructor waits.
    rev = [queue[1], queue[0]]
    e = workqueue.next_entry(rev, [], [], TIERS, pairs=lambda a: w.pair_of(int(a, 16)))
    assert (e['address'], e['pair']['role'], e['pair']['partners']) == \
        ('0x00401020', 'real', ['0x00401000'])


def test_the_session_model_follows_the_largest_open_function_of_the_pair():
    w = world()
    queue = q((DEL_A, 30), (REAL_A, 600))
    pa = lambda a: w.pair_of(int(a, 16))  # noqa: E731
    assert workqueue.next_entry(queue, [], [], TIERS)['model'] == 'sonnet'  # alone: 30 bytes
    assert workqueue.next_entry(queue, [], [], TIERS, pairs=pa)['model'] == 'opus'
    # The destructor already exact: the pair needs only the small one.
    e = workqueue.next_entry(queue, [], [], TIERS, pairs=pa, exact={'0x00401020'})
    assert e['model'] == 'sonnet' and e['address'] == '0x00401000'


def test_an_exact_or_listed_partner_does_not_hold_the_other_one_back():
    w = world()
    pa = lambda a: w.pair_of(int(a, 16))  # noqa: E731
    queue = q((DEL_A, 30), (REAL_A, 60))
    done = [dict(address='0x00401000', outcome='exact', time='t', model='opus',
                 score_before='0', score_after='100')]
    assert workqueue.next_entry(queue, done, [], TIERS, pairs=pa)['address'] == '0x00401020'
    d = [dict(address='0x00401000')]
    assert workqueue.next_entry(queue, [], d, TIERS, pairs=pa)['address'] == '0x00401020'
    # On the revisit list (not taken) it is not open either.
    rv = [dict(address='0x00401000', listed='t')]
    assert workqueue.next_entry(queue, [], [], TIERS, revisit=rv, pairs=pa)['address'] == \
        '0x00401020'


def test_an_unpaired_deleting_destructor_is_parked_not_handed_out():
    w = world()
    queue = q((LOOSE, 30), (REAL_A, 60))
    parked = []
    e = workqueue.next_entry(queue, [], [], TIERS, pairs=lambda a: w.pair_of(int(a, 16)),
                             parked=parked)
    # Its reason comes back; the next function is handed out as usual (no pair: tA's deleting
    # destructor is not in this queue, but it is still the real destructor's partner).
    assert [(x['address'], 'no real destructor' in why.reason, why.transient)
                                   for x, why in parked] == \
        [('0x00401080', True, False)]
    assert e['address'] == '0x00401020'


def _paths(tmp_path):
    return types.SimpleNamespace(ATTEMPTS=str(tmp_path / 'attempts.tsv'),
                                 DIFFICULT=str(tmp_path / 'difficult.tsv'),
                                 REVISIT=str(tmp_path / 'revisit.tsv'))


def test_parking_lists_the_address_as_difficult_with_the_reason_once(tmp_path):
    p = _paths(tmp_path)
    entry = dict(address='0x00401080', name="tC::`vector deleting destructor'")
    assert workqueue.park(p, entry, 'no real destructor of its class is named') is True
    assert workqueue.park(p, entry, 'again') is False
    (r,) = workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS)
    assert (r['address'], r['attempts'], r['best_score']) == ('0x00401080', '0', '0.00')
    assert r['reason'] == ('not attempted: a deleting destructor is generated from its class\'s '
                           'real destructor and cannot be written alone, and no real destructor '
                           'of its class is named (harness/lib/pairs.py)')
    assert workqueue.next_entry([entry], [], workqueue.read_tsv(p.DIFFICULT,
                                workqueue.DIFFICULT_FIELDS), TIERS) is None


LEAD = '0x00401000'
ROW = dict(time='2026-11-10T00:01:00Z', address=LEAD, name='lead', model='sonnet',
           outcome='exact', score_before='0.00', score_after='100.00', input_tokens=1,
           output_tokens=1, cache_read_tokens=0, cache_write_tokens=0, cost_usd=1, turns=2,
           seconds=3)


def test_an_exact_pair_leaves_neither_address_to_the_queue(tmp_path):
    p = _paths(tmp_path)
    entry = dict(address=LEAD, name='lead')
    where = workqueue.record(p, entry, ROW, TIERS)
    workqueue.settle_partners(p, entry, where, ROW, [
        dict(address='0x00401020', name='tA::~tA', exact=True, score_before='0.00',
             score_after='100.00')], TIERS)
    attempts = workqueue.read_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS)
    assert [(r['address'], r['outcome']) for r in attempts] == [(LEAD, 'exact'),
                                                                ('0x00401020', 'exact')]
    w = world()
    queue = q((DEL_A, 30), (REAL_A, 60))
    assert workqueue.next_entry(queue, attempts, [], TIERS,
                                pairs=lambda a: w.pair_of(int(a, 16))) is None


def test_a_partner_that_is_not_exact_stays_open_and_leads_once_the_lead_is_done(tmp_path):
    p = _paths(tmp_path)
    entry = dict(address=LEAD, name='lead')
    where = workqueue.record(p, entry, ROW, TIERS)
    workqueue.settle_partners(p, entry, where, ROW, [
        dict(address='0x00401020', name='tA::~tA', exact=False, score_after='62.00')], TIERS)
    attempts = workqueue.read_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS)
    assert len(attempts) == 1  # no row for the partner
    w = world()
    e = workqueue.next_entry(q((DEL_A, 30), (REAL_A, 60)), attempts, [], TIERS,
                             pairs=lambda a: w.pair_of(int(a, 16)))
    assert (e['address'], e['pair']['role']) == ('0x00401020', 'real')


def test_a_lead_the_stop_rule_ends_takes_its_partners_onto_the_same_list(tmp_path):
    p = _paths(tmp_path)
    entry = dict(address=LEAD, name='lead')
    partner = dict(address='0x00401020', name='tA::~tA', exact=False, score_after='10.00')
    rows = [dict(ROW, outcome='improved', score_after='40.00', time='2026-11-10T00:01:00Z'),
            dict(ROW, outcome='failed', score_before='40.00', score_after='40.00',
                 model='opus', time='2026-11-10T00:02:00Z')]
    got = [workqueue.record(p, entry, r, TIERS) for r in rows]
    assert got == [None, 'revisit']
    workqueue.settle_partners(p, entry, got[-1], rows[-1], [partner], TIERS)
    r = workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)
    assert [(x['address'], x['best_score'], x['listed']) for x in r] == [
        (LEAD, '40.00', '2026-11-10T00:02:00Z'), ('0x00401020', '40.00', '2026-11-10T00:02:00Z')]
    assert r[1]['note'].endswith(f'(with {LEAD}, its pair)')
    # Listed twice is not listed twice.
    workqueue.settle_partners(p, entry, 'revisit', rows[-1], [partner], TIERS)
    assert len(workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)) == 2
    w = world()
    attempts = workqueue.read_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS)
    assert workqueue.next_entry(q((DEL_A, 30), (REAL_A, 60)), attempts, [], TIERS, revisit=r,
                                pairs=lambda a: w.pair_of(int(a, 16))) is None


def test_the_group_locks_are_taken_and_released_together(tmp_path):
    d = str(tmp_path / 'claims')
    assert workqueue.acquire_group(d, LEAD, ['0x00401020'], 'me', 1)
    assert sorted(os.listdir(d)) == ['0x00401000.lock', '0x00401020.lock']
    with open(os.path.join(d, '0x00401020.lock')) as f:
        assert json.load(f)['lead'] == LEAD
    # Another loop cannot take the partner, so the group is unavailable to it, and it takes none.
    assert not workqueue.acquire_group(d, '0x00401040', ['0x00401020'], 'other', 2)
    assert not os.path.exists(os.path.join(d, '0x00401040.lock'))
    workqueue.release_group(d, LEAD, 'other')   # not its lock
    assert len(os.listdir(d)) == 2
    workqueue.release_group(d, LEAD, 'me')
    assert os.listdir(d) == []


def test_a_partner_held_by_another_loop_leaves_the_lead_with_no_lock(tmp_path):
    d = str(tmp_path / 'claims')
    assert workqueue.acquire(d, '0x00401020', 'other', os.getpid())
    assert not workqueue.acquire_group(d, LEAD, ['0x00401020'], 'me', os.getpid())
    assert os.listdir(d) == ['0x00401020.lock']


# ---- the session ---------------------------------------------------------------------------

def test_the_claim_carries_the_partners_and_the_prompt_says_how_the_pair_is_written(
        tmp_path, monkeypatch):
    monkeypatch.setattr(loopio.paths, 'CLAIM', str(tmp_path / 'claim.json'))
    entry = dict(address=LEAD, name='lead', model='sonnet', size=30,
                 pair=dict(role='deleting', kind='scalar', partners=['0x00401020'],
                           states={'0x00401020': 'open'}, names={'0x00401020': 'tA::~tA'}))
    c = loopio.claim(entry, None)
    assert c['partners'] == ['0x00401020']
    text = loopio.prompt(entry)
    assert 'scalar deleting destructor of its class' in text
    assert '- 0x00401020 tA::~tA: not claimed yet; claim it as part of this session.' in text
    assert 'never written by hand' in text and '??_G' in text
    plain = loopio.prompt(dict(address=LEAD, name='lead', model='sonnet', size=30))
    assert 'deleting destructor of its class' not in plain.split('## Deleting destructors')[0]


def test_the_exact_pair_is_told_to_keep_the_exact_partner():
    got = pairs.describe(dict(role='real', partners=['0x00401000']), {'0x00401000': 'exact'},
                         {'0x00401000': "tA::`scalar deleting destructor'"})
    assert 'already exact; leave its annotation where it is' in got


def test_the_session_may_change_the_claim_lines_of_its_whole_pair(tmp_path, monkeypatch):
    claim = tmp_path / 'claim.json'
    claim.write_text(json.dumps(dict(address=LEAD, partners=['0x00401020'])))
    monkeypatch.setattr(guard.paths, 'CLAIM', str(claim))
    assert guard.claimed_addresses() == {0x401000, 0x401020}
    text = ('// FUNCTION: STARFLEETOP 0x00401000\n// FUNCTION: STARFLEETOP 0x00401020\n'
            '// FUNCTION: STARFLEETOP 0x00401999\n')
    assert guard.other_claims(text, guard.claimed_addresses()) == \
        {('FUNCTION', 'STARFLEETOP', 0x401999): 1}
    # An int still means one address.
    assert len(guard.other_claims(text, 0x401000)) == 2
    # An edit that drops the partner's line is the session's own affair, not another function's.
    new = text.replace('// FUNCTION: STARFLEETOP 0x00401020\n', '')
    p = tmp_path / 'a.cpp'
    p.write_text(text)
    ti = dict(file_path=str(p), old_string='// FUNCTION: STARFLEETOP 0x00401020\n',
              new_string='')
    assert guard.check_claims('Edit', str(p), ti) is None
    ti['old_string'] = '// FUNCTION: STARFLEETOP 0x00401999\n'
    assert 'removes or changes the claim line' in guard.check_claims('Edit', str(p), ti)
    assert new != text


def test_the_claim_snapshot_leaves_out_every_address_of_the_pair(tmp_path):
    (tmp_path / 'a.cpp').write_text(
        '// FUNCTION: STARFLEETOP 0x00401000\n// FUNCTION: STARFLEETOP 0x00401020\n'
        '// FUNCTION: STARFLEETOP 0x00401999\n')
    assert loopio.claim_lines(str(tmp_path), {0x401000, 0x401020}) == [
        'a.cpp\tFUNCTION STARFLEETOP 0x00401999']
    assert len(loopio.claim_lines(str(tmp_path), 0x401000)) == 2


def test_the_stop_hook_reports_the_partner_and_checks_its_stand_ins():
    claim = dict(address=LEAD, baseline_score=0.0, baseline_exact=False, max_blocks=3,
                 partners=['0x00401020'])
    verify = dict(exit=0, lines=[], standins=[], results=[
        dict(address=LEAD, score=100.0, exact=True),
        dict(address='0x00401020', score=62.5, exact=False)])
    allow, why = stophook.decide(claim, verify, 0)
    assert allow and why.splitlines() == ['0x00401000 is now exact', 'partner 0x00401020: 62.50%']
    verify['standins'] = [dict(function='0x00401020', target='0x1', claimed='?a', symbol='?b')]
    allow, why = stophook.decide(claim, verify, 0)
    assert not allow and 'stand-in' in why


def test_the_attempt_row_and_trailers_carry_the_scored_partners():
    entry = dict(address=LEAD, name='lead', model='sonnet',
                 pair=dict(role='deleting', partners=['0x00401020']))
    verify = dict(results=[dict(address=LEAD, score=100.0, exact=True),
                           dict(address='0x00401020', name='tA::~tA', score=100.0, exact=True)])
    r = loopio.row(entry, 'exact', '0', '100', {}, 5, 'now', verify)
    assert r['partners'] == [dict(address='0x00401020', name='tA::~tA', exact=True,
                                  score_before='0.00', score_after='100.00')]
    assert loopio.score_trailers('STARFLEETOP', LEAD, '100.00', True, entry, verify) == (
        'Harness-Score: STARFLEETOP 0x00401000 exact\n'
        'Harness-Score: STARFLEETOP 0x00401020 exact')
    # No pair, no extra keys; a partner the verify did not score is left out.
    assert 'partners' not in loopio.row(dict(address=LEAD, name='x'), 'exact', '0', '1', {}, 5,
                                        'n')
    assert loopio.partner_results(entry, dict(results=[])) == []


# ---- queue.py next, end to end -----------------------------------------------------------

def _cli(tmp_path, *args, pairs_on=True):
    import subprocess
    import sys
    env = dict(os.environ, HARNESS_STATE=str(tmp_path / 'state'),
               HARNESS_FUNCTIONS=str(tmp_path / 'functions.csv'),
               HARNESS_EXE=str(tmp_path / 'exe'), HARNESS_SYMBOL_TABLE=str(tmp_path / 'none.tsv'),
               HARNESS_MATCHED_SET=str(tmp_path / 'matched.json'),
               HARNESS_SRC_ROOT=str(tmp_path / 'src'), HARNESS_NEARMISS='0',
               HARNESS_PAIRS='1' if pairs_on else '0')
    env.pop('HARNESS_SELECTION', None)
    script = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'queue.py')
    return subprocess.run([sys.executable, script, *args], env=env, capture_output=True,
                          text=True)


def _cli_world(tmp_path):
    from conftest import make_pe
    cols = ('address,size,name,mangled,demangled,name_source,translation_unit,is_library,'
            'library,kind,tags')
    rows = ["0x00401000,30,tA::`scalar deleting destructor',??_GtA@@UAEPAXI@Z,,pdb,,0,,function,",
            "0x00401020,60,tA::~tA,??1tA@@UAE@XZ,,pdb,,0,,function,",
            "0x00401060,30,tC::`vector deleting destructor',??_EtC@@UAEPAXI@Z,,pdb,,0,,function,"]
    (tmp_path / 'functions.csv').write_text('\n'.join([cols] + rows) + '\n')
    text = bytearray(b'\x90' * 0x100)
    text[0:0x20] = call_to(0x401000, 0x401020) + b'\x90' * 4
    (tmp_path / 'exe').write_bytes(make_pe(bytes(text), text_rva=0x1000))
    (tmp_path / 'src').mkdir()
    (tmp_path / 'matched.json').write_text(json.dumps(
        {'version': 1, 'module': 'STARFLEETOP', 'functions': {}}))
    q = [dict(address=a, name=n, size=s, named=True, similarity=0.0) for a, n, s in (
        ('0x00401060', 'loose', 30), ('0x00401000', 'del', 30), ('0x00401020', 'real', 60))]
    workqueue.save_queue(str(tmp_path / 'state' / 'queue.json'), q, 'test')


def test_queue_cli_hands_out_the_pair_once_and_parks_the_unpaired_address(tmp_path):
    _cli_world(tmp_path)
    r = _cli(tmp_path, 'next')
    assert r.returncode == 0, r.stderr
    got = json.loads(r.stdout)
    assert got['address'] == '0x00401000'
    assert got['pair']['partners'] == ['0x00401020']
    assert got['pair']['states'] == {'0x00401020': 'open'}
    # tC's vector deleting destructor has no destructor: listed as difficult, with the reason.
    assert '0x00401060 listed in difficult_functions.tsv' in r.stderr
    lists = tmp_path / 'state' / 'lists' / 'state' / 'difficult_functions.tsv'
    (row,) = workqueue.read_tsv(str(lists), workqueue.DIFFICULT_FIELDS)
    assert row['address'] == '0x00401060' and 'no real destructor' in row['reason']
    # Both addresses are locked while the session runs, the partner's lock naming its lead.
    claims = tmp_path / 'state' / 'claims'
    assert sorted(os.listdir(claims)) == ['0x00401000.lock', '0x00401020.lock']
    _cli(tmp_path, 'release', '0x00401000')
    assert os.listdir(claims) == []


def test_queue_cli_with_pairs_off_hands_out_each_address_alone(tmp_path):
    _cli_world(tmp_path)
    got = json.loads(_cli(tmp_path, 'next', pairs_on=False).stdout)
    assert got['address'] == '0x00401060' and 'pair' not in got
    assert os.listdir(tmp_path / 'state' / 'claims') == ['0x00401060.lock']


def test_queue_cli_record_credits_the_exact_partner(tmp_path):
    _cli_world(tmp_path)
    row = dict(ROW, partners=[dict(address='0x00401020', name='tA::~tA', exact=True,
                                   score_before='0.00', score_after='100.00')])
    r = _cli(tmp_path, 'record', json.dumps(row))
    assert r.returncode == 0, r.stderr
    attempts = workqueue.read_tsv(str(tmp_path / 'state' / 'attempts.tsv'),
                                  workqueue.ATTEMPT_FIELDS)
    assert [(a['address'], a['outcome']) for a in attempts] == [
        (LEAD, 'exact'), ('0x00401020', 'exact')]


# ---- the review round ---------------------------------------------------------------------

def test_unreadable_bytes_make_an_unpaired_address_transient_not_a_finding():
    assert world(read=False).pair_of(LOOSE).transient is True
    assert world().pair_of(LOOSE).transient is False   # bytes read, no call: a real finding
    r = world()
    r.read = lambda a, n: None                          # the image cannot be read
    got = r.pair_of(LOOSE)
    assert got.transient and 'bytes were not read' in got.reason


def test_queue_cli_skips_an_address_whose_bytes_cannot_be_read_and_lists_nothing(tmp_path):
    _cli_world(tmp_path)
    os.remove(tmp_path / 'exe')
    r = _cli(tmp_path, 'next')
    assert r.returncode == 0, r.stderr
    assert '0x00401060 skipped for this call, not listed' in r.stderr
    assert not (tmp_path / 'state' / 'lists').exists()


def test_the_asymmetric_group_pairs_only_what_the_bytes_confirm():
    # tA has a scalar deleting destructor that calls its destructor and a vector one whose
    # bytes call something else.
    other = 0x401100
    idx = index(fn(DEL_A, "tA::`scalar deleting destructor'", '??_GtA@@UAEPAXI@Z'),
                fn(DEL_B, "tA::`vector deleting destructor'", '??_EtA@@UAEPAXI@Z'),
                fn(REAL_A, 'tA::~tA', '??1tA@@UAE@XZ'), fn(other, 'fn_x', '', named=False))
    code = {DEL_A: call_to(DEL_A, REAL_A), DEL_B: call_to(DEL_B, other)}
    r = pairs.Resolver(idx, {}, lambda a, n: code.get(a, b'\x90' * n)[:n])
    assert r.pair_of(REAL_A).partners == [DEL_A]
    assert r.pair_of(DEL_A).partners == [REAL_A]
    odd = r.pair_of(DEL_B)
    assert isinstance(odd, pairs.Unpaired) and 'the bytes call 0x00401100' in odd.reason


def test_parking_checks_under_the_lock_so_two_loops_add_one_row(tmp_path, monkeypatch):
    p = _paths(tmp_path)
    entry = dict(address='0x00401080', name='x')
    # The other loop wins between this one's check and its append.
    monkeypatch.setattr(workqueue, 'read_lists', lambda *a: [])
    assert workqueue.park(p, entry, 'r') is True
    assert workqueue.park(p, entry, 'r') is False
    assert len(workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS)) == 1
    f = str(tmp_path / 'x.tsv')
    row = dict(address='1', name='n')
    assert workqueue.append_tsv_once(f, ['address', 'name'], 'address', row) is True
    assert workqueue.append_tsv_once(f, ['address', 'name'], 'address', row) is False


def test_settle_partners_uses_the_queues_size_for_the_partners_model_order(tmp_path,
                                                                           monkeypatch):
    seen = []
    monkeypatch.setattr(workqueue, 'settle', lambda p, e, *a: seen.append(e['size']))
    p = _paths(tmp_path)
    partner = dict(address='0x00401020', name='d', exact=True, score_after='100.00')
    workqueue.settle_partners(p, dict(address=LEAD, name='l'), 'exact', ROW, [partner], TIERS,
                              sizes={'0x00401020': 60})
    assert seen == [60]


def test_judge_keeps_a_round_when_a_partner_improved_and_the_lead_did_not_get_worse():
    entry = dict(address=LEAD, partners=None, pair=dict(partners=['0x00401020']))
    base = {'0x00401020': dict(score=20.0, exact=False)}

    def verify(lead, partner, lead_exact=False, partner_exact=False, exit_=0):
        return dict(exit=exit_, results=[dict(address=LEAD, score=lead, exact=lead_exact),
                                         dict(address='0x00401020', score=partner,
                                              exact=partner_exact)])
    judge = lambda v, before='50.00': loopio.judge(entry, v, before, base)  # noqa: E731
    assert judge(verify(50.0, 60.0)) == (50.0, 'partial', 0, 1)     # partner up, lead level
    assert judge(verify(40.0, 90.0)) == (40.0, 'partial', 0, 0)     # lead worse: not kept
    assert judge(verify(50.0, 20.0)) == (50.0, 'partial', 0, 0)     # nothing improved
    assert judge(verify(60.0, 20.0)) == (60.0, 'partial', 0, 1)     # lead up
    assert judge(verify(100.0, 20.0, lead_exact=True)) == (100.0, 'exact', 0, 1)
    assert judge(verify(50.0, 100.0, partner_exact=True))[3] == 1   # partner newly exact
    # A partner below its starting score blocks the round, whatever else improved.
    assert judge(verify(60.0, 10.0))[3] == 0
    assert loopio.judge(entry, verify(100.0, 20.0, lead_exact=True), '50.00',
                        {'0x00401020': dict(score=20.0, exact=True)})[3] == 0
    # No pair: the lead alone decides, as before.
    assert loopio.judge(dict(address=LEAD), verify(50.0, 99.0), '50.00')[3] == 0


def test_the_stop_hook_lets_go_when_only_a_partner_improved():
    claim = dict(address=LEAD, baseline_score=50.0, baseline_exact=False, max_blocks=3,
                 partners=['0x00401020'],
                 partner_baselines={'0x00401020': dict(score=20.0, exact=False)})
    v = dict(exit=0, lines=[], standins=[], results=[
        dict(address=LEAD, score=50.0, exact=False),
        dict(address='0x00401020', score=60.0, exact=False)])
    assert stophook.decide(claim, v, 0)[0] is True
    v['results'][1]['score'] = 20.0
    assert stophook.decide(claim, v, 0)[0] is False


def test_the_claim_records_each_partners_starting_score(tmp_path, monkeypatch):
    monkeypatch.setattr(loopio.paths, 'CLAIM', str(tmp_path / 'claim.json'))
    ms = tmp_path / 'ms.json'
    ms.write_text(json.dumps({'version': 1, 'module': 'STARFLEETOP', 'functions': {
        '0x00401020': dict(name='d', score=33.0, exact=False)}}))
    monkeypatch.setattr(loopio.paths, 'MATCHED_SET', str(ms))
    c = loopio.claim(dict(address=LEAD, name='l', pair=dict(partners=['0x00401020',
                                                                     '0x00401040'])), None)
    assert c['partner_baselines'] == {'0x00401020': dict(score=33.0, exact=False),
                                      '0x00401040': dict(score=0.0, exact=False)}


def test_the_entry_points_the_loop_calls_take_several_ids(tmp_path, monkeypatch, capsys):
    src = tmp_path / 'src'
    src.mkdir()
    (src / 'a.cpp').write_text('// FUNCTION: STARFLEETOP 0x00401000\n'
                               '// FUNCTION: STARFLEETOP 0x00401020\n'
                               '// STUB: STARFLEETOP 0x00401040\n'
                               '// FUNCTION: STARFLEETOP 0x00401999\n')
    monkeypatch.setattr(loopio.paths, 'SRC_ROOT', str(src))
    monkeypatch.setattr(loopio.source, 'load_units', lambda: dict(module='STARFLEETOP'))
    entry = tmp_path / 'entry.json'
    entry.write_text(json.dumps(dict(address=LEAD, pair=dict(
        partners=['0x00401020', '0x00401040', '0x00401060']))))
    assert loopio.main(['ids', str(entry)]) == 0
    assert capsys.readouterr().out.split() == [LEAD, '0x00401020']   # a STUB is no claim
    assert loopio.main(['group', str(entry)]) == 0
    assert capsys.readouterr().out.split() == [LEAD, '0x00401020', '0x00401040', '0x00401060']
    assert loopio.main(['claims', str(src), LEAD, '0x00401020', '0x00401040']) == 0
    assert capsys.readouterr().out.split('\n')[0] == 'a.cpp\tFUNCTION STARFLEETOP 0x00401999'
    # The updater and the verifier take every id.
    from harness.lib import verify
    got = []
    monkeypatch.setattr(verify, 'run', lambda ids, **k: got.append(list(ids)) or verify.Report())
    verify.main([LEAD, '0x00401020', '--json'])
    assert got == [[LEAD, '0x00401020']]
