"""Matched-set list (1.5 T3): add entries to a matched-set TOML list and check them against built objects + retail XBE.

CLI: matched_set.py add --list P --symbol S
     matched_set.py add --list P --emitter S --unit U --reason TEXT
     matched_set.py check --list P --src DIR --objs DIR [--config P]
List entries: `[[function]] symbol, unit, va (int), size (int), xbe_sha1, body_sha1` (body_sha1 = sha1 of the retail
XBE bytes va..va+size; xbe_sha1 = config/check.retail_xbe.sha pin) and `[[emitter]] symbol, unit, reason` (a
scaffold symbol with no retail counterpart that its unit's obj may define). Hashes only, never bytes (D46).
add: va+size from config/retail/symbols.txt (exact name, func, unique, size set); unit = the one units.toml unit with
text_start <= va < text_end (edge_start/edge_end refuse only when both are ints; today they are edge classes);
appends to the file text (header kept). Refuses an XBE whose sha1 differs from the pin, duplicates, missing file.
3.4 T6.1: a name with no symbols.txt row falls back (add and check, _func_row) to config/retail/symbols_pin.txt:
exactly one sized `func` pin line, status disasm-verified with another func name at its VA (a fold alias, C0130) or
whose pin row passes tools/pin_rows.py (D118, 4.2 T5), or near-corr; else refused with the reason. add of a near-corr pin also needs a `"lane": "near"` row of the symbol at
that VA in derived/candidates/candidates.jsonl (add-time only; check never reads it). One row per retail VA.
check: per function `xbe_sha1`, `size`, `body_sha1`, `defined`, `asm`; global `"*objects*"` -> `extra` (rules
eh-funclet, local-label, inline-copy; 5.1 T2 D124: an `[[emitter]]` symbol with a hand-written definition in its
unit's source (brace depth 0, `{` before `;`) that is not a marked scaffold (`pin_emit_*` under `//` lines with
`!FAKE:` and `scaffold`) fails `[[emitter]] <symbol> (<obj>) has a hand-written body at <source>:<line>`; a symbol
with no hand spelling or no definition passes). `asm`: one preprocess per unique (source, flags), sorted, on WORKERS
threads, results by key (5.1 T2 O18; was: serial). Every layer reported (D51). `built` reconstructed from --src + --objs with
build.unit_map/unit_path/flags_of (no rebuild; obj = objs/<objname basename>).
Exit (D26): 0 all pass and N >= 1; 1 any fail; 2 refusal (stderr `matched_set: REFUSED <why>`).
Library: load(list_path) -> entries (dicts with `kind` "function"|"emitter"); check(entries, built, src_root) ->
{symbol: {check: verdict}, "*objects*": {"extra": verdict}}; REQUIRED; is_done(row); Refuse.
"""
import argparse
import concurrent.futures
import hashlib
import json
import re
import shutil
import struct
import sys
import tempfile
import tomllib
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import build  # noqa: E402
import byte_compare  # noqa: E402  4.5 T16.2.c4: _phase_run_dir (no cycle: it imports neither)
import coff_info  # noqa: E402
import msvc71  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

REPO = vv_inputs.REPO
SYMBOLS = REPO / "config" / "retail" / "symbols.txt"
PINS = REPO / "config" / "retail" / "symbols_pin.txt"
CANDIDATES = REPO / "derived" / "candidates" / "candidates.jsonl"
UNITS = build.UNITS
SCRATCH = byte_compare._phase_run_dir()  # 4.5 T16.2.c4 (G15); was: REPO / ".run" / "1.5"
REQUIRED = ("xbe_sha1", "size", "body_sha1", "defined", "asm")
OBJECTS = "*objects*"
SCN_CNT_CODE, SCN_LNK_COMDAT, SEL_ANY = 0x20, 0x1000, 2
FIELDS = {"function": {"symbol": str, "unit": str, "va": int, "size": int, "xbe_sha1": str, "body_sha1": str},
          "emitter": {"symbol": str, "unit": str, "reason": str}}
