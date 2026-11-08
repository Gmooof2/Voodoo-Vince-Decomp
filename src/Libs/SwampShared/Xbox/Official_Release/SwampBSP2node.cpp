// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampBSP2node.cpp OBJ: Xbox\Official_Release\SwampBSP2node.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float LengthSquared() const { return x * x + y * y + z * z; }
    Vector3& operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector3& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    Vector4() {}

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
    float w; // 0x0C

    operator Vector3() const { return *(const Vector3*)this; }
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

Vector3 Normalize(const Vector3&);                                    // NAME: proto-pdb:default.pdb
Vector4 GetPlaneEquation(const Vector3&, const Vector3&, const Vector3&); // NAME: proto-pdb:default.pdb
float FastReciprocalSquareRoot(float);                                // NAME: proto-pdb:default.pdb

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

    int _RefCount; // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int _tag;        // 0x04
    float _pct;               // 0x08
    const void* _blindData;   // 0x0C
    Vector4 _plane;           // 0x10
    char field_20[0x40 - 0x20]; // 0x20
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
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

    T* _Object; // 0x00
};
// !FAKE: linker COMDAT selection; explicit specialization with the retail body (C0233 Fix 1); emits this unit's alias of the fold-VA row
// ??4?$Ptr@VCollisionResultObject@@... 0x000B1EC6 (SwampCollide.cpp) as a byte-equal COMDAT; inline gives it pick-any selection (relink same() refuses NODUPLICATES)
template <> inline Ptr<BSPQueryResultObject>& Ptr<BSPQueryResultObject>::operator=(const Ptr<BSPQueryResultObject>& p)
{
    if (_Object)
        _Object->SubRef();
    _Object = p._Object;
    if (_Object)
        _Object->AddRef();
    return *this;
}

class BSPResultManager { // NAME: proto-pdb:default.pdb
public:
    static Ptr<BSPQueryResultObject> GetNewResult();
};

struct BSPTriangleInfo { // NAME: proto-pdb:default.pdb
    unsigned int pt[3];  // 0x00
    unsigned int plane;  // 0x0C
    unsigned int tag;    // 0x10
};
DECOMP_SIZE_ASSERT(BSPTriangleInfo, 0x14);

class BSPnode;

class BSP { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x24];                       // 0x00
    const Vector4* _planes;                   // 0x24
    const BSPTriangleInfo* _triangleList;     // 0x28
    const Vector3* _vertexList;               // 0x2C
    const unsigned int* _triangleIndices;     // 0x30
    const BSPnode* _nodes;                    // 0x34
    char field_38[0x40 - 0x38];               // 0x38
};
DECOMP_SIZE_ASSERT(BSP, 0x40);

class BSPnode { // NAME: proto-pdb:default.pdb
public:
    struct CheckInfo { // NAME: proto-pdb:default.pdb
        CheckInfo(const BSP& bsp_, float radius_, unsigned int ignoreMask_, const Vector3& start, const Vector3& end);

        const BSP& bsp;           // 0x00
        float radius;             // 0x04
        unsigned int ignoreMask;  // 0x08
        const Vector3& origStart; // 0x0C
        const Vector3& origEnd;   // 0x10
    };

    Ptr<BSPQueryResultObject> CheckLine(const Vector3& start, const Vector3& end, const CheckInfo& info) const;
    Ptr<BSPQueryResultObject> CheckSphere(const Vector3& start, const Vector3& end, const CheckInfo& info) const;
    const unsigned int* GetTriangleIndices(const BSP& bsp) const;
    unsigned int GetNumTris() const { return _numTris; }

protected:
    bool HasFrontNode() const { return _flags != 0; }
    const BSPnode* GetFrontNode() const { return this + 1; }
    bool HasBackNode() const { return _back != 0; }
    const BSPnode* GetBackNode(const BSP& bsp) const { return &bsp._nodes[_back]; }
    const Vector4& GetPlane(const BSP& bsp) const;
    bool PointInTriangle(const Vector3&, const Vector3&, const Vector3&, const Vector3&, const Vector3&) const;
    unsigned int HitTag(const Vector3& loc, const CheckInfo& info) const;

