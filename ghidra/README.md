# ghidra/ — P0-3 Ghidra project and symbol recovery

Headless, scripted symbol recovery for `StarFleetOP.exe` 2.5.5.2 with Ghidra 12.2. One command
rebuilds the project and every export from the retail inputs:

```bash
ghidra/build.sh
```

`ghidra/build.sh 3 4 5` reruns only the listed steps on the existing project. A full run takes about
11 minutes on the M1 Pro; run it from bash (the scripts are bash, `env.sh` is not zsh-safe). Counts, timings and
the list of remaining unnamed regions are in [`REPORT.md`](REPORT.md) (numbers generated into
[`results/counts.md`](results/counts.md) by `scripts/counts.py`).

## Where things live

| What | Path |
|---|---|
| Ghidra project (exe + imported SDK `.lib` members under `/sdklibs`) | `$SFCOP_ROOT/work/ghidra/projects/SFCOP.gpr` |
| Project snapshot (tarball of the above) | `work/ghidra/SFCOP_ghidra_project.tar.gz` |
| Program only, importable into any Ghidra 12.2+ project without re-analysis | `work/ghidra/StarFleetOP_2552.gzf` |
| SDK Function ID database | `work/ghidra/sfcop_sdk.fidb` |
| Raw headless logs | `work/ghidra/logs/` |
| Deliverable CSVs | `ghidra/exports/` |
| Per-step tables (FID hits, exact-match log, TU pairs, gap-fill passes, stats, timings) | `ghidra/results/` |

To reuse without re-analysing: unpack the tarball into a projects directory and open `SFCOP.gpr`,
or File → Import the `.gzf` into your own project. Either way, also use the isolated settings dir
below if you want the arm64 decompiler.

## Requirements and environment

- Ghidra 12.2 from Homebrew (`GHIDRA_HOME`, default `/opt/homebrew/Cellar/ghidra/12.2/libexec`),
  JDK 25, Xcode command line tools (clang, make, bison, flex), python3 (stdlib only).
- `scripts/env.sh` holds every path and limit: 8 GB heap (`GHIDRA_HEADLESS_MAXMEM`), 4 cores
  (`CORES`), work dir `WORK`.
- Ghidra runs with `-Dapplication.settingsdir=$WORK/settings` (plus cache/temp dirs under `$WORK`),
  so nothing touches `~/Library/ghidra`, the Homebrew tree or the internal disk. The SDK fidb is
  registered only in that isolated settings dir, and only for the duration of step 2c.

## Steps

