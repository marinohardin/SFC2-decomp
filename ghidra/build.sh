#!/bin/bash
# P0-3: rebuild the Ghidra project of StarFleetOP.exe and all exports from scratch.
#   ghidra/build.sh            full rebuild (about 11 min on the M1 Pro, see results/timings.tsv)
#   ghidra/build.sh 3 4 5      rerun only the listed steps on the existing project
# Steps: 0 natives+hashes, 1 import/analysis, 1b VC6 RTTI + gap fill + class recovery, 2 SDK FID,
#        3 exact matches, 4 translation units, 5 exports + snapshot.
set -euo pipefail
cd "$(dirname "$0")/scripts"
source ./env.sh

steps=("$@")
[ ${#steps[@]} -eq 0 ] && steps=(0 1 1b 2 3 4 5)

for s in "${steps[@]}"; do
	case "$s" in
	0)
		# retail inputs must match analysis/results/retail_hashes.sha256
		(cd "$REPO/reference" && grep -E "StarFleetOP.exe|Libraries/" "$HASHES" \
			| grep -v SFCOPServer | shasum -a 256 -c -)
		./build_decompiler.sh
		;;
	1)  : > "$RESULTS/timings.tsv"; ./step1_import.sh ;;
	1b) ./step1b_classes.sh ;;
	2)  ./step2_fid.sh ;;
	3)  ./step3_exact.sh ;;
	4)  ./step4_tu.sh ;;
	5)  ./step5_export.sh ;;
	*)  echo "unknown step $s" >&2; exit 2 ;;
	esac
done
