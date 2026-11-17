#!/usr/bin/env python3
"""Scan every commit for what must not be published.

Everything reachable from the refs is read once: each commit (its message, author and
committer), each ref name, each path, and each version of each file, including files deleted
long ago. Three kinds of thing are looked for:

1. Private terms: names that must never appear. They are not in this repository, since the
   list would itself publish them. They are read from a terms file outside it (`--terms`, else
   `$SFCOP_SCAN_TERMS`, else `history-scan/terms.txt` in the work folder beside the clone).
2. Personal data, by shape: absolute paths into a home folder or a mounted volume, e-mail
   addresses, commit identities that are not a hosting-service no-reply address, and strings
   shaped like a licence key (shown masked).
3. Retail content: any stretch of a committed file that is byte for byte a stretch of a file
   under a retail folder (`--retail`, repeatable; default: the linked install), and any
   committed file made mostly of lines that occur in one. Hex dumps and C byte arrays in text
   files are decoded and compared too. Line endings are ignored. A copied stretch is found
   when it holds a whole line, or a whole run between two NUL bytes, of 16 bytes or more.

    python3 tools/history_scan.py                       # all refs, the default terms and install
    python3 tools/history_scan.py --refs origin/develop # one branch's history
    python3 tools/history_scan.py --extra pulls.txt     # also scan a text file (PR texts, say)
    printf %s "$KEY" | python3 tools/history_scan.py --hash-term   # a terms line for a secret

The terms file holds one entry per line; `#` starts a comment:

    some name                 a case-insensitive regular expression
    allow: some name's ok use a regular expression; a match inside an allowed stretch is dropped
    sha256: <64 hex digits>   the hash of a secret, so the secret itself is never written down;
                              compared with every licence-key-shaped string and long token

`allow:` lines apply to the personal-data shapes as well. A finding lists the paths, how many
versions of the file hold it, whether the version at `--tip` (default HEAD) still does, and the
oldest commit that introduced it. What is only in history is not removed by fixing the tree.

Not covered: pull request titles, descriptions and comments, which the hosting service
publishes too (dump them to a file and pass `--extra`); commits no ref reaches; annotated tag
messages; retail content that was re-encoded other than as hex.

Exit 0 when nothing is found, 1 when something is (each is listed), 2 on a usage or git error.
A check whose input is absent prints `skip: <why>` and finds nothing.
"""

import argparse
import collections
import hashlib
import os
import re
import subprocess
import sys
import tempfile

HOME_PATH = re.compile(rb'(?<![A-Za-z0-9._-])(?:/Users/|/home/|/Volumes/|[A-Za-z]:\\Users\\)'
                       rb'[A-Za-z0-9][A-Za-z0-9._-]*')
EMAIL = re.compile(rb'[A-Za-z0-9._%+-]+@[A-Za-z0-9-]+(?:\.[A-Za-z0-9-]+)*\.[A-Za-z]{2,}')
KEY_SHAPE = re.compile(rb'(?<![A-Za-z0-9-])[A-Z0-9]{4,6}(?:-[A-Z0-9]{4,6}){2,5}(?![A-Za-z0-9-])')
TOKEN = re.compile(rb'(?<![A-Za-z0-9])[A-Za-z0-9]{12,40}(?![A-Za-z0-9])')
# Addresses that identify nobody: the hosting service's, and the reserved example domains.
PUBLIC_EMAIL = re.compile(
    rb'@(?:users\.noreply\.github\.com|github\.com|anthropic\.com'
    rb'|(?:[A-Za-z0-9-]+\.)*example\.(?:com|org|net)|[A-Za-z0-9.-]+\.(?:invalid|test|example))$',
    re.I)
PLACEHOLDER_HOME = re.compile(rb'[/\\](?:me|you|user|username|name|runner|example)$', re.I)

# A chunk is a stretch with no line end and no NUL; a copied region holds whole chunks.
CHUNK = re.compile(rb'[^\n\x00]{16,}')
HEX_RUN = re.compile(rb'(?:[0-9A-Fa-f]{2}[ \t]?){24,}|(?:0[xX][0-9A-Fa-f]{2}\s*,\s*){16,}')
TEXT_BYTES = bytes(range(32, 127)) + b'\t\n\r'
LOCATIONS_PER_CHUNK = 3
MIN_TEXT_RUN = 200
MIN_BINARY_RUN = 48
MIN_COVERED = 2048
MIN_COVERED_SHARE = 0.30


class ScanError(Exception):
    pass


