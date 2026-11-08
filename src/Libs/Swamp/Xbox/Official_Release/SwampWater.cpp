// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampWater.cpp OBJ: Xbox\Official_Release\SwampWater.obj OPT: /O1 /Oy
#include <xmmintrin.h>
// !FAKE: c2 intrinsics; file-scope pragma; retail inlines the WaterField ctor's memset as rep stosd/stosb under /O1 /Oy (C0181)
extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memset)
#include "../../../../include/decomp.h"

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
double RandomFloat(float, float);
}

float FastReciprocalSquareRoot(float f); // NAME: proto-pdb:default.pdb
void operator delete[](void* p);
void* operator new[](unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

extern "C" unsigned long D3D__DirtyFlags; // NAME: proto-pdb:default.pdb
extern "C" unsigned long D3D__RenderState[]; // NAME: proto-pdb:default.pdb
extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawVerticesUP(unsigned long, unsigned int, const void*, unsigned int); // NAME: proto-pdb:default.pdb

struct Vector2 { // NAME: proto-pdb:default.pdb
    Vector2() {}
    Vector2(float u_, float v_) : u(u_), v(v_) {}

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    void Normalize();

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    void AffineInvert();
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }
    void Apply(const Vector3&, Vector3&) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

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
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    // !FAKE: c2 register allocation; visible callee body (C0136); SwampWaterNode::Render keeps the temp's pointer in ebx across the call (95.86 opaque)
    ResourceHandle& operator=(const ResourceHandle& h)
    {
        if (_Resource)
            _Resource->SubRef();
        _Resource = h._Resource;
        if (_Resource)
            _Resource->AddRef();
        return *this;
    }
    bool IsValid() const { return _Resource != 0; }
    const T& operator()() const { return *_Resource; }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

enum _D3DFORMAT { // NAME: proto-pdb:default.pdb
    D3DFMT_A8R8G8B8 = 0x6,
};

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> CreateTexture(const void*, int, int, _D3DFORMAT, bool);
};

class MaterialResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    const ResourceHandle<TextureResource>& GetEnvmapTexture() const { return _envmapTex; }
    unsigned int GetDetailType() const { return _detailType; }
    float GetReflectivity() const { return _reflectivity; }

    char field_24[0x30 - 0x24];        // 0x24
    ResourceHandle<TextureResource> _envmapTex; // 0x30
    char field_34[0x40 - 0x34];        // 0x34
    unsigned int _detailType;          // 0x40
    float _reflectivity;               // 0x44
    char field_48[0x58 - 0x48];        // 0x48
};
DECOMP_SIZE_ASSERT(MaterialResource, 0x58);

struct WaterVertex { // NAME: proto-pdb:default.pdb
    Vector3 pos;                       // 0x00
    Vector3 normal;                    // 0x0C
    Vector2 bumpcoord;                 // 0x18
    unsigned long texcoord;            // 0x20
    unsigned long detailcoord;         // 0x24
    unsigned long na[2];               // 0x28
};
DECOMP_SIZE_ASSERT(WaterVertex, 0x30);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    bool IsGamePaused() const { return _pauseGame; }

    char field_0[0x34];                // 0x00
    bool _pauseGame;                   // 0x34
};

struct D3DVertexBuffer { // NAME: proto-pdb:default.pdb
    unsigned long Common;              // 0x00
    unsigned long Data;                // 0x04
    unsigned long Lock;                // 0x08
};
DECOMP_SIZE_ASSERT(D3DVertexBuffer, 0xC);

extern "C" D3DVertexBuffer* __stdcall D3DDevice_CreateVertexBuffer2(unsigned int length); // NAME: proto-pdb:default.pdb
extern "C" unsigned char* __stdcall D3DVertexBuffer_Lock2(D3DVertexBuffer* vb, unsigned long flags); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DXGetErrorStringA(long hr, char* buffer, unsigned int size); // NAME: proto-pdb:default.pdb

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void SetEmptyTexture(unsigned long stage);
    long SetStreamSource(D3DVertexBuffer* vb, unsigned int stride);
    void SetCullMode(unsigned long);
};

class MaterialManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> GetDefaultEnvMap() const { return _defaultEMap; }

    char field_0[0x5C];                // 0x00
    ResourceHandle<TextureResource> _defaultEMap; // 0x5C
};

class SwampCamera; // NAME: proto-pdb:default.pdb

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampCamera* GetCamera() const { return _currentCamera; }
    SwampInput& GetInput() const { return *_input; }
    SwampRenderer& GetRenderer() const { return *_renderer; }
    MaterialManager& GetMaterialManager() const { return *_materialManager; }
    TextureManager& GetTextureManager() const { return *_textureManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x24 - 0xC];          // 0x0C
    MaterialManager* _materialManager; // 0x24
    char field_28[0x38 - 0x28];        // 0x28
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x4];                // 0x3C
    SwampCamera* _currentCamera;       // 0x40
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct AABox { // NAME: proto-pdb:default.pdb
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct Quaternion; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2,
        UseAlpha = 0x8
    };
    SwampSceneNode(int type);
    void SetOption(unsigned int option, bool value, bool recursive);
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
    Matrix GetWorldTransformation() const;

    char field_4[0x6C - 0x4];          // 0x04
    AABox _BoundingBox;                // 0x6C
    char field_84[0xA4 - 0x84];        // 0x84
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

template <class T> void RELEASE(T*& p); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0015099F
// PROTOTYPE: PREVIEW 0x0053A01F identical
// NAME: proto-pdb:default.pdb
// ??_GFloatyBits: compiler-generated with the vtable ~FloatyBits emits; calls ??1 out of line
class FloatyBits : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    FloatyBits(unsigned int numBits);
    virtual ~FloatyBits();                                         // vtable+0x00
protected:
    virtual void Render(const Matrix&) const;                      // vtable+0x20
public:

    float _waterLevel;                 // 0xA4
    unsigned int _numBits;             // 0xA8
    D3DVertexBuffer* _vertexBuffer;    // 0xAC
};
DECOMP_SIZE_ASSERT(FloatyBits, 0xB0);

class WaterField { // NAME: proto-pdb:default.pdb
public:
    WaterField(unsigned int width, unsigned int height, bool wrap);
    ~WaterField();
    bool Update();
    void UpdateBumpmap();
    void Perturb(const Vector2& loc, float amount);
    void RenderSetup(const Matrix& m, const ResourceHandle<TextureResource>& eMap, float ref) const;
    void RenderCleanup() const;
    void RenderOverlaySetup(const Matrix& m, const ResourceHandle<MaterialResource>& mtl) const;
    void RenderOverlayCleanup() const;
    void InitSurfaceDistortions();
protected:
    static void InitShaders();
public:

    unsigned int _width;               // 0x00
    unsigned int _height;              // 0x04
    unsigned int _widthLog2;           // 0x08
    unsigned int _heightLog2;          // 0x0C
    float* _hf[2];                     // 0x10
    unsigned int _fieldIndex;          // 0x18
    float _turbulence;                 // 0x1C
    float _speed;                      // 0x20
    float _elasticity;                 // 0x24
    float _verticalness;               // 0x28
    bool _wrap;                        // 0x2C
    bool _calm;                        // 0x2D
    ResourceHandle<TextureResource> _bumpTex; // 0x30
    Vector4 _color;                    // 0x34
};
DECOMP_SIZE_ASSERT(WaterField, 0x44);

// FUNCTION: USA 0x00151202
// PROTOTYPE: PREVIEW 0x0053A882 identical
// NAME: proto-pdb:default.pdb
// ??_GSwampPuddle: compiler-generated with the vtable ~SwampPuddle emits; calls ??1 out of line
class SwampPuddle : public SwampSceneNode, public WaterField { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampPuddle();                                        // vtable+0x00
    Vector3 ProjectAndPerturb(const Vector3& worldLoc, float amount);
    Vector2 Project(const Vector3& worldLoc) const;
protected:
    virtual void Update();
    virtual void Render(const Matrix& m) const;