    struct SphereHitResult { // NAME: proto-pdb:default.pdb
        unsigned int tag; // 0x00
        float pct;        // 0x04
    };
    SphereHitResult SphereHitTag(const Vector3& start, const Vector3& end, const CheckInfo& info) const;
    Ptr<BSPQueryResultObject> HitMe(float pct, unsigned int tag, const Vector4& plane) const;

public:
    unsigned short _numTris;  // 0x00
    unsigned short _flags;    // 0x02
    unsigned int _offsetTris; // 0x04
    unsigned int _back;       // 0x08
};
DECOMP_SIZE_ASSERT(BSPnode, 0xC);
DECOMP_SIZE_ASSERT(BSPnode::CheckInfo, 0x14);

// FUNCTION: USA 0x000DF591
// PROTOTYPE: PREVIEW 0x004CA968 identical
// NAME: proto-pdb:default.pdb
BSPnode::CheckInfo::CheckInfo(const BSP& bsp_, float radius_, unsigned int ignoreMask_, const Vector3& start, const Vector3& end)
    : bsp(bsp_), radius(radius_), ignoreMask(ignoreMask_), origStart(start), origEnd(end)
{
}

// FUNCTION: USA 0x000DF5B7
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.66, bytes mismatch; one scheduling gap: retail pops esi at the end of the first
// cross-product block, ours right after pop edi (operand order fixed by the v4 ref and the v6 float stand-in)
bool BSPnode::PointInTriangle(const Vector3& pt, const Vector3& dir, const Vector3& v0, const Vector3& v1,
                              const Vector3& v2) const
{
    Vector3 spokes[3];
    // !FAKE: inliner; hand form of the inlined Vector3 subtraction via the PDB ctor; no PDB record of the operator, no before/after recorded
    spokes[0] = Vector3(v0.x - pt.x, v0.y - pt.y, v0.z - pt.z);
    spokes[1] = Vector3(v1.x - pt.x, v1.y - pt.y, v1.z - pt.z);
    spokes[2] = Vector3(v2.x - pt.x, v2.y - pt.y, v2.z - pt.z);
    // !FAKE: inliner; hand form of the inlined cross and dot products; no PDB record of the operators, no before/after recorded
    {
        const Vector3& v4 = spokes[1];
        Vector3 v3(spokes[0].y * v4.z - spokes[0].z * v4.y,
                   spokes[0].z * v4.x - spokes[0].x * v4.z,
                   spokes[0].x * v4.y - spokes[0].y * v4.x);
        float v6 = (v3.x * dir.x + v3.y * dir.y) + v3.z * dir.z;
        if (v6 < 0.0f)
            return false;
    }
    {
        const Vector3& v4 = spokes[2];
        Vector3 v3(spokes[1].y * v4.z - spokes[1].z * v4.y,
                   spokes[1].z * v4.x - spokes[1].x * v4.z,
                   spokes[1].x * v4.y - spokes[1].y * v4.x);
        if ((v3.x * dir.x + v3.y * dir.y) + v3.z * dir.z < 0.0f)
            return false;
    }
    {
        const Vector3& v4 = spokes[0];
        Vector3 v3(spokes[2].y * v4.z - spokes[2].z * v4.y,
                   spokes[2].z * v4.x - spokes[2].x * v4.z,
                   spokes[2].x * v4.y - spokes[2].y * v4.x);
        if ((v3.x * dir.x + v3.y * dir.y) + v3.z * dir.z < 0.0f)
            return false;
    }
    return true;
}
#endif

// FUNCTION: USA 0x000DF731
// PROTOTYPE: PREVIEW 0x004CAB08 identical
// NAME: proto-pdb:default.pdb
const unsigned int* BSPnode::GetTriangleIndices(const BSP& bsp) const
{
    if (_numTris == 1)
        return &_offsetTris;
    return &bsp._triangleIndices[_offsetTris];
}

