"""Bucket the unmatched candidate pool by verdict and by what each bucket needs (D29; T14, phase 2.5).

Usage: PY tools/pool_buckets.py [--out P]   (default docs/matching/pool_buckets.md)
Inputs: derived/candidates/candidates.jsonl (PY tools/candidates.py --write), docs/matching/attempts.jsonl (ledger),
config/matched_set.toml, config/retail/units.toml (opt), the delink cut derived/delink/retail-<pin8>/ (C0093 extent).
Each unmatched pool row lands in the first bucket whose test it meets (BUCKETS order). Exit 0 written; 1 the bucket
counts do not sum to pool minus matched; 2 refused (missing input, unit absent from units.toml or the cut).
3.4 T13 sections, each with a sum line checked against its proto_map class count (functions.jsonl):
- `## Near lane`: candidates rows with lane near, same bucket logic and BUCKETS, per-lane ranks.
- `## Near-identical outside the lane`: near-identical rows not in the lane, by the reason candidates.py excludes them
  (recomputed with its helpers; cross-checked against the lane row count). lane + outside == class count.
- `## Ambiguous`: class ambiguous by k = targetVAs that are Ghidra entries (applied/functions.jsonl): short-inline-copy,
  multiple-targets, other.
- `## Absent`: docs/evidence/qa_absent.md rows by QA result (QA evidence only, I5: no names derived).
Inputs added: derived/proto_map/retail-xbe/functions.jsonl, derived/dup_report/retail_folds.jsonl,
config/retail/symbols.txt + symbols_pin.txt, derived/ghidra/retail-<pin8>/applied/functions.jsonl, qa_absent.md.
A NOTES/NOTES_VA key no unmatched row uses prints a WARN line (no failure): its row was matched, drop the key.
"""
import argparse
import bisect
import hashlib
import json
import re
import sys
import tomllib
from collections import Counter, defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))
import byte_compare  # noqa: E402
import candidates  # noqa: E402  near-lane exclusion helpers (3.4 T13)
import coff_info  # noqa: E402
import proto_map  # noqa: E402

CAND = REPO / "derived" / "candidates" / "candidates.jsonl"
LEDGER = REPO / "docs" / "matching" / "attempts.jsonl"
MATCHED = REPO / "config" / "matched_set.toml"
UNITS = REPO / "config" / "retail" / "units.toml"
QA_ABSENT = REPO / "docs" / "evidence" / "qa_absent.md"
OUT = REPO / "docs" / "matching" / "pool_buckets.md"
STOP = 20  # START_HERE.md: stop at about 20 attempts without progress

# Verdicts recorded only in task summaries (the ledger has no verdict field): symbol -> (bucket, source).
NOTES = {
    "?GetTSC@BeepState@@QAE_KXZ": ("wall-d45", "2.5 T4, C0084"),
    "??$RELEASE@UDirect3D@@@@YAXAAPAUDirect3D@@@Z": ("held-d78", "2.5 T12"),
    # 3.3 verdicts (R3.3-005)
    "?Render@Sprite@@QBEXXZ": ("wall-thinker", "3.3 T4, T21 R3.3-004: best 98.597565; R1 cvttss2si operand order, "
                               "R2 y-add order; untried lead: name _loc.u/_loc.v (FAKE)"),  # was past-stop (3.3 T4)
}
# Same, keyed by VA where one symbol names several folded rows (T14 row = line of logs/T14.rows.txt).
NOTES_VA = {
    0x000CA770: ("relink-refused", "3.3 T14 row 55, same construct, not attempted; 3.4 T10 C0154"),
    0x000E88E1: ("relink-refused", "3.3 T14 row 57, same construct, not attempted; 3.4 T10 C0154"),
    **{va: ("pin-only-alias", "3.3 T14 C0132") for va in (
        0x000292A6, 0x0006A5D2, 0x0001819A, 0x000BE119, 0x000C1028, 0x000C0299)},
    **{va: ("pin-only-alias", "3.3 T10 C0132") for va in (0x000775B9, 0x000775E2)},
    # 3.4 verdicts (R3.4-005)
    0x0010027B: ("refused-by-tool", "3.4 T12: ??1Diary@@QAE@XZ pin-only, matched_set refuses"),
    0x00125F98: ("refused-by-tool", "3.4 T6.1/T12: ?Set@GameOptions@@QBEXXZ sole name a disasm-verified pin"),
    **{va: ("held-d78-inline", "3.4 T12 ruling; Gen 4 question 40a575bd") for va in (
        0x00063D47, 0x000CB0D1, 0x000CAFF0, 0x000277AF)},
    **{va: ("wall-dir32", "3.4 T12 C0155") for va in (0x00033D78, 0x00033C74)},
    **{va: ("wall-d45", "3.4 AABox __asm") for va in (0x000DDE5B, 0x000DDEA2)},
    0x000AD6E6: ("wall-data-name", "3.4 T12: SwampGamePad ctor"),
    **{va: ("wall-vtable-relink", "3.3 T4 C0123; vtable pins C0129") for va in (
        0x000DA7A4, 0x0014F54F, 0x0014F56C, 0x0014F5BE)},
    **{va: ("xdk-callee-unnamed", "3.4") for va in (0x000C5C15, 0x000BD0E9)},
    0x000BD3A8: ("folded-template-name", "3.4 T12: AddOverride"),
}
# Symbols past the stop whose pa-thinker consultation (M17) is done: symbol -> source. None in 3.3.
THINKER = {}

