"""tools/unlanded.py finds a lane that committed source only, by its Harness-Score trailers.

Throwaway git repositories under pytest's tmp_path, no remote, no network. Expected lists are
written out by hand.
"""
import importlib.util
import io
import json
import pathlib
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]
_spec = importlib.util.spec_from_file_location('unlanded', ROOT / 'tools/unlanded.py')
unlanded = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(unlanded)


def _git(repo, *args):
    subprocess.run(['git', '-c', 'user.name=Test', '-c', 'user.email=test@example.invalid',
                    *args], cwd=repo, check=True, capture_output=True)


def _commit_matched_set(repo, functions, message):
    path = repo / 'analysis/results/matched_set.json'
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps({'version': 1, 'functions': functions}))
    _git(repo, 'add', '-A')
    _git(repo, 'commit', '-q', '-m', message)


def _commit_source(repo, name, trailer):
    (repo / name).write_text(trailer)
    _git(repo, 'add', '-A')
    _git(repo, 'commit', '-q', '-m', 'feat(op): match f (0x00405000)',
         '-m', 'build-and-verify: 0% -> 100% (exact) with opus.', '-m', trailer)


def _repo(tmp_path):
    repo = tmp_path / 'repo'
    repo.mkdir()
    _git(repo, 'init', '-q', '-b', 'develop')
    _commit_matched_set(repo, {'0x00401000': {'exact': True}}, 'base')
    return repo


def test_a_lane_that_committed_source_only_is_found_by_its_trailers(tmp_path):
    repo = _repo(tmp_path)
    _git(repo, 'checkout', '-q', '-b', 'grind/lane-c')
    _commit_source(repo, 'a.cpp', 'Harness-Score: STARFLEETOP 0x00405000 exact')
    _commit_source(repo, 'b.cpp', 'Harness-Score: STARFLEETOP 0x00406000 97.93')   # not exact
    _commit_source(repo, 'c.cpp', 'Harness-Score: STARFLEETOP 0x00407abc exact')
    out = io.StringIO()
    assert unlanded.report('develop', ['grind/'], cwd=repo, out=out) == 1
    assert out.getvalue() == ('grind/lane-c: 2 exact not in develop (not pushed): '
                              '0x00405000 0x00407abc\n')


def test_a_trailer_branch_has_landed_once_the_base_matched_set_has_it_exact(tmp_path):
    repo = _repo(tmp_path)
    _git(repo, 'checkout', '-q', '-b', 'grind/lane-c')
    _commit_source(repo, 'a.cpp', 'Harness-Score: STARFLEETOP 0x00405000 exact')
    # A squash merge: develop gets one new commit with the matched set, and the lane's own
    # commits stay unmerged.
    _git(repo, 'checkout', '-q', 'develop')
    _commit_matched_set(repo, {'0x00401000': {'exact': True}, '0x00405000': {'exact': True}},
                        'squash')
    out = io.StringIO()
    assert unlanded.report('develop', ['grind/'], cwd=repo, out=out) == 0
    assert out.getvalue() == ''


def test_a_branch_with_both_kinds_of_evidence_lists_the_union(tmp_path):
    repo = _repo(tmp_path)
    _git(repo, 'checkout', '-q', '-b', 'grind/old-lane')
    _commit_matched_set(repo, {'0x00401000': {'exact': True}, '0x00409000': {'exact': True}},
                        'old style')
    _commit_source(repo, 'a.cpp', 'Harness-Score: STARFLEETOP 0x00405000 exact')
    out = io.StringIO()
    assert unlanded.report('develop', ['grind/'], cwd=repo, out=out) == 1
    assert out.getvalue() == ('grind/old-lane: 2 exact not in develop (not pushed): '
                              '0x00405000 0x00409000\n')
