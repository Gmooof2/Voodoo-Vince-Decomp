// UNIT: src/Apps/Vince/core/CarriedObject.cpp OBJ: core\XBox\OfficialRelease\CarriedObject.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

class AnimationResource; // NAME: proto-pdb:default.pdb
class AnimationControl;  // NAME: proto-pdb:default.pdb

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    T* _Resource;                      // 0x00
};

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
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
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0x0,
        STATE_CARRIED = 0x1,
        STATE_USE = 0x2,
        STATE_OBJECTSPECIFIC = 0x3
    };

    virtual ~CarriedObjectMain();
    virtual void AnimationFinished();

    ResourceHandle<AnimationResource> _animCarry; // 0xBC
    ResourceHandle<AnimationResource> _animUse;   // 0xC0
    char field_C4[0xD4 - 0xC4];        // 0xC4
    Critter* _owner;                   // 0xD4
    bool _persistentFrameTick;         // 0xD8
    bool _useAnimDone;                 // 0xD9
    bool _oldCollidable;               // 0xDA
    bool _oldEdgeGlow;                 // 0xDB
    char field_DC[0xF8 - 0xDC];        // 0xDC
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

// FUNCTION: USA 0x0010E93E
// PROTOTYPE: PREVIEW 0x004F9284 identical
// NAME: proto-pdb:default.pdb
void CarriedObjectMain::AnimationFinished()
{
    if (_state < STATE_OBJECTSPECIFIC) {
        _useAnimDone = true;
        if (_state != STATE_IDLE) {
            SetState(STATE_CARRIED);
            GetCritter()->StartAnimation(_animCarry, true, false);
        }
    }
}
