// UNIT: src/Apps/Vince/scripts/Bayou/BogWallow.cpp OBJ: Xbox\OfficialRelease\BogWallow.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    int compare(const E* _Ptr) const;

    static const unsigned int npos;

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char field_0[0x1C];                // 0x00
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
template <class E, class T, class A> const unsigned int basic_string<E, T, A>::npos = (unsigned int)(-1);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    int _RefCount;                     // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int HitTag() const { return _tag; }

    unsigned int _tag;                 // 0x04
    char field_8[0x40 - 0x8];          // 0x08
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    const T* operator->() const { return _Object; }

    T* _Object;                        // 0x00
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x10];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }

    T* _Resource;                      // 0x00
};

struct Matrix;               // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
    const SwampString& GetName() const { return _Name; }
    Vector3 GetFacing() const;
    void InvalidateChildren();

    char field_4[0x8 - 0x4];           // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
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
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    bool IsStunned() const;
    void PlaySound(const SwampString& name, unsigned int flags, bool bAttach, float minDist3D, float maxDist3D);

    float _MaxSpeed;                   // 0x200
    float _CurSpeed;                   // 0x204
    float _stunTimer;                  // 0x208
};

// FUNCTION: USA 0x0002BFC9
// PROTOTYPE: PREVIEW 0x0041AA67 identical
// NAME: proto-pdb:default.pdb
bool Critter::IsStunned() const
{
    return _stunTimer > 0.0f;
}

class Crawdaddy : public Critter { // NAME: proto-pdb:default.pdb
public:
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode* jorgeNode, const Vector3& delta) const;
};

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
    SwampSceneNode* GetNode() { return _node; }
    void RegisterFrameTick();
    void RegisterTimerEvent(unsigned int timerId, float time, bool repeat);

protected:
    SwampScript(SwampSceneNode* node); // retail ??0SwampScript@@IAE@PAVSwampSceneNode@@@Z

public:
    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CrawdaddyHintRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CrawdaddyHintRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);

    bool _trapUpHintDone;              // 0x4C
    bool _trapDownHintDone;            // 0x4D
};
DECOMP_SIZE_ASSERT(CrawdaddyHintRegionMain, 0x50);

// FUNCTION: USA 0x0002BFA9
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); precheck relink `our ranges overlap at 0x00025012`:
// the ctor emits ??_7CrawdaddyHintRegionMain, whose slot 0 ??_GCrawdaddyHintRegionMain@@UAEPAXI@Z folds at 0x00025012 and is not pinned there by name
CrawdaddyHintRegionMain::CrawdaddyHintRegionMain(SwampSceneNode* node) : SwampScript(node), _trapUpHintDone(false), _trapDownHintDone(false)
{
}
#endif

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    void SetWanderChaseTarget(const SwampString& targetName);

protected:
    virtual void SetState(int);                                    // vtable+0x6C

public:
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                      // 0x4C
    Vector3 _wanderPatrolCenter;       // 0x50
    float _wanderPatrolRadius;         // 0x5C
    float _wanderLeashRadius;          // 0x60
    char field_64[0x6C - 0x64];        // 0x64
    Vector3 _wanderDir;                // 0x6C
    char field_78[0x84 - 0x78];        // 0x78
    SwampString _wanderChaseTarget;    // 0x84
    float _wanderSpeed;                // 0xA0
    float _wanderTurnRate;             // 0xA4
    bool _wanderAvoidLedges;           // 0xA8
    char field_A9[0xBC - 0xA9];        // 0xA9
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum State { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0,
        STATE_OPEN = 1,
        STATE_OPENING = 2
    };
    virtual ~BasicDoorMain();
    virtual void AnimationFinished();
    virtual void Message(SwampScript*, int, void*);

    char field_BC[0x110 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

// FUNCTION: USA 0x0002D983
// PROTOTYPE: PREVIEW 0x0041C3CB identical
// NAME: proto-pdb:default.pdb
class CrawdadTrapMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_TRAP_OPENING = 5,
        STATE_TRAP_OPEN = 6,
        STATE_TRAP_CLOSING = 7,
        STATE_TRAP_CLOSED = 8
    };
    enum TimerEvents { // NAME: proto-pdb:default.pdb
        TIMER_CAPTURED = 0
    };
    virtual void AnimationFinished();
    virtual void HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void SetState(int);

    ResourceHandle<AnimationResource> _animTrap; // 0x110
};
DECOMP_SIZE_ASSERT(CrawdadTrapMain, 0x114);

