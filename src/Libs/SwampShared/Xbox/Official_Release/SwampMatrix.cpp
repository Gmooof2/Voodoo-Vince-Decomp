// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampMatrix.cpp OBJ: Xbox\Official_Release\SwampMatrix.obj OPT: /O1 /Oy /Oi
#include <string.h>
#include <math.h>
#include <xmmintrin.h>
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    void Normalize();

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    void Set(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
    void Set(const Vector3& v) { x = v.x; y = v.y; z = v.z; }
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
    float w; // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v; // 0x00
    float w; // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

Vector3 Normalize(const Vector3&); // NAME: proto-pdb:default.pdb

struct Matrix { // NAME: proto-pdb:default.pdb
    Matrix() {}

    void Zero() { memset(this, 0, sizeof(Matrix)); }
    void FullInvert();
    void AffineInvert();
    void Translation(const Vector3& vec);
    void Rotation(const Quaternion& q);
    void Rotation(float xr, float yr, float zr);
    void Mirror(const Vector4& plane);
    void Orient(const Vector3& dir, const Vector3& up);
    void View(const Vector3& from, const Vector3& to, const Vector3& up);
    void Scaling(float scale);
    void RotationAboutPivot(float xr, float yr, float zr, const Vector3& pivot);
    void TransformationAboutPivot(float xr, float yr, float zr, const Vector3& t, const Vector3& pivot);
    void Apply(const Vector4& vec, Vector4& result) const;
    void Apply(const Vector3& vec, Vector3& result) const;
    void BatchApply(const Vector3* src, Vector3* dest, int count) const;
    void ApplyNormal(const Vector3& vec, Vector3& result) const;
    void Multiply(float s, Matrix& result) const;
    void Multiply(const Matrix& m, Matrix& result) const;
    void AffineMultiply(const Matrix& m, Matrix& result) const;

    Vector4 row[4]; // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

#pragma warning(push)
#pragma warning(disable: 4700) // tmp1, row1, row3 feed _mm_loadl_pi uninitialized; retail loads their stack homes (movaps xmm2,[esp+0x14])
// FUNCTION: USA 0x000DCED9
// PROTOTYPE: PREVIEW 0x004C8236 identical
// NAME: proto-pdb:default.pdb
void Matrix::FullInvert()
{
    __m128 minor0, minor1, minor2, minor3;
    __m128 row0, row1, row2, row3;
    __m128 det, tmp1;

    tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)&row[0].x), (__m64*)&row[1].x);
    row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)&row[2].x), (__m64*)&row[3].x);
    row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
    row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
    tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)&row[0].z), (__m64*)&row[1].z);
    row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)&row[2].z), (__m64*)&row[3].z);
    row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
    row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);

    tmp1 = _mm_mul_ps(row2, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor0 = _mm_mul_ps(row1, tmp1);
    minor1 = _mm_mul_ps(row0, tmp1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
    minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
    minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

    tmp1 = _mm_mul_ps(row1, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
    minor3 = _mm_mul_ps(row0, tmp1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
    minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

    tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    row2 = _mm_shuffle_ps(row2, row2, 0x4E);
    minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
    minor2 = _mm_mul_ps(row0, tmp1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
    minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

    tmp1 = _mm_mul_ps(row0, row1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));

    tmp1 = _mm_mul_ps(row0, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
    minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));

    tmp1 = _mm_mul_ps(row0, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

    det = _mm_mul_ps(row0, minor0);
    det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
    det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
    tmp1 = _mm_rcp_ss(det);
    det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
    det = _mm_shuffle_ps(det, det, 0x00);

    minor0 = _mm_mul_ps(det, minor0);
    _mm_storel_pi((__m64*)&row[0].x, minor0);
    _mm_storeh_pi((__m64*)&row[0].z, minor0);
    minor1 = _mm_mul_ps(det, minor1);
    _mm_storel_pi((__m64*)&row[1].x, minor1);
    _mm_storeh_pi((__m64*)&row[1].z, minor1);
    minor2 = _mm_mul_ps(det, minor2);
    _mm_storel_pi((__m64*)&row[2].x, minor2);
    _mm_storeh_pi((__m64*)&row[2].z, minor2);
    minor3 = _mm_mul_ps(det, minor3);
    _mm_storel_pi((__m64*)&row[3].x, minor3);
    _mm_storeh_pi((__m64*)&row[3].z, minor3);
}
#pragma warning(pop)

// FUNCTION: USA 0x000DD1CB
// PROTOTYPE: PREVIEW 0x004C8528 identical
// NAME: proto-pdb:default.pdb
void Matrix::Scaling(float scale)
{
    Zero();
    row[0].x = row[1].y = row[2].z = scale;
    row[3].w = 1.0f;
}

// FUNCTION: USA 0x000DD1EC
// PROTOTYPE: PREVIEW 0x004C8549 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 0.0, bytes mismatch; wall: inline-asm SSE, D116 (C0219): retail has no frame, esp arg reads, rows preloaded into xmm7..xmm4; intrinsics give an and esp,-16 ebp frame (C0211)
void Matrix::Apply(const Vector3& vec, Vector3& result) const
{
    __m128 r = _mm_add_ps(_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(_mm_set_ps1(vec.x), _mm_loadu_ps(&row[0].x)),
        _mm_mul_ps(_mm_set_ps1(vec.y), _mm_loadu_ps(&row[1].x))),
        _mm_mul_ps(_mm_set_ps1(vec.z), _mm_loadu_ps(&row[2].x))),
        _mm_loadu_ps(&row[3].x));
    _mm_store_ss(&result.x, r);
    _mm_store_ss(&result.y, _mm_shuffle_ps(r, r, 0x55));
    _mm_store_ss(&result.z, _mm_shuffle_ps(r, r, 0xAA));
}
#endif

// FUNCTION: USA 0x000DD24E
// PROTOTYPE: PREVIEW 0x004C85AB identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 26.13, bytes mismatch; wall: inline-asm SSE, D116 (C0219): retail has no frame, esp arg reads, rows preloaded into xmm7..xmm4; intrinsics give an and esp,-16 ebp frame (C0211)
void Matrix::Apply(const Vector4& vec, Vector4& result) const
{
    __m128 v = _mm_loadu_ps(&vec.x);
    _mm_storeu_ps(&result.x, _mm_add_ps(_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(_mm_shuffle_ps(v, v, 0x00), _mm_loadu_ps(&row[0].x)),
        _mm_mul_ps(_mm_shuffle_ps(v, v, 0x55), _mm_loadu_ps(&row[1].x))),
        _mm_mul_ps(_mm_shuffle_ps(v, v, 0xAA), _mm_loadu_ps(&row[2].x))),
        _mm_mul_ps(_mm_shuffle_ps(v, v, 0xFF), _mm_loadu_ps(&row[3].x))));
}
#endif

