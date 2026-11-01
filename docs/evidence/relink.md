# Relink census (phase 2.3 T1, 2026-10-26)

Retail XBE sha1 `2953b78229018a34e7f53e85bd34f124d51ef210` (pin `config/check.retail_xbe.sha`). Tool `tools/relink.py census`;
usage `docs/ops/tools.md` "relink". Output `derived/relink/retail-2953b782/census.json` (gitignored; addresses, sizes,
classes, names only, D46).

## Method
- Expected regions come only from the XBE headers (`xbe_info.parse_xbe`): `header` [baseAddr, baseAddr+sizeOfHeaders),
  then each section in table order: raw [va, va+rawSize), plus [va+rawSize, va+vsize) when vsize > rawSize.
- One check for every mode: each range must have positive length, one of the 9 classes, and lie inside one region;
  per region, assigned raw bytes == rawSize, assigned total == vsize, 0 overlaps, 0 unassigned bytes.
- Range sources: the default list (below), `--objects F` (TAB lines `start end class name`), or a negative control.

## Class rules (default list)
- `header`: the header region.
- `.text`: split by `config/retail/units.toml` units whose text_start/text_end lie inside .text: delink=true ->
  `pinned unit`, else `unpinned unit`; holes between them -> `gap` (gap walk confined to .text).
- `XDK section`: D3D, D3DX, XGRPH, DSOUND, XMV, XPP, DOLBY, one raw range each.
- `.rdata`, `.data`: one raw range each. `inserted file`: sections flagged INSERTED_FILE.
- `uninitialized tail`: [va+rawSize, va+vsize) of any section with vsize > rawSize.
- A section matching no rule -> exit 2. A unit crossing a region edge, outside every region, or inside a region other
  than .text / an XDK section -> exit 1.

## Finding: 87 units lie inside XDK sections
- R2.3-001 said units.toml has .text ranges only; it does not. 87 units (all kind=xdk, delink=false) have text ranges
  inside XDK sections: D3D 27, D3DX 11, XGRPH 2, DSOUND 16, XMV 10, XPP 21 (DOLBY 0). Each lies wholly inside one
  section; 0 cross a section edge or fall outside every section.
- Decision (T1.c2, option a): they are not ranges in the default list (the section stays one `XDK section` range);
  counted per section on stdout and listed in census.json `unitsInXdkSections` (name, start, end, section, delink) for T5.
- 163 units have no text range; counted as skipped.

## Green run (`PY tools/relink.py census`, exit 0)
- 570 ranges; .text: 143 pinned unit, 296 unpinned unit, 108 gap.
- Every region: assigned raw == rawSize, total == vsize, 0 overlaps, 0 unassigned:
  header 0x1000/0x1000; .text 0x14515C/0x14515C; D3D 0x10558/0x1402C; D3DX 0xC200/0xC208; XGRPH 0x240C/0x240C;
  DSOUND 0x1ED74/0x1EFDC; XMV 0x2820C/0x2821C; XPP 0x77C4/0x77C4; .rdata 0x3C804/0x3C80C; .data 0x2165C/0x3E4AC;
  DOLBY 0x716C/0x7180; $$XTIMAGE 0x2800; $$XSIMAGE 0x1000; sysfont 0x21800; loadtex 0x9000; vstr 0x76CB; .XTLID 0x5E8
  (rawSize/vsize; one value when equal).
- Totals: rawSize 0x24E083, vsize 0x26EC43, assignedRaw 0x24E083, assignedTotal 0x26EC43, overlaps 0, unassigned 0,
  flagged 0.

## Controls
- `--control overlap`: injected 0x00011000-0x00011010 over `Xbox\OfficialRelease\Init.obj` -> .text raw/total 0x14516C,
  1 overlap; exit 1.
- `--control drop`: removed `Init.obj` 0x00011000-0x000120C1 -> .text 0x10C1 unassigned; exit 1.
- `--objects F`, F = the 570 census.json ranges as TAB lines: exit 0. F minus one line (`BumperCarCutscenes.obj` at
  0x0004EAC8): .text 0x1012 unassigned; exit 1.

## XBE writer (T2)
Tool `tools/xbe_write.py` (usage `docs/ops/tools.md` "xbe_write"). Pairs: prototype `default.exe` (LINK 7.10, imageBase
0x400000, TimeDateStamp 0x3F146348) + `default.xbe` (peTimeDate 0x3F146348: one build, C0009); retail XBE alone (its
header holds the PE headers, below). Self-test (a) prototype: 16/16 digests, file sha1 `1496652c1701931264128b0f63d1d5c651dddd05`
reproduced; (b) retail round-trip: 16/16, sha1 = pin `2953b782...`. Blanked-pin control: with every recomputed field and
all linked section data zeroed in the prototype pin, the output still equals the real pin (byte-identical).
- Section map: PE sections minus INIT, .XBLD, .tls, .reloc = the XBE's 10 non-INSERTED_FILE sections, same names, same
  order (.text D3D D3DX XGRPH DSOUND XMV XPP .rdata .data DOLBY), both builds. INSERTED_FILE sections follow them in the table.
- VA packing: XBE VA = PE RVA + peBaseAddr, peBaseAddr = baseAddr + XBE sizeOfHeaders - PE SizeOfHeaders
  = 0x10000 + 0x1000 - 0x400 = 0x10C00 (both builds). PE section VAs are packed at 0x20 (SectionAlignment = FileAlignment
  = 0x20, VA == raw pointer), so the XBE linked VAs are 0x20-packed: .text 0x11000 ... DOLBY ends 0x242560 (proto),
  0x249020 (retail).
