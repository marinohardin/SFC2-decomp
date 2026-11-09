"""JSON plumbing for harness/loop.sh: the claim, the prompt, scores and the attempt row.

    python3 -m harness.lib.loopio claim <entry.json> <verify.json>     # writes claim.json
    python3 -m harness.lib.loopio prompt <entry.json>                  # prints the prompt
    python3 -m harness.lib.loopio score <verify.json> <address>        # "score exact exit"
    python3 -m harness.lib.loopio limited <claude.json>                # exit 0 on a usage limit
    python3 -m harness.lib.loopio autherror <claude.json>              # exit 0 on a login failure
    claude auth status | python3 -m harness.lib.loopio login -        # exit 0 on a Max login
    python3 -m harness.lib.loopio row <entry.json> <outcome> <before> <after> <claude.json> <secs> <time>
"""
import json
import os
import re
import sys

from . import funcs, paths

LIMIT = re.compile(r'usage limit|limit reached|rate.?limit|resets? at|overloaded|429|'
                   r'credit balance|quota', re.I)
AUTH = re.compile(r'authenticat|oauth|not logged in|log ?in again|/login|invalid api key', re.I)
MAX_BLOCKS = int(os.environ.get('HARNESS_MAX_STOP_BLOCKS', 3))


def load(path):
    """A JSON file, or standard input for '-'; None when missing or not JSON."""
    try:
        if path == '-':
            return json.load(sys.stdin)
        with open(path) as f:
            return json.load(f)
    except (OSError, ValueError):
        return None


def score(verify, address):
    """(score, exact, exit) of address in a build-and-verify --json result; unscored is 0."""
    if not verify:
        return 0.0, False, 2
    r = next((x for x in verify.get('results', []) if x.get('address') == address), None)
    if r is None or r.get('error') and 'relocs' not in r:
        return 0.0, False, verify.get('exit', 2)
    return float(r.get('score', 0.0)), bool(r.get('exact')), verify.get('exit', 2)


def claim(entry, verify):
    s, exact, _ = score(verify, entry['address'])
    c = dict(address=entry['address'], name=entry['name'], model=entry.get('model'),
             baseline_score=s, baseline_exact=exact, max_blocks=MAX_BLOCKS, blocks=0)
    os.makedirs(os.path.dirname(paths.CLAIM), exist_ok=True)
    with open(paths.CLAIM, 'w') as f:
        json.dump(c, f)
    return c


ROLES = {
    'build': 'Your job is to build this function: write it from the original instructions.',
    'fix': ('Your job is a fix round: an earlier attempt left this function at the baseline '
            'score below. Start from the side-by-side listing and close the remaining '
            'differences; rewrite only what the listing shows is wrong.'),
}


def prompt(entry, template_path=None, index=None):
    template_path = template_path or os.path.join(paths.HARNESS, 'prompts', 'function.md')
    with open(template_path) as f:
        text = f.read()
    fn = None
    if index is not None:
        fn = index.by_address.get(int(entry['address'], 16))
    c = load(paths.CLAIM) or {}
    values = {
        'ADDRESS': entry['address'],
        'NAME': entry['name'],
        'SIZE': str(entry.get('size', '')),
        'MANGLED': (fn.mangled if fn else '') or '(none known)',
        'PROTOTYPE': (fn.demangled if fn else '') or '(none known)',
        'TRANSLATION_UNIT': (fn.translation_unit if fn else '') or '(not known)',
        'ATTEMPT': str(entry.get('attempts', 0) + 1),
        'MODEL': entry.get('model', ''),
        'ROLE': ROLES[entry.get('role', 'build')],
        'BASELINE': f'{c.get("baseline_score", 0.0):.2f}',
        'SRC_ROOT': os.path.relpath(paths.SRC_ROOT, paths.REPO),
        'SDK_HEADERS': os.path.join(paths.REFERENCE, 'installed game',
                                    'Starfleet Command Orion Pirates', 'OP API - R2.1'),
    }
    for k, v in values.items():
        text = text.replace('{{' + k + '}}', v)
    brief = os.environ.get('HARNESS_BRIEF')
    if brief:
        try:
            with open(brief) as f:
                extra = f.read().strip()
        except OSError as e:
            raise SystemExit(f'ERROR cannot read the batch brief {brief}: {e}')
        if extra:
            text = text.rstrip('\n') + '\n\n---\n\n' + extra + '\n'
    return text


