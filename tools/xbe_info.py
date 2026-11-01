"""XBE reader (T6, phase 1.1): header, certificate, sections, library versions, InitFlags.

CLI: xbe_info.py <xbe> [--json] [--config P] [--manifest P]
     xbe_info.py --self-test     (synthetic header under .run/1.1/, refusals; deleted after)
Exit: 0 parsed and every known-true check passed; 1 a check failed; 2 refusal
(no config, no manifest, not an XBE). Prints hashes, header fields, names and digests only,
never byte runs. Layouts: PHASE_PLAN Interfaces "XBE" (R1.1-034, R0-024, R0-010).
Importable: parse_xbe(data) -> dict, va_to_off(x, va), run_checks(x, file_size) -> list.
"""
import hashlib
import json
import struct
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

MAGIC = b"XBEH"
HDR_MIN = 0x178
HEADER_FIELDS = (  # (name, offset); all u32 LE
    ("baseAddr", 0x104), ("sizeOfHeaders", 0x108), ("sizeOfImage", 0x10C),
    ("sizeOfImageHeader", 0x110), ("timeDate", 0x114), ("certificateAddr", 0x118),
    ("sectionCount", 0x11C), ("sectionHeadersAddr", 0x120), ("initFlags", 0x124),
    ("entryAddr", 0x128), ("tlsAddr", 0x12C), ("peStackCommit", 0x130),
    ("peHeapReserve", 0x134), ("peHeapCommit", 0x138), ("peBaseAddr", 0x13C),
    ("peImageSize", 0x140), ("peChecksum", 0x144), ("peTimeDate", 0x148),
    ("debugPathnameAddr", 0x14C), ("debugFilenameAddr", 0x150),
    ("debugUnicodeFilenameAddr", 0x154), ("kernThunkAddr", 0x158), ("importDirAddr", 0x15C),
    ("libVersionsCount", 0x160), ("libVersionsAddr", 0x164), ("kernLibVersionAddr", 0x168),
    ("xapiLibVersionAddr", 0x16C), ("logoAddr", 0x170), ("logoSize", 0x174),
)
EXTRA_NOTE = ("0x178+ unresolved (ghidra: libFeaturesAddr 0x178, libFeaturesCount 0x17C, "
              "debugInfo 0x180; xboxdevwiki: Unknown1; R1.1-031): raw")
ENTRY_KEYS = (("retail", 0xA8FC57AB), ("debug", 0x94859D4B))
THUNK_KEYS = (("retail", 0x5B6D40B6), ("debug", 0xEFB1F152))
SEC_FLAGS = ((0x1, "WRITABLE"), (0x2, "PRELOAD"), (0x4, "EXECUTABLE"), (0x8, "INSERTED_FILE"),
             (0x10, "HEAD_PAGE_RO"), (0x20, "TAIL_PAGE_RO"))
INIT_FLAGS = ((0x1, "MountUtilityDrive"), (0x2, "FormatUtilityDrive"),
              (0x4, "Limit64Megabytes"), (0x8, "DontSetupHarddisk"))
SEC_SIZE, LIB_SIZE = 0x38, 16


class NotXbe(Exception):
    pass


def _u32(d, o):
    return struct.unpack_from("<I", d, o)[0]


def _flag_names(v, table):
    names = [n for b, n in table if v & b]
    rest = v & ~sum(b for b, _ in table)
    return names + ([f"unknown 0x{rest:X}"] if rest else [])


def _cstr(d, off, cap=64):
    if off is None or not 0 <= off < len(d):
        return None
    end = d.find(b"\0", off, off + cap)
    raw = d[off:end if end >= 0 else off + cap]
    return raw.decode("ascii", "replace")


def _wstr(d, off, cap=520):
    if off is None or not 0 <= off < len(d):
        return None
    raw = d[off:off + cap]
    end = next((i for i in range(0, len(raw) - 1, 2) if raw[i:i + 2] == b"\0\0"), len(raw) & ~1)
    return raw[:end].decode("utf-16-le", "replace")


def va_to_off(x, va):
    """File offset of a VA: header region, else inside a section's raw data; None if unmapped."""
    base = x["header"]["baseAddr"]
    if base <= va < base + x["header"]["sizeOfHeaders"]:
        return va - base
    for s in x.get("sections", []):
        if s["virtualAddr"] <= va < s["virtualAddr"] + s["rawSize"]:
            return va - s["virtualAddr"] + s["rawAddr"]
    return None


