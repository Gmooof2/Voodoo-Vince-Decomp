// UNIT: src/Apps/Vince/core/Monster.cpp OBJ: core\XBox\OfficialRelease\Monster.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
#include <xmmintrin.h>

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _Alval;                       // 0x00
    union {
        E _Buf[16];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class T, class A = allocator<T> > class vector {
public:
    class iterator {
    public:
        iterator(T* _Ptr) : _Myptr(_Ptr) {}
        T& operator*() const { return *_Myptr; }
        T* operator->() const { return (&**this); }
        iterator& operator++() { ++_Myptr; return *this; }
        bool operator!=(const iterator& _Right) const { return _Myptr != _Right._Myptr; }
        iterator operator+(int _Off) const { return iterator(_Myptr + _Off); }
        T* _Myptr;
    };
    iterator begin() { return iterator(_Myfirst); }
    iterator end() { return iterator(_Mylast); }

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int _RefCount;            // 0x00
};

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int HitTag() const { return _tag; }

    unsigned int _tag;                 // 0x04
};

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _Object != 0; }
    const T* operator->() const { return _Object; }

    T* _Object;                        // 0x00
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
    void UnRegisterFrameTick();
    void RemoveTimerEvents();

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    SwampScript* _parent;              // 0x24
    char field_28[0x24];               // 0x28
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    void Normalize();
    Vector3& operator*=(float f) { x *= f; y *= f; z *= f; return *this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

inline Vector3 operator+(const Vector3& a, const Vector3& b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(float x, float y, float z, float w_) : v(x, y, z), w(w_) {}
    void Apply(const Vector3& in, Vector3& out) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct AABox;                // NAME: proto-pdb:default.pdb
class Vector4 { // NAME: proto-pdb:default.pdb
public:
    void Set(const Vector3& v) { x = v.x; y = v.y; z = v.z; w = 1.0f; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Matrix() {}
    Matrix(const Quaternion& q, const Vector3& t) { Rotation(q); row[3].Set(t); }
    void Rotation(const Quaternion&);

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

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
    SwampScript* GetScript() const;
    Vector3 GetFacing() const;
    const Quaternion& SetFacing(const Vector3&);
    void SetOption(unsigned int, bool, bool);
    void SetRenderingSetting(unsigned int, unsigned int, bool);
    void SetScript(const SwampString&);
    void SetScale(Vector3);
    const Vector3& GetScale() const { return _Scale; }

    char field_4[0x40 - 0x4];          // 0x04
    Quaternion _Rotation;              // 0x40
    Vector3 _Scale;                    // 0x50
    char field_5C[0xA4 - 0x5C];        // 0x5C
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
    float GetRadius() const { return _radius; }

    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource; // NAME: proto-pdb:default.pdb

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int GetBoneCount() const;
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

// visible body: cl 13.10 /O1 keeps a handle local in a register only with it (C0136, C0330)
template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    T Fetch();

    char field_0[0x14];                // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

class MultiModel { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<ModelResource> FetchModel() { return _modelQueue.Fetch(); }

    SwampShuffleQueue<ResourceHandle<ModelResource> > _modelQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiModel, 0x14);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30

    void CalcBounds(unsigned int) const;

    ResourceHandle<ModelResource> _model; // 0xB4
    char field_B8[0x16C - 0xB8];       // 0xB8
    std::vector<Matrix, std::allocator<Matrix> > _deformMatrices; // 0x16C
    char field_17C[0x200 - 0x17C];     // 0x17C
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

// Retail layout (C0147): _health 0x25C -> 0x26C.
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    void MakeDead(bool);

    char field_200[0x20C - 0x200];     // 0x200
    Vector3 _Velocity;                 // 0x20C
    SwampString _species;              // 0x218
    char field_234[0x26C - 0x234];     // 0x234
    int _health;                       // 0x26C proto 0x25C
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void AddNode(SwampSceneNode* node, unsigned int);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    SwampScene& GetScene() const { return *_scene; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x4];                 // 0x08
    SwampScene* _scene;                // 0x0C
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetGravity() const { return Gravity; }

    char field_0[0x8C];                // 0x00
    float Gravity;                     // 0x8C
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
}

namespace Particle { // NAME: proto-pdb:default.pdb
void MonsterDeath(const Vector3&);
}

struct DamageSourceInfo { // NAME: proto-pdb:default.pdb
    DamageSourceInfo();
    ~DamageSourceInfo();

    SwampString sourceType;            // 0x00
    SwampString attackType;            // 0x1C
    float _multiplier;                 // 0x38
    bool _canBlock;                    // 0x3C
};
DECOMP_SIZE_ASSERT(DamageSourceInfo, 0x40);

class MonsterGiblets : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    struct Giblet { // NAME: proto-pdb:default.pdb
        void Init(GatorNode*);
        bool Update();

        GatorNode* owner;              // 0x00
        Vector3 location;              // 0x04
        Quaternion rot;                // 0x10
        Vector3 velocity;              // 0x20
        Vector3 rotaxis;               // 0x2C
        bool active;                   // 0x38
    };

    MonsterGiblets(const ResourceHandle<ModelResource>& mh);
    virtual void Update();
    void Explode();

    std::vector<Giblet, std::allocator<Giblet> > _giblets; // 0x200
    int _alive;                        // 0x210
    float _age;                        // 0x214
};
DECOMP_SIZE_ASSERT(MonsterGiblets, 0x218);

class WaitForWhammyMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    WaitForWhammyMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual ~WaitForWhammyMain();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};

class MonsterFall : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    MonsterFall(SwampSceneNode* node);
    virtual void FrameTick();
    virtual void TimerEvent(unsigned int timerId);
    virtual ~MonsterFall();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);

    char field_4C[0xBC - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum StandardModelTypes { // NAME: proto-pdb:default.pdb
        STANDARD_MODEL_DISINTEGRATE = 0,
        STANDARD_MODEL_CUT_HORIZONTAL = 1,
        STANDARD_MODEL_CUT_VERTICAL = 2,
        STANDARD_MODEL_GIBLETS = 3
    };
    enum AttackDirection { // NAME: proto-pdb:default.pdb
        ATTACKDIR_NONE = 0,
        ATTACKDIR_STRAIGHT = 1,
        ATTACKDIR_LEFT = 2,
        ATTACKDIR_RIGHT = 3
    };
    virtual void DeathFX();                                        // vtable+0x70
    virtual Vector3 GetAttackVectorStraight();                     // vtable+0x74
    virtual Vector3 GetAttackVectorRight();                        // vtable+0x78
    virtual Vector3 GetAttackVectorLeft();                         // vtable+0x7C
    virtual float GetAttackVectorTolerance();                      // vtable+0x80
    AttackDirection CheckAttackDir();
    virtual void CollidedWith(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr);
    virtual void HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr);
    ResourceHandle<ModelResource> GetStandardModel(int modelType);
    void UpdateKnockBack();
    void KnockBack(float amount);
    virtual ~MonsterMain();

    char field_BC[0x1B0 - 0xBC];       // 0xBC
    ResourceHandle<ModelResource> _modelDisintegrate; // 0x1B0
    MultiAnimation _multiAnimDisintegrate; // 0x1B4
    ResourceHandle<ModelResource> _modelCutHorizontal; // 0x1C8
    MultiAnimation _multiAnimCutHorizontal; // 0x1CC
    ResourceHandle<ModelResource> _modelCutVertical; // 0x1E0
    MultiAnimation _multiAnimCutVertical; // 0x1E4
    MultiModel _multiModelGiblets;     // 0x1F8
    char field_20C[0x230 - 0x20C];     // 0x20C
    float _knockBackAmount;            // 0x230
    float _knockBackTimer;             // 0x234
    char field_238[0x240 - 0x238];     // 0x238
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

