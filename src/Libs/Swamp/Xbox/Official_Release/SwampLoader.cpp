// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampLoader.cpp OBJ: Xbox\Official_Release\SwampLoader.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
extern "C" void* __cdecl memset(void*, int, unsigned int); // NAME: proto-pdb:default.pdb
#pragma intrinsic(memset)
#include <xmmintrin.h>

inline void* operator new(unsigned int, void* _Where) throw() { return (_Where); } // VC 7.1 <new>

namespace std { // NAME: proto-pdb:default.pdb
template<class _Elem>
struct char_traits {};

template<class _Ty> inline
_Ty* _Allocate(unsigned int _Count, _Ty*)
{
    return ((_Ty*)operator new(_Count * sizeof(_Ty)));
}

template<class _T1, class _T2> inline
void _Construct(_T1* _Ptr, const _T2& _Val)
{
    new ((void*)_Ptr) _T1(_Val);
}

template<class _Ty>
class allocator {
public:
    void deallocate(_Ty* _Ptr, unsigned int) { operator delete(_Ptr); }
    _Ty* allocate(unsigned int _Count) { return (_Allocate(_Count, (_Ty*)0)); }
    void construct(_Ty* _Ptr, const _Ty& _Val) { _Construct(_Ptr, _Val); }
    unsigned int max_size() const
    {
        unsigned int _Count = (unsigned int)(-1) / sizeof(_Ty);
        return (0 < _Count ? _Count : 1);
    }
};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    enum { _BUF_SIZE = 16 / sizeof(_Elem) < 1 ? 1 : 16 / sizeof(_Elem) };
    // !FAKE: inliner; hand form of the VC 7.1 default ctor's _Tidy(); retail inlines _Tidy(false) but calls
    // _Tidy(true) by name, so _Tidy stays declared only (C0245)
    basic_string()
    {
        _Myres = _BUF_SIZE - 1;
        _Eos(0);
    }
    basic_string(const basic_string& _Right);
    basic_string(const _Elem* _Ptr);
    ~basic_string() { _Tidy(true, 0); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& operator+=(const _Elem* _Ptr) { return append(_Ptr); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, npos); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& append(const _Elem* _Ptr);
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    bool empty() const { return (_Mysize == 0); }
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Eos(unsigned int _Newsize);
    void _Tidy(bool _Built, unsigned int _Newsize);

private:
    union _Bxty {
        _Elem _Buf[_BUF_SIZE];
        _Elem* _Ptr;
    };
    char _Alval;                       // 0x00
    _Bxty _Bx;                         // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
typedef basic_string<char> string;
template<class _Elem, class _Traits, class _Alloc>
basic_string<_Elem, _Traits, _Alloc> operator+(const basic_string<_Elem, _Traits, _Alloc>& _Left,
                                               const basic_string<_Elem, _Traits, _Alloc>& _Right);
template<class _Elem, class _Traits, class _Alloc>
basic_string<_Elem, _Traits, _Alloc> operator+(const _Elem* _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right);
template<class _Elem, class _Traits, class _Alloc>
basic_string<_Elem, _Traits, _Alloc> operator+(const basic_string<_Elem, _Traits, _Alloc>& _Left, const _Elem* _Right);

template <class T> struct less {};
template <class T1, class T2> struct pair {
    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef pair<const K, T> value_type;
};
template <class Tr> class _Tree {
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
    };
public:
    class const_iterator {
    public:
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const_iterator& operator++() { _Inc(); return *this; }
        bool operator!=(const const_iterator& _Right) const { return _Ptr != _Right._Ptr; }
        void _Inc();
        _Node* _Ptr;
    };
    const_iterator begin() const { return const_iterator(_Myhead->_Left); }
    const_iterator end() const { return const_iterator(_Myhead); }

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class multimap
    : public _Tree<_Tmap_traits<K, T, Pr, A, true> > {
};

struct _Nonscalar_ptr_iterator_tag {};
struct _Scalar_ptr_iterator_tag {};

template<class _T1, class _T2> inline
_Nonscalar_ptr_iterator_tag _Ptr_cat(_T1&, _T2&)
{
    _Nonscalar_ptr_iterator_tag _Cat;
    return (_Cat);
}

template<class _Ty> inline
_Scalar_ptr_iterator_tag _Ptr_cat(_Ty**, _Ty**)
{
    _Scalar_ptr_iterator_tag _Cat;
    return (_Cat);
}

// retail calls these out of line at their named fold VAs (0x0007F3FD, 0x00033F35): declared only
template<class _InIt, class _FwdIt, class _Alloc>
_FwdIt _Uninitialized_copy(_InIt _First, _InIt _Last, _FwdIt _Dest, _Alloc& _Al);
template<class _OutIt, class _Diff, class _Ty> inline
void fill_n(_OutIt _First, _Diff _Count, const _Ty& _Val)
{
    for (; 0 < _Count; --_Count, ++_First)
        *_First = _Val;
}

template<class _Ty, class _Diff, class _Tval> inline
void _Uninit_fill_n(_Ty* _First, _Diff _Count, const _Tval& _Val, allocator<_Ty>&, _Scalar_ptr_iterator_tag)
{
    fill_n(_First, _Count, _Val);
}

template<class _FwdIt, class _Diff, class _Tval, class _Alloc> inline
void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al)
{
    _Uninit_fill_n(_First, _Count, _Val, _Al, _Ptr_cat(_First, _First));
}

template<class _Ty, class _Alloc> inline
void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al, _Scalar_ptr_iterator_tag)
{
}

template<class _Ty, class _Alloc> inline
void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al)
{
    _Destroy_range(_First, _Last, _Al, _Ptr_cat(_First, _Last));
}

