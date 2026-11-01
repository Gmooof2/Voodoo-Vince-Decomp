# Compiler-pin test functions (T4.c1, phase 1.4)

Run 2026-10-25, scratch `.run/1.4/T4/scan.py` (not kept). Retail USA `default.xbe` sha1
`2953b78229018a34e7f53e85bd34f124d51ef210`. Output: `config/compiler_pin.toml` (hand-kept). Names, addresses, counts,
PDB types only (D46). Facts: prototype `default.pdb` (types, publics), retail disassembly (iced-x86 1.21.0, C0010),
units.toml/symbols.txt/functions.jsonl rows (D78). Positions are proto_map `unverified` matches.

## Selection rule
Pool = `functions.jsonl` rows with: class `identical`; exactly one targetVA; unit (targetVA in units.toml
`text_start..text_end`) with `kind = "game"` and `delink = true`; not a tail stub; targetVA in no `retail_folds.jsonl`
retailVA; a `func` line in symbols.txt at that VA.
- Tail rule, deviation: brief said `targetVA < 0x13BD4E`. 0x13BD4E..0x13F2F4 is a prototype `.text` offset
  (units.md:17-20, C0020), not a retail VA; retail layout differs (proto 0x53B077 -> retail 0x1519F7). The literal rule
  would drop 91 rows, incl. all of LocString.obj (0x140B35..0x140D9C), the brief's gs unit. Applied: protoVA - 0x400400
  in 0x13BD4E..0x13F2F4 -> tail. The 893 proto-tail rows are 878 ambiguous, 13 identical, 2 near-identical; none
  falls in a unit range (units.md:40), so the rule excludes 0 after the unit test.
- Features, per decoded body:
  - fpo: first two insns not `push ebp; mov ebp,esp` and no memory operand based on ebp.
  - loop: a jcc/jmp near target in `[start, insn)`.
  - switch: `jmp [index*4+disp32]`, no base.
  - eh: an `fs:[0]` access, or a call to a proc whose first 4 insns touch `fs:[0]` (`__EH_prolog` shape); plus
    `push imm32`/`mov eax,imm32` of a handler that is `___CxxFrameHandler` or jumps there within 3 insns.
  - gs: LocString.obj only (`security_checks = true`); an abs-memory load into a reg in the first 8 insns.
  - ob: body contains the masked bytes (C0010 mask; trailing ret dropped; >= 6 B, >= 4 unmasked) of a smaller proc
    it does not call and that does not start at its own VA. Candidates: same-unit pool procs; all pool `std::` procs;
    plus 1615 non-pool identical/ambiguous rows (`std::` names or first targetVA in a pool unit). Short inline procs
    are mostly `ambiguous` (C0010), so pool-only candidates gave 0 hits.
- Picks: covers the six features; prefer 16..200 B, no `call [abs]`, few callees and few struct accesses.

## Denominators
- functions.jsonl rows 10388. Excluded in order: not identical 5254; targetVAs != 1 0; no unit 73; unit not game 1436;
  unit not delink 2417; tail 0; fold 0; no symbol 60. Pool 1148.
- Per feature in the pool: fpo 615, ob 8, loop 229, switch 1, gs 0, eh 23.
- In 16..200 B without `call [abs]`: fpo 558, ob 3, loop 127, switch 0, gs 0, eh 16. Switch has no in-band
  candidate, so its one pool proc (1732 B) is picked outside the band; gs is the only absent feature.

## Picks
### std::fill<Vector3 *,Vector3> 0x00044995, 33 B: fpo, loop
- Why: smallest fpo+loop with 0 calls and 0 struct-base accesses; `[esp+n]` args, push esi/edi, rep-free `movsd` x3
  copy per element, backward `jne`.
- PDB: `void (Vector3*, Vector3*, const Vector3&)`, cdecl. `Vector3` size 0xC: `float x` 0x0, `y` 0x4, `z` 0x8.
- Callees: none.
### std::_Uninit_fill_n<Vector3 *,unsigned int,Vector3,std::allocator<Vector3> > 0x00044B6E, 79 B: eh, loop
- Why: smallest eh with a loop; same unit and type as pick 1. Frame via `mov eax,<handler>; call __EH_prolog`, try
  state `[ebp-4]`, loop `dec [ebp+0Ch]` / `add [ebp+8],0Ch`, catch block `_CxxThrowException(0,0)` (rethrow).
