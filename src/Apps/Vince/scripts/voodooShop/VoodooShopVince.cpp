// UNIT: src/Apps/Vince/scripts/voodooShop/VoodooShopVince.cpp OBJ: Xbox\OfficialRelease\VoodooShopVince.obj OPT: /O1 /Oy
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
class AnimationControl;      // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
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
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    T Fetch();

    char field_0[0x14];                // 0x00
};

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);

    char field_0[0x184];               // 0x00
    float _animSoundRange;             // 0x184
    float _animSoundVolume;            // 0x188
    unsigned int _animTriggerFlags;    // 0x18C
};
DECOMP_SIZE_ASSERT(GatorNode, 0x190);

// Retail layout: Critter _defaultIdleAnim 0x278 -> 0x288; Vince _idlesAny 0x4D8 -> 0x4EC.
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> GetDefaultIdleAnim() const { return _defaultIdleAnim; }

    char field_190[0x288 - 0x190];     // 0x190
    ResourceHandle<AnimationResource> _defaultIdleAnim; // 0x288
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    char field_28C[0x4EC - 0x28C];     // 0x28C
    SwampShuffleQueue<ResourceHandle<AnimationResource> > _idlesAny; // 0x4EC
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
}

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x668];               // 0x00
    int _voodooShopAmbientIdleMin;     // 0x668
    int _voodooShopAmbientIdleMax;     // 0x66C
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
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
protected:
    virtual void SetState(int);                                    // vtable+0x6C
public:
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class VoodooShopVinceMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    VoodooShopVinceMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    int _defaultIdleCount;             // 0xBC
};
DECOMP_SIZE_ASSERT(VoodooShopVinceMain, 0xC0);

#ifdef NON_MATCHING
// best: match_one objdiff 96.27, bytes mismatch (masked 0)
// T15.c6: retail loads lo (eax) and hi (edi) before push ebx, then mov ebx,eax and spills lo to [ebp-4] (the freed handle slot); 18 probe forms (no locals, order, int, split +=, refs, outer scope) keep 46 normdiff
// FUNCTION: USA 0x0013F331
// PROTOTYPE: PREVIEW 0x00527C0D near-identical
// NAME: proto-pdb:default.pdb
void VoodooShopVinceMain::AnimationFinished()
{
    if (--_defaultIdleCount < 0) {
        GetCritter()->StartAnimation(GetCritter()->GetDefaultIdleAnim(), false, false);
        unsigned int lo = gSettings._voodooShopAmbientIdleMin;
        unsigned int hi = gSettings._voodooShopAmbientIdleMax;
        _defaultIdleCount = BeepLib::Random() % (hi - lo) + lo;
    } else if (_defaultIdleCount == 0) {
        GetCritter()->StartAnimation(((Vince*)GetCritter())->_idlesAny.Fetch(), false, false);
    } else {
        GetCritter()->StartAnimation(GetCritter()->GetDefaultIdleAnim(), false, false);
    }
}
#endif

// FUNCTION: USA 0x0013F3E8
// NAME: proto-pdb:default.pdb
VoodooShopVinceMain::VoodooShopVinceMain(SwampSceneNode* node)
    : CritterMain(node), _defaultIdleCount(0)
{
    AnimationFinished();
    GetCritter()->_animTriggerFlags = 8;
    GetCritter()->_animSoundRange = 1.0f;
}

// FUNCTION: USA 0x0013F426
// PROTOTYPE: PREVIEW 0x00527D02 identical
// NAME: proto-pdb:default.pdb
SwampScript* VoodooShopVinceMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("VoodooShopVince.cpp", 12) VoodooShopVinceMain(node);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};


// FUNCTION: USA 0x0013F44D
// NAME: proto-pdb:default.pdb
VoodooShopVinceMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VoodooShopVinceMain", creationFunc);
}
