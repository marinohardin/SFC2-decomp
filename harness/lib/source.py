"""Rebuilt translation units: where a function's source is, how its unit is compiled, and which
compiled symbol is the annotated function.

A function is claimed by an annotation on the line(s) before it (harness/SRC_LAYOUT.md):

    // FUNCTION: STARFLEETOP 0x006fc690
    tDynamicAllocator* tDynamicAllocator::Instance()

`STUB:` instead of `FUNCTION:` marks a placeholder that is not scored. An annotation in a unit is
compiled in that unit; one in a header in the unit op/UNITS.tsv assigns to its address
(harness/lib/claims.py), whose listing has line records for the header's inline code.
"""
import hashlib
import json
import os
import re
import shlex
import shutil
import signal
import subprocess
import time

from . import claims, deleting, paths

ANNOTATION = re.compile(r'^\s*//\s*(FUNCTION|STUB):\s*(\w+)\s+0x([0-9a-fA-F]+)\s*$')
SOURCE_EXTS = ('.cpp', '.c', '.cxx')
# Files scan_annotations reads: units, and headers, whose claims claim_unit places in a unit.
ANNOTATED_EXTS = SOURCE_EXTS + claims.HEADER_EXTS
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
            if not name.lower().endswith(ANNOTATED_EXTS):
                continue
            p = os.path.join(d, name)
            with open(p, encoding='latin1') as f:
                for n, line in enumerate(f, 1):
                    m = ANNOTATION.match(line)
                    if m and m.group(2) == module:
                        a = Annotation(m.group(1), m.group(2), int(m.group(3), 16), p, n)
                        found.setdefault(a.address, []).append(a)
    return found


def claim_unit(ann, src_root, units_tsv=None):
    """The unit that compiles the annotation `ann`: its own file, or for an annotation in a
    header the unit op/UNITS.tsv assigns to its address (harness/lib/claims.header_unit).
    Raises SourceError when a header claim has no such unit."""
    if not claims.is_header(ann.path):
        return ann.path
    try:
        return claims.header_unit(ann.address, ann.path, src_root, units_tsv)
    except claims.ClaimError as e:
        raise SourceError(str(e)) from None


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


def _stop_group(p, grace=5):
    """Stop a process started with start_new_session and everything it started: TERM first, so
    a wrapper's cleanup trap runs, then KILL."""
    for sig, wait in ((signal.SIGTERM, grace), (signal.SIGKILL, grace)):
        try:
            os.killpg(p.pid, sig)
        except ProcessLookupError:
            break
        try:
            p.communicate(timeout=wait)
            break
        except subprocess.TimeoutExpired:
            continue


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
    # Its own process group, so a timeout stops the whole compile: the wrapper and the compiler
    # it starts (cl-native's wibo, or the docker client).
    p = subprocess.Popen(cmd, cwd=os.path.dirname(os.path.abspath(unit_path)), env=env,
                         stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
                         errors='replace', start_new_session=True)
    try:
        out_, err_ = p.communicate(timeout=COMPILE_TIMEOUT)
        log, rc = out_ + err_, p.returncode
    except subprocess.TimeoutExpired:
        _stop_group(p)
        subprocess.run(['docker', 'kill', cname], capture_output=True)
        log, rc = f'compile killed after {COMPILE_TIMEOUT}s', -1
    except BaseException:
        # Ctrl-C reaches only the harness now that the compile has its own group: stop it too.
        _stop_group(p)
        subprocess.run(['docker', 'kill', cname], capture_output=True)
        raise
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
ENDS = re.compile(r'^(\S+)\s+ENDS\b')


def listing_functions(listing_text):
    """[(symbol, file, first_line)] from a VC6 /FAs listing: each PROC with the source line of
    its first line record (the opening brace).

    The file starts as the listing's TITLE. A `; File` record inside a PROC applies at once (an
    inline function from a header). One outside a PROC applies only if the segment it heads is
    code: VC6 also writes a record before a class's vftable, naming the header that declares the
    class, and does not repeat the unit's own record before the code that follows."""
    return [(proc, recs[0][0], recs[0][1]) for proc, recs in listing_line_records(listing_text)
            if recs]


def listing_line_records(listing_text):
    """[(symbol, [(file, line)])] from a VC6 /FAs listing: each PROC, in order, with every line
    record inside it and the file each one is in, read as listing_functions reads the first. An
    inline function from a header that VC6 expanded in a PROC leaves its header's records there."""
    out = []
    cur_file, pending, proc, recs = None, None, None, None
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
            proc, recs = m.group(1), []
            continue
        if proc:
            m = LINE.match(line)
            if m:
                recs.append((cur_file, int(m.group(1))))
            elif ENDP.match(line):
                out.append((proc, recs))
                proc = None
    return out


