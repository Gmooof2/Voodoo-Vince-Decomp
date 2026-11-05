// UNIT: src/Libs/Swamp/Xbox/Official_Release/swamptextureeffects.cpp OBJ: Xbox\Official_Release\SwampTextureEffects.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct D3DResource { // NAME: proto-pdb:default.pdb
    unsigned long Common;              // 0x00
    unsigned long Data;                // 0x04
    unsigned long Lock;                // 0x08
};
DECOMP_SIZE_ASSERT(D3DResource, 0xC);

struct D3DPixelContainer : public D3DResource { // NAME: proto-pdb:default.pdb
    unsigned long Format;              // 0x0C
    unsigned long Size;                // 0x10
};
DECOMP_SIZE_ASSERT(D3DPixelContainer, 0x14);

struct D3DBaseTexture; // NAME: proto-pdb:default.pdb

struct D3DSurface : public D3DPixelContainer { // NAME: proto-pdb:default.pdb
    D3DBaseTexture* Parent;            // 0x14
};
DECOMP_SIZE_ASSERT(D3DSurface, 0x18);

extern "C" long __stdcall D3DDevice_SetRenderTarget(D3DSurface* pRenderTarget, D3DSurface* pNewZStencil); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_SetRenderState_StencilEnable(unsigned long value); // NAME: proto-pdb:default.pdb

class ResourceCache { // NAME: proto-pdb:default.pdb
public:
    bool AllocateHunk(unsigned int size, void** ptr, unsigned long protectFlags);
    void FreeHunk(void*);
};

class BundleWrapper { // NAME: proto-pdb:default.pdb
public:
    static ResourceCache& GetMemoryPool();
};

enum _D3DFORMAT { // NAME: proto-pdb:default.pdb
    D3DFMT_R5G6B5 = 0x5,
    D3DFMT_LIN_A8R8G8B8 = 0x12,
    D3DFMT_LIN_D24S8 = 0x2E,
    D3DFMT_LIN_D16 = 0x30
};

extern "C" void __stdcall XGSetSurfaceHeader(unsigned int Width, unsigned int Height, _D3DFORMAT Format, D3DSurface* pSurface,
                                             unsigned int Data, unsigned int Pitch); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DResource_Register(D3DResource* pThis, void* pBase); // NAME: proto-pdb:default.pdb

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true, 0); }

protected:
    void _Tidy(bool _Built, unsigned int _Newsize);

private:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    char _Alval;                       // 0x00
    union {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct _D3DVERTEXATTRIBUTEFORMAT; // NAME: proto-pdb:default.pdb

struct SpookyVertex { // NAME: proto-pdb:default.pdb
    static const _D3DVERTEXATTRIBUTEFORMAT* GetVAF();
};

struct D3DTexture; // NAME: proto-pdb:default.pdb
struct D3DDevice; // NAME: proto-pdb:default.pdb

extern "C" long __stdcall D3DXCreateTexture(D3DDevice*, unsigned int, unsigned int, unsigned int, unsigned long, _D3DFORMAT,
                                            unsigned long, D3DTexture**); // NAME: proto-pdb:default.pdb
template <class T> void RELEASE(T*& p); // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource)
    {
        if (_Resource)
            _Resource->AddRef();
    }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }
    ResourceHandle& operator=(const ResourceHandle& h);
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class PixelShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class VertexShaderManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<VertexShaderResource> CreateShader(const SwampString& name, const _D3DVERTEXATTRIBUTEFORMAT* vaf);
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class PixelShaderManager : public ResourceManager<PixelShaderResource> { // NAME: proto-pdb:default.pdb
};

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> CreateCubemap(int size, _D3DFORMAT format);
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    const Vector3& SetLocation(const Vector3&);
    const Quaternion& SetFacing(const Vector3&);

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampCamera();
    virtual ~SwampCamera();
    void SetFOV(float);
    void SetAspectRatio(float);
    void SetUp(const Vector3& up) { _up = up; }

    char field_A4[0xB4 - 0xA4];        // 0xA4
    Vector3 _up;                       // 0xB4
    char field_C0[0x10C - 0xC0];       // 0xC0
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void ClearScreen(unsigned int, bool, bool, bool) const;
    void RenderToTexture(const ResourceHandle<TextureResource>&, D3DTexture*, unsigned int);
    void RenderToScreen();
    D3DDevice* GetDevice() const { return _D3DDevice; }

    char field_0[0x20];                // 0x00
    D3DDevice* _D3DDevice;             // 0x20
};

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    float RenderScene();
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampRenderer& GetRenderer() const { return *_renderer; }
    SwampScene& GetScene() const { return *_scene; }
    TextureManager& GetTextureManager() const { return *_textureManager; }
    PixelShaderManager& GetPixelShaderManager() const { return *_pixelShaderManager; }
    VertexShaderManager& GetVertexShaderManager() const { return *_vertexShaderManager; }
    SwampCamera* SetCamera(SwampCamera* camera)
    {
        SwampCamera* old = _currentCamera;
        _currentCamera = camera;
        return old;
    }

    char field_0[0x8];                 // 0x00
    SwampRenderer* _renderer;          // 0x08
    SwampScene* _scene;                // 0x0C
    char field_10[0x2C - 0x10];        // 0x10
    PixelShaderManager* _pixelShaderManager;   // 0x2C
    char field_30[0x38 - 0x30];        // 0x30
    TextureManager* _textureManager;   // 0x38
    VertexShaderManager* _vertexShaderManager; // 0x3C
    SwampCamera* _currentCamera;       // 0x40
    char field_44[0x68 - 0x44];        // 0x44
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class DrawLevel { // NAME: proto-pdb:default.pdb
public:
    static bool AreWeMirroring() { return _mirroringHappening; }
    static bool AreMirrorsEnabled() { return _mirrorsEnabled; }
    static void EnableMirrors(bool);

protected:
    static bool _mirroringHappening;
    static bool _mirrorsEnabled;
};

