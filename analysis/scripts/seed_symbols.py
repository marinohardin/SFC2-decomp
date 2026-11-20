#!/usr/bin/env python3
"""Seed analysis/symbols/STARFLEETOP.tsv: one row per function of the original.

    python3 analysis/scripts/seed_symbols.py --report <path> [--out TABLE] [--build DIR]
                                             [--reseed]

Level 1 (name, class, evidence) comes from the claims in op/, functions.csv, the SDK label
tables and the vtable export. A placeholder is never evidence (tools/symbols.py
has_placeholder): a name with a placeholder component is fn_<address>. Level 2 (symbol and
symbol_evidence) comes from, most trusted first:

  claim    the symbol the claiming unit compiled (<build>/symbols.tsv, harness/lib/symtab.py,
           rows of current claims only). It always tracks the build: a seed replaces any other
           value of a claimed address and lists the replacement in the report;
  mangled  the mangled name in functions.csv, unless it decodes to another name than the row's;
  callers  an unclaimed function that exact functions call, when every caller's relocation
           names it by one symbol (<build>/<unit>/<address>/result.json, as
           harness/lib/verify.py's exact_reloc_uses reads them), and the symbol has no
           placeholder component other than the function's own fn_<address>.

For a row that is not claimed an existing symbol is kept (first writer wins); --reseed takes
the seed's instead. A symbol that would stand at two addresses is left empty at both (a claim
and an existing value excepted). Declarations are never seeded; a re-seed keeps them.

The seed refuses to run when <build>/symbols.tsv is missing or older than the sources in op/:
run `harness/build-and-verify.sh --all` first. The check is a guard against an old build, not
proof that the build matches the sources. Same inputs, same bytes out.
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

# name_source of functions.csv -> the evidence column. Any other source (Ghidra's analysis and
# imports, defaults) gives a name that is not evidence.
SOURCE_EVIDENCE = {'rtti-class-recovery': 'rtti', 'sdk-fid': 'sdk', 'sdklib-exact': 'sdk',
                   'ghidra-fid-vs': 'fid'}


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


def evidence_name(fn):
    """The name functions.csv gives the function when it is evidence, else None."""
    if not fn.has_name or tool.has_placeholder(fn.name):
        return None
    if fn.name_source in SOURCE_EVIDENCE or fn.mangled:
        return fn.name
    return None


def mangled_disagrees(fn):
    """True when the mangled name of functions.csv decodes to another name than the row's."""
    d = tool.decode_symbol(fn.mangled) if fn.mangled else None
    name = evidence_name(fn)
    return bool(d and name and d[0] != name)


def level1(fn, claim_symbol, index, vt_entries, bases, stats):
    """(name, class, evidence) for one function."""
    name = cls = ev = None
    claim_cls = ''
    if claim_symbol:
        d = tool.decode_symbol(claim_symbol)
        if d and not tool.has_placeholder(d[0]):
            name, cls, ev = d[0], d[1], 'claim'
        elif d and not tool.has_placeholder(d[1]):
            claim_cls = d[1]
    if name is None and fn.mangled and not mangled_disagrees(fn):
        d = tool.decode_symbol(fn.mangled)
        csv_name = evidence_name(fn)
        if d and not tool.has_placeholder(d[0]):
            name, cls, ev = csv_name or d[0], d[1], 'mangled'
        elif csv_name:
            name, cls, ev = csv_name, tool.split_qualified(csv_name)[0], 'mangled'
    if name is None and not fn.mangled:
        labels = sorted(n for n in index.names_at.get(fn.address, ()) if n.startswith('?'))
        if len(labels) == 1:
            d = tool.decode_symbol(labels[0])
            csv_name = evidence_name(fn)
            if csv_name:
                name, cls, ev = csv_name, tool.split_qualified(csv_name)[0], 'sdk'
            elif d and not tool.has_placeholder(d[0]):
                name, cls, ev = d[0], d[1], 'sdk'
    if name is None and not fn.mangled and evidence_name(fn):
        name, ev = fn.name, SOURCE_EVIDENCE[fn.name_source]
        cls = tool.split_qualified(name)[0]
    if name is not None:
        stats['name'][ev] = stats['name'].get(ev, 0) + 1
        return name, cls or owner_class(vt_entries.get(fn.address, ()), bases), ev
    # no evidence for a name: the class, when the claim, a vtable or the analysis gives one
    cls = claim_cls
    from_vtable = False
    if not cls:
        cls = owner_class(vt_entries.get(fn.address, ()), bases)
        from_vtable = bool(cls)
    if not cls and '::vfunction' in fn.name:
        pre = fn.name.split('::vfunction')[0]
        cls = '' if tool.has_placeholder(pre) else pre
        from_vtable = bool(cls)
    ev = 'vtable' if from_vtable else 'none'
    stats['name'][ev] = stats['name'].get(ev, 0) + 1
    return f'fn_{fn.address:08x}', cls, ev


def callers_verdict(symbol, address):
    """(True, '') when a caller-named symbol may be seeded for the function at `address`, else
    (False, reason). No placeholder component is allowed except the function's own name
    fn_<address>, free or member of a class with a real name."""
    h = tool.head_scopes(symbol)
    if h is None:
        return True, ''
    special, scopes, _ = h
    bad = [i for i, s in enumerate(scopes) if tool.is_placeholder(s)]
    if not bad:
        return True, ''
    if not special and bad == [0] and scopes[0].lower() == f'fn_{address:08x}':
        return True, ''
    if bad == [0] and not special:
        return False, 'fn-name-of-another-address'
    return False, 'placeholder-class'


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


