# Source layout for Phase 2 (proposal)

A proposal for where rebuilt source lives, written for the harness (P1-5). Nothing here is
created yet: the plan phase decides, and `harness/` follows whatever it decides through
`harness/units.json` and `HARNESS_SRC_ROOT`.

## The tree

```
game/                         rebuilt game code, compiled by VC6 SP4 and scored by the harness
  <original path>/<Unit>.cpp  one file per original translation unit, at its original path
  <original path>/<Unit>.h    headers beside their units, as in the original tree
  unsorted/<Class>.cpp        units whose original file is not known yet (see below)
src/                          the platform layer (SDL3, audio, files, sockets); never scored
harness/units.json            compile profile of every unit in game/ (protected)
```

`PROPOSAL.md` (Phase 2) and the owner's porting conventions put decompiled game code in
`game/` and the platform layer in `src/`, so this proposal does the same; the brief for this
task called the tree `src/`. Only `game/` matters to the harness, and its location is one
setting (`HARNESS_SRC_ROOT`, default `game/`).

## One file per original translation unit

- **Path.** The original path, from the strongest evidence available: `__FILE__` strings and
  assertion text (`translation_unit` in `ghidra/exports/functions.csv`, 1,658 functions today),
  the `.file` and member names in the SDK libraries, then the class. The prefix that was the
  original developers' drive and project root is dropped, so a unit compiled as
  `C:\...\Taldren\Common\DynamicAllocator.cpp` lives at `game/Common/DynamicAllocator.cpp`
  (`docs/CONVENTIONS.md` §2 and §3 give the tree; `game/UNITS.tsv` the map).
- **Unknown origin.** A function whose unit is not known goes into `game/unsorted/<Class>.cpp`
  with the rest of its class (or `game/unsorted/fn_<page>.cpp` for free functions, grouped by
  4 KB address page, since the linker kept units contiguous). When evidence for its unit turns up,
  it moves; the matched set is keyed by address, so a move cannot lose a score.
- **Order.** Functions appear in the order of their original addresses. VC6 emitted a unit's
  functions in source order, so address order is the original source order.

## Claiming a function

The harness finds a function by an annotation on the line before its definition (the
convention `reccmp` uses):

```cpp
// FUNCTION: STARFLEETOP 0x006fc690
tDynamicAllocator* tDynamicAllocator::Instance()
{
```

- `FUNCTION:` claims a function for scoring. Exactly one per address in the whole tree.
- `STUB:` marks a placeholder (a declaration-compatible body so the unit compiles and, later,
  links) that is not scored yet.
- The compiled symbol is found from the compiler's own listing (`/FAs`): the function whose
  first source line follows the annotation, with no other annotation in between. The name in
  `functions.csv` is a fallback, not a requirement, so a function can be claimed before it has a
  name.
- Annotations stacked above a global object's definition (`tINIMgr g_tINIMgr;`) claim what VC6
  generates from it, counted upwards: the dynamic initializer (its line record is the
  definition), the function on the initializer's startup path that calls `_atexit`, and the
  destructor that function pushes. An annotation with nothing to claim there (a global with no
  destructor, or more than three) is an error.
- A scalar or vector deleting destructor (`??_G` / `??_E`) is claimed by an annotation stacked
  on its class's out-of-class destructor definition, directly above it or above the
  destructor's own annotation; a destructor written inline in the class gets an out-of-class
  definition for this. The address's evidence (a `??_G` / `??_E` symbol or a
  `scalar_deleting_destructor` / `vector_deleting_destructor` name in `functions.csv` or the
  matched set, read by `harness/lib/deleting.py` for the manifest, the harness and the object
  lift alike) says which annotation it is; the harness then claims the `??_G` / `??_E` that
  calls that destructor. If the unit does not emit it, the address cannot be claimed yet.
- Functions the compiler does not emit on their own (unused inlines, functions folded into
  their callers) cannot be claimed; they are covered by the functions they are inlined into.

## Stubs for what is not rebuilt yet

A unit only has to compile for the harness to score its claimed functions; nothing is linked.
So a unit holds the claimed functions plus whatever declarations they need. `STUB:` bodies are
for the later link step (the delink route or the native build), where every original function
must exist exactly once; a stub's body is the simplest one that compiles (`return 0;`,
`{}`), never a guess at behaviour.

## Compile profiles

`harness/units.json` maps each unit to a profile: compiler version, `/I` directories put first,
and the flags. The harness adds only output paths and `/FAs`, and drops `/Fr /FR /FD /YX /Gm`
and output options from a profile (they change no code bytes; see
`toolchain/calibration/report.md`). A `default_profile` applies to units without an entry.

The SDK's Release flags (`/MT /W3 /GR /GX /O2`, defines `WIN32 NDEBUG _WINDOWS`) reproduce the
SDK libraries byte for byte, and the exe's Rich header shows the same compiler build. They are
the `sdk-release` profile, for every unit at or above `0x0060f600`, where the linked libraries
start. The game's own objects, below that address, were built for size: `game-release` is the
same line with `/O1` for `/O2` (`toolchain/calibration/game_flags.md`). `tools/units.py` gives
each unit its profile from its address range. A unit that cannot match under its profile is
evidence of different flags, recorded per unit, never changed by an agent. Include directories
(`game/` itself, then vendored headers) are set when the first game units exist.

## Rules the harness enforces on this tree

- Function sessions may edit only files under `game/`, and never `.asm`/`.obj` output.
- `harness/units.json`, `analysis/results/matched_set.json` and everything else that judges are
  read-only to them (harness/hooks).
- `--all` re-verifies every recorded function, so moving a function between units, renaming
  it, or removing its annotation is caught as a regression.
