"""Local input paths for Voodoo Vince tools (T13; manifest added in T5).

Reads config/local_paths.json (gitignored; template config/local_paths.example.json).
Paths are absolute or relative to game_dir. Any refusal exits 2 with a message.
CLI: vv_inputs.py [--config PATH]  -> prints each key and its resolved path, exit 0.
"""
import json
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
CONFIG = REPO / "config" / "local_paths.json"
MANIFEST = REPO / "config" / "inputs.manifest.jsonl"
MANIFEST_SHA1 = REPO / "config" / "inputs.manifest.sha1"
PATH_KEYS = ("game_dir", "retail_xbe", "retail_iso", "prototype_dir",
             "preview_iso", "preview_cue", "ghidra_project_dir",
             "ghidra_dir", "java_home", "objdiff_cli")  # toolchain (1.3 T4)
NULLABLE = ("xbox_ip",)  # null, one address, or (identify only) a list of two (1.7 T3)
OPTIONAL_PATHS = ("msvc71_dir", "xbdm_dll")  # may be absent or null; a string must exist (1.4 T3; xbdm_dll 1.7 T3)
KEYS = PATH_KEYS + NULLABLE + OPTIONAL_PATHS


def _refuse(msg):
    print(f"vv_inputs: {msg}", file=sys.stderr)
    sys.exit(2)


def _resolve(cfg, key):
    p = Path(cfg[key])
    return p if p.is_absolute() or key == "game_dir" else Path(cfg["game_dir"]) / p


def load_config(path=None):
    """Return the config with every path key resolved to an existing Path; exit 2 otherwise."""
    path = Path(path) if path else CONFIG
    hint = f"copy {REPO / 'config' / 'local_paths.example.json'} to {CONFIG} and fill it"
    if not path.is_file():
        _refuse(f"config missing: {path} ({hint})")
    text = path.read_text(encoding="utf-8").strip()
    if not text:
        _refuse(f"config empty: {path} ({hint})")
    try:
        cfg = json.loads(text)
    except json.JSONDecodeError as e:
        _refuse(f"config not valid JSON: {path}: {e}")
    if not isinstance(cfg, dict):
        _refuse(f"config is not a JSON object: {path}")
    missing = [k for k in KEYS if k not in cfg and k not in OPTIONAL_PATHS]
    if missing:
        _refuse(f"config {path} missing key(s): {', '.join(missing)}")
    for k in PATH_KEYS:
        if not isinstance(cfg[k], str) or not cfg[k]:
            _refuse(f"config {path} key {k} must be a non-empty path string")
    if not Path(cfg["game_dir"]).is_absolute():
        _refuse(f"config {path} key game_dir must be absolute: {cfg['game_dir']}")
    out = dict(cfg)
    for k in PATH_KEYS:
        p = _resolve(cfg, k)
        if not p.exists():
            _refuse(f"config {path} key {k}: path does not exist: {p}")
        out[k] = p
    for k in OPTIONAL_PATHS:
        v = cfg.get(k)
        if v is None:
            out[k] = None
            continue
        if not isinstance(v, str) or not v:
            _refuse(f"config {path} key {k} must be null or a non-empty path string")
        p = _resolve(cfg, k)
        if not p.exists():
            _refuse(f"config {path} key {k}: path does not exist: {p}")
        out[k] = p
    return out


def input_path(role, name=None, cfg=None):
    """Path of a configured input; name joins a file under a directory role (e.g. prototype_dir)."""
    cfg = cfg or load_config()
    if role not in PATH_KEYS + OPTIONAL_PATHS:
        _refuse(f"unknown input role: {role} (known: {', '.join(PATH_KEYS + OPTIONAL_PATHS)})")
    if cfg.get(role) is None:
        _refuse(f"input {role} is not set in the config")
    p = cfg[role] / name if name else cfg[role]
    if not p.exists():
        _refuse(f"input {role}{'/' + name if name else ''} does not exist: {p}")
    return p


def load_manifest(path=None):
    """Records of config/inputs.manifest.jsonl; exit 2 on missing/empty manifest or bad .sha1 sidecar."""
    import hashlib
    path = Path(path) if path else MANIFEST
    if not path.is_file():
        _refuse(f"manifest missing: {path} (run tools/manifest.py build)")
    data = path.read_bytes()
    if not data.strip():
        _refuse(f"manifest empty: {path}")
    side = path.with_suffix(".sha1")
    want = side.read_text(encoding="ascii").strip() if side.is_file() else ""
    if want != hashlib.sha1(data).hexdigest():
        _refuse(f"manifest sidecar {side} missing or does not match {path}")
    try:
        return [json.loads(line) for line in data.decode("ascii").splitlines() if line]
    except (UnicodeDecodeError, json.JSONDecodeError) as e:
        _refuse(f"manifest not valid JSON lines: {path}: {e}")


def main(argv):
    path = argv[argv.index("--config") + 1] if "--config" in argv else None
    cfg = load_config(path)
    for k in KEYS:
        print(f"{k}: {cfg[k]}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