# (key, verdict, needs); the order is the precedence.
BUCKETS = [
    ("wall-d45", "byte-matched only as `__asm` (rdtsc); D45 refuses an asm body",
     "nothing under the current rules: cl 13.10 has no `__rdtsc` intrinsic (C0084); permanent wall"),
    ("wall-dir32", "base-displacement DIR32 reloc the delink cut does not express; objdiff stays < 100 (C0155)",
     "a delink rule for base-disp DIR32 fields (C0155); source cannot fix it"),
    ("refused-by-tool", "the row's lane name is a pin only; matched_set / match_finish refuses it",
     "a fold-alias name (symbols.txt func line) or a pin ruling (T6.1/T12)"),
    ("held-d78-inline", "match needs an inline helper absent from the PDB; held by the T12 ruling (D78)",
     "a PDB-recorded inline helper (Gen 4 question, commit 40a575bd)"),
    ("wall-data-name", "a data target's only name is undecorated (PDB data records, C0129)",
     "a decorated data name for the target (T12)"),
    ("wall-vtable-relink", "ctor stores a vptr; the vtable target is unnamed, relink fails (C0123)",
     "vtable pins (C0129, 3.3 T4)"),
    ("xdk-callee-unnamed", "a callee in an XDK library unit has no symbol",
     "the XDK callee named (symbols pin)"),
    ("folded-template-name", "the callee is a folded template instance with no single name (T12)",
     "a fold-VA naming rule for the template instance (C0154)"),
    ("wall-thinker", f"attempted >= {STOP} times; pa-thinker consulted (M17), its round run, residuals unmoved: wall",
     "a new lever outside the consultation (the row's note names residuals and untried leads)"),
    ("held-d78", "objdiff 100 and bytes equal, held under NON_MATCHING by D78",
     "a source for the inlined callee's body (e.g. `Direct3D::Release`); the constant has no PDB/disasm/string origin"),
    ("inline-only", "header inline with no standalone emission from its own unit",
     "its unmatched caller matched first (MoviePlayer::IsActive: a caller in Intro.obj)"),
    ("cut-wall-attempted", "attempted; bytes equal but objdiff < 100; cut symbol runs past the row size (C0085)",
     "a function symbol at the unsymboled tail target in the delink cut; source cannot fix it"),
    ("cut-body-merged", "a pre-existing Ghidra body spans past the row into its neighbour's entry (ApplySymbols "
     "`inside`); a symbols pin cannot split it",
     "the merged body clipped at the neighbour's entry in ApplySymbols (rename path never clips), then a fresh cut"),
    ("relink-refused", "objdiff 100 and bytes equal, but the gate's relink refuses the unit (PE entry != pin entry: "
     "out-of-line COMDAT copy of a folded member, C0136); rows sharing the construct included",
     "a relink rule for out-of-line copies of folded COMDATs (tools/relink.py, verifier-locked: a plan decision)"),
    ("pin-only-alias", "self-alias folded VA named only by pins; match_one passes, match_finish refuses "
     "(no func line in symbols.txt, C0132)",
     "a symbols.txt func line for the pin alias (multi-public/fold naming, D44 residue), then match_finish"),
    ("past-stop-thinker", f"attempted >= {STOP} times without a match; pa-thinker consulted (M17)",
     "the consultation's lever tried, else filed as a wall"),
    ("past-stop", f"attempted >= {STOP} times (ledger or recorded scratch runs) without a match; thinker not yet consulted",
     "one pa-thinker consultation (M17) before any further attempt"),
    ("attempted-open", f"attempted < {STOP} times, no match, no recorded wall",
     "more attempts, from the ledger's best run (CK18)"),
    ("cut-nosym", "the cut has no function symbol with the row's name",
     "a function symbol for the row in the delink cut (delink/Ghidra naming)"),
    ("cut-extent", "never attempted; cut symbol extent > row size (C0093 tell)",
     "a function symbol at the unsymboled target past the row in the cut, else objdiff stays < 100"),
    ("bodymax", "Ghidra body ends before the row size (candidates `bodymax`)",
     "the Ghidra function boundary fixed, then a fresh cut"),
    ("stl-reloc", "STL instantiation (`std::` name) with unresolved reloc targets",
     "both: the targets named (symbols pin or matched callee) and the STL route (explicit instantiation, C0096; T10.1)"),
    ("stdsig-reloc", "game function with a std type in its signature (`@std@@`), unresolved reloc targets",
     "both: the targets named and opaque std decls from VC 7.1 headers (C0096; T12 BeepFile); library filter is policy"),
    ("reloc-provisional", "reloc fails on provisional pins only (status outside STATUSES, e.g. site-corr, boundary)",
     "the pins verified (status promoted, e.g. disasm-verified) or the targets matched with a symbol"),
    ("reloc-code", "reloc gaps in the cut: >= 1 unresolved target, every unresolved target a call/jump (rel)",
     "the callees named: pinned in symbols or matched with a symbol (C0084, C0059)"),
    ("reloc-data", "reloc gaps in the cut: at least one unresolved absolute (abs) target",
     "the data or pointer targets named as symbols (globals, vtables, literals)"),
    ("stl", "STL instantiation (`std::` name, candidates `library`), relocs clean",
     "the STL route: explicit instantiation from VC 7.1 headers (C0096; T10.1 `_Uninit_fill_n` matched this way)"),
    ("stdsig", "game function with a std type in its signature (`@std@@`), relocs clean",
     "opaque std decls from VC 7.1 headers (C0096; T12 BeepFile); the `library` filter is policy, not a wall"),
    ("large", "passing, never attempted, larger than any matched function",
     "an untested size: expect more attempts per row (2.5 matched nothing this size)"),
    ("ready-opt-unknown", "passing, never attempted; unit `opt` is \"unknown\"",
     "the unit's opt decided by a one-flag control on its first claim (C0065), then a batch claim"),
    ("ready", "passing, never attempted; unit `opt` set",
     "a batch claim only (next batch input)"),
]


