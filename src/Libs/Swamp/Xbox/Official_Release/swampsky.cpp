// UNIT: src/Libs/Swamp/Xbox/Official_Release/swampsky.cpp OBJ: Xbox\Official_Release\SwampSky.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);

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
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

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

void InitVAF(_D3DVERTEXATTRIBUTEFORMAT& vaf); // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }
    const SwampString& GetName() const { return _Name; }

    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x60 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(ModelResource, 0x60);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    T& operator()() const { return *_Resource; }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<ModelResource>, 0x4);

struct AABox;      // NAME: proto-pdb:default.pdb
struct Vector3;    // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb
struct Matrix;     // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampSceneNode(int type);
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
public:
    void SetName(const SwampString& name) { _Name = name; }

    char field_4[0x4];                 // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampSky : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampSky(const ResourceHandle<ModelResource>& mh, const ResourceHandle<ModelResource>& mhCloudTop, const ResourceHandle<ModelResource>& mhCloudBottom);
    virtual ~SwampSky();                                           // vtable+0x00

    ResourceHandle<ModelResource> _skyModel;             // 0xA4
    ResourceHandle<ModelResource> _cloudLayerTopModel;   // 0xA8
    ResourceHandle<ModelResource> _cloudLayerBottomModel; // 0xAC
    bool _nightAndDay;                 // 0xB0
    float _nightDayScroll;             // 0xB4
    float _cloudVertOffset;            // 0xB8
    float _cloudSpeedUBottom;          // 0xBC
    float _cloudSpeedVBottom;          // 0xC0
    float _cloudSpeedUTop;             // 0xC4
    float _cloudSpeedVTop;             // 0xC8
    float _cloudTileCountU;            // 0xCC
    float _cloudTileCountV;            // 0xD0
private:
    static _D3DVERTEXATTRIBUTEFORMAT _vafSkyAndClouds;
};
DECOMP_SIZE_ASSERT(SwampSky, 0xD4);

// FUNCTION: USA 0x000D8494
// PROTOTYPE: PREVIEW 0x004C3A1B identical
// NAME: proto-pdb:default.pdb
SwampSky::~SwampSky()
{
}

// FUNCTION: USA 0x000D84C6
// PROTOTYPE: PREVIEW 0x004C3A4D identical
// NAME: proto-pdb:default.pdb
SwampSky::SwampSky(const ResourceHandle<ModelResource>& mh, const ResourceHandle<ModelResource>& mhCloudTop, const ResourceHandle<ModelResource>& mhCloudBottom)
    : SwampSceneNode(10), _skyModel(mh), _cloudLayerTopModel(mhCloudTop), _cloudLayerBottomModel(mhCloudBottom),
      _nightAndDay(false), _nightDayScroll(0.5f), _cloudVertOffset(1.0f), _cloudSpeedUBottom(0.5f), _cloudSpeedVBottom(0.5f),
      _cloudSpeedUTop(0.25f), _cloudSpeedVTop(0.25f), _cloudTileCountU(16.0f), _cloudTileCountV(16.0f)
{
    SetName(mh().GetName());
    InitVAF(_vafSkyAndClouds);
    _vafSkyAndClouds.Input[0].StreamIndex = 0;
    _vafSkyAndClouds.Input[0].Offset = 0;
    _vafSkyAndClouds.Input[0].Format = 0x32; // retail
    _vafSkyAndClouds.Input[1].StreamIndex = 0;
    _vafSkyAndClouds.Input[1].Offset = 0xC;
    _vafSkyAndClouds.Input[1].Format = 0x44; // retail
    _vafSkyAndClouds.Input[6].StreamIndex = 0;
    _vafSkyAndClouds.Input[6].Offset = 0x10;
    _vafSkyAndClouds.Input[6].Format = 0x44; // retail
    _vafSkyAndClouds.Input[2].StreamIndex = 0;
    _vafSkyAndClouds.Input[2].Offset = 0x14;
    _vafSkyAndClouds.Input[2].Format = 0x16; // retail
    _vafSkyAndClouds.Input[9].StreamIndex = 0;
    _vafSkyAndClouds.Input[9].Offset = 0x18;
    _vafSkyAndClouds.Input[9].Format = 0x21; // retail
    _vafSkyAndClouds.Input[10].StreamIndex = 0;
    _vafSkyAndClouds.Input[10].Offset = 0x1C;
    _vafSkyAndClouds.Input[10].Format = 0x21; // retail
}
