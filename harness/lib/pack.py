"""The context pack: what a function session would otherwise spend its first turns looking up.

    python3 -m harness.lib.pack <entry.json>        # prints the pack for the entry's function

Four sections, each capped, and the whole pack capped at HARNESS_PACK_MAX characters (12000):

1. Ghidra's decompiled pseudo-C of the function, from the cache that
   ghidra/scripts/export_decomp.sh writes ($SFCOP_WORK/ghidra/decomp/<0xaddress>.c, or
   HARNESS_DECOMP). Ghidra's types and names are rough; it is a reading aid, not source.
2. The functions it calls directly, with their prototypes from ghidra/exports/functions.csv and
   the source file under op/ that already holds one.
3. The declarations of the classes it touches (its own class, then its callees'), taken from
   the headers under the source root; a class declared in no header is left out.
4. One function already exact in matched_set.json from the same class, or else from the same
   unit (op/UNITS.tsv), as it stands in the source.

Every section that has nothing to show says so in one line, so the session does not look again.
"""
import json
import os
import re
import sys

from . import coff, funcs, paths, source

PACK_MAX = int(os.environ.get('HARNESS_PACK_MAX', 12000))
DECOMP_MAX = 5000
CALLEES_MAX = 24
CLASS_MAX = 2200
CLASSES_MAX = 4400
SAMPLE_MAX = 2400
HEADER_EXTS = ('.h', '.hpp', '.inl')


def decomp_dir():
    return paths.env_path('HARNESS_DECOMP', os.path.join(paths.WORK, 'ghidra', 'decomp'))


def cut(text, limit, what='lines'):
    """Text cut to at most `limit` characters at a line end, with a note of what was left out."""
    text = text.rstrip('\n')
    if len(text) <= limit:
        return text
    lines, out, used = text.split('\n'), [], 0
    for line in lines:
        if used + len(line) + 1 > limit:
            break
        out.append(line)
        used += len(line) + 1
    return '\n'.join(out) + f'\n... ({len(lines) - len(out)} more {what} cut)'


def class_of(name):
    """The class part of a qualified name: 'tFoo::Bar' -> 'tFoo', 'A::B::c' -> 'B'; None for a
    free function or a placeholder."""
    head = name.split('(')[0]
    if '::' not in head:
        return None
    cls = re.sub(r'<.*', '', head.split('::')[-2]).split()
    cls = cls[-1] if cls else ''
    return cls if re.match(r'^[A-Za-z_]\w*$', cls or '') and not cls.startswith('FUN_') else None


PLATE_LINES = 3


def short_plate(text):
    """Ghidra's plate comment (the /* ... */ block before the signature) cut to PLATE_LINES
    lines: it often lists every class that inherits the method."""
    body = text.lstrip('\n')
    if not body.startswith('/*'):
        return body
    end = body.find('*/')
    if end < 0:
        return body
    plate = body[:end + 2].split('\n')
    if len(plate) <= PLATE_LINES:
        return body
    kept = plate[:PLATE_LINES - 1] + [f'   ... ({len(plate) - PLATE_LINES + 1} more lines of '
                                      f"Ghidra's comment cut) */"]
    return '\n'.join(kept) + body[end + 2:]


def decompiled(address, directory=None):
    path = os.path.join(directory or decomp_dir(), f'{address}.c')
    try:
        with open(path, encoding='utf-8', errors='replace') as f:
            return f.read()
    except OSError:
        return None


def call_targets(code, address):
    """Addresses of direct `call rel32` targets in `code` loaded at `address`, in order, once
    each. With capstone the code is disassembled; without, every E8 byte is a candidate and the
    caller keeps only targets that are function starts."""
    out = []
    try:
        import capstone
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        for i in md.disasm(code, address):
            if i.mnemonic == 'call' and i.op_str.startswith('0x'):
                out.append(int(i.op_str, 16))
    except ImportError:
        for i in range(len(code) - 4):
            if code[i] == 0xE8:
                rel = int.from_bytes(code[i + 1:i + 5], 'little', signed=True)
                out.append((address + i + 5 + rel) & 0xFFFFFFFF)
    seen, uniq = set(), []
    for t in out:
        if t not in seen:
            seen.add(t)
            uniq.append(t)
    return uniq


