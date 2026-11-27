"""Tests for the analysis tooling that do not need the retail files.

They exercise the COFF archive parser in analysis/scripts/libmatch.py on a synthetic archive and
check the shape of the retail hash list. The retail files themselves are never read here.
"""
import importlib.util
import pathlib
import struct

ROOT = pathlib.Path(__file__).resolve().parents[1]


def _load_libmatch():
    # libmatch.py runs its main logic at import time when given argv; load it as a module with a
    # fake argv so only its functions are defined.
    import sys
    saved = sys.argv
    sys.argv = ["libmatch.py", str(ROOT / "tests" / "__no_exe__")]
    try:
        spec = importlib.util.spec_from_file_location("libmatch", ROOT / "analysis/scripts/libmatch.py")
        mod = importlib.util.module_from_spec(spec)
        src = (ROOT / "analysis/scripts/libmatch.py").read_text()
        # Execute only the function definitions: everything up to the first top-level loop.
        head = src.split("\ngrand = ", 1)[0]
        head = head.replace("exe_path = sys.argv[1]; libs = sys.argv[2:]", "exe_path = None; libs = []")
        head = head.replace("pe = pefile.PE(exe_path, fast_load=True)", "pe = None")
        head = head.replace("text = [s for s in pe.sections if s.Name.startswith(b'.text')][0]", "text = None")
        head = head.replace("TEXT = text.get_data(); TBASE = pe.OPTIONAL_HEADER.ImageBase + text.VirtualAddress", "TEXT = b''; TBASE = 0")
        head = head.replace("import pefile", "pefile = None")
        exec(compile(head, "libmatch_head", "exec"), mod.__dict__)
        return mod
    finally:
        sys.argv = saved


def _member(name: bytes, body: bytes) -> bytes:
    hdr = name.ljust(16) + b"0".ljust(12) + b"0".ljust(6) + b"0".ljust(6) + b"100644".ljust(8)
    hdr += str(len(body)).encode().ljust(10) + b"`\n"
    return hdr + body + (b"\n" if len(body) % 2 else b"")


def _coff_object() -> bytes:
    """One COFF object with a 16-byte .text section, one relocation and one external symbol."""
    code = bytes([0x55, 0x8B, 0xEC, 0xE8, 0, 0, 0, 0, 0x5D, 0xC3, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC])
    nsec, nsym = 1, 1
    sec_off = 20
    raw_off = sec_off + 40
    rel_off = raw_off + len(code)
    sym_off = rel_off + 10
    header = struct.pack("<HHIIIHH", 0x14C, nsec, 0, sym_off, nsym, 0, 0)
    section = struct.pack("<8sIIIIIIHHI", b".text", 0, 0, len(code), raw_off, rel_off, 0, 1, 0, 0x60500020)
    reloc = struct.pack("<IIH", 4, 0, 0x14)
    symbol = struct.pack("<8sIhHBB", b"_func001", 0, 1, 0x20, 2, 0)
    strtab = struct.pack("<I", 4)
    return header + section + code + reloc + symbol + strtab


def test_parse_archive_and_coff(tmp_path):
    lm = _load_libmatch()
    obj = _coff_object()
    archive = b"!<arch>\n" + _member(b"/", b"\0\0\0\0") + _member(b"a.obj/", obj)
    path = tmp_path / "t.lib"
    path.write_bytes(archive)
    members = lm.parse_archive(str(path))
    assert [m[0] for m in members] == ["a.obj"]
    secs, syms = lm.parse_coff(members[0][1])
    assert secs[0]["name"] == ".text" and secs[0]["size"] == 16 and secs[0]["nrel"] == 1
    assert syms[0][0] == "_func001" and syms[0][2] == 1 and syms[0][3] == 2


def test_retail_hash_list_shape():
    lines = [l for l in (ROOT / "analysis/results/retail_hashes.sha256").read_text().splitlines() if l.strip()]
    assert len(lines) >= 5
    for line in lines:
        digest, _, name = line.partition("  ")
        assert len(digest) == 64 and all(c in "0123456789abcdef" for c in digest), line
        assert name.endswith((".exe", ".lib", ".dll")), line
    assert any(name.endswith("StarFleetOP.exe") for name in (l.partition("  ")[2] for l in lines))


def test_retail_hash_list_pins_the_whole_3d_library():
    # The 4,229,212-byte library with all 56 objects, not the 848,082-byte copy cut off in its tenth.
    lines = (ROOT / "analysis/results/retail_hashes.sha256").read_text().splitlines()
    pinned = [l.partition("  ")[0] for l in lines if l.endswith("/dmp3D_MSVCR.lib")]
    assert pinned == ["b89b29d36ffcc04a3adc8893f5c5b5e8a21366f43128850d589907e309766653"]


def test_setup_checks_only_the_linked_install():
    # A contributor links the game install only: setup must not ask for the server kit's files.
    import re
    script = (ROOT / "scripts/setup.sh").read_text()
    pattern = re.search(r'grep -E "([^"]+)" "\$repo/analysis/results/retail_hashes\.sha256"', script).group(1)
    lines = (ROOT / "analysis/results/retail_hashes.sha256").read_text().splitlines()
    names = [l.partition("  ")[2] for l in lines if re.search(pattern, l)]
    assert names and all(n.startswith("installed game/Starfleet Command Orion Pirates/") for n in names)
    assert "installed game/Starfleet Command Orion Pirates/StarFleetOP.exe" in names
    assert sum(n.endswith(".lib") for n in names) == 6