HEX40 = re.compile(r"^[0-9a-f]{40}$")
ASM = re.compile(r"(?<![A-Za-z0-9_])_?_asm(?![A-Za-z0-9_])")
LITERAL = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'')
FUNCLET = re.compile(r"^(?:__ehhandler\$(.+)|__(?:unwindfunclet|catch|tryend)\$(.+)\$\d+)$")
LOCAL_LABEL = re.compile(r"^\$L\d+$")
WORKERS = 8  # 5.1 T2 (O18): asm preprocess threads; a fixed constant, never tuned per run
SCAFFOLD_PREFIX = "pin_emit_"  # 5.1 T2 (D124): a marked scaffold's name prefix
IDENT = re.compile(r"[A-Za-z_]\w*")


class Refuse(Exception):
    pass


def is_done(row):
    """D28: the single done function. True iff every REQUIRED check is present and every verdict is "pass"."""
    v = row.get("verdicts", {})
    return all(k in v for k in REQUIRED) and all(x == "pass" for x in v.values())


def _parse(list_path):
    p = Path(list_path)
    if not p.is_file():
        raise Refuse(f"missing list {p}")
    try:
        doc = tomllib.loads(p.read_text(encoding="utf-8"))
    except tomllib.TOMLDecodeError as e:
        raise Refuse(f"list {p} is not TOML: {e}")
    entries, syms, vas = [], set(), set()
    for kind, fields in FIELDS.items():
        for i, e in enumerate(doc.get(kind, [])):
            for f, t in fields.items():
                if f not in e:
                    raise Refuse(f"{kind} {i + 1}: missing field {f}")
                if not isinstance(e[f], t) or isinstance(e[f], bool):
                    raise Refuse(f"{kind} {i + 1}: field {f} is not {t.__name__}")
            if kind == "function" and not (HEX40.match(e["xbe_sha1"]) and HEX40.match(e["body_sha1"])):
                raise Refuse(f"function {e['symbol']}: xbe_sha1/body_sha1 not 40 lowercase hex")
            if e["symbol"] in syms:
                raise Refuse(f"duplicate symbol {e['symbol']}")
            syms.add(e["symbol"])
            if kind == "function":
                if e["va"] in vas:
                    raise Refuse(f"duplicate va 0x{e['va']:08X}")
                vas.add(e["va"])
            entries.append({"kind": kind, **{f: e[f] for f in fields}})
    return entries


def load(list_path):
    """-> entries (functions then emitters; each has `kind`). Refuse: missing file, zero functions, dup, bad field."""
    entries = _parse(list_path)
    if not any(e["kind"] == "function" for e in entries):
        raise Refuse(f"list {list_path} has no [[function]] entry")
    return entries


def _symbols():
    rows = {}
    for line in SYMBOLS.read_text(encoding="utf-8").splitlines():
        p = line.split()
        if not p or p[0].startswith("#") or len(p) < 6:
            continue
        rows.setdefault(" ".join(p[3:-2]), []).append({"va": int(p[0], 16), "kind": p[1],
                                                         "size": None if p[2] == "-" else int(p[2], 16)})
    return rows


def _pins():
    """-> {name: [{va, kind, size, status}]} of symbols_pin.txt (format as symbols.txt; status = last field)."""
    rows = {}
    for line in PINS.read_text(encoding="utf-8").splitlines():
        p = line.split()
        if not p or p[0].startswith("#") or len(p) < 6:
            continue
        rows.setdefault(" ".join(p[3:-2]), []).append({"va": int(p[0], 16), "kind": p[1], "status": p[-1],
                                                         "size": None if p[2] in ("-", "0") else int(p[2], 16)})
    return rows


