"""PDB 7.00 reader (T6, phase 1.2): MSF superblock and stream directory, info stream, DBI header, modules
(S_OBJNAME, compile record, procs) and publics; section contributions and section names (T8, phase 1.3).

CLI: pdb_info.py <pdb> [--exe <exe>] [--json] [--dump <dir under derived/>]
     pdb_info.py --self-test [--fallback]   (fixture builds via msvc_fixtures; else/forced a clang-cl+lld-link PDB)
Exit: 0 parsed (and the pair valid with --exe); 1 pair mismatch (field named) or a self-test check failed;
2 refusal (no such file, not MSF 7.00, unknown section contribution version, exe not a PE, --dump outside derived/).
Prints counts, names, versions and GUIDs only, never byte runs; --dump writes header.json, modules.json (with procs),
publics.json and contributions.json (version, records, sectionNames).
Layout (PHASE_PLAN Interfaces "PDB 7.0", R1.2-004): superblock magic `Microsoft C/C++ MSF 7.00`; stream 1 info
(Version, Signature, Age, GUID); stream 3 DBI (64-byte header, then the module info list, then the section
contribution substream: u32 version V60 0xEFFE0000+19970605 (28-byte records) or V2 +20140516 (32 bytes, +isectCoff)).
Section names: section-header stream = optional dbg header entry 5 (last DBI substream), 40-byte IMAGE_SECTION_HEADERs.
Contribution report: per-section totals, module index >= module count, procs whose [off, off+length) lies in no
contribution of their own module with section == proc seg. Self-test: fixture contributions == llvm-pdbutil
--section-contribs.
Compile record: S_COMPILE 0x0001, S_COMPILE2 0x1116 (_ST 0x1013), S_COMPILE3 0x113c. For COMPILE2/3 the flags dword
holds the language in its low byte and fLTCG = bit 10 (Interfaces "fLTCG"); S_COMPILE has no fLTCG (`n/a`); a module
without a compile record reports `missing`, never fLTCG 0. Proven on the fixture for S_COMPILE3 only (T6).
Pair check (Interfaces "DIA loadAndValidateDataFromPdb"): info GUID and DBI age against the exe's RSDS GUID and age;
RSDS carries no 32-bit signature (DIA uses `sig` only without a GUID), so the signature is reported, not compared.
Importable: parse_pdb(data) -> dict, validate(pdb, rsds) -> list of mismatched fields.
"""
import json
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

MSF7 = b"Microsoft C/C++ MSF 7.00\r\n\x1aDS\0\0\0"
S_COMPILE, S_COMPILE2, S_COMPILE2_ST, S_COMPILE3 = 0x0001, 0x1116, 0x1013, 0x113C
S_OBJNAME, S_OBJNAME_ST = 0x1101, 0x0009
PROCS = {0x1110: ("S_GPROC32", False), 0x110F: ("S_LPROC32", False), 0x1147: ("S_GPROC32_ID", False),
         0x1146: ("S_LPROC32_ID", False), 0x100B: ("S_GPROC32_ST", True), 0x100A: ("S_LPROC32_ST", True)}
PUBS = {0x110E: False, 0x1009: True}  # kind -> pascal name
COMPILE_KINDS = {S_COMPILE: "S_COMPILE", S_COMPILE2: "S_COMPILE2", S_COMPILE2_ST: "S_COMPILE2_ST",
                 S_COMPILE3: "S_COMPILE3"}
LANGS = {0: "c", 1: "c++", 2: "fortran", 3: "masm", 4: "pascal", 5: "basic", 6: "cobol", 7: "link", 8: "cvtres",
         9: "cvtpgd", 10: "c#", 11: "vb", 12: "ilasm", 13: "java", 14: "jscript", 15: "msil", 16: "hlsl"}
CPUS = {0x03: "80386", 0x04: "80486", 0x05: "pentium", 0x06: "pentium pro", 0x07: "pentium 3", 0xD0: "amd64"}
FLAG_BITS = ("ec", "noDbgInfo", "ltcg", "noDataAlign", "managed", "securityChecks", "hotPatch", "cvtcil",
             "msilModule", "sdl", "pgo", "exp")  # bits 8.. of the COMPILE2/3 flags dword (R1.2-004)
MACHINES = {0x14C: "i386", 0x8664: "amd64", 0x1C0: "arm", 0xAA64: "arm64"}
SC_VERSIONS = {0xEFFE0000 + 19970605: ("V60", 28), 0xEFFE0000 + 20140516: ("V2", 32)}  # -> name, record bytes


class NotPdb(Exception):
    pass


