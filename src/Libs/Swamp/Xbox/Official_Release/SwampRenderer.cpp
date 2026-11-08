// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampRenderer.cpp OBJ: Xbox\Official_Release\SwampRenderer.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct D3DSurface; // NAME: proto-pdb:default.pdb
struct Direct3D; // NAME: proto-pdb:default.pdb
struct D3DDevice; // NAME: proto-pdb:default.pdb
struct HWND__; // NAME: proto-pdb:default.pdb
struct D3DBaseTexture { }; // NAME: proto-pdb:default.pdb
struct D3DTexture : D3DBaseTexture { }; // NAME: proto-pdb:default.pdb
struct D3DCubeTexture : D3DBaseTexture { }; // NAME: proto-pdb:default.pdb
struct D3DVertexBuffer; // NAME: proto-pdb:default.pdb
struct D3DIndexBuffer; // NAME: proto-pdb:default.pdb
struct _D3DMATRIX; // NAME: proto-pdb:default.pdb
struct _D3DVERTEXATTRIBUTEFORMAT; // NAME: proto-pdb:default.pdb
struct Matrix; // NAME: proto-pdb:default.pdb
class VertexShaderResource; // NAME: proto-pdb:default.pdb
class PixelShaderResource { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x24];                // 0x00
    const unsigned long* _shaderCode;  // 0x24
};

struct _D3DGAMMARAMP { // NAME: proto-pdb:default.pdb
    unsigned char red[256];            // 0x000
    unsigned char green[256];          // 0x100
    unsigned char blue[256];           // 0x200
};
DECOMP_SIZE_ASSERT(_D3DGAMMARAMP, 0x300);

enum _D3DFORMAT { // NAME: proto-pdb:default.pdb
    D3DFMT_X8R8G8B8 = 0x7,
    D3DFMT_D24S8 = 0x2A
};

enum _D3DSWAPEFFECT { // NAME: proto-pdb:default.pdb
    D3DSWAPEFFECT_DISCARD = 0x1
};

enum _D3DDEVTYPE { // NAME: proto-pdb:default.pdb
    D3DDEVTYPE_HAL = 0x1
};

struct _D3DPRESENT_PARAMETERS_ { // NAME: proto-pdb:default.pdb
    unsigned int BackBufferWidth;      // 0x00
    unsigned int BackBufferHeight;     // 0x04
    _D3DFORMAT BackBufferFormat;       // 0x08
    unsigned int BackBufferCount;      // 0x0C
    unsigned long MultiSampleType;     // 0x10
    _D3DSWAPEFFECT SwapEffect;         // 0x14
    HWND__* hDeviceWindow;             // 0x18
    int Windowed;                      // 0x1C
    int EnableAutoDepthStencil;        // 0x20
    _D3DFORMAT AutoDepthStencilFormat; // 0x24
    unsigned long Flags;               // 0x28
    unsigned int FullScreen_RefreshRateInHz; // 0x2C
    unsigned int FullScreen_PresentationInterval; // 0x30
    D3DSurface* BufferSurfaces[3];     // 0x34
    D3DSurface* DepthStencilSurface;   // 0x40
};
DECOMP_SIZE_ASSERT(_D3DPRESENT_PARAMETERS_, 0x44);

