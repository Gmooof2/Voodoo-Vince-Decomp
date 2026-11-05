// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampResourceManager.cpp OBJ: Xbox\Official_Release\SwampResourceManager.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template<class _Elem>
struct char_traits {};

template<class _Ty>
class allocator {};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    class const_iterator { // NAME: proto-pdb:default.pdb
    public:
        const_iterator(const _Elem* _Ptr) { _Myptr = _Ptr; } // src proto-pdb:default.pdb const_iterator
        const _Elem* _Myptr;
    };
    class iterator : public const_iterator { // NAME: proto-pdb:default.pdb
    public:
        iterator(_Elem* _Ptr) : const_iterator(_Ptr) {}
    };

    basic_string() { _Myres = _BUF_SIZE - 1; _Eos(0); }
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true, 0); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& operator+=(_Elem _Ch) { return (append((unsigned int)1, _Ch)); }
    basic_string& append(unsigned int _Count, _Elem _Ch); // src proto-pdb:default.pdb append
    iterator begin() { return (iterator(_Myptr())); } // src proto-pdb:default.pdb begin
    // decl only: 0x0003559F is BeepLib's matched const end; BeepFile emits this fold alias (C0244)
    iterator end();
    unsigned int find(const _Elem* _Ptr, unsigned int _Off, unsigned int _Count) const;
    unsigned int find(_Elem _Ch, unsigned int _Off = 0) const { return (find((const _Elem*)&_Ch, _Off, 1)); }
    unsigned int size() const { return (_Mysize); }
    static const unsigned int npos = (unsigned int)(-1);

    enum { _BUF_SIZE = 16 / sizeof(_Elem) < 1 ? 1 : 16 / sizeof(_Elem) };

protected:
    void _Tidy(bool _Built, unsigned int _Newsize);
    void _Eos(unsigned int _Newsize);

    _Ax _Alval;         // 0x00
    union _Bxty { // src proto-pdb:default.pdb _Bxty
        _Elem _Buf[_BUF_SIZE];
        _Elem* _Ptr;
    } _Bx;              // 0x04

public:
    _Elem* _Myptr() { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }

    unsigned int _Mysize; // 0x14
    unsigned int _Myres;  // 0x18
};
typedef basic_string<char> string;
template<class _Elem, class _Traits, class _Alloc>
basic_string<_Elem, _Traits, _Alloc> __cdecl operator+(const basic_string<_Elem, _Traits, _Alloc>& _Left,
                                                       const basic_string<_Elem, _Traits, _Alloc>& _Right);
template<class _FwdIt, class _Ty>
void replace(_FwdIt _First, _FwdIt _Last, const _Ty& _Oldval, const _Ty& _Newval); // src proto-pdb:default.pdb replace
template<class _Elem, class _Traits, class _Alloc> inline
basic_string<_Elem, _Traits, _Alloc> __cdecl operator+(const basic_string<_Elem, _Traits, _Alloc>& _Left, const _Elem _Right) // src proto-pdb:default.pdb operator+
{
    return (basic_string<_Elem, _Traits, _Alloc>(_Left) += _Right);
}

template <class T, class A = allocator<T> > class vector {
public:
    class const_iterator { // NAME: proto-pdb:default.pdb
    public:
        const_iterator(T* _Ptr) : _Myptr(_Ptr) {}
        const T& operator*() const { return *_Myptr; }
        const T* operator->() const { return &**this; }
        const_iterator& operator++() { ++_Myptr; return *this; }
        bool operator==(const const_iterator& _Right) const { return _Myptr == _Right._Myptr; }
        bool operator!=(const const_iterator& _Right) const { return !(*this == _Right); }
        T* _Myptr;
    };
    class iterator : public const_iterator { // NAME: proto-pdb:default.pdb
    public:
        iterator(T* _Ptr) : const_iterator(_Ptr) {}
        T& operator*() const { return (T&)**(const_iterator*)this; }
        T* operator->() const { return &**this; }
        iterator& operator++() { ++this->_Myptr; return *this; }
    };

    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
    iterator begin() { return iterator(_Myfirst); }
    iterator end() { return iterator(_Mylast); }
    unsigned int capacity() const { return (_Myfirst == 0 ? 0 : _Myend - _Myfirst); }
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }

private:
    A _Alval;                          // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class T> struct less {};
