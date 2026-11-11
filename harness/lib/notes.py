"""Handoff notes: what an unfinished attempt leaves for the next one on the same function.

    python3 -m harness.lib.notes write <entry.json> <outcome> <before> <after> <claude.json> <verify.json>
    python3 -m harness.lib.notes show <address>

A note is written by the loop after every scored attempt that did not end exact, into
$HARNESS_SHARED/notes/<address>.md (the harness state, never the repository). It holds the
attempt's model and scores, the remaining differences from the verifier, and the session's own
account: the `HANDOFF:` section the prompt asks it to end with (what it tried, what it suspects),
or the end of its last message when it wrote none. The next attempt's prompt carries the latest
notes (`latest`).
"""
import json
import os
import re
import sys

from . import paths

HANDOFF = re.compile(r'(?:^|\n)\s*(?:#+\s*)?\**HANDOFF\**:?\**[ \t]*\n?(.*?)(?=\n#+\s|\Z)', re.S | re.I)
HANDOFF_MAX = 1200
TAIL_MAX = 600
DIFF_LINES = 8
SHOWN = 2          # notes the next prompt carries
SHOWN_MAX = 3000   # characters of notes the next prompt carries
SEPARATOR = '\n<!-- note -->\n'


def notes_path(address, directory=None):
    return os.path.join(directory or paths.NOTES, f'{address}.md')


def _clip(text, limit):
    text = text.strip()
    return text if len(text) <= limit else text[:limit].rstrip() + ' ...(cut)'


def session_account(out):
    """The session's handoff section, or the end of its last message; '' when there is none."""
    result = (out or {}).get('result') or ''
    if not isinstance(result, str) or not result.strip():
        return ''
    m = None
    for m in HANDOFF.finditer(result):
        pass
    if m and m.group(1).strip():
        return _clip(m.group(1), HANDOFF_MAX)
    tail = result.strip()[-TAIL_MAX:]
    return '(no HANDOFF section; the end of its last message)\n' + tail


def remaining_diff(verify, address):
    """Lines about `address` from a build-and-verify --json result: its score line and the
    differing instructions and relocation errors under it, at most DIFF_LINES of the latter."""
    if not verify:
        return ['(no verifier output)']
    lines = verify.get('lines') or []
    out, inside = [], False
    for line in lines:
        head = not line[6:8].isspace() if len(line) > 8 else True
        if head:
            if inside:
                break
            inside = address in line
            if inside:
                out.append(line.strip())
        elif inside and len(out) <= DIFF_LINES and 'side by side:' not in line:
            out.append(line.strip())
    return out or [f'(nothing about {address} in the verifier output)']


def compose(entry, outcome, before, after, out, verify, when=''):
    model = entry.get('model', '')
    n = entry.get('attempts', 0) + 1
    parts = [f'### Attempt {n} ({model}, {when}): {outcome}, {float(before):.2f}% -> '
             f'{float(after):.2f}%', '', 'Remaining differences (verifier):', '']
    parts += ['    ' + x for x in remaining_diff(verify, entry['address'])]
    account = session_account(out)
    parts += ['', 'The session\'s handoff:', '']
    parts += [account or '(the session left no message)']
    return '\n'.join(parts).rstrip() + '\n'


def write(entry, outcome, before, after, out, verify, when='', directory=None):
    path = notes_path(entry['address'], directory)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'a') as f:
        if f.tell():
            f.write(SEPARATOR)
        f.write(compose(entry, outcome, before, after, out, verify, when))
    return path


def latest(address, directory=None, count=SHOWN, limit=SHOWN_MAX):
    """The newest `count` notes on the function, newest last, within `limit` characters (older
    ones are dropped first, then the oldest kept one is cut). '' when there are none."""
    try:
        with open(notes_path(address, directory)) as f:
            notes = [n.strip() for n in f.read().split(SEPARATOR) if n.strip()]
    except OSError:
        return ''
    notes = notes[-count:]
    while len(notes) > 1 and sum(len(n) + 2 for n in notes) > limit:
        notes.pop(0)
    text = '\n\n'.join(notes)
    return _clip(text, limit) if len(text) > limit else text


def main(argv):
    def load(p):
        try:
            with open(p) as f:
                return json.load(f)
        except (OSError, ValueError):
            return None
    if argv[:1] == ['write'] and len(argv) >= 7:
        entry = load(argv[1])
        print(write(entry, argv[2], argv[3], argv[4], load(argv[5]), load(argv[6]),
                    argv[7] if len(argv) > 7 else ''))
        return 0
    if argv[:1] == ['show'] and len(argv) == 2:
        print(latest(argv[1]))
        return 0
    print(__doc__)
    return 2


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
