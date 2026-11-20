#!/usr/bin/env python3
"""The symbol table (analysis/symbols/STARFLEETOP.tsv): check it, fill a symbol in, show rows.

One row per function of the original; analysis/symbols/README.md describes the columns. The
table is edited through this tool only (`add`), and written first by
analysis/scripts/seed_symbols.py.

    python3 tools/symbols.py [--table T] check [--functions F] [--build B] [--src-root R]
    python3 tools/symbols.py [--table T] add <address> <symbol> [--declaration TEXT]
    python3 tools/symbols.py [--table T] claim [<address>...] [--build B] [--src-root R]
    python3 tools/symbols.py [--table T] show <address>...

`check` exits 1 with one line per finding that starts ERROR; a line that starts WARN is
informational.
"""
import argparse
import csv
import json
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, REPO)

COLUMNS = ('address', 'kind', 'name', 'class', 'symbol', 'symbol_evidence', 'declaration',
           'evidence')
KINDS = ('function', 'thunk', 'folded')
# Where a row's name came from: claim (the claiming unit's compiled symbol), mangled
# (functions.csv), sdk (an SDK library match), rtti (class recovery), fid (Ghidra's function id);
# vtable and none mean the name is the placeholder fn_<address> (vtable: the class is known
# from a vtable slot).
EVIDENCE = ('claim', 'mangled', 'sdk', 'rtti', 'fid', 'vtable', 'none')
PLACEHOLDER_EVIDENCE = ('vtable', 'none')
# Where a symbol came from: claim (what the claiming unit compiled; it tracks the build),
# mangled (functions.csv), callers (every exact caller's relocation names it), add (this tool).
SYMBOL_EVIDENCE = ('claim', 'mangled', 'callers', 'add')
PER_UNIT_PREFIXES = ('$', '_$', '__real@', '??_C@')   # as harness/lib/verify.py: never seeded
ADDRESS = re.compile(r'^0x[0-9a-f]{8}$')
# ?name@scope@@... (a C++ symbol), _name or _name@N (cdecl, stdcall), @name@N (fastcall).
MANGLED = re.compile(r'^(\?[^\s@][^\s]*@[^\s]*|_[A-Za-z0-9_$?@]+|@[A-Za-z0-9_]+@\d+)$')
# A name that stands in for evidence: Ghidra's and this project's placeholders, the stand-in
# classes sessions invented, an analyzer's vfunctionN.
PLACEHOLDER = re.compile(r'^(?:(?:fn|FUN|thunk_FUN)_[0-9a-fA-F]{8}(?:_\w+)?|tObject_[0-9a-fA-F]{8}\w*'
                         r'|[cC]_[0-9a-fA-F]{8}\w*|c_\w+_Field\d+|vfunction\d+)$')
VTBL_CLASS = re.compile(r'^tVtbl_[0-9a-f]{8}$')
OPERATORS = {'2': 'new', '3': 'delete', '4': '=', '5': '>>', '6': '<<', '7': '!', '8': '==',
             '9': '!=', 'A': '[]', 'C': '->', 'D': '*', 'E': '++', 'F': '--', 'G': '-', 'H': '+',
             'I': '&', 'J': '->*', 'K': '/', 'L': '%', 'M': '<', 'N': '<=', 'O': '>', 'P': '>=',
             'Q': ',', 'R': '()', 'S': '~', 'T': '^', 'U': '|', 'V': '&&', 'W': '||', 'X': '*=',
             'Y': '+=', 'Z': '-='}


def shown(path):
    rel = os.path.relpath(path, REPO)
    return path if rel.startswith('..') else rel


def default_table():
    return os.path.join(REPO, 'analysis', 'symbols', 'STARFLEETOP.tsv')


def plausible_symbol(symbol):
    """True when `symbol` can be an MSVC symbol: no blanks, no C++ scope or parentheses."""
    return bool(MANGLED.match(symbol)) and '::' not in symbol and '(' not in symbol


def is_placeholder(component):
    """True for one name component (`fn_00401000`, `tObject_00401000`, `vfunction3`, ...)."""
    return bool(PLACEHOLDER.match(component.lstrip('~')))


def components(name):
    """The parts of 'Outer::Inner::method' split at '::' outside <...>."""
    out, depth, start = [], 0, 0
    for i, ch in enumerate(name):
        if ch == '<':
            depth += 1
        elif ch == '>':
            depth -= 1
        elif ch == ':' and depth == 0 and name[i:i + 2] == '::':
            out.append(name[start:i])
            start = i + 2
    out.append(name[start:])
    return out


