"""Near-miss labels: why a claimed function that is not exact differs, from its own comparison.

    python3 -m harness.lib.nearmiss                      # every non-exact claim of the last verify
    python3 -m harness.lib.nearmiss 0x005fb3af ...       # these functions
    python3 -m harness.lib.nearmiss --unit Tiger/tsay.cpp
    python3 -m harness.lib.nearmiss --tsv | --json       # for a script
    python3 -m harness.lib.nearmiss --store              # also store the labels in the lane's lists
    python3 -m harness.lib.nearmiss --parked             # the stored reference-only functions that wait
    python3 -m harness.lib.nearmiss --rules [--tsv]      # RULE: lines from the lanes' sessions

It reads the build folder of the last build-and-verify.sh run in this checkout (each function's
result.json and diff.txt, and the unit's object) and the original's bytes, and rebuilds the
masked comparison the scorer made (target.build): it never compiles, never edits source and
never writes into the repository. One label per function, the first rule that holds:

- reference-only  the bytes are equal except inside the fields of relocations the scorer marked
                  mismatch (a call, vtable or data reference reaching another address than the
                  original's), and every differing row of the side-by-side listing holds a
                  relocation field. Also when no byte differs and no relocation mismatches but
                  objdiff still counts a row with a relocation (the strict check passes): such a
                  function waits, since no source change can help it.
- order           no relocation mismatches, and the differing rows hold the same instructions on
                  both sides (as multisets of instruction text), in another order.
- stack           no relocation mismatches, every differing row has an instruction on both
                  sides, each row is equal once esp/ebp displacements and the frame size
                  (`sub|add esp, N`) are masked, registers masked too where needed, and at least
                  one row needs the stack mask.
- registers       no relocation mismatches, every differing row has an instruction on both
                  sides, and each row is equal once register names are masked.
- structural      anything else: instructions missing or extra, other operands, other sizes, a
                  mix the rules above do not cover, or a function with no comparison to read.

Precedence is the order of the list. A function the rules do not fit cleanly is `structural`.
"""
import collections
import csv
import fcntl
import glob
import json
import os
import re
import sys
import time

from . import coff, funcs, paths, symtab, target

LABELS = ('reference-only', 'order', 'stack', 'registers', 'structural')
FIELDS = ('address', 'name', 'unit', 'label', 'score', 'differing', 'wrong_refs', 'waits',
          'labelled')
LISTING_MAX = 2500     # characters of the listing a context pack carries

_REG = r'(?:e?(?:ax|bx|cx|dx|si|di|bp|sp)|[abcd][lh])'
REGISTER = re.compile(r'(?<![\w?@$])' + _REG + r'(?![\w?@$])')
STACK_SLOT = re.compile(r'\[(esp|ebp)(?:[+-](?:0x[0-9a-f]+|\d+))?\]')
FRAME = re.compile(r'^(sub|add) esp, (?:0x[0-9a-f]+|\d+)$')
VALUE_PREFIXES = ('__real@', '??_C@')


def enabled(env=os.environ):
    """HARNESS_NEARMISS (default 1): the loop routes reference-only functions by their label."""
    return env.get('HARNESS_NEARMISS', '1') != '0'


# --- the comparison ------------------------------------------------------------------------

def parse_side_by_side(text):
    """Rows of a diff.txt (verify.write_side_by_side): (differs, offset or None, original,
    rebuilt). The header line is skipped."""
    rows = []
    for line in text.split('\n')[1:]:
        if not line:
            continue
        mark, body = line[0], line[1:]
        off = body[:7].strip()
        left, _, right = body[8:].partition('|')
        right = right[1:] if right.startswith(' ') else right
        rows.append((mark == '*', int(off[3:], 16) if off.startswith('+0x') else None,
                     left.rstrip(), right.rstrip()))
    return rows


def mask_registers(text):
    return REGISTER.sub('R', text)


def mask_stack(text):
    return FRAME.sub(r'\1 esp, N', STACK_SLOT.sub(r'[\1+N]', text))


