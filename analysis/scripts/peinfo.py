import sys, pefile, datetime
p = pefile.PE(sys.argv[1], fast_load=False)
print("== ", sys.argv[1])
print("linker", p.OPTIONAL_HEADER.MajorLinkerVersion, p.OPTIONAL_HEADER.MinorLinkerVersion)
print("timestamp", datetime.datetime.utcfromtimestamp(p.FILE_HEADER.TimeDateStamp))
print("entry", hex(p.OPTIONAL_HEADER.AddressOfEntryPoint), "imagebase", hex(p.OPTIONAL_HEADER.ImageBase))
print("sections:")
for s in p.sections:
    print("  ", s.Name.decode(errors='ignore').strip('\0'), hex(s.VirtualAddress), "vsize", hex(s.Misc_VirtualSize), "raw", hex(s.SizeOfRawData), "chars", hex(s.Characteristics))
rich = p.parse_rich_header()
if rich:
    print("rich header entries (prodid, build, count):")
    vals = rich['values']
    for i in range(0, len(vals), 2):
        v, c = vals[i], vals[i+1]
        print("   prodid", hex(v >> 16), "build", v & 0xffff, "count", c)
else:
    print("no rich header")
print("imports:")
for e in getattr(p, 'DIRECTORY_ENTRY_IMPORT', []):
    names = [i.name.decode() if i.name else f"ord{i.ordinal}" for i in e.imports]
    print("  ", e.dll.decode(), len(names))
    if len(sys.argv) > 2: print("     ", ", ".join(names))
if hasattr(p, 'DIRECTORY_ENTRY_DEBUG'):
    for d in p.DIRECTORY_ENTRY_DEBUG:
        print("debug type", d.struct.Type, getattr(d.entry, 'PdbFileName', None))
if hasattr(p, 'DIRECTORY_ENTRY_EXPORT'):
    print("exports", len(p.DIRECTORY_ENTRY_EXPORT.symbols))
