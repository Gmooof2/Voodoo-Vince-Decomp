// UNIT: src/Apps/Vince/scripts/Crypt_city/Cannon.cpp OBJ: Xbox\OfficialRelease\Cannon.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

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

// Retail layout: Critter _CritterType 0x258 -> 0x268 (+0x10).
class Critter { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x268];               // 0x00
    CritterType _CritterType;          // 0x268
};

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3&, Vector3&) const;
    // !FAKE: __forceinline; plain inline stays an out-of-line call at /O1, retail inlines it
    __forceinline Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }

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
    _baseFacing = _baseRotation.Apply(Vector3(0.0f, 0.0f, 1.0f));
}

class SoundNode; // NAME: proto-pdb:default.pdb

class CannonMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    CannonMain(SwampSceneNode* node);
    virtual ~CannonMain();

    AimingCam* _cannonCam;             // 0xBC
    SoundNode* _tensionSound;          // 0xC0
    float _pan;                        // 0xC4
    float _pitch;                      // 0xC8
};
DECOMP_SIZE_ASSERT(CannonMain, 0xCC);

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
// FUNCTION: USA 0x0005DF92
// PROTOTYPE: PREVIEW 0x0044A3D1 identical
// NAME: proto-pdb:default.pdb
SwampScript* CannonCartMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\sarcophagushustle\\Cannon.h", 82) CannonCartMain(node);
}
