"""does: lists every free-text comment with citation status and counts `!FAKE` by label class, raw casts by form and placeholders
existing: provenance_lint `--casts` covers only PDB-typed casts and resolves `// src` only
delta: the list, the counts, and the classes

readability_census (3.6 T5). CLI: readability_census.py [--src DIR] [--label NAME] [--out P] [--self-test]
Library: census(src, pdb, xbe, matched) -> dict (raises provenance_lint.Refuse).
--label NAME writes docs/evidence/readability.md (or --out): one section per label, delimited by
`<!-- readability:NAME -->` / `<!-- /readability:NAME -->`; a run replaces only its own section. No --label: counts on
stdout, nothing written. A dirty `src` refuses a --label run unless --src is elsewhere.
Free text: a comment line (all annotations.EXTS files, trailing comments included) that holds a letter or digit and is
not wholly one annotation (ANNOTATION). Status: `cited`, `unresolved` or `uncited`, by the block of consecutive
comment-only lines. Raw casts: lint CAST with the lint's filters, on code with comments and strings masked.
--self-test copies src to .run/3.6/readability_selftest/, plants, compares, prints `controls: N failed as expected of N`,
deletes the scratch; docs untouched.
D46, M20: prints and writes file:line, counts and our identifiers only; retail text is compared, never output; comment
text is never output.
Exit: 0 ok; 1 a control did not fail as expected; 2 refusal (unpinned input, matched set, dirty src, git, compile).
"""
import argparse
import hashlib
import re
import shutil
import subprocess
import sys
import time
import tomllib
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import build  # noqa: E402
import provenance_lint as pl  # noqa: E402
import vv_inputs  # noqa: E402

REPO = Path(__file__).resolve().parent.parent
DOC = REPO / "docs" / "evidence" / "readability.md"
CAST_CENSUS = REPO / "docs" / "matching" / "cast_census.md"
SCRATCH = REPO / ".run" / "3.6" / "readability_selftest"
pl.LOCALS_SCRATCH = REPO / ".run" / "3.6" / "readability_locals"  # the lint's compile scratch, under this phase

# Public zlib 1.1.x sources under src (each file's header states it). BeepZlib.cpp (Beep's own zcalloc/zcfree) and
# SwampLoader.cpp (a z_stream_s member) also hit a `zlib` grep and are first-party.
THIRD_PARTY = frozenset("Libs/BeepLib/Xbox/Official_Release/" + n for n in
                        ("adler32.c", "infblock.c", "infcodes.c", "inflate.c", "infutil.c"))

HEX = r"0[xX][0-9A-Fa-f]+"
# Whole comment. A prefix followed by prose is free text; `best:` and `!FAKE:` are record lines whose text is their form.
ANNOTATION = re.compile(r"//\s*(?:UNIT:\s+src/\S+\s+OBJ:\s+\S+\s+OPT:\s+[/-]\S*(?:\s+[/-]\S*)*"
                        rf"|FUNCTION:\s+\w+\s+{HEX}"
                        r"|NAME:\s*(?:proto-pdb(?::default\.pdb)?|retail)"
                        rf"|PROTOTYPE:\s+PREVIEW\s+{HEX}(?:\s+(?:identical|near-identical|excluded))?"
                        r"|!FAKE:.*|best:.*"
                        rf"|vtable\+{HEX}|{HEX})")
SRC_ANY = re.compile(r"//\s*src(?:\s|$)")
TEXT = re.compile(r"[A-Za-z0-9]")
LABEL = re.compile(r"\s*//\s*!FAKE:\s*(.*)", re.S)
LABEL_CLASSES = (  # first match wins, on the label text after `!FAKE:`
    ("retail-string-literal", re.compile(r"^retail string literal\b")),
    ("scaffold", re.compile(r"^c1xx front end; scaffold (?:call|delete|address-of)\b")),
    ("explicit-instantiation", re.compile(r"^c1xx front end; explicit (?:member )?instantiations?\b")),
    ("forceinline", re.compile(r"^__forceinline\b")),
    ("register-local", re.compile(r"^(?:c2 register allocation; )?local;")),
    ("volatile-local", re.compile(r"^c1xx front end; volatile local\b")),
    ("pragma", re.compile(r"^c1xx front end; [^;]*\bpragma\b")),
    ("copy-ctor", re.compile(r"^front end \(implicit member emission\); user-declared copy ctor\b")),
    ("other", re.compile(r"")),
)
CAST_FORMS = ("deref-offset", "ptr-offset", "byte-index", "byte-ptr", "pdb-typed", "other-ptr", "int-offset")
OFFSET_END = re.compile(rf"\+\s*(?:{HEX}|\d+)\s*$")
INT_OFFSET = re.compile(rf"\(\s*(?:int|DWORD|unsigned(?:\s+(?:int|long|short|char))?)\s*\)\s*[A-Za-z_]\w*\s*\+\s*"
                        rf"(?:{HEX}|\d+)\b")
