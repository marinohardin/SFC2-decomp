# harness: the compiler is the judge

Lets a headless Claude Code loop rebuild the game one function at a time, with the pinned VC6
SP4 compiler and objdiff deciding whether it worked. One script judges; hooks keep the agent away
from everything that judges; scores only go up.

| File | What it is |
| --- | --- |
| `build-and-verify.sh <fn>` | The only judge. Compiles the function's unit, diffs it against the original bytes, refuses any score below `matched_set.json`. Read-only |
| `update-matched-set.sh <fn>` | The only writer of the matched set a lane scores against. Takes no score: it verifies the function itself and records an improvement |
| `landing.sh [base]` | The landing step: records every claim's score in the committed `matched_set.json`, then regenerates `op/manifest.tsv` |
| `../analysis/results/matched_set.json` | Best verified score per original function, keyed by original address. Develop's copy; a lane works on its own (see "Lanes commit source only") |
| `../analysis/results/reloc_baseline.tsv` | Known error-class relocations of exact functions that name another symbol than the claim at their target (step 8 of `build-and-verify.sh`) |
| `units.json` | Compile profile per unit of the rebuilt tree (flags, compiler, include order) |
| `queue.py` | Builds and serves the work queue; claims functions; logs attempts; keeps the lane's `difficult_functions.tsv` and `revisit_functions.tsv` |
| `$SFCOP_WORK/harness/lists/<lane>/difficult_functions.tsv` | Functions that had every attempt the stop rule gives without becoming exact, for a human. Outside the repository |
| `$SFCOP_WORK/harness/lists/<lane>/revisit_functions.tsv` | Functions the stop rule ended short of 100% before their full chance, for a later revisit pass (`--revisit`). Outside the repository |
| `$SFCOP_WORK/harness/lists/<lane>/nearmiss_labels.tsv` | The near-miss label of each non-exact function of the lane (`lib/nearmiss.py`). Outside the repository |
| `loop.sh` | The headless loop: one fresh `claude -p` per function |
| `prompts/function.md` | The prompt each session gets |
| `hooks/` | `PreToolUse` guard and `Stop` verifier, plus `settings.fragment.json` to merge |
| `SRC_LAYOUT.md` | Proposed layout of the rebuilt source (`op/`, one file per original unit) |
| `proof/` | The round-trip proofs and their transcripts (`proof/results/`) |
| `lib/`, `tests/` | The Python behind all of the above (stdlib only), and its tests |

Requirements: the `sfcop-msvc6` image (`toolchain/build-image.sh`), objdiff-cli under
`../tools/objdiff/` (`toolchain/calibrate.sh` fetches it), python3, the retail exe under the
main checkout's `reference/` (its hash is checked against `analysis/results/retail_hashes.sha256`
on every run; a missing hash line or another exe stops the run, also with `HARNESS_EXE`, unless
`HARNESS_EXE_UNVERIFIED=1`, which prints a `WARN` line on every run). Generated files go to
`$SFCOP_WORK/harness/` (default `../work/harness/`), never into the repository: per checkout,
`wt-<checkout>-<hash>/` holds objects, listings, targets, the queue and the claim; `shared/`
holds what all loops on the machine share, the attempt log and the claim locks.

The committed matched set is in `analysis/results/`, not here, and the revisit and difficult lists
are not committed at all (see "Lanes commit source only"): `harness/` is a hotspot.

## build-and-verify.sh

```sh
harness/build-and-verify.sh 0x006fc690             # or 006fc690, FUN_006fc690, a name, a mangled name
harness/build-and-verify.sh 0x006fc690 --json      # the same, as JSON (the Stop hook and loop read this)
harness/build-and-verify.sh 0x006fc690 --behavioural
harness/build-and-verify.sh --all                  # re-verify every function in matched_set.json
harness/build-and-verify.sh --all --base origin/develop   # and hold it to the base branch's copy
harness/build-and-verify.sh --all -j 2             # build 2 units at once (default HARNESS_JOBS, else 4)
```

Units build side by side, four at a time by default (`-j N` or `HARNESS_JOBS`). Each build is a
single-threaded compiler in its own container followed by one objdiff per function, so `-j` is
roughly the number of cores a run uses. The output does not depend on it: each unit reports into
its own buffer, and the buffers are printed in unit order. Two lines move: the exe check's
warning comes before the first unit, and an untrusted exe stops the run before any unit builds.
An interrupted run (Ctrl-C) stops the compiles it started, in every worker, and their containers. Each compile runs in its own process group, so a harness stopped by another signal (`kill`, a CI timeout) leaves its running compiles to finish on their own, each within the compile timeout; send it Ctrl-C (SIGINT) instead to stop them.

1. Finds the one source file under the rebuilt tree (`HARNESS_SRC_ROOT`, default `op/`) with
   `// FUNCTION: STARFLEETOP 0x006fc690` on the line before the function (`SRC_LAYOUT.md`).
   That file is the unit, unless it is a header (`.h`, `.inl`): a claim the original wrote in a
   header (an inline destructor's deleting destructor, a part binding in a class body) is
   compiled in the unit `op/UNITS.tsv` assigns to its address, which must include the header,
   directly or through other headers; anything else is an error (`lib/claims.py`, shared with
   `tools/manifest.py`).
2. Compiles that unit with the unit's profile from `units.json`, adding only output paths and
   `/FAs`. The compiler is `toolchain/bin/cl-native` when `toolchain/fetch-native.sh` has
   installed it, else `toolchain/bin/cl` (`HARNESS_COMPILER=docker` or `native` forces one; see
   `toolchain/README.md`, "Native compiler"); `paths.HARNESS_CL` holds the choice. A consumer
   calling `source.compile_unit` without `cl=` gets the image, the compiler of record (the
   scheduled Docker audit checks it). Which compiled function the annotation claims is found by
   symbol first (see "How a claim is found"); the listing's line records tell it only for a
   claim the symbol table has no current symbol for, and for a header claim they are the
   header's line records in the unit's listing. An inline
   destructor's deleting destructor, whose listing may have no line records, is the `??_G` /
   `??_E` whose mangled class is the class named on the definition.
3. Takes the function's original bytes from the exe at the address and size in
   `ghidra/exports/functions.csv` (extended to the rebuilt size when the analysis stopped short,
   never into the next function; trailing `int3`/`nop` padding dropped).
4. Writes a one-section COFF target object around those bytes whose relocations mirror the
   rebuilt object's (`lib/target.py`). Each relocation's target address in the exe is computed
   from the exe bytes and checked:
   - **ok**: the symbol's address is known (`functions.csv`, the SDK name matches in
     `analysis/results/`, a label inside the function, `__except_list`) and the exe uses it;
   - **unverified**: nothing is known about the symbol, and the function uses one address for it
     consistently, not one known under another name;
   - **mismatch**: a wrong callee or global, an inconsistent use, a field past the end, or a
     floating-point constant (`__real@`) or string literal (`??_C@`) whose bytes in the rebuilt
     object differ from the bytes at the address the exe uses (a wrong value, or a stand-in
     string). A source file name (`__FILE__`) is accepted where the exe holds the same name under
     the original build folder.

   ok and unverified fields are masked so objdiff sees them equal; a mismatch keeps the exe bytes
   and points at a symbol named after the exe address, so objdiff counts it. Identical opcodes
   calling the wrong function therefore do not pass.
5. Runs `objdiff-cli diff` for the score, and a strict byte comparison of the masked bytes.
   **Exact** means objdiff 100% and the strict check equal.
6. Re-scores every other recorded function in the same unit (a header or shared edit can break
   them) and compares all against `matched_set.json`.
