"""Delink pipeline (T5, phase 1.3). Subcommands; T6 adds more.

CLI: delink.py ingest [--fresh] [--config P]
  Headless Ghidra import of the pinned retail XBE (XbeLoader, auto-analysis) into a new project
  <ghidra_project_dir>/retail-<sha1[:8]>/ (outside the repo), then tools/ghidra_scripts/ExportProgram.java writes
  derived/ghidra/retail-<sha1[:8]>/{functions,symbols}.jsonl and summary.json (last). Prints counts and the
  boundary cross-check vs derived/proto_map/retail-xbe/functions.jsonl. Ghidra output: .run/<phase>/logs/delink_ingest*.log.
Exit: 0 ok; 1 check failed (Ghidra rc, missing output, count mismatch, cross-check sums); 2 refusal (hash mismatch,
  project exists without --fresh, proto_map missing or for another target).
CLI: delink.py pick [--config P]   (T6)
  Applies the T6 selection rule to proto_map identical rows (see docs/ops/tools.md); prints the candidate count and
  the lowest-VA pick (VA, size, PDB name, module, Ghidra name). Exit 1 if none; 2 refusal (sha1, T5 export missing).
CLI: delink.py one <va> [--config P]   (T6)
  analyzeHeadless -process on the existing project (-noanalysis -readOnly) + DelinkProgram.java COFF export of the
  T5 body range to derived/delink_one/retail-<sha8>/<0xVA>.obj (outside the cut; T1, 2.1), then an objdiff-cli self-diff (read check) to
  <0xVA>.objdiff.json; prints rc, symbol, match_percent (3.8.3 JSON has no fuzzy_match_percent). Logs .run/<phase>/logs/delink_one*.log. Exit 0 ok;
  1 Ghidra rc, no/unparseable obj, objdiff failure; 2 refusal (sha1, T5 export or project missing, no T5 function).
CLI: delink.py --fresh [--config P]   (T11)
  ingest --fresh with tools/ghidra_scripts/ApplySymbols.java (config/retail/symbols.txt names) run before
  ExportProgram.java, which writes the post-apply export to derived/ghidra/retail-<sha8>/applied/ (T5's
  pre-apply files beside it are never touched; refusal 2 if they are missing); then analyzeHeadless -process -noanalysis -readOnly + DelinkProgram.java with one
  /include-range <text_start>-<text_end-1> /export pair per `delink = true` unit of config/retail/units.toml (one
  call: DelinkRanges.java reads them from an args file, 5.4 T3.c2) to derived/delink/retail-<sha8>/<name, backslash and / -> __>; hashes.txt
  (<sha256>  <file>, sorted), objdiff.json (target-only units) and `objdiff-cli report generate` -> report.json.
  cut.complete is unlinked first and written last (`cut-complete v1 hashes-sha256 <hex64> objects <N>`, T1 2.1);
  readers refuse without a valid one (cut_check). Memory guard (ghidra_mem_guard) first in ingest and --fresh. Logs .run/<phase>/logs/delink_fresh*.log. Exit 0 ok; 1 any rc, ApplySymbols failure, missing object or report unit;
  2 refusal (sha1, T5 export missing).
  REL32 post-pass (T3 2.1; every `delink = true` unit since T5 2.1): after label demotion, _add_rel32 adds REL32
  relocs to each unit's .text at its census sites without a reloc (function symbol at the target, field 0; else the
  .text section symbol, field = target offset); prints `rel32: added A in O objects (section-symbol S)`.
CLI: delink.py --repro [--config P]   (T11)
  Two --fresh runs; exit 0 only if both hashes.txt are byte-equal, else lists the differing objects.
CLI: delink.py relocscan [--unit NAME] [--config P]   (T12, phase 1.5; C0034; census T3 2.1)
  cut_check first (refusal 2). Per object listed in derived/delink/retail-<sha8>/hashes.txt, function symbol >= 64 B (byte_compare extent), expected sites =
  proto_map.mask_proc on the retail bytes at its table VA ('abs', and 'rel' with a target outside the extent); code
  relocs = obj relocs at those sites. A function with 0 prints one line: `no Ghidra function` (VA not an entry of
  applied/functions.jsonl), `no address operands` (0 sites), `intra-section branches only` (every site a 'rel' into
  its own obj section: the delinker never relocates those), `Ghidra body ends at +0x..` (every other site past the
  T5 bodyMax) or `unexplained (<n> sites)`.
  Census (T3, 2.1): per delink unit [text_start, text_end) and its cut object's .text (offset = va - text_start,
  asserted on its function symbols with a symbols.txt row; refusal 2 on disagreement), per symbols.txt func row in
  the unit: 'rel' sites with a target in the unit's .text outside the row's extent = N, of which U carry no reloc;
  K = 'rel' sites targeting inside their own extent that carry a reloc. Prints `intra-section rel32: U unrelocated
  of N; relocated inside own extent K`, with --unit NAME also `intra-section rel32 <NAME>: ...`; then
  `body ends early: <name> va .. size .. at +0x..` (applied bodyMax < va+size-1) and `Ghidra body ends early: E
  functions (no Ghidra function at entry: M)`. Exit 0; 1 any unexplained or K > 0; 2 refusal.
Prints counts, addresses, names and hashes only (D46).
"""
import argparse
import hashlib
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import time
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))
import vv_inputs  # noqa: E402

PIN = REPO / "config" / "check.retail_xbe.sha"
PROTO_MAP = REPO / "derived" / "proto_map" / "retail-xbe" / "functions.jsonl"
def _phase_run_dir():
    """.run/<phase> for the open plan's `# Phase <id>` heading (as tools/run.sh, msvc_fixtures), else .run."""
    plan = REPO / "phase-ends" / "current" / "PHASE_PLAN.md"
    if plan.is_file():
        for line in plan.read_text(encoding="utf-8").splitlines():
            m = re.match(r"# Phase (\S+)", line)
            if m:
                return REPO / ".run" / m.group(1)
    return REPO / ".run"


LOGS = _phase_run_dir() / "logs"  # was REPO / ".run" / "1.3" / "logs" (T13, 1.3)
SCRIPTS = REPO / "tools" / "ghidra_scripts"


def refuse(msg):
    print(f"delink: {msg}", file=sys.stderr)
    return 2


def fail(msg):
    print(f"delink: FAIL {msg}", file=sys.stderr)
    return 1


