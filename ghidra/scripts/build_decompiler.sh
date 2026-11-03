#!/bin/bash
# The Homebrew Ghidra 12.2 ships no macOS arm64 native decompiler (os/mac_arm_64/decompile),
# which switch-table recovery and RecoverClassesFromRTTIScript need. Build it from the C++
# sources inside the install and expose it as a Ghidra extension in our isolated settings dir,
# so the Homebrew tree and ~/Library/ghidra stay untouched.
set -euo pipefail
source "$(dirname "$0")/env.sh"

GHIDRA_VER=$(awk -F= '/application.version/ {print $2}' "$GHIDRA_HOME/Ghidra/application.properties")
EXT="$WORK/settings/$(whoami)-ghidra/ghidra_${GHIDRA_VER}_PUBLIC/Extensions/SfcNatives"
BIN="$EXT/os/mac_arm_64/decompile"
if [ -x "$BIN" ] && [ -z "${FORCE:-}" ]; then
	echo "decompiler already built: $BIN" >&2
	exit 0
fi

B="$WORK/natives-build"
rm -rf "$B"
mkdir -p "$B"
cp -R "$GHIDRA_HOME/Ghidra/Features/Decompiler/src/decompile" "$B/"
make -C "$B/decompile/cpp" -j4 ghidra_opt \
	CXX="clang++ -std=c++11" ARCH_TYPE="-arch arm64" \
	ADDITIONAL_FLAGS="-mmacosx-version-min=11.0 -w" > "$LOGS/build_decompiler.log" 2>&1

mkdir -p "$(dirname "$BIN")"
cp "$B/decompile/cpp/ghidra_opt" "$BIN"
chmod +x "$BIN"
cat > "$EXT/extension.properties" <<EOF
name=SfcNatives
description=Locally built mac_arm_64 decompiler for the SFCOP P0-3 pipeline
author=SFCOP-decomp
createdOn=$(date +%Y-%m-%d)
version=$GHIDRA_VER
EOF
touch "$EXT/Module.manifest"
rm -rf "$B"
echo "built $BIN" >&2
