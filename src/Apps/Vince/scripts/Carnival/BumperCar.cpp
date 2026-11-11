// UNIT: src/Apps/Vince/scripts/Carnival/BumperCar.cpp OBJ: Xbox\OfficialRelease\BumperCar.obj OPT: /O1 /Oy
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
struct Vector3 { // NAME: proto-pdb:default.pdb
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
class SwampScript; // NAME: proto-pdb:default.pdb

struct AABox;      // NAME: proto-pdb:default.pdb
struct Quaternion { // NAME: proto-pdb:default.pdb
    void Set(float, float, float);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);
struct Matrix;     // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

public:
    SwampScript* GetScript() const;
    const Quaternion& GetRotation() const { return _Rotation; }
    void SetScript(const SwampString&);

    char field_4[0x40 - 0x4];          // 0x04
    Quaternion _Rotation;              // 0x40
    char field_50[0xA4 - 0x50];        // 0x50
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
    void RegisterFrameTick();
protected:
    SwampScript(SwampSceneNode*);
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

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xD4 - 0xA4];        // 0xA4
    int _posBone;                      // 0xD4
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void CheckRegions();

    char field_D8[0x20C - 0xD8];       // 0xD8
    Vector3 _Velocity;                 // 0x20C
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    Vince* GetVince() const { return _vinceCache; }
    void NotifyMatchingCritters(const char*, SwampScript*, unsigned int, void*);

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetCamElevation() const { return _camElevation; }
    float GetCamOrbitRadius() const { return _camOrbitRadius; }
    float GetCamLookAhead() const { return _camLookAhead; }

    char field_0[0x1D8];               // 0x00
    float _camLookAhead;               // 0x1D8
    float _camElevation;               // 0x1DC
    float _camOrbitRadius;             // 0x1E0
    char field_1E4[0xA0C - 0x1E4];     // 0x1E4
};
DECOMP_SIZE_ASSERT(Globals, 0xA0C);

extern Globals gSettings; // NAME: proto-pdb:default.pdb

class GameCam { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x11C];               // 0x00
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void TransitionTo(float radius, float elevation, float lookAhead, float delay);
    void ClearPositionFixed();

    float _speed;                      // 0x11C
    char field_120[0x238 - 0x120];     // 0x120
};
DECOMP_SIZE_ASSERT(VinceCam, 0x238);

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class BumperTargetMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_IMPACT = 1
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_BRACEFORIMPACT = 0x14,
        MESSAGE_IMPACTED = 0x15
    };
    BumperTargetMain(SwampSceneNode* node);
    virtual void HitBy(CollisionType type, SwampSceneNode* critter, const Ptr<CollisionResultObject>& cr);
    virtual void AnimationFinished();
    virtual void Message(SwampScript* source, int message, void* data);

    bool _okayToImpact;                // 0xBC
    SwampScript* _bumperSwitchScript;  // 0xC0
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BumperTargetMain, 0xC4);

// FUNCTION: USA 0x00046AFD
// PROTOTYPE: PREVIEW 0x00434390 identical
// NAME: proto-pdb:default.pdb
void BumperTargetMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_BRACEFORIMPACT:
        _okayToImpact = true;
        _bumperSwitchScript = source;
        break;
    case MESSAGE_IMPACTED:
        _bumperSwitchScript = 0;
        _okayToImpact = false;
        break;
    }
    CritterMain::Message(source, message, data);
}

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript*, int, void*);

    char field_BC[0x139 - 0xBC];       // 0xBC
    bool _oneWayThrow;                 // 0x139
    char field_13A[0x13C - 0x13A];     // 0x13A
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BumperSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_TARGETHIT = 0x18,
        MESSAGE_NEXTDEFCON = 0x19
    };
    BumperSwitchMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);

    Vector3 _worldButtonPos;           // 0x13C
    int _defConLevel;                  // 0x148
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_14C[0x150 - 0x14C];     // 0x14C (not reached; PDB size 0x14C, retail creationFunc pushes 0x150)
};
DECOMP_SIZE_ASSERT(BumperSwitchMain, 0x150); // was 0x14C (PDB); retail push 0x150

class BumperCarMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_PARKED = 0,
        STATE_BOARDING = 1,
        STATE_IDLE = 2,
        STATE_DRIVING = 3,
        STATE_PARKING = 4,
        STATE_CRASHFORWARD = 5,
        STATE_CRASHREVERSE = 6
    };
    BumperCarMain(SwampSceneNode* node);
    virtual bool TestActivate() const;
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void RestoreVinceCam();

public:
    char field_BC[0x144 - 0xBC];       // 0xBC
    float _oldCamSpeed;                // 0x144
    char field_148[0x19C - 0x148];     // 0x148
};
DECOMP_SIZE_ASSERT(BumperCarMain, 0x19C);

// FUNCTION: USA 0x00046BAF
// PROTOTYPE: PREVIEW 0x00434442 identical
// NAME: proto-pdb:default.pdb
bool BumperCarMain::TestActivate() const
{
    return _state == STATE_PARKED ? true : false;
}

// FUNCTION: USA 0x00046BB7
// PROTOTYPE: PREVIEW 0x0043455C identical
// NAME: proto-pdb:default.pdb
void BumperSwitchMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_TARGETHIT:
        _oneWayThrow = true;
        pManager->NotifyMatchingCritters("bumperswitch", this, MESSAGE_NEXTDEFCON, 0);
        break;
    case MESSAGE_NEXTDEFCON:
        _defConLevel++;
        break;
    }
    SwitchMain::Message(source, message, data);
}

class VinceEnterBumperCar : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished();
    Vince* GetVince() { return (Vince*)_node; }

    int _oldBone;                      // 0x4C
};
DECOMP_SIZE_ASSERT(VinceEnterBumperCar, 0x50);

// FUNCTION: USA 0x0004799A
// PROTOTYPE: PREVIEW 0x00435226 identical
// NAME: proto-pdb:default.pdb
void VinceEnterBumperCar::AnimationFinished()
{
    InvalidateScript();
    // !FAKE: c1xx front end; stand-in local; the PDB records no local, retail calls Find once and keeps it in edi
    Critter* tmp = pManager->Find("fanboat");
    tmp->GetScript()->Message(this, 0x12, 0);
    SoundManager::PlaySound("fanbtstart", 0x20, tmp, true, 0.0f, 0.0f);
    GetVince()->_posBone = _oldBone;
    ((VinceCam*)g_CameraManager->Find("vincecam"))->ClearPositionFixed();
    tmp->CheckRegions();
}

// FUNCTION: USA 0x00047592
// PROTOTYPE: PREVIEW 0x00434D94 identical
// NAME: proto-pdb:default.pdb
void BumperCarMain::RestoreVinceCam()
{
    VinceCam* cam = (VinceCam*)g_CameraManager->Find("vincecam");
    cam->TransitionTo(gSettings.GetCamOrbitRadius(), gSettings.GetCamElevation(), gSettings.GetCamLookAhead(), 3.0f);
    cam->_speed = _oldCamSpeed;
}

class BumperCarExitArea : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    BumperCarExitArea(SwampSceneNode* node);
    virtual ~BumperCarExitArea();
    virtual void FrameTick();
    virtual void Message(SwampScript*, int, void*);

    Quaternion _qstart;                // 0x4C
    Quaternion _qend;                  // 0x5C
    Vector3 _velstart;                 // 0x6C
    Vector3 _velend;                   // 0x78
    float _age;                        // 0x84
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BumperCarExitArea, 0x88);

// FUNCTION: USA 0x00047B78
// PROTOTYPE: PREVIEW 0x00435404 ambiguous
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2); wall-vtable-relink (C0203): the vptr store's
// retail vtable (Ghidra PTR_??_GUIClusterMain@@UAEPAXI@Z_*, named for its first slot) and the "noscript" literal (s_noscript_001c9b1c)
// have no symbols.txt/symbols_pin.txt name
BumperCarExitArea::BumperCarExitArea(SwampSceneNode* node) : SwampScript(node)
{
    _qend.Set(0.0f, 0.0f, 1.0f);
    _velend.Set(0.0f, 0.0f, 1.0f);
    _age = 0.0f;
    _qstart = node->GetRotation();
    _velstart = ((Critter*)node)->_Velocity;
    _velstart.y = 0.0f;
    RegisterFrameTick();
    pManager->GetVince()->SetScript("noscript");
}
#endif

