"""Minimal i386 COFF object reader and writer, and a PE image reader (stdlib only).

Enough to pull one function out of a VC6 object, and to write the one-section target object that
objdiff compares it against.
"""
import struct

IMAGE_FILE_MACHINE_I386 = 0x14C
# i386 relocation types
REL_DIR32 = 0x06
REL_DIR32NB = 0x07
REL_SECTION = 0x0A
REL_SECREL = 0x0B
REL_REL32 = 0x14
REL_NAMES = {REL_DIR32: 'DIR32', REL_DIR32NB: 'DIR32NB', REL_SECTION: 'SECTION',
             REL_SECREL: 'SECREL', REL_REL32: 'REL32'}

SYM_CLASS_EXTERNAL = 2
SYM_CLASS_STATIC = 3
SYM_CLASS_LABEL = 6
SYM_TYPE_FUNCTION = 0x20
TEXT_CHARACTERISTICS = 0x60500020  # CNT_CODE | ALIGN_16BYTES | MEM_EXECUTE | MEM_READ


class CoffError(Exception):
    pass


class Symbol:
    __slots__ = ('index', 'name', 'value', 'section', 'type', 'storage', 'naux')

    def __init__(self, index, name, value, section, type_, storage, naux):
        self.index, self.name, self.value, self.section = index, name, value, section
        self.type, self.storage, self.naux = type_, storage, naux

    @property
    def is_function(self):
        return self.section > 0 and (self.type & 0x30) == SYM_TYPE_FUNCTION

    def __repr__(self):
        return f'Symbol({self.name!r}, sec={self.section}, value={self.value:#x})'


class Section:
    __slots__ = ('number', 'name', 'data', 'relocs', 'characteristics')

    def __init__(self, number, name, data, relocs, characteristics):
        self.number, self.name, self.data = number, name, data
        self.relocs, self.characteristics = relocs, characteristics  # relocs: [(offset, symidx, type)]


class CoffObject:
    """A parsed COFF object: sections (1-based), symbols by table index, symbols by name."""

    def __init__(self, raw):
        if len(raw) < 20:
            raise CoffError('file too short for a COFF header')
        machine, nsec, _, symoff, nsym, opt, _ = struct.unpack_from('<HHIIIHH', raw, 0)
        if machine != IMAGE_FILE_MACHINE_I386:
            raise CoffError(f'not an i386 COFF object (machine {machine:#x})')
        strtab = symoff + nsym * 18
        self._strings = raw[strtab:]
        self.sections = {}
        for i in range(nsec):
            h = 20 + opt + i * 40
            name, _, _, size, dataoff, reloff, _, nrel, _, chars = struct.unpack_from(
                '<8sIIIIIIHHI', raw, h)
            data = raw[dataoff:dataoff + size] if dataoff else b'\0' * size
            relocs = [struct.unpack_from('<IIH', raw, reloff + j * 10) for j in range(nrel)]
            self.sections[i + 1] = Section(i + 1, self._name(name), data, relocs, chars)
        self.symbols = {}
        i = 0
        while i < nsym:
            name, value, sec, typ, cls, naux = struct.unpack_from('<8sIhHBB', raw, symoff + i * 18)
            self.symbols[i] = Symbol(i, self._name(name), value, sec, typ, cls, naux)
            i += 1 + naux
        self.by_name = {}  # the first defined symbol of a name wins over undefined ones
        for s in self.symbols.values():
            prev = self.by_name.get(s.name)
            if prev is None or prev.section <= 0 < s.section:
                self.by_name[s.name] = s

    def _name(self, raw8):
        if raw8[:4] == b'\0\0\0\0':
            off = struct.unpack_from('<I', raw8, 4)[0]
            end = self._strings.index(b'\0', off)
            return self._strings[off:end].decode('latin1')
        if raw8[:1] == b'/' and raw8[1:].rstrip(b'\0').isdigit():  # long section name
            off = int(raw8[1:].rstrip(b'\0'))
            end = self._strings.index(b'\0', off)
            return self._strings[off:end].decode('latin1')
        return raw8.rstrip(b'\0').decode('latin1')

    def function(self, name):
        """(section, offset, size) of a defined function symbol. A function ends at the next
        function symbol in its section, or at the section end (VC6 /Gy puts each in a COMDAT)."""
        sym = self.by_name.get(name)
        if sym is None or sym.section <= 0:
            raise CoffError(f'symbol {name} is not defined in the object')
        sec = self.sections[sym.section]
        later = [s.value for s in self.symbols.values()
                 if s.section == sym.section and s.is_function and s.value > sym.value]
        end = min(later) if later else len(sec.data)
        return sec, sym.value, end - sym.value

    def function_names(self):
        return [s.name for s in self.symbols.values()
                if s.is_function and s.storage == SYM_CLASS_EXTERNAL]


