// UNIT: src/Apps/Vince/scripts/Bayou/jeanlafittesship.cpp OBJ: Xbox\OfficialRelease\jeanlafittesship.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

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
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
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

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwitchMain();
    virtual bool TestActivate() const;
    virtual void Message(SwampScript*, int, void*);
    virtual void TimerEvent(unsigned int);
    void SetOnAnimName(const SwampString& name);
    void SetDelay(float delay) { _delay = delay; }

    char field_BC[0xFC - 0xBC];        // 0xBC
    float _delay;                      // 0xFC
    char field_100[0x11C - 0x100];     // 0x100
    SwampString _target;               // 0x11C
    bool _popupOK;                     // 0x138
    bool _oneWayThrow;                 // 0x139
    char field_13A[0x13C - 0x13A];     // 0x13A
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
public:
    BasicWallSwitchMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void AnimationFinished();
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class Switch_ShipDoorMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    Switch_ShipDoorMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Switch_ShipDoorMain, 0x13C);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x110 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class WindowShipMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    WindowShipMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(WindowShipMain, 0x110);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb

    char field_4C[0x64 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class JeanLaFittesShipRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    JeanLaFittesShipRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(JeanLaFittesShipRegionMain, 0x64);

// FUNCTION: USA 0x00022D90
// NAME: proto-pdb:default.pdb
Switch_ShipDoorMain::Switch_ShipDoorMain(SwampSceneNode* node) : BasicWallSwitchMain(node)
{
    SetOnAnimName("SwitchSwamp_down");
    _oneWayThrow = true;
    SetDelay(-1.0f);
    _target = "windowShip";
}

// FUNCTION: USA 0x00022E0F
// PROTOTYPE: PREVIEW 0x00411D10 identical
// NAME: proto-pdb:default.pdb
SwampScript* Switch_ShipDoorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("JeanLaFittesShip\\jeanlafittesship.cpp", 76) Switch_ShipDoorMain(node);
}

// FUNCTION: USA 0x00022EC2
// PROTOTYPE: PREVIEW 0x00411DC3 identical
// NAME: proto-pdb:default.pdb
SwampScript* WindowShipMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("JeanLaFittesShip\\jeanlafittesship.cpp", 57) WindowShipMain(node);
}

// FUNCTION: USA 0x00022EE9
// PROTOTYPE: PREVIEW 0x00411DEA identical
// NAME: proto-pdb:default.pdb
SwampScript* JeanLaFittesShipRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("JeanLaFittesShip\\jeanlafittesship.cpp", 24) JeanLaFittesShipRegionMain(node);
}