template<class _Ty, class _Ax = allocator<_Ty> >
class deque {
public:
    enum {
        _DEQUEMAPSIZ = 8,
        _DEQUESIZ = sizeof(_Ty) <= 1 ? 16 : sizeof(_Ty) <= 2 ? 8 : sizeof(_Ty) <= 4 ? 4 : sizeof(_Ty) <= 8 ? 2 : 1
    };
    typedef deque<_Ty, _Ax> _Myt;
    typedef unsigned int size_type;
    typedef _Ty* _Tptr;
    typedef _Tptr* _Mapptr;
    typedef const _Ty& const_reference;

    class const_iterator {
    public:
        const_reference operator*() const
        {
            size_type _Block = _Myoff / _DEQUESIZ;
            size_type _Off = _Myoff - _Block * _DEQUESIZ;
            if (_Mycont->_Mapsize <= _Block)
                _Block -= _Mycont->_Mapsize;
            return ((_Mycont->_Map)[_Block][_Off]);
        }

    protected:
        const _Myt* _Mycont;               // 0x00
        size_type _Myoff;                  // 0x04
    };
    friend class const_iterator;

    deque() : _Map(0), _Mapsize(0), _Myoff(0), _Mysize(0) {}
    ~deque() { _Tidy(); }
    size_type max_size() const { return (this->_Alval.max_size()); }
    void push_back(const _Ty& _Val)
    {
        if ((_Myoff + _Mysize) % _DEQUESIZ == 0 && _Mapsize <= (_Mysize + _DEQUESIZ) / _DEQUESIZ)
            _Growmap(1);
        size_type _Newoff = _Myoff + _Mysize;
        size_type _Block = _Newoff / _DEQUESIZ;
        if (_Mapsize <= _Block)
            _Block -= _Mapsize;
        if (_Map[_Block] == 0)
            _Map[_Block] = this->_Alval.allocate(_DEQUESIZ);
        this->_Alval.construct(_Map[_Block] + _Newoff % _DEQUESIZ, _Val);
        ++_Mysize;
    }
    bool empty() const { return _Mysize == 0; }
    _Ty& front();
    void pop_front();

protected:
    // !FAKE: c2 noreturn; declaration-only __declspec(noreturn) _Xlen; retail 0x0001D5C2 (named fold alias) is called out of line and _Growmap does not reload
    // eax after it; the VC 7.1 body (throw length_error) gives that but emits unnamed throw-info data (C0236)
    __declspec(noreturn) void _Xlen() const;
    void _Growmap(size_type _Count)
    {
        if (max_size() / _DEQUESIZ - _Mapsize < _Count)
            _Xlen();

        size_type _Inc = _Mapsize / 2;
        if (_Inc < _DEQUEMAPSIZ)
            _Inc = _DEQUEMAPSIZ;
        if (_Count < _Inc && _Mapsize <= max_size() / _DEQUESIZ - _Inc)
            _Count = _Inc;
        size_type _Myboff = _Myoff / _DEQUESIZ;
        _Mapptr _Newmap = this->_Almap.allocate(_Mapsize + _Count);
        _Mapptr _Myptr = _Newmap + _Myboff;

        _Myptr = _Uninitialized_copy(_Map + _Myboff, _Map + _Mapsize, _Myptr, this->_Almap);
        if (_Myboff <= _Count) {
            _Myptr = _Uninitialized_copy(_Map, _Map + _Myboff, _Myptr, this->_Almap);
            _Uninitialized_fill_n(_Myptr, _Count - _Myboff, (_Tptr)0, this->_Almap);
            _Uninitialized_fill_n(_Newmap, _Myboff, (_Tptr)0, this->_Almap);
        } else {
            _Uninitialized_copy(_Map, _Map + _Count, _Myptr, this->_Almap);
            _Myptr = _Uninitialized_copy(_Map + _Count, _Map + _Myboff, _Newmap, this->_Almap);
            _Uninitialized_fill_n(_Myptr, _Count, (_Tptr)0, this->_Almap);
        }

        _Destroy_range(_Map + _Myboff, _Map + _Mapsize, this->_Almap);
        if (_Map)
            this->_Almap.deallocate(_Map, _Mapsize);

        _Map = _Newmap;
        _Mapsize += _Count;
    }
    void _Tidy();

    allocator<_Tptr> _Almap;               // 0x00
    allocator<_Ty> _Alval;                 // 0x01
    _Mapptr _Map;                          // 0x04
    size_type _Mapsize;                    // 0x08
    size_type _Myoff;                      // 0x0C
    size_type _Mysize;                     // 0x10
};
}

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
std::string StripPath(const std::string& path);
}

class LoadScreen { // NAME: proto-pdb:default.pdb
public:
    void DisplayLoadScreen();
    bool LoadScreenFinished();

    char field_0[0x4];                 // 0x00
    float _startDisplayTime;           // 0x04
    float _minimumDisplayTime;         // 0x08
    char field_C[0x24];                // 0x0C
};
DECOMP_SIZE_ASSERT(LoadScreen, 0x30);

class SmartBase { // NAME: proto-pdb:default.pdb
public:
    SmartBase() : _RefCount(0) {}
protected:
    virtual ~SmartBase() {}            // vtable+0x00
public:
    void AddRef() { _RefCount++; }
    void SubRef()
    {
        if (--_RefCount == 0)
            delete this;
    }

    int _RefCount;                     // 0x04
};
DECOMP_SIZE_ASSERT(SmartBase, 0x8);

extern "C" void __stdcall XPhysicalProtect(void*, unsigned long, unsigned long); // NAME: proto-pdb:default.pdb

