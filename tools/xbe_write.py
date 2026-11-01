"""XBE writer (imagebld equivalent) for a linked PE plus a pin XBE (phase 2.3 T2).

CLI: xbe_write.py <pe> --xbe <pin> --out <path>   (path under derived/relink/; writes the image, compares to pin)
     xbe_write.py --self-test                     (prototype pair, retail round-trip, byte-flip and PE controls)
Exit: 0 image equals the pin; 1 compare found a difference (names it); 2 refusal (pair not one build, section
map mismatch, input not derivable). Prints names, sizes, offsets and hashes only, never byte runs (D46).
Importable: write(pe_bytes, xbe_pin_bytes) -> bytes (raises Refusal), compare(out, pin) -> list of difference names.
  write_section(pe_bytes, xbe_pin_bytes, name) -> {va, vsize, rawSize, digest, data} of one linked section,
  same rebase/trim/digest as write() without the build-pair parts (T3: one-section relink).
From the pin (non-linker parts): header region bytes (signature, certificate, library table, debug strings, logo,
section names/flags/refcounts), baseAddr, sizeOfHeaders, sizeOfImage, peStackCommit, the entry/thunk key kind, and the
inserted-file sections (data, flags, raw order). Recomputed from the PE: every section's VA, vsize, rawAddr, rawSize,
digest; entry, kernel thunk, tlsAddr; peBaseAddr, peImageSize, peTimeDate, peChecksum, peHeap*; the PE-header copy.
Rules and evidence: docs/evidence/relink.md "XBE writer (T2)".
"""
import argparse
import hashlib
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import pe_info  # noqa: E402
import vv_inputs  # noqa: E402  read-only import (verifier-locked)
import xbe_info  # noqa: E402  read-only import (verifier-locked)

REPO = Path(__file__).resolve().parent.parent
PIN = REPO / "config" / "check.retail_xbe.sha"
OUT_ROOT = REPO / "derived" / "relink"
DROP = ("INIT", ".XBLD", ".tls", ".reloc")  # PE-only sections: evidence, not image
VA_ALIGN, RAW_ALIGN = 0x20, 0x1000
KEYS = {"entry": dict(xbe_info.ENTRY_KEYS), "thunk": dict(xbe_info.THUNK_KEYS)}


class Refusal(Exception):
    pass


def _u32(d, o):
    return struct.unpack_from("<I", d, o)[0]


def _align(v, a):
    return (v + a - 1) & ~(a - 1)


def _pe_fields(pe):
    """Optional-header fields pe_info does not return."""
    opt = _u32(pe, 0x3C) + 24
    return {"entry": _u32(pe, opt + 16), "sizeOfImage": _u32(pe, opt + 56), "sizeOfHeaders": _u32(pe, opt + 60),
            "checksum": _u32(pe, opt + 64), "heapReserve": _u32(pe, opt + 80), "heapCommit": _u32(pe, opt + 84),
            "dirs": [struct.unpack_from("<II", pe, opt + 96 + 8 * i) for i in range(16)]}


def _relocs(pe, secs, rva, size):
    """HIGHLOW site RVAs of the .reloc directory; a zero-size block ends it."""
    s = next((s for s in secs if s["va"] <= rva < s["va"] + s["rawsize"]), None)
    if s is None:
        return []
    r, i, out = pe[s["rawptr"] + rva - s["va"]:s["rawptr"] + rva - s["va"] + size], 0, []
    while i + 8 <= len(r):
        page, n = struct.unpack_from("<II", r, i)
        if n == 0:
            break
        for j in range(8, n, 2):
            e = struct.unpack_from("<H", r, i + j)[0]
            if e >> 12 == 3:
                out.append(page + (e & 0xFFF))
            elif e >> 12:
                raise Refusal(f".reloc entry type {e >> 12} at page 0x{page:X}: only HIGHLOW is derivable")
        i += n
    return out


def _parse(pe_bytes, xbe_pin_bytes):
    try:
        p = pe_info.parse_pe(pe_bytes)
    except pe_info.NotPe as e:
        raise Refusal(f"not a PE: {e}")
    try:
        x = xbe_info.parse_xbe(xbe_pin_bytes)
    except xbe_info.NotXbe as e:
        raise Refusal(f"pin is not an XBE: {e}")
    return p, x, _pe_fields(pe_bytes)


