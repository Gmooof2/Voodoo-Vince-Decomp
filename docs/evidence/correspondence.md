# Prototype-to-retail correspondence map (T8/T9, phase 1.2)

Facts only, from runs on 2026-10-25. The sources are the prototype `default.pdb`/`default.exe` (reference only) and
the retail USA `default.xbe` (the oracle, sha1 `2953b78229018a34e7f53e85bd34f124d51ef210`). None of the classes below
is match evidence: "identical" means masked-byte identity, and similarity is a heuristic (D81, D40). Every emitted
retail address carries `source: proto_map`, the retail sha1 and `status_note: unverified` (D42). The lists live only
in the gitignored `derived/proto_map/retail-xbe/` (`functions.jsonl`, `near_identical.jsonl`, `absent.jsonl`,
`summary.json`). This note holds no lists and no bytes (D46).

## Command
- `PY tools/proto_map.py` (default target `retail-xbe`; exit 0; about 15 s). Control: `PY tools/proto_map.py --target prototype-xbe`.
- Inputs are verified first: manifest sha1+size for the exe, the pdb and the target, plus the PE/PDB pair check.
  On a failure the tool exits 2.

## Method
- Procs: every `default.pdb` proc record, from all modules, deduplicated by VA (10388). Bytes come from `default.exe`.
- Mask (why: these fields change when code moves, not when it changes). Masked are the rel32 of near call/jmp/jcc,
  every 4-byte imm/disp whose value lies inside the prototype image, and every PE `.reloc` HIGHLOW site inside the
  proc. The `.reloc` sites cover jump tables inside procs, which the decoder cannot see, and they fixed T8's 11
  control misses.
- Excluded, with reason (why: there is no function body to compare). zero-length 0; no-raw-bytes 0; data-in-text 61
  (iced decode errors, 66 invalid instructions); thunk 36 (the body is one unconditional jmp).
- identical: the masked bytes occur at exactly one place in the retail executable sections.
- ambiguous: they occur at 2 or more places (stored hits capped at 32). No minimum length is applied, so short procs
  that recur stay ambiguous rather than being skipped.
- Window (why: functions keep their link order between builds a month apart, so a changed function sits between its
  unchanged neighbours). The anchors are the longest increasing subsequence of (proto VA -> retail VA) over the
  identical procs: 4938 of 5134 are order-consistent, and out-of-order uniques never bound a window. A no-hit proc's
  window is the retail gap between its neighbouring anchors. When the anchors sit in different exec sections, the
  window is two ranges, each clipped to its section. A missing side ends at the section bound. 16 procs have no
  candidate in their gap (`noWindow`) and count as absent.
- Candidates: the gap start, and each instruction start that follows a ret, an unconditional jmp or an int3, once
  int3/nop padding is skipped (why: these are the function-start positions; this avoids sliding over every byte).
- Similarity = 100 * difflib ratio of per-instruction tokens. A token is the iced code, the registers, the memory
  base/index/scale and the unmasked imm/disp. Branch targets and in-image absolute values become wildcards, so the
  score counts differing instructions and length difference (the Halo 2 per-function score idea, R1.2-002 (e)).
  Assignment is one-to-one per gap: greedy by similarity, then lower proto VA, then lower candidate VA. 1463 of 1498
  windowed procs got a candidate.
- near-identical: the assigned similarity is >= 70. absent: the similarity is below 70, there is no candidate, or
  there is no window.

## Threshold 70: reason
- Decoy control: each windowed proc is also scored against the best candidate in a wrong gap (the first non-empty
  gap at least 50 gaps further on). Decoy p50 is 16.67, against 92.31 for the real assigned similarity.
- Decoy share at or above the cut: 4.21% at 70, 4.01% at 75, 3.94% at 80, 0.60% at 85, 0.13% at 90. The decoy rate
  only applies to procs whose true counterpart is missing (about 254 at 70), so the expected false near-identicals at
  70 are about 11 of 1260, under 1%. A cut at 85 would cut decoy hits by 54 but would move 181 procs to absent, most
  of them likely true, and the counts become unstable there (near-identical moves about 9% per 5 points).
