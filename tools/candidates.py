"""First-match candidate filter + idiom tags (T2, phase 1.6).

CLI: candidates.py [--write | --check | --control member] [--config P]
  (no flag) compute and print counts; --write also writes docs/matching/next_candidates.md,
  docs/matching/delink_residue.md (3.2 T8: undelinked game units, reason, evidence searched, `unit not delink` rows)
  and derived/candidates/candidates.jsonl and prints fail counts + a Pins block; --check renders fresh and compares
  with both committed docs (first differing line number printed).
  Stamp (5.5 T6): --write writes candidates.jsonl first (temp + os.replace), then the sidecar STAMP
  (derived/candidates/candidates.stamp.json: sha256 of each input compute() opens and of candidates.jsonl), then the
  docs. fresh() -> stale causes ([] when current); --check exits 1 on any. Any EXPECT* count mismatch exits 1
  before --check or --write (the tuples below are the expectations; a re-cut updates them, old value in a comment).
Pool (1.4 rule, docs/evidence/compiler_pin.md:8-17): derived/proto_map/retail-xbe/functions.jsonl class identical, one
  targetVA, unit (config/retail/units.toml text_start <= va < text_end) kind game + delink true, not tail (protoVA -
  0x400400 in 0x13BD4E..0x13F2F4), VA not a derived/dup_report/retail_folds.jsonl retailVA, a symbols.txt `func` at VA,
  whose name is not a cut boundary `boundary_<8 hex>` (non-callable, 3.1 T7; EXCL `boundary`).
Checks per row (row passes iff all pass): sig, reloc, c0035, bodymax, library (docs/ops/tools.md `## candidates`).
  reloc: an abs site name_sites.Images.match calls a constant (T6 rule, not copied) is `resolved: "constant"`, never
  unresolved or provisional.
  Member rule (3.4 T16): a size `-` data row g named N whose PDB public N is one proto VA P with S_GDATA32/S_LDATA32
  records at P named N of one pdb_types.tsize S (name_sites.rec_size, route step b) spans [g, min(g + S, next pinned
  VA > g, func or data, either table)); a target inside resolves to that row's status (accepted() decides)
  and its site carries `member: "<name>+0x<off>"`. Otherwise the row stays 1 byte; sized rows unchanged.
  --control member: positive 0x235794 -> ?beepState@@3VBeepState@@A+0x1C; planted g+S past a typed global and g+1
  of an untyped sizeless global -> unresolved. Writes nothing; exit 0 iff all hold, else 1.
Tags y/-/? : float, switch, bittest, inlined (always ?), vcall, eh; iced-x86 decode as proto_map.mask_proc.
done y/- : (symbol, va) is a config/matched_set.toml [[function]] row (T7).
Near lane (3.4 T2, C0142): functions.jsonl class near-identical with candidateVA and similarity >= proto_map.THRESHOLD,
  va = candidateVA; NEAR_EXCL in order (unit as above, tail, fold, no lane name = no table func at va, size `-`, or
  status not near-corr / PIN_STATUSES); size = that func line's table size. Same CHECKS and TAGS, no identity (no
  Images.match, so no `constant` sites). Site acceptance (4.2 T9, D115): accepted() = self, constant, STATUSES,
  PIN_STATUSES, or CALLEE_STATUSES (near-corr) when the target is a func-table VA; a near-corr data target stays
  provisional. Every jsonl row carries `lane` (identical | near), ranks count per lane; near rows add similarity,
  protoLength. The doc's identical sections hold identical rows only; `## Near lane (3.4 T2)` follows them.
Ambiguous lane (5.3 T3, amb_lane): class ambiguous rows whose name is pinned (either table) at one of their own hits
  (C0366 past HIT_CAP); AMB_EXCL in order; lane `ambiguous`, jsonl `amb_class` = the pin's tag class; same CHECKS and
  TAGS; `## Ambiguous lane (5.3 T3)` is the last section. --control amb-outside|amb-site-corr|amb-shared plant a pin
  outside the hits, a site-corr pin, one body pinned by two rows; exit 0 iff each is refused, else DID NOT FAIL and 1.
Doc holds names, VAs, sizes, units, tags, verdicts and reasons only, no bytes or disassembly (D46); no dates.
Exit: 0 ok; 1 --check differs or stale stamp, or an EXPECT* count differs; 2 refusal (missing input, retail sha1 != pin).
"""
import argparse
import bisect
import hashlib
import json
import os
import re
import struct
import sys
import tomllib
from pathlib import Path

from iced_x86 import Code, Decoder, Mnemonic, OpKind, Register

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

import byte_compare  # noqa: E402
import coff_info  # noqa: E402  pin file (5.5 T6 stamp)
import delink  # noqa: E402
import match_one  # noqa: E402  size BUCKETS (near lane, 3.4 T2)
import pdb_info  # noqa: E402
import name_sites  # noqa: E402
import pe_info  # noqa: E402
import proto_map  # noqa: E402
import units_seed  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

FUNCS = REPO / "derived" / "proto_map" / "retail-xbe" / "functions.jsonl"
FOLDS = REPO / "derived" / "dup_report" / "retail_folds.jsonl"
UNITS = REPO / "config" / "retail" / "units.toml"
SYMS = REPO / "config" / "retail" / "symbols.txt"
SYMS_PIN = REPO / "config" / "retail" / "symbols_pin.txt"  # 2.1 T4: read after SYMS, as gate.py SYMTABS
PIN_STATUSES = ("disasm-verified", "corr-verified")  # was ("disasm-verified",); 4.2 T5 adds D113's corr-verified
# 2.1 T4: symbols_pin.txt status, evidence per line in docs/evidence/compiler_pin.md
PINS = REPO / "config" / "compiler_pin.toml"
MATCHED = REPO / "config" / "matched_set.toml"  # [[function]] rows: gate-verified matches (D40)
DOC = REPO / "docs" / "matching" / "next_candidates.md"
OUT = REPO / "derived" / "candidates" / "candidates.jsonl"
STAMP = OUT.with_name("candidates.stamp.json")  # 5.5 T6: sha256 of compute()'s inputs and of OUT, written by --write
CHECKS = ("sig", "reloc", "c0035", "bodymax", "library")
TAGS = ("float", "switch", "bittest", "inlined", "vcall", "eh")
EXCL = ("not identical", "targetVAs != 1", "no unit", "unit not game", "unit not delink", "tail", "fold", "no symbol", "boundary")
RESIDUE = REPO / "docs" / "matching" / "delink_residue.md"  # 3.2 T8: undelinked game units, written with DOC
EXPECT = dict(zip(EXCL, (5254, 0, 73, 1436, 758, 0, 24, 64, 4)))  # 5.5 T26.c1 re-cut over the T7.c7 pinpending pins (1 row out of no symbol)
# was (5254, 0, 73, 1436, 758, 0, 24, 65, 4) 5.5 T7.c6 re-cut over T7.c2-c5 pins (2 rows out of no symbol)
# was (5254, 0, 73, 1436, 758, 0, 24, 67, 4) 5.4 T11.1.c3 re-cut 1 (Monster#448 cut: 34 rows out of not delink, 1 to no symbol)
# was (5254, 0, 73, 1436, 792, 0, 24, 66, 4) 5.3 T13.2 re-cut 4 (28 rows out of no symbol)
# was (5254, 0, 73, 1436, 792, 0, 24, 94, 4) 5.3 T4 re-cut over T4.c1 pins (7 rows out of no symbol)
# was (5254, 0, 73, 1436, 792, 0, 24, 101, 4) 5.2 T6 pin pass (25 rows out of no symbol, 1 out of boundary)
# was (5254, 0, 73, 1436, 792, 0, 24, 126, 5) 5.2 T5 B2 cut CarriedObject+HeavyObject+VinceCutScene+Boomerang (9 rows out of not delink)
# was (5254, 0, 73, 1436, 801, 0, 24, 126, 5) 5.2 T4 B1 cut FanBoat+BogWallow (57 rows out of not delink, 6 to no symbol)
# was (5254, 0, 73, 1436, 858, 0, 24, 120, 5) 5.2 T3 LevelLoader+BasicDoor cut (97 rows out of not delink, 1 to no symbol)
# was (5254, 0, 73, 1436, 955, 0, 24, 119, 5) 4.4 T8 B5 cut (HannaCutscenes, DoorPulleyBucket, Spring, bakeryMain, WallSwitch, MainTunnelCutscenes, PowerIcon, VinceWalkIn, Terradactyl: 30 rows out of not delink, 2 to no symbol, 2 to fold)
# was (5254, 0, 73, 1436, 985, 0, 22, 117, 5) 4.4 T8 B4 cut (theSquare, GasPumps, MoleGatePuzzle, Bell: 116 rows out of not delink, 4 to no symbol, 1 to fold); was (5254, 0, 73, 1436, 1101, 0, 21, 113, 5) 4.4 T7.1 B3 cut (HouseOfMirrors, SwampDebugGeometry, SwampBSP, FontManager: 57 rows out of not delink, 4 to no symbol); was (5254, 0, 73, 1436, 1158, 0, 21, 109, 5) 4.4 T7.1 B2 cut (ZombieGuidanceCounselor, VoodooShop, FFlyController, SwampCollide, SwampSoundResource: 131 rows out of not delink, 13 to no symbol); was (5254, 0, 73, 1436, 1289, 0, 21, 96, 5) 4.4 T7.1 B1 cut (SwampWorld, SwampLight, Whammy, Critter, SwampFireFlyNode: 246 rows out of not delink, 18 to no symbol); was (5254, 0, 73, 1436, 1535, 0, 21, 78, 5) 4.4 T6.1; was (5254, 0, 73, 1436, 1576, 0, 18, 77, 5) 4.4 T4; was (5254, 0, 73, 1436, 1576, 0, 18, 81, 5) 4.3 T15; was (5254, 0, 73, 1436, 1576, 0, 18, 85, 2) 3.4 T1; was (5254, 0, 73, 1436, 1576, 0, 18, 120, 2) 3.3 T1; was (5254, 0, 73, 1436, 1724, 0, 18, 114, 2) 3.2 T8.c1; was (5254, 0, 73, 1436, 2392, 0, 0, 60, 2) 3.2 T3;
# was 2417 not delink, 1.4
EXPECT_POOL = 2775  # 5.5 T26.c1 re-cut over the T7.c7 pinpending pins; was 2774 (5.5 T7.c6 re-cut over T7.c2-c5 pins); was 2772 (5.4 T11.1.c3 re-cut 1); was 2739 (5.3 T13.2 re-cut 4); was 2711 (5.3 T4 re-cut over T4.c1 pins); was 2704 (5.2 T6 pin pass); was 2678 (5.2 T5 B2 cut CarriedObject+HeavyObject+VinceCutScene+Boomerang; was 2669 (5.2 T4 B1 cut FanBoat+BogWallow); was 2618 (5.2 T3 LevelLoader+BasicDoor cut); was 2522 (4.4 T8 B5 cut); was 2496 (4.4 T8 B4 cut); was 2385 (4.4 T7.1 B3 cut); was 2332 (4.4 T7.1 B2 cut); was 2214 (4.4 T7.1 B1 cut); was 1986 (4.4 T6.1 ScareHouse cut + its name_sites pins); was 1949 (4.4 T4 vtable slot pins); was 1945 (4.3 T15, 0x000388BB hand pin); was 1944 (3.4 T1); was 1909 (3.3 T1); was 1767 (3.2 T8.c1); was 1171 (3.2 T3); was 1148 (1.4, compiler_pin.md:31-32)
NEAR_EXCL = ("no unit", "unit not game", "unit not delink", "tail", "fold", "no lane name")  # 3.4 T2 near lane
NEAR_STATUSES = ("near-corr",) + PIN_STATUSES  # lane name statuses
CALLEE_STATUSES = ("near-corr",)  # 4.2 T9 (D115): accepted for a func-table target only, never data or vtable
# was: near-corr never joins the accepted ones (3.4 T2, D104)


