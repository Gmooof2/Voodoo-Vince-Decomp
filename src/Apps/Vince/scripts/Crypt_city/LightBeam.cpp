// UNIT: src/Apps/Vince/scripts/Crypt_city/LightBeam.cpp OBJ: Xbox\OfficialRelease\LightBeam.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <string.h>
#pragma intrinsic(memcpy) // src proto-pdb:default.pdb memcpy

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

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
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(LightBeamMain, 0x4C);

// FUNCTION: USA 0x0006ACE1
// PROTOTYPE: PREVIEW 0x0045709F identical
// NAME: proto-pdb:default.pdb
SwampScript* LightBeamMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
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

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    template <class T> D3DVertexBuffer* CreateVertexBuffer(const T* verts, unsigned int num) const;
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

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    // FUNCTION: USA 0x0006AD64
    // PROTOTYPE: PREVIEW 0x00457122 identical
    // NAME: proto-pdb:default.pdb
    ResourceHandle<TextureResource> GetDefaultTexture() const { return _defaultTexture; }

    char field_0[0x58];                // 0x00
    ResourceHandle<TextureResource> _defaultTexture; // 0x58
    char field_5C[0x28];               // 0x5C
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
