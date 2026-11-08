// UNIT: src/Apps/Vince/core/WaterRegion.cpp OBJ: core\XBox\OfficialRelease\WaterRegion.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> inline void _Destroy(T* _Ptr) { (_Ptr)->~T(); }
template <class T> class allocator {
public:
    void destroy(T* _Ptr) { _Destroy(_Ptr); }
};
template <class _InIt, class _OutIt> _OutIt copy(_InIt _First, _InIt _Last, _OutIt _Dest);
template <class _Ty, class _Alloc> void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al);
struct _Nonscalar_ptr_iterator_tag {};
template <class _Ty, class _Alloc> inline void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_First)
        _Al.destroy(_First);
}
template <class _FwdIt, class _Ty> inline void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}
template <class _InIt, class _OutIt> inline _OutIt _Copy_opt(_InIt _First, _InIt _Last, _OutIt _Dest, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_Dest, ++_First)
        *_Dest = *_First;
    return (_Dest);
}
template <class _BidIt1, class _BidIt2> inline _BidIt2 _Copy_backward_opt(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Nonscalar_ptr_iterator_tag)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}
template <class E, class T, class A> class basic_string {
public:
    basic_string() { _Myres = 15; _Eos(0); }
    basic_string(const E* ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, npos); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    int compare(const basic_string& _Right) const;
    const E* c_str() const { return _Myptr(); }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);
    void _Eos(unsigned int _Newsize);

private:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    char _Alval;                       // 0x00
    union {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres; // 0x18
};
template <class E, class T, class A> inline bool operator==(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right)
{
    return (_Left.compare(_Right) == 0);
}
template <class T, class A = allocator<T> > class vector {
public:
    class const_iterator {
    public:
        const_iterator(T* ptr) : _Myptr(ptr) {}
        const T& operator*() const { return (*_Myptr); }
        bool operator==(const const_iterator& _Right) const { return (_Myptr == _Right._Myptr); }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
        T* _Myptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(T* ptr) : const_iterator(ptr) {}
        T& operator*() const { return ((T&)**(const_iterator*)this); }
        T* operator->() const { return (&**this); }
        iterator& operator++() { ++this->_Myptr; return (*this); }
    };
    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; }
    ~vector();
    iterator begin() { return (iterator(_Myfirst)); }
    iterator end() { return (iterator(_Mylast)); }
    unsigned int size() const;
    void push_back(const T& _Val);
    iterator erase(iterator _Where)
    {
        copy(_Where._Myptr + 1, _Mylast, _Where._Myptr);
        _Destroy(_Mylast - 1, _Mylast);
        --_Mylast;
        return (_Where);
    }

protected:
    void _Destroy(T* _First, T* _Last) { _Destroy_range(_First, _Last, this->_Alval); }

private:
    A _Alval;                          // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

inline void* operator new(unsigned int, void* p) { return p; }

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class SwampPuddle;           // NAME: proto-pdb:default.pdb
class RegionTrigger;         // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum CollisionType { // NAME: proto-pdb:default.pdb
        HorizontalCollision = 0,
        VerticalCollision = 1
    };
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04
    virtual void Activate();                                       // vtable+0x08
    virtual void Deactivate();                                     // vtable+0x0C
    virtual void FrameTick();                                      // vtable+0x10
    virtual void TimerEvent(unsigned int);                         // vtable+0x14
    virtual void AnimationFinished();                              // vtable+0x18
    virtual void SecondaryAnimationFinished();                     // vtable+0x1C
    virtual void AnimationInterrupted();                           // vtable+0x20
    virtual void SecondaryAnimationInterrupted();                  // vtable+0x24
    virtual void AnimationTrigger(const SwampString&, float, float, unsigned int); // vtable+0x28
    virtual bool LosingControl(const SwampString&);                // vtable+0x2C
    virtual void GainingControl(SwampScript*);                     // vtable+0x30
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    virtual void EnteringRegion(SwampSceneNode*);                  // vtable+0x38
    virtual void LeavingRegion(SwampSceneNode*);                   // vtable+0x3C
    virtual void SoundFinished(const SwampString&);                // vtable+0x40
    virtual void SoundStreamerFinished(const SwampString&);        // vtable+0x44
    virtual void MovieFinished(const SwampString&);                // vtable+0x48
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x50
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual SwampString Save();                                    // vtable+0x58
    virtual void SetNode(SwampSceneNode*);                         // vtable+0x5C
    virtual void InvalidateScript();                               // vtable+0x60
    void RegisterFrameTick();
    void UnRegisterFrameTick();

