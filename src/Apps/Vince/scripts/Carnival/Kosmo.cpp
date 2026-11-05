// UNIT: src/Apps/Vince/scripts/Carnival/Kosmo.cpp OBJ: Xbox\OfficialRelease\Kosmo.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const E* _Ptr);
    int compare(const E* _Ptr) const;

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class E, class T, class A> inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return (_Left.compare(_Right) == 0);
}
template <class T, class A = allocator<T> > class vector {
public:
    ~vector() { _Tidy(); }
    class iterator {
    public:
        iterator(T* _Ptr) : current(_Ptr) {}
        T& operator*() const { return (*current); }
        iterator& operator+=(int _Off)
        {
            current += _Off;
            return (*this);
        }
        iterator operator+(int _Off) const
        {
            iterator _Tmp = *this;
            return (_Tmp += _Off);
        }
        T* current;
    };
    iterator begin() { return (iterator(_Myfirst)); }
    unsigned int size() const;
    T& operator[](unsigned int _Pos) { return (*(begin() + _Pos)); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class T, class A = allocator<T> > class deque {
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    float Length() const;
    Vector3& operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    float x;                         // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

inline Vector3 operator+(const Vector3& a, const Vector3& b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(const Vector3& v) { Set(v); }
    void Set(const Vector3&);
    void Apply(const Vector3&, Vector3&) const;
    Quaternion& operator*=(const Quaternion&);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct AABox; // NAME: proto-pdb:default.pdb

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    T Fetch();

    std::deque<T> _queue;              // 0x00
};

class SwampScript;           // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C

    SwampScript* GetScript() const;
    void SetScript(const SwampString&);
    unsigned int GetZone() const;

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line);

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
    SwampSceneNode* GetNode() { return _node; }

protected:
    SwampScript(SwampSceneNode* node);

public:
    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class Critter; // NAME: proto-pdb:default.pdb

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)_node; }
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class KosmoBotLegsMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_TRANSPORTVINCE = 0x15,
        MESSAGE_FINISHED_ELEVATORCAM = 0x17
    };
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);

    char field_BC[0x130 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(KosmoBotLegsMain, 0x130);

// FUNCTION: USA 0x00049BD0
// PROTOTYPE: PREVIEW 0x004374D7 identical
// NAME: proto-pdb:default.pdb
void KosmoBotLegsMain::CollidedWith(CollisionType type, SwampSceneNode* nodehitby,
                                    const Ptr<CollisionResultObject>& cr)
{
    HitBy(type, nodehitby, cr);
}

class Camera_elevatorMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Camera_elevatorMain(SwampSceneNode* node);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Camera_elevatorMain, 0x4C);

// FUNCTION: USA 0x00049BED
// PROTOTYPE: PREVIEW 0x004374F4 identical
// NAME: proto-pdb:default.pdb
SwampScript* Camera_elevatorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BumperCarBumpoff\\Kosmo.cpp", 18) Camera_elevatorMain(node);
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationTime() const { return _time; }

    char field_24[0x38 - 0x24];        // 0x24
    float _time;                       // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }
    ResourceHandle& operator=(const ResourceHandle& h);
    bool IsValid() const { return _Resource != 0; }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode : SwampSceneNode, all at 0x0; only
// SwampSceneNode modelled
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    ResourceHandle<AnimationControl> TransitionAnimation(ResourceHandle<AnimationResource>, float, bool, bool);
    ResourceHandle<AnimationControl> GetCurrentAnimation() const { return _currentAnim; }
    void GetBoneOrientation(int, Quaternion&, Vector3&) const;

    char field_A4[0x140 - 0xA4];       // 0xA4
    ResourceHandle<AnimationControl> _currentAnim; // 0x140
    char field_144[0x200 - 0x144];     // 0x144
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void StopSounds();
    bool IsStunned() const;
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    Critter* GetVehicle() const;
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    Critter* Create(const SwampString&, const SwampString&, const char*, ResourceHandle<ModelResource>, const Vector3&,
                    const Quaternion&, unsigned int);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0004C9D4
