// UNIT: src/Apps/Vince/scripts/Carnival/InsideKosmobotCutscenes.cpp OBJ: Xbox\OfficialRelease\InsideKosmobotCutscenes.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
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

// src proto-pdb:default.pdb FadeToBlack base UINode at 0x0, not modelled
class FadeToBlack { // NAME: proto-pdb:default.pdb
public:
    void Start(float sec);

    char field_0[0xD4];                // 0x00
    float _alpha;                      // 0xD4
    float _dt;                         // 0xD8
};
DECOMP_SIZE_ASSERT(FadeToBlack, 0xDC);

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

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb

    char field_4C[0x78 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

class KosmoJustDessertsCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_LIGHTS = 0x0,
        TIMER_FADE = 0x1
    };
    KosmoJustDessertsCutscene(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int id);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_78[0xB4 - 0x78];        // 0x78
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

// FUNCTION: USA 0x00056C55
// PROTOTYPE: PREVIEW 0x0044363D identical
// NAME: proto-pdb:default.pdb
SwampScript* KosmoJustDessertsCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("InsideKosmobot\\InsideKosmobotCutscenes.cpp", 61) KosmoJustDessertsCutscene(node);
}
