#!/bin/sh
# One-time setup of a clone for matching work. Checks the prerequisites, links your own Orion
# Pirates install (patched to 2.5.5.2) where the tools read it, checks its files against the known
# hashes, builds the VC6 compiler image and fetches objdiff.
#
# Usage: sh scripts/setup.sh --install "/path/to/Starfleet Command Orion Pirates"
#        sh scripts/setup.sh            (the install is already linked)
#
# Generated data and downloaded tools go in work/ and tools/ beside the clone, so clone into a
# folder of its own (for example sfc2/SFC2-decomp). SFCOP_WORK and SFCOP_TOOLS move them.
set -eu
cd "$(dirname "$0")/.."
repo=$(pwd)
link="$repo/reference/installed game/Starfleet Command Orion Pirates"

install=
while [ $# -gt 0 ]; do
    case $1 in
    --install) install=$2; shift 2 ;;
    -h | --help) sed -n '2,11p' "$0"; exit 0 ;;
    *) echo "setup: unknown argument $1" >&2; exit 2 ;;
    esac
done

fail() { echo "setup: $*" >&2; exit 1; }

command -v python3 >/dev/null || fail "python3 is required"
python3 -c 'import sys; sys.exit(sys.version_info < (3, 9))' || fail "python3 3.9 or later is required"
python3 -c 'import pytest' 2>/dev/null || echo "setup: note: pytest is not installed (python3 -m pip install pytest)"
command -v docker >/dev/null || fail "Docker is required (the VC6 compiler runs in a container)"
docker info >/dev/null 2>&1 || fail "Docker is installed but not running"

if [ -n "$install" ]; then
    [ -f "$install/StarFleetOP.exe" ] || fail "no StarFleetOP.exe in $install"
    if [ -e "$link" ] || [ -L "$link" ]; then
        fail "an install is already linked at reference/; remove that link first"
    fi
    mkdir -p "$(dirname "$link")"
    ln -s "$install" "$link"
    echo "setup: linked your install"
fi
[ -f "$link/StarFleetOP.exe" ] || fail "no install linked: run with --install <your Orion Pirates folder>"

echo "setup: checking the install against analysis/results/retail_hashes.sha256"
(cd reference && grep -E "StarFleetOP\.exe|OP API - R2\.1/" "$repo/analysis/results/retail_hashes.sha256" \
    | shasum -a 256 -c --quiet -) || fail "files differ from 2.5.5.2: reinstall and patch to 2.5.5.2"

if docker image inspect "${SFCOP_MSVC_IMAGE:-sfcop-msvc6:latest}" >/dev/null 2>&1; then
    echo "setup: the VC6 image is present"
else
    echo "setup: building the VC6 image (downloads the compiler; a few minutes)"
    sh toolchain/build-image.sh
fi

echo "setup: calibrating (fetches objdiff, then compiles the SDK sources and compares them)"
sh toolchain/calibrate.sh
echo "setup: done. Next: sh scripts/check.sh"
