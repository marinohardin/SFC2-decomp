"""Which unit compiles a claim that sits in a header.

An annotation in a unit (`.cpp`, `.c`) is compiled in that unit. An annotation in a header (`.h`,
`.inl`) claims code the original wrote in the header and compiled in some unit that includes it:
an inline destructor's deleting destructor, a part binding inside a class body. The header is not
a unit, so the claim is compiled in the unit `op/UNITS.tsv` assigns to its address (the row
whose range holds the address), and that unit must include the header, directly or through
other headers. tools/manifest.py, the harness (harness/lib/verify.py) and the object lift
(tools/objlift/objlift.py) all resolve a header claim here, so they cannot disagree. This module
imports nothing from the harness, so tools/manifest.py can use it alone.
"""
import csv
import os
import re

HEADER_EXTS = ('.h', '.inl')
INCLUDE = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]')


class ClaimError(Exception):
    pass


def is_header(path):
    return path.lower().endswith(HEADER_EXTS)


def read_unit_map(path):
    """op/UNITS.tsv as [(first, last, unit)], address ranges as ints."""
    with open(path, newline='') as f:
        return [(int(r['first'], 16), int(r['last'], 16), r['unit'])
                for r in csv.DictReader(f, delimiter='\t') if r.get('unit')]


def default_include_dirs(src_root):
    """The include path every compile profile in harness/units.json gives (`include_first`:
    the source root and its port/ directory)."""
    return [src_root, os.path.join(src_root, 'port')]


def _resolve(name, quoted, from_dir, include_dirs):
    for d in ([from_dir] if quoted else []) + list(include_dirs):
        p = os.path.join(d, name)
        if os.path.isfile(p):
            return os.path.realpath(p)
    return None


def includes(unit_path, header_path, include_dirs):
    """True when the file at unit_path includes header_path, directly or through other headers
    it can find on include_dirs (a quoted include is looked up beside its file first). Includes
    it cannot find (the compiler's own headers) are not followed."""
    want = os.path.realpath(header_path)
    seen, todo = set(), [os.path.realpath(unit_path)]
    while todo:
        p = todo.pop()
        if p in seen:
            continue
        seen.add(p)
        try:
            with open(p, encoding='latin1') as f:
                lines = f.read().splitlines()
        except OSError:
            continue
        for line in lines:
            m = INCLUDE.match(line)
            if not m:
                continue
            q = _resolve(m.group(2), m.group(1) == '"', os.path.dirname(p), include_dirs)
            if q == want:
                return True
            if q is not None:
                todo.append(q)
    return False


def header_unit(address, header_path, src_root, units_tsv=None, include_dirs=None):
    """The unit (an absolute path) that compiles the claim of `address` in the header at
    header_path: the op/UNITS.tsv row whose range holds the address. Raises ClaimError when no
    row holds it, the row's file does not exist, or that file does not include the header."""
    units_tsv = units_tsv or os.path.join(src_root, 'UNITS.tsv')
    shown = os.path.relpath(header_path, src_root).replace(os.sep, '/')
    try:
        rows = read_unit_map(units_tsv)
    except (OSError, KeyError, ValueError) as e:
        raise ClaimError(f'0x{address:08x} is claimed in the header {shown}, and the unit map '
                         f'{units_tsv} that says which unit compiles it cannot be read ({e})')
    hits = [u for lo, hi, u in rows if lo <= address <= hi]
    if len(hits) != 1:
        what = 'no row' if not hits else f'{len(hits)} rows ({", ".join(hits)})'
        raise ClaimError(f'0x{address:08x} is claimed in the header {shown}, but {what} of '
                         'op/UNITS.tsv holds that address; a header claim is compiled in the '
                         'unit whose range holds it')
    unit = os.path.join(src_root, hits[0])
    if not os.path.isfile(unit):
        raise ClaimError(f'0x{address:08x} is claimed in the header {shown}; op/UNITS.tsv '
                         f'assigns it to {hits[0]}, which does not exist')
    dirs = default_include_dirs(src_root) if include_dirs is None else include_dirs
    if not includes(unit, header_path, dirs):
        raise ClaimError(f'0x{address:08x} is claimed in the header {shown}; op/UNITS.tsv '
                         f'assigns it to {hits[0]}, which does not include that header')
    return unit
