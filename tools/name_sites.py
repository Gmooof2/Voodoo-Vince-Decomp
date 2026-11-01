"""Name unresolved reloc targets of pool rows by prototype site correspondence (T5, phase 3.1).

Usage: PY tools/name_sites.py [--unit U] [--apply] [--doc] [--rows P] [--control positive|planted] | --check-cut
       | --near [--apply] | --control near-positive|near-planted --unit U
Rows: pool rows (derived/candidates/candidates.jsonl minus config/matched_set.toml), all or one unit.
Per row: identity re-verified (prototype default.exe bytes at protoVA vs retail XBE bytes at va, `size` bytes,
  proto_map.mask_proc each side: same (off, kind) mask list, equal unmasked bytes), else its sites -> proto-mismatch.
  A retail abs mask where the prototype has no mask and no .reloc entry and the raw dwords are equal is a constant
  (T6): the row stays identical, that site -> residue `constant`.
Per unresolved site: retail target (rel va+off+4+rel32 | abs dword, asserted == the jsonl target); proto target the same
  way at protoVA+off; name = the PDB public(s) at exactly the proto target. Kind: ??_7 vtable, ??_C@ string, __real@
  float, proto target in an exec section func, else data. Byte check per target -> evidence checked | site-only.
      Callee check (func, vtable slots): masked-equal plus named outgoing masks agree (callee-name-conflict) and a
      < 8 unmasked-byte callee needs one agreeing mask (callee-short); downgrades counted in the dry run.
  section-mismatch: proto and retail target section names differ (XBE flags every section EXECUTABLE; names map).
Residue (never pinned): no-public, multi-public (D44), va-two-names, name-two-vas, named-differently, inside-symbol,
  section-mismatch, proto-mismatch, constant; consistency reasons always over all pool rows.
Second pass (T6, always, all rows): each unnamed non-constant residue target blocking >= 3 pool rows gets (a) caller-xref
  (C0059) over proto_map identical + near-identical rows with the same length and mask list, (b) cut-sym (C0063),
  (c) count of ??_C@ targets in its retail body. One public at one proto target + the T5 gates and byte check -> pin,
  source proto-pdb-public+caller-xref[+check]; else the residue entries carry each route's result (`evidence`).
--doc writes docs/matching/naming_residue.md (no bytes; ??_C@ names and string labels masked, D46).
--check-cut: every pool site whose target is in symbols_pin.txt, resolved or not, has a cut relocation at its offset naming
  the pin (3.2 T4); every other unresolved site's target is in residue.jsonl; exit 0 all hold, 1 FAIL lines.
Outputs (gitignored): derived/name_sites/{names.jsonl,residue.jsonl,pins.txt}; addresses, names, counts only (D46).
  residue.jsonl always holds every pool row's residue, --unit included (3.4 T1, C0139); names/pins follow --unit.
--apply appends pins.txt to config/retail/symbols_pin.txt (env NAME_SITES_SYMS_PIN overrides that path, scratch tests).
--control positive: derived name == the proto-pdb* name already at the retail target, for every site; planted: one
  row's protoVA moved +0x10 in memory must be refused proto-mismatch with nothing derived from it. No outputs written.
--promote-data [--unit U] [--apply] (3.3 T8): data byte route over every `data` pin of symbols_pin.txt with status site-corr
  (--unit: pins that are a site target of a pool row of U); first failing step gives the reason. a. the one PDB public
  named N (else proto-name). b. ??_C@/__real@: the 3.1 check (literal-differs/real-differs); ??_7: size = 3.1 vtable
  extent, every slot a pointer field (vtable-no-slots); else the unique pdb_types.tsize of the S_GDATA32/S_LDATA32 (_ST)
  record at the public's VA whose undecorated name is N (global stream, else module streams; no-type, type-size).
  c. proto bytes past raw: no-proto-bytes; retail: bss. d. pointer fields = proto .reloc offsets: pubs[proto dword] must
  meet the accepted names at the retail dword (symbols.txt units_seed.STATUSES, pins candidates.PIN_STATUSES, pins
  promoted this run; fixed point, VA order): pointer-unnamed, pointer-unchecked, pointer-differs. e. other bytes equal
  (bytes-differ). f. a non-zero non-pointer byte or an agreeing pointer (zero-bytes). Writes derived/name_sites/
  promote.jsonl; --apply rewrites passing lines in place (by VA + data + site-corr) to source
  proto-pdb-public+site-corr+data-bytes, status disasm-verified, plus one `# 3.3 T8` comment line.
--control planted-data: first passing pin's VA with a later passing pin's name whose bytes differ there must fail the
  route and rewrite 0 lines through the real --apply path (pins already promoted by the route count as candidates). --doc adds "Data pins kept site-corr (3.3 T8)".
--near [--apply] (3.4 T2): every proto_map near-identical row (candidateVA, similarity >= THRESHOLD) through near_check
  (1 one public N, proto exec func; 2 retail VA inside the row's LIS-anchor gap window, own anchor removed; 3 proto_map
  token similarity >= THRESHOLD, drift vs the row > 0.01 refused; 4 caller-xref C0059 over accepted, img.match()ed
  table callers: >= 1 agreeing site, no conflicting one), then fold, va-two-rows, already / named-differently,
  name-two-vas, inside-symbol, Ghidra extent (no-ghidra-function, ghidra-noncontiguous, extent-overlap). Writes
  derived/name_sites/{near.jsonl,near_pins.txt}; --apply appends near_pins.txt (status near-corr) to symbols_pin.txt
  and rewrites in place (`upgrade`) the one unsized site-corr func line of an admitted VA with the same name.
  A candidates row with lane "near" has no identity in derive() (sites -> proto-mismatch); callee() agreement ignores
  near-corr names. --control positive excludes callee-moved disagreements (proto site target an identical row whose
  one target differs from the retail target), printed and counted.
--control near-positive --unit U: identical candidates.jsonl rows of U (done included) through near_check: named (own
  symbol), no-caller (no-xref, abstain), wrong (else); exit 0 iff wrong 0 and named >= 10 (judgeable rows only); near-planted --unit U: swapped-name pair of U's first two passing near rows and the first decoy-window
  pair (proto_map DECOY_SHIFT gap, best_candidate >= THRESHOLD) must both fail.
Exit: 0 ok; 1 control or --check-cut failed; 2 refusal (missing input, retail sha mismatch, bad args, --apply VA already
  named, --promote-data with --control/--check-cut/--doc, --promote-data --apply without one data site-corr line per VA).
"""
import argparse
import bisect
import json
import os
import struct
import sys
import tomllib
from collections import Counter, defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

import byte_compare  # noqa: E402
import pdb_info  # noqa: E402
import pe_info  # noqa: E402
import proto_map  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

CAND = REPO / "derived" / "candidates" / "candidates.jsonl"
FUNCS = REPO / "derived" / "proto_map" / "retail-xbe" / "functions.jsonl"
MATCHED = REPO / "config" / "matched_set.toml"
SYMS = REPO / "config" / "retail" / "symbols.txt"
SYMS_PIN = Path(os.environ.get("NAME_SITES_SYMS_PIN") or REPO / "config" / "retail" / "symbols_pin.txt")
OUT = REPO / "derived" / "name_sites"
DOC = REPO / "docs" / "matching" / "naming_residue.md"
DUP_DOC = REPO / "docs" / "matching" / "dup_multi_public.md"  # 3.3 T9: D44 report on the multi-public residue
FOLDS = REPO / "derived" / "dup_report" / "retail_folds.jsonl"
MIN_ROWS = 3  # second pass: residue targets blocking at least this many distinct pool rows
REASONS = ("proto-mismatch", "constant", "no-public", "multi-public", "inside-symbol", "section-mismatch",
           "va-two-names", "name-two-vas", "named-differently")
KINDS = ("func", "vtable", "string", "float", "data")
SRC_CHECK = {"func": "callee-bytes", "vtable": "vtable-slots", "string": "literal-bytes", "float": "real-bytes"}
PE_EXEC = 0x20000000 | 0x20  # IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_CODE
VT_CAP, STR_CAP = 64, 0x10000


class Refuse(Exception):
    pass


def load_jsonl(p):
    return [json.loads(ln) for ln in p.read_text(encoding="utf-8").splitlines() if ln.strip()]


def need(p):
    if not p.is_file() or p.stat().st_size == 0:
        raise Refuse(f"input missing or empty: {p}")
    return p


NEAR_CORR = set()  # 3.4 T2: (va, name) of table lines with status near-corr; callee() agreement ignores them


def load_table():
    """[(va, kind, size|None, name, source)] from symbols.txt then symbols_pin.txt (candidates.load_symbols split).
    Fills NEAR_CORR with the (va, name) of near-corr status lines (3.4 T2)."""
    out = []
    NEAR_CORR.clear()
    for tab in (SYMS, SYMS_PIN):
        for ln in need(tab).read_text(encoding="utf-8").splitlines():
            if not ln.strip() or ln.startswith("#"):
                continue
            f = ln.split()
            out.append((int(f[0], 16), f[1], None if f[2] == "-" else int(f[2], 16), " ".join(f[3:-2]), f[-2]))
            if f[-1] == "near-corr":
                NEAR_CORR.add((out[-1][0], out[-1][3]))
    return out


