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
# Functions the stop rule ended short of 100% before they had their full chance; the queue skips
# them unless told to take them (loop.sh --revisit). `listed` is when the run ended: a revisit
# run counts only attempts after it.
REVISIT_FIELDS = ['address', 'name', 'best_score', 'attempts', 'last_model', 'listed', 'note']
# Outcomes that count as attempts. A usage-limit stop, a harness error or a dry run does not.
COUNTED = {'failed', 'improved', 'exact', 'no-change', 'tampered', 'timeout'}
LARGE_BYTES = 4096  # about 1,000 instructions; such functions go to the end of the queue
SOLVED_SAMPLE = 200
ADDRESS = re.compile(r'^(0x)?[0-9A-Fa-f]{1,8}$')


# Model order by function size (harness/README.md, "Model tiers"): Sonnet builds a function of
# fewer than SMALL_BYTES bytes, Opus builds anything larger, and Opus takes every later attempt.
DEFAULT_TIERS = ('opus', 'opus', 'opus')
DEFAULT_SMALL_TIERS = ('sonnet', 'opus', 'opus')
SMALL_BYTES = 128
# A function's run goes on past two attempts only while its best score is above this, and Fable
# is offered only to a function whose best is at least this (harness/README.md, "Model tiers").
NEAR = float(os.environ.get('HARNESS_NEAR', 90.0))
EPSILON = 1e-6


class Tiers:
    """Model by attempt, chosen by the function's size. A function of fewer than `small_bytes`
    bytes runs the `small` sequence (Sonnet builds, then Opus, then Opus); any other the regular
    one (Opus three times). The stop rule (`plan`) usually ends a run sooner. Fable comes only
    after the run, only for a near match, and only when explicitly allowed
    (HARNESS_ALLOW_FABLE=1 or loop.sh --allow-fable).

    HARNESS_TIERS replaces the regular sequence, e.g. "opus,sonnet", and HARNESS_SMALL_TIERS the
    small one; HARNESS_TIERS alone sets the order for every size, as it did before sizes
    mattered. HARNESS_SMALL_BYTES (128) moves the line; 0 turns it off. `for_size` gives the
    sequence one function runs; a Tiers built without `small` has one sequence for all."""

    def __init__(self, sequence=DEFAULT_TIERS, fable=1, allow_fable=False, small=None,
                 small_bytes=0):
        self.regular = list(sequence)
        self.small = list(small) if small else list(self.regular)
        self.small_bytes = small_bytes if small else 0
        self.fable = fable
        self.allow_fable = allow_fable
        self.sequence = self.regular + (['fable'] * fable if allow_fable else [])

    @classmethod
    def from_env(cls, env=os.environ):
        def seq(var):
            out = [m.strip() for m in env.get(var, '').split(',') if m.strip()]
            if 'fable' in out:
                raise ValueError(f'{var} may not name fable; use HARNESS_ALLOW_FABLE=1')
            return out
        regular, small = seq('HARNESS_TIERS'), seq('HARNESS_SMALL_TIERS')
        if regular and not small:
            small = regular
        return cls(regular or DEFAULT_TIERS, int(env.get('HARNESS_FABLE_TRIES', 1)),
                   env.get('HARNESS_ALLOW_FABLE') == '1', small or DEFAULT_SMALL_TIERS,
                   int(env.get('HARNESS_SMALL_BYTES', SMALL_BYTES)))

    def for_size(self, size):
        """The tiers one function runs: the small sequence below `small_bytes`, the regular one
        otherwise, and the regular one when its size is unknown."""
        try:
            small = 0 < int(size) < self.small_bytes
        except (TypeError, ValueError):
            small = False
        return Tiers(self.small if small else self.regular, self.fable, self.allow_fable)

    @property
    def cap(self):
        return len(self.sequence)

    def model(self, attempts):
        """The model by position in the sequence, ignoring the stop rule (see `plan`)."""
        return self.sequence[attempts] if attempts < len(self.sequence) else None

    @staticmethod
    def role(attempts, model):
        """What the session is for: building the function, or fixing the near match left by
        an earlier attempt. The second attempt is always a fix round, whichever model takes it;
        a later Sonnet or Fable attempt is one too."""
        if attempts == 0:
            return 'build'
        return 'fix' if attempts == 1 or model in ('sonnet', 'fable') else 'build'


