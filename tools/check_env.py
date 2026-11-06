"""Environment check for the matching toolchain (1.4 T3).

Usage: check_env.py [--pin] [--pins FILE] [--msvc-dir DIR] [--config P]
Prints one line per check (`ok <check>: ...` or `FAIL <check>: <cause>`), every failure reported.
Checks: python 3.15; git core.hooksPath == githooks (in this script's repo); ghidra_dir, java_home,
objdiff_cli exist; msvc71_dir set and exists (or --msvc-dir); pins file (default config/toolchain.sha256)
exists, non-empty, each line `<sha256>  <relpath>` names a file under the msvc dir with that sha256; then (4.5 T16.1)
iced_x86: the imported module is a file of the iced-x86 ICED_X86 distribution; objdiff_cli: its basename and sha256
== config/objdiff_cli.sha256 (`objdiff_cli: sha mismatch` / `pin missing`).
--pin: hash the PIN_NAMES present directly in the msvc dir, write the pins file (sha256sum format,
sorted by path, LF), then run every check.
Exit: 0 every check passed; 1 any check failed.
"""
import argparse
import hashlib
import os
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import vv_inputs  # noqa: E402

REPO = Path(__file__).resolve().parent.parent
PINS = REPO / "config" / "toolchain.sha256"
PIN_NAMES = ("cl.exe", "c1.dll", "c1xx.dll", "c2.dll", "link.exe", "mspdb71.dll", "msobj71.dll",
             "msvcr71.dll", "msvcp71.dll")
PIN_LINE = re.compile(r"^([0-9a-f]{64})  (\S.*)$")
OBJDIFF_PIN = REPO / "config" / "objdiff_cli.sha256"  # 4.5 T16.1 G4: `<sha256>  <basename>` of the objdiff-cli binary
ICED_X86 = "1.21.0"  # 4.5 T16.1 O15: the iced-x86 distribution requirements.txt pins


