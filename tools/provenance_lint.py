"""provenance_lint (3.2 T6): static provenance checks on matched sources (docs/matching/CONVENTIONS.md §1, §2, §4, §7).

CLI: provenance_lint.py [--src DIR] [--list TOML] [--accepted TXT] [--diff REV] [--casts] [--self-test]
Library (3.2 T7, the gate's `provenance` layer): lint(src, list_path=MATCHED, rev=UPSTREAM, accepted=ACCEPTED,
  base=None, config=None, cache=True) -> (fails, nscoped, nfiles, cov); raises Refuse. base=DIR: fake-diff reads each
  old file from DIR/<rel> (all source files of src) instead of `git show REV`. 3.5 T4: rev UPSTREAM (`@{u}`) = `git
  merge-base HEAD @{u}` in the repo holding src (was "HEAD"); no repo, no upstream or a failed `git show` of a
  non-added file refuses; the PDB, EXE and retail XBE must equal their config/inputs.manifest.jsonl rows (Refuse);
  cache=False (the gate) compiles every locals obj into scratch, never reading the cache. --list / --accepted default to
  config/matched_set.toml / config/provenance_accepted.txt.
Scope: files under DIR (default src) holding a `// FUNCTION: USA 0x<VA>` whose VA is in the --list matched set;
a function's body = its FUNCTION line to the next FUNCTION line or EOF. The hedge check scans comments in all of DIR.
Checks (each failure `FAIL <id> <file>:<line>: <why>`):
  proto-pdb    each `NAME: proto-pdb` identifier resolves in default.pdb (publics, procs, TPI type or member names); an
               own-line tag's definition by its declarator, as written or under its enclosing namespaces (4.1 T4)
  placeholder  `function_/struct_<VA>` = a func start in symbols.txt/pin or a FUNCTION VA of the file; `global_<VA>`
               = a data start; `field_<off>` declared on a line whose offset comment is `// 0x<off>`
  src-pdb      `// src proto-pdb:default.pdb <identifier>` resolves as proto-pdb does
  src-retail   `// src retail 0x<VA> "<text>"` equals the retail XBE bytes at VA through the NUL
  hedge        probably|likely|seems|maybe|perhaps|presumably|appears to|guess (whole words, any case) in a comment,
               except exact lines in config/provenance_accepted.txt (`path<TAB>verbatim line<TAB>reason`; a line whose
               first field is a BASELINE_CHECKS id is a baseline entry, see accepted)
  accepted     (T7) each accepted entry equals (stripped) a line of DIR/<path minus `src/`> whose comment holds a
               hedge word; else `FAIL accepted <path>: entry matches no hedge line` (stale or widened). 5.5 T13:
               baseline entries `<check><TAB><path><TAB><verbatim line><TAB><reason>` (check in BASELINE_CHECKS) each
               waive one hit of that check on a line of that text; an unused entry fails `accepted` (stale; the
               baseline only shrinks)
  fake-diff    each line holding `// !FAKE:` removed pairs with a code change, or a `// !FAKE:` added, in the same
               function (difflib; a hunk that changes only comments is no code change; 4.1 T4), per step: each commit
               of REV..HEAD vs its first parent (`git rev-list --reverse`), then the working tree vs HEAD (5.5 T13 I56;
               was: one diff REV vs the working tree, a removal undone by a later commit's re-add unseen; was: own-line
               labels, any changed non-comment line)
  name-retail  each `NAME: retail` identifier has a retail XBE string naming it: equal, `.?AV<Class>@@`, or a
               NUL-terminated string holding it as a whole token
  d66          `volatile`, `#pragma optimize`, `__asm` in a matched function body has `// !FAKE:` on the line or
               heading the comment block directly above; so does each on a `#define` line (and its backslash continuations)
               of any file of DIR (4.5 T16.2 G18)
  forceinline  (4.2 T5) `__forceinline` in code (comments aside) in any file of DIR, with or without `// !FAKE:` (D116,
               D117: a call retail inlined is matched by a pointer local or a hand form, C0191)
  noinline     (4.5 T16.2 G9) `noinline` in code in any file of DIR, with or without `// !FAKE:` (D116)
  pragma       (4.5 T16.2 G9) a `#pragma` line inside a function body (in a FUNCTION extent at a brace depth above the
               FUNCTION line's; comments and literals skipped) in any file of DIR, with or without `// !FAKE:` (D116)
  include      (5.1 T1) a quoted `#include "<name>"` in any file of DIR whose (file dir / name).resolve() is not an
               existing file inside DIR (escaping DIR or missing); every line, `#if` arms included; no allowlist
  attr         (5.5 T13, D138) `__declspec(noreturn)` in code in any file of DIR, unless `// !FAKE:` on or above the line
               whose reason field records a before/after (holds `->`, not `no before/after recorded`)
  pragma-scope (5.5 T13, D138) `#pragma intrinsic` in any file of DIR after its first FUNCTION line or at a brace
               depth above 0 (a file's head only)
  fake-fields  (5.5 T13, I43, D33) each line of any file of DIR holding `// !FAKE:` has three non-empty `;`-separated
               fields after the tag on that line (kind; instrument; reason, the reason the rest of the line)
  members      (5.5 T13.c3, S4, D78) in any file of DIR (a scope parse, FileParse: comments, literals, directives and
               dead `#if 0` / NON_MATCHING arms blanked; function bodies not entered), each data member, method and
               nested typedef of a class/struct/union resolves as a member of its owner or of any base (LF_BCLASS,
               LF_VBCLASS, LF_IVBCLASS, transitively; operator names normalized), each namespace-scope typedef is an
               S_UDT, PDB type or name, each enumerator a PDB enumerator; `field_<off>` and `_pad*` data pass;
               parameters are not checked (census-only). was: no member check (the 5.5 T3 census only)
  untagged     (5.5 T13.c3, I52) in each scoped file and its quoted headers, a free-function definition that is not a
               FUNCTION line's own definition fails without a NAME tag (on its line or the `//` block above) and with
               no PDB proc, public or symbols.txt/symbols_pin name; a marked `pin_emit_` scaffold (matched_set's test:
               depth 0, `!FAKE:` and `scaffold` in the `//` lines directly above) passes. was: no check
NAME tags (proto-pdb, name-retail) and placeholder run on every file of DIR (4.5 T16.2 G18; was: scoped files only);
src-pdb, src-retail, d66 bodies and locals on scoped files.
  locals      (T6.c2) each S_BPREL32/S_REGREL32/S_REGISTER local (params included) of a matched function's proc in
               its /Z7 obj is a local of the PDB proc at its `// PROTOTYPE: PREVIEW 0x<VA>` (no PROTOTYPE line: of the
               PDB procs named exactly as the obj proc) or neutral (`v<N>`, `tmp`, `i`, `p`); line = first whole-word
               use in the body. The obj proc is the one whose offset field relocates to the matched symbol. Objs: unit
               flags (build.flags_of) + /Z7 via msvc71.compile_one, cached in
               derived/provenance_locals/<sha1(source, flags, quoted headers)>.obj (scratch: a fresh per-run dir
               `provenance_locals.*` under .run/<phase>/, only it deleted at exit; 5.5 T13 G5; was: the shared
               .run/3.2/provenance_locals, deleted whole). Coverage printed: functions with obj locals / without / no
               obj proc; a matched function with no obj proc fails `locals` at its FUNCTION line (5.5 T13 G4; was:
               counted only). C++ is never parsed for locals.
  neutral     (5.5 T13, S5) a neutral obj local that is not a local of its PDB proc, where that proc has locals, has
               `// !FAKE:` on or above its first use line (was: any neutral name passed)
--casts writes docs/matching/cast_census.md (C-style casts to a pointer to a PDB-named type or to an S_UDT typedef of
a class/struct/union, C0112; never fails).
--self-test copies DIR to .run/3.2/provenance_selftest/, plants one violation per check (fake-diff on a synthetic
pair; accepted: a scratch accepted file with a real line + " probably"; body plants in the first matched body whose `{`
is at its FUNCTION line's depth and whose PROTOTYPE proc has PDB locals, 5.5 T13.c3; was: the first `{` of scoped[0]'s
matched extents), expects each check's failures to rise,
prints `controls: N failed as expected of N`, deletes the scratch.
D46: prints file:line, the check id and identifiers as written in our source only; never retail bytes or text.
Exit: 0 clean; 1 any violation (or a control that did not fail); 2 refusal (unreadable PDB, XBE, matched set, git).
"""
import argparse
import bisect
import difflib
import hashlib
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import annotations  # noqa: E402
import build  # noqa: E402
import byte_compare  # noqa: E402  5.5 T13 G5: _phase_run_dir
import coff_info  # noqa: E402
import matched_set  # noqa: E402
import msvc71  # noqa: E402
import pdb_info  # noqa: E402
import pdb_types  # noqa: E402
import pe_info  # noqa: E402
import vv_inputs  # noqa: E402
import xbe_info  # noqa: E402

REPO = Path(__file__).resolve().parent.parent
MATCHED = REPO / "config" / "matched_set.toml"
SYMBOLS = (REPO / "config" / "retail" / "symbols.txt", REPO / "config" / "retail" / "symbols_pin.txt")
ACCEPTED = REPO / "config" / "provenance_accepted.txt"
CENSUS = REPO / "docs" / "matching" / "cast_census.md"
SCRATCH = REPO / ".run" / "3.2" / "provenance_selftest"
LOCALS_CACHE = REPO / "derived" / "provenance_locals"
LOCALS_SCRATCH = byte_compare._phase_run_dir() / "provenance_locals"  # the base: per-run dirs beside it (5.5 T13 G5)
# was: REPO / ".run" / "3.2" / "provenance_locals" (one shared dir, removed whole by each run)
UPSTREAM = "@{u}"  # fake-diff rev: the merge-base of HEAD and its upstream (3.5 T4 W5)
EXTS = annotations.EXTS
FUNC = annotations.FUNC
CHECKS = ("proto-pdb", "placeholder", "src-pdb", "src-retail", "hedge", "fake-diff", "name-retail", "d66", "locals",
          "accepted", "forceinline", "noinline", "pragma", "include",  # 5.1 T1: +include
          "neutral", "attr", "pragma-scope", "fake-fields",  # 5.5 T13
          "members", "untagged")  # 5.5 T13.c3
