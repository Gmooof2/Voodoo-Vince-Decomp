"""Gate (1.5 T4): the one gate entry. Every layer, in order, never stopping at a failure (D51); green is the rc (D26).

CLI: gate.py [--self-test] [--config P]
  no args      src/ + config/matched_set.toml (the project's matched set); writes derived/gate/last.json.
  --self-test  tests/gate/src + tests/gate/matched_set.toml (the gate fixture, NOT the matched set); banner first;
               never writes last.json; the positive run passes on rc 0 AND its global key set == POSITIVE_GLOBALS + `target <unit>`
               per listed function unit (4.5 T16.2.c5; was: rc 0 only). Then 138 controls (was 135 before 4.5
               T16.2.c5: positive-keys (direct: _positive_ok on a key set with `relink` dropped), last-tree (direct:
               _write_last into scratch paths; a list edit moves tree.list_sha256), cut-unpinned (hashes.txt copy
               edited + cut.complete re-sealed -> `cut: hashes.txt sha256 != config/delink_cut.sha256`, rc 2);
               extra, clean-build, toolchain-split, relink tightened to their own cause text (G13); 131 before 4.5 T16.2.c4: pin-i10-static (direct: I10
               checks a tabled same-section static), pin-vtable-prefix (direct: a proto vtable a prefix of retail's
               fails `slot-count-differs`), pin-accepted-unused (direct: a waiver no conflict uses fails
               `accepted-stale ... unused`), cfg-pin-row (a config copy's pin-row State reaches matched_set's
               row_ok); relink-weak re-planted before relink._ours' kept walk; clean-build seeds with the config;
               124 before 4.5 T16.2.c3: toolchain-hash (a pinned
               msvc binary's sha mismatch fails check_env naming it), size-list (a list size edit fails `size`),
               annotations-refused/-error, pin-refused/-error (layer monkeypatched: REFUSED rc 2 / `<layer> error`
               rc 1), pin-fold-site (direct: D119 fold sites count only beside an agreeing site); 121 before 4.5 T16.2.c2: prov-pragma, prov-noinline (D116 in-body
               `#pragma`, `noinline`, each under a `// !FAKE:` line), prov-include-body (a header `#define` carrying
               `volatile` FAILs d66 and build.flags_of refuses a `/FI` opt); 116 before 4.5 T16.2: audit-renamed-blob, audit-shallow,
               audit-paths-empty, audit-route-pin (direct), relink-exit-zero; 113 before 4.5 T16.1.c4: objdiff-pin, lock-shadow, audit-marker-contradiction; 111 before 4.5 T16.1.c3: pin-plant-near, pin-plant-V6; 109 before 4.5 T16.1: audit-short-blob, audit-gzip-disasm, both direct; 107 before 4.4 T3, 106 before 4.3 T3, 76 before 4.2 T5.c4, 75 before 4.2 T5, 63 before 4.1 T4; 13 from 1.5 T5, 5 lock controls from T6, cut-marker from
               2.1 T2: targets + hashes.txt without cut.complete -> rc 2 at `cut`; relink from 2.3 T7: the relink
               layer with relink control one-function -> rc 1, only the `relink` global fails; 9 prov-* from 3.2 T7:
               prov-hedge prov-proto-pdb prov-name-retail prov-placeholder prov-src prov-fake-diff prov-locals prov-d66
               prov-accepted (and prov-forceinline from 4.2 T5: a `__forceinline` member under a `// !FAKE:` label),
               each a planted violation -> `provenance` fails with a FAIL line of that check at the
               plant's path:line (3.5 T4 C7; was: any line of that check); 3 pin-*
               from 3.4 T6.1 on a list copy: pin-unsized (a row renamed to an unsized func pin) and pin-site-corr (a
               site-corr func pin) fail `size`, pin-alias-dup (a fold-alias name of a listed VA beside it): add
               refused `already in`, then the gate refuses the list rc 2 `duplicate va`); 2 relink-* from 3.4
               T6.1: relink-entry (relink control entry: our first .text function to the head of /ORDER -> rc 2,
               relink REFUSED `our PE entry`) and relink-literal (control literal: a synthetic duplicate literal
               COMDAT with one byte flipped -> rc 2, relink REFUSED `our ranges overlap`; 4.3 T3: relink-fold, control fold, two
               synthetic fold-alias .text COMDAT copies at one VA, one byte flipped, the same refusal; 4.4 T3: relink-samename,
               control samename, a same-name copy of a matched row's COMDAT, one byte flipped, the same refusal, and
               relink-weak, control weak, a row's reloc via a weak external whose TagIndex is the row itself -> rc 1,
               relink `.text digest` FAIL; 4.5 T16.2.c4: planted in relink._ours before its kept walk, TagIndex a
               non-row COMDAT .text no reloc reaches, so a _weak_tag that does not follow gives `vsize`), only relink
               failing; each
               one mutated scratch copy under <.run/phase>/gate/controls/<name>/ (removed after) that must fail at its own named
               check; each prints its full gate output under `control <name>:`, then `control <name>: failed as
               expected at <check>` or `... DID NOT FAIL ...`; last line `controls: J failed as expected of K`.
               Exit 0 iff positive run 0, J == K. Lock controls run on a scratch d/repo (copies of the listed files +
               their fingerprint): lock-edit, lock-missing, lock-fp-missing, lock-fp-empty, lock-unlisted, lock-crlf
               (3.1 T4: a scratch listed file converted to CRLF; verifier_lock.write refused rc 2 naming it, fingerprint
               bytes unchanged, and the lock layer fails); 3.5 T4: lock-pyc (a timestamp-valid planted pyc of
               byte_compare in scratch tools/__pycache__: a child `gate.py --help` there must not run it, and loaded
               in-process with the prefix unset the lock fails on its repo __pycache__ bytecode), lock-symlink
               (scratch tools/ext a symlink or junction out of the repo: `unlisted` flags tools/ext/evil.py via a
               link), symtab-pin (one row of scratch config/retail/symbols.txt renamed -> lock sha256 mismatch), and
               lock-imports (no scratch repo: delink imported after the lock layer through the default sys.modules
               path -> only `lock-imports` fails naming tools/delink.py; its modules removed after).
               Bytecode: a fresh sys.pycache_prefix under <.run/phase>/gate/ is set before the tools imports
               (removed at exit) and passed to child Pythons as PYTHONPYCACHEPREFIX (3.5 T4, C0158).
               3.5 T4 gap controls: data-impostor (the fn as a const data array of the retail bytes, read from the
               XBE at run time into the scratch copy only, in a .text COMDAT -> bytes REFUSED `not a function`;
               objdiff reports fuzzy 0.0), xbe-wrong (config copy, retail_xbe byte-flipped -> xbe_sha1 + bytes setup
               REFUSED, rc 2), objdiff-cli (config copy, objdiff_cli exits non-zero -> `objdiff report`), build-refuse
               (a source mapping to no unit -> rc 2, build `fail: REFUSED`), target-missing, target-unhashed (a
               hashes.txt line renamed, cut.complete re-sealed, a scratch cut pin over it (4.5 T16.2.c5)), bytes-refuse (symbol renamed -> compare REFUSED),
               bytes-two-objs (a second unit row of the same name -> objdiff + bytes `2 built objs`),
               toolchain-split (config copy, msvc71_dir an empty dir -> check_env on the gate's config fails rc 1),
               audit-env (the audit plant with AUDIT_PUBLIC_MANIFEST an empty manifest in the gate's env -> still
               rc 1 magic), relink-bare-exit (relink.load_xbe raises a bare SystemExit() -> rc 2, relink REFUSED),
               relink-not-built (a unit's build fails -> `fail: not run: build failed`), relink-no-funcs (no entries
               -> `fail: no functions`), relink-error / relink-rc (relink.gate_check raises / returns rc 7 -> `fail:
               rc None: relink error` / `fail: rc 7`), prov-locals-cache (the prov-locals plant + the clean source's
               obj planted under its locals cache key, cache dir redirected to scratch -> `locals` FAIL: the gate
               never reads the cache), prov-inputs-pin (config copy, prototype_dir -> a default.pdb with a byte
               flipped -> rc 2, provenance REFUSED vs config/inputs.manifest.jsonl), prov-fake-head (scratch git repo:
               a `// !FAKE:` line committed on upstream zz-base, removed in a HEAD commit -> fake-diff FAIL at it; the
               gate lints fake-diff vs `git merge-base HEAD @{u}`), prov-refused / prov-error (provenance_lint.lint
               raises Refuse / RuntimeError -> rc 2 `fail: REFUSED` / rc 1 `fail: provenance error`). A control
               raising SystemExit is a control error (DID NOT FAIL), the rest still run (3.5 T4).
               4.1 T3.1: audit-null-key (config copy, xbox_ip null, marker root an empty dir, a clean --paths file ->
               `fail: not checked: ` naming address; the marker root makes it fail in a public clone too); audit-tail
               (the audit plant with all three value keys null -> rc 1, the HIT line in the verdict and `6 not checked
               lines dropped`); audit-line-pin and audit-literal-pin (the audit run from a scratch tools dir with a
               line of audit_public.py mutated -> rc 2 naming `route pin` and the cause).
               4.1 T4, each asserting the check's FAIL lines exactly (_prov_exact): prov-fake-reword (base two labels,
               the first removed, the second reworded -> one fake-diff FAIL, at the removed one), prov-fake-trailing (a
               stripped trailing label on a line d66 does not match), prov-fake-comment-excuse (own-line label removed,
               only a trailing comment edited in its function), prov-name-ns (`namespace std` plants: an unqualified
               right member and a right member with a scoped return pass; a wrong member, a wrong one with a scoped
               return and a right one with the namespace closed fail proto-pdb), prov-d66-pdb-volatile (M21: the real
               BeepAlloc.cpp copy with its labels stripped, the AllocateMemory row in a scratch list: d66 FAILs at x and
               y, which the PDB types volatile); annotations-folded (kw `direct`, no gate run: annotations.check on
               scratch src + list: both FOLDED no failure at the VA, one FOLDED and bare duplicates fail `need 1`, also a bare
               duplicate on a VA with no row); annotations-malformed, -no-name, -no-source, -count (direct, a matched row
               with 0 lines), each exactly its one FAIL; annotations-order (direct: an out-of-order plant plus one real
               failure -> exactly that FAIL and the ORDER line; real failure gone -> no FAIL, ORDER line still there);
               annotations-row (through _core: a matched row's NAME line removed -> rc 1, the `annotations` global fails).
               4.2 T5.c4 (D119, D118): 28 pin-plant-<id> (direct: PIN_PLANTS, one plant in a scratch pin copy, rows
               re-run in-process by pin_rows State.replant -> a new FAIL pin-row line of the plant's own class at its
               VA); pin-literal-list (through _core with the pin layer: an accepted-list copy waiving a non-literal row
               -> pin_rows fails `accepted-stale` at it, the count line names pin_rows); pin-unruled (direct: a scratch
               pin copy with ??1Diary@@QAE@XZ's tag forged to a hand xref -> matched_set add refused naming the
               pin-row check; with the real pins the same add passes).
               Controls with a config take make(d, config) (CFG_CONTROLS);
               a control's kw "undo" (env / monkeypatch restore) runs in the runner's finally. objdiff, marker, asm
               assert build pass + their verdict text, d26 `cl exit 0` + D4002, body_sha1 / xbe_sha1 their verdict
               text, target `sha256`, audit rc 1 + its magic HIT line.
Layers: lock (verifier_lock.check, D76: listed files vs config/verifier.sha256, no unlisted tools/ module loaded; on
fail prints `FAIL lock: <causes>` and returns 1 at once, no other layer); check_env (subprocess rc 0, `--config` the gate's); matched_set.load;
cut (2.1 T2: byte_compare.cut_check on the delink dir; an absent/malformed/stale cut.complete prints
`gate: REFUSED cut: <reason>` and returns 2, no per-unit row; 4.5 T16.2.c5 G17: then sha256(hashes.txt) == the
config/delink_cut.sha256 line `<sha256>  retail-<pin[:8]>` (LISTED; a hash only, D46), else `fail: cut: hashes.txt
sha256 != config/delink_cut.sha256` (or `... has no line for retail-<sha8>`), rc 2: cut.complete alone is self-sealed); build.build into <.run/phase>/gate/build (wiped first; a
BuildFailed keeps the built units, failing units' functions get `build` fail); per needed unit the delinked target
derived/delink/retail-<pin[:8]>/<unit __>.obj sha256 == its hashes.txt line; retail XBE sha1 == pin; objdiff
(objdiff.json in the build dir + `objdiff-cli report generate -f json`, each listed fn fuzzy_match_percent == 100.0);
byte_compare.setup once + compare per fn (only `match` passes; 3.5 T4: function-typed symbol, extent ==
listed size; a unit built from > 1 obj fails objdiff + bytes `<n> built objs`); relink (2.3 T7: relink.gate_check, the whole retail
image relinked with every listed function substituted from the build dir's objects, no rebuild; rc 0 pass, 1 `fail:
<FAIL digest/sha1 lines>`, 2 `fail: REFUSED <reason>` (e.g. an incomplete/stale relink cut) and gate rc 2; not run
-> `fail: not run: build failed` / `fail: no functions`); matched_set.check (xbe_sha1 size body_sha1 defined asm,
*objects* extra); audit_public (no args: worktree + staged + history; 5.3 s measured T4.c1) subprocess rc 0 (AUDIT_PUBLIC_CONFIG the gate's config, inherited AUDIT_PUBLIC_*
dropped, 3.5 T4; a rc != 0 verdict's tail leaves out the per-class `not checked` lines, counted; 4.1 T3.1: a value class
null or absent in the config fails a passing audit row `fail: not checked: <classes>` unless the tree holds
config/PUBLIC_TREE, decided from the config and that file, never the audit's text);
provenance (3.2 T7: provenance_lint.lint(src_root, list_path, lint_rev, accepted) in-process; no failures `pass`;
failures `fail: <n> failure(s) (<check n, ...>)`, each `FAIL <check> <file>:<line>: ...` line printed; Refuse `fail:
REFUSED <why>` and gate rc 2; accepted = config/provenance_accepted.txt, on --self-test
tests/gate/provenance_accepted.txt; 3.5 T4: lint_rev = provenance_lint.UPSTREAM (fake-diff vs `git merge-base HEAD
@{u}`, was HEAD), cache=False (every locals obj compiled in scratch), PDB/EXE/XBE pinned to the inputs manifest);
annotations (4.1 T4: annotations.check(src_root, list_path) in-process; no failures `pass`; failures `fail: <n>
failure(s)`, each `FAIL <file>:<line>: ...` line printed; Refuse `fail: REFUSED <why>` and gate rc 2; order lines are
advisory: `ORDER <file>:<line>: ...` each, then `gate: annotations order lines: <n>`, never failing the row);
pin_rows (4.2 T5, D119: pin_rows.check(config, bdir, list_path) in-process: every pin row plus I10 over the run's
list and build dir; no failures `pass`; failures `fail: <n> failure(s)`, each `FAIL pin-row ...` line printed; Refuse
(pin_rows / matched_set / byte_compare) `fail: REFUSED <why>` and gate rc 2; controls run it only in pin-literal-list);
lock-imports (verifier_lock.unlisted again: no tools/ module outside the lock was imported by the layers).
Output: `PASS|FAIL <symbol>: <check> <verdict>; ...` per function (build target xbe_sha1 size body_sha1 defined asm
objdiff bytes), `PASS|FAIL <global>: <verdict>` per global check, `gate: audit classes not checked: <list or none>` when
the audit ran, `gate: layer seconds: <layer> <s>, ...; total <s>` (lock check_env list cut build target xbe matched_set
objdiff bytes relink audit provenance annotations pins lock-imports report; a layer not run is absent; an early return prints neither;
4.1 T3.1), `gate: tree head <sha12> dirty <n> list <sha12> fp <sha12>` (run() only: the real gate and the self-test's
positive run, not the controls; 4.5 T16.2.c5 G16), then `N passed, F failed of N` (functions).
A function passes iff matched_set.is_done over ALL its verdicts. Build dir removed on pass, kept (path printed) on fail.
last.json: {date, xbe_sha1, rows: [{symbol, unit, va, size, body_sha1, verdicts, passed}], audit_not_checked: [class],
layer_seconds: {layer: s}, tree: {head, dirty (`git status --porcelain` line count), list_sha256 (the list file),
fp_sha256 (config/verifier.sha256)}} (hashes only, D46; tree 4.5 T16.2.c5, None where git or a file fails).
Exit: 0 every function and global check pass and N >= 1; 1 any failure, N = 0, or missing list (reason printed);
2 refusal (malformed list, cut.complete absent/malformed/stale, cut pin mismatch, build Refuse, unexpected byte_compare setup refusal,
relink refusal).
Library: run(src_root, list_path, self_test=False, config=None) -> _core's 4-tuple (4.5 T16.2.c5; was: rc);
  _core(src_root, list_path, config, write_last, units_path, symtabs, tdir, bdir, audit_args, lock_root, lock_fp,
        lock_modules, relink_control, run_relink, run_audit, run_lint, lint_accepted, lint_base, lint_rev,
        marker_root, audit_tools, run_annotations, run_pins, tree, cut_pin)
        -> (rc, verdicts, glob, info); info["provenance"] = the lint's FAIL lines; info["annotations"],
        info["annotation_orders"] = annotations.check's FAIL and ORDER lines; info["audit_not_checked"],
        info["layer_seconds"] as in last.json.
"""
import argparse
import atexit
import contextlib
import datetime
import gzip
import hashlib
import io
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
import tomllib
import types
from pathlib import Path


def _pyc_prefix():
    """3.5 T4 F1 (C0158): a fresh empty pycache dir under .run/<phase>/gate/ (phase as byte_compare._phase_run_dir,
    inlined: it must exist before any tools import), so no repo __pycache__ pyc runs instead of a listed source."""
    repo = Path(__file__).resolve().parent.parent
    run, plan = repo / ".run", repo / "phase-ends" / "current" / "PHASE_PLAN.md"
    m = re.search(r"^# Phase (\S+)", plan.read_text(encoding="utf-8"), re.M) if plan.is_file() else None
    run = (run / m.group(1) if m else run) / "gate"
    run.mkdir(parents=True, exist_ok=True)
    return tempfile.mkdtemp(prefix="pyc-", dir=run)


sys.pycache_prefix = _pyc_prefix()  # was: unset (repo tools/__pycache__ pycs loaded by timestamp; C0158)
atexit.register(shutil.rmtree, sys.pycache_prefix, True)
sys.path.insert(0, str(Path(__file__).resolve().parent))
import annotations  # noqa: E402  annotations layer (4.1 T4)
import audit_public  # noqa: E402  value classes and the marker name, one source (4.1 T3.1)
import build  # noqa: E402
import byte_compare  # noqa: E402
import coff_info  # noqa: E402
import matched_set  # noqa: E402
import pin_rows  # noqa: E402  pin-row layer (4.2 T5, D119)
import provenance_lint  # noqa: E402  provenance layer (3.2 T7)
import relink  # noqa: E402  whole-image relink layer (2.3 T7)
import verifier_lock  # noqa: E402
import vv_inputs  # noqa: E402