- PDB: `void (Vector3*, uint, const Vector3&, std::allocator<Vector3>&, std::_Nonscalar_ptr_iterator_tag)`, cdecl.
  `std::allocator<Vector3>` size 0x1 (base `std::_Allocator_base<Vector3>`). `std::allocator<Vector3>::construct`:
  thiscall `void (Vector3*, const Vector3&)`.
- Callees: `?construct@?$allocator@UVector3@@@std@@QAEXPAUVector3@@ABU3@@Z` 0x00044A73; `__CxxThrowException@8`
  0x0014161F; retail 0x00141600 (no symbols.txt name; body opens `push -1; push eax; mov eax,fs:[0]`; prototype PDB
  public `__EH_prolog` exists, VA mapping unverified). Handler thunk 0x00152BDC (no name; `mov eax,<funcinfo>; jmp
  ___CxxFrameHandler` 0x001411E6) lies outside the unit range: likely a tail piece (EvilVince `procs_tail = 1`), so the
  delinked unit lacks it (units.md:40).
### FlourSackMain::~FlourSackMain 0x00079868, 31 B: fpo, ob
- Why: smallest ob hit. Body = two inlined `ResourceHandle<AnimationResource>` dtors (`if (_Resource)
  --_Resource->_RefCount`) on `_animFall` 0xFC then `_animStuck` 0xF8, then `jmp ??1CarriedObjectMain@@UAE@XZ`
  (no separate HeavyObjectMain dtor code). The out-of-line `ResourceHandle<AnimationResource>::~ResourceHandle`
  exists in the PDB (Onion.obj) and is not called. Matched proc: `GbcmlMain::~GbcmlMain` (ambiguous, targets
  0x00079875, 0x001228EC) = the second half. No vptr store in the body.
- PDB: thiscall, `FlourSackMain*`. `FlourSackMain` size 0x108: base `HeavyObjectMain` 0x0;
  `ResourceHandle<AnimationResource> _animStuck` 0xF8, `_animFall` 0xFC; `int _id` 0x100; `bool _pickedUp` 0x104,
  `_vinceInBounds` 0x105, `_destroyOnImpact` 0x106, `_firstDrop` 0x107. `HeavyObjectMain` size 0xF8: base
  `CarriedObjectMain`, virtual `~HeavyObjectMain`. `CarriedObjectMain` size 0xF8 (base `CritterMain`).
  `ResourceHandle<AnimationResource>` size 0x4: `AnimationResource* _Resource` 0x0. `AnimationResource` size 0x34:
  base `ResourceBase`. `ResourceBase` size 0x24: `std::basic_string _Name` 0x0, `uint _RefCount` 0x1C, `bool _valid`
  0x20. Enums in scope: `FlourSackMain::Messages`, `FlourSackMain::States` (not used by this body).
- Callees: `??1CarriedObjectMain@@UAE@XZ` 0x0010F085 (tail jump).
### LocString_GetStringID 0x00140B98, 34 B: fpo
- Why: the gs unit (`security_checks = true`); fpo C-linkage, 0 calls, so it pins /GS (no cookie when no local
  buffer) together with fpo outside the std:: templates. Body: null handle -> -1; `id > m_pshString->m_uNumberOfEntries`
  (unsigned `ja`) -> -1; else `m_aStringIndex[id].m_wValue` (`movzx` word, stride 6).
- PDB: `int (void*, uint)`, cdecl, symbol `_LocString_GetStringID`. The `void*` handle fits `NLocString::BLOBDATA`
  (size 0x14; LocString_OpenBLOB mallocs 0x14 B and stores +0x0..+0x10): `SECTIONHEADER* m_pshString` 0x0, `STRINGENTRY* m_aStringIndex` 0x4,
  `ushort* m_pStringData` 0x8, `BeepFile* m_blobFile` 0xC, `long m_lStringDataInFile` 0x10. `NLocString::SECTIONHEADER`
  size 0x8: `ulong m_dwSectionSize` 0x0, `uint m_uNumberOfEntries` 0x4. `NLocString::STRINGENTRY` size 0x6 (packed):
  `ushort m_wValue` 0x0, `ulong m_dwOfs` 0x2.
- Callees: none.
### VinceDropsInCutscene::SetState 0x000A871D, 1732 B: switch (T4.c2)
- Why: the only jump-table proc in the 1148 pool; picked over the 16..200 B preference because the done-when needs
  switch coverage when a candidate exists. Detector tags switch only: frame `push ebp; lea ebp,[esp-74h]; sub
  esp,32Ch` (ebp-based, not fpo), no backward branch, no `fs:[0]`, no `call [abs]`.
