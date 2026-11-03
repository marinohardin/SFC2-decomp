import sys, struct, collections
exe_path = sys.argv[1]; libs = sys.argv[2:]
import pefile
pe = pefile.PE(exe_path, fast_load=True)
text = [s for s in pe.sections if s.Name.startswith(b'.text')][0]
TEXT = text.get_data(); TBASE = pe.OPTIONAL_HEADER.ImageBase + text.VirtualAddress

def parse_archive(path):
    data = open(path,'rb').read()
    assert data[:8] == b'!<arch>\n', path
    off = 8; longnames = b''; members = []
    while off + 60 <= len(data):
        hdr = data[off:off+60]
        name = hdr[:16].decode('latin1').rstrip()
        try: size = int(hdr[48:58].decode().strip())
        except ValueError: break
        body = data[off+60:off+60+size]
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
    if machine != 0x14c or symoff + nsym*18 > len(obj): return None
    strtab_off = symoff + nsym*18
    syms = []
    i = 0
    while i < nsym:
        raw = obj[symoff+i*18: symoff+(i+1)*18]
        if len(raw) < 18: break
        nm, val, secnum, typ, sc, naux = struct.unpack('<8sIhHBB', raw)
        if nm[:4] == b'\0\0\0\0':
            so = struct.unpack('<I', nm[4:])[0]
            e = obj.find(b'\0', strtab_off+so); name = obj[strtab_off+so:e].decode('latin1')
        else: name = nm.rstrip(b'\0').decode('latin1')
        syms.append((name, val, secnum, sc))
        for _ in range(naux): syms.append(None)
        i += 1 + naux
    secs = []
    for s in range(nsec):
        h = obj[20+s*40: 60+s*40]
        if len(h) < 40: break
        name, vsz, va, rawsz, rawptr, relptr, lnptr, nrel, nln, ch = struct.unpack('<8sIIIIIIHHI', h)
        secs.append(dict(idx=s+1, name=name.rstrip(b'\0').decode('latin1'), size=rawsz, ptr=rawptr, relptr=relptr, nrel=nrel, ch=ch))
    return secs, syms

def find_masked(pattern, mask):
    # mask: bytearray 1=must match. anchor = longest run of 1s
    best=(0,0); run=0; start=0
    for i,m in enumerate(mask):
        if m:
            if run==0: start=i
            run+=1
            if run>best[0]: best=(run,start)
        else: run=0
    L, a = best
    if L < 10: return None
    anchor = pattern[a:a+L]; pos = TEXT.find(anchor)
    hits=[]
    while pos != -1 and len(hits) < 5:
        st = pos - a
        if st >= 0 and st+len(pattern) <= len(TEXT):
            ok = all((not mask[i]) or TEXT[st+i]==pattern[i] for i in range(len(pattern)))
            if ok: hits.append(st)
        pos = TEXT.find(anchor, pos+1)
    return hits

grand = collections.Counter(); matched_names = []
covered = bytearray(len(TEXT))
for lib in libs:
    try: members = parse_archive(lib)
    except Exception as e: print(lib, "ARCHIVE ERROR", e); continue
    c = collections.Counter(); objs=set()
    for name, body in members:
        r = parse_coff(body)
        if not r: continue
        secs, syms = r; objs.add(name)
        for sec in secs:
            if not (sec['ch'] & 0x20) or sec['size'] < 16 or sec['ptr']+sec['size'] > len(body): continue
            code = bytearray(body[sec['ptr']:sec['ptr']+sec['size']]); mask = bytearray([1])*len(code)
            for k in range(sec['nrel']):
                ro = sec['relptr']+k*10
                if ro+10 > len(body): break
                va, si, ty = struct.unpack_from('<IIH', body, ro)
                for j in range(va, min(va+4, len(code))): mask[j]=0
            fn = [s[0] for s in syms if s and s[2]==sec['idx'] and s[1]==0 and s[3]==2]
            fname = fn[0] if fn else f"{name}:{sec['name']}#{sec['idx']}"
            c['funcs'] += 1; c['bytes'] += len(code)
            hits = find_masked(bytes(code), mask)
            if hits is None: c['too_small_anchor'] += 1
            elif len(hits)==1:
                c['unique'] += 1; c['unique_bytes'] += len(code); matched_names.append((fname, TBASE+hits[0], len(code)))
                covered[hits[0]:hits[0]+len(code)] = b'\1'*len(code)
            elif len(hits)>1: c['multi'] += 1
            else: c['nomatch'] += 1
    print(f"{lib.split('/')[-1]}: objects={len(objs)} code_sections={c['funcs']} unique_match={c['unique']} multi={c['multi']} nomatch={c['nomatch']} small={c['too_small_anchor']} bytes={c['bytes']} unique_bytes={c['unique_bytes']}")
    grand.update(c)
print("TOTAL", dict(grand))
print("exe .text bytes", len(TEXT), "covered by unique lib matches", sum(covered), f"({100*sum(covered)/len(TEXT):.1f}%)")
with open('lib_matches.tsv','w') as f:
    for n,a,l in sorted(matched_names, key=lambda x:x[1]): f.write(f"{a:08x}\t{l}\t{n}\n")
print("sample matches:")
for n,a,l in sorted(matched_names, key=lambda x:x[1])[::max(1,len(matched_names)//25)][:30]: print(f"  {a:08x} {l:5d} {n[:110]}")
