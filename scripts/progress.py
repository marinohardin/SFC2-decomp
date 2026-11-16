#!/usr/bin/env python3
"""Progress metrics for the decompilation: how much of the original code is replaced, and how much
of it is named.

Every ordinary function of the original exe (kind `function` in `ghidra/exports/functions.csv`)
gets one status and one name tier:

  status      exact         rebuilt byte-exact (harness matched set, or objdiff at 100%)
              functional    rebuilt and verified by the replay oracle
              partial       claimed in op/ and scored, not yet exact (manifest mode `partial`)
              lifted        still mechanically lifted (Route L): its entry lies in lifted code
              original-x86  none of the above; still only the original machine code

  named-by    sdk-exact     byte-exact SDK library match (P0-3 step 3, or the libmatch table)
              sdk-fid       Ghidra Function ID against the SDK objects
              vs-fid        Ghidra's bundled Visual Studio Function ID (CRT)
              manual        named by hand from evidence (name_source `manual`)
              sdk-fuzzy     fuzzy SDK matcher, tiers it trusts as labels (score >= 0.85)
              string        a name taken from a string in the exe
              rtti-class    a class from RTTI recovery, method name may be a placeholder
              none          FUN_xxxxxxxx

The first tier that applies wins. A C++ EH funclet (`Unwind@`/`Catch@`) belongs to the function
whose EH tables reference it and is counted under that function's status: its bytes are added to
the status and also reported apart as funclet bytes. The funclet-to-parent table,
`analysis/results/progress/op/eh_funclet_parents.tsv`, is built once from the original exe with
`--build-funclet-map`; funclets it does not list stay unclassified. Thunks are counted, not
classified.

Byte percentages are given over the original `.text` (3,751,936 bytes) and over the bytes of
ordinary functions plus their funclets; a game-only scope repeats the figures without the
functions flagged as library code (CRT, STL, WONAPI, Crypto++, import thunks, and GameSpy or
SmartHeap once flagged). The flag comes from names, so unnamed library code still counts as game
code.

Every input is optional: a missing file contributes nothing, so before the harness exists exact
and functional are zero. Accepted shapes of the harness inputs:

  harness/matched_set.json, harness/functional_set.json
      a list of addresses ("00401000", "0x00401000" or an integer), or of objects with an
      "address" and an optional "status" ("exact" or "functional"); or an object whose "exact",
      "functional" or "functions" keys hold such lists. Entries without a status are exact in
      matched_set.json and functional in functional_set.json.
  op/manifest.tsv
      the claims tools/manifest.py writes; a row whose modes include `partial` is partial.
      Partial is only as fresh as the manifest: regenerate it first (python3 tools/manifest.py).
  harness/objdiff/**/*.json
      objdiff report JSON ("units" -> "functions"); a function counts as exact when its
      "fuzzy_match_percent" is 100. The original address comes only from
      "metadata.virtual_address" (decimal); a function without it is skipped with a warning.

Usage:
  python3 scripts/progress.py                    write progress.json, print summary and table
  python3 scripts/progress.py --markdown         also write PROGRESS.md and append a history line
  python3 scripts/progress.py --gate BASE.json   write nothing; exit 1 if a function that was
                                                 exact or functional in BASE.json no longer is
  python3 scripts/progress.py --build-funclet-map EXE
                                                 rebuild eh_funclet_parents.tsv from the exe

The gate compares against the base branch's copy of progress.json, never the working tree's own
(which a pull request regenerates), for example:
  git show origin/develop:analysis/results/progress/op/progress.json > base.json
  python3 scripts/progress.py --gate base.json
"""
import argparse
import bisect
import csv
import datetime
import glob
import hashlib
import json
import os
import pathlib
import re
import struct
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]

# Size of the original .text section (ghidra/REPORT.md).
TEXT_BYTES = 3_751_936

STATUSES = ('exact', 'functional', 'partial', 'lifted', 'original-x86')
NAME_TIERS = ('sdk-exact', 'sdk-fid', 'vs-fid', 'manual', 'sdk-fuzzy', 'string', 'rtti-class',
              'none')
REAL_SYMBOL_TIERS = ('sdk-exact', 'sdk-fid', 'vs-fid', 'manual', 'sdk-fuzzy', 'string')
# Names applied in functions.csv; they outrank sdk-fuzzy, which is a candidate from a side table.
APPLIED_NAME_TIERS = ('sdk-fid', 'vs-fid', 'manual')

# functions.csv name_source -> name tier. sdklib-exact and the fuzzy table are checked separately.
NAME_SOURCE_TIER = {
    'sdklib-exact': 'sdk-exact',
    'sdk-fid': 'sdk-fid',
    'ghidra-fid-vs': 'vs-fid',
    'manual': 'manual',
    # A game copy of a library function, named by hand in analysis/results/game_copy_names.tsv
    # with its evidence; ghidra/results/counts.md counts these as named too.
    'game-copy': 'manual',
    'string': 'string',
    'rtti-class-recovery': 'rtti-class',
}
# Sources that carry no evidence for a name: Ghidra placeholders, switch-case labels, the PE entry.
NO_NAME_SOURCES = {'', 'default', 'ghidra-analysis', 'imported', 'eh-analyzer'}