- XBE vsize = align4(PE VirtualSize): PE value already aligned on 9/10 (proto) and 8/10 (retail) sections; XGRPH
  0x240A -> 0x240C (both), retail .text 0x14515A -> 0x14515C. (max(vsize, rawSize) fits the same data; not distinguished.)
- Per-section rebase: one delta per section, all 10 equal: peBaseAddr - imageBase (0x10C00 - 0x400000). Proto .reloc:
  112304 bytes, 54130 HIGHLOW + 234 ABSOLUTE padding entries, 0 sites outside a section. After adding the delta at every
  site, 9/10 linked sections are byte-equal to the XBE; .rdata differs in 7 bytes, all in the TLS directory (next rule).
- .tls: 9-byte section, all zero; TLS directory (PE dir 9, .rdata +0x8A0) Start/End = 0x242820/0x242828 after rebase.
  imagebld writes Start = End = 0 and adds End-Start (8) to SizeOfZeroFill (0 -> 8). Retail: Start = End = 0 already in the
  XBE. tlsAddr = TLS dir RVA + peBaseAddr (proto 0x1C1C00, retail 0x1C7A60). A non-zero template is refused (exit 2).
- INIT (0x206 proto, 0x202 retail): the import directory (PE dir 1, 0x28 bytes: one xboxkrnl descriptor + null). Its
  FirstThunk = IAT dir (PE dir 12) = .rdata start; the IAT already holds ordinal thunks (0 byte diffs after rebase).
  Kernel thunk = IAT RVA + peBaseAddr (proto 0x1C1360, retail 0x1C71C0). INIT is not in the image.
- .XBLD (0x90 = 9 x 16): equal to the XBE library version table at header +0x778 (proto); not in the image.
- Entry = PE AddressOfEntryPoint + peBaseAddr (proto 0x1353 -> 0x11F53, retail 0x155F -> 0x1215F). Keys: prototype uses
  the DEBUG entry/thunk keys (0x94859D4B / 0xEFB1F152), retail the RETAIL keys (0xA8FC57AB / 0x5B6D40B6); the key kind is
  read from the pin.
- PE-header copy: XBE file [peBaseAddr-baseAddr, +PE SizeOfHeaders) = [0xC00, 0x1000) = the PE's first 0x400 bytes
  (0 diffs, proto). peImageSize = PE SizeOfImage (0x250960 proto), peTimeDate = TimeDateStamp, peChecksum = 0,
  peHeapReserve/Commit = PE 0x100000/0x1000. peStackCommit 0x40000 != PE stack commit 0x1000: from the pin (imagebld option).
- rawSize = align4(length of the section data with trailing zeros trimmed) (10/10 sections, both builds); e.g. proto .data
  vsize 0x3E3AC, rawSize 0x2163C (retail 0x3E4AC / 0x2165C): the zero tail becomes uninitialized, not file bytes.
- Raw offsets: header 0x1000, then each section at the next 4 KB boundary after the previous one's rawAddr+rawSize, linked
  sections in table order, then inserted files in the pin's raw order; padding is zero; file length = 4 KB-aligned end
  (proto 0x23F000 = 2355200).
- Inserted files: VA order = table order ($$XTIMAGE $$XSIMAGE sysfont loadtex vstr .XTLID), packed at 0x20 from the last
  linked section's end (proto vstr end 0x2671AB -> .XTLID 0x2671C0). Raw order differs: sysfont loadtex vstr $$XTIMAGE
  $$XSIMAGE .XTLID (the three PRELOAD files first). Their data and order come from the pin.
- From the pin, not derived: header region (signature, certificate, library table, debug strings, logo, section names and
  refcount slots, flags), baseAddr, sizeOfHeaders, initFlags, peStackCommit, sizeOfImage (proto 0x2577C0, retail 0x26ED00:
  not an end of any section; headers + sum of vsizes is 0xB5 / 0xBD short; rule unknown).

## One section relinked (T3)
Tools `tools/relink.py cut|link|check`, `tools/xbe_write.py write_section` (usage `docs/ops/tools.md` "relink").
Result: XGRPH relinked from the range-cut original; XBE digest equal to retail (`7292af35b92924c0335c24b9c8b52b78f3b4d4fd`),
va 0x1763C0, vsize 0x240C, rawSize 0x240C, all equal. Negative control: filler symbol moved +4 -> digest differs at 1 byte
offset (the one DIR32 to it).
- Cut: census range XGRPH 0x1763C0-0x1787CC -> DelinkProgram `/include-range 001763c0-001787cb` on the existing project
  (`-noanalysis -readOnly`), one call, ~9 s. `XGRPH.obj` 18429 B: section XGRPH 0x240C chars 0xE0000020 (Ghidra block name
  and PE flags carried), `.comment` 0x21 (LNK_INFO|LNK_REMOVE, dropped by LINK); 239 symbols; 8 relocations, all type 0x6
  (DIR32): 7 intra-section, 1 to the one external. No 0x14 (REL32): intra-range rel32 trimmed (C0058, fine: the section
  moves as one).