| Step | Script | What it does |
|---|---|---|
| 0 | `build.sh` / `scripts/build_decompiler.sh` | Verifies the exe and SDK libs against `analysis/results/retail_hashes.sha256`. The Homebrew Ghidra ships no `os/mac_arm_64/decompile`; this builds it from the C++ sources inside the install (35 s) and installs it as an extension in the isolated settings dir. Without it the switch-table analyzer and class recovery silently do nothing. |
| 1 | `scripts/step1_import.sh` | Imports the exe (PE, x86:LE:32, windows cspec) and runs full auto-analysis with the RTTI analyzer, Ghidra's built-in VS Function ID databases and the MS demangler on (`SfcAnalysisOptions.java` prints the analyzer set into the log). |
| 1b | `scripts/step1b_classes.sh` | `SfcVc6Rtti.java`, `SfcFillGaps.java`, then Ghidra's `RecoverClassesFromRTTIScript.java`. Ghidra's RTTI analyzer finds the type descriptors but no complete object locators or vftables in this VC6 binary: VC6 base class descriptors are 0x18 bytes (no `pClassDescriptor`), so its validation fails and class recovery finds nothing. `SfcVc6Rtti.java` marks up COLs, hierarchy descriptors, base class arrays/descriptors and vftables with VC6-sized structures under the names the class recoverer looks up, and creates functions at vftable slots. `SfcFillGaps.java` then disassembles the ~1.1 MB of `.text` that auto-analysis never reached (gaps start on prologues right after the previous function; padding, lone `ret` and jump-table-looking starts are skipped; auto-analysis runs between passes). |
| 2 | `scripts/step2_fid.sh` | `prep_libs.py` copies the five SDK archives (`ScriptInterfaceR`, `SharedR`, `cryptlibR`, `dmp3D_MSVCR`, `sfcTiger_MSVCR`; `BugslayerUtil.lib` is an import library and is skipped) and cuts the truncated last member off `dmp3D_MSVCR.lib`. `SfcImportLibs.java` reads each archive and loads every COFF member with `MSCoffLoader` into `/sdklibs/<lib>/` (headless `-import -recursive` skips members whose names are relative paths such as `.\Release\x.obj`); a second headless run (`-process -recursive`) analyses them with Ghidra's own `FunctionIDHeadlessPrescript/Postscript` (FID and demanglers off). The lib copies are deleted afterwards. `SfcBuildFidb.java` builds `sfcop_sdk.fidb`, one FID library per archive, with Ghidra's `common_symbols_win32.txt`. `SfcApplyFid.java` applies only that fidb: unique-name hits (score >= 14.6) are named (mangled label + demangled function name, tag `sdk-fid`), multi-name hits are tagged `sdk-fid-multi` and left unnamed, IMPORTED/USER_DEFINED names are never overwritten. |
| 3 | `scripts/step3_exact.sh` | `SfcApplyExact.java` applies `analysis/results/StarFleetOP_2552_sdklib_matches.tsv`: creates missing functions with the TSV size, sets the name USER_DEFINED and demangles it, tags `sdklib-exact` (`sdklib-exact-icf` where several names share one address through identical-code folding; the shortest name is primary, the rest stay as labels). Exact matches override FID; every disagreement is logged in `results/exact_apply.tsv` (`fid-conflict`, `replaced`). Addresses named only by an object-section id (`x.obj:.text#n`) are tagged `sdklib-exact-section` with the id in a comment instead of renamed; a match starting inside the previous match is skipped and logged (`overlaps-previous-match`). |
| 4 | `scripts/step4_tu.sh` | `SfcMapTU.java` first adds a DATA reference for any instruction scalar equal to a defined string address (a safety net; analysis normally made them), then finds every string containing `\` and ending in a C/C++ extension (defining it if analysis left raw bytes), follows references (direct, or one hop through a data pointer) to functions, and tags them `TU:<path>` (`.c/.cpp`) or `HDR:<path>` (headers). |
| 5 | `scripts/step5_export.sh` | `SfcExport.java` writes the CSVs below; `join_eh_prologs.py` joins each function Ghidra splits after its 10-byte exception-frame prolog (`mov eax, <table>; call __EH_prolog`) to its body, in `functions.csv` and the CSVs that name functions; `SfcSaveGzf.java` saves the `.gzf`; the project is tarred; `counts.py` writes `results/counts.md`. |

## Exports (`ghidra/exports/`)

- `functions.csv`: `address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,headers,is_library,library,kind,tags`.
  - `name_source`: `sdklib-exact` (byte-exact table), `sdk-fid` (SDK Function ID), `ghidra-fid-vs`
    (Ghidra's bundled Visual Studio FID: CRT), `rtti-class-recovery` (function sits in a class
    namespace from RTTI / class recovery, e.g. constructors, destructors, `vfunctionN`),
    `eh-analyzer` (`Unwind@`/`Catch@` funclets), `imported`, `ghidra-analysis`, `default`
    (`FUN_xxxxxxxx`).
  - `translation_unit` / `tu_evidence`: `assert-path` = the function itself references the path
    string; `neighbour` = inferred because the nearest TU-tagged functions before and after it carry
    the same single TU (MSVC links a TU's functions contiguously; weaker evidence, kept separate).
    Several TUs are joined with `;`.
  - `library`: `crt`, `stl`, `cryptopp`, `wonapi`, `gamespy`, `import-thunk`; `is_library` is 1 when
    set. Rules are name/namespace/TU based (see `SfcExport.java#library`), so unnamed library code
    stays 0.
  - `kind`: `function`, `thunk`, `eh-funclet`.
- `classes.csv`: `class,parents,all_bases,vtables,size,size_source,type_descriptor,is_library`.
  Parents come straight from the RTTI base class arrays (direct bases = skipping each base's
  `numContainedBases`); `size` is the structure length RecoverClassesFromRTTIScript inferred
  (an estimate from field accesses, not ground truth).
- `vtables.csv`: `vtable_address,class,vtable_name,slot,function_address,function`.
- `strings_to_functions.csv`: every defined string of 4+ characters referenced from a function
  (`via` = `direct` or `ptr@<pointer address>`).
- `unnamed_regions.csv`: runs of consecutive functions that still have default names, largest first.

## Results tables (`ghidra/results/`)

`timings.tsv` (wall time, peak RSS per headless run), `stats_*.txt` (function/symbol counts after
each step), `vc6_vftables.tsv`, `gap_fill.tsv`, `class_recovery_summary.txt`,
`sdk_libs_prepared.tsv`, `sdk_lib_members.tsv` (every archive member and whether it was imported),
`fidb_build.tsv`, `fid_sdk_matches.tsv` (every SDK FID hit with the name it replaced),
`exact_apply.tsv` (every event of step 3, summary line last), `tu_paths.tsv`, `tu_summary.txt`,
`export_summary.txt`, `snapshot_files.txt`, and `counts.md` (the numbers quoted in `REPORT.md`).