def has_placeholder(name):
    """True when any component of a qualified name is a placeholder."""
    return any(is_placeholder(c) for c in components(name))


def split_qualified(name):
    """('Outer::Inner', 'method') for 'Outer::Inner::method', ignoring a '::' inside <...>."""
    parts = components(name)
    return '::'.join(parts[:-1]), parts[-1]


def head_scopes(symbol):
    """(special code or '', scopes, what follows '@@') of a plain MSVC symbol, or None when it
    does not read with certainty (a template, another special name, a C symbol, a scope that is
    not an identifier or is only digits)."""
    if not symbol.startswith('?'):
        return None
    body, special = symbol[1:], ''
    if body.startswith('?'):
        code, body = body[1:2], body[2:]
        if code and code in '01':
            special = code
        elif code in OPERATORS:
            special = 'op' + code
        else:
            return None
    if '@@' not in body:
        return None
    head, rest = body.split('@@', 1)
    if '?' in head:
        return None
    scopes = head.split('@')
    if not all(s and s.replace('_', 'a').isalnum() and not s.isdigit() for s in scopes):
        return None
    return special, scopes, rest


def callee_verdict(symbol, address):
    """(True, '') when a symbol named by a caller's relocation may be written for the function at
    the int `address` (the seed's rule for `callers`, and the loop's for `add`), else (False,
    reason). No placeholder component is allowed except the function's own name fn_<address>,
    free or member of a class with a real name."""
    h = head_scopes(symbol)
    if h is None:
        return True, ''
    special, scopes, _ = h
    bad = [i for i, s in enumerate(scopes) if is_placeholder(s)]
    if not bad:
        return True, ''
    if not special and bad == [0] and scopes[0].lower() == f'fn_{address:08x}':
        return True, ''
    if bad == [0] and not special:
        return False, 'fn-name-of-another-address'
    return False, 'placeholder-class'


def decode_symbol(symbol):
    """(qualified name, class) from a plain MSVC symbol, or None for anything it does not read
    with certainty. `?m@A@B@@Q...` is B::A::m of class B::A; a non-member (`@@Y...`) has no
    class, its qualifier is a namespace; a constructor `??0A@B@@...` is B::A::A, a destructor
    B::A::~A."""
    h = head_scopes(symbol)
    if h is None:
        return None
    special, scopes, rest = h
    nonmember = rest[:1] == 'Y'
    if special:
        qual = '::'.join(reversed(scopes))
        last = scopes[0] if special in ('0', '1') else f'operator{OPERATORS[special[2:]]}'
        name = f'{qual}::{"~" if special == "1" else ""}{last}'
    else:
        qual = '::'.join(reversed(scopes[1:]))
        name = f'{qual}::{scopes[0]}' if qual else scopes[0]
    return name, '' if nonmember else qual


def symbol_placeholders(symbol):
    """The placeholder name components of a symbol's name and scopes (not its argument types)."""
    h = head_scopes(symbol)
    if h is None:
        return []
    return [s for s in h[1] if is_placeholder(s)]


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


def write_atomic(path, text):
    """Write `text` to a temporary file beside `path` and rename it over `path`."""
    tmp = path + '.tmp'
    try:
        with open(tmp, 'w', encoding='utf-8', newline='') as f:
            f.write(text)
        os.replace(tmp, path)
    except BaseException:
        try:
            os.remove(tmp)
        except OSError:
            pass
        raise


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


