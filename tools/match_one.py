"""match_one (1.6 T3): build one function's unit alone and run gate's objdiff + bytes layers on it. NOT a match claim.

CLI: match_one.py --symbol S --task T<n> [--src DIR (default src)] [--config P] [--ledger P] [--control "<flags>"]
  [--listing] [--workdir NAME]  |  match_one.py --table [--ledger P]
  S: exact mangled name from a func line of gate.SYMTABS, else a unique MSVC qualified name (`Class::Method`,
  `Class::~Class`, `std::fill`, a C name without its `_`). Qualified names of `?` symbols come from the pinned
  toolchain's `undname.exe 0x1000` (template args stripped), run only on rows containing the last name component.
Unit: the units.toml unit with text_start <= va < text_end. Workdir <.run/phase>/match_one (wiped first): --src copied
to W/src minus every .c/.cpp except the unit's source (build.unit_path); build.build(W/src, W/build).
Target: derived/delink/retail-<pin[:8]>/<gate._tname(unit)>, sha256 == its hashes.txt line; refusal 2 first unless
delink.cut_check passes (valid cut.complete, T1 2.1). objdiff via gate._objdiff;
bytes via byte_compare.setup + compare. Never writes derived/gate/last.json.
Output: header `<symbol> unit <unit> va 0x.. size 0x..`; `PASS|FAIL objdiff: <pct>`; `PASS|FAIL bytes: <verdict>
(masked <n>)`; `flags: <unit flags>`; last line `not a match claim: run PY tools/gate.py`.
Exit: 0 objdiff 100.0 and bytes `match`; 1 any failure; 2 refusal (symbol absent/ambiguous, no unit/source,
build.Refuse, byte_compare.Refuse, --task missing). W removed on pass, kept (path printed) on fail or --listing.
Ledger (2.2 T2): every run, refusals too, appends one JSON line to --ledger (default docs/matching/attempts-<phase>.jsonl,
4.4 T11: attempts.jsonl is the frozen base; readers take it first, then the phase files by phase; relative to the repo): keys ts (UTC ISO seconds), phase (byte_compare._phase_run_dir().name), task, symbol, va
("0x%08X"), size (int), insns (iced Decoder(32) count over retail bytes [va, va+size)), unit, opt (flags built with),
kind (attempt|control|listing), objdiff (pct float or verdict), bytes (verdict), rc; unknown -> null. No byte content (D46).
The append holds an OS byte lock on the ledger (4.1 T6; a plain append lost and tore rows under concurrent runs).
--control F: the unit is built with opt = F (units.toml copy W/units.toml, passed as build's units_path; works for
`opt = "unknown"` units); prints `control: F` and `control run: no claim`; kind control.
--listing: after the build, the unit's source is compiled again (msvc71.compile_one, same flags + `/FAs`
`/Fa<W>/listing/<obj stem>.asm`); prints `listing: <path>`; W kept; kind listing (control wins if both).
--workdir NAME (4.1 T6): W is <.run/phase>/match_one.<NAME>, a sibling of the default folder, which a default run wipes;
only W is wiped. NAME: letters, digits, and single . _ - between them; anything else refuses before any wipe. Use it
whenever match_one runs may overlap (coders in parallel), one NAME per coder (C0152). Default: unchanged, one folder.
--table: writes docs/matching/attempts_by_size.md from the ledger + config/matched_set.toml (definitions in its
header; deterministic, no timestamp) and exits 0.
"""
import argparse
import contextlib
import datetime
import hashlib
import io
import json
import msvcrt
import re
import shutil
import statistics
import subprocess
import sys
import time
import tomllib
from pathlib import Path

from iced_x86 import Decoder

sys.path.insert(0, str(Path(__file__).resolve().parent))
import build  # noqa: E402
import byte_compare  # noqa: E402
import coff_info  # noqa: E402
import delink  # noqa: E402
import gate  # noqa: E402
import matched_set  # noqa: E402
import msvc71  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

