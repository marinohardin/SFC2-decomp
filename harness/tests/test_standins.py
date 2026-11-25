"""The new-stand-in check (harness/lib/standins.py): small invented trees, a throw-away git
repository for the base, no compile."""
import json
import os
import subprocess

import pytest

from harness.lib import paths, standins

IDENT = dict(GIT_AUTHOR_NAME='t', GIT_AUTHOR_EMAIL='t@example.invalid',
             GIT_COMMITTER_NAME='t', GIT_COMMITTER_EMAIL='t@example.invalid')
PILOT = ['c_0067b2b0', 'c_tMachine_Field04', 'c_tMachine_Field14', 'fn_00674250_Owner',
         'fn_006932f0_Event', 'tVisObjectSlots']
# What the symbol evidence knows: tMachine and tVisGroup are classes of the table.
KNOWN = {'tMachine', 'tVisGroup'}


def git(repo, *args):
    p = subprocess.run(['git'] + list(args), cwd=repo, capture_output=True, text=True,
                       env=dict(os.environ, **IDENT))
    assert p.returncode == 0, p.stderr
    return p.stdout.strip()


def write(repo, rel, text):
    path = repo / rel
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


@pytest.fixture
def repo(tmp_path, monkeypatch):
    r = tmp_path / 'repo'
    r.mkdir()
    git(r, 'init', '-q')
    write(r, 'op/unsorted/Old.h', 'class fn_00401000_Old\n{\n};\nclass tMachine;\n')
    git(r, 'add', '-A')
    git(r, 'commit', '-q', '-m', 'base')
    monkeypatch.setattr(paths, 'STATE', str(tmp_path / 'state'))
    monkeypatch.setattr(paths, 'WORK', str(tmp_path / 'work'))
    return r


def found(repo, base='HEAD', allowed=None):
    return [f[0] for f in standins.findings(repo=str(repo), base=base, known=KNOWN,
                                            allowed=allowed or {})]


def test_the_six_pilot_names_are_flagged(repo):
    body = ''.join(f'class {n}\n{{\n}};\n' for n in PILOT)
    write(repo, 'op/unsorted/New.h', body)
    assert found(repo) == sorted(PILOT)


def test_other_placeholder_forms_are_flagged(repo):
    write(repo, 'op/a.h', 'struct tObject_00410fd4 {\n};\nstruct tVtbl_0041aabb;\n'
                          'class tChunk_0042dd9b : public tMachine\n{\n};\n')
    assert found(repo) == ['tChunk_0042dd9b', 'tObject_00410fd4', 'tVtbl_0041aabb']


def test_a_class_the_table_knows_passes(repo):
    write(repo, 'op/a.h', 'class tVisGroup : public tMachine\n{\n};\nstruct tMachine;\n')
    assert found(repo) == []


def test_an_unknown_t_class_is_flagged_but_other_names_are_not(repo):
    write(repo, 'op/a.h', 'class tNeverSeen\n{\n};\nstruct PcxHeader { int a; };\n'
                          'template <class T> class allocator\n{\n};\n')
    assert found(repo) == ['tNeverSeen']


def test_a_free_fn_name_is_not_a_class(repo):
    write(repo, 'op/a.cpp', 'void fn_00401234(int a);\nstruct tMachine *fn_00401238(void);\n'
                            'void f()\n{\n    struct tMachine *p = 0;\n}\n')
    assert found(repo) == []


def test_a_stand_in_already_in_the_base_passes(repo):
    write(repo, 'op/unsorted/Old.h', 'class fn_00401000_Old\n{\n    int x;\n};\nclass tMachine;\n')
    assert found(repo) == []


def test_a_moved_or_renamed_file_is_not_new(repo):
    (repo / 'op' / 'ui').mkdir()
    git(repo, 'mv', 'op/unsorted/Old.h', 'op/ui/Renamed.h')
    assert found(repo) == []
    # Also when the declaration moved to another file and the old one is gone.
    write(repo, 'op/other/Elsewhere.h', 'class fn_00401000_Old;\n')
    assert found(repo) == []


def test_committed_work_since_the_base_counts(repo):
    base = git(repo, 'rev-parse', 'HEAD')
    write(repo, 'op/a.h', 'class c_0067b2b0\n{\n};\n')
    git(repo, 'add', '-A')
    git(repo, 'commit', '-q', '-m', 'lane work')
    assert found(repo, base) == ['c_0067b2b0']
    assert found(repo) == []


def test_the_base_comes_from_the_environment_or_the_merge_base(repo):
    base = git(repo, 'rev-parse', 'HEAD')
    assert standins.base_ref(str(repo), {'HARNESS_STANDIN_BASE': 'abc'}) == 'abc'
    assert standins.base_ref(str(repo), {}) is None
    git(repo, 'branch', 'develop')
    assert standins.base_ref(str(repo), {}) == base


