#!/usr/bin/env python3
"""Map every function of the original exe to the translation unit it was compiled in.

Writes `game/UNITS.tsv`, one row per unit, and the `game/` tree skeleton: a placeholder
`game/<unit>.cpp` for every unit, a placeholder `game/<unit>.h` where a header beside the unit is
evidenced, and the units' entries in `harness/units.json`. See `game/README.md`.

Evidence, strongest first:

- `assert-path`: an absolute source path in an assertion string the function uses
  (`translation_unit` in `ghidra/exports/functions.csv`). Relative paths (`.\\DataStore\\...`,
  `..\\Common\\...`) are the path an `#include` resolved to, so they mark code inlined from
  another file and are not unit evidence.
- `sdk-member`: the function's bytes match an SDK library function
  (`analysis/results/StarFleetOP_2552_sdklib_matches.tsv`) whose defining object names one
  source file (its `.file` symbol). Select-any COMDATs (inline and template functions) are
  emitted by every object that uses them, so they are not evidence.
- `class`: functions of one class, outside every unit above, go to `unsorted/<Class>.cpp`.
- `page`: the rest go to `unsorted/fn_<page>.cpp`, by 4 KB address page.

The linker kept each unit's functions together, so a unit is one run of consecutive functions in
address order and no two units overlap. Evidence that would put one unit in two places is
resolved by keeping the unit's largest run (most anchors) and dropping the anchors elsewhere,
largest units first; the functions between two anchors of a unit belong to it, and EH funclets
directly after a unit's last function stay with it.

Usage: python3 tools/units.py [--sdk DIR] [--check]
"""
import argparse
import collections
import csv
import json
import os
import re
import struct
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FUNCTIONS_CSV = os.path.join(REPO, 'ghidra', 'exports', 'functions.csv')
SDK_MATCHES = os.path.join(REPO, 'analysis', 'results', 'StarFleetOP_2552_sdklib_matches.tsv')
GAME = os.path.join(REPO, 'game')
UNITS_TSV = os.path.join(GAME, 'UNITS.tsv')
UNITS_JSON = os.path.join(REPO, 'harness', 'units.json')

PROFILE = 'sdk-release'
PAGE = 0x1000
COLUMNS = ('unit', 'first', 'last', 'functions', 'evidence', 'profile', 'header')
MARKER = '// Placeholder written by tools/units.py.'
# Paths under game/ that other packages own: the generator never writes or deletes them.
NOT_OURS = ('port/', 'common/dynamicallocator.')

# Original source roots, matched inside an absolute path, and their directory under game/
# (docs/CONVENTIONS.md section 2). The SDK's script projects keep their copy of the game's
# shared source under SCRIPTS\Shared, laid out as the game's tree.
ROOTS = [
    (r'\\SFC2_5\\SCRIPTS\\Shared\\Source\\Taldren\\Common\\', 'Common/'),
    (r'\\SFC2_5\\SCRIPTS\\Shared\\Source\\', 'Source/'),
    (r'\\SFC2_5\\SCRIPTS\\Shared\\Meta\\', 'Meta/'),
    (r'\\SFC2_5\\Source\\', 'Source/'),
    (r'\\SFC2_5\\Meta\\', 'Meta/'),
    (r'\\Taldren\\Projects\\Common\\', 'Common/'),
    (r'\\Taldren\\Projects\\Switch\\', 'Switch/'),
    (r'\\QSIInc\\Tiger\\', 'Tiger/'),
    (r'\\CORE\\dmp3d\\', 'dmp3d/'),
]
ROOTS = [(re.compile(p, re.I), d) for p, d in ROOTS]
ABSOLUTE = re.compile(r'^[A-Za-z]:\\')
# The SDK's shipped source tree (Scripts/shared) and its directories under game/.
SDK_TREE = [('Source/Taldren/Common/', 'Common/'), ('Source/', 'Source/'), ('Meta/', 'Meta/'),
            ('QSI/Projects/QSIInc/Tiger/', 'Tiger/')]
