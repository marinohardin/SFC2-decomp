"""Build the objdiff target for one original function: its bytes from the exe, wrapped in a
one-section COFF object whose relocations mirror the rebuilt object's.

A relocated field in the exe holds a final address, where the rebuilt object holds an addend and
a symbol. For each relocation of the rebuilt function, the address the exe uses is computed from
the exe bytes and checked:

- ok          the symbol's address is known (functions.csv, SDK matches, or a label in the
              function's own section) and the exe uses exactly that address;
- unverified  nothing is known about the symbol, and the function uses one address for it
              consistently, not one already known under another name;
- mismatch    anything else: a wrong callee, a wrong global, an inconsistent use, a field past
              the end of the original.

For ok and unverified relocations the field is masked (the target gets the rebuilt addend and the
same symbol name), so objdiff compares them as equal. A mismatch keeps the exe bytes and points
the relocation at a symbol named after the address the exe uses, so objdiff reports it.
"""
import struct

from . import coff

MASK = 0xFFFFFFFF
PADDING = (0x90, 0xCC)


def _u32(b, o):
    return struct.unpack_from('<I', b, o)[0]


def _local_name(name):
    """Labels and compiler-generated statics that never carry a name in the exe."""
    return name.startswith(('$', '.', '_$', '??_C@', '__real@', '__tls')) or name == ''


class Reloc:
    __slots__ = ('offset', 'type', 'symbol', 'exe_address', 'status', 'detail')

    def __init__(self, offset, type_, symbol, exe_address, status, detail=''):
        self.offset, self.type, self.symbol = offset, type_, symbol
        self.exe_address, self.status, self.detail = exe_address, status, detail

    def as_dict(self):
        return dict(offset=self.offset, type=coff.REL_NAMES.get(self.type, hex(self.type)),
                    symbol=self.symbol,
                    exe_address=None if self.exe_address is None else f'0x{self.exe_address:08x}',
                    status=self.status, detail=self.detail)


def target_length(fn_address, fn_size, base_size, exe, next_start):
    """How many original bytes to compare: the function's size in functions.csv, extended to the
    rebuilt size when that is larger (an analyzer may stop short of an inline jump table) but
    never into the next function, and trimmed of trailing int3/nop padding."""
    n = fn_size or base_size
    if base_size > n:
        limit = (next_start - fn_address) if next_start else base_size
        n = max(n, min(base_size, limit))
    data = exe.read(fn_address, n)
    if n > base_size and all(b in PADDING for b in data[base_size:]):
        data = data[:base_size]
    return data


def build(fn_address, fn_size, obj, symbol, exe, index):
    """Returns dict(base, target, object, relocs, strict, first_diff)."""
    sec, off, bsize = obj.function(symbol)
    base = sec.data[off:off + bsize]
    target = bytearray(target_length(fn_address, fn_size, bsize, exe, index.next_start(fn_address)))
    seen = {}
    relocs, out_relocs, locals_ = [], [], []
    for r_off, symidx, typ in sorted(sec.relocs):
        if not off <= r_off < off + bsize:
            continue
        o = r_off - off
        sym = obj.symbols[symidx]
        name = sym.name
        if sym.section == sec.number:
            locals_.append((name, sym.value - off))
        if o + 4 > len(target):
            relocs.append(Reloc(o, typ, name, None, 'mismatch', 'past the end of the original'))
            continue
        fb, ft = _u32(base, o), _u32(target, o)
        if typ == coff.REL_DIR32:
            s_exe = (ft - fb) & MASK
        elif typ == coff.REL_REL32:
            s_exe = (ft + fn_address + o + 4 - fb) & MASK
        elif typ == coff.REL_DIR32NB:
            s_exe = (ft + exe.image_base - fb) & MASK
        else:
            st = 'ok' if ft == fb else 'mismatch'
            relocs.append(Reloc(o, typ, name, None, st, 'compared as raw bytes'))
            out_relocs.append((o, name, typ))
            continue
        status, detail = _check(name, sym, sec, off, s_exe, fn_address, index, seen)
        relocs.append(Reloc(o, typ, name, s_exe, status, detail))
        if status == 'mismatch':
            out_relocs.append((o, f'$exe_{s_exe:08x}', typ))
        else:
            target[o:o + 4] = base[o:o + 4]
            out_relocs.append((o, name, typ))
    target = bytes(target)
    first_diff = next((i for i in range(min(len(base), len(target))) if base[i] != target[i]),
                      None if len(base) == len(target) else min(len(base), len(target)))
    strict = (first_diff is None and all(r.status != 'mismatch' for r in relocs))
    objbytes = coff.write_object(target, symbol, out_relocs, locals_)
    return dict(base=base, target=target, object=objbytes, relocs=relocs, strict=strict,
                first_diff=first_diff)


def _check(name, sym, sec, off, s_exe, fn_address, index, seen):
    if sym.section == -1:  # IMAGE_SYM_ABSOLUTE, e.g. __except_list (fs:[0])
        if s_exe == sym.value:
            return 'ok', 'absolute'
        return 'mismatch', f'absolute symbol is 0x{sym.value:08x}'
    if sym.section == sec.number:
        want = fn_address + sym.value - off
        if s_exe == want:
            return 'ok', 'own section'
        return 'mismatch', f'own-section label should be 0x{want:08x}'
    known = index.known.get(name)
    if known:
        if s_exe in known:
            return 'ok', 'known address'
        return 'mismatch', 'known at ' + ', '.join(f'0x{a:08x}' for a in sorted(known))
    if name in seen and seen[name] != s_exe:
        return 'mismatch', f'used for 0x{seen[name]:08x} earlier in this function'
    seen[name] = s_exe
    others = index.names_at.get(s_exe)
    if others and not _local_name(name):
        return 'mismatch', 'the exe address is known as ' + ', '.join(sorted(others))
    return 'unverified', 'no address known for this symbol'
