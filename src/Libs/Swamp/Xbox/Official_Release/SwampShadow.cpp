// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampShadow.cpp OBJ: Xbox\Official_Release\SwampShadow.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#ifdef NON_MATCHING
#include <xmmintrin.h> // _mm_rsqrt_ss for the guarded RenderNow draft
#endif

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator;
template <class T, class A> class vector;
}

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
    float LengthSquared() const { return x * x + y * y + z * z; }
    Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class ModelResource;                   // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource)
    {
        if (_Resource)
            _Resource->AddRef();
    }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};
struct AABox;                          // NAME: proto-pdb:default.pdb

class ProjectModelShadow { // NAME: proto-pdb:default.pdb
public:
    ProjectModelShadow(const ResourceHandle<ModelResource>& model, const AABox& bounds,
                       const std::vector<bool, std::allocator<bool> >& stripControl);

    Vector3 _lightLoc;                 // 0x00
    Vector3 _lightColor;               // 0x0C
    Vector3 _lightDir;                 // 0x18
    float _lightConeCos;               // 0x24
    const ResourceHandle<ModelResource>& _model; // 0x28
    const AABox& _bounds;              // 0x2C
    const std::vector<bool, std::allocator<bool> >& _stripControl; // 0x30
};
DECOMP_SIZE_ASSERT(ProjectModelShadow, 0x34);

struct Vector4 { // NAME: proto-pdb:default.pdb
    void Set(const Vector3& v) { x = v.x; y = v.y; z = v.z; w = 1.0f; }
    void Set(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    void Scaling(float s);
    void Multiply(const Matrix& m, Matrix& result) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00 std::string _Name
    unsigned int _RefCount;            // 0x1C
};

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

struct _D3DVERTEXATTRIBUTEFORMAT; // NAME: proto-pdb:default.pdb
struct _D3DMATRIX;                // NAME: proto-pdb:default.pdb

enum _D3DPRIMITIVETYPE { // NAME: proto-pdb:default.pdb
    D3DPT_QUADLIST = 8
};

enum _D3DTRANSFORMSTATETYPE { // NAME: proto-pdb:default.pdb
    D3DTS_VIEW = 0
};

extern "C" long __stdcall D3DDevice_GetTransform(_D3DTRANSFORMSTATETYPE, _D3DMATRIX*); // NAME: proto-pdb:default.pdb
extern "C" void __fastcall D3DDevice_SetVertexShaderConstant4(int, const void*); // NAME: proto-pdb:default.pdb
extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // NAME: proto-pdb:default.pdb

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void DrawPrimitiveUP(_D3DPRIMITIVETYPE, unsigned int, const void*, unsigned int) const;
    void SetEmptyTexture(unsigned long);
    void GetProjectionTransform(Matrix&) const;
    void SetBasicPixelShader();
    void SetTexture(unsigned long, const ResourceHandle<TextureResource>&);
    void SetBasicVertexShader(const _D3DVERTEXATTRIBUTEFORMAT*);
};

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> GetCircleTexture() const { return _circleTexture; }

    char field_0[0x5C];                // 0x00
    ResourceHandle<TextureResource> _circleTexture; // 0x5C
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { UseAlpha = 0x8 }; // NAME: proto-pdb:default.pdb (enumerators not reached omitted)

    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    bool GetRenderingOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }

    char field_4[0x88 - 0x4];          // 0x04
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampCamera* GetCamera() const { return _currentCamera; }
    SwampRenderer& GetRenderer() const { return *_renderer; }
    TextureManager& GetTextureManager() const { return *_textureManager; }

    char field_0[0x8];                 // 0x00
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x38 - 0xC];          // 0x0C
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x40 - 0x3C];        // 0x3C
    SwampCamera* _currentCamera;       // 0x40
    char field_44[0x68 - 0x44];        // 0x44
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};
class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
};
class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
};

struct ShadowVertex { // NAME: proto-pdb:default.pdb
    static const _D3DVERTEXATTRIBUTEFORMAT* GetVAF();

    Vector3 loc;                       // 0x00
    short u;                           // 0x0C
    short v;                           // 0x0E
    Vector4 color;                     // 0x10
};
DECOMP_SIZE_ASSERT(ShadowVertex, 0x20);

class SwampShadow : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void RenderNow(ResourceHandle<TextureResource> tex, float alphaFactor) const;

    GatorNode* _parent;                // 0xA4
    float _radius;                     // 0xA8
    float _scale;                      // 0xAC
    float _opacity;                    // 0xB0
    mutable ShadowVertex _verts[4];    // 0xB4
};
DECOMP_SIZE_ASSERT(SwampShadow, 0x134);