def _section_map(p, x):
    """(linked PE sections, pin linked sections, pin inserted files); refuses unless names, order and align4(vsize)
    agree."""
    linked = [s for s in p["sections"] if s["name"] not in DROP]
    xl = [s for s in x["sections"] if "INSERTED_FILE" not in s["flagNames"]]
    xi = [s for s in x["sections"] if "INSERTED_FILE" in s["flagNames"]]
    for k in range(max(len(linked), len(xl))):
        a = linked[k]["name"] if k < len(linked) else None
        b = xl[k]["name"] if k < len(xl) else None
        if a != b:
            raise Refusal(f"section map: PE linked section #{k} {a} != XBE {b}")
        if _align(linked[k]["vsize"], 4) != xl[k]["virtualSize"]:
            raise Refusal(f"section map: {a} vsize 0x{linked[k]['vsize']:X} does not give XBE 0x"
                          f"{xl[k]['virtualSize']:X}")
    if x["sections"][:len(xl)] != xl:
        raise Refusal("section map: an INSERTED_FILE section precedes a linked section in the XBE table")
    return linked, xl, xi


def _rebase(pe_bytes, p, f, pe_base):
    """Section name -> data with every .reloc HIGHLOW site moved by one delta (peBaseAddr - imageBase); -> (img, at)."""
    delta = (pe_base - p["imageBase"]) & 0xFFFFFFFF
    img = {s["name"]: bytearray(pe_bytes[s["rawptr"]:s["rawptr"] + s["rawsize"]]) for s in p["sections"]}

    def at(rva):
        s = next((s for s in p["sections"] if s["va"] <= rva < s["va"] + s["rawsize"]), None)
        return (s["name"], rva - s["va"]) if s else (None, None)

    for rva in _relocs(pe_bytes, p["sections"], *f["dirs"][5]):
        n, o = at(rva)
        if n is None:
            raise Refusal(f"reloc site RVA 0x{rva:X} outside every PE section")
        if n not in DROP:
            struct.pack_into("<I", img[n], o, (_u32(img[n], o) + delta) & 0xFFFFFFFF)
    return img, at


def _trim(d):
    """XBE raw data of a section: trailing zeros dropped, length rounded up to 4."""
    size = _align(len(d.rstrip(b"\0")), 4)
    return (d + b"\0" * size)[:size]


def _digest(d):
    """C0002: SHA1(u32le(rawSize) + data)."""
    return hashlib.sha1(struct.pack("<I", len(d)) + d).digest()


def write_section(pe_bytes, xbe_pin_bytes, name):
    """One linked section of the XBE write() would build, without the build-pair parts (INIT, .XBLD, .tls, kernel
    import, timestamp): -> {va, vsize, rawSize, digest, data}. Refuses unless the section map agrees and every linked PE
    section lands at its pin VA (one peBaseAddr as write())."""
    p, x, f = _parse(pe_bytes, xbe_pin_bytes)
    linked, xl, _xi = _section_map(p, x)
    h = x["header"]
    pe_base = h["baseAddr"] + h["sizeOfHeaders"] - f["sizeOfHeaders"]
    for s, t in zip(linked, xl):
        if s["va"] + pe_base != t["virtualAddr"]:
            raise Refusal(f"section map: {s['name']} at XBE VA 0x{s['va'] + pe_base:X} != pin 0x{t['virtualAddr']:X}")
    s = next((s for s in linked if s["name"] == name), None)
    if s is None:
        raise Refusal(f"no linked section {name}")
    d = _trim(bytes(_rebase(pe_bytes, p, f, pe_base)[0][name]))
    return {"va": s["va"] + pe_base, "vsize": _align(s["vsize"], 4), "rawSize": len(d), "digest": _digest(d),
            "data": d}


