# Pin-evidence censuses, phase 5.5 (T2; R5.5-003 B1, B2, S1-S3, S9)
Read-only. Scripts in .run/5.5/t2/ (scratch). Coders T2.c1 (87504a71e), T2.c2 (e64792d20), T2.c3 (ba8c75f0c).
D135 push sites: docs/evidence/d135_sites.md. No table or pin changed.

## Counts
- (1) A hand/xref: 98 rows (78 hand, 20 xref); evidence-found 90, name-only 8 (7 line-wraps, 1 real: 0x001EC830 $T318), refused-only 0, absent 0; sized+matched 5, all evidenced.
- (2) D bracket: 286 lines (279 active, 7 disabled); amb_bracket_down new fails 0 (0 on matched); plant 0x000A3EC9 -> dtor-not-own. `--recheck bracket` refuses 27 (callee-bytes, none matched).
- (2) E siblings: 56; confirmed 44 (29 independent, 15 via bracket-pin callees), unconfirmed 12, conflict 0; unconfirmed AND matched 3 (0x000E3E3B, 0x000E4429, 0x00121CB3).
- (3) F chain: 767 anchors walked; conflicts 41 items (18 creationFunc, 16 ctor, 7 vtable), one-class shift along proto order; on matched rows 6 (creationFunc 0x0005FB63, 0x0005FC5F, 0x00071769, 0x00071793, 0x000717BD, 0x000717E7) -> reopened in T7. Plant of 2 swapped creationFunc names -> 2. I81 owner: 0x000717E7 = ?creationFunc@DialogueBurlesqueRegionMain (symbols.txt's DialogueMovieRegionMain is wrong; Movie's is 0x00071811). Refusals: registerme callee-bytes 59 = callee-name rule (46 boundary_ names, 13 shifted names), bytes equal in all; abs-site already-named 31 = 13 agreeing chain hops, 7 static locals equal to proto public, 11 vacuous.
- (4) G M30: 60 rows; chain 36, agreeing site 24, slot-only 0, circular 0; twins 12, each with chain or site evidence.
- (5) B 0x000D9104: 13 rel32 sites, 5 in 4 matched functions, all matched after the Play pin (circular through I10): no independent byte-identical caller.
- (6) D135 sites: 61 bodies, 65 push sites, one named masked-equal caller each.

## Census A: hand/xref pin rows (T2.c1)
- source: .run/5.5/t2/census_a.py over config/retail/symbols_pin.txt, tools/pin_rows.py TAGS, docs/evidence/compiler_pin.md, config/matched_set.toml
- rows 98: hand 78, xref 20
- verdicts: evidence-found 90, not:name-only 1, not:name-only (VA on adjacent line, wrapped) 7
- by tag: `crt-absolute-0` 1, `proto-pdb-public+retail-data` 11, `proto-pdb-public+retail-disasm` 61, `proto-pdb-public+retail-xref` 18, `proto-pdb:default.pdb+proto-map` 1, `proto-pdb:default.pdb+proto-map+retail-xref` 2, `proto-pdb:default.pdb+retail-disasm` 1, `retail-disasm` 2, `retail-disasm+cl-listing` 1
- sized and matched (VA equal): 5; of them without evidence: 0
- verdict rule: evidence-found = a compiler_pin.md line naming the exact name and the row VA (any hex spelling), outside a `- Refused` bullet/heading

### Rows (symbols_pin order)
- 0x00000000 `__except_list` hand `crt-absolute-0` disasm-verified size -; evidence-found; compiler_pin.md:153 `### std::_Uninit_fill_n: matched (1.4 T9, 5 of 5)`
- 0x00141600 `__EH_prolog` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:153 `### std::_Uninit_fill_n: matched (1.4 T9, 5 of 5)`
- 0x00152BDC `__ehhandler$??$_Uninit_fill_n@PAUVector3@@IU1@V?$allocator@UVector3@@@std@@@std@@YAXPAUVector3@@IABU1@AAV?$allocator@UVector3@@@0@U_Nonscalar_ptr_iterator_tag@0@@Z` hand `retail-disasm` disasm-verified size -; not:name-only (VA on adjacent line 151, wrapped); compiler_pin.md:152 `### std::_Uninit_fill_n: matched (1.4 T9, 5 of 5)`
- 0x002369CC `?g_swamp@@3PAVSwamp@@A` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:176 `### VinceDropsInCutscene::SetState: matched (1.4 T9, 5 of 5)`
- 0x00125F98 `?Set@GameOptions@@QBEXXZ` xref `proto-pdb:default.pdb+proto-map+retail-xref` disasm-verified size 0x2B; evidence-found; compiler_pin.md:186 `### GameOptions::Set 0x00125F98 (2.1 T4)`
- 0x000E0887 `?handledMain@@YAXXZ` xref `proto-pdb:default.pdb+proto-map+retail-xref` disasm-verified size 0x329; evidence-found; compiler_pin.md:206 `### main 0x000E0BB0 SEH callees (2.2 T9)`
- 0x001418AC `__SEH_prolog` xref `proto-pdb-public+retail-xref` disasm-verified size 0x3B; evidence-found; compiler_pin.md:209 `### main 0x000E0BB0 SEH callees (2.2 T9)`
- 0x001418E7 `__SEH_epilog` xref `proto-pdb-public+retail-xref` disasm-verified size 0x11; evidence-found; compiler_pin.md:209 `### main 0x000E0BB0 SEH callees (2.2 T9)`
- 0x00141900 `__except_handler3` xref `proto-pdb-public+retail-xref` disasm-verified size -; evidence-found; compiler_pin.md:211 `### main 0x000E0BB0 SEH callees (2.2 T9)`
- 0x001EC830 `$T318` hand `retail-disasm+cl-listing` disasm-verified size -; not:name-only; compiler_pin.md:219 `### main 0x000E0BB0 SEH callees (2.2 T9)`
- 0x002231E0 `__fltused` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:221 `### __fltused 0x002231E0 (2.3 T6)`
- 0x00018696 `??1Record@BeepTDF@@QAE@XZ` hand `proto-pdb:default.pdb+proto-map` disasm-verified size -; evidence-found; compiler_pin.md:237 `### ~BeepState callees (2.5 T4)`
- 0x000386F8 `??1DebugState@BeepState@@QAE@XZ` hand `proto-pdb:default.pdb+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:240 `### ~BeepState callees (2.5 T4)`
- 0x0017A211 `_IDirectSoundBuffer_SetMinDistance@12` xref `proto-pdb-public+retail-xref` disasm-verified size 0x24; not:name-only (VA on adjacent line 253, wrapped); compiler_pin.md:254 `### SoundNode DSOUND callees (2.5 T6)`
- 0x0017A1ED `_IDirectSoundBuffer_SetMaxDistance@12` xref `proto-pdb-public+retail-xref` disasm-verified size 0x24; not:name-only (VA on adjacent line 253, wrapped); compiler_pin.md:254 `### SoundNode DSOUND callees (2.5 T6)`
- 0x001C09A8 `_XInputClose@4` xref `proto-pdb-public+retail-xref` disasm-verified size 0xC; evidence-found; compiler_pin.md:272 `### XDK callee pins (3.3 T19)`
- 0x001790A8 `?Pause@CDirectSoundBuffer@DirectSound@@QAGJK@Z` xref `proto-pdb-public+retail-xref` disasm-verified size 0x51; evidence-found; compiler_pin.md:277 `### XDK callee pins (3.3 T19)`
- 0x001790F9 `?GetStatus@CDirectSoundBuffer@DirectSound@@QAGJPAK@Z` xref `proto-pdb-public+retail-xref` disasm-verified size 0x51; evidence-found; compiler_pin.md:278 `### XDK callee pins (3.3 T19)`
- 0x00179A7C `_IDirectSoundBuffer_Pause@8` xref `proto-pdb-public+retail-xref` disasm-verified size 0x1C; evidence-found; compiler_pin.md:274 `### XDK callee pins (3.3 T19)`
- 0x00179A98 `_IDirectSoundBuffer_GetStatus@8` xref `proto-pdb-public+retail-xref` disasm-verified size 0x1C; evidence-found; compiler_pin.md:275 `### XDK callee pins (3.3 T19)`
- 0x0015B440 `_D3DTexture_GetLevelDesc@12` xref `proto-pdb-public+retail-xref` disasm-verified size 0x5; evidence-found; compiler_pin.md:281 `### XDK callee pins (3.3 T19)`
- 0x00179435 `?Pause@CDirectSoundStream@DirectSound@@QAGJK@Z` xref `proto-pdb-public+retail-xref` disasm-verified size 0x51; evidence-found; compiler_pin.md:290 `### XDK callee pins (3.3 T19)`
- 0x00179AD5 `_IDirectSoundStream_Pause@8` xref `proto-pdb-public+retail-xref` disasm-verified size 0x5; not:name-only (VA on adjacent line 288, wrapped); compiler_pin.md:289 `### XDK callee pins (3.3 T19)`
- 0x000D8C2D `?SetVolume@SoundStreamer@@QAEXM@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:519 `### T15 needs pinned (5.4 T25.c1)`
- 0x00141F74 `__ftol2` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:520 `### T15 needs pinned (5.4 T25.c1)`
- 0x001575A0 `_D3DDevice_SetRenderState_OcclusionCullEnable@4` xref `proto-pdb-public+retail-xref` disasm-verified size 0x62; not:name-only (VA on adjacent line 298, wrapped); compiler_pin.md:299 `### SwampRenderer::SetRenderState D3D pins (4.3 T9.c48)`
- 0x00157610 `_D3DDevice_SetRenderState_StencilCullEnable@4` xref `proto-pdb-public+retail-xref` disasm-verified size 0x62; not:name-only (VA on adjacent line 299, wrapped); compiler_pin.md:300 `### SwampRenderer::SetRenderState D3D pins (4.3 T9.c48)`
- 0x00157680 `_D3DDevice_SetRenderState_RopZCmpAlwaysRead@4` xref `proto-pdb-public+retail-xref` disasm-verified size 0x11; not:name-only (VA on adjacent line 300, wrapped); compiler_pin.md:301 `### SwampRenderer::SetRenderState D3D pins (4.3 T9.c48)`
- 0x001576A0 `_D3DDevice_SetRenderState_RopZRead@4` xref `proto-pdb-public+retail-xref` disasm-verified size 0x11; evidence-found; compiler_pin.md:301 `### SwampRenderer::SetRenderState D3D pins (4.3 T9.c48)`
- 0x001576C0 `_D3DDevice_SetRenderState_DoNotCullUncompressed@4` xref `proto-pdb-public+retail-xref` disasm-verified size 0x11; evidence-found; compiler_pin.md:302 `### SwampRenderer::SetRenderState D3D pins (4.3 T9.c48)`
- 0x000388BB `_unzlocal_GetCurrentFileInfoInternal` hand `retail-disasm` disasm-verified size 0x2AA; evidence-found; compiler_pin.md:325 `### unzlocal_GetCurrentFileInfoInternal 0x000388BB (4.3 T15)`
- 0x000164B3 `?Update@LoadScreen@@UAEX_NM@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:329 `### Vtable slot pins at 0x000164B3 (4.4 T4.c1)`
- 0x00017E42 `?TestActivate@SwampScript@@UBE_NXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:334 `### Vtable slot pins at 0x00017E42 (4.4 T4.c1)`
- 0x00017E45 `?AnimationTrigger@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@MMI@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:339 `### Vtable slot pins at 0x00017E45 (4.4 T4.c1)`
- 0x00017E48 `?LosingControl@SwampScript@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:344 `### Vtable slot pins at 0x00017E48 (4.4 T4.c1)`
- 0x00017E4D `?CollidedWith@SwampScript@@UAEXW4CollisionType@1@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:349 `### Vtable slot pins at 0x00017E4D (4.4 T4.c1)`
- 0x00017E4D `?HitBy@SwampScript@@UAEXW4CollisionType@1@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:350 `### Vtable slot pins at 0x00017E4D (4.4 T4.c1)`
- 0x00017E4D `?Message@SwampScript@@UAEXPAV1@HPAX@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:351 `### Vtable slot pins at 0x00017E4D (4.4 T4.c1)`
- 0x000946F7 `?EndOfTransition@LoadScreen@@UAEX_N@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:356 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?EnteringRegion@SwampScript@@UAEXPAVSwampSceneNode@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:357 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?GainingControl@SwampScript@@UAEXPAV1@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:358 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?LeavingRegion@SwampScript@@UAEXPAVSwampSceneNode@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:359 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?MovieFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:360 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?Render@SwampSceneNode@@MBEXABUMatrix@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:361 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?SetState@CutsceneDriver@@MAEXH@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:362 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?SoundFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:363 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?SoundStreamerFinished@SwampScript@@UAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:364 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?TimerEvent@SwampScript@@UAEXI@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:365 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x000946F7 `?UpdateAnimation@SwampSceneNode@@MAEXI@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:366 `### Vtable slot pins at 0x000946F7 (4.4 T4.c1)`
- 0x0011C7BC `?Activate@GameCam@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:371 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?Activate@SwampScript@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:372 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?AnimationFinished@SwampScript@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:373 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?AnimationInterrupted@SwampScript@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:374 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?Deactivate@SwampScript@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:375 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?FrameTick@SwampScript@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:376 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?LosingControl@GameCam@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:377 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?SecondaryAnimationFinished@SwampScript@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:378 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?SecondaryAnimationInterrupted@SwampScript@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:379 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?Update@LoadScreen@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:380 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x0011C7BC `?Update@SwampSceneNode@@MAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:381 `### Vtable slot pins at 0x0011C7BC (4.4 T4.c1)`
- 0x001FCF9C `__CTA3?AVlength_error@std@@` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:393 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x001FCF10 `__CTA3?AVout_of_range@std@@` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:397 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x001FCF80 `__CT??_R0?AVlength_error@std@@@8??0length_error@std@@QAE@ABV01@@Z40` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:394 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x001FCEF4 `__CT??_R0?AVout_of_range@std@@@8??0out_of_range@std@@QAE@ABV01@@Z40` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:398 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x001FCED8 `__CT??_R0?AVlogic_error@std@@@8??0logic_error@std@@QAE@ABV01@@Z40` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:395 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x001FCEBC `__CT??_R0?AVexception@@@8??0exception@@QAE@ABV0@@Z12` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:396 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x00204D08 `??_R0?AVlength_error@std@@@8` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:399 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x00204778 `??_R0?AVout_of_range@std@@@8` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:399 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x00204758 `??_R0?AVlogic_error@std@@@8` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:400 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x00204740 `??_R0?AVexception@@@8` hand `proto-pdb-public+retail-data` disasm-verified size -; evidence-found; compiler_pin.md:400 `### Throw-info data for length_error / out_of_range (4.4 T4.c2)`
- 0x000180E1 `?GetGravity@Critter@@UAEMXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x7; evidence-found; compiler_pin.md:422 `#### Vtable slot pins at 0x000180E1 (5.2 T6.c1)`; SIZED+MATCHED
- 0x00037178 `?Open@MemoryFileHandler@@UAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x5; evidence-found; compiler_pin.md:427 `#### Vtable slot pins at 0x00037178 (5.2 T6.c1)`
- 0x00037178 `?Write@MemoryFileHandler@@UAE_NPBXI@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x5; evidence-found; compiler_pin.md:428 `#### Vtable slot pins at 0x00037178 (5.2 T6.c1)`
- 0x00037178 `?Write@ZipFileHandler@@UAE_NPBXI@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x5; evidence-found; compiler_pin.md:429 `#### Vtable slot pins at 0x00037178 (5.2 T6.c1)`; SIZED+MATCHED
- 0x0003735F `?Size@AsyncFileHandler@@UAEIXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:434 `#### Vtable slot pins at 0x0003735F (5.2 T6.c1)`
- 0x0003735F `?Size@MemoryFileHandler@@UAEIXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:435 `#### Vtable slot pins at 0x0003735F (5.2 T6.c1)`
- 0x0003735F `?Size@ZipFileHandler@@UAEIXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:436 `#### Vtable slot pins at 0x0003735F (5.2 T6.c1)`
- 0x00037363 `?Pos@AsyncFileHandler@@UAEHXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x4; evidence-found; compiler_pin.md:441 `#### Vtable slot pins at 0x00037363 (5.2 T6.c1)`
- 0x00037363 `?Pos@MemoryFileHandler@@UAEHXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x4; evidence-found; compiler_pin.md:442 `#### Vtable slot pins at 0x00037363 (5.2 T6.c1)`
- 0x00037363 `?Pos@ZipFileHandler@@UAEHXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x4; evidence-found; compiler_pin.md:443 `#### Vtable slot pins at 0x00037363 (5.2 T6.c1)`; SIZED+MATCHED
- 0x0006972D `?AnimationInterrupted@NetMain@@UAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x5; evidence-found; compiler_pin.md:448 `#### Vtable slot pins at 0x0006972D (5.2 T6.c1)`; SIZED+MATCHED
- 0x00078E59 `?CanJorgeCollideWithMe@CollidableNode@@UBE_NPBVSwampSceneNode@@ABUVector3@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x5; evidence-found; compiler_pin.md:453 `#### Vtable slot pins at 0x00078E59 (5.2 T6.c1)`
- 0x00078E59 `?DoesJorgeRepelMe@CollidableNode@@UBE_NPBVSwampSceneNode@@_N@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size 0x5; evidence-found; compiler_pin.md:454 `#### Vtable slot pins at 0x00078E59 (5.2 T6.c1)`; SIZED+MATCHED
- 0x0013240F `?HitBy@ClownMissileMain@@UAEXW4CollisionType@SwampScript@@PAVSwampSceneNode@@ABV?$Ptr@VCollisionResultObject@@@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:459 `#### Vtable slot pins at 0x0013240F (5.2 T6.c1)`
- 0x00138E4E `?GetDefaultState@BasicDoorMain@@UBEHXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:464 `#### Vtable slot pins at 0x00138E4E (5.2 T6.c1)`
- 0x000D9104 `?Play@SoundStreamer@@QAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:494 `### T15 needs pinned (5.4 T25.c1)`
- 0x000D3074 `?CancelRequest@DiskQueue@@QAEXPAI@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:501 `### T15 needs pinned (5.4 T25.c1)`
- 0x000D90CA `?Stopped@SoundManager@@AAEXPAVSoundStreamer@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:497 `### T15 needs pinned (5.4 T25.c1)`
- 0x000D8D1D `?CancelRequests@SoundStreamer@@AAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:499 `### T15 needs pinned (5.4 T25.c1)`
- 0x0003729B `??0MemoryFileHandler@@QAE@I@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:503 `### T15 needs pinned (5.4 T25.c1)`
- 0x000D3314 `?Update@DiskQueue@@QAEXXZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:505 `### T15 needs pinned (5.4 T25.c1)`
- 0x001799C8 `_IDirectSoundBuffer_SetVolume@8` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:509 `### T15 needs pinned (5.4 T25.c1)`
- 0x0010F0F8 `??0HeavyObjectMain@@QAE@PAVSwampSceneNode@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:507 `### T15 needs pinned (5.4 T25.c1)`
- 0x0008132B `?creationFunc@ReggieWreckageMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:534 `### Row-end pins for the T25 cut-wall rows (5.4 T26.c1)`
- 0x000714F5 `?creationFunc@Beret@@CAPAVSwampSceneNode@@XZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:535 `### Row-end pins for the T25 cut-wall rows (5.4 T26.c1)`
- 0x00073A31 `?creationFunc@BeretMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:536 `### Row-end pins for the T25 cut-wall rows (5.4 T26.c1)`
- 0x0007447F `?creationFunc@MasqueradeMask@@CAPAVSwampSceneNode@@XZ` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:537 `### Row-end pins for the T25 cut-wall rows (5.4 T26.c1)`
- 0x0005FC89 `?creationFunc@MoleGateMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` hand `proto-pdb-public+retail-disasm` disasm-verified size -; evidence-found; compiler_pin.md:538 `### Row-end pins for the T25 cut-wall rows (5.4 T26.c1)`