CLASS_NAME = re.compile(r'^([A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)::~?[A-Za-z_]\w*$')

# Evidence strength for spelling a directory or file name that the evidence spells in more than
# one case: the exe's own strings first.
SPELLING = {'assert-path': 0, 'sdk-member': 1, 'header': 2, 'sdk-tree': 3}


class Function:
    __slots__ = ('address', 'size', 'name', 'kind', 'is_library', 'tu', 'tu_evidence', 'headers')

    def __init__(self, row):
        self.address = int(row['address'], 16)
        self.size = int(row['size'] or 0)
        self.name = row['name']
        self.kind = row['kind']
        self.is_library = row['is_library'] == '1'
        self.tu = row['translation_unit']
        self.tu_evidence = row['tu_evidence']
        self.headers = row['headers']


def read_functions(path=FUNCTIONS_CSV):
    with open(path, newline='') as f:
        fns = [Function(r) for r in csv.DictReader(f)]
    fns.sort(key=lambda fn: fn.address)
    return fns


def game_path(original):
    """`game/`-relative path of an absolute original source path, or None for another root."""
    if not original or not ABSOLUTE.match(original):
        return None
    for pattern, directory in ROOTS:
        m = pattern.search(original)
        if m:
            rest = original[m.end():].replace('\\', '/')
            return directory + rest if rest else None
    return None


def header_path(original):
    """`game/`-relative path of a header named by functions.csv (absolute, or relative to a
    project directory: `..\\..\\Common\\X.h`)."""
    p = game_path(original)
    if p:
        return p
    m = re.match(r'^(?:\.\.\\)+(Common|Switch)\\(.+)$', original or '', re.I)
    return f'{m.group(1)}/{m.group(2)}'.replace('\\', '/') if m else None


# --- the SDK libraries ------------------------------------------------------------------------

def archive_members(data):
    """[(member name, bytes)] of a COFF archive (`!<arch>`)."""
    if data[:8] != b'!<arch>\n':
        raise ValueError('not a COFF archive')
    off, longnames, out = 8, b'', []
    while off + 60 <= len(data):
        hdr = data[off:off + 60]
        name = hdr[:16].decode('latin1').rstrip()
        size = int(hdr[48:58].decode('latin1').strip() or 0)
        body = data[off + 60:off + 60 + size]
        if name == '//':
            longnames = body
        elif name not in ('/', '//'):
            if name.startswith('/') and name[1:].rstrip('/').isnumeric():
                i = int(name[1:].rstrip('/'))
                name = longnames[i:longnames.index(b'\0', i)].decode('latin1')
            out.append((name.rstrip('/'), body))
        off += 60 + size + (size & 1)
    return out


def object_evidence(obj):
    """(main source file, [other .file names], {defined function symbol: select_any}) of one
    i386 COFF object, or None for anything else."""
    if len(obj) < 20:
        return None
    machine, nsec, _, symoff, nsym, _, _ = struct.unpack_from('<HHIIIHH', obj, 0)
    if machine != 0x14c or symoff + nsym * 18 > len(obj):
        return None
    strtab = symoff + nsym * 18
    files, selection, functions = [], {}, {}
    i = 0
    while i < nsym:
        raw, value, section, typ, sclass, naux = struct.unpack_from('<8sIhHBB', obj, symoff + i * 18)
        if raw[:4] == b'\0\0\0\0':
            start = strtab + struct.unpack('<I', raw[4:])[0]
            name = obj[start:obj.find(b'\0', start)].decode('latin1')
        else:
            name = raw.rstrip(b'\0').decode('latin1')
        aux = obj[symoff + (i + 1) * 18:symoff + (i + 1 + naux) * 18]
        if sclass == 103:                                  # .file: the name is in the aux records
            files.append(aux.rstrip(b'\0').decode('latin1'))
        elif sclass == 3 and naux and value == 0 and section > 0 and len(aux) >= 15:
            selection[section] = aux[14]                   # section definition: COMDAT selection
        elif sclass == 2 and section > 0 and (typ >> 4) == 2:
            functions[name] = section
        i += 1 + naux
    if not files:
        return None
    return files[0], files[1:], {n: selection.get(s) == 2 for n, s in functions.items()}


