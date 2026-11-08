// UNIT: src/Apps/Vince/scripts/Bayou/VinceInWind.cpp OBJ: Xbox\OfficialRelease\VinceInWind.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};

template <class T, class A> class vector {
public:
    ~vector() { _Tidy(); }

protected:
    void _Tidy();

private:
    int _pad00[4];
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
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    char field_24[0x34 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

class ModelResource;        // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
class Matrix;                // NAME: proto-pdb:default.pdb

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
};

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C
};

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
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
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                       // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class VinceShared : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    char field_BC[0xC0 - 0xBC];        // 0xBC
    SwampString _objectToPickUp;       // 0xC0
    ResourceHandle<TextureResource> _footprintMud; // 0xDC
    ResourceHandle<TextureResource> _footprintGrass; // 0xE0
};
DECOMP_SIZE_ASSERT(VinceShared, 0xE4);

// FUNCTION: USA 0x00028891
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1VinceShared@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceShared_dtor(VinceShared* p)
{
    p->VinceShared::~VinceShared();
}

class VinceMain : public VinceShared { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript*, int, void*);
private:

    int _idleCount;                    // 0xE4
    ResourceHandle<AnimationResource> _animJump; // 0xE8
    ResourceHandle<AnimationResource> _animFloat; // 0xEC
    ResourceHandle<AnimationResource> _animWalk; // 0xF0
    ResourceHandle<AnimationResource> _animRun; // 0xF4
    ResourceHandle<AnimationResource> _animDamage[2]; // 0xF8
    ResourceHandle<AnimationResource> _animAirDamage[2]; // 0x100
    ResourceHandle<AnimationResource> _animDoubleJump; // 0x108
    ResourceHandle<AnimationResource> _animFlourWalk; // 0x10C
    ResourceHandle<AnimationResource> _animObjectWalk; // 0x110
    ResourceHandle<AnimationResource> _animObjectWalkInjured; // 0x114
    ResourceHandle<AnimationResource> _animObjectRun; // 0x118
    ResourceHandle<AnimationResource> _animFallOfDoom; // 0x11C
    ResourceHandle<AnimationResource> _animFreeFall; // 0x120
    ResourceHandle<AnimationResource> _animWalkInjured; // 0x124
    ResourceHandle<AnimationResource> _animObjectIdle; // 0x128
    ResourceHandle<AnimationResource> _animInjuredObjectIdle; // 0x12C
    ResourceHandle<AnimationResource> _animInjuredIdle; // 0x130
    ResourceHandle<AnimationResource> _animNormalIdle; // 0x134
    char field_138[0x15C - 0x138];     // 0x138
    std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > _carryCaption; // 0x15C
    bool _wasInEyeCam;                 // 0x178
    std::vector<Critter*, std::allocator<Critter*> > _affectedCritters; // 0x17C
};
DECOMP_SIZE_ASSERT(VinceMain, 0x18C);

// FUNCTION: USA 0x000288C5
// PROTOTYPE: PREVIEW 0x004176F8 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1VinceMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceMain_dtor(VinceMain* p)
{
    p->VinceMain::~VinceMain();
}

class VinceInWind : public VinceMain { // NAME: proto-pdb:default.pdb
public:
    VinceInWind(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(VinceInWind, 0x18C);

// FUNCTION: USA 0x00028864
// PROTOTYPE: PREVIEW 0x00417697 identical
// NAME: proto-pdb:default.pdb
void VinceInWind::Message(SwampScript* source, int message, void* data)
{
    VinceMain::Message(source, message, data);
    if (message == 0)
        GetCritter()->Stun(0.0f);
}

// FUNCTION: USA 0x00028C26
// PROTOTYPE: PREVIEW 0x00417A59 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceInWind::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\VinceInWind.cpp", 21) VinceInWind(node);
}
