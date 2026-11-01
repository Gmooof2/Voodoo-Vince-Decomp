"""XBE pristine check (T7, phase 1.1): section digests, header signature, source ISO format.

CLI: xbe_pristine.py [<xbe>] [--config P] [--manifest P] [--iso [P]]
     xbe_pristine.py --self-test   (byte-flipped copies under .run/1.1/: section body, cert title; deleted after)
Default xbe: config `retail_xbe`; ISO check default on (`retail_iso`, or --iso P).
Exit: 0 pristine yes; 1 pristine no; 2 refusal (no config/manifest, missing file, not an XBE).
Prints names, sizes, offsets and hashes only, never byte runs.
Digest forms: PHASE_PLAN Interfaces "Section digest" (Cxbx raw bytes, R1.1-034; wiki size-prefixed, R1.1-033).
"""
import hashlib
import json
import shutil
import struct
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))
import xbe_info  # noqa: E402

REDUMP = None  # no redump record available (redump unreachable, R1.1-033)
ISO_MAGIC = b"MICROSOFT*XBOX*MEDIA"
ISO_LAYOUTS = ((0, "trimmed XISO"), (0x18300000, "full image XGD1 game partition at 0x18300000"),
               (0x0FD90000, "full image XGD2 game partition at 0x0FD90000"),
               (0x02080000, "full image XGD3 game partition at 0x02080000"))
SELFTEST_DIR = REPO / ".run" / "1.1" / "xbe_pristine_selftest"
KEY_FILE = REPO / "config" / "xbox_retail_pubkey.json"  # gitignored; {source, exponent, modulus_size, modulus_hex LE}
# Cxbx EmuRsa.cpp RSApkcs1paddingtable (from xbedump xboxlib.c): [length, bytes...], matched at block[20:]
RSA_PKCS1_PADDING = (
    (0x0F, 0x14, 0x04, 0x00, 0x05, 0x1A, 0x02, 0x03, 0x0E, 0x2B, 0x05, 0x06, 0x09, 0x30, 0x21, 0x30),
    (0x0D, 0x14, 0x04, 0x1A, 0x02, 0x03, 0x0E, 0x2B, 0x05, 0x06, 0x07, 0x30, 0x1F, 0x30, 0x00, 0x00),
)


def section_digests(data, x):
    """Per section: (name, status) with status skipped|raw|size-prefixed|BAD."""
    out = []
    for s in x["sections"]:
        n = s["rawSize"]
        if n == 0:
            out.append((s["name"], "skipped"))
            continue
        body = data[s["rawAddr"]:s["rawAddr"] + n]
        a = hashlib.sha1(body).hexdigest()
        b = hashlib.sha1(struct.pack("<I", n) + body).hexdigest()
        out.append((s["name"], "raw" if a == s["digest"] else "size-prefixed" if b == s["digest"] else "BAD"))
    return out


def check_signature(data, x):
    """Header RSA signature: SHA1(u32le(sizeOfHeaders-0x104) + file[0x104:sizeOfHeaders]) as xbedump
    xboxlib.c shax() (the raw slice of Interfaces "Header signature" fails on retail, T7.c2),
    RSA public op and padding check as Cxbx EmuRsa.cpp xbox_rsa_public + verify_hash (R1.1-035).
    Key: gitignored KEY_FILE (xbedump xboxPublicKeyData[]); never printed."""
    if not KEY_FILE.is_file():
        return "not checked", "no sourced retail key"
    key = json.loads(KEY_FILE.read_text())
    mod = bytes.fromhex(key["modulus_hex"])
    msize = key["modulus_size"]
    body = data[0x104:x["header"]["sizeOfHeaders"]]
    h = hashlib.sha1(struct.pack("<I", len(body)) + body).digest()
    s = int.from_bytes(data[4:4 + 256], "little")
    blk = pow(s, key["exponent"], int.from_bytes(mod, "little")).to_bytes(256, "little")
    good = blk[:20] == h[::-1]
    zero = 20
    for pad in RSA_PKCS1_PADDING:
        if blk[20:20 + pad[0]] == pad[1:1 + pad[0]]:
            zero = 20 + pad[0]
            break
    good = (good and blk[zero] == 0 and blk[msize] == 0 and blk[msize - 1] == 1
            and all(b == 0xFF for b in blk[zero + 1:msize - 1]))
    if not good:
        return "BAD", None
    return "ok", f"key sha256 {key['modulus_sha256'][:16]}, source {key['source']}"


def check_iso(path):
    """(format label or 'unknown', size) reading only the magic windows."""
    size = path.stat().st_size
    with open(path, "rb") as f:
        for off, label in ISO_LAYOUTS:
            hits = []
            for o in (off + 0x10000, off + 0x10000 + 0x7EC):
                if o + len(ISO_MAGIC) > size:
                    break
                f.seek(o)
                hits.append(f.read(len(ISO_MAGIC)) == ISO_MAGIC)
            if len(hits) == 2 and all(hits):
                return label, size
    return "unknown", size


