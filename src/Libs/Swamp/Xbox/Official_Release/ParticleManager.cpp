// UNIT: src/Libs/Swamp/Xbox/Official_Release/ParticleManager.cpp OBJ: Xbox\Official_Release\ParticleManager.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

// !FAKE: c1xx front end; file-scope warning pragma; VC7.1 <string> without /EH warns C4530 and the build refuses it
#pragma warning(disable: 4530)
#include <xstring>

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0132, C0090); the string copy ctor stays visible (retail RemoveAll
// keeps the iterator in esi across it) and calls assign out of line (named VA), whose body would emit the _Copy
// __ehhandler$ funclet; declared after <xstring>, before <stdexcept> instantiates it (C2908 otherwise)
template <> std::basic_string<char, std::char_traits<char>, std::allocator<char> >&
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::assign(
    const std::basic_string<char, std::char_traits<char>, std::allocator<char> >&, unsigned int, unsigned int);
// !FAKE: c1xx front end; declaration-only (C0231 lever 1, C0235 same-name wall); Init.obj defines these as matched rows
template <> void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Eos(unsigned int);
template <> void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Tidy(bool, unsigned int);
template <> std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(
    const std::basic_string<char, std::char_traits<char>, std::allocator<char> >&);
// !FAKE: c1xx front end; declaration-only (C0154, C0237 named callee); Init.obj defines it as a matched row (0x000117DE);
// erase's out_of_range throw calls it out of line (retail ParticleManager.obj refs it); 5.2 T13.c1
template <> std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(const char*);
#endif

#include <string>
#include <vector>
#include <map>

class AABox;    // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
class Quaternion; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                   // vtable+0x0
    virtual const AABox& GetBounds() const;                      // vtable+0x4
    virtual Vector3 GetWorldPosition() const;                    // vtable+0x8
    virtual Quaternion GetWorldRotation() const;                 // vtable+0xC
    virtual void SetWorldPosition(const Vector3& pos);           // vtable+0x10
    virtual void Invalidate();                                   // vtable+0x14
    const Vector3& SetLocation(const Vector3&);
};

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void Play();

    char field_4[0xCE - 0x4];          // 0x04
    bool _deleteWhenDone;              // 0xCE
    char field_CF[0xE8 - 0xCF];        // 0xCF
};
DECOMP_SIZE_ASSERT(FireFlyNode, 0xE8);

struct FireFlyNodeCloneInfo { // NAME: proto-pdb:default.pdb
    // retail SetCloneEffect inlines it: 0, 0, -1.0f (0xBF800000)
    FireFlyNodeCloneInfo() : master(0), referenceCount(0), lastUpdateTime(-1.0f) {}

    FireFlyNode* master;               // 0x00
    unsigned int referenceCount;       // 0x04
    float lastUpdateTime;              // 0x08
};
DECOMP_SIZE_ASSERT(FireFlyNodeCloneInfo, 0xC);

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

// !FAKE: c1xx front end; LangInfo element type instantiation; retail folds pair<const string, FireFlyNodeCloneInfo>::pair with this one at 0x000B8077 and
// the delinked object names the VA by the LangInfo alias, so the row instantiates the PDB LangInfo element type
class FontManager { // NAME: proto-pdb:default.pdb
public:
    struct LangInfo { // NAME: proto-pdb:default.pdb
        LangInfo();

        Vector2 scaling;               // 0x00
        float verticalSpacing;         // 0x08
    };
};
DECOMP_SIZE_ASSERT(FontManager::LangInfo, 0xC);

class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    typedef std::map<std::string, FireFlyNode*> NameMap; // NAME: proto-pdb:default.pdb

    SwampParticleManager();
    ~SwampParticleManager();
    FireFlyNode* Find(const std::string& s);
    FireFlyNode* Add(const std::string&, const std::string&, unsigned int, bool);
    void SetCloneEffect(const std::string& effectName);
    FireFlyNode* AddOneShot(const std::string& type, const Vector3& loc);
    void Remove(FireFlyNode* node);
    void RemoveAll();
    void Autostart();

    bool _autostarted;                                            // 0x00
    NameMap _names;                                               // 0x04
    std::vector<FireFlyNode*> _autostart;                         // 0x10
    std::map<std::string, FireFlyNodeCloneInfo> _cloneInfo;       // 0x20
};
DECOMP_SIZE_ASSERT(SwampParticleManager, 0x2C);

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0154, C0215); retail calls these out of line (named VAs);
// their header bodies would emit _Buynode/_Copy __ehhandler$ funclets and _Xlen/_Xran RTTI .data here (gate objects
// extra; relink: data section with no VA by name)
typedef std::_Tree<std::_Tmap_traits<std::string, FireFlyNode*, std::less<std::string>,
                                     std::allocator<std::pair<const std::string, FireFlyNode*> >, false> >
    NameTree;
