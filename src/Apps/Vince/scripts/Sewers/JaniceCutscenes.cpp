// UNIT: src/Apps/Vince/scripts/Sewers/JaniceCutscenes.cpp OBJ: Xbox\OfficialRelease\JaniceCutscenes.obj OPT: /O1 /Oy

namespace std {

template<class _Elem>
struct char_traits {};

template<class _Ty>
class allocator {};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    basic_string(const _Elem *_Ptr);
    ~basic_string() { _Tidy(true); }
    int compare(const _Elem *_Ptr) const;
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);
    char _pad00[0x1C];
};

}

class ResourceBase {
public:
    char _Name[0x1C];
    unsigned int _RefCount;
    bool _valid;
};

class AnimationResource : public ResourceBase {
};

class AnimationControl;

template<class T>
class ResourceHandle {
public:
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    T* _Resource;
};

template<class T>
class ResourceManager {
public:
    ResourceHandle<T> Load(const std::basic_string<char>& name);
};

class AnimationManager : public ResourceManager<AnimationResource> {
};

class Swamp {
public:
    char _pad00[0x18];
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager;
};

extern Swamp* g_swamp;

struct AABox;                // NAME: proto-pdb:default.pdb
class Matrix;                // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb
class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template<class T>
class Ptr;                   // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion(float x, float y, float z, float _w) : v(x, y, z), w(_w) {}

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};

class SwampSceneNode {
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    void SetOption(unsigned int option, bool a, bool b);
    void SetScript(SwampScript* script);
    const Quaternion& SetRotation(const Quaternion&);
};

class GatorNode : public SwampSceneNode {
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
};

class Critter : public GatorNode {
};

// Retail layout: Vince _whammyReady 0x434 -> 0x448 (+0x14).
class Vince : public Critter {
public:
    void SetWhammyReady(bool);

    char field_4[0x448 - 0x4];         // 0x04
    bool _whammyReady;                 // 0x448 (proto 0x434)
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const std::basic_string<char>&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class AnimCam;

class FireFlyNode { // NAME: proto-pdb:default.pdb
public:
    void Play();
    void StopSpawning();
};

class SwampScript {
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
    virtual void AnimationTrigger(const std::basic_string<char>&, float, float, unsigned int); // vtable+0x28
    virtual bool LosingControl(const std::basic_string<char>&);    // vtable+0x2C
    virtual void GainingControl(SwampScript*);                     // vtable+0x30
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    virtual void EnteringRegion(SwampSceneNode*);                  // vtable+0x38
    virtual void LeavingRegion(SwampSceneNode*);                   // vtable+0x3C
    virtual void SoundFinished(const std::basic_string<char>&);    // vtable+0x40
    virtual void SoundStreamerFinished(const std::basic_string<char>&); // vtable+0x44
    virtual void MovieFinished(const std::basic_string<char>&);    // vtable+0x48
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x50
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual std::basic_string<char> Save();                        // vtable+0x58
    virtual void SetNode(SwampSceneNode*);                         // vtable+0x5C
    virtual void InvalidateScript();                               // vtable+0x60
    char _pad04[0x4C - 0x4];
};

class CutsceneAnimationScript : public SwampScript { // NAME: proto-pdb:default.pdb
};

class CutsceneDriver : public SwampScript {
public:
    CutsceneDriver(SwampSceneNode* node);
    virtual void AnimationFinished(unsigned int);                  // vtable+0x64
    virtual void AnimationTrigger(unsigned int id, const std::basic_string<char>& name, float value, float prev, unsigned int bone); // vtable+0x68
    static void BeginPlayback();
protected:
    virtual void Init();                                           // vtable+0x6C
    virtual void Cleanup();                                        // vtable+0x70
public:
    virtual void SetState(int);                                    // vtable+0x74
protected:
    AnimCam* NewCamera(const std::basic_string<char>& name);
    CutsceneAnimationScript* NewScript(SwampSceneNode* node, unsigned int id);
    void StateTransition(unsigned int anim, int state);
public:
    AnimCam* _cam;
    bool _cancelled;
    bool _autostop;
    int _state;
    float _cameraShake;
    std::basic_string<char> _cameraName;
};

class VinceDropsInCutscene : public CutsceneDriver {
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
public:
    enum States {
        STATE_V_FALLSIN = 0x0,
        // 0x1..0xA: names unknown; the case labels below use the values
        STATE_V_ONIT = 0xB,
        STATE_LAST = 0xC
    };
    VinceDropsInCutscene(SwampSceneNode* node);
protected:
    virtual void Init();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);
public:
    Vince* _vince;
    Critter* _ethel;
    bool _wasWhammyReady;
};

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const std::basic_string<char>& name, CreationFunc func);
};

class JaniceDefeatCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    enum States {
        STATE_N_EXPLODES = 0x0,
        STATE_J_DEATH = 0x1,
        STATE_V_WALKS = 0x2,
        STATE_V_WONDERS = 0x3,
        STATE_V_LOOKING = 0x4,
        STATE_V_HMMM = 0x5,
        STATE_LAST = 0x6
    };
    JaniceDefeatCutscene(SwampSceneNode* node);
