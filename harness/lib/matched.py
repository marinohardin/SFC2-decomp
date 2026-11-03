"""matched_set.json: the best verified score per original function, keyed by original address.

Scores only go up. A verification below the recorded score is a regression, whatever the
function is called now.
"""
import json
import os

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