// FUNCTION: USA 0x0002C0B4
// PROTOTYPE: PREVIEW 0x0041AB61 identical
// NAME: proto-pdb:default.pdb
void CrawdadTrapMain::Message(SwampScript* source, int message, void* data)
{
    BasicDoorMain::Message(source, message, data);
    if (message == 6 && _state != STATE_CLOSED)
        _node->InvalidateChildren();
}

// FUNCTION: USA 0x0002C173
// PROTOTYPE: PREVIEW 0x0041AC20 identical
// NAME: proto-pdb:default.pdb
bool Crawdaddy::CanJorgeCollideWithMe(const SwampSceneNode* jorgeNode, const Vector3& delta) const
{
    if (jorgeNode->GetName() == "shrimpsub")
        return true;
    return Critter::CanJorgeCollideWithMe(jorgeNode, delta);
}

// FUNCTION: USA 0x0002C1A1
// PROTOTYPE: PREVIEW 0x0041AC4E identical
// NAME: proto-pdb:default.pdb
void CrawdadTrapMain::HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr)
{
    // !FAKE: c1xx front end; cast; retail compares the tag's low byte (cmp byte ptr [eax+4],1), the PDB gives HitTag unsigned int (match_one 99.77 -> 100.0)
    if (critterHitBy && critterHitBy->GetName() == "shrimpsub" && _state == STATE_OPEN && (unsigned char)cr->HitTag() == 1)
        SetState(STATE_TRAP_OPENING);
}

class CrawdaddyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_IN_TRAP = 0x14
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_PATROL = 0,
        STATE_CHASE = 1,
        STATE_ATTACK = 2,
        STATE_TRAPPED = 3
    };
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
    virtual void SetState(int);

    char field_BC[0x10C - 0xBC];       // 0xBC
    float _speed;                      // 0x10C
    bool _trapUpHintDone;              // 0x110
    bool _trapDownHintDone;            // 0x111
};
DECOMP_SIZE_ASSERT(CrawdaddyMain, 0x114);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString& name, unsigned int flags, SwampSceneNode* parent, bool bAttach,
                          float minDist3D, float maxDist3D);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
}

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0002C1DC
// PROTOTYPE: PREVIEW 0x0041AC89 identical
// NAME: proto-pdb:default.pdb
void CrawdaddyMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    // !FAKE: c2 block layout; stand-in locals; node load and format call before the arg pushes, the PDB lists none (match_one 49.84 -> 94.81);
    // v0 points at the format temporary, assigned inside the call's first argument: format runs before the float pushes and
    // its eax is passed (a named string local re-leas its slot, 94.81)
    if (name == "tail") {
        Critter* p = GetCritter();
        const SwampString* v0;
        p->PlaySound(*(v0 = &(const SwampString&)format("crawtail%02d", BeepLib::Random() % 2 + 1)), 0x28, true, 0.0f, 0.0f);
    } else if (name == "pincer") {
        SwampSceneNode* p = GetNode();
        const SwampString* v0;
        SoundManager::PlaySound(*(v0 = &(const SwampString&)format("crawpince%02d", BeepLib::Random() % 3 + 1)), 0x20, p, true, 0.0f, 0.0f);
    } else if (name == "swim") {
        Critter* p = GetCritter();
        const SwampString* v0;
        p->PlaySound(*(v0 = &(const SwampString&)format("crawdist%02d", BeepLib::Random() % 4 + 1)), 0x28, true, 0.0f, 0.0f);
    }
}

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Return(float);
};

