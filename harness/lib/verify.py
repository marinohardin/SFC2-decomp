"""The build-and-verify oracle. Entry points: harness/build-and-verify.sh (read-only) and
harness/update-matched-set.sh (the only writer of matched_set.json).

    verify.py <function-id> [--json] [--behavioural] [--base <file or git ref>]
    verify.py --all [--base <file or git ref>]
    verify.py --record <function-id>

Output is a few lines, each starting with OK, ERROR, WARN or INFO, and a RESULT line. Exit
status: 0 nothing regressed; 1 a function scored below matched_set.json (or lost its source);
2 the claimed function could not be built or scored, or the inputs could not be trusted.

--base compares against another copy of the matched set as well, by original address: a file,
or a git ref whose analysis/results/matched_set.json is read with `git show` (for example
origin/develop). Every entry of the base must still be in matched_set.json at the same score or
higher, and every function is held to the higher of the two. This is how a pull request is
stopped from lowering or deleting an entry along with the source that earned it.
"""
import argparse
import hashlib
import json
import os
import subprocess
import sys
import time

from . import coff, funcs, matched, paths, source, target

EXIT_OK, EXIT_REGRESSION, EXIT_FAILED = 0, 1, 2
DIFF_ROWS = 12


class Untrusted(Exception):
    """An input the verdict depends on could not be checked; nothing is scored."""


class Report:
    def __init__(self):
        self.lines, self.results, self.regressions, self.failed = [], [], [], False

    def add(self, tag, text):
        self.lines.append(f'{tag:<5} {text}')


def check_exe(rep):
    """The original exe, after checking it against retail_hashes.sha256. A missing hash line or
    a different exe stops the run, also when HARNESS_EXE points somewhere else; only
    HARNESS_EXE_UNVERIFIED=1 lets a different exe through, with a WARN line on every run."""
    want = None
    with open(paths.HASHES) as f:
        for line in f:
            h, p = line.rstrip('\n').split('  ', 1)
            if p == paths.EXE_REL:
                want = h
    if want is None:
        raise Untrusted(f'no hash for {paths.EXE_REL} in {_shown(paths.HASHES)}')
    raw = open(paths.EXE, 'rb').read()
    got = hashlib.sha256(raw).hexdigest()
    if got != want:
        if os.environ.get('HARNESS_EXE_UNVERIFIED') != '1':
            raise Untrusted(f'{paths.EXE} has sha256 {got}, not the original exe\'s {want}')
        rep.add('WARN', f'HARNESS_EXE_UNVERIFIED=1: scoring against {paths.EXE}, which is NOT '
                'the original exe; these scores mean nothing for the matched set')
    return coff.PeImage(raw)


def load_base(spec):
    """The matched set to hold this run to as well: a file, or analysis/results/matched_set.json
    at a git ref. A ref without the file gives an empty set."""
    if os.path.isfile(spec):
        return matched.load(spec), _shown(spec)
    rel = os.path.relpath(paths.MATCHED_SET, paths.REPO)
    if rel.startswith('..'):
        raise Untrusted(f'--base {spec}: the matched set is outside the repository; give a file')
    git = ['git', '-C', paths.REPO]
    if subprocess.run(git + ['rev-parse', '--verify', '--quiet', f'{spec}^{{commit}}'],
                      capture_output=True).returncode != 0:
        raise Untrusted(f'--base {spec}: neither a file nor a git commit')
    p = subprocess.run(git + ['show', f'{spec}:{rel}'], capture_output=True, text=True)
    if p.returncode != 0:
        return matched.empty(), f'{spec} (no {rel} there)'
    try:
        return matched.parse(p.stdout, f'{spec}:{rel}'), f'{spec}:{rel}'
    except ValueError as e:
        raise Untrusted(str(e)) from None


OBJDIFF_TIMEOUT = 300   # seconds; objdiff has been seen to spin for hours on some objects


def objdiff(target_obj, base_obj, symbol, out_dir, timeout=OBJDIFF_TIMEOUT):
    out = os.path.join(out_dir, 'objdiff.json')
    try:
        p = subprocess.run([paths.OBJDIFF, 'diff', '-1', target_obj, '-2', base_obj, '-o', out,
                            '--format', 'json', symbol], capture_output=True, text=True,
                           timeout=timeout)
    except subprocess.TimeoutExpired:
        return None, None, f'objdiff did not finish in {timeout} s'
    if p.returncode != 0 or not os.path.exists(out):
        return None, None, p.stderr.strip()[-500:]
    with open(out) as f:
        d = json.load(f)
    left = next((s for s in d['left']['symbols'] if s['name'] == symbol), None)
    if left is None:
        return None, None, 'symbol missing from objdiff output'
    right = None
    if left.get('target_symbol') is not None:
        right = d['right']['symbols'][int(left['target_symbol'])]
    return left, right, ''