# 5.5 T13: checks an accepted baseline entry waives (5.5 T13.c3: +members, untagged; was: the first four only)
BASELINE_CHECKS = ("neutral", "attr", "pragma-scope", "fake-fields", "members", "untagged")
FAKE_TAG = "// !FAKE:"

ID = r"~?[A-Za-z_]\w*"
QUAL = rf"{ID}(?:\s*<[^()]*?>)?(?:::{ID}(?:\s*<[^()]*?>)?)*"
NAME_TAG = re.compile(r"//.*?\bNAME:\s*(proto-pdb|retail)\b")
SRC_PDB = re.compile(r"//\s*src proto-pdb:default\.pdb\s+(\S+)")
SRC_RETAIL = re.compile(r'//\s*src retail 0x([0-9A-Fa-f]+) "((?:[^"\\]|\\.)*)"')
PLACE = re.compile(r"\b(function|global|struct)_([0-9A-Fa-f]{8})\b|\bfield_([0-9A-Fa-f]+)\b")
FIELD_DECL = re.compile(r"^[^/]*?\bfield_([0-9A-Fa-f]+)\s*(\[[^\]]*\])?\s*;")
OFF_COMMENT = re.compile(r"//\s*0x([0-9A-Fa-f]+)\b")
HEDGE = re.compile(r"\b(probably|likely|seems|maybe|perhaps|presumably|appears\s+to|guess)\b", re.I)
D66 = re.compile(r"\bvolatile\b|#\s*pragma\s+optimize\b|\b__asm\b")
FORCEINLINE = re.compile(r"\b__forceinline\b")  # D116, D117: refused, `!FAKE` or not
NOINLINE = re.compile(r"\bnoinline\b")  # 4.5 T16.2 G9, D116: refused, `!FAKE` or not; was: only `__forceinline`
PRAGMA = re.compile(r"^\s*#\s*pragma\b")  # 4.5 T16.2 G9, D116: in a function body refused, `!FAKE` or not
DEFINE = re.compile(r"^\s*#\s*define\b")  # 4.5 T16.2 G18: d66 on every file's `#define` lines
KEYWORD_DECL = re.compile(rf"^(?:typedef\s+)?(?:class|struct|union|enum|namespace)\s+(?:__declspec\([^)]*\)\s+)?({QUAL})")
ANON = re.compile(r"^\s*(?:typedef\s+)?enum\s*\{")
CALL_NAME = re.compile(rf"({QUAL}|{QUAL}::operator\s*[^\s(]+|operator\s*[^\s(]+)\s*\(")  # 5.3 T2: outer classes kept
# was: rf"({QUAL}|{ID}::operator\s*[^\s(]+|operator\s*[^\s(]+)\s*\(" (`std::deque<T>::const_iterator::operator*` ->
# `const_iterator::operator*`, not in default.pdb)
CAST = re.compile(r"\(\s*(?:const\s+)?(?:struct\s+|class\s+|union\s+)?([A-Za-z_]\w*(?:::\w+)*)\s*(?:const\s*)?\*+\s*\)")
NOT_CAST_NEXT = re.compile(r"\s*(?:[;{,)\]=:]|const\b|throw\b|$)")
PROTO_VA = re.compile(r"^\s*// PROTOTYPE: PREVIEW 0x([0-9A-Fa-f]{8})\b")
NEUTRAL = re.compile(r"^(?:v\d+|tmp|i|p)$")
INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"]+)"')
NORETURN = re.compile(r"\b__declspec\s*\(\s*noreturn\s*\)")  # 5.5 T13 (D138): attr
INTRINSIC = re.compile(r"^\s*#\s*pragma\s+intrinsic\b")  # 5.5 T13 (D138): pragma-scope
HIT = re.compile(r"FAIL (\S+) (src/[^:]+):(\d+): ")  # 5.5 T13: a fail line's check, path and line (baseline)


class Refuse(Exception):
    pass


def lines_of(path):
    return path.read_text(encoding="utf-8", errors="replace").splitlines()


def strip_templates(s):
    """`A<B<C>>::f<D>` -> `A::f` (template argument lists removed, spaces dropped)."""
    out, depth = [], 0
    for ch in s:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth = max(0, depth - 1)
        elif depth == 0 and not ch.isspace():
            out.append(ch)
    return "".join(out)


def strip_template_head(code):
    """Drop a leading `template <...>` (balanced) from a code line."""
    m = re.match(r"\s*template\s*<", code)
    if not m:
        return code
    depth, i = 1, m.end()
    while i < len(code) and depth:
        depth += {"<": 1, ">": -1}.get(code[i], 0)
        i += 1
    return code[i:]


def code_part(line):
    """The line before its `//` comment (string literals with `//` are rare in decl lines; not handled)."""
    i = line.find("//")
    return line if i < 0 else line[:i]


def decl_ident(code):
    """Identifier a declaration line declares (as written, template args kept), or None."""
    c = strip_template_head(code).strip()
    c = re.sub(r'^extern\s+"C"\s*', "", c)
    m = KEYWORD_DECL.match(c)
    if m:
        return m.group(1)
    if "(" in c:
        m = CALL_NAME.search(c)
        return m.group(1) if m else None
    head = re.split(r"[=\[;{]", c, maxsplit=1)[0]
    head = head.split(":")[0] if "::" not in head else head
    ids = re.findall(QUAL, head)
    return ids[-1] if ids else None


def declarator(ident):
    """Last whitespace token at `<>` depth 0, dropping a return type QUAL swallowed as a template argument
    (`vector<M>::size_type vector<M>::size` -> `vector<M>::size`); `operator X` is left whole (4.1 T4); 5.3 T2: the
    scope before `operator` gets the same cut (`vector<bool>::reference & vector<bool>::reference::operator=`)."""
    m = re.search(r"\boperator\b", ident)
    if m:
        return declarator(ident[:m.start()]) + ident[m.start():] if m.start() else ident
    # was: if "operator" in ident: return ident
    depth, cut = 0, 0
    for k, ch in enumerate(ident):
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif ch.isspace() and depth == 0:
            cut = k + 1
    return ident[cut:]


def enclosing_ns(ls, i):
    """Namespaces open at line index i, outermost first, by brace depth, from `namespace X {` lines (4.1 T4)."""
    stack, depth = [], 0
    for k in range(i):
        c = code_part(ls[k])
        m = re.match(r"\s*namespace\s+(\w+)\s*\{", c)
        for ch in c:
            if ch == "{":
                depth += 1
                if m:
                    stack.append((m.group(1), depth))
                    m = None
            elif ch == "}":
                if stack and stack[-1][1] == depth:
                    stack.pop()
                depth -= 1
    return [n for n, _d in stack]


def name_resolves(pdb, ls, j, ident):
    """An own-line NAME tag's definition at line index j: its declarator resolves as written, else qualified by the
    enclosing namespaces, innermost first (4.1 T4; was: pdb.resolves(ident) alone, refusing an unqualified member in
    `namespace std` and a scoped return type)."""
    d = declarator(ident)
    if pdb.resolves(d):
        return True
    ns = enclosing_ns(ls, j)
    return any(pdb.resolves("::".join(ns[:k]) + "::" + d) for k in range(len(ns), 0, -1))


def enumerators(lines, i):
    """-> [(line index, name)] of the enumerators of the anonymous enum opened on line i, to its `}`."""
    out = []
    for k in range(i + 1, len(lines)):
        c = re.sub(r"/\*.*?\*/", "", code_part(lines[k]))
        for part in c.split(","):
            m = re.match(r"\s*([A-Za-z_]\w*)\s*(=|$)", part)
            if m and "}" not in part:
                out.append((k, m.group(1)))
        if "}" in c:
            break
    return out


def func_ident(lines, i):
    """Identifier of the definition after an annotation NAME line at index i (next code line(s))."""
    for j in range(i + 1, min(i + 8, len(lines))):
        s = lines[j].strip()
        if not s or s.startswith("//") or s.startswith("#"):
            continue
        c = strip_template_head(code_part(lines[j])).strip()
        if not c or re.fullmatch(r"(inline|static|extern|__forceinline)\s*", c):
            continue
        return decl_ident(c), j
    return None, i


# ---------- 5.5 T13.c3: scope parser for `members` and `untagged` (ported from the 5.5 T3 census) ----------
BUILTIN = {"int", "char", "float", "double", "void", "long", "short", "unsigned", "signed", "bool", "const",
           "volatile", "__int64", "wchar_t", "__int8", "__int16", "__int32", "struct", "class", "enum", "union",
           "typename"}
CVKW = re.compile(r"\b(const|volatile|struct|class|enum|union|typename|register|mutable|static|inline|virtual|"
                  r"explicit|extern|friend|__cdecl|__stdcall|__fastcall|__thiscall)\b")
ACCESS = re.compile(r"\b(public|protected|private)\s*:(?!:)")
FP_DECL = re.compile(r"\(\s*(?:__\w+\s+)?(?:[\w:]+\s*::\s*)?\*\s*(?:const\s+)?([A-Za-z_]\w*)\s*\)\s*\(")  # fn pointer
CLASS_HEAD = re.compile(r"^(typedef\s+)?(class|struct|union)\b\s*(?:__declspec\s*\([^)]*\)\s*)?"
                        r"([A-Za-z_][\w:]*)?\s*(<.*>)?\s*(:[^{]*)?$", re.S)
ENUM_HEAD = re.compile(r"^(typedef\s+)?enum\b\s*([A-Za-z_]\w*)?\s*$", re.S)
NS_HEAD = re.compile(r"^namespace\b\s*([A-Za-z_]\w*)?\s*$", re.S)
EXTERN_C = re.compile(r'^extern\s*"\s*"\s*$', re.S)  # `extern "C"` after literal contents are blanked
NM_IF = re.compile(r"^\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)$")
PAD_FIELD = re.compile(r"field_[0-9A-Fa-f]+|_pad.*")  # data members that pass `members` (placeholder, padding)


