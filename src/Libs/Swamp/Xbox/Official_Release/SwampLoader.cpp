// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampLoader.cpp OBJ: Xbox\Official_Release\SwampLoader.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template<class _Elem>
struct char_traits {};

template<class _Ty>
class allocator {};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true, 0); }

protected:
    void _Tidy(bool _Built, unsigned int _Newsize);

public:
    char _pad00[0x1C];
};
typedef basic_string<char> string;

template<class _Ty, class _Ax = allocator<_Ty> >
class deque {
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
}

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
}

class LoadScreen { // NAME: proto-pdb:default.pdb
public:
    bool LoadScreenFinished();

    char field_0[0x4];                 // 0x00
    float _startDisplayTime;           // 0x04
    float _minimumDisplayTime;         // 0x08
    char field_C[0x24];                // 0x0C
};
DECOMP_SIZE_ASSERT(LoadScreen, 0x30);

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

class BundleBackgroundLoader : public SmartBase { // NAME: proto-pdb:default.pdb
public:
    char field_8[0x38];                // 0x08
};
DECOMP_SIZE_ASSERT(BundleBackgroundLoader, 0x40);

extern "C" void __stdcall XPhysicalProtect(void*, unsigned long, unsigned long); // NAME: proto-pdb:default.pdb

class HottieBundle; // NAME: proto-pdb:default.pdb
class ResourceCache { // NAME: proto-pdb:default.pdb
public:
    void FreeHunk(void* ptr);
};
struct HottieFile;  // NAME: proto-pdb:default.pdb
struct z_stream_s;  // NAME: proto-pdb:default.pdb

class BundleWrapper : public SmartBase { // NAME: proto-pdb:default.pdb
public:
    virtual ~BundleWrapper();
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
    bool IsValid() const { return _Object != 0; }

    T* _Object;                        // 0x00
};

template Ptr<BundleWrapper>& Ptr<BundleWrapper>::operator=(BundleWrapper* obj);

class BeepTDF; // NAME: proto-pdb:default.pdb
BeepTDF* Swamp_LoadWorld(const std::string&, Ptr<BundleWrapper>, Ptr<BundleWrapper>); // NAME: proto-pdb:default.pdb

class SwampLoader { // NAME: proto-pdb:default.pdb
public:
    struct Bundle { // NAME: proto-pdb:default.pdb
        // !FAKE: front end (implicit member emission); user-declared copy ctor; PDB param `__that` marks it compiler-generated, but with it removed cl emits none here (no copy in this unit): "match_one: REFUSED bytes: symbol ??0Bundle@SwampLoader@@QAE@ABU01@@Z not defined in SwampLoader.obj"
        Bundle(const Bundle&);
        ~Bundle();

        std::string _filename;                 // 0x00
        Ptr<BundleBackgroundLoader> _loader;   // 0x1C
        double _startTime;                     // 0x20
        unsigned int _completionMarker;        // 0x28
        bool _dontDiscardFlag;                 // 0x2C
    };

    struct UnpackJob { // NAME: proto-pdb:default.pdb
        // !FAKE: front end (implicit member emission); user-declared copy ctor, as Bundle above (PDB param `__that`)
        UnpackJob(const UnpackJob&);
        ~UnpackJob();

        Ptr<BundleWrapper> _sourceBundle;      // 0x00
        const HottieFile* _fileInfo;           // 0x04
        Ptr<BundleWrapper> _destBundle;        // 0x08
        z_stream_s* _zlibStream;               // 0x0C
        double _startTime;                     // 0x10
        unsigned int _completionMarker;        // 0x18
        bool _dontDiscardFlag;                 // 0x1C
    };

protected:
    ~SwampLoader();
    BeepTDF* Finish();

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

// FUNCTION: USA 0x000DA7FE
// PROTOTYPE: PREVIEW 0x004C5D2F identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match; gate relink fails
BundleWrapper::~BundleWrapper()
{
    if (!(_flags & 4))
        GetMemoryPool().FreeHunk(_bundle);
}
#endif

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

// FUNCTION: USA 0x000DB1E6
// PROTOTYPE: PREVIEW 0x004C671D identical
// NAME: proto-pdb:default.pdb
SwampLoader::~SwampLoader()
{
}