BRANCH = re.compile(r'^\t(?:call|jmp)\t(?!SHORT\b)(\S+)')
PUSH_OFFSET = re.compile(r'^\tpush\tOFFSET FLAT:(\S+)')
TABLE_ENTRY = re.compile(r'^\S*\s+DD\s+FLAT:(\S+)')
# The functions VC6 generates from a global object's definition, counted upwards from the
# annotation directly above it (tools/manifest.py).
GLOBAL_ROLES = ('dynamic initializer', 'atexit registration', 'atexit destructor')


def listing_bodies(listing_text):
    """({symbol: [(op, target)]}, [startup entries]) from a VC6 /FAs listing: for each PROC, in
    order, what it calls or jumps to (op 'branch') and the addresses it pushes (op 'push'); and
    the functions the CRT$XCU segment lists, which the C runtime calls at startup."""
    bodies, startup, proc, in_xcu = {}, [], None, False
    for line in listing_text.splitlines():
        m = SEGMENT.match(line)
        if m:
            in_xcu = m.group(1) == 'CRT$XCU'
            continue
        if ENDS.match(line):
            in_xcu = False
            continue
        if in_xcu:
            m = TABLE_ENTRY.match(line)
            if m:
                startup.append(m.group(1))
            continue
        m = PROC.match(line)
        if m:
            proc = m.group(1)
            bodies[proc] = []
        elif proc is not None and ENDP.match(line):
            proc = None
        elif proc is not None:
            m = BRANCH.match(line)
            if m:
                bodies[proc].append(('branch', m.group(1)))
            m = PUSH_OFFSET.match(line)
            if m:
                bodies[proc].append(('push', m.group(1)))
    return bodies, startup


def global_generated_functions(listing_text, initializer):
    """What VC6 generates from the global whose dynamic initializer is `initializer`, in
    GLOBAL_ROLES order: [initializer, registration, destructor], or [initializer] alone when the
    global registers no destructor. None when `initializer` is not a dynamic initializer: no
    CRT$XCU entry is it or calls or jumps to it.

    The registration is the function that calls _atexit on the initializer's path: one the
    initializer branches to, or one its CRT$XCU entry branches to after it (VC6 writes
    `call _$E1` / `jmp _$E3` there). The destructor is the address it pushes for _atexit."""
    bodies, startup = listing_bodies(listing_text)
    if initializer not in bodies:
        return None
    entries = [e for e in startup
               if e == initializer or ('branch', initializer) in bodies.get(e, [])]
    if not entries:
        return None
    reached = [t for op, t in bodies[initializer] if op == 'branch']
    for e in entries:
        targets = [t for op, t in bodies.get(e, []) if op == 'branch']
        if initializer in targets:
            reached += targets[targets.index(initializer) + 1:]
    for r in reached:
        body = bodies.get(r, [])
        if r == initializer or ('branch', '_atexit') not in body:
            continue
        pushed = [t for op, t in body[:body.index(('branch', '_atexit'))] if op == 'push']
        if pushed and pushed[-1] in bodies:
            return [initializer, r, pushed[-1]]
    return [initializer]


def deleting_destructor_symbol(listing_text, destructor, kind):
    """The `kind` deleting destructor VC6 generated for the class whose destructor is
    `destructor` (a `??1` symbol): the `??_G` (scalar) or `??_E` (vector) PROC that calls it or
    passes it on. None when the unit did not emit one (nothing in it needs it)."""
    bodies, _ = listing_bodies(listing_text)
    prefix = deleting.KINDS[kind][0]
    hits = [p for p, body in bodies.items() if p.startswith(prefix)
            and (('branch', destructor) in body or ('push', destructor) in body)]
    return hits[0] if len(hits) == 1 else None


def symbol_for_claim(listing_text, unit_path, ann_line, other_ann_lines, recorded=None, mangled='',
                     name='', unit=None):
    """symbol_for_annotation for the annotation of an address whose evidence is `recorded` (its
    matched-set entry, or None) and functions.csv's `mangled` and `name`: the evidence decides
    whether it claims a deleting destructor (harness/lib/deleting.py). The harness and the object
    lift both claim through this, as tools/manifest.py names rows from the same evidence."""
    return symbol_for_annotation(listing_text, unit_path, ann_line, other_ann_lines,
                                 deleting=deleting.kind(recorded, mangled, name), unit=unit)


