"""Gate (1.5 T4): the one gate entry. Every layer, in order, never stopping at a failure (D51); green is the rc (D26).

CLI: gate.py [--self-test | --warm-audit] [--config P]
  no args      src/ + config/matched_set.toml (the project's matched set); writes derived/gate/last.json; the audit
               writes its history cache (.run/<phase>/audit/history_cache.json) and never reads it (cold, 5.4 T5.c5).
  --warm-audit as no args, the audit also reading that cache (audit_public --warm); refused with --self-test.
  --self-test  tests/gate/src + tests/gate/matched_set.toml (the gate fixture, NOT the matched set); banner first;
               never writes last.json; the positive run passes on rc 0 AND its global key set == POSITIVE_GLOBALS + `target <unit>`
               per listed function unit (4.5 T16.2.c5; was: rc 0 only). Then 159 controls (5.3 T2.c4: +pin-slotnames-
               unnamed, -site-corr, -count (a `??_7` row retagged `+slot-names` passes, then a slot stripped of names,
               named only by a site-corr row, retail one slot longer -> `route-slotnames:<why>`), +pin-bracket-outside,
               -ambiguous (a func row retagged `+unit-bracket+callee-bytes` passes, then its lower anchor moved onto it,
               a copy of its body inside the bracket -> `route-bracket:<why>`); was 154 after 5.3 T2: +prov-nested-op (a
               nested operator keeps its outer classes; right ones pass, a wrong outer class fails proto-pdb exactly),
               +relink-site-past-buffer (a reloc field past its cut section's raw -> REFUSED naming it, rc 2); was 152
               after 5.1 T14: +relink-label-unref, +relink-unit-key; was 150 after 5.1 T13; 146 after 5.1 T3.1; 145 before 5.1 T3.1:
               pin-near-listed (direct: a State copy's by_pd and by_pd_rel copies (5.5 T28.c2; was: by_pd only) drop a
               passing near row's agreeing sites -> a fourth unlisted no-site near row fails
               `route-near:no-agreeing-site`); 144 before 5.1 T3:
               pin-accepted-other-conflict (direct: a by_pd copy gives an accepted row's proto one more retail dest
               -> the waiver's recorded conflict differs, `site-conflict-*` at the row); 141 before 5.1 T2:
               gate-lock (direct: a child holds the run lock -> main rc 2, run not called; killed -> rc 0; a
               --self-test is not refused by its own lock), ms-extra-emitter (an [[emitter]] row naming an unmarked
               hand-written body fails `*objects* extra`), ms-parallel (an __asm in SetState fails its row's asm only;
               matched_set.WORKERS > 1); 138 before 5.1 T1:
               objdiff-noexec (config copy, objdiff_cli a text file zz.exe -> `objdiff report` `fail: objdiff-cli
               OSError`, rc 1, xbe_sha1 + bytes still run), prov-include-dotdot (a quoted #include escaping the src
               root, one to a missing file under `#if 0`, one to an existing file -> FAIL include at both),
               prov-pragma-if0 (an in-body `#pragma` inside `#if 0` .. `#endif` -> FAIL pragma: every `#if` arm is
               scanned); 135 before 4.5 T16.2.c5: positive-keys (direct: _positive_ok on a key set with `relink` dropped), last-tree (direct:
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
               The controls live in tools/gate_controls.py (5.4 T5), imported by main on --self-test only.
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
objdiff bytes), `PASS|FAIL <global>: <verdict>` per global check, `gate: audit classes not checked: <list or none>` and (5.5 T12.c2,
I73 F2) `gate: audit history cache: cold|warm, <H of N (key S)>` (the audit child's stats line; `not used (no stats
line)` without --cache; masked in gate_golden) when the audit ran, `gate: layer seconds: <layer> <s>, ...; total <s>`
(lock check_env list cut build target xbe matched_set
objdiff bytes relink audit provenance annotations pins lock-imports report; a layer not run is absent; an early return prints neither;
4.1 T3.1), `gate: tree head <sha12> dirty <n> list <sha12> fp <sha12> listed_dirty <n|none>` then one `gate: listed
dirty <path>` per such path (run() only: the real gate and the self-test's positive run, not the controls; 4.5
T16.2.c5 G16; listed_dirty 5.5 T12.c2 G1), then `N passed, F failed of N` (functions).
A function passes iff matched_set.is_done over ALL its verdicts. Build dir removed on pass, kept (path printed) on fail.
last.json: {date, xbe_sha1, rows: [{symbol, unit, va, size, body_sha1, verdicts, passed}], audit_not_checked: [class],
layer_seconds: {layer: s}, tree: {head, dirty (`git status --porcelain` line count), list_sha256 (the list file),
fp_sha256 (config/verifier.sha256), listed_dirty ([LISTED + FP paths differing from HEAD, untracked included])},
warm_audit (bool: the audit read its history cache), config (the --config path or null)} (hashes only, D46; tree 4.5
T16.2.c5, None where git or a file fails; listed_dirty, warm_audit, config 5.5 T12.c2: progress.py and
match_finish_batch refuse a non-empty or absent listed_dirty). A write_last run (the real gate) deletes last.json
first (5.5 T12.c2 G11): a refusal, lock fail or crash leaves none, never the previous green one.
G5 (5.5 T12.c2, I71): with the provenance lane, provenance_lint.inputs(config) and pin_rows.state(config) are built on
the main thread before any lane starts (pdb_types.recs is module-global), no child process.
Lanes consumed on every return after they start (5.5 T12.c3, I73 F1, G17): a build Refuse, or any Exception / SystemExit
from the build to the bytes layer (then global `gate error`: `fail: gate error <Type>: <e>`, `gate: ERROR ...` printed,
rc 1, 2 for a SystemExit), runs the audit, provenance and annotations handlers, prints every global so far and returns
(no count line); was: the lanes joined and their verdicts dropped, or abandoned by the exception.
Exit: 0 every function and global check pass and N >= 1; 1 any failure, N = 0, or missing list (reason printed);
2 refusal (malformed list, cut.complete absent/malformed/stale, cut pin mismatch, build Refuse, unexpected byte_compare setup refusal,
relink refusal, 5.1 T2: another gate holds the run lock RUN_LOCK (`gate: REFUSED run lock: another gate is running
(<path>)`, before any layer; main only, so run()/_core never take it)).
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
import hashlib
import io
import json
import msvcrt
import os
import re
import shutil
import subprocess
import sys
import tempfile
import threading
import time
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
import timing  # noqa: E402  5.4 T5.c4: main's timing record (LISTED)
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
RUN_LOCK = byte_compare._phase_run_dir() / "gate" / "run.lock"  # 5.1 T2 (O21): one gate at a time, held by main
RUN_LOCK_AT = 1 << 40  # the locked byte, past any content (as match_one.LEDGER_LOCK_AT)
AUDIT_CACHE = byte_compare._phase_run_dir() / "audit" / "history_cache.json"  # 5.4 T5.c5: the audit's history cache
_UNTIMED = []  # 5.4 T5.c4: non-empty in a control worker or inside a timed main: main writes no timing record


def _tool(name, args=(), config=None, tools=TOOLS, stats=None):
    """-> verdict of `PY tools/<name>.py` by its rc (D26). The gate's config reaches the child: check_env as
    `--config`, audit_public as AUDIT_PUBLIC_CONFIG (3.5 T4 F4/N5; was: config ignored, the child used the default).
    Inherited AUDIT_PUBLIC_* are dropped (3.5 T4 W6; was: the gate's env steered the audit's manifest/config).
    tools: the dir the script runs from. audit_public's per-class `not checked` lines leave the tail, counted (4.1 T3.1;
    was: printed last, they crowded a HIT out of the verdict). stats (5.5 T12.c2, I73 F2): a dict that gets "line", the
    audit child's `audit: history cache <H of N (key S)>` text (None when it printed none), whatever the rc; was:
    dropped on pass."""
    cmd = [sys.executable, str(Path(tools) / f"{name}.py"), *args]
    env = {k: v for k, v in os.environ.items() if not k.startswith("AUDIT_PUBLIC_")}
    env["PYTHONPYCACHEPREFIX"] = sys.pycache_prefix  # 3.5 T4 F1; was: inherited env (no prefix)
    if config and name == "check_env":
        cmd += ["--config", str(config)]
    elif config and name == "audit_public":
        env["AUDIT_PUBLIC_CONFIG"] = str(config)
    p = subprocess.run(cmd, stdin=subprocess.DEVNULL, capture_output=True, text=True, errors="replace", env=env)
    if stats is not None:
        st = [ln for ln in p.stderr.splitlines() if ln.startswith("audit: history cache ")]
        stats["line"] = st[-1].removeprefix("audit: history cache ").strip() if st else None
    if p.returncode == 0:
        return "pass"
    lines = (p.stdout + p.stderr).strip().splitlines()
    if name == "audit_public":  # 5.4 T5.c5: the cache stats line never crowds a HIT out of the tail
        lines = [ln for ln in lines if not ln.startswith("audit: history cache ")]
    kept = lines
    if name == "audit_public":
        ends = tuple(f"{c}: not checked ({k} not set in local config)" for c, k in audit_public.VALUE_KEYS.items())
        kept = [ln for ln in lines if ln.startswith("HIT ") or not ln.endswith(ends)]
    dropped = len(lines) - len(kept)
    tail = " | ".join(kept[-3:] + ([f"{dropped} not checked lines dropped"] if dropped else []))
    return f"fail: rc {p.returncode}: {tail}"


class _LaneDead(Exception):
    """5.4 T5.c3 (D26): a dead lane (a BaseException escaped it, or it set no result); str = the layer's glob verdict."""


