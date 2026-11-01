"""PE reader (T5, phase 1.2): COFF/optional header facts, sections, .reloc, CodeView RSDS, decoded Rich header.

CLI: pe_info.py <exe> [--json]
     pe_info.py --self-test   (synthetic PEs; then fixture builds via msvc_fixtures, or python.exe as fallback)
Exit: 0 parsed; 1 a self-test check failed; 2 refusal (no such file, not a PE). Absent Rich / .reloc / debug
directory are reported, never errors. Prints header fields, names, versions and GUIDs only, never byte runs.
Rich layout: `Rich` + XOR key ends the table; walking back, dwords XOR key until `DanS`; 3 padding dwords; then
(comp.id, count) pairs, comp.id = (prodid << 16) | build (PHASE_PLAN Interfaces "Rich header", R1.2-004;
layout proven on the fixture build, T5). Stored key is checked against the recomputed checksum.
Importable: parse_pe(data) -> dict, parse_rich(data, pe_off) -> dict | None, PRODUCTS.
"""
import json
import re
import struct
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

SRC_RICHPRINT = "R1.2-004 (richprint)"
SRC_MODERN = "R1.2-005 (richprint comp_id.txt, VS2015 block)"
SRC_LTCG_FIX = "R1.2-005 + T5 fixture (cl 19.51.36260)"
SRC_LTCG_INTERP = "R1.2-005 (richprint: interpolated)"
PRODUCTS = {  # prodid -> (name, source)
    0x0000: ("Unknown", SRC_MODERN), 0x0001: ("Import0", SRC_MODERN),
    # VC 7.0 (build 9466)
    0x001c: ("Utc13_C (VC7.0)", SRC_RICHPRINT), 0x001d: ("Utc13_CPP (VC7.0)", SRC_RICHPRINT),
    0x0040: ("Masm (VC7.0)", SRC_RICHPRINT), 0x003d: ("Linker (VC7.0)", SRC_RICHPRINT),
    0x0045: ("Cvtres (VC7.0)", SRC_RICHPRINT), 0x003f: ("Export (VC7.0)", SRC_RICHPRINT),
    0x0019: ("Implib (VC7.0)", SRC_RICHPRINT),
    # VC 7.1 (build 3077)
    0x005f: ("Utc1310_C (VC7.1)", SRC_RICHPRINT), 0x0060: ("Utc1310_CPP (VC7.1)", SRC_RICHPRINT),
    0x000f: ("Masm (VC7.1)", SRC_RICHPRINT), 0x005a: ("Linker (VC7.1)", SRC_RICHPRINT),
    0x005e: ("Cvtres (VC7.1)", SRC_RICHPRINT), 0x005c: ("Export (VC7.1)", SRC_RICHPRINT),
    0x005d: ("Implib (VC7.1)", SRC_RICHPRINT),
    # VS2015+
    0x00ff: ("Cvtres1400", SRC_MODERN), 0x0100: ("Export1400", SRC_MODERN),
    0x0101: ("Implib1400", SRC_MODERN), 0x0102: ("Linker1400", SRC_MODERN), 0x0103: ("Masm1400", SRC_MODERN),
    0x0104: ("Utc1400_C", SRC_MODERN), 0x0105: ("Utc1400_CPP", SRC_MODERN),
    # fixture /GL build: both fix_c.c and fix_cpp.cpp counted under 0x0108 (count 2); 0x0109 not observed
    0x0108: ("Utc1400_LTCG_C", SRC_LTCG_FIX), 0x0109: ("Utc1400_LTCG_CPP", SRC_LTCG_INTERP),
}
MACHINES = {0x14c: "i386", 0x8664: "amd64", 0x1c0: "arm", 0xaa64: "arm64"}
SUBSYSTEMS = {1: "native", 2: "windows_gui", 3: "windows_cui", 14: "xbox"}
DEBUG_TYPES = {1: "COFF", 2: "CodeView", 4: "Misc", 12: "VCFeature", 13: "POGO", 14: "ILTCG", 16: "Repro",
               20: "ExDllCharacteristics"}


