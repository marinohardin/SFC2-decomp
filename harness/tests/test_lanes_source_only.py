"""A grind lane commits source only (harness/README.md, "Lanes commit source only").

The loop runs in a scratch git repository holding a copy of the harness, with stand-ins for the
verifier, the recorder and the agent (no compiler, no network, no real clock). Expected values are
written out by hand.
"""
import json
import os
import shutil
import subprocess
import sys

import pytest

from harness.lib import loopio, matched, paths, workqueue

HARNESS = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ADDR = '0x00401000'

VERIFY_STUB = '''#!/bin/sh
case "$1" in --all) echo "RESULT OK"; exit 0 ;; esac
printf '{"status":"OK","exit":0,"lines":[],"results":[{"address":"%s","score":0.0,"exact":false}]}\\n' "$1"
'''

# Records the function in the lane's matched set (whatever HARNESS_MATCHED_SET names, as the real
# recorder does through harness/lib/paths.py), then reports exact, or a failure in 'fail' mode.
RECORD_STUB = '''#!/bin/sh
here=$(cd "$(dirname "$0")" && pwd)
PYTHONPATH="$(dirname "$here")" python3 - "$1" <<'EOF'
import json, os, sys
from harness.lib import matched, paths
a = sys.argv[1]
d = matched.load(paths.MATCHED_SET)
matched.record(d, int(a, 16), dict(name='f', symbol='?f@@YAXXZ', unit='a.cpp', size=4,
                                   score=100.0, exact=True), '2026-01-01')
matched.save(paths.MATCHED_SET, d)
code = 1 if os.environ.get('STUB_MODE') == 'fail' else 0
print(json.dumps(dict(status='OK', exit=code, lines=[], standins=[], results=[
    dict(address=a, score=100.0, exact=True)])))
EOF
'''

AGENT_STUB = '''#!/usr/bin/env python3
import json
with open('op/a.cpp', 'a') as f:
    f.write('int edited;\\n')
print(json.dumps({"type": "result", "subtype": "success", "is_error": False, "result": "done",
                  "num_turns": 1, "total_cost_usd": 0, "usage": {"input_tokens": 0,
                  "output_tokens": 0, "cache_read_input_tokens": 0,
                  "cache_creation_input_tokens": 0}}))
'''


def _git(repo, *args):
    return subprocess.run(['git', *args], cwd=repo, check=True, capture_output=True,
                          text=True).stdout


def _lane_repo(tmp_path):
    repo = tmp_path / 'repo'
    shutil.copytree(HARNESS, repo / 'harness',
                    ignore=shutil.ignore_patterns('__pycache__', 'tests', 'proof'))
    for name, text in (('build-and-verify.sh', VERIFY_STUB), ('update-matched-set.sh', RECORD_STUB)):
        (repo / 'harness' / name).write_text(text)
    (repo / 'op').mkdir()
    (repo / 'op' / 'a.cpp').write_text('int a;\n')
    (repo / 'analysis' / 'results').mkdir(parents=True)
    (repo / 'analysis' / 'results' / 'matched_set.json').write_text(
        '{"version": 1, "functions": {}}\n')
    (repo / '.gitignore').write_text('__pycache__/\n')
    agent = tmp_path / 'agent.py'
    agent.write_text(AGENT_STUB)
    agent.chmod(0o755)
    csv = tmp_path / 'functions.csv'
    csv.write_text('address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,'
                   'headers,is_library,library,kind,tags\n'
                   '00401000,4,f,,void f(void),rtti-class-recovery,,,,0,,function,\n')
    _git(repo, 'init', '-q', '-b', 'grind/lane-x')
    _git(repo, 'add', '-A')
    _git(repo, '-c', 'user.name=T', '-c', 'user.email=t@example.invalid', 'commit', '-q', '-m', 'base')
    state = tmp_path / 'state'
    workqueue.save_queue(str(state / 'queue.json'), [
        dict(address=ADDR, name='f', size=4, named=True, similarity=0.0)], 't')
    env = {k: v for k, v in os.environ.items() if not k.startswith(('HARNESS_', 'ANTHROPIC'))}
    env.update(HARNESS_STATE=str(state), HARNESS_CLAUDE=str(agent), HARNESS_NO_LEAN='1',
               HARNESS_PACK='0', HARNESS_WHOLETREE='0', HARNESS_FAIL_SLEEP='0',
               HARNESS_FUNCTIONS=str(csv), SFCOP_ROOT=str(tmp_path / 'root'),
               PYTHONDONTWRITEBYTECODE='1', GIT_AUTHOR_NAME='T', GIT_AUTHOR_EMAIL='t@example.invalid',
               GIT_COMMITTER_NAME='T', GIT_COMMITTER_EMAIL='t@example.invalid')
    return repo, state, env


def _run_loop(repo, env, **extra):
    return subprocess.run(['sh', str(repo / 'harness' / 'loop.sh'), '--once'], cwd=repo,
                          capture_output=True, text=True, env=dict(env, **extra))