def _row_spans(rows, size):
    """{row index: (start, end)} of each original instruction, from the listing's offsets."""
    starts = [(i, r[1]) for i, r in enumerate(rows) if r[1] is not None]
    spans = {}
    for k, (i, o) in enumerate(starts):
        spans[i] = (o, starts[k + 1][1] if k + 1 < len(starts) else size)
    return spans


def classify(rows, base, original, relocs):
    """The label for one function. `rows` from parse_side_by_side, `base` the rebuilt bytes,
    `original` the original's bytes as the scorer compared them (fields it accepted masked),
    `relocs` the scorer's relocations (dicts with offset and status). See the module text."""
    diff = [(i, r) for i, r in enumerate(rows) if r[0]]
    if not diff:
        return 'structural'
    mism = [r['offset'] for r in relocs if r['status'] == 'mismatch']
    fields = set()
    for o in mism:
        fields.update(range(o, o + 4))
    if len(base) == len(original):
        bytes_differ = [k for k in range(len(base)) if base[k] != original[k]]
        if all(k in fields for k in bytes_differ) and (bytes_differ or not mism):
            spans = _row_spans(rows, len(original))
            all_fields = [r['offset'] for r in relocs]
            if all(i in spans and any(spans[i][0] <= o < spans[i][1] for o in all_fields)
                   for i, _ in diff):
                return 'reference-only'
    if mism:
        return 'structural'
    left = collections.Counter(r[2] for _, r in diff if r[2])
    right = collections.Counter(r[3] for _, r in diff if r[3])
    if left and left == right:
        return 'order'
    if not all(r[2] and r[3] for _, r in diff):
        return 'structural'
    if all(mask_registers(r[2]) == mask_registers(r[3]) for _, r in diff):
        return 'registers'
    if all(mask_stack(r[2]) == mask_stack(r[3]) or
           mask_registers(mask_stack(r[2])) == mask_registers(mask_stack(r[3]))
           for _, r in diff):
        return 'stack'
    return 'structural'


# --- what a wrong reference reaches --------------------------------------------------------

def load_rows(path=None):
    """{address: row dict} of the symbol table's primary rows (a `folded` row is skipped)."""
    path = path or paths.SYMBOL_TABLE
    try:
        with open(path, encoding='utf-8', newline='') as f:
            return {r['address']: r for r in csv.DictReader(f, delimiter='\t')
                    if r.get('kind') != 'folded'}
    except OSError:
        return {}


def load_vtables(path=None):
    """{0x address: class} of the vtables export."""
    path = path or os.path.join(paths.REPO, 'ghidra', 'exports', 'vtables.csv')
    out = {}
    try:
        with open(path, newline='') as f:
            for r in csv.DictReader(f):
                out.setdefault(f'0x{int(r["vtable_address"], 16):08x}', r['class'])
    except (OSError, KeyError, ValueError):
        pass
    return out


def row_level(row):
    """How much the table knows of an address: full (symbol and declaration), symbol, name
    (a level 1 row only) or none (no row)."""
    if not row:
        return 'none'
    if row.get('symbol'):
        return 'full' if row.get('declaration') else 'symbol'
    return 'name'


