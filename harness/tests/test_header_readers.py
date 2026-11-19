"""A single-function run also rebuilds every other unit that reads a file changed since HEAD, so a
header edit that breaks another unit's recorded functions is a regression of the run."""
import subprocess
from types import SimpleNamespace

import pytest

from harness.lib import funcs, matched, paths, source, verify

A, B, C = 0x00401000, 0x00401010, 0x00401020


class Index:
    def __init__(self):
        self.by_address = {a: SimpleNamespace(address=a, name=f'f{i}', size=16)
                           for i, a in enumerate((A, B, C))}

    def resolve(self, ident):
        return self.by_address[int(ident, 16)]


def git(cwd, *args):
    subprocess.run(['git', *args], cwd=cwd, check=True, capture_output=True,
                   env={'GIT_AUTHOR_NAME': 't', 'GIT_AUTHOR_EMAIL': 't@example.invalid',
                        'GIT_COMMITTER_NAME': 't', 'GIT_COMMITTER_EMAIL': 't@example.invalid',
                        'HOME': str(cwd), 'PATH': '/usr/bin:/bin:/usr/local/bin:/opt/homebrew/bin'})


@pytest.fixture
def tree(tmp_path, monkeypatch):
    """a.cpp (the claimed function) and b.cpp both include shared.h, through inner.h for b.cpp;
    c.cpp includes nothing. b.cpp and c.cpp hold an exact recorded function each. Every build
    fails, so a unit that is built shows up in `built` and its recorded function regresses."""
    game = tmp_path / 'game'
    game.mkdir()
    (game / 'shared.h').write_text('struct S { int x; };\n')
    (game / 'inner.h').write_text('#include "shared.h"\n')
    (game / 'a.cpp').write_text(f'#include "shared.h"\n// FUNCTION: STARFLEETOP 0x{A:08x}\nvoid f0() {{}}\n')
    (game / 'b.cpp').write_text(f'#include "inner.h"\n// FUNCTION: STARFLEETOP 0x{B:08x}\nvoid f1() {{}}\n')
    (game / 'c.cpp').write_text(f'// FUNCTION: STARFLEETOP 0x{C:08x}\nvoid f2() {{}}\n')
    (game / 'UNITS.tsv').write_text('unit\tfirst\tlast\n')
    git(tmp_path, 'init', '-q')
    git(tmp_path, 'add', '-A')
    git(tmp_path, 'commit', '-q', '-m', 'base')
    entries = {f'0x{a:08x}': dict(name=f'f{i}', symbol=f'?f{i}@@YAXXZ', unit=u, size=16,
                                  score=100.0, exact=True, recorded='2026-11-01')
               for i, (a, u) in enumerate(((B, 'b.cpp'), (C, 'c.cpp')), start=1)}
    monkeypatch.setattr(paths, 'SRC_ROOT', str(game))
    monkeypatch.setattr(paths, 'UNITS_TSV', str(game / 'UNITS.tsv'))
    monkeypatch.setattr(paths, 'MATCHED_SET', str(tmp_path / 'matched_set.json'))
    monkeypatch.setattr(paths, 'BOUNDARIES', str(tmp_path / 'boundary_errors.tsv'))
    monkeypatch.setattr(funcs, 'FunctionIndex', lambda *a: Index())
    monkeypatch.setattr(verify, 'check_exe', lambda rep: None)
    monkeypatch.setattr(source, 'unit_profile',
                        lambda cfg, root, path: (path.rsplit('/', 1)[-1], {'name': 'test'}))
    matched.save(paths.MATCHED_SET, {'version': 1, 'functions': entries})
    built = []

    def compile_unit(path, profile, src_root, build_root=None, cl=None):
        built.append(path.rsplit('/', 1)[-1])
        return dict(ok=False, rc=2, obj=None, listing=None, secs=0.0, out_dir=str(tmp_path),
                    log='x.cpp(1) : error C2535: broken')

    monkeypatch.setattr(source, 'compile_unit', compile_unit)
    return SimpleNamespace(game=game, built=built)


def test_a_clean_tree_builds_only_the_claimed_unit(tree):
    rep = verify.run([f'0x{A:08x}'], jobs=1)
    assert tree.built == ['a.cpp']
    assert rep.regressions == []


def test_a_changed_header_rebuilds_the_units_that_read_it(tree):
    (tree.game / 'shared.h').write_text('struct S { int x; int y; };\n')
    rep = verify.run([f'0x{A:08x}'], jobs=1)
    assert sorted(tree.built) == ['a.cpp', 'b.cpp']
    assert rep.regressions == [B]
    assert any('1 other units read a file changed since HEAD' in ln and 'b.cpp' in ln
               for ln in rep.lines)


def test_a_change_to_the_claimed_unit_alone_rebuilds_nothing_else(tree):
    (tree.game / 'a.cpp').write_text(f'#include "shared.h"\n// FUNCTION: STARFLEETOP 0x{A:08x}\nvoid f0() {{ }}\n')
    verify.run([f'0x{A:08x}'], jobs=1)
    assert tree.built == ['a.cpp']


def test_a_new_header_no_unit_reads_rebuilds_nothing_else(tree):
    (tree.game / 'untracked.h').write_text('int u;\n')
    verify.run([f'0x{A:08x}'], jobs=1)
    assert tree.built == ['a.cpp']


def test_reads_any_follows_nested_includes_and_probes(tree):
    cfg = {}
    shared = {str((tree.game / 'shared.h').resolve())}
    assert source.reads_any(cfg, str(tree.game), str(tree.game / 'b.cpp'), shared)
    assert not source.reads_any(cfg, str(tree.game), str(tree.game / 'c.cpp'), shared)
    # A deleted header is still matched through the probe of its old path.
    (tree.game / 'shared.h').unlink()
    assert source.reads_any(cfg, str(tree.game), str(tree.game / 'b.cpp'),
                            {str(tree.game.resolve() / 'shared.h')})


def test_reads_any_takes_a_unit_with_a_macro_include(tree):
    (tree.game / 'c.cpp').write_text('#include HEADER\n')
    assert source.reads_any({}, str(tree.game), str(tree.game / 'c.cpp'), {'/nowhere.h'})


def test_changed_files_lists_modified_and_untracked_files(tree):
    (tree.game / 'shared.h').write_text('changed\n')
    (tree.game / 'new.h').write_text('new\n')
    root = tree.game.resolve()
    assert source.changed_files(str(tree.game)) == {str(root / 'shared.h'), str(root / 'new.h')}


def test_changed_files_is_empty_outside_a_checkout(tmp_path):
    assert source.changed_files(str(tmp_path)) == set()


def test_a_git_failure_inside_a_checkout_is_a_warning(tree, monkeypatch):
    (tree.game / 'shared.h').write_text('struct S { int x; int y; };\n')
    real = subprocess.run

    def run(cmd, *a, **k):
        if cmd[:2] == ['git', 'diff']:
            return subprocess.CompletedProcess(cmd, 128, '', 'fatal: bad revision')
        return real(cmd, *a, **k)

    monkeypatch.setattr(source.subprocess, 'run', run)
    assert source.changed_files(str(tree.game)) is None
    rep = verify.run([f'0x{A:08x}'], jobs=1)
    assert tree.built == ['a.cpp']
    assert any(ln.startswith('WARN') and 'git could not list' in ln for ln in rep.lines)
