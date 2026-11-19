#!/bin/sh
# The one command that is the definition of done. CI runs the same steps, and an agent that
# says "done" has run this once at the end. In order:
#
#   1. the Python tests (tests/, harness/tests/); CHECK_SKIP_TESTS=1 skips them where another
#      job has run them
#   2. the hotspot list and CODEOWNERS agree
#   3. the PORT: lines in op/ (listed, never a failure) and the SFCOP_FIX sites there (a failure:
#      a fix to game code never goes in op/)
#   4. the claims in op/: empty bodies, fragment claims, address order and stray files, each
#      break not in analysis/results/lint_baseline.tsv a failure, and so is a stale baseline row
#      (tools/lint_claims.py)
#   5. the progress gate: no function loses its status (scripts/progress.py)
#   6. the matched-set gate: no score drops below the base branch's. With the install and the
#      compiler image, every matched function is rebuilt (harness/build-and-verify.sh), unless
#      every changed file is documentation (scripts/rebuild_scope.py); otherwise only the
#      recorded scores are compared
#
# A step whose inputs are absent on this machine prints `skip: <why>` and does not fail; it is
# never left to pass silently. The base branch's copies are read with `git show <base>:<path>`;
# the base is $SFCOP_BASE_REF if set, else the base side of CI's merge checkout, else the merge
# base with origin/develop, else (on develop itself) the commit before.
#
# Usage: sh scripts/check.sh

set -eu

cd "$(dirname "$0")/.."

skip() { echo "skip: $*"; }
tmp=$(mktemp -d "${TMPDIR:-/tmp}/sfcop-check.XXXXXX")
trap 'rm -rf "$tmp"' EXIT INT TERM

echo "== tests"
if [ "${CHECK_SKIP_TESTS:-}" = 1 ]; then
    skip "CHECK_SKIP_TESTS=1: the tests ran in another job"
else
    python3 -m pytest tests harness/tests -q
fi

echo "== hotspot list and CODEOWNERS agree"
python3 - <<'EOF'
import re, sys
pats = [l.strip() for l in open('.github/hotspots.txt') if l.strip() and not l.startswith('#')]
owners = [l.split()[0].lstrip('/').rstrip('/') for l in open('.github/CODEOWNERS') if l.strip() and not l.startswith('#')]
missing = [p for p in pats if p.replace('/**', '').rstrip('/') not in owners]
if missing:
    print("hotspots.txt patterns with no CODEOWNERS line:", missing); sys.exit(1)
print("ok:", len(pats), "hotspot patterns,", len(owners), "CODEOWNERS lines")
EOF

echo "== PORT: lines in op/ (SFCOP_FIX sites there fail)"
if [ -d op ]; then
    python3 scripts/port_footprint.py --no-fix-sites op
else
    skip "no op/ tree yet"
fi

echo "== claims in op/ (tools/lint_claims.py; breaks not in its baseline fail)"
if [ -d op ]; then
    python3 tools/lint_claims.py
else
    skip "no op/ tree yet"
fi

# The commit whose copies of the progress and matched-set files this tree is held to. Locally a
# branch is held to its merge base with origin/develop, not to origin/develop's tip: a figure
# develop raised after the branch was cut is not this branch's to keep, and CI (the merge with
# develop's tip) holds it to the tip anyway.
base_ref() {
    if [ -n "${SFCOP_BASE_REF:-}" ]; then
        echo "$SFCOP_BASE_REF"
    elif [ "${GITHUB_ACTIONS:-}" = true ] && git rev-parse --verify --quiet HEAD^2 >/dev/null; then
        git rev-parse HEAD^1                                    # CI: the merge's base side
    elif git rev-parse --verify --quiet origin/develop >/dev/null \
            && [ "$(git rev-parse HEAD)" != "$(git rev-parse origin/develop)" ]; then
        git merge-base HEAD origin/develop                     # a branch: where it left develop
    elif git rev-parse --verify --quiet HEAD^1 >/dev/null; then
        git rev-parse HEAD^1                                    # develop itself: the commit before
    fi
}
base=$(base_ref || true)