def reference(rel, rows, claims, vtables, names_at=None):
    """One wrong reference: what the source names at the relocation and what the original
    reaches there, with what the symbol table says of that address."""
    addr = rel.get('exe_address')
    row = rows.get(addr) if addr else None
    sym = rel['symbol']
    out = dict(offset=rel['offset'], type=rel.get('type', ''), symbol=sym, exe_address=addr,
               detail=rel.get('detail', ''), status=rel['status'], level=row_level(row),
               name=(row or {}).get('name', ''), cls=(row or {}).get('class', ''),
               table_symbol=(row or {}).get('symbol', ''),
               declaration=(row or {}).get('declaration', ''),
               claim_symbol='', claim_file='', vtable_class=vtables.get(addr, '') if addr else '',
               sdk_symbol='')
    if addr in claims:
        out['claim_symbol'], out['claim_file'] = claims[addr]
    if sym.startswith(VALUE_PREFIXES):
        out['kind'] = 'constant'
    elif sym.startswith(('$', '.')) or 'own-section' in out['detail']:
        out['kind'] = 'label'
    elif sym.startswith('??_7') or out['vtable_class']:
        out['kind'] = 'vtable'
    elif row:
        out['kind'] = 'function'
    else:
        out['kind'] = 'data'
        if addr and names_at:
            named = sorted(n for n in names_at.get(int(addr, 16), ()) if n.startswith('?'))
            out['sdk_symbol'] = named[0] if len(named) == 1 else ''
    return out


def _placeholder(name):
    return not name or name.startswith(('tVtbl_', 'fn_', 'FUN_'))


def waits(refs, strict_only=False):
    """Why a reference-only function cannot go to a session yet, or ''. The reason starts with
    `objdiff:` (the strict check passes; nothing in the source to fix), `unnamed:` (a target has
    no symbol in the table: wait for the callee to be named) or `evidence:` (the source already
    names the symbol the table gives the target, and the scorer's evidence places that symbol
    elsewhere: for the integrator, not a session)."""
    if strict_only:
        return ('objdiff: the strict check accepts every reference and objdiff still counts a '
                'row; no source change can fix it')
    missing, agree = [], []
    for r in refs:
        if r['kind'] in ('constant', 'label'):
            continue
        if r['kind'] == 'vtable':
            if _placeholder(r['vtable_class']) and not r['table_symbol']:
                missing.append(r['exe_address'] or '?')
        elif r['kind'] == 'data':
            if not r['sdk_symbol']:
                missing.append(r['exe_address'] or '?')
        elif not (r['table_symbol'] or r['claim_symbol']):
            missing.append(r['exe_address'] or '?')
        elif r['symbol'] in (r['table_symbol'], r['claim_symbol']):
            agree.append(r['exe_address'])
    if missing:
        return 'unnamed: no symbol yet at ' + ', '.join(sorted(set(missing)))
    if agree and len(agree) == len([r for r in refs if r['kind'] not in ('constant', 'label')]):
        return ('evidence: the source names the table\'s symbol at ' +
                ', '.join(sorted(set(agree))) + ' and the scorer knows it elsewhere')
    return ''


def describe(r):
    """One line of the listing for a wrong reference."""
    return _describe(r) + (f' (scorer: {r["detail"]})' if r['detail'] and
                           r['kind'] not in ('constant', 'label') else '')


def _describe(r):
    head = (f'+0x{r["offset"]:x}: the source names `{r["symbol"]}`; the original reaches '
            f'{r["exe_address"] or "an address past the end"}')
    if r['kind'] == 'constant':
        return f'{head}, a constant: {r["detail"]}'
    if r['kind'] == 'label':
        return f'{head}, a label in the function: {r["detail"]}'
    if r['claim_symbol']:
        head += f', claimed in `{r["claim_file"]}`, which compiles to `{r["claim_symbol"]}`'
    if r['kind'] == 'vtable':
        cls = r['vtable_class']
        return head + (f', the vtable of `{cls}`' if cls and not _placeholder(cls)
                       else ', a vtable with no class name yet')
    if r['kind'] == 'data':
        return head + (f'; no table row (not a function); the SDK names it `{r["sdk_symbol"]}`'
                       if r['sdk_symbol'] else '; no table row and no name: not a function')
    what = f'; table: `{r["name"]}`' + (f', class `{r["cls"]}`' if r['cls'] else '')
    if r['level'] == 'full':
        return head + what + f', symbol `{r["table_symbol"]}`, declared `{r["declaration"]}`'
    if r['level'] == 'symbol':
        return head + what + f', symbol `{r["table_symbol"]}`, no declaration yet'
    return head + what + ', the row has only a name (no symbol yet)'


