# D135 push-start sites (5.5 T2.c1)
Evidence for rules/D135.md (R5.5-003 S9): the 5.4 T11.1 abs-site non-entry push-start bodies and every retail
`push imm32` of each body VA. Addresses and symbol names only (D46: no bytes).

- Source: `.run/5.5/t2/census_bc.py` (scratch) over the retail XBE via `tools/name_sites.py` load() and
  amb_state(); body set = the `# 5.4 T11.1 name_sites --ambiguous abs-site` block of
  `git show ee4712477:config/retail/symbols_pin.txt` (74 lines, 63 bodies) whose every accepted-caller abs site of
  the row is a `push imm32` of the body VA and whose body is masked-equal to the row (name_sites amb_push_site /
  amb_push_start minus the Ghidra-entry test, which the post-ingest functions.jsonl no longer answers).
  Re-derived because `name_sites --ambiguous abs-site` now refuses all 94 bodies `already-named` and
  `.run/5.4/t3/` is gone. Sites: raw `68 imm32` scan of the retail exec sections, each decoded (iced) from its
  Ghidra container entry. Caller = containing Ghidra function; name from symbols.txt + symbols_pin.txt.
- Counts: bodies 61 (validation.md V5.4-NP1/NP5: 61); with a push site 61; push sites 65; distinct callers 64; sites decoded as `push imm32` 65; sites whose caller is a named masked-equal (accepted) caller 61; callers in matched_set.toml 28.
- The 2 other bodies of the block (0x000164B3, 0x0011C7BC) are entry pairs with non-push abs sites (site-corr).
- Line: `body VA  body name | site VA | caller VA  caller name | masked-equal-caller yes/no`