template <class T1, class T2> struct pair {
    pair(const T1& _Val1, const T2& _Val2) : first(_Val1), second(_Val2) {}

    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
};
template <class K, class Pr, class A, bool Mfl> class _Tset_traits {
public:
    typedef K value_type;
};
template <class Tr> class _Tree_nod {
public:
    // FUNCTION: USA 0x000BE279
    // NAME: proto-pdb:default.pdb
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
        char _Color;                   // 0x28
        char _Isnil;                   // 0x29
    };
};
template <class Tr> class _Tree : public _Tree_nod<Tr> {
    typedef typename _Tree_nod<Tr>::_Node _Node;
public:
    typedef typename Tr::value_type value_type;
    class const_iterator {
    public:
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const_iterator& operator++() { _Inc(); return *this; }
        bool operator!=(const const_iterator& _Right) const { return _Ptr != _Right._Ptr; }
        bool operator==(const const_iterator& _Right) const { return _Ptr == _Right._Ptr; }
        const value_type& operator*() const { return _Ptr->_Myval; }
        const value_type* operator->() const { return &**this; }
        void _Inc();
        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
    };
    const_iterator begin() const { return const_iterator(_Myhead->_Left); }
    const_iterator end() const { return const_iterator(_Myhead); }
    pair<iterator, bool> insert(const value_type& _Val);
    const_iterator find(const typename Tr::key_type& _Keyval) const;

protected:
    _Tree() { _Init(); }
    void _Init();
    void _Rrotate(_Node* _Wherenode);

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class multimap
    : public _Tree<_Tmap_traits<K, T, Pr, A, true> > {
public:
    multimap() {}
};
}

class ResourceCache { // NAME: proto-pdb:default.pdb
public:
    struct Block { // NAME: proto-pdb:default.pdb
        void** _resource;                  // 0x00
        unsigned int _numBlocks;           // 0x04
        unsigned int _nextBlock;           // 0x08
        unsigned int _previousBlock;       // 0x0C
    };

    ResourceCache(unsigned int size, unsigned int blockSize, bool lockAllocations);
    bool AllocateHunk(unsigned int size, void** ptr, unsigned long protectFlags);
    void FreeHunk(void* ptr);
    unsigned int GetLargestAvailableSpace() const;

protected:
    unsigned int FreeHunk(unsigned int block);
    void RemoveHunkFromFreeList(unsigned int block);

public:
    unsigned int _totalSize;           // 0x00
    unsigned int _blockSize;           // 0x04
    unsigned int _numBlocks;           // 0x08
    unsigned int _inUseHead;           // 0x0C
    unsigned int _inUseTail;           // 0x10
    unsigned int _freeHead;            // 0x14
    unsigned int _freeTail;            // 0x18
    Block* _blocks;                    // 0x1C
    void* _memory;                     // 0x20
    bool _lockAllocations;             // 0x24
};
DECOMP_SIZE_ASSERT(ResourceCache::Block, 0x10);
DECOMP_SIZE_ASSERT(ResourceCache, 0x28);

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    ~BeepFile();

    char field_0[0x20];                // 0x00
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    BeepTDF(BeepFile& f, const std::string& name);

    char field_0[0x58];                // 0x00
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);