7. Refuses a symbol that the exact functions of the run use at two exe addresses (an `ERROR`
   line naming both, exit 2): the native build links one definition per name, so one of them
   names the wrong callee, global or method. Labels, compiler statics and literal constants are
   resolved per unit and compared within one. `--all` checks every recorded function this way.
8. Checks every REL32 / DIR32 relocation of an exact function that reaches a claimed function
   (an address in `matched_set.json`, or one scored in the run) and names another symbol than
   that claim defines (`report_claim_symbols` in `lib/verify.py`). The byte match masks the
   field, so such a call scores exact; whether it matters depends on how a consumer linking the
   exact objects (manifest mode `vc6`) resolves the name, in this order: (1) a definition in
   another input object, by name; (2) the exe's symbol evidence (`functions.csv`, the SDK
   tables), by name; (3) the address the exact caller's original bytes hold for that field;
   (4) otherwise it stops. Each such relocation is of one class (`classify_break`), decided from
   the objects the run built and that evidence:

   | Class | Meaning | Result | What a session does |
   | --- | --- | --- | --- |
   | `other-object` | the caller's symbol is defined in another built object: the consumer binds the call to that body | `ERROR`, exit 2 | declare the callee as its claim does |
   | `other-address` | the evidence names the caller's symbol at another address | `ERROR`, exit 2 | same |
   | `wrong-function` | the evidence names the callee's address, and the caller's symbol is not among those names: the caller names another real function | `ERROR`, exit 2 | same |
   | `folded` | the evidence gives the target several names, the caller's symbol one of them (bodies the original folded into one address): resolved by name | `WARN`, one summary line, never fails | nothing needed |
   | `stand-in` | the symbol is defined in no other object and the evidence does not place it elsewhere (an invented class or signature): resolved by name or by address | `WARN`, one summary line, never fails | nothing needed; declaring the callee as its claim does removes it |
   | `own-object` | the symbol is defined in the caller's own object (an inline or template instance emitted there) | `WARN`, one summary line, never fails | nothing needed; **unresolved**: whether a consumer binds the call to that local body or to the exe's address is for the consumer's maintainer to say |

   An error is a line naming the function, the relocation offset, the symbol, the exe address,
   the claimed symbol and the reason. The claim's symbol is the one the run's rebuild of its
   unit defines; a matched-set `symbol` field is only a fallback, because it is rewritten only
   when a score improves and can be stale after a rename. An error against a recorded symbol
   only (a single-function run that did not build the target's unit) is a `WARN` line, and
   `--all`, which rebuilds every claim, decides. A single-function run knows only the objects it
   built, so it can miss a definition in another unit: `--all` is the judge. Targets nothing
   claims (library and CRT code) are not checked. A legitimate case, such as two identical
   functions the linker folded into one address, is listed in the baseline.

   **The relocation baseline** (`analysis/results/reloc_baseline.tsv`, `HARNESS_RELOC_BASELINE`):
   the errors known when the check came in, one row each (`function`, `offset`, `symbol`,
   `target`, `claimed`), matched by function, symbol and target (the offset is for the reader).
   A listed error is tolerated, with one `INFO` line counting them; a row whose break is gone,
   or under `--all` has become a warning class, is a `WARN` line asking for its removal (under
   `--all` every row, otherwise only the rows of the exact functions scored), so the list only
   shrinks. Warnings are never listed in it. Every `--all` run writes every break it found, with
   a sixth column naming its class, to `reloc_breaks.tsv` in its build folder (the reader of the
   baseline ignores that column); the baseline is only ever replaced from such a file by the
   integrator, never extended to let a new error through.

Output is a few lines, each tagged `OK`, `ERROR` or `INFO`, then a `RESULT` line; for a function
that is not exact, the first differing instructions side by side and the path of a full
side-by-side listing. A failed compile says `BUILD HAS FAILED` with the compiler's errors.
Exit status: 0 nothing regressed, 1 a recorded function scored lower (or lost its source, or its
unit no longer builds), 2 the claimed function could not be built or scored, or step 7 or 8
found a symbol that the exact functions use wrongly.

**One kind of definition per symbol** (`--all` only, `report_comdat_kinds` in `lib/verify.py`). After
every unit is built, a symbol that two built objects define as COMDATs with different selection
kinds is an `ERROR` line naming the symbol, the kinds and the units, and fails the run (exit 2); so
is a symbol defined outside COMDATs in two objects, and so is a symbol that two objects both define
as a "no duplicates" COMDAT (the same kind twice is still two definitions: two different functions
given one decorated name, typically one class name written on both). VC6 emits an out-of-line definition as "no
duplicates" and an inline copy as "any", so a function defined out of line in one unit and inline
in another (typically behind a macro the claiming unit sets) cannot be linked: a linker, and the
consumer's lifter, refuse the pair. A session gives the function one definition, the same in every
unit (inline in the header, claimed there), and never an out-of-line variant behind a macro.

**Units that read a changed file.** A run for some functions (not `--all`) builds their units,
plus every other unit with a recorded function that reads a file changed since the checkout's
HEAD (modified, added, deleted or untracked under `op/`): the unit itself, a file it includes
directly or not, or a path its include search probes; a unit whose includes cannot be followed
(`#include MACRO`) is always taken. An `INFO` line names them. Their recorded functions are held
to their scores like any other, so a header edit that breaks another unit is a regression of
the run, and the Stop hook and `update-matched-set.sh` refuse it. In the loop HEAD is the round's
starting commit, so this covers everything the session changed.

`--base <file or git ref>` also holds the run to another copy of the matched set, compared by
original address: a file, or `analysis/results/matched_set.json` at a git ref (read with
`git show`). Every entry in the base must still be in this tree's matched set at the same score
or higher (and still exact if it was), or it is a regression; every function is then verified
against the higher of the two scores. Without it, a pull request that lowered or deleted an
entry together with the source that earned it would pass, since `--all` only checks the copy in
its own tree.

`--behavioural` is a hook for an external replay oracle, such as the port's; none lives in this repository. It is off: it prints
`behavioural: oracle not available` until `HARNESS_ORACLE` names a command that takes a function
address and exits 0 when the replay trace matches; a non-zero exit then counts as a regression.

### Caches

A second run does only the work that changed (`lib/cache.py`):

- **Incremental build.** A unit is rebuilt only when its build key changes: the compile command,
  the compiler (the native wrapper, wibo and the MSVC tree, or the image's id), the MSVC
  environment, the folder it compiles in, and the content of every file it includes, found by
  following every `#include` line whatever `#if` surrounds it, searched as MSVC searches (a
  quoted include beside its file, then beside each file above it, then the `/I` folders), and
  every path searched before the one found, so a header that appears earlier on the search
  rebuilds the unit; also the environment the compiler reads (`CL`, `_CL_`, `INCLUDE`, `LIB`). A unit with an `#include` naming no file (a macro) is always rebuilt. The key is kept in
  the unit's build folder, `build.key`, and a reused build prints "unchanged, not rebuilt".
- **Score cache.** A function is rescored only when its score key changes: its object (COFF
  timestamp masked), the symbol scored, its address and size, the judge's own code (every module of `lib/`) and objdiff,
  and the inputs (the exe, the function index, the SDK symbol tables). Entries are
  content-addressed under `$HARNESS_SHARED/cache/scores/`, shared by every checkout; only a
  completed objdiff comparison is stored.

`HARNESS_NO_CACHE=1` turns both off. CI's `check` runs that way, so no cached build or score
stands in a pull request's check. Measured on 2026-11-19 over all 158 units and 4,228 claimed
functions: an uncached, a cold and a warm `--all` printed the same lines; the warm run took
115 s against 228 s cold. The caches are safe to delete at any time.

### Build folders

Each checkout keeps its state in `$SFCOP_WORK/harness/wt-<name>-<hash>/`, and the build output of
its runs in `build/` there. A function's folder in `build/` holds `target.obj`, `diff.txt` (the
rows a session is shown) and `result.json`; objdiff's own report is read once and deleted, so a
full `--all` leaves about 115 MB, not 4 GB. Nothing removes `build/` when the git worktree is
removed. The harness writes the worktree's absolute path to `worktree.txt` in the state folder
whenever it builds or builds the queue, and

    python3 -m harness.lib.clean [--dry-run] [--unrecorded]

deletes `build/` of every state folder whose recorded worktree no longer exists, and prints each
folder and the bytes freed. It never touches anything else in the folder (`attempts/`,
`queue.json`, `cache/` stay), and leaves a folder with no `worktree.txt` alone unless
`--unrecorded` is given. Run it after removing a worktree and before launching lanes; start with
`--dry-run`. Nothing runs it by itself.

## matched_set.json and update-matched-set.sh

`{"version": 1, "functions": {"0x006fc690": {name, symbol, unit, size, score, exact, recorded}}}`.
Keyed by original address, so renaming or moving a function cannot hide a regression.
`update-matched-set.sh` runs the same verification with `--record` and stores a result only when
it improves on the recorded one (higher score, or newly exact) and nothing in the unit
regressed. It accepts no score from its caller. The hooks deny it, and `--record`, to agents.

**Released functions** (`analysis/results/released.tsv`: `address`, `date`, `reason`, one row per
function, sorted by address). The base gate (`--base`, `scripts/check.sh`, and the progress gate in
`scripts/progress.py`) refuses a function that lost its recorded score, its exact status or its
claim. An address on this list is not held: the run prints one `INFO` line per listed address it
used, with the date and reason. It releases whatever the gate would call worse for that address,
a lowered score included. A listed address that is still exact prints a stale-row warning, so the
list cannot rot; remove the row then. A malformed row is an error. Adding a row needs the owner's
approval and a reason a reader can check (for a function given up so that objects link, say); it
is not a way to pass a lane or to hide a regression, and a lane never adds one.

## Hooks (`hooks/`)

`hooks/settings.fragment.json` holds two hooks for the integrator to merge into
`.claude/settings.json`. `loop.sh` also passes it to each session with `--settings`. Both hooks
do nothing unless `HARNESS_FUNCTION_SESSION=1`, which only `loop.sh` sets, so interactive
sessions are unaffected.

- **PreToolUse** (`hooks/guard.py`, logic in `lib/guard.py`): Edit/Write only to source under the
  rebuilt tree, never `.asm`/`.obj`; everything else is denied, in particular `harness/` (the
  verifier, `matched_set.json`, `units.json` with the flags, the prompt and these hooks),
  `toolchain/`, `analysis/` (`retail_hashes.sha256` and the name evidence), `ghidra/exports/`,
  `reference/`, `tools/`, `.claude/`, `$SFCOP_WORK/harness/` (queue, objects, listings). Bash
  is limited to `harness/build-and-verify.sh` (without `--record`; a look-alike path elsewhere
  is refused), read-only tools and read-only git; no redirection to files, no command
  substitution, no environment overrides, no `sed -i`, no API key. A hook error denies.
  An edit may not remove or change the claim line (`FUNCTION:`, `STUB:`, `VTABLE:`) of any
  function but the session's own, in any file; the loop also compares every other claim line in
  the tree before and after the session and records a change as `tampered`.
- **Stop** (`hooks/stop_verify.py`, logic in `lib/stophook.py`): re-runs
  `build-and-verify.sh` for the function in `$HARNESS_STATE/claim.json` and blocks completion
  unless it improved on the score it had when the session started, with nothing regressed and
  no stand-in (below). After three refusals (`HARNESS_MAX_STOP_BLOCKS`) it lets the session end
  and the loop records a failed attempt.

### Lanes commit source only

A grind lane (a `grind/` branch the loop runs on) commits source and nothing else, so lanes never
conflict on the matched set, the manifest or the lists when they land. The one generated file a
lane does commit is the symbol table (`analysis/symbols/STARFLEETOP.tsv`), together with the
source in the same commit; only the tool edits it, and lanes can conflict on it (see "The landing
step" for what to do) ("Symbol table and stand-ins", "What the loop does to the
table").

- **Matched set.** The loop exports `HARNESS_MATCHED_SET=$HARNESS_STATE/matched_set.json`, the
  lane's own copy: seeded from `analysis/results/matched_set.json` and topped up from it at every
  start (an entry the committed set has higher is put back, never the reverse). Every reader goes
  through `lib/paths.py`, so the queue, the verifier, the hooks and the whole-tree step see the
  lane's scores, and `update-matched-set.sh` writes them there. A round that is undone restores
  the lane's set to its state when the round began (`git reset` cannot: it is outside the
  worktree).
- **Lists.** `queue.py` keeps the revisit and difficult lists in `$HARNESS_LISTS/<lane>/`
  (default `$SFCOP_WORK/harness/lists/`), the lane being the state folder's name (`HARNESS_LANE`
  overrides). A loop writes only its own lane's two files; the queue reads the union of every
  lane's, so memory carries across lanes with no write races and no merge. A revisit row for a
  function another lane listed difficult is ignored. Nothing is committed and nothing is listed
  in a commit. Clearing is per lane: a function that ends exact comes off this lane's revisit
  file only; a row in another lane's file (or in `seed/`) stays, and the loop says so
  (`listed by lane X, left there`), since the queue skips an exact function anyway. A lane's list
  files have one writer, so `HARNESS_STATE` (and with it the lane name) must not be shared
  between worktrees or clones. To start from the lists as they were committed, run once:
  `python3 harness/queue.py seed-lists origin/develop` (it copies the two files into
  `lists/seed/` and never overwrites one). Once develop no longer has the files, each is read
  from the last commit reachable from the ref that still had it (the parent of the commit that
  deleted it), and the output names that commit.
