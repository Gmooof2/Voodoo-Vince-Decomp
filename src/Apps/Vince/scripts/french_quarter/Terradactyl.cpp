// UNIT: src/Apps/Vince/scripts/french_quarter/Terradactyl.cpp OBJ: Xbox\OfficialRelease\Terradactyl.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const E* _Ptr);

private:
    unsigned int _pad00[0x1C / 4];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class AABox;                 // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    void Normalize();

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

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

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    virtual void Load(const BeepTDF&);

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class AnimationControl; // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);

    T* _Resource;                      // 0x00
};

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

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2
    };

    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08

    void SetOption(unsigned int option, bool value, bool recursive);
    SwampScript* GetScript() const;

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode : SwampSceneNode, all at 0x0; not modelled
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool restart, bool loop);

    char field_A4[0x15C];              // 0xA4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void SetVelocity(const Vector3& v) { _Velocity = v; }

    char field_200[0xC];               // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0xB0];              // 0x218
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class DamageSourceInfo { // NAME: proto-pdb:default.pdb
public:
    DamageSourceInfo();
    ~DamageSourceInfo();

    SwampString sourceType;            // 0x00
    SwampString attackType;            // 0x1C
    float _multiplier;                 // 0x38
    bool _canBlock;                    // 0x3C
};
DECOMP_SIZE_ASSERT(DamageSourceInfo, 0x40);

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_DAMAGE = 0x0
};

class TerradactylMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_HIDE = 0x14
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_WAITFORACTIVATE = 0,
        STATE_GLIDE = 1
    };

    TerradactylMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void SetState(int state);
    virtual void Load(const BeepTDF& tdf);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void DamageVince(SwampSceneNode* pvince);

    ResourceHandle<AnimationResource> _animFlyPath; // 0xBC
    SwampString _terryAnimName;        // 0xC0
};
DECOMP_SIZE_ASSERT(TerradactylMain, 0xDC);

// FUNCTION: USA 0x0008BC6A
// PROTOTYPE: PREVIEW 0x0047778D identical
// NAME: proto-pdb:default.pdb
SwampScript* TerradactylMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\museum\\Terradactyl.h", 7) TerradactylMain(node);
}

// FUNCTION: USA 0x0008BA65
// NAME: proto-pdb:default.pdb
void TerradactylMain::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_HIDE)
        _node->SetOption(SwampSceneNode::DontDraw, true, false);
}

// FUNCTION: USA 0x0008BA3D
// NAME: proto-pdb:default.pdb
void TerradactylMain::SetState(int state)
{
    switch (state) {
    case STATE_GLIDE:
        ((GatorNode*)_node)->StartAnimation(_animFlyPath, true, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x0008BA7D
// NAME: proto-pdb:default.pdb
void TerradactylMain::Load(const BeepTDF& tdf)
{
    CritterMain::Load(tdf);
    _animFlyPath = g_swamp->GetAnimationManager().Load(_terryAnimName);
}

// FUNCTION: USA 0x0008BBA1
// PROTOTYPE: PREVIEW 0x004776C4 identical
// NAME: proto-pdb:default.pdb
void TerradactylMain::DamageVince(SwampSceneNode* pvince)
{
    DamageSourceInfo dsi;
    dsi.sourceType = "turbodactyl";
    dsi.attackType = "bump";
    pvince->GetScript()->Message(this, MESSAGE_DAMAGE, &dsi);
    // !FAKE: inliner; pointer locals + hand form of retail's inlined Vector3 difference; retail inlines it, no before/after recorded (C0191)
    const Vector3* v0 = &_node->GetWorldPosition();
    const Vector3* v1 = &pvince->GetWorldPosition();
    Vector3 diff;
    diff.x = v1->x - v0->x;
    diff.y = v1->y - v0->y;
    diff.z = v1->z - v0->z;
    diff.Normalize();
    diff.x *= 2.0f;
    diff.z *= 2.0f;
    diff.y = 2.0f;
    static_cast<Critter*>(pvince)->SetVelocity(diff);
}
