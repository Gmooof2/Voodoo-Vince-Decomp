// UNIT: src/Libs/Swamp/Xbox/Official_Release/Sprite.cpp OBJ: Xbox\Official_Release\Sprite.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <xmmintrin.h>

struct D3DTexture; // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class SingleTexture;   // NAME: proto-pdb:default.pdb
class AnimatedTexture; // NAME: proto-pdb:default.pdb
class MovieTexture;    // NAME: proto-pdb:default.pdb

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    D3DTexture* GetTexture() const;

    SingleTexture* _singleTexture;     // 0x24
    AnimatedTexture* _animatedTexture; // 0x28
    MovieTexture* _movieTexture;       // 0x2C
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    T& operator()() { return *_Resource; }
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

class SwampRenderer; // NAME: proto-pdb:default.pdb

class Swamp { // NAME: proto-pdb:default.pdb
public:
    TextureManager& GetTextureManager() const { return *_textureManager; }
    SwampRenderer& GetRenderer() const { return *_renderer; }

    char field_0[0x8];                 // 0x00
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x2C];                // 0x0C
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x2C];               // 0x3C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SingleTexture { // NAME: proto-pdb:default.pdb
    friend class TextureResource;
protected:
    D3DTexture* GetTexture() const;

    D3DTexture* _texPtr;               // 0x00
    char field_4[0x1C];                // 0x04
};
DECOMP_SIZE_ASSERT(SingleTexture, 0x20);

class AnimatedTexture { // NAME: proto-pdb:default.pdb
    friend class TextureResource;
protected:
    D3DTexture* GetTexture() const;

    char field_0[0x20];                // 0x00
};
DECOMP_SIZE_ASSERT(AnimatedTexture, 0x20);

class MovieTexture { // NAME: proto-pdb:default.pdb
    friend class TextureResource;
protected:
    D3DTexture* GetTexture() const;

    char field_0[0x8];                 // 0x00
};
DECOMP_SIZE_ASSERT(MovieTexture, 0x8);