def accepted(status, target, funcs):
    """4.2 T9 (D115): a site's resolved status is accepted: self or constant, units_seed.STATUSES, PIN_STATUSES, or
    CALLEE_STATUSES when target is a func-table VA (funcs = load_symbols()[0]). candidates.py and family_census.py."""
    return (status in ("self", "constant") or status in units_seed.STATUSES + PIN_STATUSES
            or status in CALLEE_STATUSES and target in funcs)


EXPECT_NEAR = dict(zip(NEAR_EXCL, (142, 0, 331, 0, 9, 443)))  # 5.5 T27.c2 re-cut over the sized registerme-twin pins; was (142, 0, 331, 0, 9, 598) 5.4 T11.1.c3 re-cut 1; was (143, 0, 334, 0, 9, 598) 5.3 T13.2 re-cut 4; was (143, 0, 334, 0, 9, 599) 5.2 T5 B2 cut CarriedObject+HeavyObject+VinceCutScene+Boomerang; was (148, 0, 339, 0, 9, 593) 5.2 T4 B1 cut FanBoat+BogWallow; was (150, 0, 375, 0, 9, 560) 5.2 T3 LevelLoader+BasicDoor cut; was (152, 0, 388, 0, 9, 549) 4.4 T8 after the T7.1+T8 cuts (27 units); was (167, 0, 598, 0, 6, 380) 4.4 T6.1 ScareHouse cut; was (167, 0, 615, 0, 5, 366) 3.4 T2 (1260 near rows, 238 near-corr pins); was 456 at 96 pins
AMB_EXCL = ("no unit", "unit not game", "unit not delink", "tail", "fold", "boundary", "no pin", "pin outside targets",
            "pin not accepted", "shared body")  # 5.3 T3 ambiguous lane
EXPECT_AMB = dict(zip(AMB_EXCL, (316, 136, 1172, 0, 0, 2, 450, 1, 99, 0)))  # 5.5 T27 retag 66 ctor pins site-corr -> +callee-bytes (66 out of pin not accepted, into the pool); was (316, 136, 1172, 0, 0, 2, 450, 1, 165, 0) 5.5 T27.c4 re-cut t27c over the c4 ctor pins (60 rows out of no pin: 63 to pin not accepted (site-corr), 1 to pin outside targets (??0BumperBlockMain, 30 hits), 4 out of no unit/not delink); was (317, 136, 1175, 0, 0, 2, 510, 0, 102, 0) 5.5 T26.c1 re-cut over the T7.c7 pinpending pins; was (317, 136, 1175, 0, 0, 2, 521, 0, 92, 0) 5.5 T7.c6 re-cut over T7.c2-c5 pins; was (311, 136, 1171, 0, 0, 2, 540, 0, 92, 0) 5.4 T16 after T25/T26 pins; was (311, 136, 1171, 0, 0, 2, 541, 0, 92, 0) 5.4 T11.1.c3 re-cut 1 (T11.1 pins)
# was (143, 136, 1058, 0, 0, 2, 1275, 0, 105, 0) 5.3 T13.2 re-cut 4
# was (134, 134, 1050, 0, 0, 2, 1673, 0, 11, 0) 5.3 T13.2 fold = true folds only; was (134, 134, 1050, 0, 2120, 0, 13, 0, 0, 0) 5.3 T4.c3 re-cut, _Buynode fold pair disabled; was (134, 134, 1049, 0, 2121, 0, 13, 0, 0, 0) 5.3 T4 re-cut over T4.c1 pins; was (137, 134, 1058, 0, 2124, 0, 19, 0, 0, 0) 5.3 T3: new
EXPECT_AMB_POOL = 1467  # 5.5 T27 retag 66 ctor pins; was 1401 5.5 T26.c1 re-cut over the T7.c7 pinpending pins; was 1400 (5.5 T7.c6 re-cut over T7.c2-c5 pins); was 1391 (5.4 T16 after T25/T26 pins); was 1390 5.4 T11.1.c3 re-cut 1 (T11.1 pins); was 924 (5.3 T13.2 re-cut 4); was 639 (5.3 T13.2 fold = true folds only); was 192 5.3 T4.c3 re-cut, 5.3 T4 re-cut over T4.c1 pins; was 171 (5.3 T3: new)
EXPECT_NEAR_POOL = 335  # 5.5 T27.c2 re-cut over the sized registerme-twin pins; was 180 (5.4 T11.1.c3 re-cut 1); was 176 (5.3 T13.2 re-cut 4); was 175 (5.2 T5 B2 cut CarriedObject+HeavyObject+VinceCutScene+Boomerang; was 171 (5.2 T4 B1 cut FanBoat+BogWallow); was 166 (5.2 T3 LevelLoader+BasicDoor cut); was 162 (4.4 T8 after the T7.1+T8 cuts); was 109 4.4 T6.1; was 107 3.4 T2; was 17 at 96 pins
PROTO_BASE, TAIL = 0x400400, (0x13BD4E, 0x13F2F4)
# LF_PROCEDURE / LF_MFUNCTION, 16-bit-index (0x0008/0x0009) and 32-bit-index (0x1008/0x1009) forms (CodeView cvinfo.h)
PROC_LEAVES = {0x0008: "LF_PROCEDURE_16t", 0x0009: "LF_MFUNCTION_16t", 0x1008: "LF_PROCEDURE", 0x1009: "LF_MFUNCTION"}
XDK_MODS = ("obj\\i386\\", "d:\\xboxfre", "* linker *")  # docs/ops/tools.md:259-263, compared lowercase
PREFIXES = {0x26, 0x2E, 0x36, 0x3E, 0x64, 0x65, 0x66, 0x67, 0xF0, 0xF2, 0xF3}
CXX = "___CxxFrameHandler"
BOUNDARY = re.compile(r"^boundary_[0-9A-F]{8}$")  # 3.1 T7 cut-boundary pin, not callable


class Refuse(Exception):
    pass


def load_jsonl(p):
    return [json.loads(ln) for ln in p.read_text(encoding="utf-8").splitlines() if ln.strip()]


def need(p):
    if not p.is_file() or p.stat().st_size == 0:
        raise Refuse(f"input missing or empty: {p}")
    return p


def stamp_inputs(config=None):
    """5.5 T6: [(key, path)] of every file compute() opens; the XBE by its pin file (setup() checks its sha1)."""
    pin = coff_info.pinned_sha() or ""
    t5 = REPO / "derived" / "ghidra" / f"retail-{pin[:8]}"
    t5 = t5 / "applied" if (t5 / "applied" / "summary.json").is_file() else t5  # byte_compare.setup's tables
    cfg = vv_inputs.load_config(config)
    ps = [coff_info.PIN, t5 / "functions.jsonl", t5 / "symbols.jsonl",
          REPO / "derived" / "ghidra" / f"retail-{pin[:8]}" / "applied" / "functions.jsonl", FUNCS, FOLDS, UNITS,
          SYMS, SYMS_PIN, PINS, MATCHED, delink.cut_dir(pin) / "hashes.txt"]
    out = [(p.relative_to(REPO).as_posix(), p) for p in dict.fromkeys(ps)]
    return out + [(f"prototype_dir/{n}", vv_inputs.input_path("prototype_dir", n, cfg)) for n in ("default.pdb",
                                                                                                  "default.exe")]


def _sha256(p):
    return hashlib.sha256(p.read_bytes()).hexdigest() if p.is_file() else None


