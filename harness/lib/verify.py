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
import csv
import hashlib
import io
import json
import os
import subprocess
import threading
import sys
import time

from . import cache, clean, coff, funcs, matched, paths, source, symtab, target

EXIT_OK, EXIT_REGRESSION, EXIT_FAILED = 0, 1, 2
DIFF_ROWS = 12
# Relocations of exact functions that name another symbol than the claim at their target, known
# on develop when the check came in; only breaks not listed here fail (report_claim_symbols).
RELOC_BASELINE = paths.env_path('HARNESS_RELOC_BASELINE', os.path.join(
    paths.REPO, 'analysis', 'results', 'reloc_baseline.tsv'))
RELOC_BASELINE_FIELDS = ('function', 'offset', 'symbol', 'target', 'claimed')
# A relocation that names another symbol than the claim at its target is of one class
# (classify_break): the first three fail the run, the last two are listed only.
CLASS_ERRORS = ('other-object', 'other-address', 'wrong-function')
CLASS_WARNS = ('own-object', 'folded', 'stand-in')


class Untrusted(Exception):
    """An input the verdict depends on could not be checked; nothing is scored."""


class Report:
    def __init__(self):
        self.lines, self.results, self.regressions, self.failed = [], [], [], False
        self.claim_breaks = []
        self.standins = []
        self.objects = {}      # unit (as in a result's `unit`) -> its object file, for step 8

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
    img = coff.PeImage(raw)
    img.sha256 = got        # the score cache keys a score to the exe it was scored against
    return img


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


def function_sizes(index):
    """Matched-set key -> size for every function in a FunctionIndex."""
    return {matched.key(a): f.size for a, f in index.by_address.items()}


def joined_keys(index):
    """Matched-set keys of the functions ghidra/scripts/join_eh_prologs.py joined."""
    return {matched.key(a) for a, f in index.by_address.items()
            if 'eh-prolog-joined' in getattr(f, 'tags', ())}


def load_base_sizes(spec):
    """The sizes in functions.csv at the git ref spec (matched-set key -> size), or None when spec
    is a file or the ref has no functions.csv: then no entry is released as reshaped."""
    if os.path.isfile(spec):
        return None
    rel = os.path.relpath(paths.FUNCTIONS_CSV, paths.REPO)
    if rel.startswith('..'):
        return None
    p = subprocess.run(['git', '-C', paths.REPO, 'show', f'{spec}:{rel}'], capture_output=True,
                       text=True)
    if p.returncode != 0:
        return None
    rows = csv.DictReader(io.StringIO(p.stdout))
    return {matched.key(int(r['address'], 16)): int(r['size'] or 0) for r in rows}


# Units built at once. Each build is a single-threaded compiler in a container capped at one CPU
# (toolchain/bin/msvc-run), then one objdiff per function, so this is about the cores a run uses.
DEFAULT_JOBS = 4


def default_jobs():
    """HARNESS_JOBS if set to a positive number, else DEFAULT_JOBS."""
    try:
        n = int(os.environ.get('HARNESS_JOBS', ''))
    except ValueError:
        return DEFAULT_JOBS
    return n if n > 0 else DEFAULT_JOBS


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
    # Nothing reads the report again: the score, the diff rows and diff.txt are made from it
    # here, and it is about 1 MB a function.
    os.remove(out)
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


# A score cache entry leaves these to the function being scored (cache.py).
CACHE_OWN_FIELDS = ('address', 'name', 'unit', 'side_by_side')


