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
