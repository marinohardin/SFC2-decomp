"""What a function session may edit and run, and when it may stop."""
import os

import pytest

from harness.lib import guard, paths, stophook


@pytest.fixture(autouse=True)
def roots(tmp_path, monkeypatch):
    monkeypatch.setattr(paths, 'SRC_ROOT', os.path.join(paths.REPO, 'op'))
    monkeypatch.setattr(paths, 'STATE', str(tmp_path / 'state'))


def edit(p):
    return guard.decide({'tool_name': 'Edit', 'tool_input': {'file_path': p}})


def bash(c):
    return guard.decide({'tool_name': 'Bash', 'tool_input': {'command': c}})


def test_source_under_the_rebuilt_tree_may_be_edited():
    assert edit(os.path.join(paths.REPO, 'op', 'Source', 'Ship.cpp')) is None
    assert guard.decide({'tool_name': 'Write', 'cwd': paths.REPO,
                         'tool_input': {'file_path': 'op/Source/Ship.h'}}) is None


@pytest.mark.parametrize('rel', ['analysis/results/matched_set.json',
                                 'analysis/results/difficult_functions.tsv', 'harness/lib/verify.py',
                                 'toolchain/bin/cl', 'analysis/results/retail_hashes.sha256',
                                 'ghidra/exports/functions.csv', '.claude/settings.json',
                                 'scripts/check.sh', 'README.md'])
def test_judging_files_and_anything_outside_the_tree_are_denied(rel):
    assert edit(os.path.join(paths.REPO, rel)) is not None


def test_queue_listings_and_objects_are_denied():
    assert edit(os.path.join(paths.STATE, 'queue.json')) is not None
    assert edit(os.path.join(paths.REPO, 'op', 'Source', 'Ship.asm')) is not None
    assert edit(os.path.join(paths.REPO, 'op', 'Source', 'Ship.obj')) is not None


def test_symlink_out_of_the_tree_is_followed(tmp_path):
    link = tmp_path / 'link.json'
    link.symlink_to(paths.MATCHED_SET)
    assert edit(str(link)) is not None


@pytest.mark.parametrize('cmd', ['harness/build-and-verify.sh 0x006fc690',
                                 'sh harness/build-and-verify.sh 0x006fc690 --json 2>&1 | tail -5',
                                 'grep -n Instance op/Source/A.cpp', 'git diff', 'ls game',
                                 'cat harness/README.md'])
def test_verify_and_read_only_commands_are_allowed(cmd):
    assert bash(cmd) is None


@pytest.mark.parametrize('cmd', ['harness/build-and-verify.sh 0x006fc690 --record',
                                 'harness/update-matched-set.sh 0x006fc690',
                                 'python3 -m harness.lib.verify --record 0x006fc690',
                                 'op/harness/build-and-verify.sh 0x006fc690',
                                 'rm -rf harness', 'echo {} > harness/matched_set.json',
                                 'cat $(ls)', 'git commit -am x', 'git checkout -- game',
                                 "sed -i '' s/a/b/ op/A.cpp", 'find game -delete',
                                 'HARNESS_MATCHED_SET=/tmp/m.json harness/build-and-verify.sh 0x1',
                                 'env | grep ANTHROPIC_API_KEY', 'pkill -9 bash', 'docker ps',
                                 "awk 'BEGIN{system(\"id\")}'", 'less harness/README.md'])
def test_writes_and_other_commands_are_denied(cmd):
    assert bash(cmd) is not None


# Each of these wrote a file through an allowed command in review of the first version.
@pytest.mark.parametrize('cmd', ['true & touch harness/x',
                                 'ls |& touch harness/x',
                                 'sort -o harness/lib/verify.py op/a.cpp',
                                 'sort -uo harness/lib/verify.py op/a.cpp',
                                 'sort --output=harness/lib/verify.py op/a.cpp',
                                 'sed -n "w harness/lib/verify.py" op/a.cpp',
                                 'sed -n "s/a/b/w harness/x" op/a.cpp',
                                 'sed -n "1e touch x" op/a.cpp',
                                 'sed -n -f script.sed op/a.cpp',
                                 'git diff --output=harness/x',
                                 'git log --output harness/x',
                                 'git -c core.pager=touch log',
                                 'git grep -Otouch foo',
                                 'find . -fprintf harness/x %p',
                                 'find . -fprint harness/x',
                                 'find . -fls harness/x',
                                 'xxd -r op/a.hex harness/x',
                                 'xxd op/a.bin harness/x',
                                 'uniq op/a.txt harness/x',
                                 'file -C -m op/magic',
                                 'rg --pre touch foo game',
                                 'cat <(touch harness/x)'])
