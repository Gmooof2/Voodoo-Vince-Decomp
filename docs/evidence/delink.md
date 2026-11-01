# Delink chain proof (phase 1.3 T6, 2026-10-25)

Retail XBE sha1 `2953b78229018a34e7f53e85bd34f124d51ef210`; Ghidra 12.1.4, ghidra-xbe build-202610092147,
ghidra-delinker-extension v0.9.2, objdiff-cli 3.8.3. Outputs under `derived/delink/retail-2953b782/` (gitignored).
Status: one function proven; not match evidence (D40 needs our compiled object, phase 1.4+).

## Function (chosen by rule, `PY tools/delink.py pick`)
- Rule: proto_map class identical, hits 1, one targetVA; PDB name; game module (no module path under `obj\i386\`
  (XDK libs), `D:\xboxfre-jun03\` or `* Linker *`); length >= 64; retail VA in no dup_report retail group/fold, proto
  VA in no proto group or multi_name row; T5 function size == length; >= 1 `call rel32` to another identical proc;
  >= 1 absolute operand into a data section (no exec flag, or `.rdata`/`.data`: this XBE flags both executable).
- 1155 candidates; pick = lowest VA.
- VA `0x00011800`, size 624 (0x270), range `0x00011800-0x00011A6F`; Ghidra / obj symbol `FUN_00011800`.
- PDB name `std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::erase`,
  module `Xbox\OfficialRelease\Init.obj`.

## Export
- Exporter: `COFF relocatable object` (delinker `CoffRelocatableObjectExporter`).
- `PY tools/delink.py one 0x00011800` runs `analyzeHeadless.bat <ghidra_project_dir>/retail-2953b782 retail-2953b782
  -process default.xbe -noanalysis -readOnly -postScript DelinkProgram.java /exporter "COFF relocatable object"
  /include-range 00011800-00011a6f /export derived/delink/retail-2953b782/0x00011800.obj`.
- Object: 2 sections, 52 symbols, 14 relocations.

## Relocation check (`PY tools/coff_info.py derived/delink/retail-2953b782/0x00011800.obj --check-xbe`)
- Types: `IMAGE_REL_I386_DIR32` 0x0006 x3, `IMAGE_REL_I386_REL32` 0x0014 x11; both equal the PE/COFF spec values;
  the delinker's x86 builder emits no other type.
- Reloc identity (XBE field at the site vs symbol VA + in-place addend): agree 14, mismatch 0, unresolved 0, refused 0.
  S for 11 from T5 `symbols.jsonl`; S for 3 DIR32 (Ghidra dynamic `DAT_`/`PTR_` labels, absent from the T5 export)
  from the label's address suffix, counted separately.
- Sites outside a `proto_map.mask_proc` decoded field: 0.
- Negative control: one addend bit flipped in a scratch copy reports mismatch 1.

## objdiff read
- `objdiff-cli diff -1 <obj> -2 <obj> FUN_00011800 -o derived/delink/retail-2953b782/0x00011800.objdiff.json
  --format json` rc 0, `match_percent` 100.0 (self-diff: proves objdiff parses the object, never match evidence).

## Unit delink (T11)
- T12.1 (2026-10-25): `--fresh` writes its post-ApplySymbols export to `derived/ghidra/retail-<sha8>/applied/`; before,
  it overwrote T5's `symbols.jsonl` (xbsymdb 296 -> 79, xtlid 186 -> 76) and so changed xdk_report's label counts.
- Commands: `PY tools/delink.py --fresh` (run.sh `t11_fresh`), `PY tools/delink.py --repro` (`t11_repro`),
  `PY tools/byte_compare.py --self-check` (`t11_selfcheck`); Ghidra 12.1.4, ghidra-delinker-extension v0.9.2,
  objdiff-cli 3.8.3.
- ApplySymbols (config/retail/symbols.txt, 4845 lines): renamed 4096, labelled 749, failed 0; same-name labels
  removed 6; duplicate function names reset 28 (13 thunks -> `thunk_<name>`, 15 ANALYSIS names -> `FUN_<va>`).
- Units `delink = true` 143; objects 143 in `derived/delink/retail-2953b782/`; 4 analyzeHeadless export calls
  (cmd.exe line cap).
- hashes.txt sha256 `14b51ab6c4d47963b2821ac2b63104c5640cd1c768c5005a89d9cf75c684e6fe`: repro run 1 and run 2
  byte-equal (also the preceding --fresh run: 3/3). No per-run COFF header field varied; no determinism fix needed.
- Runtimes per run: ingest+apply 102.8 / 106.2 s, export 62.2 / 64.0 s, objdiff 0.1 s; total 165.0 / 170.3 s.
- objdiff `report generate` (target-only units): rc 0, 143/143 units, total_functions 1983, total_code 317803
  (a read check, never match evidence, D81).
- byte_compare self-check: functions 1733, match 1733, provisional 0, mismatch 0, no-table-VA 0; relocs resolved
  12463, masked 0; controls 3/3; rc 0.
- Fixes found on the way (generator step, objects never edited):
  - Export failure "Failed to export" for BeepFile/SwampGeometry (+ xbresource): duplicate global names in one object
    (ANALYSIS name on 3 functions; thunks showing their target's name) -> duplicate-name reset in ApplySymbols.
  - First self-check: 1720/1733 match, 12 mismatch + 1 provisional, all calls to Ghidra thunk stubs relocated
    against the thunk target (delinker option `Redirect thunks to targets`, default true); set false -> 1733/1733.
- T3.c2 (2.1, 2026-10-26): `_add_rel32` on scratch copies of the retail-2953b782 cut: Globals added 1 (`.text+0x78`
  REL32 -> FUN_00125f98, = the T3.c1 trim-off entry, .text bytes equal), SwampLoader 70 of census 70, all 143 units
  807 in 97 objects (= relocscan's 807; section-symbol 2); coff_info --check-xbe 0 mismatch 0 unresolved on each.
- T5 (2.1, 2026-10-26): `--fresh` runs `_add_rel32` on all 143 delink units (`--rel32` removed). run.sh `t5_fresh`:
  `rel32: added 807 in 97 objects (section-symbol 2)`, total 129.8 s; hashes.txt sha256
  `4675c2f38ae53643bd2d239fcfea1a785b798b9ceda118b5387c3a5c50323d33`, `t5_repro` run 1 = run 2 = that (3/3).
- `t5_relocscan`: functions>=64B 928 reloc-free 34 explained 34 unexplained 0, partial 313; `intra-section rel32: 0
  unrelocated of 807; relocated inside own extent 0`; Ghidra body ends early 18 (no Ghidra function at entry 1); rc 0.
- `t5_selfcheck`: objs 143, 1893/1893 match, provisional 0, mismatch 0, relocs resolved 14217, controls 3/3; rc 0.
- candidates c0035 now reads the cut's .text relocs: c0035 fails 422 -> 0; pass 244 -> 287 of pool 1148 (+43 = the
  43 rows that failed only c0035). Negative control (scratch copy, one REL32 removed from RegionTrigger.obj .text+0x1598):
  `??0RegionManager@@QAE@XZ` c0035 fails `1 intra-unit rel site(s) without a target reloc`, 1 verdict changed; object
  edited after hashes.txt and cut.complete deleted both refuse.
- `coff_info --check-xbe` on SwampRenderer/SwitchSafe/FrontEndUI.obj: mismatch 0 unresolved 0 (agree 109/62/389);
  SwampRenderer rc 1 on 9 off-field DIR32 sites (switch table caseD_* x8, DAT_80000000), Ghidra's own, not REL32.
- `t5_gate` 1 passed 0 failed of 1; match_one `?BackupState@GameOptions@@QAEXXZ` objdiff 100.0, bytes match.