class HottieBundle; // NAME: proto-pdb:default.pdb
class ResourceCache { // NAME: proto-pdb:default.pdb
public:
    bool AllocateHunk(unsigned int size, void** ptr, unsigned long protectFlags);
    void FreeHunk(void* ptr);
    unsigned int GetLargestAvailableSpace() const;
};
struct HottieFile { // NAME: proto-pdb:default.pdb
    unsigned int locName;              // 0x00
    unsigned int sizeHeader;           // 0x04
    unsigned int offsetHeader;         // 0x08
    unsigned int sizeDataUncompressed; // 0x0C
    unsigned int sizeDataCompressed;   // 0x10
    unsigned int offsetData;           // 0x14
};
DECOMP_SIZE_ASSERT(HottieFile, 0x18);
class HottieBundle { // NAME: proto-pdb:default.pdb
public:
    const HottieFile* GetFileInfo(const std::string& name) const;

    unsigned int id;                   // 0x00
    unsigned int version;              // 0x04
    unsigned int sizeInfo;             // 0x08
    unsigned int sizeHeaders;          // 0x0C
    unsigned int sizeTotal;            // 0x10
    unsigned int offsetStrings;        // 0x14
    unsigned int numFiles;             // 0x18
};
DECOMP_SIZE_ASSERT(HottieBundle, 0x1C);
struct z_stream_s { // NAME: proto-pdb:default.pdb
    unsigned char* next_in;            // 0x00
    unsigned int avail_in;             // 0x04
    unsigned long total_in;            // 0x08
    unsigned char* next_out;           // 0x0C
    unsigned int avail_out;            // 0x10
    char field_14[0x38 - 0x14];        // 0x14
};
DECOMP_SIZE_ASSERT(z_stream_s, 0x38);
extern "C" int inflateInit_(z_stream_s* z, const char* version, int stream_size); // NAME: proto-pdb:default.pdb
extern "C" int inflate(z_stream_s*, int);    // NAME: proto-pdb:default.pdb
extern "C" int inflateEnd(z_stream_s*);      // NAME: proto-pdb:default.pdb
void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

class BundleWrapper : public SmartBase { // NAME: proto-pdb:default.pdb
public:
    BundleWrapper(unsigned int size, unsigned int flags);
    virtual ~BundleWrapper();
    HottieBundle* GetPtr() { return _bundle; }
    void SetProtection(unsigned long flags);
    static ResourceCache& GetMemoryPool();

    HottieBundle* _bundle;             // 0x08
    unsigned int _size;                // 0x0C
    unsigned int _flags;               // 0x10
};
DECOMP_SIZE_ASSERT(BundleWrapper, 0x14);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    Ptr() : _Object(0) {}
    Ptr(T* obj) : _Object(obj)
    {
        if (_Object)
            _Object->AddRef();
    }
    Ptr(const Ptr<T>& p) : _Object(p._Object)
    {
        if (_Object)
            _Object->AddRef();
    }
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    Ptr<T>& operator=(T* obj);
    Ptr<T>& operator=(const Ptr<T>& p); // retail 0x000BDC97 ??4?$Ptr@VBundleWrapper@@@@QAEAAV0@ABV0@@Z
    T* operator->() { return _Object; }
    bool IsValid() const { return _Object != 0; }

    T* _Object;                        // 0x00
};

template Ptr<BundleWrapper>& Ptr<BundleWrapper>::operator=(BundleWrapper* obj);

extern "C" void __stdcall Sleep(unsigned long); // NAME: proto-pdb:default.pdb

class FileHandler; // NAME: proto-pdb:default.pdb
class BeepFile { // NAME: proto-pdb:default.pdb
public:
    BeepFile();
    ~BeepFile();
    bool OpenForAsync(const std::string& filename);
    void Close();
    bool IsValid() const { return _fh != 0; }
    unsigned int Size();

    FileHandler* _fh;                  // 0x00
    char field_4[0x20 - 0x4];          // 0x04 (_filename, not reached)
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

class BundleBackgroundLoader : public SmartBase { // NAME: proto-pdb:default.pdb
public:
    BundleBackgroundLoader(const std::string& filename, unsigned int flags);
    virtual ~BundleBackgroundLoader();
    Ptr<BundleWrapper> GetBundlePtr();
    bool IsLoadingComplete(bool);

    Ptr<BundleWrapper> _bundle;        // 0x08
    BeepFile _file;                    // 0x0C
    unsigned int _statusRead;          // 0x2C
    unsigned int _statusWrite;         // 0x30
    unsigned int _currentReadLoc;      // 0x34
    unsigned int _currentReadSize;     // 0x38
    bool _summaryOnly;                 // 0x3C
    bool _mirrorFile;                  // 0x3D
    bool _topDownAllocation;           // 0x3E
    bool _headerLoaded;                // 0x3F
};
DECOMP_SIZE_ASSERT(BundleBackgroundLoader, 0x40);

class BeepTDF; // NAME: proto-pdb:default.pdb

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void DestroyAll();
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    typedef std::multimap<std::string, T*> NameMap;

    void CollectGarbage();
    void OutputContents() const;
    void DiscardBundles(bool);
    void AddBundle(const Ptr<BundleWrapper>&, bool);

    NameMap _Objects;                  // 0x00
};

// C0231: the verbatim body (SwampResourceManager.cpp); the calls' fold VA 0x000BE119 is that unit's matched row,
// so our byte-equal copies ride its hole as fold duplicates
template <class T> void ResourceManager<T>::OutputContents() const
{
    // !FAKE: c2 register allocation; dummy local; the PDB records only `it`, retail holds _Myhead in esi
    typename NameMap::const_iterator e = _Objects.end();
    for (typename NameMap::const_iterator it = _Objects.begin(); it != e; ++it) {
    }
}

