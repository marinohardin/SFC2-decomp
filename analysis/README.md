# analysis/

First-pass static analysis of the retail binaries (2026-11-02). Scripts need `pip install pefile`.

- `scripts/peinfo.py <exe> [full]` — PE headers, Rich header (compiler builds), imports.
- `scripts/libmatch.py <exe> <lib>...` — matches per-function COMDAT code from the SDK's MSVC 6 static libs
  (`OP API - R2.1/Scripts/Libraries/*.lib`) into the exe's .text byte-for-byte (relocations masked).
  Writes `lib_matches.tsv` = `address  size  mangled_name`.
- `scripts/funccount.py <exe>` — rough function-count heuristics (call targets, data->text pointers).

Results:
- `StarFleetOP_2552_sdklib_matches.tsv` — 3,758 unique name->address matches in the patched 2.5.5.2 game exe.
- `StarFleetOP_2500_sdklib_matches.tsv` — same against the original disc 2.5.0.0 exe (2,989 matches; different optimization).
- `ServerPlatform_sdklib_matches.tsv` — 3,100 matches in the Dynaverse server kit exe.
- `StarFleetOP_2552_strings.txt` — `strings -n 8` of the game exe.
