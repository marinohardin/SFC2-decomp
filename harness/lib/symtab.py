"""The symbol table: for an original function's address, the symbol a call to it must name.

Evidence, most trusted first:

- claim         the symbol the claiming unit compiled for it at the last `build-and-verify.sh
                --all` in this checkout, which writes the claims' rows to <build>/symbols.tsv;
- table         the committed symbol table (analysis/symbols/STARFLEETOP.tsv), any row with a
                symbol, whatever its symbol_evidence (a claim row equals the build's when the
                table is in sync);
- functions.csv the mangled name in ghidra/exports/functions.csv;
- sdk           a single SDK library label at the address (the matcher's label tiers).

A function session calls a claimed function through the claim's own declaration: the pack lists
each callee's symbol, and the Stop hook refuses a session whose function reaches a claimed
address through any other symbol (a stand-in: a class or function declared again under another
name). The matched set's `symbol` field is not used here: it is written only when a score rises,
so a later rename leaves it stale.

    python3 -m harness.lib.symtab [<address>...]   # rows, as TSV, for these or every address
"""
import csv
import os
import re
import sys

from . import paths

FIELDS = ('address', 'symbol', 'file')
# MSVC: after the qualified name, a member function's access letter, the `this` qualifier and
# then the calling convention; a free function's Y and the convention.
_CONV = re.compile(r'@@(?:Y([ACEGI])|[CDKLST]([ACEGI])|[ABEFIJMNQRUV][A-D]([ACEGI]))')
CONVENTIONS = {'A': 'cdecl', 'C': 'pascal', 'E': 'thiscall', 'G': 'stdcall', 'I': 'fastcall'}


def path():
    return os.path.join(paths.BUILD, 'symbols.tsv')


def convention(symbol):
    """The calling convention a mangled MSVC function symbol encodes, or '' when it is not one."""
    if not symbol.startswith('?'):
        return ''
    m = _CONV.search(symbol)
    return CONVENTIONS[next(g for g in m.groups() if g)] if m else ''


def write_claims(out, rows):
    """rows: (address 0x..., symbol, file under the source root), one per claim."""
    with open(out, 'w', newline='') as f:
        w = csv.writer(f, delimiter='\t', lineterminator='\n')
        w.writerow(FIELDS)
        for row in sorted(set(rows)):
            w.writerow(row)


def load_claims(src=None):
    """{address 0x...: (symbol, file)} from the table, or {} when no --all has written one."""
    src = src or path()
    try:
        with open(src, newline='') as f:
            return {r['address']: (r['symbol'], r['file'])
                    for r in csv.DictReader(f, delimiter='\t') if r.get('symbol')}
    except OSError:
        return {}


class TableError(Exception):
    """The committed symbol table exists and cannot be read."""


def load_table(src=None):
    """{address 0x...: (symbol, symbol_evidence)} of the committed symbol table's rows that have a
    symbol (a `folded` row is skipped). Columns are found by their header names, so an added
    column does not move them. {} when there is no table; a table that cannot be read (a missing
    column, a row with another cell count) raises TableError."""
    src = src or paths.SYMBOL_TABLE
    try:
        with open(src, encoding='utf-8', newline='') as f:
            lines = f.read().split('\n')
    except FileNotFoundError:
        return {}
    except OSError as e:
        raise TableError(f'{src}: {e.strerror}') from None
    head = lines[0].split('\t')
    need = ('address', 'kind', 'symbol', 'symbol_evidence')
    if any(c not in head for c in need):
        raise TableError(f'{src}: the header lacks {", ".join(c for c in need if c not in head)}')
    at = {c: head.index(c) for c in need}
    out = {}
    for n, line in enumerate(lines[1:], 2):
        if not line:
            continue
        c = line.split('\t')
        if len(c) != len(head):
            raise TableError(f'{src}:{n}: {len(c)} cells, the header has {len(head)}')
        if c[at['symbol']] and c[at['kind']] != 'folded':
            out[c[at['address']]] = (c[at['symbol']], c[at['symbol_evidence']])
    return out


def tool():
    """tools/symbols.py as a module: the table's rules (which symbols may be written) live there."""
    import importlib.util
    spec = importlib.util.spec_from_file_location(
        'sfcop_symbols_tool', os.path.join(paths.REPO, 'tools', 'symbols.py'))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def lookup(address, index, claims, table=None):
    """(symbol, source, file) for the int `address`, or None when nothing names it. `table` is
    load_table's result; without it the committed table is not consulted."""
    key = f'0x{address:08x}'
    if key in claims:
        sym, where = claims[key]
        return sym, 'claim', where
    if table and key in table:
        return table[key][0], 'table', table[key][1]
    fn = index.by_address.get(address)
    if fn is not None and getattr(fn, 'mangled', ''):
        return fn.mangled, 'functions.csv', ''
    names = [n for n in getattr(index, 'names_at', {}).get(address, ()) if n.startswith('?')]
    if len(names) == 1:
        return names[0], 'sdk', ''
    return None


def main(argv):
    from . import funcs
    index = funcs.FunctionIndex(paths.FUNCTIONS_CSV, paths.SYMBOL_TSVS)
    claims = load_claims()
    rows = load_table()
    if not claims:
        print(f'note: no {path()} yet: run harness/build-and-verify.sh --all for the claims\' '
              'symbols', file=sys.stderr)
    wanted = [int(a, 16) for a in argv] or sorted(index.by_address)
    w = csv.writer(sys.stdout, delimiter='\t', lineterminator='\n')
    w.writerow(('address', 'name', 'symbol', 'source', 'convention', 'file'))
    for a in wanted:
        fn = index.by_address.get(a)
        hit = lookup(a, index, claims, rows)
        if hit is None:
            continue
        sym, source, where = hit
        w.writerow((f'0x{a:08x}', fn.name if fn else '', sym, source, convention(sym), where))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
