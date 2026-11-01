# Toolchain evidence (T11, phase 1.2)

Facts and judgments, from runs on 2026-10-25. Every conclusion carries a tag and its source:
- `observed`: read in this session from the named artifact's bytes by the named tool, command and field.
- `inferred`: follows from observed facts through a stated step that is not itself proven.
- `unknown`: no record answers it.
Prototype files (`<prototype_dir>/default.exe|pdb`, `Vince_qa.exe|pdb`) are reference only. Retail USA `default.xbe`
(sha1 `2953b78229018a34e7f53e85bd34f124d51ef210`) is the only oracle. It is an XBE: no Rich header, no PDB, no
PE optional header. So nothing about retail's compiler is `observed`; retail compiler conclusions are `inferred`
through the T9 identical share (`docs/evidence/correspondence.md`). Facts read from the prototype are never relabelled
as retail facts.

A matching test compile proves compatibility, not identity.

## Commands (all exit 0)
- `PY tools/pe_info.py <prototype_dir>/default.exe` and `.../Vince_qa.exe`: COFF/optional header, decoded Rich.
- `PY tools/pdb_info.py <prototype_dir>/default.pdb --exe .../default.exe` (exit 0, pair VALID), and
  `PY tools/pdb_info.py <pdb> --json` for both PDBs, counted by a scratch script that was not kept (counts below;
  regenerate with the same `--json` runs). Grouping: module `obj` path not a `.lib` = direct
  object; `.lib` under `Microsoft Xbox SDK` = XDK library; any other `.lib` = studio library (BeepLib, VinceLib,
  Swamp, scripts).
- `PY tools/xbe_info.py <retail default.xbe>` and `<prototype_dir>/default.xbe`: library version tables.
- `PY tools/pdb_info.py --self-test` (exit 0), and `PY tools/msvc_fixtures.py --out DIR` then
  `PY tools/pe_info.py DIR/{plain,ltcg}/fixture.exe`: our local fixture builds (cl 19.51.36260).
- Scratch MSF walk of `default.pdb` (not kept): it opens each module symbol stream raw and checks the `S_COMPILE2`
  record for zero-terminated strings after the version string.

## Module totals (pdb_info `modules`)
| | default.pdb | Vince_qa.pdb |
|---|---|---|
| modules | 689 | 691 |
| direct objects (SwampD3D.obj, Init.obj) | 2 | 2 |
| studio library modules | 304 | 307 |
| XDK library modules | 382 | 381 |
| `* Linker *` | 1 | 1 |
| `S_COMPILE2` records | 686 | 688 |
| compile record `missing` | 3 | 3 |
- `observed`: every compile record is `S_COMPILE2` (0x1116); there are no `S_COMPILE`, `S_COMPILE3` or `_ST` records.
  The 3 `missing` are the same in both PDBs: 2 OLDNAMES.lib alias modules (`stricmp.obj`, `strnicmp.obj`, which have
  no symbol stream) and `* Linker *`. Game code is 306 (default) and 309 (QA) modules.

## XDK build
- `observed`, retail: 9 libraries, all 1.0.5558 with flags 0x4001 (XAPILIB, XBOXKRNL, LIBCP, LIBC, D3D8, D3DX8,
  XGRAPHC, DSOUND, XMV). Source: `xbe_info.py`, "library versions" records. These are read from the retail bytes.
- `observed`, prototype: the preview `default.xbe` table is the same, entry for entry and in the same order (T7,
  `docs/evidence/timeline.md`; re-read in this task).
