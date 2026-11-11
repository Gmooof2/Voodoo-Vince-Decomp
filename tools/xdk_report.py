"""XDK library identification report (T9, phase 1.3).

CLI: xdk_report.py [--config P]
  1 modules: every default.pdb module classed linker (name `* Linker *`) | xdk | game. xdk = (a) S_OBJNAME under
    D:\\xboxfre-jun03\\ (case-insensitive) or (b) obj library basename minus .lib in the retail XBE library table
    (read at runtime) or (c) obj library under ...\\Microsoft Xbox SDK\\Xbox\\Lib\\. Counts by module, proc, bytes.
  2 xdk procs -> derived/proto_map/retail-xbe/functions.jsonl rows, joined on protoVA (the PDB proc's VA in
    default.exe); split identical/ambiguous/near-identical/absent/excluded(/no-row) by count and proc bytes.
  3 cross-check: retail VAs of xdk procs (identical hits 1: targetVAs[0]; near: candidateVA) vs T5 labels with
    xbsymdb true (and, second line, xtlid true): agree / pdb-only / label-only by count and T5 function size.
  4 bucket: union of population function bodies [bodyMin, bodyMax] inside EXECUTABLE XBE sections, per byte,
    attributed game/xdk/linker/mixed from mapped retail ranges (identical, near, ambiguous when all its modules share
    a class), then xdk-label (population function whose entry carries a T5 xbsymdb/xtlid label), rest unidentified.
    Population (T7, 1.5) = T5 applied/functions.jsonl when applied/summary.json exists (its executableSHA256 must be
    the pinned XBE's, and delink.cut_check must pass (T1 2.1), else refuse; superset of T5 checked: a T5-only entry must be absorbed (body inside one applied function), 0 T5-only body bytes), else T5
    functions.jsonl. Crosscheck (3) and labels always use the T5 pre-apply files.
  5 section-name signal: exec section name vs library-table names (exact, prefix, abbreviation) + dominant class.
Outputs derived/xdk/retail-<sha1[:8]>/ (cleared first): modules.jsonl, xdk_procs.jsonl, crosscheck.jsonl,
  unidentified.jsonl, functions.jsonl (one per population function: entry, size, section, class = bucket class of
  its entry byte, "outside" if in no exec section), summary.json (summary["functions"] = {source, count, byClass};
  summary["ingestSha256"] = ingest_sha256(pin) over exactly the ingest files read: derived/ghidra/retail-<pin8>/
  summary.json, functions.jsonl, symbols.jsonl, plus applied/summary.json and applied/functions.jsonl when applied/
  is used; progress.py and recut_proof.py refuse a report whose value != the current ingest's (5.4 T1)).
  Prints counts, names and addresses only, never bytes (D46).
Exit: 0 ok; 1 a sum check failed; 2 refusal (input missing, proto_map targetSha1 or T5/applied summary sha != pin).
"""
import argparse
import hashlib
import json
import shutil
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff_info  # noqa: E402
import delink  # noqa: E402
import pdb_info  # noqa: E402
import pe_info  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

REPO = Path(__file__).resolve().parent.parent
PROTO = REPO / "derived" / "proto_map" / "retail-xbe"
SEC_EXEC = 0x4
LINKER = "* Linker *"
XDK_BUILD = "d:\\xboxfre-jun03\\"
XDK_LIB = "\\microsoft xbox sdk\\xbox\\lib\\"
CLASSES = ("game", "xdk", "linker")
MAP_CLASSES = ("identical", "ambiguous", "near-identical", "absent", "excluded", "no-row")
BIT = {"game": 1, "xdk": 2, "linker": 4}
BUCKET = ("game", "xdk", "linker", "mixed", "xdk-label", "unidentified")
FAILS = []


def _refuse(msg):
    print(f"xdk_report: refused: {msg}", file=sys.stderr)
    sys.exit(2)


def _check(label, got, want):
    ok = got == want
    if not ok:
        FAILS.append(label)
    print(f"  sum check {label}: {got} == {want} {'ok' if ok else 'FAIL'}")


