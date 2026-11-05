// UNIT: src/Apps/Vince/scripts/Crypt_city/MoleGatePuzzle.cpp OBJ: Xbox\OfficialRelease\MoleGatePuzzle.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as Oven.cpp
#include <xstring>
#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specialization before <string> (as init.cpp): <stdexcept>'s logic_error
// instantiates the copy ctor (C2908 after it); retail calls it out of line (0x00011753)
template <> std::basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
#endif
#include <string>
#include <deque>
#include <queue>
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class Popup; // NAME: proto-pdb:default.pdb

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> std::basic_string<char,std::char_traits<char>,std::allocator<char> >::const_iterator basic_string<char,std::char_traits<char>,std::allocator<char> >::end()const;
template <> basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string(const char *);
template <> void basic_string<char,std::char_traits<char>,std::allocator<char> >::_Tidy(bool,unsigned int);
template <> int basic_string<char,std::char_traits<char>,std::allocator<char> >::compare(const char *)const;
template <> basic_string<char,std::char_traits<char>,std::allocator<char> >& basic_string<char,std::char_traits<char>,std::allocator<char> >::append(const basic_string<char,std::char_traits<char>,std::allocator<char> >&,unsigned int,unsigned int);
template <> void deque<Popup *,std::allocator<Popup *> >::_Tidy(); // retail 0x0002E6E8 (named fold alias)
}
#endif

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString& id, int def) const;
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
    static void Return(float);
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion(const Vector3& axis, float angle) { Set(axis, angle); }
    void Set(const Vector3&, float);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

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
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specialization (C0132, C0154); retail calls it out of line (0x000185BB, fold alias)
namespace std {
template <> void deque<ResourceHandle<AnimationResource>,std::allocator<ResourceHandle<AnimationResource> > >::_Tidy();
}
#endif

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    std::deque<T, std::allocator<T> > _queue; // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim._Resource->_length; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x10];               // 0x28
    float _time;                       // 0x38
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class SwampScript;

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;
    unsigned int GetZone() const;
    const SwampString& GetName() const { return _Name; }
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const struct AABox& GetBounds() const;                 // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual struct Quaternion GetWorldRotation() const;            // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10

    int _Type;                         // 0x04
    SwampString _Name;                 // 0x08
};

// src proto-pdb:default.pdb GatorNode base CollidableCylinder (SwampSceneNode at 0x0), not modelled
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void SetAnimSpeed(float);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void StayIdle(bool);
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Create(const SwampString&, const SwampString&, const char*, ResourceHandle<ModelResource>, const Vector3&,
                    const Quaternion&, unsigned int);
    Critter* Find(const SwampString&);
    void ActivateMatching(const SwampString&, unsigned int);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00 (not reached)
    Vince* _vinceCache;                // 0x28
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger* Find(const SwampString& name);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    ModelManager& GetModelManager() const { return *_modelManager; }
    RegionManager& GetRegionManager() const { return *_regionManager; }

    char field_0[0x28];                // 0x00
    ModelManager* _modelManager;       // 0x28
    char field_2C[0x4];                // 0x2C
    RegionManager* _regionManager;     // 0x30
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random(); // NAME: proto-pdb:default.pdb
}

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

    void RegisterTimerEvent(unsigned int timerId, float time, bool repeat);
    void RegisterFrameTick();
    void UnRegisterFrameTick();

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)_node; }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class Popup : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_POPUP = 0,
        TIMER_DONE = 1
    };
    virtual void Activate();
    virtual void TimerEvent(unsigned int timerId);

    CritterMain* _tomb;                // 0x4C
    class PopupManager* _manager;      // 0x50
};
DECOMP_SIZE_ASSERT(Popup, 0x54);

class PopupManager { // NAME: proto-pdb:default.pdb
public:
    ~PopupManager() { ClearQueue(); }
    void InitQueue();
    void ClearQueue();
    void DequePopup();

    bool _enabled;                     // 0x00
    std::queue<Popup*, std::deque<Popup*, std::allocator<Popup*> > > _popups; // 0x04
};
DECOMP_SIZE_ASSERT(PopupManager, 0x18);

class MoleHole { // NAME: proto-pdb:default.pdb
public:
    MoleHole();
    void Occupy() { _occupied = true; }
    bool IsOccupied() { return _occupied; }
    const Vector3& GetLocation() { return _loc; }

    Vector3 _loc;                      // 0x00
    bool _occupied;                    // 0x0C
};
DECOMP_SIZE_ASSERT(MoleHole, 0x10);

