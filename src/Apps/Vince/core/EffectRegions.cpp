// UNIT: src/Apps/Vince/core/EffectRegions.cpp OBJ: core\XBox\OfficialRelease\EffectRegions.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, (unsigned int)-1); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    const E* c_str() const { return _Myptr(); }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

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
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
DECOMP_SIZE_ASSERT(SwampString, 0x1C);

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb

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

protected:
    SwampScript(SwampSceneNode*);

public:
    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
    Vector2() {}
    Vector2(float u_, float v_) { u = u_; v = v_; }
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    void AddChild(SwampSceneNode*);
    void SetOption(unsigned int, bool, bool);
    bool GetOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }

    char field_4[0x88 - 0x4];          // 0x04
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetSize() const { return _size; }

    char field_A4[0xC0 - 0xA4];        // 0xA4
    Vector3 _size;                     // 0xC0
    char field_CC[0x124 - 0xCC];       // 0xCC
};
DECOMP_SIZE_ASSERT(RegionTrigger, 0x124);

class HeatShimmer : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    HeatShimmer(const Vector3&);
    void SetNoiseTextureScaling(unsigned int i, const Vector2& s) { _noiseTextureScaling[i] = s; }
    void SetNoiseTextureOffsetRate(unsigned int i, const Vector2& r) { _noiseTextureOffsetRate[i] = r; }
    void SetNoisePerturbRange(unsigned int i, const Vector2& r) { _noisePerturbRange[i] = r; }
    const Vector2& GetNoiseTextureScaling(unsigned int i) const { return _noiseTextureScaling[i]; }
    const Vector2& GetNoiseTextureOffsetRate(unsigned int i) const { return _noiseTextureOffsetRate[i]; }
    const Vector2& GetNoisePerturbRange(unsigned int i) const { return _noisePerturbRange[i]; }

    Vector3 _ellipsoidRadius;          // 0xA4
    Vector2 _noiseTextureScaling[2];   // 0xB0
    Vector2 _noiseTextureOffsetRate[2]; // 0xC0
    Vector2 _noisePerturbRange[2];     // 0xD0
};
DECOMP_SIZE_ASSERT(HeatShimmer, 0xE0);

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
    bool GetBoolRecord(const SwampString&, bool) const;
};
DECOMP_SIZE_ASSERT(BeepTDF::Record, 0x50);

void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb
extern "C" int __cdecl sscanf(const char*, const char*, ...); // NAME: proto-pdb:default.pdb

class ShimmerRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger* GetRegion();
    HeatShimmer* GetShimmer();
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

    HeatShimmer* _shimmer;             // 0x4C
};
DECOMP_SIZE_ASSERT(ShimmerRegionMain, 0x50);

inline RegionTrigger* ShimmerRegionMain::GetRegion()
{
    return (RegionTrigger*)_node;
}

inline HeatShimmer* ShimmerRegionMain::GetShimmer()
{
    return _shimmer;
}

// FUNCTION: USA 0x0012A156
// PROTOTYPE: PREVIEW 0x00512BC7 identical
// NAME: proto-pdb:default.pdb
void ShimmerRegionMain::Load(const BeepTDF& tdf)
{
    RegionTrigger* region = GetRegion();
    _shimmer = new("core\\EffectRegions.cpp", 0x20) HeatShimmer(region->GetSize());
    region->AddChild(_shimmer);
    // !FAKE: c1xx front end; dummy local; retail destroys the key string before SetOption (match_one 89.30 -> 100.0)
    bool v0 = tdf.GetBoolRecord("shimmeron", true);
    GetShimmer()->SetOption(2, !v0, false);
    const BeepTDF::Record* record = tdf.FindRecord("shimmerpass1", true);
    if (record) {
        Vector2 scale(1.0f, 1.0f);
        Vector2 rate(0.0f, -0.87f);
        Vector2 range(1.0f, 1.0f);
        sscanf(record->stringdata.c_str(), "%f %f %f %f %f %f", &scale.u, &scale.v, &rate.u, &rate.v, &range.u, &range.v);
        GetShimmer()->SetNoiseTextureScaling(0, scale);
        GetShimmer()->SetNoiseTextureOffsetRate(0, rate);
        GetShimmer()->SetNoisePerturbRange(0, range);
    }
    record = tdf.FindRecord("shimmerpass2", true);
    if (record) {
        Vector2 scale(10.0f, 10.0f);
        Vector2 rate(0.0f, 0.87f);
        Vector2 range(2.0f, 2.0f);
        sscanf(record->stringdata.c_str(), "%f %f %f %f %f %f", &scale.u, &scale.v, &rate.u, &rate.v, &range.u, &range.v);
        GetShimmer()->SetNoiseTextureScaling(1, scale);
        GetShimmer()->SetNoiseTextureOffsetRate(1, rate);
        GetShimmer()->SetNoisePerturbRange(1, range);
    }
}

// FUNCTION: USA 0x0012A364
// PROTOTYPE: PREVIEW 0x00512DD5 identical
// NAME: proto-pdb:default.pdb
SwampString ShimmerRegionMain::Save()
{
    SwampString s = SwampScript::Save();
    if (GetShimmer()) {
        s += format("shimmeron = %d;\n", !GetShimmer()->GetOption(2));
        // !FAKE: c2 register allocation; reference local; retail forms lea ecx,[eax+0xd0] for the range pair only (match_one 92.89 -> 100.0)
        const Vector2& v0 = GetShimmer()->GetNoisePerturbRange(0);
        s += format("shimmerpass1 = %f %f %f %f %f %f;\n",
                      GetShimmer()->GetNoiseTextureScaling(0).u, GetShimmer()->GetNoiseTextureScaling(0).v,
                      GetShimmer()->GetNoiseTextureOffsetRate(0).u, GetShimmer()->GetNoiseTextureOffsetRate(0).v,
                      v0.u, v0.v);
        // !FAKE: c2 register allocation; reference local; same form for pass 2 (lea ecx,[eax+0xd8])
        const Vector2& v1 = GetShimmer()->GetNoisePerturbRange(1);
        s += format("shimmerpass2 = %f %f %f %f %f %f;\n",
                      GetShimmer()->GetNoiseTextureScaling(1).u, GetShimmer()->GetNoiseTextureScaling(1).v,
                      GetShimmer()->GetNoiseTextureOffsetRate(1).u, GetShimmer()->GetNoiseTextureOffsetRate(1).v,
                      v1.u, v1.v);
    }
    return s;
}
