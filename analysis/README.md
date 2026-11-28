# analysis/

First-pass static analysis of the retail binaries (2026-11-02). Scripts need `pip install pefile`.

- `scripts/peinfo.py <exe> [full]` — PE headers, Rich header (compiler builds), imports.
- `scripts/libmatch.py <exe> <lib>...` — matches per-function COMDAT code from the SDK's MSVC 6 static libs
  (`OP API - R2.1/Scripts/Libraries/*.lib`) into the exe's .text byte-for-byte (relocations masked).
  Writes `lib_matches.tsv` = `address  size  mangled_name`.
- `scripts/funccount.py <exe>` — rough function-count heuristics (call targets, data->text pointers).

Results:
- `StarFleetOP_2552_sdklib_matches.tsv` — 3,758 unique name->address matches in the patched 2.5.5.2 game exe, from a damaged copy of the SDK's 3D library (9 of its 56 objects). With the whole library the same command gives 4,277 rows (2,885 distinct names at 2,389 addresses), and the figures and calibration below are from that run. `tools/units.py` reads this file and a replacement regroups the 3D library's units under `op/` and `op/UNITS.tsv`, so the file is replaced in the pull request that regenerates them.
- `StarFleetOP_2500_sdklib_matches.tsv` — same against the original disc 2.5.0.0 exe (2,989 matches; different optimization).
- `ServerPlatform_sdklib_matches.tsv` — 3,906 matches in the Dynaverse server kit exe (2.5.5.2 kit, linked 2004-01-20).
- `StarFleetOP_2552_strings.txt` — `strings -n 8` of the game exe.

## Fuzzy SDK matching (P0-7): `scripts/fuzzymatch.py`

### Rerun

Python 3.13 venv with `pefile` and `capstone` (5.0.x), on the external drive:

```sh
W=$SFCOP_ROOT/work/fuzzy
PIP_CACHE_DIR=$SFCOP_ROOT/work/pip-cache python3.13 -m venv $W/venv
$W/venv/bin/pip install pefile capstone
L="reference/installed game/Starfleet Command Orion Pirates/OP API - R2.1/Scripts/Libraries"
LIBS=("$L/SharedR.lib" "$L/ScriptInterfaceR.lib" "$L/sfcTiger_MSVCR.lib" "$L/cryptlibR.lib" "$L/dmp3D_MSVCR.lib")
# exact byte matcher, unchanged (writes lib_matches.tsv into the current directory)
$W/venv/bin/python analysis/scripts/libmatch.py "reference/installed game/Starfleet Command Orion Pirates/StarFleetOP.exe" "${LIBS[@]}"
# fuzzy matcher, ~3 min per exe on one core (calibration included)
R=analysis/results
$W/venv/bin/python analysis/scripts/fuzzymatch.py --exe "reference/installed game/Starfleet Command Orion Pirates/StarFleetOP.exe" \
  --truth $R/StarFleetOP_2552_sdklib_matches.tsv --out $R/StarFleetOP_2552_sdklib_fuzzy.tsv \
  --data-out $R/StarFleetOP_2552_sdklib_fuzzy_data.tsv --calib $R/StarFleetOP_2552_sdklib_fuzzy_calibration.txt "${LIBS[@]}"
$W/venv/bin/python analysis/scripts/fuzzymatch.py --exe reference/SFCOPServer2552_B36/ServerPlatform.exe \
  --truth $R/ServerPlatform_sdklib_matches.tsv --out $R/ServerPlatform_sdklib_fuzzy.tsv \
  --data-out $R/ServerPlatform_sdklib_fuzzy_data.tsv --calib $R/ServerPlatform_sdklib_fuzzy_calibration.txt "${LIBS[@]}"
```

`--ghidra ghidra/exports/functions.csv` adds Ghidra's function starts when that file exists; it
did not exist for the run below. Progress and the summary go to stderr (saved as `*_sdklib_fuzzy.log`).

### Method