// FUNCTION: USA 0x000DD29C
// PROTOTYPE: PREVIEW 0x004C85F9 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 26.73, bytes mismatch; wall: inline-asm SSE, D116 (C0219): retail saves esi/edi twice, preloads rows into xmm7..xmm4 and pads the loop head with asm align bytes
void Matrix::BatchApply(const Vector3* src, Vector3* dest, int count) const
{
    __m128 r0 = _mm_loadu_ps(&row[0].x);
    __m128 r1 = _mm_loadu_ps(&row[1].x);
    __m128 r2 = _mm_loadu_ps(&row[2].x);
    __m128 r3 = _mm_loadu_ps(&row[3].x);
    for (; count != 0; count--) {
        __m128 r = _mm_add_ps(_mm_add_ps(_mm_add_ps(
            _mm_mul_ps(_mm_set_ps1(src->x), r0),
            _mm_mul_ps(_mm_set_ps1(src->y), r1)),
            _mm_mul_ps(_mm_set_ps1(src->z), r2)), r3);
        src++;
        _mm_store_ss(&dest->x, r);
        _mm_store_ss(&dest->y, _mm_shuffle_ps(r, r, 0x55));
        _mm_store_ss(&dest->z, _mm_shuffle_ps(r, r, 0xAA));
        dest++;
    }
}
#endif

