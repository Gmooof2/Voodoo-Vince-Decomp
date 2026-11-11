"""Bucket the unmatched candidate pool by verdict and by what each bucket needs (D29; T14, phase 2.5).

Usage: PY tools/pool_buckets.py [--out P]   (default docs/matching/pool_buckets.md)
Inputs: derived/candidates/candidates.jsonl (PY tools/candidates.py --write), docs/matching/attempts.jsonl (ledger),
config/matched_set.toml, config/retail/units.toml (opt), the delink cut derived/delink/retail-<pin8>/ (C0093 extent).
Each unmatched pool row lands in the first bucket whose test it meets (BUCKETS order). Exit 0 written; 1 the bucket
counts do not sum to pool minus matched; 2 refused (missing input, unit absent from units.toml or the cut).
3.4 T13 sections, each with a sum line checked against its proto_map class count (functions.jsonl):
- `## Near lane`: candidates rows with lane near, same bucket logic and BUCKETS, per-lane ranks.
- `## Near-identical outside the lane`: near-identical rows not in the lane, by the reason candidates.py excludes them
  (recomputed with its helpers; cross-checked against the lane row count). lane + outside == class count.
- `## Ambiguous`: class ambiguous by k = targetVAs that are Ghidra entries (applied/functions.jsonl): short-inline-copy,
  multiple-targets, other.
- `## Absent`: docs/evidence/qa_absent.md rows by QA result (QA evidence only, I5: no names derived).
- `## Lane sizing` (5.1 T9): ambiguous rows by settling class (k1, D115, D113, caller-xref, none) and near no-name rows
  by caller-site / vtable-slot reach; 8 in-memory plants (wrong body, wrong proto per class) must be refused. 5.4 T9:
  candidates registerme (RegisterMe ctor row and body push the same class-name literal) and abs-site with
  registerme-settled pairs as chain callers; creationFunc chain sum.
- `## Near sizing (5.4 T7)`: near lane unmatched by bucket x size bin and unit; name-not-admitted size `-` rows by
  sizing route (ghidra-extent, cut-extent, none); near reloc targets by naming route; `??_G` rows by their ctor's place;
  unit-not-delink rows by unit (settled-edge); fold rows (C0154 verdict); pilot ranking (top 8). Each a checked sum.
Inputs added: derived/proto_map/retail-xbe/functions.jsonl, derived/dup_report/retail_folds.jsonl,
config/retail/symbols.txt + symbols_pin.txt, derived/ghidra/retail-<pin8>/applied/functions.jsonl, qa_absent.md.
A NOTES/NOTES_VA key no unmatched row uses fails (exit 1, `FAIL note key ...`): its row was matched, drop the key
(5.5 T6; was a WARN line). Refused (2) when candidates.fresh() reports candidates.jsonl stale against its inputs.
"""
import argparse
import bisect
import hashlib
import json
import re
import struct
import sys
import tomllib
from collections import Counter, defaultdict
from pathlib import Path
from types import SimpleNamespace

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "tools"))
import byte_compare  # noqa: E402
import candidates  # noqa: E402  near-lane exclusion helpers (3.4 T13)
import coff_info  # noqa: E402
import proto_map  # noqa: E402

CAND = REPO / "derived" / "candidates" / "candidates.jsonl"
import match_one  # noqa: E402  ledger files and order (4.4 T11)
LEDGER = match_one.LEDGER  # the frozen base; every reader takes match_one.ledger_paths() (4.4 T11)
MATCHED = REPO / "config" / "matched_set.toml"
UNITS = REPO / "config" / "retail" / "units.toml"
QA_ABSENT = REPO / "docs" / "evidence" / "qa_absent.md"
OUT = REPO / "docs" / "matching" / "pool_buckets.md"
STOP = 20  # START_HERE.md: stop at about 20 attempts without progress; an attempt = match_one.is_attempt (5.2 T7)

