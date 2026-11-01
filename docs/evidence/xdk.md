# XDK library identification (T9, phase 1.3)

Run 2026-10-25, `PY tools/xdk_report.py` (exit 0, all sum checks ok; two runs, outputs and log identical). Retail USA
`default.xbe` sha1 `2953b78229018a34e7f53e85bd34f124d51ef210`. Lists: `derived/xdk/retail-2953b782/` (untracked).
Counts, names and addresses only (D46). Status: `observed` from the inputs below; mapping rows are proto_map's
`unverified` matches.

## Inputs
- `<prototype_dir>/default.pdb` modules/procs (`pdb_info.parse_pdb`), proc VA = default.exe image base + section VA + off.
- Retail XBE library table (`xbe_info.parse_xbe`, read at run time, 9): D3D8 D3DX8 DSOUND LIBC LIBCP XAPILIB XBOXKRNL
  XGRAPHC XMV. Sections from the same parse.
- `derived/proto_map/retail-xbe/functions.jsonl` (targetSha1 == pin), T5 `derived/ghidra/retail-2953b782/`
  (summary executableSHA256 == sha256 of the pinned XBE).

## Rule and module classes
- linker = module name `* Linker *`. xdk = (a) S_OBJNAME under `D:\xboxfre-jun03\` (case-insensitive) or (b) obj
  library basename minus `.lib` in the library table or (c) obj library under `...\Microsoft Xbox SDK\Xbox\Lib\`.
  Else game.
- Criterion counts (modules): a 379, b 377, c 382, linker 1. xdk combos: a+b+c 376 (dsound 20, d3dx8 60, d3d8 33,
  xapilib 77, xboxkrnl 1, xmv 11, libcp 1, LIBC 173); a+c 3 (xgraphics.lib: basename `xgraphics` != table `XGRAPHC`);
  b+c 1 (xboxkrnl.lib, no objname); c 2 (OLDNAMES.lib, no objname).
- game: 306 modules, 10868 procs, 1331068 B. xdk: 382 modules, 1645 procs, 277664 B. linker: 1 module, 0 procs.
  Sums 689 / 12513 / 1608732 == PDB totals.

## xdk procs -> retail (proto_map rows joined on protoVA)
- identical 1480 procs (261246 B), ambiguous 134 (5397 B), near-identical 0, absent 2 (137 B), excluded 29 (10884 B),
  no row 0. Sums 1645 / 277664 B == xdk population.

## Cross-check vs T5 labels
- pdb set = retail VAs of xdk procs (identical hits 1: targetVAs[0]; near: candidateVA): 1447 VAs.
- xbsymdb (293 label VAs): agree 229 (38019 B), pdb-only 1218 (212398 B), xbsymdb-only 64 (2056 B); 49 labels have
  no T5 function (bytes 0). Bytes = T5 function `size` at the VA.
- xtlid (186 label VAs): agree 132 (19197 B), pdb-only 1315 (231220 B), xtlid-only 54 (790 B); 19 labels no T5 function.
- Low agreement is a coverage gap of the label sources, not a contradiction: XbSymbolDatabase has no evidence for XDK
  5558 and labels only a subset; 1218 PDB-mapped xdk functions carry no xbsymdb label.

## Unidentified bucket
- Denominator: union of T5 function bodies `[bodyMin, bodyMax]` inside EXECUTABLE sections, per byte: 1291614 B
  (exec sections total 2326416 B; non-function remainder 1034802 B).
- Attribution: mapped retail ranges `[VA, VA+length)` of identical (5134 rows), near-identical (1260) and ambiguous
  rows whose modules (PDB procs at that protoVA) share one class (3640, all targetVAs; capped at proto_map HIT_CAP 32),
  then T5 functions whose entry carries an xbsymdb/xtlid label -> xdk-label.
- Totals: game 843712, xdk 256219, linker 0, mixed 2212, xdk-label 1012, unidentified 188459 B (943 ranges) of
  1291614 B.
- Per section (function B: game / xdk / mixed / xdk-label / unidentified; non-function):
  - .text 0x00011000 flags 0x16: 1081232: 843712 / 53715 / 2124 / 18 / 181663; 250316
  - D3D 0x00156160 flags 0x7: 57196: 0 / 55024 / 23 / 942 / 1207; 24768
  - D3DX 0x0016A1A0 flags 0x7: 37010: 0 / 36884 / 6 / 0 / 120; 12662
  - XGRPH 0x001763C0 flags 0x7: 7738: 0 / 7691 / 5 / 42 / 0; 1490
  - DSOUND 0x001787E0 flags 0x7: 42959: 0 / 42879 / 41 / 10 / 29; 83981
  - XMV 0x001977C0 flags 0x7: 35023: 0 / 32921 / 0 / 0 / 2102; 129357
  - XPP 0x001BF9E0 flags 0x7: 27128: 0 / 27105 / 13 / 0 / 10; 3532
  - .rdata 0x001C71C0 flags 0x6: 0; 247820
  - .data 0x002039E0 flags 0x7: 3328: all unidentified; 251820
  - DOLBY 0x00241EA0 flags 0x26: 0; 29056
- 2026-10-25 (1.5 T7): bucket figures now use the applied population (T5 `applied/functions.jsonl`, 8878 functions,
  superset of T5 checked); totals game 907697, xdk 261201, mixed 2241, xdk-label 1012, unidentified 188511 B of
  1360662 B (figures above are the pre-apply run); per-function classes in `functions.jsonl`.

## Section-name signal
- Match = exact, prefix (shortest library name wins) or abbreviation (same first letter, subsequence).
- D3D -> D3D8 (prefix), D3DX -> D3DX8 (prefix), XGRPH -> XGRAPHC (abbreviation), DSOUND exact, XMV exact: all
  dominant xdk. XPP: no table name, dominant xdk. .text: none, dominant game. .rdata, .data, DOLBY: no attributed bytes.
- `inferred`: library-named sections hold xdk code almost exclusively; XDK code also sits in .text (53715 B xdk).

## Applied population, 3.5 T8 (2026-10-30)
- Same HEAD 33314d4d, clone-3.5 vs working copy (before regen): `xdk_report.py` output identical, `functions (applied)`
  8929 (T5 pre-apply 8177); the 1.5/2.6 figures 8878 (WC) / 8880 (2.6 clone) are history: ApplySymbols has since
  created, grown, split and absorbed functions (3.2 T2), population 8929 on both sides.
- Applied export diff WC vs clone: 2 VAs, count equal: 0x00179435 WC `CDirectSoundStream_Pause` ANALYSIS, clone
  `?Pause@CDirectSoundStream@DirectSound@@QAGJK@Z` USER_DEFINED; 0x00179AD5 WC `IDirectSoundStream_Pause` ANALYSIS,
  clone `_IDirectSoundStream_Pause@8` USER_DEFINED. Cause: WC export (2026-10-29 23:21) older than the pin commit
  92c4095c (3.4 T12.c1, symbols_pin.txt, 2026-10-30 01:05) (C0160). Removed: WC `delink.py --fresh` regenerated,
  WC applied export, hashes.txt (sha256 236937a5...) and xdk summary.json now byte-equal to the scratch clone's.
- Three fresh rounds (scratch clone clone-3.5-t8, `ingest --fresh` then `delink.py --fresh` then `xdk_report.py`):
  every round T5 8177 functions (ANALYSIS 553, DEFAULT 7623, IMPORTED 1; symbols ANALYSIS 2095), applied 8929
  (ANALYSIS 54, DEFAULT 3426, USER_DEFINED 5449), FID canonicalised 167, hashes.txt sha256 236937a5...; exports and
  xdk summary.json byte-equal across rounds and to clone-3.5. No Ghidra nondeterminism seen (C0053/C0054).
- Figures (applied 8929): byClass game 5675 xdk 1538 mixed 72 xdk-label 10 unidentified 1634; bytes game 925032 xdk
  262892 mixed 2241 xdk-label 1012 linker 0 unidentified 198338 (951 ranges) of 1389515 function bytes; non-function
  936901; exec 2326416.
- FINDING: `xdk_report.py` exits 1 on both sides, `sum checks: FAILED population T5-only entries` (`7 == 0`; T5-only
  bytes `0 == 0 ok`). The 7 are DEFAULT T5 functions removed by ApplySymbols' absorb (3.2 T2, b7c19eac), bodies
  inside sized pins: 0x00015AC4, 0x00015CB8, 0x000163E3, 0x0007FB15, 0x00084AA0, 0x00084D13, 0x00143A18. The
  entry-superset check (xdk_report.py:270) predates absorb; the byte check holds. Not fixed here (tool outside T8).
- Fixed 3.6 T2 (b819d5b7, 2026-10-30): the entry check passes a T5-only entry only if its `[bodyMin, bodyMax]` lies inside
  one applied function; all 7 do; `sum checks: ok`, exit 0, 8929. `progress.py` runs again: denominators 7351 functions,
  1118722 bytes (counted + excluded = 1389515 above), replacing 7296 / 1091560 of the 8878 population.