def annotated_files(anns, src_root):
    """{address: relative path} of every FUNCTION/STUB annotation in `anns`."""
    found = {}
    for addr, anns in anns.items():
        for a in anns:
            found.setdefault(addr, os.path.relpath(a.path, src_root))
    return found


def callees(fn, index, exe, annotated):
    """[(address, prototype or name, file under the source root or '')] of direct callees."""
    if exe is None or not fn.size:
        return []
    try:
        code = exe.read(fn.address, fn.size)
    except Exception:  # a function the image cannot hand out whole: no callee list, keep the rest
        return []
    out = []
    for t in call_targets(code, fn.address):
        c = index.by_address.get(t)
        if c is None or t == fn.address:
            continue
        out.append((c.id, c.demangled if c.has_prototype else c.name, annotated.get(t, '')))
    return out


def _headers(src_root):
    for d, dirs, files in os.walk(src_root):
        dirs[:] = sorted(x for x in dirs if not x.startswith('.'))
        for name in sorted(files):
            if name.lower().endswith(HEADER_EXTS):
                yield os.path.join(d, name)


def braced_block(text, start):
    """From `start` to the end of the first {...} after it (with a trailing ';' if any), or None
    when the braces do not close."""
    i = text.find('{', start)
    if i < 0:
        return None
    depth = 0
    for j in range(i, len(text)):
        if text[j] == '{':
            depth += 1
        elif text[j] == '}':
            depth -= 1
            if depth == 0:
                end = j + 1
                if text[end:end + 1] == ';':
                    end += 1
                return text[start:end]
    return None


def class_declaration(cls, src_root):
    """(header path relative to src_root, declaration text) of `class cls {...};` or
    `struct cls {...};` in a header, or None."""
    decl = re.compile(r'^[ \t]*(?:class|struct)[ \t]+(?:\w+[ \t]+)?' + re.escape(cls) +
                      r'\b[^;{]*\{', re.M)
    for path in _headers(src_root):
        try:
            with open(path, encoding='latin1') as f:
                text = f.read()
        except OSError:
            continue
        m = decl.search(text)
        if m:
            block = braced_block(text, m.start())
            if block:
                return os.path.relpath(path, src_root), block
    return None


def _unit_of(address, unit_map):
    for lo, hi, unit in unit_map:
        if lo <= address <= hi:
            return lo, hi, unit
    return None


def function_text(path, line):
    """The annotated definition that starts after line `line` (1-based) of `path`: the
    annotation lines and the function through its closing brace."""
    with open(path, encoding='latin1') as f:
        lines = f.read().split('\n')
    start = line - 1
    while start > 0 and source.ANNOTATION.match(lines[start - 1]):
        start -= 1
    text = '\n'.join(lines[start:])
    body = braced_block(text, 0)
    return body if body else None


def exact_sample(fn, index, matched_set, src_root, unit_map, anns):
    """(address, name, relative path, text) of an exact function from fn's class, or failing
    that from its unit; the nearest by address. None if there is none."""
    recorded = matched_set.get('functions', {})
    exact = [int(k, 16) for k, v in recorded.items() if v.get('exact') and int(k, 16) != fn.address]

    def names(a):
        return [n for n in (recorded[f'0x{a:08x}'].get('name'),
                            getattr(index.by_address.get(a), 'name', None)) if n]

    cls = class_of(fn.name)
    unit = _unit_of(fn.address, unit_map)
    pools = []
    if cls:
        pools.append([a for a in exact if cls in {class_of(n) for n in names(a)}])
    if unit:
        pools.append([a for a in exact if unit[0] <= a <= unit[1]])
    for pool in pools:
        for a in sorted(pool, key=lambda x: (abs(x - fn.address), x)):
            for ann in anns.get(a, []):
                if ann.kind != 'FUNCTION':
                    continue
                text = function_text(ann.path, ann.line)
                if text:
                    return (f'0x{a:08x}', (names(a) or [f'fn_{a:08x}'])[0],
                            os.path.relpath(ann.path, src_root), text)
    return None