- 0x000110B4 `?creationFunc@BayouLevelMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00011FEA | caller 0x00011FD1 `??0RegisterMe@BayouLevelMain@@QAE@XZ` | masked-equal-caller yes
- 0x000110D8 `?creationFunc@SewersLevelMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00012026 | caller 0x0001200D `??0RegisterMe@SewersLevelMain@@QAE@XZ` | masked-equal-caller yes
- 0x000110FC `?creationFunc@CarnivalLevelMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00012062 | caller 0x00012049 `??0RegisterMe@CarnivalLevelMain@@QAE@XZ` | masked-equal-caller yes
- 0x00011120 `?creationFunc@ManorHouseLevelMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0001209E | caller 0x00012085 `??0RegisterMe@ManorHouseLevelMain@@QAE@XZ` | masked-equal-caller yes
- 0x0001C6F6 `?creationFunc@VinceMeetsJimmyCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0001C78A | caller 0x0001C771 `??0RegisterMe@VinceMeetsJimmyCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x000244EA `?creationFunc@JimmysFanboatRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00024527 | caller 0x0002450E `??0RegisterMe@JimmysFanboatRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x000276BB `?creationFunc@JimmyVictoryDanceCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000276FB | caller 0x000276E2 `??0RegisterMe@JimmyVictoryDanceCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x00028660 `?creationFunc@HurricaneHannahRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0002869D | caller 0x00028684 `??0RegisterMe@HurricaneHannahRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0002B92C `?creationFunc@HannaPreludeCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0002BF4A | caller 0x0002BF31 `??0RegisterMe@HannaPreludeCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x0003E859 `?creationFunc@MidwayRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0003E8B8 | caller 0x0003E89F `??0RegisterMe@MidwayRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x00042202 `?creationFunc@ScareHouseRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0004223F | caller 0x00042226 `??0RegisterMe@ScareHouseRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0004E888 `?creationFunc@BumpercarBumpoffRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0004E8C5 | caller 0x0004E8AC `??0RegisterMe@BumpercarBumpoffRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0005BA1D `?creationFunc@ZombieGuidanceCounselorRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0005BA5A | caller 0x0005BA41 `??0RegisterMe@ZombieGuidanceCounselorRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0005C1DD `?creationFunc@SarcophagusHustleRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0005C21A | caller 0x0005C201 `??0RegisterMe@SarcophagusHustleRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0006059E `?creationFunc@KosmoAppearsSarHusCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000605DE | caller 0x000605C5 `??0RegisterMe@KosmoAppearsSarHusCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x000625B0 `?creationFunc@GatekeeperJamRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000625ED | caller 0x000625D4 `??0RegisterMe@GatekeeperJamRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x00063BCD `?creationFunc@GatekeeperJamWelcomeCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00063C37 | caller 0x00063C1E `??0RegisterMe@GatekeeperJamWelcomeCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x00068130 `?creationFunc@RatRaceRodeoRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0006816D | caller 0x00068154 `??0RegisterMe@RatRaceRodeoRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x000696CD `?creationFunc@GwistlethorpeMausoleumRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0006970A | caller 0x000696F1 `??0RegisterMe@GwistlethorpeMausoleumRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x00069FB0 `?creationFunc@VinceSwipe@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00069FED | caller 0x00069FD4 `??0RegisterMe@VinceSwipe@@QAE@XZ` | masked-equal-caller yes
- 0x000745EC `?creationFunc@TheSquareRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00074629 | caller 0x00074610 `??0RegisterMe@TheSquareRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0007511E `?creationFunc@MuseumStartBattleRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000758E6 | caller 0x000758CD `??0RegisterMe@MuseumStartBattleRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0007F171 `?creationFunc@KosmoPigCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0007F1B1 | caller 0x0007F198 `??0RegisterMe@KosmoPigCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x0008055B `?creationFunc@CanGasMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00081452 | caller 0x00081439 `??0RegisterMe@CanGasMain@@QAE@XZ` | masked-equal-caller yes
- 0x00083D5C `?creationFunc@WallBlockingSquareMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00083E61 | caller 0x00083E48 `??0RegisterMe@WallBlockingSquareMain@@QAE@XZ` | masked-equal-caller yes
- 0x00084068 `?creationFunc@ClockHandLittleMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00085D59 | caller 0x00085D40 `??0RegisterMe@ClockHandLittleMain@@QAE@XZ` | masked-equal-caller yes
- 0x0008437E `??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ / ??1FigureTimeInfo@@QAE@XZ` | site 0x0003C8C7 | caller 0x0003C833 `(unnamed)` | masked-equal-caller no
- 0x0008437E `??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ / ??1FigureTimeInfo@@QAE@XZ` | site 0x0009487A | caller 0x00094879 `??1TrainSwitchMain@@UAE@XZ` | masked-equal-caller yes
- 0x0008437E `??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ / ??1FigureTimeInfo@@QAE@XZ` | site 0x0009E2F8 | caller 0x0009DFC9 `boundary_0009DFC9` | masked-equal-caller no
- 0x0008437E `??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ / ??1FigureTimeInfo@@QAE@XZ` | site 0x00155E42 | caller 0x00155E42 `(unnamed)` | masked-equal-caller no
- 0x0008437E `??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ / ??1FigureTimeInfo@@QAE@XZ` | site 0x00155ED6 | caller 0x00155ED6 `(unnamed)` | masked-equal-caller no
- 0x000843CB `?creationFunc@Sky_TheSquare_DayNightSkyMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00085F39 | caller 0x00085F20 `??0RegisterMe@Sky_TheSquare_DayNightSkyMain@@QAE@XZ` | masked-equal-caller yes
- 0x00086845 `?creationFunc@VinceThrowApple@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00086AC1 | caller 0x00086AA8 `??0RegisterMe@VinceThrowApple@@QAE@XZ` | masked-equal-caller yes
- 0x000874DE `?creationFunc@GatekeeperNoOpenCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00087548 | caller 0x0008752F `??0RegisterMe@GatekeeperNoOpenCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x0008C44A `?creationFunc@VinceZapHat@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0008C487 | caller 0x0008C46E `??0RegisterMe@VinceZapHat@@QAE@XZ` | masked-equal-caller yes
- 0x0008C6A8 `?creationFunc@ZapHatMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0008C724 | caller 0x0008C70B `??0RegisterMe@ZapHatMain@@QAE@XZ` | masked-equal-caller yes
- 0x0008E08F `?creationFunc@TumblerRoomRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0008E0CC | caller 0x0008E0B3 `??0RegisterMe@TumblerRoomRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0008EC74 `?creationFunc@KosmoAppearsTumblerCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0008ECB4 | caller 0x0008EC9B `??0RegisterMe@KosmoAppearsTumblerCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x0008F7D1 `?creationFunc@PassageEnterMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0008F8AD | caller 0x0008F894 `??0RegisterMe@PassageEnterMain@@QAE@XZ` | masked-equal-caller yes
- 0x0008F7F8 `?creationFunc@CaptainsRoomRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0008F835 | caller 0x0008F81C `??0RegisterMe@CaptainsRoomRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x000940DC `?creationFunc@ChicoryMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000941C9 | caller 0x000941B0 `??0RegisterMe@ChicoryMain@@QAE@XZ` | masked-equal-caller yes
- 0x00094150 `?creationFunc@FrontPorchRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0009418D | caller 0x00094174 `??0RegisterMe@FrontPorchRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0009C459 `?creationFunc@CrossBowMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0009D2A2 | caller 0x0009D289 `??0RegisterMe@CrossBowMain@@QAE@XZ` | masked-equal-caller yes
- 0x0009D47B `?creationFunc@GlowberryTangleRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0009D4B8 | caller 0x0009D49F `??0RegisterMe@GlowberryTangleRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x0009EC1A `?creationFunc@LiftMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0009ED7D | caller 0x0009ED64 `??0RegisterMe@LiftMain@@QAE@XZ` | masked-equal-caller yes
- 0x0009F677 `?creationFunc@EthelRocketLabCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0009F6B7 | caller 0x0009F69E `??0RegisterMe@EthelRocketLabCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x000A1461 `?creationFunc@KosmoAppearsMainTunnelCutscene@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000A14A1 | caller 0x000A1488 `??0RegisterMe@KosmoAppearsMainTunnelCutscene@@QAE@XZ` | masked-equal-caller yes
- 0x000A383A `?creationFunc@StealthRoomRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000A3877 | caller 0x000A385E `??0RegisterMe@StealthRoomRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x000A45D4 `?creationFunc@PropellerRoomRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000A4611 | caller 0x000A45F8 `??0RegisterMe@PropellerRoomRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x000A5786 `?creationFunc@WrathOfJaniceRegionMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000A57C3 | caller 0x000A57AA `??0RegisterMe@WrathOfJaniceRegionMain@@QAE@XZ` | masked-equal-caller yes
- 0x000AAC3E `?creationFunc@ThrottleUpObstacleMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000AACB7 | caller 0x000AAC9E `??0RegisterMe@ThrottleUpObstacleMain@@QAE@XZ` | masked-equal-caller yes
- 0x000F2BAB `?creationFunc@SwingHookTargetMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000F304F | caller 0x000F3036 `??0RegisterMe@SwingHookTargetMain@@QAE@XZ` | masked-equal-caller yes
- 0x000F2CD6 `?creationFunc@VinceHardLanding@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000F322F | caller 0x000F3216 `??0RegisterMe@VinceHardLanding@@QAE@XZ` | masked-equal-caller yes
- 0x000F2CFA `?creationFunc@VinceSoftLanding@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000F326B | caller 0x000F3252 `??0RegisterMe@VinceSoftLanding@@QAE@XZ` | masked-equal-caller yes
- 0x000F2D1E `?creationFunc@VinceSwitch@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000F32A7 | caller 0x000F328E `??0RegisterMe@VinceSwitch@@QAE@XZ` | masked-equal-caller yes
- 0x000F2D42 `?creationFunc@VinceFadeIn@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000F32E3 | caller 0x000F32CA `??0RegisterMe@VinceFadeIn@@QAE@XZ` | masked-equal-caller yes
- 0x000F2F04 `?creationFunc@DustClusterMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x000F362B | caller 0x000F3612 `??0RegisterMe@DustClusterMain@@QAE@XZ` | masked-equal-caller yes
- 0x00107F98 `?creationFunc@ActivatorMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0010859C | caller 0x00108583 `??0RegisterMe@ActivatorMain@@QAE@XZ` | masked-equal-caller yes
- 0x0010F486 `?creationFunc@SwitchMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x0010F8AA | caller 0x0010F891 `??0RegisterMe@SwitchMain@@QAE@XZ` | masked-equal-caller yes
- 0x00130FBE `?creationFunc@MysteryBoxMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00131084 | caller 0x0013106B `??0RegisterMe@MysteryBoxMain@@QAE@XZ` | masked-equal-caller yes
- 0x001315A2 `?creationFunc@CharmPageMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x001315E2 | caller 0x001315C9 `??0RegisterMe@CharmPageMain@@QAE@XZ` | masked-equal-caller yes
- 0x00134AE0 `?creationFunc@WhammyResultTwisterMain@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z` | site 0x00136187 | caller 0x0013616E `??0RegisterMe@WhammyResultTwisterMain@@QAE@XZ` | masked-equal-caller yes
- 0x0017CEFF `?DeferredCommandDpcRoutine@CMcpxAPU@DirectSound@@CGXPAU_KDPC@@PAX11@Z` | site 0x0017D2B1 | caller 0x0017D25A `?Initialize@CMcpxAPU@DirectSound@@QAEJXZ` | masked-equal-caller yes
- 0x0017D06E `?ApuInterruptDpcRoutine@CMcpxAPU@DirectSound@@CGXPAU_KDPC@@PAX11@Z` | site 0x0017D293 | caller 0x0017D25A `?Initialize@CMcpxAPU@DirectSound@@QAEJXZ` | masked-equal-caller yes
- 0x00182F74 `?AciInterruptDpcHandler@CAc97Device@DirectSound@@CGXPAU_KDPC@@PAX11@Z` | site 0x00183099 | caller 0x0018304A `?Initialize@CAc97Device@DirectSound@@QAEJK@Z` | masked-equal-caller yes
