#!/usr/bin/env python3
"""Instruction-level fuzzy matcher: SDK static-library functions -> addresses in an exe.

Usage:
  fuzzymatch.py --exe EXE --out OUT.tsv [--truth libmatch.tsv] [--calib CALIB.txt]
                [--ghidra functions.csv] [--data-out DATA.tsv] LIB...

Pipeline (see analysis/README.md for the write-up and the calibration numbers):
  1. Every library COMDAT code section and every exe function candidate is disassembled with
     Capstone and turned into three token streams per instruction:
       A  mnemonic + operands; relocated / address-like values masked as A, calls as CALL,
          jumps leaving the function as X, intra-function jumps keep their displacement;
       B  as A, but every remaining immediate, displacement and jump distance masked;
       C  operand kinds only (mnemonic, r/i/m/A/CALL): survives register re-allocation.
     Jump/byte tables addressed from inside the function truncate the stream.
  2. Tier a ("exact"): A-stream equality at a candidate start.  Tier b ("masked"): B-stream
     equality.  Candidates that several names share, or that one name has at several addresses,
     are resolved with context: relocation targets of the library function (callees, globals,
     vtables, imports) must agree with what the exe instruction points at for every name whose
     address is already known, and callers already placed vote for their callees.
  3. Tier c ("fuzzy"): n-gram retrieval + difflib alignment ratio of C streams (--stream), length and call
     count close, no disagreement with known callees, and a margin over the runner-up.
  4. "ref": names that appear only as relocation targets (callees, vtable slots) of placed
     functions; the address comes from the aligned exe instruction.
"""
import sys, os, struct, argparse, collections, bisect, difflib, re, random, time
import pefile, capstone
from capstone import x86

X86_OP_REG, X86_OP_IMM, X86_OP_MEM = x86.X86_OP_REG, x86.X86_OP_IMM, x86.X86_OP_MEM
GRP_CALL, GRP_REL = capstone.CS_GRP_CALL, capstone.CS_GRP_BRANCH_RELATIVE

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
MD.detail = True

INTERN = {}
def intern(s):
    v = INTERN.get(s)
    if v is None: v = INTERN[s] = len(INTERN)
    return v

T_CALL = None  # filled after interning helpers exist
PAD_MNEMS = {'int3', 'nop'}

def log(*a):
    print(*a, file=sys.stderr, flush=True)

# ----------------------------------------------------------------------------------------------
# Tokeniser

class Toks:
    __slots__ = ('A', 'B', 'C', 'addr', 'refs', 'jt', 'pad', 'ncall', 'trunc')

def tokenize(code, va, classify, lo_addr, hi_addr, data_ref):
    """Disassemble code at va.  classify(ins, enc_off, value, is_rel) -> (cls, ref) where cls is
    'CALL', 'X', 'A', 'j' (ref = jump displacement) or 'v' (plain value).
    data_ref(ins, enc_off, value) -> target va if the mem operand addresses data inside this
    function (switch tables), else None.  Returns Toks."""
    t = Toks()
    A = []; B = []; C = []; addr = []; refs = {}; jt = {}; pad = set()
    datastart = None
    pos = 0; n = len(code)
    while pos < n:
        stopped = True
        for ins in MD.disasm(code[pos:], va + pos):
            off = ins.address - va
            pos = off + ins.size
            m = ins.mnemonic
            if m in PAD_MNEMS:
                pad.add(len(A)); continue
            sa = [m]; sb = [m]; sc = [m]; rr = []
            rel = ins.group(GRP_REL)
            for op in ins.operands:
                ty = op.type
                if ty == X86_OP_REG:
                    r = ins.reg_name(op.reg); sa.append(r); sb.append(r); sc.append('r')
                elif ty == X86_OP_IMM:
                    v = op.imm & 0xffffffff
                    cls, ref = classify(ins, ins.imm_offset, v, rel)
                    if cls == 'v':
                        sa.append('i%x' % v); sb.append('I'); sc.append('i')
                    elif cls == 'j':
                        sa.append('j%d' % ref); sb.append('J'); sc.append('J')
                        jt[len(A)] = v
                    else:
                        sa.append(cls); sb.append(cls); sc.append(cls); rr.append((cls, ref))
                elif ty == X86_OP_MEM:
                    mm = op.mem
                    base = ins.reg_name(mm.base) if mm.base else ''
                    idx = ins.reg_name(mm.index) if mm.index else ''
                    seg = ins.reg_name(mm.segment) + ':' if mm.segment else ''
                    if idx: idx = '+%s*%d' % (idx, mm.scale)
                    d = mm.disp & 0xffffffff
                    if ins.disp_size:
                        cls, ref = classify(ins, ins.disp_offset, d, False)
                        tgt = data_ref(ins, ins.disp_offset, d)
                        if tgt is not None and (datastart is None or tgt < datastart):
                            datastart = tgt
                    else:
                        cls = 'n'
                    if cls == 'A':
                        da = db = 'A'; rr.append(('A', ref))
                    elif cls == 'n':
                        da = db = ''
                    else:
                        da = '%x' % d; db = 'D'
                    pre = '%s%d[%s%s' % (seg, op.size, base, idx)
                    sa.append(pre + '+' + da + ']'); sb.append(pre + '+' + db + ']')
                    sc.append('m%d%s' % (op.size, 'a' if cls == 'A' else ''))
            if rr: refs[len(A)] = tuple(rr)
            A.append(intern(' '.join(sa))); B.append(intern(' '.join(sb))); C.append(intern(' '.join(sc)))
            addr.append(ins.address)
            stopped = False
        else:
            pass
        if pos < n:
            # Capstone stopped on an undecodable byte (or the loop consumed nothing): emit BAD, skip it
            if not stopped or True:
                A.append(intern('BAD')); B.append(intern('BAD')); C.append(intern('BAD'))
                addr.append(va + pos); pos += 1
    t.trunc = False
    if datastart is not None:
        k = bisect.bisect_left(addr, datastart)
        if k < len(addr):
            t.trunc = True
            A = A[:k]; B = B[:k]; C = C[:k]; addr = addr[:k]
            refs = {i: r for i, r in refs.items() if i < k}
            jt = {i: r for i, r in jt.items() if i < k}
            pad = {i for i in pad if i <= k}
    t.A, t.B, t.C, t.addr, t.refs, t.jt, t.pad = A, B, C, addr, refs, jt, pad
    t.ncall = sum(1 for r in refs.values() for c, _ in r if c == 'CALL')
    return t

# ----------------------------------------------------------------------------------------------
# Library side

def parse_archive(path):
    data = open(path, 'rb').read()
    assert data[:8] == b'!<arch>\n', path
    off = 8; longnames = b''; members = []
    while off + 60 <= len(data):
        hdr = data[off:off + 60]
        name = hdr[:16].decode('latin1').rstrip()
        try: size = int(hdr[48:58].decode().strip())
        except ValueError: break
        body = data[off + 60:off + 60 + size]
        if name == '//': longnames = body
        elif name not in ('/', '//'):
            if name.startswith('/'):
                try:
                    i = int(name[1:]); name = longnames[i:longnames.index(b'\0', i)].decode('latin1')
                except Exception: pass
            members.append((name.rstrip('/'), body))
        off += 60 + size + (size & 1)
    return members

