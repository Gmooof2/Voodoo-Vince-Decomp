// UNIT: src/Libs/Swamp/Xbox/Official_Release/LoadScreen.cpp OBJ: Xbox\Official_Release\LoadScreen.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

struct D3DTexture; // NAME: proto-pdb:default.pdb
struct D3DDevice; // NAME: proto-pdb:default.pdb

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    D3DTexture* GetTexture() const;

    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

class TextureResource;

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle&);
    T& operator()() { return *_Resource; }
    bool IsValid() const { return _Resource != 0; }
    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

// visible body: cl 13.10 /O1 elides a null temporary's dtor after operator= only with it (C0136)
template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

struct D3DResource { // NAME: proto-pdb:default.pdb
    unsigned long Release();

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

enum _D3DFORMAT { // NAME: proto-pdb:default.pdb
    D3DFMT_LIN_X8R8G8B8 = 0x1E,
};

struct _D3DSURFACE_DESC { // NAME: proto-pdb:default.pdb
    _D3DFORMAT Format;                 // 0x00
    unsigned long Type;                // 0x04 enum _D3DRESOURCETYPE
    unsigned long Usage;               // 0x08
    unsigned int Size;                 // 0x0C
    unsigned long MultiSampleType;     // 0x10
    unsigned int Width;                // 0x14
    unsigned int Height;               // 0x18
};
DECOMP_SIZE_ASSERT(_D3DSURFACE_DESC, 0x1C);

extern "C" D3DSurface* __stdcall D3DDevice_GetPersistedSurface2(); // NAME: proto-pdb:default.pdb
extern "C" D3DSurface* __stdcall D3DDevice_GetDepthStencilSurface2(); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DDevice_SetRenderTarget(D3DSurface* pRenderTarget, D3DSurface* pNewZStencil); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DSurface_GetDesc(D3DSurface* pSurface, _D3DSURFACE_DESC* pDesc); // NAME: proto-pdb:default.pdb
extern "C" D3DSurface* __stdcall D3DTexture_GetSurfaceLevel2(D3DTexture* pTexture, unsigned int Level); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall D3DResource_Release(D3DResource* pResource); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DXCheckTextureRequirements(D3DDevice* pDevice, unsigned int* pWidth, unsigned int* pHeight, unsigned int* pNumMipLevels, unsigned long Usage, _D3DFORMAT* pFormat, unsigned long Pool); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall D3DXLoadSurfaceFromSurface(D3DSurface* pDestSurface, const void* pDestPalette, const void* pDestRect, D3DSurface* pSrcSurface, const void* pSrcPalette, const void* pSrcRect, unsigned long Filter, unsigned long ColorKey); // NAME: proto-pdb:default.pdb

inline unsigned long D3DResource::Release() { return D3DResource_Release(this); }

struct D3DTexture { // NAME: proto-pdb:default.pdb
    long GetSurfaceLevel(unsigned int Level, D3DSurface** ppSurfaceLevel) { *ppSurfaceLevel = D3DTexture_GetSurfaceLevel2(this, Level); return 0; }
};

struct D3DDevice { // NAME: proto-pdb:default.pdb
    static long GetDepthStencilSurface(D3DSurface** ppZStencilSurface) { *ppZStencilSurface = D3DDevice_GetDepthStencilSurface2(); return 0; }
    static long GetPersistedSurface(D3DSurface** ppSurface) { *ppSurface = D3DDevice_GetPersistedSurface2(); return 0; }
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
}

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> CreateTexture(const void*, int, int, _D3DFORMAT, bool);
};

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    D3DDevice* GetDevice() const { return _D3DDevice; }
    bool EnableDepthBuffering(bool);
    void RenderToScreen();

    char field_0[0x20];                // 0x00
    D3DDevice* _D3DDevice;             // 0x20
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    void IgnoreInput(bool ignore) { _ignoreInput = ignore; }
    float GetSessionTime() const { return _sessionTimePublic; }

    char field_0[0xC];                 // 0x00
    float _sessionTimePublic;          // 0x0C
    char field_10[0x38 - 0x10];        // 0x10
    bool _ignoreInput;                 // 0x38
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    void BeginScene();
    void EndScene();
    SwampInput& GetInput() const { return *_input; }
    SwampRenderer& GetRenderer() const { return *_renderer; }
    TextureManager& GetTextureManager() const { return *_textureManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x38 - 0xC];          // 0x0C
    TextureManager* _textureManager;   // 0x38
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class LoadScreen { // NAME: proto-pdb:default.pdb
public:
    LoadScreen();
    virtual ~LoadScreen();                                         // vtable+0x00
    virtual void RenderGameScene() = 0;                            // vtable+0x04
    virtual void RenderLoadScene() = 0;                            // vtable+0x08
    virtual void Update();                                         // vtable+0x10
    virtual void Update(bool, float);                              // vtable+0x0C
    virtual void EndOfTransition(bool);                            // vtable+0x14

    void Transition(bool, float);
    void DisplayLoadScreen();
    void RenderPersisted() const;
    bool HasPersistedImage() const { return _persistedScreen.IsValid(); }

    float _startDisplayTime;           // 0x04
    float _minimumDisplayTime;         // 0x08
    float _loadTime;                   // 0x0C
    float _loadRate;                   // 0x10
    D3DSurface _loadScreen;            // 0x14
    ResourceHandle<TextureResource> _persistedScreen; // 0x2C
private:
    void RenderScreen(float, bool) const;
};
DECOMP_SIZE_ASSERT(LoadScreen, 0x30);

