"""Progress figures (T7, phase 1.5): matched functions and bytes against the counted game population.

CLI: progress.py [--last P] [--self-test]
Inputs: derived/gate/last.json (`PY tools/gate.py`; or --last P), derived/xdk/retail-<sha8>/{summary.json,
  functions.jsonl} (`PY tools/xdk_report.py`), derived/delink/retail-<sha8>/report.json (objdiff report).
Numerator: last.json rows with matched_set.is_done(row) (D28, the only done test): a = count, c = sum of row size.
  Each done row must have an objdiff ReportItem (unit name with `/`, name == symbol) of the same size, and its va
  must be the entry of a counted functions.jsonl record or a listed gap function's va (sizes equal); else exit 1.
Rule (binding): counted(class, dominant) = class == "game" or (class in {mixed, unidentified} and the section's
  summary.sections[].dominant == "game"). B = counted functions.jsonl records + listed gap functions; D = sum over
  sections of game bytes + (mixed + unidentified where dominant == game) + sum of listed gap sizes (moved out of
  that section's nonFunction in the reconcile).
Gap list (T18 5.2): config/gap_functions.toml [[gap]] va, size, section, symbol, evidence, at, note: retail
  functions with no functions.jsonl record, in B and D whether matched or not. Refused (2, `gap list: <va>: <why>`):
  evidence not in {pin, call-site, vtable-slot}; pin without a func line at va in config/retail/symbols_pin.txt or
  symbols.txt of status disasm-verified|corr-verified, or a sized pin != size; call-site/vtable-slot without `at`;
  [va, va+size) not inside one section named `section` with dominant game; overlap with a functions.jsonl record
  or another entry.
Cut: default and --last runs refuse (2) first unless delink.cut_check passes (valid cut.complete, T1 2.1).
Reconcile: per section and total, counted + excluded (xdk + xdk-label + linker + mixed/unidentified in non-game
  sections) + nonFunction == virtualSize (total == execSectionBytes == sum virtualSize); sum byClass == functions
  count == records. Any mismatch -> exit 1, docs/progress.md not written.
Output: `functions: a of B (p%)   bytes: c of D (q%)`, sources, remainder, claim type, last.json date. Counts only
  (D46). A default run that exits 0 writes docs/progress.md (generated, dated, with its command, D59); --last P and
  --self-test never write it; it is rewritten only when its body without the `Generated` stamp line differs (T6 2.1),
  else `unchanged` is printed. --self-test builds last.json in a scratch dir under .run/<phase>/ from
  tests/gate/matched_set.toml (5 rows all pass + 1 row with a failed verdict) and runs two refusal controls;
  asserts the real gap list loads clean; gap controls no-evidence, non-game-section, record-overlap (scratch
  lists) each pass unvalidated (rc 0) and refuse validated (rc 2); control stale-xdk (scratch copy of the xdk report
  with a changed ingestSha256) passes unchecked (rc 0) and refuses checked (rc 2).
Stale report (5.4 T1): load() refuses (2) a summary.json without ingestSha256 or whose value !=
  xdk_report.ingest_sha256(pin) of the current derived/ghidra/retail-<pin8>/ ingest (`run PY tools/xdk_report.py`).
Current last.json (5.5 T6): default and --last runs refuse (2) a last.json with a failed row, or whose `tree`
  list_sha256 / fp_sha256 differ from config/matched_set.toml / config/verifier.sha256 now (last_current()).
Exit: 0 ok; 1 a check failed; 2 refusal (stderr `progress: REFUSED <why>`).
"""
import argparse
import datetime
import hashlib
import json
import re
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import byte_compare  # noqa: E402
import coff_info  # noqa: E402
import delink  # noqa: E402
import matched_set  # noqa: E402
import xdk_report  # noqa: E402

