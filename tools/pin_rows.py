"""pin_rows (4.2 T5; D119, D113, D115, D118, I10, M23): the pin-row check over config/retail/symbols_pin.txt.

CLI: pin_rows.py --build DIR [--config CFG]  (DIR: the built objects, `build.py --out`) -> one `FAIL pin-row ...` line
  per failing row or I10 use, then counts by class. Exit 0 no fail; 1 any fail; 2 refusal (an input unreadable or not
  equal to its config/inputs.manifest.jsonl row).
Library: check(cfg, build_dir, list_path=MATCHED) -> [fail line] (the gate; I10 over list_path's functions);
  row_ok(symbol, cfg=None) -> True | reason (D118 `_pin_row`); state(cfg).replant(pin, accepted) -> a State over another
  pin file / accepted list, inputs and site index shared (the gate's pin-plant controls).
Per row: grammar; status against tag (TAGS); the name a PDB public where the tag claims one; VA on the image, func VA
  with raw bytes; proto and retail section names equal; duplicates (same row twice, name at two VAs here or in
  symbols.txt, a second name at the VA that is neither a fold alias nor the undecorated library name); byte route by tag:
  callee and fold masked bytes plus the callee name rule, vtable slots (4.5 T16.2.c4: the retail slot count too,
  `slot-count-differs`), string and `__real@` bytes, label = VA, hand
  rows named in docs/evidence/compiler_pin.md, corr-verified `+site-bundle` and `+slot-bytes` re-derived (D113);
  corr-verified `+slot-names` (5.3 T2.c4, M30): each slot's retail VA named, accepted, by a slot target public, slot
  counts equal (`route-slotnames:*`); `+unit-bracket+callee-bytes` (5.3 T2.c4, T1 ruling): the callee route plus the
  module's matched-neighbour bracket, the unit's text range and no second masked-equal body in the bracket
  (`route-bracket:*`); site test on every row with a public.
Site index (locked): each symbols.txt func whose name is one PDB public P, PDB proc length n, prototype and retail
  masked-equal over n; each outgoing mask -> (proto dest, retail dest, caller P). Agree: (P(name), VA). Conflict: P(name)
  reaching another VA, or VA reached from a proto dest whose publics exclude the name.
I10: every pin row a matched function's built object relocates through (function text relocations) has an accepted
  status (ACCEPTED; site-corr never, D102) and passes; a relocation name in neither table fails. A same-section
  static is skipped only where byte_compare.resolve falls back: in no table, inside the function's extent (4.5 T16.2.c4).
Dispositions (D119): fold case: a proto dest with 2+ publics agrees when the retail name is one of them; fold site
  (4.2 T5.c4): a conflicting site from the row's proto dest with 2+ publics whose retail dest has no table name does
  not count (the site cannot say which member it calls) while the row has 1+ agreeing site; an undecorated
  library name in symbols.txt equal to the row's (`_F@N` -> `F`, `?M@C@...` -> `C_M`) is one name; a literal copy's site
  conflict is waived only by a row of config/pin_accepted.txt (`0x<VA><TAB><name><TAB><reason><TAB><conflict>`,
  conflict: the sorted comma-joined tokens `r:0x<VA8>` (each retail dest != VA from the row's proto dests, fold sites
  too) and `p:0x<VA8>` (each proto dest reaching VA whose publics exclude the name); 5.1 T3) whose conflict equals the
  row's, else `site-conflict-both|only:accepted-conflict-differs`; a waiver no conflict uses fails `accepted-stale ...
  unused` (4.5 T16.2.c4).
Near rows (4.5 T16.1, D115): 1+ agreeing site, else `route-near:no-agreeing-site`, except NEAR_NO_SITE (va, name)
  rows; a listed row with an agreeing site, or no near row of that (va, name), fails `route-near:stale-exception`.
  Not re-derived here: near_check (`+retail-xref` rows rest on the site test). Data-bytes route (4.5 T16.1): proto and
  retail bytes equal, proto `.reloc` dwords masked both sides, over min(next proto public - P capped EXT_CAP, next
  retail table name - VA, the PDB S_GDATA32/S_LDATA32 type size at P when one size), else `route-global:data-differs`.
  Limits (docs/ops/validation.md "4.2 Pin-row check"): a byte twin with no site
  and a wrong name no site reaches pass; a pass cannot prove a name true.
Inputs: provenance_lint.inputs (PDB, EXE, XBE pinned to the manifest; the two prototype files re-read for raw publics,
  proc lengths, section flags, data symbols and TPI type sizes (pdb_types)), symbols.txt, the pin file, the evidence
  doc, config/pin_accepted.txt, config/matched_set.toml, config/retail/units.toml (bracket). No derived/. Output: names, VAs, counts only; `??_C@` names
  masked (D46).
"""
# was (docstring before 5.1 T3): ... waived only by a row of config/pin_accepted.txt (`0x<VA><TAB><name><TAB><reason>`);
#   a waiver of any conflict at the row.
# was (docstring before 4.5 T16.1):
# Not re-derived here: near_check (near-corr and `+retail-xref` rows rest on the site test), the data-bytes record route
#   (its rows: name kind and site test). Limits (docs/ops/validation.md "4.2 Pin-row check"): a byte twin with no site
#   and a wrong name no site reaches pass; a pass cannot prove a name true.
# Inputs: provenance_lint.inputs (PDB, EXE, XBE pinned to the manifest; the two prototype files re-read for raw publics,
#   proc lengths and section flags), symbols.txt, the pin file, the evidence doc, config/pin_accepted.txt,
#   config/matched_set.toml. No derived/. Output: names, VAs, counts only; `??_C@` names masked (D46).
import argparse
import bisect
import copy
import hashlib
import re
import struct
import sys
import time
import tomllib
from collections import Counter, defaultdict
from pathlib import Path

from iced_x86 import Code, Decoder

sys.path.insert(0, str(Path(__file__).resolve().parent))

import byte_compare  # noqa: E402
import coff_info  # noqa: E402
import matched_set  # noqa: E402
import pdb_info  # noqa: E402
import pdb_types  # noqa: E402
import pe_info  # noqa: E402
import provenance_lint  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

