// UNIT: src/Apps/Vince/scripts/Sewers/RocketLab.cpp OBJ: Xbox\OfficialRelease\RocketLab.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string() { _Myres = 15; _Eos(0); }
    basic_string(const E* ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    int compare(const E* _Ptr) const;

protected:
    void _Eos(unsigned int _Newsize);
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    char _Alval;                       // 0x00
    union {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
template <class T, class A = allocator<T> > class deque { // NAME: proto-pdb:default.pdb
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class Critter;               // NAME: proto-pdb:default.pdb

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
    ResourceHandle& operator=(const ResourceHandle& other);

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

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
    void UnRegisterFrameTick();
    void RegisterTimerEvent(unsigned int, float, bool);
    SwampSceneNode* GetNode() { return _node; }

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
    virtual void Load(const BeepTDF&);
    virtual void Message(SwampScript*, int, void*);
    Critter* GetCritter();

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode*);

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_4E[0x64 - 0x4E];        // 0x4E (not reached)
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    static void SetStream(const SwampString& wav, bool looped);
};

class RocketLabRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    RocketLabRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(RocketLabRegionMain, 0x64);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum State { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0,
        STATE_OPEN = 1,
        STATE_OPENING = 2,
        STATE_CLOSING = 3,
        STATE_LOCKEDOPEN = 4,
        STATE_BASICDOOR_USER = 5
    };
    virtual void Activate();
    virtual void AnimationFinished();
    virtual void Message(SwampScript*, int, void*);
protected:
    virtual void SetState(int);

public:
    char field_BC[0x110 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class ShortcutHatchMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    ShortcutHatchMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Activate();
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ShortcutHatchMain, 0x110);

class LiftMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_ACTIVATE = 0,
        TIMER_RETURN = 1
    };
    virtual void TimerEvent(unsigned int timerId);
    virtual void HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr);
    virtual void FrameTick();
    virtual void SetState(int state);
private:
    bool IsVinceInTheWay();

public:
    SwampString _liftSound;            // 0x110
};
DECOMP_SIZE_ASSERT(LiftMain, 0x12C);

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
    static void Return(float);
};

// FUNCTION: USA 0x0009D79B
// PROTOTYPE: PREVIEW 0x00488815 identical
// NAME: proto-pdb:default.pdb
void ShortcutHatchMain::AnimationFinished()
{
    BasicDoorMain::AnimationFinished();
    AnimCam::Return(0.0f);
}

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    T Fetch();

    std::deque<T> _queue;              // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    void Load(const SwampString& name, const char* format, int start);
    ResourceHandle<AnimationResource> FetchAnim() { return _animQueue.Fetch(); }

    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

class SoundNode; // NAME: proto-pdb:default.pdb

class FlamePipesMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum FlameFlags { // NAME: proto-pdb:default.pdb
        FLAME_A = 1,
        FLAME_B = 2,
        FLAME_C = 4
    };
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
    virtual void Load(const BeepTDF& tdf);
    virtual void Message(SwampScript* source, int message, void* data);

    int _flameFlags;                   // 0xBC
    SoundNode* _sound;                 // 0xC0
    int _flamenum;                     // 0xC4
    SwampString _animname;             // 0xC8
    MultiAnimation _multiAnim;         // 0xE4
};
DECOMP_SIZE_ASSERT(FlamePipesMain, 0xF8);

// FUNCTION: USA 0x0009D7AD
// PROTOTYPE: PREVIEW 0x00488827 identical
// NAME: proto-pdb:default.pdb
void FlamePipesMain::Load(const BeepTDF& tdf)
{
    CritterMain::Load(tdf);
    _multiAnim.Load(_animname, "%s%003i", 1);
}

// FUNCTION: USA 0x0009DFA4
// PROTOTYPE: PREVIEW 0x00489080 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1FlamePipesMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_FlamePipesMain_dtor(FlamePipesMain* p)
{
    p->FlamePipesMain::~FlamePipesMain();
}

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> _animCarry; // 0xBC
    char field_C0[0x18];               // 0xC0
    bool _persistentFrameTick;         // 0xD8
    char field_D9[0x1F];               // 0xD9
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

class HeavyObjectMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    HeavyObjectMain(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(HeavyObjectMain, 0xF8);

class GasCanisterMain : public HeavyObjectMain { // NAME: proto-pdb:default.pdb
public:
    GasCanisterMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    bool _armed;                       // 0xF8
    bool field_F9;                     // 0xF9 retail only: zeroed by the retail ctor, absent in the proto PDB
    int _dropCount;                    // 0xFC
    int _flameCount;                   // 0x100
};
DECOMP_SIZE_ASSERT(GasCanisterMain, 0x104);

class QuickCanisterMain : public GasCanisterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript* source, int message, void* data);
};
DECOMP_SIZE_ASSERT(QuickCanisterMain, 0x104);

class FlameWallMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_ON = 0,
        STATE_OFF = 1
    };
    void SetState(int state);
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);

    int _state;                        // 0x4C
};
DECOMP_SIZE_ASSERT(FlameWallMain, 0x50);

class AirplaneMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void FrameTick();
protected:
    virtual void TimerEvent(unsigned int timerId);

public:
    char field_BC[0xD0 - 0xBC];        // 0xBC
    float _recoveryTimer;              // 0xD0
    char field_D4[0xEC - 0xD4];        // 0xD4
    bool _doDamage;                    // 0xEC
};
DECOMP_SIZE_ASSERT(AirplaneMain, 0xF0);

class Airplane_RocketLabMain : public AirplaneMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_DODAMAGE = 0
    };
    virtual void FrameTick();
    virtual void TimerEvent(unsigned int timerId);
};
DECOMP_SIZE_ASSERT(Airplane_RocketLabMain, 0xF0);

// FUNCTION: USA 0x0009D7F4
// PROTOTYPE: PREVIEW 0x0048886E identical
// NAME: proto-pdb:default.pdb
void Airplane_RocketLabMain::TimerEvent(unsigned int id)
{
    switch (id) {
    case TIMER_DODAMAGE:
        _doDamage = true;
        break;
    default:
        AirplaneMain::TimerEvent(id);
        break;
    }
}

// FUNCTION: USA 0x0009D80A
// PROTOTYPE: PREVIEW 0x00488884 identical
// NAME: proto-pdb:default.pdb
void Airplane_RocketLabMain::FrameTick()
{
    _recoveryTimer = 0.0f;
    AirplaneMain::FrameTick();
}

// FUNCTION: USA 0x0009D7D6
// PROTOTYPE: PREVIEW 0x00488850 identical
// NAME: proto-pdb:default.pdb
void FlameWallMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_ON:
        RegisterFrameTick();
        break;
    case STATE_OFF:
        UnRegisterFrameTick();
        break;
    }
}

class AnimationControl; // NAME: proto-pdb:default.pdb
class AABox;            // NAME: proto-pdb:default.pdb
struct Matrix;          // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float Length() const;

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                         // vtable+0x00
    virtual const AABox& GetBounds() const;            // vtable+0x04
    virtual Vector3 GetWorldPosition() const;          // vtable+0x08
    virtual Quaternion GetWorldRotation() const;       // vtable+0x0C
    virtual void SetWorldPosition(const Vector3& pos); // vtable+0x10
    virtual void Invalidate();                         // vtable+0x14

    SwampScript* GetScript() const;
    void SetOption(unsigned int, bool, bool);
    const Quaternion& GetRotation() const { return _Rotation; }

    char field_4[0x40 - 0x4];          // 0x04
    Quaternion _Rotation;              // 0x40
    char field_50[0xA4 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode : SwampSceneNode, all at 0x0; CollidableNode not modelled
class CollidableCylinder : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    float GetHeight() const { return _height; }
    float GetRadius() const { return _radius; }

    char field_A4[0xAC - 0xA4];        // 0xA4
    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    Matrix GetBoneOrientation(const SwampString&) const;
};
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};
class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

inline Critter* CritterMain::GetCritter() { return (Critter*)_node; }

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

struct Vector4 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

class BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    const Vector4& ImpactPlane() const { return _plane; }

    char field_0[0x10];                // 0x00
    Vector4 _plane;                    // 0x10
    char field_20[0x40 - 0x20];        // 0x20
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    const T* operator->() const { return _Object; }

    T* _Object;                        // 0x00
};

// FUNCTION: USA 0x0009D817
// PROTOTYPE: PREVIEW 0x00488891 identical
// NAME: proto-pdb:default.pdb
void LiftMain::HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr)
{
    if (critterHitBy == pManager->_vinceCache && cr->ImpactPlane().y > 0.0f && _state == STATE_CLOSED)
        RegisterTimerEvent(TIMER_ACTIVATE, 0.2f, false);
}

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static SoundNode* CreateSound(const SwampString&, unsigned int, SwampSceneNode*, bool);
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

