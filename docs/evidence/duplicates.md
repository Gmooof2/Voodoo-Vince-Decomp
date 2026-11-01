# Duplicate report before matching (T10, phase 1.2)

Facts only, from runs on 2026-10-25 (`PY tools/dup_report.py`, exit 0). Sources: prototype `default.pdb`/`default.exe`
(reference only), retail USA `default.xbe` (the oracle, sha1 `2953b78229018a34e7f53e85bd34f124d51ef210`) and T9's map
`derived/proto_map/retail-xbe/functions.jsonl`. Groups live only in the gitignored `derived/dup_report/`
(`proto_groups.jsonl`, `retail_groups.jsonl`, `retail_folds.jsonl`, `multi_name.jsonl`, `summary.json`; names, VAs,
lengths, group ids; `source: dup_report`, `status_note: unverified`). This note holds counts, no lists, no bytes (D46).
"Identical masked body" is masked-byte identity, never match evidence (D40, D81).

## Method
- Body key = (length, bytes with every masked field zeroed). Proto mask = proto_map's (rel32 of near call/jmp/jcc,
  4-byte imm/disp inside the prototype image, `.reloc` HIGHLOW sites), imported from `tools/proto_map.py`.
  Retail mask = the same decoder mask over the XBE image bounds; the XBE has no reloc table.
- Retail extents are the mapped proto length (the largest if several protos map to one VA); near-identical
  candidates have no known extent and form the class `no-extent`.
- Refusal (exit 2) when inputs miss the manifest or `functions.jsonl` was built against another retail sha1.
- Every population is split into classes whose counts are asserted to sum to it (D24; exit 1 otherwise).

## Counts
1. Prototype, all `default.pdb` procs deduped by VA (10388): unique 6645, in a duplicate group 3646 (377 groups),
   excluded 97 (data-in-text 61, thunk 36). Group sizes run from 2 (221 groups) to 810; by length: <16 B 958 of
   1191 in groups, 16-63 B 2139/4711, >=64 B 549/4389.
2. Retail, only the VAs T9 located (8467; identical and ambiguous targetVAs plus near candidateVAs): unique 5135,
   in a group 2085 (409 groups), no-extent 1244, data-in-text 3. The rest of retail is unknown until 1.3's ingest.
   19 groups of size 32 are capped by proto_map's `HIT_CAP` 32: their true size is larger or unknown.
3. Fold candidates, over the same 8467 retail VAs: one proto 6745; several protos 1722, of which identical-unique 14,
   ambiguous-hit 1690, near candidate 0, mixed 18. 1704/1722 have all their protos in one report-1 group.
4. Several PDB names at one prototype VA: proc VAs 431/10388; public VAs 612/18692; union 19683 VAs, 612 with
   several names (procs and publics 431, publics only 181). Proc and public names are not merged (publics are
   decorated, procs are not), so a VA counts when it has >=2 distinct proc names or >=2 distinct public names.

## What each count means for matching
- 1: 3646 prototype procs share a masked body with at least one other (mostly short bodies). Bytes alone cannot
  tell such procs apart; their retail location must come from order (T9 anchors) or callers, and one matched body
  is a template for its group, though masked fields may differ in callees and data refs.
- 2: retail also keeps 2085 located duplicates in 409 groups, so these bodies were not folded away in retail at
  masked level. Masked identity hides callee and data differences, so this does not show /OPT:ICF off: ICF folds
  only bodies equal with their relocations.
- 3: the 14 identical-unique VAs, and the 18 mixed ones, are the real fold candidates: two prototype procs, each with
  exactly one retail hit, land on one retail body. Matching writes one function there and marks the others folded
  (reccmp `FOLDED`, R0-025). The 1690 ambiguous-hit VAs are not folds: they are report-1 groups whose members all hit
  every retail copy, so the VA-to-proc pairing stays open.
- 4: 431 prototype proc VAs already carry several proc names, so the prototype link folded identical functions
  itself (inference: ICF or COMDAT folding was on in the prototype link). Expect the same in retail. Names at such a
  VA are aliases of one body; a matched function there covers all of them.

## Multi-public callee VAs (3.3 T9, D44)
- Probe (T9.c1, 0x1819A, two folded `ResourceHandle::operator=` publics): one pin -> the caller naming the other public
  is REFUSED by relink (`does not resolve by name (0 VAs in byname)`); one pin per public -> both callers objdiff 100,
  bytes `match (masked 0)`, relink `digests 16/16 OK`, gate 254/254. Bytes and relink resolve name -> VA, not the cut label.
- binding: a multi-public retail callee VA is named by alias pins, one `symbols_pin.txt` line per prototype public
  (`via fold-alias`, callee-bytes disasm-verified), written by `name_sites.py` `alias()`; a VA failing its gates stays
  residue in `docs/matching/dup_multi_public.md`. Applied 3.3 T9: 84 of 88 VAs, 676 pins; 4 kept.