def _replace(path, text):  # 5.5 T6: temp + os.replace, a reader never sees a half-written file
    tmp = path.with_name(path.name + ".tmp")
    tmp.write_text(text, encoding="utf-8", newline="\n")
    os.replace(tmp, path)


def fresh(config=None):
    """5.5 T6: -> stale causes ([] = candidates.jsonl is current): STAMP absent, candidates.jsonl changed since --write,
    or an input's sha256 != its stamp value."""
    if not STAMP.is_file():
        return [f"{STAMP.relative_to(REPO).as_posix()} absent (run PY tools/candidates.py --write)"]
    st = json.loads(STAMP.read_text(encoding="utf-8"))
    why = [] if _sha256(OUT) == st.get("out") else [f"{OUT.relative_to(REPO).as_posix()} changed since --write"]
    cur = {k: _sha256(p) for k, p in stamp_inputs(config)}
    old = st.get("inputs", {})
    return why + [f"input {k} changed since --write" for k in sorted(set(cur) | set(old)) if cur.get(k) != old.get(k)]


def load_symbols():
    funcs, data = {}, []
    for tab, ln in ((tab, ln) for tab in (SYMS, SYMS_PIN) for ln in need(tab).read_text(encoding="utf-8").splitlines()):
        if not ln.strip() or ln.startswith("#"):
            continue
        f = ln.split()
        va, kind, size, status, source, name = int(f[0], 16), f[1], f[2], f[-1], f[-2], " ".join(f[3:-2])
        if kind == "func":
            funcs[va] = {"name": name, "source": source, "status": status,
                         "size": int(size, 16) if size != "-" else None,  # size: near lane (3.4 T2)
                         "pin": tab is SYMS_PIN}  # 5.3 T3: amb_class symbols when False
        else:
            data.append((va, va + (int(size, 16) if size != "-" else 1), status, name, size != "-"))  # sized flag 3.4 T16
    return funcs, sorted(data)


def member_extents(funcs, data, pdb_b, exe, img, cfg):
    """3.4 T16 member rule: each size `-` data row (g, named N) whose PDB public N is one proto VA P with a unique
    name_sites.rec_size S > 1 gets end min(g + S, next pinned VA > g, func or data, either table); 5th field = S.
    Every other row keeps its end, 5th field None (sized rows unchanged; sizeless stay 1 byte)."""
    pe = pe_info.parse_pe(exe)
    byname = {}
    for p in pdb_info.parse_pdb(pdb_b)["publics"]:  # as name_sites pubs: segment-relative -> proto VA
        if 1 <= p["seg"] <= len(pe["sections"]):
            byname.setdefault(p["name"], set()).add(img.base + pe["sections"][p["seg"] - 1]["va"] + p["off"])
    drecs = name_sites.data_records(img, cfg)
    pinned = sorted(set(funcs) | {d[0] for d in data})
    out = []
    for g, end, status, name, sized in data:
        ps = byname.get(name, ()) if not sized else ()
        S = name_sites.rec_size(drecs, next(iter(ps)), name) if len(ps) == 1 else None
        if isinstance(S, int) and S > 1:
            j = bisect.bisect_right(pinned, g)
            out.append((g, min(g + S, pinned[j]) if j < len(pinned) else g + S, status, name, S))
        else:
            out.append((g, end, status, name, None))
    return out


def resolve_target(t, va, size, funcs, dstarts, data):
    """-> (status | "self" | None, member "<name>+0x<off>" | None): own extent, a table func VA, else the data row
    [g, end) holding t (data as member_extents); member set when t > g inside a member extent (3.4 T16)."""
    if va <= t < va + size:
        return "self", None
    if t in funcs:
        return funcs[t]["status"], None
    i = bisect.bisect_right(dstarts, t) - 1
    if i >= 0 and data[i][0] <= t < data[i][1]:
        return data[i][2], (f"{data[i][3]}+0x{t - data[i][0]:X}" if data[i][4] and t > data[i][0] else None)
    return None, None


def proto_publics(pdb_b, exe):
    """5.3 T3: proto VA -> {PDB public names} (as member_extents: segment-relative -> proto VA)."""
    pe, out = pe_info.parse_pe(exe), {}
    for p in pdb_info.parse_pdb(pdb_b)["publics"]:
        if 1 <= p["seg"] <= len(pe["sections"]):
            out.setdefault(pe["imageBase"] + pe["sections"][p["seg"] - 1]["va"] + p["off"], set()).add(p["name"])
    return out


def amb_class_of(f):
    """5.3 T3: a pin's class from its source tag middle field (tools/pin_rows.py TAGS); symbols.txt -> symbols."""
    if not f["pin"]:
        return "symbols"
    p = f["source"].split("+")
    mid = p[1] if len(p) > 1 else p[0]
    if mid == "site-corr" and p[2:] in (["slot-bytes"], ["site-bundle"], ["slot-names"]):
        return "D113"
    if mid == "site-corr" and p[2:] == ["callee-bytes"]:
        return "caller-xref"
    return {"unit-bracket": "bracket", "anchor-window": "D115"}.get(mid, mid)


def func_names():
    """5.3 T3: name -> {VA} over every `func` line of both tables (load_symbols keeps one name per VA; a fold's alias
    pins, C0130, are one line per public)."""
    out = {}
    for ln in (ln for tab in (SYMS, SYMS_PIN) for ln in need(tab).read_text(encoding="utf-8").splitlines()):
        f = ln.split()
        if ln.strip() and not ln.startswith("#") and f[1] == "func":
            out.setdefault(" ".join(f[3:-2]), set()).add(int(f[0], 16))
    return out


def amb_lane(rows, funcs, byname, units, ustarts, folds, pub, ivas, confirm):
    """5.3 T3 ambiguous lane -> (excl counts, pool [(size, va, row, unit)], {va: amb_class}, {protoVA: AMB_EXCL key}).
    Row class ambiguous; pins = func VAs (byname = func_names()) named by a PDB public at the row's protoVA or a row
    name; status and class from funcs[va] (load_symbols, the table read order); own =
    pins among targetVAs, or (len(targetVAs) == proto_map.HIT_CAP) pins where confirm(row, va) (C0366). va = own[0],
    else pins[0], else targetVAs[0]. AMB_EXCL in order; shared body (a va claimed twice, or an identical pool va in
    ivas) excludes every ambiguous claimant. fold (5.3 T13.2) = a true fold only: folds = {retail_folds retailVA:
    protoVAs of kind identical-unique}; the row claims va by its pin, so va is a fold when another protoVA's sole
    target is va (two distinct claiming rows; C0130 alias pins). A va listed only as ambiguous-hit/near candidate
    is no fold. Was: any retail_folds retailVA (5.3 T3)."""
    excl, cand, why = dict.fromkeys(AMB_EXCL, 0), [], {}
    for r in rows:
        if r["class"] != "ambiguous":
            continue
        tv = [int(v, 16) for v in r["targetVAs"]]
        pins = sorted({v for n in pub.get(int(r["protoVA"], 16), set()) | set(r["names"]) for v in byname.get(n, ())})
        own = [v for v in pins if v in tv]
        if not own and len(tv) == proto_map.HIT_CAP:
            own = [v for v in pins if confirm(r, v)]
        va = own[0] if own else pins[0] if pins else tv[0]
        i = bisect.bisect_right(ustarts, va) - 1
        u = units[i] if i >= 0 and units[i]["text_start"] <= va < units[i]["text_end"] else None
        pv = int(r["protoVA"], 16) - PROTO_BASE
        f = funcs.get(va)
        for k, bad in zip(AMB_EXCL[:-1], (lambda: u is None, lambda: u["kind"] != "game", lambda: not u["delink"],
                                          lambda: TAIL[0] <= pv < TAIL[1],
                                          lambda: bool(folds.get(va, frozenset()) - {r["protoVA"]}),
                                          lambda: f is not None and BOUNDARY.match(f["name"]), lambda: not pins,
                                          lambda: not own,
                                          lambda: f["status"] == "site-corr" or not accepted(f["status"], va, funcs))):
            if bad():
                excl[k] += 1
                why[r["protoVA"]] = k
                break
        else:
            cand.append((r["length"], va, r, u))
    claims = {}
    for c in cand:
        claims[c[1]] = claims.get(c[1], 0) + 1
    pool = []
    for c in cand:
        if claims[c[1]] > 1 or c[1] in ivas:
            excl["shared body"] += 1
            why[c[2]["protoVA"]] = "shared body"
        else:
            pool.append(c)
    pool.sort(key=lambda p: (p[0], p[1]))
    return excl, pool, {p[1]: amb_class_of(funcs[p[1]]) for p in pool}, why


def pdb_procs(pdb_b, exe_b):
    """protoVA -> [(name, typind)] from every module's S_*PROC32*; TPI stream 2 leaf by type index."""
    st = pdb_info.read_streams(pdb_b)["streams"]
    pe = pe_info.parse_pe(exe_b)
    base, secs = pe["imageBase"], pe["sections"]
    dbi = st[3]
    modi_sz = struct.unpack_from("<i", dbi, 24)[0]
    procs, o, end = {}, 64, 64 + modi_sz
    while o + 64 <= end:
        sn, symsz = struct.unpack_from("<HI", dbi, o + 34)
        name = pdb_info._cstr(dbi, o + 64)
        o2 = o + 64 + len(name.encode()) + 1
        obj = pdb_info._cstr(dbi, o2)
        o = (o2 + len(obj.encode()) + 1 + 3) & ~3
        if sn == 0xFFFF or sn >= len(st) or not st[sn]:
            continue
        for kind, p in pdb_info.iter_records(st[sn][4:symsz]):
            if kind not in pdb_info.PROCS:
                continue
            ti, off, seg = struct.unpack_from("<I", p, 24)[0], struct.unpack_from("<I", p, 28)[0], \
                struct.unpack_from("<H", p, 32)[0]
            if 1 <= seg <= len(secs):
                procs.setdefault(base + secs[seg - 1]["va"] + off, []).append(
                    (pdb_info._cstr(p, 35, pdb_info.PROCS[kind][1]), ti))
    tpi = st[2]
    ver, hdr, timin, timax = struct.unpack_from("<4I", tpi, 0)
    leaves, o, ti = {}, hdr, timin
    while o + 4 <= len(tpi) and ti < timax:
        ln, leaf = struct.unpack_from("<HH", tpi, o)
        leaves[ti] = leaf
        o += 2 + ln
        ti += 1
    return procs, leaves, ver