def arms(ls):
    """-> (dead[i]: line in a `#if 0` arm or a NON_MATCHING-defined arm, directive[i])."""
    dead, direc, st = [], [], []  # frame: [kind, state] kind nm|zero|other, state True = dead arm
    cont = False
    for line in ls:
        m = NM_IF.match(line)
        is_dir = cont or bool(re.match(r"^\s*#", line))
        cont = is_dir and line.rstrip().endswith("\\")
        if m:
            k, rest = m.group(1), m.group(2).split("//")[0].strip()
            if k == "ifdef":
                st.append(["nm", True] if rest == "NON_MATCHING" else ["other", False])
            elif k == "ifndef":
                st.append(["nm", False] if rest == "NON_MATCHING" else ["other", False])
            elif k == "if":
                if rest == "0":
                    st.append(["zero", True])
                elif re.fullmatch(r"defined\s*\(?\s*NON_MATCHING\s*\)?|NON_MATCHING", rest):
                    st.append(["nm", True])
                elif re.fullmatch(r"!\s*defined\s*\(?\s*NON_MATCHING\s*\)?", rest):
                    st.append(["nm", False])
                else:
                    st.append(["other", False])
            elif k == "elif" and st:
                st[-1] = ["other", False]
            elif k == "else" and st:
                if st[-1][0] in ("nm", "zero"):
                    st[-1][1] = not st[-1][1]
            elif k == "endif" and st:
                st.pop()
        dead.append(any(f[1] for f in st))
        direc.append(is_dir)
    return dead, direc


def clean_lines(ls):
    """Lines with comments and string/char literal contents blanked (lengths kept); preprocessor and dead-arm lines
    blanked."""
    dead, direc = arms(ls)
    out, inblock = [], False
    for i, line in enumerate(ls):
        if direc[i] or dead[i]:
            out.append(" " * len(line))
            continue
        buf, j = [], 0
        while j < len(line):
            if inblock:
                e = line.find("*/", j)
                if e < 0:
                    buf.append(" " * (len(line) - j))
                    j = len(line)
                else:
                    buf.append(" " * (e + 2 - j))
                    inblock, j = False, e + 2
                continue
            ch = line[j]
            if ch in "\"'":
                k = j + 1
                while k < len(line) and line[k] != ch:
                    k += 2 if line[k] == "\\" else 1
                k = min(k, len(line) - 1)
                buf.append(ch + " " * (k - j - 1) + ch)
                j = k + 1
            elif line.startswith("//", j):
                buf.append(" " * (len(line) - j))
                break
            elif line.startswith("/*", j):
                inblock = True
                buf.append("  ")
                j += 2
            else:
                buf.append(ch)
                j += 1
        out.append("".join(buf)[:len(line)].ljust(len(line)))
    return out


def match_brace(t, i):
    """Index of the `}` matching the `{` at i (or len)."""
    d = 0
    for k in range(i, len(t)):
        if t[k] == "{":
            d += 1
        elif t[k] == "}":
            d -= 1
            if d == 0:
                return k
    return len(t)


def tmpl_head_off(s):
    """s without its leading `template <...>` heads -> (rest, offset of rest in s)."""
    s2 = s.lstrip()
    off = len(s) - len(s2)
    while s2.startswith("template"):
        m = re.match(r"template\s*<", s2)
        if not m:
            break
        d, k = 0, m.end() - 1
        for k in range(m.end() - 1, len(s2)):
            if s2[k] == "<":
                d += 1
            elif s2[k] == ">":
                d -= 1
                if d == 0:
                    break
        rest = s2[k + 1:]
        r2 = rest.lstrip()
        off += (len(s2) - len(rest)) + (len(rest) - len(r2))
        s2 = r2
    return s2, off


def split_top(s, sep=","):
    """Split s on sep at paren/angle/bracket depth 0 -> [(piece, offset)]."""
    out, d, st = [], 0, 0
    for k, c in enumerate(s):
        if c in "(<[{":
            d += 1
        elif c in ")>]}":
            d = max(0, d - 1)
        elif c == sep and d == 0:
            out.append((s[st:k], st))
            st = k + 1
    out.append((s[st:], st))
    return out


def first_paren(s):
    """The call paren of a declaration: after `operator<sym>` if present, else the first `(` at angle depth 0;
    -> (name, name offset, paren index, qualifier) or None."""
    m = re.search(r"\boperator\b", s)
    if m:
        k = m.end()
        while k < len(s) and s[k].isspace():
            k += 1
        p = s.find("(", k + 2) if s.startswith("()", k) else s.find("(", k)
        if p < 0:
            return None
        sym = re.sub(r"\s+", " ", s[m.end():p]).strip()
        sym = sym if re.match(r"[A-Za-z_]", sym) else sym.replace(" ", "")
        nm = "operator" + (" " + sym if re.match(r"[A-Za-z_]", sym) else sym)
        q = m.start()
        mm = re.search(r"((?:[A-Za-z_]\w*\s*(?:<[^()]*?>)?\s*::\s*)*)$", s[:q])
        return nm, q, p, (mm.group(1).replace(" ", "") if mm else "")
    d = 0
    for k, c in enumerate(s):
        if c == "<":
            d += 1
        elif c == ">":
            d = max(0, d - 1)
        elif c == "(" and d == 0:
            mm = re.search(r"((?:[A-Za-z_]\w*\s*(?:<[^()]*?>)?\s*::\s*)*)(~?\s*[A-Za-z_]\w*)\s*$", s[:k])
            if not mm:
                return None
            return re.sub(r"\s", "", mm.group(2)), mm.start(2), k, re.sub(r"\s", "", mm.group(1))
    return None


def data_declarators(s):
    """Data member names of a member declaration (no call paren): [(name, offset)]."""
    out = []
    for k, (piece, off) in enumerate(split_top(s)):
        piece = piece.split("=")[0]
        piece = re.sub(r":\s*\w+\s*$", "", piece)  # bitfield
        q = re.sub(r"(\s*\[[^\]]*\])+\s*$", "", piece.rstrip())
        mm = re.search(r"([A-Za-z_]\w*)\s*$", q)
        if not mm or mm.group(1) in BUILTIN:
            continue
        if k == 0 and not CVKW.sub(" ", q[:mm.start(1)]).strip():
            continue  # a lone type (e.g. a stray macro)
        out.append((mm.group(1), off + mm.start(1)))
    return out


def opnorm(op):
    """Operator name without class/struct/union/enum keywords and spaces."""
    return re.sub(r"\s+", "", re.sub(r"\b(class|struct|union|enum)\s+", "", op))


class FileParse:
    """Scope parse of one file (comments, literals, directives and dead `#if` arms blanked): decls [(kind, owner,
    name, line idx)] with kind data|method|typedef|typedef-nested|enumerator (parameters not recorded: census-only),
    free_defs [(qualified name, line idx)] of free-function definitions. Function bodies are not entered."""

    def __init__(self, ls):
        cl = clean_lines(ls)
        self.t = "\n".join(cl)
        self.starts = [0]
        for line in cl:
            self.starts.append(self.starts[-1] + len(line) + 1)
        self.decls = []
        self.free_defs = []
        self.namespaces = set()
        self.parse(0, len(self.t), {"ns": [], "cls": []})

    def line(self, off):
        return bisect.bisect_right(self.starts, off) - 1

    def add(self, kind, ctx, name, off):
        self.decls.append((kind, "::".join(ctx["ns"] + ctx["cls"]), name, self.line(off)))

    def parse(self, i, end, ctx):
        t, st, k = self.t, i, i
        while k < end:
            c = t[k]
            if c == ";":
                self.stmt(st, k, ctx)
                st = k = k + 1
                continue
            if c == "}":
                return k
            if c == "{":
                k = self.block(st, k, ctx)
                st = k
                continue
            k += 1
        return end

    def head(self, st, k):
        s = ACCESS.sub(lambda m: " " * len(m.group()), self.t[st:k])
        s2, off = tmpl_head_off(s)
        s3 = s2.lstrip()
        off += len(s2) - len(s3)
        s3 = re.sub(r"^(?:__declspec\s*\([^)]*\)\s*)+", lambda m: " " * len(m.group()), s3)
        return s3, st + off

    def block(self, st, k, ctx):
        """Head t[st:k] then `{` at k; -> index after the construct."""
        t = self.t
        h, hoff = self.head(st, k)
        hs = h.strip()
        close = match_brace(t, k)
        m = NS_HEAD.match(hs)
        if m:
            if m.group(1):
                self.namespaces.add(m.group(1))
            self.parse(k + 1, close, dict(ctx, ns=ctx["ns"] + ([m.group(1)] if m.group(1) else [])))
            return close + 1
        if EXTERN_C.match(hs):
            self.parse(k + 1, close, ctx)
            return close + 1
        m = CLASS_HEAD.match(hs)
        if m and "(" not in hs and "=" not in hs:
            self.parse(k + 1, close, dict(ctx, cls=ctx["cls"] + [m.group(3) or "__anon"]))
            return self.declarators(close, ctx, typedef=bool(m.group(1)))
        m = ENUM_HEAD.match(hs)
        if m:
            for piece, off in split_top(t[k + 1:close]):
                mm = re.match(r"\s*([A-Za-z_]\w*)", piece)
                if mm:
                    self.add("enumerator", ctx, mm.group(1), k + 1 + off + mm.start(1))
            return self.declarators(close, ctx, typedef=bool(m.group(1)))
        if hs.endswith("=") or not hs:
            e = t.find(";", close)
            return (e + 1) if e >= 0 else close + 1
        fp = first_paren(h)
        if fp:
            name, noff, _p, qual = fp
            if ctx["cls"]:
                self.add("method", ctx, name, hoff + noff)
            else:
                q = qual.rstrip(":")
                q = strip_templates(q) if q else ""
                if not q or q.split("::")[0] in self.namespaces:
                    self.free_defs.append(("::".join(ctx["ns"] + ([q] if q else []) + [name]), self.line(hoff + noff)))
        return close + 1

    def declarators(self, close, ctx, typedef):
        t = self.t
        e = t.find(";", close)
        if e < 0:
            return close + 1
        s = t[close + 1:e]
        if "{" in s or "}" in s:
            return close + 1
        for name, off in data_declarators(" x " + s):
            o = close + 1 + off - 3
            if typedef:
                self.add("typedef-nested" if ctx["cls"] else "typedef", ctx, name, o)
            elif ctx["cls"]:
                self.add("data", ctx, name, o)
        return e + 1

    def stmt(self, st, k, ctx):
        h, hoff = self.head(st, k)
        hs = h.strip()
        if not hs or re.match(r"(friend|using|template|static_assert|extern\s*\")\b", hs):
            return
        if re.match(r"(class|struct|union|enum)\s+[\w:]+\s*$", hs):
            return  # forward declaration
        if hs.startswith("typedef"):
            fm = FP_DECL.search(h)
            if fm:
                name, o = fm.group(1), hoff + fm.start(1)
            else:
                mm = re.search(r"([A-Za-z_]\w*)\s*$", re.sub(r"(\s*\[[^\]]*\])+\s*$", "", h.rstrip()))
                if not mm:
                    return
                name, o = mm.group(1), hoff + mm.start(1)
            self.add("typedef-nested" if ctx["cls"] else "typedef", ctx, name, o)
            return
        eq = h.find("=")
        fm = FP_DECL.search(h)
        fp = first_paren(h)
        if fp and fp[0] == "sizeof":
            fp = None  # `char pad[N - sizeof(T)];` is data
        is_func = fp is not None and (eq < 0 or eq > fp[2] or fp[0].startswith("operator")) \
            and not (fm and fm.start() <= (fp[2] if fp else 0))
        if is_func:
            if ctx["cls"]:
                self.add("method", ctx, fp[0], hoff + fp[1])
            return  # census: parameters recorded here; not checked (census-only)
        if ctx["cls"]:
            if fm:
                self.add("data", ctx, fm.group(1), hoff + fm.start(1))
                return
            for name, off in data_declarators(h):
                self.add("data", ctx, name, hoff + off)