def sdk_evidence(sdk_dir, matches_tsv=SDK_MATCHES):
    """{address: set of unit paths} for the exact SDK matches whose definition is not
    select-any, and the header paths the matched objects name, from the SDK's libraries."""
    defs = collections.defaultdict(set)                    # symbol -> unit paths
    object_headers = collections.defaultdict(set)          # unit path -> header paths
    lib_dir = os.path.join(sdk_dir, 'Scripts', 'Libraries')
    for lib in sorted(os.listdir(lib_dir)):
        if not lib.lower().endswith('.lib'):
            continue
        with open(os.path.join(lib_dir, lib), 'rb') as f:
            try:
                members = archive_members(f.read())
            except ValueError:
                continue
        for _, body in members:
            ev = object_evidence(body)
            if not ev:
                continue
            main, others, functions = ev
            unit = game_path(main)
            if not unit:
                continue
            for other in others:
                if other.lower().endswith('.h'):
                    h = game_path(other) or _relative_to(unit, other)
                    if h:
                        object_headers[unit].add(h)
            for name, select_any in functions.items():
                if not select_any:
                    defs[name].add(unit)
    by_address = collections.defaultdict(set)
    with open(matches_tsv, newline='') as f:
        for row in csv.reader(f, delimiter='\t'):
            if len(row) >= 3 and re.fullmatch(r'[0-9a-fA-F]{8}', row[0]):
                by_address[int(row[0], 16)] |= defs.get(row[2], set())
    return dict(by_address), dict(object_headers)


def _relative_to(unit, header):
    if ABSOLUTE.match(header) or header.startswith('..'):
        return None
    base = unit.rsplit('/', 1)[0]
    return f'{base}/{header[2:] if header.startswith(".\\") else header}'.replace('\\', '/')


def sdk_tree_headers(sdk_dir):
    """`game/`-relative paths of the headers the SDK ships in its copy of the shared tree."""
    root = os.path.join(sdk_dir, 'Scripts', 'shared')
    out = set()
    for d, dirs, files in os.walk(root):
        dirs.sort()
        rel = os.path.relpath(d, root).replace(os.sep, '/') + '/'
        for prefix, target in SDK_TREE:
            if rel.lower().startswith(prefix.lower()):
                out.update(target + rel[len(prefix):] + f for f in files if f.lower().endswith('.h'))
                break
    return out


# --- the map ----------------------------------------------------------------------------------

def _resolve(labels):
    """Make every label one run: while a label occurs in more than one run of consecutive
    labelled positions, drop its runs but the largest, taking the label with the largest run
    first. `labels` is {position: key}; returns the kept ones."""
    labels = dict(labels)
    while True:
        runs = collections.defaultdict(list)
        prev = None
        for pos in sorted(labels):
            key = labels[pos]
            if key != prev:
                runs[key].append([])
                prev = key
            runs[key][-1].append(pos)
        split = {k: r for k, r in runs.items() if len(r) > 1}
        if not split:
            return labels
        key = max(split, key=lambda k: (max(len(r) for r in split[k]), -split[k][0][0]))
        keep = max(split[key], key=len)
        for r in split[key]:
            if r is not keep:
                for pos in r:
                    del labels[pos]


def _class_of(fn):
    if fn.is_library or fn.kind != 'function':
        return None
    m = CLASS_NAME.match(fn.name)
    if not m or m.group(1).startswith(('std::', 'FUN_')):
        return None
    return m.group(1)


