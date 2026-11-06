"""Relink: exact-cover census of the pinned retail XBE (phase 2.3 T1).

CLI: relink.py census [--objects FILE] [--control overlap|drop] [--config P]
  Expected extents come only from the retail XBE headers (pinned by config/check.retail_xbe.sha):
  region `header` [baseAddr, baseAddr+sizeOfHeaders) and per section [va, va+rawSize) raw-backed,
  plus [va+rawSize, va+vsize) when vsize > rawSize.
  Default range list: .text split by the config/retail/units.toml units whose text_start/text_end lie
  inside .text (delink=true -> `pinned unit`, else `unpinned unit`; holes in .text -> `gap`); other
  sections one range each by name/flags; vsize>rawSize parts -> `uninitialized tail`. Units lying
  inside an XDK section are no ranges: counted per section (info) and listed in census.json
  `unitsInXdkSections`. A unit crossing a region edge, outside every region or inside any other
  region -> exit 1. A section matching no rule -> exit 2.
  --objects FILE  check FILE instead of the default list. One range per line, TAB-separated:
                  <start_hex>\t<end_hex>\t<class>\t<name>  (VA, end exclusive); `#` starts a comment line.
  --control overlap|drop  negative control on the default list: inject one overlapping range, or
                  drop one range; prints what it did; must exit 1.
  Default run (no --objects/--control) writes derived/relink/retail-<sha1[:8]>/census.json
  (addresses, sizes, classes, names; never bytes, D46).
Exit: 0 every region exactly covered and no flagged range; 1 check failed; 2 refusal.

CLI: relink.py cut [--section NAME] [--repro] [--config P]   (T3; no --section: every linked section, T5)
  Range-export the section's census ranges via DelinkProgram (existing Ghidra project, -noanalysis -readOnly) into
  derived/relink/retail-<sha8>/cut/<NAME>/ + hashes.txt + cut.complete (delink marker), one dir per section (T4);
  .text = its unit/gap ranges, one object each, batched over analyzeHeadless calls under delink.ARG_BUDGET.
  Prints externals per target section. --repro: second cut into cut.repro/<NAME>/; exit 0 only if both hashes.txt equal.
  No --section (T5): all 10 linked sections (pin order), then cut/partition.tsv (census --objects format: every cut
  object range, name `<NAME>/<file>`, + the header, inserted-file and uninitialized-tail ranges) and, LAST, the top
  cut/hashes.txt (sha256 of each <NAME>/hashes.txt and of partition.tsv) + cut/cut.complete over it. --repro compares
  the two top hashes.txt.
CLI: relink.py link [--section A[,B..]]   (T3, T4; no --section: whole image, T5)
  Finished cuts of the real sections (bad marker -> exit 2) + Python filler objects for every run of other linked retail
  sections (zero-filled, retail order, PE vsize and flags). Every external no linked object defines gets an alias at
  its target VA in the object covering it (cut object or filler; outside every section: absolute); a name defined
  twice, with != 1 VA by name, or whose reloc sites' retail fields disagree is VA-resolved per site (`<name>@va_<va8>`).
  Last cut object zero-padded to the retail PE VirtualSize. .text cut objects -> one COMDAT each (NODUPLICATES,
  `__rl_<va8>` at 0, ALIGN_1), /ORDER (VA order) + /OPT:REF + /INCLUDE each, all via a response file. Pinned LINK 7.10
  (LINK_FLAGS) -> derived/relink/retail-<sha8>/link/<A+B..>/<A+B..>.exe + .map. Any LINK message -> exit 1.
CLI: relink.py check --section A[,B..] (--originals | --units U[,V..] [--control no-order|noref])   (T3, T4)
  link, then xbe_write.write(); on its refusal xbe_write.write_section() per section; compare each XBE digest to retail.
  --units (T4.c2): matched-set units by basename stem; our objects .run/<phase>/build/<basename> (tools/build.py).
  Each matched row's [va, va+size) and each of our image data sections ([VA of a symbol in it by name - its value,
  +size)) is a hole: the covering cut object is split into pieces (COMDATs in .text), the hole gets no piece; our
  function symbol takes the hole's /ORDER slot (+ /INCLUDE), our data object sits between the data pieces on the
  command line; a section receiving our data is added to --section. References of the originals into a hole go to
  our symbol there (field += offset); our objects are never edited. Our undefined externals resolve by name only
  (byte_compare.setup byname, one VA) -> alias at that VA; no VA -> refusal. Controls: no-order (no /ORDER),
  noref (/OPT:NOREF): each must exit 1. ld dir link/<A+B..>=ours-<U+V..>[-<control>]/.
  Exit 0 all equal, 1 any differ (prints the differing byte-offset count, never bytes), 2 refusal.
CLI: relink.py link | check --originals   (no --section, T5: whole image from originals)
  Refuses a missing/bad top cut/cut.complete. All 10 linked sections real, zero fillers (absolute aliases, if any, ride
  in a section-less absolute.obj); entry alias `_relink_entry` in the .text cut object at the retail entry; each
  section's last object padded to its PE VirtualSize. ld dir link/all/. check then writes the XBE in relink.py: the
  pin's header region [0, sizeOfHeaders) (entry, thunk, tls, peBaseAddr, peTimeDate, PE-header copy all the pin's;
  our PE entry + pe_base must equal the pin's entry), each linked section by xbe_write.write_section() with
  va/vsize/rawAddr/rawSize/digest rewritten in its section header, raw offsets and inserted files as xbe_write.write()
  -> link/all/relinked.xbe. Prints every section digest OK/FAIL, the image sha1 vs config/check.retail_xbe.sha,
  xbe_write.compare() names on a difference, and (info) our .reloc dir size and HIGHLOW count vs retail.
  Exit 0 all digests and the sha1 equal, 1 any differ, 2 refusal.
CLI: relink.py check [--control one-function|rdata-unsorted|entry|literal|fold|samename|weak]   (no --section, no --originals, T6: every matched
  function substituted)
  Builds our objects with build.build (units.toml, src; the gate's function) into .run/<phase>/relink/build (wiped
  first), then the whole-image link above with T4's swap route (as --units) for every config/matched_set.toml unit
  -> link/all=ours/relinked.xbe; prints the 16 digests OK/FAIL, the sha1 vs the pin and our symbols' XBE VAs
  (`$T318` from the linked DIR32 field, vs config/retail/symbols_pin.txt). --control one-function: one matched
  function's object copied to .run/<phase>/relink/control/, its final `ret` (C3, no reloc) -> `int3` (CC); prints
  symbol, VA, offset, opcode names; ld dir link/all=ours-one-function/; must exit 1 (one FAIL: .text).
  .rdata VA order (T12, any link with our .rdata data): every plain .rdata cut piece not past one of our `$` group
  holes, and every plain .rdata data section of ours (in a copy of our object, ld dir obj/ours/), is renamed
  `.rdata$<va8>` (its retail VA), so LINK's `$` sort places each at its VA; our objects are no longer placed by
  command-line order for .rdata (C0126 scattered items, C0133 adjacent literals). --control rdata-unsorted: that
  renaming off (the T7 order); ld dir link/all=ours-rdata-unsorted/; must exit 1 while a matched row relies on it.
  Exit 0 all equal, 1 any differ, 2 refusal.
Library (T7): gate_check(list_path, bdir, cfg_path=None, control=None) -> (rc, lines): the check above from the
  gate's ALREADY built objects in bdir (no build) for every function of list_path -> link/all=gate[-<control>]/;
  stdout+stderr captured; a refusal's SystemExit -> rc 2 (3.5 T4: a bare or non-int SystemExit too). The check/link path imports only verifier-locked modules
  (byte_compare's cut_check/_phase_run_dir, = delink's); delink is imported lazily by cut only.
C++ EH substitution (T7): a section of ours the matched functions reach (kept closure) that no symbol name places
  (`.xdata$x` FuncInfo `$T<n>`) gets its VA from a DIR32/REL32 site in an already-placed section of ours: retail
  field - our addend (REL32: + site + 4) - target symbol value; fixpoint; disagreeing or no sites -> refusal; each
  printed `ours: <obj> <sec> ... from reloc site`. The `.text$x` `__ehhandler$` stub is a .text hole (no /ORDER slot:
  static, it rides its group); `.xdata$x` a .rdata hole (LINK merges .xdata into .rdata itself, MERGED). LINK sorts
  `$`-groups after the plain pieces, so a cut piece past such a hole is renamed `<group>~` (`.text$x~`, `.xdata$x~`,
  long names as `/<strtab offset>`). After link every placed section and `$` static is checked at its VA (exit 1).
Folded members and shared literals (3.4 T6.1, C0136/C0135): a kept plain `.text` COMDAT code section of ours (our
  out-of-line copy of a folded member, `??_I`) takes its hole's /ORDER slot by its COMDAT symbol; our external naming
  another public at offset 0 of such a hole (C0130 alias pins) -> a weak external to that symbol (weak.obj, SEARCH_ALIAS),
  any other external in a hole still refused. Two of our objects' holes at one VA with the same COMDAT name, section,
  Selection (not NODUPLICATES/associative), bytes and relocs (to externals, by name) -> one hole, the copy rides it
  (T12 renames both); any other overlap still refused. Printed `identical duplicate holes ...; weak fold aliases ...`.
  4.3 T2: also two different names at one VA when both are func pins there (C0130 fold aliases): the dup's name
  renamed `<name>@fold_<va8>` in every copy of ours, weak to the kept name; relocs compared by target VA.
  --control entry: our first .text function moved to the head of /ORDER; must exit 2 at the PE-entry check.
  --control literal: two synthetic objects (.run/<phase>/relink/control/literal_{a,b}.obj), the first `__real@<16 hex>`
  name with one VA as a COMDAT (Selection ANY) literal at it (bytes from the name), b with byte 0 flipped; must exit 2
  `our ranges overlap`.
  --control fold: two synthetic objects (control/fold_{a,b}.obj), one COMDAT (Selection ANY) .text copy each under
  two fold-alias func pin names at the first such VA none of ours covers; one byte 0 flipped; must exit 2
  `our ranges overlap`.
  4.4 T3 (C0215): a same-name pair may have one matched row (its section from its defs). (C0217) a weak external
  (class 105) reloc target -> its default by aux TagIndex in the kept walk, the referenced set and reloc-site placement.
  --control samename: a copy of the object of the first matched row in a plain COMDAT .text (control/samename_<obj>),
  that section a code hole at the row's VA, one non-reloc byte flipped; must exit 2 `our ranges overlap`.
  --control weak: in a copy of a matched row's object, its first DIR32/REL32 to another external -> a new weak external
  `__weak_control` whose TagIndex names a non-row COMDAT .text no reloc reaches (a wrong body), planted in _ours before
  the kept walk so _weak_tag must follow it (4.5 T16.2.c4; was: after _ours, TagIndex the row itself); must fail.
  All ld dir link/all=<tag>-<control>/.
Evidence: docs/evidence/relink.md "One section relinked (T3)", "Two real sections, ordered COMDAT originals (T4.c1)",
"Whole image from originals (T5)", "Every matched function substituted (T6)", "C++ EH substitution (T7)", "3.4 T6.1".
"""
import argparse
import bisect
import hashlib
import json
import re
import shutil
import struct
import subprocess
import sys
import time
import tomllib
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import byte_compare  # noqa: E402  read-only import (verifier-locked): _phase_run_dir, cut_check (= delink's, T7)
import msvc71  # noqa: E402  read-only import (verifier-locked)
import pe_info  # noqa: E402
import vv_inputs  # noqa: E402  read-only import (verifier-locked)
import xbe_info  # noqa: E402  read-only import (verifier-locked)
import xbe_write  # noqa: E402

REPO = Path(__file__).resolve().parent.parent
PIN = REPO / "config" / "check.retail_xbe.sha"
UNITS = REPO / "config" / "retail" / "units.toml"
OUT = REPO / "derived" / "relink"
CLASSES = ("header", "pinned unit", "unpinned unit", "gap", "XDK section", ".rdata", ".data",
           "uninitialized tail", "inserted file")
XDK_SECTIONS = ("D3D", "D3DX", "XGRPH", "DSOUND", "XMV", "XPP", "DOLBY")


def refuse(msg):
    print(f"relink: {msg}", file=sys.stderr)
    sys.exit(2)


def load_xbe(cfg_path):
    """Read the retail XBE named by the config; refuse unless its sha1 equals the pin."""
    cfg = vv_inputs.load_config(cfg_path)
    path = vv_inputs.input_path("retail_xbe", cfg=cfg)
    data = Path(path).read_bytes()
    sha1 = hashlib.sha1(data).hexdigest()
    pin = PIN.read_text(encoding="utf-8").split()[0].lower()
    if sha1 != pin:
        refuse(f"retail XBE sha1 {sha1} != pin {pin} ({PIN})")
    try:
        x = xbe_info.parse_xbe(data)
    except xbe_info.NotXbe as e:
        refuse(f"not an XBE: {e}")
    return x, sha1


def regions_of(x):
    """Expected regions, header first then the section table order."""
    h = x["header"]
    out = [{"name": "header", "va": h["baseAddr"], "rawSize": h["sizeOfHeaders"],
            "vsize": h["sizeOfHeaders"], "flags": []}]
    for s in x["sections"]:
        out.append({"name": s["name"], "va": s["virtualAddr"], "rawSize": s["rawSize"],
                    "vsize": s["virtualSize"], "flags": list(s["flagNames"])})
    for r in out:
        r["end"] = r["va"] + max(r["rawSize"], r["vsize"])
        r["rawEnd"] = r["va"] + r["rawSize"]
    return out


def section_class(r):
    if "INSERTED_FILE" in r["flags"]:
        return "inserted file"
    if r["name"] in XDK_SECTIONS:
        return "XDK section"
    if r["name"] in (".rdata", ".data"):
        return r["name"]
    return None


def default_ranges(regions):
    """The default range list. Units are placed by the region wholly holding their text range:
    .text units split .text; XDK-section units are not ranges (returned in `xdk` for T5);
    a unit crossing a region edge, outside every region, or in another region -> `ufails`.
    Last value counts units with no text range (skipped)."""
    units = tomllib.loads(UNITS.read_text(encoding="utf-8"))["unit"]
    ranged = [u for u in units if "text_start" in u and "text_end" in u]
    text_units, xdk, ufails = [], [], []
    for u in sorted(ranged, key=lambda u: (u["text_start"], u["text_end"], u["name"])):
        a, b = u["text_start"], u["text_end"]
        home = [r for r in regions if r["va"] <= a and b <= r["end"]]
        if home and home[0]["name"] == ".text":
            text_units.append(u)
        elif home and home[0]["name"] in XDK_SECTIONS:
            xdk.append({"name": u["name"], "start": a, "end": b, "section": home[0]["name"],
                        "delink": bool(u.get("delink"))})
        else:
            where = f"inside {home[0]['name']}" if home else "crosses a region edge or lies outside every region"
            ufails.append(f"unit {u['name']} 0x{a:08X}-0x{b:08X}: {where}")
    ranges = []
    for r in regions:
        if r["name"] == "header":
            ranges.append((r["va"], r["va"] + r["rawSize"], "header", "header"))
        elif r["name"] == ".text":
            cur = r["va"]
            for u in text_units:
                if u["text_start"] > cur:
                    ranges.append((cur, u["text_start"], "gap", f"gap@{cur:08X}"))
                ranges.append((u["text_start"], u["text_end"],
                               "pinned unit" if u.get("delink") else "unpinned unit", u["name"]))
                cur = max(cur, u["text_end"])
            if cur < r["rawEnd"]:
                ranges.append((cur, r["rawEnd"], "gap", f"gap@{cur:08X}"))
        else:
            c = section_class(r)
            if c is None:
                refuse(f"section {r['name']!r} flags {'|'.join(r['flags']) or '-'} matches no class rule")
            ranges.append((r["va"], r["rawEnd"], c, r["name"]))
        if r["vsize"] > r["rawSize"]:
            ranges.append((r["rawEnd"], r["va"] + r["vsize"], "uninitialized tail", f"{r['name']}.tail"))
    return ranges, xdk, ufails, len(units) - len(ranged)


