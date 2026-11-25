#!/usr/bin/env python3
"""Plan grind lanes: check that a lane owns the headers its classes need, before it starts.

A lane may edit only the files on its lane file list (`HARNESS_LANE_FILES`). A function whose
class is declared in a header that units outside the lane also include cannot be worked: the
lane may not edit that header. This reads the include graph of `op/`, the unit map
(`op/UNITS.tsv`), the symbol table and the matched set, and for each lane reports

- which headers the lane's units include, and who else includes them: `owned` (only this lane's
  units), `shared` (units outside the lane), `contested` (a unit of another lane in the plan);
- for each function of the lane's units that is not exact, the header that declares its class,
  and whether the lane owns it (workable) or not (blocked, naming the header);
- the lane file list: the units' sources and the headers the lane owns.

    python3 tools/lane_plan.py --units units.txt
    python3 tools/lane_plan.py --lane widgets=units-widgets.txt --lane threads=units-threads.txt
    python3 tools/lane_plan.py --lane a=a.txt --write-files out/ --max-blocked 0.2

A units file lists one unit per line, as in `op/UNITS.tsv` (`unsorted/tSlider.cpp`); a leading
`op/` is accepted, blank lines and `#` comments are skipped. Exit 0 when every lane is within
`--max-blocked` (a share of its non-exact functions) and no two lanes contest a file, 1 when a
lane is over the threshold, a unit is in two lanes, or a header is given to two lanes (grants), 2 on a usage error.

`--grant LANE=HEADER` (repeatable) is the integrator's decision to give a shared header to one lane:
it counts as owned by that lane, goes on its file list and is contested for every other lane; the
output lists each grant with the units outside the lane that include it. A grant of a header under
`op/port/` is refused. A header under `op/port/` is a `hotspot`: never owned, never on a list, and a
function whose class is declared there is blocked. `--write-files DIR` writes `files-<lane>.txt` (what `HARNESS_LANE_FILES` takes) and
`functions-<lane>.tsv` (address, size, class, unit, status, blocking header) for each lane.

Limits: the class header is the first header found that defines `class X` or `struct X` with a
body, preferring one the lane's own units include; a class defined nowhere under `op/` is
`undeclared` and counts as blocked (the lane has no header it may extend). Only quoted includes
that resolve under `op/` and `op/port/` make the graph; system headers are ignored.
"""
import argparse
import csv
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
INCLUDE = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]', re.M)
CLASS_DEF = re.compile(r'\b(?:class|struct)\s+(?:__declspec\s*\([^)]*\)\s*)?(\w+)\s*(?::[^;{]*)?\{')
LANE_NAME = re.compile(r'^[A-Za-z0-9_-]+$')
COMMENT = re.compile(r'/\*.*?\*/|//[^\n]*', re.S)
SOURCE_EXT = ('.cpp', '.c', '.cc')
HEADER_EXT = ('.h', '.hpp', '.inl')


