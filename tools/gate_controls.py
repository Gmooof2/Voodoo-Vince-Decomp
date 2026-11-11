"""Gate self-test controls (moved out of tools/gate.py, 5.4 T5): CONTROLS, CFG_CONTROLS, LINE_GLOBALS, controls()
and every control maker; their history and contract are in gate.py's docstring (--self-test).
Imported only by gate.main on --self-test. Binding: controls(gate, config) gets the RUNNING gate module (gate.py runs as
__main__; `import gate` here would load a second copy: a new pycache prefix, separate globals, and every monkeypatch
control would patch the wrong module). _bind copies GATE_NAMES from it into this module once per call; a control that
patches a gate global (lock-imports `_tool`; gate-lock `RUN_LOCK`, `run`, `_positive_ok`) patches vars(G), the module
gate's own functions read."""
import contextlib
import copy
import gzip
import hashlib
import io
import json
import os
import re
import shutil
import subprocess
import sys
import tomllib
import types
from pathlib import Path

import annotations
import audit_public
import build
import byte_compare
import coff_info
import matched_set
import pin_rows
import provenance_lint
import relink
import timing  # 5.4 T5.c2: free_mb sizes the workers
import verifier_lock
import vv_inputs

REPO = vv_inputs.REPO  # == gate.REPO; needed at import (PIN_TOML), rebound by _bind
G = None  # the running gate module, set by _bind
GATE_NAMES = ("FIX_ACC", "FIX_LIST", "FIX_SRC", "LIST", "POSITIVE_GLOBALS", "REPO", "RUN_LOCK_AT", "SRC", "SYMTABS",
              "TOOLS", "_core", "_positive_ok", "_sha", "_tname", "_tool", "_write_last", "main", "run")


def _bind(gate):
    """G = gate (the running module, never a second import); GATE_NAMES copied from it into this module's globals."""
    global G
    G = gate
    globals().update({k: getattr(gate, k) for k in GATE_NAMES})


# --- self-test controls (1.5 T5): each mutates one scratch copy of the fixture and must fail at its own check ---
LOC_UNIT, LOC_SYM = "Xbox\\OfficialRelease\\LocString.obj", "_LocString_GetStringID"
LOC_SRC = Path("Libs") / "LocString" / "LocString.cpp"
SET_SYM = "?SetState@VinceDropsInCutscene@@EAEXH@Z"
FLOUR_UNIT, FLOUR_SYM = "Xbox\\OfficialRelease\\FlourSack.obj", "??1FlourSackMain@@UAE@XZ"
PIN_TOML = REPO / "config" / "compiler_pin.toml"


def _units_copy(dst, unit, edit):
    """units.toml copy with `unit`'s opt replaced by edit(old opt); asserted by re-parsing the copy."""
    lines = build.UNITS.read_text(encoding="utf-8").splitlines(keepends=True)
    head = [i for i, ln in enumerate(lines) if ln.strip() == f"name = {json.dumps(unit)}"]
    if len(head) != 1:
        raise RuntimeError(f"unit {unit}: {len(head)} name lines in {build.UNITS}")
    old = new = None
    for i in range(head[0] + 1, len(lines)):
        if lines[i].startswith("[[unit]]"):
            break
        if lines[i].startswith("opt = "):
            old = tomllib.loads(lines[i])["opt"]
            new = edit(old)
            lines[i] = f"opt = {json.dumps(new)}  # gate control\n"
            break
    if new is None:
        raise RuntimeError(f"unit {unit}: no opt line")
    dst.write_text("".join(lines), encoding="utf-8", newline="\n")
    got = [u for u in tomllib.loads(dst.read_text(encoding="utf-8"))["unit"] if u["name"] == unit]
    if [u.get("opt") for u in got] != [new]:
        raise RuntimeError(f"units copy: {unit} opt {got} != {new!r}")
    return old, new


def _edit_text(path, old, new):
    s = path.read_text(encoding="utf-8")
    if s.count(old) != 1:
        raise RuntimeError(f"{path}: anchor {old!r} found {s.count(old)} times")
    path.write_text(s.replace(old, new), encoding="utf-8", newline="\n")


def _c_objdiff(d):
    flip = next(f["control"] for f in tomllib.loads(PIN_TOML.read_text(encoding="utf-8"))["function"]
                if f["symbol"] == LOC_SYM)
    old, new = _units_copy(d / "units.toml", LOC_UNIT, lambda o: " ".join(flip[1] if t == flip[0] else t
                                                                          for t in o.split()))
    print(f"control objdiff: {LOC_UNIT} opt {old!r} -> {new!r} (compiler_pin control {flip})")
    # 3.5 T4 C1: build pass + the verdict texts; was: objdiff != pass and bytes != pass (a broken compile passed)
    return {"units_path": d / "units.toml"}, lambda r, v, g, i: (
        v[LOC_SYM]["build"] == "pass" and v[LOC_SYM]["objdiff"].startswith("fail: fuzzy") and
        v[LOC_SYM]["bytes"].startswith(("fail: mismatch", "fail: REFUSED extent")),  # the flag: extent 0x25 != 0x22
        f"{LOC_SYM} objdiff + bytes (build pass)")


def _c_bytes(d):
    return {"symtabs": SYMTABS[:1]}, lambda r, v, g, i: (
        v[SET_SYM]["bytes"].startswith("fail: provisional") and v[SET_SYM]["objdiff"] == "pass",
        f"{SET_SYM} bytes provisional (objdiff pass)")


def _c_target(d):
    pin = coff_info.pinned_sha()
    src = REPO / "derived" / "delink" / f"retail-{(pin or '')[:8]}"
    t = d / "targets"
    t.mkdir()
    shutil.copy2(src / "hashes.txt", t / "hashes.txt")
    shutil.copy2(src / byte_compare.CUT_MARKER, t / byte_compare.CUT_MARKER)  # valid over the unchanged hashes.txt
    for e in matched_set.load(FIX_LIST):
        if e["kind"] == "function" and not (t / _tname(e["unit"])).exists():
            shutil.copy2(src / _tname(e["unit"]), t / _tname(e["unit"]))
    obj = t / _tname(FLOUR_UNIT)
    b = bytearray(obj.read_bytes())
    b[-1] ^= 0xFF
    obj.write_bytes(bytes(b))
    print(f"control target: {obj.name} copy, last byte flipped")
    return {"tdir": t}, lambda r, v, g, i: (
        "sha256" in g.get(f"target {FLOUR_UNIT}", "pass") and "sha256" in v[FLOUR_SYM]["target"],
        f"target {FLOUR_UNIT} (sha256)")  # 3.5 T4 C5; was: != pass (`target missing` passed it)


def _c_cut_marker(d):
    pin = coff_info.pinned_sha()
    src = REPO / "derived" / "delink" / f"retail-{(pin or '')[:8]}"
    t = d / "targets"
    t.mkdir()
    shutil.copy2(src / "hashes.txt", t / "hashes.txt")
    for e in matched_set.load(FIX_LIST):
        if e["kind"] == "function" and not (t / _tname(e["unit"])).exists():
            shutil.copy2(src / _tname(e["unit"]), t / _tname(e["unit"]))
    print(f"control cut-marker: hashes.txt + targets copied, no {byte_compare.CUT_MARKER}")
    return {"tdir": t}, lambda r, v, g, i: (
        r == 2 and byte_compare.CUT_MARKER in g.get("cut", ""), "cut (marker absent)")


def _list_copy(d, field):
    lines = FIX_LIST.read_text(encoding="utf-8").splitlines(keepends=True)
    i = lines.index(f"symbol = {json.dumps(FLOUR_SYM)}\n")
    i = next(j for j in range(i + 1, len(lines)) if lines[j].startswith(f"{field} = "))
    lines[i] = f'{field} = "{"0" * 40}"\n'
    lp = d / "matched_set.toml"
    lp.write_text("".join(lines), encoding="utf-8", newline="\n")
    print(f"control {field}: list copy, {FLOUR_SYM} {field} -> {'0' * 40}")
    return lp


def _c_body(d):
    return {"list_path": _list_copy(d, "body_sha1")}, lambda r, v, g, i: (  # 3.5 T4 C5; was: != pass
        v[FLOUR_SYM]["body_sha1"].startswith(f"fail: body_sha1 {'0' * 40} != XBE"), f"{FLOUR_SYM} body_sha1")


def _c_xbe(d):
    return {"list_path": _list_copy(d, "xbe_sha1")}, lambda r, v, g, i: (  # 3.5 T4 C5; was: != pass
        v[FLOUR_SYM]["xbe_sha1"].startswith(f"fail: xbe_sha1 {'0' * 40} != pin"), f"{FLOUR_SYM} xbe_sha1")


def _c_size_list(d):  # 4.5 T16.2.c3 (G14): a list `size` edit fails the row's size verdict, body_sha1 still passes
    lines = FIX_LIST.read_text(encoding="utf-8").splitlines(keepends=True)
    i = lines.index(f"symbol = {json.dumps(FLOUR_SYM)}\n")
    i = next(j for j in range(i + 1, len(lines)) if lines[j].startswith("size = "))
    new = int(lines[i].split("=", 1)[1].strip(), 16) + 1
    lines[i] = f"size = 0x{new:X}\n"
    lp = d / "matched_set.toml"
    lp.write_text("".join(lines), encoding="utf-8", newline="\n")
    print(f"control size-list: list copy, {FLOUR_SYM} size -> 0x{new:X}")
    return {"list_path": lp}, lambda r, v, g, i_: (
        v[FLOUR_SYM]["size"].startswith("fail: list va ") and f" size 0x{new:X} != symbols.txt " in v[FLOUR_SYM]["size"]
        and v[FLOUR_SYM]["body_sha1"] == "pass", f"{FLOUR_SYM} size")


def _c_empty(d):
    lp = d / "matched_set.toml"
    lp.write_text("# gate control: no entries\n", encoding="utf-8", newline="\n")
    return {"list_path": lp}, lambda r, v, g, i: (r == 1 and g.get("list", "pass") != "pass", "list (N=0)")


def _c_missing(d):
    return {"list_path": d / "absent.toml"}, lambda r, v, g, i: (
        r == 1 and g.get("list", "pass") != "pass", "list (missing)")


def _src_copy(d):
    s = d / "src"
    shutil.copytree(FIX_SRC, s)
    return s


def _c_extra(d):
    s = _src_copy(d)
    with open(s / LOC_SRC, "a", encoding="utf-8", newline="\n") as f:
        f.write('\nextern "C" int gate_control_extra(int x)\n{\n    return x + 1;\n}\n')
    print(f"control extra: unmarked _gate_control_extra appended to {LOC_SRC.as_posix()}")
    return {"src_root": s}, lambda r, v, g, i: (  # 4.5 T16.2.c5 G13: the planted name; was: != "pass"
        "unlisted code symbol(s) LocString.obj:_gate_control_extra" in g.get("*objects* extra", ""),
        "*objects* extra (_gate_control_extra)")


def _c_marker(d):
    s = _src_copy(d)
    p = s / LOC_SRC
    _edit_text(p, 'extern "C" int LocString_GetStringID', '#ifdef NON_MATCHING\nextern "C" int LocString_GetStringID')
    with open(p, "a", encoding="utf-8", newline="\n") as f:
        f.write("#endif  // NON_MATCHING\n")
    print(f"control marker: {LOC_SYM} wrapped in #ifdef NON_MATCHING")
    return {"src_root": s}, lambda r, v, g, i: (  # 3.5 T4 C2; was: defined != pass (a broken compile passed)
        v[LOC_SYM]["build"] == "pass" and v[LOC_SYM]["defined"].startswith(f"fail: {LOC_SYM} not defined in"),
        f"{LOC_SYM} defined (build pass)")


def _c_asm(d):
    s = _src_copy(d)
    _edit_text(s / LOC_SRC, "    return -1;\n}", "    __asm { nop }\n    return -1;\n}")
    print(f"control asm: __asm {{ nop }} inserted in {LOC_SYM}")
    return {"src_root": s}, lambda r, v, g, i: (  # 3.5 T4 C2; was: asm != pass (a broken compile passed)
        v[LOC_SYM]["build"] == "pass" and v[LOC_SYM]["asm"].startswith("fail: __asm/_asm in preprocessed"),
        f"{LOC_SYM} asm (build pass)")


def _c_audit(d):
    p = d / "probe.obj"
    p.write_bytes(b"MZ" + bytes(62))
    print(f"control audit: {p} (MZ magic, .obj suffix) audited via --paths")
    return {"audit_args": ("--paths", str(p))}, lambda r, v, g, i: (  # 3.5 T4 C3; was: != pass (an rc 2 crash passed)
        g["audit_public"].startswith("fail: rc 1: ") and f"HIT paths magic {p}" in g["audit_public"],
        "audit_public (rc 1, magic hit)")


def _c_lane_audit_missing(d):  # 5.4 T5.c3: the audit lane's child cannot start -> audit_public fails, rc != 0
    tools = d / "tools"
    tools.mkdir()
    print("control lane-audit-missing: audit run from a scratch tools dir without audit_public.py")
    return {"audit_tools": tools, "run_audit": True}, lambda r, v, g, i: (
        g.get("audit_public", "").startswith("fail: rc ") and "audit_public.py" in g["audit_public"],
        "audit_public (the lane's child: no audit_public.py)")


def _c_lane_raise(d):  # 5.4 T5.c3 (D26): the audit lane's callable raises -> audit_public a lane error, rc != 0
    orig = _tool

    def tool(name, *a, **k):  # check_env (on the main thread) still runs the real _tool
        if name == "audit_public":
            raise RuntimeError("gate control lane-raise")
        return orig(name, *a, **k)
    vars(G)["_tool"] = tool  # set last in make: restored by the runner's undo (finally)
    print("control lane-raise: the audit lane's _tool raises RuntimeError")
    return {"run_audit": True, "undo": lambda: vars(G).__setitem__("_tool", orig)}, lambda r, v, g, i: (
        g.get("audit_public") == "fail: audit_public lane error RuntimeError: gate control lane-raise",
        "audit_public (lane error RuntimeError)")


def _c_clean(d, config=None):
    seed_src = d / "seed_src"
    (seed_src / LOC_SRC).parent.mkdir(parents=True)
    shutil.copy2(FIX_SRC / LOC_SRC, seed_src / LOC_SRC)
    seed = build.build(seed_src, d / "seed", build.UNITS, config)  # 4.5 T16.2.c4 (G15); was: no config (default)
    s = _src_copy(d)
    (s / LOC_SRC).unlink()
    bdir = d / "build"
    bdir.mkdir()
    # 5.1 T4.c2: planted at the source-path obj; was: bdir / Path(seed[0]["obj"]).name
    (bdir / LOC_SRC).parent.mkdir(parents=True)
    shutil.copy2(seed[0]["obj"], bdir / LOC_SRC.with_suffix(".obj"))
    print(f"control clean-build: good {Path(seed[0]['obj']).name} planted in {bdir}; {LOC_SRC.as_posix()} removed")
    return {"src_root": s, "bdir": bdir}, lambda r, v, g, i: (  # 4.5 T16.2.c5 G13: the removed source; was: != "pass"
        v[LOC_SYM]["build"] == f"fail: no source built for unit {LOC_UNIT}", f"{LOC_SYM} build (no source built)")


def _c_d26(d):
    old, new = _units_copy(d / "units.toml", LOC_UNIT, lambda o: o + " /Zq9")
    print(f"control d26: {LOC_UNIT} opt {old!r} -> {new!r}")

    def expect(r, v, g, i):
        fu = i["failures"].get(LOC_UNIT)
        m = re.match(r"cl exit (-?\d+)", fu["text"]) if fu else None
        print(f"control d26: cl rc {m.group(1) if m else 'not reported'}; codes {fu['codes'] if fu else None}; "
              f"obj exists {(i['bdir'] / LOC_SRC.with_suffix('.obj')).is_file()}")  # 5.1 T4.c2; was: bdir / 'LocString.obj'
        # 3.5 T4 C4: cl exit 0 + D4002; was: build != pass only (any compile failure passed)
        return (v[LOC_SYM]["build"] != "pass" and bool(m) and m.group(1) == "0" and "D4002" in fu["codes"],
                f"{LOC_SYM} build (cl exit 0, D4002)")
    return {"units_path": d / "units.toml"}, expect


def _c_data_impostor(d, config=None):  # 3.5 T4 F2: the fn as a const data array of the retail bytes, .text COMDAT
    pin, xb, x = matched_set._xbe(config)  # the bytes are read from the pinned XBE now, into the scratch copy only (D46)
    e = next(e for e in matched_set.load(FIX_LIST) if e["symbol"] == LOC_SYM)
    off = byte_compare.xbe_info.va_to_off(x, e["va"])
    init = ", ".join(f"0x{b:02X}" for b in xb[off:off + e["size"]])
    s = _src_copy(d)
    p = s / LOC_SRC
    t = p.read_text(encoding="utf-8")
    i = t.index('extern "C" int LocString_GetStringID')
    p.write_text(t[:i] + '#pragma const_seg(".text")\nextern "C" __declspec(selectany) const unsigned char '
                 f'LocString_GetStringID[{e["size"]}] = {{{init}}};\n#pragma const_seg()\n',
                 encoding="utf-8", newline="\n")
    body = matched_set._body_sha1(xb, x, e["va"], e["size"])  # the row's hashes recomputed: only the impostor differs
    lp = d / "matched_set.toml"
    lp.write_text(FIX_LIST.read_text(encoding="utf-8").replace(f'xbe_sha1 = "{e["xbe_sha1"]}"\nbody_sha1 = '
                                                               f'"{e["body_sha1"]}"', f'xbe_sha1 = "{pin}"\n'
                                                               f'body_sha1 = "{body}"'), encoding="utf-8", newline="\n")
    print(f"control data-impostor: {LOC_SYM} -> const data[{e['size']}] in .text COMDAT (retail bytes, scratch "
          f"only); row hashes recomputed (body {'same' if body == e['body_sha1'] else 'CHANGED'})")

    def expect(r, v, g, i):
        print(f"control data-impostor: objdiff {v[LOC_SYM]['objdiff']}")
        return v[LOC_SYM]["bytes"].startswith("fail: REFUSED") and "not a function" in v[LOC_SYM]["bytes"], \
            f"{LOC_SYM} bytes (data symbol)"
    return {"src_root": s, "list_path": lp}, expect


def _cfg_copy(d, config, key, value):  # 3.5 T4: config copy with one key replaced (private paths: never printed)
    c = json.loads(Path(config or vv_inputs.CONFIG).read_text(encoding="utf-8"))
    c[key] = str(value)
    p = d / "local_paths.json"
    p.write_text(json.dumps(c, indent=1) + "\n", encoding="utf-8", newline="\n")
    return p


def _cfg_null(d, config, keys):  # 4.1 T3.1: config copy with these keys null (_cfg_copy's str() cannot say null)
    c = json.loads(Path(config or vv_inputs.CONFIG).read_text(encoding="utf-8"))
    c.update(dict.fromkeys(keys))
    p = d / "local_paths.json"
    p.write_text(json.dumps(c, indent=1) + "\n", encoding="utf-8", newline="\n")
    return p


def _c_xbe_wrong(d, config=None):  # 3.5 T4 N2
    xp = vv_inputs.input_path("retail_xbe", cfg=vv_inputs.load_config(config))
    b = bytearray(xp.read_bytes())
    b[-1] ^= 0xFF
    (d / "retail.xbe").write_bytes(bytes(b))
    print("control xbe-wrong: config copy, retail_xbe -> a copy with its last byte flipped")
    return {"config": _cfg_copy(d, config, "retail_xbe", d / "retail.xbe")}, lambda r, v, g, i: (
        g.get("xbe_sha1", "").startswith("fail: XBE sha1") and g.get("bytes setup", "").startswith("fail: REFUSED XBE sha1"),
        "xbe_sha1 + bytes setup REFUSED")


def _c_objdiff_cli(d, config=None):  # 3.5 T4 N3: objdiff_cli -> the Python (`report` is no script: rc 2)
    print("control objdiff-cli: config copy, objdiff_cli -> the Python executable (exits non-zero)")
    return {"config": _cfg_copy(d, config, "objdiff_cli", sys.executable)}, lambda r, v, g, i: (
        g.get("objdiff report", "").startswith("fail: objdiff-cli report rc") and
        v[LOC_SYM]["objdiff"] == "fail: not in report", "objdiff report")


def _c_objdiff_pin(d, config=None):  # 4.5 T16.1 G4: check_env pins the objdiff-cli binary (config/objdiff_cli.sha256)
    src = vv_inputs.load_config(config)["objdiff_cli"]
    b = bytearray(src.read_bytes())
    b[-1] ^= 0xFF
    (d / src.name).write_bytes(bytes(b))
    print("control objdiff-pin: config copy, objdiff_cli -> a copy of the binary with its last byte flipped")
    return {"config": _cfg_copy(d, config, "objdiff_cli", d / src.name)}, lambda r, v, g, i: (
        g.get("check_env", "").startswith("fail: rc 1") and "objdiff_cli" in g.get("check_env", ""),
        "check_env (objdiff_cli pin)")


def _c_objdiff_noexec(d, config=None):  # 5.1 T1: objdiff_cli exists but is not executable -> CreateProcess OSError
    p = d / "zz.exe"
    p.write_text("gate control: a text file, not a program\n", encoding="utf-8", newline="\n")
    print("control objdiff-noexec: config copy, objdiff_cli -> a text file zz.exe (exists, not executable)")
    return {"config": _cfg_copy(d, config, "objdiff_cli", p)}, lambda r, v, g, i: (
        r == 1 and g.get("objdiff report", "").startswith("fail: objdiff-cli OSError") and
        v[LOC_SYM]["objdiff"] == "fail: not in report" and "xbe_sha1" in g and "bytes setup" in g and
        "bytes" in v[LOC_SYM], "objdiff report (OSError; xbe_sha1 + bytes still run)")


def _c_toolchain_hash(d, config=None):  # 4.5 T16.2.c3 (G14): check_env check_pins on a pinned msvc binary's sha
    src, m = Path(vv_inputs.load_config(config)["msvc71_dir"]), d / "msvc"
    m.mkdir()
    for ln in (REPO / "config" / "toolchain.sha256").read_text(encoding="ascii").splitlines():
        shutil.copy2(src / ln.split("  ", 1)[1], m)
    b = bytearray((m / "link.exe").read_bytes())  # link.exe: the gate's build compiles only, never runs it
    b[-1] ^= 0xFF
    (m / "link.exe").write_bytes(bytes(b))
    print("control toolchain-hash: config copy, msvc71_dir -> a copy of the pinned files, link.exe last byte flipped")
    return {"config": _cfg_copy(d, config, "msvc71_dir", m)}, lambda r, v, g, i: (
        g.get("check_env", "").startswith("fail: rc 1") and "FAIL pins: link.exe: sha mismatch" in g["check_env"],
        "check_env (link.exe pin)")


def _c_build_refuse(d, config=None):  # 3.5 T4 N4. 5.5 T12.c3 (I73 F1): kw `direct`, the three lanes run (audit on
    # one clean --paths file) and their verdicts must be consumed and printed; was: _one, no lane, rc 2 + build only
    s = _src_copy(d)
    (s / "Libs" / "NoUnit.cpp").write_text("int gate_control_no_unit;\n", encoding="utf-8", newline="\n")
    probe = d / "probe.txt"
    probe.write_text("gate control: a clean file\n", encoding="utf-8", newline="\n")
    print("control build-refuse: Libs/NoUnit.cpp (maps to no unit) added to a src copy; audit, provenance and "
          "annotations lanes run")

    def expect(r, v, g, i):
        w = io.StringIO()
        with contextlib.redirect_stdout(w):
            rc, _v, gl, _i = _core(s, FIX_LIST, config=config, bdir=d / "build", run_relink=False, run_audit=True,
                                   audit_args=("--paths", str(probe)), run_lint=True, lint_accepted=FIX_ACC,
                                   run_annotations=True, run_pins=False)
        out = w.getvalue()
        sys.stdout.write(out)
        got = {k: (gl.get(k), _printed(out, k)) for k in LANE_KEYS}
        print(f"control build-refuse: gate rc {rc}; lane globals (verdict, printed) {got}")
        return (rc == 2 and "maps to no unit" in gl.get("build", "") and _printed(out, "build")
                and all(x is not None and p for x, p in got.values()),
                "build (REFUSED) + audit_public, provenance, annotations consumed and printed")
    return {"direct": True}, expect


def _c_build_objname_path(d, config):  # 5.1 T4.c2 (C0247): obj per source path; a same-output-path pair refused
    def unit(name, obj, lang):
        return (f'[[unit]]\nname = {json.dumps(name)}\nobjname = {json.dumps(obj)}\nlanguage = "{lang}"\n'
                f'opt = "/O1 /Oy"\n')
    a = ("Apps/Vince/scripts/french_quarter/Switch.cpp", "d:\\beep\\Apps\\Vince\\scripts\\french_quarter\\Xbox\\"
         "OfficialRelease\\Switch.obj")
    b = ("Apps/Vince/core/Switch.cpp", "d:\\beep\\Apps\\Vince\\core\\XBox\\OfficialRelease\\Switch.obj")
    dup = "d:\\beep\\Apps\\Vince\\x\\Dup.obj"
    sides = {"two": ([a, b], [unit("fq Switch.obj", a[1], "c++"), unit("core Switch.obj", b[1], "c++")]),
             "dup": ([("Apps/Vince/x/Dup.c", dup), ("Apps/Vince/x/Dup.cpp", dup)],
                     [unit("c Dup.obj", dup, "c"), unit("c++ Dup.obj", dup, "c++")])}
    for k, (srcs, rows) in sides.items():
        for rel, _ in srcs:
            p = d / k / "src" / rel
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text("int gate_control_obj;\n", encoding="utf-8", newline="\n")
        (d / k / "units.toml").write_text("\n".join(rows), encoding="utf-8", newline="\n")

    def expect(r, v, g, i):
        out = d / "two" / "build"
        try:
            got = build.build(d / "two" / "src", out, d / "two" / "units.toml", config)
        except build.Refuse as e:
            got = f"REFUSED {e}"
        want = sorted(str((out / s[0]).with_suffix(".obj")) for s in (a, b))
        objs = sorted(str(x["obj"]) for x in got) if isinstance(got, list) else got
        try:
            build.build(d / "dup" / "src", d / "dup" / "build", d / "dup" / "units.toml", config)
            why = "built"
        except build.Refuse as e:
            why = f"REFUSED {e}"
        print(f"control build-objname-path: two Switch.obj units -> {objs}; Dup.c + Dup.cpp -> {why}")
        ok = objs == want and all(Path(o).is_file() for o in want) and why.startswith("REFUSED") and "Dup" in why
        return ok, "build (two Switch.obj per source path; Dup.obj refused)"
    print("control build-objname-path: scratch src+units: french_quarter + core Switch.obj; x/Dup.c + x/Dup.cpp")
    return {"direct": True}, expect


def _c_build_pool(d, config):  # 5.4 T5.c4: kw `direct`: build.build serial (workers 1) vs pooled, same out dir
    """The fixture built twice into d/b: the {obj relpath: sha256} sets equal (COFF TimeDateStamp, bytes 4..8,
    zeroed: cl stamps each obj; the obj also embeds its out path, hence one dir); then a src copy with two units
    broken: BuildFailed's failures in canonical (source) order in both builds."""
    out, w = d / "b", max(2, build.pool_size())

    def objs():
        got = {}
        for p in sorted(out.rglob("*.obj")):
            b = bytearray(p.read_bytes())
            b[4:8] = bytes(4)
            got[p.relative_to(out).as_posix().lower()] = hashlib.sha256(b).hexdigest()
        return got
    s = _src_copy(d)
    srcs = sorted(p.relative_to(s).as_posix() for p in s.rglob("*") if p.suffix.lower() in (".c", ".cpp"))
    bad = [srcs[-1], srcs[0]]  # planted last-first: the order checked is the build's, not the plant's
    for rel in bad:
        with (s / rel).open("a", encoding="utf-8", newline="\n") as fh:
            fh.write("\nint gate_control_build_pool(  /* syntax error */\n")
    print(f"control build-pool: fixture built serial (workers 1) and pooled (workers pool_size(), >= 2); "
          f"then {bad} broken")

    def expect(r, v, g, i):
        sets = []
        for n in (1, w):
            build.build(FIX_SRC, out, build.UNITS, config, workers=n)
            sets.append(objs())
        fails = []
        for n in (1, w):
            try:
                build.build(s, d / "f", build.UNITS, config, workers=n)
                fails.append(None)
            except build.BuildFailed as e:
                fails.append([f["source"] for f in e.failures])
        print(f"control build-pool: {len(sets[0])} objs serial, {len(sets[1])} pooled, equal {sets[0] == sets[1]}; "
              f"failures serial {fails[0]}, pooled {fails[1]}")
        want = sorted(bad)
        return (sets[0] == sets[1] and len(sets[0]) == len(srcs) and fails == [want, want],
                "build (pooled objs == serial; failures in source order)")
    return {"direct": True}, expect


def _target_copy(d, skip=()):
    pin = coff_info.pinned_sha()
    src = REPO / "derived" / "delink" / f"retail-{(pin or '')[:8]}"
    t = d / "targets"
    t.mkdir()
    shutil.copy2(src / "hashes.txt", t / "hashes.txt")
    shutil.copy2(src / byte_compare.CUT_MARKER, t / byte_compare.CUT_MARKER)
    for e in matched_set.load(FIX_LIST):
        n = _tname(e["unit"])
        if e["kind"] == "function" and n not in skip and not (t / n).exists():
            shutil.copy2(src / n, t / n)
    return t


def _c_target_missing(d):  # 3.5 T4 N6
    t = _target_copy(d, skip=(_tname(FLOUR_UNIT),))
    print(f"control target-missing: targets copy without {_tname(FLOUR_UNIT)}")
    return {"tdir": t}, lambda r, v, g, i: (
        v[FLOUR_SYM]["target"].startswith("fail: target missing"), f"target {FLOUR_UNIT} (missing)")


def _c_target_unhashed(d):  # 3.5 T4 N6: its hashes.txt line renamed; cut.complete re-sealed over the edited file
    t = _target_copy(d)
    h, n = t / "hashes.txt", _tname(FLOUR_UNIT)
    data = h.read_bytes()
    if data.count(f"  {n}\n".encode()) != 1:
        raise RuntimeError(f"{n}: not one hashes.txt line")
    new = data.replace(f"  {n}\n".encode(), f"  gate_control_{n}\n".encode())
    h.write_bytes(new)
    (t / byte_compare.CUT_MARKER).write_bytes(
        f"cut-complete v1 hashes-sha256 {hashlib.sha256(new).hexdigest()} objects {len(new.splitlines())}\n".encode())
    if byte_compare.cut_check(t):
        raise RuntimeError(f"re-sealed cut does not pass: {byte_compare.cut_check(t)}")
    cp = d / "delink_cut.sha256"  # 4.5 T16.2.c5 (G17): a scratch pin over the edited copy; was: no pin (cut_check only)
    cp.write_text(f"{hashlib.sha256(new).hexdigest()}  retail-{(coff_info.pinned_sha() or '')[:8]}\n", encoding="ascii",
                  newline="\n")
    print(f"control target-unhashed: hashes.txt copy, {n} line renamed, cut.complete re-sealed, scratch cut pin")
    return {"tdir": t, "cut_pin": cp}, lambda r, v, g, i: (
        "not in" in v[FLOUR_SYM]["target"] and "hashes.txt" in v[FLOUR_SYM]["target"], f"target {FLOUR_UNIT} (unhashed)")


def _c_bytes_refuse(d):  # 3.5 T4 N9: the symbol not defined in the built obj -> byte_compare.compare Refuse
    s = _src_copy(d)
    _edit_text(s / LOC_SRC, 'extern "C" int LocString_GetStringID(', 'extern "C" int LocString_GetStringID_x(')
    print(f"control bytes-refuse: {LOC_SYM} renamed in a src copy")
    return {"src_root": s}, lambda r, v, g, i: (
        v[LOC_SYM]["bytes"].startswith("fail: REFUSED symbol") and "not defined" in v[LOC_SYM]["bytes"],
        f"{LOC_SYM} bytes (compare REFUSED)")


