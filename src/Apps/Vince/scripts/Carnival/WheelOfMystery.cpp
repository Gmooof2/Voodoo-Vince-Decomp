// UNIT: src/Apps/Vince/scripts/Carnival/WheelOfMystery.cpp OBJ: Xbox\OfficialRelease\WheelOfMystery.obj OPT: /O1 /Oy
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

class Critter; // NAME: proto-pdb:default.pdb

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter();
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class ExtraLifeMain : public CritterMain { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(ExtraLifeMain, 0xBC);

class ExtraLifeOfMysteryMain : public ExtraLifeMain { // NAME: proto-pdb:default.pdb
public:
    ExtraLifeOfMysteryMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ExtraLifeOfMysteryMain, 0xBC);

// FUNCTION: USA 0x0003B7AB
// PROTOTYPE: PREVIEW 0x00429E6A identical
// NAME: proto-pdb:default.pdb
class WheelOfMysteryMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    WheelOfMysteryMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_BC[0xC8 - 0xBC];        // 0xBC
    SwampString _switchName;           // 0xC8
    SwampString _lastPrizeName;        // 0xE4
    char field_100[0x108 - 0x100];     // 0x100
};
DECOMP_SIZE_ASSERT(WheelOfMysteryMain, 0x108);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1WheelOfMysteryMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WheelOfMysteryMain_dtor(WheelOfMysteryMain* p)
{
    p->WheelOfMysteryMain::~WheelOfMysteryMain();
}

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x13C - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class switch_wheelOfMysteryMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    switch_wheelOfMysteryMain(SwampSceneNode* node);
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
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(WheelOfMysteryDoorsMain, 0x110);

// FUNCTION: USA 0x0003BEED
// PROTOTYPE: PREVIEW 0x0042A636 identical
// NAME: proto-pdb:default.pdb
SwampScript* ExtraLifeOfMysteryMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\WheelOfMystery.cpp", 319) ExtraLifeOfMysteryMain(node);
}

// FUNCTION: USA 0x0003BFAF
// PROTOTYPE: PREVIEW 0x0042A6F8 identical
// NAME: proto-pdb:default.pdb
SwampScript* WheelOfMysteryMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\WheelOfMystery.cpp", 32) WheelOfMysteryMain(node);
}

// FUNCTION: USA 0x0003BFD6
// PROTOTYPE: PREVIEW 0x0042A71F identical
// NAME: proto-pdb:default.pdb
SwampScript* switch_wheelOfMysteryMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\WheelOfMystery.cpp", 384) switch_wheelOfMysteryMain(node);
}

// FUNCTION: USA 0x0003C062
// PROTOTYPE: PREVIEW 0x0042A7AB identical
// NAME: proto-pdb:default.pdb
SwampScript* WheelOfMysteryDoorsMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\WheelOfMystery.cpp", 271) WheelOfMysteryDoorsMain(node);
}

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;
};

// Retail layout: Critter byte proto 0x252 read at retail 0x260.
class Critter : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetSpecies(SwampString);

    char field_0[0x260];               // 0x00
    bool field_260;                    // 0x260
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

inline Critter* CritterMain::GetCritter() { return (Critter*)_node; }

class PitFrogMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    PitFrogMain(SwampSceneNode* node);

    char field_BC[0x39C - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(PitFrogMain, 0x39C);

class FrogOfMysteryMain : public PitFrogMain { // NAME: proto-pdb:default.pdb
public:
    FrogOfMysteryMain(SwampSceneNode* node);
    virtual ~FrogOfMysteryMain();
};
DECOMP_SIZE_ASSERT(FrogOfMysteryMain, 0x39C);

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1 = the vptr store)
// FUNCTION: USA 0x0003C202
// PROTOTYPE: PREVIEW 0x0042A94B near-identical
// NAME: proto-pdb:default.pdb
FrogOfMysteryMain::FrogOfMysteryMain(SwampSceneNode* node) : PitFrogMain(node)
{
    Critter* wheel = pManager->Find("WheelOfMystery");
    wheel->GetScript()->Message(this, 0x14, 0); // src proto-pdb:default.pdb MESSAGE_ONEMOREFROG
    GetCritter()->SetSpecies("pitfrog");
    GetCritter()->field_260 = false;
}
#endif