def score_function(fn, unit, build, exe, index, symbol_hint=None, recorded=None):
    """Score one annotated function against an already compiled unit; `recorded` is its
    matched-set entry, part of the evidence for what its annotation claims. Returns a result dict."""
    res = dict(address=f'0x{fn.address:08x}', name=fn.name, unit=unit['rel'], score=0.0,
               exact=False, symbol=None, size=fn.size, error=None)
    obj = coff.CoffObject(open(build['obj'], 'rb').read())
    listing = open(build['listing'], encoding='latin1').read() if build['listing'] else ''
    try:
        sym = symbol_hint or source.symbol_for_claim(
            listing, unit.get('file', unit['path']), unit['line'], unit['other_lines'],
            recorded=recorded, mangled=fn.mangled, name=fn.name, unit=unit['path'])
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
    fdir = os.path.join(build['out_dir'], res['address'])
    os.makedirs(fdir, exist_ok=True)
    # The score cache (harness/lib/cache.py): an unchanged object is not compared again.
    skey = cache.score_key(build['obj'], sym, fn.address, fn.size, exe)
    hit = cache.load_score(skey)
    if hit is not None:
        res.update({k: v for k, v in hit['result'].items() if k not in CACHE_OWN_FIELDS})
        res['diff_rows'] = [tuple(r) for r in res.get('diff_rows') or []]
        res['side_by_side'] = os.path.join(fdir, 'diff.txt')
        with open(res['side_by_side'], 'w') as f:
            f.write(hit['side_by_side'])
        with open(os.path.join(fdir, 'result.json'), 'w') as f:
            json.dump(res, f, indent=1)
        return res
    t = target.build(fn.address, fn.size, obj, sym, exe, index)
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
    with open(res['side_by_side']) as f:
        side = f.read()
    # What the comparison found, without what names the function here: a hit may come from
    # another unit or name with the same object, symbol, address and size.
    cache.store_score(skey, {'result': {k: v for k, v in json.loads(json.dumps(res)).items()
                                        if k not in CACHE_OWN_FIELDS},
                             'side_by_side': side})
    return res


def _shown(path):
    rel = os.path.relpath(path, paths.REPO)
    return path if rel.startswith('..') else rel


def _units_for(addresses, annotations, src_root, units_tsv=None):
    """Group addresses by the unit that compiles their claim (source.claim_unit: the
    annotation's own file, or for one in a header the unit op/UNITS.tsv assigns):
    ({unit path: [(address, Annotation)]}, [(address, number of annotations)] for an address
    claimed by none or several, [(address, error)] for a header claim with no unit)."""
    out, missing, unplaced = {}, [], []
    for a in addresses:
        anns = [x for x in annotations.get(a, []) if x.kind == 'FUNCTION']
        if len(anns) != 1:
            missing.append((a, len(anns)))
            continue
        try:
            unit = source.claim_unit(anns[0], src_root, units_tsv)
        except source.SourceError as e:
            unplaced.append((a, str(e)))
            continue
        out.setdefault(unit, []).append((a, anns[0]))
    return out, missing, unplaced


def claimed_keys(cfg=None):
    """The matched-set keys of every address a FUNCTION annotation in op/ claims."""
    cfg = cfg or source.load_units()
    return {matched.key(a) for a, anns in source.scan_annotations(paths.SRC_ROOT, cfg['module']).items()
            if any(x.kind == 'FUNCTION' for x in anns)}



def _in_threads(fn, items, jobs):
    """fn applied to every item, at most `jobs` at once, results in the items' order. An exception
    in any call is raised once all calls have ended. Plain threads, not concurrent.futures, whose
    `import queue` can pick up harness/queue.py when harness/ is on sys.path."""
    items = list(items)
    # A run interrupted earlier in this process may have left compiles stopped (a worker of it
    # outlived the wait); this run starts them again.
    source.allow_compiles()
    results = [None] * len(items)
    errors = [None] * len(items)
    pending = iter(range(len(items)))
    lock = threading.Lock()
    stop = threading.Event()

    def worker():
        while not stop.is_set():
            with lock:
                i = next(pending, None)
            if i is None:
                return
            try:
                results[i] = fn(items[i])
            except BaseException as e:  # re-raised below, in the caller's thread
                errors[i] = e

    threads = [threading.Thread(target=worker, daemon=True)
               for _ in range(max(1, min(jobs, len(items))))]
    for t in threads:
        t.start()
    try:
        for t in threads:
            t.join()
    except BaseException:
        # Ctrl-C reaches only this thread; the compiles run in their own process groups.
        stop.set()
        source.stop_all()
        # Wait for the workers (their compiles are stopped, and none starts another) before
        # compiles are allowed again; polled, since join is where the interrupt arrives.
        deadline = time.time() + 60
        while any(t.is_alive() for t in threads) and time.time() < deadline:
            time.sleep(0.05)
        if not any(t.is_alive() for t in threads):
            source.allow_compiles()
        raise
    for e in errors:
        if e is not None:
            raise e
    return results