protected:
    SwampScript(SwampSceneNode*);

public:
    char field_4[0x1C];               // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    Vector3() {}
    Vector3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
    Quaternion() {}
    Quaternion(float x, float y, float z, float w_) { v.x = x; v.y = y; v.z = z; w = w_; }
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct AABox;                // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    void AddChild(SwampSceneNode*);
    const Vector3& SetLocation(const Vector3&);
    const Quaternion& SetRotation(const Quaternion&);
    void SetOption(unsigned int, bool, bool);

    int _Type;                         // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x40 - 0x24];        // 0x24
    Quaternion _Rotation;              // 0x40
    char field_50[0xA4 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
    Vector2() {}
    Vector2(float u_, float v_) { u = u_; v = v_; }
    void Set(float u_, float v_) { u = u_; v = v_; }
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

class PuddleRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    struct Splasher;                   // NAME: proto-pdb:default.pdb

    PuddleRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
    virtual void FrameTick();
    RegionTrigger* GetRegion();
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

private:
    void RemoveCritterFromSplashList(const SwampString&);

    SwampPuddle* _water;               // 0x4C
    float _perturbSplash;              // 0x50
    float _perturbMove;                // 0x54
    float _perturbStill;               // 0x58
    int _saveMatId;                    // 0x5C
    float _saveDensity;                // 0x60
    int _saveFieldsize;                // 0x64
    std::vector<Splasher> _splashers;  // 0x68
    unsigned int _nextID;              // 0x78
};
DECOMP_SIZE_ASSERT(PuddleRegionMain, 0x7C);

// FUNCTION: USA 0x0012A5D1
// PROTOTYPE: PREVIEW 0x00512FF6 identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012B449
// PROTOTYPE: PREVIEW 0x00513E6E identical
// NAME: proto-pdb:default.pdb
struct PuddleRegionMain::Splasher { // NAME: proto-pdb:default.pdb
    SwampString name;                  // 0x00
    unsigned int id;                   // 0x1C
    Vector2 lastPos;                   // 0x20
};
DECOMP_SIZE_ASSERT(PuddleRegionMain::Splasher, 0x28);

// FUNCTION: USA 0x0012B92C
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; wall: relink refuses: ??_GPuddleRegionMain (0x0012BC8C) calls ~vector<Splasher> 0x0012B64E, unnamed: external ??1?$vector@USplasher@PuddleRegionMain@@V?$allocator@USplasher@PuddleRegionMain@@@std@@@std@@QAE@XZ does not resolve by name (4.4 T4.c9)
PuddleRegionMain::PuddleRegionMain(SwampSceneNode* node) : SwampScript(node), _water(0)
{
    _nextID = 0;
}
#endif

// FUNCTION: USA 0x0012B955
// PROTOTYPE: PREVIEW 0x0051437A identical
// NAME: proto-pdb:default.pdb
void PuddleRegionMain::RemoveCritterFromSplashList(const SwampString& name)
{
    for (std::vector<Splasher>::iterator it = _splashers.begin(); it != _splashers.end(); ++it) {
        if (it->name == name) {
            _splashers.erase(it);
            break;
        }
    }
    if (_splashers.size() == 0) {
        UnRegisterFrameTick();
    }
}