class AnimationResource; // NAME: proto-pdb:default.pdb
class FireFlyResource;   // NAME: proto-pdb:default.pdb
class RumbleResource;    // NAME: proto-pdb:default.pdb
class MaterialResource;  // NAME: proto-pdb:default.pdb
class ModelResource;     // NAME: proto-pdb:default.pdb
class SoundResource;     // NAME: proto-pdb:default.pdb
class TextureResource;   // NAME: proto-pdb:default.pdb

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};
class FireFlyManager : public ResourceManager<FireFlyResource> { // NAME: proto-pdb:default.pdb
};
class RumbleManager : public ResourceManager<RumbleResource> { // NAME: proto-pdb:default.pdb
};
class MaterialManager : public ResourceManager<MaterialResource> { // NAME: proto-pdb:default.pdb
};
class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};
class SoundManager : public ResourceManager<SoundResource> { // NAME: proto-pdb:default.pdb
public:
    void CollectGarbage();
    void AddBundle(const Ptr<BundleWrapper>&, bool);
};
class TextureManager : public ResourceManager<TextureResource> { // NAME: proto-pdb:default.pdb
};
// !FAKE: c1xx front end; C0154 decl-only explicit specialization; the TextureResource copy is SwampResourceManager.cpp's matched
// row (same name, no fold merge), so the call stays external to it
template <> void ResourceManager<TextureResource>::OutputContents() const;

class DiskQueue { // NAME: proto-pdb:default.pdb
public:
    bool AddReadRequest(BeepFile* f, unsigned int offset, void* data, unsigned int bytes, unsigned int* resultLoc,
                        unsigned int priority);

    char field_0[0x7C];                // 0x00
};

// Retail layout: DiskQueue shrank 0x84 -> 0x7C, so _dvdQueue sits at 0x7C (proto 0x84).
class DiskArbitrator { // NAME: proto-pdb:default.pdb
public:
    DiskQueue& GetDVDQueue() { return _dvdQueue; }

    DiskQueue _hardDriveQueue;         // 0x00
    DiskQueue _dvdQueue;               // 0x7C (proto 0x84)
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    DiskArbitrator& GetDiskArbitrator() const { return *_diskArbitrator; }

    char field_0[0xC];                         // 0x00
    SwampScene* _scene;                        // 0x0C
    char field_10[0x4];                        // 0x10
    DiskArbitrator* _diskArbitrator;           // 0x14
    AnimationManager* _animationManager;       // 0x18
    FireFlyManager* _fireFlyManager;           // 0x1C
    RumbleManager* _rumbleManager;             // 0x20
    MaterialManager* _materialManager;         // 0x24
    ModelManager* _modelManager;               // 0x28
    char field_2C[0x8];                        // 0x2C
    SoundManager* _soundManager;               // 0x34
    TextureManager* _textureManager;           // 0x38
    char field_3C[0x68 - 0x3C];                // 0x3C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

BeepTDF* Swamp_LoadWorld(const std::string&, Ptr<BundleWrapper>, Ptr<BundleWrapper>); // NAME: proto-pdb:default.pdb
std::string GetLangCode(); // NAME: proto-pdb:default.pdb

class SwampLoader { // NAME: proto-pdb:default.pdb
public:
    struct Bundle { // NAME: proto-pdb:default.pdb
        // !FAKE: c1xx front end (implicit member emission); user-declared copy ctor; PDB param `__that` marks it compiler-generated, but with it removed cl emits none here (no copy in this unit): "match_one: REFUSED bytes: symbol ??0Bundle@SwampLoader@@QAE@ABU01@@Z not defined in SwampLoader.obj"
        Bundle(const Bundle&);
        Bundle() {}
        ~Bundle();

        std::string _filename;                 // 0x00
        Ptr<BundleBackgroundLoader> _loader;   // 0x1C
        double _startTime;                     // 0x20
        unsigned int _completionMarker;        // 0x28
        bool _dontDiscardFlag;                 // 0x2C
    };

    struct UnpackJob { // NAME: proto-pdb:default.pdb
        // !FAKE: c1xx front end (implicit member emission); user-declared copy ctor; as Bundle above (PDB param `__that`)
        UnpackJob(const UnpackJob&);
        UnpackJob() {}
        ~UnpackJob();

        Ptr<BundleWrapper> _sourceBundle;      // 0x00
        const HottieFile* _fileInfo;           // 0x04
        Ptr<BundleWrapper> _destBundle;        // 0x08
        z_stream_s* _zlibStream;               // 0x0C
        double _startTime;                     // 0x10
        unsigned int _completionMarker;        // 0x18
        bool _dontDiscardFlag;                 // 0x1C
    };

