// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampDebugGeometry.cpp OBJ: Xbox\Official_Release\SwampDebugGeometry.obj OPT: /O1 /Oy
// !FAKE: c2 intrinsics; file-scope pragma; retail inlines fsin/fcos in SwampDebugGeometry::AddSphere under /O1 /Oy;
// the CRT sin/cos declared first so the pragma precedes <math.h> (its sin(float) inline then gets fsin, else call _sin)
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
#pragma intrinsic(sin, cos)
#include <math.h>
#include "../../../../include/decomp.h"

inline void* __cdecl operator new(unsigned int, void* _Where) throw() { return (_Where); }
void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    unsigned int GetRGB() const;
    void Normalize();

    float x;                          // 0x00
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
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    void Rotation(const Quaternion&);
    void BatchApply(const Vector3*, Vector3*, int) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

enum _D3DPRIMITIVETYPE { // NAME: proto-pdb:default.pdb
    D3DPT_POINTLIST = 1,
    D3DPT_LINELIST = 2,
    D3DPT_LINELOOP = 3,
    D3DPT_LINESTRIP = 4,
    D3DPT_TRIANGLELIST = 5,
    D3DPT_TRIANGLESTRIP = 6,
    D3DPT_TRIANGLEFAN = 7,
    D3DPT_QUADLIST = 8,
    D3DPT_QUADSTRIP = 9,
    D3DPT_POLYGON = 10,
    D3DPT_MAX = 11
};

struct D3DVertexBuffer; // NAME: proto-pdb:default.pdb
struct D3DIndexBuffer; // NAME: proto-pdb:default.pdb
struct Vertex; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();
    void RemoveTreeFromScene();
    const Vector3& SetLocation(const Vector3&);
    const Quaternion& SetRotation(const Quaternion&);
    void SetOption(unsigned int, bool, bool);

    char field_4[0xA4 - 0x4];          // 0x04
};

class GeometryNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    GeometryNode();
    virtual ~GeometryNode();
    void SetVertices(const Vertex*, int);
    void SetIndices(const unsigned short*, int);
    void SetPrimitives(_D3DPRIMITIVETYPE type, int num)
    {
        _PrimitiveType = type;
        _NumPrimitives = num;
    }

    D3DVertexBuffer* _Vertices;        // 0xA4
    D3DIndexBuffer* _Indices;          // 0xA8
    char field_AC[0x4];                // 0xAC (_Tex; not reached)
    int _NumVertices;                  // 0xB0
    _D3DPRIMITIVETYPE _PrimitiveType;  // 0xB4
    int _NumPrimitives;                // 0xB8
};
DECOMP_SIZE_ASSERT(GeometryNode, 0xBC);

class DebugGeometryNode : public GeometryNode { // NAME: proto-pdb:default.pdb
public:
    DebugGeometryNode();
    void SetEndTime(float t) { _endTime = t; }

    bool _enableZ;                     // 0xBC
    float _endTime;                    // 0xC0
};
DECOMP_SIZE_ASSERT(DebugGeometryNode, 0xC4);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetGameTime() const { return _gameTimePublic; }

    float _elapsed;                    // 0x00
    float _actualElapsed;              // 0x04
    float _gameTimePublic;             // 0x08
};

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void AddNode(SwampSceneNode* node, unsigned int);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x4];                 // 0x08
    SwampScene* _scene;                // 0x0C
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// Opaque std (C0090): VC 7.1 bodies only for the rows; the full <set>/<vector> would emit basic_string EH
// funclets and out_of_range throw info here
namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator {};
template <class T> struct less {};
template <class _Ty1, class _Ty2> struct pair {
    pair(const _Ty1& _Val1, const _Ty2& _Val2) : first(_Val1), second(_Val2) {}

    _Ty1 first;
    _Ty2 second;
};
template <class T, class A = allocator<T> > class vector {
public:
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
    ~vector() { _Tidy(); }
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    T& operator[](unsigned int _Pos) { return (*(_Myfirst + _Pos)); }
    void push_back(const T& _Val);

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
struct _Nonscalar_ptr_iterator_tag {};
template <class _T1, class _T2> inline void _Construct(_T1* _Ptr, const _T2& _Val)
{
    new ((void*)_Ptr) _T1(_Val);
}
template <class _FwdIt, class _Ty> inline void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}
template <class _BidIt1, class _BidIt2> inline _BidIt2 _Copy_backward_opt(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Nonscalar_ptr_iterator_tag)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}
template <class K, class Pr, class A, bool Mfl> class _Tset_traits {
public:
    typedef K value_type;
    Pr comp;                           // 0x00
};
template <class Traits> class _Tree : public Traits {
protected:
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Traits::value_type _Myval; // 0x0C
        char _Color;
        char _Isnil;
    };

