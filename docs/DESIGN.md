# SFC2-decomp — Design

What the decompilation does and how it behaves. This document defines behavior; `CONVENTIONS.md` defines form; `INTERFACE.md` defines what programs that build on this code may rely on.

Sections are numbered and stay numbered, because briefs and reviews cite them by number (`DESIGN §7.1`). The numbering is shared with the design of the native port, which builds on this repository; a gap in the numbers is a section that describes the port and lives there. Where a decision is not made, it is written in §12 rather than guessed.

---

## 1. Purpose

SFC2-decomp is a matching decompilation of Star Trek Starfleet Command II: Orion Pirates 2.5.5.2: the game's code recovered as readable C++, function by function, and verified byte for byte against the original executable with the original compiler (Visual C++ 6.0 SP4). It exists so that the game's code can be read, understood and built on, by a native port among others. The one hard constraint: the repository holds no retail file or asset and nothing derived from one byte for byte. A contributor supplies their own installed copy, which the tools verify by hash before they read anything.

## 2. What it does and does not do

It does:

- Recover the original's code into `op/`, one translation unit at a time, with each function claimed by its original address and scored against the original bytes.
- Recover names: the original's identifiers, taken from the executable and the modding SDK, used as-is.
- Publish what programs that build on it may rely on (`INTERFACE.md`): the source, the manifest of claimed functions and their modes, the matched set and the analysis exports.
- Measure itself: `scripts/progress.py` reports exact code and named functions; no change may lower a function's score.

It does not:

- Ship, download or require anything but the contributor's own retail install.
- Fix, change or extend the game: `op/` holds the original's code only (§7.1). Fixes and new features belong to the programs that build on it.
- Run the game. Behavior checks (a replay oracle, a native build) belong to the port.
- Target any build other than 2.5.5.2, until Empires at War starts as a second module (§7.5).

## 3. Constraints

| Constraint | Value |
| --- | --- |
| Languages | `op/` is C++ that compiles with VC6 SP4 for the byte check; tooling is Python 3, standard library only |
| Verification | A function is **exact** when its VC6 SP4 build matches the original bytes under objdiff. No function's score may fall, compared by original address |
| Data | Every original file the tools read is found in the contributor's install and verified against `analysis/results/retail_hashes.sha256` |
| Licensing | Undecided (§12.1). Until decided, nothing is included that this project could not license |

## 6. Interfaces

### 6.3 Function replacement

A decompiled function is claimed by the annotation on the line before its definition, `// FUNCTION: STARFLEETOP 0x006FC690`, exactly one per original address in the whole tree (`harness/SRC_LAYOUT.md`); `// STUB:` marks a body that compiles but is not scored. `tools/manifest.py` derives `op/manifest.tsv` from those annotations (address, size, symbol, source file, modes). Only a function `analysis/results/matched_set.json` records byte-exact has the mode `vc6`, which lets a consumer link its VC6 object in place of the original's code; one still being matched keeps its annotation and source in `op/` with the mode `partial`, and a consumer keeps the original's code for it. Exact and partial functions may share a unit: `/O1` gives each function its own section, and an exact function's call to a partial one resolves to the address its original bytes use. `analysis/results/matched_set.json` is written only by the harness. A function never appears twice in `op/`.

### 6.10 Progress and matched-set files

`analysis/results/progress/op/progress.json` and `PROGRESS.md` (written only by `scripts/progress.py`) and `analysis/results/matched_set.json` (written only by the harness) keep their shapes; a change to either shape is a change to `INTERFACE.md`. The gate in `scripts/check.sh` compares them against the base branch's copy.

## 7. Architecture

### 7.1 The source trees

```
op/          Orion Pirates' code as the original wrote it; op/port/ holds the type header
eaw/         Empires at War's code, once it starts (§7.5)
harness/     compiles a unit with the pinned compiler and scores each function with objdiff
toolchain/   the pinned compiler (VC6 SP4 in a container, or natively on macOS) and its calibration
ghidra/      the analysis exports every tool reads, and the scripts that make them
analysis/    the matched set, the SDK symbol matches, boundary errors, retail hashes, progress
tools/       manifest and unit-map generators, progress charts, the batch check
scripts/     setup, the definition of done, the progress report
tests/       the tooling's tests
```

