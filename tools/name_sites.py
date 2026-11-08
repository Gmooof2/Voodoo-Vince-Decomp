"""Name unresolved reloc targets of pool rows by prototype site correspondence (T5, phase 3.1).

Usage: PY tools/name_sites.py [--unit U] [--apply] [--doc] [--rows P] [--control positive|planted] | --check-cut
       | --near [--apply] | --control near-positive|near-planted --unit U | --control near-callee-planted
       | --provisional [--check] | --control prov-drop|prov-false-unblocked
       | --ambiguous bracket|fold-alias|fold-k1 [--max N] [--apply]
       | --control bracket-positive|bracket-planted|bracket-ambiguous|fold-alias-planted|fold-k1-planted
       | --control bracket-own-dtor | --recheck bracket
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
  the pin (3.2 T4); every other unresolved site's target is in residue.jsonl or the site is in ACCEPTED_SITES (4.4 T18,
  counted by reason); a listed site that no longer fails prints STALE; exit 0 all hold, 1 FAIL/STALE lines.
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
--control near-callee-planted (4.2 T1): wrong names on called functions (look-alike, sibling, forged, moved), all near
  rows; see run_near_callee_planted. Writes nothing.
--promote-corr [--bss] [--apply] (4.2 T7, D113): the pin file's site-corr rows retagged in a copy under .run/<phase>/promote_corr/
  (data ??_7 -> +site-corr+slot-bytes corr-verified; __TI and other non-literal data with raw retail bytes ->
  +site-corr+site-bundle corr-verified; bss data only with --bss; ??_C@/__real@ never; func -> +site-corr+callee-bytes
  disasm-verified, size = PDB proc length; 4.2 T9, D115: a func whose proto VA passes near_check at its VA with a
  contiguous Ghidra extent, no table start inside -> +anchor-window+caller-xref near-corr, size = Ghidra size, route
  near), replanted once through pin_rows (the locked checker); a candidate passes
  when its row has no fails; every other row must equal the real file's state (else refusal). Writes
  derived/name_sites/corr.jsonl; --apply rewrites the passing lines in place plus one `# <phase> <T<n>>` comment line (4.3 T15: phase
  from the plan heading as .run/<phase>, task from the required --task T<n>; was the literal `# 4.2 T7`). --doc
  adds "Pins not promoted (4.2 T7, for T11)" when corr.jsonl exists. --control planted-corr: names swapped between
  two passing slots, bundle, func and near rows (where 2 exist), replanted: every planted row must be refused. Writes nothing.
--provisional [--check] (4.2 T11): docs/matching/provisional_residue.md from the frozen start set
  docs/matching/provisional_start.md (484 rows): each row once, Unblocked (every start target accepted now:
  candidates.accepted over the pin tables, func VA or the data line at target - member offset) or Blocked under its
  primary blocking target with the evidence searched (corr.jsonl, near.jsonl, residue.jsonl, apply.jsonl); buckets by
  pool_buckets.assign; figures vs T2 (484 start, 585 name-only supply, 719 census rows blocked). Reads residue.jsonl as
  the last run left it (run `--min-rows 1` dry first). --check parses the doc back: exit 1 when a start row is in
  neither/both sections, an Unblocked row has a target not accepted, or a Blocked row has none blocking.
  --control prov-drop (one parsed row dropped) / prov-false-unblocked (one Blocked row moved): exit 0 only when the
  check fails. Pin tables only; no cut.
--ambiguous CLS [--max N] [--apply] (5.3 T3, D128-D130): the recounted ambiguous rows (pool_buckets.ruled_state, C0366)
  settled by pool_buckets' bracket / fold_alias / fold_k1 predicate, in order unit of the body then VA; each pair through
  the writer's chain (fold-k1: at an already-named body; the publics not yet at the body; callee bytes, D114), then one
  pin_rows replant (a line whose row fails or moves an existing row at its VA is dropped: the disagreement count).
  Lines (bracket +unit-bracket+callee-bytes, fold-* +fold-alias+callee-bytes, one per public, C0130) to
  .run/<phase>/t3/ambiguous_<CLS>.txt; --apply appends them to the pin file. --control bracket-positive|bracket-planted|
  bracket-ambiguous|fold-alias-planted|fold-k1-planted: the writer's chain on one pair; planted exit 0 only when refused.
  T13.2, bracket only (check() unchanged): dtor-not-own (a ??_G/??_E public whose class has a table ??1<Class>@@ at X,
  no retail rel call/jmp out of the body reaching X), callee-down (a rel dest out of the body unnamed in the table whose
  proto dest at the same mask is unsized or not masked-equal). --ambiguous bracket also writes
  .run/<phase>/t3/ambiguous_bracket_siblings.txt (kept bodies with masked-identical proto rows of the same module).
  --control bracket-own-dtor: ??_GPropellerMain at 0x000A3EC9, its pin lines removed in memory; exit 0 when refused.
  --recheck bracket: the pin file's `# 5.3 T3 name_sites --ambiguous bracket` block, each body re-run through the
  chain with its own lines removed; prints the newly refused bodies (VA, name, reason) and a count; exit 0.
Exit: 0 ok; 1 control or --check-cut failed; 2 refusal (missing input, retail sha mismatch, bad args, --apply VA already
  named, --promote-data with --control/--check-cut/--doc, --promote-data --apply without one data site-corr line per VA).
"""
import argparse
import bisect
import contextlib
import json
import os
import re
import struct
import subprocess
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
        # 3.4 T2: near-corr names never agree. Unchanged by 4.2 T9 (D115): this is byte-route evidence for promotion
        # (mirrors locked pin_rows.py callee); near-corr is accepted at candidates.accepted, never promoted from here.
        rn = {x for x in tab_va.get(rd, ()) if (rd, x) not in NEAR_CORR}
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
    if via == "caller-xref":  # 4.2 T10: pin_rows TAGS has no caller-xref tag; the name rests on a site correspondence
        via = "site-corr"     # (proto_map rows, C0059); --apply keeps it only checked and replant-passing (apply_replant)
    if e["evidence"] == "checked" and e["kind"] in SRC_CHECK:  # data has no byte check (3.2 T4): never disasm-verified
        src, st = f"proto-pdb-public+{via}+{SRC_CHECK[e['kind']]}", "disasm-verified"
    elif e["evidence"] == "near" and k == "func":  # T13.2.c3 --ambiguous D115: near_check pass (D115, pin_rows TAGS)
        src, st = NEAR_SRC, "near-corr"
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
    if CORR.is_file():
        cs = load_jsonl(CORR)
        cf = [e for e in cs if e["verdict"] == "fail"]
        rv = Counter((e["route"], e["verdict"]) for e in cs)
        cr = Counter(e["reason"] for e in cf)
        out += ["", "## Pins not promoted (4.2 T7, for T11)", "",
                f"`--promote-corr` route (pin_rows corr-verified check, D113): {len(cs)} candidate(s), {len(cf)} not"
                " promoted.", ""]
        out += [f"- {r}: pass {rv[(r, 'pass')]}, fail {rv[(r, 'fail')]}" for r in CORR_ROUTES]
        out += [""] + [f"- {r}: {cr[r]}" for r in sorted(cr)]
        out += ["", "`VA · name · kind · route · reason`.", ""]
        out += [f"- {e['va']} · {safe(e['name'])} · {e['kind']} · {e['route']} · {e['reason']}" for e in cf]
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


# 4.4 T18: accepted `neither pinned nor residue` sites, keyed (row VA, site offset, target VA) -> reason key; reason ->
# (reason, commit, pointer, mark). A listed site that no longer fails that way is STALE (exit 1); any other site at a listed
# target fails. 5.1 T8 (O10): each pointer is checked (`_pointer_fails`): the commit is an ancestor of HEAD and the cited
# `path:line` still contains `mark` (the DISABLED note's prefix, or the validation.md phrase recording the site); else POINTER.
# 5.3 T17: in a public tree the commit check is skipped and an unexported cited path is skipped; shipped marks still checked.
ACCEPTED_REASONS = {
    # puddle commit was 6bdf5e1c (the ScareHouse cut commit before it); b5437fdd1 added the DISABLED note (R70, R4.5-013)
    "puddle": ("Puddle _Tidy pin disabled (4.4 T6.1)", "b5437fdd1", "config/retail/symbols_pin.txt:3274",
               "# DISABLED: Puddle _Tidy pin"),
    "whammy": ("Whammy _Tree::insert pin disabled (4.4 T7.1 B1)", "22237cf8f", "config/retail/symbols_pin.txt:3421",
               "# DISABLED: Whammy _Tree::insert pin"),
    "union": ("SwampFireFlyNode AABox::Union pin disabled (4.4 T7.1 B1)", "22237cf8f",
              "config/retail/symbols_pin.txt:3547", "# DISABLED: SwampFireFlyNode AABox::Union pin"),
    "font": ("FontManager nested-map insert pin disabled, name over Ghidra's 2000-char limit (4.4 T7.1 B3)", "53ba2d16a",
             "config/retail/symbols_pin.txt:3678", "# DISABLED: FontManager nested-map insert pin"),
    "sethealth": ("?SetHealth@Critter@@QAEXH@Z second-pass site-only name dropped at the T7.1 B1 replant, not pinned"
                  " (site-only evidence is not a pin route)", "22237cf8f", "docs/ops/validation.md:1976",
                  "`?SetHealth@Critter@@QAEXH@Z`, second-pass site-only name dropped at replant"),
    "indexdata": ("_D3D__IndexData caller-xref second-pass pin not applied (4.4 T7.1 B3)", "53ba2d16a",
                  "docs/ops/validation.md:2002", "`_D3D__IndexData`, caller-xref second-pass pin not applied"),
    "bbl": ("??1BundleBackgroundLoader pin disabled (5.3 T4)", "63ce24c26", "config/retail/symbols_pin.txt:4862",  # was :4858 (T13.2 DISABLED lines)
            "# DISABLED: ??1BundleBackgroundLoader pin"),
    # 5.3 T13.2.c5: _Init@_Tree<ZoneNode> 0x000C2038+0x4 calls the DISABLED _Buynode fold pair 0x000C1C27 (T4 R8)
    "buynode": ("_Buynode fold pair pin disabled (5.3 T4)", "63ce24c26", "config/retail/symbols_pin.txt:4807",
                "# DISABLED: _Buynode fold pair 0x000C1C27"),
    # 5.3 T13.2: vector<ResourceHandle<TextureResource>> copy ctor 0x000C7FA1 is pinned at the Ghidra size 0x6A (true
    # length 0x7C, candidates bodymax); its call at +0x6E to the pinned _Tidy fold 0x000C7B97 lies past the pin, so the
    # delinker emits no REL32 there (C0131). Accepted only while the cut has no relocation at the site (check_cut).
    "vecsize": ("site past the 0x6A pin of 0x000C7FA1 (Ghidra size; true length 0x7C), no relocation (5.3 T13.2)",
                "e1f8466f5", "config/retail/symbols_pin.txt:3536", "0x000C7FA1 func 0x6A"),
    # 5.2 T3 (R8) onion, hudui, music, signtrolley, critter (commit 320e85770) removed at 5.2 T6.c4: every site STALE,
    # its target now pinned by the T6 pin pass (DISABLED notes stay in symbols_pin.txt)
}


