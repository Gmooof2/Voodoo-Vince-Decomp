"""Compare one delinked obj function to the retail XBE bytes (T7, phase 1.3).

CLI: byte_compare.py --obj <coff> --symbol <name> --va <hex> [--symbols T ...] [--xbe P] [--config P]
     byte_compare.py --self-check [--symbols T ...] [--xbe P] [--config P]
  Extent: the symbol's value to the next defined function symbol (class 2, type 0x20) in its section, or section end.
  Expected bytes are read from the retail XBE at compare time (sha1 == config/check.retail_xbe.sha), never cached.
  Per reloc in the extent: S = unique table VA by exact name; else, a name not FUN_-prefixed and absent from the
  tables uses its `_<8 hex>` suffix (Ghidra label); else the field is masked. A section symbol of the function's
  section = function VA - symbol value; a static symbol of that section inside the function's extent (switch $L labels,
  jump tables; 1.4 T9) = function VA + (its value - function value). P = va + (offset - value); A = in-place s32. DIR32 0x0006 field = S+A,
  REL32 0x0014 field = S+A-(P+4). The patched obj copy is compared with the XBE outside masked fields.
  Tables (default: derived/ghidra/retail-<sha8>/{functions,symbols}.jsonl): T5 jsonl or symbols.txt lines.
  Prints `<name> va 0x.. size 0x.. relocs n resolved r (via label suffix k) masked m verdict v`.
  --self-check: refuses (2.1 T2) unless derived/delink/retail-<sha8>/cut.complete is valid over hashes.txt (cut_check,
  the T1 contract); every defined function of every derived/delink/retail-<sha8>/*.obj must match with 0 masked; then
  three negative controls on scratch copies under .run/<phase>/byte_compare_selfcheck/ (deleted at end), each run as a
  subprocess: (a) REL32 symbol swap -> mismatch, (b) DIR32 symbol swap -> mismatch, (c) one REL32 target name
  removed from a table copy -> provisional masked 1.
Exit: 0 all match (self-check: known-true and 3 controls pass); 1 provisional/mismatch (self-check: any failure);
  2 refusal (missing/empty obj, table or XBE; self-check: cut.complete absent/malformed/stale; unparseable obj; sha1 != pin; symbol not defined; VA unmapped;
  reloc type other than DIR32/REL32 in the extent).
Prints names, counts, offsets and VAs only, never bytes (D46).
"""
import argparse
import hashlib
import json
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import coff_info  # noqa: E402
import xbe_info  # noqa: E402

REPO = coff_info.REPO
def _phase_run_dir():
    """.run/<phase> for the open plan's `# Phase <id>` heading (as tools/run.sh, msvc_fixtures), else .run."""
    plan = REPO / "phase-ends" / "current" / "PHASE_PLAN.md"
    if plan.is_file():
        for line in plan.read_text(encoding="utf-8").splitlines():
            m = re.match(r"# Phase (\S+)", line)
            if m:
                return REPO / ".run" / m.group(1)
    return REPO / ".run"


SCRATCH = _phase_run_dir() / "byte_compare_selfcheck"  # was REPO / ".run" / "1.3" / ... (T13, 1.3)
FUNC_CLASS, FUNC_TYPE = 2, 0x20


class Refuse(Exception):
    pass


def is_func(y):
    return y["class"] == FUNC_CLASS and y["type"] == FUNC_TYPE and y["section"] > 0


def unique_va(byname, name):
    v = byname.get(name, set())
    return next(iter(v)) if len(v) == 1 else None


def load_obj(path):
    p = Path(path)
    if not p.is_file() or p.stat().st_size == 0:
        raise Refuse(f"obj missing or empty: {p}")
    data = p.read_bytes()
    try:
        return data, coff_info.parse(data)
    except (coff_info.BadCoff, struct.error, ValueError, IndexError) as e:
        raise Refuse(f"unparseable obj {p}: {e}")


