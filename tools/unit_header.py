#!/usr/bin/env python3
"""unit_header.py -- the `// UNIT:` line 1 of every source file under src (3.6 T6; CONVENTIONS §1).

Exact line 1 of every .c/.cpp: `// UNIT: src/<on-disk-case posix path under src> OBJ: <units.toml name> OPT: <opt>`.
Rows come from build.unit_map / build.flags_of (the same one-row rule as build.py:93); never objname (it holds a drive
letter). Work is in bytes: a file keeps its EOL.

--write: a file whose line 1 is not `// UNIT:` gains the line, and loses its leading prose block when the block's first
         line is `// <file>: unit ...`. A leading `//` block of any other first line is kept and listed. A file already
         headed has line 1 rewritten and nothing removed, so a second --write changes 0 files.
--check: exit 0 when all exact; exit 1 and a list on any stale, missing or unmappable file.
A file with several rows or none gets no header and makes the exit 1. A unit `name` holding a drive letter: exit 2.

--self-test: controls on a scratch copy under .run/3.6/unit_header_selftest/, removed at the end.
"""

import argparse
import contextlib
import io
import re
import shutil
import sys
import tomllib
from pathlib import Path

import build

REPO = Path(__file__).resolve().parent.parent
SCRATCH = REPO / ".run" / "3.6" / "unit_header_selftest"
UNIT = b"// UNIT:"
PROSE_FIRST = re.compile(rb"^// \S+: unit ")
# A leading block stops before a record line, a blank line or a non-comment line.
RECORD = re.compile(rb"^\s*//\s*(FUNCTION:|NAME:|PROTOTYPE:|!FAKE:|best:|src\s)")
DRIVE = re.compile(r"[A-Za-z]:")


def sources(src):
    return sorted((p for p in src.rglob("*") if p.is_file() and p.suffix.lower() in (".c", ".cpp")),
                  key=lambda p: p.relative_to(src).as_posix())


def unit_line(rel, u):
    """-> the exact line 1 (str); build.Refuse on a row that cannot give one."""
    name, opt = u.get("name"), " ".join(build.flags_of(u))
    if not name or re.search(r"\s", name):
        raise build.Refuse(f"unit {name!r}: name is not one token")
    return f"// UNIT: src/{rel} OBJ: {name} OPT: {opt}"


def leading_block(lines):
    """-> count of contiguous `//` lines from line 1, stopping before a record, blank or non-comment line."""
    n = 0
    for raw in lines:
        t = raw.rstrip(b"\r\n")
        if not t.startswith(b"//") or RECORD.match(t):
            break
        n += 1
    return n


def rewritten(data, line):
    """-> (new bytes, prose lines removed, head kept: a `//` block not removed as prose)."""
    lines = data.splitlines(keepends=True)
    first = lines[0] if lines else b""
    eol = b"\r\n" if first.endswith(b"\r\n") else b"\n"
    new = line.encode("utf-8") + eol
    if first.startswith(UNIT):
        return new + b"".join(lines[1:]), 0, False
    n = leading_block(lines)
    if n and PROSE_FIRST.match(first):
        return new + b"".join(lines[n:]), n, False
    return new + data, 0, n > 0


def run(src, units, write):
    """Prints the listing; -> exit code."""
    umap = build.unit_map(units)
    for rows in umap.values():
        for u in rows:
            if DRIVE.search(str(u.get("name", ""))):
                raise build.Refuse(f"unit name {u['name']!r} holds a drive letter")
    several, none, bad, stale, missing, heads = [], [], [], [], [], []
    changed = inserted = removed = 0
    srcs = sources(src)
    for p in srcs:
        rel = p.relative_to(src).as_posix()
        rows = umap.get(rel.lower(), [])
        if len(rows) != 1:  # the build.py:93 rule
            (several if rows else none).append(f"{rel}: {len(rows)} rows" if rows else rel)
            continue
        try:
            line = unit_line(rel, rows[0])
        except build.Refuse as e:
            bad.append(f"{rel}: {e}")
            continue
        data = p.read_bytes()
        first = (data.splitlines() or [b""])[0]
        if first == line.encode("utf-8"):
            continue
        if not write:
            (stale if first.startswith(UNIT) else missing).append(rel)
            continue
        new, n, kept = rewritten(data, line)
        p.write_bytes(new)
        changed += 1
        removed += n
        inserted += not first.startswith(UNIT)
        if kept:
            heads.append(rel)
    for label, items in (("several", several), ("none", none), ("bad row", bad), ("stale", stale),
                         ("missing", missing), ("head kept", heads)):
        for x in items:
            print(f"{label}: {x}")
    print(f"several: {len(several)}, none: {len(none)}")
    if write:
        print(f"files: {len(srcs)}, changed: {changed}, lines inserted: {inserted}, prose lines removed: {removed}, "
              f"leading blocks kept: {len(heads)}")
    fails = len(several) + len(none) + len(bad) + len(stale) + len(missing)
    print(f"unit_header: {'FAIL' if fails else 'PASS'} {fails} file(s) of {len(srcs)}")
    return 1 if fails else 0


