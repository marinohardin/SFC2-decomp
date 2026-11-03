#!/bin/sh
# The only writer of analysis/results/matched_set.json. It takes no score: it verifies the function
# itself, exactly as build-and-verify.sh does, and stores the result only when it beats the
# recorded one and nothing else in the unit regressed. Called by harness/loop.sh after a session;
# the hooks in harness/hooks/ deny it to the agent.
#
#   harness/update-matched-set.sh <function-id>...
here=$(cd "$(dirname "$0")" && pwd)
[ $# -gt 0 ] || { echo "usage: $0 <function-id>..." >&2; exit 2; }
PYTHONPATH="$(dirname "$here")" exec python3 -m harness.lib.verify --record "$@"