    ResourceHandle<MaterialResource> _mtl; // 0xE8
    ResourceHandle<TextureResource> _envMap; // 0xEC
    WaterVertex _verts[4];             // 0xF0
    Vector2 _size;                     // 0x1B0
};
DECOMP_SIZE_ASSERT(SwampPuddle, 0x1B8);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

struct GatorVertex { // NAME: proto-pdb:default.pdb
    Vector3 loc;                       // 0x00
    unsigned char boneWeights[4];      // 0x0C
    unsigned char boneIndices[4];      // 0x10
    unsigned int norm;                 // 0x14
    unsigned int tex[2];               // 0x18
};
DECOMP_SIZE_ASSERT(GatorVertex, 0x20);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void LowLevelRender(bool) const;
    unsigned int GetVertexCount() const;
    const AABox& GetBounds() const;
    D3DVertexBuffer* GetVertexBuffer() const { return (D3DVertexBuffer*)&_vertexBuffer; }

    char field_24[0x40 - 0x24];        // 0x24
    D3DVertexBuffer _vertexBuffer;     // 0x40
    char field_4C[0x60 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(ModelResource, 0x60);

// FUNCTION: USA 0x001509BB
// PROTOTYPE: PREVIEW 0x0053A03B identical
// NAME: proto-pdb:default.pdb
// ??_GSwampWaterNode: compiler-generated with the vtable ~SwampWaterNode emits; calls ??1 out of line
class SwampWaterNode : public SwampSceneNode, public WaterField { // NAME: proto-pdb:default.pdb
public:
    SwampWaterNode(const ResourceHandle<ModelResource>& model, const ResourceHandle<MaterialResource>& mtl, float density, unsigned int fieldsize);
    virtual ~SwampWaterNode();                                     // vtable+0x00
protected:
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix& m) const;                    // vtable+0x20

    float _density;                    // 0xE8
    ResourceHandle<MaterialResource> _mtl; // 0xEC
    ResourceHandle<ModelResource> _model; // 0xF0
    D3DVertexBuffer* _vertexBuffer;    // 0xF4
};
DECOMP_SIZE_ASSERT(SwampWaterNode, 0xF8);

// FUNCTION: USA 0x0014FBF9
// PROTOTYPE: PREVIEW 0x005392A4 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c2 register allocation; inline keyword; without it cl knows Log2 leaves ecx and keeps height there across the calls (C0128 wall; WaterField ctor 94.6)
inline unsigned int Log2(unsigned int x)
{
    float fx = (float)x;
    return (*(unsigned int*)&fx >> 23) - 127;
}

// FUNCTION: USA 0x0014FC1A
// PROTOTYPE: PREVIEW 0x005392C5 identical
// NAME: proto-pdb:default.pdb
WaterField::~WaterField()
{
    // !FAKE: c1xx front end; dummy local; the PDB records no loop index
    for (int v1 = 0; v1 < 2; v1++)
        delete[] _hf[v1];
}

// FUNCTION: USA 0x0014FF84
// PROTOTYPE: PREVIEW 0x0053962F identical
// NAME: proto-pdb:default.pdb
void WaterField::InitSurfaceDistortions()
{
    if (_turbulence != 0.0f) {
        // !FAKE: c1xx front end; dummy local; the PDB records no loop index
        for (int v1 = _width * _height - 1; v1 >= 0; v1 -= (BeepLib::Random() & 0x1F) + 1) {
            // !FAKE: c2 x87 stack; temporary; direct store keeps a double copy (fld st(0)/fstp st(0)) (match_one 87.57 -> 100.0)
            float v2 = BeepLib::RandomFloat(-_turbulence, _turbulence) * 0.75;
            _hf[_fieldIndex][v1] = v2;
        }
        _calm = false;
    }
}