REPO = vv_inputs.REPO
TOOLS = REPO / "tools"
SRC, LIST = REPO / "src", REPO / "config" / "matched_set.toml"
FIX_SRC, FIX_LIST = REPO / "tests" / "gate" / "src", REPO / "tests" / "gate" / "matched_set.toml"
FIX_ACC = REPO / "tests" / "gate" / "provenance_accepted.txt"
LAST = REPO / "derived" / "gate" / "last.json"
CUT_PIN = REPO / "config" / "delink_cut.sha256"  # 4.5 T16.2.c5 (G17): sha256 of the real cut's hashes.txt
SYMTABS = [REPO / "config" / "retail" / "symbols.txt", REPO / "config" / "retail" / "symbols_pin.txt"]
ORDER = ("build", "target", *matched_set.REQUIRED, "objdiff", "bytes")


def _tool(name, args=(), config=None, tools=TOOLS):
    """-> verdict of `PY tools/<name>.py` by its rc (D26). The gate's config reaches the child: check_env as
    `--config`, audit_public as AUDIT_PUBLIC_CONFIG (3.5 T4 F4/N5; was: config ignored, the child used the default).
    Inherited AUDIT_PUBLIC_* are dropped (3.5 T4 W6; was: the gate's env steered the audit's manifest/config).
    tools: the dir the script runs from. audit_public's per-class `not checked` lines leave the tail, counted (4.1 T3.1;
    was: printed last, they crowded a HIT out of the verdict)."""
    cmd = [sys.executable, str(Path(tools) / f"{name}.py"), *args]
    env = {k: v for k, v in os.environ.items() if not k.startswith("AUDIT_PUBLIC_")}
    env["PYTHONPYCACHEPREFIX"] = sys.pycache_prefix  # 3.5 T4 F1; was: inherited env (no prefix)
    if config and name == "check_env":
        cmd += ["--config", str(config)]
    elif config and name == "audit_public":
        env["AUDIT_PUBLIC_CONFIG"] = str(config)
    p = subprocess.run(cmd, stdin=subprocess.DEVNULL, capture_output=True, text=True, errors="replace", env=env)
    if p.returncode == 0:
        return "pass"
    lines = (p.stdout + p.stderr).strip().splitlines()
    kept = lines
    if name == "audit_public":
        ends = tuple(f"{c}: not checked ({k} not set in local config)" for c, k in audit_public.VALUE_KEYS.items())
        kept = [ln for ln in lines if ln.startswith("HIT ") or not ln.endswith(ends)]
    dropped = len(lines) - len(kept)
    tail = " | ".join(kept[-3:] + ([f"{dropped} not checked lines dropped"] if dropped else []))
    return f"fail: rc {p.returncode}: {tail}"


def _sha(path, algo):
    return hashlib.new(algo, Path(path).read_bytes()).hexdigest()


def _hashes(tdir):
    h = tdir / "hashes.txt"
    if not h.is_file():
        return {}
    return {p[1]: p[0] for p in (ln.split() for ln in h.read_text(encoding="ascii").splitlines()) if len(p) == 2}


def _objdiff(bdir, units, cfg):
    """units: {unit: (target, base)} -> ({unit: {fn: pct}}, global verdict)."""
    proj = {"units": [{"name": u.replace("\\", "/"), "target_path": str(t), "base_path": str(b)}
                      for u, (t, b) in sorted(units.items())]}
    (bdir / "objdiff.json").write_text(json.dumps(proj, indent=2) + "\n", encoding="utf-8", newline="\n")
    report = bdir / "report.json"
    cmd = [str(cfg["objdiff_cli"]), "report", "generate", "-p", str(bdir), "-o", str(report), "-f", "json"]
    p = subprocess.run(cmd, stdin=subprocess.DEVNULL, capture_output=True, text=True, errors="replace")
    if p.returncode != 0 or not report.is_file():
        tail = " | ".join((p.stdout + p.stderr).strip().splitlines()[-3:])
        return {}, f"fail: objdiff-cli report rc {p.returncode}: {tail}"
    got = {}
    for u in json.loads(report.read_text(encoding="utf-8")).get("units", []):
        got[u.get("name")] = {f.get("name"): float(f.get("fuzzy_match_percent", 0.0)) for f in u.get("functions", [])}
    return got, "pass"


def _tname(u):
    name = u.replace("\\", "__").replace("/", "__")
    return name if name.lower().endswith(".obj") else name + ".obj"


def run(src_root, list_path, self_test=False, config=None):
    if self_test:
        print(f"gate: SELF-TEST on the fixture {Path(src_root)} + {Path(list_path)}; NOT the matched set; "
              f"last.json not written")
    return _core(src_root, list_path, config=config, write_last=not self_test,
                 lint_accepted=FIX_ACC if self_test else None, tree=True)  # 4.5 T16.2.c5: the 4-tuple, the tree
    # line; was: [0] (rc only), no tree


def _core(src_root, list_path, config=None, write_last=False, units_path=None, symtabs=None, tdir=None, bdir=None,
          audit_args=(), lock_root=None, lock_fp=None, lock_modules=None, relink_control=None, run_relink=True,
          lock_names=None,  # 4.5 T16.1 G5: top-level module names for the shadow rule (None = sys.modules)
          run_audit=True, run_lint=True, lint_accepted=None, lint_base=None,
          lint_rev=provenance_lint.UPSTREAM,  # 3.5 T4 W5; was: lint_rev="HEAD"
          marker_root=None, audit_tools=None,  # 4.1 T3.1: the tree holding config/PUBLIC_TREE, the audit script's dir
          run_annotations=True,  # 4.1 T4
          run_pins=True,  # 4.2 T5
          tree=False, cut_pin=None):  # 4.5 T16.2.c5: print the tree line (G16); the cut pin file (G17, None = CUT_PIN)
    """-> (rc, {symbol: {check: verdict}}, {global: verdict}, {"bdir", "failures"}); prints the gate output.
    Overrides default to the gate's own inputs (units.toml, SYMTABS, the pinned delink dir, <.run/phase>/gate/build).
    run_relink / run_audit False (self-test controls other than `relink` / `audit`, 2.6 T11): that layer is not run
    and adds no global (the history audit is ~25 s of each control, the relink ~3 s). run_lint False likewise (3.2 T7:
    the provenance layer runs in the positive run and the prov-* controls); run_annotations False likewise (4.1 T4:
    the positive run and the annotations-* controls); run_pins False likewise (4.2 T5: the positive run and the
    pin-literal-list control)."""
    refused, glob, verd, failed_units = False, {}, {}, {}
    bdir = Path(bdir) if bdir else byte_compare._phase_run_dir() / "gate" / "build"
    secs = {}
    info = {"bdir": bdir, "failures": failed_units, "audit_not_checked": [], "layer_seconds": secs}
    start = last = time.perf_counter()

    def mark(layer):  # 4.1 T3.1: seconds since the previous mark
        nonlocal last
        now = time.perf_counter()
        secs[layer] = round(now - last, 2)
        last = now
    # verifier lock (D76): first; an unproven verifier's verdicts mean nothing, so no other layer runs on a fail
    lock_root = Path(lock_root) if lock_root else REPO
    causes = verifier_lock.check(lock_root, lock_fp, lock_modules, lock_names)  # was: no lock_names (4.5 T16.1)
    glob["lock"] = "pass" if not causes else "fail: " + "; ".join(causes)
    if causes:
        print(f"FAIL lock: {'; '.join(causes)}")
        return 1, verd, glob, info
    mark("lock")
    glob["check_env"] = _tool("check_env", config=config)  # 3.5 T4 F4; was: _tool("check_env") (default config)
    mark("check_env")
    # list
    entries, reason = [], None
    try:
        entries = matched_set.load(list_path)
    except matched_set.Refuse as e:
        try:
            parsed = matched_set._parse(list_path)
        except matched_set.Refuse:
            parsed = None
        if parsed is None and Path(list_path).is_file():
            print(f"gate: REFUSED malformed list: {e}")
            return 2, verd, glob, info
        reason = str(e)
        entries = parsed or []
        print(f"gate: {reason}")
    funcs = [e for e in entries if e["kind"] == "function"]
    glob["list"] = f"fail: {reason}" if reason else "pass"
    mark("list")
    # cut completeness (2.1 T2, T1 contract via byte_compare.cut_check): a partial or stale cut is refused
    pin = coff_info.pinned_sha()
    tdir = Path(tdir) if tdir else REPO / "derived" / "delink" / f"retail-{(pin or '')[:8]}"
    why = byte_compare.cut_check(tdir) or _cut_pinned(tdir, pin, cut_pin)  # 4.5 T16.2.c5 (G17): + the pin kept
    # outside the cut; was: cut_check only (cut.complete checks hashes.txt, both in the cut: self-sealed)
    glob["cut"] = f"fail: {why}" if why else "pass"
    if why:
        print(f"gate: REFUSED cut: {why}")
        return 2, verd, glob, info
    mark("cut")
    # build
    if bdir.exists():
        shutil.rmtree(bdir)
    try:
        built = build.build(src_root, bdir, units_path or build.UNITS, config)
        glob["build"] = "pass"
    except build.Refuse as e:
        print(f"gate: REFUSED build: {e}")
        glob["build"] = f"fail: REFUSED {e}"  # 3.5 T4 N4; was: printed only, no `build` key
        return 2, verd, glob, info
    except build.BuildFailed as e:
        built = e.built
        failed_units.update({f["unit"]: f for f in e.failures})
        glob["build"] = f"fail: {len(e.failures)} unit(s) failed: {', '.join(sorted(failed_units))}"
    byunit = {b["unit"]: b for b in built}
    # 3.5 T4 N9: a unit built from more than one obj has no one base: dropped from byunit (was: the last obj kept,
    # objdiff + bytes ran on it); its functions fail objdiff and bytes naming the count
    nobj = {u: sum(b["unit"] == u for b in built) for u in byunit}
    multi = {u: n for u, n in nobj.items() if n > 1}
    for u in multi:
        del byunit[u]
    verd.update({f["symbol"]: {} for f in funcs})
    for f in funcs:
        if f["unit"] in byunit or f["unit"] in multi:
            verd[f["symbol"]]["build"] = "pass"
        elif f["unit"] in failed_units:
            fu = failed_units[f["unit"]]
            verd[f["symbol"]]["build"] = f"fail: {fu['source']} {fu['codes']}"
        else:
            verd[f["symbol"]]["build"] = f"fail: no source built for unit {f['unit']}"
    mark("build")
    # delinked targets (tdir, pin: set at the cut check)
    hashes, tverd, targets = _hashes(tdir), {}, {}
    for u in sorted({f["unit"] for f in funcs}):
        name = _tname(u)
        t = tdir / name
        if not t.is_file():
            tverd[u] = f"fail: target missing {t}"
        elif name not in hashes:
            tverd[u] = f"fail: {name} not in {tdir / 'hashes.txt'}"
        elif _sha(t, "sha256") != hashes[name]:
            tverd[u] = f"fail: {name} sha256 {_sha(t, 'sha256')} != hashes.txt {hashes[name]}"
        else:
            tverd[u] = "pass"
            targets[u] = t
        glob[f"target {u}"] = tverd[u]
    for f in funcs:
        verd[f["symbol"]]["target"] = tverd[f["unit"]]
    mark("target")
    # retail XBE pin
    xbe_sha1 = None
    try:
        xp = vv_inputs.input_path("retail_xbe", cfg=vv_inputs.load_config(config))
        xbe_sha1 = _sha(xp, "sha1")
        glob["xbe_sha1"] = "pass" if pin and xbe_sha1 == pin else f"fail: XBE sha1 {xbe_sha1} != pin {pin}"
    except (OSError, KeyError) as e:
        glob["xbe_sha1"] = f"fail: {e}"
    mark("xbe")
    # matched_set checks
    try:
        ms = matched_set.check(entries, built, src_root, config)
    except matched_set.Refuse as e:
        ms = {f["symbol"]: {k: f"fail: {e}" for k in matched_set.REQUIRED} for f in funcs}
        ms[matched_set.OBJECTS] = {"extra": f"fail: {e}"}
    for f in funcs:
        verd[f["symbol"]].update(ms.get(f["symbol"], {}))
    glob["*objects* extra"] = ms.get(matched_set.OBJECTS, {}).get("extra", "fail: not reported")
    mark("matched_set")
    # objdiff
    pairs = {u: (targets[u], byunit[u]["obj"]) for u in targets if u in byunit}
    if pairs:
        cfg = vv_inputs.load_config(config)
        rep, glob["objdiff report"] = _objdiff(bdir, pairs, cfg)
    else:
        rep = {}
        glob["objdiff report"] = "pass" if not funcs else "fail: no unit has both a target and a built obj"
    for f in funcs:
        u = f["unit"]
        if u in multi:
            v = f"fail: unit has {multi[u]} built objs (need 1)"
        elif u not in pairs:
            v = "fail: no target/base pair for unit"
        else:
            pct = rep.get(u.replace("\\", "/"), {}).get(f["symbol"])
            v = ("fail: not in report" if pct is None else "pass" if pct == 100.0 else f"fail: fuzzy {pct}")
        verd[f["symbol"]]["objdiff"] = v
    mark("objdiff")
    # byte compare
    if funcs:
        try:
            _p, byname, _t, _xp, xb, x = byte_compare.setup(
                argparse.Namespace(symbols=[str(t) for t in (symtabs or SYMTABS)], xbe=None, config=config))
            glob["bytes setup"] = "pass"
        except byte_compare.Refuse as e:
            glob["bytes setup"] = f"fail: REFUSED {e}"
            refused = True
            byname = None
        for f in funcs:
            if byname is None:
                v = "fail: setup refused"
            elif f["unit"] in multi:
                v = f"fail: unit has {multi[f['unit']]} built objs (need 1)"
            elif f["unit"] not in byunit:
                v = "fail: no built obj"
            else:
                try:
                    with contextlib.redirect_stdout(io.StringIO()):
                        bv, nmask, _n = byte_compare.compare(byunit[f["unit"]]["obj"], f["symbol"], f["va"],
                                                             byname, xb, x, f["size"])  # 3.5 T4 F2; was: no size
                    v = "pass" if bv == "match" else f"fail: {bv} (masked {nmask})"
                except byte_compare.Refuse as e:
                    v = f"fail: REFUSED {e}"
            verd[f["symbol"]]["bytes"] = v
        mark("bytes")
    # relink (2.3 T7): the whole retail image relinked with every listed function substituted from THIS build (bdir,
    # never rebuilt or wiped here); verdict from relink.gate_check's rc (D26), its printed lines only for the message
    if not run_relink:  # no key: a skip must not make rc non-zero, so each control still fails at its own check only
        print("gate: relink layer not run (self-test control; it runs in the positive run and the relink control)")
    elif glob.get("build") != "pass":
        glob["relink"] = "fail: not run: build failed"
    elif not funcs:
        glob["relink"] = "fail: no functions"
    else:
        try:
            rrc, rlines = relink.gate_check(list_path, bdir, config, relink_control)
        except Exception as e:  # a layer that cannot run fails; the other layers still run (D51)
            rrc, rlines = None, [f"relink error {type(e).__name__}: {e}"]
        if rrc == 0:
            glob["relink"] = "pass"
        elif rrc == 2:
            refused = True
            why = [ln for ln in rlines if ln.startswith("relink: ")] or rlines[-1:] or ["no reason printed"]
            glob["relink"] = f"fail: REFUSED {why[-1].removeprefix('relink: ')}"
        else:
            bad = [re.sub(r"\s+", " ", ln.strip()) for ln in rlines if "FAIL" in ln] or \
                  [ln.strip() for ln in rlines[-3:]]
            bad = [re.sub(r"^\S+relinked\.xbe \d+ B ", "", b) for b in bad]  # the sha1 line, path and size dropped
            glob["relink"] = f"fail: {'' if rrc == 1 else f'rc {rrc}: '}" + "; ".join(bad[:6])
    if run_relink:
        mark("relink")
    if run_audit:
        skipped = [c for c, k in audit_public.VALUE_KEYS.items() if vv_inputs.load_config(config).get(k) is None]
        v = _tool("audit_public", audit_args, config, audit_tools or TOOLS)  # 3.5 T4 F4; was: no config
        # 4.1 T3.1: from the config and the marker file, never the audit's text (D26); was: a null class passed anywhere
        marker = Path(marker_root or REPO) / "config" / audit_public.PUBLIC_MARKER
        if v == "pass" and skipped and not marker.is_file():
            v = (f"fail: not checked: {', '.join(skipped)} (null or absent in config, tree has no "
                 f"config/{audit_public.PUBLIC_MARKER})")
        # 4.5 T16.1 G6: a marker beside .claude/pa.json is a private tree (was: the marker alone excused null classes)
        if marker.is_file() and (Path(marker_root or REPO) / ".claude" / "pa.json").is_file():
            v = (f"fail: {audit_public.PUBLIC_MARKER} marker in a private tree (.claude/pa.json present)" +
                 ("" if v == "pass" else f"; {v}"))
        glob["audit_public"] = v
        info["audit_not_checked"] = skipped
        mark("audit")
    else:  # no key, as for relink above
        print("gate: audit_public layer not run (self-test control; runs in the positive run and the audit control)")
    # provenance (3.2 T7): provenance_lint in-process; verdict from its failure list / Refuse (D26), never its text
    info["provenance"] = []
    if not run_lint:  # no key, as for relink above
        print("gate: provenance layer not run (self-test control; runs in the positive run and the prov-* controls)")
    else:
        try:
            pf = provenance_lint.lint(src_root, list_path, lint_rev, lint_accepted or provenance_lint.ACCEPTED,
                                      lint_base, config, cache=False)[0]  # 3.5 T4 F4; was: no config (default
            # config inputs). 3.5 T4 F5: cache=False, every locals obj compiled in scratch; was: the derived cache
            info["provenance"] = pf
            for ln in pf:
                print(ln)
            pc = ", ".join(f"{k} {n}" for k, n in provenance_lint.counts(pf).items() if n)
            glob["provenance"] = "pass" if not pf else f"fail: {len(pf)} failure(s) ({pc})"
        except provenance_lint.Refuse as e:
            refused = True
            glob["provenance"] = f"fail: REFUSED {e}"
        except Exception as e:  # a layer that cannot run fails; the other layers still run (D51)
            glob["provenance"] = f"fail: provenance error {type(e).__name__}: {e}"
        mark("provenance")
    # annotations (4.1 T4): annotations.check in-process; verdict from its failure list / Refuse (D26), never its text
    info["annotations"], info["annotation_orders"] = [], []
    if not run_annotations:  # no key, as for relink above
        print("gate: annotations layer not run (self-test control; runs in the positive run and the annotations-* "
              "controls)")
    else:
        try:
            af, _nrows, _nfiles, orders = annotations.check(src_root, list_path)
            info["annotations"], info["annotation_orders"] = af, orders
            for ln in orders + af:
                print(ln)
            print(f"gate: annotations order lines: {len(orders)}")  # advisory: never in the row's verdict
            glob["annotations"] = "pass" if not af else f"fail: {len(af)} failure(s)"
        except matched_set.Refuse as e:
            refused = True
            glob["annotations"] = f"fail: REFUSED {e}"
        except Exception as e:  # a layer that cannot run fails; the other layers still run (D51)
            glob["annotations"] = f"fail: annotations error {type(e).__name__}: {e}"
        mark("annotations")
    # pin rows (4.2 T5, D119): pin_rows.check in-process (every pin row + I10 over this run's list and build dir);
    # verdict from its fail list / Refuse (D26), never its text
    info["pin_rows"] = []
    if not run_pins:  # no key, as for relink above
        print("gate: pin_rows layer not run (self-test control; runs in the positive run and pin-literal-list)")
    else:
        try:
            pf = pin_rows.check(config, bdir, list_path)
            info["pin_rows"] = pf
            for ln in pf:
                print(ln)
            glob["pin_rows"] = "pass" if not pf else f"fail: {len(pf)} failure(s)"
        except (pin_rows.Refuse, matched_set.Refuse, byte_compare.Refuse) as e:
            refused = True
            glob["pin_rows"] = f"fail: REFUSED {e}"
        except Exception as e:  # a layer that cannot run fails; the other layers still run (D51)
            glob["pin_rows"] = f"fail: pin_rows error {type(e).__name__}: {e}"
        mark("pins")
    lcauses = verifier_lock.unlisted(lock_root, lock_modules, lock_names)  # modules imported lazily by the layers above
    glob["lock-imports"] = "pass" if not lcauses else "fail: " + "; ".join(lcauses)
    mark("lock-imports")
    # report
    rows, npass = [], 0
    for f in funcs:
        v = verd[f["symbol"]] = {k: verd[f["symbol"]].get(k, "fail: not reported") for k in ORDER}
        ok = matched_set.is_done({"verdicts": v})
        npass += ok
        print(f"{'PASS' if ok else 'FAIL'} {f['symbol']}: " + "; ".join(f"{k} {v[k]}" for k in ORDER))
        rows.append({"symbol": f["symbol"], "unit": f["unit"], "va": f["va"], "size": f["size"],
                     "body_sha1": f["body_sha1"], "verdicts": v, "passed": ok})
    for k, v in glob.items():
        print(f"{'PASS' if v == 'pass' else 'FAIL'} {k}: {v}")
    n = len(funcs)
    mark("report")
    if run_audit:  # 4.1 T3.1: before the count line, which stays the last line of a green run
        print(f"gate: audit classes not checked: {', '.join(info['audit_not_checked']) or 'none'}")
    print("gate: layer seconds: " + ", ".join(f"{k} {s:.2f}" for k, s in secs.items())
          + f"; total {time.perf_counter() - start:.2f}")
    if tree or write_last:  # 4.5 T16.2.c5 (G16): the tree the verdict is of, before the count line
        info["tree"] = t = _tree(list_path)
        print(f"gate: tree head {(t['head'] or 'none')[:12]} dirty {t['dirty']} list "
              f"{(t['list_sha256'] or 'none')[:12]} fp {(t['fp_sha256'] or 'none')[:12]}")
    # count line (1.8 T1): fn rows only; any failed global (or N = 0) is named on the same line, so the bare
    # `N passed, 0 failed of N` form appears only on a green run
    gfail = [k for k, v in glob.items() if v != "pass"] or (["N=0"] if n == 0 else [])
    info["count_line"] = f"{npass} passed, {n - npass} failed of {n}" + (
        f"; FAIL globals: {', '.join(gfail)}" if gfail else "")
    print(info["count_line"])
    allok = n >= 1 and npass == n and all(v == "pass" for v in glob.values())
    if write_last:
        _write_last(LAST, list_path, xbe_sha1, rows, info, secs, info.get("tree"))
    if allok:
        shutil.rmtree(bdir, ignore_errors=True)
    else:
        print(f"gate: build dir kept: {bdir}")
    return (2 if refused else (0 if allok else 1)), verd, glob, info


