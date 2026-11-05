// UNIT: src/Apps/Vince/core/VinceFlat.cpp OBJ: core\XBox\OfficialRelease\VinceFlat.obj OPT: /O1 /Oy
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
    unsigned int _pad00[7];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    void InvalidateAllSubScripts();

    char field_4[0xA4 - 0x4];          // 0x04
};

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetCollidable(bool b) { _collidable = b; }

    char field_A4[0xA8 - 0xA4];        // 0xA4
    bool _collidable;                  // 0xA8
    char field_A9[0xAC - 0xA9];        // 0xA9
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0xB4 - 0xAC];        // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

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
    char field_24[0x10];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }
    ResourceHandle& operator=(const ResourceHandle& h);

    T* _Resource;                      // 0x00
};

class AnimationControl; // NAME: proto-pdb:default.pdb

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void SetCurSpeed(float f) { _CurSpeed = f; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }

    float _MaxSpeed;                   // 0x200
    float _CurSpeed;                   // 0x204
    char field_208[0x20C - 0x208];     // 0x208
    Vector3 _Velocity;                 // 0x20C
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
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

protected:
    SwampScript(SwampSceneNode* node);

public:

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    SwampScript* _parent;              // 0x24
    char field_28[0x24];               // 0x28
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

class VinceFlat : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Message { // NAME: proto-pdb:default.pdb
        MESSAGE_POPUP = 0x14,
        MESSAGE_OUCH = 0x15,
        MESSAGE_SUCCESS = 0x16
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_FLAT = 0,
        STATE_POPUP = 1
    };
    VinceFlat(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void AnimationFinished();
    virtual void TimerEvent(unsigned int id);
    Vince* GetVince() { return (Vince*)_node; }
    virtual ~VinceFlat();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _flatten; // 0x4C
    ResourceHandle<AnimationResource> _popup;   // 0x50
    States _state;                     // 0x54
    bool _playQuip;                    // 0x58
    bool _playSuccess;                 // 0x59
};
DECOMP_SIZE_ASSERT(VinceFlat, 0x5C);

// FUNCTION: USA 0x00121740
// NAME: proto-pdb:default.pdb
void VinceFlat::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_POPUP:
        RegisterTimerEvent(0, (float)(int)data, false);
        break;
    case MESSAGE_OUCH:
        _playQuip = true;
        break;
    case MESSAGE_SUCCESS:
        _playSuccess = true;
        break;
    default:
        _parent->Message(source, message, data);
    }
}

// FUNCTION: USA 0x0012177A
// PROTOTYPE: PREVIEW 0x0050A43D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; wall: relink refuses: external ??1VinceFlat@@UAE@XZ does not resolve by name (??_G 0x001348F8 calls 0x00121879, unnamed fold of ~VinceFlat/~BasketStopperMain) (4.4 T4.c9)
VinceFlat::VinceFlat(SwampSceneNode* node) : SwampScript(node), _playQuip(false), _playSuccess(false)
{
    GetVince()->SetCurSpeed(0.0f);
    Vector3 vel(0.0f, 0.0f, 0.0f);
    GetVince()->SetVelocity(vel);
    GetVince()->SetCollidable(false);
    _flatten = g_swamp->GetAnimationManager().Load("vince_flatten");
    _popup = g_swamp->GetAnimationManager().Load("vince_flatten_recover");
    GetVince()->StartAnimation(_flatten, false, false);
    _state = STATE_FLAT;
    GetVince()->InvalidateAllSubScripts();
}
#endif

// FUNCTION: USA 0x00121947
// PROTOTYPE: PREVIEW 0x0050A5F1 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceFlat::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\VinceFlat.h", 13) VinceFlat(node);
}
