// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampCamera.cpp OBJ: Xbox\Official_Release\SwampCamera.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
// !FAKE: c2 intrinsics; file-scope pragma; retail inlines fptan in GetProjectionMatrix under /O1 /Oy (C0213)
extern "C" double __cdecl tan(double);
#pragma intrinsic(tan)
extern "C" void* __cdecl memset(void*, int, unsigned int); // NAME: proto-pdb:default.pdb
#pragma intrinsic(memset)

struct AABox;      // NAME: proto-pdb:default.pdb
class SwampScript; // NAME: proto-pdb:default.pdb
class SwampSceneNode;
struct SwampDeformation;

void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(T* r) : _Resource(r) { _Resource->AddRef(); }
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource)
    {
        if (_Resource)
            _Resource->AddRef();
    }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }
    ResourceHandle& operator=(const ResourceHandle& h)
    {
        if (_Resource)
            _Resource->SubRef();
        _Resource = h._Resource;
        if (_Resource)
            _Resource->AddRef();
        return *this;
    }

    T* _Resource;                      // 0x00
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x10];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    bool Update(SwampScript*, SwampDeformation&, bool);
    void UpdateTriggers(SwampScript*);
    void SetAnimationSpeed(float);

protected:
    AnimationControl(const ResourceHandle<AnimationResource>&, SwampSceneNode*, bool, bool, bool);
    friend class SwampCamera;

