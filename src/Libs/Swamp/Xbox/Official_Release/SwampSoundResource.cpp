// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampSoundResource.cpp OBJ: Xbox\Official_Release\SwampSoundResource.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class T> struct less {
    bool operator()(const T& _Left, const T& _Right) const { return (_Left < _Right); }
};
template <class E, class T, class A> class basic_string {
public:
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    unsigned int size() const { return (_Mysize); }
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0); // retail ?_Tidy@...@IAEX_NI@Z 0x000111D6

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
template <class K, class Pr, class A, bool Mfl> class _Tset_traits {
public:
    typedef K key_type;
    typedef K value_type;
    Pr comp;
};
// _Tree: VC 7.1 <xtree> const_iterator::_Inc body
template <class Tr> class _Tree_nod : public Tr {
protected:
    typedef typename Tr::value_type value_type;

public:
    struct _Node;
    typedef _Node* _Genptr;
    struct _Node {
        _Genptr _Left;                 // 0x00
        _Genptr _Parent;               // 0x04
        _Genptr _Right;                // 0x08
        value_type _Myval;             // 0x0C
        char _Color;
        char _Isnil;
    };
};
template <class Tr> class _Tree : public _Tree_nod<Tr> {
protected:
    typedef typename _Tree_nod<Tr>::_Node _Node;
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
    class const_iterator {
    public:
        const_iterator(_Nodeptr _Pnode) : _Ptr(_Pnode) {}
        const typename Tr::value_type& operator*() const { return (*_Ptr)._Myval; }
        const_iterator& operator++()
        {
            _Inc();
            return (*this);
        }
        bool operator==(const const_iterator& _Other) const { return (_Ptr == _Other._Ptr); }
        bool operator!=(const const_iterator& _Other) const { return (!(*this == _Other)); }
        void _Inc()
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
        _Nodeptr _Ptr;
    };
    const_iterator begin() const { return (const_iterator(_Left(_Myhead))); }
    const_iterator end() const { return (const_iterator(_Myhead)); }
    unsigned int size() const { return (_Mysize); }
    bool empty() const { return (size() == 0); }

protected:
    _Nodeptr _Myhead;                  // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class Pr, class A> class set : public _Tree<_Tset_traits<K, Pr, A, false> > {
};
// list: VC 7.1 <list> layout; ctor and _Insert out of line in retail (0x000B9F2D, 0x000BA3AC)
template <class T, class A> class list {
public:
    struct _Node;
    typedef _Node* _Nodeptr;
    struct _Node {
        _Nodeptr _Next;                // 0x00
        _Nodeptr _Prev;                // 0x04
        T _Myval;                      // 0x08
    };
    class iterator {
    public:
        iterator(_Nodeptr _Pnode) : _Ptr(_Pnode) {}
        T& operator*() const { return _Ptr->_Myval; }
        iterator& operator++() { _Ptr = _Ptr->_Next; return (*this); }
        bool operator==(const iterator& _Right) const { return (_Ptr == _Right._Ptr); }
        bool operator!=(const iterator& _Right) const { return (!(*this == _Right)); }
        _Nodeptr _Ptr;
    };
    list();
    iterator begin() { return (iterator(_Myhead->_Next)); }
    iterator end() { return (iterator(_Myhead)); }
    void push_back(const T& _Val) { _Insert(end(), _Val); }
    void _Insert(iterator _Where, const T& _Val);

private:
    char _Alval;                       // 0x00
    _Nodeptr _Myhead;                  // 0x04
    unsigned int _Mysize;              // 0x08
};
}

class SoundStreamer;     // NAME: proto-pdb:default.pdb
class SoundNode;         // NAME: proto-pdb:default.pdb
class SwampScript;       // NAME: proto-pdb:default.pdb

