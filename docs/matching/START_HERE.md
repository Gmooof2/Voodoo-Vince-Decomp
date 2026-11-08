# Start here: matching one function (1.6 T4; Halo 2 START_HERE shape)

PY = the python on the card. Forms (annotations, tags, asserts, `!FAKE`, NON_MATCHING, method): `CONVENTIONS.md`.
Matched means the gate (D40, D26); nothing else is a claim.

## 1. Setup check (every session, before any edit)
- `PY tools/manifest.py verify` exits 0 (inputs and the retail XBE pin).
- `PY tools/check_env.py` exits 0 (env, MSVC 7.1 sha256 pins).
- Baseline: `PY tools/gate.py > .run/<phase>/baseline.txt 2>&1; echo "rc=$?" >> .run/<phase>/baseline.txt`
  (long: through `bash tools/run.sh` with a raised timeout). Keep it until the finish; an empty matched set gives rc 1
  with the N=0 refusal, which is the baseline then.

## 2. Claim scope (one function)
- One function: a passing row of `docs/matching/next_candidates.md`, any lane (identical, near, or ambiguous: a row pinned at one of its hits by a ruled class, 5.3 T3) (zero unresolved relocs, zero same-section rel32
  sites, inside the Ghidra body), or the plan's named fallback.
- Its unit file: `src/` path by the rule in `src/README.md` (units.toml `objname`); the unit's `opt` set in
  `config/retail/units.toml` from evidence, old value kept in a comment (`opt = "/O1 /Oy" # was "unknown"`).
- Its matched_set row: `PY tools/matched_set.py add --list config/matched_set.toml --symbol <symbols.txt name>`,
  never typed by hand.
- Nothing else is claimed. Any other file you must touch (a header, `symbols_pin.txt`, another unit's `opt`) is listed
  at the finish.

## 3. Work loop
- Iterate with `PY tools/match_one.py --symbol <name> --task T<n>` (builds the unit alone; objdiff + bytes; not a
  claim). `--task` is required; every run, refusals too, appends one line to the attempt ledger
  `docs/matching/attempts-<phase>.jsonl` (4.4 T11: `attempts.jsonl` is the frozen base, read first; `--ledger P` for
  scratch runs; test runs never write the committed ledger).
- A resubmission refuses (rc 2, 5.2 T15): an attempt or control whose unit source sha256 and opt equal an earlier
  rc 0/1 row of the same symbol and kind names that row; change the unit's .cpp first (a header-only edit does not
  change the hash: touch the .cpp's comment). `--listing` runs are exempt.
- `--listing` also writes the unit's `/FAs` listing into the kept workdir and prints `listing: <path>`.
- `--control "<flags>"` builds the unit with its `opt` replaced (also for `opt = "unknown"` units); it prints
  `control run: no claim` and is the one-flag-flipped control a newly set `opt` needs.
- `--workdir <name>` builds in `.run/<phase>/match_one.<name>` and wipes only that. Whenever runs may overlap (coders
  in parallel), each coder passes its own name, e.g. `--workdir T6.c1`: in the one default folder overlapping runs
  wipe each other and report wrong failures (C0152). It separates the builds only: `--src` and its headers stay shared.
- `PY tools/match_one.py --table` regenerates `docs/matching/attempts_by_size.md` (attempts and match rate by
  instruction-count bucket) from the ledger; commit it with the ledger.
- Before keeping anything: `PY tools/gate.py`, exit 0 with `N passed, 0 failed of N`.
- Stop after about 20 attempts without progress; leave the best under `#ifdef NON_MATCHING` with its best line, and
  report it (`blocked` with evidence, never a weaker check).
- Never keep a change that breaks a matched function: the gate's other rows stay `passed`.
- Judge in the destination unit (CK25): the draft lives in its unit file with that unit's includes and declarations;
  a standalone compile never decides a unit claim.
- No invented structure (D78): every field, type, callee, guard and constant comes from the prototype PDB, the retail
  disassembly or retail strings; an unknown stays `field_<offset>`, `function_<VA>`, `undefined4`.
- D80 is inactive: retail /GL is no (docs/evidence/toolchain.md:169-172). If /GL turns `yes` or `unknown`, never swap
  arguments or change a signature at a call into an unmatched callee; match the callee first.
- Every match-only construct gets `// !FAKE:`; the D116 forms (was D79) never appear, with or without the marker (CONVENTIONS §4).
- Keep the attempt journal in the task log: edit, `match_one` lines, undo to the last best (CK18).

## 4. Finish
- One command runs the finish steps below in order (matched_set add, gate with N = the matched_set rows, progress,
  commit, candidates `--write`/`--check`, commit of the list), stopping at the first failed step:
  `bash tools/match_finish.sh T<n>.c<k> <symbols.txt name> <files of the match...>` (`--help`; 3.1 T2). Do not run
  §2's `matched_set.py add` by hand before it.
- Batch form, the expert runs it: `PY tools/match_finish_batch.py T<n> (--symbol S ... | --symbols-from FILE) --paths P ...`
  (`--help`; 4.1 T7). Rows the full gate fails leave, guarded per CONVENTIONS §5, and the gate re-runs. Coders never run
  the gate: they stop at `match_one` and name their rows and files.
- Coders per batch, measured so far: 2 clean at 4.3 T4 (~50 matches/wall h, 12-50 B rows) and 4 clean at 4.3 T5
  (~66 matches/wall h, 52-128 B rows; 0 collisions, 0 wrong failures each). Rule for T8: 6 coders only if T5's
  matches per wall hour beat T4's by a quarter or more (49.7 x 1.25 = 62.1) with no collision or wrong failure, else
  the best count measured; later batch tasks use the best count measured, or fewer when the slice holds fewer units.
  Measured at 4.3 T8: 6 clean (~48 matches, ~8.6 kB per wall h, 129-256 B rows; 0 collisions, 0 wrong failures). Final count for the rest of 4.3: 6 coders, fewer when the slice holds fewer units.
  Rows: `docs/ops/validation.md` "4.3 Coders per batch". Finish in the foreground only (C0201).
- A row that fails bytes, or a kept guard: `PY tools/va_dis.py obj <build>/<unit>.obj <symbol> [<retailVA>]` prints
  our function beside retail's bytes, relocation targets checked by name, first differing line marked `>>` (`--help`
  for a guarded build; 4.4 T9). Prints only (D46). For the retail body with its delinked reloc names: C0063.
- Full `PY tools/gate.py` again; compare with `baseline.txt`: the new row passes, every baseline row still passes, rc 0.
- List every change outside the claim (file, line, why) in the task summary.
- Commit through `bash tools/commit_task.sh`; `PY tools/progress.py` refreshes `docs/progress.md` from the gate's
  `derived/gate/last.json`.
- After the match commit: `PY tools/candidates.py --write`, then `PY tools/candidates.py --check` exits 0 (a stale
  `next_candidates.md` is a failed step, GENERATION_PLAN.md:35); commit the refreshed list.
- After any pool growth (new delinked units, pins, cut changes): `PY tools/name_sites.py --apply`, then `--doc`, then
  `--check-cut` (exit 0, FAIL 0); in that order, or check-cut fails on unpinned new sites (3.2 T10).
- Standing checks and known limits (objdiff 100.0 without the target's relocation, I6; the phase-close
  `provenance_lint.py --diff <phase-start commit>` backstop, I7): `docs/ops/validation.md` "3.3 Known limits and
  standing checks".
