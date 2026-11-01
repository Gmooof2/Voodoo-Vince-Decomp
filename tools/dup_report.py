"""Duplicate-body report over the prototype and the T9-located retail functions (T10, phase 1.2).

CLI: dup_report.py   (no args)
Reports (each population split into classes, counts summed and asserted, D24):
 1 proto: identical masked bodies among default.pdb procs (dedup by VA as proto_map); excluded procs (zero-length,
   no-raw-bytes, data-in-text, thunk) are a class, never grouped.
 2 retail: identical masked bodies among retail VAs T9 located (identical + ambiguous targetVAs of
   derived/proto_map/retail-xbe/functions.jsonl; extent = mapped proto length, max if several); near-identical
   candidateVAs only = class no-extent. The rest of retail is unknown until 1.3's ingest.
 3 retail VAs that >=2 distinct proto procs map to (ICF/COMDAT-fold candidates), split by mapping kind.
 4 proto VAs with several PDB names: >=2 proc names, >=2 public names, combined (either).
Mask: proto exactly as proto_map (decoded masks over [imageBase, +SizeOfImage) plus .reloc sites inside the proc);
retail = mask_proc(buf, va, baseAddr, baseAddr+sizeOfImage), no relocs. Body key = sha1(length, masked-zeroed
bytes), in memory only. Writes derived/dup_report/{proto_groups,retail_groups,retail_folds,multi_name}.jsonl +
summary.json (names, VAs, lengths, group ids; status_note unverified; never bytes or hashes).
Exit: 0 ok; 1 a class sum failed; 2 refusal (inputs not matching the manifest, PE/PDB pair invalid, unparseable,
functions.jsonl missing or its targetSha1 != manifest retail sha1). Prints counts, addresses and names only (D46).
"""
import bisect
import hashlib
import json
import sys
from collections import Counter, defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

import pdb_info  # noqa: E402
import pe_info  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402
from proto_map import _check_manifest, _pe_extra, _rva_off, is_thunk, mask_proc, parse_reloc_blocks  # noqa: E402

DERIVED = REPO / "derived"
FUNCS = DERIVED / "proto_map" / "retail-xbe" / "functions.jsonl"
BUCKETS = (("<16", 0, 16), ("16-63", 16, 64), (">=64", 64, 1 << 62))
FAILED = []


def _refuse(msg):
    print(f"dup_report: {msg}", file=sys.stderr)
    sys.exit(2)


def body_key(buf, masks):
    b = bytearray(buf)
    for o, w, _ in masks:
        b[o:o + w] = bytes(len(b[o:o + w]))
    return hashlib.sha1(len(b).to_bytes(4, "little") + bytes(b)).hexdigest()


def check(label, parts, total):
    s = sum(parts.values())
    ok = s == total
    if not ok:
        FAILED.append(label)
    cls = ", ".join(f"{k} {v}" for k, v in parts.items())
    print(f"  {label}: {cls}; sum {s}/{total} {'ok' if ok else 'FAILED'}")


def group(items, prefix):
    """items [(va, key, length, names)] -> sorted groups (size >= 2) with ids, and va -> gid."""
    by = defaultdict(list)
    for it in items:
        by[it[1]].append(it)
    groups = sorted((sorted(m, key=lambda t: (t[0], t[3])) for m in by.values() if len(m) >= 2),
                    key=lambda m: (m[0][0], m[0][3]))
    gid = {}
    out = []
    for i, m in enumerate(groups, 1):
        g = f"{prefix}{i:04d}"
        for t in m:
            gid[t[0]] = g
        out.append({"group": g, "size": len(m), "length": m[0][2],
                    "members": [{"va": f"0x{t[0]:08X}", "names": t[3]} for t in m]})
    return out, gid


def breakdown(label, items, groups, gid):
    sizes = Counter(g["size"] for g in groups)
    print(f"  {label} group sizes: " + ", ".join(f"{k}x{sizes[k]}" for k in sorted(sizes)))
    for name, a, b in BUCKETS:
        pop = [t for t in items if a <= t[2] < b]
        ing = sum(t[0] in gid for t in pop)
        ng = sum(a <= g["length"] < b for g in groups)
        print(f"  {label} length {name} B: population {len(pop)}, in-group {ing}, groups {ng}")