def _pin_row(rows, pins, symbol, cfg=None):
    """3.4 T6.1: -> (va, size, status) of `symbol`'s one sized func pin line, status near-corr, or disasm-verified with
    another func name at the same VA in symbols.txt or the pins (a fold alias, C0130), or (D118, 4.2 T5) disasm-verified
    sole name whose pin row passes the pin-row check (pin_rows.row_ok is True, under cfg: the gate's config, 4.5
    T16.2.c4); else a reason string."""
    fs = [r for r in pins.get(symbol, []) if r["kind"] == "func"]
    if not fs:
        return f"{symbol} not in symbols.txt or symbols_pin.txt"
    if len(fs) != 1:
        return f"{symbol} has {len(fs)} symbols_pin.txt func lines"
    r = fs[0]
    if r["status"] not in ("disasm-verified", "near-corr"):
        return f"{symbol} pin status {r['status']} (need disasm-verified fold alias or near-corr)"
    if r["size"] is None:
        return f"{symbol} pin has no size in symbols_pin.txt"
    if r["status"] == "disasm-verified" and not any(
            n != symbol and any(x["kind"] == "func" and x["va"] == r["va"] for x in xs)
            for tab in (rows, pins) for n, xs in tab.items()):
        import pin_rows  # noqa: PLC0415  D118 (4.2 T5): lazy, pin_rows imports this module
        ok = pin_rows.row_ok(symbol, cfg)  # 4.5 T16.2.c4 (G15); was: row_ok(symbol), the default config
        if ok is not True:  # D118: the pin-row check (D119) guards the sole name; an unruled pin stays refused
            return (f"{symbol} pin disasm-verified with no other func name at 0x{r['va']:08X}: not a fold alias, "
                    f"pin-row check: {ok}")
    return r["va"], r["size"], r["status"]


def _func_row(rows, symbol, pins=None, cfg=None):
    """-> (va, size, status) of the unique func row of `symbol` (status "symbols.txt" for a symbols.txt row; else
    the symbols_pin.txt fallback, _pin_row), or a reason string."""
    rs = rows.get(symbol, [])
    fs = [r for r in rs if r["kind"] == "func"]
    if not rs:
        return _pin_row(rows, _pins() if pins is None else pins, symbol, cfg)
    if len(rs) != 1 or len(fs) != 1:
        return f"{symbol} has {len(rs)} symbols.txt rows ({len(fs)} func)"
    if fs[0]["size"] is None:
        return f"{symbol} has no size in symbols.txt"
    return fs[0]["va"], fs[0]["size"], "symbols.txt"


def _near_lane(symbol, va):
    """add-time only (never check/gate): True iff candidates.jsonl has a `"lane": "near"` row of symbol at va."""
    if not CANDIDATES.is_file():
        return False
    for line in CANDIDATES.read_text(encoding="utf-8").splitlines():
        if line.strip():
            c = json.loads(line)
            if c.get("symbol") == symbol and c.get("lane") == "near" and c.get("va") == va:
                return True
    return False


def _units():
    return tomllib.loads(UNITS.read_text(encoding="utf-8")).get("unit", [])


def _xbe(config=None):
    """-> (pin, xbe bytes, parsed xbe); Refuse when the XBE sha1 differs from the pin."""
    pin = coff_info.pinned_sha()
    if not pin:
        raise Refuse(f"no retail_xbe pin in {coff_info.PIN}")
    xp = vv_inputs.input_path("retail_xbe", cfg=vv_inputs.load_config(config))
    xb = xp.read_bytes()
    got = hashlib.sha1(xb).hexdigest()
    if got != pin:
        raise Refuse(f"XBE sha1 {got} != pinned {pin}")
    try:
        return pin, xb, xbe_info.parse_xbe(xb)
    except xbe_info.NotXbe as e:
        raise Refuse(f"XBE unparseable: {e}")


def _body_sha1(xb, x, va, size):
    off = xbe_info.va_to_off(x, va)
    if off is None or xbe_info.va_to_off(x, va + size - 1) != off + size - 1:
        return None
    return hashlib.sha1(xb[off:off + size]).hexdigest()


def _toml_str(s):
    return '"' + s.replace("\\", "\\\\").replace('"', '\\"') + '"'


