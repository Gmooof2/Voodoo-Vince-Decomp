# src/ — decompiled game source

Built by `PY tools/build.py --src src --out .run/<phase>/build` (docs/ops/tools.md `build`).
Before matching: `docs/matching/START_HERE.md` (setup, claim, work loop, finish); every form a function carries
(annotations, provenance tags, `src/include/decomp.h` size asserts, `!FAKE`, NON_MATCHING): `docs/matching/CONVENTIONS.md`.

## Path rule
One source file per `config/retail/units.toml` unit. Path = unit `objname` minus the `d:\beep\` prefix, with
`\Xbox\OfficialRelease\` dropped, `.obj` -> `.c` (`language = "c"`) or `.cpp` (`language = "c++"`), `\` -> `/`,
under `src/`. Example: `d:\beep\Apps\Vince\scripts\Carnival\Xbox\OfficialRelease\EvilVince.obj` ->
`src/Apps/Vince/scripts/Carnival/EvilVince.cpp`. Matched case-insensitively; keep the objname's case.
Only `.c`/`.cpp` are sources (headers and this file are ignored). A source that maps to no unit, or to a unit whose
`opt` is still `"unknown"`, makes the build refuse (exit 2). Each unit compiles with its own `opt`; no per-function
flag override.

## NON_MATCHING marker (D41)
Code outside the matched set sits inside `#ifdef NON_MATCHING` ... `#endif`. The gate build never defines
`NON_MATCHING`, so only matched code reaches the gate objs. This is the only mechanism for unmatched code.
