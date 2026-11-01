// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampCamera.cpp OBJ: Xbox\Official_Release\SwampCamera.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct AABox;      // NAME: proto-pdb:default.pdb
class SwampScript; // NAME: proto-pdb:default.pdb

class AnimationControl { // NAME: proto-pdb:default.pdb
public:
    void UpdateTriggers(SwampScript*);
};

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    T* _Resource;                      // 0x00
};

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

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

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Set(const Vector3&, float);
    void Apply(const Vector3&, Vector3&) const;
    // !FAKE: __forceinline; plain inline stays an out-of-line call at /O1, retail inlines it
    __forceinline Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    void AffineInvert();
    void View(const Vector3&, const Vector3&, const Vector3&);
    // !FAKE: __forceinline; plain inline stays an out-of-line call at /O1, retail inlines it
    __forceinline Matrix AffineInverse() const { Matrix m(*this); m.AffineInvert(); return m; }
    void Apply(const Vector3&, Vector3&) const;
    // !FAKE: __forceinline; plain inline stays an out-of-line call at /O1, retail inlines it
    __forceinline Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct Frustum { // NAME: proto-pdb:default.pdb
    Frustum();

    Vector4 _Plane[5];                 // 0x00
};
DECOMP_SIZE_ASSERT(Frustum, 0x50);

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

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Vector3 GetViewPosition() const;                       // vtable+0x24
    virtual Matrix GetViewMatrix() const;                          // vtable+0x28
    Frustum GetViewPlanes(bool usenearplane) const;
    Vector4 GetNearClipPlane() const;
    Vector4 GetFrontClipPlane() const;
    void SetFOV(float fov);
    void SetAspectRatio(float ratio);
    void SetFarPlane(float dist);
    virtual void Update();

    float _fov;                        // 0xA4
    float _aspectRatio;                // 0xA8
    float _nearPlane;                  // 0xAC
    float _farPlane;                   // 0xB0
    Vector3 _up;                       // 0xB4
    ResourceHandle<AnimationControl> _currentPath; // 0xC0
    char field_C4[0x4];                // 0xC4
    bool _projectionValid;             // 0xC8
    char field_C9[0x43];               // 0xC9
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

#ifdef NON_MATCHING
// best: match_one objdiff 74.10
// FUNCTION: USA 0x000CB0D1
// PROTOTYPE: PREVIEW 0x004B631A identical
// NAME: proto-pdb:default.pdb
Matrix SwampCamera::GetViewMatrix() const
{
    Matrix mtx;
    Vector3 location = GetViewPosition();
    const Vector3& facing = GetWorldFacing();
    mtx.View(location, Vector3(location.x + facing.x, location.y + facing.y, location.z + facing.z), _up);
    return mtx;
}
#endif

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
    Vector3 left = leftright.Apply(Vector3(0.0f, 0.0f, 1.0f));
    Vector3 right(-left.x, 0.0f, left.z);
    Vector3 top = updown.Apply(Vector3(0.0f, 0.0f, 1.0f));
    top.y /= _aspectRatio;
    Vector3 bottom(0.0f, -top.y, top.z);
    Matrix cam2world = GetViewMatrix().AffineInverse();
    Vector3 world_up = cam2world.Apply(Vector3(0.0f, 1.0f, 0.0f));
    Vector3 world_right = cam2world.Apply(Vector3(1.0f, 0.0f, 0.0f));
    Vector3 location = GetWorldPosition();
    Frustum result;
    result._Plane[0] = GetPlaneEquation(location, world_up, cam2world.Apply(left));
    result._Plane[1] = GetPlaneEquation(location, world_right, cam2world.Apply(top));
    result._Plane[2] = GetPlaneEquation(location, cam2world.Apply(right), world_up);
    result._Plane[3] = GetPlaneEquation(location, cam2world.Apply(bottom), world_right);
    if (usenearplane)
        result._Plane[4] = GetNearClipPlane();
    else
        result._Plane[4] = GetFrontClipPlane();
    return result;
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