REPO = gate.REPO
LEDGER = REPO / "docs" / "matching" / "attempts.jsonl"  # base, frozen at 4.4 T11 (audit oversize); never appended
PHASE_LEDGER = re.compile(r"attempts-(\d+(?:\.\d+)*)\.jsonl")  # 4.4 T11: one appended ledger per phase beside it
TABLE = REPO / "docs" / "matching" / "attempts_by_size.md"
MATCHED = REPO / "config" / "matched_set.toml"
KEYS = ("ts", "phase", "task", "symbol", "va", "size", "insns", "unit", "opt", "kind", "objdiff", "bytes", "rc")
WORKDIR_NAME = re.compile(r"[A-Za-z0-9]+(?:[._-][A-Za-z0-9]+)*")
LEDGER_LOCK_AT = 1 << 40  # a byte past any ledger's end: the OS lock there blocks no read or write of the rows
BUCKETS = ((1, 8), (9, 16), (17, 32), (33, 64), (65, 128), (129, None))


class Refuse(Exception):
    pass


def _funcs():
    """-> [(name, va, size)] of func lines in gate.SYMTABS."""
    rows = []
    for t in gate.SYMTABS:
        for line in t.read_text(encoding="utf-8").splitlines():
            p = line.split()
            if len(p) < 6 or p[0].startswith("#") or p[1] != "func":
                continue
            rows.append((" ".join(p[3:-2]), int(p[0], 16), None if p[2] == "-" else int(p[2], 16)))
    return rows


def _strip_targs(s):
    out, depth = [], 0
    for ch in s:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif depth == 0:
            out.append(ch)
    return "".join(out)


def _undname(names, config):
    """-> {mangled: qualified name} via undname.exe 0x1000 (name only); rc checked (D26)."""
    exe = Path(vv_inputs.load_config(config).get("msvc71_dir") or "") / "undname.exe"
    if not exe.is_file():
        raise Refuse(f"undname.exe missing: {exe}")
    got = {}
    for i in range(0, len(names), 40):
        chunk = names[i:i + 40]
        p = subprocess.run([str(exe), "0x1000", *chunk], stdin=subprocess.DEVNULL, capture_output=True, text=True,
                           errors="replace")
        if p.returncode != 0:
            raise Refuse(f"undname rc {p.returncode}: {(p.stdout + p.stderr).strip()[-200:]}")
        outs = re.findall(r'^is :- "(.*)"$', p.stdout, re.M)
        if len(outs) != len(chunk):
            raise Refuse(f"undname returned {len(outs)} names for {len(chunk)}")
        got.update(zip(chunk, outs))
    return got


def resolve(sym, config=None):
    """-> (mangled, va, size); Refuse when absent or ambiguous."""
    rows = _funcs()
    exact = [r for r in rows if r[0] == sym]
    if len(exact) == 1:
        return exact[0]
    if len(exact) > 1:
        raise Refuse(f"{sym}: {len(exact)} func rows: " + ", ".join(f"0x{r[1]:08X}" for r in exact))
    last = _strip_targs(sym).split("::")[-1].lstrip("~")
    near = [r for r in rows if last and last in r[0]]
    qual = {}
    for r in near:
        if not r[0].startswith("?"):
            qual[r[0]] = re.sub(r"@\d+$", "", r[0][1:] if r[0].startswith("_") else r[0])
    qual.update(_undname(sorted({r[0] for r in near if r[0].startswith("?")}), config))
    hits = [r for r in near if sym in (qual.get(r[0]), _strip_targs(qual.get(r[0], "")))]
    if len(hits) == 1:
        return hits[0]
    cands = hits or near
    lst = "".join(f"\n  0x{r[1]:08X} {r[0]}  ({qual.get(r[0])})" for r in cands[:20])
    raise Refuse(f"{sym}: {'ambiguous' if hits else 'not found'}; {len(cands)} candidate(s) (first 20):{lst or ' -'}")