PLACE_PREFIXES = ("function", "global", "struct", "field")
D66_KINDS = (("pragma-optimize", re.compile(r"#\s*pragma\s+optimize\b"), re.compile(r"\boptimize\b")),
             ("volatile", re.compile(r"\bvolatile\b"), re.compile(r"\bvolatile\b")))


def mask(lines):
    """-> (code lines with comments and string/char literal interiors blanked, columns kept; {comment-only line index}).
    Same state machine as provenance_lint.comment_spans; a line inside a block comment counts as comment-only even when
    blank, so a block comment is one block."""
    out, only, inblock = [], set(), False
    for i, line in enumerate(lines):
        chars, j, touched = list(line), 0, inblock
        while j < len(line):
            if inblock:
                e = line.find("*/", j)
                stop = len(line) if e < 0 else e + 2
                chars[j:stop] = " " * (stop - j)
                inblock, j = e < 0, stop
                continue
            ch = line[j]
            if ch in "\"'":
                k = j + 1
                while k < len(line) and line[k] != ch:
                    k += 2 if line[k] == "\\" else 1
                stop = min(k, len(line))
                chars[j + 1:stop] = " " * (stop - j - 1)
                j = k + 1
            elif line.startswith("//", j):
                chars[j:] = " " * (len(line) - j)
                touched = True
                break
            elif line.startswith("/*", j):
                chars[j:j + 2] = "  "
                inblock, j, touched = True, j + 2, True
            else:
                j += 1
        masked = "".join(chars)
        out.append(masked)
        if touched and not masked.strip():
            only.add(i)
    return out, only


def src_resolves(text, pdb, xbe):
    """-> None (no `// src` in the comment), True (every one resolves) or False."""
    hits = list(SRC_ANY.finditer(text))
    if not hits:
        return None
    for h in hits:
        m = pl.SRC_PDB.match(text, h.start())
        r = pl.SRC_RETAIL.match(text, h.start())
        if r:  # decoded as provenance_lint.py:513-515; compared only
            try:
                want = r.group(2).encode("latin-1", "replace").decode("unicode_escape").encode("latin-1", "replace")
                ok = xbe.cstr_at(int(r.group(1), 16)) == want
            except UnicodeDecodeError:
                ok = False
        else:
            ok = bool(m) and pdb.resolves(m.group(1))
        if not ok:
            return False
    return True


def comment_statuses(lines, only, pdb, xbe):
    """-> ({line index: status} of free-text and `// src` lines, annotation-form lines, text-free lines)."""
    comments = pl.comment_spans(lines)
    state = {i: src_resolves(t, pdb, xbe) for i, t in comments.items()}
    block_of, blocks = {}, []
    for i in sorted(only):
        if not blocks or blocks[-1][-1] != i - 1:
            blocks.append([])
        blocks[-1].append(i)
        block_of[i] = blocks[-1]
    out, nann, nblank = {}, 0, 0
    for i, t in comments.items():
        if state[i] is not None:
            out[i] = "cited" if state[i] else "unresolved"
            continue
        if ANNOTATION.fullmatch(t.strip()):
            nann += 1
        elif not TEXT.search(t):
            nblank += 1
        else:
            near = [state.get(k) for k in block_of.get(i, ())]
            out[i] = "cited" if True in near else "unresolved" if False in near else "uncited"
    return out, nann, nblank


def label_class(text):
    return next(name for name, r in LABEL_CLASSES if r.search(text))


def paren_group(s):
    """Text inside the parenthesis s opens (after leading spaces), or None if it does not open or close on the line."""
    t = s.lstrip()
    if not t.startswith("("):
        return None
    depth = 0
    for k, ch in enumerate(t):
        depth += {"(": 1, ")": -1}.get(ch, 0)
        if depth == 0:
            return t[1:k]
    return None