def run(ids, check_all=False, behavioural=False, record=False, now=None, base=None,
        base_sizes=None, jobs=None):
    """base: (matched set, label) to hold the run to as well, from load_base; base_sizes: the
    base's functions.csv sizes, from load_base_sizes, or None to release nothing for them.
    jobs: how many units build at once (default HARNESS_JOBS, else DEFAULT_JOBS)."""
    jobs = jobs or default_jobs()
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
        base_data, misscored = matched.release_misscored(base_data, data, claimed_keys(cfg))
        for k in misscored:
            rep.add('INFO', f'{k} dropped from matched_set.json: its entry in {label} was scored '
                    f'against the plain destructor, not the deleting destructor it names')
        if base_sizes is not None:
            base_data, reshaped = matched.release_reshaped(base_data, base_sizes, function_sizes(
                index), joined_keys(index))
            for k in reshaped:
                rep.add('INFO', f'{k} not held to {label}: the analysis joined its exception '
                        f'prolog and body (its size differs from the base\'s) and it was never exact there')
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
    units_tsv = paths.UNITS_TSV
    by_unit, missing, unplaced = _units_for(claimed_addrs, annotations, src_root, units_tsv)
    for a, err in unplaced:
        rep.add('ERROR', f'0x{a:08x} {index.by_address[a].name}: {err}')
        rep.failed = True
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
    others, lost, unplaced = _units_for(recorded_addrs - claimed_addrs, annotations, src_root,
                                        units_tsv)
    for a, err in unplaced:
        if check_all:
            old = data['functions'][matched.key(a)]
            rep.add('ERROR', f'0x{a:08x} {old["name"]} REGRESSION: {err}')
            rep.regressions.append(a)
    for a, _ in lost:
        if check_all:
            old = data['functions'][matched.key(a)]
            rep.add('ERROR', f'0x{a:08x} {old["name"]} REGRESSION: no source claims it any more '
                    f'(matched_set.json has {old["score"]:.2f}%)')
            rep.regressions.append(a)
    for path, items in others.items():
        if check_all or path in by_unit:
            by_unit.setdefault(path, []).extend(items)
    if not check_all and by_unit:
        # A header (or any file) changed since HEAD can break units that are not the claimed
        # function's: rebuild every unit with a recorded function that reads a changed file, so
        # a regression there counts as one here.
        listed = source.changed_files(src_root)
        if listed is None:
            rep.add('WARN', 'git could not list the files changed since HEAD, so units other than '
                    'the claimed one are not rebuilt: a header edit that breaks them goes unseen')
            listed = set()
        changed = {f for f in listed
                   if f not in {os.path.normpath(os.path.realpath(p)) for p in by_unit}}
        readers = []
        if changed:
            for path, items in sorted(others.items()):
                if path in by_unit:
                    continue
                try:
                    hit = source.reads_any(cfg, src_root, path, changed)
                except source.SourceError:
                    hit = True
                if hit:
                    by_unit[path] = list(items)
                    readers.append(os.path.relpath(path, src_root).replace(os.sep, '/'))
        if readers:
            shown = ', '.join(readers[:8]) + (f' and {len(readers) - 8} more' if len(readers) > 8 else '')
            rep.add('INFO', f'{len(readers)} other units read a file changed since HEAD and are '
                    f'rebuilt to check their recorded functions: {shown}')

    lines_in = {}
    for lst in annotations.values():
        for x in lst:
            lines_in.setdefault(x.path, []).append(x.line)
    if by_unit:
        exe = check_exe(rep)

    def build_and_score(job):
        # One unit: compile it and score its functions, into a report of its own so units can
        # run side by side; the reports are joined in unit order, so the output does not depend
        # on how many run at once.
        path, items = job
        sub = Report()
        try:
            rel, profile = source.unit_profile(cfg, src_root, path)
        except source.SourceError as e:
            sub.add('ERROR', str(e))
            sub.failed = True
            return sub
        build = source.compile_unit(path, profile, src_root, cl=paths.HARNESS_CL)
        if not build['ok']:
            errs = [ln for ln in build['log'].splitlines() if ' error ' in ln or 'fatal error' in ln]
            sub.add('ERROR', f'build {rel}: BUILD HAS FAILED. Treat this as a build failure, '
                    f'not a score. ({build["secs"]} s)')
            for ln in (errs or build['log'].splitlines())[:10]:
                sub.add('ERROR', '  ' + ln.strip())
            for a, _ in items:
                if a in recorded_addrs:
                    sub.regressions.append(a)
                    sub.add('ERROR', f'0x{a:08x} REGRESSION: unit no longer builds')
            sub.failed = any(a in claimed_addrs for a, _ in items)
            return sub
        how = 'unchanged, not rebuilt' if build.get('cached') else f'{build["secs"]} s'
        sub.add('OK', f'build {rel} ({how}, profile {profile["name"]})')
        sub.objects[rel] = build['obj']
        for a, ann in sorted(items, key=lambda x: x[0]):
            fn = index.by_address[a]
            # The other annotations in the annotation's own file: the unit, or the header it is in.
            unit = dict(rel=rel, path=path, file=ann.path, line=ann.line,
                        other_lines=[n for n in lines_in[ann.path] if n != ann.line])
            res = score_function(fn, unit, build, exe, index,
                                 recorded=data['functions'].get(matched.key(a)))
            res['claimed'] = a in claimed_addrs
            sub.results.append(res)
            _report_function(sub, data, fn, res)
        return sub

    for sub in _in_threads(build_and_score, sorted(by_unit.items()), jobs):
        rep.lines.extend(sub.lines)
        rep.results.extend(sub.results)
        rep.regressions.extend(sub.regressions)
        rep.objects.update(sub.objects)
        rep.failed = rep.failed or sub.failed

    report_shared_symbols(rep)
    report_claim_symbols(rep, data, load_reloc_baseline(RELOC_BASELINE), check_all,
                         symtab.load_claims(), index)

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