protected:
    virtual void Init();
private:
    virtual void SetState(int state);
public:
    Vince* _vince;
    Critter* _janice;
    GatorNode* _debris;
    bool _wasWhammyReady;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};

class LeavingRoachfortCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    enum States {
        STATE_V_INHALE = 0x0,
        STATE_V_FLOATS = 0x1,
        STATE_LAST = 0x2
    };
    LeavingRoachfortCutscene(SwampSceneNode* node);
protected:
    virtual void Init();
private:
    virtual void SetState(int state);
public:
    Vince* _vince;
    bool _wasWhammyReady;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};

class PlanGoesAwryCutscene : public CutsceneDriver {
public:
    enum Ids { // NAME: proto-pdb:default.pdb
        ID_VINCE = 0x1
    };
    enum States {
        STATE_E_PULLCORD = 0x0,
        STATE_LAST = 0xE
    };
    PlanGoesAwryCutscene(SwampSceneNode* node);
protected:
    virtual void Init();
public:
    virtual void AnimationFinished(unsigned int id);
    virtual void AnimationTrigger(unsigned int id, const std::basic_string<char>& name, float value, float prev, unsigned int bone);

    Vince* _vince;                     // 0x78
    Critter* _ethel;                   // 0x7C
    Critter* _janice;                  // 0x80
    GatorNode* _balloon;               // 0x84
    FireFlyNode* _effect;              // 0x88
    bool _wasWhammyReady;              // 0x8C
};

// Retail string addresses (push imm32 operands), in body order; contents not reproduced.
extern "C" const char s_001EA324[], s_001EA30C[], s_001EA2FC[], s_001EA2EC[], s_001EA2D0[], s_001EA2AC[],
    s_001EA29C[], s_001EA284[], s_001EA264[], s_001EA254[], s_001EA244[], s_001EA22C[], s_001EA21C[], s_001EA200[],
    s_001EA1DC[], s_001EA1CC[], s_001EA1B0[], s_001EA1A0[], s_001EA180[], s_001EA160[], s_001EA150[], s_001EA138[],
    s_001EA128[], s_001EA108[], s_001EA0F8[], s_001EA0E4[], s_001EA0C8[];
extern "C" const char s_001EA60C[], s_001EA5F4[], s_001EA62C[], s_001EA61C[], s_001EA654[], s_001EA63C[],
    s_001EA67C[], s_001EA664[], s_001EA68C[], s_001EA6B4[], s_001EA69C[];

void* operator new(unsigned int size, const char* file, int line);

// FUNCTION: USA 0x000A82A9
// PROTOTYPE: PREVIEW 0x004931B7 identical
// NAME: proto-pdb:default.pdb
void PlanGoesAwryCutscene::AnimationFinished(unsigned int id)
{
    StateTransition(id, STATE_LAST);
}

// FUNCTION: USA 0x000A8353
// PROTOTYPE: PREVIEW 0x00493253 identical
// NAME: proto-pdb:default.pdb
void PlanGoesAwryCutscene::AnimationTrigger(unsigned int id, const std::basic_string<char>& name, float value, float prev, unsigned int bone)
{
    CutsceneDriver::AnimationTrigger(id, name, value, prev, bone);
    if (id == ID_VINCE && name.compare("trigger_sizzle") == 0 && _effect) {
        if (value > 0.0f)
            _effect->Play();
        else
            _effect->StopSpawning();
    }
}

// FUNCTION: USA 0x000A83B8
// PROTOTYPE: PREVIEW 0x004932B8 ambiguous
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 45.83, bytes provisional (masked 2; wall: same instructions, ??_7LeavingRoachfortCutscene@@6B@ vptr store and ?Init@LeavingRoachfortCutscene@@MAEXXZ call target have no retail VA row, C0203)
LeavingRoachfortCutscene::LeavingRoachfortCutscene(SwampSceneNode* node) : CutsceneDriver(node), _vince(0)
{
    Init();
}
#endif

// FUNCTION: USA 0x000A83DB
// NAME: proto-pdb:default.pdb
SwampScript* LeavingRoachfortCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("WrathOfJanice\\JaniceCutscenes.cpp", 633) LeavingRoachfortCutscene(node);
}

// FUNCTION: USA 0x000A8405
// PROTOTYPE: PREVIEW 0x00493321 near-identical
// NAME: proto-pdb:default.pdb
void VinceDropsInCutscene::Init()
{
    CutsceneDriver::Init();
    _vince = pManager->GetVince();
    _vince->SetScript(NewScript(_vince, 1));
    _vince->SetOption(2, false, false);
    _vince->SetWorldPosition(Vector3(0.0f, 0.0f, 0.0f));
    _vince->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
    _wasWhammyReady = _vince->_whammyReady;
    _vince->SetWhammyReady(false);
    _ethel = pManager->Find("professorethel");
    _ethel->SetScript(NewScript(_ethel, 2));
    _ethel->SetWorldPosition(Vector3(0.0f, 0.0f, 0.0f));
    _ethel->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
    _ethel->SetOption(2, true, false);
    SetState(STATE_V_FALLSIN);
    BeginPlayback();
}

