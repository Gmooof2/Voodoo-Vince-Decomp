"""Compiler pin (T8, phase 1.4): compile config/compiler_pin.toml test sources with the matching cl, byte-compare.

CLI: compiler_pin.py [--control] [--only NAME] [--self-test]
Runs `PY tools/check_env.py` first; any failure refuses (exit 2). Per [[function]] with `source` (repo-relative):
  <msvc71_dir>/cl.exe /nologo /c <flags> /Fo<.run/<phase>/pin/<i>.obj> <source>
with a per-call env (PATH=<bin>;%SystemRoot%\\system32, INCLUDE=<Vc7>\\include, LIB=<Vc7>\\lib, TMP=pin dir), then
`byte_compare.py --obj O --symbol S --va X --symbols config/retail/symbols.txt`. An entry matches only when cl exits
0, its output holds no `[CDL]####` code (D31; cl's bare source-name echo skipped) and byte_compare exits 0.
Prints one verdict line per entry and `N matched of N` (`, K pending` = entries without source, not counted).
--control: each entry's flags with token control[0] replaced by control[1]; passes when byte_compare says mismatch.
--self-test: the first sourced entry with `/Zq9` appended must fail with D4002 extracted.
Exit: 0 all match (--control: every control mismatches; --self-test: D4002 caught); 1 otherwise;
2 refusal (check_env fails, missing toml/source/msvc71_dir, unknown --only, bad control). Scratch .run/<phase>/pin
is deleted on exit.
"""
import argparse
import shutil
import subprocess
import sys
import tomllib
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import byte_compare  # noqa: E402
import vv_inputs  # noqa: E402
from msvc71 import compile_one  # noqa: E402  (1.5 T2: cl_env/compile_one/CODE_RE moved to tools/msvc71.py)

REPO = vv_inputs.REPO
TOML = REPO / "config" / "compiler_pin.toml"
SYMBOLS = REPO / "config" / "retail" / "symbols.txt"
SYMBOLS_PIN = REPO / "config" / "retail" / "symbols_pin.txt"  # T9: runtime reloc targets (EH), hand-kept
PIN_DIR = byte_compare._phase_run_dir() / "pin"


class Refuse(Exception):
    pass


def check_env():
    r = subprocess.run([sys.executable, str(REPO / "tools" / "check_env.py")], capture_output=True, text=True,
                       errors="replace")
    if r.returncode != 0:
        fails = [l for l in (r.stdout + r.stderr).splitlines() if not l.startswith("ok ")]
        raise Refuse(f"check_env exit {r.returncode}: " + "; ".join(fails))


def compare_one(obj, fn):
    """-> (exit code, first stdout line or stderr)."""
    r = subprocess.run([sys.executable, str(REPO / "tools" / "byte_compare.py"), "--obj", str(obj),
                        "--symbol", fn["symbol"], "--va", f"{fn['va']:08X}", "--symbols", str(SYMBOLS),
                        *([str(SYMBOLS_PIN)] if SYMBOLS_PIN.is_file() else [])],
                       capture_output=True, text=True, errors="replace")
    if r.returncode not in (0, 1):
        return r.returncode, r.stderr.strip() or f"byte_compare exit {r.returncode}"
    return r.returncode, " | ".join(l.strip() for l in r.stdout.splitlines() if l.strip())


def flags_of(fn, control):
    toks = fn.get("flags", "").split()
    if not control:
        return toks
    ctl = fn.get("control")
    if not (isinstance(ctl, list) and len(ctl) == 2):
        raise Refuse(f"{fn['name']}: control must be [\"<tok>\", \"<flipped>\"]")
    if ctl[0] not in toks:
        raise Refuse(f"{fn['name']}: control token {ctl[0]} not in flags {fn.get('flags', '')}")
    return [ctl[1] if t == ctl[0] else t for t in toks]


def run(a):
    check_env()
    print("check_env: ok")
    cfg = vv_inputs.load_config()
    bindir = cfg.get("msvc71_dir")
    if bindir is None or not (Path(bindir) / "cl.exe").is_file():
        raise Refuse(f"msvc71_dir unset or has no cl.exe: {bindir}")
    bindir = Path(bindir)
    if not TOML.is_file():
        raise Refuse(f"missing {TOML}")
    fns = tomllib.loads(TOML.read_text(encoding="utf-8")).get("function", [])
    if a.only:
        fns = [f for f in fns if f["name"] == a.only]
        if not fns:
            raise Refuse(f"--only {a.only}: no such [[function]]")
    sourced = [f for f in fns if f.get("source")]
    pending = len(fns) - len(sourced)
    for f in sourced:
        if not (REPO / f["source"]).is_file():
            raise Refuse(f"{f['name']}: source missing: {f['source']}")
    if a.self_test:
        if not sourced:
            raise Refuse("--self-test: no sourced entry")
        sourced = sourced[:1]
    if PIN_DIR.exists():
        shutil.rmtree(PIN_DIR)
    PIN_DIR.mkdir(parents=True)
    good = 0
    caught = False
    for i, f in enumerate(sourced):
        flags = flags_of(f, a.control) + (["/Zq9"] if a.self_test else [])
        src, obj = REPO / f["source"], PIN_DIR / f"{i}.obj"
        ok, codes, text = compile_one(bindir, src, flags, obj, scratch=PIN_DIR)
        if not ok:
            caught |= "D4002" in codes
            print(f"FAIL {f['name']} [{' '.join(flags)}]: codes {' '.join(codes) or '-'}; {text}")
            continue
        rc, line = compare_one(obj, f)
        if a.control:
            passed = rc == 1 and "verdict mismatch" in line
            print(f"{'ok' if passed else 'FAIL'} control {f['name']} [{' '.join(flags)}]: {line}")
        else:
            passed = rc == 0
            print(f"{'match' if passed else 'FAIL'} {f['name']} [{' '.join(flags)}]: {line}")
        good += passed
    tail = f", {pending} pending" if pending else ""
    if a.self_test:
        print(f"self-test: D4002 {'caught' if caught else 'NOT caught'}")
        return 0 if caught else 1
    print(f"{good} {'controls mismatched' if a.control else 'matched'} of {len(sourced)}{tail}")
    return 0 if sourced and good == len(sourced) else 1


def main(argv):
    ap = argparse.ArgumentParser(prog="compiler_pin.py", description=__doc__.splitlines()[0])
    ap.add_argument("--control", action="store_true", help="flip each entry's control flag; expect mismatch")
    ap.add_argument("--only", metavar="NAME", help="only the [[function]] with this name")
    ap.add_argument("--self-test", action="store_true", help="append /Zq9 to the first entry; expect D4002")
    a = ap.parse_args(argv)
    try:
        return run(a)
    except Refuse as e:
        print(f"compiler_pin: REFUSED {e}", file=sys.stderr)
        return 2
    finally:
        if PIN_DIR.exists():
            shutil.rmtree(PIN_DIR, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