def exact_reloc_uses(results):
    """(symbol, exe address text, per-unit?, unit, caller address, relocation) for each resolved
    relocation of an exact function: one whose target address is known and whose bytes match.
    A per-unit symbol is a label, compiler static, literal constant or one the object marks
    static, which two units may define under one name."""
    for res in results:
        if not res.get('exact'):
            continue
        for r in res.get('relocs') or []:
            name, addr = r['symbol'], r['exe_address']
            if addr is None or r['status'] == 'mismatch':
                continue
            per_unit = name.startswith(PER_UNIT_PREFIXES) or bool(r.get('file_local'))
            yield name, addr, per_unit, res['unit'], res['address'], r


def shared_symbol_conflicts(results):
    """Error lines for a symbol that exact functions use at two exe addresses. The native build
    links one definition per name, so the object lift refuses such a pair; it means one of the
    functions names the wrong callee, global or method, which the byte match cannot see. Labels,
    compiler statics, literal constants and any symbol the object marks static (a `static`
    function or variable, which two units may define under one name) are resolved per unit, so
    they are compared only within one."""
    seen = {}
    for name, addr, per_unit, unit, caller, _ in exact_reloc_uses(results):
        key = (unit, name) if per_unit else ('', name)
        seen.setdefault(key, {}).setdefault(addr, set()).add(caller)
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