def fl_bases(fl, seen=None):
    """Base class type indices of fieldlist `fl` (LF_BCLASS, LF_VBCLASS, LF_IVBCLASS; LF_INDEX followed; 5.5 T13.c3).
    The walk as pdb_types.fl_names; unknown leaves end it."""
    seen = set() if seen is None else seen
    out = []
    if fl in seen or fl not in pdb_types.recs:
        return out
    seen.add(fl)
    b, o = pdb_types.recs[fl][1], 0
    while o < len(b):
        if b[o] >= 0xF0:
            o += b[o] & 0xF
            continue
        lf = struct.unpack_from("<H", b, o)[0]
        if lf in (0x150D, 0x1502):
            _, p = pdb_types.num(b, o + 8) if lf == 0x150D else pdb_types.num(b, o + 4)
            _, o = pdb_types.cstr(b, p)
        elif lf in (0x150F, 0x1510, 0x150E):
            _, o = pdb_types.cstr(b, o + 8)
        elif lf == 0x1511:
            p = o + 8 + (4 if (struct.unpack_from("<H", b, o + 2)[0] >> 2) & 7 in (4, 6) else 0)
            _, o = pdb_types.cstr(b, p)
        elif lf == 0x1400:  # bclass: attr, type, offset
            out.append(struct.unpack_from("<I", b, o + 4)[0])
            _, o = pdb_types.num(b, o + 8)
        elif lf in (0x1401, 0x1402):  # vbclass / ivbclass: attr, btype, vbptr type, two numerics
            out.append(struct.unpack_from("<I", b, o + 4)[0])
            _, p = pdb_types.num(b, o + 12)
            _, o = pdb_types.num(b, p)
        elif lf == 0x1409:
            o += 8
        elif lf == 0x1404:
            out += fl_bases(struct.unpack_from("<I", b, o + 4)[0], seen)
            o += 8
        else:
            break
    return out


def class_name_of(b):  # name field of an LF_CLASS/LF_STRUCTURE record body
    return pdb_types.cstr(b, pdb_types.num(b, 16)[1])[0]


def class_name(ti):  # name of the class/struct type record ti, else None
    leaf, b = pdb_types.recs.get(ti, (None, b""))
    return class_name_of(b) if leaf in (0x1504, 0x1505) else None


class Pdb:
    """Name sets from default.pdb: publics (demangled to scope::name), procs, TPI types and members."""

    def __init__(self, path, exe_path):
        try:
            data = path.read_bytes()
            pdb_types.recs.clear()
            self.streams = pdb_info.read_streams(data)["streams"]
            pdb_types.load_tpi(self.streams)
            types, self.members = pdb_types.names()
            p = pdb_info.parse_pdb(data)
            pe = pe_info.parse_pe(exe_path.read_bytes())
        except (OSError, pdb_info.NotPdb, pe_info.NotPe, struct.error, IndexError, KeyError, ValueError) as e:
            raise Refuse(f"cannot read PDB {path} / EXE {exe_path}: {e}")
        if pdb_info.validate(p, pe["rsds"]):
            raise Refuse("default.pdb / default.exe pair invalid")
        self.proc_at = {}  # prototype VA -> {proc name} (VA = imageBase + section[seg-1].va + off)
        for m in p["modules"]:
            for pr in m["procs"]:
                if 1 <= pr["seg"] <= len(pe["sections"]):
                    va = pe["imageBase"] + pe["sections"][pr["seg"] - 1]["va"] + pr["off"]
                    self.proc_at.setdefault(va, set()).add((pr["name"], pr["seg"], pr["off"]))
        self.typedefs = set()  # S_UDT names whose type is a class/struct/union record (C0112)
        symrec = (p["dbi"] or {}).get("symRecordStream")
        if symrec is not None and symrec < len(self.streams) and self.streams[symrec]:
            for kind, rec in pdb_info.iter_records(self.streams[symrec]):
                if kind == 0x1108 and len(rec) > 4:  # S_UDT: type index u32, name
                    ti = struct.unpack_from("<I", rec, 0)[0]
                    if pdb_types.recs.get(ti, (None,))[0] in (0x1504, 0x1505, 0x1506):
                        self.typedefs.add(pdb_types.cstr(rec, 4)[0])
        self._locals = {}
        self.full = set()
        for t in types:
            self._add(strip_templates(t))
        for m in p["modules"]:
            for pr in m["procs"]:
                self._add(strip_templates(pr["name"]))
        for pub in p["publics"]:
            q = demangle_scope(pub["name"])
            if q:
                self._add(q)
        self.simple = {n.split("::")[-1] for n in self.full}
        for t, ms in self.members.items():
            self.simple |= ms
        self.member_of = {}
        for t, ms in self.members.items():
            self.member_of.setdefault(strip_templates(t), set()).update(ms)
        # 5.5 T13.c3 (members, untagged; the 5.5 T3 census's PdbX): S_UDT names (module streams and globals), proc
        # names, publics, enumerators, members with operator names normalized, and class -> direct base names
        self.udt, self.proc_names, self.enumerators = set(), set(), set()
        for m in p["modules"]:
            self.proc_names |= {strip_templates(pr["name"]) for pr in m["procs"]}
            st = self.streams[m["stream"]] if m["stream"] is not None and m["stream"] < len(self.streams) else None
            pos = 4
            while st and pos + 4 <= len(st):
                ln, kind = struct.unpack_from("<HH", st, pos)
                if ln < 2:
                    break
                if kind == 0x1108 and ln > 6:
                    self.udt.add(pdb_types.cstr(st[pos + 4:pos + 2 + ln], 4)[0])
                pos += 2 + ln
        if symrec is not None and symrec < len(self.streams) and self.streams[symrec]:
            for kind, rec in pdb_info.iter_records(self.streams[symrec]):
                if kind == 0x1108 and len(rec) > 4:
                    self.udt.add(pdb_types.cstr(rec, 4)[0])
        self.publics = {q for pub in p["publics"] if (q := demangle_scope(pub["name"]))}
        self.mem, self.bases = {}, {}
        for t, ms in self.members.items():
            self.mem.setdefault(strip_templates(t), set()).update(
                opnorm(x) if x.lstrip("~").startswith("operator") else strip_templates(x) for x in ms)
        for leaf, b in pdb_types.recs.values():
            if leaf == 0x1507 and struct.unpack_from("<I", b, 8)[0]:
                self.enumerators |= pdb_types.fl_names(struct.unpack_from("<I", b, 8)[0])
            elif leaf in (0x1504, 0x1505):
                prop, fl = struct.unpack_from("<HI", b, 2)
                if fl and not prop & 0x80:  # not a forward reference
                    self.bases.setdefault(strip_templates(class_name_of(b)), set()).update(
                        strip_templates(x) for x in map(class_name, fl_bases(fl)) if x)

    def has_member(self, own, nm):
        """nm (operator names opnorm'd) a member of own or of any base, transitively (5.5 T13.c3)."""
        seen, st = {own}, [own]
        while st:
            t = st.pop()
            if nm in self.mem.get(t, ()):
                return True
            for bn in self.bases.get(t, ()):
                if bn not in seen:
                    seen.add(bn)
                    st.append(bn)
        return False

    def locals_of(self, name):
        """pdb_types.locals records of the procs named exactly `name` (cached)."""
        if name not in self._locals:
            self._locals[name] = pdb_types.locals(self.streams, name)
        return self._locals[name]

    def _add(self, q):
        self.full.add(q)
        parts = q.split("::")
        for k in range(1, len(parts)):
            self.full.add("::".join(parts[:k]))  # namespaces and enclosing classes

    def resolves(self, ident):
        q = strip_templates(ident)
        if q in self.full:
            return True
        if "::" not in q:
            return q in self.simple
        scope, last = q.rsplit("::", 1)
        return last in self.member_of.get(scope, ()) or (last.startswith("~") and last[1:] == scope.split("::")[-1]
                                                         and scope in self.full)


def demangle_scope(name):
    """MSVC public -> `scope::name` for plain `?n@s@@`, ctor/dtor `??0`/`??1`; C `_n[@k]` -> `n`; else None."""
    if name.startswith("??0") or name.startswith("??1"):
        parts = name[3:].split("@@")[0].split("@")
        cls = parts[0]
        return "::".join(reversed(parts)) + "::" + ("~" if name[2] == "1" else "") + cls
    if name.startswith("?") and not name.startswith("??"):
        parts = [x for x in name[1:].split("@@")[0].split("@") if x and not x[0].isdigit()]
        return "::".join(reversed(parts)) if parts else None
    if name.startswith("_") or name[:1].isalpha():
        return re.sub(r"@\d+$", "", name[1:] if name.startswith("_") else name)
    return None


class Xbe:
    def __init__(self, path):
        try:
            self.data = path.read_bytes()
            self.x = xbe_info.parse_xbe(self.data)
        except (OSError, xbe_info.NotXbe) as e:
            raise Refuse(f"cannot read retail XBE {path}: {e}")
        self._strings = None

    def cstr_at(self, va):
        off = xbe_info.va_to_off(self.x, va)
        if off is None:
            return None
        end = self.data.find(b"\0", off)
        return self.data[off:end if end >= 0 else len(self.data)]

    def strings(self):
        if self._strings is None:
            self._strings = [m.group(0).decode("latin-1") for m in re.finditer(rb"[\x20-\x7e]{3,}(?=\0)", self.data)]
        return self._strings

    def names(self, ident):
        q = strip_templates(ident)
        cands = {q, q.split("::")[-1]}
        rtti = {f".?AV{c}@@" for c in cands} | {".?AV" + "@".join(reversed(q.split("::"))) + "@@"}
        tok = [re.compile(rf"(?<![A-Za-z0-9_]){re.escape(c)}(?![A-Za-z0-9_])") for c in cands]
        for s in self.strings():
            if s in cands or any(r in s for r in rtti) or any(t.search(s) for t in tok):
                return True
        return False