def _jsonl(path):
    if not path.is_file():
        _refuse(f"input missing: {path}")
    with open(path, encoding="utf-8") as f:
        return [json.loads(line) for line in f if line.strip()]


def _hx(v):
    return f"0x{v:08X}"


def classify(m, libnames):
    """(class, criteria) of one PDB module."""
    if m["name"] == LINKER:
        return "linker", ["linker"]
    crit = []
    if (m.get("objname") or "").lower().startswith(XDK_BUILD):
        crit.append("a")
    obj = (m.get("obj") or "").lower()
    if obj.endswith(".lib"):
        base = obj.replace("/", "\\").rsplit("\\", 1)[-1][:-4]
        if base.upper() in libnames:
            crit.append("b")
        if XDK_LIB in obj.replace("/", "\\"):
            crit.append("c")
    return ("xdk" if crit else "game"), crit


def name_signal(sec, libnames):
    """(lib, kind) of the best library-table match for a section name, or (None, None)."""
    s = sec.upper()
    if s in libnames:
        return s, "exact"
    pre = sorted((n for n in libnames if n.startswith(s) or s.startswith(n)), key=lambda n: (len(n), n))
    if pre and s[:1].isalpha():
        return pre[0], "prefix"

    def subseq(a, b):
        it = iter(b)
        return all(ch in it for ch in a)
    ab = sorted(n for n in libnames if len(s) >= 3 and s[:1] == n[:1] and subseq(s, n))
    if ab:
        return ab[0], "abbreviation"
    return None, None


INGEST_FILES = ("summary.json", "functions.jsonl", "symbols.jsonl")
APPLIED_FILES = ("applied/summary.json", "applied/functions.jsonl")


def ingest_sha256(pin):
    """sha256 over exactly the ingest files main() reads from derived/ghidra/retail-<pin8>/ (INGEST_FILES, plus
    APPLIED_FILES when applied/summary.json exists): per file its relative name, NUL, its byte length, NUL, its bytes.
    -> hex digest, or None when a file is missing. progress.py and recut_proof.py recompute it with this function."""
    t5 = REPO / "derived" / "ghidra" / f"retail-{pin[:8]}"
    names = INGEST_FILES + (APPLIED_FILES if (t5 / "applied" / "summary.json").is_file() else ())
    h = hashlib.sha256()
    for n in names:
        p = t5 / n
        if not p.is_file():
            return None
        b = p.read_bytes()
        h.update(n.encode() + b"\0" + str(len(b)).encode() + b"\0" + b)
    return h.hexdigest()