def _fmt(row):
    ins = (row or {}).get('instruction')
    if not ins:
        return '', ''
    return f"+0x{int(ins.get('address', 0)):x}", ins.get('formatted', '')


def diff_rows(left, right, limit=DIFF_ROWS):
    """Side-by-side rows of differing instructions: (offset, original, rebuilt, kind)."""
    rows = []
    li, ri = left.get('instructions', []), (right or {}).get('instructions', [])
    for i in range(max(len(li), len(ri))):
        lr = li[i] if i < len(li) else {}
        rr = ri[i] if i < len(ri) else {}
        kind = lr.get('diff_kind') or rr.get('diff_kind')
        if kind and kind != 'DIFF_NONE':
            off, a = _fmt(lr)
            off2, b = _fmt(rr)
            rows.append((off or off2, a, b, kind.replace('DIFF_', '').lower()))
    return rows[:limit], len(rows)


def write_side_by_side(left, right, path):
    li, ri = left.get('instructions', []), (right or {}).get('instructions', [])
    with open(path, 'w') as f:
        f.write(f"{'original (exe)':<52}| rebuilt\n")
        for i in range(max(len(li), len(ri))):
            lr = li[i] if i < len(li) else {}
            rr = ri[i] if i < len(ri) else {}
            o, a = _fmt(lr)
            _, b = _fmt(rr)
            kind = lr.get('diff_kind') or rr.get('diff_kind') or ''
            mark = ' ' if kind in ('', 'DIFF_NONE') else '*'
            f.write(f'{mark}{o:>7} {a:<43}| {b}\n')


def score_function(fn, unit, build, exe, index, symbol_hint=None, recorded=None):
    """Score one annotated function against an already compiled unit; `recorded` is its
    matched-set entry, part of the evidence for what its annotation claims. Returns a result dict."""
    res = dict(address=f'0x{fn.address:08x}', name=fn.name, unit=unit['rel'], score=0.0,
               exact=False, symbol=None, size=fn.size, error=None)
    obj = coff.CoffObject(open(build['obj'], 'rb').read())
    listing = open(build['listing'], encoding='latin1').read() if build['listing'] else ''
    try:
        sym = symbol_hint or source.symbol_for_claim(
            listing, unit['path'], unit['line'], unit['other_lines'],
            recorded=recorded, mangled=fn.mangled, name=fn.name)
    except source.SourceError as e:
        res['error'] = str(e)
        return res
    if isinstance(sym, list):
        sym = fn.mangled if fn.mangled in sym else None
        if sym is None:
            res['error'] = 'the annotation is followed by several functions on one line'
            return res
    if sym is None and fn.mangled and fn.mangled in obj.by_name:
        sym = fn.mangled
    if sym is None or sym not in obj.by_name or obj.by_name[sym].section <= 0:
        res['error'] = ('no compiled function follows the annotation (a function that is never '
                        'emitted, e.g. an unused inline, cannot be scored)')
        return res
    res['symbol'] = sym
    t = target.build(fn.address, fn.size, obj, sym, exe, index)
    fdir = os.path.join(build['out_dir'], res['address'])
    os.makedirs(fdir, exist_ok=True)
    tpath = os.path.join(fdir, 'target.obj')
    open(tpath, 'wb').write(t['object'])
    left, right, err = objdiff(tpath, build['obj'], sym, fdir)
    if left is None:
        res['error'] = f'objdiff failed: {err}'
        return res
    pct = float(left.get('match_percent') or 0.0)
    res.update(score=round(pct, 3), exact=bool(pct >= 100.0 and t['strict']),
               size=len(t['target']), rebuilt_size=len(t['base']),
               relocs=[r.as_dict() for r in t['relocs']], strict=t['strict'],
               first_diff=t['first_diff'], side_by_side=os.path.join(fdir, 'diff.txt'))
    write_side_by_side(left, right, res['side_by_side'])
    res['diff_rows'], res['diff_count'] = diff_rows(left, right)
    if pct >= 100.0 and not t['strict']:
        res['error'] = 'objdiff reports 100% but the strict byte/relocation check does not'
    with open(os.path.join(fdir, 'result.json'), 'w') as f:
        json.dump(res, f, indent=1)
    return res


