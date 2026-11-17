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
