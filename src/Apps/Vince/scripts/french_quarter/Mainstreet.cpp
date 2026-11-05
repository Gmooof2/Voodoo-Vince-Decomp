// UNIT: src/Apps/Vince/scripts/french_quarter/Mainstreet.cpp OBJ: Xbox\OfficialRelease\Mainstreet.obj OPT: /O1 /Oy
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
template <class E, class T, class A>
basic_string<E, T, A> operator+(const E* _Left, const basic_string<E, T, A>& _Right);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion(const Vector3& v) { Set(v); }
    void Set(const Vector3&);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class Locator { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetLocation() const { return _pos; }
    const Vector3& GetBasisVector(int i) const { return _basis[i]; }

    Vector3 _pos;                      // 0x00
    Quaternion _rot;                   // 0x0C
    Vector3 _basis[3];                 // 0x1C
    char field_40[0x48 - 0x40];        // 0x40
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    Locator* Find(const SwampString& s) const;
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class Critter;               // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef();

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x8];                // 0x24
    float _length;                     // 0x2C
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim._Resource->_length; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x10];               // 0x28
    float _time;                       // 0x38
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
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

    void RegisterTimerEvent(unsigned int, float, bool);

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    Critter* GetCritter();

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class PowerIconMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0xD8 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(PowerIconMain, 0xD8);

class RewardPowerIconMain : public PowerIconMain { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(RewardPowerIconMain, 0xD8);

class RewardGateFrogMain : public RewardPowerIconMain { // NAME: proto-pdb:default.pdb
public:
    RewardGateFrogMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(RewardGateFrogMain, 0xD8);

class GatesRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_CUTAWAY = 0
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_FROGDIED = 0x14
    };
    GatesRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void AddFrog(const SwampString& name);

    bool _solved;                      // 0x4C
    char field_4D[0x50 - 0x4D];        // 0x4D
    int _frogCount;                    // 0x50
};
DECOMP_SIZE_ASSERT(GatesRegionMain, 0x54);

class GateTrapMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Message { // NAME: proto-pdb:default.pdb
        MESSAGE_CLOSE = 0x14,
        MESSAGE_OPEN = 0x15,
        MESSAGE_RESET = 0x16
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_OPEN = 0,
        STATE_CLOSED = 1,
        STATE_OPENING = 2,
        STATE_CLOSING = 3
    };
    GateTrapMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _open; // 0xBC
};
DECOMP_SIZE_ASSERT(GateTrapMain, 0xC0);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    LevelScript(SwampSceneNode*, bool);
    virtual ~LevelScript();
    virtual void TimerEvent(unsigned int id);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
    virtual bool DoProgressHint(int level, const SwampString& hint); // vtable+0x64

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_4E[0x64 - 0x4E];        // 0x4E
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class MainstreetRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    MainstreetRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
    virtual bool DoProgressHint(int level, const SwampString& hint);
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(MainstreetRegionMain, 0x64);

// FUNCTION: USA 0x00070514
// PROTOTYPE: PREVIEW 0x0045C6A4 identical
// NAME: proto-pdb:default.pdb
void GateTrapMain::AnimationFinished()
{
    if (_state == STATE_OPENING)
        SetState(STATE_OPEN);
    else if (_state == STATE_CLOSING)
        SetState(STATE_CLOSED);
}

// FUNCTION: USA 0x0007052D
// PROTOTYPE: PREVIEW 0x0045C6BD identical
// NAME: proto-pdb:default.pdb
SwampString GatesRegionMain::Save()
{
    return format("\t\tsolved = %s;\n", _solved ? "true" : "false");
}

// FUNCTION: USA 0x000705BC
// PROTOTYPE: PREVIEW 0x0045C730 identical
// NAME: proto-pdb:default.pdb
SwampScript* RewardGateFrogMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("mainstreet\\Mainstreet.cpp", 27) RewardGateFrogMain(node);
}

// FUNCTION: USA 0x00070A0E
// NAME: proto-pdb:default.pdb
void GatesRegionMain::Load(const BeepTDF& tdf)
{
    _solved = tdf.GetBoolRecord("solved", false);
}

// FUNCTION: USA 0x00070B9F
// PROTOTYPE: PREVIEW 0x0045CD21 identical
// NAME: proto-pdb:default.pdb
SwampScript* GatesRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("mainstreet\\Mainstreet.cpp", 41) GatesRegionMain(node);
}