// FUNCTION: USA 0x00048034
// PROTOTYPE: PREVIEW 0x00435854 identical
// NAME: proto-pdb:default.pdb
void BumperTargetMain::HitBy(CollisionType type, SwampSceneNode* critter, const Ptr<CollisionResultObject>& cr)
{
    if (critter == pManager->GetVince() && _okayToImpact) {
        SoundManager::PlaySound("kbc_target", 0x20, _node, true, 0.0f, 0.0f);
        if (_state == STATE_IDLE)
            SetState(STATE_IMPACT);
        _okayToImpact = false;
    }
}

// FUNCTION: USA 0x0004820F
// PROTOTYPE: PREVIEW 0x004359E2 identical
// NAME: proto-pdb:default.pdb
void BumperTargetMain::AnimationFinished()
{
    switch (_state) {
    case STATE_IMPACT:
        // !FAKE: c1xx front end; stand-in local; ends the string temporary before GetScript (one expression: bytes mismatch)
        Critter* tmp = pManager->Find("kosmoBotLegs");
        tmp->GetScript()->Message(this, 0x14, 0); // proto-pdb:default.pdb KosmoBotLegsMain::MESSAGE_LEGATTACK
        break;
    }
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

class Sprite { // NAME: proto-pdb:default.pdb
public:
    Sprite();
    void SetTexture(const SwampString&);
    void Render() const;
    void SetLocation(int x, int y)
    {
        _loc.u = (float)x;
        _loc.v = (float)y;
    }
    void SetScale(float s)
    {
        _scale.u = _scale.v = s;
    }
    void SetCentered(bool c) { _centered = c; }

    Vector2 _loc;                      // 0x00
    char field_8[0x10 - 0x8];          // 0x08
    Vector2 _scale;                    // 0x10
    ResourceHandle<TextureResource> _texture; // 0x18
    bool _centered;                    // 0x1C
    char field_1D[0x48 - 0x1D];        // 0x1D
    float _rotation;                   // 0x48
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    UINode();
    virtual void Update();
    virtual void Render(const Matrix&) const;

    char field_A4[0xD4 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

class HUDUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    char field_D4[0xE4 - 0xD4];        // 0xD4
    bool _letterboxEnabled;            // 0xE4

private:
    static HUDUI* _self;
    friend class BumperCarHud;
};

struct Matrix; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000483BF
// PROTOTYPE: PREVIEW 0x00435B92 identical
// NAME: proto-pdb:default.pdb
class BumperCarHud : public UINode { // NAME: proto-pdb:default.pdb
public:
    BumperCarHud();
    virtual void Update();
    virtual void Render(const Matrix& m) const;

    Sprite _bgMeter;                  // 0xD4
    Sprite _needle;                    // 0x120
    float _curSpeed;                   // 0x16C
    float _maxSpeed;                   // 0x170
};
DECOMP_SIZE_ASSERT(BumperCarHud, 0x174);

// FUNCTION: USA 0x00046B3B
// PROTOTYPE: PREVIEW 0x004343CE identical
// NAME: proto-pdb:default.pdb
void BumperCarHud::Update()
{
    float pct;
    if (_maxSpeed == 0.0f) {
        pct = 0.0f;
    } else {
        pct = _curSpeed / _maxSpeed;
        if (pct > 1.0f)
            pct = 1.0f;
    }
    // !FAKE: c1xx front end; retail double literals (__real@400ba5614d2562ef, __real@3fc41b2f7ba3bcbe); the original expression is
    // unknown
    _needle._rotation = pct * 3.455751993850178 - 0.1570796350201586;
}

// FUNCTION: USA 0x00046B84
// NAME: proto-pdb:default.pdb
void BumperCarHud::Render(const Matrix& m) const
{
    if (!HUDUI::_self->_letterboxEnabled) {
        _bgMeter.Render();
        _needle.Render();
    }
}

// FUNCTION: USA 0x000482D4
// PROTOTYPE: PREVIEW 0x00435AA7 identical
// NAME: proto-pdb:default.pdb
BumperCarHud::BumperCarHud()
    : _curSpeed(0.0f), _maxSpeed(1.0f)
{
    _bgMeter.SetTexture("speedometer");
    _bgMeter.SetLocation(500, 375);
    _bgMeter.SetScale(0.75f);
    _bgMeter.SetCentered(true);
    _needle.SetTexture("speedometerneedle");
    _needle.SetLocation(500, 375);
    _needle.SetScale(0.75f);
    _needle.SetCentered(true);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1BumperCarHud@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_BumperCarHud_dtor(BumperCarHud* p)
{
    p->BumperCarHud::~BumperCarHud();
}


void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

// FUNCTION: USA 0x000483DE
// PROTOTYPE: PREVIEW 0x00435BB1 identical
// NAME: proto-pdb:default.pdb
SwampScript* BumperCarMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\bumpercarbumpoff\\BumperCar.h", 35) BumperCarMain(node);
}

class VinceExitBumperCar : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    VinceExitBumperCar(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_4C[0x50 - 0x4C];        // 0x4C (not reached)
};
DECOMP_SIZE_ASSERT(VinceExitBumperCar, 0x50);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x110 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class BumperBlockMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    BumperBlockMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_110[0x11C - 0x110];     // 0x110 (not reached)
};
DECOMP_SIZE_ASSERT(BumperBlockMain, 0x11C);

// FUNCTION: USA 0x00048429
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100, bytes match; wall: ctor pin ??0VinceExitBumperCar@@QAE@PAVSwampSceneNode@@@Z 0x00047A63 is site-corr only (callee-bytes-differ), I10 refuses it (D102): needs an accepted ctor pin (5.5 T27)
SwampScript* VinceExitBumperCar::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\bumpercarbumpoff\\BumperCar.h", 126) VinceExitBumperCar(node);
}
#endif

