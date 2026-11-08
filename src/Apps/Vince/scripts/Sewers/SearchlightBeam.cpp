// UNIT: src/Apps/Vince/scripts/Sewers/SearchlightBeam.cpp OBJ: Xbox\OfficialRelease\SearchlightBeam.obj OPT: /O1 /Oy
// SearchlightBeam: the PDB has two class records (0x148 with _lightSound, 0x144 without); retail's
// ctor ??0SearchlightBeam@@QAE@HABUQuaternion@@@Z matches the later 0x144 record, used here.
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct AABox;  // NAME: proto-pdb:default.pdb
struct Matrix; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion& operator*=(const Quaternion&);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class PixelShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    ModelManager& GetModelManager() const { return *_modelManager; }

    char field_0[0x28];                // 0x00
    ModelManager* _modelManager;       // 0x28
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
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

    const Vector3& SetLocation(const Vector3&);
    const Quaternion& SetRotation(const Quaternion&);
    SwampSceneNode* GetParent() { return _Parent; }

    int _Type;                         // 0x04
    char field_8[0x24 - 0x8];          // 0x08
    SwampSceneNode* _Parent;           // 0x24
    char field_28[0xA4 - 0x28];        // 0x28
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xAC - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0xB4 - 0xAC];        // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    void GetBoneOrientation(int, Quaternion&, Vector3&) const;

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

// FUNCTION: USA 0x000AB4C7
// NAME: proto-pdb:default.pdb
class FogZone : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    FogZone(const ResourceHandle<ModelResource>&, const Vector3&, float);
    virtual ~FogZone() {}                                          // vtable+0x00
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    ResourceHandle<ModelResource> _model;          // 0xA4
    char field_A8[0x118 - 0xA8];       // 0xA8
    unsigned long _destAlphaBlend;     // 0x118
    char field_11C[0x128 - 0x11C];     // 0x11C
    ResourceHandle<PixelShaderResource> _pshader;  // 0x128
    ResourceHandle<VertexShaderResource> _vshader; // 0x12C
};
DECOMP_SIZE_ASSERT(FogZone, 0x130);

class SearchlightBeam : public FogZone { // NAME: proto-pdb:default.pdb
public:
    SearchlightBeam(int parentBone, const Quaternion& baseRotation);
    virtual void Update();                                         // vtable+0x1C

    int _parentBone;                   // 0x130
    Quaternion _baseRotation;          // 0x134
};
DECOMP_SIZE_ASSERT(SearchlightBeam, 0x144);

// FUNCTION: USA 0x000AB4F9
// NAME: proto-pdb:default.pdb
SearchlightBeam::SearchlightBeam(int parentBone, const Quaternion& baseRotation)
    : FogZone(g_swamp->GetModelManager().Load("suspendedLight_base"), Vector3(0.8f, 1.0f, 0.5f), 1.25f),
      _parentBone(parentBone), _baseRotation(baseRotation)
{
    _destAlphaBlend = 1;
    _Type = 9;
}

// FUNCTION: USA 0x000AB463
// PROTOTYPE: PREVIEW 0x00496285 identical
// NAME: proto-pdb:default.pdb
void SearchlightBeam::Update()
{
    if (_parentBone >= 0) {
        Vector3 pos;
        Quaternion rot;
        static_cast<GatorNode*>(GetParent())->GetBoneOrientation(_parentBone, rot, pos);
        // !FAKE: inliner; hand form of the inline Quaternion operator* (copy, *=, by-value result); no before/after recorded
        Quaternion v1;
        {
            Quaternion v0 = rot;
            v0 *= _baseRotation;
            v1 = v0;
        }
        SetRotation(v1);
        SetLocation(pos);
    }
}

// !FAKE: c1xx front end; scaffold call; retail emits the inline ??1FogZone@@UAE@XZ in this unit (its users, the
// SearchlightBeam ctor and the folded ??_G, are unmatched); this qualified call odr-uses it (matched_set [[emitter]] row)
void pin_emit_FogZone_dtor(FogZone* p)
{
    p->FogZone::~FogZone();
}
