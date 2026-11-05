// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampGeometry.cpp OBJ: Xbox\Official_Release\SwampGeometry.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float Length() const;

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
    Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    // !FAKE: inliner; hand form of the PDB inline conversion operator
    operator Vector3() const { return *(const Vector3*)this; }

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
    float w; // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

Vector3 Normalize(const Vector3&);
Vector4 GetPlaneEquation(const Vector3&, const Vector3&, const Vector3&);

// FUNCTION: USA 0x000DE0F6
// PROTOTYPE: PREVIEW 0x004C94B1 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 89.74, x87 operand order in the cross y term (fld mem vs fld st) and post-call store schedule
Vector4 GetPlaneEquation(const Vector3& a, const Vector3& b, const Vector3& c)
{
    // !FAKE: inliner; hand form of the Vector3 difference and cross product (no free operator in the PDB): stand-ins v1, v2
    Vector3 v1(c.x - a.x, c.y - a.y, c.z - a.z);
    const Vector3& v2 = Vector3(b.x - a.x, b.y - a.y, b.z - a.z);
    Vector3 normal = Normalize(Vector3(v2.y * v1.z - v2.z * v1.y, v2.z * v1.x - v2.x * v1.z, v2.x * v1.y - v2.y * v1.x));
    Vector4 equation(normal);
    equation.w = -(normal.z * a.z + normal.x * a.x + normal.y * a.y);
    return equation;
}
#endif

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

struct Matrix { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3&, Vector3&) const;

    Vector4 row[4]; // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

// FUNCTION: USA 0x000DE1FC
// PROTOTYPE: PREVIEW 0x004C95B7 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 91.47, frame 0x58 vs 0x40 (stand-in v2 and planePt1 not packed like retail's inlined Apply temps), dot term order fixed x,y,z; T16.c11: by-value Apply inline stays out of line, declaration order (31) and block scopes do not reach 0x40
Vector4 TransformPlane(const Matrix& m, const Vector4& plane)
{
    Vector3 normal0 = plane;
    float d = -plane.w;
    // !FAKE: inliner; hand form of the Vector3 scale, sum and difference (no free operator in the PDB): stand-ins d, v1
    Vector3 planePt0(normal0.x * d, normal0.y * d, normal0.z * d);
    Vector3 normalPt0(planePt0.x + normal0.x, planePt0.y + normal0.y, planePt0.z + normal0.z);
    // !FAKE: inliner; hand form of the by-value Matrix::Apply (out of line in our /O1 build, D117): stand-in v2
    Vector3 v2;
    m.Apply(planePt0, v2);
    Vector3 planePt1 = v2;
    m.Apply(normalPt0, v2);
    Vector3 normalPt1 = v2;
    const Vector3& v1 = Vector3(normalPt1.x - planePt1.x, normalPt1.y - planePt1.y, normalPt1.z - planePt1.z);
    Vector3 normal1 = Normalize(v1);
    Vector4 result(normal1);
    result.w = -(normal1.y * planePt1.y + normal1.z * planePt1.z + planePt1.x * normal1.x);
    return result;
}
#endif

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

struct Frustum { // NAME: proto-pdb:default.pdb
    bool IsVisible(const AABox&, float) const;
    bool IsVisible(const Vector3*, int, float) const;
    void CreateEmptyInverse(const Frustum&);
    void TrimFrustum(const Frustum&, const Vector3&, const Vector3*, unsigned int);

    Vector4 _Plane[5]; // 0x00
};
DECOMP_SIZE_ASSERT(Frustum, 0x50);

// FUNCTION: USA 0x000DE2E2
// PROTOTYPE: PREVIEW 0x004C969D identical
// NAME: proto-pdb:default.pdb
bool Frustum::IsVisible(const Vector3* points, int num_points, float farPlaneDist) const
{
    for (int i = 0; i < 5; i++) {
        // !FAKE: c2 register allocation; stand-in v1 for the point counter (no PDB local)
        int v1;
        for (v1 = 0; v1 < num_points; v1++) {
            if (points[v1].z * _Plane[i].z + points[v1].x * _Plane[i].x + points[v1].y * _Plane[i].y + _Plane[i].w > 0.0f)
                break;
        }
        if (v1 == num_points)
            return false;
    }
    if (farPlaneDist != 0.0f) {
        // !FAKE: c2 register allocation; stand-in v2 for the point counter (no PDB local)
        int v2;
        for (v2 = 0; v2 < num_points; v2++) {
            // !FAKE: c1xx front end; plane reference v5 and point pointer v4 (operand kind sets the x87 dot-term order:
            // with plain indexing in both loops the y term flips, kind sweep T16.c11 99.44 -> 100.0)
            const Vector4& v5 = _Plane[4];
            const Vector3* v4 = &points[v2];
            if (v4->z * v5.z + v4->x * v5.x + v4->y * v5.y + v5.w < farPlaneDist)
                break;
        }
        if (v2 == num_points)
            return false;
    }
    return true;
}

