// UNIT: src/Apps/Vince/scripts/Crypt_city/Mirror.cpp OBJ: Xbox\OfficialRelease\Mirror.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString& id, int def) const;
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    void Normalize();

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    Vector3 GetFacing() const;
    SwampScript* GetScript() const;
    const Vector3& SetLocation(const Vector3& location);
    const Quaternion& SetRotation(const Quaternion& rotation);
    void AddChild(SwampSceneNode* child);
};

class SwampGamePad { // NAME: proto-pdb:default.pdb
public:
    unsigned char GetButtonState(int b) const;
    unsigned char GetButtonPreviousState(int b) const;
    bool WasButtonReleased(int b) const;

    char field_0[0x98];                // 0x00
};
DECOMP_SIZE_ASSERT(SwampGamePad, 0x98);


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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void GetBoneOrientation(const SwampString& bone, Quaternion& rot, Vector3& pos) const;
};

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_STATIC = 0x4
};

// Retail layout: Critter _CritterType 0x258 -> 0x268 (+0x10).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_4[0x268 - 0x4];         // 0x04
    CritterType _CritterType;          // 0x268
};

class LightBeam : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    LightBeam();

    char field_4[0xB0 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(LightBeam, 0xB0);

class GameCam; // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line);

class BellMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ADDMIRROR = 0x14,
        MESSAGE_SUBTRACTMIRROR = 0x15,
        MESSAGE_STRIKE = 0x16,
        MESSAGE_ISPUZZLEDONE = 0x17
    };
};

class MirrorUrnMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    MirrorUrnMain(SwampSceneNode* node);
    virtual bool TestActivate() const;
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
    bool IsPuzzleDone() const;
    void TestForAim();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    GameCam* _mirrorCam;               // 0xBC
    Critter* _bell;                    // 0xC0
    Vector3 _focusPos;                 // 0xC4
    Quaternion _focusRot;              // 0xD0
    bool _okayToQuit;                  // 0xE0
    bool _wasMoved;                    // 0xE1
    bool _onBell;                      // 0xE2
};
DECOMP_SIZE_ASSERT(MirrorUrnMain, 0xE4);

// FUNCTION: USA 0x0006B8A0
// PROTOTYPE: PREVIEW 0x00457C63 identical
// NAME: proto-pdb:default.pdb
bool SwampGamePad::WasButtonReleased(int b) const
{
    return GetButtonState(b) < 0x20 && GetButtonPreviousState(b) >= 0x20;
}

// FUNCTION: USA 0x0006B8CA
// PROTOTYPE: PREVIEW 0x00457C8D identical
// NAME: proto-pdb:default.pdb
bool MirrorUrnMain::IsPuzzleDone() const
{
    bool b;
    _bell->GetScript()->Message(0, BellMain::MESSAGE_ISPUZZLEDONE, &b);
    return b;
}

// FUNCTION: USA 0x0006B8ED
// PROTOTYPE: PREVIEW 0x00457CB0 identical
// NAME: proto-pdb:default.pdb
void MirrorUrnMain::TestForAim()
{
    // !FAKE: c2 scheduling; dummy local; retail reads _onBell into bl before the calls; the proto PDB lists no such local
    bool tmp = _onBell;
    // !FAKE: inliner; pointer locals + hand form of retail's inlined Vector3 difference (no free operator in the PDB, C0191)
    const Vector3* v0 = &_node->GetWorldPosition();
    const Vector3* v1 = &_bell->GetWorldPosition();
    Vector3 diff;
    diff.x = v1->x - v0->x;
    diff.y = v1->y - v0->y;
    diff.z = v1->z - v0->z;
    diff.Normalize();
    // !FAKE: inliner; pointer local + hand form of retail's inlined dot product (C0191)
    const Vector3* p = &_node->GetFacing();
    _onBell = diff.z * p->z + diff.y * p->y + diff.x * p->x > 0.99f;
    if (_onBell) {
        if (!tmp)
            _bell->GetScript()->Message(this, BellMain::MESSAGE_ADDMIRROR, 0);
    } else if (tmp) {
        _bell->GetScript()->Message(this, BellMain::MESSAGE_SUBTRACTMIRROR, 0);
    }
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 3 = vptr store, two literals)
// FUNCTION: USA 0x0006B9B0
// PROTOTYPE: PREVIEW 0x00457D73 near-identical
// NAME: proto-pdb:default.pdb
MirrorUrnMain::MirrorUrnMain(SwampSceneNode* node)
    : CritterMain(node), _mirrorCam(0), _bell(0), _okayToQuit(false), _wasMoved(false), _onBell(false)
{
    // !FAKE: c2 register allocation; local; retail holds _node in edi across the calls (match_one 90.25 -> 100.0);
    // the proto PDB lists no such local
    Critter* critter = (Critter*)_node;
    _state = 0;
    critter->_CritterType = CRITTER_TYPE_STATIC;
    critter->GetBoneOrientation("locator_center", _focusRot, _focusPos);
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    LightBeam* beam = new("Gwistlethorpe\\Mirror.cpp", 36) LightBeam;
    beam->SetLocation(_focusPos);
    beam->SetRotation(_focusRot);
    critter->AddChild(beam);
}
#endif

// FUNCTION: USA 0x0006BB69
// PROTOTYPE: PREVIEW 0x00457F2C identical
// NAME: proto-pdb:default.pdb
bool MirrorUrnMain::TestActivate() const
{
    if (_state == 1 && !IsPuzzleDone())
        return true;
    return false;
}

// FUNCTION: USA 0x0006BEE2
// PROTOTYPE: PREVIEW 0x004581CE identical
// NAME: proto-pdb:default.pdb
void MirrorUrnMain::Load(const BeepTDF& tdf)
{
    int state = tdf.GetIntRecord("state", 0);
    _wasMoved = tdf.GetBoolRecord("wasmoved", false);
    SetState(state);
}

// FUNCTION: USA 0x0006BF50
// PROTOTYPE: PREVIEW 0x0045823C identical
// NAME: proto-pdb:default.pdb
SwampString MirrorUrnMain::Save()
{
    // !FAKE: c2 register allocation; dummy local; retail loads _state into esi before the first format call (no PDB local; match_one 86.57 -> 100.0)
    int v0 = _state;
    return format("\tstate = %d;\n", v0) + format("\twasmoved = %d;\n", _wasMoved);
}

// FUNCTION: USA 0x0006BFB2
// PROTOTYPE: PREVIEW 0x0045829E identical
// NAME: proto-pdb:default.pdb
SwampScript* MirrorUrnMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\gwistlethorpe\\Mirror.h", 12) MirrorUrnMain(node);
}
