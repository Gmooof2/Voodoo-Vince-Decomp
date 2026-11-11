// UNIT: src/Apps/Vince/scripts/french_quarter/DoorStorageRoom.cpp OBJ: Xbox\OfficialRelease\DoorStorageRoom.obj OPT: /O1 /Oy
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
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString& id, int def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb

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
    void SetAnimSpeed(float);
    char field_B4[0x14C];              // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0xC8];              // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class DoorStorageRoom : public Critter { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampSceneNode* creationFunc();
};
DECOMP_SIZE_ASSERT(DoorStorageRoom, 0x2C8);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString& name, unsigned int flags, SwampSceneNode* parent, bool bAttach,
                          float minDist3D, float maxDist3D);
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
    void RegisterTimerEvent(unsigned int, float, bool);

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

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class DoorStorageRoomMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum State { // NAME: proto-pdb:default.pdb
        Closed = 0,
        Opening = 1,
        Open = 2,
        Closing = 3,
        Cracked = 4
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_DOOR_CRACK = 0x14,
        MESSAGE_DOOR_OPEN = 0x15,
        MESSAGE_DOOR_CLOSE = 0x16
    };

    DoorStorageRoomMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void TimerEvent(unsigned int);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    ResourceHandle<AnimationResource> _open; // 0xBC
private:
    DoorStorageRoom* GetDoor() { return static_cast<DoorStorageRoom*>(_node); }
    static SwampScript* creationFunc(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(DoorStorageRoomMain, 0xC0);

// FUNCTION: USA 0x00079324
// PROTOTYPE: PREVIEW 0x00465338 identical
// NAME: proto-pdb:default.pdb
void DoorStorageRoomMain::Activate()
{
    if (_state == Closed) {
        GetDoor()->StartAnimation(_open, false, false);
        GetDoor()->SetAnimSpeed(1.0f);
        SetState(Opening);
    } else if (_state == Cracked) {
        GetDoor()->SetAnimSpeed(1.0f);
        SetState(Opening);
    }
}

// FUNCTION: USA 0x0007935F
// PROTOTYPE: PREVIEW 0x00465373 identical
// NAME: proto-pdb:default.pdb
void DoorStorageRoomMain::TimerEvent(unsigned int)
{
    GetDoor()->SetAnimSpeed(0.0f);
}

// FUNCTION: USA 0x00079418
// PROTOTYPE: PREVIEW 0x0046542C identical
// NAME: proto-pdb:default.pdb
void DoorStorageRoomMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_DOOR_CRACK:
        if (_state == Closed) {
            RegisterTimerEvent(1, 0.5f, false);
            Activate();
            SetState(Cracked);
            SoundManager::PlaySound("qb_doorpartial", 0x20, _node, true, 0.0f, 0.0f);
        }
        break;
    case MESSAGE_DOOR_OPEN:
        Activate();
        SoundManager::PlaySound("qb_doorOpen", 0x20, _node, true, 0.0f, 0.0f);
        break;
    case MESSAGE_DOOR_CLOSE:
        if (_state != Closed) {
            SoundManager::PlaySound("qb_doorpartial", 0x20, _node, true, 0.0f, 0.0f);
            GetDoor()->SetAnimSpeed(-1.0f);
            SetState(Closing);
        }
        break;
    }
}

// FUNCTION: USA 0x00079539
// PROTOTYPE: PREVIEW 0x0046554D identical
// NAME: proto-pdb:default.pdb
void DoorStorageRoomMain::Load(const BeepTDF& tdf)
{
    int v1 = tdf.GetIntRecord("state", 0);
    if (v1 == Open) {
        ResourceHandle<AnimationControl> tmp = GetDoor()->StartAnimation(_open, false, false);
        tmp().SetAnimationTime(tmp().GetAnimationLength());
        SetState(Open);
    }
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x000795AC
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); gate pin_rows: ctor pin 0x000793AB is site-corr, relocated through by this row (D102); lift = a disasm-/corr-verified ctor pin (5.3 T5)
// 5.4 wall (T6.c4): promote-corr refuses the 0x000793AB ctor pin `route-near:no-agreeing-site`, so D102 still applies
SwampScript* DoorStorageRoomMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\DoorStorageRoom.h", 22) DoorStorageRoomMain(node);
}
#endif

// FUNCTION: USA 0x0007960F
// NAME: proto-pdb:default.pdb
DoorStorageRoomMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("DoorStorageRoomMain", creationFunc);
}

class SwampNodeManager { // NAME: proto-pdb:default.pdb
public:
    static SwampNodeManager& Instance();
    void Add(const SwampString& name, SwampSceneNode* (*func)());
};

// FUNCTION: USA 0x000795D3
// NAME: proto-pdb:default.pdb
DoorStorageRoom::RegisterMe::RegisterMe()
{
    SwampNodeManager::Instance().Add("DoorStorageRoom", creationFunc);
}