# analysis/README.md: fuzzy, exact-short, low-score ref and multi rows are leads, not names.
FUZZY_LABEL_TIERS = {'exact', 'exact+ctx', 'masked', 'masked+ctx', 'ref'}
FUZZY_MIN_SCORE = 0.85


def _work_dir():
    """The shared work directory beside the main checkout (worktrees live inside it)."""
    if os.environ.get('SFCOP_WORK'):
        return pathlib.Path(os.environ['SFCOP_WORK'])
    try:
        common = subprocess.run(['git', '-C', str(ROOT), 'rev-parse', '--path-format=absolute',
                                 '--git-common-dir'], capture_output=True, text=True, check=True)
        return pathlib.Path(common.stdout.strip()).parent.parent / 'work'
    except (OSError, subprocess.CalledProcessError):
        return ROOT.parent / 'work'


def parse_hex_address(value):
    """An address from our own files: an integer, or hex with or without 0x."""
    if isinstance(value, bool):
        return None
    if isinstance(value, int):
        return value
    if isinstance(value, str) and value.strip():
        try:
            return int(value.strip(), 16)
        except ValueError:
            return None
    return None


def fmt_address(address):
    return '%08x' % address


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for block in iter(lambda: f.read(1 << 20), b''):
            h.update(block)
    return h.hexdigest()


class InputLog:
    """Records every input the run looked at, with its hash, so a result can be traced back."""

    def __init__(self, work_dir):
        self.work_dir = pathlib.Path(work_dir) if work_dir else None
        self.entries = {}

    def display(self, path):
        path = pathlib.Path(path)
        for base, label in ((ROOT, ''), (self.work_dir, '$SFCOP_WORK/')):
            if base is None:
                continue
            try:
                return label + path.resolve().relative_to(base.resolve()).as_posix()
            except ValueError:
                continue
        return path.as_posix()

    def note(self, key, path):
        """Log `path` under `key`; return True if it exists."""
        present = path is not None and os.path.isfile(path)
        self.entries[key] = {
            'path': self.display(path) if path is not None else None,
            'present': present,
            'sha256': sha256_file(path) if present else None,
        }
        return present


class Function:
    __slots__ = ('address', 'size', 'name', 'name_source', 'kind', 'is_library', 'library')

    def __init__(self, address, size, name, name_source, kind, is_library, library):
        self.address = address
        self.size = size
        self.name = name
        self.name_source = name_source
        self.kind = kind
        self.is_library = is_library
        self.library = library


def load_functions(path, log):
    if not log.note('functions_csv', path):
        return []
    out = []
    with open(path, newline='') as f:
        for row in csv.DictReader(f):
            address = parse_hex_address(row.get('address'))
            if address is None:
                continue
            out.append(Function(address, int(row.get('size') or 0), row.get('name', ''),
                                row.get('name_source', ''), row.get('kind', 'function'),
                                row.get('is_library') == '1', row.get('library', '')))
    return out


def _is_section_id(name):
    # libmatch names a COMDAT with no function symbol by its object section, e.g. "x.obj:.text#3".
    return '.obj:' in name or '.obj|' in name


def load_sdk_exact(path, log):
    """Addresses the byte-exact SDK table gives a real symbol name."""
    if not log.note('sdk_exact_tsv', path):
        return set()
    out = set()
    with open(path) as f:
        for line in f:
            parts = line.rstrip('\n').split('\t')
            if len(parts) < 3 or _is_section_id(parts[2]):
                continue
            address = parse_hex_address(parts[0])
            if address is not None:
                out.add(address)
    return out


def load_sdk_fuzzy(path, log):
    """Addresses the fuzzy SDK matcher names with a tier and score it trusts as a label."""
    if not log.note('sdk_fuzzy_tsv', path):
        return set()
    out = set()
    with open(path, newline='') as f:
        for row in csv.DictReader(f, delimiter='\t'):
            if row.get('tier') not in FUZZY_LABEL_TIERS or _is_section_id(row.get('mangled_name', '')):
                continue
            try:
                if float(row.get('score') or 0) < FUZZY_MIN_SCORE:
                    continue
            except ValueError:
                continue
            address = parse_hex_address(row.get('address'))
            if address is not None:
                out.add(address)
    return out


def load_funclet_parents(path, log):
    """funclet address -> parent function address, from eh_funclet_parents.tsv."""
    if not log.note('eh_funclet_parents', path):
        return {}
    out = {}
    with open(path) as f:
        for line in f:
            parts = line.rstrip('\n').split('\t')
            if line.startswith('#') or len(parts) < 2:
                continue
            funclet, parent = parse_hex_address(parts[0]), parse_hex_address(parts[1])
            if funclet is not None and parent is not None:
                out[funclet] = parent
    return out


def load_partial(path, log):
    """Addresses op/manifest.tsv claims with the mode `partial`: written, scored, not exact."""
    if not log.note('manifest', path):
        return set()
    out = set()
    with open(path, newline='') as f:
        for row in csv.DictReader(f, delimiter='\t'):
            if 'partial' not in (row.get('modes') or '').split():
                continue
            address = parse_hex_address(row.get('address'))
            if address is not None:
                out.add(address)
    return out


