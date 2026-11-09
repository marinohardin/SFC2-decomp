#!/usr/bin/env python3
"""Progress badges and a treemap of the game's code, as SVG files for the README.

    python3 tools/progress_chart.py [--out DIR]

Reads, all from the repository:
  analysis/results/progress/progress.json   the figures scripts/progress.py wrote (the badges)
  ghidra/exports/functions.csv              every function's address, size, kind and library flag
  game/UNITS.tsv                            the translation units and their address ranges
  analysis/results/matched_set.json         the best verified score of every attempted function

Writes into analysis/results/progress/ (or --out):
  badge-exact.svg       game code rebuilt byte-exact, % of game code bytes
  badge-functional.svg  game code rebuilt exact or functionally verified, % of game code bytes
  badge-named.svg       game functions named from a real symbol, % of game functions
  treemap.svg           one rectangle per unit, area = its game code bytes, grouped by top-level
                        directory; bright green when every function in it is exact, otherwise dark
                        grey shading toward green by the share of its bytes matched (exact bytes,
                        plus partial scores from the matched set weighted by size)

The badges repeat progress.json, so they are only as current as it is: run
`python3 scripts/progress.py --markdown --matched-set analysis/results/matched_set.json` first,
then this, when a batch closes. The output has no timestamp, so running it twice on the same
inputs changes nothing. Standard library only.
"""
import argparse
import bisect
import csv
import html
import json
import os
import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
PROGRESS = ROOT / 'analysis' / 'results' / 'progress'

WIDTH, HEIGHT = 1200, 600
GROUP_GAP = 2           # pixels between top-level directory groups
COLOR_EMPTY = (0x33, 0x36, 0x33)
COLOR_PARTIAL = (0x3d, 0x8f, 0x3d)
COLOR_DONE = (0x00, 0xd0, 0x00)
BADGE_LABEL = '#555'
BADGE_VALUE = '#347abe'


# --- inputs ------------------------------------------------------------------------------------

def badge_values(progress):
    """(exact, exact + functional, named) percentages for the game-only scope."""
    g = progress['scopes']['game']
    st = g['status']
    exact = st['exact']['pct_bytes']
    functional = st['exact']['pct_bytes'] + st['functional']['pct_bytes']
    named = g['named_real_symbol']['pct_functions']
    return exact, functional, named


def _addresses(entries):
    out = set()
    for e in entries or []:
        a = e.get('address') if isinstance(e, dict) else e
        if isinstance(a, int):
            out.add(a)
        elif a is not None:
            out.add(int(str(a), 16))
    return out


def read_units(functions_csv, units_tsv, progress, matched):
    """[(group, unit, bytes, matched_bytes, complete)] for every unit with game code."""
    with open(units_tsv, newline='') as f:
        rows = sorted(((int(r['first'], 16), int(r['last'], 16), r['unit'])
                       for r in csv.DictReader(f, delimiter='\t') if r.get('unit')))
    starts = [r[0] for r in rows]
    exact = _addresses(progress.get('exact')) | _addresses(progress.get('functional'))
    scores = {}
    for k, v in (matched.get('functions') or {}).items():
        a = int(k, 16)
        if v.get('exact'):
            exact.add(a)
        scores[a] = float(v.get('score') or 0.0)
    totals = {}
    with open(functions_csv, newline='') as f:
        for r in csv.DictReader(f):
            if r['kind'] != 'function' or r['is_library'] == '1':
                continue
            size = int(r['size'] or 0)
            if size <= 0:
                continue
            a = int(r['address'], 16)
            i = bisect.bisect_right(starts, a) - 1
            if i < 0 or a > rows[i][1]:
                continue
            unit = rows[i][2]
            t = totals.setdefault(unit, [0, 0.0, True])
            t[0] += size
            if a in exact:
                t[1] += size
            else:
                t[1] += size * scores.get(a, 0.0) / 100.0
                t[2] = False
    out = []
    for unit, (size, done, complete) in totals.items():
        out.append((group_of(unit), unit, size, done, complete))
    return out


def group_of(unit):
    parts = unit.split('/')
    if parts[0] in ('Source', 'Meta') and len(parts) > 2:
        return '/'.join(parts[:2])
    return parts[0] if len(parts) > 1 else '(root)'


# --- layout ------------------------------------------------------------------------------------

def _worst(row, side):
    s = sum(row)
    if s <= 0 or side <= 0:
        return float('inf')
    return max(max(side * side * r / (s * s), (s * s) / (side * side * r)) for r in row)


def squarify(values, x, y, w, h):
    """Squarified treemap: rectangles (x, y, w, h) for values (already sorted largest first,
    all > 0), filling the box exactly. Returns them in the input order."""
    total = float(sum(values))
    if total <= 0 or w <= 0 or h <= 0:
        return [(x, y, 0.0, 0.0) for _ in values]
    scale = w * h / total
    areas = [v * scale for v in values]
    rects = []
    i = 0
    while i < len(areas):
        side = min(w, h)
        row = [areas[i]]
        j = i + 1
        while j < len(areas) and _worst(row + [areas[j]], side) <= _worst(row, side):
            row.append(areas[j])
            j += 1
        s = sum(row)
        # The last row takes all the space that is left, so rounding never leaves a gap.
        if w >= h:  # lay the row down the left edge
            col_w = s / h if j < len(areas) else w
            yy = y
            for k, a in enumerate(row):
                hh = a / col_w if k < len(row) - 1 else y + h - yy
                rects.append((x, yy, col_w, hh))
                yy += hh
            x += col_w
            w -= col_w
        else:       # along the top edge
            row_h = s / w if j < len(areas) else h
            xx = x
            for k, a in enumerate(row):
                ww = a / row_h if k < len(row) - 1 else x + w - xx
                rects.append((xx, y, ww, row_h))
                xx += ww
            y += row_h
            h -= row_h
        i = j
    return rects


