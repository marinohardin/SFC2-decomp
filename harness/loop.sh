#!/bin/sh
# The headless decompilation loop: one fresh `claude -p` session per function, judged only by
# harness/build-and-verify.sh.
#
#   harness/loop.sh [--once] [--max N] [--dry-run] [--allow-fable]
#
# Each round: claim the next function (harness/queue.py; a lock file keeps loops in other
# worktrees off it), score it as it stands, write the claim the Stop hook checks, run claude with
# the prompt harness/prompts/function.md and the hooks in harness/hooks/settings.fragment.json,
# then let harness/update-matched-set.sh judge the result. An improvement is committed on the
# current branch; anything else is undone and counted against the function's attempt cap.
# Usage-limit stops sleep and retry the same function without counting.
#
# The worktree must be clean when the loop starts. After each session the loop refuses to score
# if anything outside the source tree changed (tracked or untracked), or HEAD moved: the attempt
# is recorded as "tampered" and the whole worktree is reset to the round's starting commit.
# Ignored files under the judging directories (harness/, toolchain/, analysis/, ghidra/,
# scripts/, oracle/) are removed before scoring, so nothing planted there can run.
#
# Models (harness/README.md, "Model tiers"): attempt 1 Opus builds, attempts 2-3 Sonnet fix the
# near match, attempt 4 Opus. Fable runs only after those and only with --allow-fable.
#
# Runs under the owner's Claude Max login: it aborts if ANTHROPIC_API_KEY (or another variable
# that would switch billing) is set. Run it in its own worktree on a grind/ or feature/ branch.
#
# --dry-run does everything except call claude (no commit, no reset): it checks the environment,
# picks the function and model, scores it, writes the claim and the prompt, and logs the attempt
# with zero tokens and outcome "dry-run".
#
# HARNESS_CLAUDE replaces the claude command (same arguments); the loop proof uses it to drive a
# scripted stand-in agent. With the real claude, `claude auth status` must show a subscription
# login before the first round.
#
# Environment: HARNESS_TIERS (opus,sonnet,sonnet,opus), HARNESS_ALLOW_FABLE (0),
# HARNESS_FABLE_TRIES (1), HARNESS_MAX_TURNS (60), HARNESS_LIMIT_SLEEP (300 s),
# HARNESS_FAIL_SLEEP (5 s), plus the paths in harness/lib/paths.py.
set -u
here=$(cd "$(dirname "$0")" && pwd)
repo=$(dirname "$here")
cd "$repo" || exit 2
export PYTHONPATH="$repo"
py() { python3 -m harness.lib.loopio "$@"; }

once=0 dry=0 max=0
while [ $# -gt 0 ]; do
    case "$1" in
        --once) once=1 ;;
        --dry-run) dry=1 ;;
        --max) max=$2; shift ;;
        --allow-fable) export HARNESS_ALLOW_FABLE=1 ;;
        *) echo "usage: $0 [--once] [--max N] [--dry-run] [--allow-fable]" >&2; exit 2 ;;
    esac
    shift
done

for v in ANTHROPIC_API_KEY ANTHROPIC_AUTH_TOKEN CLAUDE_CODE_USE_BEDROCK CLAUDE_CODE_USE_VERTEX; do
    if [ -n "$(printenv "$v" 2>/dev/null)" ]; then
        echo "ERROR $v is set: this loop runs on the Claude Max login only. Unset it and rerun." >&2
        exit 2
    fi
done

