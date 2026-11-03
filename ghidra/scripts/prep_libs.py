#!/usr/bin/env python3
"""Copy the SDK .lib archives into the work dir for import, dropping a truncated final member.

dmp3D_MSVCR.lib ships damaged: its last member's header claims more bytes than the file has, which
makes Ghidra's COFF archive file system reject the whole archive. The copy keeps every complete
member and cuts the file at the end of the last one. The other archives are copied unchanged.
Usage: prep_libs.py <out dir> <lib>...
Prints one line per archive: name, members kept, members dropped, bytes kept.
"""
import os
import sys


def complete_prefix(data):
    off, kept, dropped = 8, 0, 0
    while off + 60 <= len(data):
        header = data[off:off + 60]
        try:
            size = int(header[48:58].decode("ascii").strip())
        except ValueError:
            break
        end = off + 60 + size
        if end > len(data):
            dropped += 1
            break
        name = header[:16].decode("latin1").strip()
        if name not in ("/", "//"):
            kept += 1
        off = end + (size & 1)
    return min(off, len(data)), kept, dropped


def main():
    out_dir, libs = sys.argv[1], sys.argv[2:]
    os.makedirs(out_dir, exist_ok=True)
    for lib in libs:
        with open(lib, "rb") as f:
            data = f.read()
        if data[:8] != b"!<arch>\n":
            sys.exit(f"{lib}: not a COFF archive")
        end, kept, dropped = complete_prefix(data)
        with open(os.path.join(out_dir, os.path.basename(lib)), "wb") as f:
            f.write(data[:end])
        print(f"{os.path.basename(lib)}\tmembers={kept}\tdropped={dropped}\tbytes={end}/{len(data)}")


if __name__ == "__main__":
    main()
