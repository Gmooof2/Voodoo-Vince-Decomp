// UNIT: src/Apps/Vince/scripts/Carnival/TrailerFrogs.cpp OBJ: Xbox\OfficialRelease\TrailerFrogs.obj OPT: /O1 /Oy
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

class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
}

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    T Fetch();

    char field_0[0x14];                // 0x00
};

// Retail layout: Critter byte proto 0x252 read at retail 0x260.
class Critter { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x260];               // 0x00
    bool field_260;                    // 0x260

    void PlaySound(const SwampString& name, unsigned int flags, bool bAttach, float minDist3D, float maxDist3D);
};

void* operator new(unsigned int size, const char* file, int line);

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

protected:
    void RegisterSaveLoadVar(const SwampString&, bool*, bool);

public:
    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x2D - 0x24];        // 0x24
    bool _sleeping;                    // 0x2D
    char field_2E[0x4C - 0x2E];        // 0x2E
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class PitFrogMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    PitFrogMain(SwampSceneNode* node);

    char field_4C[0x39C - 0x4C];       // 0x4C
};
DECOMP_SIZE_ASSERT(PitFrogMain, 0x39C);

// FUNCTION: USA 0x0003DEB9
// NAME: proto-pdb:default.pdb
// ??_GPitFrogMain unmatched, no draft; fold-name wall: this VA is ??_GTrailerFrogMain (after ??0TrailerFrogMain 0x0003DE5D, pin only,
// candidates fail) folded with ??_GPitFrogMain, whose vtable is PitFrog.obj's; match_one: `not defined in TrailerFrogs.obj` (C0320, 5.3 T6)
class TrailerFrogMain : public PitFrogMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
public:
    TrailerFrogMain(SwampSceneNode* node);
    virtual void Activate();

    bool _awake;                       // 0x39C

private:
    static int _numFrogs; // NAME: proto-pdb:default.pdb
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(TrailerFrogMain, 0x3A0);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual SwampString Save();                                    // vtable+0x58
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
protected:
    virtual void SetState(int);                                    // vtable+0x6C
public:
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwitchMain();
    virtual bool TestActivate() const;
    virtual void TimerEvent(unsigned int);
    virtual void Message(SwampScript*, int, void*);
    void SetOnAnimName(const SwampString&);
    void SetDelay(float delay) { _delay = delay; }

    char field_BC[0xFC - 0xBC];        // 0xBC
    float _delay;                      // 0xFC
    char field_100[0x139 - 0x100];     // 0x100
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

// FUNCTION: USA 0x0003DA72
// NAME: proto-pdb:default.pdb
// ??_Gswitch_wheelOfMysteryMain unmatched, no draft; fold-name wall: this VA is ??_GSwitch_TrailerFrogsMain (after its ctor 0x0003DA1F)
// folded (12 ??_G names); the row's name is WheelOfMystery.obj's class; match_one: `not defined in TrailerFrogs.obj` (C0320, 5.3 T6)
class Switch_TrailerFrogsMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
public:
    Switch_TrailerFrogsMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Switch_TrailerFrogsMain, 0x13C);

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    void ActivateMatching(const SwampString&, unsigned int);

    char field_0[0x2C];                // 0x00
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript*, int, void*);

    char field_BC[0x110 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class TrailerFrogsMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_FROGNOISE = 1
    };
    TrailerFrogsMain(SwampSceneNode* node);
    virtual void Message(SwampScript*, int, void*);
    virtual void TimerEvent(unsigned int id);

    SwampShuffleQueue<SwampString> _frogNoises; // 0x110
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(TrailerFrogsMain, 0x124);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0003DA13
// PROTOTYPE: PREVIEW 0x0042BEF6 identical
// NAME: proto-pdb:default.pdb
void TrailerFrogMain::Activate()
{
    _awake = true;
    _sleeping = false;
}

// FUNCTION: USA 0x0003DBFF
// PROTOTYPE: PREVIEW 0x0042C0C6 identical
// NAME: proto-pdb:default.pdb
void TrailerFrogsMain::Message(SwampScript* source, int message, void* data)
{
    if (message == 6) {
        if (_state)
            pManager->ActivateMatching("trailerfrog", 0);
        else
            RegisterTimerEvent(TIMER_FROGNOISE, 1.0f, false);
    }
    BasicDoorMain::Message(source, message, data);
}

// FUNCTION: USA 0x0003DA1F
// NAME: proto-pdb:default.pdb
Switch_TrailerFrogsMain::Switch_TrailerFrogsMain(SwampSceneNode* node) : BasicWallSwitchMain(node)
{
    SetOnAnimName("SwitchCarnival_down");
    _oneWayThrow = true;
    SetDelay(-1.0f);
}

// FUNCTION: USA 0x0003DC66
// PROTOTYPE: PREVIEW 0x0042C12D identical
// NAME: proto-pdb:default.pdb
SwampScript* Switch_TrailerFrogsMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\TrailerFrogs.cpp", 10) Switch_TrailerFrogsMain(node);
}

// FUNCTION: USA 0x0003DC8D
// PROTOTYPE: PREVIEW 0x0042C154 identical
// NAME: proto-pdb:default.pdb
void TrailerFrogsMain::TimerEvent(unsigned int id)
{
    if (id == TIMER_FROGNOISE && _state == 0) {
        {
            // !FAKE: c2 block layout; dummy local; hoist node load before Fetch and the pushes (match_one 78.35 -> 96.73)
            Critter* critter = GetCritter();
            // !FAKE: c1xx front end; comma expression with pointer local; Fetch's temp passed through eax before the float pushes (match_one 96.73 -> 100.0)
            const SwampString* p;
            critter->PlaySound((p = &_frogNoises.Fetch(), *p), 0x28, true, 0.0f, 0.0f);
        }
        RegisterTimerEvent(TIMER_FROGNOISE, BeepLib::RandomFloat(0.5f, 2.0f), false);
    }
}

// FUNCTION: USA 0x0003DDFA
// PROTOTYPE: PREVIEW 0x0042C2C1 identical
// NAME: proto-pdb:default.pdb
SwampScript* TrailerFrogsMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\TrailerFrogs.cpp", 93) TrailerFrogsMain(node);
}

// FUNCTION: USA 0x0003DF11
// PROTOTYPE: PREVIEW 0x0042C3BC identical
// NAME: proto-pdb:default.pdb
SwampScript* TrailerFrogMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\TrailerFrogs.cpp", 21) TrailerFrogMain(node);
}

// FUNCTION: USA 0x0003DE5D
// PROTOTYPE: PREVIEW 0x0042C324 near-identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1 = vptr store; needs pin ??_7TrailerFrogMain@@6B@ at
// 0x001CF6C0); source is final; 5.4 T15.c3
TrailerFrogMain::TrailerFrogMain(SwampSceneNode* node) : PitFrogMain(node)
{
    _numFrogs = 0;
    RegisterSaveLoadVar("awake", &_awake, false);
    ((Critter*)_node)->field_260 = false;
}
#endif

// FUNCTION: USA 0x0003DE21
// NAME: proto-pdb:default.pdb
Switch_TrailerFrogsMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("Switch_TrailerFrogsMain", creationFunc);
}

// FUNCTION: USA 0x0003DED5
// NAME: proto-pdb:default.pdb
TrailerFrogsMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("TrailerFrogsMain", creationFunc);
}

// FUNCTION: USA 0x0003DF38
// NAME: proto-pdb:default.pdb
TrailerFrogMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("TrailerFrogMain", creationFunc);
}