def pinned_sha1():
    for line in PIN.read_text(encoding="ascii").splitlines():
        parts = line.split()
        if len(parts) == 2 and parts[1] == "retail_xbe":
            return parts[0].lower()
    return None


# T1 (2.1, D36): cut.complete written last by fresh(); every cut reader calls cut_check first. 5.4 T3.c2: the
# contract lives in byte_compare (verifier-locked, identical text); was: a second copy of all four here.
from byte_compare import CUT_MARKER, MARKER_RE, cut_check, cut_dir  # noqa: E402,F401

# 5.4 T3.c2: free physical memory a Ghidra launch needs, else refuse before anything is deleted or written.
# 4608 MB = Claude Code's low-memory kill level (C0201, ~4.5 GB free) + 1185 MB java peak working set measured over
# delink --fresh + relink cut (5.4 T3.c2, PowerShell PeakWorkingSet64 1 s poll, MAXMEM 2G; the cut alone 792 MB,
# the ingest's analysis is the peak), rounded up to 512 MB: 5793 -> 6144.
GHIDRA_MIN_FREE_MB = 6144


def ghidra_mem_guard(what):
    """0 when free physical memory >= the threshold (env VV_GHIDRA_MIN_FREE_MB overrides; controls only),
    else prints the refusal and returns 2."""
    import ctypes

    class MEMORYSTATUSEX(ctypes.Structure):
        _fields_ = [("dwLength", ctypes.c_ulong), ("dwMemoryLoad", ctypes.c_ulong),
                    ("ullTotalPhys", ctypes.c_ulonglong), ("ullAvailPhys", ctypes.c_ulonglong),
                    ("ullTotalPageFile", ctypes.c_ulonglong), ("ullAvailPageFile", ctypes.c_ulonglong),
                    ("ullTotalVirtual", ctypes.c_ulonglong), ("ullAvailVirtual", ctypes.c_ulonglong),
                    ("ullAvailExtendedVirtual", ctypes.c_ulonglong)]
    st = MEMORYSTATUSEX()
    st.dwLength = ctypes.sizeof(MEMORYSTATUSEX)
    if not ctypes.windll.kernel32.GlobalMemoryStatusEx(ctypes.byref(st)):
        return refuse(f"{what}: GlobalMemoryStatusEx failed; memory guard cannot run")
    need = int(os.environ.get("VV_GHIDRA_MIN_FREE_MB", GHIDRA_MIN_FREE_MB))
    free = st.ullAvailPhys // (1024 * 1024)
    if free < need:
        return refuse(f"{what}: free physical memory {free} MB < {need} MB threshold; nothing written "
                      f"(close programs and rerun)")
    return 0


def write_marker(d, n_objects):
    """fresh() only, after every check passed: d/cut.complete over d/hashes.txt."""
    hsha = hashlib.sha256((Path(d) / "hashes.txt").read_bytes()).hexdigest()
    (Path(d) / CUT_MARKER).write_bytes(f"cut-complete v1 hashes-sha256 {hsha} objects {n_objects}\n".encode("ascii"))


def count_lines(p):
    with open(p, encoding="utf-8") as f:
        return sum(1 for line in f if line.strip())


def load_jsonl(p):
    with open(p, encoding="utf-8") as f:
        return [json.loads(line) for line in f if line.strip()]


def ingest(args, pre=(), logname="delink_ingest", sub=None):
    """pre: extra (script, *args) postScripts run before ExportProgram.java (--fresh adds ApplySymbols).
    sub: export to derived/ghidra/retail-<sha8>/<sub>/ instead (--fresh: "applied"), leaving T5's files intact."""
    if ghidra_mem_guard("ingest"):  # 5.4 T3.c2: first, before the project or the export is removed
        return 2
    cfg = vv_inputs.load_config(args.config)
    if not PIN.is_file():
        return refuse(f"pin missing: {PIN}")
    want = pinned_sha1()
    if not want:
        return refuse(f"no retail_xbe line in {PIN}")
    xbe = vv_inputs.input_path("retail_xbe", cfg=cfg)
    got = hashlib.sha1(xbe.read_bytes()).hexdigest()
    if got != want:
        return refuse(f"retail_xbe sha1 {got} != pinned {want}: {xbe}")
    if not PROTO_MAP.is_file():
        return refuse(f"proto_map missing: {PROTO_MAP} (run tools/proto_map.py)")
    rows = load_jsonl(PROTO_MAP)
    bad = {r.get("targetSha1") for r in rows} - {want}
    if bad:
        return refuse(f"proto_map targetSha1 {sorted(map(str, bad))} != pinned {want}")

    tag = f"retail-{want[:8]}"
    root = Path(cfg["ghidra_project_dir"]).resolve()
    proj = root / tag
    if any(part.startswith(".") for part in proj.parts[1:]):
        return refuse(f"project path has an element starting with '.': {proj}")
    if proj.exists():
        if not args.fresh:
            return refuse(f"project exists: {proj} (use --fresh; projects are never reused)")
        if proj.parent != root or not proj.name.startswith("retail-"):
            return refuse(f"refusing to delete {proj}: not a retail-* child of {root}")
        shutil.rmtree(proj)
    proj.mkdir(parents=True)
    out = REPO / "derived" / "ghidra" / tag
    if sub:
        out = out / sub
    if out.exists():
        shutil.rmtree(out)
    LOGS.mkdir(parents=True, exist_ok=True)
    bat = Path(cfg["ghidra_dir"]) / "support" / "analyzeHeadless.bat"
    cmd = [str(bat), str(proj), tag, "-import", str(xbe), "-loader", "XbeLoader",
           "-scriptPath", str(SCRIPTS), "-preScript", "TrimAnalyzers.java"]  # 5.4 T3.c4: analyzer trim
    for script in pre:
        cmd += ["-postScript", *script]
    cmd += ["-postScript", "ExportProgram.java", str(out),
            "-log", str(LOGS / f"{logname}.ghidra.log"), "-scriptlog", str(LOGS / f"{logname}.script.log")]
    with open(LOGS / f"{logname}.log", "w", encoding="utf-8") as log:
        rc = subprocess.run(cmd, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT).returncode
    if rc != 0:
        return fail(f"analyzeHeadless rc {rc} (see {LOGS / (logname + '.log')})")
    files = {k: out / f"{k}.jsonl" for k in ("functions", "symbols")}
    summ = out / "summary.json"
    for p in (*files.values(), summ):
        if not p.is_file():
            return fail(f"missing output {p} (see {LOGS / (logname + '.log')})")
    summary = json.loads(summ.read_text(encoding="utf-8"))
    for k, p in files.items():
        n = count_lines(p)
        if n != summary.get(k):
            return fail(f"{p.name} has {n} lines, summary.json says {summary.get(k)}")
    if summary.get("executableSHA256") != hashlib.sha256(xbe.read_bytes()).hexdigest():
        return fail(f"summary executableSHA256 {summary.get('executableSHA256')} != retail_xbe sha256")

    funcs = load_jsonl(files["functions"])
    syms = load_jsonl(files["symbols"])
    print(f"ghidra {summary['ghidraVersion']} loader {summary['loader']} project {proj}")
    print(f"functions {len(funcs)} (thunks {sum(f['thunk'] for f in funcs)})")
    print(f"symbols {len(syms)} labels {sum(s['symbolType'] == 'Label' for s in syms)}")
    print(f"xbsymdb {sum(s['xbsymdb'] for s in syms)} xtlid {sum(s['xtlid'] for s in syms)} "
          f"kernel {sum(s['kernel'] for s in syms)}")
    print("symbols by source: " + ", ".join(f"{k} {v}" for k, v in sorted(Counter(s["source"] for s in syms).items())))
    print("functions by nameSource: " +
          ", ".join(f"{k} {v}" for k, v in sorted(Counter(f["nameSource"] for f in funcs).items())))

    length = {}
    for r in rows:
        if r.get("class") == "identical" and r.get("targetVAs"):
            length.setdefault(int(r["targetVAs"][0], 16), r["length"])  # first row wins on a duplicate VA
    gsize = {int(f["entry"], 16): f["size"] for f in funcs}
    m, g = set(length), set(gsize)
    agree, monly, gonly = len(m & g), len(m - g), len(g - m)
    same = sum(gsize[v] == length[v] for v in m & g)
    print(f"boundary vs proto_map identical: agree {agree}, map-only {monly} of |M| {len(m)}, "
          f"ghidra-only {gonly} of |G| {len(g)}")
    print(f"agree size-equal {same}, size-differs {agree - same}")
    if agree + monly != len(m) or agree + gonly != len(g) or same + (agree - same) != agree:
        return fail("cross-check sums do not hold")
    return 0


