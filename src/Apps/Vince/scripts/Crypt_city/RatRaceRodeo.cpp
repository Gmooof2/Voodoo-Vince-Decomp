// UNIT: src/Apps/Vince/scripts/Crypt_city/RatRaceRodeo.cpp OBJ: Xbox\OfficialRelease\RatRaceRodeo.obj OPT: /O1 /Oy
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
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    // !FAKE: inliner; hand form of the VC7.1 body; vector() { _Buy(0); } folds to these three stores (C0191)
    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; }
    ~vector() { _Tidy(); }
    iterator begin() { return _Myfirst; }
    iterator end() { return _Mylast; }
    void clear() { _Tidy(); }
    void push_back(const T& _Val);
    unsigned int size() const; // retail calls it by name (0x00044948)
    T& operator[](unsigned int _Pos) { return (*(begin() + _Pos)); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const E* _Right);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

const SwampString format(const char* fmt, ...); // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
unsigned int Random();
}

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float Length() const;
    void Normalize();

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    void Set(float, float, float);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class CritterCollision;      // NAME: proto-pdb:default.pdb

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
    void RegisterTimerEvent(unsigned int, float, bool);

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
    virtual SwampString Save();
    CritterCollision MoveMe(float);

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class ExitRampMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    ExitRampMain(SwampSceneNode* node);
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ON = 0x14
    };
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(ExitRampMain, 0xBC);

class EntranceRampMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    EntranceRampMain(SwampSceneNode* node);
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ON = 0x14
    };
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(EntranceRampMain, 0xBC);

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
    ResourceHandle(const ResourceHandle& other);
    ResourceHandle& operator=(const ResourceHandle& other);

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class FloorHoleMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    FloorHoleMain(SwampSceneNode* node);
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_FALL = 0x14
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_FALL = 1
    };
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);

    ResourceHandle<AnimationResource> _animFall; // 0xBC
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(FloorHoleMain, 0xC0);

class RegionManager; // NAME: proto-pdb:default.pdb

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    AnimationManager* GetAnimationManager() { return _animationManager; }
    RegionManager& GetRegionManager() const { return *_regionManager; }
    ModelManager& GetModelManager() const { return *_modelManager; }
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x28 - 0x1C];        // 0x1C
    ModelManager* _modelManager;       // 0x28
    char field_2C[0x30 - 0x2C];        // 0x2C
    RegionManager* _regionManager;     // 0x30
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class AABox; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { DontDraw = 0x2, NoCull = 0x4 };
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    void SetOption(unsigned int option, bool on, bool recurse);
    Vector3 GetFacing() const;
    const Quaternion& GetRotation() const { return _Rotation; }
    const Quaternion& SetRotation(const Quaternion& q);
    SwampScript* GetScript() const;
    unsigned int GetZone() const;

    char field_4[0x3C];                // 0x04
    Quaternion _Rotation;              // 0x40
    char field_50[0xA4 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class GameCam; // NAME: proto-pdb:default.pdb

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void Pause();
    void Reset();
    void Play();

    char field_A4[0xE8 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(FireFlyNode, 0xE8);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x00067E94
// NAME: proto-pdb:default.pdb
class MrsGMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    MrsGMain(SwampSceneNode* node);
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_DAMAGE = 0,
        TIMER_ATTACK = 1
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_FROZEN = 0,
        STATE_INITIAL_IDLE = 1,
        STATE_IDLE = 2,
        STATE_STOMP = 3,
        STATE_HURT_STOMP = 4,
        STATE_DAMAGE = 5,
        STATE_SCREAM = 6,
        STATE_DEATH = 7
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_RESET_ATTACK_TIMER = 0x16
    };
    virtual void TimerEvent(unsigned int timerId);
    virtual void Activate();
    virtual void AnimationFinished();
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void Initialize();
    void KillScream();
    void PostBattle();

    ResourceHandle<AnimationResource> _animIdle;   // 0xBC
    ResourceHandle<AnimationResource> _animStomp;  // 0xC0
    ResourceHandle<AnimationResource> _animDamage; // 0xC4
    ResourceHandle<AnimationResource> _animScream; // 0xC8
    float _screamTimer;                // 0xCC
    bool _screamWillHurt;              // 0xD0
    float _screamDelay;                // 0xD4
    float _stompDelay;                 // 0xD8
    float _repeatDelay;                // 0xDC
    int _attackCounter;                // 0xE0
    int _floorPieceCounter;            // 0xE4
    std::vector<Vector3> _fallingChunkPositions; // 0xE8
    int _hurtCounter;                  // 0xF8
    GameCam* _stompCamera;             // 0xFC
    FireFlyNode* _shockwaveEffect;     // 0x100
    int _attackCounterMax;             // 0x104
    float _minTimeBetweenAttacks;      // 0x108
    float _maxTimeBetweenAttacks;      // 0x10C
    bool _canAttack;                   // 0x110
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(MrsGMain, 0x114);

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x1AC];               // 0x00
    SwampString _miscPostBossMusic;    // 0x1AC
    char field_1C8[0x3DC - 0x1C8];     // 0x1C8
    float _ratRaceRubbleGravity;       // 0x3DC
    char field_3E0[0x3E4 - 0x3E0];     // 0x3E0
    float _ratRaceRubbleRandomness;    // 0x3E4
    int _ratRaceNumScreamsStage1;      // 0x3E8
    char field_3EC[0x3F4 - 0x3EC];     // 0x3EC
    float _ratRaceScreamDelay1;        // 0x3F4
    float _ratRaceScreamDelay2;        // 0x3F8
    float _ratRaceScreamDelay3;        // 0x3FC
    float _ratRaceStompDelay1;         // 0x400
    float _ratRaceStompDelay2;         // 0x404
    float _ratRaceStompDelay3;         // 0x408
    float _ratRacePatternRepeatDelay1; // 0x40C
    float _ratRacePatternRepeatDelay2; // 0x410
    float _ratRacePatternRepeatDelay3; // 0x414
    char field_418[0xA0C - 0x418];     // 0x418
};
DECOMP_SIZE_ASSERT(Globals, 0xA0C);