struct Vector2 { // NAME: proto-pdb:default.pdb
    void Set(float u_, float v_) { u = u_; v = v_; }

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Vector4 { // NAME: proto-pdb:default.pdb
    // !FAKE: PDB declares Vector4(); an empty inline one makes Matrix() call a vector ctor iterator
    // (out-of-line ??0Matrix call) that retail Render lacks

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

class Matrix { // NAME: proto-pdb:default.pdb
public:
    Matrix() {}
    void Rotation(float, float, float);
    void Apply(const Vector4&, Vector4&) const;
    // !FAKE: __forceinline; plain inline stays an out-of-line call at /O1, retail inlines it
    __forceinline Vector4 Apply(const Vector4& v) const { Vector4 r; Apply(v, r); return r; }

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

enum _D3DPRIMITIVETYPE { // NAME: proto-pdb:default.pdb
    D3DPT_QUADLIST = 8
};

enum _D3DFORMAT { // NAME: proto-pdb:default.pdb
    D3DFMT_UNKNOWN = 0xFF
};

enum _D3DRESOURCETYPE { // NAME: proto-pdb:default.pdb
    D3DRTYPE_NONE = 0
};

struct _D3DSURFACE_DESC { // NAME: proto-pdb:default.pdb
    _D3DFORMAT Format;                 // 0x00
    _D3DRESOURCETYPE Type;             // 0x04
    unsigned long Usage;               // 0x08
    unsigned int Size;                 // 0x0C
    unsigned long MultiSampleType;     // 0x10
    unsigned int Width;                // 0x14
    unsigned int Height;               // 0x18
};
DECOMP_SIZE_ASSERT(_D3DSURFACE_DESC, 0x1C);

extern "C" void __stdcall D3DTexture_GetLevelDesc(D3DTexture*, unsigned int, _D3DSURFACE_DESC*); // src proto-pdb:default.pdb D3DTexture_GetLevelDesc

extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_Simple
extern "C" void __stdcall D3DDevice_DrawVerticesUP(_D3DPRIMITIVETYPE, unsigned int, const void*, unsigned int); // src proto-pdb:default.pdb D3DDevice_DrawVerticesUP

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    bool EnableDepthBuffering(bool enable);
    void SetTexture(unsigned long, const ResourceHandle<TextureResource>&);
    long SetVertexShader(unsigned long);
    void SetBasicPixelShader();
};

class Sprite { // NAME: proto-pdb:default.pdb
public:
    void SetUV(float u1, float v1, float u2, float v2);
    void Render() const;

private:
    void ComputeSize();

public:
    struct SpriteVertex { // NAME: proto-pdb:default.pdb
        SpriteVertex() {}

        Vector4 loc;                   // 0x00
        unsigned int color;            // 0x10
        Vector2 tex;                   // 0x14
    };

    Vector2 _loc;                      // 0x00
    int _w;                            // 0x08
    int _h;                            // 0x0C
    Vector2 _scale;                    // 0x10
    ResourceHandle<TextureResource> _texture; // 0x18
    bool _centered;                    // 0x1C
    float _alpha;                      // 0x20
    Vector2 _uv[4];                    // 0x24
    unsigned int _color;               // 0x44
    float _rotation;                   // 0x48
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

// FUNCTION: USA 0x000BD585
// PROTOTYPE: PREVIEW 0x004A82DC identical
// NAME: proto-pdb:default.pdb
void Sprite::SetUV(float u1, float v1, float u2, float v2)
{
    _uv[0].u = _uv[3].u = u1;
    _uv[0].v = _uv[1].v = v1;
    _uv[1].u = _uv[2].u = u2;
    _uv[2].v = _uv[3].v = v2;
}

// FUNCTION: USA 0x000BD61C
// PROTOTYPE: PREVIEW 0x004A8373 identical
// NAME: proto-pdb:default.pdb
D3DTexture* TextureResource::GetTexture() const
{
    if (_singleTexture)
        return _singleTexture->GetTexture();
    if (_animatedTexture)
        return _animatedTexture->GetTexture();
    if (_movieTexture)
        return _movieTexture->GetTexture();
    return g_swamp->GetTextureManager().GetDefaultTexture()().GetTexture();
}

// FUNCTION: USA 0x000BD667
// PROTOTYPE: PREVIEW 0x004A83BE identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match; held out: progress.py refuses the done row
D3DTexture* SingleTexture::GetTexture() const
{
    if (_texPtr)
        return _texPtr;
    return g_swamp->GetTextureManager().GetDefaultTexture()().GetTexture();
}
#endif

// FUNCTION: USA 0x000BD690
// PROTOTYPE: PREVIEW 0x004A83E7 identical
// NAME: proto-pdb:default.pdb
void Sprite::ComputeSize()
{
    if (_texture.IsValid()) {
        D3DTexture* tex = _texture().GetTexture();
        if (tex) {
            _D3DSURFACE_DESC desc;
            D3DTexture_GetLevelDesc(tex, 0, &desc);
            _w = desc.Width;
            _h = desc.Height;
        } else {
            _w = _h = 1;
        }
    } else {
        _w = _h = 0;
    }
}

// FUNCTION: USA 0x000BD746
// PROTOTYPE: PREVIEW 0x004A849D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.6
void Sprite::Render() const
{
    if (!_w || !_h)
        return;
    const float w = _w * _scale.u;
    const float h = _h * _scale.v;
    float x1 = 0.0f;
    float y1 = 0.0f;
    if (_centered) {
        x1 = w * -0.5f;
        y1 = h * -0.5f;
    }
    float v1 = _alpha * 255.0f;
    SpriteVertex v[4];
    for (int i = 0; i < 4; i++) {
        v[i].color = (_mm_cvtt_ss2si(_mm_load_ss(&v1)) << 24) | _color;
        v[i].loc.z = 0.5f;
        v[i].loc.w = 1.0f;
        v[i].tex.Set(_uv[i].u, _uv[i].v);
    }
    float x2 = x1 + w;
    float y2 = y1 + h;
    v[0].loc.x = x1;
    v[0].loc.y = y1;
    v[1].loc.x = x2;
    v[1].loc.y = y1;
    v[2].loc.x = x2;
    v[2].loc.y = y2;
    v[3].loc.x = x1;
    v[3].loc.y = y2;
    if (_rotation != 0.0f) {
        Matrix rot;
        rot.Rotation(0.0f, 0.0f, _rotation);
        for (int i = 0; i < 4; i++)
            v[i].loc = rot.Apply(v[i].loc);
    }
    for (int i = 0; i < 4; i++) {
        v[i].loc.x = _loc.u + v[i].loc.x;
        v[i].loc.y = _loc.v + v[i].loc.y;
    }
    SwampRenderer& r = g_swamp->GetRenderer();
    bool wasDepth = r.EnableDepthBuffering(false);
    r.SetTexture(0, _texture);
    r.SetVertexShader(0x144);
    r.SetBasicPixelShader();
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 0x303);
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    D3DDevice_DrawVerticesUP(D3DPT_QUADLIST, 4, v, sizeof(SpriteVertex));
    D3DDevice_SetRenderState_Simple(0x40304, 0);
    r.EnableDepthBuffering(wasDepth);
}
#endif