def _pinned_inputs(cfg):
    """(sha1, xbe bytes, xbe path, T5 dir) or an int refusal code."""
    want = pinned_sha1() if PIN.is_file() else None
    if not want:
        return refuse(f"no retail_xbe pin in {PIN}")
    xbe = vv_inputs.input_path("retail_xbe", cfg=cfg)
    data = xbe.read_bytes()
    got = hashlib.sha1(data).hexdigest()
    if got != want:
        return refuse(f"retail_xbe sha1 {got} != pinned {want}: {xbe}")
    t5 = REPO / "derived" / "ghidra" / f"retail-{want[:8]}"
    if not all((t5 / n).is_file() for n in ("functions.jsonl", "symbols.jsonl", "summary.json")):
        return refuse(f"T5 export missing: {t5} (run tools/delink.py ingest)")
    return want, data, xbe, t5


def _game_module(m):
    """Game module test for proto_map `modules`: they hold relative obj paths; the XDK libs sit under obj\\i386\\."""
    low = m.lower()
    return not (low.startswith("obj\\i386\\") or low.startswith("d:\\xboxfre-jun03\\") or "* linker *" in low)


def pick(args):
    from iced_x86 import Code, Decoder
    import proto_map
    import xbe_info
    cfg = vv_inputs.load_config(args.config)
    got = _pinned_inputs(cfg)
    if isinstance(got, int):
        return got
    want, data, _xbe, t5 = got
    if not PROTO_MAP.is_file():
        return refuse(f"proto_map missing: {PROTO_MAP}")
    rows = load_jsonl(PROTO_MAP)
    dup = REPO / "derived" / "dup_report"
    bad_r, bad_p = set(), set()
    for r in load_jsonl(dup / "retail_groups.jsonl"):
        bad_r |= {int(m["va"], 16) for m in r["members"]}
    for r in load_jsonl(dup / "retail_folds.jsonl"):
        bad_r.add(int(r["retailVA"], 16))
    for r in load_jsonl(dup / "multi_name.jsonl"):
        bad_p.add(int(r["va"], 16))  # multi_name va is a prototype VA (0x004xxxxx), not retail
    for r in load_jsonl(dup / "proto_groups.jsonl"):
        bad_p |= {int(m["va"], 16) for m in r["members"]}
    funcs = {int(f["entry"], 16): f for f in load_jsonl(t5 / "functions.jsonl")}
    x = xbe_info.parse_xbe(data)
    lo = x["header"]["baseAddr"]
    hi = lo + x["header"]["sizeOfImage"]
    # data = no exec flag, or .rdata/.data: this XBE flags .rdata 0x6 and .data 0x7 executable
    dsecs = [(s["virtualAddr"], s["virtualAddr"] + s["virtualSize"]) for s in x["sections"]
             if not s["flags"] & proto_map.SEC_EXEC or s["name"] in (".rdata", ".data")]
    ident = [r for r in rows if r.get("class") == "identical" and r.get("hits") == 1
             and len(r.get("targetVAs") or []) == 1]
    ident_va = Counter(int(r["targetVAs"][0], 16) for r in ident)
    cands = []
    for r in ident:
        va = int(r["targetVAs"][0], 16)
        mods = r.get("modules") or []
        if not r.get("names") or not mods or not all(_game_module(m) for m in mods) or r["length"] < 64:
            continue
        if va in bad_r or int(r["protoVA"], 16) in bad_p:
            continue
        f = funcs.get(va)
        if not f or f["size"] != r["length"]:
            continue
        off = xbe_info.va_to_off(x, va)
        buf = data[off:off + r["length"]]
        calls = 0
        d = Decoder(32, buf, ip=va)
        for ins in d:
            if ins.code == Code.CALL_REL32_32:
                t = ins.near_branch_target
                calls += t != va and ident_va.get(t, 0) >= 1
        masks, _err = proto_map.mask_proc(buf, va, lo, hi)
        dabs = sum(1 for o, _n, k in masks if k == "abs"
                   and any(a <= struct.unpack_from("<I", buf, o)[0] < b for a, b in dsecs))
        if calls and dabs:
            cands.append((va, r, f))
    print(f"candidates {len(cands)} (game module: no modules path under obj\\i386\\, D:\\xboxfre-jun03\\, * Linker *; "
          f"data: no exec flag or .rdata/.data)")
    if not cands:
        return fail("no candidate")
    va, r, f = min(cands, key=lambda c: c[0])
    print(f"pick 0x{va:08X} size {r['length']} name {r['names'][0]} module {r['modules'][0]} ghidra {f['name']}")
    return 0


