"""Tests for tools/unlanded.py.

The matched sets are invented; the expected lists are written out by hand. The repository test
builds a throwaway git repository under pytest's tmp_path, with no remote and no network.
"""
import importlib.util
import io
import json
import pathlib
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]


def _load():
    spec = importlib.util.spec_from_file_location('unlanded', ROOT / 'tools/unlanded.py')
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


unlanded = _load()


def test_a_function_exact_on_the_branch_and_not_exact_on_the_base_is_listed():
    base = {
        '0x00401000': {'exact': True, 'score': 100.0},
        '0x00402000': {'exact': False, 'score': 80.0},
    }
    branch = {
        '0x00401000': {'exact': True, 'score': 100.0},    # exact on both: landed
        '0x00402000': {'exact': True, 'score': 100.0},    # partial on the base, exact here
        '0x00403000': {'exact': True, 'score': 100.0},    # not on the base at all
        '0x00404000': {'exact': False, 'score': 95.0},    # not exact here: not counted
    }
    assert unlanded.unlanded(base, branch) == ['0x00402000', '0x00403000']


def test_a_branch_with_nothing_new_lists_nothing():
    base = {'0x00401000': {'exact': True}}
    assert unlanded.unlanded(base, {}) == []
    assert unlanded.unlanded(base, {'0x00401000': {'exact': True}}) == []


def _git(repo, *args):
    subprocess.run(['git', '-c', 'user.name=Test', '-c', 'user.email=test@example.invalid',
                    *args], cwd=repo, check=True, capture_output=True)


def _commit_matched_set(repo, functions, message):
    path = repo / 'analysis/results/matched_set.json'
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps({'version': 1, 'functions': functions}))
    _git(repo, 'add', '-A')
    _git(repo, 'commit', '-q', '-m', message)


def test_report_lists_each_branch_with_unlanded_work_and_filters_by_prefix(tmp_path):
    repo = tmp_path / 'repo'
    repo.mkdir()
    _git(repo, 'init', '-q', '-b', 'develop')
    _commit_matched_set(repo, {'0x00401000': {'exact': True}}, 'base')
    _git(repo, 'checkout', '-q', '-b', 'grind/lane-a')
    _commit_matched_set(repo, {'0x00401000': {'exact': True}, '0x00405000': {'exact': True},
                               '0x00406000': {'exact': True}}, 'lane a')
    _git(repo, 'checkout', '-q', '-b', 'grind/lane-b', 'develop')
    _commit_matched_set(repo, {'0x00401000': {'exact': True}, '0x00407000': {'exact': False}},
                        'lane b, nothing exact')
    _git(repo, 'checkout', '-q', '-b', 'feat/other', 'develop')
    _commit_matched_set(repo, {'0x00401000': {'exact': True}, '0x00408000': {'exact': True}},
                        'other')

    out = io.StringIO()
    assert unlanded.report('develop', ['grind/'], cwd=repo, out=out) == 1
    assert out.getvalue() == ('grind/lane-a: 2 exact not in develop (not pushed): '
                              '0x00405000 0x00406000\n')

    out = io.StringIO()
    assert unlanded.report('develop', [], cwd=repo, out=out) == 2
    assert out.getvalue().splitlines() == [
        'feat/other: 1 exact not in develop (not pushed): 0x00408000',
        'grind/lane-a: 2 exact not in develop (not pushed): 0x00405000 0x00406000',
    ]


def _two_lane_repo(tmp_path):
    repo = tmp_path / 'repo'
    repo.mkdir()
    _git(repo, 'init', '-q', '-b', 'develop')
    _commit_matched_set(repo, {'0x00401000': {'exact': True}}, 'base')
    _git(repo, 'checkout', '-q', '-b', 'grind/lane-a')
    _commit_matched_set(repo, {'0x00405000': {'exact': True}}, 'lane a')
    _git(repo, 'checkout', '-q', '-b', 'grind/lane-b', 'develop')
    _commit_matched_set(repo, {'0x00406000': {'exact': True}}, 'lane b')
    return repo


def test_a_disposed_branch_is_listed_but_not_counted(tmp_path):
    repo = _two_lane_repo(tmp_path)
    out = io.StringIO()
    assert unlanded.report('develop', [], cwd=repo, out=out, disposed={'grind/lane-b'}) == 1
    assert out.getvalue().splitlines() == [
        'grind/lane-a: 1 exact not in develop (not pushed): 0x00405000',
        'grind/lane-b: 1 exact not in develop (not pushed, disposed): 0x00406000',
    ]


def test_main_exits_0_when_every_listed_branch_is_disposed_and_1_otherwise(tmp_path, monkeypatch,
                                                                         capsys):
    repo = _two_lane_repo(tmp_path)
    monkeypatch.chdir(repo)
    assert unlanded.main(['--base', 'develop']) == 1
    assert unlanded.main(['--base', 'develop', '--disposed', 'grind/lane-a',
                          '--disposed', 'grind/lane-b']) == 0
    assert unlanded.main(['--base', 'no-such-branch']) == 2
    assert 'no-such-branch has no analysis/results/matched_set.json' in capsys.readouterr().err


def test_a_matched_set_that_is_not_json_is_an_error_not_a_finding(tmp_path, monkeypatch, capsys):
    repo = _two_lane_repo(tmp_path)
    (repo / 'analysis/results/matched_set.json').write_text('{"functions": <<<<<<< HEAD\n')
    _git(repo, 'commit', '-q', '-am', 'a conflict marker')
    monkeypatch.chdir(repo)
    assert unlanded.main(['--base', 'develop']) == 2
    assert 'grind/lane-b:analysis/results/matched_set.json is not valid JSON' in \
        capsys.readouterr().err
