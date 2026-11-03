#!/bin/sh
# The only judge of a rebuilt function: compile its unit with the pinned VC6 SP4 compiler, diff
# it against the original bytes, and refuse any score below analysis/results/matched_set.json.
#
#   harness/build-and-verify.sh <function-id> [--behavioural] [--json]
#   harness/build-and-verify.sh --all [--base <file or git ref>]
#
# Read-only: it never writes matched_set.json (harness/update-matched-set.sh does).
# Exit 0 nothing regressed, 1 regression, 2 the claimed function could not be built or scored.
here=$(cd "$(dirname "$0")" && pwd)
for a in "$@"; do
    [ "$a" = --record ] && { echo "ERROR --record is for harness/update-matched-set.sh" >&2; exit 2; }
done
PYTHONPATH="$(dirname "$here")" exec python3 -m harness.lib.verify "$@"
