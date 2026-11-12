#!/usr/bin/env python3
"""Write the install manifest: one line per file of the import set of an installed 2.5.5.2
game, with its size and SHA-256 (src/app/Import.h reads it).

    python3 tools/install_manifest.py INSTALL [-o OUT] [--unchecked LIST]

The import set is what the game reads at run time: the executable, the settings and sound
setup files beside it (*.gf, *.ini, *.q3, SFC25.txt), and everything under Assets/ and
MetaAssets/. Left out: the SDK, the manuals, the installer, patcher, uninstaller and
registration programs and their files, the other executables and libraries, and every file or
folder whose name starts with '.'. Names are matched ignoring case, as the game does.

Rows are `path<TAB>size<TAB>sha256`, sorted by path; the file holds hashes only, never a byte
of the game. LIST names, one per line ('#' starts a comment), the files whose content is not
known to be intact (repaired, or suspect): their rows are `path<TAB><TAB>`, unchecked, meaning
the file must be there but its size and hash are not given. Writes to standard output without
-o. Exits 0, or 2 on bad usage, when INSTALL holds no StarFleetOP.exe, or when LIST names the
executable or a file that is not in the import set.
"""
import hashlib
import os
import sys

GAME_EXE = 'starfleetop.exe'
ROOT_SUFFIXES = ('.gf', '.ini', '.q3')
ROOT_NAMES = (GAME_EXE, 'sfc25.txt')
FOLDERS = ('assets', 'metaassets')

HEADER = (
    '# Sizes and SHA-256 of the files of an installed 2.5.5.2 game: the import set the client\n'
    '# copies (src/app/Import.h). Written by tools/install_manifest.py. Hashes only.\n'
    '# %d rows checked; %d rows unchecked (path only: the file must be there, content not given).\n'
    '# path\tsize\tsha256\n')


def in_import_set(rel):
    """Whether `rel` ('/'-separated, relative to the install) is copied by an import."""
    parts = rel.split('/')
    if any(p.startswith('.') for p in parts):
        return False
    name = parts[-1].lower()
    if len(parts) == 1:
        return name in ROOT_NAMES or name.endswith(ROOT_SUFFIXES)
    return parts[0].lower() in FOLDERS


def sha256_of(path):
    digest = hashlib.sha256()
    with open(path, 'rb') as f:
        for piece in iter(lambda: f.read(1 << 20), b''):
            digest.update(piece)
    return digest.hexdigest()


def manifest_rows(root, unchecked=()):
    """[(path, size, sha256)] of the import set under `root`, sorted by path; size and sha256
    are None for a path in `unchecked` (lowercase)."""
    rows = []
    for d, dirs, files in os.walk(root):
        dirs[:] = sorted(x for x in dirs if not x.startswith('.'))
        for name in sorted(files):
            path = os.path.join(d, name)
            rel = os.path.relpath(path, root).replace(os.sep, '/')
            if os.path.isfile(path) and not os.path.islink(path) and in_import_set(rel):
                if rel.lower() in unchecked:
                    rows.append((rel, None, None))
                else:
                    rows.append((rel, os.path.getsize(path), sha256_of(path)))
    return sorted(rows)


def render(rows):
    checked = sum(1 for r in rows if r[1] is not None)
    return (HEADER % (checked, len(rows) - checked) +
            ''.join('%s\t\t\n' % r[0] if r[1] is None else '%s\t%d\t%s\n' % r for r in rows))


def read_list(path):
    """The lowercase paths LIST names."""
    with open(path, encoding='utf-8') as f:
        names = (line.split('#', 1)[0].strip() for line in f)
        return {n.replace('\\', '/').lower() for n in names if n}


def main(argv):
    args = argv[1:]
    options = {}
    while len(args) >= 3 and args[-2] in ('-o', '--unchecked') and args[-2] not in options:
        options[args[-2]] = args[-1]
        args = args[:-2]
    out = options.get('-o')
    if len(args) != 1 or args[0].startswith('-'):
        print(__doc__.strip().splitlines()[3].strip(), file=sys.stderr)
        return 2
    root = args[0]
    if not os.path.isdir(root) or not any(n.lower() == GAME_EXE for n in os.listdir(root)):
        print('install_manifest: %s holds no StarFleetOP.exe' % root, file=sys.stderr)
        return 2
    unchecked = read_list(options['--unchecked']) if '--unchecked' in options else set()
    rows = manifest_rows(root, unchecked)
    stray = sorted(unchecked - {r[0].lower() for r in rows} | (unchecked & {GAME_EXE}))
    if stray:
        print('install_manifest: the unchecked list names files outside the import set, or the '
              'executable: %s' % ', '.join(stray), file=sys.stderr)
        return 2
    text = render(rows)
    if out is None:
        sys.stdout.write(text)
    else:
        with open(out, 'w', newline='\n') as f:
            f.write(text)
    unknown = sum(1 for r in rows if r[1] is None)
    print('install_manifest: %d files, %d bytes checked, %d unchecked'
          % (len(rows), sum(r[1] for r in rows if r[1] is not None), unknown), file=sys.stderr)
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
