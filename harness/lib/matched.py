"""matched_set.json: the best verified score per original function, keyed by original address.

Scores only go up. A verification below the recorded score is a regression, whatever the
function is called now.
"""
import csv
import json
import os
import re

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
    game/ still claims (claimed: matched-set keys): a listed address has no definition there."""
    gone = [k for k, old in base['functions'].items()
            if k in listed and k not in data['functions'] and not old.get('exact')
            and k not in claimed]
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


def record(data, address, entry, now):
    """Store entry for address if it improves on the recorded one. `now` is an ISO date string
    passed in by the caller. Returns True when stored."""
    if not improves(data, address, entry['score'], entry.get('exact', False)):
        return False
    data['functions'][key(address)] = dict(entry, recorded=now)
    return True