1. Every library COMDAT code section (deduplicated by name + bytes + relocations: 11,904 bodies) and
   every exe function candidate is disassembled with Capstone into per-instruction tokens.
   Stream A keeps mnemonic, registers, constants and intra-function jump distances, and masks
   relocated or address-like values (`A`), calls (`CALL`) and jumps that leave the function (`X`).
   Stream B also masks every constant, displacement and jump distance. Stream C keeps only mnemonics
   and operand kinds, so it survives register re-allocation. A switch or byte table addressed from
   inside a function truncates its stream. `__except_list` (fs:[0]) counts as a constant.
2. Exe function starts: call targets from a linear sweep, data pointers and `push`/`mov` immediates
   into `.text` that follow a `ret`/`jmp`/padding, 16-aligned code after `ret`+nop padding, and
   targets of jumps that leave their region (47.7k starts for 2.5.5.2).
3. Tier a (`exact`): A-stream equal at a start. Tier b (`masked`): B-stream equal, every jump
   inside the matched range. An address is taken by one body unless the bodies are A-identical
   (linker folding).
4. Context (`+ctx`): when one name fits several addresses, each candidate is scored by its
   relocation targets (callees, tail calls, globals, vtables, IAT imports seeded from the PE import
   table) that already have a known address: one disagreement removes the candidate, agreements add
   evidence, and placed callers vote for their callees. Vtables and other data objects whose address
   becomes known vote for their slots once at least one slot agrees and none disagrees. Nop padding
   matching the library's in-section padding adds a little evidence. The best candidate must
   strictly beat the runner-up. Rounds repeat until nothing changes.
5. Tier c (`fuzzy`): for named functions with at least 12 instructions that are still unplaced,
   C-stream 6-gram retrieval, then the difflib alignment ratio over the exe extent closest in
   length. Accepted at ratio ≥ 0.85, at least 0.05 above the runner-up, with a call count within
   max(1, 20%) and no known callee or global contradicting the alignment. Scores are capped at 0.8.
6. `ref`: names that only appear as relocation targets of placed functions (callees, vtable
   slots), with the address taken from the aligned exe instruction. A single caller is enough only
   when the name has no library body. A name that has a body needs two callers, or a body similarity
   of at least 0.5, because a template instantiation with identical code can stand in for the caller.
7. `multi` rows: names whose identical-structure candidates context could not separate; every
   candidate address is listed (groups up to 8), with score 1/k.

### Results (before → after)

`libmatch` counts above (4,277 with the whole library, and 3,906) are TSV rows: the same COMDAT comes from several libraries.
Distinct, they are **2,885 names at 2,389 addresses** (2.5.5.2) and **1,126 names at 794 addresses**
(ServerPlatform).

| `StarFleetOP.exe` 2.5.5.2 | names placed | new vs libmatch | new, excluding anonymous funclets/static initialisers | `.text` bytes |
|---|---|---|---|---|
| libmatch (before) | 2,885 | – | – | 586,417 (15.6%) |
| exact | 2,852 | 198 | 61 | 579,183 |
| exact+ctx (multi resolved by context) | 904 | 721 | 539 | 34,049 |
| masked | 32 | 32 | 21 | 5,751 |
| masked+ctx | 36 | 36 | 13 | 1,627 |
| exact-short (1–5 instructions, unique, no context; score 0.5) | 105 | 81 | 71 | 1,094 |
| fuzzy | 39 | 34 | 33 | 8,960 |
| ref (address from callers/vtables) | 196 | 188 | 188 | 21,425 |
| **after, all tiers** | **4,164** | **1,290** | **926** | **649,639 (17.3%)** |
| still ambiguous (`multi`) | 3,108 names | | | |

| `ServerPlatform.exe` | names placed | new vs libmatch | new, named | `.text` bytes |
|---|---|---|---|---|
| libmatch (before) | 1,126 | – | – | 188,826 (7.7%) |
| exact / exact+ctx | 1,264 / 460 | 189 / 454 | 39 / 296 | 185,156 / 29,733 |
| masked / masked+ctx | 36 / 17 | 32 / 17 | 16 / 8 | 2,119 / 1,151 |
| exact-short / fuzzy / ref | 42 / 35 / 83 | 30 / 31 / 69 | 19 / 31 / 69 | 293 / 7,394 / 11,770 |
| **after, all tiers** | **1,937** | **822** | **478** | **237,543 (9.7%)** |

