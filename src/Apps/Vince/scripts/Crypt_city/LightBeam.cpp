// UNIT: src/Apps/Vince/scripts/Crypt_city/LightBeam.cpp OBJ: Xbox\OfficialRelease\LightBeam.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <stdlib.h>
#include <string.h>
#pragma intrinsic(memcpy) // src proto-pdb:default.pdb memcpy

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                          // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x3AC];               // 0x00
    Vector3 _fireflyLightLanternColor; // 0x3AC
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { DontDraw = 0x2 }; // NAME: proto-pdb:default.pdb
    SwampSceneNode(int);
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

    SwampScript* GetScript() const;
    void SetOption(unsigned int option, bool on, bool recurse);
    void SetScript(const SwampString&);

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// src proto-pdb:default.pdb Critter base GatorNode (SwampSceneNode at 0x0), not modelled
class Critter : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum CollisionType { // NAME: proto-pdb:default.pdb
        HorizontalCollision = 0,
        VerticalCollision = 1
    };
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04
    virtual void Activate();                                       // vtable+0x08
    virtual void Deactivate();                                     // vtable+0x0C
    virtual void FrameTick();                                      // vtable+0x10
    virtual void TimerEvent(unsigned int);                         // vtable+0x14
    virtual void AnimationFinished();                              // vtable+0x18
    virtual void SecondaryAnimationFinished();                     // vtable+0x1C
    virtual void AnimationInterrupted();                           // vtable+0x20
    virtual void SecondaryAnimationInterrupted();                  // vtable+0x24
    virtual void AnimationTrigger(const SwampString&, float, float, unsigned int); // vtable+0x28
    virtual bool LosingControl(const SwampString&);                // vtable+0x2C
    virtual void GainingControl(SwampScript*);                     // vtable+0x30
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    virtual void EnteringRegion(SwampSceneNode*);                  // vtable+0x38
    virtual void LeavingRegion(SwampSceneNode*);                   // vtable+0x3C
    virtual void SoundFinished(const SwampString&);                // vtable+0x40
    virtual void SoundStreamerFinished(const SwampString&);        // vtable+0x44
    virtual void MovieFinished(const SwampString&);                // vtable+0x48
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x50
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual SwampString Save();                                    // vtable+0x58
    virtual void SetNode(SwampSceneNode*);                         // vtable+0x5C
    virtual void InvalidateScript();                               // vtable+0x60

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class LightBeamMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    LightBeamMain(SwampSceneNode* node);
protected:
    virtual void FrameTick();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(LightBeamMain, 0x4C);

// FUNCTION: USA 0x0006ACE1
// PROTOTYPE: PREVIEW 0x0045709F identical
// NAME: proto-pdb:default.pdb
SwampScript* LightBeamMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\gwistlethorpe\\LightBeam.h", 38) LightBeamMain(node);
}

struct D3DVertexBuffer; // NAME: proto-pdb:default.pdb

extern "C" D3DVertexBuffer* __stdcall D3DDevice_CreateVertexBuffer2(unsigned int length); // src proto-pdb:default.pdb D3DDevice_CreateVertexBuffer2
extern "C" unsigned char* __stdcall D3DVertexBuffer_Lock2(D3DVertexBuffer* vb, unsigned long flags); // src proto-pdb:default.pdb D3DVertexBuffer_Lock2
extern "C" long __stdcall D3DXGetErrorStringA(long hr, char* buffer, unsigned int size); // src proto-pdb:default.pdb D3DXGetErrorStringA

struct SpotlightGlowVertex { // NAME: proto-pdb:default.pdb
    char loc[0xC];                     // 0x00
    char normal[0xC];                  // 0x0C
    float alpha;                       // 0x18
};
DECOMP_SIZE_ASSERT(SpotlightGlowVertex, 0x1C);

class TextureResource; // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle; // NAME: proto-pdb:default.pdb

enum _D3DRENDERSTATETYPE { // NAME: proto-pdb:default.pdb
    D3DRS_ZFUNC = 0x39,
    D3DRS_STENCILZFAIL = 0x44,
    D3DRS_STENCILPASS = 0x45,
    D3DRS_STENCILFUNC = 0x46,
    D3DRS_STENCILREF = 0x47,
    D3DRS_STENCILFAIL = 0x91
};

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    template <class T> D3DVertexBuffer* CreateVertexBuffer(const T* verts, unsigned int num) const;
    void SetTexture(unsigned long stage, const ResourceHandle<TextureResource>& texture);
    void SetEmptyTexture(unsigned long stage);
    long SetStreamSource(D3DVertexBuffer* vb, unsigned int stride);
    void SetRenderState(_D3DRENDERSTATETYPE state, unsigned long value);
    void ReverseCullMode();
};