void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb
void* operator new[](unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb
extern "C" void* __stdcall XPhysicalAlloc(unsigned long, unsigned long, unsigned long, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall XPhysicalProtect(void*, unsigned long, unsigned long); // NAME: proto-pdb:default.pdb

struct HottieFile { // NAME: proto-pdb:default.pdb
    unsigned int locName;              // 0x00
    unsigned int sizeHeader;           // 0x04
    unsigned int offsetHeader;         // 0x08
    unsigned int sizeDataUncompressed; // 0x0C
    unsigned int sizeDataCompressed;   // 0x10
    unsigned int offsetData;           // 0x14
};
DECOMP_SIZE_ASSERT(HottieFile, 0x18);

struct HottieRawData { // NAME: proto-pdb:default.pdb
    void MakeNull() { header = 0; data = 0; size = 0; }

    void* header;                      // 0x00
    void* data;                        // 0x04
    unsigned int size;                 // 0x08
};
DECOMP_SIZE_ASSERT(HottieRawData, 0xC);

class HottieBundle { // NAME: proto-pdb:default.pdb
public:
    const HottieFile* GetFileInfo(const std::string& name) const;
    HottieRawData GetRawFileData(const HottieFile* hf) const;
    BeepFile* OpenBundledFile(const std::string& name) const;
    void CloseBundledFile(BeepFile* f) const;

    unsigned int id;                   // 0x00
    unsigned int version;              // 0x04
    unsigned int sizeInfo;             // 0x08
    unsigned int sizeHeaders;          // 0x0C
    unsigned int sizeTotal;            // 0x10
    unsigned int offsetStrings;        // 0x14
    unsigned int numFiles;             // 0x18
};
DECOMP_SIZE_ASSERT(HottieBundle, 0x1C);

class SmartBase { // NAME: proto-pdb:default.pdb
public:
    virtual ~SmartBase();              // vtable+0x00
    void AddRef() { _RefCount++; }
    void SubRef()
    {
        if (--_RefCount == 0)
            delete this;
    }

    int _RefCount;                     // 0x04
};
DECOMP_SIZE_ASSERT(SmartBase, 0x8);

class BundleWrapper : public SmartBase { // NAME: proto-pdb:default.pdb
public:
    virtual ~BundleWrapper();
    HottieBundle* GetPtr() { return _bundle; }

    HottieBundle* _bundle;            // 0x08
    unsigned int _size;                // 0x0C
    unsigned int _flags;               // 0x10
};
DECOMP_SIZE_ASSERT(BundleWrapper, 0x14);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    Ptr<T>& operator=(const Ptr<T>& p);
    T* operator->() { return _Object; }

    T* _Object;                        // 0x00
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _valid; }
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    std::string _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

template <class T> class ResourceManager;

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    friend class ResourceManager<TextureResource>;

protected:
    TextureResource(const std::string& name);
    ~TextureResource();

public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

class RumbleResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    RumbleResource(const std::string& name);
    ~RumbleResource();
    bool IsValid() const { return _rumble != 0; }

    BeepTDF* _rumble;                  // 0x24
    float _duration;                   // 0x28
    unsigned int _rate;                // 0x2C
};
DECOMP_SIZE_ASSERT(RumbleResource, 0x30);

struct tWAVEFORMATEX;

class SoundResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    SoundResource(const std::string& name);
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

class PixelShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    friend class ResourceManager<PixelShaderResource>;

protected:
    PixelShaderResource(const std::string& name);

public:
    const unsigned long* _shaderCode;  // 0x24
};
DECOMP_SIZE_ASSERT(PixelShaderResource, 0x28);

struct GatorAnimationFile;

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    AnimationResource(const std::string& name);
    ~AnimationResource();
    bool IsValid() const { return _anim != 0; }

    const GatorAnimationFile* _anim;   // 0x24
    void* _myAnim;                     // 0x28
    float _length;                     // 0x2C
    int _triggers;                     // 0x30
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

struct FFlyBinFile;

class FireFlyResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    FireFlyResource(const std::string& name);
    ~FireFlyResource();
    bool IsValid() const { return _bin != 0 || _tdf != 0; }

    const FFlyBinFile* _bin;           // 0x24
    BeepTDF* _tdf;                     // 0x28
};
DECOMP_SIZE_ASSERT(FireFlyResource, 0x2C);

class GatorFile;

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    ModelResource(const std::string& name);
    ~ModelResource();
    bool IsValid() const { return _gator != 0; }

    const GatorFile* _gator;           // 0x24
    char field_28[0x60 - 0x28];        // 0x28
};
DECOMP_SIZE_ASSERT(ModelResource, 0x60);

struct _D3DVERTEXATTRIBUTEFORMAT;

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    friend class ResourceManager<VertexShaderResource>;

protected:
    VertexShaderResource(const std::string& name, const _D3DVERTEXATTRIBUTEFORMAT* decl = 0);

public:
    ~VertexShaderResource();

    unsigned long _handle;                           // 0x24
    const unsigned long* _shaderCode;                  // 0x28
    const _D3DVERTEXATTRIBUTEFORMAT* _declaration;     // 0x2C
};
DECOMP_SIZE_ASSERT(VertexShaderResource, 0x30);

template <class T> class ResourceHandle;

namespace BeepLib { // NAME: proto-pdb:default.pdb
std::string AddExtension(const std::string& s, const std::string& ext);
std::string MakeLower(const std::string& s);
}

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    typedef std::multimap<std::string, T*> NameMap;
    struct ResourceBundle { // NAME: proto-pdb:default.pdb
        Ptr<BundleWrapper> _bundle;    // 0x00
        bool _dontDiscard;             // 0x04
    };

    ResourceManager();
    void OutputContents() const;
    HottieRawData GetDataInBundle(const std::string& filename);
    HottieRawData GetDataInBundle(const std::string& filename, Ptr<BundleWrapper>& bundleptr);
    BeepTDF* GetTDFInBundle(const std::string& filename);
    ResourceHandle<T> Load(const std::string& name);

protected:
    T* Find(const std::string& name) const;
    void Add(T* obj);