CLAIM_RELOC_TYPES = ('REL32', 'DIR32')


def write_symbol_table(rep, out=None):
    """The claims' rows from an --all run. A claim that could not be built or scored (an error
    and no symbol) keeps its row from the table the run replaces; any other claim without a
    symbol, and any address no longer claimed, loses it."""
    out = out or symtab.path()
    rows = symbol_table_rows(rep.results)
    unbuilt = {r['address'] for r in rep.results if r.get('error') and not r.get('symbol')}
    seen = {r['address'] for r in rep.results}
    # A failed run may also have left claims without any result (a unit that aborted); their
    # rows stay too. A row whose claim is gone is ignored when read (claimed_symbols).
    rows += [(a, sym, where) for a, (sym, where) in symtab.load_claims(out).items()
             if a in unbuilt or (rep.failed and a not in seen)]
    symtab.write_claims(out, rows)


def symbol_table_rows(results):
    """(address, symbol, unit) of each claim this run rebuilt, for harness/lib/symtab.py. A
    compiler static (resolved per unit) or a symbol two claims compile to names no one target,
    so neither is a row."""
    rows = [(r['address'], r['symbol'], r['unit']) for r in results
            if r.get('symbol') and not r['symbol'].startswith(PER_UNIT_PREFIXES)]
    count = {}
    for _, sym, _ in rows:
        count[sym] = count.get(sym, 0) + 1
    return [r for r in rows if count[r[1]] == 1]


def claimed_symbols(results, data, table=None):
    """Matched-set key -> (symbol, source) for every claimed function: the symbol this run's
    rebuild of the claiming unit defines for it ('rebuilt'), else the one the last --all in this
    checkout rebuilt (`table`, harness/lib/symtab.py: 'table'), else the `symbol` field of its
    matched-set entry ('recorded'). A rebuild wins because a recorded field can be stale: it is
    written only when a score improves, so a later rename leaves the old name there."""
    out = {k: (e['symbol'], 'recorded') for k, e in data['functions'].items() if e.get('symbol')}
    for k, (sym, _) in (table or {}).items():
        if k in data['functions']:      # a row whose claim is gone since the table was written
            out[k] = (sym, 'table')
    for res in results:
        if res.get('symbol'):
            out[res['address']] = (res['symbol'], 'rebuilt')
    return out


def claim_symbol_breaks(results, claims, exact_only=True):
    """Each REL32/DIR32 relocation of an exact function that reaches a claimed function's
    address but names another symbol than the claim defines there. The byte match masks the
    field, so the function scores exact, but a consumer that links the exact objects (manifest
    mode vc6) is left with an unresolved external. Targets nothing claims (library and CRT code
    the decomposition does not rebuild) are not checked."""
    out = []
    for res in results:
        if exact_only and not res.get('exact'):
            continue
        for r in res.get('relocs') or []:
            if (r.get('type') not in CLAIM_RELOC_TYPES or r['exe_address'] is None
                    or r['status'] == 'mismatch'):
                continue
            claim = claims.get(r['exe_address'])
            if claim is None or claim[0] == r['symbol']:
                continue
            out.append(dict(function=res['address'], offset=r['offset'], symbol=r['symbol'],
                            target=r['exe_address'], claimed=claim[0], source=claim[1]))
    return out


def _break_key(b):
    """A break is known by function, symbol and target: the offset moves with any edit above."""
    return (b['function'], b['symbol'], b['target'])


