// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampQuat.cpp OBJ: Xbox\Official_Release\SwampQuat.obj OPT: /O1 /Oy /Oi
#include <math.h>
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    void Normalize();

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
    float w; // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Set(float xr, float yr, float zr);
    void Set(const Vector3& axis, float angle);
    void Apply(const Vector3& vv, Vector3& result) const;
    void Matrix(struct Matrix& mtx) const;
    Quaternion& operator*=(const Quaternion& qp);

    Vector3 v; // 0x00
    float w;   // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Vector4 row[4]; // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

float FastReciprocalSquareRoot(float f); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000DC598
// PROTOTYPE: PREVIEW 0x004C7AD0 identical
// NAME: proto-pdb:default.pdb
void Quaternion::Apply(const Vector3& vv, Vector3& result) const
{
    // !FAKE: c1xx front end; dummy locals; the PDB records no locals for the q*vv product (x87-held values)
    float v1 = -(v.x * vv.x + v.y * vv.y + v.z * vv.z);
    float v2 = w * vv.x + v.y * vv.z - v.z * vv.y;
    float v3 = w * vv.y - v.x * vv.z + v.z * vv.x;
    float v4 = w * vv.z + v.x * vv.y - v.y * vv.x;
    result.x = v2 * w - v1 * v.x - v3 * v.z + v4 * v.y;
    result.y = v2 * v.z - v1 * v.y + v3 * w - v4 * v.x;
    result.z = v3 * v.x - v1 * v.z - v2 * v.y + v4 * w;
}

// FUNCTION: USA 0x000DC651
// PROTOTYPE: PREVIEW 0x004C7B89 identical
// NAME: proto-pdb:default.pdb
void Quaternion::Matrix(struct Matrix& mtx) const
{
    // !FAKE: c1xx front end; dummy locals; the PDB records only the stack-held xy, wz, xz, yz, y2
    const float v1 = v.x * v.x;
    const float v2 = v.y * v.y;
    const float v3 = v.z * v.z;
    const float v4 = w * w;
    const float v5 = v.x * 2.0f;
    const float y2 = v.y * 2.0f;
    const float v6 = v.z * 2.0f;
    const float v7 = v5 * w;
    const float v8 = y2 * w;
    const float wz = v6 * w;
    const float xy = y2 * v.x;
    const float xz = v6 * v.x;
    const float yz = v6 * v.y;
    mtx.row[0].x = v4 + v1 - v2 - v3;
    mtx.row[0].y = xy + wz;
    mtx.row[0].z = xz - v8;
    mtx.row[0].w = 0.0f;
    mtx.row[1].x = xy - wz;
    mtx.row[1].y = v4 - v1 + v2 - v3;
    mtx.row[1].z = yz + v7;
    mtx.row[1].w = 0.0f;
    mtx.row[2].x = xz + v8;
    mtx.row[2].y = yz - v7;
    mtx.row[2].z = v4 - v1 - v2 + v3;
    mtx.row[2].w = 0.0f;
    mtx.row[3].x = 0.0f;
    mtx.row[3].y = 0.0f;
    mtx.row[3].z = 0.0f;
    mtx.row[3].w = v4 + v3 + v2 + v1;
}

// FUNCTION: USA 0x000DC73D
// PROTOTYPE: PREVIEW 0x004C7C75 identical
// NAME: proto-pdb:default.pdb
Quaternion& Quaternion::operator*=(const Quaternion& qp)
{
    // !FAKE: c2 x87 scheduling; dummy locals; v.Set(...) with ww first computes ww,z,y,x (match_one 73.81 -> 78.40 -> 100.0 with stores x,y,z,w)
    float v1 = w * qp.v.x + v.x * qp.w + v.y * qp.v.z - v.z * qp.v.y;
    float v2 = w * qp.v.y - v.x * qp.v.z + v.y * qp.w + v.z * qp.v.x;
    float v3 = w * qp.v.z + v.x * qp.v.y - v.y * qp.v.x + v.z * qp.w;
    float ww = w * qp.w - v.x * qp.v.x - v.y * qp.v.y - v.z * qp.v.z;
    v.x = v1;
    v.y = v2;
    v.z = v3;
    w = ww;
    return *this;
}

// FUNCTION: USA 0x000DC7CE
// PROTOTYPE: PREVIEW 0x004C8A33 identical
// NAME: proto-pdb:default.pdb
Vector4 Normalize(const Vector4& vec)
{
    // !FAKE: inliner; dummy local; inline LengthSquared stays out of line at /O1; retail loads w,z,y,x into x87 (match_one 50.73 -> 97.02 -> 100.0)
    float v4 = vec.w, v3 = vec.z, v2 = vec.y, v1 = vec.x;
    // !FAKE: c2 register allocation; dummy local; scale held on the x87 stack across four products
    float v5 = FastReciprocalSquareRoot(v1 * v1 + v2 * v2 + v3 * v3 + v4 * v4);
    return Vector4(vec.x * v5, vec.y * v5, vec.z * v5, vec.w * v5);
}

// FUNCTION: USA 0x000DC99B
// PROTOTYPE: PREVIEW 0x004C7E75 identical
// NAME: proto-pdb:default.pdb
void Quaternion::Set(float xr, float yr, float zr)
{
    const float halfX = xr * 0.5f;
    const float halfY = yr * 0.5f;
    const float halfZ = zr * 0.5f;
    // !FAKE: c1xx front end; dummy locals; the PDB records no sin/cos locals (x87-held values)
    float v1 = sin(halfX), v2 = cos(halfX);
    float v3 = sin(halfY), v4 = cos(halfY);
    float v5 = sin(halfZ), v6 = cos(halfZ);
    v.x = v1 * (v6 * v4) - v2 * (v5 * v3);
    v.y = v1 * (v5 * v4) + v2 * (v6 * v3);
    v.z = v2 * (v5 * v4) - v1 * (v6 * v3);
    w = v2 * (v6 * v4) + v1 * (v5 * v3);
}

// FUNCTION: USA 0x000DCCD2
// PROTOTYPE: PREVIEW 0x004C802F identical
// NAME: proto-pdb:default.pdb
void Quaternion::Set(const Vector3& axis, float angle)
{
    const float halfangle = angle * 0.5f;
    v = axis;
    v.Normalize();
    // !FAKE: inliner; dummy local; operator*= stays out of line at /O1
    float v1 = sin(halfangle);
    v.x *= v1;
    v.y *= v1;
    v.z *= v1;
    w = cos(halfangle);
}
