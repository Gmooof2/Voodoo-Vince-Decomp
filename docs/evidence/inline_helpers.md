# Inline helpers: evidence for a ruling (4.2 T3.c1)

Brief for a pa-thinker who reads only this file. It decides nothing and recommends no rule change.

The question: seven `__forceinline` uses sit in `src` although CONVENTIONS forbids the attribute, and five rows are held
under D78 for want of a helper or a body. What does each possible answer cost, in matched functions lost and held rows freed?

Source of the numbers: scratch runs of `tools/match_one.py` in `.run/4.2/t3/a/` (`--ledger` scratch, `src` copies), `objdiff`
and `bytes` from each ledger row. "pass" = objdiff 100.0 and bytes `match`. match_one is not a match claim: the gate
(`tools/gate.py`) and `matched_set.py check` were not run on any scratch variant. A claim from a log or a source
annotation is labelled as such.

## Texts, verbatim

I9, `phase-ends/phase-4.1/research/R0-002.md:23-29`:

> ### I9 (3.4 T12.c2): D78 and inline helpers
> - Problem: retail inlines a free Vector3 dot, Vector3 operator+, and a by-value Matrix product. default.pdb has none of them (not in publics, not in TPI names; a free inline never appears in a class method list). The eax-reached temp shows an inline existed; its name and form would be a guess (D78).
> - Held functions: 4 in the pool_buckets held-d78-inline bucket (docs/matching/pool_buckets.md:49-55, list at :256): ranks 1113, 1115, 1150, 1168. Example SwampCamera::GetFrontClipPlane (1113, 92 B). Other named rows: RodeoRatMain::CollidedWith 98.05, SwampCamera::GetViewMatrix 74.10, HannahDebris::Render (logs/T12.c2.md:29-34,52). A second table at pool_buckets.md:308 shows held-d78-inline: 0 (not reconciled).
> - The bucket VAs are hard-coded in tools/pool_buckets.py:66-87 (ruling text "3.4 T12 ruling").
> - Options: (a) ask pa-thinker if a `!FAKE` inline helper with a neutral placeholder name meets D78; (b) keep held until the developer rules; (c) PDB-recorded helper once Gen 4 question 40a575bd is answered.
> - Recommendation: (a) then developer ruling before gen-4 matching. pool_buckets.py is unlocked, so the held list can change without review.
> - Files: rules/D78.md (status active, [now]); docs/matching/CONVENTIONS.md; tools/pool_buckets.py (unlocked).

D78, `rules/D78.md:6`:

> No invented structure: never introduce a field, type, callee, guard or constant absent from the prototype PDB, the retail disassembly or retail strings; an unknown stays visible as `undefined4`, `unknownNN` or `function_<address>`, never as a plausible name or a guessed member

D79, `rules/D79.md:6`:

> No codegen steering the original developers would not have written: no `noinline` or non-original attributes, no `#pragma` inside a function body, no `volatile` on globals, no global address or offset tricks, no volatile function pointers; a residue survives only under D66's `!FAKE` with its reason

CONVENTIONS, `docs/matching/CONVENTIONS.md:86-89`:

> - Forbidden, `!FAKE` or not (D79; Halo 2 "Do not" list): `__declspec(noinline)`, `__forceinline` or any attribute the
>   original lacks; `#pragma` inside a function body (`#pragma optimize` included); `volatile` on a global; global address
>   or offset tricks; volatile function pointers; `__asm`/`_asm` (also refused by the gate's `asm` verdict); per-function
>   flags (build.py has none: a unit has one `opt`).

C0125, `cookbook/C0125.md:5-7`:

> - Value-returning inline wrappers (`Quaternion::Apply(v)`, `Matrix::Apply(v)`, `Matrix::AffineInverse()`) that retail inlined stay out-of-line in our /O1 build with plain `inline`: mark them `__forceinline` with `// !FAKE:` (SwampCamera::GetViewPlanes 65.1 -> 97.6).
> - Two struct-returning calls in a ternary get separate return temps; `if (c) X = f(); else X = g();` shares one temp (GetViewPlanes 97.6 -> 100).
> - An empty inline `Vector4(){}` keeps an inline `Matrix(){}` out-of-line (member array needs a vector ctor iterator): drop it under `// !FAKE:` (Sprite::Render).

The 3.4 T12 ruling, `phase-ends/phase-3.4/tasks/T12.md:17` (line 7 of that file, the pointer in the task brief, is summary prose; the ruling is line 17):

> - Ruling (partner, D78): no inline helper absent from default.pdb in 3.4; __forceinline precedent covers only PDB-recorded methods; the 4 inline-operator rows stay NON_MATCHING with best lines, T13 buckets them; question filed for Gen 4 (40a575bd)

So the two standing texts differ: CONVENTIONS forbids `__forceinline` "`!FAKE` or not"; T12 treats it as a precedent limited to PDB-recorded methods. All seven uses below are on PDB-recorded methods. No held row's helper is.

## Enforcement today

No tool enforces the ban. A grep of `tools/`, `.claude/hooks`, `.claude/skills` and `config/` for the word finds two Python files:
- `tools/provenance_lint.py:226`: treats `__forceinline` as a skippable declaration prefix while finding the name a marker belongs to. No verdict.
- `tools/readability_census.py:60`: a classifier pattern that counts `!FAKE` reasons starting with `__forceinline`. It counts only.

## Row counts

`config/matched_set.toml` for the five units: SwampCamera 6, Sprite 3, Onion 23, Cannon 2, switchCrypt 11: **45 `[[function]]` rows**. The task brief's 50 (Onion 28) counts 5 `[[emitter]]` rows of Onion, which are non-retail scaffold symbols with no target; they were not run. Baseline: 45/45 pass.

**The 7 uses protect 4 matched rows, not 7:** SwampCamera's three all serve GetViewPlanes, and Sprite's serves no matched row.

## Results common to all 7 uses (levers on all 45 rows)

Each lever replaced every `__forceinline` in the five units.

- In-class implicit inline (`__forceinline` removed): 41/45. The 4 rows lost are GetViewPlanes, GetFacing, SetBaseRotation, GetWorldFacing. The other 41 are unmoved.
- Plain `inline`, `__inline`: 41/45, the same 4 rows at the same percentages.
- Helper body forms, each 41/45, same 4 rows, same percentages: out-of-class `inline` definition; `this->Apply`; comma return; `return T(r)`; `const` return type; value overload declared first (the PDB's method order); `Vector3()` ctor defined out of class. Budget stopped at those 7 (with removal and `inline`, 9).
- Mechanism, from `/FAs` listings of the removal lever on GetFacing, SetBaseRotation, GetWorldFacing: the user calls the value-returning helper out of line, where the `__forceinline` build calls the void overload directly.
- Flag controls on the removal source (one flag on `/O1 /Oy`; `/O1` expanded to `/Og /Os /Oy /Ob2 /Gs /GF /Gy` passes 45/45 on the real source, so the expansion is sound):
  - `/Ob1`, `/Ob2` explicit, `/Oi`: 41/45, same 4 rows.
  - `/O2`: 11/45. `/Ot` in place of `/Os`: 11/45, identical percentages. These move 34 rows and recover none of the 4.
- File-scope pragmas on the removal source (**D79 spirit**: not in the forbidden list's letter, but they steer codegen):
  - `#pragma auto_inline(on)`, `#pragma inline_depth(255)`: 41/45, same 4 rows.
  - `#pragma inline_depth(1)`: 35/45. The same 4 rows plus Sprite's GetTexture 31.96, Onion's `~Sprite` 85.0, `~LevelLoader` objdiff 100 with bytes `provisional`, EnteringRegion 76.81, LeavingRegion 76.81, `AreaStats` scalar deleting dtor 85.71.
- Hand-expanded at the call site, helper deleted: 43/45. Passes GetFacing, SetBaseRotation. Fails GetViewPlanes, GetWorldFacing (per case below).
- No flag or pragma lever passed any of the 4 rows.

## The 7 uses, per case

PDB, for all seven: the value-returning helper is declared in the class's TPI method list; `derived/pdb_info/default/publics.json` holds only the void overloads (`?Apply@Quaternion@@QBEXABUVector3@@AAU2@@Z`, `?Apply@Matrix@@QBEXABUVector3@@AAU2@@Z`, `?Apply@Matrix@@QBEXABUVector4@@AAU2@@Z`) and no `AffineInverse`, so the prototype has no out-of-line copy of any helper. Retail shows the same absence: every user calls the void overload. The source's `PROTOTYPE ... identical` lines (source annotation, not re-checked here) say the same of the prototype users.

### SwampCamera.cpp:40 `Quaternion::Apply(const Vector3&)`, :51 `Matrix::AffineInverse()`, :54 `Matrix::Apply(const Vector3&)`
- User: `?GetViewPlanes@SwampCamera@@QBE?AUFrustum@@_N@Z` 0x000CB135, 686 B. The other 5 rows of the unit do not move under any source lever.
- Retail: no call to a helper. Calls include `Quaternion::Apply` void ×2, `Matrix::AffineInvert` ×1, `Matrix::Apply` void ×6, `GetPlaneEquation` ×4.
- PDB: `Quaternion` method `Apply` ×2, `Matrix` onemethod `AffineInverse`, `Matrix` method `Apply` ×4. Locals of the proc include `cam2world`, `result`, `world_up`, `world_right`, `location`, `left`, `right`, `top`, `bottom`, `half_fov`, `updown`, `leftright`.
- Each `__forceinline` removed alone: :40 83.37; :51 89.39; :54 84.74. All three removed: 67.23.
- `inline`, body forms, `/Ob1`, `/Ob2`, `/Oi`, `auto_inline`, `inline_depth`: 67.23. `/O2`, `/Ot`: 24.83.
- Hand-expanded: direct out-parameter 70.04; temp then copy, function scope 92.43; temp then copy, block scope 92.85. Stopped at 3 forms, so not proved impossible.

### Sprite.cpp:123 `Matrix::Apply(const Vector4&)`
- User: `?Render@Sprite@@QBEXXZ` 0x000BD746, 527 B, under `#ifdef NON_MATCHING` (best line 98.6). **Not a matched row; the 3 matched rows of the unit do not compile the use.** Removal costs no matched row.
- Retail: one call of the void overload `Matrix::Apply(Vector4)`. PDB: `Matrix` method `Apply` ×4 includes both Vector4 forms.
- Guard lifted in scratch, Render alone: with `__forceinline` 98.60; removed, `inline`, all 7 body forms, `/Ob1`, `/Ob2`, `/Oi`, pragmas 96.17; `/O2`, `/Ot` 32.55; hand-expanded 97.87. None passes.

### Onion.cpp:92 `Quaternion::Apply(const Vector3&)`
- User: `?GetFacing@SwampSceneNode@@QBE?AUVector3@@XZ` 0x00018056, 56 B.
- Retail: builds the axis, calls the void overload with a stack temp as out-parameter, copies the temp to the return slot. PDB: as above.
- Removed, `inline`, body forms, `/Ob1`, `/Ob2`, `/Oi`, pragmas: 40.52. `/O2`, `/Ot`: 32.67. **Hand-expanded (`Vector3 r; _Rotation.Apply(axis, r); return r;`): pass**, and the other 22 rows of the unit pass.

### Cannon.cpp:116 `Quaternion::Apply(const Vector3&)`
- User: `?SetBaseRotation@AimingCam@@QAEXABUQuaternion@@@Z` 0x0005D50B, 73 B.
- Retail: one call, the void overload. PDB: as above.
- Removed, `inline`, body forms, `/Ob1`, `/Ob2`, `/Oi`, pragmas: 98.23. `/O2`, `/Ot`: 28.97. **Hand-expanded (temp, then assign): pass**; the other row passes.

### switchCrypt.cpp:30 `Quaternion::Apply(const Vector3&)`
- User: `?GetWorldFacing@SwampSceneNode@@QBE?AUVector3@@XZ` 0x0005E3C6, 64 B.
- Retail: virtual `GetWorldRotation`, then the void overload with `this` taken **from the returned pointer in eax**. PDB: as above.
- Removed, `inline`, body forms, `/Ob1`, `/Ob2`, `/Oi`, pragmas: 46.23. `/O2`, `/Ot`: 41.00.
- Hand-expanded, 6 forms: temporary as object, axis local, axis const-ref temp 82.19 (3 forms); `const Quaternion&` bound to the call 97.42 (3 placements, same score). Listing of the best against retail: `this` is the address of the temp's stack slot rather than eax, and the argument pushes go through eax rather than ecx. A reference local is folded back to the stack slot; only an inlined parameter reference kept the eax reach in these runs.

## The 5 held rows, per case

For each: today's best, hand-expanded (no helper), plain-inline neutral helper, `__forceinline` neutral helper. Neutral helpers are scratch only and named `unknown_inline_dot`, `unknown_inline_add`, `unknown_inline_mul`: they prove what would unlock a row, nothing else.

The free helpers are absent from the PDB as I9 says: `publics.json` has no free `??H`, `??D`, `??G`, `Dot` or `Cross` over Vector3 or Matrix, and `Matrix` has `operator*=` but no by-value product in its method list.

### `?GetFrontClipPlane@SwampCamera@@QBE?AUVector4@@XZ` 0x000CAFF0, 92 B (held-d78-inline)
- Retail: `GetWorldFacing` into a stack Vector3; x, y, z copied to a Vector4 through eax as integers; virtual `GetWorldPosition`; a dot product reading the second operand **through eax, the returned pointer**; negated into w; Vector4 copied to the return slot.
- PDB: locals `this`, `result` (Vector4, bp-0x28), `viewdir` (Vector3, bp-0xc). No third named local; the Vector3 in between is a temp. `Vector4(const Vector3&)` is PDB-recorded.
- Today's best: **no draft in `src`** (the brief's "under NON_MATCHING" does not hold for this row; only the log line `T12.c2.md:34` names it, a log claim).
- Hand-expanded, 6 forms: fields copied with a ref, value or block local 86.71 (4 forms, dot order irrelevant); block copy through a cast 56.59 (2 forms). Listing: the dot reads the temp's stack slot, retail reads through eax.
- Plain-inline neutral: fields copied, **pass** (7 forms, all dot orders). With a block copy instead 68.05.
- `__forceinline` neutral: with a block copy 68.05 (2 forms); with fields copied, pass (run only in the unit check below).
- Unit: FCP and GVM helpers together, plain inline: 8/8 (the 6 matched rows plus both held rows). Same with `__forceinline`: 8/8.

### `?GetViewMatrix@SwampCamera@@UBE?AUMatrix@@XZ` 0x000CB0D1, 100 B (held-d78-inline)
- Retail: virtual `GetViewPosition` into a stack Vector3; `GetWorldFacing`; a Vector3 sum reading the second operand **through eax**, stored in a temp; `Matrix::View`; 16-dword copy to the return slot.
- PDB: locals `this`, `mtx` (Matrix, bp-0x64), `location` (Vector3, bp-0xc). No `facing` local.
- Today's best: `src` line, guard lifted: 74.10 (reproduced: 74.0952). Its `const Vector3& facing` is not a PDB local.
- Hand-expanded, 4 forms: 74.10 each. Listing: the sum reads the facing temp's stack slot, and the add order differs from retail.
- Plain-inline neutral: **pass**. `__forceinline` neutral: pass (2 forms).

### `?Render@HannahDebris@@UBEXABUMatrix@@@Z` 0x000277AF, 93 B (held-d78-inline)
- Retail: builds a Vector3 axis; `Matrix::Translation` on a stack Matrix; `Matrix::Multiply(const Matrix&, Matrix&) const` into a stack temp; 16-dword copy to a second stack temp; `GatorNode::Render` with the second temp.
- PDB: locals `this`, `m`, `trans` (Matrix). `Multiply`, `Translation` are PDB-recorded and public; a by-value product is not. `HannahDebris` base `Critter`; the unit holds no class for it, so the scratch used a stub chain (`GatorNode`, `Critter`, `HannahDebris`) that does not follow the PDB layout.
- Today's best: **no draft in `src`** (log claim only, `T12.c2.md:34`; `git log -S` finds no body).
- Hand-expanded: `Matrix t; m.Multiply(trans, t); Matrix t2(t); GatorNode::Render(t2);` **pass**, 2 runs, unit 5/5 (4 matched rows plus this one). It has two named locals, `t` and `t2`, which the PDB's locals do not list. One temp only: 59.08.
- Plain-inline neutral: 36.00; listing shows the helper stays a call. `__forceinline` neutral: pass, unit 5/5. Also pass with no `Matrix()` ctor.

### `?CollidedWith@RodeoRatMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z` 0x00063D47, 97 B (held-d78-inline)
- Retail: tests the `Ptr` and `type`; copies 3 dwords of `ImpactPlane` to a stack Vector3; `GetFacing`; dot reading the second operand through eax, starting with the z pair then the y pair then x; compare; virtual `SetState`.
- PDB: locals `this`, `type`, `critterHit`, `cr` only. The source's `normal` and `facing` are not PDB locals.
- Today's best: guard lifted 98.05 (reproduced 98.0465); the unit's 9 matched rows pass with it lifted.
- Hand-expanded: x-first 98.05; z-first 98.00.
- Plain-inline neutral: 99.91 in every form. `__forceinline` neutral: 99.91 in every form. 16 forms over 6 dot orders, swapped arguments, a `normal` local. Listing: identical to retail except that the y pair is emitted before the z pair. **Not freed by any form tried; stopped over budget.** Unit 9/9 with the helper.

### `??$RELEASE@UDirect3D@@@@YAXAAPAUDirect3D@@@Z` 0x000CA38E, 13 B (held-d78)
- Bucket's claim (`docs/matching/pool_buckets.md:93`): "a source for the inlined callee's body (e.g. `Direct3D::Release`); the constant has no PDB/disasm/string origin".
- Retail and prototype (0x004B55D7), same mnemonics: load the argument, compare the pointer with zero, branch, clear it, return. **No call and no constant of the callee.**
- PDB: `Direct3D` size 1, static `AddRef` and `Release`, both returning `prim0x22` (unsigned long). `publics.json` has no `Release@Direct3D`.
- Test against D78's wording: the claim is literally true of the value, and the value cannot be observed. `src` draft `return 1` pass; `return 0` pass; a third value pass. So retail cannot tell these bodies apart, and the digit in `src` is a constant absent from all three of D78's sources.
- Forms holding no constant: `static void Release() {}` **pass**, but its return type contradicts the PDB. Explicit specialization of `RELEASE<Direct3D>` with `if (obj) obj = 0;` and `Release` only declared **pass**; it omits the call `RELEASE<T>` makes for every other T. `unsigned long` with no return statement: refused by the compiler, C4716. `Release` only declared with the shared template: 35.80 (an emitted call).
- Unit: 20/20 (19 matched rows plus this one) for `return 1`, `void`, and the specialization.

## Cost table

Counts from the runs above: of 45 matched functions and 5 held rows. VP = GetViewPlanes, GF = GetFacing, SBR = SetBaseRotation, GWF = GetWorldFacing; FCP = GetFrontClipPlane, GVM = GetViewMatrix, HDR = HannahDebris::Render, COL = CollidedWith, REL = RELEASE<Direct3D>.

| Answer | Matched lost | Which | Held freed | Which |
|---|---|---|---|---|
| Forbid; helpers kept as plain inline | 4 | VP, GF, SBR, GWF | 0 | - |
| Forbid; the 7 uses hand-expanded, forms tried here | 2 | VP (best 92.85), GWF (best 97.42) | 0 | - |
| Allow `__forceinline` on PDB-recorded methods only (T12's text) | 0 | - | 0 | no held helper is PDB-recorded |
| Allow neutral helpers, plain inline only | 0 by itself | the `__forceinline` answer is separate | 2 | FCP, GVM |
| Allow neutral helpers with `__forceinline` | 0 by itself | same | 3 | FCP, GVM, HDR |
| Allow hand locals absent from the PDB's locals | 0 | - | 1 | HDR |
| Accept a dead-value body or a no-constant form for REL | 0 | - | 1 | REL |

The helper answers and the REL answer combine: at most 4 of 5 held rows are freed (FCP, GVM, HDR, REL). No lever tried freed COL (best 99.91).

## Options and their consequences

- **Keep the ban as written and remove the 7 uses.** CONVENTIONS and `src` then agree. Needs: delete 7 helper lines and their `!FAKE` lines; rewrite users.
  - Sprite: gate unaffected; `Sprite::Render`'s guarded best falls from 98.60 to 97.87 (hand) or 96.17 (plain).
  - Onion GetFacing and Cannon SetBaseRotation: hand forms pass here; each gains a local the source's helper hid. Rows stay matched.
  - switchCrypt GetWorldFacing and SwampCamera GetViewPlanes: no form passed, so `matched_set` loses both unless further forms succeed. Both lose an entry from the gate's matched count; their bodies go under NON_MATCHING.
  - The prototype records these helpers, so a rewrite drops a PDB-recorded method from the source's use.
- **Amend CONVENTIONS to match T12 (PDB-recorded methods only).** No row moves. The texts agree. Note that the T12 text covers PDB-recorded methods only, so it says nothing for the 4 held-d78-inline rows, and D79's "non-original attributes" stays open: nothing here shows whether the original source carried the attribute. Plain `inline` does not reproduce retail with these bodies under `/O1 /Oy`, by any form tried; it does not follow that the original differs only by the attribute.
- **Allow a neutral plain-inline helper under `!FAKE`.** Frees FCP, GVM. D78's "an unknown stays visible" and "callee" apply; the name would be visibly unknown, and the callee has no PDB record, though retail's reach through eax shows an inline existed (I9). No new attribute, so D79 is not touched.
- **Allow a neutral `__forceinline` helper under `!FAKE`.** Adds HDR. Touches both D78 and CONVENTIONS' ban.
- **Hand locals for HDR.** No helper and no attribute. The two locals are absent from the PDB's locals list; D78's list names field, type, callee, guard, constant, and not locals. COL's and GVM's `src` bests already carry such locals.
- **REL.** The three forms differ in what they add: a constant no source records, a return type the PDB contradicts, or a body that omits the call the template makes. Bytes are the same.
- **Keep all 5 held.** Nothing moves. `tools/pool_buckets.py` stays as it is.

Measured side facts:
- A plain or forced helper still leaves a `PROC` for the helper in the `/FAs` listing even when its user inlines it (helper `Quaternion::Apply` in the real `__forceinline` build of Onion, Cannon, switchCrypt; both neutral helpers in SwampCamera; the neutral product in HurricaneHannah). `tools/matched_set.py:343-344` passes a pick-any COMDAT code symbol as `inline-copy`; whether these are COMDAT was not checked on the scratch builds.
- No scratch row went through the gate or its relink check.
