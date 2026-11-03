#!/bin/sh
# Build the sfcop-msvc6 image. The build context is staged under work/toolchain/context so
# nothing large lands in the repo; the compiler tarballs and wibo source are fetched by Docker.
set -e
here=$(cd "$(dirname "$0")" && pwd)
work=${SFCOP_WORK:-$(cd "$here/../.." && pwd)/work}/toolchain/context
mkdir -p "$work"
cp "$here/Dockerfile" "$here/msvc" "$work/"
exec docker build --platform linux/386 -t "${SFCOP_MSVC_IMAGE:-sfcop-msvc6:latest}" "$work"
