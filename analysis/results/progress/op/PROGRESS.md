# Progress

Written by `python3 scripts/progress.py --markdown`; do not edit by hand. Generated 2026-11-20T04:54:37Z.

Progress: 14.24% exact, 0.00% functional, 4.55% partial, 0.00% lifted, 73.80% original x86 of .text (3,751,936 bytes); named: 13.68% real symbol, 45.58% with class, of 23,071 functions. Game-only: 16.04% exact, 0.00% functional, 5.13% partial of 3,330,800 bytes; 11.66% real symbol, 43.36% with class, of 22,164 functions.

## Code

One row per ordinary function; its bytes include the EH funclets its EH tables reference (16,492 funclets, 167,541 bytes, matched through `eh_funclet_parents.tsv`). `% .text` is over the whole original `.text` (3,751,936 bytes); `% code` is over the bytes of ordinary functions and their funclets. Not classified: 52 funclets with no known parent (555 bytes) and 209 thunks (1,076 bytes); the rest of `.text` is data and padding.

| status | functions | bytes | of which funclets | % .text | % code | game fns | game bytes | % game code |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| exact | 4,196 | 534,191 | 11,243 | 14.24 | 15.38 | 4,196 | 534,191 | 16.04 |
| functional | 0 | 0 | 0 | 0.00 | 0.00 | 0 | 0 | 0.00 |
| partial | 401 | 170,783 | 8,853 | 4.55 | 4.92 | 401 | 170,783 | 5.13 |
| lifted | 0 | 0 | 0 | 0.00 | 0.00 | 0 | 0 | 0.00 |
| original-x86 | 18,474 | 2,768,867 | 147,445 | 73.80 | 79.71 | 17,567 | 2,625,826 | 78.83 |
| all | 23,071 | 3,473,841 | 167,541 | 92.59 | 100.00 | 22,164 | 3,330,800 | 100.00 |

## Names

One tier per function, the first that applies in this order. "Real symbol" is every tier above `rtti-class`; "with class" adds `rtti-class`.

| named by | functions | % | game fns | % game |
| --- | ---: | ---: | ---: | ---: |
| sdk-exact | 1,859 | 8.06 | 1,550 | 6.99 |
| sdk-fid | 217 | 0.94 | 148 | 0.67 |
| vs-fid | 265 | 1.15 | 83 | 0.37 |
| manual | 2 | 0.01 | 2 | 0.01 |
| sdk-fuzzy | 315 | 1.37 | 303 | 1.37 |
| string | 0 | 0.00 | 0 | 0.00 |
| claim | 498 | 2.16 | 498 | 2.25 |
| rtti-class | 7,360 | 31.90 | 7,026 | 31.70 |
| none | 12,555 | 54.42 | 12,554 | 56.64 |
| real symbol | 3,156 | 13.68 | 2,584 | 11.66 |
| with class | 10,516 | 45.58 | 9,610 | 43.36 |

Game-only leaves out the functions flagged as library code: crt 180, cryptopp 8, stl 523, wonapi 196. The flag comes from names, so unnamed library code still counts as game code.

## History

- 2026-11-03T11:43:47Z: exact 0.00%, functional 0.00%, lifted 92.52% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 0.00%, functional 0.00%
- 2026-11-09T00:47:08Z: exact 0.09%, functional 0.00%, lifted 92.42% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 0.10%, functional 0.00%
- 2026-11-10T04:47:55Z: exact 0.36%, functional 0.00%, lifted 92.16% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 0.41%, functional 0.00%
- 2026-11-11T12:47:02Z: exact 1.90%, functional 0.00%, lifted 90.62% of .text; real symbol 9.81%, with class 39.12% of 27,479 functions; game-only exact 2.14%, functional 0.00%
- 2026-11-16T05:01:14Z: exact 4.46%, functional 0.00%, lifted 0.00% of .text; real symbol 11.53%, with class 44.94% of 23,071 functions; game-only exact 5.02%, functional 0.00%
- 2026-11-16T18:30:47Z: exact 4.56%, functional 0.00%, partial 1.72%, lifted 0.00% of .text; real symbol 11.53%, with class 44.94% of 23,071 functions; game-only exact 5.13%, functional 0.00%, partial 1.93%
- 2026-11-17T04:19:10Z: exact 8.40%, functional 0.00%, partial 3.25%, lifted 0.00% of .text; real symbol 11.53%, with class 44.94% of 23,071 functions; game-only exact 9.46%, functional 0.00%, partial 3.66%
- 2026-11-19T02:41:04Z: exact 13.12%, functional 0.00%, partial 4.17%, lifted 0.00% of .text; real symbol 11.52%, with class 44.94% of 23,071 functions; game-only exact 14.78%, functional 0.00%, partial 4.70%
- 2026-11-19T07:05:13Z: exact 13.12%, functional 0.00%, partial 4.17%, lifted 0.00% of .text; real symbol 13.09%, with class 45.53% of 23,071 functions; game-only exact 14.78%, functional 0.00%, partial 4.70%
- 2026-11-19T07:05:37Z: exact 13.12%, functional 0.00%, partial 4.17%, lifted 0.00% of .text; real symbol 13.09%, with class 45.53% of 23,071 functions; game-only exact 14.78%, functional 0.00%, partial 4.70%
- 2026-11-20T04:54:37Z: exact 14.24%, functional 0.00%, partial 4.55%, lifted 0.00% of .text; real symbol 13.68%, with class 45.58% of 23,071 functions; game-only exact 16.04%, functional 0.00%, partial 5.13%
