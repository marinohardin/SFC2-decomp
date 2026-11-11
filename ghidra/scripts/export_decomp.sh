#!/bin/bash
# Decompiled pseudo-C for the grind harness's context pack: one file per function in
# $WORK/decomp/<0xaddress>.c (harness/lib/pack.py reads it; HARNESS_DECOMP overrides the path).
#
#   ghidra/scripts/export_decomp.sh                 # every non-library function in functions.csv
#   ghidra/scripts/export_decomp.sh ADDRESSES.txt   # only these, one hex address per line
#
# Opens the project read-only with no analysis, so it changes nothing, and skips functions already
# exported: an interrupted run resumes. Runs at the lowest priority with the heap and cores of
# env.sh. The first full run takes long (see harness/README.md, "Context pack"); run it in the
# background. It does not write ghidra/results/.
set -euo pipefail
source "$(dirname "$0")/env.sh"

OUT="${HARNESS_DECOMP:-$WORK/decomp}"
mkdir -p "$OUT"
list="${1:-}"
if [ -z "$list" ]; then
	list="$WORK/decomp-addresses.txt"
	python3 -I - "$EXPORTS/functions.csv" > "$list" <<'EOF'
import csv, sys
for r in csv.DictReader(open(sys.argv[1], newline='')):
    if r['kind'] == 'function' and r['is_library'] != '1' and int(r['size'] or 0) > 0:
        print(r['address'])
EOF
fi
echo "[$(date '+%H:%M:%S')] export_decomp: $(grep -c . "$list") addresses -> $OUT" >&2
t0=$(date +%s)
rc=0
nice -n 19 "$HEADLESS" "$PROJECTS" "$MAIN_PROJECT" -process StarFleetOP.exe -noanalysis -readOnly \
	-scriptPath "$SCRIPTS" -postScript SfcExportDecomp.java "$OUT" "$list" \
	-log "$LOGS/export_decomp.log" -scriptlog "$LOGS/export_decomp.script.log" \
	> "$LOGS/export_decomp.out" 2>&1 || rc=$?
grep -h "SfcExportDecomp:" "$LOGS/export_decomp.out" | tail -1 >&2 || true
echo "[$(date '+%H:%M:%S')] export_decomp: rc=$rc in $(( $(date +%s) - t0 )) s" >&2
if grep -q "REPORT SCRIPT ERROR" "$LOGS/export_decomp.out"; then
	echo "export_decomp: script error, see $LOGS/export_decomp.out" >&2; exit 1
fi
exit $rc