// FUNCTION: USA 0x000D6E93
// PROTOTYPE: PREVIEW 0x004C2424 identical
// NAME: proto-pdb:default.pdb
ProjectModelShadow::ProjectModelShadow(const ResourceHandle<ModelResource>& model, const AABox& bounds,
                                       const std::vector<bool, std::allocator<bool> >& stripControl)
    : _lightDir(0.0f, 0.0f, 1.0f), _lightConeCos(-1.0f), _model(model), _bounds(bounds), _stripControl(stripControl)
{
}

// FUNCTION: USA 0x000D701B
// PROTOTYPE: PREVIEW 0x004C25AC identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 83.58, bytes mismatch; wall: retail's inlined Vector3::ApproxNormalize is inline asm rsqrtss
// (no `and esp,-16`; the _mm_rsqrt_ss intrinsic draft adds it), D116 forbids __asm (C0219); out of line 81.65
void SwampShadow::RenderNow(ResourceHandle<TextureResource> tex, float alphaFactor) const
{
    SwampRenderer& renderer = g_swamp->GetRenderer();
    if (_scale != 0.0f) {
        Vector3 loc = GetWorldPosition();
        // !FAKE: inliner; pointer local to the call, hand form of the inlined free operator- (no PDB record of a free operator, D78); no before/after recorded
        const Vector3* p = &g_swamp->GetCamera()->GetWorldPosition();
        Vector3 dir2cam;
        dir2cam.x = p->x - loc.x;
        dir2cam.y = p->y - loc.y;
        dir2cam.z = p->z - loc.z;
        {
            // !FAKE: inliner; stand-in locals, hand form of the inlined Vector3::ApproxNormalize (/O1 calls it); no before/after recorded
            float v5 = dir2cam.LengthSquared();
            if (v5 > 0.0f) {
                float v6;
                _mm_store_ss(&v6, _mm_rsqrt_ss(_mm_load_ss(&v5)));
                dir2cam.x *= v6;
                dir2cam.y *= v6;
                dir2cam.z *= v6;
            }
        }
        // !FAKE: inliner; stand-in local, hand form of the inlined free operator* (no PDB record of a free operator, D78); no before/after recorded
        const Vector3& v1 = Vector3(dir2cam.x * 0.02f, dir2cam.y * 0.02f, dir2cam.z * 0.02f);
        loc += v1;
        Matrix scaleAndLoc;
        scaleAndLoc.Scaling(_radius * _scale);
        scaleAndLoc.row[3].Set(loc);
        Matrix view;
        D3DDevice_GetTransform(D3DTS_VIEW, (_D3DMATRIX*)&view);
        Matrix proj;
        renderer.GetProjectionTransform(proj);
        // !FAKE: inliner; hand form of the retail-inlined Matrix product by value: stand-ins v2-v4 for its result and copy temps (as Decal::Render); no before/after recorded
        Matrix v2;
        scaleAndLoc.Multiply(view, v2);
        Matrix v3 = v2;
        Matrix v4;
        v3.Multiply(proj, v4);
        Matrix transform = v4;
        // retail: register 0 biased by 96 (the XDK inline SetVertexShaderConstant form)
        D3DDevice_SetVertexShaderConstant4(0x60, &transform);
        renderer.SetBasicVertexShader(ShadowVertex::GetVAF());
        renderer.SetBasicPixelShader();
        float clr = 0.0f;
        if (tex._Resource) {
            renderer.SetTexture(0, tex);
            clr = 1.0f;
        } else
            renderer.SetTexture(0, g_swamp->GetTextureManager().GetCircleTexture());
        D3DDevice_SetRenderState_Simple(0x40304, 1);
        D3DDevice_SetRenderState_Simple(0x40344, 0x302);
        D3DDevice_SetRenderState_Simple(0x40348, 0x303);
        D3DDevice_SetRenderState_Simple(0x4035C, 0);
        for (int i = 0; i < 4; i++)
            _verts[i].color.Set(clr, clr, clr, alphaFactor * _opacity);
        renderer.DrawPrimitiveUP(D3DPT_QUADLIST, 1, _verts, sizeof(ShadowVertex));
        renderer.SetEmptyTexture(0);
        D3DDevice_SetRenderState_Simple(0x4035C, 1);
        if (_parent->GetRenderingOption(SwampSceneNode::UseAlpha)) {
            D3DDevice_SetRenderState_Simple(0x40344, 0x8003);
            D3DDevice_SetRenderState_Simple(0x40348, 0x8004);
        } else
            D3DDevice_SetRenderState_Simple(0x40304, 0);
    }
}
#endif