// FUNCTION: USA 0x00070D0D
// PROTOTYPE: PREVIEW 0x0045CE8F identical
// NAME: proto-pdb:default.pdb
SwampScript* GateTrapMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("mainstreet\\Mainstreet.cpp", 69) GateTrapMain(node);
}

// FUNCTION: USA 0x00071080
// PROTOTYPE: PREVIEW 0x0045D202 identical
// NAME: proto-pdb:default.pdb
SwampScript* MainstreetRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\mainstreet\\Mainstreet.h", 11) MainstreetRegionMain(node);
}

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_MONSTER_KILLED = 0x14
    };

    char field_BC[0x250 - 0xBC];       // 0xBC
};

class PitFrogMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    PitFrogMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
protected:
    virtual void SetState(int);

public:
    char field_250[0x39C - 0x250];     // 0x250
};

class GateFrogMain : public PitFrogMain { // NAME: proto-pdb:default.pdb
public:
    GateFrogMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(GateFrogMain, 0x39C);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00

    enum NodeOptions { DontDraw = 0x2 }; // NAME: proto-pdb:default.pdb
    SwampScript* GetScript() const;
    void SetOption(unsigned int option, bool on, bool recurse);

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);

    char field_A4[0x200 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

// Retail layout: the ctor clears a node bool at 0x260 (proto PDB Critter 0x252 _deathTemporary).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0x260 - 0x200];     // 0x200
    bool field_260;                    // 0x260
};

inline Critter* CritterMain::GetCritter() { return (Critter*)_node; }

class Vince; // NAME: proto-pdb:default.pdb

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    void RemoveMatching(const SwampString&);
    void SetSleepMatching(const SwampString&, bool, unsigned int);
    Critter* Find(const SwampString&);
    Critter* Create(const SwampString&, const SwampString&, const char*, ResourceHandle<ModelResource>, const Vector3&,
                    const Quaternion&, unsigned int);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger* Find(const SwampString&);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    RegionManager& GetRegionManager() const { return *_regionManager; }
    ModelManager& GetModelManager() const { return *_modelManager; }

    char field_0[0x28];                // 0x00
    ModelManager* _modelManager;       // 0x28
    char field_2C[0x30 - 0x2C];        // 0x2C
    RegionManager* _regionManager;     // 0x30
    char field_34[0x68 - 0x34];        // 0x34
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void StayIdle(bool);
};

class AnimCam : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
};

class Settings { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
};
DECOMP_SIZE_ASSERT(Settings, 0x34);

class Globals : public Settings { // NAME: proto-pdb:default.pdb
public:
    char field_34[0x278 - 0x34];       // 0x34
    int _mainstreetFrogTrapCount;      // 0x278
};
extern Globals gSettings; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0007071D
// PROTOTYPE: PREVIEW 0x0045C891 identical
// NAME: proto-pdb:default.pdb
void MainstreetRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (_inRegion) {
        LevelScript::LeavingRegion(node);
        pManager->SetSleepMatching("Rocket", true, 0);
        pManager->SetSleepMatching("Reggie", true, 0);
        pManager->SetSleepMatching("Primo", true, 0);
    }
}

// FUNCTION: USA 0x00070A46
// PROTOTYPE: PREVIEW 0x0045CBC8 identical
// NAME: proto-pdb:default.pdb
void GateTrapMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_CLOSE:
        SetState(STATE_CLOSING);
        GetCritter()->StartAnimation(_open, false, true);
        SoundManager::PlaySound("qms_trapshuts", 0x20, _node, true, 0.0f, 0.0f);
        break;
    case MESSAGE_OPEN:
        SetState(STATE_OPENING);
        GetCritter()->StartAnimation(_open, false, false);
        SoundManager::PlaySound("qms_trapopens", 0x20, _node, true, 0.0f, 0.0f);
        break;
    case MESSAGE_RESET: {
        SetState(STATE_OPEN);
        // !FAKE: c1xx front end; neutral local tmp; the PDB names no local here
        ResourceHandle<AnimationControl> tmp = GetCritter()->StartAnimation(_open, false, false);
        tmp().SetAnimationTime(tmp().GetAnimationLength());
        break;
    }
    }
}

