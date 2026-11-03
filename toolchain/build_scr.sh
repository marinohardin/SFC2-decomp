#!/bin/sh
# Build a mission script DLL: build_scr.sh <srcdir> <out.scr> [--jobs N]
# See scr/build_scr.py. MSVC_LIB_SET=initial (VC6 RTM CRT, default) or sp6 picks the VC6 Lib/ set.
here=$(cd "$(dirname "$0")" && pwd)
exec python3 -I "$here/scr/build_scr.py" "$@"