def symbol_starts():
    """-> (func starts, data starts, {func va: first row name}) from symbols.txt then the pin file."""
    func, data, names = set(), set(), {}
    for p in SYMBOLS:
        for line in p.read_text(encoding="utf-8").splitlines():
            f = line.split()
            if len(f) < 2 or f[0].startswith("#"):
                continue
            try:
                va = int(f[0], 16)
            except ValueError:
                continue
            (func if f[1] == "func" else data).add(va)
            if f[1] == "func" and len(f) >= 6:
                names.setdefault(va, " ".join(f[3:-2]))
    return func, data, names


def comment_spans(lines):
    """-> {line index: comment text on that line} for // and /* */ comments (string literals skipped)."""
    out, inblock = {}, False
    for i, line in enumerate(lines):
        j, buf = 0, []
        while j < len(line):
            if inblock:
                e = line.find("*/", j)
                buf.append(line[j:] if e < 0 else line[j:e])
                if e < 0:
                    j = len(line)
                else:
                    inblock, j = False, e + 2
                continue
            ch = line[j]
            if ch in "\"'":
                k = j + 1
                while k < len(line) and line[k] != ch:
                    k += 2 if line[k] == "\\" else 1
                j = k + 1
            elif line.startswith("//", j):
                buf.append(line[j:])
                break
            elif line.startswith("/*", j):
                inblock, j = True, j + 2
            else:
                j += 1
        if buf:
            out[i] = " ".join(buf)
    return out


def bodies(lines):
    """-> [(va, start index, end index exclusive)] per FUNCTION line."""
    marks = [(i, int(m.group(1), 16)) for i, l in enumerate(lines) if (m := FUNC.match(l))]
    return [(va, i, marks[k + 1][0] if k + 1 < len(marks) else len(lines)) for k, (i, va) in enumerate(marks)]


def code_depths(lines):
    """-> [(brace depth before the line, the line's code with comments and string/char literals blanked)] per line
    (4.5 T16.2 G9: `#pragma` in a function body = its line at a depth above its FUNCTION line's, inside the extent)."""
    out, depth, inblock = [], 0, False
    for line in lines:
        start, buf, j = depth, [], 0
        while j < len(line):
            if inblock:
                e = line.find("*/", j)
                inblock, j = (True, len(line)) if e < 0 else (False, e + 2)
                continue
            ch = line[j]
            if ch in "\"'":
                k = j + 1
                while k < len(line) and line[k] != ch:
                    k += 2 if line[k] == "\\" else 1
                buf.append(ch + ch)
                j = k + 1
            elif line.startswith("//", j):
                break
            elif line.startswith("/*", j):
                inblock, j = True, j + 2
            else:
                depth += (ch == "{") - (ch == "}")
                buf.append(ch)
                j += 1
        out.append((start, "".join(buf)))
    return out


def func_at(bs, i):
    for va, s, e in bs:
        if s <= i < e:
            return va
    return None


def scoped_files(src, matched):
    files = sorted((p for p in src.rglob("*") if p.is_file() and p.suffix.lower() in EXTS),
                   key=lambda p: p.relative_to(src).as_posix())
    out = []
    for p in files:
        ls = lines_of(p)
        if any(va in matched for va, _s, _e in bodies(ls)):
            out.append((p, ls))
    return files, out


def load_accepted(path=ACCEPTED, baseline=None):
    """-> hedge set {(path, stripped line)}; 5.5 T13: a line whose first field is in BASELINE_CHECKS is a baseline
    entry `check<TAB>path<TAB>line<TAB>reason`, counted into the Counter baseline[(check, path, stripped line)]."""
    path = Path(path)
    acc = set()
    if path.is_file():
        for line in path.read_text(encoding="utf-8").splitlines():
            if not line.strip() or line.startswith("#"):
                continue
            f = line.split("\t")
            if f[0] in BASELINE_CHECKS:
                if len(f) != 4 or not f[3].strip():
                    raise Refuse(f"{path.name}: need check<TAB>path<TAB>line<TAB>reason: {line[:60]}")
                if baseline is not None:
                    baseline[(f[0], f[1], f[2].strip())] += 1
                continue
            if len(f) != 3:
                raise Refuse(f"{path.name}: need path<TAB>line<TAB>reason: {line[:60]}")
            acc.add((f[0], f[1].strip()))
    return acc


def apply_baseline(src, fails, baseline):
    """5.5 T13: fails minus one hit per baseline entry (check, path, stripped line text), plus `FAIL accepted` for each
    entry left unused (stale: the baseline only shrinks)."""
    left, out, cache = Counter(baseline), [], {}
    for f in fails:
        m = HIT.match(f)
        if m and m.group(1) in BASELINE_CHECKS:
            path, n = m.group(2), int(m.group(3))
            if path not in cache:
                p = src / path.removeprefix("src/")
                cache[path] = lines_of(p) if p.is_file() else []
            ls = cache[path]
            key = (m.group(1), path, ls[n - 1].strip() if n <= len(ls) else None)
            if left[key] > 0:
                left[key] -= 1
                continue
        out.append(f)
    for (c, path, text), n in sorted(left.items()):
        out += [f"FAIL accepted {path}: baseline `{c}` entry matches no `{c}` hit (stale): {text[:60]}"] * n
    return out


def fake_pairs(old, new):
    """Old lines holding `// !FAKE:` (own-line or trailing) that went, with no code change and no label added in the
    same function -> [old index]. A hunk is a code change only if its non-empty code parts differ.
    4.1 T4: TAG anywhere on a line, comment-only hunks excuse nothing, an added label excuses one removal (one in the
    same hunk first, so a reword never takes the removed label's blame); was: own-line labels only, any non-comment line
    in a hunk marked its function changed, no pairing (a reworded label refused, a stripped trailing label passed)."""
    ob, nb = bodies(old), bodies(new)
    changed, removed, added = set(), [], []
    sm = difflib.SequenceMatcher(None, old, new, autojunk=False)
    for h, (tag, i1, i2, j1, j2) in enumerate(sm.get_opcodes()):
        if tag == "equal":
            continue
        oc = [code_part(old[i]).strip() for i in range(i1, i2)]
        nc = [code_part(new[j]).strip() for j in range(j1, j2)]
        same = [c for c in oc if c] == [c for c in nc if c]  # the hunk changed only comments
        for i in range(i1, i2):
            if "// !FAKE:" in old[i]:
                removed.append((i, func_at(ob, i), h))
            if not same and oc[i - i1]:
                changed.add(func_at(ob, i))
        for j in range(j1, j2):
            if "// !FAKE:" in new[j]:
                added.append((func_at(nb, j), h))
            if not same and nc[j - j1]:
                changed.add(func_at(nb, j))
    rest = []
    for i, f, h in removed:
        if f in changed:
            continue
        if (f, h) in added:
            added.remove((f, h))
        else:
            rest.append((i, f))
    funcs = [f for f, _h in added]
    left = []
    for i, f in rest:
        if f in funcs:
            funcs.remove(f)
        else:
            left.append(i)
    return left


def git(*args, cwd=REPO):  # 3.5 T4 W5: cwd = the repo holding src; was: always REPO
    r = subprocess.run(["git", *args], cwd=cwd, capture_output=True)
    if r.returncode != 0:
        raise Refuse(f"git {' '.join(args)}: exit {r.returncode}")
    return r.stdout.decode("utf-8", "replace")


def check_fake_diff(src, rev, fails, base=None):
    if base is not None:  # old files from base/<rel> (gate control, T7), no git
        for p in sorted(src.rglob("*"), key=lambda q: q.relative_to(src).as_posix()):
            if not p.is_file() or p.suffix.lower() not in EXTS:
                continue
            rel = p.relative_to(src).as_posix()
            o = Path(base) / rel
            for i in fake_pairs(lines_of(o) if o.is_file() else [], lines_of(p)):
                fails.append(f"FAIL fake-diff src/{rel}:{i + 1}: removed `// !FAKE:` with no changed code line in its"
                             f" function (base line)")
        return
    # 3.5 T4 W5: git runs in the repo holding src (no repo: Refuse); rev UPSTREAM = `git merge-base HEAD @{u}`; an
    # added file's old side is empty, any other failed `git show` refuses. Was: cwd REPO, src outside REPO returned
    # silently, a failed `git show` read as an empty file, gate rev HEAD (a removal committed to HEAD passed).
    top = Path(git("rev-parse", "--show-toplevel", cwd=src).strip()).resolve()
    if rev == UPSTREAM:
        rev = git("merge-base", "HEAD", UPSTREAM, cwd=top).strip()
    srel = src.relative_to(top).as_posix()
    # 5.5 T13 (I56): each commit of rev..HEAD vs its first parent, then the working tree vs HEAD; was: one diff rev vs
    # the working tree (a `// !FAKE:` removed in one commit and re-added in a later one was never seen)
    for c in git("rev-list", "--reverse", f"{rev}..HEAD", cwd=top).split():
        fake_diff_step(top, srel, f"{c}^", c, fails)
    fake_diff_step(top, srel, "HEAD", None, fails)


def fake_diff_step(top, srel, old_rev, new_rev, fails):
    """fake-diff of srel between old_rev and new_rev (None: the working tree) in the repo top (5.5 T13 I56)."""
    for st in git("diff", "--name-status", "--no-renames", old_rev, *([new_rev] if new_rev else []), "--", srel,
                  cwd=top).splitlines():
        stat, rel = st.split("	", 1)
        if Path(rel).suffix.lower() not in EXTS:
            continue
        old = [] if stat == "A" else git("show", f"{old_rev}:{rel}", cwd=top).splitlines()
        if new_rev is None:
            p = top / rel
            new = lines_of(p) if p.is_file() else []
        else:
            new = [] if stat == "D" else git("show", f"{new_rev}:{rel}", cwd=top).splitlines()
        for i in fake_pairs(old, new):
            fails.append(f"FAIL fake-diff {rel}:{i + 1}: removed `// !FAKE:` with no changed code line in its function"
                         f" ({old_rev} line)")


