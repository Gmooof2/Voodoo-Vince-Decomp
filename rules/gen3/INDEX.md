<!-- Rules index. One line per rule, written by tools/rules_add.py. Grep by id or tag; the full text is rules/<id>.md. Sunset rules stay listed, marked. -->
<!-- <id> | <headline> | <tags> | active | superseded-by:<id> | sunset | <origin: seed-2.0 or Phase <N>> -->
M22 | D96-D107: generation 3's decisions, filed as rules by 3.5 T11. Ratify them as filed. | - | sunset | phase 3.6 plan
M19 | Retire rules/M16.md (verifier_lock write refuses CRLF working copies) in favour of D95, which has the same text; rules/gen2/M16.md, a different archived rule, is untouched | - | superseded-by:D95 | phase 3.2 plan
M16 | 2.6 T5: `verifier_lock.py write` refuses a listed file whose working copy has Windows line endings (w/crlf), so a lock fingerprint can never again be taken from a converted copy (built in T4, the one review task) | - | superseded-by:D95 | phase 3.1 plan
M15 | D83 replaces D82: the same single write path into the boot folder, with no launch command (the client's boot/magicboot verb is removed) | - | superseded-by:D83 | phase 2.4 plan
D82 | Runtime access to the box is read-only to the configured LAN address, except xbdm_deploy.py writing default.xbe in the configured boot folder, attended | runtime,safety | superseded-by:D83 | Phase 2.4 T1 (plan approval 2026-10-27)
D75 | Runtime access to the developer's box is read-only and only to the configured LAN address until a phase plan widens it | runtime, safety | superseded-by:D82 | Phase 1.1 (gen-1 approval 2026-10-25)
X5 | Capture context-dependent knowledge before a fresh session | knowledge, capture | active | seed-2.0
