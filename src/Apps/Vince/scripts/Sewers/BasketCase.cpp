// UNIT: src/Apps/Vince/scripts/Sewers/BasketCase.cpp OBJ: Xbox\OfficialRelease\BasketCase.obj OPT: /O1 /Oy
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
    unsigned int _pad00[7];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

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
    ResourceHandle() : _Resource(0) {}
    ResourceHandle& operator=(const ResourceHandle& h);
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class AnimationControl; // NAME: proto-pdb:default.pdb

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

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xA4];                // 0x00
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetCollidable(bool collidable) { _collidable = collidable; }

    char field_A4[0x4];                // 0xA4
    bool _collidable;                  // 0xA8
    char field_A9[0x3];                // 0xA9
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0x8];                // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    char field_B4[0x14C];              // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0xC8];              // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

// Retail layout: Vince _whammyReady 0x434 -> 0x448 (+0x14).
class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetWhammyReady(bool);

    char field_2C8[0x448 - 0x2C8];     // 0x2C8
    bool _whammyReady;                 // 0x448
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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    Critter* GetCritter() { return (Critter*)GetNode(); }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class VinceLaundryCar : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_RIDING = 1,
        STATE_DYING = 2,
        STATE_DEAD = 3,
        STATE_FINISHED = 4,
        STATE_MENU = 5
    };
    VinceLaundryCar(SwampSceneNode* node);
    virtual void AnimationFinished();
private:
    virtual void SetState(int);
    static bool _tryagainmenu;
public:
    Critter* _laundryCar;              // 0xBC
    int _resetcam;                     // 0xC0
    bool _reachedBottom;               // 0xC4
    bool _success;                     // 0xC5
    bool _wasWhammyReady;              // 0xC6
    ResourceHandle<AnimationResource> _animRide;  // 0xC8
    ResourceHandle<AnimationResource> _animCrash; // 0xCC
};
DECOMP_SIZE_ASSERT(VinceLaundryCar, 0xD0);

class LaundryCarMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    LaundryCarMain(SwampSceneNode* node);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    char field_BC[0x4C];               // 0xBC
    ResourceHandle<AnimationResource> _animRide;        // 0x108
    ResourceHandle<AnimationResource> _animJump;        // 0x10C
    ResourceHandle<AnimationResource> _animCornerRight; // 0x110
    ResourceHandle<AnimationResource> _animCornerLeft;  // 0x114
    char field_118[0x8];               // 0x118
};
DECOMP_SIZE_ASSERT(LaundryCarMain, 0x120);

class BasketStopperMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void Activate();
    virtual void AnimationFinished();
    GatorNode* GetStopper() { return (GatorNode*)GetNode(); }

    ResourceHandle<AnimationResource> _idle; // 0x4C
    ResourceHandle<AnimationResource> _hit;  // 0x50
};
DECOMP_SIZE_ASSERT(BasketStopperMain, 0x54);

void* operator new(unsigned int size, const char* file, int line);

class BasketStopperPanMain : public BasketStopperMain { // NAME: proto-pdb:default.pdb
public:
    BasketStopperPanMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BasketStopperPanMain, 0x54);

// FUNCTION: USA 0x000A1520
// NAME: proto-pdb:default.pdb
void BasketStopperMain::Activate()
{
    GetStopper()->StartAnimation(_hit, false, false);
    GetStopper()->SetCollidable(false);
}

// FUNCTION: USA 0x000A153F
// PROTOTYPE: PREVIEW 0x0048C5EE identical
// NAME: proto-pdb:default.pdb
void BasketStopperMain::AnimationFinished()
{
    GetStopper()->StartAnimation(_idle, true, false);
    GetStopper()->SetCollidable(true);
}

// FUNCTION: USA 0x000A155E
// NAME: proto-pdb:default.pdb
void LaundryCarMain::AnimationFinished()
{
    GetCritter()->StartAnimation(_animRide, true, false);
}

// FUNCTION: USA 0x000A1572
// PROTOTYPE: PREVIEW 0x0048C621 identical
// NAME: proto-pdb:default.pdb
void VinceLaundryCar::AnimationFinished()
{
    if (_state == STATE_DYING)
        SetState(STATE_DEAD);
}

// FUNCTION: USA 0x000A2095
// PROTOTYPE: PREVIEW 0x0048D18E identical
// NAME: proto-pdb:default.pdb
SwampScript* BasketStopperPanMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BasketCase\\BasketCase.cpp", 205) BasketStopperPanMain(node);
}

// FUNCTION: USA 0x000A22F9
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1LaundryCarMain@@UAE@XZ because this unit defines
// the ctor 0xA20BC (not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LaundryCarMain_dtor(LaundryCarMain* p)
{
    p->LaundryCarMain::~LaundryCarMain();
}

// FUNCTION: USA 0x000A2691
// PROTOTYPE: PREVIEW 0x0048D77E identical
// NAME: proto-pdb:default.pdb
SwampScript* LaundryCarMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\basketcase\\BasketCase.h", 16) LaundryCarMain(node);
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 4)
// FUNCTION: USA 0x000A26B8
// PROTOTYPE: PREVIEW 0x0048D7A5 near-identical
// NAME: proto-pdb:default.pdb
VinceLaundryCar::VinceLaundryCar(SwampSceneNode* node)
    : CritterMain(node), _laundryCar(0), _resetcam(2), _reachedBottom(false), _success(false), _wasWhammyReady(false)
{
    AnimationManager& mgr = g_swamp->GetAnimationManager();
    _animRide = mgr.Load("vince_idle01");
    _animCrash = mgr.Load("vince_laundryCarFall");
    _tryagainmenu = false;
    _wasWhammyReady = ((Vince*)_node)->_whammyReady;
    ((Vince*)_node)->SetWhammyReady(false);
    SetState(STATE_IDLE);
}
#endif