// FUNCTION: USA 0x00070FCF
// PROTOTYPE: PREVIEW 0x0045D151 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); wall: relink refuses: external ?Message@MainstreetRegionMain@@UAEXPAVSwampScript@@HPAX@Z does not resolve by name (vtable 0x001DB7E8 slot +0x34 = 0x00070505, unnamed; PDB-declared override) (4.4 T4.c9)
MainstreetRegionMain::MainstreetRegionMain(SwampSceneNode* node) : LevelScript(node, true)
{
    pManager->RemoveMatching("Wrench");
    Critter* reggie = pManager->Find("Reggie");
    if (reggie) {
        Critter* cangas = pManager->Find("CanGas");
        cangas->SetOption(SwampSceneNode::DontDraw, true, false);
    }
}
#endif

// FUNCTION: USA 0x00070B40
// PROTOTYPE: PREVIEW 0x0045CCC2 identical
// NAME: proto-pdb:default.pdb
void GateFrogMain::Message(SwampScript* source, int message, void* data)
{
    PitFrogMain::Message(source, message, data);
    if (message == MESSAGE_MONSTER_KILLED)
        g_swamp->GetRegionManager().Find("gates")->GetScript()->Message(this, GatesRegionMain::MESSAGE_FROGDIED, 0);
}

// FUNCTION: USA 0x00070BC3
// PROTOTYPE: PREVIEW 0x0045CD45 identical
// NAME: proto-pdb:default.pdb
void GatesRegionMain::AddFrog(const SwampString& frogName)
{
    // !FAKE: c2 register allocation; dummy local; retail holds g_locatorManager in esi across operator+ (match_one 95.08 -> 100.0)
    LocatorManager* v0 = g_locatorManager;
    Locator* loc = v0->Find("locator_" + frogName);
    // !FAKE: c2 block layout; inner block; the block lets v2 reuse the operator+ temp slot (frame 0x64 -> 0x48, 99.95 -> 100.0)
    {
        // !FAKE: c1xx front end; dummy locals; retail builds both strings and the Quaternion before loading pManager (94.75 -> 99.95)
        SwampString v2("pitfrog_base");
        SwampString v3("gatefrog");
        Quaternion v4(loc->GetBasisVector(2));
        // !FAKE: c2 register allocation; dummy local; retail holds pManager in edi across Load (88.52 -> 100.0)
        CritterManager* v1 = pManager;
        v1->Create(frogName, v3, "pitfrog", g_swamp->GetModelManager().Load(v2), loc->GetLocation(), v4, 0);
    }
}

// FUNCTION: USA 0x00070D34
// PROTOTYPE: PREVIEW 0x0045CEB6 identical
// NAME: proto-pdb:default.pdb
void GatesRegionMain::EnteringRegion(SwampSceneNode* node)
{
    Vince* vince = pManager->GetVince();
    if (vince == node && !_solved) {
        if (_frogCount == 0) {
            _frogCount = gSettings._mainstreetFrogTrapCount;
            for (int i = 0; i < _frogCount; i++) {
                char frogname[] = "gatefroga";
                frogname[8] = 'a' + i;
                AddFrog(frogname);
            }
            pManager->Find("GateTrap01")->GetScript()->Message(this, GateTrapMain::MESSAGE_CLOSE, 0);
            pManager->Find("GateTrap02")->GetScript()->Message(this, GateTrapMain::MESSAGE_CLOSE, 0);
            AnimCam::SwitchTo("cutaway_ratTrapCamera", 0.0f, false, 0.0f);
            RegisterTimerEvent(TIMER_CUTAWAY, 2.0f, false);
            vince->StayIdle(true);
        }
    }
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x00071194
// PROTOTYPE: PREVIEW 0x0045D316 near-identical
// NAME: proto-pdb:default.pdb
GateFrogMain::GateFrogMain(SwampSceneNode* node) : PitFrogMain(node)
{
    ((Critter*)_node)->field_260 = false;
    SetState(3);
}
#endif

// FUNCTION: USA 0x000711BF
// PROTOTYPE: PREVIEW 0x0045D341 identical
// NAME: proto-pdb:default.pdb
SwampScript* GateFrogMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("mainstreet\\Mainstreet.cpp", 90) GateFrogMain(node);
}
