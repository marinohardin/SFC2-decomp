#!/usr/bin/env python3
"""The modernization's footprint inside the decompilation (DESIGN §7.1): every `PORT:` line and
every `SFCOP_FIX(` site under a tree, with file and line, and the two totals.

    python3 scripts/port_footprint.py [--no-fix-sites] game

A `PORT:` line is a comment tagged on the line itself, `// PORT: <reason>` (CONVENTIONS §4.1); a
fix site is each `SFCOP_FIX(` on a line, so `#if SFCOP_FIX(a) || SFCOP_FIX(b)` is two. Only C and
C++ sources and headers are read. The listing is informational: the exit status is 0 whatever
the counts, 2 when the tree does not exist. With `--no-fix-sites` a tree holding any fix site
exits 1 after the listing: `game/` carries no fix, a fix to game code is an override unit under
`src/fixes/overrides/` (DESIGN §6.9).
"""
import os
import re
import sys

SOURCE_SUFFIXES = ('.c', '.cc', '.cpp', '.cxx', '.h', '.hh', '.hpp', '.hxx', '.inl', '.mm')
PORT_RE = re.compile(r'(?://|/\*)\s*PORT:')
FIX_RE = re.compile(r'\bSFCOP_FIX\(')


def scan(root):
    """(port, fix): lists of (path, line number, line text), in path and line order. Paths are
    root joined with the file's path below it."""
    port, fix = [], []
    for dirpath, dirnames, filenames in os.walk(root):
        dirnames.sort()
        for name in sorted(filenames):
            if not name.endswith(SOURCE_SUFFIXES):
                continue
            path = os.path.join(dirpath, name)
            with open(path, encoding='utf-8', errors='replace') as f:
                for number, text in enumerate(f, 1):
                    text = text.rstrip('\n')
                    if PORT_RE.search(text):
                        port.append((path, number, text.strip()))
                    for _ in FIX_RE.finditer(text):
                        fix.append((path, number, text.strip()))
    return port, fix


def main(argv):
    args = argv[1:]
    forbid_fixes = '--no-fix-sites' in args
    args = [a for a in args if a != '--no-fix-sites']
    if len(args) != 1:
        print('usage: port_footprint.py [--no-fix-sites] ROOT', file=sys.stderr)
        return 2
    root = args[0]
    if not os.path.isdir(root):
        print('port_footprint: no directory %s' % root, file=sys.stderr)
        return 2
    port, fix = scan(root)
    for kind, sites in (('PORT', port), ('SFCOP_FIX', fix)):
        for path, number, text in sites:
            print('%s %s:%d: %s' % (kind, path, number, text))
    print('PORT lines: %d' % len(port))
    print('SFCOP_FIX sites: %d' % len(fix))
    if forbid_fixes and fix:
        print('port_footprint: %d SFCOP_FIX site(s) under %s; a fix to game code is an override '
              'unit under src/fixes/overrides/ (DESIGN §6.9)' % (len(fix), root), file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
