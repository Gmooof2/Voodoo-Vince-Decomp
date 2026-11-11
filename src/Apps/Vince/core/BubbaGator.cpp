// UNIT: src/Apps/Vince/core/BubbaGator.cpp OBJ: core\XBox\OfficialRelease\BubbaGator.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
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
template <class T, class A = allocator<T> > class vector {
public:
    unsigned int capacity() const { return (_Myfirst == 0 ? 0 : _Myend - _Myfirst); }

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

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x240 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

class BubbaGatorMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    BubbaGatorMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_BORED = 0,
        TIMER_BLOCK_HOLD = 1,
        TIMER_ATTACK_REST = 2
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_IDLE2 = 1,
        STATE_IDLE3 = 2,
        STATE_PATROL = 3,
        STATE_TAUNT = 4,
        STATE_WINDUP_TAIL = 5,
        STATE_ATTACK_TAIL = 6,
        STATE_WINDUP_BELLY_STRAIGHT = 7,
        STATE_ATTACK_BELLY_STRAIGHT = 8,
        STATE_ATTACK_BITE = 9
    };
    virtual void TimerEvent(unsigned int timerId);
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);

    // retail creationFunc pushes new size 0x388; the prototype PDB says 0x38C; the
    // PDB's second BubbaGatorMain record (size 0x388, no _loopSound) puts _attackCount at 0x384
    char field_240[0x380 - 0x240];     // 0x240
    bool _attackMoving;                // 0x380
    int _attackCount;                  // 0x384
};
DECOMP_SIZE_ASSERT(BubbaGatorMain, 0x388);

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random(); // src proto-pdb:default.pdb Random
}

// FUNCTION: USA 0x0010FB4B
// PROTOTYPE: PREVIEW 0x004FA3DE identical
// NAME: proto-pdb:default.pdb
void BubbaGatorMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_BORED:
        switch (_state) {
        case STATE_IDLE:
            switch (BeepLib::Random() % 3) {
            case 0:
                SetState(STATE_PATROL);
                break;
            case 1:
                SetState(STATE_IDLE2);
                break;
            case 2:
                SetState(STATE_IDLE3);
                break;
            }
            break;
        case STATE_PATROL:
            SetState(STATE_IDLE);
            break;
        }
        break;
    case TIMER_BLOCK_HOLD:
        SetState(STATE_ATTACK_BITE);
        break;
    case TIMER_ATTACK_REST:
        _attackCount--;
        break;
    }
}

// FUNCTION: USA 0x0010FBA5
// PROTOTYPE: PREVIEW 0x004FA438 identical
// NAME: proto-pdb:default.pdb
void BubbaGatorMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    if (_state == STATE_ATTACK_BELLY_STRAIGHT) {
        if (value > prev)
            _attackMoving = true;
        else
            _attackMoving = false;
    }
}

// FUNCTION: USA 0x00111413
// PROTOTYPE: PREVIEW 0x004FBC7A identical
// NAME: proto-pdb:default.pdb
SwampScript* BubbaGatorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\BubbaGator.h", 14) BubbaGatorMain(node);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0011143A
// NAME: proto-pdb:default.pdb
BubbaGatorMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BubbaGatorMain", creationFunc);
}

enum PARAM_TYPES { // NAME: proto-pdb:default.pdb
    PARAM_FLOAT = 0x0,
    PARAM_STRING = 0x1,
    PARAM_VECTOR2 = 0x2,
    PARAM_VECTOR3 = 0x3,
    PARAM_VECTOR4 = 0x4,
    PARAM_VERTEX = 0x5,
    PARAM_QUATERNION = 0x6,
    PARAM_INT = 0x7,
    PARAM_DOTANGLE = 0x8,
};

class Param { // NAME: proto-pdb:default.pdb
public:
    const char* _Name;                 // 0x00
    PARAM_TYPES _Type;                 // 0x04
    float _Min;                        // 0x08
    float _Max;                        // 0x0C
    const char* _s;                    // 0x10
    void* val;                         // 0x14
    bool _Modify;                      // 0x18
};
DECOMP_SIZE_ASSERT(Param, 0x1C);

// FUNCTION: USA 0x00111476
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<Param,std::allocator<Param> >::capacity()const;