// NAME: proto-pdb:default.pdb
class KosmoBotHeadMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_STARTPATROL = 0x14
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_PATROL = 3,
        STATE_ATTACK = 4
    };
    virtual void FrameTick();
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
    virtual void SetState(int);

    std::vector<int> _cannonJoint;     // 0xBC
    int _curCannon;                    // 0xCC
    bool _mouthOpen;                   // 0xD0
    int _targetJoint;                  // 0xD4
    bool _didCutScene;                 // 0xD8
    ResourceHandle<AnimationResource> _animIdle;   // 0xDC
    ResourceHandle<AnimationResource> _animAttack; // 0xE0
    ResourceHandle<AnimationResource> _animDamage; // 0xE4
    ResourceHandle<AnimationResource> _animDeath;  // 0xE8
};
DECOMP_SIZE_ASSERT(KosmoBotHeadMain, 0xEC);

// FUNCTION: USA 0x00049CEA
// PROTOTYPE: PREVIEW 0x00437601 identical
// NAME: proto-pdb:default.pdb
void KosmoBotHeadMain::FrameTick()
{
    if (_state == STATE_ATTACK && !pManager->GetVince()->GetVehicle())
        SetState(STATE_PATROL);
}

// FUNCTION: USA 0x00049E7F
// PROTOTYPE: PREVIEW 0x00437626 identical
// NAME: proto-pdb:default.pdb
void Camera_elevatorMain::AnimationFinished()
{
    // !FAKE: c1xx front end; stand-in local; ends the string temporary before GetScript (BumperTargetMain form)
    Critter* tmp = pManager->Find("kosmobotlegs");
    tmp->GetScript()->Message(this, KosmoBotLegsMain::MESSAGE_FINISHED_ELEVATORCAM, 0);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1KosmoBotHeadMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_KosmoBotHeadMain_dtor(KosmoBotHeadMain* p)
{
    p->KosmoBotHeadMain::~KosmoBotHeadMain();
}

// FUNCTION: USA 0x0004D2DD
// NAME: proto-pdb:default.pdb
class KosmoBotMidriffMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_VINCEONBOT = 0x14,
        MESSAGE_VINCEOFFBOT = 0x15,
        MESSAGE_FINISHED_INTRO = 0x16
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_STILL = 0,
        STATE_IDLE = 1,
        STATE_ATTACK = 2,
        STATE_DEAD = 4
    };
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
    virtual void Message(SwampScript* from, int message, void* data);
    virtual void SetState(int state);

    std::vector<Vector3> _cannonLocs;  // 0xBC
    SwampShuffleQueue<SwampString> _handSmackSounds; // 0xCC
};
DECOMP_SIZE_ASSERT(KosmoBotMidriffMain, 0xE0);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1KosmoBotMidriffMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_KosmoBotMidriffMain_dtor(KosmoBotMidriffMain* p)
{
    p->KosmoBotMidriffMain::~KosmoBotMidriffMain();
}

// visible body (C0136): emits ??4?$ResourceHandle@VAnimationControl@@@@ in Kosmo.obj, byte-identical to the matched
// Generator.obj row, which it rides in relink (T3 change, D117); KosmoBotMidriffMain::SetState needs it (88.56 without)
template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_GAMESTART = 0x6
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    AnimationManager& GetAnimationManager() const { return *_animationManager; }
    ModelManager& GetModelManager() const { return *_modelManager; }
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x28 - 0x1C];        // 0x1C
    ModelManager* _modelManager;       // 0x28
    char field_2C[0x68 - 0x2C];        // 0x2C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0004A37F
// NAME: proto-pdb:default.pdb
void KosmoBotMidriffMain::Message(SwampScript* from, int message, void* data)
{
    switch (message) {
    case MESSAGE_GAMESTART:
        if (_state == STATE_DEAD)
            SetState(STATE_DEAD);
        break;
    case MESSAGE_VINCEONBOT:
        if (_state != STATE_ATTACK)
            SetState(STATE_ATTACK);
        break;
    case MESSAGE_VINCEOFFBOT: {
        // !FAKE: c1xx front end; stand-in local; ends the string temporary before GetScript (BumperTargetMain form)
        Critter* tmp = pManager->Find("kosmobothead");
        tmp->GetScript()->Message(this, KosmoBotHeadMain::MESSAGE_STARTPATROL, 0);
        break;
    }
    case MESSAGE_FINISHED_INTRO:
        ((GatorNode*)GetNode())->StartAnimation(g_swamp->GetAnimationManager().Load("kosmobotmidriff_legsattack001"),
                                                true, false);
        break;
    }
    CritterMain::Message(from, message, data);
}