def indexed_after_close(s):
    """s continues inside an open parenthesis: is its close followed by `[`?"""
    depth = 1
    for k, ch in enumerate(s):
        depth += {"(": 1, ")": -1}.get(ch, 0)
        if depth == 0:
            return s[k + 1:].lstrip().startswith("[")
    return False


def pdb_typed_names(pdb):
    """The lint's census test (provenance_lint.py:690) as one set."""
    return (pdb.full & {pl.strip_templates(k) for k in pdb.members}) | pdb.typedefs


def cast_forms(code, typed):
    """(form, cast type) of the raw casts on one masked code line, each cast in exactly one form."""
    out = []
    for m in pl.CAST.finditer(code):
        before = code[:m.start()].rstrip()
        if before and (before[-1].isalnum() or before[-1] == "_") and not before.endswith("return"):
            continue
        rest = code[m.end():]
        if pl.NOT_CAST_NEXT.match(rest):
            continue
        t, inner = m.group(1), paren_group(rest)
        prev = before[:-1].rstrip()[-1:] if before.endswith("*") else None
        deref = before.endswith("*") and not (prev and (prev.isalnum() or prev in "_)]"))
        if inner is not None and OFFSET_END.search(inner):
            out.append(("deref-offset" if deref else "ptr-offset", t))
        elif t in ("char", "BYTE") and before.endswith("(") and indexed_after_close(rest):
            out.append(("byte-index", t))
        elif t in ("char", "void", "BYTE"):
            out.append(("byte-ptr", t))
        else:
            out.append(("pdb-typed" if t in typed else "other-ptr", t))
    out += [("int-offset", None)] * len(INT_OFFSET.findall(code))
    return out


def label_text(ls, i):
    """Text of the `// !FAKE:` has_fake finds for line i: on the line, or from the heading line above down to i."""
    if "// !FAKE:" in ls[i]:
        return ls[i][ls[i].index("// !FAKE:"):]
    j = i - 1
    while not ls[j].strip().startswith("// !FAKE:"):
        j -= 1
    return " ".join(ls[j:i])


def d66_rows(rel, ls, masked):
    rows = []
    for i, code in enumerate(masked):
        for kind, find, names in D66_KINDS:
            if find.search(code):
                status = ("no-label" if not pl.has_fake(ls, i) else
                          "labelled" if names.search(label_text(ls, i)) else "label-other")
                rows.append((rel, i + 1, kind, status))
    return rows


def coverage(src, files):
    try:
        umap = build.unit_map(build.UNITS)
        rows = len(tomllib.loads(Path(build.UNITS).read_text(encoding="utf-8")).get("unit", []))
    except build.Refuse as e:
        raise pl.Refuse(f"units: {e}")
    cov = {"files": 0, "zero": [], "one": 0, "many": [], "rows": rows, "unknown_opt": 0}
    mapped = 0
    for p in files:
        if p.suffix.lower() not in (".c", ".cpp"):
            continue
        rel = p.relative_to(src).as_posix()
        us = umap.get(rel.lower(), [])
        cov["files"] += 1
        mapped += len(us)
        if not us:
            cov["zero"].append("src/" + rel)
        elif len(us) == 1:
            cov["one"] += 1
        else:
            cov["many"].append((f"src/{rel}", len(us)))
        cov["unknown_opt"] += any(u.get("opt") == "unknown" for u in us)
    cov["unmapped_rows"] = rows - mapped
    return cov, umap