// FUNCTION: USA 0x0009D857
// PROTOTYPE: PREVIEW 0x004888D1 identical
// NAME: proto-pdb:default.pdb
void LiftMain::SetState(int state)
{
    BasicDoorMain::SetState(state);
    switch (state) {
    case STATE_OPEN:
        RegisterTimerEvent(TIMER_RETURN, 1.0f, false);
        break;
    case STATE_OPENING:
    case STATE_CLOSING:
        SoundManager::PlaySound(_liftSound, 0x20, GetNode(), true, 0.0f, 0.0f); // retail constant
        break;
    }
}

// FUNCTION: USA 0x0009D8FA
// PROTOTYPE: PREVIEW 0x00488974 identical
// NAME: proto-pdb:default.pdb
void FlameWallMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    if (value > prev)
        SetState(STATE_ON);
    else
        SetState(STATE_OFF);
}

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0009D976
// PROTOTYPE: PREVIEW 0x00488A71 identical
// NAME: proto-pdb:default.pdb
void FlamePipesMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    int v1;
    if (name == "flamea")
        v1 = FLAME_A;
    else if (name == "flameb")
        v1 = FLAME_B;
    else if (name == "flamec")
        v1 = FLAME_C;
    else
        return;
    if (value > prev) {
        SwampString bone;
        if (!_flameFlags)
            RegisterFrameTick();
        _flameFlags |= v1;
        SwampString name = format("rrl_flame%02d", _flamenum + 1);
        _flamenum++;
        if (_flamenum == 5)
            _flamenum = 0;
        if (_sound)
            _sound->Invalidate();
        _sound = SoundManager::CreateSound(name, 0xD, GetNode(), true); // retail constant
    } else {
        _flameFlags &= ~v1;
        if (!_flameFlags) {
            if (_sound) {
                _sound->Invalidate();
                _sound = 0;
            }
            UnRegisterFrameTick();
        }
    }
}

// FUNCTION: USA 0x0009DAB0
// NAME: proto-pdb:default.pdb
void RocketLabRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (!_inRegion) {
        LevelScript::EnteringRegion(node);
        CutsceneDriver::SetStream("ethelRocketLab", false);
    }
}

// FUNCTION: USA 0x0009DAEE
// PROTOTYPE: PREVIEW 0x00488BE9 identical
// NAME: proto-pdb:default.pdb
void ShortcutHatchMain::Message(SwampScript* source, int message, void* data)
{
    if (message == 6) {
        // !FAKE: c1xx front end; dummy local; retail destroys the name temp before GetScript (one expression 67.6)
        Critter* tmp = pManager->Find("switch_shortcutHatch");
        tmp->GetScript()->Message(this, 0x16, 0);
    }
    BasicDoorMain::Message(source, message, data);
}

// FUNCTION: USA 0x0009DB51
// PROTOTYPE: PREVIEW 0x00488C4C identical
// NAME: proto-pdb:default.pdb
void ShortcutHatchMain::Activate()
{
    BasicDoorMain::Activate();
    AnimCam::SwitchTo("camera_shortcuthatch", 0.0f, false, 0.0f);
}