STUB_RE = re.compile(r'void sub_([0-9A-Fa-f]{8})\(void\) \{ RECOMP_NOT_LIFTED\(0x')
ERROR_RE = re.compile(r'^0x([0-9A-Fa-f]{8})\t')


def load_lifted(lifter_dir, log):
    """Sorted, merged [start, end) ranges of lifted code, and a summary of the lift.

    The lifter's catalog (`cat/functions.json`) lists each function it lifts with its extent. A
    lift run writes `out/lift_stats.json`; without it nothing counts as lifted. Entries that hit a
    lift error (`out/lift_errors.txt`) or were emitted as not-lifted stubs (`out/gen/*.c`, read
    only when the stats report stubs) are left out.
    """
    lifter_dir = pathlib.Path(lifter_dir) if lifter_dir else None
    catalog = lifter_dir / 'cat' / 'functions.json' if lifter_dir else None
    stats_path = lifter_dir / 'out' / 'lift_stats.json' if lifter_dir else None
    errors_path = lifter_dir / 'out' / 'lift_errors.txt' if lifter_dir else None
    have_catalog = log.note('lift_catalog', catalog)
    have_stats = log.note('lift_stats', stats_path)
    have_errors = log.note('lift_errors', errors_path)
    info = {'catalog_functions': 0, 'excluded_errors': 0, 'excluded_stubs': 0, 'ranges': 0}
    if not (have_catalog and have_stats):
        return [], info
    with open(stats_path) as f:
        stats = json.load(f)
    with open(catalog) as f:
        cat = json.load(f)
    entries = cat.get('functions', []) if isinstance(cat, dict) else cat
    excluded = set()
    if have_errors:
        with open(errors_path) as f:
            for line in f:
                m = ERROR_RE.match(line)
                if m:
                    excluded.add(int(m.group(1), 16))
    info['excluded_errors'] = len(excluded)
    if stats.get('not_lifted_stubs'):
        stubs = set()
        for path in sorted(glob.glob(str(lifter_dir / 'out' / 'gen' / '*.c'))):
            with open(path, errors='replace') as f:
                stubs.update(int(m.group(1), 16) for m in STUB_RE.finditer(f.read()))
        info['excluded_stubs'] = len(stubs - excluded)
        excluded |= stubs
    ranges = []
    for e in entries:
        start, end = e.get('address'), e.get('end')
        if isinstance(start, int) and isinstance(end, int) and end > start and start not in excluded:
            ranges.append((start, end))
    info['catalog_functions'] = len(entries)
    merged = []
    for start, end in sorted(ranges):
        if merged and start <= merged[-1][1]:
            merged[-1][1] = max(merged[-1][1], end)
        else:
            merged.append([start, end])
    info['ranges'] = len(merged)
    return [tuple(r) for r in merged], info


def in_ranges(address, ranges):
    lo, hi = 0, len(ranges)
    while lo < hi:
        mid = (lo + hi) // 2
        if ranges[mid][1] <= address:
            lo = mid + 1
        else:
            hi = mid
    return lo < len(ranges) and ranges[lo][0] <= address < ranges[lo][1]


def _replaced_entries(data, default_status):
    """(address, status) pairs from a matched/functional set in any accepted shape."""
    if isinstance(data, dict):
        for key, status in (('exact', 'exact'), ('functional', 'functional'),
                            ('functions', default_status)):
            yield from _replaced_entries_list(data.get(key) or [], status)
    elif isinstance(data, list):
        yield from _replaced_entries_list(data, default_status)


def _replaced_entries_list(items, default_status):
    if isinstance(items, dict):
        # The harness's own shape: {"0x...": {"exact": bool, "score": ...}}. Only an entry it
        # records exact counts; a partial match it keeps for a later attempt does not.
        for key, record in items.items():
            address = parse_hex_address(key)
            if isinstance(record, dict) and 'exact' in record:
                status = default_status if record['exact'] is True else None
            else:
                status = default_status
            if address is not None and status in ('exact', 'functional'):
                yield address, status
        return
    for item in items:
        if isinstance(item, dict):
            address = parse_hex_address(item.get('address'))
            status = item.get('status') or default_status
        else:
            address, status = parse_hex_address(item), default_status
        if address is not None and status in ('exact', 'functional'):
            yield address, status


def _objdiff_address(fn):
    """The original address of an objdiff report function, or None.

    Only `metadata.virtual_address` is the address in the original exe; the report's own `address`
    field is the symbol's offset in its section and must not be used in its place.
    """
    value = (fn.get('metadata') or {}).get('virtual_address')
    if isinstance(value, int) and not isinstance(value, bool):
        return value
    if isinstance(value, str) and value.strip():
        try:
            return int(value, 0) if value.startswith(('0x', '0X')) else int(value)
        except ValueError:
            return None
    return None


