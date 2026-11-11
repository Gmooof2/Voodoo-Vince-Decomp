# Reference unit: SearchlightBeam (readability pilot, 5.5 T17, updated T17.1; R133 stage 2, R142)

The first unit taken through readability stage 2: headers, evidence names, `!FAKE` trials, doc comments. Bytes
unchanged (gate `2797 passed, 0 failed of 2797`, PASS relink, at 20dc15570; T17.1 again, see Counts). Read it as the
proposed form for every finished unit; the open points at the end are for the developer. The form we aim for once
the tools allow it, with what each step needs: `docs/matching/reference_unit_target.md` (not compiled). Terms: Glossary.

## The unit
- `src/Apps/Vince/scripts/Sewers/SearchlightBeam.cpp`, obj `Xbox\OfficialRelease\SearchlightBeam.obj`, `/O1 /Oy`.
- 3 functions, 312 bytes, all matched: the SearchlightBeam ctor 0x000AB4F9, `SearchlightBeam::Update` 0x000AB463,
  the inline `FogZone::~FogZone` 0x000AB4C7. No open candidate row; no other task touches it.
- Picked from the 16 game units whose every function is matched: small, and carrying the debt the pilot needs to show
  (placeholders, two `!FAKE` lines, neutral locals, a scaffold).

## Headers
Where a header lives and what it is called comes from evidence, in this order:
1. A path the prototype PDB records in its C11 line info (cookbook C0445), when a proc of that type is attributed to
   it: `libs\swamp\include\fogzone.h` (FogZone's inline dtor, line 23), `libs\swampshared\swampresource.h`
   (`ResourceHandle<>::operator=`, line 68), `libs\swampshared\swampvector.h`, `libs\swamp\include\swampgatornode.h`.
   Mapped under `src/` by the recorded directory: `d:\beep\libs\swamp\include\` -> `src/Libs/Swamp/include/`,
   `d:\beep\libs\swampshared\` -> `src/Libs/SwampShared/`.
2. Else a header named for the PDB class it declares, saying no file is recorded (`SwampSceneNode.h`, `Swamp.h`,
   `SwampString.h`, and the unit's own `SearchlightBeam.h`: the PDB records `searchlightbeam.cpp`, no `.h`).
Every header's first line states which: `// HEADER: proto-pdb:default.pdb file <recorded path>` or
`// HEADER: named for PDB class <Name>; no file recorded`. One include guard, relative includes (build.py passes no
`/I`), no `#pragma`.

Contents: declarations, plus only the inline bodies the unit already had (PDB-recorded inlines: `~FogZone`,
`ResourceHandle<>::~ResourceHandle`, accessors). No new inline, helper or callee (D78, D117). Members follow §3:
the unit's own classes take PDB members where their type is declared (FogZone gained `_points[8]`,
`_visibilityDistance`, `_noiseSpeed`, `_noiseScale`; `_color` stays `field_108` because `Vector4` is not declared);
engine bases keep `field_<off>` for members the unit never reaches.

These headers are this unit's view of each class, partial by design. Stage 3 (shared headers) merges them across
units; until then no other unit includes them. Since T17.1 each of the 8 says so on line 2:
`// Partial: declares only what matched units use so far.`

## `!FAKE` trials (D66, D123)
- Update's Quaternion product (hand form of an inline `operator*`): `operator*` is not in the PDB (no method, public or
  proc), so the natural `rot * _baseRotation` needs a helper D117 forbids. Five forms using only the PDB locals
  `pos`, `rot` were tried (best 98.61, `SetRotation(Quaternion(Quaternion(rot) *= _baseRotation))`); none matched.
  The hand form and its `!FAKE` line stay. The PDB line info puts the product and `SetRotation` on one source line
  (38), which agrees with an inline `operator*` call in the original.