struct IDirectSoundBuffer { // NAME: proto-pdb:default.pdb
    long __stdcall SetPosition(float x, float y, float z, unsigned long dwApply);
};
struct _DSMIXBINVOLUMEPAIR; // NAME: proto-pdb:default.pdb
struct _DSMIXBINS { // NAME: proto-pdb:default.pdb
    unsigned long dwMixBinCount;                       // 0x00
    const _DSMIXBINVOLUMEPAIR* lpMixBinVolumePairs;    // 0x04
};
DECOMP_SIZE_ASSERT(_DSMIXBINS, 0x8);
struct IDirectSound { // NAME: proto-pdb:default.pdb
    long __stdcall SetOrientation(float xFront, float yFront, float zFront, float xTop, float yTop, float zTop, unsigned long dwApply);
};

extern "C" long __stdcall IDirectSound_SetOrientation(IDirectSound*, float, float, float, float, float, float, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetPosition(IDirectSoundBuffer*, float, float, float, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_StopEx(IDirectSoundBuffer*, __int64, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_GetStatus(IDirectSoundBuffer*, unsigned long*); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetBufferData(IDirectSoundBuffer*, void*, unsigned long); // NAME: proto-pdb:default.pdb
struct tWAVEFORMATEX; // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetFormat(IDirectSoundBuffer*, const tWAVEFORMATEX*); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetVolume(IDirectSoundBuffer*, long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetFrequency(IDirectSoundBuffer*, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetMaxDistance(IDirectSoundBuffer*, float, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetMinDistance(IDirectSoundBuffer*, float, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetMixBins(IDirectSoundBuffer*, const _DSMIXBINS*); // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
protected:
    ResourceBase(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name);

public:
    ~ResourceBase() {}
    void SubRef() { --_RefCount; }

    std::basic_string<char, std::char_traits<char>, std::allocator<char> > _Name; // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

void operator delete[](void* p);

struct tWAVEFORMATEX { // NAME: proto-pdb:default.pdb
    unsigned short wFormatTag;         // 0x00
    unsigned short nChannels;          // 0x02
    unsigned long nSamplesPerSec;      // 0x04
    unsigned long nAvgBytesPerSec;     // 0x08
    unsigned short nBlockAlign;        // 0x0C
    unsigned short wBitsPerSample;     // 0x0E
    unsigned short cbSize;             // 0x10
};

class SoundResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    SoundResource(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name);
    ~SoundResource();

    unsigned char* _buffer;            // 0x24
    unsigned int _bufferLen;           // 0x28
    unsigned int _offset;              // 0x2C
    unsigned int _dataLen;             // 0x30
    tWAVEFORMATEX* _format;            // 0x34
    float _playbackTime;               // 0x38
    unsigned char* _myData;            // 0x3C
};
DECOMP_SIZE_ASSERT(SoundResource, 0x40);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

class SmartBase { // NAME: proto-pdb:default.pdb
protected:
    virtual ~SmartBase();              // vtable+0x00
public:
    void SubRef()
    {
        if (--_RefCount == 0)
            delete this;
    }

    int _RefCount;                     // 0x04
};

class HottieBundle; // NAME: proto-pdb:default.pdb

class BundleWrapper : public SmartBase { // NAME: proto-pdb:default.pdb
public:
    HottieBundle* _bundle;             // 0x08
    unsigned int _size;                // 0x0C
    unsigned int _flags;               // 0x10
};
DECOMP_SIZE_ASSERT(BundleWrapper, 0x14);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    Ptr() : _Object(0) {}
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    T* operator->() { return _Object; }

    T* _Object;                        // 0x00
};

struct HottieRawData { // NAME: proto-pdb:default.pdb
    void* header;                      // 0x00
    void* data;                        // 0x04
    unsigned int size;                 // 0x08
};
DECOMP_SIZE_ASSERT(HottieRawData, 0xC);

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    HottieRawData GetDataInBundle(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name, Ptr<BundleWrapper>& bundle);
    ResourceHandle<T> Load(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >&);

    char field_0[0x58];                // 0x00
};

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AABox;      // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb
struct Matrix;     // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                         // vtable+0x00
    virtual const AABox& GetBounds() const;            // vtable+0x04
    virtual Vector3 GetWorldPosition() const;          // vtable+0x08
    SwampScript* GetScript() const;
    void AddChild(SwampSceneNode*);
    const Vector3& SetLocation(const Vector3&);

    int _Type;                         // 0x04
    std::basic_string<char, std::char_traits<char>, std::allocator<char> > _Name; // 0x08
};

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetVolume(float);
    void UpdatePosition();
    void Stop();

    char field_pad[0xB0 - sizeof(SwampSceneNode)]; // after SwampSceneNode (members unread here)
    float _volume;                     // 0xB0
};