def decode(buf, va):
    d = Decoder(32, buf, ip=va)
    out, err = [], 0
    while d.can_decode:
        ins = d.decode()
        if ins.code == Code.INVALID:
            err += 1
        out.append(ins)
    return out, err


def fs0(ins):
    return any(ins.op_kind(i) == OpKind.MEMORY for i in range(ins.op_count)) and ins.memory_segment == Register.FS \
        and ins.memory_base == Register.NONE and ins.memory_index == Register.NONE and ins.memory_displacement == 0


class Img:
    def __init__(self, xb, x):
        self.xb, self.x, self.memo = xb, x, {}

    def head(self, va, n):
        if va not in self.memo:
            off = xbe_info.va_to_off(self.x, va)
            self.memo[va] = decode(self.xb[off:off + 64], va)[0] if off is not None else []
        return self.memo[va][:n]


def tags(buf, va, img, cxx):
    insns, err = decode(buf, va)
    t = dict.fromkeys(TAGS, False)
    for k, ins in enumerate(insns):
        off = ins.ip - va
        b = next((c for c in buf[off:off + ins.len] if c not in PREFIXES), None)
        if b is not None and 0xD8 <= b <= 0xDF:
            t["float"] = True
        mem0 = ins.op_count and ins.op0_kind == OpKind.MEMORY
        if ins.mnemonic == Mnemonic.JMP and mem0 and ins.memory_base == Register.NONE \
                and ins.memory_index != Register.NONE and ins.memory_index_scale == 4 and ins.memory_displ_size == 4:
            t["switch"] = True
        if ins.mnemonic in (Mnemonic.BT, Mnemonic.BTS, Mnemonic.BTR, Mnemonic.BTC):
            t["bittest"] = True
        if ins.mnemonic == Mnemonic.TEST and ins.op_count == 2 and ins.op1_kind in proto_map._IMM_KINDS:
            v = ins.immediate(1) & 0xFFFFFFFF
            t["bittest"] |= v != 0 and v & (v - 1) == 0
        if ins.mnemonic == Mnemonic.CALL and mem0 and ins.memory_base != Register.NONE:
            t["vcall"] = True
        if fs0(ins):
            t["eh"] = True
        if ins.is_call_near and ins.op0_kind == OpKind.NEAR_BRANCH32 \
                and any(fs0(i) for i in img.head(ins.near_branch_target, 4)):
            t["eh"] = True
        h = None
        if ins.mnemonic == Mnemonic.PUSH and ins.op0_kind == OpKind.IMMEDIATE32:
            h = ins.immediate32
        elif ins.mnemonic == Mnemonic.MOV and ins.op0_kind == OpKind.REGISTER and ins.op0_register == Register.EAX \
                and ins.op1_kind == OpKind.IMMEDIATE32:
            h = ins.immediate32
        if h is not None and cxx is not None and (h == cxx or any(
                i.mnemonic == Mnemonic.JMP and i.op0_kind == OpKind.NEAR_BRANCH32 and i.near_branch_target == cxx
                for i in img.head(h, 3))):
            t["eh"] = True
    out = {k: ("y" if v else ("?" if err else "-")) for k, v in t.items()}
    out["inlined"] = "?"
    return out, err