REPO = Path(__file__).resolve().parent.parent
LAST = REPO / "derived" / "gate" / "last.json"
DOC = REPO / "docs" / "progress.md"
FIXTURE = REPO / "tests" / "gate" / "matched_set.toml"
CMD = "PY tools/progress.py"
STAMP = "Generated "
SPLIT = ("mixed", "unidentified")
ALWAYS_EXCLUDED = ("xdk", "xdk-label", "linker")
GAPS = REPO / "config" / "gap_functions.toml"
GAP_EVIDENCE = {"pin", "call-site", "vtable-slot"}
PIN_FILES = (REPO / "config" / "retail" / "symbols_pin.txt", REPO / "config" / "retail" / "symbols.txt")
PIN_VERIFIED = {"disasm-verified", "corr-verified"}


class Refuse(Exception):
    pass


def _rel(p):
    return p.relative_to(REPO).as_posix()


def _counted(cls, dominant):
    return cls == "game" or (cls in SPLIT and dominant == "game")


def _pins():
    """va -> [(size or None, status)] for `func` lines of symbols_pin.txt and symbols.txt."""
    out = {}
    for p in PIN_FILES:
        for ln in p.read_text(encoding="utf-8").splitlines():
            f = ln.split()
            if len(f) < 4 or ln.startswith("#") or f[1] != "func":
                continue
            out.setdefault(int(f[0], 16), []).append((None if f[2] == "-" else int(f[2], 16), f[-1]))
    return out


def load_gaps(gap_path, summ, recs, validate=True):
    """-> validated list of gap entries; raises Refuse `gap list: <va>: <why>`."""
    import tomllib
    gaps = tomllib.loads(gap_path.read_text(encoding="utf-8")).get("gap", [])
    if not validate:
        return gaps
    pins = _pins()
    spans = sorted((int(r["entry"], 16), int(r["entry"], 16) + r["size"]) for r in recs)
    seen = []
    for g in gaps:
        va, size = g.get("va"), g.get("size")
        tag = f"0x{va:08X}" if isinstance(va, int) else repr(va)

        def bad(why):
            raise Refuse(f"gap list: {tag}: {why}")
        if not isinstance(va, int) or not isinstance(size, int) or size <= 0:
            bad("va and size must be integers, size > 0")
        ev = g.get("evidence")
        if ev not in GAP_EVIDENCE:
            bad(f"evidence {ev!r} not in {sorted(GAP_EVIDENCE)}")
        if ev == "pin":
            ok = [p for p in pins.get(va, []) if p[1] in PIN_VERIFIED]
            if not ok:
                bad(f"no func pin at va with status in {sorted(PIN_VERIFIED)}")
            wrong = [p[0] for p in ok if p[0] is not None and p[0] != size]
            if wrong:
                bad(f"pin size 0x{wrong[0]:X} != entry size 0x{size:X}")
        elif not isinstance(g.get("at"), int):
            bad(f"evidence {ev} needs an `at` VA")
        end = va + size
        sec = [s for s in summ["sections"]
               if int(s["va"], 16) <= va and end <= int(s["va"], 16) + s["virtualSize"]]
        if not sec or sec[0]["name"] != g.get("section") or sec[0]["dominant"] != "game":
            where = f"{sec[0]['name']} (dominant {sec[0]['dominant']})" if sec else "no single section"
            bad(f"[va, va+size) in {where}; need section {g.get('section')!r} with dominant game")
        for s0, e0 in spans:
            if s0 < end and va < e0:
                bad(f"overlaps functions.jsonl record 0x{s0:08X} size {e0 - s0}")
        for s0, e0 in seen:
            if s0 < end and va < e0:
                bad(f"overlaps or duplicates list entry 0x{s0:08X}")
        seen.append((va, end))
    return gaps


