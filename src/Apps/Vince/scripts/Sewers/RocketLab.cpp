// UNIT: src/Apps/Vince/scripts/Sewers/RocketLab.cpp OBJ: Xbox\OfficialRelease\RocketLab.obj OPT: /O1 /Oy
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
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

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

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

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
    void UnRegisterFrameTick();
    void RegisterTimerEvent(unsigned int, float, bool);

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
    virtual void Load(const BeepTDF&);

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum State { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0,
        STATE_OPEN = 1,
        STATE_OPENING = 2,
        STATE_CLOSING = 3,
        STATE_LOCKEDOPEN = 4,
        STATE_BASICDOOR_USER = 5
    };
    virtual void Activate();

    char field_BC[0x110 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class ShortcutHatchMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    ShortcutHatchMain(SwampSceneNode* node);
    virtual void Activate();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ShortcutHatchMain, 0x110);

class LiftMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_ACTIVATE = 0,
        TIMER_RETURN = 1
    };
    virtual void TimerEvent(unsigned int timerId);
private:
    bool IsVinceInTheWay();
};

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    void Load(const SwampString& name, const char* format, int start);

    char field_0[0x14];                // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

class FlamePipesMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF& tdf);

    char field_BC[0xC8 - 0xBC];        // 0xBC
    SwampString _animname;             // 0xC8
    MultiAnimation _multiAnim;         // 0xE4
};
DECOMP_SIZE_ASSERT(FlamePipesMain, 0xF8);

// FUNCTION: USA 0x0009D7AD
// PROTOTYPE: PREVIEW 0x00488827 identical
// NAME: proto-pdb:default.pdb
void FlamePipesMain::Load(const BeepTDF& tdf)
{
    CritterMain::Load(tdf);
    _multiAnim.Load(_animname, "%s%003i", 1);
}

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> _animCarry; // 0xBC
    char field_C0[0x18];               // 0xC0
    bool _persistentFrameTick;         // 0xD8
    char field_D9[0x1F];               // 0xD9
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

class HeavyObjectMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    HeavyObjectMain(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(HeavyObjectMain, 0xF8);

class GasCanisterMain : public HeavyObjectMain { // NAME: proto-pdb:default.pdb
public:
    GasCanisterMain(SwampSceneNode* node);

    bool _armed;                       // 0xF8
    bool field_F9;                     // 0xF9 retail only: zeroed by the retail ctor, absent in the proto PDB
    int _dropCount;                    // 0xFC
    int _flameCount;                   // 0x100
};
DECOMP_SIZE_ASSERT(GasCanisterMain, 0x104);

class FlameWallMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_ON = 0,
        STATE_OFF = 1
    };
    void SetState(int state);

    int _state;                        // 0x4C
};
DECOMP_SIZE_ASSERT(FlameWallMain, 0x50);

// FUNCTION: USA 0x0009D7D6
// PROTOTYPE: PREVIEW 0x00488850 identical
// NAME: proto-pdb:default.pdb
void FlameWallMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_ON:
        RegisterFrameTick();
        break;
    case STATE_OFF:
        UnRegisterFrameTick();
        break;
    }
}

// FUNCTION: USA 0x0009DB51
// PROTOTYPE: PREVIEW 0x00488C4C identical
// NAME: proto-pdb:default.pdb
void ShortcutHatchMain::Activate()
{
    BasicDoorMain::Activate();
    AnimCam::SwitchTo("camera_shortcuthatch", 0.0f, false, 0.0f);
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 3 = base ctor call, vptr store, literal)
// FUNCTION: USA 0x0009DCC3
// PROTOTYPE: PREVIEW 0x00488DBE near-identical
// NAME: proto-pdb:default.pdb
GasCanisterMain::GasCanisterMain(SwampSceneNode* node)
    : HeavyObjectMain(node), _armed(false), field_F9(false), _dropCount(0), _flameCount(0)
{
    _persistentFrameTick = true;
    _animCarry = g_swamp->GetAnimationManager()->Load("gasCanister_walk");
}
#endif

// FUNCTION: USA 0x0009E648
// PROTOTYPE: PREVIEW 0x00489723 identical
// NAME: proto-pdb:default.pdb
void LiftMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_ACTIVATE:
        Activate();
        break;
    case TIMER_RETURN:
        if (_state == STATE_OPEN) {
            if (IsVinceInTheWay()) {
                RegisterTimerEvent(TIMER_RETURN, 1.0f, false);
            } else {
                SetState(STATE_CLOSING);
            }
        }
        break;
    }
}

// FUNCTION: USA 0x0009ECD1
// PROTOTYPE: PREVIEW 0x00489D8D identical
// NAME: proto-pdb:default.pdb
SwampScript* ShortcutHatchMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("RocketLab\\RocketLab.cpp", 41) ShortcutHatchMain(node);
}
