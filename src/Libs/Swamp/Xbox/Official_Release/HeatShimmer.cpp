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

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00

    char field_4[0x6C - 0x4];          // 0x04
    AABox _BoundingBox;                // 0x6C
    char field_84[0xA4 - 0x84];        // 0x84
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class HeatShimmer : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~HeatShimmer();
    void SetEllipsoidRadius(const Vector3& ellipsoidRadius);

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
