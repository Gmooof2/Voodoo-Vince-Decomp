# Unit config and symbol file seed (T10, phase 1.3)

Run 2026-10-25, `PY tools/units_seed.py --write --force` (regenerated after the window-resolution ruling) then `--check` (exit 0; rerun byte-identical). Retail USA
`default.xbe` sha1 `2953b78229018a34e7f53e85bd34f124d51ef210`. Outputs (tracked, hand-edited after seeding):
`config/retail/units.toml` (body-sha1 `23a7407f…`), `config/retail/symbols.txt` (body-sha1 `d08f70db…`). Names,
addresses and counts only (D46). Status: positions are proto_map `unverified` matches; nothing here is match evidence
(D81). Tool detail: `docs/ops/tools.md` "units_seed"; technique: C0020.

## Inputs
- `default.pdb` modules, procs, publics, compile records, DBI section contributions (`pdb_info.parse_pdb`, C0018).
- Kind per module: T9 `derived/xdk/retail-2953b782/modules.jsonl` (game 306, xdk 382, linker 1, unknown 0).
- Retail positions: `derived/proto_map/retail-xbe/functions.jsonl`; anchors = `lis_anchors` over identical hits==1
  rows, 4938 (== proto_map summary). Labels: T5 `derived/ghidra/retail-2953b782/symbols.jsonl`.

## Prototype layout finding (why ranges come from the main run)
- Proto exec contributions collapse into runs of consecutive same-module records: `.text` 9475 records, 792 runs, 502
  modules. Each game module has one main run in link order, plus pieces in a section tail after every main run
  (`.text` tail from +0x13BD4E to the end, 0x13F2F4): the linker's grouped `$` sub-sections, in module order:
  proc-less 10-B multiples (EH handler thunks), `$E<n>` init/atexit stubs, local-static atexit stubs.
