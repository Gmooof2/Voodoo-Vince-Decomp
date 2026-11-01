// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampModelResource.cpp OBJ: Xbox\Official_Release\SwampModelResource.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator;
template <class E> struct char_traits;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[0x1C / 4];
};
template <class T, class A = allocator<T> > class vector {
public:
    class const_iterator {
    public:
        const_iterator(T* ptr) : _Myptr(ptr) {}
        const T& operator*() const { return *_Myptr; }
        const T* operator->() const { return _Myptr; }
        const_iterator& operator++() { ++_Myptr; return *this; }
        bool operator!=(const const_iterator& right) const { return _Myptr != right._Myptr; }
        const_iterator& operator+=(int off) { _Myptr += off; return *this; }
        const_iterator operator+(int off) const { const_iterator tmp = *this; return tmp += off; }
        T* _Myptr;
    };
    const_iterator begin() const { return const_iterator(_Myfirst); }
    const_iterator end() const { return const_iterator(_Mylast); }
    const T& operator[](unsigned int pos) const { return *(begin() + pos); }

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
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

struct Matrix { // NAME: proto-pdb:default.pdb
    void AffineMultiply(const Matrix&, Matrix&) const;

    char field_0[0x40];                // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct GatorBone { // NAME: proto-pdb:default.pdb
    unsigned short nameString;         // 0x00
    unsigned short parentIndex;        // 0x02
    unsigned int offsetBSP;            // 0x04
    AABox bounds;                      // 0x08
    Matrix forwardTransform;           // 0x20
    Matrix invTransform;               // 0x60
    Matrix rotateAxis;                 // 0xA0
    Matrix jointOrient;                // 0xE0
};
DECOMP_SIZE_ASSERT(GatorBone, 0x120);

struct GatorStrip { // NAME: proto-pdb:default.pdb
    unsigned int numIndices;           // 0x00
    char field_4[0x10];                // 0x04
    unsigned short matid;              // 0x14
    char field_16[0x30 - 0x16];        // 0x16
};
DECOMP_SIZE_ASSERT(GatorStrip, 0x30);

struct GatorFile { // NAME: proto-pdb:default.pdb
    const unsigned short* GetIndices() const { return (const unsigned short*)((const char*)this + offsetIndices); }
    const GatorStrip* GetStrips() const { return (const GatorStrip*)((const char*)this + offsetStrips); }
    const GatorBone* GetBones() const { return (const GatorBone*)((const char*)this + offsetBones); }
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

struct D3DVertexBuffer { // NAME: proto-pdb:default.pdb
    unsigned long Common;              // 0x00
    unsigned long Data;                // 0x04
    unsigned long Lock;                // 0x08
};
DECOMP_SIZE_ASSERT(D3DVertexBuffer, 0xC);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    const T& operator()() const { return *_Resource; }
    bool operator==(const ResourceHandle& h) const { return _Resource == h._Resource; }

    T* _Resource;                      // 0x00
};

class MaterialResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    const Vector2& uvRate1() const { return _uvRate1; }
    const Vector2& uvRate2() const { return _uvRate2; }

    char field_24[0x48 - 0x24];        // 0x24
    Vector2 _uvRate1;                  // 0x48
    Vector2 _uvRate2;                  // 0x50
};
DECOMP_SIZE_ASSERT(MaterialResource, 0x58);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    static void InitShaders();
    unsigned int GetVertexCount() const;
    unsigned int GetStripCount() const;
    bool IsPrelit() const;
    unsigned int GetBoneCount() const;
    unsigned int GetParticleBoneCount() const;
    const char* GetBoneName(unsigned int boneIndex) const;
    unsigned int GetParticleBoneIndex(unsigned int i) const;
    void GetBoneOrientation(int boneIndex, const Matrix& deformation, Matrix& result) const;
    const AABox& GetBounds() const;
    const AACylinder& GetCylinder() const;
    const AABox& GetBoneBounds(unsigned int bone) const;
    void GetStripUVRate1(unsigned int stripIndex, Vector2& uvRate) const;
    void GetStripUVRate2(unsigned int stripIndex, Vector2& uvRate) const;
    void LowLevelRender(bool vb_override) const;

    const GatorFile* _gator;           // 0x24
    char field_28[0x2C - 0x28];        // 0x28
    std::vector<ResourceHandle<MaterialResource> > _materials; // 0x2C
    char field_3C[0x40 - 0x3C];        // 0x3C
    D3DVertexBuffer _vertexBuffer;     // 0x40
    char field_4C[0x60 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(ModelResource, 0x60);

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    long SetStreamSource(D3DVertexBuffer* vb, unsigned int stride);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampRenderer& GetRenderer() const { return *_renderer; }

    char field_0[0x8];                 // 0x00
    SwampRenderer* _renderer;          // 0x08
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

enum _D3DPRIMITIVETYPE { D3DPT_TRIANGLESTRIP = 6 }; // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawIndexedVertices(_D3DPRIMITIVETYPE primitiveType, unsigned int vertexCount, const unsigned short* indexData); // src proto-pdb:default.pdb D3DDevice_DrawIndexedVertices

struct SingleTexture { // NAME: proto-pdb:default.pdb
    char field_0[0x1C];                // 0x00
    bool _usesAlpha;                   // 0x1C
};

struct AnimatedTexture { // NAME: proto-pdb:default.pdb
    char field_0[0x1C];                // 0x00
    bool _usesAlpha;                   // 0x1C
};

class MovieTexture;          // NAME: proto-pdb:default.pdb

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    bool UsesAlpha() const;

    SingleTexture* _singleTexture;     // 0x24
    AnimatedTexture* _animatedTexture; // 0x28
    MovieTexture* _movieTexture;       // 0x2C
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

// FUNCTION: USA 0x000BA6E8
// PROTOTYPE: PREVIEW 0x004A53F3 identical
// NAME: proto-pdb:default.pdb
unsigned int ModelResource::GetVertexCount() const
{
    return _gator ? _gator->numVertices : 0;
}

// FUNCTION: USA 0x000BA6F6
// PROTOTYPE: PREVIEW 0x004A5401 identical
// NAME: proto-pdb:default.pdb
unsigned int ModelResource::GetStripCount() const
{
    return _gator ? _gator->numStrips : 0;
}

// FUNCTION: USA 0x000BA704
// PROTOTYPE: PREVIEW 0x004A540F identical
// NAME: proto-pdb:default.pdb
bool ModelResource::IsPrelit() const
{
    return _gator ? _gator->usesPreLitVerts : false;
}

// FUNCTION: USA 0x000BA712
// PROTOTYPE: PREVIEW 0x004A541D identical
// NAME: proto-pdb:default.pdb
unsigned int ModelResource::GetBoneCount() const
{
    return _gator ? _gator->numBones : 0;
}

// FUNCTION: USA 0x000BA720
// PROTOTYPE: PREVIEW 0x004A542B identical
// NAME: proto-pdb:default.pdb
unsigned int ModelResource::GetParticleBoneCount() const
{
    return _gator ? _gator->numParticleBones : 0;
}

// FUNCTION: USA 0x000BA72F
// PROTOTYPE: PREVIEW 0x004A543A identical
// NAME: proto-pdb:default.pdb
const char* ModelResource::GetBoneName(unsigned int boneIndex) const
{
    return _gator->GetString(_gator->GetBones()[boneIndex].nameString);
}

// FUNCTION: USA 0x000BA754
// PROTOTYPE: PREVIEW 0x004A545F identical
// NAME: proto-pdb:default.pdb
unsigned int ModelResource::GetParticleBoneIndex(unsigned int i) const
{
    return _gator->numRenderBones + _gator->numLocatorBones + i;
}

// FUNCTION: USA 0x000BA768
// PROTOTYPE: PREVIEW 0x004A5473 identical
// NAME: proto-pdb:default.pdb
void ModelResource::GetBoneOrientation(int boneIndex, const Matrix& deformation, Matrix& result) const
{
    _gator->GetBones()[boneIndex].forwardTransform.AffineMultiply(deformation, result);
}

// FUNCTION: USA 0x000BA78C
// PROTOTYPE: PREVIEW 0x004A5497 identical
// NAME: proto-pdb:default.pdb
const AABox& ModelResource::GetBounds() const
{
    return _gator->bounds;
}

// FUNCTION: USA 0x000BA793
// PROTOTYPE: PREVIEW 0x004A549E identical
// NAME: proto-pdb:default.pdb
const AACylinder& ModelResource::GetCylinder() const
{
    return _gator->bc;
}

// FUNCTION: USA 0x000BA79A
// PROTOTYPE: PREVIEW 0x004A54A5 identical
// NAME: proto-pdb:default.pdb
const AABox& ModelResource::GetBoneBounds(unsigned int bone) const
{
    return _gator->GetBones()[bone].bounds;
}

// FUNCTION: USA 0x000BA7B1
// PROTOTYPE: PREVIEW 0x004A54BC identical
// NAME: proto-pdb:default.pdb
bool TextureResource::UsesAlpha() const
{
    if (_singleTexture)
        return _singleTexture->_usesAlpha;
    if (_animatedTexture)
        return _animatedTexture->_usesAlpha;
    return false;
}

// FUNCTION: USA 0x000BA7CA
// PROTOTYPE: PREVIEW 0x004A54D5 identical
// NAME: proto-pdb:default.pdb
void ModelResource::LowLevelRender(bool vb_override) const
{
    if (_gator) {
        SwampRenderer& renderer = g_swamp->GetRenderer();
        if (!vb_override)
            renderer.SetStreamSource(const_cast<D3DVertexBuffer*>(&_vertexBuffer), 0x20);
        const GatorStrip* strips = _gator->GetStrips();
        const unsigned short* indices = _gator->GetIndices();
        for (unsigned int stripNum = 0; stripNum < _gator->numStrips; stripNum++) {
            D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLESTRIP, strips[stripNum].numIndices, indices);
            indices += strips[stripNum].numIndices;
        }
        if (!vb_override)
            renderer.SetStreamSource(0, 0);
    }
}

