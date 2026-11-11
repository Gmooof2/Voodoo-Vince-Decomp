// UNIT: src/Apps/Vince/scripts/Sewers/RocketLabCutscene.cpp OBJ: Xbox\OfficialRelease\RocketLabCutscene.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std {
template <class E> struct char_traits; // NAME: proto-pdb:default.pdb
template <class T> class allocator;    // NAME: proto-pdb:default.pdb
template <class E, class T = char_traits<E>, class A = allocator<E> >
class basic_string { // NAME: proto-pdb:default.pdb
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v; // 0x00
    float w;   // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);
class AABox;      // NAME: proto-pdb:default.pdb

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
    Locator* Find(const std::basic_string<char>& name) const;
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

class GameCam; // NAME: proto-pdb:default.pdb

class VinceCam { // NAME: proto-pdb:default.pdb
public:
    void CopyCamera(GameCam* cam);
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const std::basic_string<char>& name) const;

    char field_0[0xC];                 // 0x00
    std::basic_string<char> _curCamName; // 0x0C
};
extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const std::basic_string<char>& name, float blend, bool hold, float delay);
    static void Return(float blend);
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();         // vtable+0x00
    virtual const AABox& GetBounds() const; // vtable+0x04
    virtual Vector3 GetWorldPosition() const; // vtable+0x08
    virtual Quaternion GetWorldRotation() const; // vtable+0x0C
    virtual void SetWorldPosition(const Vector3& pos); // vtable+0x10
    virtual void Invalidate();         // vtable+0x14

    bool PopScript();
    void SetOption(unsigned int option, bool on, bool recurse);
    const Quaternion& SetFacing(const Vector3& dir);
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class Vince : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void SetWhammyReady(bool ready);
};

class BeepTDF;                // NAME: proto-pdb:default.pdb
class CollisionResultObject;  // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum CollisionType { // NAME: proto-pdb:default.pdb
    };
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04
    virtual void Activate();                                       // vtable+0x08
    virtual void Deactivate();                                     // vtable+0x0C
    virtual void FrameTick();                                      // vtable+0x10
    virtual void TimerEvent(unsigned int id);                      // vtable+0x14
    virtual void AnimationFinished();                              // vtable+0x18
    virtual void SecondaryAnimationFinished();                     // vtable+0x1C
    virtual void AnimationInterrupted();                           // vtable+0x20
    virtual void SecondaryAnimationInterrupted();                  // vtable+0x24
    virtual void AnimationTrigger(const std::basic_string<char>& name, float a, float b, unsigned int c); // vtable+0x28
    virtual bool LosingControl(const std::basic_string<char>& name); // vtable+0x2C
    virtual void GainingControl(SwampScript* script);              // vtable+0x30
    virtual void Message(SwampScript* from, int msg, void* data);  // vtable+0x34
    virtual void EnteringRegion(SwampSceneNode* region);           // vtable+0x38
    virtual void LeavingRegion(SwampSceneNode* region);            // vtable+0x3C
    virtual void SoundFinished(const std::basic_string<char>& name); // vtable+0x40
    virtual void SoundStreamerFinished(const std::basic_string<char>& name); // vtable+0x44
    virtual void MovieFinished(const std::basic_string<char>& name); // vtable+0x48
    virtual void CollidedWith(CollisionType type, SwampSceneNode* node, const Ptr<CollisionResultObject>& result); // vtable+0x4C
    virtual void HitBy(CollisionType type, SwampSceneNode* node, const Ptr<CollisionResultObject>& result); // vtable+0x50
    virtual void Load(const BeepTDF& tdf);                         // vtable+0x54
    virtual std::basic_string<char> Save();                        // vtable+0x58
    virtual void SetNode(SwampSceneNode* node);                    // vtable+0x5C
    virtual void InvalidateScript();                               // vtable+0x60

    char field_4[0x4C - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class AreaLoadLocatorMain { // NAME: proto-pdb:default.pdb
public:
    static void FinishedLevelIntro(SwampScript* script);
};

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished(unsigned int id);                // vtable+0x64
    virtual void AnimationTrigger(unsigned int id, const std::basic_string<char>& name, float a, float b, unsigned int c); // vtable+0x68
    virtual void Init();                                            // vtable+0x6C

protected:
    virtual void Cleanup();                                         // vtable+0x70

public:
    virtual void SetState(int state);                               // vtable+0x74

protected:
    void StateTransition(unsigned int id, int state);

public:
    AnimCam* _cam;                     // 0x4C
    bool _cancelled;                   // 0x50
    bool _autostop;                    // 0x51
    char field_52[0x78 - 0x52];        // 0x52
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class EthelRocketLabCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_LAST = 0x8
    };
    EthelRocketLabCutscene(SwampSceneNode* node);
    virtual void AnimationFinished(unsigned int id);

protected:
    virtual void Cleanup();

public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

public:
    Vince* _vince;                    // 0x78
    GatorNode* _ethel;                 // 0x7C
    bool _wasWhammyReady;              // 0x80
};
DECOMP_SIZE_ASSERT(EthelRocketLabCutscene, 0x84);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0009F088
// PROTOTYPE: PREVIEW 0x0048A12C identical
// NAME: proto-pdb:default.pdb
void EthelRocketLabCutscene::AnimationFinished(unsigned int id)
{
    StateTransition(id, STATE_LAST);
}

// FUNCTION: USA 0x0009F193
// PROTOTYPE: PREVIEW 0x0048A237 identical
// NAME: proto-pdb:default.pdb
void EthelRocketLabCutscene::Cleanup()
{
    CutsceneDriver::Cleanup();
    if (!_cancelled) {
        AnimCam::SwitchTo("camera_06", 0.0f, false, 0.0f);
        // !FAKE: c1xx evaluation order; dummy local; retail finds vincecam first (inline form 95.77 -> 100.0)
        VinceCam* v1 = (VinceCam*)g_CameraManager->Find("vincecam");
        v1->CopyCamera(g_CameraManager->Find(g_CameraManager->_curCamName));
        AnimCam::Return(1.0f);
    } else {
        AnimCam::Return(0.0f);
    }
    // !FAKE: c2 register allocation; dummy local; the locator is used by two calls
    Locator* v2 = g_locatorManager->Find("locator_RLVinceEnd");
    _vince->PopScript();
    _vince->SetOption(2, false, false);
    _vince->SetWorldPosition(v2->GetLocation());
    _vince->SetFacing(v2->GetBasisVector(2));
    _vince->SetWhammyReady(_wasWhammyReady);
    _ethel->Invalidate();
    AreaLoadLocatorMain::FinishedLevelIntro(this);
    InvalidateScript();
}

// FUNCTION: USA 0x0009F677
// NAME: proto-pdb:default.pdb
SwampScript* EthelRocketLabCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("RocketLab\\RocketLabCutscene.cpp", 19) EthelRocketLabCutscene(node);
}

// FUNCTION: USA 0x0009F69E
// NAME: proto-pdb:default.pdb
EthelRocketLabCutscene::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("EthelRocketLabCutscene", creationFunc);
}
