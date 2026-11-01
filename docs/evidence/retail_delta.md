# Retail delta: near-lane matches (3.4)

One line per near-lane match (lane bar, PHASE_PLAN Context): what retail changed against the prototype body.
No bytes here (D46): symbol, retail VA, proto_map similarity, change class in words, cookbook id or `new`.
A change class seen a second time gets a cookbook entry in the task that sees it.

Form: `- <symbol> | <retail VA> | sim <n> | <change class> | <C<nnnn>|new> | <task>`

## Change classes (3.4 T13; counts = lines under Matches)
- debug new(file, __LINE__) line constant shifted | 4 | C0143
- member offset shift (retail class grew, body unchanged) | 10 | C0147
- total 14 = the near-lane matches (3.4 lane 14); every class with >= 2 matches has its cookbook entry

## Matches
- ?Open@BeepFile@@QAE_NPBXI@Z | 0x00037832 | sim 95.0 | new(file, __LINE__) line constant shifted (+15 lines, 908->923) | C0143 | T3
- ?OpenForWrite@BeepFile@@QAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z | 0x000378AD | sim 97.44 | new(file, __LINE__) line constant shifted (+15 lines, 927->942) | C0143 | T3
- ?OpenForAsync@BeepFile@@QAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z | 0x00037911 | sim 97.5 | new(file, __LINE__) line constant shifted (+15 lines, 968->983) | C0143 | T3
- ?Open@BeepFile@@QAE_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z | 0x0003772F | sim 97.96 | new(file, __LINE__) line constants shifted (+15 lines, 832->847, 843->858) | C0143 | T3
- ?DisplayMissingControllerMessageThisFrame@HUDUI@@QAEXXZ | 0x000E142A | sim 75.0 | member offset shift: HUDUI members from proto 0xEC up at +4 (frame id 0xEC->0xF0) | C0147 | T5
- ?EndLetterbox@HUDUI@@QAEXM@Z | 0x000E0D84 | sim 81.82 | member offset shift: HUDUI +4 (_letterboxFadeRate 0xF4->0xF8) | C0147 | T5
- ?EndExpandedLetterbox@HUDUI@@QAEXM@Z | 0x000E0DB4 | sim 81.82 | member offset shift: HUDUI +4 (_letterboxFadeRateEx 0xFC->0x100) | C0147 | T5
- ?BloodHaze@HUDUI@@QAEXM@Z | 0x000E0DE4 | sim 71.43 | member offset shift: HUDUI +4 (_bloodHazeAlpha/_bloodHazeFadeRate 0x104/0x108->0x108/0x10C) | C0147 | T5
- ?FindUINode@HUDUI@@QAEPAVUINode@@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z | 0x000E6837 | sim 93.1 | member offset shift: HUDUI +4 (_uiNodeMap 0x25C->0x260) | C0147 | T5
- ?SetTargetLocation@Vince@@QAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z | 0x00018B34 | sim 81.82 | member offset shift: Vince _ridingTrolley 0x307->0x317, _targetLocation 0x390->0x3A4 | C0147 | T5
- ?SetVehicleName@Vince@@QAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z | 0x00018B58 | sim 83.33 | member offset shift: Vince _vehicleName 0x518->0x548 | C0147 | T5
- ?SetDefaultIdleAnim@Critter@@QAEXV?$ResourceHandle@VAnimationResource@@@@@Z | 0x00019E1B | sim 88.89 | member offset shift: Critter _defaultIdleAnim 0x278->0x288 | C0147 | T5
- ?GetDefaultIdleAnim@Critter@@QBE?AV?$ResourceHandle@VAnimationResource@@@@XZ | 0x00019E39 | sim 85.71 | member offset shift: Critter _defaultIdleAnim 0x278->0x288 | C0147 | T5
- ?Pause@SoundStreamer@@QAEXXZ | 0x000D8BFB | sim 94.44 | member offset shift: SoundStreamer +0x78 from proto 0x64 (_pauseTime 0xB4->0x12C); XDK callee pinned | C0147 | T12