extern "C" Direct3D* __stdcall Direct3DCreate8(unsigned int); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall Direct3D_SetPushBufferSize(unsigned long, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall Direct3D_CreateDevice(unsigned int, _D3DDEVTYPE, HWND__*, unsigned long, _D3DPRESENT_PARAMETERS_*, D3DDevice**); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DXGetErrorStringA(long hr, char* buffer, unsigned int size); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_GetGammaRamp(_D3DGAMMARAMP*); // NAME: proto-pdb:default.pdb

enum _D3DTRANSFORMSTATETYPE { // NAME: proto-pdb:default.pdb
    D3DTS_VIEW = 0,
    D3DTS_PROJECTION = 1
};

extern "C" void __stdcall D3DDevice_SetRenderTarget(D3DSurface*, D3DSurface*); // NAME: proto-pdb:default.pdb
enum _D3DCUBEMAP_FACES { // NAME: proto-pdb:default.pdb
    D3DCUBEMAP_FACE_FORCE_DWORD = 0x7FFFFFFF
};
extern "C" D3DSurface* __stdcall D3DTexture_GetSurfaceLevel2(D3DTexture*, unsigned int); // NAME: proto-pdb:default.pdb
extern "C" D3DSurface* __stdcall D3DCubeTexture_GetCubeMapSurface2(D3DCubeTexture*, _D3DCUBEMAP_FACES, unsigned int); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall D3DDevice_Release(); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DDevice_GetTransform(_D3DTRANSFORMSTATETYPE, _D3DMATRIX*); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_SetRenderState_CullMode(unsigned long); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_SetPixelShaderProgram(const unsigned long*); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DDevice_EndVisibilityTest(unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DDevice_GetVisibilityTestResult(unsigned long, unsigned int*, unsigned __int64*); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_SetGammaRamp(unsigned long, const _D3DGAMMARAMP*); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_SetRenderState_ZEnable(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_ZEnable
extern "C" void __stdcall D3DDevice_SetVertexShader(unsigned long);
extern "C" void __stdcall D3DDevice_SetTexture(unsigned long, D3DBaseTexture*); // src proto-pdb:default.pdb D3DDevice_SetTexture
extern "C" long __stdcall D3DDevice_LoadVertexShaderProgram(const unsigned long*, unsigned long); // src proto-pdb:default.pdb D3DDevice_LoadVertexShaderProgram
extern "C" long __stdcall D3DDevice_SelectVertexShaderDirect(const _D3DVERTEXATTRIBUTEFORMAT*, unsigned long); // src proto-pdb:default.pdb D3DDevice_SelectVertexShaderDirect
extern "C" void __stdcall D3DDevice_SetStreamSource(unsigned int, D3DVertexBuffer*, unsigned int);
extern "C" unsigned long __stdcall D3DDevice_Swap(unsigned long); // NAME: proto-pdb:default.pdb
struct _D3DRECT; // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_Clear(unsigned long, const _D3DRECT*, unsigned long, unsigned long, float, unsigned long); // NAME: proto-pdb:default.pdb

struct D3DDevice { // NAME: proto-pdb:default.pdb
    static unsigned long __stdcall Release() { return D3DDevice_Release(); }
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

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    unsigned long GetD3DHandle() const { return _handle; }
    const unsigned long* GetShaderCode() const { return _shaderCode; }
    const _D3DVERTEXATTRIBUTEFORMAT* GetDeclaration() const { return _declaration; }

    unsigned long _handle;             // 0x24
    const unsigned long* _shaderCode;  // 0x28
    const _D3DVERTEXATTRIBUTEFORMAT* _declaration; // 0x2C
};
DECOMP_SIZE_ASSERT(VertexShaderResource, 0x30);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    D3DTexture* GetTexture() const;
    char field_24[0xC];                // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    bool IsValid() const { return _Resource != 0; }
    const T& operator()() const { return *_Resource; }
    bool operator==(const ResourceHandle& h) const { return _Resource == h._Resource; }
    ResourceHandle& operator=(const ResourceHandle& h);

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<PixelShaderResource>, 0x4);

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> GetDefaultTexture() const { return _defaultTexture; }

    char field_0[0x58];                // 0x00
    ResourceHandle<TextureResource> _defaultTexture; // 0x58
    char field_5C[0x28];               // 0x5C
};
DECOMP_SIZE_ASSERT(TextureManager, 0x84);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    TextureManager& GetTextureManager() const { return *_textureManager; }

    char field_0[0x38];                // 0x00
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x2C];               // 0x3C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void InitDirect3D(HWND__* hwnd, const SwampString& renderswitches);
    void SetPresentationParameters(const SwampString&, bool);
    void ClearScreen(unsigned int color, bool clearColor, bool clearZ, bool clearStencil) const;
    void RenderToScreen();
    void RenderToTexture(const ResourceHandle<TextureResource>& color, D3DTexture* depth, unsigned int cubeFace);
    long SetVertexShader(const ResourceHandle<VertexShaderResource>&, const _D3DVERTEXATTRIBUTEFORMAT*);
    long SetVertexShader(const ResourceHandle<VertexShaderResource>& handle);
    long SetPixelShader(const ResourceHandle<PixelShaderResource>&);
    void GetProjectionTransform(Matrix&) const;
    void SetBasicVertexShader(const _D3DVERTEXATTRIBUTEFORMAT*);
    void SetBasicPixelShader();
    void SetCullMode(unsigned long value);
    void ReverseCullMode();
    unsigned int EndRasterTest() const;
    int GetRasterTestResults(unsigned int handle) const;
    void RestoreGammaRamp(bool immediate);
    void SetTexture(unsigned long stage, const ResourceHandle<TextureResource>& tex);
    void SetDefaultTexture(unsigned long stage);
    void SetGammaRamp(const _D3DGAMMARAMP& ramp, bool immediate, bool temp);
    bool EnableDepthBuffering(bool enable);
    bool UseWBuffering(bool useW);
    void SetEmptyTexture(unsigned long stage);
    void EndScene();
