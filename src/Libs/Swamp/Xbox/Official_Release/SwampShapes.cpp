// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampShapes.cpp OBJ: Xbox\Official_Release\SwampShapes.obj OPT: /O1 /Oy /Oi
#include "../../../../include/decomp.h"
#include <string.h>

struct D3DVertexBuffer; // NAME: proto-pdb:default.pdb

struct D3DIndexBuffer { // NAME: proto-pdb:default.pdb
    unsigned long Common;              // 0x00
    unsigned long Data;                // 0x04
    unsigned long Lock;                // 0x08
};
DECOMP_SIZE_ASSERT(D3DIndexBuffer, 0xC);

extern "C" D3DVertexBuffer* __stdcall D3DDevice_CreateVertexBuffer2(unsigned int length); // NAME: proto-pdb:default.pdb
extern "C" D3DIndexBuffer* __stdcall D3DDevice_CreateIndexBuffer2(unsigned int length); // NAME: proto-pdb:default.pdb
extern "C" unsigned char* __stdcall D3DVertexBuffer_Lock2(D3DVertexBuffer* vb, unsigned long flags); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DXGetErrorStringA(long hr, char* buffer, unsigned int size); // NAME: proto-pdb:default.pdb

template <class T> void RELEASE(T*& p); // NAME: proto-pdb:default.pdb

struct Vertex { // NAME: proto-pdb:default.pdb
    char loc[0xC];                     // 0x00
    char norm[0xC];                    // 0x0C
    unsigned long color;               // 0x18
    char tex[0x10];                    // 0x1C
};
DECOMP_SIZE_ASSERT(Vertex, 0x2C);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0xC];                // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> GetDefaultTexture() const { return _defaultTexture; }

    char field_0[0x58];                // 0x00
    ResourceHandle<TextureResource> _defaultTexture; // 0x58
    char field_5C[0x28];               // 0x5C
};
DECOMP_SIZE_ASSERT(TextureManager, 0x84);

enum _D3DPRIMITIVETYPE { // NAME: proto-pdb:default.pdb
    D3DPT_FORCE_DWORD = 0x7FFFFFFF
};

enum _D3DTRANSFORMSTATETYPE { // NAME: proto-pdb:default.pdb
    D3DTS_WORLD = 6
};

struct _D3DMATRIX; // NAME: proto-pdb:default.pdb

struct Vector4 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    static const Matrix& GetIdentity() { return _identity; }

    Vector4 row[4];                    // 0x00
private:
    static Matrix _identity;
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

extern "C" void __stdcall D3DDevice_SetTransform(_D3DTRANSFORMSTATETYPE, const _D3DMATRIX*); // NAME: proto-pdb:default.pdb
extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // NAME: proto-pdb:default.pdb

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    long SetStreamSource(D3DVertexBuffer* vb, unsigned int stride);
    long SetVertexShader(unsigned long);
    void SetBasicPixelShader();
    void SetTexture(unsigned long, const ResourceHandle<TextureResource>&);
    long SetIndices(D3DIndexBuffer*);
    void DrawIndexedPrimitive(_D3DPRIMITIVETYPE, unsigned int, unsigned int, unsigned int, unsigned int) const;
    void DrawPrimitive(_D3DPRIMITIVETYPE, unsigned int, unsigned int) const;
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    TextureManager& GetTextureManager() const { return *_textureManager; }
    SwampRenderer& GetRenderer() const { return *_renderer; }

    char field_0[0x8];                 // 0x00
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x2C];                // 0x0C
    TextureManager* _textureManager;   // 0x38
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class GeometryNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetVertices(const Vertex* verts, int num);
    void SetIndices(const unsigned short* indices, int num);
protected:
    virtual void Render(const Matrix& m) const;
public:

    D3DVertexBuffer* _Vertices;        // 0xA4
    D3DIndexBuffer* _Indices;          // 0xA8
    ResourceHandle<TextureResource> _Tex; // 0xAC
    int _NumVertices;                  // 0xB0
    _D3DPRIMITIVETYPE _PrimitiveType;  // 0xB4
    int _NumPrimitives;                // 0xB8
};
DECOMP_SIZE_ASSERT(GeometryNode, 0xBC);

// FUNCTION: USA 0x000DBF0C
// PROTOTYPE: PREVIEW 0x004C7443 identical
// NAME: proto-pdb:default.pdb
void GeometryNode::SetVertices(const Vertex* verts, int num)
{
    char buff[0x80];
    RELEASE(_Vertices);
    _Vertices = D3DDevice_CreateVertexBuffer2(num * sizeof(Vertex));
    // !FAKE: c1xx front end; stand-in local for the XDK inline wrapper's HRESULT; no PDB local
    long v0 = _Vertices ? 0 : 0x8007000E;
    if (v0 < 0)
        D3DXGetErrorStringA(v0, buff, sizeof(buff));
    // !FAKE: c2 register allocation; stand-in locals for the XDK inline Lock's buffer and locked pointer; no PDB local
    D3DVertexBuffer* v1 = _Vertices;
    unsigned char* p = D3DVertexBuffer_Lock2(v1, 0x10);
    memcpy(p, verts, num * sizeof(Vertex));
    _NumVertices = num;
}

// FUNCTION: USA 0x000DBF83
// PROTOTYPE: PREVIEW 0x004C74BA identical
// NAME: proto-pdb:default.pdb
void GeometryNode::SetIndices(const unsigned short* indices, int num)
{
    char buff[0x80];
    RELEASE(_Indices);
    _Indices = D3DDevice_CreateIndexBuffer2(num * sizeof(unsigned short));
    // !FAKE: c1xx front end; stand-in local for the XDK inline wrapper's HRESULT; no PDB local
    long v0 = _Indices ? 0 : 0x8007000E;
    if (v0 < 0)
        D3DXGetErrorStringA(v0, buff, sizeof(buff));
    memcpy((void*)_Indices->Data, indices, num * sizeof(unsigned short));
}

// FUNCTION: USA 0x000DBFE5
// PROTOTYPE: PREVIEW 0x004C751C identical
// NAME: proto-pdb:default.pdb
void GeometryNode::Render(const Matrix& m) const
{
    SwampRenderer& r = g_swamp->GetRenderer();
    D3DDevice_SetTransform(D3DTS_WORLD, (const _D3DMATRIX*)&m);
    r.SetStreamSource(_Vertices, sizeof(Vertex));
    r.SetVertexShader(0x252);
    r.SetBasicPixelShader();
    if (_Tex.IsValid())
        r.SetTexture(0, _Tex);
    else
        r.SetTexture(0, g_swamp->GetTextureManager().GetDefaultTexture());
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 0x303);
    D3DDevice_SetRenderState_Simple(0x4035C, 0);
    if (_Indices) {
        r.SetIndices(_Indices);
        r.DrawIndexedPrimitive(_PrimitiveType, 0, _NumVertices, 0, _NumPrimitives);
    } else {
        r.DrawPrimitive(_PrimitiveType, 0, _NumPrimitives);
    }
    D3DDevice_SetRenderState_Simple(0x4035C, 1);
    D3DDevice_SetTransform(D3DTS_WORLD, (const _D3DMATRIX*)&Matrix::GetIdentity());
}
