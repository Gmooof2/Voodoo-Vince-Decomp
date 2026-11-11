"""Public firewall audit (T8, phase 1.1): five content classes over tracked, staged and historical blobs.

CLI: audit_public.py [--worktree|--staged|--history|--range OLD..NEW|--paths P...]   default: worktree, staged, history
     audit_public.py --self-test   (scratch repos under .run/1.1/audit_selftest/; deleted after)
Classes: forbidden-hash (content sha1 in manifest + config/check.*.sha), magic (offset 0), oversize
(not for paths under .claude-state/journal/), toolchain (name or magic),
text-run (x86 disassembly / hex-dump line runs in text blobs). Rules: docs/ops/INDEX.md.
5.5 T14.c1: magic also fails closed on an archive at offset 0 (zip, 7z, rar 4/5, xz, bzip2, cab, zstd; tar: `ustar` at
257), magic only, not toolchain. A blob starting with a UTF-32 or UTF-16 BOM (UTF-32 tested first) is transcoded to
UTF-8 (errors replaced) and every class runs on that as text, as gzip blobs run on their decompressed bytes;
forbidden-hash and oversize also on the raw bytes.
address (T7 phase 2.1): the local config's xbox_ip (read at run time, never in source) as a whole token; a blob
holding it passes iff its git blob id is in config/audit_address_accepted.txt, or every line of it holding the
address occurs in a listed blob (in .claude-state/journal/INDEX-*.md only, compared without the trailing ' | ' link
field on both sides, T8 phase 2.4). xbox_ip null: `<scope> address: not checked ...` on stdout and stderr, exit unchanged.
boot-folder, original-install (T4 phase 2.6): the last path component of the local config's boot_folder /
original_install, case-insensitive whole word ([A-Za-z0-9_] boundaries); same pass rules as address, one shared list
config/audit_private_accepted.txt. Key absent or null: `not checked` as for address; non-string or empty: refusal.
user-path (T3 phase 3.6): only in a tree holding the marker file config/PUBLIC_TREE, over text blobs: a Windows user
folder with a name after it, drive form or MSYS form, any case, any run of separators. No accept list. Without the
marker the class does not run and prints no line, so this private repo's own history (which holds such paths) stays out.
audit_public.py --public-ci (T3 phase 3.6): marker tree only, for a host with no local config or disc: tracked tree and
full history (refuses a shallow clone), all classes but the three value classes, which print `not checked`. Exit 2 if
the marker is absent or the manifest missing, empty or off its sidecar (no disc, so no coverage check). Limit: it does
not check private values and does not prove bytes. Default mode, hooks and gate are unchanged and never pass it.
Claims only these classes, never the absence of all game-derived content.
Speed route (T3.1 phase 4.1), one code path, within one process, nothing stored, every default scope still read whole:
max_runs memoises the two line tests per distinct line (line splitting and run counting stay per line); it rests on
_is_asm and _is_hex reading only the line. address_hit tests the rule's literals before its regex; it rests on every value
regex being lookarounds around a required literal (_literal_rx builds them and refuses anything else), and, for a
case-insensitive rule, on re.I over bytes folding as bytes.lower() does (self-test: all 65,536 byte pairs). _route_pin
checks the two line tests and both regex builders first thing in main: a cause is exit 2. user-path has no prefilter.
Exit: 0 clean; 1 offenders; 2 refusal (manifest/config/accept list missing, empty or uninterpretable; manifest
stale; a listed blob id missing from the object store; 4.5 T16.2: the history scope on a shallow clone, `--paths`
with no file). A history blob tracked under several paths is classified under each (4.5 T16.2).
Prints paths, oids, sizes and counts only, never file content.
Test overrides (env): AUDIT_PUBLIC_MANIFEST, AUDIT_PUBLIC_CONFIG (check.*.sha are read beside the manifest).
History cache (5.4 T5.c5), opt-in, before the scope args: `--cache FILE` writes FILE, `--warm` (needs --cache) reads it
too. FILE: JSON {key, clean: sorted ["<oid> <path>"], binary: sorted [oid of a clean non-text blob], heads: sorted
[tip commit at the scan, `rev-list --no-walk --all`] (5.5 T12.c3, I73 F3)}, written atomically
after a run with the history scope, holding only pairs clean in this run (a warm run's cache hits included). The key
(history_key) is a sha256 over everything a history verdict reads; a missing, unreadable, malformed, heads-less or
other-key file is a full miss. Warm, the history scope skips `cat-file` for an oid whose every path has a clean entry,
except a blob reachable from a commit not under the recorded heads (`rev-list --objects --all --not <heads>`): its
pairs are dropped first, so a pair forged for a commit made after the write is read; rev-list, log --raw
and --batch-check still run. One stderr line: `audit: history cache <hits> of <n> (key ok|changed|none)` (cold: none).
"""
import contextlib
import hashlib
import io
import json
import os
import re
import shutil
import stat
import struct
import subprocess
import sys
import zlib
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))
import vv_inputs  # noqa: E402

SELFTEST_DIR = REPO / ".run" / "1.1" / "audit_selftest"
CLASSES = ("forbidden-hash", "magic", "oversize", "toolchain", "text-run", "address", "boot-folder",
           "original-install", "user-path")
ACCEPT = REPO / "config" / "audit_address_accepted.txt"  # address class: accepted blob ids (T7 phase 2.1)
ACCEPT_PRIVATE = REPO / "config" / "audit_private_accepted.txt"  # boot-folder, original-install (T4 phase 2.6)
VALUE_KEYS = {"address": "xbox_ip", "boot-folder": "boot_folder", "original-install": "original_install"}
PUBLIC_MARKER = "PUBLIC_TREE"  # config/<this> present: the tree is public and the user-path class runs (T3 phase 3.6)
PUBLIC_FLAG = "--public-ci"  # the public repo's run with no local config; see public_ci
# drive or MSYS root, 'Users', separator, one character a Windows name may hold; a '<you>' placeholder holds none
USER_PATH = re.compile(rb"""(?:(?<![A-Za-z0-9_])[A-Za-z]:[\\/]+|/[A-Za-z]/+)Users[\\/]+[^\s\\/:*?"<>|]""", re.I)
OVERSIZE = 1_572_864  # 1.5 MiB: largest history blob 857,215 (journal); retail XBE 2,453,504
GZ_MAGIC = b"\x1f\x8b"  # 4.5 T16.1 G8: gzip blobs are audited on their decompressed bytes
GZ_CAP = 64 << 20  # 64 MiB decompressed per blob: a gzip-bomb bound; largest tracked ledger 18,221,397 (4.5 T16.1);
# over it, or not valid gzip = an oversize hit (fail closed, OVERSIZE_EXEMPT does not apply)
BOMS = ((b"\xff\xfe\0\0", "utf-32"), (b"\0\0\xfe\xff", "utf-32"),  # 5.5 T14.c1: UTF-32 first (its LE BOM starts
        (b"\xff\xfe", "utf-16"), (b"\xfe\xff", "utf-16"))  # with UTF-16's); such a blob is audited transcoded to UTF-8
ARCHIVE_MAGICS = (b"PK\x03\x04", b"PK\x05\x06", b"PK\x07\x08", b"7z\xbc\xaf\x27\x1c",  # 5.5 T14.c1: zip, 7z,
                  b"Rar!\x1a\x07\x00", b"Rar!\x1a\x07\x01\x00", b"\xfd7zXZ\x00", b"MSCF",  # rar 4/5, xz, cab,
                  b"\x28\xb5\x2f\xfd")  # zstd; bzip2 and tar (ustar at 257) in _archive; a magic hit (fail closed)
INDEX_RE = re.compile(r"\.claude-state/journal/INDEX-[^/]*\.md")  # address: link field dropped (T8 phase 2.4)
OVERSIZE_EXEMPT = (".claude-state/journal/", ".claude-state/ledger/")  # repo-relative prefixes; only the oversize
# class is skipped (journal 1.7 T7, ledger slices 4.4 T1); was: ".claude-state/journal/"
TOOL_SUFFIXES = {".exe", ".dll", ".lib", ".obj", ".pdb", ".xbe", ".iso", ".bin",
                 ".ilk", ".exp", ".pch", ".idb"}
MSF7 = b"Microsoft C/C++ MSF 7.00\r\n\x1a"
AR_MAGIC = b"!<arch>\n"
SNIFF = 8192
CHECK_LINE = re.compile(r"^[0-9a-f]{40}  \S+$")
SHA1_RE = re.compile(r"^[0-9a-f]{40}$")
COVER = (("retail_xbe", "retail_xbe"), ("retail_iso", "retail_iso"), ("preview_iso", "preview_iso"),
         ("preview_cue", "preview_cue"), ("prototype_dir", "prototype"), ("xbdm_dll", "xbdm_dll"))
NOT_INPUTS = ("game_dir", "ghidra_project_dir", "ghidra_dir", "java_home", "objdiff_cli", "xbox_ip", "msvc71_dir")

# text-run shapes (case-insensitive); thresholds = consecutive matching lines (calibration: docs/ops/INDEX.md)
_REG = r"%?(?:e?[abcd]x|[abcd][lh]|e?[sd]i|e?[sb]p|st\(?[0-7]\)?|xmm[0-7]|mm[0-7])\b"
_JCC = r"j(?:n?[abceglopsz]|n?[abgl]e|p[eo]|ecxz|mp)"
_MNEM = (r"(?:mov(?:zx|sx|s[bdw]|ss|sd|aps|ups|d|q)?|lea|push|pop|call|" + _JCC + r"|cmp|test|add|adc|sub|sbb|xor"
         r"|and|or|not|neg|inc|dec|i?mul|i?div|sh[lr]d?|sa[rl]|ro[lr]|rc[lr]|xchg|cmov[a-z]{1,3}|set[a-z]{1,3}"
         r"|fi?ld|fi?stp?|fi?(?:add|sub|subr|mul|div|divr)p?|fxch|fu?comp{0,2}|fnstsw|rep[a-z]{0,2}|stos[bdw]"
         r"|lods[bdw]|bt[crs]?|bswap|(?:add|sub|mul|div|sqrt|ucomi|comi)ss|cvt[a-z]{2,6}|xorps|andps|pxor)[lbwq]?")
