// UNIT: src/Libs/Swamp/Xbox/Official_Release/HeatShimmer.cpp OBJ: Xbox\Official_Release\HeatShimmer.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AABox { // NAME: proto-pdb:default.pdb
    void AddPoint(const Vector3&);

    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct Quaternion; // NAME: proto-pdb:default.pdb
struct Matrix;     // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
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

    char field_4[0x6C - 0x4];          // 0x04
    AABox _BoundingBox;                // 0x6C
    char field_84[0xA4 - 0x84];        // 0x84
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// FUNCTION: USA 0x000D0769
// NAME: proto-pdb:default.pdb
class HeatShimmer : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~HeatShimmer() {}
    void SetEllipsoidRadius(const Vector3& ellipsoidRadius);
    virtual void Render(const Matrix&) const;

    Vector3 _ellipsoidRadius;          // 0xA4
    char field_B0[0xE0 - 0xB0];        // 0xB0
};
DECOMP_SIZE_ASSERT(HeatShimmer, 0xE0);

// FUNCTION: USA 0x000CFF08
// PROTOTYPE: PREVIEW 0x004BB1C8 identical
// NAME: proto-pdb:default.pdb
void HeatShimmer::SetEllipsoidRadius(const Vector3& ellipsoidRadius)
{
    _ellipsoidRadius = ellipsoidRadius;
    // !FAKE: inliner; hand form of AABox::Clear through a pointer local; /O1 keeps the inline Clear out of line,
    // retail inlines it with the box in esi (match_one 76.19 -> 77.52 hand form -> 100.0 with the local)
    AABox* p = &_BoundingBox;
    p->low.z = p->low.y = p->low.x = 3.402823466e+38F;
    p->high.z = p->high.y = p->high.x = -3.402823466e+38F;
    p->AddPoint(ellipsoidRadius);
    p->AddPoint(Vector3(-ellipsoidRadius.x, -ellipsoidRadius.y, -ellipsoidRadius.z));
}

// !FAKE: c1xx front end; scaffold call; retail emits the HeatShimmer vtable and its scalar deleting dtor in this unit
// (the ctor is not matched); this qualified call stores the vptr and odr-uses them instead (matched_set [[emitter]] row)
void pin_emit_HeatShimmer_dtor(HeatShimmer* p)
{
    p->HeatShimmer::~HeatShimmer();
}
