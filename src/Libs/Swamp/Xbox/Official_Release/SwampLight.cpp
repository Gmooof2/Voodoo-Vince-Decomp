// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampLight.cpp OBJ: Xbox\Official_Release\SwampLight.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#pragma warning(disable: 4530) // no /EH in the unit flags; as SwampGatorNode.cpp
#include <xstring>
#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specializations before <vector> (as MoleGatePuzzle.cpp, C0277); retail calls
// these out of line (0x000117DE, 0x00011655, 0x000111D6); the header bodies would emit them here (C0090)
namespace std {
template <> basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string(const char *);
template <> void basic_string<char,std::char_traits<char>,std::allocator<char> >::_Tidy(bool,unsigned int);
template <> basic_string<char,std::char_traits<char>,std::allocator<char> >& basic_string<char,std::char_traits<char>,std::allocator<char> >::assign(const basic_string<char,std::char_traits<char>,std::allocator<char> >&,unsigned int,unsigned int);
}
#endif
#include <vector>
#include <set>
#ifdef NON_MATCHING
#include <math.h> // sqrtf for the guarded ConsiderLight draft
#endif
#pragma intrinsic(memset) // retail inlines memset as stosd/rep stosd (IlluminationInfo::Initialize, C0153)

struct AABox;      // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb
struct Matrix;     // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    Vector4() {}
    Vector4(const Vector3& v) { Set(v); }
    // src retail 0x000CBB50 the 3-component Set forms store w = 1.0f (FillInfoBlock)
    void Set(const Vector3& v) { x = v.x; y = v.y; z = v.z; w = 1.0f; }
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; w = 1.0f; }
    void Set(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
    void Zero() { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00 std::string _Name
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

class AnimationResource; // NAME: proto-pdb:default.pdb
class LightNode;
class SwampSceneNode;

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(T* r) : _Resource(r) { _Resource->AddRef(); }
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource)
    {
        if (_Resource)
            _Resource->AddRef();
    }
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

    T* _Resource;                      // 0x00
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();

protected:
    AnimationControl(const ResourceHandle<AnimationResource>&, SwampSceneNode*, bool, bool, bool);
    friend class LightNode;

public:
    char field_24[0x44];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                         // vtable+0x00
    virtual const AABox& GetBounds() const;            // vtable+0x04
    virtual Vector3 GetWorldPosition() const;          // vtable+0x08
    virtual Quaternion GetWorldRotation() const;       // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);     // vtable+0x10
    virtual void Invalidate();                         // vtable+0x14
    virtual void UpdateAnimation(unsigned int pass);   // vtable+0x18
    virtual void Update();                             // vtable+0x1C
    virtual void Render(const Matrix&) const;          // vtable+0x20
    void SetName(const std::string& name) { _Name = name; }
    const Vector3& SetLocation(const Vector3&);
    bool GetRenderingOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    bool IsValid() const { return !GetRenderingOption(Invalid); }
    bool GetOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    bool IsVisible() const { return !GetOption(DontDraw); }

    enum NodeOptions { Invalid = 0x1, DontDraw = 0x2 }; // NAME: proto-pdb:default.pdb (enumerators not reached omitted)

    int _Type;                         // 0x04
    std::string _Name;                 // 0x08
    char field_24[0x88 - 0x24];        // 0x24 (_Parent .. _lastRenderFrame; not reached)
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

struct LightDefinition { // NAME: proto-pdb:default.pdb
    enum Type { AMBIENT = 0x0, POINT = 0x1, SPOTLIGHT = 0x2, DIRECTIONAL = 0x3 }; // NAME: proto-pdb:default.pdb

    // src retail 0x000CE565 the inlined ctor zeroes 0x12 dwords (rep stosd) in CreateDirectionalLight
    LightDefinition() { memset(this, 0, sizeof(LightDefinition)); }
    float GetIntensity() const;

    Type _type;                        // 0x00
    float _decay;                      // 0x04
    Vector3 _color;                    // 0x08
    Vector3 _shadowColor;              // 0x14
    float _range;                      // 0x20
    float _glowSize;                   // 0x24
    float _glowIntensity;              // 0x28
    float _ballFactor;                 // 0x2C
    Vector3 _direction;                // 0x30
    float _coneAngle;                  // 0x3C
    float _penumbraAngle;              // 0x40
    bool _affectsWorldGeometry;        // 0x44
    bool _affectsObjects;              // 0x45
    bool _castsShadows;                // 0x46
    bool _castsWorldShadows;           // 0x47
};
DECOMP_SIZE_ASSERT(LightDefinition, 0x48);

class SpotBeam { // NAME: proto-pdb:default.pdb
public:
    SpotBeam(float, float, float);
    ~SpotBeam();

    char field_0[0x50];                // 0x00 SpotlightGlowVertex _v[4]
    ResourceHandle<TextureResource> _spotBeamTexture; // 0x50
    float _intensity;                  // 0x54
};
DECOMP_SIZE_ASSERT(SpotBeam, 0x58);

class LightNode : public SwampSceneNode, public LightDefinition { // NAME: proto-pdb:default.pdb
public:
    LightNode(const LightDefinition& def);
    virtual void Update();
    LightDefinition::Type GetLightType() const { return _type; }
    const LightDefinition& GetDefinition() const { return *this; }
    const Vector3& GetCachedWorldPosition() const { return _worldPos; }
    float GetConeCos() const { return _coneCos; }
    float GetPenumbraCos() const { return _penumbraCos; }
    float GetCentimeterDecay() const { return _centimeterDecay; }
    ResourceHandle<AnimationControl> StartPlayback(const ResourceHandle<AnimationResource>& lgtPath, bool looped, bool reverse);
    void Enable();
    void Disable();
    void SetBallFactor(float factor);
    void UpdateCachedWorldPosition();

    Vector3 _worldPos;                 // 0xEC
    float _coneCos;                    // 0xF8
    float _penumbraCos;                // 0xFC
    float _centimeterDecay;            // 0x100
    char field_104[0x150 - 0x104];     // 0x104 (_shadowBuffer .. _shadowBufferRange; not reached)
    SpotBeam* _spotBeam;               // 0x150
    bool _enabled;                     // 0x154
    char field_155[0x158 - 0x155];     // 0x155
    ResourceHandle<AnimationControl> _currentPath; // 0x158
    float _speed;                      // 0x15C
};
DECOMP_SIZE_ASSERT(LightNode, 0x160);

struct IlluminationInfo { // NAME: proto-pdb:default.pdb
    struct LightInfo { // NAME: proto-pdb:default.pdb
        Vector4 position;              // 0x00
        Vector4 color;                 // 0x10
        Vector4 direction;             // 0x20
        Vector4 factors;               // 0x30
    };

    IlluminationInfo();
    void Initialize();

    LightInfo info[4];                 // 0x000
    const LightNode* source[4];        // 0x100
    Vector4 ambientColor;              // 0x110
    float ballFactor;                  // 0x120
    unsigned char ballLight;           // 0x124
    bool spotsUsed;                    // 0x125
    bool anyLights;                    // 0x126
};
DECOMP_SIZE_ASSERT(IlluminationInfo::LightInfo, 0x40);
DECOMP_SIZE_ASSERT(IlluminationInfo, 0x128);

struct GatorAnimationFile { // NAME: proto-pdb:default.pdb
    const char* GetString(unsigned int i) const
    {
        return (const char*)this + ((const unsigned int*)((const char*)this + offsetStringTable))[i] + offsetStrings;
    }

    unsigned int id;                   // 0x00
    unsigned int version;              // 0x04
    float duration;                    // 0x08
    unsigned int numTracks;            // 0x0C
    unsigned int numTriggers;          // 0x10
    unsigned int numStrings;           // 0x14
    unsigned int reserved;             // 0x18
    unsigned int offsetTracks;         // 0x1C
    unsigned int offsetTriggers;       // 0x20
    unsigned int offsetAuxInfo;        // 0x24
    unsigned int offsetStringTable;    // 0x28
    unsigned int offsetStrings;        // 0x2C
};
DECOMP_SIZE_ASSERT(GatorAnimationFile, 0x30);

struct GatorLightInfo { // NAME: proto-pdb:default.pdb
    unsigned short name;               // 0x00
    unsigned short instanceName;       // 0x02
    unsigned short type;               // 0x04
    unsigned short decay;              // 0x06
    bool lightObjects;                 // 0x08
    bool rtShadows;                    // 0x09
    bool rtWorldShadows;               // 0x0A
    bool animated;                     // 0x0B
    float intensity;                   // 0x0C
    float range;                       // 0x10
    float coneAngle;                   // 0x14
    float penumbraAngle;               // 0x18
    Vector3 location;                  // 0x1C
    Vector3 direction;                 // 0x28
    Vector3 color;                     // 0x34
    Vector3 shadowColor;               // 0x40
    float glowSize;                    // 0x4C
    float glowIntensity;               // 0x50
    unsigned int reserved[3];          // 0x54
};
DECOMP_SIZE_ASSERT(GatorLightInfo, 0x60);

class LightManager { // NAME: proto-pdb:default.pdb
public:
    struct LightLocation { // NAME: proto-pdb:default.pdb
        // FUNCTION: USA 0x000CB5BB
        // PROTOTYPE: PREVIEW 0x004B8142 identical
        // NAME: proto-pdb:default.pdb
        // !FAKE: c2 register allocation; inline callee; _Lbound/_Ubound/insert keep esi/edi across the call without
        // a same-unit register summary (C0128, C0251; _Lbound 87.3, _Ubound 87.9, insert 74.1 -> 100)
        bool operator<(const LightLocation& ll) const { return xPos < ll.xPos; }

        float xPos;                    // 0x00
        const LightNode* light;        // 0x04
    };

    static LightNode* CreateGatorLight(const GatorAnimationFile* anim, const GatorLightInfo* lightinfo);
    static LightNode* CreateDirectionalLight(const Vector3& direction, const Vector3& color);

protected:
    static void AddLight(const LightNode* light);
    static void RemoveLight(const LightNode* light);
    static void FillInfoBlock(IlluminationInfo& ii, const LightNode* light, unsigned int slot);

    friend class LightNode;
};
DECOMP_SIZE_ASSERT(LightManager::LightLocation, 0x8);

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> void vector<IlluminationInfo,allocator<IlluminationInfo> >::_Xlen()const;
template <> void vector<IlluminationInfo,allocator<IlluminationInfo> >::_Tidy();
template <> void vector<IlluminationInfo,allocator<IlluminationInfo> >::_Insert_n(iterator,size_type,const IlluminationInfo&);
template <> vector<IlluminationInfo,allocator<IlluminationInfo> >::iterator vector<IlluminationInfo,allocator<IlluminationInfo> >::erase(iterator,iterator);
template <> void _Uninitialized_fill_n<IlluminationInfo*,unsigned int,IlluminationInfo,allocator<IlluminationInfo> >(IlluminationInfo*,unsigned int,const IlluminationInfo&,allocator<IlluminationInfo>&);
template <> void _Tree<_Tset_traits<LightManager::LightLocation,less<LightManager::LightLocation>,allocator<LightManager::LightLocation>,1> >::const_iterator::_Dec();
template <> _Tree<_Tset_traits<LightManager::LightLocation,less<LightManager::LightLocation>,allocator<LightManager::LightLocation>,1> >::iterator _Tree<_Tset_traits<LightManager::LightLocation,less<LightManager::LightLocation>,allocator<LightManager::LightLocation>,1> >::_Insert(bool,_Nodeptr,const LightManager::LightLocation&);
}
#endif

