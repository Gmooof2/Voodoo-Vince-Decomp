# Results log (public)

Headline results per phase, taken from the project's private validation log and cleaned for
publication: no local paths, no machine values, no private commit ids. Each line is an observed
run (exit code checked), not a projection. Phase numbers are `<generation>.<phase>`.

Terms:
- **gate**: `tools/gate.py`. One check per matched function plus global checks (lock, cut, build,
  relink, audit, provenance, annotations, pin rows). Green is exit 0 and `N passed, 0 failed of N`.
- **matched**: objdiff 100% against the object cut from retail, and bytes equal to the retail XBE
  with zero unresolved relocations, register allocation included (rule `D123`, `rules/D123.md`).
- **self-test**: `tools/gate.py --self-test`. Each control plants one fault and must fail at the
  named check: `controls: K failed as expected of K`.
- **relink**: `tools/relink.py` links the matched objects with retail's remaining bytes; green is
  every section digest equal (`16/16`) and the whole image's SHA-1 equal to the retail pin.
- **progress**: `tools/progress.py`, functions and bytes matched of the retail game code. The
  denominator grew over time as more functions were identified in retail.

## Generation 1: ground truth and the first match

- 1.1-1.4: inputs fingerprinted, retail XBE ingested and split into units, compiler (MSVC 7.1)
  pinned by evidence, public firewall audit in place from the first commit.
- 1.6 first match: `GameOptions::BackupState` at objdiff 100, bytes match. Perturbed-constant
  control: objdiff 99.8, bytes mismatch, exit 1.
- 1.7 hardware oracle: read-only comparison of a function in a running Xbox's memory with the
  retail file.
- 1.8 validation, from a fresh clone: gate `1 passed, 0 failed of 1`; self-test 18 of 18;
  audit over the whole history `OK (0 hits)`; progress `1 of 7296` functions (0.01%).

## Generation 2: delinked targets and relink

- 2.1 objects cut from retail become the comparison targets; a cut marker guards them.
- 2.2 attempt ledger and source annotations.
- 2.3 relink to the retail image: census covers every region exactly; `digests 16/16 OK`,
  relinked SHA-1 equal to the retail pin. Gate `24 passed, 0 failed of 24`, PASS relink.
- 2.6 validation, from a fresh clone: gate `110 passed, 0 failed of 110`, PASS cut, PASS relink;
  self-test 20 of 20.

## Generation 3: the gate under review, the first public export

- 3.5 validation, from a fresh clone: gate `607 passed, 0 failed of 607`; self-test 59 of 59
  (was 35); verifier lock pass.
  Progress 607 of 7351 functions (8.26%), bytes 2.65%.
- 3.6 first public export (one commit, allowlist only). From its clone: gate `607 passed, 0 failed
  of 607`; self-test 59 of 59; relink digests 16/16, SHA-1 equal to the pin; audit `OK (0 hits)`.

## Generation 4: scale, naming, public cuts

- 4.1 gate timings and lint; public re-exports walked from a fresh clone with the machine keys
  left empty: gate `615 passed, 0 failed of 615`; self-test 75 of 75.
- 4.2 naming passes by ruled routes: gate `615 passed, 0 failed of 615`; self-test 106 of 106.
- 4.3 close: gate `1398 passed, 0 failed of 1398`, PASS relink; self-test 107 of 107. Progress
  1398 of 7351 (19.02%), bytes 13.01%.
- 4.4 close: gate `1776 passed, 0 failed of 1776`, PASS relink; self-test 109 of 109. Progress
  1776 of 7351 (24.16%), bytes 16.21%. Public cut 1 and Public cut 2, each walked from a fresh
  clone (audit `OK (0 hits)`, audit self-test 220 of 220).
- 4.5 gate review and locked fixes: gate `1776 passed, 0 failed of 1776`; self-test 138 of 138.
  Public cut 3: setup walk from a fresh clone, mismatches 0.

## Generation 5: supply, generators, pins

- 5.1 locked fixes and lane sizing; 5.2 started at 1795 of 7351 (24.42%).
- 5.2 close: delink reproducible (two fresh delinks byte-equal, hash equal to the pinned cut);
  relink `digests 16/16 OK`, SHA-1 equal to the pin; relocation scan 0 unexplained. Gate
  `2011 passed, 0 failed of 2011`, PASS relink; self-test 152 of 152. Progress 2011 of 7396
  (27.19%), bytes 19.82%.
- 5.3 close: delink reproducible; relink 16/16. Gate `2157 passed, 0 failed of 2157`, PASS relink;
  self-test 159 of 159. Progress 2157 of 7403 (29.14%), bytes 20.79%. Public cut 4: walked from a
  fresh clone, gate and self-test as above, mismatches 0.
- 5.4 close: delink reproducible; relink 16/16. Gate `2507 passed, 0 failed of 2507`, PASS relink;
  self-test 169 of 169. Progress 2507 of 7535 (33.27%), bytes 22.65%. Of the phase's +350, 222
  came from generated `RegisterMe` constructors once their class-name literals were pinned.
- 5.5 (in progress): gate `2797 passed, 0 failed of 2797`, PASS relink; self-test 232 of 232.