def test_no_base_means_no_findings(repo):
    write(repo, 'op/a.h', 'class c_0067b2b0;\n')
    assert standins.findings(repo=str(repo), base=None, known=KNOWN, allowed={},
                             environ={}) == []


def test_an_allowance_in_the_final_message_is_accepted_and_kept(repo, tmp_path):
    write(repo, 'op/a.h', 'class fn_00674250_Owner;\nclass c_0067b2b0;\n')
    text = ('Done.\nSTAND-IN: fn_00674250_Owner 0x00674250 the exe has no RTTI for this owner\n'
            'STAND-IN: bad line\nHEADER REQUEST: tFoo needs op/ui/tFoo.h\n')
    new = standins.record(text)
    assert new == [('fn_00674250_Owner', '0x00674250', 'the exe has no RTTI for this owner')]
    assert standins.record(text) == []   # the same line again adds nothing
    assert open(os.path.join(paths.STATE, 'header_requests.txt')).read() == \
        'tFoo needs op/ui/tFoo.h\n'
    allowed = standins.load_allowed()
    assert list(allowed) == ['fn_00674250_Owner']
    assert found(repo, allowed=allowed) == ['c_0067b2b0']


def test_refusal_text_says_what_to_do(repo):
    write(repo, 'op/a.h', 'class c_0067b2b0\n{\n};\n')
    lines = standins.check_lines(repo=str(repo), base='HEAD', known=KNOWN, allowed={})
    text = '\n'.join(lines)
    assert 'op/a.h:1: c_0067b2b0' in text
    assert 'tools/symbols.py' in text and 'HEADER REQUEST:' in text and 'STAND-IN:' in text


def test_a_name_already_told_is_not_refused_again(repo):
    write(repo, 'op/a.h', 'class c_0067b2b0;\nclass c_0067b2b1;\n')
    kw = dict(repo=str(repo), base='HEAD', known=KNOWN, allowed={})
    lines = standins.check_lines(told=['c_0067b2b0'], **kw)
    assert any('c_0067b2b1' in x for x in lines) and not any('c_0067b2b0' in x for x in lines)
    assert standins.check_lines(told=['c_0067b2b0', 'c_0067b2b1'], **kw) is None
    assert standins.pending_names(**kw) == ['c_0067b2b0', 'c_0067b2b1']


def test_known_tokens_read_class_columns_and_mangled_names(tmp_path):
    table = tmp_path / 't.tsv'
    table.write_text('0x1\tfunction\ttThread::go\ttThread\t??0tWinTimer@@QAE@XZ\tclaim\t\tclaim\n'
                     '0x2\tfunction\tx\t\t?f@@YAXPAVtResourceGuard@@@Z\tclaim\t\tclaim\n')
    got = standins.known_tokens([str(table), str(tmp_path / 'missing')])
    assert {'tThread', 'tWinTimer', 'tResourceGuard'} <= got


def test_the_landing_report_counts_and_lists(repo, tmp_path, monkeypatch):
    write(repo, 'op/a.h', 'class c_0067b2b0;\nclass tVisObjectSlots;\n')
    lane = tmp_path / 'work' / 'harness' / 'wt-x'
    lane.mkdir(parents=True)
    (lane / 'standins.tsv').write_text('tVisObjectSlots\t0x0067ea20\tno RTTI\n')
    monkeypatch.setattr(standins, 'default_known_files', lambda: [])
    out = standins.landing_lines('HEAD', repo=str(repo))
    assert out[0] == 'new stand-in classes against HEAD: 2'
    assert any('c_0067b2b0' in x and 'no STAND-IN allowance' in x for x in out)
    assert any('tVisObjectSlots' in x and 'allowed at 0x0067ea20: no RTTI' in x for x in out)


def test_last_message_reads_the_hook_input_or_the_transcript(tmp_path):
    assert standins.last_message({'last_assistant_message': 'hi'}) == 'hi'
    t = tmp_path / 'tr.jsonl'
    t.write_text(json.dumps({'message': {'role': 'assistant', 'content': [
        {'type': 'text', 'text': 'first'}]}}) + '\n' +
        json.dumps({'message': {'role': 'user', 'content': 'x'}}) + '\n' +
        json.dumps({'message': {'role': 'assistant', 'content': [
            {'type': 'tool_use'}, {'type': 'text', 'text': 'STAND-IN: a 0x1 b'}]}}) + '\n')
    assert standins.last_message({'transcript_path': str(t)}) == 'STAND-IN: a 0x1 b'
    assert standins.last_message({}) == ''