class Images:
    """Prototype PE and retail XBE byte access, sections, exec flags, mask-equal primitive."""

    def __init__(self, exe, pe, xb, x):
        self.exe, self.xb, self.x = exe, xb, x
        self.base = pe["imageBase"]
        e = struct.unpack_from("<I", exe, 0x3C)[0]
        sh = e + 24 + struct.unpack_from("<H", exe, e + 20)[0]
        self.psecs = [dict(s, exec=bool(struct.unpack_from("<I", exe, sh + 40 * i + 36)[0] & PE_EXEC))
                      for i, s in enumerate(pe["sections"])]
        size_img, rel_rva, rel_sz = proto_map._pe_extra(exe)
        self.plo, self.phi = self.base, self.base + size_img
        ro = proto_map._rva_off(pe, rel_rva) if rel_rva and rel_sz else None
        self.prelocs = proto_map.parse_reloc_blocks(exe[ro:ro + rel_sz], self.base) if ro is not None else set()
        self.rlo = x["header"]["baseAddr"]
        self.rhi = self.rlo + x["header"]["sizeOfImage"]

    def psec(self, va):
        for s in self.psecs:
            if self.base + s["va"] <= va < self.base + s["va"] + max(s["vsize"], s["rawsize"]):
                return s
        return None

    def rsec(self, va):
        for s in self.x["sections"]:
            if s["virtualAddr"] <= va < s["virtualAddr"] + max(s["virtualSize"], s["rawSize"]):
                return s
        return None

    def pexec(self, va):
        s = self.psec(va)
        return bool(s and s["exec"])

    def pbytes(self, va, n):
        s = self.psec(va)
        if s is None:
            return None
        o = va - self.base - s["va"]
        if o + n > s["rawsize"]:
            return None
        return self.exe[s["rawptr"] + o:s["rawptr"] + o + n]

    def rbytes(self, va, n):
        s = self.rsec(va)
        if s is None or va - s["virtualAddr"] + n > s["rawSize"]:
            return None
        o = xbe_info.va_to_off(self.x, va)
        return self.xb[o:o + n]

    def match(self, pva, rva, n, allow_const=True, need_bytes=True):
        """-> None, or (constant offsets, proto masks). A retail abs mask where the prototype has no mask and no
        .reloc entry and the raw dwords are equal is a constant (T6), dropped from the retail list when allow_const;
        then same (off, kind) mask list and (need_bytes) equal unmasked bytes."""
        pb, rb = self.pbytes(pva, n), self.rbytes(rva, n)
        if pb is None or rb is None or n <= 0:
            return None
        mp, ep = proto_map.mask_proc(pb, pva, self.plo, self.phi)
        mr, er = proto_map.mask_proc(rb, rva, self.rlo, self.rhi)
        po = {o for o, _n, _k in mp}
        consts = [o for o, _n, k in mr if allow_const and k == "abs" and o not in po and pva + o not in self.prelocs
                  and pb[o:o + 4] == rb[o:o + 4]]
        if ep != er or [(o, k) for o, _n, k in mp] != [(o, k) for o, _n, k in mr if o not in consts]:
            return None
        if need_bytes:
            pb, rb = bytearray(pb), bytearray(rb)
            for o, w, _k in mp:
                pb[o:o + w] = rb[o:o + w] = b"\0" * w
            if pb != rb:
                return None
        return consts, mp

    def masked_equal(self, pva, rva, n):
        return self.match(pva, rva, n, allow_const=False) is not None


def target(buf, va, off, kind):
    if kind == "rel":
        return (va + off + 4 + struct.unpack_from("<i", buf, off)[0]) & 0xFFFFFFFF
    return struct.unpack_from("<I", buf, off)[0]


def load():
    try:
        pin, _b, _t, _xp, xb, x = byte_compare.setup(argparse.Namespace(symbols=None, xbe=None, config=None))
    except byte_compare.Refuse as e:
        raise Refuse(str(e))
    cfg = vv_inputs.load_config()
    records = vv_inputs.load_manifest()
    exe_p = vv_inputs.input_path("prototype_dir", "default.exe", cfg)
    pdb_p = vv_inputs.input_path("prototype_dir", "default.pdb", cfg)
    for p in (exe_p, pdb_p):
        need(p)
        proto_map._check_manifest(p, cfg, records)  # exits 2 on a manifest mismatch
    exe = exe_p.read_bytes()
    try:
        pe = pe_info.parse_pe(exe)
        pdb = pdb_info.parse_pdb(pdb_p.read_bytes())
    except (pe_info.NotPe, pdb_info.NotPdb, struct.error) as e:
        raise Refuse(f"default.exe/default.pdb unparseable: {e}")
    bad = pdb_info.validate(pdb, pe["rsds"])
    if bad:
        raise Refuse(f"default.pdb / default.exe pair invalid: {'; '.join(bad)}")
    img = Images(exe, pe, xb, x)
    img.pin = pin
    pubs = defaultdict(set)
    for p in pdb["publics"]:
        if 1 <= p["seg"] <= len(pe["sections"]):
            pubs[img.base + pe["sections"][p["seg"] - 1]["va"] + p["off"]].add(p["name"])
    procs = {}
    for r in load_jsonl(need(FUNCS)):
        v = int(r["protoVA"], 16)
        procs[v] = max(procs.get(v, 0), r["length"])
    return img, pubs, procs


def insiders(procs, table):
    """-> (inside_retail(t), inside_proto(pt)): strictly inside a sized table symbol / a prototype proc."""
    pstarts = sorted(procs)
    sized = sorted((va, va + sz) for va, _k, sz, _n, _s in table if sz)
    sstarts = [a for a, _b in sized]
    smax = [0] * len(sized)  # running max end, for "strictly inside any sized symbol"
    m = 0
    for i, (_a, b) in enumerate(sized):
        m = max(m, b)
        smax[i] = m

    def inside_retail(t):
        i = bisect.bisect_left(sstarts, t) - 1  # symbols starting strictly before t
        if i < 0 or smax[i] <= t:
            return False
        return any(a < t < b for a, b in sized[:i + 1] if b > t)

    def inside_proto(pt):
        i = bisect.bisect_left(pstarts, pt) - 1
        while i >= 0 and pt - pstarts[i] < 0x100000:
            if pstarts[i] < pt < pstarts[i] + procs[pstarts[i]]:
                return True
            i -= 1
        return False

    return inside_retail, inside_proto


def kind_of(img, n, pt):
    if n.startswith("??_7"):
        return "vtable"
    if n.startswith("??_C@"):
        return "string"
    if n.startswith("__real@"):
        return "float"
    return "func" if img.pexec(pt) else "data"


def derive(img, pubs, procs, table, rows, all_sites=False):
    """-> (cands [(t, name, kind, pt, row, site)], residue [dict], derived per site [(row, site, t, name|None)])."""
    pub_vas = sorted(pubs)
    inside_retail, inside_proto = insiders(procs, table)
    cands, residue, per_site = [], [], []

    def res(r, s, t, pt, reason, names=()):
        residue.append({"rowSymbol": r["symbol"], "rowVA": f"0x{r['va']:08X}", "unit": r["unit"], "off": s["off"],
                        "siteKind": s["kind"], "target": f"0x{t:08X}",
                        "protoTarget": None if pt is None else f"0x{pt:08X}", "reason": reason,
                        "names": sorted(names)})

    for r in rows:
        va, size, pva = r["va"], r["size"], int(r["protoVA"], 16)
        rb = img.rbytes(va, size)
        if rb is None:
            raise Refuse(f"row {r['symbol']} 0x{va:08X}: no retail bytes")
        sites = [s for s in r["sites"] if (s["resolved"] != "self" if all_sites else s["resolved"] is None)]
        for s in sites:
            if target(rb, va, s["off"], s["kind"]) != s["target"]:
                raise Refuse(f"row {r['symbol']} 0x{va:08X} +0x{s['off']:X}: retail target != candidates.jsonl "
                             f"target 0x{s['target']:08X} (stale; rerun candidates.py --write)")
        if not sites:
            continue
        m = None if r.get("lane") == "near" else img.match(pva, va, size)  # 3.4 T2: a near-lane row has no identity
        if m is None:
            for s in sites:
                res(r, s, s["target"], None, "proto-mismatch")
                per_site.append((r, s, s["target"], None))
            continue
        pb = img.pbytes(pva, size)
        for s in sites:
            if s["kind"] == "abs" and s["off"] in m[0]:
                res(r, s, s["target"], None, "constant")
                per_site.append((r, s, s["target"], None))
                continue
            t, pt = s["target"], target(pb, pva, s["off"], s["kind"])
            names = pubs.get(pt, set())
            per_site.append((r, s, t, next(iter(names)) if len(names) == 1 else None))
            if not names:
                res(r, s, t, pt, "no-public")
                continue
            if len(names) > 1:
                res(r, s, t, pt, "multi-public", names)
                continue
            n = next(iter(names))
            kind = kind_of(img, n, pt)
            if inside_retail(t) or inside_proto(pt):
                res(r, s, t, pt, "inside-symbol", names)
            elif img.rsec(t) is None or img.psec(pt)["name"] != img.rsec(t)["name"]:
                res(r, s, t, pt, "section-mismatch", names)
            else:
                cands.append((t, n, kind, pt, r, s))
    return cands, residue, per_site, pub_vas


DOWN = {"callee-name-conflict": set(), "callee-short": set()}  # retail callee VAs downgraded per rule (counted)


def callee(img, pubs, tab_va, pva, rva, n):
    """Masked-equal is necessary, not sufficient: every outgoing mask (rel dest outside the callee, abs value) whose
    retail dest is a table start must name-agree with the single PDB public at the proto dest; a callee with < 8
    unmasked bytes also needs one such agreement."""
    if not img.masked_equal(pva, rva, n):
        return False
    pb, rb = img.pbytes(pva, n), img.rbytes(rva, n)
    mp = proto_map.mask_proc(pb, pva, img.plo, img.phi)[0]
    agree = 0
    for o, _w, k in mp:  # masked_equal asserted the same (off, kind) list on both sides
        pd, rd = target(pb, pva, o, k), target(rb, rva, o, k)
        if k == "rel" and rva <= rd < rva + n:
            continue
        rn = {x for x in tab_va.get(rd, ()) if (rd, x) not in NEAR_CORR}  # 3.4 T2: near-corr names never agree
        if not rn:
            continue
        ps = pubs.get(pd, ())
        if len(ps) != 1 or next(iter(ps)) not in rn:
            DOWN["callee-name-conflict"].add(rva)
            return False
        agree += 1
    if sum(b - a for a, b in proto_map.segments(n, mp)) < 8 and not agree:
        DOWN["callee-short"].add(rva)
        return False
    return True


def str_len(img, n, pt):
    """Prototype byte length of the ??_C@ literal at pt through its NUL (wide for ??_C@_1); None past raw bytes/cap."""
    step, nul = (2, b"\0\0") if n.startswith("??_C@_1") else (1, b"\0")
    k = 0
    while k < STR_CAP:
        c = img.pbytes(pt + k, step)
        if c is None:
            return None
        k += step
        if c == nul:
            return k
    return None


