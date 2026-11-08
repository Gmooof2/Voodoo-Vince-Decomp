// UNIT: src/Apps/Vince/scripts/french_quarter/GingerDM.cpp OBJ: Xbox\OfficialRelease\GingerDM.obj OPT: unknown
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

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }

    T* _Object;                        // 0x00
};
struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float Length() const;

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class AABox;      // NAME: proto-pdb:default.pdb
class Quaternion; // NAME: proto-pdb:default.pdb
struct Matrix;    // NAME: proto-pdb:default.pdb
class ModelResource { // NAME: proto-pdb:default.pdb
public:
    int GetBoneIndex(const SwampString&, bool) const;
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];               // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x10];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();

    char field_24[0x68 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }
    const T& operator()() const { return *_Resource; }

    T* _Resource;                      // 0x00
};

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    int _RefCount;                     // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    char field_4[0x40 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

class CritterCollision { // NAME: proto-pdb:default.pdb
public:
    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};
DECOMP_SIZE_ASSERT(CritterCollision, 0x8);
class SwampScript; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                         // vtable+0x00
    virtual const AABox& GetBounds() const;            // vtable+0x04
    virtual Vector3 GetWorldPosition() const;          // vtable+0x08
    virtual Quaternion GetWorldRotation() const;       // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);     // vtable+0x10
    virtual void Invalidate();                         // vtable+0x14
    virtual void UpdateAnimation(unsigned int);        // vtable+0x18
    virtual void Update();                             // vtable+0x1C
    virtual void Render(const Matrix&) const;          // vtable+0x20

    SwampScript* GetScript() const;
    Vector3 GetFacing() const;

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Invalidate();                                     // vtable+0x14
    void Stop();
};

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C

    char field_A4[0x8];                // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0x8];                // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    ResourceHandle<AnimationControl> TransitionAnimation(ResourceHandle<AnimationResource>, float, bool, bool);
    const ResourceHandle<ModelResource>& GetModel() const { return _model; }
    void SetPosBone(int bone) { _posBone = bone; }

    ResourceHandle<ModelResource> _model; // 0xB4
    char field_B8[0xD4 - 0xB8];        // 0xB8
    int _posBone;                      // 0xD4
    char field_D8[0x200 - 0xD8];       // 0xD8
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void SetVelocity(const Vector3& v) { _Velocity = v; }
    const Vector3& GetHomePosition() const { return _homePosition; }

    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    void MakeDead(bool);
    void PlaySound(const SwampString&, unsigned int, bool, float, float);
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C

    char field_200[0x20C - 0x200];     // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x234 - 0x218];     // 0x218
    Vector3 _homePosition;             // 0x234
    char field_240[0x2C8 - 0x240];     // 0x240
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

// FUNCTION: USA 0x0007A1C3
// NAME: proto-pdb:default.pdb
// ??_GGingerDeadMan (alias ??_EGingerDeadMan) unmatched, no draft; wall: a virtual ??_G is emitted only with the class vtable, i.e. with its ctor
// ??0GingerDeadMan 0x0007A185 (candidates fail: 2 provisional targets) (C0320); match_one: `not defined in GingerDM.obj` (5.3 T6)
class GingerDeadMan : public Critter { // NAME: proto-pdb:default.pdb
public:
    virtual bool DoesJorgeRepelMe(const SwampSceneNode* jorgeNode, bool bHorizontal) const;
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    bool CanBeAttacked() const;
};

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetGingerDMAttackRange() const { return _gingerDMAttackRange; }
    float GetGingerDMWanderRange() const { return _gingerDMWanderRange; }

    char field_0[0x428];               // 0x00
    float _gingerDMAttackRange;        // 0x428
    float _gingerDMWanderRange;        // 0x42C
    char field_430[0xA0C - 0x430];     // 0x430
};
DECOMP_SIZE_ASSERT(Globals, 0xA0C);

