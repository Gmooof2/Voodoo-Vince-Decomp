// UNIT: src/Apps/Vince/scripts/Carnival/BumpercarBumpoff.cpp OBJ: Xbox\OfficialRelease\BumpercarBumpoff.obj OPT: /O1 /Oy
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
class SwampScript;           // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    SwampScript* GetScript() const;

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

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

    void RegisterTimerEvent(unsigned int id, float deltaTime, bool looped);
    void UnregisterTimerEvent(unsigned int id);

protected:
    SwampScript(SwampSceneNode* node);

public:
    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    virtual void Message(SwampScript*, int, void*);

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class AirplaneMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript*, int, void*);
protected:
    virtual void DoDeath();                                        // vtable+0x70

    char field_BC[0xF0 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(AirplaneMain, 0xF0);

class TentPlaneMain : public AirplaneMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_SHOOT = 0x1,
        TIMER_INSTRUCTIONS = 0x2
    };
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Activate();

    int _gunIndex;                     // 0xF0
    bool _firstRide;                   // 0xF4
};
DECOMP_SIZE_ASSERT(TentPlaneMain, 0xF8);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C

    char field_A4[0xAC - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0xB4 - 0xAC];        // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    virtual void ReturnToHome();                                   // vtable+0x44
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class MusicManager { // NAME: proto-pdb:default.pdb
public:
    void PlayTrack(const SwampString&);
};

extern MusicManager* g_MusicManager; // NAME: proto-pdb:default.pdb

class CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    static void SetStream(const SwampString& wav, bool looped);
};

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode*);

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_4E[0x64 - 0x4E];        // 0x4E
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class BumpercarBumpoffRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BumpercarBumpoffRegionMain, 0x64);

class BcboStartBattleRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    BcboStartBattleRegionMain(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);

protected:
    static bool _started;
};
DECOMP_SIZE_ASSERT(BcboStartBattleRegionMain, 0x4C);

// FUNCTION: USA 0x0004DECE
// PROTOTYPE: PREVIEW 0x0043AE60 identical
// NAME: proto-pdb:default.pdb
class AreaLoadRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);

    SwampString _nextLevel;            // 0x4C
    SwampString _nextArea;             // 0x68
    SwampString _targetLocation;       // 0x84
    SwampString _animVince;            // 0xA0
    bool _activateTrolleys;            // 0xBC
    bool _activateChapter;             // 0xBD
};
DECOMP_SIZE_ASSERT(AreaLoadRegionMain, 0xC0);

// FUNCTION: USA 0x0004DD3F
// PROTOTYPE: PREVIEW 0x0043ACD6 identical
// NAME: proto-pdb:default.pdb
void TentPlaneMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 2:
        UnregisterTimerEvent(TIMER_SHOOT);
        break;
    case 0x12:
        UnregisterTimerEvent(TIMER_SHOOT);
        RegisterTimerEvent(TIMER_SHOOT, 0.06666667f, true);
        if (_firstRide) {
            _firstRide = false;
            RegisterTimerEvent(TIMER_INSTRUCTIONS, 2.0f, false);
        }
        break;
    }
    AirplaneMain::Message(source, message, data);
}

// FUNCTION: USA 0x0004DDB4
// PROTOTYPE: PREVIEW 0x0043AD46 identical
// NAME: proto-pdb:default.pdb
BcboStartBattleRegionMain::BcboStartBattleRegionMain(SwampSceneNode* node)
    : SwampScript(node)
{
    _started = false;
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1AreaLoadRegionMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead
void pin_emit_AreaLoadRegionMain_dtor(AreaLoadRegionMain* p)
{
    p->AreaLoadRegionMain::~AreaLoadRegionMain();
}

// FUNCTION: USA 0x0004DF92
// NAME: proto-pdb:default.pdb
void BumpercarBumpoffRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (!_inRegion) {
        LevelScript::EnteringRegion(node);
        // !FAKE: c1xx front end; stand-in local; the PDB records no local, ends the string temporary before the call
        Critter* tmp = pManager->Find("bumpercar");
        tmp->ReturnToHome();
        tmp = pManager->Find("doorexitbumpoff");
        tmp->GetScript()->Message(this, 0x15, 0);
        CutsceneDriver::SetStream("kosmobotPrelude", false);
    }
}

// FUNCTION: USA 0x0004E3A5
// PROTOTYPE: PREVIEW 0x0043B07C identical
// NAME: proto-pdb:default.pdb
void TentPlaneMain::Activate()
{
    // !FAKE: c1xx front end; stand-in local; the PDB records no local, ends the string temporary before GetScript
    Critter* tmp = pManager->Find("kosmobothead");
    tmp->GetScript()->Message(this, 0x12, 0);
    g_MusicManager->PlayTrack("fightingAceMusic");
}

// FUNCTION: USA 0x0004E6F5
// PROTOTYPE: PREVIEW 0x0043B66A identical
// NAME: proto-pdb:default.pdb
void BcboStartBattleRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (node == pManager->GetVince() && !_started) {
        _started = true;
        // !FAKE: c1xx front end; stand-in local; the PDB records no local, ends the string temporary before GetScript
        Critter* tmp = pManager->Find("doorexitbumpoff");
        tmp->GetScript()->Message(this, 0x14, 0);
    }
}

// FUNCTION: USA 0x0004E760
// PROTOTYPE: PREVIEW 0x0043B6D5 identical
// NAME: proto-pdb:default.pdb
void BcboStartBattleRegionMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 2:
        _started = false;
        // !FAKE: c1xx front end; stand-in local; the PDB records no local, ends the string temporary before GetScript
        Critter* tmp = pManager->Find("doorexitbumpoff");
        tmp->GetScript()->Message(this, 0x15, 0);
        break;
    }
}