def load(last_path, gap_path=None, validate=True, xdk_dir=None, check_ingest=True):
    """-> inputs dict; raises Refuse. validate=False skips the gap-list checks, xdk_dir replaces the xdk report dir,
    check_ingest=False skips the ingestSha256 check (self-test controls only)."""
    pin = coff_info.pinned_sha()
    if not pin:
        raise Refuse("no retail_xbe pin in config/check.retail_xbe.sha")
    if not last_path.is_file():
        raise Refuse(f"{last_path} absent (run PY tools/gate.py)")
    last = json.loads(last_path.read_text(encoding="utf-8"))
    if last.get("xbe_sha1") != pin:
        raise Refuse(f"{last_path} xbe_sha1 {last.get('xbe_sha1')} != pin {pin}")
    xdk = xdk_dir or REPO / "derived" / "xdk" / f"retail-{pin[:8]}"
    sp, fp = xdk / "summary.json", xdk / "functions.jsonl"
    if not sp.is_file() or not fp.is_file():
        raise Refuse(f"{_rel(sp)} or {_rel(fp)} missing (run PY tools/xdk_report.py)")
    summ = json.loads(sp.read_text(encoding="utf-8"))
    if summ.get("targetSha1") != pin:
        raise Refuse(f"{_rel(sp)} targetSha1 != pin (run PY tools/xdk_report.py)")
    if summ.get("sumChecksFailed"):
        raise Refuse(f"{_rel(sp)} sumChecksFailed {summ['sumChecksFailed']} (run PY tools/xdk_report.py)")
    if "functions" not in summ:
        raise Refuse(f"{_rel(sp)} has no functions key (run PY tools/xdk_report.py)")
    if check_ingest:  # 5.4 T1: a report built from another ingest is stale (a re-cut re-runs xdk_report)
        cur = xdk_report.ingest_sha256(pin)
        if not summ.get("ingestSha256") or summ["ingestSha256"] != cur:
            raise Refuse(f"{_rel(sp)} ingestSha256 {str(summ.get('ingestSha256'))[:12]} != current ingest "
                         f"{str(cur)[:12]} (run PY tools/xdk_report.py)")
    recs = [json.loads(line) for line in fp.read_text(encoding="utf-8").splitlines() if line.strip()]
    rp = REPO / "derived" / "delink" / f"retail-{pin[:8]}" / "report.json"
    if not rp.is_file():
        raise Refuse(f"{_rel(rp)} missing (run the delink/objdiff report)")
    report = json.loads(rp.read_text(encoding="utf-8"))
    gp = gap_path or GAPS
    if not gp.is_file():
        raise Refuse(f"gap list {gp} absent")
    gaps = load_gaps(gp, summ, recs, validate)
    return {"last": last, "last_path": last_path, "summ": summ, "summ_path": sp, "recs": recs, "recs_path": fp,
            "report": report, "report_path": rp, "gaps": gaps, "gaps_path": gp}