extern Globals gSettings; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00063EF8
// PROTOTYPE: PREVIEW 0x0045048F identical
// NAME: proto-pdb:default.pdb
void MrsGMain::Initialize()
{
    _screamDelay = gSettings._ratRaceScreamDelay1;
    _stompDelay = gSettings._ratRaceStompDelay1;
    _repeatDelay = gSettings._ratRacePatternRepeatDelay1;
    _attackCounterMax = gSettings._ratRaceNumScreamsStage1;
    _hurtCounter = 0;
    _attackCounter = _attackCounterMax;
    _floorPieceCounter = 1;
}

// FUNCTION: USA 0x00063F41
// PROTOTYPE: PREVIEW 0x004504D8 identical
// NAME: proto-pdb:default.pdb
void MrsGMain::KillScream()
{
    _screamTimer = 0.0f;
    _screamWillHurt = false;
    if (_shockwaveEffect) {
        _shockwaveEffect->Pause();
        _shockwaveEffect->SetOption(SwampSceneNode::DontDraw, true, false);
    }
}

// FUNCTION: USA 0x00063F71
// PROTOTYPE: PREVIEW 0x00450508 identical
// NAME: proto-pdb:default.pdb
void MrsGMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_DAMAGE:
        Message(this, 0, 0); // message 0: name unknown
        break;
    case TIMER_ATTACK:
        if (_state == STATE_IDLE || _state == STATE_INITIAL_IDLE) {
            if (_attackCounter != 0) {
                SetState(STATE_SCREAM);
                _attackCounter--;
            } else {
                SetState(STATE_STOMP);
                _attackCounter = _attackCounterMax;
            }
        }
        break;
    }
}

// FUNCTION: USA 0x000641A6
// PROTOTYPE: PREVIEW 0x0045073D identical
// NAME: proto-pdb:default.pdb
SwampScript* ExitRampMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 348) ExitRampMain(node);
}

// FUNCTION: USA 0x000682BC
// NAME: proto-pdb:default.pdb
ExitRampMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("ExitRampMain", creationFunc);
}

// FUNCTION: USA 0x000641D0
// PROTOTYPE: PREVIEW 0x00450767 identical
// NAME: proto-pdb:default.pdb
SwampScript* EntranceRampMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 379) EntranceRampMain(node);
}

// FUNCTION: USA 0x000682F8
// NAME: proto-pdb:default.pdb
EntranceRampMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("EntranceRampMain", creationFunc);
}

// FUNCTION: USA 0x00067C15
// PROTOTYPE: PREVIEW 0x004541CC identical
// NAME: proto-pdb:default.pdb
SwampScript* FloorHoleMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 287) FloorHoleMain(node);
}

// FUNCTION: USA 0x00068244
// NAME: proto-pdb:default.pdb
FloorHoleMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("FloorHoleMain", creationFunc);
}

// FUNCTION: USA 0x00067EDE
// PROTOTYPE: PREVIEW 0x00454495 identical
// NAME: proto-pdb:default.pdb
SwampScript* MrsGMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 210) MrsGMain(node);
}

// FUNCTION: USA 0x00068208
// NAME: proto-pdb:default.pdb
MrsGMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("MrsGMain", creationFunc);
}