def load_replaced(matched_set, functional_set, objdiff_reports, log):
    """Sets of exact and functional addresses, and the number of objdiff functions skipped."""
    exact, functional = set(), set()
    skipped = 0
    for key, path, default in (('matched_set', matched_set, 'exact'),
                               ('functional_set', functional_set, 'functional')):
        if not log.note(key, path):
            continue
        with open(path) as f:
            for address, status in _replaced_entries(json.load(f), default):
                (exact if status == 'exact' else functional).add(address)
    for i, path in enumerate(objdiff_reports):
        if not log.note('objdiff_report_%d' % i, path):
            continue
        with open(path) as f:
            report = json.load(f)
        for unit in report.get('units', []) if isinstance(report, dict) else []:
            for fn in unit.get('functions', []):
                if float(fn.get('fuzzy_match_percent') or 0) < 100.0:
                    continue
                address = _objdiff_address(fn)
                if address is None:
                    skipped += 1
                    print('progress: warning: %s: %s/%s has no metadata.virtual_address; skipped'
                          % (log.display(path), unit.get('name', '?'), fn.get('name', '?')),
                          file=sys.stderr)
                    continue
                exact.add(address)
    return exact, functional - exact, skipped


def classify_status(address, exact, functional, partial, lifted_ranges):
    if address in exact:
        return 'exact'
    if address in functional:
        return 'functional'
    if address in partial:
        return 'partial'
    if in_ranges(address, lifted_ranges):
        return 'lifted'
    return 'original-x86'


def classify_name(fn, sdk_exact, sdk_fuzzy):
    if fn.name_source == 'sdklib-exact' or fn.address in sdk_exact:
        return 'sdk-exact'
    tier = NAME_SOURCE_TIER.get(fn.name_source)
    if tier in APPLIED_NAME_TIERS:
        return tier
    if fn.address in sdk_fuzzy:
        return 'sdk-fuzzy'
    return tier or 'none'


def _pct(part, whole):
    return round(100.0 * part / whole, 4) if whole else 0.0


class Row:
    __slots__ = ('fn', 'status', 'tier', 'funclet_bytes')

    def __init__(self, fn, status, tier):
        self.fn = fn
        self.status = status
        self.tier = tier
        self.funclet_bytes = 0


def _byte_totals(rows):
    fb = sum(r.fn.size for r in rows)
    eb = sum(r.funclet_bytes for r in rows)
    return {'function_bytes': fb, 'funclet_bytes': eb, 'bytes': fb + eb}


def _scope(rows, text_bytes=None):
    """Counts and percentages for one scope."""
    n = len(rows)
    totals = _byte_totals(rows)
    status = {}
    for st in STATUSES:
        sel = [r for r in rows if r.status == st]
        entry = dict({'functions': len(sel), 'pct_functions': _pct(len(sel), n)}, **_byte_totals(sel))
        entry['pct_bytes'] = _pct(entry['bytes'], totals['bytes'])
        if text_bytes is not None:
            entry['pct_text'] = _pct(entry['bytes'], text_bytes)
        status[st] = entry
    named = {}
    for tier in NAME_TIERS:
        sel = [r for r in rows if r.tier == tier]
        named[tier] = {'functions': len(sel), 'function_bytes': sum(r.fn.size for r in sel),
                       'pct_functions': _pct(len(sel), n)}
    real = sum(named[t]['functions'] for t in REAL_SYMBOL_TIERS)
    with_class = real + named['rtti-class']['functions']
    return dict({'functions': n}, **totals, **{
        'status': status,
        'named': named,
        'named_real_symbol': {'functions': real, 'pct_functions': _pct(real, n)},
        'named_with_class': {'functions': with_class, 'pct_functions': _pct(with_class, n)},
    })


def compute(paths, now, text_bytes=TEXT_BYTES):
    """Build the progress report. `paths` maps input names to paths (None or missing = absent)."""
    log = InputLog(paths.get('work_dir'))
    functions = load_functions(paths.get('functions_csv'), log)
    sdk_exact = load_sdk_exact(paths.get('sdk_exact_tsv'), log)
    sdk_fuzzy = load_sdk_fuzzy(paths.get('sdk_fuzzy_tsv'), log)
    funclet_parents = load_funclet_parents(paths.get('eh_funclet_parents'), log)
    lifted_ranges, lift_info = load_lifted(paths.get('lifter_dir'), log)
    exact, functional, objdiff_skipped = load_replaced(
        paths.get('matched_set'), paths.get('functional_set'), paths.get('objdiff_reports') or [], log)
    partial = load_partial(paths.get('manifest'), log)

    rows = []
    by_address = {}
    unmapped = {}
    for fn in functions:
        if fn.kind != 'function':
            continue
        row = Row(fn, classify_status(fn.address, exact, functional, partial, lifted_ranges),
                  classify_name(fn, sdk_exact, sdk_fuzzy))
        if fn.name_source not in NAME_SOURCE_TIER and fn.name_source not in NO_NAME_SOURCES:
            unmapped[fn.name_source] = unmapped.get(fn.name_source, 0) + 1
        rows.append(row)
        by_address[fn.address] = row

    funclets = {'functions': 0, 'bytes': 0, 'with_parent': 0, 'with_parent_bytes': 0,
                'without_parent': 0, 'without_parent_bytes': 0}
    kinds = {}
    for fn in functions:
        k = kinds.setdefault(fn.kind, {'functions': 0, 'bytes': 0})
        k['functions'] += 1
        k['bytes'] += fn.size
        if fn.kind != 'eh-funclet':
            continue
        funclets['functions'] += 1
        funclets['bytes'] += fn.size
        parent = by_address.get(funclet_parents.get(fn.address))
        side = 'with_parent' if parent else 'without_parent'
        funclets[side] += 1
        funclets[side + '_bytes'] += fn.size
        if parent:
            parent.funclet_bytes += fn.size

    libraries = {}
    for r in rows:
        if r.fn.is_library:
            lib = libraries.setdefault(r.fn.library or 'unspecified', {'functions': 0, 'bytes': 0})
            lib['functions'] += 1
            lib['bytes'] += r.fn.size + r.funclet_bytes

    def listing(status):
        return [{'address': fmt_address(r.fn.address), 'name': r.fn.name}
                for r in sorted(rows, key=lambda r: r.fn.address) if r.status == status]

    return {
        'schema': 3,
        'generated_at': now,
        'text_bytes': text_bytes,
        'inputs': log.entries,
        'function_kinds': dict(sorted(kinds.items())),
        'eh_funclets': funclets,
        'scopes': {
            'total': _scope(rows, text_bytes),
            'game': _scope([r for r in rows if not r.fn.is_library]),
        },
        'excluded_libraries': dict(sorted(libraries.items())),
        'lift': lift_info,
        'objdiff_skipped_without_address': objdiff_skipped,
        'unmapped_name_sources': dict(sorted(unmapped.items())),
        'exact': listing('exact'),
        'functional': listing('functional'),
        'partial': listing('partial'),
    }


