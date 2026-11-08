// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampCollide.cpp OBJ: Xbox\Official_Release\SwampCollide.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

// Opaque std (C0090, C0132): bodies copied from the VC 7.1 headers (<xmemory>, <vector>, <xutility>); no
// #include <vector>, whose <stdexcept> instantiates basic_string members into this unit
namespace std { // NAME: proto-pdb:default.pdb
template<class _Ty>
class allocator {
public:
    template<class _Other>
    struct rebind {
        typedef allocator<_Other> other;
    };
    typedef unsigned int size_type;

    allocator() {}
};

template<class _Ty, class _Alloc>
class _Vector_val {
protected:
    _Vector_val(_Alloc _Al = _Alloc())
        : _Alval(_Al)
    {
    }

    typedef typename _Alloc::template rebind<_Ty>::other _Alty;

    _Alty _Alval;
};

template<class _Ty, class _Ax = allocator<_Ty> >
class vector : public _Vector_val<_Ty, _Ax> {
public:
    typedef _Vector_val<_Ty, _Ax> _Mybase;
    typedef typename _Mybase::_Alty _Alloc;
    typedef typename _Alloc::size_type size_type;

    explicit vector(size_type _Count)
        : _Mybase()
    {
        _Construct_n(_Count, _Ty());
    }

