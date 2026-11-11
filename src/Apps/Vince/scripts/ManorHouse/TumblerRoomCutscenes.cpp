// UNIT: src/Apps/Vince/scripts/ManorHouse/TumblerRoomCutscenes.cpp OBJ: Xbox\OfficialRelease\TumblerRoomCutscenes.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string { // NAME: proto-pdb:default.pdb
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

    char field_0[0x1C];                // 0x00
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
DECOMP_SIZE_ASSERT(SwampString, 0x1C);

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb
class AnimCam;               // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

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

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<AnimationResource>, 0x4);

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x68 - 0x1C];        // 0x1C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetOption(unsigned int, bool, bool);
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
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

    void RegisterTimerEvent(unsigned int, float, bool);

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished(unsigned int);                  // vtable+0x64
    virtual void AnimationTrigger(unsigned int, const SwampString&, float, float, unsigned int); // vtable+0x68
    virtual void Init();                                           // vtable+0x6C
    virtual void Cleanup();                                        // vtable+0x70
    virtual void SetState(int);                                    // vtable+0x74
protected:
    AnimCam* NewCamera(const SwampString&);

    char field_4C[0x54 - 0x4C];        // 0x4C
    int _state;                        // 0x54
    char field_58[0x78 - 0x58];        // 0x58
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

class KosmoAppearsTumblerCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    KosmoAppearsTumblerCutscene(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
private:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_KOSMOFADE = 0
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_V_WALKSIN = 0,
        STATE_K_APPEARS = 1,
        STATE_K_OTS = 2,
        STATE_K_CONTINUES = 3,
        STATE_LAST = 4
    };
    virtual void SetState(int state);                              // vtable+0x74

    Vince* _vince;                     // 0x78
    GatorNode* _kosmo;                 // 0x7C
    float _kosmoAlpha;                 // 0x80
    float _kosmoDeltaAlpha;            // 0x84
    bool _fadeKosmo;                   // 0x88
    bool _wasWhammyReady;              // 0x89
};
DECOMP_SIZE_ASSERT(KosmoAppearsTumblerCutscene, 0x8C);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

// FUNCTION: USA 0x0008EA51
// PROTOTYPE: PREVIEW 0x0047A110 identical
// NAME: proto-pdb:default.pdb
void KosmoAppearsTumblerCutscene::SetState(int state)
{
    switch (state) {
    case STATE_V_WALKSIN:
        NewCamera("camera_00");
        _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_walkstop_long"), false, false);
        break;
    case STATE_K_APPEARS:
        NewCamera("camera_01");
        _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_idle01"), true, false);
        _kosmo->StartAnimation(g_swamp->GetAnimationManager().Load("kosmoHead_tumblerRoomIntro"), false, false);
        _kosmo->SetOption(2, false, false);
        break;
    case STATE_K_OTS:
        NewCamera("camera_02");
        _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_idle04"), true, false);
        break;
    case STATE_K_CONTINUES:
        NewCamera("camera_03");
        RegisterTimerEvent(TIMER_KOSMOFADE, 4.9f, false);
        break;
    }
    _state = state;
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0008EC74
// NAME: proto-pdb:default.pdb
SwampScript* KosmoAppearsTumblerCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("tumblerRoom\\TumblerRoomCutscenes.cpp", 17) KosmoAppearsTumblerCutscene(node);
}

// FUNCTION: USA 0x0008EC9B
// NAME: proto-pdb:default.pdb
KosmoAppearsTumblerCutscene::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("KosmoAppearsTumblerCutscene", creationFunc);
}