extern Globals gSettings; // NAME: proto-pdb:default.pdb

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
    static SoundNode* CreateSound(const SwampString&, unsigned int, SwampSceneNode*, bool);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
double RandomFloat(float, float);
}

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

namespace Particle { // NAME: proto-pdb:default.pdb
void MonsterDeath(const Vector3&); // NAME: proto-pdb:default.pdb
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
    void UnregisterTimerEvent(unsigned int);
    void RemoveTimerEvents();

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)_node; }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    CritterCollision MoveMe(float);
    void UpdateWander();
    enum WanderModes { // NAME: proto-pdb:default.pdb
        WANDER_NONE = 0x0,
        WANDER_PATROL = 0x1,
        WANDER_CHASE = 0x2,
        WANDER_GOTO = 0x3
    };
    void SetWanderMode(int mode) { _wanderMode = mode; }
    void SetWanderPatrolCenter(const Vector3& center) { _wanderPatrolCenter = center; }
    void SetWanderPatrolRadius(float radius) { _wanderPatrolRadius = radius; }
    void SetWanderSpeed(float speed) { _wanderSpeed = speed; }
    void SetWanderTurnRate(float rate) { _wanderTurnRate = rate; }

    int _state;                        // 0x4C
    Vector3 _wanderPatrolCenter;       // 0x50
    float _wanderPatrolRadius;         // 0x5C
    char field_60[0x68 - 0x60];        // 0x60 (_wanderLeashRadius, _wanderLeashMargin; not reached)
    int _wanderMode;                   // 0x68
    char field_6C[0xA0 - 0x6C];        // 0x6C (_wanderDir .. _wanderChaseTarget; not reached)
    float _wanderSpeed;                // 0xA0
    float _wanderTurnRate;             // 0xA4
    char field_A8[0xBC - 0xA8];        // 0xA8 (_wanderAvoidLedges .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~MonsterMain();                                        // retail ??1MonsterMain@@UAE@XZ 0x000195A1
    virtual void DeathFX();                                        // vtable+0x70
    void UpdateKnockBack();

    char field_BC[0x240 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

// FUNCTION: USA 0x0007B033
// NAME: proto-pdb:default.pdb
// ??_GGingerDeadManMain (alias ??_EGingerDeadManMain) unmatched, no draft; wall: a virtual ??_G is emitted only with the class vtable, i.e. with its ctor
// ??0GingerDeadManMain 0x0007AA42 (0x5F1, candidates fail: unresolved 0x1DE8A0) (C0320); match_one: `not defined in GingerDM.obj`
// FUNCTION: USA 0x0007B04F
// PROTOTYPE: PREVIEW 0x00467056 ambiguous
// NAME: proto-pdb:default.pdb
class GingerDeadManMain : public MonsterMain { // NAME: proto-pdb:default.pdb
private:
    GingerDeadManMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void FrameTick();
    virtual void AnimationFinished();
    virtual void TimerEvent(unsigned int timerid);
    void ResetGruntTimer();
    GingerDeadMan* GetGinger() { return (GingerDeadMan*)_node; }
    virtual void DeathFX();
    virtual void SetState(int state);

    enum State { // NAME: proto-pdb:default.pdb
        STATE_WAIT = 0x0,
        STATE_OVEN = 0x1,
        STATE_WALK = 0x2,
        STATE_WALK_REST = 0x3,
        STATE_DAMAGE = 0x4,
        STATE_DEATH_SPIN = 0x5,
        STATE_ATTACK = 0x6
    };
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_FINISHATTACK = 0x0,
        TIMER_REST = 0x1,
        TIMER_GRUNT = 0x2
    };

    ResourceHandle<AnimationResource> _oven;      // 0x240
    ResourceHandle<AnimationResource> _idle;      // 0x244
    ResourceHandle<AnimationResource> _walk;      // 0x248
    ResourceHandle<AnimationResource> _damage;    // 0x24C
    ResourceHandle<AnimationResource> _deathSpin; // 0x250
    ResourceHandle<AnimationResource> _attack;    // 0x254
    SoundNode* _loopSound;                        // 0x258
};
DECOMP_SIZE_ASSERT(GingerDeadManMain, 0x25C);