def compute():
    pin, _byname, _t, _xp, xb, x = byte_compare.setup(argparse.Namespace(symbols=None, xbe=None, config=ARGS.config))
    why = delink.cut_check(delink.cut_dir(pin))  # T1 (2.1): valid cut.complete before any cut/applied read
    if why:
        raise Refuse(why)
    fj = need(REPO / "derived" / "ghidra" / f"retail-{pin[:8]}" / "applied" / "functions.jsonl")
    cfg = vv_inputs.load_config(ARGS.config)
    pdb_p = vv_inputs.input_path("prototype_dir", "default.pdb", cfg)
    exe_p = vv_inputs.input_path("prototype_dir", "default.exe", cfg)
    rows = load_jsonl(need(FUNCS))
    frecs = load_jsonl(need(FOLDS))
    folds = {int(r["retailVA"], 16) for r in frecs}
    afolds = {int(r["retailVA"], 16): frozenset(p["protoVA"] for p in r["protos"] if p["kind"] == "identical-unique")
              for r in frecs}  # 5.3 T13.2: amb lane, va -> protoVAs whose sole target it is (dup_report.py:228-231)
    units = sorted((u for u in tomllib.loads(need(UNITS).read_text(encoding="utf-8"))["unit"] if "text_start" in u),
                   key=lambda u: u["text_start"])  # units without a .text range hold no VA
    ustarts = [u["text_start"] for u in units]
    funcs, data = load_symbols()
    pins = tomllib.loads(need(PINS).read_text(encoding="utf-8"))["function"]
    bodymax = {int(f["entry"], 16): int(f["bodyMax"], 16) for f in load_jsonl(fj)}
    try:
        exe = need(exe_p).read_bytes()
        procs, leaves, tpiver = pdb_procs(need(pdb_p).read_bytes(), exe)
        nimg = name_sites.Images(exe, pe_info.parse_pe(exe), xb, x)  # T3 (3.2): constant abs sites
        data = member_extents(funcs, data, need(pdb_p).read_bytes(), exe, nimg, ARGS.config)  # 3.4 T16
        pub = proto_publics(need(pdb_p).read_bytes(), exe)  # 5.3 T3 ambiguous lane names
    except (pdb_info.NotPdb, pe_info.NotPe, struct.error, IndexError, TypeError) as e:
        raise Refuse(f"default.pdb/default.exe unparseable: {e}")
    dstarts = [d[0] for d in data]
    cxx = next((va for va, f in funcs.items() if f["name"] == CXX), None)
    lo_img, hi_img = x["header"]["baseAddr"], x["header"]["baseAddr"] + x["header"]["sizeOfImage"]
    img = Img(xb, x)
    # T5 (2.1): c0035 reads the cut objects; each verified against its hashes.txt line, parsed once per unit
    cut = delink.cut_dir(pin)
    hsum = dict(reversed(ln.split()) for ln in (cut / "hashes.txt").read_text(encoding="ascii").splitlines()
                if ln.strip())
    srows = delink._func_rows()
    vas_of = {}
    for v, _s, n in srows:
        vas_of.setdefault(n, set()).add(v)
    roffs_of = {}

    def unit_roffs(u):
        if u["name"] in roffs_of:
            return roffs_of[u["name"]]
        f = delink._unit_file(u["name"])
        if f not in hsum:
            raise Refuse(f"{f} not listed in {cut / 'hashes.txt'}")
        b, c = byte_compare.load_obj(cut / f)
        if hashlib.sha256(b).hexdigest() != hsum[f]:
            raise Refuse(f"{f}: sha256 != its {cut / 'hashes.txt'} line (cut changed after --fresh)")
        texts = [sc for sc in c["sections"] if sc["name"] == ".text"]
        if len(texts) != 1:
            raise Refuse(f"{f}: {len(texts)} .text sections")
        sec = texts[0]
        for y in c["symbols"].values():  # as delink relocscan: object offset == va - text_start
            v = vas_of.get(y["name"], set())
            if byte_compare.is_func(y) and y["section"] == sec["number"] and len(v) == 1                     and y["value"] != next(iter(v)) - u["text_start"]:
                raise Refuse(f"{f}: {y['name']} at +0x{y['value']:X}, symbols.txt says "
                             f"+0x{next(iter(v)) - u['text_start']:X} (object offset != va - text_start)")
        roffs_of[u["name"]] = {r["offset"] for r in sec["relocs"]}
        return roffs_of[u["name"]]

    rcov_of = {}

    def unit_rcover(u):  # T8 (2.2): object .text bytes inside a 4-byte reloc field (DIR32/REL32), same checks
        if u["name"] not in rcov_of:
            rcov_of[u["name"]] = {o + i for o in unit_roffs(u) for i in range(4)}
        return rcov_of[u["name"]]

    excl, why_out, pool, nd = dict.fromkeys(EXCL, 0), {}, [], {}
    for r in rows:
        tv = [int(v, 16) for v in r["targetVAs"]]
        u = None
        if tv:
            i = bisect.bisect_right(ustarts, tv[0]) - 1
            u = units[i] if i >= 0 and units[i]["text_start"] <= tv[0] < units[i]["text_end"] else None
        pv = int(r["protoVA"], 16) - PROTO_BASE
        for k, bad in zip(EXCL, (lambda: r["class"] != "identical", lambda: len(tv) != 1, lambda: u is None,
                                 lambda: u["kind"] != "game", lambda: not u["delink"],
                                 lambda: TAIL[0] <= pv < TAIL[1], lambda: tv[0] in folds,
                                 lambda: tv[0] not in funcs, lambda: BOUNDARY.match(funcs[tv[0]]["name"]))):
            if bad():
                excl[k] += 1
                if k == "unit not delink":  # 3.2 T8: residue doc rows per unit
                    nd.setdefault(u["index"], []).append((tv[0], r))
                for v in tv:
                    why_out.setdefault(v, k)
                break
        else:
            pool.append((r["length"], tv[0], r, u))
    pool.sort(key=lambda p: (p[0], p[1]))
    # 3.4 T2 near lane: va = candidateVA, size = the lane name's table size
    nexcl, npool, nrows = dict.fromkeys(NEAR_EXCL, 0), [], 0
    for r in rows:
        if r["class"] != "near-identical" or not r.get("candidateVA") or r.get("similarity") is None \
                or r["similarity"] < proto_map.THRESHOLD:
            continue
        nrows += 1
        va = int(r["candidateVA"], 16)
        i = bisect.bisect_right(ustarts, va) - 1
        u = units[i] if i >= 0 and units[i]["text_start"] <= va < units[i]["text_end"] else None
        pv = int(r["protoVA"], 16) - PROTO_BASE
        f = funcs.get(va)
        for k, bad in zip(NEAR_EXCL, (lambda: u is None, lambda: u["kind"] != "game", lambda: not u["delink"],
                                      lambda: TAIL[0] <= pv < TAIL[1], lambda: va in folds,
                                      lambda: f is None or f["size"] is None or f["status"] not in NEAR_STATUSES)):
            if bad():
                nexcl[k] += 1
                break
        else:
            npool.append((f["size"], va, r, u))
    npool.sort(key=lambda p: (p[0], p[1]))

    def confirm(r, va):  # 5.3 T3 (C0366): va past a capped hit list holds the row's masked bytes, as proto_map.run
        pva, n = int(r["protoVA"], 16), r["length"]
        pb, rb = nimg.pbytes(pva, n), nimg.rbytes(va, n)
        if pb is None or rb is None:
            return False
        masks, _e = proto_map.mask_proc(pb, pva, nimg.plo, nimg.phi)
        masks = sorted(set(masks) | {(s - pva, 4, "abs") for s in nimg.prelocs if pva <= s < pva + n})
        segs = proto_map.segments(n, masks)
        anc = proto_map.anchor(segs)
        return bool(anc) and proto_map.find_matches([(va, rb)], pb, segs, anc) == [va]

    actx = {"rows": rows, "funcs": funcs, "byname": func_names(), "units": units, "ustarts": ustarts, "folds": afolds, "pub": pub,
            "ivas": {p[1] for p in pool}, "confirm": confirm}  # 5.3 T3: --control amb-* replant from here
    aexcl, apool, acls, _why = amb_lane(**actx)

    def resolve(t, va, size):  # 3.4 T16: -> (status, member)
        return resolve_target(t, va, size, funcs, dstarts, data)

    out, near, amb = [], [], []
    for lane, (size, va, r, u) in ([("identical", p) for p in pool] + [("near", p) for p in npool]
                                   + [("ambiguous", p) for p in apool]):
        dst = {"identical": out, "near": near, "ambiguous": amb}[lane]  # ranks count per lane
        off = xbe_info.va_to_off(x, va)
        buf = xb[off:off + size]
        sym = funcs[va]
        names = sorted(r["names"])
        chk = {}
        # sig
        if not names:
            chk["sig"] = (False, "no prototype names")
        elif sym["name"].startswith("?"):
            chk["sig"] = (True, f"decorated name (symbols.txt {sym['source']})")
        else:
            cands = [ti for n, ti in procs.get(int(r["protoVA"], 16), []) if n in names]
            ok = [ti for ti in cands if leaves.get(ti) in PROC_LEAVES]
            if ok:
                chk["sig"] = (True, f"PDB TPI 0x{ok[0]:X} {PROC_LEAVES[leaves[ok[0]]]}")
            elif cands:
                chk["sig"] = (False, f"PDB type 0x{cands[0]:X} leaf "
                                     f"{'none' if cands[0] not in leaves else hex(leaves[cands[0]])} not a procedure")
            else:
                chk["sig"] = (False, "no PDB proc at protoVA with a prototype name")
        # reloc / c0035
        masks, _e = proto_map.mask_proc(buf, va, lo_img, hi_img)
        m = nimg.match(int(r["protoVA"], 16), va, size) if lane != "near" else None  # near: no identity
        consts = set(m[0]) if m else set()
        sites, unres, prov, c35, c35s = [], [], [], [], []
        for o, _n, k in masks:
            if k == "rel":
                t = (va + o + 4 + struct.unpack_from("<i", buf, o)[0]) & 0xFFFFFFFF
                if va <= t < va + size:
                    continue
                if u["text_start"] <= t < u["text_end"]:
                    c35.append(t)
                    c35s.append(va + o - u["text_start"])
            else:
                t = struct.unpack_from("<I", buf, o)[0]
            st_, mem = ("constant", None) if k == "abs" and o in consts else resolve(t, va, size)
            sites.append({"off": o, "kind": k, "target": t, "resolved": st_})
            if mem:
                sites[-1]["member"] = mem  # 3.4 T16
            if st_ is None:
                unres.append(t)
            elif not accepted(st_, t, funcs):  # 4.2 T9 (D115): near-corr func callee accepted
                prov.append(t)
        ut = sorted(set(unres))
        if ut:
            more = f" +{len(ut) - 4} more" if len(ut) > 4 else ""
            chk["reloc"] = (False, f"{len(ut)} unresolved target(s): " + ", ".join(f"0x{t:X}" for t in ut[:4]) + more)
        elif prov:
            chk["reloc"] = (False, f"provisional: {len(set(prov))} target(s) with status outside STATUSES")
        else:
            chk["reloc"] = (True, f"{len(sites)} site(s) resolved")
        miss = sum(s_ not in unit_roffs(u) for s_ in c35s) if c35s else 0
        chk["c0035"] = (not miss, f"{miss} intra-unit rel site(s) without a target reloc" if miss else
                        f"{len(c35s)} intra-unit rel site(s) relocated" if c35s else "0 intra-unit rel")
        # bodymax
        if va not in bodymax:
            chk["bodymax"] = (False, "no Ghidra function at VA")
        elif va + size - 1 > bodymax[va]:
            # T8 (2.2): bytes past the Ghidra body pass iff all lie in cut-object .text reloc fields (jump table)
            past = range(bodymax[va] + 1 - u["text_start"], va + size - u["text_start"])
            bare = sum(o not in unit_rcover(u) for o in past)
            if bare:
                chk["bodymax"] = (False, f"Ghidra body ends at +0x{bodymax[va] - va + 1:X} < size 0x{size:X}; "
                                         f"{bare} byte(s) past it not reloc-covered")
            else:
                chk["bodymax"] = (True, f"past Ghidra body +0x{bodymax[va] - va + 1:X}: {len(past)} byte(s) "
                                        "all reloc fields (jump table)")
        else:
            chk["bodymax"] = (True, "inside Ghidra body")
        # library
        lib = [n for n in names if n.startswith("std::")]
        if lib or "@std@@" in sym["name"]:
            chk["library"] = (False, "std:: name")
        elif any(any(p in m.lower() for p in XDK_MODS) for m in r["modules"]):
            chk["library"] = (False, "XDK module path")
        else:
            chk["library"] = (True, "game module, no std::")
        tg, derr = tags(buf, va, img, cxx)
        dst.append({"rank": len(dst) + 1, "va": va, "size": size, "names": names, "symbol": sym["name"],
                    "unit": u["name"], "modules": sorted(r["modules"]), "protoVA": r["protoVA"],
                    "pass": all(v[0] for v in chk.values()),
                    "checks": {k: {"pass": v[0], "reason": v[1]} for k, v in chk.items()}, "tags": tg,
                    "decodeErrors": derr, "sites": sites, "c0035Targets": sorted(set(c35)), "lane": lane})
        if lane == "near":
            dst[-1].update(similarity=r["similarity"], protoLength=r["length"])
        elif lane == "ambiguous":
            dst[-1]["amb_class"] = acls[va]  # 5.3 T3
    # T5 (2.1): relocscan's early-body rows (delink.py relocscan): symbols.txt func rows in a delink unit
    early, nofunc = [], 0
    for va, size, name in srows:
        i = bisect.bisect_right(ustarts, va) - 1
        if i < 0 or not units[i]["text_start"] <= va < units[i]["text_end"] or units[i].get("delink") is not True:
            continue
        if va not in bodymax:
            nofunc += 1
        elif bodymax[va] < va + size - 1:
            early.append((va, size, name, bodymax[va] - va + 1))
    done = {(f["symbol"], f["va"]) for f in tomllib.loads(need(MATCHED).read_text(encoding="utf-8")).get("function", [])}
    for o in out + near + amb:
        o["done"] = (o["symbol"], o["va"]) in done
    return {"pin": pin, "rows": len(rows), "excl": excl, "out": out, "near": near, "nexcl": nexcl, "nrows": nrows,
            "amb": amb, "aexcl": aexcl, "arows": sum(r["class"] == "ambiguous" for r in rows), "actx": actx,
            "pins": pins, "why_out": why_out, "done": done,
            "tpiver": tpiver, "cxx": cxx, "inputs": [FUNCS, UNITS, SYMS, SYMS_PIN, FOLDS, fj, PINS, MATCHED], "pdb": "default.pdb",
            "exe": "default.exe", "early": sorted(early), "nofunc": nofunc, "nd": nd, "funcs": funcs,
            "undelinked": [u for u in tomllib.loads(UNITS.read_text(encoding="utf-8"))["unit"]
                           if u["kind"] == "game" and u["delink"] is not True]}


def esc(s):
    return s.replace("|", "\\|")


def fails(row):
    return "; ".join(f"{k}: {v['reason']}" for k, v in row["checks"].items() if not v["pass"])


