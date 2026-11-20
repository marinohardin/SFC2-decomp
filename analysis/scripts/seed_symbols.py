#!/usr/bin/env python3
"""Seed analysis/symbols/STARFLEETOP.tsv: one row per function of the original.

    python3 analysis/scripts/seed_symbols.py --report <path> [--out TABLE] [--build DIR]
                                             [--reseed] [--no-folded]

Level 1 (name, class, evidence) comes from the claims in op/, functions.csv, the SDK label
tables and the vtable export. Level 2 (symbol) comes from, most trusted first:

  a  claim     the symbol the claiming unit compiled (<build>/symbols.tsv, harness/lib/symtab.py);
  b  mangled   the mangled name in functions.csv;
  c  callers   an unclaimed function that exact functions call, when every caller's relocation
               names it by one symbol (<build>/<unit>/<address>/result.json, as
               harness/lib/verify.py's exact_reloc_uses reads them).

A disagreement between sources goes to the report and the more trusted one wins. An address
whose callers disagree is not seeded; it is listed in the report with each symbol and its callers.
A symbol that would stand at two addresses is left empty at both. The declaration column is
never seeded.

Name precedence: the claim's compiled symbol, a mangled name, an unambiguous SDK label, another
name the harness counts as one (Function.has_name: not Ghidra's FUN_, an analyzer's vfunctionN or
a default), else fn_<address>. A table that exists keeps its symbol and declaration values (they
may have been added by `tools/symbols.py add`); --reseed takes the seed's instead.

The seed refuses to run when <build>/symbols.tsv is missing or older than the sources in op/:
run `harness/build-and-verify.sh --all` first. Same inputs, same bytes out.
"""
import argparse
import csv
import importlib.util
import json
import os
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, REPO)

from harness.lib import funcs, paths, source, symtab, verify  # noqa: E402


def _tool():
    spec = importlib.util.spec_from_file_location('sfcop_symbols_tool',
                                                  os.path.join(REPO, 'tools', 'symbols.py'))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


tool = _tool()

OPERATORS = {'2': 'new', '3': 'delete', '4': '=', '5': '>>', '6': '<<', '7': '!', '8': '==',
             '9': '!=', 'A': '[]', 'C': '->', 'D': '*', 'E': '++', 'F': '--', 'G': '-', 'H': '+',
             'I': '&', 'J': '->*', 'K': '/', 'L': '%', 'M': '<', 'N': '<=', 'O': '>', 'P': '>=',
             'Q': ',', 'R': '()', 'S': '~', 'T': '^', 'U': '|', 'V': '&&', 'W': '||', 'X': '*=',
             'Y': '+=', 'Z': '-='}
# name_source of functions.csv -> the evidence column, for a name the harness counts as one.
SOURCE_EVIDENCE = {'rtti-class-recovery': 'rtti', 'sdk-fid': 'sdk', 'sdklib-exact': 'sdk',
                   'ghidra-fid-vs': 'fid'}


def split_qualified(name):
    """('Outer::Inner', 'method') for 'Outer::Inner::method', ignoring a '::' inside <...>."""
    depth, cut = 0, -1
    for i, ch in enumerate(name):
        if ch == '<':
            depth += 1
        elif ch == '>':
            depth -= 1
        elif ch == ':' and depth == 0 and name[i:i + 2] == '::':
            cut = i
    return (name[:cut], name[cut + 2:]) if cut >= 0 else ('', name)


def decode_symbol(symbol):
    """(qualified name, class) from a plain MSVC symbol, or None for anything it does not read
    with certainty (templates, other special names, C symbols). `?m@A@B@@...` is B::A::m, a
    constructor `??0A@B@@...` is B::A::A, a destructor B::A::~A."""
    if not symbol.startswith('?') or '?$' in symbol:
        return None
    body, special = symbol[1:], ''
    if body.startswith('?'):
        code = body[1:2]
        body = body[2:]
        if code and code in '01':
            special = code
        elif code in OPERATORS:
            special = 'op' + code
        else:
            return None
    head = body.split('@@', 1)[0] if '@@' in body else None
    if head is None:
        return None
    scopes = head.split('@')
    if not all(s and s.replace('_', 'a').isalnum() for s in scopes):
        return None
    qual = '::'.join(reversed(scopes[1:] if not special else scopes))
    if special in ('0', '1'):
        cls = qual
        return f'{cls}::{"~" if special == "1" else ""}{scopes[0]}', cls
    if special:
        return f'{qual}::operator{OPERATORS[special[2:]]}', qual
    return (f'{qual}::{scopes[0]}' if qual else scopes[0]), qual