REPO = Path(__file__).resolve().parent.parent
PIN = REPO / "config" / "retail" / "symbols_pin.txt"
SYMS = REPO / "config" / "retail" / "symbols.txt"
UNITS = REPO / "config" / "retail" / "units.toml"  # 5.3 T2.c4: the bracket route's unit text ranges
EVID = REPO / "docs" / "evidence" / "compiler_pin.md"
ACCEPTED = REPO / "config" / "pin_accepted.txt"
MATCHED = REPO / "config" / "matched_set.toml"
RE_ROW = re.compile(r"^0x[0-9A-F]{8} (func|data) (-|0x[0-9A-F]+) \S+ \S+ \S+$")
STATUSES = ("disasm-verified", "corr-verified", "site-corr", "near-corr", "boundary")
ACCEPTED_STATUS = ("disasm-verified", "corr-verified", "near-corr", "boundary")  # I10; site-corr alone refused (D102)
PE_EXEC = 0x20000000 | 0x20  # IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_CODE
VT_CAP, STR_CAP, EXT_CAP = 64, 0x10000, 0x1000
PP = "proto-pdb-public+"
# (kind, tag) -> (status, route, public claimed). From name_sites pin_line, the near apply, the pin file's hand sections
# (4.2 T3) and D113. route None: the tag claims no byte route (site test only).
TAGS = {
    ("data", PP + "site-corr+literal-bytes"): ("disasm-verified", "string", True),
    ("data", PP + "site-corr+real-bytes"): ("disasm-verified", "float", True),
    ("data", PP + "site-corr+vtable-slots"): ("disasm-verified", "vtable", True),
    ("data", PP + "site-corr+data-bytes"): ("disasm-verified", "global", True),
    ("data", PP + "site-corr+site-bundle"): ("corr-verified", "bundle", True),  # D113: bss, __TI, initialized globals
    ("data", PP + "site-corr+slot-bytes"): ("corr-verified", "slots", True),  # D113: vtables
    ("data", PP + "site-corr+slot-names"): ("corr-verified", "slotnames", True),  # 5.3 T2.c4 (M30, R212): vtables
    ("func", PP + "fold-alias+callee-bytes"): ("disasm-verified", "fold", True),
    ("func", PP + "site-corr+callee-bytes"): ("disasm-verified", "func", True),
    ("func", PP + "unit-bracket+callee-bytes"): ("disasm-verified", "bracket", True),  # 5.3 T2.c4 (T1 ruling)
    ("func", PP + "proto-neighbour+retail-bytes"): ("disasm-verified", "func", True),
    ("func", PP + "anchor-window+caller-xref"): ("near-corr", "near", True),  # D115: func only
    ("data", PP + "site-corr"): ("site-corr", None, True),
    ("func", PP + "site-corr"): ("site-corr", None, True),
    ("func", "retail-disasm+cut-boundary"): ("boundary", "label", False),
    ("data", "retail-disasm"): ("disasm-verified", "label", False),
    # hand tags, allowed explicitly: the name must appear in docs/evidence/compiler_pin.md
    ("func", "retail-disasm"): ("disasm-verified", "hand", False),
    ("data", "retail-disasm+cl-listing"): ("disasm-verified", "hand", False),
    ("data", "crt-absolute-0"): ("disasm-verified", "hand", False),
    ("func", PP + "retail-xref"): ("disasm-verified", "xref", True),
    ("func", "proto-pdb:default.pdb+proto-map+retail-xref"): ("disasm-verified", "xref", True),
    ("func", "proto-pdb:default.pdb+proto-map"): ("disasm-verified", "hand", True),
    ("func", "proto-pdb:default.pdb+retail-disasm"): ("disasm-verified", "hand", True),
    ("func", PP + "retail-disasm"): ("disasm-verified", "hand", True),
    ("data", PP + "retail-disasm"): ("disasm-verified", "hand", True),
    ("data", PP + "retail-data"): ("disasm-verified", "hand", True),
}
# 4.5 T16.1 (G2, D115): near rows with no agreeing site, allowed by (va, name). The list only shrinks (C0286).
# 5.1 T3.1 (ruled no slot route, T3 review): kept with their evidence, docs/ops/validation.md "5.1 Near re-check";
#   control pin-near-listed: a fourth unlisted no-site near row fails route-near:no-agreeing-site.
_NNS = "virtual Init, no rel32 caller in pin_rows' site index; D115 evidence is name_sites near_check; re-check gen 5"
NEAR_NO_SITE = {
    (0x000688EA, "?Init@RatRacePreludeCutscene@@MAEXXZ"): _NNS,
    (0x00068E09, "?Init@RatRaceDeathCutscene@@MAEXXZ"): _NNS,
    (0x000A8405, "?Init@VinceDropsInCutscene@@MAEXXZ"): _NNS,
}
PDB_DATA = (0x110C, 0x110D, 0x1007, 0x1008)  # S_LDATA32, S_GDATA32 and their _ST forms: typind u32, off u32, seg u16


class Refuse(Exception):
    pass


def mask(n):  # D46: names carrying string text
    if n.startswith("??_C@"):
        return "??_C@ (string literal)"
    if n.startswith(("s_", "u_")):
        return "string label"
    return n


def undecorated(n):  # D119: the library name symbols.txt carries for a decorated public, else None
    m = re.fullmatch(r"_([A-Za-z_]\w*)@\d+", n)
    if m:
        return m.group(1)
    m = re.match(r"\?([A-Za-z_]\w*)@([A-Za-z_]\w*)@", n)
    return f"{m.group(2)}_{m.group(1)}" if m else None


