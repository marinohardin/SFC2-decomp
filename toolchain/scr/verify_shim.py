#!/usr/bin/env python3
"""Check that dmp3d_shim.obj (as compiled by build_scr.py) matches the copies of the same two
functions statically linked into the shipped Gen_Playback.scr, byte for byte over each COMDAT
section, with relocated fields masked.

    verify_shim.py <path/to/_dmp3d_shim.obj>
"""
import os, struct, sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SCR = os.path.join(REPO, 'reference/installed game/Starfleet Command Orion Pirates/Assets/Scripts/Gen_Playback.scr')
FUNCS = {'??0tErrInfo@@QAA@PBDH0ZZ': 0x1001d270, '?mIdentity@tMatrix@@QAEXXZ': 0x1001d150}
IMAGE_BASE, TEXT_RVA, TEXT_RAW = 0x10000000, 0x1000, 0x1000  # Gen_Playback.scr .text


def main():
    import hashlib
    rel = os.path.relpath(SCR, os.path.join(REPO, 'reference'))
    want = [l.split('  ', 1)[0] for l in open(os.path.join(REPO, 'analysis/results/retail_hashes.sha256'))
            if l.rstrip('\n').endswith('  ' + rel)]
    g = open(SCR, 'rb').read()
    if want and hashlib.sha256(g).hexdigest() != want[0]:
        sys.exit(f'hash mismatch for {rel}')
    o = open(sys.argv[1], 'rb').read()
    _, nsec, _, symoff, nsym, _, _ = struct.unpack_from('<HHIIIHH', o, 0)
    strtab = symoff + nsym * 18
    secs = [struct.unpack_from('<8sIIIIIIHHI', o, 20 + k * 40) for k in range(nsec)]
    found, i, ok = {}, 0, True
    while i < nsym:
        raw = o[symoff + i * 18:symoff + i * 18 + 18]
        name, val, sec, _, _, naux = struct.unpack('<8sIhHBB', raw)
        if raw[:4] == b'\0' * 4:
            off = struct.unpack_from('<I', raw, 4)[0]
            name = o[strtab + off:o.index(b'\0', strtab + off)]
        name = name.rstrip(b'\0').decode('latin1')
        if name in FUNCS and sec > 0:
            found[name] = (sec, val)
        i += 1 + naux
    for name, va in FUNCS.items():
        sec, val = found[name]
        s = secs[sec - 1]
        ours = bytearray(o[s[4] + val:s[4] + s[3]])
        start = va - IMAGE_BASE - TEXT_RVA + TEXT_RAW
        theirs = bytearray(g[start:start + len(ours)])
        for r in range(s[7]):
            roff = struct.unpack_from('<I', o, s[5] + r * 10)[0] - val
            ours[roff:roff + 4] = theirs[roff:roff + 4] = b'\0\0\0\0'
        same = ours == theirs
        ok &= same
        print(f'{name}: {len(ours)} bytes, {s[7]} relocations masked, '
              f'{"identical" if same else "DIFFERENT"} to Gen_Playback.scr at {va:#x}')
    sys.exit(0 if ok else 1)


if __name__ == '__main__':
    main()