def _insns(va, size, config=None):
    """-> iced Decoder(32) instruction count over retail XBE bytes [va, va+size) (as candidates.decode), or None."""
    if size is None:
        return None
    try:
        xb = vv_inputs.input_path("retail_xbe", cfg=vv_inputs.load_config(config)).read_bytes()
        x = xbe_info.parse_xbe(xb)
        off = xbe_info.va_to_off(x, va)
        if off is None or xbe_info.va_to_off(x, va + size - 1) != off + size - 1:
            return None
    except Exception:  # noqa: BLE001 -- ledger field only; unknown -> null
        return None
    d, n = Decoder(32, xb[off:off + size], ip=va), 0
    while d.can_decode:
        d.decode()
        n += 1
    return n


def _control_units(unit, flags, out):
    """Write build.UNITS with `unit`'s opt line replaced by `flags` to `out`; -> out. Refuse when not exactly one."""
    lines = build.UNITS.read_text(encoding="utf-8").splitlines(keepends=True)
    name_line = f"name = {json.dumps(unit['name'])}"
    starts = [i for i, l in enumerate(lines) if l.strip() == "[[unit]]"] + [len(lines)]
    hit = 0
    for a, b in zip(starts, starts[1:]):
        if not any(l.strip() == name_line for l in lines[a:b]):
            continue
        for i in range(a, b):
            if re.match(r"\s*opt\s*=", lines[i]):
                lines[i] = f"opt = {json.dumps(flags)}\n"
                hit += 1
    if hit != 1:
        raise Refuse(f"--control: {hit} opt lines for unit {unit['name']} in {build.UNITS}")
    out.write_text("".join(lines), encoding="utf-8")
    got = [u for u in tomllib.loads(out.read_text(encoding="utf-8"))["unit"] if u.get("name") == unit["name"]]
    if len(got) != 1 or got[0].get("opt") != flags:
        raise Refuse(f"--control: units copy {out} does not give unit {unit['name']} opt {flags!r}")
    return out


