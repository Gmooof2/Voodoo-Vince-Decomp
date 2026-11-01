"""Write a unit's src file holding SwampScript creationFunc rows (C0126 form, generated per C0135).

Usage: PY tools/gen_creationfunc.py <obj basename> <VA>... [--out DIR]
  <obj basename>  unit object, e.g. switchCrypt.obj (matched case-insensitively against units.toml names)
  <VA>            creationFunc row VAs (hex) of that unit, from pool_buckets.md / next_candidates.md
  --out DIR       write <name>.cpp into DIR instead of the unit's src path (default: src/<path from objname>)

Per row: the class's PDB base chain to SwampScript as size shells (D78: names from the PDB, opaque tail
field_<off>), ctor access from the retail reloc name, creationFunc access from the mangled name, file literal from
the retail `??_C` pin, line and size from the pushes, PROTOTYPE line from proto_map. Asserts the pushed size equals
the PDB size and the ctor reloc exists; a row that fails an assert is not the script form (C0135). The written file
carries the unit's current opt; set opt from a control first (C0065, C0135). Needs llvm-objdump on PATH.
Origin: 3.3 T13 scratch gen_cf.py (69 creationFunc rows matched), promoted at 3.3 T18.
"""
import argparse, json, os, pathlib, re, subprocess, sys, tomllib

ROOT = pathlib.Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import vv_inputs, xbe_info  # noqa: E402

B = chr(92)
NL = chr(10)
P = sys.executable

PRELUDE = r"""namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum CollisionType { // NAME: proto-pdb:default.pdb
        HorizontalCollision = 0,
        VerticalCollision = 1
    };
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04
    virtual void Activate();                                       // vtable+0x08
    virtual void Deactivate();                                     // vtable+0x0C
    virtual void FrameTick();                                      // vtable+0x10
    virtual void TimerEvent(unsigned int);                         // vtable+0x14
    virtual void AnimationFinished();                              // vtable+0x18
    virtual void SecondaryAnimationFinished();                     // vtable+0x1C
    virtual void AnimationInterrupted();                           // vtable+0x20
    virtual void SecondaryAnimationInterrupted();                  // vtable+0x24
    virtual void AnimationTrigger(const SwampString&, float, float, unsigned int); // vtable+0x28
    virtual bool LosingControl(const SwampString&);                // vtable+0x2C
    virtual void GainingControl(SwampScript*);                     // vtable+0x30
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    virtual void EnteringRegion(SwampSceneNode*);                  // vtable+0x38
    virtual void LeavingRegion(SwampSceneNode*);                   // vtable+0x3C
    virtual void SoundFinished(const SwampString&);                // vtable+0x40
    virtual void SoundStreamerFinished(const SwampString&);        // vtable+0x44
    virtual void MovieFinished(const SwampString&);                // vtable+0x48
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x50
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual SwampString Save();                                    // vtable+0x58
    virtual void SetNode(SwampSceneNode*);                         // vtable+0x5C
    virtual void InvalidateScript();                               // vtable+0x60

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);"""

KNOWN = {"SwampScript": 0x4C, "CritterMain": 0xBC}


