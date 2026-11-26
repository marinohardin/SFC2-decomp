"""tools/contrib_guard.py reads a contributor's diff and refuses what a source-only pull request
must not change.

Throwaway git repositories under pytest's tmp_path; no network, no gh. Expected messages are
written out by hand.
"""
import importlib.util
import json
import os
import pathlib
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]
_spec = importlib.util.spec_from_file_location('contrib_guard', ROOT / 'tools/contrib_guard.py')
guard = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(guard)

ISSUES = [
    {'number': 7, 'state': 'open', 'labels': [{'name': 'claim'}], 'user': {'login': 'ann'},
     'body': '### Units\n\nop/unsorted/a.cpp\nop/unsorted/b.cpp\n\n'
             '### Ranges\n\nrange: 0x00401000-0x004010ff\nrange: 0x00402000-0x00402010\n'},
    {'number': 8, 'state': 'closed', 'labels': [{'name': 'claim'}], 'user': {'login': 'ann'},
     'body': 'op/unsorted/c.cpp\nrange: 0x00500000-0x00500fff\n'},
    {'number': 9, 'state': 'open', 'labels': [{'name': 'question'}], 'user': {'login': 'ann'},
     'body': 'op/unsorted/c.cpp\nrange: 0x00500000-0x00500fff\n'},
    {'number': 11, 'state': 'OPEN', 'labels': [{'name': 'reserved'}], 'author': {'login': 'own'},
     'body': 'Batch 9\nrange: 0x00600000-0x006000ff\n'},
]
BODY = 'Some text.\n\nClaim: #7\n'
UNITS = ('unit\tfirst\tlast\tfunctions\tevidence\tprofile\theader\n'
         'unsorted/a.cpp\t0x00401000\t0x004010ff\t3\tpage\tgame-release\t\n'
         'unsorted/b.cpp\t0x00402000\t0x00402fff\t3\tpage\tgame-release\t\n'
         'unsorted/c.cpp\t0x00500000\t0x00500fff\t3\tpage\tgame-release\t\n'
         'unsorted/d.cpp\t0x00600000\t0x00600fff\t3\tpage\tgame-release\t\n')


def _git(repo, *args):
    subprocess.run(['git', '-c', 'user.name=Test', '-c', 'user.email=test@example.invalid',
                    *args], cwd=repo, check=True, capture_output=True)


def _write(repo, rel, text, mode=None):
    p = repo / rel
    p.parent.mkdir(parents=True, exist_ok=True)
    if isinstance(text, bytes):
        p.write_bytes(text)
    else:
        p.write_text(text)
    if mode:
        os.chmod(p, mode)


def _repo(tmp_path):
    repo = tmp_path / 'repo'
    repo.mkdir()
    _git(repo, 'init', '-q', '-b', 'develop')
    _write(repo, 'op/unsorted/a.cpp', '#include "a.h"\n\nvoid f() {}\n')
    _write(repo, 'op/unsorted/a.h', 'void f();\n')
    _write(repo, 'tools/x.py', 'print(1)\n')
    _write(repo, 'op/UNITS.tsv', UNITS)
    _git(repo, 'add', '-A')
    _git(repo, 'commit', '-q', '-m', 'base')
    _git(repo, 'checkout', '-q', '-b', 'contrib/7-a')
    return repo


def _commit(repo):
    _git(repo, 'add', '-A')
    _git(repo, 'commit', '-q', '-m', 'change')


def _check(repo, body=BODY, issues=ISSUES, author='ann'):
    return guard.check(str(repo), 'develop', 'HEAD', body, issues, author=author)


def test_a_claimed_source_change_passes(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', '#include "a.h"\n#include "../Source/b.h"\n\n'
           '// FUNCTION: STARFLEETOP 0x00401010\nvoid f() {}\n')
    _commit(repo)
    res = _check(repo)
    assert res.failures == []
    assert res.added == {0x00401010}
    assert [c.path for c in res.changes] == ['op/unsorted/a.cpp']


def test_paths_outside_op_and_the_maintainers_files_fail(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'tools/x.py', 'print(2)\n')
    _write(repo, 'analysis/results/matched_set.json', '{}\n')
    _write(repo, 'op/UNITS.tsv', 'unit\n')
    _write(repo, 'op/port/p.h', '\n')
    _commit(repo)
    assert _check(repo).failures == [
        'analysis/results/matched_set.json: outside op/',
        "op/UNITS.tsv: op/port/, op/UNITS.tsv and op/manifest.tsv are the maintainer's",
        "op/port/p.h: op/port/, op/UNITS.tsv and op/manifest.tsv are the maintainer's",
        'tools/x.py: outside op/',
    ]