// FUNCTION: USA 0x000DD325
// PROTOTYPE: PREVIEW 0x004C8682 identical
// NAME: proto-pdb:default.pdb
void Matrix::ApplyNormal(const Vector3& vec, Vector3& result) const
{
    // !FAKE: c2 x87 scheduling; dummy locals; retail loads vec.x, vec.y, vec.z onto the x87 stack first (match_one 59.88 -> 100.0)
    float v1 = vec.x, v2 = vec.y, v3 = vec.z;
    result.x = v1 * row[0].x + v2 * row[1].x + v3 * row[2].x;
    result.y = v1 * row[0].y + v2 * row[1].y + v3 * row[2].y;
    result.z = v1 * row[0].z + v2 * row[1].z + v3 * row[2].z;
}

// FUNCTION: USA 0x000DD37A
// PROTOTYPE: PREVIEW 0x004C86D7 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 9.72, bytes mismatch; wall: inline-asm SSE, D116 (C0219): retail has no frame, esp arg reads, m rows preloaded into xmm7..xmm4
void Matrix::Multiply(const Matrix& m, Matrix& result) const
{
    __m128 m0 = _mm_loadu_ps(&m.row[0].x);
    __m128 m1 = _mm_loadu_ps(&m.row[1].x);
    __m128 m2 = _mm_loadu_ps(&m.row[2].x);
    __m128 m3 = _mm_loadu_ps(&m.row[3].x);
    for (int v1 = 0; v1 < 4; v1++) {
        __m128 v = _mm_loadu_ps(&row[v1].x);
        _mm_storeu_ps(&result.row[v1].x, _mm_add_ps(_mm_add_ps(_mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(v, v, 0xAA), m2),
            _mm_mul_ps(_mm_shuffle_ps(v, v, 0x00), m0)),
            _mm_mul_ps(_mm_shuffle_ps(v, v, 0x55), m1)),
            _mm_mul_ps(_mm_shuffle_ps(v, v, 0xFF), m3)));
    }
}
#endif

// FUNCTION: USA 0x000DD46A
// PROTOTYPE: PREVIEW 0x004C87C7 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 13.875, bytes mismatch; wall: inline-asm SSE, D116 (C0219): retail has no frame, esp arg reads, rows preloaded into xmm7..xmm4; intrinsics give an and esp,-16 ebp frame (C0211)
void Matrix::Multiply(float s, Matrix& result) const
{
    __m128 sv = _mm_set_ps1(s);
    _mm_storeu_ps(&result.row[0].x, _mm_mul_ps(_mm_loadu_ps(&row[0].x), sv));
    _mm_storeu_ps(&result.row[1].x, _mm_mul_ps(_mm_loadu_ps(&row[1].x), sv));
    _mm_storeu_ps(&result.row[2].x, _mm_mul_ps(_mm_loadu_ps(&row[2].x), sv));
    _mm_storeu_ps(&result.row[3].x, _mm_mul_ps(_mm_loadu_ps(&row[3].x), sv));
}
#endif

// FUNCTION: USA 0x000DD4A5
// PROTOTYPE: PREVIEW 0x004C8802 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 95.31, bytes mismatch; x87 sum term order: cl reorders the three products by an internal key per row (term order, operand order, ref/pointer row locals tried)
void Matrix::AffineMultiply(const Matrix& m, Matrix& result) const
{
    // !FAKE: c2 x87 scheduling; dummy locals; the three sums stay on the x87 stack until the x, y, z stores (match_one 72.23 -> 95.31)
    {
        float v1 = row[0].x * m.row[0].x + row[0].y * m.row[1].x + row[0].z * m.row[2].x;
        float v2 = zb.x * up.x + zb.y * up.y + zb.z * up.z;
        float v3 = row[0].x * m.row[0].z + row[0].y * m.row[1].z + row[0].z * m.row[2].z;
        result.row[0].x = v1;
        result.row[0].y = v2;
        result.row[0].z = v3;
        result.row[0].w = 0.0f;
    }
    {
        float v1 = row[1].x * m.row[0].x + row[1].y * m.row[1].x + row[1].z * m.row[2].x;
        float v2 = row[1].x * m.row[0].y + row[1].y * m.row[1].y + row[1].z * m.row[2].y;
        float v3 = row[1].x * m.row[0].z + row[1].y * m.row[1].z + row[1].z * m.row[2].z;
        result.row[1].x = v1;
        result.row[1].y = v2;
        result.row[1].z = v3;
        result.row[1].w = 0.0f;
    }
    {
        float v1 = row[2].x * m.row[0].x + row[2].y * m.row[1].x + row[2].z * m.row[2].x;
        float v2 = row[2].x * m.row[0].y + row[2].y * m.row[1].y + row[2].z * m.row[2].y;
        float v3 = row[2].x * m.row[0].z + row[2].y * m.row[1].z + row[2].z * m.row[2].z;
        result.row[2].x = v1;
        result.row[2].y = v2;
        result.row[2].z = v3;
        result.row[2].w = 0.0f;
    }
    {
        float v1 = row[3].x * m.row[0].x + row[3].y * m.row[1].x + row[3].z * m.row[2].x + m.row[3].x;
        float v2 = row[3].x * m.row[0].y + row[3].y * m.row[1].y + row[3].z * m.row[2].y + m.row[3].y;
        float v3 = row[3].x * m.row[0].z + row[3].y * m.row[1].z + row[3].z * m.row[2].z + m.row[3].z;
        result.row[3].x = v1;
        result.row[3].y = v2;
        result.row[3].z = v3;
        result.row[3].w = 1.0f;
    }
}
#endif