def owner_class(entries, bases):
    """The class a vtable slot belongs to. `entries` is [(vtable address text, class)] for the
    vtables holding the function, `bases` {class: set of all its base classes}. One class is
    that class; several are the one every other derives from, else ''. A vtable whose class
    is empty has no evidence name and is named tVtbl_<its address>."""
    named = sorted({c for _, c in entries if c})
    anon = sorted(v for v, c in entries if not c)
    if not named:
        return f'tVtbl_{anon[0][2:] if anon[0].startswith("0x") else anon[0]}' if anon else ''
    if len(named) == 1:
        return named[0]
    roots = [c for c in named if all(c == o or c in bases.get(o, ()) for o in named)]
    return roots[0] if len(roots) == 1 else ''


def level1(fn, claim_symbol, index, vt_entries, bases, stats):
    """(name, class, evidence) for one function."""
    place = f'fn_{fn.address:08x}'
    name = cls = ev = None
    if claim_symbol:
        d = decode_symbol(claim_symbol)
        if d:
            (name, cls), ev = d, 'claim'
    if name is None and fn.mangled:
        if fn.has_name:
            name = fn.name
        elif decode_symbol(fn.mangled):
            name = decode_symbol(fn.mangled)[0]
        ev = 'mangled'
    if name is None:
        labels = [n for n in index.names_at.get(fn.address, ()) if n.startswith('?')]
        if len(labels) == 1 and not fn.mangled:
            d = decode_symbol(labels[0])
            name = fn.name if fn.has_name else (d[0] if d else None)
            ev = 'sdk'
    if name is None and fn.has_name:
        name, ev = fn.name, SOURCE_EVIDENCE.get(fn.name_source, 'other')
    if name is not None:
        stats['name'][ev] = stats['name'].get(ev, 0) + 1
        cls = cls if cls is not None else split_qualified(name)[0]
        if not cls:
            cls = owner_class(vt_entries.get(fn.address, ()), bases)
        return name, cls, ev
    # no evidence for a name: the class, when the analysis or a vtable gives one
    cls = ''
    if '::vfunction' in fn.name:
        cls = fn.name.split('::vfunction')[0]
    if not cls:
        cls = owner_class(vt_entries.get(fn.address, ()), bases)
    ev = 'vtable' if cls else 'none'
    stats['name'][ev] = stats['name'].get(ev, 0) + 1
    return place, cls, ev


def caller_symbols(results, index, claimed):
    """{target address int: {symbol: set of caller addresses}} from the relocations of exact
    functions that reach an unclaimed function or thunk by a symbol that is not per unit."""
    out = {}
    for name, addr, per_unit, _, caller, r in verify.exact_reloc_uses(results):
        if per_unit or r.get('type') not in verify.CLAIM_RELOC_TYPES or name.startswith('$'):
            continue
        a = int(addr, 16)
        fn = index.by_address.get(a)
        if fn is None or fn.kind not in ('function', 'thunk') or a in claimed:
            continue
        if not tool.plausible_symbol(name):
            continue
        out.setdefault(a, {}).setdefault(name, set()).add(caller)
    return out


