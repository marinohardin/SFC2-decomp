# Progress

Written by `python3 scripts/progress.py --markdown`; do not edit by hand. Generated 2026-11-09T00:47:08Z.

Progress: 0.09% exact, 0.00% functional, 92.42% lifted, 0.07% original x86 of .text (3,751,936 bytes); named: 9.81% real symbol, 39.12% with class, of 27,479 functions. Game-only: 0.10% exact, 0.00% functional of 3,328,192 bytes; 7.59% real symbol, 36.63% with class, of 26,399 functions.

## Code

One row per ordinary function; its bytes include the EH funclets its EH tables reference (16,492 funclets, 167,541 bytes, matched through `eh_funclet_parents.tsv`). `% .text` is over the whole original `.text` (3,751,936 bytes); `% code` is over the bytes of ordinary functions and their funclets. Not classified: 52 funclets with no known parent (555 bytes) and 209 thunks (1,076 bytes); the rest of `.text` is data and padding.

| status | functions | bytes | of which funclets | % .text | % code | game fns | game bytes | % game code |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| exact | 32 | 3,384 | 0 | 0.09 | 0.10 | 32 | 3,384 | 0.10 |
| functional | 0 | 0 | 0 | 0.00 | 0.00 | 0 | 0 | 0.00 |
| lifted | 27,347 | 3,467,722 | 167,541 | 92.42 | 99.82 | 26,267 | 3,322,073 | 99.82 |
| original-x86 | 100 | 2,735 | 0 | 0.07 | 0.08 | 100 | 2,735 | 0.08 |
| all | 27,479 | 3,473,841 | 167,541 | 92.59 | 100.00 | 26,399 | 3,328,192 | 100.00 |

## Names

One tier per function, the first that applies in this order. "Real symbol" is every tier above `rtti-class`; "with class" adds `rtti-class`.

| named by | functions | % | game fns | % game |
| --- | ---: | ---: | ---: | ---: |
| sdk-exact | 1,859 | 6.77 | 1,550 | 5.87 |
| sdk-fid | 217 | 0.79 | 148 | 0.56 |
| vs-fid | 307 | 1.12 | 0 | 0.00 |
| manual | 0 | 0.00 | 0 | 0.00 |
| sdk-fuzzy | 314 | 1.14 | 306 | 1.16 |
| string | 0 | 0.00 | 0 | 0.00 |
| rtti-class | 8,053 | 29.31 | 7,666 | 29.04 |
| none | 16,729 | 60.88 | 16,729 | 63.37 |
| real symbol | 2,697 | 9.81 | 2,004 | 7.59 |
| with class | 10,750 | 39.12 | 9,670 | 36.63 |

Game-only leaves out the functions flagged as library code: crt 304, cryptopp 8, stl 562, wonapi 206. The flag comes from names, so unnamed library code still counts as game code.

## History

- 2026-11-03T11:43:47Z: exact 0.00%, functional 0.00%, lifted 92.52% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 0.00%, functional 0.00%
- 2026-11-09T00:47:08Z: exact 0.09%, functional 0.00%, lifted 92.42% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 0.10%, functional 0.00%
