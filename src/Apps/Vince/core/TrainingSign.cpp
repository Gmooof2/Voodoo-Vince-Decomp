// UNIT: src/Apps/Vince/core/TrainingSign.cpp OBJ: core\XBox\OfficialRelease\TrainingSign.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

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

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetOption(unsigned int, bool, bool);

    char field_0[0xA4];                // 0x00
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
};

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_NONE = 0x0,
    CRITTER_TYPE_ITEM = 0x1,
    CRITTER_TYPE_ENEMY = 0x2,
    CRITTER_TYPE_STATIC = 0x4,
    CRITTER_TYPE_CARRIEDOBJECT = 0x8,
    CRITTER_TYPE_DUST = 0x10,
    CRITTER_TYPE_FRIEND = 0x20,
    CRITTER_TYPE_HEAVY_OBJECT = 0x40
};

class Critter { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x268];               // 0x00
    CritterType _CritterType;          // 0x268
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

    void RegisterFrameTick();

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);


class TrainingSignMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    TrainingSignMain(SwampSceneNode* node);
    virtual ~TrainingSignMain();
    virtual void FrameTick();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(TrainingSignMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1 = the vptr store); /O2 /Oy control 90.53
// FUNCTION: USA 0x00136907
// PROTOTYPE: PREVIEW 0x00520ACD near-identical
// NAME: proto-pdb:default.pdb
TrainingSignMain::TrainingSignMain(SwampSceneNode* node) : CritterMain(node)
{
    ((Critter*)_node)->_CritterType = CRITTER_TYPE_STATIC;
    _node->SetOption(8, true, true);
    ((CollidableNode*)_node)->_collidable = false;
    RegisterFrameTick();
}
#endif

// FUNCTION: USA 0x0013694B
// PROTOTYPE: PREVIEW 0x00520B11 identical
// NAME: proto-pdb:default.pdb
SwampScript* TrainingSignMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("scripts\\common\\TrainingSign.cpp", 6) TrainingSignMain(node);
}