def vt_slots(img, pub_vas, pt, t):
    """Vtable extent (3.1): slots up to the next public (cap 64) while the proto slot points into an exec section;
    -> [(proto dword, retail dword)]."""
    i = bisect.bisect_right(pub_vas, pt)
    nslot = min((pub_vas[i] - pt) // 4 if i < len(pub_vas) else VT_CAP, VT_CAP)
    slots = []
    for k in range(nslot):
        pd, rd = img.pbytes(pt + 4 * k, 4), img.rbytes(t + 4 * k, 4)
        if pd is None or rd is None:
            break
        pv = struct.unpack("<I", pd)[0]
        if not img.pexec(pv):
            break
        slots.append((pv, struct.unpack("<I", rd)[0]))
    return slots


def check(img, procs, pubs, tab_va, pub_vas, t, n, kind, pt):
    """-> (checked bool, func size or None). Every compare reads retail bytes at the retail target."""
    if kind == "func":
        ln = procs.get(pt)
        ok = bool(ln) and callee(img, pubs, tab_va, pt, t, ln)
        return ok, ln if ok else None
    if kind == "vtable":
        slots = vt_slots(img, pub_vas, pt, t)
        return bool(slots) and all(pv in procs and callee(img, pubs, tab_va, pv, rv, procs[pv]) for pv, rv in slots), None
    if kind == "string":
        k = str_len(img, n, pt)
        if k is None:
            return False, None
        pb, rb = img.pbytes(pt, k), img.rbytes(t, k)
        return pb is not None and pb == rb, None
    if kind == "float":
        h = n[len("__real@"):]
        if len(h) not in (8, 16) or any(c not in "0123456789abcdefABCDEF" for c in h):
            return False, None
        return img.rbytes(t, len(h) // 2) == bytes.fromhex(h)[::-1], None
    return False, None


def resolve(img, procs, pubs, pub_vas, table, cands, residue):
    """Consistency over all candidates; -> {t: entry} for named VAs (pin or already-named)."""
    by_t, by_n = defaultdict(set), defaultdict(set)
    for t, n, *_ in cands:
        by_t[t].add(n)
        by_n[n].add(t)
    tab_va, tab_name = defaultdict(set), defaultdict(set)
    for va, _k, _sz, nm, _src in table:
        tab_va[va].add(nm)
        tab_name[nm].add(va)
    named = {}
    for t, n, kind, pt, r, s in cands:
        reason = None
        if len(by_t[t]) > 1:
            reason = "va-two-names"
        elif len(by_n[n] | tab_name[n]) > 1:
            reason = "name-two-vas"
        elif tab_va[t] and n not in tab_va[t]:
            reason = "named-differently"
        if reason:
            residue.append({"rowSymbol": r["symbol"], "rowVA": f"0x{r['va']:08X}", "unit": r["unit"],
                            "off": s["off"], "siteKind": s["kind"], "target": f"0x{t:08X}",
                            "protoTarget": f"0x{pt:08X}", "reason": reason, "names": sorted(by_t[t] | tab_va[t])})
            continue
        e = named.get(t)
        if e is None:
            ok, fsz = check(img, procs, pubs, tab_va, pub_vas, t, n, kind, pt) if kind != "data" else (False, None)
            e = named[t] = {"va": f"0x{t:08X}", "name": n, "kind": kind, "protoVA": f"0x{pt:08X}",
                            "evidence": "checked" if ok else "site-only", "size": fsz,
                            "already": n in tab_va[t], "sites": [], "units": set()}
        e["sites"].append(f"0x{r['va']:08X}+0x{s['off']:X}")
        e["units"].add(r["unit"])
    return named


def alias(img, procs, pubs, pub_vas, table, residue, named):
    """3.3 T9 (D44, T9.c1 probe): multi-public residue grouped by retail target t. One proto target, every public a
    func, the inside/section gates, no table name at t outside the publics, no public naming another VA, then check()
    once -> one fold-alias entry per public (the cut labels only the last pin; gate and relink resolve any table name).
    Checked t leave the residue; others stay multi-public. -> ({t: [entry]}, kept VA count)."""
    inside_retail, inside_proto = insiders(procs, table)
    tab_va, tab_name = defaultdict(set), defaultdict(set)
    for va, _k, _sz, nm, _src in table:
        tab_va[va].add(nm)
        tab_name[nm].add(va)
    other = defaultdict(set)  # name -> VAs named by the single-public route
    for t, e in named.items():
        other[e["name"]].add(t)
    by = defaultdict(list)
    for x in residue:
        if x["reason"] == "multi-public":
            by[int(x["target"], 16)].append(x)
    out, kept = {}, 0
    for t in sorted(by):
        xs = by[t]
        pts = {x["protoTarget"] for x in xs}
        ns = {n for x in xs for n in x["names"]}
        pt = int(next(iter(pts)), 16)
        ok = (len(pts) == 1 and t not in named and pubs.get(pt, set()) == ns
              and all(kind_of(img, n, pt) == "func" for n in ns)
              and not inside_retail(t) and not inside_proto(pt)
              and img.rsec(t) is not None and img.psec(pt)["name"] == img.rsec(t)["name"]
              and tab_va[t] <= ns and not any((tab_name[n] | other[n]) - {t} for n in ns))
        if ok:
            ok, fsz = check(img, procs, pubs, tab_va, pub_vas, t, sorted(ns)[0], "func", pt)
        if not ok:
            kept += 1
            continue
        sites = [f"{x['rowVA']}+0x{x['off']:X}" for x in xs]
        units = {x["unit"] for x in xs}
        out[t] = [{"va": f"0x{t:08X}", "name": n, "kind": "func", "protoVA": f"0x{pt:08X}", "evidence": "checked",
                   "size": fsz, "already": n in tab_va[t], "sites": sites, "units": units, "via": "fold-alias"}
                  for n in sorted(ns)]
    residue[:] = [x for x in residue if not (x["reason"] == "multi-public" and int(x["target"], 16) in out)]
    return out, kept


def pin_line(e):
    t = int(e["va"], 16)
    k = "func" if e["kind"] == "func" else "data"
    size = f"0x{e['size']:X}" if k == "func" and e["size"] else "-"
    via = e.get("via", "site-corr")
    if e["evidence"] == "checked" and e["kind"] in SRC_CHECK:  # data has no byte check (3.2 T4): never disasm-verified
        src, st = f"proto-pdb-public+{via}+{SRC_CHECK[e['kind']]}", "disasm-verified"
    else:
        src, st = f"proto-pdb-public+{via}", "site-corr"
    return f"0x{t:08X} {k} {size} {e['name']} {src} {st}"


def cut_name(cut, objs, unit, symbol, off):
    """Symbol named by the cut object's relocation at row `symbol` +off (C0063); None: no obj/symbol/relocation."""
    f = cut / unit.replace("\\", "__").replace("/", "__")
    if f.name not in objs:
        try:
            objs[f.name] = byte_compare.load_obj(f)[1]
        except byte_compare.Refuse:
            objs[f.name] = None
    c = objs[f.name]
    fs = [y for y in c["symbols"].values() if byte_compare.is_func(y) and y["name"] == symbol] if c else []
    if not fs:
        return None
    sec, lo, _hi = byte_compare.extent(c, fs[0])
    return next((c["symbols"][r["symbol"]]["name"] for r in sec["relocs"] if r["offset"] == lo + off), None)


def second_pass(img, pubs, procs, pub_vas, table, cands, residue, named):
    """T6: each unnamed, non-constant residue target blocking >= MIN_ROWS distinct pool rows gets (a) caller-xref
    (C0059: identical + near-identical proto_map rows, same length and mask list, PDB publics at the proto targets),
    (b) cut-sym (C0063), (c) count of ??_C@ targets referenced from its retail body (Ghidra applied function).
    Exactly one public from (a) at one proto target, the T5 gates and byte check -> named[t] (via caller-xref);
    else every residue entry of t carries the evidence. -> (examined, pinned)."""
    tab_va, tab_name = defaultdict(set), defaultdict(set)
    for va, _k, _sz, nm, _src in table:
        tab_va[va].add(nm)
        tab_name[nm].add(va)
    blocked, by_res = defaultdict(set), defaultdict(list)
    for x in residue:
        if x["reason"] != "constant":
            blocked[int(x["target"], 16)].add((x["rowSymbol"], x["rowVA"]))
            by_res[int(x["target"], 16)].append(x)
    ts = {t for t, rs in blocked.items() if len(rs) >= MIN_ROWS and t not in named and not tab_va.get(t)}
    if not ts:
        return 0, 0
    hits = defaultdict(list)  # (a): retail target -> proto targets, one per xref site
    for fr in load_jsonl(need(FUNCS)):
        if fr["class"] == "identical" and fr["targetVAs"]:
            rva = int(fr["targetVAs"][0], 16)
        elif fr["class"] == "near-identical" and fr["candidateVA"]:
            rva = int(fr["candidateVA"], 16)
        else:
            continue
        pva, n = int(fr["protoVA"], 16), fr["length"]
        rb = img.rbytes(rva, n)
        if rb is None or n <= 0:
            continue
        if not any(target(rb, rva, o, k) in ts for o, _w, k in proto_map.mask_proc(rb, rva, img.rlo, img.rhi)[0]):
            continue
        m = img.match(pva, rva, n, need_bytes=False)
        if m is None:
            continue
        pb = img.pbytes(pva, n)
        for o, _w, k in m[1]:
            if target(rb, rva, o, k) in ts:
                hits[target(rb, rva, o, k)].append(target(pb, pva, o, k))
    by_t, by_n = defaultdict(set), defaultdict(set)
    for t, n, *_ in cands:
        by_t[t].add(n)
        by_n[n].add(t)
    inside_retail, inside_proto = insiders(procs, table)
    gf = {int(g["entry"], 16): g["size"] for g in load_jsonl(need(
        REPO / "derived" / "ghidra" / f"retail-{img.pin[:8]}" / "applied" / "functions.jsonl"))}
    cut, objs, evid, want = byte_compare.cut_dir(img.pin), {}, {}, {}
    for t in sorted(ts):
        pds = sorted(set(hits[t]))
        names = set().union(*(pubs.get(pd, set()) for pd in pds))
        if not pds:
            why = "no xref"
        elif len(pds) > 1:
            why = "several proto targets"
        elif len(names) != 1:
            why = "multi-public" if names else "no-public"
        else:
            n, pt = next(iter(names)), pds[0]
            if inside_retail(t) or inside_proto(pt):
                why = "inside-symbol"
            elif img.rsec(t) is None or img.psec(pt) is None or img.psec(pt)["name"] != img.rsec(t)["name"]:
                why = "section-mismatch"
            elif by_t[t] - {n} or (by_n[n] | tab_name[n]) - {t}:
                why = "name-two-vas" if (by_n[n] | tab_name[n]) - {t} else "va-two-names"
            else:
                why = None
                want[t] = (n, pt)
        sz = gf.get(t)
        if sz is None:
            strs = "no function at VA"
        else:
            rb = img.rbytes(t, sz) or b""
            ds = {target(rb, t, o, k) for o, _w, k in proto_map.mask_proc(rb, t, img.rlo, img.rhi)[0] if k == "abs"}
            strs = sum(any(nm.startswith("??_C@") for nm in tab_va.get(d, ())) for d in ds)
        evid[t] = {"rows": len(blocked[t]),
                   "xref": {"sites": len(hits[t]), "protoTargets": len(pds), "names": sorted(names), "result": why},
                   "cutSym": sorted({cut_name(cut, objs, x["unit"], x["rowSymbol"], x["off"]) or "none"
                                     for x in by_res[t]}),
                   "strings": strs}
    dup = Counter(n for n, _pt in want.values())
    for t, (n, pt) in sorted(want.items()):
        if dup[n] > 1:
            evid[t]["xref"]["result"] = "name-two-vas"
            continue
        kind = kind_of(img, n, pt)
        ok, fsz = check(img, procs, pubs, tab_va, pub_vas, t, n, kind, pt) if kind != "data" else (False, None)
        evid[t]["xref"]["result"] = "pinned " + ("checked" if ok else "site-only")
        named[t] = {"va": f"0x{t:08X}", "name": n, "kind": kind, "protoVA": f"0x{pt:08X}",
                    "evidence": "checked" if ok else "site-only", "size": fsz, "already": False, "via": "caller-xref",
                    "sites": sorted(f"{x['rowVA']}+0x{x['off']:X}" for x in by_res[t]),
                    "units": {x["unit"] for x in by_res[t]}, "second": evid[t]}
    residue[:] = [x for x in residue if int(x["target"], 16) not in named]
    for x in residue:
        if int(x["target"], 16) in evid:
            x["evidence"] = evid[int(x["target"], 16)]
    return len(ts), sum(1 for t in ts if t in named)


def safe(n):
    """D46: decorated string-literal names and Ghidra string labels carry string text: masked in the doc."""
    if n.startswith("??_C@"):
        return "??_C@ (string literal)"
    if n.startswith(("s_", "u_")):
        return "string label"
    return n


def names_txt(ns):
    ns = sorted(ns)
    if len(ns) <= 1:
        return safe(ns[0]) if ns else "-"
    return f"{len(ns)} names: " + ", ".join(safe(n) for n in ns[:2])


def write_doc(rows, residue, examined, pinned, kept=None):
    by = defaultdict(list)
    for x in residue:
        by[int(x["target"], 16)].append(x)
    rr = Counter(x["reason"] for x in residue)
    rv = defaultdict(set)
    for x in residue:
        rv[x["reason"]].add(x["target"])
    nrows = {t: len({(x["rowSymbol"], x["rowVA"]) for x in xs}) for t, xs in by.items()}
    out = ["# Naming residue (generated)", "",
           "Generated by `PY tools/name_sites.py --doc` (3.1 T6) from the pool rows, the prototype PDB and the cut;"
           " never hand-edited. Addresses, names and counts only (D46): `??_C@` names and Ghidra string labels are"
           " masked. Reasons and routes: docs/ops/tools.md `## name_sites`.", "",
           f"Pool rows {len(rows)}; residue sites {len(residue)} over {len(by)} target VAs. Second pass (targets"
           f" blocking >= {MIN_ROWS} rows): examined {examined}, pinned {pinned}, kept {examined - pinned}.", "",
           "## Counts by reason", ""]
    out += [f"- {r}: {rr[r]} sites, {len(rv[r])} VAs" for r in REASONS if rr[r]]
    out += ["", "## Residue targets", "",
            "Rows blocked desc, then VA. `VA · sites · rows blocked · reasons · names · evidence`.", ""]
    for t in sorted(by, key=lambda t: (-nrows[t], t)):
        xs = by[t]
        rc = Counter(x["reason"] for x in xs)
        reasons = ", ".join(f"{r} {rc[r]}" for r in REASONS if rc[r])
        names = names_txt({n for x in xs for n in x["names"]})
        ev = xs[0].get("evidence")
        if ev:
            xr = ev["xref"]
            evt = (f"xref: {xr['result'] or 'one public'} (sites {xr['sites']}, proto targets {xr['protoTargets']}, "
                   f"names {names_txt(xr['names'])}); cut-sym: {names_txt(ev['cutSym'])}; strings: {ev['strings']}")
        elif set(rc) == {"constant"}:
            evt = "not searched (constant, not an address)"
        else:
            evt = f"not searched (< {MIN_ROWS} rows)"
        out.append(f"- 0x{t:08X} · {len(xs)} · {nrows[t]} · {reasons} · {names} · {evt}")
    if kept is not None:
        kr = Counter(e["reason"] for e in kept)
        out += ["", "## Data pins kept site-corr (3.3 T8)", "",
                f"`--promote-data` route (docs/ops/tools.md `## name_sites`): {len(kept)} data pin(s) kept `site-corr`.",
                ""]
        out += [f"- {r}: {kr[r]}" for r in PROMO_REASONS if kr[r]]
        out += ["", "`VA · name · kind · reason`.", ""]
        out += [f"- {e['va']} · {safe(e['name'])} · {e['kind']} · {e['reason']}" for e in kept]
    DOC.write_bytes(("\n".join(out) + "\n").encode("utf-8"))


def write_dup_doc(residue):
    """3.3 T9 (D44): each multi-public target VA with its prototype publics, retail fold status, sites, the pool rows
    it blocks and how many of them it alone blocks (no other residue site in the row)."""
    mp = defaultdict(list)
    for x in residue:
        if x["reason"] == "multi-public":
            mp[x["target"]].append(x)
    targets = defaultdict(set)  # row -> residue target VAs, all reasons
    for x in residue:
        targets[(x["rowSymbol"], x["rowVA"])].add(x["target"])
    folds = {}
    for e in load_jsonl(need(FOLDS)):
        if e["retailVA"] in mp:
            folds[e["retailVA"]] = e
    rows = {t: sorted({(x["rowVA"], x["rowSymbol"]) for x in xs}) for t, xs in mp.items()}
    sole = {t: {r for r in rs if targets[(r[1], r[0])] == {t}} for t, rs in rows.items()}
    fk = Counter(folds[t]["kind"] if t in folds else "absent" for t in mp)
    out = ["# Multi-public residue (generated)", "",
           "Generated by `PY tools/name_sites.py --doc` (3.3 T9, D44) from the residue (`derived/name_sites/"
           "residue.jsonl`, reason multi-public) and `derived/dup_report/retail_folds.jsonl`; never hand-edited. A"
           " multi-public VA is a retail call target whose prototype VA carries several publics (ICF/COMDAT folds)."
           " Fold status `absent`: the VA is outside the dup_report population (proto_map located no proto there)."
           " Sole: rows with no other residue site. Binding: docs/evidence/duplicates.md.", "",
           f"Multi-public VAs {len(mp)}; sites {sum(map(len, mp.values()))}; rows blocked"
           f" {len({r for rs in rows.values() for r in rs})}; rows blocked by one multi-public VA alone"
           f" {sum(map(len, sole.values()))}.", "",
           "Fold status: " + ", ".join(f"{k} {fk[k]}" for k in sorted(fk)) + ".", "",
           "## VAs", "", "Rows blocked desc, then VA. `VA · rows blocked · sole · sites · fold · publics`, then rows"
           " (`*` sole).", ""]
    for t in sorted(mp, key=lambda t: (-len(rows[t]), t)):
        f = folds.get(t)
        fs = f"{f['kind']} ({len(f['protos'])} protos, {f['retailGroup']})" if f else "absent"
        pubs = sorted({n for x in mp[t] for n in x["names"]})
        out.append(f"- {t} · {len(rows[t])} · {len(sole[t])} · {len(mp[t])} · {fs} · {len(pubs)} publics: "
                   + ", ".join(safe(n) for n in pubs))
        out += [f"  - {'*' if r in sole[t] else ''}{r[0]} {safe(r[1])}" for r in rows[t]]
    DUP_DOC.write_bytes(("\n".join(out) + "\n").encode("utf-8"))


def check_cut(rows):
    """Every pool site whose target is pinned in symbols_pin.txt, resolved or not (3.2 T4): the cut object has a relocation
    at the site naming one of the target's table names; every other unresolved site: residue."""
    try:
        pin = byte_compare.setup(argparse.Namespace(symbols=None, xbe=None, config=None))[0]
    except byte_compare.Refuse as e:
        raise Refuse(str(e))
    pinned, names = set(), defaultdict(set)
    for va, _k, _sz, nm, src in load_table():
        names[va].add(nm)
    for ln in need(SYMS_PIN).read_text(encoding="utf-8").splitlines():
        if ln.strip() and not ln.startswith("#"):
            pinned.add(int(ln.split()[0], 16))
    res_t = {int(x["target"], 16) for x in load_jsonl(need(OUT / "residue.jsonl"))}
    cut, objs, fails, npin, nres = byte_compare.cut_dir(pin), {}, [], 0, 0
    for r in rows:
        for s in r["sites"]:
            t, where = s["target"], f"{r['symbol']} 0x{r['va']:08X} +0x{s['off']:X} -> 0x{s['target']:08X}"
            if t in pinned:
                nm = cut_name(cut, objs, r["unit"], r["symbol"], s["off"])
                if nm in names[t] or nm and nm.rpartition("::")[2] in names[t]:  # Ghidra library namespace (XAPILIB::)
                    npin += 1
                else:
                    fails.append(f"FAIL {where} {s['kind']}: pinned {sorted(names[t])}, cut relocation names {nm}")
            elif s["resolved"] is not None:
                continue
            elif t in res_t:
                nres += 1
            else:
                fails.append(f"FAIL {where}: neither pinned nor residue")
    print(f"name_sites --check-cut: {len(rows)} pool rows; pinned sites cut-named {npin}, unresolved residue {nres}, "
          f"FAIL {len(fails)}")
    for ln in fails[:200]:
        print(f"  {ln}")
    return 1 if fails else 0


def pool_rows(path):
    rows = load_jsonl(need(path))
    ms = tomllib.loads(need(MATCHED).read_text(encoding="utf-8"))
    matched = {(f["symbol"], f["va"] if isinstance(f["va"], int) else int(str(f["va"]), 16))
               for f in ms.get("function", [])}
    return [r for r in rows if (r["symbol"], r["va"]) not in matched]


def run_positive(img, pubs, procs, table, rows):
    _c, _r, per_site, _p = derive(img, pubs, procs, table, rows, all_sites=True)
    proto_names, other = defaultdict(set), defaultdict(set)
    for va, _k, _sz, nm, src in table:
        (proto_names if src.startswith("proto-pdb") else other)[va].add(nm)
    checked, bad, underived, others, others_agree = set(), [], 0, 0, 0
    for r, s, t, n in per_site:
        if t in proto_names:
            if n is None:
                underived += 1
            elif n in proto_names[t]:
                checked.add(t)
            else:
                pva = int(r["protoVA"], 16)
                bad.append((t, n, sorted(proto_names[t]), r["symbol"], s["off"],
                            target(img.pbytes(pva, r["size"]), pva, s["off"], s["kind"])))
        elif t in other:
            others += 1
            others_agree += n in other[t]
    # 3.4 T2: callee-moved: the proto site target pt is itself a proto_map identical row whose one target X != t
    # (the retail site calls another function than the proto's callee counterpart) -> excluded, counted, printed.
    ident = {}
    for fr in load_jsonl(need(FUNCS)):
        if fr["class"] == "identical" and len(fr["targetVAs"]) == 1:
            ident[int(fr["protoVA"], 16)] = int(fr["targetVAs"][0], 16)
    excluded, kept = [], []
    for t, n, want, sym, off, pt in bad:
        x = ident.get(pt)
        (excluded if x is not None and x != t else kept).append((t, n, want, sym, off, pt, x))
    print(f"name_sites --control positive: {len(checked)} retail VAs checked (proto-pdb names), "
          f"{len(kept)} disagreement(s), {len(excluded)} excluded callee-moved, {underived} site(s) with no single public")
    print(f"  other sources (ghidra, xbsymdb, retail-disasm, ...): {others} site(s), {others_agree} agree (counted only)")
    for t, _n, _w, sym, off, pt, x in sorted(set((a, b, tuple(c), d, e, f, g) for a, b, c, d, e, f, g in excluded)):
        print(f"  EXCLUDED 0x{t:08X} callee-moved: proto 0x{pt:08X} -> identical 0x{x:08X} (site {sym} +0x{off:X})")
    for t, n, want, sym, off, _pt, _x in sorted(set((a, b, tuple(c), d, e, f, g) for a, b, c, d, e, f, g in kept))[:20]:
        print(f"  DISAGREE 0x{t:08X}: derived {n} != table {', '.join(want)} (site {sym} +0x{off:X})")
    return 1 if kept else 0


def run_planted(img, pubs, procs, table, rows):
    pick = next((r for r in sorted(rows, key=lambda r: r["va"]) if any(s["resolved"] is None for s in r["sites"])
                 and img.masked_equal(int(r["protoVA"], 16), r["va"], r["size"])), None)
    if pick is None:
        print("name_sites --control planted: no identical pool row with an unresolved site")
        return 1
    planted = dict(pick, protoVA=f"0x{int(pick['protoVA'], 16) + 0x10:08X}")
    rows = [planted if r is pick else r for r in rows]
    cands, residue, _ps, _pv = derive(img, pubs, procs, table, rows)
    mine = [x for x in residue if x["rowSymbol"] == pick["symbol"] and x["rowVA"] == f"0x{pick['va']:08X}"]
    nsites = sum(s["resolved"] is None for s in pick["sites"])
    derived = [c for c in cands if c[4] is planted]
    ok = len(mine) == nsites and all(x["reason"] == "proto-mismatch" for x in mine) and not derived
    print(f"name_sites --control planted: row {pick['symbol']} 0x{pick['va']:08X} protoVA {pick['protoVA']} -> "
          f"{planted['protoVA']}: {len(mine)}/{nsites} site(s) proto-mismatch, {len(derived)} derived -> "
          f"{'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


DATA_RECS = {0x110D: False, 0x110C: False, 0x1008: True, 0x1007: True}  # S_GDATA32, S_LDATA32, _ST: pascal name
PROMO_SRC = "proto-pdb-public+site-corr+data-bytes"
PROMO_REASONS = ("proto-name", "literal-differs", "real-differs", "vtable-no-slots", "no-type", "type-size",
                 "no-proto-bytes", "bss", "pointer-unnamed", "pointer-unchecked", "pointer-differs", "bytes-differ",
                 "zero-bytes")
PROMO_KINDS = ("vtable", "global", "string", "float")


def data_records(img, cfg=None):
    """{"glob"|"mod": {proto VA: {record name: {typind}}}} of S_GDATA32/S_LDATA32 (and _ST) records: global symbol
    stream, module streams; also loads pdb_types' TPI. PDB re-read here: pdb_info.parse_pdb keeps publics only.
    cfg: local_paths.json path (default config/local_paths.json; candidates passes its --config, 3.4 T16)."""
    import pdb_types
    st = pdb_info.read_streams(vv_inputs.input_path("prototype_dir", "default.pdb", vv_inputs.load_config(cfg))
                               .read_bytes())["streams"]
    pdb_types.load_tpi(st)
    out = {"glob": defaultdict(lambda: defaultdict(set)), "mod": defaultdict(lambda: defaultdict(set))}

    def scan(b, key):
        for kind, p in pdb_info.iter_records(b):
            if kind in DATA_RECS and len(p) >= 10:
                ti, off, seg = struct.unpack_from("<IIH", p, 0)
                if 1 <= seg <= len(img.psecs):
                    out[key][img.base + img.psecs[seg - 1]["va"] + off][pdb_info._cstr(p, 10, DATA_RECS[kind])].add(ti)

    dbi = st[3]
    symrec, modi_sz = struct.unpack_from("<H", dbi, 20)[0], struct.unpack_from("<i", dbi, 24)[0]
    if symrec != 0xFFFF and st[symrec]:
        scan(st[symrec], "glob")
    o, end = 64, 64 + modi_sz  # module info walk as pdb_info.parse_pdb
    while o + 64 <= end:
        sn, symsz = struct.unpack_from("<HI", dbi, o + 34)
        o2 = dbi.index(b"\0", o + 64) + 1
        o = (dbi.index(b"\0", o2) + 1 + 3) & ~3
        if sn != 0xFFFF and sn < len(st) and st[sn]:
            scan(st[sn][4:symsz], "mod")
    return out


def promote_inputs(with_promoted=False):
    """-> (site-corr data pins [(va, name)] in VA order, accepted {va: {names}}) from symbols.txt (status in
    units_seed.STATUSES) and symbols_pin.txt (status in candidates.PIN_STATUSES). with_promoted (planted-data control):
    pins already promoted by this route (source PROMO_SRC) join the pin list, so the control runs after --apply."""
    import candidates
    import units_seed
    pins, acc = [], defaultdict(set)
    for tab, ok in ((SYMS, units_seed.STATUSES), (SYMS_PIN, candidates.PIN_STATUSES)):
        for ln in need(tab).read_text(encoding="utf-8").splitlines():
            if not ln.strip() or ln.startswith("#"):
                continue
            f = ln.split()
            va, nm = int(f[0], 16), " ".join(f[3:-2])
            if f[-1] in ok:
                acc[va].add(nm)
                if with_promoted and tab == SYMS_PIN and f[1] == "data" and f[-2] == PROMO_SRC:
                    pins.append((va, nm))
            elif tab == SYMS_PIN and f[1] == "data" and f[-1] == "site-corr":
                pins.append((va, nm))
    return sorted(pins), acc


def rec_size(drecs, P, N):
    """Route step b: the S_GDATA32/S_LDATA32 records at proto VA P named N (global stream first, then modules) ->
    their one pdb_types.tsize (int), else the reason "no-type" | "type-size". Also candidates' member extents (3.4 T16)."""
    import pdb_types
    # data records carry the undecorated name: rec named N = N itself, `_` + rec (C), or `?` + rec scope reversed `@@`
    q = lambda r: N in (r, "_" + r) or N.startswith("?" + "@".join(reversed(r.split("::"))) + "@@")
    tis = (set().union(*(t for r, t in drecs["glob"].get(P, {}).items() if q(r)))
           or set().union(*(t for r, t in drecs["mod"].get(P, {}).items() if q(r))))
    if not tis:
        return "no-type"
    try:
        sizes = set().union(*(pdb_types.tsize(t) for t in tis))
    except (pdb_types.TypeSizeError, KeyError):
        return "type-size"
    if len(sizes) != 1:
        return "type-size"
    return sizes.pop()


def route_one(img, pubs, byname, pub_vas, drecs, R, N, acc):
    """The 3.3 T8 data byte route for retail VA R named N; first failing step gives the one reason."""
    kind = ("vtable" if N.startswith("??_7") else "string" if N.startswith("??_C@")
            else "float" if N.startswith("__real@") else "global")
    e = {"va": f"0x{R:08X}", "name": N, "kind": kind, "protoVA": None, "size": None, "verdict": "fail",
         "reason": None, "pointers": []}

    def fail(r):
        e["reason"] = r
        return e

    ps = byname.get(N, ())
    if len(ps) != 1:
        return fail("proto-name")
    P = next(iter(ps))
    e["protoVA"] = f"0x{P:08X}"
    if kind in ("string", "float"):
        e["size"] = str_len(img, N, P) if kind == "string" else (len(N) - len("__real@")) // 2
        if not check(img, None, pubs, None, pub_vas, R, N, kind, P)[0]:
            return fail("literal-differs" if kind == "string" else "real-differs")
        e["verdict"] = "pass"
        return e
    slot_offs = set()
    if kind == "vtable":
        slots = vt_slots(img, pub_vas, P, R)
        if not slots:
            return fail("vtable-no-slots")
        S, slot_offs = 4 * len(slots), {4 * k for k in range(len(slots))}
    else:
        S = rec_size(drecs, P, N)
        if isinstance(S, str):
            return fail(S)
    e["size"] = S
    pb, rb = img.pbytes(P, S), img.rbytes(R, S)
    if pb is None:
        return fail("no-proto-bytes")
    if rb is None:
        return fail("bss")
    offs = sorted(slot_offs | {o for o in range(0, S - 3) if P + o in img.prelocs})
    first, agree = None, 0
    for o in offs:
        PT, RT = struct.unpack_from("<I", pb, o)[0], struct.unpack_from("<I", rb, o)[0]
        names, ok_n = pubs.get(PT, set()), acc.get(RT, set())
        ok = bool(names & ok_n)
        agree += ok
        e["pointers"].append({"off": o, "protoTarget": f"0x{PT:08X}", "retailTarget": f"0x{RT:08X}", "agree": ok})
        if not ok and first is None:
            first = "pointer-unnamed" if not names else "pointer-unchecked" if not ok_n else "pointer-differs"
    if first:
        return fail(first)
    mp, mr = bytearray(pb), bytearray(rb)
    for o in offs:
        mp[o:o + 4] = mr[o:o + 4] = b"\0\0\0\0"
    if mp != mr:
        return fail("bytes-differ")
    if not agree and not any(mp):
        return fail("zero-bytes")
    e["verdict"] = "pass"
    return e


def promote_route(img, pubs, pins, acc):
    """Route every pin; pins that pass join the accepted names; repeat to a fixed point (VA order). -> [entry]."""
    byname = defaultdict(set)
    for va, ns in pubs.items():
        for n in ns:
            byname[n].add(va)
    pub_vas, drecs = sorted(pubs), data_records(img)
    acc = defaultdict(set, {k: set(v) for k, v in acc.items()})
    res, passed = {}, set()
    while True:
        grew = False
        for va, nm in pins:
            if va in passed:
                continue
            e = res[va] = route_one(img, pubs, byname, pub_vas, drecs, va, nm, acc)
            if e["verdict"] == "pass":
                passed.add(va)
                acc[va].add(nm)
                grew = True
        if not grew:
            break
    return [res[va] for va, _n in pins], (byname, pub_vas, drecs)


def apply_promote(entries, unit):
    """Rewrite in place the passing pins' lines (VA + `data` + `site-corr`, not by name); -> lines rewritten.
    Refuse when a passing VA has not exactly one such line."""
    good = {int(e["va"], 16) for e in entries if e["verdict"] == "pass"}
    if not good:
        return 0
    lines = SYMS_PIN.read_bytes().decode("utf-8").split("\n")
    hits = defaultdict(list)
    for i, ln in enumerate(lines):
        f = ln.split()
        if ln.strip() and not ln.startswith("#") and f[1] == "data" and f[-1] == "site-corr" and int(f[0], 16) in good:
            hits[int(f[0], 16)].append(i)
    bad = sorted(va for va in good if len(hits[va]) != 1)
    if bad:
        raise Refuse(f"--apply: {len(bad)} passing VA(s) without exactly one data site-corr pin line, e.g. "
                     + ", ".join(f"0x{v:08X}" for v in bad[:4]))
    for va in good:
        i = hits[va][0]
        lines[i] = lines[i].rsplit(" ", 2)[0] + f" {PROMO_SRC} disasm-verified"
    text = "\n".join(lines)
    sep = "" if text.endswith("\n") else "\n"
    text += (sep + f"# 3.3 T8 name_sites --promote-data{' --unit ' + unit if unit else ''}: {len(good)} lines promoted "
             f"in place; evidence docs/evidence/correspondence.md \"## Data byte route (3.3 T8)\"\n")
    SYMS_PIN.write_bytes(text.encode("utf-8"))
    return len(good)


def run_promote(img, pubs, rows, unit, apply):
    pins, acc = promote_inputs()
    if unit:
        ts = {s["target"] for r in rows if r["unit"] == unit for s in r["sites"]}
        pins = [p for p in pins if p[0] in ts]
    ents, _ = promote_route(img, pubs, pins, acc)
    OUT.mkdir(parents=True, exist_ok=True)
    with open(OUT / "promote.jsonl", "w", encoding="utf-8", newline="\n") as f:
        for e in ents:
            f.write(json.dumps(e, sort_keys=True) + "\n")
    vc = Counter(e["verdict"] for e in ents)
    rc = Counter(e["reason"] for e in ents if e["reason"])
    kc = Counter((e["kind"], e["verdict"]) for e in ents)
    print(f"name_sites --promote-data{' --unit ' + unit if unit else ''}: {len(ents)} site-corr data pin(s); "
          f"pass {vc['pass']}, fail {vc['fail']}")
    print("  kinds: " + ", ".join(f"{k} pass {kc[(k, 'pass')]} fail {kc[(k, 'fail')]}" for k in PROMO_KINDS))
    print("  reasons: " + ", ".join(f"{r} {rc[r]}" for r in PROMO_REASONS if rc[r]))
    print(f"  wrote {OUT.relative_to(REPO).as_posix()}/promote.jsonl")
    if apply:
        n = apply_promote(ents, unit)
        print(f"  promoted {n} line(s) in place in {SYMS_PIN}")
    return 0


def run_planted_data(img, pubs):
    pins, acc = promote_inputs(with_promoted=True)
    ents, (byname, pub_vas, drecs) = promote_route(img, pubs, pins, acc)
    ok_e = [e for e in ents if e["verdict"] == "pass"]
    if not ok_e:
        print("name_sites --control planted-data: no passing pin")
        return 1
    A = ok_e[0]
    a = int(A["va"], 16)
    B = next((b for b in ok_e[1:] if b["size"] and img.pbytes(int(b["protoVA"], 16), b["size"]) != img.rbytes(a, b["size"])),
             None)
    if B is None:
        print("name_sites --control planted-data: no passing pin whose bytes differ at A")
        return 1
    e = route_one(img, pubs, byname, pub_vas, drecs, a, B["name"], acc)
    n = apply_promote([e], None)
    ok = e["verdict"] == "fail" and n == 0
    print(f"name_sites --control planted-data: VA {A['va']} ({safe(A['name'])}) planted name {safe(B['name'])}: "
          f"verdict {e['verdict']} {e['reason']}, {n} line(s) rewritten -> {'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


NEAR_SRC = "proto-pdb-public+anchor-window+caller-xref"  # 3.4 T2 near-corr pin source
NEAR_POS_FLOOR = 10  # 3.4 T2 ruling: near-positive needs >= 10 named (judged) rows
NEAR_REASONS = ("new", "upgrade", "already", "no-public", "multi-public", "not-func", "outside-window", "similarity-below",
                "similarity-drift", "no-xref", "xref-conflict", "fold", "va-two-rows", "named-differently",
                "name-two-vas", "inside-symbol", "no-ghidra-function", "ghidra-noncontiguous", "extent-overlap")
UNITS = REPO / "config" / "retail" / "units.toml"


def near_ctx(img, pubs, procs):
    """3.4 T2: indexes near_check reads, built once: proto_map identical LIS anchors (proto_map.lis_anchors over
    functions.jsonl identical rows), near rows by protoVA, retail exec section bounds, and the caller-xref index
    (C0059) over accepted table funcs (symbols.txt units_seed.STATUSES, symbols_pin.txt candidates.PIN_STATUSES;
    never site-corr, boundary, near-corr) whose unique proto public counterpart img.match()es: per rel mask with proto
    dest outside the caller, (pd, rd, caller VA)."""
    import candidates
    import units_seed
    funcs = load_jsonl(need(FUNCS))
    anchors = proto_map.lis_anchors([(int(r["protoVA"], 16), int(r["targetVAs"][0], 16), r["length"])
                                     for r in funcs if r["class"] == "identical" and len(r["targetVAs"]) == 1])
    near = {int(r["protoVA"], 16): r for r in funcs if r["class"] == "near-identical" and r["candidateVA"]}
    secb = sorted((s["virtualAddr"], s["virtualAddr"] + s["rawSize"]) for s in img.x["sections"]
                  if s["flags"] & proto_map.SEC_EXEC)
    pubname = defaultdict(set)
    for v, ns in pubs.items():
        for n in ns:
            pubname[n].add(v)
    by_pd, by_rd = defaultdict(list), defaultdict(list)
    for tab, ok in ((SYMS, units_seed.STATUSES), (SYMS_PIN, candidates.PIN_STATUSES)):
        for ln in need(tab).read_text(encoding="utf-8").splitlines():
            if not ln.strip() or ln.startswith("#"):
                continue
            f = ln.split()
            if f[1] != "func" or f[-1] not in ok or f[-1] in ("site-corr", "boundary", "near-corr"):
                continue
            cva, pv = int(f[0], 16), pubname.get(" ".join(f[3:-2]), set())
            if len(pv) != 1:
                continue
            cpva = next(iter(pv))
            ln_ = procs.get(cpva)
            m = img.match(cpva, cva, ln_) if ln_ else None
            if m is None:
                continue
            pb, rb = img.pbytes(cpva, ln_), img.rbytes(cva, ln_)
            for o, _w, k in m[1]:
                if k != "rel":
                    continue
                pd = target(pb, cpva, o, k)
                if cpva <= pd < cpva + ln_:
                    continue
                rd = target(rb, cva, o, k)
                by_pd[pd].append((rd, cva))
                by_rd[rd].append((pd, cva))
    return {"img": img, "pubs": pubs, "procs": procs, "anchors": anchors, "aprotos": [a[0] for a in anchors],
            "near": near, "secb": secb, "pubname": pubname, "by_pd": by_pd, "by_rd": by_rd}


def near_check(ctx, pva, rva):
    """3.4 T2: one verdict for a (protoVA, retailVA) pair; the first failing step is the reason.
    1 name: one PDB public N at pva (no-public, multi-public), pva a proto exec func (not-func). 2 window: rva in a
    proto_map gap range of pva's gap, anchors without pva's own pair (outside-window). 3 similarity: proto_map tokens
    and similarity >= THRESHOLD (similarity-below); a proto_map near row at its own candidateVA must reproduce its
    similarity within 0.01 (similarity-drift). 4 xref (C0059): agreeing caller sites (pd == pva, rd == rva) >= 1
    (no-xref), no conflicting ones (xref-conflict); callers at rva skipped."""
    img, procs = ctx["img"], ctx["procs"]
    out = {"verdict": "fail", "reason": None, "name": None, "sim": None, "window": None, "agree": 0, "conflicts": 0}
    ns = ctx["pubs"].get(pva, set())
    if len(ns) != 1:
        return dict(out, reason="multi-public" if ns else "no-public")
    out["name"] = next(iter(ns))
    ln = procs.get(pva)
    pb = img.pbytes(pva, ln) if ln else None
    if not img.pexec(pva) or pb is None:
        return dict(out, reason="not-func")
    anchors, aprotos = ctx["anchors"], ctx["aprotos"]
    if pva in aprotos:
        anchors = [a for a in anchors if a[0] != pva]
        aprotos = [a[0] for a in anchors]
    g = bisect.bisect_left(aprotos, pva)
    win = next(((a, b) for a, b in proto_map.gap_ranges(anchors, g, ctx["secb"]) if a <= rva < b), None)
    if win is None:
        return dict(out, reason="outside-window")
    out["window"] = f"0x{win[0]:08X}-0x{win[1]:08X}"
    ptok = proto_map.tokens(pb, pva, img.plo, img.phi)[0]
    rb = img.rbytes(rva, min(win[1], rva + 15 * len(ptok)) - rva) or b""
    ctok = proto_map.tokens(rb, rva, img.rlo, img.rhi)[0][:len(ptok)]
    sim = proto_map.similarity(ptok, ctok)
    out["sim"] = round(sim, 2)
    if sim < proto_map.THRESHOLD:
        return dict(out, reason="similarity-below")
    row = ctx["near"].get(pva)
    if row is not None and int(row["candidateVA"], 16) == rva and abs(sim - row["similarity"]) > 0.01:
        return dict(out, reason="similarity-drift")
    out["agree"] = sum(1 for rd, cva in ctx["by_pd"].get(pva, ()) if rd == rva and cva != rva)
    out["conflicts"] = (sum(1 for rd, cva in ctx["by_pd"].get(pva, ()) if rd != rva and cva != rva)
                        + sum(1 for pd, cva in ctx["by_rd"].get(rva, ()) if pd != pva and cva != rva))
    if not out["agree"]:
        return dict(out, reason="no-xref")
    if out["conflicts"]:
        return dict(out, reason="xref-conflict")
    return dict(out, verdict="pass", reason="pass")


def unit_of_va():
    """-> (va -> units.toml unit name | None, {name: unit dict}); text_start <= va < text_end (candidates.py rule)."""
    us = sorted((u for u in tomllib.loads(need(UNITS).read_text(encoding="utf-8"))["unit"] if "text_start" in u),
                key=lambda u: u["text_start"])
    st = [u["text_start"] for u in us]

    def f(va):
        i = bisect.bisect_right(st, va) - 1
        return us[i]["name"] if i >= 0 and va < us[i]["text_end"] else None
    return f, {u["name"]: u for u in us}


def near_rows(ctx):
    """proto_map near-identical rows with candidateVA and similarity >= THRESHOLD -> [(rva, pva, row)] in VA order."""
    return sorted((int(r["candidateVA"], 16), pva, r) for pva, r in ctx["near"].items()
                  if r["similarity"] is not None and r["similarity"] >= proto_map.THRESHOLD)


def size_bucket(n):
    lo, hi = next(((a, b) for a, b in BUCKETS if n is not None and n >= a and (b is None or n <= b)), (None, None))
    return "none" if lo is None else f"{lo}-{hi}" if hi else f">={lo}"


BUCKETS = ((1, 8), (9, 16), (17, 32), (33, 64), (65, 128), (129, None))  # = match_one.BUCKETS


def run_near(img, pubs, procs, table, apply):
    """3.4 T2 --near: every proto_map near row through near_check, then the production gates (fold, va-two-rows,
    already / named-differently, name-two-vas, inside-symbol, Ghidra extent). Writes near.jsonl, near_pins.txt."""
    ctx = near_ctx(img, pubs, procs)
    uof, _units = unit_of_va()
    folds = {int(e["retailVA"], 16) for e in load_jsonl(need(FOLDS))}
    gf = {int(g["entry"], 16): (g["size"], int(g["bodyMax"], 16)) for g in load_jsonl(need(
        REPO / "derived" / "ghidra" / f"retail-{img.pin[:8]}" / "applied" / "functions.jsonl"))}
    tab_va, tab_name = defaultdict(set), defaultdict(set)
    for va, _k, _sz, nm, _src in table:
        tab_va[va].add(nm)
        tab_name[nm].add(va)
    tstarts = sorted(tab_va)
    inside_retail, _ip = insiders(procs, table)
    rows = near_rows(ctx)
    nrva = Counter(rva for rva, _p, _r in rows)
    nline = Counter(va for va, *_ in table)
    upg = set()  # 3.4 T2 ruling: (va, name) of the one table line at va, a symbols_pin.txt unsized site-corr func
    for ln in need(SYMS_PIN).read_text(encoding="utf-8").splitlines():
        f = ln.split()
        if ln.strip() and not ln.startswith("#") and f[1] == "func" and f[2] == "-" and f[-1] == "site-corr" \
                and nline[int(f[0], 16)] == 1:
            upg.add((int(f[0], 16), " ".join(f[3:-2])))
    recs, lines, ups = [], [], {}
    for rva, pva, _r in rows:
        c = near_check(ctx, pva, rva)
        n = c["name"]
        size = gf[rva][0] if rva in gf else None
        reason = c["reason"]
        if c["verdict"] == "pass":
            if rva in folds:
                reason = "fold"
            elif nrva[rva] > 1:
                reason = "va-two-rows"
            elif tab_va[rva] and (rva, n) not in upg:
                reason = "already" if n in tab_va[rva] else "named-differently"
            elif tab_name[n] - {rva} or ctx["pubname"][n] - {pva}:
                reason = "name-two-vas"
            elif inside_retail(rva):
                reason = "inside-symbol"
            elif rva not in gf:
                reason = "no-ghidra-function"
            elif gf[rva][0] != gf[rva][1] - rva + 1:
                reason = "ghidra-noncontiguous"
            elif any(rva < s < rva + size for s in tstarts[bisect.bisect_right(tstarts, rva):
                                                              bisect.bisect_left(tstarts, rva + size)]):
                reason = "extent-overlap"
            elif (rva, n) in upg:  # 3.4 T2 ruling: the unsized site-corr line is rewritten in place, sized near-corr
                reason = "upgrade"
                ups[rva] = f"0x{rva:08X} func 0x{size:X} {n} {NEAR_SRC} near-corr"
            else:
                reason = "new"
                lines.append(f"0x{rva:08X} func 0x{size:X} {n} {NEAR_SRC} near-corr")
        recs.append({"protoVA": f"0x{pva:08X}", "va": f"0x{rva:08X}", "name": n, "unit": uof(rva), "size": size,
                     "sim": c["sim"], "verdict": "pass" if reason in ("new", "already", "upgrade") else "fail",
                     "reason": reason, "agree": c["agree"], "conflicts": c["conflicts"]})
    OUT.mkdir(parents=True, exist_ok=True)
    with open(OUT / "near.jsonl", "w", encoding="utf-8", newline="\n") as f:
        for x in recs:
            f.write(json.dumps(x, sort_keys=True) + "\n")
    (OUT / "near_pins.txt").write_bytes("".join(ln + "\n" for ln in lines).encode("utf-8"))
    rc = Counter(x["reason"] for x in recs)
    adm = [x for x in recs if x["verdict"] == "pass"]
    print(f"name_sites --near: {len(recs)} near row(s); admitted {len(adm)} (new {rc['new']}, upgrade "
          f"{rc['upgrade']}, already {rc['already']}); pins {len(lines)}, rewrites {len(ups)}")
    print("  by reason: " + ", ".join(f"{r} {rc[r]}" for r in NEAR_REASONS))
    bk = Counter(size_bucket(x["size"]) for x in adm)
    print("  admitted by size: " + ", ".join(f"{b} {bk[b]}" for b in [size_bucket(a) for a, _b in BUCKETS] + ["none"]))
    pu = Counter(x["unit"] or "(no unit)" for x in adm)
    print(f"  admitted per unit ({len(pu)} unit(s)): "
          + ", ".join(f"{u.replace(chr(92), '/').split('/')[-1]} {k}" for u, k in sorted(pu.items(), key=lambda t: (-t[1], t[0]))))
    print(f"  wrote {OUT.relative_to(REPO).as_posix()}/{{near.jsonl,near_pins.txt}}")
    if apply and lines:
        have = {va for va, *_ in load_table()}
        dup = [ln.split()[0] for ln in lines if int(ln.split()[0], 16) in have]
        if dup:
            print(f"name_sites: REFUSED --apply: {len(dup)} VA(s) already named, e.g. {', '.join(dup[:4])}")
            return 2
        old = SYMS_PIN.read_bytes().decode("utf-8")
        hdr = (f"# 3.4 T2 name_sites --near: {len(lines)} lines; evidence docs/evidence/correspondence.md "
               f"\"## Near-identical naming (3.4 T2)\"")
        sep = "" if not old or old.endswith("\n") else "\n"
        SYMS_PIN.write_bytes((old + sep + hdr + "\n" + "".join(ln + "\n" for ln in lines)).encode("utf-8"))
        print(f"  applied {len(lines)} line(s) to {SYMS_PIN}")
    if apply and ups:  # 3.4 T2 ruling: one unsized site-corr func line per VA, rewritten in place
        done, out = set(), []
        for ln in SYMS_PIN.read_bytes().decode("utf-8").splitlines():
            f = ln.split()
            va = int(f[0], 16) if ln.strip() and not ln.startswith("#") else None
            if va in ups and f[1] == "func" and f[2] == "-" and f[-1] == "site-corr":
                if va in done:
                    print(f"name_sites: REFUSED --apply: two site-corr lines at 0x{va:08X}")
                    return 2
                done.add(va)
                ln = ups[va]
            out.append(ln)
        if done != set(ups):
            print(f"name_sites: REFUSED --apply: {len(set(ups) - done)} rewrite(s) without their line")
            return 2
        out.append(f"# 3.4 T2 name_sites --near: {len(ups)} site-corr func lines rewritten in place as sized near-corr")
        SYMS_PIN.write_bytes(("\n".join(out) + "\n").encode("utf-8"))
        print(f"  rewrote {len(ups)} line(s) in {SYMS_PIN}")
    return 0


def run_near_positive(img, pubs, procs, unit, rows_path):
    """3.4 T2 --control near-positive: every identical candidates.jsonl row of the unit (done included) passes
    near_check(protoVA, va) with the row's symbol as N. 3.4 T2 ruling: tests judgeable rows only: named = pass with the
    row's symbol; no-caller = refused no-xref (no accepted caller, abstain); wrong = anything else. Exit 0 iff wrong 0
    and named >= NEAR_POS_FLOOR."""
    ctx = near_ctx(img, pubs, procs)
    rows = sorted((r for r in load_jsonl(need(rows_path)) if r["unit"] == unit and r.get("lane", "identical") ==
                   "identical"), key=lambda r: r["va"])
    k = Counter()
    for r in rows:
        c = near_check(ctx, int(r["protoVA"], 16), r["va"])
        ok = c["verdict"] == "pass" and c["name"] == r["symbol"]
        v = "named" if ok else "no-caller" if c["verdict"] == "fail" and c["reason"] == "no-xref" else "wrong"
        k[v] += 1
        why = c["reason"] if c["verdict"] == "fail" else "pass" if ok else "name-differs"
        print(f"  {v.upper()} 0x{r['va']:08X} {r['symbol']}: {why}, sim {c['sim']}, agree {c['agree']}, "
              f"conflicts {c['conflicts']}")
    ok = k["wrong"] == 0 and k["named"] >= NEAR_POS_FLOOR
    print(f"name_sites --control near-positive --unit {unit}: {len(rows)} identical row(s): named {k['named']}, "
          f"no-caller {k['no-caller']}, wrong {k['wrong']} (judged {k['named'] + k['wrong']}, floor {NEAR_POS_FLOOR}) "
          f"-> {'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


def run_near_planted(img, pubs, procs, unit):
    """3.4 T2 --control near-planted: (1) swapped name (R2.protoVA, R1.va) of the unit's first two passing near rows;
    (2) decoy window: the first near row (VA order) whose proto_map decoy gap (gap + DECOY_SHIFT, wrap, first with
    candidates) has best_candidate >= THRESHOLD/100 -> (protoVA, decoy VA). Exit 0 iff both near_check verdicts fail."""
    ctx = near_ctx(img, pubs, procs)
    uof, _units = unit_of_va()
    rows = near_rows(ctx)
    ps = []
    for rva, pva, _r in rows:
        if uof(rva) == unit and near_check(ctx, pva, rva)["verdict"] == "pass":
            ps.append((rva, pva))
            if len(ps) == 2:
                break
    planted = []
    if len(ps) == 2:
        planted.append(("swapped name", ps[1][1], ps[0][0], None))
    else:
        print(f"  swapped name: {len(ps)} passing near row(s) in the unit, need 2")
    anchors, aprotos, ngaps, cache = ctx["anchors"], ctx["aprotos"], len(ctx["anchors"]) + 1, {}

    def gap(g):  # as proto_map.py gap(): tokens of each gap range from its start, candidate starts
        if g not in cache:
            out = []
            for a, b in proto_map.gap_ranges(anchors, g, ctx["secb"]):
                t, st, term, pad, _e = proto_map.tokens(img.rbytes(a, b - a) or b"", a, img.rlo, img.rhi)
                out.append((t, st, proto_map.candidate_starts(term, pad)))
            cache[g] = out
        return cache[g]

    def has_cand(g):
        return any(cands for _t, _s, cands in gap(g))
    for _rva, pva, _r in rows:
        g = bisect.bisect_left(aprotos, pva)
        dg = next((h % ngaps for h in range(g + proto_map.DECOY_SHIFT, g + proto_map.DECOY_SHIFT + ngaps)
                   if has_cand(h % ngaps)), None)
        if dg is None:
            continue
        ptok = proto_map.tokens(img.pbytes(pva, procs[pva]), pva, img.plo, img.phi)[0]
        dr, dva = proto_map.best_candidate(ptok, gap(dg))
        if dr is not None and dr >= proto_map.THRESHOLD / 100:
            planted.append(("decoy window", pva, dva, round(100.0 * dr, 2)))
            break
    else:
        print("  decoy window: no near row with a decoy best_candidate >= THRESHOLD")
    refused = 0
    for kind, pva, rva, dsim in planted:
        c = near_check(ctx, pva, rva)
        refused += c["verdict"] == "fail"
        print(f"  {kind}: proto 0x{pva:08X} ({c['name']}) -> retail 0x{rva:08X}: verdict {c['verdict']} {c['reason']}"
              f", sim {c['sim']}" + (f", decoy similarity {dsim}" if dsim is not None else ""))
    ok = len(planted) == 2 and refused == 2
    print(f"name_sites --control near-planted --unit {unit}: {refused} refused as expected of {len(planted)} "
          f"-> {'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--unit", help="one units.toml unit name (exact, or the .obj basename)")
    ap.add_argument("--apply", action="store_true", help="append pins.txt lines to config/retail/symbols_pin.txt")
    ap.add_argument("--rows", default=str(CAND), help="candidates jsonl (default derived/candidates/candidates.jsonl)")
    ap.add_argument("--control", choices=("positive", "planted", "planted-data", "near-positive", "near-planted"))
    ap.add_argument("--near", action="store_true",
                    help="3.4 T2: name proto_map near-identical rows (near_check + gates); --apply appends near_pins.txt")
    ap.add_argument("--doc", action="store_true", help="also write docs/matching/naming_residue.md (all rows)")
    ap.add_argument("--check-cut", action="store_true",
                    help="every pinned pool site: cut relocation at its offset names the pin; other unresolved: residue; exit 0/1")
    ap.add_argument("--promote-data", action="store_true",
                    help="3.3 T8 data byte route over site-corr data pins; --apply rewrites the passing lines in place")
    try:
        a = ap.parse_args(argv)
    except SystemExit as e:
        return 0 if e.code == 0 else 2
    if a.promote_data and (a.control or a.check_cut or a.doc):
        print("name_sites: REFUSED --promote-data takes no --control/--check-cut/--doc")
        return 2
    near_ctl = a.control in ("near-positive", "near-planted")
    if a.near and (a.unit or a.control or a.doc or a.check_cut or a.promote_data):
        print("name_sites: REFUSED --near takes only --apply")
        return 2
    if near_ctl and (a.apply or not a.unit):
        print("name_sites: REFUSED --control near-positive/near-planted take --unit U and no --apply")
        return 2
    if a.control and not near_ctl and (a.apply or a.unit):
        print("name_sites: REFUSED --control takes no --unit/--apply")
        return 2
    if (a.doc or a.check_cut) and (a.unit or a.control) or a.check_cut and (a.apply or a.doc):
        print("name_sites: REFUSED --doc/--check-cut take no --unit/--control; --check-cut runs alone")
        return 2
    try:
        rows = pool_rows(Path(a.rows))
        unit = None
        if a.unit:
            us = sorted({r["unit"] for r in (load_jsonl(need(Path(a.rows))) if near_ctl else rows)})
            hit = [u for u in us if u == a.unit] or [u for u in us if u.replace("/", "\\").split("\\")[-1].lower()
                                                     in (a.unit.lower(), a.unit.lower() + ".obj")]
            if len(hit) != 1:
                raise Refuse(f"--unit {a.unit}: {len(hit)} pool unit(s) match")
            unit = hit[0]
        if a.check_cut:
            return check_cut(rows)
        table = load_table()
        img, pubs, procs = load()
        if a.control == "positive":
            return run_positive(img, pubs, procs, table, rows)
        if a.control == "planted":
            return run_planted(img, pubs, procs, table, rows)
        if a.control == "planted-data":
            return run_planted_data(img, pubs)
        if a.control == "near-positive":
            return run_near_positive(img, pubs, procs, unit, Path(a.rows))
        if a.control == "near-planted":
            return run_near_planted(img, pubs, procs, unit)
        if a.near:
            return run_near(img, pubs, procs, table, a.apply)
        if a.promote_data:
            return run_promote(img, pubs, rows, unit, a.apply)
        cands, residue, _ps, pub_vas = derive(img, pubs, procs, table, rows)
        named = resolve(img, procs, pubs, pub_vas, table, cands, residue)
        examined, pinned2 = second_pass(img, pubs, procs, pub_vas, table, cands, residue, named)
        aliases, akept = alias(img, procs, pubs, pub_vas, table, residue, named)
        kept = None
        if a.doc:
            kept = [e for e in promote_route(img, pubs, *promote_inputs())[0] if e["verdict"] == "fail"]
    except Refuse as e:
        print(f"name_sites: REFUSED {e}")
        return 2
    residue_all = list(residue)  # 3.4 T1: residue.jsonl always holds every row's residue (--check-cut reads it, C0139)
    if unit:
        residue = [x for x in residue if x["unit"] == unit]
        named = {t: e for t, e in named.items() if unit in e["units"]}
        aliases = {t: es for t, es in aliases.items() if unit in es[0]["units"]}
    pins = [named[t] for t in sorted(named) if not named[t]["already"]]
    apins = [e for t in sorted(aliases) for e in aliases[t] if not e["already"]]
    pins += apins
    lines = [pin_line(e) for e in pins]
    OUT.mkdir(parents=True, exist_ok=True)
    with open(OUT / "names.jsonl", "w", encoding="utf-8", newline="\n") as f:
        for t in sorted(named):
            e = dict(named[t], units=sorted(named[t]["units"]))
            f.write(json.dumps(e, sort_keys=True) + "\n")
        for t in sorted(aliases):
            for e in aliases[t]:
                f.write(json.dumps(dict(e, units=sorted(e["units"])), sort_keys=True) + "\n")
    residue.sort(key=lambda x: (x["target"], x["rowVA"], x["off"]))
    residue_all.sort(key=lambda x: (x["target"], x["rowVA"], x["off"]))
    with open(OUT / "residue.jsonl", "w", encoding="utf-8", newline="\n") as f:
        for x in residue_all:
            f.write(json.dumps(x, sort_keys=True) + "\n")
    (OUT / "pins.txt").write_bytes("".join(ln + "\n" for ln in lines).encode("utf-8"))
    ke = Counter((e["kind"], e["evidence"]) for e in pins)
    rr = Counter(x["reason"] for x in residue)
    print(f"name_sites{' --unit ' + unit if unit else ''}: {len(rows)} pool rows; "
          f"{len(named)} named VA(s), {len(pins)} pin(s), {sum(e['already'] for e in named.values())} already-named")
    for k in KINDS:
        print(f"  pins {k:6}: checked {ke[(k, 'checked')]}, site-only {ke[(k, 'site-only')]}")
    print(f"  callees downgraded (all rows): callee-name-conflict {len(DOWN['callee-name-conflict'])}, "
          f"callee-short (<8 unmasked bytes, no name-agreeing mask) {len(DOWN['callee-short'])}")
    print(f"  residue sites {len(residue)} ({len({x['target'] for x in residue})} VAs): "
          + ", ".join(f"{r} {rr[r]}" for r in REASONS))
    print(f"  second pass (all rows, residue targets blocking >= {MIN_ROWS} rows): examined {examined}, "
          f"pinned {pinned2} (caller-xref), kept {examined - pinned2}")
    print(f"  fold-alias (multi-public VAs): checked {len(aliases)}, pins {len(apins)}, "
          f"already-named {sum(e['already'] for es in aliases.values() for e in es)}, kept {akept}")
    print(f"  wrote {OUT.relative_to(REPO).as_posix()}/{{names.jsonl,residue.jsonl,pins.txt}}")
    if a.doc:
        write_doc(rows, residue, examined, pinned2, kept)
        print(f"  wrote {DOC.relative_to(REPO).as_posix()}")
        write_dup_doc(residue)
        print(f"  wrote {DUP_DOC.relative_to(REPO).as_posix()}")
    if a.apply and lines:
        have = {va for va, *_ in load_table()}
        dup = [e["va"] for e in pins if int(e["va"], 16) in have]
        if dup:
            print(f"name_sites: REFUSED --apply: {len(dup)} VA(s) already named, e.g. {', '.join(dup[:4])}")
            return 2
        old = SYMS_PIN.read_bytes().decode("utf-8")
        hdr = (f"# 3.1 T6 name_sites{' --unit ' + unit if unit else ''}: {len(lines)} lines; evidence "
               f"docs/evidence/correspondence.md \"## Site-correspondence naming (3.1 T5)\"")
        sep = "" if not old or old.endswith("\n") else "\n"
        SYMS_PIN.write_bytes((old + sep + hdr + "\n" + "".join(ln + "\n" for ln in lines)).encode("utf-8"))
        print(f"  applied {len(lines)} line(s) to {SYMS_PIN}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
