"""scripts/check.sh's base_ref(): the commit the branch is held to.

A pull request's CI checkout is a merge commit whose first parent is the base. A push whose tip
happens to be a merge commit (develop merged into the branch) is not that: its base is the merge
base with origin/develop, never the branch's previous tip.
"""
import os
import pathlib
import re
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]


def base_ref_function():
    text = (ROOT / "scripts" / "check.sh").read_text()
    m = re.search(r"^base_ref\(\) \{\n.*?^\}\n", text, re.S | re.M)
    assert m, "base_ref() not found in scripts/check.sh"
    return m.group(0)


def git(repo, *args):
    return subprocess.run(["git", "-C", str(repo), *args], check=True, capture_output=True,
                          text=True).stdout.strip()


def commit(repo, name):
    (repo / name).write_text(name)
    git(repo, "add", name)
    git(repo, "-c", "user.name=t", "-c", "user.email=t@example.invalid", "commit", "-qm", name)
    return git(repo, "rev-parse", "HEAD")


def history(tmp_path):
    """develop: d1 d2; branch: cut at d1, b1, then develop (d2) merged in as m."""
    repo = tmp_path / "r"
    repo.mkdir()
    git(repo, "init", "-q", "-b", "develop")
    d1 = commit(repo, "d1")
    git(repo, "checkout", "-q", "-b", "work")
    b1 = commit(repo, "b1")
    git(repo, "checkout", "-q", "develop")
    d2 = commit(repo, "d2")
    git(repo, "update-ref", "refs/remotes/origin/develop", d2)
    git(repo, "checkout", "-q", "work")
    git(repo, "-c", "user.name=t", "-c", "user.email=t@example.invalid", "merge", "-q", "--no-ff",
        "-m", "m", "develop")
    return repo, d1, b1, d2


def run_base_ref(repo, **env):
    script = base_ref_function() + "base_ref\n"
    clean = {k: v for k, v in os.environ.items()
             if k not in ("GITHUB_ACTIONS", "GITHUB_EVENT_NAME", "SFCOP_BASE_REF")}
    clean.update(env)
    return subprocess.run(["sh", "-c", script], cwd=repo, env=clean, capture_output=True,
                          text=True).stdout.strip()


def test_a_push_whose_tip_is_a_merge_is_held_to_the_merge_base_with_develop(tmp_path):
    repo, d1, b1, d2 = history(tmp_path)
    got = run_base_ref(repo, GITHUB_ACTIONS="true", GITHUB_EVENT_NAME="push")
    # The merge base of the tip and develop is develop's tip d2 (already merged in), not b1,
    # which is the first parent of the tip.
    assert got == d2
    assert got != git(repo, "rev-parse", "HEAD^1")


def test_a_pull_request_checkout_is_held_to_its_first_parent(tmp_path):
    repo, d1, b1, d2 = history(tmp_path)
    got = run_base_ref(repo, GITHUB_ACTIONS="true", GITHUB_EVENT_NAME="pull_request")
    assert got == b1


def test_an_explicit_base_wins(tmp_path):
    repo, d1, b1, d2 = history(tmp_path)
    assert run_base_ref(repo, SFCOP_BASE_REF="abc") == "abc"