// FUNCTION: USA 0x000DD6D6
// PROTOTYPE: PREVIEW 0x004C8A91 identical
// NAME: proto-pdb:default.pdb
void Matrix::AffineInvert()
{
    // !FAKE: c2 store order; reference locals and block-scoped temporary; a plain temp swap stores the x87 copy first (match_one 53.66 -> 85.37 -> 100.0)
    { float& v1 = row[0].y; float& v2 = row[1].x; float tmp = v1; v1 = v2; v2 = tmp; }
    { float& v1 = row[0].z; float& v2 = row[2].x; float tmp = v1; v1 = v2; v2 = tmp; }
    { float& v1 = row[1].z; float& v2 = row[2].y; float tmp = v1; v1 = v2; v2 = tmp; }
    Vector4 trans;
    trans.Set(-row[3].x, -row[3].y, -row[3].z, 1.0f);
    row[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
    Apply(trans, row[3]);
}

// FUNCTION: USA 0x000DD73E
// PROTOTYPE: PREVIEW 0x004C8AF9 identical
// NAME: proto-pdb:default.pdb
void Matrix::Translation(const Vector3& vec)
{
    memset(this, 0, sizeof(Vector4) * 3);
    row[2].z = 1.0f;
    row[1].y = 1.0f;
    row[0].x = 1.0f;
    row[3].x = vec.x;
    row[3].y = vec.y;
    row[3].z = vec.z;
    row[3].w = 1.0f;
}

// FUNCTION: USA 0x000DD776
// PROTOTYPE: PREVIEW 0x004C8B31 identical
// NAME: proto-pdb:default.pdb
void Matrix::Rotation(const Quaternion& q)
{
    // !FAKE: c2 x87 register allocation; neutral stand-ins; the PDB records no slot for the two squares retail keeps on the x87 stack
    const float v1 = q.v.x * q.v.x;
    const float xy = q.v.y * q.v.x;
    const float xz = q.v.z * q.v.x;
    const float xw = q.w * q.v.x;
    const float v2 = q.v.y * q.v.y;
    const float yz = q.v.z * q.v.y;
    const float yw = q.w * q.v.y;
    const float zz = q.v.z * q.v.z;
    const float zw = q.w * q.v.z;
    row[0].x = 1.0f - 2.0f * (v2 + zz);
    row[0].y = 2.0f * (xy + zw);
    row[0].z = 2.0f * (xz - yw);
    row[0].w = 0.0f;
    row[1].x = 2.0f * (xy - zw);
    row[1].y = 1.0f - 2.0f * (v1 + zz);
    row[1].z = 2.0f * (yz + xw);
    row[1].w = 0.0f;
    row[2].x = 2.0f * (xz + yw);
    row[2].y = 2.0f * (yz - xw);
    row[2].z = 1.0f - 2.0f * (v1 + v2);
    row[2].w = 0.0f;
    row[3].x = 0.0f;
    row[3].y = 0.0f;
    row[3].z = 0.0f;
    row[3].w = 1.0f;
}

// FUNCTION: USA 0x000DD86A
// PROTOTYPE: PREVIEW 0x004C8C25 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 62.96, bytes mismatch; wall: inline-asm fsincos, D116 (C0219): cl 13.10 /Oi emits fsin + fcos for sinf/cosf, never fsincos
void Matrix::Rotation(float xr, float yr, float zr)
{
    float sinX = sinf(xr), cosX = cosf(xr);
    float sinY = sinf(yr), cosY = cosf(yr);
    float sinZ = sinf(zr), cosZ = cosf(zr);
    row[0].x = cosZ * cosY;
    row[0].y = sinZ * cosY;
    row[0].z = -sinY;
    row[0].w = 0.0f;
    row[1].x = cosZ * sinY * sinX - sinZ * cosX;
    row[1].y = sinZ * sinY * sinX + cosZ * cosX;
    row[1].z = cosY * sinX;
    row[1].w = 0.0f;
    row[2].x = cosZ * sinY * cosX + sinZ * sinX;
    row[2].y = sinZ * sinY * cosX - cosZ * sinX;
    row[2].z = cosY * cosX;
    row[2].w = 0.0f;
    row[3].x = 0.0f;
    row[3].y = 0.0f;
    row[3].z = 0.0f;
    row[3].w = 1.0f;
}
#endif

// FUNCTION: USA 0x000DD92F
// PROTOTYPE: PREVIEW 0x004C8CEA identical
// NAME: proto-pdb:default.pdb
void Matrix::Mirror(const Vector4& plane)
{
    row[0].Set(plane.x, plane.y, plane.z, 0.0f);
    row[1].Set(plane.x, plane.y, plane.z, 0.0f);
    row[2].Set(plane.x, plane.y, plane.z, 0.0f);
    // !FAKE: inliner; hand form of Vector4::operator*=(float) through a pointer local; retail inlines it, /O1 keeps it out of line; row members CSE'd without the pointer (match_one 85.90 -> 93.40)
    { Vector4* p = &row[0]; float s = plane.x * -2.0f; p->x *= s; p->y *= s; p->z *= s; p->w *= s; }
    { Vector4* p = &row[1]; float s = plane.y * -2.0f; p->x *= s; p->y *= s; p->z *= s; p->w *= s; }
    { Vector4* p = &row[2]; float s = plane.z * -2.0f; p->x *= s; p->y *= s; p->z *= s; p->w *= s; }
    row[0].x += 1.0f;
    row[1].y += 1.0f;
    row[2].z += 1.0f;
    // !FAKE: inliner; hand form of Vector4::operator Vector3(); retail copies plane.x..z with rep movsd per conversion
    Vector3 v1; memcpy(&v1, &plane, sizeof(Vector3));
    Vector3 v2; memcpy(&v2, &plane, sizeof(Vector3));
    Vector3 v3; memcpy(&v3, &plane, sizeof(Vector3));
    // !FAKE: inliner; neutral stand-ins and mixed operand order; hand forms of the inlined Vector3 * float and dot, the operand order sets the x87 order and v2/v5 stack slots (match_one 98.04 -> 99.96 -> 100.0)
    float v4 = -plane.w;
    Vector3 v5(v3.x * v4, v3.y * v4, v3.z * v4);
    float v6 = 2.0f * (v2.x * v5.x + v5.y * v2.y + v5.z * v2.z);
    Vector3 v7(v1.x * v6, v1.y * v6, v1.z * v6);
    row[3].Set(v7);
    row[3].w = 1.0f;
}

// FUNCTION: USA 0x000DDB22
// PROTOTYPE: PREVIEW 0x004C8EDD identical
// NAME: proto-pdb:default.pdb
void Matrix::RotationAboutPivot(float xr, float yr, float zr, const Vector3& pivot)
{
    // !FAKE: inliner; hand form of Translation; retail inlines both Translation calls, /O1 keeps them out of line
    {
        memset(this, 0, sizeof(Vector4) * 3);
        row[2].z = 1.0f;
        row[1].y = 1.0f;
        row[0].x = 1.0f;
        Vector3 v1(-pivot.x, -pivot.y, -pivot.z);
        row[3].x = v1.x;
        row[3].y = v1.y;
        row[3].z = v1.z;
        row[3].w = 1.0f;
    }
    Matrix tmp;
    tmp.Rotation(xr, yr, zr);
    Multiply(tmp, *this);
    // !FAKE: inliner; hand form of Translation; see above
    memset(&tmp, 0, sizeof(Vector4) * 3);
    tmp.row[2].z = 1.0f;
    tmp.row[1].y = 1.0f;
    tmp.row[0].x = 1.0f;
    tmp.row[3].x = pivot.x;
    tmp.row[3].y = pivot.y;
    tmp.row[3].z = pivot.z;
    tmp.row[3].w = 1.0f;
    Multiply(tmp, *this);
}

// FUNCTION: USA 0x000DDBDA
// PROTOTYPE: PREVIEW 0x004C8F95 identical
// NAME: proto-pdb:default.pdb
void Matrix::TransformationAboutPivot(float xr, float yr, float zr, const Vector3& t, const Vector3& pivot)
{
    RotationAboutPivot(xr, yr, zr, pivot);
    Matrix tmp;
    tmp.Translation(t);
    Multiply(tmp, *this);
}

// FUNCTION: USA 0x000DDC1E
// PROTOTYPE: PREVIEW 0x004C8FD9 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 95.24, bytes mismatch; dot zb.up: retail sums z, x, y, cl here z, y, x (term order, operand order tried); stand-in keeps an extra v1.x store, a separate temporary costs a 0xC frame slot
void Matrix::View(const Vector3& from, const Vector3& to, const Vector3& up)
{
    // !FAKE: inliner; one Vector3 stand-in for the inlined to - from and zb * v2 temporaries (retail keeps both in one slot)
    Vector3 v1(to.x - from.x, to.y - from.y, to.z - from.z);
    Vector3 zb = Normalize(v1);
    float v2 = zb.x * up.x + zb.y * up.y + zb.z * up.z;
    v1.x = zb.x * v2;
    v1.y = zb.y * v2;
    v1.z = zb.z * v2;
    Vector3 yb;
    yb.x = up.x - v1.x;
    yb.y = up.y - v1.y;
    yb.z = up.z - v1.z;
    yb.Normalize();
    Vector3 xb;
    xb.x = yb.y * zb.z - yb.z * zb.y;
    xb.y = yb.z * zb.x - yb.x * zb.z;
    xb.z = yb.x * zb.y - yb.y * zb.x;
    row[0].Set(xb.x, yb.x, zb.x, 0.0f);
    row[1].Set(xb.y, yb.y, zb.y, 0.0f);
    row[2].Set(xb.z, yb.z, zb.z, 0.0f);
    row[3].Set(-(xb.x * from.x + xb.y * from.y + xb.z * from.z),
        -(yb.x * from.x + yb.y * from.y + yb.z * from.z),
        -(zb.x * from.x + zb.y * from.y + zb.z * from.z), 1.0f);
}
#endif

// FUNCTION: USA 0x000DDD76
// PROTOTYPE: PREVIEW 0x004C9131 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 84.65, bytes mismatch; cross product: retail keeps zb.x on the x87 stack and reads yb.z from memory, cl here swaps the two (operand order, locals, Vector4::Set forms tried)
void Matrix::Orient(const Vector3& dir, const Vector3& up)
{
    Vector3 zb = Normalize(dir);
    float tmp = zb.x * up.x + zb.y * up.y + zb.z * up.z;
    // !FAKE: c2 stack slots; Vector3 temporary; hand form of the inlined up - zb * tmp, retail stores the product before the subtraction
    Vector3 v1;
    v1.x = zb.x * tmp;
    v1.y = zb.y * tmp;
    v1.z = zb.z * tmp;
    Vector3 yb;
    yb.x = up.x - v1.x;
    yb.y = up.y - v1.y;
    yb.z = up.z - v1.z;
    yb.Normalize();
    float v2 = yb.y * zb.z - yb.z * zb.y;
    float v3 = yb.z * zb.x - yb.x * zb.z;
    row[0].x = v2;
    row[0].y = v3;
    row[0].z = yb.x * zb.y - yb.y * zb.x;
    row[0].w = 0.0f;
    row[1].z = yb.z;
    row[1].x = yb.x;
    row[1].y = yb.y;
    row[1].w = 0.0f;
    row[2].x = zb.x;
    row[2].y = zb.y;
    row[2].z = zb.z;
    row[2].w = 0.0f;
    row[3].x = 0.0f;
    row[3].y = 0.0f;
    row[3].z = 0.0f;
    row[3].w = 1.0f;
}
#endif
