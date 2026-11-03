#!/bin/bash
# Step 3: apply the byte-exact SDK matches (analysis/results/StarFleetOP_2552_sdklib_matches.tsv);
# exact matches override FID and log every conflict.
set -euo pipefail
source "$(dirname "$0")/env.sh"

run_headless 03_exact "$PROJECTS" "$MAIN_PROJECT" -process StarFleetOP.exe -noanalysis \
	-postScript SfcApplyExact.java "$TSV" "$RESULTS/exact_apply.tsv" \
	-postScript SfcStats.java "$RESULTS/stats_03_exact.txt"