def _c_bytes_two_objs(d):  # 3.5 T4 N9: a second unit row of the same name (LocString2.obj) -> two built objs, one unit
    s = _src_copy(d)
    shutil.copy2(s / LOC_SRC, s / LOC_SRC.with_name("LocString2.cpp"))
    text = build.UNITS.read_text(encoding="utf-8")
    i = text.index(f"name = {json.dumps(LOC_UNIT)}\n")
    i, j = text.rindex("[[unit]]", 0, i), text.find("\n[[unit]]", i)
    row = text[i:j if j >= 0 else len(text)]
    dup = re.sub(r'(?m)^(objname = ".*)LocString\.obj"$', r'\1LocString2.obj"', row)
    if dup.count("LocString2.obj") != 1:
        raise RuntimeError("units copy: objname anchor not found once")
    (d / "units.toml").write_text(text + "\n" + dup + "\n", encoding="utf-8", newline="\n")
    print(f"control bytes-two-objs: units copy + a second {LOC_UNIT} row (objname LocString2.obj); src copy + "
          f"LocString2.cpp")
    return {"src_root": s, "units_path": d / "units.toml"}, lambda r, v, g, i: (
        "2 built objs" in v[LOC_SYM]["bytes"] and "2 built objs" in v[LOC_SYM]["objdiff"],
        f"{LOC_SYM} bytes + objdiff (two objs)")


def _lock_repo(d):
    """-> scratch root d/repo: copies of the LISTED files + a fingerprint written for them (asserted to pass)."""
    r = d / "repo"
    for p in verifier_lock.LISTED:
        (r / p).parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(REPO / p, r / p)
    fp = r / verifier_lock.FP
    fp.write_text(verifier_lock.fingerprint(r), encoding="ascii", newline="\n")
    pre = verifier_lock.check(r, fp, ())
    if pre:
        raise RuntimeError(f"scratch lock does not pass before the mutation: {pre}")
    return r, fp


def _lock_kw(r, fp, want, mods=(), names=None):  # 4.5 T16.1: names -> lock_names; was: (r, fp, mods). 5.5 T12.c3
    # (G16): want, the control's own cause: the lock global is exactly `fail: <want>`; was: any lock fail
    return {"lock_root": r, "lock_fp": fp, "lock_modules": mods, "lock_names": names}, lambda rv, v, g, i: (
        rv == 1 and g.get("lock") == f"fail: {want}" and "check_env" not in g, f"lock ({want})")


def _c_lock_edit(d):
    r, fp = _lock_repo(d)
    f = r / "tools" / "byte_compare.py"
    b = bytearray(f.read_bytes())
    b[0] ^= 0x01
    f.write_bytes(bytes(b))
    print(f"control lock-edit: first byte of scratch {f.relative_to(r).as_posix()} changed")
    return _lock_kw(r, fp, "tools/byte_compare.py sha256 mismatch")


def _c_lock_missing(d):
    r, fp = _lock_repo(d)
    (r / "tools" / "xbe_info.py").unlink()
    print("control lock-missing: scratch tools/xbe_info.py deleted")
    return _lock_kw(r, fp, "listed file missing tools/xbe_info.py")


def _c_lock_fp_missing(d):
    r, fp = _lock_repo(d)
    fp.unlink()
    print("control lock-fp-missing: scratch fingerprint deleted")
    return _lock_kw(r, fp, f"fingerprint missing {fp}")


def _c_lock_fp_empty(d):
    r, fp = _lock_repo(d)
    fp.write_text("", encoding="ascii")
    print("control lock-fp-empty: scratch fingerprint emptied")
    return _lock_kw(r, fp, f"fingerprint empty {fp}")


def _c_lock_unlisted(d):
    r, fp = _lock_repo(d)
    m = r / "tools" / "_unlisted.py"
    m.write_text("# gate control: an unlisted verifier module\n", encoding="utf-8", newline="\n")
    print("control lock-unlisted: scratch tools/_unlisted.py passed as a loaded module")
    return _lock_kw(r, fp, "unlisted module tools/_unlisted.py", [str(m)])


def _c_lock_shadow(d):  # 4.5 T16.1 G5: a top-level module name with an unlisted tools/<name>.py, its __file__ deleted
    r, fp = _lock_repo(d)
    (r / "tools" / "hashlib.py").write_text("# gate control: shadows the stdlib hashlib\n", encoding="utf-8",
                                            newline="\n")
    fake = {"hashlib": types.ModuleType("hashlib")}  # a sys.modules-like entry; a fresh ModuleType has no __file__
    print("control lock-shadow: scratch tools/hashlib.py (unlisted); a module entry `hashlib` with no __file__")
    kw, _expect = _lock_kw(r, fp, "shadow module tools/hashlib.py", [getattr(m, "__file__", None) for m in fake.values()], list(fake))
    return kw, lambda rv, v, g, i: (
        rv == 1 and "shadow module tools/hashlib.py" in g.get("lock", "") and "check_env" not in g,
        "lock (shadow module tools/hashlib.py)")


def _c_lock_crlf(d):
    r, fp = _lock_repo(d)
    p = "tools/matched_set.py"
    f = r / p
    f.write_bytes(f.read_bytes().replace(b"\r\n", b"\n").replace(b"\n", b"\r\n"))
    before = fp.read_bytes()
    rc, msg = verifier_lock.write(r, fp, [])
    same = fp.read_bytes() == before
    refused = rc == 2 and p in msg and same
    print(f"control lock-crlf: scratch {p} converted to CRLF; write rc {rc}: {msg}")
    print(f"control lock-crlf: write {'refused' if rc == 2 else 'NOT refused'}, fingerprint "
          f"{'unchanged' if same else 'CHANGED'}")
    kw, expect = _lock_kw(r, fp, f"{p} sha256 mismatch")
    return kw, lambda rv, v, g, i: (expect(rv, v, g, i)[0] and refused,
                                    "lock (write refused naming the file, fingerprint unchanged)")


LOCK_PYC_MARK = "control lock-pyc: PLANTED bytecode ran"


def _c_lock_pyc(d):
    """3.5 T4 F1 (C0158): a timestamp-valid planted pyc of a listed module in the scratch repo's tools/__pycache__.
    (a) a child `gate.py --help` on the scratch repo (no PYTHONPYCACHEPREFIX) must not run it; (b) the module loaded
    in-process under an alias with the prefix unset (a listed module imported before the gate set it) runs it, and
    the lock (default sys.modules path) must fail naming its bytecode."""
    import importlib._bootstrap_external as ibe
    import importlib.util
    r, fp = _lock_repo(d)
    src = r / "tools" / "byte_compare.py"
    st = os.stat(src)
    code = compile(src.read_text(encoding="utf-8") + f"\n_LOCK_PYC_PLANTED = True\nprint({LOCK_PYC_MARK!r})\n",
                   str(src), "exec")
    pyc = src.parent / "__pycache__" / Path(importlib.util.cache_from_source(str(src))).name
    pyc.parent.mkdir(parents=True, exist_ok=True)
    pyc.write_bytes(ibe._code_to_timestamp_pyc(code, int(st.st_mtime), st.st_size))
    env = {k: v for k, v in os.environ.items() if k != "PYTHONPYCACHEPREFIX"}
    p = subprocess.run([sys.executable, str(r / "tools" / "gate.py"), "--help"], stdin=subprocess.DEVNULL, env=env,
                       capture_output=True, text=True, errors="replace", cwd=r)
    child_ran = LOCK_PYC_MARK in p.stdout
    child_ok = p.returncode == 0 and "usage" in p.stdout
    print(f"control lock-pyc: planted {pyc.relative_to(r).as_posix()}; child gate.py --help rc {p.returncode}, "
          f"plant {'RAN' if child_ran else 'not run'}")
    alias = "_lock_pyc_byte_compare"
    old = sys.pycache_prefix
    try:
        sys.pycache_prefix = None
        spec = importlib.util.spec_from_file_location(alias, src)
        spec.cached  # lazy on 3.15: fix it while the prefix is unset, as an early import would
        mod = importlib.util.module_from_spec(spec)
        sys.modules[alias] = mod
        spec.loader.exec_module(mod)
    finally:
        sys.pycache_prefix = old
    live = getattr(mod, "_LOCK_PYC_PLANTED", False)
    print(f"control lock-pyc: in-process alias load: plant {'ran' if live else 'NOT run'} (cached {mod.__spec__.cached})")

    def expect(rv, v, g, i):
        sys.modules.pop(alias, None)
        return (rv == 1 and "__pycache__" in g.get("lock", "") and "check_env" not in g and live and child_ok
                and not child_ran, "lock (repo __pycache__ bytecode; child gate entry did not run the plant)")
    return {"lock_root": r, "lock_fp": fp, "lock_modules": None}, expect


def _c_lock_symlink(d):
    """3.5 T4 W7: a module reached through a link (symlink, else a directory junction) out of the scratch repo."""
    r, fp = _lock_repo(d)
    out = d / "outside"
    out.mkdir()
    (out / "evil.py").write_text("# gate control: a module outside the repo\n", encoding="utf-8", newline="\n")
    link = r / "tools" / "ext"
    try:
        os.symlink(out, link, target_is_directory=True)
        how = "symlink"
    except OSError:
        import _winapi  # no symlink privilege: a junction needs none (never skipped silently)
        _winapi.CreateJunction(str(out), str(link))
        how = "junction"
    print(f"control lock-symlink: scratch tools/ext is a {how} to {out.name}/; tools/ext/evil.py passed as loaded")
    return _lock_kw(r, fp, "unlisted module tools/ext/evil.py (via a link)", [str(link / "evil.py")])


def _c_symtab_pin(d):
    """3.5 T4 F3: one row of the scratch config/retail/symbols.txt renamed (a forged name -> VA binding)."""
    r, fp = _lock_repo(d)
    p = "config/retail/symbols.txt"
    f = r / p
    if not f.is_file():  # not LISTED before 3.5 T4: copied here so the edit is the same
        f.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(REPO / p, f)
    lines = f.read_bytes().split(b"\n")
    k = next(n for n, ln in enumerate(lines) if ln.strip() and not ln.startswith(b"#"))
    t = lines[k].split(b" ")
    t[3] += b"_forged"
    lines[k] = b" ".join(t)
    f.write_bytes(b"\n".join(lines))
    print(f"control symtab-pin: scratch {p} line {k + 1}: name field renamed")
    return _lock_kw(r, fp, f"{p} sha256 mismatch")


def _c_lock_imports(d):
    """3.5 T4 N1/C6: `delink` (unlisted) imported after the lock layer, before lock-imports (the first _tool call),
    through the default sys.modules path; the modules it brought in are removed after."""
    orig, before = _tool, set(sys.modules)

    def tool(*a, **k):
        vars(G)["_tool"] = orig
        import delink  # noqa: F401
        return orig(*a, **k)
    vars(G)["_tool"] = tool
    print("control lock-imports: tools/delink.py imported at the first _tool call (after the lock layer)")

    def expect(rv, v, g, i):
        vars(G)["_tool"] = orig
        for m in set(sys.modules) - before:
            sys.modules.pop(m, None)
        return (rv == 1 and g.get("lock") == "pass" and "unlisted module tools/delink.py" in g.get("lock-imports", ""),
                "lock-imports (unlisted module tools/delink.py)")
    return {}, expect


def _c_relink(d):
    print("control relink: relink layer with control one-function (one listed function's final ret -> int3 in a copy)")
    return {"relink_control": "one-function"}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith("fail: .text digest ") and bool(v)  # 4.5 T16.2.c5 G13: .text,
        and " FAIL (1 differing byte offsets; " in g["relink"]  # the one ret byte; was: != pass, no `relink error`
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        "relink (.text digest FAIL, 1 differing byte; every fn row and other global pass)")


def _relink_refused(control, why, what, has=()):  # 5.4 T3 (G8): +has, substrings the refusal must also hold
    """3.4 T6.1: relink layer with relink control `control` -> rc 2, the relink global REFUSED with `why`."""
    print(f"control relink-{control}: relink layer with control {control} ({what})")
    return {"relink_control": control}, lambda r, v, g, i: (
        r == 2 and g.get("relink", "pass").startswith(f"fail: REFUSED {why}")
        and all(x in g.get("relink", "") for x in has) and bool(v)
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        f"relink (REFUSED {why}; every fn row and other global pass)")


def _c_relink_entry(d):
    return _relink_refused("entry", "our PE entry", "our first .text function to the head of /ORDER")


def _c_relink_literal(d):
    return _relink_refused("literal", "our ranges overlap", "a synthetic duplicate literal COMDAT, one byte flipped")


def _c_relink_fold(d):  # 4.3 T3: the fold-VA accept (C0198) must not swallow a differing copy
    return _relink_refused("fold", "our ranges overlap", "two synthetic fold-alias .text COMDAT copies, one byte flipped")


def _c_relink_alias(d):  # 5.4 T3 (G8): the fold-alias external resolve must need both func pins (fpins)
    return _relink_refused("alias", "our ", "an external at a matched row's start, pinned but not a fold alias",
                           ("external ?pa_control_alias@@3HA 0x", " lies in a replaced range"))


def _c_relink_samename(d):  # 4.4 T3 (C0215): the same-name row merge must not swallow a differing copy
    return _relink_refused("samename", "our ranges overlap",
                           "a synthetic same-name copy of a matched row's COMDAT, one byte flipped")


def _c_relink_samevt(d):  # 5.1 T4 (C0255 limit 1): a weak reloc followed in same() must not merge two names
    return _relink_refused("samevt", "our ranges overlap",
                           "two byte-identical synthetic vtable COMDATs under two names at one VA, slot 0 a weak ??_E")


def _c_relink_xdata_order(d):  # 5.1 T4 (C0255 limit 2): `.xdata$x` sorted by VA as `.rdata$<va8>`, checked there
    print("control relink-xdata-order: relink layer with control xdata-order (two synthetic .xdata$x sections, "
          "retail bytes between them, each holding the other's retail bytes)")
    return {"relink_control": "xdata-order"}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith("fail: .rdata digest ") and bool(v)
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        "relink (rc 1, .rdata digest FAIL; every fn row and other global pass)")


def _c_relink_data_order(d):  # 5.1 T13 (C0302 wall 2): one object's `.data` sorted by VA as `.data$<va8>`
    print("control relink-data-order: relink layer with control data-order (one synthetic object, two .data sections, "
          "retail bytes between them, each holding the other's retail bytes)")
    return {"relink_control": "data-order"}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith("fail: .data digest ") and bool(v)
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        "relink (rc 1, .data digest FAIL; every fn row and other global pass)")


def _c_relink_label_unref(d):  # 5.1 T14 (C0305): an unreferenced type-0 STATIC `$L` label of a code hole is not wanted
    print("control relink-label-unref: relink layer with control label-unref (a synthetic code hole of retail .text "
          "bytes, type-0 STATIC labels $Lctl_unref (no reloc) and $Lctl_ref (DIR32 target from a COMDAT LINK drops))")
    return {"relink_control": "label-unref"}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith("fail: ") and "$Lctl_ref linked absent " in g["relink"]
        and "$Lctl_unref" not in g["relink"] and "digest" not in g["relink"] and bool(v)
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        "relink (rc 1, $Lctl_ref linked absent FAIL, $Lctl_unref not named, no digest FAIL; every fn row and other "
        "global pass)")


def _c_relink_unit_key(d):  # 5.1 T14 (C0304): two matched-set units of one basename stem, keyed by full unit name
    print("control relink-unit-key: relink layer with control unit-key (a list copy moves a row to a second unit of "
          "the same basename stem; its object a copy with one byte of that row flipped; both objects carry a data "
          "section _link stages under ours/, else relink refuses, 5.1 T14.c3 C0309)")
    # was: "...the same basename stem; its object a copy with one byte of that row flipped)"
    return {"relink_control": "unit-key"}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith("fail: .text digest ") and bool(v)
        and " FAIL (1 differing byte offsets; " in g["relink"]
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        "relink (rc 1, .text digest FAIL, 1 differing byte; every fn row and other global pass)")


def _c_relink_site_past_buffer(d):  # 5.3 T2: a reloc field past its buffer -> refused naming it, not a struct.error
    return _relink_refused("site-past-buffer", "reloc field past its buffer",
                           "a cut's external reloc, its section raw cut to the field + 3 bytes")


def _c_relink_weak(d):  # 4.4 T3 (C0217): a weak external whose TagIndex names a wrong body -> the image differs
    print("control relink-weak: relink layer with control weak (a row's reloc -> a weak external, TagIndex a non-row "
          "COMDAT .text no reloc reaches; planted before relink._ours' kept walk, 4.5 T16.2.c4)")
    return {"relink_control": "weak"}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith("fail: .text digest") and bool(v)
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        "relink (rc 1, .text digest FAIL; every fn row and other global pass)")


def _c_relink_bare_exit(d):  # 3.5 T4 W1: a bare SystemExit() in the relink chain (relink.load_xbe) -> rc 2, not 0
    orig = relink.load_xbe

    def bare(*a, **k):
        print("relink: control bare-exit: SystemExit() next")  # 5.5 T12 (G16): its own cause in the REFUSED text
        raise SystemExit()
    relink.load_xbe = bare  # set last in make: restored by the runner's undo (finally)
    print("control relink-bare-exit: relink.load_xbe -> raise SystemExit() (code None)")
    # 5.5 T12 (G16): the refusal is this plant's; was: g["relink"].startswith("fail: REFUSED") (any refusal)
    return {"undo": lambda: setattr(relink, "load_xbe", orig)}, lambda r, v, g, i: (
        r == 2 and g.get("relink") == "fail: REFUSED control bare-exit: SystemExit() next",
        "relink (bare SystemExit -> REFUSED, its own line)")


def _c_relink_exit_zero(d):  # 4.5 T16.2 G12: sys.exit(0) in the relink chain (relink.load_xbe) -> rc 2, not 0
    orig = relink.load_xbe

    def zero(*a, **k):
        print("relink: control exit-zero: sys.exit(0) next")  # 5.5 T12 (G16): its own cause in the REFUSED text
        sys.exit(0)
    relink.load_xbe = zero  # set last in make: restored by the runner's undo (finally)
    print("control relink-exit-zero: relink.load_xbe -> sys.exit(0)")
    # 5.5 T12 (G16): the refusal is this plant's; was: g["relink"].startswith("fail: REFUSED") (any refusal)
    return {"undo": lambda: setattr(relink, "load_xbe", orig)}, lambda r, v, g, i: (
        r == 2 and g.get("relink") == "fail: REFUSED control exit-zero: sys.exit(0) next",
        "relink (SystemExit(0) -> REFUSED, its own line)")


def _c_relink_not_built(d):  # 3.5 T4 N7: a unit's build fails -> the relink layer is not run and fails
    old, new = _units_copy(d / "units.toml", LOC_UNIT, lambda o: o + " /Zq9")
    print(f"control relink-not-built: {LOC_UNIT} opt {old!r} -> {new!r}")
    return {"units_path": d / "units.toml"}, lambda r, v, g, i: (
        g.get("relink") == "fail: not run: build failed", "relink (not run: build failed)")


def _c_relink_no_funcs(d):  # 3.5 T4 N7: a list without functions -> relink `fail: no functions`
    kw, _e = _c_empty(d)
    print("control relink-no-funcs: list copy with no entries")
    return kw, lambda r, v, g, i: (g.get("relink") == "fail: no functions", "relink (no functions)")


def _relink_patched(name, fake, want, what):
    """3.5 T4 N7: relink.gate_check replaced by fake (restored by the runner's undo) -> relink fails `want...`, rc 1."""
    orig = relink.gate_check
    relink.gate_check = fake
    print(f"control relink-{name}: relink.gate_check -> {what}")
    return {"undo": lambda: setattr(relink, "gate_check", orig)}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith(want), f"relink ({want})")


def _c_relink_error(d):
    def boom(*a, **k):
        raise RuntimeError("gate control relink-error")
    return _relink_patched("error", boom, "fail: rc None: relink error RuntimeError", "raise RuntimeError")


def _c_relink_rc(d):
    return _relink_patched("rc", lambda *a, **k: (7, ["gate control relink-rc"]), "fail: rc 7: gate control",
                           "return rc 7")


def _c_relink_tag_sentinel(d, config=None):  # kw `direct` (5.5 T12, B1): a second gate_check under another tag
    # (precheck's pre_<TAG>) leaves the first tag's link dir alone: a sentinel in link/all=<TAG>/obj survives
    other = f"{TAG}-sentinel"
    print(f"control relink-tag-sentinel: fixture built, gate_check (tag {TAG}), a sentinel in link/all={TAG}/obj, "
          f"gate_check again (tag {other})")

    def undo():
        for ld in relink.OUT.glob(f"retail-*/link/all={other}"):
            shutil.rmtree(ld, ignore_errors=True)

    def expect(r, v, g, i):
        bd = d / "build"
        build.build(FIX_SRC, bd, build.UNITS, config)
        rc1, _l1 = relink.gate_check(FIX_LIST, bd, config)
        lds = sorted(relink.OUT.glob(f"retail-*/link/all={TAG}"))
        sent = lds[0] / "obj" / "pa_control_sentinel.txt" if len(lds) == 1 else None
        if sent is not None:
            sent.write_text("relink-tag-sentinel\n", encoding="utf-8")
        rc2, l2 = relink.gate_check(FIX_LIST, bd, config, tag=other)
        kept = sent is not None and sent.is_file()
        print(f"control relink-tag-sentinel: gate_check rc {rc1} / {rc2}; link dirs all={TAG} {len(lds)}; sentinel "
              f"{'kept' if kept else 'DELETED'}{'' if rc2 == 0 else '; second: ' + ' | '.join(l2[-3:])}")
        return rc1 == 0 and rc2 == 0 and kept, f"sentinel in link/all={TAG}/obj kept by gate_check tag {other}"
    return {"direct": True, "undo": undo}, expect


def _c_relink_site_opcode(d):  # 5.5 T12 (I54): a REL32 site whose retail opcode differs (Tesla 0x0008BFDD +4, retail
    # 8B mid-instruction, derived 0x0009D06B) -> refused at the site, not placed
    return _relink_refused("site-opcode", "retail opcode 8b != ours e8 before REL32 site 0x0008BFE1",
                           "Tesla's ??_G row planted in our first object: retail bytes, +3 -> E8, a REL32 at +4 into a "
                           "new 1 B COMDAT .text no name places")


def _c_relink_names_unpinned(d):  # 5.5 T12 (G10): relink names our externals from the unpinned derived table; a name
    # remapped there passes relink, and the bytes layer (SYMTABS) still fails the function
    s = _src_copy(d)
    _edit_text(s / SET_SRC, "    case STATE_V_FALLSIN:\n        NewCamera(s_001EA324);",
               "    case STATE_V_FALLSIN:\n        NewCamera(pa_ctl_name);")
    _edit_text(s / SET_SRC, 'extern "C" const char s_001EA324[],',
               'extern "C" const char pa_ctl_name[];\nextern "C" const char s_001EA324[],')
    pin = coff_info.pinned_sha()
    t5 = REPO / "derived" / "ghidra" / f"retail-{pin[:8]}"
    t5 = t5 / "applied" if (t5 / "applied" / "summary.json").is_file() else t5
    fn, sy = d / "functions.jsonl", d / "symbols.jsonl"
    shutil.copy(t5 / "functions.jsonl", fn)
    old = sy_text = (t5 / "symbols.jsonl").read_text(encoding="utf-8")
    sy_text = sy_text.replace('"name": "_s_001EA324",', '"name": "_pa_ctl_name",')
    if old.count('"name": "_s_001EA324",') != 1:
        raise RuntimeError(f"{t5 / 'symbols.jsonl'}: `_s_001EA324` rows != 1")
    sy.write_text(sy_text, encoding="utf-8", newline="\n")
    orig = relink._our_names
    relink._our_names = lambda cfg_path: byte_compare.setup(
        types.SimpleNamespace(symbols=[str(fn), str(sy)], xbe=None, config=cfg_path))[1]
    print(f"control relink-names-unpinned: {SET_SYM} pushes `_pa_ctl_name` for s_001EA324 (src copy); relink's names "
          f"from a scratch {t5.name}/symbols.jsonl with `_s_001EA324` -> `_pa_ctl_name` (0x001EA324)")
    return {"src_root": s, "undo": lambda: setattr(relink, "_our_names", orig)}, lambda r, v, g, i: (
        r == 1 and g.get("relink") == "pass" and v.get(SET_SYM, {}).get("bytes", "pass") != "pass",
        f"{SET_SYM} bytes (SYMTABS) with relink pass")


def _c_toolchain_split(d, config=None):  # 3.5 T4 F4/N10/N5: check_env runs on the gate's --config
    (d / "no-msvc").mkdir()  # exists (vv_inputs refuses a missing path) but holds no pinned toolchain
    print("control toolchain-split: config copy, msvc71_dir -> an empty dir")
    return {"config": _cfg_copy(d, config, "msvc71_dir", d / "no-msvc")}, lambda r, v, g, i: (
        g.get("check_env", "").startswith("fail: rc 1") and  # 4.5 T16.2.c5 G13: + the msvc cause; was: rc 1 only
        f"FAIL pins: msvcr71.dll: missing ({d / 'no-msvc' / 'msvcr71.dll'})" in g["check_env"],
        "check_env (the gate's config: msvc71_dir's pinned msvcr71.dll missing)")


def _c_audit_env(d):  # 3.5 T4 W6: AUDIT_PUBLIC_* in the gate's env must not steer the audit layer
    m = d / "env" / "inputs.manifest.jsonl"
    m.parent.mkdir()
    m.write_text("", encoding="utf-8")
    kw, expect = _c_audit(d)
    old = os.environ.get("AUDIT_PUBLIC_MANIFEST")

    def undo():
        if old is None:
            os.environ.pop("AUDIT_PUBLIC_MANIFEST", None)
        else:
            os.environ["AUDIT_PUBLIC_MANIFEST"] = old
    os.environ["AUDIT_PUBLIC_MANIFEST"] = str(m)  # set last in make: restored by the runner's undo (finally)
    print("control audit-env: the audit plant with AUDIT_PUBLIC_MANIFEST -> an empty manifest in the gate's env")
    return {**kw, "run_audit": True, "undo": undo}, expect


def _c_audit_null_key(d, config=None):  # 4.1 T3.1: a null value class outside a public tree fails the audit row
    t = d / "clean.txt"
    t.write_text("clean\n", encoding="utf-8")
    print("control audit-null-key: config copy, xbox_ip null; marker root an empty dir; a clean file via --paths")
    return {"config": _cfg_null(d, config, ["xbox_ip"]), "marker_root": d, "audit_args": ("--paths", str(t)),
            "run_audit": True}, lambda r, v, g, i: (
        g["audit_public"].startswith("fail: not checked: ") and "address" in g["audit_public"],
        "audit_public (not checked: address)")


def _c_audit_marker_contradiction(d, config=None):  # 4.5 T16.1 G6: PUBLIC_TREE beside .claude/pa.json is private
    for p in ("config/" + audit_public.PUBLIC_MARKER, ".claude/pa.json"):
        (d / p).parent.mkdir(parents=True, exist_ok=True)
        (d / p).write_text("{}\n" if p.endswith(".json") else "", encoding="utf-8", newline="\n")
    t = d / "clean.txt"
    t.write_text("clean\n", encoding="utf-8")
    print("control audit-marker-contradiction: config copy, value keys null; marker root holds config/"
          f"{audit_public.PUBLIC_MARKER} and .claude/pa.json; a clean file via --paths")
    return {"config": _cfg_null(d, config, list(audit_public.VALUE_KEYS.values())), "marker_root": d,
            "audit_args": ("--paths", str(t)), "run_audit": True}, lambda r, v, g, i: (
        g.get("audit_public", "").startswith("fail: PUBLIC_TREE marker in a private tree"),
        "audit_public (marker in a private tree)")


def _c_audit_short_blob(d):  # 4.5 T16.1 G7: a truncated cat-file stream (and rc 1) must refuse, never yield
    oid = "0" * 40
    real = audit_public.subprocess.Popen

    class ShortCat:  # cat-file --batch announcing 100 bytes, sending 10, exiting 1
        def __init__(self, *a, **k):
            self.stdin, self.stdout, self.returncode = io.BytesIO(), io.BytesIO(f"{oid} blob 100\n".encode() + b"x" * 10), None

        def wait(self):
            self.returncode = 1
            return 1

    def undo():
        audit_public.subprocess.Popen = real

    def expect(r, v, g, i):
        audit_public.subprocess.Popen = ShortCat
        try:
            got = list(audit_public._cat_blobs([oid], d, None))
            why = f"yielded {[(o, len(b)) for o, b in got]}"
        except SystemExit as e:
            why = f"exit {e.code}"
        finally:
            undo()
        print(f"control audit-short-blob: _cat_blobs on a 10-of-100-byte stream, rc 1: {why}")
        return why == "exit 2", "audit_public._cat_blobs (refused, exit 2)"
    print("control audit-short-blob: audit_public.subprocess.Popen -> a short cat-file stream")
    return {"direct": True, "undo": undo}, expect


def _c_audit_gzip_disasm(d):  # 4.5 T16.1 G8: a tracked gzip blob is classified on its decompressed bytes
    repo, rel = d / "repo", ".claude-state/ledger/x.jsonl.gz"
    env = {k: v for k, v in os.environ.items() if not k.startswith("GIT_")}
    cfgs = ["-c", "user.name=control", "-c", "user.email=control@invalid", "-c", "commit.gpgsign=false",
            "-c", "core.hooksPath=", "-c", "core.autocrlf=false"]
    (repo / rel).parent.mkdir(parents=True)
    (repo / rel).write_bytes(gzip.compress(audit_public._fixtures()["asm.txt"][0], mtime=0))  # generated, no run here
    for a in (["init", "-q"], ["add", rel], ["commit", "-q", "--no-verify", "-m", "plant"]):
        audit_public._git([*cfgs, *a], repo, env)

    def expect(r, v, g, i):
        got = {s: {(c, p) for c, p, _o, _m in audit_public.audit(s, repo, env, set())[1]} for s in ("worktree", "history")}
        print(f"control audit-gzip-disasm: hits {got}")
        return all(("text-run", rel) in h for h in got.values()), f"audit_public text-run {rel} (worktree, history)"
    print(f"control audit-gzip-disasm: scratch repo, {rel} tracked holding gzipped disasm lines")
    return {"direct": True, "undo": lambda: shutil.rmtree(repo, onexc=lambda f, p, e: (os.chmod(p, 0o700), f(p)))}, \
        expect


def _c_audit_utf16(d, config=None):  # 5.5 T14.c1: a UTF-16LE-BOM blob is classified on its UTF-8 transcoding
    ip = vv_inputs.load_config(config).get("xbox_ip") or "192.0.2.1"  # the local value, else RFC 5737; never printed
    one = ip if isinstance(ip, str) else ip[0]
    run, val = audit_public._fixtures()["asm.txt"][0], f"host {one} here\n".encode()  # generated, no run here
    u16 = lambda b: b"\xff\xfe" + b.decode("ascii").encode("utf-16-le")  # noqa: E731
    want = {("text-run", "run16.txt"), ("text-run", "run.txt"), ("address", "val16.txt"), ("address", "val.txt")}
    repo, acc = d / "repo", d / "accept.txt"
    env, _git = _scratch_repo(repo, {"run16.txt": u16(run), "run.txt": run, "val16.txt": u16(val), "val.txt": val})
    acc.write_text("# none\n", encoding="ascii")

    def expect(r, v, g, i):
        rules = {"address": audit_public.address_rule(ip, acc, repo, env)}
        got = {s: {(c, p) for c, p, _o, _m in audit_public.audit(s, repo, env, set(), rules=rules)[1]}
               for s in ("worktree", "history")}
        print(f"control audit-utf16: hits {got}")
        return all(h == want for h in got.values()), "audit_public text-run and address, UTF-16 and ASCII (worktree, history)"
    print("control audit-utf16: scratch repo, generated disasm and an address line, each UTF-16LE-BOM and ASCII")
    return {"direct": True, "undo": lambda: _rmtree_ro(repo)}, expect


