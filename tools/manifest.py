"""Input hash manifest (T5): build | verify.

build  [--config P]                 hash every file of every configured input, write
                                    config/inputs.manifest.jsonl, its .sha1 sidecar and
                                    config/check.retail_xbe.sha; print count per role / files found.
verify [--config P] [--manifest P]  recompute every hash; exit 0 all match, 1 mismatch
                                    (hash, size, missing or extra file, pin), 2 refusal
                                    (missing/empty config or manifest, bad sidecar). A role whose key
                                    is null and in vv_inputs.OPTIONAL_PATHS is printed as not checked,
                                    named on the last line and not counted among the records.
Records: {"name","role","sha1","sha256","size"}; name relative to the role root (forward
slashes); BFM serialisation, sorted by (role, name). Prints names, sizes, hashes only.
"""
import hashlib
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import vv_inputs  # noqa: E402

REPO = vv_inputs.REPO
PIN = REPO / "config" / "check.retail_xbe.sha"
# role -> config key; a file key's root is its parent, name its basename
ROLES = (("retail_xbe", "retail_xbe"), ("retail_iso", "retail_iso"),
         ("prototype", "prototype_dir"), ("preview_iso", "preview_iso"),
         ("preview_cue", "preview_cue"), ("xbdm_dll", "xbdm_dll"))
CHUNK = 1 << 20


def hash_file(p):
    s1, s256, n = hashlib.sha1(), hashlib.sha256(), 0
    with open(p, "rb") as f:
        while b := f.read(CHUNK):
            s1.update(b)
            s256.update(b)
            n += len(b)
    return s1.hexdigest(), s256.hexdigest(), n


def role_files(cfg, key):
    """[(name, path)] for every file of the input; count found = len."""
    p = cfg[key]
    if p.is_file():
        return [(p.name, p)]
    return sorted((f.relative_to(p).as_posix(), f) for f in p.rglob("*") if f.is_file())


def scan(cfg):
    recs, found = [], {}
    for role, key in ROLES:
        files = role_files(cfg, key) if cfg.get(key) is not None else []  # optional (1.7 T3)
        found[role] = len(files)
        for name, p in files:
            h1, h256, n = hash_file(p)
            recs.append({"name": name, "role": role, "sha1": h1, "sha256": h256, "size": n})
    recs.sort(key=lambda r: (r["role"], r["name"]))
    return recs, found


def serialise(recs):
    return "".join(json.dumps(r, separators=(",", ":"), ensure_ascii=True, sort_keys=True) + "\n"
                   for r in recs)


def build(cfg):
    recs, found = scan(cfg)
    text = serialise(recs).encode("ascii")
    vv_inputs.MANIFEST.write_bytes(text)
    vv_inputs.MANIFEST_SHA1.write_bytes((hashlib.sha1(text).hexdigest() + "\n").encode("ascii"))
    xbe = [r for r in recs if r["role"] == "retail_xbe"]
    PIN.write_bytes(f"{xbe[0]['sha1']}  retail_xbe\n".encode("ascii"))
    for role, _ in ROLES:
        n = sum(1 for r in recs if r["role"] == role)
        print(f"{role}: {n} recorded / {found[role]} files found")
    print(f"manifest: {len(recs)} records -> {vv_inputs.MANIFEST.relative_to(REPO).as_posix()}")
    print(f"pin: retail_xbe {xbe[0]['sha1']}")
    return 0


def verify(cfg, manifest):
    # 3.6 T14.1: only a null key in OPTIONAL_PATHS skips its role; the records are named, never claimed as checked
    skip = {role for role, key in ROLES if key in vv_inputs.OPTIONAL_PATHS and cfg.get(key) is None}
    skipped = {role: sum(1 for r in manifest if r["role"] == role) for role, _ in ROLES if role in skip}
    want = {(r["role"], r["name"]): r for r in manifest if r["role"] not in skip}
    recs, found = scan(cfg)
    got = {(r["role"], r["name"]): r for r in recs}
    bad = 0
    for k in sorted(want.keys() | got.keys()):
        w, g = want.get(k), got.get(k)
        if w is None:
            print(f"EXTRA {k[0]}/{k[1]} (on disk, not in manifest)")
        elif g is None:
            print(f"MISSING {k[0]}/{k[1]}")
        elif any(w[f] != g[f] for f in ("sha1", "sha256", "size")):
            print(f"MISMATCH {k[0]}/{k[1]} size {w['size']}->{g['size']} sha1 {w['sha1']}->{g['sha1']}")
        else:
            continue
        bad += 1
    pin = PIN.read_text(encoding="ascii").strip() if PIN.is_file() else ""
    xbe = [r["sha1"] for r in manifest if r["role"] == "retail_xbe"]
    if not xbe or pin != f"{xbe[0]}  retail_xbe":
        print(f"PIN mismatch: {PIN.name} '{pin}' vs manifest retail_xbe {xbe[:1]}")
        bad += 1
    for role, _ in ROLES:
        n = sum(1 for r in manifest if r["role"] == role)
        if role in skip:
            print(f"{role}: {n} in manifest, not checked (key is null)")
        else:
            print(f"{role}: {n} in manifest / {found[role]} files found")
    tail = f"; not checked: {', '.join(f'{r} {n}' for r, n in skipped.items())}" if skip else ""
    print(f"verify: {'OK' if not bad else 'FAIL'} — {bad} problem(s) among {len(want)} records{tail}")
    return 1 if bad else 0


def main(argv):
    if not argv or argv[0] not in ("build", "verify"):
        print(__doc__)
        return 2
    opt = lambda f: argv[argv.index(f) + 1] if f in argv else None  # noqa: E731
    cfg = vv_inputs.load_config(opt("--config"))
    if argv[0] == "build":
        return build(cfg)
    return verify(cfg, vv_inputs.load_manifest(opt("--manifest")))


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