public:
    NameMap _Objects;                             // 0x00
    std::string _basePath;                         // 0x0C
    std::string _Extension;                        // 0x28
    std::vector<ResourceBundle> _bundles;          // 0x44
    bool _missingResourceMessages;                 // 0x54
};
DECOMP_SIZE_ASSERT(ResourceManager<VertexShaderResource>, 0x58);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(T* r) : _Resource(r) { _Resource->AddRef(); }
    ResourceHandle(const ResourceHandle& h);
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

class MaterialResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    friend class ResourceManager<MaterialResource>;

    MaterialResource(const std::string& name, const BeepTDF* tdf = 0);

public:
    ~MaterialResource();

    bool _uvScrolling;                                 // 0x24
    bool _autoGenEMap;                                 // 0x25
    ResourceHandle<TextureResource> _baseTex;          // 0x28
    ResourceHandle<TextureResource> _detailTex;        // 0x2C
    ResourceHandle<TextureResource> _envmapTex;        // 0x30
    ResourceHandle<TextureResource> _reflectmapTex;    // 0x34
    ResourceHandle<TextureResource> _bumpmapTex;       // 0x38
    ResourceHandle<TextureResource> _lightmapTex;      // 0x3C
    unsigned int _detailType;                          // 0x40
    float _reflectivity;                               // 0x44
    Vector2 _uvRate1;                                  // 0x48
    Vector2 _uvRate2;                                  // 0x50
};
DECOMP_SIZE_ASSERT(MaterialResource, 0x58);

// FUNCTION: USA 0x000BD955
// PROTOTYPE: PREVIEW 0x004A86AC identical
// NAME: proto-pdb:default.pdb
HottieRawData HottieBundle::GetRawFileData(const HottieFile* hf) const
{
    HottieRawData hrd;
    if (hf) {
        hrd.data = (char*)this + hf->offsetData;
        hrd.size = hf->sizeDataUncompressed;
        if (hf->sizeHeader)
            hrd.header = (char*)this + hf->offsetHeader;
        else
            hrd.header = hrd.data;
    } else {
        hrd.MakeNull();
    }
    return hrd;
}

// FUNCTION: USA 0x000BD9A2
// PROTOTYPE: PREVIEW 0x004A86F9 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.47369, bytes mismatch (loop stores +4/+8/+C: SIB base/index swapped vs retail)
ResourceCache::ResourceCache(unsigned int size, unsigned int blockSize, bool lockAllocations)
    : _totalSize(size), _blockSize(blockSize), _numBlocks(size / blockSize), _inUseHead(0xFFFFFFFF),
      _inUseTail(0xFFFFFFFF), _freeHead(0), _freeTail(0), _lockAllocations(lockAllocations)
{
    _memory = XPhysicalAlloc(size, (unsigned long)-1, 0, 2);
    _blocks = new("SwampResourceManager.cpp", 36) Block[_numBlocks];
    for (unsigned int i = 0; i < _numBlocks; i++) {
        _blocks[i]._resource = 0;
        _blocks[i]._numBlocks = _numBlocks;
        _blocks[i]._nextBlock = 0xFFFFFFFF;
        _blocks[i]._previousBlock = 0xFFFFFFFF;
    }
}
#endif

// FUNCTION: USA 0x000BDA3A
// PROTOTYPE: PREVIEW 0x004A8791 identical
// NAME: proto-pdb:default.pdb
void ResourceCache::RemoveHunkFromFreeList(unsigned int block)
{
    // !FAKE: c2 register allocation; dummy local; retail holds &_blocks[block] in eax across the body
    Block* p = &_blocks[block];
    if (p->_nextBlock == 0xFFFFFFFF)
        _freeTail = p->_previousBlock;
    else
        _blocks[p->_nextBlock]._previousBlock = p->_previousBlock;
    if (p->_previousBlock == 0xFFFFFFFF)
        _freeHead = p->_nextBlock;
    else
        _blocks[p->_previousBlock]._nextBlock = p->_nextBlock;
    p->_previousBlock = 0xFFFFFFFF;
    p->_nextBlock = 0xFFFFFFFF;
}

// FUNCTION: USA 0x000BDA8C
// PROTOTYPE: PREVIEW 0x004A87E3 identical
// NAME: proto-pdb:default.pdb
unsigned int ResourceCache::GetLargestAvailableSpace() const
{
    // !FAKE: c2 register allocation; dummy local; the PDB records no locals for this proc
    unsigned int v1 = 0;
    // !FAKE: c2 register allocation; dummy local; the PDB records no locals for this proc
    for (unsigned int v2 = _freeHead; v2 != 0xFFFFFFFF; v2 = _blocks[v2]._nextBlock) {
        if (v1 < _blocks[v2]._numBlocks)
            v1 = _blocks[v2]._numBlocks;
    }
    return v1 * _blockSize;
}

