// UNIT: src/Apps/Vince/scripts/Crypt_city/Cannon.cpp OBJ: Xbox\OfficialRelease\Cannon.obj OPT: /O1 /Oy
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
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString& id, int def) const;
};
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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual SwampString Save();                                    // vtable+0x58
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
protected:
    virtual void SetState(int);                                    // vtable+0x6C
public:

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class CannonCartMain : public CritterMain { // NAME: proto-pdb:default.pdb
private:
    CannonCartMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CannonCartMain, 0xBC);

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
}

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x38C];               // 0x00
    float _maxCannonPan;               // 0x38C
    float _maxCannonPitchDown;         // 0x390
    float _maxCannonPitchUp;           // 0x394
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_STATIC = 0x4
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager* GetAnimationManager() { return _animationManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
    void SetAnimSpeed(float speed);
    Matrix GetBoneOrientation(const SwampString&) const;
};

// Retail layout: Critter _CritterType 0x258 -> 0x268 (+0x10).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_4[0x268 - 0x4];         // 0x04
    CritterType _CritterType;          // 0x268
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class Cannon : public Critter { // NAME: proto-pdb:default.pdb
};

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    // !FAKE: inliner; hand form of the PDB inline conversion operator (FanCeiling.cpp form)
    operator Vector3() const { return *(const Vector3*)this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class GameCam { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x11C];               // 0x00
};

class AimingCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void SetBaseRotation(const Quaternion& q);

    Quaternion _baseRotation;          // 0x11C
    Vector3 _baseFacing;               // 0x12C
    char field_138[0x188 - 0x138];     // 0x138
};
DECOMP_SIZE_ASSERT(AimingCam, 0x188);

// FUNCTION: USA 0x0005D50B
// PROTOTYPE: PREVIEW 0x00449963 identical
// NAME: proto-pdb:default.pdb
void AimingCam::SetBaseRotation(const Quaternion& q)
{
    _baseRotation = q;
    // !FAKE: inliner; dummy local v0 for the by-value Apply; retail inlines it, /O1 keeps it out of line (T4.c1 E.SB 100.0)
    Vector3 v0;
    _baseRotation.Apply(Vector3(0.0f, 0.0f, 1.0f), v0);
    _baseFacing = v0;
}

class SoundNode; // NAME: proto-pdb:default.pdb

class CannonMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_UPRIGHT = 0,
        STATE_TIPPING = 1,
        STATE_TIPPED = 2,
        STATE_AIMING = 3
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ISTIPPED = 0x14
    };

    CannonMain(SwampSceneNode* node);
    virtual ~CannonMain();
    virtual void AnimationFinished();
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
private:
    void StartAiming();
    Cannon* GetCannon() { return (Cannon*)_node; }
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    AimingCam* _cannonCam;             // 0xBC
    SoundNode* _tensionSound;          // 0xC0
    float _pan;                        // 0xC4
    float _pitch;                      // 0xC8
};
DECOMP_SIZE_ASSERT(CannonMain, 0xCC);

// FUNCTION: USA 0x0005D56D
// PROTOTYPE: PREVIEW 0x004499C5 identical
// NAME: proto-pdb:default.pdb
void CannonMain::Message(SwampScript* source, int message, void* data)
{
    if (message != MESSAGE_ISTIPPED) {
        CritterMain::Message(source, message, data);
        return;
    }
    *(bool*)data = _state != STATE_UPRIGHT;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1 = the vptr store; gSettings+0x38C.. resolve)
// FUNCTION: USA 0x0005D5AE
// PROTOTYPE: PREVIEW 0x00449A06 near-identical
// NAME: proto-pdb:default.pdb
CannonMain::CannonMain(SwampSceneNode* node) : CritterMain(node)
{
    _cannonCam = 0;
    _tensionSound = 0;
    _state = 0;
    ((Critter*)_node)->_CritterType = CRITTER_TYPE_STATIC;
    _pan = BeepLib::RandomFloat(-gSettings._maxCannonPan, gSettings._maxCannonPan);
    _pitch = BeepLib::RandomFloat(-gSettings._maxCannonPitchDown, gSettings._maxCannonPitchUp);
}
#endif
// FUNCTION: USA 0x0005D628
// PROTOTYPE: PREVIEW 0x00449A80 identical
// NAME: proto-pdb:default.pdb
SwampScript* CannonMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\sarcophagushustle\\Cannon.h", 41) CannonMain(node);
}

// FUNCTION: USA 0x0005DBCE
// PROTOTYPE: PREVIEW 0x0044A00D identical
// NAME: proto-pdb:default.pdb
void CannonMain::AnimationFinished()
{
    SetState(STATE_TIPPED);
    StartAiming();
}

// FUNCTION: USA 0x0005DDCD
// PROTOTYPE: PREVIEW 0x0044A20C identical
// NAME: proto-pdb:default.pdb
void CannonMain::Load(const BeepTDF& tdf)
{
    // !FAKE: c2 block layout; dummy local; the record value held in a local gives retail's `mov edi,eax` + `cmp edi,2` (if: sete temp; switch: dec/dec)
    int v0 = tdf.GetIntRecord("state", 0);
    if (v0 == STATE_TIPPED) {
        // !FAKE: c1xx front end; temporary; retail keeps the StartAnimation handle copy alive to the block end, no PDB local (stand-in)
        ResourceHandle<AnimationControl> tmp = GetCannon()->StartAnimation(g_swamp->GetAnimationManager()->Load("cannon_aim"), false, true);
        ((GatorNode*)_node)->SetAnimSpeed(0.0f);
        SetState(STATE_TIPPED);
    }
}

// FUNCTION: USA 0x0005DEDB
// NAME: proto-pdb:default.pdb
CannonCartMain::CannonCartMain(SwampSceneNode* node) : CritterMain(node)
{
    // !FAKE: inliner; hand form of the inline free Vector3 operator+ (no PDB record, D78): C0365 comma expression with a
    // C0191 pointer local ending in the 3-float ctor (split statements 96.92, temp ctor arg 91.97); neutral stand-in locals
    Critter* v0 = pManager->Find("cannon");
    Matrix v1 = ((GatorNode*)_node)->GetBoneOrientation("locator_cannon");
    Vector3 v2 = v1.row[3];
    const Vector3* p;
    v0->SetWorldPosition((p = &_node->GetWorldPosition(), Vector3(v2.x + p->x, v2.y + p->y, v2.z + p->z)));
}

// FUNCTION: USA 0x0005DF92
// PROTOTYPE: PREVIEW 0x0044A3D1 identical
// NAME: proto-pdb:default.pdb
SwampScript* CannonCartMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\sarcophagushustle\\Cannon.h", 82) CannonCartMain(node);
}