def mask_proc(buf, va, lo, hi):
    """Decode buf at va -> (masks [(off, 4, 'rel'|'abs')], decode errors): rel32 of near call/jmp/jcc, every 4-byte
    imm/disp in [lo, hi). Same rule as proto_map.mask_proc (not imported: D119)."""
    masks, errors, off = [], 0, 0
    d = Decoder(32, buf, ip=va)
    while d.can_decode:
        ins = d.decode()
        if ins.code == Code.INVALID:
            errors += 1
            off += ins.len or 1
            continue
        c = d.get_constant_offsets(ins)
        if c.has_immediate and c.immediate_size == 4:
            o = off + c.immediate_offset
            if ins.is_call_near or ins.is_jmp_near or ins.is_jcc_short_or_near:
                masks.append((o, 4, "rel"))
            elif lo <= struct.unpack_from("<I", buf, o)[0] < hi:
                masks.append((o, 4, "abs"))
        if c.has_displacement and c.displacement_size == 4:
            o = off + c.displacement_offset
            if lo <= struct.unpack_from("<I", buf, o)[0] < hi:
                masks.append((o, 4, "abs"))
        off += ins.len
    return sorted(masks), errors


def unmasked(n, masks):
    pos = tot = 0
    for o, sz, _k in masks:
        tot += max(0, o - pos)
        pos = max(pos, o + sz)
    return tot + max(0, n - pos)


def target(buf, va, off, kind):
    if kind == "rel":
        return (va + off + 4 + struct.unpack_from("<i", buf, off)[0]) & 0xFFFFFFFF
    return struct.unpack_from("<I", buf, off)[0]


class Images:
    """Prototype PE and retail XBE bytes, sections, exec flags, the masked-equal primitive (name_sites.Images' rules)."""

    def __init__(self, exe, pe, xb, x):
        self.exe, self.xb, self.x = exe, xb, x
        self.base = pe["imageBase"]
        e = struct.unpack_from("<I", exe, 0x3C)[0]
        sh = e + 24 + struct.unpack_from("<H", exe, e + 20)[0]
        self.psecs = [dict(s, exec=bool(struct.unpack_from("<I", exe, sh + 40 * i + 36)[0] & PE_EXEC))
                      for i, s in enumerate(pe["sections"])]
        opt = e + 24
        size_img = struct.unpack_from("<I", exe, opt + 56)[0]
        ndd = struct.unpack_from("<I", exe, opt + 92)[0]
        rel_rva, rel_sz = struct.unpack_from("<II", exe, opt + 96 + 8 * 5) if ndd > 5 else (0, 0)
        self.plo, self.phi = self.base, self.base + size_img
        self.prelocs = set()
        ro = next((s["rawptr"] + rel_rva - s["va"] for s in pe["sections"]
                   if rel_sz and s["va"] <= rel_rva < s["va"] + s["rawsize"]), None)
        if ro is not None:
            blob, o = exe[ro:ro + rel_sz], 0
            while o + 8 <= len(blob):
                page, size = struct.unpack_from("<II", blob, o)
                if size < 8:
                    break
                for k in range(o + 8, min(o + size, len(blob)) - 1, 2):
                    w = struct.unpack_from("<H", blob, k)[0]
                    if w >> 12 == 3:
                        self.prelocs.add(self.base + page + (w & 0xFFF))
                o += size
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
        if s is None or va - self.base - s["va"] + n > s["rawsize"]:
            return None
        o = s["rawptr"] + va - self.base - s["va"]
        return self.exe[o:o + n]

    def rbytes(self, va, n):
        s = self.rsec(va)
        if s is None or va - s["virtualAddr"] + n > s["rawSize"]:
            return None
        o = xbe_info.va_to_off(self.x, va)
        return self.xb[o:o + n]

    def match(self, pva, rva, n, allow_const=True):
        """-> None, or proto masks. A retail abs mask with no proto mask, no .reloc entry and equal raw dwords is a
        constant (dropped when allow_const); then the same (off, kind) mask list and equal unmasked bytes."""
        pb, rb = self.pbytes(pva, n), self.rbytes(rva, n)
        if pb is None or rb is None or n <= 0:
            return None
        mp, ep = mask_proc(pb, pva, self.plo, self.phi)
        mr, er = mask_proc(rb, rva, self.rlo, self.rhi)
        po = {o for o, _n, _k in mp}
        consts = {o for o, _n, k in mr if allow_const and k == "abs" and o not in po and pva + o not in self.prelocs
                  and pb[o:o + 4] == rb[o:o + 4]}
        if ep != er or [(o, k) for o, _n, k in mp] != [(o, k) for o, _n, k in mr if o not in consts]:
            return None
        pb, rb = bytearray(pb), bytearray(rb)
        for o, w, _k in mp:
            pb[o:o + w] = rb[o:o + w] = b"\0" * w
        return mp if pb == rb else None


def table_lines(path):
    for i, ln in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        if ln.strip() and not ln.startswith("#"):
            yield i, ln, ln.split()