def run(xbe, cfg, records, iso):
    """Print the report; return 0 pristine yes, 1 no, 2 refusal."""
    if not xbe.is_file():
        print(f"xbe_pristine: no such file: {xbe}", file=sys.stderr)
        return 2
    data = xbe.read_bytes()
    try:
        x = xbe_info.parse_xbe(data)
    except xbe_info.NotXbe as e:
        print(f"xbe_pristine: not an XBE: {xbe}: {e}", file=sys.stderr)
        return 2
    sha1 = hashlib.sha1(data).hexdigest()
    status, rec = xbe_info.manifest_status(xbe, sha1, len(data), cfg, records)
    print(f"file {xbe}")
    print(f"sha1 {sha1} manifest {status}")
    ok, checks = True, []
    res = section_digests(data, x)
    for name, st in res:
        print(f"{name} " + ("skipped (rawSize 0)" if st == "skipped" else
                            "digest BAD" if st == "BAD" else f"digest ok ({st})"))
    forms = {st for _, st in res if st in ("raw", "size-prefixed")}
    if forms == {"raw"}:
        print("digest form: raw-bytes (Cxbx)")
    elif forms == {"size-prefixed"}:
        print("digest form: size-prefixed (xboxdevwiki)")
    else:
        print("digest form: mixed" if forms else "digest form: none matched")
        ok = False
    if any(st == "BAD" for _, st in res):
        ok = False
    checks.append(f"section-digests({sum(st != 'skipped' for _, st in res)})")
    sig, why = check_signature(data, x)
    print(f"signature {sig}" + (f" ({why})" if why else ""))
    checks.append("signature-skipped" if sig == "not checked" else "signature")
    if sig == "BAD":
        ok = False
    if iso is not None:
        if not iso.is_file():
            print(f"iso missing: {iso}")
            ok = False
        else:
            label, size = check_iso(iso)
            print(f"iso {iso} size {size} %2048={size % 2048} %2352={size % 2352}")
            print(f"iso format: {label}")
            r = next((r for r in records if r.get("role") == "retail_iso" and r.get("size") == size), None)
            print(f"iso sha1 {r['sha1'] if r else 'none'} (manifest record, not rehashed)")
            print(f"redump: not compared ({'no redump record available' if REDUMP is None else 'unused'})")
            if label == "unknown":
                ok = False
        checks.append("iso-format")
    print(f"pristine {'yes' if ok else 'no'}, checks run: {', '.join(checks)}")
    return 0 if ok else 1


def main(argv):
    if "--self-test" in argv:
        return self_test()
    import vv_inputs
    opts = ("--config", "--manifest", "--iso")
    pos = [a for i, a in enumerate(argv) if not a.startswith("--") and (i == 0 or argv[i - 1] not in opts)]
    if len(pos) > 1:
        print("usage: xbe_pristine.py [<xbe>] [--config P] [--manifest P] [--iso [P]] | --self-test",
              file=sys.stderr)
        return 2
    opt = lambda k: argv[argv.index(k) + 1] if k in argv and argv.index(k) + 1 < len(argv) else None
    cfg = vv_inputs.load_config(opt("--config"))
    records = vv_inputs.load_manifest(opt("--manifest"))
    xbe = Path(pos[0]) if pos else vv_inputs.input_path("retail_xbe", cfg=cfg)
    iso_arg = opt("--iso")
    iso = Path(iso_arg) if iso_arg and not iso_arg.startswith("--") else vv_inputs.input_path("retail_iso", cfg=cfg)
    return run(Path(xbe), cfg, records, Path(iso))


def self_test():
    import vv_inputs
    src = vv_inputs.input_path("retail_xbe")
    x = xbe_info.parse_xbe(Path(src).read_bytes())
    s = next(s for s in x["sections"] if s["rawSize"] > 0)
    title = x["header"]["certificateAddr"] - x["header"]["baseAddr"] + 0x0C  # cert titleName, inside 0x104..sizeOfHeaders
    cases = [(f"{s['name']} midpoint", s["rawAddr"] + s["rawSize"] // 2, f"{s['name']} digest BAD")]
    if KEY_FILE.is_file():
        cases.append(("certificate title", title, "signature BAD"))
    else:
        print("note: signature mutation skipped (no sourced retail key)")
    SELFTEST_DIR.mkdir(parents=True, exist_ok=True)
    fails = []
    try:
        for label, off, want in cases:
            cp = SELFTEST_DIR / "flipped.xbe"
            shutil.copyfile(src, cp)
            cp.chmod(0o644)
            d = bytearray(cp.read_bytes())
            d[off] ^= 0xFF
            cp.write_bytes(d)
            p = subprocess.run([sys.executable, __file__, str(cp)], capture_output=True, text=True)
            lines = p.stdout.splitlines()
            if p.returncode != 1:
                fails.append(f"{label}: exit {p.returncode}, want 1")
            if want not in lines:
                fails.append(f"{label}: '{want}' not reported")
            if not any(l.startswith("pristine no,") for l in lines):
                fails.append(f"{label}: final line not 'pristine no'")
    finally:
        shutil.rmtree(SELFTEST_DIR, ignore_errors=True)
    for f in fails:
        print(f"FAIL {f}")
    print(f"xbe_pristine self-test: {'OK' if not fails else 'FAIL'} (flipped {', '.join(c[0] for c in cases)})")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