- **Commits.** `feat(op): match|improve <name> (<address>)`, the score change in the body, and a
  trailer: `Harness-Score: STARFLEETOP 0x00401234 exact`, or the percent for an improvement.
  `tools/unlanded.py` reads the trailers in `base..branch`: a lane's exact functions are those
  its commits name plus those its committed matched set has exact (older branches). A function
  has landed once the base's matched set has it exact.
- **Checks on a lane.** A lane is recognised, in this order, by `SFCOP_LANE=1` (the loop exports it), a
  `GITHUB_HEAD_REF` that starts with `grind/` (CI checks out a detached merge commit), or a
  `grind/` branch name (`lib/lane.py`; `scripts/check.sh` and the manifest test both ask it). There the only check
  that changes is `test_committed_manifest_matches_the_tree`, which is skipped: the manifest is
  stale until landing. The tests, the claims lint, the progress gate and the matched-set gate
  run as on any branch, against develop's committed files, which a lane does not change.
  `tools/manifest.py` and `tools/lint_claims.py` read `HARNESS_MATCHED_SET` when it is set, so
  the loop's whole-tree step judges against the lane's scores. On `develop`, an integration
  branch and any pull request that is not a lane, nothing is skipped.

### The landing step

Run once on the integration branch, after the lanes are merged into it:

```sh
harness/landing.sh origin/develop     # rebuilds every claim, records its score, syncs the symbol table, regenerates the manifest
git add analysis/results/matched_set.json analysis/symbols/STARFLEETOP.tsv op/manifest.tsv
git commit -m "chore(op): land <batch>"
python3 -m pytest tests harness/tests && sh scripts/check.sh
```

`landing.sh` is `verify.py --land --base <ref>` (every function claimed in `op/`, rebuilt,
recorded when its score rose or it became exact, with its name, symbol, unit and size refreshed
from the tree), then `tools/symbols.py claim` (every claimed row takes the symbol the build just
made), the seed in its keep-existing mode (`analysis/scripts/seed_symbols.py`, no `--reseed`:
empty rows are filled from exact callers that agree, evidence `callers`, existing values are
untouched) and `tools/symbols.py check`, so an integration branch ends with the table in sync,
then `tools/manifest.py`.