def _find_percent(o, sym):
    """match_percent of the left symbol named sym in objdiff-cli 3.8.3 JSON (it has no fuzzy_match_percent key)."""
    for y in o.get("left", {}).get("symbols", []):
        if y.get("name") == sym:
            return y.get("match_percent")
    return None


def one(args):
    import coff_info
    cfg = vv_inputs.load_config(args.config)
    got = _pinned_inputs(cfg)
    if isinstance(got, int):
        return got
    want, _data, xbe, t5 = got
    va = int(args.va, 16)
    tag = f"retail-{want[:8]}"
    proj = Path(cfg["ghidra_project_dir"]).resolve() / tag
    if not (proj / f"{tag}.gpr").is_file():
        return refuse(f"project missing: {proj / (tag + '.gpr')} (run tools/delink.py ingest)")
    f = next((f for f in load_jsonl(t5 / "functions.jsonl") if int(f["entry"], 16) == va), None)
    if not f:
        return refuse(f"no T5 function at 0x{va:08X}")
    bmin, bmax = int(f["bodyMin"], 16), int(f["bodyMax"], 16)
    outdir = REPO / "derived" / "delink_one" / tag  # T1 (2.1): outside the cut; was derived/delink/<tag>
    outdir.mkdir(parents=True, exist_ok=True)
    obj = outdir / f"0x{va:08X}.obj"
    js = outdir / f"0x{va:08X}.objdiff.json"
    for p in (obj, js):
        if p.exists():
            p.unlink()
    LOGS.mkdir(parents=True, exist_ok=True)
    bat = Path(cfg["ghidra_dir"]) / "support" / "analyzeHeadless.bat"
    cmd = [str(bat), str(proj), tag, "-process", xbe.name, "-noanalysis", "-readOnly",
           "-postScript", "DelinkProgram.java", "/exporter", "COFF relocatable object",
           "/include-range", f"{bmin:08x}-{bmax:08x}", "/export", str(obj),
           "-log", str(LOGS / "delink_one.ghidra.log"), "-scriptlog", str(LOGS / "delink_one.script.log")]
    with open(LOGS / "delink_one.log", "w", encoding="utf-8") as log:
        rc = subprocess.run(cmd, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT).returncode
    if rc != 0:
        return fail(f"analyzeHeadless rc {rc} (see {LOGS / 'delink_one.log'})")
    if not obj.is_file():
        return fail(f"no obj written: {obj} (see {LOGS / 'delink_one.log'})")
    try:
        c = coff_info.parse(obj.read_bytes())
    except (coff_info.BadCoff, struct.error, ValueError) as e:
        return fail(f"unparseable obj {obj}: {e}")
    names = [y["name"] for y in c["symbols"].values() if y["section"] > 0]
    sym = f["name"] if f["name"] in names else next(
        (y["name"] for y in c["symbols"].values() if y["section"] > 0 and y["type"] == 0x20), None)
    if not sym:
        return fail(f"no function symbol in {obj}")
    print(f"obj {obj} sections {len(c['sections'])} symbols {len(c['symbols'])} "
          f"relocs {sum(len(s['relocs']) for s in c['sections'])} range 0x{bmin:08X}-0x{bmax:08X}")
    cmd = [str(cfg["objdiff_cli"]), "diff", "-1", str(obj), "-2", str(obj), sym, "-o", str(js), "--format", "json"]
    with open(LOGS / "delink_one.objdiff.log", "w", encoding="utf-8") as log:
        orc = subprocess.run(cmd, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT).returncode
    pct = _find_percent(json.loads(js.read_text(encoding="utf-8")), sym) if js.is_file() else None
    print(f"objdiff rc {orc} symbol {sym} match_percent {pct}")
    if orc != 0 or pct is None:
        return fail(f"objdiff self-diff failed (see {LOGS / 'delink_one.objdiff.log'})")
    return 0


SYMBOLS = REPO / "config" / "retail" / "symbols.txt"
SYMBOLS_PIN = REPO / "config" / "retail" / "symbols_pin.txt"  # 2.1 T4: applied after SYMBOLS, as gate.py SYMTABS
UNITS = REPO / "config" / "retail" / "units.toml"
APPLY_RE = re.compile(r"ApplySymbols: lines (\d+) renamed (\d+) labelled (\d+) failed (\d+) "
                      r"\(same-name labels removed (\d+), duplicate names reset (\d+)\) "
                      r"created (\d+) clipped (\d+) inside (\d+) createFailed (\d+) holes (\d+) instrs (\d+) "
                      r"grown (\d+) growRefused (\d+) split (\d+) splitBelow (\d+) absorbed (\d+) "
                      r"scalarRefs data (\d+) func (\d+)")  # holes: 2.2 T9; grown: 3.1 T8; split: 3.2 T2; scalar, splitBelow: 3.2 T4
FID_RE = re.compile(r"ApplySymbols: FID canonicalised addresses \d+ labels \d+ functions \d+")


def _unit_file(name):
    """Object file name for a unit: \\ and / -> __; names already end in .obj, so .obj is appended only if absent."""
    stem = name.replace("\\", "__").replace("/", "__")
    return stem if stem.lower().endswith(".obj") else stem + ".obj"


GHIDRA_LABEL = re.compile(r"^(switchD_|LAB_)|caseD_|::(switchD|default)$")


