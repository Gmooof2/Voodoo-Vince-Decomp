"""i386 COFF object inspector (T6, phase 1.3).

CLI: coff_info.py <obj> [--check-xbe] [--config P]
  Parses header, sections, symbols (long names via the string table) and relocations; prints counts, offsets,
  types and names, never bytes.
  --check-xbe: maps every reloc site to its retail VA (section VA = T5 VA of a defined symbol in that section minus
  its value; T5 = derived/ghidra/retail-<sha8>/{functions,symbols}.jsonl), recovers the target from the retail XBE
  field (DIR32 u32; REL32 P+4+disp) and compares it with S + A (S = the reloc symbol's T5 VA by name, a section
  symbol uses its section's VA; a name absent from T5 (Ghidra dynamic DAT_/PTR_/s_ labels, not exported) uses
  its _<8 hex> suffix, counted separately; A = the in-place addend in the obj field). Then proto_map.mask_proc over each
  function body lists reloc sites that are not the start of a decoded 4-byte field.
Exit: 0 ok; 1 a parse error, or (--check-xbe) any mismatch, unresolved symbol, type other than DIR32 0x0006 /
  REL32 0x0014, or off-field site; 2 refusal (retail sha1 != config/check.retail_xbe.sha, T5 export missing).
Prints counts, offsets, VAs, types and names only (D46).
"""
import argparse
import hashlib
import json
import re
import struct
import sys
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))
import vv_inputs  # noqa: E402

PIN = REPO / "config" / "check.retail_xbe.sha"
I386 = 0x14C
DIR32, REL32 = 0x0006, 0x0014
TYPE_NAMES = {DIR32: "DIR32", REL32: "REL32"}
SC_STATIC = 3
DYN = re.compile(r"^[A-Za-z]+_.*?([0-9a-fA-F]{8})$")


class BadCoff(Exception):
    pass


def _name(raw, strtab):
    if raw[:4] == b"\0\0\0\0":
        off = struct.unpack_from("<I", raw, 4)[0]
        end = strtab.find(b"\0", off)
        if off < 4 or end < 0:
            raise BadCoff(f"string table offset {off} out of range")
        return strtab[off:end].decode("latin-1")
    return raw.rstrip(b"\0").decode("latin-1")


def parse(data):
    """Parse an i386 COFF object -> {machine, sections, symbols}; raises BadCoff."""
    if len(data) < 20:
        raise BadCoff("shorter than the 20-byte file header")
    machine, nsec, _ts, symptr, nsym, optsz, chars = struct.unpack_from("<HHIIIHH", data, 0)
    if machine != I386:
        raise BadCoff(f"machine 0x{machine:04X} is not i386 0x014C")
    strpos = symptr + 18 * nsym
    if symptr == 0 or strpos + 4 > len(data):
        raise BadCoff("symbol table / string table outside the file")
    strsz = struct.unpack_from("<I", data, strpos)[0]
    strtab = data[strpos:strpos + strsz]
    syms, i = {}, 0
    while i < nsym:
        o = symptr + 18 * i
        val, sec, typ, scl, naux = struct.unpack_from("<IhHBB", data, o + 8)
        syms[i] = {"index": i, "name": _name(data[o:o + 8], strtab), "value": val, "section": sec,
                   "type": typ, "class": scl, "aux": naux}
        i += 1 + naux
    secs = []
    for k in range(nsec):
        o = 20 + optsz + 40 * k
        if o + 40 > len(data):
            raise BadCoff(f"section header {k + 1} outside the file")
        raw = data[o:o + 8]
        name = _name(raw, strtab) if raw[:1] != b"/" else strtab[int(raw[1:].rstrip(b"\0"))
                                                                  :].split(b"\0")[0].decode("latin-1")
        vsz, va, rsz, rptr, relptr, _ln, nrel, _nl, sch = struct.unpack_from("<IIIIIIHHI", data, o + 8)
        relocs = []
        for r in range(nrel):
            ro = relptr + 10 * r
            if ro + 10 > len(data):
                raise BadCoff(f"relocation {r} of section {k + 1} outside the file")
            vaddr, si, rt = struct.unpack_from("<IIH", data, ro)
            if si not in syms:
                raise BadCoff(f"relocation {r} of section {k + 1} names symbol index {si} not in the table")
            if vaddr + 4 > rsz:
                raise BadCoff(f"relocation {r} of section {k + 1} at 0x{vaddr:X} outside raw size 0x{rsz:X}")
            relocs.append({"offset": vaddr, "symbol": si, "type": rt})
        secs.append({"number": k + 1, "name": name, "size": rsz, "rawPtr": rptr, "chars": sch, "relocs": relocs})
    return {"machine": machine, "chars": chars, "sections": secs, "symbols": syms}


