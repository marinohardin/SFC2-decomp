# toolchain: VC6 SP4 compile oracle (tasks P0-2, P1-8)

A Docker image that runs Taldren's compiler, Visual C++ 6.0 SP4, under
[wibo](https://github.com/decompals/wibo), plus host wrappers, a calibration that proves it
reproduces Taldren's own objects (results: [`calibration/report.md`](calibration/report.md)),
and a one-command build of OP API mission scripts (`.scr` DLLs, see
[Building mission scripts](#building-mission-scripts-p1-8)).

## Quick start

```sh
toolchain/calibrate.sh
```

On a fresh checkout this builds the image if it is missing (about 18 minutes, see below), downloads
objdiff-cli v3.8.3 into `../tools/objdiff/` (checksum pinned), compiles the SDK sources, diffs them
and rewrites `calibration/report.md`. Arguments pass through to `calibration/calibrate.py`, e.g.
`toolchain/calibrate.sh --variants dsp --jobs 2`. A full run of all seven variants takes about
10 minutes with 3 parallel compiles (the default is 4, or set `JOBS=`); `--variants dsp` alone
takes about 1.5 minutes. `--no-compile` rescores the objects of the previous run.

Requirements: Docker Desktop (Apple Silicon is fine), python3, the retail SDK under
`reference/` (hash-checked against `analysis/results/retail_hashes.sha256` before use).

## The image

```sh
toolchain/build-image.sh          # tags sfcop-msvc6:latest
```

- `Dockerfile` builds wibo from a pinned `main` commit (`74517de`, 2026-11-02, after PR #141
  added `lstrcmpiW` for `link.exe`) and unpacks the decomp.me compiler tarballs `msvc6.4`
  (VC6 SP4) and `msvc6.5` (SP5) into `/opt/msvc6.4` and `/opt/msvc6.5`, checksum-pinned.
- The image is **linux/386** with the i686 wibo build. Docker Desktop's Rosetta cannot run the
  x86_64 wibo (`rosetta error: invalid gdt selector index 4`: wibo switches to a 32-bit code
  segment, which Rosetta for Linux does not support), so the image runs under QEMU i386. A
  typical SDK file compiles in 3 to 16 s.
- The build context is staged in `../work/toolchain/context/`; the downloads happen inside
  Docker. The image is 102 MB. Docker keeps it in its VM disk on the internal drive, which cannot be
  moved from here. A build also leaves about 1.5 GB of build cache for the wibo builder stage
  (Alpine with clang/LLVM); it was pruned on 2026-11-02 (`docker system df` shows 0 B), so the
  next image build takes the full 18 minutes and recreates it. Prune it again afterwards with
  `docker builder prune` if the internal disk is short.
- The wibo build stage compiles under QEMU and takes about 17 minutes.

Tool versions in `msvc6.4` (from the PE version resources): `CL.EXE` 12.00.8804, `C1XX.DLL`
12.00.8867, `C2.DLL` 12.00.8799, `LINK.EXE` 6.00.8447. Objects carry `@comp.id 0x000B225F`
(C++ front end, build 8799), the same stamp as the retail `ScriptInterfaceR.lib` members and as
the bulk of `StarFleetOP.exe`'s Rich header; a test exe linked here gets the Rich entry
`0x0a / 8799`. `msvc6.5` differs in `C1XX.DLL` (8964) and `C2.DLL` (8966) and stamps 8966.

## Compiling

`toolchain/bin/cl`, `link`, `lib`, `dumpbin` run the tool in a fresh container (about 0.7 s of
overhead per call). The SFCOP root (the repo's parent, holding `work/` and `tools/`) and the
current directory are mounted at their host paths, so paths in arguments work unchanged:

```sh
cd $SFCOP_ROOT/work/somewhere
/path/to/SFCOP-decomp/toolchain/bin/cl /nologo /c /MT /GR /GX /O2 foo.cpp     # -> foo.obj
/path/to/SFCOP-decomp/toolchain/bin/lib /nologo /out:foo.lib foo.obj
MSVC_VERSION=6.5 /path/to/SFCOP-decomp/toolchain/bin/cl ...                  # SP5 instead
```

Environment:

- `MSVC_VERSION`: `6.4` (default) or `6.5`.
- `MSVC_EXTRA_INCLUDE`: `;`-separated dirs put before the compiler's own `INCLUDE`.
- `SFCOP_MSVC_MOUNTS`: `host:container;...` extra bind mounts. wibo maps `C:\x` to `/x`, so a
  mount at `/Projects/...` lets a file be compiled under its original `C:\Projects\...` path.
- `SFCOP_MSVC_IMAGE`, `SFCOP_MSVC_NAME` (container name, so a caller can `docker kill` it).
- `MSVC_LIB_SET`: the VC6 `Lib/` set under `../tools/vc6-lib/` for `link`: `initial` (default,
  VC6 RTM CRT) or `sp6`. `MSVC_LIB=dir;dir` overrides it with any host directories. See
  [`vc6-lib/PROVENANCE.md`](vc6-lib/PROVENANCE.md).

Limits:

- `lib` and `dumpbin` run `LINK.EXE /lib` and `LINK.EXE /dump`: `LIB.EXE` and `DUMPBIN.EXE` only
  re-spawn `LINK.EXE`, and wibo's child-process `clone()` fails under QEMU
  (`clone: Invalid argument`). The `.dsp` files use `link.exe -lib` anyway.
- The decomp.me tarballs are compile-only: no `Lib/` directory. The CRT and Win32 import
  libraries come from a separate VC6 `VC98/Lib`, kept outside the repo under
  `../tools/vc6-lib/` and mounted at run time (`vc6-lib/fetch.sh` fetches and hash-checks it).
- For the same `clone()` reason, `link /dll` cannot build a DLL's export file itself (it spawns
  `link -lib -def` and stops with LNK1141). Build the `.exp` first with `lib /def` and pass it to
  `link`, as `build_scr.py` does. `wibo-qemu-spawn.patch` is an untested wibo fix (fall back to
  `fork()` + `pidfd_open` when `CLONE_PIDFD` is rejected) that would remove this limit and the
  previous one; it is not applied because rebuilding wibo under QEMU takes about 17 minutes and
  the internal disk has little room for another build cache.
- Paths beginning with `/` are read as options by `link` (LNK4044) and `cl` (D2003). Prefix
  host paths with `Z:` (wibo maps `Z:\x` to `/x`).
- One `cl` call with several sources sharing a `/YX` `.pch` can hang at 0% CPU under wibo.
  Compile one source per call (the calibration does).
- Wine was not needed; wibo handles `cl`, `link` and `link /lib`.

## Native compiler (macOS)

```sh
toolchain/fetch-native.sh         # tools/wibo/1.2.0/wibo-macos, tools/msvc6/msvc6.4 and 6.5
toolchain/bin/cl-native /nologo /c /MT /GR /GX /O1 foo.cpp
```

The wibo 1.2.0 release runs `CL.EXE` directly on Apple Silicon under Rosetta 2, with no Docker
and no QEMU (wibo 1.0.0 does not: `C2.DLL` stops with an internal compiler error). The compiler
files are the image's own decomp.me tarballs, with the same checksums. `scripts/setup.sh` fetches
it on macOS, and the harness's own scoring then compiles with it (`harness/lib/paths.py`,
`HARNESS_CL`, chosen by `pick_cl`); `HARNESS_COMPILER=docker` sends it back to the image.
`paths.CL` and the default of `source.compile_unit`, which a consumer's build calls
(`docs/INTERFACE.md`), stay the image. `cl-native` takes `MSVC_VERSION`
and `MSVC_EXTRA_INCLUDE` like `cl`, but not `SFCOP_MSVC_MOUNTS`; `link`, `lib`, `dumpbin` and
the mission-script build still run in the image.

Checked on 2026-11-19 against the image, over every unit with a claim (158):

- `harness/build-and-verify.sh --all` printed the same verdict and score for all 4,228 claimed
  functions, line for line, with either compiler.
- 133 objects were byte-identical apart from the COFF timestamp. The other 25 hold the same
  sections, bytes and relocations, but list some COMDAT data sections (RTTI records) in another
  order. Each compiler is deterministic: a second run of either reproduced its own objects.
  So the native compiler is for the harness's per-function scores only: anything that links
  objects or compares whole objects (a link, a consumer of `docs/INTERFACE.md`) uses the
  image's `toolchain/bin/cl`. CI's per-pull-request `check` also compiles natively; the image
  stays the compiler of record through the scheduled Docker audit
  (`.github/workflows/docker-audit.yml`), which also runs before a consumer's pin is bumped or a
  release. The audit rebuilds every matched function with the image and holds each score to the
  committed matched set; it fails if the rebuild did not run. A failed audit shows as a red run
  under "Docker audit" in the repository's Actions tab, and the integrator checks it before
  proposing a batch and before a consumer's pin bump.
- Total compile time for the 158 units fell from 842 s to 90 s, and a full verify from 410 s to
  215 s; most of what is left is objdiff.

## Calibration

`calibration/calibrate.py` (stdlib Python; `calibrate.sh` is the entry point):

1. Checks the hash of `ScriptInterfaceR.lib`, copies the SDK's `Scripts/` and `STL/` trees to
   `../work/toolchain/calibration/sdk/` (the reference tree is never touched) and extracts the 23
   `.\Release\*.obj` members to `../work/toolchain/calibration/target/`.
2. Reads the Release `# ADD CPP` line and the source list from `_ScriptInterface.dsp` and
   `_Shared.dsp`, and compiles each source from its project directory with those flags verbatim,
   plus `/I <SDK STL/Include>` first (the SDK readme says to copy those headers over the
   compiler's) and per-file output paths (`/Fo /Fd /Fp`, and `/Fr<dir>`). Each source is passed
   as `C:\Projects\Taldren\Projects\SFC2_5\SCRIPTS\...`, Taldren's original path, with the staged
   tree mounted there, so `__FILE__` strings and `.file` records match.
3. Runs each variant (`dsp` baseline, `dsp-nopch`, `sp5`, `ox`, `ob1`, `gy`, `gf`), then
   `objdiff-cli report generate` per variant, a one-shot `objdiff-cli diff` per file for
   function-level results, and `strictcmp.py`, an independent byte-level comparison of every
   COFF section, relocation and symbol.
4. `make_report.py` renders `report.md` from `results/*.json` and appends the hand analysis in
   `analysis.md`.

Scratch output (about 12 MB; `.pch`/`.sbr`/`.idb` intermediates are deleted after each compile)
is in `../work/toolchain/calibration/`; delete it freely. Raw per-variant data is kept in
`calibration/results/*.json`.

## Building mission scripts (P1-8)

```sh
toolchain/build_scr.sh <srcdir> <out.scr> [--jobs N]     # e.g. a copy of oracle/scr/
toolchain/scr/test_template.sh [Name]                     # the proof: SDK template -> .scr -> checks
```

`build_scr.sh` runs `scr/build_scr.py`. If `<srcdir>` has a `.dsp`, its Release compile and link
lines and source list are used; otherwise every `.cpp` is compiled with the flags of the SDK's
Single-mission template. The sources and a copy of the SDK's `Scripts/` and `STL/` are staged in
`../work/toolchain/scr/<name>/` (the script sits at `Scripts/Missions/<name>/`, the depth the
template's relative `/I` paths expect), compiled one file per `cl` call, and linked with the
template's libraries: the SDK `Scripts/Libraries/` plus the VC6 `Lib/` set. The `.scr` name is
the output file's name. Each run takes about a minute for the 15-file template.

Four things differ from building in Visual Studio, each forced by the inputs or the emulator:

1. **The SDK's `dmp3D_MSVCR.lib` is truncated as shipped** (848,082 bytes of about 4.2 MB, and
   the file's hash matches the retail install, so it is not a copying fault). link.exe stops with
   LNK1127. `build_scr.py` rebuilds a library from the 9 intact object members into
   `../work/toolchain/scr/_salvaged/` and puts it first on the library path.
2. **Two functions every script needs are in the lost part of that library**:
   `tErrInfo::tErrInfo` and `tMatrix::mIdentity`. `scr/dmp3d_shim.cpp` supplies them,
   reconstructed from the copies statically linked into the shipped `Gen_Playback.scr`;
   `scr/verify_shim.py` checks that the compiled shim is byte-identical to those copies (both
   64 bytes, relocations masked). Scripts that call other functions from the lost members will
   fail to link with LNK2001 naming them; add them to the shim the same way.
3. **The export file is built separately** (`lib /def /name:<out>.scr`), because of the spawn
   limit above. The exports come from `-export:_ScriptEntry -export:_MissionSpecEntry` in the
   `.drectve` section of `ScriptInterfaceR.lib(ScriptInterface.obj)`; `build_scr.py` extracts
   every SDK library member with `-export:` directives and adds it to the `lib /def` inputs, then
   links with the resulting `.exp`.
4. The template's link line has `machine:I386` without its `/`, which link.exe would take as an
   input file; `build_scr.py` corrects it.

`scr/test_template.sh` instantiates `_ScriptTemplates/Single` as `OracleTest`
(`scr/new_from_template.py`, which substitutes `##ProjName##` as Taldren's wizard did), builds it
into `../work/toolchain/scr-out/OracleTest.scr`, and runs `scr/check_scr.py` and
`scr/verify_shim.py`. `check_scr.py` compares the result against all 88 shipped
`Assets/Scripts/*.scr`: every shipped script exports exactly `MissionSpecEntry` and `ScriptEntry`
and names its export directory after its own file, and so does the built one; machine, PE
characteristics (0x210e), linker version (6.0), image base (0x10000000) and subsystem (GUI) also
match. The Rich header shows the same 140 C objects at CRT build 8168 as the shipped
`Ski_1Free4All.scr`. Last run: [`scr/results/test_template.log`](scr/results/test_template.log)
(PASS, 352,256 bytes). The script has not yet been loaded by the game; that is the oracle's job
(P0-5).
