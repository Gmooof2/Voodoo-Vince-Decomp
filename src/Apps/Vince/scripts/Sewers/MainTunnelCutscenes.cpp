// UNIT: src/Apps/Vince/scripts/Sewers/MainTunnelCutscenes.cpp OBJ: Xbox\OfficialRelease\MainTunnelCutscenes.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string { // NAME: proto-pdb:default.pdb
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

    char field_0[0x1C];                // 0x00
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
DECOMP_SIZE_ASSERT(SwampString, 0x1C);

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb
class AABox;                 // NAME: proto-pdb:default.pdb
class AnimCam;               // NAME: proto-pdb:default.pdb

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

class Locator { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetLocation() const { return _pos; }
    const Vector3& GetBasisVector(int i) const { return _basis[i]; }

    Vector3 _pos;                      // 0x00
    char field_C[0x1C - 0xC];          // 0x0C
    Vector3 _basis[3];                 // 0x1C
    char field_40[0x48 - 0x40];        // 0x40
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    Locator* Find(const SwampString& name) const;
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();         // vtable+0x00
    virtual const AABox& GetBounds() const; // vtable+0x04
    virtual Vector3 GetWorldPosition() const; // vtable+0x08
    virtual Quaternion GetWorldRotation() const; // vtable+0x0C
    virtual void SetWorldPosition(const Vector3& pos); // vtable+0x10

    bool PopScript();
    void SetScript(const SwampString& name);
    void SetOption(unsigned int option, bool on, bool recurse);
    const Quaternion& SetFacing(const Vector3& dir);
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<AnimationResource>, 0x4);

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x68 - 0x1C];        // 0x1C (not reached)
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
};

class Vince : public GatorNode { // NAME: proto-pdb:default.pdb
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

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished(unsigned int id);                // vtable+0x64
    virtual void AnimationTrigger(unsigned int id, const SwampString& name, float a, float b, unsigned int c); // vtable+0x68
    virtual void Init();                                            // vtable+0x6C
    virtual void Cleanup();                                         // vtable+0x70
    virtual void SetState(int state);                               // vtable+0x74

protected:
    AnimCam* NewCamera(const SwampString& name);

public:
    AnimCam* _cam;                     // 0x4C
    bool _cancelled;                   // 0x50
    bool _autostop;                    // 0x51
    char field_52[0x54 - 0x52];        // 0x52
    int _state;                        // 0x54
    char field_58[0x78 - 0x58];        // 0x58
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

class KosmoAppearsMainTunnelCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
private:
    virtual void SetState(int state);

public:
    Vince* _vince;                    // 0x78
    GatorNode* _kosmo;                 // 0x7C
    bool _wasWhammyReady;              // 0x80
};
DECOMP_SIZE_ASSERT(KosmoAppearsMainTunnelCutscene, 0x84);

// FUNCTION: USA 0x000A10EF
// PROTOTYPE: PREVIEW 0x0048C19E identical
// NAME: proto-pdb:default.pdb
void KosmoAppearsMainTunnelCutscene::SetState(int state)
{
    switch (state) {
    case 0:
        NewCamera("camera_MTKA_01");
        _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_walkexit"), false, false);
        break;
    case 1: {
        NewCamera("camera_MTKA_02");
        // !FAKE: c2 register allocation; stand-in local; the locator is used by two calls
        Locator* v0 = g_locatorManager->Find("locator_vincestanding");
        _vince->SetWorldPosition(v0->GetLocation());
        _vince->SetFacing(v0->GetBasisVector(2));
        _vince->PopScript();
        _vince->SetScript("VinceListeningToKosmoMT");
        _kosmo->StartAnimation(g_swamp->GetAnimationManager().Load("kosmoHead_MTIntro"), false, false);
        _kosmo->SetOption(2, false, false);
        break;
    }
    }
    _state = state;
}

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class VinceListeningToKosmoMT : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    VinceListeningToKosmoMT(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int timerId);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    int _state;                        // 0x4C
    Vince* _vince;                     // 0x50
    int _idle;                         // 0x54
};
DECOMP_SIZE_ASSERT(VinceListeningToKosmoMT, 0x58);

// FUNCTION: USA 0x000A135E
// NAME: proto-pdb:default.pdb
void VinceListeningToKosmoMT::TimerEvent(unsigned int timerId)
{
    if (_state == 0) {
        switch (_idle) {
        case 0:
            _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_idle08"), false, false);
            break;
        case 1:
            _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_idle02"), false, false);
            break;
        default:
            _idle = 0;
            return;
        }
        _idle++;
        _state = 1;
    }
}

// FUNCTION: USA 0x000A143A
// PROTOTYPE: PREVIEW 0x0048C4E9 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 50.0, bytes match (cut-extent wall: retail cut symbol runs past the
// row; va_dis no difference; fix is row-end pins + re-cut, source is final; generated, 4.4 T10)
SwampScript* VinceListeningToKosmoMT::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("MainTunnel\\MainTunnelCutscenes.cpp", 167) VinceListeningToKosmoMT(node);
}
#endif
