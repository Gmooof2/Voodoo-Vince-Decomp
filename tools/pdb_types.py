"""pdb_types: read VC 7.1 PDB types and locals directly (TPI 19990903, which llvm-pdbutil refuses).

Promoted in 2.2 T12 from the scratch readers rebuilt in six tasks (.run/2.2/T9/tpi.py, T6/tpi_enum.py, T6/locals.py).
Techniques: C0023 (TPI reader), C0044 (class fieldlist, vbaseoff, method lists), C0066, C0068 (S_BPREL32 locals).

  class <Name> [--methods]  fieldlist of the full (non-forward-ref) LF_CLASS/LF_STRUCTURE named Name:
                            members, methods + signatures (INTRO/PUREINTRO vtable offset), bases, vfptr, nested,
                            static; --methods adds every LF_MFUNCTION whose class is that record
  enum <Name>               enumerators of every non-forward-ref LF_ENUM whose name contains Name
  locals <proc>             S_BPREL32/S_REGREL32/S_REGISTER locals of each S_*PROC32 named exactly proc in each module
                            stream, to its S_END (S_BLOCK32 nesting followed); importable as locals(streams, proc)
                            -> records (3.2 T6.c2, read by provenance_lint.py; scope_locals shared with its obj reader)
  names                     every TPI class/struct/union/enum name (`type N`), then each full record's member,
                            method, nested, static and enumerator names (`member N::M`); importable as names()
                            after load_tpi (3.2 T6, read by provenance_lint.py)

Array types (LF_ARRAY) print as <element>[<count>], count = byte size / element size; an element size the
reader cannot determine is a refusal (exit 2), never a guess.
Default PDB: vv_inputs prototype_dir/default.pdb. Exit 0 found, 1 not found, 2 refusal (unreadable PDB or config,
unknown array element size).
Output is PDB-derived (D46): never commit it.
"""
import argparse
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import vv_inputs  # noqa: E402
from pdb_info import NotPdb, read_streams  # noqa: E402

MP = {0: "vanilla", 1: "virtual", 2: "static", 3: "friend", 4: "INTRO", 5: "purevirt", 6: "PUREINTRO"}
recs = {}


def load_tpi(streams):
    tpi = streams[2]
    hdr_size, ti_min, ti_max = struct.unpack_from("<III", tpi, 4)
    pos, ti = hdr_size, ti_min
    while ti < ti_max:
        ln, leaf = struct.unpack_from("<HH", tpi, pos)
        recs[ti] = (leaf, tpi[pos + 4:pos + 2 + ln])
        pos += 2 + ln
        ti += 1


def num(b, o):
    v = struct.unpack_from("<H", b, o)[0]
    if v < 0x8000:
        return v, o + 2
    sz = {0x8000: 1, 0x8001: 2, 0x8002: 2, 0x8003: 4, 0x8004: 4}[v]
    return int.from_bytes(b[o + 2:o + 2 + sz], "little"), o + 2 + sz


def cstr(b, o):
    e = b.index(b"\0", o)
    return b[o:e].decode("latin1"), e + 1