_OPER = (r"\s+(?:(?:byte|word|dword|qword|tbyte|xmmword)\s+ptr\b|(?:[cdefgs]s:)?\[|" + _REG
         + r"|(?:FUN_|sub_|LAB_|loc_)[0-9a-f]+|(?:0x)?[0-9a-f]{5,}h?\b|[^\s,]{1,40}\s*,\s*(?:\[|" + _REG + r"))")
_PFX = r"^\s*(?:(?:0x)?[0-9a-f]{4,16}h?:?\s+)?(?:(?:[0-9a-f]{2}\s){1,15}\s*)?"
ASM_RE = re.compile(_PFX + _MNEM + _OPER, re.I)
ASM_BARE_RE = re.compile(_PFX + r"(?:retn?|leave|cdq|nop|int3|pushad|popad|pushfd|popfd)(?:\s+(?:0x)?[0-9a-f]+h?)?\s*$",
                         re.I)
_BYTE = r"\b(?:0x)?[0-9a-f]{2}\b"
HEX_RE = re.compile(r"(?:" + _BYTE + r"(?:[ \t]*,[ \t]*|[ \t]+)){7,}" + _BYTE, re.I)
HEXESC_RE = re.compile(r"(?:\\x[0-9a-f]{2}){8,}", re.I)
RUN_N = {"disasm": 4, "hexdump": 3}  # legit history max: disasm 0-2, hexdump 2 (tools/xbe_pristine.py tables)


def _is_asm(line):
    return bool(ASM_RE.match(line) or ASM_BARE_RE.match(line))


def _is_hex(line):
    return bool(HEX_RE.search(line) or HEXESC_RE.search(line))


_LINE_MEMO = {}  # line: (_is_asm, _is_hex); this process only (T3.1 phase 4.1, pinned by _route_pin)


def max_runs(data):
    """{shape: (longest run of consecutive matching lines, 1-based first line)} for a text blob."""
    memo = _LINE_MEMO
    best_a = best_h = (0, 0)
    cur_a = cur_h = 0
    for i, line in enumerate(data.decode("latin-1").splitlines(), 1):
        r = memo.get(line)
        if r is None:
            r = memo[line] = (_is_asm(line), _is_hex(line))
        cur_a = cur_a + 1 if r[0] else 0
        if cur_a > best_a[0]:
            best_a = (cur_a, i - cur_a + 1)
        cur_h = cur_h + 1 if r[1] else 0
        if cur_h > best_h[0]:
            best_h = (cur_h, i - cur_h + 1)
    return {"disasm": best_a, "hexdump": best_h}


def _magic(data):
    if data[:4] == b"XBEH" or data.startswith(MSF7):
        return True
    binary = b"\0" in data[:SNIFF]
    if data[:2] == b"MZ":
        if binary:
            return True
        if len(data) >= 0x40:
            pe = struct.unpack_from("<I", data, 0x3C)[0]
            if data[pe:pe + 4] == b"PE\0\0":
                return True
    if binary and len(data) >= 20:
        machine, nsec = struct.unpack_from("<HH", data, 0)
        opt = struct.unpack_from("<H", data, 16)[0]
        if machine == 0x014C and opt in (0, 0xE0) and 1 <= nsec <= 96:
            return True
    return False


def _archive(data):
    """5.5 T14.c1: an archive magic at offset 0 (tar: `ustar` at 257); gzip has its own branch in audit."""
    return (data.startswith(ARCHIVE_MAGICS) or (data[:3] == b"BZh" and data[3:4] in b"123456789" and len(data) > 3)
            or data[257:262] == b"ustar")


def _bom(data):
    """5.5 T14.c1: the codec for a blob starting with a UTF-32 or UTF-16 BOM, else None."""
    return next((codec for bom, codec in BOMS if data.startswith(bom)), None)


def _gunzip(data):
    """(decompressed bytes, at most GZ_CAP, of every member; ok) for a gzip blob; not ok: over GZ_CAP or not valid gzip."""
    out, n, rest = [], 0, data
    while rest:
        d = zlib.decompressobj(16 + zlib.MAX_WBITS)
        try:
            chunk = d.decompress(rest, GZ_CAP - n + 1)
        except zlib.error:
            return b"".join(out), False
        out.append(chunk)
        n += len(chunk)
        if n > GZ_CAP or not d.eof:  # over the cap, or a truncated member
            return b"".join(out)[:GZ_CAP], False
        rest = d.unused_data
    return b"".join(out), True


def classify(path, data, forbidden, text=None):
    """(set of classes hit, is_text) for one blob; text True: treat as text (a transcoded blob, 5.5 T14.c1)."""
    hits = set()
    if hashlib.sha1(data).hexdigest() in forbidden:
        hits.add("forbidden-hash")
    magic = _magic(data)
    if magic or _archive(data):  # 5.5 T14.c1: + archive magics (magic only, not toolchain)
        hits.add("magic")
    if len(data) > OVERSIZE and not path.replace("\\", "/").startswith(OVERSIZE_EXEMPT):
        hits.add("oversize")
    if magic or data.startswith(AR_MAGIC) or Path(path).suffix.lower() in TOOL_SUFFIXES:
        hits.add("toolchain")
    is_text = text or b"\0" not in data[:SNIFF]
    if is_text:
        runs = max_runs(data)
        if any(runs[s][0] >= RUN_N[s] for s in RUN_N):
            hits.add("text-run")
    return hits, is_text


# ---- git plumbing ----

def _git(args, cwd, env, inp=None):
    r = subprocess.run(["git", *args], cwd=cwd, env=env, input=inp, capture_output=True)
    if r.returncode != 0:
        print(f"audit_public: git {' '.join(args[:3])} failed: {r.stderr.decode(errors='replace').strip()}",
              file=sys.stderr)
        sys.exit(2)
    return r.stdout


