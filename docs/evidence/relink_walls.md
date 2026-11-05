# Relink walls and unknown01 (phase 4.3 T2, 2026-11-02)

Names, VAs, sizes and counts only (D46). Rows: the 10 walled rows of 3.4 T10 (`phase-ends/phase-3.4/tasks/T10.md`
Findings) = today's `relink-refused` (2) + `pin-only-alias` (8) rows (`tools/pool_buckets.py` `NOTES_VA` :53-58,
`docs/matching/pool_buckets.md` :266-267). Rank, size, lane from `derived/candidates/candidates.jsonl`.
Prototypes ran in scratch clones of 03950fb7 (C0177): `.run/4.3/t2/c1/repo` (fold VA), `.run/4.3/t2/c2/repo` (cut name);
coder logs `phase-ends/current/logs/T2.c1.md`, `T2.c2.md`. Real tree: only this file changed.

## Rows
| VA | rank | size | retail unit | bucket | 3.4 T10 wall | what refuses it today |
|---|---|---|---|---|---|---|
| 0x000CA770 ?ClearStateCache@SwampRenderer | 984 | 75 | SwampRenderer.obj | relink-refused | fold VA | `tools/relink.py:1034` `our ranges overlap at 0x0001819A` |
| 0x000E88E1 ??1HealthUI | 1004 | 78 | HUDUI.obj | relink-refused | fold VA | `tools/relink.py:1034` (via ~ResourceHandle at 0x000292A6) |
| 0x000292A6 ??1ResourceHandle<VertexShaderResource> | 20 | 10 | Hannah.obj (+Onion) | pin-only-alias | fold VA | `tools/relink.py:1034` `our ranges overlap at 0x000292A6` |
| 0x0001819A ??4ResourceHandle<VertexShaderResource> | 264 | 29 | Onion.obj (+4) | pin-only-alias | cut name | cut name: objdiff None; listed: `tools/relink.py:1034` |
| 0x0006A5D2 ?FrameTick@LevelFlythroughCamMain | 71 | 16 | Lantern.obj (+3) | pin-only-alias | cut name | cut name: objdiff None (no relink refusal) |
| 0x000775B9 ?size vector<TrolleyManager::MapStop> | 105 | 19 | Blender.obj (+SignTrolley) | pin-only-alias | cut name | cut name: objdiff None (no relink refusal) |
| 0x000775E2 ?capacity vector<TrolleyManager::MapStop> | 106 | 19 | Blender.obj (+SignTrolley) | pin-only-alias | cut name | cut name: objdiff None (no relink refusal) |
| 0x000BE119 ?OutputContents ResourceManager<TextureResource> | 335 | 33 | SwampResourceManager.obj | pin-only-alias | no source | none: no source unit in `src/` |
| 0x000C1028 ??0 ResourceManager<VertexShaderResource> | 649 | 43 | SwampResourceManager.obj | pin-only-alias | no source | none: no source unit in `src/` |
| 0x000C0299 ?Add ResourceManager<VertexShaderResource> | 816 | 58 | SwampResourceManager.obj | pin-only-alias | no source | none: no source unit in `src/` |

- The fold refusal (the one R4.3-001 did not find): `tools/relink.py:1032-1034`, the overlap loop after the dedupe pass
  :1023-1031. `same()` :1007-1022 lets a second copy ride the first's hole only with the same name, in a different
  object, with equal section, bytes, size, Selection and reloc target names. Two template args folded to one VA by
  retail LINK (/OPT:ICF) have different names, so the second copy falls through to :1034. `:1383` is the PE-entry
  refusal (C0136), not this one.
- Cut-name rows: relink places by VA and does not refuse them; objdiff pairs by the cut symbol name (C0154), so
  `match_one` reports objdiff None (bytes equal, rc 1) when our unit cannot emit the cut name.
- The cut name at a fold VA is the LAST `config/retail/symbols_pin.txt` line at that VA: `tools/ghidra_scripts/ApplySymbols.java:150-162`
  calls `setName` once per line (llvm-nm confirmed at all 5 VAs). `delink._func_rows` (`tools/delink.py:450-462`, first
  seen) feeds only `_add_rel32` (va, size) and does not name anything.

## Wall 1: fold VA, several template copies at one folded address
Smallest change (scratch `.run/4.3/t2/c1/relink_fold.patch`, `tools/relink.py` +64 -14):
- holes sorted by (lo, has num): a matched row comes first at a tied VA and is the kept copy.
- `same()` also accepts two different names when both are `func` pins at that VA in `symbols_pin.txt` (same object
  allowed), the duplicate is a code hole, and size, bytes, Selection (not NODUPLICATES or associative) and relocs
  are equal. Relocs must go to externals only and are compared as (offset, type, target VA). The same-name path is unchanged.