def neutral_locals(src, scoped, pdb, matched, umap):
    """-> (candidates [(rel, line, name)], reach, sites [(rel, body start, body end)] whose PDB list is non-empty)."""
    bindir = vv_inputs.load_config(None).get("msvc71_dir")
    if bindir is None or not (Path(bindir) / "cl.exe").is_file():
        raise pl.Refuse(f"locals: msvc71_dir unset or has no cl.exe: {bindir}")
    rows, sites = set(), []
    reach = {"bodies": 0, "noproc": 0, "no_locals": 0, "with_locals": 0, "empty_allowed": 0}
    try:
        for p, ls in scoped:
            rel = p.relative_to(src).as_posix()
            us = umap.get(rel.lower(), [])
            if len(us) != 1:
                raise pl.Refuse(f"locals: {rel} maps to {len(us)} units")
            try:
                flags = build.flags_of(us[0])
            except build.Refuse as e:
                raise pl.Refuse(f"locals: {e}")
            procs = pl.obj_procs(pl.locals_obj(p, flags, Path(bindir)))
            for va, s, e in pl.bodies(ls):
                if va not in matched:
                    continue
                reach["bodies"] += 1
                pr = procs.get(matched[va]["symbol"])
                if pr is None:
                    reach["noproc"] += 1
                    continue
                if not pr[1]:
                    reach["no_locals"] += 1
                    continue
                reach["with_locals"] += 1
                allowed = pl.pdb_local_names(pdb, ls, s, e, pr[0])
                if not allowed:
                    reach["empty_allowed"] += 1
                    continue
                sites.append((rel, s, e))
                for x in pr[1]:
                    if pl.NEUTRAL.match(x["name"]) and x["name"] not in allowed:
                        rows.add((f"src/{rel}", pl.use_line(ls, s, e, x["name"]) + 1, x["name"]))
    finally:
        shutil.rmtree(pl.LOCALS_SCRATCH, ignore_errors=True)
    return sorted(rows), reach, sites


def census(src, pdb, xbe, matched):
    """-> {comments [(rel, line, status)], labels [(rel, line, class)], casts [(rel, line, form)], places [(rel, line,
    prefix, in code)], d66, neutral, reach, sites, coverage, counters}; rel = `src/<posix>`, all sorted."""
    src = Path(src).resolve()
    files, scoped = pl.scoped_files(src, matched)
    typed = pdb_typed_names(pdb)
    c = {"comments": [], "labels": [], "casts": [], "places": [], "d66": [], "raw_fake": 0, "annotation": 0,
         "textfree": 0, "comment_lines": 0, "nfiles": len(files), "nscoped": len(scoped), "lint_test_casts": 0}
    scope = {p for p, _ls in scoped}
    for p in files:
        rel = "src/" + p.relative_to(src).as_posix()
        ls = pl.lines_of(p)
        masked, only = mask(ls)
        comments = pl.comment_spans(ls)
        st, nann, nblank = comment_statuses(ls, only, pdb, xbe)
        c["comment_lines"] += len(comments)
        c["annotation"] += nann
        c["textfree"] += nblank
        c["comments"] += [(rel, i + 1, s) for i, s in sorted(st.items())]
        c["raw_fake"] += sum("// !FAKE:" in x for x in ls)
        for i, t in sorted(comments.items()):
            m = LABEL.match(t)
            if m:
                c["labels"].append((rel, i + 1, label_class(m.group(1))))
        for i, line in enumerate(ls):
            for f, t in cast_forms(masked[i], typed):
                c["casts"].append((rel, i + 1, f))
                c["lint_test_casts"] += p in scope and t in typed
            for m in pl.PLACE.finditer(line):
                c["places"].append((rel, i + 1, m.group(1) or "field", masked[i][m.start():m.end()] == m.group(0)))
        c["d66"] += d66_rows(rel, ls, masked)
    c["coverage"], umap = coverage(src, files)
    c["neutral"], c["reach"], c["sites"] = neutral_locals(src, scoped, pdb, matched, umap)
    for k in ("comments", "labels", "casts", "places", "d66"):
        c[k].sort()
    if sum(sum(r[2] == n for r in c["labels"]) for n, _r in LABEL_CLASSES) != len(c["labels"]):
        raise pl.Refuse("label classes do not sum to the labels")
    return c


def is_third(rel):
    return rel[len("src/"):] in THIRD_PARTY


def columns(rows, key):
    """-> {key: (first-party hits, third-party hits, files)} for rows (rel, line, ...)."""
    out = {}
    for r in rows:
        out.setdefault(key(r), []).append(r)
    return {k: (sum(not is_third(r[0]) for r in v), sum(is_third(r[0]) for r in v), len({r[0] for r in v}))
            for k, v in out.items()}


def table(title, order, rows, key):
    cols = columns(rows, key)
    out = [f"### {title}", "", "| | first-party | third-party | files |", "|---|---:|---:|---:|"]
    for k in order:
        a, b, n = cols.get(k, (0, 0, 0))
        out.append(f"| {k} | {a} | {b} | {n} |")
    a, b = sum(not is_third(r[0]) for r in rows), sum(is_third(r[0]) for r in rows)
    out += [f"| total | {a} | {b} | {len({r[0] for r in rows})} |", ""]
    return out