// FUNCTION: USA 0x000F3786
// PROTOTYPE: PREVIEW 0x004DEA06 identical
// NAME: proto-pdb:default.pdb
void MonsterFall::FrameTick()
{
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    Critter* critter = (Critter*)_node;
    Vector3 vel = critter->_Velocity;
    vel.z = 0.0f;
    vel.x = 0.0f;
    vel.y -= critter->GetGravity() * elapsed;
    critter->_Velocity = vel;
    critter->SetWorldPosition(critter->GetWorldPosition() + Vector3(0.0f, vel.y * elapsed, 0.0f));
}

// FUNCTION: USA 0x000F3809
// PROTOTYPE: PREVIEW 0x004DEA89 identical
// NAME: proto-pdb:default.pdb
SwampScript* WaitForWhammyMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("core\\Monster.cpp", 66) WaitForWhammyMain(node);
}

// FUNCTION: USA 0x000F382D
// PROTOTYPE: PREVIEW 0x004DEAAD identical
// NAME: proto-pdb:default.pdb
void WaitForWhammyMain::Message(SwampScript* source, int message, void* data)
{
    if (message == 10) {
        SwampScript* parent = _parent;
        InvalidateScript();
        parent->Message(source, message, data);
    }
}

// FUNCTION: USA 0x000F3852
// PROTOTYPE: PREVIEW 0x004DEAD2 identical
// NAME: proto-pdb:default.pdb
void MonsterMain::UpdateKnockBack()
{
    if (_knockBackTimer > 0.0f) {
        Vector3 vel = ((Critter*)_node)->_Velocity;
        _knockBackTimer -= g_swamp->GetInput().GetElapsedTime();
        if (_knockBackTimer < 0.0f) {
            _knockBackTimer = 0.0f;
            vel.z = 0.0f;
            vel.x = 0.0f;
        } else {
            float move = _knockBackTimer * _knockBackAmount * -2.0f;
            vel.x = move * _node->GetFacing().x;
            vel.z = move * _node->GetFacing().z;
        }
        ((Critter*)_node)->_Velocity = vel;
    }
}

