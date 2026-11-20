# harness: the compiler is the judge

Lets a headless Claude Code loop rebuild the game one function at a time, with the pinned VC6
SP4 compiler and objdiff deciding whether it worked. One script judges; hooks keep the agent away
from everything that judges; scores only go up.

| File | What it is |
| --- | --- |
| `build-and-verify.sh <fn>` | The only judge. Compiles the function's unit, diffs it against the original bytes, refuses any score below `matched_set.json`. Read-only |
| `update-matched-set.sh <fn>` | The only writer of `matched_set.json`. Takes no score: it verifies the function itself and records an improvement |
| `../analysis/results/matched_set.json` | Best verified score per original function, keyed by original address |
| `../analysis/results/reloc_baseline.tsv` | Known relocations of exact functions that name another symbol than the claim at their target (step 8 of `build-and-verify.sh`) |
| `units.json` | Compile profile per unit of the rebuilt tree (flags, compiler, include order) |
| `queue.py` | Builds and serves the work queue; claims functions; logs attempts; keeps `difficult_functions.tsv` |
| `../analysis/results/difficult_functions.tsv` | Functions that had every attempt the stop rule gives without becoming exact, for a human |
| `../analysis/results/revisit_functions.tsv` | Functions the stop rule ended short of 100% before their full chance, for a later revisit pass (`--revisit`) |
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