def run(sym, src, config=None, rec=None, control=None, listing=False, workdir=None):
    rec = {} if rec is None else rec
    rec["symbol"] = sym
    try:
        why = delink.cut_check(delink.cut_dir(coff_info.pinned_sha() or ""))  # T1 (2.1): cut.complete first
        if why:
            raise Refuse(why)
        mangled, va, size = resolve(sym, config)
        rec.update(symbol=mangled, va=f"0x{va:08X}", size=size, insns=_insns(va, size, config))
        us = [u for u in matched_set._units() if isinstance(u.get("text_start"), int)
              and isinstance(u.get("text_end"), int) and u["text_start"] <= va < u["text_end"]]
        if len(us) != 1:
            raise Refuse(f"va 0x{va:08X} lies in {len(us)} units (need 1)")
        unit = us[0]
        rec.update(unit=unit["name"], opt=control if control is not None else unit.get("opt"))
        rel = build.unit_path(unit)
        if rel is None:
            raise Refuse(f"unit {unit['name']} has no source path")
        src = Path(src).resolve()
        srcs = [p for p in src.rglob("*") if p.is_file() and p.relative_to(src).as_posix().lower() == rel]
        if len(srcs) != 1:
            raise Refuse(f"{len(srcs)} sources for {rel} under {src}")
        if workdir is not None and not WORKDIR_NAME.fullmatch(workdir):
            raise Refuse(f"--workdir {workdir!r}: one name of letters, digits and single . _ - between them")
        w = byte_compare._phase_run_dir() / ("match_one" if workdir is None else f"match_one.{workdir}")
        shutil.rmtree(w, ignore_errors=True)
        units_path = build.UNITS
        if control is not None:
            w.mkdir(parents=True)
            units_path = _control_units(unit, control, w / "units.toml")
    except Refuse as e:
        print(f"match_one: REFUSED {e}", file=sys.stderr)
        return 2
    u = unit["name"]
    print(f"{mangled} unit {u} va 0x{va:08X} size {'-' if size is None else f'0x{size:X}'}")
    if control is not None:
        print(f"control: {control}")
    shutil.copytree(src, w / "src", ignore=lambda d, names: [
        n for n in names if Path(n).suffix.lower() in (".c", ".cpp") and (Path(d) / n).resolve() != srcs[0]])
    rc = 1
    try:
        try:
            built = {b["unit"]: b for b in build.build(w / "src", w / "build", units_path, config)}
        except build.BuildFailed as e:
            for f in e.failures:
                print(f"FAIL build: {f['source']} {f['codes']}")
            raise _Done
        except build.Refuse as e:
            print(f"match_one: REFUSED build: {e}", file=sys.stderr)
            rc = 2
            raise _Done
        if u not in built:
            print(f"FAIL build: no obj for unit {u}")
            raise _Done
        obj = built[u]["obj"]
        rec["opt"] = built[u]["flags"]
        if listing:
            ld = w / "listing"
            ld.mkdir(parents=True, exist_ok=True)
            asm = ld / f"{Path(obj).stem}.asm"
            ok_l, codes, _text = msvc71.compile_one(Path(vv_inputs.load_config(config)["msvc71_dir"]),
                                                    w / "src" / built[u]["source"], built[u]["flags"].split()
                                                    + ["/FAs", f"/Fa{asm}"], ld / Path(obj).name, scratch=ld)
            if not ok_l or not asm.is_file():
                print(f"FAIL listing: {asm} {codes}")
                raise _Done
            print(f"listing: {asm}")
        tdir = REPO / "derived" / "delink" / f"retail-{(coff_info.pinned_sha() or '')[:8]}"
        name, hashes = gate._tname(u), gate._hashes(tdir)
        t = tdir / name
        if not t.is_file() or name not in hashes or hashlib.sha256(t.read_bytes()).hexdigest() != hashes[name]:
            print(f"FAIL target: {t} missing, unlisted in hashes.txt, or sha256 mismatch")
            raise _Done
        rep, gv = gate._objdiff(w / "build", {u: (t, obj)}, vv_inputs.load_config(config))
        pct = rep.get(u.replace("\\", "/"), {}).get(mangled)
        ok_o = gv == "pass" and pct == 100.0
        rec["objdiff"] = pct if gv == "pass" else gv
        print(f"{'PASS' if ok_o else 'FAIL'} objdiff: {pct if gv == 'pass' else gv}")
        try:
            _p, byname, _t, _xp, xb, x = byte_compare.setup(argparse.Namespace(
                symbols=[str(p) for p in gate.SYMTABS], xbe=None, config=config))
            with contextlib.redirect_stdout(io.StringIO()):
                bv, nmask, _n = byte_compare.compare(obj, mangled, va, byname, xb, x)
        except byte_compare.Refuse as e:
            print(f"match_one: REFUSED bytes: {e}", file=sys.stderr)
            rc = 2
            raise _Done
        rec["bytes"] = bv
        print(f"{'PASS' if bv == 'match' else 'FAIL'} bytes: {bv} (masked {nmask})")
        print(f"flags: {' '.join(built[u]['flags']) if isinstance(built[u]['flags'], list) else built[u]['flags']}")
        rc = 0 if ok_o and bv == "match" else 1
    except _Done:
        pass
    if rc == 0 and not listing:
        shutil.rmtree(w, ignore_errors=True)
    else:
        print(f"workdir kept: {w}")
    if control is not None:
        print("control run: no claim")
    print("not a match claim: run PY tools/gate.py")
    return rc


class _Done(Exception):
    pass


def _rel(p):
    p = Path(p).resolve()
    return p.relative_to(REPO).as_posix() if p.is_relative_to(REPO) else p.as_posix()


def phase_ledger():
    """The current phase's appended ledger, docs/matching/attempts-<phase>.jsonl (4.4 T11)."""
    name = byte_compare._phase_run_dir().name
    if not PHASE_LEDGER.fullmatch(f"attempts-{name}.jsonl"):
        raise SystemExit(f"match_one: REFUSED no open phase for the ledger name (got {name!r})")
    return LEDGER.with_name(f"attempts-{name}.jsonl")


