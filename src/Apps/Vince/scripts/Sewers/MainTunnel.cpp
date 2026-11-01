// UNIT: src/Apps/Vince/scripts/Sewers/MainTunnel.cpp OBJ: Xbox\OfficialRelease\MainTunnel.obj OPT: /O1 /Oy
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

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class AnimationResource;     // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void SetAnimSpeed(float speed);
    float GetAnimSpeed();
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
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
    virtual void FrameTick();
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

    char field_4C[0x64 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class MainTunnelRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    MainTunnelRegionMain(SwampSceneNode* node);
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
    virtual void Activate();
private:
    virtual void SetState(int state);
public:
    ResourceHandle<AnimationResource> _animIdle; // 0xBC
    ResourceHandle<AnimationResource> _animOpen; // 0xC0
    char field_C4[0xCC - 0xC4];        // 0xC4
    bool _open;                        // 0xCC
};
DECOMP_SIZE_ASSERT(ArmorDilloChuteMain, 0xD0);

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

// FUNCTION: USA 0x0009F8C9
// PROTOTYPE: PREVIEW 0x0048A94D identical
// NAME: proto-pdb:default.pdb
void WheelPlatformAMain::FrameTick()
{
    // !FAKE: local; retail holds _node in edi across the state case (no PDB local)
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
    // !FAKE: local; retail loads _node into ecx before the _state store (no PDB local)
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
    // !FAKE: local; retail holds _node in esi across the cases (no PDB local)
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
    // !FAKE: local; retail holds _node in esi across both calls of the closed case (no PDB local)
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

// FUNCTION: USA 0x000A0602
// PROTOTYPE: PREVIEW 0x0048B686 identical
// NAME: proto-pdb:default.pdb
SwampScript* WheelPlatformAMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\maintunnel\\MainTunnel.h", 38) WheelPlatformAMain(node);
}

// FUNCTION: USA 0x000A0629
// PROTOTYPE: PREVIEW 0x0048B6AD identical
// NAME: proto-pdb:default.pdb
SwampScript* ReverseElevatorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\maintunnel\\MainTunnel.h", 66) ReverseElevatorMain(node);
}

// FUNCTION: USA 0x000A0A59
// PROTOTYPE: PREVIEW 0x0048BAEC identical
// NAME: proto-pdb:default.pdb
SwampScript* MainTunnelRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\maintunnel\\MainTunnel.h", 19) MainTunnelRegionMain(node);
}

// Retail layout: the ctor clears a node bool at 0x260 (proto PDB Critter 0x252 _deathTemporary).
class Critter { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x260];               // 0x00
    bool field_260;                    // 0x260
};

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    char field_BC[0x250 - 0xBC];       // 0xBC
};

class KillerDilloMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    KillerDilloMain(SwampSceneNode* node);

    char field_250[0x324 - 0x250];     // 0x250
};

class ChuteKillerDilloMain : public KillerDilloMain { // NAME: proto-pdb:default.pdb
public:
    ChuteKillerDilloMain(SwampSceneNode* node);

    SwampScript* _chute;               // 0x324
};

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