def test_symlinks_modes_binaries_and_large_files_fail(tmp_path):
    repo = _repo(tmp_path)
    os.symlink('/etc/hosts', repo / 'op/link.h')
    _write(repo, 'op/unsorted/a.h', 'void f();\n', mode=0o755)
    _write(repo, 'op/blob.cpp', b'\x00\x01\x02binary')
    _write(repo, 'op/big.cpp', '// x\n' * 60000)
    _commit(repo)
    assert _check(repo).failures == [
        'op/big.cpp: 300000 bytes, over the 262144 byte limit',
        'op/blob.cpp: a binary file',
        'op/link.h: a symlink',
        'op/unsorted/a.h: file mode 100644 -> 100755 (only 100644 files)',
    ]


def test_includes_that_leave_op_and_imports_fail(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', '#include "a.h"\n'
           '#include "/etc/passwd"\n'
           '#include <C:\\Windows\\win.ini>\n'
           '#include "../../harness/lib/x.h"\n'
           '#  include SOME_MACRO\n'
           '#import "x.tlb"\n'
           '#include "../Common/ok.h"\n'
           'void f() {}\n')
    _commit(repo)
    assert _check(repo).failures == [
        "op/unsorted/a.cpp: adds an absolute #include: '/etc/passwd'",
        "op/unsorted/a.cpp: adds an #include with a drive letter: 'C:/Windows/win.ini'",
        "op/unsorted/a.cpp: adds an #include that leaves op/: '../../harness/lib/x.h'",
        "op/unsorted/a.cpp: adds an #include that names no file: 'SOME_MACRO'",
        'op/unsorted/a.cpp: adds an #import',
    ]


def test_an_existing_include_is_not_judged_again(tmp_path):
    repo = tmp_path / 'repo'
    repo.mkdir()
    _git(repo, 'init', '-q', '-b', 'develop')
    _write(repo, 'op/a.cpp', '#include MACRO\nvoid f() {}\n')
    _write(repo, 'op/UNITS.tsv', UNITS)
    _commit(repo)
    _git(repo, 'checkout', '-q', '-b', 'contrib/7-a')
    _write(repo, 'op/a.cpp', '#include MACRO\n// FUNCTION: STARFLEETOP 0x00402000\nvoid f() {}\n')
    _commit(repo)
    assert _check(repo).failures == []