### Sized and matched
- 0x000180E1 `?GetGravity@Critter@@UAEMXZ`: evidence-found
- 0x00037178 `?Write@ZipFileHandler@@UAE_NPBXI@Z`: evidence-found
- 0x00037363 `?Pos@ZipFileHandler@@UAEHXZ`: evidence-found
- 0x0006972D `?AnimationInterrupted@NetMain@@UAEXXZ`: evidence-found
- 0x00078E59 `?DoesJorgeRepelMe@CollidableNode@@UBE_NPBVSwampSceneNode@@_N@Z`: evidence-found

## Census B: byte-identical callers of 0x000D9104 `?Play@SoundStreamer@@QAEXXZ` (T2.c1)
- source: .run/5.5/t2/census_bc.py; retail exec sections via name_sites.load(); rel32 E8/E9/0F8x scan, each hit decoded (iced) from its Ghidra container entry; matched = VA in config/matched_set.toml (D123)
- rel32 sites 13; decoded 13; in a matched function 5; push imm32 of the VA 0
- answer: yes

### Sites
- site 0x000B9CB8 call rel32; decoded yes; in 0x000B9A1E `?Update@SoundManager@@QAEXXZ`; matched_set no
- site 0x000D920D call rel32; decoded yes; in 0x000D91A7 `?Stop@SoundStreamer@@QAEXXZ`; matched_set yes
- site 0x000D9311 call rel32; decoded yes; in 0x000D926C `(unnamed)`; matched_set no
- site 0x000D95B3 call rel32; decoded yes; in 0x000D9519 `?Set@SoundStreamer@@QAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@I@Z`; matched_set yes
- site 0x000D9868 call rel32; decoded yes; in 0x000D95CC `?Update@SoundStreamer@@QAEXXZ`; matched_set no
- site 0x0010893B jmp rel32; decoded yes; in 0x0010892C `?BeginPlayback@CutsceneDriver@@SAXXZ`; matched_set yes
- site 0x00112A28 call rel32; decoded yes; in 0x00112A0B `?Resume@MusicManager@@QAEXXZ`; matched_set yes
- site 0x00112A3D call rel32; decoded yes; in 0x00112A0B `?Resume@MusicManager@@QAEXXZ`; matched_set yes
- site 0x00113AC5 call rel32; decoded yes; in 0x00113A05 `?Update@MusicManager@@QAEXXZ`; matched_set no
- site 0x00113BD1 call rel32; decoded yes; in 0x00113A05 `?Update@MusicManager@@QAEXXZ`; matched_set no
- site 0x00113BD9 call rel32; decoded yes; in 0x00113A05 `?Update@MusicManager@@QAEXXZ`; matched_set no
- site 0x00113C58 call rel32; decoded yes; in 0x00113A05 `?Update@MusicManager@@QAEXXZ`; matched_set no
- site 0x00113CFA call rel32; decoded yes; in 0x00113A05 `?Update@MusicManager@@QAEXXZ`; matched_set no

### Caveat
- matched callers: Stop 0x000D91A7, Set 0x000D9519, BeginPlayback 0x0010892C (tail jmp), MusicManager::Resume 0x00112A0B (2 sites)
- all four entered matched_set.toml in 102ec72db (5.4 T25), after the Play pin 5a8efdc59 (T25.c1); their rel32 to Play resolves through that pin row (I10), so the byte identity of the call field itself is circular; the rest of each body is byte-equal (D123), which fixes the call ordinal

## Census D: bracket rows through name_sites.amb_bracket_down (own lines removed; report mode)

- rows: 286 bracket-tag lines (279 active, 7 commented under `# DISABLED:`); bodies: 226 active, 6 disabled
- active fails: 0 (0); on matched bodies: 0
- T3-block bodies (`--recheck bracket` scope): 211; full-chain amb_verdict refusals there: 27 [('callee-bytes', 27)]; on matched bodies 0; full chain over all active bodies: 27 refusals [('callee-bytes', 27)]
- disabled bodies: 0x000A3EC9 dtor-not-own, 0x000A83B8 callee-down, 0x000AA1F3 callee-down, 0x000AA499 callee-down, 0x000FED6B pass, 0x000FEDD6 pass

### Fails (active)
- none

### Disabled bodies (plant and pilot disables)
- 0x000A3EC9 ??_EPropellerMain@@UAEPAXI@Z reason dtor-not-own chain dtor-not-own matched no
- 0x000A83B8 ??0LeavingRoachfortCutscene@@QAE@PAVSwampSceneNode@@@Z reason callee-down chain callee-down matched no
- 0x000AA1F3 ??0PlanGoesAwryCutscene@@QAE@PAVSwampSceneNode@@@Z reason callee-down chain callee-down matched no
- 0x000AA499 ??0JaniceDefeatCutscene@@QAE@PAVSwampSceneNode@@@Z reason callee-down chain callee-down matched no
- 0x000FED6B ?_Buynode@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_string@D reason pass chain pass matched no
- 0x000FEDD6 ?_Insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_string@DU reason pass chain callee-bytes matched no