def refuse(msg):
    print(f"pool_buckets: REFUSED {msg}")
    sys.exit(2)


def sha8(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()[:12]


def as_int(v):
    return v if isinstance(v, int) else int(str(v), 16)


def assign(rows, matched, opt, pin, cut, led, maxsize, objs, used):
    """Bucket the unmatched rows of one lane (BUCKETS order); used collects the NOTES/NOTES_VA keys hit (3.4 T13)."""
    out, extra = defaultdict(list), Counter()
    targets = Counter()
    unmatched = [r for r in rows if (r["symbol"], r["va"]) not in matched]
    for r in unmatched:
        if r["unit"] not in opt:
            refuse(f"unit {r['unit']} not in units.toml")
        f = byte_compare.cut_dir(pin) / r["unit"].replace("\\", "__").replace("/", "__")
        if f.name not in objs:
            if not f.is_file():
                refuse(f"unit {r['unit']} absent from the cut {cut.relative_to(REPO)}")
            objs[f.name] = byte_compare.load_obj(f)[1]
        c = objs[f.name]
        fs = [y for y in c["symbols"].values() if byte_compare.is_func(y) and y["name"] == r["symbol"]]
        ext = None
        if fs:
            _s, lo, hi = byte_compare.extent(c, fs[0])
            ext = hi - lo
        runs = list({id(x): x for x in led[r["va"]] + led[r["symbol"]]}.values())
        tries = sum(1 for x in runs if x["kind"] == "attempt")
        ck = r["checks"]
        unres = [s for s in r["sites"] if s["resolved"] is None]
        note = NOTES.get(r["symbol"]) or NOTES_VA.get(r["va"])
        used.add(r["symbol"] if r["symbol"] in NOTES else r["va"] if note else None)
        if note:
            b = note[0]
            if b == "past-stop" and r["symbol"] in THINKER:
                b = "past-stop-thinker"
        elif runs and ext is not None and ext > r["size"]:
            b = "cut-wall-attempted"
        elif runs:
            b = ("past-stop-thinker" if r["symbol"] in THINKER else "past-stop") if tries >= STOP \
                else "attempted-open"
        elif ext is None:
            b = "cut-nosym"
        elif ext > r["size"]:
            b = "cut-extent"
        elif not ck["bodymax"]["pass"]:
            b = "bodymax"
        elif not ck["library"]["pass"] and not ck["reloc"]["pass"]:
            b = ("stl" if r["names"][0].startswith("std::") else "stdsig") + "-reloc"
        elif not ck["reloc"]["pass"]:
            b = ("reloc-data" if any(s["kind"] != "rel" for s in unres) else "reloc-code") if unres                 else "reloc-provisional"
            ut = {s["target"] for s in unres}
            if len(ut) == 1:
                extra[(b, "one")] += 1
                targets[next(iter(ut))] += 1
        elif not ck["library"]["pass"]:
            b = "stl" if r["names"][0].startswith("std::") else "stdsig"
        elif not r["pass"]:
            refuse(f"rank {r['rank']} fails a check no bucket covers: {[k for k, v in ck.items() if not v['pass']]}")
        elif r["size"] > maxsize:
            b = "large"
        elif opt[r["unit"]] == "unknown":
            b = "ready-opt-unknown"
        else:
            b = "ready"
        out[b].append(r)
    return unmatched, out, extra, targets


NEAR_NEEDS = {  # 3.4 T13: near-lane reloc gaps are callees the lane name's prototype calls but retail leaves unnamed
    "reloc-code": "the unnamed callee named: pinned in symbols or matched with a symbol (C0059, C0101)",
    "reloc-provisional": "the unnamed callee's provisional pin verified (status promoted) or the callee matched "
                         "with a symbol",
}
QA_CLASSES = ("identical", "near-identical", "excluded", "ambiguous", "absent")
QA_ROW = re.compile(r"^- (0x[0-9A-F]{8}) (.*?)(?:: (0x[0-9A-F]{8} .*))?$")


def render(L, out, extra, targets, h, needs_over):
    """Bucket subsections, single-target gaps and rows by bucket for one lane; h = heading prefix."""
    L += [f"{h} Buckets", ""]
    for k, verdict, needs in BUCKETS:
        rs = sorted(out.get(k, []), key=lambda r: r["rank"])
        ex = f"`{rs[0]['names'][0]}` (rank {rs[0]['rank']}, {rs[0]['size']} B, {rs[0]['unit'].split(chr(92))[-1]})" \
            if rs else "-"
        L.append(f"{h}# {k}: {len(rs)}")
        L.append(f"- verdict: {verdict}")
        L.append(f"- needs: {needs_over.get(k, needs)}")
        L.append(f"- example: {ex}")
        if rs:
            src = sorted({(NOTES.get(r["symbol"]) or NOTES_VA[r["va"]])[1] for r in rs
                          if r["symbol"] in NOTES or r["va"] in NOTES_VA})
            units = Counter(r["unit"] for r in rs)
            tags = Counter(t for r in rs for t, v in r["tags"].items() if v == "y")
            bands = Counter("<=32" if r["size"] <= 32 else "33-128" if r["size"] <= 128 else
                            "129-512" if r["size"] <= 512 else ">512" for r in rs)
            L.append("- size bytes: " + ", ".join(f"{b} {bands[b]}" for b in ("<=32", "33-128", "129-512", ">512")))
            L.append(f"- units {len(units)}; tags " + (", ".join(f"{t} {n}" for t, n in sorted(tags.items())) or "none")
                     + (f"; source {', '.join(src)}" if src else "")
                     + (f"; one unresolved target {extra[(k, 'one')]}" if (k, "one") in extra else ""))
        L.append("")
    L += [f"{h} Single-target reloc gaps", "",
          "Unresolved targets that alone block a reloc-code/reloc-data row (rows unblocked by naming that one target):", ""]
    for t, n in sorted(targets.items(), key=lambda kv: (-kv[1], kv[0]))[:15]:
        L.append(f"- 0x{t:06X}: {n}")
    L += ["", f"{h} Rows by bucket (ranks, ascending)", ""]
    for k, _v, _n in BUCKETS:
        rs = sorted(r["rank"] for r in out.get(k, []))
        L.append(f"- {k} ({len(rs)}): " + (", ".join(map(str, rs)) or "-"))
    L.append("")


def outside_lane(fx):
    """Near-identical rows outside the lane by candidates.py's exclusion (candidates.py:343-361, its helpers)."""
    try:
        folds = {int(r["retailVA"], 16) for r in candidates.load_jsonl(candidates.need(candidates.FOLDS))}
        units = sorted((u for u in tomllib.loads(candidates.need(UNITS).read_text(encoding="utf-8"))["unit"]
                        if "text_start" in u), key=lambda u: u["text_start"])
        funcs, _data = candidates.load_symbols()
    except candidates.Refuse as e:
        refuse(str(e))
    ustarts = [u["text_start"] for u in units]
    why, lane = {}, 0  # why: protoVA -> (reason, sub-reason)
    for r in fx:
        if r["class"] != "near-identical":
            continue
        if not r.get("candidateVA") or r.get("similarity") is None or r["similarity"] < proto_map.THRESHOLD:
            why[r["protoVA"]] = (f"no candidateVA/sim<{proto_map.THRESHOLD}", None)
            continue
        va = int(r["candidateVA"], 16)
        i = bisect.bisect_right(ustarts, va) - 1
        u = units[i] if i >= 0 and units[i]["text_start"] <= va < units[i]["text_end"] else None
        pv = int(r["protoVA"], 16) - candidates.PROTO_BASE
        f = funcs.get(va)
        if u is None:
            why[r["protoVA"]] = ("cut", "no unit")
        elif u["kind"] != "game":
            why[r["protoVA"]] = ("cut", "unit not game")
        elif not u["delink"]:
            why[r["protoVA"]] = ("cut", "unit not delink")
        elif candidates.TAIL[0] <= pv < candidates.TAIL[1]:
            why[r["protoVA"]] = ("cut", "tail")
        elif va in folds:
            why[r["protoVA"]] = ("fold", None)
        elif f is None:
            why[r["protoVA"]] = ("no name evidence", None)
        elif f["size"] is None:
            why[r["protoVA"]] = ("name not admitted", "size -")
        elif f["status"] not in candidates.NEAR_STATUSES:
            why[r["protoVA"]] = ("name not admitted", f"status {f['status']}")
        else:
            lane += 1
    return why, lane


def qa_bucket(ln):
    """One qa_absent.md row line -> (protoVA, bucket, QA result classes); reads QA evidence only (I5)."""
    m = QA_ROW.match(ln)
    if not m:
        return None, None, []
    res = re.split(r"; (?=0x[0-9A-F]{8} )", m.group(3)) if m.group(3) else []
    cls = [next((t for t in reversed(x.split()) if t in QA_CLASSES), "?") for x in res]
    b = ("any identical hit" if "identical" in cls else "near-identical" if "near-identical" in cls else
         "excluded" if "excluded" in cls else "absent in QA too" if "absent" in cls else "no QA name")
    return m.group(1), b, cls


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--out", default=str(OUT))
    a = ap.parse_args(argv)
    for p in (CAND, LEDGER, MATCHED, UNITS):
        if not p.is_file():
            refuse(f"missing input {p.relative_to(REPO)}")
    pin = coff_info.pinned_sha() or ""
    cut = byte_compare.cut_dir(pin)
    why = byte_compare.cut_check(cut)
    if why:
        refuse(why)
    rows = [json.loads(x) for x in CAND.read_text(encoding="utf-8").splitlines() if x.strip()]
    nrows = [r for r in rows if r.get("lane") == "near"]  # 3.4 T13: near lane section
    rows = [r for r in rows if r.get("lane", "identical") == "identical"]  # 3.4 T2: near lane in next_candidates.md
    ledger = [json.loads(x) for x in LEDGER.read_text(encoding="utf-8").splitlines() if x.strip()]
    ms = tomllib.loads(MATCHED.read_text(encoding="utf-8"))
    matched = {(f["symbol"], as_int(f["va"])) for f in ms.get("function", [])}
    maxsize = max((f["size"] for f in ms.get("function", [])), default=0)
    opt = {u["name"]: u.get("opt", "unknown") for u in tomllib.loads(UNITS.read_text(encoding="utf-8"))["unit"]}
    led = defaultdict(list)  # keyed by VA and by symbol: refused runs log va null
    for x in ledger:
        if x.get("va") not in (None, "None"):
            led[as_int(x["va"])].append(x)
        led[x["symbol"]].append(x)

    pool = len(rows)
    objs, used = {}, set()
    unmatched, out, extra, targets = assign(rows, matched, opt, pin, cut, led, maxsize, objs, used)
    unm_n, out_n, extra_n, targets_n = assign(nrows, matched, opt, pin, cut, led, maxsize, objs, used)
    try:
        fx = candidates.load_jsonl(candidates.need(candidates.FUNCS))
    except candidates.Refuse as e:
        refuse(str(e))
    ncls = Counter(r["class"] for r in fx)
    why, lane = outside_lane(fx)
    gfj = REPO / "derived" / "ghidra" / f"retail-{pin[:8]}" / "applied" / "functions.jsonl"
    if not gfj.is_file() or not QA_ABSENT.is_file():
        refuse(f"missing input {gfj.relative_to(REPO)} or {QA_ABSENT.relative_to(REPO)}")
    entries = {int(f["entry"], 16) for f in candidates.load_jsonl(gfj)}
    qa = [x for x in QA_ABSENT.read_text(encoding="utf-8").splitlines() if x.startswith("- 0x")]

    fail = []
    total = sum(len(v) for v in out.values())
    npm = pool - len(unmatched)  # matched rows inside the pool; lane (near) matches sit outside it (3.4 T4)
    sums = [f"identical pool {pool} = buckets {total} + matched {npm}"]
    if total != pool - npm or total != len(unmatched):
        fail.append(sums[-1])
    tn = sum(len(v) for v in out_n.values())
    nm = len(nrows) - len(unm_n)
    sums.append(f"near-identical {ncls['near-identical']} = near lane {tn} + matched-near {nm} + outside {len(why)}")
    if tn + nm + len(why) != ncls["near-identical"] or tn != len(unm_n) or lane != len(nrows):
        fail.append(sums[-1] + f" (recomputed lane {lane}, candidates near rows {len(nrows)})")

    amb, kc, hc = defaultdict(list), defaultdict(Counter), defaultdict(Counter)
    for r in fx:
        if r["class"] != "ambiguous":
            continue
        k = sum(1 for v in r["targetVAs"] if int(v, 16) in entries)
        b = "short-inline-copy" if r["length"] < 64 or k == 1 else "multiple-targets" if k >= 2 else "other"
        amb[b].append(r)
        kc[b]["0" if k == 0 else "1" if k == 1 else "2+"] += 1
        hc[b]["32 (cap)" if r["hits"] >= 32 else "2-3" if r["hits"] <= 3 else "4-31"] += 1
    ta = sum(len(v) for v in amb.values())
    AMB = (("short-inline-copy", "length < 64 or exactly one target is a Ghidra entry (an inline copy inside "
            "callers, or one standalone body)", "resolve by the entry hit / site evidence (C0010)"),
           ("multiple-targets", "k >= 2 targets are Ghidra entries (several standalone bodies, same bytes)",
            "resolve per target by caller/site correspondence (C0059, C0101)"),
           ("other", "length >= 64 and no target is a Ghidra entry", "a Ghidra function at a target first, then as above"))
    sums.append(f"ambiguous {ncls['ambiguous']} = " + " + ".join(f"{k} {len(amb[k])}" for k, _v, _n in AMB))
    if ta != ncls["ambiguous"]:
        fail.append(sums[-1])

    ab, abx = defaultdict(list), Counter()
    for ln in qa:
        pva, b, cls = qa_bucket(ln)
        ab[b].append(pva)
        abx[(b, len(cls) > 1)] += 1
    ABS = ("any identical hit", "near-identical", "excluded", "absent in QA too", "no QA name")
    tq = sum(len(v) for v in ab.values())
    sums.append(f"absent {ncls['absent']} = " + " + ".join(f"{k} {len(ab[k])}" for k in ABS)
                + f" (qa_absent.md rows {len(qa)})")
    if tq != ncls["absent"] or len(qa) != ncls["absent"] or None in ab:
        fail.append(sums[-1])

    for key in sorted(set(NOTES) | set(NOTES_VA), key=str):
        if key not in used:
            print(f"pool_buckets: WARN note key {key if isinstance(key, str) else f'0x{key:08X}'} "
                  "is not an unmatched row of any lane")
    for s in sums:
        print(f"{s}: {'FAIL' if any(f.startswith(s) for f in fail) else 'OK'}")
    if fail:
        for f in fail:
            print(f"pool_buckets: FAIL {f}")
        return 1

    ins = [("derived/candidates/candidates.jsonl", CAND, f"{pool} identical + {len(nrows)} near rows, "
            "from `PY tools/candidates.py --write`"),
           ("docs/matching/attempts.jsonl", LEDGER, f"{len(ledger)} ledger lines"),
           ("config/matched_set.toml", MATCHED, f"{len(matched)} matched, {npm} identical + {nm} near in the lanes"),
           ("config/retail/units.toml", UNITS, "opt, kind, delink"),
           ("derived/proto_map/retail-xbe/functions.jsonl", candidates.FUNCS, f"{len(fx)} rows, class counts"),
           ("derived/dup_report/retail_folds.jsonl", candidates.FOLDS, "fold VAs"),
           ("config/retail/symbols.txt", candidates.SYMS, "lane names"),
           ("config/retail/symbols_pin.txt", candidates.SYMS_PIN, "lane names"),
           (gfj.relative_to(REPO).as_posix(), gfj, f"{len(entries)} Ghidra entries"),
           ("docs/evidence/qa_absent.md", QA_ABSENT, f"{len(qa)} row lines")]
    L = ["# Pool buckets: every unmatched candidate row by verdict and need (D29)", "",
         "Generated by `PY tools/pool_buckets.py` (2.5 T14; 3.3 verdicts 3.3 T16; near lane, outside, ambiguous, "
         "absent 3.4 T13); do not hand-edit, re-run it. Inputs (sha256[:12]):", ""]
    L += [f"- `{n}` {sha8(p)} ({d})" for n, p, d in ins]
    L += [f"- delink cut `{cut.relative_to(REPO).as_posix()}` (function symbol extents, `byte_compare.extent`); wall "
          "verdicts kept only in task summaries are the `NOTES` / `NOTES_VA` tables in the tool.", "",
          "Sums (each checked; a mismatch exits 1):", ""] + [f"- {s}" for s in sums]
    L += ["", "## Identical lane", "",
          f"Pool {pool}, matched {npm}, unmatched {total} = sum of the buckets below. A row sits in the first "
          "bucket (in this order) whose test it meets. `attempted` = has ledger lines; attempts count `kind` attempt. "
          f"Largest matched function: {maxsize} bytes.", ""]
    render(L, out, extra, targets, "###", {})
    L += ["## Near lane", "",
          f"candidates.jsonl lane near: {len(nrows)} rows, matched {nm}, unmatched {tn} in the buckets below "
          "(same tests and order as the identical lane; ranks are per lane; reloc gaps are unnamed callees).", ""]
    render(L, out_n, extra_n, targets_n, "###", NEAR_NEEDS)
    L += ["## Near-identical outside the lane", "",
          f"functions.jsonl class near-identical rows not in the lane: {len(why)}, by the first reason "
          "`candidates.py` excludes them (recomputed with its helpers; recomputed lane "
          f"{lane} == candidates near rows {len(nrows)}).", ""]
    OUTS = (("cut", "candidateVA outside a delinked game unit (sub-counts by NEAR_EXCL reason)",
             "the unit delinked / classed game (units.toml), else outside the matching scope"),
            ("fold", "candidateVA is a fold VA (retail_folds.jsonl)", "a fold-VA naming rule (C0154)"),
            ("no name evidence", "no func line at candidateVA in symbols.txt / symbols_pin.txt",
             "a near-corr or disasm-verified func line (C0059, C0101)"),
            ("name not admitted", "func line has size `-` or a status outside NEAR_STATUSES (sub-counts by status)",
             "the line's size set and its status promoted to near-corr or disasm-verified"),
            (f"no candidateVA/sim<{proto_map.THRESHOLD}", "no candidateVA or similarity below the threshold",
             "nothing in this lane"))
    byva = {r["protoVA"]: r for r in fx}
    for k, verdict, needs in OUTS:
        ps = sorted(p for p, w in why.items() if w[0] == k)
        sub = Counter(w[1] for w in why.values() if w[0] == k and w[1])
        ex = f"`{byva[ps[0]]['names'][0]}` (protoVA {ps[0]})" if ps else "-"
        L += [f"### {k}: {len(ps)}", f"- verdict: {verdict}", f"- needs: {needs}", f"- example: {ex}"]
        if sub:
            L.append("- sub-counts: " + ", ".join(f"{s} {n}" for s, n in sorted(sub.items())))
        L.append("")
    L += ["## Ambiguous", "",
          f"functions.jsonl class ambiguous: {ta}. k = targetVAs that are Ghidra entries; hits are capped at 32.", ""]
    for k, verdict, needs in AMB:
        rs = sorted(amb[k], key=lambda r: r["protoVA"])
        ex = f"`{rs[0]['names'][0]}` (protoVA {rs[0]['protoVA']}, {rs[0]['length']} B)" if rs else "-"
        L += [f"### {k}: {len(rs)}", f"- verdict: {verdict}", f"- needs: {needs}", f"- example: {ex}",
              "- by k: " + ", ".join(f"{s} {kc[k][s]}" for s in ("0", "1", "2+")),
              "- by hits: " + ", ".join(f"{s} {hc[k][s]}" for s in ("2-3", "4-31", "32 (cap)")), ""]
    L += ["## Absent", "",
          f"`docs/evidence/qa_absent.md` row lines: {len(qa)}, by QA result (first match in this order). QA evidence "
          "only (I5): no names derived from it.", ""]
    ABSN = {"any identical hit": "a QA name hit an identical row: the default name is likely an overload (T11)",
            "near-identical": "a QA name hit a near-identical row", "excluded": "a QA name hit an excluded row",
            "absent in QA too": "absent in the QA build as well", "no QA name": "no QA result on the row"}
    for k in ABS:
        ps = sorted(ab[k])
        L += [f"### {k}: {len(ps)}", f"- verdict: {ABSN[k]}", f"- rows with > 1 QA result: {abx[(k, True)]}",
              "- protoVAs: " + (", ".join(ps) if 0 < len(ps) <= 8 else f"{ps[0]} .. {ps[-1]}" if ps else "-"), ""]
    out_path = Path(a.out).resolve()  # relative to the working directory, inside the repo or not (4.1 T10)
    out_path.write_text("\n".join(L).rstrip("\n") + "\n", encoding="utf-8", newline="\n")
    for k, _v, _n in BUCKETS:
        print(f"{k} {len(out.get(k, []))} near {len(out_n.get(k, []))}")
    print(f"pool_buckets: wrote {out_path.relative_to(REPO).as_posix() if out_path.is_relative_to(REPO) else out_path}: pool {pool}, matched {npm}, "
          f"unmatched {total} in {sum(1 for v in out.values() if v)} buckets; near {tn} in "
          f"{sum(1 for v in out_n.values() if v)} buckets")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