def _c_audit_archive(d):  # 5.5 T14.c1: zip and 7z magics at offset 0 are a magic hit; a clean text blob is not
    files = {"a.zip": b"PK\x03\x04" + b"\x14\x00" * 20, "a.7z": b"7z\xbc\xaf\x27\x1c" + b"\x00\x04" * 20,
             "clean.txt": b"plain text\n"}
    repo = d / "repo"
    env, _git = _scratch_repo(repo, files)
    want = {("magic", "a.zip"), ("magic", "a.7z")}

    def expect(r, v, g, i):
        got = {s: {(c, p) for c, p, _o, _m in audit_public.audit(s, repo, env, set())[1]} for s in ("worktree", "history")}
        print(f"control audit-archive: hits {got}")
        return all(h == want for h in got.values()), "audit_public magic a.zip, a.7z; clean.txt none (worktree, history)"
    print("control audit-archive: scratch repo, a zip-magic, a 7z-magic and a clean text blob tracked")
    return {"direct": True, "undo": lambda: _rmtree_ro(repo)}, expect


HOOKS = REPO / "githooks"  # 5.5 T14.c2: the real hooks, run by the hook-* controls in scratch trees
# 5.5 T32: the value keys of a hook tree's config, fake (path keys stay the running config's: load_config, COVER)
HOOK_VALUES = {"xbox_ip": "192.0.2.1", "boot_folder": r"E:\VVHookCtl\VVHookCtlBoot",
               "original_install": r"C:\VVHookCtl\VVHookCtlOrig"}


def _commitmsg_public():
    """5.5 T32: True in a public tree without githooks/commit-msg (not shipped, export/allowlist.txt). A private tree
    missing it is never skipped."""
    return (REPO / "config" / "PUBLIC_TREE").is_file() and not (HOOKS / "commit-msg").is_file()


def _hook_tree(d, config, files):
    """5.5 T14.c2: scratch private tree d/repo whose core.hooksPath is the real githooks/: copies of tools/*.py (the
    hooks call $ROOT/tools), the running config with its value keys set to HOOK_VALUES (5.5 T32; was: the local
    config copied), the manifest and check.*.sha copied in, `# none` accept lists,
    .claude/pa.json naming this Python (C0177, C0006); files {rel: bytes} committed; bare d/remote.git as origin, its
    URL in vv.privateRemote. -> (repo, env, url, git(*args, env=, inp=, cwd=) -> (rc, stdout+stderr))."""
    repo, remote = d / "repo", d / "remote.git"
    env = {k: v for k, v in os.environ.items() if not k.startswith(("GIT_", "AUDIT_PUBLIC_"))}
    env["PY"] = Path(sys.executable).as_posix()

    def git(*a, env=env, inp=b"", cwd=repo):  # stdin as bytes (C0162)
        r = subprocess.run(["git", *a], cwd=cwd, env=env, input=inp, capture_output=True)
        return r.returncode, (r.stdout + r.stderr).decode("utf-8", "replace")
    repo.mkdir(parents=True)
    rc, _o = git("init", "-q")
    rc2, top = git("rev-parse", "--show-toplevel")
    if rc or rc2 or Path(top.strip()).resolve() != repo.resolve():  # C0177: never configure the real repo
        raise RuntimeError(f"scratch repo is not its own top (rc {rc}, {rc2})")
    for k, v in (("user.name", "control"), ("user.email", "control@invalid"), ("commit.gpgsign", "false"),
                 ("core.autocrlf", "false"), ("core.hooksPath", HOOKS.as_posix())):
        git("config", k, v)
    (repo / "tools").mkdir()
    for p in (REPO / "tools").glob("*.py"):
        shutil.copyfile(p, repo / "tools" / p.name)
    conf = repo / "config"
    conf.mkdir()
    raw = json.loads((Path(config) if config else vv_inputs.CONFIG).read_text(encoding="utf-8"))
    (conf / "local_paths.json").write_text(json.dumps({**raw, **HOOK_VALUES}), encoding="utf-8")
    for p in [*(REPO / "config").glob("inputs.manifest.*"), *(REPO / "config").glob("check.*.sha")]:
        shutil.copyfile(p, conf / p.name)
    for n in ("audit_address_accepted.txt", "audit_private_accepted.txt"):
        (conf / n).write_bytes(b"# none\n")
    (repo / ".claude").mkdir()
    (repo / ".claude" / "pa.json").write_text(json.dumps({"python": env["PY"]}), encoding="utf-8")
    for rel, data in files.items():
        (repo / rel).parent.mkdir(parents=True, exist_ok=True)
        (repo / rel).write_bytes(data)
    url = remote.as_posix()
    for a in (("add", *files), ("commit", "-q", "--no-verify", "-m", "base")):
        rc, o = git(*a)
        if rc:
            raise RuntimeError(f"scratch {a[0]} rc {rc}: {o[-200:]}")
    git("init", "-q", "--bare", str(remote), cwd=d)
    git("remote", "add", "origin", url)
    git("config", "vv.privateRemote", url)
    return repo, env, url, git


def _hook_sh():
    """Git's own sh (`bash`/`sh` on PATH from Python on Windows may be WSL's, C0177) -> (sh, its usr/bin, mingw64/bin)."""
    root = Path(subprocess.run(["git", "--exec-path"], capture_output=True, text=True).stdout.strip()).parents[2]
    sh = root / "usr" / "bin" / "sh.exe"
    if not sh.is_file():
        sh = Path(shutil.which("sh"))
    return sh, sh.parent, root / "mingw64" / "bin"


def _hooks_direct(repo, url, env, sh):
    """The three real hooks run directly in repo (b.txt staged, a message file, one new-ref push line) -> {hook: (rc,
    output)}. 5.5 T32: the commit-msg leg skipped in a public tree without it (_commitmsg_public)."""
    (repo / "b.txt").write_bytes(b"staged\n")
    subprocess.run(["git", "add", "b.txt"], cwd=repo, env=env, capture_output=True)
    msg = repo / ".git" / "COMMIT_EDITMSG"
    msg.write_bytes(b"T0: hook control message\n")
    sha = subprocess.run(["git", "rev-parse", "HEAD"], cwd=repo, env=env, capture_output=True).stdout.decode().strip()
    out = {}
    skip = _commitmsg_public()
    if skip:
        print("hooks: commit-msg leg skipped (public tree: githooks/commit-msg is not shipped, export/allowlist.txt)")
    for h, args, inp in (("pre-commit", [], b""), ("commit-msg", [msg.as_posix()], b""),
                         ("pre-push", ["origin", url], f"refs/heads/main {sha} refs/heads/main {'0' * 40}\n".encode())):
        if skip and h == "commit-msg":
            continue
        r =subprocess.run([str(sh), (HOOKS / h).as_posix(), *args], cwd=repo, env=env, input=inp, capture_output=True)
        out[h] = (r.returncode, (r.stdout + r.stderr).decode("utf-8", "replace"))
    return out


def _c_hook_prepush_remote(d, config=None):  # 5.5 T14.c2: a private tree pushes only to vv.privateRemote's URL
    repo, env, url, git = _hook_tree(d, config, {"a.txt": b"clean\n"})
    other = d / "other.git"
    git("init", "-q", "--bare", str(other), cwd=d)
    git("remote", "add", "other", other.as_posix())
    msg = "not the URL set in git config vv.privateRemote"

    def expect(r, v, g, i):
        res = {"key, origin": git("push", "-q", "origin", "HEAD:refs/heads/main")}
        res["key, other"] = git("push", "-q", "other", "HEAD:refs/heads/main")
        git("config", "--unset", "vv.privateRemote")
        res["no key, origin"] = git("push", "-q", "origin", "HEAD:refs/heads/b2")
        git("config", "vv.privateRemote", "")
        res["empty key, origin"] = git("push", "-q", "origin", "HEAD:refs/heads/b3")
        (repo / "config" / "PUBLIC_TREE").write_bytes(b"# public tree (hook control)\n")
        (repo / ".claude" / "pa.json").unlink()
        res["public tree, empty key, other"] = git("push", "-q", "other", "HEAD:refs/heads/pub")
        clean = ("key, origin", "public tree, empty key, other")
        leak = [k for k, (_rc, o) in res.items() for ln in o.splitlines()
                if ln.startswith("pre-push:") and (url in ln or other.as_posix() in ln)]
        print(f"control hook-prepush-remote: rc {({k: rc for k, (rc, _o) in res.items()})}; block message "
              f"{({k: msg in o for k, (_rc, o) in res.items() if k not in clean})}; URL in a hook line: {leak}")
        ok = all(res[k][0] == 0 for k in clean) and not leak and all(
            rc != 0 and msg in o for k, (rc, o) in res.items() if k not in clean)
        return ok, "githooks/pre-push remote pin (other URL, no key, empty key blocked; public tree free)"
    print("control hook-prepush-remote: scratch private tree, origin = vv.privateRemote, a second bare remote")
    return {"direct": True, "undo": lambda: _rmtree_ro(d)}, expect


HOOK_FAKE = b"void f()\n{\n    // !FAKE: hook control\n    int a = 1;\n}\n"


def _c_hook_prepush_fakediff(d, config=None):  # 5.5 T14.c2: a pushed commit removing `// !FAKE:` alone blocks
    repo, env, url, git = _hook_tree(d, config, {"src/a.cpp": HOOK_FAKE})
    a = repo / "src" / "a.cpp"

    def commit(data, m):
        a.write_bytes(data)
        rc, o = git("commit", "-q", "-am", m)
        if rc:
            raise RuntimeError(f"scratch commit {m!r} rc {rc}: {o[-300:]}")

    def expect(r, v, g, i):
        commit(HOOK_FAKE.replace(b"    // !FAKE: hook control\n", b"").replace(b"= 1", b"= 2"), "clean: code changed")
        res = {"clean, new ref (whole history)": git("push", "-q", "origin", "HEAD:refs/heads/main")}
        commit(HOOK_FAKE.replace(b"= 1", b"= 2"), "fake line back")
        res["clean, range"] = git("push", "-q", "origin", "HEAD:refs/heads/main")
        commit(HOOK_FAKE.replace(b"    // !FAKE: hook control\n", b"").replace(b"= 1", b"= 2"), "plant: fake alone")
        res["plant, range"] = git("push", "-q", "origin", "HEAD:refs/heads/main")
        res["plant, new ref (whole history)"] = git("push", "-q", "origin", "HEAD:refs/heads/new")
        want = "FAIL fake-diff src/a.cpp:3"
        print(f"control hook-prepush-fakediff: rc {({k: rc for k, (rc, _o) in res.items()})}; "
              f"FAIL line {({k: want in o for k, (_rc, o) in res.items()})}")
        ok = all(rc == 0 and want not in o for k, (rc, o) in res.items() if k.startswith("clean")) and all(
            rc != 0 and want in o for k, (rc, o) in res.items() if k.startswith("plant"))
        return ok, "githooks/pre-push fake-diff (range and whole history)"
    print("control hook-prepush-fakediff: scratch private tree, src/a.cpp with a `// !FAKE:` line in f()")
    return {"direct": True, "undo": lambda: _rmtree_ro(d)}, expect


def _c_hook_commitmsg_audit(d, config=None):  # 5.5 T14.c2: commit-msg audits every message
    one = HOOK_VALUES["xbox_ip"]  # 5.5 T32: the tree's fake value (was: the local value, never printed)
    repo, env, url, git = _hook_tree(d, config, {"a.txt": b"clean\n"})

    def commit(n, m):
        (repo / f"m{n}.txt").write_bytes(b"clean\n")
        git("add", f"m{n}.txt")
        head = git("rev-parse", "HEAD")[1]
        rc, o = git("commit", "-q", "-m", m)
        return rc, o, git("rev-parse", "HEAD")[1] != head

    def expect(r, v, g, i):
        res = {"clean": commit(1, "T0: clean message"), "plant": commit(2, f"T0: box at {one} here")}
        want = "commit-msg: audit_public.py --paths on the message exit 1"
        print(f"control hook-commitmsg-audit: (rc, block line, HEAD moved) "
              f"{({k: (rc, want in o, moved) for k, (rc, o, moved) in res.items()})}")
        (rc, o, moved), (prc, po, pmoved) = res["clean"], res["plant"]
        ok = rc == 0 and moved and want not in o and prc != 0 and want in po and "HIT paths address" in po \
            and not pmoved
        return ok, "githooks/commit-msg audit of the message (address plant)"
    print("control hook-commitmsg-audit: scratch private tree, a clean message, then one holding the tree's address")
    return {"direct": True, "undo": lambda: _rmtree_ro(d)}, expect


def _c_hook_classes_off(d, config=None):  # 5.5 T14.c2 (S3): a private tree's hooks ignore the audit overrides
    repo, env, url, git = _hook_tree(d, config, {"a.txt": b"clean\n"})
    local = repo / "config" / "local_paths.json"
    full, off = d / "full.json", d / "off.json"
    shutil.copyfile(local, full)
    off.write_text(json.dumps({**json.loads(local.read_text(encoding="utf-8")), "xbox_ip": None}), encoding="utf-8")
    man = (repo / "config" / "inputs.manifest.jsonl").as_posix()
    sh = _hook_sh()[0]

    def expect(r, v, g, i):
        res = {"clean: local full, overrides off + bad manifest": _hooks_direct(repo, url, dict(
            env, AUDIT_PUBLIC_CONFIG=off.as_posix(), AUDIT_PUBLIC_MANIFEST=(d / "none.jsonl").as_posix()), sh)}
        local.unlink()
        res["clean: no local config, override full"] = _hooks_direct(repo, url, dict(
            env, AUDIT_PUBLIC_CONFIG=full.as_posix(), AUDIT_PUBLIC_MANIFEST=man), sh)
        shutil.copyfile(off, local)
        res["plant: local off, override full"] = _hooks_direct(repo, url, dict(
            env, AUDIT_PUBLIC_CONFIG=full.as_posix(), AUDIT_PUBLIC_MANIFEST=man), sh)
        print(f"control hook-classes-off: (rc, `not checked` in output) "
              f"{({k: {h: (rc, 'not checked' in o) for h, (rc, o) in hs.items()} for k, hs in res.items()})}")
        ok = all(rc == 0 and "not checked" not in o for k, hs in res.items() if k.startswith("clean")
                 for rc, o in hs.values()) and all(
            rc != 0 and "not checked" in o for rc, o in res["plant: local off, override full"].values())
        return ok, "githooks/pre-commit, commit-msg, pre-push: overrides unset, `not checked` blocks"
    print("control hook-classes-off: scratch private tree; the three hooks run directly under AUDIT_PUBLIC_* overrides")
    return {"direct": True, "undo": lambda: _rmtree_ro(d)}, expect


def _c_hook_python_lookup(d, config=None):  # 5.5 T14.c2 (C5): a Python counts only when it prints ok
    repo, env, url, git = _hook_tree(d, config, {"a.txt": b"clean\n"})
    fb = d / "fakebin"
    fb.mkdir()
    fake = fb / "python"
    fake.write_bytes(b"#!/bin/sh\nexit 0\n")  # runs, prints nothing
    (repo / ".claude" / "pa.json").write_text(json.dumps({"python": fake.as_posix()}), encoding="utf-8")
    sh, usr_bin, mingw_bin = _hook_sh()
    path = os.pathsep.join(str(p) for p in (fb, usr_bin, mingw_bin))  # git and sh stay on PATH (C0162); no Python

    def expect(r, v, g, i):
        base = dict(env, PATH=path)
        res = {"clean: env PY real": _hooks_direct(repo, url, base, sh),
               "plant: env PY, pa.json, PATH python all non-printing": _hooks_direct(
                   repo, url, dict(base, PY=fake.as_posix()), sh)}
        print(f"control hook-python-lookup: (rc, no-Python message) "
              f"{({k: {h: (rc, 'no Python runs' in o) for h, (rc, o) in hs.items()} for k, hs in res.items()})}")
        (cl, pl) = res.values()
        ok = all(rc == 0 for rc, _o in cl.values()) and all(
            rc != 0 and f"{h}: no Python runs" in o for h, (rc, o) in pl.items())
        return ok, "githooks/pre-commit, commit-msg, pre-push: non-printing Pythons skipped, no Python blocks"
    print("control hook-python-lookup: scratch private tree; the three hooks run directly, PATH = a fake python + git")
    return {"direct": True, "undo": lambda: _rmtree_ro(d)}, expect


def _scratch_repo(repo, files, commits=1):
    """4.5 T16.2: a scratch git repo holding files {rel: bytes}, committed `commits` times -> (env, git(args, cwd))."""
    env = {k: v for k, v in os.environ.items() if not k.startswith("GIT_")}
    cfgs = ["-c", "user.name=control", "-c", "user.email=control@invalid", "-c", "commit.gpgsign=false",
            "-c", "core.hooksPath=", "-c", "core.autocrlf=false"]
    repo.mkdir(parents=True)
    for rel, data in files.items():
        (repo / rel).write_bytes(data)
    git = lambda a, cwd=repo: audit_public._git([*cfgs, *a], cwd, env)  # noqa: E731
    git(["init", "-q"])
    git(["add", *files])
    for n in range(commits):
        git(["commit", "-q", "--no-verify", "--allow-empty", "-m", f"plant {n}"])
    return env, git


def _rmtree_ro(p):  # git object files are read-only on Windows
    shutil.rmtree(p, onexc=lambda f, q, e: (os.chmod(q, 0o700), f(q)))


def _refusal(fn, *args):
    """'exit <code>: <stderr>' when fn(*args) exits, else 'returned'; stderr captured."""
    err = io.StringIO()
    try:
        with contextlib.redirect_stderr(err):
            fn(*args)
    except SystemExit as e:
        return f"exit {e.code}: {err.getvalue().strip()}"
    return "returned"


def _c_audit_renamed_blob(d):  # 4.5 T16.2 G10: one blob at two paths, one with a tool suffix -> toolchain hit there
    repo, tool = d / "repo", "b.exe"
    env, _git = _scratch_repo(repo, {"a.txt": b"plain text\n", tool: b"plain text\n"})

    def expect(r, v, g, i):
        got = {(c, p) for c, p, _o, _m in audit_public.audit("history", repo, env, set())[1]}
        print(f"control audit-renamed-blob: history hits {got}")
        return ("toolchain", tool) in got, f"audit_public toolchain {tool} (history)"
    print(f"control audit-renamed-blob: scratch repo, one blob at a.txt and {tool}")
    return {"direct": True, "undo": lambda: _rmtree_ro(repo)}, expect


def _audit_cache(d, name, warm, change, want_rc, want_key):
    """kw `direct` (5.4 T5.c5): a scratch repo with a planted forbidden blob; a cache with the right key and a forged
    clean entry for the plant; change ('accepted', 'fp' or None) edits a key input after; audit_public.run on the history
    scope with the cache, warm or cold -> want_rc and `(key <want_key>)` on stderr."""
    repo, plant, cpath = d / "repo", b"control cache plant\n", d / "history_cache.json"
    env, _git = _scratch_repo(repo, {"a.txt": b"plain text\n", "p.txt": plant})
    forbidden = {hashlib.sha1(plant).hexdigest()}
    key = audit_public.history_key(repo, forbidden, {})
    forged = sorted([f"{audit_public._blob_id(plant)} p.txt"])
    heads = audit_public._git(["rev-list", "--no-walk", "--all"], repo, env).decode().split()  # 5.5 T12.c3 (I73 F3):
    # forged with this repo's tips, so the plant's commit is under the recorded heads (the trust boundary kept)
    cpath.write_text(json.dumps({"key": key, "clean": forged, "binary": [], "heads": sorted(heads)}), encoding="ascii")
    (repo / "config").mkdir()
    if change == "accepted":  # untracked: the history is unchanged, only a key input
        (repo / "config" / audit_public.ACCEPT.name).write_text("# changed\n", encoding="ascii")
    elif change == "fp":
        (repo / "config" / "verifier.sha256").write_text("0" * 64 + "  tools/audit_public.py\n", encoding="ascii")

    def expect(r, v, g, i):
        out, err = io.StringIO(), io.StringIO()
        cache = audit_public.HistoryCache(cpath, audit_public.history_key(repo, forbidden, {}), warm)
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            rc = audit_public.run([("history", None)], forbidden, repo, env, {}, cache)
        stats = (err.getvalue().strip().splitlines() or [""])[-1]  # the `not checked` lines before it are dropped
        print(f"control {name}: rc {rc}; {stats}")
        # 5.5 T12.c3 (G16): the whole stats line and, on rc 1, the plant's own HIT; was: rc + the `(key S)` suffix
        hit = f"HIT history forbidden-hash p.txt blob {audit_public._blob_id(plant)}"
        return (rc == want_rc and stats == f"audit: history cache {int(want_key == 'ok')} of 2 (key {want_key})"
                and (hit in out.getvalue()) == (want_rc == 1)), \
            f"audit_public history with the cache (rc {want_rc}, key {want_key}, plant HIT {want_rc == 1})"
    print(f"control {name}: scratch repo, planted forbidden blob, cache with a forged clean entry"
          f"{f', {change} changed' if change else ''}, {'warm' if warm else 'cold'}")
    return {"direct": True, "undo": lambda: _rmtree_ro(repo)}, expect


def _c_audit_cache_forged_cold(d):  # 5.4 T5.c5: a cold run never reads the cache: the forged entry still HITs
    return _audit_cache(d, "audit-cache-forged-cold", False, None, 1, "none")


def _c_audit_cache_warm_reads(d):  # 5.4 T5.c5: warm reads it: the forged entry passes (the trust boundary)
    return _audit_cache(d, "audit-cache-warm-reads", True, None, 0, "ok")


def _c_audit_cache_key_accepted(d):  # 5.4 T5.c5: a changed accept list changes the key: a full miss, HIT
    return _audit_cache(d, "audit-cache-key-accepted", True, "accepted", 1, "changed")


def _c_audit_cache_key_fp(d):  # 5.4 T5.c5: a changed fingerprint line changes the key: a full miss, HIT
    return _audit_cache(d, "audit-cache-key-fp", True, "fp", 1, "changed")


def _c_audit_cache_newer_commit(d):  # kw `direct` (5.5 T12.c3, I73 F3): a cold run writes the cache (heads recorded);
    # then a forbidden blob is committed and a clean pair forged for it under the right key -> warm rc 1 (a blob
    # reachable only from a commit not under the recorded heads is read, never skipped)
    repo, plant, cpath = d / "repo", b"control cache plant\n", d / "history_cache.json"
    env, git = _scratch_repo(repo, {"a.txt": b"plain text\n"})
    forbidden = {hashlib.sha1(plant).hexdigest()}
    print("control audit-cache-newer-commit: scratch repo, cold run writes the cache; p.txt (forbidden) committed "
          "after; a clean pair forged for it; warm run")

    def audit(warm):
        out, err = io.StringIO(), io.StringIO()
        cache = audit_public.HistoryCache(cpath, audit_public.history_key(repo, forbidden, {}), warm)
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            rc = audit_public.run([("history", None)], forbidden, repo, env, {}, cache)
        return rc, (err.getvalue().strip().splitlines() or [""])[-1], out.getvalue()

    def expect(r, v, g, i):
        rc0, s0, _o = audit(False)
        (repo / "p.txt").write_bytes(plant)
        git(["add", "p.txt"])
        git(["commit", "-q", "--no-verify", "-m", "after the cache"])
        doc = json.loads(cpath.read_text(encoding="ascii"))
        doc["clean"] = sorted(doc["clean"] + [f"{audit_public._blob_id(plant)} p.txt"])
        cpath.write_text(json.dumps(doc), encoding="ascii")
        rc1, s1, o1 = audit(True)
        hit = f"HIT history forbidden-hash p.txt blob {audit_public._blob_id(plant)}" in o1
        print(f"control audit-cache-newer-commit: cold rc {rc0}; {s0}; warm rc {rc1}; {s1}; plant HIT {hit}; "
              f"recorded heads {len(doc['heads']) if 'heads' in doc else 'absent'}")  # a count: the sha varies (golden)
        return (rc0 == 0 and rc1 == 1 and hit and s1.endswith("(key ok)"),
                "audit_public warm history (forged pair for a post-write commit: rc 1, plant HIT, key ok)")
    return {"direct": True, "undo": lambda: _rmtree_ro(repo)}, expect


def _c_audit_shallow(d):  # 4.5 T16.2 G11: the history scope on a shallow clone refuses
    repo, shallow = d / "repo", d / "shallow"
    env, git = _scratch_repo(repo, {"a.txt": b"plain text\n"}, commits=2)
    git(["clone", "-q", "--depth", "1", repo.as_uri(), str(shallow)], d)
    is_shallow = audit_public._git(["rev-parse", "--is-shallow-repository"], shallow, env).strip().decode()

    def expect(r, v, g, i):
        why = _refusal(audit_public.audit, "history", shallow, env, set())
        print(f"control audit-shallow: history scope on a clone (shallow {is_shallow}): {why}")
        return is_shallow == "true" and why.startswith("exit 2: ") and "shallow clone" in why, \
            "audit_public history (refused, exit 2, shallow clone)"
    print("control audit-shallow: scratch repo of 2 commits, cloned --depth 1")
    return {"direct": True, "undo": lambda: (_rmtree_ro(repo), _rmtree_ro(shallow))}, expect


def _c_audit_paths_empty(d):  # 4.5 T16.2 G19: `--paths` with no file refuses, never an empty OK scan
    def expect(r, v, g, i):
        why = _refusal(audit_public.parse, ["--paths"])
        print(f"control audit-paths-empty: parse(['--paths']): {why}")
        return why.startswith("exit 2: ") and "--paths needs" in why, "audit_public --paths (refused, exit 2)"
    print("control audit-paths-empty: audit_public argv `--paths` with no file")
    return {"direct": True}, expect


def _c_audit_route_pin(d):  # 4.5 T16.2 G14: an empty literal from _address_rx -> the route pin names it
    orig = audit_public._address_rx

    def unsound(ips):
        rx, literals, fold = orig(ips)
        return rx, (*literals, b""), fold

    def undo():
        audit_public._address_rx = orig

    def expect(r, v, g, i):
        audit_public._address_rx = unsound
        try:
            causes = audit_public._route_pin()
        finally:
            undo()
        print(f"control audit-route-pin: _route_pin() with _address_rx giving an empty literal: {causes}")
        return "_address_rx gives no literal" in causes, "audit_public._route_pin (_address_rx gives no literal)"
    print("control audit-route-pin: audit_public._address_rx -> its literals plus an empty one")
    return {"direct": True, "undo": undo}, expect


def _c_audit_tail(d, config=None):  # 4.1 T3.1: `not checked` lines must not crowd a HIT out of the verdict
    kw, _expect = _c_audit(d)
    p = d / "probe.obj"
    print("control audit-tail: the audit plant with all three value keys null")
    return {**kw, "config": _cfg_null(d, config, audit_public.VALUE_KEYS.values()), "run_audit": True}, \
        lambda r, v, g, i: (g["audit_public"].startswith("fail: rc 1: ") and f"HIT paths magic {p}" in g["audit_public"]
                            and "6 not checked lines dropped" in g["audit_public"],
                            "audit_public (rc 1, HIT kept, 6 lines dropped)")


def _audit_mutant(d, name, old, new, why):
    """kw + expect: the audit run from a scratch tools dir with one mutated line must refuse, naming the route pin."""
    tools = d / "tools"
    tools.mkdir()
    shutil.copy(TOOLS / "vv_inputs.py", tools / "vv_inputs.py")
    src = (TOOLS / "audit_public.py").read_text(encoding="utf-8")
    if src.count(old) != 1:
        raise RuntimeError(f"{name}: mutation text occurs {src.count(old)} times")
    (tools / "audit_public.py").write_text(src.replace(old, new), encoding="utf-8", newline="\n")
    print(f"control {name}: audit run from a scratch copy with one line mutated")
    return {"audit_tools": tools, "run_audit": True}, lambda r, v, g, i: (
        g["audit_public"].startswith("fail: rc 2: ") and "route pin" in g["audit_public"] and why in g["audit_public"],
        f"audit_public (rc 2, route pin: {why})")


def _c_audit_line_pin(d):  # 4.1 T3.1: as audit_public's mutant; the newline leaves its self-test's copy of the text out
    line = "return bool(HEX_RE.search(line) or HEXESC_RE.search(line))"
    return _audit_mutant(d, "audit-line-pin", line + "\n", line[:-1] + " or OVERSIZE < 0)\n", "_is_hex reads a name")


def _c_audit_literal_pin(d):  # 4.1 T3.1: as audit_public's mutant
    line = 'return _literal_rx([name.encode()], rb"(?<![A-Za-z0-9_])", rb"(?![A-Za-z0-9_])", re.I)'
    return _audit_mutant(d, "audit-literal-pin", line, line + "[0], (), True", "_name_rx gives no literal")


def _prov(checks, kw, at, why=None):
    """kw + expect: `provenance` fails (not refused) and the lint reported a FAIL line of each check in checks at
    at[check] (`<path>:<line>` of the plant, or `<path>` for accepted); why (5.5 T12.c3, G16): {check: the cause text
    that line must end with}; was: the check and place only."""
    def expect(r, v, g, i):
        # 3.5 T4 C7; was: any FAIL line of the check (startswith f"FAIL {c} ")
        got = {c: sum(1 for f in i.get("provenance", []) if f.startswith(f"FAIL {c} {at[c]}:")
                      and f.endswith((why or {}).get(c, ""))) for c in checks}
        print(f"control prov: FAIL lines at the plant {got}")
        gv = g.get("provenance", "pass")
        return gv != "pass" and "REFUSED" not in gv and all(got.values()), "provenance " + " + ".join(checks)
    return kw, expect


def _line_of(path, text):  # 1-based number of the one line of path holding text (3.5 T4 C7)
    ns = [n + 1 for n, ln in enumerate(path.read_text(encoding="utf-8").splitlines()) if text in ln]
    if len(ns) != 1:
        raise RuntimeError(f"{path}: {text!r} on {len(ns)} lines")
    return ns[0]


def _prov_plant(d, old, new, checks, marks=None, why=None):  # marks: {check: text of its planted line} (default: the
    # last line); why: as _prov (5.5 T12.c3)
    s = _src_copy(d)
    _edit_text(s / LOC_SRC, old, new)
    last = new.strip().splitlines()[-1]
    at = {c: f"src/{LOC_SRC.as_posix()}:{_line_of(s / LOC_SRC, (marks or {}).get(c, last))}" for c in checks}
    print(f"control prov: planted in {LOC_SRC.as_posix()}: {last!r}; want FAIL at {at}")
    return _prov(checks, {"src_root": s}, at, why)


TAIL = "    return -1;\n}\n"  # end of LocString_GetStringID, inside its FUNCTION body (to EOF)


def _c_prov_hedge(d):
    return _prov_plant(d, TAIL, TAIL + "// this is probably a planted hedge\n", ("hedge",))


def _c_prov_proto(d):
    return _prov_plant(d, TAIL, TAIL + "struct ZzGateNotInPdb; // NAME: proto-pdb:default.pdb\n", ("proto-pdb",))


def _c_prov_name(d):
    return _prov_plant(d, TAIL, TAIL + "struct ZzGateNotInXbe; // NAME: retail\n", ("name-retail",))


def _c_prov_place(d):
    return _prov_plant(d, TAIL, TAIL + "extern int function_00000001;\n", ("placeholder",))


def _c_prov_src(d):
    return _prov_plant(d, TAIL, TAIL + '// src proto-pdb:default.pdb ZzGateNotInPdb\n'
                       '// src retail 0x00140B98 "zz_gate_not_retail"\n', ("src-pdb", "src-retail"),
                       {"src-pdb": "ZzGateNotInPdb", "src-retail": "zz_gate_not_retail"})


def _c_prov_fake(d):
    s, b = _src_copy(d), d / "base"
    shutil.copytree(FIX_SRC, b)
    _edit_text(b / LOC_SRC, TAIL, "    // !FAKE: c2 register allocation; gate control\n" + TAIL)
    print(f"control prov: lint base {LOC_SRC.as_posix()} has a `// !FAKE:` line the src copy lacks")
    return _prov(("fake-diff",), {"src_root": s, "lint_base": b},
                 {"fake-diff": f"src/{LOC_SRC.as_posix()}:{_line_of(b / LOC_SRC, '!FAKE: c2 register allocation; gate')}"})


