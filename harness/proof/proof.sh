#!/bin/sh
# Proof of the harness (P1-5): one real function round-trips, and a regression is refused.
#
#   harness/proof/proof.sh            # writes harness/proof/results/proof.txt
#
# The function is tDynamicAllocator::Instance (0x006fc690), whose source ships with the SDK
# (Scripts/ScriptInterface/DynamicAllocator.cpp) and which the SDK library matcher placed in
# the exe byte for byte. The SDK tree is copied to $SFCOP_WORK/harness/proof/game and used as the
# source root, with its own unit registry and matched set there, so nothing committed changes and
# no SDK source enters the repository. Takes about a minute (five compiles of one unit).
set -u
here=$(cd "$(dirname "$0")" && pwd)
harness=$(dirname "$here")
eval "$(PYTHONPATH="$(dirname "$harness")" python3 -c 'from harness.lib import paths; print(f"WORK={paths.WORK!r}; REF={paths.REFERENCE!r}")')"
proof="$WORK/harness/proof"
sdk="$REF/installed game/Starfleet Command Orion Pirates/OP API - R2.1"
unit="$proof/game/Scripts/ScriptInterface/DynamicAllocator.cpp"
out="$here/results/proof.txt"
fn=0x006fc690

export HARNESS_SRC_ROOT="$proof/game"
export HARNESS_UNITS="$proof/units.json"
export HARNESS_MATCHED_SET="$proof/matched_set.json"
export HARNESS_STATE="$proof/state"

step() { printf '\n$ %s\n' "$*"; "$@"; echo "[exit $?]"; }
sub() {  # replace one exact line in the unit, failing loudly if it is not there once
    python3 - "$unit" "$1" "$2" <<'EOF'
import sys
path, old, new = sys.argv[1:]
text = open(path, encoding='latin1').read()
if text.count(old) != 1:
    sys.exit(f'expected one occurrence of {old!r}')
open(path, 'w', encoding='latin1').write(text.replace(old, new))
EOF
}

set -e  # setup must succeed; the demonstration below records failures instead
rm -rf "$proof"
mkdir -p "$proof/game" "$here/results"
cp -R "$sdk/Scripts" "$proof/game/"
cp -R "$sdk/STL" "$proof/game/"
chmod -R u+w "$proof/game"
flags=$(grep -m1 '^# ADD CPP /nologo /MT ' "$proof/game/Scripts/ScriptInterface/_ScriptInterface.dsp" \
        | sed 's/^# ADD CPP //' | tr -d '\r')
python3 - "$proof/units.json" "$flags" <<'EOF'
import json, sys
path, flags = sys.argv[1:]
json.dump({'module': 'STARFLEETOP', 'default_profile': None,
           'profiles': {'sdk-script-interface': {'compiler': '6.4', 'include_first': ['STL/Include'],
                                                 'flags': flags}},
           'units': {'Scripts/ScriptInterface/DynamicAllocator.cpp':
                     {'profile': 'sdk-script-interface'}}}, open(path, 'w'), indent=1)
EOF
printf '{"version": 1, "functions": {}}\n' > "$HARNESS_MATCHED_SET"
# Claim the function: the annotation goes on the line before its definition.
sub 'tDynamicAllocator* tDynamicAllocator::Instance()' \
    "// FUNCTION: STARFLEETOP $fn
tDynamicAllocator* tDynamicAllocator::Instance()"

set +e
{
echo "# harness proof, $(date -u +%Y-%m-%dT%H:%MZ)"
echo "# source root: \$SFCOP_WORK/harness/proof/game (SDK copy); matched set: \$SFCOP_WORK/harness/proof/matched_set.json"
echo
echo "## 1. The real SDK source scores 100% and is not yet recorded"
step "$harness/build-and-verify.sh" "$fn" --behavioural
echo
echo "## 2. The harness records it"
step "$harness/update-matched-set.sh" "$fn"
step cat "$HARNESS_MATCHED_SET"
echo
echo "## 3. One line broken (== NULL -> != NULL): the regression is refused"
sub '	if( fpDynamicAllocator == NULL )' '	if( fpDynamicAllocator != NULL )'
step "$harness/build-and-verify.sh" "$fn"
before=$(shasum -a 256 "$HARNESS_MATCHED_SET" | cut -c1-16)
step "$harness/update-matched-set.sh" "$fn"
after=$(shasum -a 256 "$HARNESS_MATCHED_SET" | cut -c1-16)
echo "matched_set.json sha256 before $before, after $after"
echo
echo "## 4. Line restored: back to 100%, nothing to record"
sub '	if( fpDynamicAllocator != NULL )' '	if( fpDynamicAllocator == NULL )'
step "$harness/build-and-verify.sh" "$fn"
step "$harness/update-matched-set.sh" "$fn"
echo
echo "## 5. A change that deletes the entry from the matched set passes --all on its own, but not"
echo "##    when held to the base branch's copy (here: the matched set as recorded in step 2)"
cp "$HARNESS_MATCHED_SET" "$proof/base_matched_set.json"
printf '{"version": 1, "functions": {}}\n' > "$HARNESS_MATCHED_SET"
step "$harness/build-and-verify.sh" --all
step "$harness/build-and-verify.sh" --all --base "$proof/base_matched_set.json"
} 2>&1 | sed -e "s#$WORK#\$SFCOP_WORK#g" -e "s#$(dirname "$harness")/##g" | tee "$out"