// FUNCTION: USA 0x0007A14A
// PROTOTYPE: PREVIEW 0x00466151 identical
// NAME: proto-pdb:default.pdb
void GingerDeadManMain::ResetGruntTimer()
{
    UnregisterTimerEvent(TIMER_GRUNT);
    RegisterTimerEvent(TIMER_GRUNT, (float)BeepLib::RandomFloat(5.0f, 10.0f), false);
}

// FUNCTION: USA 0x0007A1DF
// PROTOTYPE: PREVIEW 0x004661E6 identical
// NAME: proto-pdb:default.pdb
bool GingerDeadMan::DoesJorgeRepelMe(const SwampSceneNode* jorgeNode, bool bHorizontal) const
{
    if (bHorizontal && jorgeNode == pManager->GetVince()) {
        return true;
    }
    return Critter::DoesJorgeRepelMe(jorgeNode, bHorizontal);
}

// FUNCTION: USA 0x0007A228
// PROTOTYPE: PREVIEW 0x0046622F identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 84.36, bytes mismatch (wall: retail inlines Vector3::Length as __asm rsqrtss/fcompi/fcmovbe, D116; frame 0x38 vs 0x34)
void GingerDeadManMain::FrameTick()
{
    switch (_state) {
    case STATE_WALK:
    case STATE_WALK_REST:
    case STATE_ATTACK:
        UpdateWander();
        MoveMe(GetCritter()->GetGravity());
        break;
    case STATE_DAMAGE:
        UpdateKnockBack();
        MoveMe(GetCritter()->GetGravity());
        break;
    }
    if (_state == STATE_WALK) {
        // !FAKE: c2 register allocation; neutral stand-in local; retail keeps the Vince pointer in ebx
        Vince* p = pManager->GetVince();
        if (p->CanBeAttacked()) {
            const float attackRange = gSettings.GetGingerDMAttackRange();
            // !FAKE: inliner; neutral stand-in pointer locals; hand form of the inline Vector3 difference (C0191)
            const Vector3* v2 = &_node->GetWorldPosition();
            const Vector3* v3 = &p->GetWorldPosition();
            Vector3 tovince;
            tovince.x = v3->x - v2->x;
            tovince.y = v3->y - v2->y;
            tovince.z = v3->z - v2->z;
            Vector3 facing = _node->GetFacing();
            // !FAKE: c2 block layout; neutral stand-in local; one dot product for both tests
            float v1 = tovince.x * facing.x + tovince.y * facing.y + tovince.z * facing.z;
            if (v1 > 0.0f && v1 < attackRange && tovince.Length() < attackRange) {
                SetState(STATE_ATTACK);
            }
        }
    }
}
#endif

// FUNCTION: USA 0x0007A397
// PROTOTYPE: PREVIEW 0x0046639E identical
// NAME: proto-pdb:default.pdb
void GingerDeadManMain::TimerEvent(unsigned int timerid)
{
    switch (timerid) {
    case TIMER_GRUNT: {
        // !FAKE: c2 register allocation; neutral stand-in local; retail loads the node before the string (match_one 85.71 -> 100)
        GingerDeadMan* p = GetGinger();
        // !FAKE: c1xx front end; neutral stand-in local, comma expression; retail builds the string before the float arguments (match_one 67.33 -> 100)
        const SwampString* v1;
        p->PlaySound((v1 = &format("gdmgrowl%02d", (BeepLib::Random() & 1) + 1), *v1), 0x28, true, 0.0f, 0.0f);
        ResetGruntTimer();
        break;
    }
    case TIMER_REST:
        SetState(STATE_WALK);
        break;
    case TIMER_FINISHATTACK:
        SetState(STATE_WALK_REST);
        break;
    }
}