protected:
    void ClearStateCache();
public:

    char field_0[0x1C];                // 0x00
    Direct3D* _D3D;                    // 0x1C
    D3DDevice* _D3DDevice;             // 0x20
    D3DSurface* _ColorBuffer;          // 0x24
    D3DSurface* _DepthBuffer;          // 0x28
    int _screenWidth;                  // 0x2C
    int _screenHeight;                 // 0x30
    char field_34[0x44 - 0x34];        // 0x34
    D3DVertexBuffer* _CurrentVB;       // 0x44
    D3DIndexBuffer* _CurrentIB;        // 0x48
    ResourceHandle<VertexShaderResource> _CurrentVertexShader; // 0x4C
    const _D3DVERTEXATTRIBUTEFORMAT* _CurrentVertexDeclaration; // 0x50
    const unsigned long* _CurrentPixelShader; // 0x54
    ResourceHandle<TextureResource> _CurrentTexture[4]; // 0x58
    unsigned int _textureSetCount;     // 0x68
    unsigned int _textureChangeCount;  // 0x6C
    mutable unsigned int _nextRasterHandle; // 0x70 written by the const EndRasterTest, retail
    char field_74[0x4];                // 0x74
    unsigned long _cullMode;           // 0x78
    bool _cullingReversed;             // 0x7C
    bool _depthBufferingEnabled;       // 0x7D
    bool _useWBuffering;               // 0x7E
    char field_7F[0x1];                // 0x7F
    ResourceHandle<VertexShaderResource> _basicVertexShader; // 0x80
    ResourceHandle<PixelShaderResource> _basicPixelShader;   // 0x84
    _D3DGAMMARAMP _gammaRamp;          // 0x88
    char field_388[0x4];               // 0x388
};
DECOMP_SIZE_ASSERT(SwampRenderer, 0x38C);

template <class T>
void RELEASE(T*& obj) // NAME: proto-pdb:default.pdb
{
    if (obj) {
        obj->Release();
        obj = 0;
    }
}

// FUNCTION: USA 0x000CA37A
// PROTOTYPE: PREVIEW 0x004B55C3 identical
// NAME: proto-pdb:default.pdb
template void RELEASE<D3DDevice>(D3DDevice*&);

// FUNCTION: USA 0x000CA38E
// PROTOTYPE: PREVIEW 0x004B55D7 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); held out: the inline body of Direct3D::Release is unknown
struct Direct3D { // NAME: proto-pdb:default.pdb
    static unsigned long __stdcall Release() { return 1; } // placeholder body: unknown
};
template void RELEASE<Direct3D>(Direct3D*&);
#endif
#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0132); RELEASE<Direct3D> is held out above, InitDirect3D calls it by name
template <> void RELEASE<Direct3D>(Direct3D*&);
#endif