def parse_coff(obj):
    if len(obj) < 20: return None
    machine, nsec, ts, symoff, nsym, optsz, chars = struct.unpack_from('<HHIIIHH', obj, 0)
    if machine != 0x14c or symoff + nsym * 18 > len(obj): return None
    strtab = symoff + nsym * 18
    syms = [None] * nsym
    i = 0
    while i < nsym:
        raw = obj[symoff + i * 18: symoff + (i + 1) * 18]
        if len(raw) < 18: break
        nm, val, secnum, typ, sc, naux = struct.unpack('<8sIhHBB', raw)
        if nm[:4] == b'\0\0\0\0':
            so = struct.unpack('<I', nm[4:])[0]
            e = obj.find(b'\0', strtab + so); name = obj[strtab + so:e].decode('latin1')
        else: name = nm.rstrip(b'\0').decode('latin1')
        syms[i] = (name, val, secnum, sc, typ)
        i += 1 + naux
    secs = []
    for s in range(nsec):
        h = obj[20 + s * 40: 60 + s * 40]
        if len(h) < 40: break
        name, vsz, va, rawsz, rawptr, relptr, lnptr, nrel, nln, ch = struct.unpack('<8sIIIIIIHHI', h)
        relocs = []
        for k in range(nrel):
            ro = relptr + k * 10
            if ro + 10 > len(obj): break
            relocs.append(struct.unpack_from('<IIH', obj, ro))
        secs.append(dict(idx=s + 1, name=name.rstrip(b'\0').decode('latin1'), size=rawsz, ptr=rawptr,
                         relocs=relocs, ch=ch))
    return secs, syms

class LibFunc:
    __slots__ = ('name', 'lmname', 'libs', 'size', 'code', 'toks', 'n', 'key', 'id', 'pad')

def load_libs(paths, image_lo, image_hi):
    """Returns (funcs, dataobjs).  dataobjs: symbol name -> (seckey, offset); seckeys -> list of
    (offset, refname, addend, size)."""
    funcs = {}; seen_keys = {}
    data_sym = {}; data_sec = {}
    for lib in paths:
        libname = os.path.basename(lib)
        try: members = parse_archive(lib)
        except Exception as e: log(lib, 'ARCHIVE ERROR', e); continue
        for objname, body in members:
            r = parse_coff(body)
            if not r: continue
            secs, syms = r
            secbyidx = {s['idx']: s for s in secs}
            def refname(symidx, own, addend):
                s = syms[symidx] if symidx < len(syms) else None
                if s is None: return None, 0
                nm, val, secnum, sc, typ = s
                if secnum == own: return '<self>', val + addend
                if secnum == -1 or nm == '__except_list':   # absolute symbols, e.g. fs:[0]
                    return '<abs>', val + addend
                if sc == 3 and nm.startswith('.') and val == 0 and secnum > 0:
                    return '%s|%s#%d' % (objname, nm, secnum), addend
                if secnum > 0 and sc in (3, 6):  # static symbol / label inside another section
                    return '%s|%s' % (objname, nm), addend
                return nm, addend
            for sec in secs:
                if sec['ptr'] + sec['size'] > len(body) or sec['size'] == 0: continue
                raw = body[sec['ptr']:sec['ptr'] + sec['size']]
                rel = {}
                for va, si, ty in sec['relocs']:
                    if va + 4 > len(raw): continue
                    addend = struct.unpack_from('<i', raw, va)[0] if ty in (6, 7) else 0
                    nm, add = refname(si, sec['idx'], addend)
                    rel[va] = (ty, nm, add)
                if sec['ch'] & 0x20:
                    if sec['size'] < 1: continue
                    ext = [s for s in syms if s and s[2] == sec['idx'] and s[1] == 0 and s[3] == 2]
                    stat = [s for s in syms if s and s[2] == sec['idx'] and s[1] == 0 and s[3] == 3
                            and not s[0].startswith('.') and not s[0].startswith('$')]
                    lmname = ext[0][0] if ext else "%s:%s#%d" % (objname, sec['name'], sec['idx'])
                    name = ext[0][0] if ext else (('%s|%s' % (objname, stat[0][0])) if stat else lmname)
                    key = (name, raw, tuple(sorted((k, v[0], v[1]) for k, v in rel.items())))
                    if key in seen_keys:
                        f = seen_keys[key]
                        if libname not in f.libs: f.libs.append(libname)
                        continue
                    f = LibFunc(); f.name = name; f.lmname = lmname; f.libs = [libname]
                    f.size = len(raw); f.code = raw; f.key = key
                    f.pad = len(raw) - len(raw.rstrip(b'\x90'))   # in-section alignment padding
                    f.toks = tokenize_lib(raw, rel, image_lo, image_hi)
                    f.n = len(f.toks.A)
                    if f.n == 0: continue
                    seen_keys[key] = f
                    f.id = len(funcs); funcs[f.id] = f
                elif sec['ch'] & 0xC0 and rel:
                    seckey = '%s|%s#%d' % (objname, sec['name'], sec['idx'])
                    if seckey in data_sec: continue
                    data_sec[seckey] = [(off, v[1], v[2], v[0]) for off, v in sorted(rel.items()) if v[0] == 6]
                    data_sym.setdefault(seckey, (seckey, 0))
                    for s in syms:
                        if s and s[2] == sec['idx'] and s[3] in (2, 3) and not s[0].startswith('.'):
                            nm = s[0] if s[3] == 2 else '%s|%s' % (objname, s[0])
                            data_sym.setdefault(nm, (seckey, s[1]))
    return list(funcs.values()), data_sym, data_sec

def tokenize_lib(raw, rel, image_lo, image_hi):
    def classify(ins, enc, v, isrel):
        r = rel.get(ins.address + enc)
        if r and r[1] == '<abs>':
            return ('v', None)
        if r:
            ty, nm, add = r
            if ty == 20:
                if nm == '<self>': return ('CALL' if ins.group(GRP_CALL) else 'j', None if ins.group(GRP_CALL) else 0)
                return ('CALL' if ins.group(GRP_CALL) else 'X', (nm, 0))
            if ty == 6:
                return ('A', None if nm == '<self>' else (nm, add))
            return ('A', None)
        if isrel:
            if ins.group(GRP_CALL): return ('CALL', None)
            return ('j', v - (ins.address + ins.size))
        if image_lo <= v < image_hi: return ('A', None)
        return ('v', None)
    def data_ref(ins, enc, v):
        r = rel.get(ins.address + enc)
        if r and r[0] == 6 and r[1] == '<self>': return r[2]
        return None
    return tokenize(raw, 0, classify, 0, len(raw), data_ref)

# ----------------------------------------------------------------------------------------------
# Exe side

class Exe:
    pass