def build(fns, sdk_units=None, headers=()):
    """The unit map: a list of dicts with the COLUMNS, in address order, covering every function
    once. `sdk_units` is {address: set of unit paths}; `headers` the evidenced header paths."""
    sdk_units = sdk_units or {}
    spellings = []                                         # (path, evidence) for the spelling vote
    strong = {}                                            # position -> lower-case unit path
    kinds = collections.defaultdict(set)
    for pos, fn in enumerate(fns):
        unit, kind = None, None
        if fn.tu_evidence == 'assert-path' and ';' not in fn.tu:
            unit, kind = game_path(fn.tu), 'assert-path'
        if not unit:
            cands = {u.lower(): u for u in sdk_units.get(fn.address, ())}
            if len(cands) == 1:
                unit, kind = next(iter(cands.values())), 'sdk-member'
        if unit:
            strong[pos] = unit.lower()
            kinds[unit.lower()].add(kind)
            spellings.append((unit, kind))
        for h in filter(None, fn.headers.split(';')):
            hp = header_path(h)
            if hp:
                spellings.append((hp, 'header'))
    for h, kind in headers:
        spellings.append((h, kind))
    spell = _spelling(spellings)

    strong = _resolve(strong)
    unit_of = _fill(len(fns), fns, strong)
    classes = {}
    for pos, fn in enumerate(fns):
        if pos not in unit_of:
            c = _class_of(fn)
            if c:
                classes[pos] = 'unsorted/' + c.replace('::', '__') + '.cpp'
    combined = _resolve({**{p: k for p, k in unit_of.items()}, **classes})
    classes = {p: k for p, k in combined.items() if p not in unit_of}
    unit_of.update(_fill(len(fns), fns, classes, taken=unit_of))

    rows, pos = [], 0
    while pos < len(fns):
        key = unit_of.get(pos)
        end = pos
        if key is None:
            page = fns[pos].address // PAGE
            while end + 1 < len(fns) and end + 1 not in unit_of and fns[end + 1].address // PAGE == page:
                end += 1
        else:
            while end + 1 < len(fns) and unit_of.get(end + 1) == key:
                end += 1
        rows.append(dict(key=key, start=pos, end=end))
        pos = end + 1

    used, out = set(), []
    for r in rows:
        first, last = fns[r['start']], fns[r['end']]
        if r['key'] is None:
            name = f'unsorted/fn_{first.address // PAGE * PAGE:08X}.cpp'
            if name.lower() in used:
                name = f'unsorted/fn_{first.address:08X}.cpp'
            evidence, unit = 'page', name
        elif r['key'].startswith('unsorted/'):
            evidence, unit = 'class', r['key']
            if unit.lower() in used:                       # a case-only clash between classes
                unit = unit[:-4] + f'_{first.address:08X}.cpp'
        else:
            evidence = 'assert-path' if 'assert-path' in kinds[r['key']] else 'sdk-member'
            unit = spell(r['key'])
        used.add(unit.lower())
        out.append(dict(unit=unit, first=f'0x{first.address:08x}', last=f'0x{last.address:08x}',
                        functions=r['end'] - r['start'] + 1, evidence=evidence, profile=PROFILE,
                        header=''))
    header_set = {spell(h.lower()).lower(): spell(h.lower()) for h, _ in spellings
                  if h.lower().endswith('.h')}
    for row in out:
        if row['evidence'] in ('assert-path', 'sdk-member'):
            h = row['unit'].rsplit('.', 1)[0] + '.h'
            row['header'] = header_set.get(h.lower(), '')
    return out


def _fill(n, fns, labels, taken=None):
    """{position: key}: each label's run from its first to its last position, then the EH
    funclets right after it."""
    taken = taken or {}
    out = {}
    span = {}
    for pos, key in labels.items():
        lo, hi = span.get(key, (pos, pos))
        span[key] = (min(lo, pos), max(hi, pos))
    for key, (lo, hi) in span.items():
        for pos in range(lo, hi + 1):
            out[pos] = key
        pos = hi + 1
        while pos < n and pos not in taken and pos not in labels and fns[pos].kind == 'eh-funclet':
            out[pos] = key
            pos += 1
    return out