- The false-positive risk sits in short procs. Decoy p99 is 84.62 for procs under 16 tokens (n 453), 88.89 for
  16-63 tokens (n 408) and 41.41 for 64 tokens and up (n 637). A short near-identical proc is a weak claim.
- Stability at ±5 points: at 65, near-identical 1301 and absent 213; at 70, 1260 and 254; at 75, 1226 and 288.
  Near-identical moves under 4% per 5 points, and absent stays between 2.1% and 2.8% of non-excluded procs.

## Control (prototype default.xbe, the same build)
- identical 6593 + ambiguous 3698 = 10291, all of the non-excluded procs; near-identical 0; absent 0; excluded 97 (the same reasons).
- All 6593 identical procs sit at the same-name XBE section VA + offset. imagebld rebases, so a raw VA check does not apply (C0010).

## Retail counts (threshold 70)
- All procs: 10388 (1513684 B). Non-excluded: 10291 (1436350 B). Excluded: 97 (77334 B).
- identical: 5134 (49.42% of all, 49.89% of non-excluded); 797606 B (52.69% / 55.53%).
- ambiguous: 3643 (35.07% / 35.40%); 181578 B (12.00% / 12.64%).
- near-identical: 1260 (12.13% / 12.24%); 378220 B (24.99% / 26.33%).
- absent: 254 (2.45% / 2.47%); 78946 B (5.22% / 5.50%).
- The tool asserts that the classes sum to both denominators, for counts and bytes. 0 candidates are shared by more than one near-identical proc.
- Only 1104 of 5134 identical procs sit at the prototype's section-mapped offset: retail layout shifts.

## Site-correspondence naming (3.1 T5)

Names for unresolved relocation targets of pool rows (`PY tools/name_sites.py`, docs/ops/tools.md "## name_sites";
cookbook C0101). Every pool row is prototype-identical, so its prototype proc (candidates.jsonl `protoVA`) has each
relocation site at the same offset. Per row the tool re-checks identity (same mask list, equal unmasked bytes) before
any site is read; a wrong `protoVA` is refused (`--control planted`). Per site: retail target from the XBE bytes,
prototype target from default.exe, name = the one `default.pdb` S_PUB32 public at exactly the prototype target.
Never pinned (generated residue `derived/name_sites/residue.jsonl`): no-public, multi-public (D44), va-two-names,
name-two-vas, named-differently, inside-symbol, section-mismatch, proto-mismatch.
Positive control (`--control positive`, 2026-10-27): 742 retail VAs already named from the PDB, 0 disagreements
(752 after the 10 pilot pins, T5.c2). Other-source names (ghidra, xbsymdb, retail-disasm `_s_`) are counted, not
compared: they are undecorated or retail-made (35 sites).

### Evidence class, source tag and status per target kind (binding, 3.1 T5)
The status says whether the retail target's own bytes were checked; candidates (candidates.py:44, units_seed
STATUSES) resolves `disasm-verified`, `unverified`, `verified` and nothing else, so an unchecked name is `site-corr`
and its row stays `provisional` (reloc check fails) until a byte check or a match names it.
- callee (prototype target in an exec section): class site correspondence + callee body bytes. Checked when the
  prototype proc's masked bytes equal the retail bytes at the retail target and the callee's outgoing targets agree
  by name where retail names them; a callee under 8 unmasked bytes (a `jmp` thunk) needs one such agreeing target.
  Checked: `func 0x<proc length>`, source `proto-pdb-public+site-corr+callee-bytes`, status `disasm-verified`.
  Unchecked (near-identical, no proc): `func -`, `proto-pdb-public+site-corr`, `site-corr`.
- vtable (`??_7`): checked when every slot (to the next prototype public, cap 64, exec-section slots) passes the
  callee check at the retail slot: `data -`, `proto-pdb-public+site-corr+vtable-slots`, `disasm-verified`; else
  `proto-pdb-public+site-corr`, `site-corr`.
- string literal (`??_C@`): checked when the prototype literal bytes through the NUL (2-byte NUL for `_1`) equal the
  retail bytes: `data -`, `proto-pdb-public+site-corr+literal-bytes`, `disasm-verified`.
- float constant (`__real@<hex>`): checked when the retail bytes equal the encoded value (4 or 8 bytes):
  `data -`, `proto-pdb-public+site-corr+real-bytes`, `disasm-verified`.
