# harness: the compiler is the judge (task P1-5)

Lets a headless Claude Code loop rebuild the game one function at a time, with the pinned VC6
SP4 compiler and objdiff deciding whether it worked. One script judges; hooks keep the agent away
from everything that judges; scores only go up.

| File | What it is |
| --- | --- |
| `build-and-verify.sh <fn>` | The only judge. Compiles the function's unit, diffs it against the original bytes, refuses any score below `matched_set.json`. Read-only |
| `update-matched-set.sh <fn>` | The only writer of `matched_set.json`. Takes no score: it verifies the function itself and records an improvement |
| `../analysis/results/matched_set.json` | Best verified score per original function, keyed by original address |
| `units.json` | Compile profile per unit of the rebuilt tree (flags, compiler, include order) |
| `queue.py` | Builds and serves the work queue; claims functions; logs attempts; keeps `difficult_functions.tsv` |
| `../analysis/results/difficult_functions.tsv` | Functions that reached the attempt cap without becoming exact, for a human |
| `loop.sh` | The headless loop: one fresh `claude -p` per function |
| `prompts/function.md` | The prompt each session gets |
| `hooks/` | `PreToolUse` guard and `Stop` verifier, plus `settings.fragment.json` to merge |
| `SRC_LAYOUT.md` | Proposed layout of the rebuilt source (`game/`, one file per original unit) |
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
```

1. Finds the one source file under the rebuilt tree (`HARNESS_SRC_ROOT`, default `game/`) with
   `// FUNCTION: STARFLEETOP 0x006fc690` on the line before the function (`SRC_LAYOUT.md`).
2. Compiles that unit with `toolchain/bin/cl` and the unit's profile from `units.json`, adding
   only output paths and `/FAs`. The listing tells which compiled symbol follows the annotation.
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
   - **mismatch**: a wrong callee or global, an inconsistent use, a field past the end.

   ok and unverified fields are masked so objdiff sees them equal; a mismatch keeps the exe bytes
   and points at a symbol named after the exe address, so objdiff counts it. Identical opcodes
   calling the wrong function therefore do not pass.
5. Runs `objdiff-cli diff` for the score, and a strict byte comparison of the masked bytes.
   **Exact** means objdiff 100% and the strict check equal.
6. Re-scores every other recorded function in the same unit (a header or shared edit can break
   them) and compares all against `matched_set.json`.

Output is a few lines, each tagged `OK`, `ERROR` or `INFO`, then a `RESULT` line; for a function
that is not exact, the first differing instructions side by side and the path of a full
side-by-side listing. A failed compile says `BUILD HAS FAILED` with the compiler's errors.
Exit status: 0 nothing regressed, 1 a recorded function scored lower (or lost its source, or its
unit no longer builds), 2 the claimed function could not be built or scored.

`--base <file or git ref>` also holds the run to another copy of the matched set, compared by
original address: a file, or `analysis/results/matched_set.json` at a git ref (read with
`git show`). Every entry in the base must still be in this tree's matched set at the same score
or higher (and still exact if it was), or it is a regression; every function is then verified
against the higher of the two scores. Without it, a pull request that lowered or deleted an
entry together with the source that earned it would pass, since `--all` only checks the copy in
its own tree.

`--behavioural` is the hook for the replay oracle (`oracle/`). It is off: it prints
`behavioural: oracle not available` until `HARNESS_ORACLE` names a command that takes a function
address and exits 0 when the replay trace matches; a non-zero exit then counts as a regression.

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
  unless it improved on the score it had when the session started, with nothing regressed. After
  three refusals (`HARNESS_MAX_STOP_BLOCKS`) it lets the session end and the loop records a failed
  attempt.

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

### Model tiers

By counted attempts on the function:

| Attempt | Model | Role |
| --- | --- | --- |
| 1 | Opus | Build: write the function from the original instructions |
| 2, 3 | Sonnet | Fix rounds: close the differences left in the committed near match |
| 4 | Opus | Build again, with what the earlier attempts left |
| 5 | Fable | Only with `loop.sh --allow-fable` (or `HARNESS_ALLOW_FABLE=1`); `HARNESS_FABLE_TRIES`, default 1 |

The prompt tells each session its role. `HARNESS_TIERS` replaces the default sequence (for
example `opus,sonnet,opus`) and may not name Fable. The cap is the length of the sequence; at the
cap a function moves to `difficult_functions.tsv`, and the loop commits that. Usage-limit stops
and harness errors do not count; failed, no-change, improved and tampered attempts do. Improved
attempts are committed, so a fix round starts from the best source so far.

## The loop (`loop.sh`)

```sh
harness/loop.sh --dry-run        # everything except calling claude: no commit, no reset
harness/loop.sh --once           # one function
harness/loop.sh --max 20
harness/loop.sh --unit Tiger/tsay.cpp            # one grind batch: one unit, in address order
harness/loop.sh --once --address 0x006d2800      # a chosen function; both flags repeat
harness/loop.sh --allow-fable    # add the Fable tier after attempt 4
```

`--unit` takes a unit path from `game/UNITS.tsv` and `--address` a function address; each may
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
`prompts/function.md`; `claude -p --model <tier> --output-format json --settings
hooks/settings.fragment.json --permission-mode acceptEdits` runs with `HARNESS_FUNCTION_SESSION=1`;
then `update-matched-set.sh` judges. An improvement is committed on the current branch
(`feat(game): match <name> (<address>)` with the score change in the body). Anything else is
undone: the worktree is reset to the commit the round started from. Tokens, cost, turns and seconds of every attempt go to
`$HARNESS_STATE/attempts.tsv`, and the prompt, output and verification JSON of each attempt to
`$HARNESS_STATE/attempts/`.

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
tree as `game/`; the queue holds `tDynamicAllocator::LockDownAllocator` (0x006fc750), whose code
is there but unclaimed. `loop.sh --max 2` runs with `HARNESS_CLAUDE` set to
`proof/standin_agent.py`, a scripted stand-in that makes the calls a session would: the guard
denies its edit of `matched_set.json` and its `update-matched-set.sh`, the Stop hook blocks it
before any change, the guard allows the annotation in `game/`, build-and-verify says 100% exact,
the Stop hook then allows it.

- Round 1 (Opus, build): the stand-in also writes `harness/lib/planted.py` behind the guard's
  back. The loop refuses to score, records `tampered`, and resets the worktree; the planted file
  and the annotation are gone and nothing is committed.
- Round 2 (Sonnet, fix round): the same without the tampering. `update-matched-set.sh` records
  it and `loop.sh` commits `feat(game): match tDynamicAllocator::LockDownAllocator (0x006fc750)`
  with the source line and the matched set.

**One function through the loop with a real session** (`proof/loop_proof.sh`, about 1 minute,
[`proof/results/loop_proof.txt`](proof/results/loop_proof.txt)). The same scratch setup, with one
Opus `claude -p` session: it added the annotation, build-and-verify said 100% exact, the Stop hook
let it finish, and the loop committed it (6 turns, 37 s). It needs `claude auth login` on this
machine. The scratch `game/` is the SDK tree alone with an empty matched set: an earlier run
copied the checkout's own units, where the function is already claimed, and its session deleted
that other unit's annotation to get past the double claim, which led to the claim-line rule under
Hooks.

## Limits and open points

- **Fable tier.** Opus builds, Sonnet does fix rounds, Fable only after Opus fails and only when
  explicitly enabled (`--allow-fable`); it is off by default.
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
