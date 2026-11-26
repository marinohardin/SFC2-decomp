#!/usr/bin/env python3
"""Claims on units of work, kept as GitHub issues (CONTRIBUTING.md, "Find and claim work").

    python3 tools/claims.py free [--issues FILE] [--limit N]
    python3 tools/claims.py expire --now 2026-11-25T03:00:00Z [--issues FILE] [--apply]

`free` lists the units of op/UNITS.tsv that hold functions not yet exact in
analysis/results/matched_set.json, leaving out every unit that overlaps the `range:` lines of an
open issue labelled `claim` or `reserved` (the owner's grind batches). Library functions, thunks,
EH funclets and the boundary errors are not counted. Smallest unmatched bytes first.

`expire` handles claims that have gone quiet. Activity is the issue's creation or any comment
other than this tool's own. At 10 days without activity it posts one reminder; at 14 it closes
the issue with a comment. `reserved` issues are never touched. Without `--apply` it only prints
what it would do. The workflow .github/workflows/claim-expiry.yml passes the date.

The issues come from `gh issue list` (read-only for `free`), or from `--issues`, a JSON list of
issues as `gh issue list --json number,body,labels,state,createdAt,comments` prints them.
"""
import argparse
import bisect
import csv
import datetime
import json
import os
import subprocess
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)
import contrib_guard  # noqa: E402

UNITS_TSV = os.path.join(REPO, 'op', 'UNITS.tsv')
FUNCTIONS_CSV = os.path.join(REPO, 'ghidra', 'exports', 'functions.csv')
MATCHED_SET = os.path.join(REPO, 'analysis', 'results', 'matched_set.json')
BOUNDARIES = os.path.join(REPO, 'analysis', 'results', 'boundary_errors.tsv')
FIELDS = 'number,title,body,labels,state,createdAt,comments'
MARKER = '<!-- claims.py expire -->'
REMIND_DAYS, CLOSE_DAYS = 10, 14


def gh_issues(labels=('claim', 'reserved')):
    out = []
    for label in labels:
        r = subprocess.run(['gh', 'issue', 'list', '--state', 'open', '--label', label,
                            '--limit', '1000', '--json', FIELDS], capture_output=True, text=True)
        if r.returncode != 0:
            raise RuntimeError(f'gh issue list --label {label}: {r.stderr.strip()}')
        out += json.loads(r.stdout)
    return out


def taken_ranges(issues):
    """The ranges of open issues labelled claim or reserved."""
    out = []
    for i in issues:
        if str(i.get('state', 'open')).lower() == 'open' \
                and contrib_guard.labels(i) & {'claim', 'reserved'}:
            out += contrib_guard.parse_ranges(i.get('body'))
    return out


def free_units(units_rows, functions_rows, matched, boundaries, taken):
    """[(unit, first, last, unmatched functions, unmatched bytes)], smallest bytes first."""
    work = sorted((int(r['address'], 16), int(r['size'] or 0)) for r in functions_rows
                  if r['kind'] == 'function' and r['is_library'] == '0')
    exact = {int(k, 16) for k, v in matched.items() if v.get('exact')}
    starts = [a for a, _ in work]
    out = []
    for u in units_rows:
        lo, hi = int(u['first'], 16), int(u['last'], 16)
        if any(a <= hi and lo <= b for a, b in taken):
            continue
        todo = [(a, s) for a, s in work[bisect.bisect_left(starts, lo):
                                        bisect.bisect_right(starts, hi)]
                if a not in exact and a not in boundaries]
        if todo:
            out.append((u['unit'], lo, hi, len(todo), sum(s for _, s in todo)))
    return sorted(out, key=lambda x: (x[4], x[1]))


def read_tsv(path):
    with open(path, newline='') as f:
        return list(csv.DictReader(f, delimiter='\t'))


def cmd_free(a, issues):
    with open(FUNCTIONS_CSV, newline='') as f:
        functions_rows = list(csv.DictReader(f))
    with open(MATCHED_SET) as f:
        matched = json.load(f).get('functions', {})
    boundaries = set()
    if os.path.isfile(BOUNDARIES):
        boundaries = {int(r['address'], 16) for r in read_tsv(BOUNDARIES)}
    rows = free_units(read_tsv(UNITS_TSV), functions_rows, matched, boundaries,
                      taken_ranges(issues))
    print('unit\trange\tunmatched functions\tunmatched bytes')
    for unit, lo, hi, n, size in rows[:a.limit]:
        print(f'op/{unit}\trange: 0x{lo:08x}-0x{hi:08x}\t{n}\t{size}')
    print(f'# {len(rows)} units free', file=sys.stderr)
    return 0


def parse_time(text):
    return datetime.datetime.fromisoformat(text.replace('Z', '+00:00'))


def expiry_plan(issues, now):
    """[(action, number, idle days)]: 'remind' or 'close' for each quiet claim."""
    out = []
    for i in issues:
        names = contrib_guard.labels(i)
        if 'claim' not in names or 'reserved' in names \
                or str(i.get('state', 'open')).lower() != 'open':
            continue
        times = [parse_time(i['createdAt'])]
        reminded = None
        for c in i.get('comments', []):
            t = parse_time(c['createdAt'])
            if MARKER in (c.get('body') or ''):
                reminded = t if reminded is None or t > reminded else reminded
            else:
                times.append(t)
        last = max(times)
        idle = (now - last).days
        if idle >= CLOSE_DAYS:
            out.append(('close', int(i['number']), idle))
        elif idle >= REMIND_DAYS and not (reminded and reminded > last):
            out.append(('remind', int(i['number']), idle))
    return out


REMIND = (f'{MARKER}\nThis claim has had no activity for {{days}} days. Comment here with your '
          f'progress to keep it; it is closed after {CLOSE_DAYS} days without activity, and the '
          'units become free again.')
CLOSE = (f'{MARKER}\nClosed after {{days}} days without activity; the units are free again. Open '
         'a new claim to carry on.')


def cmd_expire(a, issues, run=subprocess.run):
    now = parse_time(a.now)
    for action, number, days in expiry_plan(issues, now):
        print(f'{action} #{number} ({days} days without activity)')
        if not a.apply:
            continue
        if action == 'remind':
            cmd = ['gh', 'issue', 'comment', str(number), '--body', REMIND.format(days=days)]
        else:
            cmd = ['gh', 'issue', 'close', str(number), '--comment', CLOSE.format(days=days)]
        if run(cmd).returncode != 0:
            print(f'claims: {" ".join(cmd[:4])} failed')
            return 1
    return 0


def main(argv=None, run=subprocess.run):
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    sub = ap.add_subparsers(dest='cmd', required=True)
    f = sub.add_parser('free', help='list unclaimed units with work left')
    f.add_argument('--issues', help='a JSON file of issues instead of gh')
    f.add_argument('--limit', type=int, default=30)
    e = sub.add_parser('expire', help='remind or close quiet claims')
    e.add_argument('--now', required=True, help='the current time, ISO 8601 (UTC)')
    e.add_argument('--issues', help='a JSON file of issues instead of gh')
    e.add_argument('--apply', action='store_true', help='post the comments and close')
    a = ap.parse_args(argv)
    try:
        if a.issues:
            with open(a.issues, encoding='utf-8') as fh:
                issues = json.load(fh)
        else:
            issues = gh_issues(('claim', 'reserved') if a.cmd == 'free' else ('claim',))
    except (OSError, ValueError, RuntimeError) as err:
        print(f'claims: {err}')
        return 2
    return cmd_free(a, issues) if a.cmd == 'free' else cmd_expire(a, issues, run)


if __name__ == '__main__':
    sys.exit(main())