class Critter; // NAME: proto-pdb:default.pdb
class AnimationControl; // NAME: proto-pdb:default.pdb

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~CarriedObjectMain();

    char field_BC[0xF8 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

// FUNCTION: USA 0x00064085
// NAME: proto-pdb:default.pdb
class WeaponMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> _animAttack; // 0xF8
    SwampString _CarriedObjectClass;   // 0xFC
    SwampString _effectName;           // 0x118
    char field_134[0x144 - 0x134];     // 0x134
    std::vector<Critter*> _affectedCritters; // 0x144
    char field_154[0x160 - 0x154];     // 0x154
    SwampString _weaponClass;          // 0x160
    float _damageMultiplier;           // 0x17C
};
DECOMP_SIZE_ASSERT(WeaponMain, 0x180);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1WeaponMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WeaponMain_dtor(WeaponMain* p)
{
    p->WeaponMain::~WeaponMain();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1MrsGMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MrsGMain_dtor(MrsGMain* p)
{
    p->MrsGMain::~MrsGMain();
}

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_ITEM = 0x1
};

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetCollidable(bool c) { _collidable = c; }

    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    float GetHeight() const { return _height; }
    float GetRadius() const { return _radius; }

    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool, bool);
    void OverrideTexture(const SwampString& name, const SwampString& newName);
    void SetShadow(bool, float);
    void SetProjectedShadow(const ResourceHandle<ModelResource>&);
    void SetProjectedShadowLight(const Vector3& loc, const Vector3& color)
    {
        _projectedShadowLightLoc = loc;
        _projectedShadowLightColor = color;
    }

    char field_B4[0xC2 - 0xB4];        // 0xB4
    bool _useAllLights;                // 0xC2
    char field_C3[0xFC - 0xC3];        // 0xC3
    Vector3 _projectedShadowLightLoc;  // 0xFC
    Vector3 _projectedShadowLightColor; // 0x108
    char field_114[0x200 - 0x114];     // 0x114
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

// FUNCTION: USA 0x00066A90
// PROTOTYPE: PREVIEW 0x0045301C identical
// NAME: proto-pdb:default.pdb
void FloorHoleMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_FALL:
        SoundManager::PlaySound("ccrr_ffall", 0, _node, true, 0.0f, 0.0f);
        ((GatorNode*)_node)->StartAnimation(_animFall, false, false);
        break;
    }
}

// FUNCTION: USA 0x00063FC8
// PROTOTYPE: PREVIEW 0x0045055F identical
// NAME: proto-pdb:default.pdb
void FloorHoleMain::AnimationFinished()
{
    switch (_state) {
    case STATE_FALL:
        _node->SetOption(SwampSceneNode::DontDraw, true, false);
        ((CollidableNode*)_node)->SetCollidable(false);
        break;
    }
}

struct Vector4 { // NAME: proto-pdb:default.pdb
    operator Vector3() const { return *(const Vector3*)this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    bool IsValid() const { return _Object != 0; }
    const T& operator*() const { return *_Object; }

    T* _Object;                        // 0x00
};

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    int _RefCount;                     // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    const Vector4& ImpactPlane() const { return _plane; }

    char field_4[0x10 - 0x4];          // 0x04
    Vector4 _plane;                    // 0x10
    char field_20[0x40 - 0x20];        // 0x20
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};

class CritterCollision { // NAME: proto-pdb:default.pdb
public:
    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};
DECOMP_SIZE_ASSERT(CritterCollision, 0x8);

class SoundNode; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000649EC
// PROTOTYPE: PREVIEW 0x00450F83 identical
// NAME: proto-pdb:default.pdb
class RodeoRatMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    RodeoRatMain(SwampSceneNode* node);
    enum States { // NAME: proto-pdb:default.pdb
        STATE_NULL = 0,
        STATE_ATTACK = 1,
        STATE_DAMAGE_BARRIER = 2
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ATTACK = 0x14,
        MESSAGE_MRSG_FINISHED = 0x16,
        MESSAGE_STARTRIDE = 0x1A
    };
    virtual void CollidedWith(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr);
    virtual SwampString Save();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void UpdateCamera(bool resetcamera);

    SoundNode* _runSound;              // 0xBC
    ResourceHandle<AnimationResource> _animAttack;         // 0xC0
    ResourceHandle<AnimationResource> _animDamageBarrier;  // 0xC4
    ResourceHandle<AnimationResource> _animDamageSound;    // 0xC8
    ResourceHandle<AnimationResource> _animFallDeath;      // 0xCC
    ResourceHandle<AnimationResource> _animScreamDeath;    // 0xD0
    ResourceHandle<AnimationResource> _animJumpFloat;      // 0xD4
    ResourceHandle<AnimationResource> _animJumpLand;       // 0xD8
    ResourceHandle<AnimationResource> _animJumpStart;      // 0xDC
    ResourceHandle<AnimationResource> _animRun;            // 0xE0
    ResourceHandle<AnimationResource> _animSlowdown;       // 0xE4
    ResourceHandle<AnimationResource> _animWeakJumpStart;  // 0xE8
    ResourceHandle<AnimationResource> _animWeakJumpFloat;  // 0xEC
    ResourceHandle<AnimationResource> _animWeakJumpLand;   // 0xF0
    ResourceHandle<AnimationResource> _animStagger;        // 0xF4
    char field_F8[0x128 - 0xF8];       // 0xF8
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(RodeoRatMain, 0x128);

// FUNCTION: USA 0x0006700D
// PROTOTYPE: PREVIEW 0x004535C4 identical
// NAME: proto-pdb:default.pdb
SwampScript* RodeoRatMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 35) RodeoRatMain(node);
}

// FUNCTION: USA 0x00068190
// NAME: proto-pdb:default.pdb
RodeoRatMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("RodeoRatMain", creationFunc);
}

