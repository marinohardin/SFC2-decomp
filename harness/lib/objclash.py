"""Two checks on the built objects that a link would trip over (`--all`, harness/README.md,
"Build paths and clashing copies").

  build-path strings  a data section that holds the path of the checkout, or a source path with
                      a `.` segment (`<root>/.\\<include>`): `__FILE__` in an inline body that a header defines (an
                      assert, a throw) is expanded by the compiler to the path it was given, and
                      the exe never holds that string. A function that is to match the exe gives
                      its original file through a `#line` directive; a header body that cannot
                      stays out of units that are not its owner.
  clashing copies     an external function COMDAT that two objects define with different code
                      or different relocation targets, at least one of them a claimed function:
                      a linker keeps the first copy it sees, which may be the wrong one. Equal
                      copies are fine.

Both only read the COFF objects (harness/lib/coff.py). `objects` maps a unit to its object file.
"""
import re

from . import coff

SOURCE_EXTS = ('.h', '.hpp', '.inl', '.c', '.cpp')
# a NUL-terminated run of printable bytes that ends in a source extension
STRING = re.compile(rb'[\x20-\x7e]{4,}?(?:%s)(?=\0)' % b'|'.join(
    re.escape(e.encode()) for e in SOURCE_EXTS))
# the harness joins the source root and an include path as `<root>/.\<include>`, so a `__FILE__`
# the compiler expands carries a `.` path segment; the original's own absolute paths (a drive letter
# and a folder of the original's machine) are strings the exe holds and are not reported
DOT_SEGMENT = re.compile(r'[\\/]\.[\\/]')
SCN_CNT_CODE = 0x20


def _read(path):
    try:
        with open(path, 'rb') as f:
            return coff.CoffObject(f.read())
    except (OSError, coff.CoffError):
        return None


def _section_symbols(obj):
    """{section number: [external or static symbol names defined in it, in file order]}."""
    out = {}
    for s in obj.symbols.values():
        if s.section > 0 and s.storage in (coff.SYM_CLASS_EXTERNAL, coff.SYM_CLASS_STATIC):
            out.setdefault(s.section, []).append(s.name)
    return out


def _referrers(obj, num):
    """Names of the external symbols defined in sections that have a relocation against a symbol
    of section `num`, in sorted order."""
    out = set()
    for n, sec in obj.sections.items():
        if n == num or not any(obj.symbols[i].section == num for _, i, _ in sec.relocs
                               if i in obj.symbols):
            continue
        out.update(s.name for s in obj.symbols.values()
                   if s.section == n and s.storage == coff.SYM_CLASS_EXTERNAL)
    return sorted(out)


def build_path_strings(obj, roots=()):
    """[(section name, symbol, string)] for each source-file path in a data section of `obj` that
    has a `.` path segment (how the harness joins the source root and an include path) or holds one
    of `roots` (the checkout's path, in either slash direction). `symbol` names what uses the
    string: the first external symbol (a function) that refers to it, else the string's own
    symbol (whose mangled name holds the start of the path, so it is not stable across machines)."""
    found = []
    names = _section_symbols(obj)
    for num, sec in sorted(obj.sections.items()):
        if sec.characteristics & SCN_CNT_CODE:
            continue
        for m in STRING.finditer(sec.data):
            text = m.group().decode('latin1')
            hit = DOT_SEGMENT.search(text) or any(
                r and r.lower() in text.replace('\\', '/').lower()
                for r in (x.replace('\\', '/') for x in roots))
            if hit:
                users = _referrers(obj, num)
                own = next((n for n in names.get(num, ()) if not n.startswith('.')), sec.name)
                found.append((sec.name, users[0] if users else own, text))
    return found


def build_path_lines(objects, roots=(), allowed=()):
    """ERROR lines, one per (unit, symbol) whose object holds a build path. `allowed`: the
    (unit, symbol) pairs a baseline lists (they are not reported)."""
    out = []
    for unit, path in sorted(objects.items()):
        obj = _read(path)
        if obj is None:
            continue
        seen = set()
        for _, sym, text in build_path_strings(obj, roots):
            if (unit, sym) in allowed or (unit, sym) in seen:
                continue
            seen.add((unit, sym))
            out.append(f'{unit}: {sym} uses the build path {text!r}: an inline body that uses '
                       '__FILE__ was emitted here, and the exe never has that string (give the '
                       'body its original file and line with #line, or keep this unit from '
                       'emitting it)')
    return out


