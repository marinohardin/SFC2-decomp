"""Decision of the Stop hook of a function session (harness/hooks/stop_verify.py).

The session may finish only when build-and-verify.sh shows the claimed function better than when
the session started (higher score, or newly exact) with nothing regressed, and with no stand-in:
no call that reaches a claimed function through another symbol than the claim's
(harness/lib/symtab.py). After max_blocks
refusals it is let go, and harness/loop.sh records the attempt as failed.
"""
EPSILON = 1e-6


def standins(verify, address):
    """The stand-ins build-and-verify --json found in the function at `address`."""
    return [b for b in (verify or {}).get('standins') or [] if b.get('function') == address]


def partner_note(claim, verify):
    """One line per pair partner the verify result scored (harness/lib/pairs.py): the session
    is judged on the claimed function, and the partners' results are reported beside it."""
    out = []
    for a in claim.get('partners') or []:
        r = next((x for x in (verify or {}).get('results', []) if x.get('address') == a), None)
        if r is not None:
            shown = 'exact' if r.get('exact') else '%.2f%%' % r.get('score', 0.0)
            out.append(f'partner {a}: {shown}')
    return out


def decide(claim, verify, blocks):
    """The decision of `_decide` with the pair partners' results added to the message."""
    allow, message = _decide(claim, verify, blocks)
    notes = partner_note(claim, verify)
    return allow, message + ('\n' + '\n'.join(notes) if notes else '')


def _decide(claim, verify, blocks):
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
    mine = [b for a in [claim['address']] + list(claim.get('partners') or [])
            for b in standins(verify, a)]
    if mine:
        return False, ('The function calls a claimed function through a stand-in: a declaration '
                       'that compiles to another symbol than the claim. Declare each callee as '
                       'its claim does (include the header that defines its class) and remove '
                       'the stand-in, even if the score drops.\n' + '\n'.join(
                           f'- {b["target"]} is claimed as {b["claimed"]}; this function names '
                           f'{b["symbol"]}' for b in mine))
    base = claim.get('baseline_score', 0.0)
    if res.get('exact') and not claim.get('baseline_exact'):
        return True, f'{claim["address"]} is now exact'
    if res.get('score', 0.0) > base + EPSILON:
        return True, f'{claim["address"]} improved from {base:.2f}% to {res["score"]:.2f}%'
    return False, (f'{claim["address"]} scores {res.get("score", 0.0):.2f}%, not above the '
                   f'{base:.2f}% it had when this session started. Keep working on the '
                   f'function, or stop again to give up.\n' + lines)
