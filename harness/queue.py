#!/usr/bin/env python3
"""The work queue for harness/loop.sh.

    harness/queue.py build                 # (re)build $HARNESS_STATE/queue.json
    harness/queue.py next                  # claim the next function: JSON with its model, or nothing
    harness/queue.py release <address>     # drop this checkout's claim on a function
    harness/queue.py record <json-row>     # log an attempt (loop.sh only)
    harness/queue.py show [N]              # the first N entries and the attempt totals

Skips library-flagged functions, EH funclets and thunks, functions already exact in
matched_set.json, and those in difficult_functions.tsv. See lib/workqueue.py for the order.
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
    if cmd == 'build':
        index = funcs.FunctionIndex(paths.FUNCTIONS_CSV)
        exe = coff.PeImage(open(paths.EXE, 'rb').read())
        q = workqueue.build(index, exe, matched.load(paths.MATCHED_SET), difficult, tiers)
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
    if cmd == 'next':
        pid = int(os.environ.get('HARNESS_LOOP_PID', os.getppid()))
        e = workqueue.next_entry(q, attempts, difficult, tiers,
                                 lambda a: workqueue.acquire(paths.CLAIMS, a, owner, pid))
        if e:
            print(json.dumps(e))
        return 0
    if cmd == 'release':
        workqueue.release(paths.CLAIMS, args[0], owner)
        return 0
    if cmd == 'record':
        row = json.loads(args[0])
        entry = {'address': row['address'], 'name': row.get('name', '')}
        moved = workqueue.record(paths, entry, row, tiers)
        if moved:
            print(f'INFO  {row["address"]} moved to difficult_functions.tsv after {tiers.cap} attempts')
        return 0
    if cmd == 'show':
        n = int(args[0]) if args else 20
        for e in q[:n]:
            k = workqueue.counted_attempts(attempts, e['address'])
            print(f'{e["address"]}  {e["size"]:>6}  sim {e["similarity"]:.2f}  '
                  f'{"named " if e["named"] else "      "}  attempts {k}  {e["name"]}')
        print(f'{len(q)} queued, {len(attempts)} attempts logged, {len(difficult)} difficult')
        return 0
    print(__doc__)
    return 2


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
