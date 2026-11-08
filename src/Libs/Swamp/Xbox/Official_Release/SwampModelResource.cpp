// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampModelResource.cpp OBJ: Xbox\Official_Release\SwampModelResource.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator {};
template <class E> struct char_traits;
template <class _InIt, class _OutIt> _OutIt copy(_InIt _First, _InIt _Last, _OutIt _Dest);
template <class _Ty, class _Alloc> void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al);
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    const E* c_str() const { return _Myptr(); }
    unsigned int size() const { return (_Mysize); }
    bool empty() const { return (size() == 0); }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

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
    class iterator : public const_iterator {
    public:
        iterator(T* ptr) : const_iterator(ptr) {}
        T& operator*() const { return (T&)**(const_iterator*)this; }
        iterator& operator+=(int off) { this->_Myptr += off; return *this; }
        iterator operator+(int off) const { iterator tmp = *this; return tmp += off; }
    };
    iterator begin() { return iterator(_Myfirst); }
    const_iterator begin() const { return const_iterator(_Myfirst); }
    iterator end() { return iterator(_Mylast); }
    const_iterator end() const { return const_iterator(_Mylast); }
    T& operator[](unsigned int pos) { return *(begin() + pos); }
    const T& operator[](unsigned int pos) const { return *(begin() + pos); }
    void resize(unsigned int _Newsize) { resize(_Newsize, T()); }
    void resize(unsigned int _Newsize, T _Val)
    {
        if (size() < _Newsize)
            _Insert_n(end(), _Newsize - size(), _Val);
        else if (_Newsize < size())
            erase(begin() + _Newsize, end());
    }
    unsigned int size() const;
    void reserve(unsigned int _Count);
    void push_back(const T& _Val);
    iterator erase(iterator _Where)
    {
        copy(_Where._Myptr + 1, _Mylast, _Where._Myptr);
        _Destroy(_Mylast - 1, _Mylast);
        --_Mylast;
        return (_Where);
    }
    iterator erase(iterator _First, iterator _Last);
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}

protected:
    void _Insert_n(iterator _Where, unsigned int _Count, const T& _Val);
    void _Destroy(T* _First, T* _Last) { _Destroy_range(_First, _Last, this->_Alval); }

private:
    A _Alval;                          // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
