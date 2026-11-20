"""Keeps the symbol table current after a function session improved (harness/loop.sh), by the
tool and never by the model. Run before the loop commits the round:

    python3 -m harness.lib.symsync <build-and-verify --json output> <address>

1. The claims of the units the session's run rebuilt: `tools/symbols.py claim --results`, so each
   row of a claimed function carries the symbol the unit compiled now (evidence `claim`).
2. The unclaimed functions the session's function calls: a function whose row has no symbol gets
   the symbol its relocation names (evidence `add`, `tools/symbols.py add`), but only when that
   symbol passes the rule the seed uses for callers (tools/symbols.callee_verdict: a real name,
   or exactly fn_<the target's own address>, free or a member of a class that is not a
   placeholder). A symbol that fails the rule is not written and gives one WARN line; so does a
   row that already has another `mangled` or `add` symbol (the session was refused for it in
   build-and-verify, and a Stop hook that let it through after three refusals does not write it).

After its edits it runs `tools/symbols.py check` on the table; on a finding it restores the
table's earlier content, prints ERROR lines and exits 1. Every other line starts with INFO or
WARN; the exit status is 0.

    python3 -m harness.lib.symsync --scratch

is the whole-tree step's check: claim (all) then check, on a copy of the table; exit 1 on a finding.
"""
import contextlib
import io
import json
import os
import sys

from . import paths, symtab, verify

REQUIRED = ('mangled', 'add')


def own_result(text, address):
    """The result of `address` in a build-and-verify --json output (text may start with other
    lines), or None; and every result."""
    try:
        results = json.loads(text[text.index('{'):])['results']
    except (ValueError, KeyError):
        return None, []
    return next((r for r in results if r.get('address') == address), None), results


def callee_symbols(res):
    """[(target address text, symbol)] of the relocations of a result that reach a function by a
    name a table row may hold: resolved, in the original, not per unit, in call order."""
    out, seen = [], set()
    for r in res.get('relocs') or []:
        sym, addr = r.get('symbol') or '', r.get('exe_address')
        if (r.get('type') not in verify.CLAIM_RELOC_TYPES or not addr or r.get('status') == 'mismatch'
                or sym.startswith(verify.PER_UNIT_PREFIXES + ('__imp_', '_imp_'))
                or (addr, sym) in seen):
            continue
        seen.add((addr, sym))
        out.append((addr, sym))
    return out


def run_tool(tool, argv, table):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        code = tool.main((['--table', table] if table else []) + argv)
    return code, [x for x in buf.getvalue().splitlines() if x]


def sync(path, address, table=None, src_root=None):
    """The lines to print for the build-and-verify --json output in the file `path` of the
    session on `address`."""
    tool = symtab.tool()
    with open(path, encoding='utf-8', errors='replace') as f:
        text = f.read()
    table = table or paths.SYMBOL_TABLE
    table_path = table
    with open(table_path, encoding='utf-8', newline='') as f:
        previous = f.read()
    res, _ = own_result(text, address)
    lines = []
    if res is None:
        return [f'WARN  symbol table not updated: no result for {address} in the build output']
    src = ['--src-root', src_root] if src_root else []
    code, out = run_tool(tool, ['claim', '--results', path] + src, table)
    lines += [f'INFO  symbol table: {x}' for x in out] if code == 0 else \
        [f'WARN  symbol table: claim sync failed: {x}' for x in out]
    rows = {}
    for r in tool.read_rows(table):
        if r['kind'] in ('function', 'thunk'):
            rows[r['address']] = r
    claimed = tool.current_claims(src_root or paths.SRC_ROOT)
    for target, sym in callee_symbols(res):
        row = rows.get(target)
        if row is None or target in claimed or row['symbol_evidence'] == 'claim':
            continue
        if row['symbol'] == sym:
            continue
        if row['symbol']:
            if row['symbol_evidence'] in REQUIRED:
                lines.append(f'WARN  symbol table: {target} has {row["symbol"]} '
                             f'({row["symbol_evidence"]}); {address} names {sym}: not written')
            continue
        if not tool.plausible_symbol(sym):
            lines.append(f'WARN  symbol table: {sym} for {target} is not written: it is not a '
                         'plausible function symbol')
            continue
        ok, why = tool.callee_verdict(sym, int(target, 16))
        if not ok:
            lines.append(f'WARN  symbol table: {sym} for {target} is not written ({why})')
            continue
        code, out = run_tool(tool, ['add', target, sym, '--callee'], table)
        if code == 0:
            lines.append(f'INFO  symbol table: {target} gets {sym} (add)')
        else:
            lines += [f'WARN  symbol table: {x}' for x in out]
    bad = checked(tool, table_path, src_root)
    if bad:
        with open(table_path, 'w', encoding='utf-8', newline='') as f:
            f.write(previous)
        lines += [f'ERROR symbol table: {x}' for x in bad]
        lines.append('ERROR symbol table: restored to its content before this sync')
    return lines


def checked(tool, table, src_root=None):
    """The ERROR lines of `tools/symbols.py check` on the table, without a build to compare (the
    session's run is newer than the build folder's symbols.tsv); [] when it is clean."""
    scratch = os.path.join(paths.STATE, 'symsync-build')
    os.makedirs(scratch, exist_ok=True)
    code, out = run_tool(tool, ['check', '--functions', paths.FUNCTIONS_CSV, '--build', scratch,
                                '--src-root', src_root or paths.SRC_ROOT], table)
    return [x for x in out if x.startswith('ERROR')] or (['check failed'] if code else [])


def scratch_check():
    """`tools/symbols.py claim` for every claim, then `check`, on a copy of the committed table in
    the state folder (the whole-tree step). Prints what `check` reports; 0 when it is clean."""
    import shutil
    tool = symtab.tool()
    os.makedirs(paths.STATE, exist_ok=True)
    copy = os.path.join(paths.STATE, 'wholetree-symbols.tsv')
    shutil.copyfile(paths.SYMBOL_TABLE, copy)
    for cmd in ('claim', 'check'):
        code, out = run_tool(tool, [cmd], copy)
        if code != 0:
            print('\n'.join(out))
            return 1
    return 0


def main(argv):
    if argv == ['--scratch']:
        return scratch_check()
    if len(argv) != 2:
        print(__doc__)
        return 2
    lines = sync(argv[0], argv[1])
    for line in lines:
        print(line)
    return 1 if any(x.startswith('ERROR') for x in lines) else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
