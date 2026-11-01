# Matching conventions: annotations, provenance, asserts, markers, method (1.6 T4)

The forms every matched function copies. Minimal and greppable: each form has one spelling, and `grep -rn '<marker>' src`
finds every use. Process: `docs/matching/START_HERE.md`. Rules cited by id (`rules/<id>.md`).

## 1. Annotation lines (D77's two lines, reccmp grammar, LEGO Island LEGO1/BETA10 manner)
File line 1, every `.c`/`.cpp` under `src` (`decomp.h` and `src/README.md` have none), one spelling, no prose head above or
beside it; `PY tools/unit_header.py --check` holds it, and the gate does not:
```
// UNIT: src/<on-disk-case posix path under src> OBJ: <units.toml name> OPT: <units.toml opt>
```
Written by `tools/unit_header.py --write`; never by hand. After any change to a unit's `opt` in `config/retail/units.toml`,
re-run `--write`.

Above each function:
```
// FUNCTION: USA 0x00140B98
// PROTOTYPE: PREVIEW 0x0052A466 identical
// NAME: proto-pdb:default.pdb
int LocString_GetStringID(...)
```
- Line 1, the target, always: `// FUNCTION: <target id> 0x<VA>`. VA = retail XBE VA in `0x%08X` form (the matched_set
  `va` spelling, so `grep 'FUNCTION: USA 0x00140B98' src` joins an annotation to its `config/matched_set.toml` row).
- Target ids: `USA` = the retail USA `default.xbe` whose sha1 is `config/check.retail_xbe.sha`
  (`2953b78229018a34e7f53e85bd34f124d51ef210`). The only target (X3). Another build, another id; never reuse `USA`.
- Line 2, prototype provenance, where the 1.2 map has a row: `// PROTOTYPE: PREVIEW 0x<protoVA> <class>`. `PREVIEW` =
  the prototype `default.exe` (+ `default.pdb`) of the Jul 15 2003 preview (`prototype_dir`, sha in
  `config/inputs.manifest.jsonl`); protoVA and class from `derived/proto_map/retail-xbe/functions.jsonl` (`protoVA`,
  `class`: identical | near-identical | ambiguous). The class is the confidence (the map has no other). Never a target:
  the marker word is `PROTOTYPE`, never `FUNCTION`, so no reccmp-style parser reads it as one; never match evidence (D81),
  unverified (D42). No map row: no line 2.
- Line 3, the name's source (D42), §2.
- Functions in a unit file run in ascending VA where declarations allow; never restructure a unit to reach it (D111,
  was D77). `annotations.py --check` prints order problems as `ORDER` lines with their count and does not fail on them.
- FOLDED (D111; reccmp docs/annotations.md "Code folding", R0-025, R2.2-005): the linker's identical-code folding
  (`/OPT:ICF`) gives several functions one VA. Each such function carries the shared VA with `FOLDED` as the last
  token, after the address; every copy is marked, none left bare (the reccmp example marks both):
  ```
  // FUNCTION: USA 0x<VA> FOLDED
  ```
  Lines 2-3 as above, per function. `annotations.py --check` agrees (4.1 T4): more than one line for a VA passes only if
  every line is FOLDED; a bare duplicate fails. The matched set lists one row per VA (`add_function` refuses a second);
  the other copies are never rows. First case: `src/Apps/Vince/scripts/french_quarter/Blender.cpp` 0x00077A04.
- Compiler-generated function (implicit dtor, no body in source; 2.2 T3 `??1FlourSackMain@@UAE@XZ`): the three lines
  sit directly above the class declaration that implies it (src/Apps/Vince/scripts/french_quarter/FlourSack.cpp).
- GLOBAL, VTABLE, STRING, TEMPLATE, SYNTHETIC: not used yet; a task that needs one writes its form here first.

