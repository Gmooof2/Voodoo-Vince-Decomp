// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampFireFlyNode.cpp OBJ: Xbox\Official_Release\SwampFireFlyNode.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
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

class FFlyEmitter { // NAME: proto-pdb:default.pdb
public:
    void Reset();
    void StartSpawning() { _spawning = true; }
    void UpdateReset(double t) { _lastTime = t; }

    bool _spawning;                    // 0x00
    char field_1[0x20 - 0x1];          // 0x01
    double _lastTime;                  // 0x20
    char field_28[0x260 - 0x28];       // 0x28
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

class TextureResource; // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
    T* _Resource;                      // 0x00
};

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

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

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
