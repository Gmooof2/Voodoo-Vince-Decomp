// UNIT: src/Apps/Vince/scripts/Carnival/WheelOfMystery.cpp OBJ: Xbox\OfficialRelease\WheelOfMystery.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class T, class A = allocator<T> > class deque {
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

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

    SwampSceneNode* GetNode() { return _node; }
    void RegisterFrameTick();

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class Critter; // NAME: proto-pdb:default.pdb

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter();
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class ExtraLifeMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
};
DECOMP_SIZE_ASSERT(ExtraLifeMain, 0xBC);

class ExtraLifeOfMysteryMain : public ExtraLifeMain { // NAME: proto-pdb:default.pdb
public:
    ExtraLifeOfMysteryMain(SwampSceneNode* node);
    virtual void HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ExtraLifeOfMysteryMain, 0xBC);

// FUNCTION: USA 0x0003B7AB
// PROTOTYPE: PREVIEW 0x00429E6A identical
// NAME: proto-pdb:default.pdb
class WheelOfMysteryMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ONEMOREFROG = 0x14,
        MESSAGE_ONELESSFROG = 0x15
    };

    WheelOfMysteryMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void TimerEvent(unsigned int);
    virtual void Message(SwampScript* source, int message, void* data);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    float _wheelTime;                  // 0xBC
    float _maxWheelTime;               // 0xC0
    int _frogCount;                    // 0xC4
    SwampString _switchName;           // 0xC8
    SwampString _lastPrizeName;        // 0xE4
    int _spinIndex;                    // 0x100
    bool _didCoin;                     // 0x104
    char field_105[0x108 - 0x105];     // 0x105
};
DECOMP_SIZE_ASSERT(WheelOfMysteryMain, 0x108);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1WheelOfMysteryMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WheelOfMysteryMain_dtor(WheelOfMysteryMain* p)
{
    p->WheelOfMysteryMain::~WheelOfMysteryMain();
}

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
protected:
    char field_BC[0x11C - 0xBC];       // 0xBC
    SwampString _target;               // 0x11C
    char field_138[0x13C - 0x138];     // 0x138
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class switch_wheelOfMysteryMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    switch_wheelOfMysteryMain(SwampSceneNode* node);
    virtual bool TestActivate() const;
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(switch_wheelOfMysteryMain, 0x13C);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x110 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class WheelOfMysteryDoorsMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    WheelOfMysteryDoorsMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(WheelOfMysteryDoorsMain, 0x110);

// FUNCTION: USA 0x0003BEED
// PROTOTYPE: PREVIEW 0x0042A636 identical
// NAME: proto-pdb:default.pdb
SwampScript* ExtraLifeOfMysteryMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\WheelOfMystery.cpp", 319) ExtraLifeOfMysteryMain(node);
}

// FUNCTION: USA 0x0003BFAF
// PROTOTYPE: PREVIEW 0x0042A6F8 identical
// NAME: proto-pdb:default.pdb
SwampScript* WheelOfMysteryMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\WheelOfMystery.cpp", 32) WheelOfMysteryMain(node);
}

// FUNCTION: USA 0x0003BFD6
// PROTOTYPE: PREVIEW 0x0042A71F identical
// NAME: proto-pdb:default.pdb
SwampScript* switch_wheelOfMysteryMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\WheelOfMystery.cpp", 384) switch_wheelOfMysteryMain(node);
}

// FUNCTION: USA 0x0003C062
// PROTOTYPE: PREVIEW 0x0042A7AB identical
// NAME: proto-pdb:default.pdb
SwampScript* WheelOfMysteryDoorsMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\WheelOfMystery.cpp", 271) WheelOfMysteryDoorsMain(node);
}

class AABox;     // NAME: proto-pdb:default.pdb
class Quaternion; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1,
        DontDraw = 0x2
    };

    virtual ~SwampSceneNode();                         // vtable+0x00
    virtual const AABox& GetBounds() const;            // vtable+0x04
    virtual Vector3 GetWorldPosition() const;          // vtable+0x08
    virtual Quaternion GetWorldRotation() const;       // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);     // vtable+0x10
    virtual void Invalidate();                         // vtable+0x14

    SwampScript* GetScript() const;
    const SwampString& GetName() const { return _Name; }
    void SetOption(unsigned int option, bool value, bool recurse);
    bool GetRenderingOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    bool IsVisible() const { return !GetRenderingOption(DontDraw); }

    char field_4[0x8 - 0x4];           // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x88 - 0x24];        // 0x24
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// Retail layout: Critter byte proto 0x252 read at retail 0x260.
class Critter : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetSpecies(SwampString);
    void MakeDead(bool);

    char field_A4[0x260 - 0xA4];       // 0xA4
    bool field_260;                    // 0x260
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    void Remove(const SwampString&);
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