def _tree(list_path):  # 4.5 T16.2.c5 (G16): -> {head, dirty, list_sha256, fp_sha256}; None where git / a file fails
    def git(*a):
        p = subprocess.run(["git", *a], cwd=REPO, stdin=subprocess.DEVNULL, capture_output=True, text=True,
                           errors="replace")
        return p.stdout if p.returncode == 0 else None  # D26: by rc
    head, st = git("rev-parse", "HEAD"), git("status", "--porcelain")
    fp = REPO / verifier_lock.FP
    return {"head": head.strip() if head else None, "dirty": None if st is None else len(st.splitlines()),
            "list_sha256": _sha(list_path, "sha256") if Path(list_path).is_file() else None,
            "fp_sha256": _sha(fp, "sha256") if fp.is_file() else None}


def _cut_pinned(tdir, pin, cut_pin=None):  # 4.5 T16.2.c5 (G17): None when sha256(tdir/hashes.txt) == the pin's line
    """The pin file (CUT_PIN; lines `<sha256>  retail-<pin[:8]>`, a hash only, D46) is the cut's seal outside it."""
    p = Path(cut_pin) if cut_pin else CUT_PIN
    name = CUT_PIN.relative_to(REPO).as_posix() if not cut_pin else str(p)
    rows = dict(reversed(ln.split("  ", 1)) for ln in p.read_text(encoding="ascii").splitlines()
                if ln.count("  ") == 1) if p.is_file() else {}
    want = rows.get(f"retail-{(pin or '')[:8]}")
    if not want:
        return f"cut: {name} has no line for retail-{(pin or '')[:8]}"
    return None if _sha(Path(tdir) / "hashes.txt", "sha256") == want else f"cut: hashes.txt sha256 != {name}"


def _write_last(path, list_path, xbe_sha1, rows, info, secs, tree=None):  # 4.5 T16.2.c5: factored out of _core
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    doc = {"date": datetime.datetime.now(datetime.timezone.utc).isoformat(timespec="seconds"),
           "xbe_sha1": xbe_sha1, "rows": rows, "audit_not_checked": info["audit_not_checked"],
           "layer_seconds": secs, "tree": tree or _tree(list_path)}  # G16; was: no tree
    path.write_text(json.dumps(doc, indent=1) + "\n", encoding="utf-8", newline="\n")


def _positive_ok(rc, glob, list_path):  # 4.5 T16.2.c5: the self-test's positive-run verdict, factored out of main
    """rc 0 and the global key set == POSITIVE_GLOBALS + `target <unit>` per listed function unit (G13); was: rc == 0
    (a layer that added no key passed unseen)."""
    units = {e["unit"] for e in matched_set.load(list_path) if e["kind"] == "function"}
    want = set(POSITIVE_GLOBALS) | {f"target {u}" for u in units}
    missing, extra = sorted(want - set(glob)), sorted(set(glob) - want)
    if missing or extra:
        print(f"gate: FAIL positive run global keys: missing {missing}; unexpected {extra}")
    return rc == 0 and not missing and not extra


# --- self-test controls (1.5 T5): each mutates one scratch copy of the fixture and must fail at its own check ---
LOC_UNIT, LOC_SYM = "Xbox\\OfficialRelease\\LocString.obj", "_LocString_GetStringID"
LOC_SRC = Path("Libs") / "LocString" / "LocString.cpp"
SET_SYM = "?SetState@VinceDropsInCutscene@@EAEXH@Z"
FLOUR_UNIT, FLOUR_SYM = "Xbox\\OfficialRelease\\FlourSack.obj", "??1FlourSackMain@@UAE@XZ"
PIN_TOML = REPO / "config" / "compiler_pin.toml"


def _units_copy(dst, unit, edit):
    """units.toml copy with `unit`'s opt replaced by edit(old opt); asserted by re-parsing the copy."""
    lines = build.UNITS.read_text(encoding="utf-8").splitlines(keepends=True)
    head = [i for i, ln in enumerate(lines) if ln.strip() == f"name = {json.dumps(unit)}"]
    if len(head) != 1:
        raise RuntimeError(f"unit {unit}: {len(head)} name lines in {build.UNITS}")
    old = new = None
    for i in range(head[0] + 1, len(lines)):
        if lines[i].startswith("[[unit]]"):
            break
        if lines[i].startswith("opt = "):
            old = tomllib.loads(lines[i])["opt"]
            new = edit(old)
            lines[i] = f"opt = {json.dumps(new)}  # gate control\n"
            break
    if new is None:
        raise RuntimeError(f"unit {unit}: no opt line")
    dst.write_text("".join(lines), encoding="utf-8", newline="\n")
    got = [u for u in tomllib.loads(dst.read_text(encoding="utf-8"))["unit"] if u["name"] == unit]
    if [u.get("opt") for u in got] != [new]:
        raise RuntimeError(f"units copy: {unit} opt {got} != {new!r}")
    return old, new


def _edit_text(path, old, new):
    s = path.read_text(encoding="utf-8")
    if s.count(old) != 1:
        raise RuntimeError(f"{path}: anchor {old!r} found {s.count(old)} times")
    path.write_text(s.replace(old, new), encoding="utf-8", newline="\n")


def _c_objdiff(d):
    flip = next(f["control"] for f in tomllib.loads(PIN_TOML.read_text(encoding="utf-8"))["function"]
                if f["symbol"] == LOC_SYM)
    old, new = _units_copy(d / "units.toml", LOC_UNIT, lambda o: " ".join(flip[1] if t == flip[0] else t
                                                                          for t in o.split()))
    print(f"control objdiff: {LOC_UNIT} opt {old!r} -> {new!r} (compiler_pin control {flip})")
    # 3.5 T4 C1: build pass + the verdict texts; was: objdiff != pass and bytes != pass (a broken compile passed)
    return {"units_path": d / "units.toml"}, lambda r, v, g, i: (
        v[LOC_SYM]["build"] == "pass" and v[LOC_SYM]["objdiff"].startswith("fail: fuzzy") and
        v[LOC_SYM]["bytes"].startswith(("fail: mismatch", "fail: REFUSED extent")),  # the flag: extent 0x25 != 0x22
        f"{LOC_SYM} objdiff + bytes (build pass)")


def _c_bytes(d):
    return {"symtabs": SYMTABS[:1]}, lambda r, v, g, i: (
        v[SET_SYM]["bytes"].startswith("fail: provisional") and v[SET_SYM]["objdiff"] == "pass",
        f"{SET_SYM} bytes provisional (objdiff pass)")


def _c_target(d):
    pin = coff_info.pinned_sha()
    src = REPO / "derived" / "delink" / f"retail-{(pin or '')[:8]}"
    t = d / "targets"
    t.mkdir()
    shutil.copy2(src / "hashes.txt", t / "hashes.txt")
    shutil.copy2(src / byte_compare.CUT_MARKER, t / byte_compare.CUT_MARKER)  # valid over the unchanged hashes.txt
    for e in matched_set.load(FIX_LIST):
        if e["kind"] == "function" and not (t / _tname(e["unit"])).exists():
            shutil.copy2(src / _tname(e["unit"]), t / _tname(e["unit"]))
    obj = t / _tname(FLOUR_UNIT)
    b = bytearray(obj.read_bytes())
    b[-1] ^= 0xFF
    obj.write_bytes(bytes(b))
    print(f"control target: {obj.name} copy, last byte flipped")
    return {"tdir": t}, lambda r, v, g, i: (
        "sha256" in g.get(f"target {FLOUR_UNIT}", "pass") and "sha256" in v[FLOUR_SYM]["target"],
        f"target {FLOUR_UNIT} (sha256)")  # 3.5 T4 C5; was: != pass (`target missing` passed it)


def _c_cut_marker(d):
    pin = coff_info.pinned_sha()
    src = REPO / "derived" / "delink" / f"retail-{(pin or '')[:8]}"
    t = d / "targets"
    t.mkdir()
    shutil.copy2(src / "hashes.txt", t / "hashes.txt")
    for e in matched_set.load(FIX_LIST):
        if e["kind"] == "function" and not (t / _tname(e["unit"])).exists():
            shutil.copy2(src / _tname(e["unit"]), t / _tname(e["unit"]))
    print(f"control cut-marker: hashes.txt + targets copied, no {byte_compare.CUT_MARKER}")
    return {"tdir": t}, lambda r, v, g, i: (
        r == 2 and byte_compare.CUT_MARKER in g.get("cut", ""), "cut (marker absent)")


def _list_copy(d, field):
    lines = FIX_LIST.read_text(encoding="utf-8").splitlines(keepends=True)
    i = lines.index(f"symbol = {json.dumps(FLOUR_SYM)}\n")
    i = next(j for j in range(i + 1, len(lines)) if lines[j].startswith(f"{field} = "))
    lines[i] = f'{field} = "{"0" * 40}"\n'
    lp = d / "matched_set.toml"
    lp.write_text("".join(lines), encoding="utf-8", newline="\n")
    print(f"control {field}: list copy, {FLOUR_SYM} {field} -> {'0' * 40}")
    return lp


def _c_body(d):
    return {"list_path": _list_copy(d, "body_sha1")}, lambda r, v, g, i: (  # 3.5 T4 C5; was: != pass
        v[FLOUR_SYM]["body_sha1"].startswith(f"fail: body_sha1 {'0' * 40} != XBE"), f"{FLOUR_SYM} body_sha1")


def _c_xbe(d):
    return {"list_path": _list_copy(d, "xbe_sha1")}, lambda r, v, g, i: (  # 3.5 T4 C5; was: != pass
        v[FLOUR_SYM]["xbe_sha1"].startswith(f"fail: xbe_sha1 {'0' * 40} != pin"), f"{FLOUR_SYM} xbe_sha1")


def _c_size_list(d):  # 4.5 T16.2.c3 (G14): a list `size` edit fails the row's size verdict, body_sha1 still passes
    lines = FIX_LIST.read_text(encoding="utf-8").splitlines(keepends=True)
    i = lines.index(f"symbol = {json.dumps(FLOUR_SYM)}\n")
    i = next(j for j in range(i + 1, len(lines)) if lines[j].startswith("size = "))
    new = int(lines[i].split("=", 1)[1].strip(), 16) + 1
    lines[i] = f"size = 0x{new:X}\n"
    lp = d / "matched_set.toml"
    lp.write_text("".join(lines), encoding="utf-8", newline="\n")
    print(f"control size-list: list copy, {FLOUR_SYM} size -> 0x{new:X}")
    return {"list_path": lp}, lambda r, v, g, i_: (
        v[FLOUR_SYM]["size"].startswith("fail: list va ") and f" size 0x{new:X} != symbols.txt " in v[FLOUR_SYM]["size"]
        and v[FLOUR_SYM]["body_sha1"] == "pass", f"{FLOUR_SYM} size")


def _c_empty(d):
    lp = d / "matched_set.toml"
    lp.write_text("# gate control: no entries\n", encoding="utf-8", newline="\n")
    return {"list_path": lp}, lambda r, v, g, i: (r == 1 and g.get("list", "pass") != "pass", "list (N=0)")


def _c_missing(d):
    return {"list_path": d / "absent.toml"}, lambda r, v, g, i: (
        r == 1 and g.get("list", "pass") != "pass", "list (missing)")


def _src_copy(d):
    s = d / "src"
    shutil.copytree(FIX_SRC, s)
    return s


def _c_extra(d):
    s = _src_copy(d)
    with open(s / LOC_SRC, "a", encoding="utf-8", newline="\n") as f:
        f.write('\nextern "C" int gate_control_extra(int x)\n{\n    return x + 1;\n}\n')
    print(f"control extra: unmarked _gate_control_extra appended to {LOC_SRC.as_posix()}")
    return {"src_root": s}, lambda r, v, g, i: (  # 4.5 T16.2.c5 G13: the planted name; was: != "pass"
        "unlisted code symbol(s) LocString.obj:_gate_control_extra" in g.get("*objects* extra", ""),
        "*objects* extra (_gate_control_extra)")


def _c_marker(d):
    s = _src_copy(d)
    p = s / LOC_SRC
    _edit_text(p, 'extern "C" int LocString_GetStringID', '#ifdef NON_MATCHING\nextern "C" int LocString_GetStringID')
    with open(p, "a", encoding="utf-8", newline="\n") as f:
        f.write("#endif  // NON_MATCHING\n")
    print(f"control marker: {LOC_SYM} wrapped in #ifdef NON_MATCHING")
    return {"src_root": s}, lambda r, v, g, i: (  # 3.5 T4 C2; was: defined != pass (a broken compile passed)
        v[LOC_SYM]["build"] == "pass" and v[LOC_SYM]["defined"].startswith(f"fail: {LOC_SYM} not defined in"),
        f"{LOC_SYM} defined (build pass)")


def _c_asm(d):
    s = _src_copy(d)
    _edit_text(s / LOC_SRC, "    return -1;\n}", "    __asm { nop }\n    return -1;\n}")
    print(f"control asm: __asm {{ nop }} inserted in {LOC_SYM}")
    return {"src_root": s}, lambda r, v, g, i: (  # 3.5 T4 C2; was: asm != pass (a broken compile passed)
        v[LOC_SYM]["build"] == "pass" and v[LOC_SYM]["asm"].startswith("fail: __asm/_asm in preprocessed"),
        f"{LOC_SYM} asm (build pass)")


def _c_audit(d):
    p = d / "probe.obj"
    p.write_bytes(b"MZ" + bytes(62))
    print(f"control audit: {p} (MZ magic, .obj suffix) audited via --paths")
    return {"audit_args": ("--paths", str(p))}, lambda r, v, g, i: (  # 3.5 T4 C3; was: != pass (an rc 2 crash passed)
        g["audit_public"].startswith("fail: rc 1: ") and f"HIT paths magic {p}" in g["audit_public"],
        "audit_public (rc 1, magic hit)")


def _c_clean(d, config=None):
    seed_src = d / "seed_src"
    (seed_src / LOC_SRC).parent.mkdir(parents=True)
    shutil.copy2(FIX_SRC / LOC_SRC, seed_src / LOC_SRC)
    seed = build.build(seed_src, d / "seed", build.UNITS, config)  # 4.5 T16.2.c4 (G15); was: no config (default)
    s = _src_copy(d)
    (s / LOC_SRC).unlink()
    bdir = d / "build"
    bdir.mkdir()
    shutil.copy2(seed[0]["obj"], bdir / Path(seed[0]["obj"]).name)
    print(f"control clean-build: good {Path(seed[0]['obj']).name} planted in {bdir}; {LOC_SRC.as_posix()} removed")
    return {"src_root": s, "bdir": bdir}, lambda r, v, g, i: (  # 4.5 T16.2.c5 G13: the removed source; was: != "pass"
        v[LOC_SYM]["build"] == f"fail: no source built for unit {LOC_UNIT}", f"{LOC_SYM} build (no source built)")


def _c_d26(d):
    old, new = _units_copy(d / "units.toml", LOC_UNIT, lambda o: o + " /Zq9")
    print(f"control d26: {LOC_UNIT} opt {old!r} -> {new!r}")

    def expect(r, v, g, i):
        fu = i["failures"].get(LOC_UNIT)
        m = re.match(r"cl exit (-?\d+)", fu["text"]) if fu else None
        print(f"control d26: cl rc {m.group(1) if m else 'not reported'}; codes {fu['codes'] if fu else None}; "
              f"obj exists {(i['bdir'] / 'LocString.obj').is_file()}")
        # 3.5 T4 C4: cl exit 0 + D4002; was: build != pass only (any compile failure passed)
        return (v[LOC_SYM]["build"] != "pass" and bool(m) and m.group(1) == "0" and "D4002" in fu["codes"],
                f"{LOC_SYM} build (cl exit 0, D4002)")
    return {"units_path": d / "units.toml"}, expect