// FUNCTION: USA 0x0012BCB2
// PROTOTYPE: PREVIEW 0x005146D7 identical
// NAME: proto-pdb:default.pdb
void PuddleRegionMain::EnteringRegion(SwampSceneNode* node)
{
    RemoveCritterFromSplashList(node->_Name);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit Splasher copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Splasher_copy(void* p, const PuddleRegionMain::Splasher& v)
{
    new (p) PuddleRegionMain::Splasher(v);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit Splasher::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Splasher_assign(PuddleRegionMain::Splasher* p, const PuddleRegionMain::Splasher& v)
{
    *p = v;
}

// FUNCTION: USA 0x0012BD02
// PROTOTYPE: PREVIEW 0x00514727 identical
// NAME: proto-pdb:default.pdb
void PuddleRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (_splashers.size() == 0) {
        RegisterFrameTick();
    }
    Splasher splasher;
    splasher.name = node->_Name;
    splasher.id = _nextID++;
    splasher.lastPos.Set(3.4028235e38f, 3.4028235e38f);
    _splashers.push_back(splasher);
}

enum RegionShape { // NAME: proto-pdb:default.pdb
    REGION_AABB = 0,
    REGION_PLANE = 1
};

struct AABox { // NAME: proto-pdb:default.pdb
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetPlane(const Vector3& size);

    RegionShape _shape;                // 0xA4
    AABox _bounds;                     // 0xA8
    Vector3 _size;                     // 0xC0
    Quaternion _unrot;                 // 0xCC
    char field_DC[0x120 - 0xDC];       // 0xDC
    bool _enabled;                     // 0x120
    char field_121[0x124 - 0x121];     // 0x121
};
DECOMP_SIZE_ASSERT(RegionTrigger, 0x124);

// FUNCTION: USA 0x0012A4D6
// NAME: proto-pdb:default.pdb
void RegionTrigger::SetPlane(const Vector3& size)
{
    _size = size;
    // !FAKE: inliner; hand form of the inlined Quaternion inverse (C0191), stand-in locals p/v0/v1; the sum through
    // a named local gives retail's fstp st(4) (71.64 without it), the reads through p place the _shape store late
    // (96.72 -> 100.0)
    const Quaternion* p = &_Rotation;
    float v0 = p->v.x * p->v.x + p->v.y * p->v.y + p->v.z * p->v.z + p->w * p->w;
    float v1 = FastReciprocalSquareRoot(v0);
    _unrot = Quaternion(-(v1 * p->v.x), -(v1 * p->v.y), -(v1 * p->v.z), v1 * p->w);
    _shape = REGION_PLANE;
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class MaterialResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle&);
    bool IsValid() const { return _Resource != 0; }
    T* Ptr() { return _Resource; }

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class MaterialManager : public ResourceManager<MaterialResource> { // NAME: proto-pdb:default.pdb
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    const AABox& GetBounds() const;
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x24];                // 0x00
    MaterialManager& GetMaterialManager() const { return *_materialManager; }
    ModelManager& GetModelManager() const { return *_modelManager; }

    MaterialManager* _materialManager; // 0x24
    ModelManager* _modelManager;       // 0x28
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    struct Record { // NAME: proto-pdb:default.pdb
        SwampString name;              // 0x00
        SwampString stringdata;        // 0x1C
        int intdata;                   // 0x38
        double floatdata;              // 0x40
        bool booldata;                 // 0x48
    };
    const Record* FindRecord(const SwampString&, bool) const;
    int GetIntRecord(const SwampString&, int) const;
    double GetFloatRecord(const SwampString&, double) const;
    const SwampString GetStringRecord(const SwampString&, const SwampString&) const;
};
DECOMP_SIZE_ASSERT(BeepTDF::Record, 0x50);

struct Vector4 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
    Vector4() {}
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

class WaterField { // NAME: proto-pdb:default.pdb
public:
    void SetTurbulence(float t) { _turbulence = t; }
    void SetWaveSpeed(float s) { _speed = s; }
    void SetElasticity(float e) { _elasticity = e; }
    void SetVerticalness(float v) { _verticalness = v; }
    void SetColor(const Vector4& c) { _color = c; }
    float GetTurbulence() const { return _turbulence; }
    float GetWaveSpeed() const { return _speed; }
    float GetElasticity() const { return _elasticity; }
    float GetVerticalness() const { return _verticalness; }
    const Vector4& GetColor() const { return _color; }
    void InitSurfaceDistortions();

    char field_0[0x1C];                // 0x00
    float _turbulence;                 // 0x1C
    float _speed;                      // 0x20
    float _elasticity;                 // 0x24
    float _verticalness;               // 0x28
    char field_2C[0x34 - 0x2C];        // 0x2C
    Vector4 _color;                    // 0x34
};
DECOMP_SIZE_ASSERT(WaterField, 0x44);