// FUNCTION: USA 0x0007A40C
// PROTOTYPE: PREVIEW 0x00466413 identical
// NAME: proto-pdb:default.pdb
void GingerDeadManMain::SetState(int state)
{
    int oldstate = _state;
    _state = state;
    if (_loopSound) {
        _loopSound->Stop();
        _loopSound->Invalidate();
        _loopSound = 0;
    }
    switch (_state) {
    case STATE_OVEN:
        GetGinger()->SetPosBone(GetGinger()->GetModel()().GetBoneIndex("body", false));
        GetCritter()->StartAnimation(_oven, false, false);
        break;
    case STATE_WALK_REST:
        RegisterTimerEvent(TIMER_REST, 0.5f, false);
    case STATE_WALK:
        ResetGruntTimer();
        GetGinger()->SetPosBone(0);
        if (oldstate != STATE_WALK_REST && oldstate != STATE_WALK) {
            GetCritter()->TransitionAnimation(_walk, 0.1f, true, false);
        }
        SetWanderMode(WANDER_PATROL);
        SetWanderTurnRate(4.0f);
        SetWanderSpeed(0.25f);
        SetWanderPatrolCenter(GetCritter()->GetHomePosition());
        SetWanderPatrolRadius(gSettings.GetGingerDMWanderRange());
        break;
    case STATE_DAMAGE:
        UnregisterTimerEvent(TIMER_GRUNT);
        GetCritter()->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
        GetCritter()->StartAnimation(_damage, false, false);
        SoundManager::PlaySound("gdmdamage", 0x20, _node, true, 0.0f, 0.0f);
        break;
    case STATE_DEATH_SPIN:
        GetCritter()->StartAnimation(_deathSpin, false, false);
        GetCritter()->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
        break;
    case STATE_ATTACK:
        SetWanderMode(WANDER_CHASE);
        RegisterTimerEvent(TIMER_FINISHATTACK, 0.75f, false);
        GetCritter()->TransitionAnimation(_attack, 0.1f, true, false);
        SetWanderSpeed(0.5f);
        _loopSound = SoundManager::CreateSound("gdmattack", 0xD, GetCritter(), true);
        break;
    }
}

// FUNCTION: USA 0x0007A689
// PROTOTYPE: PREVIEW 0x00466690 identical
// NAME: proto-pdb:default.pdb
void GingerDeadManMain::AnimationFinished()
{
    switch (_state) {
    case STATE_OVEN: {
        SetState(STATE_WALK);
        // !FAKE: c1xx front end; neutral stand-in local; retail destroys the temporary string before GetScript
        Critter* p = pManager->Find("oven");
        p->GetScript()->Message(this, 0x15, 0);
        break;
    }
    case STATE_DAMAGE:
        SetState(STATE_WALK);
        break;
    case STATE_DEATH_SPIN:
        RemoveTimerEvents();
        DeathFX();
        Message(this, 0x14, 0);
        GetCritter()->MakeDead(true);
        break;
    }
}

// FUNCTION: USA 0x0007A71A
// PROTOTYPE: PREVIEW 0x00466721 identical
// NAME: proto-pdb:default.pdb
void GingerDeadManMain::DeathFX()
{
    Particle::MonsterDeath(_node->GetWorldPosition());
    SoundManager::PlaySound("gdmdeath", 0, _node, false, 0.0f, 0.0f);
    SoundManager::PlaySound("monsterpoof", 0, _node, false, 0.0f, 0.0f);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1GingerDeadManMain@@UAE@XZ in this unit (its
// user, the ctor, is unmatched); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_GingerDeadManMain_dtor(GingerDeadManMain* p)
{
    p->GingerDeadManMain::~GingerDeadManMain();
}

// FUNCTION: USA 0x0007B0DE
// PROTOTYPE: PREVIEW 0x004670E5 identical
// NAME: proto-pdb:default.pdb
SwampScript* GingerDeadManMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\GingerDM.h", 31) GingerDeadManMain(node);
}
