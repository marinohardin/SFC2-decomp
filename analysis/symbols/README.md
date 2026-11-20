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
each current claim's row to the build's symbol (evidence `claim`), replacing any `mangled`,
`callers` or `add` value and printing each replacement, and, with no address, resets a `claim`
row whose address is no longer claimed. `check`, with a build present, fails until you have.
