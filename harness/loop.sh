#!/bin/sh
# The headless decompilation loop: one fresh `claude -p` session per function, judged only by
# harness/build-and-verify.sh.
#
#   harness/loop.sh [--once] [--max N] [--dry-run] [--unit U]... [--address A]... [--brief FILE]
#                   [--lean FILE | --no-lean] [--allow-fable] [--revisit]
#
# Each round: claim the next function (harness/queue.py; a lock file keeps loops in other
# worktrees off it), score it as it stands, write the claim the Stop hook checks, run claude with
# the prompt harness/prompts/function.md and the hooks in harness/hooks/settings.fragment.json,
# then let harness/update-matched-set.sh judge the result. An improvement is committed (source
# only) on the current branch; anything else is undone and counts as an attempt. An attempt that does not end
# exact leaves a handoff note in the harness state for the next one (harness/lib/notes.py), and
# the prompt carries a context pack (harness/lib/pack.py). Usage-limit stops sleep and retry the
# same function without counting.
#
# The worktree must be clean when the loop starts. After each session the loop refuses to score
# if anything outside the source tree changed (tracked or untracked), or HEAD moved: the attempt
# is recorded as "tampered" and the whole worktree is reset to the round's starting commit.
# Ignored files under the judging directories (harness/, toolchain/, analysis/, ghidra/,
# scripts/, oracle/) are removed before scoring, so nothing planted there can run.
#
# Models and the stop rule (harness/README.md, "Model tiers"): attempt 1 builds (Sonnet for a
# function under 128 bytes, Opus otherwise), attempt 2 Opus fixes the near match, attempt 3 Opus
# builds again. From attempt 2 on, an attempt that does not raise the best score ends the run,
# and after two attempts so does a best of 90% or less. Fable gets one time-boxed try at a
# function whose best is at least 90%, and only with --allow-fable. A run that ends short of
# 100% lists the function in this lane's revisit_functions.tsv (or difficult_functions.tsv when it
# had every attempt), outside the repository; the queue skips listed functions, of every lane,
# unless --revisit is given.
#
# A lane commits source only: each commit carries a `Harness-Score:` trailer with the function's
# result, and the scores themselves go to the lane's matched set in the state folder. Develop's
# matched_set.json and manifest are written once, by the landing step (harness/landing.sh).
#
# Runs under the owner's Claude Max login: it aborts if ANTHROPIC_API_KEY (or another variable
# that would switch billing) is set. Run it in its own worktree on a grind/ or feature/ branch.
#
# --dry-run does everything except call claude (no commit, no reset): it checks the environment,
# picks the function and model, scores it, writes the claim and the prompt, and logs the attempt
# with zero tokens and outcome "dry-run".
#
# --unit U (a path from op/UNITS.tsv, e.g. Tiger/tsay.cpp) and --address A, each repeatable,
# restrict the loop to those units' and addresses' open functions, taken in address order: a
# grind batch is one unit. They are resolved once, when the loop starts (harness/queue.py
# select), into a file in the harness state that the session cannot edit. Without them the loop
# takes the whole queue.
#
# --brief FILE appends a batch brief to every session's prompt: notes on the unit's classes,
# say, that are working material for one batch and are never committed. The file must lie
# outside this worktree; it is copied into the harness state when the loop starts, so the
# session cannot change it and later rounds get the same text.
#
# --lean FILE gives every session a lean instruction file in place of the repository's
# CLAUDE.md and AGENTS.md: the session runs with `--setting-sources user` (no project or local
# settings, so no CLAUDE.md is loaded) and the file's text as `--append-system-prompt`. The file
# is never committed and must lie outside this worktree; it is copied into the harness state when
# the loop starts. HARNESS_LEAN_INSTRUCTIONS names it too. The loop refuses to start without one
# unless --no-lean (or HARNESS_NO_LEAN=1) says to run with the repository's instructions instead.
# Every session runs with `--strict-mcp-config` and no MCP server either way.
#
# HARNESS_CLAUDE replaces the claude command (same arguments); the loop proof uses it to drive a
# scripted stand-in agent. With the real claude, `claude auth status` must show a subscription
# login before the first round.
#
# Environment: HARNESS_TIERS (opus,opus,opus), HARNESS_SMALL_TIERS (sonnet,opus,opus),
# HARNESS_SMALL_BYTES (128; 0: one order for every size), HARNESS_NEAR (90), HARNESS_ALLOW_FABLE (0),
# HARNESS_FABLE_TRIES (1), HARNESS_FABLE_MAX_TURNS (30), HARNESS_FABLE_TIMEOUT (1800 s),
# HARNESS_MAX_TURNS (60), HARNESS_SESSION_TIMEOUT (0: none), HARNESS_LEAN_INSTRUCTIONS,
# HARNESS_NO_LEAN (0), HARNESS_WHOLETREE (1; 0 skips the whole-tree checks), HARNESS_WHOLETREE_BUDGET (600 s), HARNESS_LANE_FILES,
# HARNESS_TAKE_REVISIT (0), HARNESS_PACK (1), HARNESS_PACK_MAX (12000), HARNESS_NEARMISS (1; 0:
# reference-only near misses take the tiers like any other),
# HARNESS_LIMIT_SLEEP (300 s), HARNESS_FAIL_SLEEP (5 s), plus the paths in
# harness/lib/paths.py.
set -u
here=$(cd "$(dirname "$0")" && pwd)
repo=$(dirname "$here")
caller=$(pwd)
cd "$repo" || exit 2
export PYTHONPATH="$repo"
py() { python3 -m harness.lib.loopio "$@"; }