def _c_prov_locals(d):
    return _prov_plant(d, "{\n    NLocString::BLOBDATA* p", "{\n    volatile int zz_gate_local = 0;\n"
                       "    NLocString::BLOBDATA* p", ("locals",), {"locals": "zz_gate_local"},
                       {"locals": ": local `zz_gate_local` is neither a PDB local of its proc nor neutral"})
    # 5.5 T12.c3 (G16): its own cause (prov-locals-cache too); was: the check and place only


def _c_prov_d66(d):
    return _prov_plant(d, TAIL, TAIL + "extern volatile int zz_gate_d66;\n", ("d66",))


def _c_prov_forceinline(d):  # 4.2 T5 (D116, D117): refused even under a `// !FAKE:` label
    return _prov_plant(d, TAIL, TAIL + "struct ZzGateForceinline {\n    // !FAKE: inliner; gate control\n"
                       "    __forceinline int zz_gate_forceinline() const { return 0; }\n};\n", ("forceinline",),
                       {"forceinline": "__forceinline int zz_gate_forceinline"})


def _c_prov_pragma(d):  # 4.5 T16.2 G9 (D116): an in-body `#pragma` is refused even under a `// !FAKE:` line
    return _prov_plant(d, TAIL, "    // !FAKE: warning; gate control\n#pragma warning(disable:4100)\n" + TAIL,
                       ("pragma",), {"pragma": "#pragma warning(disable:4100)"})


def _c_prov_pragma_if0(d):  # 5.1 T1: the depth scan covers every `#if` arm (no preprocessor handling): `#if 0` too
    return _prov_plant(d, TAIL, "#if 0\n#pragma warning(disable:4100)\n#endif\n" + TAIL,
                       ("pragma",), {"pragma": "#pragma warning(disable:4100)"})


def _c_prov_include_dotdot(d):  # 5.1 T1: a quoted #include escaping the src root FAILs `include`, target present or not
    s = _src_copy(d)
    up = "../" * len(LOC_SRC.parts)  # LocString.cpp's dir -> d (outside src)
    (d / "zz_gate_outside.h").write_text("// gate control: a header outside the src root\n", encoding="utf-8",
                                         newline="\n")
    miss, out = f'#include "{up}zz_gate_missing.h"', f'#include "{up}zz_gate_outside.h"'
    # the missing one under `#if 0`: the lint reads every arm, the locals /Z7 compile must not fail (a REFUSED lint)
    _edit_text(s / LOC_SRC, TAIL, TAIL + f"#if 0\n{miss}\n#endif\n{out}\n")
    at = [f"src/{LOC_SRC.as_posix()}:{_line_of(s / LOC_SRC, t)}" for t in (miss, out)]
    print(f"control prov: planted in {LOC_SRC.as_posix()}: {miss!r}, {out!r}; want FAIL include at {at}")

    def expect(r, v, g, i):
        got = {a: sum(1 for f in i.get("provenance", []) if f.startswith(f"FAIL include {a}:")) for a in at}
        print(f"control prov: FAIL lines at the plants {got}")
        gv = g.get("provenance", "pass")
        return gv != "pass" and "REFUSED" not in gv and all(got.values()), "provenance include (missing + outside)"
    return {"src_root": s}, expect


def _c_prov_noinline(d):  # 4.5 T16.2 G9 (D116): `noinline` is refused even under a `// !FAKE:` line
    return _prov_plant(d, TAIL, TAIL + "// !FAKE: inliner; gate control\n__declspec(noinline) int zz_gate_noinline();\n",
                       ("noinline",))


def _c_prov_include_body(d):  # 4.5 T16.2 G18: a header `#define` carrying `volatile` FAILs d66 and a `/FI` opt is
    s = _src_copy(d)          # refused by build.flags_of (the lint never reads a force-included header)
    h = s / LOC_SRC.parent / "zz_gate_include.h"
    h.write_text("// gate control: a header macro adds a qualifier to a body\n#define ZZ_GATE_VOL volatile\n",
                 encoding="utf-8", newline="\n")
    rel = f"src/{(LOC_SRC.parent / h.name).as_posix()}"
    print(f"control prov: planted {rel}: '#define ZZ_GATE_VOL volatile'; want FAIL d66 at {rel}:2 and a /FI refusal")
    kw, lint_expect = _prov(("d66",), {"src_root": s}, {"d66": f"{rel}:2"})

    def expect(r, v, g, i):
        ok, at = lint_expect(r, v, g, i)
        try:
            build.flags_of({"name": "zz_gate", "opt": "/O2 /FIzz_gate_include.h"})
            why = ""
        except build.Refuse as e:
            why = str(e)
        print(f"control prov: flags_of /FI -> {why or 'accepted'}")
        return ok and "/FIzz_gate_include.h" in why and "steers" in why, at + " + build.flags_of /FI refused"
    return kw, expect


def _c_prov_accepted(d):
    a = d / "accepted.txt"
    a.write_text(FIX_ACC.read_text(encoding="utf-8") + f"src/{LOC_SRC.as_posix()}\t    return -1; probably\tgate "
                 f"control\n", encoding="utf-8", newline="\n")
    print(f"control prov: scratch accepted entry `return -1; probably` for src/{LOC_SRC.as_posix()}")
    return _prov(("accepted",), {"lint_accepted": a}, {"accepted": f"src/{LOC_SRC.as_posix()}"},
                 {"accepted": ": entry matches no hedge line"})  # 5.5 T12.c3 (G16): its own cause; was: none


def _c_prov_locals_cache(d, config=None):  # 3.5 T4 F5: the prov-locals plant + the clean source's obj planted under
    kw, expect = _c_prov_locals(d)        # the planted source's cache key (cache dir redirected to scratch, undo)
    old = provenance_lint.LOCALS_CACHE
    provenance_lint.LOCALS_CACHE = d / "locals_cache"
    try:
        flags = build.flags_of(build.unit_map(build.UNITS)[LOC_SRC.as_posix().lower()][0])
        clean = provenance_lint.locals_obj(FIX_SRC / LOC_SRC, flags,
                                           Path(vv_inputs.load_config(config)["msvc71_dir"]))
        plant = provenance_lint.LOCALS_CACHE / f"{provenance_lint.locals_key(kw['src_root'] / LOC_SRC, flags)}.obj"
        shutil.copyfile(clean, plant)
    except BaseException:
        provenance_lint.LOCALS_CACHE = old
        raise
    print("control prov-locals-cache: the clean source's /Z7 obj planted under the planted source's cache key")
    return {**kw, "undo": lambda: setattr(provenance_lint, "LOCALS_CACHE", old)}, expect


def _c_prov_inputs_pin(d, config=None):  # 3.5 T4 F6: config copy, prototype_dir -> a dir whose default.pdb differs
    cfg = vv_inputs.load_config(config)
    p = d / "proto"
    p.mkdir()
    b = bytearray(vv_inputs.input_path("prototype_dir", "default.pdb", cfg).read_bytes())
    b[-1] ^= 0xFF
    (p / "default.pdb").write_bytes(bytes(b))
    shutil.copyfile(vv_inputs.input_path("prototype_dir", "default.exe", cfg), p / "default.exe")
    print("control prov-inputs-pin: config copy, prototype_dir -> a copy whose default.pdb has its last byte flipped")
    return {"config": _cfg_copy(d, config, "prototype_dir", p)}, lambda r, v, g, i: (
        r == 2 and g.get("provenance", "").startswith("fail: REFUSED") and "default.pdb" in g.get("provenance", ""),
        "provenance REFUSED (default.pdb)")


def _c_prov_fake_head(d):  # 3.5 T4 W5: scratch git repo; a `// !FAKE:` line committed, then removed in a commit
    r, fake = d / "repo", "    // !FAKE: c2 register allocation; gate control\n"
    s = r / "src"
    shutil.copytree(FIX_SRC, s)

    def git(*a):
        subprocess.run(["git", "-C", str(r), "-c", "user.name=gate", "-c", "user.email=gate@control.invalid",
                        "-c", "commit.gpgsign=false", "-c", "core.autocrlf=false", *a], check=True,
                       capture_output=True)
    git("init", "-q")
    _edit_text(s / LOC_SRC, TAIL, fake + TAIL)
    line = _line_of(s / LOC_SRC, fake.strip())
    git("add", "-A")
    git("commit", "-q", "--no-verify", "-m", "base: fake line")
    git("branch", "zz-base")
    _edit_text(s / LOC_SRC, fake + TAIL, TAIL)
    git("commit", "-q", "--no-verify", "-am", "fake line removed")
    git("branch", "--set-upstream-to=zz-base")
    print("control prov-fake-head: scratch repo, `// !FAKE:` committed (upstream zz-base) then removed at HEAD")
    kw, expect = _prov(("fake-diff",), {"src_root": s}, {"fake-diff": f"src/{LOC_SRC.as_posix()}:{line}"})
    # undo: git's read-only object files would survive the runner's rmtree(ignore_errors) on Windows
    return {**kw, "undo": lambda: shutil.rmtree(r, onexc=lambda f, p, e: (os.chmod(p, 0o700), f(p)))}, expect


def _c_prov_fake_percommit(d):  # 5.5 T13 (I56): scratch git repo; a `// !FAKE:` line committed (upstream zz-base),
    r, fake = d / "repo", "    // !FAKE: c2 register allocation; gate control\n"  # removed in one commit, re-added in
    s = r / "src"                                                                    # the next: no net diff, the
    shutil.copytree(FIX_SRC, s)                                                      # middle commit fails

    def git(*a):
        subprocess.run(["git", "-C", str(r), "-c", "user.name=gate", "-c", "user.email=gate@control.invalid",
                        "-c", "commit.gpgsign=false", "-c", "core.autocrlf=false", *a], check=True,
                       capture_output=True)
    git("init", "-q")
    _edit_text(s / LOC_SRC, TAIL, fake + TAIL)
    line = _line_of(s / LOC_SRC, fake.strip())
    git("add", "-A")
    git("commit", "-q", "--no-verify", "-m", "base: fake line")
    git("branch", "zz-base")
    _edit_text(s / LOC_SRC, fake + TAIL, TAIL)
    git("commit", "-q", "--no-verify", "-am", "fake line removed")
    _edit_text(s / LOC_SRC, TAIL, fake + TAIL)
    git("commit", "-q", "--no-verify", "-am", "fake line re-added")
    git("branch", "--set-upstream-to=zz-base")
    print("control prov-fake-percommit: scratch repo, `// !FAKE:` committed (upstream zz-base), removed, re-added")
    kw, expect = _prov(("fake-diff",), {"src_root": s}, {"fake-diff": f"src/{LOC_SRC.as_posix()}:{line}"})
    return {**kw, "undo": lambda: shutil.rmtree(r, onexc=lambda f, p, e: (os.chmod(p, 0o700), f(p)))}, expect


def _c_prov_baseline_stale(d):  # 5.5 T13: a baseline entry with no hit of its check on its line text -> stale
    a = d / "accepted.txt"
    a.write_text(FIX_ACC.read_text(encoding="utf-8") + f"attr\tsrc/{LOC_SRC.as_posix()}\t    return -1;\tgate "
                 f"control\n", encoding="utf-8", newline="\n")
    print(f"control prov: scratch accepted baseline entry `attr` on `return -1;` for src/{LOC_SRC.as_posix()}")
    return _prov(("accepted",), {"lint_accepted": a}, {"accepted": f"src/{LOC_SRC.as_posix()}"},
                 {"accepted": ": baseline `attr` entry matches no `attr` hit (stale): return -1;"})


def _c_prov_locals_neutral(d):  # 5.5 T13 (S5): a neutral stand-in the PDB proc lacks, no `// !FAKE:`
    return _prov_plant(d, "{\n    NLocString::BLOBDATA* p", "{\n    volatile int v9 = 0;\n"
                       "    NLocString::BLOBDATA* p", ("neutral",), {"neutral": "volatile int v9"},
                       {"neutral": ": neutral local `v9` is not a PDB local of its proc and has no `// !FAKE:` on or "
                                   "above its first use"})


def _c_prov_locals_noproc(d):  # 5.5 T13 (G4): the matched body under `#if 0` -> no obj proc -> `locals` at FUNCTION
    s = _src_copy(d)
    _edit_text(s / LOC_SRC, 'extern "C" int LocString_GetStringID(', '#if 0\nextern "C" int LocString_GetStringID(')
    _edit_text(s / LOC_SRC, TAIL, TAIL + "#endif\n")
    at = f"src/{LOC_SRC.as_posix()}:{_line_of(s / LOC_SRC, '// FUNCTION: USA 0x00140B98')}"
    print(f"control prov: {LOC_SRC.as_posix()} matched body under `#if 0`; want FAIL locals at {at}")
    return _prov(("locals",), {"src_root": s}, {"locals": at},
                 {"locals": ": no obj proc for the matched function (its locals unchecked)"})


def _c_prov_scratch_shared(d, config=None):  # kw `direct` (5.5 T13, G5): a lint run removes only its own per-run
    base = provenance_lint.LOCALS_SCRATCH      # locals scratch: a sentinel in the base dir survives it
    sent = base / "pa_control_sentinel.txt"
    print(f"control prov-scratch-shared: a sentinel in {base.name}/, then provenance_lint.lint (cache=False)")

    def undo():
        sent.unlink(missing_ok=True)
        with contextlib.suppress(OSError):
            base.rmdir()

    def expect(r, v, g, i):
        base.mkdir(parents=True, exist_ok=True)
        sent.write_text("prov-scratch-shared\n", encoding="utf-8")
        before = set(base.parent.glob(base.name + ".*"))
        fails, _n, _f, cov = provenance_lint.lint(FIX_SRC, FIX_LIST, None, FIX_ACC, config=config, cache=False)
        kept, left = sent.is_file(), set(base.parent.glob(base.name + ".*")) - before
        print(f"control prov-scratch-shared: lint {len(fails)} fail(s), {cov['with']} function(s) with obj locals; "
              f"sentinel {'kept' if kept else 'DELETED'}; per-run dirs left {len(left)}")
        return kept and not left and cov["with"] > 0, f"sentinel in {base.name}/ kept by a lint run"
    return {"direct": True, "undo": undo}, expect


def _c_prov_attr(d):  # 5.5 T13 (D138): `__declspec(noreturn)` under a `!FAKE` with no before/after record
    return _prov_plant(d, TAIL, TAIL + "// !FAKE: c2 block layout; noreturn; no before/after recorded\n"
                       "__declspec(noreturn) void zz_gate_attr();\n", ("attr",))


def _c_prov_pragma_scope(d):  # 5.5 T13 (D138): `#pragma intrinsic` after the first FUNCTION line (depth 0)
    return _prov_plant(d, TAIL, TAIL + 'extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);\n'
                       "// !FAKE: c1xx front end; file-scope intrinsic; gate control\n#pragma intrinsic(memcpy)\n",
                       ("pragma-scope",))


def _c_prov_fake_fields(d):  # 5.5 T13 (I43, D33): a `!FAKE` line with two fields
    return _prov_plant(d, TAIL, TAIL + "// !FAKE: c2 register allocation; gate control\n", ("fake-fields",))


def _c_prov_member_invented(d):  # 5.5 T13.c3 (S4, D78): a data member NLocString::SECTIONHEADER lacks in default.pdb
    s, old = _src_copy(d), "    unsigned int m_uNumberOfEntries;\n};"
    _edit_text(s / LOC_SRC, old, old[:-3] + "    int m_count; // 0x04\n};")
    print("control prov: `int m_count;` planted in NLocString::SECTIONHEADER; want one `members` FAIL there")
    return _prov_exact("members", {"src_root": s}, [f"src/{LOC_SRC.as_posix()}:{_line_of(s / LOC_SRC, 'm_count')}"])


def _c_prov_untagged_helper(d):  # 5.5 T13.c3 (I52): an untagged free helper before the first FUNCTION line
    s, fn = _src_copy(d), "// FUNCTION: USA 0x00140B98"
    _edit_text(s / LOC_SRC, fn, "static int zz_helper() { return 0; }\n\n" + fn)
    print("control prov: `static int zz_helper()` planted above the FUNCTION line; want one `untagged` FAIL there")
    return _prov_exact("untagged", {"src_root": s}, [f"src/{LOC_SRC.as_posix()}:{_line_of(s / LOC_SRC, 'zz_helper')}"])


def _prov_exact(check, kw, want):  # 4.1 T4: kw + expect: `provenance` fails (not refused) with check's FAIL lines at
    def expect(r, v, g, i):        # exactly want (`<path>:<line>`), none more; plain _prov asks only for one at the plant
        pre = f"FAIL {check} "
        got = sorted(f[len(pre):].split(": ", 1)[0] for f in i.get("provenance", []) if f.startswith(pre))
        print(f"control prov: {check} FAIL lines {got}; want {sorted(want)}")
        gv = g.get("provenance", "pass")
        return gv != "pass" and "REFUSED" not in gv and got == sorted(want), f"provenance {check} exactly {len(want)}"
    return kw, expect


def _fake_base(d, base_edits, src_edits, text):  # 4.1 T4: lint base and src copies of LOC_SRC edited; want the base line of text
    s, b = _src_copy(d), d / "base"
    shutil.copytree(FIX_SRC, b)
    for old, new in base_edits:
        _edit_text(b / LOC_SRC, old, new)
    for old, new in src_edits:
        _edit_text(s / LOC_SRC, old, new)
    return _prov_exact("fake-diff", {"src_root": s, "lint_base": b},
                       [f"src/{LOC_SRC.as_posix()}:{_line_of(b / LOC_SRC, text)}"])


def _c_prov_fake_reword(d):  # 4.1 T4: two labels in one function, the first removed, the second reworded
    top = "{\n    NLocString::BLOBDATA* p"
    print("control prov: base labels gone and kept-reworded, one fake-diff FAIL at the removed one")
    return _fake_base(d, ((top, "{\n    // !FAKE: c2 gate control, removed\n    NLocString::BLOBDATA* p"),
                          (TAIL, "    // !FAKE: c2 gate control, kept\n" + TAIL)),
                      ((TAIL, "    // !FAKE: c2 gate control, kept and reworded\n" + TAIL),),
                      "!FAKE: c2 gate control, removed")


def _c_prov_fake_trailing(d):  # 4.1 T4: a trailing label on a line d66 does not match, stripped
    print("control prov: base `return -1; // !FAKE:`, stripped in the src copy")
    return _fake_base(d, ((TAIL, "    return -1; // !FAKE: c2 gate control\n}\n"),), (), "!FAKE: c2 gate control")


def _c_prov_fake_comment_excuse(d):  # 4.1 T4: own-line label removed; only a trailing comment edited in its function
    print("control prov: base own-line label removed, a trailing comment edited in the same function")
    return _fake_base(d, ((TAIL, "    // !FAKE: c2 gate control\n    return -1; // a note\n}\n"),),
                      ((TAIL, "    return -1; // a note, edited\n}\n"),), "!FAKE: c2 gate control")


def _c_prov_name_ns(d):  # 4.1 T4: both NAME causes in `namespace std`; right names pass, wrong ones and a closed namespace fail
    s = _src_copy(d)
    nm = "// NAME: proto-pdb:default.pdb\n"
    _edit_text(s / LOC_SRC, TAIL, TAIL + "#ifdef NON_MATCHING\nnamespace std {\n"
               + nm + "template void vector<Matrix>::resize(size_type);\n"
               + nm + "template vector<Matrix>::size_type vector<Matrix>::size() const;\n"
               + nm + "template void vector<Matrix>::zzGateWrong(size_type);\n"
               + nm + "template vector<Matrix>::size_type vector<Matrix>::zzGateWrongScoped() const;\n}\n"
               + nm + "template void vector<Matrix>::capacity() const;  // zzGateClosed\n#endif\n")
    # FAIL names the NAME line, directly above the code line
    want = [f"src/{LOC_SRC.as_posix()}:{_line_of(s / LOC_SRC, t) - 1}"
            for t in ("zzGateWrong(", "zzGateWrongScoped(", "zzGateClosed")]
    print(f"control prov: namespace std plants in {LOC_SRC.as_posix()}; want proto-pdb exactly at {want}")
    return _prov_exact("proto-pdb", {"src_root": s}, want)


def _c_prov_nested_op(d):  # 5.3 T2: a nested-class operator keeps its outer class; right ones pass, a wrong outer fails
    s = _src_copy(d)
    nm = "// NAME: proto-pdb:default.pdb\n"
    _edit_text(s / LOC_SRC, TAIL, TAIL + "#ifdef NON_MATCHING\n"
               + nm + "template Matrix const & std::deque<Matrix,std::allocator<Matrix> >::const_iterator::operator*()const;\n"
               + nm + "template std::vector<bool,std::allocator<bool> >::reference & "
                      "std::vector<bool,std::allocator<bool> >::reference::operator=(bool);\n"
               + nm + "template Matrix const & zzGateWrong<Matrix>::const_iterator::operator*()const;\n#endif\n")
    want = [f"src/{LOC_SRC.as_posix()}:{_line_of(s / LOC_SRC, 'zzGateWrong<') - 1}"]
    print(f"control prov: nested operator plants in {LOC_SRC.as_posix()}; want proto-pdb exactly at {want}")
    return _prov_exact("proto-pdb", {"src_root": s}, want)


def _c_prov_d66_pdb_volatile(d, config=None):  # 4.1 T4 (M21): the real unit, labels stripped, the row in a scratch list; the
    rel = Path("Libs") / "BeepLib" / "Xbox" / "Official_Release" / "BeepAlloc.cpp"  # PDB types x, y volatile, d66 still refuses
    s, lp, acc = d / "src", d / "matched_set.toml", d / "accepted.txt"
    (s / rel).parent.mkdir(parents=True)
    shutil.copyfile(SRC / rel, s / rel)
    shutil.copytree(SRC / "include", s / "include")
    _edit_text(s / rel, "        // !FAKE: c1xx front end; volatile local; not match-only: the PDB types x and y `volatile "
               "int`\n        // (pdb_types locals AllocateMemory), the marker is for the d66 lint\n", "")
    _edit_text(s / rel, "volatile int y = 1 / x; // !FAKE: c1xx front end; volatile local; as x above (PDB-typed, d66 "
               "lint marker)", "volatile int y = 1 / x;")
    m = re.search(r'\[\[function\]\]\nsymbol = "\?AllocateMemory@[^\n]*\n(?:[^\n]+\n)+', LIST.read_text(encoding="utf-8"))
    lp.write_text(m.group(0), encoding="utf-8", newline="\n")
    acc.write_text("", encoding="utf-8", newline="\n")
    want = [f"src/{rel.as_posix()}:{_line_of(s / rel, t)}" for t in ("volatile int x = 0;", "volatile int y = 1 / x;")]
    kw, exact = _prov_exact("d66", {"src_root": s, "list_path": lp, "lint_accepted": acc}, want)

    def expect(r, v, g, i):
        ok, at = exact(r, v, g, i)
        typed = {x["name"]: provenance_lint.pdb_types.tname(x["ti"])
                 for rec in provenance_lint._INPUTS[config][0].locals_of("AllocateMemory") for x in rec["locals"]}
        print(f"control prov: PDB AllocateMemory x {typed.get('x')!r}, y {typed.get('y')!r}")
        return ok and all(typed.get(n, "").startswith("volatile ") for n in "xy"), at + " on PDB-volatile locals"
    return kw, expect


def _c_annotations_folded(d):  # 4.1 T4: annotations.check direct on scratch src and list
    lp, head = d / "matched_set.toml", "// FUNCTION: USA 0x00140B98"
    shutil.copyfile(FIX_LIST, lp)

    def fails(name, first, second):
        s = d / name
        shutil.copytree(FIX_SRC, s)
        _edit_text(s / LOC_SRC, f"{head}\n", f"{head}{first}\n// NAME: proto-pdb:default.pdb\n{head}{second}\n")
        return [f for f in annotations.check(s, lp)[0] if "00140B98" in f]

    def norow(name, tail):  # the same pair on a VA no row names, at the end of the file
        s = d / name
        shutil.copytree(FIX_SRC, s)
        line = f"// FUNCTION: USA 0x{ANN_NO_ROW:08X}{tail}\n// NAME: proto-pdb:default.pdb\n"
        _ann_append(s, line + line)
        return annotations.check(s, lp)[0]

    def expect(r, v, g, i):
        pair, mixed, bare = fails("pair", " FOLDED", " FOLDED"), fails("mixed", " FOLDED", ""), fails("bare", "", "")
        rowless, rowless_ok = norow("norow", ""), norow("norow_folded", " FOLDED")
        print(f"control annotations-folded: both FOLDED {pair}; one FOLDED {mixed}; bare {bare}; "
              f"no row bare {rowless}; no row both FOLDED {rowless_ok}")
        return (not pair and any("need 1" in f for f in mixed) and any("need 1" in f for f in bare)
                and len(rowless) == 1 and "need 1" in rowless[0] and f"{ANN_NO_ROW:08X}" in rowless[0]
                and not rowless_ok), "annotations count rule"
    return {"direct": True}, expect


ANN_NO_ROW = 0x0FFFFFF0  # 4.1 T4: a VA no fixture row names
ANN_BAD = "// FUNCTION: USA 0x00140b98\n"  # 4.1 T4: lower-case hex, malformed
ANN_NAME = '// NAME: proto-pdb:default.pdb\nextern "C" int LocString_GetStringID'  # 4.1 T4: the row's NAME line, its code


def _ann_append(s, text):
    with open(s / LOC_SRC, "a", encoding="utf-8", newline="\n") as f:
        f.write("\n" + text)


def _ann_no_name(s):
    _edit_text(s / LOC_SRC, ANN_NAME, ANN_NAME.split("\n", 1)[1])


def _ann_one(d, name, edit, want):  # 4.1 T4: annotations.check direct on a planted scratch src: exactly one FAIL, holding want
    def expect(r, v, g, i):
        s = _src_copy(d)
        edit(s)
        fails = annotations.check(s, FIX_LIST)[0]
        print(f"control annotations-{name}: {fails}")
        return len(fails) == 1 and want in fails[0], f"annotations {want}"
    return {"direct": True}, expect


def _c_annotations_malformed(d):
    return _ann_one(d, "malformed", lambda s: _ann_append(s, ANN_BAD), "malformed FUNCTION line")


def _c_annotations_no_name(d):
    return _ann_one(d, "no-name", _ann_no_name, "0x00140B98 not followed by")


def _c_annotations_no_source(d):
    return _ann_one(d, "no-source", lambda s: (s / LOC_SRC).unlink(), "no source")


def _c_annotations_count(d):  # a matched row with 0 FUNCTION lines
    return _ann_one(d, "count", lambda s: _edit_text(s / LOC_SRC, "// FUNCTION: USA 0x00140B98\n", ""),
                    "0 `// FUNCTION: USA 0x00140B98` lines")


def _c_annotations_order(d):  # 4.1 T4: order never fails: one real failure beside it is the only FAIL; gone, no FAIL
    def expect(r, v, g, i):
        def run(name, tail):
            s = d / name
            shutil.copytree(FIX_SRC, s)
            _ann_append(s, "// FUNCTION: USA 0x00000010\n// NAME: proto-pdb:default.pdb\n" + tail)
            res = annotations.check(s, FIX_LIST)
            return res[0], [o for o in res[3] if o.startswith(f"ORDER {LOC_SRC.as_posix()}:")
                            and "0x00000010 not ascending" in o]
        (both, bo), (alone, ao) = run("both", ANN_BAD), run("alone", "")
        print(f"control annotations-order: order plus real {both} {bo}; order alone {alone} {ao}")
        return (len(both) == 1 and "malformed" in both[0] and len(bo) == 1 and not alone and len(ao) == 1), \
            "annotations order advisory"
    return {"direct": True}, expect


def _c_annotations_row(d):  # 4.1 T4: through _core: rc 1, the annotations global fails on the one line
    s = _src_copy(d)
    _ann_no_name(s)
    print(f"control annotations-row: NAME line of 0x00140B98 removed in {LOC_SRC.as_posix()}")
    return {"src_root": s}, lambda r, v, g, i: (
        r == 1 and g.get("annotations") == "fail: 1 failure(s)" and len(i.get("annotations", [])) == 1
        and "0x00140B98 not followed" in i["annotations"][0], "annotations global")


def _prov_patched(name, fake, want, rc):  # 3.5 T4 N8: provenance_lint.lint replaced (restored by the runner's undo)
    orig = provenance_lint.lint
    provenance_lint.lint = fake
    print(f"control prov-{name}: provenance_lint.lint replaced by a fake")
    return {"undo": lambda: setattr(provenance_lint, "lint", orig)}, lambda r, v, g, i: (
        r == rc and g.get("provenance", "pass").startswith(want), f"provenance ({want}, rc {rc})")


def _c_prov_refused(d):
    def refuse(*a, **k):
        raise provenance_lint.Refuse("gate control prov-refused")
    return _prov_patched("refused", refuse, "fail: REFUSED gate control", 2)


def _c_prov_error(d):
    def boom(*a, **k):
        raise RuntimeError("gate control prov-error")
    return _prov_patched("error", boom, "fail: provenance error RuntimeError: gate control prov-error", 1)
    # 5.5 T12.c3 (G16): the whole verdict; was: "fail: provenance error RuntimeError" (any RuntimeError)


def _layer_patched(mod, attr, key, name, fake, want, rc):  # 4.5 T16.2.c3 (G14): mod.attr replaced (runner's undo)
    orig = getattr(mod, attr)
    setattr(mod, attr, fake)
    print(f"control {name}: {mod.__name__}.{attr} replaced by a fake")
    return {"undo": lambda: setattr(mod, attr, orig), "run_pins": key == "pin_rows"}, lambda r, v, g, i: (
        r == rc and g.get(key, "pass").startswith(want), f"{key} ({want}, rc {rc})")


def _c_annotations_refused(d):
    def refuse(*a, **k):
        raise matched_set.Refuse("gate control annotations-refused")
    return _layer_patched(annotations, "check", "annotations", "annotations-refused", refuse,
                          "fail: REFUSED gate control annotations-refused", 2)


def _c_annotations_error(d):
    def boom(*a, **k):
        raise RuntimeError("gate control annotations-error")
    return _layer_patched(annotations, "check", "annotations", "annotations-error", boom,
                          "fail: annotations error RuntimeError: gate control annotations-error", 1)


def _c_pin_refused(d):
    def refuse(*a, **k):
        raise pin_rows.Refuse("gate control pin-refused")
    return _layer_patched(pin_rows, "check", "pin_rows", "pin-refused", refuse,
                          "fail: REFUSED gate control pin-refused", 2)


def _c_pin_error(d):
    def boom(*a, **k):
        raise RuntimeError("gate control pin-error")
    return _layer_patched(pin_rows, "check", "pin_rows", "pin-error", boom,
                          "fail: pin_rows error RuntimeError: gate control pin-error", 1)


def _c_pin_fold_site(d):
    """kw `direct` (4.5 T16.2.c3, G14, D119): pin_rows.State.site_test on a stub State, one proto public P with two
    names: fold sites alone -> site-conflict-only; a fold site beside an agreeing site passes; a site whose rd is in
    tab_va is no fold site -> site-conflict-both."""
    p, va, rd, n = 0x00011000, 0x00011000, 0x00022000, "?a@@YAXXZ"

    def run(case, sites, tab):
        s = types.SimpleNamespace(by_pd={p: {x: ["call"] for x in sites}}, pubs={p: {n, "?b@@YAXXZ"}}, tab_va=tab,
                                  by_rd={}, accepted={})  # was: accepted=set() (5.1 T3: a dict, (va, n) -> (line, conflict))
        r = {"name": n, "va": va, "protos": [p], "fails": [], "notes": []}
        pin_rows.State.site_test(s, r)
        print(f"control pin-fold-site: {case}: site {r['site']} fails {r['fails']}")
        return r["fails"]

    def expect(r_, v, g, i):
        return (run("fold only", [rd], {}) == ["site-conflict-only"] and run("fold + agree", [va, rd], {}) == []
                and run("rd in tab_va + agree", [va, rd], {rd: {"?c@@YAXXZ"}}) == ["site-conflict-both"]), \
            "pin_rows.site_test fold sites"
    return {"direct": True}, expect


