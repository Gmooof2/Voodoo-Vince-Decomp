"""Prototype-to-XBE function map by masked-byte identity (T8, phase 1.2) + four-class classification (T9).

CLI: proto_map.py [--target prototype-xbe|retail-xbe] [--pair default|qa]   (defaults retail-xbe, default)
     --pair qa maps Vince_qa.exe/.pdb instead of default.exe/.pdb (T11; QA-build evidence only, never a pin)
     proto_map.py --self-test     (synthetic code built with iced-x86's encoder; no game bytes)
T9 classes: identical (1 masked hit), ambiguous (>=2 hits, stored hits capped at 32), near-identical (best token
similarity >= THRESHOLD inside the anchor window), absent; excluded with reason zero-length / no-raw-bytes /
data-in-text / thunk. Mask also covers every PE `.reloc` HIGHLOW site inside the proc. Anchors = LIS of
(protoVA -> targetVA) over identical procs; window = target gap between neighbouring anchors; similarity =
100 * difflib ratio of per-instruction tokens (rel branch targets and in-image abs values wildcarded).
Writes functions.jsonl, near_identical.jsonl, absent.jsonl, summary.json.
Exit: 0 mapped (or self-test passed); 1 a self-test check failed; 2 refusal (default.exe, default.pdb or the target
not matching the manifest by sha1+size, the PE/PDB pair invalid, an unparseable input, output outside derived/).
Prints counts with denominators, addresses, names and hashes only, never byte runs or disassembly (D46).
Method: procs from the prototype default.pdb (every module, dedup by VA keeping all names); bytes from default.exe
(VA = imageBase + section[seg-1].va + off). Each proc is decoded with iced-x86 (32-bit); masked fields are the rel32
of near call/jmp/jcc and every 4-byte imm/disp whose value lies in [imageBase, imageBase+SizeOfImage) of the PE;
rel8 stays unmasked; T9 adds every `.reloc` (data dir 5, HIGHLOW) site inside the proc to the mask. The reloc
cross-check still compares decoded imm/disp mask starts vs reloc sites inside decoded proc ranges (both mismatch
counts printed). Target: executable XBE sections
(SEC_FLAGS 0x4); anchor = the proc's longest unmasked run (bytes.find), then a full masked compare per hit.
"Identical" = masked-byte identity, never match evidence (D42: every record carries status_note unverified).
Output: derived/proto_map/<target>/functions.jsonl + summary.json (names, modules, VAs, lengths, statuses; no bytes);
pair qa -> derived/proto_map/qa/<target>/ (summary adds pair, protoExe, protoPdb); pair qa + retail-xbe also writes
docs/evidence/qa_absent.md: QA lookup (qa_join) of every default-pair absent row, names/VAs/class/similarity only.
Importable: mask_proc(buf, va, lo, hi), segments(n, masks), anchor(segs), find_matches(secs, buf, segs, anc),
parse_reloc_blocks(blob, image_base), reloc_crosscheck(relocs, abs_masks, ranges), tokens(buf, va, lo, hi),
candidate_starts(term, pad), similarity(a, b), is_thunk(buf, va), lis_anchors(pairs), best_candidate(ptok, gap),
qa_join(absent_rows, qa_rows).
"""
import argparse
import bisect
import difflib
import json
import math
import random
import struct
import sys
from pathlib import Path

from iced_x86 import Code, Decoder, FlowControl, Mnemonic, OpKind

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

import manifest  # noqa: E402
import pdb_info  # noqa: E402
import pe_info  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

TARGETS = {"prototype-xbe": ("prototype_dir", "default.xbe"), "retail-xbe": ("retail_xbe", None)}
PAIRS = {"default": ("default.exe", "default.pdb"), "qa": ("Vince_qa.exe", "Vince_qa.pdb")}  # role prototype_dir
QA_DOC = REPO / "docs" / "evidence" / "qa_absent.md"
SEC_EXEC = 0x4
HIT_CAP = 32  # stored hits per proc; >=2 already means ambiguous (T9; T8 had MIN_LEN 6 / MIN_ANCHOR 4 skips)
THRESHOLD = 70  # near-identical similarity cut; reason in docs/evidence/correspondence.md (T9)
DECOY_SHIFT = 50  # decoy window = gap index + 50 (wrap)
DERIVED = REPO / "derived"
WILD = "*"
_IMM_KINDS = {OpKind.IMMEDIATE8, OpKind.IMMEDIATE8_2ND, OpKind.IMMEDIATE16, OpKind.IMMEDIATE32,
              OpKind.IMMEDIATE8TO16, OpKind.IMMEDIATE8TO32, OpKind.IMMEDIATE64, OpKind.IMMEDIATE8TO64,
              OpKind.IMMEDIATE32TO64}
_BR_KINDS = {OpKind.NEAR_BRANCH16, OpKind.NEAR_BRANCH32, OpKind.NEAR_BRANCH64}
_INTERN = {}


def _refuse(msg):
    print(f"proto_map: {msg}", file=sys.stderr)
    sys.exit(2)