inline Critter* CritterMain::GetCritter() { return (Critter*)_node; }

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~MonsterMain();
    virtual void InvalidateScript();

    char field_BC[0x240 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x2C - 0x24];        // 0x24
    float _length;                     // 0x2C
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    T& operator()() { return *_Resource; }
    const T& operator()() const { return *_Resource; }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    std::deque<T> _queue;              // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

// FUNCTION: USA 0x0003C08C
// PROTOTYPE: PREVIEW 0x0042A7D5 identical
// NAME: proto-pdb:default.pdb
class PitFrogMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    PitFrogMain(SwampSceneNode* node);
    virtual void Message(SwampScript*, int, void*);

    MultiAnimation _multiAnimWindup;   // 0x240
    MultiAnimation _multiAnimAttackLeft; // 0x254
    MultiAnimation _multiAnimAttackRight; // 0x268
    MultiAnimation _multiAnimAttackStraight; // 0x27C
    MultiAnimation _multiAnimAttackUp; // 0x290
    MultiAnimation _multiAnimWindupAlt; // 0x2A4
    MultiAnimation _multiAnimAttackAlt; // 0x2B8
    MultiAnimation _multiAnimIdle;     // 0x2CC
    MultiAnimation _multiAnimIdle2;    // 0x2E0
    MultiAnimation _multiAnimIdleHurt; // 0x2F4
    MultiAnimation _multiAnimRun;      // 0x308
    MultiAnimation _multiAnimWalk;     // 0x31C
    ResourceHandle<AnimationResource> _animHoverLand[3]; // 0x330
    char field_33C[0x358 - 0x33C];     // 0x33C
    SwampString _patrolLoc;            // 0x358
    SwampString _pit;                  // 0x374
    char field_390[0x39C - 0x390];     // 0x390
};
DECOMP_SIZE_ASSERT(PitFrogMain, 0x39C);

// !FAKE: c1xx front end; scaffold call; retail's ??1PitFrogMain@@UAE@XZ is implicit (no vptr store, C0213); this
// qualified call odr-uses it so this unit emits it (needs a matched_set [[emitter]] row)
void pin_emit_PitFrogMain_dtor(PitFrogMain* p)
{
    p->PitFrogMain::~PitFrogMain();
}

class FrogOfMysteryMain : public PitFrogMain { // NAME: proto-pdb:default.pdb
public:
    FrogOfMysteryMain(SwampSceneNode* node);
    virtual ~FrogOfMysteryMain();
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void InvalidateScript();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(FrogOfMysteryMain, 0x39C);

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
    static void Return(float);
};

// FUNCTION: USA 0x0003B64F
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 20.0, bytes match (masked 0); wall: cut extent: the delink cut runs 72 B to the next external symbol, over the unnamed 57 B function at 0x0003B65E (LAB_0003b65e)
// re-screened 5.4 T12.1 (cut retail-2953b782): extent still 72 vs row 15; lift = a pin at 0x0003B65E + re-cut
void WheelOfMysteryMain::TimerEvent(unsigned int)
{
    AnimCam::Return(0.0f);
}
#endif

// FUNCTION: USA 0x0003B697
// PROTOTYPE: PREVIEW 0x00429D56 identical
// NAME: proto-pdb:default.pdb
void FrogOfMysteryMain::Message(SwampScript* source, int message, void* data)
{
    PitFrogMain::Message(source, message, data);
    switch (message) {
    case 2:
        GetCritter()->MakeDead(true);
        break;
    }
}

// FUNCTION: USA 0x0003B6D9
// PROTOTYPE: PREVIEW 0x00429D98 identical
// NAME: proto-pdb:default.pdb
void ExtraLifeOfMysteryMain::HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr)
{
    ExtraLifeMain::HitBy(type, critterHitBy, cr);
    if (!GetCritter()->IsVisible())
        GetCritter()->Invalidate();
}

// FUNCTION: USA 0x0003B707
// PROTOTYPE: PREVIEW 0x00429DC6 identical
// NAME: proto-pdb:default.pdb
bool switch_wheelOfMysteryMain::TestActivate() const
{
    return pManager->Find(_target)->GetScript()->TestActivate();
}

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim()._length; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x38 - 0x28];        // 0x28
    float _time;                       // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    void SetAnimSpeed(float speed);
    void OverrideTexture(const SwampString&, const SwampString&);
    ResourceHandle<AnimationControl> GetCurrentAnimation() const { return _currentAnim; }

    char field_0[0x140];               // 0x00
    ResourceHandle<AnimationControl> _currentAnim; // 0x140
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
}