def top_files(rows, n=6):
    cnt = {}
    for r in rows:
        cnt[r[0]] = cnt.get(r[0], 0) + 1
    return ", ".join(f"{p} {k}" for p, k in sorted(cnt.items(), key=lambda kv: (-kv[1], kv[0]))[:n])


def lint_cast_figure():
    m = re.search(r"^(\d+) cast\(s\)", CAST_CENSUS.read_text(encoding="utf-8"), re.M)
    if not m:
        raise pl.Refuse(f"no cast count in {CAST_CENSUS.name}")
    return int(m.group(1))


def git(*args):
    r = subprocess.run(["git", "-C", str(REPO), *args], capture_output=True, text=True)
    if r.returncode:
        raise pl.Refuse(f"git {' '.join(args)}: rc {r.returncode}")
    return r.stdout.strip()


def section(c, label, head, tree, clean):
    out = [f"<!-- readability:{label} -->", f"## {label}", "", f"- HEAD: `{head}`", f"- HEAD:src: `{tree}`",
           f"- `git status --porcelain -- src` empty: {clean}", f"- source files: {c['nfiles']}", ""]
    out += table("`!FAKE` labels by class", [n for n, _r in LABEL_CLASSES], c["labels"], lambda r: r[2])
    nonlabel = c["raw_fake"] - len(c["labels"])
    out += [f"Labels: {len(c['labels'])}; raw lines holding `// !FAKE:`: {c['raw_fake']}. A label is a comment whose text "
            f"starts `// !FAKE:`; the {nonlabel} other line(s) mention it inside a longer comment (prose or commented-out "
            "code) and are free text.", f"By file: {len({r[0] for r in c['labels']})} files; top: "
            f"{top_files(c['labels'])}.", ""]
    out += table("Raw casts by form (code only, all of src)", CAST_FORMS, c["casts"], lambda r: r[2])
    out += [f"These counts cover all of src and every pointer cast form. `provenance_lint --casts` covers matched-scope "
            f"files ({c['nscoped']} of {c['nfiles']}) and PDB-typed casts only, whatever their form. Its committed "
            f"docs/matching/cast_census.md states {lint_cast_figure()}; the same test over this source gives "
            f"{c['lint_test_casts']}, so where they differ that file predates this source (not regenerated here).", ""]
    for f in CAST_FORMS:
        hits = [r for r in c["casts"] if r[2] == f]
        if len(hits) > 40:
            out += [f"`{f}`: {len(hits)} hits; top files: {top_files(hits)}.", ""]
        elif hits:
            out += [f"`{f}`:", ""] + [f"- {r[0]}:{r[1]}" for r in hits] + [""]
    out += table("Placeholders by prefix (whole lines)", PLACE_PREFIXES, c["places"], lambda r: r[2])
    nc = sum(r[3] for r in c["places"])
    out += [f"In code: {nc}; in a comment or string only: {len(c['places']) - nc}.", ""]
    cov = c["coverage"]
    out += ["### Unit-to-file coverage", "", f"`.c/.cpp` files: {cov['files']}: 0 rows {len(cov['zero'])}, 1 row "
            f"{cov['one']}, >1 rows {len(cov['many'])}. units.toml rows: {cov['rows']}; rows mapped to no src file: "
            f"{cov['unmapped_rows']}; mapped files with `opt` unknown: {cov['unknown_opt']}. Header files are outside "
            "the mapping.", ""]
    if cov["zero"]:
        out += ["0 rows:", ""] + [f"- {p}" for p in cov["zero"]] + [""]
    if cov["many"]:
        out += [">1 rows:", ""] + [f"- {p} {n}" for p, n in cov["many"]] + [""]
    out += table("D66 candidates by status (`volatile`, `#pragma optimize`; code only, all of src)",
                 ("no-label", "label-other", "labelled"), c["d66"], lambda r: r[3])
    listed = [r for r in c["d66"] if r[3] != "labelled"]
    if listed:
        out += [f"- {r[0]}:{r[1]} {r[2]} {r[3]}" for r in listed] + [""]
    rc = c["reach"]
    out += ["### Neutral-named locals absent from a non-empty PDB list", "",
            f"Matched bodies: {rc['bodies']}: no obj proc {rc['noproc']}, no obj locals {rc['no_locals']}, with obj "
            f"locals {rc['with_locals']}, of which empty PDB list (not checkable) {rc['empty_allowed']}. Candidates: "
            f"{len(c['neutral'])}.", ""]
    if c["neutral"]:
        out += [f"- {r[0]}:{r[1]} {r[2]}" for r in c["neutral"]] + [""]
    out += table("Free-text comment lines by status", ("cited", "unresolved", "uncited"), c["comments"], lambda r: r[2])
    out += [f"Comment lines: {c['comment_lines']}; annotation form {c['annotation']}; with no letter or digit "
            f"{c['textfree']} (neither listed).", "", "### List", ""]
    out += [f"{r[0]}:{r[1]} {r[2]}" for r in c["comments"]]
    out += ["", f"<!-- /readability:{label} -->"]
    return "\n".join(out) + "\n"


