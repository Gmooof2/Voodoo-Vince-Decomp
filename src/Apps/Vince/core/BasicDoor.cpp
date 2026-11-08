// UNIT: src/Apps/Vince/core/BasicDoor.cpp OBJ: core\XBox\OfficialRelease\BasicDoor.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class T, class A = allocator<T> > class vector {
public:
    T* begin() { return _Myfirst; }
    T* end() { return _Mylast; }

private:
    char _pad00[0x4];                  // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;
};

class Critter : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
};

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

    char field_4[0x48];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual SwampString Save();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum State { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0,
        STATE_OPEN = 1,
        STATE_OPENING = 2,
        STATE_CLOSING = 3,
        STATE_LOCKEDOPEN = 4,
        STATE_BASICDOOR_USER = 5
    };
    virtual void Message(SwampScript*, int, void*);
    virtual void AnimationFinished();
    virtual void Activate();
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
    virtual int GetDefaultState() const;                           // vtable+0x70
    virtual void SetState(int);
    virtual ~BasicDoorMain();

protected:
    void NotifySwitches(int msg);

public:
    char field_BC[0x8];                // 0xBC
    std::vector<SwampString> _ownerSwitches; // 0xC4
    char field_D4[0x3C];               // 0xD4
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

// FUNCTION: USA 0x0010790D
// PROTOTYPE: PREVIEW 0x004F22C1 identical
// NAME: proto-pdb:default.pdb
void BasicDoorMain::NotifySwitches(int msg)
{
    for (SwampString* i = _ownerSwitches.begin(); i != _ownerSwitches.end(); ++i) {
        SwampString name = *i;
        Critter* p = pManager->Find(name);
        if (p)
            p->GetScript()->Message(this, msg, 0);
    }
}

// FUNCTION: USA 0x0010796C
// PROTOTYPE: PREVIEW 0x004F2320 identical
// NAME: proto-pdb:default.pdb
void BasicDoorMain::Activate()
{
    if (_state == STATE_CLOSED)
        SetState(STATE_OPENING);
    else if (_state == STATE_OPEN)
        NotifySwitches(8);
}