class SwampTextureEffects { // NAME: proto-pdb:default.pdb
public:
    static ResourceHandle<TextureResource> GenerateEnvironmentMap(const Vector3& loc, unsigned int size);
};

class SwampSpookyRenderer { // NAME: proto-pdb:default.pdb
public:
    SwampSpookyRenderer(unsigned int bufferWidth, unsigned int bufferHeight, unsigned int backgroundColor, unsigned int flags);
    ~SwampSpookyRenderer();
    void BeginRender();

    unsigned int _bufferWidth;         // 0x00
    unsigned int _bufferHeight;        // 0x04
    unsigned int _backgroundColor;     // 0x08
    D3DSurface _renderTarget;          // 0x0C
    D3DSurface _depthTarget;           // 0x24
    D3DSurface _depthTargetAlias;      // 0x3C
    unsigned int _flags;               // 0x54

protected:
    static void* _colorBuffer;
    static void* _depthBuffer;
    static unsigned int _largestBufferWidth;
    static unsigned int _largestBufferHeight;
    static unsigned int _refCount;
    static ResourceHandle<VertexShaderResource> vertexShader;
    static ResourceHandle<VertexShaderResource> vertexShaderBlur;
    static ResourceHandle<PixelShaderResource> pixelShader;
    static ResourceHandle<PixelShaderResource> pixelShaderBlur;
};
DECOMP_SIZE_ASSERT(SwampSpookyRenderer, 0x58);

// FUNCTION: USA 0x000D63BD
// PROTOTYPE: PREVIEW 0x004C194E identical
// NAME: proto-pdb:default.pdb
SwampSpookyRenderer::~SwampSpookyRenderer()
{
    if (--_refCount == 0) {
        // !FAKE: c2 register allocation; dummy local; pool held in esi across both calls, two GetMemoryPool calls without it (match_one 73.9 -> 100.0)
        ResourceCache* p = &BundleWrapper::GetMemoryPool();
        p->FreeHunk(_depthBuffer);
        p->FreeHunk(_colorBuffer);
        _colorBuffer = 0;
        _depthBuffer = 0;
        _largestBufferWidth = 0;
        _largestBufferHeight = 0;
    }
}

// FUNCTION: USA 0x000D6466
// PROTOTYPE: PREVIEW 0x004C19F7 identical
// NAME: proto-pdb:default.pdb
void SwampSpookyRenderer::BeginRender()
{
    // !FAKE: c2 register allocation; dummy local; renderer loaded into edi before the calls, reloaded after without it (match_one 69.3 -> 100.0)
    SwampRenderer* p = &g_swamp->GetRenderer();
    D3DDevice_SetRenderTarget(&_renderTarget, &_depthTarget);
    if (DrawLevel::AreWeMirroring()) {
        D3DDevice_SetRenderState_StencilEnable(0);
    }
    p->ClearScreen(_backgroundColor, true, true, true);
}