usage="usage: $0 [--once] [--max N] [--dry-run] [--unit U]... [--address A]... [--brief FILE] [--lean FILE | --no-lean] [--allow-fable] [--revisit]"
once=0 dry=0 max=0
selargs= brief=
lean=${HARNESS_LEAN_INSTRUCTIONS:-} nolean=${HARNESS_NO_LEAN:-0}
while [ $# -gt 0 ]; do
    case "$1" in
        --once) once=1 ;;
        --dry-run) dry=1 ;;
        --max) max=$2; shift ;;
        --unit|--address)
            # Unit paths and addresses use these characters only, so the list needs no quoting.
            case "${2:-}" in
                ''|*[!A-Za-z0-9_./+-]*) echo "ERROR $1 takes a unit path from op/UNITS.tsv or an address" >&2; exit 2 ;;
            esac
            selargs="$selargs $1 $2"; shift ;;
        --brief) [ $# -ge 2 ] && [ -n "$2" ] || { echo "$usage" >&2; exit 2; }
            brief=$2; shift ;;
        --lean) [ $# -ge 2 ] && [ -n "$2" ] || { echo "$usage" >&2; exit 2; }
            lean=$2; shift ;;
        --no-lean) nolean=1 ;;
        --allow-fable) export HARNESS_ALLOW_FABLE=1 ;;
        --revisit) export HARNESS_TAKE_REVISIT=1 ;;
        *) echo "$usage" >&2; exit 2 ;;
    esac
    shift
done

