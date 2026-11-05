// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampMaterialResource.cpp OBJ: Xbox\Official_Release\SwampMaterialResource.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const basic_string& _Right);
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true, 0); }
    const E* c_str() const { return _Myptr(); }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }
    unsigned int find(const E* _Ptr, unsigned int _Off, unsigned int _Count) const;
    unsigned int find(E _Ch, unsigned int _Off = 0) const { return find((const E*)&_Ch, _Off, 1); }

    enum { npos = -1 };

protected:
    void _Tidy(bool _Built, unsigned int _Newsize);

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
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;

    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; }
#ifdef NON_MATCHING
    // for the guarded FixupEnvmaps draft only: a visible ~vector makes every vector member's implicit dtor call
    // _Tidy, which has no name for vector<ResourceBundle> (relink refuses; 4.3 T16)
    vector(const vector& _Right);
    ~vector() { _Tidy(); }
#endif
    iterator begin() { return _Myfirst; }
    iterator end() { return _Mylast; }
    void push_back(const T& _Val);
    void clear() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class T1, class T2> struct pair {
    pair(const T1& _Val1, const T2& _Val2) : first(_Val1), second(_Val2) {}

    T1 first;                          // 0x00
    T2 second;
};
template <class T> struct less;
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
};
template <class Traits> class _Tree {
public:
    typedef typename Traits::key_type key_type;
    typedef typename Traits::value_type value_type;

private:
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        value_type _Myval;             // 0x0C
        char _Color;
        char _Isnil;
    };

public:
    class iterator {
    public:
        iterator() {}
        iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        value_type& operator*() const { return _Ptr->_Myval; }
        bool operator==(const iterator& _Right) const { return _Ptr == _Right._Ptr; }
        bool operator!=(const iterator& _Right) const { return !(*this == _Right); }

        _Node* _Ptr;
    };
    _Tree() { _Init(); }
    iterator begin() { return iterator(_Myhead->_Left); }
    iterator end() { return iterator(_Myhead); }
    pair<iterator, bool> insert(const value_type& _Val);
    iterator erase(iterator _Where);
    iterator find(const key_type& _Keyval);
    void clear();

protected:
    void _Init();

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > >
class multimap : public _Tree<_Tmap_traits<K, T, Pr, A, true> > {
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString MakeLower(const SwampString& s);
}

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

struct Vector2 { // NAME: proto-pdb:default.pdb
    Vector2(float u_, float v_) : u(u_), v(v_) {}

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AABox { // NAME: proto-pdb:default.pdb
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct AACylinder { // NAME: proto-pdb:default.pdb
    char field_0[0x14];                // 0x00
};
DECOMP_SIZE_ASSERT(AACylinder, 0x14);

struct GatorMaterial { // NAME: proto-pdb:default.pdb
    unsigned short base;               // 0x00
    unsigned short detail;             // 0x02
    unsigned short bumpmap;            // 0x04
    unsigned short envmap;             // 0x06
    unsigned short reflectmap;         // 0x08
    unsigned short lightmap;           // 0x0A
    unsigned int blend;                // 0x0C
    float reflectivity;                // 0x10
    float URate1;                      // 0x14
    float VRate1;                      // 0x18
    float URate2;                      // 0x1C
    float VRate2;                      // 0x20
    unsigned int reserved[3];          // 0x24
};
DECOMP_SIZE_ASSERT(GatorMaterial, 0x30);

struct GatorFile { // NAME: proto-pdb:default.pdb
    const GatorMaterial* GetMaterials() const { return (const GatorMaterial*)((const char*)this + offsetMaterials); }
    const char* GetString(unsigned int i) const
    {
        return (const char*)this + offsetStrings + ((const unsigned int*)((const char*)this + offsetStringTable))[i];
    }

    unsigned int id;                   // 0x00
    unsigned int version;              // 0x04
    unsigned int maxBoneBlend;         // 0x08
    bool usesExVerts;                  // 0x0C
    bool usesBoneTables;               // 0x0D
    bool usesPreLitVerts;              // 0x0E
    bool reserved1;                    // 0x0F
    unsigned int numVertices;          // 0x10
    unsigned int numIndices;           // 0x14
    unsigned int numStrips;            // 0x18
    unsigned int numMaterials;         // 0x1C
    unsigned int numBones;             // 0x20
    unsigned short numRenderBones;     // 0x24
    unsigned short numLocatorBones;    // 0x26
    unsigned short numParticleBones;   // 0x28
    unsigned short numReferences;      // 0x2A
    unsigned short numBoneTables;      // 0x2C
    unsigned short numStrings;         // 0x2E
    unsigned int offsetVertices;       // 0x30
    unsigned int offsetIndices;        // 0x34
    unsigned int offsetStrips;         // 0x38
    unsigned int offsetMaterials;      // 0x3C
    unsigned int offsetReferences;     // 0x40
    unsigned int offsetBones;          // 0x44
    unsigned int offsetBoneTables;     // 0x48
    unsigned int offsetStringTable;    // 0x4C
    unsigned int offsetStrings;        // 0x50
    AABox bounds;                      // 0x54
    AACylinder bc;                     // 0x6C
};
DECOMP_SIZE_ASSERT(GatorFile, 0x80);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetBlockName() const { return _blockname; }
    const std::vector<BeepTDF*>& GetBlocks() const { return _blocklist; }
    const BeepTDF* FindBlock(const SwampString&, bool) const;
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
    int GetIntRecord(const SwampString& id, int def) const;

    SwampString _blockname;            // 0x00
    char field_1C[0x3C - 0x1C];        // 0x1C
    std::vector<BeepTDF*> _blocklist;  // 0x3C
    char field_4C[0x58 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

protected:
    ResourceBase(const SwampString& name);

public:
    SwampString _Name;                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(T* r) : _Resource(r) { _Resource->AddRef(); }
    ResourceHandle(const ResourceHandle& h); // decl only: 0x00028D58 is another unit's row (C0244)
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }
    ResourceHandle& operator=(const ResourceHandle& h);
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};
template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

class MaterialManager;
class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

class MaterialResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    friend class MaterialManager;

private:
    MaterialResource();
    MaterialResource(const SwampString& name, const BeepTDF* definition);
    MaterialResource(const SwampString& name, const GatorFile& gator, const GatorMaterial& mtl);

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
    ResourceHandle<T> Load(const SwampString& name);

protected:
    T* Find(const SwampString& name) const;
    void Add(T* obj);

private:
    NameMap _Objects;                              // 0x00
    SwampString _basePath;                         // 0x0C
    SwampString _Extension;                        // 0x28
    std::vector<ResourceBundle> _bundles;          // 0x44
    bool _missingResourceMessages;                 // 0x54
};