- Jump table: `mov ebx,[ebp+7Ch]` (arg `state`); `cmp ebx,0Bh; ja 0x000A8DA5` (default = common tail); `jmp
  [ebx*4+0x000A8DB1]`. 12 cases 0..11, 12 distinct targets. Table 0x000A8DB1..0x000A8DE1 = the last 0x30 B of the
  1732-B row length (table inside the proc). Tail 0x000A8DA5: `mov [esi+54h],ebx` (`_state = state`).
  Case body shape: `basic_string(const char*)` on a 0x1C-stride stack local, `NewCamera` or
  `Swamp::_animationManager->Load` then `GatorNode::StartAnimation` on `_vince`/`_ethel`, inlined
  `ResourceHandle<AnimationResource>` dtor (`if (_Resource) --_Resource->_RefCount`, `dec [eax+1Ch]`), `_Tidy(true,0)`.
  27 `push imm32` string addresses (retail strings; not listed).
- PDB: `VinceDropsInCutscene::SetState`, thiscall `void (int)`, virtual, JaniceCutscenes.obj. `VinceDropsInCutscene`
  size 0x84: base `CutsceneDriver` 0x0; `Vince* _vince` 0x78, `Critter* _ethel` 0x7C, `bool _wasWhammyReady` 0x80.
  `CutsceneDriver` size 0x78: base `SwampScript`; `AnimCam* _cam` 0x4C, `bool _cancelled` 0x50, `_autostop` 0x51,
  `int _state` 0x54, `float _cameraShake` 0x58, `std::basic_string _cameraName` 0x5C. `SwampScript` size 0x4C (vfptr
  0x0). `enum VinceDropsInCutscene::States`: `STATE_V_FALLSIN` 0x0 .. `STATE_V_ONIT` 0xB (12 = the 12 cases),
  `STATE_LAST` 0xC. `enum VinceDropsInCutscene::Ids`: `ID_VINCE` 1, `ID_ETHEL` 2. `Vince` base `Critter` base
  `GatorNode` (size 0x200), all at 0x0. Global 0x002369CC (no symbols.txt name), read as `[g+18h]` -> Load `this`:
  fits `Swamp*` (size 0x68, `AnimationManager* _animationManager` 0x18; PDB public `?g_swamp@@3PAVSwamp@@A` exists,
  VA mapping unverified). `AnimationManager` size 0x5C: base `ResourceManager<AnimationResource>` (size 0x58) 0x0.
  `ResourceBase::_RefCount` 0x1C (see FlourSackMain pick).
- Callees (6 distinct, 89 call sites): `??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z`
  0x000117DE x27; `?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@IAEX_NI@Z` 0x000111D6 x17;
  `?StartAnimation@GatorNode@@QAEAAV?$ResourceHandle@VAnimationControl@@@@ABV?$ResourceHandle@VAnimationResource@@@@_N1@Z`
  0x000AF048 x15; `?Load@?$ResourceManager@VAnimationResource@@@@QAE?AV?$ResourceHandle@VAnimationResource@@@@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`
  0x000C02D3 x15; `?NewCamera@CutsceneDriver@@IAEPAVAnimCam@@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`
  0x001087D6 x12; `?SetOption@SwampSceneNode@@QAEXI_N0@Z` 0x000B3E23 x3. No tail jump, no indirect call.

## Absent
- gs: 0 of 7 LocString.obj procs (6 pool + `BSearchGetString` 0x00140B35, no symbols.txt name) load an abs global in
  their first insns; no `___security_cookie` read in any (prototype PDB public `___security_cookie` exists, seg 9).
  LocString_OpenBLOB (401 B, 0x30-B frame) holds a `std::basic_string` local, no char array, and no cookie.
- switch: not absent (T4.c2). 1 of 1148 pool procs, `VinceDropsInCutscene::SetState`, now picked (see Picks).
  Jump-table procs carry abs table words in code (C0010 `.reloc` note), which likely keeps most out of `identical`.

## Tool gaps
- `llvm-pdbutil dump --types` on default.pdb: "Unsupported TPI Version" (TPI 19990903, VC 7.0/7.1). pdb_info.py has
  no TPI reader. Types above came from a scratch TPI reader (`_ST` leaves, pascal names).

## Results (T9)
### LocString_GetStringID: match
- Source `tests/pin/locstring_getstringid.cpp`; flags `/O1 /Oy` (the std::fill flags, first try); control
  `["/Oy","/Oy-"]` -> mismatch (size 0x25, 36 differing bytes). `compiler_pin.py` 2 matched of 2; `--control` exit 0.