On a merge conflict in `analysis/symbols/STARFLEETOP.tsv`, take either side for the file and run
`harness/landing.sh`: it rewrites every claim row and refills callee rows from the build. The one
case to look at by hand is a row two lanes filled with different `add` symbols: that is a real
disagreement (the seed report lists the callers of each). A function that scores below the base's record,
or lost its source, is reported as a `REGRESSION` and the script exits 1 without writing the
matched set; exit 2 means a function could not be built. The agents' guard refuses `--land`.

### Whole-tree checks

Once the function itself is judged good, the Stop hook runs five checks on the whole tree
(`lib/wholetree.py`), and `loop.sh` runs them again before it commits the round, so the session
that causes a problem is told while it still has the context:

1. `build-and-verify.sh --all --json`: a regression or an `ERROR` in any unit, the relocation
   and claim-symbol steps included. Warm, unchanged units are neither recompiled nor rescored.
2. `tools/manifest.py`, written to a scratch file in the harness state: a claim on a non-function,
   a class declared twice or a declaration it cannot read. The loop never regenerates
   `op/manifest.tsv`; the landing step does.
3. `tools/lint_claims.py`: a new finding. A stale baseline row is not the session's doing and
   does not fail.
4. `pytest tests/test_units.py tests/test_manifest.py`, without
   `test_committed_manifest_matches_the_tree`, which the loop cannot satisfy because it never
   regenerates the manifest.
5. `tools/symbols.py claim` (every claim) then `check`, on a copy of the symbol table in the
   harness state (`python3 -m harness.lib.symsync --scratch`): the tree is left alone, and the
   table is held to the claims as they stand.

A failure reaches the session as a refusal to stop: the tools' own lines (at most 15 each), then
one sentence saying what to do. It counts against `HARNESS_MAX_STOP_BLOCKS` like any refusal;
when they are used up the session is let go, the loop's own run of the same checks fails, and
the round is undone and recorded as `failed`, as for a regression. `HARNESS_WHOLETREE=0` turns
the checks off, in the hook and in the loop (default 1; for emergencies).

The checks are not compared with a baseline: a tree that already fails would refuse every
session. So `loop.sh` runs them once at start, after its own `--all` (that run is reused, not
repeated), and stops before any session if they fail; the lane must start from a green tree. A
non-zero exit of that startup `--all` used to be a warning and now stops the loop; the detail is in
`$HARNESS_STATE/symbols.log`. The whole step shares one time budget, `HARNESS_WHOLETREE_BUDGET`
(600 s, inside the Stop hook's 900 s): each command gets what remains, and a command that runs
out of it is a failure that names it, the ones after it reported as not run. With a cold build
cache the `--all` alone is a full build (189 s here), so run the loop's start once before
sessions.

### Lane file list

`HARNESS_LANE_FILES`, when set to a file, narrows what the guard allows under the source tree
to the files that list names. The lane launcher sets it; `loop.sh` does not. One
repository-relative path or glob per line (`fnmatch`: `*` also matches `/`; for example
`op/Tiger/tsay.cpp`, `op/Tiger/tsay*.h`); blank lines and lines starting with `#` are ignored. An
edit or new file under `op/` that matches no line is denied, with a message that tells the
session to leave the file alone and end its final message with `HEADER REQUEST: <header> <what
and why> (for 0x<address>)` or `UNIT REQUEST: <file> <what and why> (for 0x<address>)`. A list that
is set but cannot be read denies every edit. A session's Bash cannot write files at all
(no redirect, `sed -i`, `cp` or `mv` passes the guard), so the list covers Edit and Write only. Unset, the guard allows any edit under `op/`, as
before.

### Symbol table and stand-ins

Every `build-and-verify.sh --all` writes `<build>/symbols.tsv`: each claim's address, the symbol
its unit compiled for it and the claiming file (`lib/symtab.py`). With `functions.csv`'s mangled
names and single SDK labels it answers, for an original address, which symbol a call to it must
name; `python3 -m harness.lib.symtab [<address>...]` prints the rows with the calling convention.
The matched set's `symbol` field is not used for this: it is written only when a score rises, so
a later rename leaves it stale (446 of 4,228 entries on 2026-11-19).

A **stand-in** is a relocation in a scored function, exact or not, that reaches a claimed
function through another symbol than the one the claim compiles to (this run's rebuild, or the
table): the callee declared again under another class or signature. It matches bytes, but a
consumer linking the objects may not resolve it. A single-function run prints a `WARN stand-in`
line for each and lists them under `standins` in `--json`. The Stop hook refuses a session whose
function holds one, and the loop does not commit it (exit 5 from `loopio.score`). Rows on the
relocation baseline (`analysis/results/reloc_baseline.tsv`) are tolerated, as for exact
functions; so is a warning-class break of a function that is exact, which step 8 only counts,
except an exact function's `stand-in` break that the last `--all` in this checkout (the loop's
start; its `reloc_breaks.tsv`) did not already have: the session created it, and it is refused. Without
that file such a break is only counted (a later symbol-table change tightens it). Without a table (no `--all` yet in the checkout) only claims rebuilt in the same run
are checked. The table is as old as the `--all` that wrote it, so `loop.sh` writes it again at
every start (a full `--all`: about 2 minutes warm, 4 cold with the native compiler; start lanes
one after another); after merging into a running lane, restart its loop (or run `--all`) so a renamed
claim is not refused as a stand-in. A claim that an `--all` could not build keeps its row from the table
it replaces. Known cost: a session refused against a table older than a merge cannot fix the
refusal itself, and the attempt is lost until the loop is restarted. The context pack lists a claimed callee's symbol as what the
call must name; a symbol from `functions.csv` or the SDK is given as a lead only.

### How a claim is found

A `// FUNCTION:` annotation claims one compiled function, and the harness has to say which. It
asks the symbol table first (`lib/verify.py`, `claim_symbol`): when the claimed address's row has
a symbol with evidence `claim` and the unit's object defines that symbol, that function is the
claim; no line record is read, so moved lines, several functions on one line, or an inline
function opening on a bare `{` cannot attach the claim to a neighbour. Every other claim is found
by line, as before: the function whose first line record follows the annotation
(`source.symbol_for_claim`). That covers a new claim (its row is empty or not a `claim` row), a
symbol the unit no longer produces (the session renamed or re-signed the function), and the forms
the table has no row for: the compiler-generated functions of a global (`_$E<n>` is per unit and
never a row) and a symbol two claims compile to. Each claim found by line prints
`INFO ... found by line (table has no current symbol)`, and a run prints how many claims were
found by symbol and by line. A deleting destructor and a header inline copy are found by symbol
like any other claim once they have a row.

### What the loop does to the table

After a session's function improves, and before the commit, the loop runs `lib/symsync.py` (never
the model; the guard keeps the table and the tools out of a session's reach):

1. `tools/symbols.py claim --results <build-and-verify --json output>`: the claims of the units
   the session's run rebuilt take the symbols they compile to now (evidence `claim`).
2. For each unclaimed function the session's function calls, whose row has no symbol: the symbol
   its relocation names is written with evidence `add` (`tools/symbols.py add --callee`), but
   only when it passes the seed's rule for callers (`tools/symbols.callee_verdict`: a real name,
   or exactly `fn_<the target's own address>`, free or a member of a class that is not a
   placeholder). A symbol that fails the rule is not written and gives one `WARN` line. A result file without
the function skips that round's sync with a `WARN`; the next sync or the landing catches up.