def pin_lines(res):
    by = {r["va"]: r for r in res["out"]}
    lines = []
    for p in res["pins"]:
        r = by.get(p["va"])
        if r is None:
            lines.append(f"- 0x{p['va']:08X} {p['name']}: not in pool ({res['why_out'].get(p['va'], 'no proto row')})")
            continue
        lines.append(f"- 0x{p['va']:08X} {p['name']} (rank {r['rank']}, size {r['size']}): "
                     f"{'pass' if r['pass'] else 'fail'}")
        lines.append("  - " + "; ".join(f"{k} {'pass' if v['pass'] else 'FAIL'} ({v['reason']})"
                                         for k, v in r["checks"].items()))
        lines.append("  - tags " + " ".join(f"{k}={v}" for k, v in r["tags"].items()))
    return lines


def counts(res):
    out = res["out"]
    fc = {k: sum(not r["checks"][k]["pass"] for r in out) for k in CHECKS}
    return fc, sum(r["pass"] for r in out)


def render(res):
    out = res["out"]
    fc, npass = counts(res)
    tagc = {k: sum(r["tags"][k] == "y" for r in out) for k in TAGS}
    rel = lambda p: p.relative_to(REPO).as_posix() if p.is_relative_to(REPO) else p.name
    L = ["# Next candidates: first-match filter + idiom tags", "",
         "Generated by `PY tools/candidates.py --write` (T2, phase 1.6); `--check` compares. Do not hand-edit.",
         f"Retail `default.xbe` sha1 `{res['pin']}`. Inputs: " + ", ".join(f"`{rel(p)}`" for p in res["inputs"])
         + f", prototype `{res['pdb']}` (TPI version {res['tpiver']}) + `{res['exe']}` (proto VAs). "
         "Names, VAs, sizes, units, tags, verdicts and reasons only (D46).", "",
         "## Rules",
         "- Pool (1.4 rule, docs/evidence/compiler_pin.md:8-17): proto_map class identical; one targetVA; unit "
         "(units.toml `text_start <= va < text_end`) kind game and delink true; not tail (protoVA - 0x400400 in "
         "0x13BD4E..0x13F2F4); VA not a retail_folds retailVA; a symbols.txt `func` line at the VA.",
         "- Rank: (size, VA) ascending. A row passes iff every check passes.",
         "- sig: decorated symbols.txt name (`?` prefix), or the PDB S_*PROC32* at the protoVA with a prototype name "
         "has a type index whose TPI leaf is LF_PROCEDURE/LF_MFUNCTION (0x0008/0x0009 16t or 0x1008/0x1009); "
         "fails also when the row has no prototype names.",
         "- reloc: sites = proto_map.mask_proc on the retail bytes (abs all; rel only if the target is outside "
         "[va, va+size)). Resolved iff target inside own extent, == a symbols table func VA, or inside a symbols table "
         "data row [va, va+size) (size `-` = exact VA, or the member extent [va, min(va + S, next pinned VA)) when "
         "the PDB public of that name has S_GDATA32/S_LDATA32 records of one type size S; the site then carries "
         "`member` name+offset), with the row status in units_seed.STATUSES "
         f"({', '.join(units_seed.STATUSES)}) or the pin status ({', '.join(PIN_STATUSES)}), or "
         f"{' / '.join(CALLEE_STATUSES)} on a func row only (D115; 4.2 T9). Tables: "
         "config/retail/symbols.txt, then config/retail/symbols_pin.txt (as gate.py). \"provisional\" = a "
         "resolving row's status outside both.",
         "- c0035: rel sites whose target lies in the unit's [text_start, text_end) and outside own extent: each must "
         "carry a reloc in the unit's cut object .text at offset va + site - text_start (object sha256 == its hashes.txt "
         "line; function symbol offsets == va - text_start).",
         "- bodymax: a Ghidra applied functions.jsonl entry == VA and va+size-1 <= bodyMax, or every byte of "
         "(bodyMax, va+size) lies inside the 4-byte field of a reloc in the unit's cut object .text (object offset "
         "= va - text_start; a jump table; same object checks as c0035).",
         "- library: fails on a `std::` prototype name, `@std@@` in the decorated name, or an XDK module path "
         "(`obj\\i386\\`, `D:\\xboxfre`, `* Linker *`).",
         "",
         "## Detectors (iced-x86 32-bit decode of [va, va+size))",
         "- float: an instruction whose first non-prefix byte is an x87 escape D8..DF.",
         "- switch: `jmp [index*4+disp32]` with no base register.",
         "- bittest: bt/bts/btr/btc, or `test` with a power-of-two immediate.",
         "- inlined: always `?` (no reliable detector: short inline procs are mostly proto_map ambiguous, C0010; the "
         "1.4 containment scan gave 0 pool hits).",
         "- vcall: `call` with a memory operand that has a base register.",
         "- eh (compiler_pin.md:21-22): an `fs:[0]` access; or a near call to a proc whose first 4 instructions touch "
         f"`fs:[0]`; or `push imm32`/`mov eax,imm32` of a handler that is `{CXX}` or whose first 3 instructions "
         "jmp there.",
         "- Tag values: `y` found; `-` not found; `?` not found but the body had decode errors (inlined always `?`).",
         "",
         "## Denominators",
         f"- functions.jsonl rows {res['rows']}. Excluded in order: "
         + "; ".join(f"{k} {v}" for k, v in res["excl"].items()) + f". Pool {len(out)}.",
         f"- Pass {npass}; fail {len(out) - npass}. Rows failing each check (a row may fail several): "
         + "; ".join(f"{k} {v}" for k, v in fc.items()) + ".",
         "- Tags `y` in the pool: " + "; ".join(f"{k} {v}" for k, v in tagc.items()) + ".",
         "- done: `y` iff (symbol, va) is a `[[function]]` row of config/matched_set.toml (every row gate-verified, D40). "
         f"Done {sum(r['done'] for r in out)} in the pool of {len(res['done'])} matched.",
         "", "## Pins (config/compiler_pin.toml)"] + pin_lines(res) + ["", "## Matched (config/matched_set.toml)", ""]
    # was f"- 0x{va} {names} (rank {rank}, size {size}): done" (5.5 T27.c4: rank/size are in the tables)
    L += [f"- 0x{r['va']:08X} {esc('; '.join(r['names']))}" for r in out if r["done"]] or ["None."]
    L += ["", "## Passing rows", ""]
    hdr = "| rank | VA | size | name | unit | " + " | ".join(TAGS) + " | done |"
    sep = "|" + "---|" * (6 + len(TAGS))
    good = [r for r in out if r["pass"]]
    if good:
        L += [hdr, sep] + [f"| {r['rank']} | 0x{r['va']:08X} | {r['size']} | {esc('; '.join(r['names']))} | "
                           f"{esc(r['unit'])} | " + " | ".join(r["tags"][k] for k in TAGS) + f" | {'y' if r['done'] else '-'} |"
                           for r in good]
    else:
        L.append("None.")
    # was "## All rows (rank order)" over every row; 5.5 T27.c4: failing rows only (passing rows are above), doc size
    L += ["", "## Failing rows (rank order)", "", hdr[:-1] + "| verdict | reasons |", sep + "---|---|"]
    L += [f"| {r['rank']} | 0x{r['va']:08X} | {r['size']} | {esc('; '.join(r['names']))} | {esc(r['unit'])} | "
          + " | ".join(r["tags"][k] for k in TAGS) + f" | {'y' if r['done'] else '-'} | {'pass' if r['pass'] else 'fail'} | "
          f"{esc(fails(r))} |"
          for r in out if not r["pass"]]
    L += ["", "## Ghidra bodies that end early", "",
          "symbols.txt func rows in a delink unit whose applied functions.jsonl bodyMax < va+size-1 (as `delink.py "
          f"relocscan`): {len(res['early'])}; rows with no Ghidra function at entry: {res['nofunc']}.", ""]
    L += [f"- {esc(n)} va 0x{va:08X} size 0x{sz:X} at +0x{at:X}" for va, sz, n, at in res["early"]] or ["None."]
    return "\n".join(L) + "\n"


def render_near(res):  # 3.4 T2: appended after render(res); identical sections untouched
    near = res["near"]
    fc = {k: sum(not r["checks"][k]["pass"] for r in near) for k in CHECKS}
    npass = sum(r["pass"] for r in near)
    bk = lambda lo, hi: f"{lo}-{hi}" if hi else f"{lo}+"
    inb = lambda r, lo, hi: lo <= r["size"] and (hi is None or r["size"] <= hi)
    L = ["", "## Near lane (3.4 T2)", "",
         "- Rows (C0142): proto_map class near-identical with a candidateVA and similarity >= proto_map.THRESHOLD "
         f"({proto_map.THRESHOLD}); va = candidateVA. Excluded in order: no unit, unit not game, unit not delink, tail, "
         "fold (as the pool rule), no lane name (no symbols table `func` line at va, its size `-`, or its status not "
         f"{' / '.join(NEAR_STATUSES)}).",
         "- size = the table size of that `func` line; rank (size, VA) ascending within the lane.",
         "- Same checks and tags as the identical lane, with no identity: no name_sites.Images.match, so no "
         "`constant` sites.",
         "- Acceptance (D115, 4.2 T9): a near-corr target of any lane's site is accepted when it is a symbols table "
         "func VA (a called function), never a data row; the row's own name uses near-corr as above.",
         "",
         f"- Near rows {res['nrows']}. Excluded in order: " + "; ".join(f"{k} {v}" for k, v in res["nexcl"].items())
         + f". Pool {len(near)}.",
         f"- Pass {npass}; fail {len(near) - npass}. Rows failing each check (a row may fail several): "
         + "; ".join(f"{k} {v}" for k, v in fc.items()) + ".",
         "- By size bucket (match_one.BUCKETS), pool/passing: "
         + "; ".join(f"{bk(lo, hi)} {sum(inb(r, lo, hi) for r in near)}/{sum(inb(r, lo, hi) and r['pass'] for r in near)}"
                     for lo, hi in match_one.BUCKETS) + ".",
         f"- Done {sum(r['done'] for r in near)}.", "", "### Near passing rows", ""]
    hdr = "| rank | VA | size | sim | name | unit | " + " | ".join(TAGS) + " | done |"
    sep = "|" + "---|" * (7 + len(TAGS))
    row = lambda r: (f"| {r['rank']} | 0x{r['va']:08X} | {r['size']} | {r['similarity']} | {esc('; '.join(r['names']))} | "
                     f"{esc(r['unit'])} | " + " | ".join(r["tags"][k] for k in TAGS) + f" | {'y' if r['done'] else '-'} |")
    good = [r for r in near if r["pass"]]
    L += [hdr, sep] + [row(r) for r in good] if good else ["None."]
    # was "### Near all rows (rank order)" over every row (5.5 T27.c4: failing only)
    L += ["", "### Near failing rows (rank order)", "", hdr[:-1] + "| verdict | reasons |", sep + "---|---|"]
    L += [row(r) + f" {'pass' if r['pass'] else 'fail'} | {esc(fails(r))} |" for r in near if not r["pass"]]
    return "\n".join(L) + "\n"


