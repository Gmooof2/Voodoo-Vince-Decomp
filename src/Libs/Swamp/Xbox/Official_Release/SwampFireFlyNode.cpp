// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampFireFlyNode.cpp OBJ: Xbox\Official_Release\SwampFireFlyNode.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <xmmintrin.h>

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    void Set(unsigned int argb);
    unsigned int GetARGB() const;

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct AABox { // NAME: proto-pdb:default.pdb
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct Matrix; // NAME: proto-pdb:default.pdb

class FireFlyNode;

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { RenderRealTime = 0x1000 }; // NAME: proto-pdb:default.pdb
    const Vector3& SetLocation(const Vector3& location);
    const Quaternion& SetRotation(const Quaternion& rotation);
    bool GetOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    char field_4[0x24 - 0x4];          // 0x04
    SwampSceneNode* _Parent;           // 0x24
    char field_28[0x88 - 0x28];        // 0x28
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

struct FFlyParticle { // NAME: proto-pdb:default.pdb
    enum { ALIVE = 0x1, SPAWNING = 0x2 };

    unsigned int _flags;               // 0x00
    float _life;                       // 0x04
    Vector3 _position;                 // 0x08
    Vector3 _lastPosition;             // 0x14
    Vector3 _acceleration;             // 0x20
    unsigned int _color;               // 0x2C
    unsigned int _texId;               // 0x30
    float _size;                       // 0x34
    unsigned int _pad[2];              // 0x38
};
DECOMP_SIZE_ASSERT(FFlyParticle, 0x40);

struct FFlyData { // NAME: proto-pdb:default.pdb
    enum { // NAME: proto-pdb:default.pdb
        EFFECT_NONE = 0x0,
        EFFECT_INTERP_COLOR = 0x1,
        EFFECT_INTERP_ALPHA = 0x2,
        EFFECT_INTERP_TEXID = 0x4,
        EFFECT_Z_WRITES = 0x8,
        EFFECT_ADDITIVE = 0x10,
        EFFECT_SUBTRACTIVE = 0x20,
        EFFECT_BILLBOARDS = 0x40,
        EFFECT_INTERP_SIZE = 0x80,
        EFFECT_FIXED_NORMAL = 0x100,
        EFFECT_WORLD_EMITTER = 0x200
    };

    char field_0[0x50];                // 0x00
    unsigned int _renderEffects;       // 0x50
    char field_54[0x14C - 0x54];       // 0x54
};
DECOMP_SIZE_ASSERT(FFlyData, 0x14C);

class FFlyEmitter { // NAME: proto-pdb:default.pdb
public:
    void Reset();
    void StartSpawning() { _spawning = true; }
    void StopSpawning() { _spawning = false; }
    void UpdateReset(double t) { _lastTime = t; }
    const FFlyData& GetData() const { return _data; }
    void GetRenderColor(float life, Vector3& color) const;
    float GetRenderAlpha(float life, float alpha) const;
    unsigned int GetRenderTex(float life, unsigned int tex) const;
    float GetRenderSize(float life, float size) const;

    bool _spawning;                    // 0x00
    char field_1[0x20 - 0x1];          // 0x01
    double _lastTime;                  // 0x20
    char field_28[0x114 - 0x28];       // 0x28
    FFlyData _data;                    // 0x114
};
DECOMP_SIZE_ASSERT(FFlyEmitter, 0x260);

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
struct _Nonscalar_ptr_iterator_tag {};
template <class _Ty> inline void _Destroy(_Ty* _Ptr) { (_Ptr)->~_Ty(); }
template <class T> class allocator {
public:
    void destroy(T* _Ptr) { _Destroy(_Ptr); }
};
template <class _Ty, class _Alloc>
inline void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_First)
        _Al.destroy(_First);
}
template <class _FwdIt, class _Ty> inline void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}
template <class _InIt, class _OutIt>
inline _OutIt _Copy_opt(_InIt _First, _InIt _Last, _OutIt _Dest, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_Dest, ++_First)
        *_Dest = *_First;
    return (_Dest);
}
template <class _BidIt1, class _BidIt2>
inline _BidIt2 _Copy_backward_opt(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Nonscalar_ptr_iterator_tag)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}
template <class E, class T, class A> class basic_string {
private:
    char _pad00[0x1C];
};
template <class T, class A = allocator<T> > class vector {
public:
    class const_iterator {
    public:
        const_iterator(T* ptr) : _Myptr(ptr) {}
        const T& operator*() const { return *_Myptr; }
        const T* operator->() const { return _Myptr; }
        const_iterator& operator++() { ++_Myptr; return *this; }
        bool operator!=(const const_iterator& right) const { return _Myptr != right._Myptr; }
        T* _Myptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(T* ptr) : const_iterator(ptr) {}
        T& operator*() const { return (T&)**(const_iterator*)this; }
        T* operator->() const { return &**this; }
        iterator& operator++() { ++this->_Myptr; return *this; }
        iterator& operator+=(int off) { this->_Myptr += off; return *this; }
        iterator operator+(int off) const { iterator tmp = *this; return tmp += off; }
    };
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
    vector(const vector& _Right);
    ~vector() { _Tidy(); }
    vector& operator=(const vector& _Right);
    iterator begin() { return iterator(_Myfirst); }
    iterator end() { return iterator(_Mylast); }
    const T& operator[](unsigned int _Pos) const { return *(_Myfirst + _Pos); }
    void resize(unsigned int _Newsize) { resize(_Newsize, T()); }
    void resize(unsigned int _Newsize, T _Val)
    {
        if (size() < _Newsize)
            _Insert_n(end(), _Newsize - size(), _Val);
        else if (_Newsize < size())
            erase(begin() + _Newsize, end());
    }
    unsigned int size() const;
    iterator erase(iterator _First, iterator _Last);

protected:
    void _Insert_n(iterator _Where, unsigned int _Count, const T& _Val);
    void _Tidy();

private:
    A _Alval;                          // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class FFlyEffect { // NAME: proto-pdb:default.pdb
public:
    // FUNCTION: USA 0x000C83C2
    // PROTOTYPE: PREVIEW 0x004B3675 identical
    // NAME: proto-pdb:default.pdb
    struct Entry { // NAME: proto-pdb:default.pdb
        FFlyEmitter* emitter;          // 0x00
        std::vector<SwampString> textures; // 0x04
    };
    typedef std::vector<Entry> EmitterList; // NAME: proto-pdb:default.pdb

    unsigned int _flags;               // 0x00
    float _genTime;                    // 0x04
    EmitterList _emitters;             // 0x08
};
DECOMP_SIZE_ASSERT(FFlyEffect::Entry, 0x14);
DECOMP_SIZE_ASSERT(FFlyEffect, 0x18);

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
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> GetDefaultTexture() const { return _defaultTexture; }

    char field_0[0x58];                // 0x00
    ResourceHandle<TextureResource> _defaultTexture; // 0x58
    char field_5C[0x28];               // 0x5C
};
DECOMP_SIZE_ASSERT(TextureManager, 0x84);

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    typedef std::vector<ResourceHandle<TextureResource> > TextureList; // NAME: proto-pdb:default.pdb
    virtual ~FireFlyNode();
    virtual void Update();
    virtual void Render(const Matrix&) const;
    void Play();
    void Pause();
    void Reset();
    void StartSpawning();
    void StopSpawning();