// C0231: the verbatim bodies (SwampResourceManager.cpp); the calls' fold VAs 0x000C1028 and 0x000C0299 are that
// unit's matched rows, so our byte-equal copies ride their holes as fold duplicates
template <class T> ResourceManager<T>::ResourceManager() : _missingResourceMessages(true)
{
}
template <class T> void ResourceManager<T>::Add(T* obj)
{
    _Objects.insert(typename NameMap::value_type(obj->_Name, obj));
}

class MaterialManager : public ResourceManager<MaterialResource> { // NAME: proto-pdb:default.pdb
public:
    struct FixupInfo { // NAME: proto-pdb:default.pdb
        ResourceHandle<TextureResource>* texPtr; // 0x00
        Vector3 loc;                   // 0x04
        unsigned int size;             // 0x10
    };

    void AddEnvmapFixup(const SwampString& envname, MaterialResource* mat);
    void AddEnvmapFixup(const Vector3& loc, unsigned int size, ResourceHandle<TextureResource>* texPtr);
    ResourceHandle<MaterialResource> CreateMaterial(const BeepTDF* definition);
    ResourceHandle<MaterialResource> CreateMaterial(const SwampString& GatorName, const GatorFile& gator, unsigned int matid);
    void FixupEnvmaps(const BeepTDF* indexfile);
    void ClearFixupEnvmaps();

private:
    MaterialManager();

    ResourceHandle<MaterialResource> _defaultMaterial;           // 0x58
    ResourceHandle<TextureResource> _defaultEMap;                // 0x5C
    std::multimap<SwampString, MaterialResource*> _fixups;       // 0x60
    std::vector<FixupInfo> _locationFixups;                      // 0x6C
};
DECOMP_SIZE_ASSERT(MaterialManager::FixupInfo, 0x14);
DECOMP_SIZE_ASSERT(MaterialManager, 0x7C);

class TextureManager : public ResourceManager<TextureResource> { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> GetDefaultTexture() const; // inline; 0x0006AD64 is another unit's row (C0244)

    ResourceHandle<TextureResource> _defaultTexture;             // 0x58
    char field_5C[0x84 - 0x5C];                                  // 0x5C
};
DECOMP_SIZE_ASSERT(TextureManager, 0x84);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x24];                // 0x00
    MaterialManager* _materialManager; // 0x24
    char field_28[0x38 - 0x28];        // 0x28
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampTextureEffects { // NAME: proto-pdb:default.pdb
public:
    static ResourceHandle<TextureResource> GenerateEnvironmentMap(const Vector3& loc, unsigned int size);
};

