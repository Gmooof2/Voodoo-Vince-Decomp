// UNIT: src/Apps/Vince/scripts/Sewers/MainTunnel.cpp OBJ: Xbox\OfficialRelease\MainTunnel.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
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
struct Vector3 { // NAME: proto-pdb:default.pdb
    void Zero() { x = y = z = 0.0f; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    void Identity() { v.Zero(); w = 1.0f; }

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);
class SwampScript;           // NAME: proto-pdb:default.pdb
class AnimationResource;     // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x34 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const struct AABox& GetBounds() const;                 // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08

    SwampScript* GetScript() const;
    unsigned int GetZone() const;
    void SetOption(unsigned int, bool, bool);

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode; CollidableCylinder not modelled
class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetCollidable(bool collidable) { _collidable = collidable; }

    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class GatorNode : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void SetAnimSpeed(float speed);
    float GetAnimSpeed();

    char field_AC[0x200 - 0xAC];       // 0xAC
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    ModelManager& GetModelManager() const { return *_modelManager; }
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x18 - 0x8];          // 0x08
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x28 - 0x1C];        // 0x1C
    ModelManager* _modelManager;       // 0x28
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

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
    SwampSceneNode* GetNode() { return _node; }
protected:
    SwampScript(SwampSceneNode* node);
    void RegisterSaveLoadVar(const SwampString&, SwampString*, const char*);
    void RegisterSaveLoadVar(const SwampString&, bool*, bool);
public:

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual SwampString Save();                                    // vtable+0x58
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
protected:
    virtual void SetState(int);                                    // vtable+0x6C
public:

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class SoundNode { // NAME: proto-pdb:default.pdb
public:
    void SetMinDistance(float);
    void SetPitch(float);
    void Play();
};

class WheelPlatformAMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    WheelPlatformAMain(SwampSceneNode* node);
    enum States { // NAME: proto-pdb:default.pdb
        STATE_OFF = 0,
        STATE_SPEEDING_UP = 1,
        STATE_ON = 2
    };
    virtual void Activate();
    virtual void FrameTick();
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);

    ResourceHandle<AnimationResource> _animIdle; // 0xBC
    bool _on;                          // 0xC0
    SoundNode* _sound;                 // 0xC4
};
DECOMP_SIZE_ASSERT(WheelPlatformAMain, 0xC8);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static SoundNode* CreateSound(const SwampString&, unsigned int, SwampSceneNode*, bool);
};

class ReverseElevatorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    ReverseElevatorMain(SwampSceneNode* node);
    enum States { // NAME: proto-pdb:default.pdb
        STATE_OFF = 0,
        STATE_ACTIVATING = 1,
        STATE_ON = 2
    };
    virtual void Activate();
    virtual void AnimationFinished();
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);

    ResourceHandle<AnimationResource> _animIdle;     // 0xBC
    ResourceHandle<AnimationResource> _animActivate; // 0xC0
    bool _on;                          // 0xC4
    SoundNode* _sound;                 // 0xC8
};
DECOMP_SIZE_ASSERT(ReverseElevatorMain, 0xCC);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode*);                  // vtable+0x38
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_4E[0x64 - 0x4E];        // 0x4E
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class Critter; // NAME: proto-pdb:default.pdb

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Create(const SwampString&, const SwampString&, const char*, ResourceHandle<ModelResource>, const Vector3&,
                    const Quaternion&, unsigned int);
    void InvalidateMatching(const char*, unsigned int);
    void NotifyMatchingCritters(const char*, SwampScript*, unsigned int, void*);
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class DoorRockBlockMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_DILLO_EXPLODE = 0x14,
        MESSAGE_OPEN = 0x15
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0,
        STATE_OPEN = 1
    };
    DoorRockBlockMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
private:
    virtual void SetState(int state);

public:
    bool _cleared;                     // 0xBC
private:
    static int _rockDoors;
    // access only: retail mangles _rockDoors private (@@0HA); MainTunnelRegionMain::EnteringRegion stores it
    friend class MainTunnelRegionMain;
};
DECOMP_SIZE_ASSERT(DoorRockBlockMain, 0xC0);

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    static void SetStream(const SwampString&, bool);
};

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34

    char field_BC[0x110 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class SwitchBoxGateMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript* source, int message, void* data);
};
DECOMP_SIZE_ASSERT(SwitchBoxGateMain, 0x110);

class MainTunnelRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    MainTunnelRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x68 - 0x64];        // 0x64
};
DECOMP_SIZE_ASSERT(MainTunnelRegionMain, 0x68);

class ArmorDilloChuteMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0,
        STATE_OPENING = 1,
        STATE_OPEN = 2
    };
    ArmorDilloChuteMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void AnimationFinished();
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void FrameTick();
private:
    virtual void SetState(int state);
    static SwampScript* creationFunc(SwampSceneNode* node);
public:
    ResourceHandle<AnimationResource> _animIdle; // 0xBC
    ResourceHandle<AnimationResource> _animOpen; // 0xC0
    int _dilloCount;                   // 0xC4
    float _nextDilloCountdown;         // 0xC8
    bool _open;                        // 0xCC
};
DECOMP_SIZE_ASSERT(ArmorDilloChuteMain, 0xD0);

// FUNCTION: USA 0x0009F6DA
// PROTOTYPE: PREVIEW 0x0048A77E identical
// NAME: proto-pdb:default.pdb
void WheelPlatformAMain::Message(SwampScript* source, int message, void* data)
{
    if (message != 6) {
        CritterMain::Message(source, message, data);
        return;
    }
    SetState(_on ? STATE_ON : STATE_OFF);
}

// FUNCTION: USA 0x0009F6FC
// PROTOTYPE: PREVIEW 0x0048A7A0 identical
// NAME: proto-pdb:default.pdb
void ReverseElevatorMain::Message(SwampScript* source, int message, void* data)
{
    if (message != 6) {
        CritterMain::Message(source, message, data);
        return;
    }
    SetState(_on ? STATE_ON : STATE_OFF);
}

// FUNCTION: USA 0x0009F71E
// PROTOTYPE: PREVIEW 0x0048A7C2 identical
// NAME: proto-pdb:default.pdb
void ArmorDilloChuteMain::Activate()
{
    if (!_open) {
        _open = true;
        SetState(STATE_OPENING);
    }
}

// FUNCTION: USA 0x0009F734
// PROTOTYPE: PREVIEW 0x0048A7D8 identical
// NAME: proto-pdb:default.pdb
void ArmorDilloChuteMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 6:
        SetState(_open ? STATE_OPEN : STATE_CLOSED);
        break;
    case 0x14:
        _dilloCount--;
        break;
    default:
        CritterMain::Message(source, message, data);
        break;
    }
}

// FUNCTION: USA 0x0009F7AD
// PROTOTYPE: PREVIEW 0x0048A831 identical
// NAME: proto-pdb:default.pdb
void DoorRockBlockMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 6:
        CritterMain::Message(source, message, data);
        SetState(_cleared ? STATE_OPEN : STATE_CLOSED);
        if (!_cleared)
            _rockDoors++;
        break;
    case MESSAGE_DILLO_EXPLODE:
        if (!_cleared) {
            _rockDoors--;
            _cleared = true;
            SetState(STATE_OPEN);
        }
        break;
    case MESSAGE_OPEN:
        _cleared = true;
        SetState(STATE_OPEN);
        break;
    default:
        CritterMain::Message(source, message, data);
        break;
    }
}

// FUNCTION: USA 0x000A0417
// NAME: proto-pdb:default.pdb
DoorRockBlockMain::DoorRockBlockMain(SwampSceneNode* node) : CritterMain(node)
{
    _cleared = false;
    RegisterSaveLoadVar("cleared", &_cleared, false);
    SetState(STATE_CLOSED);
}

// FUNCTION: USA 0x0009F83D
// PROTOTYPE: PREVIEW 0x0048A8C1 identical
// NAME: proto-pdb:default.pdb
void SwitchBoxGateMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 2:
    case 6:
        BasicDoorMain::Message(source, message, data);
        BasicDoorMain::Message(this, 0x17, 0);
        break;
    default:
        BasicDoorMain::Message(source, message, data);
        break;
    }
}

class BoxGateHintRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    BoxGateHintRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BoxGateHintRegionMain, 0x4C);

// FUNCTION: USA 0x0009F87B
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; relink wall: its vtable's ??_E slot is the 12-name fold 0x00025012
// without ??_EBoxGateHintRegionMain, so our ??_G links as extra .text (precheck: unlisted `.text vsize 0x145178 does
// not give XBE 0x14515C`, listed `our ranges overlap at 0x00025012`; C0217); lift: fold-alias pin; 5.3 T12.1.c2
BoxGateHintRegionMain::BoxGateHintRegionMain(SwampSceneNode* node) : SwampScript(node)
{
}
#endif

// FUNCTION: USA 0x0009F893
// PROTOTYPE: PREVIEW 0x0048A917 identical
// NAME: proto-pdb:default.pdb
void MainTunnelRegionMain::Message(SwampScript* source, int message, void* data)
{
    if (message == 6)
        pManager->NotifyMatchingCritters("switch_", this, 0x16, 0);
    LevelScript::Message(source, message, data);
}

