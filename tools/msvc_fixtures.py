"""Fixture builder (T5, phase 1.2): builds our tiny C/C++ fixture with the fixture compiler (VS Build Tools).

CLI: msvc_fixtures.py [--out DIR]     default DIR = .run/<open phase>/fixtures (no open plan: .run/fixtures)
Builds tests/fixtures/msvc/fix_c.c + fix_cpp.cpp into one exe twice:
  plain: cl /Zi /O2,     link /DEBUG
  ltcg:  cl /Zi /O2 /GL, link /DEBUG /LTCG
via a generated .bat that calls vcvarsall.bat x86 then cl/link by full path (Git's usr\\bin\\link.exe shadows
MSVC's). Writes DIR/versions.json (cl and link banner versions). Test fixtures only, never the matching
compiler; outputs stay under .run/ (never committed). Prints paths, versions and counts only.
Exit: 0 built; 1 a build failed; 2 refusal (no vswhere / no Build Tools / no toolset).
Importable: find_toolset() -> dict | None, build(out_dir) -> dict (raises BuildError).
"""
import json
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SRC = REPO / "tests" / "fixtures" / "msvc"
SOURCES = ("fix_c.c", "fix_cpp.cpp")
EXE = "fixture.exe"


def _default_out():
    """.run/<phase>/fixtures for the open plan's `# Phase <id>` heading (as tools/run.sh), else .run/fixtures."""
    plan = REPO / "phase-ends" / "current" / "PHASE_PLAN.md"
    if plan.is_file():
        for line in plan.read_text(encoding="utf-8").splitlines():
            m = re.match(r"# Phase (\S+)", line)
            if m:
                return REPO / ".run" / m.group(1) / "fixtures"
    return REPO / ".run" / "fixtures"


DEFAULT_OUT = _default_out()  # was REPO / ".run" / "1.2" / "fixtures" (T14, 1.2)
VSWHERE = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe")
VARIANTS = (("plain", "/Zi /O2", "/DEBUG"), ("ltcg", "/Zi /O2 /GL", "/DEBUG /LTCG"))
CL_RE = re.compile(r"C/C\+\+ Optimizing Compiler Version (\d+)\.(\d+)\.(\d+)(?:\.(\d+))?")
LINK_RE = re.compile(r"Incremental Linker Version (\d+)\.(\d+)\.(\d+)(?:\.(\d+))?")


class BuildError(Exception):
    pass


def find_toolset():
    """vswhere -> install path, vcvarsall, newest MSVC toolset bin\\Hostx86\\x86. None if absent."""
    if not VSWHERE.is_file():
        return None
    r = subprocess.run([str(VSWHERE), "-products", "*", "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
                        "-latest", "-property", "installationPath"], capture_output=True, text=True)
    lines = [l.strip() for l in r.stdout.splitlines() if l.strip()]
    if r.returncode != 0 or not lines:
        return None
    inst = Path(lines[0])
    vcvars = inst / "VC" / "Auxiliary" / "Build" / "vcvarsall.bat"
    msvc = inst / "VC" / "Tools" / "MSVC"
    vers = sorted((d for d in msvc.iterdir() if d.is_dir()),
                  key=lambda d: tuple(int(p) if p.isdigit() else 0 for p in d.name.split("."))) if msvc.is_dir() else []
    if not vcvars.is_file() or not vers:
        return None
    bindir = vers[-1] / "bin" / "Hostx86" / "x86"
    cl, link = bindir / "cl.exe", bindir / "link.exe"
    if not cl.is_file() or not link.is_file():
        return None
    return {"install": str(inst), "vcvarsall": str(vcvars), "toolset": vers[-1].name, "cl": str(cl), "link": str(link)}


def _ver(rx, text):
    m = rx.search(text)
    if not m:
        return None
    parts = [int(p) for p in m.groups() if p is not None]
    return {"version": ".".join(m.group(i) for i in range(1, 5) if m.group(i) is not None),
            "major": parts[0], "minor": parts[1], "build": parts[2]}


def build(out_dir=DEFAULT_OUT):
    """Build both variants under out_dir; returns {"toolset", "cl", "link", "variants": {name: exe}}."""
    ts = find_toolset()
    if ts is None:
        raise BuildError("no VS Build Tools with the x86 C++ toolset (vswhere)")
    out = Path(out_dir).resolve()
    out.mkdir(parents=True, exist_ok=True)
    res = {"toolset": ts["toolset"], "install": ts["install"], "variants": {}}
    srcs = " ".join(f'"{SRC / s}"' for s in SOURCES)
    banners = ""
    for name, cflags, lflags in VARIANTS:
        vdir = out / name
        vdir.mkdir(parents=True, exist_ok=True)
        for old in vdir.iterdir():
            if old.is_file():
                old.unlink()
        bat = out / f"build_{name}.bat"
        bat.write_text("@echo off\r\n"
                       f'call "{ts["vcvarsall"]}" x86 >nul 2>&1\r\n'
                       f'cd /d "{vdir}"\r\n'
                       f'"{ts["cl"]}" /c {cflags} {srcs} 2>&1\r\n'
                       "if errorlevel 1 exit /b 1\r\n"
                       f'"{ts["link"]}" {lflags} /OUT:{EXE} fix_c.obj fix_cpp.obj 2>&1\r\n'
                       "if errorlevel 1 exit /b 1\r\n"
                       "exit /b 0\r\n", encoding="ascii")
        r = subprocess.run(["cmd", "/c", str(bat)], capture_output=True, text=True, errors="replace")
        (out / f"build_{name}.log").write_text(r.stdout + r.stderr, encoding="utf-8")
        exe = vdir / EXE
        if r.returncode != 0 or not exe.is_file():
            raise BuildError(f"{name}: build failed (exit {r.returncode}); log {out / f'build_{name}.log'}")
        res["variants"][name] = str(exe)
        banners += r.stdout + r.stderr
    # without /nologo both cl /c and link print their banner during the build
    res["cl"] = _ver(CL_RE, banners)
    res["link"] = _ver(LINK_RE, banners)
    if res["cl"] is None or res["link"] is None:
        raise BuildError(f"banner not parsed (cl {res['cl']}, link {res['link']})")
    (out / "versions.json").write_text(json.dumps(res, indent=1, sort_keys=True) + "\n", encoding="utf-8")
    return res


def main(argv):
    if "--help" in argv or "-h" in argv:
        print(__doc__)
        return 0
    out = Path(argv[argv.index("--out") + 1]) if "--out" in argv else DEFAULT_OUT
    if find_toolset() is None:
        print("msvc_fixtures: no VS Build Tools with the x86 C++ toolset found (vswhere)", file=sys.stderr)
        return 2
    try:
        res = build(out)
    except BuildError as e:
        print(f"msvc_fixtures: FAIL — {e}", file=sys.stderr)
        return 1
    print(f"toolset {res['toolset']}; cl {res['cl']['version']}; link {res['link']['version']}")
    for name, exe in res["variants"].items():
        print(f"  {name}: {exe} ({Path(exe).stat().st_size} bytes)")
    print(f"msvc_fixtures: OK — versions {Path(out).resolve() / 'versions.json'}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