- Lever that mattered: source shape only. Two early returns (`if (!blob) return -1; if (id > n) return -1;`) put the
  `-1` block first (`jbe` over it): 16 differing bytes at +0x7. One combined condition with the value as fall-through
  (`if (blob && id <= n) return v; return -1;`) gives retail block order (`je`/`ja` to a trailing `or eax,-1`).
- /GS: `/O1 /Oy /GS` bytes == `/O1 /Oy` bytes (no local buffer, no cookie); cl 13.10 has no `/GS-` (D4002). This body
  does not pin /GS.

### FlourSackMain::~FlourSackMain: match
- Source `tests/pin/floursackmain_dtor.cpp`; flags `/O1 /Oy /Ob1` (`/Ob1` is /O1's default, spelled out for the
  control); control `["/Ob1","/Ob0"]` -> mismatch (size 0x21, handle dtors called, 19 differing bytes).
  `compiler_pin.py` 3 matched of 3; `--control` exit 0.
- Lever that mattered: dtor declarations. User-declared dtors store the vptr even with an empty body: declared
  `~FlourSackMain` + inline `virtual ~HeavyObjectMain() {}` = size 0x2B (two `mov [ecx],offset ??_7...`, 35 differing
  bytes); dropping only `~HeavyObjectMain` = 0x25. Both implicit = match. Retail ctor 0x796EC and `~CarriedObjectMain`
  0x10F085 do store their vptr, so `__declspec(novtable)` (also matches bytes) is ruled out: it would drop the ctor's
  store too.
- Implicit dtor needs an odr-use to be emitted; retail's is the ctor (not modeled). The source uses a test-only driver
  `pin_emit_FlourSackMain_dtor` (qualified `p->FlourSackMain::~FlourSackMain()`), marked as scaffolding (D78 note).
- Pads (D78): `CritterMain` past vptr (0xF4), `ResourceBase::_Name` (0x1C), `AnimationResource` past base (0x10).
- Not pinned by this body: `/Oy` vs `/Oy-` (same bytes, no frame), `/O1` vs `/O2` (`/O2 /Oy` also matches).

### std::_Uninit_fill_n: matched (1.4 T9, 5 of 5)
- Source `tests/pin/uninit_fill_n_vector3.cpp`; flags `/O1 /Oy /EHa`: size 0x4F, relocs 5, resolved 2 (construct,
  `__CxxThrowException@8`), masked 3, zero differing bytes. Proposed control `["/EHa","/EHsc"]` -> mismatch (0x4A, 37
  differing bytes at +0xB). Toml entry not filled pending the reloc decision (entry would read provisional, gate rc 1).
- Lever that mattered: `/EHa`. `/EHsc`, `/GX`, `/EHs` (all 0x4A, 37 diff) drop the try-exit `or [ebp-4],-1`, put the
  catch block before the epilogue and reschedule the prologue; `/EHa` keeps the state store and retail block order.
- Ladder (all `/EHsc` unless noted): `/Ob0`, `/Ob2`, `/Oy-` = base 0x4A; `/O2`/`/Ox` 0x60; `/Os` 0x82; `/Od` 0x9D;
  `/O2 /Oy /EHa` 0x6E. `/O1 /Oy /EHa /Ob0` also 0x4F zero diff (/Ob does not pin here).
- Source shape: VC 7.1 <xmemory> body with the `_Next` destroy loop gives the same bytes as the destroy-free
  `catch (...) { throw; }` (trivial Vector3 dtor); kept destroy-free (no `allocator::destroy` emitted, D78).
- Unresolved relocs (retail field in parens): +0x1 DIR32 `__ehhandler$<sym>` (text$x static, 0x00152BDC);
  +0x6 REL32 `__EH_prolog` (0x00141600); +0x3C DIR32 `__except_list` (CRT absolute 0, field 0x00000000).
  Retail 0x152BDC = `mov eax,0x1FD8DC; jmp ___CxxFrameHandler` (named 0x001411E6), our thunk's shape; retail funcinfo
  0x1FD8DC = magic 0x19930520, maxState 2, 1 try block {0,0,1,1}, catch-all handler at 0x44BB3 = fn+0x45 (our catch
  block offset), as our `$T341`. Retail 0x141600 decodes as the CRT `__EH_prolog` body (push -1/push eax/fs:[0] ...
  lea ebp,[esp+0Ch]; push eax; ret).