- each different-name copy is bound by a weak alias (dup name -> kept name, weak.obj SEARCH_ALIAS, the T6.1 path); every
  COMDAT copy of ours with a fold name is renamed `<name>@fold_<va8>` in each object of ours that defines it, and /OPT:REF
  drops the renamed copy. A non-COMDAT definition is refused.
- new `--control fold`: flips byte 0 of one copy of the first fold pair in memory, so `same()` returns false and :1034 refuses.
- Everything else is still refused (other overlaps, names that are not pins, data holes, different bytes or relocs).

Scratch row edits (`.run/4.3/t2/c1/rows.patch`): guards lifted on the 3 rows; `src/Libs/Swamp/Xbox/Official_Release/SwampRenderer.cpp:223`
decl-only `template<> ResourceHandle<VertexShaderResource>::operator=` removed (kept: ClearStateCache objdiff 77.54);
HUDUI row tags restored from 7c554e91; matched_set +3 functions, +1 emitter (`pin_emit_HealthUI_dtor`), 615 -> 618.

Observed (scratch, cwd asserted):
- before, unpatched relink.py: `match_one` PASS objdiff 100, bytes match, on all 3 rows; `relink.py check` exit 2 `our ranges overlap at 0x0001819A`.
- after: `relink.py check` exit 0, `check all: digests 16/16 OK`, relinked sha1 = pin. Dups: 0x1819A TextureResource op= kept,
  VertexShaderResource op= folded; 0x292A6 VertexShaderResource dtor kept, HUDUI TextureResource and Hannah/VinceSwing AnimationResource dtors folded.
- after: `match_one` PASS objdiff 100.0, bytes match (masked 0) on 0x000CA770, 0x000E88E1, 0x000292A6.
- planted copy whose bytes differ: `relink.py check --control fold` exit 2 `our ranges overlap at 0x0001819A` (refused, as required).
- `--control literal` still exit 2 at 0x001CDBA8; HEAD sources with the patched relink.py: exit 0, sha1 = pin (no regression).
- scratch gate `618 passed, 0 failed of 618; FAIL globals: check_env, provenance`. Both globals failed for scratch-only reasons
  (hooksPath set by the coder; no upstream). The relink layer passed.

Frees 3 rows: 0x000CA770, 0x000E88E1, 0x000292A6. Touches: `tools/relink.py` (LISTED, `tools/verifier_lock.py:50`) ->
`config/verifier.sha256` one line (scratch `verifier_lock write`: 1-line diff); the docstring :84-93; self-test gets a
new relink control `fold` (exit 2, `our ranges overlap`), so controls go 106 -> 107; src: SwampRenderer.cpp:223 removal,
HUDUI.cpp and Hannah.cpp guard lifts; `config/matched_set.toml` +3 functions, +1 emitter.
Risk: the weak alias assumes no cut (non-our) object defines a fold name strongly. Cut-hole aliases are deleted (:1219).
This was shown for these two VAs only.