- `observed`: in both PDBs the XDK library objects come from `C:\Program Files\Microsoft Xbox SDK\Xbox\Lib\*.lib`.
  Their `S_OBJNAME` sits under `D:\xboxfre-jun03\` for 379 of 382 modules (default) and 377 of 381 (QA). The rest are
  2 OLDNAMES modules and the import modules `xboxkrnl.exe` (plus `xbdm.dll` in QA). Source: pdb_info `objname`.
- The XDK 5558 libraries are confirmed for both builds. The library table says nothing about the compiler.

## cl build per prototype exe
| Record (source) | default.exe | Vince_qa.exe |
|---|---|---|
| Rich Utc1310_C, prodid 0x5f build 3077 (pe_info Rich) | count 200 | count 201 |
| Rich Utc1310_CPP, prodid 0x60 build 3077 | count 443 | count 446 |
| Rich Masm 7.1, prodid 0x0f build 3077 | count 42 | count 39 |
| compile records lang c, fe/be 13.10.3077 (pdb_info `compile.fe/be`) | 200 | 201 |
| compile records lang c++, fe/be 13.10.3077 | 443 | 446 |
| compile records lang masm, be 7.10.3077 (fe 0.0.0) | 42 | 39 |
- `observed`: in both exes the two records agree exactly, count for count. Every C/C++ compile record in each PDB
  says front end and back end 13.10.3077, and the Rich C/C++ entries are all build 3077, with counts equal to the
  record counts per language. Agreement, no conflict.
- `observed`: this holds for the game code and for the XDK and CRT library objects alike. Every compiled object in
  both links came from cl 13.10.3077.
- `observed`: the other Rich entries are Import0 (158 / 161), Implib 7.1 build 3077 (3 / 5) and prodid 0x0c build
  7291, count 2 in both. Per R1.2-006, prodid 0x0c is AliasObj60, and richprint marks build 7291 "VS6 OLDNAMES.LIB".
  That fits the 2 OLDNAMES modules without a compile record.

## Link build
- `observed`: Rich Linker 7.1 is prodid 0x5a build 3077, count 1 in both exes (pe_info Rich). The PE optional header
  linker version is 7.10 in both (pe_info `linker`). DBI `PdbDllVersion` is 3077 with rbld 0 in both PDBs
  (pdb_info `dbi.pdbDllVersion`). The module `xboxkrnl.exe` (import lib) carries a LINK record with be 7.10.3077.
- `observed`: all three sources agree on LINK 7.10.3077. That `PdbDllVersion` is the mspdb DLL build is `inferred`:
  the field's meaning for VC7 is unsourced (PHASE_PLAN Interfaces "PDB 7.0"). Its value matches the other two.

## Flags from the compile records
The `S_COMPILE2` flags dword: bits 0-7 hold the language; bit 8 fEC; bit 9 fNoDbgInfo; bit 10 fLTCG; bit 11
fNoDataAlign; bit 12 fManagedPresent; bit 13 fSecurityChecks; bit 14 fHotPatch; bit 15 fCVTCIL; bit 16 fMSILModule.
These positions come from the modern `cvinfo.h` field order (R1.2-004, R1.2-006). No VS2003-era header was found.
- `observed`, default / QA: flags 0x0 in 643 / 648 records (every C/C++ record but one, plus the LINK import records 1 / 2); noDbgInfo (0x200) in 42 / 39,
  every MASM module of the XDK libraries; securityChecks (0x2000) in 1 / 1, `LocString.obj` (LocString.lib, studio
  library). fEC, fLTCG, fNoDataAlign, fManagedPresent, fHotPatch, fCVTCIL and fMSILModule are set in 0 modules.
- `inferred`: the game code was built without /GS, except LocString (305 of 306 game modules, default). The step
  assumes cl 13.10 sets bit 13 for /GS. That one module does carry it, so 13.10 does write flag bits above the
  language.
- `observed`, CPU field `compile.machine`, default / QA: pentium pro 643 / 647 (all game modules), pentium 21 / 18,
  80386 17 / 18, 0x1 5 / 5 (XDK MASM). Which /G switch this reflects (/G6 or the /GB blend) is `unknown`.
- `observed`: no `S_COMPILE2` record carries strings after its version string. Source: the scratch MSF walk, 686
  of 686 records in `default.pdb`. So the records hold no command line. /O level, /Oy, /Gy, /Ob, /EH, /Zp,
  /arch-type switches and every other switch beyond the bits above are `unknown` from the records.

## Flags from the compiler pin (1.4 T9, 2026-10-25)
Build: cl 13.10.3077 from `msvc71_dir` (pins pass, `check_env`), `cl /nologo /c <flags>`, byte-compared to the retail
`default.xbe` (sha1 pin) by `byte_compare.py` via `PY tools/compiler_pin.py`: 5 matched of 5 (match rate 5/5), every
entry 0 masked relocs; `--control`: 5 of 5 flipped flags change the bytes. Entries and levers: `docs/evidence/compiler_pin.md`.
- `observed`, retail: /O1 /Oy for the game code tested: EvilVince.obj (2 fns), FlourSack.obj, LocString.obj,
  JaniceCutscenes.obj. /O1 vs /O2 is pinned by `_Uninit_fill_n` and `SetState` (/O2 changes size); /Oy by std::fill,
  LocString_GetStringID and SetState (/Oy- changes bytes); /Ob1 (the /O1 default) by FlourSackMain dtor and SetState
  (/Ob0 changes bytes; /Ob2 gives the same bytes there, so /Ob1 vs /Ob2 is not pinned).
- `observed`, per unit (a finding, not a failure): /EH differs. EvilVince.obj: /EHa (`/EHsc`, `/EHs`, `/GX` drop the
  try-exit state store; C0030). JaniceCutscenes.obj: no /EH (no EH frame although every case builds a basic_string
  temporary; `/EHa` and `/EHsc` grow the function).
- `unknown` still: /GS on LocString.obj (no local buffer in the tested fn: same bytes with /GS), /G5-/G7 (same bytes in
  SetState), /Gy, /Zp beyond the one `#pragma pack(1)` struct, /GR.