def _guid(b):
    d1, d2, d3 = struct.unpack_from("<IHH", b, 0)
    r = b[8:16].hex().upper()
    return f"{{{d1:08X}-{d2:04X}-{d3:04X}-{r[:4]}-{r[4:]}}}"


def _cstr(b, o, pascal=False):
    if pascal:
        n = b[o] if o < len(b) else 0
        return b[o + 1:o + 1 + n].decode("utf-8", "replace")
    e = b.find(b"\0", o)
    return b[o:e if e >= 0 else len(b)].decode("utf-8", "replace")


def read_streams(data):
    """MSF 7.00 -> list of stream bytes (None for a nil stream)."""
    if len(data) < 56 or data[:32] != MSF7:
        raise NotPdb("no MSF 7.00 superblock")
    bs, _, nblocks, dir_bytes, _, map_addr = struct.unpack_from("<6I", data, 32)
    if bs not in (512, 1024, 2048, 4096) or nblocks * bs > len(data) + bs:
        raise NotPdb(f"bad block size {bs} or block count {nblocks}")
    blk = lambda i: data[i * bs:(i + 1) * bs]
    ndir = -(-dir_bytes // bs)
    dir_blocks = struct.unpack_from(f"<{ndir}I", data, map_addr * bs)
    d = b"".join(blk(i) for i in dir_blocks)[:dir_bytes]
    n = struct.unpack_from("<I", d, 0)[0]
    sizes = struct.unpack_from(f"<{n}I", d, 4)
    pos, streams = 4 + 4 * n, []
    for sz in sizes:
        if sz == 0xFFFFFFFF:
            streams.append(None)
            continue
        k = -(-sz // bs)
        idx = struct.unpack_from(f"<{k}I", d, pos)
        pos += 4 * k
        streams.append(b"".join(blk(i) for i in idx)[:sz])
    return {"blockSize": bs, "blocks": nblocks, "streams": streams}


def iter_records(b, start=0, end=None):
    """CodeView symbol records: (kind, payload) with payload = bytes after the kind field."""
    o, end = start, len(b) if end is None else end
    while o + 4 <= end:
        ln, kind = struct.unpack_from("<HH", b, o)
        if ln < 2:
            break
        yield kind, b[o + 4:o + 2 + ln]
        o += 2 + ln


def parse_compile(kind, p):
    if kind == S_COMPILE:  # COMPILESYM: machine u8, flags[3] (language = low byte), version pascal string
        return {"kind": "S_COMPILE", "language": LANGS.get(p[1], str(p[1])), "machine": CPUS.get(p[0], f"{p[0]:#x}"),
                "flags": f"{p[1] | p[2] << 8 | p[3] << 16:#08x}", "flagNames": [], "fLTCG": "n/a", "fe": None,
                "be": None, "version": _cstr(p, 4, True)}
    flags, machine = struct.unpack_from("<IH", p, 0)
    if kind == S_COMPILE3:
        fe, be, vo = struct.unpack_from("<4H", p, 6), struct.unpack_from("<4H", p, 14), 22
    else:
        fe, be, vo = struct.unpack_from("<3H", p, 6), struct.unpack_from("<3H", p, 12), 18
    return {"kind": COMPILE_KINDS[kind], "language": LANGS.get(flags & 0xFF, str(flags & 0xFF)),
            "machine": CPUS.get(machine, f"{machine:#x}"), "flags": f"{flags & ~0xFF:#x}",
            "flagNames": [nm for i, nm in enumerate(FLAG_BITS) if flags >> (8 + i) & 1],
            "fLTCG": flags >> 10 & 1, "fe": ".".join(map(str, fe)), "be": ".".join(map(str, be)),
            "version": _cstr(p, vo, kind == S_COMPILE2_ST)}


def parse_module_symbols(b):
    """Symbol bytes of one module (signature dword already skipped) -> objname, compile, procs."""
    out = {"objname": None, "compile": "missing", "procs": []}
    for kind, p in iter_records(b):
        if kind in (S_OBJNAME, S_OBJNAME_ST) and out["objname"] is None:
            out["objname"] = _cstr(p, 4, kind == S_OBJNAME_ST)
        elif kind in COMPILE_KINDS and out["compile"] == "missing":
            out["compile"] = parse_compile(kind, p)
        elif kind in PROCS:
            ln, off, seg = struct.unpack_from("<I", p, 12)[0], struct.unpack_from("<I", p, 28)[0], \
                struct.unpack_from("<H", p, 32)[0]
            out["procs"].append({"name": _cstr(p, 35, PROCS[kind][1]), "kind": PROCS[kind][0], "seg": seg,
                                 "off": off, "length": ln})
    return out


def parse_pdb(data):
    msf = read_streams(data)
    st = msf["streams"]
    if len(st) < 2 or st[1] is None or len(st[1]) < 28:
        raise NotPdb("no info stream")
    ver, sig, age = struct.unpack_from("<3I", st[1], 0)
    res = {"blockSize": msf["blockSize"], "blocks": msf["blocks"], "streams": len(st),
           "info": {"version": ver, "signature": sig, "age": age, "guid": _guid(st[1][12:28])},
           "dbi": None, "modules": [], "publics": [], "contributions": None, "sectionNames": {}}
    dbi = st[3] if len(st) > 3 else None
    if not dbi or len(dbi) < 64:
        return res
    (vsig, vhdr, dage, _, bno, _, dllv, symrec, dllr, modi_sz) = struct.unpack_from("<iIIHHHHHHi", dbi, 0)
    machine = struct.unpack_from("<H", dbi, 58)[0]  # flags u16 @56, machine u16 @58, padding u32 @60
    res["dbi"] = {"versionSignature": vsig, "versionHeader": vhdr, "age": dage, "buildNumber": bno,
                  "build": f"{bno >> 8 & 0x7F}.{bno & 0xFF}" if bno & 0x8000 else None,
                  "pdbDllVersion": dllv, "pdbDllRbld": dllr, "machine": MACHINES.get(machine, f"{machine:#x}"),
                  "symRecordStream": symrec}
    o, end = 64, 64 + modi_sz
    while o + 64 <= end:
        sn, symsz = struct.unpack_from("<HI", dbi, o + 34)
        name = _cstr(dbi, o + 64)
        o2 = o + 64 + len(name.encode()) + 1
        obj = _cstr(dbi, o2)
        o = (o2 + len(obj.encode()) + 1 + 3) & ~3
        m = {"index": len(res["modules"]), "name": name, "obj": obj, "stream": None if sn == 0xFFFF else sn}
        sb = st[sn] if sn != 0xFFFF and sn < len(st) else None
        m.update(parse_module_symbols(sb[4:symsz] if sb else b""))
        res["modules"].append(m)
    # substreams after modi (DBI header i32 sizes): SC @28, section map @32, file info @36, type server map @40,
    # EC @52, optional dbg header @48 (last); order modi, SC, secmap, fileinfo, tsmap, EC, dbg header
    sc_sz, secmap_sz, src_sz, ts_sz, _, dbg_sz, ec_sz = struct.unpack_from("<iiiiIii", dbi, 28)
    o = 64 + modi_sz
    if sc_sz >= 4:
        scv = struct.unpack_from("<I", dbi, o)[0]
        rec = SC_VERSIONS.get(scv)
        if rec is None:
            raise NotPdb(f"unknown section contribution version {scv:#x}")
        res["contributions"] = {"version": scv, "versionName": rec[0], "records": []}
        for r in range(o + 4, o + sc_sz - rec[1] + 1, rec[1]):
            sec, off, size, ch, mod, dcrc, rcrc = struct.unpack_from("<H2xiiIH2xII", dbi, r)
            c = {"section": sec, "offset": off, "size": size, "characteristics": ch, "module": mod,
                 "dataCrc": dcrc, "relocCrc": rcrc}
            if rec[1] == 32:
                c["isectCoff"] = struct.unpack_from("<I", dbi, r + 28)[0]
            res["contributions"]["records"].append(c)
    o += sc_sz + secmap_sz + src_sz + ts_sz + ec_sz
    if dbg_sz >= 12 and o + dbg_sz <= len(dbi):
        shs = struct.unpack_from("<H", dbi, o + 10)[0]  # optional dbg header entry 5: section header stream
        if shs != 0xFFFF and shs < len(st) and st[shs]:
            b = st[shs]
            res["sectionNames"] = {i // 40 + 1: b[i:i + 8].split(b"\0")[0].decode("latin-1")
                                   for i in range(0, len(b) - 39, 40)}
    if symrec != 0xFFFF and symrec < len(st) and st[symrec]:
        for kind, p in iter_records(st[symrec]):
            if kind in PUBS:
                _, off, seg = struct.unpack_from("<IIH", p, 0)
                res["publics"].append({"name": _cstr(p, 10, PUBS[kind]), "seg": seg, "off": off})
    return res


def validate(pdb, rsds):
    """DIA-style pair check; [] when valid, else the mismatched field names."""
    if rsds is None:
        return ["rsds (exe has no RSDS entry)"]
    bad = []
    if pdb["info"]["guid"] != rsds["guid"].upper():
        bad.append(f"guid (pdb {pdb['info']['guid']} exe {rsds['guid']})")
    age = pdb["dbi"]["age"] if pdb["dbi"] else pdb["info"]["age"]
    if age != rsds["age"]:
        bad.append(f"age (pdb DBI {age} exe {rsds['age']})")
    return bad


def summary(p):
    """Header + per-module facts without proc/public lists (stdout/--json)."""
    s = {k: p[k] for k in ("blockSize", "blocks", "streams", "info", "dbi")}
    s["modules"] = [{k: m[k] for k in ("index", "name", "obj", "objname", "compile")} | {"procs": len(m["procs"])}
                    for m in p["modules"]]
    s["publics"] = len(p["publics"])
    c = p["contributions"]
    s["contributions"] = c and {"version": c["version"], "versionName": c["versionName"], "count": len(c["records"])}
    return s


def contrib_stats(p):
    """Per-section totals, module indices out of range, procs outside every same-module contribution."""
    recs, names = p["contributions"]["records"], p["sectionNames"]
    totals = {}
    for c in recs:
        t = totals.setdefault(c["section"], [0, 0])
        t[0] += 1
        t[1] += c["size"]
    bad = sum(c["module"] >= len(p["modules"]) for c in recs)
    by = {}
    for c in recs:
        by.setdefault((c["module"], c["section"]), []).append(c)
    inside = lambda cs, x: any(c["offset"] <= x["off"] and x["off"] + x["length"] <= c["offset"] + c["size"]
                               for c in cs)
    outside = []
    for m in p["modules"]:
        for x in m["procs"]:
            if not inside(by.get((m["index"], x["seg"]), []), x):
                other = sorted({k[0] for k, cs in by.items() if k[1] == x["seg"] and inside(cs, x)})
                outside.append({"module": m["index"], "moduleName": m["name"], "proc": x["name"], "kind": x["kind"],
                                "section": names.get(x["seg"], str(x["seg"])), "inOtherModule": other})
    return totals, bad, outside


def _print(path, p):
    i, d = p["info"], p["dbi"]
    print(f"file {path}")
    print(f"MSF 7.00 block size {p['blockSize']} blocks {p['blocks']} streams {p['streams']}")
    print(f"info: version {i['version']} signature {i['signature']} age {i['age']} guid {i['guid']}")
    if d is None:
        print("DBI: absent")
        return
    print(f"DBI: version header {d['versionHeader']} age {d['age']} build {d['build'] or 'old-format'} "
          f"(raw {d['buildNumber']:#06x}) PdbDllVersion {d['pdbDllVersion']} PdbDllRbld {d['pdbDllRbld']} "
          f"machine {d['machine']}")
    print(f"modules {len(p['modules'])}, procs {sum(len(m['procs']) for m in p['modules'])}, "
          f"publics {len(p['publics'])}")
    for m in p["modules"]:
        c = m["compile"]
        cs = "compile missing" if c == "missing" else (
            f"{c['kind']} {c['language']} fLTCG {c['fLTCG']} fe {c['fe']} be {c['be']} '{c['version']}'")
        print(f"  {m['index']:4} {m['name']}  [{cs}] procs {len(m['procs'])}")
    sc = p["contributions"]
    if sc is None:
        print("section contributions: absent")
        return
    totals, bad, outside = contrib_stats(p)
    print(f"section contributions: version {sc['versionName']} ({sc['version']:#x}) records {len(sc['records'])}, "
          f"section names {len(p['sectionNames'])}")
    for sec in sorted(totals):
        print(f"  section {sec:3} {p['sectionNames'].get(sec, '?'):8} contributions {totals[sec][0]:6} "
              f"bytes {totals[sec][1]}")
    print(f"contributions with module index >= {len(p['modules'])}: {bad}")
    print(f"procs outside any same-module contribution: {len(outside)}")
    if outside:
        agg = {}
        for x in outside:
            k = (x["kind"], x["section"], "in another module's contribution" if x["inOtherModule"] else "in none")
            agg[k] = agg.get(k, 0) + 1
        for k in sorted(agg):
            print(f"  {agg[k]:6} {k[0]} section {k[1]} {k[2]}")
        mods = {}
        for x in outside:
            mods[x["moduleName"]] = mods.get(x["moduleName"], 0) + 1
        for nm, n in sorted(mods.items(), key=lambda kv: (-kv[1], kv[0]))[:10]:
            print(f"  {n:6} module {nm}")


def main(argv):
    if "--help" in argv or "-h" in argv:
        print(__doc__)
        return 0
    if "--self-test" in argv:
        return self_test("--fallback" in argv)
    opt = lambda f: argv[argv.index(f) + 1] if f in argv and argv.index(f) + 1 < len(argv) else None
    vals = {opt("--exe"), opt("--dump")}
    pos = [a for a in argv if not a.startswith("--") and a not in vals]
    if len(pos) != 1 or ("--exe" in argv and not opt("--exe")) or ("--dump" in argv and not opt("--dump")):
        print("usage: pdb_info.py <pdb> [--exe <exe>] [--json] [--dump <dir>] | --self-test", file=sys.stderr)
        return 2
    path = Path(pos[0])
    if not path.is_file():
        print(f"pdb_info: no such file: {path}", file=sys.stderr)
        return 2
    dump = None
    if opt("--dump"):
        dump = Path(opt("--dump")).resolve()
        if not dump.is_relative_to((REPO / "derived").resolve()):
            print(f"pdb_info: --dump must be under {REPO / 'derived'}: {dump}", file=sys.stderr)
            return 2
    try:
        p = parse_pdb(path.read_bytes())
    except (NotPdb, struct.error) as e:
        print(f"pdb_info: not an MSF 7.00 PDB: {path}: {e}", file=sys.stderr)
        return 2
    if "--json" in argv:
        print(json.dumps(summary(p) | {"file": str(path)}, indent=1, sort_keys=True))
    else:
        _print(path, p)
    if dump:
        dump.mkdir(parents=True, exist_ok=True)
        for nm, obj in (("header", summary(p) | {"file": str(path)}), ("modules", p["modules"]),
                        ("publics", p["publics"])):
            (dump / f"{nm}.json").write_text(json.dumps(obj, indent=1, sort_keys=True) + "\n", encoding="utf-8")
        sc = p["contributions"]
        (dump / "contributions.json").write_text(json.dumps(
            {"version": sc and sc["version"], "versionName": sc and sc["versionName"],
             "contributions": sc["records"] if sc else [], "sectionNames": p["sectionNames"]},
            indent=1, sort_keys=True) + "\n", encoding="utf-8")
        print(f"dump: {dump} (header.json, modules.json, publics.json, contributions.json)")
    if opt("--exe"):
        import pe_info
        exe = Path(opt("--exe"))
        if not exe.is_file():
            print(f"pdb_info: no such file: {exe}", file=sys.stderr)
            return 2
        try:
            rsds = pe_info.parse_pe(exe.read_bytes())["rsds"]
        except (pe_info.NotPe, struct.error) as e:
            print(f"pdb_info: exe not a PE: {exe}: {e}", file=sys.stderr)
            return 2
        bad = validate(p, rsds)
        print(f"pair {exe.name} / {path.name}: {'VALID' if not bad else 'MISMATCH ' + '; '.join(bad)}")
        return 1 if bad else 0
    return 0


# ---- self-test ----

def _rec(kind, payload):
    payload += b"\0" * (-(len(payload) + 4) % 4)
    return struct.pack("<HH", len(payload) + 2, kind) + payload


def _proc(name, kind=0x1110, pascal=False):
    nm = bytes([len(name)]) + name.encode() if pascal else name.encode() + b"\0"
    return _rec(kind, struct.pack("<IIIIIIIIHB", 0, 0, 0, 7, 0, 6, 0, 0x20, 1, 0) + nm)


def _synthetic_checks(want):
    """Symbol-record level: missing compile record, S_COMPILE2 (+_ST) fLTCG bit, S_COMPILE n/a."""
    objn = _rec(S_OBJNAME, struct.pack("<I", 0) + b"syn.obj\0")
    m = parse_module_symbols(objn + _proc("syn_fn"))
    want("missing compile record -> 'missing'", (m["compile"], m["objname"], [p["name"] for p in m["procs"]]),
         ("missing", "syn.obj", ["syn_fn"]))
    c2 = struct.pack("<IH6H", 1 | 1 << 10, 0x07, 13, 10, 3077, 13, 10, 3077)
    m = parse_module_symbols(objn + _rec(S_COMPILE2, c2 + b"syn cl\0\0") + _proc("a"))
    c = m["compile"]
    want("S_COMPILE2 ltcg", (c["kind"], c["language"], c["fLTCG"], c["fe"], c["be"], c["version"], c["flagNames"]),
         ("S_COMPILE2", "c++", 1, "13.10.3077", "13.10.3077", "syn cl", ["ltcg"]))
    m = parse_module_symbols(_rec(S_COMPILE2_ST, struct.pack("<IH6H", 0, 0x07, 13, 0, 9466, 13, 0, 9466)
                                  + b"\x06syn st") + _proc("b", 0x100B, True))
    want("S_COMPILE2_ST plain + S_GPROC32_ST", (m["compile"]["kind"], m["compile"]["fLTCG"], m["compile"]["version"],
                                                 m["procs"][0]["name"], m["procs"][0]["length"]),
         ("S_COMPILE2_ST", 0, "syn st", "b", 7))
    m = parse_module_symbols(_rec(S_COMPILE, bytes([0x03, 0x00, 0x00, 0x00]) + b"\x04old!"))
    want("S_COMPILE fLTCG n/a", (m["compile"]["kind"], m["compile"]["fLTCG"], m["compile"]["version"]),
         ("S_COMPILE", "n/a", "old!"))


def _pdbutil(pdb):
    """llvm-pdbutil dump --summary --modules --symbols -> {guid, age, signature, modules, procs} or an error str."""
    tool = shutil.which("llvm-pdbutil")
    if tool is None:
        return "llvm-pdbutil not on PATH"
    r = subprocess.run([tool, "dump", "--summary", "--modules", "--symbols", str(pdb)], capture_output=True,
                       text=True, errors="replace")
    if r.returncode != 0:
        return f"llvm-pdbutil exit {r.returncode}"
    out, sect, mod = {"modules": 0, "procs": {}}, None, None
    for line in r.stdout.splitlines():
        s = line.strip()
        if s in ("Summary", "Modules", "Symbols"):
            sect = s
            continue
        if sect == "Summary":
            for key, rx in (("guid", r"GUID: (\{[0-9A-Fa-f-]+\})"), ("age", r"Age: (\d+)"),
                            ("signature", r"Signature: (\d+)")):
                g = re.match(rx, s)
                if g:
                    out[key] = g.group(1).upper() if key == "guid" else int(g.group(1))
        elif sect == "Modules" and re.match(r"Mod \d+ \|", s):
            out["modules"] += 1
        elif sect == "Symbols":
            g = re.match(r"Mod (\d+) \|", s)
            if g:
                mod = int(g.group(1))
                out["procs"].setdefault(mod, [])
                continue
            g = re.match(r"\d+ \| S_[GL]PROC32(?:_ID)? \[size = \d+\] `(.*)`$", s)
            if g and mod is not None:
                out["procs"][mod].append(g.group(1))
    if not all(k in out for k in ("guid", "age", "signature")):
        return "llvm-pdbutil summary not parsed"
    return out


SCN = {"TYPE_NO_PAD": 0x8, "CNT_CODE": 0x20, "CNT_INITIALIZED_DATA": 0x40, "CNT_UNINITIALIZED_DATA": 0x80,
       "LNK_OTHER": 0x100, "LNK_INFO": 0x200, "LNK_REMOVE": 0x800, "LNK_COMDAT": 0x1000, "GPREL": 0x8000,
       "MEM_PURGEABLE": 0x20000, "MEM_16BIT": 0x20000, "MEM_LOCKED": 0x40000, "MEM_PRELOAD": 0x80000,
       "LNK_NRELOC_OVFL": 0x1000000, "MEM_DISCARDABLE": 0x2000000, "MEM_NOT_CACHED": 0x4000000,
       "MEM_NOT_PAGED": 0x8000000, "MEM_SHARED": 0x10000000, "MEM_EXECUTE": 0x20000000, "MEM_READ": 0x40000000,
       "MEM_WRITE": 0x80000000} | {f"ALIGN_{1 << i}BYTES": (i + 1) << 20 for i in range(14)}


def _pdbutil_contribs(pdb):
    """llvm-pdbutil dump --section-contribs -> list of records (file order) or an error str.
    Record line `SC[<name>] | mod = M, SSSS:OFF, size = N, data crc = D, reloc crc = R` (decimal), then
    continuation lines of `IMAGE_SCN_*` names joined by ` | ` (characteristics as names, not hex)."""
    tool = shutil.which("llvm-pdbutil")
    if tool is None:
        return "llvm-pdbutil not on PATH"
    r = subprocess.run([tool, "dump", "--section-contribs", str(pdb)], capture_output=True, text=True,
                       errors="replace")
    if r.returncode != 0:
        return f"llvm-pdbutil exit {r.returncode}"
    out = []
    for line in r.stdout.splitlines():
        s = line.strip()
        g = re.search(r"\| mod = (\d+), (\d+):(-?\d+), size = (-?\d+), data crc = (\d+), reloc crc = (\d+)$", s)
        if s.startswith("SC[") and g:
            mod, sec, off, size, dcrc, rcrc = map(int, g.groups())
            out.append({"section": sec, "offset": off, "size": size, "characteristics": 0, "module": mod,
                        "dataCrc": dcrc, "relocCrc": rcrc})
        elif s.startswith("IMAGE_SCN_") and out:
            for f in filter(None, (t.strip() for t in s.split("|"))):
                if f[10:] not in SCN:
                    return f"llvm-pdbutil: unknown flag {f}"
                out[-1]["characteristics"] |= SCN[f[10:]]
    return out or "llvm-pdbutil section contributions not parsed"


def _structural(want, label, pdb_path, p):
    """Module count, proc names per module, GUID/age/signature == llvm-pdbutil."""
    u = _pdbutil(pdb_path)
    if isinstance(u, str):
        want(f"{label}: llvm-pdbutil", u, "ok")
        return
    mine = {m["index"]: [x["name"] for x in m["procs"]] for m in p["modules"] if m["procs"]}
    theirs = {k: v for k, v in u["procs"].items() if v}
    want(f"{label}: module count == llvm-pdbutil", len(p["modules"]), u["modules"])
    want(f"{label}: proc names per module == llvm-pdbutil", mine, theirs)
    want(f"{label}: guid/age/signature == llvm-pdbutil", (p["info"]["guid"], p["info"]["age"], p["info"]["signature"]),
         (u["guid"], u["age"], u["signature"]))
    print(f"  {label}: modules {len(p['modules'])} procs {sum(map(len, mine.values()))} publics {len(p['publics'])} "
          f"guid {p['info']['guid']} age {p['info']['age']}; llvm-pdbutil agrees: "
          f"{(len(p['modules']), mine) == (u['modules'], theirs)}")


def _pairs(want, pairs):
    """pairs: {variant: (exe, pdb)}; matched pairs valid, crossed pairs fail, via the CLI exit code."""
    me = [sys.executable, str(Path(__file__).resolve())]
    names = list(pairs)
    for a in names:
        for b in names:
            r = subprocess.run(me + [str(pairs[b][1]), "--exe", str(pairs[a][0])], capture_output=True, text=True,
                               errors="replace")
            want(f"pair exe {a} / pdb {b}: exit", r.returncode, 0 if a == b else 1)
            if a != b:
                want(f"pair exe {a} / pdb {b}: names guid", "MISMATCH guid" in r.stdout, True)


def _check_fixtures(want, built):
    pairs = {}
    for name, exe in built["variants"].items():
        pdb = Path(exe).with_suffix(".pdb")
        want(f"{name}: pdb beside exe", pdb.is_file(), True)
        if not pdb.is_file():
            continue
        pairs[name] = (Path(exe), pdb)
        p = parse_pdb(pdb.read_bytes())
        d = p["dbi"]
        print(f"  {name}: info version {p['info']['version']}; DBI version header {d['versionHeader']} build "
              f"{d['build']} PdbDllVersion {d['pdbDllVersion']} PdbDllRbld {d['pdbDllRbld']} machine {d['machine']}")
        for src in ("fix_c", "fix_cpp"):
            mods = [m for m in p["modules"] if Path(m["name"].replace("\\", "/")).stem.lower() == src]
            want(f"{name}: one {src} module", len(mods), 1)
            if not mods:
                continue
            c = mods[0]["compile"]
            want(f"{name}: {src} compile record present", c != "missing", True)
            if c == "missing":
                continue
            want(f"{name}: {src} fLTCG", c["fLTCG"], 1 if name == "ltcg" else 0)
            print(f"  {name}: {src} {c['kind']} {c['language']} fLTCG {c['fLTCG']} flags {c['flags']} "
                  f"{c['flagNames']} fe {c['fe']} be {c['be']}")
        others = sorted({(m["compile"] if m["compile"] == "missing" else
                          f"{m['compile']['kind']} fLTCG {m['compile']['fLTCG']}") for m in p["modules"]
                         if Path(m["name"].replace("\\", "/")).stem.lower() not in ("fix_c", "fix_cpp")})
        print(f"  {name}: other modules (reported, not asserted): {others}")
        _structural(want, name, pdb, p)
        idx = {m["index"] for m in p["modules"]
               if Path(m["name"].replace("\\", "/")).stem.lower() in ("fix_c", "fix_cpp")}
        u = _pdbutil_contribs(pdb)
        if isinstance(u, str):
            want(f"{name}: llvm-pdbutil section contribs", u, "ok")
            continue
        keys = ("section", "offset", "size", "characteristics", "module", "dataCrc", "relocCrc")
        mine = [{k: c[k] for k in keys} for c in p["contributions"]["records"] if c["module"] in idx]
        theirs = [c for c in u if c["module"] in idx]
        want(f"{name}: fix_c/fix_cpp section contribs == llvm-pdbutil", mine, theirs)
        want(f"{name}: section contrib count == llvm-pdbutil", len(p["contributions"]["records"]), len(u))
        print(f"  {name}: section contribs version {p['contributions']['versionName']} records "
              f"{len(p['contributions']['records'])}; fix_c/fix_cpp {len(mine)}; llvm-pdbutil agrees: {mine == theirs}")
    _pairs(want, pairs)


def _check_fallback(want):
    """clang-cl /Zi + lld-link /DEBUG: two tiny C exes; structural and pair checks, no /GL check."""
    cc, ld = shutil.which("clang-cl"), shutil.which("lld-link")
    if cc is None or ld is None:
        want("fallback: clang-cl and lld-link on PATH", (cc, ld), "both")
        return
    tmp = REPO / ".run" / "1.2" / "pdb_fallback"
    tmp.mkdir(parents=True, exist_ok=True)
    try:
        pairs = {}
        for name, body in (("a", "static int sq(int x){return x*x;}\nint fa(int v){return sq(v)+1;}\n"),
                           ("b", "int fb(int v){return v*3;}\n")):
            src = tmp / f"{name}.c"
            src.write_text(body + f"int __cdecl entry(void){{return f{name}(2);}}\n", encoding="ascii")
            cmds = ([cc, "--target=i686-pc-windows-msvc", "/nologo", "/Zi", "/O1", "/GS-", "/c", str(src),
                     f"/Fo{tmp / (name + '.obj')}"],
                    [ld, "/nologo", "/DEBUG", "/MACHINE:X86", "/NODEFAULTLIB", "/ENTRY:entry",
                     "/SUBSYSTEM:CONSOLE", f"/OUT:{tmp / (name + '.exe')}", f"/PDB:{tmp / (name + '.pdb')}",
                     str(tmp / (name + ".obj"))])
            ok = True
            for cmd in cmds:
                r = subprocess.run(cmd, capture_output=True, text=True, errors="replace")
                want(f"fallback {name}: {Path(cmd[0]).name} exit", r.returncode, 0)
                ok = ok and r.returncode == 0
            if not ok:
                continue
            pdb = tmp / f"{name}.pdb"
            p = parse_pdb(pdb.read_bytes())
            pairs[name] = (tmp / f"{name}.exe", pdb)
            ours = [m for m in p["modules"] if Path(m["name"].replace("\\", "/")).stem == name]
            want(f"fallback {name}: one module for {name}.c", len(ours), 1)
            if ours:
                c = ours[0]["compile"]
                print(f"  fallback {name}: compile {c if c == 'missing' else c['kind'] + ' fLTCG ' + str(c['fLTCG'])}"
                      f" (clang-cl; reported, not asserted)")
            _structural(want, f"fallback {name}", pdb, p)
        _pairs(want, pairs)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    print("  /GL check: not run (no local MSVC)")
    print("  section contribs vs llvm-pdbutil: not run (fallback)")


def self_test(force_fallback=False):
    import msvc_fixtures
    fails = []

    def want(label, got, exp):
        if got != exp:
            fails.append(f"{label}: got {got!r} want {exp!r}")
    _synthetic_checks(want)
    tmp = REPO / ".run" / "1.2" / "pdb_selftest"
    tmp.mkdir(parents=True, exist_ok=True)
    try:
        me = [sys.executable, str(Path(__file__).resolve())]
        junk = tmp / "not_a_pdb.pdb"
        junk.write_bytes(b"\0" * 0x100)
        for label, cmd, code in (("not a PDB", me + [str(junk)], 2), ("missing file", me + [str(tmp / "none.pdb")], 2),
                                 ("--dump outside derived/", me + [str(junk), "--dump", str(tmp)], 2)):
            r = subprocess.run(cmd, capture_output=True, text=True, errors="replace")
            want(f"exit code: {label}", r.returncode, code)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    print(f"  synthetic: {len(fails)} failure(s)")
    if msvc_fixtures.find_toolset() is not None and not force_fallback:
        try:
            _check_fixtures(want, msvc_fixtures.build())
        except msvc_fixtures.BuildError as e:
            fails.append(f"fixture build: {e}")
    else:
        _check_fallback(want)
    for f in fails:
        print(f"  FAIL {f}")
    print(f"pdb_info self-test: {'OK' if not fails else 'FAIL'} — {len(fails)} failure(s)")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