extern "C" int __cdecl sscanf(const char*, const char*, ...); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000D5311
// PROTOTYPE: PREVIEW 0x004C08A2 identical
// NAME: proto-pdb:default.pdb
MaterialResource::MaterialResource()
    : ResourceBase("1"), _uvScrolling(false), _detailType(0), _reflectivity(0.0f), _uvRate1(0.0f, 0.0f),
      _uvRate2(0.0f, 0.0f)
{
    {
        TextureManager& mgr = *g_swamp->_textureManager;
        // !FAKE: hand form of the inline GetDefaultTexture() copy (its body is another unit's row, C0244)
        ResourceHandle<TextureResource> tmp;
        tmp._Resource = mgr._defaultTexture._Resource;
        if (tmp._Resource)
            tmp._Resource->AddRef();
        _baseTex = tmp;
    }
    _valid = true;
}

// FUNCTION: USA 0x000D563B
// PROTOTYPE: PREVIEW 0x004C0BCC identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 95.27749, bytes mismatch (this esi/edi swapped; cl cross-jumps the two find() calls into one, retail keeps them apart with fit in edi; frame 0xD0 vs 0xCC)
void MaterialManager::FixupEnvmaps(const BeepTDF* indexfile)
{
    // !FAKE: c2 register allocation; dummy local; the "EnvMaps" temp dies before the vector copy, block in ebx
    const BeepTDF* v1 = indexfile->FindBlock("EnvMaps", false);
    std::vector<BeepTDF*> envmaps(v1->GetBlocks());
    // !FAKE: c2 register allocation; dummy local; block iterator in ebx, the PDB records no slot
    for (std::vector<BeepTDF*>::iterator v2 = envmaps.begin(); v2 != envmaps.end(); ++v2) {
        SwampString locStr = (*v2)->GetStringRecord("Location", "");
        Vector3 location;
        sscanf(locStr.c_str(), "%f %f %f", &location.x, &location.y, &location.z);
        // !FAKE: c2 register allocation; dummy local; size in edi, the PDB records no slot
        unsigned int v3 = (*v2)->GetIntRecord("Size", 32);
        if (v3 < 32)
            v3 = 32;
        ResourceHandle<TextureResource> envTex = SwampTextureEffects::GenerateEnvironmentMap(location, v3);
        SwampString envmapname = BeepLib::MakeLower((*v2)->GetBlockName());
        std::multimap<SwampString, MaterialResource*>::iterator fit;
        for (fit = _fixups.find(envmapname); fit != _fixups.end(); fit = _fixups.find(envmapname)) {
            (*fit).second->_envmapTex = envTex;
            _fixups.erase(fit);
        }
    }
    // !FAKE: c2 register allocation; dummy local; location-fixup iterator in ebx, the PDB records no slot
    for (std::vector<FixupInfo>::iterator v4 = _locationFixups.begin(); v4 != _locationFixups.end(); ++v4)
        *(*v4).texPtr = SwampTextureEffects::GenerateEnvironmentMap((*v4).loc, (*v4).size);
    _locationFixups.clear();
    // !FAKE: c2 register allocation; dummy local; fixup iterator in eax, the PDB records no slot
    for (std::multimap<SwampString, MaterialResource*>::iterator v5 = _fixups.begin(); v5 != _fixups.end();) {
        if (!(*v5).second->_envmapTex.IsValid())
            (*v5).second->_detailType = 0;
        v5 = _fixups.erase(v5);
    }
}
#endif

// FUNCTION: USA 0x000D584E
// PROTOTYPE: PREVIEW 0x004C0DDF identical
// NAME: proto-pdb:default.pdb
void MaterialManager::ClearFixupEnvmaps()
{
    _fixups.clear();
    _locationFixups.clear();
}

// FUNCTION: USA 0x000D58A2
// PROTOTYPE: PREVIEW 0x004C0E33 identical
// NAME: proto-pdb:default.pdb
void MaterialManager::AddEnvmapFixup(const Vector3& loc, unsigned int size, ResourceHandle<TextureResource>* texPtr)
{
    if (size < 32)
        size = 32;
    FixupInfo info;
    info.texPtr = texPtr;
    info.loc = loc;
    info.size = size;
    _locationFixups.push_back(info);
}

// FUNCTION: USA 0x000D58D9
// NAME: proto-pdb:default.pdb
void MaterialManager::AddEnvmapFixup(const SwampString& envname, MaterialResource* mat)
{
    _fixups.insert(std::multimap<SwampString, MaterialResource*>::value_type(BeepLib::MakeLower(envname), mat));
}