// FUNCTION: USA 0x0009F8C9
// PROTOTYPE: PREVIEW 0x0048A94D identical
// NAME: proto-pdb:default.pdb
void WheelPlatformAMain::FrameTick()
{
    // !FAKE: c2 register allocation; stand-in local (no PDB local); retail holds _node in edi across the state case
    GatorNode* node = (GatorNode*)_node;
    switch (_state) {
    case STATE_SPEEDING_UP: {
        float s = node->GetAnimSpeed();
        s += g_swamp->GetInput().GetElapsedTime() * 0.33333334f;
        if (s >= 1.0f) {
            s = 1.0f;
            _state = STATE_ON;
        }
        _sound->SetPitch(s - 1.25f);
        node->SetAnimSpeed(s);
        break;
    }
    default:
        UnRegisterFrameTick();
        break;
    }
}

// FUNCTION: USA 0x0009F945
// PROTOTYPE: PREVIEW 0x0048A9C9 identical
// NAME: proto-pdb:default.pdb
void WheelPlatformAMain::SetState(int state)
{
    // !FAKE: c2 register allocation; stand-in local (no PDB local); retail loads _node into ecx before the _state store
    GatorNode* node = (GatorNode*)_node;
    _state = state;
    switch (state) {
    case STATE_OFF:
        node->SetAnimSpeed(0.0f);
        break;
    case STATE_SPEEDING_UP:
        if (_animIdle.IsValid()) {
            node->SetAnimSpeed(0.0f);
            RegisterFrameTick();
        } else {
            SetState(STATE_ON);
        }
        break;
    case STATE_ON:
        node->SetAnimSpeed(1.0f);
        break;
    }
}

// FUNCTION: USA 0x0009F998
// PROTOTYPE: PREVIEW 0x0048AA1C identical
// NAME: proto-pdb:default.pdb
void ReverseElevatorMain::SetState(int state)
{
    _state = state;
    // !FAKE: c2 register allocation; stand-in local (no PDB local); retail holds _node in esi across the cases
    GatorNode* node = (GatorNode*)_node;
    switch (state) {
    case STATE_OFF:
        node->StartAnimation(_animActivate, false, false);
        node->SetAnimSpeed(0.0f);
        break;
    case STATE_ACTIVATING:
        if (_animActivate.IsValid())
            node->StartAnimation(_animActivate, false, false);
        else
            SetState(STATE_ON);
        break;
    case STATE_ON:
        node->StartAnimation(_animIdle, true, false);
        break;
    }
}

// FUNCTION: USA 0x0009F9FB
// PROTOTYPE: PREVIEW 0x0048AA7F identical
// NAME: proto-pdb:default.pdb
void ArmorDilloChuteMain::SetState(int state)
{
    _state = state;
    // !FAKE: c2 register allocation; stand-in local (no PDB local); retail holds _node in esi across both calls of the closed case
    GatorNode* node = (GatorNode*)_node;
    switch (state) {
    case STATE_CLOSED:
        node->StartAnimation(_animOpen, false, false);
        node->SetAnimSpeed(0.0f);
        break;
    case STATE_OPENING:
        node->StartAnimation(_animOpen, false, false);
        break;
    case STATE_OPEN:
        node->StartAnimation(_animIdle, true, false);
        break;
    }
}

// FUNCTION: USA 0x0009FA50
// PROTOTYPE: PREVIEW 0x0048AAD4 identical
// NAME: proto-pdb:default.pdb
void DoorRockBlockMain::SetState(int state)
{
    _state = state;
    // !FAKE: c2 register allocation; stand-in local (no PDB local); retail holds _node in esi across the state cases
    GatorNode* node = (GatorNode*)_node;
    switch (state) {
    case STATE_CLOSED:
        node->SetOption(2, false, false);
        node->SetCollidable(true);
        break;
    case STATE_OPEN:
        node->SetOption(2, true, false);
        node->SetCollidable(false);
        break;
    }
}

// FUNCTION: USA 0x0009FAB7
// PROTOTYPE: PREVIEW 0x0048AB3B identical
// NAME: proto-pdb:default.pdb
void MainTunnelRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (!_inRegion) {
        LevelScript::EnteringRegion(node);
        pManager->InvalidateMatching("chuteKillerDillo", 0);
        DoorRockBlockMain::_rockDoors = 0;
        CutsceneDriver::SetStream("kosmoAppearsMainTunnel", false);
    }
}

