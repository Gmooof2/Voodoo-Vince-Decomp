"""Progress figures (T7, phase 1.5): matched functions and bytes against the counted game population.

CLI: progress.py [--last P] [--self-test]
Inputs: derived/gate/last.json (`PY tools/gate.py`; or --last P), derived/xdk/retail-<sha8>/{summary.json,
  functions.jsonl} (`PY tools/xdk_report.py`), derived/delink/retail-<sha8>/report.json (objdiff report).
Numerator: last.json rows with matched_set.is_done(row) (D28, the only done test): a = count, c = sum of row size.
  Each done row must have an objdiff ReportItem (unit name with `/`, name == symbol) of the same size, and its va
  must be the entry of a counted functions.jsonl record; else exit 1 with the cause.
Rule (binding): counted(class, dominant) = class == "game" or (class in {mixed, unidentified} and the section's
  summary.sections[].dominant == "game"). B = counted functions.jsonl records; D = sum over sections of game bytes
  + (mixed + unidentified where dominant == game).
Cut: default and --last runs refuse (2) first unless delink.cut_check passes (valid cut.complete, T1 2.1).
Reconcile: per section and total, counted + excluded (xdk + xdk-label + linker + mixed/unidentified in non-game
  sections) + nonFunction == virtualSize (total == execSectionBytes == sum virtualSize); sum byClass == functions
  count == records. Any mismatch -> exit 1, docs/progress.md not written.
Output: `functions: a of B (p%)   bytes: c of D (q%)`, sources, remainder, claim type, last.json date. Counts only
  (D46). A default run that exits 0 writes docs/progress.md (generated, dated, with its command, D59); --last P and
  --self-test never write it; it is rewritten only when its body without the `Generated` stamp line differs (T6 2.1),
  else `unchanged` is printed. --self-test builds last.json in a scratch dir under .run/<phase>/ from
  tests/gate/matched_set.toml (5 rows all pass + 1 row with a failed verdict) and runs two refusal controls.
Exit: 0 ok; 1 a check failed; 2 refusal (stderr `progress: REFUSED <why>`).
"""
import argparse
import datetime
import hashlib
import json
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import byte_compare  # noqa: E402
import coff_info  # noqa: E402
import delink  # noqa: E402
import matched_set  # noqa: E402

REPO = Path(__file__).resolve().parent.parent
LAST = REPO / "derived" / "gate" / "last.json"
DOC = REPO / "docs" / "progress.md"
FIXTURE = REPO / "tests" / "gate" / "matched_set.toml"
CMD = "PY tools/progress.py"
STAMP = "Generated "
SPLIT = ("mixed", "unidentified")
ALWAYS_EXCLUDED = ("xdk", "xdk-label", "linker")


class Refuse(Exception):
    pass


def _rel(p):
    return p.relative_to(REPO).as_posix()


def _counted(cls, dominant):
    return cls == "game" or (cls in SPLIT and dominant == "game")


def load(last_path):
    """-> inputs dict; raises Refuse."""
    pin = coff_info.pinned_sha()
    if not pin:
        raise Refuse("no retail_xbe pin in config/check.retail_xbe.sha")
    if not last_path.is_file():
        raise Refuse(f"{last_path} absent (run PY tools/gate.py)")
    last = json.loads(last_path.read_text(encoding="utf-8"))
    if last.get("xbe_sha1") != pin:
        raise Refuse(f"{last_path} xbe_sha1 {last.get('xbe_sha1')} != pin {pin}")
    xdk = REPO / "derived" / "xdk" / f"retail-{pin[:8]}"
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
    recs = [json.loads(line) for line in fp.read_text(encoding="utf-8").splitlines() if line.strip()]
    rp = REPO / "derived" / "delink" / f"retail-{pin[:8]}" / "report.json"
    if not rp.is_file():
        raise Refuse(f"{_rel(rp)} missing (run the delink/objdiff report)")
    report = json.loads(rp.read_text(encoding="utf-8"))
    return {"last": last, "last_path": last_path, "summ": summ, "summ_path": sp, "recs": recs, "recs_path": fp,
            "report": report, "report_path": rp}


def compute(inp):
    """-> (errors, figures, lines)."""
    summ, recs, errs, lines = inp["summ"], inp["recs"], [], []
    dom = {s["name"]: s["dominant"] for s in summ["sections"]}
    # reconcile
    lines.append("reconcile (bytes): counted + excluded + nonFunction == virtualSize")
    D = excl_t = nonf_t = vs_t = 0
    for s in summ["sections"]:
        game_dom = s["dominant"] == "game"
        cnt = s["game"] + (sum(s[k] for k in SPLIT) if game_dom else 0)
        exc = sum(s[k] for k in ALWAYS_EXCLUDED) + (0 if game_dom else sum(s[k] for k in SPLIT))
        tot = cnt + exc + s["nonFunction"]
        ok = tot == s["virtualSize"]
        lines.append(f"  {s['name']} (dominant {s['dominant']}): {cnt} + {exc} + {s['nonFunction']} = {tot} "
                     f"{'==' if ok else '!='} {s['virtualSize']} {'ok' if ok else 'FAIL'}")
        if not ok:
            errs.append(f"reconcile {s['name']}: {tot} != {s['virtualSize']}")
        D, excl_t, nonf_t, vs_t = D + cnt, excl_t + exc, nonf_t + s["nonFunction"], vs_t + s["virtualSize"]
    b = summ["bucket"]
    tot = D + excl_t + nonf_t
    ok = tot == b["execSectionBytes"] == vs_t and nonf_t == b["nonFunction"]
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
    B = len(counted)
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
        if row["va"] not in counted:
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
    return [f"functions: {a} of {B} ({p:.2f}%)   bytes: {c} of {D} ({q:.2f}%)",
            f"functions denominator: {_rel(inp['recs_path'])} (population {src}), records counted where class == "
            f"game, or class in mixed/unidentified and the section's dominant == game",
            f"bytes denominator: {_rel(inp['summ_path'])} sections[], sum of game + (mixed + unidentified where "
            f"dominant == game)",
            f"remainder: {B - a} functions, {D - c} bytes",
            f"claim type: gate verdicts (D40), rows with matched_set.is_done in {_rel(inp['last_path'])}",
            f"last.json date: {inp['last'].get('date')}"]


def run(last_path):
    """-> (rc, figures or None, output lines)."""
    try:
        inp = load(last_path)
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
        flip = dict(doc, xbe_sha1=("0" if pin[0] != "0" else "1") + pin[1:])
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
    rc, fig, lines = run(Path(args.last).resolve() if args.last else LAST)
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
