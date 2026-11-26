"""A function that is exact at the branch's base keeps its symbol.

A rename of an exact function (a layout pass that renames a virtual function in a header, say)
is consistent inside the lane, so the lane's own checks pass, and breaks only at landing, when the
symbol table's row for the address and every other unit's calls to it name the old symbol. This
check finds it inside the lane: for every address `analysis/results/matched_set.json` at the base
records as exact, the symbol this run's build gives it must equal the symbol the table
`analysis/symbols/STARFLEETOP.tsv` has at the base.

The base is the one the stand-in check uses (harness/lib/standins.py: `HARNESS_STANDIN_BASE`, else
the merge base with origin/develop). No base, no row at the base, or no symbol in the build (a unit
that was not built, a symbol two claims share) is not an error. No compile: the symbols are the ones
`build-and-verify.sh --all` already has (`verify.report_renamed_exact`, so the whole-tree checks and
a layout pass see it), or `<build>/symbols.tsv` of a build of this tree:

    python3 -m harness.lib.renames [--base REF]       # exit 1 on a rename

A deliberate, evidenced rename is listed in `analysis/results/rename_baseline.tsv`
(`HARNESS_RENAME_BASELINE`; address, base symbol, new symbol, one tab-separated row each) in the
integrator's own pull request. A function session cannot set the variable (the guard refuses
environment assignments) or edit the file (`analysis/` is out of its reach).
"""
import csv
import io
import json
import os
import subprocess
import sys

from . import paths, standins, symtab

BASELINE = paths.env_path('HARNESS_RENAME_BASELINE', os.path.join(
    paths.REPO, 'analysis', 'results', 'rename_baseline.tsv'))
MATCHED_REL = 'analysis/results/matched_set.json'
TABLE_REL = 'analysis/symbols/STARFLEETOP.tsv'


def _show(base, rel, repo=None):
    p = subprocess.run(['git', '-C', repo or paths.REPO, 'show', f'{base}:{rel}'],
                       capture_output=True, text=True)
    return p.stdout if p.returncode == 0 else None


def base_exact(base, repo=None):
    """The addresses (0x...) the matched set at `base` records as exact."""
    text = _show(base, MATCHED_REL, repo)
    if text is None:
        return set()
    try:
        funcs = json.loads(text)['functions']
    except (ValueError, KeyError):
        return set()
    return {a for a, e in funcs.items() if e.get('exact')}


def base_symbols(base, repo=None):
    """{address: symbol} of the symbol table at `base` (a `folded` row has none)."""
    text = _show(base, TABLE_REL, repo)
    if text is None:
        return {}
    return {r['address']: r['symbol']
            for r in csv.DictReader(io.StringIO(text), delimiter='\t')
            if r.get('symbol') and r.get('kind') != 'folded'}


def load_baseline(path=None):
    """{(address, base symbol, new symbol)} of the allowed renames, empty without the file."""
    try:
        with open(path or BASELINE, encoding='utf-8') as f:
            rows = [x.rstrip('\n').split('\t') for x in f if x.strip() and not x.startswith('#')]
    except OSError:
        return set()
    return {tuple(r[:3]) for r in rows if len(r) >= 3 and r[0] != 'address'}


def findings(built, base, allowed=None, repo=None):
    """[(address, base symbol, new symbol)] of the base's exact functions whose symbol in `built`
    ({address: symbol}) differs from the base table's, minus the allowed ones."""
    if base is None:
        return []
    allowed = load_baseline() if allowed is None else allowed
    symbols = base_symbols(base, repo)
    out = []
    for a in sorted(base_exact(base, repo)):
        new, old = built.get(a), symbols.get(a)
        if new and old and new != old and (a, old, new) not in allowed:
            out.append((a, old, new))
    return out


def line(finding, base):
    a, old, new = finding
    return (f'{a}: exact at {base[:12]} as {old}, but this tree compiles {new}; keep the base name '
            '(a rename of an exact function breaks every call to it and its symbol table row at '
            'landing), or stop and write a HEADER REQUEST:/report line saying why it must change')


def main(argv=None):
    a = list(sys.argv[1:] if argv is None else argv)
    base = a[a.index('--base') + 1] if '--base' in a else standins.base_ref()
    if base is None:
        print('no base to compare with')
        return 0
    if not symtab.build_is_current(paths.BUILD, paths.SRC_ROOT):
        print('the last build was not made from the current source tree: run '
              'harness/build-and-verify.sh --all first')
        return 1
    built = {k: v[0] for k, v in symtab.load_claims().items()}
    found = findings(built, base)
    for f in found:
        print('ERROR ' + line(f, base))
    return 1 if found else 0


if __name__ == '__main__':
    sys.exit(main())
