// UNIT: src/Apps/Vince/scripts/Carnival/LandORides.cpp OBJ: Xbox\OfficialRelease\LandORides.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

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
    char _pad00[0x14];
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A>
basic_string<E, T, A> operator+(const E* _Left, const basic_string<E, T, A>& _Right);
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const E* _Right);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb
class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { DontDraw = 0x2 }; // NAME: proto-pdb:default.pdb
    const SwampString& GetName() const { return _Name; }
    SwampScript* GetScript() const;
    void SetOption(unsigned int option, bool on, bool recurse);
    void SetScript(const SwampString& name);
    Vector3 GetFacing() const;
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10

    char field_4[0x4];                 // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// src proto-pdb:default.pdb Vince base Critter : GatorNode : CollidableCylinder : CollidableNode : SwampSceneNode, all at 0x0; only Critter and SwampSceneNode modelled
class Critter : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetTargetLocation(const SwampString& location, bool);
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }
    float GetGameTime() const { return _gameTimePublic; }

    float _elapsed;                    // 0x00
    char field_4[0x4];                 // 0x04
    float _gameTimePublic;             // 0x08
    char field_C[0x120 - 0xC];         // 0x0C
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x68 - 0x8];          // 0x08
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    void RequestArea(const SwampString&, const SwampString&, bool, float, bool);
};

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
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

    void RegisterTimerEvent(unsigned int, float, bool);

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

class RidesCannonMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_COUNTDOWN = 1,
        STATE_LAUNCH = 2,
        STATE_LAND = 3
    };
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_LAUNCH = 0,
        TIMER_CHANGECAMERAS = 1,
        TIMER_RESUMEGAME = 2
    };
    virtual void Activate();
    virtual void FrameTick();

    float _moveScale;                  // 0xBC
};
DECOMP_SIZE_ASSERT(RidesCannonMain, 0xC0);

class ChopperMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    ChopperMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ChopperMain, 0x4C);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    bool CanCreate(const SwampString& name) const;
    SwampScript* Create(const SwampString& name, SwampSceneNode* node) const;
    static SwampScriptManager& Instance();
};

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb

    char field_4C[0x64 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class LandORidesRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    LandORidesRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x68 - 0x64];        // 0x64
};
DECOMP_SIZE_ASSERT(LandORidesRegionMain, 0x68);

// FUNCTION: USA 0x0004FB2E
// PROTOTYPE: PREVIEW 0x0043CA67 identical
// NAME: proto-pdb:default.pdb
SwampScript* ChopperMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\landorides\\LandORides.h", 32) ChopperMain(node);
}

// FUNCTION: USA 0x0004FB52
// NAME: proto-pdb:default.pdb
void RidesCannonMain::FrameTick()
{
    // !FAKE: c2 register allocation; pointer local; retail holds Vince in esi across the calls (as Activate); the
    // proto PDB lists no such local
    Vince* p = pManager->GetVince();
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    switch (_state) {
    case STATE_LAND: {
        _moveScale -= elapsed;
        if (_moveScale < 0.0f) {
            _moveScale = 0.0f;
        }
        // !FAKE: inliner; pointer locals, hand forms of the inlined Vector3 products and sum; no before/after recorded (C0191, C0223 D78)
        const Vector3* v1 = &p->GetFacing();
        Vector3 v2(v1->x * elapsed, v1->y * elapsed, v1->z * elapsed);
        // !FAKE: c2 x87 scheduling; float stand-in for the inlined product's by-value scale; retail loads _moveScale
        // once; match_one 90.25 -> 100.0)
        float v6 = _moveScale;
        Vector3 v3(v2.x * v6, v2.y * v6, v2.z * v6);
        const Vector3* v4 = &p->GetWorldPosition();
        p->SetWorldPosition(Vector3(v3.x + v4->x, v3.y + v4->y, v3.z + v4->z));
        break;
    }
    case STATE_LAUNCH: {
        // !FAKE: inliner; pointer locals, hand forms of the inlined Vector3 products and sum; no before/after recorded (C0191, C0223 D78)
        const Vector3* v1 = &p->GetFacing();
        Vector3 v2(v1->x, v1->y + 0.25f, v1->z);
        Vector3 v3(v2.x * elapsed, v2.y * elapsed, v2.z * elapsed);
        Vector3 v5(v3.x * 5.0f, v3.y * 5.0f, v3.z * 5.0f);
        const Vector3* v4 = &p->GetWorldPosition();
        p->SetWorldPosition(Vector3(v5.x + v4->x, v5.y + v4->y, v5.z + v4->z));
        break;
    }
    }
}

// FUNCTION: USA 0x0005075B
// PROTOTYPE: PREVIEW 0x0043D556 identical
// NAME: proto-pdb:default.pdb
void RidesCannonMain::Activate()
{
    if (_state == STATE_IDLE) {
        // !FAKE: c2 register allocation; pointer local; retail holds Vince in ebx across both calls, a second GetVince() reloads pManager (match_one 91.14 -> 100.0); the proto PDB lists no such local
        Vince* p = pManager->GetVince();
        p->SetOption(SwampSceneNode::DontDraw, true, false);
        p->SetScript("");
        SwampString name = "camera_" + _node->GetName() + "Launch";
        AnimCam::SwitchTo(name, 0.0f, false, 0.0f);
        RegisterTimerEvent(TIMER_LAUNCH, 1.0f, false);
        SetState(STATE_COUNTDOWN);
    }
}

// FUNCTION: USA 0x00050926
// PROTOTYPE: PREVIEW 0x0043D721 identical
// NAME: proto-pdb:default.pdb
SwampScript* LandORidesRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\landorides\\LandORides.h", 14) LandORidesRegionMain(node);
}

// FUNCTION: USA 0x000509C2
// PROTOTYPE: PREVIEW 0x0043D7BD identical
// NAME: proto-pdb:default.pdb
void ChopperMain::Activate()
{
    if (SwampScriptManager::Instance().CanCreate("VinceRidesChopperCutscene"))
        SwampScriptManager::Instance().Create("VinceRidesChopperCutscene", 0);
    else
        Message(this, 20, 0);
}

// FUNCTION: USA 0x00051153
// PROTOTYPE: PREVIEW 0x0043DF49 identical
// NAME: proto-pdb:default.pdb
void ChopperMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 20: {
        Critter* door = pManager->Find("doorservice");
        door->GetScript()->Message(this, 22, 0);
        Critter* actpad = pManager->Find("activator_chopper");
        if (actpad)
            actpad->GetScript()->Message(this, 21, 0);
        pManager->GetVince()->SetTargetLocation("loadFromLandORides", false);
        LevelLoader::Instance().RequestArea("d:\\levels\\carnival", "bumpercarbumpoff", true, g_swamp->GetInput().GetGameTime(), true);
        InvalidateScript();
        break;
    }
    }
}
