"""MSVC 7.1 (cl 13.10) invocation helpers (1.5 T2; moved out of compiler_pin.py): per-call env, compile, preprocess.

Library only. `bindir` = config msvc71_dir (holds cl.exe; its parent is Vc7 with include/ and lib/).
compile_one(bindir, src, flags, obj, scratch=None) -> (ok, codes, text): `cl /nologo /c <flags> /Fo<obj> <src>`.
preprocess(bindir, src, flags, out, scratch=None) -> (ok, codes, text): `cl /nologo /E <flags> <src>`, stdout -> out.
ok = cl exit 0 and no `[CDL]####` message code (cl 13.10 exits 0 on unknown options: D4002 is caught only here,
C0026; cl's bare source-name echo skipped) and the output file exists. scratch = cwd and TMP/TEMP of the call
(default: the output file's parent); callers own and delete it. Subprocess list args, never a shell.
"""
import os
import re
import subprocess
from pathlib import Path

CODE_RE = re.compile(r"\b[CDL]\w*\d{4}\b")


def cl_env(bindir, scratch):
    vc7 = bindir.parent
    root = os.environ.get("SystemRoot", r"C:\Windows")
    return {"PATH": f"{bindir};{root}\\system32", "INCLUDE": str(vc7 / "include"), "LIB": str(vc7 / "lib"),
            "SystemRoot": root, "TMP": str(scratch), "TEMP": str(scratch)}


def _codes(text, src):
    return sorted({c for l in text.splitlines() if l.strip() != src.name for c in CODE_RE.findall(l)})


def compile_one(bindir, src, flags, obj, scratch=None):
    """-> (ok, codes, text). ok = cl exit 0 and no message code."""
    bindir, src, obj = Path(bindir), Path(src).resolve(), Path(obj).resolve()
    scratch = Path(scratch) if scratch else obj.parent
    r = subprocess.run([str(bindir / "cl.exe"), "/nologo", "/c", *flags, f"/Fo{obj}", str(src)], cwd=scratch,
                       env=cl_env(bindir, scratch), capture_output=True, text=True, errors="replace")
    text = r.stdout + r.stderr
    codes = _codes(text, src)
    return r.returncode == 0 and not codes and obj.is_file(), codes, f"cl exit {r.returncode}: {text.strip()}"


def preprocess(bindir, src, flags, out, scratch=None):
    """-> (ok, codes, text). Preprocessed source (stdout) written to `out`; codes from stderr only."""
    bindir, src, out = Path(bindir), Path(src).resolve(), Path(out).resolve()
    scratch = Path(scratch) if scratch else out.parent
    r = subprocess.run([str(bindir / "cl.exe"), "/nologo", "/E", *flags, str(src)], cwd=scratch,
                       env=cl_env(bindir, scratch), capture_output=True, text=True, errors="replace")
    out.write_text(r.stdout, encoding="utf-8")
    codes = _codes(r.stderr, src)
    return r.returncode == 0 and not codes, codes, f"cl exit {r.returncode}: {r.stderr.strip()}"