// FUNCTION: USA 0x0005F149
// PROTOTYPE: PREVIEW 0x0044B5B0 identical
// NAME: proto-pdb:default.pdb
// retail ??1 has no vptr store: the class declares no dtor (C0029)
class MondoMolePuzzleRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void FrameTick();
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void EnteringRegion(SwampSceneNode* node);
    int GetMoleCount() { return _moleCount; }
    int GetMoleHoleRows() { return _locatorRows; }
    int GetMoleHoleCols() { return _locatorCols; }
    bool IsHoleOccupied(int row, int col) { return _moleHole[row][col].IsOccupied(); }
    const Vector3& GetHoleLocation(int row, int col) { return _moleHole[row][col].GetLocation(); }
    void OccupyHole(int row, int col) { _moleHole[row][col].Occupy(); }
    void DecrementMoleCount() { --_moleCount; }
    static MondoMolePuzzleRegionMain* GetPuzzleScript();
protected:
    MondoMolePuzzleRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    // !FAKE: access; Popup::TimerEvent reaches _popupManager directly (retail GetPuzzleScript()+0x4C)
    friend class Popup;
    // !FAKE: access; MoleGateMain::Message reaches _popupManager directly (retail GetPuzzleScript()+0x4C)
    friend class MoleGateMain;

    // retail: Random() % rows is `and 7`, % cols is `div 5`
    static const int _locatorRows = 8;
    static const int _locatorCols = 5;

    PopupManager _popupManager;        // 0x4C
    int _moleCount;                    // 0x64
    // pdb_types prints MoleHole[5][8] inner count first; retail indexes (row*5 + col)*0x10
    MoleHole _moleHole[8][5];          // 0x68
    bool _started;                     // 0x2E8
    bool _solved;                      // 0x2E9
};
DECOMP_SIZE_ASSERT(MondoMolePuzzleRegionMain, 0x2EC);

class MoleTombMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        MESSAGE_POPUP = 0x14
    };
    enum { // NAME: proto-pdb:default.pdb
        STATE_DOWN = 0,
        STATE_UP = 1
    };
    MoleTombMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void Load(const BeepTDF& tdf);
    virtual ~MoleTombMain();
private:
    void CreateMole();
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _rise; // 0xBC
    Critter* _mole;                    // 0xC0
};
DECOMP_SIZE_ASSERT(MoleTombMain, 0xC4);

class MoleTomb_AMain : public MoleTombMain { // NAME: proto-pdb:default.pdb
public:
    MoleTomb_AMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(MoleTomb_AMain, 0xC4);

class MoleGateMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_GATE_CLOSE = 0x14,
        MESSAGE_GATE_OPEN = 0x15,
        MESSAGE_GATE_INSTANTOPEN = 0x16,
        MESSAGE_QUERYSOLVED = 0x17,
        MESSAGE_SETSOLVED = 0x18
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_OPEN = 0,
        STATE_CLOSED = 1
    };
    MoleGateMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
    virtual ~MoleGateMain();

    ResourceHandle<AnimationResource> _open; // 0xBC
    bool _solved;                      // 0xC0
};
DECOMP_SIZE_ASSERT(MoleGateMain, 0xC4);

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~MonsterMain();

    char field_BC[0x240 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

// FUNCTION: USA 0x0005FDE6
// NAME: proto-pdb:default.pdb
// retail ??1 has no vptr store: the class declares no dtor (C0029)
class MondoMoleMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Activate();

    bool _active;                      // 0x240
    float _targetTimer;                // 0x244
    MultiAnimation _multiAnimIdle;     // 0x248
    MultiAnimation _multiAnimWindup;   // 0x25C
    MultiAnimation _multiAnimWindupAlt; // 0x270
    MultiAnimation _multiAnimAttack;   // 0x284
    MultiAnimation _multiAnimAttackAlt; // 0x298
    MultiAnimation _multiAnimAppear;   // 0x2AC
    MultiAnimation _multiAnimDisappear; // 0x2C0
    MultiAnimation _multiAnimIdleHurt; // 0x2D4
    MultiAnimation _multiAnimIdleLaugh; // 0x2E8
    MultiAnimation _multiAnimBlockStart; // 0x2FC
    MultiAnimation _multiAnimBlockHold; // 0x310
    MultiAnimation _multiAnimBlockEnd; // 0x324
    Critter* _mound;                   // 0x338
    bool _altAttack;                   // 0x33C
};
DECOMP_SIZE_ASSERT(MondoMoleMain, 0x340);