// FUNCTION: USA 0x000CA39B
// PROTOTYPE: PREVIEW 0x004B55E4 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1); wall: D3DDevice_Clear named only undecorated (C0192, C0138)
void SwampRenderer::ClearScreen(unsigned int color, bool clearColor, bool clearZ, bool clearStencil) const
{
    D3DDevice_Clear(0, 0, (clearColor ? 0xF0 : 0) | (clearZ ? 1 : 0) | (clearStencil ? 2 : 0), color, 1.0f, 0);
}
#endif

// FUNCTION: USA 0x000CA3D9
// PROTOTYPE: PREVIEW 0x004B5622 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::RenderToScreen()
{
    D3DDevice_SetRenderTarget(_ColorBuffer, _DepthBuffer);
}

template <> void RELEASE<D3DSurface>(D3DSurface*& p);

template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

// FUNCTION: USA 0x000CA3E5
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetEmptyTexture(unsigned long stage)
{
    if (_CurrentTexture[stage].IsValid()) {
        _CurrentTexture[stage] = ResourceHandle<TextureResource>();
        D3DDevice_SetTexture(stage, 0);
        ++_textureChangeCount;
    }
}

// FUNCTION: USA 0x000CA416
// PROTOTYPE: PREVIEW 0x004B565F identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetCullMode(unsigned long value)
{
    _cullMode = value;
    if (_cullMode != 0 && _cullingReversed)
        _cullMode ^= 1;
    D3DDevice_SetRenderState_CullMode(_cullMode);
}

// FUNCTION: USA 0x000CA438
// PROTOTYPE: PREVIEW 0x004B5681 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::ReverseCullMode()
{
    _cullingReversed = !_cullingReversed;
    if (_cullMode != 0) {
        _cullMode ^= 1;
        D3DDevice_SetRenderState_CullMode(_cullMode);
    }
}

// FUNCTION: USA 0x000CA456
// PROTOTYPE: PREVIEW 0x004B569F identical
// NAME: proto-pdb:default.pdb
bool SwampRenderer::EnableDepthBuffering(bool enable)
{
    // !FAKE: c2 register allocation; local; removal changes the bytes (the PDB lists no such local)
    bool v1 = _depthBufferingEnabled;
    if (enable != v1) {
        if (enable) {
            if (_useWBuffering)
                D3DDevice_SetRenderState_ZEnable(2); // src proto-pdb:default.pdb D3DZB_USEW
            else
                D3DDevice_SetRenderState_ZEnable(1); // src proto-pdb:default.pdb D3DZB_TRUE
        } else {
            D3DDevice_SetRenderState_ZEnable(0); // src proto-pdb:default.pdb D3DZB_FALSE
        }
        _depthBufferingEnabled = enable;
    }
    return v1;
}

// FUNCTION: USA 0x000CA490
// PROTOTYPE: PREVIEW 0x004B56D9 identical
// NAME: proto-pdb:default.pdb
bool SwampRenderer::UseWBuffering(bool useW)
{
    // !FAKE: c2 register allocation; local; removal changes the bytes (the PDB lists no such local)
    bool v1 = _useWBuffering;
    if (useW != v1) {
        if (_depthBufferingEnabled) {
            if (useW)
                D3DDevice_SetRenderState_ZEnable(2); // src proto-pdb:default.pdb D3DZB_USEW
            else
                D3DDevice_SetRenderState_ZEnable(1); // src proto-pdb:default.pdb D3DZB_TRUE
        }
        _useWBuffering = useW;
    }
    return v1;
}

// FUNCTION: USA 0x000CA58D
// PROTOTYPE: PREVIEW 0x004B57D6 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::GetProjectionTransform(Matrix& m) const
{
    D3DDevice_GetTransform(D3DTS_PROJECTION, (_D3DMATRIX*)&m);
}

