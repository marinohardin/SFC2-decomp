#!/usr/bin/env python3
"""Join the functions the analysis splits after their exception-frame prolog (step 5b).

VC6 opens a function that has an exception frame with `mov eax, <unwind table>; call
__EH_prolog`, 10 bytes, and the body follows directly. Ghidra ends the function at that call and
starts a second one at the next byte, so functions.csv lists the entry point as a 10-byte
function and the body as another function at entry + 10. Neither half is something the compiler
emits on its own, so neither can be rebuilt; this script makes the two rows one.

A row is joined when it is a 10-byte `function` whose bytes are `b8 <imm32> e8 <rel32>`, the
call's target holds __EH_prolog's bytes, and functions.csv has a `function` row at entry + 10.
The joined row keeps the entry's address and has the size of both. Its name is the entry's when
that name comes from class recovery (vtables and constructors point at the entry); otherwise the
body's when the body is named; otherwise the entry's. Its library flag, translation unit and
headers are the body's, since the body is the code. Its tags gain `eh-prolog-joined`.

strings_to_functions.csv rows that name a body address are moved to the joined function, and
vtables.csv's function names follow the joined names. unnamed_regions.csv is rebuilt from the
joined rows by SfcExport.java's rule (runs of `function` rows with a default name; an
`eh-funclet` row neither extends nor ends a run). functions.csv has sizes, not bodies, so a
region keeps its old end when it still ends on the same function; classes.csv holds no function
address.

Usage: join_eh_prologs.py <exports dir> <StarFleetOP.exe>. Rewrites the CSVs in place and prints
how many functions it joined; running it again joins nothing.
"""
import bisect
import csv
import struct
import sys

csv.field_size_limit(1 << 24)

# VC6's __EH_prolog: push -1; push eax; mov eax, fs:[0]; push eax; mov eax, [esp+0xc];
# mov fs:[0], esp; mov [esp+0xc], ebp; lea ebp, [esp+0xc]; push eax; ret
EH_PROLOG = bytes.fromhex('6aff5064a100000000508b44240c64892500000000896c240c8d6c240c50c3')
PROLOG_SIZE = 10
TAG = 'eh-prolog-joined'
CLASS_RECOVERY = 'rtti-class-recovery'


def pe_reader(image):
    """A function read(va, n) over a PE image's sections."""
    pe = struct.unpack_from('<I', image, 0x3c)[0]
    nsec = struct.unpack_from('<H', image, pe + 6)[0]
    opt = struct.unpack_from('<H', image, pe + 20)[0]
    base = struct.unpack_from('<I', image, pe + 24 + 28)[0]
    secs = []
    for i in range(nsec):
        off = pe + 24 + opt + 40 * i
        vsize, va, rsize, raw = struct.unpack_from('<IIII', image, off + 8)
        secs.append((base + va, max(vsize, rsize), raw, rsize))

    def read(va, n):
        for start, size, raw, rsize in secs:
            if start <= va and va + n <= start + size:
                o = va - start
                return image[raw + o:raw + min(o + n, rsize)].ljust(n, b'\0')
        return b''
    return read


def prolog_target(read, address):
    """The call target when the 10 bytes at address are `mov eax, imm32; call rel32`, else None."""
    b = read(address, PROLOG_SIZE)
    if len(b) != PROLOG_SIZE or b[0] != 0xB8 or b[5] != 0xE8:
        return None
    return (address + PROLOG_SIZE + struct.unpack_from('<i', b, 6)[0]) & 0xFFFFFFFF


def _named(row):
    return row['name'] and not row['name'].startswith('FUN_')


def joined_row(entry, body):
    """The one row for an entry and its body."""
    if entry['name_source'] == CLASS_RECOVERY or not _named(body):
        named = entry
    else:
        named = body
    row = dict(entry)
    for k in ('name', 'mangled', 'demangled', 'name_source'):
        row[k] = named[k]
    if not _named(row):
        row['name'] = entry['name']
    row['size'] = str(int(entry['size']) + int(body['size']))
    for k in ('is_library', 'library', 'translation_unit', 'tu_evidence', 'headers'):
        row[k] = body[k]
    tags = [t for t in (entry.get('tags') or '').split(';') if t]
    row['tags'] = ';'.join(tags + [TAG])
    return row


