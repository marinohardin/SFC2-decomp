import struct, sys
sys.path.insert(0, '/Volumes/MacPersonal/gitdir-portable/SFCOP/SFC2-decomp/.claude/worktrees/w3-A2')
from harness.lib import paths
d = open(paths.EXE, 'rb').read()
pe = struct.unpack_from('<I', d, 0x3c)[0]
n = struct.unpack_from('<H', d, pe + 6)[0]
opt = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]
secs = [struct.unpack_from('<8sIIII', d, pe + 24 + opt + 40 * i) for i in range(n)]


def rd(va, ln):
    for s in secs:
        if base + s[2] <= va < base + s[2] + max(s[1], s[3]):
            o = va - base - s[2] + s[4]
            return d[o:o + ln]


if sys.argv[1] == 'jt':
    a = int(sys.argv[2], 16)
    cnt = int(sys.argv[3])
    t = rd(a, cnt * 4)
    for i in range(cnt):
        print(i, hex(struct.unpack_from('<I', t, 4 * i)[0] - 0x668970))
elif sys.argv[1] == 'bytes':
    a = int(sys.argv[2], 16)
    print(rd(a, int(sys.argv[3])).hex())
else:
    for a in sys.argv[2:]:
        print(a, rd(int(a, 16), 200).split(b'\0')[0])
