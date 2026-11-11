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
- Unresolved relocs (retail field in parens): +0x1 DIR32 `__ehhandler$<sym>` (text$x static, 0x00152BDC; full name
  `__ehhandler$??$_Uninit_fill_n@PAUVector3@@IU1@V?$allocator@UVector3@@@std@@@std@@YAXPAUVector3@@IABU1@AAV?$allocator@UVector3@@@0@U_Nonscalar_ptr_iterator_tag@0@@Z`);
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
- Pinned 2.2 T9.c1: 0x001EC830 `$T318` (Main.obj at "/O1 /Oy", match_one `--listing`: `$T318 DD 0ffffffffH`, `push OFFSET FLAT:$T318`).

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

### SwampRenderer::SetRenderState D3D pins (4.3 T9.c48)
Addresses and names only, no bytes (D46). Retail `?SetRenderState@SwampRenderer@@QAEXW4_D3DRENDERSTATETYPE@@K@Z`
0x00028D96 (Hannah.obj, 641 B) = proto 0x00417BCE, proc length 641, masked-equal (pin_rows `Images.match`). The body is
the XDK inline `D3DDevice_SetRenderState`; symbols.txt names its callees/data with xbsymdb undecorated labels (C0192).
- Cross-reference chain (E8 rel32 at the same caller offset, both images): +0x234 -> retail 0x001575A0 / proto
  0x00540B40 `_D3DDevice_SetRenderState_OcclusionCullEnable@4`; +0x245 -> 0x00157610 / 0x00540BB0
  `_D3DDevice_SetRenderState_StencilCullEnable@4`; +0x256 -> 0x00157680 / 0x00540C20
  `_D3DDevice_SetRenderState_RopZCmpAlwaysRead@4`; +0x267 -> 0x001576A0 / 0x00540C40 `_D3DDevice_SetRenderState_RopZRead@4`;
  +0x278 -> 0x001576C0 / 0x00540C60 `_D3DDevice_SetRenderState_DoNotCullUncompressed@4`. Each proto target is the single
  PDB public.
- Callee bytes: the three 0x11 callees masked-equal (allow_const off); the callee route refuses them only on
  callee-name-rule: their abs operand's retail target carries an xbsymdb interior label `D3DRS_<state>` inside
  D3D__RenderState (proto target = `_D3D__RenderState` + 4*state, no public). The two 0x62 callees masked-equal with
  constants allowed (retail +0x52 dword reads as an address, no proto .reloc entry, raw dwords equal). Sizes = symbols.txt
  rows = proto proc lengths (98, 17). Hence tag `retail-xref` (hand route), not `site-corr+callee-bytes`.
- `_D3D__RenderState` 0x001674E0 (xbsymdb `D3D_g_RenderState`): `site-bundle`, passes the D113 bundle route.
- `_D3DDIRTYFROMRENDERSTATE` 0x001C7410 = proto 0x005B09B0: 0xB0 bytes (0x2C entries, states 0x5C..0x87) equal in both
  images; extent = the gap to the next proto public. The compiler addresses it as `DIRTY[state - 0x5C]` =
  disp32 `_D3DDIRTYFROMRENDERSTATE - 0x170`, landing on retail 0x001C72A0 / proto 0x005B0840, both
  `__imp__KeInitializeTimerEx@8` (existing pin kept: the address is that import slot). No direct site reaches 0x001C7410,
  so `site-bundle` fails callers-below-2; tag `site-corr+data-bytes` as sibling `_D3DSIMPLERENDERSTATEENCODE` 0x001C74C0.

### unzlocal_GetCurrentFileInfoInternal 0x000388BB (4.3 T15)
Addresses and names only, no bytes (D46). Callee of 3 pool rows (naming_residue.md: `no-public 3`); 4.2 T10 left it
residue because the PDB names it only undecorated and the `_` form had not been read from the calltype (D78).
- Name: prototype default.pdb module stream of `Xbox\Official_Release\unzip.obj`, S_LPROC32 (static; no public)
  `unzlocal_GetCurrentFileInfoInternal`, seg 1 off 0x26BB2 (proto 0x00426FB2), length 682; its typind is an LF_PROCEDURE
  with calltype 0x00 = CV_CALL_NEAR_C (cdecl), 9 parameters, funcattr 0. cdecl decoration -> `_unzlocal_GetCurrentFileInfoInternal`.
  Read with a scratch reader over `pdb_types.load_tpi` / `PROC_KINDS` (4.3 T15, not kept).
- Bytes: proto_map class `identical`, one target 0x000388BB, length 682, masked 25 (derived/proto_map/retail-xbe/functions.jsonl);
  Ghidra entry 0x000388BB size 682 (`FUN_000388bb`). Size 0x2AA = proc length.
- Pin: `0x000388BB func 0x2AA _unzlocal_GetCurrentFileInfoInternal retail-disasm disasm-verified` (hand route, pin_rows TAGS).

### Vtable slot pins at 0x000164B3 (4.4 T4.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?Update@LoadScreen@@UAEX_NM@Z`: retail ??_7LoadScreen@@6B@ slot 3 = 0x000164B3; the same proto vtable slots = proto 0x004056A7, whose 2 publics include `?Update@LoadScreen@@UAEX_NM@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x000164B3 func - ?Update@LoadScreen@@UAEX_NM@Z proto-pdb-public+retail-disasm disasm-verified`

### Vtable slot pins at 0x00017E42 (4.4 T4.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?TestActivate@SwampScript@@UBE_NXZ`: retail ??_7AssDoorRegionMain@@6B@ slot 1, ??_7BasketStopperMain@@6B@ slot 1, ??_7BasketStopperPanMain@@6B@ slot 1, ??_7BcboStartBattleRegionMain@@6B@ slot 1, ??_7CameraHelperRegionMain@@6B@ slot 1, ??_7CarnivalLevelMain@@6B@ slot 1 (+12 more) = 0x00017E42; the same proto vtable slots = proto 0x004070A8, whose 2 publics include `?TestActivate@SwampScript@@UBE_NXZ`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x00017E42 func - ?TestActivate@SwampScript@@UBE_NXZ proto-pdb-public+retail-disasm disasm-verified`

### Vtable slot pins at 0x00017E45 (4.4 T4.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?AnimationTrigger@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@MMI@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 10, ??_7BasketStopperMain@@6B@ slot 10, ??_7BasketStopperPanMain@@6B@ slot 10, ??_7BcboStartBattleRegionMain@@6B@ slot 10, ??_7CameraHelperRegionMain@@6B@ slot 10, ??_7CarnivalLevelMain@@6B@ slot 10 (+11 more) = 0x00017E45; the same proto vtable slots = proto 0x004070AB, whose 3 publics include `?AnimationTrigger@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@MMI@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x00017E45 func - ?AnimationTrigger@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@MMI@Z proto-pdb-public+retail-disasm disasm-verified`

### Vtable slot pins at 0x00017E48 (4.4 T4.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?LosingControl@SwampScript@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 11, ??_7BasketStopperMain@@6B@ slot 11, ??_7BasketStopperPanMain@@6B@ slot 11, ??_7BcboStartBattleRegionMain@@6B@ slot 11, ??_7CameraHelperRegionMain@@6B@ slot 11, ??_7CarnivalLevelMain@@6B@ slot 11 (+12 more) = 0x00017E48; the same proto vtable slots = proto 0x004070AE, whose 1 publics include `?LosingControl@SwampScript@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`; proto proc length 5, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x00017E48 func - ?LosingControl@SwampScript@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z proto-pdb-public+retail-disasm disasm-verified`

