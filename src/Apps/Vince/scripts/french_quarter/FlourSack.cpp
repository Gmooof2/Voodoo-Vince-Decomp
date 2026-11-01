// UNIT: src/Apps/Vince/scripts/french_quarter/FlourSack.cpp OBJ: Xbox\OfficialRelease\FlourSack.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

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

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    T* _Resource;                      // 0x00
};

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    int compare(const E* ptr) const; // src proto-pdb:default.pdb compare

private:
    char _pad00[0x1C];
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& left, const E* right)
{
    return left.compare(right) == 0;
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

// src proto-pdb:default.pdb BSPQueryResultObject base DumbBase at 0x0, not modelled
class BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    unsigned int HitTag() const { return _tag; }

    char field_0[0x4];                 // 0x00
    unsigned int _tag;                 // 0x04
    char field_8[0x40 - 0x8];          // 0x08
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

template <class T>
class Ptr { // NAME: proto-pdb:default.pdb
public:
    const T* operator->() const { return _Object; }

    T* _Object;                        // 0x00
};

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

    int _Type;                         // 0x04
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

    char field_200[0x2C8 - 0x200];     // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

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
    Critter* GetCritter() { return (Critter*)GetNode(); }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~CarriedObjectMain();
    char field_BC[0x3C];               // 0xBC
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

class HeavyObjectMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
};
DECOMP_SIZE_ASSERT(HeavyObjectMain, 0xF8);

void* operator new(unsigned int size, const char* file, int line);

class DialogueRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb

    char field_4C[0xA4 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(DialogueRegionMain, 0xA4);

class DialogueVinceCarryRegionMain : public DialogueRegionMain { // NAME: proto-pdb:default.pdb
public:
    DialogueVinceCarryRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DialogueVinceCarryRegionMain, 0xA4);

// FUNCTION: USA 0x000796C5
// PROTOTYPE: PREVIEW 0x004656D9 identical
// NAME: proto-pdb:default.pdb
SwampScript* DialogueVinceCarryRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\FlourSack.h", 52) DialogueVinceCarryRegionMain(node);
}

// FUNCTION: USA 0x00079868
// PROTOTYPE: PREVIEW 0x0046587C identical
// NAME: proto-pdb:default.pdb
class FlourSackMain : public HeavyObjectMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_STUCK = 3,
        STATE_FALLING = 4,
        STATE_NOTINWORLD = 5,
        STATE_INWORLD = 6,
        STATE_ONSCALE = 7
    };

    virtual void Activate();
    virtual void CollidedWith(CollisionType type, SwampSceneNode* critterhit, const Ptr<CollisionResultObject>& cr);

    ResourceHandle<AnimationResource> _animStuck; // 0xF8
    ResourceHandle<AnimationResource> _animFall;  // 0xFC
    int _id;                           // 0x100
    bool _pickedUp;                    // 0x104
    bool _vinceInBounds;               // 0x105
    bool _destroyOnImpact;             // 0x106
    bool _firstDrop;                   // 0x107
};
DECOMP_SIZE_ASSERT(FlourSackMain, 0x108);

// FUNCTION: USA 0x00079672
// NAME: proto-pdb:default.pdb
void FlourSackMain::CollidedWith(CollisionType type, SwampSceneNode* critterhit, const Ptr<CollisionResultObject>& cr)
{
    if (critterhit && critterhit->GetName() == "blender" && type == VerticalCollision && cr->HitTag() == 1)
        GetCritter()->ReturnToHome();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1FlourSackMain@@UAE@XZ because this unit defines
// the ctor 0x796EC (not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_FlourSackMain_dtor(FlourSackMain* p)
{
    p->FlourSackMain::~FlourSackMain();
}

// FUNCTION: USA 0x0007964B
// PROTOTYPE: PREVIEW 0x0046565F identical
// NAME: proto-pdb:default.pdb
void FlourSackMain::Activate()
{
    if (_state == STATE_STUCK) {
        GetCritter()->StartAnimation(_animFall, false, false);
        SetState(STATE_FALLING);
    }
}