def add_function(list_path, symbol, config=None):
    entries = _parse(list_path)
    if any(e["symbol"] == symbol for e in entries):
        raise Refuse(f"{symbol} already in {list_path}")
    r = _func_row(_symbols(), symbol)
    if isinstance(r, str):
        raise Refuse(r)
    va, size, status = r
    if status == "near-corr" and not _near_lane(symbol, va):
        raise Refuse(f"{symbol} near-corr pin has no lane near row at 0x{va:08X} in {CANDIDATES.name}")
    if any(e.get("va") == va for e in entries):
        raise Refuse(f"va 0x{va:08X} already in {list_path}")
    us = [u for u in _units() if isinstance(u.get("text_start"), int) and isinstance(u.get("text_end"), int)
          and u["text_start"] <= va < u["text_end"]]
    if len(us) != 1:
        raise Refuse(f"va 0x{va:08X} lies in {len(us)} units (need 1)")
    u = us[0]
    es, ee = u.get("edge_start"), u.get("edge_end")
    if isinstance(es, int) and isinstance(ee, int) and es <= va < ee:
        raise Refuse(f"va 0x{va:08X} inside unit {u['name']} edge 0x{es:08X}..0x{ee:08X}")
    pin, xb, x = _xbe(config)
    body = _body_sha1(xb, x, va, size)
    if body is None:
        raise Refuse(f"va 0x{va:08X}+0x{size:X} not mapped in the XBE")
    _append(list_path, "function", [("symbol", _toml_str(symbol)), ("unit", _toml_str(u["name"])),
                                    ("va", f"0x{va:08X}"), ("size", f"0x{size:X}"),
                                    ("xbe_sha1", _toml_str(pin)), ("body_sha1", _toml_str(body))])
    return u["name"], va, size


def add_emitter(list_path, symbol, unit, reason):
    entries = _parse(list_path)
    if any(e["symbol"] == symbol for e in entries):
        raise Refuse(f"{symbol} already in {list_path}")
    if symbol in _symbols():
        raise Refuse(f"{symbol} is a retail symbol (symbols.txt), not an emitter")
    if not any(u.get("name") == unit for u in _units()):
        raise Refuse(f"unit {unit} not in {UNITS.name}")
    _append(list_path, "emitter", [("symbol", _toml_str(symbol)), ("unit", _toml_str(unit)),
                                   ("reason", _toml_str(reason))])


def _append(list_path, kind, kv):
    p = Path(list_path)
    text = p.read_text(encoding="utf-8")
    if text and not text.endswith("\n"):
        text += "\n"
    text += f"\n[[{kind}]]\n" + "".join(f"{k} = {v}\n" for k, v in kv)
    p.write_text(text, encoding="utf-8", newline="\n")


def _obj_symbols(obj):
    """-> [(name, class, section, section chars, comdat selection or None)] of non-section symbols with section > 0.
    coff_info.parse skips aux records; the section-definition aux (format 5) is read here: Selection = aux byte 14."""
    d = Path(obj).read_bytes()
    c = coff_info.parse(d)
    symptr = struct.unpack_from("<I", d, 8)[0]
    secs = c["sections"]

    def is_secdef(y):
        return (y["aux"] and y["class"] == coff_info.SC_STATIC and y["value"] == 0 and y["section"] > 0
                and y["name"] == secs[y["section"] - 1]["name"])
    sel = {}
    for y in c["symbols"].values():
        if is_secdef(y):
            sel.setdefault(y["section"], d[symptr + 18 * (y["index"] + 1) + 14])
    out = []
    for y in c["symbols"].values():
        if y["section"] > 0 and not is_secdef(y):
            ch = secs[y["section"] - 1]["chars"]
            out.append((y["name"], y["class"], y["section"], ch, sel.get(y["section"]) if ch & SCN_LNK_COMDAT else None))
    return out


