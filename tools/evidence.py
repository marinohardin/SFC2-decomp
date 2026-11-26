#!/usr/bin/env python3
"""Print the evidence block a contributor pastes into a pull request (CONTRIBUTING.md).

    python3 tools/evidence.py [--base origin/develop] [--log harness-output.txt]

The block says whose judge scored the work and on what: the develop commit the branch was cut
from and the head, INTERFACE_VERSION, a hash of harness/lib/*.py (the judge's own code), the
compiler and objdiff, the exe's SHA-256, the platform, and the functions the branch's
`Harness-Score:` trailers record exact. With `--log`, a saved output of
`sh harness/build-and-verify.sh`, it adds the RESULT line and the ERROR and WARN lines counted by
class. The home directory and the user name are masked everywhere. Markdown on stdout.
"""
import argparse
import getpass
import hashlib
import os
import platform
import re
import struct
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, REPO)
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

TAG = re.compile(r'^(ERROR|WARN)\s+(.*)$')
BREAK_CLASSES = ('other-object', 'other-address', 'wrong-function', 'folded', 'stand-in',
                 'own-object')


def mask(text, home=None, user=None):
    """`text` with the home directory shown as ~ and the user name as <user>."""
    home = os.path.expanduser('~') if home is None else home
    if user is None:
        try:
            user = getpass.getuser()
        except Exception:
            user = ''
    if home and home not in ('/', '~'):
        text = text.replace(home, '~')
    if user and len(user) > 2:
        text = re.sub(r'(?<![\w.-])' + re.escape(user) + r'(?![\w.-])', '<user>', text)
    return text


def line_class(text):
    """The class of an ERROR or WARN line's text, for the counts."""
    if 'REGRESSION' in text:
        return 'regression'
    if 'BUILD HAS FAILED' in text:
        return 'build-failed'
    m = re.search(r'\(class ([a-z-]+)\)', text)
    if m:
        return m.group(1)
    for c in BREAK_CLASSES:
        if c in text:
            return c
    if 'COMDAT' in text:
        return 'comdat'
    if 'build path' in text:
        return 'build-path'
    if text.lstrip().startswith('relocation +0x'):
        return 'relocation'
    return 'other'


def summarize_log(text):
    """(RESULT line or None, {(tag, class): count}) of a harness output."""
    result, counts = None, {}
    for line in text.splitlines():
        if line.startswith('RESULT '):
            result = line.strip()
        m = TAG.match(line)
        if m:
            key = (m.group(1), line_class(m.group(2)))
            counts[key] = counts.get(key, 0) + 1
    return result, counts


def pe_version(path):
    """The file version in a PE file's VS_FIXEDFILEINFO, as a.b.c.d, or None."""
    try:
        with open(path, 'rb') as f:
            data = f.read()
    except OSError:
        return None
    i = data.find(b'\xbd\x04\xef\xfe')
    if i < 0 or i + 16 > len(data):
        return None
    ms, ls = struct.unpack_from('<II', data, i + 8)
    return f'{ms >> 16}.{ms & 0xffff}.{ls >> 16}.{ls & 0xffff}'


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1 << 20), b''):
            h.update(chunk)
    return h.hexdigest()


def judge_hash(repo=REPO):
    """SHA-256 over harness/lib/*.py: each file's name and content, in name order."""
    lib = os.path.join(repo, 'harness', 'lib')
    h = hashlib.sha256()
    for name in sorted(n for n in os.listdir(lib) if n.endswith('.py')):
        h.update(name.encode() + b'\0')
        with open(os.path.join(lib, name), 'rb') as f:
            h.update(f.read() + b'\0')
    return h.hexdigest()


def run(*cmd, cwd=REPO):
    try:
        r = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, timeout=60)
    except (OSError, subprocess.TimeoutExpired):
        return None
    return r.stdout.strip() if r.returncode == 0 else None


def compiler_lines():
    from harness.lib import paths
    if os.path.basename(paths.HARNESS_CL) == 'cl-native':
        tools = os.environ.get('SFCOP_TOOLS') or paths.TOOLS
        out = ['native (toolchain/bin/cl-native)']
        for f in ('CL.EXE', 'C1XX.DLL', 'C2.DLL'):
            v = pe_version(os.path.join(tools, 'msvc6', 'msvc6.4', 'Bin', f))
            out.append(f'{f} {v or "not read"}')
        wibo = paths.wibo(tools)
        if os.path.isfile(wibo):
            out.append(f'wibo sha256 {sha256(wibo)}')
        return out
    image = os.environ.get('SFCOP_MSVC_IMAGE', 'sfcop-msvc6:latest')
    iid = run('docker', 'image', 'inspect', '--format', '{{.Id}}', image)
    return [f'the image {image} ({iid or "not present"})']


def block(base, log_text=None):
    """The evidence block's lines (unmasked)."""
    from harness.lib import paths
    import unlanded
    head = run('git', 'rev-parse', 'HEAD') or 'unknown'
    cut = run('git', 'merge-base', 'HEAD', base) or f'unknown (no {base})'
    try:
        with open(os.path.join(REPO, 'INTERFACE_VERSION')) as f:
            iv = f.read().strip()
    except OSError:
        iv = 'unknown'
    objdiff = run(paths.OBJDIFF, '--version') if os.path.isfile(paths.OBJDIFF) else None
    exe = sha256(paths.EXE) if os.path.isfile(paths.EXE) else 'no install linked'
    try:
        claimed = sorted(unlanded.trailer_exact(base, 'HEAD', cwd=REPO))
    except RuntimeError:
        claimed = []
    out = ['### Evidence (tools/evidence.py)', '',
           f'- Cut from: {cut}', f'- Head: {head}', f'- INTERFACE_VERSION: {iv}',
           f'- harness/lib hash: {judge_hash()}',
           f'- Compiler: {"; ".join(compiler_lines())}',
           f'- objdiff: {objdiff or "not found"}',
           f'- Exe SHA-256: {exe}',
           f'- Platform: {platform.system()} {platform.release()} {platform.machine()}, '
           f'Python {platform.python_version()}',
           f'- Claimed exact: {len(claimed)}' + (f' ({", ".join(claimed)})' if claimed else '')]
    if log_text is not None:
        result, counts = summarize_log(log_text)
        out.append(f'- {result or "RESULT line: none in the log"}')
        for (tag, cls), n in sorted(counts.items()):
            out.append(f'  - {tag} {cls}: {n}')
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('--base', default='origin/develop')
    ap.add_argument('--log', help='a saved output of harness/build-and-verify.sh')
    a = ap.parse_args(argv)
    log_text = None
    if a.log:
        with open(a.log, encoding='utf-8', errors='replace') as f:
            log_text = f.read()
    print(mask('\n'.join(block(a.base, log_text))))
    return 0


if __name__ == '__main__':
    sys.exit(main())