def main():
    cfg = vv_inputs.load_config()
    records = vv_inputs.load_manifest()
    exe_p = vv_inputs.input_path("prototype_dir", "default.exe", cfg)
    pdb_p = vv_inputs.input_path("prototype_dir", "default.pdb", cfg)
    xbe_p = vv_inputs.input_path("retail_xbe", None, cfg)
    _check_manifest(exe_p, cfg, records)
    _check_manifest(pdb_p, cfg, records)
    xsha1 = _check_manifest(xbe_p, cfg, records)
    if not FUNCS.is_file():
        _refuse(f"{FUNCS.relative_to(REPO)} missing (run proto_map.py first)")
    rows = [json.loads(ln) for ln in FUNCS.read_text(encoding="ascii").splitlines() if ln.strip()]
    shas = {r.get("targetSha1") for r in rows}
    if shas != {xsha1}:
        _refuse(f"functions.jsonl targetSha1 {sorted(map(str, shas))} != manifest retail sha1 {xsha1}")
    exe, pdb_b, xbe = exe_p.read_bytes(), pdb_p.read_bytes(), xbe_p.read_bytes()
    try:
        pe = pe_info.parse_pe(exe)
        pdb = pdb_info.parse_pdb(pdb_b)
        x = xbe_info.parse_xbe(xbe)
    except (pe_info.NotPe, pdb_info.NotPdb, xbe_info.NotXbe) as e:
        _refuse(f"unparseable input: {e}")
    bad = pdb_info.validate(pdb, pe["rsds"])
    if bad:
        _refuse(f"default.pdb / default.exe pair invalid: {'; '.join(bad)}")
    out = (DERIVED / "dup_report").resolve()
    base = pe["imageBase"]
    size_img, rel_rva, rel_sz = _pe_extra(exe)
    lo, hi = base, base + size_img
    relocs = set()
    if rel_rva and rel_sz:
        ro = _rva_off(pe, rel_rva)
        if ro is not None:
            relocs = parse_reloc_blocks(exe[ro:ro + rel_sz], base)
    rsorted = sorted(relocs)

    # proc table exactly as proto_map.run
    procs = {}
    for m in pdb["modules"]:
        for p in m["procs"]:
            if not 1 <= p["seg"] <= len(pe["sections"]):
                continue
            s = pe["sections"][p["seg"] - 1]
            va = base + s["va"] + p["off"]
            e = procs.setdefault(va, {"names": [], "length": 0, "sec": s, "off": p["off"]})
            if p["name"] not in e["names"]:
                e["names"].append(p["name"])
            e["length"] = max(e["length"], p["length"])

    # report 1
    p_items, p_exc = [], Counter()
    for va in sorted(procs):
        e = procs[va]
        n, s = e["length"], e["sec"]
        if n == 0:
            p_exc["zero-length"] += 1
            continue
        if e["off"] + n > s["rawsize"]:
            p_exc["no-raw-bytes"] += 1
            continue
        buf = exe[s["rawptr"] + e["off"]:s["rawptr"] + e["off"] + n]
        masks, errs = mask_proc(buf, va, lo, hi)
        if errs:
            p_exc["data-in-text"] += 1
            continue
        if is_thunk(buf, va):
            p_exc["thunk"] += 1
            continue
        i0 = bisect.bisect_left(rsorted, va)
        i1 = bisect.bisect_left(rsorted, va + n)
        masks = sorted(set(masks) | {(r - va, 4, "abs") for r in rsorted[i0:i1]})
        p_items.append((va, body_key(buf, masks), n, sorted(e["names"])))
    p_groups, p_gid = group(p_items, "P")
    print(f"dup_report: retail sha1 {xsha1}; functions.jsonl rows {len(rows)}")
    print("1 proto: population = default.pdb procs deduped by VA "
          f"({len(procs)})")
    in_g = sum(t[0] in p_gid for t in p_items)
    check("classes", {"unique": len(p_items) - in_g, "in-group": in_g,
                      **{f"excluded {k}": p_exc[k] for k in ("zero-length", "no-raw-bytes", "data-in-text", "thunk")}},
          len(procs))
    print(f"  groups {len(p_groups)}")
    breakdown("proto", p_items, p_groups, p_gid)

    # report 2
    ext, near, maps = {}, set(), defaultdict(dict)  # maps: retail va -> {protoVA: kind}
    rnames = defaultdict(set)
    for r in rows:
        pva = int(r["protoVA"], 16)
        if r["class"] in ("identical", "ambiguous"):
            kind = "identical-unique" if r["class"] == "identical" else "ambiguous-hit"
            for t in r["targetVAs"]:
                v = int(t, 16)
                ext[v] = max(ext.get(v, 0), r["length"])
                maps[v][pva] = kind
                rnames[v].update(r["names"])
        elif r["class"] == "near-identical" and r["candidateVA"]:
            v = int(r["candidateVA"], 16)
            near.add(v)
            maps[v].setdefault(pva, "near candidate")
    xsecs = sorted((s["virtualAddr"], s["rawAddr"], s["rawSize"]) for s in x["sections"])
    tlo = x["header"]["baseAddr"]
    thi = tlo + x["header"]["sizeOfImage"]
    r_items, r_exc = [], Counter()
    for v in sorted(ext):
        n = ext[v]
        k = bisect.bisect_right(xsecs, (v, 1 << 62, 0)) - 1
        if k < 0 or not xsecs[k][0] <= v or v + n > xsecs[k][0] + xsecs[k][2]:
            r_exc["no-raw-bytes"] += 1
            continue
        o = xsecs[k][1] + v - xsecs[k][0]
        buf = xbe[o:o + n]
        masks, errs = mask_proc(buf, v, tlo, thi)
        if errs:
            r_exc["data-in-text"] += 1
            continue
        r_items.append((v, body_key(buf, masks), n, sorted(rnames[v])))
    r_groups, r_gid = group(r_items, "R")
    pop = set(ext) | near
    no_ext = len(near - set(ext))
    print(f"2 retail: population = retail VAs located by T9 (identical/ambiguous targetVAs + near candidateVAs) "
          f"({len(pop)}); rest of retail unknown until 1.3's ingest")
    in_g = sum(t[0] in r_gid for t in r_items)
    check("classes", {"unique": len(r_items) - in_g, "in-group": in_g, "no-extent": no_ext,
                      **{f"excluded {k}": r_exc[k] for k in ("no-raw-bytes", "data-in-text")}}, len(pop))
    print(f"  groups {len(r_groups)}")
    breakdown("retail", r_items, r_groups, r_gid)

    # report 3
    folds, fcls = [], Counter()
    for v in sorted(pop):
        m = maps[v]
        if len(m) < 2:
            fcls["single proto"] += 1
            continue
        kinds = set(m.values())
        k = next(iter(kinds)) if len(kinds) == 1 else "mixed"
        fcls[f"multi {k}"] += 1
        pg = {p_gid.get(p) for p in m}
        folds.append({"retailVA": f"0x{v:08X}", "kind": k, "retailGroup": r_gid.get(v),
                      "protoOneGroup": len(pg) == 1 and None not in pg,
                      "protos": [{"protoVA": f"0x{p:08X}", "kind": m[p], "protoGroup": p_gid.get(p),
                                  "names": sorted(procs[p]["names"]) if p in procs else []} for p in sorted(m)]})
    print(f"3 retail folds: population = report-2 retail VAs ({len(pop)})")
    check("classes", {k: fcls[k] for k in ("single proto", "multi identical-unique", "multi ambiguous-hit",
                                           "multi near candidate", "multi mixed")}, len(pop))
    one = sum(f["protoOneGroup"] for f in folds)
    print(f"  consistency: {one}/{len(folds)} multi-proto VAs have all their protos in one report-1 group")

    # report 4
    pubs = defaultdict(list)
    for p in pdb["publics"]:
        if not 1 <= p["seg"] <= len(pe["sections"]):
            continue
        va = base + pe["sections"][p["seg"] - 1]["va"] + p["off"]
        if p["name"] not in pubs[va]:
            pubs[va].append(p["name"])
    mp = {v for v in procs if len(procs[v]["names"]) >= 2}
    mu = {v for v in pubs if len(pubs[v]) >= 2}
    allv = set(procs) | set(pubs)
    print("4 multi-name: populations = proc VAs, public VAs (valid seg), union")
    print(f"  proc VAs ({len(procs)}):")
    check("classes", {"1 name": len(procs) - len(mp), ">=2 names": len(mp)}, len(procs))
    print(f"  public VAs ({len(pubs)}):")
    check("classes", {"1 name": len(pubs) - len(mu), ">=2 names": len(mu)}, len(pubs))
    print(f"  union VAs ({len(allv)}), several = >=2 proc names or >=2 public names:")
    check("classes", {"single": len(allv - mp - mu), "procs only": len(mp - mu), "publics only": len(mu - mp),
                      "both": len(mp & mu)}, len(allv))
    multi = [{"va": f"0x{v:08X}", "procNames": sorted(procs[v]["names"]) if v in procs else [],
              "publicNames": sorted(pubs.get(v, [])), "multiProc": v in mp, "multiPublic": v in mu}
             for v in sorted(mp | mu)]

    tag = {"source": "dup_report", "targetSha1": xsha1, "status_note": "unverified"}
    out.mkdir(parents=True, exist_ok=True)
    for fn, recs in (("proto_groups.jsonl", p_groups), ("retail_groups.jsonl", r_groups),
                     ("retail_folds.jsonl", folds), ("multi_name.jsonl", multi)):
        with open(out / fn, "w", encoding="ascii", newline="\n") as f:
            for r in recs:
                f.write(json.dumps({**r, **tag}, ensure_ascii=True, sort_keys=True) + "\n")
    summ = {**tag, "protoProcs": len(procs), "protoGroups": len(p_groups), "protoExcluded": dict(p_exc),
            "retailPopulation": len(pop), "retailGroups": len(r_groups), "retailNoExtent": no_ext,
            "retailExcluded": dict(r_exc), "foldClasses": dict(fcls), "foldsProtoOneGroup": one,
            "multiProcName": len(mp), "multiPublicName": len(mu), "sumCheckFailed": FAILED}
    (out / "summary.json").write_text(json.dumps(summ, indent=1, sort_keys=True) + "\n", encoding="ascii")
    if FAILED:
        print(f"sum check FAILED: {', '.join(FAILED)}")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