def _demote_labels(outdir, files):
    """T11 (1.5): Ghidra default labels exported as class 2 (external) inside a function's extent end that function
    in objdiff (C0033). Rewrite their storage-class byte (entry offset 16) to 3 (static) in place: no renumbering,
    so relocation symbol indices stay valid. -> (demoted, objects) or an error string."""
    import coff_info
    known = {ln.split()[3] for tab in (SYMBOLS, SYMBOLS_PIN) for ln in tab.read_text(encoding="utf-8").splitlines()
             if ln.strip() and not ln.startswith("#") and len(ln.split()) > 3}
    demoted, ext_refs, blobs = {}, {}, {}
    for f in sorted(files):
        data = bytearray((outdir / f).read_bytes())
        try:
            c = coff_info.parse(bytes(data))
        except (coff_info.BadCoff, struct.error, ValueError, IndexError) as e:
            return f"unparseable object {f}: {e}"
        syms = c["symbols"]
        ext_refs[f] = {syms[r["symbol"]]["name"] for s in c["sections"] for r in s["relocs"]
                       if syms[r["symbol"]]["section"] == 0}
        symptr = struct.unpack_from("<I", data, 8)[0]
        hits = []
        for s in c["sections"]:
            starts = sorted(y["value"] for y in syms.values() if y["section"] == s["number"] and y["class"] == 2
                            and not GHIDRA_LABEL.search(y["name"]))
            for y in syms.values():
                if y["section"] != s["number"] or y["class"] != 2 or y["name"] in known \
                        or not GHIDRA_LABEL.search(y["name"]) or y["type"] == 0x20:
                    continue
                lo = [v for v in starts if v < y["value"]]
                hi = [v for v in starts if v > y["value"]] + [s["size"]]
                if lo and y["value"] < min(hi) and y["value"] not in starts:
                    hits.append(y)
        for y in hits:
            data[symptr + 18 * y["index"] + 16] = coff_info.SC_STATIC
        if hits:
            demoted[f] = {y["name"] for y in hits}
            blobs[f] = bytes(data)
    for f, names in demoted.items():
        for g, refs in ext_refs.items():
            if g != f and names & refs:
                return f"demoted label {sorted(names & refs)[0]} of {f} is referenced by a relocation in {g}"
    for f, b in blobs.items():
        (outdir / f).write_bytes(b)
    return sum(len(v) for v in demoted.values()), len(demoted)


def _func_rows():
    """symbols.txt and sized symbols_pin.txt func rows -> [(va, size, name)], one per VA (first seen, symbols.txt
    first: fold-alias pins put several names on one VA); pin rows with size '-' or 0 are skipped."""
    rows, seen = [], set()
    for path in (SYMBOLS, SYMBOLS_PIN):
        for ln in path.read_text(encoding="utf-8").splitlines():
            p = ln.split()
            if len(p) > 3 and not ln.startswith("#") and p[1] == "func" and p[2] != "-":
                va, size = int(p[0], 16), int(p[2], 16)
                if size and va not in seen:
                    seen.add(va)
                    rows.append((va, size, p[3]))
    return rows


def _rel_targets(xb, x, va, size):
    """'rel' sites of the retail bytes [va, va+size) -> [(site va, target va)] (proto_map.mask_proc)."""
    import proto_map
    import xbe_info
    lo_img = x["header"]["baseAddr"]
    off = xbe_info.va_to_off(x, va)
    buf = xb[off:off + size]
    masks, _err = proto_map.mask_proc(buf, va, lo_img, lo_img + x["header"]["sizeOfImage"])
    return [(va + o, (va + o + 4 + struct.unpack_from("<i", buf, o)[0]) & 0xFFFFFFFF) for o, _n, k in masks
            if k == "rel"]


def _add_rel32(outdir, units, xb, x):
    """T3 (2.1, route b): per unit, add IMAGE_REL_I386_REL32 at the census sites (symbols.txt and sized symbols_pin.txt func rows in the unit,
    'rel' target outside the row's extent and inside [text_start, text_end)) that carry no .text reloc yet. Symbol: a
    function symbol of .text at the target offset (field 0), else the .text section symbol (field = target offset).
    Entries merge into .text's reloc table sorted by offset; later file pointers shift. -> (added, objects,
    section-symbol uses) or an error string."""
    import coff_info
    funcs = _func_rows()
    added = objs = secsym = 0
    for u in sorted(units, key=lambda u: u["name"]):
        ts, te = u["text_start"], u["text_end"]
        f = outdir / _unit_file(u["name"])
        data = bytearray(f.read_bytes())
        try:
            c = coff_info.parse(bytes(data))
        except (coff_info.BadCoff, struct.error, ValueError, IndexError) as e:
            return f"unparseable object {f.name}: {e}"
        texts = [s for s in c["sections"] if s["name"] == ".text"]
        if len(texts) != 1:
            return f"{f.name}: {len(texts)} .text sections"
        sec = texts[0]
        syms = c["symbols"]
        roffs = {r["offset"] for r in sec["relocs"]}
        fsym = {}
        for y in sorted(syms.values(), key=lambda y: y["index"]):
            if y["type"] == 0x20 and y["section"] == sec["number"] and y["class"] == 2:  # byte_compare.is_func
                fsym.setdefault(y["value"], y["index"])
        ssym = [y["index"] for y in syms.values() if y["class"] == coff_info.SC_STATIC and y["value"] == 0
                and y["section"] == sec["number"] and y["name"] == ".text"]
        new = {}
        for va, size, _name in funcs:
            if ts <= va < te:
                for s, t in _rel_targets(xb, x, va, size):
                    if not va <= t < va + size and ts <= t < te and s - ts not in roffs:
                        new[s - ts] = t - ts
        if not new:
            continue
        if any(t not in fsym for t in new.values()) and not ssym:
            return f"{f.name}: no .text section symbol"
        ents = []
        for o, t in sorted(new.items()):
            si, field = (fsym[t], 0) if t in fsym else (ssym[0], t)
            secsym += t not in fsym
            struct.pack_into("<I", data, sec["rawPtr"] + o, field)
            ents.append((o, si))
        h = 20 + struct.unpack_from("<H", data, 16)[0] + 40 * (sec["number"] - 1)
        relptr, nrel = struct.unpack_from("<I", data, h + 24)[0], struct.unpack_from("<H", data, h + 32)[0]
        old = [struct.unpack_from("<IIH", data, relptr + 10 * r) for r in range(nrel)]
        if nrel + len(ents) > 0xFFFF:
            return f"{f.name}: {nrel + len(ents)} .text relocations exceed 0xFFFF"
        at = relptr + 10 * nrel if nrel else sec["rawPtr"] + sec["size"]
        grow = 10 * len(ents)
        nsec = struct.unpack_from("<H", data, 2)[0]
        optsz = struct.unpack_from("<H", data, 16)[0]
        if struct.unpack_from("<I", data, 8)[0] >= at:
            struct.pack_into("<I", data, 8, struct.unpack_from("<I", data, 8)[0] + grow)
        for k in range(nsec):
            for p in (20, 24, 28):  # PointerToRawData, PointerToRelocations, PointerToLinenumbers
                q = 20 + optsz + 40 * k + p
                v = struct.unpack_from("<I", data, q)[0]
                if v and v >= at and not (k == sec["number"] - 1 and p == 24):
                    struct.pack_into("<I", data, q, v + grow)
        merged = sorted(old + [(o, si, coff_info.REL32) for o, si in ents], key=lambda r: r[0])
        table = b"".join(struct.pack("<IIH", *r) for r in merged)
        start = relptr if nrel else at
        data[start:at] = table
        struct.pack_into("<I", data, h + 24, start)
        struct.pack_into("<H", data, h + 32, len(merged))
        f.write_bytes(bytes(data))
        added += len(ents)
        objs += 1
    return added, objs, secsym