## /GL and /LTCG
- `observed`, default: fLTCG 1 in 0 modules, fLTCG 0 in 686, missing in 3 (`unknown`), of 689 modules.
- `observed`, Vince_qa: fLTCG 1 in 0, fLTCG 0 in 688, missing in 3 (`unknown`), of 691.
- `observed`: the Rich header has no Utc1310_LTCG_C (0x63) or Utc1310_LTCG_CPP (0x64) entries. These product ids
  exist in the richprint table (R1.2-006). All C/C++ objects are counted under the plain 0x5f/0x60.
- `observed`, local control (cl 19.51.36260, our fixture, `pdb_info --self-test`, pe_info on the fixtures): the
  `/GL` build gives `S_COMPILE3` fLTCG 1 (flags 0x2400) on both fixture modules; the plain build gives fLTCG 0
  (0x2000). In the Rich header, the /GL objects move from Utc1400_C (0x104) to Utc1400_LTCG_C (0x108), count 2.
- The T6 fLTCG reader was proven on a local /GL compile: yes, `observed`, on cl 19.51 `S_COMPILE3` (above) and on
  cl 13.10 `S_COMPILE2` (next line).
- `observed`, pinned control (1.4 T7, 2026-10-25): cl 13.10.3077 and LINK 7.10.3077 from `msvc71_dir` (pins pass,
  `check_env.py` exit 0), the 1.2 fixtures `tests/fixtures/msvc/fix_c.c fix_cpp.cpp`. Commands (hand-run, scratch not kept; recipe C0025):
  `cl /c /Zi /O2 /GL` then `<msvc71_dir>\link.exe /DEBUG /LTCG` (LINK by full path), and the control
  `cl /c /Zi /O2` then `link /DEBUG`; read with `pdb_info.py <pdb> --exe <exe> --json` and `pe_info.py <exe>`.
  /GL: both fixture modules `S_COMPILE2` fLTCG 1, flags 0x400 (`ltcg`), language read `c++` for fix_c.obj too;
  cl skips "Generating Code" and LINK prints "Generating code". Control: fLTCG 0, flags 0x0, languages c / c++.
  Rich: /GL adds prodid 0x64 build 3077 count 2 (both modules, C and C++ under one id, as C0007 on 19.51) and
  0x5f/0x60 drop from 37/3 to 36/2; no 0x63. So cl 13.10.3077 marks /GL in both signals.
- `observed`, prototype: /GL was off for every compiled module of both exes, so there was no /LTCG code generation.
  Was `inferred`. Two signals agree, each now proven on 13.10.3077 (1.4 T7, above): fLTCG clear in all compile
  records, and no LTCG product id (0x63/0x64) in the Rich header with exact count agreement on 0x5f/0x60.
- `inferred`, retail: /GL off. The premise (the prototype, built by the same tree) is now `observed`; the retail
  step itself stays `inferred`, because the XBE carries no compile record or Rich header to show a mark (see
  "Retail compiler and linker"). The step is the T9 identical share. 5134 of 10388 prototype procs (49.42%, 797,606 B
  = 52.69%) are masked-byte identical to one place in retail, and 1260 more (12.13%) are near-identical. Cross-module
  inlining and the custom calling conventions of /LTCG would change bodies broadly, so whole-program codegen in
  retail is unlikely. Masked identity is not match evidence (D81, D40), and the share is a heuristic.