PIN_UNSIZED = "__EH_prolog"  # symbols_pin.txt func, size `-`, disasm-verified (3.4 T6.1)
PIN_SITE = "?GetTextExtent@CXBFont@@QBEJPBGPAM1H@Z"  # symbols_pin.txt func, site-corr
PIN_ALIAS_VA, PIN_ALIAS = 0x000292A6, ("??1?$ResourceHandle@VAnimationResource@@@@QAE@XZ",
                                       "??1?$ResourceHandle@VTextureResource@@@@QAE@XZ")  # fold aliases, C0130


def _pin_rename(d, name):
    """Scratch list copy with FLOUR_SYM's row renamed to the pin name `name` (va/size/hashes kept)."""
    lp = d / "matched_set.toml"
    lp.write_text(FIX_LIST.read_text(encoding="utf-8").replace(f"symbol = {json.dumps(FLOUR_SYM)}\n",
                                                               f"symbol = {json.dumps(name)}\n", 1),
                  encoding="utf-8", newline="\n")
    print(f"control pin: list copy, {FLOUR_SYM} row renamed to pin {name}")
    return lp


def _c_pin_unsized(d):
    return {"list_path": _pin_rename(d, PIN_UNSIZED)}, lambda r, v, g, i: (
        "pin has no size" in v[PIN_UNSIZED]["size"], f"{PIN_UNSIZED} size (unsized pin)")


def _c_pin_site(d):
    return {"list_path": _pin_rename(d, PIN_SITE)}, lambda r, v, g, i: (
        "pin status site-corr" in v[PIN_SITE]["size"], f"{PIN_SITE} size (site-corr pin)")


def _c_pin_alias_dup(d):
    lp = d / "matched_set.toml"
    shutil.copy2(FIX_LIST, lp)
    a, b = PIN_ALIAS
    matched_set.add_function(lp, a)
    try:
        matched_set.add_function(lp, b)
        add_why = None
    except matched_set.Refuse as e:
        add_why = str(e)
    print(f"control pin-alias-dup: list copy + {a}; add {b} at the same VA: {add_why or 'NOT refused'}")
    text = lp.read_text(encoding="utf-8")
    lp.write_text(text + text[text.rindex("\n[[function]]\n"):].replace(json.dumps(a), json.dumps(b)),
                  encoding="utf-8", newline="\n")
    print(f"control pin-alias-dup: {b} row appended beside it by hand (0x{PIN_ALIAS_VA:08X} twice)")
    try:
        matched_set._parse(lp)
        why = ""
    except matched_set.Refuse as e:
        why = str(e)
    return {"list_path": lp}, lambda r, v, g, i: (
        r == 2 and f"0x{PIN_ALIAS_VA:08X} already in" in (add_why or "") and f"duplicate va 0x{PIN_ALIAS_VA:08X}" in why,
        "add refusal + list (duplicate va)")


# 4.2 T5 pin-row plants (c2/c3, D119): (id, VA of the row edited (first pin row there), field edits ("+": the edited
# copy inserted after the row; tag None: the column dropped), the plant's own fail class, the VA it fails at, the
# exact fail text there (5.5 T28, G16: the sub-cause, not the class token alone))
PIN_FIELDS = ("va", "kind", "size", "name", "tag", "status")
_PP = "proto-pdb-public+"
PIN_PLANTS = (
    ("H1", 0x000FFD8C, {"status": "disasm-verified"}, "status-vs-tag", 0x000FFD8C, "status-vs-tag"),
    ("H2", 0x000FFD8C, {"status": "disasm-verified", "tag": _PP + "site-corr+callee-bytes"}, "route-func", 0x000FFD8C,
     "route-func:callee-bytes-differ"),
    ("H3", 0x000FFD8C, {"status": "disasm-verified", "tag": _PP + "retail-xref"}, "hand-name-not-in-evidence",
     0x000FFD8C, "hand-name-not-in-evidence"),
    ("H4", 0x001C8648, {"status": "disasm-verified", "tag": _PP + "site-corr+data-bytes"}, "route-global", 0x001C8648,
     "route-global:name-kind-vs-tag"),
    ("H5", 0x001C8648, {"status": "disasm-verified", "tag": _PP + "retail-disasm"}, "hand-name-not-in-evidence",
     0x001C8648, "hand-name-not-in-evidence"),
    ("T1", 0x00027A42, {"tag": None}, "grammar", 0x00027A42, "grammar"),
    ("N1", 0x00027A42, {"name": "?NotAPublic@@YAXXZ"}, "name-not-public", 0x00027A42, "name-not-public"),
    ("N2", 0x000E0887, {"name": "?NotAPublic@@YAXXZ"}, "name-not-public", 0x000E0887, "name-not-public"),
    ("N3", 0x000220C6, {"name": "?NotAPublic@@YAXXZ"}, "route-label", 0x000220C6, "route-label:label-not-va"),
    ("N4", 0x001EA324, {"name": "_s_00000000"}, "route-label", 0x001EA324, "route-label:label-not-va"),
    ("V1", 0x000E0887, {"va": "0x000E0518"}, "site-conflict-only", 0x000E0518, "site-conflict-only"),
    ("V2", 0x00027A42, {"va": "0x000BFFD7"}, "route-func", 0x000BFFD7, "route-func:callee-bytes-differ"),
    ("V3", 0x00027A42, {"va": "0x7FFF0000"}, "va-off-image", 0x7FFF0000, "va-off-image"),
    ("V4", 0x002369CC, {"va": "0x002369D0"}, "site-conflict-only", 0x002369D0, "site-conflict-only"),
    ("V5", 0x001C8648, {"va": "0x001C864C"}, "site-conflict-only", 0x001C864C, "site-conflict-only"),
    ("Wa1", 0x000E0887, {"name": "?RunLevel@@YAXXZ"}, "site-conflict-only", 0x000E0887, "site-conflict-only"),
    ("Wa2", 0x00027A42, {"name": "?RunLevel@@YAXXZ"}, "route-func", 0x00027A42, "route-func:callee-bytes-differ"),
    ("Wa3", 0x00018B34, {"name": "?RunLevel@@YAXXZ"}, "site-conflict-only", 0x00018B34, "site-conflict-only"),
    ("Wa4", 0x00011178, {"name": "?RunLevel@@YAXXZ"}, "fold-other-name-not-alias", 0x00011178,
     "fold-other-name-not-alias"),
    ("Wb1", 0x002369CC, {"name": "_g_pfnxcsuOnline"}, "site-conflict-only", 0x002369CC, "site-conflict-only"),
    ("Wb2", 0x001CAE20, {"name": "__real@40e00000"}, "route-float", 0x001CAE20, "route-float:float-differs"),
    ("Wb3", 0x002231E0, {"name": "_g_pfnxcsuOnline"}, "site-conflict-only", 0x002231E0, "site-conflict-only"),
    ("Wb4", 0x002231E0, {"name": "__imp__XcVerifyPKCS1Signature@12"}, "section-differs", 0x002231E0, "section-differs"),
    ("Wb5", 0x00225748, {"name": "__imp__XcVerifyPKCS1Signature@12"}, "section-differs", 0x00225748, "section-differs"),
    ("D1", 0x00027A42, {"+": True}, "dup-same-row", 0x00027A42, "dup-same-row"),
    ("D2", 0x00027A42, {"+": True, "va": "0x00011000"}, "dup-name-two-vas", 0x00011000, "dup-name-two-vas"),
    ("D3", 0x00027A42, {"+": True, "name": "?RunLevel@@YAXXZ"}, "va-two-names", 0x00027A42, "va-two-names"),
    ("Tw1", 0x001253D7, {"name": "_XShaHmacUpdate@12"}, "route-func", 0x001253D7, "route-func:callee-name-rule"),
    # 4.5 T16.1 (G2, G3): a site-less near VA; a global row moved 4 bytes onto unequal data
    ("near", 0x000688EA, {"va": "0x000688EB"}, "route-near", 0x000688EB, "route-near:no-agreeing-site"),
    ("V6", 0x001C7410, {"va": "0x001C7414"}, "route-global", 0x001C7414, "route-global:data-differs"),
)
PIN_UNRULED = "??1Diary@@QAE@XZ"  # D118: sized disasm-verified sole-name func pin 0x0010027B, its row passes pin_rows


def _pin_scratch(d, path, text):
    """Write a scratch copy under the control's dir d, never over the real file (C0177: assert the target first)."""
    p = (d / path.name).resolve()
    assert p.is_relative_to(d.resolve()) and p != path.resolve(), f"pin control scratch {p} outside {d}"
    p.write_text(text, encoding="utf-8", newline="\n")
    return p


def _pin_new(pid, base, s):
    """Fail lines of State s not in base's (line numbers dropped), each printed under the control."""
    key = lambda x: re.sub(r" \S+\.txt:\d+:", ":", x)  # noqa: E731
    before = {key(x) for x in base.row_fail_lines()}
    new = [x for x in s.row_fail_lines() if key(x) not in before]
    for x in new:
        print(f"control {pid}: {x}")
    return new


def _pin_has(new, why, at):
    """5.5 T28 (G16): a fail line of `new` at VA `at` (`0x%08X`) whose fail text is exactly `why` (`<file>:<line>:
    <why> (`), the sub-cause and not the class token alone."""
    return any(x.split()[3:4] == [at] and f": {why} (" in x for x in new)


def _pin_plant(pid, va, edits, cls, at, why):
    """kw `direct`: the pin file copy with one plant, rows re-run in-process (State.replant) -> a new fail line of
    class cls at VA at whose fail text is why (5.5 T28, G16; was: the class token and VA only)."""
    def make(d, config=None):
        base = pin_rows.state(config)
        L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
        i = next(k for k, ln in enumerate(L) if ln.startswith(f"0x{va:08X} "))
        f, e = L[i].split(), dict(edits)
        ins = e.pop("+", False)
        for k, v in e.items():
            f[PIN_FIELDS.index(k)] = v
        new = " ".join(x for x in f if x is not None)
        L[i:i + 1] = [L[i], new] if ins else [new]
        p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
        print(f"control pin-plant-{pid}: pin copy, row 0x{va:08X} {'+ inserted' if ins else 'edited'} {sorted(e)}")

        def expect(r, v, g, i_):
            new_lines = _pin_new(f"pin-plant-{pid}", base, base.replant(p))
            # was: any(x.split()[2:4] == [cls, f"0x{at:08X}"] for x in new_lines)
            return (any(x.split()[2:4] == [cls, f"0x{at:08X}"] and f": {why} (" in x for x in new_lines),
                    f"pin-row {cls} 0x{at:08X} {why}")
        return {"direct": True}, expect
    return make


# 5.5 T28.c2 (G2, R579): a pin copy row retagged near-corr; the near route re-derives similarity and a rel32 site from a
#   one-public proto dest. near-retag: no sized passing site-corr func row has an agreeing site and similarity < 70
#   (0 of 14 sized), so the unsized site-corr row 0x000B8AFF (19 agreeing sites, similarity 57.36) gets its proto proc
#   length 0x15A as size. near-abs-site: a callee-bytes row whose one agreeing site is an abs mask. near-fold: a fold
#   row (proto dest with 5 publics).
_NEAR = {"tag": _PP + "anchor-window+caller-xref", "status": "near-corr"}
_c_pin_near_retag = _pin_plant("near-retag", 0x000B8AFF, dict(_NEAR, size="0x15A"), "route-near", 0x000B8AFF,
                               "route-near:similarity-below")
_c_pin_near_abs_site = _pin_plant("near-abs-site", 0x00155EF4, _NEAR, "route-near", 0x00155EF4,
                                  "route-near:no-agreeing-site")
_c_pin_near_fold = _pin_plant("near-fold", 0x00018696, _NEAR, "route-near", 0x00018696, "route-near:multi-public")


def _c_pin_literal_list(d, config=None):
    """Through _core with the pin layer: the accepted list copy waives a non-literal row (?handledMain@@YAXXZ) ->
    pin_rows fails `accepted-stale` at it; pin_rows' cached State swapped for the copy's (restored by undo)."""
    base = pin_rows.state(config)
    acc = _pin_scratch(d, pin_rows.ACCEPTED, pin_rows.ACCEPTED.read_text(encoding="utf-8")
                       + "0x000E0887\t?handledMain@@YAXXZ\tgate control: a waiver for a non-literal row\tr:0x00000000\n")
    pin_rows._STATE[config] = base.replant(pin_rows.PIN, acc)
    print("control pin-literal-list: accepted list copy + a waiver of non-literal 0x000E0887")
    return {"undo": lambda: pin_rows._STATE.__setitem__(config, base)}, lambda r, v, g, i: (
        g.get("pin_rows", "pass").startswith("fail: ")
        and any(x.split()[2:4] == ["accepted-stale", "0x000E0887"] for x in i.get("pin_rows", [])),
        "pin_rows accepted-stale 0x000E0887")


def _c_pin_unruled(d, config=None):
    """kw `direct` (D118): a scratch pin copy with PIN_UNRULED's tag forged to a hand xref tag; matched_set add of it
    on a list copy is refused naming the pin-row check; the same add with the real pins passes (restored by undo)."""
    base, pins0 = pin_rows.state(None), matched_set.PINS
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    i = next(k for k, ln in enumerate(L) if ln.split()[3:4] == [PIN_UNRULED])
    f = L[i].split()
    f[4] = _PP + "retail-xref"
    L[i] = " ".join(f)
    p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
    lp = d / "matched_set.toml"
    shutil.copy2(FIX_LIST, lp)

    def undo():
        matched_set.PINS = pins0
        pin_rows._STATE[None] = base

    def expect(r, v, g, i_):
        matched_set.PINS, pin_rows._STATE[None] = p, base.replant(p)
        try:
            matched_set.add_function(lp, PIN_UNRULED)
            why = None
        except matched_set.Refuse as e:
            why = str(e)
        undo()
        print(f"control pin-unruled: forged pin, add {PIN_UNRULED}: {why or 'NOT refused'}")
        matched_set.add_function(lp, PIN_UNRULED)  # real pins: D118 lifts the refusal (raises if not)
        print(f"control pin-unruled: real pins, add {PIN_UNRULED}: added")
        # was: bool(why) and "pin-row check: " in why (5.5 T28, G16: the cause text)
        return (bool(why) and why.endswith("pin-row check: hand-name-not-in-evidence"),
                "matched_set add refused (pin-row check: hand-name-not-in-evidence)")
    return {"direct": True, "undo": undo}, expect


def _c_pin_i10_static(d, config=None):
    """kw `direct` (4.5 T16.2.c4, G14): the fixture built; a same-section static a fixture function relocates through
    inside its own extent, given a pin row (status site-corr) on a State stub -> I10 fails `i10` at it
    (byte_compare.resolve takes a tabled static by name); the fixture's untabled same-section statics stay skipped."""
    base, bdir = pin_rows.state(config), d / "build"
    build.build(FIX_SRC, bdir, build.UNITS, config)
    statics = []
    for e in matched_set.load(FIX_LIST):
        if e["kind"] != "function":
            continue
        # 5.1 T4.c2: obj at its source path; was: bdir / e["unit"].replace("/", "\\").rsplit("\\", 1)[-1]
        c = byte_compare.load_obj(bdir / build.obj_rels()[e["unit"].replace("/", "\\").lower()])[1]
        f = next(y for y in c["symbols"].values() if y["name"] == e["symbol"] and y["section"] > 0)
        sec, lo, hi = byte_compare.extent(c, f)
        statics += [y["name"] for rl in sec["relocs"] if lo <= rl["offset"] < hi
                    for y in [c["symbols"][rl["symbol"]]] if y["class"] == coff_info.SC_STATIC
                    and y["section"] == f["section"] and y["value"] and lo <= y["value"] < hi
                    and y["name"] not in base.sy_name and y["name"] not in base.by_row_name]
    n = statics[0]
    rows = dict(base.by_row_name)
    rows[n] = [{"va": 0x00011000, "status": "site-corr", "fails": [], "line": 0}]
    print(f"control pin-i10-static: {len(set(statics))} untabled same-section statics; {pin_rows.mask(n)} given a "
          f"site-corr pin row 0x00011000")

    def expect(r, v, g, i_):
        lines = pin_rows.State.i10(types.SimpleNamespace(sy_name=base.sy_name, by_row_name=rows, pin=base.pin),
                                   bdir, FIX_LIST)
        for x in lines:
            print(f"control pin-i10-static: {x}")
        ours = [x for x in lines if x.split()[2:5] == ["i10", "0x00011000", pin_rows.mask(n)]]
        rest = [x for x in lines if x not in ours and any(pin_rows.mask(s) in x.split() for s in statics)]
        return (len(ours) == 1 and "status site-corr" in ours[0] and not rest,
                "pin-row i10 at the tabled same-section static, untabled ones skipped")
    return {"direct": True}, expect


def _c_pin_vtable_prefix(d, config=None):
    """kw `direct` (4.5 T16.2.c4, G14): a passing vtable-literal pin row re-routed on a State copy whose proto slots
    stop one short of retail's (vt_slots less its last slot: the proto table a prefix) ->
    `route-vtable:slot-count-differs`; the same row unplanted passes."""
    base = pin_rows.state(config)
    r0 = next(r for r in base.rows if r.get("cls") and r["cls"][1] == "vtable" and not r["fails"] and r.get("protos")
              and len(base.vt_slots(r["protos"][0], r["va"])) >= 2)

    def run(case, cut):
        s = object.__new__(pin_rows.State)
        s.__dict__.update(vars(base))
        if cut:
            s.vt_slots = lambda p, t: base.vt_slots(p, t)[:-1]
        r = dict(r0, fails=[], notes=[])
        pin_rows.State.route(s, r)
        print(f"control pin-vtable-prefix: {case}: 0x{r0['va']:08X} {pin_rows.mask(r0['name'])} fails {r['fails']}")
        return r["fails"]

    def expect(r_, v, g, i):
        return (run("unplanted", False) == [] and run("proto slots a prefix", True)
                == ["route-vtable:slot-count-differs"]), "pin-row route-vtable:slot-count-differs"
    return {"direct": True}, expect


def _c_pin_accepted_unused(d, config=None):
    """kw `direct` (4.5 T16.2.c4, G14): the accepted list copy waives a live passing literal row no site conflict
    reaches (no `literal-copy-accepted` note) -> `accepted-stale ... unused` at it."""
    base = pin_rows.state(config)
    lit = [r for r in base.rows if r.get("name", "").startswith(("__real@", "??_C@")) and r.get("va") is not None
           and not r["fails"] and "literal-copy-accepted" not in r["notes"] and (r["va"], r["name"]) not in base.accepted]
    r0 = sorted(lit, key=lambda r: (not r["name"].startswith("__real@"), r["va"]))[0]
    acc = _pin_scratch(d, pin_rows.ACCEPTED, pin_rows.ACCEPTED.read_text(encoding="utf-8")
                       + f"0x{r0['va']:08X}\t{r0['name']}\tgate control: a waiver no site conflict uses\tr:0x00000000\n")
    va = f"0x{r0['va']:08X}"
    print(f"control pin-accepted-unused: accepted list copy + a waiver of passing literal {va} {pin_rows.mask(r0['name'])}")

    def expect(r, v, g, i_):
        new = _pin_new("pin-accepted-unused", base, base.replant(pin_rows.PIN, acc))
        return (any(x.split()[2:4] == ["accepted-stale", va] and ": unused " in x for x in new),
                f"pin-row accepted-stale {va} unused")
    return {"direct": True}, expect


def _c_pin_accepted_other_conflict(d, config=None):
    """kw `direct` (5.1 T3): a State copy whose by_pd (copied, base untouched) gives an accepted row's proto P one extra
    retail dest (another accepted row's VA, a table VA) -> the waiver no longer covers the conflict: a new
    `site-conflict-*` fail line at the row."""
    base = pin_rows.state(config)
    rows = {(r.get("va"), r.get("name")): r for r in base.rows}
    keys = sorted(k for k in base.accepted if rows.get(k, {}).get("protos"))
    (va, n), rd = keys[0], keys[1][0]
    p = rows[keys[0]]["protos"][0]
    by_pd = copy.copy(base.by_pd)
    by_pd[p] = copy.copy(base.by_pd[p])
    by_pd[p][rd] = list(by_pd[p].get(rd, [])) + [next(iter(by_pd[p].values()))[0]]
    s = copy.copy(base)
    s.by_pd = by_pd
    at = f"0x{va:08X}"
    why = ("site-conflict-both" if (rows[keys[0]].get("site") or (0,))[0] else "site-conflict-only") \
        + ":accepted-conflict-differs"  # 5.5 T28 (G16): the cause text; the agreeing sites are the base row's
    print(f"control pin-accepted-other-conflict: State copy, proto 0x{p:08X} of accepted {at} {pin_rows.mask(n)} "
          f"+ a site to retail 0x{rd:08X}")

    def expect(r_, v, g, i):
        new = _pin_new("pin-accepted-other-conflict", base, s.replant(pin_rows.PIN))
        # was: any(x.split()[2].startswith("site-conflict-") and x.split()[3] == at for x in new)
        return _pin_has(new, why, at), f"pin-row {why} {at}"
    return {"direct": True}, expect


def _c_pin_near_listed(d, config=None):
    """kw `direct` (5.1 T3.1, R77 ruled no slot route): a State copy whose by_pd (copied, base untouched) drops the
    agreeing sites (by_pd_rel too, 5.5 T28.c2) of a passing near row with only agreeing sites -> a fourth no-site near
    row, not in NEAR_LISTED (was NEAR_NO_SITE): a new `route-near:no-agreeing-site` fail line at it (the list does not
    grow by itself)."""
    base = pin_rows.state(config)
    near = [r for r in base.rows if r.get("cls") and r["cls"][1] == "near" and not r["fails"] and r.get("protos")
            and (r["va"], r["name"]) not in pin_rows.NEAR_LISTED and (r.get("site") or (0,))[0]
            and all(set(base.by_pd.get(p, {})) == {r["va"]} for p in r["protos"]) and not base.by_rd.get(r["va"], {})
            .keys() - set(r["protos"])]
    r0 = sorted(near, key=lambda r: r["va"])[0]
    by_pd = copy.copy(base.by_pd)
    for p in r0["protos"]:
        by_pd[p] = copy.copy(base.by_pd[p])
        del by_pd[p][r0["va"]]
    s = copy.copy(base)
    s.by_pd = by_pd
    s.by_pd_rel = copy.copy(base.by_pd_rel)  # 5.5 T28.c2: the near route reads the rel32 sites
    for p in r0["protos"]:
        s.by_pd_rel[p] = copy.copy(base.by_pd_rel[p])
        s.by_pd_rel[p].pop(r0["va"], None)
    at = f"0x{r0['va']:08X}"
    print(f"control pin-near-listed: State copy, the agreeing sites of near row {at} {pin_rows.mask(r0['name'])} "
          f"dropped ({len(near)} candidates)")

    def expect(r_, v, g, i):
        new = _pin_new("pin-near-listed", base, s.replant(pin_rows.PIN))
        return (any(x.split()[2:4] == ["route-near", at] and "route-near:no-agreeing-site" in x for x in new),
                f"pin-row route-near:no-agreeing-site {at}")
    return {"direct": True}, expect


def _pin_slotnames(pid, cut):
    """kw `direct` (5.3 T2.c4, M30): the first (by VA) `??_7` site-corr pin row whose slot names pass (5.5 T4.c3: with
    1+ agreeing site or slot_own, else its unplanted run fails `slot-not-independent`), retagged
    `+slot-names` corr-verified in memory; corr_route on the base State passes (positive path), then on a State copy
    cut(base, slots) -> `route-slotnames:<pid>`."""
    tag = _PP + "site-corr+slot-names"

    def make(d, config=None):
        base = pin_rows.state(config)
        r0 = next(r for r in sorted(base.rows, key=lambda r: r.get("va") or 0) if r.get("tag") == _PP + "site-corr"
                  and r["kind"] == "data" and r["name"].startswith("??_7") and len(r.get("protos", ())) == 1
                  and not base.slot_names(r["name"], r["protos"][0], r["va"])
                  and ((r.get("site") or (0,))[0] or (base.slot_own(r["name"], r["protos"][0], r["va"])  # 5.5 T4.c3
                                                      and not base.slot_rivals(r["name"], r["protos"][0], r["va"]))))
        slots = base.vt_slots(r0["protos"][0], r0["va"])

        def run(case, s):
            r = dict(r0, tag=tag, status="corr-verified", cls=pin_rows.TAGS[("data", tag)], fails=[], notes=[])
            pin_rows.State.corr_route(s, r)
            print(f"control pin-slotnames ({pid}): {case}: 0x{r0['va']:08X} {r0['name']} ({len(slots)} slots) "
                  f"fails {r['fails']}")
            return r["fails"]

        def expect(r_, v, g, i):
            s = copy.copy(base)
            cut(base, s, slots)
            return (run("unplanted", base) == [] and run("planted", s) == [f"route-slotnames:{pid}"],
                    f"pin-row route-slotnames:{pid}")
        return {"direct": True}, expect
    return make


def _slot_strip(base, s, slots, fake=None):
    """State copy s: the first slot's retail VA loses its symbols.txt and pin-row names (fake: one row in their place)."""
    rv = slots[0][1]
    s.sy_va, s.by_row_va = copy.copy(base.sy_va), copy.copy(base.by_row_va)
    s.sy_va[rv] = set()
    s.by_row_va[rv] = [fake] if fake else []


def _slot_site_corr(base, s, slots):
    pv, rv = slots[0]
    _slot_strip(base, s, slots, {"va": rv, "name": sorted(base.pubs[pv])[0], "status": "site-corr", "fails": []})


def _slot_count(base, s, slots):
    s.rt_slots = lambda va: base.rt_slots(va) + 1  # retail one slot longer than the proto table


_c_pin_slotnames_unnamed = _pin_slotnames("slot-unnamed", _slot_strip)
_c_pin_slotnames_site_corr = _pin_slotnames("slot-site-corr", _slot_site_corr)
_c_pin_slotnames_count = _pin_slotnames("slot-count-differs", _slot_count)


def _c_pin_slotnames_twin(d, config=None):
    """kw `direct` (5.5 T4.c3, M30 S2): the first (by VA) passing `+slot-names` row R and the first (by name) other
    proto `??_7` public T with R's proto slot targets (an equal-slot twin, census G) and no slot target a member of T's
    class (a base class's table names its own slots: own-class evidence, not a twin plant); R renamed in memory to T (protos =
    T's proto VA, r["site"] re-derived by site_test for that name, its site fails dropped); corr_route on R unrenamed
    passes, renamed -> `route-slotnames:slot-not-independent`."""
    base = pin_rows.state(config)
    tag = _PP + "site-corr+slot-names"
    r0, t, q = _slot_twin(base)

    def run(case, name, protos, site):
        r = dict(r0, name=name, protos=protos, tag=tag, status="corr-verified", cls=pin_rows.TAGS[("data", tag)],
                 fails=[], notes=[])
        if site:
            base.site_test(r)
            r["fails"] = []
        pin_rows.State.corr_route(base, r)
        print(f"control pin-slotnames-twin: {case}: 0x{r0['va']:08X} {name} site {r.get('site')} fails {r['fails']}")
        return r["fails"]

    def expect(r_, v, g, i):
        return (run("unplanted", r0["name"], r0["protos"], False) == []
                and run("planted", t, [q], True) == ["route-slotnames:slot-not-independent"],
                "pin-row route-slotnames:slot-not-independent")
    return {"direct": True}, expect


def _slot_twin(base):
    """(R, T, T's proto VA) of _c_pin_slotnames_twin (moved here in 5.5 T28, shared with pin-slotnames-dispatch)."""
    vts = sorted(n for n in base.byname if n.startswith("??_7") and len(base.byname[n]) == 1)
    plant = None
    for r0 in sorted((r for r in base.rows if r.get("cls") and r["cls"][1] == "slotnames" and not r["fails"]
                      and len(r.get("protos", ())) == 1), key=lambda r: r["va"]):
        p, va = r0["protos"][0], r0["va"]
        mine = [pv for pv, _rv in base.vt_slots(p, va)]
        for t in vts:
            q = next(iter(base.byname[t]))
            own = f"@{t[4:t.index('@@6B')]}@@" if "@@6B" in t else None
            if (own and t != r0["name"] and q != p and base.img.pbytes(q, 4) == base.img.pbytes(p, 4)
                    and not any(own in x for pv in mine for x in base.pubs.get(pv, ()))  # T names no slot (no base)
                    and [pv for pv, _rv in base.vt_slots(q, va)] == mine):
                plant = (r0, t, q)
                break
        if plant:
            break
    return plant


def _c_pin_slotnames_dispatch(d, config=None):
    """kw `direct` (5.5 T28, G3; R5.5-013 finding 2): the gate's dispatch (State.replant -> run_rows -> corr_route), not
    corr_route on a dict: _slot_twin's row R moved onto its equal-slot twin T (R's pin line renamed T, a pin file copy),
    rows re-run on a State copy whose site index (by_pd, by_rd copied, base untouched) drops T's proto sites and the
    sites into R's VA -> `route-slotnames:slot-not-independent` at R's VA."""
    base = pin_rows.state(config)
    r0, t, q = _slot_twin(base)
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    f = L[r0["line"] - 1].split()
    assert f[3] == r0["name"], f"pin-slotnames-dispatch: line {r0['line']} is not 0x{r0['va']:08X}'s row"
    f[3] = t
    L[r0["line"] - 1] = " ".join(f)
    p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
    s = copy.copy(base)
    s.by_pd, s.by_rd = copy.copy(base.by_pd), copy.copy(base.by_rd)
    s.by_pd.pop(q, None)
    s.by_rd.pop(r0["va"], None)
    at = f"0x{r0['va']:08X}"
    print(f"control pin-slotnames-dispatch: pin copy, {at} {pin_rows.mask(r0['name'])} renamed "
          f"{pin_rows.mask(t)} (proto 0x{q:08X}); its sites dropped on a State copy")

    def expect(r_, v, g, i):
        new = _pin_new("pin-slotnames-dispatch", base, s.replant(p))
        return (_pin_has(new, "route-slotnames:slot-not-independent", at),
                f"pin-row route-slotnames:slot-not-independent {at}")
    return {"direct": True}, expect


def _c_pin_slotnames_own_base(d, config=None):
    """kw `direct` (5.5 T28.c5, G8; R5.5-013 finding 5; the reverse of pin-slotnames-twin): the first (by VA) passing
    `+slot-names` row R with an equal-slot rival T (another `??_7` public, same proto slot list at R's VA) whose
    class owns a slot (slot_own(T) True); R's pin line renamed T (a pin file copy), the sites of T's proto table and
    into R's VA dropped on a State copy, replant -> `route-slotnames:slot-not-independent` at R's VA (pre-fix: slot_own
    alone passed it)."""
    base = pin_rows.state(config)
    vts = sorted(n for n in base.byname if n.startswith("??_7") and len(base.byname[n]) == 1)
    plant = None
    for r0 in sorted((r for r in base.rows if r.get("cls") and r["cls"][1] == "slotnames" and not r["fails"]
                      and len(r.get("protos", ())) == 1), key=lambda r: r["va"]):
        p, va = r0["protos"][0], r0["va"]
        mine = [pv for pv, _rv in base.vt_slots(p, va)]
        plant = next(((r0, t, q) for t in vts for q in base.byname[t] if q != p and base.slot_own(t, q, va)
                      and [pv for pv, _rv in base.vt_slots(q, va)] == mine), None)
        if plant:
            break
    assert plant, "pin-slotnames-own-base: no slot-names row with an own-slot rival"
    r0, t, q = plant
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    f = L[r0["line"] - 1].split()
    assert f[3] == r0["name"], f"pin-slotnames-own-base: line {r0['line']} is not 0x{r0['va']:08X}'s row"
    f[3] = t
    L[r0["line"] - 1] = " ".join(f)
    p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
    s = copy.copy(base)
    s.by_pd, s.by_rd = copy.copy(base.by_pd), copy.copy(base.by_rd)
    s.by_pd.pop(q, None)
    s.by_rd.pop(r0["va"], None)
    at = f"0x{r0['va']:08X}"
    print(f"control pin-slotnames-own-base: pin copy, {at} {r0['name']} renamed {t} (proto 0x{q:08X}, owns a slot); "
          f"its sites dropped on a State copy")

    def expect(r_, v, g, i):
        new = _pin_new("pin-slotnames-own-base", base, s.replant(p))
        return (_pin_has(new, "route-slotnames:slot-not-independent", at),
                f"pin-row route-slotnames:slot-not-independent {at}")
    return {"direct": True}, expect