def seed(index, claim_syms, claimed, vtables, bases, results, existing=None, reseed=False,
         folded=True):
    """(rows, stats) for the table. claim_syms {address text: (symbol, file)}, claimed the
    addresses a FUNCTION annotation claims, vtables [(vtable, class, function address text)],
    bases {class: set of base classes}, results the build's result dicts, existing {address
    text: row} of the table so far."""
    existing = existing or {}
    vt_entries = {}
    for vt, cls, fa in vtables:
        vt_entries.setdefault(int(fa, 16), []).append((vt, cls))
    for v in vt_entries.values():
        v.sort()
    stats = {'name': {}, 'symbol': {}, 'conflicts': [], 'disagree': [], 'duplicates': [],
             'kept': [], 'unbuilt': [], 'folded': 0}
    callers = caller_symbols([r for r in results if r.get('address') in claim_syms
                              and claim_syms[r['address']][1] == r.get('unit')], index, claimed)
    fns = sorted((f for f in index.functions if f.kind in ('function', 'thunk')),
                 key=lambda f: f.address)
    chosen = {}                     # address int -> (symbol, source)
    for fn in fns:
        a = fn.address
        key = f'0x{a:08x}'
        cand = {}
        if key in claim_syms and claim_syms[key][0] and tool.plausible_symbol(claim_syms[key][0]) \
                and not claim_syms[key][0].startswith(tool.PER_UNIT_PREFIXES):
            cand['claim'] = claim_syms[key][0]
        elif a in claimed:
            stats['unbuilt'].append(key)
        if fn.mangled and not fn.mangled.startswith(tool.PER_UNIT_PREFIXES) \
                and tool.plausible_symbol(fn.mangled):
            cand['mangled'] = fn.mangled
        if a in callers:
            syms = callers[a]
            if len(syms) == 1:
                cand['callers'] = next(iter(syms))
            else:
                stats['conflicts'].append((key, {s: sorted(c) for s, c in sorted(syms.items())}))
        if len(set(cand.values())) > 1:
            stats['disagree'].append((key, dict(sorted(cand.items()))))
        for src in ('claim', 'mangled', 'callers'):
            if src in cand:
                chosen[a] = (cand[src], src)
                break
    # an existing value wins over the seed's, unless --reseed
    for key, row in existing.items():
        a = int(key, 16)
        if a not in index.by_address:
            continue
        if not reseed and row['symbol']:
            if a in chosen and chosen[a][0] != row['symbol']:
                stats['kept'].append((key, row['symbol'], chosen[a][0]))
            chosen[a] = (row['symbol'], 'existing')
    users = {}
    for a, (sym, _) in chosen.items():
        users.setdefault(sym, []).append(a)
    for sym, addrs in sorted(users.items()):
        if len(addrs) > 1:
            stats['duplicates'].append((sym, [f'0x{a:08x}' for a in sorted(addrs)]))
            for a in addrs:
                if chosen[a][1] != 'existing':
                    del chosen[a]
    rows = []
    bases = bases or {}
    for fn in fns:
        a = fn.address
        key = f'0x{a:08x}'
        sym, src = chosen.get(a, ('', ''))
        name, cls, ev = level1(fn, claim_syms.get(key, ('', ''))[0], index, vt_entries, bases,
                               stats)
        decl = existing[key]['declaration'] if key in existing and not reseed else ''
        if sym:
            stats['symbol'][src] = stats['symbol'].get(src, 0) + 1
        rows.append(dict(address=key, kind=fn.kind, name=name, **{'class': cls}, symbol=sym,
                         declaration=decl, evidence=ev))
        if folded and sym:
            labels = sorted(n for n in index.names_at.get(a, ()) if n.startswith('?'))
            if sym in labels:
                for extra in labels:
                    if extra != sym and not extra.startswith(tool.PER_UNIT_PREFIXES):
                        d = decode_symbol(extra)
                        rows.append(dict(address=key, kind='folded', name=d[0] if d else extra,
                                         **{'class': d[1] if d else ''}, symbol=extra,
                                         declaration='', evidence=('mangled' if extra == fn.mangled
                                                                   else 'sdk')))
                        stats['folded'] += 1
    return sorted(rows, key=tool.sort_key), stats


def report_text(rows, stats):
    out = ['rows: %d primary, %d folded' % (sum(r['kind'] != 'folded' for r in rows), stats['folded']),
           '', 'name evidence (primary rows):']
    out += [f'  {k:<8} {stats["name"][k]}' for k in sorted(stats['name'])]
    out += ['', 'symbols by source (after existing values):']
    out += [f'  {k:<8} {stats["symbol"][k]}' for k in sorted(stats['symbol'])]
    out += ['', f'claimed addresses without a compiled symbol: {len(stats["unbuilt"])}']
    out += [f'  {a}' for a in stats['unbuilt']]
    out += ['', f'callers disagree, not seeded: {len(stats["conflicts"])}']
    for key, syms in stats['conflicts']:
        out.append(f'  {key}')
        for s, c in syms.items():
            out.append(f'    {s}  called by {", ".join(c)}')
    out += ['', f'sources disagree, the more trusted won: {len(stats["disagree"])}']
    for key, c in stats['disagree']:
        out.append(f'  {key}  ' + '  '.join(f'{s}={v}' for s, v in c.items()))
    out += ['', f'a symbol at two addresses, seeded values left empty: {len(stats["duplicates"])}']
    for sym, addrs in stats['duplicates']:
        out.append(f'  {sym}  {", ".join(addrs)}')
    out += ['', f'an existing symbol kept over the seed\'s: {len(stats["kept"])}']
    for key, old, new in stats['kept']:
        out.append(f'  {key}  kept {old}  seed {new}')
    return '\n'.join(out) + '\n'


