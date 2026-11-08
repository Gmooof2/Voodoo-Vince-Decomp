// UNIT: src/Libs/Swamp/Xbox/Official_Release/SoundNode.cpp OBJ: Xbox\Official_Release\SoundNode.obj OPT: /O1 /Oy
// !FAKE: c1xx front end; file-scope warning pragma; VC7.1 <string> without /EH warns C4530 and the build refuses it
#pragma warning(disable: 4530)
#include <string>
#include <functional>
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class K, class Pr, class A, bool Mfl> class _Tset_traits {
public:
    typedef K key_type;
    typedef K value_type;
    Pr comp;
};
template <class Tr> class _Tree_nod : public Tr { // NAME: proto-pdb:default.pdb
protected:
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
        char _Color;
        char _Isnil;
    };
};
template <class Tr> class _Tree : public _Tree_nod<Tr> {
    typedef typename _Tree_nod<Tr>::_Node _Node;
protected:
    typedef _Node* _Nodeptr;
    static char& _Isnil(_Nodeptr _Pnode) { return ((char&)(*_Pnode)._Isnil); }
    static _Nodeptr& _Left(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Left); }
    static _Nodeptr& _Parent(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Parent); }
    static _Nodeptr& _Right(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Right); }
    static _Nodeptr _Min(_Nodeptr _Pnode)
    {
        while (!_Isnil(_Left(_Pnode)))
            _Pnode = _Left(_Pnode);
        return (_Pnode);
    }

public:
    class const_iterator;
    friend class const_iterator;
    class const_iterator
        : public _Bidit<typename Tr::value_type, int, const typename Tr::value_type*, const typename Tr::value_type&> {
    public:
        const_iterator() : _Ptr(0) {}
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const_iterator& operator++() { _Inc(); return (*this); }
        bool operator==(const const_iterator& _Right) const { return (_Ptr == _Right._Ptr); }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
        void _Inc(); // src proto-pdb:default.pdb _Inc
        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        iterator() {}
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
        iterator& operator++() { ++(*(const_iterator*)this); return (*this); }
    };
    iterator lower_bound(const typename Tr::key_type& _Keyval) { return (iterator(_Lbound(_Keyval))); }
    iterator upper_bound(const typename Tr::key_type& _Keyval) { return (iterator(_Ubound(_Keyval))); }
    pair<iterator, iterator> equal_range(const typename Tr::key_type& _Keyval);
    iterator erase(iterator _First, iterator _Last);
    unsigned int erase(const typename Tr::key_type& _Keyval);

protected:
    _Node* _Lbound(const typename Tr::key_type& _Keyval) const;
    _Node* _Ubound(const typename Tr::key_type& _Keyval) const;

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class Pr = less<K>, class A = allocator<K> > class set
    : public _Tree<_Tset_traits<K, Pr, A, false> > {
};
}

struct AABox;      // NAME: proto-pdb:default.pdb
struct Vector3 {   // NAME: proto-pdb:default.pdb
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
struct IDirectSoundBuffer { // NAME: proto-pdb:default.pdb
    long __stdcall SetPosition(float x, float y, float z, unsigned long apply);
};

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    void GetBoneOrientation(int index, Quaternion& rot, Vector3& pos) const;
};