def render_amb(res):  # 5.3 T3: appended after render_near(res)
    amb = res["amb"]
    fc = {k: sum(not r["checks"][k]["pass"] for r in amb) for k in CHECKS}
    npass = sum(r["pass"] for r in amb)
    cls = {}
    for r in amb:
        cls[r["amb_class"]] = cls.get(r["amb_class"], 0) + 1
    L = ["", "## Ambiguous lane (5.3 T3)", "",
         "- Rows: proto_map class ambiguous whose name (a PDB public at the protoVA, or a row name) is a symbols table "
         "`func` at one of the row's own hits (targetVAs; a row with proto_map.HIT_CAP "
         f"({proto_map.HIT_CAP}) hits also at a VA past the list where proto_map.find_matches confirms the masked "
         "bytes, C0366); va = that VA, size = proto length.",
         "- Excluded in order: no unit, unit not game, unit not delink, tail (as the pool rule, at va); fold (a true "
         "fold: a retail_folds retailVA that another proto row has as its sole identical target, so two rows claim "
         "the body; a va listed only as ambiguous hits is no fold; 5.3 T13.2); boundary (as the pool rule); no pin; pin outside targets; pin not accepted (accepted() false or status site-corr); shared body (a va "
         "claimed by two ambiguous rows, or by one and an identical pool row: every ambiguous claimant).",
         "- Same checks and tags as the identical lane (name_sites.Images.match constants included); rank (size, VA) "
         "ascending within the lane. class = the pin's source tag middle field (unit-bracket bracket, anchor-window "
         "D115, site-corr+callee-bytes caller-xref, D113 tags D113; symbols.txt symbols).",
         "",
         f"- Ambiguous rows {res['arows']}. Excluded in order: " + "; ".join(f"{k} {v}" for k, v in res["aexcl"].items())
         + f". Pool {len(amb)}.",
         "- By class: " + ("; ".join(f"{k} {v}" for k, v in sorted(cls.items())) or "none") + ".",
         f"- Pass {npass}; fail {len(amb) - npass}. Rows failing each check (a row may fail several): "
         + "; ".join(f"{k} {v}" for k, v in fc.items()) + ".",
         f"- Done {sum(r['done'] for r in amb)}.", "", "### Ambiguous passing rows", ""]
    hdr = "| rank | VA | size | class | name | unit | " + " | ".join(TAGS) + " | done |"
    sep = "|" + "---|" * (7 + len(TAGS))
    row = lambda r: (f"| {r['rank']} | 0x{r['va']:08X} | {r['size']} | {r['amb_class']} | {esc('; '.join(r['names']))} | "
                     f"{esc(r['unit'])} | " + " | ".join(r["tags"][k] for k in TAGS) + f" | {'y' if r['done'] else '-'} |")
    good = [r for r in amb if r["pass"]]
    L += [hdr, sep] + [row(r) for r in good] if good else ["None."]
    # was "### Ambiguous all rows (rank order)" over every row (5.5 T27.c4: failing only)
    L += ["", "### Ambiguous failing rows (rank order)", "", hdr[:-1] + "| verdict | reasons |", sep + "---|---|"]
    L += [row(r) + f" {'pass' if r['pass'] else 'fail'} | {esc(fails(r))} |" for r in amb if not r["pass"]]
    return "\n".join(L) + "\n"


def render_residue(res):  # 3.2 T8: each undelinked game unit, its route reason and its "unit not delink" rows
    nd, units = res["nd"], sorted(res["undelinked"], key=lambda u: u["index"])
    total = sum(len(v) for v in nd.values())
    if total != res["excl"]["unit not delink"] or set(nd) - {u["index"] for u in units}:
        raise Refuse(f"residue rows {total} != excluded unit not delink {res['excl']['unit not delink']}")
    L = ["# Delink residue: game units not delinked", "",
         "Generated by `PY tools/candidates.py --write` (3.2 T8); `--check` compares. Do not hand-edit.",
         f"Retail `default.xbe` sha1 `{res['pin']}`. Reason: config/retail/units.toml `delink_reason` (route: "
         "`units_seed.py --route`, rules docs/evidence/units.md \"Unit-edge route\"); rows: proto_map identical rows "
         "excluded as `unit not delink` (as next_candidates.md). Names, VAs and counts only (D46).", "",
         f"Units {len(units)}. Total `unit not delink` rows: {total}.", ""]
    for u in units:
        items, cur = [], None
        for s in [x for x in u["delink_reason"].split("; ") if x]:
            if cur is not None and s.startswith(("searched: ", "fails: ")):
                k, v = s.split(": ", 1)
                cur[k] = v
            else:
                cur = {"item": s}
                items.append(cur)
        rows = sorted(nd.get(u["index"], []), key=lambda t: t[0])
        L.append(f"## {esc(u['name'])}#{u['index']}")
        for it in items:
            L.append(f"- reason: {esc(it['item'])}")
            L.append(f"  - searched: {esc(it.get('searched', 'not routed yet'))}")
            if "fails" in it:
                L.append(f"  - fails: {esc(it['fails'])}")
        L.append(f"- rows {len(rows)}" + (":" if rows else "."))
        for va, r in rows:
            nm = res["funcs"][va]["name"] if va in res["funcs"] else (sorted(r["names"]) or ["-"])[0]
            L.append(f"  - 0x{va:08X} {esc(nm)}")
        L.append("")
    return "\n".join(L)


def _differs(path, text):  # -> None or the first-difference message (as --check of DOC)
    if not path.is_file():
        return f"{path} missing"
    old, new = path.read_text(encoding="utf-8").splitlines(), text.splitlines()
    for i, (a, b) in enumerate(zip(old, new), 1):
        if a != b:
            return f"{path.name} differs at line {i}"
    if len(old) != len(new):
        return f"{path.name} differs at line {min(len(old), len(new)) + 1} (length {len(old)} vs {len(new)})"
    return None


def control_member():
    """--control member (3.4 T16): one line per case, then the summary; 0 iff all hold. Writes nothing."""
    _pin, _b, _t, _xp, xb, x = byte_compare.setup(argparse.Namespace(symbols=None, xbe=None, config=ARGS.config))
    cfg = vv_inputs.load_config(ARGS.config)
    pdb_b = need(vv_inputs.input_path("prototype_dir", "default.pdb", cfg)).read_bytes()
    exe = need(vv_inputs.input_path("prototype_dir", "default.exe", cfg)).read_bytes()
    funcs, data = load_symbols()
    ext = member_extents(funcs, data, pdb_b, exe, name_sites.Images(exe, pe_info.parse_pe(exe), xb, x), ARGS.config)
    dstarts = [d[0] for d in ext]
    res = lambda t: resolve_target(t, 0, 0, funcs, dstarts, ext)
    # planted targets: not a func VA and inside no other row (linear scan, independent of resolve_target)
    lo = x["header"]["baseAddr"]  # in-image rows only (absolute symbols such as __except_list sit at VA 0)
    free = lambda t, k: t not in funcs and not any(d[0] <= t < d[1] for j, d in enumerate(ext) if j != k) and t > lo
    beep = next((d for d in ext if d[3] == "?beepState@@3VBeepState@@A"), None)
    a = next(((k, d) for k, d in enumerate(ext) if d[4] and d[1] == d[0] + d[4] and free(d[1], k)), None)
    b = next(((k, d) for k, d in enumerate(ext) if not data[k][4] and d[4] is None and free(d[0] + 1, k)), None)
    cases = [("positive", 0x235794, (beep[2], "?beepState@@3VBeepState@@A+0x1C") if beep else "row", ""),
             ("planted-a", a[1][1] if a else None, (None, None), f" (g+S of {a[1][3]}, S=0x{a[1][4]:X})" if a else ""),
             ("planted-b", b[1][0] + 1 if b else None, (None, None), f" (g+1 of {b[1][3]}, no PDB size)" if b else "")]
    held = 0
    for nm, t, want, why in cases:
        got = res(t) if t is not None and want != "row" else None
        ok = got == want
        held += ok
        print(f"member {nm} " + (f"0x{t:08X}{why} -> {got[0] or 'unresolved'}" + (f" {got[1]}" if got[1] else "")
                                 if got else "no case found") + (": held" if ok else ": FAILED"))
    print(f"controls: {held} held as expected of {len(cases)}")
    return 0 if held == len(cases) else 1