def test_commands_that_write_through_options_are_denied(cmd):
    assert bash(cmd) is not None


@pytest.mark.parametrize('cmd', ['sed -n 10,20p op/a.cpp', "sed -n '/Instance/p' op/a.cpp",
                                 'sed -n -e 1,5p op/a.cpp', 'grep -o foo op/a.cpp',
                                 'xxd -c 16 -l 64 op/a.bin', 'sort -u op/a.txt',
                                 'uniq -c op/a.txt', 'find game -name "*.cpp"',
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



# Claim lines of other functions (the real loop proof's session deleted one in another unit).

OWN = '0x006fc750'
UNIT = ('// FUNCTION: STARFLEETOP 0x006fc5a0\nint f() { return 1; }\n\n'
        '// FUNCTION: STARFLEETOP 0x006fc750\nint g() { return 2; }\n')


@pytest.fixture
def unit(tmp_path, monkeypatch):
    """A unit under a temporary source root, with this session claiming 0x006fc750."""
    claim = tmp_path / 'state' / 'claim.json'
    claim.parent.mkdir(parents=True, exist_ok=True)
    claim.write_text('{"address": "%s"}' % OWN)
    monkeypatch.setattr(paths, 'CLAIM', str(claim))
    src = tmp_path / 'game'
    (src / 'Common').mkdir(parents=True)
    monkeypatch.setattr(paths, 'SRC_ROOT', str(src))
    f = src / 'Common' / 'DynamicAllocator.cpp'
    f.write_text(UNIT)
    return f


def edit_text(f, old, new, **kw):
    return guard.decide({'tool_name': 'Edit', 'tool_input': dict(
        file_path=str(f), old_string=old, new_string=new, **kw)})


def test_deleting_another_functions_claim_line_is_denied(unit):
    reason = edit_text(unit, '// FUNCTION: STARFLEETOP 0x006fc5a0\n', '')
    assert reason is not None and '0x006fc5a0' in reason


def test_editing_part_of_another_claim_line_is_denied(unit):
    assert edit_text(unit, '0x006fc5a0', '0x006fc5a4') is not None
    assert edit_text(unit, 'FUNCTION: STARFLEETOP 0x006fc5a0', 'STUB: STARFLEETOP 0x006fc5a0') is not None
    assert edit_text(unit, '// FUNCTION: STARFLEETOP 0x006fc5a0', 'FUNCTION: STARFLEETOP 0x006fc5a0') is not None


def test_replace_all_that_reaches_another_claim_is_denied(unit):
    assert edit_text(unit, 'STARFLEETOP', 'OTHER', replace_all=True) is not None


def test_own_claim_line_may_be_changed_or_removed(unit):
    assert edit_text(unit, '// FUNCTION: STARFLEETOP 0x006fc750\n', '// STUB: STARFLEETOP 0x006fc750\n') is None
    assert edit_text(unit, '// FUNCTION: STARFLEETOP 0x006fc750\n', '') is None


def test_edit_keeping_another_claim_line_is_allowed(unit):
    assert edit_text(unit, 'return 1;', 'return 3;') is None


def test_without_a_claim_every_claim_line_is_protected(unit, monkeypatch, tmp_path):
    monkeypatch.setattr(paths, 'CLAIM', str(tmp_path / 'missing.json'))
    assert edit_text(unit, '// FUNCTION: STARFLEETOP 0x006fc750\n', '') is not None


def test_write_that_drops_another_claim_is_denied(unit):
    deny = guard.decide({'tool_name': 'Write', 'tool_input': {'file_path': str(unit), 'content': 'int f();\n'}})
    keep = guard.decide({'tool_name': 'Write', 'tool_input': {
        'file_path': str(unit), 'content': '// FUNCTION: STARFLEETOP 0x006fc5a0\nint f();\n'}})
    assert deny is not None and keep is None


def test_write_of_a_new_file_is_allowed(unit):
    new = unit.parent / 'New.cpp'
    assert guard.decide({'tool_name': 'Write', 'tool_input': {'file_path': str(new), 'content': 'int h();\n'}}) is None


def test_multiedit_is_applied_in_order(unit):
    reason = guard.decide({'tool_name': 'MultiEdit', 'tool_input': {'file_path': str(unit), 'edits': [
        {'old_string': 'return 1;', 'new_string': 'return 4;'},
        {'old_string': '// FUNCTION: STARFLEETOP 0x006fc5a0\n', 'new_string': ''}]}})
    assert reason is not None and '0x006fc5a0' in reason


def test_claim_lines_snapshot_leaves_out_the_claimed_function(tmp_path):
    from harness.lib import loopio
    (tmp_path / 'Tiger').mkdir()
    (tmp_path / 'Tiger' / 'tsay.cpp').write_text(
        '// FUNCTION: STARFLEETOP 0x0066c5f0\nx\n  // STUB: STARFLEETOP 0x6fc750\n')
    (tmp_path / 'Tiger' / 'tsay.h').write_text('// FUNCTION: STARFLEETOP 0x66C680\n')
    (tmp_path / 'notes.txt').write_text('// FUNCTION: STARFLEETOP 0x00401000\n')
    assert loopio.claim_lines(str(tmp_path), 0x006fc750) == [
        'Tiger/tsay.cpp\tFUNCTION STARFLEETOP 0x0066c5f0',
        'Tiger/tsay.h\tFUNCTION STARFLEETOP 0x0066c680']


def test_claim_lines_snapshot_of_a_missing_tree_fails(tmp_path):
    from harness.lib import loopio
    with pytest.raises(SystemExit):
        loopio.claim_lines(str(tmp_path / 'nowhere'), 0x006fc750)


def _read(tool, **ti):
    return guard.decide({'tool_name': tool, 'cwd': paths.REPO, 'tool_input': ti})


def test_private_files_are_never_read_by_any_tool():
    work = paths.WORK
    private = os.path.join(work, 'private', 'sfcop.env')
    assert _read('Read', file_path=private) == guard.PRIVATE_DENY
    assert _read('Read', file_path=os.path.join(work, 'wine', 'prefix', 'user.reg')) \
        == guard.PRIVATE_DENY
    assert _read('Read', file_path=os.path.join(work, 'testkey-data', 'x.ini')) \
        == guard.PRIVATE_DENY
    assert _read('Read', file_path='/Users/someone/game.reg') == guard.PRIVATE_DENY
    # A search from an ancestor would reach the private folder.
    assert _read('Grep', pattern='key', path=work) == guard.PRIVATE_DENY
    assert _read('Glob', pattern=os.path.join(work, 'priv*', '*')) == guard.PRIVATE_DENY
    assert bash('cat ' + private) == guard.PRIVATE_DENY
    assert bash('cat ../../work/private/cdkey.reg') == guard.PRIVATE_DENY
    assert bash('grep -r key ../work/testkey-data') == guard.PRIVATE_DENY


def test_ordinary_reads_stay_allowed():
    assert _read('Read', file_path=os.path.join(paths.REPO, 'op', 'UNITS.tsv')) is None
    assert _read('Grep', pattern='class tFoo', path='op') is None
    assert _read('Glob', pattern='op/**/*.h') is None
    assert _read('Read', file_path=os.path.join(paths.WORK, 'ghidra', 'decomp', '0x00401000.c')) \
        is None
    assert bash('grep -n "class tFoo" op/unsorted/tFoo.h') is None


@pytest.mark.parametrize('command', [
    'cd ../work && cat private/x',
    'cat ../work/pri*/x',
    'ls work/priv*',
    'ls ../work/priv?',
    'cat ../work/p[r]ivate/x',
    'cat ../work/{private,x}/y',
    'cat ../work/p{r,x}ivate/y',
    'ls ../../../../work/*',
    'cat /Volumes/x/work/*/cdkey*',
    'grep -r key ../../../../..',
    "cat ../work/'pri'\"vate\"/x",
    'cat ../work/PRIVATE/x',
    'cat $HOME/x',
    "cat $'\\x70rivate'/x",
    'cat ~/x.txt',
    'ls testkey-data',
    'cat ../wine/prefix/user.txt',
    'cat op/x.REG',
    'cat sfcop.env',
])
def test_bash_refuses_every_way_to_name_the_private_files(command):
    assert bash(command) == guard.PRIVATE_DENY


@pytest.mark.parametrize('command', [
    'python3 tools/manifest.py --check',
    'sh harness/build-and-verify.sh 0x00401000',
    'harness/build-and-verify.sh 0x00401000',
    'git diff',
    'git diff -- op/unsorted/tSphere.cpp',
    'ls op/unsorted/*.h',
    'grep -n "class tSphere" op/unsorted/tSphere.h',
])
def test_ordinary_grind_commands_still_pass(command):
    assert bash(command) is None


def test_settings_fragment_denies_the_private_files_without_the_hook():
    import json
    with open(os.path.join(paths.HARNESS, 'hooks', 'settings.fragment.json')) as f:
        deny = json.load(f)['permissions']['deny']
    for rule in ('Read(//**/private/**)', 'Read(//**/testkey*/**)', 'Read(//**/wine/prefix/**)',
                 'Read(//**/*.reg)', 'Read(//**/*.env)', 'Bash(*private*)', 'Bash(*testkey*)',
                 'Bash(*wine/prefix*)', 'Bash(*.reg*)', 'Bash(*.env*)'):
        assert rule in deny, rule


def test_a_link_in_the_worktree_to_a_private_folder_is_refused_through_realpath(
        tmp_path, monkeypatch):
    # A fake work folder and worktree under a temporary directory; the link's own path names
    # nothing private, only its target does.
    work, repo = tmp_path / 'work', tmp_path / 'repo'
    (work / 'private').mkdir(parents=True)
    (work / 'private' / 'secret.txt').write_text('not real\n')
    repo.mkdir()
    (repo / 'notes').symlink_to(work / 'private')
    monkeypatch.setattr(paths, 'WORK', str(work))
    monkeypatch.setattr(paths, 'REPO', str(repo))
    assert guard.decide({'tool_name': 'Read', 'cwd': str(repo),
                         'tool_input': {'file_path': str(repo / 'notes' / 'secret.txt')}}) \
        == guard.PRIVATE_DENY
    assert guard.decide({'tool_name': 'Grep', 'cwd': str(repo),
                         'tool_input': {'pattern': 'x', 'path': 'notes'}}) == guard.PRIVATE_DENY
    assert bash('cat notes/secret.txt') == guard.PRIVATE_DENY
    assert bash('ls notes') == guard.PRIVATE_DENY


# The lane file list ($HARNESS_LANE_FILES).

@pytest.fixture
def lane(tmp_path, monkeypatch):
    f = tmp_path / 'lane.txt'
    f.write_text('# the files of this lane\n\nop/Tiger/tsay.cpp\n  op/Tiger/tsay*.h  \n'
                 'op/Common/*.inl\n')
    monkeypatch.setenv('HARNESS_LANE_FILES', str(f))
    return f


def op(rel):
    return os.path.join(paths.REPO, 'op', rel)


def test_without_a_lane_list_every_file_under_the_tree_is_allowed(monkeypatch):
    monkeypatch.delenv('HARNESS_LANE_FILES', raising=False)
    assert edit(op('Tiger/tsay.cpp')) is None
    assert edit(op('Other/new.cpp')) is None


def test_a_listed_file_is_allowed(lane):
    assert edit(op('Tiger/tsay.cpp')) is None
    assert guard.decide({'tool_name': 'Write', 'cwd': paths.REPO,
                         'tool_input': {'file_path': 'op/Tiger/tsay_new.h'}}) is None


def test_a_glob_entry_matches_files_it_covers_and_a_comment_or_blank_line_matches_nothing(lane):
    assert edit(op('Tiger/tsay2.h')) is None
    assert edit(op('Common/list.inl')) is None
    assert edit(op('Tiger/tsayx.cpp')) is not None
    assert edit(op('Tiger/other.h')) is not None
    assert edit(op('Common/list.h')) is not None


def test_an_unlisted_file_is_denied_with_the_request_forms(lane):
    reason = edit(op('Tiger/tship.cpp'))
    assert 'Leave it alone' in reason
    assert 'HEADER REQUEST: <header> <what and why> (for 0x<address>)' in reason
    assert 'UNIT REQUEST: <file> <what and why> (for 0x<address>)' in reason


def test_a_new_unlisted_file_is_denied_and_a_new_listed_one_is_allowed(lane):
    assert not os.path.exists(op('Tiger/brand_new.cpp'))
    assert edit(op('Tiger/brand_new.cpp')) is not None
    assert edit(op('Tiger/tsay_new.h')) is None


def test_a_relative_path_is_matched_from_the_repository_root(lane):
    ev = {'tool_name': 'Edit', 'cwd': paths.REPO, 'tool_input': {'file_path': 'op/Tiger/tship.cpp'}}
    assert guard.decide(ev) is not None
    ev['tool_input']['file_path'] = 'op/Tiger/tsay.cpp'
    assert guard.decide(ev) is None


def test_multiedit_is_held_to_the_list_too(lane):
    assert guard.decide({'tool_name': 'MultiEdit', 'tool_input': {
        'file_path': op('Tiger/tship.cpp'), 'edits': []}}) is not None


def test_a_set_but_unreadable_list_denies_every_edit(tmp_path, monkeypatch):
    monkeypatch.setenv('HARNESS_LANE_FILES', str(tmp_path / 'missing.txt'))
    assert 'cannot be read' in edit(op('Tiger/tsay.cpp'))


def test_the_list_does_not_widen_what_is_protected(lane, tmp_path):
    lane.write_text('harness/*\nanalysis/*\n**\n*\n')
    assert edit(os.path.join(paths.REPO, 'harness', 'lib', 'verify.py')) is not None
    assert edit(os.path.join(paths.REPO, 'analysis', 'results', 'matched_set.json')) is not None