class GateMoleMain : public MondoMoleMain { // NAME: proto-pdb:default.pdb
public:
    GateMoleMain(SwampSceneNode* node);
    virtual void Activate();
    void SetTombScript(CritterMain* tomb) { _tomb = tomb; }
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    CritterMain* _tomb;                // 0x340
    int _holeRow;                      // 0x344
    int _holeCol;                      // 0x348
};
DECOMP_SIZE_ASSERT(GateMoleMain, 0x34C);

// FUNCTION: USA 0x0005EDC6
// PROTOTYPE: PREVIEW 0x0044B205 identical
// NAME: proto-pdb:default.pdb
void MondoMolePuzzleRegionMain::Message(SwampScript* source, int message, void* data)
{
    if (message == 2) {
        _started = false;
        UnRegisterFrameTick();
    }
}

// FUNCTION: USA 0x0005EDF9
// PROTOTYPE: PREVIEW 0x0044B238 identical
// NAME: proto-pdb:default.pdb
void Popup::Activate()
{
    RegisterTimerEvent(TIMER_POPUP, 1.5f, false);
    RegisterTimerEvent(TIMER_DONE, 3.0f, false);
}

// FUNCTION: USA 0x0005EE26
// PROTOTYPE: PREVIEW 0x0044B265 identical
// NAME: proto-pdb:default.pdb
MoleHole::MoleHole()
{
    _occupied = false;
}

// FUNCTION: USA 0x0005EEB1
// PROTOTYPE: PREVIEW 0x0044B2F0 identical
// NAME: proto-pdb:default.pdb
template std::reverse_iterator<std::basic_string<char,std::char_traits<char>,std::allocator<char> >::const_iterator> std::basic_string<char,std::char_traits<char>,std::allocator<char> >::rbegin()const;

// FUNCTION: USA 0x0005EF4B
// PROTOTYPE: PREVIEW 0x0044B3B2 identical
// NAME: proto-pdb:default.pdb
void PopupManager::InitQueue()
{
    ClearQueue();
    _enabled = true;
}

// FUNCTION: USA 0x0005EF58
// NAME: proto-pdb:default.pdb
void MoleGateMain::Load(const BeepTDF& tdf)
{
    // !FAKE: stand-in local; the PDB lists none
    int state = tdf.GetIntRecord("state", STATE_OPEN);
    if (state == STATE_OPEN)
        Message(this, MESSAGE_GATE_INSTANTOPEN, 0);
    SetState(state);
    if (GetCritter()->GetName().compare("molegate_a") == 0)
        _solved = tdf.GetBoolRecord("solved", false);
}

