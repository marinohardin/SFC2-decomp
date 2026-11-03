#!/usr/bin/env python3
"""Calibrate the VC6 compile oracle against Taldren's own build of the OP API SDK.

Compiles every ScriptInterface and Shared .cpp that has a member in ScriptInterfaceR.lib, using
the Release flags from _ScriptInterface.dsp / _Shared.dsp, and diffs each .obj against the retail
member with objdiff-cli. Writes toolchain/calibration/report.md plus raw data under
toolchain/calibration/results/.

    toolchain/calibration/calibrate.py                 # baseline + standard variants
    toolchain/calibration/calibrate.py --variants dsp  # only the baseline

Scratch (staged SDK copy, objects, objdiff projects) lives in $SFCOP_WORK/toolchain/calibration.
"""
import argparse, concurrent.futures, hashlib, json, os, shlex, shutil, struct, subprocess, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(HERE))
SFCOP = os.path.dirname(REPO)
WORK = os.path.join(os.environ.get('SFCOP_WORK', os.path.join(SFCOP, 'work')), 'toolchain', 'calibration')
SDK_REL = 'reference/installed game/Starfleet Command Orion Pirates/OP API - R2.1'
SDK = os.path.join(REPO, SDK_REL)
LIB = os.path.join(SDK, 'Scripts/Libraries/ScriptInterfaceR.lib')
HASHES = os.path.join(REPO, 'analysis/results/retail_hashes.sha256')
CL = os.path.join(REPO, 'toolchain/bin/cl')
OBJDIFF = os.environ.get('OBJDIFF', os.path.join(SFCOP, 'tools/objdiff/objdiff-cli-macos-arm64'))
RESULTS = os.path.join(HERE, 'results')
sys.dont_write_bytecode = True
sys.path.insert(0, HERE)
import strictcmp  # noqa: E402
TIMEOUT = int(os.environ.get('CAL_TIMEOUT', '900'))  # seconds per compile

# (project dir under Scripts/, .dsp file). Sources are compiled from the project dir, as the IDE does.
PROJECTS = [('ScriptInterface', '_ScriptInterface.dsp'), ('shared', '_Shared.dsp')]

# Taldren's build tree, from the .file symbols and __FILE__ strings of the retail members. The
# staged SDK Scripts/ dir is mounted there and sources are passed by this absolute path, as the
# VC6 IDE did, so path strings in the objects match too.
ORIG_ROOT = 'C:\\Projects\\Taldren\\Projects\\SFC2_5\\SCRIPTS'
ORIG_PROJ = {'ScriptInterface': 'ScriptInterface', 'shared': 'Shared'}

# Variant name -> (compiler version, flags to remove, flags to add, description).
# The baseline 'dsp' keeps the .dsp Release line verbatim; output-path flags are always added.
VARIANTS = {
    'dsp': ('6.4', [], [], 'VC6 SP4, .dsp Release flags verbatim'),
    'dsp-nopch': ('6.4', ['/YX', '/Fr', '/FR', '/FD'], [], 'SP4, without /YX /Fr /FD'),
    'sp5': ('6.5', [], [], 'VC6 SP5 (msvc6.5), .dsp flags'),
    'ox': ('6.4', ['/O2'], ['/Ox'], 'SP4, /Ox instead of /O2'),
    'ob1': ('6.4', [], ['/Ob1'], 'SP4, .dsp flags + /Ob1'),
    'gy': ('6.4', [], ['/Gy'], 'SP4, .dsp flags + /Gy'),
    'gf': ('6.4', [], ['/GF'], 'SP4, .dsp flags + /GF'),
}
DEFAULT_VARIANTS = ['dsp', 'dsp-nopch', 'sp5', 'ox', 'ob1', 'gy', 'gf']


def log(*a):
    print(*a, file=sys.stderr, flush=True)


