# Symbol table

`STARFLEETOP.tsv` has one row per function of the original (Orion Pirates 2.5.5.2): every
function and thunk of `ghidra/exports/functions.csv`, so nobody invents a name for a function
that is not decompiled yet. Exception funclets are not functions and have no row. It is not
part of the interface (`docs/INTERFACE.md`) yet.

Tab-separated, one header line, sorted by address, LF, no quoting.

| Column | Meaning |
| --- | --- |
| `address` | `0x` and 8 lowercase hex digits |
| `kind` | `function`, `thunk`, or `folded` (a second real name for a body the original folded into the address above it) |
| `name` | The original identifier when evidence gives one, else `fn_<address>` |
| `class` | The class the evidence or the vtable export gives; `tVtbl_<vtable address>` for a vtable class that has no evidence name; else empty |
| `symbol` | Level 2: the MSVC mangled name a call to this function must use; empty until known |
| `symbol_evidence` | Where `symbol` came from; empty when `symbol` is empty. `claim`: what the claiming unit compiled, which always tracks the build (a seed replaces any other value of a claimed address, and `add` refuses a claimed row); `mangled`: functions.csv; `callers`: every exact caller's relocation names it; `add`: written by `tools/symbols.py add`. A `callers` symbol is provisional: `add` may replace it with another (evidence becomes `add`); `mangled` and `add` values are first-writer-wins |
| `declaration` | Level 2: optional C++ declaration text; empty until known |
| `evidence` | Where `name` came from: `claim` (the symbol the claiming unit compiled), `mangled` (functions.csv), `sdk` (an SDK library match), `rtti`, `fid` (Ghidra function id); `vtable` and `none` mean the name is the placeholder `fn_<address>` (`vtable`: the class is known from a vtable slot) |

A placeholder is never evidence: a name or class component such as `fn_<address>`, `FUN_<address>`,
`tObject_<address>`, `c_<address>`, `c_<Class>_Field<N>` or `vfunction<N>` is not recorded as a
name (`tVtbl_<address>` is the one placeholder class, naming a vtable that has no evidence name).

Level 1 (`address` to `evidence`) is complete from the start. Level 2 (`symbol`, `declaration`)
fills in as it becomes known. One address has one primary row; only a `folded` row (for later
hand or tool use; the seed writes none) may repeat an address, and one symbol appears once in
the whole table.

## Who edits it

The table is edited through `tools/symbols.py` only, never by hand:

```
python3 tools/symbols.py check
python3 tools/symbols.py add <address> <symbol> [--declaration TEXT]
python3 tools/symbols.py show <address>...
```

`add` fills level 2 for a row whose `symbol` is empty; the first writer wins. The first fill
of the whole table is `analysis/scripts/seed_symbols.py`, which reads a finished
`harness/build-and-verify.sh --all` build; a re-run keeps the values `add` wrote.

## When you add, rename or remove a claim

A claimed row's `symbol` is what the claiming unit compiled, so it changes with the claims. After
`harness/build-and-verify.sh --all`, run `python3 tools/symbols.py claim [<address>...]`: it sets
each current claim's row to the build's symbol (evidence `claim`), replacing a `callers` value
and printing each replacement, and, with no address, resets a `claim` row whose address is no
longer claimed. A `mangled` or `add` symbol that differs from the build's is not replaced: `claim`
prints an `ERROR` naming the address, both symbols and the evidence, and exits 1, unless
`--replace-evidence` is given. `check`, with a current build present (one made from this tree),
fails until the table agrees with it.

## The data symbols: `STARFLEETOP.data.tsv`

`STARFLEETOP.tsv` has function rows only. Objects built from `op/` that define a polymorphic class's
constructor or destructor also emit run-time type information, and the table gives a program that
places symbols by evidence from the executable no address for it. `STARFLEETOP.data.tsv` holds
those addresses. Like the table, it is not part of the declared interface (`docs/INTERFACE.md`);
it joins it on the same footing, and a program that builds on this repository may use it to place
data symbols.

Tab-separated, `#` comment lines first, then one header line (`address`, `kind`, `symbol`,
`evidence`), sorted by address, LF, no quoting.

| Column | Meaning |
| --- | --- |
| `address` | `0x` and 8 lowercase hex digits |
| `kind` | `type_descriptor` (`??_R0`: the record starting with the `type_info` vftable pointer) or `vftable` (`??_7`: address of the first slot) |
| `symbol` | The decorated symbol an object's definition has: `??_R0` + the descriptor's name string without its dot + `@8` (`.?AVfoo@@` gives `??_R0?AVfoo@@@8`; builtin and enum names derive the same way); `??_7<class>@@6B@`, or `??_7<class>@@6B<base>@@@` for a class's vftable at a nonzero offset |
| `evidence` | `rtti`: found in the executable by structure, not by a name list |

Not in it: `??_R1` to `??_R4` (the records an object defines for itself; their addresses are
derivable, but their decorated names are not written yet), the exception type tables
(`__TI...`), and any vftable whose name cannot be derived with certainty: a class with several
vftables gets a row for the one at offset 0 and for one at another offset only when exactly one
base of the class's `??_R2` array sits at that offset without a virtual base; the rest are left out
(99 of 2149 at the time of writing, all at nonzero offsets, plus one vftable that two places
reference). A class the original has no descriptor for has no row, whatever a source file
declares (a placeholder class, or `struct` where the original has `class`).

`python3 tools/rtti_symbols.py generate` writes it from the executable (after checking the
executable's hash against `analysis/results/retail_hashes.sha256`); `python3 tools/rtti_symbols.py
check` regenerates it in memory and fails when the committed file differs, and prints an INFO line
and passes where there is no install. `scripts/check.sh` runs `check`.