def test_a_lane_commits_source_only_with_a_trailer_and_keeps_its_scores_outside(tmp_path):
    repo, state, env = _lane_repo(tmp_path)
    r = _run_loop(repo, env)
    assert r.returncode == 0, r.stdout + r.stderr
    assert _git(repo, 'rev-list', '--count', 'HEAD').strip() == '2'
    assert _git(repo, 'show', '--format=', '--name-only', 'HEAD').split() == ['op/a.cpp']
    body = _git(repo, 'log', '-1', '--format=%B').rstrip()
    assert body.splitlines()[0] == 'feat(op): match f (0x00401000)'
    assert body.splitlines()[-1] == 'Harness-Score: STARFLEETOP 0x00401000 exact'
    assert 'Co-Authored-By' not in body
    # The committed matched set and the lists are untouched; the lane's own set has the score.
    assert (repo / 'analysis/results/matched_set.json').read_text() == \
        '{"version": 1, "functions": {}}\n'
    assert _git(repo, 'status', '--porcelain') == ''
    lane = json.loads((state / 'matched_set.json').read_text())
    assert [(k, v['exact'], v['score']) for k, v in lane['functions'].items()] == \
        [(ADDR, True, 100.0)]


def test_a_round_that_is_undone_restores_the_lanes_matched_set(tmp_path):
    repo, state, env = _lane_repo(tmp_path)
    r = _run_loop(repo, env, STUB_MODE='fail')
    assert r.returncode == 0, r.stdout + r.stderr
    assert _git(repo, 'rev-list', '--count', 'HEAD').strip() == '1'
    assert _git(repo, 'status', '--porcelain') == ''
    assert json.loads((state / 'matched_set.json').read_text())['functions'] == {}


def test_the_loop_seeds_the_lane_set_from_the_committed_one_and_tops_it_up(tmp_path):
    committed = tmp_path / 'committed.json'
    lane = tmp_path / 'lane.json'
    entry = dict(name='f', symbol='s', unit='a.cpp', size=4, exact=True, recorded='d')
    committed.write_text(json.dumps({'version': 1, 'functions': {
        '0x00401000': dict(entry, score=100.0), '0x00402000': dict(entry, score=90.0, exact=False)}}))
    loopio.seed_matched(str(lane), str(committed))
    assert sorted(json.loads(lane.read_text())['functions']) == ['0x00401000', '0x00402000']
    # The lane raised one and lost track of the other: its higher score stays, the missing one
    # comes back from the committed set.
    data = json.loads(lane.read_text())
    data['functions']['0x00402000'].update(score=95.0)
    del data['functions']['0x00401000']
    lane.write_text(json.dumps(data))
    loopio.seed_matched(str(lane), str(committed))
    got = json.loads(lane.read_text())['functions']
    assert (got['0x00401000']['score'], got['0x00402000']['score']) == (100.0, 95.0)


def test_the_trailer_names_the_module_address_and_result():
    assert loopio.score_trailer('STARFLEETOP', '0x00401234', 100.0, True) == \
        'Harness-Score: STARFLEETOP 0x00401234 exact'
    assert loopio.score_trailer('STARFLEETOP', '0x401234', 97.926, False) == \
        'Harness-Score: STARFLEETOP 0x00401234 97.93'


# --- the lists --------------------------------------------------------------------------------

DIFFICULT_HEAD = '\t'.join(workqueue.DIFFICULT_FIELDS) + '\n'
REVISIT_HEAD = '\t'.join(workqueue.REVISIT_FIELDS) + '\n'


def _queue_env(tmp_path, lane, **extra):
    env = {k: v for k, v in os.environ.items() if not k.startswith('HARNESS_')}
    env.update(HARNESS_STATE=str(tmp_path / 'state'), HARNESS_LISTS=str(tmp_path / 'lists'),
               HARNESS_LANE=lane, **extra)
    return env


def _queue(tmp_path, lane, *args, **extra):
    return subprocess.run([sys.executable, os.path.join(HARNESS, 'queue.py'), *args],
                          capture_output=True, text=True, env=_queue_env(tmp_path, lane, **extra))


def _lane_file(tmp_path, lane, name, text):
    (tmp_path / 'lists' / lane).mkdir(parents=True, exist_ok=True)
    (tmp_path / 'lists' / lane / name).write_text(text)


def test_the_queue_skips_what_any_lane_listed_and_reads_the_union(tmp_path):
    entries = [dict(address=a, name=n, size=4, named=True, similarity=0.0)
               for a, n in (('0x00401000', 'A'), ('0x00402000', 'B'), ('0x00403000', 'C'))]
    workqueue.save_queue(str(tmp_path / 'state' / 'queue.json'), entries, 't')
    _lane_file(tmp_path, 'lane-a', 'difficult_functions.tsv',
               DIFFICULT_HEAD + '0x00401000\tA\t3\topus\t80.00\tt\tr\n')
    _lane_file(tmp_path, 'lane-b', 'revisit_functions.tsv',
               REVISIT_HEAD + '0x00402000\tB\t40.00\t2\tsonnet\t2026-11-10T00:02:00Z\tstopped\n')
    r = _queue(tmp_path, 'lane-c', 'next')
    assert (r.returncode, json.loads(r.stdout)['address']) == (0, '0x00403000')
    # The lane that asks for a revisit takes the other lane's revisit row, never the difficult one.
    _queue(tmp_path, 'lane-c', 'release', '0x00403000')
    r = _queue(tmp_path, 'lane-c', 'next', HARNESS_TAKE_REVISIT='1')
    assert json.loads(r.stdout)['address'] == '0x00402000'


