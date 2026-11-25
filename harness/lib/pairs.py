"""A deleting destructor and the real destructor it is generated from: one unit of work.

A scalar or vector deleting destructor (`??_G` / `??_E`, about 30 bytes: call the class's real
destructor, test the flag, call operator delete) has no source of its own. The compiler writes it
from the class's out-of-class destructor definition, so a session cannot produce one without
also claiming that destructor, an address of its own. The loop therefore hands out the group
(the real destructor and the class's deleting destructors) as one piece of work: any member the
queue reaches first is the lead, the others are its partners, and the session may claim all of
them (harness/README.md, "Deleting destructors").

Which functions belong together is read from evidence, never from a name alone:

- the class of each function, from its symbol (the symbol table's, a claim's, functions.csv's) or
  its name: `??1Cls@@` is the real destructor, `??_GCls@@` / `??_ECls@@` (or the older
  `scalar_deleting_destructor` member form) a deleting destructor of Cls;
- the bytes of a deleting destructor: the first call near its start goes to the real destructor.
  When the class and the call disagree, or neither names one, the address is `Unpaired` with the
  reason and the queue routes it out of the loop (harness/lib/workqueue.py, `park`).

This module imports nothing from the harness except deleting.py, so the queue can use it alone.
"""
import re
import struct

from . import deleting

CALL_SCAN = 16  # a deleting destructor calls the real destructor within its first bytes
REAL = re.compile(r'^\?\?1(.+?)@@')
DELETING_SYMBOL = re.compile(r'^\?\?_[GE](.+?)@@')
MEMBER_SYMBOL = re.compile(r'^\?(?:scalar|vector)_deleting_destructor@(.+?)@@')


class Pair:
    """The group an address belongs to: its role ('deleting' or 'real'), the kind of deleting
    destructor it is (or None), and the other members (addresses, as ints)."""

    def __init__(self, role, kind, partners, how=''):
        self.role, self.kind, self.partners, self.how = role, kind, list(partners), how

    def as_dict(self):
        return {'role': self.role, 'kind': self.kind, 'how': self.how,
                'partners': [f'0x{a:08x}' for a in self.partners]}

    def __repr__(self):
        return f'Pair({self.role}, {self.kind}, {[hex(a) for a in self.partners]})'


class Unpaired:
    """A deleting destructor whose real destructor cannot be identified."""

    def __init__(self, reason):
        self.reason = reason

    def __repr__(self):
        return f'Unpaired({self.reason!r})'


def class_of_name(name):
    """The mangled class of a demangled name `A::B::member` (`B@A`), or ''."""
    head, sep, _ = (name or '').replace('`', '').rpartition('::')
    if not sep:
        return ''
    return '@'.join(reversed(head.split('::')))


def classify(symbol, name):
    """(role, kind, class) from one function's symbol and name: role 'real' for a destructor,
    'deleting' for a deleting destructor, None for anything else."""
    symbol, name = symbol or '', name or ''
    k = deleting.kind(mangled=symbol, name=name)
    if k:
        for rx in (DELETING_SYMBOL, MEMBER_SYMBOL):
            m = rx.match(symbol)
            if m:
                return 'deleting', k, m.group(1)
        return 'deleting', k, class_of_name(name)
    m = REAL.match(symbol)
    if m:
        return 'real', None, m.group(1)
    if not symbol and name.rpartition('::')[2].startswith('~'):
        return 'real', None, class_of_name(name)
    return None, None, ''


def first_call(code, address, is_function):
    """The target of the first `call rel32` among the first CALL_SCAN bytes of `code` (the
    function at `address`) that lands on a function start, or None."""
    for i in range(min(len(code) - 4, CALL_SCAN)):
        if code[i] == 0xE8:
            target = (address + i + 5 + struct.unpack_from('<i', code, i + 1)[0]) & 0xFFFFFFFF
            if is_function(target):
                return target
    return None