def show(c):
    print(f"machine 0x{c['machine']:04X} sections {len(c['sections'])} symbols {len(c['symbols'])}")
    for s in c["sections"]:
        print(f"  section {s['number']} {s['name']} size 0x{s['size']:X} chars 0x{s['chars']:08X} "
              f"relocs {len(s['relocs'])}")
    for y in c["symbols"].values():
        print(f"  sym {y['index']} {y['name']} sec {y['section']} value 0x{y['value']:X} type 0x{y['type']:X} "
              f"class {y['class']}")
    for s in c["sections"]:
        for r in s["relocs"]:
            print(f"  reloc {s['name']}+0x{r['offset']:X} type 0x{r['type']:04X} "
                  f"{TYPE_NAMES.get(r['type'], '?')} -> {c['symbols'][r['symbol']]['name']}")


def pinned_sha():
    """Pinned retail XBE sha1 (lowercase hex) from config/check.retail_xbe.sha, or None."""
    want = None
    for line in PIN.read_text(encoding="ascii").splitlines():
        p = line.split()
        if len(p) == 2 and p[1] == "retail_xbe":
            want = p[0].lower()
    return want


def load_names(paths):
    """name -> set of VAs from T5 jsonl rows {entry|va, name, namespace?} (also `namespace::name`) or symbols.txt
    lines `<va> <func|data> <size|-> <name> <source> <status>` (# comments). Returns (byname, rows per path)."""
    byname, counts = {}, []
    for path in paths:
        n = 0
        with open(path, encoding="utf-8") as f:
            for line in f:
                if not line.strip():
                    continue
                if str(path).endswith(".jsonl"):
                    r = json.loads(line)
                    va = int(r.get("va") or r.get("entry"), 16)
                    byname.setdefault(r["name"], set()).add(va)
                    if r.get("namespace"):
                        byname.setdefault(f"{r['namespace']}::{r['name']}", set()).add(va)
                else:
                    if line.lstrip().startswith("#"):
                        continue
                    p = line.split()
                    byname.setdefault(p[3], set()).add(int(p[0], 16))
                n += 1
        counts.append(n)
    return byname, counts