def load_reloc_baseline(path):
    """The rows of the relocation baseline (RELOC_BASELINE_FIELDS), or none without the file. A
    malformed row stops the run with the file and line."""
    if not os.path.exists(path):
        return []
    out = []
    with open(path, newline='') as f:
        rows = csv.reader(f, delimiter='\t')
        for n, row in enumerate(rows, 1):
            if not row or row[0].startswith('#') or (n == 1 and row[0] == 'function'):
                continue
            # (a sixth column is the class a reloc_breaks.tsv row carries; it is not read)
            if len(row) not in (len(RELOC_BASELINE_FIELDS), len(RELOC_BASELINE_FIELDS) + 1):
                raise Untrusted(f'{_shown(path)}:{n}: expected {len(RELOC_BASELINE_FIELDS)} '
                                f'tab-separated fields ({", ".join(RELOC_BASELINE_FIELDS)})')
            b = dict(zip(RELOC_BASELINE_FIELDS, row[:len(RELOC_BASELINE_FIELDS)]))
            try:
                b['offset'] = int(b['offset'], 16)
            except ValueError:
                raise Untrusted(f'{_shown(path)}:{n}: offset {b["offset"]!r} is not hexadecimal') from None
            out.append(b)
    return out


def reloc_baseline_text(breaks, classes=False):
    """breaks in the baseline's format, sorted, one row per break; `classes` adds the column
    naming each break's class (the reloc_breaks.tsv an --all run writes)."""
    rows = sorted({(b['function'], f'0x{b["offset"]:x}', b['symbol'], b['target'], b['claimed'])
                   + ((b['class'],) if classes else ()) for b in breaks})
    head = RELOC_BASELINE_FIELDS + (('class',) if classes else ())
    return ''.join('\t'.join(r) + '\n' for r in [head] + rows)


def defined_symbols(objects):
    """{symbol: set of units} for every external symbol the unit objects define (a function,
    or an inline or template instance a unit emitted). `objects` maps a unit to its object file."""
    out = {}
    for unit, path in objects.items():
        try:
            with open(path, 'rb') as f:
                obj = coff.CoffObject(f.read())
        except (OSError, coff.CoffError):
            continue
        for s in obj.symbols.values():
            if s.section > 0 and s.storage == coff.SYM_CLASS_EXTERNAL:
                out.setdefault(s.name, set()).add(unit)
    return out


def startup_breaks():
    """The keys (_break_key) of the breaks the last `--all` in this checkout wrote to
    reloc_breaks.tsv (the loop runs one at its start), or None without that file."""
    try:
        with open(os.path.join(paths.BUILD, 'reloc_breaks.tsv'), newline='') as f:
            return {(r['function'], r['symbol'], r['target'])
                    for r in csv.DictReader(f, delimiter='\t')}
    except (OSError, KeyError):
        return None


def classify_break(b, unit, defined, evidence):
    """(class, detail) of a claim_symbol_breaks row: how the consumer that links the exact
    objects resolves the call, which names the symbol S, to the target T (harness/README.md,
    step 8). An object defining S wins (the consumer binds by name), then the exe's evidence
    names (functions.csv and the SDK tables), then the address in the caller's original bytes.
    `unit` is the caller's; `defined` is defined_symbols; `evidence` has `known` (name ->
    addresses) and `names_at` (address -> names), like funcs.FunctionIndex."""
    sym, target = b['symbol'], int(b['target'], 16)
    other = sorted(defined.get(sym, set()) - {unit})
    if other:
        return 'other-object', other[0]
    known, names_at = getattr(evidence, 'known', {}), getattr(evidence, 'names_at', {})
    elsewhere = sorted(a for a in known.get(sym, ()) if a != target)
    if elsewhere:
        return 'other-address', f'0x{elsewhere[0]:08x}'
    names = names_at.get(target, set())
    if names - {sym}:
        if sym in names:
            return 'folded', ', '.join(sorted(names))
        return 'wrong-function', sorted(names)[0]
    if unit in defined.get(sym, ()):
        return 'own-object', ''
    return 'stand-in', ''


CLASS_TEXT = {
    'other-object': 'it is defined in another object ({d}): a consumer would bind the call to '
                    'that body',
    'other-address': 'the evidence has it at another address ({d}): a consumer would bind the '
                     'call there',
    'wrong-function': 'it names another function than the one at this address ({d})',
}