def compute(inp):
    """-> (errors, figures, lines)."""
    summ, recs, errs, lines = inp["summ"], inp["recs"], [], []
    dom = {s["name"]: s["dominant"] for s in summ["sections"]}
    gaps = {g["va"]: g for g in inp["gaps"]}
    moved = {}
    lines.append(f"listed gap functions ({_rel(inp['gaps_path'])}): {len(gaps)}")
    for g in inp["gaps"]:
        lines.append(f"  0x{g['va']:08X} size {g['size']} {g.get('section')} {g.get('symbol')} "
                     f"evidence {g.get('evidence')}")
        sec = [s["name"] for s in summ["sections"]
               if int(s["va"], 16) <= g["va"] < int(s["va"], 16) + s["virtualSize"]]
        name = sec[0] if sec else g.get("section")
        moved[name] = moved.get(name, 0) + g["size"]
    # reconcile
    lines.append("reconcile (bytes): counted + excluded + nonFunction == virtualSize "
                 "(listed gap bytes moved from nonFunction to counted)")
    D = excl_t = nonf_t = vs_t = 0
    for s in summ["sections"]:
        game_dom = s["dominant"] == "game"
        mv = moved.get(s["name"], 0)
        cnt = s["game"] + (sum(s[k] for k in SPLIT) if game_dom else 0) + mv
        exc = sum(s[k] for k in ALWAYS_EXCLUDED) + (0 if game_dom else sum(s[k] for k in SPLIT))
        nonf = s["nonFunction"] - mv
        tot = cnt + exc + nonf
        ok = tot == s["virtualSize"]
        lines.append(f"  {s['name']} (dominant {s['dominant']}): {cnt} + {exc} + {nonf} = {tot} "
                     f"{'==' if ok else '!='} {s['virtualSize']} {'ok' if ok else 'FAIL'}"
                     + (f" ({mv} listed gap bytes moved from nonFunction)" if mv else ""))
        if not ok:
            errs.append(f"reconcile {s['name']}: {tot} != {s['virtualSize']}")
        D, excl_t, nonf_t, vs_t = D + cnt, excl_t + exc, nonf_t + nonf, vs_t + s["virtualSize"]
    b = summ["bucket"]
    tot = D + excl_t + nonf_t
    ok = tot == b["execSectionBytes"] == vs_t and nonf_t == b["nonFunction"] - sum(moved.values())
    lines.append(f"  total: {D} + {excl_t} + {nonf_t} = {tot}; execSectionBytes {b['execSectionBytes']}; "
                 f"sum virtualSize {vs_t} {'ok' if ok else 'FAIL'}")
    if not ok:
        errs.append(f"reconcile total: {tot} / {b['execSectionBytes']} / {vs_t}")
    fs = summ["functions"]
    nby = sum(fs["byClass"].values())
    ok = nby == fs["count"] == len(recs)
    lines.append(f"reconcile (functions): sum byClass {nby}, count {fs['count']}, records {len(recs)} "
                 f"{'ok' if ok else 'FAIL'}")
    if not ok:
        errs.append(f"reconcile functions: {nby} / {fs['count']} / {len(recs)}")
    counted = {int(r["entry"], 16): r for r in recs if _counted(r["class"], dom.get(r["section"]))}
    entries = {int(r["entry"], 16): r for r in recs}
    B = len(counted) + len(gaps)
    # numerator
    items = {}
    for u in inp["report"].get("units", []):
        for f in u.get("functions") or []:
            items[(u["name"], f["name"])] = f
    a = c = 0
    for row in inp["last"].get("rows", []):
        if not matched_set.is_done(row):
            continue
        unit = row["unit"].replace("\\", "/")
        it = items.get((unit, row["symbol"]))
        if it is None:
            errs.append(f"done row {row['symbol']}: no report.json item in unit {unit}")
        elif int(it["size"]) != row["size"]:
            errs.append(f"done row {row['symbol']}: report.json size {it['size']} != row size {row['size']}")
        if row["va"] in gaps:
            if gaps[row["va"]]["size"] != row["size"]:
                errs.append(f"done row {row['symbol']} va 0x{row['va']:08X}: size {row['size']} != listed gap "
                            f"size {gaps[row['va']]['size']}")
        elif row["va"] not in counted:
            r = entries.get(row["va"])
            why = "no functions.jsonl entry" if r is None else f"class {r['class']} in {r['section']} not counted"
            errs.append(f"done row {row['symbol']} va 0x{row['va']:08X}: {why}")
        a, c = a + 1, c + row["size"]
    return errs, {"a": a, "B": B, "c": c, "D": D}, lines


