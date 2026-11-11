// UNIT: src/Apps/Vince/scripts/french_quarter/WallSwitch.cpp OBJ: Xbox\OfficialRelease\WallSwitch.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

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
class AnimationControl;      // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x34 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;

    char field_0[0x8];                 // 0x00
    SwampString _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    char field_A4[0x200 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
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

// FUNCTION: USA 0x0007C254
// NAME: proto-pdb:default.pdb
class WallSwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        TURN_OFF = 0x14,
        MESSAGE_QUERY_PULLED_ONCE = 0x15
    };
    enum State { // NAME: proto-pdb:default.pdb
        Off = 0x0,
        TurningOn = 0x1,
        On = 0x2,
        TurningOff = 0x3
    };

    WallSwitchMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void AnimationFinished();
    Critter* GetSwitch() { return (Critter*)_node; }

    ResourceHandle<AnimationResource> _turnOn; // 0xBC
    SwampString _padName;              // 0xC0
    int _pulledOnce;                   // 0xDC

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(WallSwitchMain, 0xE0);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1WallSwitchMain@@UAE@XZ in this unit (its ctor
// is unmatched); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WallSwitchMain_dtor(WallSwitchMain* p)
{
    p->WallSwitchMain::~WallSwitchMain();
}

// FUNCTION: USA 0x0007C359
// PROTOTYPE: PREVIEW 0x00468308 identical
// NAME: proto-pdb:default.pdb
void WallSwitchMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 7:
        _padName = source->_node->_Name;
        break;
    case TURN_OFF:
        GetSwitch()->StartAnimation(_turnOn, false, true);
        SetState(TurningOff);
        SoundManager::PlaySound("qb_blendstp", 0x20, _node, true, 0.0f, 0.0f);
        break;
    case MESSAGE_QUERY_PULLED_ONCE:
        *(int*)data = _pulledOnce;
        break;
    }
}

// FUNCTION: USA 0x0007C3F4
// PROTOTYPE: PREVIEW 0x004683A3 identical
// NAME: proto-pdb:default.pdb
void WallSwitchMain::AnimationFinished()
{
    if (_state == TurningOn) {
        SetState(On);
        // !FAKE: c1xx front end; stand-in local; the PDB records no local, retail ends the string temporary before the null test
        Critter* p = pManager->Find("Blender");
        if (p)
            p->GetScript()->Message(this, 0x14, 0); // src proto-pdb:default.pdb TURN_ON
    } else {
        SetState(Off);
    }
}

// FUNCTION: USA 0x0007C457
// NAME: proto-pdb:default.pdb
SwampScript* WallSwitchMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\WallSwitch.h", 11) WallSwitchMain(node);
}

// FUNCTION: USA 0x0007C47E
// NAME: proto-pdb:default.pdb
WallSwitchMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WallSwitchMain", creationFunc);
}