echo "== progress gate"
if [ -z "$base" ]; then
    skip "no base commit to compare with (set SFCOP_BASE_REF)"
elif ! git show "$base:analysis/results/progress/op/progress.json" > "$tmp/progress-base.json" 2>/dev/null; then
    skip "$base has no analysis/results/progress/op/progress.json"
else
    echo "held to $base"
    rc=0
    python3 scripts/progress.py --gate "$tmp/progress-base.json" \
        --matched-set analysis/results/matched_set.json > "$tmp/progress.log" 2>&1 || rc=$?
    grep -E '^(Progress:|REGRESSION|gate:|progress:)' "$tmp/progress.log" || tail -n 20 "$tmp/progress.log"
    [ "$rc" = 0 ] || exit "$rc"
fi

echo "== matched set holds the base branch's"
have_install=$(python3 -c 'import os, sys; sys.path.insert(0, "."); from harness.lib import paths; print(1 if os.path.isfile(paths.EXE) else "")')
# The harness compiles natively when toolchain/fetch-native.sh has installed the compiler, else
# in the sfcop-msvc6 image (harness/lib/paths.py, pick_cl).
cl=$(python3 -c 'import sys; sys.path.insert(0, "."); from harness.lib import paths; print(paths.HARNESS_CL)')
if [ "$(basename "$cl")" = cl-native ]; then
    have_image=1
    echo "compiler: native (toolchain/bin/cl-native); a preview: CI rebuilds with the image, the result of record"
else
    have_image=$(docker image inspect sfcop-msvc6:latest >/dev/null 2>&1 && echo 1 || true)
    echo "compiler: the sfcop-msvc6 image (toolchain/bin/cl)"
fi
scope=rebuild
if [ -n "$base" ]; then
    scope=$(python3 scripts/rebuild_scope.py "$base" 2>/dev/null || echo rebuild)
fi
if [ -z "$base" ]; then
    skip "no base commit to compare with (set SFCOP_BASE_REF)"
elif [ -n "$have_install" ] && [ -n "$have_image" ] && [ "$scope" != skip ]; then
    sh harness/build-and-verify.sh --all --base "$base"
else
    if [ "$scope" = skip ]; then
        skip "only documentation changed since $base: the matched functions are not rebuilt; only their scores are held to it"
    else
        skip "no compiler (native or the sfcop-msvc6 image) or no install: the matched functions are not rebuilt; only their scores are held to $base"
    fi
    python3 - "$base" <<'EOF'
import os
import sys
sys.path.insert(0, '.')
from harness.lib import funcs, matched, paths, verify
base, label = verify.load_base(sys.argv[1])
data = matched.load(paths.MATCHED_SET)
base, released = matched.release_listed(base, data, matched.boundary_listed(paths.BOUNDARIES),
                                         verify.claimed_keys())
for k in released:
    print('released %s: on the boundary-error list and never exact in %s' % (k, label))
base, misscored = matched.release_misscored(base, data, verify.claimed_keys())
for k in misscored:
    print('released %s: scored against the plain destructor, not its deleting destructor' % k)
sizes = verify.load_base_sizes(sys.argv[1])
if sizes is not None and os.path.isfile(paths.FUNCTIONS_CSV):
    index = funcs.FunctionIndex(paths.FUNCTIONS_CSV, paths.SYMBOL_TSVS)
    base, reshaped = matched.release_reshaped(base, sizes, verify.function_sizes(index),
                                              verify.joined_keys(index))
    for k in reshaped:
        print('released %s: the analysis joined its prolog and body; never exact in %s' % (k, label))
bad = 0
for k, old in sorted(base['functions'].items()):
    new = data['functions'].get(k)
    if matched.worse(new, old):
        what = 'drops' if new is None else 'lowers to %.2f%%' % new['score']
        print('REGRESSION %s %s: matched_set.json %s the entry in %s (%.2f%%%s)'
              % (k, old.get('name', ''), what, label, old['score'], ' exact' if old.get('exact') else ''))
        bad += 1
if bad:
    sys.exit(1)
print('ok: %d entries held to %s' % (len(base['functions']), label))
EOF
fi

echo "check: ok"
