"""Decision of the Stop hook of a function session (harness/hooks/stop_verify.py).

The session may finish only when build-and-verify.sh shows the claimed function better than when
the session started (higher score, or newly exact) with nothing regressed. After max_blocks
refusals it is let go, and harness/loop.sh records the attempt as failed.
"""
EPSILON = 1e-6


def decide(claim, verify, blocks):
    """claim: {'address', 'baseline_score', 'baseline_exact', 'max_blocks'};
    verify: build-and-verify.sh --json output (dict), or None if it could not run;
    blocks: refusals so far. Returns (allow, message)."""
    if blocks >= claim.get('max_blocks', 3):
        return True, 'stop allowed after repeated refusals; the attempt is recorded as failed'
    if verify is None:
        return False, ('build-and-verify.sh did not run to completion. BUILD HAS FAILED. Fix the '
                       'source and run harness/build-and-verify.sh ' + claim['address'])
    lines = '\n'.join(verify.get('lines', []))
    res = next((r for r in verify.get('results', []) if r.get('address') == claim['address']),
               None)
    if verify.get('exit') == 1:
        return False, 'A previously matched function regressed. Undo that first.\n' + lines
    if res is None or verify.get('exit') != 0:
        return False, 'The claimed function was not built and scored.\n' + lines
    base = claim.get('baseline_score', 0.0)
    if res.get('exact') and not claim.get('baseline_exact'):
        return True, f'{claim["address"]} is now exact'
    if res.get('score', 0.0) > base + EPSILON:
        return True, f'{claim["address"]} improved from {base:.2f}% to {res["score"]:.2f}%'
    return False, (f'{claim["address"]} scores {res.get("score", 0.0):.2f}%, not above the '
                   f'{base:.2f}% it had when this session started. Keep working on the '
                   f'function, or stop again to give up.\n' + lines)