// FUNCTION: USA 0x00063D47
// PROTOTYPE: PREVIEW 0x004502DE identical
// NAME: proto-pdb:default.pdb
void RodeoRatMain::CollidedWith(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr)
{
    if (cr.IsValid() && type == HorizontalCollision) {
        // !FAKE: inliner; dummy local; stand-in for the inlined dot product's first operand (not a PDB local)
        Vector3 v0 = (*cr).ImpactPlane();
        // !FAKE: c2 register allocation; dummy local; keeps the eax read of the returned facing (const& form 98.05 -> 100.0)
        const Vector3* p = &_node->GetFacing();
        if (v0.x * p->x + v0.y * p->y + v0.z * p->z < -0.5f)
            SetState(STATE_DAMAGE_BARRIER);
    }
}

// Retail layout: Critter _CritterType 0x258 -> 0x268 (+0x10 above _Velocity 0x20C).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0xC];               // 0x200
    void SetHomePosition(const Vector3& p) { _homePosition = p; }
    bool IsOnFloor() const { return _OnFloor; }

    Vector3 _Velocity;                 // 0x20C
    char field_218[0x234 - 0x218];     // 0x218
    Vector3 _homePosition;             // 0x234
    char field_240[0x250 - 0x240];     // 0x240
    bool _OnFloor;                     // 0x250
    char field_251[0x268 - 0x251];     // 0x251
    CritterType _CritterType;          // 0x268
};

class RockChunkMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    RockChunkMain(SwampSceneNode* node);
    virtual ~RockChunkMain();
    enum States { // NAME: proto-pdb:default.pdb
        STATE_HOLD = 0,
        STATE_FALLING = 1
    };
    virtual void FrameTick();
    virtual void TimerEvent(unsigned int timerId);

private:
    virtual void SetState(int);
    void DestroyRock();
    static SwampScript* creationFunc(SwampSceneNode* node);

    static int _chunkCount; // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(RockChunkMain, 0xBC);

// FUNCTION: USA 0x0006417C
// PROTOTYPE: PREVIEW 0x00450713 identical
// NAME: proto-pdb:default.pdb
SwampScript* RockChunkMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 317) RockChunkMain(node);
}

// FUNCTION: USA 0x00068280
// NAME: proto-pdb:default.pdb
RockChunkMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("RockChunkMain", creationFunc);
}

// FUNCTION: USA 0x00063C96
// PROTOTYPE: PREVIEW 0x0045022D identical
// NAME: proto-pdb:default.pdb
void RockChunkMain::TimerEvent(unsigned int timerId)
{
    if (timerId == 0)
        SetState(STATE_FALLING);
}

// FUNCTION: USA 0x00063FFD
// PROTOTYPE: PREVIEW 0x00450594 identical
// NAME: proto-pdb:default.pdb
void RockChunkMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_FALLING: {
        Vector3 startPos = _node->GetWorldPosition();
        startPos.y += ((Critter*)_node)->GetHeight() * 3.0f;
        ((Critter*)_node)->SetProjectedShadowLight(startPos, Vector3(0.75f, 0.75f, 0.75f));
        break;
    }
    }
}

class VinceOnRatMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    VinceOnRatMain(SwampSceneNode* node);
    virtual void InvalidateScript();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _animAttack;         // 0xBC
    ResourceHandle<AnimationResource> _animDamageBarrier;  // 0xC0
    ResourceHandle<AnimationResource> _animDamageSound;    // 0xC4
    ResourceHandle<AnimationResource> _animScreamDeath;    // 0xC8
    ResourceHandle<AnimationResource> _animJumpFloat;      // 0xCC
    ResourceHandle<AnimationResource> _animJumpLand;       // 0xD0
    ResourceHandle<AnimationResource> _animJumpStart;      // 0xD4
    ResourceHandle<AnimationResource> _animRun;            // 0xD8
    ResourceHandle<AnimationResource> _animSlowdown;       // 0xDC
    ResourceHandle<AnimationResource> _animWeakJumpStart;  // 0xE0
    ResourceHandle<AnimationResource> _animWeakJumpFloat;  // 0xE4
    ResourceHandle<AnimationResource> _animWeakJumpLand;   // 0xE8
    ResourceHandle<AnimationResource> _animStagger;        // 0xEC
    bool _whammyReady;                 // 0xF0
    char field_F1[0xF4 - 0xF1];        // 0xF1
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(VinceOnRatMain, 0xF4);

// FUNCTION: USA 0x00067034
// PROTOTYPE: PREVIEW 0x004535EB identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceOnRatMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 140) VinceOnRatMain(node);
}

// FUNCTION: USA 0x000681CC
// NAME: proto-pdb:default.pdb
VinceOnRatMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VinceOnRatMain", creationFunc);
}