def resolve(c, sym, fsym, va, byname):
    """-> (S or None, via_suffix)."""
    secname = {s["number"]: s["name"] for s in c["sections"]}
    if sym["class"] == coff_info.SC_STATIC and sym["section"] > 0 and sym["value"] == 0 \
            and sym["name"] == secname.get(sym["section"]):
        return (va - fsym["value"] if sym["section"] == fsym["section"] else None), False
    if sym["class"] == coff_info.SC_STATIC and sym["section"] == fsym["section"] and sym["name"] not in byname:
        _sec, lo, hi = extent(c, fsym)  # T9 (1.4): a static label inside the function's own extent ($L case labels,
        if lo <= sym["value"] < hi:     # jump tables) sits at the same offset in retail
            return va - lo + sym["value"], False
    S = unique_va(byname, sym["name"])
    if S is not None:
        return S, False
    if not sym["name"].startswith("FUN_") and sym["name"] not in byname:
        m = coff_info.DYN.search(sym["name"])
        if m:
            return int(m.group(1), 16), True
    return None, False


def extent(c, fsym):
    sec = c["sections"][fsym["section"] - 1]
    nxt = [y["value"] for y in c["symbols"].values()
           if is_func(y) and y["section"] == fsym["section"] and y["value"] > fsym["value"]]
    return sec, fsym["value"], min(nxt) if nxt else sec["size"]


def compare(obj, symbol, va, byname, xb, x, want_size=None):
    """want_size (3.5 T4 F2): the listed size; the extent must equal it. The symbol must be function-typed
    (was: cands[0] of any type, extent unchecked: a const data array of the retail bytes matched)."""
    data, c = load_obj(obj)
    cands = [y for y in c["symbols"].values() if y["name"] == symbol and y["section"] > 0]
    cands.sort(key=lambda y: not is_func(y))
    if not cands or cands[0]["section"] > len(c["sections"]):
        raise Refuse(f"symbol {symbol} not defined in {obj}")
    fsym = cands[0]
    if not is_func(fsym):
        raise Refuse(f"symbol {symbol} in {obj} is not a function (class {fsym['class']} type 0x{fsym['type']:X})")
    sec, lo, hi = extent(c, fsym)
    size = hi - lo
    if size <= 0 or sec["rawPtr"] + hi > len(data):
        raise Refuse(f"unparseable obj {obj}: extent of {symbol} outside the section data")
    if want_size is not None and size != want_size:
        raise Refuse(f"extent of {symbol} 0x{size:X} != listed size 0x{want_size:X}")
    off, last = xbe_info.va_to_off(x, va), xbe_info.va_to_off(x, va + size - 1)
    if off is None or last is None or last - off != size - 1:
        raise Refuse(f"VA 0x{va:08X} size 0x{size:X} not mapped contiguously in the XBE")
    got = bytearray(data[sec["rawPtr"] + lo:sec["rawPtr"] + hi])
    masked, nrel, nres, viadyn = set(), 0, 0, 0
    for r in sec["relocs"]:
        if not lo <= r["offset"] < hi:
            continue
        sym = c["symbols"][r["symbol"]]
        if r["type"] not in coff_info.TYPE_NAMES:
            raise Refuse(f"reloc type 0x{r['type']:04X} at +0x{r['offset'] - lo:X} -> {sym['name']} "
                         f"is not DIR32 0x0006 / REL32 0x0014")
        nrel += 1
        rel = r["offset"] - lo
        S, dyn = resolve(c, sym, fsym, va, byname)
        if S is None or rel + 4 > size:
            masked |= set(range(rel, min(rel + 4, size)))
            continue
        nres += 1
        viadyn += dyn
        A = struct.unpack_from("<i", data, sec["rawPtr"] + r["offset"])[0]
        P = va + rel
        field = S + A if r["type"] == coff_info.DIR32 else S + A - (P + 4)
        struct.pack_into("<I", got, rel, field & 0xFFFFFFFF)
    want = xb[off:off + size]
    diff = [i for i in range(size) if i not in masked and got[i] != want[i]]
    nmask = nrel - nres
    verdict = "mismatch" if diff else ("provisional" if nmask else "match")
    print(f"{symbol} va 0x{va:08X} size 0x{size:X} relocs {nrel} resolved {nres} (via label suffix {viadyn}) "
          f"masked {nmask} verdict {verdict}")
    if diff:
        print(f"  differing bytes {len(diff)}, first at +0x{diff[0]:X}")
    return verdict, nmask, nres


