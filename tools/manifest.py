#!/usr/bin/env python3
"""The manifest of decompiled functions and the native build's override table (DESIGN §6.3).

Scans `game/` for the annotations `harness/SRC_LAYOUT.md` defines and writes:

- `game/manifest.tsv`: one row per claimed address: address, size (from
  `ghidra/exports/functions.csv`), symbol, source file, compile modes (DESIGN §7.2).
  `// FUNCTION:` claims a function for both modes, `vc6 guest`; `// STUB:` is `stub`: compiled,
  not scored, and its lifted body stays in the native build.
- `src/app/guest/override.cpp`: for every function in guest mode, an entry point named like the
  lifted body it replaces (`sub_006FC5A0`) that takes the guest calling convention and calls the
  function compiled from the source; for every virtual member of a `// VTABLE:` class that has no
  body in game/, a dispatcher that calls through the guest vtable (game/port/guest.h, item 5);
  and the table `src/app/guest/override.h` declares.

The scanner reads the original's style of C++ (one declaration per line, braces on their own or
at the end of a line); what it does not understand it reports rather than guesses:

- a virtual member's slot is its place in the class's declaration order after the base class's
  slots, an override taking its base's slot (VC6's rule for single inheritance). Overloaded
  virtuals and multiple bases are refused: VC6 orders them in ways not modelled yet;
- a member function is __thiscall and a free function or static member __cdecl; other
  conventions are refused.

Usage: python3 tools/manifest.py [--game DIR] [--functions CSV] [--manifest TSV]
                                 [--override CPP] [--check]
"""
import argparse
import csv
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GAME = os.path.join(REPO, 'game')
FUNCTIONS_CSV = os.path.join(REPO, 'ghidra', 'exports', 'functions.csv')
MANIFEST = os.path.join(GAME, 'manifest.tsv')
OVERRIDE = os.path.join(REPO, 'src', 'app', 'guest', 'override.cpp')

MODULE = 'STARFLEETOP'
SOURCES = ('.cpp', '.c', '.h', '.inl')
COLUMNS = ('address', 'size', 'symbol', 'source', 'modes')
ANNOTATION = re.compile(r'^\s*//\s*(FUNCTION|STUB|VTABLE):\s*(\w+)\s+0x([0-9A-Fa-f]{1,8})\s*$')
CLASS = re.compile(r'^\s*(?:class|struct)\s+(\w+)\s*(?::\s*(.*?))?\s*(\{)?\s*$')
BASE = re.compile(r'^(?:(?:public|protected|private)\s+)?(?:virtual\s+)?([\w:]+)$')
IDENT = re.compile(r'[A-Za-z_]\w*$')
CONVENTIONS = ('__stdcall', '__fastcall', '__pascal')


class ManifestError(Exception):
    pass


class NotAFunction(Exception):
    """A declaration with parentheses that is not a function: `ptr32( T ) fMember;`."""


def strip_code(line):
    """The line without string and character literals and without its // comment."""
    line = re.sub(r'"(?:\\.|[^"\\])*"', '""', line)
    line = re.sub(r"'(?:\\.|[^'\\])*'", "''", line)
    cut = line.find('//')
    return line if cut < 0 else line[:cut]


def split_top(text, sep=','):
    """Split at `sep` outside parentheses and angle brackets."""
    parts, depth, cur = [], 0, ''
    for ch in text:
        if ch in '(<[':
            depth += 1
        elif ch in ')>]':
            depth -= 1
        if ch == sep and depth == 0:
            parts.append(cur)
            cur = ''
        else:
            cur += ch
    if cur.strip():
        parts.append(cur)
    return [p.strip() for p in parts]


def param_type_and_name(param, index):
    """('const char*', 'raw_name') from 'const char* raw_name'; a name is made up when missing."""
    param = param.split('=')[0].strip()
    m = IDENT.search(param)
    head = param[:m.start()].strip() if m else param
    if m and head and not head.endswith('::') and head not in ('const', 'unsigned', 'signed'):
        return head, m.group(0)
    return param, 'a%d' % index


