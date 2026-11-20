#!/usr/bin/env python3
"""The symbol table (analysis/symbols/STARFLEETOP.tsv): check it, fill a symbol in, show rows.

One row per function of the original; analysis/symbols/README.md describes the columns. The
table is edited through this tool only (`add`), and written first by
analysis/scripts/seed_symbols.py.

    python3 tools/symbols.py [--table T] check [--functions F] [--build B]
    python3 tools/symbols.py [--table T] add <address> <symbol> [--declaration TEXT]
    python3 tools/symbols.py [--table T] show <address>...

`check` exits 1 with one line per finding that starts ERROR; a line that starts WARN is
informational (a claimed function whose last compiled symbol differs from the table's).
"""
import argparse
import csv
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, REPO)

COLUMNS = ('address', 'kind', 'name', 'class', 'symbol', 'declaration', 'evidence')
KINDS = ('function', 'thunk', 'folded')
# Where a row's name came from: claim (the claiming unit's compiled symbol), mangled
# (functions.csv), sdk (an SDK library match), rtti (class recovery), fid (Ghidra's function id),
# other (another name the harness counts as one); vtable and none mean the name is the
# placeholder fn_<address> (vtable: the class is known from a vtable slot).
EVIDENCE = ('claim', 'mangled', 'sdk', 'rtti', 'fid', 'other', 'vtable', 'none')
PLACEHOLDER_EVIDENCE = ('vtable', 'none')
PER_UNIT_PREFIXES = ('$', '_$', '__real@', '??_C@')   # as harness/lib/verify.py: never seeded
ADDRESS = re.compile(r'^0x[0-9a-f]{8}$')
# ?name@scope@@... (a C++ symbol), _name or _name@N (cdecl, stdcall), @name@N (fastcall).
MANGLED = re.compile(r'^(\?[^\s@][^\s]*@[^\s]*|_[A-Za-z0-9_$?@]+|@[A-Za-z0-9_]+@\d+)$')


def shown(path):
    rel = os.path.relpath(path, REPO)
    return path if rel.startswith('..') else rel


def default_table():
    return os.path.join(REPO, 'analysis', 'symbols', 'STARFLEETOP.tsv')


def plausible_symbol(symbol):
    """True when `symbol` can be an MSVC symbol: no blanks, no C++ scope or parentheses."""
    return bool(MANGLED.match(symbol)) and '::' not in symbol and '(' not in symbol


def address_text(text):
    """'0x006fc690', '006FC690' or 'fn_006fc690' as '0x006fc690'; ValueError otherwise."""
    s = text.strip().lower()
    for prefix in ('fn_', '0x'):
        if s.startswith(prefix):
            s = s[len(prefix):]
    if not re.fullmatch(r'[0-9a-f]{1,8}', s):
        raise ValueError(f'{text!r} is not an address')
    return f'0x{int(s, 16):08x}'


def sort_key(row):
    """Address, the primary row before its folded rows, then the folded rows by symbol."""
    return (int(row['address'], 16), row['kind'] == 'folded', row['symbol'], row['name'])


def format_rows(rows):
    """The file text: the header and one line per row, sorted, LF, no quoting."""
    lines = ['\t'.join(COLUMNS)]
    lines += ['\t'.join(r[c] for c in COLUMNS) for r in sorted(rows, key=sort_key)]
    return '\n'.join(lines) + '\n'


def read_rows(path):
    """The rows of a table as dicts; a line with another column count is skipped (`check`
    reports it)."""
    rows = []
    with open(path, encoding='utf-8', newline='') as f:
        for n, line in enumerate(f.read().split('\n')[:-1], 1):
            cells = line.split('\t')
            if n == 1 or len(cells) != len(COLUMNS):
                continue
            rows.append(dict(zip(COLUMNS, cells)))
    return rows


def known_functions(functions_csv):
    """{address text: 'function' or 'thunk'} for the functions of functions.csv (the exception
    funclets are not functions)."""
    out = {}
    with open(functions_csv, newline='') as f:
        for r in csv.DictReader(f):
            kind = r.get('kind') or 'function'
            if kind in ('function', 'thunk'):
                out[f'0x{int(r["address"], 16):08x}'] = kind
    return out