- The `pin_emit_FogZone_dtor` scaffold: without it `~FogZone` is still emitted at 100.0 (the matched ctor odr-uses it),
  so the construct is no longer needed. It stays for now: the gate fails when it is removed because (a) the
  `[[emitter]]` row in `config/matched_set.toml` goes stale and only a tool may remove it, which no tool does, and
  (b) the dtor's `FUNCTION:` line would have to sit in `FogZone.h`, and annotations.py and provenance_lint read
  annotations only in a unit's `.cpp`. Its `!FAKE` text ("the ctor is unmatched") is now out of date; the fake-diff
  check blocks editing it in place.
- The scaffold's annotation block (`SearchlightBeam.cpp`, the lines above `pin_emit_FogZone_dtor`) is mislabelled three
  ways, all left in place because no annotation or `!FAKE` line may change here: (1) `// FUNCTION: USA 0x000AB4C7` is
  the address of `FogZone::~FogZone`, not of the scaffold function beneath it; (2) its `// NAME: proto-pdb:default.pdb`
  vouches for `pin_emit_FogZone_dtor`, a name we made up; (3) its `!FAKE` text calls the SearchlightBeam ctor
  unmatched, and it is matched. The fix is the scaffold's removal (target doc, difference 6).

## PDB lookups (T17.1, `pdb_types.py enum SwampSceneNodeTypes`, `class SwampSceneNode`, `locals FogZone::FogZone`)
- `_Type = 9`: `SwampSceneNode::SwampSceneNodeTypes` has `0x9 UI` (0 Unknown .. 0xe Water); `_Type` is a plain `int`.
  The ctor now writes `_Type = UI;`, the enum declared in `SwampSceneNode.h` with only that enumerator, as other units
  do (`Zone`, `Sound`, `Region`). The PDB gives the name only; why a searchlight beam has type UI is not recorded.
- FogZone's ctor `FogZone(const ResourceHandle<ModelResource>&, const Vector3&, float)`: the PDB's locals for it name
  the parameters `volume`, `color`, `visibilityDistance`; `FogZone.h` now declares them. So the call in
  SearchlightBeam's ctor passes the model as `volume`, `Vector3(0.8f, 1.0f, 0.5f)` as `color` and `1.25f` as
  `visibilityDistance`. (A second FogZone ctor in the PDB takes `const BeepTDF* def`; not used here.)

## Doc comments
T17 put one line above each `// FUNCTION:`; each restated the code below it. T17.1 replaced the three with one
fact-only summary on the class in `SearchlightBeam.h`, and `rules/DRAFT-doc-comments.md` (for ratification at the next
approval) now says never restate what the code below shows, with the class summary as its good example.

## Counts (the .cpp before T17; the .cpp plus its 8 headers after T17, then after T17.1)
- `field_` lines 9 -> 8 -> 8 (`field_108` now names its PDB member); `!FAKE` lines 2 -> 2 -> 2; casts 1 -> 1 -> 1
  (`static_cast<GatorNode*>(GetParent())`, the PDB types `_Parent` as `SwampSceneNode*`); locals outside the PDB
  2 -> 2 -> 2 (`v0`, `v1`, the hand form); doc comments 0 -> 3 per function -> 1 class summary; bare type number
  `_Type = 9` -> `_Type = UI`; unnamed FogZone ctor parameters 3 -> 3 -> 0; headers saying they are partial 0 -> 0 -> 8.
  The .cpp went from 194 lines to 48, then 45. Gate after T17.1: `2797 passed, 0 failed of 2797`, PASS relink, listed_dirty 0 (T17.1.c1, 93b2ae881).

## Glossary
- prototype: an earlier build of the game, the Jul 15 2003 preview disc (`docs/ops/inputs.md`). Its `default.exe` came
  with its debug database, which holds the original developers' own names. Reference only, never a match target.