- 198 of 306 game modules have own procs in the tail (893 procs, `procs_tail`); 55 proc-less tail runs, 2620 B.
- True interleaving (own procs inside another module's main run): 12 procs in 9 units, all xdk, section XPP
  (USB/OHCI `*_Init`, `HCD_*`).
- Folds: 1674 of 12513 PDB procs sit in another module's contribution (ICF, C0018) and are not the module's own.

## Bounding rule
- Own proc = inside one of its own module's exec contributions. Main run = the run with most own-proc bytes.
- Start edge: `anchor` if the run's first proc is an anchor at the run start (start = its retail VA); else `neighbour`
  if the previous run ends with an anchor proc (start = that anchor's retail end; the cut between two units is known
  when either side's edge proc is an anchor); else `unpinned-<class of the edge proc>` and the range starts at the
  run's first anchor. End edge symmetric.
- Anchor-window resolution (ruled into T10 after the first seed returned 33): an ambiguous proc that is the first or
  last own proc of a run is resolved iff its hit list is complete (hits <= 32, the proto_map cap), exactly one hit
  lies in the retail gap between its nearest LIS anchors (proto VA order, same section), and that hit equals the
  proto-offset prediction from the previous or the next anchor. Zero or several hits = unresolved, no guessing. A
  resolved proc pins its own edge (`window`) or the neighbour's (`neighbour-window`); it is never an anchor and does
  not enter the consistency check. Tally: considered 559, resolved 39; unresolved: no hit in gap 428, several hits in
  gap 73, no anchor on a side 18, hit not at proto-offset position 1, capped 0. All 28 `window` edges re-verified
  independently against proto_map rows (one-off check, result in the 1.3 T10 log).
- Range present iff the main run holds >= 1 anchor, inside one retail exec section; overlapping ranges are dropped
  (none occurred). The tail pieces are not in `text_start..text_end`: a delinked unit lacks its tail stubs.
- Counts (all kinds): start anchor 425, neighbour 21, window 19, unpinned ambiguous 15 / near-identical 28 /
  identical 10 / excluded 5 / absent 3; end anchor 330, neighbour 107, neighbour-window 14, window 9, unpinned
  ambiguous 48 / near-identical 14 / absent 2 / excluded 2. No range 163: no exec contribution 99, no own procs 42, no
  anchor in main run 22. Ranged 526.

## Selection rule (`delink = true`)
- kind game, range present, both edges `anchor`, `neighbour`, `window` or `neighbour-window`, no interleaved (split)
  procs, and every near-identical candidate and non-LIS identical VA of the main run's own procs inside the range.
  Else `delink_reason` lists every failing item: kind, no range, edge start/end unpinned-<class>, split, inconsistent.
- Selected: 143 game units, 317976 B (game ranged 288 units, 1159278 B); 22 of them (60983 B) have a window edge.
- Before resolution: 127 units, 270660 B, and 33 game units (108599 B) failed only on unpinned ambiguous edges.
  Resolution released 9 of the 33 (23303 B); 24 stay gated (85758 B). 7 more game units (223, 286, 290, 302, 303,
  354, 467) went false -> true: each had failed on an unpinned edge plus `inconsistent: 1-2 procs`, an artefact of the
  narrow fallback range (first/last anchor); once the edge was pinned the wider range passed the same consistency rule.
- Game units not selected, by first reason: inconsistent 40, edge end unpinned-ambiguous 34, edge start
  unpinned-near-identical 28, no range 18 (no anchor in main run 16, no exec contribution 2), edge start
  unpinned-ambiguous 15, edge start unpinned-identical 10, edge end unpinned-near-identical 9, start unpinned-excluded
  5, start unpinned-absent 3, end unpinned-absent 1.
- Ambiguous-boundary count after resolution: 24 game units (85758 B). Near-identical and inconsistent edges are out of
  scope (ruling).

## Symbol file
- proto-pdb:default.pdb 4738: identical hits==1 procs whose retail VA no other proto proc maps to and whose protoVA
  has exactly one S_PUB32 name (decorated), size = proto length, status unverified.
- XDK labels: xbsymdb 59, ghidra (xtlid, not xbsymdb) 48; kernel labels skipped; func with T5 size when a T5 function
  starts there, else data.
- One name per VA (proto-pdb > xbsymdb > ghidra), each name once. Drops: VA already named 369, >1 S_PUB32 at the
  protoVA 254, no S_PUB32 at the protoVA 108 (statics), retail VA mapped by another proto proc 34 (folds), label
  outside every retail section 6 (`*_OFFSET`-style offsets). 4845 lines.
- Header: retail sha1 and the scope rule (a symbol enters only the file of the binary it lives in; BFM 2026-07-14,
  D53). Placeholder lint in `--check`: `FUN_/DAT_/LAB_/sub_/func_<hex>` must equal the VA (none emitted).

## Unit-edge route (3.2 T1)
binding: a retail unit range is fixed only from retail evidence; a unit is `delink = true` when both edges are in
`EDGE_OK` and every main-run proc outside its range is explained by a class below; else `delink_reason` keeps the
failing item and names what was searched. Never a prototype offset alone (D78): an offset prediction counts only with
equal retail bytes there. Status note: names/VAs only (D46).
- Route taken: per-unit edges from retail evidence (this section). Route not taken: per-function target objects
  (`delink.py one`, derived/delink_one/). The gate and `match_one.py` read the per-unit object only
  (`gate.py:189-200`, `match_one.py:245`, R3.2-001); a per-function target means editing `gate.py`, a locked file
  (`config/verifier.sha256`); `build.py` flags and the relink partition are per unit anyway (`relink.py:155-190`). The
  unit route touches no locked file (`units_seed.py`, `units.toml`, `delink.py` are not LISTED).
- Edge classes (`edge_start`/`edge_end`; `tools/units_seed.py` EDGE_OK):
  - `anchor`, `neighbour`, `window`, `neighbour-window`: as seeded (1.3 T10, above). Anchor = LIS-consistent unique
    identical proc (C0011).
  - `chain`: from the unit's own LIS anchor nearest the edge, every following own proc of the main run up to the run
    edge (preceding, for a start) is masked-equal (`name_sites.Images.masked_equal`, proto .reloc masked, C0011) to
    the retail bytes at its proto-offset-predicted VA, and is the only masked copy in the retail gap between its
    bracketing LIS anchors (uncapped scan of the gap, or the gap is exactly filled by the chain). The window rule
    extended to capped hit lists and to several procs. Edge = predicted end (start) of the chain.
  - `chain-neighbour`: a `chain` whose end equals the retail start of an LIS anchor owned by the adjacent unit, or of
    a `disasm-verified` pin of an adjacent unit's proc (end side; symmetric for a start): the cut is closed from both
    sides.
  - `neighbour-fixed` (4.4 T8): the adjacent run in proto link order (the main run before, for a start; after, for an
    end) belongs to another unit whose facing edge is already fixed in `units.toml` (edge class in EDGE_OK, not itself
    `neighbour-fixed`), and that VA lies in our own anchor gap (no other unit's LIS anchor between). The seed's
    `neighbour` rule with the neighbour's fixed edge in place of its LIS anchor: the bytes between our last anchor and
    the neighbour's fixed edge have no third owner. Tried after `pin` and `chain`; the C0020 widened-bytes check and the
    outside-proc check run on the result as for every edge. Released 13 units at 4.4 T8 (validation.md "4.4 Cuts").
  - Pins: a `disasm-verified` pin (3.1 T5/T7, byte-checked) whose name is the PDB public of the edge proc fixes the
    edge as an anchor would (pin VA, pin VA + proc len). A `boundary` pin proves a function start only: it may close
    a chain as the far side, never assign ownership alone. `site-corr` and `unverified` pins never fix an edge.
  - Class `no-range` (3.2 T9, game units `no range: no anchor in main run`): pin anchors = main-run procs whose PDB
    public has a `disasm-verified` pin and that are masked-equal there; all pins one delta. Edge = `pin` (edge proc
    pinned) or `chain` from the first/last pin anchor; then the whole range passes the C0020/overlap/outside checks.
    Identical hits-1 non-LIS copies are cited in the reason only, never anchors. 16 units: Particle#451 released
    0x0011FBE9-0x0011FC4F pin/pin; SwampTextureResource#404 kept (chain from its one pin breaks); 14 have no pin.
- Outside-proc classes (`inconsistent: N procs outside range`):
  - moved (COMDAT placement): the proc's single retail masked copy (proto_map identical, hits 1) lies between two LIS
    anchors owned by one other unit. That unit's retail block is contiguous between them, so the copy is its
    contribution (template/inline COMDAT kept from the first obj in retail link order); the proc's bytes occur nowhere
    in our range. Explained.
  - not explained by T1 (stay in the reason): copy in a gap between units, copy bracketed by anchors of two different
    units, copy inside another retail function body (a decoy). Tally over the 40 inconsistent-only game units: 78
    outside procs, all class identical, 60 moved, 18 not explained: 13 copies in a gap between unit ranges, 5 inside
    a unit range but between anchors of two different units; 6 of the 18 start inside a Ghidra function body (a
    decoy hint, not evidence). Whammy's 0x001338E4 copy is Roach#505's chain end: a chain may explain gap copies.
- Consistency re-check after any edge change (C0020): a widened range is re-tested over the same rule; the procs it
  newly covers must be the chain's own.
- Tally for the 20 `edge end unpinned-ambiguous`-only game units: 18 chains byte-equal (the edge proc is mostly the 60-B
  `X::RegisterMe::RegisterMe`, hits capped at 32), 2 broken (TextRegion#454, VinceHang#491: proc changed in retail);
  1 closes on a neighbour anchor (Switch#331). Not yet gap-scanned except Switch (exactly filled).
- Shown on (3.2 T1):
  - `Xbox\OfficialRelease\Init.obj#7` 0x00011000-0x000120C1, edges anchor/neighbour; outside procs (both moved):
    `std::_Tree<...ScriptTimer...>` proto 0x004005BA (54 B) -> 0x0002DF9F between FanBoat.obj anchors 0x0002DF5B and
    0x0002E2E6; `std::list<...ScriptTimerEvent...>` proto 0x0040072B (45 B) -> 0x000C1AA9 between SwampWorld.obj
    anchors 0x000C19A1 and 0x000C1D7E. Range unchanged.
  - `Xbox\OfficialRelease\Switch.obj#331` end 0x0008B9DA -> 0x0008BA3D, `chain-neighbour`: anchor
    `SwitchMuseumMain::Load` 0x0008B96D+0x6D; `creationFunc` 0x0008B9DA+0x27 masked-equal, a proto_map hit (the other
    hit 0x0007ECF4 is pinned `disasm-verified` `?creationFunc@SwitchSafeMain@@...`), at its proto offset;
    `RegisterMe::RegisterMe` 0x0008BA01+0x3C masked-equal (hits capped); the gap Load-end..next anchor (0x63 B) is
    exactly the two procs; 0x0008BA3D = LIS anchor `TerradactylMain::SetState`, owned by Terradactyl.obj#332.

## Target before and after on the matched set (3.2 T5)
- Stages, each a delink `--fresh` hashes.txt: S0 = T2 before (after T1) `bfaa2e93816c...`; S1 = T2 after `8740b130ddf1...`
  (= T4 before, T3 changed no target); S2 = T4.c1 `eddb8aa3f445...`; S3 = T4.c3 = now `2f01a28343f2...`.
- S0 and S1 re-made from `git archive 5b442887` / `b9005459` trees in `.run/3.2/T5/s0|s1` (own Ghidra project dir):
  hashes digests equal the recorded ones; objects diffed per unit (section sizes, external/static symbols, relocs).
- T1: S0 minus the two new units (`Xbox\OfficialRelease\Init.obj`, `...\Switch.obj#331`) hashes to `b9e646d5...`, the
  3.1 close: T1 changed no existing target; neither new unit holds a matched function.
- Matched set: 115 functions, 49 units; 16 units changed (2 T2, 14 T4.c1, 0 T4.c3), 33 unchanged.
- T4.c1 added relocs per unit: Blender 8, BeepState 8, EvilVince 6, CrawdadJimmys 4, FrontEndUI 3, SwampLoader 3, the
  other 8 one each; targets existing symbols (19 `__except_list`, string literals, globals).
- objdiff: gate pass means fuzzy_match_percent == 100.0 (`tools/gate.py:235`); bytes = the gate's XBE byte compare.
  Before = `.run/3.2/logs/T2-before-gate.log` (S0, 115/115); after = `.run/3.2/logs/t5-gate.log` (S3, 115/115,
  `PASS relink: pass`). No file in `config/verifier.sha256` changed 3bfac8cd~1..HEAD; `verifier_lock.py check` pass.
- Rows: `symbol` | unit | S0 sha256[:12] -> S3 sha256[:12] | objdiff before/after | bytes before/after | cause.
- `?BackupState@GameOptions@@QAEXXZ` | core\XBox\OfficialRelease\Globals.obj | 8fc625df6009 -> 8fc625df6009 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?RestorePreviousState@GameOptions@@QAEXXZ` | core\XBox\OfficialRelease\Globals.obj | 8fc625df6009 -> 8fc625df6009 | objdiff 100.0/100.0 | bytes pass/pass | same
- `_LocString_GetStringID` | Xbox\OfficialRelease\LocString.obj | 7e0f7808f3d1 -> 91af1b2b59f9 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?CheckForMMX@BeepState@@AAEXXZ` | Xbox\Official_Release\BeepState.obj | 6ccab8813b72 -> 4b8d9806e054 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?CheckForRDTSC@BeepState@@AAEXXZ` | Xbox\Official_Release\BeepState.obj | 6ccab8813b72 -> 4b8d9806e054 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?CheckForSSE@BeepState@@AAEXXZ` | Xbox\Official_Release\BeepState.obj | 6ccab8813b72 -> 4b8d9806e054 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `??1FlourSackMain@@UAE@XZ` | Xbox\OfficialRelease\FlourSack.obj | cc1643ac65c9 -> cc1643ac65c9 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?MarkSceneRenderingStartTime@Swamp@@QAEXXZ` | Xbox\Official_Release\Swamp.obj | 68cf8a56685e -> df78e54b4340 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?TimerEvent@DoorStorageRoomMain@@UAEXI@Z` | Xbox\OfficialRelease\DoorStorageRoom.obj | 8c8f4f7e42b2 -> 8c8f4f7e42b2 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?RenderPersisted@LoadScreen@@QBEXXZ` | Xbox\Official_Release\LoadScreen.obj | b0dae2fa806c -> b0dae2fa806c | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetGameTime@SwampInput@@QAEXM@Z` | Xbox\Official_Release\SwampInput.obj | 3a67ec204716 -> f3dc5a90234a | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?Activate@GeneratorMain@@UAEXXZ` | Xbox\OfficialRelease\Generator.obj | a169528dfc96 -> a169528dfc96 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?AnimationFinished@VinceLaundryCar@@UAEXXZ` | Xbox\OfficialRelease\BasketCase.obj | e7e79d827359 -> e7e79d827359 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetViewPosition@SwampCamera@@UBE?AUVector3@@XZ` | Xbox\Official_Release\SwampCamera.obj | 443de863df81 -> 443de863df81 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?AnimationFinished@CutsceneAnimationScript@@UAEXXZ` | core\XBox\OfficialRelease\Cutscene.obj | ba600f2593e8 -> ba600f2593e8 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Play@MoviePlayer@@QAEXXZ` | Xbox\Official_Release\MoviePlayer.obj | aa6ffcc85c9a -> 804a34de98d9 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?DoNotifyParent@SoundNode@@IAEXXZ` | Xbox\Official_Release\SoundNode.obj | 9d8ae14ae71b -> 9d8ae14ae71b | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Update@SoundNode@@UAEXXZ` | Xbox\Official_Release\SoundNode.obj | 9d8ae14ae71b -> 9d8ae14ae71b | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetRenderTexture@MoviePlayer@@QBEPAUD3DTexture@@XZ` | Xbox\Official_Release\MoviePlayer.obj | aa6ffcc85c9a -> 804a34de98d9 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?Overlaps@AACylinder@@QBE_NABU1@@Z` | Xbox\Official_Release\SwampGeometry.obj | 584225e444cd -> 584225e444cd | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetRecord@SwampInput@@AAEABUInputRecord@1@XZ` | Xbox\Official_Release\SwampInput.obj | 3a67ec204716 -> f3dc5a90234a | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?SetState@VinceDropsInCutscene@@EAEXH@Z` | Xbox\OfficialRelease\JaniceCutscenes.obj | 078ba8618deb -> 078ba8618deb | objdiff 100.0/100.0 | bytes pass/pass | same
- `_main` | core\XBox\OfficialRelease\Main.obj | 56867b5f0662 -> 56867b5f0662 | objdiff 100.0/100.0 | bytes pass/pass | same
- `_adler32` | Xbox\Official_Release\adler32.obj | aae9d74a520a -> aae9d74a520a | objdiff 100.0/100.0 | bytes pass/pass | same
- `?BeginScene@Swamp@@QAEXXZ` | Xbox\Official_Release\Swamp.obj | 68cf8a56685e -> df78e54b4340 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `??1BeepState@@QAE@XZ` | Xbox\Official_Release\BeepState.obj | 6ccab8813b72 -> 4b8d9806e054 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `??4?$ResourceHandle@VAnimationControl@@@@QAEAAV0@ABV0@@Z` | Xbox\OfficialRelease\Generator.obj | a169528dfc96 -> a169528dfc96 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?AnimationFinished@LaundryCarMain@@UAEXXZ` | Xbox\OfficialRelease\BasketCase.obj | e7e79d827359 -> e7e79d827359 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Activate@BasketStopperMain@@UAEXXZ` | Xbox\OfficialRelease\BasketCase.obj | e7e79d827359 -> e7e79d827359 | objdiff 100.0/100.0 | bytes pass/pass | same
- `_zcfree` | Xbox\Official_Release\BeepZlib.obj | 8d58f413c00a -> 8d58f413c00a | objdiff 100.0/100.0 | bytes pass/pass | same
- `_inflateReset` | Xbox\Official_Release\inflate.obj | 0159a04fe946 -> 7fab11700244 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `_inflateEnd` | Xbox\Official_Release\inflate.obj | 0159a04fe946 -> 7fab11700244 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `_inflateInit2_` | Xbox\Official_Release\inflate.obj | 0159a04fe946 -> 7fab11700244 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `_inflateInit_` | Xbox\Official_Release\inflate.obj | 0159a04fe946 -> 7fab11700244 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `_inflate_blocks_reset` | Xbox\Official_Release\infblock.obj | 4a5cfbf2d4d5 -> 4a5cfbf2d4d5 | objdiff 100.0/100.0 | bytes pass/pass | same
- `_inflate_blocks_new` | Xbox\Official_Release\infblock.obj | 4a5cfbf2d4d5 -> 4a5cfbf2d4d5 | objdiff 100.0/100.0 | bytes pass/pass | same
- `_inflate_blocks_free` | Xbox\Official_Release\infblock.obj | 4a5cfbf2d4d5 -> 4a5cfbf2d4d5 | objdiff 100.0/100.0 | bytes pass/pass | same
- `_inflate_codes_new` | Xbox\Official_Release\infcodes.obj | fd033e633674 -> fd033e633674 | objdiff 100.0/100.0 | bytes pass/pass | same
- `_inflate_codes_free` | Xbox\Official_Release\infcodes.obj | fd033e633674 -> fd033e633674 | objdiff 100.0/100.0 | bytes pass/pass | same
- `_inflate_flush` | Xbox\Official_Release\infutil.obj | d335209ee2b2 -> d335209ee2b2 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?AnimationFinished@VinceZapHat@@UAEXXZ` | Xbox\OfficialRelease\VinceZapHat.obj | 187e4add495e -> 187e4add495e | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Message@SwingHookTargetMain@@UAEXPAVSwampScript@@HPAX@Z` | core\XBox\OfficialRelease\SwingHook.obj | 0117a4ee9c39 -> 0117a4ee9c39 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?ResumeInputStream@SwampInput@@QAEXXZ` | Xbox\Official_Release\SwampInput.obj | 3a67ec204716 -> f3dc5a90234a | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?ResumeGame@SwampInput@@QAEXXZ` | Xbox\Official_Release\SwampInput.obj | 3a67ec204716 -> f3dc5a90234a | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?LoadScreenFinished@LoadScreen@@QAE_NXZ` | Xbox\Official_Release\SwampLoader.obj | ba1cce61b4b7 -> 1265a25b07c8 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?DeActivate@Giblet@VinceGiblets@@QAEXXZ` | Xbox\OfficialRelease\Blender.obj | 9620fcc1e364 -> e652d77ea77a | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?Activate@OvenMain@@UAEXXZ` | Xbox\OfficialRelease\Oven.obj | 1eec699f7fd4 -> 255a5a308d5c | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?SetUV@Sprite@@QAEXMMMM@Z` | Xbox\Official_Release\Sprite.obj | 5daa600987da -> 302fb344459b | objdiff 100.0/100.0 | bytes pass/pass | split (T2): rel32 at .text+0xA1 retargeted `.text` -> `?GetTexture@SingleTexture@@IBEPAUD3DTexture@@XZ`
- `?SetMinDistance@SoundNode@@QAEXM@Z` | Xbox\Official_Release\SoundNode.obj | 9d8ae14ae71b -> 9d8ae14ae71b | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetMaxDistance@SoundNode@@QAEXM@Z` | Xbox\Official_Release\SoundNode.obj | 9d8ae14ae71b -> 9d8ae14ae71b | objdiff 100.0/100.0 | bytes pass/pass | same
- `?RotateJ@Vector3@@QAEXM@Z` | Xbox\Official_Release\SwampVector.obj | e20980977c42 -> e20980977c42 | objdiff 100.0/100.0 | bytes pass/pass | same
- `??0Bundle@SwampLoader@@QAE@ABU01@@Z` | Xbox\Official_Release\SwampLoader.obj | ba1cce61b4b7 -> 1265a25b07c8 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?GetRenderAlpha@FFlyEmitter@@QBEMMM@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetRenderSize@FFlyEmitter@@QBEMMM@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Apply@FFlyFloor@@UAEXAAVFFlyEmitter@@PAUFFlyParticle@@I@Z` | Xbox\Official_Release\FFlyForce.obj | 27e798344c66 -> 27e798344c66 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetFileData@BeepFile@@QAEPBXXZ` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?TimerEvent@AmbientBirdMain@@UAEXI@Z` | core\XBox\OfficialRelease\AmbientBird.obj | 344e09116953 -> 2c6e66c738e8 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?AnimationFinished@AmbientBirdMain@@UAEXXZ` | core\XBox\OfficialRelease\AmbientBird.obj | 344e09116953 -> 2c6e66c738e8 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?TimerEvent@CeilingFanMain@@UAEXI@Z` | core\XBox\OfficialRelease\FanCeiling.obj | 159683c38f4a -> 159683c38f4a | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Render@FFlyEmitter@@QBEXPAVFFlyRenderer@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetSurface@FFlyEmitter@@QAEXPAVFFlyEmitterSurface@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetParam1Ctrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetParam2Ctrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetLocationCtrl@FFlyEmitter@@QAEXPAVFFlyControllerV3@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetRotationCtrl@FFlyEmitter@@QAEXPAVFFlyControllerV3@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetRateCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetLifeSpanCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetAngleCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetSpeedCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetColorCtrl@FFlyEmitter@@QAEXPAVFFlyControllerV3@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetAlphaCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetTexIdCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetSizeCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetRenderColorCtrl@FFlyEmitter@@QAEXPAVFFlyControllerV3@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetRenderAlphaCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetRenderTexCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetRenderSizeCtrl@FFlyEmitter@@QAEXPAVFFlyController@@@Z` | Xbox\Official_Release\FFlyEmitter.obj | b7631b185032 -> b7631b185032 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Activate@MondoMoleMain@@UAEXXZ` | core\XBox\OfficialRelease\MondoMole.obj | 79b9a2dc6d4f -> 81402d74e19a | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?AnimationFinished@MondoMoundMain@@UAEXXZ` | core\XBox\OfficialRelease\MondoMole.obj | 79b9a2dc6d4f -> 81402d74e19a | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?GainingControl@CrawdadJimmyMain@@UAEXPAVSwampScript@@@Z` | Xbox\OfficialRelease\CrawdadJimmys.obj | 92eba117e002 -> 1b6f3e1b29ce | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?StateTransition@CutsceneDriver@@IAEXIH@Z` | core\XBox\OfficialRelease\Cutscene.obj | ba600f2593e8 -> ba600f2593e8 | objdiff 100.0/100.0 | bytes pass/pass | same
- `??1Bundle@SwampLoader@@QAE@XZ` | Xbox\Official_Release\SwampLoader.obj | ba1cce61b4b7 -> 1265a25b07c8 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?KillRope@VinceSwing@@AAEXXZ` | core\XBox\OfficialRelease\VinceSwing.obj | e82614692bd0 -> e82614692bd0 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?StopEmitting@VinceGiblets@@QAEXXZ` | Xbox\OfficialRelease\Blender.obj | 9620fcc1e364 -> e652d77ea77a | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?GetAllocationHeader@@YAPAUUntrackedAllocationHeader@@PBX@Z` | Xbox\Official_Release\BeepAlloc.obj | a1fb0dc4e57f -> a1fb0dc4e57f | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Invalidate@FrontEndMenu@@UAEXXZ` | Xbox\OfficialRelease\FrontEndUI.obj | 68d90577b16b -> 715e4f48fb3f | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?Pos@StandardFileHandler@@UAEHXZ` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?RenderToScreen@SwampRenderer@@QAEXXZ` | Xbox\Official_Release\SwampRenderer.obj | 41d31c7f57cc -> 41d31c7f57cc | objdiff 100.0/100.0 | bytes pass/pass | same
- `??$_Uninit_fill_n@PAUVector3@@IU1@V?$allocator@UVector3@@@std@@@std@@YAXPAUVector3@@IABU1@AAV?$allocator@UVector3@@@0@U_Nonscalar_ptr_iterator_tag@0@@Z` | Xbox\OfficialRelease\EvilVince.obj | df31a09e0dac -> cb982a716583 | objdiff 100.0/100.0 | bytes pass/pass | reloc (T4.c1 scalar-ref pass): only DIR32 relocs added, no symbol, section or size change
- `?VinceOnUpperFloor@DinoBossMain@@AAE_NXZ` | Xbox\OfficialRelease\DinoBoss.obj | 50830bf52273 -> 50830bf52273 | objdiff 100.0/100.0 | bytes pass/pass | same
- `??$RELEASE@UD3DDevice@@@@YAXAAPAUD3DDevice@@@Z` | Xbox\Official_Release\SwampRenderer.obj | 41d31c7f57cc -> 41d31c7f57cc | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetProjectionTransform@SwampRenderer@@QBEXAAUMatrix@@@Z` | Xbox\Official_Release\SwampRenderer.obj | 41d31c7f57cc -> 41d31c7f57cc | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetBasicPixelShader@SwampRenderer@@QAEXXZ` | Xbox\Official_Release\SwampRenderer.obj | 41d31c7f57cc -> 41d31c7f57cc | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetBasicVertexShader@SwampRenderer@@QAEXPBU_D3DVERTEXATTRIBUTEFORMAT@@@Z` | Xbox\Official_Release\SwampRenderer.obj | 41d31c7f57cc -> 41d31c7f57cc | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetRoot@Bog@@QAEPAUBogNode@@XZ` | Xbox\Official_Release\SwampBog.obj | c1a41c24fcb1 -> c1a41c24fcb1 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetMaxTriCount@Bog@@QBEIXZ` | Xbox\Official_Release\SwampBog.obj | c1a41c24fcb1 -> c1a41c24fcb1 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetFlags@Bog@@QBEIXZ` | Xbox\Official_Release\SwampBog.obj | c1a41c24fcb1 -> c1a41c24fcb1 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetVertices@Bog@@QBEPBUBogVertex@@XZ` | Xbox\Official_Release\SwampBog.obj | c1a41c24fcb1 -> c1a41c24fcb1 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Render@VinceIntro@@QAEXXZ` | core\XBox\OfficialRelease\Intro.obj | bbe3a129047c -> bbe3a129047c | objdiff 100.0/100.0 | bytes pass/pass | same
- `?Seek@StandardFileHandler@@UAEXH@Z` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?SeekFromEnd@StandardFileHandler@@UAEXH@Z` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?SeekRelative@StandardFileHandler@@UAEXH@Z` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?Close@ZipFileHandler@@UAEXXZ` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?Write@BeepFile@@QAE_NPBXI@Z` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?Size@BeepFile@@QAEIXZ` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?Pos@BeepFile@@QAEHXZ` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?Seek@BeepFile@@QAEXH@Z` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `?SeekRelative@BeepFile@@QAEXH@Z` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `??0BeepFile@@QAE@XZ` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `??1BeepFile@@QAE@XZ` | Xbox\Official_Release\BeepFile.obj | 4f49fea5de1c -> 44e4e76c3b02 | objdiff 100.0/100.0 | bytes pass/pass | pin (T2): label `LAB_00037363` -> `boundary_00037363`, relocs unchanged
- `??1CXBFont@@QAE@XZ` | core\XBox\OfficialRelease\xbfont.obj | 1dd6c6b46a8f -> 1dd6c6b46a8f | objdiff 100.0/100.0 | bytes pass/pass | same
- `?GetBounds@RegionTrigger@@UBEABUAABox@@XZ` | Xbox\OfficialRelease\SwitchBoat.obj | e01000846f8b -> e01000846f8b | objdiff 100.0/100.0 | bytes pass/pass | same
- `?SetToLoading@VinceLoadScreen@@QAEXXZ` | core\XBox\OfficialRelease\LoadScreens.obj | 805f8207ad29 -> 805f8207ad29 | objdiff 100.0/100.0 | bytes pass/pass | same
- `?AnimationFinished@VinceExitTrolley@@UAEXXZ` | core\XBox\OfficialRelease\Trolley.obj | 74361d3c3b33 -> 74361d3c3b33 | objdiff 100.0/100.0 | bytes pass/pass | same
- `??1LaundryCarMain@@UAE@XZ` | Xbox\OfficialRelease\BasketCase.obj | e7e79d827359 -> e7e79d827359 | objdiff 100.0/100.0 | bytes pass/pass | same
