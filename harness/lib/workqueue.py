"""The function queue, attempt log, model tiers and difficult_functions.tsv.

Order (harness/README.md, "The queue"):
  1. functions named from evidence with a typed prototype before the rest;
  2. smaller before larger, in power-of-two size buckets, with very large functions last;
  3. inside a bucket, the most similar to an already exact function first;
  4. then size, then address, so the order is stable.

Similarity is the Jaccard index of instruction-mnemonic 3-grams when capstone is importable, and
of raw byte 4-grams otherwise (a cruder proxy; the order within a bucket is all it affects).
"""
import csv
import fcntl
import re
import json
import os
import socket

ATTEMPT_FIELDS = ['time', 'address', 'name', 'model', 'outcome', 'score_before', 'score_after',
                  'input_tokens', 'output_tokens', 'cache_read_tokens', 'cache_write_tokens',
                  'cost_usd', 'turns', 'seconds']
DIFFICULT_FIELDS = ['address', 'name', 'attempts', 'last_model', 'best_score', 'last_attempt',
                    'reason']
# Outcomes that count against the attempt cap. A usage-limit stop or a harness error does not.
COUNTED = {'failed', 'improved', 'exact', 'no-change', 'tampered'}
LARGE_BYTES = 4096  # about 1,000 instructions; such functions go to the end of the queue
SOLVED_SAMPLE = 200
ADDRESS = re.compile(r'^(0x)?[0-9A-Fa-f]{1,8}$')


DEFAULT_TIERS = ('opus', 'sonnet', 'sonnet', 'opus')


class Tiers:
    """Model by number of counted attempts. Opus builds (attempt 1), Sonnet does the fix rounds
    on what Opus left (attempts 2 and 3), Opus tries once more (attempt 4). Fable comes only
    after that, and only when explicitly allowed (HARNESS_ALLOW_FABLE=1 or loop.sh
    --allow-fable). HARNESS_TIERS replaces the default sequence, e.g. "opus,sonnet,opus"."""

    def __init__(self, sequence=DEFAULT_TIERS, fable=1, allow_fable=False):
        self.sequence = list(sequence) + (['fable'] * fable if allow_fable else [])

    @classmethod
    def from_env(cls, env=os.environ):
        seq = [m.strip() for m in env.get('HARNESS_TIERS', '').split(',') if m.strip()]
        if 'fable' in seq:
            raise ValueError('HARNESS_TIERS may not name fable; use HARNESS_ALLOW_FABLE=1')
        return cls(seq or DEFAULT_TIERS, int(env.get('HARNESS_FABLE_TRIES', 1)),
                   env.get('HARNESS_ALLOW_FABLE') == '1')

    @property
    def cap(self):
        return len(self.sequence)

    def model(self, attempts):
        """The model for the next attempt, or None when the function is exhausted."""
        return self.sequence[attempts] if attempts < len(self.sequence) else None

    @staticmethod
    def role(attempts, model):
        """What the session is for: building the function, or fixing the near match left by
        an earlier attempt."""
        return 'build' if attempts == 0 or model != 'sonnet' else 'fix'


def size_bucket(size):
    if size >= LARGE_BYTES:
        return 99
    return max(size, 1).bit_length()


def _tokens(code):
    try:
        import capstone
    except ImportError:
        return {code[i:i + 4] for i in range(max(len(code) - 3, 0))}
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    mn = [i.mnemonic for i in md.disasm(code, 0)]
    return {tuple(mn[i:i + 3]) for i in range(max(len(mn) - 2, 0))}


def jaccard(a, b):
    if not a or not b:
        return 0.0
    return len(a & b) / len(a | b)


def build(index, exe, matched_set, difficult, tiers):
    """The ordered queue: [{address, name, size, named, similarity}]."""
    done = {int(k, 16) for k, v in matched_set['functions'].items() if v.get('exact')}
    given_up = {int(r['address'], 16) for r in difficult}
    solved = [index.by_address[a] for a in sorted(done) if a in index.by_address][-SOLVED_SAMPLE:]
    solved_tokens = [_tokens(exe.read(fn.address, fn.size)) for fn in solved if fn.size]
    out = []
    for fn in index.functions:
        if fn.kind != 'function' or fn.is_library or fn.size <= 0:
            continue
        if fn.address in done or fn.address in given_up:
            continue
        sim = 0.0
        if solved_tokens:
            t = _tokens(exe.read(fn.address, min(fn.size, LARGE_BYTES)))
            sim = max(jaccard(t, s) for s in solved_tokens)
        named = fn.has_name and fn.has_prototype
        out.append(dict(address=fn.id, name=fn.name, size=fn.size, named=named,
                        similarity=round(sim, 4)))
    out.sort(key=lambda e: (0 if e['named'] else 1, size_bucket(e['size']), -e['similarity'],
                            e['size'], e['address']))
    return out


def read_tsv(path, fields):
    if not os.path.exists(path):
        return []
    with open(path, newline='') as f:
        return [r for r in csv.DictReader(f, delimiter='\t') if r.get(fields[0])]