def _shown(path):
    rel = os.path.relpath(path, paths.REPO)
    return path if rel.startswith('..') else rel


def _units_for(addresses, annotations, src_root):
    """Group addresses by the unit that claims them: {unit path: [(address, Annotation)]}."""
    out, missing = {}, []
    for a in addresses:
        anns = [x for x in annotations.get(a, []) if x.kind == 'FUNCTION']
        if len(anns) != 1:
            missing.append((a, len(anns)))
            continue
        out.setdefault(anns[0].path, []).append((a, anns[0]))
    return out, missing


def claimed_keys(cfg=None):
    """The matched-set keys of every address a FUNCTION annotation in game/ claims."""
    cfg = cfg or source.load_units()
    return {matched.key(a) for a, anns in source.scan_annotations(paths.SRC_ROOT, cfg['module']).items()
            if any(x.kind == 'FUNCTION' for x in anns)}


def run(ids, check_all=False, behavioural=False, record=False, now=None, base=None):
    """base: (matched set, label) to hold the run to as well, from load_base."""
    rep = Report()
    index = funcs.FunctionIndex(paths.FUNCTIONS_CSV, paths.SYMBOL_TSVS)
    cfg = source.load_units()
    data = matched.load(paths.MATCHED_SET)
    if base is not None:
        base_data, label = base
        base_data, released = matched.release_listed(
            base_data, data, matched.boundary_listed(paths.BOUNDARIES), claimed_keys(cfg))
        for k in released:
            rep.add('INFO', f'{k} dropped from matched_set.json: on the boundary-error list and '
                    f'never exact in {label}')
        for k, old in sorted(base_data['functions'].items()):
            new = data['functions'].get(k)
            if matched.worse(new, old):
                what = 'drops' if new is None else f'lowers to {new["score"]:.2f}%'
                rep.add('ERROR', f'{k} {old["name"]} REGRESSION: matched_set.json {what} the '
                        f'entry in {label} ({old["score"]:.2f}%{" exact" if old.get("exact") else ""})')
                rep.regressions.append(int(k, 16))
        rep.add('INFO', f'held to {label} as well ({len(base_data["functions"])} entries)')
        data = matched.merged(data, base_data)
    exe = None
    src_root = paths.SRC_ROOT
    annotations = source.scan_annotations(src_root, cfg['module'])

    claimed = []
    for i in ids:
        try:
            claimed.append(index.resolve(i))
        except KeyError as e:
            rep.add('ERROR', str(e.args[0]))
            rep.failed = True
    if rep.failed:
        return rep
    claimed_addrs = {fn.address for fn in claimed}

    # Units to build: those of the claimed functions, plus every unit holding a matched function
    # that shares a unit with them (or all matched functions with --all).
    recorded_addrs = {int(k, 16) for k in data['functions']}
    by_unit, missing = _units_for(claimed_addrs, annotations, src_root)
    for a, n in missing:
        fn = index.by_address[a]
        if n == 0:
            stub = any(x.kind == 'STUB' for x in annotations.get(a, []))
            what = 'is only a STUB' if stub else 'is claimed by no source file'
            rep.add('ERROR', f'0x{a:08x} {fn.name} {what} (add // FUNCTION: {cfg["module"]} '
                    f'0x{a:08x} on the line before it, under {_shown(src_root)}/)')
        else:
            rep.add('ERROR', f'0x{a:08x} {fn.name} is claimed by {n} annotations; keep one')
        rep.failed = True
    others, lost = _units_for(recorded_addrs - claimed_addrs, annotations, src_root)
    for a, _ in lost:
        if check_all:
            old = data['functions'][matched.key(a)]
            rep.add('ERROR', f'0x{a:08x} {old["name"]} REGRESSION: no source claims it any more '
                    f'(matched_set.json has {old["score"]:.2f}%)')
            rep.regressions.append(a)
    for path, items in others.items():
        if check_all or path in by_unit:
            by_unit.setdefault(path, []).extend(items)

    for path, items in sorted(by_unit.items()):
        try:
            rel, profile = source.unit_profile(cfg, src_root, path)
        except source.SourceError as e:
            rep.add('ERROR', str(e))
            rep.failed = True
            continue
        exe = exe or check_exe(rep)
        build = source.compile_unit(path, profile, src_root)
        if not build['ok']:
            errs = [ln for ln in build['log'].splitlines() if ' error ' in ln or 'fatal error' in ln]
            rep.add('ERROR', f'build {rel}: BUILD HAS FAILED. Treat this as a build failure, '
                    f'not a score. ({build["secs"]} s)')
            for ln in (errs or build['log'].splitlines())[:10]:
                rep.add('ERROR', '  ' + ln.strip())
            for a, _ in items:
                if a in recorded_addrs:
                    rep.regressions.append(a)
                    rep.add('ERROR', f'0x{a:08x} REGRESSION: unit no longer builds')
            rep.failed = rep.failed or any(a in claimed_addrs for a, _ in items)
            continue
        rep.add('OK', f'build {rel} ({build["secs"]} s, profile {profile["name"]})')
        ann_lines = [x.line for lst in annotations.values() for x in lst if x.path == path]
        for a, ann in sorted(items, key=lambda x: x[0]):
            fn = index.by_address[a]
            unit = dict(rel=rel, path=path, line=ann.line,
                        other_lines=[n for n in ann_lines if n != ann.line])
            res = score_function(fn, unit, build, exe, index,
                                 recorded=data['functions'].get(matched.key(a)))
            res['claimed'] = a in claimed_addrs
            rep.results.append(res)
            _report_function(rep, data, fn, res)

    report_shared_symbols(rep)

    if behavioural:
        _behavioural(rep, claimed)
    if record and not rep.regressions and not rep.failed:
        now = now or time.strftime('%Y-%m-%d')
        changed = False
        for res in rep.results:
            if res['claimed'] and not res['error']:
                entry = {k: res[k] for k in ('name', 'symbol', 'unit', 'size', 'score', 'exact')}
                if matched.record(data, int(res['address'], 16), entry, now):
                    changed = True
                    rep.add('OK', f'{res["address"]} recorded in matched_set.json at '
                            f'{res["score"]:.2f}%')
                else:
                    rep.add('INFO', f'{res["address"]} not recorded: no improvement')
        if changed:
            matched.save(paths.MATCHED_SET, data)
    elif record:
        rep.add('INFO', 'matched_set.json not written: fix the errors above first')
    return rep


