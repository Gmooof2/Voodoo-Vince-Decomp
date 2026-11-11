"""Verifier lock (1.5 T6, D76): sha256 fingerprint of every file the gate's verdicts depend on.

CLI: verifier_lock.py write|check
  write  refuses (exit 2) if closure() holds a tools/ module not in LISTED, or if a LISTED file's working copy has
         CRLF line endings (3.1 T4: a w/crlf pin, C0052), naming the file(s); else writes config/verifier.sha256.
  check  exit 0 pass, 1 fail (each cause printed as `FAIL lock: <cause>`).
FP: config/verifier.sha256, lines `<sha256>  <repo-relative path>`, sorted by path; exactly the LISTED files.
Bytecode (3.5 T4, C0158): the lock hashes sources, so check also fails when a LISTED module loaded in this process has
  its `__spec__.cached` in a `__pycache__` under root, or sys.pycache_prefix is unset while one is loaded; gate.py sets
  a fresh prefix under .run/<phase>/gate/ before its tools imports and passes it to child Pythons.
  A module path under root/tools that resolves elsewhere (symlink/junction) is flagged by unlisted (3.5 T4 W7).
Library: check(root=REPO, fp=None, modules=None, names=None) -> [causes] (empty = pass; modules and names only feed
  unlisted, the bytecode rule reads sys.modules); unlisted(root, modules, names) -> [causes] (names, 4.5 T16.1 G5: a
  top-level module name N with an unlisted root/tools/N.py -> `shadow module`); pycache(root) -> [causes];
  fingerprint(root) -> FP text; crlf(root) -> LISTED paths whose bytes hold b"\r\n";
  write(root=REPO, fp=None, mods=None) -> (rc, message), mods None = closure(); closure() -> sorted repo-relative tools/ modules the gate loads (gate imported in a
  child process, plus check_env and audit_public, which the gate runs as subprocesses).
A change to any listed file or to the FP is its own `review: yes` task; a match-adding task edits no verifier file.
"""
import hashlib
import os
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
FP = Path("config") / "verifier.sha256"
LISTED = tuple(sorted([
    "config/audit_address_accepted.txt",
    "config/audit_private_accepted.txt",
    "config/check.retail_xbe.sha",
    "config/delink_cut.sha256",  # 4.5 T16.2.c5 G17: the gate's cut layer pins hashes.txt to it
    "config/inputs.manifest.jsonl",  # 3.5 T4 F6: provenance_lint.inputs pins the PDB/EXE/XBE to its rows
    "config/objdiff_cli.sha256",  # 4.5 T16.1 G4: check_env pins the objdiff-cli binary
    "config/pin_accepted.txt",  # 4.2 T5: pin_rows literal-copy waivers (D119)
    "config/pin_hand.txt",  # 5.5 T4: pin_rows hand/xref rows allowed by (VA, name)
    "config/provenance_accepted.txt",
    "config/retail/symbols.txt",  # 3.5 T4 F3: the name -> VA table the gate compares by (symbols_pin.txt not listed)
    "config/toolchain.sha256",
    "config/units_text.txt",  # 2026-11-10 5.5 T28.c4 G7: pin_rows' unit ranges (units.toml: not listed)
    "githooks/pre-commit",  # 5.5 T14.c2: the hooks ship; commit-msg does not (export/allowlist.txt)
    "githooks/pre-push",
    "tools/annotations.py",
    "tools/audit_public.py",
    "tools/build.py",
    "tools/byte_compare.py",
    "tools/check_env.py",
    "tools/coff_info.py",
    "tools/gate.py",
    "tools/gate_controls.py",  # 5.4 T5: the gate's self-test controls, imported by gate.main on --self-test
    "tools/matched_set.py",
    "tools/msvc71.py",
    "tools/pdb_info.py",
    "tools/pdb_types.py",
    "tools/pe_info.py",
    "tools/pin_rows.py",  # 4.2 T5: the gate's pin_rows layer (D119)
    "tools/provenance_lint.py",
    "tools/relink.py",
    "tools/timing.py",  # 5.4 T5.c2: gate_controls imports it (free_mb sizes the self-test's workers)
    "tools/verifier_lock.py",
    "tools/vv_inputs.py",
    "tools/xbe_info.py",
    "tools/xbe_write.py",
]))


def _rel(path, root):
    """-> repo-relative posix path of a module file under root/tools, else None. A path lexically under root/tools that
    resolves elsewhere (a symlink or junction on the way, 3.5 T4 W7) -> `<lexical path> (via a link)`, never LISTED.
    Was: resolve first, so a link out of the repo returned None (not flagged)."""
    try:
        lex = Path(os.path.abspath(path)).relative_to(os.path.abspath(root))
    except ValueError:
        lex = None
    try:
        rel = Path(path).resolve().relative_to(Path(root).resolve())
    except (OSError, ValueError):
        rel = None
    if lex is not None and lex.parts[:1] == ("tools",) and (
            rel is None or rel.as_posix().lower() != lex.as_posix().lower()):
        return f"{lex.as_posix()} (via a link)"
    return rel.as_posix() if rel is not None and rel.parts[:1] == ("tools",) else None


def fingerprint(root=REPO):
    root = Path(root)
    return "".join(f"{hashlib.sha256((root / p).read_bytes()).hexdigest()}  {p}\n" for p in LISTED)


def crlf(root=REPO):
    """-> LISTED repo-relative paths whose working copy has a CRLF line ending (3.1 T4)."""
    root = Path(root)
    return [p for p in LISTED if (root / p).is_file() and b"\r\n" in (root / p).read_bytes()]


