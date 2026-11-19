#!/bin/sh
# Fetch the native compiler for macOS: the wibo 1.2.0 release binary and the decomp.me VC6
# tarballs the Docker image also uses, checksum-pinned, into tools/ beside the clone:
#   tools/wibo/1.2.0/wibo-macos
#   tools/msvc6/msvc6.4/   (VC6 SP4)     tools/msvc6/msvc6.5/   (SP5)
# toolchain/bin/cl-native runs CL.EXE with them, with no Docker and no QEMU. Idempotent: what is
# already there is kept. SFCOP_TOOLS moves tools/.
set -e
here=$(cd "$(dirname "$0")" && pwd)
top=$(git -C "$here" rev-parse --show-superproject-working-tree 2>/dev/null)
[ -n "$top" ] || top=$here
# The SFCOP root holds the main checkout (of the containing repository, when this is the
# port's decomp/ submodule), work/ and tools/; a worktree finds it through git's common dir.
common=$(git -C "$top" rev-parse --path-format=absolute --git-common-dir 2>/dev/null) \
    && root=${SFCOP_ROOT:-$(cd "$common/../.." && pwd)} \
    || root=${SFCOP_ROOT:-$(cd "$here/../.." && pwd)}
tools=${SFCOP_TOOLS:-$root/tools}

case "$(uname -s)" in
    Darwin) ;;
    *) echo "fetch-native.sh: only macOS has a pinned wibo build here; use the Docker image" >&2
       exit 1 ;;
esac

WIBO_VERSION=1.2.0
WIBO_SUM=2b3000ef6a7a490c24ccd71967735ae0005e218922e51806cca1b8d77fd3cf7c
wibo=$tools/wibo/$WIBO_VERSION/wibo-macos
if [ ! -x "$wibo" ]; then
    mkdir -p "$(dirname "$wibo")"
    curl -fsSL -o "$wibo.part" \
        "https://github.com/decompals/wibo/releases/download/$WIBO_VERSION/wibo-macos"
    echo "$WIBO_SUM  $wibo.part" | shasum -a 256 -c - >/dev/null
    chmod +x "$wibo.part" && mv "$wibo.part" "$wibo"
    echo "fetch-native.sh: wibo $WIBO_VERSION"
fi

# The same tarballs and checksums as toolchain/Dockerfile.
fetch_msvc() {
    v=$1 sum=$2
    dir=$tools/msvc6/msvc$v
    [ -f "$dir/Bin/CL.EXE" ] && return 0
    mkdir -p "$tools/msvc6"
    tgz=$tools/msvc6/msvc$v.tar.gz
    curl -fsSL -o "$tgz.part" \
        "https://github.com/OmniBlade/decomp.me/releases/download/msvcwin9x/msvc$v.tar.gz"
    echo "$sum  $tgz.part" | shasum -a 256 -c - >/dev/null
    rm -rf "$dir.part" && mkdir -p "$dir.part"
    tar xzf "$tgz.part" -C "$dir.part" && rm -rf "$dir.part/MFC" "$tgz.part"
    mv "$dir.part" "$dir"
    echo "fetch-native.sh: msvc$v"
}
fetch_msvc 6.4 6d4ef930390ca7481ae5b63ea3bf00d62e0993cd95f1d6f42ba06bb30f993c57
fetch_msvc 6.5 16f639b7961ca74b938532d912d2bb655d5ea08b01dc2f50e4a180774d138169