# Verdicts recorded only in task summaries (the ledger has no verdict field): symbol -> (bucket, source).
NOTES = {
    "?GetTSC@BeepState@@QAE_KXZ": ("wall-d45", "2.5 T4, C0084"),
    "??$RELEASE@UDirect3D@@@@YAXAAPAUDirect3D@@@Z": ("held-d78", "2.5 T12; 4.3 T3 ruled by the developer 2026-11-02: stays "
                                                       "held under D78, no `unknown01` enumerator, no D78 exception"),
    # 4.3 T4: match_one 100, gate relink refuses the emitted vtable (unnamed ??_G / folded default slots, C0203)
    **{sym: ("wall-vtable-relink", "4.3 T4 C0201 C0203") for sym in (
        "??0PuddleRegionMain@@QAE@PAVSwampSceneNode@@@Z",)},
    # 4.3 T5: match_one 100 (RegionTrigger ctor 63.57: also an /O1 AABox inline limit), vtable slots unnamed (C0203)
    **{sym: ("wall-vtable-relink", "4.3 T5 C0203") for sym in (
        "??0RegionTrigger@@QAE@XZ",)},  # 5.5 T8: ??0VinceCeilingFan dropped (matched)
    # 5.4 T12.1.c12: ??0LevelScript dropped (matched with its ??_G companion)
    # 4.3 T8: match_one 100, vtable slots unnamed (C0203)
    **{sym: ("wall-vtable-relink", "4.3 T8 C0203") for sym in (
        "??0UIClusterMain@@QAE@PAVSwampSceneNode@@@Z",  # 5.5 T6: ??0VinceFlat dropped (matched); 5.5 T8: ??0MainstreetRegionMain dropped (matched)
        "??0VinceZapHat@@QAE@PAVSwampSceneNode@@@Z")},
    # 4.4 T4: the 4.3 T8 C0215/C0217 relink-refused rows matched (T3 relink change, D117 ruling); keys dropped
    "?DumpMemoryLeaks@AllocationTracker@@QAE_NXZ": ("wall-dir32", "4.3 T8.c17 C0155: 99.62"),
    "?IsVisible@Frustum@@QBE_NABUAABox@@M@Z": ("wall-d45", "4.3 T8.c25 C0219: inline-asm rsqrtss radius, D116"),
    # 4.3 T9: retail inline-asm length/sqrt (D116, C0219); best draft guarded (logs/T9.cost.md)
    "?FrameTick@GingerDeadManMain@@EAEXXZ": ("wall-d45", "4.3 T9.c1 C0219: 84.36"),
    "?FrameTick@RockChunkMain@@UAEXXZ": ("wall-d45", "4.3 T9.c4 C0219: 71.17"),
    "?IsVinceInTheWay@LiftMain@@AAE_NXZ": ("wall-d45", "4.3 T9.c13 C0219: 65.65"),
    "?CalcBounds@GatorNode@@QBEXI@Z": ("wall-d45", "4.3 T9.c27 C0219: 79.80 (control)"),
    "??0LoadScreen@@QAE@XZ": ("wall-vtable-relink", "4.3 T9.c3 C0203: 97.64, vtable slots unnamed"),
    # 3.3 verdicts (R3.3-005)
    "?Render@Sprite@@QBEXXZ": ("wall-thinker", "3.3 T4, T21 R3.3-004: best 98.597565; R1 cvttss2si operand order, "
                               "R2 y-add order; untried lead: name _loc.u/_loc.v (FAKE)"),  # was past-stop (3.3 T4)
}
# Same, keyed by VA where one symbol names several folded rows (T14 row = line of logs/T14.rows.txt).
NOTES_VA = {
    0x0010F1FB: ("relink-refused", "4.3 T5: FloorSwitchMain::HitBy match_one 100; relink refuses fold callee "
                 "ResetPopupTimer@SwitchMain 0x0007EA16 in the replaced SwitchSafe range (C0154)"),
    # 4.4 T4: 0x000292A6, 0x000775B9, 0x000775E2, 0x000CA770, 0x000E88E1 dropped (matched 4.3 T16)
    # 5.1 T14.c2: throw rows 13-16 (0x000AF749..0x000AFFA7) dropped (relink lift T14.c1); T14: 0x0008B96D Load (T14.c3)
    # 5.2 T13.c1: 0x00011B38 dropped (matched; CameraManager/Critter/ParticleManager emit their erase alias bodies)
    # 5.4 T19.c1: fold-alias rows 0x0006A5D2 0x0003FE34 0x000C10B1 0x000C10C4 0x000C1AA9 0x000ECEA5 0x0005D056 0x000AD5F9
    # 0x000BDB11 0x000AB5CD 0x00111476 0x001114D9 dropped (precheck rc 0 after the T3 relink weak resolve)
    # 5.4 T19.c2: 0x000C1AA9 restored (pin_rows bracket, below)
    0x000C1AA9: ("relink-refused", "5.4 T19.c2: pin_rows 0x000113EB 0x00011473 route-bracket:outside-bracket when listed "
                 "(proto 0x0040072B COMDAT in 5 PDB modules incl. Init.obj anchors Init's bracket at a retail VA "
                 "outside Init's text)"),
    0x0001819A: ("relink-refused", "4.3 T16 C0250: ??4ResourceHandle<VertexShaderResource> matched row would overlap "
                 "SwampRenderer/ShootingCam COMDAT copies at the fold VA; guarded to keep SetVertexShader + "
                 "ClearStateCache"),
    # 3.4 verdicts (R3.4-005)
    0x0010027B: ("refused-by-tool", "3.4 T12: ??1Diary@@QAE@XZ pin-only; D118 lifted it (4.2 T5: matched_set add "
                 "--list <scratch copy> passes), was: matched_set refuses"),
    0x00125F98: ("refused-by-tool", "3.4 T6.1/T12: ?Set@GameOptions@@QBEXXZ sole name a disasm-verified pin; D118 "
                 "lifted it (4.2 T5: matched_set add --list <scratch copy> passes), was: matched_set refuses"),
    **{va: ("wall-dir32", "3.4 T12 C0155") for va in (0x00033D78, 0x00033C74)},
    0x00033C26: ("wall-dir32", "4.3 T5.c10 C0155: ??0AllocationTracker, 97.63"),
    **{va: ("wall-d45", "3.4 AABox __asm") for va in (0x000DDE5B, 0x000DDEA2)},
    # 4.4 T4: the 3.3 T4 wall-vtable-relink VAs (BundleWrapper ctor, FFly ctors) and 3.3 T14 pin-only-alias VAs matched
    **{va: ("xdk-callee-unnamed", "3.4") for va in (0x000C5C15, 0x000BD0E9)},
    # 4.3 T5: objdiff 100, masked 1: callee has only the undecorated Ghidra name (C0192)
    **{va: ("xdk-callee-unnamed", "4.3 T5 C0192") for va in (0x000354F0, 0x000DA1DE, 0x000C5E54)},
    # 5.2 T10: BufferMgr::ResetBuffer objdiff 100, masked 5: _IDirectSoundBuffer_* have only Ghidra names; lift = name
    # pins 0x0017A89C, 0x0017A880, 0x001799C8, 0x0017A1B5, 0x0017A1D1
    0x000B9277: ("xdk-callee-unnamed", "5.2 T10 C0323"),
    0x000BD3A8: ("folded-template-name", "3.4 T12: AddOverride"),
    # 4.4 T4: a pin (T4.c1) added this ??_G to the pool; match_one 100 + bytes equal (inline ~FrontEndUI, T4.c8), but
    # its pin has no size so matched_set refuses it (D118 needs a sized pin) and the cut lacks the EndMenu reloc (c0035)
    0x0013E262: ("refused-by-tool", "4.4 T4: ??_GFrontEndUI match_one 100; pin unsized, matched_set refuses; size the pin (0x29) at the next re-cut"),
    # 5.2 T9 guarded rows (match_one 100 unless noted; T9 summary, C0320, C0321)
    **{va: ("refused-by-tool", "5.2 T9: match_one 100; pin size `-`, matched_set refuses; size the pin at the next re-cut")
       for va in (0x0013240F, 0x0003735F)},
    **{va: ("refused-by-tool", "5.2 T9: ??_G fold-name: the row name is another unit's copy at the fold VA; match_one "
            "refuses `not defined`") for va in (0x000223AC, 0x001089F9)},
    **{va: ("wall-vtable-relink", "5.2 T9 C0320: ??_G needs its ctor; vtable slot unpinned or vtable pin site-corr "
            "(pin_rows)") for va in (0x0003DDD1, 0x0012A5A7, 0x0005F9A5, 0x0006EE68, 0x0008BFDD, 0x000EDC3B, 0x00080806)},
    **{va: ("relink-refused", "5.2 T9 C0233/C0273: fold-VA row, other units define or call another alias; or an "
            "unplaced COMDAT callee (FanBoat _Erase/??_G _Node)")
       for va in (0x0002DF5B, 0x0002DF9F, 0x0002E6E8, 0x00030F6F, 0x0003289B)},
    **{va: ("wall-d45", "5.2 T9 C0321: retail hand-written SSE __asm") for va in (0x000DCE88, 0x000DD1EC, 0x000DD24E,
                                                                              0x000DD46A)},
    # 5.2 T11 guarded rows (T11 summary, C0324, C0325)
    **{va: ("wall-d45", "5.2 T11 C0324: retail inline-asm rsqrtss sqrt (D116); InitCurve also 4.4 T13")
       for va in (0x0013392E, 0x000D701B)},
    0x00032622: ("relink-refused", "5.2 T11 C0325: _Tree::erase match_one 100 + bytes; out_of_range throw emits "
                 "logic_error -> basic_string copy ctor, relink overlap 0x00011753; ??_G _Node _Erase wall"),
    # 5.2 T12 M17 consultation R5.2-009; developer ruling 2026-11-07: D78, D116, D117 unchanged
    0x000D69DF: ("wall-thinker", "5.2 T12 R5.2-009: barred lever: volatile g_swamp access cast on the first read (D116), 100 with it; cl merges adjacent [g_swamp] loads in every allowed spelling (C0345, C0353)"),
    0x0006B425: ("wall-thinker", "5.2 T12 R5.2-009: barred lever: volatile g_swamp cast (D116) at lightbeam.cpp:337; adjacent g_swamp loads merged (C0345)"),
    0x001255DB: ("wall-thinker", "5.2 T12 R5.2-009: barred lever: volatile g_swamp cast (D116) at LoadScreens.cpp:176 GetCamera test"),
    0x000D0973: ("wall-thinker", "5.2 T12 R5.2-009: barred lever: volatile g_swamp cast (D116) at SwampLightning.cpp:171 (not run)"),
    0x000DC46C: ("wall-thinker", "5.2 T12 R5.2-009: barred lever: volatile g_swamp cast (D116), renderer declared first; store-to-reference rejected; constant cached in a register (C0227)"),
    0x000AD679: ("wall-thinker", "5.2 T12 R5.2-009: barred lever: dead __m128 _mm_setzero_ps() forcing and esp,-16 (D78); no 16-aligned local in PDB/disasm (C0211); /arch, /G7, /Zp16, /Op tried"),
    0x00133F82: ("wall-thinker", "5.2 T12 R5.2-009: no 100 even with the barred free Vector3 operators (D78): cl keeps them out of line at /Ob2 (T11.c2); __forceinline untried (D117)"),
    0x000DE0F6: ("wall-thinker", "5.2 T12 R5.2-009: no 100 even with barred __forceinline (D117): 91.32 first diff +0x8D (T12.c9b); allowed hand form 89.74"),
    0x000C5257: ("wall-thinker", "5.2 T12 R5.2-009: bit/child spill slots swapped; spill order follows allocator priority; C0227 top-of-function form tried"),
    0x000C54D0: ("wall-thinker", "5.2 T12 R5.2-009: six slot moves, world in edi, esi holding const 1, 4-byte frame diff"),
    0x000C9AE6: ("wall-thinker", "5.2 T12 R5.2-009: named local cannot pass as the call's eax; unnamed SwampString temps pool function-wide (C0329)"),
    0x000CA08B: ("wall-thinker", "5.2 T12 R5.2-009: cl calls the heavier operand it->second first; every order fix loses ebx to the cached 0"),
    0x000DF873: ("wall-thinker", "5.2 T12 R5.2-009: slots follow address-taken order (c6b) plus x87 term +0x325"),
    0x000DF5B7: ("wall-thinker", "5.2 T12 R5.2-009: pop esi placement tied to the compare form (C0249); HitTag 0x000DF7BC waits on it"),
    0x00102647: ("wall-thinker", "5.2 T12 R5.2-009: slot order after PDB order and hoists"),
    0x0010A0EA: ("wall-thinker", "5.2 T12 R5.2-009: schedule and spilled z product; x87 tie-breaks (C0141)"),
    0x0006F61F: ("wall-thinker", "5.2 T12 R5.2-009: y=0 store across a call, lea/push schedule; 13+ forms"),
    0x0006F7B1: ("wall-thinker", "5.2 T12 R5.2-009: lea/push hoist; ~300 probes"),
    0x0006D657: ("wall-thinker", "5.2 T12 R5.2-009: zero register edi function-wide (C0227)"),
    0x000DD4A5: ("wall-thinker", "5.2 T12 R5.2-009: x87 tie-breaks: row 0 unmoved by order or scope (C0141)"),
    0x000DDD76: ("wall-thinker", "5.2 T12 R5.2-009: store order; ~350 probes; x87 tie-breaks"),
    0x000DE1FC: ("wall-thinker", "5.2 T12 R5.2-009: frame from inlined by-value Apply temps; C0335 per-call blocks tried"),
    0x000DE552: ("wall-thinker", "5.2 T12 R5.2-009: x87 tie-breaks; c24 float const& operator[] probes tried (lever A not new)"),
    0x0004AE71: ("wall-thinker", "5.2 T12 R5.2-009: GetWorldRotation temp packing; 6 forms"),
    0x00088672: ("wall-thinker", "5.2 T12 R5.2-009: x87 velocity component; 98.67 not byte-equal"),
    0x00040092: ("wall-thinker", "5.2 T12 R5.2-009: which local reuses the dead param slot +8"),
    0x00081BCB: ("wall-thinker", "5.2 T12 R5.2-009: fld; fst; fadd vs our integer copy (scheduling)"),
    0x000B032F: ("wall-thinker", "5.2 T12 R5.2-009: mov/test needs the handle reused before the next call; the only reusing form spills"),
    0x0010F4FB: ("wall-thinker", "5.2 T12 R5.2-009: vector<Critter*>::_Tidy cleanup: retail keeps the first merged copy, ours the last; scope-exit form breaks the bl merge"),
    0x0010F5BB: ("wall-thinker", "5.2 T12 R5.2-009: same as 0x0010F4FB (block layout / tail merge)"),
    0x0013DD41: ("wall-thinker", "5.2 T12 R5.2-009: coalescing vs cached 0; Dinkumware *(begin()+pos) forms tried"),
    0x000573CD: ("wall-thinker", "5.2 T12 R5.2-009: three-instruction schedule in the return copy"),
    0x001138D0: ("wall-thinker", "5.2 T12 R5.2-009: block chaining; ~20 layouts"),
    0x000BD9A2: ("wall-thinker", "5.2 T12 R5.2-009: SIB base/index order; 45 probes, 14 flags (C0354)"),
    0x00137D6F: ("wall-thinker", "5.2 T12 R5.2-009: slot order within sharing groups"),
    0x000B8EDE: ("wall-thinker", "5.2 T12 R5.2-009: eax/ecx swapped function-wide; ~22 drafts"),
    0x000BA510: ("wall-thinker", "5.2 T12 R5.2-009: false paths skip the handle dtor test; 17 forms plus flags"),
    0x000D563B: ("wall-thinker", "5.2 T12 R5.2-009: unit-coupled: lever C fits but needs the guarded vector<ResourceBundle> copy ctor/dtor (4.3 T16 naming wall)"),
    0x000B8CC3: ("wall-thinker", "5.2 T12 R5.2-009: SetCloneEffect 99.88; lever B dropped: VC 7.1 <map> operator[] "
                 "uses lower_bound + insert(_Where, value_type) (include/map:141-148), not insert(value_type).first; "
                 "insert _Pairib temp packing residual"),
    0x0004515E: ("unit-coupled", "5.2 T12: _Incsize byte-equal only built without EH; unit keeps /EHa: retail "
                 "EvilVince range 0x00044948-0x00046AFD has EH frames (call __EH_prolog 0x00141600 at 0x00044A13, "
                 "0x00044A8E, 0x00044B09; fs:[0] restores from 0x00044A52, 5 sites)"),
    # 5.2 T12 lever round after R5.2-009 (T12.c26-c29): lever tried, residual unmoved
    0x000B1F90: ("wall-thinker", "5.2 T12 R5.2-009: lever A neutral (T12.c26, C0359): z product still fmul [lineDir.z] vs retail fld; fmul st(1); fstp st(0) at +0xA3; ~10 probe forms identical"),
    0x0010B763: ("wall-thinker", "5.2 T12 R5.2-009: lever A neutral (T12.c26): z product folds fmul [dir.z] at +0xFC; ref to _orbitRadius worse (frame 0x34)"),
    0x000DDC1E: ("wall-thinker", "5.2 T12 R5.2-009: lever A variant (zb.z/zb.y aliases, C0359) 97.14 -> 98.09; row[1]/row[2] store schedule tie at +0xDA"),
    0x00036BA9: ("wall-thinker", "5.2 T12 R5.2-009: lever C neutral (T12.c27): push_back/_Tidy visible, same bytes; +0x6 and [ebp-0x44],0 vs retail push ebx; xor ebx,ebx"),
    0x00034FE2: ("wall-thinker", "5.2 T12 R5.2-009: lever C neutral (T12.c27): push_back/insert/_Insert_n/_Ufill visible, same bytes; frame 0x120 vs 0x110 (map insert temps)"),
    0x000CD9FE: ("wall-thinker", "5.2 T12 R5.2-009: lever C (T12.c27): visible _Dec 89.57; ours keeps ls in esi, retail reloads _Myhead; ConsiderLight body D127-barred"),
    0x0002E831: ("wall-thinker", "5.2 T12 R5.2-009: lever D (T12.c29): element stand-in moved base to .pos.y, 95.62; texcoord.u fst stores hoisted above the 2nd pos copy (+0x511), 6 forms"),
    0x0007FDA7: ("wall-thinker", "5.2 T12 R5.2-009: C0244 order (T12.c29) 93.74; newloc += v3 y/z fadd [mem] vs retail fld; fadd st(1) (+0x19), 5 forms"),
    0x00129063: ("wall-thinker", "5.2 T12 R5.2-009: lever E (T12.c28) 20 runs, best allowed 97.625 +0x35: retail keeps 3 products on x87 stack, elapsed in memory; every hand form moves elapsed to a register; 100 only with D78-barred free operators (C0361)"),
    0x000D1A27: ("wall-thinker", "5.2 T12 R5.2-009: probe t1 re-run (T12.c28): objdiff omits score (0.0), build real 129 B vs 125, +0x7 no push edi, zero stores and [m],0; best 96.49"),
    # 5.2 T12 guarded rows (T12 summary; .run verdicts from coders c1-c25, C0327-C0357)
    0x000D9E55: ("callee-unnamed", "5.2 T12: masked _GetTickCount@0 0x0014A4D9 ghidra-only name (C0192); code identical (T12.c3)"),
    0x0013851E: ("wall-dir32", "5.2 T12: false DIR32 DAT_80000000 on push 0x80000000 at .text+0x4B (C0329) (T12.c3)"),
    0x000DF7BC: ("unit-coupled", "5.2 T12: passes only with PointInTriangle 0x000DF5B7 compiled (C0249) (T12.c6)"),
    0x000BD419: ("callee-unnamed", "5.2 T12: _XPhysicalFree@4 0x0001785A name (C0192) + dead cmp usesExVerts no PDB inline (T12.c6)"),
    0x000DF0EC: ("unit-coupled", "5.2 T12: needs _Insert_n body 0x000DEC4E emitted unmatched (D127) (T12.c6)"),
    0x001004A6: ("callee-unnamed", "5.2 T12: __chkstk 0x00141C10 unnamed (T12.c1)"),
    0x001008B5: ("callee-unnamed", "5.2 T12: __ehhandler$ funclet 0x0015323A unnamed (T12.c1, C0331)"),
    0x0010094A: ("callee-unnamed", "5.2 T12: __ehhandler$ 0x00153244 (T12.c1)"),
    0x001009D2: ("callee-unnamed", "5.2 T12: __ehhandler$ 0x0015324E (T12.c1)"),
    0x00100AE2: ("callee-unnamed", "5.2 T12: __ehhandler$ 0x00153262 (T12.c1)"),
    0x0010239A: ("callee-unnamed", "5.2 T12: __ehhandler$ 0x0015326C (T12.c1)"),
    0x00103141: ("callee-unnamed", "5.2 T12: __ehhandler$ 0x0015328A + _Ucopy 0x00100BDC (T12.c1)"),
    0x00103663: ("callee-unnamed", "5.2 T12: __ehhandler$ 0x00153294 (T12.c1)"),
    0x001036CD: ("callee-unnamed", "5.2 T12: __ehhandler$ 0x0015329E (T12.c1)"),
    0x00103FC8: ("callee-unnamed", "5.2 T12: __ehhandler$ 0x001532B2 (T12.c1)"),
    0x00101FF5: ("wall-data-name", "5.2 T12: data words 0x0021EEF0/F4 in no symbol file (T12.c1)"),
    0x001044A9: ("relink-refused", "5.2 T12: relink overlap: second copy of HUDUI _Buy<AreaStats> 0x000E84B1 (T12.c1)"),
    0x001045A6: ("relink-refused", "5.2 T12: relink overlap _Buy (T12.c1)"),
    0x001048B0: ("callee-unnamed", "5.2 T12: pair<string,string> dtor fold 0x00018696 needs alias pin (T12.c1)"),
    0x00106631: ("relink-refused", "5.2 T12: pair dtor fold 0x00018696 masked + regalloc spill (T12.c1)"),
    0x00105D47: ("callee-unnamed", "5.2 T12: vector<Slot>::_Buy 0x001030F8 unnamed (T12.c1)"),
    0x001076D9: ("wall-data-name", "5.2 T12: data words 0x0021EEF8/FC unnamed (T12.c1)"),
    0x000CB7AC: ("wall-d45", "5.2 T12: rsqrtss inline asm (T12.c11)"),
    0x0004D2FE: ("wall-d45", "5.2 T12: rsqrtss inline asm x3 (T12.c10)"),
    0x000DD29C: ("wall-d45", "5.2 T12: SSE inline asm (T12.c9)"),
    0x000DD37A: ("wall-d45", "5.2 T12: frameless SSE asm (T12.c9)"),
    0x000DD86A: ("wall-d45", "5.2 T12: fsincos asm (T12.c9)"),
    0x000F3905: ("wall-d45", "5.4 T14: CheckAttackDir rsqrtss inline asm via ebp slots, no and esp (C0324, D116); best 84.41 guarded (T14.c5)"),
    # 5.5 T8: 0x000EDBA5 dropped (matched)
    0x0004194C: ("wall-dir32", "5.2 T12: local label in push imm32, bytes match objdiff 0.0 (T12.c12); 5.5 T8.c4: "
                 "unchanged (objdiff 0.0, bytes match)"),
    0x00103D55: ("callee-unnamed", "5.2 T12: own __ehhandler$ funclet 0x001532A8 unnamed; code exact with C0325 form (T12.c16)"),
    0x00033F22: ("wall-dir32", "5.2 T12: false DIR32 on displacements 0x10014/0x10018 (C0329) (T12.c16)"),
    0x00038B65: ("unit-coupled", "5.2 T12: static unzlocal_GetCurrentFileInfoInternal 0x000388BB must match first (custom eax convention, same-unit only; D127) (T12.c16)"),
    0x00038B8C: ("unit-coupled", "5.2 T12: same as 0x00038B65 (T12.c16)"),
    0x00038BCB: ("unit-coupled", "5.2 T12: same as 0x00038B65 (T12.c16)"),
    0x00115A82: ("wall-dir32", "5.2 T12: false DIR32 on 0x10000/0x50000 tag immediates (C0155); scratch form 99.92 bytes match (T12.c19)"),
    0x0005F43D: ("wall-d78", "5.2 T12: no CritterManager getter in PDB; ~35 placements: cl hoists pManager load above Quaternion::Set, retail after (T12.c20)"),
    0x00018B88: ("callee-unnamed", "5.2 T12: masked ??1pair<string,string> at fold 0x00018696, no pinned alias (T12.c15)"),
    0x000CA39B: ("callee-unnamed", "5.2 T12: _D3DDevice_Clear@24 0x0015C280: symbols.txt has only undecorated name (T12.c15)"),
    0x000B940F: ("callee-unnamed", "5.2 T12: _IDirectSoundBuffer_StopEx@16 0x00179A58 ghidra-only (T12.c15)"),
    0x000B9434: ("callee-unnamed", "5.2 T12: _IDirectSoundBuffer_SetBufferData@12 0x0017A89C ghidra-only (T12.c15)"),
    0x00018696: ("relink-refused", "5.2 T12: matched_set ADD REFUSED pin has no size; relink both-define ??1Record@BeepTDF (T12.c15)"),
    0x000196B3: ("relink-refused", "5.2 T12: external _Tree<CreationFunc>::_Tidy does not resolve (C0126) (T12.c15)"),
    0x0008BFB5: ("relink-refused", "5.3 T2.c5: relink refusal (reloc field past its buffer, TrophyRoom site 0x0009D068; C0369) was ctor without its pin_emit scaffold: our ??_G calls ??1 out of line, T7 placed ??1 from that site at 0x0009D06B (retail inlines ??1); ctor + scaffold: gate_check rc 0. Still refused: pin_rows site-corr ??0BasicDoorMain, ??_7TeslaCoilPartMain (C0264)"),
    # was: "5.3 T2.c3: relink REFUSED reloc field past its buffer (TrophyRoom piece 0x0009C34F, site 0x0009D068, 3356 B; C0369) + pin_rows site-corr ??0BasicDoorMain, ??_7TeslaCoilPartMain (C0264); was 5.2 T12.c15 struct.error"
    0x000C6110: ("relink-refused", "5.2 T12: external _Tree<SoundNode>::_Lbound does not resolve by name (T12.c15)"),
    0x000DAA38: ("relink-refused", "5.2 T12: external ??1BundleBackgroundLoader does not resolve by name (needs dtor pin) (T12.c15)"),
    0x000BE048: ("relink-refused", "5.2 T12: gate_check placement FAILs at fold callers (C0233) (T12.c15)"),
    0x0012A563: ("relink-refused", "5.2 T12: external WaterRegionMain::LeavingRegion unpinned vtable slot (C0320) (T12.c15)"),
    0x000E03D8: ("unit-coupled", "5.2 T12: unguarding breaks _main: push 0 -> $T521 vs retail $T318 pin (T12.c15)"),
    0x000E04F0: ("unit-coupled", "5.2 T12: same as 0x000E03D8 (T12.c15)"),
    0x001380DE: ("wall-data-name", "5.2 T12: literal pin __real@3f8ccccd (1.1f) -> 0x001F9EC0 for callee IsInRange 0x00138088; Update byte-equal with both guards flipped (free probe) (T12.c21)"),
    0x000CA9BA: ("wall-dir32", "5.2 T12: false DIR32 DAT_80000000 at .text+0x6B9 on immediate 0x80000000 (C0329) (T12.c25)"),
    # 5.3 T6 guarded identical rows (T6 summary, C0377, C0378); guarded ambiguous rows: validation.md "5.3 Batches"
    # 5.4 T6: 0x0005EDDC, 0x0002BFA9 (??_G fold pins), 0x0014FBE1 (slot access), 0x000E274C, 0x000E2A07 (T3 fold-alias
    #   resolve) matched; dropped here
    0x001089BD: ("relink-refused", "5.4 T6.c4: ctor match_one 100 (??_GCutsceneDriver pinned 0x001089F9); relink "
                 "`?SetState@CutsceneDriver@@UAEXH@Z does not resolve by name`: vtable slot +0x74 -> 0x000946F7 empty "
                 "fold, PDB public MAE (protected); lift: SetState protected + pin the name at 0x000946F7"),
    # 5.3 T9: UIBeadMain (was attempted-open 98.5 at T7) walled by T9.c1
    0x000E27C0: ("callee-unnamed", "5.3 T9.c1: ctor match_one 100 + bytes only with GetBeadDest 0x000E1053 defined in "
                 "the TU (cl emits `add esp,4` vs `pop ecx` for a declared callee; 719 orders probed); GetBeadDest "
                 "guarded, its 0.125f at .data 0x0021EE68 unnamed; 5.4 T6: pin refused (proto operand "
                 "SETTINGS_FILE+4, no public), GetBeadDest bytes differ only at that fmul operand (T6.c4)"),
    # 5.4 T12.1.c6: ??_G rows <= 128 B guarded by companion ctor (C0320, C0377); ambiguous-lane ??_G rows are not
    # bucketed here (near sizing (d) lists them by ctor place; since T12.1.c7 (d) reads their NOTES_VA keys)
    0x0002D8E6: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0009EA70 ??0LiftMain in RocketLab.obj (row unit "
                 "BogWallow.obj: fold-name) fails reloc provisional; needs ??_7LiftMain 0x001E74D8 and "
                 "??0BasicDoorMain 0x00107C33 promoted from site-corr"),
    # 5.4 T12.1.c7: T12.1.c4/c5 walls (0x0004271B near + ambiguous ??_G; the rest ambiguous, listed in (d) "Other")
    0x0008B511: ("cut-nosym", "5.4 T12.1.c5: fold-name wall (C0377): the unit's own folded class is SwitchMuseumMain "
                 "(pinned at 0x0008B511); a draft under that name gave bytes masked 0, objdiff None; needs the cut "
                 "and row named ??1SwitchMuseumMain, then a re-cut"),
    0x001228EC: ("cut-extent", "5.4 T12.1.c5: measured extent 2588 vs row 18 (LAB_001228F9 at +0xD); needs a pin at "
                 "0x001228F9, then a re-cut; not drafted"),
    0x0003B64F: ("cut-wall-attempted", "5.4 T12.1.c5: extent 72 vs row 15 (LAB_0003B65E at +0xF); needs a pin at "
                 "0x0003B65E, then a re-cut; prior: 1 attempt, best 20"),
    0x00121879: ("pin-only-alias", "5.4 T12.1.c2: row named ??1WhammyResultTwisterMain, not defined in "
                 "VinceFlat.obj; draft as implicit ??1VinceFlat gives bytes match, objdiff None; needs the cut "
                 "named ??1VinceFlat@@UAE@XZ, then a re-cut; prior: 4 attempts"),
    0x00138EA1: ("pin-only-alias", "5.4 T12.1.c2: row name not defined in VoodooShop.obj; ??0CaptionLine via "
                 "pin_emit_CaptionLine_copy gives bytes match, objdiff None; needs a re-cut under "
                 "??0CaptionLine@FrontEndMenu and an [[emitter]] row; prior: 4 attempts"),
    0x00029271: ("pin-only-alias", "5.4 T12.1.c2: row name not defined in Hannah.obj; the pin has no size "
                 "(symbols_pin.txt:3165); needs a sized pin under ?EnteringRegion@HannahStartBattleRegionMain, "
                 "then a re-cut; prior: 2 attempts"),
    # 5.4 T12.1.c12: 0x000194FB ??1LevelStats dropped (matched; LevelLoader decl-only vector<AreaStats>::_Tidy)
    0x000D8BD2: ("xdk-callee-unnamed", "5.4 T12.1.c12: SetPosition@IDirectSoundStream 100, bytes provisional "
                 "(masked 1); relink: _IDirectSoundStream_SetPosition@20 at 0x0017A28A has only a ghidra name; "
                 "needs a sized pin func 0x29 (symbols_pin.txt:72 Pause@8 form), then matched_set's near-corr "
                 "add refusal re-checked; prior: 2 attempts"),
    0x000F4F4E: ("unit-coupled", "5.4 T12.1.c13: ??_GVinceGiblets is VinceGiblets' name (ctor 0x00078D86, other "
                 "unit); Monster.obj's own ctor ??0MonsterGiblets 0x000F4E73 fails reloc 0x1EE33C, 0x1EE34C; "
                 "needs those pinned, the ctor matched, the row re-cut as ??_GMonsterGiblets; not drafted"),
    0x0003F9AB: ("pin-only-alias", "5.4 T12.1.c4: fold-name wall: the VA is pinned as "
                 "?AnimationFinished@TargetSwitchMain (symbols_pin.txt:4199, proto ScareHouse.obj); a draft under "
                 "that name gives bytes match, objdiff None; needs the row renamed (pin, then a re-cut); prior: 2 "
                 "attempts"),
    0x00042737: ("cut-wall-attempted", "5.4 T12.1.c4: cut extent: objdiff 1.30, bytes match; needs a pin at the "
                 "neighbour FUN_00042BFD, then a re-cut (C0377); prior: 3 attempts"),
    # 5.4 T27: wave rows that failed (logs/T27.c7, c11, c13)
    # 5.5 T8: 0x00068334 dropped (matched)
    0x00136828: ("wall-d45", "5.4 T27.c7: TrainingSignMain::FrameTick retail inline-asm rsqrtss xz length (D116, "
                 "C0219, C0324, C0410); best 69.88 guarded, 1 attempt"),
    # 5.5 T8: 0x000C5F3D dropped (matched)
    0x0004271B: ("wall-vtable-relink", "5.4 T12.1.c4: companion ctor 0x0004261B fails reloc 0x000419F4 0x001D0B28 "
                 "0x002363F8; needs them named; no draft"),
    # 5.4 T12.1.c7: ambiguous-lane ??_G rows of the c6 screen (+ 3 T12.1.c3 drafts), shown in near sizing (d);
    # bucket: own ??_7 a blocker -> wall-vtable-relink, else unnamed data reloc-data, unnamed code callee-unnamed,
    # provisional only reloc-provisional; no ctor row unit-coupled; fold-name with the ctor done/passing refused-by-tool
    0x00019905: ("reloc-data", "5.4 T12.1: companion ctor 0x000196CA ??0OnionMain fails reloc; needs 0x00018110 "
                 "(unnamed code), 0x001C8228 (unnamed data)"),
    0x0002A085: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00029E9F ??0HannahMain fails reloc; needs "
                 "0x0002DF55 (unnamed code), ??_7HannahMain@@6B@ 0x001CC968 (site-corr)"),
    0x0002D967: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0002D741 ??0CrawdadTrapMain fails reloc; needs "
                 "??0BasicDoorMain 0x00107C33 (site-corr), ??_7CrawdadTrapMain@@6B@ 0x001CD418 (site-corr)"),
    0x000307D7: ("callee-unnamed", "5.4 T12.1: companion ctor 0x000306E1 ??0FanBoatWake fails reloc; needs "
                 "0x0002DE9D (unnamed code)"),
    0x00032F2E: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00032A6A ??0FanBoatMain fails reloc; needs 11 "
                 "unnamed targets 0x0002E3C5..0x001CD98C, ??_7FanBoatMain@@6B@ 0x001CD998 (site-corr)"),
    0x00037459: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00036F03 ??0FileHandler fails reloc; needs "
                 "??_7FileHandler@@6B@ 0x001CDC3C (site-corr)"),
    0x000374B7: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000372DD ??0MemoryFileHandler fails reloc; needs "
                 "??_7MemoryFileHandler@@6B@ 0x001CDD2C (site-corr)"),
    0x000374D3: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00036F95 ??0StandardFileHandler fails reloc; "
                 "needs ??_7StandardFileHandler@@6B@ 0x001CDC84 (site-corr)"),
    0x000374EF: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00037090 ??0ZipFileHandler fails reloc; needs "
                 "??_7ZipFileHandler@@6B@ 0x001CDCBC (site-corr)"),
    0x0003750B: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00037187 ??0AsyncFileHandler fails reloc; needs "
                 "??_7AsyncFileHandler@@6B@ 0x001CDCF4 (site-corr)"),
    0x0003B78F: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0003B727 ??0WheelOfMysteryMain fails reloc; "
                 "needs ??_7WheelOfMysteryMain@@6B@ 0x001CE960 (site-corr)"),
    0x0003DA72: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0003BF6D ??0switch_wheelOfMysteryMain in "
                 "WheelOfMystery.obj (row unit TrailerFrogs.obj: fold-name) fails reloc; needs "
                 "??_7switch_wheelOfMysteryMain@@6B@ 0x001CEB30 (site-corr)"),
    0x0003DEB9: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0011740E ??0PitFrogMain in PitFrog.obj (row unit "
                 "TrailerFrogs.obj: fold-name) fails reloc; needs 0x0002DF55 (unnamed code), ??_7PitFrogMain@@6B@ "
                 "0x001F0E38 (site-corr)"),
    0x000406B0: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000404C0 ??0CarRideMain fails reloc; needs "
                 "??_7CarRideMain@@6B@ 0x001D0270 (site-corr)"),
    0x0004844D: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0004708D ??0BumperCarMain fails reloc; needs 10 "
                 "unnamed targets 0x001D1CCC..0x001D1D9C, ??_7BumperCarMain@@6B@ 0x001D1DB0 (site-corr)"),
    0x0004C9B8: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0004C846 ??0KosmoBotHeadMain fails reloc; needs "
                 "??_7KosmoBotHeadMain@@6B@ 0x001D29F8 (site-corr)"),
    0x0005011F: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00123308 ??0potion_boomerangMain in "
                 "Boomerang.obj (row unit LandORides.obj: fold-name) fails reloc; needs "
                 "??_7potion_boomerangMain@@6B@ 0x001F2E00 (site-corr)"),
    0x000577EB: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0005775D ??0ShootingUI fails reloc; needs "
                 "??_7ShootingUI@@6B@ 0x001D5FF8 (site-corr)"),
    0x000578F3: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00057819 ??0OverheatUI fails reloc; needs "
                 "??_7OverheatUI@@6B@ 0x001D6040 (site-corr)"),
    0x00059192: ("unit-coupled", "5.4 T12.1: no ??0ZombieMain row in candidates (ctor inlined or absent); needs the "
                 "ZombieGuidanceCounselor.obj code that emits the vtable matched first (C0320)"),
    0x0005C6DD: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0005CB52 ??0VinceSpring fails reloc; needs "
                 "??_7VinceSpring@@6B@ 0x001D7690 (site-corr)"),
    0x0005F12D: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0005F06C ??0MondoMolePuzzleRegionMain fails "
                 "reloc; needs ??_7MondoMolePuzzleRegionMain@@6B@ 0x001D8198 (site-corr)"),
    0x000649D0: ("reloc-data", "5.4 T12.1: companion ctor 0x00064515 ??0RodeoRatMain fails reloc; needs 0x001D9880 "
                 "(unnamed data)"),
    0x0006606F: ("reloc-data", "5.4 T12.1: companion ctor 0x00065C84 ??0VinceOnRatMain fails reloc; needs 0x001D9B28 "
                 "(unnamed data)"),
    0x00066B4C: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0012150E ??0WeaponMain in Weapon.obj (row unit "
                 "RatRaceRodeo.obj: fold-name) fails reloc; needs ??_7WeaponMain@@6B@ 0x001F2770 (site-corr)"),
    0x00067BF9: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000640D5 ??0RockChunkMain fails reloc; needs "
                 "??_7RockChunkMain@@6B@ 0x001D9548 (site-corr), ?_chunkCount@RockChunkMain 0x002364F0 (site-corr)"),
    0x00067E78: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00067C3F ??0MrsGMain fails reloc; needs "
                 "?Create@SwampParticleManager 0x000B8AFF (site-corr), ??_7MrsGMain@@6B@ 0x001D9E30 (site-corr)"),
    0x00069D82: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00069CBD ??0NetMain fails reloc; needs "
                 "??_7NetMain@@6B@ 0x001DA578 (site-corr)"),
    0x0006A74F: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0006A615 ??0LanternMain fails reloc; needs "
                 "??_7LanternMain@@6B@ 0x001DA8D8 (site-corr)"),
    0x0006C1F2: ("unit-coupled", "5.4 T12.1: no ??0BellMain row in candidates (ctor inlined or absent); needs the "
                 "Bell.obj code that emits the vtable matched first (C0320)"),
    0x0006C681: ("refused-by-tool", "5.4 T12.1: companion ctor 0x0009FB0D ??0WheelPlatformAMain in MainTunnel.obj "
                 "done (row unit Bell.obj: fold-name, match_one refuses `not defined`); needs the row renamed to "
                 "Bell.obj's own folded class (C0377)"),
    0x000716AB: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x001220E8 ??0DialogueRegionMain in "
                 "DialogueRegion.obj (row unit theSquare.obj: fold-name) fails reloc; needs "
                 "??_7DialogueRegionMain@@6B@ 0x001F2B20 (site-corr)"),
    0x00073BE0: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00058A18 ??0ZombieCryptMain in "
                 "ZombieGuidanceCounselor.obj (row unit theSquare.obj: fold-name) fails reloc; needs "
                 "??_7ZombieCryptMain@@6B@ 0x001D6798 (site-corr)"),
    0x00073E24: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00073D67 ??0VinceKickedOut fails reloc; needs "
                 "??_7VinceKickedOut@@6B@ 0x001DCB60 (site-corr)"),
    0x00074389: ("reloc-data", "5.4 T12.1: companion ctor 0x00074151 ??0SkeletonJazzGuyMain fails reloc; needs 8 "
                 "unnamed targets 0x001D8F64..0x001DCD90"),
    0x00077F62: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00077F2A ??0VinceShared fails reloc; needs "
                 "??_7VinceShared@@6B@ 0x001DDD78 (site-corr)"),
    0x0007912F: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00078E5E ??0BlenderMain fails reloc; needs "
                 "?Create@SwampParticleManager 0x000B8AFF (site-corr), ??_7BlenderMain@@6B@ 0x001DE060 (site-corr)"),
    0x0007984C: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000796EC ??0FlourSackMain fails reloc; needs "
                 "0x0010F0F8 and ??_7FlourSackMain@@6B@ 0x001DE3C0 pinned, slot 0x00079B36 named (T12.1.c3); prior: "
                 "3 attempts, best 100"),
    0x0007A1C3: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0007A185 ??0GingerDeadMan fails reloc; needs "
                 "??0Critter 0x000EDA8A (site-corr), ??_7GingerDeadMan@@6B@ 0x001DE550 (site-corr)"),
    0x0007B033: ("reloc-data", "5.4 T12.1: companion ctor 0x0007AA42 ??0GingerDeadManMain fails reloc; needs "
                 "0x001DE8A0 (unnamed data)"),
    0x0007B380: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0007B2CF ??0OvenMain fails reloc; needs "
                 "??_7OvenMain@@6B@ 0x001DEA50 pinned, slot 0x0007B761 named (T12.1.c3); prior: 4 attempts, best 100"),
    0x0007BC6F: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0007BB1B ??0ScaleMain fails reloc; needs "
                 "??_7ScaleMain@@6B@ 0x001DEB90 (site-corr)"),
    0x0007C238: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0007C190 ??0WallSwitchMain fails reloc; needs "
                 "??_7WallSwitchMain@@6B@ 0x001DECC0 (site-corr)"),
    0x0007FAE6: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0008023B ??0PrimoMain fails reloc; needs "
                 "??_7PrimoMain@@6B@ 0x001DF908 (site-corr)"),
    0x000810A8: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00080967 ??0ReggieWreckageMain fails reloc; "
                 "needs ??_7ReggieWreckageMain@@6B@ 0x001DFC70 (site-corr)"),
    0x0008F6B0: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0008F3D8 ??0DollyMain fails reloc; needs "
                 "??_7DollyMain@@6B@ 0x001E3B08 (site-corr)"),
    0x0009DF88: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0009DEFE ??0FlamePipesMain fails reloc; needs "
                 "??_7FlamePipesMain@@6B@ 0x001E73B0 (site-corr)"),
    0x000A22DD: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000A20BC ??0LaundryCarMain fails reloc; needs "
                 "??_7LaundryCarMain@@6B@ 0x001E8688 (site-corr)"),
    0x000A27B2: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000A26B8 ??0VinceLaundryCar fails reloc; needs "
                 "vptr store target 0x001E8798 pinned, then the ctor matched (T12.1.c3)"),
    0x000AA6F2: ("refused-by-tool", "5.4 T12.1: companion ctor 0x0004DE90 ??0AreaLoadRegionMain in "
                 "BumpercarBumpoff.obj done (row unit ThrottleUp.obj: fold-name, match_one refuses `not defined`); "
                 "needs the row renamed to ThrottleUp.obj's own folded class (C0377)"),
    0x000C8A77: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000C88CB ??0FireFlyNode fails reloc; needs "
                 "??_7FireFlyNode@@6B@ 0x001EB4A8 (site-corr)"),
    0x000C8A93: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000C8A19 ??0FireFlyNodeClone fails reloc; needs "
                 "??_7FireFlyNodeClone@@6B@ 0x001EB4D0 (site-corr)"),
    0x000CB46B: ("callee-unnamed", "5.4 T12.1: companion ctor 0x000CB04C ??0SwampCamera fails reloc; needs "
                 "0x0002DE9D (unnamed code)"),
    0x000E1E61: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000E4764 ??0OptionsMenuGuts fails reloc; needs "
                 "0x00238C4C (unnamed data), 0x002398F4 (unnamed data), 0x002398F8 (unnamed data), 0x002398FC "
                 "(unnamed data), ??_7OptionsMenuGuts@@6B@ 0x001EC8BC (site-corr)"),
    0x000E2D89: ("refused-by-tool", "5.4 T12.1: companion ctor 0x0013F3E8 ??0VoodooShopVinceMain in "
                 "VoodooShopVince.obj done (row unit HUDUI.obj: fold-name, match_one refuses `not defined`); needs "
                 "the row renamed to HUDUI.obj's own folded class (C0377); prior: 2 ledger attempts, best None"),
    0x000E6178: ("unit-coupled", "5.4 T12.1: no ??0PauseUI row in candidates (ctor inlined or absent); needs the "
                 "HUDUI.obj code that emits the vtable matched first (C0320)"),
    0x000E8469: ("unit-coupled", "5.4 T12.1: no ??0SkullUI row in candidates (ctor inlined or absent); needs the "
                 "HUDUI.obj code that emits the vtable matched first (C0320)"),
    0x000E88C5: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000E86A5 ??0HealthUI fails reloc; needs "
                 "0x0002DF55 (unnamed code), ??_7HealthUI@@6B@ 0x001ED544 (site-corr)"),
    0x000E8A5E: ("unit-coupled", "5.4 T12.1: no ??0BeadsUI row in candidates (ctor inlined or absent); needs the "
                 "HUDUI.obj code that emits the vtable matched first (C0320)"),
    0x000FDAE9: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x000FDA9A ??0AnimCam fails reloc; needs "
                 "??_7AnimCam@@6B@ 0x001EEF90 (site-corr)"),
    0x001113F7: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00110B55 ??0BubbaGatorMain fails reloc; needs "
                 "??_C@_05IJFEKAIL@tail3?$AA@ 0x001F02BC (site-corr), ??_7BubbaGatorMain@@6B@ 0x001F0648 (site-corr)"),
    0x00129BBE: ("reloc-data", "5.4 T12.1: companion ctor 0x001298F2 ??0VinceSwing fails reloc; needs 0x0002DF55 "
                 "(unnamed code), 0x00122751 (unnamed code), 0x00128D9D (unnamed code), 0x001F7940 (unnamed data)"),
    0x0012D3D2: ("reloc-data", "5.4 T12.1: companion ctor 0x0012D338 ??0SignTrolleyMain fails reloc; needs "
                 "0x001F8350 (unnamed data)"),
    0x0012F261: ("unit-coupled", "5.4 T12.1: no ??0TrolleyScreen row in candidates (ctor inlined or absent); needs "
                 "the SignTrolley.obj code that emits the vtable matched first (C0320)"),
    0x0013064C: ("reloc-data", "5.4 T12.1: companion ctor 0x00130513 ??0TrolleyMain fails reloc; needs 0x001F8678 "
                 "(unnamed data)"),
    0x001322E6: ("refused-by-tool", "5.4 T12.1: companion ctor 0x0008C38C ??0VinceZapHat in VinceZapHat.obj pass "
                 "(row unit FanCeiling.obj: fold-name, match_one refuses `not defined`); needs the row renamed to "
                 "FanCeiling.obj's own folded class (C0377)"),
    0x00133F3D: ("callee-unnamed", "5.4 T12.1: companion ctor 0x00133E96 ??0WhammyProjectileMain fails reloc; needs "
                 "0x0002DE9D (unnamed code)"),
    0x001348F8: ("refused-by-tool", "5.4 T12.1: companion ctor 0x0012177A ??0VinceFlat in VinceFlat.obj done (row "
                 "unit Whammy.obj: fold-name, match_one refuses `not defined`); needs the row renamed to "
                 "Whammy.obj's own folded class (C0377)"),
    0x001349BD: ("reloc-provisional", "5.4 T12.1: companion ctor 0x0013469C ??0WhammyResultFrozenMain fails reloc; "
                 "needs ?GetStandardAnimation@MonsterMain 0x000F4B90 (site-corr)"),
    0x00134E35: ("reloc-data", "5.4 T12.1: companion ctor 0x00134D1B ??0VinceWhammyMain fails reloc; needs "
                 "0x001F96D4 (unnamed data), 0x001F96E8 (unnamed data)"),
    0x00135F1E: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x00135939 ??0WhammyPropMain fails reloc; needs "
                 "0x000380F3 (unnamed code), ??_7WhammyPropMain@@6B@ 0x001F99E8 (site-corr)"),
    0x00136A64: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x001369D4 ??0EyeCam fails reloc; needs "
                 "??_7EyeCam@@6B@ 0x001F9D84 (site-corr)"),
    0x0013A54A: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0013AAE9 ??0VoodooShopLevelMain fails reloc; "
                 "needs 17 unnamed targets 0x001DA954..0x001FADF4, ??_7VoodooShopLevelMain@@6B@ 0x001FAAD8 "
                 "(site-corr)"),
    0x0013E05D: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0013E079 ??0FrontEndMenu fails reloc; needs "
                 "??_7FrontEndMenu@@6B@ 0x001FB1B4 (site-corr)"),
    0x0013E46E: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0013E119 ??0ExtrasMenu fails reloc; needs "
                 "??_7ExtrasMenu@@6B@ 0x001FB200 (site-corr)"),
    0x0013EC13: ("wall-vtable-relink", "5.4 T12.1: companion ctor 0x0013F0F1 ??0ChaptersMenu fails reloc; needs "
                 "__ftol2 0x00141F74 (site-corr), ??_7ChaptersMenu@@6B@ 0x001FB26C (site-corr)"),
    0x0014FBC5: ("callee-unnamed", "5.4 T12.1: companion ctor 0x0014FA4E ??0Decal fails reloc; needs 0x0002DE9D "
                 "(unnamed code), __ftol2 0x00141F74 (site-corr)"),
    # 5.4 T15: attempted-open rows given their verdict (logs/T15.c1-c6.md)
    **{va: ("wall-vtable-relink", f"5.4 T15: objdiff 100, only the vptr store masked; needs {vt} pinned (T15.{c}); T25.c1: pin_rows refused slot-names (slot-unnamed/slot-name-differs/slot-site-corr) and vtable-slots (vtable-differs), C0412")
       for va, vt, c in (
        (0x00136386, "??_7VinceWalkOut@@6B@ 0x001F9BB0", "c5"), (0x000A26B8, "??_7VinceLaundryCar@@6B@ 0x001E8798", "c5"),
        (0x000A0D59, "??_7ChuteKillerDilloMain@@6B@ 0x001E81C8", "c3"),
        (0x00071194, "??_7GateFrogMain@@6B@ 0x001DB8E8", "c3"), (0x0003DE5D, "??_7TrailerFrogMain@@6B@ 0x001CF6C0", "c3"),
        (0x0003C202, "??_7FrogOfMysteryMain@@6B@ 0x001CEC78", "c3"),
        (0x00068019, "??_7MrsGCollapsedMain@@6B@ 0x001D9F60 + literals 0x001D9FD4, 0x001D9F50", "c4"))},
    # 5.5 T8: 0x0007B2CF ??0OvenMain dropped (matched); 7 T15 ctor keys re-noted below with their T8 wall
    # 5.5 T8 wave (logs/T8.c1-c6, C0430): vptr ctors that pass match_one 100 but fail precheck's relink
    **{va: ("wall-vtable-relink", "5.5 T8: match_one 100; relink `our ranges overlap at 0x000E2D89`: slot 0 is the "
            "shared folded ??_E/??_G body, no ??_G<class> fold-alias pin there; needs that pin + re-cut (C0250, C0430)")
       for va in (0x000816C8, 0x00129D49, 0x0006B9B0, 0x0005D5AE)},
    **{va: ("wall-vtable-relink", "5.5 T8: match_one 100; relink: vtable slot ?SetState@CritterMain@@UAEXH@Z does not "
            "resolve by name; needs that slot pinned (C0203, C0430)") for va in (0x0009DCC3, 0x00126256)},
    # 5.5 T8: 0x000717E7 DialogueBurlesque creationFunc matched after match_one.resolve dedupes a name+VA dual row
    **{va: ("wall-vtable-relink", "5.5 T8: match_one 100; relink .rdata digest differs: the emitted ??_7 does not "
            "equal retail's (C0430)") for va in (0x0012D338, 0x001317E0)},
    # 5.5 T6: the 5.4 T15 callee-unnamed keys 0x0010892C, 0x000D9519, 0x000DA108, 0x000D91A7, 0x000C5B67,
    # 0x0003786D, 0x000D37C0, 0x0013EC2F dropped (matched; stale keys now fail)
    0x000E1053: ("wall-data-name", "5.4 T15: GetBeadDest needs .data 0x0021EE68 (0.125f) named; then UIBeadMain 0x000E27C0 with it (T15.c1, 5.3 T9); T25.c1: proto twin reads ?SETTINGS_FILE@@3PBDB+4, no public at the VA, no pin (D78)"),
    0x000E0421: ("wall-data-name", "5.4 T15: ReadErrorHalt: flag 0x00238C28 has no PDB public (D78); also CXBFont::Create 0x000FFC12, literals 0x001EC730/0x001EC7C8/0x001EC7D4; no draft (T15.c1)"),
    **{va: ("wall-d78", f"5.4 T15: retail calls 0x000B9E3C with 2 args, proto SoundManager::SetMute(bool) has 1 (D78); best {b} (T15.c2)")
       for va, b in ((0x001086CE, "89.09"), (0x00108762, "96.88"))},
    0x000640D5: ("unit-coupled", "5.4 T15: ??0RockChunkMain passes (100, bytes, masked 0) only at unit opt /O1 /Oy /Ob1 (C0121; SetState inlined at /O1 /Oy); every matched RatRaceRodeo row must re-pass under /Ob1 (T15.c4)"),
    0x0008F1E9: ("wall-thinker", "5.4 T15 M17: DollyMain SetState best 97.62: retail keeps Fetch's by-value temp in the dead arg slot "
                 "[ebp+8] and copy-constructs ah from it; levers P2a static_cast<const ResourceHandle&>(Fetch()) and P2b *&Fetch() "
                 "let cl elide the copy (ah built at [ebp-4], 377 B, normdiff 111); C0225/C0275 class (T15.c3, T15.c7)"),
    0x0010EB1A: ("wall-thinker", "5.4 T15 M17: CarriedObject Message best 97.87 (15 ledger + 8 probes): frame slot packing 0xf0 vs 0xe4 "
                 "(rotation temp -0x84 vs -0x78) + esi/edi swap in USE; temps from inlined free Vector3 operators and one-arg "
                 "Quaternion::Apply the PDB does not record (D78/D117); C0191 hand forms make scoped temps (T15.c4)"),
    0x000ED7EA: ("wall-thinker", "5.4 T15 M17: CanJorgeCollideWithMe best 99.92: branch layout, one jcc target +0x54 (jump threading through "
                 "two known bools, C0400); ~60 forms (T8 29, T15 31) cover every logical spelling, no PDB locals, proto byte-identical; "
                 "only a blind permutation search left (T15.c6)"),
    0x0013F331: ("wall-thinker", "5.4 T15 M17: AnimationFinished@VoodooShopVinceMain best 96.27: regalloc, retail spills min to the dead "
                 "handle slot [ebp-4] and copies to ebx after a lazy push; no allowed construct forces it; PDB locals only `this` (T15.c6)"),
}
# Symbols past the stop whose pa-thinker consultation (M17) is done: symbol -> source. None in 3.3.
THINKER = {  # M17 pa-thinker rounds (symbol -> consultation)
    "?SetSubtitle@HUDUI@@QAEXPBGM@Z": "5.4 T15 M17: lever P1a matched (T15.c7)",
    "?SetState@DollyMain@@MAEXH@Z": "5.4 T15 M17: levers P2a/P2b tried, copy elided; wall-thinker",
    "?Message@CarriedObjectMain@@UAEXPAVSwampScript@@HPAX@Z": "5.4 T15 M17: wall-thinker (slot packing, D78/D117)",
    "?CanJorgeCollideWithMe@Critter@@UBE_NPBVSwampSceneNode@@ABUVector3@@@Z": "5.4 T15 M17: wall-thinker (branch layout, C0400)",
    "?AnimationFinished@VoodooShopVinceMain@@UAEXXZ": "5.4 T15 M17: wall-thinker (regalloc spill to dead slot)",
}

