# P0-3 report: Ghidra project and symbol recovery for StarFleetOP.exe 2.5.5.2

Run of 2026-11-02, Ghidra 12.2 (Homebrew) headless on the M1 Pro, 8 GB heap, 4 cores. Every number
below comes from [`results/counts.md`](results/counts.md) (written by `scripts/counts.py` from the
exports) or from the step tables in [`results/`](results/). Reproduce with `ghidra/build.sh`
(about 11 minutes; see [`README.md`](README.md)).

## Headline counts

| | Count |
|---|---|
| Functions in the program | **44,232**: 27,479 ordinary functions, 16,544 C++ EH funclets (`Unwind@`/`Catch@`), 209 thunks |
| Functions after plain auto-analysis (step 1) | 34,013; the VC6 RTTI and gap-fill passes added the rest |
| `.text` defined as code or data | 3,661,527 of 3,751,936 bytes; 90,409 bytes left undefined (was 1,142,300 after auto-analysis), mostly padding and 1-3 byte slivers |
| Named by the byte-exact SDK table (step 3) | **1,859 functions** (59 created; 170 carry several identical-code-folded names) |
| Named by SDK Function ID (step 2) | **1,645** unique-name hits applied (751 on default names, 894 replacing class-recovery guesses); 1,427 of those are the same name the exact table gives, so **217** functions keep an SDK-FID name as their only source. 286 more hits had several candidate names and were left unnamed (tag `sdk-fid-multi`). |
| FID vs exact-table conflicts | **1** (`0067c710`: two `std::vector<T>::size` instantiations folded to the same bytes; FID and the table picked different `T`). The exact name won. |
| Named by Ghidra's bundled VS Function ID (CRT) | 307 |
| Functions with a translation unit from an assertion/source path they reference | **639**, covering 226 distinct `.cpp` files (229 path strings in the binary; all 229 are referenced from code) |
| Further functions with a TU inferred from both neighbours | 1,019 (weaker evidence, flagged `neighbour` in `functions.csv`) |
| RTTI classes recovered | **2,104** types; 2,033 polymorphic with hierarchy descriptor and vftables (2,149 vftables, 27,479 slots); 1,752 with at least one base class; 1,954 with a recovered structure size |
| Class recovery | 1,493 constructors, 1,845 destructors, 1,854 deleting destructors, 643 undetermined ctor/dtor, 27,478 virtual-function slots assigned |

Name quality of the 27,479 ordinary functions: 2,401 carry a real symbol name (1,859 exact, 307 VS
FID, 217 SDK FID, 18 other), 3,942 are class-recovery constructors/destructors/deleting destructors,
4,281 have a known class but a placeholder method name (`Class::vfunctionN`), and 16,855 are still
`FUN_xxxxxxxx`.

## Timings and memory

| Step | Wall time | Peak RSS |
|---|---|---|
| 1 import + auto-analysis | 154 s | 3.5 GB |
| 1b VC6 RTTI + gap fill + class recovery | 235 s | 2.1 GB |
| 2a import 317 SDK objects | 26 s | 1.0 GB |
| 2a analyse SDK objects | 199 s | 2.6 GB |
| 2b build fidb | 10 s | 1.8 GB |
| 2c apply fidb | 12 s | 2.1 GB |
| 3 exact matches | 6 s | 0.6 GB |
| 4 translation units | 8 s | 1.7 GB |
| 5 exports + .gzf + tarball | 16 s (+ tar) | 1.3 GB |

Disk: project 622 MB, tarball 80 MB, `.gzf` 37 MB, fidb 0.8 MB, all under
`$SFCOP_ROOT/work/ghidra/`.

## What had to be fixed to get here

1. **No arm64 decompiler in Homebrew Ghidra 12.2.** `os/mac_arm_64/decompile` is missing, so the
   switch-table analyzer and anything using `DecompInterface` silently do nothing (the log shows a
   single ERROR line). `scripts/build_decompiler.sh` builds it from the sources shipped in the
   install and loads it as an extension from an isolated settings dir. Other sessions running
   Ghidra on this Mac hit the same problem; a hand-over note is in the shared notes folder.
