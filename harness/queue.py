#!/usr/bin/env python3
"""The work queue for harness/loop.sh.

    harness/queue.py build                 # (re)build $HARNESS_STATE/queue.json
    harness/queue.py select [--unit U]... [--address A]...
                                           # restrict this checkout's loop (writes selection.json)
    harness/queue.py next                  # claim the next function: JSON with its model, or nothing
    harness/queue.py release <address>     # drop this checkout's claim on a function
    harness/queue.py record <json-row>     # log an attempt (loop.sh only)
    harness/queue.py show [N]              # the first N entries and the attempt totals
    harness/queue.py sweep                 # list functions the stop rule has already ended
    harness/queue.py seed-lists [REF]      # once: copy the lists as committed at REF (default
                                           # origin/develop) into the lists folder, lane "seed"

Skips library-flagged functions, EH funclets and thunks, functions already exact in
matched_set.json, those in difficult_functions.tsv, and the boundary errors in
boundary_errors.tsv. The two lists live outside the repository, one folder per lane under
HARNESS_LISTS; the queue reads the union of every lane's, a loop writes only its own
(harness/lib/paths.py). Functions in revisit_functions.tsv are
skipped too unless HARNESS_TAKE_REVISIT=1 (loop.sh --revisit); a revisit run counts only the
attempts made after the function was listed. See lib/workqueue.py for the order and the stop
rule (`plan`).

`record` settles a function whose run is over: difficult_functions.tsv when it had its full
chance, revisit_functions.tsv when the stop rule ended it sooner. `sweep` does the same for
every function in the attempt log that is on neither list, which is how attempts logged before
the stop rule existed are settled; it prints what it listed. It never lists a function that
matched_set.json records exact, and takes any such function off both lists.

`select` resolves units (paths in op/UNITS.tsv, such as Tiger/tsay.cpp) and addresses to the
queue entries they cover, in address order, and writes them to $HARNESS_STATE/selection.json.
`next` and `show` honour that file only when HARNESS_SELECTION names it (loop.sh sets it when
given --unit or --address), so a selection left over from an earlier run restricts nothing.
A selection with no open function is an error, not an empty batch.
"""
import json
import os
import sys
import time

# Replace this script's own directory with the repository root: as harness/ on sys.path, this
# file would shadow the stdlib queue module.
sys.path[0] = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
from harness.lib import clean, coff, funcs, matched, nearmiss, pairs, paths, source, workqueue  # noqa: E402


def _last_commit_with(ref, rel):
    """The commit to read `rel` from: `ref` when the file exists there, else the parent of the
    commit that deleted it (the last one reachable from `ref` that still had it), else None."""
    import subprocess

    def git(*a):
        return subprocess.run(['git', '-C', paths.REPO, *a], capture_output=True, text=True)
    if git('cat-file', '-e', f'{ref}:{rel}').returncode == 0:
        return ref
    last = git('log', '-1', '--format=%H', ref, '--', rel)
    sha = last.stdout.strip()
    if last.returncode != 0 or not sha:
        return None
    parent = git('rev-parse', '--verify', '--quiet', f'{sha}^')
    return parent.stdout.strip() if parent.returncode == 0 else None


def pair_resolver():
    """A `pairs.Resolver` over the functions and the symbol table, or None when HARNESS_PAIRS=0 or
    an input is missing (the loop then runs as it did, one address per session)."""
    if os.environ.get('HARNESS_PAIRS') == '0':
        return None
    from harness.lib import symtab
    try:
        index = funcs.FunctionIndex(paths.FUNCTIONS_CSV, paths.SYMBOL_TSVS)
        claims, table = symtab.load_claims(), symtab.load_table()
    except (OSError, symtab.TableError) as e:
        print(f'WARN  deleting destructors are not paired: {e}', file=sys.stderr)
        return None
    symbols = {}
    for fn in index.functions:
        got = symtab.lookup(fn.address, index, claims, table)
        if got:
            symbols[fn.address] = got[0]
    image = []

    def read(address, size):
        """The original's bytes, read from the install only when a deleting destructor's call
        has to be looked at; None when they cannot be read (the class symbols then decide)."""
        try:
            if not image:
                image.append(coff.PeImage(open(paths.EXE, 'rb').read()))
            return image[0].read(address, size)
        except (OSError, coff.CoffError):
            return None
    return pairs.Resolver(index, symbols, read)


