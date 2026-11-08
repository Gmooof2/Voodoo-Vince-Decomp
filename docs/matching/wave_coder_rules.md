# Wave coder rules (template; 5.3 T16 from T6-T8's coder_rules.md)

The expert copies this to `.run/<phase>/t<n>/coder_rules.md`, replaces `T<n>`, adds the wave's own lines at the end,
and every `T<n>.c<k>` reads it whole. Rows files come from `PY tools/wave_rows.py ... --split ... --out .run/<phase>/t<n>`.

Rows: your `.run/<phase>/t<n>/c<k>.rows` (lane, VA, size, prior attempts, best objdiff, symbol). Whole units, all yours.
Loop per row: docs/matching/START_HERE.md §2-§3. Unit file path: src/README.md. Forms: docs/matching/CONVENTIONS.md (§4, §5).
- `PY tools/match_one.py --symbol <S> --task T<n> --workdir T<n>.c<k>` (`--task T<n>` exactly; `T<n>.c<k>` is refused and writes a null ledger row, C0376).
- Byte diffs: `PY tools/va_dis.py obj <build>/<unit>.obj <S> [<VA>]`; retail vtable slots: `PY tools/va_dis.py vt <VA> <n>`; probe a form before spending an attempt: `tools/probe_form.py` (C0345).
- Screen before drafting: pin status and size in config/retail/symbols_pin.txt (unsized pin: precheck ADD refuses), denominator gap (C0378), callees that are cut or fail rows, a near-corr pin on an ambiguous row (C0378). Unfinishable row: verdict + lift, no drafting spend.
- `??_G` rows: C0377 and C0320 first (ctor companion for the vtable, `pin_emit_<Class>_dtor` emitter, walk retail vtable slots, fold-name wall), C0376 (a ??_G our build already emits needs only its `// FUNCTION:` block). A companion may be claimed only if it is a passing row in next_candidates.md (any lane); name it as a companion.
- Ctors: copy the unit's matched sibling ctors' form; the vtable slot list must equal retail (C0377, C0379). Screen each slot VA against the unit's matched rows: a slot naming a fold alias inside another matched row's replaced range is refused at relink (`lies in a replaced range`) though match_one is 100; record `relink-refused` with the slot, do not iterate (C0381).
- Also read: C0322 (finish refusals precheck misses), C0145, C0137 (opt by one-flag control), C0380, C0381.
- Barred: D78 (no invented structure), D116 forms, D117; free operators/helpers absent from default.pdb stay under NON_MATCHING (C0365). A row that matches only with a barred lever is a wall: record the lever, guard it (R195).
- M17: stop a row at 20 attempts total in the ledger (prior + yours); list it as `past-stop`, never retry it before its thinker round (R172).
- opt: keep the unit's units.toml opt; change it only from a control (C0137), old value in a comment; `unknown` is set by a one-flag control first.
- A row not matched: best draft under `#ifdef NON_MATCHING` with its best line (CONVENTIONS §5), and a verdict: wall kind + what lifts it.
- Annotations: every matched row, compiler-generated ones too (??_G, implicit ??1/??4), needs its own `// FUNCTION: USA 0x<VA>` + `// NAME: proto-pdb:default.pdb` block; for a compiler-generated row directly above the class/struct declaration, never on the pin_emit_ scaffold (C0378).
Never: run gate.py, commit (no commit_task.sh, no git add), edit config/matched_set.toml, tools/pool_buckets.py, docs/ops/validation.md, symbols*.txt, or a unit outside your rows file. Never touch another coder's unit files. Scratch files get per-coder names (C0380).
Before reporting: list matched symbols one per line in `.run/<phase>/t<n>/c<k>.syms`; run `PY tools/precheck.py .run/<phase>/t<n>/c<k>.syms T<n>.c<k> <your changed repo-relative files>`; exit 0 needed (if it fails on a row, fix or guard that row and rerun).
Log: phase-ends/current/logs/T<n>.c<k>.md (attempt journal, per row: attempts, best, verdict).
Report (first word done|blocked|question): matched symbols (+ companions); files changed; `[[emitter]]` rows needed as exact TOML blocks (symbol, unit, reason); guarded rows `VA symbol verdict lift`; past-stop rows; opt changes; precheck rc; attempts total; wall minutes; any trap a later coder needs.