def fresh(args):
    """T5 ingest + ApplySymbols, then one COFF object per `delink = true` unit, hashes.txt and an objdiff report."""
    import tomllib
    import xbe_info
    t0 = time.monotonic()
    if ghidra_mem_guard("--fresh"):  # 5.4 T3.c2: before the cut marker is unlinked
        return 2
    pin = pinned_sha1() if PIN.is_file() else None
    if pin:  # T1 (2.1, D36): first filesystem action; the cut is incomplete until write_marker() below
        (cut_dir(pin) / CUT_MARKER).unlink(missing_ok=True)
    if isinstance(_pinned_inputs(vv_inputs.load_config(args.config)), int):  # T5 export must exist; it is never rewritten
        return 2
    rc = ingest(argparse.Namespace(fresh=True, config=args.config),
                pre=[("ApplySymbols.java", str(SYMBOLS), str(SYMBOLS_PIN))], logname="delink_fresh_ingest", sub="applied")
    if rc != 0:
        return rc
    t1 = time.monotonic()
    slog = (LOGS / "delink_fresh_ingest.script.log").read_text(encoding="utf-8", errors="replace")
    # T11 (1.8, C0053): Function ID names vary between fresh runs; ApplySymbols resets them to DEFAULT first.
    fid = FID_RE.findall(slog)
    if not fid:
        return fail(f"no ApplySymbols FID line in {LOGS / 'delink_fresh_ingest.script.log'}")
    print(fid[-1])
    m = APPLY_RE.findall(slog)
    if not m:
        return fail(f"no ApplySymbols result line in {LOGS / 'delink_fresh_ingest.script.log'}")
    (lines, renamed, labelled, failed, dropped, reset, created, clipped, inside, cfail, holes, hinstrs, grown, grefused,
     split, splitbelow, absorbed, sdata, sfunc) = map(
        int, m[-1])
    print(f"ApplySymbols: lines {lines} renamed {renamed} labelled {labelled} failed {failed} "
          f"(same-name labels removed {dropped}, duplicate names reset {reset}) "
          f"created {created} clipped {clipped} inside {inside} createFailed {cfail} holes {holes} instrs {hinstrs} "
          f"grown {grown} growRefused {grefused} split {split} splitBelow {splitbelow} absorbed {absorbed} "
          f"scalarRefs data {sdata} func {sfunc}")
    if failed or renamed + created + labelled != lines:
        return fail("ApplySymbols failures (names in the script log)")
    print(f"stage ingest+apply {t1 - t0:.1f}s")

    cfg = vv_inputs.load_config(args.config)
    got = _pinned_inputs(cfg)
    if isinstance(got, int):
        return got
    want, xbe_bytes, xbe, _t5 = got
    tag = f"retail-{want[:8]}"
    proj = Path(cfg["ghidra_project_dir"]).resolve() / tag
    units = [u for u in tomllib.loads(UNITS.read_text(encoding="utf-8"))["unit"] if u.get("delink") is True]
    files = [_unit_file(u["name"]) for u in units]
    bad = [f for f in files if not re.fullmatch(r"[A-Za-z0-9_.\-]+", f)]
    if bad:
        return fail(f"unit names not usable as file names: {bad}")
    if len(set(f.lower() for f in files)) != len(files):
        return fail("unit object file names are not unique")
    outdir = REPO / "derived" / "delink" / tag
    outdir.mkdir(parents=True, exist_ok=True)
    for p in [*outdir.glob("*.obj"), *outdir.glob("*.objdiff.json"),
              *(outdir / n for n in ("hashes.txt", "objdiff.json", "report.json"))]:
        if p.is_file():
            p.unlink()
    bat = Path(cfg["ghidra_dir"]) / "support" / "analyzeHeadless.bat"
    # 5.4 T3.c2: every unit range in one args file, one analyzeHeadless call (DelinkRanges.java runs DelinkProgram
    # once); was: chunks under ARG_BUDGET (5500 chars; cmd.exe caps a line at 8191), one call each (6 calls)
    lines = ["/exporter", "COFF relocatable object"]
    for u, f in zip(units, files):
        if u["text_end"] <= u["text_start"]:
            return fail(f"empty range for unit {u['name']}")
        lines += ["/include-range", f"{u['text_start']:08x}-{u['text_end'] - 1:08x}", "/export", str(outdir / f)]
    chunks = [lines]  # one call; the summary line below still counts calls
    LOGS.mkdir(parents=True, exist_ok=True)
    argf = LOGS / "delink_fresh_export.args"
    argf.write_text("".join(s + "\n" for s in lines), encoding="utf-8")
    elog = LOGS / "delink_fresh_export.log"
    cmd = [str(bat), str(proj), tag, "-process", xbe.name, "-noanalysis", "-readOnly",
           "-scriptPath", str(SCRIPTS), "-postScript", "DelinkRanges.java", str(argf),
           "-log", str(LOGS / "delink_fresh_export.ghidra.log"),
           "-scriptlog", str(LOGS / "delink_fresh_export.script.log")]
    with open(elog, "w", encoding="utf-8") as log:
        rc = subprocess.run(cmd, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT).returncode
    if rc != 0:
        return fail(f"analyzeHeadless rc {rc} (see {elog})")
    # a script exception (e.g. DelinkProgram "Failed to export") leaves rc 0 and skips the rest of the call
    if "REPORT SCRIPT ERROR" in elog.read_text(encoding="utf-8", errors="replace"):
        return fail(f"DelinkProgram script error (see {elog})")
    missing = [f for f in files if not (outdir / f).is_file()]
    if missing:
        return fail(f"{len(missing)} objects not written: {missing[:10]}")
    got = _demote_labels(outdir, files)
    if isinstance(got, str):
        return fail(f"labels: {got}")
    print(f"labels: demoted {got[0]} in {got[1]} objects")
    got = _add_rel32(outdir, units, xbe_bytes, xbe_info.parse_xbe(xbe_bytes))  # T5 (2.1): every delink unit
    if isinstance(got, str):
        return fail(f"rel32: {got}")
    print(f"rel32: added {got[0]} in {got[1]} objects (section-symbol {got[2]})")
    t2 = time.monotonic()
    print(f"units {len(units)} objects {len(files)} export calls {len(chunks)}; stage export {t2 - t1:.1f}s")

    hashes = "".join(f"{hashlib.sha256((outdir / f).read_bytes()).hexdigest()}  {f}\n" for f in sorted(files))
    (outdir / "hashes.txt").write_text(hashes, encoding="ascii", newline="\n")
    hsha = hashlib.sha256(hashes.encode("ascii")).hexdigest()
    print(f"hashes.txt {outdir / 'hashes.txt'} sha256 {hsha}")

    # objdiff project: target-only units (no base yet); unit name = the units.toml name with / separators
    names = [u["name"].replace("\\", "/") for u in units]
    proj_json = {"units": [{"name": n, "target_path": f} for n, f in zip(names, files)]}
    (outdir / "objdiff.json").write_text(json.dumps(proj_json, indent=2) + "\n", encoding="utf-8", newline="\n")
    report = outdir / "report.json"
    cmd = [str(cfg["objdiff_cli"]), "report", "generate", "-p", str(outdir), "-o", str(report), "-f", "json"]
    with open(LOGS / "delink_fresh_objdiff.log", "w", encoding="utf-8") as log:
        orc = subprocess.run(cmd, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT).returncode
    if orc != 0 or not report.is_file():
        return fail(f"objdiff report generate rc {orc} (see {LOGS / 'delink_fresh_objdiff.log'})")
    got = {u.get("name") for u in json.loads(report.read_text(encoding="utf-8")).get("units", [])}
    absent = sorted(set(names) - got)
    t3 = time.monotonic()
    print(f"objdiff report rc {orc} units {len(got & set(names))}/{len(names)}; stage objdiff {t3 - t2:.1f}s; "
          f"total {t3 - t0:.1f}s")
    if absent:
        return fail(f"{len(absent)} units missing from report.json: {absent[:10]}")
    write_marker(outdir, len(files))
    print(f"marker {outdir / CUT_MARKER} objects {len(files)}")
    return 0