def seed(index, claim_syms, claimed, vtables, bases, results, existing=None, reseed=False):
    """(rows, stats) for the table. claim_syms {address text: (symbol, file)}, claimed the
    addresses a FUNCTION annotation claims, vtables [(vtable, class, function address text)],
    bases {class: set of base classes}, results the build's result dicts, existing {address
    text: row} of the table so far."""
    existing = existing or {}
    claim_syms = {k: v for k, v in claim_syms.items() if int(k, 16) in claimed}
    vt_entries = {}
    for vt, cls, fa in vtables:
        vt_entries.setdefault(int(fa, 16), []).append((vt, cls))
    for v in vt_entries.values():
        v.sort()
    stats = {'name': {}, 'symbol': {}, 'conflicts': [], 'disagree': [], 'duplicates': [],
             'kept': [], 'unbuilt': [], 'replaced': [], 'mangled_names': [],
             'callers_single': 0, 'callers_accepted': 0, 'callers_rejected': {}}
    callers = caller_symbols([r for r in results if r.get('address') in claim_syms
                              and claim_syms[r['address']][1] == r.get('unit')], index, claimed)
    fns = sorted((f for f in index.functions if f.kind in ('function', 'thunk')),
                 key=lambda f: f.address)
    chosen, kept = {}, set()        # address int -> (symbol, symbol_evidence)
    for fn in fns:
        a = fn.address
        key = f'0x{a:08x}'
        cand = {}
        sym = claim_syms.get(key, ('', ''))[0]
        if sym and tool.plausible_symbol(sym) and not sym.startswith(tool.PER_UNIT_PREFIXES):
            cand['claim'] = sym
        elif a in claimed:
            stats['unbuilt'].append(key)
        if fn.mangled and not fn.mangled.startswith(tool.PER_UNIT_PREFIXES) \
                and tool.plausible_symbol(fn.mangled):
            if mangled_disagrees(fn):
                stats['mangled_names'].append((key, fn.mangled, fn.name))
            else:
                cand['mangled'] = fn.mangled
        if a in callers:
            syms = callers[a]
            if len(syms) == 1:
                stats['callers_single'] += 1
                s = next(iter(syms))
                ok, why = callers_verdict(s, a)
                if ok:
                    cand['callers'] = s
                    stats['callers_accepted'] += 1
                else:
                    stats['callers_rejected'][why] = stats['callers_rejected'].get(why, 0) + 1
            else:
                stats['conflicts'].append((key, {s: sorted(c) for s, c in sorted(syms.items())}))
        if len(set(cand.values())) > 1:
            stats['disagree'].append((key, dict(sorted(cand.items()))))
        for src in ('claim', 'mangled', 'callers'):
            if src in cand:
                chosen[a] = (cand[src], src)
                break
        row = existing.get(key)
        if row and row['symbol'] and not reseed:
            old_ev = row['symbol_evidence'] or 'add'
            if 'claim' in cand and cand['claim'] != row['symbol']:
                stats['replaced'].append((key, row['symbol'], old_ev, cand['claim']))
            elif 'claim' not in cand and old_ev != 'claim':
                if a in chosen and chosen[a][0] != row['symbol']:
                    stats['kept'].append((key, row['symbol'], chosen[a][0]))
                chosen[a] = (row['symbol'], old_ev)
                kept.add(a)
    users = {}
    for a, (sym, _) in chosen.items():
        users.setdefault(sym, []).append(a)
    for sym, addrs in sorted(users.items()):
        if len(addrs) > 1:
            stats['duplicates'].append((sym, [f'0x{a:08x}' for a in sorted(addrs)]))
            for a in addrs:
                if chosen[a][1] != 'claim' and a not in kept:
                    del chosen[a]
    rows = []
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
                         symbol_evidence=src, declaration=decl, evidence=ev))
    return sorted(rows, key=tool.sort_key), stats


def report_text(rows, stats):
    out = ['rows: %d' % len(rows), '', 'name evidence:']
    out += [f'  {k:<8} {stats["name"][k]}' for k in sorted(stats['name'])]
    out += ['', 'symbols by source:']
    out += [f'  {k:<8} {stats["symbol"][k]}' for k in sorted(stats['symbol'])]
    out += ['', f'claimed addresses without a compiled symbol: {len(stats["unbuilt"])}']
    out += [f'  {a}' for a in stats['unbuilt']]
    out += ['', f'unclaimed callees with one caller name: {stats["callers_single"]}',
            f'accepted by the rules: {stats["callers_accepted"]}', 'rejected by reason:']
    out += [f'  {k:<28} {v}' for k, v in sorted(stats['callers_rejected'].items())]
    out += ['', f'callers disagree, not seeded: {len(stats["conflicts"])}']
    for key, syms in stats['conflicts']:
        out.append(f'  {key}')
        for s, c in syms.items():
            out.append(f'    {s}  called by {", ".join(c)}')
    out += ['', f'a mangled name that decodes to another name than the row, not seeded: '
                f'{len(stats["mangled_names"])}']
    out += [f'  {a}  {m}  row name {n}' for a, m, n in stats['mangled_names']]
    out += ['', f'sources disagree, the more trusted won: {len(stats["disagree"])}']
    for key, c in stats['disagree']:
        out.append(f'  {key}  ' + '  '.join(f'{s}={v}' for s, v in c.items()))
    out += ['', f'a claim replaced an earlier symbol: {len(stats["replaced"])}']
    for key, old, ev, new in stats['replaced']:
        out.append(f'  {key}  was {old} ({ev})  now {new}')
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
                       existing, a.reseed)
    os.makedirs(os.path.dirname(os.path.abspath(a.out)), exist_ok=True)
    tool.write_atomic(a.out, tool.format_rows(rows))
    tool.write_atomic(a.report, report_text(rows, stats))
    print(f'wrote {len(rows)} rows to {a.out}; report in {a.report}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