def check_members(src, files, pdb, fails):
    """5.5 T13.c3 (S4, D78): each data member, method, typedef and enumerator FileParse finds in any file resolves in
    default.pdb (a member as a member of its owner or of any base, transitively)."""
    for p in files:
        rel = "src/" + p.relative_to(src).as_posix()
        for kind, owner, name, li in FileParse(lines_of(p)).decls:
            own = strip_templates(owner)
            if kind == "enumerator":
                ok = name in pdb.enumerators
            elif kind == "typedef":
                ok = name in pdb.udt or pdb.resolves((own + "::" if own else "") + name) or name in pdb.full
            elif kind == "data" and PAD_FIELD.fullmatch(name):
                continue
            elif "__anon" in own.split("::"):
                ok = name in pdb.simple
            else:  # data, method, typedef-nested; operator names normalized both sides
                ok = (pdb.has_member(own, opnorm(name) if name.startswith("operator") else name)
                      or pdb.resolves(own + "::" + name))
            if not ok:
                fails.append(f"FAIL members {rel}:{li + 1}: {kind} `{(own + '::' if own else '') + name}` not in "
                             f"default.pdb (as a member of its owner or a base) (D78)")


def symbol_names():
    """Names of every symbols.txt / symbols_pin row, as written and demangle_scope'd (5.5 T13.c3)."""
    out = set()
    for path in SYMBOLS:
        for line in path.read_text(encoding="utf-8").splitlines():
            f = line.split()
            if len(f) >= 6 and not f[0].startswith("#"):
                nm = " ".join(f[3:-2])
                out |= {nm, demangle_scope(nm)} - {None}
    return out


def name_tagged(ls, i):  # a NAME tag on line i or in the `//` block directly above it
    j = i
    while j >= 0 and (j == i or ls[j].strip().startswith("//")):
        if NAME_TAG.search(ls[j]):
            return True
        j -= 1
    return False


def marked_scaffold(ls, i, name, depth):  # matched_set's marked test: depth 0, pin_emit_, `!FAKE:` + scaffold above
    j, above = i - 1, []
    while j >= 0 and ls[j].strip().startswith("//"):
        above.append(ls[j])
        j -= 1
    note = " ".join(above)
    return depth == 0 and name.startswith(matched_set.SCAFFOLD_PREFIX) and "!FAKE:" in note and "scaffold" in note


def check_untagged(src, scoped, pdb, fails):
    """5.5 T13.c3 (I52): a free-function definition in a scoped file or its quoted headers, other than a FUNCTION
    line's own definition, has a NAME tag, a PDB proc/public name or a symbols name, or is a marked scaffold."""
    syms, seen = symbol_names(), set()
    for p, _ls in scoped:
        for f in [p] + quoted_headers(p):
            f = f.resolve()
            if f in seen or not f.is_relative_to(src):
                continue
            seen.add(f)
            ls = lines_of(f)
            annotated = {func_ident(ls, s)[1] for _va, s, _e in bodies(ls)}
            cd = None
            for q, li in FileParse(ls).free_defs:
                q = strip_templates(q)
                if li in annotated or name_tagged(ls, li):
                    continue
                if any(n in pdb.proc_names or n in pdb.publics or n in syms for n in (q, q.split("::")[-1])):
                    continue
                cd = cd or code_depths(ls)
                if marked_scaffold(ls, li, q.split("::")[-1], cd[li][0]):
                    continue
                fails.append(f"FAIL untagged src/{f.relative_to(src).as_posix()}:{li + 1}: free function `{q}` has "
                             f"no NAME tag and no PDB or symbols name (I52)")


def run_checks(src, pdb, xbe, rev, matched, list_path=MATCHED, accepted=ACCEPTED, base=None, config=None,
               cache=True):
    fails = []
    func_va, data_va, row_name = symbol_starts()
    files, scoped = scoped_files(src, matched)
    baseline = Counter()
    acc = load_accepted(accepted, baseline)
    emitters = emitter_idents(list_path)
    for path, text in sorted(acc):  # accepted: each entry is still a hedge line of its file (not stale or widened)
        f = src / path.removeprefix("src/")
        ls = lines_of(f) if path.startswith("src/") and f.is_file() else []
        if not any(ls[i].strip() == text and HEDGE.search(c) for i, c in comment_spans(ls).items()):
            fails.append(f"FAIL accepted {path}: entry matches no hedge line")
    for p in files:  # hedge: every file
        rel = p.relative_to(src).as_posix()
        ls = lines_of(p)
        for i, text in comment_spans(ls).items():
            m = HEDGE.search(text)
            if m and ("src/" + rel, ls[i].strip()) not in acc:
                fails.append(f"FAIL hedge src/{rel}:{i + 1}: hedge word `{m.group(1)}` in a comment")
        for i, line in enumerate(ls):  # forceinline: every file, `// !FAKE:` or not (D116, D117)
            if FORCEINLINE.search(code_part(line)):
                fails.append(f"FAIL forceinline src/{rel}:{i + 1}: `__forceinline` is refused, `// !FAKE:` or not "
                             f"(D116, D117)")
            if NOINLINE.search(code_part(line)):  # 4.5 T16.2 G9: every file, `// !FAKE:` or not (D116)
                fails.append(f"FAIL noinline src/{rel}:{i + 1}: `noinline` is refused, `// !FAKE:` or not (D116)")
    in_scope = {p for p, _ls in scoped}
    # 4.5 T16.2 G18: NAME, placeholder, d66 `#define` lines and (G9) in-body `#pragma` on every file; src-pdb,
    # src-retail and d66 bodies stay on scoped files. was: `for p, ls in scoped:` (all of them scoped only)
    for p in files:
        ls = lines_of(p)
        scoped_file = p in in_scope
        rel = "src/" + p.relative_to(src).as_posix()
        bs = bodies(ls)
        file_vas = {va for va, _s, _e in bs}
        comments = comment_spans(ls)
        cd = code_depths(ls)
        for _va, s, e in bs:  # G9: `#pragma` inside a function body, `// !FAKE:` or not (D116)
            for i in range(s, e):
                if cd[i][0] > cd[s][0] and PRAGMA.match(cd[i][1]):
                    fails.append(f"FAIL pragma {rel}:{i + 1}: `#pragma` inside a function "
                                 f"body is refused, `// !FAKE:` or not (D116)")
        first = min((s for _va, s, _e in bs), default=None)  # 5.5 T13 (D138): `#pragma intrinsic` at the head only
        for i, (dp, code) in enumerate(cd):
            if INTRINSIC.match(code) and (dp > 0 or (first is not None and i > first)):
                fails.append(f"FAIL pragma-scope {rel}:{i + 1}: `#pragma intrinsic` "
                             f"{'at brace depth ' + str(dp) if dp > 0 else 'after the first FUNCTION line'} (D138)")
            if NORETURN.search(code) and not noreturn_proved(ls, i):  # 5.5 T13 (D138)
                fails.append(f"FAIL attr {rel}:{i + 1}: `__declspec(noreturn)` without a `// !FAKE:` whose reason "
                             f"records a before/after (D138)")
        for i, line in enumerate(ls):  # 5.5 T13 (I43, D33): three non-empty fields on the `!FAKE` line
            if FAKE_TAG in line and not all(x.strip() for x in fake_fields(line)):
                fails.append(f"FAIL fake-fields {rel}:{i + 1}: `// !FAKE:` needs three non-empty `;`-separated fields "
                             f"on its line (kind; instrument; reason)")
        for i, line in enumerate(ls):  # 5.1 T1: a quoted include resolves to an existing file inside src
            inc = INCLUDE.match(line)
            if inc:
                t = (p.parent / inc.group(1)).resolve()
                if not (t.is_file() and t.is_relative_to(src)):
                    fails.append(f"FAIL include {rel}:{i + 1}: quoted `#include \"{inc.group(1)}\"` is "
                                 f"{'outside the src root' if t.is_file() else 'no existing file'}")
        in_body = {i for va, s, e in bs if va in matched for i in range(s, e)}
        cont = False
        for i, (_dp, code) in enumerate(cd):  # G18: d66 on `#define` lines (and their `\` continuations) of every file
            is_def = cont or bool(DEFINE.match(code))
            cont = is_def and ls[i].rstrip().endswith("\\")
            if is_def and i not in in_body:
                w = D66.search(code)
                if w and not has_fake(ls, i):
                    fails.append(f"FAIL d66 {rel}:{i + 1}: `{w.group(0)}` in a `#define` without `// !FAKE:` on or "
                                 f"above the line")
        for i, line in enumerate(ls):
            tag = NAME_TAG.search(line)
            if tag:
                row = at = None
                if line.strip().startswith("//"):
                    ident, at = func_ident(ls, i)
                    va = func_at(bs, i)
                    row = demangle_scope(row_name.get(va, "")) if va is not None else None
                    if row and ident and strip_templates(ident) not in emitters and strip_templates(ident) != row:
                        row = None  # row-name fallback only for [[emitter]] scaffolds or source name = row name
                else:
                    ident = decl_ident(code_part(line))
                    if ANON.match(strip_template_head(code_part(line))):
                        if tag.group(1) == "proto-pdb":
                            for k, en in enumerators(ls, i):
                                if not pdb.resolves(en):
                                    fails.append(f"FAIL proto-pdb {rel}:{k + 1}: `{en}` not in default.pdb names")
                        continue
                if ident is None:
                    fails.append(f"FAIL {'proto-pdb' if tag.group(1) == 'proto-pdb' else 'name-retail'} {rel}:{i + 1}:"
                                 f" no identifier found for the NAME tag")
                elif (tag.group(1) == "proto-pdb"  # 4.1 T4: own-line tags through name_resolves; was: pdb.resolves(ident)
                      and not (name_resolves(pdb, ls, at, ident) if at is not None else pdb.resolves(ident))
                      and not (row and pdb.resolves(row))):
                    fails.append(f"FAIL proto-pdb {rel}:{i + 1}: `{ident}` not in default.pdb names")
                elif tag.group(1) == "retail" and not xbe.names(ident):
                    fails.append(f"FAIL name-retail {rel}:{i + 1}: `{ident}` has no retail string naming it")
            for m in PLACE.finditer(line):
                if m.group(1):
                    kind, va = m.group(1), m.group(2)
                    if int(va, 16) not in (data_va if kind == "global" else func_va | file_vas):
                        fails.append(f"FAIL placeholder {rel}:{i + 1}: `{m.group(0)}` is not a "
                                     f"{'data' if kind == 'global' else 'function'} start")
            d = FIELD_DECL.match(code_part(line))
            if d:
                oc = OFF_COMMENT.search(line)
                if not oc or int(oc.group(1), 16) != int(d.group(1), 16):
                    fails.append(f"FAIL placeholder {rel}:{i + 1}: `field_{d.group(1)}` does not equal its offset "
                                 f"comment")
            if not scoped_file:
                continue
            m = SRC_PDB.search(line)
            if m and not pdb.resolves(m.group(1)):
                fails.append(f"FAIL src-pdb {rel}:{i + 1}: `{m.group(1)}` not in default.pdb names")
            m = SRC_RETAIL.search(line)
            if m:
                got = xbe.cstr_at(int(m.group(1), 16))
                want = m.group(2).encode("latin-1", "replace").decode("unicode_escape").encode("latin-1", "replace")
                if got != want:
                    fails.append(f"FAIL src-retail {rel}:{i + 1}: text does not equal the retail bytes at "
                                 f"0x{int(m.group(1), 16):08X}")
        for va, s, e in bs:
            if va not in matched:
                continue
            for i in range(s, e):
                w = D66.search(code_part(ls[i]))
                if w and not has_fake(ls, i):
                    fails.append(f"FAIL d66 {rel}:{i + 1}: `{w.group(0)}` without `// !FAKE:` on or above the line")
    if rev or base is not None:
        check_fake_diff(src, rev, fails, base)
    check_members(src, files, pdb, fails)  # 5.5 T13.c3
    check_untagged(src, scoped, pdb, fails)
    cov = check_locals(src, scoped, pdb, matched, fails, config, cache)
    return apply_baseline(src, fails, baseline), len(scoped), len(files), cov