// FUNCTION: USA 0x000CA59B
// PROTOTYPE: PREVIEW 0x004B57E4 identical
// NAME: proto-pdb:default.pdb
long SwampRenderer::SetPixelShader(const ResourceHandle<PixelShaderResource>& handle)
{
    if (handle._Resource->_shaderCode != _CurrentPixelShader) {
        _CurrentPixelShader = handle._Resource->_shaderCode;
        D3DDevice_SetPixelShaderProgram(_CurrentPixelShader);
    }
    return 0;
}

// FUNCTION: USA 0x000CA5B7
// PROTOTYPE: PREVIEW 0x004B5800 identical
// NAME: proto-pdb:default.pdb
unsigned int SwampRenderer::EndRasterTest() const
{
    unsigned int v1 = _nextRasterHandle;
    _nextRasterHandle = (_nextRasterHandle + 1) & 0x1FF;
    D3DDevice_EndVisibilityTest(v1);
    return v1;
}

// FUNCTION: USA 0x000CA5D0
// PROTOTYPE: PREVIEW 0x004B5819 identical
// NAME: proto-pdb:default.pdb
int SwampRenderer::GetRasterTestResults(unsigned int handle) const
{
    unsigned int count;
    if (D3DDevice_GetVisibilityTestResult(handle, &count, 0) == (long)0x88760828)
        return -1;
    return count;
}

// FUNCTION: USA 0x000CA5F4
// PROTOTYPE: PREVIEW 0x004B583D identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetGammaRamp(const _D3DGAMMARAMP& ramp, bool immediate, bool temp)
{
    unsigned long v1 = 0;
    if (immediate)
        v1 = 2;
    D3DDevice_SetGammaRamp(v1, &ramp);
    if (!temp)
        _gammaRamp = ramp;
}

// FUNCTION: USA 0x000CA627
// PROTOTYPE: PREVIEW 0x004B5870 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::RestoreGammaRamp(bool immediate)
{
    unsigned long v1 = 0;
    if (immediate)
        v1 = 2;
    D3DDevice_SetGammaRamp(v1, &_gammaRamp);
}

// FUNCTION: USA 0x000CA642
// PROTOTYPE: PREVIEW 0x004B588B identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetBasicPixelShader()
{
    SetPixelShader(_basicPixelShader);
}

// FUNCTION: USA 0x000CA64F
// PROTOTYPE: PREVIEW 0x004B5898 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::RenderToTexture(const ResourceHandle<TextureResource>& color, D3DTexture* depth, unsigned int cubeFace)
{
    D3DSurface* colorsurf;
    D3DSurface* depthsurf = 0;
    D3DTexture* p = color().GetTexture();
    if (cubeFace == D3DCUBEMAP_FACE_FORCE_DWORD)
        colorsurf = D3DTexture_GetSurfaceLevel2(p, 0);
    else
        colorsurf = D3DCubeTexture_GetCubeMapSurface2((D3DCubeTexture*)p, (_D3DCUBEMAP_FACES)cubeFace, 0);
    if (depth)
        depthsurf = D3DTexture_GetSurfaceLevel2(depth, 0);
    D3DDevice_SetRenderTarget(colorsurf, depthsurf);
    RELEASE(colorsurf);
    RELEASE(depthsurf);
}

// FUNCTION: USA 0x000CA6B6
// PROTOTYPE: PREVIEW 0x004B58FF identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetTexture(unsigned long stage, const ResourceHandle<TextureResource>& tex)
{
    if (tex.IsValid()) {
        if (!(_CurrentTexture[stage] == tex)) {
            _CurrentTexture[stage] = tex;
            D3DDevice_SetTexture(stage, tex().GetTexture());
            ++_textureSetCount;
            ++_textureChangeCount;
        }
    } else {
        SetEmptyTexture(stage);
    }
}

// FUNCTION: USA 0x000CA6FE
// PROTOTYPE: PREVIEW 0x004B5947 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetDefaultTexture(unsigned long stage)
{
    SetTexture(stage, g_swamp->GetTextureManager().GetDefaultTexture());
}