// FUNCTION: USA 0x000BDAFE
// PROTOTYPE: PREVIEW 0x004A8811 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<Vector2,std::allocator<Vector2> >::capacity()const;

// FUNCTION: USA 0x000BDB11
// PROTOTYPE: PREVIEW 0x004A8824 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // wall-fold-va-callers (C0233): match_one 100, but 0x000BDB11 folds 9 vector<T>::size aliases; SwampGatorNode.obj calls vector<TextureOverrides::Override>::size as an external (4.3 T12)
template unsigned int std::vector<Vector2,std::allocator<Vector2> >::size()const;
#endif

// FUNCTION: USA 0x000BDB24
// PROTOTYPE: PREVIEW 0x004A8837 identical
// NAME: proto-pdb:default.pdb
void HottieBundle::CloseBundledFile(BeepFile* f) const
{
    delete f;
}

// FUNCTION: USA 0x000BDB3F
// PROTOTYPE: PREVIEW 0x004A8852 identical
// NAME: proto-pdb:default.pdb
unsigned int ResourceCache::FreeHunk(unsigned int block)
{
    // !FAKE: c2 register allocation; dummy local; retail holds &_blocks[block] in esi across the body
    Block* v1 = &_blocks[block];
    XPhysicalProtect((char*)_memory + block * _blockSize, v1->_numBlocks * _blockSize, 0x202);
    *v1->_resource = 0;
    v1->_resource = 0;
    if (v1->_nextBlock == 0xFFFFFFFF)
        _inUseTail = v1->_previousBlock;
    else
        _blocks[v1->_nextBlock]._previousBlock = v1->_previousBlock;
    if (v1->_previousBlock == 0xFFFFFFFF)
        _inUseHead = v1->_nextBlock;
    else
        _blocks[v1->_previousBlock]._nextBlock = v1->_nextBlock;
    v1->_nextBlock = 0xFFFFFFFF;
    v1->_previousBlock = 0xFFFFFFFF;
    // !FAKE: c2 register allocation; dummy local; retail keeps the merged flag in cl, the PDB records no slot
    bool v2 = false;
    if (block > 0) {
        // !FAKE: c2 register allocation; dummy local; previous hunk index in edx
        unsigned int v3 = block - v1[-1]._numBlocks;
        // !FAKE: c2 register allocation; dummy local; previous hunk in eax
        Block* v4 = &_blocks[v3];
        if (v4->_resource == 0) {
            // !FAKE: c2 register allocation; dummy local; merged count in ecx
            unsigned int v5 = v4->_numBlocks + v1->_numBlocks;
            for (unsigned int i = 0; i < v5; i++)
                v4[i]._numBlocks = v5;
            v1 = v4;
            block = v3;
            v2 = true;
        }
    }
    if (v1->_numBlocks + block < _numBlocks) {
        Block* joe = &v1[v1->_numBlocks];
        if (joe->_resource == 0) {
            if (!v2) {
                v1->_nextBlock = joe->_nextBlock;
                v1->_previousBlock = joe->_previousBlock;
                joe->_nextBlock = 0xFFFFFFFF;
                joe->_previousBlock = 0xFFFFFFFF;
                if (v1->_nextBlock == 0xFFFFFFFF)
                    _freeTail = block;
                else
                    _blocks[v1->_nextBlock]._previousBlock = block;
                if (v1->_previousBlock == 0xFFFFFFFF)
                    _freeHead = block;
                else
                    _blocks[v1->_previousBlock]._nextBlock = block;
            } else {
                RemoveHunkFromFreeList(v1->_numBlocks + block);
            }
            // !FAKE: c2 register allocation; dummy local; merged count in eax
            unsigned int v6 = joe->_numBlocks + v1->_numBlocks;
            for (unsigned int i = 0; i < v6; i++)
                v1[i]._numBlocks = v6;
            v2 = true;
        }
    }
    if (!v2) {
        if (_freeTail == 0xFFFFFFFF) {
            _freeHead = block;
        } else {
            v1->_previousBlock = _freeTail;
            _blocks[_freeTail]._nextBlock = block;
        }
        _freeTail = block;
    }
    return block;
}

// FUNCTION: USA 0x000BDC97
// PROTOTYPE: PREVIEW 0x004A89AA identical
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
template Ptr<BundleWrapper>& Ptr<BundleWrapper>::operator=(const Ptr<BundleWrapper>& p);