PER_UNIT_PREFIXES = ('$', '_$', '__real@', '??_C@')


def shared_symbol_conflicts(results):
    """Error lines for a symbol that exact functions use at two exe addresses. The native build
    links one definition per name, so the object lift refuses such a pair; it means one of the
    functions names the wrong callee, global or method, which the byte match cannot see. Labels,
    compiler statics, literal constants and any symbol the object marks static (a `static`
    function or variable, which two units may define under one name) are resolved per unit, so
    they are compared only within one."""
    seen = {}
    for res in results:
        if not res.get('exact'):
            continue
        for r in res.get('relocs') or []:
            name, addr = r['symbol'], r['exe_address']
            if addr is None or r['status'] == 'mismatch':
                continue
            per_unit = name.startswith(PER_UNIT_PREFIXES) or r.get('file_local')
            key = (res['unit'], name) if per_unit else ('', name)
            seen.setdefault(key, {}).setdefault(addr, set()).add(res['address'])
    out = []
    for (_, name), uses in sorted(seen.items()):
        if len(uses) > 1:
            where = '; '.join(f'{a} by {", ".join(sorted(fns))}' for a, fns in sorted(uses.items()))
            out.append(f'{name} is used at more than one exe address by exact functions: {where} '
                       '(one of them names the wrong symbol)')
    return out


def report_shared_symbols(rep):
    """An ERROR line, and a failed run, for each symbol shared_symbol_conflicts finds."""
    for line in shared_symbol_conflicts(rep.results):
        rep.add('ERROR', line)
        rep.failed = True