def limited(out):
    """True when a claude -p run stopped on a usage or rate limit rather than on the task."""
    if out is None:
        return False
    if not out.get('is_error') and out.get('subtype', 'success') == 'success':
        return False
    return bool(LIMIT.search(json.dumps(out.get('result', '')) + ' ' + str(out.get('subtype', ''))))


def auth_error(out):
    """True when a claude -p run never started because the login failed."""
    if out is None or not out.get('is_error'):
        return False
    return bool(AUTH.search(json.dumps(out.get('result', ''))))


def subscription_login(status):
    """(ok, why) from `claude auth status`: logged in, and not with an API key, which would
    bill the API instead of the Max plan."""
    if not status or not status.get('loggedIn'):
        return False, 'claude is not logged in: run `claude auth login` with the Max account'
    method = str(status.get('authMethod', '')).lower()
    if 'api' in method or 'key' in method:
        return False, f'claude is logged in with {method}, not the Max subscription'
    return True, f'claude logged in ({method})'


def row(entry, outcome, before, after, out, secs, now):
    u = (out or {}).get('usage') or {}
    return dict(time=now, address=entry['address'], name=entry['name'], model=entry.get('model'),
                outcome=outcome, score_before=f'{float(before):.2f}',
                score_after=f'{float(after):.2f}', input_tokens=u.get('input_tokens', 0),
                output_tokens=u.get('output_tokens', 0),
                cache_read_tokens=u.get('cache_read_input_tokens', 0),
                cache_write_tokens=u.get('cache_creation_input_tokens', 0),
                cost_usd=(out or {}).get('total_cost_usd', 0), turns=(out or {}).get('num_turns', 0),
                seconds=secs)


def claim_lines(src_root, claimed):
    """Every claim line in the tree except the claimed function's, sorted, one per line:
    `path<TAB>kind module 0xaddress`. The loop compares it before and after a session."""
    from . import guard
    if not os.path.isdir(src_root):
        raise SystemExit(f'claim snapshot: {src_root} is not a directory')
    out = []
    for d, dirs, files in os.walk(src_root):
        dirs[:] = sorted(x for x in dirs if not x.startswith('.'))
        for name in sorted(files):
            if not name.lower().endswith(guard.ANNOTATED_EXTS):
                continue
            p = os.path.join(d, name)
            with open(p, encoding='latin1') as f:
                for line in f:
                    m = guard.CLAIM_LINE.match(line)
                    if m and int(m.group(3), 16) != claimed:
                        out.append(f'{os.path.relpath(p, src_root)}\t{m.group(1)} {m.group(2)} '
                                   f'0x{int(m.group(3), 16):08x}')
    return sorted(out)


def main(argv):
    cmd = argv[0]
    if cmd == 'claim':
        c = claim(load(argv[1]), load(argv[2]))
        print(f'{c["baseline_score"]:.2f}')
    elif cmd == 'prompt':
        print(prompt(load(argv[1]), index=funcs.FunctionIndex(paths.FUNCTIONS_CSV)))
    elif cmd == 'score':
        s, exact, code = score(load(argv[1]), argv[2])
        print(f'{s:.2f} {"exact" if exact else "partial"} {code}')
    elif cmd == 'claims':
        print('\n'.join(claim_lines(argv[1], int(argv[2], 16))))
    elif cmd == 'limited':
        return 0 if limited(load(argv[1])) else 1
    elif cmd == 'autherror':
        return 0 if auth_error(load(argv[1])) else 1
    elif cmd == 'login':
        ok, why = subscription_login(load(argv[1]))
        print(why)
        return 0 if ok else 1
    elif cmd == 'row':
        entry, outcome, before, after, out, secs, now = argv[1:8]
        print(json.dumps(row(load(entry), outcome, before, after, load(out), secs, now)))
    else:
        print(__doc__)
        return 2
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