def gate(previous, report):
    """Functions exact or functional in `previous` that are not at least as good in `report`.

    Compared by original address only, so a rename cannot hide a regression. Returns a list of
    (address, old status, new status, old name, new name). Raises ValueError when `previous` is not
    a progress.json.
    """
    if not isinstance(previous, dict):
        raise ValueError('not a progress.json object')
    current = {}
    for status in ('exact', 'functional'):
        for e in report.get(status, []):
            current[e['address']] = (status, e.get('name', ''))
    allowed = {'exact': ('exact',), 'functional': ('exact', 'functional')}
    out = []
    for status in ('exact', 'functional'):
        entries = previous.get(status, [])
        if not isinstance(entries, list):
            raise ValueError('"%s" is not a list' % status)
        for e in entries:
            address = parse_hex_address(e.get('address') if isinstance(e, dict) else None)
            if address is None:
                raise ValueError('"%s" entry without a valid address: %r' % (status, e))
            address = fmt_address(address)
            now_status, now_name = current.get(address, (None, ''))
            if now_status not in allowed[status]:
                out.append((address, status, now_status or 'not replaced', e.get('name', ''), now_name))
    return sorted(out)


def summary_line(report):
    t = report['scopes']['total']
    g = report['scopes']['game']
    s = t['status']
    return ('Progress: %.2f%% exact, %.2f%% functional, %.2f%% partial, %.2f%% lifted, %.2f%% original '
            'x86 of .text (%s bytes); named: %.2f%% real symbol, %.2f%% with class, of %s functions. '
            'Game-only: %.2f%% exact, %.2f%% functional, %.2f%% partial of %s bytes; %.2f%% real '
            'symbol, %.2f%% with class, of %s functions.'
            % (s['exact']['pct_text'], s['functional']['pct_text'], s['partial']['pct_text'],
               s['lifted']['pct_text'],
               s['original-x86']['pct_text'], format(report['text_bytes'], ','),
               t['named_real_symbol']['pct_functions'], t['named_with_class']['pct_functions'],
               format(t['functions'], ','),
               g['status']['exact']['pct_bytes'], g['status']['functional']['pct_bytes'],
               g['status']['partial']['pct_bytes'], format(g['bytes'], ','),
               g['named_real_symbol']['pct_functions'],
               g['named_with_class']['pct_functions'], format(g['functions'], ',')))


def table_rows(report):
    """(header, rows) for the status table and the name table."""
    t, g = report['scopes']['total'], report['scopes']['game']
    status_rows = []
    for st in STATUSES:
        a, b = t['status'][st], g['status'][st]
        status_rows.append((st, '{:,}'.format(a['functions']), '{:,}'.format(a['bytes']),
                            '{:,}'.format(a['funclet_bytes']), '%.2f' % a['pct_text'],
                            '%.2f' % a['pct_bytes'], '{:,}'.format(b['functions']),
                            '{:,}'.format(b['bytes']), '%.2f' % b['pct_bytes']))
    status_rows.append(('all', '{:,}'.format(t['functions']), '{:,}'.format(t['bytes']),
                        '{:,}'.format(t['funclet_bytes']),
                        '%.2f' % _pct(t['bytes'], report['text_bytes']), '100.00',
                        '{:,}'.format(g['functions']), '{:,}'.format(g['bytes']), '100.00'))
    status_header = ('status', 'functions', 'bytes', 'of which funclets', '% .text', '% code',
                     'game fns', 'game bytes', '% game code')
    name_rows = []
    for tier in NAME_TIERS:
        a, b = t['named'][tier], g['named'][tier]
        name_rows.append((tier, '{:,}'.format(a['functions']), '%.2f' % a['pct_functions'],
                          '{:,}'.format(b['functions']), '%.2f' % b['pct_functions']))
    for key, label in (('named_real_symbol', 'real symbol'), ('named_with_class', 'with class')):
        name_rows.append((label, '{:,}'.format(t[key]['functions']), '%.2f' % t[key]['pct_functions'],
                          '{:,}'.format(g[key]['functions']), '%.2f' % g[key]['pct_functions']))
    name_header = ('named by', 'functions', '%', 'game fns', '% game')
    return (status_header, status_rows), (name_header, name_rows)