The `ServerPlatform.exe` figures are for the 2.5.5.2 server kit (3.1 MB, linked 2004-01-20), run with the
whole 3D library, as are the `StarFleetOP.exe` figures above.
The table was rerun whole on that kit, so its "libmatch (before)" row is the kit's own libmatch
baseline, not the earlier kit's result. On this kit the blind calibration has 4 wrong placements
out of 1,115 (99.6%), all in the masked tiers, where the earlier kit had none: a masked or
masked+ctx server placement is a lead, not a name. Nothing in the repository consumes the server
placements.

"Anonymous" means `obj:.text$x#n` (EH unwind funclets) and `obj|_$E..` (static initialisers):
they are correctly placed, but their names only tell you which object they come from.
`*_sdklib_fuzzy_data.tsv` also names 1,551 (2.5.5.2, at 1,481 addresses) and 618 (server) data addresses: vtables,
RTTI, globals and string literals, taken from the same relocation evidence.

### Rows withheld from the committed 2.5.5.2 evidence

The two files the harness reads as evidence (`StarFleetOP_2552_sdklib_fuzzy.tsv` and `_fuzzy_data.tsv`)
are the script's output minus the 9 rows (8 and 1) listed in `StarFleetOP_2552_sdklib_withheld.tsv`
(columns `file` = `fuzzy` or `data`, `address`, `name`, `tier`, `score`, `reason`, `claimed_functions`).
- `reason` = `claim` (8 rows): the claimed source in `op/` still refers to that address by a
  placeholder symbol or another name, and a row there fails the claim's relocation check, which would
  lower the claim's score (`harness/README.md`). The row is not wrong; the claim has to be corrected
  first, then the row goes back. `claimed_functions` lists the claims that disagree. Every row at
  those addresses is dropped, whatever its name. The claims that held the other 82 rows now use the
  SDK's names, and those rows are back. The 8 left:
  - `??0tView3DExt@@QAE@XZ` (0x6a9c60) and `??_7tSWView3D@@6B@` (0x7b8f38), for the constructor
    0x69f3b0: the claim (7.66%) lacks the original's exception frame around the `tGLPixFmt` member,
    so its relocations fall at other offsets than the exe's.
  - `TraceErrorD3D`, `tD3DViewMgr::mGetDirectDraw`, `tGLPixFmt::operator==`, the `tDDSurface`
    constructor and `tDDSurface::mSetDDSurface` (for 0x6a8930), and `tD3DViewMgr::tD3DBatch::mRealFlush`
    (for 0x69e490): those claims reach them through stand-in classes, because `tD3DViewMgr` and
    `tGLPixFmt` are declared only inside their own units; they need a shared header first.
- `reason` = `wrong-placement` (1 row): `twDLLMgrBase::mDone` at 0x6b5c80, a fuzzy placement
  (score 0.708) that libmatch's exact match at 0x6b5ca0 contradicts.

After a rerun of the commands above, drop the rows of that file (match `file`, `address` and `name`;
for `claim` rows every row at the address) from the two outputs before committing them. The table
figures, the calibration files and `StarFleetOP_2552_sdklib_matches.tsv` below are from the complete run.

`StarFleetOP_2552_sdklib_matches.tsv` here is still the earlier, partial one: `tools/units.py` reads it
and the complete file regroups the 3D library's units under `op/`. The complete file is deliberately not
committed until that regrouping is done. It is exactly the `lib_matches.tsv` the `libmatch.py` command
under "Rerun" writes into the current directory (the 4,277 rows above); copy it over this file in the pull
request that runs `python3 tools/units.py`.

Symbol-table rows held back as leads: the seed also wanted to name 0x006738e0 (`tView3DExt::mEnableLuminanceTexturing`,
`exact+ctx` 0.883, 4 candidates, 16-byte body of 2 instructions) and 0x00673940 (`tSysMemSurf::mRestore`,
`exact+ctx` 0.962 against `tGDISurface::mRestore` and others at 0.797; one of 4 identical-structure
addresses). They keep the table's earlier rows, and the matches stay in `_fuzzy.tsv` as leads. The other
rows whose class changed (0x0069f160, 0x0069f170, 0x006a7b00, `tD3DViewSurfMgr` to `tGLSurfMgr`) are `exact`
with one candidate and are kept.

