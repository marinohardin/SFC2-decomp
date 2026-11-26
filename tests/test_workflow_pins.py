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


def test_check_scores_with_the_native_compiler():
    lines = code_lines(".github", "workflows", "check.yml")
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


WORKFLOWS = ROOT / ".github" / "workflows"
# Events whose workflow file or payload a stranger can influence.
UNSAFE_EVENTS = {"pull_request", "pull_request_target", "issue_comment", "pull_request_review",
                 "pull_request_review_comment", "workflow_run"}


def triggers(text):
    """The event names in a workflow's top-level `on:` (block, list or single-name form)."""
    lines = text.splitlines()
    for i, line in enumerate(lines):
        m = re.match(r"^(?:on|\"on\"|'on'):\s*(.*?)\s*(?:#.*)?$", line)
        if not m:
            continue
        inline = m.group(1)
        if inline:
            return set(re.findall(r"[a-z_]+", inline.strip("[]")))
        found = set()
        for nxt in lines[i + 1:]:
            if nxt.strip() and not nxt.startswith(" ") and not nxt.startswith("#"):
                break
            k = re.match(r"^  ([a-z_]+):", nxt)
            if k:
                found.add(k.group(1))
        return found
    return set()


def self_hosted_jobs(text):
    """Names of the jobs whose `runs-on:` mentions self-hosted."""
    out, job = [], None
    for line in text.splitlines():
        m = re.match(r"^  ([\w-]+):\s*$", line)
        if m:
            job = m.group(1)
        r = re.match(r"^    runs-on:(.*)$", line)
        if r and job and "self-hosted" in r.group(1):
            out.append(job)
    return out


def unsafe_self_hosted(text):
    """Self-hosted jobs of a workflow that an unsafe event triggers."""
    return self_hosted_jobs(text) if triggers(text) & UNSAFE_EVENTS else []


def test_trigger_and_job_parsers_on_hand_written_text():
    block = "name: x\non:\n  pull_request:\n    types: [opened]\n  workflow_dispatch:\npermissions: {}\n"
    assert triggers(block) == {"pull_request", "workflow_dispatch"}
    assert triggers("on: [push, workflow_run]\n") == {"push", "workflow_run"}
    assert triggers("on: push\n") == {"push"}
    jobs = "jobs:\n  a:\n    runs-on: ubuntu-latest\n  b:\n    runs-on: [self-hosted, x]\n"
    assert self_hosted_jobs(jobs) == ["b"]
    assert unsafe_self_hosted("on:\n  pull_request:\n" + jobs) == ["b"]
    assert unsafe_self_hosted("on:\n  push:\n" + jobs) == []


def test_no_self_hosted_job_runs_on_an_event_a_stranger_can_influence():
    files = sorted(WORKFLOWS.glob("*.yml"))
    assert files, "no workflow files found"
    bad = {f.name: unsafe_self_hosted(f.read_text()) for f in files}
    assert {n: j for n, j in bad.items() if j} == {}


def test_check_runs_on_push_and_dispatch_only():
    text = (WORKFLOWS / "check.yml").read_text()
    assert triggers(text) == {"push", "workflow_dispatch"}
    assert self_hosted_jobs(text) == ["check"]


def test_docker_audit_is_schedule_and_dispatch_only():
    assert triggers((WORKFLOWS / "docker-audit.yml").read_text()) == {"schedule", "workflow_dispatch"}


def test_review_can_be_started_by_hand_and_never_checks_out_the_pull_request_head():
    text = (WORKFLOWS / "claude-code-review.yml").read_text()
    assert triggers(text) == {"pull_request", "workflow_dispatch"}
    lines = code_lines(".github", "workflows", "claude-code-review.yml")
    assert "pr:" in lines and "required: true" in lines
    assert any("inputs.pr" in l for l in lines)
    # No checkout names a ref: the default is the base, and the pull request is read through gh.
    for i, l in enumerate(lines):
        if l.startswith("uses: actions/checkout@") or l.startswith("- uses: actions/checkout@"):
            tail = lines[i + 1:i + 6]
            assert not [t for t in tail if t.startswith(("ref:", "repository:"))]
    assert not re.search(r"head\.(ref|sha)|refs/pull|pull/\S*/head", "\n".join(
        l for l in lines if "AFTER:" not in l and "head_ref" not in l and "head.repo" not in l))