def verify_hashes():
    want = {}
    for line in open(HASHES):
        h, path = line.rstrip('\n').split('  ', 1)
        want[path] = h
    for path in (LIB,):
        rel = os.path.relpath(path, os.path.join(REPO, 'reference'))  # hash file is relative to reference/
        got = hashlib.sha256(open(path, 'rb').read()).hexdigest()
        if want.get(rel) != got:
            sys.exit(f'hash mismatch for {rel}: {got} != {want.get(rel)}')
    log('retail hashes ok')


def stage_sdk():
    """Copy the SDK Scripts and STL trees to scratch; reference/ stays untouched."""
    dst = os.path.join(WORK, 'sdk')
    if os.path.isdir(dst):
        shutil.rmtree(dst)
    for sub in ('Scripts', 'STL'):
        shutil.copytree(os.path.join(SDK, sub), os.path.join(dst, sub),
                        ignore=shutil.ignore_patterns('*.lib', '*.dll', '*.Lib'))
    for d, _, files in os.walk(dst):
        for f in files:
            os.chmod(os.path.join(d, f), 0o644)
    return dst


def archive_members(path):
    data = open(path, 'rb').read()
    assert data[:8] == b'!<arch>\n'
    off, longnames, out = 8, b'', []
    while off + 60 <= len(data):
        hdr = data[off:off + 60]
        name = hdr[:16].decode('latin1').rstrip()
        size = int(hdr[48:58])
        body = data[off + 60:off + 60 + size]
        if name == '//':
            longnames = body
        elif name != '/':
            if name.startswith('/'):
                i = int(name[1:])
                name = longnames[i:longnames.index(b'\0', i)].decode('latin1')
            out.append((name, body))
        off += 60 + size + (size & 1)
    return out


def extract_targets():
    tdir = os.path.join(WORK, 'target')
    os.makedirs(tdir, exist_ok=True)
    names = {}
    for name, body in archive_members(LIB):
        if name.startswith('.\\Release\\') and name.endswith('.obj'):
            base = name.split('\\')[-1][:-4]
            p = os.path.join(tdir, base + '.obj')
            open(p, 'wb').write(body)
            names[base.lower()] = p
    return names


def dsp_info(projdir, dspname):
    text = open(os.path.join(projdir, dspname), 'rb').read().decode('latin1').replace('\r', '')
    lines = text.split('\n')
    flags, sources, in_release = None, [], False
    for ln in lines:
        if ln.startswith('!IF') or ln.startswith('!ELSEIF'):
            in_release = 'Release' in ln
        if ln.startswith('# ADD CPP') and in_release and flags is None:
            flags = shlex.split(ln[len('# ADD CPP'):])
        if ln.startswith('SOURCE=') and ln.lower().endswith('.cpp'):
            sources.append(ln[len('SOURCE='):].strip('"').replace('\\', '/'))
    return flags, sources


def build_units(sdk, targets):
    units = []
    for proj, dsp in PROJECTS:
        projdir = os.path.join(sdk, 'Scripts', proj)
        flags, sources = dsp_info(projdir, dsp)
        for src in sources:
            base = os.path.splitext(os.path.basename(src))[0]
            if not os.path.exists(os.path.join(projdir, src)):
                log(f'missing source {proj}/{src}')
                continue
            units.append(dict(name=base, project=proj, projdir=projdir, src=src, flags=flags,
                              target=targets.get(base.lower())))
    return units


