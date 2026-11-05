// UNIT: src/Apps/Vince/core/VinceCarryHeavy.cpp OBJ: core\XBox\OfficialRelease\VinceCarryHeavy.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class SwampSceneNode;    // NAME: proto-pdb:default.pdb
class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb

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
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource; // 0x00
};

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    ResourceHandle<AnimationControl> TransitionAnimation(ResourceHandle<AnimationResource>, float, bool, bool);

    char field_0[0x200];               // 0x00
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    float _MaxSpeed;                   // 0x200
    float _CurSpeed;                   // 0x204
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

    char field_4[0x1C];    // 0x04
    SwampSceneNode* _node; // 0x20
    char field_24[0x28];   // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                 // 0x4C
    char field_50[0xBC - 0x50]; // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class VinceShared : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    char field_BC[0xE4 - 0xBC]; // 0xBC
};
DECOMP_SIZE_ASSERT(VinceShared, 0xE4);

class VinceMain : public VinceShared { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript*, int, void*);
    virtual void AnimationFinished();

    char field_E4[0xF0 - 0xE4];                  // 0xE4
    ResourceHandle<AnimationResource> _animWalk; // 0xF0
    char field_F4[0x134 - 0xF4];                 // 0xF4
    ResourceHandle<AnimationResource> _animNormalIdle; // 0x134
    char field_138[0x18C - 0x138];               // 0x138
};
DECOMP_SIZE_ASSERT(VinceMain, 0x18C);

class VinceCarryHeavy : public VinceMain { // NAME: proto-pdb:default.pdb
public:
    virtual void InvalidateScript();
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void AnimationFinished();

    float _gruntTimer;              // 0x18C
    char field_190[0x1AC - 0x190];   // 0x190 std::basic_string _carriedObject
};
DECOMP_SIZE_ASSERT(VinceCarryHeavy, 0x1AC);

// FUNCTION: USA 0x00126FC4
// PROTOTYPE: PREVIEW 0x0050FB2C identical
// NAME: proto-pdb:default.pdb
void VinceCarryHeavy::Message(SwampScript* source, int message, void* data)
{
    VinceMain::Message(source, message, data);
    if (message == 0x14)
        GetCritter()->StartAnimation(_animWalk, true, false);
}

// FUNCTION: USA 0x00126FF6
// NAME: proto-pdb:default.pdb
void VinceCarryHeavy::AnimationFinished()
{
    switch (_state) {
    case 0:
        break;
    case 0xD:
        SetState(0);
        GetCritter()->StartAnimation(_animWalk, true, false);
        break;
    case 0x15:
        GetCritter()->TransitionAnimation(_animNormalIdle, 0.1f, true, false);
        _state = 0;
        GetCritter()->_CurSpeed = 0.0f;
        break;
    case 0x16:
        InvalidateScript();
        break;
    default:
        VinceMain::AnimationFinished();
        break;
    }
}