# --- one function --------------------------------------------------------------------------

def label_result(res, fdir, exe, index, rows, claims, vtables):
    """The label record of one scored function from its build folder `fdir`."""
    out = dict(address=res['address'], name=res.get('name', ''), unit=res.get('unit', ''),
               label='structural', score=float(res.get('score') or 0.0), differing=0,
               references=[], waits='', note='')
    try:
        with open(os.path.join(fdir, 'diff.txt')) as f:
            rows_ = parse_side_by_side(f.read())
    except OSError:
        out['note'] = 'no side-by-side listing'
        return out
    out['differing'] = sum(1 for r in rows_ if r[0])
    objs = glob.glob(os.path.join(os.path.dirname(fdir), '*.obj'))
    fn = index.by_address.get(int(res['address'], 16))
    if res.get('error') or not res.get('symbol') or len(objs) != 1 or fn is None or exe is None:
        out['note'] = res.get('error') or 'no object or original bytes to compare'
        return out
    try:
        obj = coff.CoffObject(open(objs[0], 'rb').read())
        t = target.build(fn.address, fn.size, obj, res['symbol'], exe, index)
    except (OSError, coff.CoffError) as e:
        out['note'] = f'cannot rebuild the comparison: {e}'
        return out
    relocs = [r.as_dict() for r in t['relocs']]
    out['label'] = classify(rows_, t['base'], t['target'], relocs)
    if out['label'] == 'reference-only':
        wrong = [r for r in relocs if r['status'] == 'mismatch']
        strict_only = not wrong
        if strict_only:   # the rows objdiff counts, by the relocations they hold
            spans = _row_spans(rows_, len(t['target']))
            held = [spans[i] for i, r in enumerate(rows_) if r[0] and i in spans]
            wrong = [r for r in relocs if any(a <= r['offset'] < b for a, b in held)]
        names_at = getattr(index, 'names_at', {})
        out['references'] = [reference(r, rows, claims, vtables, names_at) for r in wrong]
        out['waits'] = waits(out['references'], strict_only)
    return out


def listing(rec):
    """The listing for a reference-only function, as the context pack and the command show it."""
    lines = [describe(r) for r in rec['references']]
    if rec['waits']:
        lines.append(f'Parked: {rec["waits"]}.')
    return lines


def pack_section(address):
    """The context pack's near-miss section for `address`, from the build folder as it stands;
    '' when the function is not reference-only there."""
    try:
        recs = label_addresses([address])
    except Exception as e:  # a missing input must not stop the round
        return f'### Wrong references\n\n(not read: {type(e).__name__}: {e})\n'
    if not recs or recs[0]['label'] != 'reference-only':
        return ''
    from .pack import cut
    body = '\n'.join('- ' + x for x in listing(recs[0]))
    return ('### Wrong references\n\nThe instructions already match the original; only these '
            'references reach another address. Fix the declaration, overload, base class or '
            'header each one names, not the function body.\n\n' + cut(body, LISTING_MAX) + '\n')


# --- the build folder ----------------------------------------------------------------------

def results(build=None):
    """{address: (result dict, function folder)}: the newest result.json of each address."""
    build = build or paths.BUILD
    out = {}
    for p in glob.glob(os.path.join(build, '*', '0x*', 'result.json')):
        try:
            with open(p) as f:
                res = json.load(f)
            t = os.path.getmtime(p)
        except (OSError, ValueError):
            continue
        a = res.get('address')
        if a and (a not in out or t > out[a][2]):
            out[a] = (res, os.path.dirname(p), t)
    return {a: (r, d) for a, (r, d, _) in out.items()}


def _norm(a):
    return f'0x{int(a[2:] if a.lower().startswith("0x") else a, 16):08x}'


def load_inputs():
    from . import verify
    exe = verify.check_exe(verify.Report())
    index = funcs.FunctionIndex(paths.FUNCTIONS_CSV, paths.SYMBOL_TSVS)
    return exe, index, load_rows(), symtab.load_claims(), load_vtables()