def _score(v):
    try:
        return float(v or 0)
    except ValueError:
        return 0.0


def best_score(rows):
    """The best score a function has had: any attempt's result or starting point."""
    return max([_score(r.get('score_before')) for r in rows] +
               [_score(r.get('score_after')) for r in rows] or [0.0])


class Plan:
    """What happens next to a function, from its counted attempts (`plan`).

    model: the next attempt's model, or None when the run is over.
    full:  over, and the function had every attempt the rule gives it (the regular sequence to
           its end, and Fable when its best qualified). Such a function goes to
           difficult_functions.tsv; one that stopped short goes to revisit_functions.tsv.
    note:  why the run ended, for the list.
    waits: over, but its best qualifies for a Fable try it has not had (Fable not allowed)."""

    def __init__(self, model=None, role=None, full=False, note='', best=0.0, attempts=0,
                 waits=False):
        self.model, self.role, self.full, self.note = model, role, full, note
        self.best, self.attempts, self.waits = best, attempts, waits


def plan(rows, tiers, near=NEAR):
    """The stop rule. `rows` are the function's counted attempts, oldest first.

    1. Attempt 1 is the first model of the sequence (Opus).
    2. From attempt 2 on, an attempt that does not raise the best score ends the run.
    3. After two attempts the run also ends unless the best is above `near` (90%).
    4. Otherwise the next model of the sequence, until it runs out.
    5. When the run is over and the best is at least `near`, Fable gets HARNESS_FABLE_TRIES
       tries (default 1) if allowed. Without --allow-fable the function waits on the revisit
       list.
    An exact attempt ends everything (the caller checks that first)."""
    best = best_score(rows)
    n = len(rows)
    regular = [r for r in rows if r.get('model') != 'fable']
    fables = n - len(regular)
    k = len(regular)
    stop = None
    if k >= 2:
        last = regular[-1]
        before = best_score(regular[:-1] + [dict(score_before=last.get('score_before'))])
        if _score(last.get('score_after')) <= before + EPSILON:
            stop = f'attempt {k} ({last.get("model")}) did not raise the best'
        elif best <= near + EPSILON:
            stop = f'best not above {near:g}% after {k} attempts'
    if stop is None and not fables and k < len(tiers.regular):
        model = tiers.regular[k]
        return Plan(model, Tiers.role(k, model), best=best, attempts=n)
    ran_out = k >= len(tiers.regular)
    reason = stop or f'all {k} regular attempts used'
    fable_due = tiers.fable > 0 and fables < tiers.fable and best + EPSILON >= near
    if fable_due and tiers.allow_fable:
        return Plan('fable', 'fix', best=best, attempts=n)
    if fable_due:
        note = f'{reason}; waits for Fable'
    elif fables:
        note = f'{reason}; Fable tried'
    else:
        note = reason
    return Plan(None, full=ran_out and not fable_due, note=f'{note}; best {best:.2f}%',
                best=best, attempts=n, waits=fable_due)


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


def build(index, exe, matched_set, difficult, tiers, boundaries=()):
    """The ordered queue: [{address, name, size, named, similarity}]. `boundaries` holds the
    matched-set keys of addresses on the boundary-error list, which are pieces of functions and
    never taken."""
    done = {int(k, 16) for k, v in matched_set['functions'].items() if v.get('exact')}
    given_up = {int(r['address'], 16) for r in difficult} | {int(k, 16) for k in boundaries}
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


def counted_attempts(attempts, address, since=''):
    return len(history(attempts, address, since))


def history(attempts, address, since=''):
    """The function's counted attempts after `since` (an ISO time; '' for all), oldest first."""
    return [r for r in attempts if r['address'] == address and r['outcome'] in COUNTED
            and (not since or r.get('time', '') > since)]