def report_claim_symbols(rep, data, baseline, check_all, table=None, evidence=None):
    """Lines for claim_symbol_breaks, each sorted into a class by classify_break, from the unit
    objects the run built (rep.objects) and `evidence` (the funcs.FunctionIndex). A break of an
    error class (CLASS_ERRORS) fails the run when the claim's symbol comes from this run's
    rebuild, unless the baseline lists it (one INFO line for all of them); when the claim's
    symbol comes only from the matched set (a single-function run that did not build the
    target's unit), it is a WARN line, since the recorded field may be stale and --all rebuilds
    every claim. A break of a warning class (CLASS_WARNS) only adds to one summary WARN line per
    class. A baseline row whose break is gone, or under --all no longer an error, is reported as
    stale, so the list only shrinks: under --all every row, otherwise only rows of the exact
    functions scored. A single-function run knows only the objects it built, so it can miss a
    definition in another unit; --all decides."""
    claims = claimed_symbols(rep.results, data, table)
    rep.claim_breaks = claim_symbol_breaks(rep.results, claims)
    listed = {_break_key(b) for b in baseline}
    units = {res['address']: res['unit'] for res in rep.results}
    exact = {res['address'] for res in rep.results if res.get('exact')}
    scored = claim_symbol_breaks(rep.results, claims, exact_only=False)
    defined = defined_symbols(rep.objects) if scored else {}
    for b in scored + rep.claim_breaks:
        b['class'], b['detail'] = classify_break(b, units.get(b['function']), defined, evidence)
    # Stand-ins: any scored function, exact or not, that reaches a claimed function through
    # another symbol than the claim's, judged only against rebuilt symbols (this run's or the
    # table's), never a recorded field that may be stale. The Stop hook refuses these. An exact
    # function's warning-class break is not one, and is only counted below, unless it is a
    # `stand-in` the startup --all (reloc_breaks.tsv) did not have: the session made it.
    startup = startup_breaks()

    def tolerated_warning(b):
        if b['function'] not in exact or b['class'] not in CLASS_WARNS:
            return False
        return startup is None or b['class'] != 'stand-in' or _break_key(b) in startup
    rep.standins = [b for b in scored
                    if b['source'] in ('rebuilt', 'table') and _break_key(b) not in listed
                    and not tolerated_warning(b)]
    errors = [b for b in rep.claim_breaks if b['class'] in CLASS_ERRORS]
    reported = {_break_key(b) for b in errors}
    if not check_all:
        for b in rep.standins:
            if _break_key(b) in reported:
                continue
            rep.add('WARN', f'stand-in: {b["function"]} reaches {b["target"]} as {b["symbol"]}, '
                    f'but {b["target"]} is claimed as {b["claimed"]}; declare it as its claim '
                    'does (the class and header that define it), not again under another name')
    tolerated = 0
    for b in errors:
        if _break_key(b) in listed:
            tolerated += 1
            continue
        text = (f'{b["function"]} relocation +0x{b["offset"]:x} names {b["symbol"]}, which '
                f'reaches {b["target"]}, claimed as {b["claimed"]}: '
                + CLASS_TEXT[b['class']].format(d=b['detail']))
        if b['source'] == 'rebuilt':
            rep.add('ERROR', text)
            rep.failed = True
        elif b['source'] == 'table':
            rep.add('WARN', text + ' (the claim is from the symbol table, not rebuilt in this '
                    'run; the table is the last --all\'s; --all decides)')
        else:
            rep.add('WARN', text + ' (the claim is from matched_set.json, not rebuilt in this '
                    'run; the recorded symbol may be stale; --all decides)')
    if tolerated:
        rep.add('INFO', f'{tolerated} relocations name another symbol than the claim at their '
                f'target, as {_shown(RELOC_BASELINE)} lists')
    for cls, what in (
            ('stand-in', 'a stand-in no other object defines and no evidence places elsewhere: '
                         'resolved by name or by address'),
            ('folded', 'a symbol the evidence gives the target together with other names (bodies '
                       'the original folded into one address): resolved by name'),
            ('own-object', 'a symbol their own object defines (an inline or template instance): '
                           'unresolved, whether a consumer binds it to that body is open')):
        n = sum(1 for b in rep.claim_breaks if b['class'] == cls)
        if n:
            rep.add('WARN', f'{n} relocations of exact functions name {what} (class {cls}); '
                    '--all lists them in reloc_breaks.tsv')
    now = {_break_key(b): b['class'] for b in rep.claim_breaks}
    scored_fns = {res['address'] for res in rep.results if res.get('exact')}
    for b in baseline:
        if not (check_all or b['function'] in scored_fns):
            continue
        cls = now.get(_break_key(b))
        if cls is None:
            rep.add('WARN', f'{_shown(RELOC_BASELINE)} lists {b["function"]} +0x{b["offset"]:x} '
                    f'{b["symbol"]} -> {b["target"]}, which no longer breaks: remove the row')
        elif check_all and cls not in CLASS_ERRORS:
            rep.add('WARN', f'{_shown(RELOC_BASELINE)} lists {b["function"]} +0x{b["offset"]:x} '
                    f'{b["symbol"]} -> {b["target"]}, which is now only a warning ({cls}): '
                    'remove the row')


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
    ap.add_argument('--jobs', '-j', type=int, metavar='N',
                    help=f'units to build at once (default HARNESS_JOBS, else {DEFAULT_JOBS})')
    a = ap.parse_args(argv)
    if not a.ids and not a.all:
        ap.error('give a function id, or --all')
    if a.jobs is not None and a.jobs < 1:
        ap.error('--jobs must be 1 or more')
    if a.record and a.base:
        ap.error('--base is for checking; it cannot be combined with recording')
    os.makedirs(paths.BUILD, exist_ok=True)
    clean.record_worktree()
    try:
        base = load_base(a.base) if a.base else None
        sizes = load_base_sizes(a.base) if a.base else None
        rep = run(a.ids, check_all=a.all, behavioural=a.behavioural, record=a.record, base=base,
                  base_sizes=sizes, jobs=a.jobs)
    except Untrusted as e:
        rep = Report()
        rep.add('ERROR', str(e))
        rep.failed = True
    if a.all and rep.results:
        # Every claim was rebuilt: the symbol table of the claims (harness/lib/symtab.py).
        write_symbol_table(rep)
    if a.all and rep.results:
        # Every claim was rebuilt: what the relocation baseline would hold today.
        out = os.path.join(paths.BUILD, 'reloc_breaks.tsv')
        with open(out, 'w') as f:
            f.write(reloc_baseline_text(rep.claim_breaks, classes=True))
        by_class = {c: sum(1 for b in rep.claim_breaks if b['class'] == c)
                    for c in CLASS_ERRORS + CLASS_WARNS}
        rep.add('INFO', f'{len(rep.claim_breaks)} relocations name another symbol than the '
                f'claim at their target ({", ".join(f"{n} {c}" for c, n in by_class.items())}); '
                f'all written to {out}')
    if rep.regressions:
        status, code = 'ERROR regression', EXIT_REGRESSION
    elif rep.failed:
        status, code = 'ERROR not scored', EXIT_FAILED
    else:
        status, code = 'OK', EXIT_OK
    if a.json:
        print(json.dumps(dict(status=status, exit=code, lines=rep.lines, standins=rep.standins,
                              results=[
            {k: v for k, v in r.items() if k not in ('diff_rows',)} for r in rep.results]),
            indent=1))
    else:
        print('\n'.join(rep.lines))
        print(f'RESULT {status}')
    return code


if __name__ == '__main__':
    sys.exit(main())