def label_addresses(addresses=None, unit=None, build=None, inputs=None):
    """Label records of the non-exact functions of the build folder, sorted by address: these
    addresses, or this unit's, or every one (restricted to the claims of the last --all when its
    symbols.tsv is there, and to the matched set)."""
    found = results(build)
    if addresses:
        want = [_norm(a) for a in addresses]
    else:
        want = sorted(found)
        claimed = set(symtab.load_claims(os.path.join(build or paths.BUILD, 'symbols.tsv')))
        if claimed and not unit:
            # A claim with no symbol in symbols.tsv (one two claims compile to, say) is still a
            # claim when the matched set records it.
            from . import matched
            try:
                claimed |= set(matched.load(paths.MATCHED_SET).get('functions', {}))
            except (OSError, ValueError):
                pass
            want = [a for a in want if a in claimed]
    if unit:
        u = unit[3:] if unit.startswith('op/') else unit
        want = [a for a in want if a in found and found[a][0].get('unit') == u]
    want = [a for a in want if a in found and not found[a][0].get('exact')]
    if not want:
        return []
    exe, index, rows, claims, vtables = inputs or load_inputs()
    return [label_result(found[a][0], found[a][1], exe, index, rows, claims, vtables)
            for a in want]


# --- the stored labels ---------------------------------------------------------------------

def load_labels(path=None):
    """{address: stored row} of this lane's label file."""
    path = path or paths.NEARMISS
    try:
        with open(path, newline='') as f:
            return {r['address']: r for r in csv.DictReader(f, delimiter='\t') if r.get('address')}
    except OSError:
        return {}


def store(recs, addresses=None, everything=False, path=None, now=None):
    """Write the label records into this lane's label file under a lock. `everything` replaces
    the whole file; otherwise only the rows of `addresses` (an address with no record, i.e. exact
    or unscored now, loses its row)."""
    path = path or paths.NEARMISS
    now = now or time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime())
    os.makedirs(os.path.dirname(path), exist_ok=True)
    gone = {_norm(a) for a in (addresses or ())} | {r['address'] for r in recs}
    with open(path, 'a+', newline='') as f:
        fcntl.flock(f, fcntl.LOCK_EX)
        try:
            f.seek(0)
            old = [] if everything else [r for r in csv.DictReader(f, delimiter='\t')
                                         if r.get('address') and r['address'] not in gone]
            rows = old + [dict(r, score=f'{r["score"]:.3f}', wrong_refs=len(r['references']),
                               labelled=now) for r in recs]
            f.seek(0)
            f.truncate()
            w = csv.DictWriter(f, fieldnames=FIELDS, delimiter='\t', extrasaction='ignore',
                               lineterminator='\n')
            w.writeheader()
            w.writerows(sorted(rows, key=lambda r: r['address']))
        finally:
            fcntl.flock(f, fcntl.LOCK_UN)


# --- RULE: lines -----------------------------------------------------------------------------

RULE = re.compile(r'^\s*RULE:\s*(\S.*?)\s+->\s+(\S.*?)\s+\(for\s+(0x[0-9a-fA-F]{1,8})\)\s*$', re.M)


def rules_in(text):
    """(symptom, fix, address) of each well-formed RULE: line in a session's final message."""
    return [(s, f, _norm(a)) for s, f, a in RULE.findall(text or '')]