def main(argv):
    ap = argparse.ArgumentParser(prog="xdk_report.py", description=__doc__.splitlines()[0])
    ap.add_argument("--config", help="config path (default config/local_paths.json)")
    args = ap.parse_args(argv)
    cfg = vv_inputs.load_config(args.config)
    pin = coff_info.pinned_sha()
    if not pin:
        _refuse("no retail_xbe pin in config/check.retail_xbe.sha")
    xbe_b = vv_inputs.input_path("retail_xbe", cfg=cfg).read_bytes()
    if hashlib.sha1(xbe_b).hexdigest() != pin:
        _refuse("retail_xbe sha1 != pin")
    sha256 = hashlib.sha256(xbe_b).hexdigest()
    exe_b = vv_inputs.input_path("prototype_dir", "default.exe", cfg).read_bytes()
    pdb_b = vv_inputs.input_path("prototype_dir", "default.pdb", cfg).read_bytes()
    t5 = REPO / "derived" / "ghidra" / f"retail-{pin[:8]}"
    if not (PROTO / "summary.json").is_file() or not (t5 / "summary.json").is_file():
        _refuse(f"input missing: {PROTO / 'summary.json'} or {t5 / 'summary.json'}")
    psum = json.loads((PROTO / "summary.json").read_text(encoding="utf-8"))
    rows = _jsonl(PROTO / "functions.jsonl")
    if psum.get("targetSha1") != pin or any(r["targetSha1"] != pin for r in rows):
        _refuse("proto_map targetSha1 != pin")
    tsum = json.loads((t5 / "summary.json").read_text(encoding="utf-8"))
    if (tsum.get("executableSHA256") or "").lower() != sha256:
        _refuse("T5 summary executableSHA256 != sha256 of the pinned retail XBE")
    funcs = _jsonl(t5 / "functions.jsonl")
    syms = _jsonl(t5 / "symbols.jsonl")
    if (t5 / "applied" / "summary.json").is_file():  # bucket population: post-ApplySymbols bodies (T7, 1.5)
        why = delink.cut_check(delink.cut_dir(pin))  # T1 (2.1): applied/ is the cut's; valid cut.complete first
        if why:
            _refuse(why)
        asum = json.loads((t5 / "applied" / "summary.json").read_text(encoding="utf-8"))
        if (asum.get("executableSHA256") or "").lower() != sha256:
            _refuse("T5 applied summary executableSHA256 != sha256 of the pinned retail XBE")
        pop_src, pop_funcs = "applied", _jsonl(t5 / "applied" / "functions.jsonl")
    else:
        pop_src, pop_funcs = "t5", funcs
    try:
        x = xbe_info.parse_xbe(xbe_b)
        pe = pe_info.parse_pe(exe_b)
        pdb = pdb_info.parse_pdb(pdb_b)
    except (xbe_info.NotXbe, pe_info.NotPe, pdb_info.NotPdb) as e:
        _refuse(f"unparseable input: {e}")
    bad = pdb_info.validate(pdb, pe["rsds"])
    if bad:
        _refuse(f"default.pdb / default.exe pair invalid: {'; '.join(bad)}")
    libnames = {lib["name"].upper() for lib in x["libraries"]}
    out = REPO / "derived" / "xdk" / f"retail-{pin[:8]}"
    summ = {"targetSha1": pin, "libraryTable": sorted(libnames), "source": "xdk_report",
            "status_note": "unverified", "ingestSha256": ingest_sha256(pin)}  # 5.4 T1: the stale-report guard

    # 1 modules
    print(f"library table ({len(libnames)}): {' '.join(sorted(libnames))}")
    print("rule: linker = name '* Linker *'; xdk = (a) objname under D:\\xboxfre-jun03\\ or (b) obj lib basename "
          "in library table or (c) obj lib under Microsoft Xbox SDK\\Xbox\\Lib\\; else game")
    base = pe["imageBase"]
    mods, modcls, va_mods = [], {}, {}
    mc, pc, bc, crit_c = Counter(), Counter(), Counter(), Counter()
    xprocs = []
    for m in pdb["modules"]:
        cls, crit = classify(m, libnames)
        modcls[m["index"]] = cls
        mods.append({"index": m["index"], "name": m["name"], "obj": m["obj"], "class": cls, "criteria": crit})
        mc[cls] += 1
        for k in crit:
            crit_c[k] += 1
        crit_c["+".join(crit) or "none"] += 0
        if cls == "xdk":
            crit_c["only:" + "+".join(crit)] += 1
        for p in m["procs"]:
            pc[cls] += 1
            bc[cls] += p["length"]
            va = None
            if 1 <= p["seg"] <= len(pe["sections"]):
                va = base + pe["sections"][p["seg"] - 1]["va"] + p["off"]
                va_mods.setdefault(va, set()).add(m["index"])
            if cls == "xdk":
                xprocs.append({"module": m["index"], "moduleName": m["name"], "name": p["name"], "va": va,
                               "length": p["length"]})
    print("criterion counts (modules): " + ", ".join(f"{k} {crit_c[k]}" for k in ("a", "b", "c", "linker")))
    print("xdk criteria combos: " + ", ".join(f"{k[5:]} {v}" for k, v in sorted(crit_c.items())
                                              if k.startswith("only:")))
    for c in CLASSES:
        print(f"  {c}: modules {mc[c]} procs {pc[c]} bytes {bc[c]}")
    _check("modules", sum(mc.values()), len(pdb["modules"]))
    _check("module procs", sum(pc.values()), sum(len(m["procs"]) for m in pdb["modules"]))
    _check("module bytes", sum(bc.values()), sum(p["length"] for m in pdb["modules"] for p in m["procs"]))
    summ["modules"] = {c: {"modules": mc[c], "procs": pc[c], "bytes": bc[c]} for c in CLASSES}
    summ["criteria"] = {k: crit_c[k] for k in ("a", "b", "c", "linker")}
    summ["xdkCriteriaCombos"] = {k[5:]: v for k, v in sorted(crit_c.items()) if k.startswith("only:")}

    # 2 xdk procs -> proto_map rows by protoVA
    byva = {int(r["protoVA"], 16): r for r in rows}

    def row_classes(r):
        return {modcls[i] for i in va_mods.get(int(r["protoVA"], 16), ())}
    xc, xb = Counter(), Counter()
    for p in xprocs:
        r = byva.get(p["va"]) if p["va"] is not None else None
        cls = r["class"] if r else "no-row"
        p["class"] = cls
        p["retailVA"] = None
        if r:
            if cls == "identical" and r["hits"] == 1:
                p["retailVA"] = r["targetVAs"][0]
            elif cls == "near-identical":
                p["retailVA"] = r["candidateVA"]
            p["hits"] = r["hits"]
            p["excludeReason"] = r["excludeReason"]
        xc[cls] += 1
        xb[cls] += p["length"]
    print(f"xdk procs -> proto_map rows (join on protoVA): {len(xprocs)} procs")
    for c in MAP_CLASSES:
        print(f"  {c}: procs {xc[c]} bytes {xb[c]}")
    _check("xdk map count", sum(xc.values()), pc["xdk"])
    _check("xdk map bytes", sum(xb.values()), bc["xdk"])
    summ["xdkMap"] = {c: {"procs": xc[c], "bytes": xb[c]} for c in MAP_CLASSES}

    # 3 cross-check vs T5 labels
    fsize = {int(f["entry"], 16): f["size"] for f in funcs}
    pdbset = {int(p["retailVA"], 16) for p in xprocs if p["retailVA"]}
    cross = []
    summ["crosscheck"] = {}
    for flag in ("xbsymdb", "xtlid"):
        lab = {}
        for s in syms:
            if s[flag]:
                lab.setdefault(int(s["va"], 16), []).append(f"{s['namespace']}::{s['name']}" if s["namespace"]
                                                            else s["name"])
        cnt, byt = Counter(), Counter()
        for va in sorted(pdbset | set(lab)):
            st = "agree" if va in pdbset and va in lab else "pdb-only" if va in pdbset else f"{flag}-only"
            cnt[st] += 1
            byt[st] += fsize.get(va, 0)
            cross.append({"set": flag, "va": _hx(va), "status": st, "t5Size": fsize.get(va, 0),
                          "labels": sorted(lab.get(va, []))})
        nofn = sum(1 for va in lab if va not in fsize)
        keys = ("agree", "pdb-only", f"{flag}-only")
        print(f"cross-check pdb xdk retail VAs ({len(pdbset)}) vs {flag} label VAs ({len(lab)}): "
              + ", ".join(f"{k} {cnt[k]} ({byt[k]} B)" for k in keys) + f"; labels with no T5 function {nofn}")
        _check(f"crosscheck {flag} count", sum(cnt.values()), len(pdbset | set(lab)))
        _check(f"crosscheck {flag} bytes", sum(byt.values()), sum(fsize.get(v, 0) for v in pdbset | set(lab)))
        summ["crosscheck"][flag] = {"pdbVAs": len(pdbset), "labelVAs": len(lab), "labelsNoFunction": nofn,
                                    **{k: {"count": cnt[k], "bytes": byt[k]} for k in keys}}

    # 4 bucket over exec sections
    secs = [s for s in x["sections"] if s["flags"] & SEC_EXEC]
    pop = {s["name"]: bytearray(s["virtualSize"]) for s in secs}  # 1 = in a T5 function body
    attr = {s["name"]: bytearray(s["virtualSize"]) for s in secs}  # class bits

    def spans(lo, hi):
        for s in secs:
            a, b = max(lo, s["virtualAddr"]), min(hi, s["virtualAddr"] + s["virtualSize"])
            if a < b:
                yield s, a - s["virtualAddr"], b - s["virtualAddr"]
    for f in pop_funcs:
        for s, a, b in spans(int(f["bodyMin"], 16), int(f["bodyMax"], 16) + 1):
            pop[s["name"]][a:b] = b"\1" * (b - a)
    if pop_src == "applied":  # superset of T5: every T5 entry kept or absorbed, no T5 body byte outside the population
        pop_entries = {f["entry"] for f in pop_funcs}
        # was: _check("population T5-only entries", sum(f["entry"] not in pop_entries for f in funcs), 0)
        # 3.6 T2: ApplySymbols' absorb (3.2 T2) removes T5 entries whose body lies inside an applied function; that
        # function must be one: a range covered only by two neighbours is not absorbed (the byte check would pass it).
        t5_only = [f for f in funcs if f["entry"] not in pop_entries]
        pop_ranges = [(int(f["bodyMin"], 16), int(f["bodyMax"], 16)) for f in pop_funcs]
        not_absorbed = [f for f in t5_only if not any(lo <= int(f["bodyMin"], 16) and int(f["bodyMax"], 16) <= hi
                                                      for lo, hi in pop_ranges)]
        print(f"population T5-only entries: {len(t5_only)}, not absorbed {len(not_absorbed)}: "
              + " ".join(f["entry"] for f in t5_only))
        _check("population T5-only entries not absorbed", len(not_absorbed), 0)
        pre_only = 0
        for f in funcs:
            for s, a, b in spans(int(f["bodyMin"], 16), int(f["bodyMax"], 16) + 1):
                pre_only += pop[s["name"]][a:b].count(0)
        _check("population T5-only bytes", pre_only, 0)
    mapped = Counter()
    for r in rows:
        cl = row_classes(r)
        if r["class"] == "identical":
            vas = r["targetVAs"][:1]
        elif r["class"] == "near-identical":
            vas = [r["candidateVA"]] if r["candidateVA"] else []
        elif r["class"] == "ambiguous" and len(cl) == 1:
            vas = r["targetVAs"]
        else:
            continue
        bits = 0
        for c in cl:
            bits |= BIT[c]
        if not bits:
            continue
        mapped[r["class"]] += 1
        for v in vas:
            va = int(v, 16)
            for s, a, b in spans(va, va + r["length"]):
                buf = attr[s["name"]]
                for i in range(a, b):
                    buf[i] |= bits
    labelled = {int(s["va"], 16) for s in syms if s["xbsymdb"] or s["xtlid"]}
    for f in pop_funcs:
        if int(f["entry"], 16) in labelled:
            for s, a, b in spans(int(f["bodyMin"], 16), int(f["bodyMax"], 16) + 1):
                buf = attr[s["name"]]
                for i in range(a, b):
                    if not buf[i]:
                        buf[i] = 8
    name_of = {0: "unidentified", 1: "game", 2: "xdk", 4: "linker", 8: "xdk-label"}
    print("bucket rows used: " + ", ".join(f"{k} {mapped[k]}" for k in ("identical", "near-identical",
                                                                       "ambiguous")))
    print(f"bucket population = {'T5 applied' if pop_src == 'applied' else 'T5'} function bodies "
          f"({len(pop_funcs)} functions) in EXECUTABLE sections (bytes)")
    tot, uranges, summ["sections"] = Counter(), [], []
    for s in secs:
        p, a = pop[s["name"]], attr[s["name"]]
        c = Counter()
        start = None
        for i in range(len(p) + 1):
            k = None
            if i < len(p) and p[i]:
                k = name_of.get(a[i], "mixed")
                c[k] += 1
            if k == "unidentified" and start is None:
                start = i
            elif k != "unidentified" and start is not None:
                uranges.append({"section": s["name"], "start": _hx(s["virtualAddr"] + start),
                                "end": _hx(s["virtualAddr"] + i), "bytes": i - start})
                start = None
        fb = sum(p)
        rem = s["virtualSize"] - fb
        sig, kind = name_signal(s["name"], libnames)
        att = {k: c[k] for k in BUCKET}
        dom_pool = {"game": c["game"], "xdk": c["xdk"] + c["xdk-label"], "linker": c["linker"], "mixed": c["mixed"]}
        dom = max(("game", "xdk", "linker", "mixed"), key=lambda k: (dom_pool[k], -("game", "xdk", "linker",
                                                                                      "mixed").index(k)))
        dom = dom if dom_pool[dom] else "none"
        print(f"  {s['name']} flags 0x{s['flags']:X} va {_hx(s['virtualAddr'])} vsize {s['virtualSize']} "
              f"function {fb} " + " ".join(f"{k} {c[k]}" for k in BUCKET) + f" non-function {rem}")
        _check(f"bucket {s['name']}", sum(c.values()), fb)
        for k in BUCKET:
            tot[k] += c[k]
        tot["function"] += fb
        tot["nonFunction"] += rem
        tot["vsize"] += s["virtualSize"]
        summ["sections"].append({"name": s["name"], "flags": s["flags"], "va": _hx(s["virtualAddr"]),
                                 "virtualSize": s["virtualSize"], "functionBytes": fb, **att,
                                 "nonFunction": rem, "nameSignal": sig, "nameSignalKind": kind,
                                 "dominant": dom})
    print("  total function " + str(tot["function"]) + " " + " ".join(f"{k} {tot[k]}" for k in BUCKET)
          + f" non-function {tot['nonFunction']}")
    _check("bucket total", sum(tot[k] for k in BUCKET), tot["function"])
    _check("bucket ranges", sum(r["bytes"] for r in uranges), tot["unidentified"])
    print(f"unidentified: {tot['unidentified']} B in {len(uranges)} ranges of {tot['function']} B function bodies "
          f"({tot['vsize']} B exec sections)")
    summ["bucket"] = {"functionBytes": tot["function"], "execSectionBytes": tot["vsize"],
                      "nonFunction": tot["nonFunction"], "unidentifiedRanges": len(uranges),
                      **{k: tot[k] for k in BUCKET}}
    frecs = []  # one per population function: class of its entry byte
    for f in pop_funcs:
        va = int(f["entry"], 16)
        sec, cls = None, "outside"
        for s in secs:
            if s["virtualAddr"] <= va < s["virtualAddr"] + s["virtualSize"]:
                i = va - s["virtualAddr"]
                sec = s["name"]
                cls = name_of.get(attr[sec][i], "mixed") if pop[sec][i] else "outside"
                break
        frecs.append({"entry": f["entry"], "size": f["size"], "section": sec, "class": cls})
    by_class = Counter(r["class"] for r in frecs)
    _check("functions records", len(frecs), len(pop_funcs))
    _check("functions byClass", sum(by_class.values()), len(pop_funcs))
    summ["functions"] = {"source": pop_src, "count": len(frecs), "byClass": dict(sorted(by_class.items()))}
    print(f"functions ({pop_src}): {len(frecs)} " + " ".join(f"{k} {by_class[k]}" for k in BUCKET + ("outside",)))

    # 5 section-name signal
    print("section-name signal:")
    for s in summ["sections"]:
        print(f"  {s['name']}: lib {s['nameSignal'] or '-'} ({s['nameSignalKind'] or 'none'}) dominant {s['dominant']}")

    # 6 outputs
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)

    def wl(name, recs):
        with open(out / name, "w", encoding="utf-8", newline="\n") as f:
            for r in recs:
                f.write(json.dumps(r, sort_keys=True) + "\n")
    wl("modules.jsonl", mods)
    wl("xdk_procs.jsonl", [{**p, "va": _hx(p["va"]) if p["va"] is not None else None}
                           for p in sorted(xprocs, key=lambda p: (p["module"], p["va"] or 0, p["name"]))])
    wl("crosscheck.jsonl", cross)
    wl("unidentified.jsonl", uranges)
    wl("functions.jsonl", frecs)
    summ["sumChecksFailed"] = FAILS
    (out / "summary.json").write_text(json.dumps(summ, indent=1, sort_keys=True) + "\n", encoding="utf-8",
                                      newline="\n")
    print(f"out: {out.relative_to(REPO)}")
    print(f"sum checks: {'ok' if not FAILS else 'FAILED ' + ', '.join(FAILS)}")
    return 1 if FAILS else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
