#!/bin/sh
# The landing step: run once on an integration branch, after the lanes' pull requests are merged
# into it. Lanes commit source only (harness/README.md, "Lanes commit source only"), so this is
# where develop's matched_set.json and op/manifest.tsv catch up with the claims in op/.
#
#   harness/landing.sh [BASE]        BASE: the ref the landing is held to, default origin/develop
#
# 1. Rebuilds every claimed function and records its score in analysis/results/matched_set.json:
#    a raised score or a newly exact function is stored, and the name, symbol, unit and size of
#    every entry are refreshed from the tree (build-and-verify's own scoring; `verify.py --land`).
#    A function that scores lower than BASE's record, or has lost its source, is reported as a
#    REGRESSION and this script exits non-zero without writing the matched set.
# 2. Regenerates op/manifest.tsv from the annotations and the new matched set (tools/manifest.py).
#
# Commit what it changes. Exit 0 landed, 1 a regression, 2 a function could not be built or scored.
for v in HARNESS_MATCHED_SET SFCOP_LANE; do
    if [ -n "$(printenv "$v" 2>/dev/null)" ]; then
        echo "ERROR $v is set: that is a grind lane's environment, and a landing would record into the lane's copy. Unset it; the landing records into analysis/results/matched_set.json" >&2
        exit 2
    fi
done
here=$(cd "$(dirname "$0")" && pwd)
repo=$(dirname "$here")
base=${1:-origin/develop}
cd "$repo" || exit 2
PYTHONPATH="$repo" python3 -m harness.lib.verify --land --base "$base" || exit $?
exec python3 tools/manifest.py
