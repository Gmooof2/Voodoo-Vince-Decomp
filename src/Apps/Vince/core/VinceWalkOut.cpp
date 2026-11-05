// UNIT: src/Apps/Vince/core/VinceWalkOut.cpp OBJ: core\XBox\OfficialRelease\VinceWalkOut.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    unsigned int size() const { return _Mysize; }
    // npos (PDB static) left undeclared: its in-class initializer emits an unpinned ?npos@ CONST COMDAT (C0215)
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[5];            // 0x00
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
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

    void RegisterFrameTick();

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24

protected:
    SwampScript(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class ModelResource { // NAME: proto-pdb:default.pdb
public:
    int GetBoneIndex(const SwampString& name, bool) const;
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    T* operator->() const { return _Resource; }

    T* _Resource;                      // 0x00
};

class AABox;                 // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class AnimationControl;      // NAME: proto-pdb:default.pdb

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager* GetAnimationManager() { return _animationManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct Vector4 { // NAME: proto-pdb:default.pdb
    // !FAKE: inliner; struct-copy body; retail copies the 3 dwords with movsd, Vector3(x, y, z) gives mov eax (match_one 85.71 -> 97.67)
    operator Vector3() const { return *(const Vector3*)this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Matrix() {}
    Vector4& operator[](int i) { return row[i]; }

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3& v, Vector3& r) const;
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

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
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool, bool);
    Matrix GetBoneOrientation(const SwampString& name) const;

    ResourceHandle<ModelResource> _model; // 0xB4
    char field_B8[0xD4 - 0xB8];        // 0xB8
    int _posBone;                      // 0xD4
    char field_D8[0xF4 - 0xD8];        // 0xD8
    int _shadowBone;                   // 0xF4
    char field_F8[0x200 - 0xF8];       // 0xF8
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    int _RefCount;                     // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    char field_4[0x40 - 0x4];          // 0x04
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

    T* _Object;                        // 0x00
};

class CritterCollision { // NAME: proto-pdb:default.pdb
public:
    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};
DECOMP_SIZE_ASSERT(CritterCollision, 0x8);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    const Vector3& GetVelocity() const { return _Velocity; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }
    CritterCollision MoveMyFatAss(float, unsigned int);

    char field_200[0x4];               // 0x200
    float _CurSpeed;                   // 0x204
    char field_208[0x20C - 0x208];     // 0x208
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x250 - 0x218];     // 0x218
    bool _OnFloor;                     // 0x250
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void PutOutFire();

    char field_251[0x2F5 - 0x251];     // 0x251
    bool _inputEnabled;                // 0x2F5
    bool _canUseEyeCam;                // 0x2F6
};

class GameCam { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
    Vector3 _Location;                 // 0x34
};

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void SetPositionFixed(const Vector3& pos, float);
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class VinceWalkOut : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SETWALKOUTANIM = 0x0
    };
    VinceWalkOut(SwampSceneNode* node);
    virtual ~VinceWalkOut();
    virtual void FrameTick();
    virtual bool LosingControl(const SwampString&);
    virtual void Message(SwampScript*, int, void*);
    Vince* GetVince() { return (Vince*)_node; }
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    int _oldBone;                      // 0x4C
    SwampString _animName;             // 0x50
    bool _onFloor;                     // 0x6C
};
DECOMP_SIZE_ASSERT(VinceWalkOut, 0x70);

// FUNCTION: USA 0x00136312
// PROTOTYPE: PREVIEW 0x00520502 identical
// NAME: proto-pdb:default.pdb
void VinceWalkOut::FrameTick()
{
    if (!_onFloor) {
        Vector3 v = GetVince()->GetVelocity();
        v.z = 0.0f;
        v.x = 0.0f;
        GetVince()->SetVelocity(v);
        GetVince()->MoveMyFatAss(GetVince()->GetGravity(), 0);
        _onFloor = GetVince()->_OnFloor;
    }
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1 = the vptr store)
// FUNCTION: USA 0x00136386
// PROTOTYPE: PREVIEW 0x00520576 near-identical
// NAME: proto-pdb:default.pdb
VinceWalkOut::VinceWalkOut(SwampSceneNode* node) : SwampScript(node)
{
    _onFloor = false;
    GetVince()->_CurSpeed = 0;
    GetVince()->_canUseEyeCam = false;
    GetVince()->_inputEnabled = false;
    GetVince()->PutOutFire();
    RegisterFrameTick();
    VinceCam* cam = (VinceCam*)g_CameraManager->Find("vincecam");
    cam->SetPositionFixed(cam->_Location, 1.0f);
    _oldBone = GetVince()->_posBone;
    GetVince()->_posBone = GetVince()->_model->GetBoneIndex("move", false);
    GetVince()->_shadowBone = -1;
}
#endif

// FUNCTION: USA 0x0013648C
// PROTOTYPE: PREVIEW 0x00520652 identical
// NAME: proto-pdb:default.pdb
void VinceWalkOut::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_SETWALKOUTANIM) {
        _animName = *(SwampString*)data;
        if (_animName.size()) {
            GetVince()->StartAnimation(g_swamp->GetAnimationManager()->Load(_animName), false, false);
            GetVince()->UpdateAnimation(0);
            Matrix m = GetVince()->GetBoneOrientation("move");
            // !FAKE: inliner; hand form of retail's inlined by-value Quaternion::Apply (out of line in our /O1 build):
            // stand-ins v1 (result), v3 (the Vector4 -> Vector3 argument; as a temporary its slot is shared, 99.90) (C0191, C0214)
            Vector3 v1;
            Vector3 v3(m[3]);
            GetVince()->_Rotation.Apply(v3, v1);
            Vector3 offset(v1);
            // !FAKE: inliner; hand form of the inlined Vector3 operator- (no PDB record); stand-ins v0, p, v2 (C0191)
            Vince* v0 = GetVince();
            const Vector3* p = &v0->GetWorldPosition();
            Vector3 v2(p->x - offset.x, p->y - offset.y, p->z - offset.z);
            v0->SetWorldPosition(v2);
        }
    }
}

void* operator new(unsigned int size, const char* file, int line);

// FUNCTION: USA 0x0013657B
// PROTOTYPE: PREVIEW 0x00520741 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceWalkOut::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\VinceWalkOut.h", 6) VinceWalkOut(node);
}