def check_text(text, known, build_symbols=None, build_level='WARN', claimed=None):
    """The findings for the table text: ['ERROR ...' or 'WARN ...', ...], in file order.
    `known` is known_functions(); `build_symbols` {address: compiled symbol of a current claim}
    or None; a difference from the table is a finding of level `build_level`. `claimed` is the
    set of current claims' addresses when known: a row with symbol_evidence claim at another
    address is an error."""
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
        elif row['evidence'] not in PLACEHOLDER_EVIDENCE and has_placeholder(row['name']):
            err(f'ERROR line {n}: name {row["name"]!r} has a placeholder component and is not evidence')
        if any(is_placeholder(c) for c in components(row['class'])) and not VTBL_CLASS.match(row['class']):
            err(f'ERROR line {n}: class {row["class"]!r} has a placeholder component')
        if row['symbol']:
            if row['symbol'].startswith(PER_UNIT_PREFIXES):
                err(f'ERROR line {n}: symbol {row["symbol"]!r} is per unit and is never recorded')
            elif not plausible_symbol(row['symbol']):
                err(f'ERROR line {n}: symbol {row["symbol"]!r} is not a plausible MSVC symbol')
        if row['symbol_evidence'] not in SYMBOL_EVIDENCE + ('',):
            err(f'ERROR line {n}: symbol_evidence {row["symbol_evidence"]!r} is not one of '
                f'{", ".join(SYMBOL_EVIDENCE)} or empty')
        elif bool(row['symbol']) != bool(row['symbol_evidence']):
            err(f'ERROR line {n}: symbol and symbol_evidence must both be set or both empty')
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
        if len(rs) > 1:
            err(f'ERROR symbol {sym} appears {len(rs)} times: '
                f'{", ".join(r["address"] + " " + r["kind"] for r in rs)}')
    for r in rows:
        if build_symbols and r['kind'] != 'folded' and r['address'] in build_symbols \
                and build_symbols[r['address']] != r['symbol']:
            err(f'{build_level} {r["address"]}: the last build compiled '
                f'{build_symbols[r["address"]]}, the table has {r["symbol"] or "no symbol"}; '
                'run `python3 tools/symbols.py claim`')
        if claimed is not None and r['symbol_evidence'] == 'claim' and r['address'] not in claimed:
            err(f'ERROR {r["address"]}: symbol_evidence is claim but the address is no longer '
                'claimed; run `python3 tools/symbols.py claim` to reset the row')
    return out


def current_claims(src_root):
    """The addresses (text) a FUNCTION annotation under `src_root` claims."""
    from harness.lib import source
    return {f'0x{a:08x}' for a, anns in source.scan_annotations(src_root, 'STARFLEETOP').items()
            if any(x.kind == 'FUNCTION' for x in anns)}


def read_build_symbols(build):
    """{address: symbol} of the build's symbols.tsv, or None without the file."""
    try:
        with open(os.path.join(build, 'symbols.tsv'), newline='') as f:
            return {r['address']: r['symbol'] for r in csv.DictReader(f, delimiter='\t')
                    if r.get('symbol')}
    except OSError:
        return None


def claim_lines(lines, build_symbols, claimed, only=None):
    """(new lines, messages) after making each claimed row's symbol the build's, with
    symbol_evidence claim, and resetting a claim row whose address is no longer claimed (only
    when `only` is None). `lines` are the file's lines, header first; other lines stay as they
    are."""
    out, messages = list(lines), []
    for i, line in enumerate(lines[1:], 1):
        cells = line.split('\t')
        if len(cells) != len(COLUMNS) or cells[1] == 'folded':
            continue
        a = cells[0]
        if only is not None and a not in only:
            continue
        if a in claimed and a in build_symbols:
            new = build_symbols[a]
            if cells[4] != new or cells[5] != 'claim':
                messages.append(f'{a}: {cells[4] or "no symbol"} ({cells[5] or "none"}) -> {new} (claim)')
                cells[4], cells[5] = new, 'claim'
        elif only is None and a not in claimed and cells[5] == 'claim':
            messages.append(f'{a}: {cells[4]} (claim) -> no symbol (no longer claimed)')
            cells[4] = cells[5] = ''
        else:
            continue
        out[i] = '\t'.join(cells)
    return out, messages


# A claimed function's compiled symbol that differs from the table's: the seed writes the
# build's symbols, so a difference means the table is stale against the claims.
BUILD_LEVEL = 'ERROR'


