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
# A workflow with a job on the owner's machine may only be started by one of these.
SELF_HOSTED_TRIGGERS = {"push", "schedule", "workflow_dispatch"}
SELF_HOSTED_WORDS = ("self-hosted", "sfcop-mac")


def indent(line):
    return len(line) - len(line.lstrip(" "))


def meaningful(line):
    return bool(line.strip()) and not line.strip().startswith("#")


def block_after(lines, i):
    """The lines under lines[i] that are indented deeper than it (comments and blanks skipped)."""
    base, out = indent(lines[i]), []
    for nxt in lines[i + 1:]:
        if not meaningful(nxt):
            continue
        if indent(nxt) <= base:
            break
        out.append(nxt)
    return out


def triggers(text):
    """The event names in a workflow's top-level `on:`: a name, a [list], a `- name` list or a block."""
    lines = text.splitlines()
    for i, line in enumerate(lines):
        m = re.match(r"^(?:on|\"on\"|'on'):\s*(.*?)\s*(?:#.*)?$", line)
        if not m:
            continue
        if m.group(1):
            return set(re.findall(r"[a-z_]+", m.group(1).strip("[]")))
        body = block_after(lines, i)
        if not body:
            return set()
        top = min(indent(l) for l in body)
        found = set()
        for l in body:
            if indent(l) == top:
                k = re.match(r"^\s*(?:-\s*)?([a-z_]+)\s*:?\s*(?:#.*)?$", l) or \
                    re.match(r"^\s*([a-z_]+):", l)
                if k:
                    found.add(k.group(1))
        return found
    return set()


def self_hosted_jobs(text):
    """Names of the jobs with any `runs-on` value (inline, list, or labels:/group: block) that
    names self-hosted or the owner's runner label."""
    lines = text.splitlines()
    out = []
    for i, line in enumerate(lines):
        if not re.match(r"^jobs:\s*$", line):
            continue
        body = block_after(lines, i)
        top = min(indent(l) for l in body)
        job = None
        for j, l in enumerate(body):
            if indent(l) == top and re.match(r"^\s*[\w-]+:\s*$", l):
                job = l.strip().rstrip(":")
            r = re.match(r"^\s*runs-on:(.*)$", l)
            if r and job:
                value = r.group(1) + " ".join(block_after(body, j))
                if any(w in value for w in SELF_HOSTED_WORDS) and job not in out:
                    out.append(job)
    return out


def disallowed_self_hosted(text):
    """The triggers a workflow with a self-hosted job may not have."""
    return triggers(text) - SELF_HOSTED_TRIGGERS if self_hosted_jobs(text) else set()


def run_blocks(text):
    """The text of every `run:` step: its inline value and the lines under it."""
    lines, out = text.splitlines(), []
    for i, l in enumerate(lines):
        m = re.match(r"^\s*(?:-\s*)?run:(.*)$", l)
        if m:
            out.append(m.group(1) + "\n" + "\n".join(block_after(lines, i)))
    return out


def test_trigger_parser_forms():
    assert triggers("on:\n  pull_request:\n    types: [opened]\n  workflow_dispatch:\npermissions: {}\n") \
        == {"pull_request", "workflow_dispatch"}
    assert triggers("on: [push, workflow_run]\njobs: {}\n") == {"push", "workflow_run"}
    assert triggers("on: push\n") == {"push"}
    assert triggers("on:\n  - push\n  - pull_request_target\njobs:\n") == {"push", "pull_request_target"}
    assert triggers("on:\n    push:\n      branches: [x]\n    schedule:\n      - cron: '0 3 * * 0'\n") \
        == {"push", "schedule"}


def test_self_hosted_parser_forms():
    def doc(runs_on):
        return "jobs:\n  a:\n    runs-on: ubuntu-latest\n  b:\n" + runs_on
    assert self_hosted_jobs(doc("    runs-on: [self-hosted, x]\n")) == ["b"]
    assert self_hosted_jobs(doc("    runs-on: sfcop-mac\n")) == ["b"]
    assert self_hosted_jobs(doc("    runs-on:\n      - self-hosted\n      - mac\n")) == ["b"]
    assert self_hosted_jobs(doc("    runs-on:\n      labels: [sfcop-mac]\n")) == ["b"]
    assert self_hosted_jobs(doc("    runs-on:\n      group: self-hosted-pool\n")) == ["b"]
    assert self_hosted_jobs(doc("    runs-on: ubuntu-latest\n")) == []
    assert self_hosted_jobs("jobs:\n    c:\n        runs-on: [self-hosted]\n") == ["c"]


def test_disallowed_self_hosted_triggers():
    jobs = "jobs:\n  b:\n    runs-on: [self-hosted, x]\n"
    assert disallowed_self_hosted("on:\n  pull_request:\n" + jobs) == {"pull_request"}
    assert disallowed_self_hosted("on: [push, issues]\n" + jobs) == {"issues"}
    assert disallowed_self_hosted("on:\n  - push\n  - workflow_run\n" + jobs) == {"workflow_run"}
    assert disallowed_self_hosted("on:\n  push:\n  workflow_dispatch:\n" + jobs) == set()
    hosted = "jobs:\n  b:\n    runs-on: ubuntu-latest\n"
    assert disallowed_self_hosted("on:\n  pull_request:\n" + hosted) == set()


def test_run_block_parser():
    text = "steps:\n  - run: echo ${{ x }}\n  - name: n\n    run: |\n      a\n      b\n  - uses: u\n"
    blocks = run_blocks(text)
    assert len(blocks) == 2 and "echo" in blocks[0] and "a" in blocks[1] and "u" not in blocks[1]


def test_only_push_schedule_and_dispatch_start_a_self_hosted_job():
    files = sorted(WORKFLOWS.glob("*.yml"))
    assert files, "no workflow files found"
    with_job = {f.name for f in files if self_hosted_jobs(f.read_text())}
    assert {"check.yml", "docker-audit.yml"} <= with_job, "the parser lost the self-hosted jobs"
    bad = {f.name: disallowed_self_hosted(f.read_text()) for f in files}
    assert {n: t for n, t in bad.items() if t} == {}


def test_check_refuses_an_outside_commit_before_it_runs_any_local_script():
    text = (WORKFLOWS / "check.yml").read_text()
    lines = text.splitlines()
    refuse = next(i for i, l in enumerate(lines) if "git branch -r --contains HEAD" in l)
    local = [i for i, l in enumerate(lines) if re.match(r"^\s*(?:-\s*)?uses:\s*\./", l)]
    assert local and refuse < min(local)
    assert "fetch-depth: 0" in code_lines(".github", "workflows", "check.yml")
    assert not [b for b in run_blocks(text) if "inputs." in b]


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
