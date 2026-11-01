// UNIT: src/Apps/Vince/core/MondoMole.cpp OBJ: core\XBox\OfficialRelease\MondoMole.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2
    };

    void SetOption(unsigned int option, bool value, bool recursive);
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
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
    void UnRegisterFrameTick();

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

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~MonsterMain();
    virtual void DeathFX();                                        // vtable+0x70

    char field_BC[0x184];              // 0xBC
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

class MondoMoundMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_HIDDEN = 0,
        STATE_APPEAR = 1,
        STATE_APPEAR_INSTANT = 2,
        STATE_DISAPPEAR = 3,
        STATE_GOAWAY = 4
    };

    MondoMoundMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void SetState(int);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    char field_BC[0x28];               // 0xBC
};
DECOMP_SIZE_ASSERT(MondoMoundMain, 0xE4);

class MondoMoleMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE_REST = 0,
        STATE_IDLE_READY = 1,
        STATE_IDLE_LAUGH = 2,
        STATE_HIDE_REST = 3,
        STATE_HIDE_READY = 4,
        STATE_APPEAR1 = 0xA,
        STATE_APPEAR2 = 0xB,
        STATE_DISAPPEAR = 0xC,
        STATE_BLOCK_END = 0xF
    };
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_HIDE_REST = 0,
        TIMER_BORED = 1,
        TIMER_APPEAR = 2,
        TIMER_IDLE_REST = 3,
        TIMER_IDLE_FINISH = 4,
        TIMER_BLOCK_HOLD = 5
    };

    virtual void Activate();
    virtual void Deactivate();
    virtual void TimerEvent(unsigned int timerId);
    virtual void SetState(int);

    bool _active;                      // 0x240
    char field_241[0xFF];              // 0x241
};
DECOMP_SIZE_ASSERT(MondoMoleMain, 0x340);

void* operator new(unsigned int size, const char* file, int line);

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random(); // src proto-pdb:default.pdb Random
}

// FUNCTION: USA 0x0011FC4F
// PROTOTYPE: PREVIEW 0x0050894B identical
// NAME: proto-pdb:default.pdb
void MondoMoleMain::Activate()
{
    if (!_active) {
        _active = true;
        SetState(STATE_HIDE_READY);
        RegisterFrameTick();
    }
}

// FUNCTION: USA 0x0011FC9E
// PROTOTYPE: PREVIEW 0x0050899A identical
// NAME: proto-pdb:default.pdb
void MondoMoleMain::Deactivate()
{
    if (_active) {
        _active = false;
        UnRegisterFrameTick();
        _node->SetOption(SwampSceneNode::DontDraw, true, false);
    }
}

// FUNCTION: USA 0x0011FEA0
// PROTOTYPE: PREVIEW 0x00508B9C identical
// NAME: proto-pdb:default.pdb
void MondoMoleMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_HIDE_REST:
        if (_state == STATE_HIDE_REST)
            SetState(STATE_HIDE_READY);
        break;
    case TIMER_BORED:
        if (_state == STATE_HIDE_READY)
            SetState(STATE_APPEAR1);
        break;
    case TIMER_APPEAR:
        if (_state == STATE_APPEAR1)
            SetState(STATE_APPEAR2);
        break;
    case TIMER_IDLE_REST:
        if (_state == STATE_IDLE_REST)
            SetState(STATE_IDLE_READY);
        break;
    case TIMER_IDLE_FINISH:
        if (_state >= STATE_IDLE_REST && _state <= STATE_IDLE_READY) {
            if (BeepLib::Random() % 3 == 0)
                SetState(STATE_IDLE_LAUGH);
            else
                SetState(STATE_DISAPPEAR);
        }
        break;
    case TIMER_BLOCK_HOLD:
        SetState(STATE_BLOCK_END);
        break;
    }
}

// FUNCTION: USA 0x0011FF18
// PROTOTYPE: PREVIEW 0x00508C14 identical
// NAME: proto-pdb:default.pdb
void MondoMoundMain::AnimationFinished()
{
    switch (_state) {
    case STATE_DISAPPEAR:
        SetState(STATE_HIDDEN);
        break;
    case STATE_GOAWAY:
        _node->Invalidate();
        break;
    }
}

// FUNCTION: USA 0x00120AD7
// PROTOTYPE: PREVIEW 0x005097B7 identical
// NAME: proto-pdb:default.pdb
SwampScript* MondoMoundMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\MondoMole.h", 104) MondoMoundMain(node);
}
