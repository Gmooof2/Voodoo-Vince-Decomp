// UNIT: src/Libs/Swamp/Xbox/Official_Release/RumbleResource.cpp OBJ: Xbox\Official_Release\RumbleResource.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
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
    int GetIntRecord(const SwampString& id, int def) const;
    double GetFloatRecord(const SwampString& id, double def) const;
    bool GetBoolRecord(const SwampString& id, bool def) const;
    const BeepTDF* FindBlock(const SwampString& name, bool recurse) const;
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString StripPath(const SwampString& path);
}

class ResourceBase { // NAME: proto-pdb:default.pdb
protected:
    ResourceBase(const SwampString& name);

public:
    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class RumbleResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    RumbleResource(const SwampString& name);
    ~RumbleResource();
    unsigned int SamplesRequired() const;
    void GenerateSamples(unsigned short* samples, bool& looped) const;

private:
    void GenSamples(const BeepTDF* motor, unsigned short* samples, unsigned short count, int channel) const;

public:
    BeepTDF* _rumble;                  // 0x24
    float _duration;                   // 0x28
    unsigned int _rate;                // 0x2C
};
DECOMP_SIZE_ASSERT(RumbleResource, 0x30);

// FUNCTION: USA 0x000D1B0E
// PROTOTYPE: PREVIEW 0x004BCDC9 identical
// NAME: proto-pdb:default.pdb
RumbleResource::~RumbleResource()
{
    delete _rumble;
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
    BeepTDF* GetTDFInBundle(const SwampString& name);
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

class RumbleManager : public ResourceManager<RumbleResource> { // NAME: proto-pdb:default.pdb
private:
    RumbleManager();
};
DECOMP_SIZE_ASSERT(RumbleManager, 0x58);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    RumbleManager& GetRumbleManager() const { return *_rumbleManager; }

    char field_0[0x20];                // 0x00
    RumbleManager* _rumbleManager;     // 0x20
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000D1B35
// PROTOTYPE: PREVIEW 0x004BCDF0 identical
// NAME: proto-pdb:default.pdb
RumbleResource::RumbleResource(const SwampString& name) : ResourceBase(name), _rumble(0), _duration(0.0f), _rate(0)
{
    {
        SwampString shortname = BeepLib::StripPath(name);
        _rumble = g_swamp->GetRumbleManager().GetTDFInBundle(shortname);
    }
    if (_rumble) {
        int version = _rumble->GetIntRecord("Version", 0);
        if (version == 1) {
            _duration = _rumble->GetFloatRecord("Duration", 0.0);
            _rate = _rumble->GetIntRecord("SampleRate", 10);
            if (_rate != 0 && _duration != 0.0f)
                _valid = true;
        }
    }
}

// FUNCTION: USA 0x000D273A
// PROTOTYPE: PREVIEW 0x004BD9F5 identical
// NAME: proto-pdb:default.pdb
void RumbleResource::GenerateSamples(unsigned short* samples, bool& looped) const
{
    // !FAKE: c1xx; neutral stand-in local; the PDB records no local for the count retail keeps in ebx
    unsigned int v1 = SamplesRequired();
    looped = _rumble->GetBoolRecord("Looped", false);
    const BeepTDF* left = _rumble->FindBlock("LeftMotor", false);
    if (left) {
        GenSamples(left, samples, v1, 0);
    } else {
        // !FAKE: c2 loop strength reduction; stand-in pointer local; retail walks a pointer here, the index form keeps [ecx+eax*4]
        unsigned short* v3 = samples;
        for (unsigned int v2 = 0; v2 < v1; v2++) {
            *v3 = 0;
            v3 += 2;
        }
    }
    const BeepTDF* right = _rumble->FindBlock("RightMotor", false);
    if (right) {
        GenSamples(right, samples, v1, 1);
    } else {
        for (unsigned int v2 = 0; v2 < v1; v2++)
            samples[v2 * 2 + 1] = 0;
    }
}

// FUNCTION: USA 0x000D2821
// PROTOTYPE: PREVIEW 0x004BDADC identical
// NAME: proto-pdb:default.pdb
RumbleManager::RumbleManager()
{
    SetExtension(".rbl");
}