def _c_data_impostor(d, config=None):  # 3.5 T4 F2: the fn as a const data array of the retail bytes, .text COMDAT
    pin, xb, x = matched_set._xbe(config)  # the bytes are read from the pinned XBE now, into the scratch copy only (D46)
    e = next(e for e in matched_set.load(FIX_LIST) if e["symbol"] == LOC_SYM)
    off = byte_compare.xbe_info.va_to_off(x, e["va"])
    init = ", ".join(f"0x{b:02X}" for b in xb[off:off + e["size"]])
    s = _src_copy(d)
    p = s / LOC_SRC
    t = p.read_text(encoding="utf-8")
    i = t.index('extern "C" int LocString_GetStringID')
    p.write_text(t[:i] + '#pragma const_seg(".text")\nextern "C" __declspec(selectany) const unsigned char '
                 f'LocString_GetStringID[{e["size"]}] = {{{init}}};\n#pragma const_seg()\n',
                 encoding="utf-8", newline="\n")
    body = matched_set._body_sha1(xb, x, e["va"], e["size"])  # the row's hashes recomputed: only the impostor differs
    lp = d / "matched_set.toml"
    lp.write_text(FIX_LIST.read_text(encoding="utf-8").replace(f'xbe_sha1 = "{e["xbe_sha1"]}"\nbody_sha1 = '
                                                               f'"{e["body_sha1"]}"', f'xbe_sha1 = "{pin}"\n'
                                                               f'body_sha1 = "{body}"'), encoding="utf-8", newline="\n")
    print(f"control data-impostor: {LOC_SYM} -> const data[{e['size']}] in .text COMDAT (retail bytes, scratch "
          f"only); row hashes recomputed (body {'same' if body == e['body_sha1'] else 'CHANGED'})")

    def expect(r, v, g, i):
        print(f"control data-impostor: objdiff {v[LOC_SYM]['objdiff']}")
        return v[LOC_SYM]["bytes"].startswith("fail: REFUSED") and "not a function" in v[LOC_SYM]["bytes"], \
            f"{LOC_SYM} bytes (data symbol)"
    return {"src_root": s, "list_path": lp}, expect


def _cfg_copy(d, config, key, value):  # 3.5 T4: config copy with one key replaced (private paths: never printed)
    c = json.loads(Path(config or vv_inputs.CONFIG).read_text(encoding="utf-8"))
    c[key] = str(value)
    p = d / "local_paths.json"
    p.write_text(json.dumps(c, indent=1) + "\n", encoding="utf-8", newline="\n")
    return p


def _cfg_null(d, config, keys):  # 4.1 T3.1: config copy with these keys null (_cfg_copy's str() cannot say null)
    c = json.loads(Path(config or vv_inputs.CONFIG).read_text(encoding="utf-8"))
    c.update(dict.fromkeys(keys))
    p = d / "local_paths.json"
    p.write_text(json.dumps(c, indent=1) + "\n", encoding="utf-8", newline="\n")
    return p


def _c_xbe_wrong(d, config=None):  # 3.5 T4 N2
    xp = vv_inputs.input_path("retail_xbe", cfg=vv_inputs.load_config(config))
    b = bytearray(xp.read_bytes())
    b[-1] ^= 0xFF
    (d / "retail.xbe").write_bytes(bytes(b))
    print("control xbe-wrong: config copy, retail_xbe -> a copy with its last byte flipped")
    return {"config": _cfg_copy(d, config, "retail_xbe", d / "retail.xbe")}, lambda r, v, g, i: (
        g.get("xbe_sha1", "").startswith("fail: XBE sha1") and g.get("bytes setup", "").startswith("fail: REFUSED XBE sha1"),
        "xbe_sha1 + bytes setup REFUSED")


def _c_objdiff_cli(d, config=None):  # 3.5 T4 N3: objdiff_cli -> the Python (`report` is no script: rc 2)
    print("control objdiff-cli: config copy, objdiff_cli -> the Python executable (exits non-zero)")
    return {"config": _cfg_copy(d, config, "objdiff_cli", sys.executable)}, lambda r, v, g, i: (
        g.get("objdiff report", "").startswith("fail: objdiff-cli report rc") and
        v[LOC_SYM]["objdiff"] == "fail: not in report", "objdiff report")


def _c_objdiff_pin(d, config=None):  # 4.5 T16.1 G4: check_env pins the objdiff-cli binary (config/objdiff_cli.sha256)
    src = vv_inputs.load_config(config)["objdiff_cli"]
    b = bytearray(src.read_bytes())
    b[-1] ^= 0xFF
    (d / src.name).write_bytes(bytes(b))
    print("control objdiff-pin: config copy, objdiff_cli -> a copy of the binary with its last byte flipped")
    return {"config": _cfg_copy(d, config, "objdiff_cli", d / src.name)}, lambda r, v, g, i: (
        g.get("check_env", "").startswith("fail: rc 1") and "objdiff_cli" in g.get("check_env", ""),
        "check_env (objdiff_cli pin)")


def _c_toolchain_hash(d, config=None):  # 4.5 T16.2.c3 (G14): check_env check_pins on a pinned msvc binary's sha
    src, m = Path(vv_inputs.load_config(config)["msvc71_dir"]), d / "msvc"
    m.mkdir()
    for ln in (REPO / "config" / "toolchain.sha256").read_text(encoding="ascii").splitlines():
        shutil.copy2(src / ln.split("  ", 1)[1], m)
    b = bytearray((m / "link.exe").read_bytes())  # link.exe: the gate's build compiles only, never runs it
    b[-1] ^= 0xFF
    (m / "link.exe").write_bytes(bytes(b))
    print("control toolchain-hash: config copy, msvc71_dir -> a copy of the pinned files, link.exe last byte flipped")
    return {"config": _cfg_copy(d, config, "msvc71_dir", m)}, lambda r, v, g, i: (
        g.get("check_env", "").startswith("fail: rc 1") and "FAIL pins: link.exe: sha mismatch" in g["check_env"],
        "check_env (link.exe pin)")


def _c_build_refuse(d):  # 3.5 T4 N4
    s = _src_copy(d)
    (s / "Libs" / "NoUnit.cpp").write_text("int gate_control_no_unit;\n", encoding="utf-8", newline="\n")
    print("control build-refuse: Libs/NoUnit.cpp (maps to no unit) added to a src copy")
    return {"src_root": s}, lambda r, v, g, i: (
        r == 2 and "maps to no unit" in g.get("build", ""), "build (REFUSED)")


def _target_copy(d, skip=()):
    pin = coff_info.pinned_sha()
    src = REPO / "derived" / "delink" / f"retail-{(pin or '')[:8]}"
    t = d / "targets"
    t.mkdir()
    shutil.copy2(src / "hashes.txt", t / "hashes.txt")
    shutil.copy2(src / byte_compare.CUT_MARKER, t / byte_compare.CUT_MARKER)
    for e in matched_set.load(FIX_LIST):
        n = _tname(e["unit"])
        if e["kind"] == "function" and n not in skip and not (t / n).exists():
            shutil.copy2(src / n, t / n)
    return t


def _c_target_missing(d):  # 3.5 T4 N6
    t = _target_copy(d, skip=(_tname(FLOUR_UNIT),))
    print(f"control target-missing: targets copy without {_tname(FLOUR_UNIT)}")
    return {"tdir": t}, lambda r, v, g, i: (
        v[FLOUR_SYM]["target"].startswith("fail: target missing"), f"target {FLOUR_UNIT} (missing)")


def _c_target_unhashed(d):  # 3.5 T4 N6: its hashes.txt line renamed; cut.complete re-sealed over the edited file
    t = _target_copy(d)
    h, n = t / "hashes.txt", _tname(FLOUR_UNIT)
    data = h.read_bytes()
    if data.count(f"  {n}\n".encode()) != 1:
        raise RuntimeError(f"{n}: not one hashes.txt line")
    new = data.replace(f"  {n}\n".encode(), f"  gate_control_{n}\n".encode())
    h.write_bytes(new)
    (t / byte_compare.CUT_MARKER).write_bytes(
        f"cut-complete v1 hashes-sha256 {hashlib.sha256(new).hexdigest()} objects {len(new.splitlines())}\n".encode())
    if byte_compare.cut_check(t):
        raise RuntimeError(f"re-sealed cut does not pass: {byte_compare.cut_check(t)}")
    cp = d / "delink_cut.sha256"  # 4.5 T16.2.c5 (G17): a scratch pin over the edited copy; was: no pin (cut_check only)
    cp.write_text(f"{hashlib.sha256(new).hexdigest()}  retail-{(coff_info.pinned_sha() or '')[:8]}\n", encoding="ascii",
                  newline="\n")
    print(f"control target-unhashed: hashes.txt copy, {n} line renamed, cut.complete re-sealed, scratch cut pin")
    return {"tdir": t, "cut_pin": cp}, lambda r, v, g, i: (
        "not in" in v[FLOUR_SYM]["target"] and "hashes.txt" in v[FLOUR_SYM]["target"], f"target {FLOUR_UNIT} (unhashed)")


def _c_bytes_refuse(d):  # 3.5 T4 N9: the symbol not defined in the built obj -> byte_compare.compare Refuse
    s = _src_copy(d)
    _edit_text(s / LOC_SRC, 'extern "C" int LocString_GetStringID(', 'extern "C" int LocString_GetStringID_x(')
    print(f"control bytes-refuse: {LOC_SYM} renamed in a src copy")
    return {"src_root": s}, lambda r, v, g, i: (
        v[LOC_SYM]["bytes"].startswith("fail: REFUSED symbol") and "not defined" in v[LOC_SYM]["bytes"],
        f"{LOC_SYM} bytes (compare REFUSED)")


def _c_bytes_two_objs(d):  # 3.5 T4 N9: a second unit row of the same name (LocString2.obj) -> two built objs, one unit
    s = _src_copy(d)
    shutil.copy2(s / LOC_SRC, s / LOC_SRC.with_name("LocString2.cpp"))
    text = build.UNITS.read_text(encoding="utf-8")
    i = text.index(f"name = {json.dumps(LOC_UNIT)}\n")
    i, j = text.rindex("[[unit]]", 0, i), text.find("\n[[unit]]", i)
    row = text[i:j if j >= 0 else len(text)]
    dup = re.sub(r'(?m)^(objname = ".*)LocString\.obj"$', r'\1LocString2.obj"', row)
    if dup.count("LocString2.obj") != 1:
        raise RuntimeError("units copy: objname anchor not found once")
    (d / "units.toml").write_text(text + "\n" + dup + "\n", encoding="utf-8", newline="\n")
    print(f"control bytes-two-objs: units copy + a second {LOC_UNIT} row (objname LocString2.obj); src copy + "
          f"LocString2.cpp")
    return {"src_root": s, "units_path": d / "units.toml"}, lambda r, v, g, i: (
        "2 built objs" in v[LOC_SYM]["bytes"] and "2 built objs" in v[LOC_SYM]["objdiff"],
        f"{LOC_SYM} bytes + objdiff (two objs)")


def _lock_repo(d):
    """-> scratch root d/repo: copies of the LISTED files + a fingerprint written for them (asserted to pass)."""
    r = d / "repo"
    for p in verifier_lock.LISTED:
        (r / p).parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(REPO / p, r / p)
    fp = r / verifier_lock.FP
    fp.write_text(verifier_lock.fingerprint(r), encoding="ascii", newline="\n")
    pre = verifier_lock.check(r, fp, ())
    if pre:
        raise RuntimeError(f"scratch lock does not pass before the mutation: {pre}")
    return r, fp


def _lock_kw(r, fp, mods=(), names=None):  # 4.5 T16.1: names -> lock_names; was: (r, fp, mods)
    return {"lock_root": r, "lock_fp": fp, "lock_modules": mods, "lock_names": names}, lambda rv, v, g, i: (
        rv == 1 and g.get("lock", "pass") != "pass" and "check_env" not in g, "lock")


def _c_lock_edit(d):
    r, fp = _lock_repo(d)
    f = r / "tools" / "byte_compare.py"
    b = bytearray(f.read_bytes())
    b[0] ^= 0x01
    f.write_bytes(bytes(b))
    print(f"control lock-edit: first byte of scratch {f.relative_to(r).as_posix()} changed")
    return _lock_kw(r, fp)


def _c_lock_missing(d):
    r, fp = _lock_repo(d)
    (r / "tools" / "xbe_info.py").unlink()
    print("control lock-missing: scratch tools/xbe_info.py deleted")
    return _lock_kw(r, fp)


def _c_lock_fp_missing(d):
    r, fp = _lock_repo(d)
    fp.unlink()
    print("control lock-fp-missing: scratch fingerprint deleted")
    return _lock_kw(r, fp)


def _c_lock_fp_empty(d):
    r, fp = _lock_repo(d)
    fp.write_text("", encoding="ascii")
    print("control lock-fp-empty: scratch fingerprint emptied")
    return _lock_kw(r, fp)


def _c_lock_unlisted(d):
    r, fp = _lock_repo(d)
    m = r / "tools" / "_unlisted.py"
    m.write_text("# gate control: an unlisted verifier module\n", encoding="utf-8", newline="\n")
    print("control lock-unlisted: scratch tools/_unlisted.py passed as a loaded module")
    return _lock_kw(r, fp, [str(m)])


def _c_lock_shadow(d):  # 4.5 T16.1 G5: a top-level module name with an unlisted tools/<name>.py, its __file__ deleted
    r, fp = _lock_repo(d)
    (r / "tools" / "hashlib.py").write_text("# gate control: shadows the stdlib hashlib\n", encoding="utf-8",
                                            newline="\n")
    fake = {"hashlib": types.ModuleType("hashlib")}  # a sys.modules-like entry; a fresh ModuleType has no __file__
    print("control lock-shadow: scratch tools/hashlib.py (unlisted); a module entry `hashlib` with no __file__")
    kw, _expect = _lock_kw(r, fp, [getattr(m, "__file__", None) for m in fake.values()], list(fake))
    return kw, lambda rv, v, g, i: (
        rv == 1 and "shadow module tools/hashlib.py" in g.get("lock", "") and "check_env" not in g,
        "lock (shadow module tools/hashlib.py)")


def _c_lock_crlf(d):
    r, fp = _lock_repo(d)
    p = "tools/matched_set.py"
    f = r / p
    f.write_bytes(f.read_bytes().replace(b"\r\n", b"\n").replace(b"\n", b"\r\n"))
    before = fp.read_bytes()
    rc, msg = verifier_lock.write(r, fp, [])
    same = fp.read_bytes() == before
    refused = rc == 2 and p in msg and same
    print(f"control lock-crlf: scratch {p} converted to CRLF; write rc {rc}: {msg}")
    print(f"control lock-crlf: write {'refused' if rc == 2 else 'NOT refused'}, fingerprint "
          f"{'unchanged' if same else 'CHANGED'}")
    kw, expect = _lock_kw(r, fp)
    return kw, lambda rv, v, g, i: (expect(rv, v, g, i)[0] and refused,
                                    "lock (write refused naming the file, fingerprint unchanged)")


LOCK_PYC_MARK = "control lock-pyc: PLANTED bytecode ran"


def _c_lock_pyc(d):
    """3.5 T4 F1 (C0158): a timestamp-valid planted pyc of a listed module in the scratch repo's tools/__pycache__.
    (a) a child `gate.py --help` on the scratch repo (no PYTHONPYCACHEPREFIX) must not run it; (b) the module loaded
    in-process under an alias with the prefix unset (a listed module imported before the gate set it) runs it, and
    the lock (default sys.modules path) must fail naming its bytecode."""
    import importlib._bootstrap_external as ibe
    import importlib.util
    r, fp = _lock_repo(d)
    src = r / "tools" / "byte_compare.py"
    st = os.stat(src)
    code = compile(src.read_text(encoding="utf-8") + f"\n_LOCK_PYC_PLANTED = True\nprint({LOCK_PYC_MARK!r})\n",
                   str(src), "exec")
    pyc = src.parent / "__pycache__" / Path(importlib.util.cache_from_source(str(src))).name
    pyc.parent.mkdir(parents=True, exist_ok=True)
    pyc.write_bytes(ibe._code_to_timestamp_pyc(code, int(st.st_mtime), st.st_size))
    env = {k: v for k, v in os.environ.items() if k != "PYTHONPYCACHEPREFIX"}
    p = subprocess.run([sys.executable, str(r / "tools" / "gate.py"), "--help"], stdin=subprocess.DEVNULL, env=env,
                       capture_output=True, text=True, errors="replace", cwd=r)
    child_ran = LOCK_PYC_MARK in p.stdout
    child_ok = p.returncode == 0 and "usage" in p.stdout
    print(f"control lock-pyc: planted {pyc.relative_to(r).as_posix()}; child gate.py --help rc {p.returncode}, "
          f"plant {'RAN' if child_ran else 'not run'}")
    alias = "_lock_pyc_byte_compare"
    old = sys.pycache_prefix
    try:
        sys.pycache_prefix = None
        spec = importlib.util.spec_from_file_location(alias, src)
        spec.cached  # lazy on 3.15: fix it while the prefix is unset, as an early import would
        mod = importlib.util.module_from_spec(spec)
        sys.modules[alias] = mod
        spec.loader.exec_module(mod)
    finally:
        sys.pycache_prefix = old
    live = getattr(mod, "_LOCK_PYC_PLANTED", False)
    print(f"control lock-pyc: in-process alias load: plant {'ran' if live else 'NOT run'} (cached {mod.__spec__.cached})")

    def expect(rv, v, g, i):
        sys.modules.pop(alias, None)
        return (rv == 1 and "__pycache__" in g.get("lock", "") and "check_env" not in g and live and child_ok
                and not child_ran, "lock (repo __pycache__ bytecode; child gate entry did not run the plant)")
    return {"lock_root": r, "lock_fp": fp, "lock_modules": None}, expect


def _c_lock_symlink(d):
    """3.5 T4 W7: a module reached through a link (symlink, else a directory junction) out of the scratch repo."""
    r, fp = _lock_repo(d)
    out = d / "outside"
    out.mkdir()
    (out / "evil.py").write_text("# gate control: a module outside the repo\n", encoding="utf-8", newline="\n")
    link = r / "tools" / "ext"
    try:
        os.symlink(out, link, target_is_directory=True)
        how = "symlink"
    except OSError:
        import _winapi  # no symlink privilege: a junction needs none (never skipped silently)
        _winapi.CreateJunction(str(out), str(link))
        how = "junction"
    print(f"control lock-symlink: scratch tools/ext is a {how} to {out.name}/; tools/ext/evil.py passed as loaded")
    return _lock_kw(r, fp, [str(link / "evil.py")])


