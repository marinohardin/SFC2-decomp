#!/usr/bin/env python3
"""List the files of a game install that are visibly damaged: empty files, and BMP images
shorter than the size their own header gives.

    python3 tools/install_scan.py DIR [--under SUBDIR]

Prints one tab-separated row per damaged file (path relative to DIR, what is wrong, its size,
the size its header gives or "-"), sorted by path, then a summary line. Exits 0 when nothing
is damaged, 1 when something is, 2 on bad usage. Reads only the first 14 bytes of each BMP.
"""
import os
import struct
import sys


def scan(root, under=''):
    """Returns (number of BMP files read, [(relpath, problem, size, header_size)])."""
    damaged = []
    bmps = 0
    for d, dirs, files in os.walk(os.path.join(root, under)):
        dirs.sort()
        for name in sorted(files):
            path = os.path.join(d, name)
            rel = os.path.relpath(path, root).replace(os.sep, '/')
            size = os.path.getsize(path)
            if size == 0:
                damaged.append((rel, 'empty', 0, '-'))
                continue
            if not name.lower().endswith('.bmp'):
                continue
            bmps += 1
            with open(path, 'rb') as f:
                head = f.read(14)
            if len(head) < 14 or head[:2] != b'BM':
                damaged.append((rel, 'not a BMP', size, '-'))
                continue
            declared = struct.unpack('<I', head[2:6])[0]
            if size < declared:
                damaged.append((rel, 'truncated', size, declared))
    return bmps, sorted(damaged)


def main(argv):
    args = argv[1:]
    under = ''
    if len(args) == 3 and args[1] == '--under':
        under = args[2]
        args = args[:1]
    if len(args) != 1 or args[0].startswith('-'):
        print(__doc__.strip().splitlines()[2].strip(), file=sys.stderr)
        return 2
    root = args[0]
    if not os.path.isdir(os.path.join(root, under)):
        print('install_scan: no directory %s' % os.path.join(root, under), file=sys.stderr)
        return 2
    bmps, damaged = scan(root, under)
    for row in damaged:
        print('%s\t%s\t%s\t%s' % row)
    empty = sum(1 for r in damaged if r[1] == 'empty')
    print('install_scan: %d damaged (%d empty, %d BMP truncated or not a BMP) of %d BMP files read'
          % (len(damaged), empty, len(damaged) - empty, bmps))
    return 1 if damaged else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