# outside_worktree WHAT FILE: FILE (relative to the caller) must be a readable, non-empty file
# that does not lie inside this worktree. Prints the absolute path.
outside_worktree() {
    f=$2
    # A relative path is the caller's, not the repository root's.
    case "$f" in /*) ;; *) f="$caller/$f" ;; esac
    [ -f "$f" ] && [ -r "$f" ] || { echo "ERROR no readable $1 at $f" >&2; return 2; }
    [ -n "$(tr -d ' \t\r\n' < "$f")" ] || { echo "ERROR the $1 $f is empty" >&2; return 2; }
    # Inside the worktree if any parent directory of its real path is the worktree (samefile, so
    # a path that differs only in letter case on a case-insensitive volume is caught too).
    if python3 -c 'import os, sys
b, r = os.path.realpath(sys.argv[1]), sys.argv[2]
d = os.path.dirname(b)
while True:
    if os.path.samefile(d, r): sys.exit(0)
    up = os.path.dirname(d)
    if up == d: sys.exit(1)
    d = up' "$f" "$repo"; then
        echo "ERROR the $1 must lie outside this worktree: it is never committed" >&2
        return 2
    fi
    echo "$f"
}
if [ -n "$brief" ]; then
    brief=$(outside_worktree brief "$brief") || exit 2
fi
if [ "$nolean" = 1 ]; then
    lean=
elif [ -n "$lean" ]; then
    lean=$(outside_worktree "lean instruction file" "$lean") || exit 2
else
    echo "ERROR no lean instruction file: pass --lean FILE or set HARNESS_LEAN_INSTRUCTIONS (a file outside this worktree), or pass --no-lean to give sessions the repository's CLAUDE.md and AGENTS.md (harness/README.md, \"Lean sessions\")" >&2
    exit 2
fi

for v in ANTHROPIC_API_KEY ANTHROPIC_AUTH_TOKEN CLAUDE_CODE_USE_BEDROCK CLAUDE_CODE_USE_VERTEX; do
    if [ -n "$(printenv "$v" 2>/dev/null)" ]; then
        echo "ERROR $v is set: this loop runs on the Claude Max login only. Unset it and rerun." >&2
        exit 2
    fi
done

eval "$(python3 -c 'from harness.lib import paths; import os, shlex
rel = lambda p: os.path.relpath(p, paths.REPO)
print("state=" + shlex.quote(paths.STATE)); print("src=" + shlex.quote(rel(paths.SRC_ROOT)))
print("lane=" + shlex.quote(paths.LANE))')"
case "$src" in ..*|/*) echo "ERROR the source root must be inside this worktree for the loop" >&2; exit 2 ;; esac
mkdir -p "$state/attempts"
# A lane commits source only (harness/README.md, "Lanes commit source only"): its scores live in
# its own matched set in the state folder, seeded from the committed one and topped up from it at
# every start, and exported so every reader (the queue, the verifier, the hooks, the session's own
# children) sees it. HARNESS_MATCHED_SET set by the caller names the lane's set instead. The
# revisit and difficult lists are the lane's two files under the lists folder (paths.py).
if [ -z "${HARNESS_MATCHED_SET:-}" ]; then
    export HARNESS_MATCHED_SET="$state/matched_set.json"
fi
py seed "$HARNESS_MATCHED_SET" || exit 2
export SFCOP_LANE=1
judges="harness toolchain analysis ghidra scripts oracle"
export HARNESS_LOOP_PID=$$
addr=
trap '[ -n "$addr" ] && python3 harness/queue.py release "$addr"' EXIT
# A signal stops the running session first (timebox passes it to the session's process group and
# waits for it), so no session goes on editing after the EXIT trap has released its claim.
session=
trap '[ -n "$session" ] && kill -TERM "$session" 2>/dev/null && wait "$session"; exit 130' INT TERM HUP

# Undo everything since the round started: tracked changes, new files, commits, and the lane's
# matched set (outside the worktree, so git does not restore it) as it was at the round's start.
msnap=
reset_round() {
    git reset -q --hard "$1"
    git clean -fdq
    [ -n "$msnap" ] && [ -f "$msnap" ] && cp "$msnap" "$HARNESS_MATCHED_SET"
}
branch=$(git rev-parse --abbrev-ref HEAD)
case "$branch" in
    main|develop) echo "ERROR on $branch: run the loop on a grind/ or feature/ branch in its own worktree" >&2; exit 2 ;;
esac
agent=${HARNESS_CLAUDE:-claude}
if [ "$dry" = 0 ]; then
    command -v "$agent" >/dev/null || { echo "ERROR $agent is not on PATH" >&2; exit 2; }
    if [ -z "${HARNESS_CLAUDE:-}" ]; then
        claude auth status 2>/dev/null | py login - || exit 2
    fi
    if [ -n "$(git status --porcelain --untracked-files=all)" ]; then
        echo "ERROR uncommitted changes in this worktree: commit or move them first; the loop resets the worktree after a failed attempt" >&2
        exit 2
    fi
fi
[ -f "$state/queue.json" ] || python3 harness/queue.py build || exit 2
# The symbol table the stand-in check reads (harness/lib/symtab.py), written again at every
# start, so a merge into the lane between runs is in it: one full verify, cheap once the caches
# are warm.
if [ "$dry" = 0 ]; then
    echo "INFO  writing the symbol table: harness/build-and-verify.sh --all (log $state/symbols.log)"
    HARNESS_STATE="$state" nice -n 5 harness/build-and-verify.sh --all > "$state/symbols.log" 2>&1
    rc=$?
    if [ -f "$state/build/symbols.tsv" ]; then
        echo "INFO  symbol table: $(($(wc -l < "$state/build/symbols.tsv") - 1)) claims (--all exit $rc)"
        [ "$rc" = 0 ] || echo "WARN  --all exit $rc: claims it could not build keep their earlier rows"
    else
        echo "WARN  no symbol table: stand-ins are checked only against claims rebuilt in the same run"
    fi
    # A lane must start from a green tree: the checks every session is held to (harness/README.md,
    # "Whole-tree checks"), on the tree as it stands, reusing the --all run above.
    if ! wt=$(python3 -m harness.lib.wholetree --verify-log "$state/symbols.log" --verify-exit "$rc" 2>&1); then
        printf '%s\n' "$wt" >&2
        echo "ERROR the whole-tree checks fail before any session: the lane must start from a green tree. Fix the failures above (or start from a green develop); HARNESS_WHOLETREE=0 skips this check and the ones after each session" >&2
        exit 2
    fi
    # The near-miss label of every function this --all left short of exact (harness/lib/nearmiss.py),
    # stored in the lane's lists; queue.py next routes reference-only functions by it.
    python3 -m harness.lib.nearmiss --store --tsv > "$state/nearmiss.tsv" 2>> "$state/nearmiss.log" \
        || echo "WARN  could not label the near misses (see $state/nearmiss.log)"
fi
unset HARNESS_SELECTION HARNESS_BRIEF
if [ -n "$lean" ]; then
    if [ "$(python3 -c 'import os, sys; print(os.path.realpath(sys.argv[1]))' "$lean")" != \
         "$(python3 -c 'import os, sys; print(os.path.realpath(sys.argv[1]))' "$state/lean.md")" ]; then
        cp "$lean" "$state/lean.md" || exit 2
    fi
    lean="$state/lean.md"
fi
if [ -n "$brief" ]; then
    if [ "$(python3 -c 'import os, sys; print(os.path.realpath(sys.argv[1]))' "$brief")" != \
         "$(python3 -c 'import os, sys; print(os.path.realpath(sys.argv[1]))' "$state/brief.md")" ]; then
        cp "$brief" "$state/brief.md" || exit 2
    fi
    export HARNESS_BRIEF="$state/brief.md"
fi
if [ -n "$selargs" ]; then
    # shellcheck disable=SC2086  # split on purpose: the values were checked above
    python3 harness/queue.py select $selargs || exit 2
    export HARNESS_SELECTION="$state/selection.json"
fi

rounds=0
while :; do
    entry="$state/entry.json"
    python3 harness/queue.py next > "$entry" || exit 2
    if [ ! -s "$entry" ]; then
        # Functions another loop holds are skipped too, so this is not always the end of the batch.
        if [ -n "$selargs" ]; then echo "OK    nothing left to take in the selection:$selargs (each done, stopped by the stop rule, on a list, or held by another loop)"
        else echo "OK    queue empty (or the rest is held by another loop)"; fi
        break
    fi
    addr=$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1]))["address"])' "$entry")
    model=$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1]))["model"])' "$entry")
    tag="$state/attempts/$addr-$(date +%Y%m%dT%H%M%S)"
    head0=$(git rev-parse HEAD)
    msnap="$tag.matched.before"; cp "$HARNESS_MATCHED_SET" "$msnap"
    harness/build-and-verify.sh "$addr" --json > "$tag.before.json" 2>&1
    before=$(py claim "$entry" "$tag.before.json")
    # Its label as the round starts, from the committed source just scored: the next round's
    # routing reads it, and the RULE: collector reads it back (nearmiss.py --rules).
    python3 -m harness.lib.nearmiss "$addr" --store --tsv > "$tag.nearmiss.tsv" 2>> "$state/nearmiss.log" \
        || echo "WARN  could not label $addr (see $state/nearmiss.log)"
    snap_ok=1; py claims "$src" "$addr" > "$tag.claims.before" || snap_ok=0
    # Never run a session on a missing or partial prompt (an unreadable brief, say).
    if ! py prompt "$entry" "$tag.pack.md" > "$tag.prompt.md" || [ ! -s "$tag.prompt.md" ]; then
        echo "ERROR could not write the prompt for $addr (see above); stopping" >&2
        exit 2
    fi
    echo "INFO  $addr $model (baseline $before%) -> $tag.*"

    # The session's command. Fable is time-boxed: fewer turns and a wall-clock cap.
    turns=${HARNESS_MAX_TURNS:-60} limit=${HARNESS_SESSION_TIMEOUT:-0}
    if [ "$model" = fable ]; then
        turns=${HARNESS_FABLE_MAX_TURNS:-30} limit=${HARNESS_FABLE_TIMEOUT:-1800}
    fi
    set -- --model "$model" --output-format json --max-turns "$turns" \
        --settings "$here/hooks/settings.fragment.json" --permission-mode acceptEdits \
        --allowedTools "Read Edit Write Grep Glob Bash" --strict-mcp-config
    [ -n "$lean" ] && set -- "$@" --setting-sources user --append-system-prompt "$(cat "$lean")"
    {
        echo "# timebox: ${limit} s (0: none)"
        if [ -n "$lean" ]; then echo "# lean instructions: $lean"
        else echo "# lean instructions: none (the repository CLAUDE.md and AGENTS.md)"; fi
        printf '%s -p <%s>' "$agent" "$tag.prompt.md"
        prev=
        for w in "$@"; do
            if [ "$prev" = --append-system-prompt ]; then printf ' <%s>' "$lean"
            else printf " '%s'" "$w"; fi
            prev=$w
        done
        echo
    } > "$tag.command.txt"
    t0=$(date +%s)
    rc=0
    if [ "$dry" = 1 ]; then
        printf '{"type":"result","subtype":"success","is_error":false,"result":"dry run","num_turns":0,"total_cost_usd":0,"usage":{"input_tokens":0,"output_tokens":0,"cache_read_input_tokens":0,"cache_creation_input_tokens":0}}\n' > "$tag.claude.json"
    else
        # In the background and waited for, so the trap above runs at once on a signal.
        HARNESS_FUNCTION_SESSION=1 HARNESS_STATE="$state" python3 -m harness.lib.timebox "$limit" \
            "$agent" -p "$(cat "$tag.prompt.md")" "$@" > "$tag.claude.json" 2> "$tag.claude.err" < /dev/null &
        session=$!
        rc=0; wait "$session" || rc=$?
        session=
        [ "$rc" = 124 ] && echo "INFO  $addr: the session hit its ${limit} s time box; scoring what it left"
    fi
    secs=$(( $(date +%s) - t0 ))
    now=$(date -u +%Y-%m-%dT%H:%M:%SZ)

    if [ "$dry" = 0 ] && py autherror "$tag.claude.json"; then
        python3 harness/queue.py record "$(py row "$entry" error "$before" "$before" "$tag.claude.json" "$secs" "$now")"
        reset_round "$head0"
        echo "ERROR claude could not log in (see $tag.claude.json); run \`claude auth login\` and restart" >&2
        exit 2
    fi
    if [ "$dry" = 0 ] && py limited "$tag.claude.json"; then
        python3 harness/queue.py record "$(py row "$entry" limit "$before" "$before" "$tag.claude.json" "$secs" "$now")"
        python3 harness/queue.py release "$addr"; addr=
        reset_round "$head0"
        echo "INFO  usage limit reached; sleeping ${HARNESS_LIMIT_SLEEP:-300} s"
        sleep "${HARNESS_LIMIT_SLEEP:-300}"
        continue
    fi

    tampered=
    if [ "$dry" = 0 ]; then
        [ "$(git rev-parse HEAD)" = "$head0" ] || tampered="HEAD moved"
        changed=$(git status --porcelain --untracked-files=all -- . ":(exclude)$src")
        [ -z "$changed" ] || tampered="changes outside $src: $(echo "$changed" | head -5 | tr '\n' ' ')"
        if [ -z "$tampered" ]; then
            py claims "$src" "$addr" > "$tag.claims.after" || snap_ok=0
            [ "$snap_ok" = 1 ] || tampered="the claim-line snapshot failed (see $tag.claims.*)"
            [ -n "$tampered" ] || cmp -s "$tag.claims.before" "$tag.claims.after" \
                || tampered="claim lines of other functions changed: $(diff "$tag.claims.before" "$tag.claims.after" | grep '^[<>]' | head -3 | tr '\n' ' ')"
        fi
    fi
    if [ "$dry" = 1 ]; then
        outcome=dry-run after=$before
        echo "INFO  dry run: claude not called; prompt in $tag.prompt.md"
    elif [ -n "$tampered" ]; then
        outcome=tampered after=$before
        reset_round "$head0"
        echo "ERROR $addr not scored: $tampered; worktree reset to $head0"
    else
        git clean -fdXq -- $judges
        harness/update-matched-set.sh "$addr" --json > "$tag.after.json" 2>&1
        # STAND-IN: lines in the final message are allowances, kept in the lane state before the
        # whole-tree checks read them (harness/lib/standins.py).
        python3 -m harness.lib.standins --record "$tag.claude.json" || true
        set -- $(py score "$tag.after.json" "$addr")
        after=$1 kind=$2 code=$3
        improved=$(python3 -c 'import sys; print(int(float(sys.argv[2]) > float(sys.argv[1]) + 1e-6 or sys.argv[3] == "exact"))' "$before" "$after" "$kind")
        if [ "$code" = 0 ] && [ "$improved" = 1 ] && ! python3 -m harness.lib.wholetree > "$tag.wholetree.txt" 2>&1; then
            # The Stop hook runs the same checks; this catches a session it let go after three
            # refusals. The round is undone like any other failure.
            code=6
            echo "ERROR $addr: the whole-tree checks failed (see $tag.wholetree.txt)"
        fi
        # The symbol table is the one non-source file a lane commits, and only the tool writes it
        # (harness/lib/symsync.py): the claims of the rebuilt units, and the symbols of the
        # unclaimed functions this one calls. A sync that fails leaves no half-synced table.
        table=analysis/symbols/STARFLEETOP.tsv
        if [ "$code" = 0 ] && [ "$improved" = 1 ] && [ -f "$table" ] && [ -f tools/symbols.py ]; then
            if python3 -m harness.lib.symsync "$tag.after.json" "$addr" > "$tag.symbols.txt" 2>&1; then
                grep -E '^(WARN|ERROR)' "$tag.symbols.txt"
            else
                grep -v '^INFO' "$tag.symbols.txt"
                git checkout -q -- "$table"
                code=7
                echo "ERROR $addr: the symbol table sync failed (see $tag.symbols.txt); the table is restored"
            fi
        fi
        if [ "$code" = 0 ] && [ "$improved" = 1 ]; then
            outcome=$([ "$kind" = exact ] && echo exact || echo improved)
            name=$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1]))["name"])' "$entry")
            git add -A -- "$src"
            [ -f "$table" ] && git add -- "$table"
            git commit -q -m "feat(op): $([ "$kind" = exact ] && echo match || echo improve) $name ($addr)" \
                -m "build-and-verify: $before% -> $after% ($kind) with $model." \
                -m "$(py trailer "$addr" "$after" "$kind")" \
                && echo "OK    $addr $outcome $before% -> $after%, committed"
            python3 -m harness.lib.nearmiss "$addr" --store --tsv > /dev/null 2>> "$state/nearmiss.log" \
                || echo "WARN  could not relabel $addr after the commit; its stored label is stale (see $state/nearmiss.log)"
        else
            outcome=$([ "$code" = 0 ] && echo no-change || echo failed)
            reset_round "$head0"
            echo "INFO  $addr $outcome ($before% -> $after%); worktree reset"
        fi
    fi
    # A session stopped by its time box is logged as a timeout unless it left an exact match;
    # what it improved is still committed above, and the scores say how far it got.
    outcome=$(py outcome "$outcome" "$rc")
    if [ "$dry" = 0 ] && [ "$outcome" != exact ]; then
        python3 -m harness.lib.notes write "$entry" "$outcome" "$before" "$after" \
            "$tag.claude.json" "$tag.after.json" "$now" > /dev/null \
            || echo "WARN  could not write the handoff note for $addr"
    fi
    python3 harness/queue.py record "$(py row "$entry" "$outcome" "$before" "$after" "$tag.claude.json" "$secs" "$now")"
    python3 harness/queue.py release "$addr"; addr=

    rounds=$((rounds + 1))
    [ "$once" = 1 ] && break
    [ "$max" -gt 0 ] && [ "$rounds" -ge "$max" ] && break
    [ "$dry" = 1 ] && break
    case "$outcome" in failed|no-change) sleep "${HARNESS_FAIL_SLEEP:-5}" ;; esac
done