def render(inp, fig):
    a, B, c, D = fig["a"], fig["B"], fig["c"], fig["D"]
    p = 100.0 * a / B if B else 0.0
    q = 100.0 * c / D if D else 0.0
    src = inp["summ"]["functions"]["source"]
    n, m, gp = len(inp["gaps"]), sum(g["size"] for g in inp["gaps"]), _rel(inp["gaps_path"])
    return [f"functions: {a} of {B} ({p:.2f}%)   bytes: {c} of {D} ({q:.2f}%)",
            f"functions denominator: {_rel(inp['recs_path'])} (population {src}), records counted where class == "
            f"game, or class in mixed/unidentified and the section's dominant == game + {n} listed gap functions "
            f"({gp})",
            f"bytes denominator: {_rel(inp['summ_path'])} sections[], sum of game + (mixed + unidentified where "
            f"dominant == game) + {m} bytes",
            f"remainder: {B - a} functions, {D - c} bytes",
            f"claim type: gate verdicts (D40), rows with matched_set.is_done in {_rel(inp['last_path'])}",
            f"last.json date: {inp['last'].get('date')}"]


def run(last_path, gap_path=None, validate=True, **kw):
    """-> (rc, figures or None, output lines). kw: load()'s xdk_dir, check_ingest (self-test controls only)."""
    try:
        inp = load(last_path, gap_path, validate, **kw)
    except Refuse as e:
        return 2, None, [f"progress: REFUSED {e}"]
    errs, fig, lines = compute(inp)
    out = lines + render(inp, fig)
    if errs:
        return 1, fig, out + [f"FAIL {e}" for e in errs]
    return 0, fig, out


def _print(rc, lines):
    for ln in lines:
        print(ln, file=sys.stderr if rc == 2 else sys.stdout)


