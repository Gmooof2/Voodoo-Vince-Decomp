// UNIT: src/Apps/Vince/scripts/Crypt_city/RatRaceRodeo.cpp OBJ: Xbox\OfficialRelease\RatRaceRodeo.obj OPT: /O1 /Oy
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
template <class T, class A = allocator<T> > class vector {
public:
    ~vector() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
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

    void RegisterFrameTick();

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

class ExitRampMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    ExitRampMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ExitRampMain, 0xBC);

class EntranceRampMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    EntranceRampMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(EntranceRampMain, 0xBC);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }
    ResourceHandle& operator=(const ResourceHandle& other);

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class FloorHoleMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    FloorHoleMain(SwampSceneNode* node);
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_FALL = 1
    };
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);

    ResourceHandle<AnimationResource> _animFall; // 0xBC
};
DECOMP_SIZE_ASSERT(FloorHoleMain, 0xC0);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { DontDraw = 0x2 };
    void SetOption(unsigned int option, bool on, bool recurse);
    Vector3 GetFacing() const;

    char field_0[0xA4];                // 0x00
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void Pause();

    char field_A4[0xE8 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(FireFlyNode, 0xE8);

// FUNCTION: USA 0x00067E94
// NAME: proto-pdb:default.pdb
class MrsGMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    MrsGMain(SwampSceneNode* node);
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_DAMAGE = 0,
        TIMER_ATTACK = 1
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_FROZEN = 0,
        STATE_INITIAL_IDLE = 1,
        STATE_IDLE = 2,
        STATE_STOMP = 3,
        STATE_HURT_STOMP = 4,
        STATE_DAMAGE = 5,
        STATE_SCREAM = 6,
        STATE_DEATH = 7
    };
    virtual void TimerEvent(unsigned int timerId);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void KillScream();

    ResourceHandle<AnimationResource> _animIdle;   // 0xBC
    ResourceHandle<AnimationResource> _animStomp;  // 0xC0
    ResourceHandle<AnimationResource> _animDamage; // 0xC4
    ResourceHandle<AnimationResource> _animScream; // 0xC8
    float _screamTimer;                // 0xCC
    bool _screamWillHurt;              // 0xD0
    char field_D4[0xE0 - 0xD4];        // 0xD4
    int _attackCounter;                // 0xE0
    char field_E4[0xE8 - 0xE4];        // 0xE4
    std::vector<Vector3> _fallingChunkPositions; // 0xE8
    char field_F8[0x100 - 0xF8];       // 0xF8
    FireFlyNode* _shockwaveEffect;     // 0x100
    int _attackCounterMax;             // 0x104
    char field_108[0x114 - 0x108];     // 0x108
};
DECOMP_SIZE_ASSERT(MrsGMain, 0x114);

// FUNCTION: USA 0x00063F41
// PROTOTYPE: PREVIEW 0x004504D8 identical
// NAME: proto-pdb:default.pdb
void MrsGMain::KillScream()
{
    _screamTimer = 0.0f;
    _screamWillHurt = false;
    if (_shockwaveEffect) {
        _shockwaveEffect->Pause();
        _shockwaveEffect->SetOption(SwampSceneNode::DontDraw, true, false);
    }
}

// FUNCTION: USA 0x00063F71
// PROTOTYPE: PREVIEW 0x00450508 identical
// NAME: proto-pdb:default.pdb
void MrsGMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_DAMAGE:
        Message(this, 0, 0); // message 0: name unknown
        break;
    case TIMER_ATTACK:
        if (_state == STATE_IDLE || _state == STATE_INITIAL_IDLE) {
            if (_attackCounter != 0) {
                SetState(STATE_SCREAM);
                _attackCounter--;
            } else {
                SetState(STATE_STOMP);
                _attackCounter = _attackCounterMax;
            }
        }
        break;
    }
}

// FUNCTION: USA 0x000641A6
// PROTOTYPE: PREVIEW 0x0045073D identical
// NAME: proto-pdb:default.pdb
SwampScript* ExitRampMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 348) ExitRampMain(node);
}

// FUNCTION: USA 0x000641D0
// PROTOTYPE: PREVIEW 0x00450767 identical
// NAME: proto-pdb:default.pdb
SwampScript* EntranceRampMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 379) EntranceRampMain(node);
}

// FUNCTION: USA 0x00067C15
// PROTOTYPE: PREVIEW 0x004541CC identical
// NAME: proto-pdb:default.pdb
SwampScript* FloorHoleMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 287) FloorHoleMain(node);
}

// FUNCTION: USA 0x00067EDE
// PROTOTYPE: PREVIEW 0x00454495 identical
// NAME: proto-pdb:default.pdb
SwampScript* MrsGMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 210) MrsGMain(node);
}