def check_xbe(c, data, cfg):
    import proto_map
    import xbe_info
    want = pinned_sha()
    xbe_path = vv_inputs.input_path("retail_xbe", cfg=cfg)
    xb = xbe_path.read_bytes()
    got = hashlib.sha1(xb).hexdigest()
    if not want or got != want:
        print(f"coff_info: retail_xbe sha1 {got} != pinned {want}", file=sys.stderr)
        return 2
    t5 = REPO / "derived" / "ghidra" / f"retail-{want[:8]}"
    if not (t5 / "summary.json").is_file():
        print(f"coff_info: T5 export missing: {t5} (run tools/delink.py ingest)", file=sys.stderr)
        return 2
    if (t5 / "applied" / "summary.json").is_file():  # names of the post-ApplySymbols project the objects came from
        t5 = t5 / "applied"
    byname, _n = load_names([t5 / "symbols.jsonl", t5 / "functions.jsonl"])
    funcs = {}
    with open(t5 / "functions.jsonl", encoding="utf-8") as f:
        for line in f:
            if line.strip():
                r = json.loads(line)
                funcs[int(r["entry"], 16)] = (int(r["bodyMin"], 16), int(r["bodyMax"], 16))
    x = xbe_info.parse_xbe(xb)
    lo = x["header"]["baseAddr"]
    hi = lo + x["header"]["sizeOfImage"]

    def one_va(name):
        v = byname.get(name, set())
        return next(iter(v)) if len(v) == 1 else None

    def dyn_va(name):
        """Ghidra dynamic symbols (DAT_/PTR_/s_..., absent from the T5 export) end in _<8 hex digits> = their VA."""
        m = DYN.search(name)
        return int(m.group(1), 16) if m and name not in byname else None

    secname = {s["number"]: s["name"] for s in c["sections"]}

    def is_secsym(y):
        return y["class"] == SC_STATIC and y["section"] > 0 and y["value"] == 0 and y["name"] == secname.get(y["section"])

    secva = {}
    for y in c["symbols"].values():
        if y["section"] > 0 and not is_secsym(y):
            v = one_va(y["name"])
            if v is not None:
                secva.setdefault(y["section"], v - y["value"])
    types, agree, mismatch, unres, refused, offfield, viadyn = Counter(), 0, 0, 0, 0, 0, 0
    for s in c["sections"]:
        if not s["relocs"]:
            continue
        base = secva.get(s["number"])
        starts, fn_ranges = set(), []
        if base is not None:
            for e, (bmin, bmax) in funcs.items():
                if base <= e < base + s["size"]:
                    off = xbe_info.va_to_off(x, bmin)
                    buf = xb[off:off + bmax - bmin + 1]
                    masks, _err = proto_map.mask_proc(buf, bmin, lo, hi)
                    starts |= {bmin + o for o, n, _k in masks if n == 4}
                    fn_ranges.append((bmin, bmax))
        for r in s["relocs"]:
            t = r["type"]
            types[t] += 1
            sym = c["symbols"][r["symbol"]]
            where = f"{s['name']}+0x{r['offset']:X} type 0x{t:04X} -> {sym['name']}"
            if t not in TYPE_NAMES:
                refused += 1
                print(f"  REFUSED type: {where}")
                continue
            if base is None:
                unres += 1
                print(f"  unresolved (no VA for section {s['name']}): {where}")
                continue
            P = base + r["offset"]
            if is_secsym(sym):
                S = secva.get(sym["section"])
            else:
                S = one_va(sym["name"])
                if S is None:
                    S = dyn_va(sym["name"])
                    viadyn += S is not None
            if S is None:
                unres += 1
                print(f"  unresolved (no unique VA for symbol): {where}")
                continue
            A = struct.unpack_from("<i", data, s["rawPtr"] + r["offset"])[0]
            xo = xbe_info.va_to_off(x, P)
            if xo is None:
                unres += 1
                print(f"  unresolved (site 0x{P:08X} not in the XBE): {where}")
                continue
            field = struct.unpack_from("<I", xb, xo)[0]
            if t == DIR32:
                ok, target = field == (S + A) & 0xFFFFFFFF, field
            else:
                ok, target = field == (S + A - (P + 4)) & 0xFFFFFFFF, (P + 4 + field) & 0xFFFFFFFF
            if ok:
                agree += 1
            else:
                mismatch += 1
                print(f"  MISMATCH at 0x{P:08X}: xbe target 0x{target:08X}, S+A 0x{(S + A) & 0xFFFFFFFF:08X}: {where}")
            if P not in starts:
                offfield += 1
                inside = any(a <= P <= b for a, b in fn_ranges)
                print(f"  off-field site 0x{P:08X} ({'inside' if inside else 'outside'} a T5 body): {where}")
    print("reloc types: " + ", ".join(f"0x{k:04X} {TYPE_NAMES.get(k, '?')} {v}" for k, v in sorted(types.items())))
    print(f"check-xbe: agree {agree} mismatch {mismatch} unresolved {unres} refused {refused} "
          f"of {sum(types.values())} (S via Ghidra dynamic-name suffix {viadyn}); off-field sites {offfield}")
    return 1 if mismatch or unres or refused or offfield else 0


def main(argv):
    ap = argparse.ArgumentParser(prog="coff_info.py", description=__doc__.splitlines()[0])
    ap.add_argument("obj")
    ap.add_argument("--check-xbe", action="store_true", help="verify every reloc against the retail XBE")
    ap.add_argument("--config", help="config path (default config/local_paths.json)")
    a = ap.parse_args(argv)
    data = Path(a.obj).read_bytes()
    try:
        c = parse(data)
    except (BadCoff, struct.error, ValueError) as e:
        print(f"coff_info: FAIL unparseable COFF: {e}", file=sys.stderr)
        return 1
    show(c)
    if a.check_xbe:
        return check_xbe(c, data, vv_inputs.load_config(a.config))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
