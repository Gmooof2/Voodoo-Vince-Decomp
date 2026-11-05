// UNIT: src/Apps/Vince/core/FanCeiling.cpp OBJ: core\XBox\OfficialRelease\FanCeiling.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    int compare(const E* _Ptr) const;

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[0x1C / 4];
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
template <class E, class T, class A>
inline bool operator!=(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return !(_Left == _Right);
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    void Set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    // !FAKE: inliner; hand form of the PDB inline conversion operator
    operator Vector3() const { return *(const Vector3*)this; }

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
    float w; // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    const Vector4& operator[](int i) const { return row[i]; }

    Vector4 row[4]; // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v; // 0x00
    float w;   // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

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

    void RegisterTimerEvent(unsigned int timerId, float time, bool repeat);

protected:
    SwampScript(SwampSceneNode* node);

public:
    SwampString _name;                 // 0x04
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

class CeilingFanMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_ATTACK = 0,
        TIMER_RETURNIDLE = 1
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_SPINVINCE = 1,
        STATE_ATTACK = 2
    };

    CeilingFanMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int);
    virtual void HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr);
private:
    virtual void SetState(int state);
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CeilingFanMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationTime() const { return _time; }

    char field_24[0x38 - 0x24];        // 0x24
    float _time;                       // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x34 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);
    T& operator()() { return *_Resource; }

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
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

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

    const Quaternion& SetRotation(const Quaternion& rotation);
    const Quaternion& GetRotation() const { return _Rotation; }
    const Quaternion& SetFacing(const Vector3& facing);
    SwampScript* GetScript() const;
    void SetScript(const SwampString& script);

    char field_4[0x40 - 0x4];          // 0x04
    Quaternion _Rotation;              // 0x40
    char field_50[0xA4 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C

    void SetCollidable(bool c) { _collidable = c; }

    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode, all at 0x0; CollidableCylinder not modelled
class GatorNode : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl> GetCurrentAnimation() const { return _currentAnim; }
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30

    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool restart, bool loop);
    Matrix GetBoneOrientation(const SwampString& bone) const;

    char field_AC[0x140 - 0xAC];       // 0xAC
    ResourceHandle<AnimationControl> _currentAnim; // 0x140
    char field_144[0x200 - 0x144];     // 0x144
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C

    void SetVelocity(const Vector3& vel) { _Velocity = vel; }

    char field_200[0x20C - 0x200];     // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x2C8 - 0x218];     // 0x218
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);
class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class GameCam { // NAME: proto-pdb:default.pdb
};

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void SetDefaultVals();

    char field_0[0x160];               // 0x00
    float _elevation;                  // 0x160
    char field_164[0x238 - 0x164];     // 0x164
};
DECOMP_SIZE_ASSERT(VinceCam, 0x238);

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString& name) const;
};
extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class VinceCeilingFan : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SYNCANIM = 0x14
    };
    VinceCeilingFan(SwampSceneNode* node);
    virtual ~VinceCeilingFan();
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _animSpin; // 0x4C
};
DECOMP_SIZE_ASSERT(VinceCeilingFan, 0x50);

// FUNCTION: USA 0x00131F74
// PROTOTYPE: PREVIEW 0x0051A997 identical
// NAME: proto-pdb:default.pdb
void CeilingFanMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_ATTACK:
        SetState(STATE_ATTACK);
        break;
    case TIMER_RETURNIDLE:
        SetState(STATE_IDLE);
        break;
    }
}

// FUNCTION: USA 0x00131F8E
// PROTOTYPE: PREVIEW 0x0051A9B1 identical
// NAME: proto-pdb:default.pdb
void VinceCeilingFan::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_SYNCANIM)
        ((GatorNode*)_node)->GetCurrentAnimation()().SetAnimationTime(*(float*)data);
}

// FUNCTION: USA 0x00131FB6
// PROTOTYPE: PREVIEW 0x0051A9D9 identical
// NAME: proto-pdb:default.pdb
void CeilingFanMain::HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr)
{
    if (pManager->_vinceCache == critterHitBy && pManager->_vinceCache->GetScript()->_name != "vinceceilingfan")
        SetState(STATE_SPINVINCE);
}

