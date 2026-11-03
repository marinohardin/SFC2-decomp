import sys, struct, pefile
pe = pefile.PE(sys.argv[1], fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
text = [s for s in pe.sections if s.Name.startswith(b'.text')][0]
T = text.get_data(); tva = base + text.VirtualAddress; tend = tva + text.Misc_VirtualSize
calls=set(); 
for i in range(len(T)-5):
    if T[i]==0xE8:
        rel = struct.unpack_from('<i', T, i+1)[0]; tgt = tva+i+5+rel
        if tva <= tgt < tend: calls.add(tgt)
# pointers from .rdata/.data into .text (vtables, function tables)
ptrs=set()
for s in pe.sections:
    if s.Name.startswith(b'.rdata') or s.Name.startswith(b'.data'):
        d = s.get_data()
        for i in range(0, len(d)-3, 4):
            v = struct.unpack_from('<I', d, i)[0]
            if tva <= v < tend: ptrs.add(v)
# prologue heuristic
prol = sum(1 for i in range(len(T)-3) if T[i]==0x55 and T[i+1]==0x8B and T[i+2]==0xEC)
# int3 / nop padding gaps -> function boundaries (MSVC pads with CC)
pads = 0; i=0
while i < len(T)-1:
    if T[i]==0xCC and T[i+1]==0xCC:
        j=i
        while j<len(T) and T[j]==0xCC: j+=1
        if j-i>=1: pads+=1
        i=j
    else: i+=1
import pickle
allf = calls|ptrs
print(f"text size {len(T)}  call targets {len(calls)}  data->text ptrs {len(ptrs)}  union {len(allf)}  push ebp;mov ebp,esp {prol}  CC pad runs {pads}")
print(f"avg bytes/function (union) {len(T)/len(allf):.0f}")