typedef unsigned int _Vbase;
const int _VBITS = 8 * sizeof(_Vbase);
template <class A> class vector<bool, A> {
public:
    typedef bool const_reference;
    class _Vb_iter_base {
    public:
        _Vb_iter_base(_Vbase* _Ptr) : _Myoff(0), _Myptr(_Ptr) {}
        unsigned int _Myoff;
        _Vbase* _Myptr;
    };
    class reference : public _Vb_iter_base {
    public:
        reference(const _Vb_iter_base& _Right) : _Vb_iter_base(_Right) {}
        reference& operator=(bool _Val);
        operator bool() const { return ((*_Getptr() & _Mask()) != 0); }
        _Vbase* _Getptr() const { return (this->_Myptr); }

    protected:
        _Vbase _Mask() const { return ((_Vbase)(1 << this->_Myoff)); }
    };
    typedef reference _Reft;
    class const_iterator : public _Vb_iter_base {
    public:
        const_iterator(const _Vbase* _Ptr) : _Vb_iter_base((_Vbase*)_Ptr) {}
        const_reference operator*() const { return (_Reft(*this)); }
        const_iterator& operator+=(int _Off)
        {
            this->_Myoff += _Off;
            this->_Myptr += this->_Myoff / _VBITS;
            this->_Myoff %= _VBITS;
            return (*this);
        }
        const_iterator operator+(int _Off) const
        {
            const_iterator _Tmp = *this;
            return (_Tmp += _Off);
        }
    };
    class iterator : public const_iterator {
    public:
        iterator(_Vbase* _Ptr) : const_iterator(_Ptr) {}
        reference operator*() const { return (reference(*this)); }
        iterator& operator+=(int _Off)
        {
            this->_Myoff += _Off;
            this->_Myptr += this->_Myoff / _VBITS;
            this->_Myoff %= _VBITS;
            return (*this);
        }
        iterator operator+(int _Off) const
        {
            iterator _Tmp = *this;
            return (_Tmp += _Off);
        }
    };
    iterator begin() { return (iterator(_Myvec.begin()._Myptr)); }
    const_iterator begin() const { return (const_iterator(_Myvec.begin()._Myptr)); }
    reference operator[](unsigned int _Off) { return (*(begin() + _Off)); }
    const_reference operator[](unsigned int _Off) const { return (*(begin() + _Off)); }

    unsigned int _Mysize;              // 0x00
    vector<_Vbase> _Myvec;             // 0x04
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

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    void Set(float, float, float);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

Quaternion Slerp(const Quaternion&, const Quaternion&, float); // NAME: proto-pdb:default.pdb

struct AABox { // NAME: proto-pdb:default.pdb
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct AACylinder { // NAME: proto-pdb:default.pdb
    char field_0[0x14];                // 0x00
};
DECOMP_SIZE_ASSERT(AACylinder, 0x14);

struct Vector4 { // NAME: proto-pdb:default.pdb
    float& operator[](int i) { return (&x)[i]; }
    const float& operator[](int i) const { return (&x)[i]; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Vector4& operator[](int i) { return row[i]; }
    const Vector4& operator[](int i) const { return row[i]; }
    void Rotation(float, float, float);
    void Rotation(const Quaternion&);
    void Multiply(const Matrix&, Matrix&) const;
    void AffineMultiply(const Matrix&, Matrix&) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct SwampDeformation { // NAME: proto-pdb:default.pdb
    Vector3 scaling;                   // 0x00
    Vector3 rotation;                  // 0x0C
    Vector3 translation;               // 0x18
    float overlayWeight;               // 0x24
};
DECOMP_SIZE_ASSERT(SwampDeformation, 0x28);

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
    enum { MIRROR_VINCE = 0x2, MIRROR_WORLD = 0x4 }; // NAME: proto-pdb:default.pdb

    unsigned int numIndices;           // 0x00
    char field_4[0x10];                // 0x04
    unsigned short matid;              // 0x14
    char field_16[0x18 - 0x16];        // 0x16
    unsigned int render_flags;         // 0x18
    char field_1C[0x30 - 0x1C];        // 0x1C
};
DECOMP_SIZE_ASSERT(GatorStrip, 0x30);

struct GatorFile;

struct GatorReference { // NAME: proto-pdb:default.pdb
    unsigned int index;                // 0x00
    unsigned int name;                 // 0x04
};
DECOMP_SIZE_ASSERT(GatorReference, 0x8);

struct GatorBoneTable { // NAME: proto-pdb:default.pdb
    const unsigned char* Get(const GatorFile* file) const { return (const unsigned char*)file + offset; }

    unsigned int count;                // 0x00
    unsigned int offset;               // 0x04
};
DECOMP_SIZE_ASSERT(GatorBoneTable, 0x8);

struct GatorFile { // NAME: proto-pdb:default.pdb
    const GatorBoneTable& GetBoneTable(unsigned int i) const
    {
        return ((const GatorBoneTable*)((const char*)this + offsetBoneTables))[i];
    }
    const unsigned short* GetIndices() const { return (const unsigned short*)((const char*)this + offsetIndices); }
    const GatorStrip* GetStrips() const { return (const GatorStrip*)((const char*)this + offsetStrips); }
    const GatorReference* GetReferences() const
    {
        return (const GatorReference*)((const char*)this + offsetReferences);
    }
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
    void SubRef() { --_RefCount; }
    const SwampString& GetName() const { return _Name; }

protected:
    ResourceBase(const SwampString& name);

public:

    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h)
    {
        if (_Resource)
            _Resource->SubRef();
        _Resource = h._Resource;
        if (_Resource)
            _Resource->AddRef();
        return *this;
    }
    bool IsValid() const { return _Resource != 0; }
    const T& operator()() const { return *_Resource; }
    bool operator==(const ResourceHandle& h) const { return _Resource == h._Resource; }

    T* _Resource;                      // 0x00
};

class TextureResource;

class MaterialResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    const ResourceHandle<TextureResource>& GetBaseTexture() const { return _baseTex; }
    const ResourceHandle<TextureResource>& GetDetailTexture() const { return _detailTex; }
    const Vector2& uvRate1() const { return _uvRate1; }
    const Vector2& uvRate2() const { return _uvRate2; }

    char field_24[0x28 - 0x24];        // 0x24
    ResourceHandle<TextureResource> _baseTex;   // 0x28
    ResourceHandle<TextureResource> _detailTex; // 0x2C
    char field_30[0x48 - 0x30];        // 0x30
    Vector2 _uvRate1;                  // 0x48
    Vector2 _uvRate2;                  // 0x50
};
DECOMP_SIZE_ASSERT(MaterialResource, 0x58);
// !FAKE: linker fold; decl-only explicit specialization; one template arg per fold VA 0x1819A (C0154)
template <> ResourceHandle<MaterialResource>& ResourceHandle<MaterialResource>::operator=(const ResourceHandle<MaterialResource>& h);

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(VertexShaderResource, 0x30);

struct _D3DVERTEXATTRIBUTEFORMAT; // NAME: proto-pdb:default.pdb

extern "C" void __fastcall D3DDevice_SetVertexShaderConstantNotInline(int, const void*, unsigned int); // src proto-pdb:default.pdb D3DDevice_SetVertexShaderConstantNotInline

class BSP { // NAME: proto-pdb:default.pdb
public:
    BSP(void*, bool);

    char field_0[0x40];                // 0x00
};
DECOMP_SIZE_ASSERT(BSP, 0x40);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    enum BaseVertexShaders { // NAME: proto-pdb:default.pdb
        Gator = 0,
        GatorBump = 1,
        GatorEnvm = 2,
        GatorEdge = 3,
        GatorShadow = 4,
        GatorShadowBuf = 5,
        BaseVertexShadersTotal = 6
    };
    enum LightingType { // NAME: proto-pdb:default.pdb
        None = 0,
        Constant = 0,
        Point = 1,
        PointRanged = 2,
        Spot = 3,
        SpotRanged = 4,
        LightingTypeTotal = 5
    };

    void SetAppropriateVertexShader(BaseVertexShaders base, LightingType lighting, const _D3DVERTEXATTRIBUTEFORMAT* vertexType) const;
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
    ModelResource(const SwampString& name);
    int GetBoneIndex(const SwampString& boneName, bool canFail) const;
    void ProcessDeformations(const Matrix& world, const std::vector<Matrix>* deformations) const;
    void SetStripControl(std::vector<bool>& stripControl, const SwampString& name, bool on) const;
    void ApplyDeformationsToSkeletion(std::vector<Matrix>& matrices, const std::vector<SwampDeformation>& deformations,
                                      unsigned int boneCount) const;
    void ApplyDeformationsToSkeletion(std::vector<Matrix>& matrices, const std::vector<SwampDeformation>& primary,
                                      const std::vector<SwampDeformation>& secondary, unsigned int boneCount) const;
    void ApplyDeformationsToSkeletion(std::vector<Matrix>& matrices, const std::vector<SwampDeformation>& primary,
                                      const std::vector<SwampDeformation>& secondary, float tween,
                                      unsigned int boneCount) const;

private:
    void CalculateBoneDeformation(std::vector<Matrix>& matrices, unsigned int boneNum, const Vector3& scale,
                                  const Vector3& translation, const Matrix& rotation, const Vector3& parentScale) const;
    void LoadMaterials();
    void SetBoneTable(unsigned int table) const;
    void ConvertDeformationsToModelSpace(std::vector<Matrix>& matrices, unsigned int boneCount) const;

    static _D3DVERTEXATTRIBUTEFORMAT _vafGator;
    static Vector4 _vShaderBones[252];
    static ResourceHandle<VertexShaderResource> _vertexShaders[3][6][5];

public:
    const GatorFile* _gator;           // 0x24
    bool _hasBSPs;                     // 0x28
    bool _usesAlpha;                   // 0x29
    bool _hasMirrors;                  // 0x2A
    std::vector<ResourceHandle<MaterialResource> > _materials; // 0x2C
    ResourceHandle<TextureResource> _animatedTexture; // 0x3C
    D3DVertexBuffer _vertexBuffer;     // 0x40
    std::vector<BSP*> _bsps;           // 0x4C
    GatorFile* _myGator;               // 0x5C
};
DECOMP_SIZE_ASSERT(ModelResource, 0x60);

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    long SetStreamSource(D3DVertexBuffer* vb, unsigned int stride);
    long SetVertexShader(const ResourceHandle<VertexShaderResource>& shader, const _D3DVERTEXATTRIBUTEFORMAT* format);
};

class MaterialManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<MaterialResource> CreateMaterial(const SwampString& basename, const GatorFile& gator, unsigned int i);
};

class ModelManager;

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampRenderer& GetRenderer() const { return *_renderer; }
    MaterialManager& GetMaterialManager() const { return *_materialManager; }
    ModelManager& GetModelManager() const { return *_modelManager; }

    char field_0[0x8];                 // 0x00
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x24 - 0xC];          // 0x0C
    MaterialManager* _materialManager; // 0x24
    ModelManager* _modelManager;       // 0x28
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString StripPath(const SwampString&);
SwampString StripExtension(const SwampString&);
}
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
    bool IsAnimated() const { return _animatedTexture != 0; }
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

// FUNCTION: USA 0x000BAAA2
// PROTOTYPE: PREVIEW 0x004A57AD identical
// NAME: proto-pdb:default.pdb
void ModelResource::SetBoneTable(unsigned int table) const
{
    if (_gator->usesBoneTables) {
        // !FAKE: c1xx front end; neutral stand-in locals; the proto PDB lists no local for the table, bones or counter
        const GatorBoneTable& v1 = _gator->GetBoneTable(table);
        const unsigned char* p = v1.Get(_gator);
        // !FAKE: c2 scheduling; both increments inside the call arguments; base add before push 12 (match_one 94.29 -> 100.0)
        for (unsigned int i = 0; i < v1.count;)
            D3DDevice_SetVertexShaderConstantNotInline(i++ * 3 + 60, &_vShaderBones[*p++ * 3], 12);
    }
}