#ifdef NON_MATCHING
// best: match_one objdiff 90.86 at /O1 /Oy (SetState(0) inlined; retail calls 0x00063FFD); --control "/O1 /Oy /Ob1" PASS objdiff 100 + bytes (masked 0), T15.c4: needs unit opt /Ob1
// FUNCTION: USA 0x000640D5
// PROTOTYPE: PREVIEW 0x0045066C near-identical
// NAME: proto-pdb:default.pdb
RockChunkMain::RockChunkMain(SwampSceneNode* node) : CritterMain(node)
{
    ((Critter*)_node)->_Velocity = Vector3(0.0f, -1.0f, 0.0f);
    ((Critter*)_node)->_CritterType = CRITTER_TYPE_ITEM;
    SetState(0);
    RegisterFrameTick();
    _chunkCount++;
}
#endif

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    BasicDoorMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);

    ResourceHandle<AnimationResource> _animOpen; // 0xBC
    char field_C0[0x110 - 0xC0];       // 0xC0
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class MrsGCollapsedMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    MrsGCollapsedMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(MrsGCollapsedMain, 0x110);

// FUNCTION: USA 0x000680E1
// PROTOTYPE: PREVIEW 0x00454656 identical
// NAME: proto-pdb:default.pdb
SwampScript* MrsGCollapsedMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 410) MrsGCollapsedMain(node);
}

// FUNCTION: USA 0x00068334
// NAME: proto-pdb:default.pdb
MrsGCollapsedMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("MrsGCollapsedMain", creationFunc);
}

class Vince; // NAME: proto-pdb:default.pdb

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    Critter* Create(const SwampString&, const SwampString&, const char*, ResourceHandle<ModelResource>, const Vector3&, const Quaternion&, unsigned int);
    void FindAllMatching(std::vector<Critter*>&, const SwampString&, unsigned int);
    void Remove(const SwampString&);
    void ActivateMatching(const SwampString&, unsigned int);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00065C30
// PROTOTYPE: PREVIEW 0x004521BC identical
// NAME: proto-pdb:default.pdb
SwampString RodeoRatMain::Save()
{
    if (pManager->Find("mrsg"))
        SetState(STATE_NULL);
    return CritterMain::Save();
}

// FUNCTION: USA 0x00066EEE
// PROTOTYPE: PREVIEW 0x004534A5 identical
// NAME: proto-pdb:default.pdb
void MrsGCollapsedMain::Message(SwampScript* source, int message, void* data)
{
    BasicDoorMain::Message(source, message, data);
    if (message == 6) { // proto-pdb:default.pdb SwampScript::SOUND_STREAMER_FINISHED
        // !FAKE: c1xx front end; stand-in local; ends the string temporary before the test (not a PDB local)
        Critter* tmp = pManager->Find("mrsg");
        if (!tmp) {
            _node->SetOption(SwampSceneNode::DontDraw, false, true);
        } else {
            _node->SetOption(SwampSceneNode::DontDraw, true, true);
            _node->SetRotation(tmp->GetRotation());
        }
    }
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 3), T15.c4: needs pins ??_7MrsGCollapsedMain@@6B@ 0x001D9F60, literals 0x001D9FD4 "mrsgcollapsed_collapse", 0x001D9F50 "mrsgDamage02"
// FUNCTION: USA 0x00068019
// PROTOTYPE: PREVIEW 0x004545D0 near-identical
// NAME: proto-pdb:default.pdb
MrsGCollapsedMain::MrsGCollapsedMain(SwampSceneNode* node) : BasicDoorMain(node)
{
    _animOpen = g_swamp->GetAnimationManager()->Load("mrsgcollapsed_collapse");
    ((Critter*)_node)->_useAllLights = true;
    ((Critter*)_node)->OverrideTexture("mrsg", "mrsgDamage02");
}
#endif

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1RodeoRatMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_RodeoRatMain_dtor(RodeoRatMain* p)
{
    p->RodeoRatMain::~RodeoRatMain();
}

// FUNCTION: USA 0x0006608B
// PROTOTYPE: PREVIEW 0x00452617 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1VinceOnRatMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceOnRatMain_dtor(VinceOnRatMain* p)
{
    p->VinceOnRatMain::~VinceOnRatMain();
}

// ??_GRodeoRatMain 0x000649D0, ??_GVinceOnRatMain 0x0006606F, ??_GWeaponMain 0x00066B4C, ??_GRockChunkMain 0x00067BF9,
// ??_GMrsGMain 0x00067E78 unmatched, no draft (5.3 T6.c13): companion wall: a virtual ??_G is emitted only with its
// class vtable, i.e. the ctor; ??0 of RodeoRatMain (0x00064515), VinceOnRatMain (0x00065C84), WeaponMain (0x0012150E),
// RockChunkMain (0x000640D5, guarded above: site-corr pins), MrsGMain (0x00067C3F) are not passing rows.
// Lift: match the ctor (RockChunkMain: corr-verify its vtable and _chunkCount pins).

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetupShadows();
    void SetWhammyReady(bool);
};

class GameCam { // NAME: proto-pdb:default.pdb
public:
    void SetShakeFactor(float, float);

    char field_0[0x11C];               // 0x00
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void TransitionToDefault(float);
    void SetDirectionFixed(const Vector3&, float);
    void ResetLocation(bool);
    void HoldTarget(const Vector3& pos)
    {
        _holdTarget = true;
        _heldTargetPos = pos;
    }

