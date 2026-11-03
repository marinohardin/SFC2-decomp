#!/usr/bin/env python3
"""Print a PE file's export DLL name and exported symbol names, one per line after the name.

    pe_exports.py file.scr [file2.scr ...]
"""
import struct, sys


def exports(path):
    b = open(path, 'rb').read()
    pe = struct.unpack_from('<I', b, 0x3c)[0]
    nsec, optsz = struct.unpack_from('<H', b, pe + 6)[0], struct.unpack_from('<H', b, pe + 20)[0]
    opt = pe + 24
    erva = struct.unpack_from('<I', b, opt + 96)[0]
    secs = [struct.unpack_from('<8sIIII', b, opt + optsz + k * 40) for k in range(nsec)]

    def off(rva):
        for _, vsz, va, rsz, rptr in secs:
            if va <= rva < va + max(vsz, rsz):
                return rva - va + rptr
        raise ValueError(f'rva {rva:#x} outside sections')

    if not erva:
        return None, []
    e = off(erva)
    dll = b[off(struct.unpack_from('<I', b, e + 12)[0]):].split(b'\0')[0].decode('latin1')
    n, names = struct.unpack_from('<I', b, e + 24)[0], off(struct.unpack_from('<I', b, e + 32)[0])
    return dll, [b[off(struct.unpack_from('<I', b, names + 4 * k)[0]):].split(b'\0')[0].decode('latin1')
                 for k in range(n)]


if __name__ == '__main__':
    for p in sys.argv[1:]:
        dll, names = exports(p)
        print(f'{p}: {dll}')
        for n in names:
            print(f'  {n}')