// FUNCTION: USA 0x000BDD40
// PROTOTYPE: PREVIEW 0x004A8A95 identical
// NAME: proto-pdb:default.pdb
bool ResourceCache::AllocateHunk(unsigned int size, void** ptr, unsigned long protectFlags)
{
    // !FAKE: c2 register allocation; dummy local; blocks needed in ebx, the PDB records no slot
    unsigned int v1 = (size + _blockSize - 1) / _blockSize;
    unsigned int best = 0xFFFFFFFF;
    if (protectFlags & 0x100000) {
        // !FAKE: c2 register allocation; dummy local; free-list walk index in edx
        for (unsigned int v2 = _freeTail; v2 != 0xFFFFFFFF; v2 = _blocks[v2]._previousBlock) {
            if (_blocks[v2]._numBlocks >= v1) {
                best = v2;
                break;
            }
        }
    } else {
        // !FAKE: c2 register allocation; dummy local; free-list walk index in ecx
        for (unsigned int v2 = _freeHead; v2 != 0xFFFFFFFF; v2 = _blocks[v2]._nextBlock) {
            if (_blocks[v2]._numBlocks >= v1) {
                best = v2;
                break;
            }
        }
    }
    if (best == 0xFFFFFFFF) {
        if (_lockAllocations)
            return false;
        for (;;) {
            // !FAKE: c2 register allocation; dummy local; freed hunk index stays in eax until the fit test passes
            unsigned int v7 = FreeHunk(_inUseTail);
            if (_blocks[v7]._numBlocks >= v1) {
                best = v7;
                break;
            }
        }
    }
    // !FAKE: c2 register allocation; dummy local; retail holds &_blocks[best] in edi to the end
    Block* v3 = &_blocks[best];
    if (v3->_numBlocks > v1) {
        // !FAKE: c2 register allocation; dummy local; split remainder in ecx
        unsigned int v4 = v3->_numBlocks - v1;
        if (protectFlags & 0x100000) {
            for (unsigned int i = 0; i < v4; i++)
                v3[i]._numBlocks = v4;
            best += v4;
            v3 += v4;
            for (unsigned int i = 0; i < v1; i++)
                v3[i]._numBlocks = v1;
        } else {
            for (unsigned int i = 0; i < v1; i++)
                v3[i]._numBlocks = v1;
            // !FAKE: c2 register allocation; dummy local; split-off free hunk index in ebx
            unsigned int v5 = v1 + best;
            // !FAKE: c2 register allocation; dummy local; split-off free hunk in eax
            Block* v6 = &_blocks[v5];
            for (unsigned int i = 0; i < v4; i++)
                v6[i]._numBlocks = v4;
            v6->_previousBlock = v3->_previousBlock;
            if (v6->_previousBlock == 0xFFFFFFFF)
                _freeHead = v5;
            else
                _blocks[v6->_previousBlock]._nextBlock = v5;
            v6->_nextBlock = v3->_nextBlock;
            if (v6->_nextBlock == 0xFFFFFFFF)
                _freeTail = v5;
            else
                _blocks[v6->_nextBlock]._previousBlock = v5;
        }
    } else {
        RemoveHunkFromFreeList(best);
    }
    if (_inUseHead == 0xFFFFFFFF) {
        v3->_previousBlock = 0xFFFFFFFF;
        v3->_nextBlock = 0xFFFFFFFF;
        _inUseTail = best;
        _inUseHead = best;
    } else {
        v3->_previousBlock = 0xFFFFFFFF;
        v3->_nextBlock = _inUseHead;
        _inUseHead = best;
        _blocks[v3->_nextBlock]._previousBlock = best;
    }
    *ptr = (char*)_memory + best * _blockSize;
    v3->_resource = ptr;
    XPhysicalProtect(*ptr, v3->_numBlocks * _blockSize, protectFlags & ~0x100000);
    return true;
}

// FUNCTION: USA 0x000BDF1A
// PROTOTYPE: PREVIEW 0x004A8C6F identical
// NAME: proto-pdb:default.pdb
void ResourceCache::FreeHunk(void* ptr)
{
    if (ptr)
        FreeHunk(((char*)ptr - (char*)_memory) / _blockSize);
}

