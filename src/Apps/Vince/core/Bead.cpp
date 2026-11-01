// UNIT: src/Apps/Vince/core/Bead.cpp OBJ: core\XBox\OfficialRelease\Bead.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

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
class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { UseAlpha = 0x8 };
    enum NodeRenderingSettings { Alpha = 0x0 }; // NAME: proto-pdb:default.pdb
    void SetOption(unsigned int option, bool on, bool recurse);
    void SetRenderingSetting(unsigned int setting, unsigned int value, bool recurse);

    char field_0[0xA4];                // 0x00
};

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

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x164];               // 0x00
    float _miscBeadLifetime;           // 0x164
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

class CollidableCylinder : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetRadius(float);
    void SetHeight(float);

    char field_A4[0xB4 - 0xA4];        // 0xA4
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_DUST = 0x10
};

// Retail layout: Critter _wallThreshold 0x254 -> 0x264, _CritterType 0x258 -> 0x268 (+0x10).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0x264 - 0x200];     // 0x200
    float _wallThreshold;              // 0x264
    CritterType _CritterType;          // 0x268
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

    void RegisterFrameTick();

    char field_4[0x1C];               // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    Critter* GetCritter() { return (Critter*)_node; }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class BeadMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_GETBEADSIZE = 0x14
    };
    BeadMain(SwampSceneNode* node);
    virtual void Message(SwampScript* from, int message, void* data);

protected:
    void SetupVelocity();
    void Blink();

public:
    float _lifeRemaining;              // 0xBC
    float _nextBlinkTime;              // 0xC0
    float _blinkElapsedTime;           // 0xC4
    bool _blinkOn;                     // 0xC8
    bool _gettingSuckedDown;           // 0xC9
    unsigned char _blinkFrames;        // 0xCA
    int _size;                         // 0xCC
};
DECOMP_SIZE_ASSERT(BeadMain, 0xD0);

// FUNCTION: USA 0x001260C4
// PROTOTYPE: PREVIEW 0x0050EBD2 identical
// NAME: proto-pdb:default.pdb
void BeadMain::Message(SwampScript* from, int message, void* data)
{
    if (message == MESSAGE_GETBEADSIZE)
        *(int*)data = _size;
}

// FUNCTION: USA 0x00126201
// NAME: proto-pdb:default.pdb
void BeadMain::Blink()
{
    _node->SetOption(SwampSceneNode::UseAlpha, !_blinkOn, true);
    _blinkFrames = 0;
    _blinkOn = !_blinkOn;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 3)
// FUNCTION: USA 0x00126256
// PROTOTYPE: PREVIEW 0x0050ED64 near-identical
// NAME: proto-pdb:default.pdb
BeadMain::BeadMain(SwampSceneNode* node)
    : CritterMain(node), _blinkOn(false), _gettingSuckedDown(false), _blinkFrames(0), _size(0)
{
    // !FAKE: c2 register allocation; local; retail holds _node in edi across the calls; the proto PDB lists no such local
    Critter* critter = GetCritter();
    critter->_CritterType = CRITTER_TYPE_DUST;
    critter->SetRadius(0.055f);
    critter->SetHeight(0.25f);
    critter->_wallThreshold = 0.174532935f;
    _lifeRemaining = gSettings._miscBeadLifetime;
    _nextBlinkTime = _lifeRemaining < 3.0f ? _lifeRemaining : 3.0f;
    _blinkElapsedTime = 0.25f;
    RegisterFrameTick();
    critter->StartAnimation(g_swamp->GetAnimationManager().Load("bead_idle"), true, false);
    critter->SetRenderingSetting(SwampSceneNode::Alpha, 0x3F, false);
    SetupVelocity();
}
#endif

class Bead2Main : public BeadMain { // NAME: proto-pdb:default.pdb
public:
    Bead2Main(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Bead2Main, 0xD0);

// FUNCTION: USA 0x001266BB
// PROTOTYPE: PREVIEW 0x0050F070 identical
// NAME: proto-pdb:default.pdb
SwampScript* Bead2Main::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\Bead.h", 87) Bead2Main(node);
}