The matched set and the difficult list are committed in `analysis/results/`, not here: every
grind pull request updates them, and `harness/` is a hotspot.

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
   scheduled Docker audit checks it). The listing tells which compiled symbol follows the
   annotation; for a header claim, the header's line records in the unit's listing do. An inline
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
8. Checks that every REL32 / DIR32 relocation of an exact function that reaches a claimed
   function (an address in `matched_set.json`, or one scored in the run) names the symbol that
   claim defines (`report_claim_symbols` in `lib/verify.py`). The byte match masks the field,
   so a call through a stand-in class name or signature scores exact, but a consumer linking the
   exact objects (manifest mode `vc6`) is left with an unresolved external. The claim's symbol
   is the one the run's rebuild of its unit defines; a matched-set `symbol` field is only a
   fallback, because it is rewritten only when a score improves and can be stale after a rename.
   A break against a rebuilt symbol is an `ERROR` line naming the function, the relocation
   offset, the symbol, the exe address and the claimed symbol, and fails the run (exit 2); one
   against a recorded symbol only (a single-function run that did not build the target's unit)
   is a `WARN` line, and `--all`, which rebuilds every claim, decides. Targets nothing claims
   (library and CRT code) are not checked. A legitimate case, such as two identical functions
   the linker folded into one address, is listed in the baseline too.

   **The relocation baseline** (`analysis/results/reloc_baseline.tsv`, `HARNESS_RELOC_BASELINE`):
   the breaks known when the check came in, one row each (`function`, `offset`, `symbol`,
   `target`, `claimed`), matched by function, symbol and target (the offset is for the reader).
   A listed break is tolerated, with one `INFO` line counting them; a row whose break is gone is
   a `WARN` line asking for its removal (under `--all` every row, otherwise only the rows of the
   exact functions scored), so the list only shrinks. Every `--all` run writes the breaks it found,
   in the same format, to `reloc_breaks.tsv` in its build folder; the baseline is only ever
   replaced from such a file by the integrator, never extended to let a new break through.

Output is a few lines, each tagged `OK`, `ERROR` or `INFO`, then a `RESULT` line; for a function
that is not exact, the first differing instructions side by side and the path of a full
side-by-side listing. A failed compile says `BUILD HAS FAILED` with the compiler's errors.
Exit status: 0 nothing regressed, 1 a recorded function scored lower (or lost its source, or its
unit no longer builds), 2 the claimed function could not be built or scored, or step 7 or 8
found a symbol that the exact functions use wrongly.

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

### Whole-tree checks

Once the function itself is judged good, the Stop hook runs four checks on the whole tree
(`lib/wholetree.py`), and `loop.sh` runs them again before it commits the round, so the session
that causes a problem is told while it still has the context:

1. `build-and-verify.sh --all --json`: a regression or an `ERROR` in any unit, the relocation
   and claim-symbol steps included. Warm, unchanged units are neither recompiled nor rescored.
2. `tools/manifest.py`, written to a scratch file in the harness state: a claim on a non-function,
   a class declared twice or a declaration it cannot read. The loop's commit is unchanged: it
   still does not regenerate `op/manifest.tsv`.
3. `tools/lint_claims.py`: a new finding. A stale baseline row is not the session's doing and
   does not fail.
4. `pytest tests/test_units.py tests/test_manifest.py`, without
   `test_committed_manifest_matches_the_tree`, which the loop cannot satisfy because it never
   regenerates the manifest.

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
consumer linking the objects cannot resolve it. A single-function run prints a `WARN stand-in`
line for each and lists them under `standins` in `--json`. The Stop hook refuses a session whose
function holds one, and the loop does not commit it (exit 5 from `loopio.score`). Rows on the
relocation baseline (`analysis/results/reloc_baseline.tsv`) are tolerated, as for exact
functions. Without a table (no `--all` yet in the checkout) only claims rebuilt in the same run
are checked. The table is as old as the `--all` that wrote it, so `loop.sh` writes it again at
every start (a full `--all`: about 2 minutes warm, 4 cold with the native compiler; start lanes
one after another); after merging into a running lane, restart its loop (or run `--all`) so a renamed
claim is not refused as a stand-in. A claim that an `--all` could not build keeps its row from the table
it replaces. Known cost: a session refused against a table older than a merge cannot fix the
refusal itself, and the attempt is lost until the loop is restarted. The context pack lists a claimed callee's symbol as what the
call must name; a symbol from `functions.csv` or the SDK is given as a lead only.

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
judged with the regular sequence.

When a run ends short of 100%, `queue.py record` lists the function, and the loop commits the
list:

- **`analysis/results/difficult_functions.tsv`** when it had its full chance: every model of
  the regular sequence was tried and, if its best qualified for Fable, Fable was tried too. The
  queue never takes it again.
- **`analysis/results/revisit_functions.tsv`** otherwise: the stop rule ended it early, or it is
  waiting for a Fable try that was not allowed. Columns: `address`, `name`, `best_score`,
  `attempts`, `last_model`, `listed` (the time of its last attempt) and `note` (why the run
  ended). The queue skips these functions unless `--revisit` (or `HARNESS_TAKE_REVISIT=1`) is
  given. A revisit run starts over and counts only the attempts after `listed`, except for a
  function that was waiting for Fable, which gets its Fable try instead. A revisit that ends
  exact takes the function off the list. A function gets one revisit run: one that ends short
  again moves it to `difficult_functions.tsv`, unless it is left waiting for a Fable try that
  was not allowed, which keeps its row. Listing it again would hand it straight back to the
  same `--revisit` loop.

The two lists live in each checkout, but the attempt log is shared, so a loop on one branch
does not see what a loop on another branch listed. `--revisit` therefore also takes a function
that is on neither list here but whose last run in the shared log ended short of its full
chance and is not exact: it gets a new run, cut from the log alone (a run ends where the rule
gives no further model, with Fable counted as allowed, so a Fable try continues the run it was
waiting at). A function waiting for Fable stays waiting without `--allow-fable`, and one that
used its full chance is left alone. Without `--revisit` the queue skips all of them.

`queue.py sweep` settles the functions in the attempt log that are on neither list but whose
run the rule has already ended: those attempted before the rule existed. The loop does not run
it; run it once, on a grind branch, after the loops that used the old tiers have stopped, and
commit the lists it writes. It reads matched_set.json too: a function recorded exact is never
listed, and is taken off either list, because a function matched outside the loop has no exact
attempt in the log.

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
then `update-matched-set.sh` judges. An improvement is committed on the current branch
(`feat(op): match <name> (<address>)` with the score change in the body). Anything else is
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
