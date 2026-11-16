#!/bin/sh
# Proof that one function round-trips through the loop: a real `claude -p` session (Opus, the
# build tier) claims tDynamicAllocator::LockDownAllocator (0x006fc750) in a source tree where its
# code is present but unclaimed, the hooks hold, the Stop hook lets it finish,
# update-matched-set.sh records it, and loop.sh commits it.
#
#   harness/proof/loop_proof.sh            # real claude; writes results/loop_proof.txt
#   harness/proof/loop_proof.sh --standin  # scripted stand-in agent (standin_agent.py) instead
#                                          # of claude; writes results/loop_proof_standin.txt
#
# The stand-in run has two rounds: in the first the agent also writes a file under harness/
# behind the guard, and the loop must refuse to score it and reset the worktree; the second
# (Sonnet, a fix round) claims the function and is committed.
#
# Everything happens in a scratch git repository under $SFCOP_WORK/harness/loopproof: a copy of
# this checkout's files with the SDK tree as op/. It is never pushed and holds the only commits
# the loop makes. Uses the Max login (no API key) and a few minutes of one Opus session.
set -eu
here=$(cd "$(dirname "$0")" && pwd)
harness=$(dirname "$here")
repo=$(dirname "$harness")
eval "$(PYTHONPATH="$repo" python3 -c 'from harness.lib import paths; import shlex
print("WORK=" + shlex.quote(paths.WORK)); print("REF=" + shlex.quote(paths.REFERENCE))
print("ROOT=" + shlex.quote(paths.SFCOP_ROOT))')"
proof="$WORK/harness/loopproof"
sdk="$REF/installed game/Starfleet Command Orion Pirates/OP API - R2.1"
out="$here/results/loop_proof.txt"
mode="claude -p (Opus)"
standin=0 rounds="--once"
if [ "${1:-}" = --standin ]; then
    standin=1
    out="$here/results/loop_proof_standin.txt"
    mode="the scripted stand-in agent harness/proof/standin_agent.py, not claude"
    rounds="--max 2"
fi
fn=0x006fc750

rm -rf "$proof"
mkdir -p "$proof/repo/game" "$proof/state" "$here/results"
(cd "$repo" && git ls-files -co --exclude-standard -z | xargs -0 tar cf - | tar xf - -C "$proof/repo")
# op/ is the SDK tree alone, and nothing is recorded yet: the checkout's own units claim
# functions (0x006fc750 among them) that the proof needs unclaimed.
rm -rf "$proof/repo/game"; mkdir -p "$proof/repo/game"
printf '{"version": 1, "functions": {}}\n' > "$proof/repo/analysis/results/matched_set.json"
head -1 "$repo/analysis/results/difficult_functions.tsv" > "$proof/repo/analysis/results/difficult_functions.tsv"
cp -R "$sdk/Scripts" "$sdk/STL" "$proof/repo/op/"
chmod -R u+w "$proof/repo/game"
flags=$(grep -m1 '^# ADD CPP /nologo /MT ' "$proof/repo/op/Scripts/ScriptInterface/_ScriptInterface.dsp" \
        | sed 's/^# ADD CPP //' | tr -d '\r')
python3 - "$proof/units.json" "$flags" "$proof/state/queue.json" "$fn" <<'EOF'
import json, sys
units, flags, queue, fn = sys.argv[1:]
json.dump({'module': 'STARFLEETOP', 'default_profile': None,
           'profiles': {'sdk-script-interface': {'compiler': '6.4', 'include_first': ['STL/Include'],
                                                 'flags': flags}},
           'units': {'Scripts/ScriptInterface/DynamicAllocator.cpp':
                     {'profile': 'sdk-script-interface'}}}, open(units, 'w'), indent=1)
json.dump({'built': 'loop proof', 'entries': [
    {'address': fn, 'name': 'tDynamicAllocator::LockDownAllocator', 'size': 59, 'named': True,
     'similarity': 0.0}]}, open(queue, 'w'))
EOF
cd "$proof/repo"
git init -q -b grind/loop-proof
git add -A
git commit -q -m "loop proof: base tree with the SDK as op/" --no-verify

export SFCOP_ROOT="$ROOT" SFCOP_WORK="$WORK" SFCOP_REFERENCE="$REF"
export HARNESS_STATE="$proof/state" HARNESS_UNITS="$proof/units.json"
export HARNESS_MAX_TURNS=40 HARNESS_FAIL_SLEEP=0
# The proof runs with the repository's own instructions: it has no lean file.
export HARNESS_NO_LEAN=1 HARNESS_PACK=0
if [ "$standin" = 1 ]; then
    export HARNESS_CLAUDE="$proof/repo/harness/proof/standin_agent.py"
    export HARNESS_STANDIN_TAMPER="$proof/state/tampered-once"
fi

set +e
{
echo "# loop proof, $(date -u +%Y-%m-%dT%H:%MZ); scratch repository \$SFCOP_WORK/harness/loopproof/repo"
echo "# agent: $mode"
echo
echo "\$ harness/loop.sh $rounds"
sh harness/loop.sh $rounds; echo "[exit $?]"
echo
echo "\$ git status --porcelain; ls harness/lib/planted.py   (the planted file must be gone)"
git status --porcelain; ls harness/lib/planted.py 2>&1 | sed 's/^.*: //'
echo "\$ git log --format=%s"
git log --format=%s
echo "\$ git log -1 --stat"
git log -1 --stat --format='%s%n%n%b' | grep -v '^ op/S[CT]' | head -12
echo "\$ git diff -U0 HEAD~1 -- game   (changed lines only; the SDK source itself is not shown)"
git diff -U0 HEAD~1 -- game | grep -E '^(\+\+\+|[+-][^+-])'
echo "\$ cat analysis/results/matched_set.json"
cat analysis/results/matched_set.json
echo "\$ cat \$HARNESS_STATE/attempts.tsv"
cat "$proof/state/attempts.tsv"
for f in $(ls "$proof"/state/attempts/*.claude.json); do
    echo
    echo "## Session $(basename "$f" .claude.json): final message"
    python3 -c 'import json,sys; print(json.load(open(sys.argv[1])).get("result","")[:1500])' "$f"
done
} 2>&1 | sed -e "s#$WORK#\$SFCOP_WORK#g" | tee "$out"