def _c_pin_data_two_names(d, config=None):
    """kw `direct` (5.5 T28.c5, G12; R5.5-013 finding 3): a pin copy with a second data row at the first (by VA) passing
    data-route (`global`) row R's VA, named by the first (by name) untabled data public of one proto VA that is not
    at R's proto VA (another object, no alias); replant -> `va-two-names` at R's VA (pre-fix: data rows never failed
    it)."""
    base = pin_rows.state(config)
    r0 = next(r for r in sorted(base.rows, key=lambda r: r.get("va") or 0) if r.get("cls") and r["cls"][1] == "global"
              and r["kind"] == "data" and not r["fails"] and len(r.get("protos", ())) == 1)
    n2 = next(n for n in sorted(base.byname) if len(base.byname[n]) == 1 and n not in base.tab_name
              and not n.startswith(("??_C@", "??_7", "__real@")) and n not in base.pubs.get(r0["protos"][0], ())
              and not base.img.pexec(next(iter(base.byname[n]))))
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    f = L[r0["line"] - 1].split()
    assert f[3] == r0["name"], f"pin-data-two-names: line {r0['line']} is not 0x{r0['va']:08X}'s row"
    L.insert(r0["line"], " ".join(f[:3] + [n2] + f[4:]))
    p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
    at = f"0x{r0['va']:08X}"
    print(f"control pin-data-two-names: pin copy, + a data row {at} {pin_rows.mask(n2)} beside "
          f"{pin_rows.mask(r0['name'])}")

    def expect(r_, v, g, i):
        new = _pin_new("pin-data-two-names", base, base.replant(p))
        return _pin_has(new, "va-two-names", at), f"pin-row va-two-names {at}"
    return {"direct": True}, expect


def _pin_bracket(pid, cut, pick=None):
    """kw `direct` (5.3 T2.c4, T1 ruling): the first (by VA) passing `func`-route pin row whose bracket holds with room
    for a second body, retagged `+unit-bracket+callee-bytes` in memory; route() on the base State passes (positive
    path), then on a State copy cut(base, s, r0, p, ln, w0, w1) -> `route-bracket:<pid>`. 5.5 T28: pick(base, r0, p,
    ln), when given, must also hold for the row."""
    tag = _PP + "unit-bracket+callee-bytes"

    def make(d, config=None):
        base = pin_rows.state(config)
        for r0 in sorted((r for r in base.rows if r.get("cls") and r["cls"][1] == "func" and not r["fails"]
                          and len(r.get("protos", ())) == 1 and base.procs.get(r["protos"][0])), key=lambda r: r["va"]):
            p = r0["protos"][0]
            ln = base.procs[p]
            if base.bracket(p, r0["va"], ln) is None:
                m = next(iter(base.pmods[p]))
                an = base.anchors(m)
                lo, hi = max(a for a in an if a[0] < p), min(a for a in an if a[0] > p)
                w0, w1 = lo[1] + lo[2], hi[1]
                if (w0 + ln <= r0["va"] or r0["va"] + 2 * ln <= w1) and (pick is None or pick(base, r0, p, ln)):
                    break

        def run(case, s):
            r = dict(r0, tag=tag, cls=pin_rows.TAGS[("func", tag)], fails=[], notes=[])
            pin_rows.State.route(s, r)
            print(f"control pin-bracket ({pid}): {case}: 0x{r0['va']:08X} {pin_rows.mask(r0['name'])} (len 0x{ln:X}, "
                  f"bracket 0x{w0:08X}..0x{w1:08X}) fails {r['fails']}")
            return r["fails"]

        def expect(r_, v, g, i):
            s = copy.copy(base)
            cut(base, s, r0, p, ln, w0, w1)
            return (run("unplanted", base) == [] and run("planted", s) == [f"route-bracket:{pid}"],
                    f"pin-row route-bracket:{pid}")
        return {"direct": True}, expect
    return make


def _bracket_outside(base, s, r0, p, ln, w0, w1):
    """The lower anchor's retail body moved to start at the row's VA: the row now lies before the bracket's start."""
    m = next(iter(base.pmods[p]))
    an = base.anchors(m)
    lo = max(a for a in an if a[0] < p)
    s.banc = dict(base.banc)
    s.banc[m] = sorted([a for a in an if a != lo] + [(lo[0], r0["va"], lo[2])])


def _bracket_second(base, s, r0, p, ln, w0, w1):
    """A copy of the row's retail body planted at another offset inside the bracket (an image copy, base untouched)."""
    img = s.img = copy.copy(base.img)
    va = r0["va"]
    t2 = w0 if w0 + ln <= va else va + ln
    xb = bytearray(img.xb)
    o, o2 = pin_rows.xbe_info.va_to_off(img.x, va), pin_rows.xbe_info.va_to_off(img.x, t2)
    xb[o2:o2 + ln] = xb[o:o + ln]
    img.xb = bytes(xb)
    print(f"control pin-bracket-ambiguous: body 0x{va:08X} copied to 0x{t2:08X}")


def _bracket_anchor_unit(base, s, r0, p, ln, w0, w1):
    """5.5 T28 (G9, D136): every matched name of a proc of P's module above P that lies in the unit's text range moved
    past the range's end (another unit's VA, order kept; s.matched copied, anchors rebuilt): no anchor above P is left
    inside the unit, so the D136 filter (pin_rows.anchors) leaves none -> `no-anchor`; without the filter the moved
    names would bracket P from another unit."""
    m = next(iter(base.pmods[p]))
    u0, u1 = base.utext[m]
    s.matched, s.banc = dict(base.matched), {}
    moved = 0
    for x in sorted(base.mprocs[m]):
        for n in sorted(base.pubs.get(x, ())) if x > p else ():
            if n in base.matched and u0 <= base.matched[n][0] < u1:
                s.matched[n] = (base.matched[n][0] - u0 + u1, base.matched[n][1])
                moved += 1
    print(f"control pin-bracket-anchor-unit: {moved} matched names above 0x{p:08X} moved past unit end 0x{u1:08X}")


def _bracket_unit(base, s, r0, p, ln, w0, w1):
    """5.5 T28 (G9): the unit's text range ends at the row's VA (s.utext copied); the anchors kept as base's (s.banc
    copied: the range edit reaches the `outside-unit` test alone, not the D136 anchor filter)."""
    m = next(iter(base.pmods[p]))
    s.utext, s.banc = dict(base.utext), dict(base.banc)
    s.utext[m] = (base.utext[m][0], r0["va"])
    print(f"control pin-bracket-outside-unit: unit text range end 0x{base.utext[m][1]:08X} -> 0x{r0['va']:08X}")


def _bracket_has_out(base, r0, p, ln):
    return bool(base.outs(p, r0["va"], ln))


def _bracket_callee(base, s, r0, p, ln, w0, w1):
    """5.5 T28 (G9): the row's first out-of-body callee (proto dest pd, retail dest rd) loses its table names (s.tab_va
    copied) and rd's first byte is flipped in an image copy (base untouched): no table name, not masked-equal ->
    `callee-down`."""
    pd, rd = base.outs(p, r0["va"], ln)[0]
    s.tab_va = copy.copy(base.tab_va)
    s.tab_va[rd] = set()
    img = s.img = copy.copy(base.img)
    xb = bytearray(img.xb)
    o = pin_rows.xbe_info.va_to_off(img.x, rd)
    xb[o] ^= 0xFF
    img.xb = bytes(xb)
    print(f"control pin-bracket-callee-down: callee proto 0x{pd:08X} / retail 0x{rd:08X}: table names dropped, "
          f"first byte flipped")


_c_pin_bracket_outside = _pin_bracket("outside-bracket", _bracket_outside)
_c_pin_bracket_ambiguous = _pin_bracket("ambiguous", _bracket_second)
_c_pin_bracket_anchor_unit = _pin_bracket("no-anchor", _bracket_anchor_unit)  # 5.5 T28 (G9)
_c_pin_bracket_outside_unit = _pin_bracket("outside-unit", _bracket_unit)
_c_pin_bracket_callee_down = _pin_bracket("callee-down", _bracket_callee, _bracket_has_out)


OWN_DTOR = 0x000A3EC9  # 5.5 T4.c2: ??_E/??_GPropellerMain, DISABLED in 5.3 T13.2 (body reaches ??1CritterMain's path)


def _c_pin_bracket_own_dtor(d, config=None):
    """kw `direct` (5.5 T4.c2, name_sites.amb_bracket_down): a pin file copy with OWN_DTOR's two commented-out bracket
    lines re-enabled, rows re-run (State.replant) -> a new `route-bracket:dtor-not-own` fail line at OWN_DTOR."""
    base = pin_rows.state(config)
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    ix = [k for k, ln in enumerate(L) if ln.startswith(f"# 0x{OWN_DTOR:08X} func ")]
    for k in ix:
        L[k] = L[k][2:]
    p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
    print(f"control pin-bracket-own-dtor: pin copy, {len(ix)} lines at 0x{OWN_DTOR:08X} re-enabled")

    def expect(r_, v, g, i):
        new = _pin_new("pin-bracket-own-dtor", base, base.replant(p))
        return (len(ix) == 2 and any(x.split()[2:4] == ["route-bracket", f"0x{OWN_DTOR:08X}"]
                                     and "route-bracket:dtor-not-own" in x for x in new),
                f"pin-row route-bracket:dtor-not-own 0x{OWN_DTOR:08X}")
    return {"direct": True}, expect


def _c_pin_bracket_dtor_untabled(d, config=None):
    """kw `direct` (5.5 T28, G14; R5.5-013 finding 9): _c_pin_bracket_own_dtor's plant with every `??1<Class>@@` of
    OWN_DTOR's ??_G/??_E rows untabled (their symbols.txt names dropped on a State copy, sy_va copied, base untouched;
    their pin lines dropped from the copy) -> `route-bracket:dtor-not-own` at OWN_DTOR (was: skipped, no table X)."""
    base = pin_rows.state(config)
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    ix = [k for k, ln in enumerate(L) if ln.startswith(f"# 0x{OWN_DTOR:08X} func ")]
    for k in ix:
        L[k] = L[k][2:]
    owns = tuple(sorted({"??1" + n[4:n.index("@@") + 2] for n in (L[k].split()[3] for k in ix)
                         if n.startswith(("??_G", "??_E")) and "@@" in n}))
    keep = [ln for ln in L if ln.startswith("#") or not ln.split()[3:4] or not ln.split()[3].startswith(owns)]
    s = copy.copy(base)
    s.sy_va = {v: {x for x in ns if not x.startswith(owns)} for v, ns in base.sy_va.items()}
    dropped = sum(len(ns) for ns in base.sy_va.values()) - sum(len(ns) for ns in s.sy_va.values())
    p = _pin_scratch(d, pin_rows.PIN, "\n".join(keep) + "\n")
    at = f"0x{OWN_DTOR:08X}"
    print(f"control pin-bracket-dtor-untabled: pin copy, {len(ix)} lines at {at} re-enabled; {len(owns)} own dtor "
          f"prefixes untabled ({dropped} symbols.txt names, {len(L) - len(keep)} pin lines dropped)")

    def expect(r_, v, g, i):
        new = _pin_new("pin-bracket-dtor-untabled", base, s.replant(p))
        return (len(ix) == 2 and bool(owns) and dropped + len(L) - len(keep) > 0
                and _pin_has(new, "route-bracket:dtor-not-own", at), f"pin-row route-bracket:dtor-not-own {at}")
    return {"direct": True}, expect


def _rm_lit(img, va, ln):
    """The retail literals of body va over ln: [(literal VA, bytes through NUL)] (abs dests in .rdata/.data)."""
    rb = img.rbytes(va, ln)
    out = []
    for o, _w, k in pin_rows.mask_proc(rb, va, img.rlo, img.rhi)[0]:
        d = pin_rows.target(rb, va, o, k)
        s = img.rsec(d)
        if k == "abs" and s and s.get("name") in (".rdata", ".data"):
            raw = img.rbytes(d, s["virtualAddr"] + s["rawSize"] - d)
            i = raw.find(b"\0", 0, pin_rows.STR_CAP)
            if i >= 0:
                out.append((d, raw[:i + 1]))
    return out


def _c_pin_registerme_literal(d, config=None):
    """kw `direct` (5.4 T23, R418 rule a): the first (by VA) passing pin row named `??0RegisterMe@X@@QAE@XZ`, retagged
    `+literal+callee-bytes` in memory; route() on the base State passes, then on a State copy whose retail image holds
    another RegisterMe row's literal at this row's literal VA -> `route-registerme:literal-differs`."""
    tag = _PP + "literal+callee-bytes"
    base = pin_rows.state(config)
    rows = sorted((r for r in base.rows if re.fullmatch(r"\?\?0RegisterMe@\w+@@QAE@XZ", r["name"]) and not r["fails"]
                   and r.get("protos")), key=lambda r: r["va"])
    r0, r1 = rows[0], rows[1]
    (lva, lit0), = _rm_lit(base.img, r0["va"], base.procs[r0["protos"][0]])
    (_v1, lit1), = _rm_lit(base.img, r1["va"], base.procs[r1["protos"][0]])

    def run(case, s):
        r = dict(r0, tag=tag, cls=pin_rows.TAGS[("func", tag)], fails=[], notes=[])
        pin_rows.State.route(s, r)
        print(f"control pin-registerme-literal: {case}: 0x{r0['va']:08X} {pin_rows.mask(r0['name'])} fails {r['fails']}")
        return r["fails"]

    def expect(r_, v, g, i):
        s = copy.copy(base)
        img = s.img = copy.copy(base.img)
        xb = bytearray(img.xb)
        o = pin_rows.xbe_info.va_to_off(img.x, lva)
        xb[o:o + len(lit1)] = lit1
        img.xb = bytes(xb)
        print(f"control pin-registerme-literal: literal 0x{lva:08X} ({len(lit0)} B) <- row 0x{r1['va']:08X}'s "
              f"({len(lit1)} B)")
        return (run("unplanted", base) == [] and run("planted", s) == ["route-registerme:literal-differs"],
                "pin-row route-registerme:literal-differs")
    return {"direct": True}, expect


def _pin_rm(pid, plant):
    """kw `direct` (5.5 T4.c2, I76): _c_pin_registerme_literal's row r0, retagged `+literal+callee-bytes` in memory;
    route() on the base State passes, then on a State copy whose image copies (both byte buffers copied, base
    untouched) plant(base, img, r0, lva, lit0) edited -> `route-registerme:literal-<pid>`. The checks exist already
    (5.4 T23): each control fails from its first run."""
    tag = _PP + "literal+callee-bytes"

    def make(d, config=None):
        base = pin_rows.state(config)
        r0 = min((r for r in base.rows if re.fullmatch(r"\?\?0RegisterMe@\w+@@QAE@XZ", r["name"]) and not r["fails"]
                  and r.get("protos")), key=lambda r: r["va"])
        (lva, lit0), = _rm_lit(base.img, r0["va"], base.procs[r0["protos"][0]])

        def run(case, s):
            r = dict(r0, tag=tag, cls=pin_rows.TAGS[("func", tag)], fails=[], notes=[])
            pin_rows.State.route(s, r)
            print(f"control pin-registerme-{pid}: {case}: 0x{r0['va']:08X} {pin_rows.mask(r0['name'])} fails {r['fails']}")
            return r["fails"]

        def expect(r_, v, g, i):
            s = copy.copy(base)
            img = s.img = copy.copy(base.img)
            img.xb, img.exe = bytearray(img.xb), bytearray(img.exe)
            plant(base, img, r0, lva, lit0)
            img.xb, img.exe = bytes(img.xb), bytes(img.exe)
            return (run("unplanted", base) == [] and run("planted", s) == [f"route-registerme:literal-{pid}"],
                    f"pin-row route-registerme:literal-{pid}")
        return {"direct": True}, expect
    return make


def _rm_unterminated(base, img, r0, lva, lit0):
    """The retail literal and the bytes after it overwritten with `A` up to STR_CAP (or the section end): no NUL, so
    the retail side pushes no literal."""
    s = img.rsec(lva)
    k = min(pin_rows.STR_CAP, s["virtualAddr"] + s["rawSize"] - lva)
    o = pin_rows.xbe_info.va_to_off(img.x, lva)
    img.xb[o:o + k] = b"A" * k
    print(f"control pin-registerme-count: retail literal 0x{lva:08X}: {k} bytes `A`, no NUL")


def _rm_other_text(base, img, r0, lva, lit0):
    """The literal's first byte set to `#` in both images (proto: the abs dest with a `??_C@` public): bytes still
    equal, text no longer the class name."""
    p = r0["protos"][0]
    pb = img.pbytes(p, base.procs[p])
    pd = next(pin_rows.target(pb, p, o, k) for o, _w, k in pin_rows.mask_proc(pb, p, img.plo, img.phi)[0]
              if k == "abs" and any(x.startswith("??_C@") for x in base.pubs.get(pin_rows.target(pb, p, o, k), ())))
    s = img.psec(pd)
    po = s["rawptr"] + pd - img.base - s["va"]
    ro = pin_rows.xbe_info.va_to_off(img.x, lva)
    img.exe[po] = img.xb[ro] = ord("#")
    print(f"control pin-registerme-not-class: literal proto 0x{pd:08X} / retail 0x{lva:08X} first byte -> `#`")


_c_pin_registerme_count = _pin_rm("count", _rm_unterminated)
_c_pin_registerme_not_class = _pin_rm("not-class", _rm_other_text)


HAND_MOVED = None  # 5.5 T26: picked by _hand_pick; was (0x000D9104, "?Play@SoundStreamer@@QAEXXZ"), disabled in 5.5 T7
HAND_REFUSED = "?SETTINGS_FILE@@3PBDB"  # 5.5 T4: in compiler_pin.md only in a Refused bullet (5.4 T25.c1)


def _pin_hand(pid, plant):
    """kw `direct` (5.5 T4): a pin file copy with plant(base, L) applied, rows re-run (State.replant) -> a new
    `hand-row-not-listed` fail line at the planted VA (the row's (VA, name) is not in config/pin_hand.txt)."""
    def make(d, config=None):
        base = pin_rows.state(config)
        L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
        at = plant(base, L)
        p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")

        def expect(r_, v, g, i):
            new = _pin_new(pid, base, base.replant(p))
            return (any(x.split()[2:4] == ["hand-row-not-listed", at] for x in new), f"pin-row hand-row-not-listed {at}")
        return {"direct": True}, expect
    return make


def _hand_pick(base):
    """5.5 T26: the first (by VA) passing func row on the hand route, listed in config/pin_hand.txt, that no retail
    relocation reaches (base.by_rd empty at its VA)."""
    r = min((r for r in base.rows if not r["fails"] and r.get("kind") == "func" and r.get("cls")
             and r["cls"][1] == "hand" and (r["va"], r["name"]) in base.hand and not base.by_rd.get(r["va"])),
            key=lambda r: r["va"])
    return r["va"], r["name"]


def _hand_moved(base, L):
    """HAND_MOVED's line moved to the first (by VA) end of a sized symbols.txt function that opens with a prologue byte,
    no table names and no site reaches."""
    va0, n = HAND_MOVED or _hand_pick(base)
    sizes = {int(f[0], 16): int(f[2], 16) for _i, _l, f in pin_rows.table_lines(pin_rows.SYMS)
             if f[1] == "func" and f[2] != "-"}
    to = next(v + z for v, z in sorted(sizes.items()) if v + z not in base.tab_va and not base.by_rd.get(v + z)
              and v + z not in sizes and (base.img.rbytes(v + z, 1) or b"\0")[0] in (0x55, 0x56, 0x53, 0x8B, 0x83, 0x6A))
    i = next(k for k, ln in enumerate(L) if ln.split()[:1] == [f"0x{va0:08X}"] and ln.split()[3:4] == [n])
    L[i] = f"0x{to:08X}" + L[i][10:]
    print(f"control pin-hand-moved: pin copy, 0x{va0:08X} {pin_rows.mask(n)} moved to unreached 0x{to:08X}")
    return f"0x{to:08X}"


def _hand_refused_name(base, L):
    """A data row with a hand tag named HAND_REFUSED at the refused need's .data VA (compiler_pin.md T25.c1 bullet)."""
    va = 0x0021EE68
    L.append(f"0x{va:08X} data - {HAND_REFUSED} {_PP}retail-disasm disasm-verified")
    print(f"control pin-hand-refused-name: pin copy + a hand data row 0x{va:08X} {pin_rows.mask(HAND_REFUSED)}")
    return f"0x{va:08X}"


def _c_pin_twin_wrong(d, config=None):
    """kw `direct` (5.5 T4, C0414; T26: twin-route rows): the first (by VA) passing creationFunc pin row a RegisterMe pin row's push names,
    retagged `+registerme-twin` corr-verified in memory; twin() on the base State passes, then with the name of another
    class's creationFunc (one with its own RegisterMe row) -> `route-twin:twin-differs`."""
    tag = _PP + "registerme-twin"
    base = pin_rows.state(config)
    rm = {re.fullmatch(r"\?\?0RegisterMe@(\w+)@@QAE@XZ", r["name"]).group(1) for r in base.rows
          if re.fullmatch(r"\?\?0RegisterMe@\w+@@QAE@XZ", r.get("name", "")) and not r["fails"]}
    cf = sorted((r for r in base.rows if not r["fails"] and r.get("protos") and r.get("kind") == "func"
                 and (m := re.match(r"\?creationFunc@(\w+)@@", r["name"])) and m.group(1) in rm
                 and r["cls"] and r["cls"][1] == "twin"), key=lambda r: r["va"])  # was "hand" (5.5 T26: T7 retagged)
    r0, r1 = cf[0], cf[1]

    def run(case, name):
        r = dict(r0, name=name, tag=tag, status="corr-verified", cls=pin_rows.TAGS[("func", tag)],
                 protos=sorted(base.byname.get(name, ())), fails=[], notes=[])
        pin_rows.State.twin(base, r)
        print(f"control pin-twin-wrong: {case}: 0x{r0['va']:08X} {pin_rows.mask(name)} fails {r['fails']}")
        return r["fails"]

    def expect(r_, v, g, i):
        return (run("unplanted", r0["name"]) == [] and run("planted", r1["name"]) == ["route-twin:twin-differs"],
                "pin-row route-twin:twin-differs")
    return {"direct": True}, expect


def _c_matched_twin_rowok(d, config=None):
    """kw `direct` (5.5 T27, D137): matched_set._pin_row's twin route on in-memory pins (symbols_pin.txt untouched): the
    first (by VA) passing twin creationFunc row's name, sized, corr-verified +registerme-twin -> admitted; the second's
    name at that VA, same pin, its pin row run through State.twin (-> route-twin:twin-differs) and planted on a State
    copy under config (restored by undo) -> refused naming the pin-row check."""
    base = pin_rows.state(config)
    rm = {re.fullmatch(r"\?\?0RegisterMe@(\w+)@@QAE@XZ", r["name"]).group(1) for r in base.rows
          if re.fullmatch(r"\?\?0RegisterMe@\w+@@QAE@XZ", r.get("name", "")) and not r["fails"]}
    cf = sorted((r for r in base.rows if not r["fails"] and r.get("protos") and r.get("kind") == "func"
                 and (m := re.match(r"\?creationFunc@(\w+)@@", r["name"])) and m.group(1) in rm
                 and r["cls"] and r["cls"][1] == "twin"), key=lambda r: r["va"])  # as _c_pin_twin_wrong
    r0, r1 = cf[0], cf[1]
    tag = matched_set.TWIN_TAG

    def undo():
        pin_rows._STATE[config] = base

    def run(case, name):
        pins = {name: [{"va": r0["va"], "kind": "func", "size": 0x10, "status": "corr-verified", "tag": tag}]}
        got = matched_set._pin_row({}, pins, name, config)
        print(f"control matched-twin-rowok: {case}: 0x{r0['va']:08X} {pin_rows.mask(name)} sized twin pin -> "
              f"{'admitted' if isinstance(got, tuple) else got.replace(name, pin_rows.mask(name))}")
        return got

    def expect(r_, v, g, i):
        ok0 = isinstance(run("unplanted", r0["name"]), tuple)
        pr = dict(r0, name=r1["name"], tag=tag, status="corr-verified", cls=pin_rows.TAGS[("func", tag)],
                  protos=sorted(base.byname.get(r1["name"], ())), fails=[], notes=[])
        pin_rows.State.twin(base, pr)
        st = copy.copy(base)
        st.by_row_name = dict(base.by_row_name, **{r1["name"]: [pr]})
        pin_rows._STATE[config] = st
        try:
            got = run("planted", r1["name"])
        finally:
            undo()
        return (ok0 and isinstance(got, str) and "pin-row check: route-twin:twin-differs" in got,
                "matched_set _pin_row refused (pin-row check route-twin:twin-differs)")
    return {"direct": True, "undo": undo}, expect


def _twin_rows(base):
    """5.5 T28: the passing twin-route creationFunc pin rows whose class has a passing RegisterMe row, by VA (the pick of
    _c_pin_twin_wrong and _c_matched_twin_rowok)."""
    rm = {re.fullmatch(r"\?\?0RegisterMe@(\w+)@@QAE@XZ", r["name"]).group(1) for r in base.rows
          if re.fullmatch(r"\?\?0RegisterMe@\w+@@QAE@XZ", r.get("name", "")) and not r["fails"]}
    return sorted((r for r in base.rows if not r["fails"] and r.get("protos") and r.get("kind") == "func"
                   and (m := re.match(r"\?creationFunc@(\w+)@@", r["name"])) and m.group(1) in rm
                   and r["cls"] and r["cls"][1] == "twin"), key=lambda r: r["va"])


def _c_pin_twin_dispatch(d, config=None):
    """kw `direct` (5.5 T28, G3; R5.5-013 finding 2): the gate's dispatch (State.replant -> run_rows -> twin), not
    State.twin on a dict: a pin file copy with the first two _twin_rows rows' names swapped -> `route-twin:twin-differs`
    at both VAs; matched_set._pin_row over that State (pin_rows._STATE under config, restored by undo) refuses the
    second's name, sized corr-verified TWIN_TAG at the first's VA, with the cause row_ok gives."""
    base = pin_rows.state(config)
    r0, r1 = _twin_rows(base)[:2]
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    for a, b in ((r0, r1), (r1, r0)):
        f = L[a["line"] - 1].split()
        assert f[3] == a["name"], f"pin-twin-dispatch: line {a['line']} is not 0x{a['va']:08X}'s row"
        f[3] = b["name"]
        L[a["line"] - 1] = " ".join(f)
    p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
    at = [f"0x{r['va']:08X}" for r in (r0, r1)]
    print(f"control pin-twin-dispatch: pin copy, names of {at[0]} and {at[1]} swapped")

    def undo():
        pin_rows._STATE[config] = base

    def expect(r_, v, g, i):
        s = base.replant(p)
        new = _pin_new("pin-twin-dispatch", base, s)
        ok = all(_pin_has(new, "route-twin:twin-differs", a) for a in at)
        pins = {r1["name"]: [{"va": r0["va"], "kind": "func", "size": r0["size"], "status": "corr-verified",
                              "tag": matched_set.TWIN_TAG}]}
        pin_rows._STATE[config] = s
        try:
            got = matched_set._pin_row({}, pins, r1["name"], config)
        finally:
            undo()
        print(f"control pin-twin-dispatch: _pin_row {at[0]} {pin_rows.mask(r1['name'])}: "
              f"{'admitted' if isinstance(got, tuple) else got.replace(r1['name'], pin_rows.mask(r1['name']))}")
        return (ok and isinstance(got, str) and got.endswith("pin-row check: route-twin:twin-differs"),
                f"pin-row route-twin:twin-differs {at[0]} {at[1]}, _pin_row refused")
    return {"direct": True, "undo": undo}, expect


def _pin_twin_cause(pid, keep):
    """kw `direct` (5.5 T28, G17): the first _twin_rows row r0, rows re-run (State.replant -> run_rows -> twin) on a
    State copy whose registerme_pushes keeps keep(r0, r1, pushes) of the real pushes -> `route-twin:<pid>` at r0."""
    def make(d, config=None):
        base = pin_rows.state(config)
        r0, r1 = _twin_rows(base)[:2]
        s = copy.copy(base)
        s.registerme_pushes = lambda live: keep(r0, r1, pin_rows.State.registerme_pushes(s, live))
        at = f"0x{r0['va']:08X}"

        def expect(r_, v, g, i):
            new = _pin_new(f"pin-twin-{pid}", base, s.replant(pin_rows.PIN))
            return _pin_has(new, f"route-twin:{pid}", at), f"pin-row route-twin:{pid} {at}"
        return {"direct": True}, expect
    return make


def _twin_nosite(r0, r1, pushes):
    """The pushes naming r0 (proto dest one of its publics or retail dest its VA) dropped."""
    out = [t for t in pushes if not (t[1] in r0["protos"] or t[2] == r0["va"])]
    print(f"control pin-twin-no-registerme-site: 0x{r0['va']:08X}: {len(pushes) - len(out)} pushes dropped")
    return out


def _twin_class(r0, r1, pushes):
    """The agreeing pushes of r0 relabelled with r1's class (a RegisterMe caller of another class)."""
    c1 = re.match(r"\?creationFunc@(\w+)@@", r1["name"]).group(1)
    out = [(c1 if t[1] in r0["protos"] and t[2] == r0["va"] else t[0], t[1], t[2]) for t in pushes]
    print(f"control pin-twin-class: 0x{r0['va']:08X}: {sum(a != b for a, b in zip(out, pushes))} pushes relabelled "
          f"{pin_rows.mask(c1)}")
    return out


_c_pin_twin_no_site = _pin_twin_cause("no-registerme-site", _twin_nosite)
_c_pin_twin_class = _pin_twin_cause("twin-class", _twin_class)


def _c_pin_site_nopublic(d, config=None):
    """kw `direct` (5.5 T28, G13; R5.5-013 finding 4): a State copy whose by_rd (copied, base untouched) gives the
    first (by VA) passing near row with an agreeing site one more site, from the first (by VA) proto function start
    with no public -> a new `site-conflict-both` fail line at it (was: `if names and n not in names`, no conflict)."""
    base = pin_rows.state(config)
    r0 = min((r for r in base.rows if not r["fails"] and r.get("cls") and r["cls"][1] == "near" and r.get("protos")
              and (r.get("site") or (0,))[0]), key=lambda r: r["va"])
    pd = min(v for v in base.procs if not base.pubs.get(v))
    s = copy.copy(base)
    s.by_rd = copy.copy(base.by_rd)
    s.by_rd[r0["va"]] = copy.copy(base.by_rd[r0["va"]])
    s.by_rd[r0["va"]][pd] = [pd]
    at = f"0x{r0['va']:08X}"
    print(f"control pin-site-nopublic: State copy, near row {at} {pin_rows.mask(r0['name'])} + a site from "
          f"no-public proc 0x{pd:08X}")

    def expect(r_, v, g, i):
        new = _pin_new("pin-site-nopublic", base, s.replant(pin_rows.PIN))
        return _pin_has(new, "site-conflict-both", at), f"pin-row site-conflict-both {at}"
    return {"direct": True}, expect


_c_pin_hand_moved = _pin_hand("pin-hand-moved", _hand_moved)
_c_pin_hand_refused_name = _pin_hand("pin-hand-refused-name", _hand_refused_name)