2. **Ghidra's RTTI analyzer does not understand VC6 RTTI.** It labelled all 2,105 type
   descriptors but created no complete object locators, hierarchy descriptors or vftables, and
   `RecoverClassesFromRTTIScript` then reported "No new classes recovered". VC6 base class
   descriptors are 0x18 bytes (no `pClassDescriptor`), and the analyzer's validation evidently
   rejects that layout. `SfcVc6Rtti.java` finds 2,149 COLs and 2,149 vftables directly and defines
   VC6-sized structures under the names the class recoverer looks up; class recovery then works.
3. **About 1.1 MB of `.text` was never disassembled.** VC6 packed functions without alignment, so
   gaps begin on prologue bytes right after the previous function; Ghidra's function-start search
   misses them. `SfcFillGaps.java` creates 6,132 functions over 8 passes, with auto-analysis
   between passes. Cost: bad-instruction bookmarks went from 8 to 130, so about 120 gap-fill starts
   ran into data and some of those functions are junk. Check: no exact-table address fell inside
   a gap-fill function body (step 3 trimmed nothing). With the extra code, class recovery assigns
   4,796 member functions and 1,493 constructors; an exploratory run without gap fill (log not
   kept) gave 2,192 and 527, and 413 functions with a TU instead of 639.
4. **Headless import of the `.lib` files only finds stubs.** Members are named with relative paths
   (`.\Release\x.obj`, `..\..\y.obj`) and `-import -recursive` skips them. `SfcImportLibs.java`
   (after Ghidra's `ImportMSLibs.java`) imports 317 objects. `dmp3D_MSVCR.lib` is truncated: its
   last (10th) member runs past the end of the file and is dropped (`prep_libs.py`). Two copies of the 3DNow! asm object
   `m3dk6.obj` (dmp3D, sfcTiger) have no MSCoffLoader load spec and are not in the fidb.
5. **Path strings pushed from undiscovered code.** Before gap fill, 143 of the 229 path strings
   had no reference because the `push offset "F:\...\x.cpp"` instructions were never
   disassembled. After gap fill all 229 are referenced (1,284 string/function pairs). Step 4 also
   adds a DATA reference for any instruction scalar equal to a defined string address; in the
   clean run it found nothing left to add.

## Notes on the inputs

- `analysis/results/StarFleetOP_2552_sdklib_matches.tsv` has 3,758 rows but only 2,396 distinct
  names at 1,934 distinct addresses (rows are repeated; 176 addresses carry several names through
  identical-code folding). The "3,758 matches" in the hand-over is a row count.
- 74 addresses in the TSV are named only by an object-section id
  (`.\sfcTiger Release\tstring.obj:.text#258`): the matcher found no function symbol for that
  COMDAT. They are tagged `sdklib-exact-section` with the id in a comment, not renamed.
- One overlapping pair: `?mDispose@tBasicFile@@QAEPAV1@XZ` (16 bytes at `0062bf4f`) lies inside
  `?mRelease@tINIFileSimple@...` (32 bytes at `0062bf40`). The second is skipped and logged. Worth
  a look by whoever owns `libmatch.py` (P0-7).
- The exe has **no Crypto++ RTTI** at all (no `@CryptoPP@@` type descriptor), so Crypto++ code is
  only recognisable by exact/FID names. Only 8 functions carry a `CryptoPP::` name, and several of
  the largest unnamed regions sit between Crypto++ and WONAPI functions (table below). Compiling
  Crypto++ 3.1 (the `crypto31` path in the binary) with VC6 into a fidb, or matching against
  P0-6's pinned sources, should name much of it.

## Library flag

`is_library` covers 1,125 functions: 562 STL, 304 CRT (VS FID), 210 WONAPI, 41 import thunks,
8 Crypto++, 0 GameSpy. The flag is name/namespace/TU based, so unnamed library code is still 0;
GameSpy currently has neither names nor source paths in the binary.

## Largest remaining unnamed regions

Runs of consecutive `FUN_` functions (EH funclets ignored), largest first. Full list:
[`exports/unnamed_regions.csv`](exports/unnamed_regions.csv).

| Start | End | Functions | Bytes | Named before | Named after |
|---|---|---|---|---|---|
| 006bf590 | 006c842e | 154 | 29,391 | CryptoPP::HashModule::CalculateDigest | tDynamicMemoryFreeList::vfunction21 |
| 006b9c40 | 006bf532 | 69 | 21,465 | CryptoPP::HashModule::CalculateDigest | tDynamicMemoryFreeList::vfunction21 |
| 006ed020 | 006f2661 | 73 | 21,361 | WONAPI::BigInteger::BigInteger | WONAPI::WONAPIPumpThread dtor |
| 0074096a | 0074531e | 81 | 18,644 | __fload (CRT) | Replicator::operator+= |
| 0072f110 | 00733533 | 95 | 16,169 | CryptoPP::DSABase::MaxDigestLength | type_info::~type_info |
| 0052c8c8 | 005307d8 | 90 | 15,846 | tNetMsg::~tNetMsg | tNetMsg_kAdjustSpeed dtor |
| 00614210 | 00618923 | 102 | 15,384 | twVolume::~twVolume | tSoundSelection::mGetSoundPartHasNoMoreUseForThis |
| 006c99b0 | 006cdb42 | 103 | 15,349 | tDynamicMemoryFreeList::vfunction21 | WONAPI::AsyncSocket::AsyncSocket |
| 0056edc3 | 00572927 | 149 | 15,205 | nAsyncDatabaseProcedure::tAsyncDatabaseSingle::vfunction66 | tTurn::tTurn |
| 006ad7d0 | 006b104b | 27 | 14,312 | GetDXVersion | tSWView3D::vfunction98 |
| 005bd3b9 | 005c0bb2 | 82 | 14,187 | nDatabase::tID::tID | tMapRelayS::tMapRelayS |
| 00530b70 | 0053430e | 49 | 14,150 | tNetMsg_kScriptPrimitive dtor | tShipInGame::tShipInGame |
| 005f8558 | 005fb9aa | 111 | 13,356 | tMetaClientChatCtlrC ctor/dtor | tMemberCallback<tMetaClientChatCtlrC,...> |
| 005c2394 | 005c5682 | 133 | 13,006 | tMapSettings::~tMapSettings | tMapWatchdog::tMapWatchdog |
| 005b74b8 | 005ba478 | 71 | 12,153 | tMapCtlrS::~tMapCtlrS | nDatabase::tID::tID |

The `0x6b0000`–`0x6f3000` and `0x72f000`–`0x746000` bands are Crypto++, WONAPI and CRT
(`__fload`, `__allshl`, `___add_12` nearby). Most other large regions sit inside game TUs between
RTTI-named neighbours.

## Caveats

- `rtti-class-recovery` names come from Ghidra's heuristics. SDK FID replaced 894 of them, and
  several of those were constructor/destructor confusions (e.g. `0061bae0` was called
  `tSession::~tSession`; FID with a score of 544 says `tFileSession::tFileSession`). Treat
  class-recovery ctor/dtor names as hints.
- Class structure sizes are Ghidra's estimates from field accesses (`size_source` says so), not
  `sizeof`.
- Neighbour-inferred TUs assume MSVC links one TU's functions contiguously; they are kept apart
  from path-string evidence.
- The FID thresholds are Ghidra's defaults (score >= 14.6, multi-name >= 30). Lower thresholds or
  the fuzzy matcher (P0-7) would name more of the small functions the fidb excluded
  (2,800 failed the minimum short-hash length).
