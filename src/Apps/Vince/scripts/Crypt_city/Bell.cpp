// UNIT: src/Apps/Vince/scripts/Crypt_city/Bell.cpp OBJ: Xbox\OfficialRelease\Bell.obj OPT: /O1 /Oy
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

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString&, int) const;
    bool GetBoolRecord(const SwampString&, bool) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

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

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                         // vtable+0x00
    SwampScript* GetScript() const;

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x8];                // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0x8];                // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0x2C8 - 0x200];     // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

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
    void RegisterTimerEvent(unsigned int, float, bool);

    char field_4[0x1C];               // 0x04 (_name; not reached)
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

// FUNCTION: USA 0x0006C1F2
// NAME: proto-pdb:default.pdb
// ??_GBellMain (alias ??_EBellMain) unmatched, no draft; wall: a virtual ??_G is emitted only with the class vtable, i.e. with its ctor
// ??0BellMain 0x0006C10A (pin only, site-corr, not a candidate row) (C0320); match_one: `not defined in Bell.obj` (5.3 T6)
// implicit ??1BellMain@@UAE@XZ, emitted through pin_emit_BellMain_dtor below
// FUNCTION: USA 0x0006C20E
// NAME: proto-pdb:default.pdb
class BellMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    BellMain(SwampSceneNode* node);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    bool ReadyToOpenDoor();

    int _mirrors;                      // 0xBC
    ResourceHandle<AnimationResource> _leftStrike;  // 0xC0
    ResourceHandle<AnimationResource> _rightStrike; // 0xC4
    Critter* _ringers[2];              // 0xC8
    int _nextRinger;                   // 0xD0
    Critter* _lantern;                 // 0xD4
    bool _puzzleDone;                  // 0xD8
};
DECOMP_SIZE_ASSERT(BellMain, 0xDC);

class BellRingerMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_STRIKE = 0
    };
    enum { // NAME: proto-pdb:default.pdb
        MESSAGE_STRIKE = 0x16
    };
    BellRingerMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int id);
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _strike; // 0xBC
    Critter* _bell;                    // 0xC0
};
DECOMP_SIZE_ASSERT(BellRingerMain, 0xC4);

// FUNCTION: USA 0x0006C681
// NAME: proto-pdb:default.pdb
// ??_GWheelPlatformAMain unmatched, no draft; fold-name wall: this VA is ??_GDoorExitMain (after ??0DoorExitMain 0x0006C614,
// pin only) folded (8 ??_G names); the row's name is MainTunnel.obj's class; match_one: `not defined in Bell.obj` (C0320, 5.3 T6)
class DoorExitMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0x0,
        STATE_OPEN = 0x1
    };
    DoorExitMain(SwampSceneNode* node);
    virtual void Load(const BeepTDF& tdf);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    Critter* GetDoor() { return (Critter*)_node; }

    ResourceHandle<AnimationResource> _open; // 0xBC
};
DECOMP_SIZE_ASSERT(DoorExitMain, 0xC0);

// FUNCTION: USA 0x0006C065
// PROTOTYPE: PREVIEW 0x0045833E identical
// NAME: proto-pdb:default.pdb
SwampString BellMain::Save()
{
    return format("\tstate = %d;\n\tdone = %d;\n", _state, _puzzleDone);
}

// FUNCTION: USA 0x0006C0BE
// PROTOTYPE: PREVIEW 0x00458397 identical
// NAME: proto-pdb:default.pdb
SwampScript* BellRingerMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\gwistlethorpe\\Bell.h", 56) BellRingerMain(node);
}

// FUNCTION: USA 0x0006C0E5
// PROTOTYPE: PREVIEW 0x004583E9 identical
// NAME: proto-pdb:default.pdb
void BellRingerMain::TimerEvent(unsigned int id)
{
    if (id == TIMER_STRIKE)
        _bell->GetScript()->Message(this, MESSAGE_STRIKE, 0);
}

// FUNCTION: USA 0x0006C22D
// PROTOTYPE: PREVIEW 0x00458531 identical
// NAME: proto-pdb:default.pdb
bool BellMain::ReadyToOpenDoor()
{
    int light;
    // !FAKE: c1xx front end; stand-in local; the PDB records no local, retail ends the string temporary before GetScript
    Critter* p = pManager->Find("lantern");
    p->GetScript()->Message(this, 0x14, &light); // src proto-pdb:default.pdb MESSAGE_LIGHTLEVEL
    return _mirrors == 3 && light;
}

// FUNCTION: USA 0x0006C471
// PROTOTYPE: PREVIEW 0x00458775 identical
// NAME: proto-pdb:default.pdb
void BellMain::Load(const BeepTDF& tdf)
{
    // !FAKE: c1xx front end; stand-in local; the PDB records no local, retail ends the string temporary before SetState
    int state = tdf.GetIntRecord("state", 0);
    SetState(state);
    _puzzleDone = tdf.GetBoolRecord("done", false);
}

// FUNCTION: USA 0x0006C4DF
// PROTOTYPE: PREVIEW 0x004587E3 identical
// NAME: proto-pdb:default.pdb
void BellRingerMain::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_STRIKE) {
        if (!_bell)
            _bell = pManager->Find("bell");
        GetCritter()->StartAnimation(_strike, false, false);
        RegisterTimerEvent(TIMER_STRIKE, 0.15f, false);
    }
}

// FUNCTION: USA 0x0006C69D
// PROTOTYPE: PREVIEW 0x00458985 identical
// NAME: proto-pdb:default.pdb
void DoorExitMain::Load(const BeepTDF& tdf)
{
    // !FAKE: c1xx front end; stand-in local; the PDB records no local, retail ends the string temporary before SetState
    int state = tdf.GetIntRecord("state", 0);
    SetState(state);
    if (state == STATE_OPEN) {
        // !FAKE: c1xx front end; temporary; the AddRef/SubRef copy needs a named handle, no PDB local (stand-in)
        ResourceHandle<AnimationControl> tmp = GetDoor()->StartAnimation(_open, false, false);
        tmp().SetAnimationTime(tmp().GetAnimationLength());
    }
}

// FUNCTION: USA 0x0006C70D
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); gate pin_rows: ctor pin 0x0006C10A is site-corr, relocated through by this row (D102); lift = a disasm-/corr-verified ctor pin (5.3 T5)
SwampScript* BellMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\gwistlethorpe\\Bell.h", 11) BellMain(node);
}
#endif

// FUNCTION: USA 0x0006C734
// PROTOTYPE: PREVIEW 0x00458A1C identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 5.2, bytes match (cut-extent wall: retail cut symbol runs past the
// row; va_dis no difference; fix is row-end pins + re-cut, source is final; generated, 4.4 T10)
SwampScript* DoorExitMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\gwistlethorpe\\Bell.h", 87) DoorExitMain(node);
}
#endif

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1BellMain@@UAE@XZ in this unit (its ??_G caller
// sits with the unpinned ctor 0x0006C10A, not matched); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_BellMain_dtor(BellMain* p)
{
    p->BellMain::~BellMain();
}