// FUNCTION: USA 0x000DC10A
// PROTOTYPE: PREVIEW 0x004C7641 identical
// NAME: proto-pdb:default.pdb
LoadScreen::~LoadScreen()
{
}

// FUNCTION: USA 0x000DC11B
// PROTOTYPE: PREVIEW 0x004C7653 identical
// NAME: proto-pdb:default.pdb
void LoadScreen::DisplayLoadScreen()
{
    double time = BeepLib::GetTime();
    if (_loadTime > 0.0f && _loadTime + _loadRate > time)
        return;
    _loadTime = time;
    Update();
    g_swamp->BeginScene();
    RenderLoadScene();
    g_swamp->EndScene();
}

// FUNCTION: USA 0x000DC16E
// PROTOTYPE: PREVIEW 0x004C76A6 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 97.64, bytes mismatch (retail reloads g_swamp after storing txtmgr; ours reuses it, -5 B) (4.3 T9.c3);
// 4.4 T4.c6: vtable slots now pinned, codegen wall only; tried dev after GetPersistedSurface: 91.92, reverted
LoadScreen::LoadScreen()
    : _startDisplayTime(0.0f), _minimumDisplayTime(0.0f), _loadTime(0.0f), _loadRate(0.33f)
{
    TextureManager& txtmgr = g_swamp->GetTextureManager();
    D3DDevice* dev = g_swamp->GetRenderer().GetDevice();
    D3DSurface* surf;
    D3DDevice::GetPersistedSurface(&surf);
    if (surf) {
        _D3DSURFACE_DESC desc;
        D3DSurface_GetDesc(surf, &desc);
        unsigned int width = desc.Width;
        unsigned int height = desc.Height;
        unsigned int mips = 1;
        _D3DFORMAT format = desc.Format;
        D3DXCheckTextureRequirements(dev, &width, &height, &mips, 0, &format, 1);
        _persistedScreen = txtmgr.CreateTexture(0, width, height, D3DFMT_LIN_X8R8G8B8, false);
        // !FAKE: c1xx front end; stand-in local; the PDB records no local for the level surface
        D3DSurface* p;
        _persistedScreen().GetTexture()->GetSurfaceLevel(0, &p);
        D3DXLoadSurfaceFromSurface(p, 0, 0, surf, 0, 0, 1, 0);
        p->Release();
    }
    // !FAKE: c1xx front end; stand-in local; the PDB records no local for the depth surface
    D3DSurface* tmp;
    D3DDevice::GetDepthStencilSurface(&tmp);
    _loadScreen = *tmp;
    tmp->Release();
    _loadScreen.Format &= ~0xFF00;
    _loadScreen.Format |= D3DFMT_LIN_X8R8G8B8 << 8;
}
#endif

// FUNCTION: USA 0x000DC46C
// PROTOTYPE: PREVIEW 0x004C79A4 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 79.41, bytes mismatch (retail: renderer in memory, ebx = 0, g_swamp reloaded, loop head not rotated; ours keeps renderer in ebx and rotates the loop) (4.3 T9.c3)
void LoadScreen::Transition(bool bFromGame, float time)
{
    SwampRenderer& renderer = g_swamp->GetRenderer();
    // !FAKE: c2 register allocation; stand-in local; the PDB records no local for the input (edi in retail)
    SwampInput* p = &g_swamp->GetInput();
    float startTime = p->GetSessionTime();
    p->IgnoreInput(true);
    // !FAKE: c2 register allocation; stand-in local; the PDB records no local for the elapsed time (x87 in retail)
    for (;;) {
        float v1 = p->GetSessionTime() - startTime;
        if (v1 > time)
            break;
        float transition = 1.0f;
        if (time != 0.0f)
            transition = v1 / time;
        Update();
        Update(bFromGame, transition);
        g_swamp->BeginScene();
        RenderGameScene();
        bool wasDepth = renderer.EnableDepthBuffering(false);
        D3DDevice_SetRenderTarget(&_loadScreen, 0);
        RenderLoadScene();
        renderer.RenderToScreen();
        RenderScreen(bFromGame ? transition : 1.0f - transition, false);
        renderer.EnableDepthBuffering(wasDepth);
        g_swamp->EndScene();
    }
    _startDisplayTime = _loadTime = BeepLib::GetTime();
    if (HasPersistedImage())
        _persistedScreen = ResourceHandle<TextureResource>();
    p->IgnoreInput(false);
    EndOfTransition(bFromGame);
}
#endif

// FUNCTION: USA 0x000DC584
// PROTOTYPE: PREVIEW 0x004C7ABC identical
// NAME: proto-pdb:default.pdb
void LoadScreen::RenderPersisted() const
{
    if (HasPersistedImage())
        RenderScreen(1.0f, true);
}