// FUNCTION: USA 0x000F3905
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 84.41, bytes mismatch; wall-d116 (C0324): retail's rsqrtss is inline asm (ebp frame, no and esp,-16); the intrinsic aligns the frame
MonsterMain::AttackDirection MonsterMain::CheckAttackDir()
{
    Vince* vince = pManager->_vinceCache;
    // !FAKE: inliner; pointer locals to the two results, as KnockBack (D117)
    const Vector3* p0 = &_node->GetWorldPosition();
    const Vector3* p1 = &vince->GetWorldPosition();
    Vector3 tovince;
    tovince.x = p1->x - p0->x;
    tovince.z = p1->z - p0->z;
    tovince.y = 0.0f;
    if (tovince.x * tovince.x + tovince.y * tovince.y + tovince.z * tovince.z
        > ((CollidableCylinder*)_node)->GetRadius() * ((CollidableCylinder*)_node)->GetRadius()) {
        float lensq = tovince.x * tovince.x + tovince.y * tovince.y + tovince.z * tovince.z;
        float inv;
        _mm_store_ss(&inv, _mm_rsqrt_ss(_mm_load_ss(&lensq)));
        tovince *= inv;
        {
            // !FAKE: inliner; retail inlines Quaternion::Apply(const Vector3&): node read first, out after the call
            SwampSceneNode* node = _node;
            const Vector3* in = &GetAttackVectorStraight();
            Vector3 r;
            node->_Rotation.Apply(*in, r);
            Vector3 straight = r;
            float d = straight.z * tovince.z + straight.x * tovince.x; // !FAKE: partial-sum stand-in sets the dot order (C0341)
            if (d + straight.y * tovince.y >= GetAttackVectorTolerance())
                return ATTACKDIR_STRAIGHT;
        }
        {
            SwampSceneNode* node = _node;
            const Vector3* in = &GetAttackVectorRight();
            Vector3 r;
            node->_Rotation.Apply(*in, r);
            Vector3 right = r;
            float d = right.z * tovince.z + right.x * tovince.x; // !FAKE: partial-sum stand-in sets the dot order (C0341)
            if (d + right.y * tovince.y >= GetAttackVectorTolerance())
                return ATTACKDIR_RIGHT;
        }
        {
            SwampSceneNode* node = _node;
            const Vector3* in = &GetAttackVectorLeft();
            Vector3 r;
            node->_Rotation.Apply(*in, r);
            Vector3 left = r;
            float d = left.z * tovince.z + left.x * tovince.x; // !FAKE: partial-sum stand-in sets the dot order (C0341)
            if (d + left.y * tovince.y >= GetAttackVectorTolerance())
                return ATTACKDIR_LEFT;
        }
    }
    return ATTACKDIR_NONE;
}
#endif