def merge_section(text, label, body):
    """text with label's section replaced by body (appended if absent), the header refreshed, other sections kept."""
    parts = re.findall(r"<!-- readability:([^ ]+) -->\n.*?<!-- /readability:\1 -->\n", text, re.S)
    sections = {n: re.search(rf"<!-- readability:{re.escape(n)} -->\n.*?<!-- /readability:{re.escape(n)} -->\n", text,
                             re.S).group(0) for n in parts}
    sections[label] = body
    head = ("# Readability census (generated)\n\nGenerated by `PY tools/readability_census.py --label NAME`; never "
            "hand-edited. One section per label; a run replaces only its own. No comment text, no retail bytes.\n\n"
            f"Third-party (public zlib, listed and columned, never dropped): {', '.join(sorted(THIRD_PARTY))}.\n\n")
    return head + "\n".join(sections.values())


def print_counts(c):
    def line(title, order, rows, key):
        cols = columns(rows, key)
        print(f"{title} {len(rows)}: " + ", ".join(f"{k} {sum(cols.get(k, (0, 0, 0))[:2])}" for k in order))

    line("labels", [n for n, _r in LABEL_CLASSES], c["labels"], lambda r: r[2])
    line("casts", CAST_FORMS, c["casts"], lambda r: r[2])
    line("placeholders", PLACE_PREFIXES, c["places"], lambda r: r[2])
    line("comments", ("cited", "unresolved", "uncited"), c["comments"], lambda r: r[2])
    line("d66", ("no-label", "label-other", "labelled"), c["d66"], lambda r: r[3])
    cov, rc = c["coverage"], c["reach"]
    print(f"coverage: {cov['files']} files, 0 rows {len(cov['zero'])}, 1 row {cov['one']}, >1 rows {len(cov['many'])}; "
          f"{cov['rows']} unit rows, {cov['unmapped_rows']} to no file, {cov['unknown_opt']} files opt unknown")
    print(f"neutral-local {len(c['neutral'])}; bodies {rc['bodies']}, noproc {rc['noproc']}, no locals {rc['no_locals']}, "
          f"with locals {rc['with_locals']}, empty PDB list {rc['empty_allowed']}")


def retail_string(xbe):
    """(va, text) of the first data symbol holding a plain printable string."""
    for va in sorted(pl.symbol_starts()[1]):
        s = xbe.cstr_at(va)
        if s and len(s) >= 4 and re.fullmatch(rb"[\x20-\x21\x23-\x5b\x5d-\x7e]+", s):
            return va, s.decode("latin-1")
    raise pl.Refuse("no printable retail string at a data symbol")


def plant_local(dst, c):
    """Plant `volatile int v99 = 0;` as the first statement of a matched body with a PDB list; -> (rel, line)."""
    for rel, s, e in c["sites"]:
        p = dst / rel
        ls = pl.lines_of(p)
        masked, _only = mask(ls)
        k = next((i for i in range(s, e) if "{" in masked[i]), None)
        if k is not None and ls[k].strip() == "{" and masked[k - 1].rstrip().endswith(")"):
            ls.insert(k + 1, "    volatile int v99 = 0;")
            p.write_text("\n".join(ls) + "\n", encoding="utf-8", newline="\n")
            return f"src/{rel}", k + 2
    raise pl.Refuse("no matched body to plant a local in")