def pair_states(entry, ms):
    """For the pair of a queue entry: ({partner address: 'exact' | 'claimed' | 'open'},
    {partner address: name})."""
    exact = {k for k, v in ms['functions'].items() if v.get('exact')}
    claimed = source.scan_annotations(paths.SRC_ROOT, source.load_units()['module'])
    index = funcs.FunctionIndex(paths.FUNCTIONS_CSV)
    states, names = {}, {}
    for a in entry['pair']['partners']:
        n = int(a, 16)
        states[a] = ('exact' if matched.key(n) in exact else
                     'claimed' if any(x.kind == 'FUNCTION' for x in claimed.get(n, [])) else
                     'open')
        fn = index.by_address.get(n)
        names[a] = fn.name if fn else ''
    return states, names


def seed_lists(ref):
    """Copy the two lists as committed at `ref` into the lists folder, as the lane "seed". Run
    once, when the lists first leave the repository; it never overwrites a file. Once they are
    deleted on `ref`, each is read from the last commit reachable from `ref` that still had it
    (the line says which)."""
    import subprocess
    out = os.path.join(paths.LISTS, 'seed')
    for name in (paths.DIFFICULT_NAME, paths.REVISIT_NAME):
        dest = os.path.join(out, name)
        rel = f'analysis/results/{name}'
        at = _last_commit_with(ref, rel)
        p = subprocess.run(['git', '-C', paths.REPO, 'show', f'{at}:{rel}'],
                           capture_output=True, text=True) if at else None
        if p is None or p.returncode != 0:
            print(f'ERROR no commit reachable from {ref} has {rel}', file=sys.stderr)
            return 2
        if os.path.exists(dest):
            print(f'INFO  {dest} exists; left alone')
            continue
        os.makedirs(out, exist_ok=True)
        with open(dest, 'w', newline='') as f:
            f.write(p.stdout)
        print(f'OK    {name} from {at[:12]} ({ref}): '
              f'{max(len(p.stdout.splitlines()) - 1, 0)} rows -> {dest}')
    return 0


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    cmd, args = argv[0], argv[1:]
    if cmd == 'seed-lists':
        return seed_lists(args[0] if args else 'origin/develop')
    if cmd in ('build', 'select', 'next'):
        clean.record_worktree()
    tiers = workqueue.Tiers.from_env()
    difficult = workqueue.read_lists(paths, 'difficult')
    revisit = workqueue.read_lists(paths, 'revisit')
    take_revisit = os.environ.get('HARNESS_TAKE_REVISIT') == '1'
    if cmd == 'sweep':
        exact = {k for k, v in matched.load(paths.MATCHED_SET)['functions'].items()
                 if v.get('exact')}
        sizes = ({e['address']: e['size'] for e in workqueue.load_queue(paths.QUEUE)}
                 if os.path.exists(paths.QUEUE) else {})
        got = workqueue.sweep(paths, tiers, time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime()),
                              exact, sizes)
        for a, where in sorted(got.items()):
            print(f'INFO  {a} -> {where}')
        listed = sum(1 for w in got.values() if w != 'cleared')
        print(f'OK    swept: {listed} functions listed, {len(got) - listed} exact ones taken off')
        return 0
    if cmd == 'build':
        index = funcs.FunctionIndex(paths.FUNCTIONS_CSV)
        exe = coff.PeImage(open(paths.EXE, 'rb').read())
        q = workqueue.build(index, exe, matched.load(paths.MATCHED_SET), difficult, tiers,
                            matched.boundary_listed(paths.BOUNDARIES))
        workqueue.save_queue(paths.QUEUE, q, time.strftime('%Y-%m-%dT%H:%M:%S'))
        named = sum(1 for e in q if e['named'])
        print(f'OK    queue: {len(q)} functions ({named} named with a prototype) -> {paths.QUEUE}')
        return 0
    if not os.path.exists(paths.QUEUE):
        print(f'ERROR no queue at {paths.QUEUE}; run harness/queue.py build', file=sys.stderr)
        return 2
    q = workqueue.load_queue(paths.QUEUE)
    attempts = workqueue.read_tsv(paths.ATTEMPTS, workqueue.ATTEMPT_FIELDS)
    owner = paths.REPO  # one loop per checkout
    if cmd == 'select':
        units, addresses = [], []
        it = iter(args)
        for a in it:
            if a not in ('--unit', '--address'):
                print(__doc__)
                return 2
            v = next(it, None)
            if not v:
                print(f'ERROR {a} needs a value', file=sys.stderr)
                return 2
            (units if a == '--unit' else addresses).append(v)
        try:
            unit_map = workqueue.read_unit_map(paths.UNITS_TSV) if units else []
            sel = workqueue.select(q, unit_map, units, addresses)
        except (OSError, ValueError) as e:
            print(f'ERROR {e}', file=sys.stderr)
            return 2
        if not sel:
            print('ERROR the selection has no open function: every one is exact or difficult, '
                  'or the addresses are not functions', file=sys.stderr)
            return 2
        os.makedirs(os.path.dirname(paths.SELECTION), exist_ok=True)
        with open(paths.SELECTION, 'w') as f:
            json.dump({'units': units, 'addresses': addresses, 'entries': sel}, f, indent=0)
        print(f'OK    selection: {len(sel)} open functions in {len(units)} units and '
              f'{len(addresses)} addresses -> {paths.SELECTION}')
        return 0
    sel_path = os.environ.get('HARNESS_SELECTION')
    if sel_path and cmd in ('next', 'show'):  # release and record never depend on it
        try:
            with open(sel_path) as f:
                q = workqueue.restrict(q, json.load(f)['entries'])
        except (OSError, ValueError, KeyError) as e:
            print(f'ERROR cannot read the selection {sel_path}: {e}', file=sys.stderr)
            return 2
    if cmd == 'next':
        pid = int(os.environ.get('HARNESS_LOOP_PID', os.getppid()))
        # The near-miss labels route a reference-only function (workqueue.route); off with
        # HARNESS_NEARMISS=0.
        labels = nearmiss.load_labels() if nearmiss.enabled() else None
        ended, parked = [], []
        ms = matched.load(paths.MATCHED_SET)
        resolver = pair_resolver()
        cache = {}

        def pair_of(a):
            if a not in cache:
                cache[a] = resolver.pair_of(int(a, 16))
            return cache[a]

        def grant(a):
            group = pair_of(a) if resolver else None
            partners = [f'0x{p:08x}' for p in group.partners] if hasattr(
                group, 'partners') else []
            return workqueue.acquire_group(paths.CLAIMS, a, partners, owner, pid)
        exact = {f'0x{int(k, 16):08x}' for k, v in ms['functions'].items() if v.get('exact')}
        e = workqueue.next_entry(q, attempts, difficult, tiers, grant, revisit, take_revisit,
                                 labels, ended, pair_of if resolver else None, exact, parked)
        for entry, reason in parked:
            if workqueue.park(paths, entry, reason):
                print(f'INFO  {entry["address"]} listed in difficult_functions.tsv (lane '
                      f'{paths.LANE}): a deleting destructor with no identifiable real '
                      f'destructor ({reason})', file=sys.stderr)
        for entry, p, rows in ended:
            workqueue.end_reference_only(paths, entry, p, rows)
            print(f'INFO  {entry["address"]} listed in difficult_functions.tsv (lane '
                  f'{paths.LANE}): a reference-only near miss gets no Fable try', file=sys.stderr)
        if e:
            if e.get('pair'):
                e['pair']['states'], e['pair']['names'] = pair_states(e, ms)
            print(json.dumps(e))
        return 0
    if cmd == 'release':
        workqueue.release_group(paths.CLAIMS, args[0], owner)
        return 0
    if cmd == 'record':
        row = json.loads(args[0])
        size = next((e['size'] for e in q if e['address'] == row['address']), None)
        entry = {'address': row['address'], 'name': row.get('name', ''), 'size': size}
        where = workqueue.record(paths, entry, row, tiers, take_revisit)
        if row.get('partners'):
            workqueue.settle_partners(paths, entry, where, row, row['partners'], tiers,
                                      take_revisit)
        if where in ('difficult', 'revisit'):
            print(f'INFO  {row["address"]} listed in {where}_functions.tsv (lane {paths.LANE})')
        elif where == 'cleared':
            print(f'INFO  {row["address"]} taken off this lane\'s revisit_functions.tsv')
        elif where and where.startswith('left:'):
            print(f'INFO  {row["address"]} is exact; listed by lane {where[5:]}, left there '
                  f'(the queue skips it because it is exact)')
        return 0
    if cmd == 'show':
        n = int(args[0]) if args else 20
        listed = {r['address'] for r in revisit}
        for e in q[:n]:
            k = workqueue.counted_attempts(attempts, e['address'])
            if e['address'] in listed:
                k = f'{k} (revisit)'
            print(f'{e["address"]}  {e["size"]:>6}  sim {e["similarity"]:.2f}  '
                  f'{"named " if e["named"] else "      "}  attempts {k}  {e["name"]}')
        print(f'{len(q)} queued, {len(attempts)} attempts logged, {len(difficult)} difficult, '
              f'{len(revisit)} to revisit')
        return 0
    print(__doc__)
    return 2


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