def fake_at(ls, i):
    """Index of the `// !FAKE:` line on line i or heading the comment block directly above it, else None."""
    if FAKE_TAG in ls[i]:
        return i
    j = i - 1
    while j >= 0 and ls[j].strip().startswith("//"):
        if ls[j].strip().startswith(FAKE_TAG):
            return j
        j -= 1
    return None


def has_fake(ls, i):
    return fake_at(ls, i) is not None  # 5.5 T13: through fake_at (same lines)


def fake_fields(line):
    """[kind, instrument, reason] after the `// !FAKE:` tag (the reason: the rest of the line, `;` kept), or the fewer
    fields there are (5.5 T13)."""
    f = line.split(FAKE_TAG, 1)[1].split(";")
    return f[:2] + [";".join(f[2:])] if len(f) >= 3 else f + [""] * (3 - len(f))


def noreturn_proved(ls, i):
    """5.5 T13 (D138): a `// !FAKE:` on or above line i whose reason holds a before/after record (`->`)."""
    j = fake_at(ls, i)
    r = fake_fields(ls[j])[2] if j is not None else ""
    return "->" in r and "no before/after recorded" not in r.lower()


def quoted_headers(path, seen=None):
    """Files reachable from path through `#include "..."` (resolved beside the including file), sorted."""
    seen = set() if seen is None else seen
    for line in lines_of(path):
        m = INCLUDE.match(line)
        if m:
            h = (path.parent / m.group(1)).resolve()
            if h.is_file() and h not in seen:
                seen.add(h)
                quoted_headers(h, seen)
    return sorted(seen)


def locals_key(src_file, flags):
    """Cache key: sha1(source, flags, quoted headers) hex."""
    h = hashlib.sha1(src_file.read_bytes())
    h.update(("\0" + " ".join(flags) + "\0").encode())
    for hd in quoted_headers(src_file):
        h.update(hd.read_bytes() + b"\0")
    return h.hexdigest()


def locals_obj(src_file, flags, bindir, cache=True, scratch=None):
    """/Z7 obj of src_file under flags, from the cache keyed by sha1(source, flags, quoted headers); compiled on a miss.
    cache=False (the gate, 3.5 T4 F5): always compiled into the scratch dir, the cache neither read nor written (a cold
    pass at 607 measured 2.6-2.8 s, 8.0 s disk-cold; the key holds no compiler identity and a hit is not re-validated).
    scratch: the compile dir (5.5 T13 G5: check_locals' per-run dir; default LOCALS_SCRATCH)."""
    scratch = LOCALS_SCRATCH if scratch is None else Path(scratch)
    obj = LOCALS_CACHE / f"{locals_key(src_file, flags)}.obj"
    if cache and obj.is_file():  # was: `if obj.is_file()` for every caller (a planted obj trusted by the gate)
        return obj
    scratch.mkdir(parents=True, exist_ok=True)
    tmp = scratch / obj.name
    tmp.unlink(missing_ok=True)
    if not cache:
        ok, codes, _text = msvc71.compile_one(bindir, src_file, flags + ["/Z7"], tmp, scratch=scratch)
        if not ok:
            raise Refuse(f"locals: /Z7 compile of {src_file.name} failed (codes {' '.join(codes) or '-'})")
        return tmp
    LOCALS_CACHE.mkdir(parents=True, exist_ok=True)
    ok, codes, _text = msvc71.compile_one(bindir, src_file, flags + ["/Z7"], tmp, scratch=scratch)
    if not ok:
        raise Refuse(f"locals: /Z7 compile of {src_file.name} failed (codes {' '.join(codes) or '-'})")
    tmp.replace(obj)
    return obj


def obj_procs(obj):
    """-> {COFF symbol: (proc name, [scope_locals records])} of each S_GPROC32/S_LPROC32 in the obj's C13 .debug$S
    sections; the symbol is the one the proc's offset field (payload +28) relocates to."""
    d = obj.read_bytes()
    try:
        c = coff_info.parse(d)
    except coff_info.BadCoff as e:
        raise Refuse(f"locals: {obj.name}: {e}")
    out = {}
    for s in c["sections"]:
        if s["name"] != ".debug$S":
            continue
        b = d[s["rawPtr"]:s["rawPtr"] + s["size"]]
        if len(b) < 4 or struct.unpack_from("<I", b, 0)[0] != 4:
            raise Refuse(f"locals: {obj.name}: .debug$S signature is not C13 (4)")
        rel = {r["offset"]: c["symbols"][r["symbol"]]["name"] for r in s["relocs"]}
        o = 4
        while o + 8 <= len(b):
            kind, n = struct.unpack_from("<II", b, o)
            end = len(b) if n == 0 else o + 8 + n  # a subsection of length 0 runs to the section end (VC7.1 /Z7)
            if kind == 0xF1:  # symbols
                pos = o + 8
                while pos + 4 <= end:
                    ln, k = struct.unpack_from("<HH", b, pos)
                    if ln < 2:
                        break
                    if k in pdb_types.PROC_KINDS and pos + 32 in rel:
                        e = out.setdefault(rel[pos + 32], (pdb_types.cstr(b, pos + 39)[0], []))
                        e[1].extend(pdb_types.scope_locals(b, pos + 2 + ln, end))
                    pos += 2 + ln
            o = (end + 3) & ~3
    return out


def use_line(ls, s, e, name):
    """Index of the first line in [s, e) using name as a whole word outside a // comment, else s."""
    w = re.compile(rf"(?<![\w$]){re.escape(name)}(?![\w$])")
    return next((i for i in range(s, e) if w.search(code_part(ls[i]))), s)


def pdb_local_names(pdb, ls, s, e, proc_name):
    """Names allowed from the PDB: locals of the proc at the PROTOTYPE VA (lines after FUNCTION), else of the procs
    named exactly proc_name."""
    m = next((m for i in range(s + 1, min(s + 3, e)) if (m := PROTO_VA.match(ls[i]))), None)
    if m:
        return {x["name"] for name, seg, off in pdb.proc_at.get(int(m.group(1), 16), ())
                for r in pdb.locals_of(name) if (r["seg"], r["off"]) == (seg, off) for x in r["locals"]}
    return {x["name"] for r in pdb.locals_of(proc_name) for x in r["locals"]}


def check_locals(src, scoped, pdb, matched, fails, config=None, cache=True):
    """locals check over the scoped files; -> coverage {with, without, noproc} (matched functions)."""
    bindir = vv_inputs.load_config(config).get("msvc71_dir")  # 3.5 T4 F4: the gate's config; was: default
    if bindir is None or not (Path(bindir) / "cl.exe").is_file():
        raise Refuse(f"locals: msvc71_dir unset or has no cl.exe: {bindir}")
    try:
        umap = build.unit_map(build.UNITS)
    except build.Refuse as e:
        raise Refuse(f"locals: {e}")
    cov = {"with": 0, "without": 0, "noproc": 0}
    LOCALS_SCRATCH.parent.mkdir(parents=True, exist_ok=True)  # 5.5 T13 G5: a fresh dir per run, only it removed
    scratch = Path(tempfile.mkdtemp(prefix=LOCALS_SCRATCH.name + ".", dir=LOCALS_SCRATCH.parent))
    try:
        for p, ls in scoped:
            rel = p.relative_to(src).as_posix()
            us = umap.get(rel.lower(), [])
            if len(us) != 1:
                raise Refuse(f"locals: {rel} maps to {len(us)} units")
            try:
                flags = build.flags_of(us[0])
            except build.Refuse as e:
                raise Refuse(f"locals: {e}")
            procs = obj_procs(locals_obj(p, flags, Path(bindir), cache, scratch))
            for va, s, e in bodies(ls):
                if va not in matched:
                    continue
                pr = procs.get(matched[va]["symbol"])
                if pr is None:
                    cov["noproc"] += 1  # 5.5 T13 G4: and a fail; was: counted only
                    fails.append(f"FAIL locals src/{rel}:{s + 1}: no obj proc for the matched function (its locals "
                                 f"unchecked)")
                    continue
                cov["with" if pr[1] else "without"] += 1
                allowed = pdb_local_names(pdb, ls, s, e, pr[0]) if pr[1] else set()
                for x in pr[1]:
                    if not NEUTRAL.match(x["name"]) and x["name"] not in allowed:
                        fails.append(f"FAIL locals src/{rel}:{use_line(ls, s, e, x['name']) + 1}: local "
                                     f"`{x['name']}` is neither a PDB local of its proc nor neutral")
                    elif allowed and x["name"] not in allowed and not has_fake(ls, u := use_line(ls, s, e, x["name"])):
                        fails.append(f"FAIL neutral src/{rel}:{u + 1}: neutral local `{x['name']}` is not a PDB local "
                                     f"of its proc and has no `// !FAKE:` on or above its first use")  # 5.5 T13 (S5)
    finally:
        shutil.rmtree(scratch, ignore_errors=True)  # was: rmtree(LOCALS_SCRATCH) (another run's scratch too)
    return cov