def sha256(p):
    h = hashlib.sha256()
    with open(p, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def write_pins(msvc, pins):
    """Write the pins file from the PIN_NAMES present in msvc; return a FAIL cause or None."""
    if msvc is None or not msvc.is_dir():
        return f"msvc dir not set or not a directory: {msvc}"
    present = sorted(n for n in PIN_NAMES if (msvc / n).is_file())
    if not present:
        return f"none of {', '.join(PIN_NAMES)} in {msvc}"
    pins.parent.mkdir(parents=True, exist_ok=True)
    pins.write_bytes("".join(f"{sha256(msvc / n)}  {n}\n" for n in present).encode("ascii"))
    return None


def check_pins(msvc, pins):
    """Return a list of FAIL causes for the pins file against msvc; empty when every pin holds."""
    if not pins.is_file():
        return [f"pins file missing: {pins}"]
    lines = pins.read_bytes().decode("ascii", "replace").splitlines()
    if not any(lines):
        return [f"pins file empty: {pins}"]
    if msvc is None or not msvc.is_dir():
        return [f"cannot verify {pins}: msvc dir not set or not a directory: {msvc}"]
    out = []
    for i, line in enumerate(lines, 1):
        m = PIN_LINE.match(line)
        if not m:
            out.append(f"{pins}:{i}: not `<sha256>  <relpath>`: {line!r}")
            continue
        f = msvc / m.group(2)
        if not f.is_file():
            out.append(f"{m.group(2)}: missing ({f})")
        elif sha256(f) != m.group(1):
            out.append(f"{m.group(2)}: sha mismatch ({f})")
    return out


def check_objdiff_pin(exe, pin=OBJDIFF_PIN):
    """-> FAIL cause or None: exe's basename and sha256 against the one-line pin file (4.5 T16.1 G4)."""
    if not pin.is_file():
        return f"pin missing ({pin})"
    lines = pin.read_bytes().decode("ascii", "replace").splitlines()
    m = PIN_LINE.match(lines[0]) if len(lines) == 1 else None
    if not m:
        return f"pin file not one `<sha256>  <basename>` line: {pin}"
    if m.group(2) != exe.name:
        return f"pin names {m.group(2)}, config names {exe.name}"
    if not exe.is_file():
        return f"missing ({exe})"
    return None if sha256(exe) == m.group(1) else f"sha mismatch ({exe})"


def check_iced():
    """-> FAIL cause or None: the imported iced_x86 is the pinned iced-x86 distribution (4.5 T16.1 O15)."""
    try:
        import importlib.metadata
        import iced_x86
        dist = importlib.metadata.distribution("iced-x86")
    except (ImportError, importlib.metadata.PackageNotFoundError) as e:
        return f"not importable: {e}"
    if dist.version != ICED_X86:
        return f"version {dist.version}, want {ICED_X86}"
    f = Path(iced_x86.__file__).resolve()
    if not any(Path(p.locate()).resolve() == f for p in dist.files or ()):
        return f"imported {f} is not a file of the iced-x86 {dist.version} distribution"
    return None


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--pin", action="store_true")
    ap.add_argument("--pins", type=Path, default=PINS)
    ap.add_argument("--msvc-dir", type=Path)
    ap.add_argument("--config")
    a = ap.parse_args(argv)
    fails = 0

    def report(check, cause, okmsg=""):
        nonlocal fails
        if cause:
            fails += 1
            print(f"FAIL {check}: {cause}")
        else:
            print(f"ok {check}: {okmsg}")

    v = sys.version_info[:2]
    report("python", None if v == (3, 15) else f"version {v[0]}.{v[1]}, want 3.15", f"{v[0]}.{v[1]}")

    env = {k: val for k, val in os.environ.items() if not k.startswith("GIT_")}
    try:
        r = subprocess.run(["git", "config", "--get", "core.hooksPath"], cwd=REPO, env=env,
                           capture_output=True, text=True)
        hp = r.stdout.strip()
        cause = None if r.returncode == 0 and hp == "githooks" else \
            f"core.hooksPath is {hp!r} (git exit {r.returncode}), want 'githooks'"
    except OSError as e:
        hp, cause = "", f"git not runnable: {e}"
    report("core.hooksPath", cause, hp)

    cfg = None
    try:
        cfg = vv_inputs.load_config(a.config)
        report("config", None, "loaded")
    except SystemExit:
        report("config", "vv_inputs.load_config refused (message above)")
    for k in ("ghidra_dir", "java_home", "objdiff_cli"):
        if cfg is None:
            report(k, "config not loaded")
        else:
            report(k, None if cfg[k].exists() else f"does not exist: {cfg[k]}", str(cfg[k]))

    if a.msvc_dir is not None:
        msvc = a.msvc_dir.resolve()
        report("msvc71_dir", None if msvc.is_dir() else f"--msvc-dir not a directory: {msvc}", f"{msvc} (--msvc-dir)")
    elif cfg is None:
        msvc = None
        report("msvc71_dir", "config not loaded")
    else:
        msvc = cfg["msvc71_dir"]
        report("msvc71_dir", "unset (null or absent in config)" if msvc is None else
               (None if msvc.is_dir() else f"not a directory: {msvc}"), str(msvc))

    pins = a.pins if a.pins.is_absolute() else Path.cwd() / a.pins
    if a.pin:
        cause = write_pins(msvc, pins)
        report("pin", cause, f"wrote {pins}")
    causes = check_pins(msvc, pins)
    for c in causes:
        report("pins", c)
    if not causes:
        report("pins", None, f"{pins} holds")
    # 4.5 T16.1 G4/O15: last, so a FAIL stays in the gate's 3-line check_env tail (was: no objdiff or iced pin)
    report("iced_x86", check_iced(), ICED_X86)
    report("objdiff_cli", "config not loaded" if cfg is None else check_objdiff_pin(cfg["objdiff_cli"]),
           f"{OBJDIFF_PIN.name} holds")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