### T3-block full-chain refusals (reconcile with --recheck bracket)
- 0x000360F3 ??$_Uninit_copy@PAUSample@FrequencyCalculator@@PAU12@V?$allocator@USample@FrequencyCalculator@@@std@@@std@@YAPAUSample@F callee-bytes matched no
- 0x0003628E ??$_Uninit_fill_n@PAUSample@FrequencyCalculator@@IU12@V?$allocator@USample@FrequencyCalculator@@@std@@@std@@YAXPAUSample callee-bytes matched no
- 0x00037F4E ?_Insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V12@U?$less@V?$basic_stri callee-bytes matched no
- 0x000ACAD9 ?insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@P6APAVSwampScript@@PAVSwam callee-bytes matched no
- 0x000B218A ??$_Uninit_copy@PAUCollidableInfo@@PAU1@V?$allocator@UCollidableInfo@@@std@@@std@@YAPAUCollidableInfo@@PAU1@00AAV?$alloc callee-bytes matched no
- 0x000B240E ??$_Uninit_fill_n@PAUCollidableInfo@@IU1@V?$allocator@UCollidableInfo@@@std@@@std@@YAXPAUCollidableInfo@@IABU1@AAV?$allo callee-bytes matched no
- 0x000B88FF ?insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PAVFireFlyNode@@U?$less@V? callee-bytes matched no
- 0x000BB470 ??$_Uninit_copy@Viterator@?$vector@PAVBSP@@V?$allocator@PAVBSP@@@std@@@std@@PAPAVBSP@@V?$allocator@PAVBSP@@@3@@std@@YAPA callee-bytes matched no
- 0x000BB4C3 ??$_Uninit_copy@PAV?$ResourceHandle@VMaterialResource@@@@PAV1@V?$allocator@V?$ResourceHandle@VMaterialResource@@@@@std@@ callee-bytes matched no
- 0x000BB960 ??$_Uninit_fill_n@PAV?$ResourceHandle@VMaterialResource@@@@IV1@V?$allocator@V?$ResourceHandle@VMaterialResource@@@@@std@ callee-bytes matched no
- 0x000BE29C ??$_Uninitialized_copy@PAUResourceBundle@?$ResourceManager@VAnimationResource@@@@PAU12@V?$allocator@UResourceBundle@?$Re callee-bytes matched no
- 0x000C1D13 ??$_Uninit_fill_n@PAUZoneRenderInfo@DrawLevel@@IU12@V?$allocator@UZoneRenderInfo@DrawLevel@@@std@@@std@@YAXPAUZoneRender callee-bytes matched no
- 0x000C7765 ??$_Uninit_copy@Vconst_iterator@?$vector@V?$ResourceHandle@VTextureResource@@@@V?$allocator@V?$ResourceHandle@VTextureRe callee-bytes matched no
- 0x000C77D8 ??$_Uninit_fill_n@PAV?$ResourceHandle@VTextureResource@@@@IV1@V?$allocator@V?$ResourceHandle@VTextureResource@@@@@std@@@ callee-bytes matched no
- 0x000D3CD6 ?_Buynode@?$_Tree@V?$_Tset_traits@PAVDebugGeometryNode@@U?$less@PAVDebugGeometryNode@@@std@@V?$allocator@PAVDebugGeometr callee-bytes matched no
- 0x000D5173 ??$_Uninit_copy@PAUFixupInfo@MaterialManager@@PAU12@V?$allocator@UFixupInfo@MaterialManager@@@std@@@std@@YAPAUFixupInfo@ callee-bytes matched no
- 0x000D51C5 ??$_Uninit_fill_n@PAUFixupInfo@MaterialManager@@IU12@V?$allocator@UFixupInfo@MaterialManager@@@std@@@std@@YAXPAUFixupInf callee-bytes matched no
- 0x000D5230 ??$_Uninit_copy@Vconst_iterator@?$vector@PAVBeepTDF@@V?$allocator@PAVBeepTDF@@@std@@@std@@PAPAVBeepTDF@@V?$allocator@PAV callee-bytes matched no
- 0x000E6B59 ?erase@?$list@USubtitle@HUDUI@@V?$allocator@USubtitle@HUDUI@@@std@@@std@@QAE?AViterator@12@V312@@Z callee-bytes matched no
- 0x000E8555 ?_Tidy@?$list@USubtitle@HUDUI@@V?$allocator@USubtitle@HUDUI@@@std@@@std@@IAEXXZ callee-bytes matched no
- 0x000E9532 ?insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PAVUINode@@U?$less@V?$basi callee-bytes matched no
- 0x000FE5B2 ?_Insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ULangInfo@FontManager@@U? callee-bytes matched no
- 0x000FE813 ?insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PAVCXBFont@@U?$less@V?$bas callee-bytes matched no
- 0x0010AEF1 ?insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PAVLocator@@U?$less@V?$bas callee-bytes matched no
- 0x0011522A ??$_Uninit_copy@Vconst_iterator@?$vector@V?$ResourceHandle@VSoundResource@@@@V?$allocator@V?$ResourceHandle@VSoundResour callee-bytes matched no
- 0x0011529D ??$_Uninit_copy@PAV?$ResourceHandle@VSoundResource@@@@PAV1@V?$allocator@V?$ResourceHandle@VSoundResource@@@@@std@@@std@@ callee-bytes matched no
- 0x0011534C ??$_Uninit_fill_n@PAV?$ResourceHandle@VSoundResource@@@@IV1@V?$allocator@V?$ResourceHandle@VSoundResource@@@@@std@@@std@ callee-bytes matched no

## Census E: sibling-flagged bracket bodies (D128 confirmation)

- bodies: 56 (47 docs/evidence/bracket_siblings_5.3.txt + 9 5.3 T12.1)
- verdicts: confirmed 44, unconfirmed 12
- confirmed by best evidence: bracket 10, bracket-in-56 5, indep 29 (indep = callee named in symbols.txt or a non-site-corr non-bracket pin, or an agreeing caller site; bracket = callee is itself a bracket pin; bracket-in-56 = that callee is one of these 56); site-corr-only callee evidence counts unconfirmed
- unconfirmed AND matched: 3; matched total: 4

