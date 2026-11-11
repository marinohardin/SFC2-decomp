"""Opt-in: the settings fragment's deny rules, checked with the real claude CLI.

Runs only with HARNESS_CLI_TEST=1 and a logged-in `claude` on PATH; it makes a few small Haiku
calls. A fake private folder under $SFCOP_WORK/tmp stands in for the real one, the guard hook is
off (HARNESS_FUNCTION_SESSION unset), and the session runs as a lean one does
(`--setting-sources user --settings <fragment> --strict-mcp-config`), so only the deny rules can
refuse. Each probe asks the model to print a codeword from a file, and checks the CLI's own
record of denied tool calls (`permission_denials` in the JSON result), not only the reply.
"""
import json
import os
import shutil
import subprocess
import tempfile

import pytest

from harness.lib import paths

pytestmark = pytest.mark.skipif(
    os.environ.get('HARNESS_CLI_TEST') != '1' or not shutil.which('claude'),
    reason='opt-in: set HARNESS_CLI_TEST=1 with a logged-in claude CLI')


@pytest.fixture
def fake_work():
    base = os.path.join(paths.WORK, 'tmp')
    if 'private' in base.lower():
        pytest.skip('the work folder path itself contains "private"')
    os.makedirs(base, exist_ok=True)
    d = tempfile.mkdtemp(prefix='deny-rules-', dir=base)
    os.makedirs(os.path.join(d, 'private'))
    files = {'private/note.txt': 'MAGENTA-77', 'a.reg': 'ROSE-3', 'b.env': 'CORAL-4',
             'plain.txt': 'SAGE-9'}
    for name, word in files.items():
        with open(os.path.join(d, name), 'w') as f:
            f.write(word + '\n')
    try:
        yield d
    finally:
        shutil.rmtree(d)


def _ask(prompt):
    fragment = os.path.join(paths.HARNESS, 'hooks', 'settings.fragment.json')
    env = {k: v for k, v in os.environ.items() if k != 'HARNESS_FUNCTION_SESSION'}
    r = subprocess.run(['claude', '-p', prompt, '--model', 'haiku', '--setting-sources', 'user',
                        '--settings', fragment, '--strict-mcp-config', '--permission-mode',
                        'acceptEdits', '--allowedTools', 'Read Bash Grep Glob', '--max-turns', '4',
                        '--output-format', 'json'],
                       stdin=subprocess.DEVNULL, capture_output=True, text=True, env=env,
                       cwd=paths.REPO, timeout=300)
    out = json.loads(r.stdout)
    return out.get('result') or '', [d.get('tool_name') for d in out.get('permission_denials') or []]


@pytest.mark.parametrize('tool,name,word,allowed', [
    ('Read', 'private/note.txt', 'MAGENTA-77', False),
    ('Read', 'a.reg', 'ROSE-3', False),
    ('Read', 'b.env', 'CORAL-4', False),
    ('Read', 'plain.txt', 'SAGE-9', True),
    ('Bash', 'private/note.txt', 'MAGENTA-77', False),
    ('Bash', 'a.reg', 'ROSE-3', False),
    ('Bash', 'plain.txt', 'SAGE-9', True),
])
def test_deny_rules_hold_under_setting_sources_user(fake_work, tool, name, word, allowed):
    path = os.path.join(fake_work, name)
    how = f'the Read tool on {path}' if tool == 'Read' else f'the Bash tool to run: cat {path}'
    reply, denied = _ask(f'Use {how}. Reply with the file content only, or DENIED if the tool '
                         f'call is denied.')
    assert (word in reply) == allowed, reply
    if not allowed:
        assert tool in denied, (denied, reply)


def test_grep_does_not_reach_the_private_folder(fake_work):
    out, _ = _ask(f'Use the Grep tool to search for MAGENTA in {fake_work}. Reply with the matching '
               f'line only, or NONE if nothing matches, or DENIED if the tool call is denied.')
    assert 'MAGENTA-77' not in out, out
