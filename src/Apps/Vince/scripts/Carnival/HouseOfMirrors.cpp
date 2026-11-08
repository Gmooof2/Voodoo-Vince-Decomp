// UNIT: src/Apps/Vince/scripts/Carnival/HouseOfMirrors.cpp OBJ: Xbox\OfficialRelease\HouseOfMirrors.obj OPT: /O1 /Oy
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
    unsigned int _pad00[0x1C / 4];
};
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

protected:
    SwampScript(SwampSceneNode* node);
public:
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

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_VINCEDIED = 0x2,
    MESSAGE_GAMESTART = 0x6,
    MESSAGE_WHOSYOURDADDY = 0x7
};

class Level { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xC];                 // 0x00
    SwampScript* _script;              // 0x0C
};

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();

    char field_0[0x4];                 // 0x00
    Level* _curLevel;                  // 0x04
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;
};

// src proto-pdb:default.pdb Critter base GatorNode : ... : SwampSceneNode, all at 0x0; not modelled
class Critter : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    void SetOnAnimName(const SwampString&);

    char field_BC[0x139 - 0xBC];       // 0xBC (not reached)
    bool _oneWayThrow;                 // 0x139
    char field_13A[0x13C - 0x13A];     // 0x13A (_multiTarget; not reached)
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
public:
    BasicWallSwitchMain(SwampSceneNode* node);
    virtual void Activate();
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class Switch_GeneratorMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    Switch_GeneratorMain(SwampSceneNode* node);
    virtual void Activate();
};

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Message { // NAME: proto-pdb:default.pdb
        MESSAGE_OPEN_INSTANT = 0x16
    };

    virtual void Message(SwampScript* source, int message, void* data);

    char field_BC[0x110 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class BattleDoorsMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript* source, int message, void* data);
};

class MirrorMonsterRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_FIND_MONSTER = 0x14
    };

    virtual void Message(SwampScript* source, int message, void* data);

    SwampString _monster;              // 0x4C
    Critter* _child;                   // 0x68
};
DECOMP_SIZE_ASSERT(MirrorMonsterRegionMain, 0x6C);

// FUNCTION: USA 0x0004484A
// NAME: proto-pdb:default.pdb
void Switch_GeneratorMain::Activate()
{
    // 0x18: CarnivalLevelMain::Messages, two PDB records disagree on its name
    LevelLoader::Instance()._curLevel->_script->Message(this, 0x18, 0);
    BasicWallSwitchMain::Activate();
}

// FUNCTION: USA 0x00043057
// NAME: proto-pdb:default.pdb
void BattleDoorsMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_VINCEDIED:
    case MESSAGE_GAMESTART:
        BasicDoorMain::Message(source, message, data);
        BasicDoorMain::Message(this, MESSAGE_OPEN_INSTANT, 0);
        break;
    default:
        BasicDoorMain::Message(source, message, data);
        break;
    }
}

class EvilVinceMirrorBrokenMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    EvilVinceMirrorBrokenMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);

    SwampScript* _parent;              // 0x4C
};
DECOMP_SIZE_ASSERT(EvilVinceMirrorBrokenMain, 0x50);

// FUNCTION: USA 0x00043095
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 60.0, bytes provisional (masked 1 = vptr store ??_7EvilVinceMirrorBrokenMain 0x001D0D38,
// no retail name; va_dis no difference): data-pin wall, source is final; 5.3 T12.1.c2
EvilVinceMirrorBrokenMain::EvilVinceMirrorBrokenMain(SwampSceneNode* node) : SwampScript(node), _parent(0)
{
}
#endif

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    LevelScript(SwampSceneNode* node, bool isZone);

    char field_4C[0x64 - 0x4C];        // 0x4C (_isZone ..; not reached)
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class HouseOMirrorsRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    HouseOMirrorsRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(HouseOMirrorsRegionMain, 0x64);

// FUNCTION: USA 0x0004321D
// PROTOTYPE: PREVIEW 0x00431547 identical
// NAME: proto-pdb:default.pdb
void EvilVinceMirrorBrokenMain::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_WHOSYOURDADDY)
        _parent = source;
}

// FUNCTION: USA 0x00043265
// NAME: proto-pdb:default.pdb
void MirrorMonsterRegionMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_GAMESTART:
        if (_child)
            _child->GetScript()->Message(this, 0x15, 0);
        break;
    case MESSAGE_FIND_MONSTER:
        _child = pManager->Find(_monster);
        break;
    }
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

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
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~MonsterMain();

    char field_BC[0x240 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

// FUNCTION: USA 0x00043A1F
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1 = vptr store ??_7Switch_GeneratorMain 0x001D0F68,
// no retail name): data-pin wall, source is final; 5.3 T12.1.c2
Switch_GeneratorMain::Switch_GeneratorMain(SwampSceneNode* node) : BasicWallSwitchMain(node)
{
    SetOnAnimName("SwitchCarnival_down");
    _oneWayThrow = true;
}
#endif

// FUNCTION: USA 0x00043F41
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 40.9, bytes provisional (masked 1 = vptr store ??_7HouseOMirrorsRegionMain 0x001D1158,
// no retail name; va_dis no difference): data-pin wall, source is final; 5.3 T12.1.c2
HouseOMirrorsRegionMain::HouseOMirrorsRegionMain(SwampSceneNode* node) : LevelScript(node, true)
{
}
#endif

// FUNCTION: USA 0x00044022
// PROTOTYPE: PREVIEW 0x00432133 identical
// NAME: proto-pdb:default.pdb
class FireBugMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> _animIdle;                // 0x240
    ResourceHandle<AnimationResource> _animFly;                 // 0x244
    ResourceHandle<AnimationResource> _animAttackStraight;      // 0x248
    ResourceHandle<AnimationResource> _animAttackDown;          // 0x24C
    ResourceHandle<AnimationResource> _animAttackLeft;          // 0x250
    ResourceHandle<AnimationResource> _animAttackRight;         // 0x254
    ResourceHandle<AnimationResource> _animRapidAttackStraight; // 0x258
    ResourceHandle<AnimationResource> _animRapidAttackDown;     // 0x25C
    ResourceHandle<AnimationResource> _animRapidAttackLeft;     // 0x260
    ResourceHandle<AnimationResource> _animRapidAttackRight;    // 0x264
    ResourceHandle<AnimationResource> _animWindup;              // 0x268
    int _barrelBone;                   // 0x26C
    char field_270[0x4];               // 0x270 (_loopSound; not reached)
};
DECOMP_SIZE_ASSERT(FireBugMain, 0x274);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1FireBugMain@@UAE@XZ in this unit (no vptr store,
// so no declared dtor; no matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_FireBugMain_dtor(FireBugMain* p)
{
    p->FireBugMain::~FireBugMain();
}