def read_objects(path):
    ranges = []
    for n, line in enumerate(Path(path).read_text(encoding="utf-8").splitlines(), 1):
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        f = line.split("\t")
        if len(f) != 4:
            refuse(f"{path}:{n}: want 4 TAB-separated fields, got {len(f)}")
        try:
            ranges.append((int(f[0], 16), int(f[1], 16), f[2], f[3]))
        except ValueError:
            refuse(f"{path}:{n}: start/end not hex")
    return ranges


def check(regions, ranges):
    """The one check: per-region coverage, overlaps, flagged ranges. Returns (report, failures)."""
    ranges = sorted(ranges, key=lambda t: (t[0], t[1], t[3]))
    fails, flagged = [], []
    per = {r["name"]: [] for r in regions}
    for a, b, c, n in ranges:
        why = []
        if b <= a:
            why.append("zero/negative length")
        if c not in CLASSES:
            why.append(f"class {c!r} not one of the 9")
        hit = [r for r in regions if a < r["end"] and r["va"] < b] if b > a else []
        if b > a and not hit:
            why.append("outside every region")
        elif len(hit) > 1:
            why.append("spans regions " + ",".join(r["name"] for r in hit))
        elif hit and not (hit[0]["va"] <= a and b <= hit[0]["end"]):
            why.append(f"extends outside region {hit[0]['name']}")
        for r in hit:
            per[r["name"]].append((max(a, r["va"]), min(b, r["end"]), c, n))
        if why:
            flagged.append({"start": a, "end": b, "class": c, "name": n, "why": why})
            fails.append(f"range 0x{a:08X}-0x{b:08X} {c!r} {n}: {'; '.join(why)}")
    report = []
    for r in regions:
        rs = per[r["name"]]
        raw = sum(max(0, min(b, r["rawEnd"]) - a) for a, b, _, _ in rs)
        total = sum(b - a for a, b, _, _ in rs)
        overlaps, covered, hi = [], 0, r["va"]
        for a, b, c, n in rs:  # already sorted
            if a < hi:
                overlaps.append(f"{n} 0x{a:08X}-0x{b:08X} overlaps up to 0x{hi:08X}")
            covered += max(0, b - max(a, hi))
            hi = max(hi, b)
        size = r["end"] - r["va"]
        classes = {}
        for _, _, c, _ in rs:
            classes[c] = classes.get(c, 0) + 1
        e = {"name": r["name"], "va": r["va"], "rawSize": r["rawSize"], "vsize": r["vsize"],
             "assignedRaw": raw, "assignedTotal": total, "overlaps": len(overlaps),
             "unassigned": size - covered, "classes": dict(sorted(classes.items()))}
        report.append(e)
        bad = []
        if raw != r["rawSize"]:
            bad.append(f"assigned raw 0x{raw:X} != rawSize 0x{r['rawSize']:X}")
        if total != r["vsize"]:
            bad.append(f"assigned total 0x{total:X} != vsize 0x{r['vsize']:X}")
        if overlaps:
            bad.append(f"{len(overlaps)} overlap(s): " + "; ".join(overlaps[:5]))
        if size - covered:
            bad.append(f"0x{size - covered:X} unassigned bytes")
        if bad:
            fails.append(f"region {r['name']}: " + "; ".join(bad))
    return report, ranges, flagged, fails


def census(args):
    x, sha1 = load_xbe(args.config)
    regions = regions_of(x)
    ranges, xdk, ufails, skipped = default_ranges(regions)
    src = "default"
    if args.objects:
        ranges, src, xdk, ufails = read_objects(args.objects), str(args.objects), [], []
    if args.control == "overlap":
        a, b, c, n = next(t for t in ranges if t[2] in ("pinned unit", "unpinned unit"))
        inj = (a, min(b, a + 0x10), "gap", "control-overlap")
        ranges = ranges + [inj]
        print(f"control overlap: injected 0x{inj[0]:08X}-0x{inj[1]:08X} {inj[2]!r} {inj[3]} (overlaps {n})")
    elif args.control == "drop":
        i = next(i for i, t in enumerate(ranges) if t[2] in ("pinned unit", "unpinned unit"))
        d = ranges[i]
        ranges = ranges[:i] + ranges[i + 1:]
        print(f"control drop: removed 0x{d[0]:08X}-0x{d[1]:08X} {d[2]!r} {d[3]}")
    report, ranges, flagged, fails = check(regions, ranges)
    fails = ufails + fails
    print(f"census: xbe sha1 {sha1[:8]}  ranges {len(ranges)} ({src})"
          + (f"  units without .text range skipped {skipped}" if src == "default" else ""))
    print(f"  {'region':10s} {'rawSize':>9s} {'vsize':>9s} {'asgRaw':>9s} {'asgTotal':>9s} ovl unassigned  classes")
    for e in report:
        cls = ", ".join(f"{k}={v}" for k, v in e["classes"].items())
        print(f"  {str(e['name']):10s} {e['rawSize']:9X} {e['vsize']:9X} {e['assignedRaw']:9X} "
              f"{e['assignedTotal']:9X} {e['overlaps']:3d} {e['unassigned']:10X}  {cls}")
    tot = {k: sum(e[k] for e in report) for k in ("rawSize", "vsize", "assignedRaw", "assignedTotal",
                                                  "overlaps", "unassigned")}
    print(f"  totals: rawSize 0x{tot['rawSize']:X} vsize 0x{tot['vsize']:X} assignedRaw 0x{tot['assignedRaw']:X} "
          f"assignedTotal 0x{tot['assignedTotal']:X} overlaps {tot['overlaps']} unassigned 0x{tot['unassigned']:X} "
          f"flagged {len(flagged)}")
    if src == "default":
        per = {}
        for u in xdk:
            per[u["section"]] = per.get(u["section"], 0) + 1
        print(f"  info: {len(xdk)} units lie inside XDK sections (not ranges; census.json unitsInXdkSections): "
              + (", ".join(f"{k} {v}" for k, v in per.items()) or "-"))
    if not args.objects and not args.control:
        out = OUT / f"retail-{sha1[:8]}" / "census.json"
        out.parent.mkdir(parents=True, exist_ok=True)
        doc = {"xbe_sha1": sha1, "regions": report, "totals": dict(tot, flagged=len(flagged)),
               "units_without_text_range": skipped, "unitsInXdkSections": xdk,
               "ranges": [{"start": a, "end": b, "class": c, "name": n} for a, b, c, n in ranges]}
        out.write_text(json.dumps(doc, indent=1) + "\n", encoding="utf-8")
        print(f"  wrote {out.relative_to(REPO).as_posix()}")
    if fails:
        print(f"census: FAIL ({len(fails)} item(s))")
        for f in fails:
            print(f"  {f}")
        return 1
    print("census: OK — every region exactly covered")
    return 0


def _section_ranges(x, name):
    """Census ranges of one section (raw-backed classes only; an uninitialized tail is no object). The ranges lying
    in the section's region: .text = its unit/gap ranges (T4), any other section its one range."""
    reg = next((r for r in regions_of(x) if r["name"] == name and r["name"] != "header"), None)
    if reg is None:
        refuse(f"no section named {name!r}")
    sel = [t for t in default_ranges(regions_of(x))[0]
           if reg["va"] <= t[0] and t[1] <= reg["rawEnd"] and t[2] != "uninitialized tail"]
    if not sel:
        refuse(f"no census range in {name!r}")
    return sel


def _cut_files(ranges):
    """Census ranges -> [(object file name, start VA, end VA)]; one range keeps the section name (T3)."""
    def unit_file(name):  # delink._unit_file, inline (T7): the check path imports no unlocked module
        stem = name.replace("\\", "__").replace("/", "__")
        return stem if stem.lower().endswith(".obj") else stem + ".obj"
    return [(unit_file(n if len(ranges) == 1 else f"{n}_{a:08X}"), a, b) for a, b, _c, n in ranges]


def _sec_dir(name):
    return name.replace("\\", "__").replace("/", "__")


def _do_cut(cfg, sha1, xbe, ranges, d):
    """Range-export ranges into d via DelinkProgram (existing project, -noanalysis -readOnly); hashes.txt and
    cut.complete last. -> list of object file names; exits 1 on a failed export."""
    import coff_info
    import delink  # cut only (T7: lazy, delink is not verifier-locked)
    tag = f"retail-{sha1[:8]}"
    proj = Path(cfg["ghidra_project_dir"]).resolve() / tag
    if not (proj / f"{tag}.gpr").is_file():
        refuse(f"project missing: {proj / (tag + '.gpr')} (run tools/delink.py ingest)")
    d.mkdir(parents=True, exist_ok=True)
    for p in [*d.glob("*.obj"), d / "hashes.txt", d / delink.CUT_MARKER]:
        if p.is_file():
            p.unlink()
    files = [f for f, _a, _b in _cut_files(ranges)]
    batches = [[]]  # one analyzeHeadless call per batch, each under delink.ARG_BUDGET (T4: .text 547 ranges)
    for f, a, b in _cut_files(ranges):
        one = ["/include-range", f"{a:08x}-{b - 1:08x}", "/export", str(d / f)]
        if len(subprocess.list2cmdline(one)) > delink.ARG_BUDGET:
            refuse(f"range {f} alone exceeds one call's argument budget")
        if batches[-1] and len(subprocess.list2cmdline(batches[-1] + one)) > delink.ARG_BUDGET:
            batches.append([])
        batches[-1] += one
    delink.LOGS.mkdir(parents=True, exist_ok=True)
    bat = Path(cfg["ghidra_dir"]) / "support" / "analyzeHeadless.bat"
    for k, args in enumerate(batches):
        log = delink.LOGS / f"relink_cut_{d.parent.name}_{d.name}_{k}.log"
        # T4.c2: RenameDuplicates first (in memory, -readOnly): DelinkProgram fails an object on a duplicate name
        cmd = [str(bat), str(proj), tag, "-process", xbe.name, "-noanalysis", "-readOnly",
               "-scriptPath", str(delink.SCRIPTS), "-postScript", "RenameDuplicates.java",
               "-postScript", "DelinkProgram.java", "/exporter", "COFF relocatable object", *args,
               "-log", str(log.with_suffix(".ghidra.log")), "-scriptlog", str(log.with_suffix(".script.log"))]
        with open(log, "w", encoding="utf-8") as lf:
            rc = subprocess.run(cmd, stdin=subprocess.DEVNULL, stdout=lf, stderr=subprocess.STDOUT).returncode
        text = log.read_text(encoding="utf-8", errors="replace")
        if rc != 0 or "REPORT SCRIPT ERROR" in text:
            print(f"relink: FAIL analyzeHeadless rc {rc} (see {log})", file=sys.stderr)
            sys.exit(1)
    print(f"cut {d.name}: {len(files)} range(s) in {len(batches)} analyzeHeadless call(s)")
    for f in files:
        try:
            coff_info.parse((d / f).read_bytes())
        except (OSError, coff_info.BadCoff, struct.error, ValueError) as e:
            print(f"relink: FAIL object {d / f}: {e}", file=sys.stderr)
            sys.exit(1)
    hashes = "".join(f"{hashlib.sha256((d / f).read_bytes()).hexdigest()}  {f}\n" for f in sorted(files))
    (d / "hashes.txt").write_text(hashes, encoding="ascii", newline="\n")
    delink.write_marker(d, len(files))
    return files


def _partition(x, names):
    """partition.tsv text (census --objects format): every cut object range of the sections `names` (name
    `<NAME>/<file>`) + the non-cut census ranges (header, inserted file, uninitialized tail), in VA order."""
    rows = []
    for n in names:
        rng = _section_ranges(x, n)
        rows += [(a, b, t[2], f"{_sec_dir(n)}/{f}") for (f, a, b), t in zip(_cut_files(rng), rng)]
    rows += [t for t in default_ranges(regions_of(x))[0] if t[2] in ("header", "inserted file", "uninitialized tail")]
    return ("# relink.py cut (T5): cut objects + non-cut ranges; check with relink.py census --objects\n"
            + "".join(f"{a:08X}\t{b:08X}\t{c}\t{n}\n" for a, b, c, n in sorted(rows)))


def _top_lines(top, names):
    """Top cut/hashes.txt lines: sha256 of each section's hashes.txt and of partition.tsv."""
    fs = [f"{_sec_dir(n)}/hashes.txt" for n in names] + ["partition.tsv"]
    return [f"{hashlib.sha256((top / f).read_bytes()).hexdigest()}  {f}" for f in fs]


def _read_top(root, names):
    """Whole-image cut: refuses unless cut/cut.complete is valid and every file its hashes.txt names is unchanged."""
    top = root / "cut"
    why = byte_compare.cut_check(top)
    if why:
        refuse(why)
    have = (top / "hashes.txt").read_text(encoding="ascii").splitlines()
    want = [f"{_sec_dir(n)}/hashes.txt" for n in names] + ["partition.tsv"]
    if [l.split("  ", 1)[1] for l in have] != want:
        refuse(f"{top / 'hashes.txt'} does not list the whole-image cut (run relink.py cut)")
    if _top_lines(top, names) != have:
        refuse(f"{top}: a section cut or partition.tsv changed after the whole cut (run relink.py cut)")


def cut(args):
    import delink  # cut only (T7: lazy, delink is not verifier-locked)
    cfg = vv_inputs.load_config(args.config)
    x, sha1 = load_xbe(args.config)
    xbe = vv_inputs.input_path("retail_xbe", cfg=cfg)
    root = OUT / f"retail-{sha1[:8]}"
    layout = _retail_layout(Path(xbe).read_bytes())[0]
    names = [args.section] if args.section else [s["name"] for s in layout]  # T5: no --section = every linked section
    t5 = REPO / "derived" / "ghidra" / f"retail-{sha1[:8]}"
    by_name = _name_vas(t5)
    got = []
    for top in [root / "cut"] + ([root / "cut.repro"] if args.repro else []):
        t0, nobj = time.monotonic(), 0
        if not args.section:  # the top marker goes first, so a cut stopped midway leaves no valid whole cut
            for p in (top / delink.CUT_MARKER, top / "hashes.txt", top / "partition.tsv"):
                if p.is_file():
                    p.unlink()
        for name in names:
            d = top / _sec_dir(name)
            files = _do_cut(cfg, sha1, xbe, _section_ranges(x, name), d)
            nobj += len(files)
            h = (d / "hashes.txt").read_bytes()
            sizes = (", ".join(f"{f} {(d / f).stat().st_size} B" for f in files) if len(files) <= 4 else
                     f"total {sum((d / f).stat().st_size for f in files)} B")
            ext = [e for f in files for e in _externals(d / f, layout, t5, by_name)]
            per = {}
            for _n, _va, s in ext:
                per[s or "absolute"] = per.get(s or "absolute", 0) + 1
            print(f"cut {d.relative_to(REPO).as_posix()}: {len(files)} object(s) ({sizes}) "
                  f"hashes.txt sha256 {hashlib.sha256(h).hexdigest()}")
            print(f"  externals {len(ext)}: " + (", ".join(f"{k} {v}" for k, v in sorted(per.items())) or "-"))
            if args.section:
                got.append(h)
        if not args.section:
            (top / "partition.tsv").write_text(_partition(x, names), encoding="ascii", newline="\n")
            lines = _top_lines(top, names)
            (top / "hashes.txt").write_text("".join(f"{l}\n" for l in lines), encoding="ascii", newline="\n")
            delink.write_marker(top, len(lines))
            h = (top / "hashes.txt").read_bytes()
            got.append(h)
            print(f"cut {top.relative_to(REPO).as_posix()}: {len(names)} sections, {nobj} objects, "
                  f"{time.monotonic() - t0:.0f}s; partition.tsv sha256 "
                  f"{hashlib.sha256((top / 'partition.tsv').read_bytes()).hexdigest()}; top hashes.txt sha256 "
                  f"{hashlib.sha256(h).hexdigest()}")
    if args.repro and got[0] != got[1]:
        print("cut --repro: FAIL hashes.txt differ between the two cuts")
        return 1
    if args.repro:
        print("cut --repro: OK hashes.txt equal")
    return 0