class Tree:
    """The include graph and the class declarations of one source tree (`<root>/op`)."""

    def __init__(self, root):
        self.root = root
        self.op = os.path.join(root, 'op')
        self.dirs = [self.op, os.path.join(self.op, 'port')]
        self.files = {}        # rel path under op/ -> text
        for base, dnames, fnames in os.walk(self.op):
            for n in fnames:
                if n.lower().endswith(SOURCE_EXT + HEADER_EXT):
                    p = os.path.join(base, n)
                    with open(p, encoding='latin1') as f:
                        self.files[os.path.relpath(p, self.op).replace(os.sep, '/')] = f.read()
        self.direct = {r: self._includes(r) for r in self.files}
        self.declares = {}     # class name -> [header rel paths]
        for r, text in self.files.items():
            if r.lower().endswith(HEADER_EXT):
                for m in CLASS_DEF.finditer(COMMENT.sub('', text)):
                    self.declares.setdefault(m.group(1), []).append(r)
        self._closure = {}
        self._includers = None

    def _resolve(self, name, includer):
        """Beside the including file, then under op/, then under op/port/ (the /I list)."""
        cands = [os.path.join(os.path.dirname(includer), name)] if includer else []
        cands += [name, os.path.join('port', name)]
        for c in cands:
            c = os.path.normpath(c).replace(os.sep, '/')
            if c in self.files:
                return c
        return None

    def _includes(self, rel):
        out = []
        for kind, name in INCLUDE.findall(COMMENT.sub('', self.files[rel])):
            r = self._resolve(name, rel) if kind == '"' else self._resolve(name, None)
            if r:
                out.append(r)
        return out

    def closure(self, rel):
        """Every file `rel` includes, directly or not (not itself)."""
        if rel not in self._closure:
            seen, todo = set(), list(self.direct.get(rel, ()))
            while todo:
                p = todo.pop()
                if p not in seen:
                    seen.add(p)
                    todo.extend(self.direct.get(p, ()))
            seen.discard(rel)
            self._closure[rel] = seen
        return self._closure[rel]

    def sources(self):
        return sorted(r for r in self.files
                      if r.lower().endswith(SOURCE_EXT) and not r.startswith('port/'))

    def includers(self, header):
        """Every source unit that includes the header, directly or through other headers."""
        if self._includers is None:
            self._includers = {}
            for u in self.sources():
                for h in self.closure(u):
                    self._includers.setdefault(h, set()).add(u)
        return self._includers.get(header, set())


def read_units(path):
    out = []
    with open(path) as f:
        for line in f:
            line = line.split('#', 1)[0].strip()
            if line:
                out.append(line[3:] if line.startswith('op/') else line)
    return out


def function_sizes(root):
    """{address as int: size} from the function boundaries export (ghidra/exports/functions.csv)."""
    out = {}
    path = os.path.join(root, 'ghidra/exports/functions.csv')
    if os.path.isfile(path):
        with open(path, newline='') as f:
            for r in csv.DictReader(f):
                try:
                    out[int(r['address'], 16)] = int(r['size'])
                except (ValueError, KeyError):
                    pass
    return out


def unit_functions(root, units):
    """{unit: [(address, size, class, exact, claimed)]} for the units, from UNITS.tsv, the symbol
    table, the matched set and the manifest."""
    ranges = {}
    with open(os.path.join(root, 'op/UNITS.tsv'), newline='') as f:
        for row in csv.DictReader(f, delimiter='\t'):
            if row['unit'] in units:
                ranges[row['unit']] = (int(row['first'], 16), int(row['last'], 16))
    with open(os.path.join(root, 'analysis/results/matched_set.json')) as f:
        matched = json.load(f).get('functions', {})
    claimed = set()
    man = os.path.join(root, 'op/manifest.tsv')
    if os.path.isfile(man):
        with open(man, newline='') as f:
            claimed = {r['address'] for r in csv.DictReader(f, delimiter='\t')}
    out = {u: [] for u in units}
    with open(os.path.join(root, 'analysis/symbols/STARFLEETOP.tsv'), newline='') as f:
        syms = [r for r in csv.DictReader(f, delimiter='\t') if r['kind'] == 'function']
    sizes = function_sizes(root)
    for r in syms:
        a = int(r['address'], 16)
        for u, (lo, hi) in ranges.items():
            if lo <= a <= hi:
                key = r['address']
                m = matched.get(key, {})
                out[u].append((key, sizes.get(a), r['class'], bool(m.get('exact')),
                               key in claimed))
                break
    return out


def class_header(tree, cls, lane_units):
    """The header that declares `cls`, preferring one a lane unit includes; None if none."""
    heads = tree.declares.get(cls, [])
    if not heads:
        return None
    seen = set()
    for u in lane_units:
        seen |= tree.closure(u)
    for h in heads:
        if h in seen:
            return h
    return heads[0]


