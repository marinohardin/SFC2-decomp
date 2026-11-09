"""Rebuilt translation units: where a function's source is, how its unit is compiled, and which
compiled symbol is the annotated function.

A function is claimed by an annotation on the line(s) before it (harness/SRC_LAYOUT.md):

    // FUNCTION: STARFLEETOP 0x006fc690
    tDynamicAllocator* tDynamicAllocator::Instance()

`STUB:` instead of `FUNCTION:` marks a placeholder that is not scored.
"""
import hashlib
import json
import os
import re
import shlex
import shutil
import subprocess
import time

from . import paths

ANNOTATION = re.compile(r'^\s*//\s*(FUNCTION|STUB):\s*(\w+)\s+0x([0-9a-fA-F]+)\s*$')
SOURCE_EXTS = ('.cpp', '.c', '.cxx')
# Flags the harness owns: output locations and the listing it reads. Browse info, the
# minimal-rebuild database and automatic precompiled headers are dropped: they change no code or
# data bytes (toolchain/calibration/report.md, variant dsp-nopch) and /YX .pch files collide
# between parallel builds.
STRIP_FLAGS = ('/Fr', '/FR', '/FD', '/YX', '/Fo', '/Fa', '/FA', '/Fp', '/Fd', '/Gm')
COMPILE_TIMEOUT = int(os.environ.get('HARNESS_COMPILE_TIMEOUT', '600'))


class SourceError(Exception):
    pass


class Annotation:
    __slots__ = ('kind', 'module', 'address', 'path', 'line')

    def __init__(self, kind, module, address, path, line):
        self.kind, self.module, self.address, self.path, self.line = kind, module, address, path, line


def load_units(path=None):
    path = path or paths.UNITS
    with open(path) as f:
        cfg = json.load(f)
    cfg.setdefault('module', 'STARFLEETOP')
    cfg.setdefault('profiles', {})
    cfg.setdefault('units', {})
    cfg.setdefault('default_profile', None)
    return cfg


def scan_annotations(src_root, module):
    """Every FUNCTION/STUB annotation under src_root for `module`: {address: [Annotation]}."""
    found = {}
    if not os.path.isdir(src_root):
        return found
    for d, dirs, files in os.walk(src_root):
        dirs[:] = sorted(x for x in dirs if not x.startswith('.'))
        for name in sorted(files):
            if not name.lower().endswith(SOURCE_EXTS):
                continue
            p = os.path.join(d, name)
            with open(p, encoding='latin1') as f:
                for n, line in enumerate(f, 1):
                    m = ANNOTATION.match(line)
                    if m and m.group(2) == module:
                        a = Annotation(m.group(1), m.group(2), int(m.group(3), 16), p, n)
                        found.setdefault(a.address, []).append(a)
    return found


def unit_profile(cfg, src_root, unit_path):
    rel = os.path.relpath(unit_path, src_root).replace(os.sep, '/')
    entry = cfg['units'].get(rel, {})
    name = entry.get('profile', cfg['default_profile'])
    if not name or name not in cfg['profiles']:
        raise SourceError(f'no compile profile for {rel}: add it to the unit registry '
                          f'({paths.UNITS}) or set a default_profile')
    return rel, dict(cfg['profiles'][name], name=name)


def _flags(profile, src_root):
    flags = profile.get('flags', [])
    if isinstance(flags, str):
        flags = shlex.split(flags)
    flags = [os.path.expandvars(f) for f in flags if not f.startswith(STRIP_FLAGS)]
    if '/c' not in flags:
        flags.append('/c')
    first = []
    for inc in profile.get('include_first', []):
        inc = os.path.expandvars(inc)
        first += ['/I', inc if os.path.isabs(inc) else os.path.join(src_root, inc)]
    return first + flags