def self_test():
    d = byte_compare._phase_run_dir() / "progress_selftest"
    if d.exists():
        shutil.rmtree(d)
    d.mkdir(parents=True)
    try:
        pin = coff_info.pinned_sha()
        fns = [e for e in matched_set.load(FIXTURE) if e["kind"] == "function"]
        rows = [{"symbol": e["symbol"], "unit": e["unit"], "va": e["va"], "size": e["size"],
                 "body_sha1": e["body_sha1"], "verdicts": {k: "pass" for k in matched_set.REQUIRED}, "passed": True}
                for e in fns]
        bad = json.loads(json.dumps(rows[0]))
        bad["verdicts"]["size"], bad["passed"] = "fail: control", False
        rows.append(bad)
        want_a, want_c = len(fns), sum(e["size"] for e in fns)
        lp = d / "last.json"
        doc = {"date": "self-test", "xbe_sha1": pin, "rows": rows}
        lp.write_text(json.dumps(doc, indent=1) + "\n", encoding="utf-8", newline="\n")
        results = []
        rc, fig, lines = run(lp)
        _print(rc, lines)
        ok = rc == 0 and fig["a"] == want_a and fig["c"] == want_c
        results.append(ok)
        print(f"self-test fixture: rc {rc}, functions {fig['a'] if fig else '-'} == {want_a}, bytes "
              f"{fig['c'] if fig else '-'} == {want_c} {'ok' if ok else 'FAIL'}")
        try:
            ng, why = len(load(lp)["gaps"]), ""
        except Refuse as e:
            ng, why = 0, f" ({e})"
        # 5.3 T17: was `ng > 0 and not why`; every entry is now a fresh xdk record (list empty by its own definition),
        # so the check is that every [[gap]] header in the file loads (a silent drop still fails) and nothing refuses
        nh = len(re.findall(r"(?m)^\[\[gap\]\]\s*$", GAPS.read_text(encoding="utf-8"))) if GAPS.is_file() else -1
        ok = ng == nh and not why
        results.append(ok)
        print(f"self-test real gap list {_rel(GAPS)}: {ng} entries load clean{why} {'ok' if ok else 'FAIL'}")
        # T18 (5.2): one scratch list per refusal kind; each must pass unvalidated (rc 0) and refuse validated (2)
        ctl = {"no-evidence": 'va = 0x00018190\nsize = 10\nsection = ".text"\nsymbol = "c"\n',
               "non-game-section": 'va = 0x00156160\nsize = 4\nsection = "D3D"\nsymbol = "c"\n'
                                   'evidence = "call-site"\nat = 0x00011000\n',
               "record-overlap": 'va = 0x000E1418\nsize = 18\nsection = ".text"\nsymbol = "c"\n'
                                 'evidence = "call-site"\nat = 0x00011000\n'}
        for name, body in ctl.items():
            gp = d / f"gaps_{name}.toml"
            gp.write_text("[[gap]]\n" + body, encoding="utf-8", newline="\n")
            rc0, _, _ = run(lp, gp, validate=False)
            rc1, _, l1 = run(lp, gp)
            ok = rc0 == 0 and rc1 == 2
            results.append(ok)
            print(f"control gap {name}: unvalidated rc {rc0} == 0 (did not fail), validated rc {rc1} == 2 "
                  f"{'ok' if ok else 'FAIL'} ({l1[-1]})")
        # 5.4 T1: a scratch copy of the xdk report whose ingestSha256 is not the current ingest's
        real = REPO / "derived" / "xdk" / f"retail-{pin[:8]}"
        sx = d / "xdk_stale"
        sx.mkdir()
        shutil.copyfile(real / "functions.jsonl", sx / "functions.jsonl")
        ss = json.loads((real / "summary.json").read_text(encoding="utf-8"))
        ss["ingestSha256"] = "0" * 64
        (sx / "summary.json").write_text(json.dumps(ss, indent=1) + "\n", encoding="utf-8", newline="\n")
        rc0, _, _ = run(lp, xdk_dir=sx, check_ingest=False)
        rc1, _, l1 = run(lp, xdk_dir=sx)
        ok = rc0 == 0 and rc1 == 2
        results.append(ok)
        print(f"control stale-xdk: unchecked rc {rc0} == 0 (did not fail), checked rc {rc1} == 2 "
              f"{'ok' if ok else 'FAIL'} ({l1[-1]})")
        flip =dict(doc, xbe_sha1=("0" if pin[0] != "0" else "1") + pin[1:])
        lp.write_text(json.dumps(flip, indent=1) + "\n", encoding="utf-8", newline="\n")
        rc, _, lines = run(lp)
        results.append(rc == 2)
        print(f"control flipped-xbe_sha1: rc {rc} == 2 {'ok' if rc == 2 else 'FAIL'} ({lines[-1]})")
        lp.unlink()
        rc, _, lines = run(lp)
        results.append(rc == 2)
        print(f"control absent-last.json: rc {rc} == 2 {'ok' if rc == 2 else 'FAIL'} ({lines[-1]})")
        # T6 (2.1): rewrite only on a content change; scratch doc, never docs/progress.md
        sd, figs = d / "progress.md", [f"fig {i}" for i in range(6)]
        w0 = write_doc(sd, figs, "2000-01-01 00:00:00")
        h0 = hashlib.sha256(sd.read_bytes()).hexdigest()
        w1 = write_doc(sd, figs, "2000-01-01 00:00:01")
        h1 = hashlib.sha256(sd.read_bytes()).hexdigest()
        ok = w0 and not w1 and h0 == h1
        results.append(ok)
        print(f"control same-figures: wrote {w1}, sha256 {h0[:16]} -> {h1[:16]} {'ok' if ok else 'FAIL'}")
        w2 = write_doc(sd, figs[:-1] + ["fig changed"], "2000-01-01 00:00:02")
        h2 = hashlib.sha256(sd.read_bytes()).hexdigest()
        ok = w2 and h2 != h1 and "2000-01-01 00:00:02" in sd.read_text(encoding="utf-8")
        results.append(ok)
        print(f"control changed-figure: wrote {w2}, sha256 {h1[:16]} -> {h2[:16]} {'ok' if ok else 'FAIL'}")
    finally:
        shutil.rmtree(d, ignore_errors=True)
    print(f"self-test: {'ok' if all(results) else 'FAILED'}")
    return 0 if all(results) else 1