struct UberIlluminationInfo { // NAME: proto-pdb:default.pdb
    UberIlluminationInfo();
    ~UberIlluminationInfo();
    unsigned int GetIICount() const;
    IlluminationInfo& GetII(unsigned int i);
    IlluminationInfo& GetLastII();
    void AddII();

    IlluminationInfo _ii;              // 0x000
    std::vector<IlluminationInfo> _iis; // 0x128
};
DECOMP_SIZE_ASSERT(UberIlluminationInfo, 0x138);

class PotentialLights { // NAME: proto-pdb:default.pdb
public:
    PotentialLights(const Vector3& loc, float radius);
    void ConsiderLight(const LightNode* light);
    void PlaceLight(const LightNode* light, const float approxDist);
    void PromoteSpotlights();

    const LightNode* _lights[4];       // 0x00
    float _distanceToLight[4];         // 0x10
    const Vector3& _center;            // 0x20
    float _radius;                     // 0x24
};
DECOMP_SIZE_ASSERT(PotentialLights, 0x28);

void* operator new(unsigned int size, const char* file, int line);

// FUNCTION: USA 0x000CB583
// PROTOTYPE: PREVIEW 0x004B810A identical
// NAME: proto-pdb:default.pdb
// !FAKE: c2 register allocation; inline callee; FillInfoBlock keeps ii/light in edx across the call without a
// same-unit register summary (C0128, C0251)
inline float LightDefinition::GetIntensity() const
{
    return (_color.x > _color.y ? _color.x : _color.y) > _color.z ? (_color.x > _color.y ? _color.x : _color.y)
                                                                  : _color.z;
}