def setup(a):
    """-> (pin, byname, table paths, xbe path, xbe bytes, parsed xbe); raises Refuse."""
    pin = coff_info.pinned_sha()
    if not pin:
        raise Refuse(f"no retail_xbe pin in {coff_info.PIN}")
    t5 = REPO / "derived" / "ghidra" / f"retail-{pin[:8]}"
    if (t5 / "applied" / "summary.json").is_file():  # names of the post-ApplySymbols project the objects came from
        t5 = t5 / "applied"
    tables = [Path(t) for t in a.symbols] if a.symbols else [t5 / "functions.jsonl", t5 / "symbols.jsonl"]
    for t in tables:
        if not t.is_file() or t.stat().st_size == 0:
            raise Refuse(f"table missing or empty: {t}")
    try:
        byname, counts = coff_info.load_names(tables)
    except (ValueError, KeyError, IndexError, TypeError) as e:
        raise Refuse(f"unparseable table: {e}")
    for t, n in zip(tables, counts):
        if n == 0:
            raise Refuse(f"table has no rows: {t}")
    if a.xbe:
        xp = Path(a.xbe)
    else:
        xp = coff_info.vv_inputs.input_path("retail_xbe", cfg=coff_info.vv_inputs.load_config(a.config))
    if not xp.is_file() or xp.stat().st_size == 0:
        raise Refuse(f"XBE missing or empty: {xp}")
    xb = xp.read_bytes()
    got = hashlib.sha1(xb).hexdigest()
    if got != pin:
        raise Refuse(f"XBE sha1 {got} != pinned {pin}")
    try:
        x = xbe_info.parse_xbe(xb)
    except xbe_info.NotXbe as e:
        raise Refuse(f"XBE unparseable: {e}")
    return pin, byname, tables, xp, xb, x


def reloc_field_pos(data, secnum, r):
    """File offset of reloc entry r of section secnum (1-based)."""
    optsz = struct.unpack_from("<H", data, 16)[0]
    relptr = struct.unpack_from("<I", data, 20 + optsz + 40 * (secnum - 1) + 24)[0]
    return relptr + 10 * r


def run_cli(args):
    p = subprocess.run([sys.executable, str(Path(__file__).resolve())] + args, capture_output=True, text=True)
    v = re.search(r"verdict (\w+)", p.stdout)
    m = re.search(r"masked (\d+) verdict", p.stdout)
    return p.returncode, (v.group(1) if v else None), (int(m.group(1)) if m else None)