def ledger_paths():
    """Ledger files in ledger order: the base attempts.jsonl, then attempts-<phase>.jsonl by phase as an integer tuple
    (4.10 after 4.9; a plain name sort puts '-' before '.', 4.4 T11)."""
    ph = [(tuple(int(x) for x in m.group(1).split(".")), p) for p in LEDGER.parent.glob("attempts-*.jsonl")
          if (m := PHASE_LEDGER.fullmatch(p.name))]
    return [LEDGER] + [p for _k, p in sorted(ph)]


def load_ledger(paths=None):
    """Every ledger row, in ledger order over `paths` (default ledger_paths())."""
    return [json.loads(l) for p in (paths or ledger_paths()) if p.is_file()
            for l in p.read_text(encoding="utf-8").splitlines() if l.strip()]


def table(ledger):
    """Write TABLE from the ledger files (a list of paths) + MATCHED; -> 0, 1 when a matched symbol has no passing
    attempt row (names printed, TABLE not written), or 2 when either is unreadable."""
    try:
        rows = load_ledger(ledger)
        matched = {e["symbol"] for e in matched_set.load(MATCHED) if e["kind"] == "function"}
    except (OSError, ValueError, matched_set.Refuse) as e:
        print(f"match_one: REFUSED table: {e}", file=sys.stderr)
        return 2
    att = [r for r in rows if r.get("kind") == "attempt" and r.get("rc") in (0, 1)]
    insns, n_try, first = {}, {}, {}
    for r in att:  # ledger (file) order
        s = r["symbol"]
        if insns.get(s) is None:
            insns[s] = r.get("insns")
        if s not in first:
            n_try[s] = n_try.get(s, 0) + 1
            if r["rc"] == 0:
                first[s] = n_try[s]
    missing = sorted(matched - first.keys())
    if missing:
        for s in missing:
            print(f"match_one: matched without a passing attempt row (kind attempt, rc 0): {s}", file=sys.stderr)
        print(f"match_one: FAIL {len(missing)} of {len(matched)} matched symbols unledgered in {' + '.join(_rel(x) for x in ledger)};"
              " table not written", file=sys.stderr)
        return 1
    out = ["# Match attempts by function size",
           "",
           f"Generated by `PY tools/match_one.py --table` from `{'` + `'.join(_rel(x) for x in ledger)}` and `config/matched_set.toml`;"
           " never hand-edited.",
           "",
           "- insns: retail instruction count (iced Decoder(32) over XBE bytes [va, va+size)), the row's `insns`.",
           "- attempt row: ledger row with `kind` = attempt and `rc` 0 or 1 (control/listing runs and refusals excluded).",
           "- attempted: distinct symbols with >= 1 attempt row, bucketed by their `insns` (null: `unknown`).",
           "- matched: attempted symbols listed in `config/matched_set.toml`.",
           "- attempts to first pass: per matched symbol, its attempt rows in ledger order up to and including the"
           " first with `rc` 0; median and max over matched symbols (`-` when none has an rc 0 attempt row).",
           "- match rate: matched / attempted.",
           "",
           "| insns | attempted | matched | attempts to first pass (median) | (max) | match rate |",
           "|---|---|---|---|---|---|"]
    def _rows(ins, rows_col=False):
        for lo, hi in BUCKETS + ((None, None),):
            if lo is None:
                label, syms = "unknown", sorted(s for s, n in ins.items() if n is None)
            else:
                label = f"{lo}-{hi}" if hi else f"{lo}+"
                syms = sorted(s for s, n in ins.items() if n is not None and n >= lo and (hi is None or n <= hi))
            m = [s for s in syms if s in matched]
            f = sorted(first[s] for s in m if s in first)
            med = "-" if not f else f"{statistics.median(f):g}"
            rate = "-" if not syms else f"{100 * len(m) / len(syms):.1f}%"
            extra = f" {sum(1 for r in att if r['symbol'] in syms)} |" if rows_col else ""
            out.append(f"| {label} | {len(syms)} |{extra} {len(m)} | {med} | {max(f) if f else '-'} | {rate} |")
    _rows(insns)
    phase_of = {}
    for r in att:  # a symbol belongs to the phase of its first attempt row (2.5 T9)
        phase_of.setdefault(r["symbol"], r.get("phase") or "unknown")
    out += ["", "## By phase", "",
            "Each attempted symbol counted once, in the phase of its first attempt row; attempt rows: all its"
            " attempt rows (any phase)."]
    for ph in sorted(set(phase_of.values())):
        out += ["", f"### phase {ph}", "",
                "| insns | attempted | attempt rows | matched | attempts to first pass (median) | (max) | match rate |",
                "|---|---|---|---|---|---|---|"]
        _rows({s: n for s, n in insns.items() if phase_of[s] == ph}, rows_col=True)
    TABLE.write_text("\n".join(out) + "\n", encoding="utf-8", newline="\n")
    print(f"wrote {_rel(TABLE)}")
    return 0