### Bodies
- 0x000649D0 ??_ERodeoRatMain@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x000649EC ??1RodeoRatMain@@UAE@XZ (proto-pdb:default.pdb) twins 11 origin 5.3-file
- 0x0006606F ??_EVinceOnRatMain@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x0006608B ??1VinceOnRatMain@@UAE@XZ (proto-pdb:default.pdb) twins 11 origin 5.3-file
- 0x0009DA98 ??0Airplane_RocketLabMain@@QAE@PAVSwampSceneNode@@@Z matched no verdict confirmed evidence [indep] callee 0x0011DB03 ??0AirplaneMain@@QAE@PAVSwampSceneNode@@@Z (proto-pdb:default.pdb) twins 1 origin 5.3-file
- 0x0009DEE6 ??0QuickCanisterMain@@QAE@PAVSwampSceneNode@@@Z matched no verdict confirmed evidence [indep] callee 0x0009DCC3 ??0GasCanisterMain@@QAE@PAVSwampSceneNode@@@Z (proto-pdb-public+anchor-window+caller-xref) twins 1 origin 5.3-file
- 0x0004271B ??_EVinceCarRide@@UAEPAXI@Z matched no verdict confirmed evidence [bracket] callee 0x00042737 ??1VinceCarRide@@UAE@XZ (proto-pdb-public+unit-bracket+callee-bytes) twins 8 origin 5.3-file
- 0x00139A97 ??$copy_backward@PAUCaptionLine@FrontEndMenu@@PAU12@@std@@YAPAUCaptionLine@FrontEndMenu@@PAU12@00@Z matched no verdict confirmed evidence [bracket] callee 0x00138FC2 ??$_Copy_backward_opt@PAUCaptionLine@FrontEndMenu@@PAU12@@std@@YAPAUCaptionLine@FrontEndMenu@@PAU12@00U_Nonscalar_ptr_it (proto-pdb-public+unit-bracket+callee-bytes) twins 3 origin 5.3-file
- 0x00139B81 ??$_Uninitialized_copy@PAUCaptionLine@FrontEndMenu@@PAU12@V?$allocator@UCaptionLine@FrontEndMenu@@@std@@@std@@YAPAUCapti matched no verdict confirmed evidence [indep] site caller 0x00139D7D; [bracket] callee 0x00138FFD ??$_Uninit_copy@PAUCaptionLine@FrontEndMenu@@PAU12@V?$allocator@UCaptionLine@FrontEndMenu@@@std@@@std@@YAPAUCaptionLine@ (proto-pdb-public+unit-bracket+callee-bytes) twins 3 origin 5.3-file
- 0x00139D7D ??$_Ucopy@PAUCaptionLine@FrontEndMenu@@@?$vector@UCaptionLine@FrontEndMenu@@V?$allocator@UCaptionLine@FrontEndMenu@@@std matched no verdict confirmed evidence [bracket-in-56] callee 0x00139B81 ??$_Uninitialized_copy@PAUCaptionLine@FrontEndMenu@@PAU12@V?$allocator@UCaptionLine@FrontEndMenu@@@std@@@std@@YAPAUCapti (proto-pdb-public+unit-bracket+callee-bytes) twins 1 origin 5.3-file
- 0x0013A4DE ?push_back@?$vector@VSavedGameInfo@SavedGamesScreen@@V?$allocator@VSavedGameInfo@SavedGamesScreen@@@std@@@std@@QAEXABVSa matched no verdict confirmed evidence [indep] callee 0x00138800 ?size@?$vector@VSavedGameInfo@SavedGamesScreen@@V?$allocator@VSavedGameInfo@SavedGamesScreen@@@std@@@std@@QBEIXZ (proto-pdb:default.pdb); [indep] callee 0x0013883C ?capacity@?$vector@VSavedGameInfo@SavedGamesScreen@@V?$allocator@VSavedGameInfo@SavedGamesScreen@@@std@@@std@@QBEIXZ (proto-pdb:default.pdb); [indep] callee 0x0013A168 ?_Ufill@?$vector@VSavedGameInfo@SavedGamesScreen@@V?$allocator@VSavedGameInfo@SavedGamesScreen@@@std@@@std@@IAEPAVSavedG (proto-pdb:default.pdb); [indep] callee 0x0013A400 ?insert@?$vector@VSavedGameInfo@SavedGamesScreen@@V?$allocator@VSavedGameInfo@SavedGamesScreen@@@std@@@std@@QAE?AViterat (proto-pdb:default.pdb) twins 1 origin 5.3-file
- 0x00059192 ??_EZombieBakerMain@@UAEPAXI@Z matched no verdict confirmed evidence [bracket] callee 0x00057F6E ??1ZombieMain@@UAE@XZ (proto-pdb-public+unit-bracket+callee-bytes) twins 8 origin 5.3-file
- 0x00071293 ??0JazzClubRegionMain@@QAE@PAVSwampSceneNode@@@Z matched no verdict unconfirmed evidence - twins 16 origin 5.3-file
- 0x00074389 ??_ESkeletonJazzGuyMain@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x000743A5 ??1SkeletonJazzGuyMain@@UAE@XZ (proto-pdb:default.pdb) twins 9 origin 5.3-file
- 0x000362DC ??$_Uninitialized_copy@PAUSample@FrequencyCalculator@@PAU12@V?$allocator@USample@FrequencyCalculator@@@std@@@std@@YAPAUS matched no verdict confirmed evidence [indep] site caller 0x000364D5; [bracket] callee 0x000360F3 ??$_Uninit_copy@PAUSample@FrequencyCalculator@@PAU12@V?$allocator@USample@FrequencyCalculator@@@std@@@std@@YAPAUSample@F (proto-pdb-public+unit-bracket+callee-bytes) twins 2 origin 5.3-file
- 0x000364B8 ??$_Uninitialized_fill_n@PAUSample@FrequencyCalculator@@IU12@V?$allocator@USample@FrequencyCalculator@@@std@@@std@@YAXPA matched no verdict confirmed evidence [indep] site caller 0x00036595; [bracket] callee 0x0003628E ??$_Uninit_fill_n@PAUSample@FrequencyCalculator@@IU12@V?$allocator@USample@FrequencyCalculator@@@std@@@std@@YAXPAUSample (proto-pdb-public+unit-bracket+callee-bytes) twins 2 origin 5.3-file
- 0x000364D5 ??$_Ucopy@PAUSample@FrequencyCalculator@@@?$vector@USample@FrequencyCalculator@@V?$allocator@USample@FrequencyCalculator matched no verdict confirmed evidence [bracket-in-56] callee 0x000362DC ??$_Uninitialized_copy@PAUSample@FrequencyCalculator@@PAU12@V?$allocator@USample@FrequencyCalculator@@@std@@@std@@YAPAUS (proto-pdb-public+unit-bracket+callee-bytes) twins 1 origin 5.3-file
- 0x000D31C0 ??$_Uninitialized_copy@PAURequest@DiskQueue@@PAU12@V?$allocator@URequest@DiskQueue@@@std@@@std@@YAPAURequest@DiskQueue@@ matched no verdict confirmed evidence [indep] site caller 0x000D3254 twins 1 origin 5.3-file
- 0x000D3237 ??$_Uninitialized_fill_n@PAURequest@DiskQueue@@IU12@V?$allocator@URequest@DiskQueue@@@std@@@std@@YAXPAURequest@DiskQueue matched no verdict confirmed evidence [indep] site caller 0x000D32A7 twins 1 origin 5.3-file
- 0x000BE29C ??$_Uninitialized_copy@PAUResourceBundle@?$ResourceManager@VAnimationResource@@@@PAU12@V?$allocator@UResourceBundle@?$Re matched no verdict confirmed evidence [indep] callee 0x000BE20B ??$_Uninit_copy@PAUResourceBundle@?$ResourceManager@VAnimationResource@@@@PAU12@V?$allocator@UResourceBundle@?$ResourceM (proto-pdb-public+fold-alias+callee-bytes) twins 1 origin 5.3-file
- 0x000AC26B ??$copy_backward@PAVSaveLoadVar@SwampScript@@PAV12@@std@@YAPAVSaveLoadVar@SwampScript@@PAV12@00@Z matched no verdict confirmed evidence [bracket] callee 0x000ABFBA ??$_Copy_backward_opt@PAVSaveLoadVar@SwampScript@@PAV12@@std@@YAPAVSaveLoadVar@SwampScript@@PAV12@00U_Nonscalar_ptr_iter (proto-pdb-public+unit-bracket+callee-bytes) twins 1 origin 5.3-file
- 0x000AC2EF ??$_Uninitialized_copy@PAVSaveLoadVar@SwampScript@@PAV12@V?$allocator@VSaveLoadVar@SwampScript@@@std@@@std@@YAPAVSaveLoa matched no verdict unconfirmed evidence [site-corr] callee 0x000ABFF5 ??$_Uninit_copy@PAVSaveLoadVar@SwampScript@@PAV12@V?$allocator@VSaveLoadVar@SwampScript@@@std@@@std@@YAPAVSaveLoadVar@Sw (proto-pdb-public+site-corr) twins 2 origin 5.3-file
- 0x000C1D61 ??$_Uninitialized_copy@PAUZoneRenderInfo@DrawLevel@@PAU12@V?$allocator@UZoneRenderInfo@DrawLevel@@@std@@@std@@YAPAUZoneR matched no verdict confirmed evidence [indep] callee 0x000C13DC ??$_Uninit_copy@PAUZoneRenderInfo@DrawLevel@@PAU12@V?$allocator@UZoneRenderInfo@DrawLevel@@@std@@@std@@YAPAUZoneRenderIn (proto-pdb-public+site-corr+callee-bytes); [indep] site caller 0x000C20C6; [indep] site caller 0x000C3EFB; [indep] site caller 0x000C3EFB twins 2 origin 5.3-file
- 0x000C20A9 ??$_Uninitialized_fill_n@PAUZoneRenderInfo@DrawLevel@@IU12@V?$allocator@UZoneRenderInfo@DrawLevel@@@std@@@std@@YAXPAUZon matched no verdict confirmed evidence [indep] site caller 0x000C238A; [bracket] callee 0x000C1D13 ??$_Uninit_fill_n@PAUZoneRenderInfo@DrawLevel@@IU12@V?$allocator@UZoneRenderInfo@DrawLevel@@@std@@@std@@YAXPAUZoneRender (proto-pdb-public+unit-bracket+callee-bytes) twins 2 origin 5.3-file
- 0x000C20C6 ??$_Ucopy@PAUZoneRenderInfo@DrawLevel@@@?$vector@UZoneRenderInfo@DrawLevel@@V?$allocator@UZoneRenderInfo@DrawLevel@@@std matched no verdict confirmed evidence [indep] site caller 0x000C3EFB; [indep] site caller 0x000C3EFB; [bracket-in-56] callee 0x000C1D61 ??$_Uninitialized_copy@PAUZoneRenderInfo@DrawLevel@@PAU12@V?$allocator@UZoneRenderInfo@DrawLevel@@@std@@@std@@YAPAUZoneR (proto-pdb-public+unit-bracket+callee-bytes) twins 1 origin 5.3-file
- 0x000C254C ??_EWorldData@@UAEPAXI@Z matched yes verdict confirmed evidence [indep] callee 0x000C246B ??1WorldData@@UAE@XZ (proto-pdb:default.pdb) twins 1 origin 5.3-file
- 0x000F0234 ?clear@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PAVCritter@@U?$less@V?$basi matched no verdict unconfirmed evidence [site-corr] callee 0x000EF490 ?_Erase@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PAVCritter@@U?$less@V?$bas (proto-pdb-public+site-corr) twins 3 origin 5.3-file
- 0x000FE5B2 ?_Insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ULangInfo@FontManager@@U? matched no verdict confirmed evidence [indep] callee 0x000FE11B ?_Buynode@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ULangInfo@FontManager@@U (proto-pdb-public+site-corr+callee-bytes) twins 1 origin 5.3-file
- 0x000FED19 ??0_Node@?$_Tree_nod@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_strin matched no verdict confirmed evidence [bracket] callee 0x000FECF8 ??0?$pair@$$CBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_string@DU?$char_traits@D@std (proto-pdb-public+unit-bracket+callee-bytes) twins 1 origin 5.3-file
- 0x000FED6B ?_Buynode@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_string@D matched no verdict confirmed evidence [bracket-in-56] callee 0x000FED19 ??0_Node@?$_Tree_nod@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_strin (proto-pdb-public+unit-bracket+callee-bytes) twins 1 origin 5.3-file
- 0x000FEDD6 ?_Insert@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_string@DU matched no verdict unconfirmed evidence - twins 1 origin 5.3-file
- 0x000FF242 ?_Erase@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_string@DU? matched no verdict confirmed evidence [indep] site caller 0x000FF27D; [bracket] callee 0x000FED4C ??_G_Node@?$_Tree_nod@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_stri (proto-pdb-public+unit-bracket+callee-bytes) twins 1 origin 5.3-file
- 0x000FF27D ?clear@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_string@DU?$ matched no verdict confirmed evidence [bracket-in-56] callee 0x000FF242 ?_Erase@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$map@V?$basic_string@DU? (proto-pdb-public+unit-bracket+callee-bytes) twins 2 origin 5.3-file
- 0x000E3E3B ?AnimationFinished@UIBagIconMain@@UAEXXZ matched yes verdict unconfirmed evidence - twins 3 origin 5.3-file
- 0x000E43A7 ??0UIHeartIconMain@@QAE@PAVSwampSceneNode@@@Z matched no verdict unconfirmed evidence - twins 3 origin 5.3-file
- 0x000E4429 ?AnimationFinished@UIHeartIconMain@@UAEXXZ matched yes verdict unconfirmed evidence - twins 3 origin 5.3-file
- 0x000E8469 ??_ESkullUI@@UAEPAXI@Z matched no verdict confirmed evidence [bracket] callee 0x000E8485 ??1SkullUI@@UAE@XZ (proto-pdb-public+unit-bracket+callee-bytes) twins 9 origin 5.3-file
- 0x000E8A5E ??_EBeadsUI@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x000E8A7A ??1BeadsUI@@UAE@XZ (proto-pdb:default.pdb) twins 9 origin 5.3-file
- 0x0010089B ??$copy_backward@PAU_XGAME_FIND_DATA@@PAU1@@std@@YAPAU_XGAME_FIND_DATA@@PAU1@00@Z matched no verdict confirmed evidence [indep] callee 0x0010078F ??$_Copy_backward_opt@PAU_XGAME_FIND_DATA@@PAU1@@std@@YAPAU_XGAME_FIND_DATA@@PAU1@00U_Nonscalar_ptr_iterator_tag@0@@Z (proto-pdb:default.pdb) twins 6 origin 5.3-file
- 0x00100B33 ??$_Uninitialized_copy@PAU_XGAME_FIND_DATA@@PAU1@V?$allocator@U_XGAME_FIND_DATA@@@std@@@std@@YAPAU_XGAME_FIND_DATA@@PAU1 matched no verdict confirmed evidence [indep] callee 0x001008B5 ??$_Uninit_copy@PAU_XGAME_FIND_DATA@@PAU1@V?$allocator@U_XGAME_FIND_DATA@@@std@@@std@@YAPAU_XGAME_FIND_DATA@@PAU1@00AAV? (proto-pdb:default.pdb); [indep] site caller 0x00100BDC twins 9 origin 5.3-file
- 0x00100BDC ??$_Ucopy@PAU_XGAME_FIND_DATA@@@?$vector@U_XGAME_FIND_DATA@@V?$allocator@U_XGAME_FIND_DATA@@@std@@@std@@IAEPAU_XGAME_FIN matched no verdict confirmed evidence [bracket-in-56] callee 0x00100B33 ??$_Uninitialized_copy@PAU_XGAME_FIND_DATA@@PAU1@V?$allocator@U_XGAME_FIND_DATA@@@std@@@std@@YAPAU_XGAME_FIND_DATA@@PAU1 (proto-pdb-public+unit-bracket+callee-bytes) twins 4 origin 5.3-file
- 0x00103B06 ?clear@?$_Tree@V?$_Tset_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@U?$less@V?$basic_string@DU? matched no verdict confirmed evidence [indep] callee 0x001035BA ?_Erase@?$_Tree@V?$_Tset_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@U?$less@V?$basic_string@DU (proto-pdb:default.pdb); [indep] site caller 0x00103F75 twins 1 origin 5.3-file
- 0x001041E4 ?clear@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@VAreaStats@@U?$less@V?$basi matched no verdict confirmed evidence [indep] callee 0x00103B2C ?_Erase@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@VAreaStats@@U?$less@V?$bas (proto-pdb:default.pdb); [indep] site caller 0x001043EC twins 1 origin 5.3-file
- 0x00121CB3 ??0RegisterMe@HoldText@@QAE@XZ matched yes verdict unconfirmed evidence - twins 1 origin 5.3-file
- 0x0012D3D2 ??_ESignTrolleyMain@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x0012D3EE ??1SignTrolleyMain@@UAE@XZ (proto-pdb:default.pdb) twins 2 origin 5.3-file
- 0x0012F261 ??_ETrolleyScreen@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x0012F27D ??1TrolleyScreen@@UAE@XZ (proto-pdb:default.pdb) twins 2 origin 5.3-file
- 0x00126DBB ??_ESwingHookTarget@@UAEPAXI@Z matched no verdict unconfirmed evidence [site-corr] callee 0x00126D9A ??1SwingHookTarget@@UAE@XZ (proto-pdb-public+site-corr) twins 1 origin 5.3-file
- 0x0013064C ??_ETrolleyMain@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x00130668 ??1TrolleyMain@@UAE@XZ (proto-pdb:default.pdb) twins 2 origin 5.3-file
- 0x00134E35 ??_EVinceWhammyMain@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x00134E51 ??1VinceWhammyMain@@UAE@XZ (proto-pdb:default.pdb) twins 5 origin 5.3-file
- 0x000A27B2 ??_EVinceLaundryCar@@UAEPAXI@Z matched no verdict confirmed evidence [bracket] callee 0x000A27CE ??1VinceLaundryCar@@UAE@XZ (proto-pdb-public+unit-bracket+callee-bytes) twins 4 origin 5.3-T12.1
- 0x0006C1F2 ??_EBellMain@@UAEPAXI@Z matched no verdict confirmed evidence [bracket] callee 0x0006C20E ??1BellMain@@UAE@XZ (proto-pdb-public+unit-bracket+callee-bytes) twins 1 origin 5.3-T12.1
- 0x0007984C ??_EFlourSackMain@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x00079868 ??1FlourSackMain@@UAE@XZ (proto-pdb:default.pdb) twins 2 origin 5.3-T12.1
- 0x0007B033 ??_EGingerDeadManMain@@UAEPAXI@Z matched no verdict confirmed evidence [bracket] callee 0x0007B04F ??1GingerDeadManMain@@UAE@XZ (proto-pdb-public+unit-bracket+callee-bytes) twins 1 origin 5.3-T12.1
- 0x00011DEC ?clear@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@P6APAVSwampScript@@PAVSwamp matched no verdict unconfirmed evidence [site-corr] callee 0x00011DB1 ?_Erase@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@P6APAVSwampScript@@PAVSwam (proto-pdb-public+site-corr) twins 1 origin 5.3-T12.1
- 0x00049BD5 ??0OnBotRegionMain@@QAE@PAVSwampSceneNode@@@Z matched no verdict unconfirmed evidence - twins 1 origin 5.3-T12.1
- 0x00019905 ??_EOnionMain@@UAEPAXI@Z matched no verdict confirmed evidence [bracket] callee 0x00019921 ??1OnionMain@@UAE@XZ (proto-pdb-public+unit-bracket+callee-bytes) twins 4 origin 5.3-T12.1
- 0x0007B259 ??_EOven@@UAEPAXI@Z matched no verdict unconfirmed evidence [site-corr] callee 0x0007B153 ??1Oven@@UAE@XZ (proto-pdb-public+site-corr) twins 2 origin 5.3-T12.1
- 0x0007B380 ??_EOvenMain@@UAEPAXI@Z matched no verdict confirmed evidence [indep] callee 0x0007B39C ??1OvenMain@@UAE@XZ (proto-pdb:default.pdb) twins 2 origin 5.3-T12.1

## sec F: RegisterMe chain census (5.5 T2.c3, R5.5-003 S1)

### Counts
- anchors: 767 RegisterMe bodies (the 5.4 T11.1 registerme pairs, rebuilt: name_sites amb_pairs over the pre-pin table; literal at +0x0A == proto class for all 767)
- hops walked: registerme 767, creationFunc 767, ctor 712, vtable 767
- hop states: creationFunc/agree 384, creationFunc/boundary 46, creationFunc/conflict 13, creationFunc/unnamed 324, ctor/agree 436, ctor/conflict 12, ctor/unnamed 264, registerme/agree 708, registerme/unnamed 59, vtable/agree 225, vtable/conflict 5, vtable/unnamed 537
- conflict items (hop VA x chain class): 41 (60 lines); on matched rows: 6; by hop creationFunc 18, ctor 16, vtable 7
- not walked: 55 (ctor-inlined (vptr from creationFunc) 55)
- hop VA reached by chains of 2 classes: 0
- plant: 2 new conflict items, 0 lost
- refusals rebuilt: registerme 767 pairs (already-named 4, callee-bytes 59, ok 704); abs-site 94 pairs (already-named 31, ok 63)

