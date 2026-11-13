#!/usr/bin/env python3
"""Name the game's own copies of library functions in functions.csv (step 5c).

Some library functions exist twice in the exe: the library's copy, which the analysis names from
the SDK libraries, and a second copy compiled into the game's own code with the game's options,
which the analysis cannot name because its bytes differ. The exe's own code calls the second
copy by the same name, so a relocation check that knows only the library address reports a call
to the game copy as a wrong target.

analysis/results/game_copy_names.tsv lists each such copy with the evidence for its name: its
address, mangled name, readable name, and why. This script writes the mangled and readable name
into the copy's functions.csv row, with name_source `game-copy`. A row that already has a
mangled name is left alone and reported, so the table never overrides the analysis.

Usage: name_game_copies.py <exports dir> <game_copy_names.tsv>. Rewrites functions.csv in place
and prints what it named; running it again changes nothing.
"""
import csv
import os
import sys

csv.field_size_limit(1 << 24)
SOURCE = 'game-copy'


def load_table(path):
    with open(path, newline='') as f:
        rows = list(csv.DictReader(f, delimiter='\t'))
    out = {}
    for n, r in enumerate(rows, 2):
        a = r['address'].strip().lower()
        if not a.startswith('0x') or not r['mangled'].strip() or not r['evidence'].strip():
            raise ValueError(f'{path}:{n}: a row needs a 0x address, a mangled name and evidence')
        out[int(a, 16)] = (r['mangled'].strip(), r['name'].strip())
    return out


def apply(rows, table):
    """Name the rows `table` lists; return (named, skipped) address lists."""
    named, skipped = [], []
    by_address = {int(r['address'], 16): r for r in rows}
    for a, (mangled, name) in sorted(table.items()):
        r = by_address.get(a)
        if r is None:
            raise ValueError(f'0x{a:08x} is not a function in functions.csv')
        if r['mangled'] == mangled and r['name_source'] == SOURCE:
            continue
        if r['mangled']:
            skipped.append(a)
            continue
        r['mangled'], r['name'], r['name_source'] = mangled, name or r['name'], SOURCE
        named.append(a)
    return named, skipped


def main(argv):
    if len(argv) != 3:
        print(__doc__.split('Usage: ')[1].split('\n')[0], file=sys.stderr)
        return 2
    path = os.path.join(argv[1], 'functions.csv')
    with open(path, newline='') as f:
        reader = csv.DictReader(f)
        fields, rows = reader.fieldnames, list(reader)
    named, skipped = apply(rows, load_table(argv[2]))
    if named:
        with open(path, 'w', newline='') as f:
            w = csv.DictWriter(f, fieldnames=fields, lineterminator='\n')
            w.writeheader()
            w.writerows(rows)
    for a in named:
        print(f'named 0x{a:08x}')
    for a in skipped:
        print(f'left 0x{a:08x}: the analysis already names it')
    print(f'name_game_copies: {len(named)} named, {len(skipped)} left as the analysis names them')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