def _asm_verdict(src, flags, bindir):
    SCRATCH.mkdir(parents=True, exist_ok=True)
    tmp = Path(tempfile.mkdtemp(prefix="matched_set-", dir=SCRATCH))
    try:
        out = tmp / "pp.i"
        ok, codes, text = msvc71.preprocess(bindir, src, flags, out, scratch=tmp)
        if not ok:
            return f"fail: preprocess failed {codes} {text[:200]}"
        body = LITERAL.sub('""', out.read_text(encoding="utf-8", errors="replace"))
        hits = [i + 1 for i, l in enumerate(body.splitlines()) if ASM.search(l)]
        return f"fail: __asm/_asm in preprocessed source at line(s) {hits[:10]}" if hits else "pass"
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def _hand_spelling(symbol):
    """5.1 T2: -> (regex of a hand-written definition's name, function name) or None (no hand spelling).
    `_name` / `@name@N` -> name; `?name@B@A@@...` -> A::B::name (bare name when no scope); `??0X@` X::X, `??1X@`
    X::~X, `??4X@` X::operator=; any other `??` name, or a scope that is not a plain identifier -> None."""
    if symbol.startswith("??"):
        m = re.match(r"^\?\?([014])([A-Za-z_]\w*)@", symbol)
        if not m:
            return None
        x = re.escape(m.group(2))
        tail = {"0": x, "1": r"~\s*" + x, "4": r"operator\s*="}[m.group(1)]
        return r"(?<![\w:~])" + x + r"\s*::\s*" + tail + r"\s*\(", m.group(2)
    if symbol.startswith("?"):
        parts = symbol[1:].split("@@", 1)[0].split("@")
        if not all(IDENT.fullmatch(p) for p in parts):
            return None
        name = parts[0]
        return r"(?<![\w:~])" + r"\s*::\s*".join(map(re.escape, parts[:0:-1] + [name])) + r"\s*\(", name
    m = re.match(r"^[_@]([A-Za-z_]\w*?)(?:@\d+)?$", symbol)
    return (r"(?<![\w:~])" + re.escape(m.group(1)) + r"\s*\(", m.group(1)) if m else None


def _code_depths(lines):
    """-> [(brace depth before the line, its code with comments blanked and literals emptied)] per line (as
    provenance_lint.code_depths; local: provenance_lint imports this module)."""
    out, depth, inblock = [], 0, False
    for line in lines:
        start, buf, j = depth, [], 0
        while j < len(line):
            if inblock:
                e = line.find("*/", j)
                inblock, j = (True, len(line)) if e < 0 else (False, e + 2)
                continue
            ch = line[j]
            if ch in "\"'":
                k = j + 1
                while k < len(line) and line[k] != ch:
                    k += 2 if line[k] == "\\" else 1
                buf.append(ch + ch)
                j = k + 1
            elif line.startswith("//", j):
                break
            elif line.startswith("/*", j):
                inblock, j = True, j + 2
            else:
                depth += (ch == "{") - (ch == "}")
                buf.append(ch)
                j += 1
        out.append((start, "".join(buf)))
    return out


def _hand_body(text, symbol):
    """5.1 T2 (D124): -> 1-based line of the first hand-written definition of symbol in text that is not a marked
    scaffold (name `pin_emit_*` and the contiguous `//` lines directly above hold `!FAKE:` and `scaffold`), else None.
    A definition: a name hit at brace depth 0 on a non-`#` code line whose following code reaches `{` before `;`."""
    sp = _hand_spelling(symbol)
    if sp is None:
        return None
    rx, name = re.compile(sp[0]), sp[1]
    lines = text.splitlines()
    code = _code_depths(lines)
    for i, (depth, c) in enumerate(code):
        if c.lstrip().startswith("#"):
            continue
        for m in rx.finditer(c):
            if depth + c[:m.start()].count("{") - c[:m.start()].count("}"):
                continue
            rest = [c[m.end():]] + [cc for _d, cc in code[i + 1:]]
            hit = next((ch for r in rest for ch in r if ch in "{;"), None)
            if hit != "{":
                continue
            j, above = i - 1, []
            while j >= 0 and lines[j].strip().startswith("//"):
                above.append(lines[j])
                j -= 1
            note = " ".join(above)
            if not (name.startswith(SCAFFOLD_PREFIX) and "!FAKE:" in note and "scaffold" in note):
                return i + 1
    return None