// FUNCTION: USA 0x0009FB0D
// NAME: proto-pdb:default.pdb
WheelPlatformAMain::WheelPlatformAMain(SwampSceneNode* node) : CritterMain(node)
{
    _on = false;
    _sound = 0;
    _animIdle = g_swamp->GetAnimationManager().Load("wheelPlatformA_idle");
    ((GatorNode*)_node)->StartAnimation(_animIdle, true, false);
    RegisterSaveLoadVar("on", &_on, false);
    SetState(STATE_OFF);
}

// FUNCTION: USA 0x0009FBC3
// PROTOTYPE: PREVIEW 0x0048AC47 identical
// NAME: proto-pdb:default.pdb
void WheelPlatformAMain::Activate()
{
    if (!_on) {
        _on = true;
        SetState(STATE_SPEEDING_UP);
        _sound = SoundManager::CreateSound("rmt_waterwheel", 9, GetNode(), true);
        _sound->SetPitch(-1.25f);
        _sound->SetMinDistance(5.0f);
        _sound->Play();
    }
}

// FUNCTION: USA 0x0009FC4C
// NAME: proto-pdb:default.pdb
ReverseElevatorMain::ReverseElevatorMain(SwampSceneNode* node) : CritterMain(node)
{
    _on = false;
    // !FAKE: c2 register allocation; stand-in local (no PDB local); retail loads the manager once, spilled to the node slot
    AnimationManager& tmp = g_swamp->GetAnimationManager();
    _animIdle = tmp.Load("reverseElevator_idle");
    _animActivate = tmp.Load("reverseElevator_activate");
    RegisterSaveLoadVar("on", &_on, false);
    SetState(STATE_OFF);
}

// FUNCTION: USA 0x0009FD3B
// PROTOTYPE: PREVIEW 0x0048ADBF identical
// NAME: proto-pdb:default.pdb
void ReverseElevatorMain::Activate()
{
    if (!_on) {
        _on = true;
        SetState(STATE_ACTIVATING);
        _sound = SoundManager::CreateSound("rmt_grind", 9, GetNode(), true);
        _sound->SetMinDistance(5.0f);
        _sound->Play();
    }
}

// FUNCTION: USA 0x0009FDAF
// NAME: proto-pdb:default.pdb
ArmorDilloChuteMain::ArmorDilloChuteMain(SwampSceneNode* node) : CritterMain(node)
{
    _dilloCount = 0;
    _nextDilloCountdown = 0.0f;
    _open = false;
    // !FAKE: c2 register allocation; stand-in local (no PDB local); retail loads the manager once, spilled to the node slot
    AnimationManager& tmp = g_swamp->GetAnimationManager();
    _animIdle = tmp.Load("armordilloChute_idle");
    _animOpen = tmp.Load("armordilloChute_open");
    RegisterSaveLoadVar("open", &_open, false);
    SetState(STATE_CLOSED);
    RegisterFrameTick();
}

class DoorRockBlockHintRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    DoorRockBlockHintRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);

    SwampString _target;               // 0x4C
};
DECOMP_SIZE_ASSERT(DoorRockBlockHintRegionMain, 0x68);

// FUNCTION: USA 0x000A052A
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2 = vptr store ??_7DoorRockBlockHintRegionMain 0x001E7F38,
// literal "Target" 0x001E7F2C; neither has a retail name: data-pin wall, source is final; 5.3 T12.1.c2)
DoorRockBlockHintRegionMain::DoorRockBlockHintRegionMain(SwampSceneNode* node) : SwampScript(node)
{
    RegisterSaveLoadVar("Target", &_target, "");
}
#endif

// FUNCTION: USA 0x000A0602
// PROTOTYPE: PREVIEW 0x0048B686 identical
// NAME: proto-pdb:default.pdb
SwampScript* WheelPlatformAMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\maintunnel\\MainTunnel.h", 38) WheelPlatformAMain(node);
}

// FUNCTION: USA 0x000A0629
// PROTOTYPE: PREVIEW 0x0048B6AD identical
// NAME: proto-pdb:default.pdb
SwampScript* ReverseElevatorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\maintunnel\\MainTunnel.h", 66) ReverseElevatorMain(node);
}

// FUNCTION: USA 0x000A0650
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 14.285714, bytes match (masked 0); cut-extent wall C0093/C0140: retail cut symbol 0x120 vs row 0x27 (5.3 T5.c2)
SwampScript* ArmorDilloChuteMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\maintunnel\\MainTunnel.h", 96) ArmorDilloChuteMain(node);
}
#endif