def _pointer_fails(key):
    """5.1 T8: [] when reason `key`'s commit is in HEAD's history and its cited line still carries its mark."""
    _why, commit, ptr, mark = ACCEPTED_REASONS[key]
    out = []
    # 5.3 T17: a public tree (config/PUBLIC_TREE, no .claude/pa.json, as gate.py decides) is exported as a fresh history
    # without the private commits and without docs/ops/validation.md; there the ancestor check cannot hold and an absent
    # cited path is not exported, so both are skipped. A cited path present in the tree (symbols_pin.txt) is still checked.
    public = (REPO / "config" / "PUBLIC_TREE").is_file() and not (REPO / ".claude" / "pa.json").is_file()
    if not public:
        rc = subprocess.run(["git", "merge-base", "--is-ancestor", commit, "HEAD"], cwd=REPO,
                            capture_output=True).returncode
        if rc != 0:
            out.append(f"POINTER {key}: commit {commit} not in HEAD's history (git merge-base rc {rc})")
    path, _, ln = ptr.rpartition(":")
    if public and not (REPO / path).is_file():
        return out
    lines = (REPO / path).read_text(encoding="utf-8").splitlines() if (REPO / path).is_file() else []
    if not (ln.isdigit() and 0 < int(ln) <= len(lines) and mark in lines[int(ln) - 1]):
        out.append(f"POINTER {key}: {ptr} does not carry {mark!r}")
    return out
ACCEPTED_SITES = {
    (0x0012BC8C, 0x7, 0x0012B64E): "puddle",
    (0x000D3ABA, 0x15, 0x001672D4): "indexdata",
    (0x000C86FF, 0x14F, 0x000DDEF3): "union",
    (0x000FF37C, 0xF0, 0x000FEF28): "font",
    (0x000DAC88, 0x4, 0x000DAA98): "bbl",
    (0x000C2038, 0x4, 0x000C1C27): "buynode",  # 5.3 T13.2.c5; was 22 accepted sites, now 23
    (0x000C7FA1, 0x6E, 0x000C7B97): "vecsize",  # 5.3 T13.2; was 23 accepted sites, now 24 (pinned-target branch)
    **{(0x00135939, o, 0x000380F3): "whammy" for o in (0x90, 0xF0, 0x14D, 0x1AA, 0x207, 0x264, 0x2C1, 0x31E, 0x37B,
                                                       0x3D8, 0x435, 0x492, 0x4EF, 0x54C)},
    (0x000196CA, 0x156, 0x00018110): "sethealth",
    (0x000F151C, 0x19E, 0x00018110): "sethealth",
    (0x000F1933, 0x1A1, 0x00018110): "sethealth",
}


def check_cut(rows):
    """Every pool site whose target is pinned in symbols_pin.txt, resolved or not (3.2 T4): the cut object has a relocation
    at the site naming one of the target's table names; every other unresolved site: residue, or ACCEPTED_SITES (4.4 T18)."""
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
    acc = defaultdict(list)
    for r in rows:
        for s in r["sites"]:
            t, where = s["target"], f"{r['symbol']} 0x{r['va']:08X} +0x{s['off']:X} -> 0x{s['target']:08X}"
            if t in pinned:
                nm = cut_name(cut, objs, r["unit"], r["symbol"], s["off"])
                if nm in names[t] or nm and nm.rpartition("::")[2] in names[t]:  # Ghidra library namespace (XAPILIB::)
                    npin += 1
                elif nm is None and (r["va"], s["off"], t) in ACCEPTED_SITES:  # 5.3 T13.2: no relocation at a listed site
                    acc[ACCEPTED_SITES[(r["va"], s["off"], t)]].append((r["va"], s["off"], t))
                else:
                    fails.append(f"FAIL {where} {s['kind']}: pinned {sorted(names[t])}, cut relocation names {nm}")
            elif s["resolved"] is not None:
                continue
            elif t in res_t:
                nres += 1
            elif (r["va"], s["off"], t) in ACCEPTED_SITES:
                acc[ACCEPTED_SITES[(r["va"], s["off"], t)]].append((r["va"], s["off"], t))
            else:
                fails.append(f"FAIL {where}: neither pinned nor residue")
    seen = {k for ks in acc.values() for k in ks}
    for k in sorted(set(ACCEPTED_SITES) - seen):
        fails.append(f"STALE 0x{k[0]:08X} +0x{k[1]:X} -> 0x{k[2]:08X}: {ACCEPTED_REASONS[ACCEPTED_SITES[k]][0]}")
    for key in ACCEPTED_REASONS:  # 5.1 T8 (O10): pointers checked, not just printed
        fails += _pointer_fails(key)
    print(f"name_sites --check-cut: {len(rows)} pool rows; pinned sites cut-named {npin}, unresolved residue {nres}, "
          f"accepted {len(seen)}, FAIL {len(fails)}")
    for key in ACCEPTED_REASONS:
        if acc[key]:
            print(f"  accepted {len(acc[key])}: {ACCEPTED_REASONS[key][0]} (commit {ACCEPTED_REASONS[key][1]},"
                  f" {ACCEPTED_REASONS[key][2]})")
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


CORR = OUT / "corr.jsonl"  # 4.2 T7 --promote-corr
CORR_TMP = byte_compare._phase_run_dir() / "promote_corr"  # was REPO / ".run" / "4.2" / "t7" (4.2 T13)
CORR_ROUTES = ("slots", "bundle", "func", "near")  # was ("slots", "bundle", "func"); 4.2 T9 adds near (D115)
CORR_SRC = {"slots": "proto-pdb-public+site-corr+slot-bytes corr-verified",  # D113 (pin_rows TAGS)
            "bundle": "proto-pdb-public+site-corr+site-bundle corr-verified",
            "func": "proto-pdb-public+site-corr+callee-bytes disasm-verified",
            "near": "proto-pdb-public+anchor-window+caller-xref near-corr"}  # D115 (= NEAR_SRC)


def corr_near(st):
    """4.2 T9 (D115): {va: size} of site-corr func rows where near_check(one of the row's proto VAs, va) passes and the
    Ghidra function at va is contiguous with no table start inside (as --near's extent gates); size = its Ghidra size."""
    table = load_table()
    img, pubs, procs = load()
    ctx = near_ctx(img, pubs, procs)
    gf = {int(g["entry"], 16): (g["size"], int(g["bodyMax"], 16)) for g in load_jsonl(need(
        REPO / "derived" / "ghidra" / f"retail-{img.pin[:8]}" / "applied" / "functions.jsonl"))}
    tstarts = sorted({va for va, *_ in table})
    out = {}
    for r in st.rows:
        if r.get("status") != "site-corr" or r.get("kind") != "func" or r.get("tag") != "proto-pdb-public+site-corr":
            continue
        va, g = r["va"], gf.get(r["va"])
        if not any(near_check(ctx, p, va)["verdict"] == "pass" for p in r.get("protos", ())) or g is None \
                or g[0] != g[1] - va + 1 or any(va < s < va + g[0] for s in tstarts):
            continue
        out[va] = g[0]
    return out


def corr_state():
    """4.2 T7: pin_rows' state over SYMS_PIN, its lines, and the site-corr candidates (bss included, kind "bss")."""
    import pin_rows
    try:
        st = pin_rows.state()
        if SYMS_PIN.resolve() != pin_rows.PIN.resolve():
            st = st.replant(SYMS_PIN)
    except pin_rows.Refuse as e:
        raise Refuse(f"pin_rows: {e}")
    lines = SYMS_PIN.read_bytes().decode("utf-8").split("\n")
    near = corr_near(st)  # 4.2 T9 (D115)
    cands = []
    for r in st.rows:
        if r["status"] != "site-corr" or r["tag"] != "proto-pdb-public+site-corr":
            continue
        n, va, size = r["name"], r["va"], None
        if r["kind"] == "func" and va in near:  # 4.2 T9: near_check evidence -> near-corr, pin_rows decides
            route, kind, size = "near", "func", near[va]
        elif r["kind"] == "func":
            route, kind = "func", "func"
            size = next((st.procs[p] for p in r["protos"] if st.procs.get(p)), None)
        elif n.startswith(("??_C@", "__real@")):
            continue
        else:
            route = "slots" if n.startswith("??_7") else "bundle"
            kind = ("bss" if st.img.rbytes(va, 1) is None else "vtable" if route == "slots"
                    else "typeinfo" if n.startswith("__TI") else "global")
        f = lines[r["line"] - 1].split()
        if not f or int(f[0], 16) != va or f[3] != n:
            raise Refuse(f"--promote-corr: pin line {r['line']} is not 0x{va:08X} {n}")
        cands.append({"va": va, "name": n, "kind": kind, "route": route, "line": r["line"], "rk": r["kind"],
                      "size": size})
    return st, lines, cands


def corr_text(c, name=None):
    sz = f"0x{c['size']:X}" if c["size"] else "-"
    return f"0x{c['va']:08X} {c['rk']} {sz} {name or c['name']} {CORR_SRC[c['route']]}"


def corr_key(r):
    return (r.get("va"), r.get("name"), r.get("kind"), r.get("status"), r.get("tag"), tuple(r["fails"]),
            tuple(r["notes"]), r.get("site"))


def corr_replant(st, lines, texts, tag, strict=True):
    """Copy of the pin file with {line: text} retagged, replanted through pin_rows -> {line: row}. Refuse when a row
    not in texts differs from the real file's state (strict; a planted name may legitimately move other rows' results:
    then the count is printed)."""
    import pin_rows
    out = list(lines)
    for i, t in texts.items():
        out[i - 1] = t
    CORR_TMP.mkdir(parents=True, exist_ok=True)
    p = CORR_TMP / f"{tag}.txt"
    p.write_bytes("\n".join(out).encode("utf-8"))
    try:
        s = st.replant(p)
    except pin_rows.Refuse as e:
        raise Refuse(f"pin_rows: {e}")
    finally:
        p.unlink()
        try:
            CORR_TMP.rmdir()
        except OSError:
            pass
    by = {r["line"]: r for r in s.rows}
    diff = [r["line"] for r in st.rows if r["line"] not in texts and (r["line"] not in by
                                                                      or corr_key(r) != corr_key(by[r["line"]]))]
    if not strict and len(by) == len(st.rows):
        print(f"  {len(diff)} non-candidate row(s) changed by the plants")
    elif diff or len(by) != len(st.rows):
        raise Refuse(f"--promote-corr: {len(diff)} non-candidate row(s) changed by the retag, e.g. lines "
                     + ", ".join(map(str, diff[:4])))
    return by