def build(entry, index, exe=None, matched_set=None, src_root=None, unit_map=(),
          decomp=None, limit=None):
    """The pack as Markdown."""
    src_root = src_root or paths.SRC_ROOT
    limit = limit or PACK_MAX
    a = int(entry['address'], 16)
    fn = index.by_address.get(a)
    if fn is None:
        return f'(no context pack: {entry["address"]} is not in functions.csv)\n'
    parts = ['## Context pack', '',
             'Gathered by the harness before this session so you need not look it up. Ghidra\'s '
             'output is a reading aid with rough types and names; the original instructions '
             'from build-and-verify.sh are what you match.', '']

    text = decompiled(fn.id, decomp)
    parts.append("### Ghidra's pseudo-C")
    parts.append('')
    if text is None:
        parts.append('(not exported: run ghidra/scripts/export_decomp.sh)')
    else:
        parts += ['```c', cut(short_plate(text), DECOMP_MAX), '```']
    parts.append('')

    anns = source.scan_annotations(src_root, 'STARFLEETOP')
    annotated = annotated_files(anns, src_root)
    calls = callees(fn, index, exe, annotated)
    parts.append('### Direct callees')
    parts.append('')
    if not calls:
        parts.append('(none found)' if exe is not None else '(the exe is not available)')
    for addr, proto, where in calls[:CALLEES_MAX]:
        parts.append(f'- `{addr}` `{proto}`' + (f' (in `{where}`)' if where else ''))
    if len(calls) > CALLEES_MAX:
        parts.append(f'- ... {len(calls) - CALLEES_MAX} more cut')
    parts.append('')

    wanted = []
    for name in [fn.name] + [index.by_address[int(c[0], 16)].name for c in calls]:
        cls = class_of(name)
        if cls and cls not in wanted:
            wanted.append(cls)
    parts.append('### Class declarations in the headers')
    parts.append('')
    used, shown, missing = 0, 0, []
    for cls in wanted:
        found = class_declaration(cls, src_root)
        if not found:
            missing.append(cls)
            continue
        where, block = found
        block = cut(block, CLASS_MAX)
        if used + len(block) > CLASSES_MAX:
            parts.append(f'(`{cls}` and later classes cut: the section is full)')
            break
        parts += [f'`{where}`:', '', '```cpp', block, '```', '']
        used += len(block)
        shown += 1
    if not shown:
        parts.append('(none of its classes is declared in a header yet)')
    if missing:
        parts.append('Not declared in any header: ' + ', '.join(f'`{c}`' for c in missing) +
                     '. Search `' + os.path.relpath(src_root, paths.REPO) +
                     '/` before declaring one; declare a class once.')
    parts.append('')

    sample = exact_sample(fn, index, matched_set or {}, src_root, list(unit_map), anns)
    parts.append('### An exact function from the same class or unit')
    parts.append('')
    if sample:
        sa, sname, spath, stext = sample
        parts += [f'`{sa}` {sname}, in `{spath}`:', '', '```cpp', cut(stext, SAMPLE_MAX), '```']
    else:
        parts.append('(none yet)')
    return cut('\n'.join(parts), limit, 'lines of the pack') + '\n'


def load_inputs():
    """The index, exe, matched set and unit map as the loop sees them; missing ones are None."""
    from . import matched, workqueue
    index = funcs.FunctionIndex(paths.FUNCTIONS_CSV)
    try:
        exe = coff.PeImage(open(paths.EXE, 'rb').read())
    except OSError:
        exe = None
    try:
        unit_map = workqueue.read_unit_map(paths.UNITS_TSV)
    except (OSError, KeyError, ValueError):
        unit_map = []
    return index, exe, matched.load(paths.MATCHED_SET), unit_map


def main(argv):
    with open(argv[0]) as f:
        entry = json.load(f)
    index, exe, ms, unit_map = load_inputs()
    sys.stdout.write(build(entry, index, exe, ms, unit_map=unit_map))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