### VinceDropsInCutscene::SetState: matched (1.4 T9, 5 of 5)
- Source `tests/pin/vincedrops_setstate.cpp`; flags `/O1 /Oy` (no `/EH`): size 0x6C4, relocs 144, resolved 131,
  masked 13, zero differing bytes. With the 13 resolved as `va - fn.value + label.value` (same-section static), a
  scratch relocate+compare over all 0x6C4 B (code and the 12-word table) gives 0 differing bytes. Proposed control
  `["/Oy","/Oy-"]` -> mismatch (0x6E3, 1638 differing bytes at +0x1). Toml entry not filled (gate rc 1 on provisional).
- No EH: retail has no `fs:[0]`/`__EH_prolog` although every case holds `basic_string` temps; `/EHa` 0x891, `/EHsc`
  0x82D. JaniceCutscenes.obj built without `/EH` (EvilVince.obj had `/EHa`): EH is per-obj, not global.
- Lever that mattered: `Swamp::GetAnimationManager()` (inline; named in the PDB `Swamp` field list beside
  `_animationManager`, no out-of-line symbol). A direct `g_swamp->_animationManager->Load(...)` loads Load's `this`
  after pushing all args (306 differing bytes, 15 sites); the inline accessor call evaluates it before the outer
  `StartAnimation` bool pushes, as retail. Explicit `std::basic_string<char>(s)` temps change sizes (0x66A/0x634).
- Body (D78, from disasm): cases 0..11 in source order; each `NewCamera(str)`, then `_vince`/`_ethel`
  `->StartAnimation(g_swamp->GetAnimationManager()->Load(str), b, false)` (b true only case 0xB `_ethel`), and
  `SetOption(2, b, false)` (case 2 `_ethel` false; 4 `_vince` true; 5 `_vince` false); `break`; tail `_state = state`.
  Shared `_Tidy(true,0)` tail and cross-jumps are the compiler's. Enum names 0x1..0xA not in this doc: values used.
- Ladder (all with the accessor): `/O1`, `/O1 /Oy` + `/G5`/`/G6`/`/G7`/`/Ob2`/`/GS` all 0x6C4 zero diff (not pinned
  here); `/Ob0` 0x6BB; `/Oy-` 0x6E3; `/O2 /Oy` 0x784; `/Os /Oy` 0x7C3 (pre-accessor).
- symbols_pin rows (T9.c4): `?g_swamp@@3PAVSwamp@@A` 0x002369CC (PDB public exists; retail `mov eax,[0x2369CC]; mov
  ecx,[eax+18h]` -> Load `this` at all 15 sites, matching `_animationManager` 0x18); 27 `_s_<va>` labels = the 27
  `push imm32` -> `basic_string(const char*)` operands. The `_<8 hex>` suffix rule cannot serve them: its regex needs a
  leading letter, and every C/C++ data symbol starts `_` or `?`.
- Unresolved relocs: 12 table words DIR32 -> `$L<n>` case labels and the `jmp [$L<n>+ebx*4]` operand, all static
  labels in the function's own section with value != 0. byte_compare.resolve maps only the section symbol (value 0)
  in that case; extending it to same-section statics (`S = va - fsym.value + sym.value`) would make this a match.

## Callee pins (2.1 T4)
### GameOptions::Set 0x00125F98 (2.1 T4)
- Pin: `0x00125F98 func 0x2B ?Set@GameOptions@@QBEXXZ` (`public: void __thiscall GameOptions::Set(void) const`).
  Source `proto-pdb:default.pdb+proto-map+retail-xref`; addresses and names only, no bytes (D46).
- PDB: prototype `default.pdb` public `?Set@GameOptions@@QBEXXZ` seg 1 off 0x10E6A5 (derived/pdb_info/default/
  publics.json) = proto VA 0x0050EAA5 (same +0x400400 as `?BackupState@GameOptions@@QAEXXZ` off 0x10E695 = 0x0050EA95);
  module `core\XBox\OfficialRelease\Globals.obj`, length 44 (derived/proto_map/prototype-xbe/functions.jsonl).
- Cross-reference chain: prototype `GameOptions::RestorePreviousState` (PDB, proto 0x0050EAD1, 24 B) ends with
  `jmp 0x0050EAA5` at +0x13 (Set); retail `?RestorePreviousState@GameOptions@@QAEXXZ` 0x00125FC3 (proto_map identical,
  24 B) ends with `jmp 0x00125F98` at +0x13 (decoded E9 rel32 from default.exe / retail default.xbe, 2.1 T4 scratch).