def plan(tree, lanes, functions, grants=None):
    """lanes: {name: [unit]}; functions: {unit: rows}; grants: {header: lane}. Returns
    ({name: result dict}, {unit: [lanes]} for units in several lanes)."""
    grants = grants or {}
    unit_lane = {}
    for name, units in lanes.items():
        for u in units:
            unit_lane.setdefault(u, []).append(name)
    result = {}
    for name, units in lanes.items():
        inside = set(units)
        others = {u for n, us in lanes.items() if n != name for u in us}

        def state_of(h):
            users = tree.includers(h)
            outside = len(users - inside)
            if h.startswith('port/'):
                return ('hotspot', outside)
            if h in grants:
                return ('owned', 0) if grants[h] == name else ('contested', outside)
            if users & others:
                return ('contested', outside)
            return ('shared', outside) if outside else ('owned', 0)

        headers = set()
        for u in units:
            headers |= {h for h in tree.closure(u) if h.lower().endswith(HEADER_EXT)}
        headers |= {h for h, n in grants.items() if n == name}
        klass = {h: state_of(h) for h in sorted(headers)}
        rows = []
        for u in units:
            for addr, size, cls, exact, claimed in functions.get(u, ()):
                if exact:
                    continue
                if not cls:
                    rows.append((addr, size, cls, u, 'workable', '', 0, claimed))
                    continue
                h = class_header(tree, cls, units)
                if h is None:
                    rows.append((addr, size, cls, u, 'blocked', '(undeclared)', 0, claimed))
                    continue
                if h not in klass:
                    # declared in a header none of the lane's units reach: still the lane's
                    # to edit when nobody else includes it, so it goes on the file list
                    klass[h] = state_of(h)
                state, n = klass[h]
                if state == 'owned':
                    rows.append((addr, size, cls, u, 'workable', h, 0, claimed))
                else:
                    rows.append((addr, size, cls, u, 'blocked', h, n, claimed))
        owned = sorted(h for h, (st, _) in klass.items() if st == 'owned')
        given = [(h, len(tree.includers(h) - inside)) for h in sorted(grants)
                 if grants[h] == name]
        result[name] = {'units': units, 'headers': klass, 'rows': rows, 'grants': given,
                        'files': sorted(['op/' + u for u in units] + ['op/' + h for h in owned])}
    return result, {u: n for u, n in unit_lane.items() if len(n) > 1}


def conflicts(result, dup_units):
    """Messages for a unit in two lanes, or a file the plan gives to two lanes. A header two
    lanes are both blocked by is not a conflict: the ranking reports it."""
    out = ['unit %s is in lanes %s' % (u, ', '.join(n)) for u, n in sorted(dup_units.items())]
    given = {}
    for name, r in result.items():
        for f in r['files']:
            given.setdefault(f, []).append(name)
    for f, names in sorted(given.items()):
        if len(names) > 1 and f.endswith(HEADER_EXT):
            out.append('file %s is given to lanes %s' % (f, ', '.join(sorted(names))))
    return out


def blocking_rank(result):
    rank = {}
    for name, r in result.items():
        for row in r['rows']:
            if row[4] == 'blocked':
                e = rank.setdefault(row[5], [0, 0, set()])
                e[0] += 1
                e[1] = max(e[1], row[6])
                e[2].add(name)
    return sorted(((h, v[0], v[1], sorted(v[2])) for h, v in rank.items()),
                  key=lambda t: (-t[1], t[0]))


def write_files(result, out_dir):
    for name in result:
        if not LANE_NAME.match(name):
            raise ValueError('lane name %r: use letters, digits, _ and -' % name)
    os.makedirs(out_dir, exist_ok=True)
    for name, r in result.items():
        with open(os.path.join(out_dir, 'files-%s.txt' % name), 'w') as f:
            f.write(''.join(p + '\n' for p in r['files']))
        with open(os.path.join(out_dir, 'functions-%s.tsv' % name), 'w') as f:
            f.write('address\tsize\tclass\tunit\tstatus\tblocking_header\n')
            for addr, size, cls, u, status, h, _n, _c in r['rows']:
                f.write('%s\t%s\t%s\t%s\t%s\t%s\n' % (
                    addr, '' if size is None else size, cls, u, status,
                    h if status == 'blocked' else ''))