def cmd_check(a):
    table = a.table or default_table()
    functions, build, src_root = a.functions, a.build, a.src_root
    if not (functions and build and src_root):
        from harness.lib import paths
        functions = functions or paths.FUNCTIONS_CSV
        build = build or paths.BUILD
        src_root = src_root or paths.SRC_ROOT
    try:
        with open(table, encoding='utf-8', newline='') as f:
            text = f.read()
    except OSError as e:
        print(f'ERROR cannot read {table}: {e.strerror}')
        return 1
    built = read_build_symbols(build)
    claimed = current_claims(src_root) if built is not None else None
    if built is not None:
        built = {k: v for k, v in built.items() if k in claimed}
    findings = check_text(text, known_functions(functions), built, BUILD_LEVEL, claimed)
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
    if not plausible_symbol(symbol) or symbol.startswith(PER_UNIT_PREFIXES + ('__imp_', '_imp_')):
        print(f'ERROR {symbol!r} is not a plausible MSVC symbol for a function (a per-unit or '
              'import symbol is never recorded)')
        return 1
    if a.callee:
        ok, why = callee_verdict(symbol, int(address, 16))
        if not ok:
            print(f'ERROR {symbol} is not accepted for {address} ({why})')
            return 1
    elif symbol_placeholders(symbol):
        print(f'ERROR {symbol} names a placeholder ({", ".join(symbol_placeholders(symbol))}): '
              'a placeholder is not evidence')
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
    if row[5] == 'claim':
        print(f'ERROR {address} is claimed: its claim defines {row[4]}, and the next seed replaces '
              'whatever is written here')
        return 1
    if row[4] and row[4] != symbol and row[5] != 'callers':
        print(f'ERROR {address} already has symbol {row[4]}; first writer wins, not {symbol}')
        return 1
    d = decode_symbol(symbol)
    if row[7] not in PLACEHOLDER_EVIDENCE and d and d[0] != row[2]:
        print(f'ERROR {address} is named {row[2]}, the symbol {symbol} decodes to {d[0]}')
        return 1
    elsewhere = sorted(set(hit) - {address})
    if elsewhere:
        print(f'ERROR symbol {symbol} is already at {", ".join(elsewhere)}')
        return 1
    if decl is not None and row[6] and row[6] != decl:
        print(f'ERROR {address} already has declaration {row[6]!r}, not {decl!r}')
        return 1
    new = list(row)
    if row[4] != symbol:
        if row[4]:
            print(f'{address}: replacing the callers symbol {row[4]} with {symbol}')
        new[4], new[5] = symbol, 'add'
    if decl is not None:
        new[6] = decl
    if new == row:
        print(f'{address}: unchanged')
        return 0
    lines[idx] = '\t'.join(new)
    write_atomic(table, '\n'.join(lines))
    print(f'{address}: {symbol}')
    return 0


def read_result_symbols(path):
    """{address: symbol} of the claims a `build-and-verify.sh <function> --json` output rebuilt
    (every function of the units it built), as harness/lib/verify.symbol_table_rows names them;
    None when the file holds no such output."""
    from harness.lib import verify
    try:
        with open(path, encoding='utf-8', errors='replace') as f:
            text = f.read()
        results = json.loads(text[text.index('{'):])['results']
    except (OSError, ValueError, KeyError):
        return None
    return {a: sym for a, sym, _ in verify.symbol_table_rows(results)}


def cmd_claim(a):
    table = a.table or default_table()
    build, src_root = a.build, a.src_root
    if not (build and src_root):
        from harness.lib import paths
        build = build or paths.BUILD
        src_root = src_root or paths.SRC_ROOT
    if a.results:
        built = read_result_symbols(a.results)
        if built is None:
            print(f'ERROR {a.results} holds no build-and-verify --json output')
            return 1
        a.address = a.address or sorted(built)
        if not a.address:
            print('0 rows changed')
            return 0
    else:
        built = read_build_symbols(build)
    if built is None:
        print(f'ERROR no symbols.tsv in {build}: run harness/build-and-verify.sh --all first')
        return 1
    only = None
    if a.address:
        try:
            only = {address_text(x) for x in a.address}
        except ValueError as e:
            print(f'ERROR {e}')
            return 1
        missing = sorted(only - set(built))
        if missing:
            print(f'ERROR the build has no symbol for {", ".join(missing)}')
            return 1
    with open(table, encoding='utf-8', newline='') as f:
        lines = f.read().split('\n')
    new, messages = claim_lines(lines, built, current_claims(src_root), only)
    for m in messages:
        print(m)
    if new != lines:
        write_atomic(table, '\n'.join(new))
    print(f'{len(messages)} rows changed')
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
    c.add_argument('--src-root', help='the source tree whose claims count (default op/)')
    c.set_defaults(fn=cmd_check)
    s = sub.add_parser('add', help="fill a row's symbol (and declaration) when it has none")
    s.add_argument('address')
    s.add_argument('symbol')
    s.add_argument('--declaration')
    s.add_argument('--callee', action='store_true',
                   help="the symbol is one a caller's relocation names: also accept the "
                   "function's own fn_<address> name, as the seed does for callers")
    s.set_defaults(fn=cmd_add)
    c = sub.add_parser('claim', help="make claimed rows carry the build's symbols")
    c.add_argument('address', nargs='*', help='default: every current claim')
    c.add_argument('--build')
    c.add_argument('--src-root')
    c.add_argument('--results', help='take the symbols from this build-and-verify --json output '
                   '(a single-function run), for the claims it rebuilt, instead of the build')
    c.set_defaults(fn=cmd_claim)
    h = sub.add_parser('show', help='print rows')
    h.add_argument('address', nargs='+')
    h.set_defaults(fn=cmd_show)
    a = p.parse_args(argv)
    return a.fn(a)


if __name__ == '__main__':
    sys.exit(main())
