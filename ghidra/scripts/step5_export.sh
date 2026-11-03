#!/bin/bash
# Step 5: CSV exports, counts, and a reusable snapshot (.gzf of the program + project tarball).
set -euo pipefail
source "$(dirname "$0")/env.sh"

run_headless 05_export "$PROJECTS" "$MAIN_PROJECT" -process StarFleetOP.exe -noanalysis -readOnly \
	-postScript SfcExport.java "$EXPORTS" \
	-postScript SfcSaveGzf.java "$WORK/StarFleetOP_2552.gzf"
grep -h -E "^INFO  Sfc[A-Za-z]+\.java> " "$LOGS/05_export.out" | sed -E 's/ \(GhidraScript\) *$//' \
	> "$RESULTS/export_summary.txt" || true

# Project snapshot: the whole project (exe + imported SDK lib members) for other sessions.
SNAP="$WORK/SFCOP_ghidra_project.tar.gz"
tar -C "$PROJECTS" -czf "$SNAP" "$MAIN_PROJECT.gpr" "$MAIN_PROJECT.rep"
ls -l "$SNAP" "$WORK/StarFleetOP_2552.gzf" "$FIDB" | tee "$RESULTS/snapshot_files.txt"
python3 -I "$SCRIPTS/counts.py" "$REPO" > "$RESULTS/counts.md"