def mask_proc(buf, va, lo, hi):
    """Decode buf at va -> (masks [(off, 4, 'rel'|'abs')], decode errors)."""
    masks, errors = [], 0
    d = Decoder(32, buf, ip=va)
    off = 0
    while d.can_decode:
        ins = d.decode()
        if ins.code == Code.INVALID:
            errors += 1
            off += ins.len or 1
            continue
        c = d.get_constant_offsets(ins)
        branch = ins.is_call_near or ins.is_jmp_near or ins.is_jcc_short_or_near
        if c.has_immediate and c.immediate_size == 4:
            o = off + c.immediate_offset
            if branch:
                masks.append((o, 4, "rel"))
            elif lo <= struct.unpack_from("<I", buf, o)[0] < hi:
                masks.append((o, 4, "abs"))
        if c.has_displacement and c.displacement_size == 4:
            o = off + c.displacement_offset
            if lo <= struct.unpack_from("<I", buf, o)[0] < hi:
                masks.append((o, 4, "abs"))
        off += ins.len
    return sorted(masks), errors


def segments(n, masks):
    """Unmasked [start, end) runs of an n-byte proc."""
    out, pos = [], 0
    for o, sz, _ in masks:
        if o > pos:
            out.append((pos, o))
        pos = max(pos, o + sz)
    if pos < n:
        out.append((pos, n))
    return out


def anchor(segs):
    """Longest unmasked run (first on ties) or None."""
    return max(segs, key=lambda s: (s[1] - s[0], -s[0]), default=None)


def find_matches(secs, buf, segs, anc, cap=None):
    """secs: [(base VA, bytes)] -> sorted VAs where buf matches on every unmasked byte (first `cap` in scan order)."""
    n, a0, a1 = len(buf), anc[0], anc[1]
    key = buf[a0:a1]
    hits = []
    for base, hay in secs:
        i = hay.find(key)
        while i >= 0:
            st = i - a0
            if st >= 0 and st + n <= len(hay) and all(hay[st + s:st + e] == buf[s:e] for s, e in segs):
                hits.append(base + st)
                if cap and len(hits) >= cap:
                    return sorted(hits)
            i = hay.find(key, i + 1)
    return sorted(hits)


def parse_reloc_blocks(blob, image_base):
    """Base-reloc blocks -> set of HIGHLOW site VAs."""
    out, o = set(), 0
    while o + 8 <= len(blob):
        page, size = struct.unpack_from("<II", blob, o)
        if size < 8:
            break
        for k in range(o + 8, min(o + size, len(blob)) - 1, 2):
            e = struct.unpack_from("<H", blob, k)[0]
            if e >> 12 == 3:
                out.add(image_base + page + (e & 0xFFF))
        o += size
    return out


def reloc_crosscheck(relocs, abs_masks, ranges):
    """(reloc-not-masked, masked-not-reloc) counts inside the [lo, hi) proc ranges."""
    ranges = sorted(ranges)
    starts = [r[0] for r in ranges]

    def inside(v):
        i = bisect.bisect_right(starts, v) - 1
        return i >= 0 and v < ranges[i][1]
    r = {v for v in relocs if inside(v)}
    m = set(abs_masks)
    return len(r - m), len(m - r)


def tokens(buf, va, lo, hi):
    """Decode buf at va -> (tokens [int per instr], starts [VA], term [bool], pad [bool], errors).
    Token = interned (code, operands): registers, memory seg/base/index/scale/disp, imm values; rel branch targets
    (near and short) and abs disp/imm values in [lo, hi) become WILD. term = ret/jmp/int3; pad = int3/nop."""
    toks, starts, term, pad, errors = [], [], [], [], 0
    d = Decoder(32, buf, ip=va)
    while d.can_decode:
        ins = d.decode()
        starts.append(ins.ip)
        if ins.code == Code.INVALID:
            errors += 1
            key = (Code.INVALID,)
        else:
            ops = []
            for i in range(ins.op_count):
                k = ins.op_kind(i)
                if k == OpKind.REGISTER:
                    ops.append(("r", ins.op_register(i)))
                elif k == OpKind.MEMORY:
                    v = ins.memory_displacement & 0xFFFFFFFF
                    ops.append(("m", ins.memory_segment, ins.memory_base, ins.memory_index,
                                ins.memory_index_scale, WILD if lo <= v < hi else v))
                elif k in _BR_KINDS:
                    ops.append(WILD)
                elif k in _IMM_KINDS:
                    v = ins.immediate(i) & 0xFFFFFFFF
                    ops.append(("i", WILD if lo <= v < hi else v))
                else:
                    ops.append(("k", k))
            key = (ins.code, tuple(ops))
        toks.append(_INTERN.setdefault(key, len(_INTERN)))
        int3 = ins.code == Code.INT3
        term.append(int3 or ins.flow_control == FlowControl.RETURN or ins.mnemonic == Mnemonic.JMP)
        pad.append(int3 or ins.mnemonic == Mnemonic.NOP)
    return toks, starts, term, pad, errors


def candidate_starts(term, pad):
    """Instruction indices that may start a function: 0 and the first non-padding instr after each terminator."""
    out, n = {0} if term else set(), len(term)
    for i in range(n):
        if term[i]:
            j = i + 1
            while j < n and pad[j]:
                j += 1
            if j < n:
                out.add(j)
    return sorted(out)


def similarity(ptok, ctok):
    """100 * difflib ratio of two token lists."""
    return 100.0 * difflib.SequenceMatcher(None, ptok, ctok, autojunk=False).ratio()


def is_thunk(buf, va):
    """Decoded body is exactly one unconditional jmp (near rel or indirect)."""
    d, ins = Decoder(32, buf, ip=va), []
    while d.can_decode and len(ins) < 2:
        ins.append(d.decode())
    return (len(ins) == 1 and ins[0].mnemonic == Mnemonic.JMP and ins[0].flow_control in
            (FlowControl.UNCONDITIONAL_BRANCH, FlowControl.INDIRECT_BRANCH))


