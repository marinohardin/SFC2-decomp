"""Lane detection (harness/lib/lane.py), shared by scripts/check.sh and tests/test_manifest.py."""
import os
import subprocess
import sys

from harness.lib import lane

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def test_the_loops_variable_wins_whatever_the_branch():
    assert lane.on_lane({'SFCOP_LANE': '1'}, branch='develop') is True
    assert lane.on_lane({'SFCOP_LANE': '0'}, branch='develop') is False


def test_a_detached_ci_checkout_is_a_lane_by_the_pull_requests_head_ref():
    assert lane.on_lane({'GITHUB_HEAD_REF': 'grind/tiger-e'}, branch='HEAD') is True
    assert lane.on_lane({'GITHUB_HEAD_REF': 'feat/x'}, branch='HEAD') is False
    assert lane.on_lane({'GITHUB_HEAD_REF': ''}, branch='HEAD') is False


def test_the_branch_name_decides_last():
    assert lane.on_lane({}, branch='grind/tiger-e') is True
    assert lane.on_lane({}, branch='feat/lanes') is False


def test_the_command_line_that_check_sh_runs_exits_0_on_a_lane():
    env = {k: v for k, v in os.environ.items() if k not in ('SFCOP_LANE', 'GITHUB_HEAD_REF')}
    cmd = [sys.executable, '-m', 'harness.lib.lane']
    assert subprocess.run(cmd, cwd=REPO, env=dict(env, GITHUB_HEAD_REF='grind/x')).returncode == 0
    assert subprocess.run(cmd, cwd=REPO, env=dict(env, SFCOP_LANE='1')).returncode == 0
