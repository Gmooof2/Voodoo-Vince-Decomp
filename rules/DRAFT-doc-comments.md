# DRAFT (5.5 T17; not in force, not indexed) — Doc comments state what the code verifiably does, never intent
status: draft, held back by the developer (2026-11-11) until Gen 6 settles the readability standard; do not ratify at a plan approval before then · origin: R133 (stage 1 and the pilot's draft rule), R142, 5.5 T17 pilot, 5.5 T17.1 (no restating)
· ratify via `PY tools/rules_add.py add --id D<next> --title ... --file rules/DRAFT-doc-comments.md`, then delete this file

Text:
1. A doc comment is one to three lines directly above a function's `// FUNCTION:` line, a class's declaration or a
   header's first declaration. It says what the code does: its effect, its conditions, the calls it makes and the
   members it writes, as read from the code beside it. It never restates what the code below shows: no comment
   that a reader gets by reading the next lines (the calls made, the constants passed, the members set); it says
   what the reader cannot see at a glance (the shape of a whole class, a condition spread over several lines).
2. Anything the code alone does not show (a member's meaning, a value's unit, a string's role, which caller uses it)
   carries its source in CONVENTIONS §2's vocabulary with a locator (`// src proto-pdb:default.pdb FogZone::_model`,
   `// src retail 0x<VA> "<text>"`). Without such a source it is left out.
3. No intent, purpose or design reason ("to avoid", "so that", "used for"), no hedge ("probably", "likely", "seems"),
   no invented name, and no claim about gameplay or behaviour on screen. A comment is never evidence for a name, type
   or match (CONVENTIONS §7).
4. Wording is ours: plain present tense, written by us from the code; never copied prose from another project or a
   decompiler's output.
5. Where: one summary on each of the unit's own classes at stage 2 (the per-unit pass), in its header. A function
   gets its own comment only where its body is not self-evident from its name and one screen of code (stage 1 or 2).
   A self-evident function or accessor needs none.
6. A doc comment changes no byte; it lands with the gate green like any edit (CONVENTIONS §7), and provenance_lint's
   hedge and `// src` checks apply to it.

Examples (from the pilot, src/Apps/Vince/scripts/Sewers/):
- good: `// A FogZone that follows one bone of its parent node (used as a GatorNode): Update copies the bone's position`
  `// and sets the rotation to the bone's rotation times _baseRotation. With a negative _parentBone, Update does nothing.`
  (SearchlightBeam.h: the class's shape in two lines, no constant or call a reader sees below)
- bad:  `// Builds the beam's FogZone from the "suspendedLight_base" model, sets _destAlphaBlend to 1 and _Type to 9.`
  (restates the ctor's own lines; the pilot's first "good" example, retired in 5.5 T17.1)
- bad:  `// Creates the searchlight that sweeps the sewer for Vince.` (behaviour not shown by the code)
- bad:  `// Probably the beam's colour.` (hedge, no source)