protected:
    void FollowParentBone();

public:
    FFlyEffect _effect;                // 0xA4
    std::vector<TextureList> _txts;    // 0xBC
    bool _spawning;                    // 0xCC
    bool _playing;                     // 0xCD
    bool _deleteWhenDone;              // 0xCE
    unsigned int _live;                // 0xD0
    int _useParentBoneIndex;           // 0xD4
    float _sizeBound;                  // 0xD8
    float _elapsed;                    // 0xDC
    double _lastTime;                  // 0xE0
};
DECOMP_SIZE_ASSERT(FireFlyNode, 0xE8);

struct FireFlyNodeCloneInfo { // NAME: proto-pdb:default.pdb
    FireFlyNode* master;               // 0x00
    unsigned int referenceCount;       // 0x04
    float lastUpdateTime;              // 0x08
};
DECOMP_SIZE_ASSERT(FireFlyNodeCloneInfo, 0xC);

class FireFlyNodeClone : public FireFlyNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~FireFlyNodeClone();
    virtual void Update();
    virtual void Render(const Matrix&) const;
    virtual const AABox& GetBounds() const;

    FireFlyNodeCloneInfo* _info;       // 0xE8
};
DECOMP_SIZE_ASSERT(FireFlyNodeClone, 0xF0);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode : SwampSceneNode, all at 0x0; only SwampSceneNode modelled
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void GetBoneOrientation(int, Quaternion&, Vector3&) const;
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetGameTime() const { return _gameTimePublic; }
    double GetGameTimeAccurate() const { return _gameTimeInternal; }
    double GetSessionTimeAccurate() const { return _sessionTimeInternal; }

    char field_0[0x8];                 // 0x00
    float _gameTimePublic;             // 0x08
    float _sessionTimePublic;          // 0x0C
    double _gameTimeInternal;          // 0x10
    double _sessionTimeInternal;       // 0x18
    char field_20[0x120 - 0x20];       // 0x20
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class SwampRenderer; // NAME: proto-pdb:default.pdb

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() { return *_input; }
    SwampRenderer& GetRenderer() const { return *_renderer; }
    TextureManager& GetTextureManager() const { return *_textureManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x38 - 0xC];          // 0x0C
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