class SwampPuddle : public SwampSceneNode, public WaterField { // NAME: proto-pdb:default.pdb
public:
    SwampPuddle(const Vector2&, const ResourceHandle<MaterialResource>&, const Vector3&, float, unsigned int);

    char field_E8[0x1B8 - 0xE8];       // 0xE8
};
DECOMP_SIZE_ASSERT(SwampPuddle, 0x1B8);

void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb
const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb
extern "C" int __cdecl sscanf(const char*, const char*, ...); // NAME: proto-pdb:default.pdb

class SwampWaterNode : public SwampSceneNode, public WaterField { // NAME: proto-pdb:default.pdb
public:
    SwampWaterNode(const ResourceHandle<ModelResource>&, const ResourceHandle<MaterialResource>&, float, unsigned int);

    float _density;                    // 0xE8
    ResourceHandle<MaterialResource> _mtl; // 0xEC
    ResourceHandle<ModelResource> _model; // 0xF0
    char field_F4[0xF8 - 0xF4];        // 0xF4
};
DECOMP_SIZE_ASSERT(SwampWaterNode, 0xF8);

// FUNCTION: USA 0x0012A5A7
// PROTOTYPE: PREVIEW 0x00512FCC identical
// NAME: proto-pdb:default.pdb
class WaterRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    WaterRegionMain(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
    RegionTrigger* GetRegion();
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

    SwampWaterNode* _water;            // 0x4C
    int _saveMatId;                    // 0x50
    float _saveSafeDepth;              // 0x54
    float _saveDensity;                // 0x58
    int _saveFieldsize;                // 0x5C
    SwampString _saveModel;            // 0x60
};
DECOMP_SIZE_ASSERT(WaterRegionMain, 0x7C);

inline RegionTrigger* WaterRegionMain::GetRegion()
{
    return (RegionTrigger*)_node;
}

// FUNCTION: USA 0x0012A563
// PROTOTYPE: PREVIEW 0x00512FA8 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match, and with it ??_GWaterRegionMain 100.0 bytes match (T9.c2); wall: relink
// refuses: vtable slot +0x3C 0x0012AC83 (?LeavingRegion@WaterRegionMain, proto PDB) has no symbols/pin row:
// `external ?LeavingRegion@WaterRegionMain@@UAEXPAVSwampSceneNode@@@Z does not resolve by name`
WaterRegionMain::WaterRegionMain(SwampSceneNode* node) : SwampScript(node), _water(0)
{
}
#endif

// FUNCTION: USA 0x0012A5F8
// PROTOTYPE: PREVIEW 0x0051301D identical
// NAME: proto-pdb:default.pdb
void WaterRegionMain::Load(const BeepTDF& tdf)
{
    RegionTrigger* region = GetRegion();
    ResourceHandle<MaterialResource> mtl;
    _saveMatId = tdf.GetIntRecord("materialid", 0);
    if (_saveMatId) {
        SwampString name = format("%d", _saveMatId);
        mtl = g_swamp->GetMaterialManager().Load(name);
    }
    _saveDensity = tdf.GetFloatRecord("waterdensity", 1.0f);
    _saveFieldsize = tdf.GetIntRecord("waterfieldsize", 32);
    ResourceHandle<ModelResource> model;
    _saveModel = tdf.GetStringRecord("model", "");
    model = g_swamp->GetModelManager().Load(_saveModel);
    _saveSafeDepth = tdf.GetFloatRecord("watersafedepth", 0.05f);
    _water = new("core\\WaterRegion.cpp", 56) SwampWaterNode(model, mtl, _saveDensity, _saveFieldsize);
    region->AddChild(_water);
    const BeepTDF::Record* colorRecord = tdf.FindRecord("color", true);
    if (colorRecord) {
        Vector4 clr;
        sscanf(colorRecord->stringdata.c_str(), "%f %f %f %f", &clr.x, &clr.y, &clr.z, &clr.w);
        _water->SetColor(clr);
    }
    float speed = tdf.GetFloatRecord("waterspeed", 0.5f);
    _water->SetWaveSpeed(speed);
    float turbulence = tdf.GetFloatRecord("waterturbulence", 0.004f);
    _water->SetTurbulence(turbulence);
    _water->InitSurfaceDistortions();
    float elasticity = tdf.GetFloatRecord("waterelasticity", 0.9992f);
    _water->SetElasticity(elasticity);
    float verticalness = tdf.GetFloatRecord("waterverticalness", 0.8f);
    _water->SetVerticalness(verticalness);
    if (model.IsValid()) {
        // !FAKE: inliner; stand-in pointer local; hand form of the inlined AABox size/centre (C0191), one GetBounds
        // call in eax
        const AABox* p = &model.Ptr()->GetBounds();
        Vector3 size;
        size.x = p->high.x - p->low.x;
        size.y = p->high.y - p->low.y;
        size.z = p->high.z - p->low.z;
        Vector3 loc;
        loc.x = size.x * 0.5f + p->low.x;
        loc.y = size.y * 0.5f + p->low.y;
        loc.z = size.z * 0.5f + p->low.z;
        region->SetPlane(size);
        region->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        // !FAKE: inliner; stand-in locals v0/v1; hand form of the inlined `loc + Vector3(0, -d, 0)` and
        // `Vector3(0, d, 0) - loc` (C0191): cl folds the 0.0f parts and loads the depth first, as retail
        Vector3 v0(0.0f, -_saveSafeDepth, 0.0f);
        region->SetLocation(Vector3(loc.x + v0.x, loc.y + v0.y, loc.z + v0.z));
        Vector3 v1(0.0f, _saveSafeDepth, 0.0f);
        _water->SetLocation(Vector3(v1.x - loc.x, v1.y - loc.y, v1.z - loc.z));
    } else {
        _water->SetOption(2, true, false);
        region->_enabled = false;
    }
}

