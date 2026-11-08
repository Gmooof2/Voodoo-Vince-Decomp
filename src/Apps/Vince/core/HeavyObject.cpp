// UNIT: src/Apps/Vince/core/HeavyObject.cpp OBJ: core\XBox\OfficialRelease\HeavyObject.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

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

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetScript(const SwampString&);
};

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetCollidable(bool b) { _collidable = b; }

    char field_0[0xA8];                // 0x00
    bool _collidable;                  // 0xA8
    char field_A9[0xAC - 0xA9];        // 0xA9
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class Critter : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    bool IsOnFloor() const { return _OnFloor; }

    char field_AC[0x250 - 0xAC];       // 0xAC
    bool _OnFloor;                     // 0x250
    char field_251[0x2C8 - 0x251];     // 0x251
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)GetNode(); }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_PICKUP = 0x14,
        MESSAGE_DROP = 0x15,
        MESSAGE_USE = 0x16,
        MESSAGE_CANCELUSE = 0x17,
        MESSAGE_OBJECTSPECIFIC = 0x18
    };

    virtual ~CarriedObjectMain();
    virtual void FrameTick();
    virtual void Message(SwampScript* source, int message, void* data);

    char field_BC[0xF8 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

class HeavyObjectMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void FrameTick();
};
DECOMP_SIZE_ASSERT(HeavyObjectMain, 0xF8);

// FUNCTION: USA 0x0010F12E
// PROTOTYPE: PREVIEW 0x004F9A4C identical
// NAME: proto-pdb:default.pdb
void HeavyObjectMain::FrameTick()
{
    bool v1 = GetCritter()->IsOnFloor();
    CarriedObjectMain::FrameTick();
    if (GetCritter()->IsOnFloor() && !v1) {
        GetCritter()->SetCollidable(true);
    }
}

// FUNCTION: USA 0x0010F15A
// PROTOTYPE: PREVIEW 0x004F9A78 identical
// NAME: proto-pdb:default.pdb
void HeavyObjectMain::Message(SwampScript* source, int message, void* data)
{
    CarriedObjectMain::Message(source, message, data);
    if (message == MESSAGE_PICKUP) {
        pManager->GetVince()->SetScript("VinceCarryHeavy");
    }
}
