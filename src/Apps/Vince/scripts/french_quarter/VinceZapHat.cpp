// UNIT: src/Apps/Vince/scripts/french_quarter/VinceZapHat.cpp OBJ: Xbox\OfficialRelease\VinceZapHat.obj OPT: /O1 /Oy
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

class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
    static void Return(float);
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
};
class AnimationControl; // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
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

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool restart, bool loop);

    char field_0[0x200];               // 0x00
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void SetCurSpeed(float speed) { _CurSpeed = speed; }

    float _MaxSpeed;                   // 0x200
    float _CurSpeed;                   // 0x204
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

protected:
    SwampScript(SwampSceneNode* node);

public:
    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class VinceZapHat : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    VinceZapHat(SwampSceneNode* node);
    virtual ~VinceZapHat();
    virtual void AnimationFinished();
    Vince* GetVince() { return (Vince*)_node; }

    ResourceHandle<AnimationResource> _animSpell; // 0x4C
};
DECOMP_SIZE_ASSERT(VinceZapHat, 0x50);

// FUNCTION: USA 0x0008C37A
// PROTOTYPE: PREVIEW 0x00477E8E identical
// NAME: proto-pdb:default.pdb
void VinceZapHat::AnimationFinished()
{
    InvalidateScript();
    AnimCam::Return(0.0f);
}

// FUNCTION: USA 0x0008C38C
// PROTOTYPE: PREVIEW 0x00477EA0 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); wall (4.4 T4.c8 relink): vtable slot ??_G 0x001322E6 calls
// the dtor 0x0008C43B (folded ~VinceZapHat/~VinceCeilingFan), no name in symbols.txt/symbols_pin.txt:
// `external ??1VinceZapHat@@UAE@XZ does not resolve by name`; with VinceCeilingFan also live, `our ranges overlap at
// 0x001322E6` (??_GVinceZapHat and ??_GVinceCeilingFan fold to one VA, emitted by two objects)
VinceZapHat::VinceZapHat(SwampSceneNode* node) : SwampScript(node)
{
    _animSpell = g_swamp->GetAnimationManager()->Load("vince_attack_zaphat");
    GetVince()->StartAnimation(_animSpell, false, false);
    GetVince()->SetCurSpeed(0.0f);
    AnimCam::SwitchTo("camera_vinceLightning", 0.0f, false, 0.0f);
}
#endif