def join(rows, read):
    """The function rows with every split prolog joined to its body, and the map body address ->
    entry address of what was joined."""
    by = {int(r['address'], 16): r for r in rows}
    is_helper = {}
    moved = {}
    for a in sorted(by):
        r = by[a]
        if r['kind'] != 'function' or int(r['size'] or 0) != PROLOG_SIZE:
            continue
        if a in moved:      # already the body of the prolog before it: never also an entry
            continue
        body = by.get(a + PROLOG_SIZE)
        if body is None or body['kind'] != 'function':
            continue
        t = prolog_target(read, a)
        if t is None:
            continue
        if t not in is_helper:
            is_helper[t] = read(t, len(EH_PROLOG)) == EH_PROLOG
        if is_helper[t]:
            moved[a + PROLOG_SIZE] = a
    out = []
    for r in rows:
        a = int(r['address'], 16)
        if a in moved:
            continue
        if a + PROLOG_SIZE in moved and moved[a + PROLOG_SIZE] == a:
            r = joined_row(r, by[a + PROLOG_SIZE])
        out.append(r)
    return out, moved


def _runs(rows):
    """Runs of unnamed functions: [start, last function, functions, bytes], in address order."""
    out, cur = [], None
    for r in sorted(rows, key=lambda r: int(r['address'], 16)):
        a = int(r['address'], 16)
        if r['name_source'] == 'default' and r['kind'] == 'function':
            if cur is None:
                cur = [a, a, 0, 0]
            cur[1] = a
            cur[2] += 1
            cur[3] += int(r['size'])
        elif r['kind'] != 'eh-funclet' and cur is not None:
            out.append(cur)
            cur = None
    if cur is not None:
        out.append(cur)
    return out


def unnamed_regions(old_rows, new_rows, old_regions):
    """unnamed_regions.csv rows for new_rows, largest first, as SfcExport.java writes them. A
    region's end is the old one when it ends on the same function as an old region; otherwise
    its last function's address + size - 1."""
    sizes = {int(r['address'], 16): int(r['size']) for r in new_rows}
    old_end = {}
    by_start = {int(r['start'], 16): int(r['end'], 16) for r in old_regions}
    for start, last, _, _ in _runs(old_rows):
        if start in by_start:
            old_end[last] = by_start[start]
    named = sorted((int(r['address'], 16), r['name']) for r in new_rows
                   if r['name_source'] != 'default' and not r['name'].startswith(('Unwind@', 'Catch@')))
    keys = [k for k, _ in named]

    def neighbour(off, forward):
        i = bisect.bisect_left(keys, off) if forward else bisect.bisect_right(keys, off) - 1
        return f'{named[i][0]:08x} {named[i][1]}' if 0 <= i < len(named) else ''
    out = []
    for start, last, count, size in sorted(_runs(new_rows), key=lambda x: -x[3]):
        end = old_end.get(last, last + sizes[last] - 1)
        out.append(dict(start=f'{start:08x}', end=f'{end:08x}', functions=str(count),
                        bytes=str(size), named_before=neighbour(start, False),
                        named_after=neighbour(end, True)))
    return out


def _rewrite(path, fn):
    with open(path, newline='') as f:
        reader = csv.DictReader(f)
        fields, rows = reader.fieldnames, list(reader)
    rows = fn(rows)
    with open(path, 'w', newline='') as f:
        w = csv.DictWriter(f, fieldnames=fields, lineterminator='\n')
        w.writeheader()
        w.writerows(rows)


def main(argv):
    exports, exe = argv[1], argv[2]
    read = pe_reader(open(exe, 'rb').read())
    result = {}

    def functions(rows):
        result['old'] = rows
        out, moved = join(rows, read)
        result['moved'] = moved
        result['names'] = {int(r['address'], 16): r['name'] for r in out}
        return out
    _rewrite(f'{exports}/functions.csv', functions)
    moved, names = result['moved'], result['names']

    def strings(rows):
        for r in rows:
            a = int(r['function_address'], 16)
            if a in moved:
                r['function_address'] = f'{moved[a]:08x}'
                r['function'] = names[moved[a]]
        return rows
    _rewrite(f'{exports}/strings_to_functions.csv', strings)

    def vtables(rows):
        for r in rows:
            a = int(r['function_address'] or '0', 16)
            if r['function_address'] and a in names:
                r['function'] = names[a]
        return rows
    _rewrite(f'{exports}/vtables.csv', vtables)
    if moved:
        with open(f'{exports}/functions.csv', newline='') as f:
            new_rows = list(csv.DictReader(f))
        _rewrite(f'{exports}/unnamed_regions.csv',
                 lambda old_regions: unnamed_regions(result['old'], new_rows, old_regions))
    print(f'joined {len(moved)} functions split after their exception-frame prolog')


if __name__ == '__main__':
    main(sys.argv)
