"""Census of unmatched proto-identical rows by method family, size, engine calls, name blockers (4.1 T9).

Usage: PY tools/family_census.py   (no args beyond --help; writes docs/matching/family_census.md)
Populations: pool = candidates.jsonl lane identical, not done; uncut = functions.jsonl class identical, one targetVA,
unit kind game, delink falsy (sites computed as candidates.py compute: same mask_proc, consts, resolve_target).
Inputs: derived/candidates/candidates.jsonl (PY tools/candidates.py --write), derived/proto_map/retail-xbe/functions.jsonl,
derived/dup_report/retail_folds.jsonl, derived/name_sites/residue.jsonl (opt: missing = all reasons `-`, one line says so),
config/matched_set.toml, config/retail/units.toml, symbols.txt, symbols_pin.txt, the retail XBE, default.exe, default.pdb.
Names, VAs, sizes, counts and shapes only (D46). Every family figure is rows and bytes against both denominators (D9, D59).
Exit 0 written; 1 a checked sum mismatches (FAIL line, nothing written); 2 refused (missing input, a rel site whose
opcode is not call, jmp or jcc).
"""
import argparse
import bisect
import hashlib
import json
import struct
import subprocess
import sys
import tomllib
from collections import Counter, defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))
import byte_compare  # noqa: E402
import candidates  # noqa: E402  setup helpers and EXPECT, read only
import name_sites  # noqa: E402
import pdb_info  # noqa: E402
import pe_info  # noqa: E402
import proto_map  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

CAND = candidates.OUT
FUNCS = candidates.FUNCS
FOLDS = candidates.FOLDS
RESIDUE = REPO / "derived" / "name_sites" / "residue.jsonl"
OUT = REPO / "docs" / "matching" / "family_census.md"
OUT_REL = "docs/matching/family_census.md"
ROADMAP = {"creationFunc": 236, "Message": 141, "FrameTick": 87, "AnimationFinished": 87, "Load": 71, "SetState": 67,
           "TimerEvent": 67, "Save": 48, "EnteringRegion": 45, "Update": 42, "Render": 42}  # GENERATION_PLAN.md:31
ROADMAP_UNCUT = 1628
BINS = ((0, 64, "<=64"), (65, 128, "65-128"), (129, 256, "129-256"), (257, 1 << 62, ">256"))
CLASSES = ("none", "site-corr", "near-corr", "boundary", "other")
SHAPE_MAX = 8
LEAF = "(leaf: no calls)"


def strip_args(s):
    """Text outside <...>."""
    out, depth = [], 0
    for c in s:
        if c == "<":
            depth += 1
        elif c == ">":
            depth = max(0, depth - 1)
        elif depth == 0:
            out.append(c)
    return "".join(out)


def split_name(n):
    """-> (class stem or "-", method): split at the last :: outside <>(); X::X <ctor>, X::~X <dtor>."""
    depth, cut, i = 0, -1, 0
    while i < len(n):
        c = n[i]
        if depth == 0 and n.startswith("operator", i) and (i == 0 or n[i - 1] == ":"):
            break  # operator<, operator() and conversion names never split further
        if c in "<(":
            depth += 1
        elif c in ">)":
            depth = max(0, depth - 1)
        elif depth == 0 and n.startswith("::", i):
            cut = i
            i += 1
        i += 1
    if cut < 0:
        return "-", strip_args(n) if not n.startswith("operator") else n
    stem, m = strip_args(n[:cut]), n[cut + 2:]
    if m.startswith("operator"):
        return stem, m
    m, last = strip_args(m), stem.rsplit("::", 1)[-1]
    return stem, "<ctor>" if m == last else "<dtor>" if m == "~" + last else m


def _skip_num(s, i):  # MSVC encoded number: one digit, or letters A-P ended by @
    if s[i] == "?":
        i += 1
    if s[i].isdigit():
        return i + 1
    return s.index("@", i) + 1