// FUNCTION: USA 0x0003B82B
// PROTOTYPE: PREVIEW 0x00429EEA identical
// NAME: proto-pdb:default.pdb
void WheelOfMysteryMain::Activate()
{
    ((GatorNode*)GetNode())->SetAnimSpeed(BeepLib::RandomFloat(3.0f, 4.0f));
    RegisterFrameTick();
    _spinIndex = -1;
    AnimCam::SwitchTo("camera_wheelOfMystery", 0.0f, false, 0.0f);
    pManager->Remove(_lastPrizeName);
}

// FUNCTION: USA 0x0003B8BA
// PROTOTYPE: PREVIEW 0x00429F79 identical
// NAME: proto-pdb:default.pdb
void WheelOfMysteryMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 6: {
        ((GatorNode*)GetNode())->SetAnimSpeed(0.0f);
        // !FAKE: c2 register allocation; stand-in local; retail keeps the handle in edi, the PDB records no local
        ResourceHandle<AnimationControl> v1 = ((GatorNode*)GetNode())->GetCurrentAnimation();
        _maxWheelTime = v1().GetAnimationLength();
        v1().SetAnimationTime(0.0f);
        _frogCount = 0;
        if (!_didCoin) {
            Critter* coin = pManager->Find("mysterycoin");
            if (coin) {
                coin->SetOption(SwampSceneNode::DontDraw, true, false);
                ((GatorNode*)GetNode())->OverrideTexture("wheelmysterymidway800", "wheelmysterymidwaycoins800");
            }
        }
        break;
    }
    case 7:
        _switchName = source->GetNode()->GetName();
        break;
    case MESSAGE_ONEMOREFROG:
        if (++_frogCount == 1) {
            // !FAKE: c1xx front end; stand-in local; retail destroys the string temp before GetScript (C0223)
            Critter* v2 = pManager->Find("WheelOfMysteryDoors");
            v2->GetScript()->Message(this, 0x15, 0); // src proto-pdb:default.pdb MESSAGE_CLOSE
        }
        break;
    case MESSAGE_ONELESSFROG:
        if (--_frogCount == 0) {
            // !FAKE: c1xx front end; stand-in local; retail destroys the string temp before GetScript (C0223)
            Critter* v2 = pManager->Find("WheelOfMysteryDoors");
            v2->GetScript()->Message(this, 0x14, 0); // src proto-pdb:default.pdb MESSAGE_OPEN
        }
        break;
    }
    CritterMain::Message(source, message, data);
}

// FUNCTION: USA 0x0003BF17
// PROTOTYPE: PREVIEW 0x0042A660 identical
// NAME: proto-pdb:default.pdb
void FrogOfMysteryMain::InvalidateScript()
{
    // !FAKE: c1xx front end; neutral stand-in local; the PDB records no local for the found wheel (test and use)
    Critter* wheel = pManager->Find("WheelOfMystery");
    if (wheel)
        wheel->GetScript()->Message(this, 0x15, 0); // src proto-pdb:default.pdb MESSAGE_ONELESSFROG
    PitFrogMain::InvalidateScript();
}

// FUNCTION: USA 0x0003C202
// PROTOTYPE: PREVIEW 0x0042A94B near-identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1 = vptr store; needs pin ??_7FrogOfMysteryMain@@6B@ at
// 0x001CEC78); source is final; 5.4 T15.c3
FrogOfMysteryMain::FrogOfMysteryMain(SwampSceneNode* node) : PitFrogMain(node)
{
    Critter* wheel = pManager->Find("WheelOfMystery");
    wheel->GetScript()->Message(this, 0x14, 0); // src proto-pdb:default.pdb MESSAGE_ONEMOREFROG
    GetCritter()->SetSpecies("pitfrog");
    GetCritter()->field_260 = false;
}
#endif

// FUNCTION: USA 0x0003C2BE
// PROTOTYPE: PREVIEW 0x0042AA07 identical
// NAME: proto-pdb:default.pdb
SwampScript* FrogOfMysteryMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\WheelOfMystery.cpp", 338) FrogOfMysteryMain(node);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0003C14E
// NAME: proto-pdb:default.pdb
WheelOfMysteryMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WheelOfMysteryMain", creationFunc);
}

// FUNCTION: USA 0x0003C18A
// NAME: proto-pdb:default.pdb
WheelOfMysteryDoorsMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WheelOfMysteryDoorsMain", creationFunc);
}

// FUNCTION: USA 0x0003C1C6
// NAME: proto-pdb:default.pdb
ExtraLifeOfMysteryMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("ExtraLifeOfMysteryMain", creationFunc);
}

// FUNCTION: USA 0x0003C282
// NAME: proto-pdb:default.pdb
switch_wheelOfMysteryMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("switch_wheelOfMysteryMain", creationFunc);
}

// FUNCTION: USA 0x0003C2E8
// NAME: proto-pdb:default.pdb
FrogOfMysteryMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("FrogOfMysteryMain", creationFunc);
}