// FUNCTION: USA 0x0004A45F
// PROTOTYPE: PREVIEW 0x00437F89 identical
// NAME: proto-pdb:default.pdb
// needs the visible operator= above (C0215, D117)
void KosmoBotMidriffMain::SetState(int state)
{
    switch (state) {
    case STATE_STILL:
        GetCritter()->StartAnimation(ResourceHandle<AnimationResource>(), false, false);
        break;
    case STATE_IDLE: {
        GetCritter()->StopSounds();
        float animtime = 0.0f;
        ResourceHandle<AnimationControl> control = GetCritter()->GetCurrentAnimation();
        if (control.IsValid())
            animtime = control().GetAnimationTime();
        GetCritter()->TransitionAnimation(
            g_swamp->GetAnimationManager().Load("kosmobotmidriff_ambientmidriffidle001"), 0.25f, true, false);
        control = GetCritter()->GetCurrentAnimation();
        control().SetAnimationTime(animtime);
        break;
    }
    case STATE_ATTACK: {
        GetCritter()->StopSounds();
        float animtime = 0.0f;
        ResourceHandle<AnimationControl> control = GetCritter()->GetCurrentAnimation();
        if (control.IsValid())
            animtime = control().GetAnimationTime();
        GetCritter()->TransitionAnimation(
            g_swamp->GetAnimationManager().Load("kosmobotmidriff_armsattack001"), 0.25f, true, false);
        control = GetCritter()->GetCurrentAnimation();
        control().SetAnimationTime(animtime);
        break;
    }
    }
    _state = state;
}

class GameCam { // NAME: proto-pdb:default.pdb
public:
    void SetShakeFactor(float, float);
};

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void ResetLocation(bool);
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;

    char field_0[0xC];                 // 0x00
    SwampString _curCamName;           // 0x0C
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void Enable(bool enable) { _enabled = enable; }

    char field_A4[0x120 - 0xA4];       // 0xA4
    bool _enabled;                     // 0x120
};
DECOMP_SIZE_ASSERT(RegionTrigger, 0x124);

class AssDoorRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    AssDoorRegionMain(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(AssDoorRegionMain, 0x4C);

// FUNCTION: USA 0x00049C11
// NAME: proto-pdb:default.pdb
AssDoorRegionMain::AssDoorRegionMain(SwampSceneNode* node)
    : SwampScript(node)
{
    ((RegionTrigger*)node)->Enable(false);
}

// FUNCTION: USA 0x0004A615
// NAME: proto-pdb:default.pdb
void AssDoorRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (node == pManager->GetVince()) {
        // !FAKE: c1xx front end; stand-in locals; end each string temporary before the next call (BumperTargetMain form)
        Critter* tmp = pManager->Find("kosmobotlegs");
        tmp->GetScript()->Message(this, KosmoBotLegsMain::MESSAGE_TRANSPORTVINCE, 0);
        GameCam* p = g_CameraManager->Find("vincecam");
        ((VinceCam*)p)->ResetLocation(false);
    }
}

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
    static void Return(float);
};