def append_tsv(path, fields, row):
    """Append one row under an exclusive lock (several loops share the attempt log)."""
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'a', newline='') as f:
        fcntl.flock(f, fcntl.LOCK_EX)
        try:
            w = csv.DictWriter(f, fieldnames=fields, delimiter='\t', extrasaction='ignore',
                               lineterminator='\n')
            if f.tell() == 0:
                w.writeheader()
            w.writerow(row)
        finally:
            fcntl.flock(f, fcntl.LOCK_UN)


def _alive(pid):
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    except PermissionError:
        return True
    return True


def acquire(claims_dir, address, owner, pid):
    """Claim a function for one loop: create claims/<address>.lock exclusively. A lock held by a
    dead process on this host is taken over. Returns True when this owner holds it."""
    os.makedirs(claims_dir, exist_ok=True)
    path = os.path.join(claims_dir, f'{address}.lock')
    me = {'owner': owner, 'pid': pid, 'host': socket.gethostname()}
    for _ in range(2):
        try:
            fd = os.open(path, os.O_CREAT | os.O_EXCL | os.O_WRONLY, 0o644)
        except FileExistsError:
            try:
                with open(path) as f:
                    held = json.load(f)
            except (OSError, ValueError):
                return False
            if held.get('owner') == owner:
                return True
            if held.get('host') == me['host'] and not _alive(int(held.get('pid', 0))):
                os.remove(path)
                continue
            return False
        with os.fdopen(fd, 'w') as f:
            json.dump(me, f)
        return True
    return False


def release(claims_dir, address, owner):
    path = os.path.join(claims_dir, f'{address}.lock')
    try:
        with open(path) as f:
            if json.load(f).get('owner') == owner:
                os.remove(path)
    except (OSError, ValueError):
        pass


def counted_attempts(attempts, address):
    return sum(1 for r in attempts if r['address'] == address and r['outcome'] in COUNTED)


def next_entry(queue, attempts, difficult, tiers, claim=lambda address: True):
    """The first queue entry still open that `claim` grants, with its attempt count, model and
    role; None if there is none."""
    done = {r['address'] for r in attempts if r['outcome'] == 'exact'}
    given_up = {r['address'] for r in difficult}
    for e in queue:
        if e['address'] in given_up or e['address'] in done:
            continue
        n = counted_attempts(attempts, e['address'])
        model = tiers.model(n)
        if model and claim(e['address']):
            return dict(e, attempts=n, model=model, role=Tiers.role(n, model))
    return None


def record(paths_, entry, row, tiers):
    """Log one attempt; move the function to difficult_functions.tsv when it reaches the cap
    without becoming exact. Returns True if it was moved."""
    append_tsv(paths_.ATTEMPTS, ATTEMPT_FIELDS, row)
    if row['outcome'] == 'exact':
        return False
    attempts = read_tsv(paths_.ATTEMPTS, ATTEMPT_FIELDS)
    n = counted_attempts(attempts, entry['address'])
    if tiers.model(n) is not None:
        return False
    if any(r['address'] == entry['address'] for r in read_tsv(paths_.DIFFICULT, DIFFICULT_FIELDS)):
        return False
    best = max([float(r['score_after'] or 0) for r in attempts if r['address'] == entry['address']]
               or [0.0])
    append_tsv(paths_.DIFFICULT, DIFFICULT_FIELDS, dict(
        address=entry['address'], name=entry['name'], attempts=n, last_model=row['model'],
        best_score=f'{best:.2f}', last_attempt=row['time'],
        reason=f'not exact after {n} attempts'))
    return True


def read_unit_map(path):
    """The unit map as [(first, last, unit)], address ranges as ints."""
    with open(path, newline='') as f:
        return [(int(r['first'], 16), int(r['last'], 16), r['unit'])
                for r in csv.DictReader(f, delimiter='\t') if r.get('unit')]


def select(queue, unit_map, units=(), addresses=()):
    """The addresses a loop restricted with --unit/--address may take, in address order (a
    batch works through a unit from its first function to its last). Raises ValueError for a
    unit that is not in the map or a value that is not a hex address. An address that is not
    in the queue (already exact or difficult, or not a function) simply drops out."""
    known = {u for _, _, u in unit_map}
    missing = [u for u in units if u not in known]
    if missing:
        raise ValueError(f'not in the unit map: {", ".join(missing)}')
    ranges = [(lo, hi) for lo, hi, u in unit_map if u in set(units)]
    want = set()
    for a in addresses:
        if not ADDRESS.match(a):
            raise ValueError(f'not an address: {a}')
        want.add(int(a, 16))
    out = []
    for e in queue:
        a = int(e['address'], 16)
        if a in want or any(lo <= a <= hi for lo, hi in ranges):
            out.append(e['address'])
    return sorted(out, key=lambda x: int(x, 16))


def restrict(queue, selection):
    """The queue limited to a selection, in the selection's order."""
    pos = {a: i for i, a in enumerate(selection)}
    return sorted((e for e in queue if e['address'] in pos), key=lambda e: pos[e['address']])


def save_queue(path, queue, built):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w') as f:
        json.dump({'built': built, 'entries': queue}, f, indent=0)


def load_queue(path):
    with open(path) as f:
        return json.load(f)['entries']