def write(pe_bytes, xbe_pin_bytes):
    """Build the XBE image of a linked PE; non-linker parts come from the pin. Raises Refusal (exit 2)."""
    p, x, f = _parse(pe_bytes, xbe_pin_bytes)
    h = x["header"]
    if h["peTimeDate"] != p["timeDateStamp"]:
        raise Refusal(f"pair is not one build: XBE peTimeDate 0x{h['peTimeDate']:08X} != PE TimeDateStamp "
                      f"0x{p['timeDateStamp']:08X}")
    secs = {s["name"]: s for s in p["sections"]}
    for n in DROP:
        if n not in secs:
            raise Refusal(f"PE section {n} missing (needed as evidence)")
    linked, xl, xi = _section_map(p, x)
    base, soh = h["baseAddr"], h["sizeOfHeaders"]
    pe_base = base + soh - f["sizeOfHeaders"]   # XBE VA = PE RVA + pe_base, one delta for every section
    img, at = _rebase(pe_bytes, p, f, pe_base)
    tls_rva = f["dirs"][9][0]
    if tls_rva:
        n, o = at(tls_rva)
        start, end = _u32(img[n], o), _u32(img[n], o + 4)
        if start or end:
            tn, to = at((start - pe_base) & 0xFFFFFFFF)
            if tn is None or img[tn][to:to + end - start].strip(b"\0"):
                raise Refusal("TLS template is not all zero: its XBE placement is not derivable from this pair")
            struct.pack_into("<II", img[n], o, 0, 0)  # zero template -> no raw data, all zero fill
            struct.pack_into("<I", img[n], o + 16, _u32(img[n], o + 16) + end - start)
    imp_rva = f["dirs"][1][0]
    if imp_rva:
        n, o = at(imp_rva)
        if img[n][o:o + 20].strip(b"\0") and _u32(img[n], o + 16) != f["dirs"][12][0]:
            raise Refusal(f"INIT import descriptor FirstThunk 0x{_u32(img[n], o + 16):X} != IAT directory "
                          f"0x{f['dirs'][12][0]:X}")
    out_secs, raw = [], soh
    for s in linked:
        d = _trim(bytes(img[s["name"]]))
        ra = _align(raw, RAW_ALIGN)
        out_secs.append((s["name"], s["va"] + pe_base, _align(s["vsize"], 4), ra, d))
        raw = ra + len(d)
    va = out_secs[-1][1] + out_secs[-1][2]
    ins_va = {}
    for s in xi:
        va = _align(va, VA_ALIGN)
        ins_va[s["name"]] = va
        va += s["virtualSize"]
    ins_raw = {}
    for s in sorted(xi, key=lambda s: s["rawAddr"]):
        ins_raw[s["name"]] = ra = _align(raw, RAW_ALIGN)
        raw = ra + s["rawSize"]
    for s in xi:
        out_secs.append((s["name"], ins_va[s["name"]], s["virtualSize"], ins_raw[s["name"]],
                         xbe_pin_bytes[s["rawAddr"]:s["rawAddr"] + s["rawSize"]]))
    out = bytearray(_align(raw, RAW_ALIGN))
    out[:soh] = xbe_pin_bytes[:soh]
    sh = h["sectionHeadersAddr"] - base
    for k, (n, sva, vs, ra, d) in enumerate(out_secs):
        o = sh + xbe_info.SEC_SIZE * k
        struct.pack_into("<IIII", out, o + 4, sva, vs, ra, len(d))
        out[o + 0x24:o + 0x38] = _digest(d)
        out[ra:ra + len(d)] = d
    put = {"entryAddr": (f["entry"] + pe_base) ^ KEYS["entry"][x["entry"]["key"] or "retail"],
           "kernThunkAddr": (f["dirs"][12][0] + pe_base) ^ KEYS["thunk"][x["kernThunk"]["key"] or "retail"],
           "tlsAddr": tls_rva + pe_base if tls_rva else 0, "peBaseAddr": pe_base,
           "peImageSize": f["sizeOfImage"], "peTimeDate": p["timeDateStamp"], "peChecksum": f["checksum"],
           "peHeapReserve": f["heapReserve"], "peHeapCommit": f["heapCommit"]}
    offs = dict(xbe_info.HEADER_FIELDS)
    for k, v in put.items():
        struct.pack_into("<I", out, offs[k], v & 0xFFFFFFFF)
    po = pe_base - base
    out[po:po + f["sizeOfHeaders"]] = pe_bytes[:f["sizeOfHeaders"]]
    lb, xbld = h["libVersionsAddr"] - base, secs[".XBLD"]
    if pe_bytes[xbld["rawptr"]:xbld["rawptr"] + xbld["vsize"]] != bytes(out[lb:lb + 16 * h["libVersionsCount"]]):
        raise Refusal(".XBLD does not equal the pin's library version table")
    return bytes(out)