class SoundStreamer { // NAME: proto-pdb:default.pdb
public:
    void Stop();
    void SetVolume(float);

    char field_0[0x10];                // 0x00
    float _volume;                     // 0x10
};

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void AddNode(SwampSceneNode*, unsigned int);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
std::basic_string<char, std::char_traits<char>, std::allocator<char> > StripPath(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >&);
std::basic_string<char, std::char_traits<char>, std::allocator<char> > StripExtension(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >&);
}

// Retail layout: SetAllVolumes takes four floats (PDB locals sound, music, cinematic, nomute); the class record's
// 0x70 member name differs by version: field_70.
class SoundManager : public ResourceManager<SoundResource> { // NAME: proto-pdb:default.pdb
public:
    class BufferMgr { // NAME: proto-pdb:default.pdb
    public:
        typedef std::list<IDirectSoundBuffer*, std::allocator<IDirectSoundBuffer*> > FreeBufferList; // NAME: proto-pdb:default.pdb

        BufferMgr();
        bool Create(unsigned int max);
        void Release(IDirectSoundBuffer* buffer);
        void StopFreeList();
        void Clean();

    private:
        static bool ResetBuffer(bool is3D, IDirectSoundBuffer* buffer, const tWAVEFORMATEX* fmt, bool isVoice);

    public:

        unsigned int _count;           // 0x00
        unsigned int _max;             // 0x04
        FreeBufferList _freeList;      // 0x08
    };

    void SetRolloffDist(float);
    void SetSoundVolume(float);
    void SetMusicVolume(float);
    void SetCinematicVolume(float);
    void SetAllVolumes(float sound, float music, float cinematic, float nomute);
    void StopAllSounds();
    void StopAllStreams();
    void SetVolumeSuppression(bool);
    static void PlaySound(const ResourceHandle<SoundResource>& sh, const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name, unsigned int flags, SwampSceneNode* parent, bool bAttach, float minDist3D, float maxDist3D);
    static void PlaySound(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name, unsigned int flags, SwampSceneNode* parent, bool bAttach, float minDist3D, float maxDist3D);
    static SoundNode* CreateSound(const ResourceHandle<SoundResource>& sh, const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name, unsigned int flags, SwampSceneNode* parent, bool bAttach);
    static SoundNode* CreateSound(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name, unsigned int flags, SwampSceneNode* parent, bool bAttach);

private:
    static void PrepareSound(SoundNode* psound, const ResourceHandle<SoundResource>& sh, const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name, SwampSceneNode* parent, bool bAttach);
    void ResetVolumes();
    void ReleaseBuffer(bool is3D, const tWAVEFORMATEX* fmt, IDirectSoundBuffer* buffer);
    void SetupMixBins(_DSMIXBINS& dsmb, bool is3D, const tWAVEFORMATEX* fmt, bool isVoice) const;

public:
    IDirectSound* _dSound;             // 0x58
    float _rolloffFactor;              // 0x5C
    float _rolloffDist;                // 0x60
    float _soundVolume;                // 0x64
    float _musicVolume;                // 0x68
    float _cinematicVolume;            // 0x6C
    float field_70;                    // 0x70
    char field_74[0x91 - 0x74];        // 0x74 (retail grew by 8 below _volSuppressed)
    bool _volSuppressed;               // 0x91 proto 0x89
    char field_92[0xA0 - 0x92];        // 0x92
    std::set<SoundNode*, std::less<SoundNode*>, std::allocator<SoundNode*> > _playing;             // 0xA0 proto 0x98
    std::set<SoundStreamer*, std::less<SoundStreamer*>, std::allocator<SoundStreamer*> > _strPlaying; // 0xAC proto 0xA4
    BufferMgr _monoVoices;             // 0xB8 proto 0xB0
    BufferMgr _stereoVoices;           // 0xCC proto 0xC4
    BufferMgr _3DVoices;               // 0xE0 proto 0xD8
};
DECOMP_SIZE_ASSERT(SoundManager, 0xF4);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xC];                 // 0x00
    SwampScene* _scene;                // 0x0C
    char field_10[0x34 - 0x10];        // 0x10
    SoundManager* _soundManager;       // 0x34
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000B8E80
// PROTOTYPE: PREVIEW 0x004A3C58 identical
// NAME: proto-pdb:default.pdb
bool SoundManager::BufferMgr::Create(unsigned int max)
{
    _count = 0;
    _max = max;
    return true;
}