def _c_pin_bracket_unit_range(d, config=None):
    """kw `direct` (5.5 T28.c4, G7, D136): a units.toml copy (a State copy's units_path; its utext too) with the text
    range of the first bracket-row module whose procs carry matched names D136 excludes (retail VA outside the range)
    widened over them (the anchors re-admitted), rows re-run (State.replant) -> `units-text <unit> ... differs`."""
    base = pin_rows.state(config)
    text = pin_rows.UNITS.read_text(encoding="utf-8")
    key = lambda m: f'name = "{m.replace(chr(92), chr(92) * 2)}"\n'  # noqa: E731 (TOML basic string)
    mods = sorted({next(iter(base.pmods[r["protos"][0]])) for r in base.rows if r.get("cls") and r["cls"][1] == "bracket"
                   and r.get("protos") and len(base.pmods.get(r["protos"][0], ())) == 1} & set(base.utext))
    for m in mods:
        u0, u1 = base.utext[m]
        ex = sorted(base.matched[n][0] for x in base.mprocs.get(m, ()) for n in base.pubs.get(x, ())
                    if n in base.matched and not u0 <= base.matched[n][0] < u1)
        if ex and text.count(key(m)) == 1:
            break
    else:
        raise AssertionError(f"no bracket-row module with a D136-excluded matched name ({len(mods)} modules)")
    w0, w1 = min(u0, ex[0]), max(u1, ex[-1] + 1)
    i = text.index(key(m))
    j = text.find("[[unit]]", i)
    j = len(text) if j < 0 else j
    blk = re.sub(r"^text_start = 0x[0-9A-Fa-f]+", f"text_start = 0x{w0:08X}", text[i:j], count=1, flags=re.M)
    blk = re.sub(r"^text_end = 0x[0-9A-Fa-f]+", f"text_end = 0x{w1:08X}", blk, count=1, flags=re.M)
    p = _pin_scratch(d, pin_rows.UNITS, text[:i] + blk + text[j:])
    s = copy.copy(base)
    s.units_path, s.utext, s.banc = p, dict(base.utext), {}
    s.utext[m] = (w0, w1)
    print(f"control pin-bracket-unit-range: units.toml copy, {m} 0x{u0:08X}..0x{u1:08X} -> 0x{w0:08X}..0x{w1:08X} "
          f"({len(ex)} D136-excluded matched names re-admitted)")

    def expect(r_, v, g, i_):
        new = _pin_new("pin-bracket-unit-range", base, s.replant(pin_rows.PIN))
        return (any(x.split()[2:4] == ["units-text", m] and " differs " in x for x in new),
                f"pin-row units-text {m} differs")
    return {"direct": True}, expect


def _c_pin_hand_retag(d, config=None):
    """kw `direct` (5.5 T28.c4, G15): a pin copy with the first (by VA) passing listed hand/xref row's tag changed to
    another hand/xref tag of the same kind, status and public claim (TAGS; `crt-absolute-0`, VA 0, neither side: the
    retag must pass the pre-T28.c4 rows), rows re-run (State.replant) -> `hand-tag-differs` at it."""
    base = pin_rows.state(config)

    def alts(r):
        return sorted(t for (k, t), c in pin_rows.TAGS.items() if k == r["kind"] and c[1] in ("hand", "xref")
                      and c[0] == r["cls"][0] and c[2] == r["cls"][2] and t not in (r["tag"], "crt-absolute-0")
                      and r["tag"] != "crt-absolute-0")
    r0 = min((r for r in base.rows if not r["fails"] and r.get("cls") and r["cls"][1] in ("hand", "xref")
              and (r["va"], r["name"]) in base.hand and alts(r)), key=lambda r: r["va"])
    at, tag = f"0x{r0['va']:08X}", alts(r0)[0]
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    k = next(j for j, ln in enumerate(L) if ln.split()[:1] == [at] and ln.split()[3:4] == [r0["name"]])
    f = L[k].split()
    f[4] = tag
    L[k] = " ".join(f)
    p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
    print(f"control pin-hand-retag: pin copy, {at} {pin_rows.mask(r0['name'])} tag {r0['tag']} -> {tag}")

    def expect(r_, v, g, i):
        new = _pin_new("pin-hand-retag", base, base.replant(p))
        return _pin_has(new, "hand-tag-differs", at), f"pin-row hand-tag-differs {at}"
    return {"direct": True}, expect


def _c_pin_hand_unused(d, config=None):
    """kw `direct` (5.5 T28.c4, G15): a pin_hand.txt copy (a State copy's hand_path; its hand too) + one pair, the
    (VA, name) of the first (by VA) passing func-route row (no hand/xref row uses it; columns copied from the first
    listed pair), rows re-run (State.replant) -> `hand-stale 0x<VA> ... unused`."""
    base = pin_rows.state(config)
    r0 = min((r for r in base.rows if not r["fails"] and r.get("cls") and r["cls"][1] == "func"
              and (r["va"], r["name"]) not in base.hand), key=lambda r: r["va"])
    at = f"0x{r0['va']:08X}"
    L = pin_rows.HAND.read_text(encoding="utf-8").splitlines()
    f = next(ln for ln in L if ln.strip() and not ln.startswith("#")).split("\t")
    f[0], f[1] = at, r0["name"]
    p = _pin_scratch(d, pin_rows.HAND, "\n".join(L + ["\t".join(f)]) + "\n")
    s = copy.copy(base)
    s.hand_path, s.hand = p, pin_rows.State.load_hand(p)
    print(f"control pin-hand-unused: pin_hand copy + a pair {at} {pin_rows.mask(r0['name'])} (a func-route row)")

    def expect(r_, v, g, i):
        new = _pin_new("pin-hand-unused", base, s.replant(pin_rows.PIN))
        return (any(x.split()[2:4] == ["hand-stale", at] and ": unused " in x for x in new),
                f"pin-row hand-stale {at} unused")
    return {"direct": True}, expect


def _c_pin_two_names_stale(d, config=None):
    """kw `direct` (5.5 T13, T28 leftover): pin_rows.TWO_NAMES_LISTED + one key no data row uses (the first listed
    VA, another name) while rows re-run (State.replant) -> `va-two-names:stale-exception` at that VA; the key removed
    after. Base fail lines are read before the key is added (row_fail_lines reads the dict)."""
    base = pin_rows.state(config)
    va = min(pin_rows.TWO_NAMES_LISTED)[0]
    key, at = (va, "_zz_gate_two_names_stale"), f"0x{va:08X}"
    print(f"control pin-two-names-stale: TWO_NAMES_LISTED + ({at}, {pin_rows.mask(key[1])}), no data row of that name")

    def expect(r_, v, g, i):
        norm = lambda x: re.sub(r" \S+\.txt:\d+:", ":", x)  # noqa: E731  (as _pin_new)
        before = {norm(x) for x in base.row_fail_lines()}
        pin_rows.TWO_NAMES_LISTED[key] = ("zz_gate", "gate control")
        try:
            lines = base.replant(pin_rows.PIN).row_fail_lines()
        finally:
            del pin_rows.TWO_NAMES_LISTED[key]
        new = [x for x in lines if norm(x) not in before]
        for x in new:
            print(f"control pin-two-names-stale: {x}")
        return _pin_has(new, "va-two-names:stale-exception", at), f"pin-row va-two-names:stale-exception {at}"
    return {"direct": True}, expect


def _c_cfg_pin_row(d, config=None):
    """4.5 T16.2.c4 (G15): through _core, a list copy with FLOUR_SYM's row renamed to PIN_UNRULED and a config copy
    (another key: pin_rows' State is per config) whose State is seeded from a pin copy with PIN_UNRULED's tag forged
    to a hand xref tag -> the size check names that config's pin-row failure (matched_set passes the gate's config
    to pin_rows.row_ok); the State is dropped by undo."""
    base, cp = pin_rows.state(config), _cfg_null(d, config, ())
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    k = next(j for j, ln in enumerate(L) if ln.split()[3:4] == [PIN_UNRULED])
    f = L[k].split()
    f[4] = _PP + "retail-xref"
    L[k] = " ".join(f)
    pin_rows._STATE[cp] = base.replant(_pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n"))
    print(f"control cfg-pin-row: config copy; its pin-row State from a pin copy, {PIN_UNRULED} tag forged")
    return {"list_path": _pin_rename(d, PIN_UNRULED), "config": cp,
            "undo": lambda: pin_rows._STATE.pop(cp, None)}, lambda r, v, g, i: (
        "pin-row check: hand-name-not-in-evidence" in v.get(PIN_UNRULED, {}).get("size", ""),
        f"{PIN_UNRULED} size (pin-row check: hand-name-not-in-evidence, the config copy's State)")


def _c_positive_keys(d):  # kw `direct` (4.5 T16.2.c5, G13): a positive-run glob with one global key dropped must fail
    units = sorted({e["unit"] for e in matched_set.load(FIX_LIST) if e["kind"] == "function"})
    full = {**dict.fromkeys(POSITIVE_GLOBALS, "pass"), **{f"target {u}": "pass" for u in units}}
    drop = {k: x for k, x in full.items() if k != "relink"}
    print(f"control positive-keys: _positive_ok(0, ...) on the full {len(full)} keys and with `relink` dropped")

    def expect(r, v, g, i):
        a, b = _positive_ok(0, full, FIX_LIST), _positive_ok(0, drop, FIX_LIST)
        print(f"control positive-keys: full -> {a}; relink dropped -> {b}")
        return a and not b, "positive run global keys (relink dropped)"
    return {"direct": True}, expect


def _c_last_tree(d):  # kw `direct` (4.5 T16.2.c5, G16): _write_last into scratch paths; a list edit shows in list_sha256
    lp = d / "matched_set.toml"
    shutil.copy2(FIX_LIST, lp)
    print(f"control last-tree: _write_last into {d.name}/a.json, list copy edited, then {d.name}/b.json")

    docs = {}

    def tree(name, warm=False):  # 5.5 T12.c2 (I73 F2): info as _core builds it, cold (a) / warm (b)
        p = d / name
        _write_last(p, lp, None, [], {"audit_not_checked": [], "warm_audit": warm, "config": None}, {})
        docs[name] = json.loads(p.read_text(encoding="utf-8"))
        return docs[name].get("tree", {})

    def expect(r, v, g, i):
        a = tree("a.json")
        with open(lp, "a", encoding="utf-8", newline="\n") as f:
            f.write("# gate control last-tree\n")
        b = tree("b.json", warm=True)
        want = {"head", "dirty", "list_sha256", "fp_sha256", "listed_dirty"}  # 5.5 T12.c2: + listed_dirty
        wa, wb = (docs[n].get("warm_audit", "absent") for n in ("a.json", "b.json"))
        print(f"control last-tree: keys a {sorted(a)}; b {sorted(b)}; warm_audit a {wa} b {wb}; config a "
              f"{docs['a.json'].get('config', 'absent')}")
        ok = (set(a) == want == set(b) and a["list_sha256"] == _sha(FIX_LIST, "sha256")
              and b["list_sha256"] == _sha(lp, "sha256") != a["list_sha256"]
              and a["fp_sha256"] == b["fp_sha256"] == _sha(REPO / verifier_lock.FP, "sha256")
              and isinstance(a["dirty"], int) and len(a["head"]) == 40 and isinstance(a["listed_dirty"], list)
              and wa is False and wb is True and "config" in docs["a.json"] and docs["a.json"]["config"] is None)
        return ok, "last.json tree (head, dirty, list_sha256 moved by the list edit, fp_sha256, listed_dirty), " \
                   "warm_audit False after cold / True after warm, config"
    return {"direct": True}, expect


def _c_lock_uncommitted(d):  # kw `direct` (5.5 T12.c2, G1 ruling 1a): a LISTED file edited and the fp rewritten, both
    # uncommitted (a LISTED file untracked too), in a scratch repo -> _tree lists them; progress.py refuses that tree
    fp = verifier_lock.FP.as_posix()
    repo = d / "repo"
    _env, git = _scratch_repo(repo, {"notes.txt": b"unlisted\n"})
    (repo / "tools").mkdir()
    (repo / "config").mkdir()
    (repo / "tools" / "gate.py").write_bytes(b"# committed\n")
    (repo / fp).write_bytes(b"0" * 64 + b"  tools/gate.py\n")
    git(["add", "tools/gate.py", fp])
    git(["commit", "-q", "--no-verify", "-m", "listed"])
    (repo / "tools" / "gate.py").write_bytes(b"# committed\n# edited, uncommitted\n")
    (repo / fp).write_bytes(b"1" * 64 + b"  tools/gate.py\n")
    (repo / "tools" / "xbe_write.py").write_bytes(b"# untracked LISTED\n")
    (repo / "notes.txt").write_bytes(b"unlisted, edited\n")
    want = sorted([fp, "tools/gate.py", "tools/xbe_write.py"])
    print(f"control lock-uncommitted: scratch repo, {want} changed vs HEAD (notes.txt too, unlisted); "
          f"progress.py --last on a last.json with that listed_dirty")

    def expect(r, v, g, i):
        t = G._tree(FIX_LIST, root=repo)
        clean = {**G._tree(LIST), "listed_dirty": []}  # this tree's list and fp hashes: only listed_dirty refuses
        out = {}
        for name, ld in (("dirty", t.get("listed_dirty")), ("clean", [])):
            p = d / f"last_{name}.json"
            p.write_text(json.dumps({"rows": [], "tree": {**clean, "listed_dirty": ld}}), encoding="utf-8")
            env = {**os.environ, "PYTHONPYCACHEPREFIX": sys.pycache_prefix}
            q = subprocess.run([sys.executable, str(TOOLS / "progress.py"), "--last", str(p)], cwd=REPO, env=env,
                               stdin=subprocess.DEVNULL, capture_output=True, text=True, errors="replace")
            out[name] = (q.returncode, "listed_dirty" in q.stdout + q.stderr)
        print(f"control lock-uncommitted: _tree listed_dirty {t.get('listed_dirty')}; progress (rc, names "
              f"listed_dirty): dirty {out['dirty']}, clean {out['clean']}")
        ok = t.get("listed_dirty") == want and out["dirty"] == (2, True) and not out["clean"][1]
        return ok, "tree listed_dirty (uncommitted LISTED files) + progress.py refusal"
    return {"direct": True, "undo": lambda: _rmtree_ro(repo)}, expect


def _c_last_stale_refused(d):  # 5.5 T12.c2 (G11): a green last.json, then a write_last run refused at the cut (an
    # empty delink dir) -> rc 2 and no last.json left (was: the previous green one stayed for progress.py to accept)
    last, orig = d / "last.json", G.LAST
    _write_last(last, FIX_LIST, None, [{"symbol": "s", "passed": True}], {"audit_not_checked": []}, {})
    (d / "nocut").mkdir()
    vars(G)["LAST"] = last  # set last in make: restored by the runner's undo (finally)
    print(f"control last-stale-refused: green {d.name}/last.json as gate.LAST; write_last run on an empty delink dir")
    return {"write_last": True, "tdir": d / "nocut", "undo": lambda: vars(G).__setitem__("LAST", orig)}, \
        lambda r, v, g, i: (r == 2 and g.get("cut", "").startswith("fail: ") and not last.exists(),
                            f"cut refusal (rc 2) leaves no last.json (exists: {last.exists()})")


def _c_lane_recs_shared(d, config=None):  # kw `direct` (5.5 T12.c2, G5/I71): pdb_types.load_tpi (recs reload) must
    # never run on a lane thread or while a lane is alive; the config's parse caches are emptied so the run rebuilds
    import threading
    import pdb_types
    ins, st = provenance_lint._INPUTS.pop(config, None), pin_rows._STATE.pop(config, None)
    lanes, seen, orig_load, orig_lane = [], [], pdb_types.load_tpi, G._Lane

    class Lane(orig_lane):
        def __init__(self, *a, **k):
            super().__init__(*a, **k)
            lanes.append(self.th)

    def load_tpi(*a, **k):
        seen.append((threading.current_thread() is threading.main_thread(), sum(t.is_alive() for t in lanes)))
        return orig_load(*a, **k)

    def undo():
        pdb_types.load_tpi, vars(G)["_Lane"] = orig_load, orig_lane
        for c, cache, x in ((config, provenance_lint._INPUTS, ins), (config, pin_rows._STATE, st)):
            if x is not None:
                cache[c] = x
    pdb_types.load_tpi, vars(G)["_Lane"] = load_tpi, Lane
    print("control lane-recs-shared: pdb_types.load_tpi recorded (main thread?, live lanes) over a fixture gate run "
          "with the provenance lane, parse caches emptied")

    def expect(r, v, g, i):
        rc, _v, gl, _i = _core(FIX_SRC, FIX_LIST, config=config, bdir=d / "build", run_relink=False, run_audit=False,
                               run_lint=True, lint_accepted=FIX_ACC, run_annotations=False, run_pins=True)
        bad = [s for s in seen if not s[0] or s[1]]
        print(f"control lane-recs-shared: gate rc {rc}; load_tpi calls {seen}; on a lane / under a live lane {bad}")
        return (bool(seen) and not bad and gl.get("provenance") == "pass" and gl.get("pin_rows") == "pass",
                "pdb_types.recs loaded on the main thread before the lanes (provenance, pin_rows pass)")
    return {"direct": True, "undo": undo}, expect


class GateControlDead(BaseException):  # 5.5 T12.c3 (G17): lane-dead-base's plant, outside Exception
    pass


def _c_lane_dead_base(d):  # 5.5 T12.c3 (G17): the audit lane's _tool raises a BaseException -> a dead lane, rc 1
    orig = _tool

    def tool(name, *a, **k):  # check_env (on the main thread) still runs the real _tool
        if name == "audit_public":
            raise GateControlDead("gate control lane-dead-base")
        return orig(name, *a, **k)
    vars(G)["_tool"] = tool  # set last in make: restored by the runner's undo (finally)
    print("control lane-dead-base: the audit lane's _tool raises GateControlDead (a BaseException)")
    return {"run_audit": True, "undo": lambda: vars(G).__setitem__("_tool", orig)}, lambda r, v, g, i: (
        r == 1 and g.get("audit_public") == "fail: audit_public lane error GateControlDead: gate control lane-dead-base",
        "audit_public (dead lane, GateControlDead)")


def _c_lane_no_result(d):  # 5.5 T12.c3 (G17): the audit lane's thread sets nothing (_Lane._run a no-op) -> no result
    orig = G._Lane._run
    G._Lane._run = lambda self, fn, a, kw: None
    print("control lane-no-result: gate._Lane._run replaced by a no-op (the lane sets no result, no exception)")
    return {"run_audit": True, "undo": lambda: setattr(G._Lane, "_run", orig)}, lambda r, v, g, i: (
        r == 1 and g.get("audit_public") == "fail: audit_public lane error: no result", "audit_public (lane no result)")


def _exit2(*a, **k):  # 5.5 T12.c3 (I73 F4): a vv_inputs-style refusal inside a lane
    raise SystemExit(2)


def _c_lane_sysexit_prov(d):  # 5.5 T12.c3 (I73 F4): SystemExit(2) in the provenance lane -> its _LaneDead verdict
    kw, expect = _layer_patched(provenance_lint, "lint", "provenance", "lane-sysexit-prov", _exit2,
                                "fail: provenance lane error SystemExit: 2", 1)
    return {**kw, "run_lint": True}, lambda r, v, g, i: (
        expect(r, v, g, i)[0] and g["provenance"] == "fail: provenance lane error SystemExit: 2",
        "provenance (lane error SystemExit: 2, rc 1)")


def _c_lane_sysexit_annot(d):  # 5.5 T12.c3 (I73 F4): SystemExit(2) in the annotations lane -> its _LaneDead verdict
    kw, expect = _layer_patched(annotations, "check", "annotations", "lane-sysexit-annot", _exit2,
                                "fail: annotations lane error SystemExit: 2", 1)
    return {**kw, "run_annotations": True}, lambda r, v, g, i: (
        expect(r, v, g, i)[0] and g["annotations"] == "fail: annotations lane error SystemExit: 2",
        "annotations (lane error SystemExit: 2, rc 1)")


LANE_KEYS = ("audit_public", "provenance", "annotations")  # 5.5 T12.c3 (I73 F1): the lane layers' globals


def _printed(out, key):  # 5.5 T12.c3: the gate printed the `PASS|FAIL <key>: ` global line
    return any(ln.startswith((f"PASS {key}: ", f"FAIL {key}: ")) for ln in out.splitlines())


def _c_objdiff_malformed(d, config=None):  # 5.5 T12.c3 (G17): objdiff_cli -> a .cmd writing a non-JSON report, rc 0:
    # the report parse raises between the build and the relink layer; the started lanes are still consumed and printed
    p = d / "zz_objdiff.cmd"
    p.write_text("@echo gate control: not json> %6\r\n@exit /b 0\r\n", encoding="ascii", newline="")
    print("control objdiff-malformed: config copy, objdiff_cli -> zz_objdiff.cmd (rc 0, report.json not JSON); "
          "the annotations lane runs")
    cfg = _cfg_copy(d, config, "objdiff_cli", p)

    def expect(r, v, g, i):
        w = io.StringIO()
        with contextlib.redirect_stdout(w):
            try:
                rc, _v, gl, _i = _core(FIX_SRC, FIX_LIST, config=cfg, bdir=d / "build", run_relink=False,
                                       run_audit=False, run_lint=False, run_annotations=True, run_pins=False)
            except Exception as e:  # the defect: the gate raised, the lane's verdict was never consumed
                rc, gl = f"raised {type(e).__name__}", {}
        out = w.getvalue()
        sys.stdout.write(out)
        print(f"control objdiff-malformed: gate rc {rc}; gate error {gl.get('gate error')!r}; annotations "
              f"{gl.get('annotations')!r} printed {_printed(out, 'annotations')}")
        return (rc == 1 and gl.get("gate error", "").startswith("fail: gate error JSONDecodeError")
                and "annotations" in gl and _printed(out, "annotations") and _printed(out, "gate error"),
                "gate error (JSONDecodeError, rc 1) + the annotations lane consumed and printed")
    return {"direct": True}, expect


def _c_warm_audit_selftest(d):  # kw `direct` (5.5 T12.c3, G17): --warm-audit with --self-test / --ctl-worker refused
    print("control warm-audit-selftest: main(--self-test --warm-audit), main(--ctl-worker <none> --warm-audit)")

    def expect(r, v, g, i):
        got = {}
        for argv in (["--self-test", "--warm-audit"], ["--ctl-worker", str(d / "no_spec.json"), "--warm-audit"]):
            w = io.StringIO()
            with contextlib.redirect_stdout(w):
                rc = main(argv)
            got[argv[0]] = (rc, "gate: REFUSED --warm-audit: real run only" in w.getvalue())
        print(f"control warm-audit-selftest: (rc, refusal printed) {got}")
        return all(x == (2, True) for x in got.values()), "--warm-audit refused (rc 2) with --self-test, --ctl-worker"
    return {"direct": True}, expect


def _c_positive_extra_key(d):  # kw `direct` (5.5 T12.c3, G17): a positive-run glob with one unexpected key must fail
    units = sorted({e["unit"] for e in matched_set.load(FIX_LIST) if e["kind"] == "function"})
    full = {**dict.fromkeys(POSITIVE_GLOBALS, "pass"), **{f"target {u}": "pass" for u in units}}
    extra = {**full, "zz gate control": "pass"}
    print(f"control positive-extra-key: _positive_ok(0, ...) on the full {len(full)} keys and with `zz gate control` "
          f"added")

    def expect(r, v, g, i):
        w = io.StringIO()
        with contextlib.redirect_stdout(w):
            a, b = _positive_ok(0, full, FIX_LIST), _positive_ok(0, extra, FIX_LIST)
        out = w.getvalue()
        sys.stdout.write(out)
        print(f"control positive-extra-key: full -> {a}; extra key -> {b}")
        return (a and not b and "unexpected ['zz gate control']" in out,
                "positive run global keys (unexpected `zz gate control`)")
    return {"direct": True}, expect


def _c_marker_objdiff(d):  # 5.5 T12.c3 (G17, I36): the marker plant: a function missing from the base fails objdiff
    kw, _expect = _c_marker(d)
    return kw, lambda r, v, g, i: (
        v[LOC_SYM]["build"] == "pass" and v[LOC_SYM]["objdiff"] == "fail: fuzzy 0.0",
        f"{LOC_SYM} objdiff (not in the base: fuzzy 0.0)")


def _c_cut_unpinned(d):  # 4.5 T16.2.c5 (G17): hashes.txt copy gains a line, cut.complete re-sealed -> the pin refuses
    t = _target_copy(d)
    h = t / "hashes.txt"
    new = h.read_bytes() + f"{'0' * 64}  gate_control_unpinned.obj\n".encode()
    h.write_bytes(new)
    (t / byte_compare.CUT_MARKER).write_bytes(
        f"cut-complete v1 hashes-sha256 {hashlib.sha256(new).hexdigest()} objects {len(new.splitlines())}\n".encode())
    if byte_compare.cut_check(t):
        raise RuntimeError(f"re-sealed cut does not pass: {byte_compare.cut_check(t)}")
    print("control cut-unpinned: hashes.txt copy, one line appended, cut.complete re-sealed (default pin)")
    return {"tdir": t}, lambda r, v, g, i: (
        r == 2 and g.get("cut") == "fail: cut: hashes.txt sha256 != config/delink_cut.sha256", "cut (unpinned)")


LOCK_CHILD = """import msvcrt, sys, time
fh = open(sys.argv[1], "r+b")
fh.seek(int(sys.argv[2]))
msvcrt.locking(fh.fileno(), msvcrt.LK_NBLCK, 1)
print("held", flush=True)
time.sleep(600)
"""


def _c_gate_lock(d):  # kw `direct` (5.1 T2, O21): a held run lock refuses main rc 2 before run(); none stale; no self-refusal
    lock, hold = d / "run.lock", d / "hold.py"
    lock.write_bytes(b"")
    hold.write_text(LOCK_CHILD, encoding="utf-8", newline="\n")
    g, names = vars(G), ("RUN_LOCK", "run", "_positive_ok")  # 5.4 T5: gate main reads these from gate's globals
    gc = globals()  # `controls` lives here now; main calls gate_controls.controls (was: all four in globals())
    saved = {k: g[k] for k in names if k in g}
    saved_controls = gc["controls"]
    child, calls = [], []
    print(f"control gate-lock: child holds byte {RUN_LOCK_AT} of {d.name}/run.lock; main([]) with run stubbed")

    def undo():
        for p in child:
            if p.poll() is None:
                p.kill()
            p.wait()
        for k in names:
            if k in saved:
                g[k] = saved[k]
            else:
                g.pop(k, None)
        gc["controls"] = saved_controls

    def main_out(argv):
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf):
            rc = main(argv)
        return rc, buf.getvalue()

    def expect(r, v, gl, i):
        g["RUN_LOCK"] = lock
        g["run"] = lambda *a, **k: (calls.append("run"), (0, {}, {}, {}))[1]
        child.append(subprocess.Popen([sys.executable, str(hold), str(lock), str(RUN_LOCK_AT)],
                                      stdout=subprocess.PIPE, text=True))
        held = child[0].stdout.readline().strip()
        rc1, out1 = main_out([])
        n1 = calls.count("run")
        child[0].kill()
        child[0].wait()
        rc2, out2 = main_out([])
        n2 = calls.count("run") - n1
        # 5.4 T5.c2: main hands controls() the positive run; was: the stub ignored it (main ran it first)
        gc["controls"] = lambda gate, config=None, positive=None: (calls.append("controls"), positive(), (1, 1))[2]
        g["_positive_ok"] = lambda *a, **k: True
        rc3, out3 = main_out(["--self-test"])
        n3, nc = calls.count("run") - n1 - n2, calls.count("controls")
        print(f"control gate-lock: child {held!r}; held: rc {rc1} run calls {n1} out {out1.strip()!r}; "
              f"killed: rc {rc2} run calls {n2}; --self-test: rc {rc3} run calls {n3} controls calls {nc}")
        ok = (held == "held" and rc1 == 2 and "another gate is running" in out1 and n1 == 0
              and rc2 == 0 and n2 == 1 and rc3 == 0 and n3 == 1 and nc == 1)
        return ok, "main run lock (held -> rc 2, run not called; released; self-test not self-refused)"
    return {"direct": True, "undo": undo}, expect


def _c_ms_extra_emitter(d):  # 5.1 T2 (D124): an [[emitter]] row naming an unmarked hand-written body fails extra
    s = _src_copy(d)
    with open(s / LOC_SRC, "a", encoding="utf-8", newline="\n") as f:
        f.write('\nextern "C" int gate_control_emit(int x)\n{\n    return x + 2;\n}\n')
    lp = d / "matched_set.toml"
    shutil.copy2(FIX_LIST, lp)
    matched_set.add_emitter(lp, "_gate_control_emit", LOC_UNIT, "gate control ms-extra-emitter: a hand-written body")
    print(f"control ms-extra-emitter: unmarked gate_control_emit body appended to {LOC_SRC.as_posix()}, "
          f"[[emitter]] _gate_control_emit row in a list copy")
    return {"src_root": s, "list_path": lp}, lambda r, v, g, i: (
        "[[emitter]] _gate_control_emit" in g.get("*objects* extra", "")
        and "hand-written body" in g.get("*objects* extra", ""), "*objects* extra (emitter _gate_control_emit body)")


def _ms_emitter(d, name, text, rows, header=None):
    """5.5 T12.c4 (G6, I32): text appended to LOC_SRC in a src copy (header: (file name, body) written beside it and
    included), rows [(symbol, cause)] added as [[emitter]] rows of LOC_UNIT to a list copy; expect: every row's
    `[[emitter]] <symbol> (LocString.obj) <cause>` in *objects* extra and no planted symbol among the unlisted ones."""
    s = _src_copy(d)
    if header:
        (s / LOC_SRC).parent.joinpath(header[0]).write_text(header[1], encoding="utf-8", newline="\n")
    with open(s / LOC_SRC, "a", encoding="utf-8", newline="\n") as f:
        f.write(text)
    lp = d / "matched_set.toml"
    shutil.copy2(FIX_LIST, lp)
    for sym, _c in rows:
        matched_set.add_emitter(lp, sym, LOC_UNIT, f"gate control {name}")
    print(f"control {name}: {len(rows)} [[emitter]] row(s) in a list copy, plant appended to {LOC_SRC.as_posix()}"
          + (f", header {header[0]} beside it" if header else ""))

    def expect(r, v, g, i):
        ex = g.get("*objects* extra", "")
        hits = [f"[[emitter]] {sym} (LocString.obj) {c}" in ex for sym, c in rows]
        loose = [sym for sym, _c in rows if f"LocString.obj:{sym}" in ex.split("; [[emitter]]")[0]]
        print(f"control {name}: causes found {hits}; planted symbols unlisted {loose}")
        return all(hits) and not loose, f"*objects* extra ({', '.join(c.split(':')[0] for _s, c in rows)})"
    return {"src_root": s, "list_path": lp}, expect


def _c_ms_emitter_header_body(d):  # 5.5 T12.c4 (G6): an unmarked body in a quoted header (was: unit source only)
    return _ms_emitter(d, "ms-emitter-header-body", '\n#include "gate_control_emit.h"\n',
                       [("_gate_control_emit", "has a hand-written body at Libs/LocString/gate_control_emit.h:2")],
                       ("gate_control_emit.h", 'extern "C" int gate_control_emit(int x);\n'
                        'extern "C" int gate_control_emit(int x)\n{\n    return x + 2;\n}\n'))


def _c_ms_emitter_wrapped(d):  # 5.5 T12.c4 (G6): bodies at brace depth 1 (was: skipped, pass)
    return _ms_emitter(d, "ms-emitter-wrapped",
                       '\nextern "C" {\nint gate_control_emit(int x)\n{\n    return x + 2;\n}\n}\n'
                       '\nnamespace gcn {\nint gate_control_emit_ns(int x)\n{\n    return x + 3;\n}\n}\n',
                       [("_gate_control_emit", "has a hand-written body at Libs/LocString/LocString.cpp:"),
                        ("?gate_control_emit_ns@gcn@@YAHH@Z", "has no marked scaffold definition")])


def _c_ms_emitter_nospelling(d):  # 5.5 T12.c4 (G6): a `??` name with no hand spelling (was: None, pass)
    return _ms_emitter(d, "ms-emitter-nospelling",
                       "\nstruct GateCtl {\n    int v;\n    int operator+(int x);\n};\n"
                       "int GateCtl::operator+(int x)\n{\n    return v + x;\n}\n",
                       [("??HGateCtl@@QAEHH@Z", "has no hand spelling")])