// FUNCTION: USA 0x00131FEC
// PROTOTYPE: PREVIEW 0x0051AA0F identical
// NAME: proto-pdb:default.pdb
void CeilingFanMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_IDLE:
        ((CollidableNode*)_node)->SetCollidable(true);
        break;
    case STATE_SPINVINCE: {
        // !FAKE: c2 register allocation; dummy locals; the PDB records no local for the cam and Vince pointers
        VinceCam* v1 = (VinceCam*)g_CameraManager->Find("vincecam");
        v1->_elevation = 0.0f;
        Vince* v2 = pManager->_vinceCache;
        v2->SetScript("VinceCeilingFan");
        ResourceHandle<AnimationControl> ach = ((GatorNode*)_node)->GetCurrentAnimation();
        float time = ach().GetAnimationTime();
        v2->GetScript()->Message(this, VinceCeilingFan::MESSAGE_SYNCANIM, &time);
        v2->SetWorldPosition(_node->GetWorldPosition());
        v2->SetRotation(_node->GetRotation());
        ((CollidableNode*)_node)->SetCollidable(false);
        RegisterTimerEvent(TIMER_ATTACK, 3.0f, false);
        break;
    }
    case STATE_ATTACK: {
        // !FAKE: c2 register allocation; dummy locals; the PDB records no local for the cam and Vince pointers
        VinceCam* v1 = (VinceCam*)g_CameraManager->Find("vincecam");
        v1->SetDefaultVals();
        Vince* v2 = pManager->_vinceCache;
        Matrix mBody = v2->GetBoneOrientation("body");
        // !FAKE: inliner; hand forms of the by-value Apply and the inline Vector3 operator+ (C0191); neutral stand-in locals
        // !FAKE: c2 register allocation; blocks and split locals give retail's slot sharing (match_one 99.02 -> 99.93 -> 100)
        Vector3 newpos;
        {
            Vector3 v6;
            v2->GetRotation().Apply(mBody[3], v6);
            Vector3 v3(v6);
            const Vector3* p = &v2->GetWorldPosition();
            newpos.Set(v3.x + p->x, v3.y + p->y, v3.z + p->z);
            v2->GetScript()->InvalidateScript();
            v2->SetWorldPosition(newpos);
        }
        v2->Stun(0.5f);
        Matrix mMove = v2->GetBoneOrientation("move");
        {
            Vector3 vel;
            Vector3 v5;
            v2->GetRotation().Apply(mBody[1], v5);
            vel = v5;
            v2->SetVelocity(vel);
            v2->SetFacing(Vector3(-vel.x, 0.0f, -vel.z));
        }
        RegisterTimerEvent(TIMER_RETURNIDLE, 0.5f, false);
        break;
    }
    }
}

// FUNCTION: USA 0x00132273
// PROTOTYPE: PREVIEW 0x0051AC96 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); wall (4.4 T4.c8 relink): vtable slot ??_G 0x001322E6 calls
// the dtor 0x0008C43B (folded ~VinceCeilingFan/~VinceZapHat, in VinceZapHat's unit), no name in symbols.txt/symbols_pin.txt:
// `external ??1VinceCeilingFan@@UAE@XZ does not resolve by name`; with VinceZapHat also live, `our ranges overlap at
// 0x001322E6` (??_GVinceCeilingFan and ??_GVinceZapHat fold to one VA, emitted by two objects)
VinceCeilingFan::VinceCeilingFan(SwampSceneNode* node) : SwampScript(node)
{
    _animSpin = g_swamp->GetAnimationManager().Load("vince_attack_fanceiling");
    ((GatorNode*)_node)->StartAnimation(_animSpin, true, false);
}
#endif

// FUNCTION: USA 0x00132302
// PROTOTYPE: PREVIEW 0x0051AD25 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceCeilingFan::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\FanCeiling.h", 36) VinceCeilingFan(node);
}

// FUNCTION: USA 0x00132347
// PROTOTYPE: PREVIEW 0x0051AD6A identical
// NAME: proto-pdb:default.pdb
SwampScript* CeilingFanMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\FanCeiling.h", 11) CeilingFanMain(node);
}