extern "C" int __cdecl _stricmp(const char*, const char*); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000BAB80
// PROTOTYPE: PREVIEW 0x004A587B identical
// NAME: proto-pdb:default.pdb
int ModelResource::GetBoneIndex(const SwampString& boneName, bool canFail) const
{
    // !FAKE: c1xx front end; neutral stand-in locals p/v0/v1; the proto PDB lists no local for the bones, the
    // counter or the name; v1 computes the name before c_str as retail (74.53 -> 100.0)
    const GatorBone* p = _gator->GetBones();
    for (unsigned int v0 = 0; v0 < _gator->numBones; ++v0, ++p) {
        const char* v1 = _gator->GetString(p->nameString);
        if (!_stricmp(boneName.c_str(), v1))
            return v0;
    }
    return -1;
}

// FUNCTION: USA 0x000BAC46
// PROTOTYPE: PREVIEW 0x004A5968 identical
// NAME: proto-pdb:default.pdb
void ModelResource::SetAppropriateVertexShader(BaseVertexShaders base, LightingType lighting, const _D3DVERTEXATTRIBUTEFORMAT* vertexType) const
{
    ResourceHandle<VertexShaderResource> shader = _vertexShaders[_gator->maxBoneBlend - 1][base][lighting];
    g_swamp->GetRenderer().SetVertexShader(shader, vertexType ? vertexType : &_vafGator);
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

void BuildTransformation(float*, const float*, const float*, const float*); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000BAD59
// PROTOTYPE: PREVIEW 0x004A5A95 identical
// NAME: proto-pdb:default.pdb
void ModelResource::CalculateBoneDeformation(std::vector<Matrix>& matrices, unsigned int boneNum, const Vector3& scale,
                                             const Vector3& translation, const Matrix& rotation,
                                             const Vector3& parentScale) const
{
    // !FAKE: c1xx front end; neutral stand-in local p; the proto PDB lists no local for the bone
    const GatorBone* p = &_gator->GetBones()[boneNum];
    Matrix acc;
    p->rotateAxis.AffineMultiply(rotation, acc);
    acc.AffineMultiply(p->jointOrient, acc);
    BuildTransformation((float*)&acc, (const float*)&scale, (const float*)&parentScale, (const float*)&translation);
    if (p->parentIndex != 0xFFFF)
        acc.AffineMultiply(matrices[p->parentIndex], matrices[boneNum]);
    else
        matrices[boneNum] = acc;
}

// FUNCTION: USA 0x000BADE7
// PROTOTYPE: PREVIEW 0x004A5B23 identical
// NAME: proto-pdb:default.pdb
void ModelResource::ConvertDeformationsToModelSpace(std::vector<Matrix>& matrices, unsigned int boneCount) const
{
    // !FAKE: c2 register allocation; neutral stand-in locals v0/v1; the proto PDB lists none; the bone matrix
    // reference gives retail's ecx/eax roles (direct call 93.06, matrix reference 98.06 -> 100.0)
    for (unsigned int v0 = 0; v0 < boneCount; ++v0) {
        const Matrix& v1 = _gator->GetBones()[v0].invTransform;
        v1.Multiply(matrices[v0], matrices[v0]);
    }
}

// FUNCTION: USA 0x000BAE2D
// PROTOTYPE: PREVIEW 0x004A5B69 identical
// NAME: proto-pdb:default.pdb
void ModelResource::ApplyDeformationsToSkeletion(std::vector<Matrix>& matrices,
                                                 const std::vector<SwampDeformation>& deformations,
                                                 unsigned int boneCount) const
{
    if (!_gator)
        return;
    unsigned int bonesToProcess = _gator->numBones;
    if (boneCount)
        bonesToProcess = boneCount;
    for (unsigned int b = 0; b < bonesToProcess; b++) {
        // !FAKE: c1xx front end; neutral stand-in locals p/v0; the proto PDB lists none for the deformation or bone;
        // p first gives retail's induction slots (v0 first 99.83 -> 100.0)
        const GatorBone* p = &_gator->GetBones()[b];
        const SwampDeformation* v0 = &deformations[b];
        Matrix rot;
        rot.Rotation(v0->rotation.x, v0->rotation.y, v0->rotation.z);
        Vector3 zip;
        if (p->parentIndex != 0xFFFF) {
            zip = deformations[p->parentIndex].scaling;
        } else {
            zip.x = 1.0f;
            zip.y = 1.0f;
            zip.z = 1.0f;
        }
        CalculateBoneDeformation(matrices, b, v0->scaling, v0->translation, rot, zip);
    }
    ConvertDeformationsToModelSpace(matrices, bonesToProcess);
}

// FUNCTION: USA 0x000BAF15
// PROTOTYPE: PREVIEW 0x004A5C51 identical
// NAME: proto-pdb:default.pdb
void ModelResource::ApplyDeformationsToSkeletion(std::vector<Matrix>& matrices,
                                                 const std::vector<SwampDeformation>& primary,
                                                 const std::vector<SwampDeformation>& secondary,
                                                 unsigned int boneCount) const
{
    if (!_gator)
        return;
    unsigned int bonesToProcess = _gator->numBones;
    if (boneCount)
        bonesToProcess = boneCount;
    for (unsigned int b = 0; b < bonesToProcess; b++) {
        // !FAKE: c1xx front end; neutral stand-in locals p/v0/v1; the proto PDB lists none for the bone or the
        // deformations
        const GatorBone* p = &_gator->GetBones()[b];
        const SwampDeformation* v0 = &secondary[b];
        const float w1 = v0->overlayWeight;
        const float w0 = 1.0f - w1;
        const SwampDeformation* v1 = &primary[b];
        // !FAKE: inliner; hand form of the inlined `primary * w0 + secondary * w1` Vector3 operators; C0191: cl /O1
        // keeps them out of line, 30.29), stand-in temps v2..v9; secondary-first sums give retail's x87 spills and
        // temp slots (92.68 -> 97.66 -> 100.0)
        Vector3 v2(v0->translation.x * w1, v0->translation.y * w1, v0->translation.z * w1);
        Vector3 v3(v1->translation.x * w0, v1->translation.y * w0, v1->translation.z * w0);
        Vector3 translation(v2.x + v3.x, v2.y + v3.y, v2.z + v3.z);
        Vector3 v4(v0->scaling.x * w1, v0->scaling.y * w1, v0->scaling.z * w1);
        Vector3 v5(v1->scaling.x * w0, v1->scaling.y * w0, v1->scaling.z * w0);
        Vector3 scaling(v4.x + v5.x, v4.y + v5.y, v4.z + v5.z);
        Quaternion q1, q0;
        q0.Set(v1->rotation.x, v1->rotation.y, v1->rotation.z);
        q1.Set(v0->rotation.x, v0->rotation.y, v0->rotation.z);
        Quaternion q = Slerp(q0, q1, w1);
        Matrix rot;
        rot.Rotation(q);
        Vector3 zip;
        if (p->parentIndex != 0xFFFF) {
            const Vector3& v6 = secondary[p->parentIndex].scaling;
            Vector3 v7(v6.x * w1, v6.y * w1, v6.z * w1);
            const Vector3& v8 = primary[p->parentIndex].scaling;
            Vector3 v9(v8.x * w0, v8.y * w0, v8.z * w0);
            zip = Vector3(v7.x + v9.x, v7.y + v9.y, v7.z + v9.z);
        } else {
            zip.x = 1.0f;
            zip.y = 1.0f;
            zip.z = 1.0f;
        }
        CalculateBoneDeformation(matrices, b, scaling, translation, rot, zip);
    }
    ConvertDeformationsToModelSpace(matrices, bonesToProcess);
}

// FUNCTION: USA 0x000BB140
// PROTOTYPE: PREVIEW 0x004A5E7C identical
// NAME: proto-pdb:default.pdb
void ModelResource::ApplyDeformationsToSkeletion(std::vector<Matrix>& matrices,
                                                 const std::vector<SwampDeformation>& primary,
                                                 const std::vector<SwampDeformation>& secondary, float tween,
                                                 unsigned int boneCount) const
{
    if (!_gator)
        return;
    unsigned int bonesToProcess = _gator->numBones;
    if (boneCount)
        bonesToProcess = boneCount;
    for (unsigned int b = 0; b < bonesToProcess; b++) {
        // !FAKE: c1xx front end; neutral stand-in locals p/v0/v1; the proto PDB lists none for the bone or the
        // deformations
        const GatorBone* p = &_gator->GetBones()[b];
        const SwampDeformation* v0 = &secondary[b];
        const float w1 = 1.0f - tween;
        const SwampDeformation* v1 = &primary[b];
        // !FAKE: inliner; hand form of the inlined `primary * tween + secondary * w1` Vector3 operators; C0191: cl
        // /O1 keeps them out of line), stand-in temps v2..v9 (same form as the two-vector overload)
        Vector3 v2(v0->translation.x * w1, v0->translation.y * w1, v0->translation.z * w1);
        Vector3 v3(v1->translation.x * tween, v1->translation.y * tween, v1->translation.z * tween);
        Vector3 translation(v2.x + v3.x, v2.y + v3.y, v2.z + v3.z);
        Vector3 v4(v0->scaling.x * w1, v0->scaling.y * w1, v0->scaling.z * w1);
        Vector3 v5(v1->scaling.x * tween, v1->scaling.y * tween, v1->scaling.z * tween);
        Vector3 scaling(v4.x + v5.x, v4.y + v5.y, v4.z + v5.z);
        Quaternion q1, q0;
        q0.Set(v1->rotation.x, v1->rotation.y, v1->rotation.z);
        q1.Set(v0->rotation.x, v0->rotation.y, v0->rotation.z);
        Quaternion q = Slerp(q0, q1, w1);
        Matrix rot;
        rot.Rotation(q);
        Vector3 zip;
        if (p->parentIndex != 0xFFFF) {
            const Vector3& v6 = secondary[p->parentIndex].scaling;
            Vector3 v7(v6.x * w1, v6.y * w1, v6.z * w1);
            const Vector3& v8 = primary[p->parentIndex].scaling;
            Vector3 v9(v8.x * tween, v8.y * tween, v8.z * tween);
            zip = Vector3(v7.x + v9.x, v7.y + v9.y, v7.z + v9.z);
        } else {
            zip.x = 1.0f;
            zip.y = 1.0f;
            zip.z = 1.0f;
        }
        CalculateBoneDeformation(matrices, b, scaling, translation, rot, zip);
    }
    ConvertDeformationsToModelSpace(matrices, bonesToProcess);
}

class BundleWrapper;

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
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
    HottieRawData GetDataInBundle(const SwampString&);
    typedef std::multimap<SwampString, T*> NameMap;
    struct ResourceBundle { // NAME: proto-pdb:default.pdb
        Ptr<BundleWrapper> _bundle;    // 0x00
        bool _dontDiscard;             // 0x04
    };

    ResourceManager();
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

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
private:
    ModelManager();
};
DECOMP_SIZE_ASSERT(ModelManager, 0x58);

