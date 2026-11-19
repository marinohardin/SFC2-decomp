"""The incremental build and the score cache.

A unit is rebuilt only when its build key changes: the compile command, the compiler (its
wrapper and its files, or the image), and the content of every file the unit includes. The
includes are found by following `#include` lines through the command's /I folders, every one
of them whatever `#if` surrounds it, so a header is never missed; a unit with an include this
cannot read (`#include MACRO`) is always rebuilt. The key and the objects stay in the unit's
build folder, per checkout, because listings carry the checkout's paths.

A function is rescored only when its score key changes: its compiled object (timestamp masked),
the symbol scored, its address and size, the original exe, the function index, the judge's own
code and objdiff. Scores are content-addressed, so they are shared by every checkout, under
HARNESS_SHARED/cache/. Only a completed objdiff comparison is stored.

HARNESS_NO_CACHE=1 turns both off: every unit is compiled and every function scored. CI runs
that way, so each pull request's check is also a check of the cache.
"""
import hashlib
import json
import os
import re
import subprocess
import threading

from . import paths
from .claims import INCLUDE, _resolve

ANY_INCLUDE = re.compile(r'^\s*#\s*include\b')
# The judge's code: what a score depends on besides its inputs.
JUDGE_FILES = ('cache.py', 'coff.py', 'deleting.py', 'funcs.py', 'source.py', 'target.py',
               'verify.py')
_memo = {}
_lock = threading.Lock()


def enabled():
    return os.environ.get('HARNESS_NO_CACHE', '') in ('', '0')


def _hash_files(h, files):
    for p in files:
        h.update(p.encode() + b'\0')
        try:
            with open(p, 'rb') as f:
                for chunk in iter(lambda: f.read(1 << 20), b''):
                    h.update(chunk)
        except OSError:
            h.update(b'<missing>')
        h.update(b'\0')


def _memoized(key, compute):
    with _lock:
        if key in _memo:
            return _memo[key]
    value = compute()
    with _lock:
        _memo[key] = value
    return value


def _tree(root):
    out = []
    for d, dirs, files in os.walk(root):
        dirs.sort()
        out += [os.path.join(d, f) for f in sorted(files)]
    return out


def compiler_id(cl, version):
    """A hash of everything the compiler wrapper `cl` runs for MSVC_VERSION `version`, or None
    when it cannot be told (no image): then nothing is reused."""
    def compute():
        h = hashlib.sha256()
        here = os.path.dirname(os.path.abspath(cl))
        name = os.path.basename(cl)
        if name == 'cl-native':
            tools = os.environ.get('SFCOP_TOOLS') or paths.TOOLS
            _hash_files(h, [cl, os.path.join(tools, 'wibo', '1.2.0', 'wibo-macos')]
                        + _tree(os.path.join(tools, 'msvc6', f'msvc{version}')))
        elif name == 'cl' and os.path.isfile(os.path.join(here, 'msvc-run')):
            image = os.environ.get('SFCOP_MSVC_IMAGE', 'sfcop-msvc6:latest')
            try:
                p = subprocess.run(['docker', 'image', 'inspect', '--format', '{{.Id}}', image],
                                   capture_output=True, text=True, timeout=60)
            except (OSError, subprocess.TimeoutExpired):
                return None
            if p.returncode != 0 or not p.stdout.strip():
                return None
            h.update(p.stdout.strip().encode())
            _hash_files(h, [cl, os.path.join(here, 'msvc-run')])
        else:
            _hash_files(h, [cl])
        return h.hexdigest()
    return _memoized(('compiler', cl, version), compute)


def include_dirs(cmd, env):
    """The folders a compile command searches: its /I arguments, then MSVC_EXTRA_INCLUDE."""
    dirs = []
    for i, a in enumerate(cmd):
        if a in ('/I', '-I') and i + 1 < len(cmd):
            dirs.append(cmd[i + 1])
        elif a[:2] in ('/I', '-I') and len(a) > 2:
            dirs.append(a[2:])
    for d in (env.get('MSVC_EXTRA_INCLUDE') or '').split(';'):
        if d:
            dirs.append(d[2:] if re.match(r'^[A-Za-z]:', d) else d)
    return dirs


