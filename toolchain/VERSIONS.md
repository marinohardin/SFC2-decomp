# Tool versions

Every compiler and tool the builds and `scripts/check.sh` touch, with the version it is pinned
to or, for a tool the machine provides, the version it was last verified with. A "minimum" is
what the build files require; "verified" is the owner's Mac and the runner `sfcop-mac`
(2026-11-03, macOS 27.0 arm64). Library dependencies are in `third_party/INVENTORY.md`.

## Native build (CMake, `cmake --preset <platform>`)

| Tool | Pinned or minimum | Verified | Where it is set |
| --- | --- | --- | --- |
| CMake | 3.28 minimum | 4.4.4 (Homebrew) | `CMakeLists.txt`, `CMakePresets.json` |
| Ninja | any | 1.13.2 (Homebrew) | `CMakePresets.json` (generator) |
| Apple clang (C11, C++17) | any with C++17 | 21.0.0 (clang-2100.3.34.2), Xcode 27.0 (27A266a), macOS SDK 27.0 | `macos-arm64` preset |
| SDL2 API for the boot platform | TEMPORARY, leaves with Wave 1 B1/B3: sdl2-compat 2.32.74, fetched and pinned in `third_party/` over the pinned SDL3 3.4.18 (built shared for it) | the same | `third_party/CMakeLists.txt` |
| Python | 3.11 minimum | 3.13.2 (`python3` on PATH); CMake found 3.14.3 | `cmake/SfcopLifter.cmake` (runs `cmake/lifter/locate.py`) |
| Git | any | 2.55.0 | `cmake/SfcopLifter.cmake` (checks the pcrecomp patches) |
| clang-format | LLVM style, see `.clang-format` | not installed yet | `.clang-format` |
| Windows x64, Linux x64 cross toolchains | none yet: the presets configure only | - | `cmake/toolchains/*.cmake` (stubs until Wave 2 A5) |

## The lift step (`cmake/lifter/`)

| Tool | Pinned | Where it is set |
| --- | --- | --- |
| pcrecomp | commit `ef3f0da6e08a8eb7a3f869273f3df071ca8d7373` plus `cmake/lifter/patches/` | `cmake/lifter/lift.sh`, `cmake/lifter/setup.sh` |
| Docker | any; verified with Docker Desktop, server 29.9.2 | `cmake/lifter/lift.sh` |
| Lifter image `sfcop-lifter-py:<Dockerfile hash>` | base `python:3.13-slim@sha256:ae14d4ce8771606477a81446e7ef429edd6762305fc50c84507c1041fa8285d1` (Python 3.13.18, 2026-10-28); capstone 5.0.7, pefile 2024.8.26, unicorn 2.1.4 | `cmake/lifter/Dockerfile` |

## 32-bit Windows build and the byte check (`toolchain/`, `harness/`)

| Tool | Pinned | Where it is set |
| --- | --- | --- |
| Visual C++ 6.0 SP4 (compiler 12.00.8799), from decomp.me `msvc6.4` | checksum-pinned tarball | `toolchain/Dockerfile` |
| wibo (in the image) | commit `74517de` | `toolchain/Dockerfile` |
| wibo (native, macOS, the harness's compiler) | release 1.2.0 `wibo-macos`, checksum-pinned; same compiler tarballs as the image | `toolchain/fetch-native.sh` |
| objdiff-cli | 3.8.3, checksum-pinned | `toolchain/calibrate.sh` |
| VC6 `Lib/` (CRT and import libraries) | hashes in `toolchain/vc6-lib/` | `toolchain/vc6-lib/fetch.sh` |

## Checks (`scripts/check.sh`)

| Tool | Pinned or minimum | Verified |
| --- | --- | --- |
| Python | 3.11 minimum, standard library only | 3.13.2 |
| pytest | any | 9.2.0 |
| CTest | with CMake above | 4.4.4 |