// FUNCTION: USA 0x000DE394
// PROTOTYPE: PREVIEW 0x004C974F identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 68.22, wall: retail's radius is an inline-asm sqrt (rsqrtss + fcompi/fcmovbe/ffree, no SSE realignment), D116 forbids __asm
bool Frustum::IsVisible(const AABox& bounds, float farPlaneDist) const
{
    // !FAKE: inliner; hand form of the Vector3 sum, scale and difference (no free operator in the PDB): stand-ins v1, v2, v3, v4
    Vector3 v1(bounds.high.x + bounds.low.x, bounds.high.y + bounds.low.y, bounds.high.z + bounds.low.z);
    Vector3 center(v1.x * 0.5f, v1.y * 0.5f, v1.z * 0.5f);
    Vector3 v2(bounds.high.x - center.x, bounds.high.y - center.y, bounds.high.z - center.z);
    float v3 = v2.Length();
    float v4;
    for (int i = 0; i < 5; i++) {
        v4 = center.z * _Plane[i].z + center.x * _Plane[i].x + center.y * _Plane[i].y + _Plane[i].w;
        if (v4 <= -v3)
            return false;
    }
    if (farPlaneDist != 0.0f && !(v4 < v3 + farPlaneDist))
        return false;
    return true;
}
#endif

// FUNCTION: USA 0x000DE491
// PROTOTYPE: PREVIEW 0x004C984C identical
// NAME: proto-pdb:default.pdb
void Frustum::CreateEmptyInverse(const Frustum& maxFrustum)
{
    // !FAKE: inliner; hand form of the Vector4 negation (no unary operator in the PDB)
    _Plane[0] = Vector4(-maxFrustum._Plane[2].x, -maxFrustum._Plane[2].y, -maxFrustum._Plane[2].z, -maxFrustum._Plane[2].w);
    _Plane[1] = Vector4(-maxFrustum._Plane[3].x, -maxFrustum._Plane[3].y, -maxFrustum._Plane[3].z, -maxFrustum._Plane[3].w);
    _Plane[2] = Vector4(-maxFrustum._Plane[0].x, -maxFrustum._Plane[0].y, -maxFrustum._Plane[0].z, -maxFrustum._Plane[0].w);
    _Plane[3] = Vector4(-maxFrustum._Plane[1].x, -maxFrustum._Plane[1].y, -maxFrustum._Plane[1].z, -maxFrustum._Plane[1].w);
    _Plane[4] = maxFrustum._Plane[4];
}

// FUNCTION: USA 0x000DE552
// PROTOTYPE: PREVIEW 0x004C990D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 68.0, regalloc: retail keeps only the x dot term across the recompute, plane ptrs in ecx/edx, &pt in ebx
void Frustum::TrimFrustum(const Frustum& maxFrustum, const Vector3& camPos, const Vector3* pts, unsigned int numPts)
{
    // !FAKE: c2 register allocation; stand-ins v1, v2 for the point and plane counters (no PDB local)
    for (unsigned int v1 = 0; v1 < numPts; v1++) {
        // !FAKE: c1xx front end; reference stand-in p to the point (operand order of the dot terms)
        const Vector3& p = pts[v1];
        for (int v2 = 0; v2 < 4; v2++) {
            if (p.x * maxFrustum._Plane[v2].x + p.y * maxFrustum._Plane[v2].y + p.z * maxFrustum._Plane[v2].z + maxFrustum._Plane[v2].w <= 0.0f) {
                _Plane[v2] = maxFrustum._Plane[v2];
                continue;
            }
            if (p.x * _Plane[v2].x + (p.y * _Plane[v2].y + p.z * _Plane[v2].z) + _Plane[v2].w < -0.00001) {
                // !FAKE: inliner; hand form of the Vector3 scale, difference, sum and cross product (no free operator in the PDB): stand-ins v3..v7
                Vector3 v3 = _Plane[v2];
                float v4 = p.z * _Plane[v2].z + p.y * _Plane[v2].y + p.x * _Plane[v2].x + _Plane[v2].w;
                Vector3 v5(v3.x * v4, v3.y * v4, v3.z * v4);
                Vector3 projPt(p.x - v5.x, p.y - v5.y, p.z - v5.z);
                Vector3 v6(projPt.x - camPos.x, projPt.y - camPos.y, projPt.z - camPos.z);
                Vector3 v7(p.x - camPos.x, p.y - camPos.y, p.z - camPos.z);
                Vector3 rotVec(v6.z * v7.y - v7.z * v6.y, v7.z * v6.x - v7.x * v6.z, v7.x * v6.y - v6.x * v7.y);
                _Plane[v2] = GetPlaneEquation(camPos, Vector3(rotVec.x + camPos.x, rotVec.y + camPos.y, rotVec.z + camPos.z), p);
            }
        }
    }
}
#endif