- default.pdb: that debug database (class layouts, member, parameter and local names, source line info).
- proto-pdb: the evidence label for "taken from the prototype's PDB"; `proto-pdb:default.pdb` names the file too.
- PREVIEW: the id of the prototype build in `// PROTOTYPE: PREVIEW 0x<VA> <class>`: the same function's address in the
  prototype `default.exe` and how alike the two are (identical, near-identical, ambiguous). The word `PROTOTYPE`, not
  `FUNCTION`, keeps it from being read as a match target (CONVENTIONS §1). prototype, PREVIEW, proto-pdb and
  default.pdb all point at the one preview build.
- NAME: `// NAME: <source>` under a `FUNCTION:` line or beside a declaration says where that name comes from (D42); here
  always `proto-pdb:default.pdb`. A name without it (`v0`, `v1`, `field_108`, `pin_emit_...`) is ours.
- !FAKE: marks code written in a form the original most likely did not have, kept because it gives the same bytes as
  retail (D66, CONVENTIONS §4): `// !FAKE: <compiler pass>; <instrument>; <reason>`. The bytes are still retail's; the
  marker says the shape is ours.
- size assert: `DECOMP_SIZE_ASSERT(Class, 0x<size>)` (`src/include/decomp.h`), a compile-time check that the class is
  the size the PDB records; a wrong member layout stops the build.

## Open points (the developer's review)
1. The header placement order (recorded path, else class name) as the stage-2 standard.
2. Tool support so the scaffold can go: a `matched_set.py remove --emitter`, and annotations and provenance_lint
   reading a `FUNCTION:` line in a unit's quoted header.
3. The doc-comment rule draft (T17.1: no restating, class summary).
4. The target form and its Gen 6 steps (`reference_unit_target.md`).

## Second unit: Whammy (5.5 T30)
- `src/Apps/Vince/core/Whammy.cpp`, obj `core\XBox\OfficialRelease\Whammy.obj`, `/O1 /Oy`; 16 own classes, the
  matched functions and 2 NON_MATCHING ones (InitCurve, FrameTick; untouched). Bytes unchanged: gate
  `2797 passed, 0 failed of 2797`, PASS relink, listed_dirty 0 (T30.c1, e975f6070).
- Headers by the same order: recorded `apps\vince\core\{whammy,critter,vince,gamecam,cameramanager}.h` -> `src/Apps/
  Vince/core/`; class-named `SwampScript.h`, `SwampInput.h` (the PDB's file list names both under
  `libs\swamp\include\`, no proc attributed), `SoundNode.h`, `SwampScriptManager.h`, `Matrix.h`, `CritterMain.h`,
  `CritterManager.h`, `Globals.h`. T17's 6 engine headers extended in place (no second declaration of a class);
  SearchlightBeam unchanged.
- New case: WaitForWhammyProjectileMain and WhammyProjectileMain stay in the .cpp. The PDB attributes their procs only
  to `whammy.cpp` and their `__FILE__` literal is `core\Whammy.cpp`, so the original declared them there.
- New case: a `FUNCTION:` block on a class (implicit `??_G`/`??1`) stays in the .cpp at its place, over a forward
  declaration of the class (annotations are read only in a unit's .cpp).
- Free declarations with no class (`_strnicmp`, the debug `operator new`, `BeepLib::RandomFloat`) stay in the .cpp.
- Counts (.cpp before; .cpp + 19 headers after): `field_` declarations 28 -> 29 (T17's SwampSceneNode split);
  `!FAKE` 28 -> 28; casts 8 -> 8; locals outside the PDB 18 -> 18; undocumented own classes 16 -> 0; .cpp lines
  1167 -> 667 (1410 with headers). Trial: KillMonsterNow without its stand-in `critter`, 2 forms, objdiff 61.46; kept.
- PDB names: declaration parameters from the PDB locals (e.g. `StartAnimation(anim, looped, reverse)`); `0x100`
  (`_animTriggerFlags`), `0x14` and `10` (messages to `_parent`) stay bare: no PDB enumerator, or the receiver unknown.
- Open: the PDB names the members behind the 8 shell classes' `field_50` (e.g. `_animFlatten`); a `ResourceHandle`
  member changes the implicit dtors, so naming them is a matching task, not this pass.