def repro(args):
    """Two full --fresh runs; exit 0 only if both hashes.txt are byte-equal."""
    runs = []
    for k in (1, 2):
        print(f"--- repro run {k}")
        rc = fresh(args)
        if rc != 0:
            return rc
        h = REPO / "derived" / "delink" / f"retail-{pinned_sha1()[:8]}" / "hashes.txt"
        runs.append(h.read_bytes())
        print(f"run {k} hashes.txt sha256 {hashlib.sha256(runs[-1]).hexdigest()}")
    return compare_hashes(*runs)


def compare_hashes(ra, rb):
    """--repro's compare of two hashes.txt byte strings: 0 byte-equal, else `differs: <obj>` lines and 1 (3.5 T7)."""
    if ra == rb:
        print("repro: hashes.txt byte-equal")
        return 0
    a, b = ({ln.split()[1]: ln.split()[0] for ln in r.decode("ascii").splitlines()} for r in (ra, rb))
    diff = sorted(f for f in set(a) | set(b) if a.get(f) != b.get(f))
    for f in diff:
        print(f"differs: {f}")
    return fail(f"repro: {len(diff)} objects differ between runs")


def relocscan(args):
    """C0034's scan: byte-matched functions of >= 64 B whose delinked object carries no code relocation."""
    import byte_compare
    import proto_map
    import xbe_info
    try:
        pin, byname, _t, _xp, xb, x = byte_compare.setup(argparse.Namespace(symbols=None, xbe=None, config=args.config))
    except byte_compare.Refuse as e:
        return refuse(str(e))
    d = cut_dir(pin)
    why = cut_check(d)
    if why:
        return refuse(why)
    fj = REPO / "derived" / "ghidra" / f"retail-{pin[:8]}" / "applied" / "functions.jsonl"
    # T1 (2.1): the objects hashes.txt lists, not glob("*.obj")
    objs = [d / ln.split()[1] for ln in (d / "hashes.txt").read_text(encoding="ascii").splitlines() if ln.strip()]
    if not objs or not fj.is_file():
        return refuse(f"no objects under {d} or no {fj} (run tools/delink.py --fresh)")
    bodymax = {int(f["entry"], 16): int(f["bodyMax"], 16) for f in load_jsonl(fj)}
    lo_img = x["header"]["baseAddr"]
    hi_img = lo_img + x["header"]["sizeOfImage"]
    rows = []
    for obj in objs:
        try:
            _data, c = byte_compare.load_obj(obj)
        except byte_compare.Refuse as e:
            return refuse(str(e))
        for y in c["symbols"].values():
            if not byte_compare.is_func(y):
                continue
            sec, lo, hi = byte_compare.extent(c, y)
            va = byte_compare.unique_va(byname, y["name"])
            if hi - lo < 64:
                continue
            if va is None:
                return refuse(f"{y['name']} in {obj.name}: no unique table VA")
            rows.append((va, hi - lo, y["name"], {r["offset"] - lo for r in sec["relocs"] if lo <= r["offset"] < hi},
                         va - lo, sec["size"]))
    rows.sort(key=lambda r: (r[0], r[2]))
    free = expl = unexpl = partial = 0
    for va, size, name, rel_offs, base, secsize in rows:
        off = xbe_info.va_to_off(x, va)
        buf = xb[off:off + size]
        masks, _err = proto_map.mask_proc(buf, va, lo_img, hi_img)
        sites, insec = set(), set()
        for o, _n, k in masks:
            t = (va + o + 4 + struct.unpack_from("<i", buf, o)[0]) & 0xFFFFFFFF if k == "rel" else None
            if t is not None and va <= t < va + size:
                continue
            sites.add(o)
            if t is not None and base <= t < base + secsize:
                insec.add(o)
        code = len(sites & rel_offs)
        if code:
            partial += code < len(sites)
            continue
        free += 1
        if va not in bodymax:
            why = "no Ghidra function"
        elif not sites:
            why = "no address operands"
        elif sites == insec:  # T12: 0 of 1013 intra-section rel sites relocated at retail-2953b782, 7798 of 8886 others
            why = "intra-section branches only"
        elif all(o > bodymax[va] - va for o in sites - insec):  # T12: code past the body is in no function
            why = f"Ghidra body ends at +0x{bodymax[va] - va + 1:X}"
        else:
            why = f"unexplained ({len(sites)} sites)"
        expl += not why.startswith("unexplained")
        unexpl += why.startswith("unexplained")
        print(f"{name} va 0x{va:08X} size 0x{size:X}: {why}")
    print(f"relocscan: functions>=64B {len(rows)} reloc-free {free} explained {expl} unexplained {unexpl}; "
          f"partial {partial}")
    # T3 (2.1): intra-section rel32 census over delink units, symbols.txt func extents, retail XBE bytes
    import tomllib
    units = [u for u in tomllib.loads(UNITS.read_text(encoding="utf-8"))["unit"] if u.get("delink") is True]
    if args.unit and not any(u["name"] == args.unit for u in units):
        return refuse(f"no delink unit named {args.unit} in {UNITS}")
    funcs = _func_rows()
    vas_of = {}
    for va, _s, name in funcs:
        vas_of.setdefault(name, set()).add(va)
    applied = {int(f["entry"], 16) for f in load_jsonl(fj)}
    tot = [0, 0, 0]
    unit_line = None
    early, nofunc = [], 0
    for u in units:
        ts, te = u["text_start"], u["text_end"]
        obj = d / _unit_file(u["name"])
        try:
            _data, c = byte_compare.load_obj(obj)
        except byte_compare.Refuse as e:
            return refuse(str(e))
        texts = [s for s in c["sections"] if s["name"] == ".text"]
        if len(texts) != 1:
            return refuse(f"{obj.name}: {len(texts)} .text sections")
        sec = texts[0]
        for y in c["symbols"].values():
            v = vas_of.get(y["name"], set())
            if byte_compare.is_func(y) and y["section"] == sec["number"] and len(v) == 1 \
                    and y["value"] != next(iter(v)) - ts:
                return refuse(f"{obj.name}: {y['name']} at +0x{y['value']:X}, symbols.txt says "
                              f"+0x{next(iter(v)) - ts:X} (object offset != va - text_start)")
        roffs = {r["offset"] for r in sec["relocs"]}
        cross, intra = set(), set()
        for va, size, _name in funcs:
            if not ts <= va < te:
                continue
            if va not in applied:
                nofunc += 1
            else:
                bm = bodymax[va]
                if bm < va + size - 1:
                    early.append((va, size, _name, bm - va + 1))
            for s, t in _rel_targets(xb, x, va, size):
                if va <= t < va + size:
                    intra.add(s)
                elif ts <= t < te:  # the census: N = cross-function rel32 sites into the unit's own .text
                    cross.add(s)
        n, un = len(cross), sum(s - ts not in roffs for s in cross)
        for s in sorted(cross):  # T3 (2.6): name each unrelocated site
            if s - ts not in roffs:
                print(f"unrelocated rel32: {u['name']} site 0x{s:08X} (+0x{s - ts:X})")
        k = sum(s - ts in roffs for s in intra)
        tot = [tot[0] + un, tot[1] + n, tot[2] + k]
        if u["name"] == args.unit:
            unit_line = f"intra-section rel32 {u['name']}: {un} unrelocated of {n}; relocated inside own extent {k}"
    print(f"intra-section rel32: {tot[0]} unrelocated of {tot[1]}; relocated inside own extent {tot[2]}")
    if unit_line:
        print(unit_line)
    for va, size, name, at in sorted(early):
        print(f"body ends early: {name} va 0x{va:08X} size 0x{size:X} at +0x{at:X}")
    print(f"Ghidra body ends early: {len(early)} functions (no Ghidra function at entry: {nofunc})")
    # T3 (2.6): unrelocated census sites now fail; was `1 if unexpl or tot[2] else 0` (exit 0 with N unrelocated)
    return 1 if unexpl or tot[2] or tot[0] else 0


