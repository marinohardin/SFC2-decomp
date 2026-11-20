"""Every action a workflow uses from another repository is pinned to a full commit SHA.

A tag such as `v4` can be moved to other code by whoever controls that repository; a commit SHA
cannot. One job runs on a self-hosted runner, so an action that changed under a tag would run there.
Local actions (`./.github/actions/...`) are part of this repository and need no pin.
"""
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[1]
USES = re.compile(r"^\s*(?:-\s*)?uses:\s*(\S+)")
PINNED = re.compile(r"^[\w.-]+/[\w./-]+@[0-9a-f]{40}$")


def unpinned(text):
    """Return the `uses:` targets in a workflow's text that are neither local nor pinned."""
    out = []
    for line in text.splitlines():
        m = USES.match(line)
        if m and not m.group(1).startswith("./") and not PINNED.match(m.group(1)):
            out.append(m.group(1))
    return out


def test_unpinned_finds_tags_and_branches():
    text = (
        "      - uses: actions/checkout@v4\n"
        "        uses: owner/action@main\n"
        "      - uses: ./.github/actions/setup\n"
        "      - uses: actions/checkout@11d5960a326750d5838078e36cf38b85af677262 # v4.4.0\n"
    )
    assert unpinned(text) == ["actions/checkout@v4", "owner/action@main"]


def test_every_workflow_action_is_pinned():
    files = sorted((ROOT / ".github").rglob("*.yml"))
    assert files, "no workflow files found"
    bad = {str(f.relative_to(ROOT)): unpinned(f.read_text()) for f in files}
    assert {f: u for f, u in bad.items() if u} == {}


def code_lines(*parts):
    """The stripped, non-comment, non-empty lines of a file under the repository root."""
    text = ROOT.joinpath(*parts).read_text()
    return [l.strip() for l in text.splitlines() if l.strip() and not l.strip().startswith("#")]


def test_pull_request_check_scores_with_the_native_compiler():
    lines = code_lines(".github", "workflows", "pull-request.yml")
    assert 'HARNESS_COMPILER: native' in lines
    assert not [l for l in lines if l.startswith("HARNESS_COMPILER: docker")]
    assert "compiler: native" in lines


def test_docker_audit_runs_the_check_with_the_image_on_a_schedule():
    lines = code_lines(".github", "workflows", "docker-audit.yml")
    for line in ("HARNESS_COMPILER: docker", "schedule:", "workflow_dispatch:",
                 "timeout-minutes: 180", "group: docker-audit", "cancel-in-progress: false",
                 "runs-on: [self-hosted, sfcop-mac]", "compiler: docker",
                 "nice -n 5 sh scripts/check.sh 2>&1 | tee \"$log\""):
        assert line in lines, line


def test_docker_audit_forces_the_rebuild_and_fails_when_it_did_not_run():
    lines = code_lines(".github", "workflows", "docker-audit.yml")
    # The base is the checked-out commit: no changed file, so check.sh rebuilds everything.
    assert "export SFCOP_BASE_REF=$(git rev-parse HEAD)" in lines
    assert "set -eu -o pipefail" in lines
    assert any(l.startswith("grep -q '^compiler: the sfcop-msvc6 image'") and l.endswith("exit 1; }")
               for l in lines)
    assert any(l.startswith("grep -qx 'RESULT OK'") and l.endswith("exit 1; }") for l in lines)


def test_setup_action_checks_docker_only_for_the_docker_compiler():
    lines = code_lines(".github", "actions", "setup", "action.yml")
    native = lines.index('if [ "${{ inputs.compiler }}" = native ]; then')
    other = lines.index("else")
    docker = [i for i, l in enumerate(lines) if l.startswith("docker version")]
    assert native < other and docker and all(i > other for i in docker)
    assert any("sh toolchain/fetch-native.sh" in l for l in lines[native:other])