// FUNCTION: USA 0x0012AA37
// PROTOTYPE: PREVIEW 0x0051345C identical
// NAME: proto-pdb:default.pdb
SwampString WaterRegionMain::Save()
{
    SwampString s = SwampScript::Save();
    s += format("materialid = %d;\n", _saveMatId);
    s += format("waterdensity = %f;\n", _saveDensity);
    s += format("waterfieldsize = %d;\n", _saveFieldsize);
    s += format("model = %s;\n", _saveModel.c_str());
    s += format("watersafedepth = %f;\n", _saveSafeDepth);
    if (_water) {
        // !FAKE: c2 register allocation; stand-in reference local v0; retail holds &_color in eax, `add eax,0xd8`
        // (GetColor() per argument 98.05 -> 100.0)
        const Vector4& v0 = _water->GetColor();
        s += format("color = %f %f %f %f;\n", v0.x, v0.y, v0.z, v0.w);
        s += format("waterspeed = %f;\n", _water->GetWaveSpeed());
        s += format("waterturbulence = %f;\n", _water->GetTurbulence());
        s += format("waterelasticity = %f;\n", _water->GetElasticity());
        s += format("waterverticalness = %f;\n", _water->GetVerticalness());
    }
    return s;
}

inline RegionTrigger* PuddleRegionMain::GetRegion()
{
    return (RegionTrigger*)_node;
}

