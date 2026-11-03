"""What a function session may edit and run, and when it may stop."""
import os

import pytest

from harness.lib import guard, paths, stophook


@pytest.fixture(autouse=True)
def roots(tmp_path, monkeypatch):
    monkeypatch.setattr(paths, 'SRC_ROOT', os.path.join(paths.REPO, 'game'))
    monkeypatch.setattr(paths, 'STATE', str(tmp_path / 'state'))


def edit(p):
    return guard.decide({'tool_name': 'Edit', 'tool_input': {'file_path': p}})


def bash(c):
    return guard.decide({'tool_name': 'Bash', 'tool_input': {'command': c}})


def test_source_under_the_rebuilt_tree_may_be_edited():
    assert edit(os.path.join(paths.REPO, 'game', 'Source', 'Ship.cpp')) is None
    assert guard.decide({'tool_name': 'Write', 'cwd': paths.REPO,
                         'tool_input': {'file_path': 'game/Source/Ship.h'}}) is None


@pytest.mark.parametrize('rel', ['analysis/results/matched_set.json',
                                 'analysis/results/difficult_functions.tsv', 'harness/lib/verify.py',
                                 'toolchain/bin/cl', 'analysis/results/retail_hashes.sha256',
                                 'ghidra/exports/functions.csv', '.claude/settings.json',
                                 'scripts/check.sh', 'README.md'])
def test_judging_files_and_anything_outside_the_tree_are_denied(rel):
    assert edit(os.path.join(paths.REPO, rel)) is not None


def test_queue_listings_and_objects_are_denied():
    assert edit(os.path.join(paths.STATE, 'queue.json')) is not None
    assert edit(os.path.join(paths.REPO, 'game', 'Source', 'Ship.asm')) is not None
    assert edit(os.path.join(paths.REPO, 'game', 'Source', 'Ship.obj')) is not None


def test_symlink_out_of_the_tree_is_followed(tmp_path):
    link = tmp_path / 'link.json'
    link.symlink_to(paths.MATCHED_SET)
    assert edit(str(link)) is not None


@pytest.mark.parametrize('cmd', ['harness/build-and-verify.sh 0x006fc690',
                                 'sh harness/build-and-verify.sh 0x006fc690 --json 2>&1 | tail -5',
                                 'grep -n Instance game/Source/A.cpp', 'git diff', 'ls game',
                                 'cat harness/README.md'])
def test_verify_and_read_only_commands_are_allowed(cmd):
    assert bash(cmd) is None


@pytest.mark.parametrize('cmd', ['harness/build-and-verify.sh 0x006fc690 --record',
                                 'harness/update-matched-set.sh 0x006fc690',
                                 'python3 -m harness.lib.verify --record 0x006fc690',
                                 'game/harness/build-and-verify.sh 0x006fc690',
                                 'rm -rf harness', 'echo {} > harness/matched_set.json',
                                 'cat $(ls)', 'git commit -am x', 'git checkout -- game',
                                 "sed -i '' s/a/b/ game/A.cpp", 'find game -delete',
                                 'HARNESS_MATCHED_SET=/tmp/m.json harness/build-and-verify.sh 0x1',
                                 'env | grep ANTHROPIC_API_KEY', 'pkill -9 bash', 'docker ps',
                                 "awk 'BEGIN{system(\"id\")}'", 'less harness/README.md'])
def test_writes_and_other_commands_are_denied(cmd):
    assert bash(cmd) is not None


# Each of these wrote a file through an allowed command in review of the first version.
@pytest.mark.parametrize('cmd', ['true & touch harness/x',
                                 'ls |& touch harness/x',
                                 'sort -o harness/lib/verify.py game/a.cpp',
                                 'sort -uo harness/lib/verify.py game/a.cpp',
                                 'sort --output=harness/lib/verify.py game/a.cpp',
                                 'sed -n "w harness/lib/verify.py" game/a.cpp',
                                 'sed -n "s/a/b/w harness/x" game/a.cpp',
                                 'sed -n "1e touch x" game/a.cpp',
                                 'sed -n -f script.sed game/a.cpp',
                                 'git diff --output=harness/x',
                                 'git log --output harness/x',
                                 'git -c core.pager=touch log',
                                 'git grep -Otouch foo',
                                 'find . -fprintf harness/x %p',
                                 'find . -fprint harness/x',
                                 'find . -fls harness/x',
                                 'xxd -r game/a.hex harness/x',
                                 'xxd game/a.bin harness/x',
                                 'uniq game/a.txt harness/x',
                                 'file -C -m game/magic',
                                 'rg --pre touch foo game',
                                 'cat <(touch harness/x)'])
def test_commands_that_write_through_options_are_denied(cmd):
    assert bash(cmd) is not None


@pytest.mark.parametrize('cmd', ['sed -n 10,20p game/a.cpp', "sed -n '/Instance/p' game/a.cpp",
                                 'sed -n -e 1,5p game/a.cpp', 'grep -o foo game/a.cpp',
                                 'xxd -c 16 -l 64 game/a.bin', 'sort -u game/a.txt',
                                 'uniq -c game/a.txt', 'find game -name "*.cpp"',
                                 'git log -p -- game'])
def test_read_only_forms_of_those_commands_are_allowed(cmd):
    assert bash(cmd) is None


CLAIM = dict(address='0x006fc690', baseline_score=80.0, baseline_exact=False, max_blocks=3)


def verify(score, exact=False, exit_=0):
    return dict(exit=exit_, lines=['OK ...'],
                results=[dict(address='0x006fc690', score=score, exact=exact)])


def test_stop_is_allowed_when_the_score_went_up():
    assert stophook.decide(CLAIM, verify(85.0), 0)[0] is True


def test_stop_is_allowed_when_newly_exact():
    assert stophook.decide(CLAIM, verify(100.0, exact=True), 0)[0] is True


def test_stop_is_blocked_without_improvement_or_after_a_regression_or_failed_build():
    assert stophook.decide(CLAIM, verify(80.0), 0)[0] is False
    assert stophook.decide(CLAIM, verify(99.0, exit_=1), 0)[0] is False
    assert stophook.decide(CLAIM, None, 0)[0] is False
    assert stophook.decide(CLAIM, dict(exit=2, lines=[], results=[]), 0)[0] is False


def test_stop_is_let_through_after_the_maximum_number_of_refusals():
    assert stophook.decide(CLAIM, verify(80.0), 3)[0] is True
