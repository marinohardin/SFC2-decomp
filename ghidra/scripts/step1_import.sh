#!/bin/bash
# Step 1: import StarFleetOP.exe into $WORK/projects/SFCOP and run full auto-analysis
# (RTTI analyzer and Ghidra's built-in VS Function ID databases on).
set -euo pipefail
source "$(dirname "$0")/env.sh"

rm -rf "$PROJECTS/$MAIN_PROJECT.gpr" "$PROJECTS/$MAIN_PROJECT.rep"
run_headless 01_import "$PROJECTS" "$MAIN_PROJECT" -import "$EXE" \
	-loader PeLoader -processor x86:LE:32:default -cspec windows \
	-preScript SfcCheckNatives.java -preScript SfcAnalysisOptions.java \
	-postScript SfcStats.java "$RESULTS/stats_01_import.txt"