class TextureOverrides { // NAME: proto-pdb:default.pdb
public:
    struct Override { // NAME: proto-pdb:default.pdb
        ResourceHandle<TextureResource> original;    // 0x00
        ResourceHandle<TextureResource> replacement; // 0x04
    };
    typedef std::vector<Override> Overrides; // NAME: proto-pdb:default.pdb

    ResourceHandle<TextureResource> GetOverride(const ResourceHandle<TextureResource>& original) const;

    Overrides _overrides;              // 0x00
};
DECOMP_SIZE_ASSERT(TextureOverrides, 0x10);

// FUNCTION: USA 0x000BAC9E
// PROTOTYPE: PREVIEW 0x004A59C0 identical
// NAME: proto-pdb:default.pdb
ResourceHandle<TextureResource> TextureOverrides::GetOverride(const ResourceHandle<TextureResource>& original) const
{
    for (Overrides::const_iterator i = _overrides.begin(); i != _overrides.end(); ++i) {
        if (original == i->original)
            return i->replacement;
    }
    return original;
}

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceManager();
    void SetExtension(const SwampString& ext) { _Extension = ext; }

    char field_0[0x28];                // 0x00
    SwampString _Extension;            // 0x28
    char field_44[0x58 - 0x44];        // 0x44
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
private:
    ModelManager();
};
DECOMP_SIZE_ASSERT(ModelManager, 0x58);

// FUNCTION: USA 0x000BB3E4
// NAME: proto-pdb:default.pdb
void ModelResource::GetStripUVRate1(unsigned int stripIndex, Vector2& uvRate) const
{
    uvRate = _materials[_gator->GetStrips()[stripIndex].matid]().uvRate1();
}

// FUNCTION: USA 0x000BB417
// NAME: proto-pdb:default.pdb
void ModelResource::GetStripUVRate2(unsigned int stripIndex, Vector2& uvRate) const
{
    uvRate = _materials[_gator->GetStrips()[stripIndex].matid]().uvRate2();
}

// FUNCTION: USA 0x000BCB8B
// PROTOTYPE: PREVIEW 0x004A78E2 identical
// NAME: proto-pdb:default.pdb
ModelManager::ModelManager()
{
    SetExtension(".gator");
    ModelResource::InitShaders();
}
