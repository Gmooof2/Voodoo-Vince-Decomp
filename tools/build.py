"""Build (1.5 T2): compile every .c/.cpp under a source root, one obj per config/retail/units.toml unit, with its `opt`.

CLI: build.py --src DIR --out DIR [--config P]
Path rule (src/README.md): unit objname minus the `d:\\beep\\` prefix, `\\Xbox\\OfficialRelease\\` dropped, `.obj` ->
`.c`/`.cpp` by `language` ("c"/"c++"), `\\` -> `/`, under the source root; matched case-insensitively. Units outside
`d:\\beep\\` have no source path. Only .c/.cpp files are sources; other files are ignored.
Each source: msvc71.compile_one(<msvc71_dir>, src, <unit opt split>, <out>/<rel source path minus ext>.obj (dirs
created), scratch=<out>) (5.1 T4.c2; was <out>/<objname basename>: two units sharing a basename were refused).
No per-function flag override. msvc71_dir from vv_inputs.load_config(--config).
Exit: 0 built (0 units allowed; prints counts); 1 any compile failure or any C/D/L message code (D26);
2 refusal (stderr `build: REFUSED ...`): a source maps to no unit (or to two), unit opt "unknown" or not a flags
string, an opt token steering includes or macros (`/FI /I /X /u /D /U`, `-` forms too; 4.5 T16.2), duplicate obj output path
(case-insensitive; was: duplicate obj name), out dir not under REPO/.run/, missing src root/units/msvc71_dir.
The out dir is wiped and recreated each run (after the refusal checks).
Library: build(src_root, out_dir, units_path=UNITS, config=None, workers=None) -> [{unit, source, obj, flags}] in
source order; raises Refuse (exit 2) or BuildFailed (exit 1, after every source was tried; failures in source order).
5.4 T5.c4: the compiles run in a thread pool of `workers` (default pool_size(): memory-sized), each in its own
scratch dir <out>/.scratch-<i> (cwd and TMP of its cl; removed after); results kept in source order whatever the
completion order. main records a timing row (step `build`, label = the phase id); stdout unchanged.
"""
import argparse
import re
import shutil
import sys
import tomllib
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import msvc71  # noqa: E402
import timing  # noqa: E402  5.4 T5.c4: free_mb sizes the pool; step records main
import vv_inputs  # noqa: E402

REPO = vv_inputs.REPO
UNITS = REPO / "config" / "retail" / "units.toml"
RUN = REPO / ".run"
PREFIX = "d:\\beep\\"
MIDDLE = "\\xbox\\officialrelease\\"
EXT = {"c": ".c", "c++": ".cpp"}
STEER = ("FI", "I", "X", "u", "D", "U")  # opt tokens refused after `/` or `-` (4.5 T16.2 G18), case-sensitive
# 5.4 T5.c4: pool size W = clamp((free_mb - RESERVE_MB) // PER_JOB_MB, 1, CAP); 1 when free_mb is unknown.
# Evidence: phase-ends/current/logs/T5.c4.md.
RESERVE_MB = 6144  # free memory left to the machine (as gate_controls.RESERVE_MB)
PER_JOB_MB = 32  # one cl.exe (c1xx/c2 in-process): peak working set max 15 MB, median 9 MB over the 216 src sources
CAP = 8  # was: 1 (serial loop). Build wall (216 src) at CAP 2/4/8/16: in the gate, its lanes running,
# 6.39/3.60/2.86/2.45 s (gate total flat ~60 s: the audit lane bounds it); alone 3.5/2.0/1.3/1.3 s


class Refuse(Exception):
    pass


class BuildFailed(Exception):
    def __init__(self, failures, built):
        super().__init__(f"{len(failures)} compile failure(s)")
        self.failures, self.built = failures, built


def unit_path(unit):
    """Unit -> lowercased posix source path relative to the src root, or None (no path rule applies)."""
    obj, ext = unit.get("objname", ""), EXT.get(unit.get("language"))
    low = obj.lower()
    if ext is None or not low.startswith(PREFIX) or not low.endswith(".obj"):
        return None
    rel = obj[len(PREFIX):-len(".obj")]
    i = rel.lower().find(MIDDLE)
    if i >= 0:
        rel = rel[:i] + "\\" + rel[i + len(MIDDLE):]
    return (rel.replace("\\", "/") + ext).lower()


def unit_map(units_path):
    if not Path(units_path).is_file():
        raise Refuse(f"missing units file {units_path}")
    m = {}
    for u in tomllib.loads(Path(units_path).read_text(encoding="utf-8")).get("unit", []):
        p = unit_path(u)
        if p is not None:
            m.setdefault(p, []).append(u)
    return m


def obj_rels(units_path=UNITS):
    """5.1 T4.c2: {unit name lowercased: obj path build() writes, relative to its out dir} (lowercased posix; the
    obj sits at the source path minus ext, so consumers look it up here, never by objname basename)."""
    return {u["name"].lower(): p.rsplit(".", 1)[0] + ".obj"
            for u in tomllib.loads(Path(units_path).read_text(encoding="utf-8")).get("unit", [])
            for p in [unit_path(u)] if p is not None}


def flags_of(unit):
    opt = unit.get("opt")
    toks = opt.split() if isinstance(opt, str) else []
    if not toks or opt == "unknown" or not all(t[0] in "/-" for t in toks):
        raise Refuse(f"unit {unit.get('name')}: opt {opt!r} is not a flags string")
    for t in toks:  # 4.5 T16.2 G18: a force-include, include path or macro the lint never reads; was: any `/` token
        if t[1:].startswith(STEER):
            raise Refuse(f"unit {unit.get('name')}: opt token {t!r} steers includes or macros (refused: "
                         f"{' '.join('/' + s for s in STEER)})")
    return toks


