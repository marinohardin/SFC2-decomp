"""The main checkout is found from a plain clone, a worktree and a submodule."""
import os
import subprocess

from harness.lib import paths


def git(cwd, *args):
    env = dict(os.environ, GIT_AUTHOR_NAME='Test', GIT_AUTHOR_EMAIL='test@example.invalid',
               GIT_COMMITTER_NAME='Test', GIT_COMMITTER_EMAIL='test@example.invalid')
    subprocess.run(['git', '-c', 'protocol.file.allow=always', '-c', 'init.defaultBranch=main',
                    *args], cwd=cwd, env=env, check=True, capture_output=True)


def make_repo(path):
    os.makedirs(path)
    git(path, 'init', '-q')
    git(path, 'commit', '-q', '--allow-empty', '-m', 'start')
    return path


def test_a_plain_clone_is_its_own_main_checkout(tmp_path):
    repo = make_repo(str(tmp_path / 'decomp'))
    assert os.path.realpath(paths._main_checkout(repo)) == os.path.realpath(repo)


def test_a_worktree_finds_the_checkout_it_came_from(tmp_path):
    repo = make_repo(str(tmp_path / 'decomp'))
    tree = str(tmp_path / 'trees' / 'one')
    git(repo, 'worktree', 'add', '-q', '-b', 'one', tree)
    assert os.path.realpath(paths._main_checkout(tree)) == os.path.realpath(repo)


def test_a_submodule_finds_the_repository_that_contains_it(tmp_path):
    decomp = make_repo(str(tmp_path / 'decomp'))
    port = make_repo(str(tmp_path / 'port'))
    git(port, 'submodule', 'add', '-q', decomp, 'decomp')
    inner = os.path.join(port, 'decomp')
    assert os.path.realpath(paths._main_checkout(inner)) == os.path.realpath(port)


def test_a_folder_outside_git_is_its_own_main_checkout(tmp_path):
    loose = str(tmp_path / 'loose')
    os.makedirs(loose)
    assert paths._main_checkout(loose) == loose