GHIDRA_NAME = re.compile(r"(?:[A-Za-z]+_)+([0-9a-fA-F]{8})")
FILLER_ALIGN = 0x00300000  # IMAGE_SCN_ALIGN_4BYTES; one contribution per output section, which starts 0x20-aligned
ENTRY_SYM = "_relink_entry"  # filler symbol at the retail entry, /ENTRY:relink_entry
LINK_FLAGS = ("/NOLOGO", "/MACHINE:X86", "/SUBSYSTEM:XBOX",
              "/FIXED:NO", "/INCREMENTAL:NO", "/OPT:NOREF", "/OPT:NOICF", "/NODEFAULTLIB")


def _retail_layout(rx):
    """Linked retail sections (pin order) -> list of {name, va (XBE), vsize (PE), chars (PE)} from the XBE's
    PE-header copy; plus the XBE entry VA."""
    x = xbe_info.parse_xbe(rx)
    h = x["header"]
    po = h["peBaseAddr"] - h["baseAddr"]
    hdr = rx[po:]
    pe_off = struct.unpack_from("<I", hdr, 0x3C)[0]
    nsec, optsz = struct.unpack_from("<H", hdr, pe_off + 6)[0], struct.unpack_from("<H", hdr, pe_off + 20)[0]
    sh = pe_off + 24 + optsz
    xs = {s["name"]: s for s in x["sections"] if "INSERTED_FILE" not in s["flagNames"]}
    out = []
    for k in range(nsec):
        o = sh + 40 * k
        n = hdr[o:o + 8].rstrip(b"\0").decode("latin-1")
        if n in xs:
            out.append({"name": n, "va": xs[n]["virtualAddr"], "vsize": struct.unpack_from("<I", hdr, o + 8)[0],
                        "chars": struct.unpack_from("<I", hdr, o + 36)[0]})
    return out, x["entry"]["va"]


def _name_vas(t5):
    """symbols.jsonl -> {Ghidra export name (`ns::name` or `name`): set of VAs}."""
    by_name = {}
    with open(t5 / "applied" / "symbols.jsonl", encoding="utf-8") as f:  # delink.load_jsonl, inline (T7)
        recs = [json.loads(line) for line in f if line.strip()]
    for r in recs:
        q = f"{r['namespace']}::{r['name']}" if r.get("namespace") else r["name"]  # Ghidra exports ns::name
        by_name.setdefault(q, set()).add(int(r["va"], 16))
    return by_name


def _rule_vas(n, by_name):
    """T3's name rules: `<prefix>_<hex8>` -> that VA, else the symbols.jsonl VAs of the name."""
    m = GHIDRA_NAME.fullmatch(n)
    return {int(m.group(1), 16)} if m else by_name.get(n, set())


def _sec_at(layout, va):
    return next((s for s in layout if s["va"] <= va < s["va"] + s["vsize"]), None)


def _externals(obj, layout, t5, by_name=None):
    """Undefined externals of a cut object -> list of (name, target VA, section name or None = absolute); a name
    with != 1 VA by the name rules -> (name, None, "ambiguous") (the link resolves it per reloc site)."""
    import coff_info
    c = coff_info.parse(Path(obj).read_bytes())
    names = sorted({y["name"] for y in c["symbols"].values() if y["section"] == 0 and y["class"] == 2})
    by_name = _name_vas(t5) if by_name is None else by_name
    out = []
    for n in names:
        vas = _rule_vas(n, by_name)
        if len(vas) != 1:
            out.append((n, None, "ambiguous"))
            continue
        va = next(iter(vas))
        s = _sec_at(layout, va)
        out.append((n, va, s["name"] if s else None))
    return out


def _coff(sections, symbols):
    """Minimal i386 COFF: sections [(name, chars, size)] zero-filled; symbols [(name, value, section#, type)]
    external (section -1 = absolute)."""
    hdr_end = 20 + 40 * len(sections)
    raw, pos = [], hdr_end
    for _n, _c, size in sections:
        raw.append(pos)
        pos += size
    strtab, syms = bytearray(), bytearray()

    def name8(n):
        b = n.encode("ascii")
        if len(b) <= 8:
            return b.ljust(8, b"\0")
        off = 4 + len(strtab)
        strtab.extend(b + b"\0")
        return struct.pack("<II", 0, off)

    for k, (n, _c, _s) in enumerate(sections, 1):
        syms += name8(n) + struct.pack("<IhHBB", 0, k, 0, 3, 0)
    for n, v, sec, typ in symbols:
        syms += name8(n) + struct.pack("<IhHBB", v, sec, typ, 2, 0)
    nsym = len(sections) + len(symbols)
    out = bytearray(struct.pack("<HHIIIHH", 0x14C, len(sections), 0, pos, nsym, 0, 0))
    for (n, c, size), rp in zip(sections, raw):
        out += name8(n) + struct.pack("<IIIIIIHHI", 0, 0, size, rp, 0, 0, 0, 0, c)
    for _n, _c, size in sections:
        out += bytes(size)
    return bytes(out + syms + struct.pack("<I", 4 + len(strtab)) + strtab)


def _weak_coff(pairs):
    """3.4 T6.1: section-less i386 COFF, per (alias, default): the default an undefined external, the alias a weak
    external (class 105, one aux: TagIndex = the default's index, Characteristics 3 SEARCH_ALIAS)."""
    strtab, syms = bytearray(), bytearray()

    def name8(n):
        b = n.encode("ascii")
        if len(b) <= 8:
            return b.ljust(8, b"\0")
        off = 4 + len(strtab)
        strtab.extend(b + b"\0")
        return struct.pack("<II", 0, off)

    for k, (alias, default) in enumerate(pairs):
        syms += name8(default) + struct.pack("<IhHBB", 0, 0, 0x20, 2, 0)
        syms += name8(alias) + struct.pack("<IhHBB", 0, 0, 0x20, 105, 1) + struct.pack("<II", 3 * k, 3).ljust(18, b"\0")
    return bytes(struct.pack("<HHIIIHH", 0x14C, 0, 0, 20, 3 * len(pairs), 0, 0) + syms
                 + struct.pack("<I", 4 + len(strtab)) + strtab)


def _read_cut(root, x, name):
    """Finished cut of one section -> [(object path, start VA, end VA)] in VA order; refuses a bad marker or a dir
    holding another cut."""
    d = root / "cut" / _sec_dir(name)
    why = byte_compare.cut_check(d)
    if why:
        refuse(why)
    want = _cut_files(_section_ranges(x, name))
    listed = [l.split("  ", 1)[1] for l in (d / "hashes.txt").read_text(encoding="ascii").splitlines()]
    if listed != sorted(f for f, _a, _b in want):
        refuse(f"{d} holds {len(listed)} object(s), not the {name} cut (run relink.py cut --section {name})")
    return [(d / f, a, b) for f, a, b in want]


def _obj_load(data):
    """COFF object -> editable form: sections {name8, vsz, va, size, raw, relocs [[offset, entry, type]], chars};
    syms = entries [name8, value, section, type, class, aux bytes] (relocs point at entries, not table indices);
    strtab with its size word."""
    _m, nsec, ts, symptr, nsym, optsz, ch = struct.unpack_from("<HHIIIHH", data, 0)
    if optsz:
        refuse("cut object with an optional header")
    strpos = symptr + 18 * nsym
    strtab = bytearray(data[strpos:strpos + struct.unpack_from("<I", data, strpos)[0]])
    ents, ent_of, i = [], {}, 0
    while i < nsym:
        o = symptr + 18 * i
        v, sec, typ, cls, naux = struct.unpack_from("<IhHBB", data, o + 8)
        ent_of[i] = len(ents)
        ents.append([bytes(data[o:o + 8]), v, sec, typ, cls, bytes(data[o + 18:o + 18 + 18 * naux])])
        i += 1 + naux
    secs = []
    for k in range(nsec):
        o = 20 + 40 * k
        vsz, va, rsz, rptr, relptr, lnptr, nrel, nl, sch = struct.unpack_from("<IIIIIIHHI", data, o + 8)
        if lnptr or nl:
            refuse("cut object with line numbers")
        rel = [list(struct.unpack_from("<IIH", data, relptr + 10 * r)) for r in range(nrel)]
        for r in rel:
            r[1] = ent_of[r[1]]
        secs.append({"name8": bytes(data[o:o + 8]), "vsz": vsz, "va": va, "size": rsz,
                     "raw": bytes(data[rptr:rptr + rsz]) if rptr else None, "relocs": rel, "chars": sch})
    return {"ts": ts, "chars": ch, "secs": secs, "syms": ents, "strtab": strtab}


def _weak_tag(o, e):
    """4.4 T3 (C0217): a weak external entry (class 105) -> its default's entry (aux TagIndex, a raw symbol table
    index), else e itself (`??_E` vtable slot -> `??_G`)."""
    if e[4] != 105:
        return e
    if len(e[5]) < 4:
        refuse(f"weak external {_obj_name(o, e[0])} without an aux record")
    k, n = struct.unpack_from("<I", e[5], 0)[0], 0
    for f in o["syms"]:
        if n == k:
            return f
        n += 1 + len(f[5]) // 18
    refuse(f"weak external {_obj_name(o, e[0])}: TagIndex {k} is no symbol entry")


def _obj_name(o, name8):
    if name8[:4] == b"\0\0\0\0":
        off = struct.unpack_from("<I", name8, 4)[0]
        return bytes(o["strtab"][off:o["strtab"].index(b"\0", off)]).decode("latin-1")
    if name8[:1] == b"/" and name8[1:2].isdigit():  # section header long name
        off = int(name8[1:].rstrip(b"\0"))
        return bytes(o["strtab"][off:o["strtab"].index(b"\0", off)]).decode("latin-1")
    return name8.rstrip(b"\0").decode("latin-1")


def _obj_name8(o, name):
    b = name.encode("latin-1")
    if len(b) <= 8:
        return b.ljust(8, b"\0")
    off = len(o["strtab"])
    o["strtab"] += b + b"\0"
    return struct.pack("<II", 0, off)


