#!/bin/sh
# Fetch the VC6 VC98/Lib/ sets into $SFCOP_ROOT/tools/vc6-lib/<set> (not the repo: Microsoft EULA)
# and check them against the manifests here. See PROVENANCE.md.
#   fetch.sh                  fetch any missing set, then verify both
#   fetch.sh --write-manifest regenerate <set>.sha256 from what is on disk (after a deliberate change)
set -eu
here=$(cd "$(dirname "$0")" && pwd)
sfcop=$(cd "$here/../../.." && pwd)
tools=$sfcop/tools/vc6-lib
clone=$sfcop/work/src/MSVC600
url=https://github.com/itsmattkc/MSVC600

if [ ! -d "$clone/.git" ]; then
	git clone --filter=blob:none --no-checkout "$url" "$clone"
fi
for spec in initial:17bae53fa330df7b576f25f1f604e4214076a650 sp6:001c4bafdcf2ef4b474d693acccd35a91e848f40; do
	set_=${spec%%:*} sha=${spec#*:}
	if [ ! -d "$tools/$set_" ]; then
		git -C "$clone" fetch --filter=blob:none origin "$sha"
		mkdir -p "$tools/$set_"
		git -C "$clone" archive "$sha" VC98/Lib | tar -x -C "$tools/$set_" --strip-components 2
	fi
	if [ "${1:-}" = --write-manifest ]; then
		(cd "$tools/$set_" && find . -type f | sed 's|^\./||' | LC_ALL=C sort | xargs shasum -a 256) > "$here/$set_.sha256"
	fi
	(cd "$tools/$set_" && shasum -a 256 -c --quiet "$here/$set_.sha256") && echo "$set_: $(wc -l < "$here/$set_.sha256" | tr -d ' ') files verified"
done
