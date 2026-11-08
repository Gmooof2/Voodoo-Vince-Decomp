// UNIT: src/Apps/Vince/scripts/french_quarter/SafeHookRope.cpp OBJ: Xbox\OfficialRelease\SafeHookRope.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    int compare(const E* _Ptr) const;
    basic_string& assign(const E* _Ptr);
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    int _RefCount;                     // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    float HitDistance() const { return _pct; }

    unsigned int _tag;                 // 0x04
    float _pct;                        // 0x08
    char field_C[0x40 - 0xC];          // 0x0C
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    T* operator->() { return _Object; }

    T* _Object;                        // 0x00
};
class SwampScript;           // NAME: proto-pdb:default.pdb
class AABox;                 // NAME: proto-pdb:default.pdb

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    SwampScript* GetScript() const;
    void SetScript(const SwampString& name);
    const Vector3& SetLocation(const Vector3&);

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

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

    SwampString _name;                 // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
protected:
    virtual void SetState(int);                                    // vtable+0x6C
public:

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

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
    ResourceHandle() : _Resource(0) {}
    ResourceHandle& operator=(const ResourceHandle& other);
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
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

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetCollidable(bool collidable) { _collidable = collidable; }
    bool GetCollidable() const { return _collidable; }

    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    float GetHeight() const { return _height; }
    float GetRadius() const { return _radius; }

    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);

    char field_B4[0x14C];              // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
    void Remove(const SwampString&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class GameCam { // NAME: proto-pdb:default.pdb
public:
    void SetShakeFactor(float a, float b);
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString& name) const;
    GameCam* GetCurCam() const { return Find(_curCamName); }

    char field_0[0xC];                 // 0x00
    SwampString _curCamName;           // 0x0C
    char field_28[0x4C];               // 0x28
};
DECOMP_SIZE_ASSERT(CameraManager, 0x74);

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
};

class SwampCollide { // NAME: proto-pdb:default.pdb
public:
    static Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float, const GatorNode*, unsigned int);
};

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_DAMAGE = 0x0,
    MESSAGE_GAMESTART = 0x6
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(double);
}

namespace Particle { // NAME: proto-pdb:default.pdb
void LandDust(const Vector3&); // NAME: proto-pdb:default.pdb
}

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(const Vector3& axis, float angle) { Set(axis, angle); }
    void Set(const Vector3&, float);
    void Apply(const Vector3&, Vector3&) const;
    // !FAKE: inliner; hand form of the inline by-value Apply overload; no before/after recorded (C0191)
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetDeleteWhenDone(bool b) { _deleteWhenDone = b; }

    char field_A4[0xCE - 0xA4];        // 0xA4
    bool _deleteWhenDone;              // 0xCE
    char field_CF[0xE8 - 0xCF];        // 0xCF
};
DECOMP_SIZE_ASSERT(FireFlyNode, 0xE8);

class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    FireFlyNode* Add(const SwampString&, const SwampString&, unsigned int, bool);
};

extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

class DamageSourceInfo { // NAME: proto-pdb:default.pdb
public:
    DamageSourceInfo();
    ~DamageSourceInfo();

    SwampString sourceType;            // 0x00
    SwampString attackType;            // 0x1C
    float _multiplier;                 // 0x38
    bool _canBlock;                    // 0x3C
};
DECOMP_SIZE_ASSERT(DamageSourceInfo, 0x40);

// FUNCTION: USA 0x0007E4C7
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0007E4E3
// PROTOTYPE: PREVIEW 0x0046A3ED identical
// NAME: proto-pdb:default.pdb
class SafeHookRopeMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_DROP = 0x14
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_UP = 0,
        STATE_GOINGDOWN = 1,
        STATE_DOWN = 2,
        STATE_GOINGUP = 3
    };

    SafeHookRopeMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void DoCrash();
    bool VinceOnX();

    char field_BC[0xC8 - 0xBC];        // 0xBC
    ResourceHandle<AnimationResource> _idle; // 0xC8
    ResourceHandle<AnimationResource> _drop; // 0xCC
    ResourceHandle<AnimationResource> _lift; // 0xD0
    bool field_D4;                     // 0xD4
    char field_D5[0xD8 - 0xD5];        // 0xD5
    Vector3 _X;                        // 0xD8
    bool _vinceWasCollidable;          // 0xE4
};
DECOMP_SIZE_ASSERT(SafeHookRopeMain, 0xE8);