def _c_ms_emitter_undefined(d):  # 5.5 T12.c4 (G6): a marked scaffold in the source, not in the obj (was: pass)
    return _ms_emitter(d, "ms-emitter-undefined",
                       "\n#if 0\n// !FAKE: gate control scaffold, compiled out: the obj does not define it\n"
                       "void pin_emit_GateGone(int* p)\n{\n    *p = 0;\n}\n#endif\n",
                       [("?pin_emit_GateGone@@YAXPAH@Z", "is stale: not defined as code in LocString.obj")])


def _c_positive_planted(d):  # kw `direct` (5.5 T12.c4, R530/R543): name_sites --control positive-planted (5.5 T5 S6)
    # plants a wrong __real@ name at an agreeing __real@ VA in memory; it must exit 1 with the plant DISAGREE (caught).
    # A subprocess: name_sites is not LISTED (an import here fails lock-imports).
    print("control positive-planted: name_sites.py --control positive-planted (a wrong __real@ name, in memory)")

    def expect(r, v, g, i):
        p = subprocess.run([sys.executable, str(TOOLS / "name_sites.py"), "--control", "positive-planted"],
                           cwd=REPO, stdin=subprocess.DEVNULL, capture_output=True, text=True, errors="replace")
        caught = any(ln.strip().startswith("planted ") and ln.rstrip().endswith(": DISAGREE (caught)")
                     for ln in p.stdout.splitlines())
        print(f"control positive-planted: name_sites rc {p.returncode}; plant "
              f"{'DISAGREE (caught)' if caught else 'NOT caught'}")
        return p.returncode == 1 and caught, "name_sites positive-planted (the planted __real@ name DISAGREE)"
    return {"direct": True}, expect


SET_SRC = Path("Apps") / "Vince" / "scripts" / "Sewers" / "JaniceCutscenes.cpp"


def _c_ms_parallel(d):  # 5.1 T2 (O18): parallel preprocess verdicts land on their own rows (by key, not completion)
    s = _src_copy(d)
    head = "void VinceDropsInCutscene::SetState(int state)\n{\n"
    _edit_text(s / SET_SRC, head, head + "    __asm { nop }\n")
    print(f"control ms-parallel: __asm {{ nop }} inserted in {SET_SYM} only; matched_set.WORKERS "
          f"{getattr(matched_set, 'WORKERS', None)}")

    def expect(r, v, g, i):
        others = sorted(k for k in v if k != SET_SYM)
        bad = [k for k in others if v[k].get("asm") != "pass"]
        print(f"control ms-parallel: {SET_SYM} asm {v.get(SET_SYM, {}).get('asm')!r}; others {len(others)}, "
              f"asm not pass {bad}")
        ok = (v[SET_SYM]["build"] == "pass" and v[SET_SYM]["asm"].startswith("fail: __asm/_asm in preprocessed")
              and others and not bad and getattr(matched_set, "WORKERS", 1) > 1)
        return ok, f"{SET_SYM} asm only (build pass, WORKERS > 1)"
    return {"src_root": s}, expect


def _c_selftest_passes(d):  # 5.4 T5.c2: worker-only maker (never in CONTROLS) for selftest-parallel-pass: does not fail
    print("control selftest-passes: a direct control whose expect reports no failure")
    return {"direct": True}, lambda r, v, g, i: (False, "nothing (this control passes)")


PAR_SUBSET = ("annotations-malformed", "selftest-passes")  # one real cheap control + one that passes


def _c_selftest_parallel_pass(d):  # kw `direct` (5.4 T5.c2, D26): main --self-test through the parallel runner,
    # 2 workers, on PAR_SUBSET (run, lock and controls patched as gate-lock): J < K, rc != 0, the pass named
    g, names = vars(G), ("RUN_LOCK", "run", "_positive_ok")
    gc = globals()
    saved = {k: g[k] for k in names if k in g}
    saved_controls = gc["controls"]
    got = []
    print(f"control selftest-parallel-pass: main --self-test, run stubbed, controls = the runner on {list(PAR_SUBSET)} "
          f"in 2 workers")

    def undo():
        for k in names:
            if k in saved:
                g[k] = saved[k]
            else:
                g.pop(k, None)
        gc["controls"] = saved_controls

    def expect(r, v, gl, i):
        g["RUN_LOCK"] = d / "run.lock"
        g["run"] = lambda *a, **k: (0, {}, {}, {})
        g["_positive_ok"] = lambda *a, **k: True
        gc["controls"] = lambda gate, config=None, positive=None: (
            got.append(_parallel(config, PAR_SUBSET, 2, d / "par", positive)), got[-1])[1]
        w = io.TextIOWrapper(io.BytesIO(), encoding="utf-8", errors="replace", write_through=True)
        with contextlib.redirect_stdout(w):
            rc = main(["--self-test"])
        out = w.buffer.getvalue().decode("utf-8", "replace")
        named = "control selftest-passes: DID NOT FAIL as expected" in out
        real = "control annotations-malformed: failed as expected" in out
        print(f"control selftest-parallel-pass: runner (J, K) {got}; main --self-test rc {rc}; selftest-passes "
              f"{'named' if named else 'NOT named'} DID NOT FAIL; annotations-malformed "
              f"{'failed' if real else 'DID NOT FAIL'} as expected")
        ok = got == [(1, 2)] and rc not in (0, None) and named and real
        return ok, "parallel runner (a passing control: J < K, self-test rc != 0)"
    return {"direct": True, "undo": undo}, expect


CONTROLS =(("objdiff", _c_objdiff), ("bytes-alone", _c_bytes), ("target", _c_target), ("cut-marker", _c_cut_marker),
            ("body_sha1", _c_body),
            ("xbe_sha1", _c_xbe), ("empty-list", _c_empty), ("missing-list", _c_missing), ("extra", _c_extra),
            ("marker", _c_marker), ("asm", _c_asm), ("audit", _c_audit), ("clean-build", _c_clean), ("d26", _c_d26),
            ("lock-edit", _c_lock_edit), ("lock-missing", _c_lock_missing), ("lock-fp-missing", _c_lock_fp_missing),
            ("lock-fp-empty", _c_lock_fp_empty), ("lock-unlisted", _c_lock_unlisted), ("lock-crlf", _c_lock_crlf),
            ("lock-pyc", _c_lock_pyc), ("lock-symlink", _c_lock_symlink), ("symtab-pin", _c_symtab_pin),
            ("lock-imports", _c_lock_imports), ("lock-shadow", _c_lock_shadow),
            ("relink", _c_relink), ("relink-entry", _c_relink_entry), ("relink-literal", _c_relink_literal),
            ("relink-fold", _c_relink_fold), ("relink-samename", _c_relink_samename), ("relink-weak", _c_relink_weak),
            ("prov-hedge", _c_prov_hedge), ("prov-proto-pdb", _c_prov_proto), ("prov-name-retail", _c_prov_name),
            ("prov-placeholder", _c_prov_place), ("prov-src", _c_prov_src), ("prov-fake-diff", _c_prov_fake),
            ("prov-locals", _c_prov_locals), ("prov-d66", _c_prov_d66), ("prov-accepted", _c_prov_accepted),
            ("prov-forceinline", _c_prov_forceinline),
            ("prov-pragma", _c_prov_pragma), ("prov-noinline", _c_prov_noinline),  # 4.5 T16.2.c2
            ("prov-include-body", _c_prov_include_body),
            ("pin-unsized", _c_pin_unsized), ("pin-site-corr", _c_pin_site), ("pin-alias-dup", _c_pin_alias_dup),
            ("data-impostor", _c_data_impostor), ("xbe-wrong", _c_xbe_wrong), ("objdiff-cli", _c_objdiff_cli),
            ("build-refuse", _c_build_refuse), ("target-missing", _c_target_missing),
            ("target-unhashed", _c_target_unhashed), ("bytes-refuse", _c_bytes_refuse),
            ("bytes-two-objs", _c_bytes_two_objs), ("toolchain-split", _c_toolchain_split),
            ("audit-env", _c_audit_env), ("audit-null-key", _c_audit_null_key), ("audit-tail", _c_audit_tail),
            ("audit-line-pin", _c_audit_line_pin), ("audit-literal-pin", _c_audit_literal_pin),
            ("audit-short-blob", _c_audit_short_blob), ("audit-gzip-disasm", _c_audit_gzip_disasm),
            ("objdiff-pin", _c_objdiff_pin), ("audit-marker-contradiction", _c_audit_marker_contradiction),
            ("audit-renamed-blob", _c_audit_renamed_blob), ("audit-shallow", _c_audit_shallow),  # 4.5 T16.2
            ("audit-paths-empty", _c_audit_paths_empty), ("audit-route-pin", _c_audit_route_pin),
            ("relink-bare-exit", _c_relink_bare_exit), ("relink-exit-zero", _c_relink_exit_zero),
            ("relink-not-built", _c_relink_not_built), ("relink-no-funcs", _c_relink_no_funcs),
            ("relink-error", _c_relink_error), ("relink-rc", _c_relink_rc),
            ("prov-locals-cache", _c_prov_locals_cache), ("prov-inputs-pin", _c_prov_inputs_pin),
            ("prov-fake-head", _c_prov_fake_head), ("prov-refused", _c_prov_refused), ("prov-error", _c_prov_error),
            ("prov-fake-reword", _c_prov_fake_reword), ("prov-fake-trailing", _c_prov_fake_trailing),
            ("prov-fake-comment-excuse", _c_prov_fake_comment_excuse), ("prov-name-ns", _c_prov_name_ns),
            ("prov-d66-pdb-volatile", _c_prov_d66_pdb_volatile), ("annotations-folded", _c_annotations_folded),
            ("annotations-malformed", _c_annotations_malformed), ("annotations-no-name", _c_annotations_no_name),
            ("annotations-no-source", _c_annotations_no_source), ("annotations-count", _c_annotations_count),
            ("annotations-order", _c_annotations_order), ("annotations-row", _c_annotations_row),
            *((f"pin-plant-{p[0]}", _pin_plant(*p)) for p in PIN_PLANTS),  # 4.2 T5: 30 pin-row plants (was 28; 4.5 T16.1 +near, +V6)
            ("pin-literal-list", _c_pin_literal_list), ("pin-unruled", _c_pin_unruled),
            ("toolchain-hash", _c_toolchain_hash), ("size-list", _c_size_list),  # 4.5 T16.2.c3 (G14)
            ("annotations-refused", _c_annotations_refused), ("annotations-error", _c_annotations_error),
            ("pin-refused", _c_pin_refused), ("pin-error", _c_pin_error), ("pin-fold-site", _c_pin_fold_site),
            ("pin-i10-static", _c_pin_i10_static), ("pin-vtable-prefix", _c_pin_vtable_prefix),  # 4.5 T16.2.c4
            ("pin-accepted-unused", _c_pin_accepted_unused), ("cfg-pin-row", _c_cfg_pin_row),
            ("positive-keys", _c_positive_keys), ("last-tree", _c_last_tree),  # 4.5 T16.2.c5
            ("cut-unpinned", _c_cut_unpinned),
            ("objdiff-noexec", _c_objdiff_noexec), ("prov-include-dotdot", _c_prov_include_dotdot),  # 5.1 T1
            ("prov-pragma-if0", _c_prov_pragma_if0),
            ("gate-lock", _c_gate_lock), ("ms-extra-emitter", _c_ms_extra_emitter),  # 5.1 T2
            ("ms-parallel", _c_ms_parallel),
            ("pin-accepted-other-conflict", _c_pin_accepted_other_conflict),  # 5.1 T3
            ("pin-near-listed", _c_pin_near_listed),  # 5.1 T3.1
            ("relink-samevt", _c_relink_samevt), ("relink-xdata-order", _c_relink_xdata_order),  # 5.1 T4
            ("build-objname-path", _c_build_objname_path),  # 5.1 T4.c2
            ("relink-data-order", _c_relink_data_order),  # 5.1 T13
            ("relink-label-unref", _c_relink_label_unref), ("relink-unit-key", _c_relink_unit_key),  # 5.1 T14
            ("prov-nested-op", _c_prov_nested_op), ("relink-site-past-buffer", _c_relink_site_past_buffer),  # 5.3 T2
            ("pin-slotnames-unnamed", _c_pin_slotnames_unnamed),  # 5.3 T2.c4
            ("pin-slotnames-site-corr", _c_pin_slotnames_site_corr), ("pin-slotnames-count", _c_pin_slotnames_count),
            ("pin-slotnames-twin", _c_pin_slotnames_twin),  # 5.5 T4.c3 (M30 S2)
            ("pin-bracket-outside", _c_pin_bracket_outside), ("pin-bracket-ambiguous", _c_pin_bracket_ambiguous),
            ("pin-bracket-own-dtor", _c_pin_bracket_own_dtor),  # 5.5 T4.c2
            ("pin-registerme-literal", _c_pin_registerme_literal),  # 5.4 T23 (R418 rule a)
            ("pin-registerme-count", _c_pin_registerme_count),  # 5.5 T4.c2 (I76)
            ("pin-registerme-not-class", _c_pin_registerme_not_class),
            ("pin-hand-moved", _c_pin_hand_moved), ("pin-hand-refused-name", _c_pin_hand_refused_name),  # 5.5 T4
            ("pin-twin-wrong", _c_pin_twin_wrong),
            ("matched-twin-rowok", _c_matched_twin_rowok),  # 5.5 T27 (D137)
            ("relink-alias", _c_relink_alias),  # 5.4 T3 (G8)
            ("selftest-parallel-pass", _c_selftest_parallel_pass),  # 5.4 T5.c2
            ("lane-audit-missing", _c_lane_audit_missing), ("lane-raise", _c_lane_raise),  # 5.4 T5.c3
            ("build-pool", _c_build_pool),  # 5.4 T5.c4
            ("audit-cache-forged-cold", _c_audit_cache_forged_cold), ("audit-cache-warm-reads", _c_audit_cache_warm_reads),
            ("audit-cache-key-accepted", _c_audit_cache_key_accepted),
            ("audit-cache-key-fp", _c_audit_cache_key_fp),  # 5.4 T5.c5
            ("relink-tag-sentinel", _c_relink_tag_sentinel), ("relink-site-opcode", _c_relink_site_opcode),  # 5.5 T12
            ("relink-names-unpinned", _c_relink_names_unpinned),
            ("lock-uncommitted", _c_lock_uncommitted), ("last-stale-refused", _c_last_stale_refused),  # 5.5 T12.c2
            ("lane-recs-shared", _c_lane_recs_shared),
            ("lane-dead-base", _c_lane_dead_base), ("lane-no-result", _c_lane_no_result),  # 5.5 T12.c3
            ("lane-sysexit-prov", _c_lane_sysexit_prov), ("lane-sysexit-annot", _c_lane_sysexit_annot),
            ("objdiff-malformed", _c_objdiff_malformed), ("warm-audit-selftest", _c_warm_audit_selftest),
            ("positive-extra-key", _c_positive_extra_key),
            ("marker-objdiff", _c_marker_objdiff), ("audit-cache-newer-commit", _c_audit_cache_newer_commit),
            ("ms-emitter-header-body", _c_ms_emitter_header_body), ("ms-emitter-wrapped", _c_ms_emitter_wrapped),
            ("ms-emitter-nospelling", _c_ms_emitter_nospelling),
            ("ms-emitter-undefined", _c_ms_emitter_undefined),  # 5.5 T12.c4 (G6, I32)
            ("positive-planted", _c_positive_planted),  # 5.5 T12.c4 (R530/R543)
            ("pin-twin-dispatch", _c_pin_twin_dispatch),  # 5.5 T28 (G3)
            ("pin-slotnames-dispatch", _c_pin_slotnames_dispatch),
            ("pin-bracket-anchor-unit", _c_pin_bracket_anchor_unit),  # 5.5 T28 (G9)
            ("pin-bracket-outside-unit", _c_pin_bracket_outside_unit),
            ("pin-bracket-callee-down", _c_pin_bracket_callee_down),
            ("pin-site-nopublic", _c_pin_site_nopublic),  # 5.5 T28 (G13)
            ("pin-bracket-dtor-untabled", _c_pin_bracket_dtor_untabled),  # 5.5 T28 (G14)
            ("pin-twin-no-registerme-site", _c_pin_twin_no_site),  # 5.5 T28 (G17)
            ("pin-twin-class", _c_pin_twin_class),
            ("pin-near-retag", _c_pin_near_retag),  # 5.5 T28.c2 (G2)
            ("pin-near-abs-site", _c_pin_near_abs_site),
            ("pin-near-fold", _c_pin_near_fold),
            ("pin-bracket-unit-range", _c_pin_bracket_unit_range),  # 5.5 T28.c4 (G7)
            ("pin-hand-retag", _c_pin_hand_retag), ("pin-hand-unused", _c_pin_hand_unused),  # 5.5 T28.c4 (G15)
            ("pin-slotnames-own-base", _c_pin_slotnames_own_base),  # 5.5 T28.c5 (G8)
            ("pin-data-two-names", _c_pin_data_two_names),  # 5.5 T28.c5 (G12)
            ("prov-baseline-stale", _c_prov_baseline_stale), ("prov-locals-neutral", _c_prov_locals_neutral),  # 5.5 T13
            ("prov-locals-noproc", _c_prov_locals_noproc), ("prov-scratch-shared", _c_prov_scratch_shared),
            ("prov-attr", _c_prov_attr), ("prov-pragma-scope", _c_prov_pragma_scope),
            ("prov-fake-fields", _c_prov_fake_fields), ("prov-fake-percommit", _c_prov_fake_percommit),
            ("pin-two-names-stale", _c_pin_two_names_stale),  # 5.5 T13 (T28 leftover)
            ("prov-member-invented", _c_prov_member_invented),  # 5.5 T13.c3
            ("prov-untagged-helper", _c_prov_untagged_helper),
            ("audit-utf16", _c_audit_utf16), ("audit-archive", _c_audit_archive),  # 5.5 T14.c1
            ("hook-prepush-remote", _c_hook_prepush_remote), ("hook-prepush-fakediff", _c_hook_prepush_fakediff),
            ("hook-commitmsg-audit", _c_hook_commitmsg_audit), ("hook-classes-off", _c_hook_classes_off),
            ("hook-python-lookup", _c_hook_python_lookup))  # 5.5 T14.c2
CFG_CONTROLS = (_c_data_impostor, _c_xbe_wrong, _c_objdiff_cli, _c_objdiff_noexec,  # 5.1 T1: +objdiff-noexec
                _c_toolchain_split, _c_prov_locals_cache, _c_prov_inputs_pin,  # 3.5 T4: make(d, config)
                _c_audit_null_key, _c_audit_tail, _c_prov_d66_pdb_volatile,
                _c_pin_literal_list, _c_pin_unruled, _c_objdiff_pin, _c_audit_marker_contradiction,  # 4.5 T16.1
                _c_toolchain_hash,  # 4.5 T16.2.c3
                _c_pin_accepted_other_conflict, _c_pin_near_listed,  # 5.1 T3, T3.1
                _c_build_objname_path, _c_build_pool,  # 5.1 T4.c2; 5.4 T5.c4: +build-pool
                _c_pin_slotnames_unnamed, _c_pin_slotnames_site_corr, _c_pin_slotnames_count,  # 5.3 T2.c4
                _c_pin_slotnames_twin,  # 5.5 T4.c3
                _c_pin_bracket_outside, _c_pin_bracket_ambiguous, _c_pin_bracket_own_dtor,  # 5.5 T4.c2: +own-dtor
                _c_pin_registerme_count, _c_pin_registerme_not_class,  # 5.5 T4.c2 (I76)
                _c_pin_hand_moved, _c_pin_hand_refused_name, _c_pin_twin_wrong,  # 5.5 T4
                _c_matched_twin_rowok,  # 5.5 T27
                _c_relink_tag_sentinel,  # 5.5 T12
                _c_lane_recs_shared,  # 5.5 T12.c2 (G5)
                _c_build_refuse, _c_objdiff_malformed,  # 5.5 T12.c3 (I73 F1, G17)
                _c_pin_twin_dispatch, _c_pin_slotnames_dispatch, _c_pin_bracket_anchor_unit,  # 5.5 T28
                _c_pin_bracket_outside_unit, _c_pin_bracket_callee_down, _c_pin_site_nopublic,
                _c_pin_bracket_dtor_untabled, _c_pin_twin_no_site, _c_pin_twin_class,
                _c_pin_near_retag, _c_pin_near_abs_site, _c_pin_near_fold,  # 5.5 T28.c2 (G2)
                _c_pin_bracket_unit_range, _c_pin_hand_retag, _c_pin_hand_unused,  # 5.5 T28.c4 (G7, G15)
                _c_pin_slotnames_own_base, _c_pin_data_two_names,  # 5.5 T28.c5 (G8, G12)
                _c_prov_scratch_shared, _c_pin_two_names_stale,  # 5.5 T13
                _c_audit_utf16,  # 5.5 T14.c1
                _c_hook_prepush_remote, _c_hook_prepush_fakediff, _c_hook_commitmsg_audit,  # 5.5 T14.c2
                _c_hook_classes_off, _c_hook_python_lookup,
                _c_clean, _c_pin_i10_static, _c_pin_vtable_prefix, _c_pin_accepted_unused, _c_cfg_pin_row,  # T16.2.c4
                *(m for n, m in CONTROLS if n.startswith("pin-plant-")))
LINE_GLOBALS = {"extra": "*objects* extra", "audit": "audit_public", "empty-list": "list",
                "relink": "relink", "relink-entry": "relink",
                "relink-literal": "relink", "relink-fold": "relink", "audit-null-key": "audit_public", "audit-tail": "audit_public",
                "audit-line-pin": "audit_public", "audit-literal-pin": "audit_public",
                "annotations-row": "annotations", "pin-literal-list": "pin_rows",
                "lane-audit-missing": "audit_public"}  # control -> global on the count line


# 5.4 T5.c2: the controls run in N worker processes (gate.py --ctl-worker SPEC), N = clamp((free - RESERVE_MB) //
# PER_WORKER_MB, 1, CAP) at start; was: 1 (serial, in the gate process). Evidence: phase-ends/current/logs/T5.c2.md.
RESERVE_MB = 6144  # free memory left to the parent's positive run and the machine
PER_WORKER_MB = 384  # one worker over all 161 controls: peak working set 243 MB, free-memory drop with its cl/link/
# git children 346 MB (129.6 s), rounded up
CAP = 4  # was: 1 (serial, 179 s). self-test wall at N 4 / 6 / 8: 64.1 / 67.6 / 65.6 s, flat: the parent's positive
# run (64.4 s, audit 56 s) is the long pole from N 4; min free memory seen 18570 MB of ~20000 at start
TAG = "gate"  # relink.gate_check's link-dir tag; a worker's is <parent TAG>.w<K> (derived/relink/.../link/all=<tag>)


def _one(name, make, root, config):
    """One control inline in this process (the serial body; scratch root/<name>/, removed after) -> its count (0/1)."""
    d = root / name
    good = 0
    print(f"control {name}:")
    undo = None
    try:
        if d.exists():
            shutil.rmtree(d)
        d.mkdir(parents=True)
        kw, expect = make(d, config) if make in CFG_CONTROLS else make(d)
        undo = kw.pop("undo", None)  # 3.5 T4: a control's env/monkeypatch restore, run in finally
        args = {"src_root": FIX_SRC, "list_path": FIX_LIST, "bdir": d / "build", "config": config,
                "run_relink": name.startswith("relink"), "run_audit": name == "audit",
                "run_lint": name.startswith("prov-"), "lint_accepted": FIX_ACC,
                "run_annotations": name.startswith("annotations-"),
                "run_pins": name == "pin-literal-list", **kw}
        # 4.1 T4: kw `direct`: the control's expect does the whole check, no gate run; was: always _core
        rc, v, g, i = (1, {}, {}, {}) if kw.get("direct") else _core(args.pop("src_root"), args.pop("list_path"), **args)
        ok, at = expect(rc, v, g, i)
        ok = ok and rc != 0
        if name in LINE_GLOBALS:  # count line names the failed global (1.8 T1)
            cl = i.get("count_line", "")
            named = cl.split("; FAIL globals: ", 1)[1].split(", ") if "; FAIL globals: " in cl else []
            lok = LINE_GLOBALS[name] in named
            print(f"control {name}: count line {'names' if lok else 'DOES NOT name'} "
                  f"{LINE_GLOBALS[name]}: {cl!r}")
            ok = ok and lok
        good += ok
        print(f"control {name}: failed as expected at {at}" if ok else
              f"control {name}: DID NOT FAIL as expected at {at} (rc {rc})")
    except (Exception, SystemExit) as e:  # a control that cannot run did not fail as expected; 3.5 T4:
        # SystemExit too (a vv_inputs refusal is exit 2); was: `except Exception` (one aborted the self-test)
        print(f"control {name}: DID NOT FAIL as expected: control error {type(e).__name__}: {e}")
    finally:
        if undo:
            undo()
        shutil.rmtree(d, ignore_errors=True)
    return good


def _named(names):
    """names (None: every control) -> [(name, make)]; selftest-passes resolves too (selftest-parallel-pass only).
    5.5 T32: None drops hook-commitmsg-audit when _commitmsg_public() (the SKIPPED line: _parallel, parent only)."""
    if names is None:
        return [c for c in CONTROLS if not (c[0] == "hook-commitmsg-audit" and _commitmsg_public())]
    known = {**dict(CONTROLS), "selftest-passes": _c_selftest_passes}
    return [(n, known[n]) for n in names]


def worker(gate, config, spec_path):
    """gate.py --ctl-worker SPEC (5.4 T5.c2): of spec `names`, the controls with index i % n == k, each inline (_one),
    its stdout captured in the parent's encoding and errors, framed `<i> <count> <len>\\n<bytes>` into spec `out`, then
    `end\\n`. A control's scratch: spec `croot`/<name> (removed after, croot by the parent). Under spec `root`: the
    _core layers' fixed .run dirs (byte_compare._phase_run_dir,
    matched_set.SCRATCH, provenance_lint.LOCALS_SCRATCH) and the relink link-dir tag are rebound here, in this process
    only. No run lock, no positive run. -> 0 (a control's verdict is its frame, never the rc)."""
    global TAG
    spec = json.loads(Path(spec_path).read_text(encoding="utf-8"))
    _bind(gate)
    root, TAG = Path(spec["root"]), spec["tag"]
    byte_compare._phase_run_dir = lambda: root  # relink control/link-tmp/build dirs, the gate's default bdir
    matched_set.SCRATCH = root
    provenance_lint.LOCALS_SCRATCH = root / "provenance_locals"
    check_whole = relink.check_whole  # gate_check's tag `gate` -> TAG (the parent's positive run keeps all=gate)
    relink.check_whole = lambda *a, **kw: check_whole(*a, **(kw | {"tag": TAG} if kw.get("tag") == "gate" else kw))
    k, n = spec["k"], spec["n"]
    croot = Path(spec["croot"])  # the controls root, shared (a control's dir is croot/<name>, printed as serial)
    with open(spec["out"], "wb") as out:
        for i, (name, make) in enumerate(_named(spec["names"])):
            if i % n != k:
                continue
            w = io.TextIOWrapper(io.BytesIO(), encoding=spec["encoding"], errors=spec["errors"], write_through=True)
            with contextlib.redirect_stdout(w):
                good = _one(name, make, croot, config)
            b = w.buffer.getvalue()
            out.write(b"%d %d %d\n" % (i, good, len(b)) + b)
            out.flush()
        out.write(b"end\n")
    return 0


def _frames(path):
    """A worker's block file -> ({i: (count, bytes)}, None) when well formed through `end`, else (frames so far, why)."""
    try:
        b = path.read_bytes()
    except OSError as e:
        return {}, f"no block file ({type(e).__name__})"
    got, at = {}, 0
    while at < len(b):
        nl = b.find(b"\n", at)
        head = b[at:nl].split() if nl >= 0 else []
        if head == [b"end"] and nl + 1 == len(b):
            return got, None
        try:
            i, c, ln = map(int, head)
        except ValueError:
            return got, f"garbled frame at byte {at}"
        body = b[nl + 1:nl + 1 + ln]
        if len(body) != ln or c not in (0, 1) or i in got:
            return got, f"garbled frame at byte {at}"
        got[i], at = (c, body), nl + 1 + ln
    return got, "no end frame"


def _parallel(config, names, n, base, positive=None):
    """The parent (5.4 T5.c2): n workers (gate.py --ctl-worker) on names (None: CONTROLS); a control's scratch
    base/<name>/, a worker's <base>-w<K>/; positive() runs here meanwhile. Each control's block is written to stdout in
    order, the bytes its worker captured. A worker that exits non-zero, writes a garbled or short block file, or prints
    outside its blocks fails every control it had (D26). base, the worker dirs and the workers' relink link dirs are
    removed after. -> (J as expected, K)."""
    lst = _named(names)
    if names is None and _commitmsg_public():  # 5.5 T32: printed here, once; never in a worker (stray output)
        print("control hook-commitmsg-audit: SKIPPED (public tree: githooks/commit-msg is not shipped); not counted")
    n = max(1, min(n, len(lst)))
    wds = [base.parent / f"{base.name}-w{k}" for k in range(n)]
    for p in (base, *base.parent.glob(f"{base.name}-w*")):
        shutil.rmtree(p, ignore_errors=True)
    enc = getattr(sys.stdout, "encoding", None) or "utf-8"
    errs = getattr(sys.stdout, "errors", None) or "strict"
    procs = []
    try:
        for k, wd in enumerate(wds):
            wd.mkdir(parents=True)
            spec = {"k": k, "n": n, "names": None if names is None else list(names), "root": str(wd),
                    "croot": str(base), "out": str(wd / "blocks.bin"), "tag": f"{TAG}.w{k}", "encoding": enc,
                    "errors": errs}
            (wd / "spec.json").write_text(json.dumps(spec), encoding="utf-8")
            cmd = [sys.executable, str(Path(G.__file__).resolve()), "--ctl-worker", str(wd / "spec.json"),
                   *(["--config", str(config)] if config else [])]
            with open(wd / "stray.txt", "wb") as so:
                procs.append(subprocess.Popen(cmd, stdin=subprocess.DEVNULL, stdout=so))
        if positive:
            positive()
        rcs = [p.wait() for p in procs]
        frames, why = [], []
        for k, (rc, wd) in enumerate(zip(rcs, wds)):
            got, err = _frames(wd / "blocks.bin")
            mine = {i for i in range(len(lst)) if i % n == k}
            stray = (wd / "stray.txt").stat().st_size
            frames.append(got)
            why.append(f"rc {rc}" if rc != 0 else err or ("controls missing" if set(got) != mine else None)
                       or (f"{stray} bytes printed outside its blocks" if stray else None))
        good = 0
        for i, (name, _make) in enumerate(lst):
            c, body = frames[i % n].get(i, (0, b""))
            sys.stdout.flush()
            sys.stdout.buffer.write(body)
            if why[i % n] is None:
                good += c
            else:
                print(f"control {name}: DID NOT FAIL as expected: worker {i % n}: {why[i % n]}")
    finally:
        for p in procs:
            if p.poll() is None:
                p.kill()
            p.wait()
        for p in (base, *wds):
            shutil.rmtree(p, ignore_errors=True)
        for ld in relink.OUT.glob(f"retail-*/link/all={TAG}.w*"):
            shutil.rmtree(ld, ignore_errors=True)
    print(f"controls: {good} failed as expected of {len(lst)}")
    return good, len(lst)


def controls(gate, config=None, positive=None):
    """gate: the running gate module (bound by _bind; 5.4 T5). -> (J as expected, K). 5.4 T5.c2: the parent of N
    worker processes (_parallel; N from free memory, never printed); positive (main's positive run) runs meanwhile, its
    output first; was: serial, in this process, after it. A worker's scratch: <.run/phase>/gate/controls/w<K>/, a
    control's <that>/gate/controls/<name>/, removed after (also on exception).
    The relink and audit_public layers run only in their own controls (relink, relink-* 3.4 T6.1; and the positive
    run): 2.6 T11, call bound;
    the provenance layer only in the prov-* controls (3.2 T7); the annotations layer only in the annotations-*
    controls (4.1 T4)."""
    _bind(gate)
    free = timing.free_mb()
    n = 1 if free is None else max(1, min(CAP, (free - RESERVE_MB) // PER_WORKER_MB))
    return _parallel(config, None, n, byte_compare._phase_run_dir() / "gate" / "controls", positive)