- global (any other data public): no retail byte check exists (mostly .bss): `data -`, `proto-pdb-public+site-corr`,
  `site-corr`. A global is promoted only by a later byte-level check or a matched user, never by this tool.
Older pins keep their tags (`proto-pdb-public+retail-disasm` etc., by hand, 2.1-2.5).

### Pilot (3.1 T5)
- All pool rows (dry run after T5.c2, 2026-10-27): checked/site-only func 144/132, vtable 15/92, string 611/1,
  float 103/0, global 0/64. Residue 984 sites: proto-mismatch 362, multi-public 340, no-public 282, the rest 0.
- proto-mismatch is mostly not a wrong `protoVA`: candidates lists retail immediates in the low image range (base
  0x10000, e.g. push-buffer constants 0x40300) as unresolved abs sites; they are constants, the prototype masks
  differ, and no name can resolve them (34 rows). A candidates fix (mask only values that are addresses on both
  sides), not naming, unblocks them.
- Pilot unit `core\XBox\OfficialRelease\xbfont.obj` (single-target entry 0x1253D7 blocks `CXBFont::~CXBFont`):
  `name_sites.py --unit xbfont.obj --apply` added 10 pins (2 callees checked, 2 site-corr, 5 `__real@`, 1 literal);
  `delink.py --fresh` exit 0 (ApplySymbols failed 0); `candidates.py --write`, `--check` exit 0; `pool_buckets.py`
  exit 0. Rows out of reloc-code/reloc-data into ready-opt-unknown, all checks pass: `CXBFont::~CXBFont`,
  `CXBFont::CXBFont`, `CXBFont::GetScalingCode`, `CXBFont::Create`. Still blocked, not by this tool's naming:
  `WordWrapCallback` (static .bss target, no public), `GetTextWidth`/`GetTextExtent`/`DrawText` (callee overload
  near-identical, `site-corr`), `CreateShaders`/`End`/`Begin` (constants listed as sites, above).
  Pool-wide (the pins are global): reloc-data 499 -> 488, reloc-code 71 -> 73, ready 87 -> 89, ready-opt-unknown
  79 -> 86. Gate after the fresh cut: 110 passed, 0 failed of 110.

## Data byte route (3.3 T8)

`PY tools/name_sites.py --promote-data [--unit U] [--apply]`, control `--control planted-data` (docs/ops/tools.md
"## name_sites"). Scope: `data` pins with status `site-corr` (249 at 3.3 T8 start: 158 `??_7`, 88 globals, 3 `??_C@`).
binding: a `site-corr` data pin at retail VA R named N is promoted to `disasm-verified` (source
`proto-pdb-public+site-corr+data-bytes`, line rewritten in place; no new status, candidates unchanged) only when the
retail XBE's initialized bytes at R equal the prototype default.exe bytes at the one PDB public named N, over:
the TPI size of the S_GDATA32/S_LDATA32 record at the public's seg:off (record names are undecorated: N == r,
`_`+r, or `?`+reversed scope of r+`@@`; tsize must be one value); for `??_7` the 3.1 slot extent; `??_C@`/`__real@`
keep the 3.1 checks. Pointer fields (offsets the prototype `.reloc` covers) are compared as relocated targets: the
PDB publics at the proto target must share a name with an accepted-status name at the retail target (symbols.txt
STATUSES, pin `disasm-verified`, or a pin promoted earlier in the run; fixed point). The span must carry identity:
one non-zero non-pointer byte or one agreeing pointer. Anything else stays `site-corr` with one reason in
docs/matching/naming_residue.md "## Data pins kept site-corr (3.3 T8)": proto-name, literal-differs, real-differs,
vtable-no-slots, no-type, type-size, no-proto-bytes, bss (no raw retail bytes), pointer-unnamed, pointer-unchecked,
pointer-differs, bytes-differ, zero-bytes. Stated limit: a short scalar passes on its equal bytes (two 1-byte bools
`?_mirrorsEnabled@DrawLevel`, `?_skyVisible@SwampSky` pass on one equal non-zero byte); the name itself already
rests on site correspondence, the bytes confirm it, they do not identify it alone.
- Pilot (M6), unit `Xbox\Official_Release\SwampShaderResource.obj`: 2 pins promoted (`?vertexShaderData@@3PAKA`,
  `?pixelShaderData@@3PAKA`); rows admitted 0: `??0PixelShaderResource` 0x000CAD59 and `??0VertexShaderResource`
  0x000CACB9 keep one unresolved target each.