def load_exe(path, ghidra_csv=None):
    E = Exe()
    pe = pefile.PE(path, fast_load=True)
    E.base = pe.OPTIONAL_HEADER.ImageBase
    E.lo = E.base; E.hi = E.base + pe.OPTIONAL_HEADER.SizeOfImage
    text = [s for s in pe.sections if s.Name.startswith(b'.text')][0]
    E.T = text.get_data()[:text.Misc_VirtualSize]
    E.tva = E.base + text.VirtualAddress; E.tend = E.tva + len(E.T)
    E.sections = [(E.base + s.VirtualAddress, s.get_data(), s.Name.rstrip(b'\0').decode()) for s in pe.sections]
    # relocations (ServerPlatform has them): set of VAs holding absolute addresses
    E.relocs = None
    pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_IMPORT'],
                                           pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_BASERELOC']])
    if hasattr(pe, 'DIRECTORY_ENTRY_BASERELOC'):
        E.relocs = set()
        for blk in pe.DIRECTORY_ENTRY_BASERELOC:
            for e in blk.entries:
                if e.type == 3: E.relocs.add(E.base + e.rva)
    E.imports = {}
    for d in getattr(pe, 'DIRECTORY_ENTRY_IMPORT', []):
        for imp in d.imports:
            if imp.name: E.imports.setdefault(imp.name.decode('latin1'), imp.address)
    T = E.T; tva = E.tva
    # pass 0: linear sweep with resync, collect instruction boundaries and code references
    insb = bytearray(len(T)); aterm = bytearray(len(T) + 16)
    calls = set(); jmps = []; immrefs = set()
    pos = 0
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    while pos < len(T):
        for a, sz, m, o in md.disasm_lite(T[pos:], tva + pos):
            insb[a - tva] = 1; pos = a - tva + sz
            if m in ('ret', 'jmp', 'int3', 'nop'): aterm[pos] = 1
            if m == 'call' and o.startswith('0x'): calls.add(int(o, 16))
            elif m[0] == 'j' and o.startswith('0x'): jmps.append((a, int(o, 16)))
            elif m in ('push', 'mov') and ', 0x' in o or m == 'push' and o.startswith('0x'):
                v = int(o.rsplit('0x', 1)[1], 16)
                if tva <= v < E.tend: immrefs.add(v)
        else:
            pos += 1; aterm[pos] = 1
    ptrs = set()
    for va, d, nm in E.sections:
        if nm.startswith('.rdata') or nm.startswith('.data'):
            for i in range(0, len(d) - 3, 4):
                v = struct.unpack_from('<I', d, i)[0]
                if tva <= v < E.tend: ptrs.add(v)
    ghidra = set()
    if ghidra_csv and os.path.exists(ghidra_csv):
        for line in open(ghidra_csv):
            mm = re.search(r'(?:0x)?([0-9a-fA-F]{6,8})', line)
            if mm:
                v = int(mm.group(1), 16)
                if tva <= v < E.tend: ghidra.add(v)
    def ok(v):  # instruction boundary of the sweep, or right after padding
        i = v - tva
        return 0 <= i < len(T) and (insb[i] or T[i - 1] in (0xCC, 0x90))
    def after_end(v):  # previous instruction ends a function (ret/jmp/padding)
        i = v - tva
        return 0 <= i < len(T) and insb[i] and aterm[i]
    raw = calls | ptrs | immrefs
    # call targets need only an instruction boundary; data/immediate pointers (vtables, callbacks,
    # EH funclets) must also follow a ret/jmp/padding, which drops pointers into function bodies
    starts = {v for v in calls if ok(v)} | {v for v in ptrs | immrefs if after_end(v)} | ghidra
    # 16-aligned code right after ret/jmp + nop/int3 padding starts a function even if nothing
    # refers to it directly (the padded functions come from objects built like the SDK libs)
    padded = {tva + i for i in range(16 - tva % 16, len(T), 16)
              if insb[i] and aterm[i] and T[i - 1] in (0x90, 0xCC) and T[i] not in (0x90, 0xCC)}
    starts |= padded
    # jumps that leave their region are tail calls: their targets start functions too
    srt = sorted(starts)
    for a, tgt in jmps:
        if not (tva <= tgt < E.tend) or tgt in starts: continue
        k = bisect.bisect_right(srt, a) - 1
        lo = srt[k] if k >= 0 else tva; hi = srt[k + 1] if k + 1 < len(srt) else E.tend
        if not (lo <= tgt < hi) and after_end(tgt):
            starts.add(tgt)
    E.stats = dict(call_targets=len(calls), data_ptrs=len(ptrs), imm_refs=len(immrefs), padded=len(padded),
                   ghidra=len(ghidra), raw_union=len(raw), starts=len(starts))
    E.starts = sorted(starts)
    E.call_targets = calls
    return E

def tokenize_exe(E):
    """Global token stream: regions [start_i, start_i+1) concatenated."""
    GA = []; GB = []; GC = []; Gaddr = []; Grefs = {}; Gj = {}
    pos_of = {}; region_pos = []; strong = set()
    relocs = E.relocs; lo, hi = E.lo, E.hi
    starts = E.starts
    for i, s in enumerate(starts):
        e = starts[i + 1] if i + 1 < len(starts) else E.tend
        code = E.T[s - E.tva:e - E.tva]
        def classify(ins, enc, v, isrel, s=s, e=e):
            if isrel:
                if ins.group(GRP_CALL): return ('CALL', v)
                if not (s <= v < e): return ('X', v)
                return ('j', v - (ins.address + ins.size))
            if relocs is not None:
                if ins.address + enc in relocs: return ('A', v)
                return ('v', None)
            if lo <= v < hi: return ('A', v)
            return ('v', None)
        def data_ref(ins, enc, v, s=s, e=e):
            return v if s < v < e else None
        t = tokenize(code, s, classify, s, e, data_ref)
        p0 = len(GA)
        pos_of[s] = p0; region_pos.append(p0)
        GA.extend(t.A); GB.extend(t.B); GC.extend(t.C); Gaddr.extend(t.addr)
        for k, r in t.refs.items(): Grefs[p0 + k] = r
        for k, r in t.jt.items(): Gj[p0 + k] = r
        for k in t.pad: strong.add(p0 + k)
        if t.trunc: strong.add(p0 + len(t.A))
    E.GA, E.GB, E.GC, E.Gaddr, E.Grefs, E.Gj = GA, GB, GC, Gaddr, Grefs, Gj
    E.pos_of = pos_of; E.addr_of_pos = {p: a for a, p in pos_of.items()}
    E.region_pos = region_pos
    E.strong = sorted(strong | {len(GA)})
    E.N = len(GA)

def read_dword(E, va):
    for base, d, nm in E.sections:
        if base <= va < base + len(d) - 3:
            return struct.unpack_from('<I', d, va - base)[0]
    return None

# ----------------------------------------------------------------------------------------------
# Matching state

TIER_W = {'exact': 1.0, 'exact+ctx': 0.9, 'masked': 0.9, 'masked+ctx': 0.8, 'fuzzy': 0.5,
          'data': 0.8, 'ref': 0.6, 'import': 1.0, 'exact-short': 0.4}

