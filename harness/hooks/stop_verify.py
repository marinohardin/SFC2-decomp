#!/usr/bin/env python3
"""Stop hook for function sessions: re-run build-and-verify.sh for the claimed function and
block completion unless it improved and the whole-tree checks
(harness/lib/wholetree.py) pass. Inert unless HARNESS_FUNCTION_SESSION=1. The claim (which
function, its score at the start) is written by harness/loop.sh to $HARNESS_STATE/claim.json."""
import json
import os
import subprocess
import sys

# The repository root, not harness/: harness/queue.py would shadow the stdlib queue module.
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
from harness.lib import loopio, paths, stophook, wholetree  # noqa: E402


def main():
    if os.environ.get('HARNESS_FUNCTION_SESSION') != '1':
        return 0
    try:
        json.load(sys.stdin)
    except ValueError:
        pass
    try:
        with open(paths.CLAIM) as f:
            claim = json.load(f)
    except (OSError, ValueError):
        return 0  # no claim: nothing to judge (the loop treats this as a failed attempt)
    # The claimed function, and the partners of its pair that the source claims (a deleting
    # destructor and its real destructor are one piece of work, harness/lib/pairs.py).
    ids = loopio.verify_ids(claim)
    p = subprocess.run([os.path.join(paths.HARNESS, 'build-and-verify.sh'), *ids, '--json'],
                       capture_output=True, text=True)
    try:
        verify = json.loads(p.stdout)
    except ValueError:
        verify = None
    allow, message = stophook.decide(claim, verify, claim.get('blocks', 0))
    if allow and claim.get('blocks', 0) < claim.get('max_blocks', 3):
        # The function is good: now the rest of the tree (harness/lib/wholetree.py).
        ok, text = wholetree.check()
        if not ok:
            allow, message = False, text
    if allow:
        print(json.dumps({'systemMessage': message}))
        return 0
    claim['blocks'] = claim.get('blocks', 0) + 1
    with open(paths.CLAIM, 'w') as f:
        json.dump(claim, f)
    print(json.dumps({'decision': 'block', 'reason': message}))
    return 0


if __name__ == '__main__':
    sys.exit(main())