// FUNCTION: USA 0x000A0A59
// PROTOTYPE: PREVIEW 0x0048BAEC identical
// NAME: proto-pdb:default.pdb
SwampScript* MainTunnelRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\maintunnel\\MainTunnel.h", 19) MainTunnelRegionMain(node);
}

// Retail layout: the ctor clears a node bool at 0x260 (proto PDB Critter 0x252 _deathTemporary).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0x260 - 0x200];     // 0x200
    bool field_260;                    // 0x260
};

class Settings { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
};
DECOMP_SIZE_ASSERT(Settings, 0x34);

class Globals : public Settings { // NAME: proto-pdb:default.pdb
public:
    char field_34[0x898 - 0x34];       // 0x34
    float _maintunnelDillosPerChute;   // 0x898
    float _maintunnelDilloRegenTime;   // 0x89C
};
extern Globals gSettings; // NAME: proto-pdb:default.pdb

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    std::deque<T> _queue;              // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~MonsterMain();

    char field_BC[0x240 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

// FUNCTION: USA 0x000A0A7D
// PROTOTYPE: PREVIEW 0x0048BB10 identical
// NAME: proto-pdb:default.pdb
class KillerDilloMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    KillerDilloMain(SwampSceneNode* node);

    MultiAnimation _multiAnimIdle;     // 0x240
    MultiAnimation _multiAnimIdle2;    // 0x254
    MultiAnimation _multiAnimIdle3;    // 0x268
    MultiAnimation _multiAnimIdleHurt; // 0x27C
    MultiAnimation _multiAnimStartRoll; // 0x290
    MultiAnimation _multiAnimRoll;     // 0x2A4
    MultiAnimation _multiAnimWindupBite; // 0x2B8
    MultiAnimation _multiAnimAttackBiteStraight; // 0x2CC
    MultiAnimation _multiAnimAttackBiteLeft; // 0x2E0
    MultiAnimation _multiAnimAttackBiteRight; // 0x2F4
    MultiAnimation _multiAnimWalk;     // 0x308
    char field_31C[0x324 - 0x31C];     // 0x31C
};
DECOMP_SIZE_ASSERT(KillerDilloMain, 0x324);

// !FAKE: c1xx front end; scaffold call; retail's ??1KillerDilloMain@@UAE@XZ is implicit (no vptr store, C0029); this
// qualified call odr-uses it so this unit emits it (matched_set [[emitter]] row)
void pin_emit_KillerDilloMain_dtor(KillerDilloMain* p)
{
    p->KillerDilloMain::~KillerDilloMain();
}

class ChuteKillerDilloMain : public KillerDilloMain { // NAME: proto-pdb:default.pdb
public:
    ChuteKillerDilloMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_DROP = 0x16
    };

    SwampScript* _chute;               // 0x324
};

// FUNCTION: USA 0x0009FEB2
// PROTOTYPE: PREVIEW 0x0048AF36 identical
// NAME: proto-pdb:default.pdb
void ArmorDilloChuteMain::FrameTick()
{
    if (_open && _dilloCount < gSettings._maintunnelDillosPerChute) {
        _nextDilloCountdown -= g_swamp->GetInput().GetElapsedTime();
        if (_nextDilloCountdown <= 0.0f) {
            _dilloCount++;
            _nextDilloCountdown = gSettings._maintunnelDilloRegenTime;
            ResourceHandle<ModelResource> model = g_swamp->GetModelManager().Load("killerDillo_base");
            Vector3 loc = _node->GetWorldPosition();
            Quaternion rot;
            rot.Identity();
            // !FAKE: c2 register allocation; neutral stand-ins v0, p; the PDB names no local for the zone (edi, read
            // before the string temps) or the new critter (edi)
            unsigned int v0 = _node->GetZone();
            Critter* p = pManager->Create("chuteKillerDillo", "ChuteKillerDilloMain", "killerdillo", model, loc, rot, v0);
            p->GetScript()->Message(this, 7, this);
            p->GetScript()->Message(this, ChuteKillerDilloMain::MESSAGE_DROP, 0);
        }
    }
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x000A0D59
// PROTOTYPE: PREVIEW 0x0048BDEC near-identical
// NAME: proto-pdb:default.pdb
ChuteKillerDilloMain::ChuteKillerDilloMain(SwampSceneNode* node) : KillerDilloMain(node)
{
    _chute = 0;
    ((Critter*)_node)->field_260 = false;
}
#endif

// FUNCTION: USA 0x000A0DFA
// PROTOTYPE: PREVIEW 0x0048BEA9 identical
// NAME: proto-pdb:default.pdb
SwampScript* ChuteKillerDilloMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\maintunnel\\MainTunnel.h", 129) ChuteKillerDilloMain(node);
}