def _append_row(ledger, line):
    """Append one whole line under the OS byte lock (released by the OS if this process dies, so never stale). 4.1 T6:
    was open("a"): not atomic here, 16 concurrent processes tore and lost rows (docs/ops/validation.md "4.1 Per-coder
    folder")."""
    ledger.parent.mkdir(parents=True, exist_ok=True)
    ledger.open("a").close()
    with ledger.open("r+b") as fh:
        fh.seek(LEDGER_LOCK_AT)
        for tries_left in range(6000, -1, -1):  # held for one write: 60 s means something is wrong, so say so
            try:
                msvcrt.locking(fh.fileno(), msvcrt.LK_NBLCK, 1)
                break
            except OSError:
                if not tries_left:
                    raise
                time.sleep(0.01)
        try:
            fh.seek(0, 2)
            fh.write(line.encode("utf-8"))
            fh.flush()
        finally:
            fh.seek(LEDGER_LOCK_AT)
            msvcrt.locking(fh.fileno(), msvcrt.LK_UNLCK, 1)


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--symbol")
    ap.add_argument("--task", help="T<n>, required for a run")
    ap.add_argument("--src", default=str(REPO / "src"))
    ap.add_argument("--config")
    ap.add_argument("--ledger", help="ledger file (default: a run appends to docs/matching/attempts-<phase>.jsonl; "
                    "--table reads attempts.jsonl then every attempts-<phase>.jsonl, 4.4 T11)")
    ap.add_argument("--control", metavar="FLAGS", help='build the unit with opt = FLAGS, e.g. "/O1 /Oy-"')
    ap.add_argument("--listing", action="store_true", help="also write a /FAs listing into the kept workdir")
    ap.add_argument("--workdir", metavar="NAME", help="build in <.run/phase>/match_one.NAME and wipe only that: use one "
                    "NAME per coder whenever runs may overlap (they collide in the one default folder)")
    ap.add_argument("--table", action="store_true", help="write docs/matching/attempts_by_size.md and exit")
    a = ap.parse_args(argv)
    ledger = None if a.ledger is None else Path(a.ledger) if Path(a.ledger).is_absolute() else REPO / a.ledger
    if a.table:
        return table([ledger] if ledger else ledger_paths())
    if ledger is None:
        ledger = phase_ledger()
    if not a.symbol:
        ap.error("--symbol is required (or --table)")
    rec = dict.fromkeys(KEYS)
    rec.update(ts=datetime.datetime.now(datetime.timezone.utc).isoformat(timespec="seconds"),
               phase=byte_compare._phase_run_dir().name, task=a.task, symbol=a.symbol,
               kind="control" if a.control is not None else "listing" if a.listing else "attempt", rc=2)
    try:
        if not (a.task and re.fullmatch(r"T\d+", a.task)):
            print(f"match_one: REFUSED --task T<n> required (got {a.task!r})", file=sys.stderr)
        else:
            rec["rc"] = run(a.symbol, a.src, a.config, rec, a.control, a.listing, a.workdir)
    finally:
        _append_row(ledger, json.dumps({k: rec[k] for k in KEYS}) + "\n")
    return rec["rc"]


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