// FUNCTION: USA 0x00150145
// PROTOTYPE: PREVIEW 0x005397C5 identical
// NAME: proto-pdb:default.pdb
void WaterField::Perturb(const Vector2& loc, float amount)
{
    // !FAKE: c2 float conversion; stand-in locals for the cvttss2si operands; no PDB local
    float v1 = _width * loc.u;
    float v2 = _height * loc.v;
    unsigned int v3 = _mm_cvtt_ss2si(_mm_load_ss(&v1)) & (_width - 1);
    unsigned int v4 = _mm_cvtt_ss2si(_mm_load_ss(&v2)) & (_height - 1);
    // !FAKE: c2 register allocation; dummy local; row pointer keeps retail's two lea (match_one 90.52 -> 100.0)
    float* v5 = _hf[_fieldIndex] + (v4 << _widthLog2);
    v5[v3] += amount;
    _calm = false;
}

// FUNCTION: USA 0x00150229
// PROTOTYPE: PREVIEW 0x005398A9 identical
// NAME: proto-pdb:default.pdb
void WaterField::RenderOverlayCleanup() const
{
    // !FAKE: c2 register allocation; dummy local; renderer held in esi across the three calls
    SwampRenderer& v1 = g_swamp->GetRenderer();
    v1.SetEmptyTexture(0);
    v1.SetEmptyTexture(1);
    v1.SetEmptyTexture(2);
    // !FAKE: inliner; hand form of the XDK inline D3DDevice_SetRenderState bodies; retail encodings, constant-folded
    D3DDevice_SetRenderState_Simple(0x4035C, 1);
    D3DDevice_SetRenderState_Simple(0x40304, 0);
    D3D__DirtyFlags |= 0x2000;
    D3D__RenderState[0x5C] = 0;
}

// FUNCTION: USA 0x00150276
// PROTOTYPE: PREVIEW 0x005398F6 identical
// NAME: proto-pdb:default.pdb
Vector3 SwampPuddle::ProjectAndPerturb(const Vector3& worldLoc, float amount)
{
    Matrix world = GetWorldTransformation();
    Matrix world2Local = world;
    world2Local.AffineInvert();
    Vector3 localLoc;
    {
        // !FAKE: inliner; hand form of the inline Matrix::Apply(const Vector3&) body; /O1 keeps it out of line
        Vector3 v1;
        world2Local.Apply(worldLoc, v1);
        localLoc = v1;
    }
    localLoc.y = 0.0f;
    Vector2 puddleSpace(localLoc.x / _size.u, localLoc.z / _size.v);
    puddleSpace.u += 0.5f;
    puddleSpace.v += 0.5f;
    if (puddleSpace.u > 0.0f && puddleSpace.u < 1.0f && puddleSpace.v > 0.0f && puddleSpace.v < 1.0f)
        Perturb(puddleSpace, amount);
    return localLoc;
}

// FUNCTION: USA 0x00150355
// PROTOTYPE: PREVIEW 0x005399D5 identical
// NAME: proto-pdb:default.pdb
Vector2 SwampPuddle::Project(const Vector3& worldLoc) const
{
    Matrix world = GetWorldTransformation();
    Matrix world2Local = world;
    world2Local.AffineInvert();
    // !FAKE: inliner; hand form of the inline Matrix::Apply(const Vector3&) body; /O1 keeps it out of line (match_one 62.88 -> 79.14 -> 100.0 with the += 0.5f stores)
    Vector3 v1;
    world2Local.Apply(worldLoc, v1);
    Vector3 localLoc = v1;
    Vector2 puddleSpace(localLoc.x / _size.u, localLoc.z / _size.v);
    puddleSpace.u += 0.5f;
    puddleSpace.v += 0.5f;
    return puddleSpace;
}

// FUNCTION: USA 0x001503DE
// PROTOTYPE: PREVIEW 0x00539A5E identical
// NAME: proto-pdb:default.pdb
FloatyBits::~FloatyBits()
{
    RELEASE(_vertexBuffer);
}

