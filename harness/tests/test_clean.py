"""harness.lib.clean removes the build/ folder of an orphaned worktree and nothing else."""
import os

from harness.lib import clean


def state(root, name, record=None):
    folder = root / name
    (folder / 'build' / 'unit').mkdir(parents=True)
    (folder / 'build' / 'unit' / 'a.obj').write_bytes(b'x' * 100)
    (folder / 'build' / 'b.txt').write_bytes(b'y' * 20)
    (folder / 'attempts').mkdir()
    (folder / 'attempts' / 'log').write_text('keep')
    (folder / 'queue.json').write_text('[]')
    if record is not None:
        (folder / clean.RECORD).write_text(record + '\n')
    return folder


def test_a_gone_worktree_loses_its_build_folder_and_keeps_the_rest(tmp_path):
    folder = state(tmp_path, 'wt-a-11111111', str(tmp_path / 'no-such-worktree'))
    out = []
    freed = clean.clean(str(tmp_path), out=type('W', (), {'write': out.append}))
    assert freed == 120
    assert not (folder / 'build').exists()
    assert (folder / 'attempts' / 'log').read_text() == 'keep'
    assert (folder / 'queue.json').exists() and (folder / clean.RECORD).exists()
    assert out[0].startswith(f'removed {folder}/build (120 bytes) worktree gone')
    assert out[-1] == 'freed 120 bytes\n'


def test_a_worktree_that_still_exists_is_untouched(tmp_path):
    live = tmp_path / 'live'
    live.mkdir()
    folder = state(tmp_path, 'wt-b-22222222', str(live))
    assert clean.clean(str(tmp_path), out=open(os.devnull, 'w')) == 0
    assert (folder / 'build' / 'b.txt').exists()


def test_a_folder_without_a_record_is_untouched_unless_asked(tmp_path):
    folder = state(tmp_path, 'wt-c-33333333')
    assert clean.clean(str(tmp_path), out=open(os.devnull, 'w')) == 0
    assert (folder / 'build').exists()
    assert clean.clean(str(tmp_path), unrecorded=True, out=open(os.devnull, 'w')) == 120
    assert not (folder / 'build').exists()
    assert (folder / 'attempts' / 'log').exists()


def test_a_dry_run_removes_nothing(tmp_path):
    folder = state(tmp_path, 'wt-d-44444444', str(tmp_path / 'gone'))
    out = []
    assert clean.clean(str(tmp_path), dry_run=True, out=type('W', (), {'write': out.append})) == 120
    assert (folder / 'build' / 'unit' / 'a.obj').exists()
    assert out[-1] == 'would free 120 bytes\n'


def test_other_folders_are_never_touched(tmp_path):
    other = tmp_path / 'shared'
    (other / 'build').mkdir(parents=True)
    clean.clean(str(tmp_path), unrecorded=True, out=open(os.devnull, 'w'))
    assert (other / 'build').exists()


def test_the_record_holds_the_worktree_path(tmp_path):
    clean.record_worktree(str(tmp_path / 'st'), '/some/worktree')
    assert (tmp_path / 'st' / 'worktree.txt').read_text() == '/some/worktree\n'
    clean.record_worktree(str(tmp_path / 'st'), '/other')
    assert (tmp_path / 'st' / 'worktree.txt').read_text() == '/other\n'
