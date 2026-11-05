// UNIT: src/Apps/Vince/core/AmbientBird.cpp OBJ: core\XBox\OfficialRelease\AmbientBird.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits; // src proto-pdb:default.pdb char_traits
template <class T> class allocator; // src proto-pdb:default.pdb allocator
template <class E, class T, class A> class basic_string; // src proto-pdb:default.pdb basic_string
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    void RotateJ(float);
    float LengthSquared() const { return x * x + y * y + z * z; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    T Fetch();

    char field_0[0x14];                // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> FetchAnim() { return _animQueue.Fetch(); }

    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2
    };

    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
    const Quaternion& SetRotation(const Quaternion&);
    Vector3 GetFacing() const;
    void SetOption(unsigned int option, bool value, bool recursive);

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C

    char field_A4[0xAC - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0xB4 - 0xAC];        // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    virtual void ReturnToHome();                                   // vtable+0x44
    const Quaternion& GetHomeRotation() const { return _homeRotation; }

    char field_200[0x240 - 0x200];     // 0x200
    Quaternion _homeRotation;          // 0x240
    char field_250[0x2C8 - 0x250];     // 0x250
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
}

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

    void RegisterTimerEvent(unsigned int, float, bool);

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum WanderModes { // NAME: proto-pdb:default.pdb
        WANDER_NONE = 0,
        WANDER_PATROL = 1,
        WANDER_CHASE = 2,
        WANDER_GOTO = 3
    };
    Critter* GetCritter() { return (Critter*)_node; }
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    Vector3 _wanderPatrolCenter;       // 0x50
    float _wanderPatrolRadius;         // 0x5C
    char field_60[0x68 - 0x60];        // 0x60
    int _wanderMode;                   // 0x68
    Vector3 _wanderDir;                // 0x6C
    char field_78[0xBC - 0x78];        // 0x78
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class AmbientBirdMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_BORED = 0
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_PATROL = 1,
        STATE_FLY_OUT = 2,
        STATE_FLY_IN = 3,
        STATE_GONE = 4
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_GO_AWAY = 0x14
    };

    AmbientBirdMain(SwampSceneNode* node);
    virtual void Message(SwampScript*, int, void*);
    virtual void TimerEvent(unsigned int);
    virtual void AnimationFinished();
protected:
    virtual void SetState(int state);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    MultiAnimation _multiAnimFlyIn;    // 0xBC
    MultiAnimation _multiAnimFlyOut;   // 0xD0
    MultiAnimation _multiAnimPatrol;   // 0xE4
    MultiAnimation _multiAnimIdle;     // 0xF8
    char field_10C[0x114 - 0x10C];     // 0x10C
};
DECOMP_SIZE_ASSERT(AmbientBirdMain, 0x114);

void* operator new(unsigned int size, const char* file, int line);

// FUNCTION: USA 0x001120B9
// PROTOTYPE: PREVIEW 0x004FC8F4 identical
// NAME: proto-pdb:default.pdb
void AmbientBirdMain::Message(SwampScript* source, int message, void* data)
{
    if (message != MESSAGE_GO_AWAY)
        CritterMain::Message(source, message, data);
    else
        SetState(STATE_GONE);
}

// FUNCTION: USA 0x001120CF
// PROTOTYPE: PREVIEW 0x004FC90A identical
// NAME: proto-pdb:default.pdb
void AmbientBirdMain::TimerEvent(unsigned int timerId)
{
    if (timerId == TIMER_BORED) {
        switch (_state) {
        case STATE_PATROL:
            SetState(STATE_IDLE);
            break;
        }
    }
}

// FUNCTION: USA 0x001120E6
// PROTOTYPE: PREVIEW 0x004FC921 identical
// NAME: proto-pdb:default.pdb
void AmbientBirdMain::AnimationFinished()
{
    switch (_state) {
    case STATE_IDLE:
    case STATE_FLY_IN:
        SetState(STATE_PATROL);
        break;
    case STATE_FLY_OUT:
        SetState(STATE_GONE);
        break;
    }
}

// FUNCTION: USA 0x001122FA
// PROTOTYPE: PREVIEW 0x004FCB35 identical
// NAME: proto-pdb:default.pdb
void AmbientBirdMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_IDLE:
        GetCritter()->StartAnimation(_multiAnimIdle.FetchAnim(), false, false);
        _wanderMode = WANDER_NONE;
        break;
    case STATE_PATROL: {
        GetCritter()->StartAnimation(_multiAnimPatrol.FetchAnim(), true, false);
        _wanderMode = WANDER_PATROL;
        // !FAKE: inliner; pointer local, hand form of the inlined Vector3 difference (C0191); retail keeps it in
        // x87 registers, no free operator- in this unit (D78; match_one 100.0 with this form)
        const Vector3* v1 = &GetCritter()->GetWorldPosition();
        Vector3 v2(_wanderPatrolCenter.x - v1->x, _wanderPatrolCenter.y - v1->y, _wanderPatrolCenter.z - v1->z);
        if (v2.LengthSquared() < _wanderPatrolRadius * _wanderPatrolRadius) {
            float angle = (float)BeepLib::RandomFloat(-3.1415927f, 3.1415927f);
            Vector3 facing = GetCritter()->GetFacing();
            facing.RotateJ(angle);
            _wanderDir = facing;
        }
        RegisterTimerEvent(TIMER_BORED, (float)BeepLib::RandomFloat(1.5f, 2.5f), false);
        break;
    }
    case STATE_FLY_OUT:
        GetCritter()->SetRotation(GetCritter()->GetHomeRotation());
        GetCritter()->StartAnimation(_multiAnimFlyOut.FetchAnim(), false, false);
        break;
    case STATE_FLY_IN:
        _node->SetOption(SwampSceneNode::DontDraw, false, false);
        GetCritter()->StartAnimation(_multiAnimFlyIn.FetchAnim(), false, false);
        break;
    case STATE_GONE:
        GetCritter()->ReturnToHome();
        _node->SetOption(SwampSceneNode::DontDraw, true, false);
        break;
    }
}

// FUNCTION: USA 0x0011263A
// PROTOTYPE: PREVIEW 0x004FCE91 identical
// NAME: proto-pdb:default.pdb
SwampScript* AmbientBirdMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\AmbientBird.h", 10) AmbientBirdMain(node);
}