def _c_symtab_pin(d):
    """3.5 T4 F3: one row of the scratch config/retail/symbols.txt renamed (a forged name -> VA binding)."""
    r, fp = _lock_repo(d)
    p = "config/retail/symbols.txt"
    f = r / p
    if not f.is_file():  # not LISTED before 3.5 T4: copied here so the edit is the same
        f.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(REPO / p, f)
    lines = f.read_bytes().split(b"\n")
    k = next(n for n, ln in enumerate(lines) if ln.strip() and not ln.startswith(b"#"))
    t = lines[k].split(b" ")
    t[3] += b"_forged"
    lines[k] = b" ".join(t)
    f.write_bytes(b"\n".join(lines))
    print(f"control symtab-pin: scratch {p} line {k + 1}: name field renamed")
    return _lock_kw(r, fp)


def _c_lock_imports(d):
    """3.5 T4 N1/C6: `delink` (unlisted) imported after the lock layer, before lock-imports (the first _tool call),
    through the default sys.modules path; the modules it brought in are removed after."""
    orig, before = _tool, set(sys.modules)

    def tool(*a, **k):
        globals()["_tool"] = orig
        import delink  # noqa: F401
        return orig(*a, **k)
    globals()["_tool"] = tool
    print("control lock-imports: tools/delink.py imported at the first _tool call (after the lock layer)")

    def expect(rv, v, g, i):
        globals()["_tool"] = orig
        for m in set(sys.modules) - before:
            sys.modules.pop(m, None)
        return (rv == 1 and g.get("lock") == "pass" and "unlisted module tools/delink.py" in g.get("lock-imports", ""),
                "lock-imports (unlisted module tools/delink.py)")
    return {}, expect


def _c_relink(d):
    print("control relink: relink layer with control one-function (one listed function's final ret -> int3 in a copy)")
    return {"relink_control": "one-function"}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith("fail: .text digest ") and bool(v)  # 4.5 T16.2.c5 G13: .text,
        and " FAIL (1 differing byte offsets; " in g["relink"]  # the one ret byte; was: != pass, no `relink error`
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        "relink (.text digest FAIL, 1 differing byte; every fn row and other global pass)")


def _relink_refused(control, why, what):
    """3.4 T6.1: relink layer with relink control `control` -> rc 2, the relink global REFUSED with `why`."""
    print(f"control relink-{control}: relink layer with control {control} ({what})")
    return {"relink_control": control}, lambda r, v, g, i: (
        r == 2 and g.get("relink", "pass").startswith(f"fail: REFUSED {why}") and bool(v)
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        f"relink (REFUSED {why}; every fn row and other global pass)")


def _c_relink_entry(d):
    return _relink_refused("entry", "our PE entry", "our first .text function to the head of /ORDER")


def _c_relink_literal(d):
    return _relink_refused("literal", "our ranges overlap", "a synthetic duplicate literal COMDAT, one byte flipped")


def _c_relink_fold(d):  # 4.3 T3: the fold-VA accept (C0198) must not swallow a differing copy
    return _relink_refused("fold", "our ranges overlap", "two synthetic fold-alias .text COMDAT copies, one byte flipped")


def _c_relink_samename(d):  # 4.4 T3 (C0215): the same-name row merge must not swallow a differing copy
    return _relink_refused("samename", "our ranges overlap",
                           "a synthetic same-name copy of a matched row's COMDAT, one byte flipped")


def _c_relink_weak(d):  # 4.4 T3 (C0217): a weak external whose TagIndex names a wrong body -> the image differs
    print("control relink-weak: relink layer with control weak (a row's reloc -> a weak external, TagIndex a non-row "
          "COMDAT .text no reloc reaches; planted before relink._ours' kept walk, 4.5 T16.2.c4)")
    return {"relink_control": "weak"}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith("fail: .text digest") and bool(v)
        and all(matched_set.is_done({"verdicts": x}) for x in v.values())
        and all(val == "pass" for k, val in g.items() if k != "relink"),
        "relink (rc 1, .text digest FAIL; every fn row and other global pass)")


def _c_relink_bare_exit(d):  # 3.5 T4 W1: a bare SystemExit() in the relink chain (relink.load_xbe) -> rc 2, not 0
    orig = relink.load_xbe

    def bare(*a, **k):
        raise SystemExit()
    relink.load_xbe = bare  # set last in make: restored by the runner's undo (finally)
    print("control relink-bare-exit: relink.load_xbe -> raise SystemExit() (code None)")
    return {"undo": lambda: setattr(relink, "load_xbe", orig)}, lambda r, v, g, i: (
        r == 2 and g.get("relink", "pass").startswith("fail: REFUSED"), "relink (bare SystemExit -> REFUSED)")


def _c_relink_exit_zero(d):  # 4.5 T16.2 G12: sys.exit(0) in the relink chain (relink.load_xbe) -> rc 2, not 0
    orig = relink.load_xbe

    def zero(*a, **k):
        sys.exit(0)
    relink.load_xbe = zero  # set last in make: restored by the runner's undo (finally)
    print("control relink-exit-zero: relink.load_xbe -> sys.exit(0)")
    return {"undo": lambda: setattr(relink, "load_xbe", orig)}, lambda r, v, g, i: (
        r == 2 and g.get("relink", "pass").startswith("fail: REFUSED"), "relink (SystemExit(0) -> REFUSED)")


def _c_relink_not_built(d):  # 3.5 T4 N7: a unit's build fails -> the relink layer is not run and fails
    old, new = _units_copy(d / "units.toml", LOC_UNIT, lambda o: o + " /Zq9")
    print(f"control relink-not-built: {LOC_UNIT} opt {old!r} -> {new!r}")
    return {"units_path": d / "units.toml"}, lambda r, v, g, i: (
        g.get("relink") == "fail: not run: build failed", "relink (not run: build failed)")


def _c_relink_no_funcs(d):  # 3.5 T4 N7: a list without functions -> relink `fail: no functions`
    kw, _e = _c_empty(d)
    print("control relink-no-funcs: list copy with no entries")
    return kw, lambda r, v, g, i: (g.get("relink") == "fail: no functions", "relink (no functions)")


def _relink_patched(name, fake, want, what):
    """3.5 T4 N7: relink.gate_check replaced by fake (restored by the runner's undo) -> relink fails `want...`, rc 1."""
    orig = relink.gate_check
    relink.gate_check = fake
    print(f"control relink-{name}: relink.gate_check -> {what}")
    return {"undo": lambda: setattr(relink, "gate_check", orig)}, lambda r, v, g, i: (
        r == 1 and g.get("relink", "pass").startswith(want), f"relink ({want})")


def _c_relink_error(d):
    def boom(*a, **k):
        raise RuntimeError("gate control relink-error")
    return _relink_patched("error", boom, "fail: rc None: relink error RuntimeError", "raise RuntimeError")


def _c_relink_rc(d):
    return _relink_patched("rc", lambda *a, **k: (7, ["gate control relink-rc"]), "fail: rc 7: gate control",
                           "return rc 7")


def _c_toolchain_split(d, config=None):  # 3.5 T4 F4/N10/N5: check_env runs on the gate's --config
    (d / "no-msvc").mkdir()  # exists (vv_inputs refuses a missing path) but holds no pinned toolchain
    print("control toolchain-split: config copy, msvc71_dir -> an empty dir")
    return {"config": _cfg_copy(d, config, "msvc71_dir", d / "no-msvc")}, lambda r, v, g, i: (
        g.get("check_env", "").startswith("fail: rc 1") and  # 4.5 T16.2.c5 G13: + the msvc cause; was: rc 1 only
        f"FAIL pins: msvcr71.dll: missing ({d / 'no-msvc' / 'msvcr71.dll'})" in g["check_env"],
        "check_env (the gate's config: msvc71_dir's pinned msvcr71.dll missing)")


def _c_audit_env(d):  # 3.5 T4 W6: AUDIT_PUBLIC_* in the gate's env must not steer the audit layer
    m = d / "env" / "inputs.manifest.jsonl"
    m.parent.mkdir()
    m.write_text("", encoding="utf-8")
    kw, expect = _c_audit(d)
    old = os.environ.get("AUDIT_PUBLIC_MANIFEST")

    def undo():
        if old is None:
            os.environ.pop("AUDIT_PUBLIC_MANIFEST", None)
        else:
            os.environ["AUDIT_PUBLIC_MANIFEST"] = old
    os.environ["AUDIT_PUBLIC_MANIFEST"] = str(m)  # set last in make: restored by the runner's undo (finally)
    print("control audit-env: the audit plant with AUDIT_PUBLIC_MANIFEST -> an empty manifest in the gate's env")
    return {**kw, "run_audit": True, "undo": undo}, expect


def _c_audit_null_key(d, config=None):  # 4.1 T3.1: a null value class outside a public tree fails the audit row
    t = d / "clean.txt"
    t.write_text("clean\n", encoding="utf-8")
    print("control audit-null-key: config copy, xbox_ip null; marker root an empty dir; a clean file via --paths")
    return {"config": _cfg_null(d, config, ["xbox_ip"]), "marker_root": d, "audit_args": ("--paths", str(t)),
            "run_audit": True}, lambda r, v, g, i: (
        g["audit_public"].startswith("fail: not checked: ") and "address" in g["audit_public"],
        "audit_public (not checked: address)")


def _c_audit_marker_contradiction(d, config=None):  # 4.5 T16.1 G6: PUBLIC_TREE beside .claude/pa.json is private
    for p in ("config/" + audit_public.PUBLIC_MARKER, ".claude/pa.json"):
        (d / p).parent.mkdir(parents=True, exist_ok=True)
        (d / p).write_text("{}\n" if p.endswith(".json") else "", encoding="utf-8", newline="\n")
    t = d / "clean.txt"
    t.write_text("clean\n", encoding="utf-8")
    print("control audit-marker-contradiction: config copy, value keys null; marker root holds config/"
          f"{audit_public.PUBLIC_MARKER} and .claude/pa.json; a clean file via --paths")
    return {"config": _cfg_null(d, config, list(audit_public.VALUE_KEYS.values())), "marker_root": d,
            "audit_args": ("--paths", str(t)), "run_audit": True}, lambda r, v, g, i: (
        g.get("audit_public", "").startswith("fail: PUBLIC_TREE marker in a private tree"),
        "audit_public (marker in a private tree)")


def _c_audit_short_blob(d):  # 4.5 T16.1 G7: a truncated cat-file stream (and rc 1) must refuse, never yield
    oid = "0" * 40
    real = audit_public.subprocess.Popen

    class ShortCat:  # cat-file --batch announcing 100 bytes, sending 10, exiting 1
        def __init__(self, *a, **k):
            self.stdin, self.stdout, self.returncode = io.BytesIO(), io.BytesIO(f"{oid} blob 100\n".encode() + b"x" * 10), None

        def wait(self):
            self.returncode = 1
            return 1

    def undo():
        audit_public.subprocess.Popen = real

    def expect(r, v, g, i):
        audit_public.subprocess.Popen = ShortCat
        try:
            got = list(audit_public._cat_blobs([oid], d, None))
            why = f"yielded {[(o, len(b)) for o, b in got]}"
        except SystemExit as e:
            why = f"exit {e.code}"
        finally:
            undo()
        print(f"control audit-short-blob: _cat_blobs on a 10-of-100-byte stream, rc 1: {why}")
        return why == "exit 2", "audit_public._cat_blobs (refused, exit 2)"
    print("control audit-short-blob: audit_public.subprocess.Popen -> a short cat-file stream")
    return {"direct": True, "undo": undo}, expect


def _c_audit_gzip_disasm(d):  # 4.5 T16.1 G8: a tracked gzip blob is classified on its decompressed bytes
    repo, rel = d / "repo", ".claude-state/ledger/x.jsonl.gz"
    env = {k: v for k, v in os.environ.items() if not k.startswith("GIT_")}
    cfgs = ["-c", "user.name=control", "-c", "user.email=control@invalid", "-c", "commit.gpgsign=false",
            "-c", "core.hooksPath=", "-c", "core.autocrlf=false"]
    (repo / rel).parent.mkdir(parents=True)
    (repo / rel).write_bytes(gzip.compress(audit_public._fixtures()["asm.txt"][0], mtime=0))  # generated, no run here
    for a in (["init", "-q"], ["add", rel], ["commit", "-q", "--no-verify", "-m", "plant"]):
        audit_public._git([*cfgs, *a], repo, env)

    def expect(r, v, g, i):
        got = {s: {(c, p) for c, p, _o, _m in audit_public.audit(s, repo, env, set())[1]} for s in ("worktree", "history")}
        print(f"control audit-gzip-disasm: hits {got}")
        return all(("text-run", rel) in h for h in got.values()), f"audit_public text-run {rel} (worktree, history)"
    print(f"control audit-gzip-disasm: scratch repo, {rel} tracked holding gzipped disasm lines")
    return {"direct": True, "undo": lambda: shutil.rmtree(repo, onexc=lambda f, p, e: (os.chmod(p, 0o700), f(p)))}, \
        expect


def _scratch_repo(repo, files, commits=1):
    """4.5 T16.2: a scratch git repo holding files {rel: bytes}, committed `commits` times -> (env, git(args, cwd))."""
    env = {k: v for k, v in os.environ.items() if not k.startswith("GIT_")}
    cfgs = ["-c", "user.name=control", "-c", "user.email=control@invalid", "-c", "commit.gpgsign=false",
            "-c", "core.hooksPath=", "-c", "core.autocrlf=false"]
    repo.mkdir(parents=True)
    for rel, data in files.items():
        (repo / rel).write_bytes(data)
    git = lambda a, cwd=repo: audit_public._git([*cfgs, *a], cwd, env)  # noqa: E731
    git(["init", "-q"])
    git(["add", *files])
    for n in range(commits):
        git(["commit", "-q", "--no-verify", "--allow-empty", "-m", f"plant {n}"])
    return env, git


def _rmtree_ro(p):  # git object files are read-only on Windows
    shutil.rmtree(p, onexc=lambda f, q, e: (os.chmod(q, 0o700), f(q)))


def _refusal(fn, *args):
    """'exit <code>: <stderr>' when fn(*args) exits, else 'returned'; stderr captured."""
    err = io.StringIO()
    try:
        with contextlib.redirect_stderr(err):
            fn(*args)
    except SystemExit as e:
        return f"exit {e.code}: {err.getvalue().strip()}"
    return "returned"


def _c_audit_renamed_blob(d):  # 4.5 T16.2 G10: one blob at two paths, one with a tool suffix -> toolchain hit there
    repo, tool = d / "repo", "b.exe"
    env, _git = _scratch_repo(repo, {"a.txt": b"plain text\n", tool: b"plain text\n"})

    def expect(r, v, g, i):
        got = {(c, p) for c, p, _o, _m in audit_public.audit("history", repo, env, set())[1]}
        print(f"control audit-renamed-blob: history hits {got}")
        return ("toolchain", tool) in got, f"audit_public toolchain {tool} (history)"
    print(f"control audit-renamed-blob: scratch repo, one blob at a.txt and {tool}")
    return {"direct": True, "undo": lambda: _rmtree_ro(repo)}, expect


def _c_audit_shallow(d):  # 4.5 T16.2 G11: the history scope on a shallow clone refuses
    repo, shallow = d / "repo", d / "shallow"
    env, git = _scratch_repo(repo, {"a.txt": b"plain text\n"}, commits=2)
    git(["clone", "-q", "--depth", "1", repo.as_uri(), str(shallow)], d)
    is_shallow = audit_public._git(["rev-parse", "--is-shallow-repository"], shallow, env).strip().decode()

    def expect(r, v, g, i):
        why = _refusal(audit_public.audit, "history", shallow, env, set())
        print(f"control audit-shallow: history scope on a clone (shallow {is_shallow}): {why}")
        return is_shallow == "true" and why.startswith("exit 2: ") and "shallow clone" in why, \
            "audit_public history (refused, exit 2, shallow clone)"
    print("control audit-shallow: scratch repo of 2 commits, cloned --depth 1")
    return {"direct": True, "undo": lambda: (_rmtree_ro(repo), _rmtree_ro(shallow))}, expect


def _c_audit_paths_empty(d):  # 4.5 T16.2 G19: `--paths` with no file refuses, never an empty OK scan
    def expect(r, v, g, i):
        why = _refusal(audit_public.parse, ["--paths"])
        print(f"control audit-paths-empty: parse(['--paths']): {why}")
        return why.startswith("exit 2: ") and "--paths needs" in why, "audit_public --paths (refused, exit 2)"
    print("control audit-paths-empty: audit_public argv `--paths` with no file")
    return {"direct": True}, expect


def _c_audit_route_pin(d):  # 4.5 T16.2 G14: an empty literal from _address_rx -> the route pin names it
    orig = audit_public._address_rx

    def unsound(ips):
        rx, literals, fold = orig(ips)
        return rx, (*literals, b""), fold

    def undo():
        audit_public._address_rx = orig

    def expect(r, v, g, i):
        audit_public._address_rx = unsound
        try:
            causes = audit_public._route_pin()
        finally:
            undo()
        print(f"control audit-route-pin: _route_pin() with _address_rx giving an empty literal: {causes}")
        return "_address_rx gives no literal" in causes, "audit_public._route_pin (_address_rx gives no literal)"
    print("control audit-route-pin: audit_public._address_rx -> its literals plus an empty one")
    return {"direct": True, "undo": undo}, expect


def _c_audit_tail(d, config=None):  # 4.1 T3.1: `not checked` lines must not crowd a HIT out of the verdict
    kw, _expect = _c_audit(d)
    p = d / "probe.obj"
    print("control audit-tail: the audit plant with all three value keys null")
    return {**kw, "config": _cfg_null(d, config, audit_public.VALUE_KEYS.values()), "run_audit": True}, \
        lambda r, v, g, i: (g["audit_public"].startswith("fail: rc 1: ") and f"HIT paths magic {p}" in g["audit_public"]
                            and "6 not checked lines dropped" in g["audit_public"],
                            "audit_public (rc 1, HIT kept, 6 lines dropped)")


def _audit_mutant(d, name, old, new, why):
    """kw + expect: the audit run from a scratch tools dir with one mutated line must refuse, naming the route pin."""
    tools = d / "tools"
    tools.mkdir()
    shutil.copy(TOOLS / "vv_inputs.py", tools / "vv_inputs.py")
    src = (TOOLS / "audit_public.py").read_text(encoding="utf-8")
    if src.count(old) != 1:
        raise RuntimeError(f"{name}: mutation text occurs {src.count(old)} times")
    (tools / "audit_public.py").write_text(src.replace(old, new), encoding="utf-8", newline="\n")
    print(f"control {name}: audit run from a scratch copy with one line mutated")
    return {"audit_tools": tools, "run_audit": True}, lambda r, v, g, i: (
        g["audit_public"].startswith("fail: rc 2: ") and "route pin" in g["audit_public"] and why in g["audit_public"],
        f"audit_public (rc 2, route pin: {why})")


def _c_audit_line_pin(d):  # 4.1 T3.1: as audit_public's mutant; the newline leaves its self-test's copy of the text out
    line = "return bool(HEX_RE.search(line) or HEXESC_RE.search(line))"
    return _audit_mutant(d, "audit-line-pin", line + "\n", line[:-1] + " or OVERSIZE < 0)\n", "_is_hex reads a name")


