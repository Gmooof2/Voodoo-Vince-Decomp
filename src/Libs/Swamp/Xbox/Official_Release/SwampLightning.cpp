// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampLightning.cpp OBJ: Xbox\Official_Release\SwampLightning.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <xmmintrin.h>

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    double GetGameTimeAccurate() const { return _gameTimeInternal; }

    char field_0[0x10];                // 0x00
    double _gameTimeInternal;          // 0x10
};

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

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> GetCircleTexture() const { return _circleTexture; }

    char field_0[0x5C];                // 0x00
    ResourceHandle<TextureResource> _circleTexture; // 0x5C
    char field_60[0x24];               // 0x60
};
DECOMP_SIZE_ASSERT(TextureManager, 0x84);

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    int GetScreenWidth() const { return _screenWidth; }
    int GetScreenHeight() const { return _screenHeight; }
    bool EnableDepthBuffering(bool enable);
    void SetTexture(unsigned long, const ResourceHandle<TextureResource>&);
    void SetEmptyTexture(unsigned long stage);
    long SetVertexShader(unsigned long);
    void SetBasicPixelShader();

    char field_0[0x2C];                // 0x00
    int _screenWidth;                  // 0x2C
    int _screenHeight;                 // 0x30
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    SwampRenderer& GetRenderer() const { return *_renderer; }
    TextureManager& GetTextureManager() const { return *_textureManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x2C];                // 0x0C
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x2C];               // 0x3C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct Vector2 { // NAME: proto-pdb:default.pdb
    void Set(float u_, float v_) { u = u_; v = v_; }

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Vector4 { // NAME: proto-pdb:default.pdb
    void Set(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct FlashVertex { // NAME: proto-pdb:default.pdb
    FlashVertex() {}

    Vector4 loc;                       // 0x00
    unsigned int color;                // 0x10
    Vector2 tex;                       // 0x14
};
DECOMP_SIZE_ASSERT(FlashVertex, 0x1C);

enum _D3DPRIMITIVETYPE { // NAME: proto-pdb:default.pdb
    D3DPT_QUADLIST = 8
};

extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_Simple
extern "C" void __stdcall D3DDevice_DrawVerticesUP(_D3DPRIMITIVETYPE, unsigned int, const void*, unsigned int); // src proto-pdb:default.pdb D3DDevice_DrawVerticesUP

struct AABox;   // NAME: proto-pdb:default.pdb
struct Vector3; // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb
struct Matrix;  // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                              // vtable+0x00
    virtual const AABox& GetBounds() const;                 // vtable+0x04
    virtual Vector3 GetWorldPosition() const;               // vtable+0x08
    virtual Quaternion GetWorldRotation() const;            // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);          // vtable+0x10
    virtual void Invalidate();                              // vtable+0x14
    virtual void UpdateAnimation(unsigned int);             // vtable+0x18
    virtual void Update();                                  // vtable+0x1C
    virtual void Render(const Matrix&) const;               // vtable+0x20

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampLightningFlash : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetScreenLocation(float x, float y);
    virtual void Update();                                  // vtable+0x1C
    virtual void Render(const Matrix&) const;               // vtable+0x20

    double _startTime;                // 0xA8
    double _peakTime;                  // 0xB0
    double _finishTime;                // 0xB8
    unsigned int _color;               // 0xC0
    float _x;                          // 0xC4
    float _y;                          // 0xC8
    float _halfWidth;                  // 0xCC
    float _halfHeight;                 // 0xD0
    char field_D4[0xD8 - 0xD4];        // 0xD4
};
DECOMP_SIZE_ASSERT(SwampLightningFlash, 0xD8);

// FUNCTION: USA 0x000D08CA
// PROTOTYPE: PREVIEW 0x004BBB85 identical
// NAME: proto-pdb:default.pdb
void SwampLightningFlash::SetScreenLocation(float x, float y)
{
    _x = x;
    _y = y;
}

// FUNCTION: USA 0x000D0973
// PROTOTYPE: PREVIEW 0x004BBC2E identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 96.91, bytes mismatch (g_swamp load kept in edx across the branch; retail reloads it into eax)
void SwampLightningFlash::Update()
{
    // !FAKE: c2 x87 scheduling; stand-in local; the time is read once (no PDB local)
    double v0 = g_swamp->GetInput().GetGameTimeAccurate();
    if (v0 >= _finishTime) {
        Invalidate();
        return;
    }
    // !FAKE: c2 x87 scheduling; stand-in local; no PDB local
    float v1 = 1.0 - (v0 - _peakTime) / ((v0 >= _peakTime ? _finishTime : _startTime) - _peakTime);
    // !FAKE: c2 x87 scheduling; stand-in local; no PDB local
    float v2 = v1 * 3.0f;
    _halfWidth = g_swamp->GetRenderer().GetScreenWidth() * v2;
    _halfHeight = g_swamp->GetRenderer().GetScreenHeight() * v2;
    // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand (as Sprite.cpp); no PDB local
    float v3 = v1 * 255.0;
    _color &= 0x00FFFFFF;
    _color |= _mm_cvtt_ss2si(_mm_load_ss(&v3)) << 24;
}
#endif

// FUNCTION: USA 0x000D0A30
// PROTOTYPE: PREVIEW 0x004BBCEB identical
// NAME: proto-pdb:default.pdb
void SwampLightningFlash::Render(const Matrix&) const
{
    // !FAKE: c2 register allocation; stand-in local; retail keeps the renderer in ebx (no PDB local)
    SwampRenderer& v0 = g_swamp->GetRenderer();
    v0.SetVertexShader(0x144);
    v0.SetBasicPixelShader();
    v0.SetTexture(0, g_swamp->GetTextureManager().GetCircleTexture());
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 1);
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    bool wasDepth = v0.EnableDepthBuffering(false);
    FlashVertex v[4];
    v[0].loc.Set(_x - _halfWidth, _y - _halfHeight, 0.0f, 1.0f);
    v[1].loc.Set(_halfWidth + _x, _y - _halfHeight, 0.0f, 1.0f);
    v[2].loc.Set(_halfWidth + _x, _halfHeight + _y, 0.0f, 1.0f);
    v[3].loc.Set(_x - _halfWidth, _halfHeight + _y, 0.0f, 1.0f);
    v[0].tex.Set(0.0f, 0.0f);
    v[1].tex.Set(1.0f, 0.0f);
    v[2].tex.Set(1.0f, 1.0f);
    v[3].tex.Set(0.0f, 1.0f);
    // !FAKE: c2 loop; stand-in loop counter; no PDB local
    for (int v1 = 0; v1 < 4; v1++)
        v[v1].color = _color;
    D3DDevice_DrawVerticesUP(D3DPT_QUADLIST, 4, v, sizeof(FlashVertex));
    v0.SetEmptyTexture(0);
    v0.EnableDepthBuffering(wasDepth);
    D3DDevice_SetRenderState_Simple(0x40304, 0);
}
