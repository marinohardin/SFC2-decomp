# Shared settings for the P0-3 Ghidra pipeline. Sourced by build.sh and the step scripts.
# Override any of these from the environment.

REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
GHIDRA_DIR="$REPO/ghidra"
SCRIPTS="$GHIDRA_DIR/scripts"
EXPORTS="$GHIDRA_DIR/exports"      # deliverable CSVs
RESULTS="$GHIDRA_DIR/results"      # per-step logs/tables small enough to keep in the repo

GHIDRA_HOME="${GHIDRA_HOME:-/opt/homebrew/Cellar/ghidra/12.2/libexec}"
HEADLESS="$GHIDRA_HOME/support/analyzeHeadless"

WORK="${WORK:-$SFCOP_ROOT/work/ghidra}"
PROJECTS="$WORK/projects"
LOGS="$WORK/logs"
MAIN_PROJECT="SFCOP"          # the analysed exe
LIBS_PROJECT="sdklibs"        # imported .lib members used to build the fidb
FIDB="$WORK/sfcop_sdk.fidb"

GAME="$REPO/reference/installed game/Starfleet Command Orion Pirates"
EXE="$GAME/StarFleetOP.exe"
LIBDIR="$GAME/OP API - R2.1/Scripts/Libraries"
SDK_LIBS=(ScriptInterfaceR.lib SharedR.lib cryptlibR.lib dmp3D_MSVCR.lib sfcTiger_MSVCR.lib)
TSV="$REPO/analysis/results/StarFleetOP_2552_sdklib_matches.tsv"
HASHES="$REPO/analysis/results/retail_hashes.sha256"

# At most 8 GB heap and 4 cores: several sessions share this machine.
export GHIDRA_HEADLESS_MAXMEM="${GHIDRA_HEADLESS_MAXMEM:-8G}"
# Isolated user settings/cache/temp so the custom fidb registration never touches
# ~/Library/ghidra (which other sessions may use) and nothing large lands on the internal disk.
export GHIDRA_HEADLESS_JAVA_OPTIONS="-Dapplication.settingsdir=$WORK/settings -Dapplication.cachedir=$WORK/cache -Dapplication.tempdir=$WORK/tmp -Djava.io.tmpdir=$WORK/tmp -Dcpu.core.limit=${CORES:-4}"

mkdir -p "$PROJECTS" "$LOGS" "$WORK/settings" "$WORK/cache" "$WORK/tmp" "$EXPORTS" "$RESULTS"

# run_headless <logname> <args...>: analyzeHeadless with time/peak-RSS capture.
run_headless() {
	local name="$1"; shift
	echo "[$(date '+%H:%M:%S')] $name: analyzeHeadless $*" >&2
	rm -f "$LOGS/$name".*
	local rc=0
	/usr/bin/time -l "$HEADLESS" "$@" -scriptPath "$SCRIPTS" \
		-log "$LOGS/$name.log" -scriptlog "$LOGS/$name.script.log" \
		> "$LOGS/$name.out" 2> "$LOGS/$name.time" || rc=$?
	local secs rss
	secs=$(awk '/real/ {print $1; exit}' "$LOGS/$name.time")
	rss=$(awk '/maximum resident set size/ {printf "%.0f", $1/1048576; exit}' "$LOGS/$name.time")
	echo -e "$name\t${secs}s\t${rss}MB\trc=$rc" | tee -a "$RESULTS/timings.tsv" >&2
	if grep -q "REPORT SCRIPT ERROR" "$LOGS/$name.out" 2>/dev/null; then
		echo "$name: script error, see $LOGS/$name.out" >&2; return 1
	fi
	return $rc
}
