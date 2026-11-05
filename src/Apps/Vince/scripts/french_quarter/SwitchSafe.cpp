// UNIT: src/Apps/Vince/scripts/french_quarter/SwitchSafe.cpp OBJ: Xbox\OfficialRelease\SwitchSafe.obj OPT: /O1 /Oy
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

    void RegisterTimerEvent(unsigned int id, float deltaTime, bool looped);
    void RemoveTimerEvents();

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    T* _Resource;                      // 0x00
};
class AnimationResource;     // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00

    SwampScript* GetScript() const;

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    const T* operator->() const { return _Object; }

    T* _Object;                        // 0x00
};

class BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    unsigned int HitTag() const { return _tag; }

    char field_0[0x4];                 // 0x00
    unsigned int _tag;                 // 0x04
    char field_8[0x40 - 0x8];          // 0x08
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class FireFlyNode { // NAME: proto-pdb:default.pdb
public:
    void Play();
    void Reset();
    void StopSpawning();
};

class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    FireFlyNode* Find(const SwampString&);

    char field_0[0x2C];                // 0x00
};
DECOMP_SIZE_ASSERT(SwampParticleManager, 0x2C);
extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

class SwitchSafeMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_POPUP = 0,
        TIMER_PIGDEAD = 1
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_UP = 0,
        STATE_GOINGDOWN = 1,
        STATE_DOWN = 2,
        STATE_GOINGUP = 3,
        STATE_WAITINGTOGOUP = 4
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_POPUP = 0x14
    };

    virtual void AnimationFinished();
    virtual void Message(SwampScript*, int, void*);
    virtual void HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr);
    void SetSparks(bool);
private:
    void ResetPopupTimer();
public:

    ResourceHandle<AnimationResource> _on;  // 0xBC
    ResourceHandle<AnimationResource> _off; // 0xC0
};
DECOMP_SIZE_ASSERT(SwitchSafeMain, 0xC4);

// FUNCTION: USA 0x0007EA16
// PROTOTYPE: PREVIEW 0x0046A920 identical
// NAME: proto-pdb:default.pdb
void SwitchSafeMain::ResetPopupTimer()
{
    RegisterTimerEvent(TIMER_POPUP, 1.0f, false);
}

// FUNCTION: USA 0x0007EADD
// PROTOTYPE: PREVIEW 0x0046A9E7 identical
// NAME: proto-pdb:default.pdb
void SwitchSafeMain::HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr)
{
    if (type == VerticalCollision && cr->HitTag() == 1) {
        // !FAKE: c1xx front end; stand-in local; ends the string temporaries before the compare and GetScript (one
        // expression keeps a sete bool / calls GetScript before _Tidy)
        Critter* tmp = pManager->Find("bigpig");
        if (critterHitBy == tmp) {
            if (_state == STATE_WAITINGTOGOUP) {
                RemoveTimerEvents();
                ResetPopupTimer();
            }
            if (_state == STATE_UP) {
                tmp = pManager->Find("SafeHookRope");
                tmp->GetScript()->Message(this, 0x14, 0); // proto-pdb:default.pdb SafeHookRopeMain::MESSAGE_DROP
                GetCritter()->StartAnimation(_on, false, false);
                SetState(STATE_DOWN);
            }
        }
    }
}

// FUNCTION: USA 0x0007EBA6
// PROTOTYPE: PREVIEW 0x0046AAB0 identical
// NAME: proto-pdb:default.pdb
void SwitchSafeMain::SetSparks(bool b)
{
    // !FAKE: c1xx front end; stand-in local; the PDB records no local, retail calls Find once and keeps it in esi
    FireFlyNode* tmp = g_particleManager->Find("particle_safeSwitch");
    if (b) {
        tmp->Reset();
        tmp->Play();
    } else {
        tmp->StopSpawning();
    }
}

// FUNCTION: USA 0x0007EBF7
// NAME: proto-pdb:default.pdb
void SwitchSafeMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_POPUP:
        if (_state == STATE_DOWN) {
            ResetPopupTimer();
            SetState(STATE_WAITINGTOGOUP);
        }
        break;
    case 6: // name unknown
        SetSparks(false);
        break;
    }
}

// FUNCTION: USA 0x0007ED1B
// PROTOTYPE: PREVIEW 0x0046AC25 identical
// NAME: proto-pdb:default.pdb
void SwitchSafeMain::AnimationFinished()
{
    if (_state == STATE_DOWN) {
        SetSparks(true);
    }
}
