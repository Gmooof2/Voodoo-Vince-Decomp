// UNIT: src/Apps/Vince/scripts/Bayou/Outhouse.cpp OBJ: Xbox\OfficialRelease\Outhouse.obj OPT: /O1 /Oy
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
    unsigned int _pad00[0x1C / 4];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
class Matrix;                // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Invalidate();

    char field_A4[0xD0 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(SoundNode, 0xD0);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static SoundNode* CreateSound(const SwampString&, unsigned int, SwampSceneNode*, bool);
};

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetEWWOuthouseDoorTime() const { return _ewwOuthouseDoorTime; }

    char field_0[0x76C];               // 0x00
    float _ewwOuthouseDoorTime;        // 0x76C
    char field_770[0xA0C - 0x770];     // 0x770
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

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
    void RegisterTimerEvent(unsigned int, float, bool);
    void UnregisterTimerEvent(unsigned int);

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

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_OFF = 0,
        STATE_TURNINGON = 1,
        STATE_ON = 2,
        STATE_TURNINGOFF = 3
    };
    virtual void Message(SwampScript*, int, void*);
    virtual void TimerEvent(unsigned int);
    void SetDelay(float delay) { _delay = delay; }
    float GetDelay() const { return _delay; }

    char field_BC[0xFC - 0xBC];        // 0xBC
    float _delay;                      // 0xFC
    char field_100[0x13C - 0x100];     // 0x100
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class Switch_DoorOuthouseMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_STARTTICK = 1
    };
private:
    virtual void SetState(int state);
    virtual void TimerEvent(unsigned int id);
    virtual void Message(SwampScript* source, int message, void* data);

    SoundNode* _tickSound;             // 0x13C
};
DECOMP_SIZE_ASSERT(Switch_DoorOuthouseMain, 0x140);

// FUNCTION: USA 0x0002285A
// PROTOTYPE: PREVIEW 0x00411761 identical
// NAME: proto-pdb:default.pdb
void Switch_DoorOuthouseMain::Message(SwampScript* source, int message, void* data)
{
    if (message == 0x16) {
        UnregisterTimerEvent(TIMER_STARTTICK);
        if (_tickSound)
            _tickSound->Invalidate();
        _tickSound = 0;
    }
    BasicWallSwitchMain::Message(source, message, data);
}

// FUNCTION: USA 0x000228E4
// PROTOTYPE: PREVIEW 0x004117B7 identical
// NAME: proto-pdb:default.pdb
void Switch_DoorOuthouseMain::SetState(int state)
{
    switch (state) {
    case STATE_TURNINGON: {
        // !FAKE: c2 register allocation; stand-in local; retail keeps the time on the x87 stack (fst), SetDelay/GetDelay copied it as int (match_one 75.61 -> 100.0)
        float v0 = gSettings.GetEWWOuthouseDoorTime();
        SetDelay(v0);
        RegisterTimerEvent(TIMER_STARTTICK, v0 - 5.0f, false);
        break;
    }
    case STATE_TURNINGOFF:
        if (_tickSound) {
            _tickSound->Invalidate();
            _tickSound = 0;
        }
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x00022B1D
// PROTOTYPE: PREVIEW 0x004119A9 identical
// NAME: proto-pdb:default.pdb
void Switch_DoorOuthouseMain::TimerEvent(unsigned int id)
{
    switch (id) {
    case TIMER_STARTTICK:
        _tickSound = SoundManager::CreateSound("timertik", 5, GetNode(), true);
        break;
    default:
        BasicWallSwitchMain::TimerEvent(id);
        break;
    }
}
