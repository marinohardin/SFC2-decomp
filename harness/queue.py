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

Skips library-flagged functions, EH funclets and thunks, functions already exact in
matched_set.json, those in difficult_functions.tsv, and the boundary errors in
boundary_errors.tsv. Functions in revisit_functions.tsv are
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
from harness.lib import coff, funcs, matched, paths, workqueue  # noqa: E402


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    cmd, args = argv[0], argv[1:]
    tiers = workqueue.Tiers.from_env()
    difficult = workqueue.read_tsv(paths.DIFFICULT, workqueue.DIFFICULT_FIELDS)
    revisit = workqueue.read_tsv(paths.REVISIT, workqueue.REVISIT_FIELDS)
    take_revisit = os.environ.get('HARNESS_TAKE_REVISIT') == '1'
    if cmd == 'sweep':
        exact = {k for k, v in matched.load(paths.MATCHED_SET)['functions'].items()
                 if v.get('exact')}
        got = workqueue.sweep(paths, tiers, time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime()),
                              exact)
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
        e = workqueue.next_entry(q, attempts, difficult, tiers,
                                 lambda a: workqueue.acquire(paths.CLAIMS, a, owner, pid),
                                 revisit, take_revisit)
        if e:
            print(json.dumps(e))
        return 0
    if cmd == 'release':
        workqueue.release(paths.CLAIMS, args[0], owner)
        return 0
    if cmd == 'record':
        row = json.loads(args[0])
        entry = {'address': row['address'], 'name': row.get('name', '')}
        where = workqueue.record(paths, entry, row, tiers, take_revisit)
        if where in ('difficult', 'revisit'):
            print(f'INFO  {row["address"]} listed in {where}_functions.tsv')
        elif where == 'cleared':
            print(f'INFO  {row["address"]} taken off revisit_functions.tsv')
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