def control_amb(kind):
    """--control amb-outside|amb-site-corr|amb-shared (5.3 T3): plant in memory on the first admitted ambiguous row;
    0 iff the planted row(s) are refused with the expected AMB_EXCL key, else DID NOT FAIL and 1. Writes nothing.
    amb-fold (5.3 T13.2): (a) pool[0]'s va given a planted second claiming protoVA in folds -> fold; (b) a bracket-pinned
    row whose va is a retail_folds retailVA with no identical-unique proto, admitted with that va dropped from folds,
    is admitted with folds as loaded. 0 iff both hold."""
    ctx = compute()["actx"]
    _e, pool, _c, _w = amb_lane(**ctx)
    if not pool:
        print(f"{kind}: no admitted ambiguous row to plant on")
        return 1
    _s, va, r, _u = pool[0]
    if kind == "amb-fold":
        folds, held = ctx["folds"], 0
        fa = {**folds, va: folds.get(va, frozenset()) | {"0xFFFFFFF0"}}
        got = amb_lane(**dict(ctx, folds=fa))[3].get(r["protoVA"], "admitted")
        held += got == "fold"
        print(f"amb-fold (a) row {r['protoVA']} {ctx['funcs'][va]['name']} (planted claim 0xFFFFFFF0 at 0x{va:08X}) -> "
              f"{got}: " + ("refused" if got == "fold" else "DID NOT FAIL"))
        byname, pub, b = ctx["byname"], ctx["pub"], None
        for r2 in (x for x in ctx["rows"] if x["class"] == "ambiguous"):
            ns = pub.get(int(r2["protoVA"], 16), set()) | set(r2["names"])
            for v in sorted({v for n in ns for v in byname.get(n, ())}):
                f = ctx["funcs"].get(v)
                if f and amb_class_of(f) == "bracket" and v in folds and not folds[v] - {r2["protoVA"]}:
                    fb = {k: s for k, s in folds.items() if k != v}
                    if amb_lane(**dict(ctx, folds=fb))[3].get(r2["protoVA"], "admitted") == "admitted":
                        b = (r2, v)
                        break
            if b:
                break
        if b is None:
            print("amb-fold (b) no bracket row with an ambiguous-hit-only fold va found: DID NOT FAIL")
            return 1
        got = _w.get(b[0]["protoVA"], "admitted")
        held += got == "admitted"
        print(f"amb-fold (b) row {b[0]['protoVA']} {ctx['funcs'][b[1]]['name']} (0x{b[1]:08X} in retail_folds as an "
              f"ambiguous hit only) -> {got}: " + ("admitted" if got == "admitted" else "DID NOT FAIL"))
        print(f"controls: {held} held as expected of 2")
        return 0 if held == 2 else 1
    if kind == "amb-shared":  # the twin has no proto bytes: plant on a row whose pin is in its listed targetVAs
        _s, va, r, _u = next((p for p in pool if f"0x{p[1]:08X}" in p[2]["targetVAs"]), pool[0])
    funcs, rows = dict(ctx["funcs"]), ctx["rows"]
    if kind == "amb-outside":  # move the pin to the next table func VA outside the row's targetVAs
        tv = {int(v, 16) for v in r["targetVAs"]}
        to = next(v for v in sorted(funcs) if v > va and v not in tv)
        funcs[to] = funcs.pop(va)
        byname = {n: {to if v == va else v for v in vs} for n, vs in ctx["byname"].items()}
        ctx = dict(ctx, byname=byname)
        want, keys, what = "pin outside targets", [r["protoVA"]], f"pin moved 0x{va:08X} -> 0x{to:08X}"
    elif kind == "amb-site-corr":
        funcs[va] = dict(funcs[va], status="site-corr")
        want, keys, what = "pin not accepted", [r["protoVA"]], "pin status site-corr"
    else:  # amb-shared: a second row (its own protoVA) whose name is the same pin
        r2 = dict(r, protoVA="0xFFFFFFF0", names=[funcs[va]["name"]])
        rows = rows + [r2]
        want, keys, what = "shared body", [r["protoVA"], r2["protoVA"]], f"second row pinned at 0x{va:08X}"
    _e, _p, _c, why = amb_lane(**dict(ctx, funcs=funcs, rows=rows))
    got = [why.get(k, "admitted") for k in keys]
    ok = all(g == want for g in got)
    print(f"{kind} row {r['protoVA']} {funcs.get(va, ctx['funcs'][va])['name']} ({what}) -> {', '.join(got)}: "
          + ("refused" if ok else "DID NOT FAIL"))
    return 0 if ok else 1


def main(argv):
    global ARGS
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--write", action="store_true", help="write the doc + derived jsonl")
    g.add_argument("--check", action="store_true", help="fresh render vs committed doc; exit 1 if they differ")
    g.add_argument("--control", choices=["member", "amb-outside", "amb-site-corr", "amb-shared", "amb-fold"],
                   help="member rule control (3.4 T16), ambiguous lane plants (5.3 T3, amb-fold T13.2); writes nothing")
    ap.add_argument("--config", help="local_paths.json (default config/local_paths.json)")
    ARGS = ap.parse_args(argv)
    if ARGS.control:
        try:
            return control_member() if ARGS.control == "member" else control_amb(ARGS.control)
        except (Refuse, byte_compare.Refuse, pdb_info.NotPdb, pe_info.NotPe, struct.error) as e:
            print(f"candidates: REFUSED {e}", file=sys.stderr)
            return 2
    try:
        pre = {k: _sha256(q) for k, q in stamp_inputs(ARGS.config)} if ARGS.write else None  # 5.5 T6: before compute
        res = compute()
    except (Refuse, byte_compare.Refuse) as e:
        print(f"candidates: REFUSED {e}", file=sys.stderr)
        return 2
    out = res["out"]
    miss = []  # 5.5 T6 (C3): an EXPECT* mismatch exits 1; was printed only
    for k, v in res["excl"].items():
        miss += [] if v == EXPECT[k] else [f"excluded {k}"]
        print(f"excluded {k} {v}" + ("" if v == EXPECT[k] else f" (expected {EXPECT[k]})"))
    miss += [] if len(out) == EXPECT_POOL else ["pool"]
    print(f"pool {len(out)}" + ("" if len(out) == EXPECT_POOL else
                                f" (expected {EXPECT_POOL}: steps differing above, or inputs regenerated since 1.4)"))
    near = res["near"]  # 3.4 T2
    for k, v in res["nexcl"].items():
        miss += [] if v == EXPECT_NEAR[k] else [f"near excluded {k}"]
        print(f"near excluded {k} {v}" + ("" if v == EXPECT_NEAR[k] else f" (expected {EXPECT_NEAR[k]})"))
    miss += [] if len(near) == EXPECT_NEAR_POOL else ["near pool"]
    print(f"near pool {len(near)} of {res['nrows']}" + ("" if len(near) == EXPECT_NEAR_POOL else
                                                         f" (expected {EXPECT_NEAR_POOL})"))
    amb = res["amb"]  # 5.3 T3
    for k, v in res["aexcl"].items():
        miss += [] if v == EXPECT_AMB[k] else [f"amb excluded {k}"]
        print(f"amb excluded {k} {v}" + ("" if v == EXPECT_AMB[k] else f" (expected {EXPECT_AMB[k]})"))
    miss += [] if len(amb) == EXPECT_AMB_POOL else ["amb pool"]
    print(f"amb pool {len(amb)} of {res['arows']}" + ("" if len(amb) == EXPECT_AMB_POOL else
                                                       f" (expected {EXPECT_AMB_POOL})"))
    if miss:
        print(f"candidates: FAIL counts differ from EXPECT*: {', '.join(miss)} (nothing written)")
        return 1
    fc, npass = counts(res)
    doc = render(res) + render_near(res) + render_amb(res)
    try:
        residue = render_residue(res)
    except Refuse as e:
        print(f"candidates: REFUSED {e}", file=sys.stderr)
        return 2
    if ARGS.check:
        stale = fresh(ARGS.config)  # 5.5 T6 (S4): was docs only
        if stale:
            print("candidates: stale stamp: " + "; ".join(stale))
            return 1
        why = _differs(RESIDUE, residue)
        if why:
            print(why)
            return 1
        if not DOC.is_file():
            print(f"candidates: {DOC} missing")
            return 1
        old = DOC.read_text(encoding="utf-8").splitlines()
        new = doc.splitlines()
        for i, (a, b) in enumerate(zip(old, new), 1):
            if a != b:
                print(f"differs at line {i}")
                return 1
        if len(old) != len(new):
            print(f"differs at line {min(len(old), len(new)) + 1} (length {len(old)} vs {len(new)})")
            return 1
        print("check: doc up to date")
        return 0
    print("fail " + "; ".join(f"{k} {v}" for k, v in fc.items()) + f"; pass {npass}")
    print("near fail " + "; ".join(f"{k} {sum(not r['checks'][k]['pass'] for r in near)}" for k in CHECKS)
          + f"; pass {sum(r['pass'] for r in near)}")
    if ARGS.write:  # 5.5 T6 (S4): jsonl first, then its stamp, then the docs; was docs first, plain writes
        OUT.parent.mkdir(parents=True, exist_ok=True)
        _replace(OUT, "".join(json.dumps(r, sort_keys=True) + "\n" for r in out + near + amb))
        _replace(STAMP, json.dumps({"inputs": pre, "out": _sha256(OUT)}, indent=1, sort_keys=True) + "\n")
        DOC.parent.mkdir(parents=True, exist_ok=True)
        DOC.write_text(doc, encoding="utf-8", newline="\n")
        RESIDUE.write_text(residue, encoding="utf-8", newline="\n")
        print("Pins")
        print("\n".join(pin_lines(res)))
        print(f"wrote {DOC.relative_to(REPO).as_posix()}, {RESIDUE.relative_to(REPO).as_posix()} and "
              f"{OUT.relative_to(REPO).as_posix()}")
    return 0


ARGS = None

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