// FUNCTION: USA 0x000BE048
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // wall-fold-va-callers (C0233): match_one 100, but SwampAnimationResource.obj calls the AnimationResource alias of 0x000BE048 as an external; our copy is no hole (unreachable from our rows) and links as extra .text (4.3 T13.c26)
template <class T> HottieRawData ResourceManager<T>::GetDataInBundle(const std::string& filename)
{
    for (typename std::vector<ResourceBundle>::iterator it = _bundles.begin(); it != _bundles.end(); ++it) {
        // !FAKE: c2 register allocation; neutral stand-in local; retail keeps the bundle in edi (no PDB local)
        HottieBundle* tmp = it->_bundle->GetPtr();
        const HottieFile* info = tmp->GetFileInfo(filename);
        if (info)
            return tmp->GetRawFileData(info);
    }
    HottieRawData result;
    result.MakeNull();
    return result;
}
template HottieRawData ResourceManager<TextureResource>::GetDataInBundle(const std::string& filename);
// !FAKE: c1xx front end; explicit instantiation; SwampAnimationResource.obj, swampfireflyresource.obj and
// SwampModelResource.obj call these fold aliases of 0x000BE048 as externals: our byte-equal copies ride the row
// hole as fold duplicates (C0233 Fix 1)
template HottieRawData ResourceManager<AnimationResource>::GetDataInBundle(const std::string& filename);
template HottieRawData ResourceManager<FireFlyResource>::GetDataInBundle(const std::string& filename);
template HottieRawData ResourceManager<ModelResource>::GetDataInBundle(const std::string& filename);
#endif

// FUNCTION: USA 0x000BE0A0
// NAME: proto-pdb:default.pdb
template <class T> BeepTDF* ResourceManager<T>::GetTDFInBundle(const std::string& filename)
{
    for (typename std::vector<ResourceBundle>::iterator it = _bundles.begin(); it != _bundles.end(); ++it) {
        if (it->_bundle->GetPtr()->GetFileInfo(filename)) {
            BeepFile* file = it->_bundle->GetPtr()->OpenBundledFile(filename);
            if (file) {
                BeepTDF* tdf = new("SwampResourceManager.cpp", 616) BeepTDF(*file, filename);
                it->_bundle->GetPtr()->CloseBundledFile(file);
                return tdf;
            }
        }
    }
    return 0;
}
template BeepTDF* ResourceManager<RumbleResource>::GetTDFInBundle(const std::string& filename);

// FUNCTION: USA 0x000BE119
// NAME: proto-pdb:default.pdb
template <class T> void ResourceManager<T>::OutputContents() const
{
    // !FAKE: c2 register allocation; dummy local; the PDB records only `it`, retail holds _Myhead in esi
    typename NameMap::const_iterator e = _Objects.end();
    for (typename NameMap::const_iterator it = _Objects.begin(); it != e; ++it) {
    }
}
template void ResourceManager<TextureResource>::OutputContents() const;

// FUNCTION: USA 0x000BE13A
// NAME: proto-pdb:default.pdb
template <class T>
HottieRawData ResourceManager<T>::GetDataInBundle(const std::string& filename, Ptr<BundleWrapper>& bundleptr)
{
    for (typename std::vector<ResourceBundle>::iterator it = _bundles.begin(); it != _bundles.end(); ++it) {
        // !FAKE: c2 register allocation; neutral stand-in local; retail keeps the bundle in edi (no PDB local)
        HottieBundle* tmp = it->_bundle->GetPtr();
        const HottieFile* info = tmp->GetFileInfo(filename);
        if (info) {
            bundleptr = it->_bundle;
            return tmp->GetRawFileData(info);
        }
    }
    HottieRawData result;
    result.MakeNull();
    return result;
}
template HottieRawData ResourceManager<SoundResource>::GetDataInBundle(const std::string& filename,
                                                                      Ptr<BundleWrapper>& bundleptr);

// FUNCTION: USA 0x000BE1C5
// PROTOTYPE: PREVIEW 0x004A8F1A identical
// NAME: proto-pdb:default.pdb
MaterialResource::~MaterialResource()
{
}

// FUNCTION: USA 0x000BE33E
// NAME: proto-pdb:default.pdb
template <class T> T* ResourceManager<T>::Find(const std::string& name) const
{
    typename NameMap::const_iterator it = _Objects.find(name);
    if (it != _Objects.end())
        return it->second;
    return 0;
}
template VertexShaderResource* ResourceManager<VertexShaderResource>::Find(const std::string& name) const;

// FUNCTION: USA 0x000BE397
// PROTOTYPE: PREVIEW 0x004A90B1 identical
// NAME: proto-pdb:default.pdb
// !FAKE: deduced-argument form of the gen_stl line (provenance_lint reads the spaced return type as the name)
template std::string std::operator+(const std::string&, const char);