class NotPe(Exception):
    pass


def _u32(d, o):
    return struct.unpack_from("<I", d, o)[0]


def _rol(v, n):
    n &= 31
    return ((v << n) | (v >> (32 - n))) & 0xFFFFFFFF if n else v


def rich_checksum(data, dans_off, pairs):
    """Rich key: dans_off + rol(byte_i, i) over the DOS header/stub (e_lfanew bytes skipped) + rol(compid, count)."""
    c = dans_off
    for i in range(dans_off):
        if 0x3C <= i < 0x40:
            continue
        c = (c + _rol(data[i], i)) & 0xFFFFFFFF
    for compid, count in pairs:
        c = (c + _rol(compid, count)) & 0xFFFFFFFF
    return c


def parse_rich(data, pe_off):
    """Decoded Rich header from the area before the PE signature, or None if absent/undecodable."""
    rich = data.rfind(b"Rich", 0x40, pe_off)
    if rich < 0 or rich % 4 or rich + 8 > len(data):
        return None
    key = _u32(data, rich + 4)
    dans = None
    for o in range(rich - 4, 0x3C, -4):
        if _u32(data, o) ^ key == 0x536E6144:  # "DanS"
            dans = o
            break
    if dans is None:
        return None
    pad = [_u32(data, dans + 4 * i) ^ key for i in (1, 2, 3)]
    pairs = [(_u32(data, o) ^ key, _u32(data, o + 4) ^ key) for o in range(dans + 16, rich, 8)]
    entries = []
    for compid, count in pairs:
        pid, build = compid >> 16, compid & 0xFFFF
        name, src = PRODUCTS.get(pid, (None, None))
        entries.append({"prodid": pid, "build": build, "count": count, "compid": f"{compid:08x}",
                        "product": name, "source": src})
    calc = rich_checksum(data, dans, pairs)
    return {"offset": dans, "key": f"{key:08x}", "checksum": f"{calc:08x}", "checksumMatch": calc == key,
            "paddingZero": pad == [0, 0, 0], "entries": entries}


def _guid(b):
    d1, d2, d3 = struct.unpack_from("<IHH", b, 0)
    r = b[8:16].hex().upper()
    return f"{{{d1:08X}-{d2:04X}-{d3:04X}-{r[:4]}-{r[4:]}}}"


