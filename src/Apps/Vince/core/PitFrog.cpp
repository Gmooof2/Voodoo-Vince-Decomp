// UNIT: src/Apps/Vince/core/PitFrog.cpp OBJ: core\XBox\OfficialRelease\PitFrog.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
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
struct Vector3 { // NAME: proto-pdb:default.pdb
    void RotateJ(float);

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    Vector3 GetFacing() const;
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
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
    char field_50[0x6C - 0x50];        // 0x50
    Vector3 _wanderDir;                // 0x6C
    char field_78[0xBC - 0x78];        // 0x78
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void DeathFX();                                        // vtable+0x70

    char field_BC[0x240 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

class PitFrogMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    PitFrogMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

public:
    virtual void DeathFX();
    void SetRandomPatrolDir();

    char field_240[0x39C - 0x240];     // 0x240
};
DECOMP_SIZE_ASSERT(PitFrogMain, 0x39C);

// FUNCTION: USA 0x00115C0C
// PROTOTYPE: PREVIEW 0x0050045A identical
// NAME: proto-pdb:default.pdb
void PitFrogMain::SetRandomPatrolDir()
{
    Vector3 patrol = _node->GetFacing();
    patrol.RotateJ(BeepLib::RandomFloat(-1.5707964f, 1.5707964f));
    _wanderDir = patrol;
}

// FUNCTION: USA 0x00115E06
// PROTOTYPE: PREVIEW 0x00500654 identical
// NAME: proto-pdb:default.pdb
void PitFrogMain::DeathFX()
{
    SoundManager::PlaySound("frogdeath", 0, _node, false, 0.0f, 0.0f);
    MonsterMain::DeathFX();
}

// FUNCTION: USA 0x00117D11
// PROTOTYPE: PREVIEW 0x00502555 identical
// NAME: proto-pdb:default.pdb
SwampScript* PitFrogMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\PitFrog.h", 13) PitFrogMain(node);
}
