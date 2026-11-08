// UNIT: src/Libs/Swamp/Xbox/Official_Release/swampfireflyresource.cpp OBJ: Xbox\Official_Release\SwampFireFlyResource.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};

template <class T, class A = allocator<T> > class vector {
public:
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
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
    struct _Node;

protected:
    _Tree() { _Init(); }
    void _Init();

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
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    ~BeepTDF();
};

struct FFlyBinFile { // NAME: proto-pdb:default.pdb
    unsigned int id;                   // 0x00
    unsigned int version;              // 0x04
    unsigned int dataSize;             // 0x08
    char field_C[0x30 - 0xC];          // 0x0C
};
DECOMP_SIZE_ASSERT(FFlyBinFile, 0x30);

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString StripPath(const SwampString&);
}

struct HottieRawData { // NAME: proto-pdb:default.pdb
    void* header;                      // 0x00
    void* data;                        // 0x04
    unsigned int size;                 // 0x08
};
DECOMP_SIZE_ASSERT(HottieRawData, 0xC);

class ResourceBase { // NAME: proto-pdb:default.pdb
protected:
    ResourceBase(const SwampString&);

public:
    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class FireFlyResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    FireFlyResource(const SwampString& name);
    ~FireFlyResource();

    const FFlyBinFile* _bin;           // 0x24
    BeepTDF* _tdf;                     // 0x28
};
DECOMP_SIZE_ASSERT(FireFlyResource, 0x2C);

// FUNCTION: USA 0x000D1A00
// PROTOTYPE: PREVIEW 0x004BCCBB identical
// NAME: proto-pdb:default.pdb
FireFlyResource::~FireFlyResource()
{
    delete _tdf;
}

class BundleWrapper;

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    T* _Object;                        // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    typedef std::multimap<SwampString, T*> NameMap;
    struct ResourceBundle { // NAME: proto-pdb:default.pdb
        Ptr<BundleWrapper> _bundle;    // 0x00
        bool _dontDiscard;             // 0x04
    };

    ResourceManager();
    HottieRawData GetDataInBundle(const SwampString&);
    void SetExtension(const SwampString& ext) { _Extension = ext; }

    NameMap _Objects;                              // 0x00
    SwampString _basePath;                         // 0x0C
    SwampString _Extension;                        // 0x28
    std::vector<ResourceBundle> _bundles;          // 0x44
    bool _missingResourceMessages;                 // 0x54
};

// C0231: the verbatim body (SwampResourceManager.cpp); the call's fold VA 0x000C1028 is that unit's matched row,
// so our byte-equal copy rides its hole as a fold duplicate
template <class T> ResourceManager<T>::ResourceManager() : _missingResourceMessages(true)
{
}

class FFlyManager { // NAME: proto-pdb:default.pdb
public:
    FFlyManager();

    char field_0[0x8];                 // 0x00
};
DECOMP_SIZE_ASSERT(FFlyManager, 0x8);

class FireFlyManager : public ResourceManager<FireFlyResource> { // NAME: proto-pdb:default.pdb
private:
    FireFlyManager();

    FFlyManager _ffmanager;            // 0x58
};
DECOMP_SIZE_ASSERT(FireFlyManager, 0x60);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x1C];                // 0x00
    FireFlyManager* _fireFlyManager;   // 0x1C
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000D1A27
// PROTOTYPE: PREVIEW 0x004BCCE2 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 96.49, bytes mismatch (retail threads the second _bin test: zero store jumps to the end, last header compare je to _valid)
// T12.c23 at-stop 20: retail = failure-first if/else with 0 CSE'd in edi; C0244 redundant-test if/else gives the layout
// but no edi (81.06: 5 zero uses stay `and [m],0`); else-if chains repeating `_bin = 0` get edi by tail merge but the
// merged block keeps the first instance's place (m2/n1 95.96, n5 91.49); repeating the dataSize test (94.47) CSEs 0x14C
FireFlyResource::FireFlyResource(const SwampString& name) : ResourceBase(name), _bin(0), _tdf(0)
{
    // !FAKE: c1xx front end; block scope; retail destroys shortname before the header check (match_one 61.8 -> 78.3)
    {
        SwampString shortname = BeepLib::StripPath(name);
        HottieRawData rawData = g_swamp->_fireFlyManager->GetDataInBundle(shortname);
        _bin = (const FFlyBinFile*)rawData.data;
    }
    if (_bin) {
        if (_bin->id != 0x594C4646 || _bin->version != 3 || _bin->dataSize != 0x14C) // retail constants
            _bin = 0;
        if (_bin)
            _valid = true;
    }
}
#endif

// FUNCTION: USA 0x000D1AA4
// PROTOTYPE: PREVIEW 0x004BCD5F identical
// NAME: proto-pdb:default.pdb
FireFlyManager::FireFlyManager()
{
    SetExtension(".fly");
}