### Method
- tables: config/retail/symbols.txt + symbols_pin.txt (any status); `boundary_<VA>` cut pins (C0103) claim no class: hop state `boundary`, not a conflict.
- hop rule: RegisterMe t +0x1A abs = creationFunc; ctor = the call/jmp with ecx = operator new's result; vptr = last `mov dword [this], imm32` (disp 0) into .rdata/.data in the ctor (or in the creationFunc when the ctor is inlined).
- conflict `at-va`: the hop VA's table names, none of them the expected prefix for class X (`??0RegisterMe@X@@`, `?creationFunc@X@@`, `??0X@@`, `??_7X@@6B`). `elsewhere <VA>`: the proto twin's same hop public (same walk on the proto) sits at another retail VA.
- matched flag: the hop VA or the named VA in config/matched_set.toml, or the name a matched symbol.

### Conflicts (one line per item: hop, hop VA, chain class X, RegisterMe VA, table names, matched)
- creationFunc 0x0001ED7B X=ConveyorAMain RegisterMe 0x00020939: at-va ?creationFunc@ConveyorBMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x0001D5F7 X=ConveyorAMain RegisterMe 0x00020939: at-va ??0ConveyorBMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- creationFunc 0x0001EDA2 X=ConveyorBMain RegisterMe 0x000209B1: at-va ?creationFunc@ConveyorCMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt]; elsewhere 0x0001ED7B ?creationFunc@ConveyorBMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x0001D63A X=ConveyorBMain RegisterMe 0x000209B1: at-va ??0ConveyorCMain@@QAE@PAVSwampSceneNode@@@Z [pin]; elsewhere 0x0001D5F7 ??0ConveyorBMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- creationFunc 0x0001EDC9 X=ConveyorCMain RegisterMe 0x00020A29: at-va ?creationFunc@ConveyorDMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt]; elsewhere 0x0001EDA2 ?creationFunc@ConveyorCMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x0001D67D X=ConveyorCMain RegisterMe 0x00020A29: at-va ??0ConveyorDMain@@QAE@PAVSwampSceneNode@@@Z [pin]; elsewhere 0x0001D63A ??0ConveyorCMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- creationFunc 0x0001EDF0 X=ConveyorDMain RegisterMe 0x00020AA1: at-va ?creationFunc@ConveyorEMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt]; elsewhere 0x0001EDC9 ?creationFunc@ConveyorDMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x0001D6C0 X=ConveyorDMain RegisterMe 0x00020AA1: at-va ??0ConveyorEMain@@QAE@PAVSwampSceneNode@@@Z [pin]; elsewhere 0x0001D67D ??0ConveyorDMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- creationFunc 0x0001EE17 X=ConveyorEMain RegisterMe 0x00020B19: at-va ?creationFunc@ConveyorFMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt]; elsewhere 0x0001EDF0 ?creationFunc@ConveyorEMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x0001D703 X=ConveyorEMain RegisterMe 0x00020B19: at-va ??0ConveyorFMain@@QAE@PAVSwampSceneNode@@@Z [pin]; elsewhere 0x0001D6C0 ??0ConveyorEMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- creationFunc 0x0001EE3E X=ConveyorFMain RegisterMe 0x00020B91: at-va ?creationFunc@ConveyorGMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt]; elsewhere 0x0001EE17 ?creationFunc@ConveyorFMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x0001D746 X=ConveyorFMain RegisterMe 0x00020B91: at-va ??0ConveyorGMain@@QAE@PAVSwampSceneNode@@@Z [pin]; elsewhere 0x0001D703 ??0ConveyorFMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- creationFunc 0x0001EE65 X=ConveyorGMain RegisterMe 0x00020C09: elsewhere 0x0001EE3E ?creationFunc@ConveyorGMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x0001D789 X=ConveyorGMain RegisterMe 0x00020C09: elsewhere 0x0001D746 ??0ConveyorGMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- creationFunc 0x000217DB X=Activator_DockAMain RegisterMe 0x00021E33: at-va ?creationFunc@Activator_DockBMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x000215A5 X=Activator_DockAMain RegisterMe 0x00021E33: at-va ??0Activator_DockBMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- creationFunc 0x00021805 X=Activator_DockBMain RegisterMe 0x00021E6F: elsewhere 0x000217DB ?creationFunc@Activator_DockBMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x000215BD X=Activator_DockBMain RegisterMe 0x00021E6F: elsewhere 0x000215A5 ??0Activator_DockBMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- creationFunc 0x0005FB63 X=MoleTomb_AMain RegisterMe 0x0005FFE0: elsewhere 0x0005FC5F ?creationFunc@MoleTomb_AMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] matched | matched yes
- ctor 0x0005F9E5 X=MoleTomb_AMain RegisterMe 0x0005FFE0: elsewhere 0x0005FA75 ??0MoleTomb_AMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- vtable 0x001D8360 X=MoleTomb_AMain RegisterMe 0x0005FFE0: elsewhere 0x001D8600 ??_7MoleTomb_AMain@@6B@ [pin] | matched no
- creationFunc 0x0005FC5F X=MoleTomb_GMain RegisterMe 0x00060148: at-va ?creationFunc@MoleTomb_AMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] matched | matched yes
- ctor 0x0005FA75 X=MoleTomb_GMain RegisterMe 0x00060148: at-va ??0MoleTomb_AMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- vtable 0x001D8600 X=MoleTomb_GMain RegisterMe 0x00060148: at-va ??_7MoleTomb_AMain@@6B@ [pin] | matched no
- creationFunc 0x00071769 X=DialogueJazzRegionMain RegisterMe 0x00074C64: at-va ?creationFunc@DialoguePawnRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] matched | matched yes
- ctor 0x000716C7 X=DialogueJazzRegionMain RegisterMe 0x00074C64: at-va ??0DialoguePawnRegionMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- vtable 0x001DBDD8 X=DialogueJazzRegionMain RegisterMe 0x00074C64: at-va ??_7DialoguePawnRegionMain@@6B@ [pin] | matched no
- creationFunc 0x00071793 X=DialoguePawnRegionMain RegisterMe 0x00074CA0: at-va ?creationFunc@DialogueCostumeRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] matched; elsewhere 0x00071769 ?creationFunc@DialoguePawnRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] matched | matched yes
- ctor 0x000716DF X=DialoguePawnRegionMain RegisterMe 0x00074CA0: at-va ??0DialogueCostumeRegionMain@@QAE@PAVSwampSceneNode@@@Z [pin]; elsewhere 0x000716C7 ??0DialoguePawnRegionMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- vtable 0x001DBE40 X=DialoguePawnRegionMain RegisterMe 0x00074CA0: at-va ??_7DialogueCostumeRegionMain@@6B@ [pin]; elsewhere 0x001DBDD8 ??_7DialoguePawnRegionMain@@6B@ [pin] | matched no
- creationFunc 0x000717BD X=DialogueCostumeRegionMain RegisterMe 0x00074CDC: at-va ?creationFunc@DialogueBurlesqueRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] matched; elsewhere 0x00071793 ?creationFunc@DialogueCostumeRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] matched | matched yes
- ctor 0x000716F7 X=DialogueCostumeRegionMain RegisterMe 0x00074CDC: at-va ??0DialogueBurlesqueRegionMain@@QAE@PAVSwampSceneNode@@@Z [pin]; elsewhere 0x000716DF ??0DialogueCostumeRegionMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- vtable 0x001DBEA8 X=DialogueCostumeRegionMain RegisterMe 0x00074CDC: at-va ??_7DialogueBurlesqueRegionMain@@6B@ [pin]; elsewhere 0x001DBE40 ??_7DialogueCostumeRegionMain@@6B@ [pin] | matched no
- creationFunc 0x000717E7 X=DialogueBurlesqueRegionMain RegisterMe 0x00074D18: at-va ?creationFunc@DialogueMovieRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt]; elsewhere 0x000717BD ?creationFunc@DialogueBurlesqueRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] matched | matched yes
- ctor 0x0007170F X=DialogueBurlesqueRegionMain RegisterMe 0x00074D18: at-va ??0DialogueMovieRegionMain@@QAE@PAVSwampSceneNode@@@Z [pin]; elsewhere 0x000716F7 ??0DialogueBurlesqueRegionMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- vtable 0x001DBF10 X=DialogueBurlesqueRegionMain RegisterMe 0x00074D18: at-va ??_7DialogueMovieRegionMain@@6B@ [pin]; elsewhere 0x001DBEA8 ??_7DialogueBurlesqueRegionMain@@6B@ [pin] | matched no
- creationFunc 0x00071811 X=DialogueMovieRegionMain RegisterMe 0x00074D54: elsewhere 0x000717E7 ?creationFunc@DialogueMovieRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- ctor 0x00071727 X=DialogueMovieRegionMain RegisterMe 0x00074D54: elsewhere 0x0007170F ??0DialogueMovieRegionMain@@QAE@PAVSwampSceneNode@@@Z [pin] | matched no
- vtable 0x001DBF78 X=DialogueMovieRegionMain RegisterMe 0x00074D54: elsewhere 0x001DBF10 ??_7DialogueMovieRegionMain@@6B@ [pin] | matched no
- creationFunc 0x0008CAC3 X=BellCMain RegisterMe 0x0008E30B: at-va ?creationFunc@Camera_OptionsMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no
- creationFunc 0x00138A20 X=Camera_OptionsMain RegisterMe 0x0013A836: elsewhere 0x0008CAC3 ?creationFunc@Camera_OptionsMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z [symbols.txt] | matched no

### Not walked (one line per hop)
- ctor X=Crawdaddy RegisterMe 0x0002DAB1 creationFunc 0x0002C027: ctor-inlined (vptr from creationFunc)
- ctor X=ClockHandLittle RegisterMe 0x00085CC8 creationFunc 0x00084261: ctor-inlined (vptr from creationFunc)
- ctor X=ClockHandBig RegisterMe 0x00085D04 creationFunc 0x00084035: ctor-inlined (vptr from creationFunc)
- ctor X=ClockFigureDay RegisterMe 0x00085E30 creationFunc 0x000842B9: ctor-inlined (vptr from creationFunc)
- ctor X=ClockFigureNight RegisterMe 0x00085EA8 creationFunc 0x000842EC: ctor-inlined (vptr from creationFunc)
- ctor X=Cymbals RegisterMe 0x0007E251 creationFunc 0x0007E1FA: ctor-inlined (vptr from creationFunc)
- ctor X=FlourSack RegisterMe 0x0007A0FD creationFunc 0x0007A055: ctor-inlined (vptr from creationFunc)
- ctor X=CanGas RegisterMe 0x000813FD creationFunc 0x00081355: ctor-inlined (vptr from creationFunc)
- ctor X=HiStrkerWeight RegisterMe 0x0003D99B creationFunc 0x0003D8F0: ctor-inlined (vptr from creationFunc)
- ctor X=CannonA RegisterMe 0x00050D7F creationFunc 0x0004FC97: ctor-inlined (vptr from creationFunc)
- ctor X=CannonB RegisterMe 0x00050DBB creationFunc 0x0004FCCA: ctor-inlined (vptr from creationFunc)
- ctor X=CannonC RegisterMe 0x00050DF7 creationFunc 0x0004FCFD: ctor-inlined (vptr from creationFunc)
- ctor X=CannonD RegisterMe 0x00050E33 creationFunc 0x0004FD30: ctor-inlined (vptr from creationFunc)
- ctor X=CannonE RegisterMe 0x00050E6F creationFunc 0x0004FD63: ctor-inlined (vptr from creationFunc)
- ctor X=Onion RegisterMe 0x00019A3D creationFunc 0x0001824A: ctor-inlined (vptr from creationFunc)
- ctor X=GasCanister RegisterMe 0x0009EDA0 creationFunc 0x0009ECF8: ctor-inlined (vptr from creationFunc)
- ctor X=NeedleHat RegisterMe 0x0004229E creationFunc 0x0003F9DC: ctor-inlined (vptr from creationFunc)
- ctor X=Spring RegisterMe 0x0005C3D7 creationFunc 0x0005C3A7: ctor-inlined (vptr from creationFunc)
- ctor X=Dynamite RegisterMe 0x0009D33D creationFunc 0x0009D215: ctor-inlined (vptr from creationFunc)
- ctor X=BellTrophy RegisterMe 0x0008E257 creationFunc 0x0008E032: ctor-inlined (vptr from creationFunc)
- ctor X=GhostBridge RegisterMe 0x0008E383 creationFunc 0x0008CD7E: ctor-inlined (vptr from creationFunc)
- ctor X=ZombieHulaGirl RegisterMe 0x0005BA7D creationFunc 0x00057CD9: ctor-inlined (vptr from creationFunc)
- ctor X=ZombieBaker RegisterMe 0x0005BAF5 creationFunc 0x00057D09: ctor-inlined (vptr from creationFunc)
- ctor X=ZombieClown RegisterMe 0x0005BB6D creationFunc 0x00057D39: ctor-inlined (vptr from creationFunc)
- ctor X=ZombieBelle RegisterMe 0x0005BBE5 creationFunc 0x00057D69: ctor-inlined (vptr from creationFunc)
- ctor X=ZombieFootball RegisterMe 0x0005BC5D creationFunc 0x00057D9C: ctor-inlined (vptr from creationFunc)
- ctor X=CryptHula RegisterMe 0x0005BCD5 creationFunc 0x00057DCF: ctor-inlined (vptr from creationFunc)
- ctor X=CryptBaker RegisterMe 0x0005BD4D creationFunc 0x00057E02: ctor-inlined (vptr from creationFunc)
- ctor X=CryptClown RegisterMe 0x0005BDC5 creationFunc 0x00057E35: ctor-inlined (vptr from creationFunc)
- ctor X=CryptBelle RegisterMe 0x0005BE3D creationFunc 0x00057E68: ctor-inlined (vptr from creationFunc)
- ctor X=CryptFootball RegisterMe 0x0005BEB5 creationFunc 0x00057E9B: ctor-inlined (vptr from creationFunc)
- ctor X=MasqueradeMask RegisterMe 0x0007464C creationFunc 0x0007447F: ctor-inlined (vptr from creationFunc)
- ctor X=Money RegisterMe 0x0007473C creationFunc 0x000743F5: ctor-inlined (vptr from creationFunc)
- ctor X=Popcorn RegisterMe 0x000747B4 creationFunc 0x00074425: ctor-inlined (vptr from creationFunc)
- ctor X=FireBug RegisterMe 0x0011C6E1 creationFunc 0x0011AF0A: ctor-inlined (vptr from creationFunc)
- ctor X=SignTrolley RegisterMe 0x0012F51D creationFunc 0x0012CE30: ctor-inlined (vptr from creationFunc)
- ctor X=Trolley RegisterMe 0x00130CE3 creationFunc 0x00130455: ctor-inlined (vptr from creationFunc)
- ctor X=HeadslamWeapon RegisterMe 0x000FC3EA creationFunc 0x000FBB2C: ctor-inlined (vptr from creationFunc)
- ctor X=ConveyorA RegisterMe 0x00020975 creationFunc 0x0001CBAC: ctor-inlined (vptr from creationFunc)
- ctor X=ConveyorB RegisterMe 0x000209ED creationFunc 0x0001CBDC: ctor-inlined (vptr from creationFunc)
- ctor X=ConveyorC RegisterMe 0x00020A65 creationFunc 0x0001CC0C: ctor-inlined (vptr from creationFunc)
- ctor X=ConveyorD RegisterMe 0x00020ADD creationFunc 0x0001CC3C: ctor-inlined (vptr from creationFunc)
- ctor X=ConveyorE RegisterMe 0x00020B55 creationFunc 0x0001CC6C: ctor-inlined (vptr from creationFunc)
- ctor X=ConveyorF RegisterMe 0x00020BCD creationFunc 0x0001CC9C: ctor-inlined (vptr from creationFunc)
- ctor X=ConveyorG RegisterMe 0x00020C45 creationFunc 0x0001CCCC: ctor-inlined (vptr from creationFunc)
- ctor X=ConveyorH RegisterMe 0x00020D35 creationFunc 0x0001CE0E: ctor-inlined (vptr from creationFunc)
- ctor X=Mouse RegisterMe 0x00020D71 creationFunc 0x00020631: ctor-inlined (vptr from creationFunc)
- ctor X=Raccoon RegisterMe 0x00020DE9 creationFunc 0x000205CB: ctor-inlined (vptr from creationFunc)
- ctor X=Beaver RegisterMe 0x00020E61 creationFunc 0x000205FE: ctor-inlined (vptr from creationFunc)
- ctor X=FortuneCoin RegisterMe 0x0003D0C5 creationFunc 0x0003D06E: ctor-inlined (vptr from creationFunc)
- ctor X=BowlingBall RegisterMe 0x0003E044 creationFunc 0x0003E014: ctor-inlined (vptr from creationFunc)
- ctor X=Apple RegisterMe 0x000869F4 creationFunc 0x00086988: ctor-inlined (vptr from creationFunc)
- ctor X=ZapHat RegisterMe 0x0008C6CF creationFunc 0x0008C678: ctor-inlined (vptr from creationFunc)
- ctor X=RecordAlbum RegisterMe 0x000942A0 creationFunc 0x0009411D: ctor-inlined (vptr from creationFunc)
- ctor X=ShrimpSub RegisterMe 0x0014E4B9 creationFunc 0x0014B9F6: ctor-inlined (vptr from creationFunc)

