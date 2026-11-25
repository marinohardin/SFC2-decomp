# The build interface

What a program that builds on this decompilation may rely on. A native port pins this repository
as a git submodule and reads only what this file lists. Anything else may change without notice.

A change to anything below is made in two steps: first a pull request here that raises the format
version and says what changed, then the consumer's pull request that moves its pin and adapts.

## Format version

`2`, the only line of the file `INTERFACE_VERSION` at the root of this repository. A consumer
reads it and stops with an error on a version it does not know.

## Source

- **Source root:** `op/`, with every include path relative to it. `eaw/` is never part of this
  interface until this file says otherwise.
- **Units:** `op/UNITS.tsv` (one row per translation unit: unit, first and last address, function
  count, evidence, compile profile, header).
- **Compile profiles:** `harness/units.json`; a unit is compiled exactly as
  `harness/lib/source.py` `compile_unit` compiles it.
- **Guest mode:** `op/port/types.h` compiles `op/` source in a consumer's guest mode when
  `SFCOP_GUEST` is defined, through `op/port/guest.h`; under VC6 neither is seen.
- **The compiler:** the container `toolchain/` builds (`toolchain/build-image.sh`), with its
  version in `toolchain/VERSIONS.md`, run through `toolchain/bin/cl`, `toolchain/bin/link` and
  `toolchain/bin/msvc-run`.
- **Mission script DLLs:** `toolchain/build_scr.sh <srcdir> <out.scr>` builds one with the same
  container.
- **Harness library:** `harness/lib/paths.py`, `coff.py`, `funcs.py`, `target.py`, `matched.py`,
  `claims.py` and `source.py`, imported by a consumer's build to find, compile and read units as
  the harness does.

## Claims

- **`op/manifest.tsv`:** one row per claimed function of the Orion Pirates executable (module
  `STARFLEETOP`): `address` (hex, 8 digits), `size` (bytes), `symbol`, `source` (relative to
  `op/`), `modes`, `decorated`.
- **The `decorated` column** (last; the columns before it keep their positions and meanings): the
  decorated (mangled) symbol the claim compiles to, such as `??1?$tQuery@VtServCharacter@@@nDatabase@@UAE@XZ`,
  exactly the symbol the harness finds the claim's compiled function by. It is the `symbol` of
  the row of `analysis/symbols/STARFLEETOP.tsv` whose `symbol_evidence` is `claim` for the
  address, copied at the time `tools/manifest.py` runs (that table is not part of this
  interface). It is empty where the table has no such row: the claim has no recorded compiled
  symbol (the initializer, registration and destructor claims of a global object, say). A
  consumer takes the symbol of a claim from this column and prefers it to deriving one from a
  listing line or from `ghidra/exports/functions.csv`, which fails for some claims (an inline
  function whose `{` opens on its own line); it falls back to deriving only when the cell is
  empty.
- **Modes:**
  - `vc6`: the function is byte-exact with the original; a consumer may link its compiled object in
    place of the original's code;
  - `vc6 guest`: as `vc6`, and the consumer may also call it through its guest table;
  - `partial`: claimed and scored, not exact; a consumer keeps the original's code for it;
  - `stub`: compiled, never scored; a consumer keeps the original's code.
- **The guest table:** `python3 tools/manifest.py --only-override --override <path>` writes the
  consumer's C++ table of guest entry points from the same annotations. This repository commits no
  copy.

## Symbols and results

- `ghidra/exports/functions.csv`: every function of the executable, with address, size and name.
- `analysis/results/matched_set.json`: the best score of every claimed function, and whether it is
  exact.
- `analysis/results/StarFleetOP_2552_sdklib_*.tsv`: the SDK library matches.
- `analysis/results/retail_hashes.sha256`: the hashes of the retail files every tool checks.

## Install and progress tools

- `analysis/results/install_manifest.tsv`, written by `tools/install_manifest.py INSTALL`: one
  line per file of the game's run-time import set, with its size and SHA-256.
- `tools/install_scan.py DIR`: lists the visibly damaged files of an install; exits 1 when it
  finds one.
- `scripts/progress.py --lifter DIR`: counts code a consumer's lifter covers as lifted; without
  it nothing is lifted.

## Version history

- `2`: `op/manifest.tsv` gains a last column, `decorated`, the decorated symbol each claim compiles
  to (empty where the symbol table has none). No other column moved or changed. Version `1` had
  five columns.
