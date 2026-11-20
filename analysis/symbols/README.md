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
| `declaration` | Level 2: optional C++ declaration text; empty until known |
| `evidence` | Where `name` came from: `claim` (the symbol the claiming unit compiled), `mangled` (functions.csv), `sdk` (an SDK library match), `rtti`, `fid` (Ghidra function id), `other`; `vtable` and `none` mean the name is the placeholder `fn_<address>` (`vtable`: the class is known from a vtable slot) |

Level 1 (`address` to `evidence`) is complete from the start. Level 2 (`symbol`, `declaration`)
fills in as it becomes known. One address has one primary row; only a `folded` row may repeat
an address, and one symbol is at one address unless a folded row is involved.

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