def tname(t):
    if t < 0x1000:
        return f"prim{t:#x}"
    leaf, b = recs[t]
    if leaf == 0x1507:
        return "enum " + cstr(b, 12)[0]
    if leaf in (0x1504, 0x1505, 0x1506):
        _, o = num(b, 16) if leaf != 0x1506 else num(b, 8)
        return cstr(b, o)[0]
    if leaf == 0x1002:
        mode = (struct.unpack_from("<I", b, 4)[0] >> 5) & 7
        return tname(struct.unpack_from("<I", b, 0)[0]) + ("&" if mode == 1 else "*")
    if leaf == 0x1001:
        m = struct.unpack_from("<H", b, 4)[0]
        return ("const " if m & 1 else "") + ("volatile " if m & 2 else "") + tname(struct.unpack_from("<I", b, 0)[0])
    if leaf == 0x1503:  # LF_ARRAY: elemtype u32, idxtype u32, size numeric, name (C0092)
        elem = struct.unpack_from("<I", b, 0)[0]
        size, _ = num(b, 8)
        cand = tsize(elem)
        counts = {size // es for es in cand if es > 0 and size % es == 0}
        if len(counts) != 1:
            raise TypeSizeError(f"LF_ARRAY {t:#x}: byte size {size:#x}, element {tname(elem)} "
                                f"sizes {sorted(hex(x) for x in cand)} give no single count")
        return f"{tname(elem)}[{counts.pop()}]"
    return f"leaf{leaf:#x}"


class TypeSizeError(Exception):
    """An array element whose byte size this reader cannot determine (D10: refuse, never guess)."""


PRIM_SIZE = {0x70: 1, 0x71: 2, 0x72: 2, 0x73: 2, 0x74: 4, 0x75: 4, 0x76: 8, 0x77: 8, 0x68: 1, 0x69: 1,
             0x40: 4, 0x41: 8, 0x42: 10}


def tsize(t):
    """Candidate byte sizes of type t (a set: a forward ref may name full records of several sizes in
    different modules, e.g. SpotlightGlowVertex 0x1c and 0x14); TypeSizeError when unknown."""
    if t < 0x1000:
        mode, kind, sub = (t >> 8) & 7, (t >> 4) & 0xF, t & 0xF
        if mode == 4:  # near32 pointer to a primitive
            return {4}
        if mode == 0 and kind in (1, 2, 3) and sub < 4:  # signed, unsigned, boolean
            return {1 << sub}
        if mode == 0 and (t & 0xFF) in PRIM_SIZE:
            return {PRIM_SIZE[t & 0xFF]}
        raise TypeSizeError(f"primitive {t:#x}: size unknown")
    leaf, b = recs[t]
    if leaf == 0x1002:  # LF_POINTER: 32-bit target; pointer-to-member modes refused
        if (struct.unpack_from("<I", b, 4)[0] >> 5) & 7 in (0, 1):
            return {4}
        raise TypeSizeError(f"LF_POINTER {t:#x}: pointer-to-member size unknown")
    if leaf == 0x1001:
        return tsize(struct.unpack_from("<I", b, 0)[0])
    if leaf == 0x1507:  # LF_ENUM: size of the underlying type
        return tsize(struct.unpack_from("<I", b, 4)[0])
    if leaf == 0x1503:
        return {num(b, 8)[0]}
    if leaf in (0x1504, 0x1505, 0x1506):
        prop = struct.unpack_from("<H", b, 2)[0]
        size, o = num(b, 16) if leaf != 0x1506 else num(b, 8)
        if not prop & 0x80:
            return {size}
        name = cstr(b, o)[0]  # forward ref: every full record of the same name and leaf (C0064)
        out = set()
        for lf2, b2 in recs.values():
            if lf2 == leaf and not struct.unpack_from("<H", b2, 2)[0] & 0x80:
                s2, o2 = num(b2, 16) if leaf != 0x1506 else num(b2, 8)
                if cstr(b2, o2)[0] == name:
                    out.add(s2)
        if out:
            return out
        raise TypeSizeError(f"{name} ({t:#x}): forward ref with no full record")
    raise TypeSizeError(f"leaf{leaf:#x} ({t:#x}): size unknown")


def sig(t):
    if t not in recs:
        return f"{t:#x}"
    leaf, b = recs[t]
    if leaf != 0x1009:
        return f"leaf{leaf:#x}"
    rv, cls, this = struct.unpack_from("<III", b, 0)
    cc, fa, np, al, adj = struct.unpack_from("<BBHIi", b, 12)
    lb = recs[al][1]
    n = struct.unpack_from("<I", lb, 0)[0]
    args = [tname(struct.unpack_from("<I", lb, 4 + 4 * i)[0]) for i in range(n)]
    return f"{tname(rv)}({', '.join(args)}) this={tname(this) if this else 'static'}"


def dump_fl(fl):
    leaf, b = recs[fl]
    o = 0
    while o < len(b):
        if b[o] >= 0xF0:
            o += b[o] & 0xF
            continue
        lf = struct.unpack_from("<H", b, o)[0]
        if lf == 0x150D:  # member
            attr, t = struct.unpack_from("<HI", b, o + 2)
            off, p = num(b, o + 8)
            n, o = cstr(b, p)
            print(f"  member {off:#x} {n}: {tname(t)}")
        elif lf == 0x150F:  # method
            cnt, ml = struct.unpack_from("<HI", b, o + 2)
            n, o = cstr(b, o + 8)
            print(f"  method {n} x{cnt}")
            mb = recs[ml][1]
            q = 0
            while q < len(mb):
                a, _, t = struct.unpack_from("<HHI", mb, q)
                q += 8
                mp = (a >> 2) & 7
                vb = ""
                if mp in (4, 6):
                    vb = f" vtable+{struct.unpack_from('<I', mb, q)[0]:#x}"
                    q += 4
                print(f"    {MP[mp]}{vb} {sig(t)}")
        elif lf == 0x1511:  # onemethod
            attr, t = struct.unpack_from("<HI", b, o + 2)
            p = o + 8
            vb = ""
            if (attr >> 2) & 7 in (4, 6):
                vb = f" vtable+{struct.unpack_from('<I', b, p)[0]:#x}"
                p += 4
            n, o = cstr(b, p)
            print(f"  onemethod {n} {MP[(attr >> 2) & 7]}{vb} {sig(t)}")
        elif lf == 0x1510:  # nested
            n, o = cstr(b, o + 8)
            print(f"  nested {n}")
        elif lf == 0x1400:  # bclass
            attr, t = struct.unpack_from("<HI", b, o + 2)
            off, o = num(b, o + 8)
            print(f"  base {tname(t)} at {off:#x}")
        elif lf == 0x1409:  # vfuncptr
            print("  vfptr")
            o += 8
        elif lf == 0x150E:  # static member
            attr, t = struct.unpack_from("<HI", b, o + 2)
            n, o = cstr(b, o + 8)
            print(f"  static {n}: {tname(t)}")
        else:
            print(f"  ?leaf {lf:#x}; stop")
            return


def cmd_class(name, methods):
    found = False
    for ti, (leaf, b) in recs.items():
        if leaf not in (0x1504, 0x1505):
            continue
        cnt, prop, fl, der, vs = struct.unpack_from("<HHIII", b, 0)
        size, o = num(b, 16)
        n, _ = cstr(b, o)
        if n != name or prop & 0x80:  # 0x80 = forward ref (size 0x1): take the full record (C0064 trap)
            continue
        found = True
        print(f"{n} size {size:#x} ti {ti:#x}")
        dump_fl(fl)
        if methods:
            for t2, (lf2, b2) in recs.items():
                if lf2 != 0x1009:  # LF_MFUNCTION
                    continue
                rv, cls, this = struct.unpack_from("<III", b2, 0)
                if cls == ti:
                    cc, attr, np, al, adj = struct.unpack_from("<BBHIi", b2, 12)
                    print(f"  mfunc {t2:#x} ret {tname(rv)} this {tname(this)} nparams {np}")
    return found


def cmd_enum(name):
    found = False
    for ti, (leaf, b) in recs.items():
        if leaf != 0x1507:
            continue
        cnt, prop, ut, fl = struct.unpack_from("<HHII", b, 0)
        n = cstr(b, 12)[0]
        if name not in n or prop & 0x80:
            continue
        found = True
        print(n)
        fb = recs[fl][1]
        o = 0
        while o < len(fb):
            if fb[o] >= 0xF0:
                o += fb[o] & 0xF
                continue
            v, p = num(fb, o + 4)
            nm, o = cstr(fb, p)
            print(f"  {v:#x} {nm}")
    return found


PROC_KINDS = (0x1110, 0x110F)  # S_GPROC32 / S_LPROC32 (name at payload +35)


def scope_locals(buf, q, end=None):
    """Locals of the scope whose opening record ends at offset q of buf, to its S_END (S_BLOCK32 0x1103 nesting
    followed): -> [{kind bprel|regrel|register, name, off, reg, ti}]. Shared by the PDB and obj (/Z7) readers."""
    end = len(buf) if end is None else end
    out, depth = [], 0
    while q + 4 <= end:
        ln, k = struct.unpack_from("<HH", buf, q)
        if ln < 2:
            break
        r = buf[q + 4:q + 2 + ln]
        if k == 0x0006:  # S_END
            if depth == 0:
                break
            depth -= 1
        elif k == 0x1103:  # S_BLOCK32
            depth += 1
        elif k == 0x110B:  # S_BPREL32
            off, ti = struct.unpack_from("<iI", r, 0)
            out.append({"kind": "bprel", "name": cstr(r, 8)[0], "off": off, "reg": None, "ti": ti})
        elif k == 0x1111:  # S_REGREL32
            off, ti, reg = struct.unpack_from("<iIH", r, 0)
            out.append({"kind": "regrel", "name": cstr(r, 10)[0], "off": off, "reg": reg, "ti": ti})
        elif k == 0x1106:  # S_REGISTER
            ti, reg = struct.unpack_from("<IH", r, 0)
            out.append({"kind": "register", "name": cstr(r, 6)[0], "off": None, "reg": reg, "ti": ti})
        q += 2 + ln
    return out


def locals(streams, proc):
    """-> [{stream, seg, off, locals}] per S_GPROC32/S_LPROC32 whose name equals proc exactly, in each module stream
    (locals as scope_locals)."""
    want = proc.encode() + b"\0"
    out = []
    for si, st in enumerate(streams):
        if not st or want not in st:
            continue
        pos = 4  # module streams start with a 4-byte signature
        while pos + 4 <= len(st):
            ln, kind = struct.unpack_from("<HH", st, pos)
            if ln < 2:
                break
            rec = st[pos + 4:pos + 2 + ln]
            if kind in PROC_KINDS and len(rec) > 35 and rec[35:35 + len(want)] == want:
                off, seg = struct.unpack_from("<IH", rec, 28)
                out.append({"stream": si, "seg": seg, "off": off, "locals": scope_locals(st, pos + 2 + ln)})
            pos += 2 + ln
    return out


def cmd_locals(streams, proc):
    procs = locals(streams, proc)
    for p in procs:
        si = p["stream"]
        for x in p["locals"]:
            if x["kind"] == "bprel":
                print(f"stream {si} bprel {x['off']:#x} {x['name']} : {tname(x['ti'])} ({x['ti']:#x})")
            elif x["kind"] == "regrel":
                print(f"stream {si} regrel r{x['reg']} {x['off']:#x} {x['name']} : {tname(x['ti'])} ({x['ti']:#x})")
            else:
                print(f"stream {si} register r{x['reg']} {x['name']} : {tname(x['ti'])} ({x['ti']:#x})")
    return bool(procs)


def fl_names(fl, seen=None):
    """Member names of fieldlist ti `fl` (members, methods, onemethods, nested, static, enumerators; LF_INDEX
    continuations followed). Unknown leaves end the walk (names so far kept)."""
    seen = set() if seen is None else seen
    out = set()
    if fl in seen or fl not in recs:
        return out
    seen.add(fl)
    b = recs[fl][1]
    o = 0
    while o < len(b):
        if b[o] >= 0xF0:
            o += b[o] & 0xF
            continue
        lf = struct.unpack_from("<H", b, o)[0]
        if lf in (0x150D, 0x1502):  # member (attr, type, offset) / enumerate (attr, value)
            _, p = num(b, o + 8) if lf == 0x150D else num(b, o + 4)
            n, o = cstr(b, p)
        elif lf in (0x150F, 0x1510, 0x150E):  # method, nested, static: 8-byte head then name
            n, o = cstr(b, o + 8)
        elif lf == 0x1511:  # onemethod: INTRO/PUREINTRO carry a vtable offset
            p = o + 8 + (4 if (struct.unpack_from("<H", b, o + 2)[0] >> 2) & 7 in (4, 6) else 0)
            n, o = cstr(b, p)
        elif lf == 0x1400:  # bclass
            _, o = num(b, o + 8)
            continue
        elif lf in (0x1401, 0x1402):  # vbclass / ivbclass: two numerics after attr, btype, vbptr type
            _, p = num(b, o + 12)
            _, o = num(b, p)
            continue
        elif lf == 0x1409:  # vfuncptr
            o += 8
            continue
        elif lf == 0x1404:  # LF_INDEX: continuation fieldlist
            out |= fl_names(struct.unpack_from("<I", b, o + 4)[0], seen)
            o += 8
            continue
        else:
            break
        out.add(n)
    return out


def names():
    """TPI name listing (load_tpi first): -> (types {name}, members {type name: {member name}}), every
    LF_CLASS/LF_STRUCTURE/LF_UNION/LF_ENUM record (forward refs included in types; members from full records)."""
    types, members = set(), {}
    for leaf, b in recs.values():
        if leaf in (0x1504, 0x1505, 0x1506):
            prop, fl = struct.unpack_from("<HI", b, 2)
            n = cstr(b, num(b, 16)[1] if leaf != 0x1506 else num(b, 8)[1])[0]
        elif leaf == 0x1507:
            prop, fl = struct.unpack_from("<H", b, 2)[0], struct.unpack_from("<I", b, 8)[0]
            n = cstr(b, 12)[0]
        else:
            continue
        types.add(n)
        if not prop & 0x80 and fl:
            members.setdefault(n, set()).update(fl_names(fl))
    return types, members


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--pdb", help="PDB path (default: prototype_dir/default.pdb from the config)")
    ap.add_argument("--config", help="local paths config (vv_inputs)")
    sub = ap.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("class", help="dump one class/struct fieldlist")
    c.add_argument("name")
    c.add_argument("--methods", action="store_true", help="also list LF_MFUNCTION records of the class")
    e = sub.add_parser("enum", help="enumerators of LF_ENUMs whose name contains NAME")
    e.add_argument("name")
    lo = sub.add_parser("locals", help="S_BPREL32/S_REGREL32/S_REGISTER locals of a proc")
    lo.add_argument("proc")
    sub.add_parser("names", help="every TPI type name, then each type's member names")
    a = ap.parse_args()
    pdb = Path(a.pdb) if a.pdb else vv_inputs.input_path("prototype_dir", "default.pdb",
                                                          vv_inputs.load_config(a.config))
    try:
        streams = read_streams(pdb.read_bytes())["streams"]
        load_tpi(streams)
    except (OSError, NotPdb, struct.error, IndexError, TypeError) as ex:
        print(f"pdb_types: cannot read {pdb}: {ex}", file=sys.stderr)
        return 2
    try:
        if a.cmd == "class":
            ok = cmd_class(a.name, a.methods)
        elif a.cmd == "enum":
            ok = cmd_enum(a.name)
        elif a.cmd == "names":
            types, members = names()
            for n in sorted(types):
                print(f"type {n}")
            for n in sorted(members):
                for m in sorted(members[n]):
                    print(f"member {n}::{m}")
            ok = bool(types)
        else:
            ok = cmd_locals(streams, a.proc)
    except TypeSizeError as ex:
        print(f"pdb_types: refused: {ex}; no other tool reads this TPI (C0023): extend "
              f"tools/pdb_types.py tsize", file=sys.stderr)
        return 2
    if not ok:
        print(f"pdb_types: {a.cmd} {getattr(a, 'name', None) or getattr(a, 'proc', '')}: not found", file=sys.stderr)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