def check(entries, built, src_root, config=None):
    """-> {symbol: {check: verdict}, "*objects*": {"extra": verdict}}; verdict "pass" or "fail: <why>"."""
    pin, xb, x = _xbe(config)
    rows, pins = _symbols(), _pins()
    bindir = Path(vv_inputs.load_config(config)["msvc71_dir"])
    byunit = {}
    for b in built:
        byunit.setdefault(b["unit"], []).append(b)
    objsyms = {str(b["obj"]): _obj_symbols(b["obj"]) for b in built}
    funcs = [e for e in entries if e["kind"] == "function"]
    emits = [e for e in entries if e["kind"] == "emitter"]
    res = {}  # was: asm_cache, res = {}, {}
    # 5.1 T2 (O18): every unique (source, flags) of a one-obj unit, sorted, preprocessed on WORKERS threads; each
    # verdict is read back by its key (a dict of futures by key), never by completion order; no cache across runs
    keys = sorted({(b["source"], b["flags"]) for e in funcs if len(byunit.get(e["unit"], [])) == 1
                   for b in byunit[e["unit"]]}, key=lambda k: (str(k[0]), k[1]))
    with concurrent.futures.ThreadPoolExecutor(WORKERS) as ex:
        futs = {k: ex.submit(_asm_verdict, Path(src_root) / k[0], k[1].split(), bindir) for k in keys}
    asm = {k: f.result() for k, f in futs.items()}
    for e in funcs:
        v, s = {}, e["symbol"]
        v["xbe_sha1"] = "pass" if e["xbe_sha1"] == pin else f"fail: xbe_sha1 {e['xbe_sha1']} != pin {pin}"
        r = _func_row(rows, s, pins, config)
        if isinstance(r, str):
            v["size"] = v["body_sha1"] = f"fail: {r}"
        else:
            va, size, _ = r
            v["size"] = ("pass" if (e["va"], e["size"]) == (va, size) else
                         f"fail: list va 0x{e['va']:08X} size 0x{e['size']:X} != symbols.txt 0x{va:08X} 0x{size:X}")
            body = _body_sha1(xb, x, va, size)
            v["body_sha1"] = "pass" if body == e["body_sha1"] else f"fail: body_sha1 {e['body_sha1']} != XBE {body}"
        bs = byunit.get(e["unit"], [])
        if len(bs) != 1:
            v["defined"] = v["asm"] = f"fail: unit {e['unit']} has {len(bs)} built objs (need 1)"
        else:
            b = bs[0]
            v["defined"] = ("pass" if any(n == s for n, *_ in objsyms[str(b["obj"])]) else
                            f"fail: {s} not defined in {Path(b['obj']).name}")
            v["asm"] = asm[(b["source"], b["flags"])]
            # was: key = (b["source"], b["flags"]); if key not in asm_cache: asm_cache[key] = _asm_verdict(
            #     Path(src_root) / b["source"], b["flags"].split(), bindir); v["asm"] = asm_cache[key]  (serial)
        res[s] = v
    listed = {(e["unit"], e["symbol"]) for e in funcs + emits}
    names = {e["symbol"] for e in funcs}
    offenders = []
    for b in built:
        for n, cls, _sec, ch, sel in objsyms[str(b["obj"])]:
            if not ch & SCN_CNT_CODE or (b["unit"], n) in listed:
                continue
            m = FUNCLET.match(n)
            if cls == coff_info.SC_STATIC and m and (m.group(1) or m.group(2)) in names:
                continue  # eh-funclet
            if cls == coff_info.SC_STATIC and LOCAL_LABEL.match(n):
                continue  # local-label
            if sel == SEL_ANY:
                continue  # inline-copy
            offenders.append(f"{Path(b['obj']).name}:{n}")
    hand = []  # 5.1 T2 (D124): an [[emitter]] row is only compiler-emitted code or a marked scaffold
    for e in emits:
        for b in byunit.get(e["unit"], []):
            ln = _hand_body((Path(src_root) / b["source"]).read_text(encoding="utf-8", errors="replace"), e["symbol"])
            if ln:
                hand.append(f"[[emitter]] {e['symbol']} ({Path(b['obj']).name}) has a hand-written body at "
                            f"{Path(b['source']).as_posix()}:{ln}")
    fails = ([f"unlisted code symbol(s) {', '.join(offenders)}"] if offenders else []) + hand
    res[OBJECTS] = {"extra": "pass" if not fails else "fail: " + "; ".join(fails)}
    # was: {"extra": "pass" if not offenders else f"fail: unlisted code symbol(s) {', '.join(offenders)}"}
    return res