enum _D3DPRIMITIVETYPE { D3DPT_POINTLIST = 1, D3DPT_TRIANGLELIST = 5, D3DPT_MAX = 11 }; // NAME: proto-pdb:default.pdb
extern "C" const unsigned int D3DPRIMITIVETOVERTEXCOUNT[11][2]; // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawVerticesUP(_D3DPRIMITIVETYPE, unsigned int, const void*, unsigned int); // NAME: proto-pdb:default.pdb

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void DrawPrimitiveUP(_D3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride) const;
    void SetTexture(unsigned long stage, const ResourceHandle<TextureResource>& texture);
    float GetProjectionScaleFactor() const { return _projectionScaleFactor; }

    char field_0[0x74];                // 0x00
    float _projectionScaleFactor;      // 0x74
    char field_78[0x388 - 0x78];       // 0x78
    // !FAKE: mutable: the const DrawPrimitiveUP adds to it (retail 0x000C629A); the PDB records no mutable flag
    mutable unsigned int _primitivesDrawn; // 0x388
};
DECOMP_SIZE_ASSERT(SwampRenderer, 0x38C);

struct ParticleVertex { // NAME: proto-pdb:default.pdb
    Vector3 loc;                       // 0x00
    unsigned int color;                // 0x0C
    float size;                        // 0x10
    float sinRot;                      // 0x14
    float cosRot;                      // 0x18
    short texU;                        // 0x1C
    short texV;                        // 0x1E
};
DECOMP_SIZE_ASSERT(ParticleVertex, 0x20);

extern ParticleVertex* g_verts; // NAME: proto-pdb:default.pdb

class FFlyRenderer { // NAME: proto-pdb:default.pdb
public:
    virtual void Render(const FFlyEmitter& emitter, const FFlyParticle* startp, unsigned int count) = 0; // vtable+0x00
};
DECOMP_SIZE_ASSERT(FFlyRenderer, 0x4);

class ParticleRenderer : public FFlyRenderer { // NAME: proto-pdb:default.pdb
public:
    ParticleRenderer(const FireFlyNode& ffNode, const FireFlyNode::TextureList& txts, const Matrix* mats, float fogFactor);
    virtual void Render(const FFlyEmitter& emitter, const FFlyParticle* startp, unsigned int count);

private:
    void RenderPointSprites(const FFlyEmitter& emitter, const FFlyParticle* startp, unsigned int count);
    void RenderBillboards(const FFlyEmitter& emitter, const FFlyParticle* startp, unsigned int count);
    void RenderFixedNormalBB(const FFlyEmitter& emitter, const FFlyParticle* startp, unsigned int count);

    const FireFlyNode& _ffNode;        // 0x04
    const FireFlyNode::TextureList& _txts; // 0x08
    const Matrix* _mats;               // 0x0C
    float _fogFactor;                  // 0x10
    bool _applyFog;                    // 0x14
};
DECOMP_SIZE_ASSERT(ParticleRenderer, 0x18);

// FUNCTION: USA 0x000C6296
// PROTOTYPE: PREVIEW 0x004B1592 identical
// NAME: proto-pdb:default.pdb
void SwampRenderer::DrawPrimitiveUP(_D3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride) const
{
    _primitivesDrawn += PrimitiveCount;
    D3DDevice_DrawVerticesUP(PrimitiveType,
        D3DPRIMITIVETOVERTEXCOUNT[PrimitiveType][0] * PrimitiveCount + D3DPRIMITIVETOVERTEXCOUNT[PrimitiveType][1],
        pVertexStreamZeroData, VertexStreamZeroStride);
}