    char field_11C[0x124 - 0x11C];     // 0x11C
    bool _directionFixed;              // 0x124
    char field_125[0x1A0 - 0x125];     // 0x125
    float _hspeedFactor;               // 0x1A0
    char field_1A4[0x1AC - 0x1A4];     // 0x1A4
    float _hmoveMax;                   // 0x1AC
    char field_1B0[0x210 - 0x1B0];     // 0x1B0
    bool _holdTarget;                  // 0x210
    char field_211[0x214 - 0x211];     // 0x211
    Vector3 _heldTargetPos;            // 0x214
    char field_220[0x238 - 0x220];     // 0x220
};
DECOMP_SIZE_ASSERT(VinceCam, 0x238);

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00066320
// PROTOTYPE: PREVIEW 0x004528AC identical
// NAME: proto-pdb:default.pdb
void VinceOnRatMain::InvalidateScript()
{
    ((GatorNode*)_node)->SetShadow(false, 0.0f);
    _node->SetOption(SwampSceneNode::DontDraw, false, false);
    ((Vince*)_node)->SetWhammyReady(_whammyReady);
    if (_node == pManager->GetVince())
        pManager->GetVince()->SetupShadows();
    else
        ((GatorNode*)_node)->SetShadow(true, 0.0f);
    // !FAKE: c1xx front end; stand-in local; ends the string temporary before the test (not a PDB local)
    VinceCam* tmp = (VinceCam*)g_CameraManager->Find("vincecam");
    if (tmp) {
        tmp->_holdTarget = false;
        tmp->_directionFixed = false;
        tmp->_hspeedFactor = 32.0f;
        tmp->_hmoveMax = 2.0f;
        tmp->TransitionToDefault(1.0f);
    }
    SwampScript::InvalidateScript();
}

// FUNCTION: USA 0x00064B92
// PROTOTYPE: PREVIEW 0x00451129 identical
// NAME: proto-pdb:default.pdb
void RodeoRatMain::UpdateCamera(bool resetcamera)
{
    // !FAKE: c1xx front end; stand-in local; ends the string temporary before the calls (not a PDB local)
    Critter* mrsg = pManager->Find("mrsg");
    // !FAKE: inliner; dummy locals; hand form of the inlined difference of the two returned positions (C0191)
    const Vector3* p = &_node->GetWorldPosition();
    const Vector3* q = &mrsg->GetWorldPosition();
    Vector3 tocenter;
    tocenter.x = q->x - p->x;
    tocenter.z = q->z - p->z;
    tocenter.y = 0.0f;
    tocenter.Normalize();
    Vector3 dir(-tocenter.z, 0.0f, tocenter.x);
    // !FAKE: c1xx front end; stand-in local; ends the string temporary before the calls (not a PDB local)
    VinceCam* cam = (VinceCam*)g_CameraManager->Find("vincecam");
    cam->SetDirectionFixed(dir, 1.0f);
    cam->HoldTarget(_node->GetWorldPosition());
    if (resetcamera)
        cam->ResetLocation(false);
}

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger* Find(const SwampString&);
};

class StartBattleRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(StartBattleRegionMain, 0x4C);

// FUNCTION: USA 0x00066F64
// PROTOTYPE: PREVIEW 0x0045351B identical
// NAME: proto-pdb:default.pdb
void StartBattleRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (node == pManager->GetVince()) {
        // !FAKE: c1xx front end; stand-in local; ends the string temporary before the call (not a PDB local)
        RegionTrigger* tmp = g_swamp->GetRegionManager().Find("cutscene");
        bool didintro = false;
        tmp->GetScript()->Message(this, 0x14, &didintro); // message 0x14: name unknown
        if (didintro) {
            // !FAKE: c1xx front end; stand-in local; ends the string temporary before the test (not a PDB local)
            Critter* p = pManager->Find("MrsG");
            if (p)
                p->GetScript()->Activate();
        }
    }
}

// FUNCTION: USA 0x00067F08
// PROTOTYPE: PREVIEW 0x004544BF identical
// NAME: proto-pdb:default.pdb
void MrsGMain::Activate()
{
    // !FAKE: c2 register allocation; dummy local; pManager read before the GetZone call (95.25 -> 98.89);
    // also assigned at the Find call so both pManager reads share the Find-result register (ebx/edi as retail)
    CritterManager* p;
    // !FAKE: c1xx front end; stand-in local; ends the string temporary before the call (not a PDB local)
    Critter* tmp = (p = pManager)->Find("rodeorat");
    tmp->GetScript()->Message(this, RodeoRatMain::MESSAGE_STARTRIDE, 0);
    _fallingChunkPositions.clear();
    std::vector<Critter*> rocklocs;
    (p = pManager)->FindAllMatching(rocklocs, "fallingChunk", _node->GetZone());
    for (std::vector<Critter*>::iterator it = rocklocs.begin(); it != rocklocs.end(); ++it) {
        _fallingChunkPositions.push_back((*it)->GetWorldPosition());
        (*it)->SetOption(SwampSceneNode::DontDraw, true, false);
    }
    SetState(STATE_INITIAL_IDLE);
    RegisterFrameTick();
}

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode* node);
    bool InRegion() const { return _inRegion; }

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_4E[0x64 - 0x4E];        // 0x4E
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class RatRaceRodeoRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    RatRaceRodeoRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(RatRaceRodeoRegionMain, 0x64);

class CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    static void SetStream(const SwampString&, bool);
};

// FUNCTION: USA 0x000641FA
// PROTOTYPE: PREVIEW 0x00450791 identical
// NAME: proto-pdb:default.pdb
void RatRaceRodeoRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (!InRegion()) {
        LevelScript::EnteringRegion(node);
        pManager->ActivateMatching("Exit", 0);
        pManager->GetVince()->SetHomePosition(pManager->GetVince()->GetWorldPosition());
        // !FAKE: c1xx front end; stand-in local; ends the string temporary before the call (not a PDB local)
        RegionTrigger* tmp = g_swamp->GetRegionManager().Find("cutscene");
        bool didintro = false;
        tmp->GetScript()->Message(this, 0x14, &didintro); // message 0x14: name unknown
        CutsceneDriver::SetStream(didintro ? "ratRaceDeathCutscene" : "ratRacePreludeCutscene", false);
        pManager->Remove("vincecowboy");
    }
}

// FUNCTION: USA 0x00068130
// NAME: proto-pdb:default.pdb
SwampScript* RatRaceRodeoRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\ratracerodeo\\RatRaceRodeo.h", 18) RatRaceRodeoRegionMain(node);
}

// FUNCTION: USA 0x00068154
// NAME: proto-pdb:default.pdb
RatRaceRodeoRegionMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("RatRaceRodeoRegionMain", creationFunc);
}

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Return(float);
};

// FUNCTION: USA 0x00066740
// PROTOTYPE: PREVIEW 0x00452CCC identical
// NAME: proto-pdb:default.pdb
void MrsGMain::AnimationFinished()
{
    switch (_state) {
    case STATE_STOMP:
        SetState(STATE_IDLE);
        break;
    case STATE_HURT_STOMP: {
        SetState(STATE_IDLE);
        AnimCam::Return(0.0f);
        _stompCamera = 0;
        // !FAKE: c1xx front end; stand-in local; ends the string temporary before the call (not a PDB local)
        Critter* tmp = pManager->Find("rodeorat");
        tmp->GetScript()->Message(this, RodeoRatMain::MESSAGE_MRSG_FINISHED, 0);
        float delay;
        switch (_hurtCounter) {
        case 0:
            delay = gSettings._ratRacePatternRepeatDelay1;
            break;
        case 1:
            delay = gSettings._ratRacePatternRepeatDelay2;
            break;
        default:
            delay = gSettings._ratRacePatternRepeatDelay3;
            break;
        }
        Message(this, MESSAGE_RESET_ATTACK_TIMER, &delay);
        break;
    }
    case STATE_DAMAGE:
        SetState(STATE_HURT_STOMP);
        break;
    case STATE_SCREAM: {
        float delay;
        switch (_hurtCounter) {
        case 0:
            delay = _attackCounter ? gSettings._ratRaceScreamDelay1 : gSettings._ratRaceStompDelay1;
            break;
        case 1:
            delay = _attackCounter ? gSettings._ratRaceScreamDelay2 : gSettings._ratRaceStompDelay2;
            break;
        default:
            delay = _attackCounter ? gSettings._ratRaceScreamDelay3 : gSettings._ratRaceStompDelay3;
            break;
        }
        SetState(STATE_IDLE);
        Message(this, MESSAGE_RESET_ATTACK_TIMER, &delay);
        break;
    }
    }
}

