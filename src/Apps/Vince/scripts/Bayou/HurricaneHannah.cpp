// UNIT: src/Apps/Vince/scripts/Bayou/HurricaneHannah.cpp OBJ: Xbox\OfficialRelease\HurricaneHannah.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

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

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1,
        DontDraw = 0x2
    };

    bool GetRenderingOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }

    char field_0[0x88];                // 0x00
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0x18];               // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class HannahDebrisMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_RESETCRASH = 0
    };

    virtual void TimerEvent(unsigned int);

    char field_BC[0x1C];               // 0xBC
    bool _okToCrash;                   // 0xD8
    float _timeTilNextFlip;            // 0xDC
};
DECOMP_SIZE_ASSERT(HannahDebrisMain, 0xE0);

class LighthouseMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual bool TestActivate() const;

    char field_BC[0x20];               // 0xBC
};
DECOMP_SIZE_ASSERT(LighthouseMain, 0xDC);

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
double RandomFloat(double range);
}

void* operator new(unsigned int size, const char* file, int line);

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x13C - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class SwitchSwampMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    SwitchSwampMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(SwitchSwampMain, 0x13C);

// FUNCTION: USA 0x0002771E
// PROTOTYPE: PREVIEW 0x00416517 identical
// NAME: proto-pdb:default.pdb
void HannahDebrisMain::TimerEvent(unsigned int id)
{
    if (id == TIMER_RESETCRASH) {
        _okToCrash = true;
    }
}

// FUNCTION: USA 0x0002772F
// PROTOTYPE: PREVIEW 0x00416528 identical
// NAME: proto-pdb:default.pdb
double BeepLib::RandomFloat(double range)
{
    return (double)Random() * range * (1.0 / 4294967296.0);
}

// FUNCTION: USA 0x0002777C
// PROTOTYPE: PREVIEW 0x00416575 identical
// NAME: proto-pdb:default.pdb
bool LighthouseMain::TestActivate() const
{
    return !_node->GetRenderingOption(SwampSceneNode::DontDraw);
}

// FUNCTION: USA 0x000284BF
// PROTOTYPE: PREVIEW 0x004172F2 identical
// NAME: proto-pdb:default.pdb
SwampScript* SwitchSwampMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\HurricaneHannah.cpp", 59) SwitchSwampMain(node);
}