def test_annotations_outside_the_claim_fail_added_or_removed(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', '// FUNCTION: STARFLEETOP 0x00403000\nvoid f() {}\n')
    _commit(repo)
    _git(repo, 'checkout', '-q', 'develop')
    _write(repo, 'op/unsorted/c.cpp', '// STUB: STARFLEETOP 0x00600000\nvoid g() {}\n')
    _commit(repo)
    _git(repo, 'checkout', '-q', '-b', 'contrib/7-b')
    (repo / 'op/unsorted/c.cpp').unlink()
    _write(repo, 'op/unsorted/a.cpp', '// FUNCTION: STARFLEETOP 0x004010ff\nvoid f() {}\n')
    _commit(repo)
    assert _check(repo).failures == [
        '0x00600000: the pull request removes this annotation, outside the claimed ranges',
    ]
    _git(repo, 'checkout', '-q', 'contrib/7-a')
    assert _check(repo).failures == [
        '0x00403000: the pull request adds or changes this annotation, outside the claimed ranges',
    ]


def test_a_missing_closed_or_unlabelled_claim_fails(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', 'void f() { }\n')
    _commit(repo)
    assert _check(repo, body='no claim here').failures == [
        'the description names no claim (a line `Claim: #<issue>`); '
        'see CONTRIBUTING.md, "Find and claim work"',
    ]
    assert _check(repo, body='Claim: #8\nClaim: #9\nClaim: #10\n').failures == [
        'claim #8: the issue is closed',
        'claim #9: the issue is not labelled `claim`',
        'claim #10: no such issue',
    ]


def test_parse_ranges_and_claim_numbers():
    assert guard.parse_ranges('x\nrange: 0x401000-0x00401FFF\n  range:0x1 - 0x2\nrange: 401000\n') \
        == [(0x401000, 0x401fff), (1, 2)]
    assert guard.claim_numbers('Claim: #12\nclaim: #3\nnot Claim: #4') == [12, 3]


def test_overlap_warnings_name_other_pull_requests():
    assert guard.overlap_warnings(5, {0x401000, 0x401010},
                                  [(5, {0x401000}), (6, {0x401010, 0x500000}), (7, set())]) == [
        'pull request #6 also annotates 0x00401010',
    ]


def test_the_command_line_prints_failures_and_exits_1(tmp_path, capsys, monkeypatch):
    repo = _repo(tmp_path)
    _write(repo, 'tools/x.py', 'print(2)\n')
    _commit(repo)
    claims = tmp_path / 'claims.json'
    claims.write_text(json.dumps(ISSUES))
    body = tmp_path / 'body.md'
    body.write_text(BODY)
    monkeypatch.chdir(repo)
    assert guard.main(['develop', 'HEAD', '--body', str(body), '--claims', str(claims),
                       '--author', 'ann']) == 1
    assert capsys.readouterr().out == (
        'FAIL tools/x.py: outside op/\n'
        'guard: 1 problems; see CONTRIBUTING.md, "Find and claim work"\n')


def test_an_out_of_claim_vtable_line_fails(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.h', '// VTABLE: STARFLEETOP 0x00790000\nclass tA {};\n')
    _commit(repo)
    assert _check(repo).failures == [
        '0x00790000: the pull request adds or changes this annotation, outside the claimed ranges',
    ]


def test_the_annotation_pattern_is_the_harness_guards():
    text = (ROOT / 'harness/lib/guard.py').read_text()
    assert f"CLAIM_LINE = re.compile(r'{guard.ANNOTATION.pattern}')" in text


def _claim(body, login='ann', number=20):
    return {'number': number, 'state': 'open', 'labels': [{'name': 'claim'}],
            'user': {'login': login}, 'body': body}


def test_a_claim_opened_by_someone_else_fails(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', 'void f() { }\n')
    _commit(repo)
    assert _check(repo, author='bob').failures == [
        "claim #7: opened by ann, not by the pull request's author (bob)",
    ]
    assert _check(repo, author=None).failures == [
        "claim #7: opened by ann, not by the pull request's author (unknown)",
    ]


def test_a_range_must_lie_in_one_unit_the_issue_names(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', 'void f() { }\n')
    _commit(repo)
    issues = [_claim('op/unsorted/a.cpp\nrange: 0x00401000-0x00402010\n'
                     'range: 0x00402000-0x00402010\nrange: 0x00700000-0x00700001\n')]
    assert _check(repo, body='Claim: #20', issues=issues).failures == [
        'claim #20: range 0x00401000-0x00402010 is not inside one unit of op/UNITS.tsv',
        'claim #20: range 0x00402000-0x00402010 is in op/unsorted/b.cpp, which the issue does '
        'not name',
        'claim #20: range 0x00700000-0x00700001 is not inside one unit of op/UNITS.tsv',
    ]


def test_a_claim_overlapping_a_reserved_issue_fails(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', 'void f() { }\n')
    _commit(repo)
    issues = ISSUES + [_claim('op/unsorted/d.cpp\nrange: 0x006000f0-0x00600200\n')]
    assert _check(repo, body='Claim: #20', issues=issues).failures == [
        'range 0x006000f0-0x00600200 overlaps reserved issue #11 (0x00600000-0x006000ff)',
    ]


def test_splices_line_directives_and_file_pragmas_fail(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', '#include "a.h"\n'
           '#define LONG(x) \\\n'
           '    (x)\n'
           '// a comment that ends in a splice \\\n'
           '#line 12 "C:\\\\src\\\\a.cpp"\n'
           '#pragma include_alias(<a.h>, <b.h>)\n'
           '#pragma message("see /etc/passwd")\n'
           '_Pragma("include_alias(\\"a.h\\", \\"b.h\\")")\n'
           '#pragma warning(disable: 4786)\n'
           '#pragma once\n'
           'void f() {}\n')
    _commit(repo)
    splice = ('op/unsorted/a.cpp: adds a line ending in a backslash (a line splice); write a '
              'multi-line macro on one line in a contributor pull request, or ask the maintainer')
    assert _check(repo).failures == [
        splice, splice,
        'op/unsorted/a.cpp: adds a #line directive; ask the maintainer',
        "op/unsorted/a.cpp: adds a #pragma that can name a file: '#pragma include_alias(<a.h>, <b.h>)'",
        'op/unsorted/a.cpp: adds a #pragma that can name a file: \'#pragma message("see /etc/passwd")\'',
        'op/unsorted/a.cpp: adds a #pragma that can name a file: '
        '\'_Pragma("include_alias(\\\\"a.h\\\\", \\\\"b.h\\\\")")\'',
    ]


def test_only_source_files_under_op_may_change(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/notes.md', 'hello\n')
    _write(repo, 'op/unsorted/build.py', 'print(1)\n')
    _write(repo, 'op/unsorted/a.hpp', '\n')
    _write(repo, 'op/unsorted/a.inl', '\n')
    _commit(repo)
    assert _check(repo).failures == [
        'op/notes.md: not a source file (.cpp, .h, .inl)',
        'op/unsorted/a.hpp: not a source file (.cpp, .h, .inl)',
        'op/unsorted/build.py: not a source file (.cpp, .h, .inl)',
    ]


def test_plus_and_minus_lines_inside_a_hunk_are_scanned(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', '#include "a.h"\n\n'
           '-- // FUNCTION: STARFLEETOP 0x00700000\nvoid f() {}\n')
    _commit(repo)
    _write(repo, 'op/unsorted/a.cpp', '#include "a.h"\n\n'
           '++ // FUNCTION: STARFLEETOP 0x00700010\nvoid f() {}\n')
    _commit(repo)
    added, removed = guard.diff_lines(str(repo), 'HEAD~1', 'HEAD', 'op/unsorted/a.cpp')
    assert added == ['++ // FUNCTION: STARFLEETOP 0x00700010']
    assert removed == ['-- // FUNCTION: STARFLEETOP 0x00700000']


def test_dot_dot_in_an_angle_include_fails_anywhere(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', '#include "a.h"\n'
           '#include <../Common/x.h>\n'
           '#include <sys/../types.h>\n'
           '#include "../Common/ok.h"\n'
           'void f() {}\n')
    _commit(repo)
    assert _check(repo).failures == [
        "op/unsorted/a.cpp: adds an #include <...> with '..': '../Common/x.h'",
        "op/unsorted/a.cpp: adds an #include <...> with '..': 'sys/../types.h'",
    ]


def test_comments_around_a_directive_do_not_hide_it(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', '#include "a.h"\n'
           '/**/#include "/etc/passwd"\n'
           '#/**/include "/etc/passwd"\n'
           '# /**/ import "x.tlb"\n'
           '/* */ #pragma include_alias("a","b")\n'
           '/* a comment\n'
           '   that closes here */ #include "/etc/passwd"\n'
           '//#include "/etc/passwd"\n'
           'int x; /* fine */ // #include "/etc/passwd"\n'
           'void f() {}\n')
    _commit(repo)
    assert _check(repo).failures == [
        "op/unsorted/a.cpp: adds an absolute #include: '/etc/passwd'",
        "op/unsorted/a.cpp: adds an absolute #include: '/etc/passwd'",
        'op/unsorted/a.cpp: adds an #import',
        "op/unsorted/a.cpp: adds a #pragma that can name a file: '#pragma include_alias(\"a\",\"b\")'",
        'op/unsorted/a.cpp: adds a # after a comment that ends on its line; write the directive '
        'on a line of its own',
    ]


def test_a_claim_overlapping_another_authors_open_claim_fails(tmp_path):
    repo = _repo(tmp_path)
    _write(repo, 'op/unsorted/a.cpp', 'void f() { }\n')
    _commit(repo)
    bob = _claim('op/unsorted/b.cpp\nrange: 0x00402008-0x00402100\n', login='bob', number=21)
    mine = _claim('op/unsorted/b.cpp\nrange: 0x00402000-0x00402010\n', number=22)
    closed = dict(_claim('op/unsorted/b.cpp\nrange: 0x00402000-0x00402fff\n', login='cy',
                         number=23), state='closed')
    assert _check(repo, issues=ISSUES + [bob, mine, closed]).failures == [
        'range 0x00402000-0x00402010 overlaps open claim #21 by bob (0x00402008-0x00402100)',
    ]