def git(repo, *args, stdin=None):
    done = subprocess.run(['git', '-C', repo, *args], capture_output=True, stdin=stdin)
    if done.returncode:
        raise ScanError('git %s: %s' % (' '.join(args), done.stderr.decode(errors='replace').strip()))
    return done.stdout


# --- terms -------------------------------------------------------------------------------------

class Terms:
    def __init__(self, patterns=(), allow=(), hashes=()):
        self.patterns = [re.compile(p.encode(), re.I) for p in patterns]
        self.allow = [re.compile(p.encode(), re.I) for p in allow]
        self.hashes = set(hashes)

    @classmethod
    def load(cls, path):
        patterns, allow, hashes = [], [], []
        with open(path, encoding='utf-8') as f:
            for number, line in enumerate(f, 1):
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                try:
                    if line.lower().startswith('allow:'):
                        allow.append(line[6:].strip())
                        re.compile(allow[-1])
                    elif line.lower().startswith('sha256:'):
                        digest = line[7:].strip().lower()
                        if not re.fullmatch(r'[0-9a-f]{64}', digest):
                            raise re.error('not 64 hex digits')
                        hashes.append(digest)
                    else:
                        patterns.append(line)
                        re.compile(line)
                except re.error as e:
                    raise ScanError('%s line %d: %s' % (path, number, e))
        return cls(patterns, allow, hashes)

    def allowed_spans(self, data):
        return [m.span() for a in self.allow for m in a.finditer(data)]


def secret_digest(text):
    """The hash a `sha256:` line holds: of the letters and digits only, in upper case."""
    return hashlib.sha256(re.sub(rb'[^A-Za-z0-9]', b'', text).upper()).hexdigest()


def mask(text):
    """A key-shaped string with all but its first two characters hidden."""
    return text[:2] + re.sub(rb'[A-Za-z0-9]', b'*', text[2:])


def text_findings(data, terms):
    """Yield (kind, shown text, offset) for each private term and personal-data shape in data."""
    spans = terms.allowed_spans(data) if terms.allow else []

    def allowed(m):
        return any(s <= m.start() and m.end() <= e for s, e in spans)

    for pattern in terms.patterns:
        for m in pattern.finditer(data):
            if m.end() > m.start() and not allowed(m):
                yield 'term', m.group(), m.start()
    for m in HOME_PATH.finditer(data):
        if not PLACEHOLDER_HOME.search(m.group()) and not allowed(m):
            yield 'path', m.group(), m.start()
    for m in EMAIL.finditer(data):
        if not PUBLIC_EMAIL.search(m.group()) and not allowed(m):
            yield 'email', m.group(), m.start()
    for m in KEY_SHAPE.finditer(data):
        text = m.group()
        if terms.hashes and secret_digest(text) in terms.hashes:
            yield 'secret', mask(text), m.start()
        elif re.search(rb'[0-9]', text) and re.search(rb'[A-Z]', text) and not allowed(m):
            yield 'key', mask(text), m.start()
    if terms.hashes:
        for m in TOKEN.finditer(data):
            if secret_digest(m.group()) in terms.hashes:
                yield 'secret', mask(m.group()), m.start()


def identity_findings(commit):
    """Yield the author and committer of a raw commit that are not public no-reply addresses."""
    header = commit.split(b'\n\n', 1)[0]
    for m in re.finditer(rb'^(?:author|committer) (.*<([^>]*)>)', header, re.M):
        if not PUBLIC_EMAIL.search(m.group(2)):
            yield 'identity', m.group(1), m.start(1)


# --- reading the history -----------------------------------------------------------------------

def list_objects(repo, refs):
    """Every object the refs reach, as {id: first path seen} ('' for commits and root trees)."""
    paths = {}
    for line in git(repo, 'rev-list', '--objects', *refs).split(b'\n'):
        if line:
            oid, _, path = line.partition(b' ')
            paths.setdefault(oid.decode(), path.decode(errors='replace'))
    return paths


def read_objects(repo, ids, kinds=('blob', 'commit')):
    """Yield (id, kind, content) for each of the ids whose kind is wanted."""
    with tempfile.TemporaryFile() as wanted:
        wanted.write(''.join(i + '\n' for i in ids).encode())
        wanted.seek(0)
        proc = subprocess.Popen(['git', '-C', repo, 'cat-file', '--batch'], stdin=wanted,
                                stdout=subprocess.PIPE)
        try:
            while True:
                header = proc.stdout.readline().split()
                if not header:
                    break
                if len(header) != 3:
                    raise ScanError('git cat-file: %s' % b' '.join(header).decode())
                content = proc.stdout.read(int(header[2]) + 1)[:-1]
                if header[1].decode() in kinds:
                    yield header[0].decode(), header[1].decode(), content
        finally:
            proc.stdout.close()
            proc.wait()