def load_results(build):
    """The result.json of every function scored in the build folder, sorted by path."""
    out = []
    if not os.path.isdir(build):
        return out
    for unit in sorted(os.listdir(build)):
        d = os.path.join(build, unit)
        if not os.path.isdir(d):
            continue
        for fn in sorted(os.listdir(d)):
            p = os.path.join(d, fn, 'result.json')
            if fn.startswith('0x') and os.path.isfile(p):
                with open(p) as f:
                    out.append(json.load(f))
    return out


def newest_source(src_root):
    """The latest modification time of a file the claims are read from."""
    newest = 0.0
    for d, dirs, files in os.walk(src_root):
        for name in files:
            if name.lower().endswith(source.ANNOTATED_EXTS):
                newest = max(newest, os.path.getmtime(os.path.join(d, name)))
    return newest


def load_vtables(path, classes_path):
    with open(path, newline='') as f:
        vt = [(r['vtable_address'], r['class'], r['function_address']) for r in csv.DictReader(f)]
    vt = [(f'0x{int(v, 16):08x}', c, f'0x{int(a, 16):08x}') for v, c, a in vt]
    with open(classes_path, newline='') as f:
        bases = {r['class']: set(filter(None, r['all_bases'].split(';')))
                 for r in csv.DictReader(f)}
    return vt, bases


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    p.add_argument('--report', required=True, help='where to write the report (not committed)')
    p.add_argument('--out', default=tool.default_table())
    p.add_argument('--build', default=paths.BUILD, help='the harness build folder')
    p.add_argument('--src-root', default=paths.SRC_ROOT)
    p.add_argument('--module', default='STARFLEETOP')
    p.add_argument('--reseed', action='store_true', help='replace existing symbol/declaration values')
    p.add_argument('--no-folded', action='store_true')
    a = p.parse_args(argv)
    table = os.path.join(a.build, 'symbols.tsv')
    if not os.path.isfile(table):
        print(f'no {table}: run harness/build-and-verify.sh --all first', file=sys.stderr)
        return 1
    if os.path.getmtime(table) < newest_source(a.src_root):
        print(f'{table} is older than the sources in {a.src_root}: run '
              'harness/build-and-verify.sh --all again', file=sys.stderr)
        return 1
    index = funcs.FunctionIndex(paths.FUNCTIONS_CSV, paths.SYMBOL_TSVS)
    exports = os.path.dirname(paths.FUNCTIONS_CSV)
    vtables, bases = load_vtables(os.path.join(exports, 'vtables.csv'),
                                  os.path.join(exports, 'classes.csv'))
    claimed = {x for x, anns in source.scan_annotations(a.src_root, a.module).items()
               if any(n.kind == 'FUNCTION' for n in anns)}
    claim_syms = symtab.load_claims(table)
    existing = {r['address']: r for r in tool.read_rows(a.out) if r['kind'] != 'folded'} \
        if os.path.isfile(a.out) else {}
    rows, stats = seed(index, claim_syms, claimed, vtables, bases, load_results(a.build),
                       existing, a.reseed, not a.no_folded)
    os.makedirs(os.path.dirname(os.path.abspath(a.out)), exist_ok=True)
    with open(a.out, 'w', encoding='utf-8', newline='') as f:
        f.write(tool.format_rows(rows))
    with open(a.report, 'w', encoding='utf-8', newline='') as f:
        f.write(report_text(rows, stats))
    print(f'wrote {len(rows)} rows to {a.out}; report in {a.report}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