def run_rows(attempts, address, listed, tiers):
    """The attempts the stop rule judges for a function: all of them, or for a revisit-listed
    function (`listed` is when it was listed) those since the listing, so its run starts over.
    A listed function that was only waiting for Fable keeps its whole history instead: a
    revisit pass with --allow-fable gives it the Fable try, and one without leaves it waiting
    rather than spending a new run on it."""
    rows = history(attempts, address)
    if not listed:
        return rows
    upto = [r for r in rows if r.get('time', '') <= listed]
    if plan(upto, Tiers(tiers.regular, tiers.fable, allow_fable=True)).model == 'fable':
        return rows
    return [r for r in rows if r.get('time', '') > listed]


def runs(rows, tiers):
    """The attempt history cut into runs: a run ends when the stop rule gives it no further
    model, with Fable counted as allowed (so a Fable try continues the run it was waiting at
    the end of), and the next attempt begins a new run (a revisit pass)."""
    allow = Tiers(tiers.regular, tiers.fable, allow_fable=True)
    out, seg = [], []
    for r in rows:
        if seg and plan(seg, allow).model is None:
            out.append(seg)
            seg = []
        seg.append(r)
    if seg:
        out.append(seg)
    return out


def unlisted_run(attempts, address, tiers):
    """The rows of the run a revisit pass would judge for a function not on this checkout's
    revisit list: none (a new run) when its last run has ended short of its full chance, its
    last run otherwise. The revisit list is kept per checkout while the attempt log is shared,
    so a function another loop listed on its own branch is found here from the log alone."""
    segs = runs(history(attempts, address), tiers)
    if not segs:
        return []
    last = segs[-1]
    p = plan(last, Tiers(tiers.regular, tiers.fable, allow_fable=True))
    return [] if p.model is None and not p.full else last


def next_entry(queue, attempts, difficult, tiers, claim=lambda address: True, revisit=(),
               take_revisit=False):
    """The first queue entry still open that `claim` grants, with its attempt count, model and
    role; None if there is none. Functions on the revisit list are skipped unless
    `take_revisit`, and then their run starts over from the time they were listed (`run_rows`).
    With `take_revisit`, a function that is not listed here but whose run has ended short of
    its full chance in the shared log is taken too, with a new run (`unlisted_run`)."""
    done = {r['address'] for r in attempts if r['outcome'] == 'exact'}
    given_up = {r['address'] for r in difficult}
    listed = {r['address']: r.get('listed', '') for r in revisit}
    whole = tiers
    for e in queue:
        a = e['address']
        if a in given_up or a in done or (a in listed and not take_revisit):
            continue
        tiers = whole.for_size(e.get('size'))
        if a not in listed and take_revisit:
            rows = unlisted_run(attempts, a, tiers)
        else:
            rows = run_rows(attempts, a, listed.get(a, ''), tiers)
        p = plan(rows, tiers)
        if p.model and claim(a):
            return dict(e, attempts=p.attempts, model=p.model, role=p.role, best=p.best)
    return None


def _rewrite_tsv(path, fields, keep):
    """Rewrite a TSV under an exclusive lock, keeping the rows `keep` accepts."""
    if not os.path.exists(path):
        return
    with open(path, 'r+', newline='') as f:
        fcntl.flock(f, fcntl.LOCK_EX)
        try:
            rows = [r for r in csv.DictReader(f, delimiter='\t') if r.get(fields[0])]
            kept = [r for r in rows if keep(r)]
            if len(kept) == len(rows):
                return
            f.seek(0)
            f.truncate()
            w = csv.DictWriter(f, fieldnames=fields, delimiter='\t', extrasaction='ignore',
                               lineterminator='\n')
            w.writeheader()
            w.writerows(kept)
        finally:
            fcntl.flock(f, fcntl.LOCK_UN)