- proto_map: derived/proto_map/retail-xbe/near_identical.jsonl `GameOptions::Set` protoVA 0x0050EAA5 -> candidateVA
  0x00125F98, similarity 92.31 (not identical: hence absent from symbols.txt, which takes identical rows only).
- Order and size: retail Globals.obj run `??0GameOptions@@QAE@XZ` 0x125F5F (0x29), `BackupState` 0x125F88 (0x10, ends
  0x125F98), Set, `RestorePreviousState` 0x125FC3, as the prototype run 0x50EA6C/0x50EA95/0x50EAA5/0x50EAD1. Size 0x2B =
  0x125FC3 - 0x125F98; Ghidra applied body 0x125F98..0x125FC2 (43 B), last byte `ret`; proto 44 B.

### main 0x000E0BB0 SEH callees (2.2 T9)
Addresses and names only, no bytes (D46). `main` (Main.obj, 0x3C, proto_map identical, proto 0x004CBF55) is SEH
`__try`/`__except` (cl 13.10 /O1 `push <frame>; push <scopetable>; call __SEH_prolog`), not C++ EH.
- Cross-reference chain (E8 rel32 decoded in prototype default.exe and retail default.xbe, 2.2 T9 scratch): proto main
  +0x7 call 0x0052B17C, +0x10 call 0x004CBC5D, +0x36 call 0x0052B1B7; retail main +0x7 call 0x001418AC, +0x10 call
  0x000E0887, +0x36 call 0x001418E7. Push at +0x2: proto 0x005D5430, retail 0x001EC830 (the scope table, below).
- 0x000E0887 `?handledMain@@YAXXZ`: PDB public seg 1 off 0xCB85D = proto 0x004CBC5D, module
  `core\XBox\OfficialRelease\Main.obj` (S_GPROC32 len 0x2F8); near_identical.jsonl protoVA 0x004CBC5D -> candidateVA
  0x000E0887 (92.34). Size 0x329 = main 0x000E0BB0 - VA; Ghidra applied bodyMax 0x000E0BAF agrees.
- 0x001418AC `__SEH_prolog`, 0x001418E7 `__SEH_epilog`: PDB publics seg 1 off 0x12AD7C / 0x12ADB7 (proto 0x0052B17C /
  0x0052B1B7), `__except_handler3` off 0x12ADD0 (proto 0x0052B1D0). Spacing retail = proto: prolog -> epilog 0x3B,
  epilog -> 0x00141900 0x19; retail prolog pushes 0x00141900 as proto pushes 0x0052B1D0 (`__except_handler3`; pinned
  2.3 T4: our Main.obj `.sxdata` names it and the relink link must resolve it; no reloc, no bytes depend on it). Retail bodies decode as the CRT sehprolg shapes (prolog: fs:[0] link, `sub esp,eax`,
  ends `ret` at +0x3A; epilog: restore fs:[0], pops, `leave`, `push ecx; ret`, ends +0x10). Sizes 0x3B, 0x11 to the
  last `ret` (Ghidra bodyMax 0x001418E6 / 0x001418F7 agree). No unit: the gap 0x1418AC..0x1419EC after sprintf.obj.
- 0x001EC830: retail .rdata dwords {0xFFFFFFFF, main+0x17, main+0x2D} = one scope entry (enclosing -1, filter,
  handler). cl 13.10 emits it as a static `$T<n>` in .rdata (fixture `/FAs`: `$T<n> DD 0ffffffffH / FLAT:$L<filter> /
  FLAT:$L<handler>`; no PDB symbol, Main.obj module has only `$L` labels for main). Pinned under our object's `$T<n>`
  name (byte_compare and objdiff resolve by exact name); a source change that renumbers it fails the gate visibly.
- Pinned 2.2 T9.c1: `$T318` (Main.obj at "/O1 /Oy", match_one `--listing`: `$T318 DD 0ffffffffH`, `push OFFSET FLAT:$T318`).

### __fltused 0x002231E0 (2.3 T6)
Addresses and CRT constants only, no bytes (D46). CRT `int _fltused = 0x9875;` (fltused.c); every float-using object
has `__fltused` undefined with no reloc, so the whole-image relink must define it (C0079). Pinned 2.3 T6.c2.
- PDB public `__fltused` seg 9 (.data) off 0x1F7E0 = proto 0x0060BC00 (proto image base + .data VA + off); the proto
  dword there is 0x9875, the only 0x9875 dword in proto .data.