def compile_unit(unit_path, profile, src_root, build_root=None):
    """Compile one translation unit with the pinned compiler. Returns a dict with obj, listing,
    log, ok, secs, out_dir."""
    build_root = build_root or paths.BUILD
    key = hashlib.sha1(os.path.abspath(unit_path).encode()).hexdigest()[:12]
    base = os.path.splitext(os.path.basename(unit_path))[0]
    out = os.path.join(build_root, f'{base}-{key}')
    shutil.rmtree(out, ignore_errors=True)
    os.makedirs(out)
    cmd = [paths.CL] + _flags(profile, src_root) + ['/FAs', f'/Fa{out}/', f'/Fo{out}/',
                                                   os.path.basename(unit_path)]
    cname = f'sfcop-harness-{key}-{os.getpid()}'
    env = dict(os.environ, SFCOP_ROOT=paths.SFCOP_ROOT, SFCOP_MSVC_NAME=cname,
               MSVC_VERSION=str(profile.get('compiler', '6.4')))
    t0 = time.time()
    try:
        p = subprocess.run(cmd, cwd=os.path.dirname(os.path.abspath(unit_path)), env=env,
                           capture_output=True, text=True, errors='replace',
                           timeout=COMPILE_TIMEOUT)
        log, rc = p.stdout + p.stderr, p.returncode
    except subprocess.TimeoutExpired:
        subprocess.run(['docker', 'kill', cname], capture_output=True)
        log, rc = f'compile killed after {COMPILE_TIMEOUT}s', -1
    obj = os.path.join(out, base + '.obj')
    listing = os.path.join(out, base + '.asm')
    with open(os.path.join(out, 'cl.log'), 'w') as f:
        f.write(' '.join(shlex.quote(c) for c in cmd) + '\n' + log)
    ok = rc == 0 and os.path.exists(obj)
    return dict(ok=ok, rc=rc, obj=obj if ok else None,
                listing=listing if os.path.exists(listing) else None,
                log=log, secs=round(time.time() - t0, 1), out_dir=out)


PROC = re.compile(r'^(\S+)\s+PROC NEAR')
ENDP = re.compile(r'^(\S+)\s+ENDP')
FILE = re.compile(r'^; File (.+?)\s*$')
LINE = re.compile(r'^; (\d+)\s+:')
TITLE = re.compile(r'^\tTITLE\t(.+?)\s*$')
SEGMENT = re.compile(r'^(\S+)\s+SEGMENT\b')


def listing_functions(listing_text):
    """[(symbol, file, first_line)] from a VC6 /FAs listing: each PROC with the source line of
    its first line record (the opening brace).

    The file starts as the listing's TITLE. A `; File` record inside a PROC applies at once (an
    inline function from a header). One outside a PROC applies only if the segment it heads is
    code: VC6 also writes a record before a class's vftable, naming the header that declares the
    class, and does not repeat the unit's own record before the code that follows."""
    out = []
    cur_file, pending, proc, first = None, None, None, None
    for line in listing_text.splitlines():
        m = TITLE.match(line)
        if m and cur_file is None:
            cur_file = m.group(1)
            continue
        m = FILE.match(line)
        if m:
            if proc:
                cur_file = m.group(1)
            else:
                pending = m.group(1)
            continue
        m = SEGMENT.match(line)
        if m:
            if pending and m.group(1) == '_TEXT':
                cur_file = pending
            pending = None
            continue
        m = PROC.match(line)
        if m:
            if pending:
                cur_file, pending = pending, None
            proc, first = m.group(1), None
            continue
        if proc:
            m = LINE.match(line)
            if m and first is None:
                first = (cur_file, int(m.group(1)))
            elif ENDP.match(line):
                if first:
                    out.append((proc, first[0], first[1]))
                proc = None
    return out


def symbol_for_annotation(listing_text, unit_path, ann_line, other_ann_lines):
    """The compiled symbol the annotation at ann_line claims: the function whose first source
    line in this file is the nearest after the annotation, with no other annotation between."""
    main = os.path.basename(unit_path).lower()
    nxt = min([n for n in other_ann_lines if n > ann_line], default=None)
    best = []
    for sym, f, line in listing_functions(listing_text):
        if f is None or os.path.basename(f.replace('\\', '/')).lower() != main:
            continue
        if line > ann_line and (nxt is None or line < nxt):
            best.append((line, sym))
    if not best:
        return None
    first = min(best)[0]
    syms = [s for line, s in best if line == first]
    return syms[0] if len(syms) == 1 else syms