## Wall 2: cut name our unit cannot instantiate
Smallest change: reorder lines inside a VA block of `config/retail/symbols_pin.txt` so that a PDB-public alias our unit
can instantiate comes last. 5 lines move (10 diff lines), no content changes. Every alias is already a
`proto-pdb-public+fold-alias` pin (D78-clean). Then `delink --fresh` + `relink.py cut` (~10 min).
- 0x0006A5D2: aliases DoDebugUpdates, CutsceneDriver, LanternMain, LevelFlythroughCamMain (cut). Last -> `?FrameTick@LanternMain@@UAEXXZ`
  (Lantern.cpp:96-103; retail Lantern.obj defines LanternMain's ctor, Message, creationFunc). The other 3 are absent from Lantern.cpp.
- 0x000775B9, 0x000775E2: vector<Matrix> vs vector<TrolleyManager::MapStop> (cut). Last -> Matrix (Blender.cpp:253, drafts
  :450-465; retail Blender.obj defines resize<Matrix>). MapStop/TrolleyManager are absent from Blender.cpp.
- 0x0001819A: 7 ResourceHandle<...> op= aliases, VertexShader cut. Last -> `??4?$ResourceHandle@VAnimationResource@@@@QAEAAV0@ABV0@@Z`.
  Retail Onion.obj calls this op= from ??0OnionMain and ??0TrapOnionMain, each call right after a
  ResourceManager<AnimationResource>::Load result. Needs AnimationResource decl + op= body in Onion.cpp (as ShootingCam.cpp:221, SwampRenderer.cpp:225).
- 0x000292A6: 4 ResourceHandle<...> dtor aliases, VertexShader cut. Last -> AnimationResource (??1HannahMain is the in-unit caller).
  Optional: with wall 1 changed, the current cut name already passes (see above).
Observed (scratch):
- pin-row check (`pin_rows`) on the changed pins: before `3011 rows, 3011 pass; fails 0`; after (reordered) `3011 rows, 3011 pass; fails 0`.
- `match_one` before: 0x6A5D2, 0x775B9, 0x775E2, 0x1819A objdiff None, bytes match, rc 1. After: all objdiff 100.0, bytes match, rc 0;
  0x292A6 under the AnimationResource name objdiff 100.0, rc 0.
- `relink.py check`, pins reordered and src/matched set unchanged: exit 0, 16/16, sha1 = pin. With all 5 rows listed: exit 2 at 0x0001819A (wall 1).
  Without Onion: exit 2 at 0x000292A6 (wall 1). Lantern + 2 Blender only: exit 0, 16/16, sha1 = pin.
- not run: gate.py on the reordered pins.
Frees 3 rows alone: 0x0006A5D2, 0x000775B9, 0x000775E2. Touches: `config/retail/symbols_pin.txt` only (not LISTED, not in
`config/verifier.sha256`). Self-test control `pin-alias-dup` (gate.py:1549, 1576-1595) uses 0x292A6 names for a list
check that does not depend on order. src guard lifts in Lantern.cpp, Blender.cpp; Onion.cpp draft; matched_set adds.

## Both walls together
- 0x0001819A needs both changes (reorder + Onion draft, and wall 1 for SwampRenderer's TextureResource op= at that VA).
  Expected to pass but NOT observed: no scratch run applied both. It needs one combined check before it counts.
- Total: 6 rows observed freed (3 + 3), 7 expected with 0x1819A. 0x000292A6 is freed by wall 1 alone; reorder it
  only if the combined run shows it is needed.
- Not freed by either: 0x000BE119, 0x000C1028, 0x000C0299. SwampResourceManager.obj has a unit in `config/retail/units.toml`:4188
  but no source in `src/` (opt unknown; VC7.1 `_Tree<map<string,T*>>` callees). These rows need a unit bring-up, not a relink or pin change.
- Seen in scratch, outside this change and unverified on the real tree: `relink.py check --control entry` at HEAD exits 1
  (image FAIL), not 2 `our PE entry`. The gate's entry control (gate.py:1091) expects rc 2. The real self-test is green
  106/106 (card), so this may come from the scratch setup. Check it before relying on it.

## unknown01: RELEASE<Direct3D>
- Row: `??$RELEASE@UDirect3D@@@@YAXAAPAUDirect3D@@@Z` 0x000CA38E, 13 B, rank 38, SwampRenderer.obj, bucket held-d78
  (`tools/pool_buckets.py:47`; `config/retail/symbols.txt:2222`).
- What retail does: RELEASE returns void (`YAX`). It loads the pointer argument, tests `*p` for zero, stores 0 if non-zero, and returns.
  There is no call: `Direct3D::Release` is inlined, and its ULONG return value is discarded, so no constant appears in the bytes.
  Any returned constant compiles to the same bytes (4.2 T3).
- PDB: `Direct3D` size 1, 14 static methods, `Release` static returning prim 0x22 (ULONG). No nested enum, no enumerator;
  the name `unknown01` does not occur in `pdb_types.py names`. There is no public `?Release@Direct3D` or `Direct3D_Release`.
- Scratch (4.2 T4.c1): `struct Direct3D { enum { unknown01 }; ... return unknown01; }`, unit 20/20 match. The pa-thinker's
  4.2 text (logs/T4.md:89-93; not a developer ruling): free with the value as unknown; if the bytes move, use enumerator `unknown01`. Rejected: bare
  `return 1`, void, specialization. No ruling since (R4.3-003).
- Options under D78 (no field, type, callee, guard or constant absent from the PDB, disassembly or strings):
  - a neutral `!FAKE` enumerator `unknown01`: adds an enum the PDB lacks. The bytes are equal and the marker says it is
    invented. D78 needs the developer's explicit exception, as the 4.2 text gives.
  - a plain constant (`return 0` / `return 1`): adds a constant absent from every source, without a marker. It reads
    as a fact. Rejected in 4.2.
  - held: the row stays out (1 row, 13 B). Nothing invented.
- Recommendation: the `!FAKE` enumerator `unknown01`, value unknown, on the developer's word. It is the form the 4.2 text
  already names, it moves no bytes, and the marker keeps the invention visible. If the developer prefers strict D78, the row stays held.
- Ruling (developer, 4.3 T3, 2026-11-02): held. No enumerator, no D78 exception; the row stays out.
