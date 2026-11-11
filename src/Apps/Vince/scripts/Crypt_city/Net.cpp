// UNIT: src/Apps/Vince/scripts/Crypt_city/Net.cpp OBJ: Xbox\OfficialRelease\Net.obj OPT: /O1 /Oy
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
template <class T, class A = allocator<T> > class vector {
public:
    ~vector() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
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

class Critter; // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

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
};
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};
class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_CARRIED = 1,
        STATE_USE = 2,
        STATE_OBJECTSPECIFIC = 3
    };
    virtual ~CarriedObjectMain();
private:

    char field_BC[0xF8 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

// FUNCTION: USA 0x00069D9E
// NAME: proto-pdb:default.pdb
class NetMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_SWIPING = 3
    };
    NetMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void AnimationInterrupted();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    ResourceHandle<AnimationResource> _swipe; // 0xF8
    bool _caughtFlies;                 // 0xFC
    std::vector<Critter*> _fliesList;  // 0x100
};
DECOMP_SIZE_ASSERT(NetMain, 0x110);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0006972D
// NAME: proto-pdb:default.pdb
void NetMain::AnimationInterrupted()
{
    AnimationFinished();
}

// FUNCTION: USA 0x00069732
// NAME: proto-pdb:default.pdb
void NetMain::AnimationFinished()
{
    if (_state == STATE_SWIPING) {
        SetState(STATE_CARRIED);
        pManager->_vinceCache->GetScript()->Message(this, 0x14, 0); // src proto-pdb:default.pdb MESSAGE_OBJECT_FINISHED
    }
}

// FUNCTION: USA 0x00069DC1
// PROTOTYPE: PREVIEW 0x004562D6 identical
// NAME: proto-pdb:default.pdb
SwampScript* NetMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\gwistlethorpe\\Net.h", 44) NetMain(node);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1NetMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_NetMain_dtor(NetMain* p)
{
    p->NetMain::~NetMain();
}

class CarriedObject : public Critter { // NAME: proto-pdb:default.pdb
};

class Net : public CarriedObject { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    Net();
private:
    static SwampSceneNode* creationFunc();
    // !FAKE: opaque shell; size from the retail push (0x308), the PDB says 0x2F8
    char field_0[0x308];               // 0x00
};

// FUNCTION: USA 0x00069E08
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100, bytes provisional (masked 1): ctor ??0Net@@QAE@XZ at 0x00069DE8 unpinned (needs pin + re-cut)
SwampSceneNode* Net::creationFunc()
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\gwistlethorpe\\Net.h", 18) Net;
}
#endif

class SwampNodeManager { // NAME: proto-pdb:default.pdb
public:
    static SwampNodeManager& Instance();
    void Add(const SwampString& name, SwampSceneNode* (*func)());
};

// FUNCTION: USA 0x00069E2A
// NAME: proto-pdb:default.pdb
Net::RegisterMe::RegisterMe()
{
    SwampNodeManager::Instance().Add("Net", creationFunc);
}

// FUNCTION: USA 0x00069E66
// NAME: proto-pdb:default.pdb
NetMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("NetMain", creationFunc);
}