def last_current(last_path):
    """5.5 T6 (C1): -> refusal text or None. last.json must hold no failed row and a `tree` block (gate.py _write_last)
    whose list_sha256 / fp_sha256 equal config/matched_set.toml / config/verifier.sha256 now; head and dirty are not
    compared (a log commit moves head without touching a verdict); 5.5 T12.c2 (G1): its listed_dirty must be present
    and empty (no LISTED file differed from HEAD at the gate). Default and --last runs; --self-test is
    fixture-only (its last.json carries a deliberate failed row and no tree)."""
    if not last_path.is_file():
        return None  # load() refuses absence
    last = json.loads(last_path.read_text(encoding="utf-8"))
    bad = [r.get("symbol") for r in last.get("rows", []) if not r.get("passed")]
    if bad:
        return f"{_rel(last_path)} has {len(bad)} failed row(s), first {bad[0]} (fix and re-run PY tools/gate.py)"
    tree = last.get("tree") or {}
    for k, p in (("list_sha256", REPO / "config" / "matched_set.toml"),
                 ("fp_sha256", REPO / "config" / "verifier.sha256")):  # gate.py LIST, verifier_lock.FP
        now = hashlib.sha256(p.read_bytes()).hexdigest() if p.is_file() else None
        if tree.get(k) != now:
            return (f"{_rel(last_path)} tree {k} {str(tree.get(k))[:12]} != {_rel(p)} now {str(now)[:12]} "
                    "(re-run PY tools/gate.py)")
    ld = tree.get("listed_dirty")  # 5.5 T12.c2 (G1 ruling 1a): a verdict of uncommitted verifier files is no verdict
    if ld is None:
        return f"{_rel(last_path)} tree listed_dirty absent (git failed or an older gate; re-run PY tools/gate.py)"
    if ld:
        return (f"{_rel(last_path)} tree listed_dirty {len(ld)}: {', '.join(ld[:3])} (LISTED files differ from HEAD; "
                "commit them, then re-run PY tools/gate.py)")
    return None


def main(argv):
    ap = argparse.ArgumentParser(prog="progress.py", description=__doc__.splitlines()[0])
    ap.add_argument("--last", help="gate last.json path (default derived/gate/last.json); no doc written")
    ap.add_argument("--self-test", action="store_true", help="fixture run + refusal controls; no doc written")
    args = ap.parse_args(argv)
    if args.self_test:
        return self_test()
    pin = coff_info.pinned_sha()
    why = pin and delink.cut_check(delink.cut_dir(pin))  # T1 (2.1): before any cut read; --self-test is fixture-only
    if why:
        _print(2, [f"progress: REFUSED {why}"])
        return 2
    lp = Path(args.last).resolve() if args.last else LAST
    why = last_current(lp)  # 5.5 T6 (C1): was bound by xbe_sha1 only (load())
    if why:
        _print(2, [f"progress: REFUSED {why}"])
        return 2
    rc, fig, lines = run(lp)
    _print(rc, lines)
    if rc == 0 and not args.last:
        now = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d %H:%M:%S")
        wrote = write_doc(DOC, lines[-6:], now)
        print(f"{'wrote' if wrote else 'unchanged (figures equal; not rewritten)'} {_rel(DOC)}")
    return rc


def _unstamped(text):
    return [ln for ln in text.split("\n") if not ln.startswith(STAMP)]


def write_doc(path, figs, now):
    """Write the progress doc only when its body without the stamp line differs (T6 2.1); True iff written."""
    body = "\n".join(["# Progress", "",
                      f"{STAMP}{now} UTC by `{CMD}` (PY = the project Python, HOW_WE_WORK.md); generated; do not edit.",
                      ""] + [f"- {ln}" for ln in figs]) + "\n"
    if path.exists() and _unstamped(path.read_text(encoding="utf-8")) == _unstamped(body):
        return False
    path.write_text(body, encoding="utf-8", newline="\n")
    return True


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