// FUNCTION: USA 0x0007E3AF
// PROTOTYPE: PREVIEW 0x0046A2B9 identical
// NAME: proto-pdb:default.pdb
SafeHookRopeMain::SafeHookRopeMain(SwampSceneNode* node) : CritterMain(node)
{
    _idle = g_swamp->GetAnimationManager()->Load("safeHookRope_idle");
    _drop = g_swamp->GetAnimationManager()->Load("safeHookRope_drop");
    _lift = g_swamp->GetAnimationManager()->Load("safeHookRope_hitLift");
    field_D4 = true;
    _state = STATE_UP;
    ((GatorNode*)_node)->StartAnimation(_idle, false, false);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1SafeHookRopeMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_SafeHookRopeMain_dtor(SafeHookRopeMain* p)
{
    p->SafeHookRopeMain::~SafeHookRopeMain();
}

// FUNCTION: USA 0x0007E305
// PROTOTYPE: PREVIEW 0x0046A20F identical
// NAME: proto-pdb:default.pdb
bool SafeHookRopeMain::VinceOnX()
{
    Vector3 vinceloc = pManager->_vinceCache->GetWorldPosition();
    // !FAKE: inliner; hand form of retail's inlined Vector3 difference and Length; pointer local p keeps the eax base
    const Vector3* p = &_X;
    float v0 = vinceloc.x - p->x;
    float v1 = vinceloc.y - p->y;
    float v2 = vinceloc.z - p->z;
    float v3 = ((CollidableCylinder*)_node)->GetRadius();
    float v4 = v0 * v0 + v1 * v1 + v2 * v2;
    if (v4 * FastReciprocalSquareRoot(v4) < v3 && vinceloc.y >= _X.y - 0.1
        && vinceloc.y < _X.y + ((CollidableCylinder*)_node)->GetHeight())
        return true;
    return false;
}

// FUNCTION: USA 0x0007E50F
// PROTOTYPE: PREVIEW 0x0046A419 identical
// NAME: proto-pdb:default.pdb
void SafeHookRopeMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_GAMESTART: {
        // !FAKE: inliner; hand form of retail's inlined Vector3 arithmetic; stand-in locals v0..v4, p, q (C0191)
        Vector3 v0;
        v0.y = ((CollidableCylinder*)_node)->GetHeight();
        const Vector3* p = &_node->GetWorldPosition();
        Vector3 v1;
        v1.x = p->x;
        v1.y = p->y - v0.y;
        v1.z = p->z;
        Vector3* q = &_X;
        *q = v1;
        Vector3 v2;
        v2.x = q->x;
        v2.y = q->y - 100.0f;
        v2.z = q->z;
        Ptr<CollisionResultObject> cr = SwampCollide::CheckLine(*q, v2, 0.0f, 0, 0);
        float v3 = cr->HitDistance();
        Vector3 v4;
        v4.x = v3 * 0.0f;
        v4.y = v3 * 100.0f;
        v4.z = v3 * 0.0f;
        q->x -= v4.x;
        q->y -= v4.y;
        q->z -= v4.z;
        break;
    }
    case MESSAGE_DROP: {
        ((GatorNode*)_node)->StartAnimation(_drop, false, false);
        SetState(STATE_GOINGDOWN);
        // !FAKE: c2 register allocation; neutral local v0; retail loads the Vince pointer once
        Vince* v0 = pManager->_vinceCache;
        _vinceWasCollidable = v0->GetCollidable();
        v0->SetCollidable(false);
        AnimCam::SwitchTo("camera_safeDrop", 3.0f, false, 0.0f);
        break;
    }
    }
}

