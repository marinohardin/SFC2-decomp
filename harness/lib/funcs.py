"""The original functions (ghidra/exports/functions.csv) and the name -> address evidence used to
check relocation targets."""
import bisect
import csv
import os

# Tiers of the SDK matcher that are good enough to be labels (analysis/README.md, P0-7).
LABEL_TIERS = {'exact', 'exact+ctx', 'masked', 'masked+ctx', 'ref'}
LABEL_MIN_SCORE = 0.85


class Function:
    __slots__ = ('address', 'size', 'name', 'mangled', 'demangled', 'name_source',
                 'translation_unit', 'is_library', 'library', 'kind', 'tags')

    def __init__(self, row):
        self.address = int(row['address'], 16)
        self.size = int(row['size'] or 0)
        self.name = row['name']
        self.mangled = row.get('mangled') or ''
        self.demangled = row.get('demangled') or ''
        self.name_source = row.get('name_source') or ''
        self.translation_unit = row.get('translation_unit') or ''
        self.is_library = (row.get('is_library') or '0').strip() == '1'
        self.library = row.get('library') or ''
        self.kind = row.get('kind') or 'function'
        self.tags = tuple(t for t in (row.get('tags') or '').split(';') if t)

    @property
    def id(self):
        return f'0x{self.address:08x}'

    @property
    def has_name(self):
        """Named from evidence, not a placeholder (Ghidra's FUN_, or an analyzer's guess)."""
        return not (self.name.startswith(('FUN_', 'fn_', 'thunk_FUN_', 'LAB_'))
                    or '::vfunction' in self.name or self.name_source in ('default', ''))

    @property
    def has_prototype(self):
        return '(' in self.demangled and 'undefined' not in self.demangled


class FunctionIndex:
    def __init__(self, functions_csv, symbol_tsvs=()):
        with open(functions_csv, newline='') as f:
            self.functions = [Function(r) for r in csv.DictReader(f)]
        self.by_address = {fn.address: fn for fn in self.functions}
        self.starts = sorted(self.by_address)
        # symbol name -> set of addresses; fs:[0], the SEH chain head, is absolute zero
        self.known = {'__except_list': {0}}
        for fn in self.functions:
            if fn.mangled:
                self.known.setdefault(fn.mangled, set()).add(fn.address)
        for path in symbol_tsvs:
            if os.path.exists(path):
                self._load_tsv(path)
        self.names_at = {}   # address -> set of symbol names
        for name, addrs in self.known.items():
            for a in addrs:
                self.names_at.setdefault(a, set()).add(name)
        self.claimed = {}    # symbol name -> addresses a source claim gives it (add_claims)

    def add_claims(self, addresses, table):
        """Let an address claimed in the source tree count as the address of the symbol its
        symbol-table row carries. `addresses` are the ints a `// FUNCTION:` annotation claims;
        `table` is symtab.load_table's {address: (symbol, evidence)}. It is a separate record
        (`claimed`), consulted by target._check only to accept a relocation: it never adds to
        `known` or `names_at`, so it can refuse nothing that passed before. A claim is not
        credited when its row is not a `claim` row (`mangled`, `add` and `callers` rows are
        evidence of their own or provisional), or when the evidence already names the address
        with another mangled name (the claim contradicts it). Returns how many were credited."""
        n = 0
        for a in addresses:
            row = table.get(f'0x{a:08x}')
            if not row or row[1] != 'claim' or not row[0].startswith('?'):
                continue
            sym = row[0]
            if any(x.startswith('?') and x != sym for x in self.names_at.get(a, ())):
                continue
            if a not in self.known.get(sym, ()):
                self.claimed.setdefault(sym, set()).add(a)
                n += 1
        return n

    def _load_tsv(self, path):
        with open(path, newline='') as f:
            for row in csv.DictReader(f, delimiter='\t'):
                name = row.get('mangled_name') or row.get('name') or ''
                tier = row.get('tier', '')
                try:
                    score = float(row.get('score') or 0)
                except ValueError:
                    continue
                if tier == 'import' or (tier in LABEL_TIERS | {'data'} and score >= LABEL_MIN_SCORE):
                    if name and not name.startswith('.'):
                        self.known.setdefault(name, set()).add(int(row['address'], 16))

    def next_start(self, address):
        """The first function start after `address`, or None."""
        i = bisect.bisect_right(self.starts, address)
        return self.starts[i] if i < len(self.starts) else None

    def resolve(self, ident):
        """A function by id: 0x006fc690, 006fc690, fn_006fc690, FUN_006fc690, a name or a
        mangled name. Raises KeyError with a reason."""
        s = ident.strip()
        low = s.lower()
        for prefix in ('fn_', 'fun_', '0x'):
            if low.startswith(prefix):
                low = low[len(prefix):]
        try:
            addr = int(low, 16)
        except ValueError:
            addr = None
        if addr is not None and addr in self.by_address:
            return self.by_address[addr]
        hits = [fn for fn in self.functions if s in (fn.name, fn.mangled)]
        if len(hits) == 1:
            return hits[0]
        if len(hits) > 1:
            raise KeyError(f'{ident} names {len(hits)} functions; use the address')
        raise KeyError(f'no function {ident} in functions.csv')
