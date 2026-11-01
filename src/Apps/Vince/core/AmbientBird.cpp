// UNIT: src/Apps/Vince/core/AmbientBird.cpp OBJ: core\XBox\OfficialRelease\AmbientBird.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits; // src proto-pdb:default.pdb char_traits
template <class T> class allocator; // src proto-pdb:default.pdb allocator
template <class E, class T, class A> class basic_string; // src proto-pdb:default.pdb basic_string
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

    char field_4[0x48];                // 0x04
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

class AmbientBirdMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_BORED = 0
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_PATROL = 1,
        STATE_FLY_OUT = 2,
        STATE_FLY_IN = 3,
        STATE_GONE = 4
    };

    AmbientBirdMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    char field_BC[0x58];               // 0xBC
};
DECOMP_SIZE_ASSERT(AmbientBirdMain, 0x114);

void* operator new(unsigned int size, const char* file, int line);

// FUNCTION: USA 0x001120CF
// PROTOTYPE: PREVIEW 0x004FC90A identical
// NAME: proto-pdb:default.pdb
void AmbientBirdMain::TimerEvent(unsigned int timerId)
{
    if (timerId == TIMER_BORED) {
        switch (_state) {
        case STATE_PATROL:
            SetState(STATE_IDLE);
            break;
        }
    }
}

// FUNCTION: USA 0x001120E6
// PROTOTYPE: PREVIEW 0x004FC921 identical
// NAME: proto-pdb:default.pdb
void AmbientBirdMain::AnimationFinished()
{
    switch (_state) {
    case STATE_IDLE:
    case STATE_FLY_IN:
        SetState(STATE_PATROL);
        break;
    case STATE_FLY_OUT:
        SetState(STATE_GONE);
        break;
    }
}

// FUNCTION: USA 0x0011263A
// PROTOTYPE: PREVIEW 0x004FCE91 identical
// NAME: proto-pdb:default.pdb
SwampScript* AmbientBirdMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\AmbientBird.h", 10) AmbientBirdMain(node);
}