def controls(obj, fname, va, byname, tables, xp):
    data, c = load_obj(obj)
    fsym = next(y for y in c["symbols"].values() if y["name"] == fname and is_func(y))
    sec, lo, hi = extent(c, fsym)
    inext = [(k, r) for k, r in enumerate(sec["relocs"]) if lo <= r["offset"] < hi]
    tabled = sorted((i, unique_va(byname, y["name"])) for i, y in c["symbols"].items()
                    if unique_va(byname, y["name"]) is not None)
    base = ["--symbol", fname, "--va", f"0x{va:08X}", "--xbe", str(xp)]
    SCRATCH.mkdir(parents=True, exist_ok=True)
    results = []
    for tag, typ in (("a", coff_info.REL32), ("b", coff_info.DIR32)):
        pick = None
        for k, r in inext:
            if r["type"] != typ:
                continue
            S, _d = resolve(c, c["symbols"][r["symbol"]], fsym, va, byname)
            alt = next((i for i, v in tabled if S is not None and v != S), None)
            if alt is not None:
                pick = (k, r, alt)
                break
        if not pick:
            print(f"control {tag}: FAIL no {coff_info.TYPE_NAMES[typ]} reloc with a table-resolved substitute")
            results.append(False)
            continue
        k, r, alt = pick
        buf = bytearray(data)
        struct.pack_into("<I", buf, reloc_field_pos(data, sec["number"], k) + 4, alt)
        cp = SCRATCH / f"control_{tag}.obj"
        cp.write_bytes(bytes(buf))
        rc, v, m = run_cli(["--obj", str(cp)] + base + ["--symbols"] + [str(t) for t in tables])
        ok = rc == 1 and v == "mismatch"
        print(f"control {tag}: {coff_info.TYPE_NAMES[typ]} at +0x{r['offset'] - lo:X} "
              f"{c['symbols'][r['symbol']]['name']} -> {c['symbols'][alt]['name']}: rc {rc} verdict {v} "
              f"-> {'pass' if ok else 'FAIL'}")
        results.append(ok)
    # (c): drop one table-resolved REL32 target (referenced once, would not fall back to a suffix) from table copies
    refs = {}
    for _k, r in inext:
        refs.setdefault(c["symbols"][r["symbol"]]["name"], []).append(r)
    cands = [n for n, rs in refs.items() if len(rs) == 1 and rs[0]["type"] == coff_info.REL32
             and unique_va(byname, n) is not None and (n.startswith("FUN_") or not coff_info.DYN.search(n))]
    cands.sort(key=lambda n: (n.startswith("FUN_"), refs[n][0]["offset"]))
    if not cands:
        print("control c: FAIL no REL32 target suitable for removal")
        results.append(False)
    else:
        name, newt = cands[0], []
        for t in tables:
            out = SCRATCH / f"control_c_{t.name}"
            keep = []
            for line in t.read_text(encoding="utf-8").splitlines(keepends=True):
                if line.strip():
                    if t.name.endswith(".jsonl"):
                        row = json.loads(line)
                        if name in (row.get("name"), f"{row.get('namespace')}::{row.get('name')}"):
                            continue
                    elif not line.lstrip().startswith("#") and line.split()[3] == name:
                        continue
                keep.append(line)
            out.write_text("".join(keep), encoding="utf-8")
            newt.append(str(out))
        rc, v, m = run_cli(["--obj", str(obj)] + base + ["--symbols"] + newt)
        ok = rc == 1 and v == "provisional" and m == 1
        print(f"control c: table copy without {name} (REL32 at +0x{refs[name][0]['offset'] - lo:X}): rc {rc} "
              f"verdict {v} masked {m} -> {'pass' if ok else 'FAIL'}")
        results.append(ok)
    return results


CUT_MARKER = "cut.complete"  # 2.1 T2: the T1 contract (tools/delink.py cut_check), inline: a locked file imports no unlocked one
MARKER_RE = re.compile(rb"cut-complete v1 hashes-sha256 ([0-9a-f]{64}) objects ([0-9]+)\n")


def cut_dir(pin):
    """The cut for a pinned retail sha1: derived/delink/retail-<pin[:8]>."""
    return REPO / "derived" / "delink" / f"retail-{pin[:8]}"


def cut_check(d):
    """None when d/cut.complete is valid (exact line, sha256 of hashes.txt bytes, objects == its line count),
    else the refusal reason naming the marker path."""
    m = Path(d) / CUT_MARKER
    if not m.is_file():
        return f"{m} absent: cut incomplete or stale (run PY tools/delink.py --fresh)"
    g = MARKER_RE.fullmatch(m.read_bytes())
    if not g:
        return f"{m} malformed (want one line 'cut-complete v1 hashes-sha256 <hex64> objects <N>')"
    h = Path(d) / "hashes.txt"
    if not h.is_file():
        return f"{m} present but {h} absent"
    data = h.read_bytes()
    if hashlib.sha256(data).hexdigest() != g.group(1).decode("ascii"):
        return f"{m} hashes-sha256 != sha256 of {h} (cut changed after --fresh)"
    if int(g.group(2)) != len(data.splitlines()):
        return f"{m} objects {int(g.group(2))} != {len(data.splitlines())} lines of {h}"
    return None