eval "$(python3 -c 'from harness.lib import paths; import os, shlex
rel = lambda p: os.path.relpath(p, paths.REPO)
print("state=" + shlex.quote(paths.STATE)); print("src=" + shlex.quote(rel(paths.SRC_ROOT)))
print("difficult=" + shlex.quote(rel(paths.DIFFICULT)))
print("matched=" + shlex.quote(rel(paths.MATCHED_SET)))')"
case "$src" in ..*|/*) echo "ERROR the source root must be inside this worktree for the loop" >&2; exit 2 ;; esac
mkdir -p "$state/attempts"
judges="harness toolchain analysis ghidra scripts oracle"
export HARNESS_LOOP_PID=$$
addr=
trap '[ -n "$addr" ] && python3 harness/queue.py release "$addr"' EXIT
trap 'exit 130' INT TERM

# Undo everything since the round started: tracked changes, new files, commits.
reset_round() {
    git reset -q --hard "$1"
    git clean -fdq
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

rounds=0
while :; do
    entry="$state/entry.json"
    python3 harness/queue.py next > "$entry" || exit 2
    if [ ! -s "$entry" ]; then echo "OK    queue empty"; break; fi
    addr=$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1]))["address"])' "$entry")
    model=$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1]))["model"])' "$entry")
    tag="$state/attempts/$addr-$(date +%Y%m%dT%H%M%S)"
    head0=$(git rev-parse HEAD)
    harness/build-and-verify.sh "$addr" --json > "$tag.before.json" 2>&1
    before=$(py claim "$entry" "$tag.before.json")
    snap_ok=1; py claims "$src" "$addr" > "$tag.claims.before" || snap_ok=0
    py prompt "$entry" > "$tag.prompt.md"
    echo "INFO  $addr $model (baseline $before%) -> $tag.*"

    t0=$(date +%s)
    if [ "$dry" = 1 ]; then
        printf '{"type":"result","subtype":"success","is_error":false,"result":"dry run","num_turns":0,"total_cost_usd":0,"usage":{"input_tokens":0,"output_tokens":0,"cache_read_input_tokens":0,"cache_creation_input_tokens":0}}\n' > "$tag.claude.json"
    else
        HARNESS_FUNCTION_SESSION=1 HARNESS_STATE="$state" "$agent" -p "$(cat "$tag.prompt.md")" \
            --model "$model" --output-format json --max-turns "${HARNESS_MAX_TURNS:-60}" \
            --settings "$here/hooks/settings.fragment.json" --permission-mode acceptEdits \
            --allowedTools "Read Edit Write Grep Glob Bash" > "$tag.claude.json" 2> "$tag.claude.err"
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
        set -- $(py score "$tag.after.json" "$addr")
        after=$1 kind=$2 code=$3
        improved=$(python3 -c 'import sys; print(int(float(sys.argv[2]) > float(sys.argv[1]) + 1e-6 or sys.argv[3] == "exact"))' "$before" "$after" "$kind")
        if [ "$code" = 0 ] && [ "$improved" = 1 ]; then
            outcome=$([ "$kind" = exact ] && echo exact || echo improved)
            name=$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1]))["name"])' "$entry")
            git add -A -- "$src" "$matched"
            git commit -q -m "feat(game): $([ "$kind" = exact ] && echo match || echo improve) $name ($addr)" \
                -m "build-and-verify: $before% -> $after% ($kind) with $model." \
                && echo "OK    $addr $outcome $before% -> $after%, committed"
        else
            outcome=$([ "$code" = 0 ] && echo no-change || echo failed)
            reset_round "$head0"
            echo "INFO  $addr $outcome ($before% -> $after%); worktree reset"
        fi
    fi
    python3 harness/queue.py record "$(py row "$entry" "$outcome" "$before" "$after" "$tag.claude.json" "$secs" "$now")"
    if [ "$dry" = 0 ] && [ -n "$(git status --porcelain -- "$difficult")" ]; then
        name=$(python3 -c 'import json,sys; print(json.load(open(sys.argv[1]))["name"])' "$entry")
        git add -- "$difficult"
        git commit -q -m "chore(harness): list $name ($addr) as difficult" \
            -m "Not exact after the attempt cap; best and last attempt in $difficult."
    fi
    python3 harness/queue.py release "$addr"; addr=

    rounds=$((rounds + 1))
    [ "$once" = 1 ] && break
    [ "$max" -gt 0 ] && [ "$rounds" -ge "$max" ] && break
    [ "$dry" = 1 ] && break
    case "$outcome" in failed|no-change) sleep "${HARNESS_FAIL_SLEEP:-5}" ;; esac
done
