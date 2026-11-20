"""seed-lists after the files left the repository; what settle reports when a row is another
lane's; and the invariants of one lane's two lists."""
import importlib.util
import json
import os
import subprocess

from harness.lib import paths, workqueue

HARNESS = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
A = '0x00401000'
DHEAD = '\t'.join(workqueue.DIFFICULT_FIELDS) + '\n'
RHEAD = '\t'.join(workqueue.REVISIT_FIELDS) + '\n'


def _git(repo, *args):
    return subprocess.run(['git', '-c', 'user.name=T', '-c', 'user.email=t@example.invalid',
                           *args], cwd=repo, check=True, capture_output=True, text=True).stdout


def _load_queue_module():
    spec = importlib.util.spec_from_file_location('harness_queue', os.path.join(HARNESS, 'queue.py'))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def test_seed_lists_reads_the_last_commit_that_still_had_a_deleted_file(tmp_path, monkeypatch,
                                                                       capsys):
    repo = tmp_path / 'repo'
    results = repo / 'analysis' / 'results'
    results.mkdir(parents=True)
    (results / 'difficult_functions.tsv').write_text(DHEAD + f'{A}\tA\t3\topus\t80.00\tt\tr\n')
    (results / 'revisit_functions.tsv').write_text(RHEAD)
    _git(repo, 'init', '-q', '-b', 'develop')
    _git(repo, 'add', '-A')
    _git(repo, 'commit', '-q', '-m', 'lists')
    (results / 'difficult_functions.tsv').unlink()
    (results / 'revisit_functions.tsv').unlink()
    (results / 'other.txt').write_text('x')
    _git(repo, 'add', '-A')
    _git(repo, 'commit', '-q', '-m', 'lists leave the repository')
    hq = _load_queue_module()
    monkeypatch.setattr(hq.paths, 'REPO', str(repo))
    monkeypatch.setattr(hq.paths, 'LISTS', str(tmp_path / 'lists'))
    assert hq.seed_lists('develop') == 0
    said = capsys.readouterr().out
    first = _git(repo, 'rev-list', '--max-parents=0', 'develop').strip()[:12]
    assert first in said and 'difficult_functions.tsv from' in said
    seeded = tmp_path / 'lists' / 'seed' / 'difficult_functions.tsv'
    assert seeded.read_text().splitlines()[1] == f'{A}\tA\t3\topus\t80.00\tt\tr'
    seeded.write_text('kept')
    assert hq.seed_lists('develop') == 0
    assert seeded.read_text() == 'kept'


def test_seed_lists_with_no_commit_that_had_the_file_is_an_error(tmp_path, monkeypatch):
    repo = tmp_path / 'repo'
    repo.mkdir()
    (repo / 'x').write_text('x')
    _git(repo, 'init', '-q', '-b', 'develop')
    _git(repo, 'add', '-A')
    _git(repo, 'commit', '-q', '-m', 'x')
    hq = _load_queue_module()
    monkeypatch.setattr(hq.paths, 'REPO', str(repo))
    monkeypatch.setattr(hq.paths, 'LISTS', str(tmp_path / 'lists'))
    assert hq.seed_lists('develop') == 2


# --- settle's report ----------------------------------------------------------------------------

class P:
    """The paths settle reads: this lane's two files and the others', under tmp_path."""

    def __init__(self, tmp_path, lane='lane-b'):
        self.root = tmp_path / 'lists'
        self.ATTEMPTS = str(tmp_path / 'attempts.tsv')
        self.DIFFICULT = str(self.root / lane / 'difficult_functions.tsv')
        self.REVISIT = str(self.root / lane / 'revisit_functions.tsv')

    def difficult_files(self):
        return [self.DIFFICULT] + sorted(
            str(p) for p in self.root.glob('*/difficult_functions.tsv') if str(p) != self.DIFFICULT)

    def revisit_files(self):
        return [self.REVISIT] + sorted(
            str(p) for p in self.root.glob('*/revisit_functions.tsv') if str(p) != self.REVISIT)


