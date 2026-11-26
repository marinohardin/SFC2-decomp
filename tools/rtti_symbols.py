#!/usr/bin/env python3
"""The data symbols of the executable's run-time type information: analysis/symbols/STARFLEETOP.data.tsv.

A polymorphic class's constructor or destructor makes the compiler emit a type descriptor
(`??_R0...@8`) and a vftable (`??_7...@@6B@`). The symbol table (STARFLEETOP.tsv) has function
rows only, so a program that places the symbols of objects built from op/ by evidence from the
executable finds no address for these. This tool finds them in the executable by structure:

- a type descriptor is the vftable pointer of `type_info`, a zero word, then the decorated name
  string (`.?AVtMetaMapSprite@@`); its symbol is `??_R0` + the name without its dot + `@8`;
- the word before a vftable points at its `??_R4` record (signature 0, offset, cdOffset, the type
  descriptor, the class hierarchy), which names the class and the offset of the subobject.

A class with one vftable gets `??_7<class>@@6B@`. A class with several gets `??_7<class>@@6B@` for
the vftable at offset 0 and `??_7<class>@@6B<base>@@@` for another only when exactly one base of
the class's `??_R2` array sits at that offset, without a virtual base; any other vftable is left
out and counted. `??_R1`, `??_R2`, `??_R3` and `??_R4` are not written.

    python3 tools/rtti_symbols.py generate [--exe EXE] [--out FILE]
    python3 tools/rtti_symbols.py check    [--exe EXE] [--file FILE]

`check` exits 1 when the committed file differs from what the executable gives (after the
executable's hash is checked against analysis/results/retail_hashes.sha256), and prints an INFO
line and exits 0 when the executable is absent.
"""
import argparse
import array
import collections
import hashlib
import os
import re
import struct
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, REPO)

from harness.lib import paths  # noqa: E402
from harness.lib.coff import CoffError, PeImage  # noqa: E402

COLUMNS = ('address', 'kind', 'symbol', 'evidence')
FILE_REL = 'analysis/symbols/STARFLEETOP.data.tsv'
COMMENT = ('# The run-time type information symbols of the original (Orion Pirates 2.5.5.2): one row per '
           'type descriptor (??_R0) and per vftable (??_7) whose decorated name is unambiguous.\n'
           '# Written by tools/rtti_symbols.py, found in the executable by structure; '
           'analysis/symbols/README.md describes it.\n')
NAME = re.compile(rb'[\x21-\x7e]{1,400}\0')
MAX_ADDRESS = 0x10000000


class RttiError(Exception):
    pass