def self_test(src, pdb, xbe, matched):
    doc_before = DOC.read_bytes() if DOC.is_file() else None
    shutil.rmtree(SCRATCH, ignore_errors=True)
    dst = SCRATCH / "src"
    shutil.copytree(src, dst)
    try:
        base = census(dst, pdb, xbe, matched)
        va, text = retail_string(xbe)
        name = next(n for n in sorted(pdb.full) if re.fullmatch(r"[A-Za-z_]\w*", n) and pdb.resolves(n))
        typed = next(n for n in sorted(pdb_typed_names(pdb)) if re.fullmatch(r"[A-Za-z_]\w*", n))
        plants = [("uncited", "// zz planted note"),
                  ("src-pdb-bad", "// src proto-pdb:default.pdb ZzNotInPdb"),
                  ("src-retail-bad", f'// src retail 0x{va:08X} "zz_not_retail"'),
                  ("src-other", "// src somewhere else"),
                  ("src-pdb-good", f"// src proto-pdb:default.pdb {name}"),
                  ("src-retail-good", f'// src retail 0x{va:08X} "{text}"'),
                  ("prose-offset", "// 0x1C the widget count"),
                  ("unit-form", "// UNIT: src/zz_selftest_plant.cpp OBJ: zz\\zz.obj OPT: /O2 -G6"),
                  ("unit-prose", "// UNIT: zz planted prose"),
                  ("deref-offset", "int zz1 = *(int *)(zzp + 4);"),
                  ("ptr-offset", "int *zz2 = (int *)(zzp + 4);"),
                  ("byte-index", "char zz3 = ((char *)zzp)[1];"),
                  ("byte-ptr", "char *zz4 = (char *)zzp;"),
                  ("pdb-typed", f"{typed} *zz5 = ({typed} *)zzp;"),
                  ("other-ptr", "ZzNotAType *zz6 = (ZzNotAType *)zzp;"),
                  ("int-offset", "int zz7 = (int)zzp + 4;"),
                  ("placeholder", "int function_00000001; int global_00000001; int struct_00000001; int field_10;"),
                  ("pragma", '#pragma optimize("", off)'),
                  ("volatile", "volatile int zz8;"),
                  ("trailing", "int zz9; // zz planted trailing")]
        ls, at = [], {}
        for k, line in plants:
            at[k] = len(ls) + 1
            ls += [line, ""]
        for k, group in (("under-good", [f"// src proto-pdb:default.pdb {name}", "// zz planted under a resolving src"]),
                         ("under-bad", ["// src proto-pdb:default.pdb ZzNotInPdb", "// zz planted under a failing src"]),
                         ("fake", ["// !FAKE: c2 register allocation; local; planted", "int zz10;"]),
                         ("label-other", ["// !FAKE: retail string literal; planted", "volatile int zz11;"]),
                         ("labelled", ["// !FAKE: c1xx front end; volatile local; planted", "volatile int zz12;"])):
            at[k] = len(ls) + 1
            ls += group + [""]
        plant = dst / "zz_selftest_plant.cpp"  # no FUNCTION line: never scoped, never compiled
        plant.write_text("\n".join(ls) + "\n", encoding="utf-8", newline="\n")
        lrel, lline = plant_local(dst, base)
        got = census(dst, pdb, xbe, matched)
        rel, results = "src/zz_selftest_plant.cpp", {}

        def status(key, want, off=0):
            return (rel, at[key] + off, want) in got["comments"]

        def form(key):
            return (((rel, at[key], key) in got["casts"])
                    and sum(r[2] == key for r in got["casts"]) > sum(r[2] == key for r in base["casts"]))

        def d66(key, kind, want):
            return (rel, at[key] + 1 if key in ("label-other", "labelled") else at[key], kind, want) in got["d66"]

        def nstatus(want):
            return sum(r[2] == want for r in got["comments"]) > sum(r[2] == want for r in base["comments"])

        def nfake(cl):
            return (sum(r[2] == cl for r in got["labels"]) > sum(r[2] == cl for r in base["labels"])
                    and len(got["labels"]) > len(base["labels"]))

        results["uncited listed at the planted line"] = status("uncited", "uncited") and nstatus("uncited")
        results["src pdb, not in PDB: unresolved"] = status("src-pdb-bad", "unresolved") and nstatus("unresolved")
        results["src retail, wrong text: unresolved"] = status("src-retail-bad", "unresolved")
        results["src, other grammar: unresolved"] = status("src-other", "unresolved")
        results["src pdb, real name: cited"] = status("src-pdb-good", "cited") and nstatus("cited")
        results["src retail, real text: cited"] = status("src-retail-good", "cited")
        results["block under a resolving src: cited"] = status("under-good", "cited", 1)
        results["block under a failing src: unresolved"] = status("under-bad", "unresolved", 1)
        results["offset with prose: free text"] = status("prose-offset", "uncited")
        results["trailing comment: free text"] = status("trailing", "uncited")
        results["UNIT form whole: annotation; UNIT with prose: free text"] = status("unit-prose", "uncited") and not any(
            r[:2] == (rel, at["unit-form"]) for r in got["comments"])
        for f in CAST_FORMS:
            results[f"cast {f}"] = form(f)
        results["!FAKE raises its class and total"] = nfake("register-local") and not any(
            r[:2] == (rel, at["fake"]) for r in got["comments"])
        results["#pragma optimize no-label"] = d66("pragma", "pragma-optimize", "no-label")
        results["volatile no-label"] = d66("volatile", "volatile", "no-label")
        results["volatile under an unrelated label: label-other"] = d66("label-other", "volatile", "label-other")
        results["volatile under its label: labelled"] = d66("labelled", "volatile", "labelled")
        results["placeholder per prefix"] = all(
            sum(r[2] == k for r in got["places"]) > sum(r[2] == k for r in base["places"]) for k in PLACE_PREFIXES)
        results["neutral-local v99"] = ((lrel, lline, "v99") in got["neutral"]
                                        and len(got["neutral"]) > len(base["neutral"]))
        two = merge_section(merge_section("", "AA", "<!-- readability:AA -->\nA\n<!-- /readability:AA -->\n"), "BB",
                            "<!-- readability:BB -->\nB\n<!-- /readability:BB -->\n")
        new = merge_section(two, "AA", "<!-- readability:AA -->\nA2\n<!-- /readability:AA -->\n")
        results["a label replaces only its own section"] = "\nA2\n" in new and "\nB\n" in new and "\nA\n" not in new
        results["doc untouched"] = (DOC.read_bytes() if DOC.is_file() else None) == doc_before
    except StopIteration:
        raise pl.Refuse("no PDB name to plant")
    finally:
        shutil.rmtree(SCRATCH, ignore_errors=True)
    if SCRATCH.exists():
        raise pl.Refuse(f"scratch not deleted: {SCRATCH}")
    for k, ok in results.items():
        print(f"control {k}: {'failed as expected' if ok else 'DID NOT FAIL'}")
    n = sum(results.values())
    print(f"controls: {n} failed as expected of {len(results)}")
    return 0 if n == len(results) else 1


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--src", default=str(REPO / "src"))
    ap.add_argument("--label", metavar="NAME")
    ap.add_argument("--out", default=str(DOC), metavar="P")
    ap.add_argument("--self-test", action="store_true")
    a = ap.parse_args(argv)
    t0 = time.time()
    src = Path(a.src).resolve()
    try:
        if not src.is_dir():
            raise pl.Refuse(f"missing src dir {src}")
        if a.label is not None and not re.fullmatch(r"[A-Za-z0-9_.-]+", a.label):
            raise pl.Refuse(f"label {a.label!r}: letters, digits, `_.-` only")
        ours = src == (REPO / "src").resolve()
        clean = "n/a (--src elsewhere)"
        if a.label is not None and ours and not a.self_test:
            if git("status", "--porcelain", "--", "src"):
                raise pl.Refuse("src is dirty; commit it first or pass --src elsewhere")
            clean = "yes"
        matched = pl.matched_vas()
        pdb, xbe = pl.inputs()
        if a.self_test:
            return self_test(src, pdb, xbe, matched)
        c = census(src, pdb, xbe, matched)
        print_counts(c)
        if a.label is not None:
            out = Path(a.out)
            out.parent.mkdir(parents=True, exist_ok=True)
            old = out.read_text(encoding="utf-8") if out.is_file() else ""
            body = section(c, a.label, git("rev-parse", "HEAD"), git("rev-parse", "HEAD:src"), clean)
            out.write_text(merge_section(old, a.label, body), encoding="utf-8", newline="\n")
            print(f"readability_census: section {a.label} -> {out.as_posix()} "
                  f"(sha256 {hashlib.sha256(out.read_bytes()).hexdigest()[:12]})")
    except pl.Refuse as e:
        print(f"readability_census: REFUSED {e}", file=sys.stderr)
        return 2
    print(f"readability_census: ok, {time.time() - t0:.1f}s")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