def pool_size():
    """5.4 T5.c4: compile workers for the free memory now (see CAP)."""
    free = timing.free_mb()
    return 1 if free is None else max(1, min(CAP, (free - RESERVE_MB) // PER_JOB_MB))


def build(src_root, out_dir, units_path=UNITS, config=None, workers=None):
    src_root, out_dir = Path(src_root).resolve(), Path(out_dir).resolve()
    if out_dir == RUN or not out_dir.is_relative_to(RUN):
        raise Refuse(f"out dir {out_dir} is not under {RUN}")
    if not src_root.is_dir():
        raise Refuse(f"missing src root {src_root}")
    cfg = vv_inputs.load_config(config)
    bindir = cfg.get("msvc71_dir")
    if bindir is None or not (Path(bindir) / "cl.exe").is_file():
        raise Refuse(f"msvc71_dir unset or has no cl.exe: {bindir}")
    umap = unit_map(units_path)
    srcs = sorted((p for p in src_root.rglob("*") if p.is_file() and p.suffix.lower() in (".c", ".cpp")),
                  key=lambda p: p.relative_to(src_root).as_posix())
    jobs, objs = [], {}
    for s in srcs:
        rel = s.relative_to(src_root).as_posix()
        us = umap.get(rel.lower(), [])
        if len(us) != 1:
            raise Refuse(f"{rel} maps to {'no unit' if not us else f'{len(us)} units'}")
        u = us[0]
        flags = flags_of(u)
        # 5.1 T4.c2 (C0247): obj per source path, so same-basename units in two dirs both build
        # was: name = u["objname"].rsplit("\\", 1)[-1]
        name = rel.rsplit(".", 1)[0] + ".obj"
        if name.lower() in objs:  # key: full output path, case-insensitive (x/A.c + x/A.cpp collide); was: basename
            # was: raise Refuse(f"{rel}: obj name {name} also built from {objs[name.lower()]}")
            raise Refuse(f"{rel}: obj path {name} also built from {objs[name.lower()]}")
        objs[name.lower()] = rel
        jobs.append((u, s, rel, flags, out_dir / name))
    if out_dir.exists():
        shutil.rmtree(out_dir)
    out_dir.mkdir(parents=True)
    for _u, _s, _rel, _flags, obj in jobs:
        obj.parent.mkdir(parents=True, exist_ok=True)

    def one(i):  # 5.4 T5.c4: one cl in its own scratch (cwd, TMP/TEMP); was: scratch=out_dir, shared by the serial loop
        u, s, rel, flags, obj = jobs[i]
        scratch = out_dir / f".scratch-{i}"
        scratch.mkdir()
        try:
            return msvc71.compile_one(Path(bindir), s, flags, obj, scratch=scratch)
        finally:
            shutil.rmtree(scratch, ignore_errors=True)
    with ThreadPoolExecutor(max_workers=workers or pool_size()) as ex:  # 5.4 T5.c4; was: a serial loop
        results = list(ex.map(one, range(len(jobs))))  # map keeps job (source) order
    built, failures = [], []
    for (u, s, rel, flags, obj), (ok, codes, text) in zip(jobs, results):
        if ok:
            built.append({"unit": u["name"], "source": rel, "obj": obj, "flags": " ".join(flags)})
        else:
            failures.append({"unit": u["name"], "source": rel, "codes": codes, "text": text, "flags": " ".join(flags)})
    if failures:
        raise BuildFailed(failures, built)
    return built


def phase_id():
    """5.4 T5.c4: the open plan's `# Phase <id>` (as gate._pyc_prefix finds it), the timing label; None if absent.
    Never raises."""
    try:
        plan = REPO / "phase-ends" / "current" / "PHASE_PLAN.md"
        m = re.search(r"^# Phase (\S+)", plan.read_text(encoding="utf-8"), re.M) if plan.is_file() else None
        return m.group(1) if m else None
    except Exception:
        return None


def main(argv):
    ap = argparse.ArgumentParser(prog="build.py", description=__doc__.splitlines()[0])
    ap.add_argument("--src", required=True, help="source root (path rule applies under it)")
    ap.add_argument("--out", required=True, help="obj dir under .run/ (wiped each run)")
    ap.add_argument("--config", help="local paths config (passed to vv_inputs.load_config)")
    a = ap.parse_args(argv)
    # 5.4 T5.c4: one record; rc = main's. 5.4 T21: shared gate marker
    mk = "build.py " + " ".join(map(str, sys.argv[1:] if argv is None else argv))
    with timing.step("build", label=phase_id(), marker=mk) as rec:
        rec.rc = _main(a)
    return rec.rc


def _main(a):
    try:
        built = build(a.src, a.out, config=a.config)
    except Refuse as e:
        print(f"build: REFUSED {e}", file=sys.stderr)
        return 2
    except BuildFailed as e:
        for b in e.built:
            print(f"built {b['source']} -> {b['obj'].name} [{b['flags']}]")
        for f in e.failures:
            print(f"FAIL {f['source']} [{f['flags']}]: codes {' '.join(f['codes']) or '-'}; {f['text']}")
        print(f"build: {len(e.built)} built, {len(e.failures)} failed")
        return 1
    for b in built:
        print(f"built {b['source']} -> {b['obj'].name} [{b['flags']}]")
    print(f"build: {len(built)} units, {len(built)} objects")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