def _spelling(spellings):
    """A function from a lower-case path to its spelling: each directory and file name as the
    strongest evidence spells it, then the most frequent spelling, then the first in order."""
    votes = collections.defaultdict(collections.Counter)
    for path, kind in spellings:
        comps = path.split('/')
        for i, comp in enumerate(comps):
            votes['/'.join(comps[:i + 1]).lower()][(SPELLING[kind], comp)] += 1
    best = {k: min(c.items(), key=lambda kv: (kv[0][0], -kv[1], kv[0][1]))[0][1]
            for k, c in votes.items()}

    def spell(lower):
        comps = lower.split('/')
        return '/'.join(best.get('/'.join(comps[:i + 1]), c) for i, c in enumerate(comps))
    return spell


# --- writing ----------------------------------------------------------------------------------

def format_tsv(rows):
    lines = ['\t'.join(COLUMNS)]
    lines += ['\t'.join(str(r[c]) for c in COLUMNS) for r in rows]
    return '\n'.join(lines) + '\n'


def read_tsv(path=UNITS_TSV):
    with open(path, newline='') as f:
        rows = list(csv.DictReader(f, delimiter='\t'))
    for r in rows:
        r['functions'] = int(r['functions'])
    return rows


EVIDENCE_TEXT = {
    'assert-path': 'assert-path (the original\'s assertion strings name this file)',
    'sdk-member': 'sdk-member (an SDK library object compiled from this file has these functions)',
    'class': 'class (functions of this class outside every evidenced unit; original file unknown)',
    'page': 'page (functions with no unit or class evidence, by 4 KB page; original file unknown)',
}


def placeholder(row):
    return (f'{MARKER} Delete this block when the unit\'s first\n'
            f'// function lands; the generator never touches a file with content.\n'
            f'// Unit:      {row["unit"]}\n'
            f'// Functions: {row["first"]} to {row["last"]}, {row["functions"]} in game/UNITS.tsv\n'
            f'// Evidence:  {EVIDENCE_TEXT[row["evidence"]]}\n'
            f'// Profile:   {row["profile"]} (harness/units.json)\n')


def header_placeholder(row):
    return (f'{MARKER} Delete this block when the header\'s first\n'
            f'// declaration lands; the generator never touches a file with content.\n'
            f'// Header of: {row["unit"]}\n')


PLACEHOLDER_FIELDS = re.compile(
    r'// Unit:      (?P<unit>\S+)\n'
    r'// Functions: (?P<first>0x[0-9a-f]{8}) to (?P<last>0x[0-9a-f]{8}), (?P<functions>\d+) in game/UNITS\.tsv\n'
    r'// Evidence:  (?P<evidence>[a-z-]+) .*\n'
    r'// Profile:   (?P<profile>\S+) \(harness/units\.json\)\n')
HEADER_FIELDS = re.compile(r'// Header of: (?P<unit>\S+)\n')


def is_placeholder(text):
    """True only when `text` is exactly what placeholder() or header_placeholder() writes for
    some unit, so a line an author adds, a note under the marker included, makes it content."""
    m = PLACEHOLDER_FIELDS.search(text)
    if m and m.group('evidence') in EVIDENCE_TEXT:
        row = dict(m.groupdict(), functions=int(m.group('functions')))
        if text == placeholder(row):
            return True
    m = HEADER_FIELDS.search(text)
    return bool(m) and text == header_placeholder(m.groupdict())


def _ours(rel):
    return not rel.lower().startswith(NOT_OURS)


def write_tree(rows, game_dir=GAME):
    """Write the placeholders for `rows` under game_dir; remove placeholders of units no longer in
    the map. Returns (written, removed, kept_with_content)."""
    want = {}
    for r in rows:
        want[r['unit']] = placeholder(r)
        if r['header']:
            want[r['header']] = header_placeholder(r)
    want = {k: v for k, v in want.items() if _ours(k)}
    written, removed, kept = [], [], []
    for rel, text in sorted(want.items()):
        path = os.path.join(game_dir, rel)
        if os.path.exists(path):
            with open(path, encoding='latin1') as f:
                old = f.read()
            if old == text:
                continue
            if not is_placeholder(old):
                kept.append(rel)
                continue
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, 'w', encoding='latin1', newline='\n') as f:
            f.write(text)
        written.append(rel)
    wanted = {k.lower() for k in want}
    for d, dirs, files in os.walk(game_dir, topdown=False):
        for name in files:
            if not name.lower().endswith(('.cpp', '.h')):
                continue
            path = os.path.join(d, name)
            rel = os.path.relpath(path, game_dir).replace(os.sep, '/')
            if rel.lower() in wanted or not _ours(rel):
                continue
            with open(path, encoding='latin1') as f:
                if is_placeholder(f.read()):
                    os.remove(path)
                    removed.append(rel)
        if d != game_dir and not os.listdir(d):
            os.rmdir(d)
    return written, removed, kept


