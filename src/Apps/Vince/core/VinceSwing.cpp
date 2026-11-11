// UNIT: src/Apps/Vince/core/VinceSwing.cpp OBJ: core\XBox\OfficialRelease\VinceSwing.obj OPT: /O1 /Oy
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
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle;
struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vector3& operator*=(float f) { x *= f; y *= f; z *= f; return *this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
#ifdef NON_MATCHING // D78: no PDB record for these operators; only the guarded FreeFall draft uses them
// !FAKE: inliner; hand forms of the inlined Vector3 free operators; retail FreeFall shows them (no PDB record), no before/after recorded
inline Vector3 operator*(const Vector3& v, float f) { return Vector3(v.x * f, v.y * f, v.z * f); }
inline Vector3 operator*(float f, const Vector3& v) { return Vector3(v.x * f, v.y * f, v.z * f); }
inline Vector3 operator+(const Vector3& a, const Vector3& b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
#endif
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb

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

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    unsigned int _RefCount;            // 0x00
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
public:
    bool HitSomething() const { return _tag != 0; }
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

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C
};

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    float GetRadius() const { return _radius; }

    char field_4[0xAC - 0x4];          // 0x04
    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40

    void SetCurSpeed(float speed) { _CurSpeed = speed; }
    const Vector3& GetVelocity() const { return _Velocity; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }

    float _MaxSpeed;                   // 0x200
    float _CurSpeed;                   // 0x204
    float _stunTimer;                  // 0x208
    Vector3 _Velocity;                 // 0x20C
};

class SwampCollide { // NAME: proto-pdb:default.pdb
public:
    static Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float, const GatorNode*, unsigned int);
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb
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

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

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
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class VinceSwing : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    static const int MAX_ROPE_NODES = 20;
    virtual void InvalidateScript();

    ResourceHandle<AnimationResource> _swing[2]; // 0xBC
    ResourceHandle<AnimationResource> _flip;     // 0xC4
    char field_C8[0xD0 - 0xC8];        // 0xC8
    Vector3 _hookPos;                  // 0xD0
    FireFlyNode* _ropeNode[MAX_ROPE_NODES]; // 0xDC
    char field_12C[0x5C];              // 0x12C

private:
    void UpdateRope();
    void KillRope();
    void KillSwing();
    void FreeFall();
};
DECOMP_SIZE_ASSERT(VinceSwing, 0x188);

// Retail layout: Vince members +0x30 here (_readyToDoubleJump 0x515 -> 0x545).
class Vince { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x545];               // 0x00
    bool _readyToDoubleJump;           // 0x545
    bool _readyToSpin;                 // 0x546
};

class GameCam { // NAME: proto-pdb:default.pdb
};

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void ClearPositionFixed();
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00128C62
// PROTOTYPE: PREVIEW 0x0051175A identical
// NAME: proto-pdb:default.pdb
void VinceSwing::KillRope()
{
    for (int i = 0; i < MAX_ROPE_NODES; i++) {
        if (_ropeNode[i]) {
            _ropeNode[i]->Invalidate();
            _ropeNode[i] = 0;
        }
    }
}

// FUNCTION: USA 0x00128C84
// PROTOTYPE: PREVIEW 0x0051177C identical
// NAME: proto-pdb:default.pdb
void VinceSwing::UpdateRope()
{
    Vector3 start = _node->GetWorldPosition();
    Vector3 end = _hookPos;
    // !FAKE: c2 stack slots; temporary; hand form of end - start, delta reuses end's slot (match_one 99.9 -> 100.0)
    Vector3 tmp(end.x - start.x, end.y - start.y, end.z - start.z);
    Vector3 delta(tmp.x * (1.0f / MAX_ROPE_NODES), tmp.y * (1.0f / MAX_ROPE_NODES), tmp.z * (1.0f / MAX_ROPE_NODES));
    for (int i = 0; i < MAX_ROPE_NODES; i++) {
        if (_ropeNode[i])
            _ropeNode[i]->SetWorldPosition(start);
        start += delta;
    }
}

// FUNCTION: USA 0x0012900F
// PROTOTYPE: PREVIEW 0x00511A7B near-identical
// NAME: proto-pdb:default.pdb
void VinceSwing::KillSwing()
{
    ((Vince*)_node)->_readyToDoubleJump = true;
    ((Vince*)_node)->_readyToSpin = true;
    KillRope();
    VinceCam* cam = (VinceCam*)g_CameraManager->Find("vincecam");
    cam->ClearPositionFixed();
}

// FUNCTION: USA 0x00129063
// PROTOTYPE: PREVIEW 0x00511ACF identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (T12.c20) but D78-blocked: needs the unrecorded free Vector3 operator*/operator+/operator- above; best allowed hand form (C0191) va_dis 48 lines off (retail keeps vel*elapsed on the x87 stack). Was 95.59 (pops hoisted above the ~Ptr test)
// T12.c28 lever E at-stop 20: best allowed 97.625 (+0x35): member-store products y,z,x + sums x,y,z, hit branch as
// 3-arg ctor temps + `float f` (hit branch then byte-equal); retail's x87 shape (3 products held to statement end, elapsed
// in memory) came only from inlined function calls in both expressions (diagnostic probes), never from a hand form
void VinceSwing::FreeFall()
{
    Vector3 vel = GetCritter()->GetVelocity();
    Vector3 oldpos = GetCritter()->GetWorldPosition();
    float elapsed = g_swamp->_input->GetElapsedTime();
    Vector3 newpos = oldpos + vel * elapsed;
    vel.y -= GetCritter()->GetGravity() * elapsed * 0.5f;
    Ptr<CollisionResultObject> cr = SwampCollide::CheckLine(oldpos, newpos, GetCritter()->GetRadius(), 0, 0);
    if (cr->HitSomething()) {
        newpos = oldpos + cr->HitDistance() * (newpos - oldpos);
        GetCritter()->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
        GetCritter()->SetCurSpeed(0.0f);
        KillSwing();
        InvalidateScript();
        // !FAKE: c2 block layout; explicit return; without it cl hoists the 3 pops above the ~Ptr test (match_one 95.59 -> 100.0, T12.c20)
        return;
    } else {
        _node->SetWorldPosition(newpos);
        GetCritter()->SetVelocity(vel);
    }
}
#endif

// FUNCTION: USA 0x00129BDA
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1VinceSwing@@UAE@XZ because this unit defines
// the ctor 0x001298F2 (not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceSwing_dtor(VinceSwing* p)
{
    p->VinceSwing::~VinceSwing();
}

// FUNCTION: USA 0x00129CC5
// PROTOTYPE: PREVIEW 0x00512731 identical
// NAME: proto-pdb:default.pdb
void VinceSwing::InvalidateScript()
{
    KillSwing();
    CritterMain::InvalidateScript();
}

