#!/usr/bin/env python3
"""Summarise the P0-3 exports and step logs as Markdown (ghidra/results/counts.md).

Usage: counts.py <repo root>. Reads only files the pipeline wrote; every number in REPORT.md comes
from here.
"""
import collections
import csv
import os
import re
import sys

csv.field_size_limit(1 << 24)


def rows(path, delim=","):
    with open(path, newline="") as f:
        return list(csv.DictReader(f, delimiter=delim))


def table(headers, data):
    out = ["| " + " | ".join(headers) + " |", "|" + "---|" * len(headers)]
    out += ["| " + " | ".join(str(c) for c in r) + " |" for r in data]
    return "\n".join(out)


def main():
    repo = sys.argv[1]
    exp = os.path.join(repo, "ghidra", "exports")
    res = os.path.join(repo, "ghidra", "results")
    out = []
    p = out.append

    funcs = rows(os.path.join(exp, "functions.csv"))
    kinds = collections.Counter(f["kind"] for f in funcs)
    real = [f for f in funcs if f["kind"] == "function"]
    p("## Functions\n")
    p(f"- total functions in the program: **{len(funcs)}** "
      f"({', '.join(f'{k} {v}' for k, v in kinds.most_common())})")
    total_bytes = sum(int(f["size"]) for f in real)
    named = [f for f in real if f["name_source"] != "default"]
    named_bytes = sum(int(f["size"]) for f in named)
    p(f"- ordinary functions (excluding thunks and EH funclets): {len(real)}, {total_bytes} bytes")
    p(f"- of those with a non-default name: **{len(named)}** ({100*len(named)/len(real):.1f}%), "
      f"{named_bytes} bytes ({100*named_bytes/total_bytes:.1f}% of function bytes)\n")
    src = collections.Counter((f["kind"], f["name_source"]) for f in funcs)
    p(table(["kind", "name_source", "functions"],
            [(k, s, n) for (k, s), n in sorted(src.items(), key=lambda x: -x[1])]))
    p("")
    # class recovery names are partly placeholders (Class::vfunctionN): only the class is known
    quality = collections.Counter()
    for f in real:
        if f["name_source"] == "default":
            continue
        leaf = f["name"].split("::")[-1]
        if f["name_source"] == "rtti-class-recovery" and (
                re.match(r"vfunction\d+", leaf) or "Constructor_or_Destructor" in leaf):
            quality["class known, method placeholder (vfunctionN / ctor-or-dtor)"] += 1
        elif f["name_source"] == "rtti-class-recovery":
            quality["class recovery: constructor / destructor / deleting destructor"] += 1
        else:
            quality[f"real symbol name ({f['name_source']})"] += 1
    p(table(["ordinary functions by name quality", "functions"], quality.most_common()))
    p("")

    p("## Per-step function counts\n")
    steps = []
    for name, label in (("stats_01_import.txt", "after auto-analysis (step 1)"),
                        ("stats_01b_rtti_gaps.txt", "after VC6 RTTI + gap fill (step 1b)"),
                        ("stats_01b_classes.txt", "after class recovery (step 1b)"),
                        ("stats_02_fid.txt", "after SDK FID (step 2)"),
                        ("stats_03_exact.txt", "after exact matches (step 3)")):
        path = os.path.join(res, name)
        if os.path.exists(path):
            kv = dict(l.rstrip("\n").split("\t", 1) for l in open(path) if "\t" in l)
            steps.append((label, kv.get("functions"), kv.get("function_bytes"),
                          kv.get("source.DEFAULT"), kv.get("classes"),
                          kv.get("bookmark.Error.Bad Instruction", "0")))
    p(table(["state", "functions", "function bytes", "default-named", "class namespaces",
             "bad-instruction bookmarks"], steps))
    p("")
    gap = os.path.join(res, "gap_fill.tsv")
    if os.path.exists(gap):
        g = rows(gap, "\t")
        created = sum(int(r["functions_created"]) for r in g if r["functions_created"])
        final = [r for r in g if r["pass"] == "#final"]
        p(f"- gap fill: {created} functions created over {len(g) - len(final)} passes; undefined "
          f".text bytes {g[0]['undefined_bytes_before']} -> "
          f"{final[0]['undefined_bytes_before'] if final else '?'}\n")

    p("## Library flag\n")
    lib = collections.Counter(f["library"] or "(game code)" for f in funcs if f["kind"] != "eh-funclet")
    p(table(["library", "functions"], lib.most_common()))
    p("")

    p("## SDK Function ID (step 2)\n")
    build = rows(os.path.join(res, "fidb_build.tsv"), "\t")
    p(table(["archive", "programs", "functions attempted", "added to fidb", "excluded",
             "exclusions"],
            [(b["library"], b["programs"], b["attempted"], b["added"], b["excluded"],
              b["exclusion_breakdown"]) for b in build]))
    fid = rows(os.path.join(res, "fid_sdk_matches.tsv"), "\t")
    disp = collections.Counter(r["disposition"] for r in fid)
    p("")
    p(f"- functions with an SDK FID hit (score >= 14.6): {len(fid)}; dispositions: "
      + ", ".join(f"{k} {v}" for k, v in disp.most_common()))
    p(f"- functions whose final name comes from SDK FID: **{sum(1 for f in funcs if f['name_source'] == 'sdk-fid')}**\n")

    p("## Exact SDK matches (step 3)\n")
    ex = rows(os.path.join(res, "exact_apply.tsv"), "\t")
    events = collections.Counter(r["address"].startswith("#") and "summary" or r["event"] for r in ex)
    summary = [r for r in ex if r["address"].startswith("#")]
    if summary:
        p("- " + summary[0]["event"].replace(" ", ", "))
    p("- events: " + ", ".join(f"{k} {v}" for k, v in events.most_common() if k != "summary"))
    p(f"- functions whose final name comes from the exact table: "
      f"**{sum(1 for f in funcs if f['name_source'] == 'sdklib-exact')}**\n")

    p("## Translation units (step 4)\n")
    tu = rows(os.path.join(res, "tu_paths.tsv"), "\t")
    paths = {r["path"] for r in tu}
    src_paths = {r["path"] for r in tu if r["kind"] == "source"}
    with_tu = [f for f in funcs if f["tu_evidence"] == "assert-path"]
    multi = [f for f in with_tu if ";" in f["translation_unit"]]
    neigh = [f for f in funcs if f["tu_evidence"] == "neighbour"]
    p(f"- source-path strings referenced from code: {len(paths)} ({len(src_paths)} .c/.cpp, "
      f"{len(paths) - len(src_paths)} headers)")
    p(f"- functions with a TU from a path string they reference: **{len(with_tu)}** "
      f"({len(multi)} reference more than one .cpp)")
    p(f"- functions with a TU inferred from both neighbours (same single TU before and after): "
      f"{len(neigh)}")
    p(f"- functions with only header paths: "
      f"{sum(1 for f in funcs if f['headers'] and not f['translation_unit'])}")
    per_tu = collections.Counter(f["translation_unit"] for f in funcs if f["translation_unit"]
                                 and ";" not in f["translation_unit"])
    p(f"- distinct TUs with at least one function: {len(per_tu)}\n")

    p("## Classes (step 1b)\n")
    cls = rows(os.path.join(exp, "classes.csv"))
    vts = rows(os.path.join(exp, "vtables.csv"))
    p(f"- classes (RTTI type descriptors / hierarchy descriptors / vftables): **{len(cls)}**")
    p(f"- with a hierarchy descriptor (parents known): "
      f"{sum(1 for c in cls if c['parents'] or c['all_bases'])} with at least one base; "
      f"with vftables: {sum(1 for c in cls if c['vtables'])}; "
      f"with a recovered structure size: {sum(1 for c in cls if c['size'])}")
    p(f"- vftables: {len({v['vtable_address'] for v in vts})}, slots: {len(vts)}, "
      f"slots pointing at a named function: "
      f"{sum(1 for v in vts if v['function'] and not v['function'].startswith('FUN_'))}\n")

    p("## Largest unnamed regions\n")
    reg = rows(os.path.join(exp, "unnamed_regions.csv"))
    p(table(["start", "end", "functions", "bytes", "named before", "named after"],
            [(r["start"], r["end"], r["functions"], r["bytes"], r["named_before"],
              r["named_after"]) for r in reg[:25]]))
    p("")

    p("## Timings and memory\n")
    tim = []
    with open(os.path.join(res, "timings.tsv")) as f:
        for line in f:
            c = line.rstrip("\n").split("\t")
            if len(c) >= 3:
                tim.append(c[:4])
    p(table(["step", "wall time", "peak RSS", "rc"], tim))
    print("\n".join(out))


if __name__ == "__main__":
    main()
