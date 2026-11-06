# Setup: a fresh clone to a green gate

Every step, in order, from nothing to `PY tools/gate.py` green, on native Windows. It takes about 12 minutes of machine
time once the prerequisites are in place. `PY` below means your Python 3.15, for example `python`.

Exit codes, unless a step says otherwise: 0 ok, 1 a check failed, 2 refusal (bad or missing config or input). When a step
fails, check its input first (config key, file on disk, the previous step's output), then the code.

**Ids you will meet.** Tools, docs and source comments cite ids such as `D42`, `M20`, `G26`, `T4`, `3.3 T4` or `R3.5-001`,
and name files such as `HOW_WE_WORK.md`, `GENERATION_PLAN.md`, `phase-ends/`, `rules/` and `docs/ops/`. Those point to a
private journal that is not in this repository. Nothing here needs them. `C<nnnn>` ids are entries in `cookbook/`.

## 1. Prerequisites (you supply them, none in this repository)

- Windows 11 (built and run on Windows 11 Pro 26200; nothing has been tested on another platform) and Git for Windows,
  whose bash runs `tools/*.sh` and the hooks.
- Python 3.15, then `PY -m pip install --require-hashes -r requirements.txt` (iced-x86, the x86 decoder).
- **Your own retail USA disc image** (redump: http://redump.org/disc/50478/). The retail `default.xbe` is the only
  reference for what is correct: sha1 in `config/check.retail_xbe.sha`, 2,453,504 bytes.
- **The July 15, 2003 prototype preview disc image**, `.bin` and `.cue`, which you source yourself. You cannot reach a green gate without it:
  the gate's provenance layer reads the prototype's PDB and EXE and refuses if they are unreadable or differ from
  `config/inputs.manifest.jsonl`. There is no mode without the prototype.
- **Visual Studio .NET 2003 Professional RTM** (cl 13.10.3077, LINK 7.10.3077; never SP1, 6030), your own copy. No setup
  run is needed: the two discs are an uncompressed image, so with 7-Zip, where `P` is
  `Program Files\Microsoft Visual Studio .NET 2003` and `T` is a target folder of your choice:
  1. `7z x -y -o"$T" "<Disc 1.iso>" "$P\Vc7\*" "$P\Common7\IDE\mspdb71.dll" "$P\Common7\IDE\msvcr71.dll"`
  2. `7z x -y -o"$T" "<Disc 2.iso>" "$P\Vc7\*" "$P\Common7\IDE\msobj71.dll"`
  3. Copy `mspdb71.dll`, `msobj71.dll` and `msvcr71.dll` from `$T/$P/Common7/IDE/` into `$T/$P/Vc7/bin/`.
  `tools/check_env.py` compares each file with `config/toolchain.sha256`.
- **Ghidra toolchain**, each file's sha256 checked after download:
  - Ghidra 12.1.4: https://github.com/NationalSecurityAgency/ghidra/releases/tag/Ghidra_12.1.4_build
    `ghidra_12.1.4_PUBLIC_20260921.zip`, sha256 `ddac49f903da9d5bac833e5cc79395098b9c33cfd3279be5f31bd00387d2d4db`
  - ghidra-xbe: https://github.com/XboxDev/ghidra-xbe/releases/tag/build-202610092147
    `ghidra_12.1.4_PUBLIC_20261009_ghidra-xbe.zip`, sha256 `c3d5b639f8be1f77896bda9e0233ecf829752b7d2eb217fb54415eb8a90c1422`
  - ghidra-delinker-extension v0.9.2: https://github.com/boricj/ghidra-delinker-extension/releases/tag/v0.9.2
    `ghidra_12.1.4_PUBLIC_20260926_ghidra-delinker-extension.zip`, sha256 `44c569cbaa8f35faccba3e5c632ae6202420a36082352638f48f0dfa37ad5765`
  - objdiff-cli v3.8.3: https://github.com/encounter/objdiff/releases/tag/v3.8.3
    `objdiff-cli-windows-x86_64.exe`, sha256 `dd12dfb5e9e94a7f6c949a37e1815963cf2cffe4c87b4999b7ebc8c15b398a1f`
  - Adoptium JDK 21.0.12 (`jdk-21.0.12.101-hotspot`); `bin/java.exe` sha256 `82051fdab26319d77d20cc0065045d05ec00b3e3d05f44935d7c06b96b621d55`

  Unzip Ghidra, unzip both extension zips into `<ghidra_dir>/Ghidra/Extensions/` (headless loads them from there, no GUI
  step), and set `JAVA_HOME_OVERRIDE=<java_home>` in `<ghidra_dir>/support/launch.properties`.
- Extract the inputs under one folder outside the repo (`game_dir`), with any XDVDFS extractor:
  - Retail image: the game partition starts at offset `0x18300000`. Take `default.xbe` only.
  - Prototype image: XDVDFS at offset 0 (the ISO 9660 part is a stub). Take the full tree, 406 files, and add nothing to
    it: step 5 fails on a missing or extra file.

## 2. Clone

`git clone <this repository> <dir>`. `.gitattributes` is `* -text`, so every file keeps its exact bytes whatever
`core.autocrlf` says; the verifier lock hashes raw bytes.

The gate's provenance layer compares `src` with the upstream of your branch (`@{u}`), so it needs one: a clone's default
branch has it. A plain `git init` copy or a new local branch without one gives `provenance_lint: REFUSED`, exit 2, until
you run `git branch --set-upstream-to=<ref>`.

## 3. Hooks

`git config core.hooksPath githooks`, once per clone. pre-commit and pre-push run `tools/audit_public.py`; never
`--no-verify`. They use env `PY`, else `python` on PATH; set `PY` if they block with `no Python runs`. The audit refuses
without `config/local_paths.json`, so commits and pushes are blocked until step 4 is done. `tools/check_env.py` fails
until this step is done.

## 4. Local config

Copy `config/local_paths.example.json` to `config/local_paths.json` (gitignored) and fill it in. Paths are absolute or
relative to `game_dir`. A value in this file never goes in a tracked file, a log or a commit.

- `game_dir`: absolute folder holding the inputs, outside the repo.
- `retail_xbe`, `retail_iso`: the extracted retail `default.xbe` and the retail image.
- `prototype_dir`, `preview_iso`, `preview_cue`: the extracted prototype tree and the preview `.bin` and `.cue`.
- `ghidra_project_dir`: an empty folder you create first, one per clone, outside the repo. No path element may start with
  `.` (Ghidra headless refuses it, so never under `.run/`). `delink.py --fresh` deletes its `retail-*` child.
- `ghidra_dir`, `java_home`, `objdiff_cli`: the Ghidra install, the JDK and the objdiff exe.
- `msvc71_dir`: the matching compiler's `Vc7/bin`. Null loads, but `tools/check_env.py` then fails.
- `xbdm_dll`, `boot_folder_source`: only for a real Xbox, which nothing below needs. Set each to `null`.
- `xbox_ip`, `boot_folder`, `original_install`: with a real Xbox, your box's address and folders, which the audit then
  keeps out of every tracked file. Without one, leave these three `null`, with no made-up values: the audit prints
  `not checked` for them and the gate stays green, because the public tree holds `config/PUBLIC_TREE`.

`config/xbox_retail_pubkey.json` (gitignored) is optional and read only by `tools/xbe_pristine.py`; absent, that tool
prints `not checked (no sourced retail key)` for its signature check. The gate does not need it.

Check: `PY tools/vv_inputs.py > /dev/null`. Exit 0 ok; exit 2 names the missing file, missing key or nonexistent path. Its
stdout prints every value, so do not log it.

## 5. Inputs

`PY tools/manifest.py verify` (about 30 s): every input file's sha1 and size against `config/inputs.manifest.jsonl`, and
the retail pin. Exit 0 match; 1 mismatch, missing or extra file; 2 config or manifest missing. With `xbdm_dll` null its
one record is not checked and that is no problem: it prints `xbdm_dll: 1 in manifest, not checked (key is null)` and exits
0, last line `verify: OK — 0 problem(s) among 410 records; not checked: xbdm_dll 1`. A set `xbdm_dll` is checked like any
other input.

## 6. Environment

`PY tools/check_env.py`: Python 3.15, `core.hooksPath`, config, the Ghidra toolchain keys, and the matching compiler
against `config/toolchain.sha256`. One `ok|FAIL <check>` line each. Never run `--pin` on a clone: it rewrites the tracked
pins. `pins ... sha mismatch` means a different compiler build at `msvc71_dir`.

## 7. Producers (gitignored `derived/`, in this order)

Each step refuses without the previous one's output. If a run stops with no output, Ghidra's `.bat` may be waiting at
"Press any key": feed stdin from `nul`. Long logs land under `.run/`.

1. `PY tools/proto_map.py --target retail-xbe`, about 20 s. Last line `wrote derived/proto_map/retail-xbe/functions.jsonl (10388 rows), ...`.
2. `PY tools/dup_report.py`, about 3 s. Last line `classes: ... sum 19683/19683 ok`.
3. `PY tools/delink.py ingest --fresh`, about 2 minutes. Exit 0.
4. `PY tools/delink.py --fresh`, about 3 minutes. Last line `marker <path> objects 215`.
5. `PY tools/xdk_report.py`, about 2 s. Prints `functions (applied): 8988 ...`, last line `sum checks: ok`.
6. `PY tools/relink.py cut`, about 5 minutes. Last line `cut derived/relink/retail-<sha8>/cut: 10 sections, 543 objects, ...`.
   Without it the gate prints `FAIL relink: fail: REFUSED ...`.

Steps 1, 3, 4 and 6 are what the gate reads. Step 5 and the next two feed the other tools:

7. `PY tools/candidates.py --write`, about 2 s, regenerates `docs/matching/next_candidates.md` and
   `docs/matching/delink_residue.md`. `git status --porcelain` is empty afterwards.
8. `PY tools/name_sites.py`, about 2 s, then `PY tools/name_sites.py --check-cut`: `... FAIL 0`, exit 0.

## 8. The gate

- `bash tools/run.sh gate -- PY tools/gate.py`, up to about 2 minutes. Green is exit 0, every global line `PASS`, and the bare
  line `1776 passed, 0 failed of 1776` (N is the `[[function]]` rows of `config/matched_set.toml`). A failed global adds
  `; FAIL globals: <names>` and exits 1; a refusal exits 2. The exit code and that line are the only claim of a match.
- `bash tools/run.sh selftest -- PY tools/gate.py --self-test`, up to about 2 minutes: plants known errors and proves the gate
  catches each one. Last line `controls: 138 failed as expected of 138`, exit 0.
- When it fails: `FAIL lock` first (a file's bytes changed), `FAIL check_env` (step 6), `target missing` or
  `gate: REFUSED cut` (step 7), `FAIL relink: fail: REFUSED` (`relink.py cut`), `FAIL provenance: fail: REFUSED` (step 2's
  upstream, or the prototype inputs), then the named layer.

## 9. Optional

- `PY tools/relink.py check`, about 30 s: `check all: digests 16/16 OK`; the relinked XBE's sha1 equals the retail pin.
- `PY tools/progress.py` regenerates `docs/progress.md` from step 5 and the last gate run.
- `PY tools/match_one.py --help`: build one function's unit and compare it, a quick check while matching. Only the gate
  decides. Start with `docs/matching/START_HERE.md`.
