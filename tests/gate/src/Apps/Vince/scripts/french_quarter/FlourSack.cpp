// gate fixture (1.5 T2): FlourSack.obj unit, copied from tests/pin/floursackmain_dtor.cpp; built with the unit opt
// `/O1 /Oy` (the pin used `/O1 /Oy /Ob1`; /Ob1 is the /O1 default).
//
// compiler-pin test (1.4 T9): FlourSackMain::~FlourSackMain, retail 0x00079868, 31 B (fpo, ob).
// Types per prototype default.pdb (docs/evidence/compiler_pin.md): FlourSackMain size 0x108 (base HeavyObjectMain,
// _animStuck 0xF8, _animFall 0xFC, _id 0x100, bools 0x104..0x107); HeavyObjectMain size 0xF8 (base CarriedObjectMain,
// virtual dtor); CarriedObjectMain size 0xF8 (base CritterMain); ResourceBase size 0x24 (_Name 0x0, _RefCount 0x1C,
// _valid 0x20); AnimationResource size 0x34 (base ResourceBase). Members this body does not touch are opaque `char`
// pads sized by those PDB offsets (D78): CritterMain past its vptr, ResourceBase::_Name (std::basic_string, 0x1C),
// AnimationResource past its base. Callee ??1CarriedObjectMain@@UAE@XZ stays external (tail jmp reloc).
// ~HeavyObjectMain and ~FlourSackMain are not user-declared: retail has no vptr store in the body (its ctor 0x796EC
// and ~CarriedObjectMain 0x10F085 do store theirs), and only the implicit dtor omits it under cl 13.10; a declared
// dtor, inline `{}` or out-of-line, stores the vptr (+6 B each).

class ResourceBase {
public:
    char _Name[0x1C];  // std::basic_string _Name 0x0, opaque here (not used by this body)
    unsigned int _RefCount;
    bool _valid;
};

class AnimationResource : public ResourceBase {
public:
    char _pad24[0x34 - 0x24];  // AnimationResource members past ResourceBase, opaque (not used by this body)
};

template<class T>
class ResourceHandle {
public:
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    T* _Resource;
};

class CritterMain {
public:
    virtual ~CritterMain();
    char _pad04[0xF8 - 0x4];  // CritterMain members past the vptr, opaque (not used by this body)
};

class CarriedObjectMain : public CritterMain {
public:
    virtual ~CarriedObjectMain();
};

class HeavyObjectMain : public CarriedObjectMain {
public:
    // destructor implicit (virtual via CarriedObjectMain)
};

// FUNCTION: USA 0x00079868
// PROTOTYPE: PREVIEW 0x0046587C identical
// NAME: proto-pdb:default.pdb
// FlourSackMain::~FlourSackMain: implicit (compiler-generated from the class below)
class FlourSackMain : public HeavyObjectMain {
public:
    // destructor implicit (virtual via HeavyObjectMain)
    ResourceHandle<AnimationResource> _animStuck;
    ResourceHandle<AnimationResource> _animFall;
    int _id;
    bool _pickedUp;
    bool _vinceInBounds;
    bool _destroyOnImpact;
    bool _firstDrop;
};

// !FAKE: test scaffold, not game code: retail emits the implicit ??1FlourSackMain@@UAE@XZ because FlourSack.obj defines
// the ctor (0x796EC, not modeled here); this qualified call odr-uses it instead. Not part of the compared symbol.
void pin_emit_FlourSackMain_dtor(FlourSackMain* p)
{
    p->FlourSackMain::~FlourSackMain();
}