// FUNCTION: USA 0x000BB36E
// PROTOTYPE: PREVIEW 0x004A60AA identical
// NAME: proto-pdb:default.pdb
void ModelResource::ProcessDeformations(const Matrix& world, const std::vector<Matrix>* deformations) const
{
    // !FAKE: c1xx front end; neutral stand-in counters v0/v1; the proto PDB lists none for the transpose loops
    for (int bone = 0; bone < _gator->numRenderBones; bone++) {
        Matrix bm;
        (*deformations)[bone].AffineMultiply(world, bm);
        for (int v0 = 0; v0 < 3; v0++)
            for (int v1 = 0; v1 < 4; v1++)
                _vShaderBones[bone * 3 + v0][v1] = bm[v1][v0];
    }
}

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

// FUNCTION: USA 0x000BB44A
// PROTOTYPE: PREVIEW 0x004A6186 identical
// NAME: proto-pdb:default.pdb
template bool std::vector<bool,std::allocator<bool> >::operator[](unsigned int)const;

// FUNCTION: USA 0x000BBA33
// PROTOTYPE: PREVIEW 0x004A678A identical
// NAME: proto-pdb:default.pdb
void ModelResource::SetStripControl(std::vector<bool>& stripControl, const SwampString& name, bool on) const
{
    if (!_gator || !_gator->numReferences)
        return;
    // !FAKE: c1xx front end; neutral stand-in local p; the proto PDB lists no local for the references
    const GatorReference* p = _gator->GetReferences();
    unsigned int i;
    if (name.empty()) {
        for (i = 0; i < _gator->numReferences; i++)
            stripControl[p[i].index] = on;
    } else {
        unsigned int strIndex = (unsigned int)-1;
        for (i = 0; i < _gator->numReferences; i++) {
            if (!_stricmp(name.c_str(), _gator->GetString(p[i].name))) {
                strIndex = p[i].name;
                break;
            }
        }
        if (strIndex == (unsigned int)-1)
            return;
        for (i = 0; i < _gator->numReferences; i++) {
            if (p[i].name == strIndex)
                stripControl[p[i].index] = on;
        }
    }
}