def _report_function(rep, data, fn, res):
    a = fn.address
    head = f'{res["address"]} {fn.name}'
    if res['error'] and 'relocs' not in res:
        rep.add('ERROR', f'{head}: {res["error"]}')
        if res['claimed']:
            rep.failed = True
        if matched.recorded(data, a):
            rep.regressions.append(a)
        return
    old = matched.recorded(data, a)
    bad_relocs = [r for r in res.get('relocs', []) if r['status'] == 'mismatch']
    unverified = sum(1 for r in res.get('relocs', []) if r['status'] == 'unverified')
    state = 'exact' if res['exact'] else 'partial'
    detail = (f'{res["score"]:.2f}% {state} ({res["size"]} bytes original, '
              f'{res["rebuilt_size"]} rebuilt; {len(res["relocs"])} relocations, '
              f'{unverified} unverified)')
    if matched.is_regression(data, a, res['score']) or (old and old.get('exact') and not res['exact']):
        rep.regressions.append(a)
        rep.add('ERROR', f'{head} {detail} REGRESSION: matched_set.json has {old["score"]:.2f}%')
    elif res['error']:
        rep.add('ERROR', f'{head} {detail}: {res["error"]}')
        rep.failed = rep.failed or res['claimed']
    else:
        was = f'; matched_set.json has {old["score"]:.2f}%' if old else '; not in matched_set.json'
        rep.add('OK', f'{head} {detail}{was}')
    for r in bad_relocs[:6]:
        addr = r['exe_address'] or '?'
        rep.add('ERROR', f'  relocation +0x{r["offset"]:x} {r["symbol"]}: exe uses {addr}, '
                f'{r["detail"]}')
    if not res['exact'] and res['claimed']:
        for off, a_, b_, kind in res['diff_rows']:
            rep.add('INFO', f'  {off:>7} {kind:<8} original: {a_:<36} rebuilt: {b_}')
        if res['diff_count'] > len(res['diff_rows']):
            rep.add('INFO', f'  ... {res["diff_count"] - len(res["diff_rows"])} more differing '
                    f'instructions')
        rep.add('INFO', f'  side by side: {res["side_by_side"]}')


def _behavioural(rep, claimed):
    """Hook for the replay oracle (oracle/). Switched off until it records traces: a later task
    sets HARNESS_ORACLE to a command taking the function address and exiting 0 on a matching
    trace."""
    cmd = os.environ.get('HARNESS_ORACLE')
    if not cmd:
        rep.add('INFO', 'behavioural: oracle not available (the replay oracle does not record '
                'traces yet; set HARNESS_ORACLE once it does)')
        return
    for fn in claimed:
        p = subprocess.run([cmd, f'0x{fn.address:08x}'], capture_output=True, text=True)
        if p.returncode == 0:
            rep.add('OK', f'behavioural 0x{fn.address:08x}: replay matches')
        else:
            rep.add('ERROR', f'behavioural 0x{fn.address:08x}: replay differs '
                    f'({(p.stdout + p.stderr).strip()[-200:]})')
            rep.regressions.append(fn.address)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('ids', nargs='*', help='function address (0x006fc690), fn_/FUN_ id or name')
    ap.add_argument('--all', action='store_true', help='re-verify every function in matched_set.json')
    ap.add_argument('--behavioural', action='store_true', help='also run the replay oracle')
    ap.add_argument('--base', metavar='FILE_OR_REF',
                    help='also hold every function to this matched set (a file or a git ref)')
    ap.add_argument('--record', action='store_true', help=argparse.SUPPRESS)
    ap.add_argument('--json', action='store_true', help='print the result as JSON')
    a = ap.parse_args(argv)
    if not a.ids and not a.all:
        ap.error('give a function id, or --all')
    if a.record and a.base:
        ap.error('--base is for checking; it cannot be combined with recording')
    os.makedirs(paths.BUILD, exist_ok=True)
    try:
        base = load_base(a.base) if a.base else None
        rep = run(a.ids, check_all=a.all, behavioural=a.behavioural, record=a.record, base=base)
    except Untrusted as e:
        rep = Report()
        rep.add('ERROR', str(e))
        rep.failed = True
    if rep.regressions:
        status, code = 'ERROR regression', EXIT_REGRESSION
    elif rep.failed:
        status, code = 'ERROR not scored', EXIT_FAILED
    else:
        status, code = 'OK', EXIT_OK
    if a.json:
        print(json.dumps(dict(status=status, exit=code, lines=rep.lines, results=[
            {k: v for k, v in r.items() if k not in ('diff_rows',)} for r in rep.results]),
            indent=1))
    else:
        print('\n'.join(rep.lines))
        print(f'RESULT {status}')
    return code


if __name__ == '__main__':
    sys.exit(main())