// FUNCTION: USA 0x00048BDF
// NAME: proto-pdb:default.pdb
SwampScript* BumperCarExitArea::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BumperCarBumpoff\\BumperCar.cpp", 893) BumperCarExitArea(node);
}

// FUNCTION: USA 0x00048C09
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100, bytes provisional (masked 1; wall: ctor ??0BumperSwitchMain@@QAE@PAVSwampSceneNode@@@Z call target 0x00047D19 has no retail VA row: needs a ctor pin + re-cut) (5.5 T27.c7)
SwampScript* BumperSwitchMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BumperCarBumpoff\\BumperCar.cpp", 1022) BumperSwitchMain(node);
}
#endif

// FUNCTION: USA 0x0004991B
// NAME: proto-pdb:default.pdb
SwampScript* BumperTargetMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BumperCarBumpoff\\BumperCar.cpp", 1169) BumperTargetMain(node);
}

// FUNCTION: USA 0x000499AE
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100, bytes match; wall: ctor pin ??0BumperBlockMain@@QAE@PAVSwampSceneNode@@@Z 0x00049945 is site-corr only (callee-bytes-differ; proto row 30 hits), I10 refuses it (D102): needs an accepted ctor pin (5.5 T27)
SwampScript* BumperBlockMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BumperCarBumpoff\\BumperCar.cpp", 969) BumperBlockMain(node);
}
#endif

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x000499D8
// NAME: proto-pdb:default.pdb
BumperCarMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BumperCarMain", creationFunc);
}

// FUNCTION: USA 0x00049A50
// NAME: proto-pdb:default.pdb
VinceExitBumperCar::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VinceExitBumperCar", creationFunc);
}

// FUNCTION: USA 0x00049A8C
// NAME: proto-pdb:default.pdb
BumperCarExitArea::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BumperCarExitArea", creationFunc);
}

// FUNCTION: USA 0x00049AC8
// NAME: proto-pdb:default.pdb
BumperBlockMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BumperBlockMain", creationFunc);
}

// FUNCTION: USA 0x00049B04
// NAME: proto-pdb:default.pdb
BumperSwitchMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BumperSwitchMain", creationFunc);
}

// FUNCTION: USA 0x00049B40
// NAME: proto-pdb:default.pdb
BumperTargetMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BumperTargetMain", creationFunc);
}
