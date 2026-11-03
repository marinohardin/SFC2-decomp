#!/bin/bash
# Step 4: translation units from source-path strings (assertions, logging) -> TU:/HDR: tags.
set -euo pipefail
source "$(dirname "$0")/env.sh"

run_headless 04_tu "$PROJECTS" "$MAIN_PROJECT" -process StarFleetOP.exe -noanalysis \
	-postScript SfcMapTU.java "$RESULTS/tu_paths.tsv" \
	-postScript SfcStats.java "$RESULTS/stats_04_tu.txt"
grep -h "SfcMapTU.java>" "$LOGS/04_tu.script.log" | sed -E 's/^[0-9-]+ [0-9:]+ INFO  //; s/ +$//' \
	> "$RESULTS/tu_summary.txt"
