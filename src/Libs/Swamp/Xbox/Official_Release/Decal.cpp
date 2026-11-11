// UNIT: src/Libs/Swamp/Xbox/Official_Release/Decal.cpp OBJ: Xbox\Official_Release\Decal.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <xmmintrin.h>

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

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
    char field_4[0x120 - 0x4];         // 0x04
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class SwampRenderer; // NAME: proto-pdb:default.pdb

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampRenderer& GetRenderer() const { return *_renderer; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x68 - 0xC];          // 0x0C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AABox;      // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb
class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

struct Vector4 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    void Multiply(const Matrix& m, Matrix& result) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

enum _D3DPRIMITIVETYPE { // NAME: proto-pdb:default.pdb
    D3DPT_TRIANGLEFAN = 7
};

enum _D3DTRANSFORMSTATETYPE { // NAME: proto-pdb:default.pdb
    D3DTS_VIEW = 0
};

struct _D3DMATRIX; // NAME: proto-pdb:default.pdb

extern "C" long __stdcall D3DDevice_GetTransform(_D3DTRANSFORMSTATETYPE, _D3DMATRIX*); // NAME: proto-pdb:default.pdb
extern "C" void __fastcall D3DDevice_SetVertexShaderConstant4(int, const void*); // NAME: proto-pdb:default.pdb
extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawVerticesUP(_D3DPRIMITIVETYPE, unsigned int, const void*, unsigned int); // NAME: proto-pdb:default.pdb

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void SetBasicVertexShader(const _D3DVERTEXATTRIBUTEFORMAT*);
    void SetBasicPixelShader();
    void GetProjectionTransform(Matrix&) const;
    void SetTexture(unsigned long, const ResourceHandle<TextureResource>&);
    void SetEmptyTexture(unsigned long);
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeRenderingSettings { Alpha = 0, TotalRenderingSettings = 1 }; // NAME: proto-pdb:default.pdb

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
    unsigned int GetRenderingSetting(unsigned int setting) const { return _RenderingSetting[setting]; }

    char field_4[0x8C - 0x4];          // 0x04
    unsigned int _RenderingSetting[1]; // 0x8C
    char field_90[0xA4 - 0x90];        // 0x90
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class Decal : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~Decal();                                              // vtable+0x00

protected:
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

private:
    static void InitShaders();

    struct DecalVertex { // NAME: proto-pdb:default.pdb
        Vector3 loc;                   // 0x00
        unsigned int color;            // 0x0C
        short tex[2];                  // 0x10
    };

    float _duration;                   // 0xA4
    float _timeRemaining;              // 0xA8
    static _D3DVERTEXATTRIBUTEFORMAT _vaf;
    ResourceHandle<TextureResource> _tex; // 0xAC
    DecalVertex _verts[4];             // 0xB0
};
DECOMP_SIZE_ASSERT(Decal, 0x100);

// FUNCTION: USA 0x0014F850
// PROTOTYPE: PREVIEW 0x00538EFB identical
// NAME: proto-pdb:default.pdb
void Decal::Update()
{
    if (_duration <= 0.0f)
        return;
    if ((_timeRemaining -= g_swamp->_input->GetElapsedTime()) <= 0.0f) {
        Invalidate();
    } else {
        // !FAKE: provenance; neutral stand-ins v0-v2 (alpha float the conversion reads, color, loop index); the PDB records no local
        // !FAKE: c1xx front end; cast; retail fimul is a signed multiply, the unsigned setting adds a 2^32 fix-up (82.93); without the parentheses the int is loaded first (79.02 -> 100)
        float v0 = (_timeRemaining / _duration) * (int)GetRenderingSetting(Alpha);
        unsigned int v1 = (_mm_cvtt_ss2si(_mm_load_ss(&v0)) << 24) | 0xFFFFFF;
        for (int v2 = 0; v2 < 4; v2++)
            _verts[v2].color = v1;
    }
}

// FUNCTION: USA 0x0014F8D0
// PROTOTYPE: PREVIEW 0x00538F7B identical
// NAME: proto-pdb:default.pdb
void Decal::InitShaders()
{
    if (_vaf.Input[0].Format == 0) {
        InitVAF(_vaf);
        _vaf.Input[0].StreamIndex = 0;
        _vaf.Input[0].Offset = 0;
        _vaf.Input[0].Format = 0x32; // retail
        _vaf.Input[3].StreamIndex = 0;
        _vaf.Input[3].Offset = 0xC;
        _vaf.Input[3].Format = 0x40; // retail
        _vaf.Input[9].StreamIndex = 0;
        _vaf.Input[9].Offset = 0x10;
        _vaf.Input[9].Format = 0x25; // retail
    }
}

// FUNCTION: USA 0x0014F932
// PROTOTYPE: PREVIEW 0x00538FDD identical
// NAME: proto-pdb:default.pdb
void Decal::Render(const Matrix& m) const
{
    SwampRenderer& r = g_swamp->GetRenderer();
    r.SetBasicVertexShader(&_vaf);
    r.SetBasicPixelShader();
    Matrix view;
    D3DDevice_GetTransform(D3DTS_VIEW, (_D3DMATRIX*)&view);
    Matrix proj;
    r.GetProjectionTransform(proj);
    // !FAKE: inliner; hand form of the retail-inlined Matrix product by value: stand-ins v0-v2 for its result and copy temps; the PDB records only viewproj
    Matrix v0;
    m.Multiply(view, v0);
    Matrix v1 = v0;
    Matrix v2;
    v1.Multiply(proj, v2);
    Matrix viewproj = v2;
    // retail: register 0 biased by 96 (the XDK inline SetVertexShaderConstant form)
    D3DDevice_SetVertexShaderConstant4(0x60, &viewproj);
    r.SetTexture(0, _tex);
    D3DDevice_SetRenderState_Simple(0x4035C, 0);
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 0x303);
    D3DDevice_DrawVerticesUP(D3DPT_TRIANGLEFAN, 4, _verts, sizeof(DecalVertex));
    r.SetEmptyTexture(0);
    D3DDevice_SetRenderState_Simple(0x4035C, 1);
    D3DDevice_SetRenderState_Simple(0x40304, 0);
}

// FUNCTION: USA 0x0014FBC5
// NAME: proto-pdb:default.pdb
// ??_GDecal (alias ??_EDecal) unmatched, no draft; wall: a virtual ??_G is emitted only with the class vtable, i.e. with its ctor
// ??0Decal 0x0014FA4E (candidates fail: unresolved target 0x2DE9D) (C0320); match_one: `not defined in Decal.obj` (5.3 T6)
// FUNCTION: USA 0x0014FBE1
// NAME: proto-pdb:default.pdb
Decal::~Decal()
{
}
