// UNIT: src/Apps/Vince/scripts/Crypt_city/RatRaceRodeoCutscenes.cpp OBJ: Xbox\OfficialRelease\RatRaceRodeoCutscenes.obj OPT: /O1 /Oy
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
struct Vector3;              // NAME: proto-pdb:default.pdb
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
    SwampScript* GetScript() const;
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

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

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

class RatRacePreludeCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    RatRacePreludeCutscene(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);                              // vtable+0x74

    Vince* _vince;                     // 0x78
    Critter* _rat;                     // 0x7C
    Critter* _mrsg;                    // 0x80
    GatorNode* _hat;                   // 0x84
    GatorNode* _kosmo;                 // 0x88
    char field_8C[0x90 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(RatRacePreludeCutscene, 0x90);

class RatRaceDeathCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    RatRaceDeathCutscene(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);                              // vtable+0x74

    Vince* _vince;                     // 0x78
    Critter* _rat;                     // 0x7C
    Critter* _mrsg;                    // 0x80
    GatorNode* _hat;                   // 0x84
    char field_88[0x8C - 0x88];        // 0x88
};
DECOMP_SIZE_ASSERT(RatRaceDeathCutscene, 0x8C);

// FUNCTION: USA 0x00068C05
// PROTOTYPE: PREVIEW 0x00455165 identical
// NAME: proto-pdb:default.pdb
void RatRacePreludeCutscene::SetState(int state)
{
    switch (state) {
    case 0:
        NewCamera("camera_rrp01");
        _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_ratRacePrelude01"), false, false);
        _rat->StartAnimation(g_swamp->GetAnimationManager().Load("rodeoRat_ratRacePrelude01"), false, false);
        _hat->StartAnimation(g_swamp->GetAnimationManager().Load("cowboyHat_ratRacePrelude01"), false, false);
        _kosmo->StartAnimation(g_swamp->GetAnimationManager().Load("kosmoHead_RRRodeoIntro"), false, false);
        break;
    case 1:
        NewCamera("camera_rrp03");
        _mrsg->StartAnimation(g_swamp->GetAnimationManager().Load("mrsG_comeToLife"), false, false);
        break;
    case 2:
        NewCamera("camera_rrp04");
        _kosmo->SetOption(2, true, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x00069208
// PROTOTYPE: PREVIEW 0x00455738 identical
// NAME: proto-pdb:default.pdb
void RatRaceDeathCutscene::SetState(int state)
{
    switch (state) {
    case 0:
        NewCamera("camera_death01");
        _mrsg->GetScript()->Message(this, 0x19, 0);
        break;
    case 1:
        NewCamera("camera_death02");
        _vince->SetOption(2, false, false);
        _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_ratRace_celebration"), false, false);
        _rat->SetOption(2, false, false);
        _rat->StartAnimation(g_swamp->GetAnimationManager().Load("rodeoRat_ratRace_celebration"), false, false);
        _hat->SetOption(2, false, false);
        _hat->StartAnimation(g_swamp->GetAnimationManager().Load("cowboyHat_ratRace_celebration"), false, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x000693F4
// PROTOTYPE: PREVIEW 0x00455924 identical
// NAME: proto-pdb:default.pdb
SwampScript* RatRacePreludeCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("RatRaceRodeo\\RatRaceRodeoCutscenes.cpp", 19) RatRacePreludeCutscene(node);
}

// FUNCTION: USA 0x0006941B
// PROTOTYPE: PREVIEW 0x0045594B identical
// NAME: proto-pdb:default.pdb
SwampScript* RatRaceDeathCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("RatRaceRodeo\\RatRaceRodeoCutscenes.cpp", 191) RatRaceDeathCutscene(node);
}