def apply_replant(pins, lines):
    """4.2 T10 (C0193, C0194): the new pin lines appended to a copy of the pin file, replanted through pin_rows (the
    locked checker decides). A line is kept when its row has no fails and, for a caller-xref pin, its byte check passed
    (site-only: no byte route, T10 brief); repeated until the kept set is stable. Refuse when any existing row's fails
    change. -> (kept lines, {line: why})."""
    import pin_rows
    try:
        st = pin_rows.state()
        if SYMS_PIN.resolve() != pin_rows.PIN.resolve():
            st = st.replant(SYMS_PIN)
    except pin_rows.Refuse as e:
        raise Refuse(f"pin_rows: {e}")
    base = SYMS_PIN.read_bytes().decode("utf-8")
    base += "" if not base or base.endswith("\n") else "\n"
    nb = len(base.split("\n")) - 1  # the appended lines start at nb + 1
    drop = {ln: "site-only (no byte check)" for e, ln in zip(pins, lines)
            if e.get("via") == "caller-xref" and e["evidence"] != "checked"}
    while True:
        keep = [ln for ln in lines if ln not in drop]
        if not keep:
            return [], drop
        CORR_TMP.mkdir(parents=True, exist_ok=True)
        p = CORR_TMP / "apply.txt"
        p.write_bytes((base + "".join(ln + "\n" for ln in keep)).encode("utf-8"))
        try:
            s = st.replant(p)
        except pin_rows.Refuse as e:
            raise Refuse(f"pin_rows: {e}")
        finally:
            p.unlink()
            try:
                CORR_TMP.rmdir()
            except OSError:
                pass
        by = {r["line"]: r for r in s.rows}
        moved = [r["line"] for r in st.rows if r["line"] not in by or r["fails"] != by[r["line"]]["fails"]]
        if moved:
            raise Refuse(f"--apply: {len(moved)} existing pin row(s) change result, e.g. lines "
                         + ", ".join(map(str, moved[:4])))
        bad = {}
        for i, ln in enumerate(keep, nb + 1):
            r = by.get(i)
            if r is None or r.get("va") != int(ln.split()[0], 16):
                raise Refuse(f"--apply: replanted line {i} is not {ln.split()[0]}")
            if r["fails"]:
                bad[ln] = ";".join(r["fails"])
        if not bad:
            return keep, drop
        drop.update(bad)


def run_promote_corr(bss, apply, task=None):
    st, lines, cands = corr_state()
    cands = [c for c in cands if bss or c["kind"] != "bss"]
    by = corr_replant(st, lines, {c["line"]: corr_text(c) for c in cands}, "promote")
    ents = []
    for c in cands:
        r = by[c["line"]]
        if r.get("va") != c["va"] or r.get("name") != c["name"]:
            raise Refuse(f"--promote-corr: replanted line {c['line']} is not 0x{c['va']:08X}")
        ents.append({"va": f"0x{c['va']:08X}", "name": c["name"], "kind": c["kind"], "route": c["route"],
                     "verdict": "fail" if r["fails"] else "pass", "reason": ";".join(r["fails"]) or None})
    OUT.mkdir(parents=True, exist_ok=True)
    with open(CORR, "w", encoding="utf-8", newline="\n") as f:
        for e in ents:
            f.write(json.dumps(e, sort_keys=True) + "\n")
    rv = Counter((e["route"], e["verdict"]) for e in ents)
    rc = Counter((e["route"], e["reason"]) for e in ents if e["reason"])
    print(f"name_sites --promote-corr{' --bss' if bss else ''}: {len(ents)} candidate(s); pass "
          f"{sum(rv[(r, 'pass')] for r in CORR_ROUTES)}, fail {sum(rv[(r, 'fail')] for r in CORR_ROUTES)}")
    print("  routes: " + ", ".join(f"{r} pass {rv[(r, 'pass')]} fail {rv[(r, 'fail')]}" for r in CORR_ROUTES))
    for (r, why), k in sorted(rc.items()):
        print(f"  reason {r}: {why} {k}")
    print(f"  wrote {CORR.relative_to(REPO).as_posix()}")
    if apply:
        good = [c for c, e in zip(cands, ents) if e["verdict"] == "pass"]
        if good:
            out = list(lines)
            for c in good:
                out[c["line"] - 1] = corr_text(c)
            text = "\n".join(out)
            sep = "" if text.endswith("\n") else "\n"
            run = f"{byte_compare._phase_run_dir().name} {task}"  # was the literal `# 4.2 T7` (4.3 T15): the run's phase, --task
            text += (sep + f"# {run} name_sites --promote-corr{' --bss' if bss else ''}: {len(good)} lines promoted in "
                     "place; checker tools/pin_rows.py corr-verified / callee route (D113), near route (D115, 4.2 T9)\n")
            SYMS_PIN.write_bytes(text.encode("utf-8"))
        print(f"  promoted {len(good)} line(s) in place in {SYMS_PIN}")
    return 0