def lis_anchors(pairs):
    """pairs [(protoVA, targetVA, length)] -> longest strictly increasing targetVA subsequence in protoVA order."""
    pairs = sorted(pairs)
    tails, tidx, prev = [], [], [-1] * len(pairs)
    for i, (_, t, _) in enumerate(pairs):
        k = bisect.bisect_left(tails, t)
        if k == len(tails):
            tails.append(t)
            tidx.append(i)
        else:
            tails[k], tidx[k] = t, i
        prev[i] = tidx[k - 1] if k else -1
    out, i = [], tidx[-1] if tidx else -1
    while i >= 0:
        out.append(pairs[i])
        i = prev[i]
    return out[::-1]


def gap_ranges(anchors, g, secs):
    """Target [start, end) ranges of gap g (between anchors[g-1] and anchors[g]); secs [(va, end)] sorted."""
    def sec(v):
        i = bisect.bisect_right([s[0] for s in secs], v) - 1
        return secs[i] if i >= 0 and v < secs[i][1] else None
    p = anchors[g - 1] if g > 0 else None
    q = anchors[g] if g < len(anchors) else None
    sp, sq = (sec(p[1]) if p else None), (sec(q[1]) if q else None)
    if p and q and sp == sq:
        rs = [(p[1] + p[2], q[1])]
    else:
        rs = ([(p[1] + p[2], sp[1])] if sp else []) + ([(sq[0], q[1])] if sq else [])
    return sorted((a, b) for a, b in rs if a < b)


def best_candidate(ptok, gap):
    """gap [(toks, starts, cands)] -> (ratio, VA) best by ratio then lowest VA, or (None, None)."""
    best, bva, n = None, None, len(ptok)
    for toks, starts, cands in gap:
        for i in cands:
            sm = difflib.SequenceMatcher(None, ptok, toks[i:i + n], autojunk=False)
            if best is not None and (sm.real_quick_ratio() <= best or sm.quick_ratio() <= best):
                continue
            r = sm.ratio()
            if best is None or r > best:
                best, bva = r, starts[i]
    return best, bva


def assign_gap(items, gap):
    """items [(key, protoVA, ptok)], gap [(toks, starts, cands)] -> {key: (ratio, VA)}; one-to-one greedy
    over all pairs: ratio desc, then lower protoVA, then lower candidate VA (full ratio for every pair)."""
    pairs = []
    for key, pva, ptok in items:
        n = len(ptok)
        for toks, starts, cands in gap:
            for i in cands:
                r = difflib.SequenceMatcher(None, ptok, toks[i:i + n], autojunk=False).ratio()
                pairs.append((-r, pva, starts[i], key))
    pairs.sort(key=lambda p: p[:3])
    out, used = {}, set()
    for nr, _, cva, key in pairs:
        if key in out or cva in used:
            continue
        out[key] = (-nr, cva)
        used.add(cva)
    return out


def sum_check(cls_n, cls_b, exc_n, exc_b, all_n, all_b, nonex_n, nonex_b):
    """Class totals vs both denominators (counts and bytes) -> list of failed check names."""
    sn, sb = sum(cls_n.values()), sum(cls_b.values())
    checks = {"count/non-excluded": sn == nonex_n, "bytes/non-excluded": sb == nonex_b,
              "count/all": sn + sum(exc_n.values()) == all_n, "bytes/all": sb + sum(exc_b.values()) == all_b}
    return sorted(k for k, ok in checks.items() if not ok)


def pctl(vals, p):
    """Nearest-rank percentile of a list (None when empty)."""
    s = sorted(vals)
    return s[min(len(s) - 1, max(0, math.ceil(p / 100.0 * len(s)) - 1))] if s else None


def _pe_extra(data):
    """SizeOfImage and data dir 5 (reloc RVA, size) of a PE32."""
    opt = struct.unpack_from("<I", data, 0x3C)[0] + 24
    size_img = struct.unpack_from("<I", data, opt + 56)[0]
    ndd = struct.unpack_from("<I", data, opt + 92)[0]
    rva, sz = struct.unpack_from("<II", data, opt + 96 + 8 * 5) if ndd > 5 else (0, 0)
    return size_img, rva, sz


def _rva_off(pe, rva):
    for s in pe["sections"]:
        if s["va"] <= rva < s["va"] + s["rawsize"]:
            return s["rawptr"] + rva - s["va"]
    return None


def _check_manifest(path, cfg, records):
    sha1, _, size = manifest.hash_file(path)
    st, _ = xbe_info.manifest_status(path, sha1, size, cfg, records)
    if st != "match":
        _refuse(f"{path.name}: manifest {st} (sha1 {sha1} size {size})")
    return sha1


def _out_dir(target, pair="default"):
    d = (DERIVED / "proto_map" / target if pair == "default" else DERIVED / "proto_map" / pair / target).resolve()
    try:
        d.relative_to(DERIVED.resolve())
    except ValueError:
        _refuse(f"output dir {d} is outside derived/")
    return d