// FUNCTION: USA 0x0012AE76
// PROTOTYPE: PREVIEW 0x0051389B identical
// NAME: proto-pdb:default.pdb
void PuddleRegionMain::Load(const BeepTDF& tdf)
{
    // !FAKE: c2 register allocation; stand-in local; retail holds the region in esi from entry, no PDB record
    // (GetRegion() at each use 86.11)
    RegionTrigger* p = GetRegion();
    ResourceHandle<MaterialResource> mtl;
    _saveMatId = tdf.GetIntRecord("materialid", 0);
    if (_saveMatId) {
        SwampString name = format("%d", _saveMatId);
        mtl = g_swamp->GetMaterialManager().Load(name);
    }
    _saveDensity = tdf.GetFloatRecord("waterdensity", 1.0f);
    _saveFieldsize = tdf.GetIntRecord("waterfieldsize", 64);
    Vector3 snapshotLoc = p->GetWorldPosition();
    _water = new("core\\WaterRegion.cpp", 237) SwampPuddle(Vector2(p->_size.x, p->_size.z), mtl, snapshotLoc,
                                                            _saveDensity, _saveFieldsize);
    p->AddChild(_water);
    const BeepTDF::Record* colorRecord = tdf.FindRecord("color", true);
    if (colorRecord) {
        Vector4 clr;
        sscanf(colorRecord->stringdata.c_str(), "%f %f %f %f", &clr.x, &clr.y, &clr.z, &clr.w);
        _water->SetColor(clr);
    }
    const BeepTDF::Record* speed = tdf.FindRecord("waterspeed", true);
    if (speed) {
        _water->SetWaveSpeed(speed->floatdata);
    }
    const BeepTDF::Record* turbulence = tdf.FindRecord("waterturbulence", true);
    if (turbulence) {
        _water->SetTurbulence(turbulence->floatdata);
        // !FAKE: inliner; split statement; retail reloads _water for this call, InitSurfaceDistortions inside the
        // inline SetTurbulence body keeps it in ecx (96.80 -> 100.0)
        _water->InitSurfaceDistortions();
    }
    const BeepTDF::Record* elasticity = tdf.FindRecord("waterelasticity", true);
    if (elasticity) {
        _water->SetElasticity(elasticity->floatdata);
    }
    const BeepTDF::Record* verticalness = tdf.FindRecord("waterverticalness", true);
    if (verticalness) {
        _water->SetVerticalness(verticalness->floatdata);
    }
    _perturbSplash = tdf.GetFloatRecord("waterperturbsplash", 1.5);
    _perturbMove = tdf.GetFloatRecord("waterperturbmove", 1.75);
    _perturbStill = tdf.GetFloatRecord("waterperturbstill", 0.08f);
}

// FUNCTION: USA 0x0012B1D5
// PROTOTYPE: PREVIEW 0x00513BFA identical
// NAME: proto-pdb:default.pdb
SwampString PuddleRegionMain::Save()
{
    SwampString s = SwampScript::Save();
    s += format("materialid = %d;\n", _saveMatId);
    s += format("waterdensity = %f;\n", _saveDensity);
    s += format("waterfieldsize = %d;\n", _saveFieldsize);
    if (_water) {
        // !FAKE: c2 register allocation; stand-in reference local v0; retail holds &_color in eax, `add eax,0xd8`
        // (same form as WaterRegionMain::Save)
        const Vector4& v0 = _water->GetColor();
        s += format("color = %f %f %f %f;\n", v0.x, v0.y, v0.z, v0.w);
        s += format("waterspeed = %f;\n", _water->GetWaveSpeed());
        s += format("waterturbulence = %f;\n", _water->GetTurbulence());
        s += format("waterelasticity = %f;\n", _water->GetElasticity());
        s += format("waterverticalness = %f;\n", _water->GetVerticalness());
    }
    s += format("waterPerturbsplash = %f;\n", _perturbSplash);
    s += format("waterPerturbmove = %f;\n", _perturbMove);
    s += format("waterPerturbstill = %f;\n", _perturbStill);
    return s;
}

// FUNCTION: USA 0x0012B6B0
// PROTOTYPE: PREVIEW 0x005140D5 identical
// NAME: proto-pdb:default.pdb
template std::vector<PuddleRegionMain::Splasher,std::allocator<PuddleRegionMain::Splasher> >::iterator std::vector<PuddleRegionMain::Splasher,std::allocator<PuddleRegionMain::Splasher> >::erase(std::vector<PuddleRegionMain::Splasher,std::allocator<PuddleRegionMain::Splasher> >::iterator);

// FUNCTION: USA 0x0012B474
// PROTOTYPE: PREVIEW 0x00513E99 identical
// NAME: proto-pdb:default.pdb
template PuddleRegionMain::Splasher * std::_Copy_opt<PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher *>(PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0012B498
// PROTOTYPE: PREVIEW 0x00513EBD identical
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<PuddleRegionMain::Splasher,std::allocator<PuddleRegionMain::Splasher> >(PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher *,std::allocator<PuddleRegionMain::Splasher> &,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0012B4B3
// PROTOTYPE: PREVIEW 0x00513ED8 identical
// NAME: proto-pdb:default.pdb
template PuddleRegionMain::Splasher * std::_Copy_backward_opt<PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher *>(PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0012B590
// PROTOTYPE: PREVIEW 0x00513FB5 identical
// NAME: proto-pdb:default.pdb
template void std::fill<PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher>(PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher *,PuddleRegionMain::Splasher const &);