def _c_audit_literal_pin(d):  # 4.1 T3.1: as audit_public's mutant
    line = 'return _literal_rx([name.encode()], rb"(?<![A-Za-z0-9_])", rb"(?![A-Za-z0-9_])", re.I)'
    return _audit_mutant(d, "audit-literal-pin", line, line + "[0], (), True", "_name_rx gives no literal")


def _prov(checks, kw, at):
    """kw + expect: `provenance` fails (not refused) and the lint reported a FAIL line of each check in checks at
    at[check] (`<path>:<line>` of the plant, or `<path>` for accepted)."""
    def expect(r, v, g, i):
        # 3.5 T4 C7; was: any FAIL line of the check (startswith f"FAIL {c} ")
        got = {c: sum(1 for f in i.get("provenance", []) if f.startswith(f"FAIL {c} {at[c]}:")) for c in checks}
        print(f"control prov: FAIL lines at the plant {got}")
        gv = g.get("provenance", "pass")
        return gv != "pass" and "REFUSED" not in gv and all(got.values()), "provenance " + " + ".join(checks)
    return kw, expect


def _line_of(path, text):  # 1-based number of the one line of path holding text (3.5 T4 C7)
    ns = [n + 1 for n, ln in enumerate(path.read_text(encoding="utf-8").splitlines()) if text in ln]
    if len(ns) != 1:
        raise RuntimeError(f"{path}: {text!r} on {len(ns)} lines")
    return ns[0]


def _prov_plant(d, old, new, checks, marks=None):  # marks: {check: text of its planted line} (default: the last line)
    s = _src_copy(d)
    _edit_text(s / LOC_SRC, old, new)
    last = new.strip().splitlines()[-1]
    at = {c: f"src/{LOC_SRC.as_posix()}:{_line_of(s / LOC_SRC, (marks or {}).get(c, last))}" for c in checks}
    print(f"control prov: planted in {LOC_SRC.as_posix()}: {last!r}; want FAIL at {at}")
    return _prov(checks, {"src_root": s}, at)


TAIL = "    return -1;\n}\n"  # end of LocString_GetStringID, inside its FUNCTION body (to EOF)


def _c_prov_hedge(d):
    return _prov_plant(d, TAIL, TAIL + "// this is probably a planted hedge\n", ("hedge",))


def _c_prov_proto(d):
    return _prov_plant(d, TAIL, TAIL + "struct ZzGateNotInPdb; // NAME: proto-pdb:default.pdb\n", ("proto-pdb",))


def _c_prov_name(d):
    return _prov_plant(d, TAIL, TAIL + "struct ZzGateNotInXbe; // NAME: retail\n", ("name-retail",))


def _c_prov_place(d):
    return _prov_plant(d, TAIL, TAIL + "extern int function_00000001;\n", ("placeholder",))


def _c_prov_src(d):
    return _prov_plant(d, TAIL, TAIL + '// src proto-pdb:default.pdb ZzGateNotInPdb\n'
                       '// src retail 0x00140B98 "zz_gate_not_retail"\n', ("src-pdb", "src-retail"),
                       {"src-pdb": "ZzGateNotInPdb", "src-retail": "zz_gate_not_retail"})


def _c_prov_fake(d):
    s, b = _src_copy(d), d / "base"
    shutil.copytree(FIX_SRC, b)
    _edit_text(b / LOC_SRC, TAIL, "    // !FAKE: c2 register allocation; gate control\n" + TAIL)
    print(f"control prov: lint base {LOC_SRC.as_posix()} has a `// !FAKE:` line the src copy lacks")
    return _prov(("fake-diff",), {"src_root": s, "lint_base": b},
                 {"fake-diff": f"src/{LOC_SRC.as_posix()}:{_line_of(b / LOC_SRC, '!FAKE: c2 register allocation; gate')}"})


def _c_prov_locals(d):
    return _prov_plant(d, "{\n    NLocString::BLOBDATA* p", "{\n    volatile int zz_gate_local = 0;\n"
                       "    NLocString::BLOBDATA* p", ("locals",), {"locals": "zz_gate_local"})


def _c_prov_d66(d):
    return _prov_plant(d, TAIL, TAIL + "extern volatile int zz_gate_d66;\n", ("d66",))


def _c_prov_forceinline(d):  # 4.2 T5 (D116, D117): refused even under a `// !FAKE:` label
    return _prov_plant(d, TAIL, TAIL + "struct ZzGateForceinline {\n    // !FAKE: inliner; gate control\n"
                       "    __forceinline int zz_gate_forceinline() const { return 0; }\n};\n", ("forceinline",),
                       {"forceinline": "__forceinline int zz_gate_forceinline"})


def _c_prov_pragma(d):  # 4.5 T16.2 G9 (D116): an in-body `#pragma` is refused even under a `// !FAKE:` line
    return _prov_plant(d, TAIL, "    // !FAKE: warning; gate control\n#pragma warning(disable:4100)\n" + TAIL,
                       ("pragma",), {"pragma": "#pragma warning(disable:4100)"})


def _c_prov_noinline(d):  # 4.5 T16.2 G9 (D116): `noinline` is refused even under a `// !FAKE:` line
    return _prov_plant(d, TAIL, TAIL + "// !FAKE: inliner; gate control\n__declspec(noinline) int zz_gate_noinline();\n",
                       ("noinline",))


def _c_prov_include_body(d):  # 4.5 T16.2 G18: a header `#define` carrying `volatile` FAILs d66 and a `/FI` opt is
    s = _src_copy(d)          # refused by build.flags_of (the lint never reads a force-included header)
    h = s / LOC_SRC.parent / "zz_gate_include.h"
    h.write_text("// gate control: a header macro adds a qualifier to a body\n#define ZZ_GATE_VOL volatile\n",
                 encoding="utf-8", newline="\n")
    rel = f"src/{(LOC_SRC.parent / h.name).as_posix()}"
    print(f"control prov: planted {rel}: '#define ZZ_GATE_VOL volatile'; want FAIL d66 at {rel}:2 and a /FI refusal")
    kw, lint_expect = _prov(("d66",), {"src_root": s}, {"d66": f"{rel}:2"})

    def expect(r, v, g, i):
        ok, at = lint_expect(r, v, g, i)
        try:
            build.flags_of({"name": "zz_gate", "opt": "/O2 /FIzz_gate_include.h"})
            why = ""
        except build.Refuse as e:
            why = str(e)
        print(f"control prov: flags_of /FI -> {why or 'accepted'}")
        return ok and "/FIzz_gate_include.h" in why and "steers" in why, at + " + build.flags_of /FI refused"
    return kw, expect


def _c_prov_accepted(d):
    a = d / "accepted.txt"
    a.write_text(FIX_ACC.read_text(encoding="utf-8") + f"src/{LOC_SRC.as_posix()}\t    return -1; probably\tgate "
                 f"control\n", encoding="utf-8", newline="\n")
    print(f"control prov: scratch accepted entry `return -1; probably` for src/{LOC_SRC.as_posix()}")
    return _prov(("accepted",), {"lint_accepted": a}, {"accepted": f"src/{LOC_SRC.as_posix()}"})


def _c_prov_locals_cache(d, config=None):  # 3.5 T4 F5: the prov-locals plant + the clean source's obj planted under
    kw, expect = _c_prov_locals(d)        # the planted source's cache key (cache dir redirected to scratch, undo)
    old = provenance_lint.LOCALS_CACHE
    provenance_lint.LOCALS_CACHE = d / "locals_cache"
    try:
        flags = build.flags_of(build.unit_map(build.UNITS)[LOC_SRC.as_posix().lower()][0])
        clean = provenance_lint.locals_obj(FIX_SRC / LOC_SRC, flags,
                                           Path(vv_inputs.load_config(config)["msvc71_dir"]))
        plant = provenance_lint.LOCALS_CACHE / f"{provenance_lint.locals_key(kw['src_root'] / LOC_SRC, flags)}.obj"
        shutil.copyfile(clean, plant)
    except BaseException:
        provenance_lint.LOCALS_CACHE = old
        raise
    print("control prov-locals-cache: the clean source's /Z7 obj planted under the planted source's cache key")
    return {**kw, "undo": lambda: setattr(provenance_lint, "LOCALS_CACHE", old)}, expect


def _c_prov_inputs_pin(d, config=None):  # 3.5 T4 F6: config copy, prototype_dir -> a dir whose default.pdb differs
    cfg = vv_inputs.load_config(config)
    p = d / "proto"
    p.mkdir()
    b = bytearray(vv_inputs.input_path("prototype_dir", "default.pdb", cfg).read_bytes())
    b[-1] ^= 0xFF
    (p / "default.pdb").write_bytes(bytes(b))
    shutil.copyfile(vv_inputs.input_path("prototype_dir", "default.exe", cfg), p / "default.exe")
    print("control prov-inputs-pin: config copy, prototype_dir -> a copy whose default.pdb has its last byte flipped")
    return {"config": _cfg_copy(d, config, "prototype_dir", p)}, lambda r, v, g, i: (
        r == 2 and g.get("provenance", "").startswith("fail: REFUSED") and "default.pdb" in g.get("provenance", ""),
        "provenance REFUSED (default.pdb)")


def _c_prov_fake_head(d):  # 3.5 T4 W5: scratch git repo; a `// !FAKE:` line committed, then removed in a commit
    r, fake = d / "repo", "    // !FAKE: c2 register allocation; gate control\n"
    s = r / "src"
    shutil.copytree(FIX_SRC, s)

    def git(*a):
        subprocess.run(["git", "-C", str(r), "-c", "user.name=gate", "-c", "user.email=gate@control.invalid",
                        "-c", "commit.gpgsign=false", "-c", "core.autocrlf=false", *a], check=True,
                       capture_output=True)
    git("init", "-q")
    _edit_text(s / LOC_SRC, TAIL, fake + TAIL)
    line = _line_of(s / LOC_SRC, fake.strip())
    git("add", "-A")
    git("commit", "-q", "--no-verify", "-m", "base: fake line")
    git("branch", "zz-base")
    _edit_text(s / LOC_SRC, fake + TAIL, TAIL)
    git("commit", "-q", "--no-verify", "-am", "fake line removed")
    git("branch", "--set-upstream-to=zz-base")
    print("control prov-fake-head: scratch repo, `// !FAKE:` committed (upstream zz-base) then removed at HEAD")
    kw, expect = _prov(("fake-diff",), {"src_root": s}, {"fake-diff": f"src/{LOC_SRC.as_posix()}:{line}"})
    # undo: git's read-only object files would survive the runner's rmtree(ignore_errors) on Windows
    return {**kw, "undo": lambda: shutil.rmtree(r, onexc=lambda f, p, e: (os.chmod(p, 0o700), f(p)))}, expect


def _prov_exact(check, kw, want):  # 4.1 T4: kw + expect: `provenance` fails (not refused) with check's FAIL lines at
    def expect(r, v, g, i):        # exactly want (`<path>:<line>`), none more; plain _prov asks only for one at the plant
        pre = f"FAIL {check} "
        got = sorted(f[len(pre):].split(": ", 1)[0] for f in i.get("provenance", []) if f.startswith(pre))
        print(f"control prov: {check} FAIL lines {got}; want {sorted(want)}")
        gv = g.get("provenance", "pass")
        return gv != "pass" and "REFUSED" not in gv and got == sorted(want), f"provenance {check} exactly {len(want)}"
    return kw, expect


def _fake_base(d, base_edits, src_edits, text):  # 4.1 T4: lint base and src copies of LOC_SRC edited; want the base line of text
    s, b = _src_copy(d), d / "base"
    shutil.copytree(FIX_SRC, b)
    for old, new in base_edits:
        _edit_text(b / LOC_SRC, old, new)
    for old, new in src_edits:
        _edit_text(s / LOC_SRC, old, new)
    return _prov_exact("fake-diff", {"src_root": s, "lint_base": b},
                       [f"src/{LOC_SRC.as_posix()}:{_line_of(b / LOC_SRC, text)}"])


def _c_prov_fake_reword(d):  # 4.1 T4: two labels in one function, the first removed, the second reworded
    top = "{\n    NLocString::BLOBDATA* p"
    print("control prov: base labels gone and kept-reworded, one fake-diff FAIL at the removed one")
    return _fake_base(d, ((top, "{\n    // !FAKE: c2 gate control, removed\n    NLocString::BLOBDATA* p"),
                          (TAIL, "    // !FAKE: c2 gate control, kept\n" + TAIL)),
                      ((TAIL, "    // !FAKE: c2 gate control, kept and reworded\n" + TAIL),),
                      "!FAKE: c2 gate control, removed")


def _c_prov_fake_trailing(d):  # 4.1 T4: a trailing label on a line d66 does not match, stripped
    print("control prov: base `return -1; // !FAKE:`, stripped in the src copy")
    return _fake_base(d, ((TAIL, "    return -1; // !FAKE: c2 gate control\n}\n"),), (), "!FAKE: c2 gate control")


def _c_prov_fake_comment_excuse(d):  # 4.1 T4: own-line label removed; only a trailing comment edited in its function
    print("control prov: base own-line label removed, a trailing comment edited in the same function")
    return _fake_base(d, ((TAIL, "    // !FAKE: c2 gate control\n    return -1; // a note\n}\n"),),
                      ((TAIL, "    return -1; // a note, edited\n}\n"),), "!FAKE: c2 gate control")


def _c_prov_name_ns(d):  # 4.1 T4: both NAME causes in `namespace std`; right names pass, wrong ones and a closed namespace fail
    s = _src_copy(d)
    nm = "// NAME: proto-pdb:default.pdb\n"
    _edit_text(s / LOC_SRC, TAIL, TAIL + "#ifdef NON_MATCHING\nnamespace std {\n"
               + nm + "template void vector<Matrix>::resize(size_type);\n"
               + nm + "template vector<Matrix>::size_type vector<Matrix>::size() const;\n"
               + nm + "template void vector<Matrix>::zzGateWrong(size_type);\n"
               + nm + "template vector<Matrix>::size_type vector<Matrix>::zzGateWrongScoped() const;\n}\n"
               + nm + "template void vector<Matrix>::capacity() const;  // zzGateClosed\n#endif\n")
    # FAIL names the NAME line, directly above the code line
    want = [f"src/{LOC_SRC.as_posix()}:{_line_of(s / LOC_SRC, t) - 1}"
            for t in ("zzGateWrong(", "zzGateWrongScoped(", "zzGateClosed")]
    print(f"control prov: namespace std plants in {LOC_SRC.as_posix()}; want proto-pdb exactly at {want}")
    return _prov_exact("proto-pdb", {"src_root": s}, want)


def _c_prov_d66_pdb_volatile(d, config=None):  # 4.1 T4 (M21): the real unit, labels stripped, the row in a scratch list; the
    rel = Path("Libs") / "BeepLib" / "Xbox" / "Official_Release" / "BeepAlloc.cpp"  # PDB types x, y volatile, d66 still refuses
    s, lp, acc = d / "src", d / "matched_set.toml", d / "accepted.txt"
    (s / rel).parent.mkdir(parents=True)
    shutil.copyfile(SRC / rel, s / rel)
    shutil.copytree(SRC / "include", s / "include")
    _edit_text(s / rel, "        // !FAKE: c1xx front end; volatile local; not match-only: the PDB types x and y `volatile "
               "int`\n        // (pdb_types locals AllocateMemory), the marker is for the d66 lint\n", "")
    _edit_text(s / rel, "volatile int y = 1 / x; // !FAKE: c1xx front end; volatile local; as x above (PDB-typed, d66 "
               "lint marker)", "volatile int y = 1 / x;")
    m = re.search(r'\[\[function\]\]\nsymbol = "\?AllocateMemory@[^\n]*\n(?:[^\n]+\n)+', LIST.read_text(encoding="utf-8"))
    lp.write_text(m.group(0), encoding="utf-8", newline="\n")
    acc.write_text("", encoding="utf-8", newline="\n")
    want = [f"src/{rel.as_posix()}:{_line_of(s / rel, t)}" for t in ("volatile int x = 0;", "volatile int y = 1 / x;")]
    kw, exact = _prov_exact("d66", {"src_root": s, "list_path": lp, "lint_accepted": acc}, want)

    def expect(r, v, g, i):
        ok, at = exact(r, v, g, i)
        typed = {x["name"]: provenance_lint.pdb_types.tname(x["ti"])
                 for rec in provenance_lint._INPUTS[config][0].locals_of("AllocateMemory") for x in rec["locals"]}
        print(f"control prov: PDB AllocateMemory x {typed.get('x')!r}, y {typed.get('y')!r}")
        return ok and all(typed.get(n, "").startswith("volatile ") for n in "xy"), at + " on PDB-volatile locals"
    return kw, expect


def _c_annotations_folded(d):  # 4.1 T4: annotations.check direct on scratch src and list
    lp, head = d / "matched_set.toml", "// FUNCTION: USA 0x00140B98"
    shutil.copyfile(FIX_LIST, lp)

    def fails(name, first, second):
        s = d / name
        shutil.copytree(FIX_SRC, s)
        _edit_text(s / LOC_SRC, f"{head}\n", f"{head}{first}\n// NAME: proto-pdb:default.pdb\n{head}{second}\n")
        return [f for f in annotations.check(s, lp)[0] if "00140B98" in f]

    def norow(name, tail):  # the same pair on a VA no row names, at the end of the file
        s = d / name
        shutil.copytree(FIX_SRC, s)
        line = f"// FUNCTION: USA 0x{ANN_NO_ROW:08X}{tail}\n// NAME: proto-pdb:default.pdb\n"
        _ann_append(s, line + line)
        return annotations.check(s, lp)[0]

    def expect(r, v, g, i):
        pair, mixed, bare = fails("pair", " FOLDED", " FOLDED"), fails("mixed", " FOLDED", ""), fails("bare", "", "")
        rowless, rowless_ok = norow("norow", ""), norow("norow_folded", " FOLDED")
        print(f"control annotations-folded: both FOLDED {pair}; one FOLDED {mixed}; bare {bare}; "
              f"no row bare {rowless}; no row both FOLDED {rowless_ok}")
        return (not pair and any("need 1" in f for f in mixed) and any("need 1" in f for f in bare)
                and len(rowless) == 1 and "need 1" in rowless[0] and f"{ANN_NO_ROW:08X}" in rowless[0]
                and not rowless_ok), "annotations count rule"
    return {"direct": True}, expect


ANN_NO_ROW = 0x0FFFFFF0  # 4.1 T4: a VA no fixture row names
ANN_BAD = "// FUNCTION: USA 0x00140b98\n"  # 4.1 T4: lower-case hex, malformed
ANN_NAME = '// NAME: proto-pdb:default.pdb\nextern "C" int LocString_GetStringID'  # 4.1 T4: the row's NAME line, its code


def _ann_append(s, text):
    with open(s / LOC_SRC, "a", encoding="utf-8", newline="\n") as f:
        f.write("\n" + text)


def _ann_no_name(s):
    _edit_text(s / LOC_SRC, ANN_NAME, ANN_NAME.split("\n", 1)[1])


