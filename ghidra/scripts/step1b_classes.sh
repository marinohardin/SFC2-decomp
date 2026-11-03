#!/bin/bash
# Step 1b: VC6 RTTI markup (SfcVc6Rtti.java; Ghidra's analyzer misses VC6 COLs/vftables), function
# creation in undefined .text gaps (SfcFillGaps.java), then Ghidra's RecoverClassesFromRTTIScript
# (class structures, vftables, ctor/dtor identification).
set -euo pipefail
source "$(dirname "$0")/env.sh"

run_headless 01b_classes "$PROJECTS" "$MAIN_PROJECT" -process "StarFleetOP.exe" -noanalysis \
	-preScript SfcCheckNatives.java \
	-postScript SfcVc6Rtti.java "$RESULTS/vc6_vftables.tsv" \
	-postScript SfcFillGaps.java "$RESULTS/gap_fill.tsv" \
	-postScript SfcStats.java "$RESULTS/stats_01b_rtti_gaps.txt" \
	-postScript RecoverClassesFromRTTIScript.java \
	-postScript SfcStats.java "$RESULTS/stats_01b_classes.txt"
grep -h -E "SfcVc6Rtti.java>|SfcFillGaps.java>|RecoverClassesFromRTTIScript.java> (Identified|Total)" \
	"$LOGS/01b_classes.script.log" | sed -E 's/^[0-9-]+ [0-9:]+ INFO  //; s/ +$//' \
	> "$RESULTS/class_recovery_summary.txt"