class State:
    """Parsed inputs and per-row results for one config and pin file; built once per process (cached in _STATE)."""

    def __init__(self, cfg):
        self.pin, self.acc_path = PIN, ACCEPTED
        if cfg not in provenance_lint._INPUTS:  # share the gate's pinned parse (provenance layer)
            try:
                provenance_lint._INPUTS[cfg] = provenance_lint.inputs(cfg)
            except provenance_lint.Refuse as e:
                raise Refuse(str(e))
        xbe = provenance_lint._INPUTS[cfg][1]
        c = vv_inputs.load_config(cfg)
        try:  # the files inputs() just pinned; Pdb keeps no raw publics nor EXE sections
            exe = vv_inputs.input_path("prototype_dir", "default.exe", c).read_bytes()
            pe = pe_info.parse_pe(exe)
            raw = vv_inputs.input_path("prototype_dir", "default.pdb", c).read_bytes()
            pdb = pdb_info.parse_pdb(raw)
            st = pdb_info.read_streams(raw)["streams"]  # 4.5 T16.1: data symbols (DBI symrec) and TPI for type sizes
            pdb_types.load_tpi(st)
            symrec = struct.unpack_from("<H", st[3], 20)[0]
            dsyms = [struct.unpack_from("<IIH", p, 0) for k, p in pdb_info.iter_records(st[symrec])
                     if k in PDB_DATA] if symrec < len(st) and st[symrec] else []
        except (OSError, pe_info.NotPe, pdb_info.NotPdb, struct.error) as e:
            raise Refuse(f"default.exe/default.pdb unreadable: {e}")
        self.img = img = Images(exe, pe, xbe.data, xbe.x)
        self.xbe_sha1 = hashlib.sha1(xbe.data).hexdigest()
        secs = pe["sections"]
        self.pubs, self.byname, self.procs = defaultdict(set), defaultdict(set), {}
        self.pmods, self.mprocs = defaultdict(set), defaultdict(set)  # 5.3 T2.c4 bracket: proc VA <-> PDB module
        for p in pdb["publics"]:
            if 1 <= p["seg"] <= len(secs):
                v = img.base + secs[p["seg"] - 1]["va"] + p["off"]
                self.pubs[v].add(p["name"])
                self.byname[p["name"]].add(v)
        for m in pdb["modules"]:
            for x in m["procs"]:
                if 1 <= x["seg"] <= len(secs):
                    v = img.base + secs[x["seg"] - 1]["va"] + x["off"]
                    self.procs[v] = max(self.procs.get(v, 0), x["length"])
                    self.pmods[v].add(m["name"])
                    self.mprocs[m["name"]].add(v)
        try:  # 5.3 T2.c4 bracket inputs: unit text ranges, matched functions (name -> retail VA, size)
            self.utext = {u["name"]: (u["text_start"], u["text_end"])
                          for u in tomllib.loads(UNITS.read_text(encoding="utf-8"))["unit"]
                          if "text_start" in u and "text_end" in u}
            self.matched = {e["symbol"]: (e["va"], e["size"]) for e in matched_set.load(MATCHED)
                            if e["kind"] == "function"}
        except (OSError, tomllib.TOMLDecodeError, matched_set.Refuse) as e:
            raise Refuse(f"units.toml/matched_set.toml unreadable: {e}")
        self.banc = {}  # module -> sorted bracket anchors (proto VA, retail VA, retail size), built on first use
        self.pub_vas = sorted(self.pubs)
        self.dtypes = defaultdict(set)  # proto VA -> PDB data symbol type indices
        for ti, off, seg in dsyms:
            if 1 <= seg <= len(secs):
                self.dtypes[img.base + secs[seg - 1]["va"] + off].add(ti)
        self.evid = EVID.read_text(encoding="utf-8")
        self.sy_name, self.sy_va = defaultdict(set), defaultdict(set)
        self.sy_func = []
        for _i, _ln, f in table_lines(SYMS):
            n, v = " ".join(f[3:-2]), int(f[0], 16)
            self.sy_name[n].add(v)
            self.sy_va[v].add(n)
            if f[1] == "func":
                self.sy_func.append((v, n))
        self.site_index()
        self.rows, self.header = self.parse_rows()
        self.accepted = self.load_accepted()
        self.run_rows()

    def replant(self, pin, accepted=None):
        """A copy over another pin file / accepted list: inputs, images and site index shared; rows re-run."""
        s = copy.copy(self)
        s.pin, s.acc_path = Path(pin), Path(accepted or self.acc_path)
        s.rows, s.header = s.parse_rows()
        s.accepted = s.load_accepted()
        s.run_rows()
        return s

    # -- inputs
    def parse_rows(self):
        rows, header = [], []
        for i, ln in enumerate(self.pin.read_text(encoding="utf-8").splitlines(), 1):
            if ln.startswith("#"):
                header.append(ln)
                continue
            if not ln.strip():
                continue
            f = ln.split()
            r = {"line": i, "fails": [], "notes": [], "size": None, "status": "", "tag": "", "kind": ""}
            if not RE_ROW.match(ln) or f[-1] not in STATUSES:
                r["fails"].append("grammar")
                r["name"] = " ".join(f[3:-2]) if len(f) >= 4 else ""
                r["va"] = int(f[0], 16) if re.fullmatch(r"0x[0-9A-Fa-f]+", f[0]) else None
                rows.append(r)
                continue
            r.update(va=int(f[0], 16), kind=f[1], size=None if f[2] == "-" else int(f[2], 16), name=f[3], tag=f[4],
                     status=f[5])
            rows.append(r)
        return rows, header

    def load_accepted(self):
        out = {}
        if self.acc_path.is_file():
            for i, ln in enumerate(self.acc_path.read_text(encoding="utf-8").splitlines(), 1):
                if not ln.strip() or ln.startswith("#"):
                    continue
                f = ln.split("\t")
                # 5.1 T3: a 4th field, the conflict the waiver covers; was: 3 fields, a waiver of any conflict
                toks = frozenset(f[3].split(",")) if len(f) == 4 else frozenset()
                if (len(f) != 4 or not re.fullmatch(r"0x[0-9A-F]{8}", f[0]) or not f[2].strip() or not toks
                        or not all(re.fullmatch(r"[rp]:0x[0-9A-F]{8}", t) for t in toks)):
                    raise Refuse(f"{self.acc_path.name}:{i}: want 0x<VA><TAB><name><TAB><reason><TAB><conflict> "
                                 f"(conflict: comma-joined r:0x<VA8> / p:0x<VA8>, non-empty)")
                out[(int(f[0], 16), f[1])] = (i, toks)
        return out

    # -- site index (locked: symbols.txt names, PDB proc lengths)
    def site_index(self):
        img = self.img
        self.by_pd, self.by_rd = defaultdict(lambda: defaultdict(list)), defaultdict(lambda: defaultdict(list))
        for v, n in self.sy_func:
            ps = self.byname.get(n, ())
            if len(ps) != 1:
                continue
            p = next(iter(ps))
            ln = self.procs.get(p)
            mp = img.match(p, v, ln) if ln else None
            if mp is None:
                continue
            pb, rb = img.pbytes(p, ln), img.rbytes(v, ln)
            for o, _w, k in mp:
                pd, rd = target(pb, p, o, k), target(rb, v, o, k)
                if k == "rel" and p <= pd < p + ln:
                    continue
                self.by_pd[pd][rd].append(p)
                self.by_rd[rd][pd].append(p)

    # -- byte checks (name_sites.check's rules, D119's fold case)
    def callee(self, pva, rva, n):
        """None when masked-equal and every outgoing mask whose retail dest is a table start name-agrees with the proto
        dest's publics (D119: one of 2+ agrees); a callee with < 8 unmasked bytes also needs one agreement."""
        img = self.img
        mp = img.match(pva, rva, n, allow_const=False)
        if mp is None:
            return "callee-bytes-differ"
        pb, rb = img.pbytes(pva, n), img.rbytes(rva, n)
        agree = 0
        for o, _w, k in mp:
            pd, rd = target(pb, pva, o, k), target(rb, rva, o, k)
            if k == "rel" and rva <= rd < rva + n:
                continue
            rn = {x for x in self.tab_va.get(rd, ()) if (rd, x) not in self.near}  # near-corr names never agree
            if not rn:
                continue
            if not rn & self.pubs.get(pd, set()):
                return "callee-name-rule"
            agree += 1
        if unmasked(n, mp) < 8 and not agree:
            return "callee-short"
        return None

    def vt_slots(self, pt, t):
        i = bisect.bisect_right(self.pub_vas, pt)
        nslot = min((self.pub_vas[i] - pt) // 4 if i < len(self.pub_vas) else VT_CAP, VT_CAP)
        slots = []
        for k in range(nslot):
            pd, rd = self.img.pbytes(pt + 4 * k, 4), self.img.rbytes(t + 4 * k, 4)
            if pd is None or rd is None:
                break
            pv = struct.unpack("<I", pd)[0]
            if not self.img.pexec(pv):
                break
            slots.append((pv, struct.unpack("<I", rd)[0]))
        return slots

    def kind_of(self, n, pt):
        if n.startswith("??_7"):
            return "vtable"
        if n.startswith("??_C@"):
            return "string"
        if n.startswith("__real@"):
            return "float"
        return "func" if self.img.pexec(pt) else "data"

    def rt_slots(self, va):
        """Retail vtable slot count at VA: dwords into a section executable in the proto, up to the next table name
        or VT_CAP (corr_route's count, 4.5 T16.2.c4 shared with literal)."""
        img = self.img
        pexec = {s["name"] for s in img.psecs if s["exec"]}
        nxt = min((v for v in self.tab_va if v > va), default=va + 4 * VT_CAP)
        k = 0
        while k < VT_CAP and va + 4 * k < nxt:
            b = img.rbytes(va + 4 * k, 4)
            s = img.rsec(struct.unpack("<I", b)[0]) if b else None
            if s is None or s["name"] not in pexec:
                break
            k += 1
        return k

    def anchors(self, m):
        """Bracket anchors of PDB module m: its procs with a public name in matched_set.toml, as sorted (proto VA,
        retail VA, retail size); several matched names at one proc: the lowest retail VA."""
        if m not in self.banc:
            an = []
            for x in sorted(self.mprocs.get(m, ())):
                hit = sorted(self.matched[n] for n in self.pubs.get(x, ()) if n in self.matched)
                if hit:
                    an.append((x, *hit[0]))
            self.banc[m] = an
        return self.banc[m]

    def bracket(self, p, va, ln):
        """5.3 T2.c4 (T1 ruling; pool_buckets' bracket :690-714, gate-verified anchors): P's proc in exactly one PDB
        module M; anchors = M's matched procs (anchors()); VA in [retail end of the nearest anchor below P, retail start
        of the nearest anchor above P) and in M's units.toml text range; no other retail offset in that window
        masked-equal to P over its proc length (Images.match, constants allowed) -> None, else a fail reason."""
        ms = self.pmods.get(p, set())
        if len(ms) != 1:
            return "module-not-unique"
        m = next(iter(ms))
        if m not in self.utext:
            return "unit-unknown"
        an = self.anchors(m)
        lo = max((a for a in an if a[0] < p), default=None)
        hi = min((a for a in an if a[0] > p), default=None)
        if lo is None or hi is None:
            return "no-anchor"
        w0, w1 = lo[1] + lo[2], hi[1]
        if not w0 <= va < w1:
            return "outside-bracket"
        if not self.utext[m][0] <= va < self.utext[m][1]:
            return "outside-unit"
        pb = self.img.pbytes(p, ln)
        mp, _e = mask_proc(pb, p, self.img.plo, self.img.phi)
        best, pos = (0, 0), 0  # (length, offset) of P's longest unmasked run: the search key
        for o, sz, _k in mp + [(ln, 0, "")]:
            if o - pos > best[0]:
                best = (o - pos, pos)
            pos = max(pos, o + sz)
        s = self.img.rsec(w0)
        end = min(w1 + ln, s["virtualAddr"] + s["rawSize"]) if s else w0
        win = self.img.rbytes(w0, end - w0) or b""
        key, ko = pb[best[1]:best[1] + best[0]], best[1]
        cands = range(w0, w1) if not key else []
        if key:
            i = win.find(key)
            while i >= 0:
                cands.append(w0 + i - ko)
                i = win.find(key, i + 1)
        for t in cands:
            if t != va and w0 <= t < w1 and self.img.match(p, t, ln) is not None:
                return "ambiguous"
        return None

    def slot_names(self, n, p, va):
        """5.3 T2.c4 (M30, R212): a `??_7` vtable named by its slots' names: every proto slot (vt_slots) has its retail
        slot VA named by one of the slot target's publics (D119: or its undecorated library name), the name from
        symbols.txt or a passing pin row of an accepted status; and rt_slots(VA) == the proto slot count -> None,
        else a fail reason. No byte compare and no site (the names carry it)."""
        slots = self.vt_slots(p, va) if n.startswith("??_7") else []
        if not slots:
            return "vtable-no-slots"
        for pv, rv in slots:
            want = set(self.pubs.get(pv, ()))
            want |= {undecorated(x) for x in want} - {None}
            rows = self.by_row_va.get(rv, ())
            names = set(self.sy_va.get(rv, ())) | {x["name"] for x in rows}
            ok = set(self.sy_va.get(rv, ())) | {x["name"] for x in rows
                                                 if x["status"] in ACCEPTED_STATUS and not x["fails"]}
            if not names:
                return "slot-unnamed"
            if not ok & want:
                return "slot-site-corr" if names & want else "slot-name-differs"
        return None if self.rt_slots(va) == len(slots) else "slot-count-differs"

    def literal(self, route, n, p, va):
        """-> True, False, or a fail reason (vtable: `slot-count-differs`)."""
        img = self.img
        if route == "vtable":
            slots = self.vt_slots(p, va)
            # was: the proto's slots a prefix of retail's passed (no retail slot count)
            if not (bool(slots) and all(pv in self.procs and not self.callee(pv, rv, self.procs[pv]) for pv, rv in slots)):
                return False
            return True if self.rt_slots(va) == len(slots) else "slot-count-differs"  # 4.5 T16.2.c4 (G14)
        if route == "string":
            step, nul = (2, b"\0\0") if n.startswith("??_C@_1") else (1, b"\0")
            k = 0
            while k < STR_CAP:
                c = img.pbytes(p + k, step)
                if c is None:
                    return False
                k += step
                if c == nul:
                    break
            else:
                return False
            pb = img.pbytes(p, k)
            return pb is not None and pb == img.rbytes(va, k)
        h = n[len("__real@"):]
        if len(h) not in (8, 16) or any(c not in "0123456789abcdefABCDEF" for c in h):
            return False
        return img.rbytes(va, len(h) // 2) == bytes.fromhex(h)[::-1]

    def data_equal(self, p, va):
        """4.5 T16.1 (G3): proto P and retail VA bytes equal, proto `.reloc` dwords masked both sides, over min(next
        proto public - P capped EXT_CAP, next retail table name - VA, the PDB data type size at P when one size)."""
        img = self.img
        i = bisect.bisect_right(self.pub_vas, p)
        bounds = [min(self.pub_vas[i] - p, EXT_CAP) if i < len(self.pub_vas) else EXT_CAP]
        j = bisect.bisect_right(self.tab_sorted, va)
        if j < len(self.tab_sorted):
            bounds.append(self.tab_sorted[j] - va)
        sizes = set()
        for ti in self.dtypes.get(p, ()):
            try:
                sizes |= pdb_types.tsize(ti)
            except pdb_types.TypeSizeError:
                pass
        if len(sizes) == 1:
            bounds += sizes
        ext = min(bounds)
        pb, rb = img.pbytes(p, ext), img.rbytes(va, ext)
        if pb is None or rb is None:
            return False
        pb, rb = bytearray(pb), bytearray(rb)
        for k in range(-3, ext):
            if p + k in img.prelocs:
                for q in range(max(k, 0), min(k + 4, ext)):
                    pb[q] = rb[q] = 0
        return pb == rb

    # -- per-row tests
    def run_rows(self):
        img, rows = self.img, self.rows
        self.tab_va, self.near = defaultdict(set), set()
        for v, ns in self.sy_va.items():
            self.tab_va[v] |= ns
        by_name, by_va, pairs = defaultdict(set), defaultdict(set), Counter()
        live = [r for r in rows if "grammar" not in r["fails"]]
        for r in live:
            self.tab_va[r["va"]].add(r["name"])
            if r["status"] == "near-corr":
                self.near.add((r["va"], r["name"]))
            by_name[r["name"]].add(r["va"])
            by_va[r["va"]].add(r["name"])
            pairs[(r["va"], r["name"])] += 1
        self.tab_sorted = sorted(self.tab_va)  # 4.5 T16.1: next retail-side name (data_equal)
        self.by_row_name, self.by_row_va = defaultdict(list), defaultdict(list)  # by_row_va: 5.3 T2.c4 slot names
        for r in rows:
            self.by_row_name[r.get("name", "")].append(r)
        for r in live:
            self.by_row_va[r["va"]].append(r)
        for r in live:
            t = r["cls"] = TAGS.get((r["kind"], r["tag"]))
            if t is None:
                r["fails"].append("unclassified")
            elif t[0] != r["status"]:
                r["fails"].append("status-vs-tag")
            n, va = r["name"], r["va"]
            if len(by_name[n]) > 1:
                r["fails"].append("dup-name-two-vas")
            if pairs[(va, n)] > 1:
                r["fails"].append("dup-same-row")
            if self.sy_name.get(n, {va}) != {va}:
                r["fails"].append("dup-symbols-txt-other-va")
            und = undecorated(n)
            others = (by_va[va] | {x for x in self.sy_va.get(va, ()) if x != und}) - {n}  # D119: one name
            claimed = t[2] if t else r["tag"].startswith("proto-pdb")
            r["protos"] = sorted(self.byname.get(n, ()))
            if claimed and not r["protos"]:
                r["fails"].append("name-not-public")
            sec = img.rsec(va)
            if sec is None and r["tag"] != "crt-absolute-0":
                r["fails"].append("va-off-image")
            elif sec is not None and r["kind"] == "func" and img.rbytes(va, 1) is None:
                r["fails"].append("func-no-bytes")
            if sec is not None and r["protos"] and all((img.psec(p) or {}).get("name") != sec["name"] for p in r["protos"]):
                r["fails"].append("section-differs")
            if r["kind"] == "func" and r["protos"] and not any(img.pexec(p) for p in r["protos"]):
                r["fails"].append("func-proto-not-exec")
            if others and not any(others <= self.pubs.get(p, set()) for p in r["protos"]) and t:
                if t[1] == "fold":
                    r["fails"].append("fold-other-name-not-alias")
                elif r["kind"] == "func":
                    r["fails"].append("va-two-names")
            self.route(r)
        for r in live:
            self.site_test(r)
        self.near_listed = set()  # 4.5 T16.1 (G2, D115): near rows need 1+ agreeing site, NEAR_NO_SITE aside
        for r in live:
            if r.get("cls") and r["cls"][1] == "near":
                key, agree = (r["va"], r["name"]), (r.get("site") or (0,))[0]
                if key in NEAR_NO_SITE:
                    self.near_listed.add(key)
                    if agree:
                        r["fails"].append("route-near:stale-exception")
                elif not agree:
                    r["fails"].append("route-near:no-agreeing-site")
        for r in live:
            if r.get("cls") and r["cls"][1] in ("bundle", "slots", "slotnames"):  # was: ("bundle", "slots")
                self.corr_route(r)

    def route(self, r):
        t = r.get("cls")
        if not t or t[1] is None:
            return
        route, n, va, why = t[1], r["name"], r["va"], None
        if route in ("hand", "xref") and n not in self.evid:
            r["fails"].append("hand-name-not-in-evidence")
        if route == "label":
            m = re.fullmatch(r"(?:_s_|boundary_)([0-9A-F]{8})", n)
            why = None if m and int(m.group(1), 16) == va else "label-not-va"
        elif route in ("func", "fold", "bracket"):  # was: ("func", "fold"); bracket: 5.3 T2.c4 (T1 ruling)
            why = "proto-name"
            for p in r["protos"]:
                ln = self.procs.get(p)
                if not ln:
                    why = "no-proc-length"
                    continue
                why = self.callee(p, va, ln)
                if why is None:
                    if r["size"] is not None and r["size"] != ln:
                        why = "size-differs"
                    elif route == "fold" and len(self.pubs.get(p, ())) < 2:
                        why = "fold-single-public"
                    elif route == "bracket":
                        why = self.bracket(p, va, ln)
                    break
        elif route in ("string", "float", "vtable"):
            why = "proto-name"
            for p in r["protos"]:
                ok = self.literal(route, n, p, va)  # 4.5 T16.2.c4: a str is its own reason
                why = None if ok is True else ok or route + "-differs"
                if why is None:
                    break
            if r["protos"] and route != self.kind_of(n, r["protos"][0]):
                why = why or "name-kind-vs-tag"
        elif route == "global":  # was: the record route is not re-derived (module doc); the name kind is
            if r["protos"] and self.kind_of(n, r["protos"][0]) != "data":
                why = "name-kind-vs-tag"
            elif r["protos"] and not any(self.data_equal(p, va) for p in r["protos"]):  # 4.5 T16.1 (G3)
                why = "data-differs"
        if why:
            r["fails"].append(f"route-{route}:{why}")

    def site_test(self, r):
        if not r.get("protos"):
            return
        n, va = r["name"], r["va"]
        agree = conf = rconf = fold = 0
        toks = set()  # 5.1 T3: the conflict as tokens, r:<retail dest> (fold sites too) and p:<proto dest>
        for p in r["protos"]:
            for rd, c in self.by_pd.get(p, {}).items():
                if rd == va:
                    agree += len(c)
                    continue
                toks.add(f"r:0x{rd:08X}")
                if len(self.pubs.get(p, ())) > 1 and not self.tab_va.get(rd):
                    fold += len(c)  # D119 fold site (4.2 T5): a fold dest's site cannot say which member it calls
                else:
                    conf += len(c)
        if not agree:  # agreeing sites still required: with none, the fold sites count
            conf += fold
        for pd, c in self.by_rd.get(va, {}).items():
            names = self.pubs.get(pd, set())
            if names and n not in names:
                rconf += len(c)
                toks.add(f"p:0x{pd:08X}")
        r["site"] = (agree, conf, rconf)
        if conf or rconf:
            acc = self.accepted.get((va, n))
            # was: if (va, n) in self.accepted and n.startswith(("??_C@", "__real@")):  (a waiver of any conflict)
            if acc and n.startswith(("??_C@", "__real@")) and acc[1] == toks:
                r["notes"].append("literal-copy-accepted")
            elif acc and n.startswith(("??_C@", "__real@")):  # 5.1 T3: the waiver names another conflict
                r["fails"].append(("site-conflict-both" if agree else "site-conflict-only") + ":accepted-conflict-differs")
            else:
                r["fails"].append("site-conflict-both" if agree else "site-conflict-only")

    def corr_route(self, r):
        """D113. +site-bundle: unique public P; sites into P's extent agree by offset from 2+ distinct callers, none
        conflicts, and no other proto dest reaches the retail extent. +slot-bytes: a vtable; every slot masked-equal,
        the same slot count on both sides, 1+ agreeing site."""
        n, va, ps = r["name"], r["va"], r["protos"]
        if len(ps) != 1:
            r["fails"].append(f"route-{r['cls'][1]}:public-not-unique")
            return
        p, img = ps[0], self.img
        if r["cls"][1] == "slotnames":  # 5.3 T2.c4 (M30, R212)
            why = self.slot_names(n, p, va)
            if why:
                r["fails"].append(f"route-slotnames:{why}")
            return
        if r["cls"][1] == "slots":
            slots = self.vt_slots(p, va) if n.startswith("??_7") else []
            why = None
            if not slots:
                why = "vtable-no-slots"
            elif not all(pv in self.procs and img.match(pv, rv, self.procs[pv], allow_const=False) is not None
                         for pv, rv in slots):
                why = "slot-bytes-differ"
            else:
                k = self.rt_slots(va)  # 4.5 T16.2.c4: the count moved to rt_slots (shared with literal), same rule
                if k != len(slots):
                    why = "slot-count-differs"
                elif not r.get("site") or r["site"][0] < 1:
                    why = "no-agreeing-site"
            if why:
                r["fails"].append(f"route-slots:{why}")
            return
        i = bisect.bisect_right(self.pub_vas, p)
        ext = r["size"] or min((self.pub_vas[i] - p) if i < len(self.pub_vas) else EXT_CAP, EXT_CAP)
        callers, conf = set(), 0
        for k in range(ext):
            for rd, c in self.by_pd.get(p + k, {}).items():
                if rd == va + k:
                    callers |= set(c)
                else:
                    conf += len(c)
            for pd, c in self.by_rd.get(va + k, {}).items():
                if not p <= pd < p + ext and n not in self.pubs.get(pd, set()):
                    conf += len(c)
        why = "site-conflict" if conf else "callers-below-2" if len(callers) < 2 else None
        if why:
            r["fails"].append(f"route-bundle:{why}")

    def row_fail_lines(self):
        out = []
        if not self.header or self.xbe_sha1 not in self.header[0]:
            out.append(f"FAIL pin-row header-sha1 {self.pin.name}:1: retail default.xbe sha1 {self.xbe_sha1} not in the header")
        live = {(r.get("va"), r.get("name")) for r in self.rows}
        used = {(r.get("va"), r.get("name")) for r in self.rows if "literal-copy-accepted" in r.get("notes", ())}
        for (va, n), (i, _toks) in sorted(self.accepted.items()):
            if (va, n) not in live or not n.startswith(("??_C@", "__real@")):
                out.append(f"FAIL pin-row accepted-stale 0x{va:08X} {mask(n)} {self.acc_path.name}:{i}: no such literal row")
            elif (va, n) not in used:  # 4.5 T16.2.c4 (G14); was: a live literal row's waiver passed, conflict or not
                out.append(f"FAIL pin-row accepted-stale 0x{va:08X} {mask(n)} {self.acc_path.name}:{i}: unused "
                           f"(no site conflict at the row to waive)")
        for va, n in sorted(set(NEAR_NO_SITE) - self.near_listed):  # 4.5 T16.1: renamed, moved or re-routed
            out.append(f"FAIL pin-row route-near 0x{va:08X} {mask(n)} pin_rows.NEAR_NO_SITE: "
                       f"route-near:stale-exception (no near row of that va and name)")
        for r in self.rows:
            for f in r["fails"]:
                va = f"0x{r['va']:08X}" if r.get("va") is not None else "-"
                out.append(f"FAIL pin-row {f.split(':')[0]} {va} {mask(r.get('name', ''))} {self.pin.name}:{r['line']}: "
                           f"{f} ({r['tag'] or '-'} {r['status'] or '-'})")
        return out

    # -- I10
    def i10(self, build, list_path=MATCHED):
        """Function text relocations of each matched function's built object: a name only the pin file holds must have
        an accepted status and a passing row; a name in neither table fails."""
        out, used, seen_none, objs = [], defaultdict(set), set(), {}
        rels = matched_set.build.obj_rels()  # 5.1 T4.c2: obj at its source path
        for e in matched_set.load(list_path):
            if e["kind"] != "function":
                continue
            # was: p = Path(build) / e["unit"].replace("/", "\\").rsplit("\\", 1)[-1]
            p = Path(build) / rels.get(e["unit"].replace("/", "\\").lower(),
                                       e["unit"].replace("/", "\\").rsplit("\\", 1)[-1])
            key = str(p).lower()
            if key not in objs:
                if not p.is_file():
                    out.append(f"FAIL pin-row i10-no-object {e['symbol']}: no {p.name} under {build}")
                    continue
                objs[key] = byte_compare.load_obj(p)[1]
            c = objs[key]
            cs = sorted((y for y in c["symbols"].values() if y["name"] == e["symbol"] and y["section"] > 0),
                        key=lambda y: not byte_compare.is_func(y))
            if not cs:
                out.append(f"FAIL pin-row i10-no-symbol {e['symbol']}: not defined in {p.name}")
                continue
            fsym = cs[0]
            sec, lo, hi = byte_compare.extent(c, fsym)
            secnames = {s["number"]: s["name"] for s in c["sections"]}
            for rl in sec["relocs"]:
                if not lo <= rl["offset"] < hi:
                    continue
                y = c["symbols"][rl["symbol"]]
                n = y["name"]
                # 4.5 T16.2.c4 (G14): skip a same-section static only where byte_compare.resolve falls back (in no
                # table, inside the function's own extent); was: `... or y["section"] == fsym["section"]` (every one)
                if y["class"] == coff_info.SC_STATIC and y["section"] > 0 and (
                        (y["value"] == 0 and n == secnames.get(y["section"])) or (
                            y["section"] == fsym["section"] and n not in self.sy_name and n not in self.by_row_name
                            and lo <= y["value"] < hi)):
                    continue
                if n in self.sy_name:
                    continue
                if n in self.by_row_name:
                    used[n].add(e["symbol"])
                elif not coff_info.DYN.search(n):
                    seen_none.add(n)
        for n in sorted(seen_none):
            out.append(f"FAIL pin-row i10-no-row - {mask(n)}: relocated through by a matched function, in neither table")
        for n in sorted(used):
            for r in self.by_row_name[n]:
                why = ("status " + (r["status"] or "-") if r["status"] not in ACCEPTED_STATUS
                       else "row fails" if r["fails"] else None)
                if why:
                    va = f"0x{r['va']:08X}" if r.get("va") is not None else "-"
                    out.append(f"FAIL pin-row i10 {va} {mask(n)} {self.pin.name}:{r['line']}: {why}; relocated through by "
                               f"{len(used[n])} matched function(s)")
        return out


_STATE = {}


def state(cfg=None):
    if cfg not in _STATE:
        _STATE[cfg] = State(cfg)
    return _STATE[cfg]


def check(cfg, build_dir, list_path=MATCHED):
    """Gate entry: -> [fail line] over every pin row and I10 (list_path's functions) against the objects in build_dir;
    raises Refuse."""
    s = state(cfg)
    return s.row_fail_lines() + s.i10(build_dir, list_path)


def row_ok(symbol, cfg=None):
    """D118: True when symbol has exactly one pin row and it passes, else the reason."""
    rs = state(cfg).by_row_name.get(symbol, [])
    if not rs:
        return "no-pin-row"
    if len(rs) > 1:
        return "pin-row-twice"
    return rs[0]["fails"][0] if rs[0]["fails"] else True


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--build", required=True, help="dir of the built objects (build.py --out)")
    ap.add_argument("--config", default=None)
    a = ap.parse_args()
    t0 = time.perf_counter()
    try:
        fails = check(a.config, a.build)
    except (Refuse, matched_set.Refuse, byte_compare.Refuse) as e:
        print(f"REFUSED: {e}")
        return 2
    for ln in fails:
        print(ln)
    s = state(a.config)
    by = Counter(ln.split()[2] for ln in fails)
    print(f"pin-row: {len(s.rows)} rows, {sum(1 for r in s.rows if not r['fails'])} pass; fails {len(fails)}"
          f"{' by class ' + ', '.join(f'{k} {v}' for k, v in sorted(by.items())) if by else ''}; "
          f"{time.perf_counter() - t0:.2f} s")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