def _skip_arg(s, i):  # one template argument of a decorated name; anything else raises, the caller falls back
    c = s[i]
    if c.isdigit():
        return i + 1
    if c == "$":
        if s.startswith("$$C", i):  # cv-qualified type argument
            return _skip_arg(s, i + 4)
        if s[i + 1] == "0":
            return _skip_num(s, i + 2)
        raise ValueError(s)
    if c in "PQRSAB":
        if s[i + 1] == "6":  # function pointer: convention, return, arguments, Z
            i = _skip_arg(s, i + 3)
            if s[i] == "X":
                i += 1
            else:
                while s[i] != "@":
                    i = _skip_arg(s, i)
                i += 1
            if s[i] != "Z":
                raise ValueError(s)
            return i + 1
        return _skip_arg(s, i + 2)
    if c in "VUT":
        return _scope(s, i + 1, [])[1]
    if c == "W":
        return _scope(s, i + 2, [])[1]
    if c == "_":
        return i + 2
    if c == "?":
        return _skip_arg(s, i + 2)
    if c == "Y" or not c.isupper():
        raise ValueError(s)
    return i + 1


def _scope(s, i, memo):
    """Scope fragments of a decorated name from i to its @ -> (parts innermost first, next index)."""
    parts = []
    while s[i] != "@":
        if s.startswith("?$", i):
            j = s.index("@", i)
            name, i = s[i + 2:j], j + 1
            while s[i] != "@":
                i = _skip_arg(s, i)
            i += 1
            parts.append(name)
            memo.append(name)
        elif s[i].isdigit():
            parts.append(memo[int(s[i])] if int(s[i]) < len(memo) else "?")
            i += 1
        elif s[i] == "?":
            raise ValueError(s)
        else:
            j = s.index("@", i)
            parts.append(s[i:j])
            memo.append(s[i:j])
            i = j + 1
    return parts, i + 1


def label_of(name):
    """Callee name -> (label, class or None, method): never a VA."""
    if name.startswith("??2@"):
        return "operator new", None, "operator new"
    if name.startswith("??3@"):
        return "operator delete", None, "operator delete"
    if name.startswith("?"):
        try:
            if name.startswith(("??0", "??1")):
                m, parts = "<ctor>" if name[2] == "0" else "<dtor>", _scope(name, 3, [])[0]
            elif name.startswith("??"):
                return name.split("@@", 1)[0], None, None
            else:
                j = name.index("@", 1)
                m = name[1:j]
                parts = _scope(name, j + 1, [m])[0]
        except (ValueError, IndexError):
            return name.split("@@", 1)[0], None, None
        cls = "::".join(reversed(parts))
        return (f"{cls}::{m}", cls, m) if cls else (m, None, m)
    if "::" in name:
        cls, m = split_name(name)
        return name, cls, m
    return name, None, name