def parse_signature(text):
    """Return type, qualified name, parameter types, parameter names and const-ness of a
    declaration or definition head (`int32 X::Y::operator==( const char* raw_name ) const`)."""
    text = ' '.join(text.split())
    at = text.find('operator()')
    start = text.find('(', at + len('operator()') if at >= 0 else 0)
    if start < 0:
        raise ManifestError('no parameter list in %r' % text)
    depth, end = 0, None
    for i in range(start, len(text)):
        if text[i] == '(':
            depth += 1
        elif text[i] == ')':
            depth -= 1
            if depth == 0:
                end = i
                break
    if end is None:
        raise ManifestError('unbalanced parameter list in %r' % text)
    head, params, tail = text[:start].strip(), text[start + 1:end].strip(), text[end + 1:].strip()
    m = re.search(r'((?:[A-Za-z_]\w*::)*(?:operator\s*(?:\(\)|[^\s\w(]+)|~?[A-Za-z_]\w*))$', head)
    if not m:
        raise ManifestError('no function name in %r' % text)
    name = m.group(1).replace(' ', '')
    ret = head[:m.start()].strip()
    for word in ('virtual', 'static', 'inline', 'explicit'):
        ret = re.sub(r'\b%s\b' % word, '', ret).strip()
    for conv in CONVENTIONS:
        if conv in ret:
            raise ManifestError('%s: calling convention %s is not handled' % (name, conv))
    ret = ret.replace('__cdecl', '').replace('__thiscall', '').strip()
    types, names = [], []
    if params and params != 'void':
        for i, p in enumerate(split_top(params)):
            t, n = param_type_and_name(p, i)
            types.append(t)
            names.append(n)
    if tail not in ('', 'const'):
        raise NotAFunction(text)
    return ret, name, tuple(types), tuple(names), tail == 'const'


def read_lines(path):
    with open(path, encoding='utf-8', errors='replace') as f:
        return f.read().splitlines()


def scan(game):
    """Every annotation and class declaration under `game` (port/ excluded)."""
    functions, classes = [], {}
    for dirpath, dirnames, filenames in os.walk(game):
        dirnames[:] = sorted(d for d in dirnames if os.path.relpath(os.path.join(dirpath, d), game) != 'port')
        for fn in sorted(filenames):
            if fn.endswith(SOURCES):
                path = os.path.join(dirpath, fn)
                scan_file(path, os.path.relpath(path, game).replace(os.sep, '/'), functions, classes)
    return functions, classes


def scan_file(path, rel, functions, classes):
    lines = read_lines(path)
    pending_vtable = None
    stack = []          # open braces: ('class', record) or ('other', None)
    i = 0
    while i < len(lines):
        raw = lines[i]
        m = ANNOTATION.match(raw)
        if m:
            kind, module, address = m.group(1), m.group(2), int(m.group(3), 16)
            if module != MODULE:
                raise ManifestError('%s:%d: module %s, not %s' % (rel, i + 1, module, MODULE))
            if kind == 'VTABLE':
                pending_vtable = (address, i + 1)
            else:
                functions.append(read_definition(lines, i, rel, kind, address))
            i += 1
            continue
        code = strip_code(raw)
        cm = CLASS.match(code)
        if cm and (cm.group(3) or (i + 1 < len(lines) and strip_code(lines[i + 1]).strip() == '{')):
            outer = [r for k, r in stack if k == 'class']
            qual = '::'.join([r['name'] for r in outer] + [cm.group(1)])
            bases = [b for b in split_top(cm.group(2) or '')]
            record = {'name': qual, 'file': rel, 'bases': [], 'members': [], 'vtable': None}
            for b in bases:
                bm = BASE.match(b)
                if not bm:
                    raise ManifestError('%s:%d: cannot read base %r' % (rel, i + 1, b))
                record['bases'].append(bm.group(1))
            if pending_vtable:
                record['vtable'] = pending_vtable[0]
                pending_vtable = None
            if qual in classes:
                raise ManifestError('%s:%d: class %s declared twice' % (rel, i + 1, qual))
            classes[qual] = record
            if not cm.group(3):
                i += 1          # the brace on the next line opens the class
            stack.append(('class', record))
            i += 1
            continue
        if pending_vtable and code.strip():
            raise ManifestError('%s:%d: a VTABLE annotation must come right before its class'
                                % (rel, pending_vtable[1]))
        if stack and stack[-1][0] == 'class' and '(' in code and code.rstrip().endswith(';') \
                and code.count('{') == 0:
            add_member(stack[-1][1], code, rel, i + 1)
        for ch in code:
            if ch == '{':
                stack.append(('other', None))
            elif ch == '}':
                if not stack:
                    raise ManifestError('%s:%d: unbalanced braces' % (rel, i + 1))
                stack.pop()
        i += 1