`op/` carries no fix and no fix switch, and includes only `op/`. A port-motivated edit inside `op/` is tagged `PORT:` on its line with the reason, and there are as few as the consumers' platforms allow (64-bit and endianness hygiene, undefined behavior the original relied on). `scripts/check.sh` counts and lists the `PORT:` lines in `op/` and fails on any fix site there, so the footprint of programs that build on this code is always visible.

### 7.2 Two compile modes

`op/` is plain VC6 C++. The harness compiles each unit with VC6 SP4 in the toolchain container and scores the object against the original; that same object is what a consumer links in place of the original's code (decided 2026-11-04, §12.4). `op/port/types.h` spells the types `op/` source is written in so that they expand to the original's spelling under VC6; its guest mode (`SFCOP_GUEST`, with `op/port/guest.h`) is used only by the port's guest build and is never seen by the byte check.

### 7.5 Empires at War, a later second module

Orion Pirates expands Star Trek Starfleet Command II: Empires at War (retail exe `StarFleet2.exe`, 2000). Decompiling it is a later project in this repository, as a second module beside `STARFLEETOP`. Until it starts, Orion Pirates 2.5.5.2 is the only target: no build, test, fixture or CI step uses the Empires at War exe, and no annotation names another module (`tools/manifest.py` refuses one).

A comparison of the two exes (2026-11-08; both carry the same compiler build in their Rich headers) measured Orion Pirates' game code, library code excluded, by bytes: 31% identical in Empires at War apart from addresses, 4% identical apart from structure offsets, 17% with small edits, 48% with no counterpart. The overlap is concentrated:

| Area | Identical | Edited or offsets only | No counterpart |
| --- | --- | --- | --- |
| `Tiger/` | 81% | 10% | 9% |
| `dmp3d/` | 42% | 29% | 29% |
| The game proper (`Source/`, most of `unsorted/`) | 27% | 25% | 49% |
| `Common/`, `Switch/`, `Meta/`, `Source/MetaClient/` | 0 to 3% | 0 to 8% | 90 to 99% |

The identical figure is a lower bound: the comparison found Empires at War's function starts by heuristic. Both games' assertion paths agree with the split: the two libraries came from a project of their own in both builds, and each game's code from its own project folder.

What follows now:

- The grind decompiles shared code first. Because `op/` holds the original's code and nothing else (§7.1), every function it matches in `Tiger/`, `dmp3d/` and the shared game code is work already done for the second module.
- Code outside `op/` names decompiled functions by original address (the manifest, a consumer's tables), never by a path in `op/`. A pull request that moves files in `op/` updates every `#include` of them in the same pull request.

What happens when the second module starts (§12.12): `Tiger/` and `dmp3d/` move to a tree shared by both modules, with one annotation per binary on each function they share (the harness then reads a stack of annotations above a function, and the manifest keys functions by module and address); each game's own code gets a tree of its own, because Orion Pirates' project is a fork of Empires at War's with about half its code changed; a function that changed between the two games has one copy per game.

## 12. Open questions

| # | Question | Who decides | Blocks | Status |
| --- | --- | --- | --- | --- |
| 12.1 | The licence of this project's own code, and therefore whether a `LICENSE` file exists | Owner | Nothing yet | Open |
| 12.4 | Whether a consumer compiles `op/` a second time in a guest mode, or links the VC6 object the harness scored (§7.2) | Owner | Consumers' builds | **Decided 2026-11-04:** consumers link the VC6 object; `op/` carries no guest-mode constructs, and `op/port/guest.h` serves only the port's guest build |
| 12.11 | Whether the decompilation and the port live in one repository or two | Owner | Nothing | **Decided 2026-11-16:** two repositories: this one (the decompilation, public) and a private port that pins it as a submodule |
| 12.12 | Empires at War as a second module (§7.5): which build is the target (the 2000 retail exe or a later official patch), and when `Tiger/` and `dmp3d/` move to the shared tree | Owner, when that work starts | The second module; nothing before it | Open; Orion Pirates is the only target until then |