// FUNCTION: USA 0x000CB5D4
// PROTOTYPE: PREVIEW 0x004B815B identical
// NAME: proto-pdb:default.pdb
PotentialLights::PotentialLights(const Vector3& loc, float radius) : _center(loc), _radius(radius)
{
    for (unsigned int i = 0; i < 4; i++) {
        _lights[i] = 0;
        _distanceToLight[i] = 3.402823466e+38f; // src retail 0x000CB5E6 __real@7f7fffff
    }
}

// FUNCTION: USA 0x000CB5FD
// PROTOTYPE: PREVIEW 0x004B8184 identical
// NAME: proto-pdb:default.pdb
void PotentialLights::PlaceLight(const LightNode* light, const float approxDist)
{
    for (unsigned int i = 0; i < 4; i++) {
        if (approxDist < _distanceToLight[i]) {
            for (unsigned int j = 3; j != i; j--) {
                _lights[j] = _lights[j - 1];
                _distanceToLight[j] = _distanceToLight[j - 1];
            }
            _lights[i] = light;
            _distanceToLight[i] = approxDist;
            return;
        }
    }
}

// FUNCTION: USA 0x000CB64D
// PROTOTYPE: PREVIEW 0x004B81D4 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<IlluminationInfo,std::allocator<IlluminationInfo> >::size()const;