def compare(out, pin):
    """Names of every difference between a written image and the pin; [] when byte-identical."""
    if out == pin:
        return []
    a, b = xbe_info.parse_xbe(out), xbe_info.parse_xbe(pin)
    diffs = [f"header {n}" for n, _ in xbe_info.HEADER_FIELDS if a["header"][n] != b["header"][n]]
    for k in range(max(len(a["sections"]), len(b["sections"]))):
        if k >= len(a["sections"]) or k >= len(b["sections"]):
            diffs.append(f"section #{k} count")
            continue
        s, t = a["sections"][k], b["sections"][k]
        for fld in ("virtualAddr", "virtualSize", "rawAddr", "rawSize", "digest"):
            if s[fld] != t[fld]:
                diffs.append(f"{t['name']} {fld}")
    if len(out) != len(pin):
        diffs.append(f"file length {len(out)} != {len(pin)}")
    if not diffs:
        soh = b["header"]["sizeOfHeaders"]
        n = sum(1 for i in range(min(soh, len(out))) if out[i] != pin[i])
        diffs.append(f"header region {n} bytes" if n else "section data or padding")
    return diffs


def _report(out, pin):
    x = xbe_info.parse_xbe(out)
    ok = sum(1 for s, t in zip(x["sections"], xbe_info.parse_xbe(pin)["sections"]) if s["digest"] == t["digest"])
    return ok, len(x["sections"]), hashlib.sha1(out).hexdigest(), hashlib.sha1(pin).hexdigest()


def _proto_pair():
    cfg = vv_inputs.load_config()
    return (vv_inputs.input_path("prototype_dir", "default.exe", cfg=cfg).read_bytes(),
            vv_inputs.input_path("prototype_dir", "default.xbe", cfg=cfg).read_bytes())


def _retail_pe(rx):
    """Retail sections in the writer's input form: the XBE's PE-header copy (section VA == raw pointer), linked
    section data at the PE raw pointers, .XBLD = the library table, INIT/.tls/.reloc zero (no reloc sites: bytes
    are already at XBE VAs)."""
    x = xbe_info.parse_xbe(rx)
    h = x["header"]
    po = h["peBaseAddr"] - h["baseAddr"]
    soh = _pe_fields(rx[po:])["sizeOfHeaders"]
    hdr = rx[po:po + soh]
    p = pe_info.parse_pe(hdr + b"\0" * 0x400)
    pe = bytearray(max(s["rawptr"] + s["rawsize"] for s in p["sections"]))
    pe[:soh] = hdr
    xs = {s["name"]: s for s in x["sections"]}
    for s in p["sections"]:
        if s["name"] in xs:
            t = xs[s["name"]]
            d = rx[t["rawAddr"]:t["rawAddr"] + min(t["rawSize"], s["rawsize"])]
        elif s["name"] == ".XBLD":
            lb = h["libVersionsAddr"] - h["baseAddr"]
            d = rx[lb:lb + 16 * h["libVersionsCount"]]
        else:
            continue
        pe[s["rawptr"]:s["rawptr"] + len(d)] = d
    return bytes(pe)