# (key, verdict, needs); the order is the precedence.
BUCKETS = [
    ("wall-d45", "byte-matched only as `__asm` (rdtsc); D45 refuses an asm body",
     "nothing under the current rules: cl 13.10 has no `__rdtsc` intrinsic (C0084); permanent wall"),
    ("wall-dir32", "base-displacement DIR32 reloc the delink cut does not express; objdiff stays < 100 (C0155)",
     "a delink rule for base-disp DIR32 fields (C0155); source cannot fix it"),
    ("refused-by-tool", "the row's lane name is a pin only; matched_set / match_finish refused it until D118 (4.2 T5: "
     "a sized disasm-verified pin whose pin row passes pin_rows.py is added; 0x0010027B, 0x00125F98 pass add on a "
     "scratch list)",
     "match_finish on the row (D118 lifted the sole-name refusal); an unruled pin: its pin row passing pin_rows.py, or a symbols.txt func line "
     "(units_seed --write, never re-run on a hand-edited table; 4.2 T3)"),
    ("held-d78-inline", "match needed an inline helper absent from the PDB; held by the 3.4 T12 ruling (D78); "
     "D117 (4.2) frees them by a pointer local or hand forms, no helper",
     "match_finish with the D117 lever its note names (C0191); a row no allowed form matches goes to the developer"),
    ("wall-data-name", "a data target's only name is undecorated (PDB data records, C0129)",
     "a decorated data name for the target (T12)"),
    ("wall-vtable-relink", "ctor stores a vptr; the vtable target is unnamed, relink fails (C0123)",
     "vtable pins (C0129, 3.3 T4)"),
    ("xdk-callee-unnamed", "a callee in an XDK library unit has no symbol",
     "the XDK callee named (symbols pin)"),
    ("callee-unnamed", "a masked callee (CRT, EH funclet, template copy, XDK) has no usable decorated symbol (5.2 T12)",
     "the callee named by a sized symbols pin with its evidence (C0192, C0331)"),
    ("unit-coupled", "the row matches only together with unmatched code of its unit (a callee D127 bars, a dependent "
     "row, temp-label numbering) (5.2 T12)",
     "the coupled row matched first or in the same batch"),
    ("wall-d78", "no allowed source form found; the matching form needs a construct with no PDB record (D78) (5.2 T12)",
     "a ruling, or new evidence for the construct"),
    ("folded-template-name", "the callee is a folded template instance with no single name (T12)",
     "a fold-VA naming rule for the template instance (C0154)"),
    ("wall-thinker", f"attempted >= {STOP} times; pa-thinker consulted (M17), its round run, residuals unmoved: wall",
     "a new lever outside the consultation (the row's note names residuals and untried leads)"),
    ("held-d78", "objdiff 100 and bytes equal, held under NON_MATCHING by D78",
     "a source for the inlined callee's body (e.g. `Direct3D::Release`); the constant has no PDB/disasm/string origin"),
    ("inline-only", "header inline with no standalone emission from its own unit",
     "its unmatched caller matched first (MoviePlayer::IsActive: a caller in Intro.obj)"),
    ("cut-wall-attempted", "attempted; bytes equal but objdiff < 100; cut symbol runs past the row size (C0085)",
     "a function symbol at the unsymboled tail target in the delink cut; source cannot fix it"),
    ("cut-body-merged", "a pre-existing Ghidra body spans past the row into its neighbour's entry (ApplySymbols "
     "`inside`); a symbols pin cannot split it",
     "the merged body clipped at the neighbour's entry in ApplySymbols (rename path never clips), then a fresh cut"),
    ("relink-refused", "objdiff 100 and bytes equal, but the gate's relink refuses the unit (PE entry != pin entry: "
     "out-of-line COMDAT copy of a folded member, C0136); rows sharing the construct included",
     "a relink rule for out-of-line copies of folded COMDATs (tools/relink.py, verifier-locked: a plan decision)"),
    ("pin-only-alias", "self-alias folded VA named only by pins; since 3.4 T6.1 matched_set add passes (4.2 T3: all 8), so nothing refuses "
     "them (sole-name pins too since D118, 4.2 T5); was: match_finish refuses, no func line in symbols.txt (C0132)",
     "a draft under the alias name the source instantiates, then match_finish; no symbols.txt line (4.2 T3)"),
    ("past-stop-thinker", f"attempted >= {STOP} times without a match; pa-thinker consulted (M17)",
     "the consultation's lever tried, else filed as a wall"),
    ("past-stop", f"attempted >= {STOP} times (ledger or recorded scratch runs) without a match; thinker not yet consulted",
     "one pa-thinker consultation (M17) before any further attempt"),
    ("attempted-open", f"attempted < {STOP} times, no match, no recorded wall",
     "more attempts, from the ledger's best run (CK18)"),
    ("cut-nosym", "the cut has no function symbol with the row's name",
     "a function symbol for the row in the delink cut (delink/Ghidra naming)"),
    ("cut-extent", "never attempted; cut symbol extent > row size (C0093 tell)",
     "a function symbol at the unsymboled target past the row in the cut, else objdiff stays < 100"),
    ("bodymax", "Ghidra body ends before the row size (candidates `bodymax`)",
     "the Ghidra function boundary fixed, then a fresh cut"),
    ("stl-reloc", "STL instantiation (`std::` name) with unresolved reloc targets",
     "both: the targets named (symbols pin or matched callee) and the STL route (explicit instantiation, C0096; T10.1)"),
    ("stdsig-reloc", "game function with a std type in its signature (`@std@@`), unresolved reloc targets",
     "both: the targets named and opaque std decls from VC 7.1 headers (C0096; T12 BeepFile); library filter is policy"),
    ("reloc-provisional", "reloc fails on provisional pins only (status outside STATUSES, e.g. site-corr, boundary)",
     "the pins verified (status promoted, e.g. disasm-verified) or the targets matched with a symbol"),
    ("reloc-code", "reloc gaps in the cut: >= 1 unresolved target, every unresolved target a call/jump (rel)",
     "the callees named: pinned in symbols or matched with a symbol (C0084, C0059)"),
    ("reloc-data", "reloc gaps in the cut: at least one unresolved absolute (abs) target",
     "the data or pointer targets named as symbols (globals, vtables, literals)"),
    ("stl", "STL instantiation (`std::` name, candidates `library`), relocs clean",
     "the STL route: explicit instantiation from VC 7.1 headers (C0096; T10.1 `_Uninit_fill_n` matched this way)"),
    ("stdsig", "game function with a std type in its signature (`@std@@`), relocs clean",
     "opaque std decls from VC 7.1 headers (C0096; T12 BeepFile); the `library` filter is policy, not a wall"),
    ("large", "passing, never attempted, larger than any matched function",
     "an untested size: expect more attempts per row (2.5 matched nothing this size)"),
    ("ready-opt-unknown", "passing, never attempted; unit `opt` is \"unknown\"",
     "the unit's opt decided by a one-flag control on its first claim (C0065), then a batch claim"),
    ("ready", "passing, never attempted; unit `opt` set",
     "a batch claim only (next batch input)"),
]