class _Lane:
    """5.4 T5.c3: one independent layer's computation on a thread, started early, joined at the layer's canonical
    position; the main thread judges and prints as before. A lane only computes: it never prints (bytes'
    redirect_stdout and relink.gate_check's are process-global) and never writes os.environ. secs: its own run seconds."""

    def __init__(self, fn, *a, **kw):
        self.out = self.exc = self.dead = None
        self.secs = 0.0
        self.th = threading.Thread(target=self._run, args=(fn, a, kw))
        self.th.start()

    def _run(self, fn, a, kw):
        t0 = time.perf_counter()
        try:
            self.out = (fn(*a, **kw),)
        except Exception as e:  # the callable's own: re-raised by get into the layer's handler, as before the lanes
            self.exc = e
        except BaseException as e:  # SystemExit (a vv_inputs refusal) and the like: a dead lane
            self.dead = e
        finally:
            self.secs = round(time.perf_counter() - t0, 2)

    def get(self, key, caught=False):
        """-> the callable's value; raises the Exception it raised, or _LaneDead for a dead lane (caught: that
        Exception too, for a layer with no handler of its own)."""
        self.th.join()
        e = self.dead or (self.exc if caught else None)
        if e is not None:
            raise _LaneDead(f"fail: {key} lane error {type(e).__name__}: {e}")
        if self.exc is not None:
            raise self.exc
        if self.out is None:
            raise _LaneDead(f"fail: {key} lane error: no result")
        return self.out[0]


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
    try:  # 5.1 T1: a non-executable objdiff_cli is a failed layer, not a gate crash; was: run unguarded (OSError up)
        p = subprocess.run(cmd, stdin=subprocess.DEVNULL, capture_output=True, text=True, errors="replace")
    except OSError as e:
        return {}, f"fail: objdiff-cli OSError: {e}"
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