def _ann_one(d, name, edit, want):  # 4.1 T4: annotations.check direct on a planted scratch src: exactly one FAIL, holding want
    def expect(r, v, g, i):
        s = _src_copy(d)
        edit(s)
        fails = annotations.check(s, FIX_LIST)[0]
        print(f"control annotations-{name}: {fails}")
        return len(fails) == 1 and want in fails[0], f"annotations {want}"
    return {"direct": True}, expect


def _c_annotations_malformed(d):
    return _ann_one(d, "malformed", lambda s: _ann_append(s, ANN_BAD), "malformed FUNCTION line")


def _c_annotations_no_name(d):
    return _ann_one(d, "no-name", _ann_no_name, "0x00140B98 not followed by")


def _c_annotations_no_source(d):
    return _ann_one(d, "no-source", lambda s: (s / LOC_SRC).unlink(), "no source")


def _c_annotations_count(d):  # a matched row with 0 FUNCTION lines
    return _ann_one(d, "count", lambda s: _edit_text(s / LOC_SRC, "// FUNCTION: USA 0x00140B98\n", ""),
                    "0 `// FUNCTION: USA 0x00140B98` lines")


def _c_annotations_order(d):  # 4.1 T4: order never fails: one real failure beside it is the only FAIL; gone, no FAIL
    def expect(r, v, g, i):
        def run(name, tail):
            s = d / name
            shutil.copytree(FIX_SRC, s)
            _ann_append(s, "// FUNCTION: USA 0x00000010\n// NAME: proto-pdb:default.pdb\n" + tail)
            res = annotations.check(s, FIX_LIST)
            return res[0], [o for o in res[3] if o.startswith(f"ORDER {LOC_SRC.as_posix()}:")
                            and "0x00000010 not ascending" in o]
        (both, bo), (alone, ao) = run("both", ANN_BAD), run("alone", "")
        print(f"control annotations-order: order plus real {both} {bo}; order alone {alone} {ao}")
        return (len(both) == 1 and "malformed" in both[0] and len(bo) == 1 and not alone and len(ao) == 1), \
            "annotations order advisory"
    return {"direct": True}, expect


def _c_annotations_row(d):  # 4.1 T4: through _core: rc 1, the annotations global fails on the one line
    s = _src_copy(d)
    _ann_no_name(s)
    print(f"control annotations-row: NAME line of 0x00140B98 removed in {LOC_SRC.as_posix()}")
    return {"src_root": s}, lambda r, v, g, i: (
        r == 1 and g.get("annotations") == "fail: 1 failure(s)" and len(i.get("annotations", [])) == 1
        and "0x00140B98 not followed" in i["annotations"][0], "annotations global")


def _prov_patched(name, fake, want, rc):  # 3.5 T4 N8: provenance_lint.lint replaced (restored by the runner's undo)
    orig = provenance_lint.lint
    provenance_lint.lint = fake
    print(f"control prov-{name}: provenance_lint.lint replaced by a fake")
    return {"undo": lambda: setattr(provenance_lint, "lint", orig)}, lambda r, v, g, i: (
        r == rc and g.get("provenance", "pass").startswith(want), f"provenance ({want}, rc {rc})")


def _c_prov_refused(d):
    def refuse(*a, **k):
        raise provenance_lint.Refuse("gate control prov-refused")
    return _prov_patched("refused", refuse, "fail: REFUSED gate control", 2)


def _c_prov_error(d):
    def boom(*a, **k):
        raise RuntimeError("gate control prov-error")
    return _prov_patched("error", boom, "fail: provenance error RuntimeError", 1)


def _layer_patched(mod, attr, key, name, fake, want, rc):  # 4.5 T16.2.c3 (G14): mod.attr replaced (runner's undo)
    orig = getattr(mod, attr)
    setattr(mod, attr, fake)
    print(f"control {name}: {mod.__name__}.{attr} replaced by a fake")
    return {"undo": lambda: setattr(mod, attr, orig), "run_pins": key == "pin_rows"}, lambda r, v, g, i: (
        r == rc and g.get(key, "pass").startswith(want), f"{key} ({want}, rc {rc})")


def _c_annotations_refused(d):
    def refuse(*a, **k):
        raise matched_set.Refuse("gate control annotations-refused")
    return _layer_patched(annotations, "check", "annotations", "annotations-refused", refuse,
                          "fail: REFUSED gate control annotations-refused", 2)


def _c_annotations_error(d):
    def boom(*a, **k):
        raise RuntimeError("gate control annotations-error")
    return _layer_patched(annotations, "check", "annotations", "annotations-error", boom,
                          "fail: annotations error RuntimeError: gate control annotations-error", 1)


def _c_pin_refused(d):
    def refuse(*a, **k):
        raise pin_rows.Refuse("gate control pin-refused")
    return _layer_patched(pin_rows, "check", "pin_rows", "pin-refused", refuse,
                          "fail: REFUSED gate control pin-refused", 2)


def _c_pin_error(d):
    def boom(*a, **k):
        raise RuntimeError("gate control pin-error")
    return _layer_patched(pin_rows, "check", "pin_rows", "pin-error", boom,
                          "fail: pin_rows error RuntimeError: gate control pin-error", 1)


def _c_pin_fold_site(d):
    """kw `direct` (4.5 T16.2.c3, G14, D119): pin_rows.State.site_test on a stub State, one proto public P with two
    names: fold sites alone -> site-conflict-only; a fold site beside an agreeing site passes; a site whose rd is in
    tab_va is no fold site -> site-conflict-both."""
    p, va, rd, n = 0x00011000, 0x00011000, 0x00022000, "?a@@YAXXZ"

    def run(case, sites, tab):
        s = types.SimpleNamespace(by_pd={p: {x: ["call"] for x in sites}}, pubs={p: {n, "?b@@YAXXZ"}}, tab_va=tab,
                                  by_rd={}, accepted=set())
        r = {"name": n, "va": va, "protos": [p], "fails": [], "notes": []}
        pin_rows.State.site_test(s, r)
        print(f"control pin-fold-site: {case}: site {r['site']} fails {r['fails']}")
        return r["fails"]

    def expect(r_, v, g, i):
        return (run("fold only", [rd], {}) == ["site-conflict-only"] and run("fold + agree", [va, rd], {}) == []
                and run("rd in tab_va + agree", [va, rd], {rd: {"?c@@YAXXZ"}}) == ["site-conflict-both"]), \
            "pin_rows.site_test fold sites"
    return {"direct": True}, expect


PIN_UNSIZED = "__EH_prolog"  # symbols_pin.txt func, size `-`, disasm-verified (3.4 T6.1)
PIN_SITE = "?GetTextExtent@CXBFont@@QBEJPBGPAM1H@Z"  # symbols_pin.txt func, site-corr
PIN_ALIAS_VA, PIN_ALIAS = 0x000292A6, ("??1?$ResourceHandle@VAnimationResource@@@@QAE@XZ",
                                       "??1?$ResourceHandle@VTextureResource@@@@QAE@XZ")  # fold aliases, C0130


def _pin_rename(d, name):
    """Scratch list copy with FLOUR_SYM's row renamed to the pin name `name` (va/size/hashes kept)."""
    lp = d / "matched_set.toml"
    lp.write_text(FIX_LIST.read_text(encoding="utf-8").replace(f"symbol = {json.dumps(FLOUR_SYM)}\n",
                                                               f"symbol = {json.dumps(name)}\n", 1),
                  encoding="utf-8", newline="\n")
    print(f"control pin: list copy, {FLOUR_SYM} row renamed to pin {name}")
    return lp


def _c_pin_unsized(d):
    return {"list_path": _pin_rename(d, PIN_UNSIZED)}, lambda r, v, g, i: (
        "pin has no size" in v[PIN_UNSIZED]["size"], f"{PIN_UNSIZED} size (unsized pin)")


def _c_pin_site(d):
    return {"list_path": _pin_rename(d, PIN_SITE)}, lambda r, v, g, i: (
        "pin status site-corr" in v[PIN_SITE]["size"], f"{PIN_SITE} size (site-corr pin)")


def _c_pin_alias_dup(d):
    lp = d / "matched_set.toml"
    shutil.copy2(FIX_LIST, lp)
    a, b = PIN_ALIAS
    matched_set.add_function(lp, a)
    try:
        matched_set.add_function(lp, b)
        add_why = None
    except matched_set.Refuse as e:
        add_why = str(e)
    print(f"control pin-alias-dup: list copy + {a}; add {b} at the same VA: {add_why or 'NOT refused'}")
    text = lp.read_text(encoding="utf-8")
    lp.write_text(text + text[text.rindex("\n[[function]]\n"):].replace(json.dumps(a), json.dumps(b)),
                  encoding="utf-8", newline="\n")
    print(f"control pin-alias-dup: {b} row appended beside it by hand (0x{PIN_ALIAS_VA:08X} twice)")
    try:
        matched_set._parse(lp)
        why = ""
    except matched_set.Refuse as e:
        why = str(e)
    return {"list_path": lp}, lambda r, v, g, i: (
        r == 2 and f"0x{PIN_ALIAS_VA:08X} already in" in (add_why or "") and f"duplicate va 0x{PIN_ALIAS_VA:08X}" in why,
        "add refusal + list (duplicate va)")


# 4.2 T5 pin-row plants (c2/c3, D119): (id, VA of the row edited (first pin row there), field edits ("+": the edited
# copy inserted after the row; tag None: the column dropped), the plant's own fail class, the VA it fails at)
PIN_FIELDS = ("va", "kind", "size", "name", "tag", "status")
_PP = "proto-pdb-public+"
PIN_PLANTS = (
    ("H1", 0x000FFD8C, {"status": "disasm-verified"}, "status-vs-tag", 0x000FFD8C),
    ("H2", 0x000FFD8C, {"status": "disasm-verified", "tag": _PP + "site-corr+callee-bytes"}, "route-func", 0x000FFD8C),
    ("H3", 0x000FFD8C, {"status": "disasm-verified", "tag": _PP + "retail-xref"}, "hand-name-not-in-evidence",
     0x000FFD8C),
    ("H4", 0x001C8648, {"status": "disasm-verified", "tag": _PP + "site-corr+data-bytes"}, "route-global", 0x001C8648),
    ("H5", 0x001C8648, {"status": "disasm-verified", "tag": _PP + "retail-disasm"}, "hand-name-not-in-evidence",
     0x001C8648),
    ("T1", 0x00027A42, {"tag": None}, "grammar", 0x00027A42),
    ("N1", 0x00027A42, {"name": "?NotAPublic@@YAXXZ"}, "name-not-public", 0x00027A42),
    ("N2", 0x000E0887, {"name": "?NotAPublic@@YAXXZ"}, "name-not-public", 0x000E0887),
    ("N3", 0x000220C6, {"name": "?NotAPublic@@YAXXZ"}, "route-label", 0x000220C6),
    ("N4", 0x001EA324, {"name": "_s_00000000"}, "route-label", 0x001EA324),
    ("V1", 0x000E0887, {"va": "0x000E0518"}, "site-conflict-only", 0x000E0518),
    ("V2", 0x00027A42, {"va": "0x000BFFD7"}, "route-func", 0x000BFFD7),
    ("V3", 0x00027A42, {"va": "0x7FFF0000"}, "va-off-image", 0x7FFF0000),
    ("V4", 0x002369CC, {"va": "0x002369D0"}, "site-conflict-only", 0x002369D0),
    ("V5", 0x001C8648, {"va": "0x001C864C"}, "site-conflict-only", 0x001C864C),
    ("Wa1", 0x000E0887, {"name": "?RunLevel@@YAXXZ"}, "site-conflict-only", 0x000E0887),
    ("Wa2", 0x00027A42, {"name": "?RunLevel@@YAXXZ"}, "route-func", 0x00027A42),
    ("Wa3", 0x00018B34, {"name": "?RunLevel@@YAXXZ"}, "site-conflict-only", 0x00018B34),
    ("Wa4", 0x00011178, {"name": "?RunLevel@@YAXXZ"}, "fold-other-name-not-alias", 0x00011178),
    ("Wb1", 0x002369CC, {"name": "_g_pfnxcsuOnline"}, "site-conflict-only", 0x002369CC),
    ("Wb2", 0x001CAE20, {"name": "__real@40e00000"}, "route-float", 0x001CAE20),
    ("Wb3", 0x002231E0, {"name": "_g_pfnxcsuOnline"}, "site-conflict-only", 0x002231E0),
    ("Wb4", 0x002231E0, {"name": "__imp__XcVerifyPKCS1Signature@12"}, "section-differs", 0x002231E0),
    ("Wb5", 0x00225748, {"name": "__imp__XcVerifyPKCS1Signature@12"}, "section-differs", 0x00225748),
    ("D1", 0x00027A42, {"+": True}, "dup-same-row", 0x00027A42),
    ("D2", 0x00027A42, {"+": True, "va": "0x00011000"}, "dup-name-two-vas", 0x00011000),
    ("D3", 0x00027A42, {"+": True, "name": "?RunLevel@@YAXXZ"}, "va-two-names", 0x00027A42),
    ("Tw1", 0x001253D7, {"name": "_XShaHmacUpdate@12"}, "route-func", 0x001253D7),
    # 4.5 T16.1 (G2, G3): a site-less near VA; a global row moved 4 bytes onto unequal data
    ("near", 0x000688EA, {"va": "0x000688EB"}, "route-near", 0x000688EB),
    ("V6", 0x001C7410, {"va": "0x001C7414"}, "route-global", 0x001C7414),
)
PIN_UNRULED = "??1Diary@@QAE@XZ"  # D118: sized disasm-verified sole-name func pin 0x0010027B, its row passes pin_rows


def _pin_scratch(d, path, text):
    """Write a scratch copy under the control's dir d, never over the real file (C0177: assert the target first)."""
    p = (d / path.name).resolve()
    assert p.is_relative_to(d.resolve()) and p != path.resolve(), f"pin control scratch {p} outside {d}"
    p.write_text(text, encoding="utf-8", newline="\n")
    return p


def _pin_new(pid, base, s):
    """Fail lines of State s not in base's (line numbers dropped), each printed under the control."""
    key = lambda x: re.sub(r" \S+\.txt:\d+:", ":", x)  # noqa: E731
    before = {key(x) for x in base.row_fail_lines()}
    new = [x for x in s.row_fail_lines() if key(x) not in before]
    for x in new:
        print(f"control {pid}: {x}")
    return new


def _pin_plant(pid, va, edits, cls, at):
    """kw `direct`: the pin file copy with one plant, rows re-run in-process (State.replant) -> a new fail line of
    class cls at VA at."""
    def make(d, config=None):
        base = pin_rows.state(config)
        L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
        i = next(k for k, ln in enumerate(L) if ln.startswith(f"0x{va:08X} "))
        f, e = L[i].split(), dict(edits)
        ins = e.pop("+", False)
        for k, v in e.items():
            f[PIN_FIELDS.index(k)] = v
        new = " ".join(x for x in f if x is not None)
        L[i:i + 1] = [L[i], new] if ins else [new]
        p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
        print(f"control pin-plant-{pid}: pin copy, row 0x{va:08X} {'+ inserted' if ins else 'edited'} {sorted(e)}")

        def expect(r, v, g, i_):
            new_lines = _pin_new(f"pin-plant-{pid}", base, base.replant(p))
            return (any(x.split()[2:4] == [cls, f"0x{at:08X}"] for x in new_lines),
                    f"pin-row {cls} 0x{at:08X}")
        return {"direct": True}, expect
    return make


def _c_pin_literal_list(d, config=None):
    """Through _core with the pin layer: the accepted list copy waives a non-literal row (?handledMain@@YAXXZ) ->
    pin_rows fails `accepted-stale` at it; pin_rows' cached State swapped for the copy's (restored by undo)."""
    base = pin_rows.state(config)
    acc = _pin_scratch(d, pin_rows.ACCEPTED, pin_rows.ACCEPTED.read_text(encoding="utf-8")
                       + "0x000E0887\t?handledMain@@YAXXZ\tgate control: a waiver for a non-literal row\n")
    pin_rows._STATE[config] = base.replant(pin_rows.PIN, acc)
    print("control pin-literal-list: accepted list copy + a waiver of non-literal 0x000E0887")
    return {"undo": lambda: pin_rows._STATE.__setitem__(config, base)}, lambda r, v, g, i: (
        g.get("pin_rows", "pass").startswith("fail: ")
        and any(x.split()[2:4] == ["accepted-stale", "0x000E0887"] for x in i.get("pin_rows", [])),
        "pin_rows accepted-stale 0x000E0887")


def _c_pin_unruled(d, config=None):
    """kw `direct` (D118): a scratch pin copy with PIN_UNRULED's tag forged to a hand xref tag; matched_set add of it
    on a list copy is refused naming the pin-row check; the same add with the real pins passes (restored by undo)."""
    base, pins0 = pin_rows.state(None), matched_set.PINS
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    i = next(k for k, ln in enumerate(L) if ln.split()[3:4] == [PIN_UNRULED])
    f = L[i].split()
    f[4] = _PP + "retail-xref"
    L[i] = " ".join(f)
    p = _pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n")
    lp = d / "matched_set.toml"
    shutil.copy2(FIX_LIST, lp)

    def undo():
        matched_set.PINS = pins0
        pin_rows._STATE[None] = base

    def expect(r, v, g, i_):
        matched_set.PINS, pin_rows._STATE[None] = p, base.replant(p)
        try:
            matched_set.add_function(lp, PIN_UNRULED)
            why = None
        except matched_set.Refuse as e:
            why = str(e)
        undo()
        print(f"control pin-unruled: forged pin, add {PIN_UNRULED}: {why or 'NOT refused'}")
        matched_set.add_function(lp, PIN_UNRULED)  # real pins: D118 lifts the refusal (raises if not)
        print(f"control pin-unruled: real pins, add {PIN_UNRULED}: added")
        return bool(why) and "pin-row check: " in why, "matched_set add refused (pin-row check)"
    return {"direct": True, "undo": undo}, expect


def _c_pin_i10_static(d, config=None):
    """kw `direct` (4.5 T16.2.c4, G14): the fixture built; a same-section static a fixture function relocates through
    inside its own extent, given a pin row (status site-corr) on a State stub -> I10 fails `i10` at it
    (byte_compare.resolve takes a tabled static by name); the fixture's untabled same-section statics stay skipped."""
    base, bdir = pin_rows.state(config), d / "build"
    build.build(FIX_SRC, bdir, build.UNITS, config)
    statics = []
    for e in matched_set.load(FIX_LIST):
        if e["kind"] != "function":
            continue
        c = byte_compare.load_obj(bdir / e["unit"].replace("/", "\\").rsplit("\\", 1)[-1])[1]
        f = next(y for y in c["symbols"].values() if y["name"] == e["symbol"] and y["section"] > 0)
        sec, lo, hi = byte_compare.extent(c, f)
        statics += [y["name"] for rl in sec["relocs"] if lo <= rl["offset"] < hi
                    for y in [c["symbols"][rl["symbol"]]] if y["class"] == coff_info.SC_STATIC
                    and y["section"] == f["section"] and y["value"] and lo <= y["value"] < hi
                    and y["name"] not in base.sy_name and y["name"] not in base.by_row_name]
    n = statics[0]
    rows = dict(base.by_row_name)
    rows[n] = [{"va": 0x00011000, "status": "site-corr", "fails": [], "line": 0}]
    print(f"control pin-i10-static: {len(set(statics))} untabled same-section statics; {pin_rows.mask(n)} given a "
          f"site-corr pin row 0x00011000")

    def expect(r, v, g, i_):
        lines = pin_rows.State.i10(types.SimpleNamespace(sy_name=base.sy_name, by_row_name=rows, pin=base.pin),
                                   bdir, FIX_LIST)
        for x in lines:
            print(f"control pin-i10-static: {x}")
        ours = [x for x in lines if x.split()[2:5] == ["i10", "0x00011000", pin_rows.mask(n)]]
        rest = [x for x in lines if x not in ours and any(pin_rows.mask(s) in x.split() for s in statics)]
        return (len(ours) == 1 and "status site-corr" in ours[0] and not rest,
                "pin-row i10 at the tabled same-section static, untabled ones skipped")
    return {"direct": True}, expect


