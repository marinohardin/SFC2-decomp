# analysis/

First-pass static analysis of the retail binaries (2026-11-02). Scripts need `pip install pefile`.

- `scripts/peinfo.py <exe> [full]` — PE headers, Rich header (compiler builds), imports.
- `scripts/libmatch.py <exe> <lib>...` — matches per-function COMDAT code from the SDK's MSVC 6 static libs
  (`OP API - R2.1/Scripts/Libraries/*.lib`) into the exe's .text byte-for-byte (relocations masked).
  Writes `lib_matches.tsv` = `address  size  mangled_name`.
- `scripts/funccount.py <exe>` — rough function-count heuristics (call targets, data->text pointers).

Results:
- `StarFleetOP_2552_sdklib_matches.tsv` — 3,758 unique name->address matches in the patched 2.5.5.2 game exe.
- `StarFleetOP_2500_sdklib_matches.tsv` — same against the original disc 2.5.0.0 exe (2,989 matches; different optimization).
- `ServerPlatform_sdklib_matches.tsv` — 3,100 matches in the Dynaverse server kit exe.
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
$W/venv/bin/python analysis/scripts/fuzzymatch.py --exe reference/SFCOPServer/ServerPlatform.exe \
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

`libmatch` counts above (3,758 and 3,100) are TSV rows: the same COMDAT comes from several libraries.
Distinct, they are **2,396 names at 1,934 addresses** (2.5.5.2) and **684 names at 318 addresses**
(ServerPlatform).

| `StarFleetOP.exe` 2.5.5.2 | names placed | new vs libmatch | new, excluding anonymous funclets/static initialisers | `.text` bytes |
|---|---|---|---|---|
| libmatch (before) | 2,396 | – | – | 471,870 (12.6%) |
| exact | 2,377 | 186 | 54 | 467,308 |
| exact+ctx (multi resolved by context) | 795 | 634 | 464 | 32,128 |
| masked | 23 | 23 | 12 | 1,239 |
| masked+ctx | 36 | 36 | 13 | 1,627 |
| exact-short (1–5 instructions, unique, no context; score 0.5) | 99 | 75 | 67 | 991 |
| fuzzy | 40 | 36 | 34 | 8,972 |
| ref (address from callers/vtables) | 185 | 179 | 179 | 17,810 |
| **after, all tiers** | **3,555** | **1,169** | **823** | **529,840 (14.1%)** |
| still ambiguous (`multi`) | 3,049 names | | | |

| `ServerPlatform.exe` | names placed | new vs libmatch | new, named | `.text` bytes |
|---|---|---|---|---|
| libmatch (before) | 684 | – | – | 60,633 (2.0%) |
| exact / exact+ctx | 716 / 348 | 145 / 272 | 36 / 176 | 55,667 / 19,026 |
| masked / masked+ctx | 25 / 20 | 25 / 20 | 8 / 5 | 1,759 / 578 |
| exact-short / fuzzy / ref | 42 / 43 / 68 | 42 / 31 / 66 | 40 / 31 / 66 | 130 / 11,035 / 8,928 |
| **after, all tiers** | **1,262** | **601** | **362** | **97,068 (3.1%)** |

"Anonymous" means `obj:.text$x#n` (EH unwind funclets) and `obj|_$E..` (static initialisers):
they are correctly placed, but their names only tell you which object they come from.
`*_sdklib_fuzzy_data.tsv` also names 1,164 (2.5.5.2) and 330 (server) data addresses: vtables,
RTTI, globals and string literals, taken from the same relocation evidence.

### Calibration (`*_sdklib_fuzzy_calibration.txt`)

The pipeline never reads libmatch output. Its exact matches serve as ground truth afterwards.
"Long" = library body of at least 6 instructions. Shorter ground-truth entries are weak: libmatch
counts a 1-3 instruction function plus its nop padding as a unique 16-byte match, and the exe also
has unpadded twins that vtables point to.

| 2.5.5.2, blind full pipeline | precision | recall |
|---|---|---|
| all 2,396 ground-truth names | 2,385/2,386 = 99.96% | 99.5% |
| 2,032 long names | 2,023/2,024 = 99.95% | 99.5% |
| tier b alone (A equality ignored, no fuzzy), long | masked 100% (1,361), masked+ctx 99.0% (412/416) | 87.3% |
| ServerPlatform, all 684 | 661/661 = 100% | 96.6% |

The single "wrong" placement is `tBasicFile::mDispose`. libmatch places it in the middle of another
function (0x62bf4f, not a function start). A placed caller points at 0x520990, where the
fuzzy matcher puts it, so the ground truth is probably the one in error.

Tier c, leave-one-out on 1,500 ground-truth functions with at least 12 instructions, at the chosen
ratio 0.85 / margin 0.05:

| variant | recall | precision |
|---|---|---|
| present (true copy available) | 88.8% | 99.9% |
| absent (true address removed): false-positive rate | 4.0% | – |
| synthetic drift (8% tokens replaced, 4% deleted, 3% duplicated) | 51.5% | 99.9% |
| real drift: 90 functions whose address ≥ 2 callers fix but whose body changed | 3.3% | 60% (3 of 5) |
| fuzzy placements outside the ground truth, re-checked against the final name set | 17 of 36 checkable | 0 contradicted |

What this means: tier c is reliable when the function is still recognisable, but most functions
that changed between the June 2001 libraries and the 2003 exe changed completely. Most are STL
(`basic_string`, streams, `_Tree`), whose 2003 code differs, so they score 0.1–0.5 against the old
bodies. Those are recovered through callers (`ref`), not through similarity. The register-agnostic
C stream doubles real-drift recall over the B stream (8.9% vs 4.5% at ratio 0.80; B-stream run in
`StarFleetOP_2552_sdklib_fuzzy_calibration_streamB.txt`, 26 fuzzy names instead of 40). The other ~6,000 SDK
functions are inlined, dropped, or still ambiguous. Fuzzy scores are capped at 0.8. Treat `fuzzy`,
`exact-short`, single-caller `ref` (score ≤ 0.8) and `multi` as leads, not facts.

### TSV columns (`*_sdklib_fuzzy.tsv`)

`address size mangled_name library tier score notes`. Size is the library body size, or the exe
extent for `fuzzy`/`ref`. Notes give the candidate count, context agreements, caller votes, token
counts, the fuzzy ratio and runner-up, and `libmatch=same|DIFFERENT` when libmatch had the name.
Names of the form `obj|symbol` are object-local (static) symbols.