// FUNCTION: USA 0x000A871D
// PROTOTYPE: PREVIEW 0x004935E1 identical
// NAME: proto-pdb:default.pdb
void VinceDropsInCutscene::SetState(int state)
{
    switch (state) {
    case STATE_V_FALLSIN:
        NewCamera(s_001EA324);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA30C), false, false);
        break;
    case 0x1:
        NewCamera(s_001EA2FC);
        break;
    case 0x2:
        NewCamera(s_001EA2EC);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA2D0), false, false);
        _ethel->SetOption(2, false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA2AC), false, false);
        break;
    case 0x3:
        NewCamera(s_001EA29C);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA284), false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA264), false, false);
        break;
    case 0x4:
        NewCamera(s_001EA254);
        _vince->SetOption(2, true, false);
        break;
    case 0x5:
        NewCamera(s_001EA244);
        _vince->SetOption(2, false, false);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA22C), false, false);
        break;
    case 0x6:
        NewCamera(s_001EA21C);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA200), false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA1DC), false, false);
        break;
    case 0x7:
        NewCamera(s_001EA1CC);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA1B0), false, false);
        break;
    case 0x8:
        NewCamera(s_001EA1A0);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA180), false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA160), false, false);
        break;
    case 0x9:
        NewCamera(s_001EA150);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA138), false, false);
        break;
    case 0xA:
        NewCamera(s_001EA128);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA108), false, false);
        break;
    case STATE_V_ONIT:
        NewCamera(s_001EA0F8);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA0E4), false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA0C8), true, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x000A9D72
// PROTOTYPE: PREVIEW 0x00494C06 identical
// NAME: proto-pdb:default.pdb
void JaniceDefeatCutscene::SetState(int state)
{
    switch (state) {
    case STATE_N_EXPLODES:
        NewCamera(s_001EA60C);
        _debris->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA5F4), false, false);
        break;
    case STATE_J_DEATH:
        NewCamera(s_001EA62C);
        _janice->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA61C), false, false);
        break;
    case STATE_V_WALKS:
        NewCamera(s_001EA654);
        _vince->SetOption(2, false, false);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA63C), false, false);
        _janice->SetOption(2, true, true);
        break;
    case STATE_V_WONDERS:
        NewCamera(s_001EA67C);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA664), false, false);
        break;
    case STATE_V_LOOKING:
        NewCamera(s_001EA68C);
        break;
    case STATE_V_HMMM:
        NewCamera(s_001EA6B4);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA69C), false, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x000AA0E1
// PROTOTYPE: PREVIEW 0x00494F55 identical
// NAME: proto-pdb:default.pdb
void LeavingRoachfortCutscene::SetState(int state)
{
    switch (state) {
    case STATE_V_INHALE:
        NewCamera("camera_00_LR");
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load("vince_janiceDefeated003b"), false, false);
        break;
    case STATE_V_FLOATS:
        NewCamera("camera_01_LR");
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load("vince_janiceDefeated004"), false, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x000AA1F3
// PROTOTYPE: PREVIEW 0x00495067 ambiguous
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2; wall: same instructions, ??_7PlanGoesAwryCutscene@@6B@ vptr store and ?Init@PlanGoesAwryCutscene@@MAEXXZ call target have no retail VA row, C0203)
PlanGoesAwryCutscene::PlanGoesAwryCutscene(SwampSceneNode* node)
    : CutsceneDriver(node), _vince(0), _ethel(0), _janice(0), _balloon(0), _effect(0)
{
    Init();
}
#endif

// FUNCTION: USA 0x000AA499
// PROTOTYPE: PREVIEW 0x004952C6 ambiguous
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2; wall: same instructions, ??_7JaniceDefeatCutscene@@6B@ vptr store and ?Init@JaniceDefeatCutscene@@MAEXXZ call target have no retail VA row, C0203)
JaniceDefeatCutscene::JaniceDefeatCutscene(SwampSceneNode* node) : CutsceneDriver(node), _vince(0), _janice(0), _debris(0)
{
    Init();
}
#endif

// FUNCTION: USA 0x000AA4C6
// PROTOTYPE: PREVIEW 0x004952F3 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceDropsInCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("WrathOfJanice\\JaniceCutscenes.cpp", 20) VinceDropsInCutscene(node);
}

// FUNCTION: USA 0x000AA517
// NAME: proto-pdb:default.pdb
SwampScript* JaniceDefeatCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("WrathOfJanice\\JaniceCutscenes.cpp", 476) JaniceDefeatCutscene(node);
}

// FUNCTION: USA 0x000AA541
// NAME: proto-pdb:default.pdb
VinceDropsInCutscene::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VinceDropsInCutscene", creationFunc);
}

// FUNCTION: USA 0x000AA5B9
// NAME: proto-pdb:default.pdb
JaniceDefeatCutscene::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("JaniceDefeatCutscene", creationFunc);
}

// FUNCTION: USA 0x000AA5F5
// NAME: proto-pdb:default.pdb
LeavingRoachfortCutscene::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("LeavingRoachfortCutscene", creationFunc);
}