def qa_join(absent_rows, qa_rows):
    """Per default absent row (by protoVA): QA rows sharing any name, as (absent_row, [qa hit dicts]).

    Hit dict: names, class (excluded -> 'excluded (<reason>)'), detail (targetVA / similarity / hit count)."""
    by_name = {}
    for q in sorted(qa_rows, key=lambda r: int(r["protoVA"], 16)):
        for nm in q["names"]:
            by_name.setdefault(nm, []).append(q)
    out = []
    for a in sorted(absent_rows, key=lambda r: int(r["protoVA"], 16)):
        seen, hits = set(), []
        for nm in a["names"]:
            for q in by_name.get(nm, []):
                if q["protoVA"] in seen:
                    continue
                seen.add(q["protoVA"])
                cl = q["class"]
                det = None
                if cl == "excluded":
                    cl = f"excluded ({q['excludeReason']})"
                elif cl == "identical":
                    det = f"targetVA {q['targetVAs'][0]}"
                elif cl == "near-identical":
                    det = f"similarity {q['similarity']} candidateVA {q['candidateVA']}"
                elif cl == "ambiguous":
                    det = f"hits {q['hits']}"
                hits.append({"names": q["names"], "protoVA": q["protoVA"], "class": cl, "detail": det})
        hits.sort(key=lambda h: int(h["protoVA"], 16))
        out.append((a, hits))
    return out


def _write_qa_doc(qa_rows, tgt_sha1, exe_name):
    src = _out_dir("retail-xbe") / "absent.jsonl"
    if not src.is_file():
        _refuse(f"{src.relative_to(REPO).as_posix()} missing: run the default pair first")
    absent = [json.loads(ln) for ln in src.read_text(encoding="ascii").splitlines() if ln.strip()]
    if any(r["targetSha1"] != tgt_sha1 for r in absent):
        _refuse(f"{src.relative_to(REPO).as_posix()} targetSha1 differs from the QA run's {tgt_sha1}")
    joined = qa_join(absent, qa_rows)
    counts, multi = {}, 0
    for _, hits in joined:
        k = hits[0]["class"] if hits else "none"
        counts[k] = counts.get(k, 0) + 1
        multi += len(hits) > 1
    lines = ["# QA-build evidence only: never a pin, a candidate or a source name; default.pdb is the primary reference",
             "",
             f"Generated by `tools/proto_map.py --pair qa --target retail-xbe` ({exe_name} vs retail XBE sha1 "
             f"{tgt_sha1}) from derived/proto_map/retail-xbe/absent.jsonl; do not hand-edit.",
             "",
             f"Absent rows (default pair): {len(absent)}. By class of the first QA name-hit (QA protoVA order): "
             + ", ".join(f"{k} {counts[k]}" for k in sorted(counts)) + f". Rows with >1 QA hit: {multi}.",
             "",
             "Row: default protoVA, default name(s): QA result(s) (QA protoVA, QA name(s), class, detail).",
             ""]
    for a, hits in joined:
        res = "; ".join(f"{h['protoVA']} {', '.join(h['names'])} {h['class']}"
                        + (f" {h['detail']}" if h["detail"] else "") for h in hits) or "none"
        lines.append(f"- {a['protoVA']} {', '.join(a['names'])}: {res}")
    QA_DOC.write_text("\n".join(lines) + "\n", encoding="ascii", newline="\n")
    print(f"wrote {QA_DOC.relative_to(REPO).as_posix()} ({len(joined)} rows; "
          + ", ".join(f"{k} {counts[k]}" for k in sorted(counts)) + f"; >1 hit {multi})")