public:
    class const_iterator {
    public:
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const typename Traits::value_type& operator*() const { return (_Myval(_Ptr)); }
        const_iterator& operator++() { _Inc(); return (*this); }
        bool operator==(const const_iterator& _Right) const { return (_Ptr == _Right._Ptr); }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
        // VC 7.1 <xtree> body, in-class (pick-any COMDAT, C0270): a declaration-only _Inc reloads _Ptr after each
        // call in DestroyAll (89.67); fold VA 0x000B9037 (SwampSoundResource.cpp), the DebugGeometryNode* alias (C0233)
        void _Inc()
        {
            if (_Isnil(_Ptr))
                ;
            else if (!_Isnil(_Right(_Ptr)))
                _Ptr = _Min(_Right(_Ptr));
            else {
                _Node* _Pnode;
                while (!_Isnil(_Pnode = _Parent(_Ptr)) && _Ptr == _Right(_Pnode))
                    _Ptr = _Pnode;
                _Ptr = _Pnode;
            }
        }

        _Node* _Ptr;                   // 0x00
    };
    static char& _Isnil(_Node* _Pnode) { return ((char&)(*_Pnode)._Isnil); }
    static _Node*& _Left(_Node* _Pnode) { return ((_Node*&)(*_Pnode)._Left); }
    static _Node*& _Parent(_Node* _Pnode) { return ((_Node*&)(*_Pnode)._Parent); }
    static _Node*& _Right(_Node* _Pnode) { return ((_Node*&)(*_Pnode)._Right); }
    static _Node* _Min(_Node* _Pnode)
    {
        while (!_Isnil(_Left(_Pnode)))
            _Pnode = _Left(_Pnode);
        return (_Pnode);
    }
    class iterator : public const_iterator {
    public:
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
        typename Traits::value_type& operator*() const { return ((typename Traits::value_type&)**(const_iterator*)this); }
        iterator& operator++() { ++(*(const_iterator*)this); return (*this); }
    };
    static typename Traits::value_type& _Myval(_Node* _Pnode) { return ((typename Traits::value_type&)(*_Pnode)._Myval); }

    _Tree() { _Init(); }
    iterator begin() { return (iterator(_Myhead->_Left)); }
    iterator end() { return (iterator(_Myhead)); }
    pair<iterator, bool> insert(const typename Traits::value_type& _Val);
    void clear();

protected:
    void _Init();

    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class Pr = less<K>, class A = allocator<K> >
class set : public _Tree<_Tset_traits<K, Pr, A, false> > {
};
}

struct D3DIndexBuffer; // NAME: proto-pdb:default.pdb

extern "C" void __stdcall D3DDevice_SetIndices(D3DIndexBuffer* pIndexData, unsigned int BaseVertexIndex); // NAME: proto-pdb:default.pdb

struct Vertex { // NAME: proto-pdb:default.pdb
    Vertex();

    Vector3 loc;                       // 0x00
    Vector3 norm;                      // 0x0C
    unsigned long color;               // 0x18
    char tex[0x10];                    // 0x1C
};
DECOMP_SIZE_ASSERT(Vertex, 0x2C);