// FUNCTION: USA 0x0005EFE9
// NAME: proto-pdb:default.pdb
SwampString MoleGateMain::Save()
{
    SwampString s = format("\tstate = %d;\n", _state);
    if (GetCritter()->GetName().compare("molegate_a") == 0)
        s +=format("\tsolved = %d;\n", _solved);
    return s;
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1MondoMolePuzzleRegionMain@@UAE@XZ because
// this unit defines the ctor 0x5F06C (not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MondoMolePuzzleRegionMain_dtor(MondoMolePuzzleRegionMain* p)
{
    p->MondoMolePuzzleRegionMain::~MondoMolePuzzleRegionMain();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1MondoMoleMain@@UAE@XZ here (GateMoleMain,
// not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MondoMoleMain_dtor(MondoMoleMain* p)
{
    p->MondoMoleMain::~MondoMoleMain();
}

// FUNCTION: USA 0x0005F168
// PROTOTYPE: PREVIEW 0x0044B5CF identical
// NAME: proto-pdb:default.pdb
void MondoMolePuzzleRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (node == pManager->GetVince()) {
        Critter* gate = pManager->Find("molegate_a");
        gate->GetScript()->Message(this, MoleGateMain::MESSAGE_QUERYSOLVED, &_solved);
        if (!_solved) {
            if (!_started) {
                gate->GetScript()->Message(this, MoleGateMain::MESSAGE_GATE_CLOSE, 0);
                // !FAKE: stand-in local; ends the string temporary before GetScript (retail edi); the PDB lists none
                Critter* gateB = pManager->Find("molegate_b");
                gateB->GetScript()->Message(this, MoleGateMain::MESSAGE_GATE_CLOSE, 0);
                AnimCam::SwitchTo("camera_moleGatesClose", 0.0f, false, 0.0f);
                RegisterTimerEvent(0, 2.0f, false);
                pManager->GetVince()->StayIdle(true);
                RegisterFrameTick();
                _started = true;
                _popupManager.InitQueue();
            }
            pManager->ActivateMatching("moletomb", 0);
        }
    }
}

// FUNCTION: USA 0x0005F2C4
// PROTOTYPE: PREVIEW 0x0044B72B identical
// NAME: proto-pdb:default.pdb
void MondoMolePuzzleRegionMain::FrameTick()
{
    if (GetMoleCount() == 0 && !_solved) {
        _solved = true;
        {
            Critter* gate = pManager->Find("molegate_a");
            gate->GetScript()->Message(this, MoleGateMain::MESSAGE_SETSOLVED, &_solved);
        }
        AnimCam::SwitchTo("camera_moleGatesOpen", 0.0f, false, 0.0f);
        RegisterTimerEvent(0, 2.0f, false);
        pManager->GetVince()->StayIdle(true);
        {
            Critter* door = pManager->Find("moleGate_A");
            door->GetScript()->Message(this, MoleGateMain::MESSAGE_GATE_OPEN, 0);
            door = pManager->Find("moleGate_B");
            door->GetScript()->Message(this, MoleGateMain::MESSAGE_GATE_OPEN, 0);
        }
        UnRegisterFrameTick();
    }
}

// FUNCTION: USA 0x0005F400
// PROTOTYPE: PREVIEW 0x0044B867 identical
// NAME: proto-pdb:default.pdb
MondoMolePuzzleRegionMain* MondoMolePuzzleRegionMain::GetPuzzleScript()
{
    // !FAKE: stand-in local; the PDB lists none
    RegionTrigger* region = g_swamp->GetRegionManager().Find("MondoMolePuzzle");
    return (MondoMolePuzzleRegionMain*)region->GetScript();
}

// FUNCTION: USA 0x0005F43D
// PROTOTYPE: PREVIEW 0x0044B8A4 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 94.80, bytes mismatch (frame and slots equal; retail stores pManager to [ebp-8] after
// Quaternion::Set and loads _node into edi before it; plain pManager-> is loaded at the call, frame 0x80)
void MoleTombMain::CreateMole()
{
    // !FAKE: stand-in local; the PDB lists none
    int moleCount = MondoMolePuzzleRegionMain::GetPuzzleScript()->GetMoleCount();
    // !FAKE: c2 register allocation; stand-in locals assigned in the call order retail loads them (C0221, Kosmo.cpp)
    CritterManager* cm;
    _mole = (cm = pManager)->Create(format("gatemole%d", moleCount),
                             "gatemole", "mondomole", g_swamp->GetModelManager().Load("mondoMole_base"),
                             _node->GetWorldPosition(), Quaternion(Vector3(0.0f, 0.0f, 1.0f), 0.0f),
                             _node->GetZone());
    ((GateMoleMain*)_mole->GetScript())->SetTombScript(this);
    _mole->GetScript()->Activate();
}
#endif

// FUNCTION: USA 0x0005F5D2
// PROTOTYPE: PREVIEW 0x0044BA39 identical
// NAME: proto-pdb:default.pdb
void MoleTombMain::Load(const BeepTDF& tdf)
{
    // !FAKE: stand-in local; the PDB lists none
    int state = tdf.GetIntRecord("state", STATE_DOWN);
    SetState(state);
    if (state == STATE_UP) {
        // !FAKE: stand-in local; the AddRef/SubRef copy needs a named handle, no PDB local
        ResourceHandle<AnimationControl> anim = GetCritter()->StartAnimation(_rise, false, true);
        GetCritter()->SetAnimSpeed(0.0f);
    }
}

// FUNCTION: USA 0x0005F64C
// NAME: proto-pdb:default.pdb
void Popup::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_POPUP: {
        // !FAKE: stand-in local; the PDB lists none
        char c = *_tomb->GetCritter()->GetName().rbegin();
        AnimCam::Return(0.0f);
        AnimCam::SwitchTo(format("camera_moleTomb_%c", c), 0.0f, false, 0.0f);
        _tomb->Message(this, MoleTombMain::MESSAGE_POPUP, 0);
        break;
    }
    case TIMER_DONE:
        MondoMolePuzzleRegionMain::GetPuzzleScript()->_popupManager.DequePopup();
        MondoMolePuzzleRegionMain::GetPuzzleScript()->DecrementMoleCount();
        InvalidateScript();
        break;
    }
}