def settle(paths_, entry, tiers, now, take_revisit=False):
    """Put a function whose run is over on the right list. Returns 'difficult', 'revisit',
    'cleared' (exact: taken off the revisit list) or None (its run goes on, or it is listed
    already)."""
    a = entry['address']
    tiers = tiers.for_size(entry.get('size'))
    attempts = read_tsv(paths_.ATTEMPTS, ATTEMPT_FIELDS)
    revisit = read_tsv(paths_.REVISIT, REVISIT_FIELDS)
    mine = [r for r in revisit if r['address'] == a]
    if any(r['address'] == a and r['outcome'] == 'exact' for r in attempts):
        if mine:
            _rewrite_tsv(paths_.REVISIT, REVISIT_FIELDS, lambda r: r['address'] != a)
            return 'cleared'
        return None
    if mine and not take_revisit:
        return None
    if not mine and take_revisit:
        segs = runs(history(attempts, a), tiers)
        rows = segs[-1] if segs else []
    else:
        rows = run_rows(attempts, a, mine[0].get('listed', '') if mine else '', tiers)
    if not rows:
        return None
    p = plan(rows, tiers)
    if p.model is not None:
        return None
    if any(r['address'] == a for r in read_tsv(paths_.DIFFICULT, DIFFICULT_FIELDS)):
        return None
    last = rows[-1]
    # A listed function gets one revisit run: if that ends short too, and it is not left
    # waiting for Fable, it goes to the difficult list. Listing it again would hand it straight
    # back to the same --revisit loop, which would run it over and over.
    if p.full or (mine and take_revisit and not p.waits):
        _rewrite_tsv(paths_.REVISIT, REVISIT_FIELDS, lambda r: r['address'] != a)
        append_tsv(paths_.DIFFICULT, DIFFICULT_FIELDS, dict(
            address=a, name=entry['name'], attempts=p.attempts, last_model=last['model'],
            best_score=f'{p.best:.2f}', last_attempt=last['time'],
            reason=f'not exact after {p.attempts} attempts: {p.note}'))
        return 'difficult'
    _rewrite_tsv(paths_.REVISIT, REVISIT_FIELDS, lambda r: r['address'] != a)
    append_tsv(paths_.REVISIT, REVISIT_FIELDS, dict(
        address=a, name=entry['name'], best_score=f'{p.best:.2f}', attempts=p.attempts,
        last_model=last['model'], listed=now, note=p.note))
    return 'revisit'


def record(paths_, entry, row, tiers, take_revisit=False):
    """Log one attempt, then settle the function if its run is over (see `settle`)."""
    append_tsv(paths_.ATTEMPTS, ATTEMPT_FIELDS, row)
    if row['outcome'] not in COUNTED:
        return None
    return settle(paths_, entry, tiers, row['time'], take_revisit)


def sweep(paths_, tiers, now, exact=frozenset(), sizes=None):
    """Settle every function with counted attempts whose run the stop rule has ended but that
    is on no list yet: those whose attempts were logged before the rule existed. `exact` holds
    the addresses matched_set.json records exact: a function matched outside the loop (by hand,
    or in a later batch) has no exact attempt in the log, so it is never listed, and is taken
    off either list if it is on one. `sizes` ({address: bytes}) picks each function's model
    order; a function without one runs the regular order. Returns
    {address: 'difficult' | 'revisit' | 'cleared'}."""
    sizes = sizes or {}
    attempts = read_tsv(paths_.ATTEMPTS, ATTEMPT_FIELDS)
    exact = {a.lower() for a in exact}
    names, out = {}, {}
    for path, fields in ((paths_.DIFFICULT, DIFFICULT_FIELDS), (paths_.REVISIT, REVISIT_FIELDS)):
        gone = [r['address'] for r in read_tsv(path, fields) if r['address'].lower() in exact]
        if gone:
            _rewrite_tsv(path, fields, lambda r: r['address'].lower() not in exact)
            out.update((a, 'cleared') for a in gone)
    for r in attempts:
        if r['outcome'] in COUNTED and r['address'].lower() not in exact:
            names.setdefault(r['address'], r['name'])
    for a, name in names.items():
        got = settle(paths_, dict(address=a, name=name, size=sizes.get(a)), tiers, now)
        if got in ('difficult', 'revisit'):
            out[a] = got
    return out


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
