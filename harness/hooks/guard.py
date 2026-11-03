#!/usr/bin/env python3
"""PreToolUse hook for function sessions: deny edits and commands that could touch what judges
the work. Inert unless HARNESS_FUNCTION_SESSION=1 (set by harness/loop.sh), so it can sit in
the project settings without getting in the way of interactive sessions."""
import json
import os
import sys

# The repository root, not harness/: harness/queue.py would shadow the stdlib queue module.
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
from harness.lib import guard  # noqa: E402


def main():
    if os.environ.get('HARNESS_FUNCTION_SESSION') != '1':
        return 0
    try:
        reason = guard.decide(json.load(sys.stdin))
    except Exception as e:  # fail closed: an unreadable event is denied, not waved through
        reason = f'the harness guard could not check this call ({type(e).__name__}: {e})'
    if reason:
        json.dump({'hookSpecificOutput': {'hookEventName': 'PreToolUse',
                                          'permissionDecision': 'deny',
                                          'permissionDecisionReason': reason}}, sys.stdout)
    return 0


if __name__ == '__main__':
    sys.exit(main())