// FUNCTION: USA 0x000B8E94
// PROTOTYPE: PREVIEW 0x004A3C6C identical
// NAME: proto-pdb:default.pdb
void SoundManager::SetRolloffDist(float dist)
{
    _rolloffDist = dist;
}

// FUNCTION: USA 0x000B8E9E
// PROTOTYPE: PREVIEW 0x004A3C76 near-identical
// NAME: proto-pdb:default.pdb
void SoundManager::SetVolumeSuppression(bool flag)
{
    if (flag) {
        if (!_volSuppressed)
            _volSuppressed = true;
    } else {
        if (_volSuppressed)
            _volSuppressed = false;
    }
}

// FUNCTION: USA 0x000B8EDE
// PROTOTYPE: PREVIEW 0x004A3CB6 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 97.24, bytes mismatch (whole-function eax/ecx swap: retail keeps filedata in eax and the
// chunk offset in ecx; 22 forms tried incl. nested ifs, chunk cursor, RIFFHEADER pointers, return orders)
bool SwampSound_GetWaveData(const unsigned char* filedata, unsigned int filesize, tWAVEFORMATEX** format, unsigned int* dataoffset, unsigned int* datalen)
{
    if (filesize < 0x2E)
        return false;
    if (*(const unsigned int*)filedata != 0x46464952 || *(const unsigned int*)(filedata + 8) != 0x45564157 || *(const unsigned int*)(filedata + 0xC) != 0x20746D66)
        return false;
    unsigned int fmtLen = *(const unsigned int*)(filedata + 0x10);
    if (fmtLen < 0x12)
        return false;
    *format = (tWAVEFORMATEX*)(filedata + 0x14);
    unsigned int skip = 0;
    if (*(const unsigned int*)(filedata + fmtLen + 0x14) == 0x44414150) {
        skip = *(const unsigned int*)(filedata + fmtLen + 0x18) + 8;
        if (skip + fmtLen + 0x18 > filesize)
            return false;
    }
    skip += fmtLen;
    if (*(const unsigned int*)(filedata + skip + 0x14) != 0x61746164)
        return false;
    *datalen = *(const unsigned int*)(filedata + skip + 0x18);
    *dataoffset = skip + 0x1C;
    return filedata + filesize >= (const unsigned char*)(*datalen + *dataoffset);
}
#endif

// FUNCTION: USA 0x000B8F63
// PROTOTYPE: PREVIEW 0x004A3D3B identical
// NAME: proto-pdb:default.pdb
long __stdcall IDirectSound::SetOrientation(float xFront, float yFront, float zFront, float xTop, float yTop, float zTop, unsigned long dwApply)
{
    return IDirectSound_SetOrientation(this, xFront, yFront, zFront, xTop, yTop, zTop, dwApply);
}

// FUNCTION: USA 0x000B8FCA
// NAME: proto-pdb:default.pdb
long __stdcall IDirectSoundBuffer::SetPosition(float x, float y, float z, unsigned long dwApply)
{
    return IDirectSoundBuffer_SetPosition(this, x, y, z, dwApply);
}