class State:
    def __init__(self):
        self.known = {}      # name -> addr
        self.tier = {}       # name -> (tier, score, notes, size, fid)
        self.owner = collections.defaultdict(set)   # addr -> names with a placed body
        self.votes = collections.defaultdict(lambda: collections.defaultdict(dict))  # name->addr->{src:w}
        self.expanded = set()
        self.align = {}      # name -> (exe pos, alignment) for fuzzy placements

def ref_pairs(f, E, p, align=None):
    """Yield (name, exe_symbol_addr) for relocation targets of f at exe position p.
    align: list of (lib_tok, exe_pos) pairs; default 1:1."""
    it = align if align is not None else ((i, p + i) for i in f.toks.refs)
    for i, q in it:
        lr = f.toks.refs.get(i)
        if not lr: continue
        er = E.Grefs.get(q)
        if not er or len(er) != len(lr): continue
        for (lc, lref), (ec, ev) in zip(lr, er):
            if lc != ec or lref is None or ev is None: continue
            nm, add = lref
            yield nm, (ev - add) & 0xffffffff

def evidence(f, E, S, p, align=None):
    agree = dis = 0
    for nm, a in ref_pairs(f, E, p, align):
        k = S.known.get(nm)
        if k is None: continue
        if k == a: agree += 1
        else: dis += 1
    return agree, dis

def closed(f, E, p):
    """Every intra-function jump of the exe copy lands inside the matched byte range."""
    a0 = E.Gaddr[p]; q = p + f.n
    a1 = E.Gaddr[q] if q < E.N else E.tend
    for i in f.toks.jt:
        t = E.Gj.get(p + i)
        if t is not None and not (a0 <= t < a1): return False
    return True

def structural_candidates(funcs, E):
    idx = [None] + [collections.defaultdict(list) for _ in range(4)]
    for s, p in E.pos_of.items():
        for k in range(1, 5):
            if p + k <= E.N: idx[k][tuple(E.GB[p:p + k])].append(p)
    cand = {}
    GA, GB = E.GA, E.GB
    for f in funcs:
        n = f.n; k = min(n, 4)
        B = f.toks.B; A = f.toks.A
        res = []
        for p in idx[k].get(tuple(B[:k]), ()):
            if p + n <= E.N and GB[p:p + n] == B and closed(f, E, p):
                res.append((p, GA[p:p + n] == A))
        cand[f.id] = res
    return cand

def add_votes(S, E, f, p, src_tier, align=None):
    w = TIER_W.get(src_tier, 0.5)
    for nm, a in ref_pairs(f, E, p, align):
        if nm == f.name: continue
        S.votes[nm][a][f.name] = max(w, S.votes[nm][a].get(f.name, 0))

def place(S, E, f, p, tier, score, notes, align=None, size=None):
    a = E.Gaddr[p] if p is not None else None
    S.known[f.name] = a
    S.tier[f.name] = (tier, score, notes, size if size is not None else f.size, f.id)
    S.owner[a].add(f.name)
    if align is not None: S.align[f.name] = (p, align)
    add_votes(S, E, f, p, tier, align)

def expand_data(S, E, data_sym, data_sec):
    """Data objects (vtables, RTTI, tables) whose address is known vote for their slots."""
    new = 0
    for nm, a in list(S.known.items()):
        if nm in S.expanded or nm not in data_sym: continue
        S.expanded.add(nm)
        seckey, off = data_sym[nm]
        base = (a - off) & 0xffffffff
        slots = []
        for o, ref, add, ty in data_sec.get(seckey, ()):
            v = read_dword(E, base + o)
            if v is None or ref is None or ref == '<self>': continue
            slots.append((ref, (v - add) & 0xffffffff))
        agree = sum(1 for r, v in slots if S.known.get(r) == v)
        dis = sum(1 for r, v in slots if r in S.known and S.known[r] != v)
        if dis or not agree: continue
        for r, v in slots:
            S.votes[r][v]['data:' + seckey] = TIER_W['data']
            new += 1
    return new

def accept_votes(S, E, funcs_by_name):
    """Names with no placed body take the address their relocation votes agree on."""
    new = 0
    for nm, byaddr in S.votes.items():
        if nm in S.known: continue
        tot = {a: sum(srcs.values()) for a, srcs in byaddr.items()}
        if len(tot) != 1: continue
        (a, w), = tot.items()
        nsrc = len(byaddr[a])
        if w < 1.0 and not (nsrc >= 1 and w >= 0.9): continue
        intext = E.tva <= a < E.tend
        if nm in funcs_by_name and intext:
            continue   # handled by resolve(): body comparison first
        S.known[nm] = a
        score = round(min(0.99, 1 - 0.5 ** (w * 2)) * (1.0 if nsrc >= 2 else 0.8), 3)
        S.tier[nm] = ('ref' if intext else 'data', score, 'votes=%d from %s' % (
            nsrc, ','.join(sorted(byaddr[a])[:3])), 0, None)
        new += 1
    return new

def resolve(S, E, funcs, cand, allow_masked=True, min_short=6, exclude=None):
    """One round over unplaced functions.  Returns number placed."""
    decisions = []
    byname = collections.defaultdict(list)
    for f in funcs:
        if f.name in S.known or (exclude and f.name in exclude): continue
        byname[f.name].append(f)
    for nm, fs in byname.items():
        cv = S.votes.get(nm, {})
        allc = [(f, p, isA) for f in fs for p, isA in cand[f.id] if isA or allow_masked]
        if not allc: continue
        anyA = any(isA for _, _, isA in allc)
        if anyA: allc = [c for c in allc if c[2]]   # A-equal candidates take precedence
        seen_a = set(); uniq = []
        for c in allc:                               # one option per address (same name, several bodies)
            if E.Gaddr[c[1]] not in seen_a: seen_a.add(E.Gaddr[c[1]]); uniq.append(c)
        allc = uniq
        nall = len(allc)
        opts = []
        for f, p, isA in allc:
            a = E.Gaddr[p]
            own = S.owner.get(a)
            if own and not all(S.tier[o][4] is not None and funcs_by_id[S.tier[o][4]].toks.A == f.toks.A
                               for o in own):
                continue   # address taken by a different body
            ag, dis = evidence(f, E, S, p)
            if dis: continue
            v = sum(cv.get(a, {}).values())
            pm = 0.5 if padmatch(f, E, a) else 0.0
            opts.append((ag + v + pm, f, p, ag, v))
        if not opts: continue
        opts.sort(key=lambda o: o[0], reverse=True)
        ev, f, p, ag, v = opts[0]
        if len(opts) > 1 and opts[1][0] == ev:
            continue   # tie: stays ambiguous this round
        if len({E.Gaddr[o[2]] for o in opts}) == 1 and len(opts) > 1:
            pass       # same address via two library bodies of one name
        ctx = nall > 1
        if ctx and len(opts) > 1 and ev == 0:
            continue
        if ctx and ev == 0 and len(opts) == 1:
            pass       # every other candidate contradicted by known relocation targets or taken
        if not ctx and f.n < min_short and ev == 0:
            if not anyA: continue
            tier = 'exact-short'
        else:
            tier = ('exact' if anyA else 'masked') + ('+ctx' if ctx else '')
        decisions.append((tier, f, p, ev, ag, v, nall))
    # one owner per address unless identical bodies
    byaddr = collections.defaultdict(list)
    for d in decisions: byaddr[E.Gaddr[d[2]]].append(d)
    placed = 0
    for a, ds in byaddr.items():
        if len({tuple(d[1].toks.A) for d in ds}) > 1:
            ds.sort(key=lambda d: d[3], reverse=True)
            if ds[0][3] == ds[1][3]: continue
            ds = [d for d in ds if d[1].toks.A == ds[0][1].toks.A]
        for tier, f, p, ev, ag, v, nall in ds:
            base = {'exact': 1.0, 'masked': 0.9 if ag else 0.8, 'exact-short': 0.5}[tier.split('+')[0]]
            score = base if '+ctx' not in tier else round(base * (1 - 0.5 ** (ev + 1)), 3)
            notes = 'cands=%d ctx_agree=%d caller_votes=%.1f ntok=%d' % (nall, ag, v, f.n)
            place(S, E, f, p, tier, score, notes)
            placed += 1
    return placed