def _exact_attempt(p):
    workqueue.append_tsv(p.ATTEMPTS, workqueue.ATTEMPT_FIELDS, dict(
        time='2026-11-12T00:00:00Z', address=A, name='A', model='opus', outcome='exact',
        score_before='90', score_after='100'))


def _revisit_row(lane_dir, name='revisit_functions.tsv'):
    lane_dir.mkdir(parents=True, exist_ok=True)
    (lane_dir / name).write_text(RHEAD + f'{A}\tA\t90.00\t2\topus\t2026-11-10T00:00:00Z\tstopped\n')


def test_an_exact_function_listed_by_this_lane_is_cleared_from_its_file(tmp_path):
    p = P(tmp_path)
    _revisit_row(tmp_path / 'lists' / 'lane-b')
    _exact_attempt(p)
    tiers = workqueue.Tiers.from_env()
    assert workqueue.settle(p, dict(address=A, name='A', size=4), tiers, 't') == 'cleared'
    assert workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS) == []


def test_an_exact_function_listed_by_another_lane_is_left_there_and_says_so(tmp_path):
    p = P(tmp_path)
    _revisit_row(tmp_path / 'lists' / 'lane-a')
    _revisit_row(tmp_path / 'lists' / 'seed')
    _exact_attempt(p)
    tiers = workqueue.Tiers.from_env()
    got = workqueue.settle(p, dict(address=A, name='A', size=4), tiers, 't')
    assert got == 'left:lane-a'
    # Nothing was removed from the other lane's file.
    other = tmp_path / 'lists' / 'lane-a' / 'revisit_functions.tsv'
    assert workqueue.read_tsv(str(other), workqueue.REVISIT_FIELDS)[0]['address'] == A


def test_sweep_reports_clearing_per_lane(tmp_path):
    p = P(tmp_path)
    _revisit_row(tmp_path / 'lists' / 'lane-b')
    _revisit_row(tmp_path / 'lists' / 'lane-a')
    tiers = workqueue.Tiers.from_env()
    got = workqueue.sweep(p, tiers, 't', exact={A})
    assert got == {A: 'cleared'}
    assert workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS) == []
    other = tmp_path / 'lists' / 'lane-a' / 'revisit_functions.tsv'
    assert len(workqueue.read_tsv(str(other), workqueue.REVISIT_FIELDS)) == 1


# --- one lane's own lists -----------------------------------------------------------------------

def _record(p, **kw):
    row = dict(time='2026-11-10T00:00:00Z', address=A, name='A', outcome='failed',
               score_before='0', score_after='10')
    row.update(kw)
    return workqueue.record(p, dict(address=A, name='A', size=4), row, workqueue.Tiers.from_env())


def test_a_lanes_lists_hold_an_address_once_and_never_on_both(tmp_path):
    p = P(tmp_path)
    rows = [dict(model='sonnet', time='2026-11-10T00:00:00Z', score_after='10'),
            dict(model='opus', time='2026-11-10T00:01:00Z', score_after='10'),   # stop rule: revisit
            dict(model='opus', time='2026-11-10T00:02:00Z', score_after='10'),
            dict(model='opus', time='2026-11-10T00:03:00Z', score_after='10')]
    for r in rows:
        _record(p, **r)
        d = [x['address'] for x in workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS)]
        v = [x['address'] for x in workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)]
        assert len(d) == len(set(d)) and len(v) == len(set(v))
        assert not set(d) & set(v)
    # A revisit run that ends short again moves it to the difficult list.
    _record(p, model='opus', time='2026-11-10T00:04:00Z', score_after='10', outcome='failed')
    v = workqueue.read_tsv(p.REVISIT, workqueue.REVISIT_FIELDS)
    d = workqueue.read_tsv(p.DIFFICULT, workqueue.DIFFICULT_FIELDS)
    assert len(v) + len(d) == 1