def text_table(header, rows):
    widths = [max(len(str(r[i])) for r in [header] + rows) for i in range(len(header))]
    line = lambda r: '  '.join(str(c).ljust(w) if i == 0 else str(c).rjust(w)
                               for i, (c, w) in enumerate(zip(r, widths)))
    return '\n'.join([line(header), line(['-' * w for w in widths])] + [line(r) for r in rows])


def markdown_table(header, rows):
    out = ['| ' + ' | '.join(header) + ' |',
           '| ' + ' | '.join(['---'] + ['---:'] * (len(header) - 1)) + ' |']
    out += ['| ' + ' | '.join(str(c) for c in r) + ' |' for r in rows]
    return '\n'.join(out)


HISTORY_HEADING = '## History'


def history_line(report):
    t, g = report['scopes']['total'], report['scopes']['game']
    s = t['status']
    return ('- %s: exact %.2f%%, functional %.2f%%, partial %.2f%%, lifted %.2f%% of .text; real '
            'symbol %.2f%%, with class %.2f%% of %s functions; game-only exact %.2f%%, functional '
            '%.2f%%, partial %.2f%%'
            % (report['generated_at'], s['exact']['pct_text'], s['functional']['pct_text'],
               s['partial']['pct_text'], s['lifted']['pct_text'],
               t['named_real_symbol']['pct_functions'], t['named_with_class']['pct_functions'],
               format(t['functions'], ','), g['status']['exact']['pct_bytes'],
               g['status']['functional']['pct_bytes'], g['status']['partial']['pct_bytes']))


def render_markdown(report, previous_text):
    history = []
    if previous_text and HISTORY_HEADING in previous_text:
        tail = previous_text.split(HISTORY_HEADING, 1)[1]
        history = [l for l in tail.splitlines() if l.startswith('- ')]
    history.append(history_line(report))
    (sh, sr), (nh, nr) = table_rows(report)
    kinds, eh = report['function_kinds'], report['eh_funclets']
    libs = ', '.join('%s %s' % (k, format(v['functions'], ','))
                     for k, v in report['excluded_libraries'].items()) or 'none'
    parts = [
        '# Progress',
        '',
        'Written by `python3 scripts/progress.py --markdown`; do not edit by hand. Generated %s.'
        % report['generated_at'],
        '',
        summary_line(report),
        '',
        '## Code',
        '',
        'One row per ordinary function; its bytes include the EH funclets its EH tables reference '
        '(%s funclets, %s bytes, matched through `eh_funclet_parents.tsv`). `%% .text` is over the '
        'whole original `.text` (%s bytes); `%% code` is over the bytes of ordinary functions and '
        'their funclets. Not classified: %s funclets with no known parent (%s bytes) and %s thunks '
        '(%s bytes); the rest of `.text` is data and padding.'
        % (format(eh['with_parent'], ','), format(eh['with_parent_bytes'], ','),
           format(report['text_bytes'], ','), format(eh['without_parent'], ','),
           format(eh['without_parent_bytes'], ','),
           format(kinds.get('thunk', {}).get('functions', 0), ','),
           format(kinds.get('thunk', {}).get('bytes', 0), ',')),
        '',
        markdown_table(sh, sr),
        '',
        '## Names',
        '',
        'One tier per function, the first that applies in this order. "Real symbol" is every tier '
        'above `rtti-class`; "with class" adds `rtti-class`.',
        '',
        markdown_table(nh, nr),
        '',
        'Game-only leaves out the functions flagged as library code: %s. The flag comes from '
        'names, so unnamed library code still counts as game code.' % libs,
        '',
        HISTORY_HEADING,
        '',
    ]
    return '\n'.join(parts + history) + '\n'


# --- Funclet-to-parent table, built from the original exe -----------------------------------------

EH_MAGIC = 0x19930520  # FuncInfo magic number of the VC6 C++ EH tables


