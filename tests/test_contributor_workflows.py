"""The workflows for outside contributors never put a fork's code on the self-hosted runner.

Read from the files' text, as tests/test_workflow_pins.py does; nothing is run.
"""
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]
WF = ROOT / '.github' / 'workflows'


def code_lines(name):
    text = (WF / name).read_text()
    return [l.strip() for l in text.splitlines() if l.strip() and not l.strip().startswith('#')]


def test_the_contributor_gate_runs_on_hosted_runners_only():
    lines = code_lines('contributor-gate.yml')
    assert [l for l in lines if l.startswith('runs-on:')] == ['runs-on: ubuntu-latest'] * 2
    assert 'pull_request_target:' not in lines
    assert lines.count('persist-credentials: false') == 2
    assert 'if: github.event.pull_request.head.repo.full_name != github.repository' in lines
    assert 'CHECK_SKIP_TESTS: "1"' in lines and 'run: sh scripts/check.sh' in lines
    assert any(l.startswith('python3 -I tools/contrib_guard.py') for l in lines)
    perms = lines[lines.index('permissions:') + 1:lines.index('permissions:') + 4]
    assert perms == ['contents: read', 'pull-requests: read', 'issues: read']


def test_the_build_review_is_started_by_hand_and_checks_out_develop():
    lines = code_lines('build-review.yml')
    on = lines[lines.index('on:') + 1]
    assert on == 'workflow_dispatch:'
    assert 'pull_request:' not in lines and 'pull_request_target:' not in lines
    assert 'runs-on: [self-hosted, sfcop-mac]' in lines
    assert 'ref: develop' in lines and 'persist-credentials: false' in lines
    assert 'compiler: native' in lines and 'HARNESS_COMPILER: native' in lines
    assert 'timeout-minutes: 60' in lines
    i = lines.index('permissions:')
    assert lines[i + 1:i + 6] == ['contents: read', 'issues: read', 'pull-requests: write',
                                  'statuses: write', 'actions: write']
    assert any('tools/verify_pr.py "$PR" --sha "$SHA"' in l for l in lines)


def test_claim_expiry_is_hosted_and_passes_the_date():
    lines = code_lines('claim-expiry.yml')
    assert 'runs-on: ubuntu-latest' in lines and 'schedule:' in lines
    assert 'issues: write' in lines
    assert any(l.startswith('run: python3 -I tools/claims.py expire --now') for l in lines)


def test_the_claim_form_labels_its_issue_claim():
    text = (ROOT / '.github' / 'ISSUE_TEMPLATE' / 'claim.yml').read_text()
    assert 'labels: [claim]' in text and 'range: 0x' in text