def _obj_dump(o):
    """Editable form -> COFF bytes (table indices recomputed from the entries)."""
    idx, n = [], 0
    for e in o["syms"]:
        idx.append(n)
        n += 1 + len(e[5]) // 18
    pos = 20 + 40 * len(o["secs"])
    hdrs, body = bytearray(), bytearray()
    for s in o["secs"]:
        if len(s["relocs"]) > 0xFFFF:
            refuse("more than 65535 relocations in one section")
        rptr = pos + len(body) if s["raw"] is not None and s["size"] else 0
        if rptr:
            body += s["raw"]
        relptr = pos + len(body) if s["relocs"] else 0
        for off, e, t in s["relocs"]:
            body += struct.pack("<IIH", off, idx[e], t)
        hdrs += s["name8"] + struct.pack("<IIIIIIHHI", s["vsz"], s["va"], s["size"], rptr, relptr, 0,
                                         len(s["relocs"]), 0, s["chars"])
    symptr = pos + len(body)
    syms = b"".join(e[0] + struct.pack("<IhHBB", e[1], e[2], e[3], e[4], len(e[5]) // 18) + e[5] for e in o["syms"])
    st = bytearray(o["strtab"])
    struct.pack_into("<I", st, 0, len(st))
    return (struct.pack("<HHIIIHH", 0x14C, len(o["secs"]), o["ts"], symptr, n, 0, o["chars"])
            + bytes(hdrs) + bytes(body) + syms + bytes(st))


COMDAT_PREFIX = "__rl_"  # T4: one NODUPLICATES COMDAT per .text cut object, symbol __rl_<start VA hex8> at 0
REL_DIR32, REL_REL32 = 0x6, 0x14


def _comdat(o, num, sym):
    """Section num of o -> COMDAT (LNK_COMDAT, ALIGN_1, aux Selection 1 NODUPLICATES); external COMDAT symbol sym
    at 0 right after the section symbol. Call last: the inserted entry shifts later entries."""
    s = o["secs"][num - 1]
    s["chars"] = (s["chars"] & ~0x00F00000) | 0x00001000 | 0x00100000
    sname = _obj_name(o, s["name8"])
    k = next((i for i, e in enumerate(o["syms"]) if e[2] == num and e[4] == 3 and e[1] == 0
              and _obj_name(o, e[0]) == sname), None)
    if k is None:
        refuse(f"no section symbol for {sname}")
    aux = bytearray(o["syms"][k][5][:18].ljust(18, b"\0"))
    struct.pack_into("<IHH", aux, 0, s["size"], len(s["relocs"]), 0)
    struct.pack_into("<HB", aux, 12, 0, 1)  # Number 0, Selection 1 NODUPLICATES (C0032: byte 14)
    o["syms"][k][5] = bytes(aux) + o["syms"][k][5][18:]
    o["syms"].insert(k + 1, [_obj_name8(o, sym), 0, num, 0x20, 2, b""])
    for t in o["secs"]:
        for r in t["relocs"]:
            if r[1] > k:
                r[1] += 1


MATCHED = REPO / "config" / "matched_set.toml"
PINS = REPO / "config" / "retail" / "symbols_pin.txt"
SCN_CODE, SCN_SKIP = 0x20, 0x200 | 0x800 | 0x02000000  # LNK_INFO | LNK_REMOVE | MEM_DISCARDABLE: not image data
MERGED = {".xdata": ".rdata"}  # T7: retail .rdata holds the C++ EH FuncInfo tables (`.xdata$x`, C0072); no .xdata


def _our_names(cfg_path):
    """Names for our objects' externals: byname exactly as byte_compare.setup builds it (Ghidra applied tables)."""
    import types
    import byte_compare
    try:
        return byte_compare.setup(types.SimpleNamespace(symbols=None, xbe=None, config=cfg_path))[1]
    except byte_compare.Refuse as e:
        refuse(f"byte_compare.setup: {e}")


def _retail_bytes(cfg_path):
    x, _sha1 = load_xbe(cfg_path)
    return x, vv_inputs.input_path("retail_xbe", cfg=vv_inputs.load_config(cfg_path)).read_bytes()


def _placed(path, layout, sname, va, t, num):
    """One of our sections at retail VA va -> {sec (our name), rsec (retail section), va, size, num}; refuses unless
    the retail section is the one LINK puts sname in (its name before `$`, or MERGED)."""
    sec = _sec_at(layout, va)
    base = sname.split("$")[0]
    if sec is None or sec["name"] != MERGED.get(base, base) or va + t["size"] > sec["va"] + sec["vsize"]:
        refuse(f"{path.name}: {sname} at 0x{va:08X} lies in retail {sec and sec['name']}")
    return {"sec": sname, "rsec": sec["name"], "va": va, "size": t["size"], "num": num}


def _weak_plant(o, path, mine, defs):
    """4.5 T16.2.c4 control weak (C0217): one matched row's DIR32/REL32 to another external -> a new weak external
    `__weak_control` (class 105) whose TagIndex names a non-row COMDAT .text of the same object that no reloc
    reaches (kept only if _weak_tag follows the weak; the call then lands on a wrong body). Copy written to
    .run/<phase>/relink/control/ -> its path, or None (no such row and section in this object)."""
    rowsec = {defs[r["symbol"]][0] for r in mine}
    hit = {o["syms"][k][2] for t in o["secs"] if not _obj_name(o, t["name8"]).startswith(".debug")
           for _f, k, _t in t["relocs"]}
    cand = [(i, e) for i, e in enumerate(o["syms"]) if e[4] == 2 and e[2] > 0 and e[1] == 0
            and e[2] not in rowsec and e[2] not in hit and o["secs"][e[2] - 1]["chars"] & 0x1000
            and _obj_name(o, o["secs"][e[2] - 1]["name8"]) == ".text"
            and not any(f[2] == e[2] for f in o["syms"] if f is not e and f[4] == 2)]
    for r in mine:
        num = defs[r["symbol"]][0]
        ri = next((i for i, x in enumerate(o["secs"][num - 1]["relocs"]) if x[2] in (REL_DIR32, REL_REL32)
                   and o["syms"][x[1]][4] == 2 and o["syms"][x[1]][2] == 0
                   and _obj_name(o, o["syms"][x[1]][0]) != r["symbol"]), None)
        if ri is None or not cand:
            continue
        cd = byte_compare._phase_run_dir() / "relink" / "control"
        if cd.exists():
            shutil.rmtree(cd)
        cd.mkdir(parents=True)
        k, de = cand[0]
        rl = o["secs"][num - 1]["relocs"][ri]
        old = _obj_name(o, o["syms"][rl[1]][0])
        tag = sum(1 + len(e[5]) // 18 for e in o["syms"][:k])
        o["syms"].append([_obj_name8(o, "__weak_control"), 0, 0, 0x20, 105,
                          struct.pack("<II", tag, 3).ljust(18, b"\0")])
        rl[1] = len(o["syms"]) - 1
        q = cd / path.name
        q.write_bytes(_obj_dump(o))
        print(f"control weak: {r['symbol']} +0x{rl[0]:X} reloc to {old} -> weak __weak_control, TagIndex {tag} = "
              f"{_obj_name(o, de[0])} (section #{de[2]}, no reloc reaches it; wrong body), object "
              f"{q.relative_to(REPO).as_posix()}")
        return q
    return None


def _ours(units, cfg_path, layout, bdir=None, list_path=None, control=None):
    """--units U[,V..] -> our compiled objects (bdir or .run/<phase>/build/<basename of the matched-set unit>) with
    their matched rows, undefined externals and image data sections ({sec, va, size, num}: VA from a symbol in the
    section by name, byname then config/retail/symbols_pin.txt). list_path: matched list (default MATCHED; T7 gate).
    control weak (4.5 T16.2.c4): _weak_plant on the first object that allows it, before the kept walk."""
    import coff_info
    from pathlib import PureWindowsPath
    rows = tomllib.loads(Path(list_path or MATCHED).read_text(encoding="utf-8")).get("function", [])
    stems = {}
    for r in rows:
        stems.setdefault(PureWindowsPath(r["unit"]).stem, set()).add(r["unit"])
    byname = _our_names(cfg_path)
    pins = coff_info.load_names([PINS])[0]
    objs, rx, planted = [], None, False  # rx: (x, retail bytes), read on the first section only a reloc site places (T7)
    for u in units.split(","):
        s = stems.get(u, set())
        if len(s) != 1:
            refuse(f"unit {u!r}: {len(s)} matched-set units with that basename stem {sorted(s)}")
        unit = next(iter(s))
        path = (Path(bdir) if bdir else byte_compare._phase_run_dir() / "build") / PureWindowsPath(unit).name
        if not path.is_file():
            refuse(f"our object missing: {path} (run tools/build.py --src src --out {path.parent})")
        o = _obj_load(path.read_bytes())
        defs, und = {}, []
        for e in o["syms"]:
            if e[4] == 2 and e[2] > 0:
                defs[_obj_name(o, e[0])] = (e[2], e[1])
            elif e[4] == 2 and e[2] == 0:
                if e[1]:
                    refuse(f"{path.name}: common symbol {_obj_name(o, e[0])}")
                und.append(_obj_name(o, e[0]))
        mine = [r for r in rows if r["unit"] == unit]
        for r in mine:
            num, v = defs.get(r["symbol"], (0, None))
            t = o["secs"][num - 1] if num else None
            if t is None or v != 0 or not t["chars"] & 0x1000 or t["size"] != r["size"] \
                    or _obj_name(o, t["name8"]) != ".text":
                refuse(f"{path.name}: {r['symbol']} is not a COMDAT .text section of 0x{r['size']:X} bytes at 0")
        if control == "weak" and not planted:  # 4.5 T16.2.c4: planted before the kept walk, so _weak_tag sees it
            q = _weak_plant(o, path, mine, defs)
            if q is not None:
                path, o, planted = q, _obj_load(q.read_bytes()), True
        kept = {defs[r["symbol"]][0] for r in mine}  # T6: sections reachable from our matched functions (in-object
        todo = list(kept)  # relocs; /OPT:REF keeps no other COMDAT of ours unless something else references it)
        while todo:
            for _off, k, _t in o["secs"][todo.pop() - 1]["relocs"]:
                e = _weak_tag(o, o["syms"][k])  # 4.4 T3: a weak external -> its default (C0217 `??_E` -> `??_G`)
                s = e[2] if e[2] > 0 else defs.get(_obj_name(o, e[0]), (0,))[0]
                if s > 0 and s not in kept:
                    kept.add(s)
                    todo.append(s)
        rowsec = {defs[r["symbol"]][0] for r in mine}
        data, code, pending = [], [], {}  # pending: kept sections no symbol name places (T7: reloc sites do)
        refd = {o["syms"][r[1]][2] if o["syms"][r[1]][4] != 105 else  # sections some reloc targets; 4.4 T3: a weak
                (lambda f: f[2] if f[2] > 0 else defs.get(_obj_name(o, f[0]), (0,))[0])(_weak_tag(o, o["syms"][r[1]]))
                for t in o["secs"] if not _obj_name(o, t["name8"]).startswith(".debug")
                for r in t["relocs"]}  # one's default (C0217: `??_E` -> an undefined `??_G` entry, defined by name)
        nunref = 0
        for num, t in enumerate(o["secs"], 1):
            sname = _obj_name(o, t["name8"])
            if sname.startswith(".debug") or not t["size"] or t["chars"] & SCN_SKIP \
                    or t["chars"] & SCN_CODE and (num in rowsec or num not in kept):
                continue  # T7: a kept code section that is no matched function (`.text$x` __ehhandler$ stub) is placed
            sk = next((e for e in o["syms"] if e[2] == num and e[4] == 3 and e[1] == 0
                       and _obj_name(o, e[0]) == sname), None)
            if t["chars"] & 0x1000 and num not in refd and sk is not None and len(sk[5]) >= 18 and sk[5][14] != 5:
                nunref += 1  # T6: unreferenced non-associative COMDAT data (SoundNode.obj: 43 std::_Iosb statics
                continue  # from the iostream headers): /OPT:REF drops it, no image data, no hole
            vas = set()
            for e in o["syms"]:
                nm = _obj_name(o, e[0])
                if e[2] != num or e[4] not in (2, 3) or (e[4] == 3 and e[1] == 0 and nm == sname):
                    continue
                va = next(iter(byname[nm])) if len(byname.get(nm, ())) == 1 else \
                    (next(iter(pins[nm])) if len(pins.get(nm, ())) == 1 else None)
                if va is not None:
                    vas.add(va - e[1])
            if not vas and num in kept:
                pending[num] = sname
                continue
            if len(vas) != 1:
                refuse(f"{path.name}: data section {sname} (#{num}): {len(vas)} VAs from its symbols by name")
            (code if t["chars"] & SCN_CODE else data).append(_placed(path, layout, sname, next(iter(vas)), t, num))
        secva = {defs[r["symbol"]][0]: r["va"] for r in mine} | {d["num"]: d["va"] for d in data + code}
        while pending:  # T7: a section no name places -> its VA from a reloc site in a placed section of ours
            if rx is None:
                rx = _retail_bytes(cfg_path)
            got = {}
            for num, sname in pending.items():
                for p, pva in sorted(secva.items()):
                    t = o["secs"][p - 1]
                    for off, k, typ in t["relocs"]:
                        e = _weak_tag(o, o["syms"][k])  # 4.4 T3: a weak external -> its default (C0217)
                        s, v = (e[2], e[1]) if e[2] > 0 else defs.get(_obj_name(o, e[0]), (0, 0))
                        if s != num:
                            continue
                        if typ not in (REL_DIR32, REL_REL32):
                            refuse(f"{path.name}: reloc type 0x{typ:X} at {_obj_name(o, t['name8'])}+0x{off:X} "
                                   f"into {sname} (#{num})")
                        site = pva + off
                        fo = xbe_info.va_to_off(rx[0], site)
                        if fo is None:
                            refuse(f"{path.name}: reloc site 0x{site:08X} into {sname} not raw-backed")
                        ret, field = struct.unpack_from("<I", rx[1], fo)[0], struct.unpack_from("<I", t["raw"], off)[0]
                        sv = (ret - field) if typ == REL_DIR32 else (ret + site + 4 - field)
                        got.setdefault(num, {}).setdefault((sv - v) & 0xFFFFFFFF, []).append(
                            f"{_obj_name(o, t['name8'])}+0x{off:X} {'DIR32' if typ == REL_DIR32 else 'REL32'} "
                            f"{_obj_name(o, e[0])} at 0x{site:08X}")
            if not got:
                refuse(f"{path.name}: sections {', '.join(f'{n} (#{k})' for k, n in sorted(pending.items()))}: "
                       f"0 VAs from their symbols by name or from reloc sites in placed sections")
            for num, vas in sorted(got.items()):
                sname, t = pending.pop(num), o["secs"][num - 1]
                if len(vas) != 1:
                    refuse(f"{path.name}: {sname} (#{num}): reloc sites disagree: "
                           + "; ".join(f"0x{va:08X} from {w[0]}" for va, w in sorted(vas.items())))
                va, why = next(iter(vas.items()))
                d = _placed(path, layout, sname, va, t, num)
                (code if t["chars"] & SCN_CODE else data).append(d)
                secva[num] = va
                print(f"ours: {path.name} {sname} (#{num}, 0x{t['size']:X} B) at 0x{va:08X} in retail {d['rsec']} "
                      f"from reloc site {why[0]}" + (f" (+{len(why) - 1} agreeing)" if len(why) > 1 else ""))
        objs.append({"unit": unit, "path": path, "o": o, "rows": mine, "defs": defs, "und": und, "data": data,
                     "code": code, "unref": nunref, "kept": kept})
    return {"objs": objs, "byname": byname, "pins": pins, "units": units}


def _split(c, holes):
    """Cut c -> piece cuts at the hole edges; holes (VA ranges inside c) get no piece. A reference crossing pieces
    goes to an undefined external: its own name (external) or `rl_s_<va8>` (static; the owning piece defines it,
    T3's `<prefix>_<hex8>` name rule gives its VA when that piece is a hole)."""
    o, num, a, b = c["o"], c["num"], c["a"], c["b"]
    sec = o["secs"][num - 1]
    if any(t["relocs"] for k, t in enumerate(o["secs"], 1) if k != num):
        refuse(f"{c['path'].name}: relocations outside its {c['sec']} section")
    sname = _obj_name(o, sec["name8"])
    ssym = next((k for k, e in enumerate(o["syms"]) if e[2] == num and e[4] == 3 and e[1] == 0
                 and _obj_name(o, e[0]) == sname), None)
    if ssym is None:
        refuse(f"{c['path'].name}: no section symbol for {sname}")
    bounds = sorted({a, b, *(h["lo"] for h in holes), *(h["hi"] for h in holes)})
    pieces = list(zip(bounds, bounds[1:]))
    hole = {(h["lo"], h["hi"]) for h in holes}

    def owner(va):  # 4.4 T3: bisect over the contiguous pieces (was a linear scan; same result)
        i = bisect.bisect_right(bounds, va) - 1
        return pieces[i] if 0 <= i < len(pieces) else pieces[-1] if va == b else None

    def va_of(k):
        return a + o["syms"][k][1]
    sown = {k: owner(va_of(k)) for k, e in enumerate(o["syms"]) if e[2] == num}  # 4.4 T3: owner per symbol, once
    crossing = set()  # VAs of statics referenced from another piece
    for off, k, _t in sec["relocs"]:
        e = o["syms"][k]
        if e[2] == num and e[4] != 2 and sown[k] != owner(a + off):
            crossing.add(va_of(k))
        if e[2] == num and k == ssym and owner(a + off) != pieces[0]:
            crossing.add(a)
    cown = {v: owner(v) for v in crossing}
    out = []
    for lo, hi in pieces:
        if (lo, hi) in hole:
            continue
        p = {"ts": o["ts"], "chars": o["chars"], "strtab": bytearray(o["strtab"]), "syms": [], "secs": []}
        new = {}
        for k, e in enumerate(o["syms"]):
            if e[2] == num and k != ssym and sown[k] != (lo, hi):
                continue
            new[k] = len(p["syms"])
            p["syms"].append([e[0], e[1] - (lo - a) if e[2] == num and k != ssym else e[1], *e[2:]])
        k0 = new[ssym]
        aux = bytearray(p["syms"][k0][5][:18].ljust(18, b"\0"))
        for va in sorted(v for v in crossing if cown[v] == (lo, hi)):
            p["syms"].append([_obj_name8(p, f"rl_s_{va:08X}"), va - lo, num, 0, 2, b""])
        ext = {}
        relocs = []
        for off, k, t in sec["relocs"]:
            if not lo <= a + off < hi:
                continue
            e = o["syms"][k]
            if e[2] == num and (k == ssym and lo != a or k != ssym and sown[k] != (lo, hi)):
                nm = _obj_name(o, e[0]) if e[4] == 2 else f"rl_s_{va_of(k) if k != ssym else a:08X}"
                if nm not in ext:
                    ext[nm] = len(p["syms"])
                    p["syms"].append([_obj_name8(p, nm), 0, 0, 0, 2, b""])
                relocs.append([off - (lo - a), ext[nm], t])
            else:
                relocs.append([off - (lo - a), new[k], t])
        for k2, t in enumerate(o["secs"], 1):
            if k2 == num:
                raw = sec["raw"][lo - a:] if hi == b else sec["raw"][lo - a:hi - a]
                p["secs"].append({**t, "raw": raw, "size": len(raw), "relocs": relocs,  # ALIGN_1: no pad between
                                  "chars": (t["chars"] & ~0x00F00000) | 0x00100000})  # pieces (.rdata after ours: +4)
            else:
                p["secs"].append(dict(t))
        struct.pack_into("<IH", aux, 0, p["secs"][num - 1]["size"], len(relocs))
        p["syms"][k0][5] = bytes(aux) + p["syms"][k0][5][18:]
        path = c["path"].with_name(f"{c['path'].stem}_p{lo:08X}.obj")
        out.append({"path": path, "a": lo, "b": hi, "sec": c["sec"], "o": p, "num": num, "dirty": True})
    return out


def _map_vas(mp, names, delta):
    """LINK map -> {name: XBE VA} for names (Rva+Base column minus the image base -> RVA, + delta)."""
    got = {}
    for line in mp.read_text(encoding="latin-1").splitlines():
        m = re.match(r"\s*[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\s", line)
        if m and m.group(1) in names:
            got.setdefault(m.group(1), int(m.group(2), 16) + delta)
    return got


def _link(cfg, sha1, x, rx, names, reverse_text=False, sub=None, control=None, ld_name=None):
    """Cut objects of the real sections `names` + fillers for every other linked section -> PE via the pinned LINK.
    Every external no linked object defines gets an alias at its target VA in the object covering it; .text cut
    objects become ordered COMDATs (/ORDER). -> (pe path, stats); exits 1 on any LINK message."""
    root = OUT / f"retail-{sha1[:8]}"
    layout, entry = _retail_layout(rx)
    idx = {s["name"]: i for i, s in enumerate(layout)}
    for n in names:
        if n not in idx:
            refuse(f"{n} is not a linked retail section")
    t5 = REPO / "derived" / "ghidra" / f"retail-{sha1[:8]}"
    by_name = _name_vas(t5)
    cuts = []  # {path, a, b, sec, o, num, dirty}
    for n in sorted(names, key=lambda n: idx[n]):
        for path, a, b in _read_cut(root, x, n):
            o = _obj_load(path.read_bytes())
            nums = [k for k, t in enumerate(o["secs"], 1) if _obj_name(o, t["name8"]) == n]
            if len(nums) != 1 or o["secs"][nums[0] - 1]["size"] != b - a:
                refuse(f"{path.name}: want one {n} section of 0x{b - a:X} bytes")
            t = o["secs"][nums[0] - 1]  # T4.c2: Ghidra block flags (.rdata 0x60000020) -> the retail PE section's, so
            want = layout[idx[n]]["chars"] | (t["chars"] & 0x00F00000)  # our same-name contributions merge into it
            dirty = t["chars"] != want
            t["chars"] = want
            cuts.append({"path": path, "a": a, "b": b, "sec": n, "o": o, "num": nums[0], "dirty": dirty})
        last, pv = cuts[-1], layout[idx[n]]["vsize"]
        tail = layout[idx[n]]["va"] + pv - last["b"]  # retail PE VirtualSize past the raw-backed census ranges
        if tail < 0 and -tail >= (-pv) % 4 + 1:  # within the XBE's 4-byte vsize alignment is fine (.text: 2)
            refuse(f"{n}: cut ends 0x{-tail:X} past the retail PE VirtualSize")
        if tail > 0:
            t = last["o"]["secs"][last["num"] - 1]
            t["raw"], t["size"], last["dirty"] = t["raw"] + bytes(tail), t["size"] + tail, True
            last["b"] += tail  # T5: aliases into the uninitialized tail (.data bss) land in the padded object
    holes = []  # T4.c2: VA ranges our objects replace: {lo, hi, sec, sym (our external there or None), ob}
    for ob in (sub or {}).get("objs", []):
        holes += [{"lo": r["va"], "hi": r["va"] + r["size"], "sec": ".text", "sym": r["symbol"], "ob": ob}
                  for r in ob["rows"]]
        for dd in ob["data"] + ob.get("code", []):
            ext = [nm for nm, (num, v) in ob["defs"].items() if num == dd["num"] and v == 0]
            holes.append({"lo": dd["va"], "hi": dd["va"] + dd["size"], "sec": dd["rsec"], "sym": ext[0] if ext else None,
                          "ob": ob, "data": dd in ob["data"], "grp": dd["sec"], "num": dd["num"]})
    holes.sort(key=lambda h: (h["lo"], "num" in h))  # 4.3 T2: a matched row first at one VA (the kept fold copy)

    fpins = set()  # 4.3 T2: (VA, name) of every func pin (C0130 fold aliases share one VA)
    with open(PINS, encoding="utf-8") as f:
        for line in f:
            p = line.split()
            if len(p) > 3 and not p[0].startswith("#") and p[1] == "func":
                fpins.add((int(p[0], 16), p[3]))

    def tva(nm):  # 4.3 T2: a reloc target's VA by name (byname, then pins), else the name itself
        v = byte_compare.unique_va(sub["byname"], nm)
        v = byte_compare.unique_va(sub["pins"], nm) if v is None else v
        return nm if v is None else v

    def same(h, g):  # 3.4 T6.1 (C0135): one COMDAT of two of our objects at one VA, same name, bytes and relocs;
        # 4.3 T2: or two fold-alias names (each a func pin at that VA, one object or two; one may be a matched row:
        # its section from its defs), relocs by target VA; 4.4 T3 (C0215): a same-name pair may have one matched row too
        if h["sym"] is None or g["sym"] is None:
            return False
        fold = h["sym"] != g["sym"]
        num = {id(x): x.get("num") or (x["ob"]["defs"].get(x["sym"], (None, 1))[1] == 0
                                       and x["ob"]["defs"][x["sym"]][0]) or None for x in (h, g)}
        if None in num.values() or any(h.get(k, d) != g.get(k, d) for k, d in (("lo", 0), ("hi", 0), ("sec", 0),
                                                                                ("grp", ".text"), ("data", False))):
            return False
        if (not fold and (h["ob"] is g["ob"] or "num" not in h and "num" not in g)) \
                or (fold and ("num" not in g or h.get("data") or not {(h["lo"], h["sym"]), (g["lo"], g["sym"])} <= fpins)):
            return False
        sel = []
        for x in (h, g):
            o = x["ob"]["o"]
            s = o["secs"][num[id(x)] - 1]
            sk = next((e for e in o["syms"] if e[2] == num[id(x)] and e[4] == 3 and e[1] == 0
                       and _obj_name(o, e[0]) == x.get("grp", ".text")), None)
            if not s["chars"] & 0x1000 or sk is None or len(sk[5]) < 18 or sk[5][14] in (1, 5) \
                    or any(o["syms"][k][4] != 2 for _off, k, _t in s["relocs"]):
                return False  # not a COMDAT, NODUPLICATES/associative, or a reloc to a static
            sel.append((sk[5][14], bytes(s["raw"]), s["size"],
                        [(off, typ, (tva if fold else str)(_obj_name(o, o["syms"][k][0])))
                         for off, k, typ in s["relocs"]]))
        return sel[0] == sel[1]
    if control == "fold":  # 4.3 T2: a planted copy, one fold-alias copy with its byte 0 flipped -> overlap refusal
        pf = next(((h, g) for h in holes for g in holes if g["lo"] == h["lo"] and g["sym"] != h["sym"] and same(g, h)),
                  None)
        if pf is None:
            refuse("control fold: no two fold-alias copies of ours at one VA")
        t = pf[0]["ob"]["o"]["secs"][pf[0]["num"] - 1]
        t["raw"] = bytes([t["raw"][0] ^ 0xFF]) + bytes(t["raw"][1:])
        print(f"control fold: {pf[0]['sym']} 0x{pf[0]['lo']:08X} {pf[0]['ob']['path'].name} byte 0 flipped "
              f"(vs {pf[1]['sym']})")
    kept_h, ndup, folds = [], [], []
    for h in holes:  # an identical duplicate rides the first copy's hole (`dups`); LINK keeps one COMDAT copy
        g = next((g for g in kept_h if g["lo"] == h["lo"] and same(g, h)), None)
        if g is None:
            kept_h.append(h)
        else:
            g.setdefault("dups", []).append(h)
            ndup.append(f"{h['sym']} 0x{h['lo']:08X} {h['ob']['path'].name} = {g['ob']['path'].name}"
                        + (f" {g['sym']}" if h["sym"] != g["sym"] else ""))
            if h["sym"] != g["sym"]:  # 4.3 T2: another name -> its copy renamed unused, the name weak to the kept
                folds.append((h, g))
    holes = kept_h
    for h, g in zip(holes, holes[1:]):
        if g["lo"] < h["hi"]:
            refuse(f"our ranges overlap at 0x{g['lo']:08X}")
    nsplit = npieces = 0
    for h in holes:
        c = next((c for c in cuts if c["sec"] == h["sec"] and c["a"] <= h["lo"] and h["hi"] <= c["b"]), None)
        if c is None:
            refuse(f"{h['sym'] or h['sec']} 0x{h['lo']:08X}..0x{h['hi']:08X}: no {h['sec']} cut object covers it")
        h["cut"] = c
    for c in list(cuts):
        mine = [h for h in holes if h["cut"] is c]
        if mine:
            ps = _split(c, mine)
            k = cuts.index(c)
            cuts[k:k + 1] = ps
            nsplit += 1
            npieces += len(ps)
    sst_grouped = []

    def rename(o, num, nm):  # section num of o -> name nm (its section symbol too)
        t = o["secs"][num - 1]
        old = _obj_name(o, t["name8"])
        for e in o["syms"]:
            if e[2] == num and e[4] == 3 and e[1] == 0 and _obj_name(o, e[0]) == old:
                e[0] = _obj_name8(o, nm)
        h8 = _obj_name8(o, nm)  # a section header's long name is `/<strtab offset>` (a symbol's: 0, offset)
        t["name8"] = h8 if h8[:4] != b"\0\0\0\0" else f"/{struct.unpack_from('<I', h8, 4)[0]}".encode().ljust(8, b"\0")
    # T12: .rdata in VA order: every plain .rdata cut piece and .rdata data section of ours -> `.rdata$<va8>`, which
    # LINK sorts by name, so each lands at its retail VA (ours no longer placed whole at its first hole, C0126/C0133)
    rsort = [h for h in holes if h.get("data") and h["sec"] == h["grp"] == ".rdata"  # (not past a `$` group hole:
             and not any(g["sec"] == ".rdata" and g["grp"] != ".rdata" and g["hi"] <= h["lo"] for g in holes)] \
        if control != "rdata-unsorted" else []  # those stay on the T7 path)
    nrs = 0
    grouped = [h for h in holes if h.get("grp", h["sec"]) != h["sec"]]  # T7: LINK sorts our `.text$x` stub (and
    # `.xdata$x`, merged into .rdata by LINK itself) after every plain-named piece of that section, so a cut piece
    for c in cuts:  # past such a hole takes a name sorting after its group: `<group>~` (layout ladder rung 3)
        before = [h for h in grouped if h["sec"] == c["sec"] and h["hi"] <= c["a"]]
        after = [h for h in grouped if h["sec"] == c["sec"] and c["b"] <= h["lo"]]
        if not before:
            if rsort and c["sec"] == ".rdata":
                rename(c["o"], c["num"], f".rdata${c['a']:08X}")
                c["dirty"] = True
                nrs += 1
            continue
        if any(g["grp"] == h["grp"] for g in before for h in after):
            refuse(f"{c['sec']} 0x{c['a']:08X}..0x{c['b']:08X}: retail bytes between two of our {before[0]['grp']} "
                   f"sections (one LINK group cannot interleave with them)")
        nm = max(h["grp"] for h in before) + "~"
        rename(c["o"], c["num"], nm)
        c["dirty"] = True
        sst_grouped.append(f"{c['sec']} piece 0x{c['a']:08X} -> {nm}")
    if rsort:
        sst_grouped.append(f".rdata VA order (T12): {len(rsort)} sections of ours, "
                           f"{nrs} cut pieces -> .rdata$<va8>")

    hlos = [h["lo"] for h in holes]  # 4.4 T3: sorted and disjoint after the dedupe (overlap refusal above)

    def hole_at(va):  # bisect: the last hole starting at or before va (an earlier one ends at or before its lo)
        i = bisect.bisect_right(hlos, va) - 1
        return holes[i] if i >= 0 and va < holes[i]["hi"] else None

    def slotted(h):  # 3.4 T6.1: a code hole of ours in plain `.text`, a COMDAT with its symbol at 0 -> an /ORDER slot
        return (h.get("grp") == ".text" and not h.get("data") and h["sym"] is not None
                and bool(h["ob"]["o"]["secs"][h["num"] - 1]["chars"] & 0x1000))

    def cover(va):
        return next((c for c in cuts if c["a"] <= va < c["b"]), None)

    defs, und = {}, {}  # name -> [(cut, entry, VA)]; name -> [(cut, entry)]
    for c in cuts:
        for k, e in enumerate(c["o"]["syms"]):
            if e[4] != 2 or e[2] == -1:
                continue
            nm = _obj_name(c["o"], e[0])
            if e[2] == 0:
                if e[1]:
                    refuse(f"{c['path'].name}: common symbol {nm}")
                und.setdefault(nm, []).append((c, k))
            elif e[2] != c["num"]:
                refuse(f"{c['path'].name}: external {nm} defined outside {c['sec']}")
            else:
                defs.setdefault(nm, []).append((c, k, c["a"] + e[1]))
    sites = {}  # name -> [(cut, reloc, S)]; S from the retail field (DIR32 u32 - field, REL32 u32 + site + 4 - field)
    for c in cuts:
        o = c["o"]
        for k, t in enumerate(o["secs"], 1):
            for r in t["relocs"]:
                e = o["syms"][r[1]]
                if e[4] != 2 or e[2] != 0:
                    continue
                if k != c["num"] or r[2] not in (REL_DIR32, REL_REL32):
                    refuse(f"{c['path'].name}: reloc type 0x{r[2]:X} in section {k} to an external")
                site = c["a"] + r[0]
                off = xbe_info.va_to_off(x, site)
                if off is None:
                    refuse(f"reloc site 0x{site:08X} not raw-backed")
                ret, field = struct.unpack_from("<I", rx, off)[0], struct.unpack_from("<I", t["raw"], r[0])[0]
                sv = (ret - field) if r[2] == REL_DIR32 else (ret + site + 4 - field)
                sites.setdefault(_obj_name(o, e[0]), []).append((c, r, sv & 0xFFFFFFFF))
    aliases = {}  # alias name -> (VA, type)
    resolved = []
    cls = {"defined": 0, "alias": 0, "va-resolved": 0, "va-resolved defs renamed": 0}
    uname = lambda n, va: f"{n}@va_{va:08X}"
    for nm in sorted(set(und) | {n for n, d in defs.items() if len(d) > 1}):
        d = defs.get(nm, [])
        want = {d[0][2]} if len(d) == 1 else (None if d else _rule_vas(nm, by_name))
        got = {sv for _c, _r, sv in sites.get(nm, [])}
        if want and len(want) == 1 and got <= want:
            if d:
                cls["defined"] += 1
            else:
                aliases[nm] = (next(iter(want)), 0)
                cls["alias"] += 1
            continue
        if not got and nm in und:
            refuse(f"external {nm}: {len(want or ())} VAs by name, no reloc site to resolve it")
        cls["va-resolved"] += 1  # defined twice, several VAs, or sites disagreeing with the name: per site
        resolved.append(f"{nm} ({len(d)} defs, {len(want or ())} VAs by name, {len(got)} site VAs)")
        renamed = set()
        for c, k, va in d:
            c["o"]["syms"][k][0] = _obj_name8(c["o"], uname(nm, va))
            c["dirty"] = True
            renamed.add(uname(nm, va))
            cls["va-resolved defs renamed"] += 1
        per_obj = {}  # id(cut) -> {unique name: entry}
        orig = {id(c): (c, k) for c, k in und.get(nm, [])}  # the object's own undefined entry takes its first name
        for c, r, sv in sites.get(nm, []):
            u = uname(nm, sv)
            m = per_obj.setdefault(id(c), {})
            if u not in m:
                o = c["o"]
                if id(c) in orig:
                    m[u] = orig.pop(id(c))[1]
                    o["syms"][m[u]][0] = _obj_name8(o, u)
                else:
                    m[u] = len(o["syms"])
                    o["syms"].append([_obj_name8(o, u), 0, 0, 0, 2, b""])
            r[1] = m[u]
            c["dirty"] = True
            if u not in renamed:
                aliases[u] = (sv, 0)
        for c, k in orig.values():  # an undefined entry with no reloc site: name it after a resolved one
            c["o"]["syms"][k][0] = _obj_name8(c["o"], uname(nm, sites[nm][0][2]))
            c["dirty"] = True
    sst = {"split": nsplit, "pieces": npieces, "holes": len(holes), "retargeted": 0, "neutralized": 0, "hole aliases": 0,
           "by name": 0, "our aliases": 0,
           "grouped": sst_grouped, "dups": ndup, "weak": {h["sym"]: g["sym"] for h, g in folds}}
    if holes:  # T4.c2: references into a hole -> our symbol there (+offset patched into the original's field)
        def_va = {}
        for c in cuts:
            for e in c["o"]["syms"]:
                if e[4] == 2 and e[2] > 0:
                    def_va[_obj_name(c["o"], e[0])] = c["a"] + e[1]
        ours = {h["sym"] for h in holes if h["sym"]}
        for c in cuts:
            o, ent = c["o"], {}
            t = o["secs"][c["num"] - 1]
            for r in t["relocs"]:
                e = o["syms"][r[1]]
                if e[4] != 2 or e[2] != 0:
                    continue
                nm = _obj_name(o, e[0])
                va = def_va.get(nm, aliases.get(nm, (None,))[0])
                h = hole_at(va) if va is not None else None
                if h is None:
                    continue
                if h["sym"] is None:
                    refuse(f"{c['path'].name}: reference to {nm} 0x{va:08X} inside our {h['sec']} data with no "
                           f"external symbol to retarget to")
                if h["sym"] not in ent:
                    ent[h["sym"]] = next((k for k, f in enumerate(o["syms"]) if f[4] == 2 and f[2] == 0
                                          and _obj_name(o, f[0]) == h["sym"]), None)
                    if ent[h["sym"]] is None:
                        ent[h["sym"]] = len(o["syms"])
                        o["syms"].append([_obj_name8(o, h["sym"]), 0, 0, 0x20, 2, b""])
                if not isinstance(t["raw"], bytearray):
                    t["raw"] = bytearray(t["raw"])
                f0 = struct.unpack_from("<I", t["raw"], r[0])[0]
                struct.pack_into("<I", t["raw"], r[0], (f0 + va - h["lo"]) & 0xFFFFFFFF)
                r[1] = ent[h["sym"]]
                c["dirty"] = True
                sst["retargeted"] += 1
            used = {r[1] for s in o["secs"] for r in s["relocs"]}
            for k, f in enumerate(o["syms"]):  # an unreferenced undefined name of a hole VA: static absolute
                nm = _obj_name(o, f[0])
                va = def_va.get(nm, aliases.get(nm, (None,))[0]) if f[4] == 2 and f[2] == 0 else None
                if va is not None and k not in used and nm not in ours and hole_at(va):
                    f[2], f[4] = -1, 3
                    c["dirty"] = True
                    sst["neutralized"] += 1
        for nm in [n for n, (va, _t) in aliases.items() if hole_at(va)]:
            del aliases[nm]
            sst["hole aliases"] += 1
        our_defs = {nm for ob in sub["objs"] for nm in ob["defs"]}
        our_und = {nm for ob in sub["objs"] for nm in ob["und"]}
        sst["dup renamed"] = 0
        for nm in sorted(our_defs & set(def_va)):  # T6: an inline copy of ours no matched function reaches (a COMDAT
            mine = [ob for ob in sub["objs"] if nm in ob["defs"]]  # /OPT:REF drops) vs the cut's retail copy: the
            if nm in our_und or any(ob["defs"][nm][0] in ob["kept"]  # cut's definition and references take
                                    or not ob["o"]["secs"][ob["defs"][nm][0] - 1]["chars"] & 0x1000 for ob in mine):
                refuse(f"our objects and a cut object both define {nm}")  # `<name>@va_<va8>`, ours stays unused
            for c in cuts:
                for e in c["o"]["syms"]:
                    if e[4] == 2 and e[2] >= 0 and _obj_name(c["o"], e[0]) == nm:
                        e[0] = _obj_name8(c["o"], uname(nm, def_va[nm]))
                        c["dirty"] = True
            sst["dup renamed"] += 1
        for ob in sub["objs"]:  # our undefined externals: by name only (byname, unique VA), never from a retail field
            for nm in ob["und"]:
                if nm in our_defs:
                    continue
                va = byte_compare.unique_va(sub["byname"], nm)
                if va is None:  # T4: then the hand-kept pins (e.g. __except_handler3, named only by .sxdata)
                    va = byte_compare.unique_va(sub["pins"], nm)
                if va is None:
                    refuse(f"our {ob['path'].name}: external {nm} does not resolve by name "
                           f"({len(sub['byname'].get(nm, ()))} VAs in byname)")
                h = hole_at(va)
                if h is not None and slotted(h) and va == h["lo"]:  # 3.4 T6.1: another public of the folded member
                    sst["weak"][nm] = h["sym"]  # our copy fills (C0130 alias pins) -> weak external to it
                    continue
                if h is not None:
                    refuse(f"our {ob['path'].name}: external {nm} 0x{va:08X} lies in a replaced range")
                sst["by name"] += 1
                have = def_va.get(nm, aliases.get(nm, (None,))[0])
                if have is None:
                    aliases[nm] = (va, 0)
                    sst["our aliases"] += 1
                elif have != va:
                    refuse(f"our {ob['path'].name}: external {nm} 0x{va:08X} by name, linked at 0x{have:08X}")
    aliases[ENTRY_SYM] = (entry, 0x20)
    syms = {i: [] for i in range(len(layout))}  # filler symbols per layout index
    absolute = []
    placed = {"cut": 0, "filler": 0, "absolute": 0, "rel32 outside": 0}
    rel32 = {_obj_name(c["o"], c["o"]["syms"][r[1]][0]) for c in cuts for r in c["o"]["secs"][c["num"] - 1]["relocs"]
             if r[2] == REL_REL32 and c["o"]["syms"][r[1]][2] == 0}
    for nm, (va, typ) in sorted(aliases.items()):
        s = _sec_at(layout, va)
        if s is None and nm in rel32:  # T5: LNK2016 on REL32 to an absolute symbol -> section-relative in the first
            c = cuts[0]  # cut object (value va - its VA, mod 2^32): REL32 is computed in PE VA space (Ghidra took a
            c["o"]["syms"].append([_obj_name8(c["o"], nm), (va - c["a"]) & 0xFFFFFFFF, c["num"], typ, 2, b""])
            c["dirty"] = True  # .data value for a call target outside the image, e.g. SUB_209a9b1a)
            placed["rel32 outside"] += 1
        elif s is None:
            absolute.append((nm, va, -1, 0))
            placed["absolute"] += 1
        elif s["name"] in names:
            c = cover(va)
            if c is None:
                refuse(f"alias {nm} at 0x{va:08X}: no cut object covers it")
            c["o"]["syms"].append([_obj_name8(c["o"], nm), va - c["a"], c["num"], typ, 2, b""])
            c["dirty"] = True
            placed["cut"] += 1
        else:
            syms[idx[s["name"]]].append((nm, va - s["va"], 0, typ))
            placed["filler"] += 1
    order = []  # .text in VA order: a cut piece's COMDAT symbol, or our function symbol in its hole's slot (T4.c2)
    slots = [(c["a"], c) for c in cuts if c["sec"] == ".text"] + [(h["lo"], h) for h in holes if h["sec"] == ".text"
                                                                   and ("grp" not in h or slotted(h))]
    # T7: our `.text$x` stub rides its group; 3.4 T6.1: a plain `.text` COMDAT code hole (our kept copy of a folded
    # member, `??_I`) takes its slot by its COMDAT symbol, so LINK places it at its pin VA (C0136 PE-entry wall)
    for _a, c in sorted(slots, key=lambda t: t[0]):
        if "o" not in c:
            order.append(c["sym"])
            continue
        order.append(f"{COMDAT_PREFIX}{c['a']:08X}")
        _comdat(c["o"], c["num"], order[-1])
        c["dirty"] = True
    if control == "entry":  # 3.4 T6.1: our first .text function out of its slot, to the head of /ORDER (C0136)
        k = next((i for i, n in enumerate(order) if not n.startswith(COMDAT_PREFIX)), None)
        if k is None:
            refuse("control entry: no .text hole of ours in /ORDER")
        print(f"control entry: {order[k]} moved from /ORDER slot {k} to the head")
        order.insert(0, order.pop(k))
    ld = root / "link" / (ld_name or ("+".join(sorted(names, key=lambda n: idx[n]))
                          + (f"=ours-{sub['units'].replace(',', '+')}" if sub else "") + (f"-{control}" if control else "")))
    od = ld / "obj"
    if od.is_dir():
        shutil.rmtree(od)
    od.mkdir(parents=True)
    rsorted = {id(h) for h in rsort}
    ocopy = {}  # T12: our object path -> its dumped copy (read from that path) with rsort sections `.rdata$<va8>`
    for h in [d for h in rsort for d in [h, *h.get("dups", [])]]:  # 3.4 T6.1: a duplicate copy too (LINK may keep it)
        p = h["ob"]["path"]
        if p not in ocopy:
            ocopy[p] = (_obj_load(p.read_bytes()), od / "ours" / p.name)
        rename(ocopy[p][0], h["num"], f".rdata${h['lo']:08X}")
    for nm, lo in sorted({(h["sym"], h["lo"]) for h, _g in folds}):  # 4.3 T2: every COMDAT copy of ours of an extra
        for ob in [ob for ob in sub["objs"] if nm in ob["defs"]]:  # name -> `<name>@fold_<va8>` (unreferenced:
            p = ob["path"]  # /OPT:REF drops it); its object's references -> a new undefined `<name>`, weak to the kept
            if p not in ocopy:
                ocopy[p] = (_obj_load(p.read_bytes()), od / "ours" / p.name)
            o = ocopy[p][0]
            k = next(i for i, e in enumerate(o["syms"]) if e[4] == 2 and e[2] > 0 and _obj_name(o, e[0]) == nm)
            if not o["secs"][o["syms"][k][2] - 1]["chars"] & 0x1000:
                refuse(f"our {p.name}: fold alias {nm} 0x{lo:08X} defined outside a COMDAT")
            o["syms"][k][0] = _obj_name8(o, f"{nm}@fold_{lo:08X}")
            o["syms"].append([_obj_name8(o, nm), 0, 0, 0x20, 2, b""])
            for t in o["secs"]:
                for r in t["relocs"]:
                    if r[1] == k and r[2] in (REL_DIR32, REL_REL32):
                        r[1] = len(o["syms"]) - 1
    for o, q in ocopy.values():
        q.parent.mkdir(exist_ok=True)
        q.write_bytes(_obj_dump(o))
    for p in [*ld.glob("filler_*.obj"), ld / "absolute.obj", ld / "weak.obj"]:
        if p.is_file():
            p.unlink()
    objs, run, nf = [], [], 0
    placed_ours = {}  # our object path -> section it is placed in (by command-line order between data pieces)

    inobjs = set()  # 4.4 T3: every path in objs (membership was a list scan)
    dholes, dnext = {}, {}  # 4.4 T3: section -> its data holes (not .rdata-sorted) in holes order; next unvisited one
    for h in holes:
        if h.get("data") and id(h) not in rsorted:
            dholes.setdefault(h["sec"], []).append(h)

    def ours_before(sec, va):  # holes ascend by lo, objs only grows: a hole visited once is skipped on every later call
        hs, i = dholes.get(sec, []), dnext.get(sec, 0)
        while i < len(hs) and hs[i]["lo"] < va:
            h = hs[i]
            i += 1
            if h["ob"]["path"] not in inobjs:
                if h["ob"]["path"] in placed_ours:
                    refuse(f"{h['ob']['path'].name}: data in two places ({placed_ours[h['ob']['path']]}, {sec})")
                placed_ours[h["ob"]["path"]] = sec
                objs.append(h["ob"]["path"])
                inobjs.add(h["ob"]["path"])
        dnext[sec] = i

    def filler(rng):
        nonlocal nf
        secs = [layout[i] for i in rng]
        num = {s["name"]: j for j, s in enumerate(secs, 1)}
        sy = [(n, v, num[layout[i]["name"]], t) for i in rng for n, v, _s, t in syms[i]]
        if not nf:
            sy += absolute  # absolute symbols ride in the first filler object
        p = ld / f"filler_{nf}.obj"
        nf += 1
        p.write_bytes(_coff([(s["name"], (s["chars"] & ~0x00F00000) | FILLER_ALIGN, s["vsize"]) for s in secs], sy))
        return p

    for i, s in enumerate(layout):
        if s["name"] not in names:
            run.append(i)
            continue
        if run:
            objs.append(filler(run))
            inobjs.add(objs[-1])
            run = []
        mine = [c for c in cuts if c["sec"] == s["name"]]
        if reverse_text and s["name"] == ".text":
            mine.reverse()
        for c in mine:
            ours_before(s["name"], c["a"])
            if c["dirty"]:
                q = od / c["path"].name
                q.write_bytes(_obj_dump(c["o"]))
                objs.append(q)
            else:
                objs.append(c["path"])
            inobjs.add(objs[-1])
        ours_before(s["name"], 1 << 32)
    if run:
        objs.append(filler(run))
    objs += [ob["path"] for ob in (sub or {}).get("objs", []) if ob["path"] not in objs]  # .text only: /ORDER places it
    if absolute and not nf:  # T5 whole image: no filler; a section-less object carries the absolute symbols
        p = ld / "absolute.obj"
        p.write_bytes(_coff([], absolute))
        objs.append(p)
    if holes and sst["weak"]:  # 3.4 T6.1: weak externals, each another public at one of our folded copies' VA
        p = ld / "weak.obj"
        p.write_bytes(_weak_coff(sorted(sst["weak"].items())))
        objs.append(p)
    flags = [("/OPT:REF" if f == "/OPT:NOREF" and order and control != "noref" else f) for f in LINK_FLAGS]
    pe, mp = ld / f"{ld.name}.exe", ld / f"{ld.name}.map"
    for p in (pe, mp):
        if p.exists():
            p.unlink()
    args = [*flags, f"/ENTRY:{ENTRY_SYM[1:]}", f"/OUT:{pe}", f"/MAP:{mp}"]
    if order:
        of = ld / "order.txt"
        # LINK 7.10 prepends `_` to /ORDER names (LNK4037 '___rl_..' with __rl_ listed); /INCLUDE takes them raw
        of.write_text("".join(f"{n[1:] if n.startswith('_') else n}\n" for n in order), encoding="ascii", newline="\n")
        args += ([] if control == "no-order" else [f"/ORDER:@{of}"]) + [f"/INCLUDE:{n}" for n in order]
    args += [str(ocopy[p][1] if p in ocopy else p) for p in objs]
    rsp = ld / "link.rsp"
    rsp.write_text("".join((f'"{a}"' if " " in a else a) + "\n" for a in args), encoding="ascii", newline="\n")
    bindir = Path(cfg["msvc71_dir"])
    tmp = byte_compare._phase_run_dir() / "relink_link_tmp"
    tmp.mkdir(parents=True, exist_ok=True)
    t0 = time.monotonic()
    try:
        r = subprocess.run([str(bindir / "link.exe"), f"@{rsp}"], cwd=tmp, env=msvc71.cl_env(bindir, tmp),
                           stdin=subprocess.DEVNULL, capture_output=True, text=True, errors="replace")
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    secs = time.monotonic() - t0
    text = (r.stdout + r.stderr).strip()
    if r.returncode != 0 or text or not pe.is_file():
        print(f"relink: FAIL LINK exit {r.returncode}:", file=sys.stderr)
        for line in text.splitlines()[:20]:
            print(f"  {line}", file=sys.stderr)
        sys.exit(1)
    return pe, {"objects": len(cuts), "fillers": nf, "comdats": len(order), "externals": len(und), "classes": cls,
                "aliases": placed, "resolved": resolved, "flags": " ".join(flags) + (
                    ("" if control == "no-order" else " /ORDER:@order.txt") + " /INCLUDE:x" + str(len(order))
                    if order else ""), "seconds": secs, "sub": sst, "map": mp,
                "dir32": sum(1 for c in cuts for r in c["o"]["secs"][c["num"] - 1]["relocs"] if r[2] == REL_DIR32),
                "ours": [p.name for p in objs if any(p == ob["path"] for ob in (sub or {}).get("objs", []))]}


def _stats_line(st):
    c, a = st["classes"], st["aliases"]
    return (f"objects {st['objects']} (COMDAT {st['comdats']}) fillers {st['fillers']}; external names "
            f"{st['externals']}: defined by a linked object {c['defined']}, alias {c['alias']}, VA-resolved "
            f"{c['va-resolved']} (definitions renamed {c['va-resolved defs renamed']}); aliases placed: cut object "
            f"{a['cut']}, filler {a['filler']}, absolute {a['absolute']}, REL32 outside the image "
            f"{a['rel32 outside']}; link {st['seconds']:.1f}s"
            + "".join(f"\n  VA-resolved {r}" for r in st["resolved"][:10]))


def link(args):
    cfg = vv_inputs.load_config(args.config)
    x, sha1 = load_xbe(args.config)
    rx = vv_inputs.input_path("retail_xbe", cfg=cfg).read_bytes()
    if args.section:
        names = args.section.split(",")
        pe, st = _link(cfg, sha1, x, rx, names)
    else:
        names = _whole_names(sha1, rx)
        pe, st = _link(cfg, sha1, x, rx, names, ld_name="all")
    p = pe_info.parse_pe(pe.read_bytes())
    print(f"link {args.section or 'all'}: {_stats_line(st)}")
    print(f"  flags {st['flags']}")
    print(f"  {pe.relative_to(REPO).as_posix()} {pe.stat().st_size} B imageBase 0x{p['imageBase']:X} sections "
          + " ".join(s["name"] for s in p["sections"]))
    return 0


def _whole_names(sha1, rx):
    """T5: every linked retail section (pin order), after the top cut marker check."""
    names = [s["name"] for s in _retail_layout(rx)[0]]
    _read_top(OUT / f"retail-{sha1[:8]}", names)
    return names


def _write_image(peb, rx):
    """Whole-image XBE (T5): the pin's header region [0, sizeOfHeaders) (header fields, PE-header copy included),
    each linked section from write_section() with va/vsize/rawAddr/rawSize/digest set in its section header, raw
    offsets and inserted files laid out as xbe_write.write(). Refuses unless our PE entry lands at the pin's entry."""
    x = xbe_info.parse_xbe(rx)
    h = x["header"]
    base, soh = h["baseAddr"], h["sizeOfHeaders"]
    f = xbe_write._pe_fields(peb)
    pe_base = base + soh - f["sizeOfHeaders"]
    if f["entry"] + pe_base != x["entry"]["va"]:
        refuse(f"our PE entry 0x{f['entry']:X} + pe_base 0x{pe_base:X} != pin entry 0x{x['entry']['va']:08X}")
    xl = [s for s in x["sections"] if "INSERTED_FILE" not in s["flagNames"]]
    xi = [s for s in x["sections"] if "INSERTED_FILE" in s["flagNames"]]
    if x["sections"][:len(xl)] != xl:
        refuse("an INSERTED_FILE section precedes a linked section in the pin's table")
    out_secs, raw = [], soh
    for t in xl:
        try:
            g = xbe_write.write_section(peb, rx, t["name"])
        except xbe_write.Refusal as e:
            refuse(f"write_section {t['name']}: {e}")
        ra = xbe_write._align(raw, xbe_write.RAW_ALIGN)
        out_secs.append((g["va"], g["vsize"], ra, g["data"]))
        raw = ra + len(g["data"])
    va = out_secs[-1][0] + out_secs[-1][1]  # xbe_write.write() layout of the inserted files
    ins_va = {}
    for s in xi:
        va = xbe_write._align(va, xbe_write.VA_ALIGN)
        ins_va[s["name"]] = va
        va += s["virtualSize"]
    ins_raw = {}
    for s in sorted(xi, key=lambda s: s["rawAddr"]):
        ins_raw[s["name"]] = ra = xbe_write._align(raw, xbe_write.RAW_ALIGN)
        raw = ra + s["rawSize"]
    for s in xi:
        out_secs.append((ins_va[s["name"]], s["virtualSize"], ins_raw[s["name"]],
                         rx[s["rawAddr"]:s["rawAddr"] + s["rawSize"]]))
    out = bytearray(xbe_write._align(raw, xbe_write.RAW_ALIGN))
    out[:soh] = rx[:soh]
    sh = h["sectionHeadersAddr"] - base
    for k, (sva, vs, ra, d) in enumerate(out_secs):
        o = sh + xbe_info.SEC_SIZE * k
        struct.pack_into("<IIII", out, o + 4, sva, vs, ra, len(d))
        out[o + 0x24:o + 0x38] = xbe_write._digest(d)
        out[ra:ra + len(d)] = d
    return bytes(out)


def _our_vas(sub, st, peb, x):
    """Our matched functions' and data symbols' XBE VAs in the linked PE -> (names, {name: VA}): the LINK map, and a
    static with no map line (`$T318`) from the linked DIR32 field of one of our functions minus our addend (T4.c2)."""
    p = pe_info.parse_pe(peb)
    t = next(s for s in p["sections"] if s["name"] == ".text")
    delta = next(s for s in x["sections"] if s["name"] == ".text")["virtualAddr"] - t["va"] - p["imageBase"]
    want = [r["symbol"] for ob in sub["objs"] for r in ob["rows"]] + [
        nm for ob in sub["objs"] for d in ob["data"] for e in ob["o"]["syms"]
        if e[2] == d["num"] and (nm := _obj_name(ob["o"], e[0])) != d["sec"]]
    want += [nm for ob in sub["objs"] for d in ob.get("code", []) for e in ob["o"]["syms"]
             if e[2] == d["num"] and (nm := _obj_name(ob["o"], e[0])) != d["sec"]]
    got = _map_vas(st["map"], set(want), delta)
    for ob in sub["objs"]:  # a static (`$T318`: no map line) -> the linked DIR32 field of a placed section of ours
        o = ob["o"]  # (T7: matched functions, then the stub and data sections a resolved symbol anchors; fixpoint)
        anchors = [(ob["defs"][r["symbol"]][0], r["symbol"], 0) for r in ob["rows"]] + [
            (d["num"], _obj_name(o, e[0]), e[1]) for d in ob["data"] + ob.get("code", []) for e in o["syms"]
            if e[2] == d["num"] and _obj_name(o, e[0]) != d["sec"]]
        new = True
        while new:
            new = False
            for num, anchor, av in anchors:
                if anchor not in got:
                    continue
                t = o["secs"][num - 1]
                for off, k, typ in t["relocs"]:
                    nm = _obj_name(o, o["syms"][k][0])
                    if typ != REL_DIR32 or nm not in want or nm in got:
                        continue
                    rva = got[anchor] - av + off - delta - p["imageBase"]
                    ps = next(s for s in p["sections"] if s["va"] <= rva < s["va"] + s["vsize"])
                    field = struct.unpack_from("<I", peb, ps["rawptr"] + rva - ps["va"])[0]
                    got[nm] = field - struct.unpack_from("<I", t["raw"], off)[0] + delta
                    new = True
    return want, got


def _all_ours(cfg_path, layout, control=None, list_path=None, bdir=None):
    """T6: build our objects (build.build, the gate's function and inputs) into .run/<phase>/relink/build (wiped
    first) -> _ours() over every matched-set unit. control one-function: one function's object copied to
    .run/<phase>/relink/control/ with its final `ret` (C3, no reloc on it) -> `int3` (CC).
    T7 (gate): bdir given -> its already built objects, no build; list_path: the matched list (default MATCHED)."""
    import build
    from pathlib import PureWindowsPath
    rd = byte_compare._phase_run_dir() / "relink"
    if bdir is not None:
        bdir = Path(bdir)
        built = sorted(bdir.glob("*.obj"))
    else:
        bdir = rd / "build"
        if bdir.exists():
            shutil.rmtree(bdir)
        try:
            built = build.build(REPO / "src", bdir, build.UNITS, cfg_path)
        except build.Refuse as e:
            refuse(f"build: {e}")
        except build.BuildFailed as e:
            refuse(f"build: {len(e.failures)} unit(s) failed: {', '.join(sorted(f['unit'] for f in e.failures))}")
    rows = tomllib.loads(Path(list_path or MATCHED).read_text(encoding="utf-8")).get("function", [])
    stems = sorted({PureWindowsPath(r["unit"]).stem for r in rows})
    sub = _ours(",".join(stems), cfg_path, layout, bdir, list_path, control)
    if control == "weak" and not any("relink/control/" in ob["path"].as_posix() for ob in sub["objs"]):
        refuse("control weak: no matched row with a DIR32/REL32 reloc to an external beside a non-row COMDAT .text "
               "no reloc reaches")
    sub["units"] = "all"
    how = "built" if bdir == rd / "build" else "found built"  # T7: the gate's own build dir
    print(f"ours: {how} {len(built)} objects into {bdir.relative_to(REPO).as_posix()}; matched-set units "
          f"{len(sub['objs'])}, functions {sum(len(ob['rows']) for ob in sub['objs'])}; data holes "
          f"{sum(len(ob['data']) for ob in sub['objs'])}, code holes (T7) {sum(len(ob['code']) for ob in sub['objs'])}, unreferenced COMDAT data skipped "
          f"{sum(ob['unref'] for ob in sub['objs'])}")
    if control == "literal":  # 3.4 T6.1: two synthetic objects, one COMDAT (Selection 2 ANY) literal each at its VA,
        nm = next((n for n in sorted(sub["byname"]) if re.fullmatch(r"__real@[0-9a-f]{16}", n)  # bytes from its name,
                   and len(sub["byname"][n]) == 1), None)  # the second with byte 0 flipped -> overlap refusal
        if nm is None:
            refuse("control literal: no `__real@<16 hex>` name with one VA")
        va, val = next(iter(sub["byname"][nm])), int(nm[7:], 16).to_bytes(8, "little")
        cd = rd / "control"
        if cd.exists():
            shutil.rmtree(cd)
        cd.mkdir(parents=True)
        for tag, raw in (("a", val), ("b", bytes([val[0] ^ 0xFF]) + val[1:])):
            o = _obj_load(_coff([(".rdata", 0x40401040, 8)], [(nm, 0, 1, 0)]))
            o["secs"][0]["raw"] = raw
            o["syms"][0][5] = struct.pack("<IHHIHB", 8, 0, 0, 0, 0, 2).ljust(18, b"\0")
            q = cd / f"literal_{tag}.obj"
            q.write_bytes(_obj_dump(o))
            sub["objs"].append({"unit": q.name, "path": q, "o": o, "rows": [], "defs": {nm: (1, 0)}, "und": [],
                                "data": [_placed(q, layout, ".rdata", va, o["secs"][0], 1)], "code": [], "unref": 0,
                                "kept": {1}})
        print(f"control literal: {nm} 0x{va:08X} in literal_a.obj and literal_b.obj (byte 0 flipped), "
              f"{cd.relative_to(REPO).as_posix()}")
    if control == "fold":  # 4.3 T3: two synthetic objects, one COMDAT (Selection ANY) .text copy each under another
        fp, used = {}, set()  # fold-alias func pin name at the first such VA none of ours covers; _link flips one
        with open(PINS, encoding="utf-8") as f:
            for line in f:
                p = line.split()
                if len(p) > 3 and not p[0].startswith("#") and p[1] == "func":
                    fp.setdefault(int(p[0], 16), []).append(p[3])
        for ob in sub["objs"]:
            used |= {r["va"] for r in ob["rows"]} | {d["va"] for d in ob["data"] + ob.get("code", [])}
        va = next((v for v in sorted(fp) if len(set(fp[v])) > 1 and v not in used), None)
        if va is None:
            refuse("control fold: no VA with two fold-alias func pins free of ours")
        cd = rd / "control"
        if cd.exists():
            shutil.rmtree(cd)
        cd.mkdir(parents=True)
        for tag, nm in zip("ab", sorted(set(fp[va]))[:2]):
            o = _obj_load(_coff([(".text", 0x60301020, 1)], [(nm, 0, 1, 0x20)]))
            o["secs"][0]["raw"] = b"\xC3"
            o["syms"][0][5] = struct.pack("<IHHIHB", 1, 0, 0, 0, 0, 2).ljust(18, b"\0")
            q = cd / f"fold_{tag}.obj"
            q.write_bytes(_obj_dump(o))
            sub["objs"].append({"unit": q.name, "path": q, "o": o, "rows": [], "defs": {nm: (1, 0)}, "und": [],
                                "data": [], "code": [_placed(q, layout, ".text", va, o["secs"][0], 1)], "unref": 0,
                                "kept": {1}})
        print(f"control fold: 0x{va:08X} two fold-alias names in fold_a.obj and fold_b.obj, "
              f"{cd.relative_to(REPO).as_posix()}")
    if control == "samename":  # 4.4 T3 (C0215): a copy of our object holding a matched row's plain COMDAT .text; that
        pick = None  # section a code hole of the copy at the row's VA (same name), one byte (no reloc field) flipped
        for ob in sub["objs"]:
            o = ob["o"]
            for r in ob["rows"]:
                num = ob["defs"][r["symbol"]][0]
                t = o["secs"][num - 1]
                sk = next((e for e in o["syms"] if e[2] == num and e[4] == 3 and e[1] == 0
                           and _obj_name(o, e[0]) == ".text"), None)
                off = next((i for i in range(t["size"]) if not any(ro <= i < ro + 4 for ro, _k, _t in t["relocs"])),
                           None)
                if sk is not None and len(sk[5]) >= 18 and sk[5][14] not in (1, 5) and off is not None \
                        and all(o["syms"][k][4] == 2 for _ro, k, _t in t["relocs"]) \
                        and [n for n, (s, v) in ob["defs"].items() if s == num and v == 0] == [r["symbol"]]:
                    pick = pick or (ob, r, num, off)
        if pick is None:
            refuse("control samename: no matched row in a plain COMDAT .text section of ours")
        ob, r, num, off = pick
        cd = rd / "control"
        if cd.exists():
            shutil.rmtree(cd)
        cd.mkdir(parents=True)
        o = _obj_load(ob["path"].read_bytes())
        t = o["secs"][num - 1]
        t["raw"] = t["raw"][:off] + bytes([t["raw"][off] ^ 0xFF]) + t["raw"][off + 1:]
        q = cd / f"samename_{ob['path'].name}"
        q.write_bytes(_obj_dump(o))
        sub["objs"].append({"unit": q.name, "path": q, "o": o, "rows": [], "defs": {r["symbol"]: (num, 0)}, "und": [],
                            "data": [], "code": [_placed(q, layout, ".text", r["va"], t, num)], "unref": 0,
                            "kept": {num}})
        print(f"control samename: {r['symbol']} 0x{r['va']:08X} copy {q.name} (+0x{off:X} flipped), "
              f"{cd.relative_to(REPO).as_posix()}")
    # was (4.4 T3): control weak planted here, after _ours (TagIndex = the row itself), so its _weak_tag walk never
    # saw the weak reloc; 4.5 T16.2.c4 plants it in _ours before the kept walk (_weak_plant)
    if control != "one-function":
        return sub
    for ob in sub["objs"]:
        for r in ob["rows"]:
            num = ob["defs"][r["symbol"]][0]
            t = ob["o"]["secs"][num - 1]
            off = r["size"] - 1
            if t["raw"][off] != 0xC3 or any(ro <= off < ro + 4 for ro, _k, _t in t["relocs"]):
                continue
            cd = rd / "control"
            if cd.exists():
                shutil.rmtree(cd)
            cd.mkdir(parents=True)
            data = bytearray(ob["path"].read_bytes())
            sh = 20 + struct.unpack_from("<H", data, 16)[0] + 40 * (num - 1)  # section header: PointerToRawData +20
            pos = struct.unpack_from("<I", data, sh + 20)[0] + off
            if data[pos] != 0xC3:
                refuse(f"control: {ob['path'].name} byte at file offset 0x{pos:X} is not the section's ret")
            data[pos] = 0xCC
            q = cd / ob["path"].name
            q.write_bytes(bytes(data))
            ob["path"] = q
            print(f"control one-function: {r['symbol']} VA 0x{r['va'] + off:08X} (offset +0x{off:X} in the function), "
                  f"ret -> int3, object {q.relative_to(REPO).as_posix()}")
            return sub
    refuse("control one-function: no matched function ends in a ret without a reloc on it")


def check_whole(cfg, sha1, x, rx, ours=False, control=None, cfg_path=None, list_path=None, bdir=None, tag="ours"):
    """T5: link every linked section from its cut original (zero fillers), write the whole XBE, compare to the pin.
    T6 (ours): every matched function substituted (_all_ours, T4's swap route) -> link/all=ours[-<control>]/.
    T7 (gate_check): list_path, bdir (built objects, no build), tag `gate` -> link/all=gate[-<control>]/."""
    names = _whole_names(sha1, rx)
    sub = _all_ours(cfg_path, _retail_layout(rx)[0], control, list_path, bdir) if ours else None
    pe, st = _link(cfg, sha1, x, rx, names, sub=sub, control=control,
                   ld_name="all" if not ours else f"all={tag}" + (f"-{control}" if control else ""))
    print(f"check all: {_stats_line(st)}")
    print(f"  flags {st['flags']}; absolute.obj {'yes' if (pe.parent / 'absolute.obj').is_file() else 'no'}")
    peb = pe.read_bytes()
    rc_vas = False
    if sub:
        u = st["sub"]
        print(f"  substitution all: our objects {len(st['ours'])}; holes {u['holes']}, cut objects split {u['split']} "
              f"into {u['pieces']} pieces; references retargeted {u['retargeted']}, unreferenced hole names made "
              f"static {u['neutralized']}, hole aliases dropped {u['hole aliases']}; our externals resolved by name "
              f"{u['by name']} (new aliases {u['our aliases']}); cut definitions renamed off our unused inline "
              f"copies {u['dup renamed']}")
        if u["grouped"]:
            print(f"  grouped (T7): {'; '.join(u['grouped'])}")
        if u.get("dups") or u.get("weak"):
            print(f"  identical duplicate holes (3.4 T6.1): {'; '.join(u['dups']) or 'none'}; weak fold aliases: "
                  + ("; ".join(f"{a} -> {b}" for a, b in sorted(u["weak"].items())) or "none"))
        want, got = _our_vas(sub, st, peb, x)
        placed = []  # T7: each placed section of ours at its linked VA vs the VA _ours gave it (name or reloc site)
        exp = {}
        for ob in sub["objs"]:
            o = ob["o"]
            for d in ob["data"] + ob.get("code", []):
                ss = [(nm, e[1]) for e in o["syms"] if e[2] == d["num"] and (nm := _obj_name(o, e[0])) != d["sec"]]
                exp.update({nm: d["va"] + v for nm, v in ss})
                lv = {got[nm] - v for nm, v in ss if nm in got}
                ok = lv == {d["va"]}
                rc_vas = rc_vas or not ok
                placed.append(f"{ob['path'].name} {d['sec']} 0x{d['va']:08X} "
                              f"{'OK' if ok else 'FAIL linked ' + ','.join(f'0x{v:08X}' for v in sorted(lv))}")
        print(f"  map (XBE VA): {sum(1 for n in want if n in got)}/{len(want)} found"
              + "".join(f"; {n} absent" for n in want if n not in got) + "; placed: " + "; ".join(placed))
        for nm in sorted(n for n in want if n.startswith("$")):
            pv = sub["pins"].get(nm, set())
            pin = f"symbols_pin 0x{next(iter(pv)):08X}" if len(pv) == 1 else (
                f"reloc site 0x{exp[nm]:08X}" if nm in exp and not pv else f"symbols_pin {len(pv)} pins")
            want_va = next(iter(pv)) if len(pv) == 1 else exp.get(nm) if not pv else None
            v = f"0x{got[nm]:08X}" if nm in got else "absent"
            ok = nm in got and got[nm] == want_va
            rc_vas = rc_vas or not ok
            print(f"  {nm} linked {v} {pin} {'OK' if ok else 'FAIL'}")
    t0 = time.monotonic()
    try:
        out = _write_image(peb, rx)
    except SystemExit:
        if control != "rdata-unsorted":
            raise
        print("check all: FAIL whole-image write refused (control rdata-unsorted: .rdata size changed)")
        return 1  # as check_section: a control's size change is the failure it shows (T12)
    xo = pe.parent / "relinked.xbe"
    xo.write_bytes(out)
    a = xbe_info.parse_xbe(out)
    rc = 1 if rc_vas else 0  # T7: a placed section or `$` static of ours off its VA fails too
    for s, t in zip(a["sections"], x["sections"]):
        ok = s["digest"] == t["digest"]
        line = f"  {t['name']:8s} digest {s['digest']} {'OK' if ok else 'FAIL'}"
        if not ok:
            rc = 1
            got = out[s["rawAddr"]:s["rawAddr"] + s["rawSize"]]
            ret = rx[t["rawAddr"]:t["rawAddr"] + t["rawSize"]]
            n = sum(1 for p, q in zip(got, ret) if p != q) + abs(len(got) - len(ret))
            line += f" ({n} differing byte offsets; rawSize 0x{s['rawSize']:X} retail 0x{t['rawSize']:X})"
        print(line)
    ndig = sum(1 for s, t in zip(a["sections"], x["sections"]) if s["digest"] == t["digest"])
    print(f"check all: digests {ndig}/{len(x['sections'])} OK; write {time.monotonic() - t0:.1f}s")
    pin = PIN.read_text(encoding="ascii").split()[0].lower()
    got = hashlib.sha1(out).hexdigest()
    print(f"  {xo.relative_to(REPO).as_posix()} {len(out)} B sha1 {got} pin {pin} {'OK' if got == pin else 'FAIL'}")
    if got != pin:
        rc = 1
        print("  compare: " + "; ".join(xbe_write.compare(out, rx)))
    p = pe_info.parse_pe(peb)  # info only: .reloc vs the retail PE-header copy's directory 5
    ours = xbe_write._pe_fields(peb)["dirs"][5]
    hl = len(xbe_write._relocs(peb, p["sections"], *ours))
    po = x["header"]["peBaseAddr"] - x["header"]["baseAddr"]
    rsz = xbe_write._pe_fields(rx[po:])["dirs"][5][1]
    print(f"  info .reloc: dir size ours {ours[1]} (0x{ours[1]:X}) retail {rsz} (0x{rsz:X}), diff {ours[1] - rsz}; "
          f"HIGHLOW ours {hl}, DIR32 in the linked cut sections {st['dir32']}, diff {hl - st['dir32']}")
    return rc


def gate_check(list_path, bdir, cfg_path=None, control=None):
    """T7 (the gate's relink layer): the T6 whole-image check substituting every function of list_path from the
    objects ALREADY built in bdir (no build, bdir untouched) -> link/all=gate[-<control>]/. -> (rc, output lines):
    rc 0 all digests and the sha1 equal, 1 any differ, 2 refusal (refuse()'s SystemExit code; any other exit, code 0
    and None included, is 2: 4.5 T16.2 G12); stdout + stderr
    captured into the lines (text for the message only, the verdict is the rc, D26)."""
    import contextlib
    import io
    buf = io.StringIO()
    try:
        with contextlib.redirect_stdout(buf), contextlib.redirect_stderr(buf):
            cfg = vv_inputs.load_config(cfg_path)
            x, sha1 = load_xbe(cfg_path)
            rx = vv_inputs.input_path("retail_xbe", cfg=cfg).read_bytes()
            rc = check_whole(cfg, sha1, x, rx, ours=True, control=control, cfg_path=cfg_path, list_path=list_path,
                             bdir=bdir, tag="gate")
    except SystemExit as e:
        # 3.5 T4 W1: a bare/non-int SystemExit is a refusal; was: (0 if e.code is None else 2) (bare exit -> pass)
        # 4.5 T16.2 G12: code 0 is a refusal too (an exit is never a verdict); was: e.code if int and not bool, else 2
        rc = e.code if isinstance(e.code, int) and not isinstance(e.code, bool) and e.code != 0 else 2
    return rc, buf.getvalue().splitlines()


WHOLE_CONTROLS = ("one-function", "rdata-unsorted", "entry", "literal", "fold", "samename", "weak")


def check_section(args):
    if not args.section:  # whole image: T5 (--originals) or T6 (every matched function substituted)
        if args.units:
            refuse("--units needs --section")
        if args.control and (args.originals or args.control not in WHOLE_CONTROLS):
            refuse("whole image: only --control one-function|rdata-unsorted|entry|literal|fold|samename|weak, "
                   "without --originals")
        cfg = vv_inputs.load_config(args.config)
        x, sha1 = load_xbe(args.config)
        rx = vv_inputs.input_path("retail_xbe", cfg=cfg).read_bytes()
        return check_whole(cfg, sha1, x, rx, ours=not args.originals, control=args.control, cfg_path=args.config)
    if args.control in WHOLE_CONTROLS:
        refuse(f"--control {args.control} is a whole-image control (no --section)")
    if bool(args.originals) == bool(args.units):
        refuse("check needs exactly one of --originals (cut objects only) or --units U[,V..] (ours substituted)")
    if args.control and not args.units:
        refuse("--control needs --units")
    if not args.section and args.units:
        refuse("--units needs --section")
    cfg = vv_inputs.load_config(args.config)
    x, sha1 = load_xbe(args.config)
    rx = vv_inputs.input_path("retail_xbe", cfg=cfg).read_bytes()
    names = args.section.split(",")
    sub = None
    if args.units:
        sub = _ours(args.units, args.config, _retail_layout(rx)[0])
        for ob in sub["objs"]:  # every section receiving our data is real and digest-checked
            names += [d["sec"] for d in ob["data"] if d["sec"] not in names]
    pe, st = _link(cfg, sha1, x, rx, names, sub=sub, control=args.control)
    print(f"check {','.join(names)}: {_stats_line(st)}")
    print(f"  flags {st['flags']}")
    peb = pe.read_bytes()
    if sub:
        u = st["sub"]
        print(f"  substitution {args.units}: our objects {', '.join(st['ours'])} (command-line order); holes "
              f"{u['holes']}, cut objects split {u['split']} into {u['pieces']} pieces; references retargeted "
              f"{u['retargeted']}, unreferenced hole names made static {u['neutralized']}, hole aliases dropped "
              f"{u['hole aliases']}; our externals resolved by name {u['by name']} (new aliases {u['our aliases']})")
        want, got = _our_vas(sub, st, peb, x)
        print("  map (XBE VA): " + ", ".join(f"{n} 0x{got[n]:08X}" if n in got else f"{n} absent" for n in want))
    try:
        xbe_write.write(peb, rx)
        print("check: write() accepted the PE (unexpected for a partial link)")
    except xbe_write.Refusal as e:
        print(f"check: write() refused ({e}); per-section path write_section()")
    rc = 0
    for name in names:
        try:
            got = xbe_write.write_section(peb, rx, name)
        except xbe_write.Refusal as e:
            if not args.control:
                refuse(f"write_section: {e}")
            print(f"check {name}: FAIL write_section refused ({e})")  # a control's size change (noref: +4)
            rc = 1
            continue
        t = next(s for s in x["sections"] if s["name"] == name)
        want = bytes.fromhex(t["digest"])
        print(f"check {name}: va 0x{got['va']:08X} vsize 0x{got['vsize']:X} rawSize 0x{got['rawSize']:X} (retail va "
              f"0x{t['virtualAddr']:08X} vsize 0x{t['virtualSize']:X} rawSize 0x{t['rawSize']:X})")
        print(f"  digest {got['digest'].hex()} retail {want.hex()}")
        if got["digest"] == want:
            print(f"check {name}: OK digest equal")
            continue
        ret = rx[t["rawAddr"]:t["rawAddr"] + t["rawSize"]]
        diff = [i for i, (a, b) in enumerate(zip(got["data"], ret)) if a != b]
        n = len(diff) + abs(len(got["data"]) - len(ret))
        where = f" first at +0x{diff[0]:X}, last at +0x{diff[-1]:X}" if diff else ""
        print(f"check {name}: FAIL digest differs ({n} differing byte offsets{where})")
        rc = 1
    return rc


def main(argv):
    p = argparse.ArgumentParser(prog="relink.py", description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("census", help="exact-cover census of the retail XBE")
    c.add_argument("--objects", type=Path)
    c.add_argument("--control", choices=("overlap", "drop"))
    c.add_argument("--config")
    c = sub.add_parser("cut", help="range-export census ranges from the Ghidra project (no --section: whole image)")
    c.add_argument("--section", help="one section (default: all 10 linked sections + partition.tsv, T5)")
    c.add_argument("--repro", action="store_true")
    c.add_argument("--config")
    c = sub.add_parser("link", help="link the finished cut (pinned LINK 7.10); fillers for sections not named")
    c.add_argument("--section", help="A[,B..] (default: whole image, zero fillers, T5)")
    c.add_argument("--config")
    c = sub.add_parser("check", help="link, then compare XBE digests to retail (no --section: whole image, T5)")
    c.add_argument("--section", help="A[,B..] (default: whole image; --originals -> link/all/, else ours -> "
                                     "link/all=ours/relinked.xbe, T6)")
    c.add_argument("--originals", action="store_true")
    c.add_argument("--units", help="matched-set units by basename stem: our objects substituted (T4.c2)")
    c.add_argument("--control", choices=("no-order", "noref") + WHOLE_CONTROLS,
                   help="with --units: no /ORDER, or /OPT:NOREF; whole image (T6): one-function (ret -> int3), "
                        "rdata-unsorted (T12: no .rdata VA order), entry (3.4 T6.1: our first .text function to the "
                        "head of /ORDER -> PE entry refusal), literal (3.4 T6.1: a synthetic duplicate literal, one "
                        "byte flipped -> overlap refusal)")
    c.add_argument("--config")
    a = p.parse_args(argv)
    return {"census": census, "cut": cut, "link": link, "check": check_section}[a.cmd](a)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
