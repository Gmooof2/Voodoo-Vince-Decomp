// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampShaderResource.cpp OBJ: Xbox\Official_Release\SwampShaderResource.obj OPT: /O1 /Oy /Oi
#include "../../../../include/decomp.h"
#include <string.h>
#pragma warning(disable: 4530)
#include <algorithm>

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

extern "C" void __stdcall D3DDevice_DeleteVertexShader(unsigned long Handle); // NAME: proto-pdb:default.pdb

struct _D3DVERTEXSHADERINPUT { // NAME: proto-pdb:default.pdb
    unsigned long StreamIndex;         // 0x00
    unsigned long Offset;              // 0x04
    unsigned long Format;              // 0x08
    unsigned char TessType;            // 0x0C
    unsigned char TessSource;          // 0x0D
};
DECOMP_SIZE_ASSERT(_D3DVERTEXSHADERINPUT, 0x10);

struct _D3DVERTEXATTRIBUTEFORMAT { // NAME: proto-pdb:default.pdb
    _D3DVERTEXSHADERINPUT Input[16];   // 0x00
};
DECOMP_SIZE_ASSERT(_D3DVERTEXATTRIBUTEFORMAT, 0x100);

struct ShaderInfo { // NAME: proto-pdb:default.pdb
    const char* name;                  // 0x00
    int index;                         // 0x04
    int len;                           // 0x08
};
DECOMP_SIZE_ASSERT(ShaderInfo, 0xC);

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString MakeLower(const SwampString&);
}

void* operator new(unsigned int size, const char* file, int line);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }

    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    ~VertexShaderResource();

protected:
    VertexShaderResource(const SwampString&, const _D3DVERTEXATTRIBUTEFORMAT*);
    friend class VertexShaderManager;

public:
    unsigned long _handle;            // 0x24
    const unsigned long* _shaderCode;  // 0x28
    const _D3DVERTEXATTRIBUTEFORMAT* _declaration; // 0x2C
};
DECOMP_SIZE_ASSERT(VertexShaderResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(T* r) : _Resource(r) { _Resource->AddRef(); }
    ResourceHandle(const ResourceHandle&);
    ~ResourceHandle();

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
protected:
    void Add(T*);
    T* Find(const SwampString&) const;

    char field_0[0x58];                // 0x00
};

class VertexShaderManager : public ResourceManager<VertexShaderResource> { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<VertexShaderResource> CreateShader(const SwampString& shaderName, const _D3DVERTEXATTRIBUTEFORMAT* declaration);
};
DECOMP_SIZE_ASSERT(VertexShaderManager, 0x58);

// FUNCTION: USA 0x000CABE4
// PROTOTYPE: PREVIEW 0x004B5E2D identical
// NAME: proto-pdb:default.pdb
bool operator<(const ShaderInfo& si0, const ShaderInfo& si1)
{
    return strcmp(si0.name, si1.name) < 0;
}

// FUNCTION: USA 0x000CAC45
// PROTOTYPE: PREVIEW 0x004B5E8E identical
// NAME: proto-pdb:default.pdb
template ShaderInfo * std::_Lower_bound<ShaderInfo *,ShaderInfo,int>(ShaderInfo *,ShaderInfo *,ShaderInfo const &,int *);

// FUNCTION: USA 0x000CAC23
// PROTOTYPE: PREVIEW 0x004B5E6C identical
// NAME: proto-pdb:default.pdb
void InitVAF(_D3DVERTEXATTRIBUTEFORMAT& vaf)
{
    memset(&vaf, 0, sizeof(vaf));
    // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
    for (int v0 = 0; v0 < 16; v0++)
        vaf.Input[v0].Format = 2; // D3DVSDT_NONE, retail
}

// FUNCTION: USA 0x000CAC9C
// PROTOTYPE: PREVIEW 0x004B5EE5 identical
// NAME: proto-pdb:default.pdb
VertexShaderResource::~VertexShaderResource()
{
    if (_handle)
        D3DDevice_DeleteVertexShader(_handle);
}

// FUNCTION: USA 0x000CADF0
// PROTOTYPE: PREVIEW 0x004B6039 identical
// NAME: proto-pdb:default.pdb
ResourceHandle<VertexShaderResource> VertexShaderManager::CreateShader(const SwampString& shaderName, const _D3DVERTEXATTRIBUTEFORMAT* declaration)
{
    SwampString name = BeepLib::MakeLower(shaderName);
    // !FAKE: c1xx front end; neutral local; the PDB records no local for the found or new resource
    VertexShaderResource* p = Find(name);
    if (!p) {
        p = new("SwampShaderResource.cpp", 108) VertexShaderResource(name, declaration);
        Add(p);
    }
    return ResourceHandle<VertexShaderResource>(p);
}