- Proto neighbours (publics, same seg): -0x10 `?__pMyUnhandledExceptionFilter@@3PAXA` = 0x0052DABC (public
  `?__CxxUnhandledExceptionFilter@@YGJPAU_EXCEPTION_POINTERS@@@Z`), +4 `__ldused` = 0x9873, +8 `__FPinit` = 0x0052B764
  (public `__fpmath`), +0xC `__FPmtinit` and +0x10 `__FPmtterm` both 0x004C7652 (a folded `ret`, public `__fpclear`).
- Retail .data (VA 0x002039E0): exactly one 0x9875 dword, at 0x002231E0. Same spacing: -0x10 = 0x001441EC
  (symbols.txt `?__CxxUnhandledExceptionFilter@@...`), +4 = 0x9873, +8 = 0x00141E94 (symbols.txt `__fpmath`), +0xC and
  +0x10 equal each other (0x0011C7BC), as proto. Unique value + five matching neighbours -> `__fltused` = 0x002231E0.

### ~BeepState callees (2.5 T4)
Addresses and names only, no bytes (D46). Retail `??1BeepState@@QAE@XZ` 0x000365DD (BeepState.obj, 0x55) calls
FUN_00018696 after loading `_TDFState._DefaultRecord` (+0x18) and ends with `jmp thunk_FUN_000386cd` (delinked cut,
llvm-objdump -d -r, 2.5 T4 scratch). Our draft emits `??1Record@BeepTDF@@QAE@XZ` and `??1DebugState@BeepState@@QAE@XZ`.
- 0x00018696 `??1Record@BeepTDF@@QAE@XZ`: derived/proto_map/retail-xbe/functions.jsonl row names
  `BeepTDF::Record::~Record` protoVA 0x004078ED, class identical, targetVAs [0x00018696] (a fold with std::pair dtors;
  hence absent from symbols.txt). The field +0x18 is `_TDFState._DefaultRecord: BeepTDF::Record*` (PDB TPI).
- 0x000386F8 `??1DebugState@BeepState@@QAE@XZ`: proto_map row `BeepState::DebugState::~DebugState` protoVA 0x00426DEF,
  length 5, excluded `thunk` (no target). Prototype run: ~DebugState 0x00426DEF (5 B) then
  `BeepState::DebugState::DebugState` 0x00426DF4 (= +5). Retail BeepDebug.obj cut: `thunk_FUN_000386cd` (one 5-byte
  `jmp FUN_000386cd`) at 0x000386F8, then `??0DebugState@BeepState@@QAE@XZ` 0x000386FD (proto_map identical to proto
  0x00426DF4) = +5. Same order and size -> ~DebugState = 0x000386F8. It is the implicit member dtor call at the end of
  ~BeepState (`_DebugState` at +0x0).

### SoundNode DSOUND callees (2.5 T6)
Addresses and names only, no bytes (D46). Retail `?SetMinDistance@SoundNode@@QAEXM@Z` 0x000C5C6F and
`?SetMaxDistance@SoundNode@@QAEXM@Z` 0x000C5C9A (SoundNode.obj, proto_map identical, proto 0x004B0F71 / 0x004B0F9C)
each call one XDK dsound stdcall (delink cut names `DSOUND::IDirectSoundBuffer_SetMin/MaxDistance`; symbols.txt has
the undecorated Ghidra names only; proto_map rows `ambiguous`, 2 hits, so no identical row).
- Cross-reference chain (E8 rel32 decoded in prototype default.exe and retail default.xbe, 2.5 T6 scratch): proto
  +0x18 call 0x005637B1 / 0x0056378D; retail +0x18 call 0x0017A211 / 0x0017A1ED.
- PDB publics `_IDirectSoundBuffer_SetMinDistance@12` seg 5 off 0x1A31, `_IDirectSoundBuffer_SetMaxDistance@12` seg 5
  off 0x1A0D; seg 5 VA 0x00561D80 (default.exe section table) -> 0x005637B1 / 0x0056378D, the proto call targets.
- Size 0x24 each: symbols.txt Ghidra rows (0x0017A1ED +0x24 = 0x0017A211), proto spacing 0x24, proto_map length 36.

