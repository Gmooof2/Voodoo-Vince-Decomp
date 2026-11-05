// UNIT: src/Libs/Swamp/Xbox/Official_Release/FogZone.cpp OBJ: Xbox\Official_Release\FogZone.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const char* s);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct _D3DVERTEXSHADERINPUT { // NAME: proto-pdb:default.pdb
    unsigned long StreamIndex;         // 0x00
    unsigned long Offset;              // 0x04
    unsigned long Format;              // 0x08
    unsigned char TessType;            // 0x0C
    unsigned char TessSource;          // 0x0D
};
DECOMP_SIZE_ASSERT(_D3DVERTEXSHADERINPUT, 0x10);

struct _D3DVERTEXATTRIBUTEFORMAT { // NAME: proto-pdb:default.pdb
    _D3DVERTEXSHADERINPUT Input[16];   // 0x00
};
DECOMP_SIZE_ASSERT(_D3DVERTEXATTRIBUTEFORMAT, 0x100);

void InitVAF(_D3DVERTEXATTRIBUTEFORMAT& vaf); // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class PixelShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(VertexShaderResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<PixelShaderResource>, 0x4);

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);

    char field_0[0x58];                // 0x00
};
DECOMP_SIZE_ASSERT(ResourceManager<PixelShaderResource>, 0x58);

class PixelShaderManager : public ResourceManager<PixelShaderResource> { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(PixelShaderManager, 0x58);

class VertexShaderManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<VertexShaderResource> CreateShader(const SwampString& name, const _D3DVERTEXATTRIBUTEFORMAT* vaf);

    char field_0[0x58];                // 0x00
};
DECOMP_SIZE_ASSERT(VertexShaderManager, 0x58);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    PixelShaderManager& GetPixelShaderManager() const { return *_pixelShaderManager; }
    VertexShaderManager& GetVertexShaderManager() const { return *_vertexShaderManager; }

    char field_0[0x2C];                // 0x00
    PixelShaderManager* _pixelShaderManager;   // 0x2C
    char field_30[0x3C - 0x30];        // 0x30
    VertexShaderManager* _vertexShaderManager; // 0x3C
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class FogZone : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~FogZone();                                            // vtable+0x00

protected:
    void InitShaders();
    static bool IsPointInside(const Vector3& loc, const Vector3* verts, unsigned int numIndices,
                              const unsigned short* indices);

    char field_A4[0x128 - 0xA4];       // 0xA4
    ResourceHandle<PixelShaderResource> _pshader;  // 0x128
    ResourceHandle<VertexShaderResource> _vshader; // 0x12C
    static _D3DVERTEXATTRIBUTEFORMAT _vaf;
};
DECOMP_SIZE_ASSERT(FogZone, 0x130);

// FUNCTION: USA 0x000D0BA2
// PROTOTYPE: PREVIEW 0x004BBE5D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.81, bytes mismatch (norm.x second product: retail fld edge1.y; fmul st(3), ours fld st(2); fmuls edge1.y)
bool FogZone::IsPointInside(const Vector3& loc, const Vector3* verts, unsigned int numIndices,
                            const unsigned short* indices)
{
    for (unsigned int i = 2; i < numIndices; i++) {
        // !FAKE: provenance; neutral stand-ins p (strip vertex i), v0 (first strip edge), v1 (loc - p), register-only
        // in retail; the PDB records only edge1, norm
        const Vector3& p = verts[indices[i]];
        Vector3 v0(p.x - verts[indices[i - 2]].x, p.y - verts[indices[i - 2]].y, p.z - verts[indices[i - 2]].z);
        Vector3 edge1(p.x - verts[indices[i - 1]].x, p.y - verts[indices[i - 1]].y, p.z - verts[indices[i - 1]].z);
        Vector3 norm(v0.y * edge1.z - v0.z * edge1.y, v0.z * edge1.x - v0.x * edge1.z,
                     v0.x * edge1.y - v0.y * edge1.x);
        Vector3 v1(loc.x - p.x, loc.y - p.y, loc.z - p.z);
        if ((norm.x * v1.x + norm.y * v1.y + norm.z * v1.z) * (int)(1 - (i & 1) * 2) >= 0.0f)
            return false;
    }
    return true;
}
#endif

// FUNCTION: USA 0x000D12DE
// PROTOTYPE: PREVIEW 0x004BC599 identical
// NAME: proto-pdb:default.pdb
void FogZone::InitShaders()
{
    if (_vaf.Input[0].Format == 0) {
        InitVAF(_vaf);
        _vaf.Input[0].StreamIndex = 0;
        _vaf.Input[0].Offset = 0;
        _vaf.Input[0].Format = 0x32; // retail
    }
    _pshader = g_swamp->GetPixelShaderManager().Load("FogZone");
    _vshader = g_swamp->GetVertexShaderManager().CreateShader("FogZone", &_vaf);
}