def run_planted_corr(bss=False):
    st, lines, cands = corr_state()
    cands = [c for c in cands if bss or c["kind"] != "bss"]  # 4.2 T8: --bss plants the bss bundle route too
    texts = {c["line"]: corr_text(c) for c in cands}
    by = corr_replant(st, lines, texts, "planted-base")
    good = [c for c in cands if not by[c["line"]]["fails"]]
    plants = []
    for route in CORR_ROUTES:
        g = [c for c in good if c["route"] == route]
        if len(g) < 2:
            print(f"  {route}: {len(g)} passing row(s), no pair planted")
            continue
        a, b = g[0], g[1]
        plants += [(a, b["name"]), (b, a["name"])]
    if not plants:
        print("name_sites --control planted-corr: no pair to plant -> FAIL")
        return 1
    for c, n in plants:
        texts[c["line"]] = corr_text(c, n)
    by = corr_replant(st, lines, texts, "planted", strict=False)
    leaks = 0
    for c, n in plants:
        r = by[c["line"]]
        leaks += not r["fails"]
        print(f"  {c['route']}: VA 0x{c['va']:08X} ({safe(c['name'])}) planted {safe(n)}: "
              + (";".join(r["fails"]) or "NOT REFUSED"))
    ok = leaks == 0
    print(f"name_sites --control planted-corr: {len(plants)} planted row(s), {leaks} not refused -> "
          f"{'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


NEAR_SRC ="proto-pdb-public+anchor-window+caller-xref"  # 3.4 T2 near-corr pin source
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


def run_near_callee_planted(img, pubs, procs):
    """4.2 T1 --control near-callee-planted (3.5 T7's missing control; near-planted never reaches the caller piece): wrong
    names on called functions, all near rows. Truth = a near row near_check passes (accepted callers agree on it).
    look-alike: another public of the row's proto gap at the row's VA; those that clear window and similarity must be
    refused by the caller piece alone. sibling: a different callee of the same accepted caller at the row's VA. forged:
    one conflicting caller injected in memory on each side. moved: every real site whose masked-equal caller's proto
    target public differs from the accepted names at the retail target (not a fold; `_X@4` and `X` are one name: library
    rows of symbols.txt are undecorated). forged agree: a caller claiming a look-alike at a VA that has a true caller.
    Exit 0 iff look-alike, sibling and forged are non-empty and every case is refused (look-alike at no-xref or
    xref-conflict; forged and forged agree at xref-conflict). Limit, by reading: a callee whose only accepted caller's
    retail target moved has nothing left to conflict with; `moved` counts how often that shape exists in the tree."""
    import candidates
    import units_seed
    ctx = near_ctx(img, pubs, procs)
    folds = {int(e["retailVA"], 16) for e in load_jsonl(need(FOLDS))}
    aprotos = ctx["aprotos"]
    passing = [(rva, pva) for rva, pva, _r in near_rows(ctx) if near_check(ctx, pva, rva)["verdict"] == "pass"]
    pvas = sorted(p for p in procs if len(pubs.get(p, ())) == 1)
    by_caller = defaultdict(set)
    for pd, sites in ctx["by_pd"].items():
        for rd, cva in sites:
            by_caller[cva].add((pd, rd))
    k, leaks = Counter(), []

    def plant(kind, pva, rva, want=None):
        c = near_check(ctx, pva, rva)
        k[f"{kind} {c['reason']}"] += 1
        bad = c["verdict"] == "pass" or (want is not None and c["reason"] not in want)
        if bad:
            leaks.append(f"{kind}: proto 0x{pva:08X} ({safe(c['name'] or '-')}) -> retail 0x{rva:08X}: {c['verdict']} "
                         f"{c['reason']}, sim {c['sim']}, agree {c['agree']}, conflicts {c['conflicts']}")
        return c
    XREF = ("no-xref", "xref-conflict")
    n_look = n_sib = 0
    forged_at = None
    for rva, pva in passing:
        g = bisect.bisect_left(aprotos, pva)
        lo, hi = aprotos[g - 1] if g else 0, aprotos[g] if g < len(aprotos) else 1 << 32
        n = procs[pva]
        for w in pvas[bisect.bisect_right(pvas, lo):bisect.bisect_left(pvas, hi)]:
            if w == pva or not 0.4 * n <= procs[w] <= 2.5 * n:
                continue
            c = near_check(ctx, w, rva)
            if c["reason"] in ("similarity-below", "outside-window", "not-func"):
                k["look-alike screened"] += 1
                continue
            n_look += 1
            plant("look-alike", w, rva, XREF)
            if forged_at is None:
                forged_at = (w, rva, next(c for _pd, c in ctx["by_rd"][rva] if c != rva))
        for _pd, cva in ctx["by_rd"].get(rva, ()):
            for pd2, rd2 in sorted(by_caller[cva]):
                if pd2 != pva and rd2 != rva and len(pubs.get(pd2, ())) == 1:
                    n_sib += 1
                    plant("sibling", pd2, rva)
    forged = 0
    if passing:
        rva, pva = passing[0]
        cva = next(c for _rd, c in ctx["by_pd"][pva] if c != rva)
        for key, side, val in (("by_pd", pva, (rva + 0x10, cva)), ("by_rd", rva, (pva + 0x10, cva))):
            by = dict(ctx[key])
            by[side] = list(by.get(side, ())) + [val]
            c = near_check(dict(ctx, **{key: by}), pva, rva)
            forged += c["reason"] == "xref-conflict"
            print(f"  forged conflict {key}: proto 0x{pva:08X} -> retail 0x{rva:08X}: {c['verdict']} {c['reason']}, "
                  f"agree {c['agree']}, conflicts {c['conflicts']}")
    if forged_at:
        w, rva, cva = forged_at
        by = dict(ctx["by_pd"])
        by[w] = list(by.get(w, ())) + [(rva, cva)]
        c = near_check(dict(ctx, by_pd=by), w, rva)
        forged += c["reason"] == "xref-conflict"
        print(f"  forged agree: proto 0x{w:08X} -> retail 0x{rva:08X}: {c['verdict']} {c['reason']}, agree {c['agree']}, "
              f"conflicts {c['conflicts']}")

    def bare(n):
        return re.sub(r"@\d+$", "", n).lstrip("_")
    names = defaultdict(set)
    for tab, ok in ((SYMS, units_seed.STATUSES), (SYMS_PIN, candidates.PIN_STATUSES)):
        for ln in need(tab).read_text(encoding="utf-8").splitlines():
            if ln.strip() and not ln.startswith("#"):
                f = ln.split()
                if f[1] == "func" and f[-1] in ok:
                    names[int(f[0], 16)].add(bare(" ".join(f[3:-2])))
    agree, moved = 0, set()
    for pd, sites in ctx["by_pd"].items():
        ns = pubs.get(pd, set())
        for rd, _c in sites:
            if len(ns) != 1 or not names.get(rd):
                continue
            if bare(next(iter(ns))) in names[rd]:
                agree += 1
            elif rd in folds:
                k["moved fold-excluded"] += 1
            else:
                moved.add((pd, rd))
    for pd, rd in sorted(moved):
        plant("moved", pd, rd)
    for t in sorted(k):
        print(f"  {t}: {k[t]}")
    for ln in leaks[:40]:
        print(f"  LEAK {ln}")
    ok = n_look > 0 and n_sib > 0 and forged == 3 and not leaks
    print(f"name_sites --control near-callee-planted: {len(passing)} passing callee row(s); caller sites naming the "
          f"accepted name {agree}, moved {len(moved)}; planted look-alike {n_look}, sibling {n_sib}, forged {forged} of 3, "
          f"moved {len(moved)}; {len(leaks)} not refused -> {'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


PROV_START = REPO / "docs" / "matching" / "provisional_start.md"  # 4.2 T2 frozen start set (484 rows)
PROV_DOC = REPO / "docs" / "matching" / "provisional_residue.md"  # 4.2 T11 --provisional
PROV_ROW = re.compile(r"^\s*- (\d+) (0x[0-9A-F]{8}) ")
T2_SUPPLY = (585, 142473)  # 4.2 T2: reloc-provisional 484 + reloc-data 93 + reloc-code 8, bytes
T2_BLOCKED = 719  # 4.2 T2: census "Rows blocked" pool
NAME_ONLY = ("reloc-provisional", "reloc-data", "reloc-code")


def prov_start():
    """-> [(rank, va, name, [(target, owner, off, status, cls)])] from the frozen start set; owner as written there."""
    out = []
    for ln in need(PROV_START).read_text(encoding="utf-8").splitlines():
        if not re.match(r"^- \d+ 0x", ln):
            continue
        head, _, rest = ln[2:].partition(" | ")
        rk, va, name = head.split(" ", 2)
        ts = []
        for e in rest.split("; "):
            p = e.split(" ")
            owner = " ".join(p[1:-2])
            m = re.search(r"\+0x([0-9A-F]+)$", owner)
            ts.append((int(p[0], 16), owner, int(m.group(1), 16) if m else 0, p[-2], p[-1]))
        out.append((int(rk), int(va, 16), name, ts))
    if len(out) != 484 or len({(r[0], r[1]) for r in out}) != 484:
        raise Refuse(f"{PROV_START.name}: {len(out)} rows, expected 484 distinct")
    return out


def prov_table():
    """-> ({va: (kind, status, line)} symbols.txt then symbols_pin.txt (pin wins, as candidates.load_symbols), sorted VAs)."""
    tab = {}
    for p in (SYMS, SYMS_PIN):
        for ln in need(p).read_text(encoding="utf-8").splitlines():
            if ln.strip() and not ln.startswith("#"):
                f = ln.split()
                tab[int(f[0], 16)] = (f[1], f[-1], ln)
    return tab, sorted(tab)


def prov_target(t, off, tab, vas, funcs, accepted):
    """-> (accepted bool, pin line | None, status | None): a func-table VA, else the data line at t - off with no pinned
    VA in (t - off, t] (the start set's owner extent, 3.4 T16 members); status through candidates.accepted."""
    if t in funcs:
        k, st, ln = tab[t]
        return accepted(st, t, funcs), ln, st
    g = t - off
    if g in tab and tab[g][0] != "func" and bisect.bisect_right(vas, t) - 1 == bisect.bisect_left(vas, g):
        k, st, ln = tab[g]
        return accepted(st, t, funcs), ln, st
    return False, None, None


def prov_accept():
    import candidates
    funcs = candidates.load_symbols()[0]
    tab, vas = prov_table()
    return lambda t, off: prov_target(t, off, tab, vas, funcs, candidates.accepted)


def prov_parse(text):
    """-> (unblocked [(rank, va)], blocked [(rank, va)]) from the doc's ## Unblocked and ## Blocked sections."""
    sec, u, b = None, [], []
    for ln in text.splitlines():
        if ln.startswith("## "):
            sec = ln[3:].strip()
            continue
        m = PROV_ROW.match(ln)
        if m and sec in ("Unblocked", "Blocked"):
            (u if sec == "Unblocked" else b).append((int(m.group(1)), int(m.group(2), 16)))
    return u, b


def prov_check(start, u, b, acc):
    """-> FAIL lines: a start row in neither or both sections (or twice), a doc row outside the start set, an Unblocked
    row with a target not accepted now, a Blocked row with every target accepted."""
    fails = []
    cu, cb = Counter(u), Counter(b)
    keys = {(r[0], r[1]): r for r in start}
    for k in sorted(set(cu) | set(cb)):
        if k not in keys:
            fails.append(f"rank {k[0]} 0x{k[1]:08X}: not a start row")
    for k, r in sorted(keys.items()):
        n = cu[k] + cb[k]
        if n != 1:
            fails.append(f"rank {k[0]} 0x{k[1]:08X}: in {n} places (Unblocked {cu[k]}, Blocked {cb[k]})")
            continue
        bad = [t for t, _o, off, _s, _c in r[3] if not acc(t, off)[0]]
        if cu[k] and bad:
            fails.append(f"rank {k[0]} 0x{k[1]:08X}: Unblocked with target(s) not accepted "
                         + ", ".join(f"0x{t:08X}" for t in bad))
        if cb[k] and not bad:
            fails.append(f"rank {k[0]} 0x{k[1]:08X}: Blocked with every target accepted")
    return fails


def run_prov_check(control=None):
    """--provisional --check, --control prov-drop|prov-false-unblocked (exit 0 only when the planted doc fails)."""
    start = prov_start()
    acc = prov_accept()
    u, b = prov_parse(need(PROV_DOC).read_text(encoding="utf-8"))
    if control == "prov-drop":
        if not u and not b:
            raise Refuse("prov-drop: no row to drop")
        k = (u or b).pop(0)
        print(f"name_sites --control prov-drop: dropped rank {k[0]} 0x{k[1]:08X}")
    elif control == "prov-false-unblocked":
        if not b:
            raise Refuse("prov-false-unblocked: no Blocked row to move")
        k = b.pop(0)
        u.append(k)
        print(f"name_sites --control prov-false-unblocked: moved rank {k[0]} 0x{k[1]:08X} to Unblocked")
    fails = prov_check(start, u, b, acc)
    for f in fails[:20]:
        print(f"  FAIL {f}")
    print(f"name_sites --provisional --check{' (' + control + ')' if control else ''}: start {len(start)}, "
          f"Unblocked {len(u)}, Blocked {len(b)}, FAIL {len(fails)}")
    if control:
        ok = bool(fails)
        print(f"  control {control}: {'PASS (check failed as planted)' if ok else 'FAIL (check passed a planted doc)'}")
        return 0 if ok else 1
    return 1 if fails else 0


def prov_buckets():
    """-> ({va: bucket | "matched"} identical lane, {va: size}, {bucket: [size]} identical lane) via pool_buckets.assign
    with pool_buckets.main's inputs."""
    import coff_info
    import pool_buckets as pb
    pin = coff_info.pinned_sha() or ""
    cut = byte_compare.cut_dir(pin)
    why = byte_compare.cut_check(cut)
    if why:
        raise Refuse(why)
    rows = [r for r in load_jsonl(need(CAND)) if r.get("lane", "identical") == "identical"]
    ms = tomllib.loads(need(MATCHED).read_text(encoding="utf-8"))
    matched = {(f["symbol"], pb.as_int(f["va"])) for f in ms.get("function", [])}
    maxsize = max((f["size"] for f in ms.get("function", [])), default=0)
    opt = {u["name"]: u.get("opt", "unknown") for u in tomllib.loads(pb.UNITS.read_text(encoding="utf-8"))["unit"]}
    led = defaultdict(list)
    for x in pb.match_one.load_ledger([need(pb.LEDGER)] + pb.match_one.ledger_paths()[1:]):
        if x.get("va") not in (None, "None"):
            led[pb.as_int(x["va"])].append(x)
        led[x["symbol"]].append(x)
    _u, out, _e, _t = pb.assign(rows, matched, opt, pin, cut, led, maxsize, {}, set())
    bk = {r["va"]: "matched" for r in rows}
    for b, rs in out.items():
        for r in rs:
            bk[r["va"]] = b
    return bk, {r["va"]: r["size"] for r in rows}, {b: [r["size"] for r in rs] for b, rs in out.items()}


def prov_ev(x):
    ev = x.get("evidence")
    if ev:
        xr = ev["xref"]
        return (f"xref {xr['result'] or 'one public'} (sites {xr['sites']}, names {names_txt(xr['names'])}), cut-sym "
                f"{names_txt(ev['cutSym'])}, strings {ev['strings']}")
    if x["reason"] == "constant":
        return "constant, not an address"
    return "second pass did not examine it (target named or no unnamed residue)"


def run_provisional():
    """--provisional (4.2 T11): docs/matching/provisional_residue.md from the start set; see the doc's header."""
    start = prov_start()
    acc = prov_accept()
    bk, size, bsz = prov_buckets()
    residue = load_jsonl(need(OUT / "residue.jsonl"))
    res_t, res_r = defaultdict(list), defaultdict(list)
    for x in residue:
        res_t[int(x["target"], 16)].append(x)
        res_r[int(x["rowVA"], 16)].append(x)
    corr = defaultdict(list)
    for e in load_jsonl(CORR) if CORR.is_file() else []:
        corr[int(e["va"], 16)].append(e)
    near = defaultdict(list)
    for e in load_jsonl(OUT / "near.jsonl") if (OUT / "near.jsonl").is_file() else []:
        near[int(e["va"], 16)].append(e)
    appl = defaultdict(list)
    for e in load_jsonl(OUT / "apply.jsonl") if (OUT / "apply.jsonl").is_file() else []:
        appl[int(e["line"].split()[0], 16)].append(e)
    unexamined = sum(1 for x in residue if x["reason"] != "constant" and not x.get("evidence"))
    # candidates.jsonl's own verdict per start target (cross-check of the pin-table re-derivation)
    import candidates
    funcs = candidates.load_symbols()[0]
    cand = {r["va"]: r for r in load_jsonl(CAND) if r.get("lane", "identical") == "identical"}
    disagree = []
    verdict = {}
    for rk, va, name, ts in start:
        for t, _o, off, _s, _c in ts:
            v = acc(t, off)
            verdict[(va, t)] = v
            cs = {s["resolved"] for s in cand.get(va, {}).get("sites", []) if s["target"] == t}
            if cs and any(candidates.accepted(s, t, funcs) != v[0] for s in cs):
                disagree.append(f"rank {rk} 0x{va:08X} target 0x{t:08X}: pin tables {v[0]}, candidates.jsonl {sorted(cs)}")

    dry = {int(e["va"], 16): e for e in load_jsonl(OUT / "names.jsonl")} if (OUT / "names.jsonl").is_file() else {}

    def other(va):  # this row's unresolved sites in candidates.jsonl (not start targets), with the evidence on file
        ts = sorted({s["target"] for s in cand.get(va, {}).get("sites", []) if s["resolved"] is None})
        out = []
        for t in ts:
            xs = [x for x in res_r.get(va, []) if int(x["target"], 16) == t]
            if xs:
                out.append(f"0x{t:08X} {xs[0]['reason']} ({prov_ev(xs[0])})")
            elif t in dry:
                ap = appl.get(t, [])
                out.append(f"0x{t:08X} second pass names {safe(dry[t]['name'])} ({dry[t]['via']}, {dry[t]['evidence']}),"
                           " not pinned: " + ("; ".join(f"T10 replant kept {e['kept']} {e['why'] or ''}".rstrip()
                                                        for e in ap) or "no T10 apply line"))
            else:
                out.append(f"0x{t:08X} not in residue.jsonl or names.jsonl")
        return ("; unresolved: " + "; ".join(out)) if out else ""

    unb, blk = [], []
    nblock = Counter()
    for r in start:
        bad = [t for t, *_ in r[3] if not verdict[(r[1], t)][0]]
        (blk if bad else unb).append(r)
        nblock.update(bad)
    owners = {t: (o, off, s, c) for r in start for t, o, off, s, c in r[3]}
    groups = defaultdict(list)
    for r in blk:
        bad = sorted((t for t, *_ in r[3] if not verdict[(r[1], t)][0]), key=lambda t: (-nblock[t], t))
        groups[bad[0]].append((r, bad[1:]))

    def mb(n):
        return f"{n:,} B"

    def sz(rs):
        return sum(size.get(r[1], 0) for r in rs)

    absent = [r for r in start if r[1] not in size]
    left = [r for r in start if bk.get(r[1]) != "reloc-provisional"]
    lb = Counter(bk.get(r[1], "not in candidates.jsonl") for r in left)
    sup = sum(len(bsz.get(b, [])) for b in NAME_ONLY), sum(sum(bsz.get(b, [])) for b in NAME_ONLY)
    cen = None
    cdoc = REPO / "docs" / "matching" / "family_census.md"
    if cdoc.is_file():
        m = re.search(r"^Rows blocked: (\d+) of (\d+) pool", cdoc.read_text(encoding="utf-8"), re.M)
        cen = (int(m.group(1)), int(m.group(2))) if m else None
    L = ["# Provisional residue (generated)", "",
         "Generated by `PY tools/name_sites.py --provisional` (4.2 T11) from the frozen start set"
         " docs/matching/provisional_start.md, config/retail/symbols.txt + symbols_pin.txt (`candidates.accepted`),"
         " derived/candidates/candidates.jsonl (`pool_buckets.assign`) and derived/name_sites/{residue,corr,near,apply}"
         ".jsonl; never hand-edited. Addresses, names and counts only (D46): `??_C@` names masked. Checked by"
         " `--provisional --check` (each start row in exactly one section; Unblocked targets all accepted).", "",
         "Acceptance per target: a func-table VA's status, else the data line at target minus the start set's member"
         " offset with no pinned VA in between; `candidates.accepted` (self, constant, units_seed.STATUSES,"
         " PIN_STATUSES, near-corr for func-table VAs). Buckets: identical lane, `matched` when in matched_set.toml.", "",
         "## Figures", "",
         f"- Start set: {len(start)} rows, {mb(sz(start))} (start rows x candidates.jsonl sizes; T2 docs carry no start"
         f" bytes){f'; {len(absent)} start rows not in candidates.jsonl' if absent else ''}.",
         f"- Of the 484: unblocked {len(unb)} rows, {mb(sz(unb))}; still blocked {len(blk)} rows, {mb(sz(blk))},"
         f" by {len(groups)} primary targets ({len(nblock)} blocking targets in all).",
         f"- Of the 484: left `reloc-provisional` {len(left)} rows, {mb(sz(left))}: "
         + ", ".join(f"{b} {n}" for b, n in sorted(lb.items(), key=lambda kv: (-kv[1], kv[0])))
         + f"; still `reloc-provisional` {len(start) - len(left)} rows, {mb(sz(start) - sz(left))}.",
         f"- Name-only supply (reloc-provisional + reloc-data + reloc-code, identical lane): T2 {T2_SUPPLY[0]} rows,"
         f" {mb(T2_SUPPLY[1])}; now {sup[0]} rows, {mb(sup[1])}; freed {T2_SUPPLY[0] - sup[0]} rows,"
         f" {mb(T2_SUPPLY[1] - sup[1])} (" + ", ".join(f"{b} {len(bsz.get(b, []))}" for b in NAME_ONLY) + ").",
         (f"- Census \"Rows blocked\" pool: T2 {T2_BLOCKED}; now {cen[0]} of {cen[1]} pool"
          f" (docs/matching/family_census.md); freed {T2_BLOCKED - cen[0]} rows (T2 recorded rows only, no bytes)."
          if cen else "- Census \"Rows blocked\": docs/matching/family_census.md absent or without the line."),
         f"- Cross-check: start targets where candidates.jsonl's verdict differs from the pin tables: {len(disagree)}.",
         f"- Residue evidence: {len(residue)} residue sites, {unexamined} non-constant without second-pass evidence"
         " (residue.jsonl of the last name_sites run; T11 runs it with `--min-rows 1`).", "",
         "## Unblocked", "",
         f"{len(unb)} rows. `rank VA name | bucket | target = pin line` per start target; `unresolved:` the row's"
         " unresolved sites in candidates.jsonl (not start targets) with residue.jsonl's reason and second-pass"
         " evidence, or the second pass's name (names.jsonl) and the T10 replant verdict (apply.jsonl).", ""]
    for rk, va, name, ts in unb:
        tl = "; ".join(f"0x{t:08X} = {safe_line(verdict[(va, t)][1])}" for t, *_x in ts)
        L.append(f"- {rk} 0x{va:08X} {name} | {bk.get(va, 'not in candidates.jsonl')} | {tl}{other(va)}")
    L += ["", "## Blocked", "",
          f"{len(blk)} rows, {len(nblock)} blocking targets, one section each; a row is listed once, under its primary"
          f" target (its blocking target that blocks most start rows, then VA; {len(groups)} targets are primary);"
          " `also:` the row's other blocking targets. Per target: the pin line now, why it is not accepted,"
          " and the evidence searched: corr.jsonl (--promote-corr verdict), near_check (near.jsonl, corr route near),"
          " residue.jsonl (reason, second-pass evidence), apply.jsonl (T10 replant).", ""]
    nctx = None
    elsewhere = defaultdict(list)
    for t0, rs in groups.items():
        for r, also in rs:
            for x in also:
                elsewhere[x].append(r)
    for t in sorted(nblock, key=lambda t: (-nblock[t], t)):
        o, off, s, c = owners[t]
        ok, ln, st = acc(t, off)
        L += [f"### 0x{t:08X} {safe(o) if ' ' not in o else o} · blocks {nblock[t]} start rows, primary for"
              f" {len(groups[t])} · start {s} {c}", ""]
        L.append(f"- pin: {safe_line(ln)}")
        if ln is None:
            why = "no pin line covers the target now"
        elif st in ("near-corr",) and t not in funcs:
            why = f"status {st} is accepted for func-table VAs only (D115); this target is data"
        else:
            why = f"status {st} is not an accepted status"
        cs = corr.get(t - off, [])
        L.append("- corr.jsonl: " + ("; ".join(f"route {e['route']} {e['verdict']} {e['reason'] or ''}".rstrip()
                                                for e in cs) if cs else "no candidate at this VA (not routed)"))
        nc = near.get(t, []) + [e for e in cs if e["route"] == "near"]
        if t in funcs:  # near_check now, at each proto VA of the pin's PDB public (T9's corr_near pair)
            if nctx is None:
                img, pubs, procs = load()
                nctx = (near_ctx(img, pubs, procs), defaultdict(list))
                for pv, ns in pubs.items():
                    for n in ns:
                        nctx[1][n].append(pv)
            nm = funcs[t]["name"]
            nc += [dict(near_check(nctx[0], pv, t), now=f"0x{pv:08X}") for pv in sorted(nctx[1].get(nm, ()))] or                 [{"verdict": "fail", "reason": "no PDB public of this name", "now": "-"}]
        L.append("- near_check: " + ("; ".join(
            (f"now proto {e['now']}: " if "now" in e else "near.jsonl: " if "sim" in e and e.get("route") is None
             else f"corr route {e.get('route')}: ") + f"{e['verdict']} {e['reason'] or ''}".rstrip()
            + (f" (sim {e['sim']})" if e.get("sim") is not None else "") for e in nc)
            if nc else "not applicable (data target, not a near row)"))
        xs = res_t.get(t, [])
        L.append("- residue.jsonl: " + ("; ".join(f"{x['reason']} ({prov_ev(x)})" for x in xs[:3])
                                         + (f"; +{len(xs) - 3} sites" if len(xs) > 3 else "")
                                         if xs else "no site at this target (a pinned name, not residue)"))
        ap = appl.get(t - off, [])
        L.append("- apply.jsonl: " + ("; ".join(f"kept {e['kept']} {e['why'] or ''}".rstrip() for e in ap)
                                       if ap else "no T10 line at this VA"))
        L.append(f"- stays blocked: {why}" + (f"; corr {cs[0]['reason']}" if cs and cs[0]["verdict"] == "fail" else
                                               "; --promote-corr never routes ??_C@/__real@ (T7)"
                                               if not cs and ln and ln.split()[3].startswith(("??_C@", "__real@"))
                                               else ""))
        if elsewhere[t]:
            L.append("- also blocks (listed under their primary): "
                     + ", ".join(f"{r[0]} 0x{r[1]:08X}" for r in sorted(elsewhere[t])))
        L.append("- rows:" if groups[t] else "- rows: none primary here")
        for (rk, va, name, _ts), also in groups[t]:
            al = (" | also: " + ", ".join(f"0x{x:08X}" for x in also)) if also else ""
            L.append(f"  - {rk} 0x{va:08X} {name} | {bk.get(va, 'not in candidates.jsonl')}{al}{other(va)}")
        L.append("")
    PROV_DOC.write_bytes(("\n".join(L).rstrip("\n") + "\n").encode("utf-8"))
    for d in disagree[:10]:
        print(f"  WARN {d}")
    print(f"name_sites --provisional: start {len(start)}; unblocked {len(unb)} ({sz(unb)} B), blocked {len(blk)} "
          f"({sz(blk)} B), {len(nblock)} blocking targets, {sum(1 for v in groups.values() if v)} primary; left reloc-provisional {len(left)} ({sz(left)} B); "
          f"name-only supply {sup[0]} ({sup[1]} B); census blocked {cen[0] if cen else '?'}; "
          f"cross-check disagreements {len(disagree)}; wrote {PROV_DOC.relative_to(REPO).as_posix()}")
    return 0


AMB_CLS = ("bracket", "fold-alias", "fold-k1",  # 5.3 T3: the classes ruled at 5.3 T1 (D128, D129, D130)
           "k1", "D115", "D113", "caller-xref")  # T13.2.c3: the classes ruled at 5.1 T9
AMB_VIA = {"bracket": "unit-bracket", "fold-alias": "fold-alias", "fold-k1": "fold-alias",  # pin_line tags (pin_rows TAGS)
           "k1": "site-corr", "caller-xref": "site-corr",  # T13.2.c3: +callee-bytes disasm-verified (check(), D114)
           "D115": "anchor-window+caller-xref"}  # T13.2.c3: NEAR_SRC near-corr; D113 has no func tag (unwritable)
AMB_NEW_CTL = ("k1-planted", "D115-planted", "D113-planted", "caller-xref-planted")  # T13.2.c3 --control
AMB_T3 = ("bracket", "fold-alias", "fold-k1")  # --apply header `# 5.3 T3 ...` (run_amb_recheck); others `# 5.3 T13.2 ...`
AMB_OUT = byte_compare._phase_run_dir() / "t3"


def amb_state(img, pubs, procs):
    """5.3 T3: (S, tab_va, tab_name, pub_vas, unit-of-VA) for the --ambiguous writer and its controls; S from
    pool_buckets.ruled_state (uncapped recount, C0366: bodies are recounted hits, not targetVAs)."""
    import pool_buckets
    S = pool_buckets.ruled_state(img, pubs, near_ctx(img, pubs, procs))
    S.pred = {"bracket": pool_buckets.bracket, "fold-alias": pool_buckets.fold_alias, "fold-k1": pool_buckets.fold_k1,
              "k1": pool_buckets.cls_k1, "D115": pool_buckets.cls_d115, "D113": pool_buckets.cls_d113,
              "caller-xref": pool_buckets.cls_caller_xref}  # T13.2.c3
    S.gf = {int(g["entry"], 16): (g["size"], int(g["bodyMax"], 16)) for g in load_jsonl(need(  # D115 size (run_near)
        REPO / "derived" / "ghidra" / f"retail-{img.pin[:8]}" / "applied" / "functions.jsonl"))}
    tab_va, tab_name = defaultdict(set), defaultdict(set)
    for va, _k, _sz, nm, _src in load_table():
        tab_va[va].add(nm)
        tab_name[nm].add(va)
    return S, tab_va, tab_name, sorted(pubs), unit_of_va()[0]


def amb_verdict(S, procs, tab_va, tab_name, pub_vas, cls, P, t):
    """5.3 T3: the writer's own chain for (P, t): class predicate (fold-k1: at an already-named body), the publics not
    yet at t (no other name at t for bracket/fold-alias, none named elsewhere), callee bytes (check(), D114).
    T13.2.c3: D113 ends no-func-slot-route; D115 skips check() (near-corr, Ghidra extent as size).
    -> (None, [entry]) or (reason, [])."""
    if not S.pred[cls](S, P, t) or cls == "fold-k1" and not tab_va.get(t):
        return f"class-{cls}", []
    ns = sorted(S.pubs.get(P, ()))
    new = [n for n in ns if n not in tab_va.get(t, ())]
    if cls != "fold-k1" and tab_va.get(t, set()) - set(ns):
        return "body-named-other", []
    if not new:
        return "already-named", []
    if any(tab_name[n] - {t} for n in new):
        return "name-elsewhere", []
    if cls == "D113":  # T13.2.c3: pin_rows TAGS has no func corr-verified slot route (slots/slotnames are data only)
        return "no-func-slot-route", []
    if cls == "D115":  # T13.2.c3: near_check passed (class step); no callee bytes (D115), size = Ghidra extent (run_near)
        g = S.gf.get(t)
        if g is None:
            return "no-ghidra-function", []
        if g[0] != g[1] - t + 1:
            return "ghidra-noncontiguous", []
        return None, [{"va": f"0x{t:08X}", "name": n, "kind": "func", "protoVA": f"0x{P:08X}", "evidence": "near",
                       "size": g[0], "already": False, "via": AMB_VIA[cls]} for n in new]
    ok, fsz = check(S.img, procs, S.pubs, tab_va, pub_vas, t, ns[0], "func", P)
    if not ok:
        return "callee-bytes", []
    if cls == "bracket":
        r = amb_bracket_down(S, procs, tab_va, tab_name, P, t, fsz)
        if r:
            return r, []
    return None, [{"va": f"0x{t:08X}", "name": n, "kind": "func", "protoVA": f"0x{P:08X}", "evidence": "checked",
                   "size": fsz, "already": False, "via": AMB_VIA[cls]} for n in new]


def amb_bracket_down(S, procs, tab_va, tab_name, P, t, n):
    """T13.2 (bracket only; check() unchanged): dtor-not-own when a ??_G/??_E public's class has a table ??1<Class>@@
    at X and no retail rel call/jmp out of the body reaches X; callee-down when a rel dest out of the body is unnamed
    in the table and the proto dest (same mask) is unsized or not masked-equal to it. -> reason or None."""
    img = S.img
    pb, rb = img.pbytes(P, n), img.rbytes(t, n)
    outs = [(target(pb, P, o, k), target(rb, t, o, k)) for o, _w, k in proto_map.mask_proc(pb, P, img.plo, img.phi)[0]
            if k == "rel" and not t <= target(rb, t, o, k) < t + n]
    for nm in sorted(S.pubs.get(P, ())):
        if nm.startswith(("??_G", "??_E")) and "@@" in nm:
            own = "??1" + nm[4:nm.index("@@") + 2]
            xs = {x for k, vs in tab_name.items() if k.startswith(own) for x in vs}
            if xs and not xs & {rd for _pd, rd in outs}:
                return "dtor-not-own"
    for pd, rd in outs:
        if {x for x in tab_va.get(rd, ()) if (rd, x) not in NEAR_CORR}:
            continue
        # T13.2 expert: a near-corr name is a name (D115); it must agree with the one proto public at pd, else refuse.
        # was: near-corr names counted as unnamed here (byte-compared), refusing matched 0x0002FBB8 (near-identical callee)
        nc = set(tab_va.get(rd, ()))
        if nc:
            ps = S.pubs.get(pd, ())
            if len(ps) == 1 and next(iter(ps)) in nc:
                continue
            return "callee-down"
        ln = procs.get(pd)
        if not ln or not img.masked_equal(pd, rd, ln):
            return "callee-down"
    return None


@contextlib.contextmanager
def amb_unpinned(tab_va, tab_name, t):
    """T13.2: tab_va/tab_name without body t's names (in memory), restored on exit."""
    gone = set(tab_va.get(t, ()))
    for n in gone:
        tab_name[n].discard(t)
    tab_va.pop(t, None)
    try:
        yield gone
    finally:
        if gone:
            tab_va[t] = set(gone)
        for n in gone:
            tab_name[n].add(t)


def amb_pairs(S, cls, uof):
    """Settled (P, t) pairs of one class over the recounted ambiguous rows, deterministic: unit of t, then t, then P."""
    ps = {(P, t) for P, _r, es in S.A for t in es if S.pred[cls](S, P, t)}
    return sorted(ps, key=lambda p: (uof(p[1]) or "~", p[1], p[0]))


def amb_replant(lines):
    """pin_rows' own route on the new lines: appended to a copy of the pin file, replanted (apply_replant's form). A
    line whose row fails, or whose VA carries an existing row whose result moves, is dropped; repeated to a fixed
    point. -> (kept lines, {line: why}); Refuse when an existing row moves at a VA with no new line."""
    import pin_rows
    try:
        st = pin_rows.state()
        if SYMS_PIN.resolve() != pin_rows.PIN.resolve():
            st = st.replant(SYMS_PIN)
    except pin_rows.Refuse as e:
        raise Refuse(f"pin_rows: {e}")
    base = SYMS_PIN.read_bytes().decode("utf-8")
    base += "" if not base or base.endswith("\n") else "\n"
    nb = len(base.split("\n")) - 1
    drop = {}
    while True:
        keep = [ln for ln in lines if ln not in drop]
        if not keep:
            return [], drop
        CORR_TMP.mkdir(parents=True, exist_ok=True)
        p = CORR_TMP / "ambiguous.txt"
        p.write_bytes((base + "".join(ln + "\n" for ln in keep)).encode("utf-8"))
        try:
            s = st.replant(p)
        except pin_rows.Refuse as e:
            raise Refuse(f"pin_rows: {e}")
        finally:
            p.unlink()
            try:
                CORR_TMP.rmdir()
            except OSError:
                pass
        by = {r["line"]: r for r in s.rows}
        moved = {r["va"] for r in st.rows if r["line"] not in by or r["fails"] != by[r["line"]]["fails"]}
        bad = {}
        for i, ln in enumerate(keep, nb + 1):
            r = by.get(i)
            if r is None or r.get("va") != int(ln.split()[0], 16):
                raise Refuse(f"--ambiguous: replanted line {i} is not {ln.split()[0]}")
            if r["fails"]:
                bad[ln] = ";".join(r["fails"])
            elif r["va"] in moved:
                bad[ln] = "moves-existing-row"
        if not bad:
            if moved - {int(ln.split()[0], 16) for ln in keep}:
                raise Refuse(f"--ambiguous: {len(moved)} existing pin row(s) change result")
            return keep, drop
        drop.update(bad)


def run_ambiguous(img, pubs, procs, cls, cap, apply):
    """5.3 T3 --ambiguous CLS [--max N] [--apply]: candidate bodies from pool_buckets' predicate, the writer's chain
    (amb_verdict), then pin_rows' route (amb_replant). Writes AMB_OUT/ambiguous_<cls>.txt; --apply appends the lines to
    the pin file. --max N: the first N bodies in order."""
    S, tab_va, tab_name, pub_vas, uof = amb_state(img, pubs, procs)
    pairs = amb_pairs(S, cls, uof)
    pset = set(pairs)
    rows = [P for P, _r, es in S.A if any((P, t) in pset for t in es)]
    byt = defaultdict(set)
    for P, t in pairs:
        byt[t].add(P)
    why, pins = Counter(), []
    for P, t in pairs:
        if cls != "fold-k1" and len(byt[t]) > 1:  # two protos would name one body: D119 one name
            why["body-two-protos"] += 1
            continue
        r, es = amb_verdict(S, procs, tab_va, tab_name, pub_vas, cls, P, t)
        if r:
            why[r] += 1
            continue
        pins += es
    bvas = list(dict.fromkeys(e["va"] for e in pins))  # bodies in order
    if cap is not None:
        bvas = bvas[:cap]
        pins = [e for e in pins if e["va"] in set(bvas)]
    nb = len(bvas)
    seen, lines = set(), []
    for e in pins:
        ln = pin_line(e)
        if ln not in seen:
            seen.add(ln)
            lines.append(ln)
    kept, drop = amb_replant(lines) if lines else ([], {})
    AMB_OUT.mkdir(parents=True, exist_ok=True)
    out = AMB_OUT / f"ambiguous_{cls}.txt"
    out.write_bytes("".join(ln + "\n" for ln in kept).encode("utf-8"))
    print(f"name_sites --ambiguous {cls}{f' --max {cap}' if cap is not None else ''}: rows {len(rows)}, settled pairs "
          f"{len(pairs)}, bodies {len(byt)}; writer refused " + (", ".join(f"{k} {v}" for k, v in sorted(why.items()))
                                                                 or "0"))
    print(f"  candidate lines {len(lines)} (bodies {nb}); pin_rows agree {len(kept)}, disagree {len(drop)}"
          + (" (" + ", ".join(f"{k} {v}" for k, v in sorted(Counter(w for w in drop.values()).items())) + ")"
             if drop else ""))
    print(f"  wrote {len(kept)} line(s) to {out.relative_to(REPO).as_posix()}")
    if cls == "bracket":  # T13.2: kept bodies with masked-identical proto rows of the same module
        kva = {int(ln.split()[0], 16) for ln in kept}
        bp = list(dict.fromkeys((int(e["va"], 16), int(e["protoVA"], 16)) for e in pins if int(e["va"], 16) in kva))
        sib = amb_siblings(S, procs, bp)
        so = AMB_OUT / "ambiguous_bracket_siblings.txt"
        so.write_bytes("".join(f"0x{t:08X} {sorted(S.pubs.get(P, ('?',)))[0]} siblings: "
                               + " ".join(f"0x{q:08X} {n}" for q, n in sib[t]) + "\n"
                               for t, P in bp if t in sib).encode("utf-8"))
        print(f"  siblings flagged {len(sib)} of {len(bp)} kept bodies; wrote {so.relative_to(REPO).as_posix()}")
    if apply and kept:
        old = SYMS_PIN.read_bytes().decode("utf-8")
        sep = "" if not old or old.endswith("\n") else "\n"
        hdr = (f"# 5.3 T3 name_sites --ambiguous {cls}: {len(kept)} lines; pin_rows replant kept (D128-D130)"
               if cls in AMB_T3 else  # T13.2.c3: the class in the header, the tag on each line
               f"# 5.3 T13.2 name_sites --ambiguous {cls}: {len(kept)} lines; pin_rows replant kept")
        SYMS_PIN.write_bytes((old + sep + hdr + "\n" + "".join(ln + "\n" for ln in kept)).encode("utf-8"))
        print(f"  applied {len(kept)} line(s) to {SYMS_PIN}")
    return 0


def amb_siblings(S, procs, bodies):
    """T13.2: {t: [(protoVA, name)]} for each kept body (t, P): the other proto_map rows sharing a module with P whose
    masked bytes (mask_proc + proto .reloc sites, as proto_map) equal P's."""
    import pool_buckets
    fx = pool_buckets.candidates.load_jsonl(pool_buckets.candidates.need(pool_buckets.candidates.FUNCS))
    img, prs = S.img, sorted(S.img.prelocs)
    by = defaultdict(list)  # (module, length) -> [row]
    for r in fx:
        for m in r.get("modules") or ():
            by[(m, r["length"])].append(r)

    def key(va, n):
        b = img.pbytes(va, n)
        if b is None or n <= 0:
            return None
        ms = {o for o, _w, _k in proto_map.mask_proc(b, va, img.plo, img.phi)[0]}
        ms |= {x - va for x in prs[bisect.bisect_left(prs, va):bisect.bisect_left(prs, va + n)]}
        b = bytearray(b)
        for o in ms:
            b[o:o + 4] = b"\0" * len(b[o:o + 4])
        return bytes(b)

    out, memo = {}, {}
    for t, P in bodies:
        rp = next((r for r in fx if int(r["protoVA"], 16) == P), None)
        if rp is None:
            continue
        kp = key(P, rp["length"])
        sib = set()
        for m in rp.get("modules") or ():
            for r in by[(m, rp["length"])]:
                q = int(r["protoVA"], 16)
                if q != P:
                    if q not in memo:
                        memo[q] = key(q, r["length"])
                    if kp is not None and memo[q] == kp:
                        sib.add((q, sorted(r["names"])[0] if r.get("names") else "?"))
        if sib:
            out[t] = sorted(sib)
    return out


def run_amb_recheck(img, pubs, procs):
    """T13.2 --recheck bracket: each body of the '# 5.3 T3 name_sites --ambiguous bracket' block of the pin file
    re-run through amb_verdict with its own lines removed; prints the newly refused bodies; exit 0."""
    S, tab_va, tab_name, pub_vas, _uof = amb_state(img, pubs, procs)
    blk, on = defaultdict(set), False
    for ln in SYMS_PIN.read_text(encoding="utf-8").splitlines():
        if ln.startswith("#"):
            on = ln.startswith("# 5.3 T3 name_sites --ambiguous bracket")
            continue
        if on and ln.strip():
            f = ln.split()
            blk[int(f[0], 16)].add(" ".join(f[3:-2]))
    if not blk:
        raise Refuse("--recheck bracket: no '# 5.3 T3 name_sites --ambiguous bracket' block in the pin file")
    bad = []
    for t in sorted(blk):
        ps = [P for P, _r, es in S.A if t in es and blk[t] <= S.pubs.get(P, set())]
        if len(ps) != 1:
            bad.append((t, sorted(blk[t])[0], f"rows-{len(ps)}"))
            continue
        with amb_unpinned(tab_va, tab_name, t):
            r, _es = amb_verdict(S, procs, tab_va, tab_name, pub_vas, "bracket", ps[0], t)
        if r:
            bad.append((t, sorted(blk[t])[0], r))
    for t, n, r in bad:
        print(f"  0x{t:08X} {n} {r}")
    print(f"name_sites --recheck bracket: bodies {len(blk)} (lines {sum(map(len, blk.values()))}); newly refused "
          f"{len(bad)} (" + (", ".join(f"{k} {v}" for k, v in sorted(Counter(r for *_x, r in bad).items())) or "0")
          + ")")
    return 0


def run_amb_control(img, pubs, procs, ctl):
    """5.3 T3 controls over the writer's chain (amb_verdict; pin_rows only for the positive): bracket-positive (the
    first bracket pair passing the chain, its lines kept by pin_rows), bracket-planted (a row body outside its bracket,
    inside its unit), bracket-ambiguous (a row with two bodies inside its bracket), fold-alias-planted (a fold-alias row
    at a sibling body its callers do not name; none: a body not among its hits), fold-k1-planted (a fold-k1 row at a
    hit other than its named body). Planted: exit 0 when refused ("failed as expected"), else DID NOT FAIL exit 1.
    T13.2.c3 k1-/D115-/D113-/caller-xref-planted: the class's first settled pair at a wrong body (one the rest of the
    chain passes when there is one); exit 0 only when refused class-<cls>."""
    S, tab_va, tab_name, pub_vas, uof = amb_state(img, pubs, procs)
    if ctl == "bracket-own-dtor":  # T13.2: ??_GPropellerMain at 0x000A3EC9 (its dtor call is not ??1PropellerMain)
        t = 0x000A3EC9
        P = next((p for p in sorted(S.pubs) if any(n.startswith("??_GPropellerMain@@") for n in S.pubs[p])), None)
        if P is None:
            print(f"name_sites --control {ctl}: no ??_GPropellerMain public -> FAIL")
            return 1
        with amb_unpinned(tab_va, tab_name, t):
            r, _es = amb_verdict(S, procs, tab_va, tab_name, pub_vas, "bracket", P, t)
        print(f"name_sites --control {ctl}: 0x{P:08X} {sorted(S.pubs[P])[0][:60]} -> 0x{t:08X} (pin lines removed): "
              + (f"refused {r}: failed as expected" if r else "DID NOT FAIL"))
        return 0 if r else 1
    ent = sorted({t for _P, _r, es in S.A for t in es})
    plant = None
    free = lambda P, x: not tab_va.get(x) and not any(tab_name[n] for n in S.pubs.get(P, ()))  # noqa: E731
    # plants: P's publics not yet in the table and an unnamed plant body, so only the targeted check refuses
    if ctl == "bracket-positive":
        for P, t in amb_pairs(S, "bracket", uof):
            r, es = amb_verdict(S, procs, tab_va, tab_name, pub_vas, "bracket", P, t)
            if r is None:
                lines = [pin_line(e) for e in es]
                kept, drop = amb_replant(lines)
                ok = len(kept) == len(lines)
                print(f"name_sites --control bracket-positive: 0x{P:08X} -> 0x{t:08X} {len(lines)} line(s); pin_rows "
                      f"kept {len(kept)}{'' if ok else ' ' + str(sorted(set(drop.values())))} -> "
                      f"{'PASS' if ok else 'FAIL'}")
                return 0 if ok else 1
        print("name_sites --control bracket-positive: no bracket pair passes the chain -> FAIL")
        return 1
    if ctl in ("bracket-planted", "bracket-ambiguous"):
        cls = "bracket"
        for P in sorted(S.brk):
            b = S.brk[P]
            inw = sorted(x for x in S.eset.get(P, ()) if b[0] <= x < b[1] and b[2] <= x < b[3])
            if ctl == "bracket-planted" and len(inw) == 1 and S.pred[cls](S, P, inw[0]):
                t2 = next((x for x in sorted(S.eset[P]) if not b[0] <= x < b[1] and b[2] <= x < b[3]
                           and free(P, x)), None)
                if t2 is not None:
                    plant = (P, t2, f"body outside the bracket 0x{b[0]:08X}-0x{b[1]:08X}, inside the unit")
                    break
            if ctl == "bracket-ambiguous" and len(inw) >= 2 and free(P, inw[0]):
                plant = (P, inw[0], f"{len(inw)} bodies inside the bracket 0x{b[0]:08X}-0x{b[1]:08X}")
                break
    elif ctl == "fold-alias-planted":
        cls = "fold-alias"
        for P, t in amb_pairs(S, cls, uof):
            t2 = next((x for x in sorted(S.eset[P]) if x != t and free(P, x)), None)
            if t2 is not None:
                plant = (P, t2, f"sibling body of 0x{t:08X} its callers do not name")
                break
        if plant is None:
            for P, t in amb_pairs(S, cls, uof):
                t2 = next((x for x in ent if x not in S.rowt[P] and free(P, x)), None)
                if t2 is not None:
                    plant = (P, t2, f"body not among the row's hits (alias body 0x{t:08X})")
                    break
    elif ctl in AMB_NEW_CTL:  # T13.2.c3: the class's first settled pair fed a wrong body; only class-<cls> refuses
        cls = ctl[:-len("-planted")]
        with_rest = None  # prefer a wrong body the rest of the chain passes (class step skipped), else any free one
        for P, t in amb_pairs(S, cls, uof):
            pool = [x for x in sorted(S.rowt[P] | S.eset[P]) if x != t] + [x for x in ent if x not in S.rowt[P]]
            fr = [x for x in pool if free(P, x)]
            if not fr:
                continue
            sv = S.pred[cls]
            S.pred[cls] = lambda *_a: True
            try:
                with_rest = next((x for x in fr[:64] if amb_verdict(S, procs, tab_va, tab_name, pub_vas, cls, P, x)[0]
                                  is None), None)
            finally:
                S.pred[cls] = sv
            t2 = with_rest if with_rest is not None else fr[0]
            plant = (P, t2, f"wrong body for settled 0x{t:08X}"
                     + ("; rest of the chain passes it" if with_rest is not None else "; rest of the chain refuses it"))
            break
        if plant is not None:
            P, t2, what = plant
            r, _es = amb_verdict(S, procs, tab_va, tab_name, pub_vas, cls, P, t2)
            ok = r == f"class-{cls}"
            print(f"name_sites --control {ctl}: 0x{P:08X} {sorted(S.pubs.get(P, ('?',)))[0][:60]} -> 0x{t2:08X} "
                  f"({what}): " + (f"refused {r}: failed as expected" if ok else f"DID NOT FAIL (verdict {r or 'pass'})"))
            return 0 if ok else 1
    else:
        cls = "fold-k1"
        for P, t in amb_pairs(S, cls, uof):
            if not tab_va.get(t):
                continue
            sp = sorted(S.pubs.get(P, ()))  # a hit whose callee bytes pass: only the class check refuses it
            t2 = next((x for x in sorted(S.rowt[P]) if x != t and free(P, x) and sp
                       and check(S.img, procs, S.pubs, tab_va, pub_vas, x, sp[0], "func", P)[0]), None)
            if t2 is not None:
                plant = (P, t2, f"hit other than the named body 0x{t:08X}")
                break
    if plant is None:
        print(f"name_sites --control {ctl}: no plant found -> FAIL")
        return 1
    P, t2, what = plant
    r, _es = amb_verdict(S, procs, tab_va, tab_name, pub_vas, cls, P, t2)
    print(f"name_sites --control {ctl}: 0x{P:08X} {sorted(S.pubs.get(P, ()))[0][:60]} -> 0x{t2:08X} ({what}): "
          + (f"refused {r}: failed as expected" if r else "DID NOT FAIL"))
    return 0 if r else 1


def safe_line(ln):
    """D46: a pin line with its name field masked (safe())."""
    if ln is None:
        return "no pin line"
    f = ln.split()
    return " ".join(f[:3] + [safe(" ".join(f[3:-2]))] + f[-2:])


def main(argv):
    global MIN_ROWS
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--unit", help="one units.toml unit name (exact, or the .obj basename)")
    ap.add_argument("--apply", action="store_true", help="append pins.txt lines to config/retail/symbols_pin.txt")
    ap.add_argument("--rows", default=str(CAND), help="candidates jsonl (default derived/candidates/candidates.jsonl)")
    ap.add_argument("--control", choices=("positive", "planted", "planted-data", "near-positive", "near-planted",
                                                 "near-callee-planted", "planted-corr", "prov-drop",
                                                 "prov-false-unblocked", "bracket-positive", "bracket-planted",
                                                 "bracket-ambiguous", "fold-alias-planted", "fold-k1-planted",
                                                 "bracket-own-dtor") + AMB_NEW_CTL)
    ap.add_argument("--ambiguous", choices=AMB_CLS,
                    help="5.3 T3: dry writer over the ambiguous rows of one ruled class (D128-D130; T13.2.c3: k1, "
                         "D115, D113, caller-xref); lines to "
                         ".run/<phase>/t3/ambiguous_<class>.txt; --apply appends them to the pin file")
    ap.add_argument("--max", type=int, metavar="N", help="with --ambiguous: the first N bodies (unit, then VA)")
    ap.add_argument("--recheck", choices=("bracket",),
                    help="T13.2: re-run the pin file's 5.3 T3 bracket block through the writer's chain, each body's own "
                         "lines removed; prints the newly refused bodies; exit 0")
    ap.add_argument("--near", action="store_true",
                    help="3.4 T2: name proto_map near-identical rows (near_check + gates); --apply appends near_pins.txt")
    ap.add_argument("--doc", action="store_true", help="also write docs/matching/naming_residue.md (all rows)")
    ap.add_argument("--check-cut", action="store_true",
                    help="every pinned pool site: cut relocation at its offset names the pin; other unresolved: residue or"
                         " ACCEPTED_SITES (STALE if a listed site no longer fails); exit 0/1")
    ap.add_argument("--promote-data", action="store_true",
                    help="3.3 T8 data byte route over site-corr data pins; --apply rewrites the passing lines in place")
    ap.add_argument("--promote-corr", action="store_true",
                    help="4.2 T7 site-corr pins through pin_rows' corr/callee routes; --apply rewrites passing lines")
    ap.add_argument("--task", help="T<n>, required with --promote-corr --apply: names the run in the pin file's comment line (4.3 T15)")
    ap.add_argument("--bss", action="store_true", help="--promote-corr, --control planted-corr: include bss data rows (off by default)")
    ap.add_argument("--min-rows", type=int, default=MIN_ROWS, metavar="N",
                    help=f"4.2 T10: second pass over residue targets blocking >= N pool rows (default {MIN_ROWS})")
    ap.add_argument("--provisional", action="store_true",
                    help="4.2 T11: write docs/matching/provisional_residue.md from the frozen start set")
    ap.add_argument("--check", action="store_true", help="with --provisional: parse the doc back and re-derive; exit 0/1")
    try:
        a = ap.parse_args(argv)
    except SystemExit as e:
        return 0 if e.code == 0 else 2
    if a.min_rows < 1:
        print("name_sites: REFUSED --min-rows takes N >= 1")
        return 2
    MIN_ROWS = a.min_rows
    amb_ctl = a.control in ("bracket-positive", "bracket-planted", "bracket-ambiguous", "fold-alias-planted",
                            "fold-k1-planted", "bracket-own-dtor") + AMB_NEW_CTL
    if a.max is not None and (not a.ambiguous or a.max < 1) or (a.ambiguous or amb_ctl) and (
            a.unit or a.doc or a.near or a.check_cut or a.promote_data or a.promote_corr or a.bss or a.provisional
            or a.check or a.ambiguous and a.control or amb_ctl and a.apply):
        print("name_sites: REFUSED --ambiguous takes only --max N (N >= 1) and --apply; --control bracket-*/fold-* "
              "run alone")
        return 2
    if a.recheck and (len([x for x in argv if x.startswith("--")]) != 1):
        print("name_sites: REFUSED --recheck bracket runs alone")
        return 2
    if a.ambiguous or amb_ctl or a.recheck:
        try:
            img, pubs, procs = load()
            return (run_ambiguous(img, pubs, procs, a.ambiguous, a.max, a.apply) if a.ambiguous
                    else run_amb_recheck(img, pubs, procs) if a.recheck
                    else run_amb_control(img, pubs, procs, a.control))
        except Refuse as e:
            print(f"name_sites: REFUSED {e}")
            return 2
    prov_ctl = a.control in ("prov-drop", "prov-false-unblocked")
    if a.check and not a.provisional or (a.provisional or prov_ctl) and (
            a.apply or a.unit or a.doc or a.near or a.check_cut or a.promote_data or a.promote_corr or a.bss
            or a.control and not prov_ctl or prov_ctl and a.check):
        print("name_sites: REFUSED --provisional takes only --check; --control prov-* runs alone (or with --provisional)")
        return 2
    if a.provisional or prov_ctl:
        try:
            return run_prov_check(a.control) if a.check or prov_ctl else run_provisional()
        except Refuse as e:
            print(f"name_sites: REFUSED {e}")
            return 2
    if a.promote_corr and (a.control or a.check_cut or a.unit or a.near or a.promote_data or a.doc):
        print("name_sites: REFUSED --promote-corr takes no --control/--check-cut/--unit/--near/--promote-data/--doc")
        return 2
    if a.promote_corr and a.apply and not re.fullmatch(r"T\d+(\.c\d+)?", a.task or "") or a.task and not (a.promote_corr and a.apply):
        print(f"name_sites: REFUSED --task T<n> goes with --promote-corr --apply, and that needs it (got {a.task!r})")
        return 2
    if a.bss and not (a.promote_corr or a.control == "planted-corr"):
        print("name_sites: REFUSED --bss goes with --promote-corr or --control planted-corr")
        return 2
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
        if a.promote_corr:
            return run_promote_corr(a.bss, a.apply, a.task)
        if a.control == "planted-corr":
            return run_planted_corr(a.bss)
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
        if a.control == "near-callee-planted":
            return run_near_callee_planted(img, pubs, procs)
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
        try:  # 4.2 T10: pin_rows replant decides every new line (C0193)
            kept, dropped = apply_replant(pins, lines)
        except Refuse as e:
            print(f"name_sites: REFUSED {e}")
            return 2
        with open(OUT / "apply.jsonl", "w", encoding="utf-8", newline="\n") as f:
            for ln in lines:
                f.write(json.dumps({"line": ln, "kept": ln in kept, "why": dropped.get(ln)}, sort_keys=True) + "\n")
        print(f"  replant: kept {len(kept)} of {len(lines)}; dropped "
              + (", ".join(f"{w} {k}" for w, k in sorted(Counter(w.split(";")[0] for w in dropped.values()).items()))
                 or "0") + f" (wrote {OUT.relative_to(REPO).as_posix()}/apply.jsonl)")
        lines = kept
        if not lines:
            return 0
        old = SYMS_PIN.read_bytes().decode("utf-8")
        hdr = (f"# 3.1 T6 name_sites{' --unit ' + unit if unit else ''}: {len(lines)} lines; evidence "
               f"docs/evidence/correspondence.md \"## Site-correspondence naming (3.1 T5)\"; 4.2 T10: pin_rows replant "
               f"kept, --min-rows {MIN_ROWS}")
        sep = "" if not old or old.endswith("\n") else "\n"
        SYMS_PIN.write_bytes((old + sep + hdr + "\n" + "".join(ln + "\n" for ln in lines)).encode("utf-8"))
        print(f"  applied {len(lines)} line(s) to {SYMS_PIN}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
