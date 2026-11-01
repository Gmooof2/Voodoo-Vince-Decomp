// UNIT: src/Apps/Vince/core/GoldJuneBug.cpp OBJ: core\XBox\OfficialRelease\GoldJuneBug.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
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
struct Vector3;              // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2,
        UseAlpha = 0x8
    };

    void SetOption(unsigned int option, bool value, bool recursive);
};

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void StopSpawning();

    char field_0[0xCE];                // 0x00
    bool _deleteWhenDone;              // 0xCE
    char field_CF[0xE8 - 0xCF];        // 0xCF
};

class CollidableCylinder : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetRadius(float);
    void SetHeight(float);
    void SetCollidable(bool b) { _collidable = b; }

    char field_0[0xA8];                // 0x00
    bool _collidable;                  // 0xA8
    char field_A9[0xB4 - 0xA9];        // 0xA9
};

class ModelResource { // NAME: proto-pdb:default.pdb
public:
    int GetBoneIndex(const SwampString& name, bool) const;
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    T* operator->() const { return _Resource; }

    T* _Resource;                      // 0x00
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    void DoLazyAnimationUpdating(unsigned char);
    void StopAnim();

    ResourceHandle<ModelResource> _model; // 0xB4
    char field_B8[0xD4 - 0xB8];        // 0xB8
    int _posBone;                      // 0xD4
    char field_D8[0x200 - 0xD8];       // 0xD8
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_DUST = 0x10
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0x264 - 0x200];     // 0x200
    float _wallThreshold;              // 0x264
    CritterType _CritterType;          // 0x268
    char field_26C[0x2C5 - 0x26C];     // 0x26C
    bool _pauseDuringWhammy;           // 0x2C5
    bool _nonRandomIdle;               // 0x2C6
    bool _isEyeItem;                   // 0x2C7
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

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class GoldenJuneBugMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_HIDDEN = 0,
        STATE_FLYING = 1,
        STATE_PAUSED = 2
    };

    GoldenJuneBugMain(SwampSceneNode* node);
    virtual void Message(SwampScript*, int, void*);
    virtual void FrameTick();
    virtual void AnimationFinished();
    virtual void AnimationTrigger(const SwampString&, float, float, unsigned int);
    virtual void Load(const BeepTDF&);
private:
    virtual void SetState(int);
public:

    bool _pausedForLB;                 // 0xBC
    float _alpha;                      // 0xC0
    FireFlyNode* _drips;               // 0xC4
    FireFlyNode* _sparks;              // 0xC8
    SwampString _animName;             // 0xCC
};
DECOMP_SIZE_ASSERT(GoldenJuneBugMain, 0xE8);

// FUNCTION: USA 0x00131655
// PROTOTYPE: PREVIEW 0x0051A088 identical
// NAME: proto-pdb:default.pdb
void GoldenJuneBugMain::SetState(int state)
{
    // !FAKE: local; retail loads _node into esi before the _state store (no PDB local)
    Critter* critter = GetCritter();
    _state = state;
    switch (state) {
    case STATE_HIDDEN:
        critter->StopAnim();
        critter->SetOption(SwampSceneNode::DontDraw, true, false);
        critter->SetCollidable(false);
        if (_drips) {
            _drips->StopSpawning();
            _drips->_deleteWhenDone = true;
            _drips = 0;
        }
        _sparks = 0;
        break;
    case STATE_FLYING:
        critter->SetCollidable(false);
        break;
    case STATE_PAUSED:
        critter->SetCollidable(true);
        break;
    }
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1 = the vptr store)
// FUNCTION: USA 0x001317E0
// PROTOTYPE: PREVIEW 0x0051A213 near-identical
// NAME: proto-pdb:default.pdb
GoldenJuneBugMain::GoldenJuneBugMain(SwampSceneNode* node)
    : CritterMain(node), _pausedForLB(false), _alpha(0.5f), _drips(0), _sparks(0)
{
    GetCritter()->_CritterType = CRITTER_TYPE_DUST;
    GetCritter()->SetRadius(0.25f);
    GetCritter()->SetHeight(0.25f);
    GetCritter()->_wallThreshold = 0.174532935f;
    GetCritter()->_posBone = GetCritter()->_model->GetBoneIndex("move", false);
    GetCritter()->DoLazyAnimationUpdating(0);
    GetCritter()->_pauseDuringWhammy = true;
    GetCritter()->SetOption(SwampSceneNode::UseAlpha, true, false);
    GetCritter()->_isEyeItem = true;
    SetState(STATE_HIDDEN);
}
#endif