// FUNCTION: USA 0x000D69DF
// PROTOTYPE: PREVIEW 0x004C1F70 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 96.758064, bytes mismatch (frame layout equal, but cl keeps a plain ebp frame where retail biases it, lea ebp,[esp-0x6c]; and g_swamp stays in eax for CreateCubemap where retail reloads it)
ResourceHandle<TextureResource> SwampTextureEffects::GenerateEnvironmentMap(const Vector3& loc, unsigned int size)
{
    // !FAKE: c2 register allocation; dummy local; renderer loaded into ebx before the first call
    SwampRenderer& v1 = g_swamp->GetRenderer();
    ResourceHandle<TextureResource> cubeTex = g_swamp->GetTextureManager().CreateCubemap(size, D3DFMT_R5G6B5);
    if (!cubeTex.IsValid())
        return ResourceHandle<TextureResource>();
    D3DTexture* zTex;
    if (D3DXCreateTexture(v1.GetDevice(), size, size, 1, 1, D3DFMT_LIN_D16, 0, &zTex) < 0)
        return ResourceHandle<TextureResource>();
    SwampCamera camera;
    camera.SetLocation(loc);
    camera.SetFOV(1.5707964f);
    camera.SetAspectRatio(1.0f);
    // !FAKE: c2 register allocation; dummy local; previous camera kept in a frame slot, the PDB records no slot
    SwampCamera* v2 = g_swamp->SetCamera(&camera);
    const Vector3 facings[6] = {
        Vector3(1.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f),
    };
    bool mirrorsWereEnabled = DrawLevel::AreMirrorsEnabled();
    DrawLevel::EnableMirrors(false);
    for (unsigned int i = 0; i < 6; i++) {
        camera.SetFacing(facings[i]);
        camera.SetUp(i == 2 ? Vector3(0.0f, 0.0f, -1.0f) : i == 3 ? Vector3(0.0f, 0.0f, 1.0f) : Vector3(0.0f, 1.0f, 0.0f));
        v1.RenderToTexture(cubeTex, zTex, i);
        v1.ClearScreen(0, true, true, true);
        g_swamp->GetScene().RenderScene();
    }
    g_swamp->SetCamera(v2);
    v1.RenderToScreen();
    DrawLevel::EnableMirrors(mirrorsWereEnabled);
    RELEASE(zTex);
    return cubeTex;
}
#endif

// FUNCTION: USA 0x000D6C00
// PROTOTYPE: PREVIEW 0x004C2191 identical
// NAME: proto-pdb:default.pdb
SwampSpookyRenderer::SwampSpookyRenderer(unsigned int bufferWidth, unsigned int bufferHeight, unsigned int backgroundColor,
                                         unsigned int flags)
    : _bufferWidth(bufferWidth), _bufferHeight(bufferHeight), _backgroundColor(backgroundColor & 0xFFFFFF), _flags(flags)
{
    if (_largestBufferWidth < bufferWidth || _largestBufferHeight < bufferHeight) {
        ResourceCache& memPool = BundleWrapper::GetMemoryPool();
        if (_colorBuffer)
            memPool.FreeHunk(_colorBuffer);
        if (_depthBuffer)
            memPool.FreeHunk(_depthBuffer);
        memPool.AllocateHunk(bufferWidth * bufferHeight * 4, &_colorBuffer, 2);
        memPool.AllocateHunk(bufferWidth * bufferHeight * 4, &_depthBuffer, 2);
        vertexShader = g_swamp->GetVertexShaderManager().CreateShader("Spooky", SpookyVertex::GetVAF());
        vertexShaderBlur = g_swamp->GetVertexShaderManager().CreateShader("SpookyBlur", SpookyVertex::GetVAF());
        pixelShader = g_swamp->GetPixelShaderManager().Load("Spooky");
        pixelShaderBlur = g_swamp->GetPixelShaderManager().Load("SpookyBlur");
    }
    XGSetSurfaceHeader(bufferWidth, bufferHeight, D3DFMT_LIN_A8R8G8B8, &_renderTarget, 0, 0);
    D3DResource_Register(&_renderTarget, _colorBuffer);
    XGSetSurfaceHeader(bufferWidth, bufferHeight, D3DFMT_LIN_D24S8, &_depthTarget, 0, 0);
    D3DResource_Register(&_depthTarget, _depthBuffer);
    XGSetSurfaceHeader(bufferWidth, bufferHeight, D3DFMT_LIN_A8R8G8B8, &_depthTargetAlias, 0, 0);
    D3DResource_Register(&_depthTargetAlias, _depthBuffer);
    _refCount++;
    _largestBufferWidth = bufferWidth;
    _largestBufferHeight = bufferHeight;
}