def padmatch(f, E, a):
    """Library body ends in nop alignment padding and the exe copy has the same padding."""
    if not f.pad: return False
    o = a - E.tva + f.size - f.pad
    return E.T[o:o + f.pad] == b'\x90' * f.pad

def body_ratio(f, E, a):
    p = E.pos_of.get(a)
    if p is None: return 0.0
    q = E.FI.end_for(p, f.n)
    return difflib.SequenceMatcher(None, f.toks.B, E.GB[p:q], autojunk=False).ratio()

def ref_named_bodies(S, E, funcs_by_name, cand):
    """Functions with a library body whose callers agree on an address where the body does not
    match (changed between builds): name them by reference."""
    new = 0
    for nm, fs in funcs_by_name.items():
        if nm in S.known or nm not in S.votes: continue
        byaddr = S.votes[nm]
        tot = {a: sum(srcs.values()) for a, srcs in byaddr.items()}
        if len(tot) != 1: continue
        (a, w), = tot.items()
        if w < 1.0 or not (E.tva <= a < E.tend) or a not in E.pos_of: continue
        if S.owner.get(a): continue
        if any(E.Gaddr[p] == a for g in fs for p, isA in cand[g.id]): continue   # resolve() decides
        f = fs[0]
        # one caller is not enough on its own: a template instantiation with identical code can
        # alias the caller; then the body at the address must still resemble the library body
        nsrc = len(byaddr[a])
        br = max(body_ratio(g, E, a) for g in fs)
        if nsrc < 2 and br < 0.5: continue
        S.known[nm] = a
        S.tier[nm] = ('ref', round(min(0.99, 1 - 0.5 ** (w * 2)) * (1.0 if br >= 0.5 else 0.9), 3),
                      'body differs (ratio %.2f); votes=%d from %s' % (br, nsrc, ','.join(sorted(byaddr[a])[:3])),
                      region_bytes(E, a), f.id)
        S.owner[a].add(nm)
        new += 1
    return new

def region_bytes(E, a):
    p = E.pos_of.get(a)
    if p is None: return 0
    k = bisect.bisect_right(E.region_pos, p)
    q = E.region_pos[k] if k < len(E.region_pos) else E.N
    end = E.Gaddr[q] if q < E.N else E.tend
    return end - a

# ----------------------------------------------------------------------------------------------
# Tier c

class FuzzyIndex:
    """stream 'B' = registers kept, values masked; 'C' = operand kinds only (survives register
    re-allocation).  Inverted index of n-grams over each exe region."""
    def __init__(self, E, stream='B', ng=4, maxlen=800, maxdf=60):
        self.E = E; self.stream = stream; self.ng = NG = ng
        inv = collections.defaultdict(list)
        GB = self.G = E.GB if stream == 'B' else E.GC
        rp = E.region_pos
        for s, p in E.pos_of.items():
            # index each region (start to next start), at most maxlen tokens
            k = bisect.bisect_right(rp, p)
            q = min(rp[k] if k < len(rp) else E.N, p + maxlen)
            seen = set()
            for j in range(p, q - NG + 1):
                g = hash(tuple(GB[j:j + NG]))
                if g not in seen:
                    seen.add(g); inv[g].append(p)
        self.inv = {g: ps for g, ps in inv.items() if len(ps) <= maxdf}
        self.rp = rp

    def end_for(self, p, n):
        """Exe extent nearest the library length: region starts and strong boundaries after p."""
        E = self.E
        k = bisect.bisect_right(E.strong, p)
        hard = min(E.strong[k] if k < len(E.strong) else E.N, p + 2 * n + 8)
        best = hard; bd = abs((hard - p) - n)
        k = bisect.bisect_right(self.rp, p)
        while k < len(self.rp) and self.rp[k] < hard:
            q = self.rp[k]; d = abs((q - p) - n)
            if d < bd: best, bd = q, d
            if q - p > 2 * n: break
            k += 1
        return best

def fuzzy_one(f, E, FI, S, excluded_addr=None, rmin=0.0, own_ok=None, topk=25, B=None):
    B = B if B is not None else getattr(f.toks, FI.stream)
    NG = FI.ng
    n = len(B)
    hits = collections.Counter()
    seen = set()
    for j in range(n - NG + 1):
        g = hash(tuple(B[j:j + NG]))
        if g in seen: continue
        seen.add(g)
        for p in FI.inv.get(g, ()): hits[p] += 1
    res = []
    for p, h in hits.most_common(topk * 3):
        a = E.Gaddr[p]
        if a == excluded_addr: continue
        own = S.owner.get(a)
        if own and not (own_ok and own <= own_ok): continue
        q = FI.end_for(p, n)
        m = q - p
        if abs(m - n) > max(4, 0.3 * n): continue
        sm = difflib.SequenceMatcher(None, B, FI.G[p:q], autojunk=False)
        if sm.real_quick_ratio() < rmin or sm.quick_ratio() < rmin:
            res.append((0.0, p, q, None)); continue
        r = sm.ratio()
        res.append((r, p, q, sm))
        if len(res) >= topk: break
    res.sort(key=lambda x: x[0], reverse=True)
    return res