### Vtable slot pins at 0x00017E4D (4.4 T4.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?CollidedWith@SwampScript@@UAEXW4CollisionType@1@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 19, ??_7BasketStopperMain@@6B@ slot 19, ??_7BasketStopperPanMain@@6B@ slot 19, ??_7BcboStartBattleRegionMain@@6B@ slot 19, ??_7CameraHelperRegionMain@@6B@ slot 19, ??_7CarnivalLevelMain@@6B@ slot 19 (+12 more) = 0x00017E4D; the same proto vtable slots = proto 0x005028C8, whose 9 publics include `?CollidedWith@SwampScript@@UAEXW4CollisionType@1@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?HitBy@SwampScript@@UAEXW4CollisionType@1@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 20, ??_7BasketStopperMain@@6B@ slot 20, ??_7BasketStopperPanMain@@6B@ slot 20, ??_7BcboStartBattleRegionMain@@6B@ slot 20, ??_7CameraHelperRegionMain@@6B@ slot 20, ??_7CarnivalLevelMain@@6B@ slot 20 (+12 more) = 0x00017E4D; the same proto vtable slots = proto 0x005028C8, whose 9 publics include `?HitBy@SwampScript@@UAEXW4CollisionType@1@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Message@SwampScript@@UAEXPAV1@HPAX@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 13, ??_7BasketStopperMain@@6B@ slot 13, ??_7BasketStopperPanMain@@6B@ slot 13, ??_7CameraHelperRegionMain@@6B@ slot 13, ??_7CutsceneAnimationScript@@6B@ slot 13, ??_7FireRegionMain@@6B@ slot 13 (+3 more) = 0x00017E4D; the same proto vtable slots = proto 0x005028C8, whose 9 publics include `?Message@SwampScript@@UAEXPAV1@HPAX@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x00017E4D func - ?CollidedWith@SwampScript@@UAEXW4CollisionType@1@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x00017E4D func - ?HitBy@SwampScript@@UAEXW4CollisionType@1@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x00017E4D func - ?Message@SwampScript@@UAEXPAV1@HPAX@Z proto-pdb-public+retail-disasm disasm-verified`

### Vtable slot pins at 0x000946F7 (4.4 T4.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?EndOfTransition@LoadScreen@@UAEX_N@Z`: retail ??_7LoadScreen@@6B@ slot 5 = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?EndOfTransition@LoadScreen@@UAEX_N@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?EnteringRegion@SwampScript@@UAEXPAVSwampSceneNode@@@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 14, ??_7BasketStopperMain@@6B@ slot 14, ??_7BasketStopperPanMain@@6B@ slot 14, ??_7BcboStartBattleRegionMain@@6B@ slot 14, ??_7CutsceneAnimationScript@@6B@ slot 14, ??_7SwampScript@@6B@ slot 14 (+4 more) = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?EnteringRegion@SwampScript@@UAEXPAVSwampSceneNode@@@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?GainingControl@SwampScript@@UAEXPAV1@@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 12, ??_7BasketStopperMain@@6B@ slot 12, ??_7BasketStopperPanMain@@6B@ slot 12, ??_7BcboStartBattleRegionMain@@6B@ slot 12, ??_7CameraHelperRegionMain@@6B@ slot 12, ??_7CarnivalLevelMain@@6B@ slot 12 (+12 more) = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?GainingControl@SwampScript@@UAEXPAV1@@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?LeavingRegion@SwampScript@@UAEXPAVSwampSceneNode@@@Z`: retail ??_7BasketStopperMain@@6B@ slot 15, ??_7BasketStopperPanMain@@6B@ slot 15, ??_7CutsceneAnimationScript@@6B@ slot 15, ??_7CutsceneRegionMain@@6B@ slot 15, ??_7SwampScript@@6B@ slot 15, ??_7UIClusterMain@@6B@ slot 15 (+3 more) = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?LeavingRegion@SwampScript@@UAEXPAVSwampSceneNode@@@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?MovieFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 18, ??_7BasketStopperMain@@6B@ slot 18, ??_7BasketStopperPanMain@@6B@ slot 18, ??_7BcboStartBattleRegionMain@@6B@ slot 18, ??_7CameraHelperRegionMain@@6B@ slot 18, ??_7CarnivalLevelMain@@6B@ slot 18 (+12 more) = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?MovieFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Render@SwampSceneNode@@MBEXABUMatrix@@@Z`: retail ??_7AmbientSound@@6B@ slot 8, ??_7GameCam@@6B@ slot 8, ??_7RegionTrigger@@6B@ slot 8 = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?Render@SwampSceneNode@@MBEXABUMatrix@@@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?SetState@CutsceneDriver@@MAEXH@Z` (5.4 T11.1.c2): retail ??_7CutsceneDriver@@6B@ (0x001EFA08) slot 29 (+0x74) = 0x000946F7 (va_dis vt); proto ??_7CutsceneDriver@@6B@ 0x005D85E8 slot +0x74 = proto 0x004FD174, whose 28 publics include `?SetState@CutsceneDriver@@MAEXH@Z`; same body as the pins above. Pin: `0x000946F7 func - ?SetState@CutsceneDriver@@MAEXH@Z proto-pdb-public+retail-disasm disasm-verified`.
- `?SoundFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 16, ??_7BasketStopperMain@@6B@ slot 16, ??_7BasketStopperPanMain@@6B@ slot 16, ??_7BcboStartBattleRegionMain@@6B@ slot 16, ??_7CameraHelperRegionMain@@6B@ slot 16, ??_7CarnivalLevelMain@@6B@ slot 16 (+12 more) = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?SoundFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?SoundStreamerFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 17, ??_7BasketStopperMain@@6B@ slot 17, ??_7BasketStopperPanMain@@6B@ slot 17, ??_7BcboStartBattleRegionMain@@6B@ slot 17, ??_7CameraHelperRegionMain@@6B@ slot 17, ??_7CarnivalLevelMain@@6B@ slot 17 (+12 more) = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?SoundStreamerFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?TimerEvent@SwampScript@@UAEXI@Z`: retail ??_7AssDoorRegionMain@@6B@ slot 5, ??_7BasketStopperMain@@6B@ slot 5, ??_7BasketStopperPanMain@@6B@ slot 5, ??_7BcboStartBattleRegionMain@@6B@ slot 5, ??_7CameraHelperRegionMain@@6B@ slot 5, ??_7CutsceneAnimationScript@@6B@ slot 5 (+7 more) = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?TimerEvent@SwampScript@@UAEXI@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?UpdateAnimation@SwampSceneNode@@MAEXI@Z`: retail ??_7AmbientSound@@6B@ slot 6, ??_7EyeUI@@6B@ slot 6, ??_7FrontEndUI@@6B@ slot 6, ??_7LaundryBitsHud@@6B@ slot 6, ??_7RegionTrigger@@6B@ slot 6 = 0x000946F7; the same proto vtable slots = proto 0x004FD174, whose 28 publics include `?UpdateAnimation@SwampSceneNode@@MAEXI@Z`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x000946F7 func - ?EndOfTransition@LoadScreen@@UAEX_N@Z proto-pdb-public+retail-disasm disasm-verified`; `0x000946F7 func - ?EnteringRegion@SwampScript@@UAEXPAVSwampSceneNode@@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x000946F7 func - ?GainingControl@SwampScript@@UAEXPAV1@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x000946F7 func - ?LeavingRegion@SwampScript@@UAEXPAVSwampSceneNode@@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x000946F7 func - ?MovieFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x000946F7 func - ?Render@SwampSceneNode@@MBEXABUMatrix@@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x000946F7 func - ?SoundFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x000946F7 func - ?SoundStreamerFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x000946F7 func - ?TimerEvent@SwampScript@@UAEXI@Z proto-pdb-public+retail-disasm disasm-verified`; `0x000946F7 func - ?UpdateAnimation@SwampSceneNode@@MAEXI@Z proto-pdb-public+retail-disasm disasm-verified`

### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?Activate@GameCam@@UAEXXZ`: retail ??_7GameCam@@6B@ slot 12 = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?Activate@GameCam@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Activate@SwampScript@@UAEXXZ`: retail ??_7AssDoorRegionMain@@6B@ slot 2, ??_7BcboStartBattleRegionMain@@6B@ slot 2, ??_7CameraHelperRegionMain@@6B@ slot 2, ??_7CarnivalLevelMain@@6B@ slot 2, ??_7CutsceneAnimationScript@@6B@ slot 2, ??_7CutsceneRegionMain@@6B@ slot 2 (+10 more) = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?Activate@SwampScript@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?AnimationFinished@SwampScript@@UAEXXZ`: retail ??_7AssDoorRegionMain@@6B@ slot 6, ??_7BcboStartBattleRegionMain@@6B@ slot 6, ??_7CameraHelperRegionMain@@6B@ slot 6, ??_7CarnivalLevelMain@@6B@ slot 6, ??_7CutsceneRegionMain@@6B@ slot 6, ??_7FireRegionMain@@6B@ slot 6 (+7 more) = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?AnimationFinished@SwampScript@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?AnimationInterrupted@SwampScript@@UAEXXZ`: retail ??_7AssDoorRegionMain@@6B@ slot 8, ??_7BasketStopperMain@@6B@ slot 8, ??_7BasketStopperPanMain@@6B@ slot 8, ??_7BcboStartBattleRegionMain@@6B@ slot 8, ??_7CameraHelperRegionMain@@6B@ slot 8, ??_7CarnivalLevelMain@@6B@ slot 8 (+12 more) = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?AnimationInterrupted@SwampScript@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Deactivate@SwampScript@@UAEXXZ`: retail ??_7AssDoorRegionMain@@6B@ slot 3, ??_7BasketStopperMain@@6B@ slot 3, ??_7BasketStopperPanMain@@6B@ slot 3, ??_7BcboStartBattleRegionMain@@6B@ slot 3, ??_7CameraHelperRegionMain@@6B@ slot 3, ??_7CarnivalLevelMain@@6B@ slot 3 (+12 more) = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?Deactivate@SwampScript@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?FrameTick@SwampScript@@UAEXXZ`: retail ??_7AssDoorRegionMain@@6B@ slot 4, ??_7BasketStopperMain@@6B@ slot 4, ??_7BasketStopperPanMain@@6B@ slot 4, ??_7BcboStartBattleRegionMain@@6B@ slot 4, ??_7CameraHelperRegionMain@@6B@ slot 4, ??_7CarnivalLevelMain@@6B@ slot 4 (+9 more) = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?FrameTick@SwampScript@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?LosingControl@GameCam@@UAEXXZ`: retail ??_7GameCam@@6B@ slot 13 = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?LosingControl@GameCam@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?SecondaryAnimationFinished@SwampScript@@UAEXXZ`: retail ??_7AssDoorRegionMain@@6B@ slot 7, ??_7BasketStopperMain@@6B@ slot 7, ??_7BasketStopperPanMain@@6B@ slot 7, ??_7BcboStartBattleRegionMain@@6B@ slot 7, ??_7CameraHelperRegionMain@@6B@ slot 7, ??_7CarnivalLevelMain@@6B@ slot 7 (+12 more) = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?SecondaryAnimationFinished@SwampScript@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?SecondaryAnimationInterrupted@SwampScript@@UAEXXZ`: retail ??_7AssDoorRegionMain@@6B@ slot 9, ??_7BasketStopperMain@@6B@ slot 9, ??_7BasketStopperPanMain@@6B@ slot 9, ??_7BcboStartBattleRegionMain@@6B@ slot 9, ??_7CameraHelperRegionMain@@6B@ slot 9, ??_7CarnivalLevelMain@@6B@ slot 9 (+12 more) = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?SecondaryAnimationInterrupted@SwampScript@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Update@LoadScreen@@UAEXXZ`: retail ??_7LoadScreen@@6B@ slot 4 = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?Update@LoadScreen@@UAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Update@SwampSceneNode@@MAEXXZ`: retail ??_7RegionTrigger@@6B@ slot 7 = 0x0011C7BC; the same proto vtable slots = proto 0x004C7652, whose 22 publics include `?Update@SwampSceneNode@@MAEXXZ`; proto proc length 1, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x0011C7BC func - ?Activate@GameCam@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?Activate@SwampScript@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?AnimationFinished@SwampScript@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?AnimationInterrupted@SwampScript@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?Deactivate@SwampScript@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?FrameTick@SwampScript@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?LosingControl@GameCam@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?SecondaryAnimationFinished@SwampScript@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?SecondaryAnimationInterrupted@SwampScript@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?Update@LoadScreen@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0011C7BC func - ?Update@SwampSceneNode@@MAEXXZ proto-pdb-public+retail-disasm disasm-verified`

### Throw-info data for length_error / out_of_range (4.4 T4.c2)
Retail throw sites `?_Xlen@...` 0x0001D5C2 and 0x00139A45 push `__TI3?AVlength_error@std@@` 0x001FCFAC (pinned) and call
`__CxxThrowException@8` 0x0014161F. Walk per `_ThrowInfo` (attributes, pmfnUnwind, pForwardCompat, pCatchableTypeArray),
`_CatchableTypeArray` (count, CT*), `_CatchableType` (properties, pType, PMD, size, copyFunction), `TypeDescriptor` (vftable,
spare, name), proto default.exe from the PDB public of each `__TI3` and retail from the pinned `__TI3` VA, field by field:
attributes, forward-compat, counts (3, 3), CT properties, PMD, sizes, TD spare and TD name equal on both sides; unwind and
copy functions resolve to the same names (retail via symbols.txt/symbols_pin.txt; the two length_error functions by masked
proc bytes); every TD vftable is `??_7type_info@@6B@` in proto and 0x001FB6FC in retail. Names are the proto PDB publics at
the matched proto elements:
- `__TI3?AVlength_error@std@@` +0xC: `__CTA3?AVlength_error@std@@` 0x001FCF9C; its CT[0..2]:
  `__CT??_R0?AVlength_error@std@@@8??0length_error@std@@QAE@ABV01@@Z40` 0x001FCF80,
  `__CT??_R0?AVlogic_error@std@@@8??0logic_error@std@@QAE@ABV01@@Z40` 0x001FCED8,
  `__CT??_R0?AVexception@@@8??0exception@@QAE@ABV0@@Z12` 0x001FCEBC.
- `__TI3?AVout_of_range@std@@` 0x001FCF20 +0xC: `__CTA3?AVout_of_range@std@@` 0x001FCF10; CT[0]
  `__CT??_R0?AVout_of_range@std@@@8??0out_of_range@std@@QAE@ABV01@@Z40` 0x001FCEF4; CT[1], CT[2] the same two VAs as above.
- CT+4: `??_R0?AVlength_error@std@@@8` 0x00204D08, `??_R0?AVout_of_range@std@@@8` 0x00204778,
  `??_R0?AVlogic_error@std@@@8` 0x00204758, `??_R0?AVexception@@@8` 0x00204740.
- TD+0: `??_7type_info@@6B@` 0x001FB6FC. `??_7exception@@6B@` 0x001FB6E0: the vtable store in retail
  `??0exception@@QAE@XZ` 0x00141658; proto `??_7exception@@6B@` and `??_7type_info@@6B@` sit 0x1C apart, as in retail.

## Vtable slot and wall pins (5.2 T6.c1)

### Vtable slot pass (5.2 T6.c1)

Population: site-corr `??_7` pins carrying pool rows (candidates.jsonl blockers via naming_levers.build) plus the 9 wall-vtable-relink classes' vtables: 196 vtables, 490 retail slots with no table name or only a `boundary_` label, 296 distinct retail targets.
Name source: proto `??_7X@@6B@` public in default.exe, slot dword k -> proto proc -> its publics. One public: that name. Several (folded stubs): the publics whose class is X or a PDB base of X (TPI LF_BCLASS chain) for every retail vtable X whose slot k reaches the target.
Target verdicts: {'candidate': 138, 'bytes-differ': 154, 'proto-proc-at-2-retail-vas': 4}. `proto-proc-at-2-retail-vas`: one proto proc reached at 2+ retail VAs (no fold on the retail side), left unnamed.
Kept after replant: 429 lines at 137 VAs: callee-bytes 6, fold-alias 408, hand 15. Boundary labels replaced: 7.
Vtables with every missing slot now named: 94; still missing some (bytes-differ targets): 89.

