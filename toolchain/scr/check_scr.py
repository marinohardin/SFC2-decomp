#!/usr/bin/env python3
"""Check a built mission script against the 88 shipped Assets/Scripts/*.scr: the same exported
names, an export DLL name equal to its own file name, and the same PE header fields.

    check_scr.py <built.scr>
"""
import glob, os, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(HERE))
SHIPPED = os.path.join(REPO, 'reference/installed game/Starfleet Command Orion Pirates/Assets/Scripts')
sys.dont_write_bytecode = True
sys.path.insert(0, HERE)
import pe_exports  # noqa: E402


def header(path):
    b = open(path, 'rb').read()
    pe = struct.unpack_from('<I', b, 0x3c)[0]
    opt = pe + 24
    return {'machine': hex(struct.unpack_from('<H', b, pe + 4)[0]),
            'characteristics': hex(struct.unpack_from('<H', b, pe + 22)[0]),
            'linker': f'{b[opt + 2]}.{b[opt + 3]}',
            'image base': hex(struct.unpack_from('<I', b, opt + 28)[0]),
            'subsystem': struct.unpack_from('<H', b, opt + 68)[0]}


def main():
    built = sys.argv[1]
    shipped = sorted(glob.glob(os.path.join(SHIPPED, '**', '*.scr'), recursive=True))
    want_exports = {tuple(pe_exports.exports(p)[1]) for p in shipped}
    want_header = {tuple(sorted(header(p).items())) for p in shipped}
    dll, names = pe_exports.exports(built)
    print(f'{len(shipped)} shipped scripts; export sets {sorted(want_exports)}')
    print(f'built: export name {dll}, exports {names}, header {header(built)}')
    ok = True
    if tuple(names) not in want_exports:
        print('FAIL: exports differ from every shipped script'); ok = False
    if (dll or '').lower() != os.path.basename(built).lower():
        print('FAIL: export DLL name is not the file name, as it is in every shipped script'); ok = False
    if tuple(sorted(header(built).items())) not in want_header:
        print(f'FAIL: PE header fields match no shipped script; shipped: {sorted(want_header)}'); ok = False
    print('PASS' if ok else 'FAILED')
    sys.exit(0 if ok else 1)


if __name__ == '__main__':
    main()
