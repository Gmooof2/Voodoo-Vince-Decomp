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
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
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
    GatesRegionMain(SwampSceneNode* node);
    virtual void Load(const BeepTDF& tdf);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    bool _solved;                      // 0x4C
    char field_4D[0x54 - 0x4D];        // 0x4D
};
DECOMP_SIZE_ASSERT(GatesRegionMain, 0x54);

class GateTrapMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_OPEN = 0,
        STATE_CLOSED = 1,
        STATE_OPENING = 2,
        STATE_CLOSING = 3
    };
    GateTrapMain(SwampSceneNode* node);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_BC[0xC0 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(GateTrapMain, 0xC0);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb

    char field_4C[0x64 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class MainstreetRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    MainstreetRegionMain(SwampSceneNode* node);
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

class PitFrogMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    PitFrogMain(SwampSceneNode* node);
protected:
    virtual void SetState(int);

public:
    char field_250[0x39C - 0x250];     // 0x250
};

class GateFrogMain : public PitFrogMain { // NAME: proto-pdb:default.pdb
public:
    GateFrogMain(SwampSceneNode* node);
};

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