### JaniceDefeatCutscene::SetState string labels (3.3 T19)
Addresses and names only, no bytes (D46). Retail `?SetState@JaniceDefeatCutscene@@EAEXH@Z` 0x000A9D72
(JaniceCutscenes.obj, matched 3.3 T13 with the literals pinned as `??_C@` names) pushes 11 string operands; the 11
VAs are repinned as `_s_<VA8>` data labels, VinceDrops form (T9.c4 above), so the source holds no string text.
- VAs (body order): 0x001EA60C, 0x001EA5F4, 0x001EA62C, 0x001EA61C, 0x001EA654, 0x001EA63C, 0x001EA67C, 0x001EA664,
  0x001EA68C, 0x001EA6B4, 0x001EA69C; status `retail-disasm disasm-verified`; each old `??_C@` line kept as a `#`
  comment in symbols_pin.txt. No other source or matched row named the 11 `??_C@` names (git grep).
- After `delink.py --fresh`, `relink.py cut`: gate 406 of 406 with the row PASS (relink PASS).

### XDK callee pins (3.3 T19)
Addresses and names only, no bytes (D46). Each retail callee was named only by Ghidra/xbsymdb (undecorated, C0123);
cross-reference chain as 2.5 T6: the E8 rel32 at the same caller offset in proto default.exe and retail default.xbe
(callers proto_map `identical`); proto target = the single PDB public; M18 callee check run through
`name_sites.callee` (masked-equal, outgoing masks name-agree, < 8 unmasked bytes need one agreement).
- `~SwampGamePad` 0x000AD74E +0xA -> retail 0x001C09A8 / proto 0x005A9F48 `_XInputClose@4`, 0xC: PASS (outgoing
  `?XID_fCloseDevice@@YIXPAU_XID_OPEN_DEVICE@@@Z` agrees). Pinned.
- `SoundNode::Pause` 0x000C5B46 +0x1B -> 0x00179A7C / 0x0056301C `_IDirectSoundBuffer_Pause@8`, 0x1C; `IsPlaying`
  0x000C5CC5 / `IsPaused` 0x000C5CF2 +0x17 -> 0x00179A98 / 0x00563038 `_IDirectSoundBuffer_GetStatus@8`, 0x1C. First
  run: callee-name-conflict (outgoing +0x15 retail names were xbsymdb `CDirectSoundBuffer_Pause`/`_GetStatus`). Those
  two targets checked themselves: 0x001790A8 / 0x00562648 `?Pause@CDirectSoundBuffer@DirectSound@@QAGJK@Z` and
  0x001790F9 / 0x00562699 `?GetStatus@CDirectSoundBuffer@DirectSound@@QAGJPAK@Z`, 0x51 each, PASS (outgoing
  `?DirectSoundEnterCriticalSection@@YGHXZ`, `?Pause|GetStatus@CMcpxBuffer@DirectSound@@...` agree). Pinned all four;
  the outer two then PASS.
- `Sprite::ComputeSize` 0x000BD690 +0x20 -> 0x0015B440 / 0x005449E0 `_D3DTexture_GetLevelDesc@12`, 0x5: PASS (one
  outgoing `?Get2DSurfaceDesc@PixelJar@D3D@@...` agrees). Pinned.
- `SwampRenderer::ClearScreen` 0x000CA39B +0x36 -> 0x0015C280 / 0x00545820 `_D3DDevice_Clear@24`: FAIL, not
  masked-equal (proto proc length 1000, retail Ghidra 0x2C9: XDK build differs). No pin; row stays NON_MATCHING.
- `Bog::Bog` 0x000DF3E8 +0x36 -> 0x0015AD60 / 0x00544300 `_D3DResource_Register@8`: FAIL, not masked-equal. No pin;
  symbols.txt already names 0x0015AD60 `_D3DResource_Register@8` (proto-pdb), so the row's reloc resolves anyway.
- Sizes: symbols.txt Ghidra rows equal the proto_map lengths (12, 28, 81, 5).
- 3.4 T12.c1: `SoundStreamer::Pause` 0x000D8BFB +0x2B (proto 0x004C41F8, near-identical, same call offset) -> 0x00179AD5 /
  0x00563075 `_IDirectSoundStream_Pause@8`, 0x5. First run: callee-name-conflict (outgoing retail name xbsymdb
  `CDirectSoundStream_Pause`); that target checked itself: 0x00179435 / 0x005629D5 `?Pause@CDirectSoundStream@DirectSound@@QAGJK@Z`,
  0x51, PASS (outgoing `?DirectSoundEnterCriticalSection@@YGHXZ`, `?Pause@CMcpxStream@DirectSound@@QAEJK@Z` agree).
  Pinned both; the outer one then PASS (name_sites.callee via a scratch probe).