def parse_pe(data):
    if len(data) < 0x40 or data[:2] != b"MZ":
        raise NotPe("no MZ signature")
    pe = _u32(data, 0x3C)
    if pe + 24 > len(data) or data[pe:pe + 4] != b"PE\0\0":
        raise NotPe("no PE signature at e_lfanew")
    machine, nsec, tds, _, _, opt_size, chars = struct.unpack_from("<HHIIIHH", data, pe + 4)
    opt = pe + 24
    if opt + 2 > len(data):
        raise NotPe("truncated optional header")
    magic = struct.unpack_from("<H", data, opt)[0]
    if magic == 0x10B:
        image_base, dd_count_off = _u32(data, opt + 28), 92
    elif magic == 0x20B:
        image_base, dd_count_off = struct.unpack_from("<Q", data, opt + 24)[0], 108
    else:
        raise NotPe(f"unknown optional header magic {magic:#x}")
    if opt + dd_count_off + 4 > len(data):
        raise NotPe("truncated optional header")
    subsystem = struct.unpack_from("<H", data, opt + 68)[0]
    ndd = min(_u32(data, opt + dd_count_off), 16)
    dirs = [struct.unpack_from("<II", data, opt + dd_count_off + 4 + 8 * i) for i in range(ndd)]
    sh = opt + opt_size
    secs = []
    for i in range(nsec):
        o = sh + 40 * i
        if o + 40 > len(data):
            raise NotPe("truncated section table")
        name = data[o:o + 8].rstrip(b"\0").decode("latin-1")
        vsize, va, rsize, rptr = struct.unpack_from("<IIII", data, o + 8)
        secs.append({"name": name, "va": va, "vsize": vsize, "rawsize": rsize, "rawptr": rptr})

    def rva_off(rva):
        for s in secs:
            if s["va"] <= rva < s["va"] + max(s["vsize"], s["rawsize"]) and rva - s["va"] < s["rawsize"]:
                return s["rawptr"] + rva - s["va"]
        return None

    reloc_dir = dirs[5] if ndd > 5 else (0, 0)
    debug = []
    if ndd > 6 and dirs[6][0] and dirs[6][1]:
        base = rva_off(dirs[6][0])
        for i in range(dirs[6][1] // 28 if base is not None else 0):
            o = base + 28 * i
            if o + 28 > len(data):
                break
            _, dtds, _, _, typ, size, _, ptr = struct.unpack_from("<IIHHIIII", data, o)
            ent = {"type": DEBUG_TYPES.get(typ, f"type{typ}"), "typeId": typ, "size": size}
            if typ == 2 and ptr + 24 <= len(data) and data[ptr:ptr + 4] == b"RSDS":
                end = data.find(b"\0", ptr + 24, ptr + max(size, 24))
                path = data[ptr + 24:end if end >= 0 else ptr + size].decode("utf-8", "replace")
                ent["rsds"] = {"guid": _guid(data[ptr + 4:ptr + 20]), "age": _u32(data, ptr + 20), "pdb": path}
            debug.append(ent)
    return {"machine": MACHINES.get(machine, f"{machine:#x}"), "machineId": machine, "timeDateStamp": tds,
            "characteristics": chars, "optMagic": "PE32+" if magic == 0x20B else "PE32",
            "linker": f"{data[opt + 2]}.{data[opt + 3]}", "subsystem": SUBSYSTEMS.get(subsystem, str(subsystem)),
            "imageBase": image_base, "sections": secs,
            "relocSection": any(s["name"] == ".reloc" for s in secs), "relocDirSize": reloc_dir[1],
            "debug": debug, "rsds": next((e["rsds"] for e in debug if "rsds" in e), None),
            "rich": parse_rich(data, pe)}


def _print(path, p):
    print(f"file {path}")
    print(f"machine {p['machine']}  {p['optMagic']}  TimeDateStamp {p['timeDateStamp']:#010x}  linker {p['linker']}"
          f"  subsystem {p['subsystem']}  imageBase {p['imageBase']:#x}")
    print(f"sections {len(p['sections'])}:")
    for s in p["sections"]:
        print(f"  {s['name']:<8} va {s['va']:#010x} vsize {s['vsize']:#x} rawsize {s['rawsize']:#x}")
    print(f".reloc section {'yes' if p['relocSection'] else 'no'} (reloc dir size {p['relocDirSize']:#x})")
    if p["debug"]:
        print(f"debug dir: {', '.join(e['type'] for e in p['debug'])}")
    else:
        print("debug dir: absent")
    r = p["rsds"]
    print(f"RSDS guid {r['guid']} age {r['age']} pdb {r['pdb']}" if r else "RSDS: absent")
    rich = p["rich"]
    if rich is None:
        print("Rich: absent")
        return
    print(f"Rich at {rich['offset']:#x} key {rich['key']} checksum {rich['checksum']} "
          f"match {'yes' if rich['checksumMatch'] else 'no'}  entries {len(rich['entries'])}")
    for e in rich["entries"]:
        print(f"  prodid {e['prodid']:#06x} build {e['build']:>5} count {e['count']:>4}  "
              f"{e['product'] or '?'}  [{e['source'] or 'not in table'}]")


def main(argv):
    if "--help" in argv or "-h" in argv:
        print(__doc__)
        return 0
    if "--self-test" in argv:
        return self_test()
    pos = [a for a in argv if not a.startswith("--")]
    if len(pos) != 1:
        print("usage: pe_info.py <exe> [--json] | --self-test", file=sys.stderr)
        return 2
    path = Path(pos[0])
    if not path.is_file():
        print(f"pe_info: no such file: {path}", file=sys.stderr)
        return 2
    try:
        p = parse_pe(path.read_bytes())
    except (NotPe, struct.error) as e:
        print(f"pe_info: not a PE: {path}: {e}", file=sys.stderr)
        return 2
    if "--json" in argv:
        p["file"] = str(path)
        print(json.dumps(p, indent=1, sort_keys=True))
    else:
        _print(path, p)
    return 0


def _synthetic(rich_pairs=None, rsds=None, reloc=True, bad_key=False):
    """Minimal synthetic PE32 (no third-party bytes): optional Rich, .reloc and RSDS debug entry."""
    d = bytearray(0x600)
    d[0:2] = b"MZ"
    pe = 0x80
    if rich_pairs is not None:
        dans = 0x80
        n = len(rich_pairs)
        pe = dans + 16 + 8 * n + 8
        pe = (pe + 7) & ~7
        struct.pack_into("<I", d, 0x3C, pe)
        key = rich_checksum(d, dans, rich_pairs) ^ (1 if bad_key else 0)
        struct.pack_into("<4I", d, dans, 0x536E6144 ^ key, key, key, key)
        for i, (c, k) in enumerate(rich_pairs):
            struct.pack_into("<II", d, dans + 16 + 8 * i, c ^ key, k ^ key)
        d[dans + 16 + 8 * n:dans + 20 + 8 * n] = b"Rich"
        struct.pack_into("<I", d, dans + 20 + 8 * n, key)
    struct.pack_into("<I", d, 0x3C, pe)
    d[pe:pe + 4] = b"PE\0\0"
    secs = [(".text", 0x1000, 0x80, 0x200, 0x200), (".rdata", 0x2000, 0x100, 0x200, 0x400)]
    if reloc:
        secs.append((".reloc", 0x3000, 0x10, 0x0, 0x0))
    struct.pack_into("<HHIIIHH", d, pe + 4, 0x14C, len(secs), 0x5A5A1234, 0, 0, 0xE0, 0x102)
    opt = pe + 24
    struct.pack_into("<HBB", d, opt, 0x10B, 7, 10)
    struct.pack_into("<I", d, opt + 28, 0x00400000)
    struct.pack_into("<H", d, opt + 68, 3)
    struct.pack_into("<I", d, opt + 92, 16)
    if reloc:
        struct.pack_into("<II", d, opt + 96 + 8 * 5, 0x3000, 0x10)
    if rsds is not None:
        struct.pack_into("<II", d, opt + 96 + 8 * 6, 0x2000, 28)
        struct.pack_into("<IIHHIIII", d, 0x400, 0, 0, 0, 0, 2, 24 + len(rsds[2]) + 1, 0x201C, 0x41C)
        d[0x41C:0x420] = b"RSDS"
        d[0x420:0x430] = rsds[0]
        struct.pack_into("<I", d, 0x430, rsds[1])
        d[0x434:0x434 + len(rsds[2])] = rsds[2].encode()
    sh = opt + 0xE0
    for i, (nm, va, vs, rs, rp) in enumerate(secs):
        d[sh + 40 * i:sh + 40 * i + len(nm)] = nm.encode()
        struct.pack_into("<IIII", d, sh + 40 * i + 8, vs, va, rs, rp)
    return bytes(d)


def _readobj_rsds(exe):
    """(guid, age) from llvm-readobj --coff-debug-directory, or an error string."""
    import shutil
    tool = shutil.which("llvm-readobj")
    if tool is None:
        return "llvm-readobj not on PATH"
    r = subprocess.run([tool, "--coff-debug-directory", str(exe)], capture_output=True, text=True, errors="replace")
    g = re.search(r"PDBGUID: (\{[0-9A-Fa-f-]+\})", r.stdout)
    a = re.search(r"PDBAge: (\d+)", r.stdout)
    if r.returncode != 0 or not g or not a:
        return f"llvm-readobj exit {r.returncode}, no PDBGUID/PDBAge"
    return g.group(1).upper(), int(a.group(1))


def msvc_src():
    import msvc_fixtures
    return msvc_fixtures.SOURCES


def _check_fixtures(want, built):
    """Rich C/C++/LINK builds == banner builds, RSDS == llvm-readobj, per variant. Returns observations."""
    cl_b, link_b = built["cl"]["build"], built["link"]["build"]
    print(f"  fixture compiler {built['toolset']}: cl {built['cl']['version']}, link {built['link']['version']}")
    obs = {}
    for name, exe in built["variants"].items():
        p = parse_pe(Path(exe).read_bytes())
        rich = p["rich"]
        want(f"{name}: Rich present", rich is not None, True)
        if rich is None:
            continue
        want(f"{name}: Rich checksum matches stored key", rich["checksumMatch"], True)
        for e in rich["entries"]:
            print(f"  {name}: prodid {e['prodid']:#06x} build {e['build']:>5} count {e['count']:>4}  "
                  f"{e['product'] or '? (not in table)'}")
        by = lambda pids: [e for e in rich["entries"] if e["prodid"] in pids]
        c, cpp, link = by((0x104, 0x108)), by((0x105, 0x109)), by((0x102,))
        ours = [e for e in c + cpp if e["build"] == cl_b]
        want(f"{name}: cl-build C/C++ objects counted", sum(e["count"] for e in ours), len(msvc_src()))
        if name == "plain":
            want(f"{name}: C entry with cl build", any(e["build"] == cl_b for e in by((0x104,))), True)
            want(f"{name}: C++ entry with cl build", any(e["build"] == cl_b for e in by((0x105,))), True)
        else:  # /GL: objects are recorded under the LTCG prodids (fixture: C and C++ both under 0x0108)
            want(f"{name}: cl-build entries all LTCG", all(e["prodid"] in (0x108, 0x109) for e in ours), True)
        want(f"{name}: LINK entry with link build", any(e["build"] == link_b for e in link), True)
        used = sorted({PRODUCTS[e["prodid"]][0] for e in ours})
        print(f"  {name}: C/C++ recorded under {', '.join(used) or 'none'}")
        obs[name] = used
        ro = _readobj_rsds(exe)
        want(f"{name}: RSDS present", p["rsds"] is not None, True)
        if p["rsds"] is not None:
            want(f"{name}: RSDS guid/age == llvm-readobj", (p["rsds"]["guid"], p["rsds"]["age"]), ro)
            print(f"  {name}: RSDS {p['rsds']['guid']} age {p['rsds']['age']} (llvm-readobj agrees: "
                  f"{ro == (p['rsds']['guid'], p['rsds']['age'])})")
    return obs


def _check_python(want):
    """Fallback: python.exe Rich must carry Utc1400 C/C++ entries; Rich builds do not encode the cl minor."""
    m = re.search(r"MSC v\.(\d\d)(\d\d)", sys.version)
    print(f"  FALLBACK: python.exe ({sys.executable}); sys.version MSC "
          f"{m.group(1) + '.' + m.group(2) if m else 'not found'}")
    want("python.exe built by MSC 19.x", bool(m) and m.group(1) == "19", True)
    rich = parse_pe(Path(sys.executable).read_bytes())["rich"]
    want("python.exe Rich present", rich is not None, True)
    if rich is None:
        return
    utc = [e for e in rich["entries"] if e["prodid"] in (0x104, 0x105, 0x108, 0x109)]
    for e in utc:
        print(f"  python.exe: {PRODUCTS[e['prodid']][0]} build {e['build']} count {e['count']}")
    # comp.id holds only the 16-bit build, not 19.<minor>: the honest check is the Utc1400 family (VS2015+ = MSC 19.x)
    want("python.exe has a Utc1400 C/C++ entry", bool(utc), True)


def self_test(force_fallback=False):
    import msvc_fixtures
    fails = []
    def want(label, got, exp):
        if got != exp:
            fails.append(f"{label}: got {got!r} want {exp!r}")
    # synthetic: Rich + .reloc + RSDS
    pairs = [(0x00010000, 7), ((0x5F << 16) | 3077, 12), ((0x60 << 16) | 3077, 3), ((0x5A << 16) | 3077, 1)]
    guid = bytes(range(0x10, 0x20))
    p = parse_pe(_synthetic(pairs, (guid, 5, "C:\\syn\\fix.pdb")))
    want("machine", (p["machine"], p["linker"], p["subsystem"], p["imageBase"], p["timeDateStamp"]),
         ("i386", "7.10", "windows_cui", 0x400000, 0x5A5A1234))
    want("sections", [(s["name"], s["va"]) for s in p["sections"]], [(".text", 0x1000), (".rdata", 0x2000),
                                                                     (".reloc", 0x3000)])
    want("reloc", (p["relocSection"], p["relocDirSize"]), (True, 0x10))
    want("rsds", p["rsds"], {"guid": "{13121110-1514-1716-1819-1A1B1C1D1E1F}", "age": 5, "pdb": "C:\\syn\\fix.pdb"})
    want("rich entries", [(e["prodid"], e["build"], e["count"], e["product"]) for e in p["rich"]["entries"]],
         [(1, 0, 7, "Import0"), (0x5F, 3077, 12, "Utc1310_C (VC7.1)"), (0x60, 3077, 3, "Utc1310_CPP (VC7.1)"),
          (0x5A, 3077, 1, "Linker (VC7.1)")])
    want("rich checksum match", (p["rich"]["checksumMatch"], p["rich"]["paddingZero"]), (True, True))
    want("rich bad key reported, not fatal", parse_pe(_synthetic(pairs, bad_key=True))["rich"]["checksumMatch"], False)
    bare = parse_pe(_synthetic(None, None, reloc=False))
    want("absent rich/reloc/debug", (bare["rich"], bare["relocSection"], bare["debug"], bare["rsds"]),
         (None, False, [], None))
    tmp = REPO / ".run" / "1.2" / "pe_selftest"
    tmp.mkdir(parents=True, exist_ok=True)
    try:
        me = [sys.executable, str(Path(__file__).resolve())]
        syn, junk = tmp / "synthetic.exe", tmp / "not_a_pe.dat"
        syn.write_bytes(_synthetic(pairs, (guid, 5, "C:\\syn\\fix.pdb")))
        junk.write_bytes(b"\0" * 0x100)
        for label, cmd, code in (("synthetic, exit 0", me + [str(syn)], 0), ("synthetic --json", me + [str(syn), "--json"], 0),
                                 ("not a PE", me + [str(junk)], 2), ("missing file", me + [str(tmp / "none.exe")], 2)):
            r = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8")
            want(f"exit code: {label}", r.returncode, code)
    finally:
        import shutil
        shutil.rmtree(tmp, ignore_errors=True)
    print(f"  synthetic: {len(fails)} failure(s)")
    if msvc_fixtures.find_toolset() is not None and not force_fallback:
        try:
            _check_fixtures(want, msvc_fixtures.build())
        except msvc_fixtures.BuildError as e:
            fails.append(f"fixture build: {e}")
    else:
        _check_python(want)
    for f in fails:
        print(f"  FAIL {f}")
    print(f"pe_info self-test: {'OK' if not fails else 'FAIL'} — {len(fails)} failure(s)")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