// FUNCTION: USA 0x0006AD05
// PROTOTYPE: PREVIEW 0x004570C3 identical
// NAME: proto-pdb:default.pdb
template <class T> D3DVertexBuffer* SwampRenderer::CreateVertexBuffer(const T* verts, unsigned int num) const
{
    unsigned int size = num * sizeof(T);
    D3DVertexBuffer* vb = D3DDevice_CreateVertexBuffer2(size);
    long hr = vb ? 0 : 0x8007000E;
    if (hr < 0) {
        char buff[0x80];
        D3DXGetErrorStringA(hr, buff, sizeof(buff));
    }
    unsigned char* p = D3DVertexBuffer_Lock2(vb, 0x30);
    memcpy(p, verts, size);
    return vb;
}
template D3DVertexBuffer* SwampRenderer::CreateVertexBuffer<SpotlightGlowVertex>(const SpotlightGlowVertex*,
    unsigned int) const;

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    char field_24[0xC];                // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    // FUNCTION: USA 0x0006AD64
    // PROTOTYPE: PREVIEW 0x00457122 identical
    // NAME: proto-pdb:default.pdb
    ResourceHandle<TextureResource> GetDefaultTexture() const { return _defaultTexture; }
    ResourceHandle<TextureResource> GetVolumeNoise() const { return _volumeNoise; }

    char field_0[0x58];                // 0x00
    ResourceHandle<TextureResource> _defaultTexture; // 0x58
    char field_5C[0x68 - 0x5C];        // 0x5C
    ResourceHandle<TextureResource> _volumeNoise; // 0x68
    char field_6C[0x84 - 0x6C];        // 0x6C
};
DECOMP_SIZE_ASSERT(TextureManager, 0x84);

// !FAKE: c1xx front end; scaffold address-of; retail emits the inline GetDefaultTexture COMDAT in this unit (no
// matched function odr-uses it); taking its address emits it instead (matched_set [[emitter]] row)
ResourceHandle<TextureResource> (TextureManager::*pin_emit_GetDefaultTexture())() const
{
    return &TextureManager::GetDefaultTexture;
}

template <class T> void RELEASE(T*& p); // NAME: proto-pdb:default.pdb
void operator delete[](void* p);

struct SpotLightBeam { // NAME: proto-pdb:default.pdb
    SpotLightBeam(float, float, float);
    ~SpotLightBeam();

    D3DVertexBuffer* verts;            // 0x00
    float cosAngle;                    // 0x04
    float projectedEndpoint;           // 0x08
    unsigned short* indicesBeam;       // 0x0C
    unsigned int indexCountBeam;       // 0x10
    unsigned short* indicesCone;       // 0x14
    unsigned short* indicesEndcaps[2]; // 0x18
    unsigned int indexCountCone;       // 0x20
    unsigned int indexCountEndcap;     // 0x24
};
DECOMP_SIZE_ASSERT(SpotLightBeam, 0x28);

// FUNCTION: USA 0x0006B01F
// NAME: proto-pdb:default.pdb
SpotLightBeam::~SpotLightBeam()
{
    RELEASE(verts);
    delete[] indicesBeam;
    delete[] indicesCone;
    for (int i = 0; i < 2; i++)
        delete[] indicesEndcaps[i];
}

// FUNCTION: USA 0x0006B322
// PROTOTYPE: PREVIEW 0x004576E5 identical
// NAME: proto-pdb:default.pdb
void LightBeamMain::FrameTick()
{
    int lightLevel;
    // !FAKE: c1xx front end; stand-in local; ends the string temporary before GetScript (BumperTargetMain form)
    Critter* tmp = pManager->Find("lantern");
    tmp->GetScript()->Message(0, 0x14, &lightLevel); // src proto-pdb:default.pdb MESSAGE_LIGHTLEVEL
    _node->SetOption(SwampSceneNode::DontDraw, lightLevel == 0, true);
}

class Swamp { // NAME: proto-pdb:default.pdb
public:
    TextureManager& GetTextureManager() const { return *_textureManager; }
    SwampRenderer& GetRenderer() const { return *_renderer; }

    char field_0[0x8];                 // 0x00
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x38 - 0xC];          // 0x0C
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

enum _D3DPRIMITIVETYPE { D3DPT_TRIANGLESTRIP = 6, D3DPT_TRIANGLEFAN = 7 }; // NAME: proto-pdb:default.pdb
extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_Simple
extern "C" void __stdcall D3DDevice_DrawIndexedVertices(_D3DPRIMITIVETYPE primitiveType, unsigned int vertexCount, const unsigned short* indexData); // src proto-pdb:default.pdb D3DDevice_DrawIndexedVertices
extern "C" void __stdcall D3DDevice_SetRenderState_StencilEnable(unsigned long value); // src proto-pdb:default.pdb D3DDevice_SetRenderState_StencilEnable
extern "C" void __stdcall D3DDevice_SetRenderState_StencilFail(unsigned long value); // src proto-pdb:default.pdb D3DDevice_SetRenderState_StencilFail

class DrawLevel { // NAME: proto-pdb:default.pdb
public:
    static bool AreWeMirroring() { return _mirroringHappening; }

protected:
    static bool _mirroringHappening;
};

// FUNCTION: USA 0x0006B08D
// NAME: proto-pdb:default.pdb
// ??_GLightBeam: compiler-generated with the vtable LightBeam's ctor emits (5.3 T6.c9)
class LightBeam : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    LightBeam();
    virtual ~LightBeam();
    virtual void Render(const Matrix&) const;

