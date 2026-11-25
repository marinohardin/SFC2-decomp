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
SCN_LNK_COMDAT = 0x1000
COMDAT_SELECT_NAMES = {1: 'no duplicates', 2: 'any', 3: 'same size', 4: 'exact match',
                       5: 'associative', 6: 'largest'}
TEXT_CHARACTERISTICS = 0x60500020  # CNT_CODE | ALIGN_16BYTES | MEM_EXECUTE | MEM_READ
RDATA_CHARACTERISTICS = 0x40301040  # CNT_INITIALIZED_DATA | LNK_COMDAT | ALIGN_4BYTES | MEM_READ


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
    __slots__ = ('number', 'name', 'data', 'relocs', 'characteristics', 'selection')

    def __init__(self, number, name, data, relocs, characteristics):
        self.number, self.name, self.data = number, name, data
        self.relocs, self.characteristics = relocs, characteristics  # relocs: [(offset, symidx, type)]
        self.selection = None  # a COMDAT section's selection kind (COMDAT_SELECT_NAMES), else None


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
            if naux and cls == SYM_CLASS_STATIC and sec in self.sections \
                    and self.sections[sec].characteristics & SCN_LNK_COMDAT:
                # the section symbol's auxiliary record ends with the COMDAT selection byte
                self.sections[sec].selection = raw[symoff + (i + 1) * 18 + 14]
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


def write_object(code, func_name, relocs, local_symbols=(), data=()):
    """Write an i386 COFF object holding `code` as the function `func_name`.

    relocs: [(offset, target_name, type)]; targets are undefined externals unless listed in
    local_symbols as (name, value), which are defined in the same section, or in data as
    (name, bytes), each defined at the start of a read-only data section of its own (the way VC6
    emits a constant or a string literal).
    Returns the object bytes.
    """
    symbols = []   # (name, value, section, type, storage, aux bytes)
    aux = struct.pack('<IHHIHB3x', len(code), len(relocs), 0, 0, 0, 0)
    symbols.append(('.text', 0, 1, 0, SYM_CLASS_STATIC, aux))
    symbols.append((func_name, 0, 1, SYM_TYPE_FUNCTION, SYM_CLASS_EXTERNAL, b''))
    index = {}
    for i, (name, _) in enumerate(data):
        symbols.append((name, 0, 2 + i, 0, SYM_CLASS_EXTERNAL, b''))
        index[name] = sum(1 + (1 if a else 0) for *_, a in symbols) - 1

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

    nsec = 1 + len(data)
    dataoff = 20 + 40 * nsec
    reloff = dataoff + len(code)
    rdataoff = reloff + 10 * len(relocs)
    symoff = rdataoff + sum(len(b) for _, b in data)
    out = bytearray(struct.pack('<HHIIIHH', IMAGE_FILE_MACHINE_I386, nsec, 0, symoff, nsym, 0, 0))
    out += struct.pack('<8sIIIIIIHHI', b'.text', 0, 0, len(code), dataoff,
                       reloff if relocs else 0, 0, len(relocs), 0, TEXT_CHARACTERISTICS)
    at = rdataoff
    for _, b in data:
        out += struct.pack('<8sIIIIIIHHI', b'.rdata', 0, 0, len(b), at, 0, 0, 0, 0,
                           RDATA_CHARACTERISTICS)
        at += len(b)
    out += code
    for off, target, typ in relocs:
        out += struct.pack('<IIH', off, index[target], typ)
    for _, b in data:
        out += b
    out += symtab + strings
    return bytes(out)


# objdiff cannot load a type 0x0B relocation (it reports an "unsupported x86 implicit relocation"),
# so the copy of an object it is given has each one rewritten as a plain 32-bit address
# (REL_DIR32). Both sides get the same rewrite and no byte of section data changes; the harness's
# own strict check (target.py) is what judges a SECREL field.
OBJDIFF_REWRITE = {REL_SECREL: REL_DIR32}


def rewrite_reloc_types(raw, mapping=None):
    """(object bytes, number of relocation records changed): `raw` with each relocation type in
    `mapping` (default OBJDIFF_REWRITE) replaced by the one it maps to. Only the 2-byte type of a
    record changes; sizes, offsets, symbols and section data stay as they were."""
    mapping = OBJDIFF_REWRITE if mapping is None else mapping
    if len(raw) < 20:
        raise CoffError('file too short for a COFF header')
    nsec = struct.unpack_from('<H', raw, 2)[0]
    opt = struct.unpack_from('<H', raw, 16)[0]
    out, changed = bytearray(raw), 0
    for i in range(nsec):
        h = 20 + opt + i * 40
        if h + 40 > len(raw):
            raise CoffError('truncated section table')
        reloff = struct.unpack_from('<I', raw, h + 24)[0]
        nrel = struct.unpack_from('<H', raw, h + 32)[0]
        if struct.unpack_from('<I', raw, h + 36)[0] & 0x01000000 and nrel == 0xFFFF:
            raise CoffError('extended relocation count (IMAGE_SCN_LNK_NRELOC_OVFL) not supported')
        if nrel and reloff + nrel * 10 > len(raw):
            raise CoffError('truncated relocation table')
        for j in range(nrel):
            at = reloff + j * 10 + 8
            typ = struct.unpack_from('<H', raw, at)[0]
            if typ in mapping:
                struct.pack_into('<H', out, at, mapping[typ])
                changed += 1
    return bytes(out), changed


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
        # The thread-local storage directory (data directory 9). `tls_start` is the address its
        # raw data starts at, which a section-relative (type 0x0B) field is counted from, and
        # `tls_end` where the block ends (raw data and zero fill); None when the image has no
        # such directory.
        self.tls_start = self.tls_end = None
        if optsize >= 96 + 10 * 8 and struct.unpack_from('<I', raw, opt + 92)[0] > 9:
            tls_rva, tls_size = struct.unpack_from('<II', raw, opt + 96 + 9 * 8)
            if tls_rva and tls_size >= 24:
                try:
                    # StartAddressOfRawData, EndAddressOfRawData, then SizeOfZeroFill at +16
                    start, end, _, _, fill = struct.unpack(
                        '<5I', self.read(self.image_base + tls_rva, 20))
                    self.tls_start, self.tls_end = start, end + fill
                except CoffError:
                    pass

    def read(self, address, size):
        rva = address - self.image_base
        for _, va, vsize, roff, rsize in self.sections:
            if va <= rva and rva + size <= va + max(vsize, rsize):
                start = rva - va
                data = self.raw[roff + start:roff + min(start + size, rsize)]
                return data + b'\0' * (size - len(data))
        raise CoffError(f'address {address:#x}+{size} is not inside one section of the image')