def sha12(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()[:12]


def pct(n, d):
    return f"{100 * n / d:.1f}%" if d else "-"


def code(s):
    s = s.replace("|", "\\|")
    return f"`` {s} ``" if "`" in s else f"`{s}`"


def size_of(rs):
    return sum(r["size"] for r in rs)


def split_pop(rs):
    return [r for r in rs if r["pop"] == "p"], [r for r in rs if r["pop"] == "u"]


def fmt_shape(s):
    if not s:
        return LEAF
    return " > ".join(s[:SHAPE_MAX]) + (f" > +{len(s) - SHAPE_MAX} more" if len(s) > SHAPE_MAX else "")


def table(head, rows):
    return ["| " + " | ".join(head) + " |", "|" + " --- |" * len(head)] + ["| " + " | ".join(map(str, r)) + " |" for r in rows]


def load():
    """Setup as candidates.py compute; -> a dict of everything the census reads."""
    _pin, _b, _t, _xp, xb, x = byte_compare.setup(argparse.Namespace(symbols=None, xbe=None, config=None))
    cfg = vv_inputs.load_config(None)
    pdb_p = vv_inputs.input_path("prototype_dir", "default.pdb", cfg)
    exe_p = vv_inputs.input_path("prototype_dir", "default.exe", cfg)
    funcs, data = candidates.load_symbols()
    try:
        exe = candidates.need(exe_p).read_bytes()
        pdb = candidates.need(pdb_p).read_bytes()
        nimg = name_sites.Images(exe, pe_info.parse_pe(exe), xb, x)
        data = candidates.member_extents(funcs, data, pdb, exe, nimg, None)
    except (pdb_info.NotPdb, pe_info.NotPe, struct.error, IndexError, TypeError) as e:
        raise candidates.Refuse(f"default.pdb/default.exe unparseable: {e}")
    units = sorted((u for u in tomllib.loads(candidates.need(candidates.UNITS).read_text(encoding="utf-8"))["unit"]
                    if "text_start" in u), key=lambda u: u["text_start"])
    return {"xb": xb, "x": x, "nimg": nimg, "funcs": funcs, "data": data, "dstarts": [d[0] for d in data], "units": units,
            "ustarts": [u["text_start"] for u in units],
            "lo": x["header"]["baseAddr"], "hi": x["header"]["baseAddr"] + x["header"]["sizeOfImage"],
            "cand": candidates.load_jsonl(candidates.need(CAND)), "protos": candidates.load_jsonl(candidates.need(FUNCS)),
            "folds": {int(r["retailVA"], 16) for r in candidates.load_jsonl(candidates.need(FOLDS))}}


def uncut_sites(env, va, size, proto):
    """Sites of an uncut row, exactly as candidates.py:392-409 for the identical lane."""
    off = xbe_info.va_to_off(env["x"], va)
    if off is None:
        raise candidates.Refuse(f"uncut row 0x{va:08X}: not in the retail XBE")
    buf = env["xb"][off:off + size]
    masks, _e = proto_map.mask_proc(buf, va, env["lo"], env["hi"])
    m = env["nimg"].match(proto, va, size)
    consts = set(m[0]) if m else set()
    sites = []
    for o, _n, k in masks:
        if k == "rel":
            t = (va + o + 4 + struct.unpack_from("<i", buf, o)[0]) & 0xFFFFFFFF
            if va <= t < va + size:
                continue
        else:
            t = struct.unpack_from("<I", buf, o)[0]
        st, _mem = ("constant", None) if k == "abs" and o in consts else \
            candidates.resolve_target(t, va, size, env["funcs"], env["dstarts"], env["data"])
        sites.append({"off": o, "kind": k, "target": t, "resolved": st})
    return sites


def site_kind(env, va, s):
    """call, jmp, jcc or abs from the retail opcode before a rel field; anything else refuses."""
    if s["kind"] != "rel":
        return "abs"
    o = xbe_info.va_to_off(env["x"], va + s["off"] - 1)
    b = env["xb"][o] if o is not None else None
    if b == 0xE8:
        return "call"
    if b == 0xE9:
        return "jmp"
    if b is not None and 0x80 <= b <= 0x8F and env["xb"][o - 1] == 0x0F:
        return "jcc"
    raise candidates.Refuse(f"rel site at 0x{va + s['off']:08X} of row 0x{va:08X}: not a call, jmp or jcc")


def make_row(env, pop, va, size, names, sites):
    names = sorted(names)
    stem, method = split_name(names[0])
    methods = {split_name(n)[1] for n in names}
    labels, raw, blockers, kinds = {}, [], [], Counter()
    for s in sites:
        k = site_kind(env, va, s)
        kinds[k] += 1
        st = s["resolved"]
        if not candidates.accepted(st, s["target"], env["funcs"]):  # 4.2 T9: candidates.py's one rule (D115)
            blockers.append((s["target"], k, st))
        if k == "call":
            f = env["funcs"].get(s["target"])
            raw.append(labels.setdefault(s["target"], label_of(f["name"])) if f else ("?", None, None))
    shape = tuple(f"self::{m}" if cls is not None and cls == stem else lab for lab, cls, m in raw)
    called = {lab for lab, cls, _m in raw if lab != "?" and not (cls is not None and cls == stem)}
    return {"pop": pop, "va": va, "size": size, "stem": stem, "method": method, "aliases": methods - {method},
            "shape": shape, "called": called, "blockers": blockers, "kinds": kinds}


def populations(env):
    """-> (rows, sums dict): the census rows and the class identical partition, tests in candidates.py EXCL order."""
    pool = [r for r in env["cand"] if r["lane"] == "identical"]
    n = Counter()
    rows = []
    for r in env["protos"]:
        if r["class"] != "identical":
            continue
        tv = [int(v, 16) for v in r["targetVAs"]]
        if len(tv) != 1:
            n["targetVAs != 1"] += 1
            continue
        va = tv[0]
        i = bisect.bisect_right(env["ustarts"], va) - 1
        u = env["units"][i] if i >= 0 and env["units"][i]["text_start"] <= va < env["units"][i]["text_end"] else None
        pv = int(r["protoVA"], 16) - candidates.PROTO_BASE
        if u is None:
            n["no unit"] += 1
        elif u["kind"] != "game":
            n["not game"] += 1
        elif not u["delink"]:
            n["uncut"] += 1
            rows.append(make_row(env, "u", va, r["length"], r["names"],
                                 uncut_sites(env, va, r["length"], int(r["protoVA"], 16))))
        elif candidates.TAIL[0] <= pv < candidates.TAIL[1]:
            n["tail"] += 1
        elif va in env["folds"]:
            n["fold"] += 1
        elif va not in env["funcs"]:
            n["no symbol"] += 1
        elif candidates.BOUNDARY.match(env["funcs"][va]["name"]):
            n["boundary"] += 1
        else:
            n["pool partition"] += 1
    for r in pool:
        if not r["done"]:
            rows.append(make_row(env, "p", r["va"], r["size"], r["names"], r["sites"]))
    n["identical"] = sum(1 for r in env["protos"] if r["class"] == "identical")
    n["pool"], n["matched"] = len(pool), sum(1 for r in pool if r["done"])
    n["unmatched"] = n["pool"] - n["matched"]
    n["delinked-not-pool"] = n["tail"] + n["fold"] + n["no symbol"] + n["boundary"]
    rows.sort(key=lambda r: (r["va"], r["pop"]))
    return rows, n


def check_sums(n, rows, bins_total, class_total):
    """-> failure messages (empty when every checked sum holds)."""
    e, bad = candidates.EXPECT, []
    tot = n["no unit"] + n["not game"] + n["uncut"] + n["delinked-not-pool"] + n["pool"] + n["targetVAs != 1"]
    if n["identical"] != tot:
        bad.append(f"class identical {n['identical']} != partition {tot}")
    if n["pool partition"] != n["pool"]:
        bad.append(f"candidates.jsonl identical rows {n['pool']} != partition {n['pool partition']}")
    if n["uncut"] != e["unit not delink"]:
        bad.append(f"uncut {n['uncut']} != EXPECT unit not delink {e['unit not delink']}")
    want = e["fold"] + e["no symbol"] + e["boundary"] + e["tail"]
    if n["delinked-not-pool"] != want:
        bad.append(f"delinked-not-pool {n['delinked-not-pool']} != EXPECT {want}")
    pop = n["unmatched"] + n["uncut"]
    if len(rows) != pop:
        bad.append(f"census rows {len(rows)} != population {pop}")
    if bins_total != pop:
        bad.append(f"size bins total {bins_total} != population {pop}")
    if class_total != pop:
        bad.append(f"status class table total {class_total} != population {pop}")
    return bad


def blocker_class(st):
    return "none" if st is None else st if st in CLASSES else "other"


def render(env, rows, n):
    """-> (doc lines, check totals, stdout tally line)."""
    funcs, data, dstarts = env["funcs"], env["data"], env["dstarts"]
    pr, ur = split_pop(rows)
    dr, db = len(rows), size_of(rows)

    def cells(rs):
        p, u = split_pop(rs)
        return [len(p), len(u), len(rs), pct(len(rs), dr), size_of(p), size_of(u), size_of(rs), pct(size_of(rs), db)]

    col = ["rows p", "rows u", "rows", "row %", "bytes p", "bytes u", "bytes", "byte %"]

    def order(groups):  # count desc, bytes desc, then key asc (total: keys are unique)
        return sorted(groups.items(), key=lambda kv: (-len(kv[1]), -size_of(kv[1]), kv[0]))

    def by(key, rs=rows):
        g = defaultdict(list)
        for r in rs:
            g[key(r)].append(r)
        return g

    def largest_size(rs):
        s, c = min(Counter(r["size"] for r in rs).items(), key=lambda kv: (-kv[1], kv[0]))
        return s, c

    tally = Counter()
    for r in rows:
        for k, v in r["kinds"].items():
            tally[(r["pop"], k)] += v
    sites_t = sum(tally.values())

    def tal(k):
        p, u = tally[("p", k)], tally[("u", k)]
        return f"{k} {p + u} ({p} pool, {u} uncut, {pct(p + u, sites_t)})"

    tally_line = (f"Site tally: {tal('call')}, {tal('jmp')}, {tal('jcc')}, {tal('abs')}; denominator {sites_t} sites "
                  f"in {dr} rows (rel sites told by the retail opcode before the field).")
    sums_line = (f"Sums (checked): class identical {n['identical']} = no unit {n['no unit']} + not game {n['not game']} + "
                 f"uncut {n['uncut']} + delinked-not-pool {n['delinked-not-pool']} + pool rows {n['pool']} "
                 f"(matched {n['matched']} + unmatched {n['unmatched']}); uncut {n['uncut']} == EXPECT unit not delink; "
                 f"delinked-not-pool == EXPECT fold {n['fold']} + no symbol {n['no symbol']} + boundary {n['boundary']} "
                 f"+ tail {n['tail']}; size bins and status class table total == population {dr}.")
    matched_rows = len(tomllib.loads(candidates.need(candidates.MATCHED).read_text(encoding="utf-8")).get("function", []))
    res = subprocess.run(["git", "log", "-1", "--format=%cs", "--", "config/matched_set.toml"], cwd=REPO,
                         capture_output=True, text=True)
    if res.returncode != 0 or not res.stdout.strip():
        raise candidates.Refuse(f"git log of config/matched_set.toml: rc {res.returncode}")
    has_res = RESIDUE.is_file()
    inputs = [(CAND, f"{len(env['cand'])} rows, from `PY tools/candidates.py --write`"),
              (FUNCS, f"{len(env['protos'])} rows"), (FOLDS, f"{len(env['folds'])} fold VAs"),
              (candidates.MATCHED, f"{matched_rows} matched rows, identical lane {n['matched']}"),
              (candidates.UNITS, "unit kind, delink"), (candidates.SYMS, "callee names, status"),
              (candidates.SYMS_PIN, "callee names, status")]
    if has_res:
        inputs.append((RESIDUE, "residue reasons"))
    L = ["# Family census: unmatched proto-identical rows by family, size, engine calls, name blockers", "",
         "Generated by `PY tools/family_census.py` (4.1 T9); do not hand-edit, re-run it. Inputs (sha256[:12]):", ""]
    L += [f"- `{p.relative_to(REPO).as_posix()}` {sha12(p)} ({why})" for p, why in inputs]
    L += ["", f"As of {res.stdout.strip()} (last commit of `config/matched_set.toml`); matched {n['matched']} of the "
          f"{n['pool']} identical pool rows.",
          "Claim type: row and byte counts of proto-identical rows, none claimed matched.",
          f"Denominators: rows {dr} = {n['unmatched']} unmatched pool + {n['uncut']} uncut; bytes {db} = "
          f"{size_of(pr)} pool + {size_of(ur)} uncut. p = pool, u = uncut. Sorts: count desc, bytes desc, name asc.",
          "", sums_line, "", tally_line, ""]

    # 2. size bins
    binrows, bins_total = [], 0
    for lo, hi, name in BINS:
        rs = [r for r in rows if lo <= r["size"] <= hi]
        bins_total += len(rs)
        binrows.append([name] + cells(rs))
    L += ["## Size bins", "", "Every population row in one bin by its size in bytes.", ""]
    L += table(["size"] + col, binrows + [["total"] + cells(rows)]) + [""]

    # 3. method families
    fam = by(lambda r: r["method"])
    alias = Counter(m for r in rows for m in r["aliases"])
    frows = []
    for m, rs in order(fam)[:40]:
        s, c = largest_size(rs)
        shapes = Counter(r["shape"] for r in rs)
        frows.append([code(m)] + cells(rs) + [alias[m], len({r["stem"] for r in rs}), len({r["size"] for r in rs}),
                                              f"{s} x {c}", len(shapes), max(shapes.values())])
    L += ["## Method families across classes", "",
          f"Top 40 of {len(fam)} methods by total rows; family = method of the sorted first name. `+alias` = rows whose "
          "primary differs but another name has this method, never added to sums.", ""]
    L += table(["method"] + col + ["+alias", "class stems", "sizes", "largest same-size", "shapes", "largest same-shape"],
               frows) + [""]

    # 4. same method, same size
    ms = {k: v for k, v in by(lambda r: (r["method"], r["size"])).items() if len(v) >= 4}
    L += ["## Same method, same size", "", f"(method, size) groups of >= 4 rows, top 40 of {len(ms)}.", ""]
    L += table(["method", "size"] + col, [[code(k[0]), k[1]] + cells(v) for k, v in order(ms)[:40]]) + [""]

    # 5. by engine calls
    shp = by(lambda r: r["shape"])
    leaf = shp.get((), [])

    def shape_row(k, v):
        mc = by(lambda r: r["method"], v)
        top, tr = order(mc)[0]
        return [code(fmt_shape(k))] + cells(v) + [len(mc), f"{code(top)} {len(tr)}",
                                                  f"{min(r['size'] for r in v)}-{max(r['size'] for r in v)}"]

    big = {k: v for k, v in shp.items() if k and len(v) >= 4}
    srows = [shape_row((), leaf)] if leaf else []
    srows += [shape_row(k, v) for k, v in sorted(big.items(), key=lambda kv: (-len(kv[1]), -size_of(kv[1]),
                                                                              " > ".join(kv[0])))[:40]]
    L += ["## By engine calls", "",
          f"Shape = ordered callees over call sites (`self::` own class, `?` no func line). First line is the leaf shape; then "
          f"top 40 of {len(big)} shapes of >= 4 rows, {len(shp)} distinct shapes in all.", ""]
    L += table(["shape"] + col + ["methods", "top method", "size range"], srows) + [""]

    # 6. most-called
    callee = defaultdict(list)
    for r in rows:
        for c in r["called"]:
            callee[c].append(r)
    L += ["## Most-called functions", "",
          f"Named callees by rows calling them, top 30 of {len(callee)}; class-agnostic, a row's own-class calls excluded.", ""]
    L += table(["callee"] + col, [[code(k)] + cells(v) for k, v in order(callee)[:30]]) + [""]

    # 7. template stems
    std = by(lambda r: r["stem"], [r for r in rows if r["stem"] == "std" or r["stem"].startswith("std::")])  # stem std: namespace-level std::f
    std_all = [r for v in std.values() for r in v]
    p, u = split_pop(std_all)
    std_line = (f"Strict template stems (class stem `std::`): {len(std_all)} rows ({len(p)} pool, {len(u)} uncut, "
                f"{pct(len(std_all), dr)}), {size_of(std_all)} bytes ({pct(size_of(std_all), db)}).")
    L += ["## Template stems", "", f"`std::` class stems by rows, top 20 of {len(std)}.", "", std_line, ""]
    L += table(["class stem"] + col + ["methods"],
               [[code(k)] + cells(v) + [len({r["method"] for r in v})] for k, v in order(std)[:20]]) + [""]

    # 8. 4.2 levers
    groups = {c: [] for c in CLASSES + ("mixed", "not blocked")}
    for r in rows:
        cl = {blocker_class(st) for _t, _k, st in r["blockers"]}
        groups["not blocked" if not cl else cl.pop() if len(cl) == 1 else "mixed"].append(r)
    class_total = sum(len(v) for v in groups.values())

    def lever_key(rs, tie):
        p, u = split_pop(rs)
        return (-len(p), -size_of(p), -len(u), tie)

    ordered = sorted(CLASSES, key=lambda c: lever_key(groups[c], c))
    L += ["## 4.2 levers, ordered", "",
          "Blockers = sites whose resolved is none or a status outside the accepted ones (self, constant never block; "
          "as candidates.accepted: near-corr on a func-table target accepted, D115). By status class: rows whose every blocker is that class, ordered pool rows freed, "
          "pool bytes, uncut rows, class; then mixed and not blocked.", ""]
    L += table(["status class"] + col, [[c] + cells(groups[c]) for c in ordered + ["mixed", "not blocked"]]) + [""]
    reasons = defaultdict(set)
    if has_res:
        for r in candidates.load_jsonl(RESIDUE):
            reasons[int(r["target"], 16)].add(r["reason"])
    else:
        L += ["`derived/name_sites/residue.jsonl` is missing: every reason below is `-`.", ""]
    single = defaultdict(list)
    for r in rows:
        ts = {t for t, _k, _s in r["blockers"]}
        if len(ts) == 1:
            single[next(iter(ts))].append(r)
    top_single = sorted(single.items(), key=lambda kv: lever_key(kv[1], kv[0]))[:25]

    def target_name(t):
        if t in funcs:
            return code(funcs[t]["name"])
        i = bisect.bisect_right(dstarts, t) - 1
        return code(data[i][3]) if i >= 0 and data[i][0] <= t < data[i][1] else "-"

    trows = []
    for t, rs in top_single:
        p, u = split_pop(rs)
        ks = sorted({k for r in rs for bt, k, _s in r["blockers"] if bt == t}, key=("call", "jmp", "jcc", "abs").index)
        st = next(s for bt, _k, s in rs[0]["blockers"] if bt == t)
        top = order(by(lambda r: r["method"], rs))[0][0]
        trows.append([f"0x{t:08X}", "/".join(ks), st if st else "none", target_name(t),
                      ",".join(sorted(reasons[t])) if t in reasons else "-", len(p), len(u), size_of(p), code(top)])
    L += [f"Single-target: rows whose one distinct blocker target is T, top 25 of {len(single)}, same order, tie VA asc.", ""]
    L += table(["target", "site kind", "status", "name", "residue reason", "rows p", "rows u", "bytes p", "top family"],
               trows) + [""]
    L += ["Order for 4.2 (status classes then single targets; single targets are subsets of the classes, never added):", ""]
    k = 0
    for c in ordered:
        p, _u = split_pop(groups[c])
        if groups[c]:
            k += 1
            L.append(f"{k}. class {c}: {len(p)} pool rows, {size_of(p)} pool bytes")
    for t, rs in top_single:
        p, _u = split_pop(rs)
        k += 1
        L.append(f"{k}. target 0x{t:08X}: {len(p)} pool rows, {size_of(p)} pool bytes")
    blocked = [r for r in rows if r["blockers"]]
    bp, bu = split_pop(blocked)
    fb = by(lambda r: r["method"], bp)
    L += ["", f"Rows blocked: {len(bp)} of {len(pr)} pool, {len(bu)} of {len(ur)} uncut. Top 15 methods by pool rows "
          "name-blocked:", ""]
    uncut_b = Counter(r["method"] for r in bu)
    L += table(["method", "pool rows blocked", "pool bytes blocked", "uncut rows blocked"],
               [[code(m), len(v), size_of(v), uncut_b[m]] for m, v in order(fb)[:15]]) + [""]

    # 9. 4.3 offers
    off = {k: v for k, v in by(lambda r: (r["method"], r["shape"])).items() if len(v) >= 8}
    orows = []
    for (m, s), v in sorted(off.items(), key=lambda kv: (-len(kv[1]), -size_of(kv[1]), kv[0][0], " > ".join(kv[0][1])))[:15]:
        _s, c = largest_size(v)
        p, u = split_pop(v)
        note = "`tools/gen_creationfunc.py` exists (3.3; C0135, C0140); rows outside the script form assert (C0135)" \
            if m == "creationFunc" else "-"
        orows.append([code(m), code(fmt_shape(s)), len(p), len(u),
                      f"{min(r['size'] for r in v)}-{max(r['size'] for r in v)}", c, pct(len(v), len(fam[m])),
                      sum(1 for r in p if r["blockers"]), len({r["stem"] for r in v}), note])
    L += ["## 4.3 generator offers", "", f"(method, shape) groups of >= 8 rows, top 15 of {len(off)}: rows desc, bytes desc, "
          "method, shape.", ""]
    L += table(["method", "shape", "rows p", "rows u", "sizes", "largest same-size", "family share", "pool name-blocked",
                "classes", "note"], orows) + [""]
    L += [f"Stdlib stems: {len(std_all)} rows ({len(split_pop(std_all)[0])} pool), {size_of(std_all)} bytes in "
          f"{len(std)} `std::` class stems; tried on one unit before claimed (C0096, C0090, C0092).", "",
          "Offers only; no generator built in 4.1; 4.3 picks, one-unit proof first.", ""]

    # 10. roadmap check
    L += ["## Roadmap check", "", "GENERATION_PLAN.md:31 figures vs this census; delta = primary rows minus roadmap.", ""]
    L += table(["method", "roadmap", "primary rows", "primary+alias", "delta"],
               [[code(m), v, len(fam.get(m, [])), len(fam.get(m, [])) + alias[m], len(fam.get(m, [])) - v]
                for m, v in ROADMAP.items()]) + [""]
    L += [f"Uncut: roadmap {ROADMAP_UNCUT}, tool {n['uncut']}, delta {n['uncut'] - ROADMAP_UNCUT}.", ""]
    return L, bins_total, class_total, sums_line, tally_line


def run():
    env = load()
    rows, n = populations(env)
    L, bins_total, class_total, sums_line, tally_line = render(env, rows, n)
    bad = check_sums(n, rows, bins_total, class_total)
    if bad:
        for b in bad:
            print(f"FAIL: {b}")
        return 1
    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text("\n".join(L).rstrip("\n") + "\n", encoding="utf-8", newline="\n")
    print(sums_line)
    print(tally_line)
    print(f"family_census: wrote {OUT_REL}: {len(rows)} rows ({n['unmatched']} pool, {n['uncut']} uncut)")
    return 0


def main(argv):
    argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter).parse_args(argv)
    try:
        return run()
    except (candidates.Refuse, byte_compare.Refuse, OSError) as e:
        print(f"family_census: REFUSED {e}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
