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
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    int compare(const E* ptr) const; // src proto-pdb:default.pdb compare

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

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
struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Quaternion;           // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    int _RefCount;                     // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int HitTag() const { return _tag; }
    const Vector4& ImpactPlane() const { return _plane; }

    unsigned int _tag;                 // 0x04
    char field_8[0x10 - 0x8];          // 0x08
    Vector4 _plane;                    // 0x10
    char field_20[0x40 - 0x20];        // 0x20
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    bool HitSomething() const { return _tag != 0; }
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

template <class T>
class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    const T* operator->() const { return _Object; }

    T* _Object;                        // 0x00
};

class CritterCollision { // NAME: proto-pdb:default.pdb
public:
    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};
DECOMP_SIZE_ASSERT(CritterCollision, 0x8);

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
    void SetCollidable(bool b) { _collidable = b; }

    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
    char field_A9[0xAC - 0xA9];        // 0xA9
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
    const Vector3& GetVelocity() const { return _Velocity; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }
    bool IsOnFloor() const { return _OnFloor; }
    CritterCollision MoveMyFatAss(float, unsigned int);

    char field_200[0x20C - 0x200];     // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x250 - 0x218];     // 0x218
    bool _OnFloor;                     // 0x250
    char field_251[0x2C8 - 0x251];     // 0x251
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SwampCollide { // NAME: proto-pdb:default.pdb
public:
    static Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float, const GatorNode*, unsigned int);
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class FireFlyNode; // NAME: proto-pdb:default.pdb
class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    FireFlyNode* AddOneShot(const SwampString&, const Vector3&);
};
extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

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
    void RegisterFrameTick();
    void UnRegisterFrameTick();

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
    virtual void FrameTick();
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
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\FlourSack.h", 52) DialogueVinceCarryRegionMain(node);
}

// FUNCTION: USA 0x0007984C
// NAME: proto-pdb:default.pdb
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

    FlourSackMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void FrameTick();
    virtual void CollidedWith(CollisionType type, SwampSceneNode* critterhit, const Ptr<CollisionResultObject>& cr);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

public:

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

inline void *__cdecl operator new(unsigned int, void *_Where)
{
    return (_Where);
}

#ifdef NON_MATCHING
// best: ??_GFlourSackMain match_one objdiff 100.0, bytes match; wall: relink refuses `floursack.obj: data section
// .rdata: 0 VAs from its symbols by name` (??_7FlourSackMain 0x001DE3C0 unnamed; lift: pin it, match the ctor 0x796EC)
// !FAKE: c1xx front end; scaffold copy construction; retail emits ??_7FlourSackMain and its ??_G here from the ctor
// 0x796EC (not matched yet); the implicit copy ctor's vptr store emits them instead ([[emitter]] row)
void pin_emit_FlourSackMain_vtable(void* p, FlourSackMain* s)
{
    new(p) FlourSackMain(*s);
}
#endif

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

// FUNCTION: USA 0x000798EE
// PROTOTYPE: PREVIEW 0x00465902 identical
// NAME: proto-pdb:default.pdb
void FlourSackMain::FrameTick()
{
    bool wasonfloor = GetCritter()->IsOnFloor();
    Vector3 oldvel = GetCritter()->GetVelocity();
    if (_state == STATE_ONSCALE) {
        GetCritter()->MoveMyFatAss(GetCritter()->GetGravity(), 5);
        return;
    }
    HeavyObjectMain::FrameTick();
    bool isonfloor = GetCritter()->IsOnFloor();
    Vector3 loc = GetCritter()->GetWorldPosition();
    Ptr<CollisionResultObject> cr1 = SwampCollide::CheckLine(loc, Vector3(loc.x, loc.y - 0.01f, loc.z), 0.01f, GetCritter(), 0);
    if (cr1->HitSomething()) {
        float up = cr1->ImpactPlane().y;
        Vector3 flourpos = GetCritter()->GetWorldPosition();
        float safestopy = flourpos.y;
        // !FAKE: c1xx front end; stand-in local; the PDB records no local for the found blender (esi in retail)
        Critter* p = pManager->Find("blender");
        if (p)
            safestopy = p->GetWorldPosition().y;
        if ((cr1->HitTag() & 0x10000) || (up > 0.96f && flourpos.y <= safestopy)) {
            Vector3 vel = GetCritter()->GetVelocity();
            vel.x = vel.z = 0.0f;
            isonfloor = true;
            wasonfloor = false;
            GetCritter()->SetVelocity(vel);
            UnRegisterFrameTick();
        }
    }
    if (isonfloor && !wasonfloor && oldvel.y <= 0.0f) {
        SoundManager::PlaySound("qb_sackvinc", 0x20, GetNode(), true, 0.0f, 0.0f);
        // !FAKE: c2 register allocation; stand-in local v1; retail loads g_particleManager before GetWorldPosition and keeps it in esi
        SwampParticleManager* v1;
        (v1 = g_particleManager)->AddOneShot("dustburstlarge", GetNode()->GetWorldPosition());
        if (_destroyOnImpact) {
            GetCritter()->ReturnToHome();
            return;
        }
        GetCritter()->SetCollidable(true);
        if (_state == STATE_ONSCALE)
            RegisterFrameTick();
    }
}

// FUNCTION: USA 0x0007A02E
// PROTOTYPE: PREVIEW 0x0046602E identical
// NAME: proto-pdb:default.pdb
SwampScript* FlourSackMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\FlourSack.h", 14) FlourSackMain(node);
}