def main(argv):
    ap = argparse.ArgumentParser(prog="delink.py", description=__doc__.splitlines()[0])
    ap.add_argument("--fresh", action="store_true", help="(no subcommand) T5 ingest + ApplySymbols + delink every "
                    "delink=true unit to derived/delink/retail-<sha8>/, REL32 post-pass on every unit, hashes.txt, "
                    "objdiff report")
    ap.add_argument("--repro", action="store_true", help="(no subcommand) two --fresh runs; exit 0 iff hashes equal")
    ap.add_argument("--config", help="config path (default config/local_paths.json)")
    sub = ap.add_subparsers(dest="cmd")
    p = sub.add_parser("ingest", help="headless Ghidra ingest + export of the pinned retail XBE")
    p.add_argument("--fresh", action="store_true", help="delete and recreate the retail-<sha8> project")
    p.add_argument("--config", help="config path (default config/local_paths.json)")
    p = sub.add_parser("pick", help="apply the T6 selection rule; print candidate count and the lowest-VA pick")
    p.add_argument("--config", help="config path (default config/local_paths.json)")
    p = sub.add_parser("relocscan", help="list functions >= 64 B whose delinked object has no code relocation (C0034); "
                       "intra-section rel32 census; Ghidra bodies that end early")
    p.add_argument("--unit", help="units.toml unit name: also print its census line")
    p.add_argument("--config", help="config path (default config/local_paths.json)")
    p = sub.add_parser("one", help="delink one function from the existing project to a COFF obj + objdiff self-diff "
                       "in derived/delink_one/retail-<sha8>/ (outside the cut)")
    p.add_argument("va", help="retail function entry VA (hex)")
    p.add_argument("--config", help="config path (default config/local_paths.json)")
    args = ap.parse_args(argv)
    if args.cmd == "ingest" or (not args.cmd and not args.repro and args.fresh):
        # 5.4 T4: usage record only, stdout unchanged; lazy import (the gate's lock-imports control imports delink)
        import timing
        phase = _phase_run_dir().name
        with timing.step("delink_ingest" if args.cmd else "delink_fresh",
                         label=None if phase == ".run" else phase,
                         # 5.4 T21: shared gate marker
                         marker="delink.py " + " ".join(map(str, sys.argv[1:] if argv is None else argv))) as rec:
            rec.rc = ingest(args) if args.cmd else fresh(args)
        return rec.rc
    if args.cmd:
        return {"ingest": ingest, "pick": pick, "one": one, "relocscan": relocscan}[args.cmd](args)
    if args.repro:
        return repro(args)
    if args.fresh:
        return fresh(args)
    ap.error("a subcommand, --fresh or --repro is required")


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
