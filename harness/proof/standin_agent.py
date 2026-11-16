#!/usr/bin/env python3
"""A scripted stand-in for `claude -p`, for harness/proof/loop_proof.sh --standin.

It drives the same hooks and scripts a real function session would, in the order a session
would, and prints a result in the shape of `claude -p --output-format json`:

1. asks the PreToolUse guard to edit analysis/results/matched_set.json and to run
   update-matched-set.sh (both must be denied);
2. asks the Stop hook to let it finish before doing anything (must be blocked);
3. claims the function by adding the annotation before its definition (the guard must allow
   the edit), and runs harness/build-and-verify.sh;
4. asks the Stop hook again (must allow).

With HARNESS_STANDIN_TAMPER set to a marker path that does not exist yet, it also writes a file
under harness/ behind the guard's back (as a session that found a hole in it would) and creates
the marker, so only the first round tampers: loop.sh must refuse to score that round.

It only knows how to claim functions whose definition is already in the tree; it is a test
fixture for the loop, not a decompiler.
"""
import json
import os
import re
import subprocess
import sys

HARNESS = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPO = os.path.dirname(HARNESS)
DEFINITIONS = {'0x006fc750': 'void tDynamicAllocator::LockDownAllocator()'}


def hook(script, event):
    p = subprocess.run([sys.executable, os.path.join(HARNESS, 'hooks', script)],
                       input=json.dumps(event), capture_output=True, text=True,
                       env=dict(os.environ, CLAUDE_PROJECT_DIR=REPO))
    return json.loads(p.stdout) if p.stdout.strip() else {}


def say(msg):
    print(msg, file=sys.stderr)
    log.append(msg)


log = []


def main(argv):
    prompt = argv[argv.index('-p') + 1]
    address = re.search(r'Original address: (0x[0-9a-f]{8})', prompt).group(1)
    src = os.path.join(REPO, os.environ.get('HARNESS_SRC_ROOT', 'op'))
    matched = os.path.join(REPO, 'analysis', 'results', 'matched_set.json')
    for ev in ({'tool_name': 'Edit', 'tool_input': {'file_path': matched}},
               {'tool_name': 'Bash', 'tool_input': {'command': f'harness/update-matched-set.sh {address}'}}):
        d = hook('guard.py', ev).get('hookSpecificOutput', {})
        say(f'guard {ev["tool_name"]} {ev["tool_input"]}: {d.get("permissionDecision", "allow")}')
    d = hook('stop_verify.py', {'hook_event_name': 'Stop', 'stop_hook_active': False})
    say(f'stop before any change: {d.get("decision", "allow")}')

    definition = DEFINITIONS[address]
    for d_, _, files in os.walk(src):
        for name in files:
            path = os.path.join(d_, name)
            if not name.endswith('.cpp'):
                continue
            text = open(path, encoding='latin1', newline='').read()
            if definition in text:
                ev = {'tool_name': 'Edit', 'tool_input': {'file_path': path}}
                d = hook('guard.py', ev).get('hookSpecificOutput', {})
                say(f'guard Edit {os.path.relpath(path, REPO)}: {d.get("permissionDecision", "allow")}')
                text = text.replace(definition, f'// FUNCTION: STARFLEETOP {address}\n{definition}', 1)
                open(path, 'w', encoding='latin1', newline='').write(text)
    marker = os.environ.get('HARNESS_STANDIN_TAMPER')
    if marker and not os.path.exists(marker):
        open(os.path.join(HARNESS, 'lib', 'planted.py'), 'w').write('# planted\n')
        open(marker, 'w').close()
        say('tamper: wrote harness/lib/planted.py without asking the guard')
    p = subprocess.run([os.path.join(HARNESS, 'build-and-verify.sh'), address],
                       capture_output=True, text=True, cwd=REPO)
    for line in p.stdout.splitlines():
        say('  ' + line)
    d = hook('stop_verify.py', {'hook_event_name': 'Stop', 'stop_hook_active': True})
    say(f'stop after the change: {d.get("decision", "allow")} {d.get("systemMessage", "")}')
    print(json.dumps({'type': 'result', 'subtype': 'success', 'is_error': False,
                      'result': 'stand-in agent:\n' + '\n'.join(log), 'num_turns': 0,
                      'total_cost_usd': 0, 'usage': {'input_tokens': 0, 'output_tokens': 0,
                                                     'cache_read_input_tokens': 0,
                                                     'cache_creation_input_tokens': 0}}))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