def self_check(a):
    pin, byname, tables, xp, xb, x = setup(a)
    d = cut_dir(pin)
    why = cut_check(d)  # 2.1 T2: a partial or stale cut is refused before hashes.txt is read
    if why:
        raise Refuse(why)
    h = d / "hashes.txt"  # the delink set; a `delink.py one` output beside it (e.g. 0x00011800.obj) is not in it
    if not h.is_file():
        raise Refuse(f"missing {h}")
    names = sorted(p[1] for p in (ln.split() for ln in h.read_text(encoding="ascii").splitlines()) if len(p) == 2)
    objs = [d / n for n in names]
    if not objs:
        raise Refuse(f"no obj named in {h}")
    gone = [o.name for o in objs if not o.is_file()]
    if gone:
        raise Refuse(f"{len(gone)} obj(s) named in {h} missing, first {gone[0]}")
    total, good, first = 0, 0, None
    seen = {"provisional": [], "mismatch": [], "no table VA": []}
    nres_all, nmask_all = 0, 0
    for obj in objs:
        _data, c = load_obj(obj)
        for y in c["symbols"].values():
            if not is_func(y):
                continue
            total += 1
            va = unique_va(byname, y["name"])
            if va is None:
                print(f"{y['name']} in {obj.name}: no unique table VA -> FAIL")
                seen["no table VA"].append(f"{y['name']} ({obj.name})")
                continue
            v, m, r = compare(obj, y["name"], va, byname, xb, x)
            nres_all, nmask_all = nres_all + r, nmask_all + m
            if v in seen:
                seen[v].append(f"{y['name']} ({obj.name})")
            if v == "match" and m == 0:
                good += 1
                first = first or (obj, y["name"], va)
    try:
        res = controls(*first, byname, tables, xp) if first else [False, False, False]
    finally:
        shutil.rmtree(SCRATCH, ignore_errors=True)
    for k, names in seen.items():
        for n in names:
            print(f"{k}: {n}")
    print(f"self-check: objs {len(objs)} known-true {good}/{total} match masked 0; controls passing {sum(res)}/3; "
          f"totals functions {total} match {good} provisional {len(seen['provisional'])} "
          f"mismatch {len(seen['mismatch'])} no-table-VA {len(seen['no table VA'])} "
          f"relocs resolved {nres_all} masked {nmask_all}")
    return 0 if total and good == total and all(res) and len(res) == 3 else 1


def main(argv):
    ap = argparse.ArgumentParser(prog="byte_compare.py", description=__doc__.splitlines()[0])
    ap.add_argument("--obj")
    ap.add_argument("--symbol")
    ap.add_argument("--va")
    ap.add_argument("--symbols", nargs="+", help="name tables (T5 jsonl or symbols.txt); default the T5 exports")
    ap.add_argument("--xbe", help="retail XBE path (default: config retail_xbe)")
    ap.add_argument("--config", help="config path (default config/local_paths.json)")
    ap.add_argument("--self-check", action="store_true")
    a = ap.parse_args(argv)
    try:
        if a.self_check:
            return self_check(a)
        if not (a.obj and a.symbol and a.va):
            ap.error("--obj, --symbol and --va are required (or --self-check)")
        try:
            va = int(a.va, 16)
        except ValueError:
            raise Refuse(f"--va {a.va} is not hex")
        _pin, byname, _t, _xp, xb, x = setup(a)
        v, _m, _r = compare(a.obj, a.symbol, va, byname, xb, x)
        return 0 if v == "match" else 1
    except Refuse as e:
        print(f"byte_compare: REFUSED {e}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