def collect_rules(states=None):
    """Every RULE: line of the attempt transcripts in the harness state folders, with the
    attempt it came from and the function's label when that attempt started (the loop's
    <attempt>.nearmiss.tsv), or '' when it is not known."""
    states = states or sorted(glob.glob(os.path.join(paths.WORK, 'harness', 'wt-*')))
    out = []
    for st in states:
        for p in sorted(glob.glob(os.path.join(st, 'attempts', '*.claude.json'))):
            try:
                with open(p) as f:
                    text = json.load(f).get('result') or ''
            except (OSError, ValueError, AttributeError):
                continue
            if not isinstance(text, str) or 'RULE:' not in text:
                continue
            tag = p[:-len('.claude.json')]
            labels = {}
            try:
                with open(tag + '.nearmiss.tsv', newline='') as f:
                    labels = {r['address']: r['label'] for r in csv.DictReader(f, delimiter='\t')}
            except (OSError, KeyError):
                pass
            for s, fix, a in rules_in(text):
                out.append(dict(address=a, label=labels.get(a, ''), symptom=s, fix=fix,
                                attempt=os.path.relpath(tag, os.path.dirname(st))))
    return out


# --- the command ---------------------------------------------------------------------------

def _print_human(recs):
    for r in recs:
        print(f'{r["address"]}  {r["label"]:<14} {r["score"]:7.3f}%  {r["differing"]:>4} differing'
              f'  {r["name"]}' + (f'  ({r["note"]})' if r['note'] else ''))
        if r['label'] == 'reference-only':
            for line in listing(r):
                print('    ' + line)
    counts = collections.Counter(r['label'] for r in recs)
    near = collections.Counter(r['label'] for r in recs if r['score'] >= 90.0)
    print(f'{len(recs)} not exact: ' + ', '.join(f'{k} {counts[k]}' for k in LABELS) +
          f'; at 90% or more: ' + ', '.join(f'{k} {near[k]}' for k in LABELS))
    ref = [r for r in recs if r['label'] == 'reference-only']
    why = collections.Counter(r['waits'].split(':')[0] for r in ref if r['waits'])
    print(f'reference-only: {sum(1 for r in ref if not r["waits"])} ready for a session, '
          f'{sum(why.values())} parked (' +
          ', '.join(f'{k} {why[k]}' for k in ('unnamed', 'evidence', 'objdiff')) + ')')


def main(argv):
    args = list(argv)
    fmt = 'human'
    for flag in ('--tsv', '--json'):
        if flag in args:
            args.remove(flag)
            fmt = flag[2:]
    if '--rules' in args:
        got = collect_rules()
        if fmt == 'json':
            print(json.dumps(got, indent=1))
        else:
            w = csv.writer(sys.stdout, delimiter='\t', lineterminator='\n')
            if fmt == 'tsv':
                w.writerow(('address', 'label', 'symptom', 'fix', 'attempt'))
            for r in got:
                if fmt == 'tsv':
                    w.writerow((r['address'], r['label'], r['symptom'], r['fix'], r['attempt']))
                else:
                    print(f'{r["address"]}  {r["label"] or "(label unknown)"}  '
                          f'{r["symptom"]} -> {r["fix"]}  [{r["attempt"]}]')
        return 0
    if '--parked' in args:
        for a, r in sorted(load_labels().items()):
            if r.get('label') == 'reference-only' and r.get('waits'):
                print(f'{a}  {r.get("name", "")}  {r["waits"]}')
        return 0
    keep = '--store' in args
    if keep:
        args.remove('--store')
    unit = None
    if '--unit' in args:
        i = args.index('--unit')
        if i + 1 >= len(args):
            print(__doc__)
            return 2
        unit = args[i + 1]
        del args[i:i + 2]
    if any(a.startswith('-') for a in args):
        print(__doc__)
        return 2
    try:
        recs = label_addresses(args, unit)
    except Exception as e:
        print(f'ERROR {type(e).__name__}: {e}', file=sys.stderr)
        return 2
    if keep:
        store(recs, args, everything=not args and not unit)
    if fmt == 'json':
        print(json.dumps(recs, indent=1))
    elif fmt == 'tsv':
        w = csv.DictWriter(sys.stdout, fieldnames=FIELDS[:-1], delimiter='\t',
                           extrasaction='ignore', lineterminator='\n')
        w.writeheader()
        w.writerows(dict(r, score=f'{r["score"]:.3f}', wrong_refs=len(r['references']))
                    for r in recs)
    else:
        _print_human(recs)
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