extern "C" long __stdcall IDirectSoundBuffer_SetMinDistance(IDirectSoundBuffer*, float, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetMaxDistance(IDirectSoundBuffer*, float, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_Pause(IDirectSoundBuffer*, unsigned long); // src proto-pdb:default.pdb IDirectSoundBuffer_Pause
extern "C" long __stdcall IDirectSoundBuffer_GetStatus(IDirectSoundBuffer*, unsigned long*); // src proto-pdb:default.pdb IDirectSoundBuffer_GetStatus
extern "C" long __stdcall IDirectSoundBuffer_SetVolume(IDirectSoundBuffer*, long); // src proto-pdb:default.pdb IDirectSoundBuffer_SetVolume
extern "C" long __stdcall IDirectSoundBuffer_StopEx(IDirectSoundBuffer*, __int64, unsigned long); // NAME: proto-pdb:default.pdb

struct tWAVEFORMATEX; // NAME: proto-pdb:default.pdb
class SoundNode;

// Retail layout: the volume getters inline with retail bodies (proto has out-of-line getters with a _mute
// test); retail reads 0x70/0x78/0x80/0x84 (proto PDB names differ by version: field_<off>).
class SoundManager { // NAME: proto-pdb:default.pdb
public:
    float GetSoundVolume() const { return field_78 * _soundVolume; }
    float GetVoiceVolume() const { return field_80 * _soundVolume; }
    float GetNoMuteVolume() const { return field_70; } // 0x70
    float GetOverlayVolume() const { return field_84 * _soundVolume; }

    char field_0[0x64];                // 0x00
    float _soundVolume;                // 0x64
    char field_68[0x8];                // 0x68
    float field_70;                    // 0x70
    char field_74[0x4];                // 0x74
    float field_78;                    // 0x78
    char field_7C[0x4];                // 0x7C
    float field_80;                    // 0x80
    float field_84;                    // 0x84

private:
    void ReleaseBuffer(bool, const tWAVEFORMATEX*, IDirectSoundBuffer*);
    void Stopped(SoundNode*);
    friend class SoundNode;
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
    SoundManager* _soundManager;       // 0x34
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    void QueueSoundFinishedMessage(const std::string&);
};

struct Matrix; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1
    };

    virtual ~SwampSceneNode();                         // vtable+0x00
    virtual const AABox& GetBounds() const;            // vtable+0x04
    virtual Vector3 GetWorldPosition() const;          // vtable+0x08
    virtual Quaternion GetWorldRotation() const;       // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);     // vtable+0x10
    virtual void Invalidate();                         // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);        // vtable+0x18
    virtual void Update();                             // vtable+0x1C
    virtual void Render(const Matrix&) const;          // vtable+0x20

public:
    SwampSceneNode(int type);
    SwampScript* GetScript() const;
    const Vector3& SetLocation(const Vector3& location);
    const Quaternion& SetRotation(const Quaternion& rotation);
    bool IsValid() const { return !(_RenderingOptions & Invalid); }

    int _Type;                         // 0x04
    std::string _Name;                 // 0x08
    SwampSceneNode* _Parent;           // 0x24
    char field_28[0x60];               // 0x28
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0x18];               // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class AnimationControl; // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }
    void AddRef() { ++_RefCount; }

    std::string _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class SoundResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    const tWAVEFORMATEX* GetFormat() const { return _format; }
    const void* GetBuffer() const { return _buffer; }

    void* _buffer;                     // 0x24
    char field_28[0x34 - 0x28];        // 0x28
    tWAVEFORMATEX* _format;            // 0x34
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    bool IsValid() const { return _Resource != 0; }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        LOOPED = 0x1,
        ONESHOT = 0x2,
        PLAYNOW = 0x4,
        USE3D = 0x8,
        NOTIFYPARENT = 0x20,
        VOICE = 0x100,
        OVERLAY = 0x200,
        NOMUTE = 0x400,
        NOAUTOPAUSE = 0x1000,
        DEFAULT_FLAGS = 0x6
    };

    SoundNode(const ResourceHandle<SoundResource>& sh, unsigned int flags, float minDist3D, float maxDist3D);
    virtual ~SoundNode();
    virtual void Update();
    virtual void Invalidate();

    void Play();
    void Stop();
    void Pause();
    void SetVolume(float vol);
    bool IsPaused() const;
    void SetMinDistance(float dist);
    void SetMaxDistance(float dist);
    bool IsPlaying() const;
    void UpdatePosition();

protected:
    void DoNotifyParent();