// FUNCTION: USA 0x000D5EB4
// NAME: proto-pdb:default.pdb
MaterialResource::MaterialResource(const SwampString& name, const GatorFile& gator, const GatorMaterial& mtl)
    : ResourceBase(name), _uvScrolling(false), _autoGenEMap(false), _detailType(0), _reflectivity(0.0f),
      _uvRate1(mtl.URate1, mtl.VRate1), _uvRate2(mtl.URate2, mtl.VRate2)
{
    TextureManager& mgr = *g_swamp->_textureManager;
    if (mtl.base != 0xFFFF) {
        {
            SwampString texname(gator.GetString(mtl.base));
            _baseTex = mgr.Load(texname);
        }
        if (!_baseTex.IsValid()) {
            // !FAKE: hand form of the inline GetDefaultTexture() copy (its body is another unit's row, C0244)
            ResourceHandle<TextureResource> tmp;
            tmp._Resource = mgr._defaultTexture._Resource;
            if (tmp._Resource)
                tmp._Resource->AddRef();
            _baseTex = tmp;
        }
    } else {
        // !FAKE: hand form of the inline GetDefaultTexture() copy (its body is another unit's row, C0244)
        ResourceHandle<TextureResource> tmp;
        tmp._Resource = mgr._defaultTexture._Resource;
        if (tmp._Resource)
            tmp._Resource->AddRef();
        _baseTex = tmp;
    }
    if (mtl.detail != 0xFFFF) {
        {
            SwampString texname(gator.GetString(mtl.detail));
            _detailTex = mgr.Load(texname);
        }
        if (_detailTex.IsValid()) {
            _detailType = mtl.blend;
            switch (_detailType) {
            case 0:
            case 1:
            case 4:
            case 5:
            case 6:
            case 10:
            case 11:
            case 12:
            case 13:
            case 15:
            case 16:
                break;
            default:
                _detailType = 0;
            }
        }
    }
    if (mtl.bumpmap != 0xFFFF) {
        SwampString texname(gator.GetString(mtl.bumpmap));
        _bumpmapTex = mgr.Load(texname);
    }
    if (mtl.envmap != 0xFFFF) {
        SwampString texname(gator.GetString(mtl.envmap));
        if (texname.find('.') != SwampString::npos) {
            _envmapTex = mgr.Load(texname);
        } else {
            _autoGenEMap = true;
            g_swamp->_materialManager->AddEnvmapFixup(texname, this);
        }
        if (_autoGenEMap || _envmapTex.IsValid()) {
            if (_detailType == 0)
                _detailType = 15;
            if (mtl.reflectmap != 0xFFFF) {
                {
                    SwampString texname(gator.GetString(mtl.reflectmap));
                    _reflectmapTex = mgr.Load(texname);
                }
                if (!_reflectmapTex.IsValid())
                    _reflectivity = 0.5f;
                else
                    _reflectivity = 1.0f;
            } else {
                _reflectivity = mtl.reflectivity;
            }
        }
    }
    if (mtl.lightmap != 0xFFFF) {
        {
            SwampString texname(gator.GetString(mtl.lightmap));
            _lightmapTex = mgr.Load(texname);
        }
        if (_lightmapTex.IsValid())
            _detailType = 16;
    }
    _valid = true;
}

// FUNCTION: USA 0x000D6205
// PROTOTYPE: PREVIEW 0x004C1796 identical
// NAME: proto-pdb:default.pdb
ResourceHandle<MaterialResource> MaterialManager::CreateMaterial(const BeepTDF* definition)
{
    SwampString name = BeepLib::MakeLower(definition->GetBlockName());
    MaterialResource* p = Find(name);
    if (!p) {
        p = new("SwampMaterialResource.cpp", 368) MaterialResource(name, definition);
        Add(p);
    }
    return ResourceHandle<MaterialResource>(p);
}

// FUNCTION: USA 0x000D6296
// NAME: proto-pdb:default.pdb
ResourceHandle<MaterialResource> MaterialManager::CreateMaterial(const SwampString& GatorName, const GatorFile& gator,
                                                                 unsigned int matid)
{
    SwampString name = format("gator:%s_%i", BeepLib::MakeLower(GatorName).c_str(), matid);
    MaterialResource* p = Find(name);
    if (!p) {
        p = new("SwampMaterialResource.cpp", 388) MaterialResource(name, gator, gator.GetMaterials()[matid]);
        Add(p);
    }
    return ResourceHandle<MaterialResource>(p);
}

// FUNCTION: USA 0x000D6350
// PROTOTYPE: PREVIEW 0x004C18E1 identical
// NAME: proto-pdb:default.pdb
MaterialManager::MaterialManager()
{
    // !FAKE: c2 register allocation; dummy local; the new result is used by Add and the handle
    MaterialResource* p = new("SwampMaterialResource.cpp", 353) MaterialResource();
    Add(p);
    _defaultMaterial = ResourceHandle<MaterialResource>(p);
}
