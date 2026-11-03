#!/bin/bash
# Step 2: Function ID database from the five SDK .lib files, applied to the exe.
#  2a import every COFF member with MSCoffLoader (SfcImportLibs.java, after Ghidra's ImportMSLibs)
#     into /sdklibs/<lib>/ of the main project, then analyse them with Ghidra's FID ingest
#     pre/post scripts (FID, library identification and demanglers off so mangled names survive);
#  2b build $FIDB, one FID library per archive;
#  2c apply it (only it) to StarFleetOP.exe.
set -euo pipefail
source "$(dirname "$0")/env.sh"

LIBCOPY="$WORK/sdk_libs"
rm -rf "$LIBCOPY"
paths=()
for l in "${SDK_LIBS[@]}"; do paths+=("$LIBDIR/$l"); done
python3 -I "$SCRIPTS/prep_libs.py" "$LIBCOPY" "${paths[@]}" | tee "$RESULTS/sdk_libs_prepared.tsv"
copies=()
for l in "${SDK_LIBS[@]}"; do copies+=("$LIBCOPY/$l"); done

run_headless 02a_import_libs "$PROJECTS" "$MAIN_PROJECT" -process StarFleetOP.exe -noanalysis -readOnly \
	-postScript SfcImportLibs.java /sdklibs "$RESULTS/sdk_lib_members.tsv" "${copies[@]}"
run_headless 02a_analyze_libs "$PROJECTS" "$MAIN_PROJECT/sdklibs" -process -recursive \
	-preScript FunctionIDHeadlessPrescript.java \
	-postScript FunctionIDHeadlessPostscript.java

run_headless 02b_fidb "$PROJECTS" "$MAIN_PROJECT" -process StarFleetOP.exe -noanalysis -readOnly \
	-postScript SfcBuildFidb.java "$FIDB" /sdklibs "$RESULTS/fidb_build.tsv" \
	"$GHIDRA_HOME/Ghidra/Features/FunctionID/data/common_symbols_win32.txt"

run_headless 02c_fid_apply "$PROJECTS" "$MAIN_PROJECT" -process StarFleetOP.exe -noanalysis \
	-postScript SfcApplyFid.java "$FIDB" "$RESULTS/fid_sdk_matches.tsv" \
	-postScript SfcStats.java "$RESULTS/stats_02_fid.txt"
rm -rf "$LIBCOPY"   # the members now live in the project under /sdklibs