class PEImage:
    """Just enough of a PE32 image to read bytes by virtual address."""

    def __init__(self, data):
        self.data = data
        pe = struct.unpack_from('<I', data, 0x3c)[0]
        if data[pe:pe + 4] != b'PE\0\0':
            raise ValueError('not a PE image')
        nsec, = struct.unpack_from('<H', data, pe + 6)
        optsz, = struct.unpack_from('<H', data, pe + 20)
        self.image_base, = struct.unpack_from('<I', data, pe + 24 + 28)
        self.sections = []
        for i in range(nsec):
            o = pe + 24 + optsz + 40 * i
            name = data[o:o + 8].rstrip(b'\0').decode('ascii', 'replace')
            vsize, va, rsize, raw = struct.unpack_from('<IIII', data, o + 8)
            self.sections.append((name, self.image_base + va, raw, rsize))

    def read(self, va, n):
        for _, start, raw, rsize in self.sections:
            if start <= va and va + n <= start + rsize:
                return self.data[raw + va - start:raw + va - start + n]
        return None

    def u32(self, va):
        b = self.read(va, 4)
        return struct.unpack('<I', b)[0] if b else None

    def section(self, name):
        for sname, start, raw, rsize in self.sections:
            if sname == name:
                return start, self.data[raw:raw + rsize]
        raise ValueError('no %s section' % name)


def _funcinfo_targets(image, funcinfo):
    """Funclet addresses a FuncInfo references: unwind actions and catch handlers."""
    head = image.read(funcinfo + 4, 16)
    if head is None:
        return set()
    max_state, unwind_map, n_try, try_map = struct.unpack('<iIiI', head)
    out = set()
    for i in range(max(0, min(max_state, 1 << 16))):
        entry = image.read(unwind_map + 8 * i, 8)
        if entry is None:
            break
        action = struct.unpack('<iI', entry)[1]
        if action:
            out.add(action)
    for t in range(max(0, min(n_try, 1 << 12))):
        entry = image.read(try_map + 20 * t, 20)
        if entry is None:
            break
        n_catches, handlers = struct.unpack('<iI', entry[12:20])
        for c in range(max(0, min(n_catches, 1 << 12))):
            handler = image.u32(handlers + 16 * c + 12)
            if handler:
                out.add(handler)
    return out


def build_funclet_map(exe_bytes, functions):
    """funclet -> parent for every funclet the exe's EH tables tie to exactly one function.

    Each function with C++ EH installs a handler stub, `mov eax, FuncInfo; jmp <frame handler>`,
    either with `push stub` in its prologue or with `mov eax, stub` before calling the EH prologue
    helper. The FuncInfo lists the function's unwind funclets and catch handlers. A funclet is
    mapped when its stub is referenced from exactly one ordinary function and no other FuncInfo
    claims it for a different function. Returns (mapping, stats).
    """
    image = PEImage(exe_bytes)
    text_va, text = image.section('.text')
    ordinary = sorted((f.address, f.size) for f in functions if f.kind == 'function')
    starts = [a for a, _ in ordinary]
    funclets = {f.address for f in functions if f.kind == 'eh-funclet'}

    def containing(va):
        i = bisect.bisect_right(starts, va) - 1
        return starts[i] if i >= 0 and va < starts[i] + max(ordinary[i][1], 1) else None

    stubs = {}
    for m in re.finditer(rb'(?=\xb8(....)\xe9)', text, re.S):
        funcinfo = struct.unpack('<I', m.group(1))[0]
        if image.u32(funcinfo) == EH_MAGIC:
            stubs[text_va + m.start()] = funcinfo
    referers = {}
    for m in re.finditer(rb'(?=[\x68\xb8](....))', text, re.S):
        target = struct.unpack('<I', m.group(1))[0]
        site = text_va + m.start()
        if target in stubs and site != target:
            parent = containing(site)
            if parent is not None:
                referers.setdefault(target, set()).add(parent)

    claims = {}
    for stub, funcinfo in stubs.items():
        parents = referers.get(stub, set())
        if len(parents) != 1:
            continue
        parent = next(iter(parents))
        for target in _funcinfo_targets(image, funcinfo) & funclets:
            claims.setdefault(target, set()).add(parent)
    mapping = {f: next(iter(p)) for f, p in claims.items() if len(p) == 1}
    stats = {'handler_stubs': len(stubs),
             'stubs_with_one_parent': sum(1 for s in stubs if len(referers.get(s, ())) == 1),
             'funclets': len(funclets), 'mapped': len(mapping),
             'conflicting': sum(1 for p in claims.values() if len(p) > 1)}
    return mapping, stats


def write_funclet_map(path, mapping, exe_sha256):
    path.parent.mkdir(parents=True, exist_ok=True)
    lines = ['# EH funclet -> parent function, from the C++ EH tables of the original exe',
             '# exe sha256 %s; rebuild with: python3 scripts/progress.py --build-funclet-map EXE'
             % exe_sha256,
             'funclet\tparent']
    lines += ['%s\t%s' % (fmt_address(f), fmt_address(p)) for f, p in sorted(mapping.items())]
    path.write_text('\n'.join(lines) + '\n')


# --- Command line ----------------------------------------------------------------------------------

