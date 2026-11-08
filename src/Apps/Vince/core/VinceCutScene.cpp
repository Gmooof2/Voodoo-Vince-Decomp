// UNIT: src/Apps/Vince/core/VinceCutScene.cpp OBJ: core\XBox\OfficialRelease\VinceCutScene.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                       // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    int _RefCount;                     // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class CollisionResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    char field_4[0x40 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

template <class T>
class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }

    T* _Object;                        // 0x00
};

class CritterCollision { // NAME: proto-pdb:default.pdb
public:
    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};
DECOMP_SIZE_ASSERT(CritterCollision, 0x8);

class Critter { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetVelocity() const { return _Velocity; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }

    char field_0[0x20C];               // 0x00
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x2C8 - 0x218];     // 0x218
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetGravity() const { return Gravity; }

    char field_0[0x8C];                // 0x00
    float Gravity;                     // 0x8C
};
extern Globals gSettings; // NAME: proto-pdb:default.pdb

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
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    CritterCollision MoveMe(float gravity);

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class VinceCutScene : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void FrameTick();
    Vince* GetVince() { return pManager->GetVince(); }
};
DECOMP_SIZE_ASSERT(VinceCutScene, 0xBC);

// FUNCTION: USA 0x001225F2
// PROTOTYPE: PREVIEW 0x0050B2C5 identical
// NAME: proto-pdb:default.pdb
void VinceCutScene::FrameTick()
{
    // !FAKE: c2 register allocation; stand-in local; retail computes &_Velocity once for both copies, two GetVince() calls recompute it (match_one 86.7 -> 100.0)
    Vince* p = GetVince();
    Vector3 vel = p->GetVelocity();
    vel.x = vel.z = 0.0f;
    p->SetVelocity(vel);
    MoveMe(gSettings.GetGravity());
}