- Two cuts (`cut --repro`, `cut/` and `cut.repro/`): hashes.txt sha256 both
  `88068560f355332763cdf8be37dba153f39fbd9f32597f530ad75fc056efb788`; equal. Marker `cut.complete` as delink (C0057).
- Outward references (cut externals): XGRPH 1 (`DAT_00050001` -> .text; a constant Ghidra took for an address, value
  0x50001 reproduced through DIR32 + rebase); XPP 162 (.data 117, .rdata 36, .text 6, absolute 3). XGRPH picked: smallest
  outward surface.
- Externals' names are Ghidra's: `<prefix>_<hex8>` -> that VA; else `namespace::name` / `name` from
  `derived/ghidra/retail-<sha8>/applied/symbols.jsonl`, exactly one VA or refused (XPP: `XAPILIB::_SetLastError@4`).
- Filler mechanism (no absolute stubs: DIR32 to an absolute symbol gets no base reloc, REL32 is computed in PE VA space):
  Python writes `filler_pre.obj` (.text D3D D3DX) and `filler_post.obj` (DSOUND XMV XPP .rdata .data DOLBY), zero-filled,
  each section at the retail PE VirtualSize and PE characteristics of the XBE's PE-header copy (alignment bits replaced by
  ALIGN_4); each external defined at (target VA - XBE section VA) in its filler section; a target outside every linked
  section would be an absolute symbol (XGRPH 0, XPP 3). Entry `_relink_entry` = .text +0x115F (retail entry 0x1215F).
  Link order filler_pre, cut, filler_post -> LINK keeps first-appearance order: .text D3D D3DX XGRPH DSOUND XMV XPP
  .rdata .data DOLBY .reloc (retail order). Counts XGRPH: 2 filler objects, 9 filler sections, 2 filler symbols.
- LINK 7.10.3077 (`msvc71_dir`), flags, with the prototype PE (`pe_info.py default.exe --json`) as evidence:
  `/SUBSYSTEM:XBOX` (proto subsystem xbox; it fixes imageBase 0x400000 and 0x20 section/file alignment: output imageBase
  0x400000, align 0x20/0x20, OS 4.0, subsystem 1.0, characteristics 0x10E, stack/heap 0x100000/0x1000, all = proto);
  `/MACHINE:X86`; `/FIXED:NO` (proto has .reloc); `/INCREMENTAL:NO`, `/OPT:NOREF`, `/OPT:NOICF` (no thunks, no folding);
  `/NODEFAULTLIB` (no CRT pulled); `/ENTRY:relink_entry`; `/NOLOGO`, `/OUT`, `/MAP`. Never `/FORCE`. `/BASE:0x400000`
  and `/ALIGN:0x20` were tried: LINK warns LNK4075 "ignoring '/BASE' / '/ALIGN' due to '/SUBSYSTEM:XBOX'" (+ LNK4108), so
  they are not passed. Any LINK output line (unresolved, duplicate, warning) fails the link; the green link prints none.
- Linked PE: SizeOfHeaders 0x380 (proto 0x400: fewer sections), so .text RVA 0x380; peBaseAddr = 0x10000 + 0x1000 -
  0x380 = 0x10C80 and every linked section lands at its retail VA (checked by write_section). .reloc 0x20 B.
- write() refuses this PE ("pair is not one build": peTimeDate 0x3F4447E0 != the fresh TimeDateStamp; no INIT/.XBLD/.tls
  either): outside the section. `write_section(pe, pin, name)` shares write()'s `_section_map`, `_rebase` (HIGHLOW, one
  delta), `_trim`, `_digest` and adds one check: every linked PE section at its pin VA. Self-test stays 6/6.
- R2.3-004 (Ghidra reloc types 0x6/0x14 break a link): this cut carries 8 x 0x6, 0 x 0x14; LINK accepted it with no
  message and the digest is equal, so 0x6 does not break a link. 0x14 is untested by XGRPH (XPP carries 8 x 0x14 + 693 x 0x6;
  not linked in T3).
