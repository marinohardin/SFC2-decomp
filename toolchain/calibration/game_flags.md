# The game's own compile flags

The game's code and the libraries it links were not built with the same flags. The game's own
objects were built for size (`/O1`); the libraries were built with the SDK's Release flags
(`/O2`), which `report.md` reproduces byte for byte. Every other flag is the same in both
profiles of `harness/units.json`: `game-release` and `sdk-release`.

Reproduce both tables with:

```sh
python3 toolchain/calibration/game_flags.py scan
python3 toolchain/calibration/game_flags.py probe   # needs the toolchain container
```

## Where the game's code ends

The game's objects fill the image from `0x00401000` to `0x0060f5dd`. From `0x0060f600`
(`GAME_END` in `tools/units.py`) the libraries follow: Tiger, dmp3d, Common, Switch and the
other SDK code. The runtime libraries start near `0x00730000`. Every unit with SDK library
evidence lies above `GAME_END` (`tests/test_units.py` checks the committed map). No unit runs across it.
`tools/units.py` splits the one 4 KB page that does, `0x0060f000`, at that address.

`scan` output (functions of 8 bytes or more, runtime libraries excluded):

| side | functions | KB | 16-byte aligned | fldz_store per KB | push_pop per KB | and_zero per KB | or_minus_one per KB | mov_zero per KB |
|---|---|---|---|---|---|---|---|---|
| game (below GAME_END) | 18977 | 1994 | 6% | 0.44 | 0.41 | 2.49 | 1.60 | 0.00 |
| libraries (GAME_END to 0x00730000) | 5307 | 1057 | 99% | 0.00 | 0.00 | 0.00 | 0.00 | 1.66 |

The idioms are VC6's size optimiser's:

- `fldz; fstp [m]` for a float zero, where `/O2` stores an integer zero.
- `push imm8; pop reg` for a small constant.
- `and dword [m], 0` and `or dword [m], -1` for stores of 0 and -1, where `/O2` uses `mov`.
- No 16-byte padding between functions.

The two sides do not overlap on any of them.

## Probes

`game_flags/probe.cpp` holds three small functions of the game's own code, one per idiom. Each
is compiled under both profiles and compared with the exe. Relocation fields are masked on both
sides, so "exact" means every byte but the 4-byte fixups (the probes' globals are placeholders):

| profile | flags | 0x0040c832 | 0x0040f975 | 0x00419c87 |
|---|---|---|---|---|
| game-release | `/O1` | exact | exact | exact |
| sdk-release | `/O2` | differs | differs | differs |

One-off observations, not reproduced by the script: a full constructor of the game's code
(`0x0040fefb`, 97 instructions with float members) gave the same result: exact under `/O1`,
11.93% under `/O2`. `/O1 /Ob1` is also exact. `/O1 /Oy-` and `/Os /Og /Oi /Ob1` reached
89.90%.

## Not settled

- The runtime libraries above `0x00730000` also show size idioms. They are library code, are
  not queued, and stay on `sdk-release` until a unit there is worked on.
- A unit below `GAME_END` that cannot match under `game-release` is evidence of its own flags,
  recorded per unit as `harness/SRC_LAYOUT.md` describes.