typedef std::_Tree<std::_Tmap_traits<std::string, FireFlyNodeCloneInfo, std::less<std::string>,
                                     std::allocator<std::pair<const std::string, FireFlyNodeCloneInfo> >, false> >
    CloneTree;
template <> void NameTree::_Init();
template <> void CloneTree::_Init();
template <> void NameTree::_Tidy();
template <> void CloneTree::_Tidy();
template <> void NameTree::clear();
// was: template <> NameTree::iterator NameTree::erase(NameTree::iterator); (decl-only). 5.2 T13.c1: the <xtree> body
// (in-class, so inline/pick-any) now emits this unit's FireFlyNode* alias of the fold 0x00011B38 (row: Init.cpp
// _Tree<string,void*>::erase); relink merges it byte-equal into the row (C0233 Fix 1, C0270)
template <> NameTree::iterator NameTree::find(const std::string&);
template <> CloneTree::_Pairib CloneTree::insert(const CloneTree::value_type&);
template <> void std::vector<FireFlyNode*>::_Tidy();
template <> std::vector<FireFlyNode*>::size_type std::vector<FireFlyNode*>::size() const;
template <> void std::vector<FireFlyNode*>::_Xlen() const;
// !FAKE: c1xx front end; declaration-only explicit specialization of copy; erase calls copy out of line (named fold VA 0x000B7E9C), its body would emit _Copy_opt/_Ptr_cat (no VA by name)
namespace std {
template <> FireFlyNode** copy(FireFlyNode**, FireFlyNode**, FireFlyNode**);
}
#endif

extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
std::string MakeLower(const std::string&);
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    std::string _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

struct FFlyData { // NAME: proto-pdb:default.pdb
    char field_0[0x50];                // 0x00
    unsigned int _renderEffects;       // 0x50
    char field_54[0x14C - 0x54];       // 0x54
};
DECOMP_SIZE_ASSERT(FFlyData, 0x14C);

struct FFlyBinEmitter { // NAME: proto-pdb:default.pdb
    char field_0[0x34];                // 0x00
    FFlyData data;                     // 0x34
};
DECOMP_SIZE_ASSERT(FFlyBinEmitter, 0x180);

struct FFlyBinFile { // NAME: proto-pdb:default.pdb
    const FFlyBinEmitter* GetEmitters() const { return (const FFlyBinEmitter*)((const char*)this + offsetEmitters); }

    char field_0[0x18];                // 0x00
    unsigned int numEmitters;          // 0x18
    char field_1C[0x24 - 0x1C];        // 0x1C
    unsigned int offsetEmitters;       // 0x24
    char field_28[0x30 - 0x28];        // 0x28
};
DECOMP_SIZE_ASSERT(FFlyBinFile, 0x30);

class BeepTDF; // NAME: proto-pdb:default.pdb

class FireFlyResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    // retail: no _bin null test before GetEmitters, so IsValid is the _bin/_tdf pair
    bool IsValid() const { return _bin || _tdf; }
    const FFlyBinFile* GetBin() const { return _bin; }

    const FFlyBinFile* _bin;           // 0x24
    BeepTDF* _tdf;                     // 0x28
};
DECOMP_SIZE_ASSERT(FireFlyResource, 0x2C);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle&);
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    bool IsValid() const { return _Resource != 0; }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const std::string& name);
};

class FireFlyManager : public ResourceManager<FireFlyResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x1C];                // 0x00
    FireFlyManager* _fireFlyManager;   // 0x1C
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000B7ED0
// PROTOTYPE: PREVIEW 0x004A299E identical
// NAME: proto-pdb:default.pdb
template std::vector<FireFlyNode *,std::allocator<FireFlyNode *> >::iterator std::vector<FireFlyNode *,std::allocator<FireFlyNode *> >::erase(std::vector<FireFlyNode *,std::allocator<FireFlyNode *> >::iterator);