protected:
    void RenderBeam(bool bInside) const;
    void RenderLighting(bool bInside) const;
    static void SelectShaders(bool);

    static unsigned int _referenceCount;
    static SpotLightBeam* _beam;

    Vector3 _color;                    // 0xA4
};
DECOMP_SIZE_ASSERT(LightBeam, 0xB0);

// FUNCTION: USA 0x0006B053
// NAME: proto-pdb:default.pdb
LightBeam::~LightBeam()
{
    if (--_referenceCount == 0) {
        delete _beam;
        _beam = 0;
    }
}

// FUNCTION: USA 0x0006B0A9
// NAME: proto-pdb:default.pdb
LightBeam::LightBeam() : SwampSceneNode(0xD)
{
    _color = gSettings._fireflyLightLanternColor;
    // !FAKE: inliner; hand form of the inline Vector3::operator/=(float) with a pointer local for its this; /O1 kept
    // it out of line (match_one 87.53; hand form without p 99.74; with p 100.0)
    float tmp = 1.0f / __max(__max(_color.x, _color.y), _color.z);
    Vector3* p = &_color;
    p->x *= tmp;
    p->y *= tmp;
    p->z *= tmp;
    if (!_beam)
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        _beam = new("Gwistlethorpe\\LightBeam.cpp", 0xC7) SpotLightBeam(0.25f, 40.0f, 0.95f);
    _referenceCount++;
    SetOption(0x10, true, false);
    SetScript("LightBeamMain");
}

// FUNCTION: USA 0x0006B384
// PROTOTYPE: PREVIEW 0x00457747 identical
// NAME: proto-pdb:default.pdb
void LightBeam::RenderBeam(bool bInside) const
{
    // !FAKE: c1xx front end; stand-in local; the PDB records no local for the renderer (edi in retail)
    SwampRenderer* p = &g_swamp->GetRenderer();
    SelectShaders(true);
    p->SetTexture(0, g_swamp->GetTextureManager().GetVolumeNoise());
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 1);
    D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLESTRIP, _beam->indexCountBeam, _beam->indicesBeam);
    p->SetStreamSource(0, 0);
    p->SetEmptyTexture(0);
    D3DDevice_SetRenderState_Simple(0x40304, 0);
}

// FUNCTION: USA 0x0006B425
// PROTOTYPE: PREVIEW 0x004577E8 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.90, bytes mismatch (retail reloads g_swamp before GetTextureManager; cl CSEs it, 30+ forms, flags too)
void LightBeam::RenderLighting(bool bInside) const
{
    if (DrawLevel::AreWeMirroring())
        return;
    SelectShaders(false);
    // !FAKE: c1xx front end; stand-in local; the PDB records no local for the renderer (esi in retail)
    SwampRenderer* p = &g_swamp->GetRenderer();
    p->SetTexture(0, g_swamp->GetTextureManager().GetDefaultTexture());
    if (bInside) {
        D3DDevice_SetRenderState_Simple(0x40354, 0x204);
        D3DDevice_SetRenderState_Simple(0x4034C, 0x40000000);
    } else {
        D3DDevice_SetRenderState_Simple(0x40358, 0);
        D3DDevice_SetRenderState_StencilEnable(1);
        D3DDevice_SetRenderState_Simple(0x40368, 1);
        D3DDevice_SetRenderState_Simple(0x40374, 0x1E01);
        p->ReverseCullMode();
        D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLESTRIP, _beam->indexCountCone, _beam->indicesCone);
        for (unsigned int i = 0; i < 2; i++)
            D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLEFAN, _beam->indexCountEndcap, _beam->indicesEndcaps[i]);
        p->ReverseCullMode();
        D3DDevice_SetRenderState_Simple(0x40364, 0x202);
        D3DDevice_SetRenderState_Simple(0x40378, 0);
        D3DDevice_SetRenderState_StencilFail(0);
        D3DDevice_SetRenderState_Simple(0x40374, 0);
        D3DDevice_SetRenderState_Simple(0x40358, 0x1010101);
        D3DDevice_SetRenderState_Simple(0x4034C, 0x18000000);
    }
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    D3DDevice_SetRenderState_Simple(0x40344, 0x8003);
    D3DDevice_SetRenderState_Simple(0x40348, 1);
    D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLESTRIP, _beam->indexCountCone, _beam->indicesCone);
    for (unsigned int i = 0; i < 2; i++)
        D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLEFAN, _beam->indexCountEndcap, _beam->indicesEndcaps[i]);
    p->SetEmptyTexture(0);
    D3DDevice_SetRenderState_Simple(0x40304, 0);
    D3DDevice_SetRenderState_StencilEnable(0);
    p->SetRenderState(D3DRS_STENCILREF, 0);
    p->SetRenderState(D3DRS_STENCILFUNC, 0x207);
    p->SetRenderState(D3DRS_STENCILPASS, 0x1E00);
    p->SetRenderState(D3DRS_STENCILFAIL, 0x1E00);
    p->SetRenderState(D3DRS_STENCILZFAIL, 0x1E00);
    p->SetRenderState(D3DRS_ZFUNC, 0x203);
}
#endif