def report(result, dup, max_blocked, out):
    bad = False
    for name, r in result.items():
        rows = r['rows']
        blocked = [x for x in rows if x[4] == 'blocked']
        share = len(blocked) / len(rows) if rows else 0.0
        counts = {}
        for s, _ in r['headers'].values():
            counts[s] = counts.get(s, 0) + 1
        over = share > max_blocked
        bad = bad or over
        out.write('lane %s: %d units, %d non-exact functions: %d workable, %d blocked (%.0f%%)%s\n'
                  % (name, len(r['units']), len(rows), len(rows) - len(blocked), len(blocked),
                     share * 100, '  OVER %.0f%%' % (max_blocked * 100) if over else ''))
        out.write('  headers: %d owned, %d shared, %d contested, %d hotspot; lane file list: '
                  '%d files\n' % (counts.get('owned', 0), counts.get('shared', 0),
                                  counts.get('contested', 0), counts.get('hotspot', 0),
                                  len(r['files'])))
        for h, n in r['grants']:
            out.write('  granted %s: %d units outside the lane include it; keep them unbroken\n'
                      % (h, n))
        by = {}
        for x in blocked:
            e = by.setdefault(x[5], [0, x[6]])
            e[0] += 1
        for h, (n, outside) in sorted(by.items(), key=lambda kv: -kv[1][0]):
            out.write('  blocked %3d  %s  (%d units outside the lane include it)\n' % (n, h, outside))
    problems = conflicts(result, dup)
    for p in problems:
        out.write('CONFLICT: %s\n' % p)
    rank = blocking_rank(result)
    if rank:
        out.write('\nShared headers to settle before the lanes start, by functions blocked:\n')
        for h, n, outside, names in rank:
            out.write('  %4d  %-40s %4d outside includers  lanes: %s\n'
                      % (n, h, outside, ', '.join(names)))
    return 1 if bad or problems else 0


def main(argv=None, root=ROOT, out=sys.stdout):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--units', help='file of candidate units: one lane named "lane"')
    ap.add_argument('--lane', action='append', default=[], metavar='NAME=FILE',
                    help='a lane and its units file; repeat for several lanes')
    ap.add_argument('--max-blocked', type=float, default=0.2,
                    help='largest share of a lane\'s non-exact functions that may be blocked')
    ap.add_argument('--grant', action='append', default=[], metavar='LANE=HEADER',
                    help='give a shared header to one lane (repeatable); op/port/ is refused')
    ap.add_argument('--write-files', metavar='DIR')
    ap.add_argument('--root', default=root)
    args = ap.parse_args(argv)
    lanes = {}
    for spec in args.lane:
        if '=' not in spec:
            ap.error('--lane takes NAME=FILE')
        name, path = spec.split('=', 1)
        lanes[name] = read_units(path)
    if args.units:
        lanes['lane'] = read_units(args.units)
    if not lanes:
        ap.error('give --units or --lane')
    if args.write_files:
        bad = [n for n in lanes if not LANE_NAME.match(n)]
        if bad:
            sys.stderr.write('lane name %s: use letters, digits, _ and -\n' % ', '.join(bad))
            return 2
    tree = Tree(args.root)
    every = {u for us in lanes.values() for u in us}
    sources = set(tree.sources())
    missing = sorted(u for u in every if u not in sources)
    if missing:
        sys.stderr.write('not a source unit under op/: %s\n' % ', '.join(missing))
        return 2
    grants = {}
    for spec in args.grant:
        lane, _, h = spec.partition('=')
        h = h[3:] if h.startswith('op/') else h
        if lane not in lanes or not h.lower().endswith(HEADER_EXT) or h not in tree.files:
            sys.stderr.write('bad grant %s: needs a lane in the plan and a header under op/\n' % spec)
            return 2
        if h.startswith('port/'):
            sys.stderr.write('grant refused: op/%s is a hotspot\n' % h)
            return 2
        if grants.get(h, lane) != lane:
            sys.stderr.write('header %s is granted to two lanes\n' % h)
            return 2
        grants[h] = lane
    result, dup = plan(tree, lanes, unit_functions(args.root, every), grants)
    code = report(result, dup, args.max_blocked, out)
    if args.write_files:
        write_files(result, args.write_files)
    return code


if __name__ == '__main__':
    sys.exit(main())