class LevelStats { // NAME: proto-pdb:default.pdb
public:
    LevelStats();
    LevelStats(const LevelStats&);
    ~LevelStats();

    char field_0[0x2C];                // 0x00
};
DECOMP_SIZE_ASSERT(LevelStats, 0x2C);

extern "C" const unsigned int D3DPRIMITIVETOVERTEXCOUNT[11][2]; // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawVertices(_D3DPRIMITIVETYPE, unsigned int, unsigned int); // NAME: proto-pdb:default.pdb

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    long SetIndices(D3DIndexBuffer*);
    void DrawPrimitive(_D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount) const;

    char field_0[0x48];                // 0x00
    D3DIndexBuffer* _CurrentIB;        // 0x48
    char field_4C[0x388 - 0x4C];       // 0x4C
    // !FAKE: mutable: the const DrawPrimitive adds to it (retail 0x000D3AF7); the PDB records no mutable flag
    mutable unsigned int _primitivesDrawn; // 0x388
};
DECOMP_SIZE_ASSERT(SwampRenderer, 0x38C);

class BSP; // NAME: proto-pdb:default.pdb

class SwampDebugGeometry { // NAME: proto-pdb:default.pdb
public:
    SwampDebugGeometry();
    ~SwampDebugGeometry();
    void DestroyAll();
    GeometryNode* AddBox(const Vector3& loc, float width, float height, float depth, const Vector3& color, float duration);
    GeometryNode* AddRotatedBox(const Vector3& loc, const Quaternion& rot, float width, float height, float depth, const Vector3& color, float duration);
    GeometryNode* AddPlaneSection(const Vector3& loc, const Quaternion& rot, const Vector3& scale, const Vector3& color, float duration);
    GeometryNode* AddSphere(const Vector3& loc, float radius, const Vector3& color, float duration);
    GeometryNode* AddCylinder(const Vector3& loc, float height, float radius, bool capped, const Vector3& color, float duration);

    std::set<DebugGeometryNode*> _nodes; // 0x00
    std::set<DebugGeometryNode*> _global_bsp_nodes; // 0x0C
    BSP* _global_bsp;                  // 0x18
};
DECOMP_SIZE_ASSERT(SwampDebugGeometry, 0x1C);

// FUNCTION: USA 0x000D39DE
// NAME: proto-pdb:default.pdb
DebugGeometryNode::DebugGeometryNode() : _enableZ(false), _endTime(0.0f)
{
}

// FUNCTION: USA 0x000D39FF
// PROTOTYPE: PREVIEW 0x004BED12 identical
// NAME: proto-pdb:default.pdb
// fold VA: FrontEndUI.cpp emits its LevelEntry alias as a byte-equal COMDAT that relink merges here (C0233)
template unsigned int std::vector<LevelStats,std::allocator<LevelStats> >::size()const;

// FUNCTION: USA 0x000D3A54
// PROTOTYPE: PREVIEW 0x004BED54 identical
// NAME: proto-pdb:default.pdb
template void std::fill<Vertex *,Vertex>(Vertex *,Vertex *,Vertex const &);