public:
    bool _active;                      // 0xA4
    unsigned int _flags;               // 0xA8
    int _useParentBoneIndex;           // 0xAC
    float _volume;                    // 0xB0
    float _pitch;                      // 0xB4
    float _minDist;                    // 0xB8
    float _maxDist;                    // 0xBC
    float _playTime;                   // 0xC0
    ResourceHandle<SoundResource> _sound; // 0xC4
    IDirectSoundBuffer* _obj;          // 0xC8
    AnimationControl* _owner;          // 0xCC

protected:
    static int _soundNodeCount;
};
DECOMP_SIZE_ASSERT(SoundNode, 0xD0);

class AnimationControl { // NAME: proto-pdb:default.pdb
public:
    void SoundInvalidated(SoundNode* p) { _sounds.erase(p); }

    char field_0[0x40];                // 0x00
    double _lastUpdateTime;            // 0x40
    char field_48[0x58 - 0x48];        // 0x48
    std::set<SoundNode*> _sounds;      // 0x58
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

// FUNCTION: USA 0x000C5B46
// PROTOTYPE: PREVIEW 0x004B0E49 identical
// NAME: proto-pdb:default.pdb
void SoundNode::Pause()
{
    if (_obj && _active) {
        _active = false;
        IDirectSoundBuffer_Pause(_obj, 1);
    }
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2)
// FUNCTION: USA 0x000C5B67
// PROTOTYPE: PREVIEW 0x004B0E6A near-identical
// NAME: proto-pdb:default.pdb
void SoundNode::SetVolume(float vol)
{
    if (_obj) {
        long db = -10000;
        float master;
        if (_flags & NOMUTE)
            master = g_swamp->_soundManager->GetNoMuteVolume();
        else if (_flags & VOICE)
            master = g_swamp->_soundManager->GetVoiceVolume();
        else if (_flags & OVERLAY)
            master = g_swamp->_soundManager->GetOverlayVolume();
        else
            master = g_swamp->_soundManager->GetSoundVolume();
        if (vol > 0.0f && master > 0.0f)
            db = (long)((1.0f - master * vol) * -3000.0f);
        IDirectSoundBuffer_SetVolume(_obj, db);
    }
    _volume = vol;
}
#endif

// FUNCTION: USA 0x000C5C6F
// PROTOTYPE: PREVIEW 0x004B0F71 identical
// NAME: proto-pdb:default.pdb
void SoundNode::SetMinDistance(float dist)
{
    if (_obj)
        IDirectSoundBuffer_SetMinDistance(_obj, dist, 1);
    _minDist = dist;
}

// FUNCTION: USA 0x000C5C9A
// PROTOTYPE: PREVIEW 0x004B0F9C identical
// NAME: proto-pdb:default.pdb
void SoundNode::SetMaxDistance(float dist)
{
    if (_obj)
        IDirectSoundBuffer_SetMaxDistance(_obj, dist, 1);
    _maxDist = dist;
}

// FUNCTION: USA 0x000C5CC5
// PROTOTYPE: PREVIEW 0x004B0FC7 identical
// NAME: proto-pdb:default.pdb
bool SoundNode::IsPlaying() const
{
    if (_obj) {
        unsigned long status = 0;
        IDirectSoundBuffer_GetStatus(_obj, &status);
        return (status & 3) != 0;
    }
    return false;
}

// FUNCTION: USA 0x000C5CF2
// PROTOTYPE: PREVIEW 0x004B0FF4 identical
// NAME: proto-pdb:default.pdb
bool SoundNode::IsPaused() const
{
    if (_obj) {
        unsigned long status = 0;
        IDirectSoundBuffer_GetStatus(_obj, &status);
        return (status & 2) != 0;
    }
    return false;
}

// FUNCTION: USA 0x000C5D1C
// PROTOTYPE: PREVIEW 0x004B101E identical
// NAME: proto-pdb:default.pdb
void SoundNode::UpdatePosition()
{
    if (_flags & USE3D) {
        if (_useParentBoneIndex != -1) {
            Quaternion rot;
            Vector3 pos;
            ((GatorNode*)_Parent)->GetBoneOrientation(_useParentBoneIndex, rot, pos);
            SetRotation(rot);
            SetLocation(pos);
        }
        if (_obj) {
            Vector3 pos = GetWorldPosition();
            _obj->SetPosition(pos.x, pos.y, pos.z, 1);
        }
    }
}

// FUNCTION: USA 0x000C5E2B
// PROTOTYPE: PREVIEW 0x004B112D identical
// NAME: proto-pdb:default.pdb
void SoundNode::DoNotifyParent()
{
    if (_flags & NOTIFYPARENT && _Parent) {
        SwampScript* p = _Parent->GetScript();
        if (p)
            p->QueueSoundFinishedMessage(_Name);
    }
}

// FUNCTION: USA 0x000C5E54
// PROTOTYPE: PREVIEW 0x004B1156 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): _IDirectSoundBuffer_StopEx@16 0x00179A58 has only the ghidra name IDirectSoundBuffer_StopEx in symbols.txt
void SoundNode::Stop()
{
    if (_obj) {
        IDirectSoundBuffer_StopEx(_obj, 0, 1);
        g_swamp->_soundManager->Stopped(this);
        if (_active) {
            DoNotifyParent();
            _active = false;
        }
        g_swamp->_soundManager->ReleaseBuffer((_flags & USE3D) != 0, _sound().GetFormat(), _obj);
        _playTime = 0.0f;
        _obj = 0;
    }
}
#endif

// FUNCTION: USA 0x000C5ECD
// NAME: proto-pdb:default.pdb
SoundNode::~SoundNode()
{
    IsPlaying();
    Stop();
    _soundNodeCount--;
}

// FUNCTION: USA 0x000C5EFD
// PROTOTYPE: PREVIEW 0x004B11FF identical
// NAME: proto-pdb:default.pdb
void SoundNode::Update()
{
    if (IsValid() && _active) {
        if (!IsPlaying()) {
            if (_flags & ONESHOT)
                Invalidate();
            else
                Stop();
        } else {
            UpdatePosition();
        }
    }
}

// FUNCTION: USA 0x000C6153
// PROTOTYPE: PREVIEW 0x004B1455 identical
// NAME: proto-pdb:default.pdb
SoundNode::SoundNode(const ResourceHandle<SoundResource>& sh, unsigned int flags, float minDist3D, float maxDist3D)
    : SwampSceneNode(8), _active(false), _flags(flags), _useParentBoneIndex(-1), _volume(1.0f), _pitch(0.0f),
      _minDist(minDist3D == 0.0f ? 1.0f : minDist3D), _maxDist(maxDist3D == 0.0f ? 1.0e9f : maxDist3D),
      _playTime(0.0f), _sound(sh), _obj(0), _owner(0)
{
    _soundNodeCount++;
    if (_sound.IsValid() && _sound().GetBuffer() && (_flags & PLAYNOW))
        Play();
}

// FUNCTION: USA 0x000C623E
// NAME: proto-pdb:default.pdb
void SoundNode::Invalidate()
{
    Stop();
    if (_owner) {
        _owner->SoundInvalidated(this);
        _owner = 0;
    }
    SwampSceneNode::Invalidate();
}

namespace std {
// !FAKE: linker COMDAT selection; explicit specialization with the VC 7.1 <xtree> body (C0233 Fix 1); emits the SoundNode* alias of the
// fold-VA row 0x000B9037 (SwampSoundResource.cpp) as a byte-equal COMDAT; inline gives it pick-any selection (relink same() refuses NODUPLICATES)
template <> inline void _Tree<_Tset_traits<SoundNode*, less<SoundNode*>, allocator<SoundNode*>, false> >::const_iterator::_Inc()
{
    if (_Isnil(_Ptr))
        ;
    else if (!_Isnil(_Right(_Ptr)))
        _Ptr = _Min(_Right(_Ptr));
    else {
        _Nodeptr _Pnode;
        while (!_Isnil(_Pnode = _Parent(_Ptr)) && _Ptr == _Right(_Pnode))
            _Ptr = _Pnode;
        _Ptr = _Pnode;
    }
}
#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only (C0231 lever 1); retail calls _Distance by name from erase(key) 0x000C6110
template <> void _Distance(_Tree<_Tset_traits<SoundNode*, less<SoundNode*>, allocator<SoundNode*>, false> >::iterator,
    _Tree<_Tset_traits<SoundNode*, less<SoundNode*>, allocator<SoundNode*>, false> >::iterator, unsigned int&);
#endif
#ifdef NON_MATCHING // erase(key) row guarded: neither body may be emitted (equal_range reaches _Lbound, unnamed FUN_000B93E6) (4.3 T12.c44)
// visible VC 7.1 body (retail 0x000C5E01): with it cl packs erase(key)'s erase(it,it) return temp over _Where (8-byte frame; 93.9 declared only)
template <class Tr> pair<typename _Tree<Tr>::iterator, typename _Tree<Tr>::iterator> _Tree<Tr>::equal_range(const typename Tr::key_type& _Keyval)
{
    return (pair<iterator, iterator>(lower_bound(_Keyval), upper_bound(_Keyval)));
}
template <class Tr> unsigned int _Tree<Tr>::erase(const typename Tr::key_type& _Keyval)
{
    pair<iterator, iterator> _Where = equal_range(_Keyval);
    unsigned int _Num = 0;
    _Distance(_Where.first, _Where.second, _Num);
    erase(_Where.first, _Where.second);
    return (_Num);
}
#endif
template <class Tr> typename _Tree<Tr>::_Node* _Tree<Tr>::_Ubound(const typename Tr::key_type& _Keyval) const
{
    _Node* _Pnode = _Myhead->_Parent;
    _Node* _Wherenode = _Myhead;
    while (!_Pnode->_Isnil)
        if (this->comp(_Keyval, _Pnode->_Myval)) {
            _Wherenode = _Pnode;
            _Pnode = _Pnode->_Left;
        } else
            _Pnode = _Pnode->_Right;
    return (_Wherenode);
}
}

// FUNCTION: USA 0x000C5D9B
// PROTOTYPE: PREVIEW 0x004B109D identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tset_traits<SoundNode *,std::less<SoundNode *>,std::allocator<SoundNode *>,0> >::_Node * std::_Tree<std::_Tset_traits<SoundNode *,std::less<SoundNode *>,std::allocator<SoundNode *>,0> >::_Ubound(SoundNode * const &)const;

// FUNCTION: USA 0x000C5DC4
// PROTOTYPE: PREVIEW 0x004B10C6 identical
// NAME: proto-pdb:default.pdb
template void std::_Distance2<std::_Tree<std::_Tset_traits<SoundNode *,std::less<SoundNode *>,std::allocator<SoundNode *>,0> >::iterator,unsigned int>(std::_Tree<std::_Tset_traits<SoundNode *,std::less<SoundNode *>,std::allocator<SoundNode *>,0> >::iterator,std::_Tree<std::_Tset_traits<SoundNode *,std::less<SoundNode *>,std::allocator<SoundNode *>,0> >::iterator,unsigned int &,std::bidirectional_iterator_tag);

// FUNCTION: USA 0x000C6110
// PROTOTYPE: PREVIEW 0x004B1412 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // wall: match_one 100 needs visible equal_range, whose _Lbound callee is unnamed in retail (FUN_000B93E6, fold, no pin; 93.9 declared only) (4.3 T12)
template unsigned int std::_Tree<std::_Tset_traits<SoundNode *,std::less<SoundNode *>,std::allocator<SoundNode *>,0> >::erase(SoundNode * const &);
#endif