def _decode(enc, keys, lo, hi):
    tried = []
    for name, key in keys:
        va = enc ^ key
        tried.append({"key": name, "keyValue": key, "va": va, "inImage": lo <= va < hi})
    hit = next((t for t in tried if t["inImage"]), None)
    return {"encoded": enc, "key": hit["key"] if hit else None,
            "va": hit["va"] if hit else None, "tried": tried}


def parse_xbe(data):
    """Parse an XBE image; raises NotXbe on bad magic or a header that does not fit the file."""
    if len(data) < HDR_MIN or data[:4] != MAGIC:
        raise NotXbe("magic is not XBEH or file shorter than the 0x178-byte image header")
    h = {n: _u32(data, o) for n, o in HEADER_FIELDS}
    if not HDR_MIN <= h["sizeOfHeaders"] <= len(data):
        raise NotXbe(f"sizeOfHeaders 0x{h['sizeOfHeaders']:X} outside 0x178..file size")
    x = {"header": h, "sections": []}
    base = h["baseAddr"]
    extra_end = min(h["sizeOfImageHeader"], h["sizeOfHeaders"]) & ~3
    x["headerExtra"] = [{"offset": o, "value": _u32(data, o)} for o in range(HDR_MIN, extra_end, 4)]
    lo, hi = base, base + h["sizeOfImage"]
    x["entry"] = _decode(h["entryAddr"], ENTRY_KEYS, lo, hi)
    x["kernThunk"] = _decode(h["kernThunkAddr"], THUNK_KEYS, lo, hi)
    x["initFlags"] = {"value": h["initFlags"], "bits": _flag_names(h["initFlags"], INIT_FLAGS),
                      "Limit64Megabytes": bool(h["initFlags"] & 0x4)}
    sh = h["sectionHeadersAddr"] - base
    for i in range(h["sectionCount"]):
        o = sh + SEC_SIZE * i
        if not 0 <= o <= len(data) - SEC_SIZE:
            break
        f = struct.unpack_from("<9I", data, o)
        s = dict(zip(("flags", "virtualAddr", "virtualSize", "rawAddr", "rawSize", "sectionNameAddr",
                      "sectionNameRefCount", "headSharedPageRefCountAddr",
                      "tailSharedPageRefCountAddr"), f))
        s["digest"] = data[o + 0x24:o + 0x38].hex()
        s["flagNames"] = _flag_names(s["flags"], SEC_FLAGS)
        x["sections"].append(s)
    for s in x["sections"]:
        s["name"] = _cstr(data, va_to_off(x, s["sectionNameAddr"]))
    c = h["certificateAddr"] - base
    if 0 <= c <= len(data) - 0xB0:
        x["certificate"] = {
            "size": _u32(data, c), "timeDate": _u32(data, c + 4), "titleId": _u32(data, c + 8),
            "titleName": data[c + 0x0C:c + 0x5C].decode("utf-16-le", "replace").split("\0")[0],
            "allowedMedia": _u32(data, c + 0x9C), "region": _u32(data, c + 0xA0),
            "ratings": _u32(data, c + 0xA4), "discNumber": _u32(data, c + 0xA8),
            "version": _u32(data, c + 0xAC)}
    libs, lb = [], h["libVersionsAddr"] - base
    for i in range(h["libVersionsCount"]):
        o = lb + LIB_SIZE * i
        if not 0 <= o <= len(data) - LIB_SIZE:
            break
        maj, mnr, bld, fl = struct.unpack_from("<4H", data, o + 8)
        va = h["libVersionsAddr"] + LIB_SIZE * i
        libs.append({"index": i, "va": va, "name": data[o:o + 8].split(b"\0")[0].decode("ascii", "replace"),
                     "major": maj, "minor": mnr, "build": bld, "flags": fl,
                     "qfe": fl & 0x1FFF, "approved": (fl & 0x6000) >> 13, "debug": bool(fl & 0x8000),
                     "role": ("kernel" if va == h["kernLibVersionAddr"] else
                              "xapi" if va == h["xapiLibVersionAddr"] else None)})
    x["libraries"] = libs
    x["debug"] = {"pathname": _cstr(data, va_to_off(x, h["debugPathnameAddr"]), 260),
                  "filename": _cstr(data, va_to_off(x, h["debugFilenameAddr"]), 260),
                  "unicodeFilename": _wstr(data, va_to_off(x, h["debugUnicodeFilenameAddr"]))}
    return x