public:
    char field_24[0x44];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    float& operator[](int i) { return (&x)[i]; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(float x, float y, float z) { Set(x, y, z); }
    void Set(float, float, float);
    void Set(const Vector3&, float);
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                        // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Vector4& operator[](int i) { return row[i]; }
    void Zero() { memset(this, 0, sizeof(Matrix)); }
    void AffineInvert();
    void Multiply(float, Matrix&) const;
    void View(const Vector3&, const Vector3&, const Vector3&);
    void Apply(const Vector3&, Vector3&) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct Frustum { // NAME: proto-pdb:default.pdb
    Frustum();

    Vector4 _Plane[5];                 // 0x00
};
DECOMP_SIZE_ASSERT(Frustum, 0x50);

struct SwampDeformation { // NAME: proto-pdb:default.pdb
    SwampDeformation() {}

    Vector3 scaling;                   // 0x00
    Vector3 rotation;                  // 0x0C
    Vector3 translation;               // 0x18
    float overlayWeight;               // 0x24
};
DECOMP_SIZE_ASSERT(SwampDeformation, 0x28);

Vector4 GetPlaneEquation(const Vector3&, const Vector3&, const Vector3&); // NAME: proto-pdb:default.pdb

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
    Vector3 GetWorldFacing() const;
    const Quaternion& SetFacing(const Vector3&);

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Vector3 GetViewPosition() const;                       // vtable+0x24
    virtual Matrix GetViewMatrix() const;                          // vtable+0x28
    virtual const Matrix& GetProjectionMatrix() const;             // vtable+0x2C
    Frustum GetViewPlanes(bool usenearplane) const;
    Vector4 GetNearClipPlane() const;
    Vector4 GetFrontClipPlane() const;
    void SetFOV(float fov);
    void SetAspectRatio(float ratio);
    void SetFarPlane(float dist);
    ResourceHandle<AnimationControl> StartPlayback(const ResourceHandle<AnimationResource>& camPath, bool looped, bool reverse);
    virtual void UpdateAnimation(unsigned int pass);
    virtual void Update();

    float _fov;                        // 0xA4
    float _aspectRatio;                // 0xA8
    float _nearPlane;                  // 0xAC
    float _farPlane;                   // 0xB0
    Vector3 _up;                       // 0xB4
    ResourceHandle<AnimationControl> _currentPath; // 0xC0
    float _speed;                      // 0xC4
    // !FAKE: c1xx; mutable; retail GetProjectionMatrix (const) writes the cache
    mutable bool _projectionValid;     // 0xC8
    mutable Matrix _projection;        // 0xCC
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

// FUNCTION: USA 0x000CAE66
// PROTOTYPE: PREVIEW 0x004B60AF identical
// NAME: proto-pdb:default.pdb
Vector3 SwampCamera::GetViewPosition() const
{
    return GetWorldPosition();
}

// FUNCTION: USA 0x000CAE76
// PROTOTYPE: PREVIEW 0x004B60BF identical
// NAME: proto-pdb:default.pdb
void SwampCamera::SetFOV(float fov)
{
    _fov = fov;
    _projectionValid = false;
}

// FUNCTION: USA 0x000CAE8A
// PROTOTYPE: PREVIEW 0x004B60D3 identical
// NAME: proto-pdb:default.pdb
void SwampCamera::SetAspectRatio(float ratio)
{
    _aspectRatio = ratio;
    _projectionValid = false;
}

// FUNCTION: USA 0x000CAE9E
// PROTOTYPE: PREVIEW 0x004B60E7 identical
// NAME: proto-pdb:default.pdb
void SwampCamera::SetFarPlane(float dist)
{
    _farPlane = dist;
    _projectionValid = false;
}

// FUNCTION: USA 0x000CAEB2
// PROTOTYPE: PREVIEW 0x004B60FB identical
// NAME: proto-pdb:default.pdb
const Matrix& SwampCamera::GetProjectionMatrix() const
{
    if (_projectionValid)
        return _projection;
    // !FAKE: c2 x87 scheduling; neutral stand-in locals w, h; the PDB records only Q (x87-only values, no slot)
    float w = (float)tan((double)(_fov * 0.5f));
    float h = w / _aspectRatio;
    float Q = _farPlane / (_farPlane - _nearPlane);
    _projection.Zero();
    _projection[0][0] = 1.0f / w;
    _projection[1][1] = 1.0f / h;
    _projection[2][2] = Q;
    _projection[2][3] = 1.0f;
    _projection[3][2] = -Q * _nearPlane;
    _projection.Multiply(1.0f / _farPlane, _projection);
    _projectionValid = true;
    return _projection;
}

// FUNCTION: USA 0x000CAF63
// PROTOTYPE: PREVIEW 0x004B61AC identical
// NAME: proto-pdb:default.pdb
Vector4 SwampCamera::GetNearClipPlane() const
{
    Vector3 viewdir = GetWorldFacing();
    Vector4 result;
    result.x = viewdir.x;
    result.y = viewdir.y;
    result.z = viewdir.z;
    // !FAKE: c2 x87 register allocation; double stand-in n (hand form of the inlined Vector3 scale, stand-ins v0, p, v1); a float n consumes it at the z product (match_one 94.58 -> 100.0)
    double n = _nearPlane;
    Vector3 v0(viewdir.x * n, viewdir.y * n, viewdir.z * n);
    const Vector3* p = &GetWorldPosition();
    Vector3 v1(v0.x + p->x, v0.y + p->y, v0.z + p->z);
    result.w = -(v1.x * viewdir.x + v1.y * viewdir.y + v1.z * viewdir.z);
    return result;
}

// FUNCTION: USA 0x000CAFF0
// PROTOTYPE: PREVIEW 0x004B6239 identical
// NAME: proto-pdb:default.pdb
Vector4 SwampCamera::GetFrontClipPlane() const
{
    Vector3 viewdir = GetWorldFacing();
    Vector4 result;
    result.x = viewdir.x;
    result.y = viewdir.y;
    result.z = viewdir.z;
    // !FAKE: inliner; pointer local p; retail reads the returned Vector3 through eax (C0191)
    const Vector3* p = &GetWorldPosition();
    result.w = -(viewdir.z * p->z + viewdir.y * p->y + viewdir.x * p->x);
    return result;
}

// FUNCTION: USA 0x000CB0D1
// PROTOTYPE: PREVIEW 0x004B631A identical
// NAME: proto-pdb:default.pdb
Matrix SwampCamera::GetViewMatrix() const
{
    Matrix mtx;
    Vector3 location = GetViewPosition();
    // !FAKE: inliner; pointer local p, named sum v0; retail reads the returned Vector3 through eax (C0191)
    const Vector3* p = &GetWorldFacing();
    Vector3 v0(location.x + p->x, location.y + p->y, location.z + p->z);
    mtx.View(location, v0, _up);
    return mtx;
}

// FUNCTION: USA 0x000CB135
// PROTOTYPE: PREVIEW 0x004B637E identical
// NAME: proto-pdb:default.pdb
Frustum SwampCamera::GetViewPlanes(bool usenearplane) const
{
    float half_fov = _fov * -0.5f;
    Quaternion leftright;
    leftright.Set(Vector3(0.0f, 1.0f, 0.0f), half_fov);
    Quaternion updown;
    updown.Set(Vector3(1.0f, 0.0f, 0.0f), half_fov);
    // !FAKE: inliner; hand form of the by-value Apply, temporary tmp; retail inlines it, /O1 keeps it out of line (T4.c3 F.00 92.85, F.01 100.0)
    Vector3 left;
    {
        Vector3 tmp;
        leftright.Apply(Vector3(0.0f, 0.0f, 1.0f), tmp);
        left = tmp;
    }
    Vector3 right(-left.x, 0.0f, left.z);
    Vector3 top;
    {
        Vector3 tmp;
        updown.Apply(Vector3(0.0f, 0.0f, 1.0f), tmp);
        top = tmp;
    }
    top.y /= _aspectRatio;
    Vector3 bottom(0.0f, -top.y, top.z);
    Matrix cam2world;
    {
        // !FAKE: inliner; pointer local p, dummy local v0 for AffineInverse's body; retail reads the returned Matrix through eax (T4.c3 F.00 92.85, F.01 100.0)
        const Matrix* p = &GetViewMatrix();
        Matrix v0(*p);
        v0.AffineInvert();
        cam2world = v0;
    }
    Vector3 world_up;
    {
        Vector3 tmp;
        cam2world.Apply(Vector3(0.0f, 1.0f, 0.0f), tmp);
        world_up = tmp;
    }
    Vector3 world_right;
    {
        Vector3 tmp;
        cam2world.Apply(Vector3(1.0f, 0.0f, 0.0f), tmp);
        world_right = tmp;
    }
    Vector3 location = GetWorldPosition();
    Frustum result;
    // !FAKE: inliner; per-plane block, temporary tmp and copy v1..v4 for the by-value Apply argument; T4.c3 F.00 92.85, F.01 100.0
    {
        Vector3 tmp;
        cam2world.Apply(left, tmp);
        Vector3 v1(tmp);
        result._Plane[0] = GetPlaneEquation(location, world_up, v1);
    }
    {
        Vector3 tmp;
        cam2world.Apply(top, tmp);
        Vector3 v2(tmp);
        result._Plane[1] = GetPlaneEquation(location, world_right, v2);
    }
    {
        Vector3 tmp;
        cam2world.Apply(right, tmp);
        Vector3 v3(tmp);
        result._Plane[2] = GetPlaneEquation(location, v3, world_up);
    }
    {
        Vector3 tmp;
        cam2world.Apply(bottom, tmp);
        Vector3 v4(tmp);
        result._Plane[3] = GetPlaneEquation(location, v4, world_right);
    }
    if (usenearplane)
        result._Plane[4] = GetNearClipPlane();
    else
        result._Plane[4] = GetFrontClipPlane();
    return result;
}

// FUNCTION: USA 0x000CB3E3
// PROTOTYPE: PREVIEW 0x004B662C identical
// NAME: proto-pdb:default.pdb
// needs the visible ResourceHandle::operator= body (C0136; 69.22 opaque); its COMDAT ??4?$ResourceHandle@VAnimationControl@@
// copy is byte-identical to the matched Generator.obj row 0x0003B04D and rides it in relink (T3 change, D117)
ResourceHandle<AnimationControl> SwampCamera::StartPlayback(const ResourceHandle<AnimationResource>& camPath, bool looped, bool reverse)
{
    if (!camPath._Resource) {
        _currentPath = ResourceHandle<AnimationControl>();
    } else {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        _currentPath = ResourceHandle<AnimationControl>(new("SwampCamera.cpp", 213) AnimationControl(camPath, 0, looped, reverse, false));
    }
    return _currentPath;
}

// FUNCTION: USA 0x000CB487
// PROTOTYPE: PREVIEW 0x004B66D0 identical
// NAME: proto-pdb:default.pdb
void SwampCamera::UpdateAnimation(unsigned int pass)
{
    if (pass && _currentPath._Resource) {
        _currentPath._Resource->SetAnimationSpeed(_speed);
        SwampDeformation deformation;
        // !FAKE: c2 call order; stand-in locals script, control; retail calls GetScript, then loads _currentPath, before pushing Update's arguments (match_one 97.40 -> 99.93)
        SwampScript* script = GetScript();
        AnimationControl* control = _currentPath._Resource;
        if (control->Update(script, deformation, false)) {
            SetWorldPosition(deformation.translation);
            Quaternion quat;
            quat.Set(deformation.rotation.x, deformation.rotation.y, deformation.rotation.z);
            // !FAKE: inliner; hand form of the by-value Apply, temporary tmp and copy v0 per block (as GetViewPlanes; match_one 99.93 -> 100.0)
            {
                Vector3 tmp;
                quat.Apply(Vector3(0.0f, 0.0f, 1.0f), tmp);
                Vector3 v0(tmp);
                SetFacing(v0);
            }
            {
                Vector3 tmp;
                quat.Apply(Vector3(0.0f, 1.0f, 0.0f), tmp);
                _up = tmp;
            }
        }
    }
}

// FUNCTION: USA 0x000CB569
// PROTOTYPE: PREVIEW 0x004B67B2 identical
// NAME: proto-pdb:default.pdb
void SwampCamera::Update()
{
    AnimationControl* v1 = _currentPath._Resource;
    if (v1)
        v1->UpdateTriggers(GetScript());
}