// FUNCTION: USA 0x0002CF6E
// PROTOTYPE: PREVIEW 0x0041B997 identical
// NAME: proto-pdb:default.pdb
void CrawdadTrapMain::AnimationFinished()
{
    switch (_state) {
    case STATE_OPENING:
        AnimCam::Return(0.0f);
        break;
    case STATE_TRAP_OPENING:
        SetState(STATE_TRAP_OPEN);
        return;
    case STATE_TRAP_CLOSING:
        SetState(STATE_TRAP_CLOSED);
        GetCritter()->StartAnimation(g_swamp->GetAnimationManager().Load("crawdadTrap_shake"), false, false);
        RegisterTimerEvent(TIMER_CAPTURED, 2.0f, false);
        return;
    }
    BasicDoorMain::AnimationFinished();
}

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    LevelScript(SwampSceneNode* node, bool isZone);
    virtual void EnteringRegion(SwampSceneNode*);

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_50[0x64 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class TheBogWallowRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    TheBogWallowRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(TheBogWallowRegionMain, 0x64);

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    void ActivateMatching(const SwampString& s, unsigned int zone);
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0002D107
// PROTOTYPE: PREVIEW 0x0041BB30 identical
// NAME: proto-pdb:default.pdb
void TheBogWallowRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (!_inRegion) {
        LevelScript::EnteringRegion(node);
        pManager->ActivateMatching("mondomole", 0);
    }
}

// FUNCTION: USA 0x0002D331
// PROTOTYPE: PREVIEW 0x0041BD5A identical
// NAME: proto-pdb:default.pdb
void CritterMain::SetWanderChaseTarget(const SwampString& targetName)
{
    _wanderChaseTarget = targetName;
}

class Level { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() { return _script; }

    char field_0[0xC];                 // 0x00
    SwampScript* _script;              // 0x0C
};

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }

    char field_0[0x4];                 // 0x00
    Level* _curLevel;                  // 0x04
};

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetCrawdaddyLeashRadius() const { return _crawdaddyLeashRadius; }
    float GetCrawdaddyPatrolRadius() const { return _crawdaddyPatrolRadius; }
    float GetCrawdaddyTurnRate() const { return _crawdaddyTurnRate; }

    char field_0[0x85C];               // 0x00
    float _crawdaddyLeashRadius;       // 0x85C
    float _crawdaddyPatrolRadius;      // 0x860
    float _crawdaddySpeed;             // 0x864
    float _crawdaddyTurnRate;          // 0x868
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0002D9BC
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 40.9, bytes provisional (masked 1: vptr store ??_7TheBogWallowRegionMain 0x001CD510 unnamed; code bytes equal)
TheBogWallowRegionMain::TheBogWallowRegionMain(SwampSceneNode* node)
    : LevelScript(node, true)
{
}
#endif

// FUNCTION: USA 0x0002DC91
// PROTOTYPE: PREVIEW 0x0041C6D9 identical
// NAME: proto-pdb:default.pdb
void CrawdaddyMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 2:
        ((Critter*)_node)->ReturnToHome();
        SetState(STATE_PATROL);
        break;
    case 6: {
        // !FAKE: c2 scheduling; stand-in local; goaway store lands after GetCurLevel, the PDB lists only goaway (match_one 92.86 -> 100.0)
        Level* p = LevelLoader::Instance().GetCurLevel();
        bool goaway = false;
        p->GetScript()->Message(this, 0x1b, &goaway);
        if (goaway) {
            _node->Invalidate();
        } else {
            ((Critter*)_node)->ReturnToHome();
            SetState(STATE_PATROL);
            RegisterFrameTick();
            _wanderLeashRadius = gSettings.GetCrawdaddyLeashRadius();
            _wanderPatrolCenter = _node->GetWorldPosition();
            _wanderPatrolRadius = gSettings.GetCrawdaddyPatrolRadius();
            _wanderSpeed = _speed;
            _wanderAvoidLedges = false;
            _wanderTurnRate = gSettings.GetCrawdaddyTurnRate();
            _wanderDir = _node->GetFacing();
        }
        break;
    }
    case 12:
    case 13:
        SetState(STATE_PATROL);
        break;
    case MESSAGE_IN_TRAP:
        SetState(STATE_TRAPPED);
        break;
    }
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1CrawdadTrapMain@@UAE@XZ because this unit defines
// the ctor (not matched); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_CrawdadTrapMain_dtor(CrawdadTrapMain* p)
{
    p->CrawdadTrapMain::~CrawdadTrapMain();
}