def run_checks(x, file_size):
    """Known-true structural checks: list of (name, ok, detail)."""
    h, base, soh = x["header"], x["header"]["baseAddr"], x["header"]["sizeOfHeaders"]
    def inside(va, n):
        return base <= va and va + n <= base + soh
    lib_vas = {l["va"] for l in x["libraries"]}
    out = [("magic XBEH", True, "bytes 0..4"),
           ("baseAddr read at 0x104", True, f"0x{base:08X}"),
           ("sizeOfImageHeader <= sizeOfHeaders", h["sizeOfImageHeader"] <= soh,
            f"0x{h['sizeOfImageHeader']:X} <= 0x{soh:X}"),
           ("certificate inside sizeOfHeaders", inside(h["certificateAddr"], 0xB0) and "certificate" in x,
            f"0x{h['certificateAddr']:08X}"),
           ("section headers inside sizeOfHeaders",
            inside(h["sectionHeadersAddr"], SEC_SIZE * h["sectionCount"])
            and len(x["sections"]) == h["sectionCount"],
            f"0x{h['sectionHeadersAddr']:08X} + {h['sectionCount']}*0x38"),
           ("library array inside sizeOfHeaders",
            inside(h["libVersionsAddr"], LIB_SIZE * h["libVersionsCount"])
            and len(x["libraries"]) == h["libVersionsCount"],
            f"0x{h['libVersionsAddr']:08X} + {h['libVersionsCount']}*16")]
    bad = [s["name"] for s in x["sections"] if s["rawAddr"] + s["rawSize"] > file_size]
    out.append(("sections inside file size", not bad, f"file {file_size}" + (f"; over: {bad}" if bad else "")))
    for key, label in (("kernLibVersionAddr", "0x0168 kernel"), ("xapiLibVersionAddr", "0x016C xapi")):
        out.append((f"{label} VA lands on a 0x0164 entry", h[key] in lib_vas, f"0x{h[key]:08X}"))
    out.append(("entry decodes into image", x["entry"]["key"] is not None, str(x["entry"]["key"])))
    out.append(("kernel thunk decodes into image", x["kernThunk"]["key"] is not None, str(x["kernThunk"]["key"])))
    return out


def manifest_status(path, sha1, size, cfg, records):
    hits = [r for r in records if r.get("sha1") == sha1 and r.get("size") == size]
    if hits:
        return "match", hits[0]
    try:
        same = Path(path).resolve() == Path(cfg["retail_xbe"]).resolve()
    except OSError:
        same = False
    return ("MISMATCH" if same else "absent"), None


def _print(path, sha1, status, rec, x, checks):
    h = x["header"]
    print(f"file {path}")
    print(f"sha1 {sha1} manifest {status}" + (f" ({rec['role']}/{rec['name']})" if rec else ""))
    print("== image header")
    for n, o in HEADER_FIELDS:
        print(f"  0x{o:03X} {n:26s} 0x{h[n]:08X}")
    print(f"  {EXTRA_NOTE}")
    for e in x["headerExtra"]:
        print(f"  0x{e['offset']:03X} {'(raw)':26s} 0x{e['value']:08X}")
    for label, d in (("entry point", x["entry"]), ("kernel thunk", x["kernThunk"])):
        tried = ", ".join(f"{t['key']}->0x{t['va']:08X}{' in image' if t['inImage'] else ''}" for t in d["tried"])
        va = f"0x{d['va']:08X}" if d["va"] is not None else "none in image"
        print(f"  {label}: encoded 0x{d['encoded']:08X} decoded {va} key {d['key']} ({tried})")
    f = x["initFlags"]
    print(f"== InitFlags 0x{f['value']:08X}")
    for b, n in INIT_FLAGS:
        print(f"  0x{b:X} {n:20s} {'set' if f['value'] & b else 'clear'}")
    for n in f["bits"]:
        if n.startswith("unknown"):
            print(f"  {n} set")
    c = x.get("certificate")
    print("== certificate")
    if c:
        print(f"  titleId 0x{c['titleId']:08X}  titleName {c['titleName']!r}")
        print(f"  allowedMedia 0x{c['allowedMedia']:08X}  region 0x{c['region']:08X}  "
              f"version 0x{c['version']:08X}  discNumber {c['discNumber']}")
        print(f"  size 0x{c['size']:X}  timeDate 0x{c['timeDate']:08X}  ratings 0x{c['ratings']:08X}")
    else:
        print("  not readable")
    dbg = x["debug"]
    print("== debug strings (0x14C pathname, 0x150 filename, 0x154 unicode filename)")
    print(f"  pathname {dbg['pathname']!r}  filename {dbg['filename']!r}  unicodeFilename {dbg['unicodeFilename']!r}")
    print(f"== sections ({len(x['sections'])})")
    for s in x["sections"]:
        print(f"  {str(s['name']):10s} flags 0x{s['flags']:02X} {'|'.join(s['flagNames']) or '-'}")
        print(f"    va 0x{s['virtualAddr']:08X} vsize 0x{s['virtualSize']:08X} "
              f"raw 0x{s['rawAddr']:08X} rawsize 0x{s['rawSize']:08X} digest {s['digest']}")
    print(f"== library versions ({len(x['libraries'])})")
    for l in x["libraries"]:
        role = f"  <- {l['role']} (0x{0x168 if l['role'] == 'kernel' else 0x16C:03X})" if l["role"] else ""
        print(f"  [{l['index']:2d}] va 0x{l['va']:08X} {l['name']:8s} {l['major']}.{l['minor']}.{l['build']} "
              f"flags 0x{l['flags']:04X} (qfe {l['qfe']}, approved {l['approved']}, "
              f"debug {int(l['debug'])}){role}")
    print("== checks")
    for n, ok, d in checks:
        print(f"  {'ok  ' if ok else 'FAIL'} {n}: {d}")