// FUNCTION: USA 0x001503FC
// PROTOTYPE: PREVIEW 0x00539A7C identical
// NAME: proto-pdb:default.pdb
void Vector4::Normalize()
{
    // !FAKE: c2 register allocation; dummy locals; retail loads w,z,y,x into x87 (as Normalize in SwampQuat)
    float v4 = w, v3 = z, v2 = y, v1 = x;
    // !FAKE: c2 register allocation; dummy local; scale held on the x87 stack across four products
    float v5 = FastReciprocalSquareRoot(v1 * v1 + v2 * v2 + v3 * v3 + v4 * v4);
    x *= v5;
    y *= v5;
    z *= v5;
    w *= v5;
}

// FUNCTION: USA 0x001507A4
// PROTOTYPE: PREVIEW 0x00539E24 identical
// NAME: proto-pdb:default.pdb
void SwampPuddle::Update()
{
    if (!g_swamp->GetInput().IsGamePaused()) {
        if (WaterField::Update())
            UpdateBumpmap();
    }
}

// FUNCTION: USA 0x001507CE
// PROTOTYPE: PREVIEW 0x00539E4E identical
// NAME: proto-pdb:default.pdb
SwampWaterNode::~SwampWaterNode()
{
    RELEASE(_vertexBuffer);
}

// FUNCTION: USA 0x00150C75
// PROTOTYPE: PREVIEW 0x0053A2F5 identical
// NAME: proto-pdb:default.pdb
WaterField::WaterField(unsigned int width, unsigned int height, bool wrap)
    : _width(width), _height(height), _fieldIndex(0), _turbulence(0.006f), _speed(4.1f), _elasticity(0.99f),
      _verticalness(0.8f), _wrap(wrap), _calm(false), _color(0.475f, 0.447f, 0.243f, 0.5f)
{
    _widthLog2 = Log2(width);
    _heightLog2 = Log2(height);
    // !FAKE: c1xx front end; dummy local; the PDB records no loop index
    for (int v1 = 0; v1 < 2; v1++) {
        _hf[v1] = new("SwampWater.cpp", 57) float[width * height];
        memset(_hf[v1], 0, width * height * sizeof(float));
    }
    InitShaders();
    _bumpTex = g_swamp->GetTextureManager().CreateTexture(0, width, height, D3DFMT_A8R8G8B8, false);
}

// FUNCTION: USA 0x0015121E
// PROTOTYPE: PREVIEW 0x0053A89E identical
// NAME: proto-pdb:default.pdb
SwampPuddle::~SwampPuddle()
{
}

// FUNCTION: USA 0x00151254
// PROTOTYPE: PREVIEW 0x0053A8D4 identical
// NAME: proto-pdb:default.pdb
void SwampPuddle::Render(const Matrix& m) const
{
    ResourceHandle<TextureResource> eMap;
    float ref = 0.0f;
    if (_mtl.IsValid()) {
        eMap = _mtl().GetEnvmapTexture();
        ref = _mtl().GetReflectivity();
    }
    if (_envMap.IsValid())
        eMap = _envMap;
    if (eMap.IsValid()) {
        RenderSetup(m, eMap, ref);
        // !FAKE: inliner; hand form of the XDK inline D3DDevice_SetRenderState body; retail encoding, constant-folded
        D3DDevice_SetRenderState_Simple(0x4035C, 1);
        D3DDevice_DrawVerticesUP(8, 4, _verts, sizeof(WaterVertex));
        RenderCleanup();
        if (_mtl.IsValid() && _mtl().GetDetailType() != 0 && (_mtl().GetDetailType() <= 0xC || _mtl().GetDetailType() > 0xF)) {
            RenderOverlaySetup(m, _mtl);
            D3DDevice_DrawVerticesUP(8, 4, _verts, sizeof(WaterVertex));
            RenderOverlayCleanup();
        }
    }
}