// FUNCTION: USA 0x0009DB91
// PROTOTYPE: PREVIEW 0x00488C8C identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 65.65, bytes mismatch; wall: inline-asm sqrt, D116 (C0219) (retail x-z length rsqrtss/fcompi/fcmovbe/ffree)
bool LiftMain::IsVinceInTheWay()
{
    // !FAKE: c2 register allocation; dummy local; retail holds Vince in ebx from the first load
    Vince* p = pManager->_vinceCache;
    Matrix m = GetCritter()->GetBoneOrientation("root");
    // !FAKE: c2 register allocation; dummy local; retail spills the node pointer across Apply
    Critter* tmp = GetCritter();
    // !FAKE: inliner; temporary; hand form of the inline Quaternion::Apply(const Vector3&) retail inlines
    Vector3 v3;
    tmp->GetRotation().Apply(Vector3(m.row[3].x, m.row[3].y, m.row[3].z), v3);
    Vector3 tovince = v3;
    Vector3 liftpos;
    {
        // !FAKE: c1xx front end; temporary; GetWorldPosition result held for the per-component sum
        Vector3 v1 = tmp->GetWorldPosition();
        liftpos.x = tovince.x + v1.x;
        liftpos.y = tovince.y + v1.y;
        liftpos.z = tovince.z + v1.z;
    }
    {
        // !FAKE: c1xx front end; temporary; Vince position held for the per-component difference
        Vector3 v1 = p->GetWorldPosition();
        tovince.x = v1.x - liftpos.x;
        tovince.y = v1.y - liftpos.y;
        tovince.z = v1.z - liftpos.z;
    }
    if (tovince.y < p->GetHeight() && tovince.y > -p->GetHeight()) {
        // !FAKE: c1xx front end; temporary; x-z length stand-in for retail's inline-asm length
        Vector3 v2(tovince.x, 0.0f, tovince.z);
        if (v2.Length() < GetCritter()->GetRadius() + p->GetRadius())
            return true;
    }
    return false;
}
#endif

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 3 = base ctor call, vptr store, literal)
// FUNCTION: USA 0x0009DCC3
// PROTOTYPE: PREVIEW 0x00488DBE near-identical
// NAME: proto-pdb:default.pdb
GasCanisterMain::GasCanisterMain(SwampSceneNode* node)
    : HeavyObjectMain(node), _armed(false), field_F9(false), _dropCount(0), _flameCount(0)
{
    _persistentFrameTick = true;
    _animCarry = g_swamp->GetAnimationManager()->Load("gasCanister_walk");
}
#endif

// FUNCTION: USA 0x0009E5FA
// PROTOTYPE: PREVIEW 0x004896D5 identical
// NAME: proto-pdb:default.pdb
SwampScript* GasCanisterMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\rocketlab\\RocketLab.h", 65) GasCanisterMain(node);
}

// FUNCTION: USA 0x0009E648
// PROTOTYPE: PREVIEW 0x00489723 identical
// NAME: proto-pdb:default.pdb
void LiftMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_ACTIVATE:
        Activate();
        break;
    case TIMER_RETURN:
        if (_state == STATE_OPEN) {
            if (IsVinceInTheWay()) {
                RegisterTimerEvent(TIMER_RETURN, 1.0f, false);
            } else {
                SetState(STATE_CLOSING);
            }
        }
        break;
    }
}

// FUNCTION: USA 0x0009E68F
// PROTOTYPE: PREVIEW 0x0048976A identical
// NAME: proto-pdb:default.pdb
void LiftMain::FrameTick()
{
    if (_state == STATE_CLOSING && IsVinceInTheWay())
        SetState(STATE_OPENING);
}

// FUNCTION: USA 0x0009EA26
// PROTOTYPE: PREVIEW 0x00489AE2 identical
// NAME: proto-pdb:default.pdb
void FlamePipesMain::Message(SwampScript* source, int message, void* data)
{
    if (message != 6)
        CritterMain::Message(source, message, data);
    else
        GetCritter()->StartAnimation(_multiAnim.FetchAnim(), true, false);
}

// FUNCTION: USA 0x0009ECAD
// NAME: proto-pdb:default.pdb
SwampScript* RocketLabRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\rocketlab\\RocketLab.h", 18) RocketLabRegionMain(node);
}
// FUNCTION: USA 0x0009ECD1
// PROTOTYPE: PREVIEW 0x00489D8D identical
// NAME: proto-pdb:default.pdb
SwampScript* ShortcutHatchMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("RocketLab\\RocketLab.cpp", 41) ShortcutHatchMain(node);
}

class Level { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() { return _script; }

    char field_0[0xC];                 // 0x00
    SwampScript* _script;              // 0x0C
    char field_10[0x140 - 0x10];       // 0x10
};
DECOMP_SIZE_ASSERT(Level, 0x140);

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }

    char field_0[0x4];                 // 0x00
    Level* _curLevel;                  // 0x04
    char field_8[0x1CC - 0x8];         // 0x08
};
DECOMP_SIZE_ASSERT(LevelLoader, 0x1CC);

// FUNCTION: USA 0x0009EF44
// PROTOTYPE: PREVIEW 0x0048A000 identical
// NAME: proto-pdb:default.pdb
void QuickCanisterMain::Message(SwampScript* source, int message, void* data)
{
    GasCanisterMain::Message(source, message, data);
    if (message == 6) {
        bool solved;
        LevelLoader::Instance().GetCurLevel()->GetScript()->Message(this, 0x1B, &solved);
        if (!solved)
            _node->SetOption(2, true, false);
    }
}