def main(argv):
    if "-h" in argv or "--help" in argv:
        print(__doc__.rstrip())  # CLI, exits and the importable surface; no source read needed (AUDIT 1.8)
        return 0
    if "--self-test" in argv:
        return self_test()
    import vv_inputs
    pos = [a for i, a in enumerate(argv) if not a.startswith("--")
           and (i == 0 or argv[i - 1] not in ("--config", "--manifest"))]
    if len(pos) != 1:
        print("usage: xbe_info.py <xbe> [--json] [--config P] [--manifest P] | --self-test", file=sys.stderr)
        return 2
    opt = lambda k: argv[argv.index(k) + 1] if k in argv else None
    cfg = vv_inputs.load_config(opt("--config"))
    records = vv_inputs.load_manifest(opt("--manifest"))
    path = Path(pos[0])
    if not path.is_file():
        print(f"xbe_info: no such file: {path}", file=sys.stderr)
        return 2
    data = path.read_bytes()
    try:
        x = parse_xbe(data)
    except NotXbe as e:
        print(f"xbe_info: not an XBE: {path}: {e}", file=sys.stderr)
        return 2
    sha1 = hashlib.sha1(data).hexdigest()
    status, rec = manifest_status(path, sha1, len(data), cfg, records)
    checks = run_checks(x, len(data))
    if "--json" in argv:
        x.update(file=str(path), sha1=sha1, manifest=status,
                 checks=[{"name": n, "ok": ok, "detail": d} for n, ok, d in checks])
        print(json.dumps(x, indent=1, sort_keys=True))
    else:
        _print(path, sha1, status, rec, x, checks)
    failed = [n for n, ok, _ in checks if not ok]
    print(f"xbe_info: {'OK' if not failed else 'FAIL'} — {len(checks) - len(failed)}/{len(checks)} checks passed",
          file=sys.stderr)
    return 1 if failed else 0