def default_paths():
    work = _work_dir()
    return {
        'work_dir': work,
        'functions_csv': ROOT / 'ghidra/exports/functions.csv',
        'sdk_exact_tsv': ROOT / 'analysis/results/StarFleetOP_2552_sdklib_matches.tsv',
        'sdk_fuzzy_tsv': ROOT / 'analysis/results/StarFleetOP_2552_sdklib_fuzzy.tsv',
        'eh_funclet_parents': ROOT / 'analysis/results/progress/op/eh_funclet_parents.tsv',
        'lifter_dir': None,
        'matched_set': ROOT / 'harness/matched_set.json',
        'functional_set': ROOT / 'harness/functional_set.json',
        'manifest': ROOT / 'op/manifest.tsv',
        'objdiff_reports': sorted(pathlib.Path(p) for p in
                                  glob.glob(str(ROOT / 'harness/objdiff/**/*.json'), recursive=True)),
    }


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0],
                                 epilog='The gate compares against the base branch copy, e.g. '
                                        '`git show origin/develop:analysis/results/progress/op/'
                                        'progress.json > base.json`.')
    d = default_paths()
    ap.add_argument('--functions', type=pathlib.Path, default=d['functions_csv'])
    ap.add_argument('--sdk-exact', type=pathlib.Path, default=d['sdk_exact_tsv'])
    ap.add_argument('--sdk-fuzzy', type=pathlib.Path, default=d['sdk_fuzzy_tsv'])
    ap.add_argument('--funclet-parents', type=pathlib.Path, default=d['eh_funclet_parents'])
    ap.add_argument('--lifter', type=pathlib.Path, default=d['lifter_dir'],
                    help="the port's lifter work directory with cat/ and out/; without it no code counts "
                         'as lifted, and what is not exact or functional is original x86')
    ap.add_argument('--matched-set', type=pathlib.Path, default=d['matched_set'])
    ap.add_argument('--functional-set', type=pathlib.Path, default=d['functional_set'])
    ap.add_argument('--manifest', type=pathlib.Path, default=d['manifest'],
                    help='claims whose modes include `partial` count as partial')
    ap.add_argument('--objdiff', type=pathlib.Path, action='append',
                    help='objdiff report JSON (repeatable; default harness/objdiff/**/*.json)')
    ap.add_argument('--text-bytes', type=int, default=TEXT_BYTES)
    ap.add_argument('--out', type=pathlib.Path,
                    default=ROOT / 'analysis/results/progress/op/progress.json')
    ap.add_argument('--markdown', action='store_true', help='also write PROGRESS.md')
    ap.add_argument('--markdown-out', type=pathlib.Path,
                    default=ROOT / 'analysis/results/progress/op/PROGRESS.md')
    ap.add_argument('--gate', type=pathlib.Path, metavar='BASE_JSON',
                    help="compare with the base branch's progress.json (not the working tree's "
                         'own, which this run would regenerate); writes nothing')
    ap.add_argument('--build-funclet-map', type=pathlib.Path, metavar='EXE',
                    help='rebuild the funclet-to-parent table from the original exe, then stop')
    ap.add_argument('--now', help='timestamp to record (ISO 8601); default the current UTC time')
    args = ap.parse_args(argv)

    if args.build_funclet_map:
        log = InputLog(None)
        functions = load_functions(args.functions, log)
        if not functions:
            print('progress: %s is missing or empty' % args.functions, file=sys.stderr)
            return 2
        exe_bytes = args.build_funclet_map.read_bytes()
        mapping, stats = build_funclet_map(exe_bytes, functions)
        write_funclet_map(args.funclet_parents, mapping, hashlib.sha256(exe_bytes).hexdigest())
        print('funclet map: %s' % ', '.join('%s %s' % (k, format(v, ',')) for k, v in stats.items()))
        return 0

    previous = None
    if args.gate:
        try:
            with open(args.gate) as f:
                previous = json.load(f)
        except (OSError, ValueError) as e:
            print('progress: cannot read %s: %s' % (args.gate, e), file=sys.stderr)
            return 2

    now = args.now or datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')
    paths = dict(d, functions_csv=args.functions, sdk_exact_tsv=args.sdk_exact,
                 sdk_fuzzy_tsv=args.sdk_fuzzy, eh_funclet_parents=args.funclet_parents,
                 lifter_dir=args.lifter, matched_set=args.matched_set,
                 functional_set=args.functional_set, manifest=args.manifest,
                 objdiff_reports=args.objdiff if args.objdiff is not None else d['objdiff_reports'])
    report = compute(paths, now, args.text_bytes)

    print(summary_line(report))
    print()
    for header, rows in table_rows(report):
        print(text_table(header, rows))
        print()
    missing = [k for k, v in report['inputs'].items() if not v['present']]
    if missing:
        print('inputs absent (counted as zero): ' + ', '.join(missing))

    if previous is not None:
        try:
            regressions = gate(previous, report)
        except (ValueError, AttributeError) as e:
            print('progress: %s is not a valid progress.json: %s' % (args.gate, e), file=sys.stderr)
            return 2
        for address, old, new, old_name, new_name in regressions:
            print('REGRESSION %s: %s -> %s (was %s, now %s)'
                  % (address, old, new, old_name or '?', new_name or '?'))
        if regressions:
            print('gate: %d function(s) regressed' % len(regressions))
            return 1
        print('gate: ok, no function lost exact or functional status')
        return 0

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=1) + '\n')
    if args.markdown:
        old = args.markdown_out.read_text() if args.markdown_out.is_file() else ''
        args.markdown_out.parent.mkdir(parents=True, exist_ok=True)
        args.markdown_out.write_text(render_markdown(report, old))
    return 0


if __name__ == '__main__':
    sys.exit(main())