// FUNCTION: USA 0x000F3AA9
// PROTOTYPE: PREVIEW 0x004DED29 identical
// NAME: proto-pdb:default.pdb
void MonsterGiblets::Giblet::Init(GatorNode* o)
{
    owner = o;
    location = Vector3(0.0f, 0.0f, 0.0f);
    velocity.x = BeepLib::RandomFloat(-0.75f, 0.75f);
    velocity.y = gSettings.GetGravity() * BeepLib::RandomFloat(0.25f, 0.75f);
    velocity.z = BeepLib::RandomFloat(-0.75f, 0.75f);
    rot = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    rotaxis.x = BeepLib::RandomFloat(-1.0f, 1.0f);
    rotaxis.y = BeepLib::RandomFloat(-1.0f, 1.0f);
    rotaxis.z = BeepLib::RandomFloat(-1.0f, 1.0f);
    active = true;
}

// FUNCTION: USA 0x000F3C01
// PROTOTYPE: PREVIEW 0x004DEE69 identical
// NAME: proto-pdb:default.pdb
void MonsterMain::KnockBack(float amount)
{
    Vince* vince = pManager->_vinceCache;
    if (vince) {
        // !FAKE: inliner; pointer locals to the calls and the node read first; retail reads both results through
        // the returned eax and loads _node before the first call (D117)
        SwampSceneNode* v0 = _node;
        const Vector3* v1 = &vince->GetWorldPosition();
        const Vector3* v2 = &v0->GetWorldPosition();
        Vector3 totarget;
        totarget.x = v2->x - v1->x;
        totarget.z = v2->z - v1->z;
        totarget.y = 0.0f;
        totarget.Normalize();
        _node->SetFacing(Vector3(-totarget.x, -totarget.y, -totarget.z));
        _knockBackTimer = 0.5f;
        _knockBackAmount = amount;
    }
}

// FUNCTION: USA 0x000F3EFE
// PROTOTYPE: PREVIEW 0x004DF17E identical
// NAME: proto-pdb:default.pdb
void MonsterGiblets::Explode()
{
    _alive = _model().GetBoneCount();
    for (std::vector<Giblet, std::allocator<Giblet> >::iterator it = _giblets.begin(); it != _giblets.end(); ++it)
        it->Init(this);
}

// FUNCTION: USA 0x000F3F35
// PROTOTYPE: PREVIEW 0x004DF1B5 identical
// NAME: proto-pdb:default.pdb
void MonsterGiblets::Update()
{
    for (std::vector<Giblet, std::allocator<Giblet> >::iterator it = _giblets.begin(); it != _giblets.end(); ++it) {
        if (it->active && !it->Update())
            --_alive;
    }
    if (_alive == 0)
        Invalidate();

    std::vector<Giblet, std::allocator<Giblet> >::iterator git = _giblets.begin() + 2;
    std::vector<Matrix, std::allocator<Matrix> >::iterator dit = _deformMatrices.begin() + 2;
    for (; git != _giblets.end(); ++git, ++dit) {
        // !FAKE: inliner; hand form of the inline Matrix(const Quaternion&, const Vector3&) ctor (C0191); retail
        // inlines it, cl 13.10 /O1 keeps the ctor out of line; neutral local
        Matrix v0;
        v0.Rotation(git->rot);
        v0.row[3].Set(git->location);
        *dit = v0;
    }

    _age += g_swamp->GetInput().GetElapsedTime();
    float alpha = 1.0f - _age;
    if (alpha < 0.0f)
        Invalidate();
    SetOption(8, true, true);
    SetRenderingSetting(0, (unsigned int)(alpha * 255.0f), true);
    CalcBounds(0);
}