def _cat_blobs(oids, cwd, env):
    """Yield (oid, bytes) via one `git cat-file --batch`, one round trip per object."""
    p = subprocess.Popen(["git", "cat-file", "--batch"], cwd=cwd, env=env,
                         stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    try:
        for oid in oids:
            p.stdin.write(oid.encode() + b"\n")
            p.stdin.flush()
            head = p.stdout.readline().split()
            if len(head) != 3:
                print(f"audit_public: cat-file: missing object {oid}", file=sys.stderr)
                sys.exit(2)
            size = int(head[2])
            data = p.stdout.read(size)
            delim = p.stdout.read(1)
            if len(data) != size or delim != b"\n":  # 4.5 T16.1 G7: a short stream refuses
                print(f"audit_public: cat-file: short read of {oid} ({len(data)} of {size} bytes, delimiter "
                      f"{delim!r})", file=sys.stderr)
                sys.exit(2)
            yield oid, data
        p.stdin.close()
        if p.wait() != 0:  # 4.5 T16.1 G7: after a full run
            print(f"audit_public: cat-file: rc {p.returncode}", file=sys.stderr)
            sys.exit(2)
        # was: data = p.stdout.read(size); p.stdout.read(1); yield oid, data  (length, delimiter and rc unchecked)
    finally:
        p.stdin.close()
        p.wait()


def _objects_blobs(revargs, cwd, env):
    """[(oid, path)] of blobs from `rev-list --objects <revargs>`, filtered with --batch-check; one pair per path a
    blob is tracked under (4.5 T16.2 G10: a blob at a.txt and b.exe is classified under both)."""
    lines = _git(["rev-list", "--objects", *revargs], cwd, env).decode("utf-8", "replace").splitlines()
    paths = {}
    for ln in lines:
        oid, _, path = ln.partition(" ")
        paths.setdefault(oid, set()).add(path)  # was: paths.setdefault(oid, path) (first path only)
    if not paths:
        return []
    # rev-list prints an object once (first path); every (blob, path) pair enters as an add/modify in a commit diff
    raw = _git(["log", *revargs, "-m", "--root", "--raw", "--no-renames", "--no-abbrev", "-z", "--format="],
               cwd, env).decode("utf-8", "replace").split("\0")
    for i, tok in enumerate(raw[:-1]):
        f = tok.lstrip("\n").split()
        if len(f) == 5 and f[0].startswith(":") and f[4] != "D" and f[3] in paths:
            paths[f[3]].add(raw[i + 1])
    out = _git(["cat-file", "--batch-check"], cwd, env, inp="\n".join(paths).encode() + b"\n").decode()
    blobs = [ln.split()[0] for ln in out.splitlines() if ln.split()[1:2] == ["blob"]]
    return sorted((oid, p) for oid in blobs for p in paths[oid])


def iter_scope(scope, cwd, env, arg=None, cached=None):
    """Yield (path, oid or None, data) for a scope. cached (history only, 5.4 T5.c5): {(oid, path)} cached clean; an oid
    whose every path is in it is not read and yields data None under each path, after the read ones."""
    if scope == "worktree":
        names = _git(["ls-files", "-z"], cwd, env).decode("utf-8", "replace").split("\0")
        for n in sorted(set(filter(None, names))):
            f = Path(cwd) / n
            if f.is_file():
                yield n, None, f.read_bytes()
    elif scope == "paths":
        for n in arg:
            yield n, None, Path(n).read_bytes()
    elif scope == "staged":
        ents = []
        for rec in _git(["ls-files", "-s", "-z"], cwd, env).decode("utf-8", "replace").split("\0"):
            if not rec:
                continue
            meta, _, path = rec.partition("\t")
            mode, oid, _stage = meta.split()
            if mode != "160000":
                ents.append((path, oid))
        ents.sort()
        data = dict(_cat_blobs(sorted({o for _, o in ents}), cwd, env))
        for path, oid in ents:
            yield path, oid, data[oid]
    else:  # history / range
        if scope == "history" and _git(["rev-parse", "--is-shallow-repository"], cwd, env).strip() != b"false":
            # 4.5 T16.2 G11: every history scope, not only --public-ci; was: no check here
            _refuse("history scope refused: shallow clone, the history would be partial (fetch the full history)")
        blobs = _objects_blobs(["--all"] if scope == "history" else [arg], cwd, env)
        paths = {}
        for oid, path in blobs:
            paths.setdefault(oid, []).append(path)
        skip = {o for o, ps in paths.items() if all((o, p) in cached for p in ps)} if cached else set()
        for oid, data in _cat_blobs([o for o in paths if o not in skip], cwd, env):  # each blob read once, yielded
            for path in paths[oid]:  # under every path
                yield path, oid, data
        for oid in sorted(skip):  # 5.4 T5.c5: cached clean under every path; was: none skipped
            for path in paths[oid]:
                yield path, oid, None
        # was: paths = dict(blobs); for oid, data in _cat_blobs([o for o, _ in blobs], ...): yield paths[oid], oid, data


def _blob_id(data):
    return hashlib.sha1(b"blob %d\0" % len(data) + data).hexdigest()


def _unlinked(ln):
    """Journal index line without its trailing link field (last ' | ' field); None if it has none (T8 phase 2.4)."""
    return ln.rsplit(b" | ", 1)[0] if b" | " in ln else None


_LOWERED = (None, None)  # (blob, blob.lower()): the last folded blob, shared by the rules on one blob


def address_hit(data, addr, rel=None):
    """True if data holds the address and is neither a listed blob nor made only of listed address lines.
    For INDEX_RE paths only, a line also passes when its text without the link field matches a listed line's."""
    global _LOWERED
    rx, ids, lines, unlinked, literals, fold = addr
    hay = data
    if fold:
        if _LOWERED[0] is not data:  # the reference it holds keeps the identity test sound
            _LOWERED = (data, data.lower())
        hay = _LOWERED[1]
    if not any(x in hay for x in literals):  # the regex cannot match without one (_literal_rx)
        return False
    if not rx.search(data) or _blob_id(data) in ids:
        return False
    index = rel is not None and INDEX_RE.fullmatch(rel) is not None
    return not all(ln in lines or (index and _unlinked(ln) is not None and _unlinked(ln) in unlinked)
                   for ln in data.splitlines() if rx.search(ln))


# whole lookarounds with no nested group, or nothing: they consume nothing, so the literal stays required
_LOOKAROUNDS = re.compile(rb"(?:\(\?<?[=!](?:[^()\\]|\\.)*\))*")


def _literal_rx(literals, before, after, flags=0):
    """(before + (?:escaped|literals) + after, literals to test, fold); fold (re.I) tests lower-cased literals against
    the lower-cased blob. Exit 2 on no or an empty literal, or a before/after that is anything but lookarounds."""
    if not literals or not all(literals):
        _refuse("value regex: no literal, or an empty one")
    if not _LOOKAROUNDS.fullmatch(before) or not _LOOKAROUNDS.fullmatch(after):
        _refuse("value regex: before and after may hold lookarounds only")
    fold = bool(flags & re.I)
    rx = re.compile(before + b"(?:" + b"|".join(re.escape(x) for x in literals) + b")" + after, flags)
    return rx, tuple(x.lower() if fold else x for x in literals), fold


def _address_rx(ips):
    """Address tokens: case-sensitive, not inside a longer dotted number."""
    return _literal_rx([i.encode() for i in ips], rb"(?<![\d.])", rb"(?!\.?\d)")


def _name_rx(name):
    """Folder name: case-insensitive whole word."""
    return _literal_rx([name.encode()], rb"(?<![A-Za-z0-9_])", rb"(?![A-Za-z0-9_])", re.I)


def address_rule(ips, accept_path, cwd, env):
    """None if ips is None (class not checked); else value_rule for the address regex."""
    if ips is None:
        return None
    ips = [ips] if isinstance(ips, str) else ips
    if not isinstance(ips, list) or not ips or not all(isinstance(i, str) and i for i in ips):
        _refuse("config xbox_ip must be null, a string or a list of strings")
    rx, literals, fold = _address_rx(ips)
    return value_rule(rx, literals, fold, accept_path, cwd, env)


def name_rule(value, key, accept_path, cwd, env):
    """None if value is None (class not checked); else value_rule for the folder name (last component of value,
    case-insensitive whole word). The value is never printed (T4 phase 2.6)."""
    if value is None:
        return None
    if not isinstance(value, str) or not value:
        _refuse(f"config {key} must be absent, null or a non-empty string")
    name = re.split(r"[\\/]", value.rstrip("\\/"))[-1]
    if not name:
        _refuse(f"config {key} has no folder name")
    rx, literals, fold = _name_rx(name)
    return value_rule(rx, literals, fold, accept_path, cwd, env)


def value_rule(rx, literals, fold, accept_path, cwd, env):
    """(regex, accepted blob ids, accepted lines matching rx, those lines without their link field, literals, fold)."""
    if not literals or not all(literals):
        _refuse("value rule: no literal, or an empty one")
    p = Path(accept_path)
    if not p.is_file():
        _refuse(f"accept list missing: {p}")
    ids = []
    for n, ln in enumerate(p.read_bytes().decode("ascii", "replace").splitlines(), 1):
        if ln.startswith("#"):
            continue
        if not SHA1_RE.match(ln):
            _refuse(f"{p.name}:{n}: line is neither '#' nor a blob id")  # line not printed: it may hold the address
        ids.append(ln)
    ids = sorted(set(ids))
    lines = set()
    if ids:
        out = _git(["cat-file", "--batch-check"], cwd, env, inp="\n".join(ids).encode() + b"\n").decode()
        bad = [ln.split()[0] for ln in out.splitlines() if ln.split()[1:2] != ["blob"]]
        if bad:
            _refuse(f"{p.name}: listed id(s) not a blob in the object store: {', '.join(bad)}")
        for _oid, data in _cat_blobs(ids, cwd, env):
            lines.update(ln for ln in data.splitlines() if rx.search(ln))
    unlinked = frozenset(u for u in map(_unlinked, lines) if u is not None)
    return rx, frozenset(ids), frozenset(lines), unlinked, literals, fold


def audit(scope, cwd, env, forbidden, arg=None, rules=None, cache=None):
    """Return (counts {class: [hits, scanned]}, hits [(class, path, oid, commit)]); rules {value class: rule};
    a value class absent from rules or None there is unchecked. cache: a HistoryCache, used by the history scope only."""
    rules = rules or {}
    counts = {c: [0, 0] for c in CLASSES if c != "user-path" or rules.get(c) is not None}
    hits = []
    hc = cache if scope == "history" else None
    if hc:  # 5.5 T12.c3 (I73 F3): tips recorded, pairs under newer commits dropped, before iter_scope takes hc.clean
        hc.begin(cwd, env)
    for path, oid, data in iter_scope(scope, cwd, env, arg, hc.clean if hc else None):
        if data is None:  # 5.4 T5.c5: a cached clean pair (same key): no class, text as recorded
            found, is_text = set(), oid not in hc.binary
            hc.hits += 1
        else:
            rel = path
            if scope == "paths":  # git scopes are repo-relative already; --paths may be absolute
                try:
                    rel = Path(path).resolve().relative_to(Path(cwd).resolve()).as_posix()
                except ValueError:
                    pass
            raw, gz = data, data[:2] == GZ_MAGIC
            codec = None if gz else _bom(raw)
            if gz:  # 4.5 T16.1 G8: every class below sees the decompressed bytes; was: the raw blob
                data, gz_ok = _gunzip(raw)
            elif codec:  # 5.5 T14.c1: every class below sees the UTF-8 transcoding, as text; was: the raw blob
                data = raw.decode(codec, errors="replace").encode("utf-8")
            found, is_text = classify(rel, data, forbidden, True if codec else None)
            if gz or codec:
                if hashlib.sha1(raw).hexdigest() in forbidden:  # forbidden-hash on raw and decompressed/transcoded
                    found.add("forbidden-hash")
                if gz and not gz_ok:
                    found.add("oversize")
                if codec and len(raw) > OVERSIZE and not rel.replace("\\", "/").startswith(OVERSIZE_EXEMPT):
                    found.add("oversize")  # 5.5 T14.c1: transcoding can halve the size; the raw size counts too
            for c in VALUE_KEYS:
                if rules.get(c) is not None and address_hit(data, rules[c], rel):
                    found.add(c)
            if "user-path" in counts and is_text and rules["user-path"].search(data):
                found.add("user-path")
        if hc:
            hc.note(oid, path, found, is_text)
        for c in CLASSES:
            if c not in counts or (c in VALUE_KEYS and rules.get(c) is None):
                continue
            if c not in ("text-run", "user-path") or is_text:
                counts[c][1] += 1
            if c in found:
                counts[c][0] += 1
                hits.append((c, path, oid))
    out = []
    for c, path, oid in hits:
        commit = None
        if scope in ("history", "range"):
            rev = ["--all"] if scope == "history" else [arg]
            commit = _git(["log", *rev, "-1", "--format=%H", f"--find-object={oid}"], cwd, env).decode().strip()
        out.append((c, path, oid, commit or None))
    return counts, out


def build_rules(cfg, root, env):
    """Rules for the tree at root: value classes from cfg, its accept lists, user-path iff it holds the marker."""
    conf = Path(root) / "config"
    rules = {"address": address_rule(cfg.get("xbox_ip"), conf / ACCEPT.name, root, env)}
    for c in ("boot-folder", "original-install"):
        rules[c] = name_rule(cfg.get(VALUE_KEYS[c]), VALUE_KEYS[c], conf / ACCEPT_PRIVATE.name, root, env)
    if (conf / PUBLIC_MARKER).is_file():
        rules["user-path"] = USER_PATH
    return rules


def report(scope, counts, hits, unchecked=()):
    for c in CLASSES:
        if c in unchecked:
            msg = f"{scope} {c}: not checked ({VALUE_KEYS[c]} not set in local config)"
            print(msg)
            print(msg, file=sys.stderr)
            continue
        if c in counts:  # user-path is absent outside a marker tree
            print(f"{scope} {c}: {counts[c][0]} of {counts[c][1]}")
    for c, path, oid, commit in hits:
        print(f"HIT {scope} {c} {path}" + (f" blob {oid}" if oid else "") + (f" commit {commit}" if commit else ""))


# ---- forbidden set and coverage ----

def _refuse(msg):
    print(f"audit_public: {msg}", file=sys.stderr)
    sys.exit(2)


def forbidden_set(manifest_path):
    rows = vv_inputs.load_manifest(manifest_path)
    out = set()
    for r in rows:
        s = r.get("sha1") if isinstance(r, dict) else None
        if not isinstance(s, str) or not SHA1_RE.match(s):
            _refuse(f"manifest row without a valid sha1: {r!r:.80}")
        out.add(s)
    for f in sorted(Path(manifest_path).parent.glob("check.*.sha")):
        for ln in f.read_text(encoding="ascii", errors="replace").splitlines():
            if not CHECK_LINE.match(ln):
                _refuse(f"{f.name}: line does not match '<sha1>  <id>': {ln[:60]!r}")
            out.add(ln[:40])
    if not out:
        _refuse("forbidden set is empty")
    return rows, out


def coverage(rows, cfg):
    by_role = {}
    for r in rows:
        by_role.setdefault(r.get("role"), set()).add((r.get("name"), r.get("size")))
    stale = False
    for key, role in COVER:
        if cfg.get(key) is None:  # optional key unset (xbdm_dll, 1.7 T3)
            continue
        p = Path(cfg[key])
        if p.is_dir():
            files = sorted(f for f in p.rglob("*") if f.is_file())
            items = [(f.relative_to(p).as_posix(), f.stat().st_size) for f in files]
        else:
            items = [(p.name, p.stat().st_size)]
        covered = sum(1 for it in items if it in by_role.get(role, ()))
        print(f"coverage {role}: {covered} of {len(items)}")
        stale |= covered != len(items)
    if stale:
        _refuse("manifest stale: run tools/manifest.py build")


# ---- history cache (5.4 T5.c5) ----

CACHE_ENTRY = re.compile(r"^([0-9a-f]{40}) (.+)$")


def history_key(root, forbidden, cfg):
    """sha256 over everything a history verdict reads, for the tree at root: its fingerprint line for this tool, this
    source, both accept lists, the sorted forbidden set, a sha256 per value class's value (never the value, D46), the
    marker, and the rule constants. Paths are root-relative so a scratch tree keys on its own lists."""
    conf, h = Path(root) / "config", hashlib.sha256()

    def part(name, data):
        h.update(name.encode() + b"\0" + hashlib.sha256(data).hexdigest().encode() + b"\n")
    fp = conf / "verifier.sha256"
    lines = fp.read_bytes().splitlines() if fp.is_file() else [b"<missing>"]
    part("fp", b"\n".join(ln for ln in lines if ln.endswith(b"  tools/audit_public.py")) or b"<absent>")
    part("source", Path(__file__).read_bytes())
    for p in (conf / ACCEPT.name, conf / ACCEPT_PRIVATE.name):
        part(p.name, p.read_bytes() if p.is_file() else b"<missing>")
    part("forbidden", "\n".join(sorted(forbidden)).encode())
    for c, k in sorted(VALUE_KEYS.items()):
        part(c, b"<unset>" if cfg.get(k) is None else json.dumps(cfg.get(k), sort_keys=True).encode())
    part("marker", str((conf / PUBLIC_MARKER).is_file()).encode())
    part("rules", repr((CLASSES, OVERSIZE, GZ_CAP, OVERSIZE_EXEMPT, sorted(TOOL_SUFFIXES), SNIFF, RUN_N,
                        INDEX_RE.pattern, USER_PATH.pattern, "private")).encode())
    return h.hexdigest()


class HistoryCache:
    """Clean history verdicts by (oid, path) under one key. warm False: never reads the file (cold)."""

    def __init__(self, path, key, warm):
        self.path, self.key, self.state = Path(path), key, "none"
        self.clean, self.binary = set(), set()
        self.hits = self.n = 0
        self.ran, self.new_clean, self.new_binary = False, set(), set()
        self.heads, self.tips = [], None  # 5.5 T12.c3 (I73 F3): the file's recorded tips; this run's (begin)
        if warm:
            self._load()

    def _load(self):
        try:
            doc = json.loads(self.path.read_text(encoding="ascii"))
            key, clean, binary, heads = doc["key"], doc["clean"], doc["binary"], doc["heads"]  # 5.5 T12.c3: + heads
            pairs = {CACHE_ENTRY.match(e).groups() for e in clean}  # a non-str or non-matching entry raises
            if not isinstance(key, str) or not all(isinstance(o, str) and SHA1_RE.match(o) for o in [*binary, *heads]):
                raise ValueError("cache shape")
        except Exception:  # missing, unreadable or malformed (5.5 T12.c3: or no heads, a pre-F3 file): a full miss
            return
        if key != self.key:
            self.state = "changed"
            return
        self.state, self.clean, self.binary, self.heads = "ok", pairs, set(binary), list(heads)

    def begin(self, cwd, env):
        """5.5 T12.c3 (I73 F3): before the history scope reads: record this run's tip commits (finish writes them as
        `heads`); warm, drop every cached pair whose blob is reachable from a commit not under the recorded heads
        (`rev-list --objects --all --not <heads>`, heads gone from the store ignored), so a clean pair forged for a
        commit made after the write is read, never skipped; was: any cached pair skipped, whatever its commit."""
        self.tips = sorted(set(_git(["rev-list", "--no-walk", "--all"], cwd, env).decode().split()))
        if not self.clean:
            return
        have = _git(["cat-file", "--batch-check"], cwd, env, inp="\n".join(self.heads).encode() + b"\n").decode() \
            if self.heads else ""
        known = [ln.split()[0] for ln in have.splitlines() if ln.split()[1:2] == ["commit"]]
        newer = {o for o, _p in _objects_blobs(["--all", "--not", *known], cwd, env)}
        self.clean = {(o, p) for o, p in self.clean if o not in newer}

    def note(self, oid, path, found, is_text):
        self.ran, self.n = True, self.n + 1
        if not found:
            self.new_clean.add((oid, path))
            if not is_text:
                self.new_binary.add(oid)

    def finish(self):
        """Write the file (history ran) atomically; print the stats line on stderr."""
        if self.ran:
            self.path.parent.mkdir(parents=True, exist_ok=True)
            tmp = self.path.with_name(self.path.name + ".tmp")
            tmp.write_text(json.dumps({"key": self.key, "clean": sorted(f"{o} {p}" for o, p in self.new_clean),
                                       "binary": sorted(self.new_binary), "heads": self.tips or []},  # 5.5 T12.c3
                                      indent=0) + "\n", encoding="ascii")
            os.replace(tmp, self.path)
        print(f"audit: history cache {self.hits} of {self.n} (key {self.state})", file=sys.stderr)


def run(scopes, forbidden, cwd, env, rules=None, cache=None):
    rules = rules or {}
    unchecked = [c for c in VALUE_KEYS if rules.get(c) is None]
    total = 0
    for scope, arg in scopes:
        counts, hits = audit(scope, cwd, env, forbidden, arg, rules, cache)
        report(scope if scope != "range" else f"range {arg}", counts, hits, unchecked)
        total += len(hits)
    print(f"audit_public: {'OK' if total == 0 else 'FAIL'} ({total} hits)"
          + "".join(f"; {c} NOT CHECKED" for c in unchecked))
    if cache:
        cache.finish()
    return 0 if total == 0 else 1


def parse(argv):
    if not argv:
        return [("worktree", None), ("staged", None), ("history", None)]
    scopes = []
    i = 0
    while i < len(argv):
        a = argv[i]
        if a in ("--worktree", "--staged", "--history"):
            scopes.append((a[2:], None))
        elif a == "--range":
            if i + 1 >= len(argv) or ".." not in argv[i + 1]:
                _refuse("--range needs OLD..NEW")
            i += 1
            scopes.append(("range", argv[i]))
        elif a == "--paths":
            if i + 1 >= len(argv):  # 4.5 T16.2 G19; was: an empty scope, scanned nothing and printed OK
                _refuse("--paths needs at least one file")
            scopes.append(("paths", argv[i + 1:]))
            break
        else:
            _refuse(f"unknown argument: {a} (see --help in the module docstring)")
        i += 1
    return scopes


def public_ci(extra):
    """The public repo's audit on a host with no local config (T3 phase 3.6). Tracked tree + full history, no config
    and no AUDIT_PUBLIC_* override, so nothing outside the repo is read. Value classes print `not checked`. Hooks and
    the gate never pass the flag; without the marker it refuses, so it cannot stand in for the private audit."""
    if extra:
        _refuse(f"{PUBLIC_FLAG} takes no other argument")
    if not (REPO / "config" / PUBLIC_MARKER).is_file():
        _refuse(f"{PUBLIC_FLAG} refused: config/{PUBLIC_MARKER} missing, this is not the public tree")
    if _git(["rev-parse", "--is-shallow-repository"], REPO, None).strip() != b"false":
        _refuse(f"{PUBLIC_FLAG} refused: shallow clone, the history scope would be partial (fetch the full history)")
    _rows, forbidden = forbidden_set(vv_inputs.MANIFEST)  # missing, empty or sidecar mismatch: refusal
    print("coverage: not checked (no local config: manifest checked against its sidecar only)")
    return run([("worktree", None), ("history", None)], forbidden, REPO, None, {"user-path": USER_PATH})


def _route_pin():
    """Causes the speed route's assumptions no longer hold (module docstring); empty when they do."""
    causes = []
    names = {"_is_asm": {"bool", "ASM_RE", "ASM_BARE_RE", "match"}, "_is_hex": {"bool", "HEX_RE", "HEXESC_RE", "search"}}
    for name, allowed in names.items():
        fn = globals()[name]
        co = fn.__code__
        if co.co_varnames != ("line",) or co.co_argcount != 1 or co.co_nlocals != 1:  # *args, **kw, kw-only: in varnames
            causes.append(f"{name} must take only line, with one local")
        if co.co_freevars or co.co_cellvars or fn.__defaults__ or fn.__kwdefaults__:
            causes.append(f"{name} must have no freevars or defaults")
        if not set(co.co_names) <= allowed:
            causes.append(f"{name} reads a name outside {sorted(allowed)}")
    for name, build, sample in (("_address_rx", _address_rx, ["192.0.2.1"]), ("_name_rx", _name_rx, "SampleName")):
        literals = build(sample)[1]
        if not literals or not all(literals):
            causes.append(f"{name} gives no literal")
    return causes


def main(argv):
    causes = _route_pin()
    if causes:
        _refuse("route pin: " + "; ".join(causes))
    if argv[:1] in (["-h"], ["--help"]):
        print(__doc__)
        return 0
    if argv[:1] == ["--self-test"]:
        return self_test()
    if argv[:1] == [PUBLIC_FLAG]:
        return public_ci(argv[1:])
    cache_path, warm = None, False
    while argv[:1] in (["--cache"], ["--warm"]):  # 5.4 T5.c5: options before the scope args
        if argv[0] == "--warm":
            warm, argv = True, argv[1:]
        elif len(argv) < 2:
            _refuse("--cache needs FILE")
        else:
            cache_path, argv = argv[1], argv[2:]
    if warm and cache_path is None:
        _refuse("--warm needs --cache FILE")
    scopes = parse(argv)
    rows, forbidden = forbidden_set(os.environ.get("AUDIT_PUBLIC_MANIFEST") or vv_inputs.MANIFEST)
    unclassed = set(vv_inputs.KEYS) - {k for k, _ in COVER} - set(NOT_INPUTS)
    if unclassed:
        print(f"audit_public: config key(s) neither covered nor NOT_INPUTS: {sorted(unclassed)}", file=sys.stderr)
        return 2
    cfg = vv_inputs.load_config(os.environ.get("AUDIT_PUBLIC_CONFIG") or None)
    coverage(rows, cfg)
    rules = build_rules(cfg, REPO, None)
    cache = HistoryCache(cache_path, history_key(REPO, forbidden, cfg), warm) if cache_path else None
    return run(scopes, forbidden, REPO, None, rules, cache)


# ---- self-test ----

def _fixtures():
    """{name: (bytes, expected classes)}; text fixtures are generated so this source holds no run."""
    regs = ("eax", "ecx", "edx", "ebx", "esi", "edi")
    asm = "".join(f"{0x11000 + 4 * i:08X}  {i:02X} {i + 1:02X}  mov {regs[i % 6]}, dword ptr [esp + 0x{4 * i:x}]\n"
                  for i in range(8))
    hexd = "".join(f"{16 * r:08x}  " + " ".join(f"{(r * 16 + b) * 7 % 256:02x}" for b in range(16)) + "\n"
                   for r in range(4))
    arr = "".join("    " + ", ".join(f"0x{(r * 16 + b) * 5 % 256:02x}" for b in range(16)) + ",\n" for r in range(4))
    esc = "".join('"' + "".join(f"\\x{(r * 16 + b) * 3 % 256:02x}" for b in range(16)) + '"\n' for r in range(4))
    mz = bytearray(b"MZ" + b"\x90" * 0x7E)
    struct.pack_into("<I", mz, 0x3C, 0x80)
    mz += b"PE" + b"\0\0" + b"\x4c\x01" * 8
    coff = struct.pack("<HHIIIHH", 0x014C, 2, 0, 0, 0, 0, 0) + b"\0" * 64
    big = hashlib.sha256(b"selftest").digest() * (OVERSIZE // 32 + 64)
    return {
        "forbidden.dat": (b"synthetic forbidden payload for audit_public self-test\n", {"forbidden-hash"}),
        "xbeh.dat": (b"XBEH" + b"\0" * 60, {"magic", "toolchain"}),
        "mzpe.dat": (bytes(mz), {"magic", "toolchain"}),
        "coff.dat": (coff, {"magic", "toolchain"}),
        "msf.dat": (MSF7 + b"\0" * 40, {"magic", "toolchain"}),
        "arch.dat": (AR_MAGIC + b"/  0 0 0 0 0 `\n", {"toolchain"}),
        "named.pdb": (b"plain text with a toolchain suffix\n", {"toolchain"}),
        "big.dat": (big, {"oversize"}),
        ".claude-state/journal/big.md": (big + b"journal", set()),  # distinct blob for history
        ".claude-state/ledger/big.gz": (big + b"ledger", set()),  # distinct blob for history (C0047)
        ".claude-state/ledger-held/big.gz": (big + b"held", {"oversize"}),  # near prefix still flags
        "x/.claude-state/ledger/big.gz": (big + b"nested", {"oversize"}),  # prefix anchored at repo root
        ".claude-state/ledger/xbeh.gz": (b"XBEH" + b"ledger" + b"\0" * 60, {"magic", "toolchain"}),
        ".claude-state/journal/xbeh.md": (b"XBEH" + b"journal" + b"\0" * 60, {"magic", "toolchain"}),
        "asm.txt": (asm.encode(), {"text-run"}),
        "hexdump.txt": (hexd.encode(), {"text-run"}),
        "hexarray.c": (arr.encode(), {"text-run"}),
        "hexesc.c": (esc.encode(), {"text-run"}),
        "asm16le.txt": (b"\xff\xfe" + asm.encode("utf-16-le"), {"text-run"}),  # 5.5 T14.c1: BOM blobs transcoded
        "hex16be.txt": (b"\xfe\xff" + hexd.encode("utf-16-be"), {"text-run"}),
        "arr32le.c": (b"\xff\xfe\0\0" + arr.encode("utf-32-le"), {"text-run"}),
        "esc32be.c": (b"\0\0\xfe\xff" + esc.encode("utf-32-be"), {"text-run"}),
        "plain16le.txt": (b"\xff\xfe" + "plain text, no run\n".encode("utf-16-le"), set()),
        **{f"arch{i}.dat": (m + bytes([i]) * 40, {"magic"}) for i, m in enumerate(  # 5.5 T14.c1: archive magics
            (*ARCHIVE_MAGICS, b"BZh9"))},
        "tar.dat": (b"name.txt" + b"\0" * 249 + b"ustar\x0000" + b"\0" * 248, {"magic"}),
        "bzh0.txt": (b"BZh0 is not a bzip2 header\n", set()),
    }


def _clean_text():
    h1 = hashlib.sha1(b"a").hexdigest()
    h2 = hashlib.sha256(b"b").hexdigest()
    return (f"# Notes\nsha1 {h1}\nsha256 {h2}\n{h1}  retail_xbe\n"
            "The function at offset 0x1A2B4 loads the table.\n"
            "Addresses: 0x00011000, 0x00011040, 0x00011080, 0x000110C0, 0x00011100, 0x00011140\n"
            "We mov the value into a register and call the helper; push and pop are balanced.\n"
            "Sizes 10 12 14 16 are decimal.\n").encode()


def self_test():
    env = {k: v for k, v in os.environ.items() if not k.startswith("GIT_")}
    cfgs = ["-c", "user.name=selftest", "-c", "user.email=selftest@invalid", "-c", "commit.gpgsign=false",
            "-c", "core.hooksPath=", "-c", "core.autocrlf=false"]
    results = []

    def check(name, ok):
        results.append(ok)
        print(f"{'PASS' if ok else 'FAIL'} {name}")

    def mkrepo(name):
        d = SELFTEST_DIR / name
        d.mkdir(parents=True)
        _git([*cfgs, "init", "-q"], d, env)
        return d

    def commit(d, msg):
        _git([*cfgs, "commit", "-q", "--no-verify", "-m", msg], d, env)
        return _git(["rev-parse", "HEAD"], d, env).decode().strip()

    def found(scope, d, forbidden, arg=None):
        _counts, hits = audit(scope, d, env, forbidden, arg)
        return {(c, p) for c, p, _o, _m in hits}, hits

    def onexc(func, path, _exc):
        os.chmod(path, stat.S_IWRITE)
        func(path)

    if SELFTEST_DIR.exists():
        shutil.rmtree(SELFTEST_DIR, onexc=onexc)
    try:
        fx = _fixtures()
        forb = {hashlib.sha1(fx["forbidden.dat"][0]).hexdigest()}
        plant = mkrepo("plant")
        for n, (data, _e) in fx.items():
            (plant / n).parent.mkdir(parents=True, exist_ok=True)
            (plant / n).write_bytes(data)
        _git([*cfgs, "add", "--", *fx], plant, env)
        commit(plant, "plant")
        for scope in ("worktree", "staged", "paths", "history"):
            arg = [str(plant / n) for n in fx] if scope == "paths" else None
            got, _h = found(scope, plant, forb, arg)
            if scope == "paths":
                got = {(c, Path(p).relative_to(plant).as_posix()) for c, p in got}
            for n, (_d, exp) in fx.items():
                have = {c for c, p in got if p == n}
                check(f"{scope} {n}: {sorted(exp)}", have == exp)

        hist = mkrepo("hist")
        (hist / "readme.txt").write_bytes(_clean_text())
        _git([*cfgs, "add", "readme.txt"], hist, env)
        c0 = commit(hist, "c0")
        (hist / "forbidden.dat").write_bytes(fx["forbidden.dat"][0])
        _git([*cfgs, "add", "forbidden.dat"], hist, env)
        c1 = commit(hist, "c1 add")
        _git([*cfgs, "rm", "-q", "forbidden.dat"], hist, env)
        c2 = commit(hist, "c2 delete")
        want = ("forbidden-hash", "forbidden.dat")
        got, hits = found("history", hist, forb)
        check("history catches add-then-delete", want in got)
        check("history names a commit touching the blob", any(h[3] in (c1, c2) for h in hits))
        check("worktree misses add-then-delete", not found("worktree", hist, forb)[0])
        check("staged misses add-then-delete", not found("staged", hist, forb)[0])
        check("range c0..HEAD catches it", want in found("range", hist, forb, f"{c0}..HEAD")[0])
        check("range c1..HEAD misses it", not found("range", hist, forb, f"{c1}..HEAD")[0])

        clean = mkrepo("clean")
        (clean / "notes.md").write_bytes(_clean_text())
        (clean / "tool.py").write_bytes(Path(__file__).read_bytes())
        _git([*cfgs, "add", "notes.md", "tool.py"], clean, env)
        c = commit(clean, "clean")
        for scope, arg in (("worktree", None), ("staged", None), ("history", None), ("range", f"{c}..{c}"),
                           ("paths", [str(clean / "notes.md"), str(Path(__file__))])):
            check(f"clean control {scope}", not found(scope, clean, forb, arg)[0])

        # address class: value from the local config, else RFC 5737 documentation value; never printed
        ip = None
        try:
            with contextlib.redirect_stderr(io.StringIO()):
                ip = vv_inputs.load_config(os.environ.get("AUDIT_PUBLIC_CONFIG") or None).get("xbox_ip")
        except SystemExit:
            pass
        if ip is None:
            ip = "192.0.2.1"
            print("address controls: synthetic value")
        ips = [ip] if isinstance(ip, str) else list(ip)
        a = ips[0].encode()
        ad = mkrepo("addr")

        def put(name, data, stage=True):
            (ad / name).write_bytes(data)
            if stage:
                _git([*cfgs, "add", name], ad, env)

        def ahits(scope, rule, arg=None):
            return {p for c, p, _o, _m in audit(scope, ad, env, set(), arg, {"address": rule})[1] if c == "address"}

        put("old.md", b"intro\nbox at " + a + b" here\nend\n")
        put("other.md", b"see " + a + b"\n")
        commit(ad, "listed")
        acc = SELFTEST_DIR / "accept.txt"
        acc.write_bytes(b"# self-test accept list\n" + "".join(
            _blob_id((ad / n).read_bytes()) + "\n" for n in ("old.md", "other.md")).encode())
        rule = address_rule(ips, acc, ad, env)
        check("address 3 listed blobs pass", not ahits("history", rule))
        put("near.md", b"near " + a + b"7\n")
        put("old.md", b"intro v2\nbox at " + a + b" here\nnew text\n")
        commit(ad, "near and v2")
        h = ahits("history", rule)
        check("address 4 near-miss passes", "near.md" not in h)
        check("address 5 old address lines only passes", "old.md" not in h)
        put("old.md", b"intro v2\nbox at " + a + b" here\nnew text\nalso " + a + b" there\n")
        check("address 6 new address line fails", "old.md" in ahits("staged", rule))
        put("syn.md", b"synthetic " + a + b"\n")
        check("address 1 staged synthetic fails", "syn.md" in ahits("staged", rule))
        commit(ad, "v3 and syn")
        _git([*cfgs, "rm", "-q", "syn.md"], ad, env)
        commit(ad, "rm syn")
        check("address 2 unlisted history blob fails", "syn.md" in ahits("history", rule))
        put("fresh.md", b"fresh " + a + b"\n", stage=False)
        check("address 7 new file fails", ahits("paths", rule, [str(ad / "fresh.md")]) == {str(ad / "fresh.md")})
        ix = ".claude-state/journal/INDEX-2000-01.md"
        (ad / ix).parent.mkdir(parents=True, exist_ok=True)
        put(ix, b"t0 | dev | msg | box at " + a + b" | .claude-state/journal/2000-01-01/x.md\n")
        commit(ad, "index")
        acc.write_bytes(acc.read_bytes() + (_blob_id((ad / ix).read_bytes()) + "\n").encode())
        rule = address_rule(ips, acc, ad, env)
        put(ix, b"t0 | dev | msg | box at " + a + b" | .claude-state/journal/2000-01-01.md#x\nt1 | mod | start | x | y\n")
        check("address 10 index line with changed link passes", ix not in ahits("staged", rule))
        put("old.md", b"t0 | dev | msg | box at " + a + b" | .claude-state/journal/2000-01-01.md#x\n")
        check("address 10b changed link outside index files fails", "old.md" in ahits("staged", rule))
        put(ix, b"t0 | dev | msg | box now at " + a + b" | .claude-state/journal/2000-01-01.md#x\n")
        check("address 11 index line with changed text fails", ix in ahits("staged", rule))
        so, se = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(so), contextlib.redirect_stderr(se):
            rc = run([("worktree", None), ("history", None)], set(), ad, env,
                     {"address": address_rule(None, acc, ad, env)})
        nc = "address: not checked (xbox_ip not set in local config)"
        check("address 8 null xbox_ip not checked, rc 0",
              rc == 0 and so.getvalue().count(nc) == 2 and se.getvalue().count(nc) == 2
              and "address: 0 of" not in so.getvalue()
              and "; address NOT CHECKED" in so.getvalue().rstrip().splitlines()[-1])

        def refuses(text):
            acc.write_bytes(text)
            try:
                with contextlib.redirect_stderr(io.StringIO()):
                    address_rule(ips, acc, ad, env)
            except SystemExit as e:
                return e.code == 2
            return False

        check("address 9 malformed accept line refuses", refuses(b"# header\nnot a blob id\n"))
        check("address 9b listed id missing from store refuses", refuses(b"# header\n" + b"0" * 40 + b"\n"))

        # boot-folder, original-install: synthetic values only (path forms: prefix, trailing separator, '/')
        for c, value, n in (("boot-folder", "F:\\Games\\SelfTestBoot\\", b"SelfTestBoot"),
                            ("original-install", "Games/SelfTestOrig", b"SelfTestOrig")):
            key, pd, pacc = VALUE_KEYS[c], mkrepo(f"priv-{c}"), SELFTEST_DIR / f"accept-{c}.txt"
            tag = c.encode()  # distinct bytes per class's history fixtures (C0047)

            def pput(name, data, stage=True, pd=pd):
                (pd / name).write_bytes(data)
                if stage:
                    _git([*cfgs, "add", name], pd, env)

            def phits(scope, rule, arg=None, pd=pd, c=c):
                return {p for k, p, _o, _m in audit(scope, pd, env, set(), arg, {c: rule})[1] if k == c}

            def prule(pd=pd, pacc=pacc, key=key, value=value):
                return name_rule(value, key, pacc, pd, env)

            pput("old.md", tag + b" intro\nfolder " + n + b" here\nend\n")
            pput("other.md", tag + b" see " + n + b"\n")
            commit(pd, "listed")
            pacc.write_bytes(b"# self-test accept list\n" + "".join(
                _blob_id((pd / f).read_bytes()) + "\n" for f in ("old.md", "other.md")).encode())
            rule = prule()
            check(f"{c} 3 listed blobs pass", not phits("history", rule))
            pput("near.md", tag + b" near " + n + b"x\n")
            pput("old.md", tag + b" intro v2\nfolder " + n + b" here\nnew text\n")
            commit(pd, "near and v2")
            h = phits("history", rule)
            check(f"{c} 4 near-miss (extra letter) passes", "near.md" not in h)
            check(f"{c} 5 old lines only passes", "old.md" not in h)
            pput("old.md", tag + b" intro v2\nfolder " + n + b" here\nnew text\nalso " + n + b" there\n")
            check(f"{c} 6 new line fails", "old.md" in phits("staged", rule))
            pput("syn.md", tag + b" synthetic " + n + b"\n")
            pput("case.md", tag + b" case " + n.swapcase() + b"\n")
            got = phits("staged", rule)
            check(f"{c} 1 staged plant fails", "syn.md" in got)
            check(f"{c} 1b other-case plant fails", "case.md" in got)
            commit(pd, "v3 and syn")
            _git([*cfgs, "rm", "-q", "syn.md"], pd, env)
            commit(pd, "rm syn")
            check(f"{c} 2 unlisted history blob fails", "syn.md" in phits("history", rule))
            pput("fresh.md", tag + b" fresh " + n + b"\n", stage=False)
            check(f"{c} 7 new file fails", phits("paths", rule, [str(pd / "fresh.md")]) == {str(pd / "fresh.md")})
            # journal files (3.5 T5, C2.6-2): a plant in a dated journal file and in an INDEX file fails in worktree,
            # staged and history (deleted); the key names alone pass, in a journal file and a tracked file
            jd, jx = ".claude-state/journal/2000-01-01/plant.md", ".claude-state/journal/INDEX-2000-01.md"
            jk = ".claude-state/journal/2000-01-02/keys.md"
            for f in (jd, jk):
                (pd / f).parent.mkdir(parents=True, exist_ok=True)
            pput(jd, tag + b" journal\n- set " + key.encode() + b" to " + value.encode() + b"\n")
            pput(jx, tag + b" t0 | dev | msg | folder " + n + b" | .claude-state/journal/2000-01-01/plant.md\n")
            keys = (tag + b' keys\n"boot_folder": null, "original_install": null\n'
                    b"set boot_folder and original_install in config/local_paths.json\n")
            pput(jk, keys)
            pput("keys.md", keys + b"tracked\n")

            def jcheck(scope, got, c=c, jd=jd, jx=jx, jk=jk):
                check(f"{c} J1 {scope} journal file plant fails", jd in got)
                check(f"{c} J2 {scope} journal index plant fails", jx in got)
                check(f"{c} J3 {scope} key names alone pass", jk not in got and "keys.md" not in got)

            for scope in ("worktree", "staged"):
                jcheck(scope, phits(scope, rule))
            commit(pd, "journal")
            _git([*cfgs, "rm", "-q", jd, jx], pd, env)
            commit(pd, "rm journal plants")
            jcheck("history", phits("history", rule))
            so, se = io.StringIO(), io.StringIO()
            with contextlib.redirect_stdout(so), contextlib.redirect_stderr(se):
                rc = run([("worktree", None), ("history", None)], set(), pd, env,
                         {c: name_rule(None, key, pacc, pd, env)})
            nc = f"{c}: not checked ({key} not set in local config)"
            check(f"{c} 8 absent key not checked, rc 0",
                  rc == 0 and so.getvalue().count(nc) == 2 and se.getvalue().count(nc) == 2
                  and f"{c}: 0 of" not in so.getvalue()
                  and f"; {c} NOT CHECKED" in so.getvalue().rstrip().splitlines()[-1])

            def prefuses(text=None, val=value, pacc=pacc, pd=pd, key=key):
                if text is not None:
                    pacc.write_bytes(text)
                try:
                    with contextlib.redirect_stderr(io.StringIO()):
                        name_rule(val, key, pacc, pd, env)
                except SystemExit as e:
                    return e.code == 2
                return False

            check(f"{c} 9 malformed accept line refuses", prefuses(b"# header\nnot a blob id\n"))
            check(f"{c} 9b listed id missing from store refuses", prefuses(b"# header\n" + b"0" * 40 + b"\n"))
            pacc.write_bytes(b"# header\n")
            check(f"{c} 9c empty or non-string value refuses", prefuses(val="") and prefuses(val=["x"]))

        # user-path (3.6 T3): spellings built at run time, so this source holds none and passes its own class (C0003);
        # synthetic values only. Trees: `um` holds the marker, `un` the same files without it; same config for both.
        u, bs, fs = b"Users", b"\\", b"/"
        spell = {"backslash": b"C:" + bs + u + bs + b"x", "slash": b"C:" + fs + u + fs + b"x",
                 "double": b"C:" + bs * 2 + u + bs * 2 + b"x", "msys": fs + b"c" + fs + u + fs + b"x"}
        plants = {f"plant-{n}-{case}.txt": (f"{n} {case} at ".encode() + fn(v) + b"\n")  # distinct bytes (C0047)
                  for n, v in spell.items()
                  for case, fn in (("asis", bytes), ("upper", bytes.upper), ("lower", bytes.lower))}
        ucfg = {"xbox_ip": "192.0.2.1", "boot_folder": "F:\\Games\\SelfTestBoot", "original_install": "Games/SelfTestOrig"}
        comments = b"# accept list: no ids\n# none\n"

        def utree(name, marker):
            d = mkrepo(name)
            (d / "config").mkdir()
            for f in (ACCEPT.name, ACCEPT_PRIVATE.name) + ((PUBLIC_MARKER,) if marker else ()):
                (d / "config" / f).write_bytes(comments)
            _git([*cfgs, "add", "config"], d, env)
            return d, commit(d, "tree")

        def uput(d, files):
            for f, data in files.items():
                (d / f).write_bytes(data)
            _git([*cfgs, "add", "--", *files], d, env)

        def ugot(scope, d, rules, cls="user-path", arg=None):
            return {p for k, p, _o, _m in audit(scope, d, env, set(), arg, rules)[1] if k == cls}

        def urun(d, rules, scopes=(("worktree", None), ("staged", None), ("history", None))):
            so = io.StringIO()
            with contextlib.redirect_stdout(so):
                rc = run(list(scopes), set(), d, env, rules)
            return rc, so.getvalue()

        check("user-path this source holds no spelling", not USER_PATH.search(Path(__file__).read_bytes()))
        um, um0 = utree("user-marker", True)
        un, _un0 = utree("user-nomarker", False)
        urm, urn = build_rules(ucfg, um, env), build_rules(ucfg, un, env)
        check("user-path marker tree: class on, no marker: class off", "user-path" in urm and "user-path" not in urn)
        for d, rules, verb in ((um, urm, "caught"), (un, urn, "passes")):
            uput(d, plants)
            want = set(plants) if verb == "caught" else set()
            rc, out = urun(d, rules)
            for scope in ("worktree", "staged"):
                got = ugot(scope, d, rules)
                for n in spell:
                    check(f"user-path {verb} {scope} {n} any case",
                          {p for p in got if p.startswith(f"plant-{n}-")} == {p for p in want if p.startswith(f"plant-{n}-")})
            if verb == "caught":
                check("user-path report names class and path, no value",
                      rc == 1 and "worktree user-path: 12 of " in out and "HIT worktree user-path plant-msys-asis.txt" in out
                      and u.decode() not in out and u.decode().lower() not in out)
            else:
                check("user-path no marker: no class line, not NOT CHECKED, rc 0",
                      rc == 0 and "user-path" not in out and "NOT CHECKED" not in out)
            commit(d, "plants")
            _git([*cfgs, "rm", "-q", "--", *plants], d, env)
            commit(d, "rm plants")
            got = ugot("history", d, rules)
            for n in spell:
                check(f"user-path {verb} history (deleted) {n} any case",
                      {p for p in got if p.startswith(f"plant-{n}-")} == {p for p in want if p.startswith(f"plant-{n}-")})
            check(f"user-path {verb}: gone from worktree", not ugot("worktree", d, rules))
        check("user-path range catches the deleted plant", ugot("range", um, urm, arg=f"{um0}..HEAD") == set(plants))
        near = {"near-placeholder.txt": b"C:" + bs + u + bs + b"<you>" + bs + b"bin\n",
                "near-bare.txt": b'"C:' + bs + u + bs + b'"\n',
                "near-other.txt": b"C:" + bs + b"Program Files" + bs + b"x and d:" + bs + b"beep" + bs + b"x\n",
                "near-relative.txt": u + fs + b"x and /usr/x\n",
                "near-word.txt": b"abc:" + bs + u + bs + b"x\n",
                "near-binary.dat": b"\0" + b"C:" + bs + u + bs + b"x\n"}
        uput(um, near)
        check("user-path near-misses pass (placeholder, bare, other folder, relative, word, binary)",
              not ugot("staged", um, urm))
        uput(um, {"other-drive.txt": b"D:" + bs + u + bs + b"x\n"})
        check("user-path other drive letter caught", ugot("staged", um, urm) == {"other-drive.txt"})
        # comment-only accept lists, as the public tree ships them: they load, hold nothing, and a plant is caught
        check("marker tree comment-only lists load empty",
              all(urm[c][1] == frozenset() and urm[c][2] == frozenset() for c in VALUE_KEYS))
        cplants = {"address": ("c-addr.txt", b"box " + ucfg["xbox_ip"].encode() + b"\n"),
                   "boot-folder": ("c-boot.txt", b"in SelfTestBoot here\n"),
                   "original-install": ("c-orig.txt", b"in SelfTestOrig here\n"),
                   "user-path": ("c-user.txt", b"at " + spell["slash"] + b"\n")}
        uput(um, dict(cplants.values()))
        for cls, (f, _b) in cplants.items():
            check(f"marker tree comment-only lists catch {cls} plant", f in ugot("staged", um, urm, cls))

        # --public-ci (3.6 T3): the real CLI from a copy of the tool in a marker tree with no local config (C0003)
        cd, _c0 = utree("public-ci", True)
        (cd / "tools").mkdir()
        for f in (__file__, vv_inputs.__file__):
            shutil.copy(f, cd / "tools" / Path(f).name)
        cman = cd / "config" / "inputs.manifest.jsonl"
        good = (b'{"name":"x.bin","role":"retail_xbe","sha1":"' + hashlib.sha1(fx["forbidden.dat"][0]).hexdigest().encode()
                + b'","sha256":"0","size":1}\n')
        cman.write_bytes(good)
        cman.with_suffix(".sha1").write_text(hashlib.sha1(good).hexdigest() + "\n", encoding="ascii")
        chk = b"synthetic payload named only by a check file\n"
        (cd / "config" / "check.sel.sha").write_text(hashlib.sha1(chk).hexdigest() + "  sel\n", encoding="ascii")
        _git([*cfgs, "add", "tools", "config"], cd, env)
        commit(cd, "public tree")
        cenv = {k: v for k, v in env.items() if not k.startswith("AUDIT_PUBLIC_")}

        def pub(*args, d=cd):
            r = subprocess.run([sys.executable, str(d / "tools" / "audit_public.py"), *args], cwd=d, env=cenv,
                               capture_output=True)
            return r.returncode, r.stdout.decode(errors="replace"), r.stderr.decode(errors="replace")

        def unchecked_ok(out):
            return all(f"{s} {c}: not checked" in out and f"; {c} NOT CHECKED" in out
                       for s in ("worktree", "history") for c in VALUE_KEYS)

        rc, out, _e = pub(PUBLIC_FLAG)
        check("public-ci clean tree rc 0, value classes not checked, others counted",
              rc == 0 and unchecked_ok(out) and all(f"history {c}: 0 of " in out for c in CLASSES if c not in VALUE_KEYS))
        cls_files = {"forbidden-hash": "forbidden.dat", "magic": "xbeh.dat", "oversize": "big.dat",
                     "toolchain": "named.pdb", "text-run": "asm.txt", "user-path": "user.txt"}
        cfiles = {f: fx[f][0] for f in cls_files.values() if f in fx}
        cfiles.update({"user.txt": b"ci at " + spell["msys"] + b"\n", "checkonly.txt": chk})
        uput(cd, cfiles)
        commit(cd, "plants")
        rc, out, _e = pub(PUBLIC_FLAG)
        for cls, f in cls_files.items():
            check(f"public-ci {cls} planted: caught in tracked tree and history",
                  f"HIT worktree {cls} {f}" in out and f"HIT history {cls} {f}" in out)
        check("public-ci check.*.sha plant caught", "HIT history forbidden-hash checkonly.txt" in out)
        check("public-ci plants rc 1, value lines do not change it", rc == 1 and unchecked_ok(out))
        _git([*cfgs, "rm", "-q", "--", *cfiles], cd, env)
        commit(cd, "rm plants")
        rc, out, _e = pub(PUBLIC_FLAG)
        check("public-ci deleted plants: tree clean, history catches, rc 1",
              rc == 1 and "HIT worktree" not in out and all(f"HIT history {c} " in out for c in cls_files))
        rc, _o, e = pub(PUBLIC_FLAG, "--worktree")
        check("public-ci takes no other argument", rc == 2 and "no other argument" in e)
        shal = SELFTEST_DIR / "public-ci-shallow"
        _git(["clone", "-q", "--depth", "1", f"file:///{cd.as_posix()}", str(shal)], SELFTEST_DIR, env)
        rc, _o, e = pub(PUBLIC_FLAG, d=shal)
        check("public-ci shallow clone refuses", rc == 2 and "shallow" in e)
        rc, _o, e = pub()
        check("default mode, no config, marker tree still refuses", rc == 2 and "config missing" in e)
        (cd / "config" / PUBLIC_MARKER).unlink()
        rc, _o, e = pub(PUBLIC_FLAG)
        check("public-ci without the marker refuses", rc == 2 and PUBLIC_MARKER in e)
        (cd / "config" / PUBLIC_MARKER).write_bytes(comments)
        cman.write_bytes(good + good)
        rc, _o, e = pub(PUBLIC_FLAG)
        check("public-ci stale manifest (off its sidecar) refuses", rc == 2 and "does not match" in e)
        cman.write_bytes(b"")
        rc, _o, e = pub(PUBLIC_FLAG)
        check("public-ci empty manifest refuses", rc == 2 and "manifest empty" in e)
        cman.unlink()
        rc, _o, e = pub(PUBLIC_FLAG)
        check("public-ci missing manifest refuses", rc == 2 and "manifest missing" in e)

        # refusals: CLI subprocess with env overrides
        ref = SELFTEST_DIR / "refuse"
        ref.mkdir()
        man = ref / "inputs.manifest.jsonl"
        row = b'{"name":"x.bin","role":"retail_xbe","sha1":"' + forb.pop().encode() + b'","sha256":"0","size":1}\n'
        man.write_bytes(row)
        man.with_suffix(".sha1").write_text(hashlib.sha1(row).hexdigest() + "\n", encoding="ascii")
        (ref / "empty.json").write_bytes(b"")
        (ref / "empty.jsonl").write_bytes(b"")
        (ref / "empty.sha1").write_text(hashlib.sha1(b"").hexdigest(), encoding="ascii")

        def cli(manifest, config, why):
            e = dict(env, AUDIT_PUBLIC_MANIFEST=str(manifest), AUDIT_PUBLIC_CONFIG=str(config))
            r = subprocess.run([sys.executable, str(Path(__file__)), "--worktree"], cwd=REPO, env=e,
                               capture_output=True)
            return r.returncode == 2 and why in r.stderr.decode(errors="replace")

        check("refuse missing manifest", cli(ref / "none.jsonl", ref / "none.json", "manifest missing"))
        check("refuse empty manifest", cli(ref / "empty.jsonl", ref / "none.json", "manifest empty"))
        check("refuse missing config", cli(man, ref / "none.json", "config missing"))
        check("refuse empty config", cli(man, ref / "empty.json", "config empty"))
        (ref / "check.bad.sha").write_text("not a hash line\n", encoding="ascii")
        check("refuse bad check.*.sha line", cli(man, ref / "empty.json", "check.bad.sha"))

        # speed route (T3.1 phase 4.1)
        def refused(fn, *args):
            try:
                with contextlib.redirect_stderr(io.StringIO()):
                    fn(*args)
            except SystemExit as e:
                return e.code == 2
            return False

        check("value_rule refuses no literal", refused(value_rule, _name_rx("SampleName")[0], (), True, ref / "none", ref, env))
        look_a, look_b = rb"(?<!x)", rb"(?!x)"
        check("_literal_rx refuses no literal", refused(_literal_rx, [], look_a, look_b))
        check("_literal_rx refuses an empty literal", refused(_literal_rx, [b"x", b""], look_a, look_b))
        for bad in (b"x", rb"(?:x)", rb"(x)", rb"(?!x)x", rb"x(?=y)", rb"(?!(x))"):
            check(f"_literal_rx refuses consuming before {bad!r}", refused(_literal_rx, [b"y"], bad, look_b))
            check(f"_literal_rx refuses consuming after {bad!r}", refused(_literal_rx, [b"y"], look_a, bad))
        check("_literal_rx takes lookarounds, and nothing", not refused(_literal_rx, [b"y"], look_a + look_b, b""))
        check("_route_pin empty on this tool", _route_pin() == [])
        src = Path(__file__).read_text(encoding="utf-8")
        hex_line = "return bool(HEX_RE.search(line) or HEXESC_RE.search(line))"
        name_line = "return " + "_literal_rx([name.encode()], rb\"(?<![A-Za-z0-9_])\", rb\"(?![A-Za-z0-9_])\", re.I)"
        for label, old, new, why in (
                ("_is_hex reading a module global", hex_line, hex_line[:-1] + " or OVERSIZE < 0)", "_is_hex reads a name"),
                ("_name_rx with its literals emptied", name_line, name_line + "[0], (), True", "_name_rx gives no literal")):
            md = SELFTEST_DIR / "mutants" / re.sub(r"\W+", "-", label)
            (md / "tools").mkdir(parents=True)
            shutil.copy(vv_inputs.__file__, md / "tools" / Path(vv_inputs.__file__).name)
            (md / "tools" / Path(__file__).name).write_text(src.replace(old, new, 1), encoding="utf-8", newline="\n")
            rc, _o, err = pub("--help", d=md)
            check(f"mutant: {label} exits 2 naming the pin", old in src and rc == 2 and "route pin" in err and why in err)
        check("control: the unmutated copy passes the pin", pub("--help")[0] == 0)

        # memo: two blobs sharing lines in another order; cold and warm memo, and the plain line tests
        lines = fx["asm.txt"][0].decode().splitlines()[:4] + fx["hexdump.txt"][0].decode().splitlines()[:3] + ["prose"]
        blob_a = "\n".join(lines).encode()
        blob_b = "\n".join(["prose", *lines[4:7], "prose", *lines[:2], "prose", *lines[2:4]]).encode()

        def plain(data):
            best = {"disasm": (0, 0), "hexdump": (0, 0)}
            cur = dict.fromkeys(best, 0)
            for i, line in enumerate(data.decode("latin-1").splitlines(), 1):
                for s, fn in (("disasm", _is_asm), ("hexdump", _is_hex)):
                    cur[s] = cur[s] + 1 if fn(line) else 0
                    if cur[s] > best[s][0]:
                        best[s] = (cur[s], i - cur[s] + 1)
            return best

        cold = {}
        for b in (blob_a, blob_b):
            _LINE_MEMO.clear()
            cold[b] = max_runs(b)
        _LINE_MEMO.clear()
        warm = {b: max_runs(b) for b in (blob_a, blob_b, blob_a)}
        check("memo: warm tuples equal cold and plain, on two blobs that differ",
              warm == cold and all(cold[b] == plain(b) for b in cold) and cold[blob_a] != cold[blob_b]
              and cold[blob_a]["disasm"] == (4, 1) and cold[blob_a]["hexdump"] == (3, 5))

        # the fold the name prefilter rests on: re.I over bytes against bytes.lower(), every byte pair
        fold_diff = 0
        for x in range(256):
            bx = bytes([x])
            rx1 = re.compile(re.escape(bx), re.I)
            for y in range(256):
                by = bytes([y])
                fold_diff += (rx1.fullmatch(by) is not None) != (bx.lower() == by.lower())
        check("re.I and bytes.lower() agree on all 65,536 byte pairs", fold_diff == 0)
    finally:
        if SELFTEST_DIR.exists():
            shutil.rmtree(SELFTEST_DIR, onexc=onexc)
    ok = all(results)
    print(f"audit_public self-test: {'PASS' if ok else 'FAIL'} ({sum(results)} of {len(results)})")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