// FUNCTION: USA 0x000CA730
// PROTOTYPE: PREVIEW 0x004B5979 identical
// NAME: proto-pdb:default.pdb
long SwampRenderer::SetVertexShader(const ResourceHandle<VertexShaderResource>& handle,
                                    const _D3DVERTEXATTRIBUTEFORMAT* declaration)
{
    if (!(handle == _CurrentVertexShader)) {
        D3DDevice_LoadVertexShaderProgram(handle().GetShaderCode(), 0);
        _CurrentVertexShader = handle;
    }
    if (declaration != _CurrentVertexDeclaration) {
        D3DDevice_SelectVertexShaderDirect(declaration, 0);
        _CurrentVertexDeclaration = declaration;
    }
    return 0;
}

// FUNCTION: USA 0x000CA770
// PROTOTYPE: PREVIEW 0x004B59B9 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::ClearStateCache()
{
    _CurrentVB = 0;
    _CurrentIB = 0;
    _CurrentVertexShader = ResourceHandle<VertexShaderResource>();
    _CurrentVertexDeclaration = 0;
    _CurrentPixelShader = 0;
    for (int i = 0; i < 4; i++)
        SetEmptyTexture(i);
    for (unsigned int i = 0; i < 16; i++)
        D3DDevice_SetStreamSource(i, 0, 0);
}

// FUNCTION: USA 0x000CA7BB
// PROTOTYPE: PREVIEW 0x004B5A04 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetBasicVertexShader(const _D3DVERTEXATTRIBUTEFORMAT* declaration)
{
    SetVertexShader(_basicVertexShader, declaration);
}

// FUNCTION: USA 0x000CA7CE
// NAME: proto-pdb:default.pdb
long SwampRenderer::SetVertexShader(const ResourceHandle<VertexShaderResource>& handle)
{
    unsigned long v1 = handle().GetD3DHandle();
    if (v1) {
        if (!(handle == _CurrentVertexShader)) {
            _CurrentVertexShader = handle;
            _CurrentVertexDeclaration = 0;
            D3DDevice_SetVertexShader(v1);
        }
        return 0;
    }
    return SetVertexShader(handle, handle().GetDeclaration());
}

// FUNCTION: USA 0x000CA88D
// NAME: proto-pdb:default.pdb
void SwampRenderer::EndScene()
{
    ClearStateCache();
    D3DDevice_Swap(0);
}

// FUNCTION: USA 0x000CA9BA
// PROTOTYPE: PREVIEW 0x004B5C03 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.92, bytes match (masked 0); wall: the delinked cut carries a DIR32 reloc on the immediate 0x80000000 (C0155, C0232)
void SwampRenderer::InitDirect3D(HWND__* hwnd, const SwampString& renderswitches)
{
    char buff[0x80];
    RELEASE(_D3DDevice);
    RELEASE(_D3D);
    _D3D = Direct3DCreate8(0);
    Direct3D_SetPushBufferSize(0x100000, 0x8000);
    _D3DPRESENT_PARAMETERS_ pp = { 0 };
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.EnableAutoDepthStencil = 1;
    pp.AutoDepthStencilFormat = D3DFMT_D24S8;
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.FullScreen_RefreshRateInHz = 0;
    pp.FullScreen_PresentationInterval = 0x80000000;
    pp.BackBufferWidth = _screenWidth;
    pp.BackBufferHeight = _screenHeight;
    // !FAKE: c1xx front end; stand-in local for the XDK inline CreateDevice wrapper's HRESULT; no PDB local
    long v0 = Direct3D_CreateDevice(0, D3DDEVTYPE_HAL, hwnd, 0x50, &pp, &_D3DDevice);
    if (v0 < 0)
        D3DXGetErrorStringA(v0, buff, sizeof(buff));
    SetPresentationParameters(renderswitches, false);
    D3DDevice_GetGammaRamp(&_gammaRamp);
}
#endif