def test_a_lane_writes_only_its_own_list_files(tmp_path):
    attempts = tmp_path / 'state' / 'attempts.tsv'
    attempts.parent.mkdir(parents=True)
    workqueue.save_queue(str(tmp_path / 'state' / 'queue.json'),
                         [dict(address='0x00401000', name='A', size=4, named=True,
                               similarity=0.0)], 't')
    _lane_file(tmp_path, 'lane-a', 'difficult_functions.tsv', DIFFICULT_HEAD)
    other = (tmp_path / 'lists' / 'lane-a' / 'difficult_functions.tsv').read_text()
    rows = [('sonnet', '0', '10'), ('opus', '10', '10')]
    for t, (model, before, after) in enumerate(rows):
        row = dict(time=f'2026-11-10T00:0{t}:00Z', address='0x00401000', name='A', model=model,
                   outcome='failed', score_before=before, score_after=after)
        r = _queue(tmp_path, 'lane-b', 'record', json.dumps(row))
        assert r.returncode == 0, r.stderr
    written = sorted(str(p.relative_to(tmp_path / 'lists')) for p in (tmp_path / 'lists').rglob('*.tsv'))
    assert written == ['lane-a/difficult_functions.tsv', 'lane-b/revisit_functions.tsv']
    assert (tmp_path / 'lists' / 'lane-a' / 'difficult_functions.tsv').read_text() == other


def test_seed_lists_copies_the_committed_lists_once(tmp_path, monkeypatch, capsys):
    repo = tmp_path / 'repo'
    (repo / 'analysis' / 'results').mkdir(parents=True)
    (repo / 'analysis' / 'results' / 'difficult_functions.tsv').write_text(
        DIFFICULT_HEAD + '0x00401000\tA\t3\topus\t80.00\tt\tr\n')
    (repo / 'analysis' / 'results' / 'revisit_functions.tsv').write_text(REVISIT_HEAD)
    _git(repo, 'init', '-q', '-b', 'develop')
    _git(repo, 'add', '-A')
    _git(repo, '-c', 'user.name=T', '-c', 'user.email=t@example.invalid', 'commit', '-q', '-m', 'x')
    import importlib.util
    spec = importlib.util.spec_from_file_location('harness_queue', os.path.join(HARNESS, 'queue.py'))
    hq = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(hq)
    monkeypatch.setattr(hq.paths, 'REPO', str(repo))
    monkeypatch.setattr(hq.paths, 'LISTS', str(tmp_path / 'lists'))
    assert hq.seed_lists('develop') == 0
    seeded = tmp_path / 'lists' / 'seed' / 'difficult_functions.tsv'
    assert seeded.read_text().splitlines()[1] == '0x00401000\tA\t3\topus\t80.00\tt\tr'
    seeded.write_text('kept')
    assert hq.seed_lists('develop') == 0
    assert seeded.read_text() == 'kept'


def test_the_default_lists_are_one_folder_per_lane_outside_the_repository():
    assert os.path.dirname(paths.DIFFICULT) == os.path.join(paths.LISTS, paths.LANE)
    assert os.path.relpath(paths.LISTS, paths.REPO).startswith('..')


# --- matched-set record and the landing step --------------------------------------------------

def test_refresh_updates_descriptive_fields_at_an_equal_score_and_never_lowers():
    data = {'version': 1, 'functions': {ADDR: dict(name='old', symbol='s', unit='u', size=4,
                                                    score=100.0, exact=True, recorded='d')}}
    new = dict(name='new', symbol='s2', unit='u', size=4, score=100.0, exact=True)
    assert matched.refresh(data, 0x401000, new) is True
    assert (data['functions'][ADDR]['name'], data['functions'][ADDR]['symbol'],
            data['functions'][ADDR]['score']) == ('new', 's2', 100.0)
    assert matched.refresh(data, 0x401000, new) is False
    lower = dict(new, name='lower', score=90.0, exact=False)
    assert matched.refresh(data, 0x401000, lower) is False
    assert data['functions'][ADDR]['name'] == 'new'
    assert matched.refresh(data, 0x402000, new) is False


def test_land_is_refused_to_agents_and_to_the_read_only_verifier():
    from harness.lib import guard
    assert guard.check_bash('harness/build-and-verify.sh --land --base develop') is not None
    r = subprocess.run(['sh', os.path.join(HARNESS, 'build-and-verify.sh'), '--land'],
                       capture_output=True, text=True)
    assert r.returncode == 2 and 'harness/landing.sh' in r.stderr
