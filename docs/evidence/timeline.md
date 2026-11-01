# Build timeline and prototype pair validation (T7, phase 1.2)

Facts only, read 2026-10-25 from the inputs located by `config/local_paths.json`. All times are the stored
`time_t` values rendered as UTC; the build machine's time zone is unknown. Retail XBE is the only oracle;
everything below about the prototype is reference.

## Pair validation (`tools/pdb_info.py <pdb> --exe <exe>`)
- `default.exe` / `default.pdb`: VALID, exit 0. RSDS GUID {94D1F446-0446-4A45-90BD-5AEC1281810A} age 6 ==
  PDB info GUID, DBI age 6. RSDS pdb path `d:\beep\Apps\Vince\Xbox\OfficialRelease\default.pdb`.
- `Vince_qa.exe` / `Vince_qa.pdb`: VALID, exit 0. RSDS GUID {34D77561-2767-4959-821B-CD71451E2544} age 8 ==
  PDB info GUID, DBI age 8. RSDS pdb path `d:\beep\Apps\Vince\Xbox\QARelease\Vince_qa.pdb`.
- Crossed: `default.exe` / `Vince_qa.pdb` exit 1 (guid, age 8 vs 6); `Vince_qa.exe` / `default.pdb` exit 1
  (guid, age 6 vs 8).
- Cross-check: `llvm-readobj --coff-debug-directory` gives the same RSDS GUID/age/path for both exes;
  `llvm-pdbutil dump --summary` reads both VC 7.x PDBs and gives the same signature, age and GUID as `pdb_info`.
- Both pairs validated: the prototype symbols of both builds may be used by later tasks (as reference, never as
  match evidence).

## PDB facts
| PDB | Info signature (source: info stream `Signature`) | as time | Age | Streams | Modules / procs / publics |
|---|---|---|---|---|---|
| `default.pdb` | 1057751319 (0x3F0C0117) | 2003-07-09 11:48:39 | 6 | 701 | 689 / 12513 / 21806 |
| `Vince_qa.pdb` | 1057750745 (0x3F0BFED9) | 2003-07-09 11:39:05 | 8 | 703 | 691 / 12834 / 22241 |
| `vc70.pdb` | 1057751303 (0x3F0C0107) | 2003-07-09 11:48:23 | 5 | 6 | DBI absent: no modules, no compile records |
- The signature is the PDB's creation time, kept across relinks (age counts the relinks): both PDBs were created
  six days before the final link.
- Both linked PDBs: info version 20000404, DBI version header 19990903, DBI build raw 0x38a0 (old format),
  `PdbDllVersion` 3077, rbld 0, machine i386 (source: DBI header). The meaning of 3077 here (mspdb DLL build) is
  unverified for VC7 (PHASE_PLAN Interfaces "PDB 7.0"); recorded, not interpreted.
- `vc70.pdb` (compiler /Fd PDB): block size 4096, 277 blocks, 6 streams: 0 old directory (1096 B), 1 PDB info
  (71 B), 2 TPI (1,045,752 B), 3 DBI (0 B), 4 IPI (25 B), 5 unnamed (34,978 B). `llvm-pdbutil`: has types, no debug
  info, no globals, no publics. Types only. Its signature is 16 s before `default.pdb`'s, consistent with the
  OfficialRelease build directory, not proven.

## Executables and XBEs
| Field (source) | Prototype | Retail |
|---|---|---|
| `Vince_qa.exe` PE TimeDateStamp (COFF header) | 0x3F14610E 2003-07-15 20:16:14 | n/a |
| `default.exe` PE TimeDateStamp (COFF header) | 0x3F146348 2003-07-15 20:25:44 | n/a |
| XBE header timeDate (0x114) | 0x3F14635F 2003-07-15 20:26:07 | 0x3F4447F7 2003-08-21 04:17:59 |
| XBE peTimeDate (0x148) | 0x3F146348 2003-07-15 20:25:44, equal to `default.exe`'s | 0x3F4447E0 2003-08-21 04:17:36 |
| Certificate timeDate (cert +0x04) | 0x3F14635F 2003-07-15 20:26:07 | 0x3F4D0C10 2003-08-27 19:52:48 |
| Certificate version (cert +0xAC) | 0x0 | 0x1 |
| Certificate titleId (cert +0x08) | 0x4D53005F | 0x4D53005F |
| Debug pathname (0x14C) | `d:\beep\Apps\Vince\Xbox\OfficialRelease\default.exe` | same |
| Debug filename (0x150) / unicode (0x154) | `default.exe` / `default.exe` | same |
- The preview `default.xbe` (prototype disc) was imaged from this `default.exe`: its peTimeDate equals the exe's
  TimeDateStamp, and its header timeDate is 23 s after the link. Retail was also built in OfficialRelease as
  `default.exe`.

## Library tables (XBE library versions, `xbe_info.py` record name, version, flags)
| # | Prototype | Retail |
|---|---|---|
| 0 | XAPILIB 1.0.5558 0x4001 | XAPILIB 1.0.5558 0x4001 |
| 1 | XBOXKRNL 1.0.5558 0x4001 | XBOXKRNL 1.0.5558 0x4001 |
| 2 | LIBCP 1.0.5558 0x4001 | LIBCP 1.0.5558 0x4001 |
| 3 | LIBC 1.0.5558 0x4001 | LIBC 1.0.5558 0x4001 |
| 4 | D3D8 1.0.5558 0x4001 | D3D8 1.0.5558 0x4001 |
| 5 | D3DX8 1.0.5558 0x4001 | D3DX8 1.0.5558 0x4001 |
| 6 | XGRAPHC 1.0.5558 0x4001 | XGRAPHC 1.0.5558 0x4001 |
| 7 | DSOUND 1.0.5558 0x4001 | DSOUND 1.0.5558 0x4001 |
| 8 | XMV 1.0.5558 0x4001 | XMV 1.0.5558 0x4001 |
- Flags 0x4001 = qfe 1, approved 2, debug 0. Identical tables: same XDK 5558 libraries, same order, prototype and
  retail. Libraries only; says nothing about the compiler.

## Order of the builds (by link time)
1. Prototype `Vince_qa.exe`, 2003-07-15 20:16:14 (PE TimeDateStamp).
2. Prototype `default.exe`, 2003-07-15 20:25:44 (PE TimeDateStamp): +570 s (0.007 days) after `Vince_qa`.
3. Preview `default.xbe`, 2003-07-15 20:26:07 (XBE timeDate): +23 s after `default.exe`, imaged from it.
4. Retail `default.xbe`, link 2003-08-21 04:17:36 (peTimeDate), image 04:17:59 (timeDate): 36.33 days
   (3,138,712 s) after the prototype `default.exe` link; certificate signed 2003-08-27 19:52:48, 6.65 days after
   the retail link.
- Note: the plan listed prototype `default` before `Vince_qa`; the stamps put `Vince_qa` first, by 9.5 minutes.
  Both PDBs were first created on 2003-07-09, `Vince_qa.pdb` 9.6 minutes before `default.pdb`.