// FUNCTION: USA 0x000CB665
// PROTOTYPE: PREVIEW 0x004B825C identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<IlluminationInfo,std::allocator<IlluminationInfo> >::capacity()const;

// FUNCTION: USA 0x000CB6AF
// PROTOTYPE: PREVIEW 0x004B8274 identical
// NAME: proto-pdb:default.pdb
template void std::fill<IlluminationInfo *,IlluminationInfo>(IlluminationInfo *,IlluminationInfo *,IlluminationInfo const &);

// FUNCTION: USA 0x000CB6F7
// PROTOTYPE: PREVIEW 0x004B8299 identical
// NAME: proto-pdb:default.pdb
template IlluminationInfo * std::_Copy_opt<IlluminationInfo *,IlluminationInfo *>(IlluminationInfo *,IlluminationInfo *,IlluminationInfo *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000CB724
// PROTOTYPE: PREVIEW 0x004B82C6 identical
// NAME: proto-pdb:default.pdb
template IlluminationInfo * std::_Copy_backward_opt<IlluminationInfo *,IlluminationInfo *>(IlluminationInfo *,IlluminationInfo *,IlluminationInfo *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000CB7A1
// PROTOTYPE: PREVIEW 0x004B8343 identical
// NAME: proto-pdb:default.pdb
SpotBeam::~SpotBeam()
{
}

// FUNCTION: USA 0x000CB7AC
// PROTOTYPE: PREVIEW 0x004B834E identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 76.65, bytes mismatch; wall: retail's sqrt is inline asm (rsqrtss + fcompi/fcmovbe/ffree),
// D116 forbids __asm (C0219); the asm also gives ballFactor/dist2 memory homes (push ecx frame) ours keeps in x87
void PotentialLights::ConsiderLight(const LightNode* light)
{
    if (light->IsValid() && light->IsVisible()) {
        switch (light->GetLightType()) {
        case LightDefinition::POINT:
        case LightDefinition::SPOTLIGHT: {
            const float ballFactor = light->_ballFactor;
            // !FAKE: stand-in local; the PDB lists none
            float range = light->_range;
            if (ballFactor != 0.0f)
                range *= ballFactor;
            // !FAKE: stand-in local; hand form of the inlined LengthSquared of the difference (D78)
            const Vector3& p = light->_worldPos;
            Vector3 d;
            d.x = p.x - _center.x;
            d.y = p.y - _center.y;
            d.z = p.z - _center.z;
            const float dist2 = d.x * d.x + d.y * d.y + d.z * d.z;
            // !FAKE: stand-in local; the PDB lists none
            float r = (range + _radius) * 1.05f;
            if (!(dist2 >= r * r))
                PlaceLight(light, ballFactor != 0.0f ? 0.0f : sqrtf(dist2));
            break;
        }
        default:
            PlaceLight(light, -light->GetIntensity());
            break;
        }
    }
}
#endif

// FUNCTION: USA 0x000CB8AF
// PROTOTYPE: PREVIEW 0x004B8451 identical
// NAME: proto-pdb:default.pdb
void PotentialLights::PromoteSpotlights()
{
    unsigned int v0 = 1;
    while (v0 < 3 && _lights[v0] && _lights[v0]->GetLightType() == LightDefinition::SPOTLIGHT)
        v0++;
    for (unsigned int i = v0 + 1; i < 4; i++) {
        if (_lights[i] && _lights[i]->GetLightType() == LightDefinition::SPOTLIGHT) {
            std::swap(_lights[i], _lights[v0]);
            v0 = i;
        }
    }
}

// FUNCTION: USA 0x000CB90A
// PROTOTYPE: PREVIEW 0x004B84AC identical
// NAME: proto-pdb:default.pdb
void IlluminationInfo::Initialize()
{
    ambientColor.Zero();
    spotsUsed = false;
    ballFactor = 0.0f;
    anyLights = false;
    memset(source, 0, sizeof(source));
}

// FUNCTION: USA 0x000CB948
// PROTOTYPE: PREVIEW 0x004B84EA identical
// NAME: proto-pdb:default.pdb
unsigned int UberIlluminationInfo::GetIICount() const
{
    if (_iis.empty())
        return 1;
    return _iis.size();
}

// FUNCTION: USA 0x000CB965
// PROTOTYPE: PREVIEW 0x004B8507 identical
// NAME: proto-pdb:default.pdb
void LightManager::FillInfoBlock(IlluminationInfo& ii, const LightNode* light, unsigned int slot)
{
    // !FAKE: stand-in local; the PDB lists none (retail ebx = &ii.info[slot] before the null test)
    IlluminationInfo::LightInfo& info = ii.info[slot];
    if (light) {
        ii.source[slot] = light;
        const LightDefinition& def = light->GetDefinition();
        switch (def._type) {
        case LightDefinition::POINT:
            info.position.Set(light->GetCachedWorldPosition());
            info.color.Set(def._color);
            info.color.w = def.GetIntensity();
            info.direction.Set(0.0f, 0.0f, 1.0f);
            info.factors.Set(-1.0f, -1.0f, 1.0f, def._decay);
            {
                // !FAKE: inliner; stand-in locals; hand form of the inlined Vector4::operator*= (C0125: /O1 calls it);
                // the pointer stands for its this: w is stored and reloaded for the compare (match_one 97.96 -> 100)
                const float decay = light->GetCentimeterDecay();
                Vector4* c = &info.color;
                c->x *= decay;
                c->y *= decay;
                c->z *= decay;
                c->w *= decay;
            }
            if (def._ballFactor != 0.0f
                && (ii.ballFactor == 0.0f || !(info.color.w < ii.info[ii.ballLight].color.w))) {
                ii.ballFactor = def._ballFactor;
                ii.ballLight = slot;
            }
            break;
        case LightDefinition::SPOTLIGHT: {
            Quaternion rot = light->GetWorldRotation();
            // !FAKE: stand-in local; hand form of the inlined value Apply (C0125: /O1 keeps it out of line)
            Vector3 tmp;
            rot.Apply(def._direction, tmp);
            Vector3 dir = tmp;
            info.direction.Set(dir);
            info.position.Set(light->GetCachedWorldPosition());
            info.color.Set(def._color);
            info.color.w = def.GetIntensity();
            info.factors.Set(light->GetConeCos(), light->GetPenumbraCos(), 1.0f, def._decay);
            {
                // !FAKE: inliner; stand-in local; hand form of the inlined Vector4::operator*= (C0125: /O1 calls it)
                const float decay = light->GetCentimeterDecay();
                info.color.x *= decay;
                info.color.y *= decay;
                info.color.z *= decay;
                info.color.w *= decay;
            }
            if (light->GetConeCos() != light->GetPenumbraCos())
                info.factors.z = 1.0f / (light->GetConeCos() - light->GetPenumbraCos());
            ii.spotsUsed = true;
            break;
        }
        case LightDefinition::DIRECTIONAL: {
            Quaternion rot = light->GetWorldRotation();
            // !FAKE: stand-in local; hand form of the inlined value Apply (C0125: /O1 keeps it out of line)
            Vector3 tmp;
            rot.Apply(def._direction, tmp);
            Vector3 dir = tmp;
            // !FAKE: stand-in local; hand form of the inlined dir * -10000.0f (no PDB record of a free operator, D78)
            Vector3 p;
            p.x = dir.x * -10000.0f;
            p.y = dir.y * -10000.0f;
            p.z = dir.z * -10000.0f;
            info.position.Set(p);
            info.color.Set(def._color);
            info.color.w = def.GetIntensity();
            info.direction.Set(0.0f, 0.0f, 1.0f);
            info.factors.Set(-1.0f, -1.0f, 1.0f, 0.0f);
            break;
        }
        }
        info.position.w = def._range * def._range;
        ii.anyLights = true;
    } else {
        info.position.Set(1000.0f, 1000.0f, 1000.0f);
        info.color.Zero();
        info.direction.Set(0.0f, 0.0f, 1.0f);
        info.factors.Set(-1.0f, -1.0f, 1.0f, 0.0f);
    }
}

// FUNCTION: USA 0x000CBC9D
// PROTOTYPE: PREVIEW 0x004B8881 identical
// NAME: proto-pdb:default.pdb
template LightManager::LightLocation const & std::reverse_iterator<std::_Tree<std::_Tset_traits<LightManager::LightLocation,std::less<LightManager::LightLocation>,std::allocator<LightManager::LightLocation>,1> >::const_iterator>::operator*()const;

// FUNCTION: USA 0x000CBCB6
// PROTOTYPE: PREVIEW 0x004B889A identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tset_traits<LightManager::LightLocation,std::less<LightManager::LightLocation>,std::allocator<LightManager::LightLocation>,1> >::_Node * std::_Tree<std::_Tset_traits<LightManager::LightLocation,std::less<LightManager::LightLocation>,std::allocator<LightManager::LightLocation>,1> >::_Ubound(LightManager::LightLocation const &)const;

// FUNCTION: USA 0x000CBD4F
// PROTOTYPE: PREVIEW 0x004B894D identical
// NAME: proto-pdb:default.pdb
template void std::_Construct<IlluminationInfo,IlluminationInfo>(IlluminationInfo *,IlluminationInfo const &);

// FUNCTION: USA 0x000CBD65
// PROTOTYPE: PREVIEW 0x004B8963 identical
// NAME: proto-pdb:default.pdb
// SwampCamera::StartPlayback form (0x000CB3E3); ??4?$ResourceHandle@VAnimationControl@@ rides Generator.obj 0x0003B04D
ResourceHandle<AnimationControl> LightNode::StartPlayback(const ResourceHandle<AnimationResource>& lgtPath, bool looped, bool reverse)
{
    if (!lgtPath._Resource) {
        _currentPath = ResourceHandle<AnimationControl>();
    } else {
        // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
        _currentPath = ResourceHandle<AnimationControl>(new("SwampLight.cpp", 221) AnimationControl(lgtPath, 0, looped, reverse, false));
    }
    return _currentPath;
}

// FUNCTION: USA 0x000CBDED
// PROTOTYPE: PREVIEW 0x004B89EB identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tset_traits<LightManager::LightLocation,std::less<LightManager::LightLocation>,std::allocator<LightManager::LightLocation>,1> >::_Node * std::_Tree<std::_Tset_traits<LightManager::LightLocation,std::less<LightManager::LightLocation>,std::allocator<LightManager::LightLocation>,1> >::_Lbound(LightManager::LightLocation const &)const;

// FUNCTION: USA 0x000CC4B1
// PROTOTYPE: PREVIEW 0x004B90AF identical
// NAME: proto-pdb:default.pdb
IlluminationInfo& UberIlluminationInfo::GetII(unsigned int i)
{
    if (_iis.empty())
        return _ii;
    return _iis[i];
}

// FUNCTION: USA 0x000CC594
// PROTOTYPE: PREVIEW 0x004B9192 identical
// NAME: proto-pdb:default.pdb
IlluminationInfo& UberIlluminationInfo::GetLastII()
{
    return GetII(GetIICount() - 1);
}

// FUNCTION: USA 0x000CC5F7
// PROTOTYPE: PREVIEW 0x004B91F5 identical
// NAME: proto-pdb:default.pdb
template IlluminationInfo * std::vector<IlluminationInfo,std::allocator<IlluminationInfo> >::_Ufill(IlluminationInfo *,unsigned int,IlluminationInfo const &);

// FUNCTION: USA 0x000CCA30
// PROTOTYPE: PREVIEW 0x004B962E identical
// NAME: proto-pdb:default.pdb
UberIlluminationInfo::~UberIlluminationInfo()
{
}

// FUNCTION: USA 0x000CD0E1
// PROTOTYPE: PREVIEW 0x004B9CDF identical
// NAME: proto-pdb:default.pdb
// VC 7.1 <vector> body verbatim as an explicit specialization: the header's _Xlen cannot be redeclared noreturn
// (C2381), so `__assume(0)` restates its throw body's effect (C0237, !FAKE) and no throw-info is emitted
template <> bool std::vector<IlluminationInfo,std::allocator<IlluminationInfo> >::_Buy(size_type _Capacity)
{
    _Myfirst = 0, _Mylast = 0, _Myend = 0;
    if (_Capacity == 0)
        return (false);
    else if (max_size() < _Capacity) {
        _Xlen();
        __assume(0); // !FAKE: noreturn restated (C0237)
    } else {
        _Myfirst = this->_Alval.allocate(_Capacity);
        _Mylast = _Myfirst;
        _Myend = _Myfirst + _Capacity;
    }
    return (true);
}

// FUNCTION: USA 0x000CD12A
// PROTOTYPE: PREVIEW 0x004B9D28 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::_Tree<std::_Tset_traits<LightManager::LightLocation,std::less<LightManager::LightLocation>,std::allocator<LightManager::LightLocation>,1> >::iterator,bool> std::_Tree<std::_Tset_traits<LightManager::LightLocation,std::less<LightManager::LightLocation>,std::allocator<LightManager::LightLocation>,1> >::insert(LightManager::LightLocation const &);

// FUNCTION: USA 0x000CD586
// PROTOTYPE: PREVIEW 0x004BA184 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 89.0, bytes mismatch (retail inlines ??_GSpotBeam: ??1 call, push ecx, delete; ours calls ??_G)
void LightNode::Disable()
{
    if (_enabled) {
        _enabled = false;
        LightManager::RemoveLight(this);
        if (_spotBeam) {
            delete _spotBeam;
            _spotBeam = 0;
        }
    }
}
#endif

// FUNCTION: USA 0x000CD5BB
// PROTOTYPE: PREVIEW 0x004BA1B9 identical
// NAME: proto-pdb:default.pdb
template void std::vector<IlluminationInfo,std::allocator<IlluminationInfo> >::resize(unsigned int,IlluminationInfo);

// FUNCTION: USA 0x000CD68A
// PROTOTYPE: PREVIEW 0x004BA288 identical
// NAME: proto-pdb:default.pdb
UberIlluminationInfo::UberIlluminationInfo()
{
    _ii.Initialize();
}

// FUNCTION: USA 0x000CD7E0
// PROTOTYPE: PREVIEW 0x004BA3DE identical
// NAME: proto-pdb:default.pdb
template void std::vector<IlluminationInfo,std::allocator<IlluminationInfo> >::resize(unsigned int);

// FUNCTION: USA 0x000CD875
// PROTOTYPE: PREVIEW 0x004BA473 identical
// NAME: proto-pdb:default.pdb
void LightNode::Enable()
{
    if (!_enabled) {
        _enabled = true;
        LightManager::AddLight(this);
        if (_type == LightDefinition::SPOTLIGHT && _glowSize > 0.0f)
            // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
            _spotBeam = new("SwampLight.cpp", 152) SpotBeam(_penumbraAngle * 0.1f + _coneAngle * 0.9f, _glowSize, _glowIntensity);
    }
}

// FUNCTION: USA 0x000CD90C
// PROTOTYPE: PREVIEW 0x004BA50A identical
// NAME: proto-pdb:default.pdb
void LightNode::SetBallFactor(float factor)
{
    // !FAKE: c2 register allocation; dummy locals; bool byte compare (cmp cl,al) needs them, the inline
    // expression compares ints (match_one 84.43 -> 100)
    bool v0 = _ballFactor != 0.0f;
    bool v1 = factor != 0.0f;
    if (v0 != v1) {
        LightManager::RemoveLight(this);
        _ballFactor = factor;
        LightManager::AddLight(this);
    } else
        _ballFactor = factor;
}

// FUNCTION: USA 0x000CD96F
// PROTOTYPE: PREVIEW 0x004BA56D identical
// NAME: proto-pdb:default.pdb
void LightNode::UpdateCachedWorldPosition()
{
    Vector3 newPos = GetWorldPosition();
    if (newPos.x != _worldPos.x) {
        LightManager::RemoveLight(this);
        _worldPos = newPos;
        LightManager::AddLight(this);
    }
}

// FUNCTION: USA 0x000CD9B2
// PROTOTYPE: PREVIEW 0x004BA5B0 identical
// NAME: proto-pdb:default.pdb
void LightNode::Update()
{
    if (_enabled) {
        LightDefinition::Type t = GetLightType();
        if (t == LightDefinition::POINT || t == LightDefinition::SPOTLIGHT)
            UpdateCachedWorldPosition();
    }
}

// FUNCTION: USA 0x000CD9D1
// PROTOTYPE: PREVIEW 0x004BA5CF identical
// NAME: proto-pdb:default.pdb
void UberIlluminationInfo::AddII()
{
    _iis.resize(_iis.size() + 1);
    _iis.back().Initialize();
}

// FUNCTION: USA 0x000CE44F
// PROTOTYPE: PREVIEW 0x004BB04D identical
// NAME: proto-pdb:default.pdb
LightNode* LightManager::CreateGatorLight(const GatorAnimationFile* anim, const GatorLightInfo* lightinfo)
{
    LightDefinition def;
    // !FAKE: stand-in locals; hand form of the inlined color * intensity (no PDB record of a free operator, D78);
    // the float local keeps intensity in st(0) as retail (match_one 92.97 -> 100)
    const float v1 = lightinfo->intensity;
    Vector3 v0;
    v0.x = v1 * lightinfo->color.x;
    v0.y = v1 * lightinfo->color.y;
    v0.z = v1 * lightinfo->color.z;
    def._type = (LightDefinition::Type)lightinfo->type;
    def._direction = lightinfo->direction;
    def._color = v0;
    def._shadowColor = lightinfo->shadowColor;
    def._decay = lightinfo->decay;
    def._range = lightinfo->range;
    def._coneAngle = lightinfo->coneAngle;
    def._penumbraAngle = lightinfo->penumbraAngle;
    def._affectsObjects = lightinfo->lightObjects;
    def._glowIntensity = lightinfo->glowIntensity;
    def._glowSize = lightinfo->glowSize;
    def._castsShadows = lightinfo->rtShadows;
    def._affectsWorldGeometry = true;
    def._castsWorldShadows = lightinfo->rtWorldShadows;
    // !FAKE: stand-in local; the PDB lists none
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    LightNode* light = new("SwampLight.cpp", 1205) LightNode(def);
    light->SetLocation(lightinfo->location);
    light->SetName(anim->GetString(lightinfo->name));
    return light;
}

// FUNCTION: USA 0x000CE558
// PROTOTYPE: PREVIEW 0x004BB156 identical
// NAME: proto-pdb:default.pdb
LightNode* LightManager::CreateDirectionalLight(const Vector3& direction, const Vector3& color)
{
    LightDefinition def;
    def._decay = 0.0f;
    def._type = LightDefinition::DIRECTIONAL;
    def._direction = direction;
    def._color = color;
    def._range = 3.402823466e+38f; // src retail 0x000CE597 0x7f7fffff
    def._affectsObjects = true;
    def._affectsWorldGeometry = false;
    def._castsShadows = false;
    def._castsWorldShadows = false;
    return new("SwampLight.cpp", 1229) LightNode(def); // src retail 0x000CE588 line 0x4CD, "SwampLight.cpp"
}