def run(src_root, list_path, self_test=False, config=None, warm_audit=False):
    if self_test:
        print(f"gate: SELF-TEST on the fixture {Path(src_root)} + {Path(list_path)}; NOT the matched set; "
              f"last.json not written")
    # 5.4 T5.c5: the real run writes the audit's history cache; it reads it only on warm_audit; was: no audit args
    audit_args = () if self_test else ("--cache", str(AUDIT_CACHE), *(("--warm",) if warm_audit else ()))
    return _core(src_root, list_path, config=config, write_last=not self_test, audit_args=audit_args,
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
    and adds no global (the history audit and the relink are the costliest layers, measured in docs/ops/validation.md).
    run_lint False likewise (3.2 T7:
    the provenance layer runs in the positive run and the prov-* controls); run_annotations False likewise (4.1 T4:
    the positive run and the annotations-* controls); run_pins False likewise (4.2 T5: the positive run and the
    pin-literal-list control)."""
    refused, glob, verd, failed_units = False, {}, {}, {}
    bdir = Path(bdir) if bdir else byte_compare._phase_run_dir() / "gate" / "build"
    secs = {}
    info = {"bdir": bdir, "failures": failed_units, "audit_not_checked": [], "layer_seconds": secs,
            "warm_audit": "--warm" in audit_args, "config": str(config) if config else None}  # 5.5 T12.c2 (I73 F2)
    if write_last:  # 5.5 T12.c2 (G11): no early return (lock, list, cut, build refusal) or crash leaves the previous
        LAST.unlink(missing_ok=True)  # green last.json behind; was: kept until the writer at the end
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
    # 5.4 T5.c3: the independent layers start here as lanes, each joined at its canonical position below; was: each
    # run there. relink stays on this thread: relink.gate_check's own redirect_stdout/stderr (process-global) would
    # interleave with bytes' redirect below and leave sys.stdout pointing at its buffer
    lanes, astats = {}, {}
    if run_lint:  # 5.5 T12.c2 (G5, I71): the PDB parses (pdb_types.recs, module-global) are built here, on this thread,
        # before any lane: the provenance lane and matched_set.check's State then only read them; was: the lane's
        # Pdb cleared and reloaded recs while this thread built State. A failure is left to the layer that needs it
        try:
            if config not in provenance_lint._INPUTS:
                provenance_lint._INPUTS[config] = provenance_lint.inputs(config)
            pin_rows.state(config)
        except (Exception, SystemExit):  # the provenance / matched_set / pin_rows layer reports it at its position
            pass  # (SystemExit: a vv_inputs refusal, a dead lane before the prebuild)
    if run_audit:
        lanes["audit"] = _Lane(_tool, "audit_public", audit_args, config, audit_tools or TOOLS, stats=astats)  # 3.5
        # T4 F4; was: no config. 5.5 T12.c2 (I73 F2): stats; was: none
    if run_lint:
        lanes["provenance"] = _Lane(provenance_lint.lint, src_root, list_path, lint_rev,
                                    lint_accepted or provenance_lint.ACCEPTED, lint_base, config, cache=False)
        # 3.5 T4 F4; was: no config (default config inputs). 3.5 T4 F5: cache=False, every locals obj compiled in
        # scratch; was: the derived cache
    if run_annotations:
        lanes["annotations"] = _Lane(annotations.check, src_root, list_path)

    def consume():  # 5.5 T12.c3 (I73 F1, G17): every started lane through its layer's handler (verdict, print,
        # seconds), once: at the layers' canonical position below, or by abort; was: inline at that position
        nonlocal refused, last
        if run_audit:
            skipped = [c for c, k in audit_public.VALUE_KEYS.items() if vv_inputs.load_config(config).get(k) is None]
            try:  # 5.4 T5.c3: the lane's _tool result; was: _tool called here. _tool raising has no handler: a dead lane
                v, dead = lanes["audit"].get("audit_public", caught=True), None
            except _LaneDead as e:
                v = dead = str(e)
            # 4.1 T3.1: from the config and the marker file, never the audit's text (D26); was: a null class passed anywhere
            marker = Path(marker_root or REPO) / "config" / audit_public.PUBLIC_MARKER
            if v == "pass" and skipped and not marker.is_file():
                v = (f"fail: not checked: {', '.join(skipped)} (null or absent in config, tree has no "
                     f"config/{audit_public.PUBLIC_MARKER})")
            # 4.5 T16.1 G6: a marker beside .claude/pa.json is a private tree (was: the marker alone excused null classes)
            if marker.is_file() and (Path(marker_root or REPO) / ".claude" / "pa.json").is_file():
                v = (f"fail: {audit_public.PUBLIC_MARKER} marker in a private tree (.claude/pa.json present)" +
                     ("" if v == "pass" else f"; {v}"))
            glob["audit_public"] = dead or v  # 5.4 T5.c3: a dead lane's verdict as is (D26); was: v
            info["audit_not_checked"] = skipped
            info["audit_cache"] = (f"{'warm' if info['warm_audit'] else 'cold'}, "  # 5.5 T12.c2 (I73 F2)
                                   f"{astats.get('line') or 'not used (no stats line)'}")
            secs["audit"], last = lanes["audit"].secs, time.perf_counter()  # 5.4 T5.c3: the lane's own seconds; was: mark
        else:  # no key, as for relink below
            print("gate: audit_public layer not run (self-test control; runs in the positive run and the audit control)")
        # provenance (3.2 T7): provenance_lint in-process; verdict from its failure list / Refuse (D26), never its text
        info["provenance"] = []
        if not run_lint:  # no key, as for relink below
            print("gate: provenance layer not run (self-test control; runs in the positive run and the prov-* controls)")
        else:
            try:
                pf = lanes["provenance"].get("provenance")[0]  # 5.4 T5.c3: the lane's provenance_lint.lint result (its
                # call and history at the lane start, after the cut); was: called here
                info["provenance"] = pf
                for ln in pf:
                    print(ln)
                pc = ", ".join(f"{k} {n}" for k, n in provenance_lint.counts(pf).items() if n)
                glob["provenance"] = "pass" if not pf else f"fail: {len(pf)} failure(s) ({pc})"
            except _LaneDead as e:  # 5.4 T5.c3 (D26)
                glob["provenance"] = str(e)
            except provenance_lint.Refuse as e:
                refused = True
                glob["provenance"] = f"fail: REFUSED {e}"
            except Exception as e:  # a layer that cannot run fails; the other layers still run (D51)
                glob["provenance"] = f"fail: provenance error {type(e).__name__}: {e}"
            secs["provenance"], last = lanes["provenance"].secs, time.perf_counter()  # 5.4 T5.c3; was: mark
        # annotations (4.1 T4): annotations.check in-process; verdict from its failure list / Refuse (D26), never its text
        info["annotations"], info["annotation_orders"] = [], []
        if not run_annotations:  # no key, as for relink below
            print("gate: annotations layer not run (self-test control; runs in the positive run and the annotations-* "
                  "controls)")
        else:
            try:
                af, _nrows, _nfiles, orders = lanes["annotations"].get("annotations")  # 5.4 T5.c3: the lane's
                # annotations.check(src_root, list_path) result; was: called here
                info["annotations"], info["annotation_orders"] = af, orders
                for ln in orders + af:
                    print(ln)
                print(f"gate: annotations order lines: {len(orders)}")  # advisory: never in the row's verdict
                glob["annotations"] = "pass" if not af else f"fail: {len(af)} failure(s)"
            except _LaneDead as e:  # 5.4 T5.c3 (D26)
                glob["annotations"] = str(e)
            except matched_set.Refuse as e:
                refused = True
                glob["annotations"] = f"fail: REFUSED {e}"
            except Exception as e:  # a layer that cannot run fails; the other layers still run (D51)
                glob["annotations"] = f"fail: annotations error {type(e).__name__}: {e}"
            secs["annotations"], last = lanes["annotations"].secs, time.perf_counter()  # 5.4 T5.c3; was: mark

    def abort(rc):  # 5.5 T12.c3 (I73 F1): an early return after the lanes started: consume them, print every
        consume()  # global so far -> _core's 4-tuple; was: lanes joined, results dropped, nothing printed
        for k, v in glob.items():
            print(f"{'PASS' if v == 'pass' else 'FAIL'} {k}: {v}")
        return rc, verd, glob, info
    try:  # 5.5 T12.c3 (G17): an exception from the build to the bytes layer fails the run and still consumes the
        # lanes; was: none (it left _core, the lanes' verdicts abandoned)
        # build
        if bdir.exists():
            shutil.rmtree(bdir)
        try:
            built = build.build(src_root, bdir, units_path or build.UNITS, config)
            glob["build"] = "pass"
        except build.Refuse as e:
            print(f"gate: REFUSED build: {e}")
            glob["build"] = f"fail: REFUSED {e}"  # 3.5 T4 N4; was: printed only, no `build` key
            return abort(2)  # 5.5 T12.c3 (I73 F1): the lanes consumed and printed; was: each lane joined, its
            # result discarded, nothing printed
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
        print("gate: matched_set inline-copy bytes (report, never a fail): "  # 5.5 T12.c4 (R5.5-003 S6)
              + ms.get(matched_set.OBJECTS, {}).get("inline-copy", "not reported"))
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
    except (Exception, SystemExit) as e:  # SystemExit: a vv_inputs refusal (rc 2, as main's exit was)
        print(f"gate: ERROR {type(e).__name__}: {e} (the layers after it not run)")
        glob["gate error"] = f"fail: gate error {type(e).__name__}: {e}"
        return abort(2 if isinstance(e, SystemExit) else 1)
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
    consume()  # 5.5 T12.c3: the audit, provenance and annotations layers (the lanes); was: their blocks here
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
        print(f"gate: audit history cache: {info['audit_cache']}")  # 5.5 T12.c2 (I73 F2); masked in gate_golden
    print("gate: layer seconds: " + ", ".join(f"{k} {s:.2f}" for k, s in secs.items())
          + f"; total {time.perf_counter() - start:.2f}")
    if tree or write_last:  # 4.5 T16.2.c5 (G16): the tree the verdict is of, before the count line
        info["tree"] = t = _tree(list_path)
        ld = t["listed_dirty"]
        print(f"gate: tree head {(t['head'] or 'none')[:12]} dirty {t['dirty']} list "
              f"{(t['list_sha256'] or 'none')[:12]} fp {(t['fp_sha256'] or 'none')[:12]} "
              f"listed_dirty {'none' if ld is None else len(ld)}")  # 5.5 T12.c2 (G1); was: no listed_dirty
        for p in ld or ():
            print(f"gate: listed dirty {p}")
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


def _tree(list_path, root=None):  # 4.5 T16.2.c5 (G16): -> {head, dirty, list_sha256, fp_sha256, listed_dirty}; None
    # where git / a file fails. 5.5 T12.c2 (G1 ruling 1a): listed_dirty = sorted LISTED + FP paths whose working copy
    # differs from HEAD (`git diff HEAD`, untracked included); root: the repo (default REPO; a control's scratch repo)
    root = Path(root) if root else REPO

    def git(*a):
        p = subprocess.run(["git", *a], cwd=root, stdin=subprocess.DEVNULL, capture_output=True, text=True,
                           errors="replace")
        return p.stdout if p.returncode == 0 else None  # D26: by rc
    head, st = git("rev-parse", "HEAD"), git("status", "--porcelain")
    paths = sorted({*verifier_lock.LISTED, verifier_lock.FP.as_posix()})
    dif = git("diff", "--no-ext-diff", "--name-only", "HEAD", "--", *paths)
    new = git("ls-files", "--others", "--exclude-standard", "--", *paths)
    fp = root / verifier_lock.FP
    return {"head": head.strip() if head else None, "dirty": None if st is None else len(st.splitlines()),
            "list_sha256": _sha(list_path, "sha256") if Path(list_path).is_file() else None,
            "fp_sha256": _sha(fp, "sha256") if fp.is_file() else None,
            "listed_dirty": None if dif is None or new is None else sorted(set((dif + new).split()))}


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
           "layer_seconds": secs, "tree": tree or _tree(list_path),  # G16; was: no tree
           "warm_audit": bool(info.get("warm_audit")), "config": info.get("config")}  # 5.5 T12.c2 (I73 F2); was: none
    path.write_text(json.dumps(doc, indent=1) + "\n", encoding="utf-8", newline="\n")


POSITIVE_GLOBALS = ("lock", "check_env", "list", "cut", "build", "xbe_sha1", "*objects* extra", "objdiff report",
                    "bytes setup", "relink", "audit_public", "provenance", "annotations", "pin_rows", "lock-imports")
# 4.5 T16.2.c5 (G13): the positive run's globals besides one `target <unit>` per listed function unit


def _positive_ok(rc, glob, list_path):  # 4.5 T16.2.c5: the self-test's positive-run verdict, factored out of main
    """rc 0 and the global key set == POSITIVE_GLOBALS + `target <unit>` per listed function unit (G13); was: rc == 0
    (a layer that added no key passed unseen)."""
    units = {e["unit"] for e in matched_set.load(list_path) if e["kind"] == "function"}
    want = set(POSITIVE_GLOBALS) | {f"target {u}" for u in units}
    missing, extra = sorted(want - set(glob)), sorted(set(glob) - want)
    if missing or extra:
        print(f"gate: FAIL positive run global keys: missing {missing}; unexpected {extra}")
    return rc == 0 and not missing and not extra


def main(argv):
    ap = argparse.ArgumentParser(prog="gate.py", description=__doc__.splitlines()[0],
                                 epilog="Exit: 0 all pass, N >= 1; 1 any failure / N = 0 / missing list; 2 refusal.")
    ap.add_argument("--self-test", action="store_true", help="run on the gate fixture (tests/gate/), not the matched set")
    ap.add_argument("--config", help="config path (default config/local_paths.json)")
    ap.add_argument("--ctl-worker", help=argparse.SUPPRESS)  # 5.4 T5.c2: internal, a self-test worker's spec json
    ap.add_argument("--warm-audit", action="store_true",
                    help="real run only: the audit reads its history cache (5.4 T5.c5); default cold")
    a = ap.parse_args(argv)
    if a.warm_audit and (a.self_test or a.ctl_worker):
        print("gate: REFUSED --warm-audit: real run only (the self-test stays cold)")
        return 2
    if a.ctl_worker:  # 5.4 T5.c2: a self-test control worker: no run lock (its parent holds it), no positive run
        _UNTIMED.append("worker")  # 5.4 T5.c4: a control's main() call in a worker records nothing
        import gate_controls
        return gate_controls.worker(sys.modules[__name__], a.config, a.ctl_worker)
    # 5.1 T2 (O21): one gate at a time: the OS byte lock on RUN_LOCK, one try, held until main returns (the OS drops it
    # at process death, so never stale; C0178); was: no lock (two gates shared .run/<phase>/gate/ scratch)
    RUN_LOCK.parent.mkdir(parents=True, exist_ok=True)
    RUN_LOCK.open("a").close()
    with RUN_LOCK.open("r+b") as fh:
        fh.seek(RUN_LOCK_AT)
        try:
            msvcrt.locking(fh.fileno(), msvcrt.LK_NBLCK, 1)
        except OSError:
            print(f"gate: REFUSED run lock: another gate is running ({RUN_LOCK})")
            return 2
        try:
            # 5.4 T5.c4: one timing record per run (step `selftest` / `gate`, label = the phase id, rc = main's); none
            # in a control worker or nested in a timed main (gate-lock, selftest-parallel-pass call main); stdout as is.
            # 5.4 T20: the timed run also writes the shared gate marker (docs/ops/gate_marker.md); workers and nested
            # main() write none
            st = (contextlib.nullcontext(argparse.Namespace(rc=0)) if _UNTIMED else
                  timing.step("selftest" if a.self_test else "gate", label=build.phase_id(),
                              marker="gate.py " + " ".join(argv)))
            _UNTIMED.append("main")
            with contextlib.ExitStack() as es:
                es.callback(_UNTIMED.pop)
                rec = es.enter_context(st)
                if a.self_test:
                    import gate_controls  # 5.4 T5: the controls, on --self-test only; was: in this file
                    pos = {}

                    def positive():  # 5.4 T5.c2: called by controls() while its workers run; was: run before controls()
                        rc, _v, g, _i = run(FIX_SRC, FIX_LIST, self_test=True, config=a.config)
                        pos["rc"], pos["ok"] = rc, _positive_ok(rc, g, FIX_LIST)  # 4.5 T16.2.c5; was: rc == 0 only
                    j, k = gate_controls.controls(sys.modules[__name__], a.config, positive)  # the running module, never a 2nd copy
                    rec.rc = 0 if pos.get("ok") and j == k else (pos.get("rc") or 1)
                else:
                    rec.rc = run(SRC, LIST, config=a.config, warm_audit=a.warm_audit)[0]
            return rec.rc
        finally:
            fh.seek(RUN_LOCK_AT)
            msvcrt.locking(fh.fileno(), msvcrt.LK_UNLCK, 1)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