// FUNCTION: USA 0x000D3A77
// PROTOTYPE: PREVIEW 0x004BED77 identical
// NAME: proto-pdb:default.pdb
template Vertex * std::_Copy_backward_opt<Vertex *,Vertex *>(Vertex *,Vertex *,Vertex *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000D3AA1
// PROTOTYPE: PREVIEW 0x004BEDA1 identical
// NAME: proto-pdb:default.pdb
long SwampRenderer::SetIndices(D3DIndexBuffer* ib)
{
    if (ib != _CurrentIB) {
        _CurrentIB = ib;
        D3DDevice_SetIndices(ib, 0);
    }
    return 0;
}

// FUNCTION: USA 0x000D3AF3
// PROTOTYPE: PREVIEW 0x004BEDF3 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::DrawPrimitive(_D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount) const
{
    _primitivesDrawn += PrimitiveCount;
    D3DDevice_DrawVertices(PrimitiveType, StartVertex,
        D3DPRIMITIVETOVERTEXCOUNT[PrimitiveType][0] * PrimitiveCount + D3DPRIMITIVETOVERTEXCOUNT[PrimitiveType][1]);
}

// FUNCTION: USA 0x000D3B3A
// PROTOTYPE: PREVIEW 0x004BEE3A identical
// NAME: proto-pdb:default.pdb
template void std::_Construct<Vertex,Vertex>(Vertex *,Vertex const &);

// FUNCTION: USA 0x000D3EAC
// PROTOTYPE: PREVIEW 0x004BF1AC identical
// NAME: proto-pdb:default.pdb
void SwampDebugGeometry::DestroyAll()
{
    {
        for (std::set<DebugGeometryNode*>::iterator i = _nodes.begin(); i != _nodes.end(); ++i) {
            (*i)->RemoveTreeFromScene();
            delete *i;
        }
    }
    _nodes.clear();
    {
        for (std::set<DebugGeometryNode*>::iterator i = _global_bsp_nodes.begin(); i != _global_bsp_nodes.end(); ++i) {
            (*i)->RemoveTreeFromScene();
            delete *i;
        }
    }
    _global_bsp_nodes.clear();
    _global_bsp = 0;
}

// FUNCTION: USA 0x000D4430
// PROTOTYPE: PREVIEW 0x004BF9A0 identical
// NAME: proto-pdb:default.pdb
GeometryNode* SwampDebugGeometry::AddBox(const Vector3& loc, float width, float height, float depth, const Vector3& color, float duration)
{
    Vertex v[8];
    v[0].loc.x = width * -0.5f;
    v[0].loc.y = height * 0.5f;
    v[0].loc.z = depth * 0.5f;
    v[1].loc.x = width * 0.5f;
    v[1].loc.y = height * 0.5f;
    v[1].loc.z = depth * 0.5f;
    v[2].loc.x = width * 0.5f;
    v[2].loc.y = height * 0.5f;
    v[2].loc.z = depth * -0.5f;
    v[3].loc.x = width * -0.5f;
    v[3].loc.y = height * 0.5f;
    v[3].loc.z = depth * -0.5f;
    v[4].loc.x = width * -0.5f;
    v[4].loc.y = height * -0.5f;
    v[4].loc.z = depth * 0.5f;
    v[5].loc.x = width * 0.5f;
    v[5].loc.y = height * -0.5f;
    v[5].loc.z = depth * 0.5f;
    v[6].loc.x = width * 0.5f;
    v[6].loc.y = height * -0.5f;
    v[6].loc.z = depth * -0.5f;
    v[7].loc.x = width * -0.5f;
    v[7].loc.y = height * -0.5f;
    v[7].loc.z = depth * -0.5f;
    for (int i = 0; i < 8; i++)
        v[i].color = color.GetRGB();
    unsigned short indices[14] = { 1, 5, 2, 6, 7, 5, 4, 1, 0, 2, 3, 7, 0, 4 };
    {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        // !FAKE: c2 register allocation; neutral stand-in v0, block-scoped home gn; v0 holds the node in esi, gn is the block-scoped home
        // in the width slot that the reference argument reads (C0221)
        DebugGeometryNode* v0 = new ("SwampDebugGeometry.cpp", 0x13D) DebugGeometryNode;
        DebugGeometryNode* gn = v0;
        v0->SetEndTime(duration >= 0.0f ? duration + g_swamp->GetInput().GetGameTime() : duration);
        v0->SetLocation(loc);
        v0->SetOption(0x80, true, false);
        v0->SetVertices(v, 8);
        v0->SetIndices(indices, 14);
        v0->SetPrimitives(D3DPT_TRIANGLESTRIP, 12);
        g_swamp->_scene->AddNode(v0, 0);
        _nodes.insert(gn);
        return v0;
    }
}

// FUNCTION: USA 0x000D4660
// PROTOTYPE: PREVIEW 0x004BFBD0 identical
// NAME: proto-pdb:default.pdb
GeometryNode* SwampDebugGeometry::AddRotatedBox(const Vector3& loc, const Quaternion& rot, float width, float height, float depth, const Vector3& color, float duration)
{
    Vertex v[8];
    v[0].loc.x = width * -0.5f;
    v[0].loc.y = height * 0.5f;
    v[0].loc.z = depth * 0.5f;
    v[1].loc.x = width * 0.5f;
    v[1].loc.y = height * 0.5f;
    v[1].loc.z = depth * 0.5f;
    v[2].loc.x = width * 0.5f;
    v[2].loc.y = height * 0.5f;
    v[2].loc.z = depth * -0.5f;
    v[3].loc.x = width * -0.5f;
    v[3].loc.y = height * 0.5f;
    v[3].loc.z = depth * -0.5f;
    v[4].loc.x = width * -0.5f;
    v[4].loc.y = height * -0.5f;
    v[4].loc.z = depth * 0.5f;
    v[5].loc.x = width * 0.5f;
    v[5].loc.y = height * -0.5f;
    v[5].loc.z = depth * 0.5f;
    v[6].loc.x = width * 0.5f;
    v[6].loc.y = height * -0.5f;
    v[6].loc.z = depth * -0.5f;
    v[7].loc.x = width * -0.5f;
    v[7].loc.y = height * -0.5f;
    v[7].loc.z = depth * -0.5f;
    for (int i = 0; i < 8; i++)
        v[i].color = color.GetRGB();
    unsigned short indices[14] = { 1, 5, 2, 6, 7, 5, 4, 1, 0, 2, 3, 7, 0, 4 };
    {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        // !FAKE: c2 register allocation; neutral stand-in v0, block-scoped home gn; v0 holds the node in esi, gn is the block-scoped home
        // in the width slot that the reference argument reads (C0221)
        DebugGeometryNode* v0 = new ("SwampDebugGeometry.cpp", 0x166) DebugGeometryNode;
        DebugGeometryNode* gn = v0;
        v0->SetEndTime(duration >= 0.0f ? duration + g_swamp->GetInput().GetGameTime() : duration);
        v0->SetLocation(loc);
        v0->SetRotation(rot);
        v0->SetOption(0x80, true, false);
        v0->SetVertices(v, 8);
        v0->SetIndices(indices, 14);
        v0->SetPrimitives(D3DPT_TRIANGLESTRIP, 12);
        g_swamp->_scene->AddNode(v0, 0);
        _nodes.insert(gn);
        return v0;
    }
}

// FUNCTION: USA 0x000D489A
// PROTOTYPE: PREVIEW 0x004BFE0A identical
// NAME: proto-pdb:default.pdb
GeometryNode* SwampDebugGeometry::AddPlaneSection(const Vector3& loc, const Quaternion& rot, const Vector3& scale, const Vector3& color, float duration)
{
    Vertex v[5];
    Vector3 halfScale(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);
    Vector3 corners[4];
    corners[0] = Vector3(-halfScale.x, 0.0f, halfScale.z);
    corners[1] = Vector3(halfScale.x, 0.0f, halfScale.z);
    corners[2] = Vector3(halfScale.x, 0.0f, -halfScale.z);
    corners[3] = Vector3(-halfScale.x, 0.0f, -halfScale.z);
    Matrix r;
    r.Rotation(rot);
    // !FAKE: inliner; hand form of retail's inlined translation-row store; Matrix::Translation is out of line in
    // SwampMatrix.cpp, also writes the identity rows)
    r.row[3].x = loc.x;
    r.row[3].y = loc.y;
    r.row[3].z = loc.z;
    r.row[3].w = 1.0f;
    r.BatchApply(corners, corners, 4);
    for (int i = 0; i < 4; i++) {
        v[i].color = color.GetRGB();
        v[i].loc = corners[i];
    }
    v[4] = v[0];
    {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        // !FAKE: c2 register allocation; neutral stand-in v0, block-scoped home gn; v0 holds the node in esi, gn is the block-scoped home
        // in the scale slot that the reference argument reads (C0221)
        DebugGeometryNode* v0 = new ("SwampDebugGeometry.cpp", 0x293) DebugGeometryNode;
        DebugGeometryNode* gn = v0;
        v0->SetEndTime(duration >= 0.0f ? duration + g_swamp->GetInput().GetGameTime() : duration);
        v0->SetPrimitives(D3DPT_LINESTRIP, 4);
        v0->SetVertices(v, 5);
        g_swamp->_scene->AddNode(v0, 0);
        _nodes.insert(gn);
        return v0;
    }
}

// FUNCTION: USA 0x000D4ACC
// PROTOTYPE: PREVIEW 0x004C003C identical
// NAME: proto-pdb:default.pdb
SwampDebugGeometry::SwampDebugGeometry()
{
    _global_bsp = 0;
}

// FUNCTION: USA 0x000D4AE4
// PROTOTYPE: PREVIEW 0x004C0054 identical
// NAME: proto-pdb:default.pdb
GeometryNode* SwampDebugGeometry::AddSphere(const Vector3& loc, float radius, const Vector3& color, float duration)
{
    Vertex v;
    v.color = color.GetRGB();
    float v1 = 0.0f;
    std::vector<Vertex> verts;
    std::vector<unsigned short> indices;
    int numTris = 0;
    for (int i = 0; i < 16; i++) {
        float theta = 0.0f;
        int v2 = -1;
        int lvi2 = -1;
        for (int j = 0; j <= 16; j++) {
            if (j == 16)
                theta = 0.0f;
            float x = -sin(theta) * sin(v1);
            float y = cos(v1);
            float z = cos(theta) * sin(v1);
            v.norm.x = x;
            v.norm.y = y;
            v.norm.z = z;
            v.norm.Normalize();
            v.loc.x = x * radius;
            v.loc.y = y * radius;
            v.loc.z = z * radius;
            int v3 = verts.size();
            verts.push_back(v);
            x = -sin(theta) * sin(v1 + 0.19634955f);
            y = cos(v1 + 0.19634955f);
            z = cos(theta) * sin(v1 + 0.19634955f);
            v.norm.x = x;
            v.norm.y = y;
            v.norm.z = z;
            v.norm.Normalize();
            v.loc.x = x * radius;
            v.loc.y = y * radius;
            v.loc.z = z * radius;
            int v4 = verts.size();
            verts.push_back(v);
            if (v2 != -1) {
                indices.push_back(v2);
                indices.push_back(v3);
                indices.push_back(v4);
                indices.push_back(v2);
                indices.push_back(v4);
                indices.push_back(lvi2);
                numTris += 2;
            }
            theta += 0.39269909f;
            v2 = v3;
            lvi2 = v4;
        }
        v1 += 0.19634955f;
    }
    {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        // !FAKE: c2 register allocation; neutral stand-in v0, block-scoped home gn; v0 holds the node in esi, gn is the block-scoped home
        // in the radius slot that the reference argument reads (C0221)
        DebugGeometryNode* v0 = new ("SwampDebugGeometry.cpp", 0x1C4) DebugGeometryNode;
        DebugGeometryNode* gn = v0;
        v0->SetEndTime(duration >= 0.0f ? duration + g_swamp->GetInput().GetGameTime() : duration);
        v0->SetLocation(loc);
        v0->SetOption(0x80, true, false);
        v0->SetVertices(&verts[0], verts.size());
        v0->SetIndices(&indices[0], indices.size());
        v0->SetPrimitives(D3DPT_TRIANGLELIST, numTris);
        g_swamp->_scene->AddNode(v0, 0);
        _nodes.insert(gn);
        return v0;
    }
}

// FUNCTION: USA 0x000D4DB1
// PROTOTYPE: PREVIEW 0x004C0321 identical
// NAME: proto-pdb:default.pdb
GeometryNode* SwampDebugGeometry::AddCylinder(const Vector3& loc, float height, float radius, bool capped, const Vector3& color, float duration)
{
    Vertex v;
    v.color = color.GetRGB();
    float half_height = height * 0.5f;
    std::vector<Vertex> verts;
    std::vector<unsigned short> indices;
    float dh = height / 8;
    int numTris = 0;
    float y = dh - half_height;
    for (int i = 0; i < 8; i++) {
        float theta = 0.0f;
        int v2 = -1;
        int lvi2 = -1;
        for (int j = 0; j <= 16; j++) {
            if (j == 16)
                theta = 0.0f;
            float x = -sin(theta);
            float z = cos(theta);
            v.norm.x = x;
            v.norm.y = 0.0f;
            v.norm.z = z;
            v.norm.Normalize();
            v.loc.x = x * radius;
            v.loc.y = y;
            v.loc.z = z * radius;
            int v3 = verts.size();
            verts.push_back(v);
            x = -sin(theta);
            z = cos(theta);
            v.norm.x = x;
            v.norm.y = 0.0f;
            v.norm.z = z;
            v.norm.Normalize();
            v.loc.x = x * radius;
            v.loc.y = y - dh;
            v.loc.z = z * radius;
            int v4 = verts.size();
            verts.push_back(v);
            if (v2 != -1) {
                if (capped) {
                    if (i == 0) {
                        v.norm.x = 0.0f;
                        v.norm.y = -1.0f;
                        v.norm.z = 0.0f;
                        v.loc.x = 0.0f;
                        v.loc.y = -half_height;
                        v.loc.z = 0.0f;
                        int index = verts.size();
                        verts.push_back(v);
                        indices.push_back(index);
                        indices.push_back(lvi2);
                        indices.push_back(v4);
                        numTris++;
                    } else if (i == 7) {
                        v.norm.x = 0.0f;
                        v.norm.y = 1.0f;
                        v.norm.z = 0.0f;
                        v.loc.x = 0.0f;
                        v.loc.y = half_height;
                        v.loc.z = 0.0f;
                        int index = verts.size();
                        verts.push_back(v);
                        indices.push_back(index);
                        indices.push_back(v3);
                        indices.push_back(v2);
                        numTris++;
                    }
                }
                indices.push_back(v2);
                indices.push_back(v3);
                indices.push_back(v4);
                indices.push_back(v2);
                indices.push_back(v4);
                indices.push_back(lvi2);
                numTris += 2;
            }
            theta += 0.39269909f;
            v2 = v3;
            lvi2 = v4;
        }
        y += dh;
    }
    {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        // !FAKE: c2 register allocation; neutral stand-in v0, block-scoped home gn; v0 holds the node in esi, gn is the block-scoped home
        // in the capped slot that the reference argument reads (C0221)
        DebugGeometryNode* v0 = new ("SwampDebugGeometry.cpp", 0x241) DebugGeometryNode;
        DebugGeometryNode* gn = v0;
        v0->SetEndTime(duration >= 0.0f ? duration + g_swamp->GetInput().GetGameTime() : duration);
        v0->SetLocation(loc);
        v0->SetOption(0x80, true, false);
        v0->SetVertices(&verts[0], verts.size());
        v0->SetIndices(&indices[0], indices.size());
        v0->SetPrimitives(D3DPT_TRIANGLELIST, numTris);
        g_swamp->_scene->AddNode(v0, 0);
        _nodes.insert(gn);
        return v0;
    }
}