// FUNCTION: USA 0x000BBC64
// PROTOTYPE: PREVIEW 0x004A69BB identical
// NAME: proto-pdb:default.pdb
template std::vector<TextureOverrides::Override,std::allocator<TextureOverrides::Override> >::iterator std::vector<TextureOverrides::Override,std::allocator<TextureOverrides::Override> >::erase(std::vector<TextureOverrides::Override,std::allocator<TextureOverrides::Override> >::iterator);

// FUNCTION: USA 0x000BCB8B
// PROTOTYPE: PREVIEW 0x004A78E2 identical
// NAME: proto-pdb:default.pdb
ModelManager::ModelManager()
{
    SetExtension(".gator");
    ModelResource::InitShaders();
}

// FUNCTION: USA 0x000BD188
// PROTOTYPE: PREVIEW 0x004A7EDF identical
// NAME: proto-pdb:default.pdb
template void std::vector<ResourceHandle<MaterialResource>,std::allocator<ResourceHandle<MaterialResource> > >::resize(unsigned int,ResourceHandle<MaterialResource>);

// FUNCTION: USA 0x000BD1E7
// PROTOTYPE: PREVIEW 0x004A7F3E identical
// NAME: proto-pdb:default.pdb
template void std::vector<TextureOverrides::Override,std::allocator<TextureOverrides::Override> >::resize(unsigned int,TextureOverrides::Override);

