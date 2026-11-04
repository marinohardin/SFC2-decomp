# game/: the decompiled original

One file per original translation unit, at the original's path with the drive and project root
dropped (`docs/CONVENTIONS.md` §2 and §3). `UNITS.tsv` maps every function in
`ghidra/exports/functions.csv` to exactly one unit; `harness/units.json` gives each unit its
compile profile. Until a unit's first function is decompiled its file is a placeholder: a comment
block with the unit's address range and evidence, and no code.

## Layout

| Directory | Original | Evidence so far |
| --- | --- | --- |
| `Source/` | `SFC2_5\Source`: the game proper | assertion paths; the SDK's scripting and network objects |
| `Meta/` | `SFC2_5\Meta`: the Dynaverse client | assertion paths |
| `Common/` | Taldren's utility layer (`Common\`) | assertion paths; the SDK's shared library |
| `Switch/` | the `nSwitch` network layer (`Switch\`) | assertion paths |
| `Tiger/`, `dmp3d/` | the inherited platform and 3D libraries | assertion paths; the SDK's libraries |
| `unsorted/` | units whose original file is not known yet | class names, address pages |
| `port/` | not original: the headers the two compile modes need | |

## UNITS.tsv

One row per unit, in address order:

| Column | Meaning |
| --- | --- |
| `unit` | path under `game/` |
| `first`, `last` | address of the unit's first and last function |
| `functions` | number of functions (EH funclets and thunks included) in that range |
| `evidence` | `assert-path`, `sdk-member`, `class` or `page` (below) |
| `profile` | compile profile in `harness/units.json` |
| `header` | path of the unit's own header under `game/`, where one is evidenced |

A function belongs to the unit whose `first`..`last` range holds its address; ranges never
overlap, because the linker kept each unit's functions together.

- `assert-path`: an absolute source path in an assertion string the function uses. A relative
  path (`.\DataStore\...`, `..\Common\...`) is where an `#include` led, so it marks code inlined
  from another file and is not evidence of the unit.
- `sdk-member`: the function's bytes match an SDK library function whose object was compiled
  from one source file (the object's `.file` record). Inline and template functions, which every
  object that uses them emits, are not evidence. The SDK's script projects keep their copy of the
  game's shared source under `SCRIPTS\Shared`; it maps to the same directories as the game's.
- `class`: functions of one class that no unit above holds, in `unsorted/<Class>.cpp`
  (`Outer__Inner.cpp` for a nested or namespaced class).
- `page`: everything else, in `unsorted/fn_<page>.cpp` by 4 KB address page. Where a class
  unit splits a page into several runs, only the first run is `fn_<page>.cpp`; each later run is
  named by the address of its own first function, `fn_<first address>.cpp`, not by the page.

Functions between two anchors of a unit belong to it, and EH funclets right after its last
function stay with it. When the evidence would put one unit in two places, the unit keeps its
largest run of anchors and the others are dropped. A directory or file name the evidence spells
in more than one case is spelled as the strongest evidence spells it (the exe's own strings first,
then the SDK, then header names): `Common/Database/`, although one header path says `DataBase`.

## The unsorted/ rule

A function goes to `unsorted/` only when no evidence names its unit. It moves the moment
evidence appears; the matched set is keyed by address, so a move cannot lose a score
(`harness/SRC_LAYOUT.md`).

## Moving a function when evidence appears

1. Record the evidence where the generator reads it: the analysis that writes
   `ghidra/exports/functions.csv` (an assertion path, a header) or the SDK match results.
2. Regenerate (below) and read the diff of `UNITS.tsv`: the function's row should now be in its
   unit's range.
3. If the function was already decompiled, move its source, `FUNCTION:` line included, to the
   new unit in address order, and run `harness/build-and-verify.sh --all`. The emptied file is
   yours to delete; the generator never deletes or rewrites a file with content.

## Regenerating

```sh
python3 tools/units.py           # rewrite UNITS.tsv, the placeholders and the harness/units.json entries
python3 tools/units.py --check   # exit 1 if UNITS.tsv is not what the evidence gives
```

It reads `ghidra/exports/functions.csv`, `analysis/results/StarFleetOP_2552_sdklib_matches.tsv`
and the SDK's libraries and headers, beside the exe in the install (`--sdk` or `SFCOP_SDK` to
point elsewhere). It is idempotent. It writes a placeholder only where there is none or where the
file is still a placeholder, removes placeholders of units that left the map, and leaves every
file with content alone (it lists them). A file is a placeholder only when its text is exactly the
block the generator writes; any added line, a note under the marker included, makes it content.
It never writes `port/` or `Common/DynamicAllocator.*`, which other packages own.

In `harness/units.json` it adds an entry for each new unit and keeps every existing entry and
profile. It never removes an entry, by design, even when the unit has left the map: an entry may
hold a profile someone recorded for that unit, and the registry is the harness's, so a stale
entry is removed by hand in the pull request that moves the unit.
