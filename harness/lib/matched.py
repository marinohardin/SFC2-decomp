"""matched_set.json: the best verified score per original function, keyed by original address.

Scores only go up. A verification below the recorded score is a regression, whatever the
function is called now.
"""
import csv
import json
import os
import re

from . import deleting

VERSION = 1
EPSILON = 1e-6


def empty():
    return {'version': VERSION, 'functions': {}}


def load(path):
    if not os.path.exists(path):
        return empty()
    with open(path) as f:
        return parse(f.read(), path)


def parse(text, where):
    data = json.loads(text)
    if data.get('version') != VERSION or not isinstance(data.get('functions'), dict):
        raise ValueError(f'{where}: not a version {VERSION} matched set')
    return data


def worse(new, old):
    """True when entry `new` (None if absent) is below entry `old`: dropped, a lower score,
    or no longer exact."""
    return (new is None or new['score'] + EPSILON < old['score']
            or (bool(old.get('exact')) and not new.get('exact')))


def merged(data, base):
    """data with every entry of base that data dropped or lowered put back: the bar a
    verification against base has to clear."""
    out = dict(data, functions=dict(data['functions']))
    for k, old in base['functions'].items():
        if worse(out['functions'].get(k), old):
            out['functions'][k] = old
    return out


BOUNDARY_FIELDS = ('address', 'name', 'part_of', 'kind', 'note')


def boundary_listed(path):
    """The addresses on the boundary-error list, as matched-set keys. A row whose address is
    not hexadecimal stops with the file and line, not a traceback from int()."""
    if not os.path.exists(path):
        return set()
    out = set()
    with open(path, newline='') as f:
        for n, r in enumerate(csv.DictReader(f, delimiter='\t'), start=2):
            a = (r.get('address') or '').strip()
            if not a:
                continue
            if not re.fullmatch(r'0x[0-9a-fA-F]{1,8}', a):
                raise ValueError(f'{path}:{n}: address {a!r} is not a 0x-prefixed hexadecimal address')
            out.add(key(int(a, 16)))
    return out


RELEASED_FIELDS = ('address', 'date', 'reason')


def load_released(path):
    """{matched-set key: (date, reason)} of the released-function list, or {} when there is no
    file. A malformed file (another header, a row that is not three cells, an address that is not
    0x-prefixed hexadecimal, a date that is not YYYY-MM-DD, an empty reason, an address twice)
    raises ValueError naming the file and line: a list that is silently misread would release
    the wrong functions."""
    if not os.path.exists(path):
        return {}
    with open(path, newline='', encoding='utf-8') as f:
        lines = f.read().split('\n')
    if lines[0].split('\t') != list(RELEASED_FIELDS):
        raise ValueError(f'{path}:1: the header must be {chr(9).join(RELEASED_FIELDS)}')
    out = {}
    for n, line in enumerate(lines[1:], 2):
        if not line:
            continue
        cells = line.split('\t')
        if len(cells) != 3:
            raise ValueError(f'{path}:{n}: {len(cells)} cells, expected 3')
        a, date, reason = cells
        if not re.fullmatch(r'0x[0-9a-fA-F]{1,8}', a):
            raise ValueError(f'{path}:{n}: address {a!r} is not a 0x-prefixed hexadecimal address')
        if not re.fullmatch(r'\d{4}-\d{2}-\d{2}', date):
            raise ValueError(f'{path}:{n}: date {date!r} is not YYYY-MM-DD')
        if not reason.strip():
            raise ValueError(f'{path}:{n}: the reason is empty')
        k = key(int(a, 16))
        if k in out:
            raise ValueError(f'{path}:{n}: address {a} is listed twice')
        out[k] = (date, reason)
    return out


def release_released(base, data, released):
    """(base without the entries of listed addresses that data lost or lowered, [(key, date,
    reason) of each released entry used], [keys of listed addresses data still has exact]).
    Whatever `worse` calls worse is released for a listed address: dropped, no longer exact, or
    a lower score. A listed address that is still exact is stale: the list says nothing about it
    and it stays held."""
    used = [(k, *released[k]) for k, old in sorted(base['functions'].items())
            if k in released and worse(data['functions'].get(k), old)]
    stale = sorted(k for k in released
                   if (data['functions'].get(k) or {}).get('exact'))
    gone = {k for k, _, _ in used}
    if not gone:
        return base, [], stale
    return dict(base, functions={k: v for k, v in base['functions'].items() if k not in gone}), \
        used, stale