def compile_unit(u, variant, stl_inc):
    ver, remove, add, _ = VARIANTS[variant]
    out = os.path.join(WORK, variant, 'obj', u['name'])
    if os.path.isdir(out):
        shutil.rmtree(out)
    os.makedirs(out)
    flags = [f for f in u['flags'] if f not in remove] + add
    # Output locations only; /Fp per unit so parallel /YX compiles do not share a .pch.
    cmd = [CL, '/I', stl_inc] + flags + [f'/Fo{out}/', f'/Fd{out}/', f'/Fp{out}/{u["name"]}.pch']
    if '/Fr' in flags or '/FR' in flags:
        cmd = [c if c not in ('/Fr', '/FR') else f'{c}{out}/' for c in cmd]
    src = u['src'][2:] if u['src'].startswith('./') else u['src']
    cmd.append('\\'.join([ORIG_ROOT, ORIG_PROJ[u['project']]] + src.split('/')))
    mounts = os.path.join(WORK, 'sdk', 'Scripts') + ':' + ORIG_ROOT[2:].replace('\\', '/')
    cname = f'sfcop-cal-{variant}-{u["name"]}-{os.getpid()}'.lower()
    env = dict(os.environ, MSVC_VERSION=ver, SFCOP_MSVC_NAME=cname, SFCOP_MSVC_MOUNTS=mounts)
    t0 = time.time()
    try:
        p = subprocess.run(cmd, cwd=u['projdir'], env=env, capture_output=True, text=True,
                           errors='replace', timeout=TIMEOUT)
    except subprocess.TimeoutExpired as e:
        subprocess.run(['docker', 'kill', cname], capture_output=True)
        p = subprocess.CompletedProcess(cmd, 'timeout', (e.stdout or b'').decode('latin1'),
                                        f'\nkilled after {TIMEOUT}s')
    obj = os.path.join(out, u['name'] + '.obj')
    open(os.path.join(out, 'cl.log'), 'w').write(' '.join(shlex.quote(c) for c in cmd) + '\n' +
                                                 p.stdout + p.stderr)
    ok = p.returncode == 0 and os.path.exists(obj)
    for f in os.listdir(out):  # .pch/.sbr/.idb are ~15 MB per unit; keep only the object and log
        if not f.endswith(('.obj', '.log')):
            os.remove(os.path.join(out, f))
    return dict(unit=u['name'], ok=ok, rc=p.returncode, secs=round(time.time() - t0, 1),
                obj=obj if ok else None, log=(p.stdout + p.stderr)[-2000:])


def objdiff_report(variant, units, results):
    vdir = os.path.join(WORK, variant)
    cfg = {'$schema': 'https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json',
           'build_target': False, 'build_base': False, 'units': []}
    for u in units:
        r = results[u['name']]
        if not u['target']:
            continue
        unit = {'name': f"{u['project']}/{u['name']}", 'target_path': u['target']}
        if r['ok']:
            unit['base_path'] = r['obj']
        cfg['units'].append(unit)
    json.dump(cfg, open(os.path.join(vdir, 'objdiff.json'), 'w'), indent=1)
    out = os.path.join(vdir, 'report.json')
    subprocess.run([OBJDIFF, 'report', 'generate', '-p', vdir, '-o', out, '-f', 'json'],
                   check=True, capture_output=True)
    return json.load(open(out))


def comp_id(path):
    """The @comp.id symbol value: (tool id << 16) | build. VC6 SP4 C++ is 0x000B225F (build 8799)."""
    b = open(path, 'rb').read()
    _, _, _, symoff, nsym, _, _ = struct.unpack_from('<HHIIIHH', b, 0)
    i = 0
    while i < nsym:
        name, val, _, _, _, naux = struct.unpack_from('<8sIhHBB', b, symoff + i * 18)
        if name == b'@comp.id':
            return val
        i += 1 + naux
    return None


def function_diffs(target, base):
    """Per-function match percentages from a one-shot objdiff diff (target=left, ours=right)."""
    tmp = base + '.diff.json'
    subprocess.run([OBJDIFF, 'diff', '-1', target, '-2', base, '-o', tmp, '--format', 'json'],
                   check=True, capture_output=True)
    d = json.load(open(tmp))
    os.remove(tmp)
    fns = []
    for s in d['left']['symbols']:
        if s.get('kind') != 'SYMBOL_FUNCTION' or 'match_percent' not in s and 'target_symbol' not in s:
            continue
        if s.get('flags', {}).get('undefined') or s.get('size') in (None, '0'):
            continue
        fns.append(dict(name=s['name'], demangled=s.get('demangled_name', s['name']),
                        size=int(s['size']), match=s.get('match_percent'),
                        paired=s.get('target_symbol') is not None))
    return fns