// FUNCTION: USA 0x0004AE71
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.54, bytes mismatch (instructions equal; frame slots differ: a string temp and a GetWorldRotation temp sit above pos, retail packs them below rot; C0225 open slot order)
void KosmoBotHeadMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    if (name == "launchmissile") {
        if (value > 0.0f && pManager->GetVince()->GetVehicle()) {
            SwampString soundname = format("kbm_fire%02d", _curCannon + 1);
            Vector3 pos;
            Quaternion rot;
            // !FAKE: inliner; hand form of the by-value Apply (C0225); one result slot for both uses
            Vector3 tmp;
            ((GatorNode*)GetNode())->GetBoneOrientation(_cannonJoint[_curCannon++], rot, pos);
            if (_curCannon >= _cannonJoint.size()) {
                _curCannon = 0;
            }
            {
                // !FAKE: c2 register allocation; stand-in local; retail loads the node once for both calls
                SwampSceneNode* p = GetNode();
                Vector3 v0;
                const Quaternion* v6 = &p->GetWorldRotation();
                v6->Apply(pos, tmp);
                v0 = tmp;
                pos = v0 + p->GetWorldPosition();
            }
            {
                // !FAKE: inliner; hand form of the inline Quaternion operator* (C0221)
                const Quaternion* v12 = &GetNode()->GetWorldRotation();
                Quaternion v1(*v12);
                v1 *= rot;
                rot = v1;
            }
            {
                Vector3 v2;
                rot.Apply(Vector3(0.0f, 0.0f, 1.0f), tmp);
                v2 = tmp;
                Quaternion v5(v2);
                // !FAKE: c2 register allocation; stand-in locals assigned in the call order retail loads them (C0221)
                CritterManager* v6;
                SwampSceneNode* v10;
                ModelManager* v11;
                // !FAKE: c1xx front end; stand-in local; ends the string temporaries before PlaySound
                Critter* v3 = (v6 = pManager)->Create("clownmissile", "clownmissile", "clownmissile",
                                                      (v11 = &g_swamp->GetModelManager())->Load("clownmissile_base"),
                                                      pos, v5, (v10 = GetNode())->GetZone());
                SoundManager::PlaySound(soundname, 0, v3, false, 0.0f, 0.0f);
            }
        }
    } else if (name == "openmouth") {
        _mouthOpen = value > 0.0f ? true : false;
    } else if (name == "hatch") {
        if (value > 0.0f) {
            AnimCam::Return(0.0f);
            AnimCam::SwitchTo("camera_exithatch", 0.0f, false, 0.0f);
            SoundManager::PlaySound("kfa_foreheadhatch", 0x20, GetNode(), true, 0.0f, 0.0f);
        } else {
            AnimCam::Return(0.0f);
        }
    }
}
#endif

class DamageSourceInfo { // NAME: proto-pdb:default.pdb
public:
    DamageSourceInfo();
    ~DamageSourceInfo();

    SwampString sourceType;            // 0x00
    SwampString attackType;            // 0x1C
    float _multiplier;                 // 0x38
    bool _canBlock;                    // 0x3C
};
DECOMP_SIZE_ASSERT(DamageSourceInfo, 0x40);

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_DAMAGE = 0x0
};

class VinceFlat { // NAME: proto-pdb:default.pdb
public:
    enum Message { // NAME: proto-pdb:default.pdb
        MESSAGE_POPUP = 0x14
    };
};

// FUNCTION: USA 0x0004D2FE
// PROTOTYPE: PREVIEW 0x0043A521 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 61.26, bytes mismatch; wall-d45 (C0219, D116): retail's three Vector3 lengths are inline-asm sqrt (rsqrtss + fcompi/fcmovbe/ffree)
void KosmoBotMidriffMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    if (name == "trigger_smack" && value > 0.0f) {
        SoundManager::PlaySound(_handSmackSounds.Fetch(), 0x20, GetNode(), true, 0.0f, 0.0f);
        Vector3 pos;
        Quaternion rot;
        ((GatorNode*)GetNode())->GetBoneOrientation(bone, rot, pos);
        Vector3 diff;
        GetNode()->GetWorldRotation().Apply(pos, diff);
        pos = diff;
        pos += GetNode()->GetWorldPosition();
        Vince* vince = pManager->GetVince();
        if (!vince->GetVehicle()) {
            Vector3 vincepos = vince->GetWorldPosition();
            // !FAKE: c2 register allocation; stand-in local; retail keeps the distance on the x87 stack
            float v0 = (pos - vincepos).Length();
            if (v0 < 12.0f)
                g_CameraManager->Find(g_CameraManager->_curCamName)->SetShakeFactor(2.0f - v0 / 6.0f, -1.0f);
            diff = vincepos - pos;
            if (diff.y <= 0.0f && diff.y >= -5.0f) {
                diff.y = 0.0f;
                if (diff.Length() < 2.0f && !vince->IsStunned()) {
                    diff = vincepos - pos;
                    diff.y = 0.0f;
                    if (diff.Length() < 3.0f) {
                        DamageSourceInfo dsi;
                        dsi.attackType = "bump";
                        dsi.sourceType = "kosmobotfist";
                        vince->GetScript()->Message(this, MESSAGE_DAMAGE, &dsi);
                        vince->SetScript("VinceFlat");
                        vince->GetScript()->Message(this, VinceFlat::MESSAGE_POPUP, (void*)3);
                    }
                }
            }
        }
    }
}
#endif