def release_listed(base, data, listed, claimed=()):
    """base without the entries data dropped because the address is on the boundary-error list
    and was never exact there: such an entry scored a fragment, not a function. Returns
    (base, [released keys]). An exact entry is never released, nor one a FUNCTION annotation in
    op/ still claims (claimed: matched-set keys): a listed address has no definition there."""
    gone = [k for k, old in base['functions'].items()
            if k in listed and k not in data['functions'] and not old.get('exact')
            and k not in claimed]
    if not gone:
        return base, []
    return dict(base, functions={k: v for k, v in base['functions'].items() if k not in gone}), \
        sorted(gone)


def misscored(entry):
    """True when the entry names a deleting destructor but was scored against a plain
    destructor (`??1`): its score measures a different function than its address."""
    return ((entry.get('symbol') or '').startswith('??1')
            and deleting.kind(name=entry.get('name') or '') is not None)


def release_misscored(base, data, claimed=()):
    """base without the entries data dropped that were scored against the wrong function: the
    name says deleting destructor, the symbol is the plain destructor (misscored). Such a score
    says nothing about the address, so losing it is not a regression. Returns (base, [released
    keys]). An exact entry is never released, nor one a FUNCTION annotation in op/ still
    claims (claimed: matched-set keys)."""
    gone = [k for k, old in base['functions'].items()
            if k not in data['functions'] and not old.get('exact') and k not in claimed
            and misscored(old)]
    if not gone:
        return base, []
    return dict(base, functions={k: v for k, v in base['functions'].items() if k not in gone}), \
        sorted(gone)


JOINED_PROLOG = 10   # ghidra/scripts/join_eh_prologs.py: the prolog's size


def release_reshaped(base, base_sizes, head_sizes, joined):
    """base without the entries of functions that join_eh_prologs.py has joined since: the
    address is a joined entry (`joined`: keys of the rows tagged eh-prolog-joined in this tree's
    functions.csv) or the body JOINED_PROLOG bytes after one, and its size differs from the
    base's (`base_sizes` and `head_sizes` map matched-set keys to functions.csv sizes; a missing
    key means no function there). Such an entry scored a prolog or a body alone, a byte range the
    function no longer has. Any other change of size is held: the gate releases only what the
    join explains. Returns (base, [released keys]). An exact entry is never released."""
    def by_join(k):
        a = int(k, 16)
        return k in joined or key(a - JOINED_PROLOG) in joined
    gone = [k for k, old in base['functions'].items()
            if not old.get('exact') and base_sizes.get(k) != head_sizes.get(k) and by_join(k)]
    if not gone:
        return base, []
    return dict(base, functions={k: v for k, v in base['functions'].items() if k not in gone}), \
        sorted(gone)


def save(path, data):
    data['functions'] = dict(sorted(data['functions'].items()))
    tmp = path + '.tmp'
    with open(tmp, 'w') as f:
        json.dump(data, f, indent=1, sort_keys=False)
        f.write('\n')
    os.replace(tmp, path)


def key(address):
    return f'0x{address:08x}'


def recorded(data, address):
    return data['functions'].get(key(address))


def is_regression(data, address, score):
    old = recorded(data, address)
    return old is not None and score + EPSILON < old['score']


def improves(data, address, score, exact):
    old = recorded(data, address)
    if old is None:
        return score > EPSILON or exact
    return score > old['score'] + EPSILON or (exact and not old.get('exact'))


REFRESHED = ('name', 'symbol', 'unit', 'size')


def refresh(data, address, entry):
    """Update the descriptive fields of the recorded entry from `entry`, which scored the same
    (not higher: record does that; not lower: a lower score is a regression, never stored). The
    landing step uses it so a name the source or the analysis has since corrected replaces the
    stale one. Returns True when something changed."""
    old = recorded(data, address)
    if old is None or entry['score'] + EPSILON < old['score'] or bool(old.get('exact')) > bool(
            entry.get('exact')):
        return False
    new = {k: entry[k] for k in REFRESHED if k in entry and old.get(k) != entry[k]}
    old.update(new)
    return bool(new)


def record(data, address, entry, now):
    """Store entry for address if it improves on the recorded one. `now` is an ISO date string
    passed in by the caller. Returns True when stored."""
    if not improves(data, address, entry['score'], entry.get('exact', False)):
        return False
    data['functions'][key(address)] = dict(entry, recorded=now)
    return True