def _words(data):
    """The 4-byte-aligned little-endian words of `data`."""
    arr = array.array('I', data[:len(data) // 4 * 4])
    if sys.byteorder == 'big':
        arr.byteswap()
    return arr


def scan(raw):
    """(rows, stats) of an executable image's bytes: rows are (address, kind, symbol) sorted by
    address; stats counts what was found and what was left out."""
    pe = PeImage(raw)
    base = pe.image_base
    sections = [(base + va, raw[roff:roff + rsize]) for _, va, vsize, roff, rsize in pe.sections if rsize]
    # type descriptors: vptr, zero, '.name'; the vptr of type_info is the commonest one
    cands = []
    for start, data in sections:
        pos = data.find(b'\0\0\0\0.')
        while pos != -1:
            if pos >= 4 and pos % 4 == 0:
                m = NAME.match(data, pos + 4)
                if m:
                    cands.append((struct.unpack_from('<I', data, pos - 4)[0],
                                  start + pos - 4, m.group()[:-1].decode('ascii')))
            pos = data.find(b'\0\0\0\0.', pos + 1)
    if not cands:
        return [], collections.Counter()
    typeinfo = collections.Counter(c[0] for c in cands).most_common(1)[0][0]
    descriptors = {a: name for vptr, a, name in cands if vptr == typeinfo}
    rows = [(a, 'type_descriptor', '??_R0' + name[1:] + '@8') for a, name in descriptors.items()]
    stats = collections.Counter(type_descriptors=len(rows))
    # ??_R4 records, then the vftables that follow the words pointing at them
    r4 = {}
    for start, data in sections:
        w = _words(data)
        for i in range(len(w) - 4):
            if w[i] == 0 and w[i + 3] in descriptors and w[i + 1] < 0x10000 and w[i + 2] < 0x10000 \
                    and base < w[i + 4] < base + MAX_ADDRESS:
                r4[start + 4 * i] = (w[i + 1], w[i + 3], w[i + 4])
    refs = collections.defaultdict(list)
    for start, data in sections:
        w = _words(data)
        for i, x in enumerate(w):
            if x in r4:
                refs[x].append(start + 4 * i + 4)
    by_class = collections.defaultdict(list)
    for a, (offset, td, _) in r4.items():
        if len(refs[a]) == 1:
            by_class[td].append((offset, refs[a][0], a))
        else:
            stats['left_out_not_one_vftable_reference'] += 1
    for td, vts in by_class.items():
        cls = descriptors[td][4:]
        offsets = collections.Counter(o for o, _, _ in vts)
        for offset, vtable, rec in vts:
            if offsets[offset] != 1:
                stats['left_out_two_vftables_at_one_offset'] += 1
            elif offset == 0:
                rows.append((vtable, 'vftable', f'??_7{cls}6B@'))
                stats['vftable_one_per_class' if len(vts) == 1 else 'vftable_offset_zero'] += 1
            else:
                base_name = _base_at(pe, r4[rec][2], offset, descriptors)
                if base_name is None:
                    stats['left_out_base_not_unique'] += 1
                else:
                    rows.append((vtable, 'vftable', f'??_7{cls}6B{base_name[4:]}@'))
                    stats['vftable_with_base'] += 1
    rows.sort()
    return rows, stats


def _base_at(pe, hierarchy, offset, descriptors):
    """The name string of the one base class of the `??_R3` record at `hierarchy` that sits at
    `offset` without a virtual base, or None when there is none or more than one."""
    try:
        _, _, count, array_at = struct.unpack('<IIII', pe.read(hierarchy, 16))
        found = []
        for i in range(1, min(count, 64)):
            r1 = struct.unpack('<I', pe.read(array_at + 4 * i, 4))[0]
            td, _, mdisp, pdisp, _, _, _ = struct.unpack('<IIiiiII', pe.read(r1, 28))
            if mdisp == offset and pdisp == -1:
                found.append(td)
            elif mdisp == offset:
                return None
        return descriptors.get(found[0]) if len(found) == 1 else None
    except (CoffError, struct.error):
        return None


def text(rows):
    return COMMENT + '\t'.join(COLUMNS) + '\n' + ''.join(
        f'0x{a:08x}\t{kind}\t{sym}\trtti\n' for a, kind, sym in rows)


def exe_hash_problem(exe, hashes=None, rel=None):
    """None when the executable's SHA-256 is the one retail_hashes.sha256 lists for it, else why not."""
    rel = rel or paths.EXE_REL
    with open(hashes or paths.HASHES, encoding='utf-8') as f:
        want = {line.split('  ', 1)[1].strip(): line.split('  ', 1)[0]
                for line in f if '  ' in line}.get(rel)
    if want is None:
        return f'{rel} is not in {os.path.basename(hashes or paths.HASHES)}'
    with open(exe, 'rb') as f:
        got = hashlib.sha256(f.read()).hexdigest()
    return None if got == want else f'{exe} is not the retail file (sha256 {got})'


def generate(exe, check_hash=True):
    if check_hash:
        problem = exe_hash_problem(exe)
        if problem:
            raise RttiError(problem)
    with open(exe, 'rb') as f:
        rows, stats = scan(f.read())
    return text(rows), stats


def summary(stats):
    return ', '.join(f'{k} {v}' for k, v in sorted(stats.items()))


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    sub = p.add_subparsers(dest='cmd', required=True)
    g = sub.add_parser('generate')
    g.add_argument('--out', default=os.path.join(REPO, FILE_REL))
    c = sub.add_parser('check')
    c.add_argument('--file', default=os.path.join(REPO, FILE_REL))
    for s in (g, c):
        s.add_argument('--exe', default=paths.EXE)
        s.add_argument('--no-hash', action='store_true', help='skip the retail hash check (tests)')
    a = p.parse_args(argv)
    if not os.path.isfile(a.exe):
        if a.cmd == 'check':
            print(f'INFO no executable at {a.exe}: the data symbols are not compared with it')
            return 0
        print(f'ERROR no executable at {a.exe}', file=sys.stderr)
        return 1
    try:
        new, stats = generate(a.exe, not a.no_hash)
    except (RttiError, CoffError, OSError) as e:
        print(f'ERROR {e}')
        return 1
    if a.cmd == 'generate':
        with open(a.out, 'w', encoding='utf-8', newline='') as f:
            f.write(new)
        print(f'wrote {new.count(chr(10)) - 3} rows to {a.out}: {summary(stats)}')
        return 0
    try:
        with open(a.file, encoding='utf-8', newline='') as f:
            old = f.read()
    except OSError as e:
        print(f'ERROR {a.file}: {e.strerror}')
        return 1
    if old != new:
        have, want = old.split('\n'), new.split('\n')
        n = next((i for i, (x, y) in enumerate(zip(have, want)) if x != y), min(len(have), len(want)))
        print(f'ERROR {a.file} differs from the executable at line {n + 1}: run '
              'python3 tools/rtti_symbols.py generate')
        return 1
    print(f'ok: {new.count(chr(10)) - 3} rows in {a.file} ({summary(stats)})')
    return 0


if __name__ == '__main__':
    sys.exit(main())