// FUNCTION: USA 0x000B9037
// PROTOTYPE: PREVIEW 0x004A3DCB identical
// NAME: proto-pdb:default.pdb
// fold VA: SoundNode.cpp emits its SoundNode* alias with this body (C0233 Fix 1)
template void std::_Tree<std::_Tset_traits<SoundStreamer *,std::less<SoundStreamer *>,std::allocator<SoundStreamer *>,0> >::const_iterator::_Inc();

// FUNCTION: USA 0x000B9277
// PROTOTYPE: PREVIEW 0x004A4005 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 5): _IDirectSoundBuffer_SetBufferData@12, _SetFormat@8, _SetVolume@8, _SetFrequency@8, _SetMixBins@8 have only ghidra names in symbols.txt
bool SoundManager::BufferMgr::ResetBuffer(bool is3D, IDirectSoundBuffer* buffer, const tWAVEFORMATEX* fmt, bool isVoice)
{
    IDirectSoundBuffer_SetBufferData(buffer, 0, 0);
    if (IDirectSoundBuffer_SetFormat(buffer, fmt) < 0)
        return false;
    IDirectSoundBuffer_SetVolume(buffer, 0);
    IDirectSoundBuffer_SetFrequency(buffer, 0);
    if (is3D) {
        buffer->SetPosition(0.0f, 0.0f, 0.0f, 1);
        IDirectSoundBuffer_SetMaxDistance(buffer, 1000000000.0f, 1);
        IDirectSoundBuffer_SetMinDistance(buffer, 1.0f, 1);
    }
    _DSMIXBINS dsmb;
    g_swamp->_soundManager->SetupMixBins(dsmb, is3D, fmt, isVoice);
    // !FAKE: c2 register allocation; cast; without it xor ecx/setge cl/mov al,cl (match_one 96.94 -> 100.0)
    return (bool)(IDirectSoundBuffer_SetMixBins(buffer, &dsmb) >= 0);
}
#endif

// FUNCTION: USA 0x000B940F
// PROTOTYPE: PREVIEW 0x004A4174 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): _IDirectSoundBuffer_StopEx@16 0x00179A58 has only the ghidra name IDirectSoundBuffer_StopEx in symbols.txt
void SoundManager::BufferMgr::StopFreeList()
{
    for (FreeBufferList::iterator it = _freeList.begin(); it != _freeList.end(); ++it)
        IDirectSoundBuffer_StopEx(*it, 0, 0);
}
#endif

// FUNCTION: USA 0x000B9434
// PROTOTYPE: PREVIEW 0x004A4199 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): _IDirectSoundBuffer_SetBufferData@12 0x0017A89C has only the ghidra name IDirectSoundBuffer_SetBufferData in symbols.txt
void SoundManager::BufferMgr::Clean()
{
    bool v0;
    do {
        v0 = true;
        for (FreeBufferList::iterator it = _freeList.begin(); it != _freeList.end(); ++it) {
            unsigned long status = 0;
            IDirectSoundBuffer_GetStatus(*it, &status);
            if (status & 1)
                v0 = false;
            else
                IDirectSoundBuffer_SetBufferData(*it, 0, 0);
        }
    } while (!v0);
}
#endif

// FUNCTION: USA 0x000B94CB
// PROTOTYPE: PREVIEW 0x004A4230 near-identical
// NAME: proto-pdb:default.pdb
void SoundManager::StopAllSounds()
{
    while (!_playing.empty())
        (*_playing.begin())->Stop();
}

// FUNCTION: USA 0x000B94EF
// PROTOTYPE: PREVIEW 0x004A4254 near-identical
// NAME: proto-pdb:default.pdb
void SoundManager::StopAllStreams()
{
    while (!_strPlaying.empty())
        (*_strPlaying.begin())->Stop();
}

