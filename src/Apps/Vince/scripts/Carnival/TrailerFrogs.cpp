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
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04
    virtual void Activate();                                       // vtable+0x08
    virtual void Deactivate();                                     // vtable+0x0C
    virtual void FrameTick();                                      // vtable+0x10
    virtual void TimerEvent(unsigned int);                         // vtable+0x14

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

class TrailerFrogMain : public PitFrogMain { // NAME: proto-pdb:default.pdb
public:
    TrailerFrogMain(SwampSceneNode* node);
    virtual void Activate();

    bool _awake;                       // 0x39C

private:
    static int _numFrogs; // NAME: proto-pdb:default.pdb
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(TrailerFrogMain, 0x3A0);

class BasicWallSwitchMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    char field_4C[0x13C - 0x4C];       // 0x4C
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class Switch_TrailerFrogsMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    Switch_TrailerFrogsMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Switch_TrailerFrogsMain, 0x13C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

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

// FUNCTION: USA 0x0003DC66
// PROTOTYPE: PREVIEW 0x0042C12D identical
// NAME: proto-pdb:default.pdb
SwampScript* Switch_TrailerFrogsMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\TrailerFrogs.cpp", 10) Switch_TrailerFrogsMain(node);
}

// FUNCTION: USA 0x0003DC8D
// PROTOTYPE: PREVIEW 0x0042C154 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 96.73, bytes mismatch (retail passes Fetch's eax, not a re-lea of a named temp; 8 drafts)
// 4.3 T16.c12 also tried: const-ref bound Fetch temp, const named temp (96.73); functional-cast temp, declared
// basic_string copy ctor with the plain call (78.35)
void TrailerFrogsMain::TimerEvent(unsigned int id)
{
    if (id == TIMER_FROGNOISE && _state == 0) {
        {
            // !FAKE: c2 block layout; dummy locals; hoist node load and Fetch before the pushes (match_one 78.35 -> 96.73)
            Critter* critter = GetCritter();
            SwampString tmp = _frogNoises.Fetch();
            critter->PlaySound(tmp, 0x28, true, 0.0f, 0.0f);
        }
        RegisterTimerEvent(TIMER_FROGNOISE, BeepLib::RandomFloat(0.5f, 2.0f), false);
    }
}
#endif

// FUNCTION: USA 0x0003DDFA
// PROTOTYPE: PREVIEW 0x0042C2C1 identical
// NAME: proto-pdb:default.pdb
SwampScript* TrailerFrogsMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\TrailerFrogs.cpp", 93) TrailerFrogsMain(node);
}

// FUNCTION: USA 0x0003DF11
// PROTOTYPE: PREVIEW 0x0042C3BC identical
// NAME: proto-pdb:default.pdb
SwampScript* TrailerFrogMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("Midway\\TrailerFrogs.cpp", 21) TrailerFrogMain(node);
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2 = vptr store, literal)
// FUNCTION: USA 0x0003DE5D
// PROTOTYPE: PREVIEW 0x0042C324 near-identical
// NAME: proto-pdb:default.pdb
TrailerFrogMain::TrailerFrogMain(SwampSceneNode* node) : PitFrogMain(node)
{
    _numFrogs = 0;
    RegisterSaveLoadVar("awake", &_awake, false);
    ((Critter*)_node)->field_260 = false;
}
#endif