// FUNCTION: USA 0x0005F6F1
// PROTOTYPE: PREVIEW 0x0044BB58 identical
// NAME: proto-pdb:default.pdb
SwampScript* MondoMolePuzzleRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("GatekeeperJam\\MoleGatePuzzle.cpp", 135) MondoMolePuzzleRegionMain(node);
}

// FUNCTION: USA 0x0005F71B
// PROTOTYPE: PREVIEW 0x0044BB82 identical
// NAME: proto-pdb:default.pdb
void GateMoleMain::Activate()
{
    // !FAKE: stand-in locals; the PDB lists none
    MondoMolePuzzleRegionMain* puzzle;
    int row;
    int col;
    do {
        puzzle = MondoMolePuzzleRegionMain::GetPuzzleScript();
        row = BeepLib::Random() % puzzle->GetMoleHoleRows();
        col = BeepLib::Random() % puzzle->GetMoleHoleCols();
    } while (puzzle->IsHoleOccupied(row, col));
    _node->SetWorldPosition(puzzle->GetHoleLocation(row, col));
    puzzle->OccupyHole(row, col);
    MondoMoleMain::Activate();
}

// FUNCTION: USA 0x0005F771
// PROTOTYPE: PREVIEW 0x0044BBD8 identical
// NAME: proto-pdb:default.pdb
void MoleGateMain::Message(SwampScript* from, int message, void* data)
{
    switch (message) {
    case 2:
        if (GetCritter()->GetName().compare("molegate_a") == 0) {
            MondoMolePuzzleRegionMain::GetPuzzleScript()->Message(this, 2, 0);
            Message(this, MESSAGE_GATE_OPEN, 0);
            // !FAKE: stand-in local; ends the string temporary before GetScript (retail ebx); the PDB lists none
            Critter* gateB = pManager->Find("molegate_b");
            gateB->GetScript()->Message(this, MESSAGE_GATE_OPEN, 0);
        }
        MondoMolePuzzleRegionMain::GetPuzzleScript()->_popupManager.ClearQueue();
        break;
    case MESSAGE_GATE_CLOSE:
        SoundManager::PlaySound("ccgj_gatesh", 0, _node, true, 0.0f, 0.0f);
        GetCritter()->StartAnimation(_open, false, true);
        SetState(STATE_CLOSED);
        break;
    case MESSAGE_GATE_OPEN:
        if (_state != STATE_OPEN) {
            SoundManager::PlaySound("ccgj_gateop", 0, _node, true, 0.0f, 0.0f);
            GetCritter()->StartAnimation(_open, false, false);
            SetState(STATE_OPEN);
        }
        break;
    case MESSAGE_GATE_INSTANTOPEN: {
        // !FAKE: stand-in local; the AddRef/SubRef copy needs a named handle, no PDB local
        ResourceHandle<AnimationControl> anim = GetCritter()->StartAnimation(_open, false, false);
        anim().SetAnimationTime(anim().GetAnimationLength());
        SetState(STATE_OPEN);
        break;
    }
    case MESSAGE_QUERYSOLVED:
        *(bool*)data = _solved;
        break;
    case MESSAGE_SETSOLVED:
        _solved = *(bool*)data;
        break;
    }
}

// FUNCTION: USA 0x0005F9D0
// PROTOTYPE: PREVIEW 0x0044BE0C identical
// NAME: proto-pdb:default.pdb
void MoleTombMain::Activate()
{
    if (_state == STATE_DOWN && _mole == 0)
        CreateMole();
}

// FUNCTION: USA 0x0005FA8D
// PROTOTYPE: PREVIEW 0x0044BEF7 identical
// NAME: proto-pdb:default.pdb
SwampScript* MoleTombMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("GatekeeperJam\\MoleGatePuzzle.cpp", 58) MoleTombMain(node);
}

// FUNCTION: USA 0x0005FC5F
// PROTOTYPE: PREVIEW 0x0044BFCD identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 25.0, bytes match (cut-extent wall: retail cut symbol runs past the
// row; va_dis no difference; fix is row-end pins + re-cut, source is final; generated, 4.4 T10)
SwampScript* MoleTomb_AMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("GatekeeperJam\\MoleGatePuzzle.cpp", 673) MoleTomb_AMain(node);
}
#endif

// FUNCTION: USA 0x00060184
// PROTOTYPE: PREVIEW 0x0044C639 identical
// NAME: proto-pdb:default.pdb
SwampScript* GateMoleMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("GatekeeperJam\\MoleGatePuzzle.cpp", 108) GateMoleMain(node);
}