## 2. Provenance tags (D42) and placeholders (D78)
- Every identifier (function, type, global, field) taken from the prototype PDB or exe carries its source, spelled as
  the `source` column of `config/retail/symbols.txt` / `units.toml`: `proto-pdb:default.pdb` (PDB names, types,
  layouts), `proto-exe:default.exe` (a fact read only from prototype exe bytes). Other sources: `xbsymdb`, `ghidra`
  (those columns' values), `retail` (read from retail disassembly or strings).
- Spelling: `// NAME: <source>`. Function: annotation line 3. Type or global: a trailing comment on its declaration
  line (`struct Vector3 { // NAME: proto-pdb:default.pdb`; `extern int g_swamp; // NAME: proto-pdb:default.pdb`).
  Field: covered by its struct's tag; a field from another source carries its own (`// 0x1C NAME: retail`).
- No tag, no PDB name: an identifier without a `NAME:` comment is a placeholder or a C/C++ keyword-level name.
- Placeholders for unnamed things (retail address or offset, never a guess; D78): function `function_<VA %08X>`
  (`function_00140B98`), global `global_<VA %08X>`, field `field_<offset %X>` (`field_1C`), unnamed type
  `struct_<VA %08X of the first function using it>`. A value of unknown type keeps a sized integer and says so:
  `int field_1C; // 0x1C undefined4`. A placeholder is renamed only with evidence (PDB, retail string), and the new
  name gets its `NAME:` tag.
- A gate function row needs the retail symbol name (`config/retail/symbols.txt`), so a matched function's name is that
  row's name and its tag is that row's source column.
- No renaming of PDB names while the repository is private (D42). Decided for release (D108, developer 2026-10-30):
  prototype-PDB names are kept, no rename; tags stay in the public source; position (b) disclosure applies.

## 3. Struct layout: size assert and offset comments (`src/include/decomp.h`)
- cl 13.10 has no `static_assert`. `DECOMP_SIZE_ASSERT(type, size);` (Halo 2 typedef form
  `typedef char x[(cond) ? 1 : -1]`) right after every PDB-derived struct a matched function uses, size from the PDB
  TPI (C0023). A wrong size fails the compile (C2118); a right one emits no symbol (1.6 T4 control, C0042). In a C
  unit name the tag (`DECOMP_SIZE_ASSERT(struct S, 0xC);`) unless the struct is typedef'd; C++ takes `S`.
- Every member of such a struct carries its offset: `float x; // 0x00`. Virtual methods: `// vtable+0x18`.
- Include by relative path (build.py passes no `/I`): `#include "../../../../include/decomp.h"` from
  `src/Apps/Vince/scripts/Carnival/X.cpp`.
- Only what a function uses; a struct is written out to the members the function reaches, the rest stay
  `field_<offset>` (D78), the size assert still holds the PDB size.

## 4. `!FAKE` marker (D66, D33) and the forbidden list (D79)
- Spelling, on the line above the construct: `// !FAKE: <compiler pass>; <instrument>; <reason>`.
  pass = what cl 13.10 does that the construct steers (c1xx front end, c2 register allocation, c2 block layout,
  inliner, EH state); instrument = the source construct used (dummy local, temporary, reordered statement, cast,
  split expression); reason = the evidence (the attempt, the before/after `match_one` lines).
  `// !FAKE: c2 register allocation; dummy local; esi/edi swapped without it (match_one 97.1 -> 100.0)`
- Marks every match-only construct the original author would not have written (D66: dummy locals, volatile locals).
  A `!FAKE` names a lever; it is never a cookbook reference (D33).
- Forbidden, `!FAKE` or not (D79; Halo 2 "Do not" list): `__declspec(noinline)`, `__forceinline` or any attribute the
  original lacks; `#pragma` inside a function body (`#pragma optimize` included); `volatile` on a global; global address
  or offset tricks; volatile function pointers; `__asm`/`_asm` (also refused by the gate's `asm` verdict); per-function
  flags (build.py has none: a unit has one `opt`).

## 5. NON_MATCHING (D41, G4)
```
// FUNCTION: USA 0x<VA>
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 96.4, bytes mismatch (ebx/esi swapped)
...
#endif
```
- Annotation lines stay outside the `#ifdef` (grep still finds the function); one comment line inside says the best
  result. The gate never defines `NON_MATCHING`; such a function is never a matched_set row (its `defined` verdict
  fails on absence). The only mechanism for unmatched code.

## 6. Method notes (CK2, CK5, CK18, CK25; adapted under D62)
Source: BFM cookbook C0003, C0006, C0133, C0294 (gcc 2.7.2, MIPS; R1.6-004, R1.6-005, R0-012). Kept only where
generic or where 1.4/1.5 cl 13.10 evidence supports it; the rest waits for a reproducer (D62).
- Work-loop rules: gate the raw XBE bytes before any transform (CK18): the claim is the gate's byte compare on retail
  bytes (D40), never a disassembly listing, a decompiler view or a normalised diff; keep an attempt journal in the task
  log (each edit, its `match_one` line, undo to the last best). Judge in the destination unit (CK25): a draft is judged
  in its unit file, built with the unit's `opt` (`match_one.py` builds the whole unit file); a standalone scratch compile
  can refuse a standalone claim, never a unit claim.
- Adopted, generic: integer width and sign follow the asm loads (`movzx`/`movsx` byte/word -> `unsigned char`/`char`/
  `unsigned short`/`short`; mask or cast where the asm masks) (C0003 T3).
- Adapted, cl 13.10 evidence: block order. `/O1` lays basic blocks in source order; shape `if`/`else` and early
  returns to the retail branch order and fall-through (C0028; BFM C0003 T4 was gcc branch selection).
- cl 13.10 classes seen in 1.4/1.5 (residual taxonomy):
  - dtor without vptr store = no declared dtor in the class; out-of-line emission forced; `/Ob0` control (C0029).
  - `/EHa` try-exit state store; `__ehhandler$`, `__EH_prolog`, `__except_list` relocations; `/EH` per unit (C0030).
  - switch: inline accessor tell, jump-table statics, `$L<n>` labels (C0031).
  - COMDAT Selection 2 = an inline copy, not a function row (C0032).
  - target-side: objdiff below 100 on a byte-matched function (Ghidra caseD labels C0033; no code relocs C0034;
    same-section rel32 never relocated C0035). A `blocked` finding about the delinked target, never a gate tuning;
    pick candidates with zero such sites (`docs/matching/next_candidates.md`).
- Class roll-call (2.2 T11; verdicts from 2.2 task summaries T4-T10; no class reached a wall, D55):
  - float/x87: match, 4 rows first attempt (Swamp::MarkSceneRenderingStartTime, DoorStorageRoomMain::TimerEvent,
    LoadScreen::RenderPersisted, SwampInput::SetGameTime); `/O2` control discriminates (2.2 T4; C0064, C0065).
  - vcall: match, 4 rows; slots from PDB intro vbaseoff (2.2 T5; C0066).
  - bit test: match, 6 rows; `test byte` from plain-int flags + LF_ENUM masks, no `bt` (2.2 T6; C0067, C0068).
  - inlined callee: match, proven by control on 2 matched rows (FlourSackMain dtor 0x00079868, MoviePlayer::
    GetRenderTexture 0x000DA005): `/Ob0` fails, `/Ob1` identical (2.2 T7; C0069).
  - switch: match, VinceDropsInCutscene::SetState 0x000A871D first attempt; jump table past Ghidra bodyMax (2.2 T8;
    C0070, C0031, C0033, C0034).
  - EH: match, `main` 0x000E0BB0 (SEH scope table); targets pinned, body holes filled in delink (2.2 T9; C0071, C0072,
    C0030).
  - library source (not a class; size-bucket fill): `_adler32` zlib 1.1.x at `/O1 /Oy`, `/O2` control (2.2 T10; C0073).
- Pending a reproducer (D62; next generation's first wave, each needs a five-line cl 13.10 repro before it is a lever):
  - CK14 (C0023) compiler-quirk wall toolkit: gcc global allocation order, rematerialisation; MSVC analogue unknown.
  - CK21 (C0190) declaration form as a lever (gcc sched1 hoist, alias.c array decl).
  - CK24 (C0293) `(void)`-declaration and empty-call wall (gcc front-end prototype checks).
  - C0003 T1 (constant folding of pre-loop pointer init), T2 (scheduling follows source order).
  - C0006 classes: hoisted invariant vs induction-variable init order; gcc phantom 16-byte frame; cross-jump tail
    merge (gcc jump.c).

## 7. Readability (developer decision 2026-10-27; BFM gen3 standards, fetched summary R0-004:169-184)
Readability ranks below every rule and every section above; it relaxes none of them (most at risk: D40, D41, D42, D66,
D78, D79, D81) and is never match evidence (D81). It acts only where the gate is blind (meaning without bytes), so
every point below is evidence-bound.
- When more than one source form has passed the gate, keep the one closest to ordinary code: plain control flow, no
  cast or temporary the gate shows can be removed.
- Struct members over pointer arithmetic only where the pointer's type is on record (the PDB type of the parameter,
  local, global or field it was read from, under its `NAME:` tag); the field by PDB name, else `field_<offset>`. A
  pointer with no type on record keeps its cast or takes `struct_<VA>` (§2). A PDB struct is never chosen because an
  offset fits.
- A readability edit to a matched function is a new attempt under §6: journaled; `PY tools/gate.py` green
  (`N passed, 0 failed of N`, exit 0) before `commit_task.sh`; on anything else the agent first reverts the file to
  its committed form. `match_one.py` is not the gate.
- A construct needed for the match stays and carries its `!FAKE` line (D66): an edit adds a missing marker, never
  removes one whose construct stays.
- Comments state only what the code, the PDB or a retail string shows. Any comment saying more than the code itself
  (purpose, behaviour, a name, a value's meaning) cites its source in §2's vocabulary with a locator:
  `// src proto-pdb:default.pdb Swamp::m_timer`, `// src retail 0x<VA> "LoadScreen"`. No guessed intent, hedged or
  not ("probably", "likely", "seems"); in D78's spirit, no invented name in a comment. No comment is required on a
  self-evident function; a comment marking an unknown as unknown needs no source (D78).
- A comment is never evidence for a name, type or match. A string a function references shows that it references the
  string, never its name or purpose; a `retail` name needs a string that is the name (RTTI descriptor, assert or log
  text naming the function or class), and that rename is a matching task with its §2 tag, never a readability edit.
- Checked by `PY tools/provenance_lint.py` (§2 tags and placeholders, `// src` locators, hedges, `!FAKE` removal
  and D66 constructs; `docs/ops/tools.md` "provenance_lint").
- Locals: the prototype PDB's local name where `PY tools/pdb_types.py locals` has the function (covered by the
  function's `NAME:` tag), else a neutral form (`v<N>`, `tmp`, `i`, `p`); never a name stating purpose or meaning (D78).
- No rename for readability: placeholders change only with evidence (§2); PDB names stay, in the public release too (D108,
  developer 2026-10-30). D42's "no renaming while private" is unchanged; D108 amends nothing.
- What a later rename would need, none being planned; it takes a new developer decision reversing D108, plus a
  developer-ratified D42 amendment if the repository is still private: the developer supplies each new name or a
  mechanical rule; each renamed identifier is tagged `NAME: rename:<decision ref>`, and this repository's symbol file
  keeps its original name in its own column (`renamed_from`), the source column unchanged; whether that column is
  exported is decided with that decision. Renames land in this repository in one task after the decision and before
  any export carrying them, gate green.