// FUNCTION: USA 0x000C6274
// PROTOTYPE: PREVIEW 0x004B155A identical
// NAME: proto-pdb:default.pdb
void FireFlyNode::Pause()
{
    _playing = false;
}

// FUNCTION: USA 0x000C627C
// PROTOTYPE: PREVIEW 0x004B1562 identical
// NAME: proto-pdb:default.pdb
void FireFlyNodeClone::Render(const Matrix& m) const
{
    _info->master->Render(m);
}

// FUNCTION: USA 0x000C6289
// PROTOTYPE: PREVIEW 0x004B156F identical
// NAME: proto-pdb:default.pdb
const AABox& FireFlyNodeClone::GetBounds() const
{
    return _info->master->GetBounds();
}

// FUNCTION: USA 0x000C6305
// PROTOTYPE: PREVIEW 0x004B1601 identical
// NAME: proto-pdb:default.pdb
void FireFlyNode::FollowParentBone()
{
    if (_useParentBoneIndex != -1) {
        Vector3 pos;
        Quaternion rot;
        static_cast<GatorNode*>(_Parent)->GetBoneOrientation(_useParentBoneIndex, rot, pos);
        SetRotation(rot);
        SetLocation(pos);
    }
}

// FUNCTION: USA 0x000C6343
// PROTOTYPE: PREVIEW 0x004B163F identical
// NAME: proto-pdb:default.pdb
void FireFlyNodeClone::Update()
{
    float t = g_swamp->GetInput().GetGameTime();
    if (_info->lastUpdateTime != t) {
        _info->lastUpdateTime = t;
        _info->master->Update();
    }
    FollowParentBone();
}

// FUNCTION: USA 0x000C63A4
// PROTOTYPE: PREVIEW 0x004B16EE identical
// NAME: proto-pdb:default.pdb
void FireFlyNode::Play()
{
    if (!_playing) {
        double t = GetOption(RenderRealTime) ? g_swamp->GetInput().GetSessionTimeAccurate()
                                             : g_swamp->GetInput().GetGameTimeAccurate();
        for (FFlyEffect::EmitterList::iterator it = _effect._emitters.begin(); it != _effect._emitters.end(); ++it) {
            it->emitter->UpdateReset(t);
        }
        _lastTime = t;
        _playing = true;
    }
}

// FUNCTION: USA 0x000C63F6
// PROTOTYPE: PREVIEW 0x004B1740 identical
// NAME: proto-pdb:default.pdb
void FireFlyNode::Reset()
{
    _live = 0;
    _elapsed = 0.0f;
    _lastTime = 0.0;
    _spawning = true;
    _playing = false;
    for (FFlyEffect::EmitterList::iterator it = _effect._emitters.begin(); it != _effect._emitters.end(); ++it) {
        it->emitter->Reset();
    }
}

// FUNCTION: USA 0x000C643C
// PROTOTYPE: PREVIEW 0x004B1786 identical
// NAME: proto-pdb:default.pdb
void FireFlyNode::StartSpawning()
{
    if (!_spawning) {
        _spawning = true;
        for (FFlyEffect::EmitterList::iterator it = _effect._emitters.begin(); it != _effect._emitters.end(); ++it) {
            it->emitter->StartSpawning();
        }
    }
}

// FUNCTION: USA 0x000C6465
// PROTOTYPE: PREVIEW 0x004B17AF identical
// NAME: proto-pdb:default.pdb
void FireFlyNode::StopSpawning()
{
    if (_spawning) {
        _spawning = false;
        for (FFlyEffect::EmitterList::iterator it = _effect._emitters.begin(); it != _effect._emitters.end(); ++it) {
            it->emitter->StopSpawning();
        }
    }
}