def run(target, pair="default"):
    cfg = vv_inputs.load_config()
    records = vv_inputs.load_manifest()
    exe_n, pdb_n = PAIRS[pair]
    exe_p = vv_inputs.input_path("prototype_dir", exe_n, cfg)
    pdb_p = vv_inputs.input_path("prototype_dir", pdb_n, cfg)
    role, name = TARGETS[target]
    tgt_p = vv_inputs.input_path(role, name, cfg)
    _check_manifest(exe_p, cfg, records)
    _check_manifest(pdb_p, cfg, records)
    tgt_sha1 = _check_manifest(tgt_p, cfg, records)
    exe, pdb_b, tgt = exe_p.read_bytes(), pdb_p.read_bytes(), tgt_p.read_bytes()
    try:
        pe = pe_info.parse_pe(exe)
        pdb = pdb_info.parse_pdb(pdb_b)
        x = xbe_info.parse_xbe(tgt)
    except (pe_info.NotPe, pdb_info.NotPdb, xbe_info.NotXbe) as e:
        _refuse(f"unparseable input: {e}")
    bad = pdb_info.validate(pdb, pe["rsds"])
    if bad:
        _refuse(f"{pdb_n} / {exe_n} pair invalid: {'; '.join(bad)}")
    out = _out_dir(target, pair)
    base = pe["imageBase"]
    size_img, rel_rva, rel_sz = _pe_extra(exe)
    lo, hi = base, base + size_img

    procs = {}
    for m in pdb["modules"]:
        for p in m["procs"]:
            if not 1 <= p["seg"] <= len(pe["sections"]):
                continue
            s = pe["sections"][p["seg"] - 1]
            va = base + s["va"] + p["off"]
            e = procs.setdefault(va, {"names": [], "modules": [], "length": 0, "sec": s, "off": p["off"]})
            if p["name"] not in e["names"]:
                e["names"].append(p["name"])
            if m["name"] not in e["modules"]:
                e["modules"].append(m["name"])
            e["length"] = max(e["length"], p["length"])
    secs = sorted((s["virtualAddr"], tgt[s["rawAddr"]:s["rawAddr"] + s["rawSize"]])
                  for s in x["sections"] if s["flags"] & SEC_EXEC)
    sec_bounds = [(v, v + len(b)) for v, b in secs]
    xsec = {s["name"]: s["virtualAddr"] for s in x["sections"]}  # imagebld rebases: control maps by name
    tlo = x["header"]["baseAddr"]
    thi = tlo + x["header"]["sizeOfImage"]

    relocs = set()
    if rel_rva and rel_sz:
        ro = _rva_off(pe, rel_rva)
        if ro is not None:
            relocs = parse_reloc_blocks(exe[ro:ro + rel_sz], base)
    rsorted = sorted(relocs)

    classes = ("identical", "ambiguous", "near-identical", "absent")
    reasons = ("zero-length", "no-raw-bytes", "data-in-text", "thunk")
    c = {"same_section_va": 0, "decode_errors_total": 0}
    abs_masks, ranges, rows, pending, ident_pairs = [], [], [], [], []
    for va in sorted(procs):
        e = procs[va]
        n, s = e["length"], e["sec"]
        row = {"names": e["names"], "modules": e["modules"], "protoVA": f"0x{va:08X}", "length": n,
               "masked": 0, "class": None, "excludeReason": None, "hits": 0, "targetVAs": [],
               "candidateVA": None, "similarity": None, "source": "proto_map", "targetSha1": tgt_sha1,
               "status_note": "unverified"}
        rows.append(row)
        if n == 0:
            row["excludeReason"] = "zero-length"
        elif e["off"] + n > s["rawsize"]:
            row["excludeReason"] = "no-raw-bytes"
        if row["excludeReason"]:
            row["class"] = "excluded"
            continue
        buf = exe[s["rawptr"] + e["off"]:s["rawptr"] + e["off"] + n]
        masks, errs = mask_proc(buf, va, lo, hi)
        if errs:
            row["class"], row["excludeReason"] = "excluded", "data-in-text"
            c["decode_errors_total"] += errs
            continue
        ranges.append((va, va + n))
        abs_masks += [va + o for o, _, k in masks if k == "abs"]
        if is_thunk(buf, va):
            row["class"], row["excludeReason"] = "excluded", "thunk"
            continue
        i0 = bisect.bisect_left(rsorted, va)
        i1 = bisect.bisect_left(rsorted, va + n)
        masks = sorted(set(masks) | {(r - va, 4, "abs") for r in rsorted[i0:i1]})
        row["masked"] = len(masks)
        segs = segments(n, masks)
        anc = anchor(segs)
        hits = find_matches(secs, buf, segs, anc, HIT_CAP) if anc else []
        row["hits"] = len(hits)
        row["targetVAs"] = [f"0x{h:08X}" for h in hits]
        if hits:
            row["class"] = "identical" if len(hits) == 1 else "ambiguous"
            if len(hits) == 1:
                c["same_section_va"] += s["name"] in xsec and xsec[s["name"]] + e["off"] == hits[0]
                ident_pairs.append((va, hits[0], n))
        else:
            pending.append((row, tokens(buf, va, lo, hi)[0]))

    anchors = lis_anchors(ident_pairs)
    aprotos = [a[0] for a in anchors]
    ngaps = len(anchors) + 1
    gap_cache = {}

    def gap(g):
        if g not in gap_cache:
            out = []
            for a, b in gap_ranges(anchors, g, sec_bounds):
                for base_va, hay in secs:
                    if base_va <= a < base_va + len(hay):
                        t, st, term, pad, _ = tokens(hay[a - base_va:b - base_va], a, tlo, thi)
                        out.append((t, st, candidate_starts(term, pad)))
                        break
            gap_cache[g] = out
        return gap_cache[g]

    def has_cand(g):
        return any(cands for _, _, cands in gap(g))

    by_gap = {}
    for k, (row, ptok) in enumerate(pending):
        g = bisect.bisect_left(aprotos, int(row["protoVA"], 16)) if anchors else None
        if g is None or not has_cand(g):
            row["noWindow"] = True
            continue
        by_gap.setdefault(g, []).append((k, int(row["protoVA"], 16), ptok))
    real, decoy, decoy_by_len, windowed = [], [], {"<16": [], "16-63": [], ">=64": []}, 0
    for g in sorted(by_gap):
        got = assign_gap(by_gap[g], gap(g))
        dg = next(h % ngaps for h in range(g + DECOY_SHIFT, g + DECOY_SHIFT + ngaps) if has_cand(h % ngaps))
        for k, _, ptok in by_gap[g]:
            windowed += 1
            row = pending[k][0]
            if k in got:
                r, cva = got[k]
                row["candidateVA"], row["similarity"] = f"0x{cva:08X}", round(100.0 * r, 2)
                real.append(100.0 * r)
            dr, _ = best_candidate(ptok, gap(dg))
            decoy.append(100.0 * dr)
            decoy_by_len["<16" if len(ptok) < 16 else "16-63" if len(ptok) < 64 else ">=64"].append(100.0 * dr)
    scored = [(row, row["similarity"]) for row, _ in pending]

    def classify(thr):
        return ["near-identical" if sim is not None and sim >= thr else "absent" for _, sim in scored]
    for (rw, _), cl in zip(scored, classify(THRESHOLD)):
        rw["class"] = cl

    total, bytes_total = len(rows), sum(r["length"] for r in rows)
    cls_n = {k: sum(r["class"] == k for r in rows) for k in classes}
    cls_b = {k: sum(r["length"] for r in rows if r["class"] == k) for k in classes}
    exc_n = {k: sum(r["excludeReason"] == k for r in rows) for k in reasons}
    exc_b = {k: sum(r["length"] for r in rows if r["excludeReason"] == k) for k in reasons}
    nonex = [r for r in rows if r["excludeReason"] is None]
    nonex_n, nonex_b = len(nonex), sum(r["length"] for r in nonex)
    bad = sum_check(cls_n, cls_b, exc_n, exc_b, total, bytes_total, nonex_n, nonex_b)
    sweep = {}
    for thr in (THRESHOLD - 5, THRESHOLD, THRESHOLD + 5):
        cl = classify(thr)
        sweep[thr] = {"near-identical": sum(v == "near-identical" for v in cl), "absent": sum(v == "absent" for v in cl)}
    cand_use = {}
    for r in rows:
        if r["class"] == "near-identical":
            cand_use[r["candidateVA"]] = cand_use.get(r["candidateVA"], 0) + 1
    shared = sorted(v for v, k in cand_use.items() if k > 1)
    shared_procs = sum(cand_use[v] for v in shared)
    dist = {nm: {f"p{p}": (round(pctl(v, p), 2) if v else None) for p in (50, 90, 99, 100)}
            for nm, v in (("real", real), ("decoy", decoy))}
    cuts = (70, 75, 80, 85, 90)
    exceed = {}
    for t in cuts:
        cl = classify(t)
        exceed[str(t)] = {"decoyGE": sum(d >= t for d in decoy), "near-identical": cl.count("near-identical"),
                          "absent": cl.count("absent")}
    decoy_p99 = {k: {"n": len(v), "p99": round(pctl(v, 99), 2) if v else None} for k, v in decoy_by_len.items()}

    reloc = None
    if relocs:
        nm, mn = reloc_crosscheck(relocs, abs_masks, ranges)
        reloc = {"sites": len(relocs), "absMasks": len(set(abs_masks)), "relocNotMasked": nm, "maskedNotReloc": mn}

    summ = {"target": target, "targetSha1": tgt_sha1, "source": "proto_map", "status_note": "unverified",
            "threshold": THRESHOLD, "procsTotal": total, "bytesTotal": bytes_total, "nonExcluded": nonex_n,
            "bytesNonExcluded": nonex_b, "classCounts": cls_n, "classBytes": cls_b, "excludedCounts": exc_n,
            "excludedBytes": exc_b, "anchors": len(anchors), "gaps": ngaps, "windowed": windowed,
            "noWindow": sum(1 for r in rows if r.get("noWindow")), "similarity": dist,
            "decoyExceedance": exceed, "decoyP99ByTokens": decoy_p99,
            "thresholdSweep": {str(k): v for k, v in sweep.items()}, "sharedCandidates": len(shared),
            "sharedCandidateProcs": shared_procs, "sameSectionVA": c["same_section_va"],
            "decodeErrorsTotal": c["decode_errors_total"], "execSections": len(secs), "reloc": reloc,
            "sumCheckFailed": bad}
    if pair != "default":
        summ.update({"pair": pair, "protoExe": exe_n, "protoPdb": pdb_n})
    out.mkdir(parents=True, exist_ok=True)
    with open(out / "functions.jsonl", "w", encoding="ascii", newline="\n") as f:
        for r in rows:
            f.write(json.dumps(r, ensure_ascii=True, sort_keys=True) + "\n")
    keep = ("names", "protoVA", "candidateVA", "similarity", "length", "source", "targetSha1", "status_note")
    for fn, cl in (("near_identical.jsonl", "near-identical"), ("absent.jsonl", "absent")):
        with open(out / fn, "w", encoding="ascii", newline="\n") as f:
            for r in rows:
                if r["class"] == cl:
                    f.write(json.dumps({k: r[k] for k in keep}, ensure_ascii=True, sort_keys=True) + "\n")
    (out / "summary.json").write_text(json.dumps(summ, indent=1, sort_keys=True) + "\n", encoding="ascii")

    def pct(a, b):
        return f"{a}/{b} ({100.0 * a / b:.2f}%)" if b else f"{a}/0"
    print(f"target {target} sha1 {tgt_sha1}; exec sections {len(secs)}; proto image [0x{lo:08X},0x{hi:08X}); "
          f"target image [0x{tlo:08X},0x{thi:08X}); threshold {THRESHOLD}")
    print(f"procs total {total} ({bytes_total} B); non-excluded {nonex_n} ({nonex_b} B)")
    for k in classes:
        print(f"  {k}: count {pct(cls_n[k], total)} of all, {pct(cls_n[k], nonex_n)} of non-excluded; "
              f"bytes {pct(cls_b[k], bytes_total)} of all, {pct(cls_b[k], nonex_b)} of non-excluded")
    print("excluded by reason: " + ", ".join(f"{k} {exc_n[k]} ({exc_b[k]} B)" for k in reasons)
          + f"; data-in-text invalid instrs {c['decode_errors_total']}")
    print(f"anchors (LIS of identical) {len(anchors)}/{cls_n['identical']}; gaps {ngaps}; windowed procs "
          f"{windowed} (assigned {len(real)}); noWindow {summ['noWindow']}")
    for nm in ("real", "decoy"):
        d = dist[nm]
        print(f"{'assigned' if nm == 'real' else 'best'} similarity {nm} "
              f"(n={len(real) if nm == 'real' else len(decoy)}): p50 {d['p50']} p90 {d['p90']} "
              f"p99 {d['p99']} max {d['p100']}")
    print("decoy >= t: " + "; ".join(f"{t}: {pct(v['decoyGE'], len(decoy))}" for t, v in exceed.items()))
    print("at t: " + "; ".join(f"{t}: near-identical {v['near-identical']}, absent {v['absent']}"
                               for t, v in exceed.items()))
    print("decoy p99 by proto tokens: " + "; ".join(f"{k}: {v['p99']} (n={v['n']})" for k, v in decoy_p99.items()))
    print("threshold sweep: " + "; ".join(f"{t}: near-identical {v['near-identical']}, absent {v['absent']}"
                                          for t, v in sweep.items()))
    print(f"candidates shared by >1 near-identical proc: {len(shared)} VAs ({shared_procs} procs)")
    print(f"identical at section-mapped VA (same-name XBE section + offset): "
          f"{pct(c['same_section_va'], cls_n['identical'])}")
    if reloc:
        print(f"reloc cross-check (decoded masks): sites {reloc['sites']}, abs masks {reloc['absMasks']}, "
              f"reloc-not-masked {reloc['relocNotMasked']}, masked-not-reloc {reloc['maskedNotReloc']}")
    else:
        print(f"reloc cross-check: no .reloc in {exe_n}")
    if bad:
        print(f"SUM CHECK FAILED: {', '.join(bad)}")
        return 1
    print("sum check: classes sum to both denominators (counts and bytes)")
    print(f"wrote {out.relative_to(REPO).as_posix()}/functions.jsonl ({len(rows)} rows), near_identical.jsonl "
          f"({cls_n['near-identical']}), absent.jsonl ({cls_n['absent']}), summary.json")
    if pair == "qa" and target == "retail-xbe":
        _write_qa_doc(rows, tgt_sha1, exe_n)
    return 0