// FUNCTION: USA 0x000B8007
// PROTOTYPE: PREVIEW 0x004A2AD5 identical
// NAME: proto-pdb:default.pdb
void SwampParticleManager::Autostart()
{
    // !FAKE: c1xx front end; neutral stand-in local; PDB Autostart records no local, std::for_each stays a call (43.0)
    for (std::vector<FireFlyNode*>::iterator v1 = _autostart.begin(); v1 != _autostart.end(); ++v1)
        (*v1)->Play();
    _autostart.clear();
    _autostarted = true;
}

// FUNCTION: USA 0x000B8077
// PROTOTYPE: PREVIEW 0x004A2C00 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo>::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo>(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,FontManager::LangInfo const &);

// FUNCTION: USA 0x000B8097
// PROTOTYPE: PREVIEW 0x004A2C40 identical
// NAME: proto-pdb:default.pdb
FireFlyNode* SwampParticleManager::Find(const std::string& s)
{
    NameMap::iterator it = _names.find(BeepLib::MakeLower(s));
    return it != _names.end() ? it->second : 0;
}

// FUNCTION: USA 0x000B84A9
// PROTOTYPE: PREVIEW 0x004A3298 identical
// NAME: proto-pdb:default.pdb
void SwampParticleManager::Remove(FireFlyNode* node)
{
    for (NameMap::iterator it = _names.begin(); it != _names.end(); ++it) {
        if (it->second == node) {
            _names.erase(it);
            if (node) {
                if (_autostart.size()) {
                    // !FAKE: c1xx front end; neutral stand-in local; PDB Remove records only node and it, retail walks _autostart by pointer
                    for (std::vector<FireFlyNode*>::iterator v1 = _autostart.begin(); v1 != _autostart.end(); ++v1) {
                        if (*v1 == node) {
                            _autostart.erase(v1);
                            break;
                        }
                    }
                }
                node->Invalidate();
            }
            return;
        }
    }
}

// FUNCTION: USA 0x000B88A5
// PROTOTYPE: PREVIEW 0x004A3694 identical
// NAME: proto-pdb:default.pdb
void SwampParticleManager::RemoveAll()
{
    for (NameMap::iterator it = _names.begin(); it != _names.end(); ++it) {
        std::string name = it->first;
        it->second->Invalidate();
    }
    _autostart.clear();
    _names.clear();
}

// FUNCTION: USA 0x000B8CC3
// PROTOTYPE: PREVIEW 0x004A3A9B identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.88, bytes mismatch (frame 0x5c vs 0x54: retail packs the insert result over the clone-info temp)
// 5.2 T12.c21 (at 20): pair/clone-info/insert-result split into two statements gives retail's slots but moves the
// MakeLower dtor before insert (95.36); pointer-to-temp, comma, block, ctor-body, decl-only pair ctor forms: no packing
void SwampParticleManager::SetCloneEffect(const std::string& effectName)
{
    ResourceHandle<FireFlyResource> ffly = g_swamp->_fireFlyManager->Load(effectName);
    if (!ffly.IsValid() || !ffly().IsValid())
        return;
    const FFlyBinFile* v1 = ffly().GetBin();
    const FFlyBinEmitter* v3 = v1->GetEmitters();
    for (unsigned int v2 = 0; v2 < v1->numEmitters; v2++) {
        if (v3[v2].data._renderEffects & 0x200) // retail constant
            return;
    }
    _cloneInfo.insert(std::pair<const std::string, FireFlyNodeCloneInfo>(BeepLib::MakeLower(effectName), FireFlyNodeCloneInfo()));
}
#endif

// FUNCTION: USA 0x000B8DFC
// PROTOTYPE: PREVIEW 0x004A3BD4 identical
// NAME: proto-pdb:default.pdb
FireFlyNode* SwampParticleManager::AddOneShot(const std::string& type, const Vector3& loc)
{
    FireFlyNode* v1 = Add(type, type, 0, true);
    if (v1) {
        v1->_deleteWhenDone = true;
        v1->SetLocation(loc);
    }
    return v1;
}

// FUNCTION: USA 0x000B8E2C
// PROTOTYPE: PREVIEW 0x004A3C04 identical
// NAME: proto-pdb:default.pdb
SwampParticleManager::~SwampParticleManager()
{
    RemoveAll();
    g_particleManager = 0;
}

// FUNCTION: USA 0x000B8E54
// PROTOTYPE: PREVIEW 0x004A3C2C identical
// NAME: proto-pdb:default.pdb
SwampParticleManager::SwampParticleManager()
    : _autostarted(false)
{
    g_particleManager = this;
}