// FUNCTION: USA 0x000C6782
// PROTOTYPE: PREVIEW 0x004B1A97 identical
// NAME: proto-pdb:default.pdb
void ParticleRenderer::RenderPointSprites(const FFlyEmitter& emitter, const FFlyParticle* startp, unsigned int count)
{
    SwampRenderer& renderer = g_swamp->GetRenderer();
    const float projectionScaleFactor = renderer.GetProjectionScaleFactor();
    const int maxid = _txts.size();
    for (unsigned int id = 0; id < maxid + 1; id++) {
        int index = 0;
        // !FAKE: c2 regalloc; stand-in local p for the particle pointer retail keeps in ebx; no PDB local here
        const FFlyParticle* p = startp;
        while (index < count) {
            ParticleVertex* vert = g_verts;
            int numPrims = 0;
            for (int i = 0; i < 0x1000; i++, index++, p++) {
                if (id >= maxid) {
                    for (; index < count; index++, p++) {
                        if (p->_flags & FFlyParticle::ALIVE) {
                            // !FAKE: c2 x87 code; stand-in local tex; no PDB local
                            unsigned int tex = p->_texId;
                            if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_TEXID)
                                tex = emitter.GetRenderTex(p->_life, tex);
                            if (tex >= maxid)
                                break;
                        }
                    }
                } else {
                    for (; index < count; index++, p++) {
                        if (p->_flags & FFlyParticle::ALIVE) {
                            unsigned int tex = p->_texId;
                            if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_TEXID)
                                tex = emitter.GetRenderTex(p->_life, tex);
                            if (tex == id)
                                break;
                        }
                    }
                }
                if (index >= count)
                    break;
                unsigned int color = p->_color;
                if (emitter.GetData()._renderEffects & (FFlyData::EFFECT_INTERP_COLOR | FFlyData::EFFECT_INTERP_ALPHA)) {
                    Vector4 clr;
                    clr.Set(color);
                    if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_COLOR)
                        emitter.GetRenderColor(p->_life, (Vector3&)clr);
                    if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_ALPHA)
                        clr.w = emitter.GetRenderAlpha(p->_life, clr.w);
                    clr.w *= _fogFactor;
                    color = clr.GetARGB();
                } else if (_applyFog) {
                    // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand; no PDB local
                    float v0 = (color >> 24) * _fogFactor;
                    color = (_mm_cvtt_ss2si(_mm_load_ss(&v0)) << 24) | (color & 0xFFFFFF);
                }
                // !FAKE: c2 x87 code; stand-in local size; x87-held, no PDB local
                float size = p->_size;
                if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_SIZE)
                    size = emitter.GetRenderSize(p->_life, size);
                vert->loc = p->_position;
                vert->size = size * projectionScaleFactor;
                vert->color = color;
                vert++;
                numPrims++;
            }
            if (numPrims == 0)
                break;
            if (id < maxid)
                renderer.SetTexture(3, _txts[id]);
            else
                renderer.SetTexture(3, g_swamp->GetTextureManager().GetDefaultTexture());
            renderer.DrawPrimitiveUP(D3DPT_POINTLIST, numPrims, g_verts, sizeof(ParticleVertex));
        }
    }
}