def self_test():
    from iced_x86 import BlockEncoder, Instruction, MemoryOperand, Register
    lo, hi = 0x10000, 0x40000
    fails = []

    def chk(name, ok):
        print(f"{'ok' if ok else 'FAIL'}: {name}")
        if not ok:
            fails.append(name)

    def code(ip, callee, g, imm=7, op=Code.MOV_R32_RM32):
        ins = [Instruction.create_branch(Code.CALL_REL32_32, callee),
               Instruction.create_reg_mem(op, Register.EAX, MemoryOperand(displ=g, displ_size=4)),
               Instruction.create_mem_u32(Code.MOV_RM32_IMM32, MemoryOperand(displ=g + 4, displ_size=4), imm),
               Instruction.create_reg_u32(Code.MOV_R32_IMM32, Register.ECX, g + 8),
               Instruction.create_branch(Code.JE_REL32_32, ip + 0x400),
               Instruction.create(Code.RETND)]
        be = BlockEncoder(32)
        be.add_many(ins)
        return be.encode(ip)

    def same(a, segs, b):
        return len(a) == len(b) and all(a[s:e] == b[s:e] for s, e in segs)

    src = code(0x11000, 0x11800, 0x12340)
    masks, errs = mask_proc(src, 0x11000, lo, hi)
    chk("synthetic decodes with 0 errors", errs == 0)
    chk("masks: 2 rel + 3 abs (imm 7 out of range stays)", [k for _, _, k in masks].count("rel") == 2
        and [k for _, _, k in masks].count("abs") == 3)
    segs = segments(len(src), masks)
    moved = code(0x25000, 0x2A000, 0x31000)
    chk("moved call target + moved in-range global stay identical", same(src, segs, moved))
    chk("changed opcode differs", not same(src, segs, code(0x11000, 0x11800, 0x12340, op=Code.ADD_R32_RM32)))
    chk("changed out-of-range imm differs", not same(src, segs, code(0x11000, 0x11800, 0x12340, imm=8)))
    rng = random.Random(1)
    hay = bytearray(rng.getrandbits(8) for _ in range(1 << 16))
    hay[12345:12345 + len(moved)] = moved
    anc = anchor(segs)
    chk("matcher finds planted pattern", find_matches([(0x20000, bytes(hay))], src, segs, anc) == [0x20000 + 12345])
    hay[40000:40000 + len(moved)] = moved
    chk("matcher finds both plants", len(find_matches([(0x20000, bytes(hay))], src, segs, anc)) == 2)
    blob = struct.pack("<II4H", 0x1000, 16, 0x3006, 0x300A, 0x0000, 0x0000)
    rel = parse_reloc_blocks(blob, lo)
    chk("reloc blocks parse 2 HIGHLOW sites", rel == {lo + 0x1006, lo + 0x100A})
    chk("reloc cross-check counts 1/1",
        reloc_crosscheck(rel | {lo + 0x5000}, [lo + 0x1006, lo + 0x1020], [(lo + 0x1000, lo + 0x1100)]) == (1, 1))

    # T9: tokenizer, similarity, candidate starts, thunks, LIS anchors, class sums
    def enc(ip, ins):
        be = BlockEncoder(32)
        be.add_many(ins)
        return be.encode(ip)
    t_src = tokens(src, 0x11000, lo, hi)[0]
    chk("tokens: same code at moved VAs -> similarity 100", similarity(t_src, tokens(moved, 0x25000, lo, hi)[0]) == 100)
    chk("tokens: one changed opcode -> similarity < 100", similarity(
        t_src, tokens(code(0x11000, 0x11800, 0x12340, op=Code.ADD_R32_RM32), 0x11000, lo, hi)[0]) < 100)
    s1 = enc(0x11000, [Instruction.create_branch(Code.JE_REL8_32, 0x11010), Instruction.create(Code.RETND)])
    s2 = enc(0x11000, [Instruction.create_branch(Code.JE_REL8_32, 0x11020), Instruction.create(Code.RETND)])
    chk("tokens: short branch target wildcarded", s1 != s2 and tokens(s1, 0x11000, lo, hi)[0]
        == tokens(s2, 0x11000, lo, hi)[0])
    pad = enc(0x30000, [Instruction.create_reg_u32(Code.MOV_R32_IMM32, Register.EAX, 1), Instruction.create(Code.RETND),
                        Instruction.create(Code.INT3), Instruction.create(Code.INT3), Instruction.create(Code.NOPD),
                        Instruction.create_reg(Code.PUSH_R32, Register.EBP), Instruction.create(Code.RETND)])
    _, st, term, pd, _ = tokens(pad, 0x30000, lo, hi)
    cs = candidate_starts(term, pd)
    chk("candidate starts: 0 and first instr after ret+int3/nop padding", cs == [0, 5] and st[5] == 0x30000 + 9)
    hay2 = pad + moved
    t2, st2, term2, pd2, _ = tokens(hay2, 0x30000, lo, hi)
    r, cva = best_candidate(tokens(moved, 0x25000, lo, hi)[0], [(t2, st2, candidate_starts(term2, pd2))])
    chk("best candidate finds moved code after padding at 100", r == 1.0 and cva == 0x30000 + len(pad))
    ca = code(0x30000, 0x31800, 0x32340)
    cb = code(0x30000 + len(ca), 0x31800, 0x32340, imm=9, op=Code.ADD_R32_RM32)
    t3, st3, term3, pd3, _ = tokens(ca + cb, 0x30000, lo, hi)
    g3 = [(t3, st3, candidate_starts(term3, pd3))]
    p_hi = tokens(src, 0x11000, lo, hi)[0]
    p_lo = tokens(code(0x11000, 0x11800, 0x12340, imm=8), 0x11000, lo, hi)[0]
    got = assign_gap([("lo", 0x10000, p_lo), ("hi", 0x11000, p_hi)], g3)
    chk("assign: two procs get distinct candidates, higher similarity wins the contested one",
        best_candidate(p_lo, g3)[1] == 0x30000 and got["hi"] == (1.0, 0x30000)
        and got["lo"][1] == 0x30000 + len(ca))
    jr = enc(0x11000, [Instruction.create_branch(Code.JMP_REL32_32, 0x12000)])
    ji = enc(0x11000, [Instruction.create_mem(Code.JMP_RM32, MemoryOperand(displ=0x12000, displ_size=4))])
    jx = enc(0x11000, [Instruction.create_branch(Code.JMP_REL32_32, 0x12000), Instruction.create(Code.RETND)])
    chk("thunk: lone near jmp and lone indirect jmp yes; jmp+ret and ret no",
        is_thunk(jr, 0x11000) and is_thunk(ji, 0x11000) and not is_thunk(jx, 0x11000)
        and not is_thunk(enc(0x11000, [Instruction.create(Code.RETND)]), 0x11000))
    a = lis_anchors([(5, 5, 1), (1, 10, 1), (2, 30, 1), (3, 20, 1), (4, 40, 1)])
    chk("LIS anchors pick increasing chain 10,20,40", [p[0] for p in a] == [1, 3, 4])
    cn, cb = {"identical": 2, "absent": 1}, {"identical": 20, "absent": 5}
    en, eb = {"thunk": 1}, {"thunk": 5}
    chk("class sums: consistent -> no failure", sum_check(cn, cb, en, eb, 4, 30, 3, 25) == [])
    chk("class sums: off by one -> failure", sum_check(cn, cb, en, eb, 5, 30, 3, 26) == ["bytes/non-excluded",
                                                                                      "count/all"])

    # T11: QA join by name
    qa = [{"names": ["B", "Bq"], "protoVA": "0x00002000", "class": "identical", "excludeReason": None,
           "targetVAs": ["0x00031000"], "hits": 1, "similarity": None, "candidateVA": None},
          {"names": ["C"], "protoVA": "0x00001000", "class": "excluded", "excludeReason": "thunk",
           "targetVAs": [], "hits": 0, "similarity": None, "candidateVA": None}]
    j = qa_join([{"names": ["Z"], "protoVA": "0x00500000"}, {"names": ["A", "B"], "protoVA": "0x00400000"},
                 {"names": ["C"], "protoVA": "0x00600000"}], qa)
    chk("qa_join: name hit gives QA class and targetVA, rows sorted by protoVA",
        [a["protoVA"] for a, _ in j] == ["0x00400000", "0x00500000", "0x00600000"]
        and j[0][1] == [{"names": ["B", "Bq"], "protoVA": "0x00002000", "class": "identical",
                         "detail": "targetVA 0x00031000"}])
    chk("qa_join: no shared name -> no hit (none); excluded keeps its reason",
        j[1][1] == [] and j[2][1][0]["class"] == "excluded (thunk)")
    print(f"self-test: {'OK' if not fails else 'FAIL'} ({len(fails)} failed)")
    return 1 if fails else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--target", choices=sorted(TARGETS), default="retail-xbe")
    g.add_argument("--self-test", action="store_true")
    ap.add_argument("--pair", choices=sorted(PAIRS), default="default")
    a = ap.parse_args()
    return self_test() if a.self_test else run(a.target, a.pair)


if __name__ == "__main__":
    sys.exit(main())