// FUNCTION: USA 0x0007E64B
// PROTOTYPE: PREVIEW 0x0046A555 identical
// NAME: proto-pdb:default.pdb
void SafeHookRopeMain::DoCrash()
{
    // !FAKE: c2 register allocation; neutral pointer p; retail hoists &_X into ebx
    const Vector3* p = &_X;
    // !FAKE: c1xx front end; neutral loop counter v0; the PDB records no local for it
    for (int v0 = 0; v0 < 20; v0++) {
        // !FAKE: c2 x87 evaluation order; neutral local v4; retail calls RandomFloat before the axis temp
        float v4 = BeepLib::RandomFloat(6.283185307179586);
        Quaternion qrot(Vector3(0.0f, 1.0f, 0.0f), v4);
        // !FAKE: inliner; hand form of the inline by-value Quaternion::Apply plus its copy; no before/after recorded (C0191)
        Vector3 tmp;
        qrot.Apply(Vector3(0.0f, 0.0f, ((CollidableCylinder*)_node)->GetRadius()), tmp);
        Vector3 offset(tmp);
        // !FAKE: inliner; hand form of the inline Vector3 sum; no PDB operator+, no before/after recorded
        Vector3 v1;
        v1.x = offset.x + p->x;
        v1.y = offset.y + p->y;
        v1.z = offset.z + p->z;
        Particle::LandDust(v1);
    }
    SoundManager::PlaySound("qbp_safefal", 0x20, _node, true, 0.0f, 0.0f);
    // !FAKE: c2 register allocation; neutral local v2; retail keeps the Vince pointer in esi, the PDB drops it
    Vince* v2 = pManager->GetVince();
    if (v2 && _vinceWasCollidable)
        v2->SetCollidable(true);
    // !FAKE: inliner; comma expression and neutral pointer v5; hand form of the inline std::operator== (no PDB record)
    const SwampString* v5;
    if (VinceOnX() && (v5 = &v2->GetScript()->_name, v5->compare("vincemain") == 0)) {
        v2->GetScript()->Message(this, 0x17, 0); // VinceMain::MESSAGE_DROPOBJECT
        pManager->Remove("cymbals");
        v2->SetScript("VinceFlat");
        v2->GetScript()->Message(this, 0x14, (void*)5); // VinceFlat::MESSAGE_POPUP
        v2->GetScript()->Message(this, 0x16, 0); // VinceFlat::MESSAGE_SUCCESS
        v2->SetLocation(_X);
        FireFlyNode* v3 = g_particleManager->Add("areaEffect_safe", "areaEffect_safe", 0, true);
        v3->SetDeleteWhenDone(true);
        v3->SetLocation(_X);
        DamageSourceInfo dsi;
        dsi.sourceType = "vince";
        dsi.attackType = "safe";
        pManager->Find("bigpig")->GetScript()->Message(this, MESSAGE_DAMAGE, &dsi);
    }
}

// FUNCTION: USA 0x0007E8DF
// PROTOTYPE: PREVIEW 0x0046A7E9 identical
// NAME: proto-pdb:default.pdb
SwampScript* SafeHookRopeMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\docks\\SafeHookRope.h", 11) SafeHookRopeMain(node);
}

// FUNCTION: USA 0x0007E906
// PROTOTYPE: PREVIEW 0x0046A810 identical
// NAME: proto-pdb:default.pdb
void SafeHookRopeMain::AnimationFinished()
{
    switch (_state) {
    case STATE_UP:
        ((GatorNode*)_node)->StartAnimation(_idle, false, false);
        break;
    case STATE_GOINGDOWN:
        DoCrash();
        g_CameraManager->GetCurCam()->SetShakeFactor(0.75f, -1.0f);
        SetState(STATE_GOINGUP);
        ((GatorNode*)_node)->StartAnimation(_lift, false, false);
        break;
    case STATE_GOINGUP: {
        SetState(STATE_UP);
        ((GatorNode*)_node)->StartAnimation(_idle, false, false);
        // !FAKE: c1xx front end; neutral local; the PDB records no local for the Find result
        Critter* v0 = pManager->Find("SwitchSafe");
        if (v0)
            v0->GetScript()->Message(this, MESSAGE_DROP, 0);
        break;
    }
    }
}