// FUNCTION: USA 0x000B9513
// PROTOTYPE: PREVIEW 0x004A4278 near-identical
// NAME: proto-pdb:default.pdb
void SoundManager::ResetVolumes()
{
    for (std::set<SoundNode*, std::less<SoundNode*>, std::allocator<SoundNode*> >::const_iterator i = _playing.begin(); i != _playing.end(); ++i)
        (*i)->SetVolume((*i)->_volume);
    for (std::set<SoundStreamer*, std::less<SoundStreamer*>, std::allocator<SoundStreamer*> >::const_iterator i = _strPlaying.begin(); i != _strPlaying.end(); ++i)
        (*i)->SetVolume((*i)->_volume);
}

// FUNCTION: USA 0x000B9DEB
// PROTOTYPE: PREVIEW 0x004A4B1C identical
// NAME: proto-pdb:default.pdb
void SoundManager::SetSoundVolume(float vol)
{
    _soundVolume = vol;
    ResetVolumes();
}

// FUNCTION: USA 0x000B9DFA
// PROTOTYPE: PREVIEW 0x004A4B2B identical
// NAME: proto-pdb:default.pdb
void SoundManager::SetMusicVolume(float vol)
{
    _musicVolume = vol;
    ResetVolumes();
}

// FUNCTION: USA 0x000B9E09
// PROTOTYPE: PREVIEW 0x004A4B3A identical
// NAME: proto-pdb:default.pdb
void SoundManager::SetCinematicVolume(float vol)
{
    _cinematicVolume = vol;
    ResetVolumes();
}

// FUNCTION: USA 0x000B9E18
// PROTOTYPE: PREVIEW 0x004A4B58 identical
// NAME: proto-pdb:default.pdb
void SoundManager::SetAllVolumes(float sound, float music, float cinematic, float nomute)
{
    _soundVolume = sound;
    _musicVolume = music;
    _cinematicVolume = cinematic;
    field_70 = nomute;
    ResetVolumes();
}

// FUNCTION: USA 0x000B9F40
// PROTOTYPE: PREVIEW 0x004A4C60 identical
// NAME: proto-pdb:default.pdb
SoundResource::~SoundResource()
{
    delete[] _myData;
}

// FUNCTION: USA 0x000B9F59
// PROTOTYPE: PREVIEW 0x004A4C79 identical
// NAME: proto-pdb:default.pdb
SoundManager::BufferMgr::BufferMgr() : _count(0), _max(0)
{
}

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    BeepFile();
    ~BeepFile();
    bool Open(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& filename);
    unsigned int Size();
    bool Read(void* buffer, unsigned int size);

    char field_0[0x20];                // 0x00 (_fh, _filename; not reached)
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