def built_from(src_root, objs):
    """Reconstruct build.build() rows from a source tree + an obj dir (no rebuild); objs missing on disk are skipped."""
    src_root, objs = Path(src_root).resolve(), Path(objs).resolve()
    if not src_root.is_dir():
        raise Refuse(f"missing src root {src_root}")
    if not objs.is_dir():
        raise Refuse(f"missing objs dir {objs}")
    umap = build.unit_map(UNITS)
    out = []
    for s in sorted((p for p in src_root.rglob("*") if p.is_file() and p.suffix.lower() in (".c", ".cpp")),
                    key=lambda p: p.relative_to(src_root).as_posix()):
        rel = s.relative_to(src_root).as_posix()
        us = umap.get(rel.lower(), [])
        if len(us) != 1:
            raise Refuse(f"{rel} maps to {len(us)} units")
        obj = objs / (rel.rsplit(".", 1)[0] + ".obj")  # 5.1 T4.c2 (build's rule); was: objs / <objname basename>
        if obj.is_file():
            out.append({"unit": us[0]["name"], "source": rel, "obj": obj, "flags": " ".join(build.flags_of(us[0]))})
    return out


def main(argv):
    ap = argparse.ArgumentParser(prog="matched_set.py", description=__doc__.splitlines()[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    a = sub.add_parser("add", help="append a [[function]] (--symbol) or [[emitter]] (--emitter --unit --reason)")
    a.add_argument("--list", required=True)
    g = a.add_mutually_exclusive_group(required=True)
    g.add_argument("--symbol")
    g.add_argument("--emitter")
    a.add_argument("--unit")
    a.add_argument("--reason")
    a.add_argument("--config")
    c = sub.add_parser("check", help="check every listed function against built objs + the retail XBE")
    c.add_argument("--list", required=True)
    c.add_argument("--src", required=True)
    c.add_argument("--objs", required=True)
    c.add_argument("--config")
    args = ap.parse_args(argv)
    try:
        if args.cmd == "add":
            if args.symbol:
                unit, va, size = add_function(args.list, args.symbol, args.config)
                print(f"added {args.symbol} unit {unit} va 0x{va:08X} size 0x{size:X}")
            else:
                if not args.unit or not args.reason:
                    raise Refuse("--emitter needs --unit and --reason")
                add_emitter(args.list, args.emitter, args.unit, args.reason)
                print(f"added emitter {args.emitter} unit {args.unit}")
            return 0
        entries = load(args.list)
        res = check(entries, built_from(args.src, args.objs), args.src, args.config)
    except (Refuse, build.Refuse) as e:
        print(f"matched_set: REFUSED {e}", file=sys.stderr)
        return 2
    npass = nfail = 0
    for s, v in res.items():
        if s == OBJECTS:
            continue
        ok = is_done({"verdicts": v})
        npass, nfail = npass + ok, nfail + (not ok)
        print(f"{'PASS' if ok else 'FAIL'} {s}: " + "; ".join(f"{k} {v[k]}" for k in REQUIRED))
    gl = res[OBJECTS]
    for k, v in gl.items():
        print(f"{'PASS' if v == 'pass' else 'FAIL'} {OBJECTS} {k}: {v}")
    print(f"{npass} passed, {nfail} failed of {npass + nfail}")
    return 0 if nfail == 0 and npass >= 1 and all(v == "pass" for v in gl.values()) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