def census(scoped, src, pdb):
    rows = []
    for p, ls in scoped:
        rel = "src/" + p.relative_to(src).as_posix()
        comments = comment_spans(ls)
        for i, line in enumerate(ls):
            code = code_part(line) if i in comments else line
            for m in CAST.finditer(code):
                before = code[:m.start()].rstrip()
                if before and (before[-1].isalnum() or before[-1] == "_") and not before.endswith("return"):
                    continue
                if NOT_CAST_NEXT.match(code[m.end():]):
                    continue
                t = m.group(1)
                if (t in pdb.full and t in {strip_templates(k) for k in pdb.members}) or t in pdb.typedefs:
                    rows.append(f"- `{rel}:{i + 1}` `{t}*`")
    head = ["# Cast census (generated)", "",
            "Generated by `PY tools/provenance_lint.py --casts`; never hand-edited. Every C-style cast to a pointer to a",
            "PDB-named type (default.pdb TPI, or an S_UDT typedef of a class/struct/union) in the matched sources:",
            "`file:line` and the cast's type.", "",
            f"{len(rows)} cast(s).", ""]
    CENSUS.write_text("\n".join(head + rows) + "\n", encoding="utf-8", newline="\n")
    return len(rows)


def pinned(path, role, recs):  # -> path; Refuse unless its size + sha256 equal the inputs manifest row (3.5 T4 F6)
    want, data = recs.get((role, path.name)), path.read_bytes()
    if want is None or want["size"] != len(data) or want["sha256"] != hashlib.sha256(data).hexdigest():
        raise Refuse(f"input {role}/{path.name} does not equal its config/inputs.manifest.jsonl row")
    return path


def inputs(config=None):
    cfg = vv_inputs.load_config(config)  # 3.5 T4 F4: the gate's config; was: default only
    try:  # 3.5 T4 F6: each file checked against the inputs manifest; was: read with no hash check
        recs = {(r["role"], r["name"]): r for r in vv_inputs.load_manifest()}
    except SystemExit as e:  # vv_inputs refuses by exit 2 (message on stderr)
        raise Refuse(f"inputs manifest unreadable (vv_inputs exit {e.code})")
    return (Pdb(pinned(vv_inputs.input_path("prototype_dir", "default.pdb", cfg), "prototype", recs),
                pinned(vv_inputs.input_path("prototype_dir", "default.exe", cfg), "prototype", recs)),
            Xbe(pinned(vv_inputs.input_path("retail_xbe", cfg=cfg), "retail_xbe", recs)))


def matched_vas(path=MATCHED):
    """-> {va: [[function]] entry} of the matched set at path (membership tests read it as a VA set)."""
    try:
        return {e["va"]: e for e in matched_set.load(path) if e["kind"] == "function"}
    except matched_set.Refuse as e:
        raise Refuse(f"matched set: {e}")


def emitter_idents(path=MATCHED):
    """Source identifiers (demangle_scope) of the matched set's [[emitter]] symbols."""
    try:
        return {demangle_scope(e["symbol"]) for e in matched_set.load(path) if e["kind"] == "emitter"} - {None}
    except matched_set.Refuse as e:
        raise Refuse(f"matched set: {e}")


_INPUTS = {}  # config -> (pdb, xbe); 3.5 T4 F4; was: one list (the default config's)


def lint(src, list_path=MATCHED, rev=UPSTREAM, accepted=ACCEPTED, base=None, config=None, cache=True):
    """Library entry (gate `provenance` layer, T7): -> (fails, nscoped, nfiles, cov); raises Refuse.
    PDB and XBE are parsed once per process per config (cached); config: the gate's --config (3.5 T4 F4)."""
    src = Path(src).resolve()
    if not src.is_dir():
        raise Refuse(f"missing src dir {src}")
    matched = matched_vas(list_path)
    if config not in _INPUTS:
        _INPUTS[config] = inputs(config)
    pdb, xbe = _INPUTS[config]
    return run_checks(src, pdb, xbe, rev, matched, list_path, accepted, base, config, cache)


def counts(fails):
    return {c: sum(1 for f in fails if f.startswith(f"FAIL {c} ")) for c in CHECKS}


def self_test(src, pdb, xbe, matched, list_path=MATCHED, accepted=ACCEPTED):
    if SCRATCH.exists():
        shutil.rmtree(SCRATCH)
    dst = SCRATCH / "src"
    shutil.copytree(src, dst)
    try:
        base = counts(run_checks(dst, pdb, xbe, None, matched, list_path, accepted)[0])
        _files, scoped = scoped_files(dst, matched)
        # 5.5 T13.c3: plant in the first matched body whose `{` is at its FUNCTION line's depth and whose PROTOTYPE
        # proc has PDB locals (was: scoped[0]'s first `{` in any matched extent, a pin_emit_ scaffold in
        # `namespace std {`, so locals, pragma and neutral did not fail)
        p, ls, va, s, k = next((p, list(ls), va, s, k) for p, ls in scoped for cd in [code_depths(ls)]
                               for va, s, e in bodies(ls)
                               if va in matched and s + 1 < e and PROTO_VA.match(ls[s + 1])
                               for k in [next((i for i in range(s, e) if ls[i].strip() == "{"), None)]
                               if k is not None and cd[k][0] == cd[s][0] and pdb_local_names(pdb, ls, s, e, ""))
        ls.insert(k + 1, "#pragma warning(disable:4100)")  # 4.5 T16.2: an in-body pragma (pragma control)
        ls.insert(k + 1, "    volatile int zz_selftest_local = 0;")  # a free-named stack local (locals control)
        ls.insert(k + 1, "    volatile int v99 = 0;")  # 5.5 T13: a neutral local, no `!FAKE` (neutral control; fails
        # only where the proc has PDB locals)
        ls.insert(s + 1, "volatile int zz_selftest_d66;")
        ls += ["struct ZzSelftestNotInPdb; // NAME: proto-pdb:default.pdb",
               "int function_00000001;",
               "// src proto-pdb:default.pdb ZzSelftestNotInPdb",
               f'// src retail 0x{va:08X} "zz_selftest_not_retail"',
               "// this is probably a planted hedge",
               "struct ZzSelftestNotInXbe; // NAME: retail",
               "// !FAKE: inliner; __forceinline; planted",
               "__forceinline int zz_selftest_forceinline() { return 0; }",
               "__declspec(noinline) int zz_selftest_noinline();",  # 4.5 T16.2 (noinline control)
               "__declspec(noreturn) void zz_selftest_attr();",  # 5.5 T13 (attr control)
               "#pragma intrinsic(memcpy)",  # 5.5 T13 (pragma-scope control: after the first FUNCTION line)
               "// !FAKE: inliner; planted",  # 5.5 T13 (fake-fields control: two fields)
               "struct ZzSelftestMembers { int zz_selftest_member; };",  # 5.5 T13.c3 (members control)
               "int zz_selftest_helper() { return 0; }",  # 5.5 T13.c3 (untagged control)
               "#if 0", '#include "../zz_selftest_missing.h"', "#endif"]  # 5.1 T1 (include control: a missing
        # target; under `#if 0` so the locals /Z7 compile still succeeds, the lint reads every arm)
        p.write_text("\n".join(ls) + "\n", encoding="utf-8", newline="\n")
        acc = SCRATCH / "accepted.txt"  # accepted control: a real line widened by a hedge word
        acc.write_text((Path(accepted).read_text(encoding="utf-8") if Path(accepted).is_file() else "")
                       + f"src/{p.relative_to(dst).as_posix()}\t{ls[s].strip()} probably\tplanted\n",
                       encoding="utf-8", newline="\n")
        got = counts(run_checks(dst, pdb, xbe, None, matched, list_path, acc)[0])
        old = [f"// FUNCTION: USA 0x{va:08X}", "// NAME: proto-pdb:default.pdb", "void f()", "{",
               "// !FAKE: c2 register allocation; dummy local; planted", "    int v0 = 0;", "}"]
        new = old[:4] + old[5:]
        got["fake-diff"] = base["fake-diff"] + len(fake_pairs(old, new))
        ok = [c for c in CHECKS if got[c] > base[c]]
        for c in CHECKS:
            print(f"control {c}: {'failed as expected' if c in ok else 'DID NOT FAIL'}")
        print(f"controls: {len(ok)} failed as expected of {len(CHECKS)}")
        return 0 if len(ok) == len(CHECKS) else 1
    finally:
        shutil.rmtree(SCRATCH, ignore_errors=True)


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--src", default=str(REPO / "src"))
    ap.add_argument("--list", default=str(MATCHED), metavar="TOML")
    ap.add_argument("--accepted", default=str(ACCEPTED), metavar="TXT")
    ap.add_argument("--diff", default="HEAD", metavar="REV")
    ap.add_argument("--casts", action="store_true")
    ap.add_argument("--self-test", action="store_true")
    a = ap.parse_args(argv)
    t0 = time.time()
    src = Path(a.src).resolve()
    try:
        if not src.is_dir():
            raise Refuse(f"missing src dir {src}")
        matched = matched_vas(a.list)
        pdb, xbe = inputs()
        if a.self_test:
            return self_test(src, pdb, xbe, matched, a.list, a.accepted)
        if a.casts:
            _files, scoped = scoped_files(src, matched)
            n = census(scoped, src, pdb)
            print(f"provenance_lint: cast census {n} cast(s) -> {CENSUS.relative_to(REPO).as_posix()}")
            return 0
        fails, nscoped, nfiles, cov = run_checks(src, pdb, xbe, a.diff, matched, a.list, a.accepted)
    except Refuse as e:
        print(f"provenance_lint: REFUSED {e}", file=sys.stderr)
        return 2
    for f in fails:
        print(f)
    c = counts(fails)
    print(f"provenance_lint: {'FAIL' if fails else 'PASS'} {len(fails)} failure(s) "
          f"({', '.join(f'{k} {v}' for k, v in c.items())}); {nscoped} matched file(s), {nfiles} source file(s); "
          f"locals coverage: {cov['with']} function(s) with obj locals, {cov['without']} without, "
          f"{cov['noproc']} no obj proc; {time.time() - t0:.1f}s")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