def _synthetic():
    """Synthetic XBE (no game bytes) and its planted values."""
    base, soh, img = 0x10000, 0x1000, 0x4000
    d = bytearray(0x3000)
    d[0:4] = MAGIC
    cert, sh, lib, names = base + 0x200, base + 0x400, base + 0x500, base + 0x600
    plant = {"baseAddr": base, "sizeOfHeaders": soh, "sizeOfImage": img, "sizeOfImageHeader": 0x184,
             "timeDate": 0x11223344, "certificateAddr": cert, "sectionCount": 2, "sectionHeadersAddr": sh,
             "initFlags": 0x5, "entryAddr": (base + 0x1010) ^ 0xA8FC57AB, "tlsAddr": 0,
             "kernThunkAddr": (base + 0x2000) ^ 0xEFB1F152, "libVersionsCount": 3, "libVersionsAddr": lib,
             "kernLibVersionAddr": lib + 16, "xapiLibVersionAddr": lib + 32, "logoAddr": 0, "logoSize": 0,
             "debugPathnameAddr": base + 0x700, "debugFilenameAddr": base + 0x709,
             "debugUnicodeFilenameAddr": base + 0x740}
    for n, o in HEADER_FIELDS:
        struct.pack_into("<I", d, o, plant.get(n, 0x100 + o))
    struct.pack_into("<3I", d, 0x178, 0xAAAA0001, 0xAAAA0002, 0xAAAA0003)
    struct.pack_into("<5I", d, cert - base, 0x1D0, 0x55667788, 0x4D530099, 0, 0)
    d[cert - base + 0x0C:cert - base + 0x0C + 16] = "SynthT".encode("utf-16-le") + b"\0\0\0\0"
    struct.pack_into("<5I", d, cert - base + 0x9C, 0x80000007, 0x1, 0x2, 3, 0x0102)
    secs = (("SYNA", 0x6, base + 0x1000, 0x900, 0x1000, 0x1000), ("SYNB", 0x1, base + 0x2000, 0x400, 0x2000, 0x800))
    for i, (nm, fl, va, vs, ra, rs) in enumerate(secs):
        na = names + 8 * i
        d[na - base:na - base + len(nm)] = nm.encode()
        struct.pack_into("<9I", d, sh - base + SEC_SIZE * i, fl, va, vs, ra, rs, na, 1, 0, 0)
        d[sh - base + SEC_SIZE * i + 0x24:sh - base + SEC_SIZE * i + 0x38] = bytes(range(i, i + 20))
    for i, (nm, b, fl) in enumerate((("LIBA", 1111, 0x4001), ("XBOXKRNL", 2222, 0x4000), ("XAPILIB", 3333, 0x8002))):
        o = lib - base + 16 * i
        d[o:o + len(nm)] = nm.encode()
        struct.pack_into("<4H", d, o + 8, 1, 0, b, fl)
    d[0x700:0x70E] = rb"d:\synth\x.exe"
    d[0x740:0x74C] = "x.exe".encode("utf-16-le") + b"\0\0"
    return bytes(d), plant, secs


def self_test():
    import shutil
    tmp = REPO / ".run" / "1.1" / "xbe_selftest"
    tmp.mkdir(parents=True, exist_ok=True)
    fails = []
    def want(label, got, exp):
        if got != exp:
            fails.append(f"{label}: got {got!r} want {exp!r}")
    try:
        data, plant, secs = _synthetic()
        xbe = tmp / "synthetic.xbe"
        xbe.write_bytes(data)
        x = parse_xbe(xbe.read_bytes())
        for k, v in plant.items():
            want(f"header {k}", x["header"][k], v)
        want("headerExtra", [e["value"] for e in x["headerExtra"]], [0xAAAA0001, 0xAAAA0002, 0xAAAA0003])
        want("entry", (x["entry"]["va"], x["entry"]["key"]), (0x11010, "retail"))
        want("thunk", (x["kernThunk"]["va"], x["kernThunk"]["key"]), (0x12000, "debug"))
        want("initFlags", x["initFlags"]["bits"], ["MountUtilityDrive", "Limit64Megabytes"])
        c = x["certificate"]
        want("cert", (c["titleId"], c["titleName"], c["allowedMedia"], c["region"], c["discNumber"], c["version"]),
             (0x4D530099, "SynthT", 0x80000007, 1, 3, 0x0102))
        want("sections", [(s["name"], s["flags"], s["virtualAddr"], s["virtualSize"], s["rawAddr"], s["rawSize"])
                          for s in x["sections"]], [tuple(s) for s in secs])
        want("digest0", x["sections"][0]["digest"], bytes(range(20)).hex())
        want("libs", [(l["name"], l["build"], l["flags"], l["role"]) for l in x["libraries"]],
             [("LIBA", 1111, 0x4001, None), ("XBOXKRNL", 2222, 0x4000, "kernel"), ("XAPILIB", 3333, 0x8002, "xapi")])
        want("debug strings", x["debug"], {"pathname": r"d:\synth\x.exe", "filename": "x.exe",
                                           "unicodeFilename": "x.exe"})
        bad = [n for n, ok, _ in run_checks(x, len(data)) if not ok]
        want("checks failing", bad, [])
        me = [sys.executable, str(Path(__file__).resolve())]
        junk = tmp / "not_an_xbe.bin"
        junk.write_bytes(b"\0" * 0x400)
        empty_mf = tmp / "none.manifest.jsonl"
        runs = (("synthetic parses, exit 0", me + [str(xbe)], 0),
                ("non-XBE scratch file", me + [str(junk)], 2),
                ("no config", me + [str(xbe), "--config", str(tmp / "missing_config.json")], 2),
                ("no manifest", me + [str(xbe), "--manifest", str(empty_mf)], 2))
        for label, cmd, code in runs:
            r = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8")
            want(f"exit code: {label}", r.returncode, code)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    for f in fails:
        print(f"  FAIL {f}")
    print(f"xbe_info self-test: {'OK' if not fails else 'FAIL'} — {len(fails)} failure(s)")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
