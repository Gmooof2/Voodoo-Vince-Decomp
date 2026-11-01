// UNIT: src/Apps/Vince/core/VinceSwing.cpp OBJ: core\XBox\OfficialRelease\VinceSwing.obj OPT: /O1 /Oy
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
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
};

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
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

class VinceSwing : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    static const int MAX_ROPE_NODES = 20;

    ResourceHandle<AnimationResource> _swing[2]; // 0xBC
    ResourceHandle<AnimationResource> _flip;     // 0xC4
    char field_C8[0xDC - 0xC8];        // 0xC8
    FireFlyNode* _ropeNode[MAX_ROPE_NODES]; // 0xDC
    char field_12C[0x5C];              // 0x12C

private:
    void KillRope();
    void KillSwing();
};
DECOMP_SIZE_ASSERT(VinceSwing, 0x188);

// Retail layout: Vince members +0x30 here (_readyToDoubleJump 0x515 -> 0x545).
class Vince { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x545];               // 0x00
    bool _readyToDoubleJump;           // 0x545
    bool _readyToSpin;                 // 0x546
};

class GameCam { // NAME: proto-pdb:default.pdb
};

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void ClearPositionFixed();
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00128C62
// PROTOTYPE: PREVIEW 0x0051175A identical
// NAME: proto-pdb:default.pdb
void VinceSwing::KillRope()
{
    for (int i = 0; i < MAX_ROPE_NODES; i++) {
        if (_ropeNode[i]) {
            _ropeNode[i]->Invalidate();
            _ropeNode[i] = 0;
        }
    }
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); not finished: g_CameraManager pin is site-corr (provisional)
// FUNCTION: USA 0x0012900F
// PROTOTYPE: PREVIEW 0x00511A7B near-identical
// NAME: proto-pdb:default.pdb
void VinceSwing::KillSwing()
{
    ((Vince*)_node)->_readyToDoubleJump = true;
    ((Vince*)_node)->_readyToSpin = true;
    KillRope();
    VinceCam* cam = (VinceCam*)g_CameraManager->Find("vincecam");
    cam->ClearPositionFixed();
}
#endif
// FUNCTION: USA 0x00129BDA
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1VinceSwing@@UAE@XZ because this unit defines
// the ctor 0x001298F2 (not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceSwing_dtor(VinceSwing* p)
{
    p->VinceSwing::~VinceSwing();
}