// FUNCTION: USA 0x000BD250
// PROTOTYPE: PREVIEW 0x004A7FA7 identical
// NAME: proto-pdb:default.pdb
template void std::vector<TextureOverrides::Override,std::allocator<TextureOverrides::Override> >::resize(unsigned int);

// FUNCTION: USA 0x000BD267
// NAME: proto-pdb:default.pdb
void ModelResource::LoadMaterials()
{
    SwampString basename = BeepLib::StripExtension(BeepLib::StripPath(GetName()));
    _materials.resize(_gator->numMaterials);
    for (unsigned int i = 0; i < _gator->numMaterials; ++i) {
        _materials[i] = g_swamp->GetMaterialManager().CreateMaterial(basename, *_gator, i);
        ResourceHandle<TextureResource> tex = _materials[i]().GetBaseTexture();
        if (tex().IsAnimated())
            _animatedTexture = tex;
        if (tex().UsesAlpha())
            _usesAlpha = true;
        tex = _materials[i]().GetDetailTexture();
        if (tex.IsValid() && tex().IsAnimated())
            _animatedTexture = tex;
    }
    for (i = 0; i < _gator->numStrips; ++i) {
        if (_gator->GetStrips()[i].render_flags & (GatorStrip::MIRROR_VINCE | GatorStrip::MIRROR_WORLD)) {
            _hasMirrors = true;
            break;
        }
    }
}