    friend BeepTDF* SwampLoader_LoadArea(const std::string& levelpath, const std::string& area, LoadScreen& loadScreen,
                                         bool newLevel);

protected:
    SwampLoader(const std::string& basepath, const std::string& area, bool loadEverything);
    bool Update();
    ~SwampLoader();
    BeepTDF* Finish();
    void UnpackALittle();
    void QueueUnpackJob(Ptr<BundleWrapper>& source, const HottieFile* fileInfo, Ptr<BundleWrapper>& dest,
                        unsigned int completionMarker, bool dontDiscardFlag);

public:
    std::deque<Bundle> _bundles;               // 0x00
    std::deque<UnpackJob> _unpackJobs;         // 0x14
    Ptr<BundleWrapper> _vertexBundle;          // 0x28
    Ptr<BundleWrapper> _worldDataBundle;       // 0x2C
    std::string _basePath;                     // 0x30
    bool _loadEverything;                      // 0x4C
};
DECOMP_SIZE_ASSERT(SwampLoader, 0x50);
DECOMP_SIZE_ASSERT(SwampLoader::Bundle, 0x30);
DECOMP_SIZE_ASSERT(SwampLoader::UnpackJob, 0x20);

// FUNCTION: USA 0x000DA6F8
// PROTOTYPE: PREVIEW 0x004C5C29 identical
// NAME: proto-pdb:default.pdb
bool LoadScreen::LoadScreenFinished()
{
    return BeepLib::GetTime() - _startDisplayTime > _minimumDisplayTime;
}

// FUNCTION: USA 0x000DA717
// PROTOTYPE: PREVIEW 0x004C5C48 identical
// NAME: proto-pdb:default.pdb
void BundleWrapper::SetProtection(unsigned long flags)
{
    XPhysicalProtect(_bundle, _size, flags);
}

// FUNCTION: USA 0x000DA765
// PROTOTYPE: PREVIEW 0x004C5C96 identical
// NAME: proto-pdb:default.pdb
// best: match_one objdiff 100.0, bytes match (4.3 T12.c21); was walled: provenance_lint nested operator
// (C0212) -- lifted 5.3 T2.c1 (C0369)
template SwampLoader::Bundle const & std::deque<SwampLoader::Bundle,std::allocator<SwampLoader::Bundle> >::const_iterator::operator*()const;

// FUNCTION: USA 0x000DA786
// PROTOTYPE: PREVIEW 0x004C5CB7 identical
// NAME: proto-pdb:default.pdb
// best: match_one objdiff 100.0, bytes match (4.3 T12.c21); was walled: provenance_lint nested operator
// (C0212) -- lifted 5.3 T2.c1 (C0369)
template SwampLoader::UnpackJob const & std::deque<SwampLoader::UnpackJob,std::allocator<SwampLoader::UnpackJob> >::const_iterator::operator*()const;

// FUNCTION: USA 0x000DA7A4
// NAME: proto-pdb:default.pdb
BundleWrapper::BundleWrapper(unsigned int size, unsigned int flags) : _bundle(0), _size(size), _flags(flags)
{
    // constants from retail disasm: PAGE_READWRITE 4, PAGE_READONLY 2, MEM_TOP_DOWN 0x100000
    if (flags & 4) {
        _bundle = 0;
    } else {
        // !FAKE: c2 instruction selection; byte cast; it takes retail's `mov dl, cl` (plain `flags & 1` reads the dword, match_one 99.72)
        unsigned long protect = (unsigned char)(flags & 1) ? 4 : 2;
        if (flags & 2)
            protect |= 0x100000;
        GetMemoryPool().AllocateHunk(size, (void**)&_bundle, protect);
    }
}

// FUNCTION: USA 0x000DA7FE
// PROTOTYPE: PREVIEW 0x004C5D2F identical
// NAME: proto-pdb:default.pdb
BundleWrapper::~BundleWrapper()
{
    if (!(_flags & 4))
        GetMemoryPool().FreeHunk(_bundle);
}

// FUNCTION: USA 0x000DA824
// PROTOTYPE: PREVIEW 0x004C5D55 identical
// NAME: proto-pdb:default.pdb
void SwampLoader_Cleanup(bool discardAll)
{
    g_swamp->_scene->DestroyAll();

    g_swamp->_fireFlyManager->CollectGarbage();
    g_swamp->_rumbleManager->CollectGarbage();
    g_swamp->_modelManager->CollectGarbage();
    g_swamp->_materialManager->CollectGarbage();
    g_swamp->_textureManager->CollectGarbage();
    g_swamp->_textureManager->CollectGarbage();
    g_swamp->_animationManager->CollectGarbage();
    g_swamp->_soundManager->CollectGarbage();

    g_swamp->_fireFlyManager->OutputContents();
    g_swamp->_rumbleManager->OutputContents();
    g_swamp->_modelManager->OutputContents();
    g_swamp->_materialManager->OutputContents();
    g_swamp->_animationManager->OutputContents();
    g_swamp->_textureManager->OutputContents();
    g_swamp->_soundManager->OutputContents();

    g_swamp->_fireFlyManager->DiscardBundles(discardAll);
    g_swamp->_rumbleManager->DiscardBundles(discardAll);
    g_swamp->_animationManager->DiscardBundles(discardAll);
    g_swamp->_modelManager->DiscardBundles(discardAll);
    g_swamp->_textureManager->DiscardBundles(discardAll);
    g_swamp->_soundManager->DiscardBundles(discardAll);
}

// FUNCTION: USA 0x000DA94F
// NAME: proto-pdb:default.pdb
template <class T> Ptr<T>& Ptr<T>::operator=(T* obj)
{
    if (_Object)
        _Object->SubRef();
    _Object = obj;
    if (_Object)
        _Object->AddRef();
    return *this;
}

// retail 0x000BDC97 (called out of line, home unit not this one); with this body and QueueUnpackJob's compiled,
// cl's same-unit summary lets SwampLoader::Update keep the new Ptr in esi across the call (Update 85.71 -> 100)
// NAME: proto-pdb:default.pdb
template <class T> Ptr<T>& Ptr<T>::operator=(const Ptr<T>& p)
{
    if (_Object)
        _Object->SubRef();
    _Object = p._Object;
    if (_Object)
        _Object->AddRef();
    return *this;
}
// FUNCTION: USA 0x000DA976
// PROTOTYPE: PREVIEW 0x004C5EA7 identical
// NAME: proto-pdb:default.pdb
SwampLoader::UnpackJob::UnpackJob(const UnpackJob& __that)
    : _sourceBundle(__that._sourceBundle), _fileInfo(__that._fileInfo), _destBundle(__that._destBundle),
      _zlibStream(__that._zlibStream), _startTime(__that._startTime), _completionMarker(__that._completionMarker),
      _dontDiscardFlag(__that._dontDiscardFlag)
{
}

// FUNCTION: USA 0x000DA9D1
// PROTOTYPE: PREVIEW 0x004C5F02 identical
// NAME: proto-pdb:default.pdb
SwampLoader::UnpackJob::~UnpackJob()
{
}

// FUNCTION: USA 0x000DA9F9
// PROTOTYPE: PREVIEW 0x004C5F2A identical
// NAME: proto-pdb:default.pdb
BeepTDF* SwampLoader::Finish()
{
    if (_worldDataBundle.IsValid() && _vertexBundle.IsValid())
        return Swamp_LoadWorld(_basePath, _worldDataBundle, _vertexBundle);
    return Swamp_LoadWorld(_basePath, Ptr<BundleWrapper>(), Ptr<BundleWrapper>());
}

// FUNCTION: USA 0x000DAA38
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; wall-vtable-relink: vtable 0x1EC4CC slot ??_G 0x000DAC88 (pinned) calls
// ??1BundleBackgroundLoader@@UAE@XZ at retail 0x000DAA98, unnamed in symbols.txt/symbols_pin.txt; relink refuses
// `external ??1BundleBackgroundLoader@@UAE@XZ does not resolve by name` (4.4 T4.c7). Lift: pin the dtor (callee-bytes).
BundleBackgroundLoader::BundleBackgroundLoader(const std::string& filename, unsigned int flags)
    : _statusRead(23), _statusWrite(23), _currentReadLoc(0), _currentReadSize(0), _summaryOnly((flags & 1) != 0),
      _mirrorFile((flags & 2) != 0), _topDownAllocation((flags & 4) != 0), _headerLoaded(false)
{
    _file.OpenForAsync(filename);
}
#endif

// FUNCTION: USA 0x000DAAFD
// NAME: proto-pdb:default.pdb
bool BundleBackgroundLoader::IsLoadingComplete(bool waitForMemoryToFreeUp)
{
    while (_file.IsValid()) {
        if (!_bundle.IsValid()) {
            // !FAKE: provenance; neutral stand-in locals v1/v2; the PDB records no local (match_one 100.0 -> 95.63 with v2 and p dropped)
            unsigned int v1 = 0;
            if (_topDownAllocation)
                v1 = 2;
            unsigned int v2;
            if (_summaryOnly) {
                v2 = sizeof(HottieBundle);
                v1 |= 2;
            } else {
                v2 = _file.Size();
            }
            _currentReadSize = (v2 + 0x7FF) & ~0x7FF;
            if (waitForMemoryToFreeUp && BundleWrapper::GetMemoryPool().GetLargestAvailableSpace() < _currentReadSize)
                return false;
            // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__
            _bundle = new("SwampLoader.cpp", 627) BundleWrapper(_currentReadSize, v1);
        }
        if (_statusRead == 1)
            return false;
        if (_statusRead != 2) {
            g_swamp->GetDiskArbitrator().GetDVDQueue().AddReadRequest(&_file, _currentReadLoc, _bundle->GetPtr(),
                                                                      _currentReadSize, &_statusRead, 13);
            return false;
        }
        _currentReadLoc += _currentReadSize;
        _currentReadSize = 0;
        _statusRead = 23;
        // !FAKE: provenance; neutral stand-in locals p/tmp; the PDB records no local (match_one 100.0 -> 99.03 with p dropped)
        HottieBundle* p = _bundle->GetPtr();
        if (p->id != 0x20544F48 || p->version != 1) {
            _bundle = 0;
            _file.Close();
            return true;
        }
        if (!_summaryOnly || _headerLoaded) {
            _file.Close();
            return true;
        }
        _headerLoaded = true;
        unsigned int tmp = (p->sizeHeaders + 0x7FF) & ~0x7FF;
        if (tmp == 0x800) {
            _file.Close();
            return true;
        }
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__
        _bundle = new("SwampLoader.cpp", 680) BundleWrapper(tmp, _topDownAllocation ? 2 : 0);
        _currentReadLoc = 0;
        _currentReadSize = tmp;
    }
    return true;
}

// FUNCTION: USA 0x000DACA4
// NAME: proto-pdb:default.pdb
Ptr<BundleWrapper> BundleBackgroundLoader::GetBundlePtr()
{
    while (!IsLoadingComplete(false))
        Sleep(0);
    return _bundle;
}

// FUNCTION: USA 0x000DAD01
// PROTOTYPE: PREVIEW 0x004C6238 identical
// NAME: proto-pdb:default.pdb
SwampLoader::Bundle::~Bundle()
{
}

// FUNCTION: USA 0x000DADC8
// PROTOTYPE: PREVIEW 0x004C62FF identical
// NAME: proto-pdb:default.pdb
SwampLoader::Bundle::Bundle(const Bundle& __that)
    : _filename(__that._filename), _loader(__that._loader), _startTime(__that._startTime),
      _completionMarker(__that._completionMarker), _dontDiscardFlag(__that._dontDiscardFlag)
{
}

// FUNCTION: USA 0x000DADFC
// PROTOTYPE: PREVIEW 0x004C6333 identical
// NAME: proto-pdb:default.pdb
void SwampLoader::UnpackALittle()
{
    if (!_unpackJobs.empty()) {
        // !FAKE: c2 register allocation; neutral stand-in local; retail keeps front() in esi (no PDB local)
        UnpackJob& v1 = _unpackJobs.front();
        if (v1._startTime == 0.0)
            v1._startTime = BeepLib::GetTime();
        // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand (as Sprite.cpp); no PDB local
        float v0 = (float)v1._fileInfo->sizeDataCompressed /
                   ((float)v1._fileInfo->sizeDataUncompressed * 2.8610227e-06f);
        unsigned int v2 = _mm_cvtt_ss2si(_mm_load_ss(&v0));
        if (v2 < 0x400)
            v2 = 0x400;
        unsigned int v3 = v1._fileInfo->sizeDataCompressed - v1._zlibStream->total_in;
        if (v3 < v2)
            v1._zlibStream->avail_in = v3;
        else
            v1._zlibStream->avail_in = v2;
        if (inflate(v1._zlibStream, 0) == 1) {
            inflateEnd(v1._zlibStream);
            BeepLib::GetTime();
            switch (v1._completionMarker) {
            case 8:
                _vertexBundle = v1._destBundle;
                break;
            case 9:
                _worldDataBundle = v1._destBundle;
                break;
            case 10:
                v1._destBundle->SetProtection(2);
                g_swamp->_modelManager->AddBundle(v1._destBundle, v1._dontDiscardFlag);
                break;
            case 11:
                v1._destBundle->SetProtection(2);
                g_swamp->_animationManager->AddBundle(v1._destBundle, v1._dontDiscardFlag);
                break;
            }
            delete v1._zlibStream;
            _unpackJobs.pop_front();
        }
    }
}

// FUNCTION: USA 0x000DAFDC
// PROTOTYPE: PREVIEW 0x004C6513 identical
// NAME: proto-pdb:default.pdb
template void std::deque<SwampLoader::Bundle,std::allocator<SwampLoader::Bundle> >::_Growmap(unsigned int);

// FUNCTION: USA 0x000DB0E1
// PROTOTYPE: PREVIEW 0x004C6618 identical
// NAME: proto-pdb:default.pdb
template void std::deque<SwampLoader::UnpackJob,std::allocator<SwampLoader::UnpackJob> >::_Growmap(unsigned int);

// FUNCTION: USA 0x000DB1E6
// PROTOTYPE: PREVIEW 0x004C671D identical
// NAME: proto-pdb:default.pdb
SwampLoader::~SwampLoader()
{
}

// FUNCTION: USA 0x000DB229
// PROTOTYPE: PREVIEW 0x004C6760 identical
// NAME: proto-pdb:default.pdb
template void std::deque<SwampLoader::Bundle,std::allocator<SwampLoader::Bundle> >::push_back(SwampLoader::Bundle const &);

// FUNCTION: USA 0x000DB283
// PROTOTYPE: PREVIEW 0x004C67BA identical
// NAME: proto-pdb:default.pdb
template void std::deque<SwampLoader::UnpackJob,std::allocator<SwampLoader::UnpackJob> >::push_back(SwampLoader::UnpackJob const &);

// FUNCTION: USA 0x000DB2DD
// NAME: proto-pdb:default.pdb
SwampLoader::SwampLoader(const std::string& basepath, const std::string& area, bool loadEverything)
    : _basePath(basepath), _loadEverything(loadEverything)
{
    std::string lang = GetLangCode();
    for (unsigned int pass = 0; pass < 2; ++pass) {
        Bundle b;
        std::string path = basepath;
        if (pass == 0) {
            if (!loadEverything)
                continue;
            path += "/Common/";
            b._dontDiscardFlag = true;
        } else {
            if (area.empty())
                path += "/";
            else
                path += "/Area_" + area + "/";
            b._dontDiscardFlag = false;
        }
        b._startTime = 0.0;
        // !FAKE: retail string literal; __FILE__/__LINE__ text and line numbers; the original passes its build's __FILE__/__LINE__
        b._filename = path + "world.hot";
        b._loader = new("SwampLoader.cpp", 225) BundleBackgroundLoader(b._filename, 6);
        b._completionMarker = 3;
        _bundles.push_back(b);
        b._filename = path + "ModelsAndAnims_" + lang + ".hot";
        b._loader = new("SwampLoader.cpp", 230) BundleBackgroundLoader(b._filename, 6);
        b._completionMarker = 4;
        _bundles.push_back(b);
        b._filename = path + "ModelsAndAnims.hot";
        b._loader = new("SwampLoader.cpp", 235) BundleBackgroundLoader(b._filename, 6);
        b._completionMarker = 4;
        _bundles.push_back(b);
        b._filename = path + "AnimatedTextures_" + lang + ".hot";
        b._loader = new("SwampLoader.cpp", 240) BundleBackgroundLoader(b._filename, 2);
        b._completionMarker = 2;
        _bundles.push_back(b);
        b._filename = path + "AnimatedTextures.hot";
        b._loader = new("SwampLoader.cpp", 245) BundleBackgroundLoader(b._filename, 2);
        b._completionMarker = 2;
        _bundles.push_back(b);
        b._filename = path + "particles.hot";
        b._loader = new("SwampLoader.cpp", 250) BundleBackgroundLoader(b._filename, 2);
        b._completionMarker = 6;
        _bundles.push_back(b);
        b._filename = path + "rumbles.hot";
        b._loader = new("SwampLoader.cpp", 255) BundleBackgroundLoader(b._filename, 2);
        b._completionMarker = 7;
        _bundles.push_back(b);
        b._filename = path + "sounds_" + lang + ".hot";
        b._loader = new("SwampLoader.cpp", 260) BundleBackgroundLoader(b._filename, 2);
        b._completionMarker = 5;
        _bundles.push_back(b);
        b._filename = path + "sounds.hot";
        b._loader = new("SwampLoader.cpp", 265) BundleBackgroundLoader(b._filename, 2);
        b._completionMarker = 5;
        _bundles.push_back(b);
        b._filename = path + "textures_" + lang + ".hot";
        b._loader = new("SwampLoader.cpp", 270) BundleBackgroundLoader(b._filename, 2);
        b._completionMarker = 1;
        _bundles.push_back(b);
        b._filename = path + "textures.hot";
        b._loader = new("SwampLoader.cpp", 275) BundleBackgroundLoader(b._filename, 2);
        b._completionMarker = 1;
        _bundles.push_back(b);
    }
}

// FUNCTION: USA 0x000DB9F4
// PROTOTYPE: PREVIEW 0x004C6F2B identical
// NAME: proto-pdb:default.pdb
void SwampLoader::QueueUnpackJob(Ptr<BundleWrapper>& source, const HottieFile* fileInfo, Ptr<BundleWrapper>& dest,
                                 unsigned int completionMarker, bool dontDiscardFlag)
{
    UnpackJob job;
    job._sourceBundle = source;
    job._fileInfo = fileInfo;
    job._destBundle = dest;
    // !FAKE: c2 register allocation; neutral stand-in local; retail keeps the new stream in esi across the calls (no PDB local)
    z_stream_s* p = new("SwampLoader.cpp", 434) z_stream_s;
    job._zlibStream = p;
    job._startTime = 0.0;
    job._completionMarker = completionMarker;
    job._dontDiscardFlag = dontDiscardFlag;
    memset(p, 0, sizeof(z_stream_s));
    inflateInit_(p, "1.1.3", sizeof(z_stream_s));
    p->next_in = (unsigned char*)source->GetPtr() + fileInfo->offsetData;
    p->next_out = (unsigned char*)dest->GetPtr();
    p->avail_out = fileInfo->sizeDataUncompressed;
    _unpackJobs.push_back(job);
}

// FUNCTION: USA 0x000DBAA0
// PROTOTYPE: PREVIEW 0x004C6FD7 identical
// NAME: proto-pdb:default.pdb
bool SwampLoader::Update()
{
    if (_bundles.empty() && _unpackJobs.empty())
        return true;
    UnpackALittle();
    if (!_bundles.empty()) {
        Bundle bundle = _bundles.front();
        if (bundle._startTime == 0.0)
            bundle._startTime = BeepLib::GetTime();
        if (bundle._loader->IsLoadingComplete(!_unpackJobs.empty())) {
            Ptr<BundleWrapper> bp = bundle._loader->GetBundlePtr();
            _bundles.pop_front();
            if (bp.IsValid()) {
                switch (bundle._completionMarker) {
                case 1:
                case 2:
                    g_swamp->_textureManager->AddBundle(bp, bundle._dontDiscardFlag);
                    break;
                case 3: {
                    Ptr<BundleWrapper> bp = bundle._loader->GetBundlePtr();
                    // !FAKE: c2 register allocation; neutral stand-in local; retail keeps the file info in ebx (no PDB local)
                    const HottieFile* v1 = bp->GetPtr()->GetFileInfo("verts.hot");
                    if (v1) {
                        Ptr<BundleWrapper> vertexBundle = new("SwampLoader.cpp", 346) BundleWrapper(v1->sizeDataUncompressed, 1);
                        QueueUnpackJob(bp, v1, vertexBundle, 8, bundle._dontDiscardFlag);
                    }
                    v1 = bp->GetPtr()->GetFileInfo("data.hot");
                    if (v1) {
                        Ptr<BundleWrapper> worldDataBundle = new("SwampLoader.cpp", 369) BundleWrapper(v1->sizeDataUncompressed, 1);
                        QueueUnpackJob(bp, v1, worldDataBundle, 9, bundle._dontDiscardFlag);
                    }
                    break;
                }
                case 4: {
                    Ptr<BundleWrapper> bp = bundle._loader->GetBundlePtr();
                    // !FAKE: c2 register allocation; neutral stand-in local; retail keeps the file info in ebx (no PDB local)
                    const HottieFile* v1 = bp->GetPtr()->GetFileInfo("models.hot");
                    if (v1) {
                        Ptr<BundleWrapper> models = new("SwampLoader.cpp", 388) BundleWrapper(v1->sizeDataUncompressed, 1);
                        QueueUnpackJob(bp, v1, models, 10, bundle._dontDiscardFlag);
                    }
                    v1 = bp->GetPtr()->GetFileInfo("anims.hot");
                    if (v1) {
                        Ptr<BundleWrapper> anims = new("SwampLoader.cpp", 399) BundleWrapper(v1->sizeDataUncompressed, 1);
                        QueueUnpackJob(bp, v1, anims, 11, bundle._dontDiscardFlag);
                    }
                    break;
                }
                case 5:
                    g_swamp->_soundManager->AddBundle(bp, bundle._dontDiscardFlag);
                    break;
                case 6:
                    g_swamp->_fireFlyManager->AddBundle(bp, bundle._dontDiscardFlag);
                    break;
                case 7:
                    g_swamp->_rumbleManager->AddBundle(bp, bundle._dontDiscardFlag);
                    break;
                }
            }
        }
    }
    return false;
}

// FUNCTION: USA 0x000DBE1F
// NAME: proto-pdb:default.pdb
BeepTDF* SwampLoader_LoadArea(const std::string& levelpath, const std::string& area, LoadScreen& loadScreen, bool newLevel)
{
    std::string levelName = BeepLib::StripPath(levelpath);
    BeepLib::GetTime();
    SwampLoader loader(levelpath, area, newLevel);
    do
        loadScreen.DisplayLoadScreen();
    while (!loader.Update() || !loadScreen.LoadScreenFinished());
    BeepLib::GetTime();
    return loader.Finish();
}