def check_text(text, known, build_symbols=None):
    """The findings for the table text: ['ERROR ...' or 'WARN ...', ...], in file order.
    `known` is known_functions(); `build_symbols` {address: compiled symbol} or None."""
    out = []
    err = out.append
    if '\r' in text:
        err('ERROR the file has CR characters; lines end in LF only')
    if text and not text.endswith('\n'):
        err('ERROR the file does not end in a newline')
    lines = text.split('\n')
    if lines and lines[-1] == '':
        lines.pop()
    if not lines or lines[0] != '\t'.join(COLUMNS):
        err('ERROR line 1: the header is not ' + '\t'.join(COLUMNS))
    rows, last = [], None
    for n, line in enumerate(lines[1:], 2):
        cells = line.split('\t')
        if len(cells) != len(COLUMNS):
            err(f'ERROR line {n}: {len(cells)} columns, expected {len(COLUMNS)}')
            continue
        row = dict(zip(COLUMNS, cells))
        if not ADDRESS.match(row['address']):
            err(f'ERROR line {n}: address {row["address"]!r} is not 0x followed by 8 lowercase hex digits')
            continue
        if row['kind'] not in KINDS:
            err(f'ERROR line {n}: kind {row["kind"]!r} is not one of {", ".join(KINDS)}')
        if row['evidence'] not in EVIDENCE:
            err(f'ERROR line {n}: evidence {row["evidence"]!r} is not one of {", ".join(EVIDENCE)}')
        if not row['name']:
            err(f'ERROR line {n}: empty name')
        elif (row['evidence'] in PLACEHOLDER_EVIDENCE) != (row['name'] == 'fn_' + row['address'][2:]):
            err(f'ERROR line {n}: name {row["name"]!r} and evidence {row["evidence"]!r} disagree: '
                'a placeholder name is fn_<address> with evidence vtable or none, and only that')
        if row['symbol']:
            if row['symbol'].startswith(PER_UNIT_PREFIXES):
                err(f'ERROR line {n}: symbol {row["symbol"]!r} is per unit and is never recorded')
            elif not plausible_symbol(row['symbol']):
                err(f'ERROR line {n}: symbol {row["symbol"]!r} is not a plausible MSVC symbol')
        if last is not None:
            la = int(last['address'], 16)
            a = int(row['address'], 16)
            if (a, row['kind'] == 'folded') < (la, last['kind'] == 'folded'):
                err(f'ERROR line {n}: {row["address"]} is out of order after {last["address"]}')
            elif a == la and row['kind'] != 'folded':
                err(f'ERROR line {n}: address {row["address"]} appears twice; only a folded '
                    'row may repeat the address of the row above')
        if row['kind'] == 'folded' and (last is None or last['address'] != row['address']):
            err(f'ERROR line {n}: a folded row must directly follow a row at {row["address"]}')
        if row['kind'] == 'folded' and not row['symbol']:
            err(f'ERROR line {n}: a folded row needs its own symbol')
        if row['kind'] != 'folded':
            if row['address'] not in known:
                err(f'ERROR line {n}: {row["address"]} is not a function or thunk of functions.csv')
            elif row['kind'] in KINDS and known[row['address']] != row['kind']:
                err(f'ERROR line {n}: {row["address"]} is a {known[row["address"]]} in functions.csv, '
                    f'the row says {row["kind"]}')
        rows.append(row)
        last = row
    have = {r['address'] for r in rows}
    missing = sorted(a for a in known if a not in have)
    if missing:
        err(f'ERROR {len(missing)} functions of functions.csv have no row, first {missing[0]}')
    at = {}
    for r in rows:
        if r['symbol']:
            at.setdefault(r['symbol'], []).append(r)
    for sym, rs in sorted(at.items()):
        addrs = sorted({r['address'] for r in rs})
        if len(addrs) > 1 and not any(r['kind'] == 'folded' for r in rs):
            err(f'ERROR symbol {sym} is at {len(addrs)} addresses: {", ".join(addrs)}')
    for r in rows:
        if build_symbols and r['kind'] != 'folded' and r['address'] in build_symbols \
                and build_symbols[r['address']] != r['symbol']:
            err(f'WARN {r["address"]}: the last build compiled {build_symbols[r["address"]]}, '
                f'the table has {r["symbol"] or "no symbol"}')
    return out


def _load_build_symbols(build):
    path = os.path.join(build, 'symbols.tsv')
    try:
        with open(path, newline='') as f:
            return {r['address']: r['symbol'] for r in csv.DictReader(f, delimiter='\t')
                    if r.get('symbol')}
    except OSError:
        return None