- All pins: 12 promoted (all globals; 0 vtables), 237 kept: pointer-unchecked 162 (all 158 vtables: their slot
  callees, e.g. 0x00090CA4, 0x00017E42, carry no table name; + 4 globals), bss 68, no-type 4 (`__TI3?` x2, an
  `__imp__` import, a function-local static), literal-differs 3. Rows whose reloc check went provisional -> pass:
  `inflate_fast` 0x0003AC3B, `GeometryNode::Render` 0x000DBFE5, `XBUtil_FindMediaFile` 0x0013851E
  (reloc-provisional -> ready-opt-unknown), `LocalizedStrings::GetGameString` 0x00107DBF (stdsig-reloc -> stdsig).
  Buckets: reloc-provisional 362 -> 359, ready 141 -> 141, ready-opt-unknown 293 -> 296.
- Planted control: R 0x001C74C0 (`_D3DSIMPLERENDERSTATEENCODE`) with planted name `_inflate_mask`: fail
  bytes-differ, 0 lines rewritten, `git diff --exit-code config/retail/symbols_pin.txt` exit 0.
- No target change: delink `hashes.txt` sha256 d099a75b...1f38 before and after `delink.py --fresh` (ApplySymbols
  ignores source and status); gate 253 passed, 0 failed of 253.

## Near-identical naming (3.4 T2)

`PY tools/name_sites.py --near [--apply]` (docs/ops/tools.md "## name_sites"; cookbook C0142, C0059, C0011).
binding: a proto_map near-identical row (no masked-byte identity; bytes differ by definition, so never
`disasm-verified` under M18) keeps its one prototype PDB public N at retail VA R, status `near-corr`, source
`proto-pdb-public+anchor-window+caller-xref`, sized from the Ghidra extent, only with both pieces of evidence:
(a) R lies in the row's LIS-anchor gap window (own anchor removed) and the recomputed similarity is >= 70;
(b) >= 1 rel32 call site to R inside an accepted-status retail function whose prototype counterpart (masked identity)
calls N at the same site, and no such site naming another public. Similarity alone names nothing (decoy windows
reach >= 70 in 4.21%). Folds, multi-public, va-two-rows, name-two-vas, inside-symbol are refused.
`near-corr` names the lane row itself only: candidates never accepts it as a callee, data or slot name in either lane
(a target with it is provisional), and name_sites callee agreement ignores it.
- Counts (3.4 T2): 1260 rows; admitted 240: 96 new near-corr pins, 142 unsized `site-corr` func lines with the same
  name rewritten in place as sized near-corr (`upgrade`, ruling: both evidence pieces hold; acceptance unchanged), 2
  `disasm-verified`; refused no-xref 1009, multi-public 9, no-public 2. Admitted by size: 9-16 2, 17-32 10, 33-64 28,
  65-128 44, >= 129 156.
- Candidates near lane: pool 107 of 1260 (excluded no unit 167, unit not delink 615, fold 5, no lane name 366), pass
  10. By size, pool/passing: 1-8 1/1, 9-16 1/1, 17-32 6/3, 33-64 12/5, 65-128 16/0, 129+ 71/0.
- Controls: docs/ops/validation.md "## 3.4 Near lane controls". The identical-lane `--control positive` disagreement
  at 0x000B4428 is a retail callee change: site `??1PropellerMain@@UAE@XZ` +0xE calls `~CritterMain` in the prototype
  (0x00407909, identical -> retail 0x000186B2) but 0x000B4428 (`~SwampSceneNode`, identical from proto 0x0049F219)
  in retail, under masked identity; production refuses it (named-differently); the control excludes it as
  `callee-moved`.
- No target change: 433 matched rows' delinked targets (extent bytes + relocations) hashed before and after
  `delink.py --fresh` + `relink.py cut`, twice (after the 96 pins, after the 142 upgrades): identical, 0 missing.