## Two real sections, ordered COMDAT originals (T4.c1)
Tools `tools/relink.py cut|link|check --section .text,.rdata` (usage `docs/ops/tools.md` "relink").
Result: `.text` and `.rdata` linked together from their cut originals, fillers for the other 9 linked sections; both XBE
digests equal retail: .text `5c465eefb6f78533f9a27d89f9fa3d087412517f` (va 0x11000 vsize/rawSize 0x14515C), .rdata
`286a6001237d807e5f9f5d6d1412e9cba0047494` (va 0x1C71C0 vsize 0x3C80C rawSize 0x3C804). XGRPH alone still equal
(`7292af35...`, cut re-made into `cut/XGRPH/`, hashes.txt sha256 `88068560...` = T3). Gate 24/24.
- Resolved in T4.c2 (RenameDuplicates, below); was the T4.c1 wall: `cut --section .text` with the shared project fails one range: DelinkProgram "Failed to export
  obj__i386__xcalcsig.obj_00017256.obj"; exporter message log: `Duplicate symbol name 'xboxkrnl.exe::thunk_XcSHAUpdate'
  (2 instances)`. The program's only duplicate function name (applied/functions.jsonl): thunks 0x000172B8 (in xcalcsig)
  and 0x0001786C (in gap@0001785A), both ANALYSIS; ApplySymbols.java:169-170 gives every duplicate thunk `thunk_<name>`,
  so two thunks to one import stay duplicates (C0021 trap). Diagnosis over all 547 ranges: 1 failure (18 calls, 4m23s).
  The green results below used a SCRATCH cut: same ranges, file names and marker, exported by a copy of DelinkProgram
  that first renames non-first duplicate functions in memory to `<name>_<va8>` (read-only project, not saved; here
  0x1786C -> `thunk_XcSHAUpdate_0001786C`); 547 objects, 16 calls, 4m19s, hashes.txt sha256 `f712d52b...`.
- Cut: .text = its 547 census ranges (units + gaps), one object each (`<unit>_<va8>.obj`), batched greedily under
  `delink.ARG_BUDGET`; .rdata 1 object 890471 B (23277 DIR32), ~11 s; XGRPH ~7 s.
- Link counts (.text,.rdata): 548 cut objects, 547 COMDATs, 2 filler objects (D3D..XPP; .data DOLBY). External names
  11289: defined by a linked object 3646, alias 6546, VA-resolved 1097 (definitions renamed 0, no name defined twice).
  Aliases placed: cut object 6201, filler 1440, absolute 3. LINK ~0.3 s; whole check ~1.4 s.
- Every external reloc site is checked against the retail field (DIR32 S = u32 - field; REL32 S = u32 + site + 4 -
  field). VA-resolved = no single VA by T3's name rules or sites disagreeing: 1097 names, each with one site VA, and
  1097/1097 end in `_<hex8>` equal to that VA (`PTR_?Open@..._001cdcf4`, `s_vinceLanded_=_%d;_001dec44`: GHIDRA_NAME
  misses names with `?@$;=%`). Caveat: a VA-resolved site reproduces retail by construction, so the digest alone does
  not test those 1097; the suffix agreement is the independent check. .rdata alone: 3 VA-resolved
  (`thunk_FUN_0010f7fa` etc.: the name rule gives the thunk's target, the field gives the thunk).
- COMDAT (post-processed at link time, `_comdat`): section chars | LNK_COMDAT, align bits -> ALIGN_1; section-symbol aux
  Length/NumberOfRelocations set, Number 0, Selection 1 (byte 14, C0032); external `__rl_<va8>` (type 0x20) inserted
  right after the section symbol; later symbol entries shifted (relocs are entry-based in the rewriter).
- Flags: `/NOLOGO /MACHINE:X86 /SUBSYSTEM:XBOX /FIXED:NO /INCREMENTAL:NO /OPT:REF /OPT:NOICF /NODEFAULTLIB
  /ENTRY:relink_entry /ORDER:@order.txt /INCLUDE:__rl_<va8> x547` + objects, all in `link.rsp`. LINK prints nothing.
  Trap: LINK 7.10 prepends `_` to /ORDER names: listing `__rl_00011000` gave LNK4037 `'___rl_00011000' does not exist`;
  order.txt lists `_rl_<va8>`. /INCLUDE takes the raw name.
- Size trap: census .text ends 2 bytes past the retail PE VirtualSize (XBE rawSize is 4-aligned): accepted within the
  4-byte alignment. .rdata cut 0x3C804 < PE VirtualSize: the last cut object is zero-padded to it (else write_section
  refuses "vsize 0x3C804 does not give XBE 0x3C80C").
- Controls (scratch): .text objects in reversed command-line order + /ORDER -> both equal. /ORDER governs: first two
  entries swapped -> .text 19337 / .rdata 5 differing offsets; /ORDER reversed -> 1301717 / 68534. But reversed objects
  with NO /ORDER (also with /INCLUDE reversed) still gave both equal; the map puts `__rl_00011000` at .text+0 there, so
  LINK 7.10 does not keep command-line order for these COMDATs without /ORDER either (cause untested; the names
  `__rl_<va8>` sort in VA order, a name sort would explain it).
## Substitution on one unit (T4.c2)
Tools `tools/relink.py cut` (RenameDuplicates pre-export) and `check --section .text --units Globals,Main
[--control no-order|noref]` (usage `docs/ops/tools.md` "relink"). Our objects `.run/2.3/build/{Globals,Main}.obj`
(`tools/build.py --src src --out .run/2.3/build`, /O1 /Oy), never edited.
- Real cut green: `tools/ghidra_scripts/RenameDuplicates.java` runs before DelinkProgram in each analyzeHeadless call
  (read-only project, in memory): `groups 1 renamed 1`, `xboxkrnl.exe::thunk_XcSHAUpdate at 0001786c ->
  ..._0001786C`. `cut --section .text` 547 objects, 16 calls, ~4.5 min, hashes.txt sha256
  `f712d52b58dad219045c10977a7e91518450f8cc2a3640766a300fb89ea15b61` = T4.c1's scratch cut. Re-cut `.rdata` 1 object,
  externals 2217 (as T4.c1), hashes.txt sha256 `7bdee313...`; XGRPH `88068560...` (= T3).
- Resolved in T4 (expert): `__except_handler3` pinned 0x00141900 in config/retail/symbols_pin.txt (evidence
  compiler_pin.md "main 0x000E0BB0 SEH callees"), our externals fall back to the pins after byname. Was the wall:
  `check --section .text --units Globals,Main` exited 2: `our Main.obj: external __except_handler3
  does not resolve by name (0 VAs in byname)`. Only Main.obj's `.sxdata` (LNK_INFO, 4 B, symbol index, no reloc) names
  it; LINK still needs it (scratch: Main.obj + stub, `LNK2001 unresolved external symbol __except_handler3`). No name
  table holds it (applied functions/symbols.jsonl, symbols.txt, symbols_pin.txt); `docs/evidence/compiler_pin.md:209-211`
  places it at retail 0x00141900 (prototype PDB public spacing, "not pinned: no game reloc needs it").
- Real run after the pin (T4 expert, `PY tools/relink.py check --section .text --units Globals,Main`, exit 0; the
  T4.c2 scratch injection of the same VA gave the same). Both digests equal retail: .text `5c465eefb6f78533f9a27d89f9fa3d087412517f`
  (va 0x11000 vsize/rawSize 0x14515C), .rdata `286a6001237d807e5f9f5d6d1412e9cba0047494` (va 0x1C71C0 vsize 0x3C80C
  rawSize 0x3C804). LINK prints nothing.
- Our symbols (XBE VA = map Rva+Base - imageBase + (retail .text VA - PE .text RVA)): `?BackupState@GameOptions@@QAEXXZ`
  0x00125F88, `?RestorePreviousState@GameOptions@@QAEXXZ` 0x00125FC3, `_main` 0x000E0BB0 (Main.obj), `$T318`
  0x001EC830 (no map line for `$` statics: from the linked DIR32 field at `_main`+3 minus our addend). The inline copy
  `?GetInput@Swamp@@QBEAAVSwampInput@@XZ` is absent (dropped by /OPT:REF).
- Holes 4 (3 .text rows, 1 .rdata: Main.obj `.rdata` 0xC, VA from `$T318` via symbols_pin.txt:45); cut objects split 3
  into 6 pieces (Globals 0x125F5F.., Main 0xE03D1.., .rdata); references retargeted 3 (field += offset), unreferenced
  hole names made static 5, aliases inside holes dropped 6; our externals resolved by name 7 (new aliases 1, the others
  already defined/aliased at the same VA). Totals: objects 551 (COMDAT 552 = 549 pieces + 3 ours), fillers 2, external
  names 11289: defined 3643, alias 6549, VA-resolved 1097.
- Order: `order.txt` = .text in VA order, our symbol in its hole's slot. A C++ decorated name is listed raw
  (`?BackupState@...`: no LNK4037, so LINK prepends no `_` to it); a `_` name loses its `_` (`_main` -> `main`).
  /INCLUDE x552 (raw names).
- Data placement: our `.rdata` is an associative COMDAT (Selection 5, to `_main`'s section), ALIGN_8; Main.obj sits
  between the two .rdata pieces on the command line and LINK placed it there. Two traps: (1) cut `.rdata` carried
  Ghidra block flags 0x60000020, so LINK made a second `.rdata` (DATA) for ours (0x40401040) -> every cut section's
  flags are set to the retail PE section's (0x40000040) at link time; (2) the piece after ours kept its alignment and
  got +4 pad (vsize 0x3C810) -> split pieces ALIGN_1.
- Controls (re-run on the real tool after the pin, each exit 1, same counts): `--control no-order` (no /ORDER, /INCLUDE kept): our functions at
  0x001560F8.. (end of .text), .text 461576 / .rdata 9845 differing offsets (first +0x64 / +0xCA8). `--control noref`
  (/OPT:NOREF, /ORDER kept): `?GetInput@Swamp@@...` kept at .text+0x14515C, .text vsize 0x145160 -> write_section
  refuses (`.text vsize 0x145160 does not give XBE 0x14515C`), counted FAIL; PE-to-PE vs the default link: .text and
  .rdata 0 differing bytes in common, .text +4 B at its end.
- /Gy (one-off check, T4.c2, script not kept: unit opt + `/Gy` vs the default build, non-debug sections compared): Globals.cpp and Main.cpp with `/O1 /Oy /Gy`: every image section (Globals
  .text 0x10, 0x18; Main .text 0x4, 0x3C, .rdata 0xC) equal to the default build's in name, size, bytes and reloc
  offsets/types (whole files differ outside them): /O1 already packages functions.
- Originals unchanged: `check --section .text,.rdata --originals` both equal (548 objects, defined 3646, alias 6546),
  `check --section XGRPH --originals` equal `7292af35...`.
## Decision (T4, binding): substitution route
- Ordering rung: (2) `/ORDER`. Every .text original is a NODUPLICATES COMDAT `__rl_<va8>` (link-time post-pass of the
  cut), our functions are cl's own COMDATs; order.txt lists all of .text in retail VA order. Rung (1) fails by
  construction for a partly matched unit (our matched functions sit adjacent in one object; `--control no-order`:
  461576 .text offsets differ); rung (3) is dead (our plain `.text` sorts before every `$` group).
- Data rung: (b) the covering original data object is split at our section's retail VA (from a symbol in it, by name;
  `$T318` 0x1EC830) and our object is placed between the pieces by command-line order; (a) for every external.
- Granularity: one cut object per census range, split at link time at matched-set [va, va+size); a new match is a
  matched-set row plus our object (the hole's piece is omitted, references into it retargeted), never a re-delink.
- LINK: `/OPT:REF` + `/INCLUDE` of every piece and matched symbol (replaces the plan's /OPT:NOREF: NOREF keeps
  unreferenced inline copies, `--control noref` fails), `/OPT:NOICF`, never /FORCE, any LINK line fails.
- opt: none changed. `/O1` already implies `/Gy` (COMDAT bit on every .text; `/O1 /Oy /Gy` build: image sections equal).
- Our externals resolve by name (byname, then symbols_pin.txt), never from a retail field.
## Whole image from originals (T5)
Tools `tools/relink.py cut [--repro]`, `link`, `check --originals` with no --section (usage `docs/ops/tools.md` "relink").
Result: every linked section relinked from its cut original in one LINK (zero fillers); the written XBE
`link/all/relinked.xbe` (2453504 B) has 16/16 section digests equal to retail and sha1
`2953b78229018a34e7f53e85bd34f124d51ef210` = the pin; two check runs, same sha1. LINK prints nothing.
- Cut: 10 sections, 556 objects (.text 547, others 1 each), 17 analyzeHeadless calls for .text, 321 s / 326 s for the two
  cuts. Per-section hashes.txt sha256: .text `f712d52b...` (= T4), XGRPH `88068560...` (= T3), .rdata `7bdee313...` (= T4),
  D3D `ea0ae172...`, D3DX `5a78a0e8...`, DSOUND `25ed8633...`, XMV `f2f52d6e...`, XPP `8d2b0a03...`, .data `ccecc545...`,
  DOLBY `d3c521a5...`. `cut/partition.tsv` (570 ranges: 556 cut objects + header + 7 uninitialized tails + 6 inserted
  files) sha256 `63fe1977...`; `census --objects cut/partition.tsv` exit 0 (every region exactly covered, flagged 0).
  Top `cut/hashes.txt` (11 lines) sha256 `94166909e72d9ad467523600b3f0d466cec140ef78c051d1ccf661ab0358b5d7`, both cuts
  equal; `cut/cut.complete` over it, written last.
- Link counts: 556 objects (COMDAT 547), fillers 0; external names 13388: defined by a linked object 3955, alias 8304,
  VA-resolved 1129 (definitions renamed 0); aliases placed: cut object 9258, absolute 175 (in a section-less
  `absolute.obj`), REL32 outside the image 1. LINK ~0.3 s; XBE write 0.6 s.
- Trap 1: an alias into a section's uninitialized tail (`?g_swamp@@3PAVSwamp@@A` 0x002369CC, .data bss) had no covering
  object: the last object is padded to the PE VirtualSize but its cover range stopped at the raw end -> the range now
  grows with the padding.
- Trap 2 (R2.3-004 lever, REL32): `.data.obj : error LNK2016: Absolute symbol 'SUB_209a9b1a' used as target of REL32
  relocation` (+ LNK1165). Ghidra took a .data value for a call target outside the image. Fix: a name with a REL32 site
  and a target outside every linked section is defined in the first cut object at (VA - its VA) mod 2^32 (REL32 is
  computed in PE VA space); DIR32-only outside targets stay absolute (no base reloc, field = value). With it, every
  REL32 cut in this image links and reproduces retail (XPP's 8 REL32 included: XPP digest equal).
- Writer (`_write_image`, relink.py; xbe_write.py unchanged): pin header region [0, sizeOfHeaders), each linked section
  from `write_section()` into its section header, raw layout and inserted files as `xbe_write.write()`. Our PE entry +
  pe_base == pin entry 0x1215F (checked).
- Info, .reloc: our dir 5 size 100524 (0x188AC) vs retail 113836 (0x1BCAC), -13312; our HIGHLOW 48109 vs 48555 DIR32 in
  the linked cut sections (-446; likely the DIR32 sites to the 175 absolute symbols, which get no base reloc: untested). Retail's HIGHLOW count is not in the XBE.
- Claim boundary: the section bytes (all 16 digests) come from the link; the header region (XBE header fields, entry,
  thunk, tls, peBaseAddr, peTimeDate, the PE-header copy at 0xC00) is the pin's. The link cannot carry Rich, INIT,
  .XBLD, .tls or the data directories, so the whole-file sha1 equality covers the section data, not a derived header.
## Every matched function substituted (T6)
Tool `tools/relink.py check` (no --section, no --originals) and `check --control one-function` (usage `docs/ops/tools.md`
"relink"). Runs 2.3 T6.c2, logs `t6c2_{check,control,originals,gate}.log` (phase scratch, not kept; results below).
Result: every matched function (18 units, 24 functions, `config/matched_set.toml`) built from our source and swapped into
the whole-image relink; `link/all=ours/relinked.xbe` (2453504 B) has 16/16 section digests equal to retail and sha1
`2953b78229018a34e7f53e85bd34f124d51ef210` = the pin; exit 0.
- Counts: our objects 18; data holes 1; unreferenced COMDAT data skipped 43; holes 25; cut objects split 19 into 34
  pieces; references retargeted 20; unreferenced hole names made static 5; hole aliases dropped 20; our externals
  resolved by name 61 (new aliases 4); cut definitions renamed off our unused inline copies 2; map 25/25 found. Link:
  571 objects (COMDAT 585 /INCLUDE), fillers 0; external names 13394: defined 3944, alias 8321, VA-resolved 1129;
  aliases placed: cut object 9259, absolute 175, REL32 outside the image 1. LINK 0.4 s, write 0.7 s.
- Digests (= retail): .text `5c465eef...`, D3D `71125b56...`, D3DX `87400ecd...`, XGRPH `7292af35...`, DSOUND
  `5016eb6a...`, XMV `f591c0f5...`, XPP `56f558f9...`, .rdata `286a6001...`, .data `9bd48836...`, DOLBY `c8c9142d...`,
  $$XTIMAGE `bb05b542...`, $$XSIMAGE `b8e0c2b7...`, sysfont `ffcbf120...`, loadtex `46d4d2d3...`, vstr `8857069c...`,
  .XTLID `c0fcd127...`.
- `$T318` linked 0x001EC830 = `symbols_pin.txt` 0x001EC830 (our Main.obj data placed at the retail VA).
- Control `--control one-function`: `?AnimationFinished@VinceLaundryCar@@UAEXXZ` VA 0x000A157F (offset +0xD in the
  function), `ret` -> `int3`, object BasketCase.obj: .text FAIL (1 differing byte offset, rawSize 0x14515C = retail),
  15 others OK, sha1 `2e773344...` != pin, exit 1. One flipped byte in one of our functions is caught.
- Originals unchanged: `check --originals` exit 0, sha1 = pin.
- Gate after the pin: `tools/gate.py` 24 passed, 0 failed of 24, exit 0.
- Our objects vs the gate build (T6.c1, C0079; not re-run in T6.c2): non-debug sections and symbol table equal 18/18;
  whole-file sha256 differs only by COFF TimeDateStamp and the `.debug$S` path.
- Traps (C0079): 43 unreferenced static-const COMDAT `.rdata` sections in SoundNode.obj (`std::_Iosb`/`_Locbase`)
  have no image VA -> skipped (non-associative, no reloc targets them); MoviePlayer.obj inline COMDATs also defined
  by a cut original -> the cut definition renamed `<name>@va_<va8>`, our unreachable copy dropped by /OPT:REF;
  `__fltused` (undefined in every float object, no reloc) refused exit 2 until pinned 0x002231E0 with evidence
  (`docs/evidence/compiler_pin.md` "### __fltused 0x002231E0 (2.3 T6)").
- Info, .reloc: our dir 100524 (0x188AC) vs retail 113836 (0x1BCAC); HIGHLOW ours 48109 vs DIR32 in the linked cut
  sections 48495 (-386; originals run: 48555, -446).
- Claim boundary: as T5: the section bytes (16 digests) come from the link, with our matched functions and Main.obj's
  data at their retail VAs; the header region is the pin's. Equality proves our 24 functions' bytes, relocations
  included, reproduce retail in place in the whole image; it says nothing about unmatched code (still the originals).

## C++ EH substitution (T7)
Case: gate fixture `tests/gate/matched_set.toml` rows 17-20, `std::_Uninit_fill_n<Vector3*>` (0x00044B6E, 0x4F) in
EvilVince.obj (`/EHa`): `.text` +0x1 DIR32 `__ehhandler$...` (`.text$x`, 0xA, ASSOCIATIVE), stub +0x1 DIR32 `$T366`
and +0x6 REL32 `___CxxFrameHandler`; `.xdata$x` 0x50 (FuncInfo/unwind/try tables `$T372` @0, `$T374` @0x10,
`$T373` @0x20, `$T366` @0x34). T7.c1 refused: `.xdata$x`: 0 VAs from its symbols by name.
- VAs (tools/relink.py `_ours`, no pin added): the stub 0x00152BDC by name (Ghidra byname has `__ehhandler$...`; the
  retail field at fn+0x1 gives the same VA); `.xdata$x` 0x001FD8A8 from reloc site `.text$x+0x1 DIR32 $T366` at
  0x00152BDD (retail field - our addend - 0x34). Rule: a kept section no name places -> any DIR32/REL32 site in a placed
  section of ours; fixpoint; sites disagreeing or none -> refusal.
- Retail placement finding: the stub is NOT right after the body (0x00044BBD); it lies in the last .text cut
  (gap 0x00152ACE..0x0015615C), i.e. retail LINK grouped every `.text$x` stub at the .text end. `.xdata$x` lies in
  .rdata near its end (`.rdata.obj` 0x001C71C0..0x002039C4): LINK merges `.xdata` into .rdata by itself (our map:
  `0008:... .xdata$x DATA`, no /MERGE flag) and sorts the group after every plain `.rdata` contribution.
- Placement (ladder rung 3, `$` group names): covering cut objects split at the holes; the piece past each hole renamed
  `<group>~` so LINK sorts it after our group: `.text piece 0x00152BE6 -> .text$x~`, `.rdata piece 0x001FD8F8 ->
  .xdata$x~` (9 chars: header long name `/<strtab offset>`). The stub has no /ORDER slot (static; rides `.text$x`).
  Refusal kept for the general case: retail bytes between two of our sections of one group.
- Green (fixture, relink.gate_check): `map (XBE VA): 10/10 found; placed: EvilVince.obj .xdata$x 0x001FD8A8 OK;
  EvilVince.obj .text$x 0x00152BDC OK`; `$T366/372/373/374 linked == reloc-site VA OK`; 16/16 digests, sha1 = pin.
- Gate: `--self-test` exit 0, positive run `5 passed, 0 failed of 5`, `PASS relink: pass`, `controls: 20 failed as
  expected of 20`, 224 s. Bare gate 24/24, `PASS relink: pass`, 18 s. `relink.py check` (real set) unchanged: 16/16,
  sha1 OK, `$T318 ... symbols_pin 0x001EC830 OK`, code holes 0; `--control one-function` exit 1 (15/16).
- Check now fails (exit 1) when a placed section or `$` static of ours links off its resolved VA.

## .rdata VA order (3.3 T12)
Cause: `_link` placed each object of ours whole, by command-line order, before the cut piece after its first .rdata
hole (`ours_before`): a unit's 2+ .rdata items linked as one block. Scattered items landed wrong (C0126:
StandardFileHandler::Open "rb"/"r+b"/"wb" 0x1CDD64..6C next to BeepFile's "" 0x1C7E50); adjacent `??_C` literals
pushed the ALIGN_1 retail gap piece between them after our block (C0133: BeepTDF "\n" 0x1CDB90 + "{}" 0x1CDB94,
BeepLib "/\:." 0x1CDBF8 + "/\:" 0x1CDC00; next piece +4, .rdata vsize +8/+0x10, write_section refusal).
- Fix (tools/relink.py `_link`, layout ladder rung 3 as T7): when our .rdata data holes exist, every plain .rdata cut
  piece not past one of our `$` group holes -> `.rdata$<va8>` (its start VA), and every plain .rdata data section of
  ours (not past such a hole) -> `.rdata$<va8>` in a copy of our object read from its path and dumped to
  `link/<ld>/obj/ours/` (the original object and `_ours` state untouched). LINK sorts `$` contributions by name, so
  each lands at its retail VA; those holes no longer place our object (`ours_before` skips them). `.xdata$x` /
  `<grp>~` (T7) and its refusal unchanged; .data and .text unchanged.
- Control: `relink.py check --control rdata-unsorted` (renaming off; `check_whole` now passes control to `_link`):
  with a lifted row the whole-image write refuses on the .rdata size (vsize 0x3C81C vs XBE 0x3C80C with all three)
  -> under this control reported `check all: FAIL whole-image write refused` and exit 1 (as check_section's control
  write refusals); placed lines FAIL (BeepLib.obj .rdata 0x001CDBF8 linked 0x001CDBFC, BeepZlib.obj 0x001CDDBC linked
  0x001CDDC0, `$T318` linked 0x001EC838 vs pin 0x001EC830).
- Probe (C0095, scratch list + gate_check): FindMatchingBrace alone, then all three: rc 0, 16/16, sha1 = pin;
  placed `BeepFile.obj .rdata 0x001CDD64/68/6C OK, 0x001C7E50 OK; BeepLib.obj 0x001CDBF8/0x001CDC00 OK;
  BeepTDF.obj 0x001CDB90/0x001CDB94 OK`.
- Lifted: BeepTDF::FindMatchingBrace, BeepLib::StripPath, StandardFileHandler::Open (3.3 T12.c1).

### 3.4 T6.1
- Mechanism (C0136, confirmed T6.1.c2): our kept out-of-line COMDAT of a folded member (`??1?$ResourceHandle<..>`,
  `??_I`, `??4?$ResourceHandle<..>`) is a code hole (`_ours`, pin VA by name) with a `grp` key, so `_link` gave it no
  /ORDER slot (T7 rule for `.text$x` stubs); LINK put it elsewhere, .text shifted: `our PE entry 0x14B5 + pe_base
  0x10C80 != pin entry 0x0001215F` (VinceSwing probe before the fix, 3 code holes). Second wall behind it
  (SetEmptyTexture): matched SwampGatorNode.obj names another public at the hole (`??4?$ResourceHandle@VModelResource..`
  0x0001819A) -> `external .. lies in a replaced range`.
- C0135: Onion.obj and HurricaneHannah.obj both place `__real@3df0000000000000` 0x001C8000 / `__real@41f0000000000000`
  0x001C8008 -> `our ranges overlap at 0x001C8000`.
- Fix (relink.py): a plain `.text` COMDAT code hole with its symbol at 0 takes its /ORDER slot by that symbol; our
  external naming another public at offset 0 of such a hole -> weak external to the hole's symbol (weak.obj, class
  105, SEARCH_ALIAS); identical duplicate holes (same VA, name, section, Selection not 1/5, bytes, relocs by name) ->
  one hole, T12 renames every copy. Any other entry mismatch, hole reference or overlap: refused as before.
- Controls: `check --control entry` (our first .text fn to the head of /ORDER): exit 2 `our PE entry 0x14F9 + pe_base
  0x10C80 != pin entry 0x0001215F`; `--control literal` (synthetic `__real@0000000000000000` 0x001CDBA8 pair, byte 0
  flipped): exit 2 `our ranges overlap at 0x001CDBA8`. Gate self-test relink-entry, relink-literal: rc 2 REFUSED, only
  relink fails; 35/35.
- Probes (C0095, scratch list + src under .run/3.4/probe/, gate._core, scratch bdir): ??1VinceSwing (pin_emit scaffold +
  emitter row) rc 0, placed VinceSwing.obj .text 0x000120C1 and 0x000292A6 OK; SetEmptyTexture rc 0 (SwampRenderer.obj
  .text 0x0001819A OK, weak ModelResource -> TextureResource operator=); Onion RandomFloat rc 0 (2 identical holes
  ride HurricaneHannah.obj's); ~ResourceHandle row as `<AnimationResource>` (pin alias): rc 2 `not a COMDAT .text
  section` (the draft instantiates the VertexShaderResource public; not relink), as `<VertexShaderResource>`: rc 0.
  All rc 0 runs: 16/16 digests, sha1 = pin.
