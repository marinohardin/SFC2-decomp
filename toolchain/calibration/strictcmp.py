#!/usr/bin/env python3
"""Independent check of objdiff's verdicts: raw COFF section comparison of two objects.

Sections are paired by section name plus the first symbol defined in them (the COMDAT symbol),
then compared byte for byte, with relocations compared as (offset, type, target symbol name).
Compiler-internal labels ($L, $T, $SG, $E + number) come from per-compilation counters, so our
label numbers are shifted by the most common offset for that prefix (found by pairing labels in
order of appearance) before comparing. The offsets are reported: a nonzero one hints that the
preprocessed input differs.

    strictcmp.py target.obj ours.obj   -> JSON on stdout
"""
import collections, json, re, struct, sys

LABEL = re.compile(r'^(\$L|\$T|\$SG|_\$E|\$E)(\d+)$')


def parse(path, shift=None):
    b = open(path, 'rb').read()
    _, nsec, _, symoff, nsym, _, _ = struct.unpack_from('<HHIIIHH', b, 0)
    strtab = symoff + nsym * 18

    def name(raw):
        if raw[:4] == b'\0\0\0\0':
            o = struct.unpack_from('<I', raw, 4)[0]
            return b[strtab + o:b.index(b'\0', strtab + o)].decode('latin1')
        return raw.rstrip(b'\0').decode('latin1')

    syms, i = [], 0
    while i < nsym:
        raw = b[symoff + i * 18:symoff + i * 18 + 18]
        _, val, sec, _, scl, naux = struct.unpack('<8sIhHBB', raw)
        syms.append((name(raw[:8]), val, sec, scl))
        syms.extend([None] * naux)
        i += 1 + naux
    secs = []
    for k in range(nsec):
        h = b[20 + k * 40:60 + k * 40]
        n, _, _, rsz, rptr, relptr, _, nrel, _, ch = struct.unpack('<8sIIIIIIHHI', h)
        relocs = [struct.unpack_from('<IIH', b, relptr + r * 10) for r in range(nrel)]
        secs.append(dict(name=name(n), data=b[rptr:rptr + rsz] if rptr else b'', ch=ch, relocs=relocs))
    # Numbered labels in order of first appearance, per prefix; shift ours by the given offsets.
    labels, seen_l = collections.defaultdict(list), set()
    for s in syms:
        m = s and LABEL.match(s[0])
        if m and s[0] not in seen_l:
            seen_l.add(s[0])
            labels[m.group(1)].append(int(m.group(2)))

    def norm(n):
        m = LABEL.match(n)
        if not m or not shift:
            return n
        return m.group(1) + str(int(m.group(2)) - shift.get(m.group(1), 0))
    first = {}
    for s in syms:
        if s and s[2] > 0 and s[0] != secs[s[2] - 1]['name'] and s[2] not in first:
            first[s[2]] = norm(s[0])
    keyed, seen = {}, collections.Counter()
    for i, s in enumerate(secs, 1):
        k = (s['name'], first.get(i))
        seen[k] += 1
        s['relocs'] = [(o, t, norm(syms[idx][0])) for o, idx, t in s['relocs']]
        keyed[k + (seen[k],)] = s
    keyed['symtab'] = [(norm(x[0]), x[1], x[2], x[3]) for x in syms if x]
    return keyed, labels


def compare(target, ours):
    A, la = parse(target)
    _, lb = parse(ours)
    shift = {}
    for prefix in set(la) & set(lb):
        offs = collections.Counter(b - a for a, b in zip(la[prefix], lb[prefix]))
        shift[prefix] = offs.most_common(1)[0][0]
    B, _ = parse(ours, shift)
    c = collections.Counter()
    diffs = []
    symtab_a, symtab_b = A.pop('symtab'), B.pop('symtab')
    c['symtab_identical_modulo_labels'] = int(symtab_a == symtab_b)
    for k, sa in A.items():
        if sa['name'] in ('.drectve', '.debug$S', '.debug$T', '.debug$F'):
            continue
        kind = 'code' if sa['ch'] & 0x20 else 'data'
        c[kind + '_sections'] += 1
        sb = B.get(k)
        if sb is None:
            c[kind + '_unpaired'] += 1
            diffs.append(dict(key=list(map(str, k)), why='unpaired'))
        elif sa['data'] != sb['data']:
            c[kind + '_bytes_differ'] += 1
            diffs.append(dict(key=list(map(str, k)), why='bytes'))
        elif sa['relocs'] != sb['relocs']:
            c[kind + '_relocs_differ'] += 1
            diffs.append(dict(key=list(map(str, k)), why='relocs'))
        else:
            c[kind + '_identical'] += 1
    ta, tb = open(target, 'rb').read(), open(ours, 'rb').read()
    # Whole file identical apart from the COFF header TimeDateStamp (bytes 4..8).
    c['file_identical_except_timestamp'] = int(ta[:4] + ta[8:] == tb[:4] + tb[8:])
    return dict(counts=dict(c), label_offset={p: o for p, o in sorted(shift.items())},
                label_count=(sum(map(len, la.values())), sum(map(len, lb.values()))),
                diffs=diffs[:50])


if __name__ == '__main__':
    print(json.dumps(compare(sys.argv[1], sys.argv[2]), indent=1))