def symbol_for_annotation(listing_text, unit_path, ann_line, other_ann_lines, deleting=None,
                          unit=None):
    """The compiled symbol the annotation at ann_line claims: the function whose first source
    line in this file is the nearest after the annotation, with no other annotation between.

    `unit_path` is the file the annotation is in and `other_ann_lines` the lines of the other
    annotations in that file. For an annotation in a header, the listing is that of the unit
    that compiles the claim (claim_unit, given as `unit`), and the header's line records in it
    are read. VC6 writes no `; File` record before the header's inline functions it emits after
    the unit's own code, so the listing gives them the unit's file; a function whose first line
    record quotes the header's text at that line, and not the unit's, is the header's.

    Annotations stacked above a global object's definition, with no function between them,
    claim what VC6 generates from that line, counted upwards (GLOBAL_ROLES): the dynamic
    initializer, whose line record is the definition, so the rule above finds it; then the
    atexit registration and the atexit destructor, which have no line record and are found
    from the initializer (global_generated_functions). Raises SourceError when such an
    annotation has nothing to claim: the global registers no destructor, or the stack is
    higher than GLOBAL_ROLES.

    `deleting` ('scalar' or 'vector', from harness/lib/deleting.kind) says the annotation claims
    a deleting destructor. Such an annotation is stacked on its class's out-of-class destructor
    definition, directly above it or above the destructor's own annotation; the deleting
    destructor has no line record, so it is found from the destructor the stack sits on
    (deleting_destructor_symbol); None when the unit emitted no such deleting destructor. A stack
    that does not sit on a destructor (a function written only to make the compiler emit the
    body) is read with the plain rule.

    In a header, the destructor the stack sits on may be inline and expanded everywhere, so that
    the unit has no `??1` to start from. The deleting destructor is then the `??_G` / `??_E`
    with line records of the destructor's definition in the header, or else the one whose
    mangled class is the class named on the definition line (_inline_deleting_destructor)."""
    if deleting:
        return _deleting_destructor(listing_text, unit_path, ann_line, other_ann_lines, deleting,
                                    unit)
    sym = _nearest_function(listing_text, unit_path, ann_line, other_ann_lines, unit)
    if sym is not None:
        return sym
    lines = sorted(set(other_ann_lines))
    depth, cur = 0, ann_line
    while sym is None:
        below = [n for n in lines if n > cur]
        if not below:
            return None
        depth, cur = depth + 1, below[0]
        sym = _nearest_function(listing_text, unit_path, cur, other_ann_lines, unit)
    if not isinstance(sym, str):
        return None
    generated = global_generated_functions(listing_text, sym)
    if generated is None:
        return None
    if depth < len(generated):
        return generated[depth]
    if depth < len(GLOBAL_ROLES):
        raise SourceError(f'the annotation at line {ann_line} claims the {GLOBAL_ROLES[depth]} '
                          f'of the global whose initializer is {sym}, but that global registers '
                          'no destructor with atexit: VC6 generated no such function')
    raise SourceError(f'the annotation at line {ann_line} is number {depth + 1} above the global '
                      f'whose initializer is {sym}; VC6 generates at most {len(GLOBAL_ROLES)} '
                      f'functions from a global ({", ".join(GLOBAL_ROLES)})')


def _deleting_destructor(listing_text, unit_path, ann_line, other_ann_lines, kind, unit=None):
    lines = sorted(set(other_ann_lines))
    cur = ann_line
    sym = _nearest_function(listing_text, unit_path, cur, other_ann_lines, unit)
    while sym is None:
        below = [n for n in lines if n > cur]
        if not below:
            break
        cur = below[0]
        sym = _nearest_function(listing_text, unit_path, cur, other_ann_lines, unit)
    if claims.is_header(unit_path):
        cls = _destructor_class(unit_path, ann_line)
        if cls is not None:
            if isinstance(sym, str) and sym.startswith('??1'):
                found = deleting_destructor_symbol(listing_text, sym, kind)
                if found is not None:
                    return found
            return _inline_deleting_destructor(listing_text, unit_path, ann_line,
                                               other_ann_lines, cls, kind)
    if sym is None:
        return None
    if not isinstance(sym, str) or not sym.startswith('??1'):
        # Not the form: a function written to make the compiler emit the deleting destructor's
        # body (tools/manifest.py warns about it). The plain rule applies, as before the form.
        return symbol_for_annotation(listing_text, unit_path, ann_line, other_ann_lines, unit=unit)
    return deleting_destructor_symbol(listing_text, sym, kind)


