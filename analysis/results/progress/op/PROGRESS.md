# Progress

Written by `python3 scripts/progress.py --markdown`; do not edit by hand. Generated 2026-11-16T05:01:14Z.

Progress: 4.46% exact, 0.00% functional, 0.00% lifted, 88.13% original x86 of .text (3,751,936 bytes); named: 11.53% real symbol, 44.94% with class, of 23,071 functions. Game-only: 5.02% exact, 0.00% functional of 3,330,800 bytes; 9.42% real symbol, 42.69% with class, of 22,164 functions.

## Code

One row per ordinary function; its bytes include the EH funclets its EH tables reference (16,492 funclets, 167,541 bytes, matched through `eh_funclet_parents.tsv`). `% .text` is over the whole original `.text` (3,751,936 bytes); `% code` is over the bytes of ordinary functions and their funclets. Not classified: 52 funclets with no known parent (555 bytes) and 209 thunks (1,076 bytes); the rest of `.text` is data and padding.

| status | functions | bytes | of which funclets | % .text | % code | game fns | game bytes | % game code |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| exact | 982 | 167,329 | 1,764 | 4.46 | 4.82 | 982 | 167,329 | 5.02 |
| functional | 0 | 0 | 0 | 0.00 | 0.00 | 0 | 0 | 0.00 |
| lifted | 0 | 0 | 0 | 0.00 | 0.00 | 0 | 0 | 0.00 |
| original-x86 | 22,089 | 3,306,512 | 165,777 | 88.13 | 95.18 | 21,182 | 3,163,471 | 94.98 |
| all | 23,071 | 3,473,841 | 167,541 | 92.59 | 100.00 | 22,164 | 3,330,800 | 100.00 |

## Names

One tier per function, the first that applies in this order. "Real symbol" is every tier above `rtti-class`; "with class" adds `rtti-class`.

| named by | functions | % | game fns | % game |
| --- | ---: | ---: | ---: | ---: |
| sdk-exact | 1,859 | 8.06 | 1,550 | 6.99 |
| sdk-fid | 217 | 0.94 | 148 | 0.67 |
| vs-fid | 267 | 1.16 | 84 | 0.38 |
| manual | 2 | 0.01 | 2 | 0.01 |
| sdk-fuzzy | 315 | 1.37 | 303 | 1.37 |
| string | 0 | 0.00 | 0 | 0.00 |
| rtti-class | 7,709 | 33.41 | 7,375 | 33.27 |
| none | 12,702 | 55.06 | 12,702 | 57.31 |
| real symbol | 2,660 | 11.53 | 2,087 | 9.42 |
| with class | 10,369 | 44.94 | 9,462 | 42.69 |

Game-only leaves out the functions flagged as library code: crt 180, cryptopp 8, stl 523, wonapi 196. The flag comes from names, so unnamed library code still counts as game code.

## History

- 2026-11-03T11:43:47Z: exact 0.00%, functional 0.00%, lifted 92.52% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 0.00%, functional 0.00%
- 2026-11-09T00:47:08Z: exact 0.09%, functional 0.00%, lifted 92.42% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 0.10%, functional 0.00%
- 2026-11-10T04:47:55Z: exact 0.36%, functional 0.00%, lifted 92.16% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 0.41%, functional 0.00%
- 2026-11-11T12:47:02Z: exact 1.90%, functional 0.00%, lifted 90.62% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 2.14%, functional 0.00%
- 2026-11-16T05:01:14Z: exact 4.46%, functional 0.00%, lifted 0.00% of .text; real symbol 11.53%, with class 44.94% of 23,071 functions; game-only exact 5.02%, functional 0.00%