### I81: owner of 0x000717E7
- chain: `??0RegisterMe@DialogueBurlesqueRegionMain@@QAE@XZ` 0x00074D18 +0x1A -> 0x000717E7 (creationFunc) -> ctor 0x0007170F -> vptr 0x001DBF10. Owner: `?creationFunc@DialogueBurlesqueRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z`, class DialogueBurlesqueRegionMain.
- symbols.txt gives 0x000717E7 `?creationFunc@DialogueMovieRegionMain@@...`: wrong by the chain; DialogueMovieRegionMain's chain is RegisterMe 0x00074D54 +0x1A -> 0x00071811 -> ctor 0x00071727 -> vptr 0x001DBF78 (all unnamed by class today).
- same one-slot shift on the neighbours (conflict list): creationFunc 0x00071769/0x00071793/0x000717BD/0x000717E7, ctors 0x000716C7..0x0007170F and vptrs 0x001DBDD8..0x001DBF10 each carry the next class's name (DialogueJazz -> Pawn -> Costume -> Burlesque -> Movie).

### Refusal buckets (5.4 T11.1, validation.md:3008)
#### registerme callee-bytes (59)
- masked bytes equal in all 59 (same mask list, no unmasked difference); the refusing check is the callee name rule at +0x1A in all 59 (pin_rows callee `callee-name-rule` on HEAD; boundary names present in the pre-pin table too): boundary 46, other-class-name 13. No stub, no real byte diff.
- boundary: ??0RegisterMe@AllSeeingEyeMain@@QAE@XZ 0x0013769D +0x1A creationFunc 0x00136D97 named boundary_00136D97 (proto ?creationFunc@AllSeeingEyeMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@Bead@@QAE@XZ 0x00126C20 +0x1A creationFunc 0x001260DA named boundary_001260DA (proto ?creationFunc@Bead@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@BoatgateRegion@@QAE@XZ 0x000227E2 +0x1A creationFunc 0x000220C6 named boundary_000220C6 (proto ?creationFunc@BoatgateRegion@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@BoxGateHintRegionMain@@QAE@XZ 0x000A0D82 +0x1A creationFunc 0x0009FA90 named boundary_0009FA90 (proto ?creationFunc@BoxGateHintRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@Camera_ContinueAMain@@QAE@XZ 0x0013A566 +0x1A creationFunc 0x00138852 named boundary_00138852 (proto ?creationFunc@Camera_ContinueAMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@Cannon@@QAE@XZ 0x0005E1F5 +0x1A creationFunc 0x0005D58C named boundary_0005D58C (proto ?creationFunc@Cannon@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@CannonA@@QAE@XZ 0x00050D7F +0x1A creationFunc 0x0004FC97 named boundary_0004FC97 (proto ?creationFunc@CannonA@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@CrawdadTrapMain@@QAE@XZ 0x0002DA75 +0x1A creationFunc 0x0002D7EE named boundary_0002D7EE (proto ?creationFunc@CrawdadTrapMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@CrawdaddyHintRegionMain@@QAE@XZ 0x0002DC55 +0x1A creationFunc 0x0002C0E1 named boundary_0002C0E1 (proto ?creationFunc@CrawdaddyHintRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@CrawdaddyMain@@QAE@XZ 0x0002D9FD +0x1A creationFunc 0x0002D347 named boundary_0002D347 (proto ?creationFunc@CrawdaddyMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@DemoLauncher@@QAE@XZ 0x0013E5CF +0x1A creationFunc 0x0013D6F1 named boundary_0013D6F1 (proto ?creationFunc@DemoLauncher@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@DockSubMain@@QAE@XZ 0x0002DA39 +0x1A creationFunc 0x0002D995 named boundary_0002D995 (proto ?creationFunc@DockSubMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@DoorServiceMain@@QAE@XZ 0x00050EAB +0x1A creationFunc 0x000508FC named boundary_000508FC (proto ?creationFunc@DoorServiceMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@EvilVinceMirrorMain@@QAE@XZ 0x00044408 +0x1A creationFunc 0x00043EB5 named boundary_00043EB5 (proto ?creationFunc@EvilVinceMirrorMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@FlamePipesMain@@QAE@XZ 0x0009EE18 +0x1A creationFunc 0x0009E621 named boundary_0009E621 (proto ?creationFunc@FlamePipesMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@FlameWallMain@@QAE@XZ 0x0009EE54 +0x1A creationFunc 0x0009D917 named boundary_0009D917 (proto ?creationFunc@FlameWallMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@FlourSack@@QAE@XZ 0x0007A0FD +0x1A creationFunc 0x0007A055 named boundary_0007A055 (proto ?creationFunc@FlourSack@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@GasCanister@@QAE@XZ 0x0009EDA0 +0x1A creationFunc 0x0009ECF8 named boundary_0009ECF8 (proto ?creationFunc@GasCanister@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@GasPuddleRegionMain@@QAE@XZ 0x000814B1 +0x1A creationFunc 0x0007F3A1 named boundary_0007F3A1 (proto ?creationFunc@GasPuddleRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@GateSmallMain@@QAE@XZ 0x000227A6 +0x1A creationFunc 0x00022740 named boundary_00022740 (proto ?creationFunc@GateSmallMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@GingerDeadMan@@QAE@XZ 0x0007B0A2 +0x1A creationFunc 0x0007A206 named boundary_0007A206 (proto ?creationFunc@GingerDeadMan@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@HannahDebris@@QAE@XZ 0x000286FC +0x1A creationFunc 0x0002778D named boundary_0002778D (proto ?creationFunc@HannahDebris@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@HannahStartBattleRegionMain@@QAE@XZ 0x0002B429 +0x1A creationFunc 0x0002924A named boundary_0002924A (proto ?creationFunc@HannahStartBattleRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@Horn@@QAE@XZ 0x000746C4 +0x1A creationFunc 0x000743D3 named boundary_000743D3 (proto ?creationFunc@Horn@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@IrisCityMain@@QAE@XZ 0x000AAD52 +0x1A creationFunc 0x000AA74C named boundary_000AA74C (proto ?creationFunc@IrisCityMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@MovieTheaterRegionMain@@QAE@XZ 0x000749D0 +0x1A creationFunc 0x000712FC named boundary_000712FC (proto ?creationFunc@MovieTheaterRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@OnBotRegionMain@@QAE@XZ 0x0004DD03 +0x1A creationFunc 0x00049C33 named boundary_00049C33 (proto ?creationFunc@OnBotRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@Onion@@QAE@XZ 0x00019A3D +0x1A creationFunc 0x0001824A named boundary_0001824A (proto ?creationFunc@Onion@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@OuthouseDoorRegionMain@@QAE@XZ 0x00022D54 +0x1A creationFunc 0x00022936 named boundary_00022936 (proto ?creationFunc@OuthouseDoorRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@Oven@@QAE@XZ 0x0007B9DF +0x1A creationFunc 0x0007B1A9 named boundary_0007B1A9 (proto ?creationFunc@Oven@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@PlanGoesAwryCutscene@@QAE@XZ 0x000AA57D +0x1A creationFunc 0x000AA4ED named boundary_000AA4ED (proto ?creationFunc@PlanGoesAwryCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@PropellerMain@@QAE@XZ 0x000A4634 +0x1A creationFunc 0x000A4470 named boundary_000A4470 (proto ?creationFunc@PropellerMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@QuickCanisterMain@@QAE@XZ 0x0009EECC +0x1A creationFunc 0x0009E9FC named boundary_0009E9FC (proto ?creationFunc@QuickCanisterMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@RockImpactWeapon@@QAE@XZ 0x000687BA +0x1A creationFunc 0x0006810B named boundary_0006810B (proto ?creationFunc@RockImpactWeapon@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@RockImpactWeaponMain@@QAE@XZ 0x000687F6 +0x1A creationFunc 0x00067BCF named boundary_00067BCF (proto ?creationFunc@RockImpactWeaponMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@SignTrolley@@QAE@XZ 0x0012F51D +0x1A creationFunc 0x0012CE30 named boundary_0012CE30 (proto ?creationFunc@SignTrolley@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@StartBattleRegionMain@@QAE@XZ 0x00068832 +0x1A creationFunc 0x0006405E named boundary_0006405E (proto ?creationFunc@StartBattleRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@Switch_WheelPlatformAMain@@QAE@XZ 0x000A0BF1 +0x1A creationFunc 0x000A0677 named boundary_000A0677 (proto ?creationFunc@Switch_WheelPlatformAMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@TracerMain@@QAE@XZ 0x0004E8E8 +0x1A creationFunc 0x0004E7BB named boundary_0004E7BB (proto ?creationFunc@TracerMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@Trolley@@QAE@XZ 0x00130CE3 +0x1A creationFunc 0x00130455 named boundary_00130455 (proto ?creationFunc@Trolley@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@UIPageMain@@QAE@XZ 0x000E9C2E +0x1A creationFunc 0x000E2D35 named boundary_000E2D35 (proto ?creationFunc@UIPageMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@VinceEnterBumperCar@@QAE@XZ 0x00049A14 +0x1A creationFunc 0x00048405 named boundary_00048405 (proto ?creationFunc@VinceEnterBumperCar@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@VinceInWindmill@@QAE@XZ 0x00028774 +0x1A creationFunc 0x00028542 named boundary_00028542 (proto ?creationFunc@VinceInWindmill@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@VinceThrownFromCar@@QAE@XZ 0x00049B7C +0x1A creationFunc 0x00048C7C named boundary_00048C7C (proto ?creationFunc@VinceThrownFromCar@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- boundary: ??0RegisterMe@ZombieHulaGirl@@QAE@XZ 0x0005BA7D +0x1A creationFunc 0x00057CD9 named boundary_00057CD9 (proto ?creationFunc@ZombieHulaGirl@@CAPAVSwampSceneNode@@XZ)
- boundary: ??0RegisterMe@potion_boomerangMain@@QAE@XZ 0x0012349D +0x1A creationFunc 0x0012343A named boundary_0012343A (proto ?creationFunc@potion_boomerangMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@Activator_DockAMain@@QAE@XZ 0x00021E33 +0x1A creationFunc 0x000217DB named ?creationFunc@Activator_DockBMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@Activator_DockAMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@BellCMain@@QAE@XZ 0x0008E30B +0x1A creationFunc 0x0008CAC3 named ?creationFunc@Camera_OptionsMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@BellCMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@ConveyorAMain@@QAE@XZ 0x00020939 +0x1A creationFunc 0x0001ED7B named ?creationFunc@ConveyorBMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@ConveyorAMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@ConveyorBMain@@QAE@XZ 0x000209B1 +0x1A creationFunc 0x0001EDA2 named ?creationFunc@ConveyorCMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@ConveyorBMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@ConveyorCMain@@QAE@XZ 0x00020A29 +0x1A creationFunc 0x0001EDC9 named ?creationFunc@ConveyorDMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@ConveyorCMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@ConveyorDMain@@QAE@XZ 0x00020AA1 +0x1A creationFunc 0x0001EDF0 named ?creationFunc@ConveyorEMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@ConveyorDMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@ConveyorEMain@@QAE@XZ 0x00020B19 +0x1A creationFunc 0x0001EE17 named ?creationFunc@ConveyorFMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@ConveyorEMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@ConveyorFMain@@QAE@XZ 0x00020B91 +0x1A creationFunc 0x0001EE3E named ?creationFunc@ConveyorGMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@ConveyorFMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@DialogueBurlesqueRegionMain@@QAE@XZ 0x00074D18 +0x1A creationFunc 0x000717E7 named ?creationFunc@DialogueMovieRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@DialogueBurlesqueRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@DialogueCostumeRegionMain@@QAE@XZ 0x00074CDC +0x1A creationFunc 0x000717BD named ?creationFunc@DialogueBurlesqueRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@DialogueCostumeRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@DialogueJazzRegionMain@@QAE@XZ 0x00074C64 +0x1A creationFunc 0x00071769 named ?creationFunc@DialoguePawnRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@DialogueJazzRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@DialoguePawnRegionMain@@QAE@XZ 0x00074CA0 +0x1A creationFunc 0x00071793 named ?creationFunc@DialogueCostumeRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@DialoguePawnRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)
- other-class-name: ??0RegisterMe@MoleTomb_GMain@@QAE@XZ 0x00060148 +0x1A creationFunc 0x0005FC5F named ?creationFunc@MoleTomb_AMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (proto ?creationFunc@MoleTomb_GMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z)

#### abs-site already-named (31)
- chain-agrees 13, no-chain-name-equals-proto-public 7, no-chain-vacuous 11; abs-site rebuilt over pre-pin + the 704-line registerme block (callers read at run time, as T11.1 ran).
- chain-agrees: 0x0006C70D ?creationFunc@BellMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x0006CA57, X=BellMain)
- chain-agrees: 0x000795AC ?creationFunc@DoorStorageRoomMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x0007960F, X=DoorStorageRoomMain)
- chain-agrees: 0x00030588 ?creationFunc@VinceExitBoat@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x0003298B, X=VinceExitBoat)
- chain-agrees: 0x0001106C ?creationFunc@FrenchquarterLevelMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x00011F59, X=FrenchquarterLevelMain)
- chain-agrees: 0x00011090 ?creationFunc@CryptCityLevelMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x00011F95, X=CryptCityLevelMain)
- chain-agrees: 0x000A0650 ?creationFunc@ArmorDilloChuteMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x000A0BB5, X=ArmorDilloChuteMain)
- chain-agrees: 0x0009ECAD ?creationFunc@RocketLabRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x0009ED28, X=RocketLabRegionMain)
- chain-agrees: 0x0008B9DA ?creationFunc@SwitchMuseumMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x0008BA01, X=SwitchMuseumMain)
- chain-agrees: 0x0007ECF4 ?creationFunc@SwitchSafeMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x0007ED29, X=SwitchSafeMain)
- chain-agrees: 0x0007C457 ?creationFunc@WallSwitchMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x0007C47E, X=WallSwitchMain)
- chain-agrees: 0x001266E2 ?creationFunc@Bead3Main@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x00126D4C, X=Bead3Main)
- chain-agrees: 0x00122122 ?creationFunc@DialogueRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x0012257A, X=DialogueRegionMain)
- chain-agrees: 0x00121A2B ?creationFunc@FireRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z (creationFunc of RegisterMe 0x00121A4F, X=FireRegionMain)
- no-chain-name-equals-proto-public: 0x00155DFC ?ssm@?1??Instance@SwampScriptManager@@SAAAV1@XZ@$AV1@A (proto 0x0053F396; not a RegisterMe hop)
- no-chain-name-equals-proto-public: 0x00155E06 ?ssm@?1??Instance@SwampNodeManager@@SAAAV1@XZ@$AV1@A (proto 0x0053F3A0; not a RegisterMe hop)
- no-chain-name-equals-proto-public: 0x00155E1A ?ll@?1??Instance@LevelLoader@@SAAAV1@XZ@$AV1@A (proto 0x0053F3B4; not a RegisterMe hop)
- no-chain-name-equals-proto-public: 0x00155E24 ?ls@?1??Instance@LocalizedStrings@@SAAAV1@XZ@$AV1@A (proto 0x0053F3BE; not a RegisterMe hop)
- no-chain-name-equals-proto-public: 0x00155E8A ?at@?1??GetAllocationTracker@AllocationTracker@@SAAAV1@XZ@$AV1@A (proto 0x0053F424; not a RegisterMe hop)
- no-chain-vacuous: 0x00155EBC proto 0x0053F456 has no public; the body has no table name (refusal is vacuous)
- no-chain-vacuous: 0x00155EC9 proto 0x0053F463 has no public; the body has no table name (refusal is vacuous)
- no-chain-name-equals-proto-public: 0x00155EEA ?p@?1??Instance@ProfileBar@@SAAAV1@XZ@$AV1@A (proto 0x0053F484; not a RegisterMe hop)
- no-chain-name-equals-proto-public: 0x00155EF4 ?nullBsps@?1??GetBSPs@GatorNode@@QBEABV?$vector@PAVBSP@@V?$allocator@PAVBSP@@@std@@@std@@XZ@$AV23@A (proto 0x0053F48E; not a RegisterMe hop)
- no-chain-vacuous: 0x00155F26 proto 0x0053F4C0 has no public; the body has no table name (refusal is vacuous)
- no-chain-vacuous: 0x00155F58 proto 0x0053F4F2 has no public; the body has no table name (refusal is vacuous)
- no-chain-vacuous: 0x00155F65 proto 0x0053F4FF has no public; the body has no table name (refusal is vacuous)
- no-chain-vacuous: 0x00155F72 proto 0x0053F50C has no public; the body has no table name (refusal is vacuous)
- no-chain-vacuous: 0x00155F7F proto 0x0053F519 has no public; the body has no table name (refusal is vacuous)
- no-chain-vacuous: 0x00155F8C proto 0x0053F526 has no public; the body has no table name (refusal is vacuous)
- no-chain-vacuous: 0x00155F99 proto 0x0053F533 has no public; the body has no table name (refusal is vacuous)
- no-chain-vacuous: 0x00156030 proto 0x0053F5CA has no public; the body has no table name (refusal is vacuous)
- no-chain-vacuous: 0x0015607D proto 0x0053F617 has no public; the body has no table name (refusal is vacuous)

### Plant
- tables copied to .run/5.5/t2/plant/; symbols.txt copy: the names of creationFunc rows 0x00011045 (OnionTrapRegionMain) and 0x00018226 (VoodooShopLevelMain) swapped (both `agree` on HEAD).
- result: 2 new conflict items, 0 lost, others unchanged:
  - creationFunc 0x00011045 X=VoodooShopLevelMain: at-va ?creationFunc@OnionTrapRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z; elsewhere 0x00018226 ?creationFunc@VoodooShopLevelMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z
  - creationFunc 0x00018226 X=OnionTrapRegionMain: at-va ?creationFunc@VoodooShopLevelMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z; elsewhere 0x00011045 ?creationFunc@OnionTrapRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z

## sec G: M30 slot-names `??_7` rows (5.5 T2.c3, R5.5-003 S2)

### Counts
- rows: 60 (pin rows tagged `+slot-names`; validation.md:2709 retag 60)
- classes: chain 36, site 24; slot 0, circular 0 unless listed
- chain conflicts 0; rows with a conflicting site 0; rows with slot evidence (independent) 50; rows with no chain, no site, no slot 0
- equal-slot twins (another proto `??_7` with the same proto slot targets): 12 rows

### Method
- order: chain (census F vptr hop == VA, class == Y) > site (abs site in a table func row, not site-corr, no fails, one public, masked-equal; proto dest == the row's public; no conflicting site) > slot (a slot's retail row of class Y, tag without fold, one proto public, size >= 8, no fails) > circular.
- site callers come from symbols.txt (all `unverified`, R5.5-003 S1) and pin rows; every site row below names its caller.

### Rows (one line per row: VA, name, class, evidence, twins)
- 0x001CB0F8 ??_7Switch_DoorOuthouseMain@@6B@ chain: chain RegisterMe 0x00022D18; sites agree 1 conflict 0; slot-ev 3 of 28
- 0x001CB268 ??_7Switch_ShipDoorMain@@6B@ chain: chain RegisterMe 0x00022F85; sites agree 1 conflict 0; slot-ev 0 of 28; twins 10: ??_7SwitchSausageMain@@6B@ ??_7Switch_ElevatorEWWMain@@6B@ ??_7SwitchSwampMain@@6B@ ??_7Switch_DrawBridgeMain@@6B@ ??_7Switch_TrapMain@@6B@ ??_7Switch_GateMain@@6B@ ??_7SwitchCarnivalMain@@6B@ ??_7Switch_TrailerFrogsMain@@6B@ ??_7Switch_ElevatorPropellorRoomMain@@6B@ ??_7BasicWallSwitchMain@@6B@
- 0x001CC3D8 ??_7LighthouseMain@@6B@ chain: chain RegisterMe 0x000287EC; sites agree 1 conflict 0; slot-ev 4 of 28
- 0x001CC798 ??_7WindyBits@@6B@ site: site 0x00029288 ??1WindyBits@@UAE@XZ (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 1 of 9
- 0x001CC828 ??_7VinceBlowBack@@6B@ chain: chain RegisterMe 0x0002B296; sites agree 1 conflict 0; slot-ev 1 of 28
- 0x001CCBC0 ??_7FanHurricaneMain@@6B@ chain: chain RegisterMe 0x0002B7F4; sites agree 1 conflict 0; slot-ev 3 of 28
- 0x001CCEC0 ??_7CrawdaddyHintRegionMain@@6B@ chain: chain RegisterMe 0x0002DC55; sites agree 1 conflict 0; slot-ev 1 of 25
- 0x001CD850 ??_7FanBoatWake@@6B@ site: site 0x000306E1 ??0FanBoatWake@@QAE@XZ (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 2 of 9
- 0x001CF550 ??_7Switch_TrailerFrogsMain@@6B@ chain: chain RegisterMe 0x0003DE21; sites agree 1 conflict 0; slot-ev 0 of 28; twins 10: ??_7SwitchSausageMain@@6B@ ??_7Switch_ElevatorEWWMain@@6B@ ??_7Switch_ShipDoorMain@@6B@ ??_7SwitchSwampMain@@6B@ ??_7Switch_DrawBridgeMain@@6B@ ??_7Switch_TrapMain@@6B@ ??_7Switch_GateMain@@6B@ ??_7SwitchCarnivalMain@@6B@ ??_7Switch_ElevatorPropellorRoomMain@@6B@ ??_7BasicWallSwitchMain@@6B@
- 0x001D0640 ??_7TargetSwitchMain@@6B@ chain: chain RegisterMe 0x000423CA; sites agree 1 conflict 0; slot-ev 3 of 28
- 0x001D2B78 ??_7KosmoBotMidriffMain@@6B@ chain: chain RegisterMe 0x0004DB9B; sites agree 1 conflict 0; slot-ev 3 of 28
- 0x001D2F90 ??_7AreaLoadRegionMain@@6B@ chain: chain RegisterMe 0x000F3522; sites agree 1 conflict 0; slot-ev 4 of 25
- 0x001D5550 ??_7FadeToBlack@@6B@ site: site 0x0005481D ??0FadeToBlack@@QAE@XZ (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 2 of 9
- 0x001D79C0 ??_7CannonCartMain@@6B@ chain: chain RegisterMe 0x0005E26D; sites agree 1 conflict 0; slot-ev 0 of 28; twins 14: ??_7WeavingBridgeMain@@6B@ ??_7FloatingPlatMain@@6B@ ??_7FloatingSinglePlatMain@@6B@ ??_7SwingingCrapMain@@6B@ ??_7SwingingBusMain@@6B@ ??_7SwingingFridgeMain@@6B@ ??_7SwingingOutHouseMain@@6B@ ??_7SwingingPickleJarMain@@6B@ ??_7SwingingTireMain@@6B@ ??_7FroggerPlatformsScript@@6B@ ??_7PlatformsAMain@@6B@ ??_7PlatformsBMain@@6B@ ??_7PlatformsCMain@@6B@ ??_7PlatformsDMain@@6B@
- 0x001D80F8 ??_7Popup@@6B@ site: site 0x0005EDDC ??0Popup@@QAE@PAVCritterMain@@@Z (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 2 of 25
- 0x001DAAE4 ??_7LightBeam@@6B@ site: site 0x0006B053 ??1LightBeam@@UAE@XZ (proto-pdb:default.pdb), 0x0006B0A9 ??0LightBeam@@QAE@XZ (proto-pdb:default.pdb); sites agree 2 conflict 0; slot-ev 1 of 9
- 0x001DB738 ??_7GateTrapMain@@6B@ chain: chain RegisterMe 0x00071158; sites agree 1 conflict 0; slot-ev 2 of 28
- 0x001DBB88 ??_7Beret@@6B@ chain: chain RegisterMe 0x000748A4; sites agree 1 conflict 0; slot-ev 1 of 18
- 0x001DC328 ??_7MuseumGateMain@@6B@ chain: chain RegisterMe 0x0007482C; sites agree 1 conflict 0; slot-ev 3 of 28
- 0x001DDFD0 ??_7VinceGiblets@@6B@ site: site 0x00078D86 ??0VinceGiblets@@QAE@ABV?$ResourceHandle@VModelResource@@@@@Z (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 1 of 13
- 0x001DF340 ??_7SafeHookRopeMain@@6B@ chain: chain RegisterMe 0x0007E9DA; sites agree 1 conflict 0; slot-ev 2 of 28
- 0x001DF468 ??_7SwitchSafeMain@@6B@ chain: chain RegisterMe 0x0007ED29; sites agree 1 conflict 0; slot-ev 4 of 28
- 0x001DFEF8 ??_7RocketMain@@6B@ chain: chain RegisterMe 0x00081F1D; sites agree 1 conflict 0; slot-ev 2 of 28
- 0x001E2478 ??_7TeslaCoilUpperMain@@6B@ chain: chain RegisterMe 0x0008C1F6; sites agree 1 conflict 0; slot-ev 2 of 29
- 0x001E79F8 ??_7WheelPlatformAMain@@6B@ chain: chain RegisterMe 0x000A0B3D; sites agree 1 conflict 0; slot-ev 4 of 28
- 0x001E7AB0 ??_7ReverseElevatorMain@@6B@ chain: chain RegisterMe 0x000A0B79; sites agree 1 conflict 0; slot-ev 3 of 28
- 0x001E7B68 ??_7ArmorDilloChuteMain@@6B@ chain: chain RegisterMe 0x000A0BB5; sites agree 1 conflict 0; slot-ev 4 of 28
- 0x001E7E90 ??_7DoorRockBlockMain@@6B@ chain: chain RegisterMe 0x000A0CE1; sites agree 1 conflict 0; slot-ev 2 of 28
- 0x001E8E20 ??_7Switch_PropellerMain@@6B@ chain: chain RegisterMe 0x000A46E8; sites agree 1 conflict 0; slot-ev 2 of 28
- 0x001E8E90 ??_7Switch_ShimmerMain@@6B@ site: site 0x000A4358 ??0Switch_ShimmerMain@@QAE@PAVSwampSceneNode@@D@Z (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 1 of 28; twins 4: ??_7Switch_ShimmerAMain@@6B@ ??_7Switch_ShimmerBMain@@6B@ ??_7Switch_ShimmerCMain@@6B@ ??_7Switch_ShimmerDMain@@6B@
- 0x001E8F00 ??_7Switch_ShimmerAMain@@6B@ chain: chain RegisterMe 0x000A4724; sites agree 1 conflict 0; slot-ev 0 of 28; twins 4: ??_7Switch_ShimmerMain@@6B@ ??_7Switch_ShimmerBMain@@6B@ ??_7Switch_ShimmerCMain@@6B@ ??_7Switch_ShimmerDMain@@6B@
- 0x001E8F70 ??_7Switch_ShimmerBMain@@6B@ chain: chain RegisterMe 0x000A4760; sites agree 1 conflict 0; slot-ev 0 of 28; twins 4: ??_7Switch_ShimmerMain@@6B@ ??_7Switch_ShimmerAMain@@6B@ ??_7Switch_ShimmerCMain@@6B@ ??_7Switch_ShimmerDMain@@6B@
- 0x001E8FE0 ??_7Switch_ShimmerCMain@@6B@ chain: chain RegisterMe 0x000A479C; sites agree 1 conflict 0; slot-ev 0 of 28; twins 4: ??_7Switch_ShimmerMain@@6B@ ??_7Switch_ShimmerAMain@@6B@ ??_7Switch_ShimmerBMain@@6B@ ??_7Switch_ShimmerDMain@@6B@
- 0x001E9050 ??_7Switch_ShimmerDMain@@6B@ chain: chain RegisterMe 0x000A47D8; sites agree 1 conflict 0; slot-ev 0 of 28; twins 4: ??_7Switch_ShimmerMain@@6B@ ??_7Switch_ShimmerAMain@@6B@ ??_7Switch_ShimmerBMain@@6B@ ??_7Switch_ShimmerCMain@@6B@
- 0x001EADA0 ??_7FogZone@@6B@ site: site 0x000AB4C7 ??1FogZone@@UAE@XZ (proto-pdb:default.pdb), 0x000D13A0 ??0FogZone@@QAE@PBVBeepTDF@@@Z (proto-pdb:default.pdb); sites agree 3 conflict 0; slot-ev 1 of 9
- 0x001EADC4 ??_7SearchlightBeam@@6B@ site: site 0x000AB4F9 ??0SearchlightBeam@@QAE@HABUQuaternion@@@Z (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 1 of 9
- 0x001EAF3C ??_7GatorNode@@6B@ site: site 0x000B051A ??1GatorNode@@UAE@XZ (proto-pdb:default.pdb), 0x000B0DD5 ??0GatorNode@@QAE@ABV?$ResourceHandle@VModelResource@@@@@Z (proto-pdb:default.pdb); sites agree 2 conflict 0; slot-ev 7 of 13
- 0x001EB468 ??_7ParticleRenderer@@6B@ site: site 0x000C62C7 ??0ParticleRenderer@@QAE@ABVFireFlyNode@@ABV?$vector@V?$ResourceHandle@VTextureResource@@@@V?$allocator@V?$ResourceHandle@VTextureResource@@@@@std@@@std@@PBUMatrix@@M@Z (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 1 of 1
- 0x001EBE7C ??_7LightNode@@6B@ site: site 0x000CD658 ??1LightNode@@UAE@XZ (proto-pdb:default.pdb), 0x000CDD95 ??0LightNode@@QAE@ABULightDefinition@@@Z (proto-pdb:default.pdb); sites agree 2 conflict 0; slot-ev 3 of 9
- 0x001EC03C ??_7HeatShimmer@@6B@ site: site 0x000D063F ??0HeatShimmer@@QAE@ABUVector3@@@Z (proto-pdb:default.pdb), 0x000D0769 ??_EHeatShimmer@@UAEPAXI@Z (proto-pdb-public+fold-alias+callee-bytes); sites agree 3 conflict 0; slot-ev 1 of 9
- 0x001EC0E4 ??_7SwampLightningFlash@@6B@ site: site 0x000D08E1 ??0SwampLightningFlash@@QAE@IMM@Z (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 2 of 9
- 0x001EC294 ??_7DebugGeometryNode@@6B@ site: site 0x000D39DE ??0DebugGeometryNode@@QAE@XZ (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 1 of 9
- 0x001EC5C8 ??_7GeometryNode@@6B@ site: site 0x000DBE96 ??0GeometryNode@@QAE@XZ (proto-pdb:default.pdb), 0x000DBED4 ??1GeometryNode@@UAE@XZ (proto-pdb:default.pdb); sites agree 2 conflict 0; slot-ev 1 of 9
- 0x001EC8E0 ??_7UINode@@6B@ site: site 0x000E1527 ??0UINode@@QAE@XZ (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 1 of 9
- 0x001EC934 ??_7HelpUI@@6B@ site: site 0x000E1E7D ??0HelpUI@@QAE@XZ (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 1 of 9
- 0x001EC958 ??_7UIHealthMain@@6B@ chain: chain RegisterMe 0x000E9A8A; sites agree 1 conflict 0; slot-ev 0 of 28; twins 2: ??_7UIPageMain@@6B@ ??_7UIHeartMain@@6B@
- 0x001EC9C8 ??_7UIBeadMain@@6B@ chain: chain RegisterMe 0x000E9AC6; sites agree 1 conflict 0; slot-ev 2 of 28
- 0x001ECA40 ??_7UIPageMain@@6B@ chain: chain RegisterMe 0x000E9C2E; sites agree 1 conflict 0; slot-ev 0 of 28; twins 2: ??_7UIHealthMain@@6B@ ??_7UIHeartMain@@6B@
- 0x001ECAB0 ??_7UIHeartMain@@6B@ chain: chain RegisterMe 0x000E9C6A; sites agree 1 conflict 0; slot-ev 0 of 28; twins 2: ??_7UIHealthMain@@6B@ ??_7UIPageMain@@6B@
- 0x001ECBC0 ??_7UIBonusBagMain@@6B@ chain: chain RegisterMe 0x000E9B02; sites agree 1 conflict 0; slot-ev 3 of 28
- 0x001ED988 ??_7CritterMain@@6B@ site: site 0x000EDBA5 ??0CritterMain@@QAE@PAVSwampSceneNode@@@Z (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 6 of 28
- 0x001EFA08 ??_7CutsceneDriver@@6B@ site: site 0x001089BD ??0CutsceneDriver@@QAE@PAVSwampSceneNode@@@Z (proto-pdb:default.pdb); sites agree 1 conflict 0; slot-ev 3 of 30
- 0x001F0148 ??_7BasicWallSwitchMain@@6B@ chain: chain RegisterMe 0x0010F909; sites agree 1 conflict 0; slot-ev 1 of 28; twins 10: ??_7SwitchSausageMain@@6B@ ??_7Switch_ElevatorEWWMain@@6B@ ??_7Switch_ShipDoorMain@@6B@ ??_7SwitchSwampMain@@6B@ ??_7Switch_DrawBridgeMain@@6B@ ??_7Switch_TrapMain@@6B@ ??_7Switch_GateMain@@6B@ ??_7SwitchCarnivalMain@@6B@ ??_7Switch_TrailerFrogsMain@@6B@ ??_7Switch_ElevatorPropellorRoomMain@@6B@
- 0x001F2308 ??_7MondoMoundMain@@6B@ chain: chain RegisterMe 0x00121331; sites agree 1 conflict 0; slot-ev 3 of 28
- 0x001F74B0 ??_7SwingHookTargetMain@@6B@ chain: chain RegisterMe 0x000F3036; sites agree 1 conflict 0; slot-ev 1 of 28
- 0x001FB2A0 ??_7VoodooShopVinceMain@@6B@ chain: chain RegisterMe 0x0013F44D; sites agree 1 conflict 0; slot-ev 1 of 28
- 0x001FCBE0 ??_7Decal@@6B@ site: site 0x0014FA4E ??0Decal@@QAE@ABV?$ResourceHandle@VTextureResource@@@@MM@Z (proto-pdb:default.pdb), 0x0014FBE1 ??1Decal@@UAE@XZ (proto-pdb:default.pdb); sites agree 2 conflict 0; slot-ev 2 of 9
- 0x001FCC1C ??_7FloatyBits@@6B@ site: site 0x001503DE ??1FloatyBits@@UAE@XZ (proto-pdb:default.pdb), 0x00150876 ??0FloatyBits@@QAE@I@Z (proto-pdb:default.pdb); sites agree 2 conflict 0; slot-ev 1 of 9
- 0x001FCC40 ??_7SwampWaterNode@@6B@ site: site 0x001507CE ??1SwampWaterNode@@UAE@XZ (proto-pdb:default.pdb), 0x0015132B ??0SwampWaterNode@@QAE@ABV?$ResourceHandle@VModelResource@@@@ABV?$ResourceHandle@VMaterialResource@@@@MI@Z (proto-pdb:default.pdb); sites agree 2 conflict 0; slot-ev 2 of 9
- 0x001FCCAC ??_7SwampPuddle@@6B@ site: site 0x00151030 ??0SwampPuddle@@QAE@ABUVector2@@ABV?$ResourceHandle@VMaterialResource@@@@ABUVector3@@MI@Z (proto-pdb:default.pdb), 0x0015121E ??1SwampPuddle@@UAE@XZ (proto-pdb:default.pdb); sites agree 2 conflict 0; slot-ev 2 of 9
