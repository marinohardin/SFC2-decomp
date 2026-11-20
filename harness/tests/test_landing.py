"""The landing step (harness/landing.sh, `verify.py --land`): records every claim and never
lowers a recorded score silently. Invented matched sets; expected values written by hand."""
import os
import shutil
import subprocess

import pytest

from conftest import FakeIndex
from harness.lib import funcs, matched, paths, verify


def _entry(score, exact=False):
    return dict(name='A::f', symbol='?f@A@@QAEXXZ', unit='A.cpp', size=16, score=score,
                exact=exact, recorded='2026-11-01')


def test_a_landing_that_lowers_a_recorded_score_reports_it_and_writes_nothing(tmp_path,
                                                                             monkeypatch):
    (tmp_path / 'game').mkdir()
    monkeypatch.setattr(paths, 'SRC_ROOT', str(tmp_path / 'game'))
    monkeypatch.setattr(paths, 'MATCHED_SET', str(tmp_path / 'matched_set.json'))
    monkeypatch.setattr(paths, 'BOUNDARIES', str(tmp_path / 'boundary_errors.tsv'))
    monkeypatch.setattr(funcs, 'FunctionIndex', lambda *a: FakeIndex())
    matched.save(paths.MATCHED_SET, {'version': 1, 'functions': {'0x00401000': _entry(95.0)}})
    before = (tmp_path / 'matched_set.json').read_text()
    base = ({'version': 1, 'functions': {'0x00401000': _entry(99.0)}}, 'develop')
    rep = verify.run([], check_all=True, record=True, base=base, land=True)
    assert 0x00401000 in rep.regressions
    assert any('REGRESSION: matched_set.json lowers to 95.00% the entry in develop' in ln
               for ln in rep.lines)
    assert (tmp_path / 'matched_set.json').read_text() == before


def test_land_verifies_every_claim_records_and_holds_the_run_to_the_base(tmp_path, monkeypatch):
    calls = {}

    def fake_run(ids, **kw):
        calls.update(ids=ids, **kw)
        rep = verify.Report()
        rep.regressions.append(0x00401000)
        return rep
    monkeypatch.setattr(verify, 'run', fake_run)
    monkeypatch.setattr(verify, 'claimed_keys', lambda cfg=None: {'0x00402000', '0x00401000'})
    monkeypatch.setattr(verify, 'load_base', lambda spec: ({'version': 1, 'functions': {}}, spec))
    monkeypatch.setattr(verify, 'load_base_sizes', lambda spec: None)
    monkeypatch.setattr(paths, 'BUILD', str(tmp_path / 'build'))
    assert verify.main(['--land', '--base', 'develop']) == verify.EXIT_REGRESSION
    assert calls['ids'] == ['0x00401000', '0x00402000']
    assert (calls['check_all'], calls['record'], calls['land']) == (True, True, True)
    assert calls['base'][1] == 'develop'
    with pytest.raises(SystemExit):
        verify.main(['--land'])


LANDING = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'landing.sh')


def _landing(tmp_path, fail=None):
    """Runs landing.sh in a scratch checkout whose python3 only logs what it was asked to run."""
    repo = tmp_path / 'repo'
    (repo / 'harness').mkdir(parents=True)
    shutil.copy(LANDING, repo / 'harness' / 'landing.sh')
    bin_dir = tmp_path / 'bin'
    bin_dir.mkdir()
    shim = bin_dir / 'python3'
    shim.write_text('#!/bin/sh\necho "$*" >> "%s"\ncase "$*" in *"%s"*) exit 1 ;; esac\n'
                    % (tmp_path / 'log', fail or 'no such command'))
    shim.chmod(0o755)
    env = {k: v for k, v in os.environ.items() if k not in ('HARNESS_MATCHED_SET', 'SFCOP_LANE')}
    env['PATH'] = f'{bin_dir}:{env["PATH"]}'
    r = subprocess.run(['sh', str(repo / 'harness' / 'landing.sh'), 'refs/base'], cwd=repo,
                       env=env, capture_output=True, text=True)
    log = (tmp_path / 'log').read_text().splitlines() if (tmp_path / 'log').exists() else []
    return r.returncode, log


def test_landing_syncs_every_claim_and_checks_the_table_before_the_manifest(tmp_path):
    code, log = _landing(tmp_path)
    assert code == 0
    assert log == ['-m harness.lib.verify --land --base refs/base',
                   'tools/symbols.py claim'] + [log[2]] + [
                   'tools/symbols.py check', 'tools/manifest.py']
    # the seed runs in keep-existing mode: no --reseed, its report in a scratch file
    assert log[2].startswith('analysis/scripts/seed_symbols.py --report ')
    assert '--reseed' not in log[2]


def test_landing_stops_when_the_table_is_not_in_sync_after_the_claim_sync(tmp_path):
    code, log = _landing(tmp_path, fail='symbols.py check')
    assert code == 2
    assert log[-1] == 'tools/symbols.py check' and 'tools/manifest.py' not in log


@pytest.mark.parametrize('var', ['HARNESS_MATCHED_SET', 'SFCOP_LANE'])
def test_landing_refuses_a_lanes_environment(var):
    import os
    import subprocess
    script = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'landing.sh')
    env = {k: v for k, v in os.environ.items() if k not in ('HARNESS_MATCHED_SET', 'SFCOP_LANE')}
    r = subprocess.run(['sh', script, 'develop'], capture_output=True, text=True,
                       env=dict(env, **{var: '/tmp/x'}))
    assert r.returncode == 2 and f'ERROR {var} is set' in r.stderr


def test_landing_stops_when_the_seed_fails(tmp_path):
    code, log = _landing(tmp_path, fail='seed_symbols')
    assert code == 2 and not any('symbols.py check' in x or 'manifest' in x for x in log)