// FUNCTION: USA 0x000C6A3B
// PROTOTYPE: PREVIEW 0x004B1D50 identical
// NAME: proto-pdb:default.pdb
void ParticleRenderer::RenderBillboards(const FFlyEmitter& emitter, const FFlyParticle* startp, unsigned int count)
{
    SwampRenderer& renderer = g_swamp->GetRenderer();
    const int maxid = _txts.size();
    for (unsigned int id = 0; id < maxid + 1; id++) {
        const FFlyParticle* p = startp;
        int index = 0;
        while (index < count) {
            int numPrims = 0;
            // !FAKE: c2 regalloc; stand-in local vert for the vertex pointer retail keeps in ebx; no PDB local here
            ParticleVertex* vert = g_verts;
            for (int i = 0; i < 0xFFB; i++, index++, p++) {
                if (id >= maxid) {
                    for (; index < count; index++, p++) {
                        if (p->_flags & FFlyParticle::ALIVE) {
                            // !FAKE: c2 x87 code; stand-in local tex; no PDB local
                            unsigned int tex = p->_texId;
                            if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_TEXID)
                                tex = emitter.GetRenderTex(p->_life, tex);
                            if (tex >= maxid)
                                break;
                        }
                    }
                } else {
                    for (; index < count; index++, p++) {
                        if (p->_flags & FFlyParticle::ALIVE) {
                            unsigned int tex = p->_texId;
                            if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_TEXID)
                                tex = emitter.GetRenderTex(p->_life, tex);
                            if (tex == id)
                                break;
                        }
                    }
                }
                if (index >= count)
                    break;
                unsigned int color = p->_color;
                if (emitter.GetData()._renderEffects & (FFlyData::EFFECT_INTERP_COLOR | FFlyData::EFFECT_INTERP_ALPHA)) {
                    Vector4 clr;
                    clr.Set(color);
                    if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_COLOR)
                        emitter.GetRenderColor(p->_life, (Vector3&)clr);
                    if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_ALPHA)
                        clr.w = emitter.GetRenderAlpha(p->_life, clr.w);
                    clr.w *= _fogFactor;
                    color = clr.GetARGB();
                } else if (_applyFog) {
                    // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand; no PDB local
                    float v0 = (color >> 24) * _fogFactor;
                    color = (_mm_cvtt_ss2si(_mm_load_ss(&v0)) << 24) | (color & 0xFFFFFF);
                }
                // !FAKE: c2 x87 code; stand-in local size; x87-held, no PDB local
                float size = p->_size;
                if (emitter.GetData()._renderEffects & FFlyData::EFFECT_INTERP_SIZE)
                    size = emitter.GetRenderSize(p->_life, size);
                vert->loc = p->_position;
                vert->color = color;
                vert->size = size;
                vert->sinRot = 0.0f;
                vert->cosRot = 1.0f;
                vert->texU = 1;
                vert->texV = 1;
                vert++;
                vert->loc = p->_position;
                vert->color = color;
                vert->size = size;
                vert->sinRot = 0.0f;
                vert->cosRot = 1.0f;
                vert->texU = 0;
                vert->texV = 1;
                vert++;
                vert->loc = p->_position;
                vert->color = color;
                vert->size = size;
                vert->sinRot = 0.0f;
                vert->cosRot = 1.0f;
                vert->texU = 0;
                vert->texV = 0;
                vert++;
                vert->loc = p->_position;
                vert->color = color;
                vert->size = size;
                vert->sinRot = 0.0f;
                vert->cosRot = 1.0f;
                vert->texU = 1;
                vert->texV = 1;
                vert++;
                vert->loc = p->_position;
                vert->color = color;
                vert->size = size;
                vert->sinRot = 0.0f;
                vert->cosRot = 1.0f;
                vert->texU = 0;
                vert->texV = 0;
                vert++;
                vert->loc = p->_position;
                vert->color = color;
                vert->size = size;
                vert->sinRot = 0.0f;
                vert->cosRot = 1.0f;
                vert->texU = 1;
                vert->texV = 0;
                vert++;
                numPrims += 2;
                i += 6;
            }
            if (numPrims == 0)
                break;
            if (id < maxid)
                renderer.SetTexture(0, _txts[id]);
            else
                renderer.SetTexture(0, g_swamp->GetTextureManager().GetDefaultTexture());
            renderer.DrawPrimitiveUP(D3DPT_TRIANGLELIST, numPrims, g_verts, sizeof(ParticleVertex));
        }
    }
}

// FUNCTION: USA 0x000C8116
// PROTOTYPE: PREVIEW 0x004B33C9 identical
// NAME: proto-pdb:default.pdb
template std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > * std::_Copy_opt<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *>(std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000C813A
// PROTOTYPE: PREVIEW 0x004B33ED identical
// NAME: proto-pdb:default.pdb
template std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > * std::_Copy_backward_opt<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *>(std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000C81B8
// PROTOTYPE: PREVIEW 0x004B346B identical
// NAME: proto-pdb:default.pdb
template void std::fill<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > >(std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > const &);

// FUNCTION: USA 0x000C82B1
// PROTOTYPE: PREVIEW 0x004B3564 identical
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > >,std::allocator<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > > >(std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > *,std::allocator<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > > &,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000C8651
// PROTOTYPE: PREVIEW 0x004B3904 identical
// NAME: proto-pdb:default.pdb
template void std::vector<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > >,std::allocator<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > > >::resize(unsigned int,std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > >);

// FUNCTION: USA 0x000C86AE
// PROTOTYPE: PREVIEW 0x004B3961 identical
// NAME: proto-pdb:default.pdb
template void std::vector<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > >,std::allocator<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > > >::resize(unsigned int);

// FUNCTION: USA 0x000C86CA
// PROTOTYPE: PREVIEW 0x004B397D identical
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<FFlyEffect::Entry,std::allocator<FFlyEffect::Entry> >(FFlyEffect::Entry *,FFlyEffect::Entry *,std::allocator<FFlyEffect::Entry> &,std::_Nonscalar_ptr_iterator_tag);
