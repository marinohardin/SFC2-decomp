# syntax=docker/dockerfile:1.7
# VC6 compile oracle: decomp.me msvc6.4 (VC6 SP4) and msvc6.5 (SP5) run under wibo.
# Build with toolchain/build-image.sh. The image is linux/386 with the i686 wibo build: Docker
# Desktop's Rosetta cannot run wibo-x86_64 ("rosetta error: invalid gdt selector index 4", wibo
# switches to a 32-bit code segment), so on Apple Silicon this image runs under QEMU i386.

ARG WIBO_REF=74517de90dc92855571c8fb76941b1051bc41d8e

FROM --platform=linux/386 alpine:3.22 AS wibo
ARG WIBO_REF
RUN apk add --no-cache bash binutils clang clang-dev cmake coreutils gcc git linux-headers lld \
        llvm-dev make mingw-w64-binutils mingw-w64-gcc musl-dev ninja python3
RUN git init /wibo && cd /wibo \
    && git remote add origin https://github.com/decompals/wibo \
    && git fetch --depth 1 origin "$WIBO_REF" && git checkout --quiet FETCH_HEAD
WORKDIR /wibo
RUN cmake -S . -B build --preset release-clang \
        -DCMAKE_TOOLCHAIN_FILE=/wibo/cmake/toolchains/i586-alpine-linux-musl.cmake \
        -DWIBO_VERSION:STRING="$WIBO_REF" \
    && cmake --build build -j4 \
    && strip -g build/wibo && ./build/wibo --version

FROM --platform=$BUILDPLATFORM alpine:3.22 AS compilers
ADD --checksum=sha256:6d4ef930390ca7481ae5b63ea3bf00d62e0993cd95f1d6f42ba06bb30f993c57 \
    https://github.com/OmniBlade/decomp.me/releases/download/msvcwin9x/msvc6.4.tar.gz /dl/msvc6.4.tar.gz
ADD --checksum=sha256:16f639b7961ca74b938532d912d2bb655d5ea08b01dc2f50e4a180774d138169 \
    https://github.com/OmniBlade/decomp.me/releases/download/msvcwin9x/msvc6.5.tar.gz /dl/msvc6.5.tar.gz
RUN for v in 6.4 6.5; do mkdir -p /opt/msvc$v && tar xzf /dl/msvc$v.tar.gz -C /opt/msvc$v \
        && rm -rf /opt/msvc$v/MFC; done

FROM --platform=linux/386 alpine:3.22
COPY --from=wibo /wibo/build/wibo /usr/local/bin/wibo
COPY --from=compilers /opt /opt
COPY msvc /usr/local/bin/msvc
RUN chmod +x /usr/local/bin/msvc && mkdir -p /tmp/msvc
ENV MSVC_VERSION=6.4
WORKDIR /work
ENTRYPOINT ["/usr/local/bin/msvc"]