def dependencies(unit_path, dirs):
    """Every file the unit reads that can be found: itself and what it includes, directly or
    through other files, following every #include line. None when an #include line names no
    file (a macro include): such a unit cannot be keyed."""
    seen, todo = set(), [os.path.realpath(unit_path)]
    while todo:
        p = todo.pop()
        if p in seen:
            continue
        seen.add(p)
        try:
            with open(p, encoding='latin1') as f:
                lines = f.readlines()
        except OSError:
            continue
        for line in lines:
            if not ANY_INCLUDE.match(line):
                continue
            m = INCLUDE.match(line)
            if not m:
                return None
            q = _resolve(m.group(2), m.group(1) == '"', os.path.dirname(p), dirs)
            if q is not None:
                todo.append(q)
    return sorted(seen)


def build_key(cmd, cwd, env, unit_path):
    """The build key of one compile, or None when it cannot be keyed."""
    if not enabled():
        return None
    version = env.get('MSVC_VERSION', '6.4')
    comp = compiler_id(cmd[0], version)
    if comp is None:
        return None
    dirs = [d if os.path.isabs(d) else os.path.join(cwd, d) for d in include_dirs(cmd, env)]
    deps = dependencies(unit_path, dirs)
    if deps is None:
        return None
    h = hashlib.sha256(b'build 1\0' + comp.encode() + b'\0' + cwd.encode() + b'\0')
    h.update('\0'.join(cmd).encode() + b'\0')
    for v in ('MSVC_VERSION', 'MSVC_EXTRA_INCLUDE', 'MSVC_LIB', 'MSVC_LIB_SET'):
        h.update(f'{v}={env.get(v, "")}\0'.encode())
    _hash_files(h, deps)
    return h.hexdigest()


def _judge_id():
    def compute():
        h = hashlib.sha256(b'judge 1\0')
        lib = os.path.dirname(os.path.abspath(__file__))
        _hash_files(h, [os.path.join(lib, f) for f in JUDGE_FILES] + [paths.OBJDIFF])
        return h.hexdigest()
    return _memoized('judge', compute)


def _inputs_id(exe_path):
    def compute():
        h = hashlib.sha256(b'inputs 1\0')
        _hash_files(h, [exe_path, paths.FUNCTIONS_CSV] + list(paths.SYMBOL_TSVS))
        return h.hexdigest()
    return _memoized(('inputs', exe_path), compute)


def object_hash(obj_path):
    """The object's hash with its COFF timestamp (bytes 4 to 8) masked: a recompile of the same
    source gives the same hash."""
    with open(obj_path, 'rb') as f:
        raw = bytearray(f.read())
    raw[4:8] = b'\0\0\0\0'
    return hashlib.sha256(raw).hexdigest()


def score_key(obj_path, symbol, address, size):
    if not enabled():
        return None
    h = hashlib.sha256(b'score 1\0')
    for part in (object_hash(obj_path), symbol, f'{address:08x}', str(size), _judge_id(),
                 _inputs_id(paths.EXE)):
        h.update(part.encode() + b'\0')
    return h.hexdigest()


def _score_path(key):
    return os.path.join(paths.SHARED, 'cache', 'scores', key[:2], key + '.json')


def load_score(key):
    if key is None:
        return None
    try:
        with open(_score_path(key)) as f:
            return json.load(f)
    except (OSError, ValueError):
        return None


def store_score(key, value):
    if key is None:
        return
    path = _score_path(key)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    tmp = f'{path}.{os.getpid()}.{threading.get_ident()}.part'
    with open(tmp, 'w') as f:
        json.dump(value, f)
    os.replace(tmp, path)