def add_member(record, code, rel, lineno):
    decl = code.strip().rstrip(';').strip()
    decl = re.sub(r'^(?:public|protected|private)\s*:\s*', '', decl)
    if decl.startswith(('friend', 'typedef', 'using')):
        return
    pure = bool(re.search(r'(?:=\s*0|\bSFCOP_PURE)\s*$', decl))
    decl = re.sub(r'(?:=\s*0|\bSFCOP_PURE)\s*$', '', decl).strip()
    try:
        ret, name, types, names, is_const = parse_signature(decl)
    except NotAFunction:
        return
    except ManifestError as e:
        raise ManifestError('%s:%d: %s' % (rel, lineno, e))
    record['members'].append({
        'name': name, 'ret': ret, 'types': types, 'names': names, 'const': is_const,
        'virtual': bool(re.search(r'\bvirtual\b', decl)), 'static': bool(re.search(r'\bstatic\b', decl)),
        'pure': pure, 'line': lineno})


def read_definition(lines, i, rel, kind, address):
    """The definition head after an annotation: the text up to its body's brace."""
    head, j = '', i + 1
    while j < len(lines) and '{' not in strip_code(lines[j]):
        head += ' ' + strip_code(lines[j])
        j += 1
    if j < len(lines):
        head += ' ' + strip_code(lines[j]).split('{')[0]
    head = head.strip()
    # A constructor's initialiser list is not part of the signature.
    depth, cut = 0, len(head)
    for k, ch in enumerate(head):
        if ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
        elif ch == ':' and depth == 0 and k > 0 and head[k - 1] != ':' and \
                (k + 1 >= len(head) or head[k + 1] != ':') and ')' in head[:k]:
            cut = k
            break
    try:
        ret, name, types, names, is_const = parse_signature(head[:cut])
    except NotAFunction:
        raise ManifestError('%s:%d: cannot read the definition %r' % (rel, i + 2, head))
    except ManifestError as e:
        raise ManifestError('%s:%d: %s' % (rel, i + 2, e))
    return {'kind': kind, 'address': address, 'file': rel, 'line': i + 1, 'ret': ret, 'name': name,
            'types': types, 'names': names, 'const': is_const}


def is_constructor_or_destructor(name):
    qual, _, short = name.rpartition('::')
    return short.startswith('~') or short == qual.rpartition('::')[2]


def symbol_of(f):
    return '%s(%s)%s' % (f['name'], ','.join(f['types']), ' const' if f['const'] else '')


def owner_class(name, classes):
    """The class a qualified function name belongs to, or None for a free function."""
    if '::' not in name:
        return None
    owner = name.rsplit('::', 1)[0]
    if owner not in classes:
        raise ManifestError('%s: no declaration of class %s in game/' % (name, owner))
    return classes[owner]


def find_member(record, f):
    short = f['name'].rsplit('::', 1)[1]
    for m in record['members']:
        if m['name'] == short and m['types'] == f['types'] and m['const'] == f['const']:
            return m
    raise ManifestError('%s: not declared in class %s (%s)' % (symbol_of(f), record['name'], record['file']))