def _pe_control(pe, kind):
    """A copy of the PE with one section renamed, two reordered or one resized; -> (bytes, section named)."""
    p = pe_info.parse_pe(pe)
    opt = _u32(pe, 0x3C) + 24
    sh = opt + struct.unpack_from("<H", pe, _u32(pe, 0x3C) + 20)[0]
    idx = {s["name"]: i for i, s in enumerate(p["sections"])}
    b = bytearray(pe)
    if kind == "renamed":
        o = sh + 40 * idx["XPP"]
        b[o:o + 8] = b"XPQ".ljust(8, b"\0")
        return bytes(b), "XPP"
    if kind == "reordered":
        o1, o2 = sh + 40 * idx["D3D"], sh + 40 * idx["D3DX"]
        b[o1:o1 + 40], b[o2:o2 + 40] = pe[o2:o2 + 40], pe[o1:o1 + 40]
        return bytes(b), "D3D"
    o = sh + 40 * idx["XMV"]
    struct.pack_into("<I", b, o + 8, _u32(pe, o + 8) + 0x40)
    return bytes(b), "XMV"


def self_test():
    res = []

    def want(label, ok, detail):
        res.append(ok)
        print(f"{'PASS' if ok else 'FAIL'} {label}: {detail}")

    pe, px = _proto_pair()
    out = write(pe, px)
    ok, n, s1, s2 = _report(out, px)
    d = compare(out, px)
    want("(a) prototype default.exe + default.xbe", not d and ok == n == 16 and s1 == s2,
         f"{ok}/{n} section digests, sha1 {s1} vs {s2}" + (f"; diffs {d}" if d else ""))
    cfg = vv_inputs.load_config()
    rx = vv_inputs.input_path("retail_xbe", cfg=cfg).read_bytes()
    pin = PIN.read_text(encoding="utf-8").split()[0].lower()
    if hashlib.sha1(rx).hexdigest() != pin:
        raise Refusal(f"retail XBE sha1 != pin {pin} ({PIN})")
    rout = write(_retail_pe(rx), rx)
    ok, n, s1, _ = _report(rout, rx)
    d = compare(rout, rx)
    want("(b) retail round-trip", not d and ok == n == 16 and s1 == pin,
         f"{ok}/{n} section digests, sha1 {s1} vs pin {pin}" + (f"; diffs {d}" if d else ""))
    ds = next(s for s in pe_info.parse_pe(pe)["sections"] if s["name"] == ".data")
    flip = bytearray(pe)
    flip[ds["rawptr"] + 0x100] ^= 0xFF
    fout = write(bytes(flip), px)
    d = compare(fout, px)
    fx, pxp = xbe_info.parse_xbe(fout), xbe_info.parse_xbe(px)
    changed = [s["name"] for s, t in zip(fx["sections"], pxp["sections"]) if s["digest"] != t["digest"]]
    want("(c) one byte flipped in .data at +0x100", changed == [".data"] and d == [".data digest"]
         and hashlib.sha1(fout).digest() != hashlib.sha1(px).digest(),
         f"digests changed {changed}; compare exit {1 if d else 0} names {d}")
    for kind in ("renamed", "reordered", "resized"):
        b, name = _pe_control(pe, kind)
        try:
            write(b, px)
            want(f"(d) {kind} {name}", False, "accepted (want exit 2)")
        except Refusal as e:
            want(f"(d) {kind} {name}", name in str(e), f"exit 2: {e}")
    print(f"{sum(res)} passed, {len(res) - sum(res)} failed of {len(res)}")
    return 0 if all(res) else 1


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("pe", nargs="?")
    ap.add_argument("--xbe")
    ap.add_argument("--out")
    ap.add_argument("--self-test", action="store_true")
    a = ap.parse_args(argv)
    try:
        if a.self_test:
            return self_test()
        if not (a.pe and a.xbe and a.out):
            raise Refusal("need <pe> --xbe <pin> --out <path>, or --self-test")
        out_path = Path(a.out).resolve()
        if OUT_ROOT.resolve() not in out_path.parents:
            raise Refusal(f"--out must lie under {OUT_ROOT}")
        pin = Path(a.xbe).read_bytes()
        out = write(Path(a.pe).read_bytes(), pin)
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_bytes(out)
        d = compare(out, pin)
        ok, n, s1, s2 = _report(out, pin)
        print(f"wrote {out_path} ({len(out)} bytes) sha1 {s1}; pin sha1 {s2}; {ok}/{n} section digests equal")
        for line in d:
            print(f"DIFF {line}")
        return 1 if d else 0
    except Refusal as e:
        print(f"xbe_write: {e}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