Vtable routes after the pins (each site-corr `??_7` line retagged in a copy, pin_rows replant):
- `+slot-bytes` corr-verified (D113): pass 0 (the route reads slot bytes, not slot names; the pins change nothing).
- `+vtable-slots` disasm-verified: pass 0.
- name_sites route_one (pointer-name agreement, the naming_levers lens): pass 20 new: ??_7WindyBits@@6B@, ??_7FanBoatWake@@6B@, ??_7FadeToBlack@@6B@, ??_7LightBeam@@6B@, ??_7FFlyMinMaxCtrlV3@@6B@, ??_7VinceGiblets@@6B@, ??_7FogZone@@6B@, ??_7SearchlightBeam@@6B@, ??_7GatorNode@@6B@, ??_7CollidableCylinder@@6B@, ??_7FireFlyNodeClone@@6B@, ??_7LightNode@@6B@, ??_7HeatShimmer@@6B@, ??_7SwampLightningFlash@@6B@, ??_7DebugGeometryNode@@6B@, ??_7GeometryNode@@6B@, ??_7Decal@@6B@, ??_7FloatyBits@@6B@, ??_7SwampWaterNode@@6B@, ??_7SwampPuddle@@6B@. No pin_rows tag accepts them: retagged `+data-bytes` (promote-data's PROMO_SRC) each fails `route-global:name-kind-vs-tag`.
Freed pool rows (all blocker keys in the set): slot pins alone 0; + slot-bytes vtables 0; + route_one vtables 23 (hypothetical: needs a vtable route by slot-name agreement in pin_rows, a locked tool).

#### Vtable slot pins at 0x000180E1 (5.2 T6.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?GetGravity@Critter@@UAEMXZ`: retail ??_7Beret@@6B@ slot 16, ??_7GingerDeadMan@@6B@ slot 16 = 0x000180E1; the same proto vtable slots = proto 0x004073B0, whose 1 publics include `?GetGravity@Critter@@UAEMXZ`; proto proc length 7, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x000180E1 func - ?GetGravity@Critter@@UAEMXZ proto-pdb-public+retail-disasm disasm-verified`

#### Vtable slot pins at 0x00037178 (5.2 T6.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?Open@MemoryFileHandler@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z`: retail ??_7ZipFileHandler@@6B@ slot 3, ??_7MemoryFileHandler@@6B@ slot 0, ??_7MemoryFileHandler@@6B@ slot 3 = 0x00037178; the same proto vtable slots = proto 0x00425886, whose 3 publics include `?Open@MemoryFileHandler@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z`; proto proc length 5, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Write@MemoryFileHandler@@UAE_NPBXI@Z`: retail ??_7ZipFileHandler@@6B@ slot 3, ??_7MemoryFileHandler@@6B@ slot 0, ??_7MemoryFileHandler@@6B@ slot 3 = 0x00037178; the same proto vtable slots = proto 0x00425886, whose 3 publics include `?Write@MemoryFileHandler@@UAE_NPBXI@Z`; proto proc length 5, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Write@ZipFileHandler@@UAE_NPBXI@Z`: retail ??_7ZipFileHandler@@6B@ slot 3, ??_7MemoryFileHandler@@6B@ slot 0, ??_7MemoryFileHandler@@6B@ slot 3 = 0x00037178; the same proto vtable slots = proto 0x00425886, whose 3 publics include `?Write@ZipFileHandler@@UAE_NPBXI@Z`; proto proc length 5, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x00037178 func - ?Open@MemoryFileHandler@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z proto-pdb-public+retail-disasm disasm-verified`; `0x00037178 func - ?Write@MemoryFileHandler@@UAE_NPBXI@Z proto-pdb-public+retail-disasm disasm-verified`; `0x00037178 func - ?Write@ZipFileHandler@@UAE_NPBXI@Z proto-pdb-public+retail-disasm disasm-verified`

#### Vtable slot pins at 0x0003735F (5.2 T6.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?Size@AsyncFileHandler@@UAEIXZ`: retail ??_7ZipFileHandler@@6B@ slot 5, ??_7AsyncFileHandler@@6B@ slot 5, ??_7MemoryFileHandler@@6B@ slot 5 = 0x0003735F; the same proto vtable slots = proto 0x00425A4E, whose 3 publics include `?Size@AsyncFileHandler@@UAEIXZ`; proto proc length 4, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Size@MemoryFileHandler@@UAEIXZ`: retail ??_7ZipFileHandler@@6B@ slot 5, ??_7AsyncFileHandler@@6B@ slot 5, ??_7MemoryFileHandler@@6B@ slot 5 = 0x0003735F; the same proto vtable slots = proto 0x00425A4E, whose 3 publics include `?Size@MemoryFileHandler@@UAEIXZ`; proto proc length 4, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Size@ZipFileHandler@@UAEIXZ`: retail ??_7ZipFileHandler@@6B@ slot 5, ??_7AsyncFileHandler@@6B@ slot 5, ??_7MemoryFileHandler@@6B@ slot 5 = 0x0003735F; the same proto vtable slots = proto 0x00425A4E, whose 3 publics include `?Size@ZipFileHandler@@UAEIXZ`; proto proc length 4, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x0003735F func - ?Size@AsyncFileHandler@@UAEIXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0003735F func - ?Size@MemoryFileHandler@@UAEIXZ proto-pdb-public+retail-disasm disasm-verified`; `0x0003735F func - ?Size@ZipFileHandler@@UAEIXZ proto-pdb-public+retail-disasm disasm-verified`

#### Vtable slot pins at 0x00037363 (5.2 T6.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?Pos@AsyncFileHandler@@UAEHXZ`: retail ??_7ZipFileHandler@@6B@ slot 6, ??_7AsyncFileHandler@@6B@ slot 6, ??_7MemoryFileHandler@@6B@ slot 6 = 0x00037363; the same proto vtable slots = proto 0x00425A52, whose 3 publics include `?Pos@AsyncFileHandler@@UAEHXZ`; proto proc length 4, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Pos@MemoryFileHandler@@UAEHXZ`: retail ??_7ZipFileHandler@@6B@ slot 6, ??_7AsyncFileHandler@@6B@ slot 6, ??_7MemoryFileHandler@@6B@ slot 6 = 0x00037363; the same proto vtable slots = proto 0x00425A52, whose 3 publics include `?Pos@MemoryFileHandler@@UAEHXZ`; proto proc length 4, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?Pos@ZipFileHandler@@UAEHXZ`: retail ??_7ZipFileHandler@@6B@ slot 6, ??_7AsyncFileHandler@@6B@ slot 6, ??_7MemoryFileHandler@@6B@ slot 6 = 0x00037363; the same proto vtable slots = proto 0x00425A52, whose 3 publics include `?Pos@ZipFileHandler@@UAEHXZ`; proto proc length 4, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x00037363 func - ?Pos@AsyncFileHandler@@UAEHXZ proto-pdb-public+retail-disasm disasm-verified`; `0x00037363 func - ?Pos@MemoryFileHandler@@UAEHXZ proto-pdb-public+retail-disasm disasm-verified`; `0x00037363 func - ?Pos@ZipFileHandler@@UAEHXZ proto-pdb-public+retail-disasm disasm-verified`

#### Vtable slot pins at 0x0006972D (5.2 T6.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?AnimationInterrupted@NetMain@@UAEXXZ`: retail ??_7NetMain@@6B@ slot 8 = 0x0006972D; the same proto vtable slots = proto 0x00455C5D, whose 1 publics include `?AnimationInterrupted@NetMain@@UAEXXZ`; proto proc length 5, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x0006972D func - ?AnimationInterrupted@NetMain@@UAEXXZ proto-pdb-public+retail-disasm disasm-verified`

#### Vtable slot pins at 0x00078E59 (5.2 T6.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?CanJorgeCollideWithMe@CollidableNode@@UBE_NPBVSwampSceneNode@@ABUVector3@@@Z`: retail ??_7VinceGiblets@@6B@ slot 10, ??_7VinceGiblets@@6B@ slot 11, ??_7CollidableNode@@6B@ slot 10, ??_7CollidableNode@@6B@ slot 11, ??_7GatorNode@@6B@ slot 10, ??_7GatorNode@@6B@ slot 11 (+2 more) = 0x00078E59; the same proto vtable slots = proto 0x0045C0D1, whose 3 publics include `?CanJorgeCollideWithMe@CollidableNode@@UBE_NPBVSwampSceneNode@@ABUVector3@@@Z`; proto proc length 5, proto and retail bytes masked-equal over it (pin_rows Images.match).
- `?DoesJorgeRepelMe@CollidableNode@@UBE_NPBVSwampSceneNode@@_N@Z`: retail ??_7VinceGiblets@@6B@ slot 10, ??_7VinceGiblets@@6B@ slot 11, ??_7CollidableNode@@6B@ slot 10, ??_7CollidableNode@@6B@ slot 11, ??_7GatorNode@@6B@ slot 10, ??_7GatorNode@@6B@ slot 11 (+2 more) = 0x00078E59; the same proto vtable slots = proto 0x0045C0D1, whose 3 publics include `?DoesJorgeRepelMe@CollidableNode@@UBE_NPBVSwampSceneNode@@_N@Z`; proto proc length 5, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x00078E59 func - ?CanJorgeCollideWithMe@CollidableNode@@UBE_NPBVSwampSceneNode@@ABUVector3@@@Z proto-pdb-public+retail-disasm disasm-verified`; `0x00078E59 func - ?DoesJorgeRepelMe@CollidableNode@@UBE_NPBVSwampSceneNode@@_N@Z proto-pdb-public+retail-disasm disasm-verified`

#### Vtable slot pins at 0x0013240F (5.2 T6.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?HitBy@ClownMissileMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z`: retail ??_7ClownMissileMain@@6B@ slot 20 = 0x0013240F; the same proto vtable slots = proto 0x0046B0DE, whose 4 publics include `?HitBy@ClownMissileMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z`; proto proc length 5, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x0013240F func - ?HitBy@ClownMissileMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z proto-pdb-public+retail-disasm disasm-verified`

#### Vtable slot pins at 0x00138E4E (5.2 T6.c1)
Addresses and names only, no bytes (D46). Hand route: callee-bytes refuses a stub under 8 B (callee-short); the slot evidence names it.
- `?GetDefaultState@BasicDoorMain@@UBEHXZ`: retail ??_7GateSmallMain@@6B@ slot 28, ??_7CrawdadTrapMain@@6B@ slot 28, ??_7DockSubMain@@6B@ slot 28, ??_7TrailerFrogsMain@@6B@ slot 28, ??_7EvilVinceMirrorMain@@6B@ slot 28, ??_7TeslaCoilPartMain@@6B@ slot 28 (+4 more) = 0x00138E4E; the same proto vtable slots = proto 0x00532768, whose 4 publics include `?GetDefaultState@BasicDoorMain@@UBEHXZ`; proto proc length 3, proto and retail bytes masked-equal over it (pin_rows Images.match).
- Pin: `0x00138E4E func - ?GetDefaultState@BasicDoorMain@@UBEHXZ proto-pdb-public+retail-disasm disasm-verified`

Slot targets left unnamed: 154 bytes-differ (proto slot proc not masked-equal at the retail target); 4 where one proto proc is reached at two retail VAs (retail did not fold): 0x000A5460 and 0x00051F8C (proto 0x00460492; ??_7FileHandler slot 4, ??_7GeneratorMain slot 1), 0x00069D6B (proto 0x004070A8, also at pinned 0x00017E42; ??_7NetMain slot 1), 0x00069917 (proto 0x004C7652, also at pinned 0x0011C7BC; ??_7NetMain slot 2).

### Wall-vtable-relink blockers (5.2 T6.c1)
C0256 follow-up list, each checked unnamed in symbols.txt and symbols_pin.txt (snapshot = live file at the run). Callers: symbols.txt funcs (pin_rows by_rd) and accepted func pin rows, masked-equal at the proto proc length, outgoing dest equal to the VA; the proto dest's publics name it.
- 0x00017F50 (wall ??0LevelScript): called by ??1LevelScript, ??_ELevelScript, ??_GLevelScript at proto dest 0x0040721F, 1 public `?_Tidy@?$vector@UProgressHint@LevelScript@@V?$allocator@UProgressHint@LevelScript@@@std@@@std@@IAEXXZ` (C0256 called it ~vector; the PDB says _Tidy); callee passes, proc length 0x2D. Pinned callee-bytes.
- 0x00070505 (wall ??0MainstreetRegionMain): retail ??_7MainstreetRegionMain slot 13; proto 0x00444272, 4 publics; the class's own `?Message@MainstreetRegionMain@@UAEXPAVSwampScript@@HPAX@Z`; callee passes. Pinned fold-alias (the other 3 publics' classes have no retail slot at this VA: not pinned).
- 0x00121879 (wall ??0VinceFlat): called by the retail ??_E rows named ??_EBasketStopperMain, ??_EBasketStopperPanMain, ??_EVinceFlat; proto dest 0x0051E9EE, 7 publics; pinned the dtors of the callers' classes: `??1VinceFlat@@UAE@XZ`, `??1BasketStopperMain@@UAE@XZ` (fold-alias, callee passes, 0x19). The 5 WhammyResult* dtor aliases there have no retail caller named for their class: not pinned.
- 0x0008C43B (walls ??0VinceZapHat, ??0VinceCeilingFan): called by ??_EVinceCeilingFan, ??_EVinceZapHat, ??_GVinceCeilingFan; proto 0x00477F4F, 2 publics `??1VinceCeilingFan@@UAE@XZ`, `??1VinceZapHat@@UAE@XZ`; pinned both fold-alias (0xF). C0256: the folded ??_G emitted by two objects still needs a fold-copy check at relink.
- 0x000DAA98 (~BundleBackgroundLoader; wall ??0LoadScreen per C0256): called by ??_E/??_GBundleBackgroundLoader at proto dest 0x004C5FC9, 1 public `??1BundleBackgroundLoader@@UAE@XZ`; NOT masked-equal (first masked difference at +0x10; masks shift by 3 from the proto rel at +0x21 to retail +0x1E; 70 differing offsets after masking); allow_const match fails; name_sites near_check passes (sim 97.22, 2 agreeing anchors, 0 conflicts); Ghidra size 101, contiguous. The near-corr line `0x000DAA98 func 0x65 ??1BundleBackgroundLoader@@UAE@XZ proto-pdb-public+anchor-window+caller-xref near-corr` fails pin_rows `route-near:no-agreeing-site` (its callers are pin rows, not symbols.txt; NEAR_NO_SITE is in locked pin_rows). Not pinned; stays.
- 0x0012B64E (wall ??0PuddleRegionMain): callers ??_E/??_GPuddleRegionMain, proto 0x00514073 `?_Tidy@?$vector@USplasher@PuddleRegionMain@@...`, callee passes; the pin exists DISABLED (symbols_pin.txt:3274-3275, 4.4 T6.1: the re-cut call site 0x0012BC8C+0x7 has no reloc; name_sites ACCEPTED_SITES puddle). A cut reason, not evidence: left disabled.
- 0x001C94D4 (deque `_Xlen` string): already pinned `??_C@_0BC@KBDMBKEH@deque?$DMT?$DO?5too?5long?$AA@` literal-bytes disasm-verified (symbols_pin.txt:3874). Nothing to add.
- Walls not lifted by a pin (C0203, C0256 source walls): ??0RegionTrigger (access spelling, SwampSceneNode M vs U), ??0UIClusterMain (LNK2005 inline SetScale .debug$F COMDAT), ??0LoadScreen (also ~BundleBackgroundLoader above). Their vtables (all corr-verified) have no unnamed slot left.

### Site-corr ctor pins (5.2 T6.c1)
- 0x0005B952 `??0DoorExitZGCMain@@QAE@PAVSwampSceneNode@@@Z` (symbols_pin.txt:3583): caller ?creationFunc@DoorExitZGCMain (symbols.txt) reaches proto 0x00447E73, 1 public, proc length 98; pin_rows callee passes (D119 rule: one of 2+ proto publics agrees). naming_levers lists it callee-name-conflict under name_sites.callee's stricter rule (a proto dest with 2+ publics never agrees). Promote in place to `+site-corr+callee-bytes disasm-verified`, size 0x62.
- 0x0003C5A8 `??0DoorPulleyBucketMain@@QAE@PAVSwampSceneNode@@@Z` (symbols_pin.txt:3829): caller ?creationFunc@DoorPulleyBucketMain, proto 0x0042ACBC, 1 public, proc length 114; same verdicts; promote to callee-bytes disasm-verified, size 0x72.
- Each frees 1 pool row alone (2 rows, 81 B).

### Fold-VA relink walls (5.2 T6.c1)
- 0x0006A5D2 LanternMain::FrameTick (C0250): every public at proto 0x004569D0 (4: DoDebugUpdates, FrameTick of CutsceneDriver, LevelFlythroughCamMain, LanternMain) is already pinned fold-alias disasm-verified (symbols_pin.txt:2299-2302); retail slot 4 of KosmobotPreludeCutscene, LanternMain, CutsceneDriver and calls from KosmoJustDessertsCutscene/KosmoPigCutscene FrameTick agree. The wall is relink (`external lies in a replaced range`), not a missing name.
- 0x0001819A ??4ResourceHandle<VertexShaderResource> (C0250, C0154): all 7 publics at proto 0x004180D1 pinned fold-alias (symbols_pin.txt:2179-2185); 148 proto callers agree. The wall is relink overlap of a matched row with COMDAT copies at a fold VA.
- 0x0010F1FB FloorSwitchMain::HitBy (C0154; pool_buckets NOTES_VA): proto 0x004F9B19, 1 public, callee passes at the VA (symbols.txt row, unverified status). The refusal is its callee ResetPopupTimer 0x0007EA16, already pinned fold-alias for SwitchMain and SwitchSafeMain (symbols_pin.txt:2312-2313), lying in the replaced SwitchSafe range. No pin lifts any of the three.

### T15 needs pinned (5.4 T25.c1)
Hand lines (`proto-pdb-public+retail-disasm`; `site-corr+callee-bytes` refused first, reason given). Each: retail call
site in the T15 row (objdiff 100, bytes provisional) at the same call ordinal as the proto twin's call to the public's VA
(`va_dis.py dis <row> <len> <protoVA>`, `va_dis.py sym`). No bytes here (D46).
- `?Play@SoundStreamer@@QAEXXZ` 0x000D9104 (callee-bytes-differ): SoundStreamer::Stop 0x000D91A7 call @0x000D920D vs proto
  0x004C4782 call 0x004C46F3 (public at +0); SoundStreamer::Set 0x000D9519 @0x000D95B3 vs proto 0x004C4ACE -> 0x004C46F3;
  CutsceneDriver::BeginPlayback proto 0x004F32BF tail-jmp 0x004C46F3.
- `?Stopped@SoundManager@@AAEXPAVSoundStreamer@@@Z` 0x000D90CA (callee-bytes-differ): Stop 0x000D91A7 @0x000D91F4 (+0x4D)
  vs proto 0x004C47CF (+0x4D) -> 0x004C46B9 (the SoundStreamer* overload; the SoundNode* one is another VA).
- `?CancelRequests@SoundStreamer@@AAEXXZ` 0x000D8D1D (callee-bytes-differ): Stop 0x000D91A7 last call @0x000D9259 vs proto
  last call -> 0x004C4319.
- `?CancelRequest@DiskQueue@@QAEXPAI@Z` 0x000D3074 (callee-bytes-differ): MoviePlayer::Stop 0x000DA108 first call @0x000DA127
  vs proto 0x004C5630 first call -> 0x004BE34B.
- `??0MemoryFileHandler@@QAE@I@Z` 0x0003729B (callee-bytes-differ): BeepFile::Open 0x0003786D 3rd call @0x00037893 vs proto
  0x00425F41 3rd call -> 0x0042598A (the `@I@Z` overload).
- `?Update@DiskQueue@@QAEXXZ` 0x000D3314 (callee-bytes-differ): DiskQueue::AddReadRequest 0x000D37C0 4th call @0x000D3859 vs
  proto 0x004BEACA 4th call -> 0x004BE5F8.
- `??0HeavyObjectMain@@QAE@PAVSwampSceneNode@@@Z` 0x0010F0F8 (callee-bytes-differ): ??0GasCanisterMain 0x0009DCC3 first call
  @0x0009DCD0 (base ctor) vs proto 0x00488DBE -> 0x004F9A16.
- `_IDirectSoundBuffer_SetVolume@8` 0x001799C8 (callee-name-rule): SoundNode::SetVolume 0x000C5B67 last call @0x000C5C01 vs
  proto 0x004B0E6A last call -> 0x00562F68; symbols.txt holds the undecorated `IDirectSoundBuffer_SetVolume` (C0192).
- Auto-route lines (no bullet needed): push_back<LevelEntry> 0x0013EB42 callee-bytes; `__real@c53b8000` 0x001EB434
  real-bytes; `??_C` literals 0x001D9FD4, 0x001D9F50 (proto public at 0x005C30E0, bytes equal; proto ctor has one push),
  0x001E72F0 literal-bytes.
- Refused (left out): the 16 `??_7` vtables of the 5.4 T15 wall-vtable-relink rows: slot-names `slot-unnamed` (11),
  `slot-name-differs` (VinceLaundryCar, PipeGasMain, OvenMain, GasCanisterMain), `slot-site-corr` (MrsGCollapsedMain);
  vtable-slots `vtable-differs` (all 16). .data 0x0021EE68: the proto site (GetBeadDest 0x004CC326 fmul) is 0x00607880 =
  `?SETTINGS_FILE@@3PBDB`+4, no public at the VA (D78). 0x00238C28 skipped (no PDB public, D78).
- Promoted in place 5.4 T25.c4 (site-corr -> hand `proto-pdb-public+retail-disasm`; pin_rows i10 refused the T25 rows):
  `?SetVolume@SoundStreamer@@QAEXM@Z` 0x000D8C2D: MusicManager::Resume 0x00112A0B calls 3, 4 @0x00112A56, @0x00112A64 vs
  proto 0x004FD261 calls 3, 4 -> 0x004C422A (public at +0; calls 1, 2 -> Play on both sides). `__ftol2` 0x00141F74:
  SoundNode::SetVolume 0x000C5B67 @0x000C5BF4 (call before the last) vs proto 0x004B0E6A @0x004B0EF5 -> 0x0052B844
  (public at +0), the next call on both sides IDirectSoundBuffer_SetVolume.

### Row-end pins for the T25 cut-wall rows (5.4 T26.c1)
C0103: T = row VA + row size (a LAB_ the row's Ghidra body runs past), P = protoVA + (T - VA); proto publics and proc
lengths from default.pdb / proto functions, `name_sites.Images.masked_equal(P, T, proc length)`. No bytes here (D46).
- 0x000419CA `?creationFunc@ScareHouseMonsterMain@@...` (row 0x000419A0 +0x2A): P 0x0042FDD2, one public, masked-equal over
  proc length 42 -> public, sized 0x2A.
- Boundary (`boundary_%08X`; one public at P, bytes differ, no masked-equal site names T): 0x00121AFC (row 0x00121AD8,
  P 0x0050A7A6 PowerIconMain::HitBy), 0x0006C75B (0x0006C734, P 0x00458A43 BellMain::Message).
- Hand `proto-pdb-public+retail-disasm` (C0412; first written as boundaries, re-cut 1's gate failed pin_rows
  `route-registerme:callee-name-rule` on the RegisterMe pin whose site names T; callee bytes differ: new() file/line).
  Each: retail `??0RegisterMe@X` (pinned, masked-equal to its proto twin) +0x1A abs -> T vs proto twin +0x1A -> the public:
  - `?creationFunc@ReggieWreckageMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` 0x0008132B (row 0x00081304): RegisterMe 0x000815B9 vs proto 0x0046D4AB -> 0x0046D21D.
  - `?creationFunc@Beret@@CAPAVSwampSceneNode@@XZ` 0x000714F5 (row 0x000714CE): RegisterMe 0x000748A4 vs proto 0x00460986 -> 0x0045D677.
  - `?creationFunc@BeretMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` 0x00073A31 (row 0x00073A07): RegisterMe 0x000748E0 vs proto 0x004609C2 -> 0x0045FB6F.
  - `?creationFunc@MasqueradeMask@@CAPAVSwampSceneNode@@XZ` 0x0007447F (row 0x00074455): RegisterMe 0x0007464C vs proto 0x0046072E -> 0x00460591.
  - `?creationFunc@MoleGateMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` 0x0005FC89 (row 0x0005FC5F): RegisterMe 0x0005FF2C vs proto 0x0044C3E1 -> 0x0044C0F3
    (the C0103 neighbour P 0x0044BFF7 is MoleTomb_BMain's: proto order differs here).
- Refused 0x00071811 (row 0x000717E7): P 0x0045D94C holds two publics (SetVisible@Beret, SetVisible@NeedleHat, folded;
  already pinned fold-alias at 0x0003FE34). RegisterMe@DialogueMovieRegionMain 0x00074D54 +0x1A -> T vs proto 0x00460E36
  -> 0x0045D922 `?creationFunc@DialogueMovieRegionMain@@...`, a name symbols.txt gives 0x000717E7 (the row itself):
  conflict, no pin (D78).

### RegisterMe class-name literals (5.4 T27.c1)
Route (D133, D78): each unmatched `??0RegisterMe@X@@QAE@XZ` candidate row whose +0x0A abs site target is its only
(or one of two, with +0x1A creationFunc) unresolved reloc: the dword at protoVA+0x0A names exactly one `??_C@` proto PDB
public; proto bytes == retail bytes at the target through the NUL; text == X; target VA has no symbols.txt / pin line and
the name sits at no other VA. Tag `proto-pdb-public+site-corr+literal-bytes disasm-verified` (T25 literal form, C0412).
No bytes or text here (D46).
- Candidates 386 (223 literal-only, 163 also creationFunc); pinned 386 (`# 5.4 T27.c1` block in symbols_pin.txt);
  refused 0 (no row hit proto-publics != 1, bytes-differ, text-not-class, va-has-line, name-at-other-va, or a pin_rows fail).
- pin_rows on the block before the re-cut (`State.replant`): 0 fails; gate pins layer after the re-cut: pass.
- Re-cut: `.rdata.obj` recut_cmp OTHER defsyms only: 386 Ghidra labels (class 3) become the pinned publics (class 2) at
  exactly the 386 newly pinned VAs; section bytes, relocs and targets equal (C0412). Delink and `.text` pairs rc 0.
- Sample: `??0RegisterMe@CannonMain@@QAE@XZ` 0x0005E231 (literal 0x001D7A58) by gen_registerme, objdiff 100, bytes match.

### I79 vtable slot names (5.5 T7.c1b)
Route (C0256, C0412, D78): each I79 `??_7X@@6B@` (retail VA = the imm32 of the vptr store in ctor `??0X`) pairs
proto slot k with retail slot k (proto and retail slot counts equal); a slot VA unnamed or holding only
`boundary_<va>` takes the proto slot-k public. callee() refuses these slots (masked bytes differ, or a stub under
8 B `callee-short`), so each is a hand row `proto-pdb-public+retail-disasm disasm-verified` listed in
config/pin_hand.txt. Differing-byte counts only, no bytes (D46).
- 0x0005DD7D ?FrameTick@CannonMain@@UAEXXZ: slot 4 of `??_7CannonMain@@6B@` 0x001D78E8 (ctor 0x0005D5AE); proto 0x0044A1BC; 11 of 80 bytes differ.
- 0x0005E09D ?HitBy@CannonMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z: slot 20 of `??_7CannonMain@@6B@` 0x001D78E8 (ctor 0x0005D5AE); proto 0x0044A4DC; 47 of 344 bytes differ.
- 0x0006BB7E ?Activate@MirrorUrnMain@@UAEXXZ: slot 2 of `??_7MirrorUrnMain@@6B@` 0x001DAB78 (ctor 0x0006B9B0); proto 0x00457F41; 12 of 12 bytes differ.
- 0x0006BC3D ?Message@MirrorUrnMain@@UAEXPAVSwampScript@@HPAX@Z: slot 13 of `??_7MirrorUrnMain@@6B@` 0x001DAB78 (ctor 0x0006B9B0); proto 0x00457F4D; 97 of 121 bytes differ.
- 0x0006BCC9 ?FrameTick@MirrorUrnMain@@UAEXXZ: slot 4 of `??_7MirrorUrnMain@@6B@` 0x001DAB78 (ctor 0x0006B9B0); proto 0x00457FC6; 371 of 520 bytes differ.
- 0x0007B761 ?Message@OvenMain@@UAEXPAVSwampScript@@HPAX@Z: slot 13 of `??_7OvenMain@@6B@` 0x001DEA50 (ctor 0x0007B2CF); proto 0x00467768; 91 of 638 bytes differ.
- 0x00081950 ?FrameTick@PipeGasMain@@UAEXXZ: slot 4 of `??_7PipeGasMain@@6B@` 0x001DFDE8 (ctor 0x000816C8); proto 0x0046D842; 401 of 501 bytes differ.
- 0x00085F5C ?LosingControl@VinceWalkOut@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z: slot 11 of `??_7VinceWalkOut@@6B@` 0x001F9BB0 (ctor 0x00136386); slot 11 of `??_7VinceDrinkjug@@6B@` 0x001F79D0 (ctor 0x00129D49); proto 0x00512741; 0 of 5 bytes differ, callee-short (raw-equal stub).
- 0x0009D8AF ?HitBy@GasCanisterMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z: slot 20 of `??_7GasCanisterMain@@6B@` 0x001E7280 (ctor 0x0009DCC3); proto 0x00488929; 6 of 42 bytes differ.
- 0x0009E6AC ?FrameTick@GasCanisterMain@@UAEXXZ: slot 4 of `??_7GasCanisterMain@@6B@` 0x001E7280 (ctor 0x0009DCC3); proto 0x00489787; 159 of 202 bytes differ.
- 0x000A0885 ?Message@ChuteKillerDilloMain@@UAEXPAVSwampScript@@HPAX@Z: slot 13 of `??_7ChuteKillerDilloMain@@6B@` 0x001E81C8 (ctor 0x000A0D59); proto 0x0048B909; 215 of 317 bytes differ.
- 0x000A3179 ?Message@VinceLaundryCar@@UAEXPAVSwampScript@@HPAX@Z: slot 13 of `??_7VinceLaundryCar@@6B@` 0x001E8798 (ctor 0x000A26B8); proto 0x0048E266; 120 of 764 bytes differ.
- 0x00115C5B ?AnimationFinished@PitFrogMain@@UAEXXZ: slot 6 of `??_7GateFrogMain@@6B@` 0x001DB8E8 (ctor 0x00071194); slot 6 of `??_7TrailerFrogMain@@6B@` 0x001CF6C0 (ctor 0x0003DE5D); slot 6 of `??_7FrogOfMysteryMain@@6B@` 0x001CEC78 (ctor 0x0003C202); proto 0x005004A9; 12 of 294 bytes differ.
- 0x00115D81 ?CollidedWith@PitFrogMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z: slot 19 of `??_7GateFrogMain@@6B@` 0x001DB8E8 (ctor 0x00071194); slot 19 of `??_7TrailerFrogMain@@6B@` 0x001CF6C0 (ctor 0x0003DE5D); slot 19 of `??_7FrogOfMysteryMain@@6B@` 0x001CEC78 (ctor 0x0003C202); proto 0x005005CF; 12 of 133 bytes differ.
- 0x00116013 ?TimerEvent@PitFrogMain@@UAEXI@Z: slot 5 of `??_7GateFrogMain@@6B@` 0x001DB8E8 (ctor 0x00071194); slot 5 of `??_7TrailerFrogMain@@6B@` 0x001CF6C0 (ctor 0x0003DE5D); slot 5 of `??_7FrogOfMysteryMain@@6B@` 0x001CEC78 (ctor 0x0003C202); proto 0x00500857; 67 of 732 bytes differ.
- 0x001162EF ?FrameTick@PitFrogMain@@UAEXXZ: slot 4 of `??_7GateFrogMain@@6B@` 0x001DB8E8 (ctor 0x00071194); slot 4 of `??_7TrailerFrogMain@@6B@` 0x001CF6C0 (ctor 0x0003DE5D); slot 4 of `??_7FrogOfMysteryMain@@6B@` 0x001CEC78 (ctor 0x0003C202); proto 0x00500B33; 84 of 1304 bytes differ.
- 0x0012359E ?CollidedWith@KillerDilloMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z: slot 19 of `??_7ChuteKillerDilloMain@@6B@` 0x001E81C8 (ctor 0x000A0D59); proto 0x0050C146; 6 of 98 bytes differ.
- 0x00123632 ?FrameTick@KillerDilloMain@@UAEXXZ: slot 4 of `??_7ChuteKillerDilloMain@@6B@` 0x001E81C8 (ctor 0x000A0D59); proto 0x0050C1DA; 84 of 1172 bytes differ.
- 0x00123E13 ?SetState@KillerDilloMain@@MAEXH@Z: slot 27 of `??_7ChuteKillerDilloMain@@6B@` 0x001E81C8 (ctor 0x000A0D59); proto 0x0050C984; 1325 of 1400 bytes differ.
- 0x00126709 ?FrameTick@BeadMain@@UAEXXZ: slot 4 of `??_7BeadMain@@6B@` 0x001F7248 (ctor 0x00126256); proto 0x0050F262; 54 of 1251 bytes differ.
- 0x00126BEC ?HitBy@BeadMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z: slot 20 of `??_7BeadMain@@6B@` 0x001F7248 (ctor 0x00126256); proto 0x0050F745; 7 of 52 bytes differ.
- 0x00129CD5 ?AnimationFinished@VinceDrinkjug@@UAEXXZ: slot 6 of `??_7VinceDrinkjug@@6B@` 0x001F79D0 (ctor 0x00129D49); proto 0x00512746; 2 of 22 bytes differ.
- 0x0012D503 ?Activate@SignTrolleyMain@@UAEXXZ: slot 2 of `??_7SignTrolleyMain@@6B@` 0x001F8350 (ctor 0x0012D338); proto 0x00516C42; 147 of 167 bytes differ.
- 0x001318F9 ?Activate@GoldenJuneBugMain@@UAEXXZ: slot 2 of `??_7GoldenJuneBugMain@@6B@` 0x001F8A50 (ctor 0x001317E0); proto 0x0051A32C; 74 of 300 bytes differ.
- 0x00131E7A ?HitBy@GoldenJuneBugMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z: slot 20 of `??_7GoldenJuneBugMain@@6B@` 0x001F8A50 (ctor 0x001317E0); proto 0x0051A89D; 54 of 250 bytes differ.
- Callee-route slots (`+site-corr+callee-bytes`, masked-equal): 0x0005DE7D CannonMain slot 22; 0x0005DFB9 CannonMain slot 5; 0x00129CEB VinceDrinkjug slot 4; 0x001318CC GoldenJuneBugMain slot 0.
- Vtables pinned `proto-pdb-public+site-corr+slot-names corr-verified` (9): CannonMain 0x001D78E8; MirrorUrnMain 0x001DAB78; OvenMain 0x001DEA50; PipeGasMain 0x001DFDE8; GasCanisterMain 0x001E7280; BeadMain 0x001F7248; VinceDrinkjug 0x001F79D0; SignTrolleyMain 0x001F8350; GoldenJuneBugMain 0x001F8A50.
- Refused `??_7ChuteKillerDilloMain@@6B@` 0x001E81C8 (ctor 0x000A0D59): slot 24 0x001234D9: both proto slot publics ?InvalidateScript@MonsterMain@@UAEXXZ and ?InvalidateScript@VinceBlowBack@@UAEXXZ are already pinned at 0x000F36DE (one name per VA, D119).
- Refused `??_7FrogOfMysteryMain@@6B@` 0x001CEC78 (ctor 0x0003C202): pin_rows route-slotnames:slot-not-independent (no agreeing site; no own-class slot of one public, 8+ B).
- Refused `??_7GateFrogMain@@6B@` 0x001DB8E8 (ctor 0x00071194): pin_rows route-slotnames:slot-not-independent (no agreeing site; no own-class slot of one public, 8+ B).
- Refused `??_7MrsGCollapsedMain@@6B@` 0x001D9F60 (ctor 0x00068019): slot 21 0x00107B84 holds the site-corr row ?Load@BasicDoorMain@@UAEXABVBeepTDF@@@Z (the proto slot public, status site-corr, not accepted); left as is per the brief.
- Refused `??_7TrailerFrogMain@@6B@` 0x001CF6C0 (ctor 0x0003DE5D): pin_rows route-slotnames:slot-not-independent (no agreeing site; no own-class slot of one public, 8+ B).
- Refused `??_7VinceLaundryCar@@6B@` 0x001E8798 (ctor 0x000A26B8): slot 27 0x000A258D holds the site-corr row ?SetState@VinceLaundryCar@@EAEXH@Z (the proto slot public, status site-corr, not accepted); left as is per the brief.
- Refused `??_7VinceWalkOut@@6B@` 0x001F9BB0 (ctor 0x00136386): pin_rows route-slotnames:slot-not-independent (no agreeing site; no own-class slot of one public, 8+ B).

### Play@SoundNode DSound callees; slot-row retags (5.5 T7.c5)
Hand lines (`proto-pdb-public+retail-disasm`; `site-corr+callee-bytes` refused first, reason given; C0412, C0426).
Callees: retail call in SoundNode::Play 0x000C5F3D at the same offset as proto twin 0x004B123F's call to the public's VA (`va_dis.py dis`, `sym`). Slots: as "I79 vtable slot names". No bytes (D46).
- `_IDirectSoundBuffer_SetCurrentPosition@8` 0x00179AB4 (callee-name-rule): Play 0x000C5F3D 7th call @0x000C6008 (+0xCB) vs proto 0x004B130A (+0xCB) -> 0x00563054; symbols.txt holds the undecorated name (C0192).
- `_IDirectSoundBuffer_SetPitch@8` 0x001799E4 (callee-name-rule): Play 0x000C5F3D 9th call @0x000C6034 (+0xF7) vs proto 0x004B1336 (+0xF7) -> 0x00562F84; symbols.txt holds the undecorated `IDirectSoundBuffer_SetPitch`.
- 0x000A258D ?SetState@VinceLaundryCar@@EAEXH@Z (callee-bytes-differ; was site-corr, blocking slot-names): slot 27 of `??_7VinceLaundryCar@@6B@` 0x001E8798 (ctor 0x000A26B8); proto 0x0048D67A; 31 of 260 bytes differ.
- 0x00107B84 ?Load@BasicDoorMain@@UAEXABVBeepTDF@@@Z (callee-bytes-differ; was site-corr, blocking slot-names): slot 21 of `??_7MrsGCollapsedMain@@6B@` 0x001D9F60 (ctor 0x00068019); proto 0x004F2538; 60 of 75 bytes differ.
- Auto-route lines (no bullet needed): `_IDirectSoundBuffer_SetBufferData@12` 0x0017A89C (3rd call @0x000C5FC2 vs proto 0x004B12C4 -> 0x00563E3C), `_IDirectSoundBuffer_SetPlayRegion@12` 0x0017A26A (4th @0x000C5FDC vs 0x004B12DE -> 0x0056380A), `_IDirectSoundBuffer_Play@16` 0x00179A1C (17th @0x000C60D8 vs 0x004B13DA -> 0x00562FBC): `+site-corr+callee-bytes`.
- D115 (R518): `?Play@SoundStreamer@@QAEXXZ` 0x000D9104 (bullet in "T15 needs pinned") DISABLED; callers Stop 0x000D91A7, Set 0x000D9519, CutsceneDriver::BeginPlayback 0x0010892C, MusicManager::Resume 0x00112A0B back under NON_MATCHING.
- Vtable pinned `proto-pdb-public+site-corr+slot-names corr-verified`: VinceLaundryCar 0x001E8798 (supersedes its T7.c1b refusal above).
- Refused `??_7MrsGCollapsedMain@@6B@` 0x001D9F60 (ctor 0x00068019) after the slot 21 retag: pin_rows route-slotnames:slot-not-independent (no agreeing site; no own-class slot of one public, 8+ B).