def vtable_slots(qual, classes, seen=()):
    """[(class, member)] in slot order for a VTABLE class."""
    record = classes[qual]
    if qual in seen:
        raise ManifestError('class %s derives from itself' % qual)
    if len(record['bases']) > 1:
        raise ManifestError('%s: multiple bases are not handled' % qual)
    slots = []
    if record['bases']:
        base = resolve_class(record['bases'][0], qual, classes)
        if base is not None and classes[base]['vtable'] is not None:
            slots = vtable_slots(base, classes, seen + (qual,))
        elif base is not None and any(m['virtual'] for m in classes[base]['members']):
            raise ManifestError('%s: base %s has virtual members but no VTABLE annotation' % (qual, base))
    names = [m['name'] for m in record['members'] if m['virtual'] and not m['name'].startswith('~')]
    for n in set(names):
        if names.count(n) > 1:
            raise ManifestError('%s: overloaded virtual %s: VC6 slot order not modelled' % (qual, n))
    for m in record['members']:
        if not m['virtual']:
            continue
        key = ('~',) if m['name'].startswith('~') else (m['name'], m['types'], m['const'])
        for k, (c, b) in enumerate(slots):
            bkey = ('~',) if b['name'].startswith('~') else (b['name'], b['types'], b['const'])
            if bkey == key:
                slots[k] = (qual, m)
                break
        else:
            slots.append((qual, m))
    return slots


def resolve_class(name, context, classes):
    scope = context.split('::')
    while True:
        cand = '::'.join(scope[:-1] + [name]) if scope[:-1] else name
        if cand in classes:
            return cand
        if not scope[:-1]:
            return None
        scope = scope[:-1]


def build(game, functions_csv):
    functions, classes = scan(game)
    sizes = {}
    with open(functions_csv, newline='') as f:
        for row in csv.DictReader(f):
            sizes[int(row['address'], 16)] = row['size']
    seen = {}
    for f in functions:
        if f['address'] in seen:
            other = seen[f['address']]
            raise ManifestError('0x%08x claimed twice: %s:%d and %s:%d' % (
                f['address'], other['file'], other['line'], f['file'], f['line']))
        seen[f['address']] = f
        if f['address'] not in sizes:
            raise ManifestError('%s:%d: 0x%08x is not a function in %s' % (
                f['file'], f['line'], f['address'], os.path.relpath(functions_csv, REPO)))
        f['size'] = sizes[f['address']]
        f['modes'] = 'vc6 guest' if f['kind'] == 'FUNCTION' else 'stub'
        record = owner_class(f['name'], classes)
        f['class'] = record
        f['static'] = bool(record) and find_member(record, f)['static']
        if record is not None and is_constructor_or_destructor(f['name']):
            # C++ has no pointer to a constructor or destructor, so the guest build cannot wrap one
            # as an entry point; the lifted body stays until the object lifter (DESIGN §7.2) carries
            # the VC6 object in.
            f['modes'] = 'vc6'
    functions.sort(key=lambda f: f['address'])
    return functions, classes


def manifest_text(functions):
    out = ['\t'.join(COLUMNS)]
    for f in functions:
        out.append('\t'.join(['0x%08x' % f['address'], f['size'], symbol_of(f), f['file'], f['modes']]))
    return '\n'.join(out) + '\n'


def entry_expression(f):
    types = ', '.join(f['types'])
    if f['class'] is not None and not f['static']:
        sig = '%s(%s)%s' % (f['ret'], ' %s ' % types if types else '', ' const' if f['const'] else '')
        target = '::sfcop::guest::member< %s >( &%s )' % (sig, f['name'])
    else:
        target = 'static_cast< %s (*)(%s) >( &%s )' % (f['ret'], ' %s ' % types if types else '', f['name'])
    return '::sfcop::guest::Entry< %s >::run();' % target