def _c_pin_vtable_prefix(d, config=None):
    """kw `direct` (4.5 T16.2.c4, G14): a passing vtable-literal pin row re-routed on a State copy whose proto slots
    stop one short of retail's (vt_slots less its last slot: the proto table a prefix) ->
    `route-vtable:slot-count-differs`; the same row unplanted passes."""
    base = pin_rows.state(config)
    r0 = next(r for r in base.rows if r.get("cls") and r["cls"][1] == "vtable" and not r["fails"] and r.get("protos")
              and len(base.vt_slots(r["protos"][0], r["va"])) >= 2)

    def run(case, cut):
        s = object.__new__(pin_rows.State)
        s.__dict__.update(vars(base))
        if cut:
            s.vt_slots = lambda p, t: base.vt_slots(p, t)[:-1]
        r = dict(r0, fails=[], notes=[])
        pin_rows.State.route(s, r)
        print(f"control pin-vtable-prefix: {case}: 0x{r0['va']:08X} {pin_rows.mask(r0['name'])} fails {r['fails']}")
        return r["fails"]

    def expect(r_, v, g, i):
        return (run("unplanted", False) == [] and run("proto slots a prefix", True)
                == ["route-vtable:slot-count-differs"]), "pin-row route-vtable:slot-count-differs"
    return {"direct": True}, expect


def _c_pin_accepted_unused(d, config=None):
    """kw `direct` (4.5 T16.2.c4, G14): the accepted list copy waives a live passing literal row no site conflict
    reaches (no `literal-copy-accepted` note) -> `accepted-stale ... unused` at it."""
    base = pin_rows.state(config)
    lit = [r for r in base.rows if r.get("name", "").startswith(("__real@", "??_C@")) and r.get("va") is not None
           and not r["fails"] and "literal-copy-accepted" not in r["notes"] and (r["va"], r["name"]) not in base.accepted]
    r0 = sorted(lit, key=lambda r: (not r["name"].startswith("__real@"), r["va"]))[0]
    acc = _pin_scratch(d, pin_rows.ACCEPTED, pin_rows.ACCEPTED.read_text(encoding="utf-8")
                       + f"0x{r0['va']:08X}\t{r0['name']}\tgate control: a waiver no site conflict uses\n")
    va = f"0x{r0['va']:08X}"
    print(f"control pin-accepted-unused: accepted list copy + a waiver of passing literal {va} {pin_rows.mask(r0['name'])}")

    def expect(r, v, g, i_):
        new = _pin_new("pin-accepted-unused", base, base.replant(pin_rows.PIN, acc))
        return (any(x.split()[2:4] == ["accepted-stale", va] and ": unused " in x for x in new),
                f"pin-row accepted-stale {va} unused")
    return {"direct": True}, expect


def _c_cfg_pin_row(d, config=None):
    """4.5 T16.2.c4 (G15): through _core, a list copy with FLOUR_SYM's row renamed to PIN_UNRULED and a config copy
    (another key: pin_rows' State is per config) whose State is seeded from a pin copy with PIN_UNRULED's tag forged
    to a hand xref tag -> the size check names that config's pin-row failure (matched_set passes the gate's config
    to pin_rows.row_ok); the State is dropped by undo."""
    base, cp = pin_rows.state(config), _cfg_null(d, config, ())
    L = pin_rows.PIN.read_text(encoding="utf-8").splitlines()
    k = next(j for j, ln in enumerate(L) if ln.split()[3:4] == [PIN_UNRULED])
    f = L[k].split()
    f[4] = _PP + "retail-xref"
    L[k] = " ".join(f)
    pin_rows._STATE[cp] = base.replant(_pin_scratch(d, pin_rows.PIN, "\n".join(L) + "\n"))
    print(f"control cfg-pin-row: config copy; its pin-row State from a pin copy, {PIN_UNRULED} tag forged")
    return {"list_path": _pin_rename(d, PIN_UNRULED), "config": cp,
            "undo": lambda: pin_rows._STATE.pop(cp, None)}, lambda r, v, g, i: (
        "pin-row check: hand-name-not-in-evidence" in v.get(PIN_UNRULED, {}).get("size", ""),
        f"{PIN_UNRULED} size (pin-row check: hand-name-not-in-evidence, the config copy's State)")


POSITIVE_GLOBALS = ("lock", "check_env", "list", "cut", "build", "xbe_sha1", "*objects* extra", "objdiff report",
                    "bytes setup", "relink", "audit_public", "provenance", "annotations", "pin_rows", "lock-imports")
# 4.5 T16.2.c5 (G13): the positive run's globals besides one `target <unit>` per listed function unit


def _c_positive_keys(d):  # kw `direct` (4.5 T16.2.c5, G13): a positive-run glob with one global key dropped must fail
    units = sorted({e["unit"] for e in matched_set.load(FIX_LIST) if e["kind"] == "function"})
    full = {**dict.fromkeys(POSITIVE_GLOBALS, "pass"), **{f"target {u}": "pass" for u in units}}
    drop = {k: x for k, x in full.items() if k != "relink"}
    print(f"control positive-keys: _positive_ok(0, ...) on the full {len(full)} keys and with `relink` dropped")

    def expect(r, v, g, i):
        a, b = _positive_ok(0, full, FIX_LIST), _positive_ok(0, drop, FIX_LIST)
        print(f"control positive-keys: full -> {a}; relink dropped -> {b}")
        return a and not b, "positive run global keys (relink dropped)"
    return {"direct": True}, expect


def _c_last_tree(d):  # kw `direct` (4.5 T16.2.c5, G16): _write_last into scratch paths; a list edit shows in list_sha256
    lp = d / "matched_set.toml"
    shutil.copy2(FIX_LIST, lp)
    print(f"control last-tree: _write_last into {d.name}/a.json, list copy edited, then {d.name}/b.json")

    def tree(name):
        p = d / name
        _write_last(p, lp, None, [], {"audit_not_checked": []}, {})
        return json.loads(p.read_text(encoding="utf-8")).get("tree", {})

    def expect(r, v, g, i):
        a = tree("a.json")
        with open(lp, "a", encoding="utf-8", newline="\n") as f:
            f.write("# gate control last-tree\n")
        b = tree("b.json")
        want = {"head", "dirty", "list_sha256", "fp_sha256"}
        print(f"control last-tree: keys a {sorted(a)}; b {sorted(b)}")
        ok = (set(a) == want == set(b) and a["list_sha256"] == _sha(FIX_LIST, "sha256")
              and b["list_sha256"] == _sha(lp, "sha256") != a["list_sha256"]
              and a["fp_sha256"] == b["fp_sha256"] == _sha(REPO / verifier_lock.FP, "sha256")
              and isinstance(a["dirty"], int) and len(a["head"]) == 40)
        return ok, "last.json tree (head, dirty, list_sha256 moved by the list edit, fp_sha256)"
    return {"direct": True}, expect


def _c_cut_unpinned(d):  # 4.5 T16.2.c5 (G17): hashes.txt copy gains a line, cut.complete re-sealed -> the pin refuses
    t = _target_copy(d)
    h = t / "hashes.txt"
    new = h.read_bytes() + f"{'0' * 64}  gate_control_unpinned.obj\n".encode()
    h.write_bytes(new)
    (t / byte_compare.CUT_MARKER).write_bytes(
        f"cut-complete v1 hashes-sha256 {hashlib.sha256(new).hexdigest()} objects {len(new.splitlines())}\n".encode())
    if byte_compare.cut_check(t):
        raise RuntimeError(f"re-sealed cut does not pass: {byte_compare.cut_check(t)}")
    print("control cut-unpinned: hashes.txt copy, one line appended, cut.complete re-sealed (default pin)")
    return {"tdir": t}, lambda r, v, g, i: (
        r == 2 and g.get("cut") == "fail: cut: hashes.txt sha256 != config/delink_cut.sha256", "cut (unpinned)")


CONTROLS =(("objdiff", _c_objdiff), ("bytes-alone", _c_bytes), ("target", _c_target), ("cut-marker", _c_cut_marker),
            ("body_sha1", _c_body),
            ("xbe_sha1", _c_xbe), ("empty-list", _c_empty), ("missing-list", _c_missing), ("extra", _c_extra),
            ("marker", _c_marker), ("asm", _c_asm), ("audit", _c_audit), ("clean-build", _c_clean), ("d26", _c_d26),
            ("lock-edit", _c_lock_edit), ("lock-missing", _c_lock_missing), ("lock-fp-missing", _c_lock_fp_missing),
            ("lock-fp-empty", _c_lock_fp_empty), ("lock-unlisted", _c_lock_unlisted), ("lock-crlf", _c_lock_crlf),
            ("lock-pyc", _c_lock_pyc), ("lock-symlink", _c_lock_symlink), ("symtab-pin", _c_symtab_pin),
            ("lock-imports", _c_lock_imports), ("lock-shadow", _c_lock_shadow),
            ("relink", _c_relink), ("relink-entry", _c_relink_entry), ("relink-literal", _c_relink_literal),
            ("relink-fold", _c_relink_fold), ("relink-samename", _c_relink_samename), ("relink-weak", _c_relink_weak),
            ("prov-hedge", _c_prov_hedge), ("prov-proto-pdb", _c_prov_proto), ("prov-name-retail", _c_prov_name),
            ("prov-placeholder", _c_prov_place), ("prov-src", _c_prov_src), ("prov-fake-diff", _c_prov_fake),
            ("prov-locals", _c_prov_locals), ("prov-d66", _c_prov_d66), ("prov-accepted", _c_prov_accepted),
            ("prov-forceinline", _c_prov_forceinline),
            ("prov-pragma", _c_prov_pragma), ("prov-noinline", _c_prov_noinline),  # 4.5 T16.2.c2
            ("prov-include-body", _c_prov_include_body),
            ("pin-unsized", _c_pin_unsized), ("pin-site-corr", _c_pin_site), ("pin-alias-dup", _c_pin_alias_dup),
            ("data-impostor", _c_data_impostor), ("xbe-wrong", _c_xbe_wrong), ("objdiff-cli", _c_objdiff_cli),
            ("build-refuse", _c_build_refuse), ("target-missing", _c_target_missing),
            ("target-unhashed", _c_target_unhashed), ("bytes-refuse", _c_bytes_refuse),
            ("bytes-two-objs", _c_bytes_two_objs), ("toolchain-split", _c_toolchain_split),
            ("audit-env", _c_audit_env), ("audit-null-key", _c_audit_null_key), ("audit-tail", _c_audit_tail),
            ("audit-line-pin", _c_audit_line_pin), ("audit-literal-pin", _c_audit_literal_pin),
            ("audit-short-blob", _c_audit_short_blob), ("audit-gzip-disasm", _c_audit_gzip_disasm),
            ("objdiff-pin", _c_objdiff_pin), ("audit-marker-contradiction", _c_audit_marker_contradiction),
            ("audit-renamed-blob", _c_audit_renamed_blob), ("audit-shallow", _c_audit_shallow),  # 4.5 T16.2
            ("audit-paths-empty", _c_audit_paths_empty), ("audit-route-pin", _c_audit_route_pin),
            ("relink-bare-exit", _c_relink_bare_exit), ("relink-exit-zero", _c_relink_exit_zero),
            ("relink-not-built", _c_relink_not_built), ("relink-no-funcs", _c_relink_no_funcs),
            ("relink-error", _c_relink_error), ("relink-rc", _c_relink_rc),
            ("prov-locals-cache", _c_prov_locals_cache), ("prov-inputs-pin", _c_prov_inputs_pin),
            ("prov-fake-head", _c_prov_fake_head), ("prov-refused", _c_prov_refused), ("prov-error", _c_prov_error),
            ("prov-fake-reword", _c_prov_fake_reword), ("prov-fake-trailing", _c_prov_fake_trailing),
            ("prov-fake-comment-excuse", _c_prov_fake_comment_excuse), ("prov-name-ns", _c_prov_name_ns),
            ("prov-d66-pdb-volatile", _c_prov_d66_pdb_volatile), ("annotations-folded", _c_annotations_folded),
            ("annotations-malformed", _c_annotations_malformed), ("annotations-no-name", _c_annotations_no_name),
            ("annotations-no-source", _c_annotations_no_source), ("annotations-count", _c_annotations_count),
            ("annotations-order", _c_annotations_order), ("annotations-row", _c_annotations_row),
            *((f"pin-plant-{p[0]}", _pin_plant(*p)) for p in PIN_PLANTS),  # 4.2 T5: 30 pin-row plants (was 28; 4.5 T16.1 +near, +V6)
            ("pin-literal-list", _c_pin_literal_list), ("pin-unruled", _c_pin_unruled),
            ("toolchain-hash", _c_toolchain_hash), ("size-list", _c_size_list),  # 4.5 T16.2.c3 (G14)
            ("annotations-refused", _c_annotations_refused), ("annotations-error", _c_annotations_error),
            ("pin-refused", _c_pin_refused), ("pin-error", _c_pin_error), ("pin-fold-site", _c_pin_fold_site),
            ("pin-i10-static", _c_pin_i10_static), ("pin-vtable-prefix", _c_pin_vtable_prefix),  # 4.5 T16.2.c4
            ("pin-accepted-unused", _c_pin_accepted_unused), ("cfg-pin-row", _c_cfg_pin_row),
            ("positive-keys", _c_positive_keys), ("last-tree", _c_last_tree),  # 4.5 T16.2.c5
            ("cut-unpinned", _c_cut_unpinned))
CFG_CONTROLS = (_c_data_impostor, _c_xbe_wrong, _c_objdiff_cli,
                _c_toolchain_split, _c_prov_locals_cache, _c_prov_inputs_pin,  # 3.5 T4: make(d, config)
                _c_audit_null_key, _c_audit_tail, _c_prov_d66_pdb_volatile,
                _c_pin_literal_list, _c_pin_unruled, _c_objdiff_pin, _c_audit_marker_contradiction,  # 4.5 T16.1
                _c_toolchain_hash,  # 4.5 T16.2.c3
                _c_clean, _c_pin_i10_static, _c_pin_vtable_prefix, _c_pin_accepted_unused, _c_cfg_pin_row,  # T16.2.c4
                *(m for n, m in CONTROLS if n.startswith("pin-plant-")))
LINE_GLOBALS = {"extra": "*objects* extra", "audit": "audit_public", "empty-list": "list",
                "relink": "relink", "relink-entry": "relink",
                "relink-literal": "relink", "relink-fold": "relink", "audit-null-key": "audit_public", "audit-tail": "audit_public",
                "audit-line-pin": "audit_public", "audit-literal-pin": "audit_public",
                "annotations-row": "annotations", "pin-literal-list": "pin_rows"}  # control -> global on the count line


def controls(config=None):
    """-> (J as expected, K). Scratch under <.run/phase>/gate/controls/<name>/, removed after (also on exception).
    The relink and audit_public layers run only in their own controls (relink, relink-* 3.4 T6.1; and the positive
    run): 2.6 T11, call bound;
    the provenance layer only in the prov-* controls (3.2 T7); the annotations layer only in the annotations-*
    controls (4.1 T4)."""
    root = byte_compare._phase_run_dir() / "gate" / "controls"
    good = 0
    try:
        for name, make in CONTROLS:
            d = root / name
            print(f"control {name}:")
            undo = None
            try:
                if d.exists():
                    shutil.rmtree(d)
                d.mkdir(parents=True)
                kw, expect = make(d, config) if make in CFG_CONTROLS else make(d)
                undo = kw.pop("undo", None)  # 3.5 T4: a control's env/monkeypatch restore, run in finally
                args = {"src_root": FIX_SRC, "list_path": FIX_LIST, "bdir": d / "build", "config": config,
                        "run_relink": name.startswith("relink"), "run_audit": name == "audit",
                        "run_lint": name.startswith("prov-"), "lint_accepted": FIX_ACC,
                        "run_annotations": name.startswith("annotations-"),
                        "run_pins": name == "pin-literal-list", **kw}
                # 4.1 T4: kw `direct`: the control's expect does the whole check, no gate run; was: always _core
                rc, v, g, i = (1, {}, {}, {}) if kw.get("direct") else _core(args.pop("src_root"), args.pop("list_path"), **args)
                ok, at = expect(rc, v, g, i)
                ok = ok and rc != 0
                if name in LINE_GLOBALS:  # count line names the failed global (1.8 T1)
                    cl = i.get("count_line", "")
                    named = cl.split("; FAIL globals: ", 1)[1].split(", ") if "; FAIL globals: " in cl else []
                    lok = LINE_GLOBALS[name] in named
                    print(f"control {name}: count line {'names' if lok else 'DOES NOT name'} "
                          f"{LINE_GLOBALS[name]}: {cl!r}")
                    ok = ok and lok
                good += ok
                print(f"control {name}: failed as expected at {at}" if ok else
                      f"control {name}: DID NOT FAIL as expected at {at} (rc {rc})")
            except (Exception, SystemExit) as e:  # a control that cannot run did not fail as expected; 3.5 T4:
                # SystemExit too (a vv_inputs refusal is exit 2); was: `except Exception` (one aborted the self-test)
                print(f"control {name}: DID NOT FAIL as expected: control error {type(e).__name__}: {e}")
            finally:
                if undo:
                    undo()
                shutil.rmtree(d, ignore_errors=True)
    finally:
        shutil.rmtree(root, ignore_errors=True)
    print(f"controls: {good} failed as expected of {len(CONTROLS)}")
    return good, len(CONTROLS)


def main(argv):
    ap = argparse.ArgumentParser(prog="gate.py", description=__doc__.splitlines()[0],
                                 epilog="Exit: 0 all pass, N >= 1; 1 any failure / N = 0 / missing list; 2 refusal.")
    ap.add_argument("--self-test", action="store_true", help="run on the gate fixture (tests/gate/), not the matched set")
    ap.add_argument("--config", help="config path (default config/local_paths.json)")
    a = ap.parse_args(argv)
    if a.self_test:
        rc, _v, g, _i = run(FIX_SRC, FIX_LIST, self_test=True, config=a.config)
        ok = _positive_ok(rc, g, FIX_LIST)  # 4.5 T16.2.c5; was: rc == 0 only
        j, k = controls(a.config)
        return 0 if ok and j == k else (rc or 1)
    return run(SRC, LIST, config=a.config)[0]


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