## /OPT:ICF
- `observed`, prototype: 431 of 10388 proc VAs in `default.pdb` carry several proc names, and 612 VAs (procs and
  publics) carry several names (T10, `docs/evidence/duplicates.md` count 4).
- `observed`, prototype: identical-function folding (/OPT:ICF) was on in the prototype link. Source: 1.3 T8
  (`phase-ends/phase-1.3/tasks/T8.md`, C0018): 1674 of 12513 `default.pdb` procs (1766 of 12834 in `Vince_qa.pdb`)
  sit inside exactly one other module's section contribution and in no contribution of their own. Was `inferred`.
- `inferred`, retail: `/OPT:ICF` in the retail link. Source: T10's fold candidates. There are 14 identical-unique
  retail VAs and 18 mixed ones where two or more prototype procs, each with exactly one retail hit, land on one retail
  body (`dup_report.py`, count 3). The same evidence fits one of the two procs being removed or changed in retail.
  The 2085 retail duplicates kept at masked level do not show ICF off: ICF folds only bodies that are equal with
  their relocations.

## Retail compiler and linker
- `inferred`: retail was compiled by cl 13.10.3077 and linked by LINK 7.10.3077. It is the same build tree
  (debug path `d:\beep\Apps\Vince\Xbox\OfficialRelease\default.exe` in both XBEs, T7) and the same XDK 5558 table
  (`observed`). 36.3 days separate the links. The T9 identical share is the codegen step. None of it is read from a
  retail compiler record, because retail has none.

## The cl 13.10.3077 lead
- Confirmed for the prototype: `observed` in both exes from two independent records, the Rich comp.ids and the PDB
  compile records. The 1.1 "developer recollection" lead is no longer unverified for the prototype. For the same
  builds the records refute 13.00.9466 (VC 7.0) and the other 13.10 builds (3052, 4035, 6030) (R1.2-002 (c), R0-005).
- Retail: still `inferred`, not observed (see above).
- Which product shipped 13.10.3077 to the studio is `unknown`: VS .NET 2003 or Toolkit 2003 v1.01 (R0-005). The
  `vc70.pdb` name is not evidence for 13.00 (Context, GENERATION_PLAN.md:33).
- The lead is now a target, not a proof. A future 13.10.3077 compile that matches retail bytes shows compatibility,
  not identity.

## Acquired compiler (1.4 T11, 2026-10-25)
- `observed`: VS .NET 2003 Professional RTM (developer's ISOs) ships cl 13.10.3077, LINK 7.10.3077, optimizing
  (`docs/ops/tools.md` "Matching compiler"). The developer's Toolkit 2003 installer is v1.0, 13.10.3052, not used.
- Still open: retail's own compiler stays `inferred`; flag values (/O, /Oy, /G, /GS) stay unknown until 1.4 tests;
  /GL is settled for the prototype (1.4 T7, "/GL and /LTCG").

## For the close
- /GL answer: no. Prototype `observed` (1.4 T7; was `inferred`): fLTCG 0 in 686/689 and 688/691 modules, the 3
  missing per PDB are `unknown` (alias objects and the linker module), the Rich header has no LTCG product id, and
  cl 13.10.3077 /GL sets both marks (fLTCG 1, Rich 0x64). Retail stays `inferred` through the T9 identical share.
- REPLAN lead: not triggered. Checked 1.4 T7: the cl 13.10.3077 /GL test compile shows fLTCG 1 and Rich 0x64,
  not fLTCG 0 or 0x5f/0x60. The trigger was: a cl 13.10.3077 /GL test compile shows fLTCG 0 or Rich 0x5f/0x60 for
  /GL objects. Then the two signals above mean
  nothing, and /GL is `unknown`. At that point Claude brings the developer the Halo 2 lead (GENERATION_PLAN.md:9;
  R0-027, R0-028, R0-029): the `docs/DECOMPILING.md` discipline for an /GL /LTCG target, and the `tools/check.py`
  design, a per-function score against retail instead of object-level matching.
- Cheapest check that would turn `inferred` into `observed`: done, 1.4 T7 (commands and result in "/GL and /LTCG").