void* operator new[](unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb
bool SwampSound_GetWaveData(const unsigned char* filedata, unsigned int filesize, tWAVEFORMATEX** format, unsigned int* dataoffset, unsigned int* datalen);

// FUNCTION: USA 0x000B9F6F
// PROTOTYPE: PREVIEW 0x004A4C8F identical
// NAME: proto-pdb:default.pdb
SoundResource::SoundResource(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name)
    : ResourceBase(name), _buffer(0), _bufferLen(0), _offset(0), _dataLen(0), _format(0), _playbackTime(0.0f), _myData(0)
{
    std::basic_string<char, std::char_traits<char>, std::allocator<char> > shortname = BeepLib::StripPath(name);
    Ptr<BundleWrapper> myBundle;
    HottieRawData rawData = g_swamp->_soundManager->GetDataInBundle(shortname, myBundle);
    if (rawData.size) {
        // !FAKE: c1xx frame packing; bare block; the uint->float temp below shares offset's slot (+8) as in retail (C0232)
        {
            unsigned int offset = 0;
            if (!SwampSound_GetWaveData((const unsigned char*)rawData.header, rawData.size, &_format, &offset, &_dataLen) || offset != 0x48) {
                _format = 0;
                _dataLen = 0;
                return;
            }
        }
        _playbackTime = (float)_dataLen / (float)_format->nAvgBytesPerSec;
        _buffer = (unsigned char*)myBundle->_bundle;
        _bufferLen = myBundle->_size;
        _offset = (unsigned char*)rawData.data - _buffer;
        _valid = true;
        return;
    }
    BeepFile f;
    if (!f.Open(name))
        return;
    if (f.Size() < 0x2E)
        return;
    _bufferLen = f.Size();
    _buffer = new("SwampSoundResource.cpp", 199) unsigned char[_bufferLen];
    if (f.Read(_buffer, _bufferLen) && SwampSound_GetWaveData(_buffer, _bufferLen, &_format, &_offset, &_dataLen)) {
        _playbackTime = (float)_dataLen / (float)_format->nAvgBytesPerSec;
        _myData = _buffer;
        _valid = true;
    } else {
        delete[] _buffer;
        _buffer = 0;
        _bufferLen = 0;
        _format = 0;
        _offset = 0;
        _dataLen = 0;
    }
}

// FUNCTION: USA 0x000BA2E1
// PROTOTYPE: PREVIEW 0x004A4FFC identical
// NAME: proto-pdb:default.pdb
void SoundManager::PrepareSound(SoundNode* psound, const ResourceHandle<SoundResource>& sh, const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name, SwampSceneNode* parent, bool bAttach)
{
    if (name.size()) {
        psound->_Name = name;
    } else if (sh._Resource) {
        std::basic_string<char, std::char_traits<char>, std::allocator<char> > name = BeepLib::StripExtension(BeepLib::StripPath(sh._Resource->_Name));
        psound->_Name = name;
    }
    if (parent) {
        if (bAttach) {
            parent->AddChild(psound);
        } else {
            g_swamp->_scene->AddNode(psound, 0);
            psound->SetLocation(parent->GetWorldPosition());
        }
    } else {
        g_swamp->_scene->AddNode(psound, 0);
    }
    psound->SetVolume(1.0f);
    psound->UpdatePosition();
}

// FUNCTION: USA 0x000BA4FC
// PROTOTYPE: PREVIEW 0x004A5217 identical
// NAME: proto-pdb:default.pdb
void SoundManager::BufferMgr::Release(IDirectSoundBuffer* buffer)
{
    _freeList.push_back(buffer);
}

// FUNCTION: USA 0x000BA510
// PROTOTYPE: PREVIEW 0x004A522B identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 94.88, bytes mismatch (retail false paths jump past the handle dtor test to leave; ours join it)
void SoundManager::PlaySound(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name, unsigned int flags, SwampSceneNode* parent, bool bAttach, float minDist3D, float maxDist3D)
{
    ResourceHandle<SoundResource> sh = g_swamp->_soundManager->Load(name);
    if (sh._Resource || (parent && bAttach && parent->GetScript() && (flags & 0x20)))
        PlaySound(sh, name, flags, parent, bAttach, minDist3D, maxDist3D);
}
#endif

// FUNCTION: USA 0x000BA57F
// PROTOTYPE: PREVIEW 0x004A529A identical
// NAME: proto-pdb:default.pdb
SoundNode* SoundManager::CreateSound(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& name, unsigned int flags, SwampSceneNode* parent, bool bAttach)
{
    ResourceHandle<SoundResource> sh = g_swamp->_soundManager->Load(name);
    return CreateSound(sh, name, flags, parent, bAttach);
}

// FUNCTION: USA 0x000BA5BB
// PROTOTYPE: PREVIEW 0x004A52D6 near-identical
// NAME: proto-pdb:default.pdb
void SoundManager::ReleaseBuffer(bool is3D, const tWAVEFORMATEX* fmt, IDirectSoundBuffer* buffer)
{
    if (is3D) {
        _3DVoices.Release(buffer);
    } else {
        switch (fmt->nChannels) {
        case 1:
            _monoVoices.Release(buffer);
            break;
        case 2:
            _stereoVoices.Release(buffer);
            break;
        }
    }
}