// FUNCTION: USA 0x000F41B9
// PROTOTYPE: PREVIEW 0x004DF439 identical
// NAME: proto-pdb:default.pdb
void MonsterFall::TimerEvent(unsigned int timerId)
{
    InvalidateScript();
    Critter* critter = (Critter*)_node;
    critter->GetScript()->Message(this, 20, (void*)-1);
    critter->MakeDead(false);
}

// FUNCTION: USA 0x000F425A
// PROTOTYPE: PREVIEW 0x004DF4DA near-identical
// NAME: proto-pdb:default.pdb
void MonsterMain::HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr)
{
    if (((Critter*)_node)->_health > 0) {
        Vince* vince = pManager->_vinceCache;
        if (vince == critterHitBy) {
            DamageSourceInfo dsi;
            dsi.attackType = "bump";
            dsi.sourceType = ((Critter*)_node)->_species;
            vince->GetScript()->Message(this, 0, &dsi);
        }
    }
}

// FUNCTION: USA 0x000F42CA
// NAME: proto-pdb:default.pdb
void MonsterMain::CollidedWith(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr)
{
    if (cr.IsValid() && (cr->HitTag() & 0x400)) {
        UnRegisterFrameTick();
        RemoveTimerEvents();
        _node->SetScript("monsterfall");
        return;
    }
    if (pManager->_vinceCache == critterHit)
        HitBy(type, critterHit, cr);
    CritterMain::CollidedWith(type, critterHit, cr);
}

// FUNCTION: USA 0x000F4343
// PROTOTYPE: PREVIEW 0x004DF5C3 identical
// NAME: proto-pdb:default.pdb
SwampScript* MonsterFall::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("core\\Monster.cpp", 21) MonsterFall(node);
}

// FUNCTION: USA 0x000F4C7B
// PROTOTYPE: PREVIEW 0x004DFF80 identical
// NAME: proto-pdb:default.pdb
ResourceHandle<ModelResource> MonsterMain::GetStandardModel(int modelType)
{
    ResourceHandle<ModelResource> mh;
    switch (modelType) {
    case STANDARD_MODEL_DISINTEGRATE:
        mh = _modelDisintegrate;
        break;
    case STANDARD_MODEL_CUT_HORIZONTAL:
        mh = _modelCutHorizontal;
        break;
    case STANDARD_MODEL_CUT_VERTICAL:
        mh = _modelCutVertical;
        break;
    case STANDARD_MODEL_GIBLETS:
        mh = _multiModelGiblets.FetchModel();
        break;
    }
    return mh;
}

// FUNCTION: USA 0x000F4F77
// NAME: proto-pdb:default.pdb
void MonsterMain::DeathFX()
{
    Particle::MonsterDeath(_node->GetWorldPosition());
    ResourceHandle<ModelResource> mh = GetStandardModel(STANDARD_MODEL_GIBLETS);
    if (mh._Resource) {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
        MonsterGiblets* p = new("core\\Monster.cpp", 451) MonsterGiblets(mh);
        p->SetWorldPosition(_node->GetWorldPosition());
        p->SetScale(_node->GetScale());
        p->Explode();
        g_swamp->GetScene().AddNode(p, 0);
    }
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x000F5211
// NAME: proto-pdb:default.pdb
MonsterFall::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("MonsterFall", creationFunc);
}

// FUNCTION: USA 0x000F524D
// NAME: proto-pdb:default.pdb
WaitForWhammyMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WaitForWhammyMain", creationFunc);
}