def function_copies(obj):
    """{symbol: (code bytes, [(offset, target symbol, type)], selection)} of the external
    function symbols that `obj` defines in a COMDAT section of their own."""
    out = {}
    for s in obj.symbols.values():
        if s.section <= 0 or s.storage != coff.SYM_CLASS_EXTERNAL or not s.is_function:
            continue
        sec = obj.sections[s.section]
        if sec.selection is None:
            continue
        relocs = [(off, obj.symbols[idx].name if idx in obj.symbols else idx, typ)
                  for off, idx, typ in sec.relocs]
        out[s.name] = (bytes(sec.data), relocs, sec.selection)
    return out


def clashing_copies(objects, claimed_symbols):
    """ERROR lines for each symbol in `claimed_symbols` that more than one object defines as a
    function COMDAT with different code or relocation targets. A claimed symbol that is defined
    by one object, or by several with the same content, is not reported."""
    copies = {}
    for unit, path in sorted(objects.items()):
        obj = _read(path)
        if obj is None:
            continue
        for name, copy in function_copies(obj).items():
            if name in claimed_symbols:
                copies.setdefault(name, []).append((unit, copy))
    out = []
    for name, defs in sorted(copies.items()):
        groups = {}
        for unit, (code, relocs, _) in defs:
            groups.setdefault((code, tuple(relocs)), []).append(unit)
        if len(groups) > 1:
            where = '; '.join(
                f'{", ".join(units)} ({len(code)} bytes, {len(relocs)} relocations)'
                for (code, relocs), units in sorted(groups.items(), key=lambda g: g[1]))
            out.append(f'{name} is a claimed function that objects define differently: {where} '
                       '(a linker keeps the first copy it sees; give it one definition)')
    return out


BASELINE_FIELDS = ('kind', 'unit', 'symbol')


def load_baseline(path):
    """The known debt: rows `path <TAB> unit <TAB> symbol` (a build path a unit holds, as
    build_path_lines names it) and `copy <TAB> - <TAB> symbol` (a claimed function that objects
    define differently). A missing file is no debt; `#` starts a comment line. Returns
    (allowed paths: set of (unit, symbol), allowed copies: set of symbols)."""
    paths_, copies = set(), set()
    try:
        f = open(path, newline='')
    except OSError:
        return paths_, copies
    with f:
        for n, line in enumerate(f, 1):
            line = line.rstrip('\r\n')
            if not line.strip() or line.startswith('#') or line.split('\t')[0] == 'kind':
                continue
            row = line.split('\t')
            if len(row) != 3 or row[0] not in ('path', 'copy'):
                raise ValueError(f'{path}:{n}: expected kind (path or copy), unit, symbol '
                                 'separated by tabs')
            (paths_.add((row[1], row[2])) if row[0] == 'path' else copies.add(row[2]))
    return paths_, copies


def check(objects, claimed_symbols, roots=(), baseline=(frozenset(), frozenset())):
    """(error lines, stale lines): the findings of both checks that the baseline does not list,
    and a line for each baseline row that no longer occurs (the list only shrinks)."""
    allowed_paths, allowed_copies = baseline
    seen_paths, seen_copies = set(), set()
    for unit, path in objects.items():
        obj = _read(path)
        if obj is not None:
            seen_paths.update((unit, sym) for _, sym, _ in build_path_strings(obj, roots))
    for line in clashing_copies(objects, claimed_symbols):
        seen_copies.add(line.split(' is a claimed function')[0])
    errors = build_path_lines(objects, roots, allowed_paths)
    errors += [x for x in clashing_copies(objects, claimed_symbols)
               if x.split(' is a claimed function')[0] not in allowed_copies]
    stale = [f'path\t{u}\t{s}' for u, s in sorted(allowed_paths - seen_paths)]
    stale += [f'copy\t-\t{s}' for s in sorted(allowed_copies - seen_copies)]
    return errors, stale