def write_object(code, func_name, relocs, local_symbols=()):
    """Write a one-section i386 COFF object holding `code` as the function `func_name`.

    relocs: [(offset, target_name, type)]; targets are undefined externals unless listed in
    local_symbols as (name, value), which are defined in the same section.
    Returns the object bytes.
    """
    symbols = []   # (name, value, section, type, storage, aux bytes)
    aux = struct.pack('<IHHIHB3x', len(code), len(relocs), 0, 0, 0, 0)
    symbols.append(('.text', 0, 1, 0, SYM_CLASS_STATIC, aux))
    symbols.append((func_name, 0, 1, SYM_TYPE_FUNCTION, SYM_CLASS_EXTERNAL, b''))
    index = {}

    def slot():
        return sum(1 + (1 if a else 0) for *_, a in symbols)

    for name, value in local_symbols:
        if name not in index and name != func_name and name != '.text':
            index[name] = slot()
            symbols.append((name, value, 1, 0, SYM_CLASS_STATIC, b''))
    index.setdefault('.text', 0)
    index.setdefault(func_name, 1)
    for _, target, _ in relocs:
        if target not in index:
            index[target] = slot()
            symbols.append((target, 0, 0, 0, SYM_CLASS_EXTERNAL, b''))

    strings = bytearray(4)
    symtab = bytearray()
    for name, value, sec, typ, cls, a in symbols:
        enc = name.encode('latin1')
        if len(enc) <= 8:
            raw = enc.ljust(8, b'\0')
        else:
            raw = struct.pack('<II', 0, len(strings))
            strings += enc + b'\0'
        symtab += raw + struct.pack('<IhHBB', value, sec, typ, cls, 1 if a else 0) + a
    struct.pack_into('<I', strings, 0, len(strings))
    nsym = len(symtab) // 18

    dataoff = 20 + 40
    reloff = dataoff + len(code)
    symoff = reloff + 10 * len(relocs)
    out = bytearray(struct.pack('<HHIIIHH', IMAGE_FILE_MACHINE_I386, 1, 0, symoff, nsym, 0, 0))
    out += struct.pack('<8sIIIIIIHHI', b'.text', 0, 0, len(code), dataoff,
                       reloff if relocs else 0, 0, len(relocs), 0, TEXT_CHARACTERISTICS)
    out += code
    for off, target, typ in relocs:
        out += struct.pack('<IIH', off, index[target], typ)
    out += symtab + strings
    return bytes(out)


class PeImage:
    """Read bytes of a PE image by virtual address."""

    def __init__(self, raw):
        if raw[:2] != b'MZ':
            raise CoffError('not a PE image')
        pe = struct.unpack_from('<I', raw, 0x3C)[0]
        if raw[pe:pe + 4] != b'PE\0\0':
            raise CoffError('missing PE signature')
        _, nsec, _, _, _, optsize, _ = struct.unpack_from('<HHIIIHH', raw, pe + 4)
        opt = pe + 24
        self.image_base = struct.unpack_from('<I', raw, opt + 28)[0]
        self.raw = raw
        self.sections = []
        for i in range(nsec):
            h = opt + optsize + i * 40
            name, vsize, va, rsize, roff = struct.unpack_from('<8sIIII', raw, h)
            self.sections.append((name.rstrip(b'\0').decode('latin1'), va, vsize, roff, rsize))

    def read(self, address, size):
        rva = address - self.image_base
        for _, va, vsize, roff, rsize in self.sections:
            if va <= rva and rva + size <= va + max(vsize, rsize):
                start = rva - va
                data = self.raw[roff + start:roff + min(start + size, rsize)]
                return data + b'\0' * (size - len(data))
        raise CoffError(f'address {address:#x}+{size} is not inside one section of the image')
