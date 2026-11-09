#!/usr/bin/env python3
"""Evidence for the game's own compile flags (game_flags.md).

  python3 toolchain/calibration/game_flags.py scan
      Code-generation idioms and function alignment in the exe, below and above GAME_END.
  python3 toolchain/calibration/game_flags.py probe [--out DIR]
      Compile game_flags/probe.cpp under the game-release and sdk-release profiles of
      harness/units.json and compare each probe function with the exe at its address,
      relocation fields masked. Exit 1 unless game-release reproduces every probe and
      sdk-release none; exit 2 if a probe does not build. Needs the toolchain container.

Both read the exe through the harness's paths (SFCOP_REFERENCE or HARNESS_EXE to override).
"""
import argparse
import csv
import json
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(REPO, 'harness'))
sys.path.insert(0, os.path.join(REPO, 'tools'))
from lib import coff, paths  # noqa: E402
import units  # noqa: E402

PROBE = os.path.join(HERE, 'game_flags', 'probe.cpp')
IMAGE_START = 0x00401000
LIBRARY_END = 0x00730000   # the runtime libraries follow; they are neither side's evidence


def idioms(ins):
    """Counts of the size-optimiser idioms in one function's instructions, and of the speed
    form of a zero store."""
    c = dict(fldz_store=0, push_pop=0, and_zero=0, or_minus_one=0, mov_zero=0)
    for x, y in zip(ins, ins[1:] + [None]):
        op = x.op_str
        if x.mnemonic == 'fldz' and y is not None and y.mnemonic.startswith('fstp') and '[' in y.op_str:
            c['fldz_store'] += 1
        if x.mnemonic == 'push' and x.size == 2 and y is not None and y.mnemonic == 'pop':
            c['push_pop'] += 1
        if x.mnemonic == 'and' and op.startswith('dword ptr [') and op.endswith(', 0'):
            c['and_zero'] += 1
        if x.mnemonic == 'or' and op.startswith('dword ptr [') and op.endswith(', 0xffffffff'):
            c['or_minus_one'] += 1
        if x.mnemonic == 'mov' and op.startswith('dword ptr [') and op.endswith(', 0'):
            c['mov_zero'] += 1
    return c


def scan():
    import capstone
    exe = coff.PeImage(open(paths.EXE, 'rb').read())
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    sides = {'game (below GAME_END)': {}, 'libraries (GAME_END to 0x00730000)': {}}
    with open(paths.FUNCTIONS_CSV, newline='') as f:
        rows = [r for r in csv.DictReader(f) if r['kind'] == 'function' and r['size']]
    for r in rows:
        a, n = int(r['address'], 16), int(r['size'])
        if not IMAGE_START <= a < LIBRARY_END or n < 8:
            continue
        side = sides['game (below GAME_END)' if a < units.GAME_END
                     else 'libraries (GAME_END to 0x00730000)']
        side['functions'] = side.get('functions', 0) + 1
        side['bytes'] = side.get('bytes', 0) + n
        side['aligned'] = side.get('aligned', 0) + (a % 16 == 0)
        for k, v in idioms(list(md.disasm(exe.read(a, n), a))).items():
            side[k] = side.get(k, 0) + v
    keys = ['fldz_store', 'push_pop', 'and_zero', 'or_minus_one', 'mov_zero']
    print(f'GAME_END = 0x{units.GAME_END:08x}')
    print('| side | functions | KB | 16-byte aligned | ' + ' | '.join(f'{k} per KB' for k in keys) + ' |')
    print('|---|---|---|---|' + '---|' * len(keys))
    for name, s in sides.items():
        kb = s['bytes'] / 1024
        print(f'| {name} | {s["functions"]} | {kb:.0f} | {100 * s["aligned"] / s["functions"]:.0f}% | '
              + ' | '.join(f'{s[k] / kb:.2f}' for k in keys) + ' |')
    return 0


def _masked(data, offsets):
    b = bytearray(data)
    for o in offsets:
        b[o:o + 4] = b'\0\0\0\0'
    return bytes(b)


def probe(out):
    with open(paths.UNITS) as f:
        profiles = json.load(f)['profiles']
    with open(PROBE) as f:
        text = f.read()
    wanted = [int(a, 16) for a in re.findall(r'^// STARFLEETOP (0x[0-9a-f]{8})$', text, re.M)]
    exe = coff.PeImage(open(paths.EXE, 'rb').read())
    failures = 0
    print('| profile | flags | ' + ' | '.join(f'0x{a:08x}' for a in wanted) + ' |')
    print('|---|---|' + '---|' * len(wanted))
    for name in ('game-release', 'sdk-release'):
        d = os.path.join(out, name)
        os.makedirs(d, exist_ok=True)
        flags = profiles[name]['flags'].split()
        p = subprocess.run([paths.CL] + flags + ['/FAs', f'/Fa{d}/', f'/Fo{d}/', 'probe.cpp'],
                           cwd=os.path.dirname(PROBE), capture_output=True, text=True,
                           env=dict(os.environ, SFCOP_ROOT=paths.SFCOP_ROOT))
        if p.returncode != 0:
            print(p.stdout + p.stderr, file=sys.stderr)
            return 2
        obj = coff.CoffObject(open(os.path.join(d, 'probe.obj'), 'rb').read())
        cells = []
        for addr in wanted:   # each probe's name carries the address it reproduces
            syms = [n for n in obj.function_names() if f'{addr:08x}' in n]
            if len(syms) != 1:
                print(f'game_flags: {len(syms)} symbols in probe.obj name 0x{addr:08x}',
                      file=sys.stderr)
                return 2
            sec, off, size = obj.function(syms[0])
            ours = sec.data[off:off + size].rstrip(b'\xcc')
            fixups = [r[0] - off for r in sec.relocs if off <= r[0] < off + size]
            theirs = exe.read(addr, len(ours))
            same = _masked(ours, fixups) == _masked(theirs, fixups)
            cells.append('exact' if same else 'differs')
        # the claim is both halves: /O1 reproduces every probe and /O2 none of them
        failures += ('differs' in cells) if name == 'game-release' else ('exact' in cells)
        print(f'| {name} | `{" ".join(f for f in flags if f.startswith("/O"))}` | '
              + ' | '.join(cells) + ' |')
    return 1 if failures else 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    sub = ap.add_subparsers(dest='cmd', required=True)
    sub.add_parser('scan')
    pp = sub.add_parser('probe')
    pp.add_argument('--out', default=os.path.join(paths.WORK, 'calibration', 'game_flags'))
    args = ap.parse_args(argv)
    return scan() if args.cmd == 'scan' else probe(args.out)


if __name__ == '__main__':
    sys.exit(main())
