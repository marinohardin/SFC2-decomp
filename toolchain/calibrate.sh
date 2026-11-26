#!/bin/sh
# One-shot calibration: build the sfcop-msvc6 image if it is missing, fetch objdiff-cli into
# tools/ if it is missing, then compile the OP API SDK sources and diff them against
# ScriptInterfaceR.lib. Output: toolchain/calibration/report.md (+ results/*.json).
# Extra arguments go to calibrate.py (e.g. --variants dsp --jobs 2).
set -e
here=$(cd "$(dirname "$0")" && pwd)
sfcop=$(cd "$here/../.." && pwd)
tools=${SFCOP_TOOLS:-$sfcop/tools}

OBJDIFF_VERSION=v3.8.3
case "$(uname -s)-$(uname -m)" in
    Darwin-arm64) asset=objdiff-cli-macos-arm64
        sum=27dbd79196394c246dde77ef0a6ac855ff6c568069171dd7db0093f39c03ee8d ;;
    # Not yet run end to end; the checksum is the release's published digest. The harness finds
    # it through OBJDIFF (CONTRIBUTING.md, "What you need").
    Linux-x86_64) asset=objdiff-cli-linux-x86_64
        sum=e823ae932a56c68dcd86b68d12f2268c953e0c027402162c8a6dfa6a00d65008 ;;
    *) echo "calibrate.sh: add an objdiff-cli asset and checksum for $(uname -s)-$(uname -m)" >&2
       exit 1 ;;
esac
objdiff=$tools/objdiff/$asset
if [ ! -x "$objdiff" ]; then
    mkdir -p "$tools/objdiff"
    curl -fsSL -o "$objdiff.part" \
        "https://github.com/encounter/objdiff/releases/download/$OBJDIFF_VERSION/$asset"
    echo "$sum  $objdiff.part" | shasum -a 256 -c - >/dev/null
    chmod +x "$objdiff.part" && mv "$objdiff.part" "$objdiff"
fi

if ! docker image inspect "${SFCOP_MSVC_IMAGE:-sfcop-msvc6:latest}" >/dev/null 2>&1; then
    "$here/build-image.sh"
fi

OBJDIFF=$objdiff exec python3 -I "$here/calibration/calibrate.py" "$@"