def tip_blobs(repo, tip):
    try:
        out = git(repo, 'ls-tree', '-r', tip)
    except ScanError:
        return None
    return {line.split()[2].decode() for line in out.split(b'\n') if line}


def first_commit(repo, refs, blob):
    """The oldest commit, of those the refs reach, that adds or changes a file to this blob."""
    out = git(repo, 'log', '--format=%h %ad', '--date=short', '--find-object=' + blob, *refs)
    lines = out.decode().split('\n')
    return lines[-2] if len(lines) > 1 else '?'


# --- retail content ----------------------------------------------------------------------------

def normalise(data):
    return data.replace(b'\r\n', b'\n')


def decoded_hex(data):
    """Yield (offset, bytes) for each hex dump or C byte array in a text file."""
    for m in HEX_RUN.finditer(data):
        digits = re.sub(rb'0[xX]|[^0-9A-Fa-f]', b'', m.group())
        raw = bytes.fromhex(digits[:len(digits) & ~1].decode())
        if len(set(raw)) >= 8:
            yield m.start(), raw


def informative(chunk):
    return len(set(chunk)) >= 6


def common_prefix(a, i, b, j):
    """How many bytes a[i:] and b[j:] share at their start."""
    limit = min(len(a) - i, len(b) - j)
    n, step = 0, 64
    while n < limit:
        step = min(step, limit - n)
        if a[i + n:i + n + step] == b[j + n:j + n + step]:
            n += step
            step *= 2
        elif step == 1:
            break
        else:
            step = max(1, step // 2)
    return n


def common_suffix(a, i, b, j):
    """How many bytes a[:i] and b[:j] share at their end."""
    limit = min(i, j)
    n, step = 0, 64
    while n < limit:
        step = min(step, limit - n)
        if a[i - n - step:i - n] == b[j - n - step:j - n]:
            n += step
            step *= 2
        elif step == 1:
            break
        else:
            step = max(1, step // 2)
    return n


class RetailCheck:
    """Finds stretches of the scanned pieces that are also stretches of the retail files.

    A piece is one version of a committed file, or one decoded hex dump from it. Pieces are
    added first, then each retail file is streamed past them once.
    """

    def __init__(self):
        self.index = {}             # chunk hash -> up to LOCATIONS_PER_CHUNK (piece, offset)
        self.pieces = []            # label of each piece: (blob id, hex offset or None)
        self.kept = {}              # piece -> its bytes, for decoded hex (not re-readable)
        self.seen = set()           # chunk hashes that occur in a retail file
        self.runs = {}              # piece -> (length, piece offset, retail path, retail offset)

    def add(self, blob, data, hex_offset=None):
        piece = len(self.pieces)
        self.pieces.append((blob, hex_offset))
        if hex_offset is not None:
            self.kept[piece] = data
        for m in CHUNK.finditer(data):
            if informative(m.group()):
                where = self.index.setdefault(hash(m.group()), [])
                if len(where) < LOCATIONS_PER_CHUNK:
                    where.append((piece, m.start()))

    def add_blob(self, blob, content):
        data = normalise(content)
        self.add(blob, data)
        if b'\x00' not in data:
            for offset, raw in decoded_hex(data):
                self.add(blob, raw, offset)

    def scan_retail(self, path, shown, fetch):
        """Stream one retail file; `fetch(blob id)` returns that blob's normalised bytes."""
        with open(path, 'rb') as f:
            retail = normalise(f.read())
        covered = {}
        for m in CHUNK.finditer(retail):
            where = self.index.get(hash(m.group()))
            if not where:
                continue
            self.seen.add(hash(m.group()))
            for piece, offset in where:
                if m.start() < covered.get(piece, 0):
                    continue
                data = self.kept[piece] if piece in self.kept else fetch(self.pieces[piece][0])
                if data[offset:offset + len(m.group())] != m.group():
                    continue
                back = common_suffix(data, offset, retail, m.start())
                length = back + common_prefix(data, offset, retail, m.start())
                covered[piece] = m.start() - back + length
                run = retail[m.start() - back:m.start() - back + length]
                binary = bool(run.translate(None, TEXT_BYTES))
                whole = length == len(data) and length >= 64
                if length >= (MIN_BINARY_RUN if binary else MIN_TEXT_RUN) or whole:
                    if length > self.runs.get(piece, (0,))[0]:
                        self.runs[piece] = (length, offset - back, shown, m.start() - back,
                                            len(data), length == len(retail))

    def coverage(self, content):
        """(bytes of this file's chunks that occur in retail files, bytes of all its chunks)."""
        hit = total = 0
        for m in CHUNK.finditer(normalise(content)):
            if informative(m.group()):
                total += len(m.group())
                if hash(m.group()) in self.seen:
                    hit += len(m.group())
        return hit, total


def retail_files(folders):
    for folder in folders:
        for base, _dirs, names in os.walk(folder, followlinks=True):
            for name in sorted(names):
                path = os.path.join(base, name)
                if os.path.isfile(path) and os.path.getsize(path) >= 16:
                    yield path, os.path.relpath(path, folder)


# --- the scan ----------------------------------------------------------------------------------

class Finding:
    def __init__(self):
        self.versions = set()
        self.blobs = set()
        self.paths = collections.Counter()
        self.sample = ''
        self.at_tip = False
        self.detail = ''


def line_of(data, offset):
    start = data.rfind(b'\n', 0, offset) + 1
    end = data.find(b'\n', offset)
    line = data[start:len(data) if end < 0 else end].strip()
    return data.count(b'\n', 0, offset) + 1, line[:160].decode(errors='replace')


def scan(repo, refs, terms, retail_folders=(), extras=(), tip='HEAD', out=None):
    """Run every check; returns {(kind, shown text): Finding}."""
    out = out or sys.stdout
    found = collections.defaultdict(Finding)
    paths = list_objects(repo, refs)
    at_tip = tip_blobs(repo, tip)
    if at_tip is None:
        print('skip: no commit %s: findings are not marked as still present' % tip, file=out)
        at_tip = set()

    def note(kind, shown, oid, path, sample='', detail=''):
        f = found[kind, shown if isinstance(shown, str) else shown.decode(errors='replace')]
        if oid not in f.versions:
            f.paths[path] += 1
        f.versions.add(oid)
        if path and path == paths.get(oid):
            f.blobs.add(oid)
        f.sample = f.sample or sample
        f.detail = f.detail or detail
        f.at_tip = f.at_tip or oid in at_tip
        return f

    def note_text(data, oid, path):
        for kind, shown, offset in text_findings(data, terms):
            number, line = line_of(data, offset)
            note(kind, shown, oid, path, '%d: %s' % (number, line))

    retail = RetailCheck() if retail_folders else None
    counts = collections.Counter()
    for oid, kind, content in read_objects(repo, paths):
        counts[kind] += 1
        if kind == 'commit':
            label = 'commit ' + oid[:9]
            note_text(content, oid, label)
            for what, shown, _ in identity_findings(content):
                note(what, shown, oid, label)
        else:
            note_text(content, oid, paths[oid])
            if retail:
                retail.add_blob(oid, content)
    names = '\n'.join(sorted(set(paths.values()))).encode()
    note_text(names, 'paths', '(a file path)')
    ref_names = git(repo, 'for-each-ref', '--format=%(refname)')
    note_text(ref_names, 'refs', '(a ref name)')
    for extra in extras:
        with open(extra, 'rb') as f:
            note_text(f.read(), extra, 'extra file ' + os.path.basename(extra))
    print('read %d commits and %d file versions from %d refs'
          % (counts['commit'], counts['blob'], len(ref_names.split())), file=out)

    if retail:
        cache = collections.OrderedDict()

        def fetch(blob):
            if blob not in cache:
                cache[blob] = normalise(git(repo, 'cat-file', 'blob', blob))
                if len(cache) > 64:
                    cache.popitem(last=False)
            cache.move_to_end(blob)
            return cache[blob]

        n = 0
        for path, shown in retail_files(retail_folders):
            retail.scan_retail(path, shown, fetch)
            n += 1
        print('compared with %d retail files' % n, file=out)
        for piece, run in retail.runs.items():
            length, offset, shown, retail_offset, size, whole_retail = run
            blob, hex_offset = retail.pieces[piece]
            if hex_offset is not None:
                what = 'hex dump at byte %d decodes to %d bytes of' % (hex_offset, length)
            elif length == size and whole_retail:
                what = 'identical to'
            elif length == size:
                what = 'whole file (%d bytes) is inside' % length
            else:
                what = '%d bytes at byte %d are in' % (length, offset)
            note('retail', paths[blob], blob, paths[blob],
                 detail='%s retail file %s (at byte %d)' % (what, shown, retail_offset))
        best = {}
        for oid, _kind, content in read_objects(repo, paths, kinds=('blob',)):
            hit, _total = retail.coverage(content)
            if hit >= MIN_COVERED and hit >= MIN_COVERED_SHARE * len(content):
                f = note('retail-lines', paths[oid], oid, paths[oid])
                share = 100 * hit // len(content)
                if share >= best.get(paths[oid], 0):
                    best[paths[oid]] = share
                    f.detail = ('%d%% of the file (%d of %d bytes) is lines that occur in '
                                'retail files' % (share, hit, len(content)))
    return found


TITLES = [
    ('secret', 'A SECRET FROM THE TERMS FILE (shown masked)'),
    ('term', 'PRIVATE TERMS'),
    ('identity', 'COMMIT IDENTITIES that are not a no-reply address'),
    ('email', 'E-MAIL ADDRESSES'),
    ('path', 'PATHS INTO A HOME FOLDER OR VOLUME'),
    ('key', 'STRINGS SHAPED LIKE A LICENCE KEY (shown masked)'),
    ('retail', 'STRETCHES COPIED FROM RETAIL FILES'),
    ('retail-lines', 'FILES MADE MOSTLY OF LINES FROM RETAIL FILES'),
]


def report(found, repo, refs, out=None, locate=200, max_paths=6):
    out = out or sys.stdout
    for kind, title in TITLES:
        rows = sorted((k[1], f) for k, f in found.items() if k[0] == kind)
        if not rows:
            continue
        print('\n== %s: %d' % (title, len(rows)), file=out)
        for shown, f in rows:
            where = 'at the tip' if f.at_tip else 'history only'
            print('%s   [%d version%s, %s]'
                  % (shown, len(f.versions), '' if len(f.versions) == 1 else 's', where), file=out)
            if f.detail:
                print('    ' + f.detail, file=out)
            if kind not in ('retail', 'retail-lines'):
                for path, n in f.paths.most_common(max_paths):
                    print('    %s%s' % (path, ' (%d versions)' % n if n > 1 else ''), file=out)
                if len(f.paths) > max_paths:
                    print('    ... and %d more paths' % (len(f.paths) - max_paths), file=out)
            if f.sample:
                print('    line ' + f.sample, file=out)
            if locate > 0 and f.blobs:
                locate -= 1
                firsts = [first_commit(repo, refs, b) for b in sorted(f.blobs)[:8]]
                print('    first in %s' % min(firsts, key=lambda s: s.split(' ')[-1]), file=out)
    total = len(found)
    print('\nhistory scan: %s' % ('%d finding%s' % (total, '' if total == 1 else 's')
                                  if total else 'nothing found'), file=out)


def default_work():
    here = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    sys.path.insert(0, here)
    try:
        from harness.lib import paths
        return paths.WORK, os.path.dirname(paths.EXE)
    except Exception:
        return os.path.join(os.path.dirname(here), 'work'), ''


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--repo', default='.')
    ap.add_argument('--refs', nargs='+', default=['--all'], help='default: every ref')
    ap.add_argument('--tip', default='HEAD', help='the commit whose tree is "still present"')
    ap.add_argument('--terms', help='the private terms file')
    ap.add_argument('--retail', action='append', help='a folder of retail files (repeatable)')
    ap.add_argument('--no-retail', action='store_true', help='skip the retail comparison')
    ap.add_argument('--extra', action='append', default=[], help='another text file to scan')
    ap.add_argument('--hash-term', action='store_true',
                    help='read a secret on standard input, print its terms line, and exit')
    args = ap.parse_args(argv)

    if args.hash_term:
        print('sha256: ' + secret_digest(sys.stdin.buffer.read().strip()))
        return 0
    work, install = default_work()
    try:
        terms_path = args.terms or os.environ.get('SFCOP_SCAN_TERMS') \
            or os.path.join(work, 'history-scan', 'terms.txt')
        if os.path.isfile(terms_path):
            terms = Terms.load(terms_path)
        elif args.terms:
            raise ScanError('no terms file %s' % args.terms)
        else:
            print('skip: no terms file at %s: private terms are not looked for' % terms_path)
            terms = Terms()
        retail = [] if args.no_retail else (args.retail or [install])
        missing = [r for r in retail if not os.path.isdir(r)]
        if args.retail and missing:
            raise ScanError('no retail folder %s' % missing[0])
        if missing or not retail:
            print('skip: %s: nothing is compared with retail files'
                  % ('--no-retail' if args.no_retail else 'no install'))
            retail = []
        found = scan(args.repo, args.refs, terms, retail, args.extra, args.tip)
        report(found, args.repo, args.refs)
    except (ScanError, OSError) as e:
        print('history_scan: %s' % e, file=sys.stderr)
        return 2
    return 1 if found else 0


if __name__ == '__main__':
    sys.exit(main())