The loop then commits the table with the source. A round that is undone restores it with the
rest of the worktree. The whole-tree step runs `claim` for every claim and then `check` on a copy
of the table, and `landing.sh` does both on the real one. A row with `mangled` or `add` evidence
is first-writer-wins: a session that calls the function through another symbol is refused
(`report_table_breaks`, same break classes as for a claim: `other-object`, `other-address` and
`wrong-function` fail the run with the table's symbol in the message; a `stand-in` goes to the
Stop hook; `folded` and `own-object` warn). A `callers` symbol is provisional and only warns. The
rule holds for the session's own function only: under `--all` the breaks are counted in one
`WARN` line and written to `<build>/table_breaks.tsv`, never failed, and nothing is added to
`analysis/results/reloc_baseline.tsv`.

### An `other-object` error on a standard-library instance claimed under a stand-in name

Some claims are standard-library template instances the original compiled and a past session
claimed under a stand-in name. When a session's function hits an `other-object` error on one,
it does not rename or declare the library function again, and it does not touch the claim. It
leaves the call as the context pack shows it, writes a `RENAME:` or `HEADER REQUEST:` note in
its handoff (what the call names, the address, the symbol the table has) and stops: settling
those claims is integrator work.

## The queue (`queue.py`)

`harness/queue.py build` reads `functions.csv` and keeps real functions (not EH funclets or
thunks), skipping library-flagged ones, those already exact, and those in
`difficult_functions.tsv`. Order: functions named from evidence with a typed prototype first;
then power-of-two size buckets, smallest first, with functions over 4 KB (about 1,000
instructions) last; inside a bucket, most similar to an already exact function first (mnemonic
3-gram Jaccard with capstone, byte 4-grams without); then size and address. Today: 26,399
functions, 4,805 named with a prototype.

`queue.py next` claims the function it returns with a lock file in
`$SFCOP_WORK/harness/shared/claims/<address>.lock` (created exclusively; it names the checkout
and the loop's process), so loops in several worktrees never work on the same function. The loop
releases it after the round and on exit; a lock whose process is gone is taken over. Attempt-log
rows are appended under `flock`, and the caps count every loop's attempts.

`queue.py select --unit U --address A` restricts this checkout's loop (see "The loop");
`next` and `show` (and nothing else) honour the selection only when `HARNESS_SELECTION` names
its file, which `loop.sh` sets, so a selection left from an earlier run restricts nothing.

### Model tiers and the stop rule

A function's run, by its counted attempts. The first model depends on the function's size in
the original: Sonnet builds a function under 128 bytes (`HARNESS_SMALL_BYTES`), where it lands
about as often as Opus for half the cost; Opus builds anything larger, and a function of unknown
size.

| Attempt | Under 128 bytes | 128 bytes or more | Role |
| --- | --- | --- | --- |
| 1 | Sonnet | Opus | Build: write the function from the original instructions |
| 2 | Opus | Opus | Fix round: close the differences left in the committed near match |
| 3 | Opus | Opus | Build again, with what the earlier attempts left |
| after the run | Fable | Fable | One time-boxed try, only for a best of at least 90%, only with `--allow-fable` |

Attempt 2 is always a fix round, whatever its model; a later attempt is a fix round only when
its model is Sonnet or Fable.

The stop rule (`plan` in `lib/workqueue.py`) usually ends a run sooner:

1. From attempt 2 on, an attempt that does not raise the function's best score ends the run.
   Attempt 1 is exempt, so a failed first build still gets its fix round.
2. After two attempts the run also ends unless the best is above 90% (`HARNESS_NEAR`).
3. When the run is over and the best is at least 90%, Fable gets `HARNESS_FABLE_TRIES` tries
   (default 1) if `--allow-fable` (or `HARNESS_ALLOW_FABLE=1`) is given, capped at
   `HARNESS_FABLE_MAX_TURNS` turns (30) and `HARNESS_FABLE_TIMEOUT` seconds (1800). A session
   that hits the time box is stopped and its work is scored like any other; an improvement is
   kept. Unless it left an exact match, the attempt is logged and noted as `timeout`, and its
   tokens, cost and turns are left empty in the attempt log: they are unknown, because a
   stopped session writes no result. Any other session that ends without a result is logged
   the same way.

The best score is the highest of every attempt's result and starting score, so a function
that another branch had already raised is judged against that. Usage-limit stops (`limit`),
login failures and dry runs never count; failed, no-change, improved, exact, tampered and
timeout attempts do. Improved attempts are committed, so a later attempt starts from the best source so
far. `HARNESS_TIERS` (default `opus,opus,opus`) and `HARNESS_SMALL_TIERS` (default
`sonnet,opus,opus`) replace the two sequences; neither may name Fable. When only `HARNESS_TIERS`
is set, it is used for every size. `HARNESS_SMALL_BYTES=0` turns the size split off. A function
the queue no longer holds, such as one already on the difficult list, has no known size and is
judged with the regular sequence. A near miss labelled `reference-only` is the one exception:
it never goes to Opus or Fable ("Near-miss labels", `HARNESS_NEARMISS`).

When a run ends short of 100%, `queue.py record` lists the function in the lane's list folder
(outside the repository; nothing is committed):

- **`difficult_functions.tsv`** when it had its full chance: every model of
  the regular sequence was tried and, if its best qualified for Fable, Fable was tried too. The
  queue never takes it again.
- **`revisit_functions.tsv`** otherwise: the stop rule ended it early, or it is
  waiting for a Fable try that was not allowed. Columns: `address`, `name`, `best_score`,
  `attempts`, `last_model`, `listed` (the time of its last attempt) and `note` (why the run
  ended). The queue skips these functions unless `--revisit` (or `HARNESS_TAKE_REVISIT=1`) is
  given. A revisit run starts over and counts only the attempts after `listed`, except for a
  function that was waiting for Fable, which gets its Fable try instead. A revisit that ends
  exact takes the function off the list. A function gets one revisit run: one that ends short
  again moves it to `difficult_functions.tsv`, unless it is left waiting for a Fable try that
  was not allowed, which keeps its row. Listing it again would hand it straight back to the
  same `--revisit` loop.

The queue reads the lists of every lane, so a function one lane listed is skipped by the others.
A list entry only arrives with a lane's loop, however, and the attempt log is shared, so
`--revisit` also takes a function
that is on neither list here but whose last run in the shared log ended short of its full
chance and is not exact: it gets a new run, cut from the log alone (a run ends where the rule
gives no further model, with Fable counted as allowed, so a Fable try continues the run it was
waiting at). A function waiting for Fable stays waiting without `--allow-fable`, and one that
used its full chance is left alone. Without `--revisit` the queue skips all of them.

`queue.py sweep` settles the functions in the attempt log that are on neither list but whose
run the rule has already ended: those attempted before the rule existed. The loop does not run
it; run it once, on a grind branch, after the loops that used the old tiers have stopped; it
writes this lane's lists. It reads matched_set.json too: a function recorded exact is never
listed, and is taken off either list, because a function matched outside the loop has no exact
attempt in the log.

### Deleting destructors

A scalar or vector deleting destructor (`??_G` / `??_E`, about 30 bytes) is generated by the
compiler from its class's real destructor; it has no source of its own, so a session given its
address alone could not finish: the real destructor is another address, and a claim line of
another function is what the claim snapshot, the guard and the Stop hook refuse. The loop now
treats the two as one piece of work (`lib/pairs.py`).

- **Pairing.** The queue (`queue.py next`) classifies functions from their symbols, read in the
  order of the symbol table ("Symbol table and stand-ins"): `??1Cls@@` is the real destructor,
  `??_GCls@@` / `??_ECls@@` (or the older `scalar_deleting_destructor` member form) a deleting
  destructor of `Cls`. A deleting destructor's real destructor is the class's `??1`, checked
  against the first `call` in the original's first 16 bytes (the install is read only for this,
  and only when a deleting destructor is reached). The class and the call must agree; one
  alone is enough when the other is silent. A real destructor's partners are the class's
  deleting destructors whose bytes do not call another function.
