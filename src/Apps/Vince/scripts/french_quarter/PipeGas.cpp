// UNIT: src/Apps/Vince/scripts/french_quarter/PipeGas.cpp OBJ: Xbox\OfficialRelease\PipeGas.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SoundNode;             // NAME: proto-pdb:default.pdb
class FireFlyNode;           // NAME: proto-pdb:default.pdb

struct Vector2 { // NAME: proto-pdb:default.pdb
    Vector2(float u_, float v_) : u(u_), v(v_) {}
    float u;                           // 0x00
    float v;                           // 0x04
};

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class HeatShimmer : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    HeatShimmer(const Vector3& radius);
    void SetNoiseTextureScaling(unsigned int i, const Vector2& v) { _noiseTextureScaling[i] = v; }
    void SetNoiseTextureOffsetRate(unsigned int i, const Vector2& v) { _noiseTextureOffsetRate[i] = v; }

    Vector3 _ellipsoidRadius;          // 0xA4
    Vector2 _noiseTextureScaling[2];   // 0xB0
    Vector2 _noiseTextureOffsetRate[2]; // 0xC0
    Vector2 _noisePerturbRange[2];     // 0xD0
};
DECOMP_SIZE_ASSERT(HeatShimmer, 0xE0);

void* operator new(unsigned int size, const char* file, int line);

class ModelResource { // NAME: proto-pdb:default.pdb
public:
    int GetBoneIndex(const SwampString& name, bool b) const;
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    const T& operator()() const { return *_Resource; }

    T* _Resource;                      // 0x00
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    const ResourceHandle<ModelResource>& GetModel() const { return _model; }

    char field_A4[0x10];               // 0xA4
    ResourceHandle<ModelResource> _model; // 0xB4
};

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_DUST = 0x10
};

// Retail layout: Critter _CritterType 0x258 -> 0x268 (+0x10).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_B8[0x268 - 0xB8];       // 0xB8
    CritterType _CritterType;          // 0x268
};

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
    void RegisterSaveLoadVar(const SwampString& name, bool* addr, bool defaultval);

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);

    char field_4C[0xBC - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class PipeGasMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_QUERY_BURNING = 0x14
    };

    PipeGasMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);

private:
    static SwampScript* creationFunc(SwampSceneNode* node);

protected:
    void TurnOnFlame();

    FireFlyNode* _emitter;             // 0xBC
    int _pipeJointId;                  // 0xC0
    bool _lit;                         // 0xC4
    SoundNode* _sound;                 // 0xC8
};
DECOMP_SIZE_ASSERT(PipeGasMain, 0xCC);

// FUNCTION: USA 0x000815F5
// PROTOTYPE: PREVIEW 0x0046D4E7 identical
// NAME: proto-pdb:default.pdb
void PipeGasMain::Message(SwampScript* source, int message, void* data)
{
    if (message != MESSAGE_QUERY_BURNING) {
        CritterMain::Message(source, message, data);
        return;
    }
    *(bool*)data = _lit;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked = vptr store, literals)
// FUNCTION: USA 0x00081613
// PROTOTYPE: PREVIEW 0x0046D505 near-identical
// NAME: proto-pdb:default.pdb
SwampSceneNode* CreateFlameHeatShimmer()
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    HeatShimmer* shimmer = new("mainstreet\\PipeGas.cpp", 152) HeatShimmer(Vector3(0.4f, 0.4f, 0.4f));
    shimmer->SetNoiseTextureScaling(0, Vector2(1.0f, 1.0f));
    shimmer->SetNoiseTextureScaling(1, Vector2(1.5f, 1.5f));
    shimmer->SetNoiseTextureOffsetRate(0, Vector2(0.0f, 0.0f));
    shimmer->SetNoiseTextureOffsetRate(1, Vector2(0.0f, -0.9f));
    return shimmer;
}
#endif

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked = vptr store, literals)
// FUNCTION: USA 0x000816C8
// PROTOTYPE: PREVIEW 0x0046D5BA near-identical
// NAME: proto-pdb:default.pdb
PipeGasMain::PipeGasMain(SwampSceneNode* node) : CritterMain(node), _emitter(0), _pipeJointId(-1), _lit(false), _sound(0)
{
    ((Critter*)_node)->_CritterType = CRITTER_TYPE_DUST;
    RegisterFrameTick();
    RegisterSaveLoadVar("lit", &_lit, false);
    _pipeJointId = ((Critter*)_node)->GetModel()().GetBoneIndex("locator_gas", false);
}
#endif

// FUNCTION: USA 0x00081929
// PROTOTYPE: PREVIEW 0x0046D81B identical
// NAME: proto-pdb:default.pdb
SwampScript* PipeGasMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\mainstreet\\PipeGas.h", 11) PipeGasMain(node);
}

// FUNCTION: USA 0x00081B78
// PROTOTYPE: PREVIEW 0x0046DA37 identical
// NAME: proto-pdb:default.pdb
void PipeGasMain::Load(const BeepTDF& tdf)
{
    CritterMain::Load(tdf);
    TurnOnFlame();
}
