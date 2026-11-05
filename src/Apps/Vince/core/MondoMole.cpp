// UNIT: src/Apps/Vince/core/MondoMole.cpp OBJ: core\XBox\OfficialRelease\MondoMole.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    int compare(const E* _Ptr) const;

private:
    unsigned int _pad00[0x1C / 4];
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x8];                // 0x24
    float _length;                     // 0x2C
    char field_30[0x4];                // 0x30
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim._Resource->_length; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x10];               // 0x28
    float _time;                       // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    T Fetch();

    char field_0[0x14];                // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> FetchAnim() { return _animQueue.Fetch(); }

    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

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

class Critter; // NAME: proto-pdb:default.pdb

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter();
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder (SwampSceneNode at 0x0), not modelled
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void SetAnimSpeed(float);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void MakeDead(bool);
};

inline Critter* CritterMain::GetCritter() { return (Critter*)_node; }

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_MONSTER_KILLED = 0x14
    };
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
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_GOAWAY = 0x14,
        MESSAGE_APPEAR = 0x15,
        MESSAGE_APPEAR_INSTANT = 0x16,
        MESSAGE_DISAPPEAR = 0x17
    };

    MondoMoundMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void AnimationFinished();
    virtual void SetState(int);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    MultiAnimation _multiAnimMoundAppear;    // 0xBC
    MultiAnimation _multiAnimMoundDisappear; // 0xD0
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
        STATE_WINDUP = 5,
        STATE_ATTACK = 6,
        STATE_ATTACK_DANGEROUS = 7,
        STATE_ATTACK_FINISH = 8,
        STATE_DAMAGE = 9,
        STATE_APPEAR1 = 0xA,
        STATE_APPEAR2 = 0xB,
        STATE_DISAPPEAR = 0xC,
        STATE_BLOCK_START = 0xD,
        STATE_BLOCK_HOLD = 0xE,
        STATE_BLOCK_END = 0xF,
        STATE_CELEBRATE = 0x10,
        STATE_DEAD = 0x11
    };
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_HIDE_REST = 0,
        TIMER_BORED = 1,
        TIMER_APPEAR = 2,
        TIMER_IDLE_REST = 3,
        TIMER_IDLE_FINISH = 4,
        TIMER_BLOCK_HOLD = 5
    };

    MondoMoleMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void Deactivate();
    virtual void TimerEvent(unsigned int timerId);
    virtual void AnimationFinished();
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
    virtual void SetState(int);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    bool _active;                    // 0x240
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

// FUNCTION: USA 0x0011FC71
// PROTOTYPE: PREVIEW 0x0050896D identical
// NAME: proto-pdb:default.pdb
void MondoMoundMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_GOAWAY:
        SetState(STATE_GOAWAY);
        break;
    case MESSAGE_APPEAR:
        SetState(STATE_APPEAR);
        break;
    case MESSAGE_APPEAR_INSTANT:
        SetState(STATE_APPEAR_INSTANT);
        break;
    case MESSAGE_DISAPPEAR:
        SetState(STATE_DISAPPEAR);
        break;
    default:
        CritterMain::Message(source, message, data);
        break;
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

// FUNCTION: USA 0x0011FE29
// PROTOTYPE: PREVIEW 0x00508B25 identical
// NAME: proto-pdb:default.pdb
void MondoMoleMain::AnimationFinished()
{
    switch (_state) {
    case STATE_IDLE_LAUGH:
    case STATE_DAMAGE:
        SetState(STATE_DISAPPEAR);
        break;
    case STATE_WINDUP:
        SetState(STATE_ATTACK);
        break;
    case STATE_ATTACK:
    case STATE_ATTACK_DANGEROUS:
    case STATE_ATTACK_FINISH:
        SetState(STATE_IDLE_REST);
        break;
    case STATE_APPEAR1:
    case STATE_APPEAR2:
        SetState(STATE_IDLE_READY);
        break;
    case STATE_DISAPPEAR:
        SetState(STATE_HIDE_REST);
        break;
    case STATE_BLOCK_START:
        SetState(STATE_BLOCK_HOLD);
        break;
    case STATE_BLOCK_END:
        SetState(STATE_WINDUP);
        break;
    case STATE_DEAD:
        Message(this, MESSAGE_MONSTER_KILLED, 0);
        GetCritter()->MakeDead(true);
        DeathFX();
        break;
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

// FUNCTION: USA 0x0011FF33
// PROTOTYPE: PREVIEW 0x00508C2F identical
// NAME: proto-pdb:default.pdb
void MondoMoleMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    switch (_state) {
    case STATE_ATTACK:
        if (value > prev && name == "attack")
            SetState(STATE_ATTACK_DANGEROUS);
        break;
    case STATE_ATTACK_DANGEROUS:
        if (value < prev && name == "attack")
            SetState(STATE_ATTACK_FINISH);
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

// FUNCTION: USA 0x001208DD
// PROTOTYPE: PREVIEW 0x005095D9 identical
// NAME: proto-pdb:default.pdb
void MondoMoundMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_HIDDEN:
        _node->SetOption(SwampSceneNode::DontDraw, true, false);
        break;
    case STATE_APPEAR:
        _node->SetOption(SwampSceneNode::DontDraw, false, false);
        GetCritter()->StartAnimation(_multiAnimMoundAppear.FetchAnim(), false, false);
        GetCritter()->SetAnimSpeed(3.0f);
        break;
    case STATE_APPEAR_INSTANT: {
        _node->SetOption(SwampSceneNode::DontDraw, false, false);
        // !FAKE: c1xx front end; temporary; the AddRef/SubRef copy needs a named handle, no PDB local (stand-in)
        ResourceHandle<AnimationControl> tmp = GetCritter()->StartAnimation(_multiAnimMoundAppear.FetchAnim(), false, false);
        tmp().SetAnimationTime(tmp().GetAnimationLength());
        break;
    }
    case STATE_DISAPPEAR:
    case STATE_GOAWAY:
        GetCritter()->StartAnimation(_multiAnimMoundDisappear.FetchAnim(), false, false);
        GetCritter()->SetAnimSpeed(1.0f);
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

// FUNCTION: USA 0x0012130A
// PROTOTYPE: PREVIEW 0x00509FCE identical
// NAME: proto-pdb:default.pdb
SwampScript* MondoMoleMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\MondoMole.h", 17) MondoMoleMain(node);
}