// FUNCTION: USA 0x0006778B
// PROTOTYPE: PREVIEW 0x00453D42 identical
// NAME: proto-pdb:default.pdb
void MrsGMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    switch (_state) {
    case STATE_SCREAM:
        _screamTimer = 5.0f;
        _screamWillHurt = true;
        _shockwaveEffect->Reset();
        _shockwaveEffect->Play();
        _shockwaveEffect->SetOption(SwampSceneNode::DontDraw, false, false);
        SoundManager::PlaySound("ccrr_shwave", 0, 0, false, 0.0f, 0.0f);
        break;
    case STATE_HURT_STOMP:
        if (_stompCamera)
            _stompCamera->SetShakeFactor(0.75f, -1.0f);
        if (_floorPieceCounter && _hurtCounter <= 2) {
            SwampString piecename = format("floorHole%02i", _floorPieceCounter);
            // !FAKE: c1xx naming; neutral stand-in tmp; no PDB local
            Critter* tmp = pManager->Find(piecename);
            if (tmp) {
                tmp->GetScript()->Message(this, FloorHoleMain::MESSAGE_FALL, 0);
                _floorPieceCounter++;
            } else {
                _floorPieceCounter = 0;
            }
        }
        break;
    case STATE_STOMP:
        if (_canAttack) {
            // !FAKE: c1xx front end; stand-in local; ends the string temporary before the call (not a PDB local)
            GameCam* tmp = g_CameraManager->Find("vincecam");
            tmp->SetShakeFactor(0.75f, -1.0f);
            // !FAKE: c1xx naming; neutral stand-in v0; no PDB local
            for (int v0 = _fallingChunkPositions.size() - 1; v0 >= 0; --v0) {
                SwampString chunkname = format("rockchunk%02i", v0 + 1);
                SwampString modelname = format("fallingChunk%02i", v0 + 1);
                Critter* chunk;
                {
                    // !FAKE: c1xx front end + c2 register allocation; stand-in locals v1, v2, v4-v6, p in an inner block (not PDB locals); load order:
                    // retail builds the string/Quaternion temporaries and reads _node, the model manager, the position and pManager
                    // before GetZone (temporary args 76.91 -> block 92.94 -> 98.83 -> 100; Quaternion(x,y,z) ctor stayed out of line)
                    SwampString v2("RockChunk");
                    SwampSceneNode* v5 = _node;
                    Quaternion v1;
                    v1.Set(0.0f, 0.0f, 0.0f);
                    ModelManager& v6 = g_swamp->GetModelManager();
                    const Vector3& v4 = _fallingChunkPositions[v0];
                    CritterManager* p = pManager;
                    chunk = p->Create(chunkname, v2, "RockChunk", v6.Load(modelname + "_base"), v4, v1, v5->GetZone());
                }
                chunk->StartAnimation(g_swamp->GetAnimationManager()->Load(modelname + "_idle"), false, false);
                chunk->SetProjectedShadow(g_swamp->GetModelManager().Load(modelname + "_shadow_base"));
                chunk->SetOption(SwampSceneNode::NoCull, true, false);
                chunk->GetScript()->RegisterTimerEvent(0, BeepLib::RandomFloat(0.0f, gSettings._ratRaceRubbleRandomness), false);
            }
        }
        break;
    }
    SoundManager::PlaySound(format("mrsgstomp%02d", BeepLib::Random() % 3 + 1), 0, 0, false, 0.0f, 0.0f);
}

// FUNCTION: USA 0x00066D5B
// PROTOTYPE: PREVIEW 0x004532CB identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 71.17, wall: retail's Length is an inline-asm sqrt (rsqrtss + fcompi/fcmovbe/ffree), D116 forbids __asm (C0219)
void RockChunkMain::FrameTick()
{
    if (_state == STATE_FALLING) {
        if (((Critter*)_node)->IsOnFloor()) {
            DestroyRock();
        } else {
            MoveMe(gSettings._ratRaceRubbleGravity);
            // !FAKE: c2 register allocation; stand-in local; retail holds Vince in esi across the calls (not a PDB local)
            Vince* p = pManager->GetVince();
            // !FAKE: inliner; hand form of the Vector3 difference, stand-ins v1, v2; no free operator in the PDB, no before/after recorded
            const Vector3& v1 = _node->GetWorldPosition();
            const Vector3& v2 = p->GetWorldPosition();
            Vector3 tovince(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
            if (tovince.y > -p->GetHeight()) {
                tovince.y = 0.0f;
                if (tovince.Length() < ((Critter*)_node)->GetRadius() + p->GetRadius()) {
                    // !FAKE: c1xx front end; stand-in local; ends the string temporary before the call (not a PDB local)
                    Critter* tmp = pManager->Find("rodeorat");
                    tmp->GetScript()->Message(this, RodeoRatMain::MESSAGE_ATTACK, 0);
                    InvalidateScript();
                }
            }
        }
    }
}
#endif

class MusicManager { // NAME: proto-pdb:default.pdb
public:
    void Reset();
    void PlayTrack(const SwampString&);
};

extern MusicManager* g_MusicManager; // NAME: proto-pdb:default.pdb

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    void NotifyMatchingLocators(const char* match, SwampScript* from, unsigned int message, void* data) const;
};

extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00066866
// PROTOTYPE: PREVIEW 0x00452DF2 identical
// NAME: proto-pdb:default.pdb
void MrsGMain::PostBattle()
{
    // !FAKE: c1xx front end; stand-in local; ends the string temporary before the test (not a PDB local)
    Critter* tmp = pManager->Find("exitramp");
    if (tmp)
        tmp->GetScript()->Message(this, ExitRampMain::MESSAGE_ON, 0);
    tmp = pManager->Find("entranceramp");
    if (tmp)
        tmp->GetScript()->Message(this, EntranceRampMain::MESSAGE_ON, 0);
    tmp = pManager->Find("ratWall");
    if (tmp)
        tmp->Invalidate();
    tmp = pManager->Find("floorhole01");
    if (tmp)
        tmp->Invalidate();
    tmp = pManager->Find("floorhole02");
    if (tmp)
        tmp->Invalidate();
    g_MusicManager->Reset();
    g_MusicManager->PlayTrack(gSettings._miscPostBossMusic);
    g_locatorManager->NotifyMatchingLocators("locator_loadFrom", 0, 0x1A, 0); // message 0x1A: name unknown
}