def refuse(msg):
    print(f"pool_buckets: REFUSED {msg}")
    sys.exit(2)


def sha8(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()[:12]


def as_int(v):
    return v if isinstance(v, int) else int(str(v), 16)


def assign(rows, matched, opt, pin, cut, led, maxsize, objs, used):
    """Bucket the unmatched rows of one lane (BUCKETS order); used collects the NOTES/NOTES_VA keys hit (3.4 T13)."""
    out, extra = defaultdict(list), Counter()
    targets = Counter()
    unmatched = [r for r in rows if (r["symbol"], r["va"]) not in matched]
    for r in unmatched:
        if r["unit"] not in opt:
            refuse(f"unit {r['unit']} not in units.toml")
        f = byte_compare.cut_dir(pin) / r["unit"].replace("\\", "__").replace("/", "__")
        if f.name not in objs:
            if not f.is_file():
                refuse(f"unit {r['unit']} absent from the cut {cut.relative_to(REPO)}")
            objs[f.name] = byte_compare.load_obj(f)[1]
        c = objs[f.name]
        fs = [y for y in c["symbols"].values() if byte_compare.is_func(y) and y["name"] == r["symbol"]]
        ext = None
        if fs:
            _s, lo, hi = byte_compare.extent(c, fs[0])
            ext = hi - lo
        runs = [x for x in {id(x): x for x in led[r["va"]] + led[r["symbol"]]}.values() if match_one.is_attempt(x)]
        tries = len(runs)  # 5.2 T7: match_one.is_attempt rows only (refusals and controls never count)
        ck = r["checks"]
        unres = [s for s in r["sites"] if s["resolved"] is None]
        note = NOTES.get(r["symbol"]) or NOTES_VA.get(r["va"])
        used.add(r["symbol"] if r["symbol"] in NOTES else r["va"] if note else None)
        if note:
            b = note[0]
            if b == "past-stop" and r["symbol"] in THINKER:
                b = "past-stop-thinker"
        elif runs and ext is not None and ext > r["size"]:
            b = "cut-wall-attempted"
        elif runs:
            b = ("past-stop-thinker" if r["symbol"] in THINKER else "past-stop") if tries >= STOP \
                else "attempted-open"
        elif ext is None:
            b = "cut-nosym"
        elif ext > r["size"]:
            b = "cut-extent"
        elif not ck["bodymax"]["pass"]:
            b = "bodymax"
        elif not ck["library"]["pass"] and not ck["reloc"]["pass"]:
            b = ("stl" if r["names"][0].startswith("std::") else "stdsig") + "-reloc"
        elif not ck["reloc"]["pass"]:
            b = ("reloc-data" if any(s["kind"] != "rel" for s in unres) else "reloc-code") if unres                 else "reloc-provisional"
            ut = {s["target"] for s in unres}
            if len(ut) == 1:
                extra[(b, "one")] += 1
                targets[next(iter(ut))] += 1
        elif not ck["library"]["pass"]:
            b = "stl" if r["names"][0].startswith("std::") else "stdsig"
        elif not r["pass"]:
            refuse(f"rank {r['rank']} fails a check no bucket covers: {[k for k, v in ck.items() if not v['pass']]}")
        elif r["size"] > maxsize:
            b = "large"
        elif opt[r["unit"]] == "unknown":
            b = "ready-opt-unknown"
        else:
            b = "ready"
        out[b].append(r)
    return unmatched, out, extra, targets


NEAR_NEEDS = {  # 3.4 T13: near-lane reloc gaps are callees the lane name's prototype calls but retail leaves unnamed
    "reloc-code": "the unnamed callee named: pinned in symbols or matched with a symbol (C0059, C0101)",
    "reloc-provisional": "the unnamed callee's provisional pin verified (status promoted) or the callee matched "
                         "with a symbol",
}
QA_CLASSES = ("identical", "near-identical", "excluded", "ambiguous", "absent")
QA_ROW = re.compile(r"^- (0x[0-9A-F]{8}) (.*?)(?:: (0x[0-9A-F]{8} .*))?$")


def render(L, out, extra, targets, h, needs_over):
    """Bucket subsections, single-target gaps and rows by bucket for one lane; h = heading prefix."""
    L += [f"{h} Buckets", ""]
    for k, verdict, needs in BUCKETS:
        rs = sorted(out.get(k, []), key=lambda r: r["rank"])
        ex = f"`{rs[0]['names'][0]}` (rank {rs[0]['rank']}, {rs[0]['size']} B, {rs[0]['unit'].split(chr(92))[-1]})" \
            if rs else "-"
        L.append(f"{h}# {k}: {len(rs)}")
        L.append(f"- verdict: {verdict}")
        L.append(f"- needs: {needs_over.get(k, needs)}")
        L.append(f"- example: {ex}")
        if rs:
            src = sorted({(NOTES.get(r["symbol"]) or NOTES_VA[r["va"]])[1] for r in rs
                          if r["symbol"] in NOTES or r["va"] in NOTES_VA})
            units = Counter(r["unit"] for r in rs)
            tags = Counter(t for r in rs for t, v in r["tags"].items() if v == "y")
            bands = Counter("<=32" if r["size"] <= 32 else "33-128" if r["size"] <= 128 else
                            "129-512" if r["size"] <= 512 else ">512" for r in rs)
            L.append("- size bytes: " + ", ".join(f"{b} {bands[b]}" for b in ("<=32", "33-128", "129-512", ">512")))
            L.append(f"- units {len(units)}; tags " + (", ".join(f"{t} {n}" for t, n in sorted(tags.items())) or "none")
                     + (f"; source {', '.join(src)}" if src else "")
                     + (f"; one unresolved target {extra[(k, 'one')]}" if (k, "one") in extra else ""))
        L.append("")
    L += [f"{h} Single-target reloc gaps", "",
          "Unresolved targets that alone block a reloc-code/reloc-data row (rows unblocked by naming that one target):", ""]
    for t, n in sorted(targets.items(), key=lambda kv: (-kv[1], kv[0]))[:15]:
        L.append(f"- 0x{t:06X}: {n}")
    L += ["", f"{h} Rows by bucket (ranks, ascending)", ""]
    for k, _v, _n in BUCKETS:
        rs = sorted(r["rank"] for r in out.get(k, []))
        L.append(f"- {k} ({len(rs)}): " + (", ".join(map(str, rs)) or "-"))
    L.append("")


def unit_at(units, ustarts, va):
    """The units.toml unit whose text range holds va (units sorted by text_start), else None."""
    i = bisect.bisect_right(ustarts, va) - 1
    return units[i] if i >= 0 and units[i]["text_start"] <= va < units[i]["text_end"] else None


def family(name):
    """5.3 T1 row family from names[0]: creationFunc, dtor, ctor (last two `::` parts equal, template args stripped),
    Message, FrameTick, other."""
    if "::creationFunc" in name:
        return "creationFunc"
    if "::~" in name:
        return "dtor"
    s = name
    while (s2 := re.sub(r"<[^<>]*>", "", s)) != s:
        s = s2
    p = s.split("::")
    if len(p) >= 2 and p[-1] == p[-2]:
        return "ctor"
    return "Message" if name.endswith("::Message") else "FrameTick" if name.endswith("::FrameTick") else "other"


def outside_lane(fx):
    """Near-identical rows outside the lane by candidates.py's exclusion (candidates.py:343-361, its helpers)."""
    try:
        folds = {int(r["retailVA"], 16) for r in candidates.load_jsonl(candidates.need(candidates.FOLDS))}
        units = sorted((u for u in tomllib.loads(candidates.need(UNITS).read_text(encoding="utf-8"))["unit"]
                        if "text_start" in u), key=lambda u: u["text_start"])
        funcs, _data = candidates.load_symbols()
    except candidates.Refuse as e:
        refuse(str(e))
    ustarts = [u["text_start"] for u in units]
    why, lane = {}, 0  # why: protoVA -> (reason, sub-reason)
    for r in fx:
        if r["class"] != "near-identical":
            continue
        if not r.get("candidateVA") or r.get("similarity") is None or r["similarity"] < proto_map.THRESHOLD:
            why[r["protoVA"]] = (f"no candidateVA/sim<{proto_map.THRESHOLD}", None)
            continue
        va = int(r["candidateVA"], 16)
        u = unit_at(units, ustarts, va)
        pv =int(r["protoVA"], 16) - candidates.PROTO_BASE
        f = funcs.get(va)
        if u is None:
            why[r["protoVA"]] = ("cut", "no unit")
        elif u["kind"] != "game":
            why[r["protoVA"]] = ("cut", "unit not game")
        elif not u["delink"]:
            why[r["protoVA"]] = ("cut", "unit not delink")
        elif candidates.TAIL[0] <= pv < candidates.TAIL[1]:
            why[r["protoVA"]] = ("cut", "tail")
        elif va in folds:
            why[r["protoVA"]] = ("fold", None)
        elif f is None:
            why[r["protoVA"]] = ("no name evidence", None)
        elif f["size"] is None:
            why[r["protoVA"]] = ("name not admitted", "size -")
        elif f["status"] not in candidates.NEAR_STATUSES:
            why[r["protoVA"]] = ("name not admitted", f"status {f['status']}")
        else:
            lane += 1
    return why, lane


def recount(img, fx, entries, gaps):
    """5.3 T1 recount (lifted 5.3 T3): proto_map's matcher (proto_map.py:444-445, :488-494) uncapped over the retail exec
    sections; bodies = hits that are Ghidra entries or gaps. -> (A [(P, row, sorted bodies)] by protoVA, H hits per row,
    pbs {P: proto bytes}, rlen {P: row length})."""
    secs = sorted((s["virtualAddr"], img.xb[s["rawAddr"]:s["rawAddr"] + s["rawSize"]])
                  for s in img.x["sections"] if s["flags"] & proto_map.SEC_EXEC)
    prs = sorted(img.prelocs)
    A, H, pbs, rlen = [], [], {}, {}  # A (P, row, sorted bodies) by protoVA; H the row's recounted hits
    for r in sorted((r for r in fx if r["class"] == "ambiguous"), key=lambda r: int(r["protoVA"], 16)):
        P, n = int(r["protoVA"], 16), r["length"]
        pb, hits = img.pbytes(P, n), []
        if pb is not None:
            ms = set(proto_map.mask_proc(pb, P, img.plo, img.phi)[0])
            ms |= {(x - P, 4, "abs") for x in prs[bisect.bisect_left(prs, P):bisect.bisect_left(prs, P + n)]}
            segs = proto_map.segments(n, sorted(ms))
            anc = proto_map.anchor(segs)
            hits = proto_map.find_matches(secs, pb, segs, anc, cap=None) if anc else []
            pbs.setdefault(P, pb)
            rlen.setdefault(P, n)
        A.append((P, r, sorted(set(hits) & (entries | gaps))))
        H.append(hits)
    return A, H, pbs, rlen


def amb_index(A, H):
    """5.3 T1 (lifted 5.3 T3): -> (claims {t: rows}, erow {P: [bodies]}, rowt {P: hits}, eset {P: bodies},
    tclaim {t: [bodies of each row claiming t]})."""
    claims = Counter(t for _P, _r, es in A for t in es)
    erow, rowt, eset, tclaim = defaultdict(list), defaultdict(set), defaultdict(set), defaultdict(list)
    for (P, r, es), h in zip(A, H):
        erow[P].append(es)
        rowt[P].update(h)
        eset[P].update(es)
        for t in es:
            tclaim[t].append(es)
    return claims, erow, rowt, eset, tclaim


def bracket_windows(fx, units, matched, A):
    """5.3 T1 (lifted 5.3 T3): bracket anchors = matched rows of the row's one unit (identical targetVAs[0],
    near-identical candidateVA) around P. -> {P: (lo retail end, hi retail start, text_start, text_end)}, first row at P."""
    ubyname = {u["name"]: u for u in units}
    banc = defaultdict(list)
    for r in fx:
        rv = (int(r["targetVAs"][0], 16) if r["class"] == "identical" and r["targetVAs"] else
              int(r["candidateVA"], 16) if r["class"] == "near-identical" and r["candidateVA"] else None)
        if rv is not None and rv in matched:
            for M in r["modules"]:
                if M in ubyname:
                    banc[M].append((int(r["protoVA"], 16), rv, r["length"]))
    brk = {}  # P -> (lo, hi, text_start, text_end), from the first row at P
    for P, r, _es in A:
        if P in brk or len(r["modules"]) != 1 or r["modules"][0] not in ubyname:
            continue
        an = sorted(banc[r["modules"][0]])
        lo = max((a for a in an if a[0] < P), default=None)
        hi = min((a for a in an if a[0] > P), default=None)
        if lo and hi:
            u = ubyname[r["modules"][0]]
            brk[P] = (lo[1] + lo[2], hi[1], u["text_start"], u["text_end"])
    return brk


def xref_ok(ctx, P, t):  # near_check :1491-1493: agree >= 1, conflicts 0
    bp = ctx["by_pd"].get(P, ())
    return (any(rd == t and cva != t for rd, cva in bp) and all(rd == t for rd, cva in bp if cva != t)
            and all(pd == P for pd, cva in ctx["by_rd"].get(t, ()) if cva != t))


# 5.3 T3: the three classes ruled at 5.3 T1 (D128 bracket, D129 fold-alias, D130 fold-k1) as module functions over a
# state S (SimpleNamespace: img, pubs, ctx, rlen, eset, erow, claims, tclaim, brk); lane_sizing and name_sites share them.
def bracket(S, P, t):  # the row's one unit, the only body between its matched neighbours' retail ends
    b = S.brk.get(P)
    return (b is not None and t in S.eset.get(P, ()) and b[0] <= t < b[1] and b[2] <= t < b[3]
            and sum(1 for x in S.eset[P] if b[0] <= x < b[1]) == 1)


def fold_alias(S, P, t):  # P has >= 2 publics, caller xref agree >= 1 and conflicts 0, img.match
    return (t in S.eset.get(P, ()) and len(S.pubs.get(P, ())) >= 2 and xref_ok(S.ctx, P, t)
            and P in S.rlen and S.img.match(P, t, S.rlen[P]) is not None)


def fold_k1(S, P, t):  # the row's one body, every row claiming it has only that body
    return (t in S.eset.get(P, ()) and [t] in S.erow.get(P, ()) and S.claims[t] >= 2
            and all(es == [t] for es in S.tclaim[t]))


# T13.2.c3: the four classes ruled at 5.1 T9 (k1, D115, D113, caller-xref) lifted the same way; S also carries nc/nok
# (near_check caches) and slots (vt_slot_pairs).
def near_chk(S, P, t):
    import name_sites
    if (P, t) not in S.nc:
        S.nc[(P, t)] = name_sites.near_check(S.ctx, P, t)
    return S.nc[(P, t)]


def near_name_ok(S, P):  # near_check step 1 alone: a fail there fails every body of P, so no per-body near_check
    if P not in S.nok:
        ln_ = S.ctx["procs"].get(P)
        S.nok[P] = len(S.pubs.get(P, ())) == 1 and S.img.pexec(P) and bool(ln_) and S.img.pbytes(P, ln_) is not None
    return S.nok[P]


def cls_k1(S, P, t):  # the row's one body, claimed by no other row
    return S.claims[t] == 1 and [t] in S.erow.get(P, ())


def cls_d115(S, P, t):  # name_sites.near_check pass
    return near_name_ok(S, P) and near_chk(S, P, t)["verdict"] == "pass"


def cls_d113(S, P, t):  # (P, t) a slot pair of an accepted vtable
    return (P, t) in S.slots


def cls_caller_xref(S, P, t):  # a caller site P -> t, no other proto dest at t
    return (any(rd == t for rd, _c in S.ctx["by_pd"].get(P, ()))
            and all(pd == P for pd, _c in S.ctx["by_rd"].get(t, ())))


# T11.1.c1: the two classes ruled at 5.4 T10 (D133 registerme, D134 abs-site) lifted from lane_sizing (5.4 T9) the
# same way; S also carries rm_ok (rm_scan) and abs_pd/abs_rd (abs_index).
RM_RE = re.compile(r"\?\?0RegisterMe@(\w+)@@QAE@XZ")


def rm_scan(img, pubs, rlen, eset):
    """5.4 T9 registerme (lifted T11.1.c1): a `??0RegisterMe@X@@QAE@XZ` row and body t push the same one string literal
    at the same mask offset (retail: an abs dest in .rdata/.data, NUL-terminated; proto: a ??_C@ public), its text == X,
    img.match. -> (rm_x {P: X}, rm_ok {(P, t)}, rm_row {P: S|D|M|N}, rm_d [(P, X, text)] first 10 D rows)."""
    import name_sites
    rm_x = {P: m.group(1) for P in rlen if len(pubs.get(P, ())) == 1
            and (m := RM_RE.fullmatch(next(iter(pubs[P]))))}
    dnames = [s for s in img.x["sections"] if s.get("name") in (".rdata", ".data")]

    def rlits(t, n):  # retail literals of body t over n bytes: [(mask offset, bytes through NUL)]
        rb = img.rbytes(t, n)
        if rb is None:
            return []
        out = []
        for o, _w, k in proto_map.mask_proc(rb, t, img.rlo, img.rhi)[0]:
            if k != "abs":
                continue
            d = name_sites.target(rb, t, o, k)
            s = next((s for s in dnames if s["virtualAddr"] <= d < s["virtualAddr"] + s["rawSize"]), None)
            if s is None:
                continue
            raw = img.xb[s["rawAddr"] + d - s["virtualAddr"]:s["rawAddr"] + s["rawSize"]]
            i = raw.find(b"\0", 0, name_sites.STR_CAP)
            if i >= 0:
                out.append((o, bytes(raw[:i + 1])))
        return out

    rlc, rm_ok, rm_row = {}, set(), {}  # rm_row: P -> "S" | "D" (equal bytes, text != X) | "M" (match refused) | "N"
    rm_d = []
    for P, X in sorted(rm_x.items()):
        pb = img.pbytes(P, rlen[P])
        plit = []
        for o, _w, k in (proto_map.mask_proc(pb, P, img.plo, img.phi)[0] if pb is not None else ()):
            if k != "abs":
                continue
            d = name_sites.target(pb, P, o, k)
            nm = next((n for n in sorted(pubs.get(d, ())) if n.startswith("??_C@")), None)
            ln_ = name_sites.str_len(img, nm, d) if nm else None
            if ln_:
                plit.append((o, bytes(img.pbytes(d, ln_))))
        st = set()
        for t in sorted(eset.get(P, ())):
            if (t, rlen[P]) not in rlc:
                rlc[(t, rlen[P])] = rlits(t, rlen[P])
            rl = rlc[(t, rlen[P])]
            if len(plit) != 1 or len(rl) != 1 or plit[0] != rl[0]:
                continue
            txt = rl[0][1][:-1].decode("latin-1")
            if txt != X:
                st.add("D")
                if len(rm_d) < 10 and (not rm_d or rm_d[-1][0] != P):
                    rm_d.append((P, X, txt))
            elif img.match(P, t, rlen[P]) is None:
                st.add("M")
            else:
                st.add("S")
                rm_ok.add((P, t))
        rm_row[P] = next((c for c in "SDM" if c in st), "N")
    return rm_x, rm_ok, rm_row, rm_d


def abs_index(img, ctx, pin, chain=(), rlen=None):
    """5.3 T1 c3 abs-site (lifted T11.1.c1): near_ctx's caller index (name_sites.py:1426-1450) over `abs` masks: the
    accepted callers of symbols.txt and pin (unique public, img.match), proto dest an exec VA outside the caller; then
    (5.4 T9) each chain pair (cpva, cva) not already an accepted caller, length rlen[cpva].
    -> (abs_pd {pd: [(rd, caller)]}, abs_rd {rd: [(pd, caller)]}, abs_src {(pd, rd, caller): accepted|chain},
    abs_all {rd: [(pd, caller)]} every accepted abs mask (data dests too), abs_off {(pd, rd, caller): mask offset})."""
    import name_sites
    import units_seed
    procs = ctx["procs"]
    abs_pd, abs_rd = defaultdict(list), defaultdict(list)
    abs_src = {}  # 5.4 T9: (pd, rd, caller) -> "accepted" | "chain"
    abs_all = defaultdict(list)  # 5.4 T7: rd -> [(pd, caller)] over every abs mask (data dests too), for near_sizing (c)
    abs_off = {}  # T11.1.c1: (pd, rd, caller) -> mask offset in the caller (D135 push site)
    for tab, ok in ((candidates.SYMS, units_seed.STATUSES), (pin, candidates.PIN_STATUSES)):
        for ln in candidates.need(tab).read_text(encoding="utf-8").splitlines():
            f = ln.split()
            if len(f) < 6 or ln.startswith("#") or f[1] != "func" or f[-1] not in ok \
                    or f[-1] in ("site-corr", "boundary", "near-corr"):
                continue
            cva, pv = int(f[0], 16), ctx["pubname"].get(" ".join(f[3:-2]), set())
            if len(pv) != 1:
                continue
            cpva = next(iter(pv))
            ln_ = procs.get(cpva)
            m = img.match(cpva, cva, ln_) if ln_ else None
            if m is None:
                continue
            pb, rb = img.pbytes(cpva, ln_), img.rbytes(cva, ln_)
            for o, _w, k in m[1]:
                if k != "abs":
                    continue
                pd = name_sites.target(pb, cpva, o, k)
                abs_all[name_sites.target(rb, cva, o, k)].append((pd, cva))
                if not img.pexec(pd) or cpva <= pd < cpva + ln_:
                    continue
                rd = name_sites.target(rb, cva, o, k)
                abs_pd[pd].append((rd, cva))
                abs_rd[rd].append((pd, cva))
                abs_src[(pd, rd, cva)] = "accepted"
                abs_off[(pd, rd, cva)] = o
    # 5.4 T9 chain: every registerme-settled (P, t) as a caller of the abs-site index (same abs-mask loop as above)
    acc_c = {c for v in abs_pd.values() for _rd, c in v}
    for cpva, cva in sorted(chain):
        if cva in acc_c:
            continue
        ln_ = rlen[cpva]
        m = img.match(cpva, cva, ln_)
        pb, rb = img.pbytes(cpva, ln_), img.rbytes(cva, ln_)
        for o, _w, k in m[1]:
            if k != "abs":
                continue
            pd = name_sites.target(pb, cpva, o, k)
            if not img.pexec(pd) or cpva <= pd < cpva + ln_:
                continue
            rd = name_sites.target(rb, cva, o, k)
            abs_pd[pd].append((rd, cva))
            abs_rd[rd].append((pd, cva))
            abs_src[(pd, rd, cva)] = "chain"
            abs_off[(pd, rd, cva)] = o
    return abs_pd, abs_rd, abs_src, abs_all, abs_off


def cls_registerme(S, P, t):  # D133: the row's pushed literal equals the body's byte for byte and reads X, img.match
    return (P, t) in S.rm_ok


def cls_abs_site(S, P, t):  # D134: an abs site P -> t in an accepted caller, agree >= 1, conflicts 0, t a body of P
    return (t in S.eset.get(P, ()) and any(rd == t and c != t for rd, c in S.abs_pd.get(P, ()))
            and all(rd == t for rd, c in S.abs_pd.get(P, ()) if c != t)
            and all(pd == P for pd, c in S.abs_rd.get(t, ()) if c != t))


def vt_slot_pairs(img, ctx, pub_vas):
    """5.1 T9 (lifted T13.2.c3): accepted vtables (name_sites' table rule), proto VA = the unique PDB public of the
    name -> ({(proto slot dword, retail slot dword)}, vtables)."""
    import pin_rows  # VT_CAP; vt_slots is pin_rows.State.vt_slots re-run on name_sites' img (no State build)
    import units_seed

    def vt_slots(pt, t):
        i = bisect.bisect_right(pub_vas, pt)
        slots = []
        for k in range(min((pub_vas[i] - pt) // 4 if i < len(pub_vas) else pin_rows.VT_CAP, pin_rows.VT_CAP)):
            pd, rd = img.pbytes(pt + 4 * k, 4), img.rbytes(t + 4 * k, 4)
            if pd is None or rd is None:
                break
            pv = struct.unpack("<I", pd)[0]
            if not img.pexec(pv):
                break
            slots.append((pv, struct.unpack("<I", rd)[0]))
        return slots

    slots, nvt = set(), 0
    for tab, ok in ((candidates.SYMS, units_seed.STATUSES), (candidates.SYMS_PIN, candidates.PIN_STATUSES)):
        for ln in candidates.need(tab).read_text(encoding="utf-8").splitlines():
            f = ln.split()
            if len(f) < 6 or ln.startswith("#") or f[1] != "data" or not f[3].startswith("??_7") or f[-1] not in ok \
                    or f[-1] in ("site-corr", "boundary", "near-corr"):
                continue
            pt = ctx["pubname"].get(" ".join(f[3:-2]), set())
            if len(pt) == 1:
                nvt += 1
                slots.update(vt_slots(next(iter(pt)), int(f[0], 16)))
    return slots, nvt


def ruled_state(img, pubs, ctx, pin_path=None):
    """5.3 T3: S for bracket/fold_alias/fold_k1 (T13.2.c3: and cls_k1/cls_d115/cls_d113/cls_caller_xref) from the
    inputs main reads (functions.jsonl, Ghidra entries, gaps, matched_set.toml VAs, units.toml), plus S.A, S.H (recount
    order). Refuses (exit 2) on a missing input. T11.1.c1: and cls_registerme (S.rm_ok) / cls_abs_site (S.abs_pd,
    S.abs_rd: accepted callers of symbols.txt and pin_path, default candidates.SYMS_PIN; no chain), S.entries, S.gaps."""
    try:
        fx = candidates.load_jsonl(candidates.need(candidates.FUNCS))
        units = sorted((u for u in tomllib.loads(candidates.need(UNITS).read_text(encoding="utf-8"))["unit"]
                        if "text_start" in u), key=lambda u: u["text_start"])
    except candidates.Refuse as e:
        refuse(str(e))
    pin = coff_info.pinned_sha() or ""
    gfj = REPO / "derived" / "ghidra" / f"retail-{pin[:8]}" / "applied" / "functions.jsonl"
    if not gfj.is_file():
        refuse(f"missing input {gfj.relative_to(REPO)}")
    entries = {int(f["entry"], 16) for f in candidates.load_jsonl(gfj)}
    gaps = {g["va"] for g in tomllib.loads((REPO / "config" / "gap_functions.toml").read_text(encoding="utf-8"))
            .get("gap", [])}
    matched = {as_int(f["va"]) for f in tomllib.loads(MATCHED.read_text(encoding="utf-8")).get("function", [])}
    A, H, _pbs, rlen = recount(img, fx, entries, gaps)
    claims, erow, rowt, eset, tclaim = amb_index(A, H)
    _x, rm_ok, _r, _d = rm_scan(img, pubs, rlen, eset)  # T11.1.c1
    abs_pd, abs_rd, _s, _a, abs_off = abs_index(img, ctx, pin_path or candidates.SYMS_PIN)
    return SimpleNamespace(img=img, pubs=pubs, ctx=ctx, rlen=rlen, eset=eset, erow=erow, rowt=rowt, claims=claims,
                           tclaim=tclaim, brk=bracket_windows(fx, units, matched, A), A=A, H=H, units=units,
                           nc={}, nok={}, slots=vt_slot_pairs(img, ctx, sorted(pubs))[0],  # T13.2.c3
                           rm_ok=rm_ok, abs_pd=abs_pd, abs_rd=abs_rd, abs_off=abs_off, entries=entries, gaps=gaps)


def lane_sizing(fx, entries, why, sums, fail, matched, irows, res, keep=None):
    """5.1 T9: size the ambiguous rows and the near no-name rows by the evidence that would settle each pair; sizing
    only, nothing written to a pin file. Plants (wrong body, wrong proto per class) run first; one passing -> FAIL,
    no counts. Returns the `## Lane sizing` doc lines, or None on a failed plant. 5.3 T1: irows = unmatched identical
    pool rows (freed counts); res is filled with the ambiguous_residue.md row lines. 5.4 T7: keep (a dict) receives
    the state near_sizing reads (S, A, ev, YC, ycls, edge-settled units, slot_rv, every accepted-caller abs site)."""
    import name_sites
    import pin_rows  # VT_CAP; vt_slots is pin_rows.State.vt_slots re-run on name_sites' img (no State build)
    import units_seed
    try:
        img, pubs, procs = name_sites.load()
        ctx = name_sites.near_ctx(img, pubs, procs)
        units = sorted((u for u in tomllib.loads(candidates.need(UNITS).read_text(encoding="utf-8"))["unit"]
                        if "text_start" in u), key=lambda u: u["text_start"])
        funcs, _data = candidates.load_symbols()
    except (name_sites.Refuse, candidates.Refuse) as e:
        refuse(str(e))
    ustarts, pub_vas = [u["text_start"] for u in units], sorted(pubs)

    slots, nvt = vt_slot_pairs(img, ctx, pub_vas)  # T13.2.c3: lifted
    slot_rv = defaultdict(set)
    for pv, rv in slots:
        slot_rv[rv].add(pv)
    secb = ctx["secb"]
    secb_lo = [a for a, _b in secb]

    def sec_end(v):  # end of the retail exec section holding v, else None
        i = bisect.bisect_right(secb_lo, v) - 1
        return secb[i][1] if i >= 0 and v < secb[i][1] else None

    # 5.3 T1 data-slot: every accepted non-vtable data line, dwords from offset 0 to its size (size `-`: to the next
    # proto public, VT_CAP cap), no stop at a non-exec dword; pairs (proto exec dword, retail dword in a retail exec section)
    dslots, ndl = set(), 0
    for tab, ok in ((candidates.SYMS, units_seed.STATUSES), (candidates.SYMS_PIN, candidates.PIN_STATUSES)):
        for ln in candidates.need(tab).read_text(encoding="utf-8").splitlines():
            f = ln.split()
            if len(f) < 6 or ln.startswith("#") or f[1] != "data" or f[3].startswith("??_7") or f[-1] not in ok \
                    or f[-1] in ("site-corr", "boundary", "near-corr"):
                continue
            pt = ctx["pubname"].get(" ".join(f[3:-2]), set())
            if len(pt) != 1:
                continue
            pt, rt = next(iter(pt)), int(f[0], 16)
            if f[2] != "-":
                nd = int(f[2], 16) // 4
            else:
                i = bisect.bisect_right(pub_vas, pt)
                nd = min((pub_vas[i] - pt) // 4 if i < len(pub_vas) else pin_rows.VT_CAP, pin_rows.VT_CAP)
            ndl += 1
            for k in range(nd):
                pd, rd = img.pbytes(pt + 4 * k, 4), img.rbytes(rt + 4 * k, 4)
                if pd is None or rd is None:
                    break
                pv, rv = struct.unpack("<I", pd)[0], struct.unpack("<I", rd)[0]
                if img.pexec(pv) and sec_end(rv) is not None:
                    dslots.add((pv, rv))

    # 5.3 T1 recount: proto_map's matcher (proto_map.py:444-445, :488-494) uncapped over the retail exec sections;
    # bodies = hits that are Ghidra entries or config/gap_functions.toml gaps (progress.GAPS); functions.jsonl untouched
    gaps = {g["va"] for g in tomllib.loads((REPO / "config" / "gap_functions.toml").read_text(encoding="utf-8"))
            .get("gap", [])}
    A, H, pbs, rlen = recount(img, fx, entries, gaps)  # 5.3 T3: lifted to a module function
    under = [(P, r, h) for (P, r, _es), h in zip(A, H) if r["hits"] < proto_map.HIT_CAP]
    bad = [f"0x{P:08X}" for P, r, h in under if set(h) != {int(v, 16) for v in r["targetVAs"]}]
    sums.append(f"recount: rows under cap {len(under)} agree {len(under) - len(bad)}")
    if bad:
        fail.append(sums[-1] + " (disagree: " + ", ".join(bad) + ")")
    capd = [(r, es) for P, r, es in A if r["hits"] >= proto_map.HIT_CAP]
    kch = sum(1 for r, es in capd if len(es) != len({int(v, 16) for v in r["targetVAs"]} & entries))
    kn = Counter("0" if len(es) == 0 else "1" if len(es) == 1 else "2+" for _r, es in capd)
    allb = {t for _P, _r, es in A for t in es}
    nbe = sum(1 for t in allb if t in entries)
    sums.append(f"recount: capped rows {len(capd)}, k changed {kch}, k now 0/1/2+ {kn['0']}/{kn['1']}/{kn['2+']}, "
                f"bodies distinct {len(allb)} (entries {nbe}, gap {len(allb) - nbe})")
    if sum(kn.values()) != len(capd) or any(t not in entries and t not in gaps for t in allb):
        fail.append(sums[-1])

    claims, erow, rowt, eset, tclaim = amb_index(A, H)  # 5.3 T3: lifted
    # bracket: matched rows of the row's one unit (identical targetVAs[0], near-identical candidateVA) around P
    S = SimpleNamespace(img=img, pubs=pubs, ctx=ctx, rlen=rlen, eset=eset, erow=erow, claims=claims, tclaim=tclaim,
                        brk=bracket_windows(fx, units, matched, A), nc={}, nok={}, slots=slots)  # 5.3 T3, T13.2.c3
    chk = lambda P, t: near_chk(S, P, t)  # noqa: E731  (T13.2.c3: lifted with its cache)
    name_ok = lambda P: near_name_ok(S, P)  # noqa: E731

    def outside_window(P, t):
        if t not in eset.get(P, ()) or len(pubs.get(P, ())) != 1 or not name_ok(P) or P not in pbs \
                or chk(P, t)["reason"] != "outside-window" or not xref_ok(ctx, P, t):
            return False
        ptok = proto_map.tokens(pbs[P], P, img.plo, img.phi)[0]
        rb = img.rbytes(t, min(sec_end(t), t + 15 * len(ptok)) - t) or b""
        ctok = proto_map.tokens(rb, t, img.rlo, img.rhi)[0][:len(ptok)]
        return proto_map.similarity(ptok, ctok) >= proto_map.THRESHOLD

    # 5.4 T9 registerme (T11.1.c1: lifted to rm_scan)
    rm_x, rm_ok, rm_row, rm_d = rm_scan(img, pubs, rlen, eset)

    # 5.3 T1 c3 abs-site and 5.4 T9 chain (T11.1.c1: lifted to abs_index): accepted callers, then every
    # registerme-settled (P, t) as a caller
    abs_pd, abs_rd, abs_src, abs_all, _off = abs_index(img, ctx, candidates.SYMS_PIN, rm_ok, rlen)

    PRED = {"k1": lambda P, t: cls_k1(S, P, t),  # T13.2.c3: the 5.1 T9 four lifted to module functions over S
            "D115": lambda P, t: cls_d115(S, P, t),
            "D113": lambda P, t: cls_d113(S, P, t),
            "caller-xref": lambda P, t: cls_caller_xref(S, P, t),
            # 5.3 T1 candidates (no writer); each first requires t in es(P)
            "fold-alias": lambda P, t: fold_alias(S, P, t),
            "fold-k1": lambda P, t: fold_k1(S, P, t),
            "outside-window": outside_window,
            "bracket": lambda P, t: bracket(S, P, t),
            "data-slot": lambda P, t: t in eset.get(P, ()) and (P, t) in dslots,
            "registerme": lambda P, t: (P, t) in rm_ok,  # 5.4 T9
            "abs-site": lambda P, t: t in eset.get(P, ()) and any(rd == t and c != t for rd, c in abs_pd.get(P, ()))
            and all(rd == t for rd, c in abs_pd.get(P, ()) if c != t)
            and all(pd == P for pd, c in abs_rd.get(t, ()) if c != t)}
    CLS = tuple(PRED)
    RULED, CAND = CLS[:4], CLS[4:]
    ev = {(c, P, t): PRED[c](P, t) for P, _r, es in A for t in es for c in CLS}

    plants, unpl = [], []  # before counting: each class's first settled pair, fed a wrong body and a wrong proto
    esorted = sorted(entries)
    ppubs = [v for v in pub_vas if v in procs]
    for c in CLS:
        st = sorted((P, t) for (c2, P, t), v in ev.items() if c2 == c and v)
        if not st:
            unpl.append(c)
            continue
        P, t = st[0]
        t2 = next((x for es in erow[P] for x in es if x != t and not ev.get((c, P, x))), None)
        if t2 is None:
            t2 = next(x for x in esorted[bisect.bisect_right(esorted, t):] if x not in rowt[P])
        P2 = next(v for v in ppubs[bisect.bisect_right(ppubs, P):] if pubs[v] != pubs[P])
        plants += [(c, pp, tt, PRED[c](pp, tt)) for pp, tt in ((P, t2), (P2, t))]
    nref = sum(1 for p in plants if not p[3])
    sums.append(f"plants {len(plants)} = refused {nref}")
    if len(plants) != 2 * (len(CLS) - len(unpl)) or nref != len(plants):  # was the literal 8 (4 ruled classes)
        fail.append(sums[-1] + "; DID NOT FAIL: " + ", ".join(f"{c} (0x{pp:08X}, 0x{tt:06X})"
                                                             for c, pp, tt, ok in plants if ok))
        return None

    hb = lambda n: "2-3" if n <= 3 else "4-31" if n < 32 else "32-99" if n < 100 else "100+"  # noqa: E731
    rc, bc, xr = Counter(), Counter(), Counter()
    cnew, both, agr, dis = Counter(), Counter(), Counter(), []
    sub, kk, hh = defaultdict(Counter), defaultdict(Counter), defaultdict(Counter)
    for (P, r, es), h in zip(A, H):
        cls = next((c for c in RULED if any(ev[(c, P, t)] for t in es)), "none")
        rc[cls] += 1
        for c in CLS:
            bc[c] += sum(1 for t in es if ev[(c, P, t)])
        if cls == "caller-xref":
            xr[chk(P, next(t for t in es if ev[(cls, P, t)]))["reason"]] += 1
        k = len(es)
        rt = {t for t in es if cls != "none" and ev[(cls, P, t)]}
        for c in CAND:
            ct = {t for t in es if ev[(c, P, t)]}
            if not ct:
                continue
            rc[c] += 1
            if not rt:
                cnew[c] += 1
                continue
            both[c] += 1
            if rt & ct:
                agr[c] += 1
            else:
                dis.append(f"- disagree {c} vs {cls} 0x{P:08X}: ruled 0x{min(rt):06X}, cand 0x{min(ct):06X} "
                           f"{r['names'][0][:60]}")
        for c in [cls] + [c for c in CAND if any(ev[(c, P, t)] for t in es)]:
            sub[c]["short-inline-copy" if r["length"] < 64 or k == 1 else "multiple-targets" if k >= 2 else "other"] += 1
            kk[c]["0" if k == 0 else "1" if k == 1 else "2+"] += 1
            hh[c][hb(len(h))] += 1
    CN = RULED + ("none",)
    sums.append(f"ambiguous {len(A)} = " + " + ".join(f"{c} {rc[c]}" for c in CN))
    if sum(rc[c] for c in CN) != len(A):
        fail.append(sums[-1])
    allt = sorted({int(v, 16) for _P, r, _es in A for v in r["targetVAs"]})
    dl = {t for t in allt if (u := unit_at(units, ustarts, t)) and u["kind"] == "game" and u["delink"]}
    ne = sum(1 for t in allt if t in entries)

    byva = {r["protoVA"]: r for r in fx}
    nn = sorted(p for p, w in why.items() if w[0] == "no name evidence")
    nx, nrs = Counter(), Counter()
    for p in nn:
        P, c = int(p, 16), int(byva[p]["candidateVA"], 16)
        site = any(rd == c for rd, _cv in ctx["by_pd"].get(P, ()))
        nx["both" if site and c in slot_rv else "site-only" if site else "slot-only" if c in slot_rv else "neither"] += 1
        nx["site conflict"] += any(rd != c for rd, _cv in ctx["by_pd"].get(P, ()))
        nx["slot agreeing"] += P in slot_rv.get(c, ())
        nrs[chk(P, c)["reason"]] += 1
    NX = ("both", "site-only", "slot-only", "neither")
    sums.append(f"near no-name {len(nn)} = " + " + ".join(f"{k} {nx[k]}" for k in NX))
    if sum(nx[k] for k in NX) != len(nn):
        fail.append(sums[-1])

    # yield bound 5.2 T17 (was .run/5.2/t15/delinked.py): rows of the ruled classes, their settled bodies, delinked
    # game units. 5.3 T3: every ruled class (was k1 + D115 only): RULED, then the 5.3 T1 rulings D128-D130 in YC order
    pin = coff_info.pinned_sha() or ""
    gsize = {int(f["entry"], 16): f["size"] for f in candidates.load_jsonl(
        REPO / "derived" / "ghidra" / f"retail-{pin[:8]}" / "applied" / "functions.jsonl")}
    YC = RULED + ("bracket", "fold-alias", "fold-k1")  # was ("k1", "D115")
    yrows, ybodies, yout = Counter(), {}, 0  # ybodies: settled body VA -> class of the first row settling it
    ycls = []  # 5.3 T10.2: per row of A, its first settling ruled class in YC order (the residue reads it)
    for P, r, es in A:
        cls = next((c for c in YC if any(ev[(c, P, t)] for t in es)), "none")
        ycls.append(cls)
        if cls != "none":
            yrows[cls] += 1
            for t in es:
                if ev[(cls, P, t)]:
                    ybodies.setdefault(t, cls)
    yin = {t: u["name"] for t in ybodies
           if (u := unit_at(units, ustarts, t)) and u["kind"] == "game" and u["delink"]}
    for P, r, es in A:
        cls = next((c for c in YC if any(ev[(c, P, t)] for t in es)), "none")
        if cls != "none" and not any(t in yin for t in es if ev[(cls, P, t)]):
            yout += 1
    yfresh = sorted(t for t in yin if t not in funcs)
    ybins = Counter("<=64" if gsize.get(t, 0) <= 64 else "65-128" if gsize.get(t, 0) <= 128 else "129-256"
                    if gsize.get(t, 0) <= 256 else ">256" for t in yfresh)
    ny = sum(yrows.values())
    sums.append(f"yield rows {ny} = " + " + ".join(f"{c} {yrows[c]}" for c in YC))
    if any(yrows[c] != rc[c] for c in RULED) or ny != sum(1 for _P, _r, es in A  # was (k1, D115) == rc
                                                         if any(ev[(c, _P, t)] for c in YC for t in es)):
        fail.append(sums[-1])

    cs = lambda C, ks: ", ".join(f"{k} {C[k]}" for k in ks)  # noqa: E731
    # 5.3 T1 c2: per class (ruled: the row's class; candidate: any settled body) rows and settled bodies by family and
    # row-length bin; settled bodies in a delinked game unit not yet named; identical pool rows naming them would free
    FAM, LB = ("creationFunc", "ctor", "dtor", "Message", "FrameTick", "other"), ("<=64", "65-128", "129-256", ">256")
    lbin = lambda n: "<=64" if n <= 64 else "65-128" if n <= 128 else "129-256" if n <= 256 else ">256"  # noqa: E731
    isdl = lambda t: bool((u := unit_at(units, ustarts, t)) and u["kind"] == "game" and u["delink"])  # noqa: E731
    rowcls = [next((c for c in RULED if any(ev[(c, P, t)] for t in es)), "none") for P, _r, es in A]
    unres = [s for s in ({x["target"] for x in r["sites"] if x["resolved"] is None} for r in irows) if s]
    PL, nbad = [], []
    for c in CLS:
        frow, brow, cb = Counter(), Counter(), {}  # cb: settled body -> (family, bin) of the first row settling it
        for (P, r, es), rcl in zip(A, rowcls):
            st = [t for t in es if ev[(c, P, t)]]
            if not st or (c in RULED and rcl != c):
                continue
            fm, lb = family(r["names"][0]), lbin(r["length"])
            frow[fm] += 1
            brow[lb] += 1
            for t in st:
                cb.setdefault(t, (fm, lb))
        if sum(frow.values()) != rc[c]:
            nbad.append(c)
        ub = sorted(t for t in cb if t not in funcs and isdl(t))
        U = set(ub)
        ubins = Counter(lbin(gsize.get(t, 0)) for t in ub)
        PL += [f"- {c}: rows {sum(frow.values())} by family {cs(frow, FAM)}; by length {cs(brow, LB)} | bodies "
               f"{len(cb)} by family {cs(Counter(f for f, _b in cb.values()), FAM)}; by length "
               f"{cs(Counter(b for _f, b in cb.values()), LB)}",
               f"- {c}: settled bodies in a delinked game unit, not yet named {len(ub)} (bytes "
               f"{sum(gsize.get(t, 0) for t in ub)}; bins {cs(ubins, LB)}; units "
               f"{len({unit_at(units, ustarts, t)['name'] for t in ub})}); identical pool rows freed fully "
               f"{sum(1 for s in unres if s <= U)}, touched {sum(1 for s in unres if s & U)}"]
    # 5.3 T1 c3 candidate-only supply: rows no ruled class settles but a candidate does; bodies counted once
    con, cob = 0, defaultdict(set)  # cob: settled body -> candidate classes settling it
    for (P, _r, es), rcl in zip(A, rowcls):
        if rcl != "none":
            continue
        hit = False
        for c in CAND:
            for t in es:
                if ev[(c, P, t)]:
                    cob[t].add(c)
                    hit = True
        con += hit
    cod = sorted(t for t in cob if isdl(t))
    cou = [t for t in cod if t not in funcs]
    PL.append(f"- candidate-only supply: rows settled by no ruled class but by a candidate {con}; their settled bodies "
              f"distinct {len(cob)}, in a delinked game unit {len(cod)}, not yet named {len(cou)} (bytes "
              f"{sum(gsize.get(t, 0) for t in cou)}; bins {cs(Counter(lbin(gsize.get(t, 0)) for t in cou), LB)}; units "
              f"{len({unit_at(units, ustarts, t)['name'] for t in cou})}); by class "
              + ", ".join(f"{c} {sum(1 for t in cou if c in cob[t])}" for c in CAND))
    sums.append(f"class breakdown rows == class rows: {len(CLS) - len(nbad)}/{len(CLS)}")
    if nbad:
        fail.append(sums[-1] + " (" + ", ".join(nbad) + ")")

    # creationFunc reach: caller site, vtable slot, data pointer (4-aligned dword == t in a retail data section: no
    # SEC_EXEC flag, plus .rdata (flags 0x6) and .data (0x7), which carry the XBE exec bit 0x4 in this image)
    cfr = sum(1 for _P, r, _es in A if family(r["names"][0]) == "creationFunc")
    cfb = sorted({t for _P, r, es in A if family(r["names"][0]) == "creationFunc" for t in es})
    dsecs = [(s["virtualAddr"], img.xb[s["rawAddr"]:s["rawAddr"] + s["rawSize"]]) for s in img.x["sections"]
             if not s["flags"] & proto_map.SEC_EXEC or s.get("name") in (".rdata", ".data")]
    dext = []  # accepted data lines of both tables (statuses as the vtable rule), size `-` = 1 byte
    for tab, ok in ((candidates.SYMS, units_seed.STATUSES), (candidates.SYMS_PIN, candidates.PIN_STATUSES)):
        for ln in candidates.need(tab).read_text(encoding="utf-8").splitlines():
            f = ln.split()
            if len(f) < 6 or ln.startswith("#") or f[1] != "data" or f[-1] not in ok \
                    or f[-1] in ("site-corr", "boundary", "near-corr"):
                continue
            dext.append((int(f[0], 16), int(f[0], 16) + (int(f[2], 16) if f[2] != "-" else 1)))
    dext.sort()
    dlo = [a for a, _b in dext]
    # 5.3 T1 c3 code immediate: any-offset dword == t in a retail exec section (not .rdata/.data, see above); the hit's
    # containing func (last symbols func at or before it, hit < va + size when sized) accepted or not
    csecs = [(s["virtualAddr"], img.xb[s["rawAddr"]:s["rawAddr"] + s["rawSize"]]) for s in img.x["sections"]
             if s["flags"] & proto_map.SEC_EXEC and s.get("name") not in (".rdata", ".data")]
    fok = (set(candidates.PIN_STATUSES) | set(units_seed.STATUSES)) - {"site-corr", "boundary", "near-corr"}
    fvas = sorted(funcs)
    cfx = Counter()
    for t in cfb:
        pk = struct.pack("<I", t)
        for va0, b in csecs:
            i = b.find(pk)
            while i >= 0:
                j = bisect.bisect_right(fvas, va0 + i) - 1
                fn = funcs[fvas[j]] if j >= 0 else None
                acc = (fn is not None and fn["status"] in fok
                       and (fn["size"] is None or va0 + i < fvas[j] + fn["size"]))
                cfx["immh"] += 1
                cfx["imm_a" if acc else "imm_b"] += 1
                i = b.find(pk, i + 1)
        cfx["imm"] += any(pk in b for _va0, b in csecs)
        pk, ps = struct.pack("<I", t), []
        for va0, b in dsecs:
            i = b.find(pk)
            while i >= 0:
                if (va0 + i) % 4 == 0:
                    ps.append(va0 + i)
                i = b.find(pk, i + 1)
        site, slot = t in ctx["by_rd"], t in slot_rv
        cfx["site"] += site
        cfx["slot"] += slot
        cfx["data"] += bool(ps)
        cfx["ptrs"] += len(ps)
        cfx["inext"] += sum(1 for v in ps if (j := bisect.bisect_right(dlo, v) - 1) >= 0 and v < dext[j][1])
        cfx["none"] += not (site or slot or ps)
    PL.append(f"- creationFunc reach (rows {cfr}, bodies {len(cfb)}, all classes): caller site {cfx['site']}, vtable "
              f"slot {cfx['slot']}, data pointer {cfx['data']} (pointers {cfx['ptrs']}, inside an accepted data line's "
              f"extent {cfx['inext']}: data-slot's reach); code immediate {cfx['imm']} (immediates {cfx['immh']}: in an "
              f"accepted caller {cfx['imm_a']}, in an unnamed or unaccepted function {cfx['imm_b']}); none "
              f"{cfx['none']} (without code immediates)")
    # 5.4 T9 registerme / abs-site chain / creationFunc chain
    rmr = Counter(rm_row[P] for P, _r, _es in A if P in rm_x)
    rmb = Counter(t for _P, t in rm_ok)
    rm2 = sorted(t for t, n in rmb.items() if n >= 2)
    PL.append(f"- registerme: rows {sum(rmr.values())}; settled {rmr['S']} (literal text == class name), literal bytes "
              f"equal but text differs {rmr['D']}" + (" (" + ", ".join(f"0x{P:08X} {X} {txt!r}" for P, X, txt in rm_d)
                                                     + ")" if rm_d else "")
              + f", equal literal but img.match refused {rmr['M']}, no literal match {rmr['N']}; bodies settled by 2+ "
              f"rows {len(rm2)}" + (" (" + ", ".join(f"0x{t:06X}" for t in rm2) + ")" if rm2 else ""))
    srcn = Counter(abs_src.values())
    via = Counter()
    for P, _r, es in A:
        sv = {abs_src[(P, rd, c)] for t in es if ev[("abs-site", P, t)] for rd, c in abs_pd.get(P, ()) if rd == t and c != t}
        for s in sv:
            via[s] += 1
    PL.append(f"- abs-site callers: accepted {srcn['accepted']} sites, chain {srcn['chain']} sites; rows settled via "
              f"accepted {via['accepted']}, via chain {via['chain']}")
    cch = Counter()
    cpairs, cmiss = [], []  # 5.4 T9 c2: chained (P, rd) of classes e/h; rows (P, r, rd) of class x
    for (P, r, es), h in zip(A, H):
        if family(r["names"][0]) != "creationFunc":
            continue
        rds = {rd for rd, c in abs_pd.get(P, ()) if abs_src[(P, rd, c)] == "chain"}
        if not rds:
            cch["u"] += 1
        elif len(rds) > 1 or any(pd != P for rd in rds for pd, c in abs_rd[rd] if abs_src[(pd, rd, c)] == "chain"):
            cch["f"] += 1
        else:
            rd = next(iter(rds))
            kc = "e" if rd in es and rd in entries else "g" if rd in gaps else "h" if rd in h else "x"
            cch[kc] += 1
            if kc in ("e", "h"):
                cpairs.append((P, rd))
            elif kc == "x":
                cmiss.append((P, r, rd))
    sums.append(f"creationFunc chain: rows {cfr} = ctor unsettled {cch['u']} + push rd an entry (settles) {cch['e']} "
                f"+ rd a gap {cch['g']} + rd a non-entry hit of the row (R183 candidate) {cch['h']} + rd not a hit "
                f"{cch['x']} + refused by conflict (t pushed from 2+ ctors with different protos, retail fold) {cch['f']}")
    PL.append("- " + sums[-1])
    if sum(cch.values()) != cfr:
        fail.append(sums[-1])
    pl_t9 = len(PL)  # 5.4 T9 c2 lines go here, built after the edge loop

    # k = 0 rows (R183): a hit has start evidence if a caller site, a vtable slot or a pin func line reaches it
    def sev(t):
        return (t in ctx["by_rd"], t in slot_rv, t in funcs and funcs[t]["status"] in candidates.PIN_STATUSES)
    k0, k0e, k0x = 0, 0, Counter()
    r183 = {}
    for (P, r, es), h in zip(A, H):
        if es:
            continue
        k0 += 1
        ev_ = [sev(t) for t in h]
        r183[P] = sum(1 for e in ev_ if any(e))
        k0e += r183[P] > 0
        k0x["a"] += sum(e[0] for e in ev_)
        k0x["b"] += sum(e[1] for e in ev_)
        k0x["c"] += sum(e[2] for e in ev_)
        k0x["H"] += r183[P]
    sums.append(f"k0 rows {k0} = start evidence {k0e} + none {k0 - k0e}")
    if k0 != kn["0"] + sum(1 for P, r, es in A if not es and r["hits"] < proto_map.HIT_CAP) or k0 < k0e:
        fail.append(sums[-1])
    PL.append(f"- k0 rows {k0} = start evidence {k0e} (sites {k0x['a']}, slots {k0x['b']}, pins {k0x['c']}; hits with "
              f"evidence {k0x['H']}) + none {k0 - k0e} (inlined into callers)")

    # edge proposal: kept game units (delink false) whose delink_reason names an M27 edge row of class ambiguous
    EDGE = re.compile(r"M27 (start|end): ([^;]+?) proto (0x[0-9A-Fa-f]+) class ambiguous")  # one note: no `;`
    arow = {P: (r, es, rcl) for (P, r, es), rcl in zip(A, rowcls)}
    idt = sorted(v for r in fx if r["class"] == "identical" and len(r["targetVAs"]) == 1
                 and (v := int(r["targetVAs"][0], 16)) not in matched)
    eP, EL, ecnt, eu, efree = defaultdict(list), [], Counter(), 0, 0
    esett = set()  # 5.4 T7: (unit name, index) with >= 1 settled edge (ruled or candidates only)
    for u in units:
        eds = EDGE.findall(u.get("delink_reason", "")) if u["kind"] == "game" and not u["delink"] else []
        if not eds:
            continue
        eu += 1
        tag = u["name"].split("\\")[-1] + f"#{u['index']}"
        parts, sett = [], False
        for side, nm, pv in eds:
            P = int(pv, 16)
            eP[P].append(f"{tag} {side}")
            r, es, rcl = arow.get(P, (None, [], "none"))
            cl = [rcl] if rcl != "none" else [c for c in CAND if any(ev[(c, P, t)] for t in es)]
            kind = "ruled" if rcl != "none" else "candidates only" if cl else "none"
            ecnt[kind] += 1
            sett |= bool(cl)
            body = f" 0x{min(t for t in es if ev[(cl[0], P, t)]):06X}" if cl else ""
            parts.append(f"{side} 0x{P:08X} {nm[:60]}" + ("" if r else " (not an ambiguous row)")
                         + f" -> {'/'.join(cl) or 'none'}{body}")
        nfree = bisect.bisect_left(idt, u["text_end"]) - bisect.bisect_left(idt, u["text_start"])
        efree += nfree if sett else 0
        if sett:
            esett.add((u["name"], u["index"]))
        EL.append(f"- {tag} (0x{u['text_start']:06X}-0x{u['text_end']:06X}): " + "; ".join(parts)
                  + f"; identical rows a cut would free {nfree}")
    ne_ = sum(ecnt.values())

    # 5.4 T9 c2 chain sites: rel masks of every chained creationFunc pair (img.match) -> sites (pd, rd2)
    csites, cref = set(), 0
    for P, rd in cpairs:
        n = rlen[P]
        m = img.match(P, rd, n)
        if m is None:
            cref += 1
            continue
        pb, rb = img.pbytes(P, n), img.rbytes(rd, n)
        for o, _w, k in m[1]:
            if k != "rel":
                continue
            pd = name_sites.target(pb, P, o, k)
            if not P <= pd < P + n:
                csites.add((pd, name_sites.target(rb, rd, o, k)))
    cs_pd = defaultdict(set)
    for pd, rd2 in csites:
        cs_pd[pd].add(rd2)
    agree = lambda P: sorted(t for t in cs_pd.get(P, ()) if t in eset.get(P, ()))  # noqa: E731
    cgain, cconf, d115b = Counter(), [], {}  # d115b: D115 row P -> its D115-settled bodies
    for (P, _r, es), cl in zip(A, ycls):
        if agree(P):
            cgain[cl if cl in ("D115", "none") else "other"] += 1
            if cl not in ("D115", "none"):
                cgain["o:" + cl] += 1
        if cl == "D115":
            d115b[P] = {t for t in es if ev[("D115", P, t)]}
            cconf += [(P, t) for t in sorted(cs_pd.get(P, ())) if t not in d115b[P]]
    ngain = len({P for P, _r, _es in A} & {pd for pd, rd2 in csites if rd2 in eset.get(pd, ())})
    nng = sum(1 for p in nn if int(byva[p]["candidateVA"], 16) in cs_pd.get(int(p, 16), ()))
    ed115, eend = [], defaultdict(set)  # edge protos of a D115 row (the edge ctors); end-edge protos per unit
    for u in units:
        for side, _nm, pv in (EDGE.findall(u.get("delink_reason", "")) if u["kind"] == "game" and not u["delink"]
                              else ()):
            P = int(pv, 16)
            if side == "end":
                eend[(u["name"], u["index"])].add(P)
            if P in d115b and P not in ed115:
                ed115.append(P)
    eag = [f"0x{P:08X} -> 0x{t:06X}" for P in ed115 for t in sorted(cs_pd.get(P, ())) if t in d115b[P]]
    exs = [f"0x{P:08X} -> 0x{t:06X}" for P in ed115 for t in sorted(cs_pd.get(P, ())) if t not in d115b[P]]
    sums.append(f"chain sites: ambiguous rows gaining an agreeing site {ngain} = D115 {cgain['D115']} + none "
                f"{cgain['none']} + other classes {cgain['other']}")
    if ngain != cgain["D115"] + cgain["none"] + cgain["other"]:
        fail.append(sums[-1])
    oc = ", ".join(f"{c} {cgain['o:' + c]}" for c in YC if cgain["o:" + c])
    T9L = [f"- chain sites: pairs {len(cpairs)} (img.match refused {cref}), rel sites {len(csites)}; " + sums[-1][13:]
           + (f" ({oc})" if oc else "") + f"; near no-name rows gaining one {nng} of {len(nn)}; D115 edge ctor protos "
           f"({', '.join(f'0x{P:08X}' for P in ed115) or 'none'}) gaining an agreeing site: {', '.join(eag) or 'none'}"
           + f", conflicting: {', '.join(exs) or 'none'}; conflicts (pd == P of a D115 row, rd2 != its "
           f"D115 body) {len(cconf)}" + (" (" + ", ".join(f"0x{P:08X} -> 0x{t:06X}" for P, t in cconf[:10]) + ")"
                                        if cconf else "")]
    # 5.4 T9 c2 registerme bodies by placement
    rm_t = defaultdict(set)
    for P, t in rm_ok:
        rm_t[t].add(P)
    rbp, eall = Counter(), set().union(*eend.values())
    for t in rm_t:
        u = unit_at(units, ustarts, t)
        if u is None or u["kind"] != "game":
            rbp["o"] += 1
            rbp["eo"] += bool(rm_t[t] & eall)
        elif u["delink"]:
            rbp["n" if t in funcs else "u"] += 1
        else:
            rbp["k"] += 1
            rbp["e"] += bool(rm_t[t] & eend.get((u["name"], u["index"]), set()))
    sums.append(f"registerme bodies {len(rm_t)} = delinked game unit named {rbp['n']} + delinked unnamed {rbp['u']} "
                f"+ kept game unit {rbp['k']} + outside every game unit {rbp['o']}")
    if len(rm_t) != rbp["n"] + rbp["u"] + rbp["k"] + rbp["o"]:
        fail.append(sums[-1])
    T9L.append(f"- registerme bodies: {sums[-1][18:]} (M27 end-edge procs: in their kept unit {rbp['e']}, "
               f"outside every game unit {rbp['eo']})")
    # 5.4 T9 c2 registerme misses: no literal match rows (proto abs dests) and creationFunc chain rd not a hit
    ml = []
    for P, r, _es in A:
        if P not in rm_x or rm_row.get(P) != "N":
            continue
        pb = img.pbytes(P, rlen[P])
        ds = [f"0x{d:08X} " + ("??_C@" if any(x.startswith("??_C@") for x in pubs.get(d, ())) else "not ??_C@")
              for o, _w, k in (proto_map.mask_proc(pb, P, img.plo, img.phi)[0] if pb is not None else ())
              if k == "abs" and (d := name_sites.target(pb, P, o, k)) is not None]
        ml.append(f"0x{P:08X} {r['names'][0][:60]} (abs dests: {', '.join(ds) or 'none'})")
    xl = []
    for P, r, rd in cmiss:
        j = bisect.bisect_right(fvas, rd) - 1
        xl.append(f"0x{P:08X} {r['names'][0][:60]} rd 0x{rd:06X} (nearest func at or below: "
                  + (f"0x{fvas[j]:06X} {funcs[fvas[j]]['name'][:60]}" if j >= 0 else "none") + ")")
    T9L.append(f"- registerme misses: no literal match {len(ml)}: {'; '.join(ml) or 'none'} | creationFunc chain rd "
               f"not a hit {len(xl)}: {'; '.join(xl) or 'none'}")
    PL[pl_t9:pl_t9] = T9L

    # residue: rows no ruled class settles, one line each (ambiguous_residue.md). 5.3 T10.2: every ruled class in YC
    # (D128-D130 included), was RULED only (rowcls), so bracket and fold-k1 settled rows sat in the residue
    for (P, r, es), h, rcl in zip(A, H, ycls):
        if rcl != "none":
            continue
        k = len(es)
        k1 = "k0" if k == 0 else "k2+" if k >= 2 else f"shared {claims[es[0]]}"
        d115 = ("no bodies" if not es else f"{chk(P, es[0])['reason']} (name step)" if not name_ok(P)
                else cs(Counter(chk(P, t)["reason"] for t in es), sorted({chk(P, t)["reason"] for t in es})))
        xs = "conflict" if any(rd in set(es) for rd, _c in ctx["by_pd"].get(P, ())) else "no-site"
        wd = [c for c in CAND if any(ev[(c, P, t)] for t in es)]
        res.append(f"- 0x{P:08X} {r['names'][0][:60]} len {r['length']}, k {k}, hits {len(h)} | k1: {k1} | D115: "
                   f"{d115} | D113: no slot pair | caller-xref: {xs} | would: {', '.join(wd) or 'none'} | R183: "
                   f"{r183[P] if P in r183 else '-'} | edge: {', '.join(eP.get(P, ())) or '-'}")
    nres = sum(1 for c in ycls if c == "none")
    sums.append(f"residue rows {len(res)} = rows no ruled class settles {nres}")  # was = none {rc['none']}
    if len(res) != nres:
        fail.append(sums[-1])
    sums.append(f"ambiguous {len(A)} = assigned (yield rows) {ny} + residue {len(res)}")  # 5.3 T10.2
    if len(A) != ny + len(res):
        fail.append(sums[-1])
    allh = sorted({t for h in H for t in h})
    dlh = sum(1 for t in allh if isdl(t))
    neh = sum(1 for t in allh if t in entries)
    L = ["## Lane sizing", "",
         f"5.1 T9, sizing only. Ambiguous rows ({len(A)}): bodies = recounted hits (5.3 T1: proto_map's matcher, no "
         "hit cap) that are Ghidra entries or gap functions; a row takes "
         "the first ruled class settling any body: k1 (one entry body, no other ambiguous row has it), D115 (near_check "
         "pass), D113 (an accepted vtable slot pair, "
         f"{nvt} vtables, {len(slots)} slot pairs), caller-xref (a caller site P -> t, no other proto dest at t), "
         "none. Bodies per class overlap.", "",
         f"- recount: rows under cap {len(under)} agree {len(under) - len(bad)} (hits set == functions.jsonl "
         "targetVAs)",
         f"- recount: capped rows {len(capd)}, k changed {kch}, k now 0/1/2+ {kn['0']}/{kn['1']}/{kn['2+']}, bodies "
         f"distinct {len(allb)} (entries {nbe}, gap {len(allb) - nbe})", "",
         "Classes, ruled first, then candidates (candidate, no writer; each needs t among the row's bodies): "
         "fold-alias (P has >= 2 publics, caller xref agree >= 1 and conflicts 0, img.match), fold-k1 (the row's one "
         "body, every row claiming it has only that body), outside-window (near_check outside-window, one public, "
         f"similarity >= {proto_map.THRESHOLD} to the section end, xref agree >= 1, conflicts 0), bracket (the row's one "
         "unit, the only body between its matched neighbours' retail ends), data-slot (a dword pair of an accepted "
         f"non-vtable data line, {ndl} lines, {len(dslots)} pairs), registerme (5.4 T9: a `??0RegisterMe@X@@QAE@XZ` "
         "row and body push one string literal at the same mask offset, equal bytes, text == X, img.match), abs-site "
         "(an `abs` immediate of an accepted caller or, 5.4 T9, a registerme-settled pair as chain caller, "
         f"P -> t, agree >= 1 and conflicts 0; {sum(map(len, abs_pd.values()))} sites). Candidate rows count apart "
         "from the ruled rows.", ""]
    for c in CN + CAND:
        L.append(f"- {c}" + (" (candidate, no writer)" if c in CAND else "") + f": rows {rc[c]}"
                 + (f", not settled by a ruled class {cnew[c]}" if c in CAND else "")
                 + (f", bodies {bc[c]}" if c in bc else "")
                 + f"; sub-kind {cs(sub[c], ('short-inline-copy', 'multiple-targets', 'other'))}"
                 + f"; k {cs(kk[c], ('0', '1', '2+'))}; hits {cs(hh[c], ('2-3', '4-31'))}, 32+ "
                 + f"{hh[c]['32-99'] + hh[c]['100+']} ({cs(hh[c], ('32-99', '100+'))})")
    L += ["", "Agreement (rows a candidate and a ruled class both settle; agree = a common settled body):", ""]
    L += [f"- {c}: rows both {both[c]}, agree {agr[c]}, disagree {both[c] - agr[c]}" for c in CAND] + dis + [""]
    L += [f"- caller-xref rows by near_check reason: {cs(xr, sorted(xr))}",
          f"- distinct retail bodies (all uncapped hits; capped targetVAs in parentheses): {len(allh)} ({len(allt)}); "
          f"entry {neh} ({ne}), non-entry {len(allh) - neh} ({len(allt) - ne}); in a delinked game unit {dlh} "
          f"({len(dl)}), not {len(allh) - dlh} ({len(allt) - len(dl)}); named in symbols "
          f"{sum(1 for t in allh if t in funcs)} ({sum(1 for t in allt if t in funcs)})",
          f"- yield bound, every ruled class ({', '.join(YC)}; a row takes the first settling class): settled rows "
          f"{ny} ({cs(yrows, YC)}); settled bodies distinct {len(ybodies)}; in a delinked "
          f"game unit {len(yin)} (not {len(ybodies) - len(yin)}); not yet named {len(yfresh)} (bytes "
          f"{sum(gsize.get(t, 0) for t in yfresh)}; bins {cs(ybins, ('<=64', '65-128', '129-256', '>256'))}; units "
          f"{len({yin[t] for t in yfresh})}); settled rows with every settled body outside a delinked game unit {yout}",
          "", "Per class (5.3 T1; ruled: rows of that class, candidate: rows it settles; family from names[0], length "
          "bins by row length, bodies binned by their first row; unnamed bodies binned by Ghidra size; freed = "
          "unmatched identical pool rows whose unresolved site targets are all (fully) / any (touched) of them):", ""]
    L += PL + ["", f"Edge proposal (no cut): kept game units whose delink_reason has an M27 edge of class ambiguous: "
               f"units {eu}, edges {ne_}; settled by ruled {ecnt['ruled']}, by candidates only "
               f"{ecnt['candidates only']}, none {ecnt['none']}; identical rows freed if every settled edge were cut "
               f"(units with >= 1 settled edge) {efree}", ""] + EL
    L += ["", f"Residue: {len(res)} rows no ruled class settles, one line each in `ambiguous_residue.md`.",
          "", f"Near no-name rows ({len(nn)}), pair (protoVA, candidateVA): site = an accepted caller reaches it, "
          "slot = an accepted vtable slot holds it.", "",
          f"- {cs(nx, NX)}; site conflicts {nx['site conflict']}; slot with agreeing protoVA {nx['slot agreeing']}",
          f"- near_check reasons: {cs(nrs, sorted(nrs))}", "", "Plants (in memory, before counting):", ""]
    L += [f"- {c} (0x{pp:08X}, 0x{tt:06X}): refused" for c, pp, tt, _ok in plants]
    L += [f"- {c}: no settled pair, not planted" for c in unpl]
    if keep is not None:  # 5.4 T7
        keep.update(S=S, A=A, ev=ev, YC=YC, ycls=ycls, esett=esett, slot_rv=slot_rv, abs_all=abs_all, funcs=funcs,
                    units=units, ustarts=ustarts)
    return L + [""]


def near_sizing(fx, why, sums, fail, K, cand, unm_n, out_n, out, matched, led, objs, pin, used):
    """5.4 T7: size the near lane and the near-identical rows outside it by the routes that exist today; sizing only,
    nothing written. K = lane_sizing's keep; cand = every candidates.jsonl row; out = the identical lane's buckets.
    Parts (a)-(g), each with a checked sum. Returns the `## Near sizing (5.4 T7)` doc lines. used collects the
    NOTES/NOTES_VA keys (d) hits (5.4 T12.1.c7: ambiguous ??_G rows are guarded there)."""
    import name_sites
    import units_seed
    S, A, ev, YC, funcs, units, ustarts = (K[k] for k in ("S", "A", "ev", "YC", "funcs", "units", "ustarts"))
    pubs, ctx = S.pubs, S.ctx
    cs = lambda C, ks: ", ".join(f"{k} {C[k]}" for k in ks if C[k])  # noqa: E731
    short = lambda u: u.split("\\")[-1]  # noqa: E731
    BINS = ("<=32", "33-64", "65-128", "129-512", ">512")
    sbin = lambda n: ("<=32" if n <= 32 else "33-64" if n <= 64 else "65-128" if n <= 128  # noqa: E731
                      else "129-512" if n <= 512 else ">512")
    BK = [k for k, _v, _n in BUCKETS]
    RB = ("stdsig-reloc", "reloc-provisional", "reloc-code", "reloc-data")
    bk = {id(r): k for o in (out, out_n) for k, rs in o.items() for r in rs}  # ids: near and identical rows differ
    byva = {r["protoVA"]: r for r in fx}
    pin8 = (coff_info.pinned_sha() or "")[:8]
    gf = {int(g["entry"], 16): (g["size"], int(g["bodyMax"], 16)) for g in candidates.load_jsonl(
        REPO / "derived" / "ghidra" / f"retail-{pin8}" / "applied" / "functions.jsonl")}
    acc_st = set(units_seed.STATUSES) | set(candidates.PIN_STATUSES)

    def tries(r):  # match_one.is_attempt runs on the row (assign's dedupe)
        return sum(1 for x in {id(x): x for x in led[r["va"]] + led[r["symbol"]]}.values() if match_one.is_attempt(x))

    def check(s, ok):
        sums.append(s)
        if not ok:
            fail.append(s)

    L = ["## Near sizing (5.4 T7)", "",
         "Sizing only: no pin, lane, units.toml or candidates change. Every figure from this run's state (near lane "
         "buckets, the outside reasons, lane_sizing's ruled classes and edge proposal, units.toml, the ledger).", ""]

    # (a) near lane unmatched by bucket x size bin and by unit
    byu = defaultdict(list)
    for r in unm_n:
        byu[r["unit"]].append(r)
    nb_ = sum(len(v) for v in out_n.values())
    check(f"near sizing (a) unmatched near {len(unm_n)} = by unit {sum(map(len, byu.values()))} = by bucket {nb_}",
          len(unm_n) == sum(map(len, byu.values())) == nb_)
    L += [f"### (a) Near lane unmatched: {len(unm_n)}", "", "By bucket x size bin (row size, bytes):", ""]
    L += [f"- {k} {len(out_n[k])}: " + cs(Counter(sbin(r["size"]) for r in out_n[k]), BINS) for k in BK if out_n.get(k)]
    L += ["", f"By unit ({len(byu)} units, count desc): count; buckets; bins", ""]
    for u, rs in sorted(byu.items(), key=lambda kv: (-len(kv[1]), kv[0])):
        L.append(f"- {short(u)} {len(rs)}; " + cs(Counter(bk[id(r)] for r in rs), BK) + "; "
                 + cs(Counter(sbin(r["size"]) for r in rs), BINS))

    # (b) name not admitted, size `-`: the route that would size each pin line
    nna = sum(1 for w in why.values() if w[0] == "name not admitted")
    nb = sorted(p for p, w in why.items() if w == ("name not admitted", "size -"))
    tstarts = sorted({va for va, *_ in name_sites.load_table()})
    brt, BL, bnd = Counter(), [], Counter()
    for p in nb:
        va = int(byva[p]["candidateVA"], 16)
        f, u, g = funcs[va], unit_at(units, ustarts, va), gf.get(va)
        j = bisect.bisect_right(tstarts, va)
        ins = [tstarts[j]] if g and j < len(tstarts) and tstarts[j] < va + g[0] else []
        gw = ("no Ghidra entry at va" if g is None else "non-contiguous" if g[0] != g[1] - va + 1
              else f"table start inside (0x{ins[0]:06X})" if ins else None)
        if gw is None:
            rt, d = "ghidra-extent", f"size 0x{g[0]:X}"
        else:
            ext, cf = None, byte_compare.cut_dir(pin) / u["name"].replace("\\", "__").replace("/", "__")
            if cf.name not in objs and cf.is_file():
                objs[cf.name] = byte_compare.load_obj(cf)[1]
            c = objs.get(cf.name)
            fs = [y for y in c["symbols"].values() if byte_compare.is_func(y) and y["name"] == f["name"]] if c else []
            if fs:
                _s, lo, hi = byte_compare.extent(c, fs[0])
                ext = hi - lo
            rt, d = ("cut-extent", f"size 0x{ext:X}; Ghidra: {gw}") if ext else \
                ("none", gw + ("; no cut object" if c is None else "; no cut func symbol"))
        brt[rt] += 1
        bnd[("boundary pin" if candidates.BOUNDARY.match(f["name"]) else "named", rt)] += 1
        BL.append(f"- {p} 0x{va:06X} {f['name'][:70]} [{f['status']}] {short(u['name'])}: {rt} ({d})")
    check(f"near sizing (b) name not admitted size - {len(nb)} = " + " + ".join(
        f"{k} {brt[k]}" for k in ("ghidra-extent", "cut-extent", "none")),
          sum(brt.values()) == len(nb) and len(nb) <= nna)
    L += ["", f"### (b) Name not admitted, size `-`: {len(nb)} (reason total {nna})", "",
          "Route that would size the pin line: ghidra-extent (corr_near's extent gate: a Ghidra entry at va, contiguous "
          "size == bodyMax - va + 1, no symbols table start inside), cut-extent (only the cut func symbol's extent), "
          "none (why). Line: protoVA candidateVA name unit: route.", "",
          f"- by route: {cs(brt, ('ghidra-extent', 'cut-extent', 'none'))}",
          "- by line name (boundary pin = a cut-boundary label `boundary_<VA>`, candidates.BOUNDARY, not name "
          "evidence: sizing it admits no row name): " + ", ".join(f"{a} {b} {n}" for (a, b), n in sorted(bnd.items())),
          ""] + BL

    # (c) reloc rows: each unresolved (else provisional) target by the naming route that could reach it
    rr = [r for k in RB for r in out_n.get(k, [])]
    amb_t, near_c = defaultdict(list), defaultdict(list)
    for P, _r, es in A:
        for t in es:
            amb_t[t].append(P)
    for r in fx:
        if r["class"] == "near-identical" and r.get("candidateVA"):
            near_c[int(r["candidateVA"], 16)].append(int(r["protoVA"], 16))
    drecs = []

    def code_route(t):
        for P in amb_t.get(t, ()):
            c = next((c for c in YC if ev.get((c, P, t))), None)
            if c:
                return c
        if any(near_chk(S, P, t)["verdict"] == "pass" for P in near_c.get(t, ())):
            return "near-corr"
        if K["slot_rv"].get(t):
            return "D113"
        if any(cls_caller_xref(S, pd, t) for pd, _c in ctx["by_rd"].get(t, ())):
            return "caller-xref"
        return "none: " + ("near_check " + near_chk(S, near_c[t][0], t)["reason"] if near_c.get(t)
                           else "ambiguous body, no ruled class" if t in amb_t else "no proto row reaches it")

    def data_route(t):
        pds = {pd for pd, c in K["abs_all"].get(t, ()) if c != t}
        if len(pds) != 1:
            return "none: " + ("no accepted abs site" if not pds else "abs site conflict")
        pd = next(iter(pds))
        ns = pubs.get(pd, ())
        if len(ns) != 1:
            return "none: " + ("no-public" if not ns else "multi-public")
        n = next(iter(ns))
        if n.startswith("??_7"):
            return "vtable-D113"
        if n.startswith(("??_C@", "__real@")):
            return "literal"
        if not drecs:
            drecs.append(name_sites.data_records(S.img))
        sz = name_sites.rec_size(drecs[0], pd, n)
        return "gdata-size" if isinstance(sz, int) else f"none: {sz}"

    troute, tkind, rfree = {}, {}, Counter()
    for r in rr:
        un = [s for s in r["sites"] if s["resolved"] is None]
        ts = un or [s for s in r["sites"] if not candidates.accepted(s["resolved"], s["target"], funcs)]
        for s in ts:
            t = s["target"]
            if t in troute:
                continue
            tkind[t] = "code" if s["kind"] == "rel" or t in gf else "data"
            troute[t] = ("pinned-provisional" if s["resolved"] is not None
                         else code_route(t) if tkind[t] == "code" else data_route(t))
        rfree["freed" if ts and all(not troute[s["target"]].startswith("none") for s in ts) else "blocked"] += 1
    rc_ = Counter(v.split(":")[0] for v in troute.values())
    RTS = ("pinned-provisional",) + tuple(YC) + ("near-corr", "vtable-D113", "literal", "gdata-size", "none")
    nsub = Counter(v[6:] for v in troute.values() if v.startswith("none"))
    check(f"near sizing (c) reloc rows {len(rr)} = freed if every reachable target named {rfree['freed']} + blocked "
          f"{rfree['blocked']}", rfree["freed"] + rfree["blocked"] == len(rr) and sum(rc_.values()) == len(troute))
    L += ["", f"### (c) Near reloc rows: {len(rr)} ({cs(Counter(bk[id(r)] for r in rr), RB)})", "",
          "Targets: a row's unresolved site targets, else its provisional ones (status not accepted). Code (rel, or a "
          "Ghidra entry): pinned-provisional, the ruled classes via an ambiguous row whose body it is (" + ", ".join(YC)
          + "), near-corr (near_check pass of a near-identical row whose candidateVA it is), D113 (an accepted vtable "
          "slot holds it), caller-xref (an accepted caller site, no other proto dest), else none. Data: pinned-"
          "provisional, else the one proto dest of the accepted callers' abs sites at it, its one PDB public: "
          "vtable-D113 (??_7), literal (??_C@/__real@), gdata-size (pdb_types size of its S_GDATA32 record, name_sites "
          "--promote-data step b), else none.", "",
          f"- targets distinct {len(troute)} (code {sum(1 for k in tkind.values() if k == 'code')}, data "
          f"{sum(1 for k in tkind.values() if k == 'data')}); by route: {cs(rc_, RTS)}",
          f"- none by why: {cs(nsub, sorted(nsub))}",
          f"- rows freed if every reachable target were named {rfree['freed']}, blocked {rfree['blocked']} "
          "(stdsig-reloc rows also need the std route)"]

    # (d) ??_G rows and their class ctors
    G_RE = re.compile(r"^\?\?_G(.+)@@[A-Z][A-Z]EPAXI@Z$")
    gall = [r for r in cand if r.get("lane") == "ambiguous" and r["symbol"].startswith("??_G")]
    gpass = [r for r in gall if r["pass"]]
    gr = sorted((r for r in gpass if (r["symbol"], r["va"]) not in matched), key=lambda r: r["va"])
    msym = {s for s, _v in matched}
    crow = defaultdict(list)  # candidates rows of ctors by class prefix lookup
    for r in cand:
        if r["symbol"].startswith("??0"):
            crow[r["symbol"]].append(r)
    ctor_pub = defaultdict(set)
    for n, vs in ctx["pubname"].items():
        if n.startswith("??0"):
            ctor_pub[n].update(vs)
    ycls = {P: c for (P, _r, _es), c in zip(A, K["ycls"])}

    def ctor_place(X):
        pre = "??0" + X + "@@"
        if any(s.startswith(pre) for s in msym):
            return "matched already", "-"
        named = sorted(f["status"] for f in funcs.values() if f["name"].startswith(pre))
        ok_ = [s for s in named if s in acc_st or s == "near-corr"]
        rt = f"named ({ok_[0]})" if ok_ else f"pinned-provisional ({named[0]})" if named else None
        crs = [r for s, rs in crow.items() if s.startswith(pre) for r in rs if (s, r["va"]) not in matched]
        if crs:
            r = crs[0]
            ln = r.get("lane", "identical")
            pl = f"{ln} lane " + (bk.get(id(r), "?") if ln != "ambiguous" else "pass" if r["pass"] else "fail")
            return pl, rt or ("name pinned, row in lane" if ln != "ambiguous" else "none")
        pvs = sorted(v for n, vs in ctor_pub.items() if n.startswith(pre) for v in vs)
        if not pvs:
            return "no proto row: no ctor public", rt or "none"
        for v in pvs:
            fr = byva.get(f"0x{v:08X}")
            if fr is None:
                continue
            if fr["class"] == "ambiguous":
                c = ycls.get(v, "none")
                return f"ambiguous ({c})", rt or (c if c != "none" else "none")
            if fr["class"] == "near-identical":
                w = why.get(fr["protoVA"])
                rv = int(fr["candidateVA"], 16) if fr.get("candidateVA") else None
                nc = rv is not None and near_chk(S, v, rv)["verdict"] == "pass"
                return f"outside lane: {w[0]}" + (f" ({w[1]})" if w and w[1] else "") if w else "near lane", \
                    rt or ("near-corr" if nc else "none")
            return f"class {fr['class']}", rt or "none"
        return "no proto row: public, no functions.jsonl row", rt or "none"

    gpl, GL, gnb = Counter(), defaultdict(list), Counter()
    for r in gr:
        m = G_RE.match(r["symbol"])
        pl, rt = ctor_place(m.group(1)) if m else ("no class parse", "none")
        gpl[pl] += 1
        note = NOTES.get(r["symbol"]) or NOTES_VA.get(r["va"])  # 5.4 T12.1.c7: guard by NOTES/NOTES_VA
        if note:
            used.add(r["symbol"] if r["symbol"] in NOTES else r["va"])
            gnb[note[0]] += 1
        GL[pl].append(f"  - 0x{r['va']:06X} {r['symbol'][:70]} {short(r['unit'])}: route {rt}"
                      + (f" — {note[0]}: {note[1]}" if note else ""))
    check(f"near sizing (d) ??_G passing unmatched {len(gr)} = by ctor place {sum(gpl.values())}",
          sum(gpl.values()) == len(gr) == len(gpass) - sum(1 for r in gpass if (r["symbol"], r["va"]) in matched))
    L += ["", f"### (d) `??_G` rows: {len(gr)}", "",
          "Filter: candidates.jsonl lane ambiguous, `symbol` starts `??_G` (names[0] is undecorated), pass, not in "
          f"matched_set. Reconcile: lane `??_G` rows {len(gall)}, passing {len(gpass)}, passing matched "
          f"{len(gpass) - len(gr)}, passing unmatched {len(gr)}. Class X from `??_GX@@?AEPAXI@Z`; ctor = PDB publics / "
          "candidates rows `??0X@@`. Place: matched already, a candidates lane row (identical, near + bucket, "
          "ambiguous pass/fail), else the ctor public's functions.jsonl row (ambiguous + settling class, outside "
          "reason, class), else no proto row. Route: named (symbols status), a ruled class, near-corr, or none. "
          "A row a NOTES/NOTES_VA key guards ends ` — <bucket>: <reason>` (5.4 T12.1.c7).", "",
          f"- guarded by NOTES/NOTES_VA {sum(gnb.values())}: {cs(gnb, sorted(gnb)) or 'none'}", ""]
    for pl in sorted(GL, key=lambda k: (-gpl[k], k)):
        L += [f"- {pl}: {gpl[pl]}"] + GL[pl]
    # 5.4 T12.1.c7: other ambiguous-lane rows (not `??_G`) a NOTES/NOTES_VA key guards; listed so the key is used
    go = sorted((r for r in cand if r.get("lane") == "ambiguous" and not r["symbol"].startswith("??_G")
                 and (r["symbol"], r["va"]) not in matched
                 and (NOTES.get(r["symbol"]) or NOTES_VA.get(r["va"]))), key=lambda r: r["va"])
    L += ["", f"Other ambiguous-lane rows (not `??_G`, unmatched) a NOTES/NOTES_VA key guards: {len(go)}", ""]
    for r in go:
        note = NOTES.get(r["symbol"]) or NOTES_VA.get(r["va"])
        used.add(r["symbol"] if r["symbol"] in NOTES else r["va"])
        L.append(f"- 0x{r['va']:06X} {r['symbol'][:70]} {short(r['unit'])} — {note[0]}: {note[1]}")

    # (e) unit not delink by unit
    nd = [p for p, w in why.items() if w == ("cut", "unit not delink")]
    cu = Counter()
    for p in nd:
        u = unit_at(units, ustarts, int(byva[p]["candidateVA"], 16))
        cu[(u["name"], u["index"])] += 1
    se = [k for k in cu if k in K["esett"]]
    check(f"near sizing (e) unit not delink {len(nd)} = by unit {sum(cu.values())} ({len(cu)} units; settled-edge "
          f"{len(se)} units, {sum(cu[k] for k in se)} rows)", sum(cu.values()) == len(nd))
    L += ["", f"### (e) Outside, unit not delink: {len(nd)} ({len(cu)} units)", "",
          "Count desc; `settled-edge` = the unit has >= 1 M27 ambiguous edge a ruled or candidate class settles "
          f"(edge proposal; {len(K['esett'])} units).", ""]
    L += [f"- {short(n)}#{i} {c}" + (" settled-edge" if (n, i) in K["esett"] else "")
          for (n, i), c in sorted(cu.items(), key=lambda kv: (-kv[1], kv[0]))]

    # (f) fold rows
    fby = defaultdict(int)
    for x in candidates.load_jsonl(candidates.need(candidates.FOLDS)):
        fby[int(x["retailVA"], 16)] += len(x.get("protos", ()))
    fl = sorted(p for p, w in why.items() if w[0] == "fold")
    FL, pays = [], 0
    for p in fl:
        P, va = int(p, 16), int(byva[p]["candidateVA"], 16)
        u = unit_at(units, ustarts, va)
        dl = bool(u and u["kind"] == "game" and u["delink"])
        nc, k1 = near_chk(S, P, va), cls_k1(S, P, va)
        ok = dl and (nc["verdict"] == "pass" or k1)
        pays += ok
        al = funcs.get(va)
        FL.append(f"- {p} 0x{va:06X} {byva[p]['names'][0][:60]}: group {fby.get(va, 0)}; unit "
                  f"{short(u['name']) if u else '-'} delink {'yes' if dl else 'no'}; alias named "
                  f"{al['name'][:50] + ' (' + al['status'] + ')' if al else 'no'}; near_check {nc['reason']}"
                  f"{'; k1' if k1 else ''} -> {'lane' if ok else 'no'}")
    check(f"near sizing (f) fold {len(fl)} = would enter the lane {pays} + not {len(fl) - pays}",
          len(fl) == sum(1 for w in why.values() if w[0] == "fold"))
    L += ["", f"### (f) Outside, fold: {len(fl)}", "",
          "Line: protoVA foldVA name: fold group size (retail_folds protos at the VA); owning unit, delinked; another "
          "alias named in symbols at the VA; near_check reason (k1 if cls_k1) -> lane if delinked and (pass or k1).", ""]
    L += FL + ["", f"- verdict: a fold-VA naming rule (C0154) would put {pays} row(s) in the lane: "
               + ("pays" if pays else "does not pay")]

    # (g) pilot ranking
    gcls = defaultdict(list)  # class X -> its ??_G lane rows (any unit)
    for r in gall:
        m = G_RE.match(r["symbol"])
        if m and (r["symbol"], r["va"]) not in matched:
            gcls[m.group(1)].append(r)
    rdy = ("ready", "ready-opt-unknown")
    PR = []
    for u, rs in byu.items():
        if len(rs) < 3:
            continue
        nr = sum(1 for r in rs if bk[id(r)] in rdy)
        nbin = len({sbin(r["size"]) for r in rs})
        pair = next((r["symbol"] for r in rs if r["symbol"].startswith("??0") and bk[id(r)] in rdy + RB
                     and any(g["pass"] or all(v["pass"] for k, v in g["checks"].items() if k != "reloc")
                             for g in gcls.get(r["symbol"][3:].split("@@")[0], ()))), None)
        at = [tries(r) for r in rs]
        PR.append((-nr, -nbin, pair is None, sum(at), u, len(rs), sum(1 for x in at if x), pair))
    PR.sort()
    nu3 = sum(1 for v in byu.values() if len(v) >= 3)
    check(f"near sizing (g) pilot units ranked {len(PR)} = near units with >= 3 unmatched {nu3}", len(PR) == nu3)
    L += ["", f"### (g) Pilot ranking: near units with >= 3 unmatched rows: {len(PR)} (top 8)", "",
          "Rank: ready rows (ready, ready-opt-unknown) desc, distinct size bins desc, a ctor row (ready or reloc) with "
          "its `??_G` (pass, or failing reloc only) first, ledger attempts asc. No pick.", ""]
    L += [f"{i}. {short(u)}: rows {n}, ready {-a}, bins {-b}, ctor+??_G {p or '-'}, attempts {t} on {na} rows"
          for i, (a, b, _np, t, u, n, na, p) in enumerate(PR[:8], 1)]
    return L + [""]


def qa_bucket(ln):
    """One qa_absent.md row line -> (protoVA, bucket, QA result classes); reads QA evidence only (I5)."""
    m = QA_ROW.match(ln)
    if not m:
        return None, None, []
    res = re.split(r"; (?=0x[0-9A-F]{8} )", m.group(3)) if m.group(3) else []
    cls = [next((t for t in reversed(x.split()) if t in QA_CLASSES), "?") for x in res]
    b = ("any identical hit" if "identical" in cls else "near-identical" if "near-identical" in cls else
         "excluded" if "excluded" in cls else "absent in QA too" if "absent" in cls else "no QA name")
    return m.group(1), b, cls


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--out", default=str(OUT))
    a = ap.parse_args(argv)
    for p in (CAND, LEDGER, MATCHED, UNITS):
        if not p.is_file():
            refuse(f"missing input {p.relative_to(REPO)}")
    stale = candidates.fresh()  # 5.5 T6 (C4)
    if stale:
        refuse("candidates.jsonl stale: " + "; ".join(stale) + " (run PY tools/candidates.py --write)")
    pin = coff_info.pinned_sha() or ""
    cut = byte_compare.cut_dir(pin)
    why = byte_compare.cut_check(cut)
    if why:
        refuse(why)
    rows = [json.loads(x) for x in CAND.read_text(encoding="utf-8").splitlines() if x.strip()]
    crows = rows  # 5.4 T7: every lane, the same row objects (near_sizing reads their buckets by id)
    nrows =[r for r in rows if r.get("lane") == "near"]  # 3.4 T13: near lane section
    rows = [r for r in rows if r.get("lane", "identical") == "identical"]  # 3.4 T2: near lane in next_candidates.md
    ledger = match_one.load_ledger()
    ms = tomllib.loads(MATCHED.read_text(encoding="utf-8"))
    matched = {(f["symbol"], as_int(f["va"])) for f in ms.get("function", [])}
    maxsize = max((f["size"] for f in ms.get("function", [])), default=0)
    opt = {u["name"]: u.get("opt", "unknown") for u in tomllib.loads(UNITS.read_text(encoding="utf-8"))["unit"]}
    led = defaultdict(list)  # keyed by VA and by symbol: refused runs log va null
    for x in ledger:
        if x.get("va") not in (None, "None"):
            led[as_int(x["va"])].append(x)
        led[x["symbol"]].append(x)

    pool = len(rows)
    objs, used = {}, set()
    unmatched, out, extra, targets = assign(rows, matched, opt, pin, cut, led, maxsize, objs, used)
    unm_n, out_n, extra_n, targets_n = assign(nrows, matched, opt, pin, cut, led, maxsize, objs, used)
    try:
        fx = candidates.load_jsonl(candidates.need(candidates.FUNCS))
    except candidates.Refuse as e:
        refuse(str(e))
    ncls = Counter(r["class"] for r in fx)
    why, lane = outside_lane(fx)
    gfj = REPO / "derived" / "ghidra" / f"retail-{pin[:8]}" / "applied" / "functions.jsonl"
    if not gfj.is_file() or not QA_ABSENT.is_file():
        refuse(f"missing input {gfj.relative_to(REPO)} or {QA_ABSENT.relative_to(REPO)}")
    entries = {int(f["entry"], 16) for f in candidates.load_jsonl(gfj)}
    qa = [x for x in QA_ABSENT.read_text(encoding="utf-8").splitlines() if x.startswith("- 0x")]

    fail = []
    total = sum(len(v) for v in out.values())
    npm = pool - len(unmatched)  # matched rows inside the pool; lane (near) matches sit outside it (3.4 T4)
    sums = [f"identical pool {pool} = buckets {total} + matched {npm}"]
    if total != pool - npm or total != len(unmatched):
        fail.append(sums[-1])
    tn = sum(len(v) for v in out_n.values())
    nm = len(nrows) - len(unm_n)
    sums.append(f"near-identical {ncls['near-identical']} = near lane {tn} + matched-near {nm} + outside {len(why)}")
    if tn + nm + len(why) != ncls["near-identical"] or tn != len(unm_n) or lane != len(nrows):
        fail.append(sums[-1] + f" (recomputed lane {lane}, candidates near rows {len(nrows)})")

    amb, kc, hc = defaultdict(list), defaultdict(Counter), defaultdict(Counter)
    for r in fx:
        if r["class"] != "ambiguous":
            continue
        k = sum(1 for v in r["targetVAs"] if int(v, 16) in entries)
        b = "short-inline-copy" if r["length"] < 64 or k == 1 else "multiple-targets" if k >= 2 else "other"
        amb[b].append(r)
        kc[b]["0" if k == 0 else "1" if k == 1 else "2+"] += 1
        hc[b]["32 (cap)" if r["hits"] >= 32 else "2-3" if r["hits"] <= 3 else "4-31"] += 1
    ta = sum(len(v) for v in amb.values())
    AMB = (("short-inline-copy", "length < 64 or exactly one target is a Ghidra entry (an inline copy inside "
            "callers, or one standalone body)", "resolve by the entry hit / site evidence (C0010)"),
           ("multiple-targets", "k >= 2 targets are Ghidra entries (several standalone bodies, same bytes)",
            "resolve per target by caller/site correspondence (C0059, C0101)"),
           ("other", "length >= 64 and no target is a Ghidra entry", "a Ghidra function at a target first, then as above"))
    sums.append(f"ambiguous {ncls['ambiguous']} = " + " + ".join(f"{k} {len(amb[k])}" for k, _v, _n in AMB))
    if ta != ncls["ambiguous"]:
        fail.append(sums[-1])

    ab, abx = defaultdict(list), Counter()
    for ln in qa:
        pva, b, cls = qa_bucket(ln)
        ab[b].append(pva)
        abx[(b, len(cls) > 1)] += 1
    ABS = ("any identical hit", "near-identical", "excluded", "absent in QA too", "no QA name")
    tq = sum(len(v) for v in ab.values())
    sums.append(f"absent {ncls['absent']} = " + " + ".join(f"{k} {len(ab[k])}" for k in ABS)
                + f" (qa_absent.md rows {len(qa)})")
    if tq != ncls["absent"] or len(qa) != ncls["absent"] or None in ab:
        fail.append(sums[-1])
    residue = []  # 5.3 T1: ambiguous_residue.md row lines
    keep = {}
    sizing = lane_sizing(fx, entries, why, sums, fail, {va for _s, va in matched},  # 5.1 T9; bracket anchors 5.3 T1
                         unmatched, residue, keep)
    nsz = near_sizing(fx, why, sums, fail, keep, crows, unm_n, out_n, out, matched, led, objs, pin, used) \
        if sizing is not None else []  # 5.4 T7

    for key in sorted(set(NOTES) | set(NOTES_VA), key=str):  # 5.5 T6 (C4): fails; was a WARN line
        if key not in used:
            fail.append(f"note key {key if isinstance(key, str) else f'0x{key:08X}'} "
                        "is not an unmatched row of any lane (drop it)")
    for s in sums:
        print(f"{s}: {'FAIL' if any(f.startswith(s) for f in fail) else 'OK'}")
    if fail:
        for f in fail:
            print(f"pool_buckets: FAIL {f}")
        return 1

    ins = [("derived/candidates/candidates.jsonl", CAND, f"{pool} identical + {len(nrows)} near rows, "
            "from `PY tools/candidates.py --write`"),
           *((p.relative_to(REPO).as_posix(), p, f"{len(ledger)} ledger lines, all files" if p == LEDGER else "ledger, "
             "appended after the base") for p in match_one.ledger_paths()),
           ("config/matched_set.toml", MATCHED, f"{len(matched)} matched, {npm} identical + {nm} near in the lanes"),
           ("config/retail/units.toml", UNITS, "opt, kind, delink"),
           ("derived/proto_map/retail-xbe/functions.jsonl", candidates.FUNCS, f"{len(fx)} rows, class counts"),
           ("derived/dup_report/retail_folds.jsonl", candidates.FOLDS, "fold VAs"),
           ("config/retail/symbols.txt", candidates.SYMS, "lane names"),
           ("config/retail/symbols_pin.txt", candidates.SYMS_PIN, "lane names"),
           (gfj.relative_to(REPO).as_posix(), gfj, f"{len(entries)} Ghidra entries"),
           ("docs/evidence/qa_absent.md", QA_ABSENT, f"{len(qa)} row lines")]
    L = ["# Pool buckets: every unmatched candidate row by verdict and need (D29)", "",
         "Generated by `PY tools/pool_buckets.py` (2.5 T14; 3.3 verdicts 3.3 T16; near lane, outside, ambiguous, "
         "absent 3.4 T13); do not hand-edit, re-run it. Inputs (sha256[:12]):", ""]
    L += [f"- `{n}` {sha8(p)} ({d})" for n, p, d in ins]
    L += [f"- delink cut `{cut.relative_to(REPO).as_posix()}` (function symbol extents, `byte_compare.extent`); wall "
          "verdicts kept only in task summaries are the `NOTES` / `NOTES_VA` tables in the tool.", "",
          "Sums (each checked; a mismatch exits 1):", ""] + [f"- {s}" for s in sums]
    L += ["", "## Identical lane", "",
          f"Pool {pool}, matched {npm}, unmatched {total} = sum of the buckets below. A row sits in the first "
          "bucket (in this order) whose test it meets. `attempted` = has ledger lines; attempts count `kind` attempt. "
          f"Largest matched function: {maxsize} bytes.", ""]
    render(L, out, extra, targets, "###", {})
    L += ["## Near lane", "",
          f"candidates.jsonl lane near: {len(nrows)} rows, matched {nm}, unmatched {tn} in the buckets below "
          "(same tests and order as the identical lane; ranks are per lane; reloc gaps are unnamed callees).", ""]
    render(L, out_n, extra_n, targets_n, "###", NEAR_NEEDS)
    L += ["## Near-identical outside the lane", "",
          f"functions.jsonl class near-identical rows not in the lane: {len(why)}, by the first reason "
          "`candidates.py` excludes them (recomputed with its helpers; recomputed lane "
          f"{lane} == candidates near rows {len(nrows)}).", ""]
    OUTS = (("cut", "candidateVA outside a delinked game unit (sub-counts by NEAR_EXCL reason)",
             "the unit delinked / classed game (units.toml), else outside the matching scope"),
            ("fold", "candidateVA is a fold VA (retail_folds.jsonl)", "a fold-VA naming rule (C0154)"),
            ("no name evidence", "no func line at candidateVA in symbols.txt / symbols_pin.txt",
             "a near-corr or disasm-verified func line (C0059, C0101)"),
            ("name not admitted", "func line has size `-` or a status outside NEAR_STATUSES (sub-counts by status)",
             "the line's size set and its status promoted to near-corr or disasm-verified"),
            (f"no candidateVA/sim<{proto_map.THRESHOLD}", "no candidateVA or similarity below the threshold",
             "nothing in this lane"))
    byva = {r["protoVA"]: r for r in fx}
    for k, verdict, needs in OUTS:
        ps = sorted(p for p, w in why.items() if w[0] == k)
        sub = Counter(w[1] for w in why.values() if w[0] == k and w[1])
        ex = f"`{byva[ps[0]]['names'][0]}` (protoVA {ps[0]})" if ps else "-"
        L += [f"### {k}: {len(ps)}", f"- verdict: {verdict}", f"- needs: {needs}", f"- example: {ex}"]
        if sub:
            L.append("- sub-counts: " + ", ".join(f"{s} {n}" for s, n in sorted(sub.items())))
        L.append("")
    L += ["## Ambiguous", "",
          f"functions.jsonl class ambiguous: {ta}. k = targetVAs that are Ghidra entries; hits in functions.jsonl are "
          f"capped at {proto_map.HIT_CAP}, the uncapped recount lives in `## Lane sizing`.", ""]
    for k, verdict, needs in AMB:
        rs = sorted(amb[k], key=lambda r: r["protoVA"])
        ex = f"`{rs[0]['names'][0]}` (protoVA {rs[0]['protoVA']}, {rs[0]['length']} B)" if rs else "-"
        L += [f"### {k}: {len(rs)}", f"- verdict: {verdict}", f"- needs: {needs}", f"- example: {ex}",
              "- by k: " + ", ".join(f"{s} {kc[k][s]}" for s in ("0", "1", "2+")),
              "- by hits: " + ", ".join(f"{s} {hc[k][s]}" for s in ("2-3", "4-31", "32 (cap)")), ""]
    L += ["## Absent", "",
          f"`docs/evidence/qa_absent.md` row lines: {len(qa)}, by QA result (first match in this order). QA evidence "
          "only (I5): no names derived from it.", ""]
    ABSN = {"any identical hit": "a QA name hit an identical row: the default name is likely an overload (T11)",
            "near-identical": "a QA name hit a near-identical row", "excluded": "a QA name hit an excluded row",
            "absent in QA too": "absent in the QA build as well", "no QA name": "no QA result on the row"}
    for k in ABS:
        ps = sorted(ab[k])
        L += [f"### {k}: {len(ps)}", f"- verdict: {ABSN[k]}", f"- rows with > 1 QA result: {abx[(k, True)]}",
              "- protoVAs: " + (", ".join(ps) if 0 < len(ps) <= 8 else f"{ps[0]} .. {ps[-1]}" if ps else "-"), ""]
    L += sizing + nsz
    out_path = Path(a.out).resolve()  # relative to the working directory, inside the repo or not (4.1 T10)
    out_path.write_text("\n".join(L).rstrip("\n") + "\n", encoding="utf-8", newline="\n")
    R = ["# Ambiguous residue: functions.jsonl ambiguous rows no ruled class settles (5.3 T1)", "",
         "Generated by `PY tools/pool_buckets.py` (5.3 T1) beside pool_buckets.md; do not hand-edit, re-run it. "
         "Inputs (sha256[:12]): " + ", ".join(f"`{p.relative_to(REPO).as_posix()}` {sha8(p)}" for p in (
             candidates.FUNCS, UNITS, candidates.SYMS, candidates.SYMS_PIN, gfj, CAND)) + f". Rows: {len(residue)}.",
         "", "Fields: k1 (k0 / k2+ / shared n = rows claiming the one body), D115 (near_check reasons over the bodies; "
         "the name-step reason once), D113, caller-xref (no-site / conflict), would (candidate classes settling it), "
         "R183 (k0 rows: hits with start evidence), edge (unit#index side of an M27 ambiguous edge).", "",
         "Ruled classes searched (a row is assigned when one settles a body; none did here): k1, D115, D113, "
         "caller-xref, bracket (D128), fold-alias (D129), fold-k1 (D130); fields for the last three are omitted.", "",
         f"Sum: residue rows {len(residue)} (ambiguous rows minus the assigned yield rows, checked in pool_buckets.md)",
         ""] + residue
    (out_path.parent / "ambiguous_residue.md").write_text("\n".join(R) + "\n", encoding="utf-8", newline="\n")
    for k, _v, _n in BUCKETS:
        print(f"{k} {len(out.get(k, []))} near {len(out_n.get(k, []))}")
    print(f"pool_buckets: wrote {out_path.relative_to(REPO).as_posix() if out_path.is_relative_to(REPO) else out_path}: pool {pool}, matched {npm}, "
          f"unmatched {total} in {sum(1 for v in out.values() if v)} buckets; near {tn} in "
          f"{sum(1 for v in out_n.values() if v)} buckets")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