    void _Construct_n(size_type _Count, const _Ty& _Val); // retail 0x000B26D4, called out of line

protected:
    _Ty* _Myfirst;
    _Ty* _Mylast;
    _Ty* _Myend;
};

struct _Nonscalar_ptr_iterator_tag {
};

template<class _InIt, class _OutIt> inline
_OutIt _Copy_opt(_InIt _First, _InIt _Last, _OutIt _Dest, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_Dest, ++_First)
        *_Dest = *_First;
    return (_Dest);
}
}

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float LengthSquared() const { return x * x + y * y + z * z; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector2 { // NAME: proto-pdb:default.pdb
    Vector2(float u_, float v_) : u(u_), v(v_) {}
    float LengthSquared() const { return u * u + v * v; }

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Vector4 { // NAME: proto-pdb:default.pdb
    Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb
Vector3 Normalize(const Vector3&);     // NAME: proto-pdb:default.pdb

struct Sphere { // NAME: proto-pdb:default.pdb
    Sphere(const Vector3& c, float r) : center(c), radius(r) {}
    bool RayIntersects(const Vector3&, const Vector3&, float&, float&) const;

    Vector3 center;                    // 0x00
    float radius;                      // 0x0C
};
DECOMP_SIZE_ASSERT(Sphere, 0x10);

struct AABox;                          // NAME: proto-pdb:default.pdb
struct Quaternion;                     // NAME: proto-pdb:default.pdb
struct Matrix;                         // NAME: proto-pdb:default.pdb
class BSP;                             // NAME: proto-pdb:default.pdb
class GatorNode;                       // NAME: proto-pdb:default.pdb

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

    unsigned int _RefCount;            // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    Ptr(T* p) : _Object(p)
    {
        if (_Object)
            _Object->AddRef();
    }
    Ptr(const Ptr<T>& p) : _Object(p._Object)
    {
        if (_Object)
            _Object->AddRef();
    }
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    Ptr<T>& operator=(const Ptr<T>& p);
    T* operator->() { return _Object; }
    T* operator()() { return _Object; }

    T* _Object;                        // 0x00
};

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int _tag;                 // 0x04
    float _pct;                        // 0x08
    const void* _blindData;            // 0x0C
    Vector4 _plane;                    // 0x10
    unsigned char _bone;               // 0x20
    char field_21[0x40 - 0x21];        // 0x21
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class SwampSceneNode;

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    void SetPlane(const Vector4& plane) { _plane = plane; }
    void SetPercentage(float pct) { _pct = pct; }
    void SetObject(const SwampSceneNode* object, unsigned char bone) { _blindData = object; _bone = bone; }
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

class BSPResultManager { // NAME: proto-pdb:default.pdb
public:
    static Ptr<BSPQueryResultObject> GetNewResult();
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        IsACollidableType = 0x10000000
    };
    SwampSceneNode(int);
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
public:
    void SetOption(unsigned int option, bool on, bool recurse);

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// FUNCTION: USA 0x000B1E61
// NAME: proto-pdb:default.pdb
class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    CollidableNode(int nodeType);
    virtual ~CollidableNode() {} // retail ??_GCollidableNode 0x000B1E61 inlines it (vptr store + ??1SwampSceneNode)
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3& start, const Vector3& end, float radius) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C
    void SetSphereRadius(float r) { _sphereRadius = r; }
    Ptr<CollisionResultObject> CheckLineAgainstSphere(const Vector3&, const Vector3&, float, const Vector3&,
                                                      float) const;

    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    void SetHeight(float h);
    void SetRadius(float r);

protected:
    void RecalculateSphereRadius();

public:
    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

struct CollidableInfo { // NAME: proto-pdb:default.pdb
    CollidableInfo();

    const CollidableNode* node;        // 0x00
    Sphere sphere;                     // 0x04
};
DECOMP_SIZE_ASSERT(CollidableInfo, 0x14);

class SwampCollide { // NAME: proto-pdb:default.pdb
public:
    static Ptr<CollisionResultObject> GetNewResult();
    static Ptr<CollisionResultObject> GetCollisionResult(const SwampSceneNode* object, float pct);
    static Ptr<CollisionResultObject> GetCollisionResult(Ptr<BSPQueryResultObject>& qr);
};

// FUNCTION: USA 0x000B1C8B
// NAME: proto-pdb:default.pdb
CollidableNode::CollidableNode(int nodeType) : SwampSceneNode(nodeType), _sphereRadius(0.0f), _collidable(true)
{
    SetOption(IsACollidableType, true, false);
}

// FUNCTION: USA 0x000B1E83
// NAME: proto-pdb:default.pdb
void CollidableCylinder::RecalculateSphereRadius()
{
    float tmp = Vector2(_height * 0.5f, _radius).LengthSquared(); // !FAKE: inliner; stand-in local tmp for the squared length; no before/after recorded
    SetSphereRadius(tmp * FastReciprocalSquareRoot(tmp));
}

// FUNCTION: USA 0x000B1EC6
// NAME: proto-pdb:default.pdb
// fold VA: SwampBSP2node.cpp emits its ??4?$Ptr@VBSPQueryResultObject@@... alias with this body (C0233 Fix 1)
template <class T> Ptr<T>& Ptr<T>::operator=(const Ptr<T>& p)
{
    if (_Object)
        _Object->SubRef();
    _Object = p._Object;
    if (_Object)
        _Object->AddRef();
    return *this;
}
template Ptr<CollisionResultObject>& Ptr<CollisionResultObject>::operator=(const Ptr<CollisionResultObject>& p);

// FUNCTION: USA 0x000B1F15
// NAME: proto-pdb:default.pdb
Ptr<CollisionResultObject> SwampCollide::GetNewResult()
{
    Ptr<BSPQueryResultObject> qr = BSPResultManager::GetNewResult();
    // !FAKE: inliner; hand form of the GetCollisionResult(qr) body; retail inlines it here, /O1 keeps the call (D117)
    Ptr<CollisionResultObject> tmp((CollisionResultObject*)qr());
    return tmp;
}

// FUNCTION: USA 0x000B1F49
// NAME: proto-pdb:default.pdb
Ptr<CollisionResultObject> SwampCollide::GetCollisionResult(Ptr<BSPQueryResultObject>& qr)
{
    Ptr<CollisionResultObject> cr((CollisionResultObject*)qr());
    return cr;
}

// FUNCTION: USA 0x000B1F64
// NAME: proto-pdb:default.pdb
Ptr<CollisionResultObject> SwampCollide::GetCollisionResult(const SwampSceneNode* object, float pct)
{
    Ptr<CollisionResultObject> cr = GetNewResult();
    cr->SetObject(object, 0);
    cr->SetPercentage(pct);
    return cr;
}

// FUNCTION: USA 0x000B1F90
// PROTOTYPE: PREVIEW 0x0049CD5A identical
// NAME: proto-pdb:default.pdb
// best: match_one objdiff 95.17 (4.4 T13.c5); frame slots match retail; open: normLineDir.z is `fmul [mem]`
// (retail `fld; fmul st1`, rcpLen popped later) and lineDir*pct loads pct first (retail loads lineDir.x first);
// T12.c26 lever A (`const float& v = rcpLen`, `lineDir.k * v|pct`, ref-bound call temp, member stores) neutral: 95.17
#ifdef NON_MATCHING
Ptr<CollisionResultObject> CollidableNode::CheckLineAgainstSphere(const Vector3& start, const Vector3& end, float radius,
                                                                  const Vector3& spherePos, float sphereRadius) const
{
    Vector3 lineDir(end.x - start.x, end.y - start.y, end.z - start.z);
    // !FAKE: c2 x87 code; neutral stand-in local; the PDB has no local for this difference
    Vector3 toSphere(spherePos.x - start.x, spherePos.y - start.y, spherePos.z - start.z);
    if (lineDir.x * toSphere.x + lineDir.y * toSphere.y + lineDir.z * toSphere.z > 0.0f) {
        const float rcpLen = FastReciprocalSquareRoot(lineDir.LengthSquared());
        Vector3 normLineDir(lineDir.x * rcpLen, lineDir.y * rcpLen, lineDir.z * rcpLen);
        Sphere sphere(spherePos, radius + sphereRadius);
        float tin, tout;
        if (sphere.RayIntersects(start, normLineDir, tin, tout)) {
            const float pct = tin * rcpLen;
            if (pct < 1.0f) {
                Ptr<CollisionResultObject> result = SwampCollide::GetCollisionResult(this, pct);
                // !FAKE: c2 x87 code; neutral stand-in locals v0, v1; retail scales before adding (inlined operators)
                Vector3 v0(pct * lineDir.x, pct * lineDir.y, pct * lineDir.z);
                Vector3 projectedPos(v0.x + start.x, v0.y + start.y, v0.z + start.z);
                { // !FAKE: c2 frame slots; inner block; it lets planeNormal share lineDir's slot (C0232)
                    Vector3 planeNormal = Normalize(Vector3(projectedPos.x - spherePos.x, projectedPos.y - spherePos.y,
                                                            projectedPos.z - spherePos.z));
                    Vector3 v1(planeNormal.x * sphereRadius, planeNormal.y * sphereRadius, planeNormal.z * sphereRadius);
                    Vector3 touchPoint(v1.x + spherePos.x, v1.y + spherePos.y, v1.z + spherePos.z);
                    // !FAKE: c2 x87 code; neutral stand-in local; retail sums x and y before z
                    float d = touchPoint.x * planeNormal.x + touchPoint.y * planeNormal.y;
                    Vector4 plane(planeNormal.x, planeNormal.y, planeNormal.z, -(d + touchPoint.z * planeNormal.z));
                    result->SetPlane(plane);
                    return result;
                }
            }
        }
    }
    return SwampCollide::GetNewResult();
}
#endif

// FUNCTION: USA 0x000B2166
// NAME: proto-pdb:default.pdb
void CollidableCylinder::SetHeight(float h)
{
    _height = h;
    RecalculateSphereRadius();
}

// FUNCTION: USA 0x000B2178
// NAME: proto-pdb:default.pdb
void CollidableCylinder::SetRadius(float r)
{
    _radius = r;
    RecalculateSphereRadius();
}

// FUNCTION: USA 0x000B21DC
// NAME: proto-pdb:default.pdb
Ptr<CollisionResultObject> CollidableNode::CheckLine(const Vector3& start, const Vector3& end, float radius) const
{
    Vector3 pos = GetWorldPosition();
    return CheckLineAgainstSphere(start, end, radius, pos, _sphereRadius);
}

// FUNCTION: USA 0x000B1CE5
// PROTOTYPE: PREVIEW 0x0049CAC3 identical
// NAME: proto-pdb:default.pdb
template CollidableInfo * std::_Copy_opt<CollidableInfo *,CollidableInfo *>(CollidableInfo *,CollidableInfo *,CollidableInfo *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000B277A
// PROTOTYPE: PREVIEW 0x0049D59E identical
// NAME: proto-pdb:default.pdb
template std::vector<BSP *,std::allocator<BSP *> >::vector<BSP *,std::allocator<BSP *> >(unsigned int);