def update_units_json(rows, path=UNITS_JSON):
    """Add an entry for every unit in the map; existing entries and profiles are kept."""
    with open(path) as f:
        cfg = json.load(f)
    units = cfg.setdefault('units', {})
    for r in rows:
        units.setdefault(r['unit'], {'profile': r['profile']})
    with open(path, 'w', newline='\n') as f:
        f.write(format_units_json(cfg))


def format_units_json(cfg):
    """units.json as the harness reads it, one line per unit so a diff shows one line per
    change."""
    rest = {k: v for k, v in cfg.items() if k != 'units'}
    head = json.dumps(rest, indent=1)[:-2]                 # without the closing "\n}"
    lines = [f'  {json.dumps(k)}: {json.dumps(v, sort_keys=True)}'
             for k, v in sorted(cfg.get('units', {}).items())]
    body = ',\n'.join(lines)
    return head + ',\n "units": {\n' + body + ('\n' if lines else '') + ' }\n}\n'


def default_sdk():
    sys.path.insert(0, REPO)
    from harness.lib import paths                         # the install's location, in one place
    return os.path.join(paths.REFERENCE, os.path.dirname(paths.EXE_REL), 'OP API - R2.1')


def sdk_dir(explicit=None):
    """The SDK directory: `--sdk`, else `SFCOP_SDK`, else beside the exe in the install."""
    return explicit or os.environ.get('SFCOP_SDK') or default_sdk()


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('--sdk', help='the SDK directory (default: beside the exe in the install)')
    ap.add_argument('--check', action='store_true',
                    help='only compare the map with game/UNITS.tsv; exit 1 if it differs')
    args = ap.parse_args(argv)
    sdk = sdk_dir(args.sdk)
    if not os.path.isdir(os.path.join(sdk, 'Scripts', 'Libraries')):
        sys.exit(f'units.py: the SDK libraries are needed and were not found under {sdk} '
                 '(pass --sdk or set SFCOP_SDK)')
    fns = read_functions()
    sdk_units, object_headers = sdk_evidence(sdk)
    rows = build(fns, sdk_units, [])
    units = {r['unit'].lower() for r in rows}
    evidenced = [(h, 'sdk-member') for u, hs in object_headers.items() if u.lower() in units for h in hs]
    evidenced += [(h, 'sdk-tree') for h in sdk_tree_headers(sdk)]
    rows = build(fns, sdk_units, evidenced)
    text = format_tsv(rows)
    if args.check:
        with open(UNITS_TSV) as f:
            same = f.read() == text
        print('units.py: game/UNITS.tsv is ' + ('current' if same else 'out of date'))
        return 0 if same else 1
    os.makedirs(GAME, exist_ok=True)
    with open(UNITS_TSV, 'w', newline='\n') as f:
        f.write(text)
    written, removed, kept = write_tree(rows)
    update_units_json(rows)
    counts = collections.Counter(r['evidence'] for r in rows)
    fn_counts = collections.Counter()
    for r in rows:
        fn_counts[r['evidence']] += r['functions']
    print(f'units.py: {len(rows)} units for {len(fns)} functions; '
          + ', '.join(f'{k} {counts[k]} units / {fn_counts[k]} functions' for k in EVIDENCE_TEXT))
    print(f'units.py: placeholders written {len(written)}, removed {len(removed)}, '
          f'left alone because they have content {len(kept)}')
    for rel in kept:
        print(f'  has content, not rewritten: {rel}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