def override_text(functions, classes):
    guest = [f for f in functions if 'guest' in f['modes']]
    bodies = {(f['name'], f['types'], f['const']) for f in functions}
    headers = sorted({f['class']['file'] for f in guest if f['class']} |
                     {c['file'] for c in classes.values() if c['vtable'] is not None})
    out = [
        '// Generated by tools/manifest.py from the annotations in game/ (DESIGN §6.3): do not edit.',
        '// Check: python3 tools/manifest.py --check',
        '#define SFCOP_GUEST_OVERRIDE_IMPLEMENTATION',
        '#include "app/guest/override.h"',
        '',
        '#if defined(SFCOP_GUEST_MODE_SOURCES)',
        '',
    ]
    out += ['#include "%s"' % h for h in headers]
    out += ['', '// Entry points: the lifted names, the guest calling convention.']
    for i, f in enumerate(guest):
        out += ['',
                '// %s, game/%s' % (symbol_of(f), f['file']),
                'extern "C" void sub_%08X(void) {' % f['address'],
                '    ::sfcop::guest::entered(%d);' % i,
                '    %s' % entry_expression(f),
                '}']
    out += ['', '// Dispatchers: virtual members with no body in game/, called through the guest vtable.']
    for qual in sorted(q for q, c in classes.items() if c['vtable'] is not None):
        for slot, (owner, m) in enumerate(vtable_slots(qual, classes)):
            if owner != qual or m['name'].startswith('~'):
                continue
            full = '%s::%s' % (qual, m['name'])
            if (full, m['types'], m['const']) in bodies:
                continue
            params = ', '.join('%s %s' % (t, n) for t, n in zip(m['types'], m['names']))
            args = ''.join(', %s' % n for n in m['names'])
            out += ['',
                    '// slot %d of %s\'s vtable (0x%08X)' % (slot, qual, classes[qual]['vtable']),
                    '%s %s(%s)%s {' % (m['ret'], full, ' %s ' % params if params else '',
                                       ' const' if m['const'] else ''),
                    '    return ::sfcop::guest::virtualCall< %s >( this, %d%s );' % (m['ret'], slot, args),
                    '}']
    out += ['', 'extern "C" {', '']
    if guest:
        out.append('const sfcop_guest_override sfcop_guest_overrides[] = {')
        for f in guest:
            out.append('    {0x%08Xu, sub_%08X, "%s", "%s"},' % (f['address'], f['address'], symbol_of(f), f['file']))
        out.append('};')
    else:
        out.append('const sfcop_guest_override sfcop_guest_overrides[] = {{0u, nullptr, nullptr, nullptr}};')
    out += ['const unsigned sfcop_guest_override_count = %du;' % len(guest), '', '}  // extern "C"', '',
            '#else  // the native build without SFCOP_GUEST_MODE_SOURCES: everything stays lifted',
            '',
            'extern "C" {',
            '',
            'const sfcop_guest_override sfcop_guest_overrides[] = {{0u, nullptr, nullptr, nullptr}};',
            'const unsigned sfcop_guest_override_count = 0u;',
            '',
            'sfcop_guest_entry sfcop_guest_override_lookup(uint32_t) { return nullptr; }',
            '',
            'uint32_t sfcop_guest_override_calls(uint32_t) { return 0u; }',
            '',
            '}  // extern "C"',
            '',
            '#endif',
            '']
    return '\n'.join(out)


def write_or_check(path, text, check):
    old = None
    if os.path.exists(path):
        with open(path, encoding='utf-8') as f:
            old = f.read()
    if old == text:
        return True
    if check:
        print('manifest: %s is not what the annotations give; run python3 tools/manifest.py'
              % os.path.relpath(path, REPO), file=sys.stderr)
        return False
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w', encoding='utf-8') as f:
        f.write(text)
    print('wrote %s' % os.path.relpath(path, REPO))
    return True


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--game', default=GAME)
    ap.add_argument('--functions', default=FUNCTIONS_CSV)
    ap.add_argument('--manifest', default=MANIFEST)
    ap.add_argument('--override', default=OVERRIDE)
    ap.add_argument('--check', action='store_true', help='exit 1 if an output is out of date')
    args = ap.parse_args(argv)
    try:
        functions, classes = build(args.game, args.functions)
        manifest, override = manifest_text(functions), override_text(functions, classes)
    except ManifestError as e:
        print('manifest: %s' % e, file=sys.stderr)
        return 2
    ok = write_or_check(args.manifest, manifest, args.check)
    ok = write_or_check(args.override, override, args.check) and ok
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