Names the old run placed and the new one does not (`D3DUtil_SetIdentityMatrix`, the `ByteQueue` scalar
deleting destructor and destructor, `tChannelAttributes::operator=`, `tMissionInfo::mSetDate`): no
current symbol row carries any of them. Only the `ByteQueue` scalar deleting destructor was a label
(0x0069ff20, `exact+ctx` 0.912, a wrong placement); the table row there is `tGLDC::scalar_deleting_destructor`.

### Calibration (`*_sdklib_fuzzy_calibration.txt`)

The pipeline never reads libmatch output. Its exact matches serve as ground truth afterwards.
"Long" = library body of at least 6 instructions. Shorter ground-truth entries are weak: libmatch
counts a 1-3 instruction function plus its nop padding as a unique 16-byte match, and the exe also
has unpadded twins that vtables point to.

| 2.5.5.2, blind full pipeline | precision | recall |
|---|---|---|
| all 2,885 ground-truth names | 2,872/2,874 = 99.93% | 99.5% |
| 2,435 long names | 2,423/2,425 = 99.92% | 99.5% |
| tier b alone (A equality ignored, no fuzzy), long | masked 100% (1,717), masked+ctx 99.1% (450/454) | 89.1% |
| ServerPlatform, all 1,126 | 1,111/1,115 = 99.6% | 98.7% |

Two placements disagree with libmatch. `tBasicFile::mDispose`: libmatch places it in the middle of
another function (0x62bf4f, not a function start). A placed caller points at 0x520990, where the
pipeline puts it, so the ground truth is probably the one in error. `twDLLMgrBase::mDone`: libmatch
matches the 144-byte body exactly at 0x6b5ca0; the fuzzy tier puts it at 0x6b5c80 (ratio 0.885,
score 0.708, below the 0.85 a label needs), so here the fuzzy placement is the one in error.

Tier c, leave-one-out on 1,500 ground-truth functions with at least 12 instructions, at the chosen
ratio 0.85 / margin 0.05:

| variant | recall | precision |
|---|---|---|
| present (true copy available) | 91.5% | 99.9% |
| absent (true address removed): false-positive rate | 3.1% | – |
| synthetic drift (8% tokens replaced, 4% deleted, 3% duplicated) | 54.8% | 99.9% |
| real drift: 91 functions whose address ≥ 2 callers fix but whose body changed | 4.4% | 67% (4 of 6) |
| fuzzy placements outside the ground truth, re-checked against the final name set | 18 of 34 checkable | 0 contradicted |

What this means: tier c is reliable when the function is still recognisable, but most functions
that changed between the June 2001 libraries and the 2003 exe changed completely. Most are STL
(`basic_string`, streams, `_Tree`), whose 2003 code differs, so they score 0.1–0.5 against the old
bodies. Those are recovered through callers (`ref`), not through similarity. The register-agnostic
C stream nearly doubles real-drift recall over the B stream (9.9% vs 5.6% at ratio 0.80; B-stream run in
`StarFleetOP_2552_sdklib_fuzzy_calibration_streamB.txt`, 25 fuzzy names instead of 39). The other ~7,000 SDK
functions are inlined, dropped, or still ambiguous. Fuzzy scores are capped at 0.8. Treat `fuzzy`,
`exact-short`, single-caller `ref` (score ≤ 0.8) and `multi` as leads, not facts.

### TSV columns (`*_sdklib_fuzzy.tsv`)

`address size mangled_name library tier score notes`. Size is the library body size, or the exe
extent for `fuzzy`/`ref`. Notes give the candidate count, context agreements, caller votes, token
counts, the fuzzy ratio and runner-up, and `libmatch=same|DIFFERENT` when libmatch had the name.
Names of the form `obj|symbol` are object-local (static) symbols.