// FUNCTION: USA 0x000DF74C
// PROTOTYPE: PREVIEW 0x004CAB23 identical
// NAME: proto-pdb:default.pdb
const Vector4& BSPnode::GetPlane(const BSP& bsp) const
{
    return bsp._planes[bsp._triangleList[*GetTriangleIndices(bsp)].plane];
}

// FUNCTION: USA 0x000DF76B
// PROTOTYPE: PREVIEW 0x004CAB42 identical
// NAME: proto-pdb:default.pdb
Ptr<BSPQueryResultObject> BSPnode::HitMe(float pct, unsigned int tag, const Vector4& plane) const
{
    Ptr<BSPQueryResultObject> result = BSPResultManager::GetNewResult();
    result->_plane = plane;
    result->_pct = pct;
    result->_tag = tag;
    if (result->_tag == 0)
        result->_tag = (unsigned int)-1;
    return result;
}

// FUNCTION: USA 0x000DF7BC
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match only while PointInTriangle (0x000DF5B7) is compiled above it (C0249:
// retail keeps the triangle pointer in edx across that call); guarded with PointInTriangle at 98.66, so this follows
unsigned int BSPnode::HitTag(const Vector3& loc, const CheckInfo& info) const
{
    // !FAKE: c1xx front end; neutral stand-ins; the PDB records no local, retail reads bsp and ignoreMask once
    const BSP& v0 = info.bsp;
    const unsigned int* triIndices = GetTriangleIndices(v0);
    const Vector4& plane = v0._planes[v0._triangleList[*triIndices].plane];
    for (unsigned int i = 0; i < GetNumTris(); i++) {
        const BSPTriangleInfo& v1 = v0._triangleList[triIndices[i]];
        if (v1.tag & info.ignoreMask)
            continue;
        if (PointInTriangle(loc, plane, v0._vertexList[v1.pt[0]], v0._vertexList[v1.pt[1]],
                            v0._vertexList[v1.pt[2]]))
            return v1.tag;
    }
    return 0;
}
#endif

// FUNCTION: USA 0x000DF873
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.90, bytes mismatch; the 0x44-0x67 frame slots (retail pct 0x44, inner-counter spill 0x48,
// result 0x4c, t 0x54, pctEdge 0x60, bInside 0x67) and one x87 operand order (closest.y: retail equation.y * v10 load order)
BSPnode::SphereHitResult BSPnode::SphereHitTag(const Vector3& start, const Vector3& end, const CheckInfo& info) const
{
    SphereHitResult result;
    const unsigned int* triIndices = GetTriangleIndices(info.bsp);
    // !FAKE: c1xx front end; neutral stand-in; the PDB records no local for the node plane
    const Vector4& v0 = info.bsp._planes[info.bsp._triangleList[*triIndices].plane];
    float startY = v0.x * start.x + v0.y * start.y + v0.z * start.z + v0.w;
    result.pct = 3.402823466e+38f;
    float endY = v0.x * end.x + v0.y * end.y + v0.z * end.z + v0.w;
    result.tag = 0;
    for (unsigned int t = 0; t < GetNumTris(); t++) {
        const BSPTriangleInfo& tri = info.bsp._triangleList[triIndices[t]];
        if (tri.tag & info.ignoreMask)
            continue;
        const Vector3* v[3];
        for (unsigned int v1 = 0; v1 < 3; v1++)
            v[v1] = &info.bsp._vertexList[tri.pt[v1]];
        float pct = -3.402823466e+38f;
        bool bInside = true;
        for (unsigned int v1 = 0; v1 < 3; v1++) {
            // !FAKE: inliner; hand form of the inlined Vector3 operator+ via the PDB ctor; stand-in v2
            Vector3 v2 = v0;
            Vector4 edgePlane = GetPlaneEquation(*v[v1], Vector3(v2.x + v[v1]->x, v2.y + v[v1]->y, v2.z + v[v1]->z),
                                                 *v[(v1 + 1) % 3]);
            const float startX = ((edgePlane.x * start.x + edgePlane.y * start.y) + edgePlane.z * start.z) + edgePlane.w;
            // !FAKE: c2 x87 operand order; named first term v4; end dot x term first (match_one 99.31 -> 99.60)
            float v4 = edgePlane.x * end.x;
            float v3 = ((v4 + edgePlane.y * end.y) + edgePlane.z * end.z) + edgePlane.w;
            if (startX < -info.radius && v3 < -info.radius) {
                pct = 3.402823466e+38f;
                break;
            }
            Vector3 delta(v3 - startX, endY - startY, 0.0f);
            Vector3 dir = Normalize(delta);
            Vector3 equation(dir.y, -dir.x, 0.0f);
            // !FAKE: c2 x87 operand order; named y term v5; equation.z y term first (match_one 99.60 -> 99.90)
            float v5 = startY * equation.y;
            equation.z = -(v5 + equation.x * startX);
            // !FAKE: inliner; stand-in v10 for the scalar argument of the inlined Vector3 * float; no before/after recorded
            float v10 = -equation.z;
            Vector3 closest(equation.x * v10, equation.y * v10, equation.z * v10);
            float pctEdge;
            if (closest.x >= 0.0f) {
                pctEdge = (startY - info.radius) * (-1.0f / delta.y);
                if (startX + delta.x * pctEdge >= -info.radius) {
                    if (pctEdge > pct)
                        pct = pctEdge;
                    continue;
                }
            }
            float offsetSq = info.radius * info.radius - equation.z * equation.z;
            if (offsetSq < 0.0f) {
                pct = 3.402823466e+38f;
                break;
            }
            // !FAKE: inliner; hand form of the inlined Vector3 operators; stand-in v11 (only y is read)
            Vector3 v11(closest.x, closest.y - FastReciprocalSquareRoot(offsetSq) * offsetSq * dir.y, 0.0f);
            pctEdge = (startY - v11.y) / (startY - endY);
            if (pctEdge > 1.0f) {
                pct = 3.402823466e+38f;
                break;
            }
            if (pctEdge < 0.0f) {
                pctEdge = 0.0f;
                if (startX < 0.0f && dir.x * startX + dir.y * startY + dir.z * 0.0f > 0.0f) {
                    pct = 3.402823466e+38f;
                    break;
                }
            }
            bInside = false;
            if (pctEdge > pct)
                pct = pctEdge;
        }
        if (pct >= result.pct)
            continue;
        {
            if (!bInside) {
                // !FAKE: inliner; hand form of the inlined Vector3 operators via the PDB ctor; stand-ins v4, v5
                Vector3 v4(end.x - start.x, end.y - start.y, end.z - start.z);
                Vector3 v5(v4.x * pct, v4.y * pct, v4.z * pct);
                Vector3 pt(start.x + v5.x, start.y + v5.y, start.z + v5.z);
                Vector3 center(0.0f, 0.0f, 0.0f);
                for (unsigned int v1 = 0; v1 < 3; v1++)
                    center += *v[v1];
                center *= 1.0f / 3.0f;
                bool v8 = true;
                for (unsigned int v1 = 0; v1 < 3; v1++) {
                    Vector3 v6(pt.x - v[v1]->x, pt.y - v[v1]->y, pt.z - v[v1]->z);
                    if (v6.LengthSquared() <= info.radius * info.radius * 1.05f)
                        goto v9;
                    Vector3 v7(center.x - v[v1]->x, center.y - v[v1]->y, center.z - v[v1]->z);
                    if (v6.x * v7.x + v6.y * v7.y + v6.z * v7.z < 0.0f)
                        v8 = false;
                }
                if (!v8)
                    continue;
            }
        v9:
            {
                result.pct = pct;
                result.tag = tri.tag;
                if (pct <= 0.0f) {
                    result.pct = 0.0f;
                    break;
                }
            }
        }
    }
    return result;
}
#endif

// FUNCTION: USA 0x000DFCFE
// NAME: proto-pdb:default.pdb
Ptr<BSPQueryResultObject> BSPnode::CheckLine(const Vector3& start, const Vector3& end, const CheckInfo& info) const
{
    // !FAKE: inliner; hand form of the inlined plane distances (no PDB record of the operator); stand-ins v0-v3
    const BSP& v0 = info.bsp;
    const Vector4& v1 = GetPlane(v0);
    float v2 = v1.x * start.x + v1.y * start.y + v1.z * start.z + v1.w;
    float v3 = v1.x * end.x + v1.y * end.y + v1.z * end.z + v1.w;
    float v4 = v2 - v3;
    bool v5 = v4 == 0.0f;
    if ((v2 > 0.01f && v3 > 0.01f) || (v5 && v2 >= 0.0f)) {
        if (!HasFrontNode())
            return BSPResultManager::GetNewResult();
        return GetFrontNode()->CheckLine(start, end, info);
    }
    if ((v2 < -0.01f && v3 < -0.01f) || v5) {
        if (!HasBackNode())
            return BSPResultManager::GetNewResult();
        return GetBackNode(v0)->CheckLine(start, end, info);
    }
    float pct = v2 / v4;
    // !FAKE: inliner; hand form of the inlined Vector3 operators via the PDB ctor; no before/after recorded
    Vector3 v6(end.x - start.x, end.y - start.y, end.z - start.z);
    Vector3 v7(v6.x * pct, v6.y * pct, v6.z * pct);
    Vector3 crosspoint(start.x + v7.x, start.y + v7.y, start.z + v7.z);
    const bool frontToBack = v2 > v3;
    const BSPnode* first;
    const BSPnode* second;
    if (frontToBack) {
        first = HasFrontNode() ? GetFrontNode() : 0;
        second = HasBackNode() ? GetBackNode(v0) : 0;
    } else {
        first = HasBackNode() ? GetBackNode(v0) : 0;
        second = HasFrontNode() ? GetFrontNode() : 0;
    }
    if (first) {
        Ptr<BSPQueryResultObject> result = first->CheckLine(start, crosspoint, info);
        if (result->_tag) {
            result->_pct *= pct;
            return result;
        }
    }
    if (frontToBack) {
        unsigned int tag = HitTag(crosspoint, info);
        if (tag)
            return HitMe(pct, tag, v1);
    }
    if (second) {
        Ptr<BSPQueryResultObject> result = second->CheckLine(crosspoint, end, info);
        if (result->_tag)
            result->_pct = pct + (1.0f - pct) * result->_pct;
        return result;
    }
    return BSPResultManager::GetNewResult();
}

// FUNCTION: USA 0x000DFF65
// NAME: proto-pdb:default.pdb
Ptr<BSPQueryResultObject> BSPnode::CheckSphere(const Vector3& start, const Vector3& end, const CheckInfo& info) const
{
    Ptr<BSPQueryResultObject> bestResult = BSPResultManager::GetNewResult();
    const Vector4& plane = GetPlane(info.bsp);
    float sr = plane.x * start.x + plane.y * start.y + plane.z * start.z + plane.w;
    float er = plane.x * end.x + plane.y * end.y + plane.z * end.z + plane.w;
    if (sr > info.radius && er > info.radius) {
        if (!HasFrontNode())
            return bestResult;
        return GetFrontNode()->CheckSphere(start, end, info);
    }
    if (sr < -info.radius && er < -info.radius) {
        if (!HasBackNode())
            return bestResult;
        return GetBackNode(info.bsp)->CheckSphere(start, end, info);
    }
    if (sr > er && sr > 0.0f) {
        SphereHitResult shr = SphereHitTag(start, end, info);
        if (shr.tag && bestResult->_pct >= shr.pct)
            bestResult = HitMe(shr.pct, shr.tag, plane);
    }
    if (HasFrontNode()) {
        if (sr > er) {
            if (er < -info.radius) {
                float pct = (sr + info.radius) / (sr - er);
                // !FAKE: inliner; hand form of the inlined Vector3 operators via the PDB ctor; stand-ins v0, v1
                Vector3 v0(end.x - start.x, end.y - start.y, end.z - start.z);
                Vector3 v1(v0.x * pct, v0.y * pct, v0.z * pct);
                Vector3 trimpoint(start.x + v1.x, start.y + v1.y, start.z + v1.z);
                Ptr<BSPQueryResultObject> result = GetFrontNode()->CheckSphere(start, trimpoint, info);
                if (result->_tag) {
                    result->_pct *= pct;
                    if (bestResult->_pct > result->_pct)
                        bestResult = result;
                }
            } else {
                Ptr<BSPQueryResultObject> result = GetFrontNode()->CheckSphere(start, end, info);
                if (result->_tag && bestResult->_pct > result->_pct)
                    bestResult = result;
            }
        } else {
            if (sr < -info.radius) {
                float pct = (sr + info.radius) / (sr - er);
                // !FAKE: inliner; hand form of the inlined Vector3 operators via the PDB ctor; stand-ins v0, v1
                Vector3 v0(end.x - start.x, end.y - start.y, end.z - start.z);
                Vector3 v1(v0.x * pct, v0.y * pct, v0.z * pct);
                Vector3 trimpoint(start.x + v1.x, start.y + v1.y, start.z + v1.z);
                Ptr<BSPQueryResultObject> result = GetFrontNode()->CheckSphere(trimpoint, end, info);
                if (result->_tag) {
                    result->_pct = pct + (1.0f - pct) * result->_pct;
                    if (bestResult->_pct > result->_pct)
                        bestResult = result;
                }
            } else {
                Ptr<BSPQueryResultObject> result = GetFrontNode()->CheckSphere(start, end, info);
                if (result->_tag && bestResult->_pct > result->_pct)
                    bestResult = result;
            }
        }
    }
    if (HasBackNode()) {
        if (sr > er) {
            if (sr > info.radius) {
                float pct = (sr - info.radius) / (sr - er);
                // !FAKE: inliner; hand form of the inlined Vector3 operators via the PDB ctor; stand-ins v0, v1
                Vector3 v0(end.x - start.x, end.y - start.y, end.z - start.z);
                Vector3 v1(v0.x * pct, v0.y * pct, v0.z * pct);
                Vector3 trimpoint(start.x + v1.x, start.y + v1.y, start.z + v1.z);
                Ptr<BSPQueryResultObject> result = GetBackNode(info.bsp)->CheckSphere(trimpoint, end, info);
                if (result->_tag) {
                    result->_pct = pct + (1.0f - pct) * result->_pct;
                    if (bestResult->_pct > result->_pct)
                        bestResult = result;
                }
            } else {
                Ptr<BSPQueryResultObject> result = GetBackNode(info.bsp)->CheckSphere(start, end, info);
                if (result->_tag && bestResult->_pct > result->_pct)
                    bestResult = result;
            }
        } else {
            if (er > info.radius) {
                float pct = (sr - info.radius) / (sr - er);
                // !FAKE: inliner; hand form of the inlined Vector3 operators via the PDB ctor; stand-ins v0, v1
                Vector3 v0(end.x - start.x, end.y - start.y, end.z - start.z);
                Vector3 v1(v0.x * pct, v0.y * pct, v0.z * pct);
                Vector3 trimpoint(start.x + v1.x, start.y + v1.y, start.z + v1.z);
                Ptr<BSPQueryResultObject> result = GetBackNode(info.bsp)->CheckSphere(start, trimpoint, info);
                if (result->_tag) {
                    result->_pct *= pct;
                    if (bestResult->_pct > result->_pct)
                        bestResult = result;
                }
            } else {
                Ptr<BSPQueryResultObject> result = GetBackNode(info.bsp)->CheckSphere(start, end, info);
                if (result->_tag && bestResult->_pct > result->_pct)
                    bestResult = result;
            }
        }
    }
    return bestResult;
}