def unlisted(root=REPO, modules=None, names=None):
    """modules: module file paths (default: every sys.modules __file__) -> causes for tools/ ones not in LISTED.
    names (4.5 T16.1 G5): module names (default: sys.modules keys); a top-level one N (no dot) with root/tools/N.py
    present and not LISTED -> `shadow module tools/N.py`, whatever that module's __file__ says (forged or deleted).
    Was: __file__ paths only."""
    if modules is None:
        modules = [getattr(m, "__file__", None) for m in list(sys.modules.values())]
    if names is None:
        names = list(sys.modules)
    listed = {p.lower() for p in LISTED}
    bad = sorted({r for r in (_rel(f, root) for f in modules if f) if r and r.lower() not in listed})
    shadow = sorted({f"tools/{n}.py" for n in names if "." not in n and (Path(root) / "tools" / f"{n}.py").is_file()
                     and f"tools/{n}.py".lower() not in listed})
    return [f"unlisted module {r}" for r in bad] + [f"shadow module {s}" for s in shadow]


def pycache(root=REPO):
    """-> causes (3.5 T4 F1, C0158) over this process's sys.modules: a LISTED module under root whose bytecode path
    (`__spec__.cached`) lies in a `__pycache__` under root (a timestamp-valid planted pyc there runs instead of the
    hashed source); `pycache prefix unset` when sys.pycache_prefix is None while a LISTED module is loaded."""
    listed, rroot = {p.lower() for p in LISTED}, Path(root).resolve()
    causes, loaded = [], False
    for m in list(sys.modules.values()):
        f = getattr(m, "__file__", None)
        r = _rel(f, root) if f else None
        cached = getattr(getattr(m, "__spec__", None), "cached", None)
        if not r or r.lower() not in listed or not cached:
            continue
        loaded = True
        try:
            c = Path(cached).resolve().relative_to(rroot)
        except (OSError, ValueError):
            continue
        if "__pycache__" in c.parts:
            causes.append(f"{r} bytecode from repo {c.as_posix()}")
    if loaded and sys.pycache_prefix is None:
        causes.insert(0, "pycache prefix unset")
    return causes


def check(root=REPO, fp=None, modules=None, names=None):
    root = Path(root)
    fp = Path(fp) if fp else root / FP
    causes = []
    if not fp.is_file():
        causes.append(f"fingerprint missing {fp}")
    else:
        lines = [ln.split("  ", 1) for ln in fp.read_text(encoding="ascii").splitlines() if ln.strip()]
        if not lines:
            causes.append(f"fingerprint empty {fp}")
        got = {p[1]: p[0] for p in lines if len(p) == 2}
        for p in LISTED:
            f = root / p
            if not f.is_file():
                causes.append(f"listed file missing {p}")
            elif lines and p not in got:
                causes.append(f"{p} not in fingerprint")
            elif lines and hashlib.sha256(f.read_bytes()).hexdigest() != got[p]:
                causes.append(f"{p} sha256 mismatch")
        causes += [f"fingerprint lists unlisted {p}" for p in sorted(set(got) - set(LISTED))]
    return causes + unlisted(root, modules, names) + pycache(root)  # was: causes + unlisted(root, modules) (3.5 T4 F1)


def closure():
    code = ("import sys; sys.path.insert(0, sys.argv[1]); import gate, check_env, audit_public; "
            "print('\\n'.join(str(getattr(m, '__file__', '') or '') for m in list(sys.modules.values())))")
    p = subprocess.run([sys.executable, "-c", code, str(REPO / "tools")], stdin=subprocess.DEVNULL,
                       capture_output=True, text=True, errors="replace", cwd=REPO)
    if p.returncode != 0:
        raise RuntimeError(f"closure import rc {p.returncode}: {p.stderr.strip()[-400:]}")
    return sorted({r for r in (_rel(f, REPO) for f in p.stdout.splitlines() if f) if r})


def write(root=REPO, fp=None, mods=None):
    """-> (rc, message). Refuses (rc 2, fp untouched) on an unlisted closure module or a CRLF listed file."""
    root = Path(root)
    fp = Path(fp) if fp else root / FP
    if mods is None:
        try:
            mods = closure()
        except RuntimeError as e:
            return 2, f"verifier_lock: REFUSED: {e}"
    extra = [m for m in mods if m not in LISTED]
    if extra:
        return 2, f"verifier_lock: REFUSED: closure modules not in LISTED: {', '.join(extra)}"
    bad = crlf(root)
    if bad:
        return 2, f"verifier_lock: REFUSED: listed files with CRLF line endings (renormalize to LF): {', '.join(bad)}"
    fp.write_text(fingerprint(root), encoding="ascii", newline="\n")
    return 0, f"verifier_lock: wrote {FP.as_posix()} ({len(LISTED)} files; closure {len(mods)} modules)"


def main(argv):
    if not argv or argv[0] in ("-h", "--help") or argv[0] not in ("write", "check") or len(argv) != 1:
        print(__doc__)
        return 0 if argv and argv[0] in ("-h", "--help") else 2
    if argv[0] == "write":
        rc, msg = write()
        print(msg)
        return rc
    causes = check()
    for c in causes:
        print(f"FAIL lock: {c}")
    print("verifier_lock: pass" if not causes else f"verifier_lock: {len(causes)} cause(s)")
    return 1 if causes else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