def cmd_check(a):
    table = a.table or default_table()
    functions, build = a.functions, a.build
    if not (functions and build):
        from harness.lib import paths
        functions = functions or paths.FUNCTIONS_CSV
        build = build or paths.BUILD
    try:
        with open(table, encoding='utf-8', newline='') as f:
            text = f.read()
    except OSError as e:
        print(f'ERROR cannot read {table}: {e.strerror}')
        return 1
    findings = check_text(text, known_functions(functions), _load_build_symbols(build))
    for line in findings:
        print(line)
    errors = [x for x in findings if x.startswith('ERROR')]
    if not errors:
        print(f'ok: {len(text.splitlines()) - 1} rows in {shown(table)}')
    return 1 if errors else 0


def cmd_add(a):
    table = a.table or default_table()
    try:
        address = address_text(a.address)
    except ValueError as e:
        print(f'ERROR {e}')
        return 1
    symbol, decl = a.symbol, a.declaration
    if not plausible_symbol(symbol) or symbol.startswith(PER_UNIT_PREFIXES):
        print(f'ERROR {symbol!r} is not a plausible MSVC symbol for a function (a per-unit '
              'symbol is never recorded)')
        return 1
    if decl is not None and ('\t' in decl or '\n' in decl):
        print('ERROR the declaration holds a tab or a newline')
        return 1
    with open(table, encoding='utf-8', newline='') as f:
        lines = f.read().split('\n')
    idx, hit = None, []
    for i, line in enumerate(lines[1:], 1):
        cells = line.split('\t')
        if len(cells) != len(COLUMNS):
            continue
        if cells[0] == address and cells[1] != 'folded':
            idx = i
        if cells[4] == symbol:
            hit.append(cells[0])
    if idx is None:
        print(f'ERROR {address} has no row in {shown(table)}')
        return 1
    row = lines[idx].split('\t')
    if row[4] and row[4] != symbol:
        print(f'ERROR {address} already has symbol {row[4]}; first writer wins, not {symbol}')
        return 1
    elsewhere = sorted(set(hit) - {address})
    if elsewhere:
        print(f'ERROR symbol {symbol} is already at {", ".join(elsewhere)}')
        return 1
    if decl is not None and row[5] and row[5] != decl:
        print(f'ERROR {address} already has declaration {row[5]!r}, not {decl!r}')
        return 1
    new = list(row)
    new[4] = symbol
    if decl is not None:
        new[5] = decl
    if new == row:
        print(f'{address}: unchanged')
        return 0
    lines[idx] = '\t'.join(new)
    tmp = table + '.tmp'
    with open(tmp, 'w', encoding='utf-8', newline='') as f:
        f.write('\n'.join(lines))
    os.replace(tmp, table)
    print(f'{address}: {symbol}')
    return 0


def cmd_show(a):
    table = a.table or default_table()
    rows = read_rows(table)
    status = 0
    print('\t'.join(COLUMNS))
    for text in a.address:
        try:
            address = address_text(text)
        except ValueError as e:
            print(f'ERROR {e}', file=sys.stderr)
            status = 1
            continue
        hits = [r for r in rows if r['address'] == address]
        if not hits:
            print(f'ERROR {address} has no row', file=sys.stderr)
            status = 1
        for r in hits:
            print('\t'.join(r[c] for c in COLUMNS))
    return status


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    p.add_argument('--table', help='the table (default analysis/symbols/STARFLEETOP.tsv)')
    sub = p.add_subparsers(dest='cmd', required=True)
    c = sub.add_parser('check', help='check the table')
    c.add_argument('--functions', help='functions.csv (default ghidra/exports/functions.csv)')
    c.add_argument('--build', help='the harness build folder whose symbols.tsv is compared')
    c.set_defaults(fn=cmd_check)
    s = sub.add_parser('add', help="fill a row's symbol (and declaration) when it has none")
    s.add_argument('address')
    s.add_argument('symbol')
    s.add_argument('--declaration')
    s.set_defaults(fn=cmd_add)
    h = sub.add_parser('show', help='print rows')
    h.add_argument('address', nargs='+')
    h.set_defaults(fn=cmd_show)
    a = p.parse_args(argv)
    return a.fn(a)


if __name__ == '__main__':
    sys.exit(main())