- **One session.** Whichever member of the group the queue reaches first is the lead; its entry
  carries `pair` (role, partners, and each partner's state: `exact`, `claimed` or `open`), the
  model follows the largest open function of the group, and the other open members wait behind
  it, so no second session is handed the same pair. `acquire_group` takes the lock of the lead
  and of every partner, or none; `queue.py release` frees them together.
- **Scope.** The claim file lists the `partners`. The guard, the claim snapshot after the session
  and the Stop hook count the pair's addresses as the session's own: it may add, move or
  re-stack their annotations. The Stop hook judges the lead as before and reports each partner
  claimed in the source (`partner 0x...: 62.50%`), and a stand-in in a partner blocks the stop
  too. A round is kept when any member of the group improved (a partner higher than the score
  recorded when the session started, or newly exact) and the queued function did not get worse;
  the Stop hook and `loopio judge` (used by `loop.sh`) decide it the same way. A lead that is
  still not exact stays in the queue for its next tier. `loop.sh` falls back to the queued
  address alone, with a warning, if it cannot read the group. After the session `update-matched-set.sh` records the lead and every partner the source
  claims, and the commit carries a `Harness-Score:` trailer for each.
- **Credit.** A lead that ends exact is logged as exact; each partner the committed round left
  exact gets an `exact` row of its own, so the queue never hands it out. A partner left short
  of exact stays open and leads its own session once the lead is done (the deleting destructor
  is then already exact). When the lead's run ends on the revisit or difficult list, its
  partners go on the same list with the same row (`(with 0x..., its pair)` in the note).
- **No identifiable destructor.** A deleting destructor with no real destructor (no `??1` of its
  class, no call to a function, or a class and a call that disagree, or a call to a named
  function that is not a destructor) is not given to a session: it goes to this lane's
  `difficult_functions.tsv` with attempts 0 and the reason, as a reference-only near miss does
  (the row is added under the file's lock, so two loops add it once). When the only gap is that
  the original's bytes could not be read (the install link is missing, the image will not
  parse), the address is not listed: `queue.py next` skips it for that call and says so on
  stderr (`skipped for this call, not listed`).
- **The prompt** says how a deleting destructor is written here: never by hand, never as a
  `scalar_deleting_destructor` member; its annotation is stacked on the real destructor's
  out-of-class definition above the destructor's own, as `tThreadEvent` and `tWinTimer` do, and
  the real destructor gets an honest body (a claim never sits on a stub or placeholder). The
  pair's partners and their states follow the function's facts.

`HARNESS_PAIRS=0` turns pairing off: every address is handed out alone, as before.

### Boundary errors

`analysis/results/boundary_errors.tsv` lists addresses the analysis records as functions that are
only pieces of one: a fragment of a static initializer, the tail of a neighbouring function, or a
compiler stub with no source line. Each row gives the address, its name in the analysis, the
function it belongs to (`part_of`, when known), the kind and a note. A piece of a function has no
source of its own, and an empty body written only so the harness can score the range is not the
original's code, so `op/` holds no definition for a listed address. The rows whose note begins
"Body kept in op/" are the exception for now: removing their empty bodies changes which compiled
function a neighbouring annotation claims, so they stay until those neighbours are claimed another
way.

The queue never takes a listed address. The matched-set gate lets a pull request drop the entry of
a listed address that was never exact and that no `// FUNCTION:` annotation in `op/` still
claims, with an `INFO` line saying so; an exact entry can never be dropped this way. A row comes
off the list when the analysis is corrected and the whole function is claimed at its real start.

The gate reads the list from the pull request itself, so the pull request that adds a row is where
the row is justified: its description says why the address is a piece and not a function, and what
`part_of` and `kind` rest on (the bytes at the address, the function that jumps or falls into it).
Review checks each new row against that, since the gate cannot tell a real boundary error from a
convenient one.

The exception prolog that VC6 puts at the start of a function with an exception frame (`mov eax,
<table>; call __EH_prolog`, 10 bytes) is not listed: `ghidra/scripts/join_eh_prologs.py` joins
each such prolog to the body that follows it in `functions.csv`, so the function starts at its
real entry point.

When a pull request joins functions this way, the gate does not hold a never-exact entry to its
score in the base when the address is a joined entry (its row is tagged `eh-prolog-joined`) or the
body 10 bytes after one, and its size in the pull request's `functions.csv` differs from the
base's or it is no longer a function: that score was for the prolog or the body alone. It says so
with an `INFO` line. Any other change of size is held as before, and an exact entry is always
held.

An entry whose name says deleting destructor (`scalar_deleting_destructor` /
`vector_deleting_destructor`) but whose recorded symbol is a plain destructor (`??1`) was scored
against the wrong function: the destructor body, not the deleting destructor at the address. The
gate lets a pull request drop such an entry when it was never exact and no `// FUNCTION:`
annotation in `op/` still claims it, with an `INFO` line saying so (`matched.release_misscored`).
This happens when a class's destructor becomes inline in its header, as the original wrote it, and
the unit the address belongs to no longer emits anything to score.

## The loop (`loop.sh`)

```sh
harness/loop.sh --dry-run        # everything except calling claude: no commit, no reset
harness/loop.sh --once           # one function
harness/loop.sh --max 20
harness/loop.sh --unit Tiger/tsay.cpp            # one grind batch: one unit, in address order
harness/loop.sh --once --address 0x006d2800      # a chosen function; both flags repeat
harness/loop.sh --unit Tiger/tsay.cpp --brief ~/notes/tsay.md    # with a batch brief
harness/loop.sh --allow-fable    # Fable's one try for near matches (best at least 90%)
harness/loop.sh --revisit        # also take functions on revisit_functions.tsv
harness/loop.sh --lean ~/grind/lean-instructions.md   # see "Lean sessions"; or --no-lean
```

`--brief FILE` appends a batch brief to every session's prompt, after a `---` line: working
notes for one batch, such as what the unit's classes do, that are never committed. The file
must lie outside the worktree and must not be empty; a relative path is taken from the
directory the loop was started in. The loop copies it into `$HARNESS_STATE/brief.md` at the start,
where a session cannot change it, and sets `HARNESS_BRIEF` to that copy; each attempt's
`.prompt.md` shows exactly what the session was given. If the prompt cannot be built (the copy
has become unreadable, say), the loop stops before starting the session.

`--unit` takes a unit path from `op/UNITS.tsv` and `--address` a function address; each may
be given more than once. Before the first round the loop runs `queue.py select`, which resolves
them against the queue and writes the open functions they cover, in address order, to
`$HARNESS_STATE/selection.json`; `queue.py next` then takes only those, and the loop stops with
`OK    nothing left to take in the selection` when each is exact, difficult, at its attempt
cap, or held by another loop. A selection with no open function at the start is an error. The selection is resolved once, from
the unit map as it was when the loop started, and lives in the harness state, which a session
cannot edit. An unknown unit or a malformed address (anything but up to eight hex digits,
with or without `0x`) stops the loop before any round. Without
these flags the loop takes the whole queue in its own order.

Each round: `queue.py next` claims the function and picks the model; `build-and-verify.sh` gives its
starting score; the claim goes to `$HARNESS_STATE/claim.json`; the prompt is rendered from
`prompts/function.md`, with the context pack and handoff notes appended; `claude -p --model
<tier> --output-format json --settings hooks/settings.fragment.json --permission-mode acceptEdits
--strict-mcp-config` (plus the lean-session flags) runs with `HARNESS_FUNCTION_SESSION=1`, under
the time box when it has one;
then `update-matched-set.sh` judges. An improvement is committed on the current branch, source
only (`feat(op): match <name> (<address>)`, the score change in the body, a `Harness-Score:`
trailer; "Lanes commit source only"). Anything else is
undone: the worktree is reset to the commit the round started from. Tokens, cost, turns and seconds of every attempt go to
`$HARNESS_STATE/attempts.tsv`, and the prompt, pack, command line, output and verification JSON
of each attempt to `$HARNESS_STATE/attempts/`.

Before the first round it refuses to start if `ANTHROPIC_API_KEY`, `ANTHROPIC_AUTH_TOKEN` or a
Bedrock or Vertex switch is set, if `claude auth status` does not show a logged-in subscription,
if the branch is `main` or `develop`, or if the worktree has any uncommitted change, tracked or
untracked (it resets the worktree after a failed attempt).

The hooks are the first line; the loop does not rely on them. After each session, before any
scoring, it checks `git status --porcelain --untracked-files=all` outside the source tree and
that `HEAD` has not moved. If anything changed, the attempt is not scored: it is recorded as
`tampered` (counted against the cap) and the worktree is reset to the round's starting commit.
Otherwise it removes ignored files under `harness/`, `toolchain/`, `analysis/`, `ghidra/`,
`scripts/` and `oracle/` (a planted `__pycache__` cannot run) and scores with the committed
verifier. A login failure mid-run stops the loop. A usage-limit stop
sleeps `HARNESS_LIMIT_SLEEP` (300 s) and retries the same function. Run it in its own worktree on
a `grind/` or `feature/` branch, and from a workspace Claude Code already trusts (an untrusted
workspace ignores the project's permission rules; the hooks and `--allowedTools` still apply).

### Lean sessions

A function session needs a page of rules, not the repository's whole `CLAUDE.md` and
`AGENTS.md`. `--lean FILE` (or `HARNESS_LEAN_INSTRUCTIONS=FILE`) gives every session that file
instead: the loop runs `claude` with `--setting-sources user`, so neither the project's nor the
local settings are read and no `CLAUDE.md`, `CLAUDE.local.md` or `AGENTS.md` is loaded, and with
`--append-system-prompt` holding the file's text. The hooks still apply: they come from
`--settings hooks/settings.fragment.json`, which is loaded whatever the setting sources, and the
tool list from `--allowedTools`. Every session also runs with `--strict-mcp-config` and no
`--mcp-config`, so no MCP server starts.

The project settings' permission rules do not apply in a lean session either. What they denied
is covered by the guard hook (`lib/guard.py`), which the fragment runs on `Read`, `Grep` and
`Glob` as well as on edits and `Bash`: private files (`work/private/`, the Wine
prefix, the test key data, any `.reg` or `.env` file outside the repository) are never read, by
any tool or command. The Bash side refuses on sight rather than parsing: any command that
contains `private`, `testkey`, `wine/prefix`, `.reg` or `.env` in any letter case, uses `$` or a
leading `~`, or has a glob or brace pattern that could expand to one of those names or reaches
outside the repository. Over-refusal is deliberate (it costs a session one turn). A second layer
does not depend on the hook: the fragment's `permissions.deny` holds `Read(//**/private/**)`-style
rules (Read rules also cover Grep and Glob) and `Bash(*private*)`-style rules, and settings
passed with `--settings` apply under `--setting-sources user` (checked on the CLI); edits outside the rebuilt source tree, `reference/` included, are denied;
and `Bash` runs only the verifier and read-only commands, so no `git push`, `gh` or `git config`.

The lean file is working material that is never committed: it must lie outside this worktree
(checked as for `--brief`), and the loop copies it into `$HARNESS_STATE/lean.md` when it starts.
The loop refuses to start without one unless `--no-lean` (or `HARNESS_NO_LEAN=1`) says to give
sessions the repository's instructions as before; the loop proof uses that. Each attempt's
`.command.txt` shows the command line the session got, with the lean file's path in place of
its text.

### Context pack

Before each session the loop builds a pack for the function (`lib/pack.py`; the attempt's
`.pack.md`) and appends it to the prompt after a `---` line:

1. Ghidra's decompiled pseudo-C, read from `$SFCOP_WORK/ghidra/decomp/<address>.c`
   (`HARNESS_DECOMP` overrides the directory). `ghidra/scripts/export_decomp.sh` writes that
   cache headless from the project, read-only, for every non-library function (26,399), or for
   an address list given as its argument; it skips what is already there.
2. The direct callees (`call rel32` targets that are function starts), with their prototypes
   from `functions.csv`, the file under `op/` that already holds one, and the symbol a call to
   it must name with its calling convention (the symbol table above).
3. The declarations of the classes it touches, its own first, from the headers under `op/`;
   classes declared in no header are named so the session searches before declaring one.
4. One function already exact in `matched_set.json` from the same class, or else the same unit,
   nearest by address, as it stands in the source.

For a `reference-only` near miss the pack opens with its wrong references ("Near-miss labels").
Each section is capped and the whole pack is cut at `HARNESS_PACK_MAX` characters (12000), at
a line end with a note of what was cut. A missing input leaves a one-line note in its section,
never a failed round; `HARNESS_PACK=0` leaves the pack out.

### Handoff notes

After every scored attempt that does not end exact, the loop writes a note
(`lib/notes.py`) to `$SFCOP_WORK/harness/shared/notes/<address>.md`: the attempt's model,
outcome and scores, the remaining differences from the verifier (the function's score line
and up to eight differing instructions or relocation errors), and the session's own account.
The prompt asks a session that stops short of 100% to end its last message with a `HANDOFF:`
section (what it tried, what remains, what it suspects); without one, the note keeps the end of
its last message. The next attempt's prompt carries the newest two notes, at most 3,000
characters, after the pack. Notes live only in the harness state, never in the repository.

### Near-miss labels

A near miss is a claimed function that is not exact. `lib/nearmiss.py` gives each one a label,
with no model, from its own comparison in the build folder of the last run (`result.json`,
`diff.txt` and the unit's object): it rebuilds the masked comparison the scorer made
(`target.build`, from the object and the original's bytes) and reads the differing rows of the
side-by-side listing. It never compiles, never edits source and never writes into the
repository.

```sh
python3 -m harness.lib.nearmiss                    # every non-exact claim of the last --all
python3 -m harness.lib.nearmiss 0x005fb3af         # one function (after a run that scored it)
python3 -m harness.lib.nearmiss --unit Tiger/tsay.cpp
python3 -m harness.lib.nearmiss --tsv              # or --json, with the references
python3 -m harness.lib.nearmiss --store            # also store the labels (below)
python3 -m harness.lib.nearmiss --parked           # stored reference-only functions that wait
```

One label per function, the first of these that holds (the order is the precedence):

| Label | Holds when |
| --- | --- |
| `reference-only` | The rebuilt and original bytes have the same length and differ only inside fields of relocations the scorer marked mismatch, and every differing row of the listing holds a relocation field: a call, vtable or data reference reaches another address than the original's. Also when no byte differs and nothing mismatches but objdiff still counts a row holding a relocation |
| `order` | No relocation mismatches, and the differing rows hold the same instructions on each side (the same multiset of instruction text), in another order |
| `stack` | No relocation mismatches, every differing row has an instruction on both sides, and each row is equal once esp/ebp displacements and the frame size (`sub`/`add esp, N`) are masked, with registers masked as well where a row needs it; at least one row needs the stack mask. A function whose slots and registers both differ is `stack` |
| `registers` | No relocation mismatches, every differing row has an instruction on both sides, and each row is equal once register names are masked |
| `structural` | Anything else: instructions missing or extra, other operands or sizes, a wrong reference together with a body difference, a function with no comparison to read. A function the rules do not fit cleanly is `structural`, never a cheaper label |

Each record also has the score and the number of differing rows. For a `reference-only`
function it lists each wrong reference: the relocation's offset, the symbol the source names,
the address the original reaches, and what the symbol table (`analysis/symbols/STARFLEETOP.tsv`)
has there (name, class, symbol and declaration; or that the row has only a name; or that there
is no row), the claim's file and symbol when the address is claimed, a vtable's class from
`ghidra/exports/vtables.csv`, and the scorer's reason. A `reference-only` function is parked,
with a reason that starts with one word, when no session can fix it yet:

- `unnamed`: a target has no symbol in the table (a level 1 row, or no row): it waits for the
  callee to be named;
- `evidence`: the source already names the symbol the table gives each target, and the scorer's
  evidence places that symbol at another address: for the integrator;
- `objdiff`: the strict check passes every reference and objdiff still counts a row: no source
  change can help.

**Stored labels.** `--store` writes the records to `nearmiss_labels.tsv` in the lane's list
folder (`$HARNESS_LISTS/<lane>/`, beside the revisit and difficult lists; `HARNESS_NEARMISS_LABELS`
overrides): `address`, `name`, `unit`, `label`, `score`, `differing`, `wrong_refs`, `waits` and
`labelled`. A run over every function replaces the file; a run for some addresses replaces their
rows, and an address that is exact now loses its row (with `--unit`, every address the unit
scored). Only this lane's file is read. The loop
labels every near miss after its startup `--all`, labels the round's function after its starting
score (copied to the attempt's `.nearmiss.tsv`) and again after an improvement is committed.

**What the loop does with it** (`route` in `lib/workqueue.py`, called from `next_entry`; on by
default, `HARNESS_NEARMISS=0` turns it off): a function whose stored label is `reference-only`
is never escalated. An Opus attempt goes to Sonnet instead, and its context pack opens with the
list of wrong references (within `HARNESS_PACK_MAX`); it gets no Fable try; and a parked one is
not taken at all. A parked function is relabelled only by the loop's startup `--all` (or by
running the command by hand), never by a round, since it gets none: it stays parked until the
next loop start after its callee is named. A reference-only function whose run reaches the Fable
step is put on the lane's `difficult_functions.tsv`, with a reason saying it is a reference-only
near miss with its regular attempts used, so it is not skipped without a trace. Nothing else about the tiers, the stop
rule or the attempt caps changes: the attempts it does get are counted as usual.

**Rules from fixes.** The prompt asks a session that takes its function to 100% from a lower
starting score to end with one line, `RULE: <what the diff showed> -> <the source change that
fixed it> (for 0x<address>)`, in source terms. `python3 -m harness.lib.nearmiss --rules` (or
`--rules --tsv`) collects every such line from the attempt transcripts (`*.claude.json`) of
every state folder under `$SFCOP_WORK/harness/`, with the attempt and the function's label when
that attempt started, when the attempt has a `.nearmiss.tsv`. It only reads; a line without the
arrow or the `(for 0x...)` part is ignored.

## Proof

Both transcripts are in `proof/results/`; the source used is the SDK's, copied into
`$SFCOP_WORK/harness/` and never into the repository.

**Round trip and refused regression** (`proof/proof.sh`, about 40 s,
[`proof/results/proof.txt`](proof/results/proof.txt)). The SDK's `DynamicAllocator.cpp` is the
source root's only unit, compiled with the SDK's Release flags; `tDynamicAllocator::Instance`
(0x006fc690) is claimed with one annotation.

1. `build-and-verify.sh 0x006fc690`: **100.00% exact** (112 bytes; 10 relocations: `operator new`
   and the constructor checked against their known addresses, `__except_list` 4 times, 4
   unverified: the EH state label and the singleton pointer, used consistently). Exit 0.
2. `update-matched-set.sh 0x006fc690`: recorded at 100.00% in the proof's `matched_set.json`.
3. One line broken (`== NULL` to `!= NULL`): **97.93%**, `REGRESSION: matched_set.json has
   100.00%`, the differing instruction shown (`jne` against `je` at +0x1d), exit 1.
   `update-matched-set.sh` refuses too, and the matched set's hash is unchanged.
4. Line restored: 100% again; nothing to record.
5. The entry deleted from the matched set: `--all` alone passes (exit 0), `--all --base` with the
   set from step 2 reports `REGRESSION: matched_set.json drops the entry`, exit 1.

**One function through the loop** (`proof/loop_proof.sh --standin`, about 30 s,
[`proof/results/loop_proof_standin.txt`](proof/results/loop_proof_standin.txt)). A scratch git
repository under `$SFCOP_WORK/harness/loopproof/` holds a copy of this checkout with the SDK
tree as `op/`; the queue holds `tDynamicAllocator::LockDownAllocator` (0x006fc750), whose code
is there but unclaimed. `loop.sh --max 2` runs with `HARNESS_CLAUDE` set to
`proof/standin_agent.py`, a scripted stand-in that makes the calls a session would: the guard
denies its edit of `matched_set.json` and its `update-matched-set.sh`, the Stop hook blocks it
before any change, the guard allows the annotation in `op/`, build-and-verify says 100% exact,
the Stop hook then allows it.

- Round 1 (Opus, build): the stand-in also writes `harness/lib/planted.py` behind the guard's
  back. The loop refuses to score, records `tampered`, and resets the worktree; the planted file
  and the annotation are gone and nothing is committed.
- Round 2 (Sonnet, fix round): the same without the tampering. `update-matched-set.sh` records
  it and `loop.sh` commits `feat(op): match tDynamicAllocator::LockDownAllocator (0x006fc750)`
  with the source line and the matched set.

**One function through the loop with a real session** (`proof/loop_proof.sh`, about 1 minute,
[`proof/results/loop_proof.txt`](proof/results/loop_proof.txt)). The same scratch setup, with one
Opus `claude -p` session: it added the annotation, build-and-verify said 100% exact, the Stop hook
let it finish, and the loop committed it (6 turns, 37 s). It needs `claude auth login` on this
machine. The scratch `op/` is the SDK tree alone with an empty matched set: an earlier run
copied the checkout's own units, where the function is already claimed, and its session deleted
that other unit's annotation to get past the double claim, which led to the claim-line rule under
Hooks.

## Limits and open points

- **Fable tier.** Sonnet (under 128 bytes) or Opus builds, Opus does one fix round and tries
  once more; Fable gets one
  time-boxed try at a near match (best at least 90%) and only when explicitly enabled
  (`--allow-fable`); it is off by default.
- **Unverified relocations** are accepted when used consistently, because most data symbols have
  no known address yet. As names are added to `functions.csv` and the analysis results, more of
  them become checked; the counts are in every result line.
- The game's own compile flags are not known; the only profile so far is the SDK's (`units.json`,
  `SRC_LAYOUT.md`).
- One compile per call, 4 to 6 s for an SDK-sized unit under QEMU; `--all` compiles each unit
  holding a recorded function once.
- `toolchain/bin/msvc-run` works out the SFCOP root as three levels above itself, which is wrong
  inside a worktree under `.claude/worktrees/`; the harness sets `SFCOP_ROOT` itself.

## For `scripts/check.sh`

`scripts/check.sh` is not this package's to edit. It should gain:

```sh
echo "== harness tests"
python3 -m pytest harness/tests -q
echo "== matched set holds, and holds the base branch's"
sh harness/build-and-verify.sh --all --base "origin/${GITHUB_BASE_REF:-develop}"
```

The second step needs the base branch fetched (`actions/checkout` with `fetch-depth: 0`, or a
`git fetch origin develop`). It compiles nothing while both matched sets are empty, and needs the
toolchain image and the exe on the runner once functions are recorded.