def color(done_fraction, complete):
    if complete:
        c = COLOR_DONE
    else:
        f = max(0.0, min(1.0, done_fraction))
        c = tuple(round(a + (b - a) * f) for a, b in zip(COLOR_EMPTY, COLOR_PARTIAL))
    return '#%02x%02x%02x' % c


# --- output ------------------------------------------------------------------------------------

def treemap_svg(units, width=WIDTH, height=HEIGHT):
    groups = {}
    for g, unit, size, done, complete in units:
        groups.setdefault(g, []).append((unit, size, done, complete))
    order = sorted(groups.items(), key=lambda kv: (-sum(u[1] for u in kv[1]), kv[0]))
    out = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
           f'viewBox="0 0 {width} {height}">',
           f'<rect width="{width}" height="{height}" fill="#1b1d1b"/>',
           '<g stroke="#1b1d1b" stroke-width="0.5">']
    boxes = squarify([sum(u[1] for u in us) for _, us in order], 0, 0, width, height)
    for (g, us), (gx, gy, gw, gh) in zip(order, boxes):
        gx, gy = gx + GROUP_GAP / 2, gy + GROUP_GAP / 2
        gw, gh = max(gw - GROUP_GAP, 0), max(gh - GROUP_GAP, 0)
        us = sorted(us, key=lambda u: (-u[1], u[0]))
        out.append(f'<g><title>{html.escape(g)}</title>')
        for (unit, size, done, complete), (x, y, w, h) in zip(
                us, squarify([u[1] for u in us], gx, gy, gw, gh)):
            pct = 100.0 * done / size
            out.append(f'<rect x="{x:.1f}" y="{y:.1f}" width="{w:.1f}" height="{h:.1f}" '
                       f'fill="{color(done / size, complete)}"><title>{html.escape(unit)}: '
                       f'{pct:.1f}% of {size:,} bytes</title></rect>')
        out.append('</g>')
    out.append('</g>')
    # A faint outline around each top-level directory, so its units read as one block.
    out.append('<g fill="none" stroke="#5a625a" stroke-width="1">')
    for (gx, gy, gw, gh) in boxes:
        out.append(f'<rect x="{gx + 0.5:.1f}" y="{gy + 0.5:.1f}" width="{max(gw - 1, 0):.1f}" '
                   f'height="{max(gh - 1, 0):.1f}"/>')
    out.append('</g>')
    out.append('</svg>')
    return '\n'.join(out) + '\n'


def _text_width(s):
    # Verdana 11px, roughly, per character.
    narrow, wide = '.,:;il ', '%mwMW'
    return sum(3.9 if c in narrow else 10.0 if c in wide else 7.0 for c in s)


def badge_svg(label, value):
    lw = round(_text_width(label) + 12)
    vw = round(_text_width(value) + 12)
    w = lw + vw
    return (f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="20" role="img" '
            f'aria-label="{label}: {value}"><title>{label}: {value}</title>'
            f'<clipPath id="r"><rect width="{w}" height="20" rx="3" fill="#fff"/></clipPath>'
            f'<g clip-path="url(#r)"><rect width="{lw}" height="20" fill="{BADGE_LABEL}"/>'
            f'<rect x="{lw}" width="{vw}" height="20" fill="{BADGE_VALUE}"/></g>'
            f'<g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,'
            f'sans-serif" font-size="11"><text x="{lw / 2:.1f}" y="14">{label}</text>'
            f'<text x="{lw + vw / 2:.1f}" y="14">{value}</text></g></svg>\n')


def write(path, text):
    with open(path, 'w', newline='\n') as f:
        f.write(text)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--out', default=str(PROGRESS))
    ap.add_argument('--progress', default=str(PROGRESS / 'progress.json'))
    ap.add_argument('--functions', default=str(ROOT / 'ghidra' / 'exports' / 'functions.csv'))
    ap.add_argument('--units', default=str(ROOT / 'game' / 'UNITS.tsv'))
    ap.add_argument('--matched-set', default=str(ROOT / 'analysis' / 'results' / 'matched_set.json'))
    a = ap.parse_args(argv)
    with open(a.progress) as f:
        progress = json.load(f)
    matched = {}
    if os.path.exists(a.matched_set):
        with open(a.matched_set) as f:
            matched = json.load(f)
    exact, functional, named = badge_values(progress)
    os.makedirs(a.out, exist_ok=True)
    for name, label, v in (('exact', 'Exact', exact), ('functional', 'Functional', functional),
                           ('named', 'Named', named)):
        write(os.path.join(a.out, f'badge-{name}.svg'), badge_svg(label, f'{v:.2f}%'))
    units = read_units(a.functions, a.units, progress, matched)
    write(os.path.join(a.out, 'treemap.svg'), treemap_svg(units))
    done = sum(1 for u in units if u[4])
    print(f'exact {exact:.2f}%  functional {functional:.2f}%  named {named:.2f}%  '
          f'units {len(units)} ({done} complete) -> {a.out}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
