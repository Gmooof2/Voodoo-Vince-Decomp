// UNIT: src/Apps/Vince/scripts/french_quarter/Scale.cpp OBJ: Xbox\OfficialRelease\Scale.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

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
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString& id, int def) const;
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

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

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim._Resource->_length; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x10];               // 0x28
    float _time;                       // 0x38
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;

    char field_0[0xA4];                // 0x00
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
    char field_B4[0x14C];              // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0xC8];              // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
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
    void RegisterTimerEvent(unsigned int id, float time, bool repeat);
    void UnregisterTimerEvent(unsigned int id);

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

class DoorStorageRoomMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_DOOR_CRACK = 0x14,
        MESSAGE_DOOR_OPEN = 0x15,
        MESSAGE_DOOR_CLOSE = 0x16
    };
};

// FUNCTION: USA 0x0007BC8B
// NAME: proto-pdb:default.pdb
class ScaleMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum State { // NAME: proto-pdb:default.pdb
        Up = 0x0,
        GoingDown = 0x1,
        Down = 0x2,
        GoingUp = 0x3,
        StayingDown = 0x4
    };

    ScaleMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int timerID);
    virtual void AnimationFinished();
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
    void RestartTimer();
    Critter* GetScale() { return static_cast<Critter*>(_node); }
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    bool _bVince;                      // 0xBC
    ResourceHandle<AnimationResource> _vinceOn; // 0xC0
    ResourceHandle<AnimationResource> _vinceOff; // 0xC4
    ResourceHandle<AnimationResource> _flourOn; // 0xC8
    ResourceHandle<AnimationResource> _flourOff; // 0xCC
    bool _vinceLanded;                 // 0xD0
};
DECOMP_SIZE_ASSERT(ScaleMain, 0xD4);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1ScaleMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_ScaleMain_dtor(ScaleMain* p)
{
    p->ScaleMain::~ScaleMain();
}

// FUNCTION: USA 0x0007BA93
// PROTOTYPE: PREVIEW 0x00467A9A identical
// NAME: proto-pdb:default.pdb
void ScaleMain::RestartTimer()
{
    UnregisterTimerEvent(1);
    RegisterTimerEvent(1, 0.3f, false);
}

// FUNCTION: USA 0x0007BAB4
// NAME: proto-pdb:default.pdb
void ScaleMain::TimerEvent(unsigned int timerID)
{
    if (_state != StayingDown && timerID == 1) {
        if (_bVince)
            GetScale()->StartAnimation(_vinceOff, false, false);
        else
            GetScale()->StartAnimation(_flourOff, false, false);
        SetState(GoingUp);
    }
}

// FUNCTION: USA 0x0007BF74
// PROTOTYPE: PREVIEW 0x00467F4A identical
// NAME: proto-pdb:default.pdb
void ScaleMain::AnimationFinished()
{
    // !FAKE: c2 register allocation; neutral stand-in local; retail keeps the Find result in ebx across the switch
    Critter* v0 = pManager->Find("DoorStorageRoom");
    switch (_state) {
    case GoingDown:
        RestartTimer();
        v0->GetScript()->Message(this, DoorStorageRoomMain::MESSAGE_DOOR_CRACK, 0);
        SetState(Down);
        break;
    case GoingUp:
        SetState(Up);
        v0->GetScript()->Message(this, DoorStorageRoomMain::MESSAGE_DOOR_CLOSE, 0);
        break;
    case StayingDown:
        v0->GetScript()->Message(this, DoorStorageRoomMain::MESSAGE_DOOR_OPEN, 0);
        break;
    }
}

// FUNCTION: USA 0x0007C006
// PROTOTYPE: PREVIEW 0x00467FDC identical
// NAME: proto-pdb:default.pdb
void ScaleMain::Load(const BeepTDF& tdf)
{
    // !FAKE: c2 register allocation; neutral stand-in local; retail keeps the state record in edi
    int v1 = tdf.GetIntRecord("state", 0);
    _vinceLanded = tdf.GetBoolRecord("vinceLanded", false);
    if (v1 == StayingDown) {
        // !FAKE: c1xx front end; neutral stand-in local; the returned handle copy retail AddRefs and SubRefs
        ResourceHandle<AnimationControl> tmp = GetScale()->StartAnimation(_vinceOn, false, false);
        tmp().SetAnimationTime(tmp().GetAnimationLength());
        SetState(StayingDown);
    }
}

// FUNCTION: USA 0x0007C0A4
// PROTOTYPE: PREVIEW 0x0046807A identical
// NAME: proto-pdb:default.pdb
SwampString ScaleMain::Save()
{
    // !FAKE: c2 register allocation; dummy local; retail loads _state into esi before the first format call (no PDB local; same form as MirrorUrnMain::Save 86.57 -> 100.0)
    int v0 = _state;
    return format("\t\tstate = %d;\n", v0) + format("\t\tvinceLanded = %d;\n", _vinceLanded);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0007C106
// PROTOTYPE: PREVIEW 0x004680DC identical
// NAME: proto-pdb:default.pdb
SwampScript* ScaleMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\Scale.h", 11) ScaleMain(node);
}

// FUNCTION: USA 0x0007C12D
// NAME: proto-pdb:default.pdb
ScaleMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("ScaleMain", creationFunc);
}