// FUNCTION: USA 0x0015132B
// NAME: proto-pdb:default.pdb
SwampWaterNode::SwampWaterNode(const ResourceHandle<ModelResource>& model, const ResourceHandle<MaterialResource>& mtl,
                               float density, unsigned int fieldsize)
    : SwampSceneNode(0xE), WaterField(fieldsize, fieldsize, true), _density(density), _mtl(mtl), _model(model),
      _vertexBuffer(0)
{
    if (_model.IsValid()) {
        // !FAKE: c1xx front end; stand-in locals for the vertex count and loop index; no PDB local
        int v0 = _model().GetVertexCount();
        char buff[0x80];
        _vertexBuffer = D3DDevice_CreateVertexBuffer2(v0 * sizeof(WaterVertex));
        // !FAKE: c1xx front end; stand-in local for the XDK inline wrapper's HRESULT; no PDB local
        long v1 = _vertexBuffer ? 0 : 0x8007000E;
        if (v1 < 0)
            D3DXGetErrorStringA(v1, buff, sizeof(buff));
        // !FAKE: c2 register allocation; stand-in local for the XDK inline Lock's buffer; no PDB local
        D3DVertexBuffer* v3 = _vertexBuffer;
        WaterVertex* dverts = (WaterVertex*)D3DVertexBuffer_Lock2(v3, 0);
        GatorVertex* sverts = (GatorVertex*)D3DVertexBuffer_Lock2(_model().GetVertexBuffer(), 0);
        const AABox& bb = model().GetBounds();
        // !FAKE: c2 x87 scheduling; stand-in local for the bump scale; no PDB local
        float v4 = density * 0.16f;
        for (int v2 = 0; v2 < v0; v2++, dverts++, sverts++) {
            dverts->pos = sverts->loc;
            dverts->normal.Set(0.0f, 1.0f, 0.0f);
            dverts->bumpcoord.u = (sverts->loc.x - bb.low.x) * v4;
            dverts->bumpcoord.v = (sverts->loc.z - bb.low.z) * v4;
            dverts->texcoord = sverts->tex[0];
            dverts->detailcoord = sverts->tex[1];
        }
        SetOption(UseAlpha, true, false);
        _BoundingBox = bb;
    } else
        SetOption(DontDraw, true, false);
}

// FUNCTION: USA 0x00151496
// PROTOTYPE: PREVIEW 0x0053AB16 identical
// NAME: proto-pdb:default.pdb
void SwampWaterNode::Render(const Matrix& m) const
{
    if (_model.IsValid()) {
        float ref = 0.0f;
        SwampRenderer& renderer = g_swamp->GetRenderer();
        ResourceHandle<TextureResource> eMap;
        if (_mtl.IsValid()) {
            eMap = _mtl().GetEnvmapTexture();
            ref = _mtl().GetReflectivity();
        }
        if (!eMap.IsValid())
            eMap = g_swamp->GetMaterialManager().GetDefaultEnvMap();
        if (eMap.IsValid()) {
            RenderSetup(m, eMap, ref);
            Vector3 camPos = g_swamp->GetCamera()->GetWorldPosition();
            // !FAKE: c2 x87 operand order; stand-in local v0 for the surface height (no PDB local); retail keeps it on the FPU stack and fcompp's camPos.y (98.26 -> 100)
            float v0 = GetWorldPosition().y + GetBounds().low.y;
            if (camPos.y < v0) {
                renderer.SetCullMode(0x900);
                // !FAKE: inliner; hand form of the XDK inline D3DDevice_SetRenderState body; retail encoding, constant-folded
                D3DDevice_SetRenderState_Simple(0x4035C, 1);
            }
            renderer.SetStreamSource(_vertexBuffer, 0x30);
            _model().LowLevelRender(true);
            RenderCleanup();
            if (_mtl.IsValid() && _mtl().GetDetailType() != 0 && (_mtl().GetDetailType() <= 0xC || _mtl().GetDetailType() > 0xF)) {
                RenderOverlaySetup(m, _mtl);
                _model().LowLevelRender(true);
                RenderOverlayCleanup();
            }
            renderer.SetStreamSource(0, 0);
            renderer.SetCullMode(0x901);
        }
    }
}
