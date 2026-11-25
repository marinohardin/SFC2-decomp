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


def _has_root(text, roots):
    """True when `text` holds one of `roots` as whole path segments (a root `/work` is not in
    `/work2/x`), in either slash direction and any case."""
    t = text.replace('\\', '/').lower()
    for root in roots:
        r = root.replace('\\', '/').lower().rstrip('/')
        if not r:
            continue
        i = t.find(r)
        while i >= 0:
            end = i + len(r)
            if (end == len(t) or t[end] == '/') and (i == 0 or (r[0] != '/' and t[i - 1] == '/')):
                return True
            i = t.find(r, i + 1)
    return False


def build_path_strings(obj, roots=()):
    """[(section name, symbol or None, string)] for each source-file path in a data section of
    `obj` that has a `.` path segment (how the harness joins the source root and an include path)
    or holds one of `roots` (the checkout's path) as whole segments. `symbol` is the first
    external symbol (a function) that refers to the string, or None when none does: the string's
    own mangled name holds the start of the path, so it is not stable across machines."""
    found = []
    for num, sec in sorted(obj.sections.items()):
        if sec.characteristics & SCN_CNT_CODE:
            continue
        for m in STRING.finditer(sec.data):
            text = m.group().decode('latin1')
            if DOT_SEGMENT.search(text) or _has_root(text, roots):
                users = _referrers(obj, num)
                found.append((sec.name, users[0] if users else None, text))
    return found


def path_findings(objs, roots=()):
    """[(unit, function or None, build path, line)], one per (unit, function) (one per string
    for a string no function refers to). `objs` maps a unit to its parsed object."""
    out = []
    for unit, obj in sorted(objs.items()):
        seen = set()
        for _, sym, text in build_path_strings(obj, roots):
            if (sym or text) in seen:
                continue
            seen.add(sym or text)
            if sym is None:
                line = (f'{unit}: the object holds the build path {text!r} and no function refers '
                        'to it, so it cannot be baselined (find what emits it)')
            else:
                line = (f'{unit}: {sym} uses the build path {text!r}: an inline body that uses '
                        '__FILE__ was emitted here, and the exe never has that string (give the '
                        'body its original file and line with #line, or keep this unit from '
                        'emitting it)')
            out.append((unit, sym, text, line))
    return out


def load_objects(objects):
    """{unit: parsed object} for the objects that can be read."""
    out = {}
    for unit, path in objects.items():
        obj = _read(path)
        if obj is not None:
            out[unit] = obj
    return out


def build_path_lines(objects, roots=(), allowed=()):
    """ERROR lines for the build paths the objects hold, except the (unit, function) pairs in
    `allowed`. `objects` maps a unit to its object file."""
    return [line for unit, sym, _, line in path_findings(load_objects(objects), roots)
            if sym is None or (unit, sym) not in allowed]


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


def copy_findings(objs, claimed_symbols):
    """[(symbol, line)] for each symbol in `claimed_symbols` that more than one object defines as
    a function COMDAT with different code or relocation targets (COMDAT selection kinds are not
    compared: harness/lib/verify.py comdat_kind_conflicts does that). `objs` maps a unit to its
    parsed object."""
    copies = {}
    for unit, obj in sorted(objs.items()):
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
            out.append((name, f'{name} is a claimed function that objects define differently: '
                              f'{where} (a linker keeps the first copy it sees; give it one '
                              'definition)'))
    return out


def clashing_copies(objects, claimed_symbols):
    """ERROR lines of copy_findings for `objects` (a unit to its object file)."""
    return [line for _, line in copy_findings(load_objects(objects), claimed_symbols)]


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
    and a line for each baseline row that no longer occurs (the list only shrinks). Each object
    is parsed once and the baseline is matched on the structured symbol, not on message text."""
    allowed_paths, allowed_copies = baseline
    objs = load_objects(objects)
    paths_ = path_findings(objs, roots)
    copies = copy_findings(objs, claimed_symbols)
    errors = [line for unit, sym, _, line in paths_
              if sym is None or (unit, sym) not in allowed_paths]
    errors += [line for sym, line in copies if sym not in allowed_copies]
    seen_paths = {(unit, sym) for unit, sym, _, _ in paths_ if sym is not None}
    seen_copies = {sym for sym, _ in copies}
    stale = [f'path\t{u}\t{s}' for u, s in sorted(allowed_paths - seen_paths)]
    stale += [f'copy\t-\t{s}' for s in sorted(allowed_copies - seen_copies)]
    return errors, stale