def fuzzy_decide(f, E, S, res, rmin, margin, gate=True):
    if not res or res[0][0] < rmin: return None
    r, p, q, sm = res[0]
    r2 = res[1][0] if len(res) > 1 else 0.0
    if r - r2 < margin: return None
    align = []
    for i, j, sz in sm.get_matching_blocks():
        for k in range(sz): align.append((i + k, p + j + k))
    ag, dis = evidence(f, E, S, p, align)
    ecalls = sum(1 for x in range(p, q) for c, _ in E.Grefs.get(x, ()) if c == 'CALL')
    if abs(ecalls - f.toks.ncall) > max(1, f.toks.ncall // 5): return None
    if gate and dis: return None
    return dict(p=p, q=q, r=r, r2=r2, ag=ag, dis=dis, align=align, ecalls=ecalls)

def run_fuzzy(S, E, FI, funcs, rmin, margin, minn, exclude=None):
    decisions = []
    for f in funcs:
        if f.name in S.known or f.n < minn or (exclude and f.name in exclude): continue
        if f.name == f.lmname and ':.text' in f.name: continue
        res = fuzzy_one(f, E, FI, S, rmin=rmin)
        d = fuzzy_decide(f, E, S, res, rmin, margin)
        if d: decisions.append((f, d))
    # one name per address, one address per name
    byaddr = collections.defaultdict(list); byname = collections.defaultdict(list)
    for f, d in decisions: byaddr[d['p']].append((f, d))
    keep = []
    for p, fds in byaddr.items():
        fds.sort(key=lambda x: x[1]['r'], reverse=True)
        if len(fds) > 1 and fds[0][1]['r'] - fds[1][1]['r'] < margin and fds[0][0].toks.A != fds[1][0].toks.A:
            continue
        keep.append(fds[0])
    for f, d in keep: byname[f.name].append((f, d))
    placed = 0
    for nm, fds in byname.items():
        fds.sort(key=lambda x: x[1]['r'], reverse=True)
        if len(fds) > 1 and fds[0][1]['p'] != fds[1][1]['p']: continue
        f, d = fds[0]
        # capped at 0.8: on functions that really changed between builds tier c is right only
        # 60-80% of the time (calibration section 3, column real_prec)
        score = round(0.8 * d['r'] * min(1.0, (d['r'] - d['r2']) / 0.2 + 0.5) * (1.0 if d['ag'] else 0.85), 3)
        p = d['p']; q = d['q']
        size = (E.Gaddr[q] if q < E.N else E.tend) - E.Gaddr[p]
        place(S, E, f, p, 'fuzzy', score, 'ratio=%.3f runner_up=%.3f ctx_agree=%d calls=%d/%d ntok=%d/%d' % (
            d['r'], d['r2'], d['ag'], f.toks.ncall, d['ecalls'], f.n, q - p), d['align'], size)
        placed += 1
    return placed

# ----------------------------------------------------------------------------------------------

def seed_imports(S, E, funcs_refnames):
    n = 0
    for nm in funcs_refnames:
        if not nm.startswith('__imp_'): continue
        s = nm[6:]
        if s.startswith('_'): s = re.sub(r'@\d+$', '', s[1:])
        a = E.imports.get(s)
        if a is not None:
            S.known[nm] = a; S.tier[nm] = ('import', 1.0, 'IAT', 4, None); n += 1
    return n

def full_pipeline(E, funcs, data_sym, data_sec, cand, args, exclude=None, fuzzy=True, allow_masked=True, quiet=False):
    S = State()
    refnames = {r[0] for f in funcs for rr in f.toks.refs.values() for c, r in rr if r}
    ni = seed_imports(S, E, refnames)
    fbn = collections.defaultdict(list)
    for f in funcs: fbn[f.name].append(f)
    def loop(tag):
        rnd = 0
        while True:
            rnd += 1
            a = resolve(S, E, funcs, cand, allow_masked=allow_masked, exclude=exclude)
            b = expand_data(S, E, data_sym, data_sec)
            c = accept_votes(S, E, fbn)
            d = ref_named_bodies(S, E, fbn, cand) if not exclude else 0
            if not quiet: log('  %s round %d: placed %d, data-slot votes %d, ref names %d, ref bodies %d' % (tag, rnd, a, b, c, d))
            if a + c + d == 0 and b == 0: break
            if rnd > 30: break
    if not quiet: log('  imports seeded:', ni)
    loop('a/b')
    if fuzzy:
        FI = args.FI
        for it in range(2):
            k = run_fuzzy(S, E, FI, funcs, args.rmin, args.margin, args.minn, exclude=exclude)
            if not quiet: log('  fuzzy pass %d: placed %d' % (it + 1, k))
            loop('post-fuzzy')
            if k == 0: break
    return S

# ----------------------------------------------------------------------------------------------

def load_truth(path):
    gt = collections.defaultdict(set)
    for line in open(path):
        a, sz, nm = line.rstrip('\n').split('\t')
        gt[nm].add(int(a, 16))
    return gt

def calibrate(E, funcs, data_sym, data_sec, cand, args, Sfull, gt):
    out = []
    P = lambda *a: out.append(' '.join(str(x) for x in a))
    lm2f = collections.defaultdict(list)
    for f in funcs: lm2f[f.lmname].append(f)
    gtf = {nm: a for nm, a in gt.items() if nm in lm2f}
    P('# Calibration on libmatch exact matches (ground truth = %d names at %d addresses)' % (
        len(gtf), len({x for v in gtf.values() for x in v})))
    P('')
    def table(St, title):
        P(title)
        P('Columns: all = every ground-truth name; long = names whose library body has >= 6 instructions')
        P('(libmatch needs 16 bytes, so a 1-3 instruction function plus nop padding counts as "unique" there;')
        P('those short entries are weak ground truth: the exe has unpadded twins that vtables point to).')
        P('tier\tplaced\tcorrect\twrong\tprecision\trecall_all\t|\tlong_placed\tlong_wrong\tlong_precision\tlong_recall')
        bt = collections.defaultdict(lambda: [0, 0, 0, 0])
        nlong = 0; unpl = 0
        for lm, addrs in gtf.items():
            f = lm2f[lm][0]; nm = f.name; lg = f.n >= 6
            nlong += lg
            if nm not in St.known: unpl += 1; continue
            t = St.tier[nm][0]; ok = St.known[nm] in addrs
            bt[t][0 if ok else 1] += 1
            if lg: bt[t][2 if ok else 3] += 1
        tot = len(gtf)
        rows_ = sorted(bt.items()) + [('ALL', [sum(v[i] for v in bt.values()) for i in range(4)])]
        for t, (c, w, lc, lw) in rows_:
            P('%s\t%d\t%d\t%d\t%.4f\t%.4f\t|\t%d\t%d\t%.4f\t%.4f' % (t, c + w, c, w, c / max(1, c + w), c / tot,
              lc + lw, lw, lc / max(1, lc + lw), lc / max(1, nlong)))
        P('ground truth names: %d (long: %d); not placed: %d' % (tot, nlong, unpl))
        P('')
    table(Sfull, '## 1. Full pipeline, blind (it never reads libmatch output): how ground-truth names were placed')
    # 2. tier b only: drop A-equality so only masked structure + context can place a name
    cand_b = {fid: [(p, False) for p, isA in cs] for fid, cs in cand.items()}
    Sb = full_pipeline(E, funcs, data_sym, data_sec, cand_b, args, fuzzy=False, quiet=True)
    table(Sb, '## 2. Tier b alone (A-stream equality ignored, no fuzzy), blind')
    # 3. tier c: leave-one-out on ground truth (each name's own placement removed)
    rng = random.Random(1)
    sample = [lm for lm in sorted(gtf) if lm2f[lm][0].n >= args.minn]
    rng.shuffle(sample)
    sample = sample[:args.calib_n]
    FI = args.FI
    P('## 3. Tier c (fuzzy, stream %s, %d-grams) leave-one-out on %d ground-truth functions with >= %d tokens' % (
        FI.stream, FI.ng, len(sample), args.minn))
    P('Each function is matched with its own placement hidden, against the state built from all other names.')
    P('Variants: present = true copy available; absent = true address excluded (any answer is a false')
    P('positive unless it is an identical copy); drift = library tokens perturbed (8% of tokens replaced by')
    P('a random token from the same function, 4% deleted, 3% duplicated) to imitate the 2001->2003 build changes.')
    P('')
    rows = {}
    for variant in ('present', 'absent', 'drift'):
        rows[variant] = []
        for lm in sample:
            f = lm2f[lm][0]; addrs = gtf[lm]
            ta = Sfull.known.get(f.name)
            own_ok = set(Sfull.owner.get(ta, ())) if ta is not None else set()
            B = list(getattr(f.toks, FI.stream))
            if variant == 'drift':
                B2 = []
                for tok in B:
                    x = rng.random()
                    if x < 0.04: continue
                    if x < 0.12: tok = rng.choice(B)
                    B2.append(tok)
                    if rng.random() < 0.03: B2.append(tok)
                B = B2
            hid = {f.name} | own_ok
            known_bak = {nm: Sfull.known.pop(nm) for nm in hid if nm in Sfull.known}
            excl = next(iter(addrs)) if variant == 'absent' else None
            res = fuzzy_one(f, E, FI, Sfull, excluded_addr=excl, own_ok=own_ok, B=B)
            for nm, a in known_bak.items(): Sfull.known[nm] = a
            if variant == 'absent':
                res = [r for r in res if E.Gaddr[r[1]] not in addrs]
            if not res: rows[variant].append((0, 0, False, False, 0)); continue
            r, p, q, sm = res[0]
            r2 = res[1][0] if len(res) > 1 else 0
            identical = sm is not None and E.GA[p:p + f.n] == f.toks.A
            if sm is not None:
                align = [(i + k, p + j + k) for i, j, sz in sm.get_matching_blocks() for k in range(sz)]
                ag, dis = evidence(f, E, Sfull, p, align)
                ecalls = sum(1 for x in range(p, q) for c, _ in E.Grefs.get(x, ()) if c == 'CALL')
                callok = abs(ecalls - f.toks.ncall) <= max(1, f.toks.ncall // 5)
            else:
                dis = 1; callok = False
            correct = E.Gaddr[p] in addrs or (variant == 'absent' and identical)
            rows[variant].append((r, r - r2, correct, callok and not dis, dis))
    # realistic drift: functions whose address is fixed by >= 2 placed callers but whose body did not
    # match structurally (changed between the 2001 libs and the 2003 exe)
    rows['real'] = []
    for nm, (t, sc, notes, sz, fid) in Sfull.tier.items():
        mm = re.search(r'votes=(\d+)', notes)
        if t != 'ref' or fid is None or 'body differs' not in notes or int(mm.group(1)) < 2: continue
        f = funcs_by_id[fid]
        if f.n < args.minn: continue
        a = Sfull.known[nm]
        res = fuzzy_one(f, E, FI, Sfull, own_ok=set(Sfull.owner.get(a, ())))
        if not res: rows['real'].append((0, 0, False, False, 0)); continue
        r, p, q, sm = res[0]; r2 = res[1][0] if len(res) > 1 else 0
        ok = sm is not None
        if ok:
            align = [(i + k, p + j + k) for i, j, sz_ in sm.get_matching_blocks() for k in range(sz_)]
            hid = Sfull.known.pop(nm); ag, dis = evidence(f, E, Sfull, p, align); Sfull.known[nm] = hid
            ecalls = sum(1 for x in range(p, q) for c, _ in E.Grefs.get(x, ()) if c == 'CALL')
            ok = not dis and abs(ecalls - f.toks.ncall) <= max(1, f.toks.ncall // 5)
        rows['real'].append((r, r - r2, E.Gaddr[p] == a, ok, 0))
    P('real = %d functions (>= %d tokens) whose address >= 2 placed callers agree on but whose body changed;' % (len(rows['real']), args.minn))
    P('       recall there is the share a fuzzy match would recover, prec the share of answers that are right.')
    P('')
    P('rmin\tmargin\tpresent_recall\tpresent_prec\tabsent_false_pos_rate\tdrift_recall\tdrift_prec\treal_recall\treal_prec')
    for rmin in (0.5, 0.6, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95):
        for margin in (0.0, 0.03, 0.05, 0.1):
            stats = {}
            for v in rows:
                acc = [x for x in rows[v] if x[0] >= rmin and x[1] >= margin and x[3]]
                c = sum(1 for x in acc if x[2])
                stats[v] = (len(acc), c, len(rows[v]))
            pr = stats['present']; ab = stats['absent']; dr = stats['drift']; rl = stats['real']
            P('%.2f\t%.2f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f' % (
                rmin, margin, pr[1] / max(1, pr[2]), pr[1] / max(1, pr[0]),
                (ab[0] - ab[1]) / max(1, ab[2]), dr[1] / max(1, dr[2]), dr[1] / max(1, dr[0]),
                rl[1] / max(1, rl[2]), rl[1] / max(1, rl[0])))
    P('')
    P('Chosen: rmin=%.2f margin=%.2f minn=%d' % (args.rmin, args.margin, args.minn))
    P('')
    # 4. independent check of fuzzy placements on the unknowns (not in libmatch ground truth)
    P('## 4. Fuzzy placements outside the ground truth, re-checked against the final name set')
    P('A fuzzy match is accepted only if no already-known callee/global contradicts it at that time.')
    P('Re-checking against the final, larger name set (exact/ctx/ref names found later) gives an')
    P('independent error estimate: a wrong match would point its calls at addresses carrying other names.')
    lmset = set(gtf)
    n = chk = bad = 0
    for nm, (p, align) in Sfull.align.items():
        f = funcs_by_id[Sfull.tier[nm][4]]
        if f.lmname in lmset: continue
        n += 1
        hid = Sfull.known.pop(nm)
        ag, dis = evidence(f, E, Sfull, p, align)
        Sfull.known[nm] = hid
        if ag + dis: chk += 1
        if dis: bad += 1
    P('fuzzy placements outside ground truth: %d; with >=1 checkable reference: %d; contradicted: %d' % (n, chk, bad))
    P('')
    return out

def fuzzy_consistency(S, E, funcs_by_name):
    """For fuzzy placements: how many have a callee/global whose address is independently known,
    and how many of those agree (gate removed: we recount disagreements without the gate)."""
    n = chk = good = 0
    for nm, (tier, score, notes, size, fid) in S.tier.items():
        if tier != 'fuzzy': continue
        n += 1
        mm = re.search(r'ctx_agree=(\d+)', notes)
        if mm and int(mm.group(1)) > 0: chk += 1
    return n, chk

# ----------------------------------------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--exe', required=True)
    ap.add_argument('--out', required=True)
    ap.add_argument('--data-out')
    ap.add_argument('--truth')
    ap.add_argument('--calib')
    ap.add_argument('--calib-n', type=int, default=1500)
    ap.add_argument('--ghidra')
    ap.add_argument('--rmin', type=float, default=0.85)   # see calibration section 3
    ap.add_argument('--margin', type=float, default=0.05)
    ap.add_argument('--minn', type=int, default=12)
    ap.add_argument('--stream', default='C', choices=('B', 'C'))
    ap.add_argument('--ng', type=int, default=6)
    ap.add_argument('libs', nargs='+')
    args = ap.parse_args()
    t0 = time.time()
    E = load_exe(args.exe, args.ghidra)
    log('exe starts:', E.stats)
    tokenize_exe(E)
    log('exe tokens: %d (%.0fs)' % (E.N, time.time() - t0))
    funcs, data_sym, data_sec = load_libs(args.libs, E.lo, E.hi)
    global funcs_by_id
    funcs_by_id = {f.id: f for f in funcs}
    log('lib functions (deduplicated): %d, data sections with relocs: %d (%.0fs)' % (
        len(funcs), len(data_sec), time.time() - t0))
    cand = structural_candidates(funcs, E)
    log('structural candidates built (%.0fs)' % (time.time() - t0))
    args.FI = E.FI = FuzzyIndex(E, args.stream, args.ng)
    log('fuzzy index built (%.0fs)' % (time.time() - t0))
    S = full_pipeline(E, funcs, data_sym, data_sec, cand, args)
    log('pipeline done (%.0fs)' % (time.time() - t0))
    gt = load_truth(args.truth) if args.truth else {}
    lmnames = {}
    for f in funcs: lmnames.setdefault(f.name, f.lmname)
    gt_names = set(gt)
    # ---- output
    fbn = collections.defaultdict(list)
    for f in funcs: fbn[f.name].append(f)
    rows = []
    for nm, (tier, score, notes, size, fid) in S.tier.items():
        a = S.known[nm]
        if tier in ('import',): continue
        if not (E.tva <= a < E.tend): continue
        if nm.rsplit('|', 1)[-1].startswith('$L'): continue   # compiler-local labels (EH funclets)
        lib = ','.join(funcs_by_id[fid].libs) if fid is not None else (','.join(fbn[nm][0].libs) if nm in fbn else '-')
        n2 = notes
        if lmnames.get(nm, nm) in gt_names:
            n2 += '; libmatch=' + ('same' if a in gt[lmnames.get(nm, nm)] else 'DIFFERENT')
        if fid is None and nm not in fbn: n2 += '; no library body (callee/vtable name only)'
        if size == 0: size = region_bytes(E, a)
        rows.append((a, size, nm, lib, tier, score, n2))
    # unresolved structural groups
    placed_names = set(S.known)
    for nm, fs in fbn.items():
        if nm in placed_names: continue
        cs = sorted({E.Gaddr[p] for f in fs for p, isA in cand[f.id]})
        if len(cs) < 2: continue
        lib = ','.join(fs[0].libs)
        if len(cs) <= 8:
            for a in cs:
                rows.append((a, fs[0].size, nm, lib, 'multi', round(1 / len(cs), 3),
                             'ambiguous group of %d identical-structure addresses: %s' % (
                                 len(cs), ' '.join('%08x' % x for x in cs))))
        else:
            rows.append((None, fs[0].size, nm, lib, 'multi', round(1 / len(cs), 3),
                         'ambiguous: %d identical-structure addresses, not listed' % len(cs)))
    rows.sort(key=lambda r: (r[0] is None, r[0] or 0, r[2]))
    with open(args.out, 'w') as fo:
        fo.write('address\tsize\tmangled_name\tlibrary\ttier\tscore\tnotes\n')
        for a, sz, nm, lib, tier, score, notes in rows:
            fo.write('%s\t%d\t%s\t%s\t%s\t%s\t%s\n' % ('%08x' % a if a is not None else '-', sz, nm, lib, tier, score, notes))
    if args.data_out:
        with open(args.data_out, 'w') as fo:
            fo.write('address\tname\ttier\tscore\tnotes\n')
            for nm, (tier, score, notes, size, fid) in sorted(S.tier.items(), key=lambda x: S.known[x[0]]):
                a = S.known[nm]
                if E.tva <= a < E.tend: continue
                fo.write('%08x\t%s\t%s\t%s\t%s\n' % (a, nm, tier, score, notes))
    # ---- summary
    summary = []
    Q = lambda *a: summary.append(' '.join(str(x) for x in a))
    Q('# %s' % os.path.basename(args.exe))
    Q('exe .text bytes %d; candidate starts %s' % (len(E.T), E.stats))
    Q('library functions (deduplicated by name+bytes): %d' % len(funcs))
    tiers = collections.defaultdict(lambda: [0, 0])
    cov = {}
    allcov = bytearray(len(E.T))
    for a, sz, nm, lib, tier, score, notes in rows:
        if a is None or tier == 'multi': continue
        new = lmnames.get(nm, nm) not in gt_names
        tiers[tier][0] += 1; tiers[tier][1] += new
        c = cov.setdefault(tier, bytearray(len(E.T)))
        o = a - E.tva; k = max(0, min(sz, len(E.T) - o))
        c[o:o + k] = b'\1' * k; allcov[o:o + k] = b'\1' * k
    Q('tier\tnames\tnew_vs_libmatch\ttext_bytes')
    for t in ('exact', 'exact+ctx', 'masked', 'masked+ctx', 'exact-short', 'fuzzy', 'ref'):
        if t in tiers:
            Q('%s\t%d\t%d\t%d' % (t, tiers[t][0], tiers[t][1], sum(cov[t])))
    Q('ALL placed\t%d\t%d\t%d (%.1f%% of .text)' % (sum(v[0] for v in tiers.values()), sum(v[1] for v in tiers.values()),
                                             sum(allcov), 100 * sum(allcov) / len(E.T)))
    nmulti = sum(1 for r in rows if r[4] == 'multi')
    Q('unresolved multi rows: %d (names: %d)' % (nmulti, len({r[2] for r in rows if r[4] == 'multi'})))
    if gt:
        gtcov = bytearray(len(E.T))
        for line in open(args.truth):
            a, sz, nm = line.rstrip('\n').split('\t'); o = int(a, 16) - E.tva; sz = int(sz)
            gtcov[o:o + sz] = b'\1' * sz
        Q('libmatch baseline: %d names at %d addresses, %d bytes (%.1f%% of .text)' % (
            len(gt), len({x for v in gt.values() for x in v}), sum(gtcov), 100 * sum(gtcov) / len(E.T)))
    for line in summary: log(line)
    if args.calib:
        lines = summary + ['']
        if gt:
            lines += calibrate(E, funcs, data_sym, data_sec, cand, args, S, gt)
        open(args.calib, 'w').write('\n'.join(lines) + '\n')
    log('done (%.0fs)' % (time.time() - t0))

funcs_by_id = {}
if __name__ == '__main__':
    main()