extern "C" void __stdcall XGSetVertexBufferHeader(unsigned int Length, unsigned long Usage, unsigned long FVF, int Pool,
                                                  D3DVertexBuffer* pBuffer, unsigned int Data); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DResource_Register(D3DVertexBuffer* pThis, void* pBase); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall XPhysicalFree(void*); // NAME: proto-pdb:default.pdb
void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000BD419
// PROTOTYPE: PREVIEW 0x004A8170 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.17, bytes mismatch (masked 1): C0192 wall, _XPhysicalFree@4 0x0001785A has only the ghidra name; frame 0x34 vs 0x30 (new-temp slot apart from offset)
ModelResource::ModelResource(const SwampString& name)
    : ResourceBase(name), _gator(0), _hasBSPs(false), _usesAlpha(false), _hasMirrors(false), _myGator(0)
{
    if (!_gator) {
        SwampString shortname = BeepLib::StripPath(name);
        HottieRawData rawData = g_swamp->GetModelManager().GetDataInBundle(shortname);
        _gator = (const GatorFile*)rawData.data;
    }
    if (_gator) {
        // !FAKE: c2 block layout; redundant `_gator &&` test; failure block first (C0244; early return 86.24 -> 99.17)
        if (_gator && (_gator->id != 0x52544147 || _gator->version != 0x1A)) {
            if (_myGator)
                XPhysicalFree(_myGator);
            _myGator = 0;
            _gator = 0;
        } else {
            XGSetVertexBufferHeader(0, 0, 0, 0, &_vertexBuffer, 0);
            D3DResource_Register(&_vertexBuffer, (char*)_gator + _gator->offsetVertices);
            LoadMaterials();
            _bsps.reserve(_gator->numBones);
            for (unsigned int b = 0; b < _gator->numBones; b++) {
                unsigned int offset = _gator->GetBones()[b].offsetBSP;
                if (offset) {
                    _bsps.push_back(new ("SwampModelResource.cpp", 120) BSP((char*)_gator + offset, false));
                    _hasBSPs = true;
                } else {
                    _bsps.push_back(0);
                }
            }
            _valid = true;
        }
    }
}
#endif
