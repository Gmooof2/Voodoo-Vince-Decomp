// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampGeometry.cpp OBJ: Xbox\Official_Release\SwampGeometry.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AACylinder { // NAME: proto-pdb:default.pdb
    bool Overlaps(const AACylinder&) const;

    Vector3 center;    // 0x00
    float radius;      // 0x0C
    float half_height; // 0x10
};
DECOMP_SIZE_ASSERT(AACylinder, 0x14);

// FUNCTION: USA 0x000DDF56
// PROTOTYPE: PREVIEW 0x004C9311 identical
// NAME: proto-pdb:default.pdb
bool AACylinder::Overlaps(const AACylinder& bc) const
{
    float v1 = bc.center.y - center.y;
    float v2 = bc.half_height + half_height;
    if (v1 > v2 || v1 < -v2)
        return false;
    float v3 = bc.center.x - center.x;
    float v4 = bc.center.z - center.z;
    float v5 = bc.radius + radius;
    return v3 * v3 + v4 * v4 <= v5 * v5;
}

struct Vector4 { // NAME: proto-pdb:default.pdb
    Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
    float w; // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

Vector3 Normalize(const Vector3&);

// FUNCTION: USA 0x000DE1A7
// PROTOTYPE: PREVIEW 0x004C9562 identical
// NAME: proto-pdb:default.pdb
Vector4 GetPlaneEquation(const Vector3& normal, const Vector3& point)
{
    Vector3 n = Normalize(normal);
    Vector4 equation(n);
    equation.w = -(n.y * point.y + n.z * point.z + n.x * point.x);
    return equation;
}

struct AABox { // NAME: proto-pdb:default.pdb
    Vector3* GeneratePoints(Vector3*) const;

    Vector3 low;  // 0x00
    Vector3 high; // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

// FUNCTION: USA 0x000DDEF8
// PROTOTYPE: PREVIEW 0x004C92B3 identical
// NAME: proto-pdb:default.pdb
Vector3* AABox::GeneratePoints(Vector3* pts) const
{
    for (int i = 0; i < 8; i++) {
        pts[i].x = (&low)[1 - (i & 1)].x;
        pts[i].y = (&low)[1 - ((i >> 1) & 1)].y;
        pts[i].z = (&low)[1 - ((i >> 2) & 1)].z;
    }
    return pts;
}