def self_test(src, units):
    shutil.rmtree(SCRATCH, ignore_errors=True)
    dst = SCRATCH / "src"
    results = {}

    def go(write=False, u=None):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            try:
                rc = run(dst, u or uf, write)
            except build.Refuse:
                rc = 2
        return rc, out.getvalue()

    try:
        shutil.copytree(src, dst)
        uf = SCRATCH / "units.toml"
        text = Path(units).read_text(encoding="utf-8")
        uf.write_text(text, encoding="utf-8", newline="\n")
        umap = build.unit_map(uf)
        ps = sources(dst)
        target = next(p for p in ps if len(umap.get(p.relative_to(dst).as_posix().lower(), [])) == 1)
        rel = target.relative_to(dst).as_posix()
        row = umap[rel.lower()][0]
        toml_str = lambda s: s.replace("\\", "\\\\")

        def variant(name, body):
            v = SCRATCH / name
            v.write_text(body, encoding="utf-8", newline="\n")
            return v

        go(write=True)
        rc, _ = go()
        results["clean: exit 0"] = rc == 0
        rc, out = go(write=True)
        results["second --write changes 0 files"] = rc == 0 and "changed: 0," in out

        good = target.read_bytes()
        target.write_bytes(good.replace(b" OPT: ", b" OPT: /zz ", 1))
        rc, out = go()
        results["stale opt: exit 1 and listed"] = rc == 1 and f"stale: {rel}" in out

        target.write_bytes(good.split(b"\n", 1)[1])
        rc, out = go()
        results["missing line: exit 1 and listed"] = rc == 1 and f"missing: {rel}" in out

        head = (b"// x.cpp: unit y (units.toml index 1)\r\n// 0x74 int, more\r\n// !FAKE: keep\r\nint zz;\r\n")
        line = unit_line(rel, row).encode("utf-8")
        target.write_bytes(head)
        rc, _ = go(write=True)
        results["--write: prose head removed, !FAKE and CRLF kept"] = (
            rc == 0 and target.read_bytes() == line + b"\r\n// !FAKE: keep\r\nint zz;\r\n")

        target.write_bytes(b"// an unrelated note\nint zz;\n")
        rc, out = go(write=True)
        results["--write: other leading block kept and listed"] = (
            rc == 0 and f"head kept: {rel}" in out and target.read_bytes() == line + b"\n// an unrelated note\nint zz;\n")
        target.write_bytes(good)

        extra = (f'\n[[unit]]\nname = "zz_dup.obj"\nobjname = "{toml_str(row["objname"])}"\n'
                 f'language = "{row["language"]}"\nopt = "{row["opt"]}"\n')
        target.write_bytes(b"int zz;\n")
        rc, out = go(write=True, u=variant("several.toml", text + extra))
        results["several rows: exit 1, listed, no header"] = (
            rc == 1 and f"several: {rel}: 2 rows" in out and target.read_bytes() == b"int zz;\n")

        q = f'objname = "{toml_str(row["objname"])}"'
        rc, out = go(write=True, u=variant("none.toml", text.replace(q, 'objname = "d:\\\\beep\\\\zz_none.obj"', 1)))
        results["no row: exit 1, listed, no header"] = (
            rc == 1 and f"none: {rel}" in out and target.read_bytes() == b"int zz;\n")
        target.write_bytes(good)

        q = f'name = "{toml_str(row["name"])}"'
        body = re.sub(rf"^{re.escape(q)}$", lambda m: 'name = "C:\\\\zz\\\\zz.obj"', text, count=1, flags=re.M)
        rc, _ = go(u=variant("drive.toml", body))
        results["drive letter in a name: exit 2"] = rc == 2
    except StopIteration:
        raise build.Refuse("no source file with one row to plant in")
    finally:
        shutil.rmtree(SCRATCH, ignore_errors=True)
    if SCRATCH.exists():
        raise build.Refuse(f"scratch not deleted: {SCRATCH}")
    for k, ok in results.items():
        print(f"control {k}: {'failed as expected' if ok else 'DID NOT FAIL'}")
    n = sum(results.values())
    print(f"controls: {n} failed as expected of {len(results)}")
    return 0 if n == len(results) else 1


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    mode = ap.add_mutually_exclusive_group(required=True)
    mode.add_argument("--write", action="store_true")
    mode.add_argument("--check", action="store_true")
    mode.add_argument("--self-test", action="store_true")
    ap.add_argument("--src", default=str(REPO / "src"), metavar="DIR")
    ap.add_argument("--units", default=str(build.UNITS), metavar="P")
    a = ap.parse_args(argv)
    src = Path(a.src).resolve()
    try:
        if not src.is_dir():
            raise build.Refuse(f"missing src dir {src}")
        if a.self_test:
            return self_test(src, a.units)
        return run(src, a.units, a.write)
    except (build.Refuse, tomllib.TOMLDecodeError) as e:
        print(f"unit_header: REFUSED: {e}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