class Resolver:
    """Pairs for the addresses of one index. `symbols` maps an address to the symbol that
    names it best (symbol table, claim, functions.csv; harness/lib/symtab.py `lookup`), and
    `read(address, size)` returns the original's bytes (or None when no image is at hand, in
    which case the class alone decides)."""

    def __init__(self, index, symbols, read=None):
        self.index, self.symbols, self.read = index, symbols, read
        self.real, self.deleting, self.info = {}, {}, {}
        self.unread = False  # a read returned nothing: the reasons say the bytes were not read
        for fn in index.functions:
            if fn.kind != 'function' or fn.is_library:
                continue
            role, kind, cls = classify(symbols.get(fn.address) or fn.mangled, fn.name)
            if not role:
                continue
            self.info[fn.address] = (role, kind, cls)
            (self.real if role == 'real' else self.deleting).setdefault(cls, []).append(fn.address)

    def _target(self, address):
        """Where the bytes of the deleting destructor at `address` first call, or None."""
        fn = self.index.by_address.get(address)
        if self.read is None or fn is None or fn.size <= 0:
            return None
        code = self.read(address, min(fn.size, CALL_SCAN + 4))
        if not code:
            self.unread = True
            return None
        return first_call(code, address, lambda a: a in self.index.by_address)

    def _calls(self, address, real):
        """True unless the bytes of the deleting destructor at `address` show another callee."""
        t = self._target(address)
        return t is None or t == real

    def pair_of(self, address):
        """A Pair, an Unpaired (a deleting destructor with no identifiable real destructor), or
        None (the function is not part of a pair)."""
        got = self.info.get(address)
        if not got:
            return None
        role, kind, cls = got
        if role == 'real':
            siblings = [d for d in self.deleting.get(cls, []) if self._calls(d, address)]
            if not siblings:
                return None
            return Pair('real', None, sorted(siblings), 'class symbols')
        by_class = self.real.get(cls, [])
        by_class = by_class[0] if len(by_class) == 1 else None
        called = self._target(address)
        if called is not None and (
                self.info.get(called, ('',))[0] == 'deleting'
                or called not in self.info and self.index.by_address[called].has_name):
            return Unpaired(f'its first call goes to 0x{called:08x}, named '
                            f'{self.index.by_address[called].name}, which is not a destructor')
        if by_class and called and by_class != called:
            return Unpaired(f'the class says the destructor is 0x{by_class:08x} but the bytes '
                            f'call 0x{called:08x}')
        real = by_class or called
        if real is None:
            return Unpaired('no real destructor of its class is named, and ' + (
                'no call near its start reaches a function' if self.read and not self.unread else
                'the original bytes were not read'))
        how = 'class symbols and bytes' if by_class and called else (
            'class symbols' if by_class else 'the call in its bytes')
        sibs = [d for d in self.deleting.get(cls, []) if d != address and self._calls(d, real)]
        return Pair('deleting', kind, [real] + sorted(sibs), how)


def describe(pair, states, names):
    """The text a session reads about its pair: `states` maps a partner address to 'exact',
    'claimed' or 'open', `names` to its name."""
    lines = []
    if pair['role'] == 'deleting':
        lines.append(f'This address is the {pair["kind"]} deleting destructor of its class. It '
                     'has no source of its own: the compiler generates it from the class\'s '
                     'out-of-class destructor definition. So its work is the pair below, and you '
                     'may claim both addresses.')
    else:
        lines.append('The deleting destructor generated from this destructor sits at another '
                     'address, and this session covers both: you may claim all the addresses '
                     'below.')
    for a in pair['partners']:
        st = states.get(a, 'open')
        lines.append(f'- {a} {names.get(a, "")}: ' + {
            'exact': 'already exact; leave its annotation where it is and keep it compiling.',
            'claimed': 'already claimed in the source, not exact; keep its annotation.',
            'open': 'not claimed yet; claim it as part of this session.'}[st])
    return '\n'.join(lines)