// FUNCTION: USA 0x000C0299
// NAME: proto-pdb:default.pdb
template <class T> void ResourceManager<T>::Add(T* obj)
{
    _Objects.insert(typename NameMap::value_type(obj->_Name, obj));
}
template void ResourceManager<VertexShaderResource>::Add(VertexShaderResource* obj);

// FUNCTION: USA 0x000C02D3
// NAME: proto-pdb:default.pdb
template <class T> ResourceHandle<T> ResourceManager<T>::Load(const std::string& name)
{
    std::string path;
    if (name.find(':') == std::string::npos)
        path = _basePath + name;
    else
        path = name;
    if (_Extension.size())
        path = BeepLib::AddExtension(path, _Extension);
    path = BeepLib::MakeLower(path);
    std::replace(path.begin(), path.end(), '\\', '/');
    T* p = Find(path);
    if (!p) {
        p = new("SwampResourceManager.cpp", 539) T(path);
        if (!p->IsValid()) {
            delete p;
            return ResourceHandle<T>();
        }
        Add(p);
    }
    return ResourceHandle<T>(p);
}
template ResourceHandle<AnimationResource> ResourceManager<AnimationResource>::Load(const std::string& name);

// FUNCTION: USA 0x000C044E
// NAME: proto-pdb:default.pdb
template ResourceHandle<FireFlyResource> ResourceManager<FireFlyResource>::Load(const std::string& name);

// FUNCTION: USA 0x000C05CE
// NAME: proto-pdb:default.pdb
template ResourceHandle<RumbleResource> ResourceManager<RumbleResource>::Load(const std::string& name);

// FUNCTION: USA 0x000C0749
// NAME: proto-pdb:default.pdb
template ResourceHandle<MaterialResource> ResourceManager<MaterialResource>::Load(const std::string& name);

// FUNCTION: USA 0x000C08C5
// NAME: proto-pdb:default.pdb
template ResourceHandle<ModelResource> ResourceManager<ModelResource>::Load(const std::string& name);

// FUNCTION: USA 0x000C0A40
// NAME: proto-pdb:default.pdb
template ResourceHandle<PixelShaderResource> ResourceManager<PixelShaderResource>::Load(const std::string& name);

// FUNCTION: USA 0x000C0BB6
// NAME: proto-pdb:default.pdb
template ResourceHandle<SoundResource> ResourceManager<SoundResource>::Load(const std::string& name);

// FUNCTION: USA 0x000C0D31
// NAME: proto-pdb:default.pdb
template ResourceHandle<TextureResource> ResourceManager<TextureResource>::Load(const std::string& name);

// FUNCTION: USA 0x000C0EAC
// NAME: proto-pdb:default.pdb
template ResourceHandle<VertexShaderResource> ResourceManager<VertexShaderResource>::Load(const std::string& name);

// FUNCTION: USA 0x000C1028
// NAME: proto-pdb:default.pdb
template <class T> ResourceManager<T>::ResourceManager() : _missingResourceMessages(true)
{
}
template ResourceManager<VertexShaderResource>::ResourceManager();

// !FAKE: c1xx front end; scaffold delete; retail emits the set<string> _Tree_nod::_Node scalar deleting dtor
// (0x000BE279) in this unit (no matched function deletes one); this delete-expression emits it instead
// (matched_set [[emitter]] row)
void pin_emit_TreeNod_set_string_delete(
    std::_Tree_nod<std::_Tset_traits<std::string, std::less<std::string>, std::allocator<std::string>, false> >::_Node* p)
{
    delete p;
}

namespace std {
template <class Tr> void _Tree<Tr>::_Rrotate(_Node* _Wherenode)
{
    _Node* _Pnode = _Wherenode->_Left;
    _Wherenode->_Left = _Pnode->_Right;

    if (!_Pnode->_Right->_Isnil)
        _Pnode->_Right->_Parent = _Wherenode;
    _Pnode->_Parent = _Wherenode->_Parent;

    if (_Wherenode == _Myhead->_Parent)
        _Myhead->_Parent = _Pnode;
    else if (_Wherenode == _Wherenode->_Parent->_Right)
        _Wherenode->_Parent->_Right = _Pnode;
    else
        _Wherenode->_Parent->_Left = _Pnode;

    _Pnode->_Right = _Wherenode;
    _Wherenode->_Parent = _Pnode;
}
}

// FUNCTION: USA 0x000BDABA
// PROTOTYPE: PREVIEW 0x004F4BA5 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::_Rrotate(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::_Node *);