def reuse_unit(u, variant):
    """--no-compile: rescore the object left by an earlier run."""
    prev = json.load(open(os.path.join(RESULTS, f'{variant}.json')))['units'][u['name']]
    obj = os.path.join(WORK, variant, 'obj', u['name'], u['name'] + '.obj')
    ok = prev['compiled'] and os.path.exists(obj)
    return dict(unit=u['name'], ok=ok, rc=0, secs=prev['secs'], obj=obj if ok else None, log=prev['log'])


def run_variant(variant, units, jobs, compile=True):
    stl_inc = os.path.join(WORK, 'sdk', 'STL', 'Include')
    log(f'[{variant}] compiling {len(units)} units with {jobs} jobs' if compile else f'[{variant}] rescoring')
    with concurrent.futures.ThreadPoolExecutor(jobs) as ex:
        work = (lambda u: compile_unit(u, variant, stl_inc)) if compile else (lambda u: reuse_unit(u, variant))
        res = {r['unit']: r for r in ex.map(work, units)}
    for r in res.values():
        log(f"[{variant}] {r['unit']}: {'ok' if r['ok'] else 'FAILED rc=%s' % r['rc']} {r['secs']}s")
    rep = objdiff_report(variant, units, res)
    per_unit = {}
    for u in units:
        r = res[u['name']]
        fns = function_diffs(u['target'], r['obj']) if (r['ok'] and u['target']) else []
        per_unit[u['name']] = dict(project=u['project'], src=u['src'], compiled=r['ok'],
                                   secs=r['secs'], has_target=bool(u['target']),
                                   n_functions=len(fns),
                                   exact=sum(1 for f in fns if f['match'] == 100.0),
                                   code_bytes=sum(f['size'] for f in fns),
                                   exact_bytes=sum(f['size'] for f in fns if f['match'] == 100.0),
                                   misses=[f for f in fns if f['match'] != 100.0],
                                   log=None if r['ok'] else r['log'],
                                   strict=strictcmp.compare(u['target'], r['obj'])
                                   if (r['ok'] and u['target']) else None,
                                   comp_id=comp_id(r['obj']) if r['ok'] else None,
                                   target_comp_id=comp_id(u['target']) if u['target'] else None)
    summary = dict(variant=variant, desc=VARIANTS[variant][3], compiler=VARIANTS[variant][0],
                   objdiff=rep.get('measures', {}), units=per_unit,
                   objdiff_units={x['name']: x.get('measures', {}) for x in rep.get('units', [])})
    os.makedirs(RESULTS, exist_ok=True)
    json.dump(summary, open(os.path.join(RESULTS, f'{variant}.json'), 'w'), indent=1)
    return summary


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--variants', default=','.join(DEFAULT_VARIANTS))
    ap.add_argument('--jobs', type=int, default=int(os.environ.get('JOBS', '4')))
    ap.add_argument('--only', help='comma-separated unit names (debugging)')
    ap.add_argument('--no-compile', action='store_true',
                    help='rescore objects from the previous run instead of compiling')
    args = ap.parse_args()
    for tool in (CL, OBJDIFF):
        if not os.path.exists(tool):
            sys.exit(f'missing {tool}; see toolchain/README.md')
    verify_hashes()
    os.makedirs(WORK, exist_ok=True)
    sdk = os.path.join(WORK, 'sdk') if args.no_compile else stage_sdk()
    targets = extract_targets()
    units = build_units(sdk, targets)
    if args.only:
        keep = set(args.only.split(','))
        units = [u for u in units if u['name'] in keep]
    for v in args.variants.split(','):
        run_variant(v, units, args.jobs, compile=not args.no_compile)
    subprocess.run([sys.executable, '-I', os.path.join(HERE, 'make_report.py')], check=True)


if __name__ == '__main__':
    main()
