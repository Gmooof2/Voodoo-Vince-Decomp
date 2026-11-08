// UNIT: src/Apps/Vince/scripts/Carnival/InsideKosmobotCutscenes.cpp OBJ: Xbox\OfficialRelease\InsideKosmobotCutscenes.obj OPT: /O1 /Oy
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
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { DontDraw = 0x2 }; // NAME: proto-pdb:default.pdb
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    void SetOption(unsigned int option, bool on, bool recurse);
protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
public:

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

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

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
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

protected:
    SwampScript(SwampSceneNode* node);
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

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    UINode();
    virtual void Update();

    char field_A4[0xD4 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

class FadeToBlack : public UINode { // NAME: proto-pdb:default.pdb
public:
    FadeToBlack();
    void Start(float sec);
    virtual void Update();
    virtual void Render(const Matrix&) const;

    float _alpha;                      // 0xD4
    float _dt;                         // 0xD8
};
DECOMP_SIZE_ASSERT(FadeToBlack, 0xDC);

// FUNCTION: USA 0x0005481D
// NAME: proto-pdb:default.pdb
FadeToBlack::FadeToBlack() : _alpha(0.0f), _dt(0.0f)
{
}

// FUNCTION: USA 0x0005483F
// NAME: proto-pdb:default.pdb
void FadeToBlack::Start(float sec)
{
    if (sec > 0.0f) {
        _alpha = 0.0f;
        _dt = 1.0f / sec;
    } else {
        _alpha = 1.0f;
        _dt = 0.0f;
    }
}

// src proto-pdb:default.pdb HUDUI base UINode at 0x0, not modelled
class HUDUI { // NAME: proto-pdb:default.pdb
public:
    void EndExpandedLetterbox(float);
    void ClearSubtitle();
private:
    static HUDUI* _self;
    friend class KJDFinalCamera;
};

class KJDFinalCamera : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    KJDFinalCamera(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
};
DECOMP_SIZE_ASSERT(KJDFinalCamera, 0x4C);

// FUNCTION: USA 0x00054873
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; wall: relink refuses `our ranges overlap at 0x00025012` (the vtable
// this ctor emits brings ??_GKJDFinalCamera, which retail folds into the shared SwampScript-family ??_G 0x00025012)
KJDFinalCamera::KJDFinalCamera(SwampSceneNode* node)
    : SwampScript(node)
{
}
#endif

// FUNCTION: USA 0x0005488B
// PROTOTYPE: PREVIEW 0x0044127A identical
// NAME: proto-pdb:default.pdb
void KJDFinalCamera::AnimationFinished()
{
    HUDUI::_self->EndExpandedLetterbox(1.0f);
    HUDUI::_self->ClearSubtitle();
    InvalidateScript();
}

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
    char field_4[0x120 - 0x4];         // 0x04
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x18 - 0x8];          // 0x08
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x68 - 0x1C];        // 0x1C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct Vector2 { // NAME: proto-pdb:default.pdb
    void Set(float u_, float v_) { u = u_; v = v_; }

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder at 0x0, modelled only as its SwampSceneNode base
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
    void StopUVScroll() { _uvScrolling = false; }
    void SetUVScrollSpeed(float u, float v) { _uvScrollingSpeed.Set(u, v); }

    char field_A4[0xC3 - 0xA4];        // 0xA4
    bool _uvScrolling;                 // 0xC3
    char field_C4[0x1F8 - 0xC4];       // 0xC4
    Vector2 _uvScrollingSpeed;         // 0x1F8
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
    char field_200[0x2C8 - 0x200];     // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void FrameTick();
    virtual void AnimationFinished(unsigned int id);               // vtable+0x64
protected:
    void StateTransition(unsigned int id, int lastState);
public:

    char field_4C[0x54 - 0x4C];        // 0x4C
    int _state;                        // 0x54
    char field_58[0x78 - 0x58];        // 0x58
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

class KosmoJustDessertsCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_LIGHTS = 0x0,
        TIMER_FADE = 0x1
    };
    enum Ids { // NAME: proto-pdb:default.pdb
        ID_KOSMO = 0x7,
        ID_JEB = 0x8,
        ID_FINGERS = 0x9
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_K_ULP = 0x15,
        STATE_LAST = 0x1C
    };
    KosmoJustDessertsCutscene(SwampSceneNode* node);
    virtual void AnimationFinished(unsigned int id);
    virtual void FrameTick();
    virtual void TimerEvent(unsigned int id);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_78[0x80 - 0x78];        // 0x78
    Critter* _kosmobotHead;            // 0x80
    char field_84[0x90 - 0x84];        // 0x84
    GatorNode* _kosmo;                 // 0x90
    GatorNode* _jeb;                   // 0x94
    GatorNode* _fingers;               // 0x98
    char field_9C[0xA8 - 0x9C];        // 0x9C
    GatorNode* _kosmoBalloon;          // 0xA8
    GatorNode* _fingersBalloon;        // 0xAC
    GatorNode* _jebBalloon;            // 0xB0
    FadeToBlack* _fadeui;              // 0xB4
    bool _wasWhammyReady;              // 0xB8
    float _lightsSpeed;                // 0xBC
    float _lightsDt;                   // 0xC0
};
DECOMP_SIZE_ASSERT(KosmoJustDessertsCutscene, 0xC4);

// FUNCTION: USA 0x000548B2
// NAME: proto-pdb:default.pdb
void KosmoJustDessertsCutscene::TimerEvent(unsigned int id)
{
    switch (id) {
    case TIMER_LIGHTS:
        _lightsSpeed = 1.0f;
        _lightsDt = 1.0f / 3.0f;
        break;
    case TIMER_FADE:
        _fadeui->Start(3.0f);
        break;
    }
}

// FUNCTION: USA 0x000548EA
// PROTOTYPE: PREVIEW 0x004412D9 identical
// NAME: proto-pdb:default.pdb
void KosmoJustDessertsCutscene::FrameTick()
{
    CutsceneDriver::FrameTick();
    if (_state == 2 && _lightsDt > 0.0f) {
        _lightsSpeed -= g_swamp->GetInput().GetElapsedTime() * _lightsDt;
        if (_lightsSpeed <= 0.0f) {
            _lightsDt = 0.0f;
            _kosmobotHead->StopUVScroll();
        } else {
            _kosmobotHead->SetUVScrollSpeed(_lightsSpeed, _lightsSpeed);
        }
    }
}

// FUNCTION: USA 0x00054A01
// PROTOTYPE: PREVIEW 0x004413F0 identical
// NAME: proto-pdb:default.pdb
void KosmoJustDessertsCutscene::AnimationFinished(unsigned int id)
{
    StateTransition(id, STATE_LAST);
    if (_state == STATE_K_ULP) {
        switch (id) {
        case ID_KOSMO:
            _kosmo->SetOption(SwampSceneNode::DontDraw, true, false);
            _kosmoBalloon->StartAnimation(g_swamp->GetAnimationManager()->Load("kosmoBalloon_KJD_01_inflate"), false, false);
            _kosmoBalloon->SetOption(SwampSceneNode::DontDraw, false, false);
            break;
        case ID_JEB:
            _jeb->SetOption(SwampSceneNode::DontDraw, true, false);
            _jebBalloon->StartAnimation(g_swamp->GetAnimationManager()->Load("jebBalloon_KJD_01_inflate"), false, false);
            _jebBalloon->SetOption(SwampSceneNode::DontDraw, false, false);
            break;
        case ID_FINGERS:
            _fingers->SetOption(SwampSceneNode::DontDraw, true, false);
            _fingersBalloon->StartAnimation(g_swamp->GetAnimationManager()->Load("fingersBalloon_KJD_01_inflate"), false, false);
            _fingersBalloon->SetOption(SwampSceneNode::DontDraw, false, false);
            break;
        }
    }
}

// FUNCTION: USA 0x00056C55
// PROTOTYPE: PREVIEW 0x0044363D identical
// NAME: proto-pdb:default.pdb
SwampScript* KosmoJustDessertsCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("InsideKosmobot\\InsideKosmobotCutscenes.cpp", 61) KosmoJustDessertsCutscene(node);
}
