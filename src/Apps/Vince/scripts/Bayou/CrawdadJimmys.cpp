// UNIT: src/Apps/Vince/scripts/Bayou/CrawdadJimmys.cpp OBJ: Xbox\OfficialRelease\CrawdadJimmys.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class T, class A> class vector {
public:
    ~vector() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x10];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    const Quaternion& SetRotation(const Quaternion& q);
    void SetVisible(bool b, bool r);
    void SetOption(unsigned int, bool, bool);
protected:
    void InvalidateCachedWorldPos();
public:
    char field_0[0x40];                // 0x00
    Quaternion _Rotation;              // 0x40
};

void* operator new(unsigned int size, const char* file, int line);

// FUNCTION: USA 0x00019D86
// PROTOTYPE: PREVIEW 0x00408FE7 identical
// NAME: proto-pdb:default.pdb
const Quaternion& SwampSceneNode::SetRotation(const Quaternion& q)
{
    InvalidateCachedWorldPos();
    return _Rotation = q;
}

// FUNCTION: USA 0x00019DA1
// PROTOTYPE: PREVIEW 0x00409002 identical
// NAME: proto-pdb:default.pdb
void SwampSceneNode::SetVisible(bool b, bool r)
{
    SetOption(2, !b, r);
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

    char field_4[0x48];                // 0x04
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

class CrawdadJimmyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_CHECKTIME = 1,
        STATE_ASK_CRAWDAD = 2,
        STATE_ASK_ONION = 3,
        STATE_ASK_SAUSAGE = 4,
        STATE_V_NOTHING = 5,
        STATE_GOTALL = 6
    };

    CrawdadJimmyMain(SwampSceneNode* node);
    virtual void GainingControl(SwampScript*);
    virtual void SetState(int);

    bool _didintro;                    // 0xBC
    bool _didchallenge;                // 0xBD
    char field_BE[0x6];                // 0xBE

private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void ResetAmbients();
};
DECOMP_SIZE_ASSERT(CrawdadJimmyMain, 0xC4);

// FUNCTION: USA 0x00019E7A
// PROTOTYPE: PREVIEW 0x004090DB identical
// NAME: proto-pdb:default.pdb
void CrawdadJimmyMain::GainingControl(SwampScript* oldScript)
{
    SetState(STATE_IDLE);
    if (_didintro) {
        ResetAmbients();
    }
}

// FUNCTION: USA 0x00019FD2
// PROTOTYPE: PREVIEW 0x00409233 identical
// NAME: proto-pdb:default.pdb
class ActivatorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    bool _useable;                     // 0xBC
    float _alpha;                      // 0xC0
    SwampString _target;               // 0xC4
};
DECOMP_SIZE_ASSERT(ActivatorMain, 0xE0);

class Activator_FanBoatMain : public ActivatorMain { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(Activator_FanBoatMain, 0xE0);

class Activator_FanBoatAMain : public Activator_FanBoatMain { // NAME: proto-pdb:default.pdb
public:
    Activator_FanBoatAMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Activator_FanBoatAMain, 0xE0);

class Activator_FanBoatBMain : public Activator_FanBoatMain { // NAME: proto-pdb:default.pdb
public:
    Activator_FanBoatBMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Activator_FanBoatBMain, 0xE0);

class Activator_FanBoatCMain : public Activator_FanBoatMain { // NAME: proto-pdb:default.pdb
public:
    Activator_FanBoatCMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Activator_FanBoatCMain, 0xE0);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1ActivatorMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_ActivatorMain_dtor(ActivatorMain* p)
{
    p->ActivatorMain::~ActivatorMain();
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x10];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) ++_Resource->_RefCount; }
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    ResourceHandle& operator=(const ResourceHandle&);
    T* _Resource;                      // 0x00
};

// Retail layout: retail disasm puts _defaultIdleAnim at 0x288 (proto PDB 0x278, Critter 0x2C8).
class Critter { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> GetDefaultIdleAnim() const;
    void SetDefaultIdleAnim(ResourceHandle<AnimationResource> ah);

    char field_0[0x288];               // 0x00
    ResourceHandle<AnimationResource> _defaultIdleAnim; // 0x288
};

// FUNCTION: USA 0x00019E1B
// PROTOTYPE: PREVIEW 0x0040907C near-identical
// NAME: proto-pdb:default.pdb
void Critter::SetDefaultIdleAnim(ResourceHandle<AnimationResource> ah)
{
    _defaultIdleAnim = ah;
}

// FUNCTION: USA 0x00019E39
// PROTOTYPE: PREVIEW 0x0040909A near-identical
// NAME: proto-pdb:default.pdb
ResourceHandle<AnimationResource> Critter::GetDefaultIdleAnim() const
{
    return _defaultIdleAnim;
}

// FUNCTION: USA 0x0001A48C
// PROTOTYPE: PREVIEW 0x00409709 identical
// NAME: proto-pdb:default.pdb
SwampScript* Activator_FanBoatAMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 560) Activator_FanBoatAMain(node);
}

// FUNCTION: USA 0x0001A4B6
// PROTOTYPE: PREVIEW 0x00409733 identical
// NAME: proto-pdb:default.pdb
SwampScript* Activator_FanBoatBMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 569) Activator_FanBoatBMain(node);
}

// FUNCTION: USA 0x0001A4E0
// PROTOTYPE: PREVIEW 0x0040975D identical
// NAME: proto-pdb:default.pdb
SwampScript* Activator_FanBoatCMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 578) Activator_FanBoatCMain(node);
}

// FUNCTION: USA 0x0001A617
// PROTOTYPE: PREVIEW 0x004098A6 identical
// NAME: proto-pdb:default.pdb
SwampScript* CrawdadJimmyMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\crawdadjimmys\\CrawdadJimmys.h", 14) CrawdadJimmyMain(node);
}

// FUNCTION: USA 0x0001A66B
// PROTOTYPE: PREVIEW 0x004098FA identical
// NAME: proto-pdb:default.pdb
class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> _animOpen;   // 0xBC
    ResourceHandle<AnimationResource> _animClose;  // 0xC0
    std::vector<SwampString, std::allocator<SwampString> > _ownerSwitches; // 0xC4
    SwampString _animCam;              // 0xD4
    SwampString _animCamClose;         // 0xF0
    bool _locked;                      // 0x10C
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class GrateExitMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    GrateExitMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(GrateExitMain, 0x110);

class TimberGateMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    TimberGateMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(TimberGateMain, 0x110);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1BasicDoorMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_BasicDoorMain_dtor(BasicDoorMain* p)
{
    p->BasicDoorMain::~BasicDoorMain();
}

// FUNCTION: USA 0x0001A77D
// PROTOTYPE: PREVIEW 0x00409A0C identical
// NAME: proto-pdb:default.pdb
SwampScript* GrateExitMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 535) GrateExitMain(node);
}

// FUNCTION: USA 0x0001A7A7
// PROTOTYPE: PREVIEW 0x00409A36 identical
// NAME: proto-pdb:default.pdb
SwampScript* TimberGateMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 521) TimberGateMain(node);
}