class Critter; // NAME: proto-pdb:default.pdb
class AnimationControl; // NAME: proto-pdb:default.pdb

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~CarriedObjectMain();

    char field_BC[0xF8 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

// FUNCTION: USA 0x00064085
// NAME: proto-pdb:default.pdb
class WeaponMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> _animAttack; // 0xF8
    SwampString _CarriedObjectClass;   // 0xFC
    SwampString _effectName;           // 0x118
    char field_134[0x144 - 0x134];     // 0x134
    std::vector<Critter*> _affectedCritters; // 0x144
    char field_154[0x160 - 0x154];     // 0x154
    SwampString _weaponClass;          // 0x160
    float _damageMultiplier;           // 0x17C
};
DECOMP_SIZE_ASSERT(WeaponMain, 0x180);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1WeaponMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WeaponMain_dtor(WeaponMain* p)
{
    p->WeaponMain::~WeaponMain();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1MrsGMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MrsGMain_dtor(MrsGMain* p)
{
    p->MrsGMain::~MrsGMain();
}

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_ITEM = 0x1
};

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool, bool);
    void OverrideTexture(const SwampString& name, const SwampString& newName);

    char field_0[0xC2];                // 0x00
    bool _useAllLights;                // 0xC2
    char field_C3[0x200 - 0xC3];       // 0xC3
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

// FUNCTION: USA 0x00066A90
// PROTOTYPE: PREVIEW 0x0045301C identical
// NAME: proto-pdb:default.pdb
void FloorHoleMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_FALL:
        SoundManager::PlaySound("ccrr_ffall", 0, _node, true, 0.0f, 0.0f);
        ((GatorNode*)_node)->StartAnimation(_animFall, false, false);
        break;
    }
}

struct Vector4 { // NAME: proto-pdb:default.pdb
    operator Vector3() const { return *(const Vector3*)this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _Object != 0; }
    const T& operator*() const { return *_Object; }

    T* _Object;                        // 0x00
};

class BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    const Vector4& ImpactPlane() const { return _plane; }

    char field_0[0x10];                // 0x00
    Vector4 _plane;                    // 0x10
    char field_20[0x40 - 0x20];        // 0x20
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};

class RodeoRatMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_NULL = 0,
        STATE_ATTACK = 1,
        STATE_DAMAGE_BARRIER = 2
    };
    virtual void CollidedWith(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr);

    char field_BC[0x128 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(RodeoRatMain, 0x128);

#ifdef NON_MATCHING
// best: match_one objdiff 98.05, bytes mismatch
// FUNCTION: USA 0x00063D47
// PROTOTYPE: PREVIEW 0x004502DE identical
// NAME: proto-pdb:default.pdb
void RodeoRatMain::CollidedWith(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr)
{
    if (cr.IsValid() && type == HorizontalCollision) {
        Vector3 normal = (*cr).ImpactPlane();
        const Vector3& facing = _node->GetFacing();
        if (normal.x * facing.x + normal.y * facing.y + normal.z * facing.z < -0.5f)
            SetState(STATE_DAMAGE_BARRIER);
    }
}
#endif

// Retail layout: Critter _CritterType 0x258 -> 0x268 (+0x10 above _Velocity 0x20C).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0xC];               // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x268 - 0x218];     // 0x218
    CritterType _CritterType;          // 0x268
};

class RockChunkMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    RockChunkMain(SwampSceneNode* node);
    virtual ~RockChunkMain();

private:
    virtual void SetState(int);

    static int _chunkCount; // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(RockChunkMain, 0xBC);

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); not finished: vtable and _chunkCount pins site-corr
// FUNCTION: USA 0x000640D5
// PROTOTYPE: PREVIEW 0x0045066C near-identical
// NAME: proto-pdb:default.pdb
RockChunkMain::RockChunkMain(SwampSceneNode* node) : CritterMain(node)
{
    ((Critter*)_node)->_Velocity = Vector3(0.0f, -1.0f, 0.0f);
    ((Critter*)_node)->_CritterType = CRITTER_TYPE_ITEM;
    SetState(0);
    RegisterFrameTick();
    _chunkCount++;
}
#endif

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    BasicDoorMain(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _animOpen; // 0xBC
    char field_C0[0x110 - 0xC0];       // 0xC0
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class MrsGCollapsedMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    MrsGCollapsedMain(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(MrsGCollapsedMain, 0x110);

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 3 = vptr store, 2 literals)
// FUNCTION: USA 0x00068019
// PROTOTYPE: PREVIEW 0x004545D0 near-identical
// NAME: proto-pdb:default.pdb
MrsGCollapsedMain::MrsGCollapsedMain(SwampSceneNode* node) : BasicDoorMain(node)
{
    _animOpen = g_swamp->GetAnimationManager()->Load("mrsgcollapsed_collapse");
    ((Critter*)_node)->_useAllLights = true;
    ((Critter*)_node)->OverrideTexture("mrsg", "mrsgDamage02");
}
#endif
