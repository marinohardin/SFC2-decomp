# VC6 `Lib/` sets

The decomp.me `msvc6.4` tarball in the image has `Bin/` and `Include/` but no `Lib/`, so link.exe
had no CRT (LIBC, LIBCMT, LIBCPMT, OLDNAMES) or Win32 import libraries. These come from a separate
copy of Visual C++ 6.0's `VC98/Lib`.

- Source: https://github.com/itsmattkc/MSVC600, directory `VC98/Lib`.
- `initial`: commit `17bae53fa330df7b576f25f1f604e4214076a650` ("initial commit"), the VC6 RTM
  libraries, 294 files, 42 MB. CRT objects are build 8168.
- `sp6`: commit `001c4bafdcf2ef4b474d693acccd35a91e848f40` (after "update to sp6"), 296 files,
  43 MB. CRT is SP6 (LIBC build 9782, LIBCPMT 8047).
- Location: `$SFCOP_ROOT/tools/vc6-lib/<set>/`, outside the repo. The files are Microsoft's and
  under its EULA; only the hashes are committed.
- `initial.sha256` and `sp6.sha256` list every file. `fetch.sh` clones the repository (blob-less)
  into `work/src/MSVC600`, extracts each commit's `VC98/Lib` with `git archive` if the set is
  missing, and checks it against the manifest. On 2026-11-02 the on-disk files were also checked
  blob by blob against `git ls-tree` of both commits: identical.

## Which set

`toolchain/bin/msvc-run` uses `MSVC_LIB_SET` (default `initial`), or `MSVC_LIB=dir;dir` for any
other list of directories.

`initial` is the default because it reproduces the shipped scripts' CRT. The Rich header of the
OracleTest.scr built by `scr/test_template.sh` shows 140 C objects and 98 imports at build 8168,
the same counts as the shipped `Assets/Scripts/Ski_1Free4All.scr`. Other shipped scripts were
linked against other CRTs (Gen_Playback.scr has the SP4 CRT, 8797; Met_10Patrol.scr has 8047), and
StarFleetOP.exe has the SP4 CRT (8797). Neither set here is SP4: no SP4 `Lib/` has been found yet.
That matters for byte-matching the game executable's CRT, not for building working scripts.