DESTRUCTOR_DEF = re.compile(r'((?:[A-Za-z_]\w*::)*([A-Za-z_]\w*))::~([A-Za-z_]\w*)\s*\(')


def _destructor_class(path, ann_line):
    """The qualified class whose out-of-class destructor definition follows the annotation stack
    at ann_line in the file at path (`A::B` from `inline A::B::~B()`), or None when what follows
    is not one."""
    try:
        with open(path, encoding='latin1') as f:
            src = f.read().split('\n')
    except OSError:
        return None
    for text in src[ann_line:]:
        t = text.strip()
        if not t or ANNOTATION.match(text) or t.startswith('//'):
            continue
        m = DESTRUCTOR_DEF.search(t.split('//')[0])
        return m.group(1) if m and m.group(2) == m.group(3) else None
    return None


def mangled_class(qualified):
    """The class part of a VC6 mangled member name: `B@A` for `A::B`, innermost first."""
    return '@'.join(reversed(qualified.split('::')))


def _inline_deleting_destructor(listing_text, path, ann_line, other_ann_lines, cls, kind):
    """The `kind` deleting destructor of class `cls` whose destructor is defined in the header at
    `path` after the annotation at ann_line: the one PROC of that kind with line records in the
    header between the annotation and the next, else the one (of those, if several) whose mangled
    name is that kind's for `cls`. None when the unit emitted neither."""
    prefix = deleting.KINDS[kind][0]
    main = os.path.basename(path).lower()
    nxt = min([n for n in other_ann_lines if n > ann_line], default=None)
    procs = [(p, recs) for p, recs in listing_line_records(listing_text) if p.startswith(prefix)]
    by_line = sorted({p for p, recs in procs for f, n in recs
                      if f is not None and os.path.basename(f.replace('\\', '/')).lower() == main
                      and n > ann_line and (nxt is None or n < nxt)})
    if len(by_line) == 1:
        return by_line[0]
    by_name = [p for p in (by_line or sorted({p for p, _ in procs}))
               if p.startswith(prefix + mangled_class(cls) + '@@')]
    return by_name[0] if len(by_name) == 1 else None


LINE_TEXT = re.compile(r'^; (\d+)\s+:(.*)$')


def listing_first_texts(listing_text):
    """{symbol: (line, text)}: the source line each PROC's first line record quotes."""
    out, proc = {}, None
    for line in listing_text.splitlines():
        m = PROC.match(line)
        if m:
            proc = m.group(1)
            continue
        if proc:
            m = LINE_TEXT.match(line)
            if m:
                out[proc] = (int(m.group(1)), m.group(2))
                proc = None
            elif ENDP.match(line):
                proc = None
    return out


def _same_text(a, b):
    return ' '.join(a.split()) == ' '.join(b.split())


def _from_this_file(listing_text, unit_path):
    """The symbols whose first line record quotes the text the unit has at that line, or None
    when the unit cannot be read. VC6 writes no `; File` record before the template instances it
    emits after the unit's own code, so listing_functions gives them the unit's file; the text
    the listing quotes for their first line is the header's, which tells them apart."""
    try:
        with open(unit_path, encoding='latin1') as f:
            src = f.read().split('\n')
    except OSError:
        return None
    return {sym for sym, (n, text) in listing_first_texts(listing_text).items()
            if 0 < n <= len(src) and _same_text(src[n - 1], text)}


def _nearest_function(listing_text, unit_path, ann_line, other_ann_lines, unit=None):
    main = os.path.basename(unit_path).lower()
    nxt = min([n for n in other_ann_lines if n > ann_line], default=None)
    own = _from_this_file(listing_text, unit_path)
    # In a header, also a function the listing gives another file (the unit's, for want of a
    # `; File` record) whose first line quotes the header's text there and not the unit's.
    theirs = (_from_this_file(listing_text, unit)
              if unit and claims.is_header(unit_path) and own is not None else None)
    best = []
    for sym, f, line in listing_functions(listing_text):
        if f is None:
            continue
        if os.path.basename(f.replace('\\', '/')).lower() != main:
            if theirs is None or sym not in own or sym in theirs:
                continue
        elif own is not None and sym not in own:
            continue
        if line > ann_line and (nxt is None or line < nxt):
            best.append((line, sym))
    if not best:
        return None
    first = min(best)[0]
    syms = [s for line, s in best if line == first]
    return syms[0] if len(syms) == 1 else syms
