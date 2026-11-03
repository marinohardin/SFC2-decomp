#!/bin/sh
# P1-8 proof: instantiate the SDK's Single-mission template as <name> (default OracleTest), build it
# into a .scr with build_scr.sh, check it against the shipped scripts and check the dmp3D shim.
# Scratch goes to $SFCOP_WORK/toolchain (default ../work/toolchain next to the repo).
set -eu
here=$(cd "$(dirname "$0")" && pwd)
repo=$(cd "$here/../.." && pwd)
work=${SFCOP_WORK:-$(dirname "$repo")/work}/toolchain
name=${1:-OracleTest}
src=$work/scr-src/$name
test ! -e "$src" || rm -rf "${src:?}"
python3 -I "$here/new_from_template.py" Single "$name" "$src"
"$repo/toolchain/build_scr.sh" "$src" "$work/scr-out/$name.scr" --jobs "${JOBS:-3}"
python3 -I "$here/check_scr.py" "$work/scr-out/$name.scr"
python3 -I "$here/verify_shim.py" "$work/scr/$name/obj/_dmp3d_shim.obj"