def main():
    ap = argparse.ArgumentParser(description=__doc__.split(NL)[0], epilog=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("obj")
    ap.add_argument("vas", nargs="+")
    ap.add_argument("--out")
    a = ap.parse_args()
    os.chdir(ROOT)
    units = tomllib.loads((ROOT / "config/retail/units.toml").read_text())["unit"]
    hits = [q for q in units if q["name"].lower().endswith(B + a.obj.lower())]
    if len(hits) != 1:
        sys.exit(f"gen_creationfunc: {len(hits)} units match {a.obj}")
    unit = hits[0]
    cuts = sorted((ROOT / "derived/delink").glob("retail-*"))
    if len(cuts) != 1:
        sys.exit(f"gen_creationfunc: expected one derived/delink/retail-* cut, found {len(cuts)}")
    syms = {}
    for f in ("config/retail/symbols.txt", "config/retail/symbols_pin.txt"):
        for line in (ROOT / f).read_text().splitlines():
            p = line.split()
            if len(p) >= 4 and p[0].startswith("0x"):
                syms.setdefault(p[3], int(p[0], 16))
                if p[1] == "func" and p[2] != "-":
                    syms.setdefault(("va", int(p[0], 16)), (p[3], int(p[2], 16)))
    vas = sorted(int(v, 16) for v in a.vas)
    proto = {}
    for l in open(ROOT / "derived/proto_map/retail-xbe/functions.jsonl"):
        d = json.loads(l)
        for t in d.get("targetVAs") or []:
            if int(t, 16) in vas and d.get("hits", 1) == 1:
                proto[int(t, 16)] = (d["protoVA"], d["class"])
    x = vv_inputs.input_path("retail_xbe").read_bytes()
    info = xbe_info.parse_xbe(x)

    def pdb(c):
        r = subprocess.run([P, "tools/pdb_types.py", "class", c], capture_output=True, text=True).stdout
        best = None
        for blk in re.split(r"(?m)^(?=\S)", r):
            m = re.match(r"(\S+) size 0x([0-9a-f]+)", blk)
            if not m or m.group(1) != c:
                continue
            sz = int(m.group(2), 16)
            b = re.search(r"base (\S+) at 0x0", blk)
            if best is None or sz > best[0]:
                best = (sz, b.group(1) if b else None)
        return best

    shells, order = {}, []

    def chain(c):
        while c not in KNOWN and c not in shells:
            sz, base = pdb(c)
            assert base, f"{c}: no base"
            shells[c] = (sz, base)
            chain(base)
            order.append(c)
            return

    out_fn = []
    for va in vas:
        name, size = syms[("va", va)]
        obj = cuts[0] / unit["name"].replace(B, "__")
        off = va - unit["text_start"]
        r = subprocess.run(["llvm-objdump", "-dr", "--no-show-raw-insn", "--x86-asm-syntax=intel",
                            "--start-address=0x%x" % off, "--stop-address=0x%x" % (off + size), str(obj)],
                           capture_output=True, text=True).stdout
        pushes = re.findall(r"push\s+0x([0-9a-f]+)", r)
        lit = re.search(r"DIR32\s+(\?\?_C@\S+)", r).group(1)
        o = xbe_info.va_to_off(info if info is not None else x, syms[lit])
        s = x[o:x.index(b"\0", o)].decode("latin-1")
        cls = re.match(r"\?creationFunc@(\w+)@@", name).group(1)
        acc = {"C": "private", "K": "protected", "S": "public"}[name[len("?creationFunc@" + cls + "@@")]]
        assert name.endswith("@@CAPAVSwampScript@@PAVSwampSceneNode@@@Z") or acc != "private", name
        line, nsz = int(pushes[0], 16), int(pushes[-1], 16)
        chain(cls)
        assert shells[cls][0] == nsz, (cls, shells[cls], nsz)
        cm = re.search(r"REL32\s+(\?\?0" + cls + r"@@(\w)AE@PAVSwampSceneNode@@@Z)", r)
        assert cm, (cls, "ctor reloc")
        shells[cls] = shells[cls] + (acc, {"Q": "public", "A": "private", "I": "protected"}[cm.group(2)])
        pv = proto.get(va)
        ann = ([f"// FUNCTION: USA 0x{va:08X}"] + ([f"// PROTOTYPE: PREVIEW {pv[0]} {pv[1]}"] if pv else [])
               + ["// NAME: proto-pdb:default.pdb"])
        hdr = "header path" if s.endswith(".h") else "relative path"
        esc = s.replace(B, B + B)
        out_fn.append(NL.join(ann + [f"SwampScript* {cls}::creationFunc(SwampSceneNode* node)", "{",
            f"    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ ({hdr}), ours differs",
            f'    return new("{esc}", {line}) {cls}(node);', "}"]))

    o = unit["objname"]
    path = re.sub(r"(?i)^d:[/" + B + B + r"]beep[/" + B + B + "]", "", o).replace(B, "/")
    path = re.sub(r"(?i)/xbox/officialrelease/", "/", path)
    path = "src/" + path[:-4] + ".cpp"
    depth = path.count("/") - 1
    txt = [f"// UNIT: {path} OBJ: {unit['name']} OPT: {unit.get('opt')}",
           f'#include "{"../" * depth}include/decomp.h"']
    txt.append(PRELUDE)
    txt.append("")
    txt.append("void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z")
    for c in order:
        sz, base = shells[c][:2]
        psz = KNOWN.get(base, shells.get(base, (None,))[0])
        body = ["", f"class {c} : public {base} {{ // NAME: proto-pdb:default.pdb", "public:"]
        if len(shells[c]) == 4:
            if shells[c][3] != "public":
                body += [f"{shells[c][3]}:", f"    {c}(SwampSceneNode* node);", "public:"]
            else:
                body.append(f"    {c}(SwampSceneNode* node);")
            if shells[c][2] != "public":
                body += [f"{shells[c][2]}:", "    static SwampScript* creationFunc(SwampSceneNode* node);", "public:"]
            else:
                body.append("    static SwampScript* creationFunc(SwampSceneNode* node);")
        if sz > psz:
            body += ["", f"    char field_{psz:X}[0x{sz:X} - 0x{psz:X}];".ljust(39) + f"// 0x{psz:X} (not reached)"]
        body += ["};", f"DECOMP_SIZE_ASSERT({c}, 0x{sz:X});"]
        txt += body
    for f in out_fn:
        txt += ["", f]
    TXT = NL.join(txt) + NL
    for p, q in (("public:" + NL + "private:" + NL, "private:" + NL), ("public:" + NL + "protected:" + NL, "protected:" + NL),
                 ("private:" + NL + "public:" + NL + "private:" + NL, "private:" + NL), ("public:" + NL + "};", "};"),
                 ("public:" + NL + NL + "    char", NL + "    char")):
        while p in TXT:
            TXT = TXT.replace(p, q)
    out = os.path.join(a.out, os.path.basename(path)) if a.out else path
    os.makedirs(os.path.dirname(out), exist_ok=True)
    open(out, "w", newline=NL).write(TXT.rstrip(NL) + NL)
    print(out, len(out_fn), "rows:", " ".join(f"0x{v:08X}:{syms[('va', v)][0]}" for v in vas))


if __name__ == "__main__":
    main()
