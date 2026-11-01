// UNIT: src/Apps/Vince/scripts/Bayou/Hannah.cpp OBJ: Xbox\OfficialRelease\Hannah.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <string.h>
#pragma intrinsic(memcpy) // src proto-pdb:default.pdb memcpy

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
template <class T, class A = allocator<T> > class deque { // src proto-pdb:default.pdb deque
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class SwampSceneNode;        // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line);

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampScript();                                        // vtable+0x00

    char field_4[0x4C - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();

    char field_4C[0xBC - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class VinceBlowBack : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    VinceBlowBack(SwampSceneNode* node);

    char field_BC[0xF8 - 0xBC];        // 0xBC
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(VinceBlowBack, 0xF8);

class QuickRand { // NAME: proto-pdb:default.pdb
public:
    QuickRand(unsigned __int64 seed);
    unsigned int GetRandom()
    {
        unsigned __int64 tmp = (unsigned __int64)_Mul * 0x7F13AC69 + _Carry;
        _Mul = (unsigned int)tmp;
        _Carry = (unsigned int)(tmp >> 32);
        return _Mul;
    }
    float GetRandomFloat();

    unsigned int _Mul;                 // 0x00
    unsigned int _Carry;               // 0x04
};
DECOMP_SIZE_ASSERT(QuickRand, 0x8);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(VertexShaderResource, 0x30);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
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

    T* _Resource;                      // 0x00
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    std::deque<T> _queue;              // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

// Retail layout = the PDB HannahMain record of size 0x138 (ti 0x142b3)
class HannahMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    HannahMain(SwampSceneNode* node);
    void SetAttackLevel(int level);

    MultiAnimation _multiAnimIdle;     // 0xBC
    MultiAnimation _multiAnimBlowBack; // 0xD0
    ResourceHandle<AnimationResource> _animTaunt;      // 0xE4
    ResourceHandle<AnimationResource> _animDamage;     // 0xE8
    ResourceHandle<AnimationResource> _animAttack[4];  // 0xEC
    int field_FC;                      // 0xFC
    char field_100[0x108 - 0x100];     // 0x100
    SwampShuffleQueue<SwampString> _debrisNames; // 0x108
    char field_11C[0x138 - 0x11C];     // 0x11C
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(HannahMain, 0x138);

// FUNCTION: USA 0x00028C89
// PROTOTYPE: PREVIEW 0x00417ABC identical
// NAME: proto-pdb:default.pdb
float QuickRand::GetRandomFloat()
{
    unsigned int bits = (GetRandom() >> 9) | 0x3F800000;
    return *(float*)&bits - 1.0f;
}

// FUNCTION: USA 0x00028CBE
// PROTOTYPE: PREVIEW 0x00417AF6 identical
// NAME: proto-pdb:default.pdb
void HannahMain::SetAttackLevel(int level)
{
    field_FC = level;
}

// FUNCTION: USA 0x00028D58
// PROTOTYPE: PREVIEW 0x00417B90 identical
// NAME: proto-pdb:default.pdb
template ResourceHandle<TextureResource>::ResourceHandle(const ResourceHandle<TextureResource>& h);

// FUNCTION: USA 0x00028D6C
// PROTOTYPE: PREVIEW 0x00417BA4 identical
// NAME: proto-pdb:default.pdb
QuickRand::QuickRand(unsigned __int64 seed)
{
    _Mul = (unsigned int)seed * 2 + 1;
    _Carry = (unsigned int)(seed >> 32) >> 2;
    GetRandom();
}

struct D3DVertexBuffer; // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_SetStreamSource(unsigned int, D3DVertexBuffer*, unsigned int); // src proto-pdb:default.pdb D3DDevice_SetStreamSource
extern "C" void __fastcall D3DDevice_SetVertexShaderConstant1(int, const void*); // src proto-pdb:default.pdb D3DDevice_SetVertexShaderConstant1
extern "C" void __fastcall D3DDevice_SetVertexShaderConstant4(int, const void*); // src proto-pdb:default.pdb D3DDevice_SetVertexShaderConstant4
extern "C" void __fastcall D3DDevice_SetVertexShaderConstantNotInline(int, const void*, unsigned int); // src proto-pdb:default.pdb D3DDevice_SetVertexShaderConstantNotInline

struct Vector4; // NAME: proto-pdb:default.pdb

extern "C" D3DVertexBuffer* __stdcall D3DDevice_CreateVertexBuffer2(unsigned int length); // src proto-pdb:default.pdb D3DDevice_CreateVertexBuffer2
extern "C" unsigned char* __stdcall D3DVertexBuffer_Lock2(D3DVertexBuffer* vb, unsigned long flags); // src proto-pdb:default.pdb D3DVertexBuffer_Lock2
extern "C" long __stdcall D3DXGetErrorStringA(long hr, char* buffer, unsigned int size); // src proto-pdb:default.pdb D3DXGetErrorStringA

class WindyBits { // NAME: proto-pdb:default.pdb
public:
    struct Floater { // NAME: proto-pdb:default.pdb
        char pos[0xC];                 // 0x00
        unsigned int color;            // 0x0C
    };
};

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    template <class T> D3DVertexBuffer* CreateVertexBuffer(const T* verts, unsigned int num) const;
    long SetStreamSource(D3DVertexBuffer* vb, unsigned int stride);
    void SetVertexShaderConstant(int regnum, const Vector4* data, int count) const;

    char field_0[0x44];                // 0x00
    D3DVertexBuffer* _CurrentVB;       // 0x44
    char field_48[0x38C - 0x48];       // 0x48
};
DECOMP_SIZE_ASSERT(SwampRenderer, 0x38C);

// FUNCTION: USA 0x00029017
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetVertexShaderConstant(int regnum, const Vector4* data, int count) const
{
    if (count == 1)
        D3DDevice_SetVertexShaderConstant1(regnum + 96, data);
    else if (count == 4)
        D3DDevice_SetVertexShaderConstant4(regnum + 96, data);
    else
        D3DDevice_SetVertexShaderConstantNotInline(regnum + 96, data, count * 4);
}

// FUNCTION: USA 0x0002904B
// PROTOTYPE: PREVIEW 0x00417E83 identical
// NAME: proto-pdb:default.pdb
long SwampRenderer::SetStreamSource(D3DVertexBuffer* vb, unsigned int stride)
{
    if (vb != _CurrentVB) {
        _CurrentVB = vb;
        D3DDevice_SetStreamSource(0, vb, stride);
    }
    return 0;
}

// FUNCTION: USA 0x00028CE3
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
template D3DVertexBuffer* SwampRenderer::CreateVertexBuffer<WindyBits::Floater>(const WindyBits::Floater*,
    unsigned int) const;

#ifdef NON_MATCHING
// FUNCTION: USA 0x000292A6
// NAME: proto-pdb:default.pdb
template ResourceHandle<VertexShaderResource>::~ResourceHandle();
#endif

// FUNCTION: USA 0x00029B16
// PROTOTYPE: PREVIEW 0x004188D3 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceBlowBack::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\Hannah.cpp", 31) VinceBlowBack(node);
}

// FUNCTION: USA 0x0002A0A1
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1HannahMain@@UAE@XZ because this unit defines
// the ctor (not matched); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_HannahMain_dtor(HannahMain* p)
{
    p->HannahMain::~HannahMain();
}

// FUNCTION: USA 0x0002A0FC
// PROTOTYPE: PREVIEW 0x00418EB9 identical
// NAME: proto-pdb:default.pdb
SwampScript* HannahMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\hurricanehanna\\Hannah.h", 48) HannahMain(node);
}
