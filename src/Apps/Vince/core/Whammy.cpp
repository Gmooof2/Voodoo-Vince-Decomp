// UNIT: src/Apps/Vince/core/Whammy.cpp OBJ: core\XBox\OfficialRelease\Whammy.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);

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
struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    void RotateJ(float);
    float Length() const;

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

inline Vector3 operator+(const Vector3& a, const Vector3& b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vector3 operator*(float f, const Vector3& v) { return Vector3(v.x * f, v.y * f, v.z * f); }

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
}
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2
    };
    void SetOption(unsigned int option, bool value, bool recursive);
    SwampScript* GetScript() const;
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

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
    float GetHeight() const { return _height; }

    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void SetEdgeGlow(bool edgeGlow) { _edgeGlow = edgeGlow; }

    char field_B4[0xC5 - 0xB4];        // 0xB4 (_model .. _dirtyAnimation; not reached)
    bool _edgeGlow;                    // 0xC5
    char field_C6[0x200 - 0xC6];       // 0xC6 (not reached)
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void MakeDead(bool);

    char field_200[0x2C8 - 0x200];     // 0x200 (not reached)
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }
    Critter* Find(const SwampString&);

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x70];                // 0x00
    float _whammyProjectileSpeed;      // 0x70
};
extern Globals gSettings; // NAME: proto-pdb:default.pdb

class GameCam { // NAME: proto-pdb:default.pdb
public:
    void SetShakeFactor(float, float);
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};
extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

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

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    SwampScript* _parent;              // 0x24
    char field_28[0x24];               // 0x28 (_frameTicks .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)_node; }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class WaitForWhammyProjectileMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    WaitForWhammyProjectileMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_4C[0x5C - 0x4C];        // 0x4C (not reached)
};
DECOMP_SIZE_ASSERT(WaitForWhammyProjectileMain, 0x5C);

class WhammyProjectileMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SET_TARGET = 0x14,
        MESSAGE_SET_RESPONSE_SCRIPT = 0x15
    };
    WhammyProjectileMain(SwampSceneNode* node);
    virtual void Message(SwampScript*, int, void*);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void InitCurve();
    void UpdateCoefficients();

    char field_4C[0x50 - 0x4C];        // 0x4C (_effect; not reached)
    Vector3 _p1;                       // 0x50
    Vector3 _p2;                       // 0x5C
    Vector3 _r1;                       // 0x68
    Vector3 _r2;                       // 0x74
    Vector3 _coeff[4];                 // 0x80
    float _age;                        // 0xB0
    float _ageMax;                     // 0xB4
    SwampString _target;               // 0xB8
    SwampString _scriptName;           // 0xD4
    char field_F0[0xF4 - 0xF0];        // 0xF0 (_soundNode; not reached)
};
DECOMP_SIZE_ASSERT(WhammyProjectileMain, 0xF4);

class WhammyResultScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void KillMonsterNow();                                 // vtable+0x64

    ResourceHandle<ModelResource> _modelRestore; // 0x4C
};
DECOMP_SIZE_ASSERT(WhammyResultScript, 0x50);

class WhammyResultFlattenMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultFlattenMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x54 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultFlattenMain, 0x54);

class WhammyResultFrozenMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_FINISH = 0x0
    };
    WhammyResultFrozenMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _animFrozen;  // 0x50
    ResourceHandle<AnimationResource> _animRecover; // 0x54
    bool _finished;                    // 0x58
};
DECOMP_SIZE_ASSERT(WhammyResultFrozenMain, 0x5C);

class WhammyResultCutHorizontalMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultCutHorizontalMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultCutHorizontalMain, 0x58);

class WhammyResultCutVerticalMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultCutVerticalMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultCutVerticalMain, 0x58);

class WhammyResultCutMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultCutMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultCutMain, 0x58);

class WhammyResultDisintegrateMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultDisintegrateMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultDisintegrateMain, 0x58);

class WhammyResultPokeMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultPokeMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x54 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultPokeMain, 0x54);

class WhammyResultJackhammerMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultJackhammerMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x54 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultJackhammerMain, 0x54);

class WhammyResultToxicMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultToxicMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x54 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultToxicMain, 0x54);

class WhammyResultBurstMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultBurstMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultBurstMain, 0x58);

class VinceWhammyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SET_WHAMMY_NAME = 0x14,
        MESSAGE_KILL_WHAMMY = 0x15
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_WHAMMY_ANIM = 0x0,
        STATE_WHAMMY_REACT_ANIM = 0x1
    };
    VinceWhammyMain(SwampSceneNode* node);
    virtual void InvalidateScript();
    virtual void SetState(int);
    virtual void Message(SwampScript*, int, void*);
    virtual void CreateProp();                                     // vtable+0x70
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    Critter* _prop;                    // 0xBC
    ResourceHandle<AnimationResource> _animPowerUp; // 0xC0
    SwampString _whammyName;           // 0xC4
};
DECOMP_SIZE_ASSERT(VinceWhammyMain, 0xE0);

// src proto-pdb:default.pdb VinceMain base VinceShared, not modelled; only its Messages enum is reached
class VinceMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_WHAMMY_CLEANUP = 0x1A
    };
};

class WhammyPropMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    WhammyPropMain(SwampSceneNode* node);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    bool _restoreVinceModel;           // 0x4C
    ResourceHandle<ModelResource> _modelVince; // 0x50
    float _cameraShake;                // 0x54
    SwampString _cameraName;           // 0x58
    float _radius;                     // 0x74
    char field_78[0x7C - 0x78];        // 0x78 (_effect; not reached)
    SwampString _scriptName;           // 0x7C
    bool _finished;                    // 0x98
};
DECOMP_SIZE_ASSERT(WhammyPropMain, 0x9C);

// FUNCTION: USA 0x0013392E
// PROTOTYPE: PREVIEW 0x0051DB51 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 67.72, bytes mismatch; wall: retail's totarget length is inline-asm sqrt
// (rsqrtss + fcompi/fcmovbe/ffree), D116 forbids __asm (C0219)
void WhammyProjectileMain::InitCurve()
{
    Critter* critter = pManager->Find(_target);
    if (critter) {
        Vince* vince = pManager->GetVince();
        _p1 = vince->GetWorldPosition();
        _p1.y += vince->GetHeight() * 0.5f;
        _p2 = critter->GetWorldPosition();
        _p2.y += critter->GetHeight() * 0.5f;
        Vector3 totarget = _p2 - _p1;
        float length = totarget.Length();
        _r1 = 4.0f * totarget;
        _r1.RotateJ(BeepLib::RandomFloat(-1.03672564f, 1.03672564f));
        _r2 = _r1;
        _r1.y += length;
        _r2.y -= length;
        _ageMax = length / gSettings._whammyProjectileSpeed;
    }
}
#endif

// FUNCTION: USA 0x00133A87
// PROTOTYPE: PREVIEW 0x0051DCAA identical
// NAME: proto-pdb:default.pdb
void WhammyProjectileMain::UpdateCoefficients()
{
    Critter* critter = pManager->Find(_target);
    _p2 = critter->GetWorldPosition();
    _p2.y += critter->GetHeight() * 0.5f;
    _coeff[0].x = _p1.x;
    _coeff[0].y = _p1.y;
    _coeff[0].z = _p1.z;
    _coeff[1].x = _r1.x;
    _coeff[1].y = _r1.y;
    _coeff[1].z = _r1.z;
    _coeff[2].x = 3.0f * _p2.x - 3.0f * _p1.x - 2.0f * _r1.x - _r2.x;
    _coeff[2].y = 3.0f * _p2.y - 3.0f * _p1.y - 2.0f * _r1.y - _r2.y;
    _coeff[2].z = 3.0f * _p2.z - 3.0f * _p1.z - 2.0f * _r1.z - _r2.z;
    _coeff[3].x = 2.0f * _p1.x - 2.0f * _p2.x + _r1.x + _r2.x;
    _coeff[3].y = 2.0f * _p1.y - 2.0f * _p2.y + _r1.y + _r2.y;
    _coeff[3].z = 2.0f * _p1.z - 2.0f * _p2.z + _r1.z + _r2.z;
}

// FUNCTION: USA 0x00133D00
// PROTOTYPE: PREVIEW 0x0051DF23 identical
// NAME: proto-pdb:default.pdb
void VinceWhammyMain::SetState(int state)
{
    _state = state;
    if (state == STATE_WHAMMY_ANIM)
        GetCritter()->StartAnimation(_animPowerUp, true, false);
}

// FUNCTION: USA 0x00133D20
// PROTOTYPE: PREVIEW 0x0051DF43 identical
// NAME: proto-pdb:default.pdb
void WhammyResultScript::KillMonsterNow()
{
    // !FAKE: stand-in local (no PDB local); retail keeps _node in edi across both virtual calls
    Critter* critter = (Critter*)_node;
    if (_modelRestore.IsValid())
        critter->SetModel(_modelRestore);
    _parent->Message(this, 0x14, 0);
    critter->SetEdgeGlow(false);
    critter->MakeDead(true);
}

// FUNCTION: USA 0x00133D57
// PROTOTYPE: PREVIEW 0x0051DF7A identical
// NAME: proto-pdb:default.pdb
void WhammyResultFrozenMain::TimerEvent(unsigned int timerId)
{
    if (timerId == TIMER_FINISH) {
        // !FAKE: stand-in local (no PDB local); retail loads _node once for both uses
        Critter* critter = (Critter*)_node;
        critter->SetEdgeGlow(false);
        if (_animRecover.IsValid())
            critter->StartAnimation(_animRecover, false, false);
        else
            AnimationFinished();
    }
}

// FUNCTION: USA 0x00133D85
// PROTOTYPE: PREVIEW 0x0051DFA8 identical
// NAME: proto-pdb:default.pdb
void WhammyResultFrozenMain::AnimationFinished()
{
    // !FAKE: stand-in local (no PDB local); retail loads _parent before the InvalidateScript call
    SwampScript* parent = _parent;
    InvalidateScript();
    parent->Message(0, 10, 0);
}

// FUNCTION: USA 0x00133DC5
// PROTOTYPE: PREVIEW 0x0051DFE8 identical
// NAME: proto-pdb:default.pdb
SwampScript* WaitForWhammyProjectileMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("core\\Whammy.cpp", 256) WaitForWhammyProjectileMain(node);
}

// FUNCTION: USA 0x00133F16
// PROTOTYPE: PREVIEW 0x0051E139 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyProjectileMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("core\\Whammy.cpp", 31) WhammyProjectileMain(node);
}

// FUNCTION: USA 0x00133F59
// PROTOTYPE: PREVIEW 0x0051E17C identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~WhammyProjectileMain in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WhammyProjectileMain_dtor(WhammyProjectileMain* p)
{
    p->WhammyProjectileMain::~WhammyProjectileMain();
}

// FUNCTION: USA 0x00134207
// NAME: proto-pdb:default.pdb
void WhammyPropMain::AnimationFinished()
{
    if (_cameraShake < 0.0f) {
        // !FAKE: stand-in local; the PDB lists none
        GameCam* cam = g_CameraManager->Find(_cameraName);
        if (cam)
            cam->SetShakeFactor(0.0f, -1.0f);
    }
    _node->SetOption(SwampSceneNode::DontDraw, true, false);
    _finished = true;
    // !FAKE: stand-in local; the PDB lists none
    Vince* vince = pManager->GetVince();
    vince->GetScript()->InvalidateScript();
    if (_restoreVinceModel)
        vince->SetModel(_modelVince);
    vince->GetScript()->Message(0, VinceMain::MESSAGE_WHAMMY_CLEANUP, 0);
}

// FUNCTION: USA 0x001349FC
// PROTOTYPE: PREVIEW 0x0051EC00 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultFlattenMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 98) WhammyResultFlattenMain(node);
}

// FUNCTION: USA 0x00134A20
// PROTOTYPE: PREVIEW 0x0051EC24 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultFrozenMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 119) WhammyResultFrozenMain(node);
}

// FUNCTION: USA 0x00134A44
// PROTOTYPE: PREVIEW 0x0051EC48 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultCutHorizontalMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 142) WhammyResultCutHorizontalMain(node);
}

// FUNCTION: USA 0x00134A6B
// PROTOTYPE: PREVIEW 0x0051EC6F identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultCutVerticalMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 163) WhammyResultCutVerticalMain(node);
}

// FUNCTION: USA 0x00134A92
// PROTOTYPE: PREVIEW 0x0051EC96 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultCutMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 184) WhammyResultCutMain(node);
}

// FUNCTION: USA 0x00134AB9
// PROTOTYPE: PREVIEW 0x0051ECBD identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 50.0, bytes match (cut-extent wall: retail cut symbol runs past the
// row; va_dis no difference; fix is row-end pins + re-cut, source is final; generated, 4.4 T10)
SwampScript* WhammyResultDisintegrateMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 205) WhammyResultDisintegrateMain(node);
}
#endif

// FUNCTION: USA 0x00134B07
// PROTOTYPE: PREVIEW 0x0051ED0B identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultPokeMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 246) WhammyResultPokeMain(node);
}

// FUNCTION: USA 0x00134B2E
// PROTOTYPE: PREVIEW 0x0051ED32 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultJackhammerMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 266) WhammyResultJackhammerMain(node);
}

// FUNCTION: USA 0x00134B55
// PROTOTYPE: PREVIEW 0x0051ED59 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultToxicMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 286) WhammyResultToxicMain(node);
}

// FUNCTION: USA 0x00134B7C
// PROTOTYPE: PREVIEW 0x0051ED80 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyResultBurstMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 306) WhammyResultBurstMain(node);
}

// FUNCTION: USA 0x00134CAB
// PROTOTYPE: PREVIEW 0x0051EEAF identical
// NAME: proto-pdb:default.pdb
void WhammyProjectileMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_SET_TARGET:
        _target = SwampString((const char*)data);
        InitCurve();
        break;
    case MESSAGE_SET_RESPONSE_SCRIPT:
        _scriptName = SwampString((const char*)data);
        break;
    }
}

// FUNCTION: USA 0x00134E51
// PROTOTYPE: PREVIEW 0x0051F055 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~VinceWhammyMain in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceWhammyMain_dtor(VinceWhammyMain* p)
{
    p->VinceWhammyMain::~VinceWhammyMain();
}

// FUNCTION: USA 0x00134F0A
// PROTOTYPE: PREVIEW 0x0051F0FA identical
// NAME: proto-pdb:default.pdb
void VinceWhammyMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_SET_WHAMMY_NAME:
        _whammyName = (const char*)data;
        CreateProp();
        break;
    case MESSAGE_KILL_WHAMMY:
        _prop->Invalidate();
        InvalidateScript();
        break;
    }
}

// FUNCTION: USA 0x00135502
// PROTOTYPE: PREVIEW 0x0051F6F2 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceWhammyMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 16) VinceWhammyMain(node);
}

// FUNCTION: USA 0x00135F3A
// PROTOTYPE: PREVIEW 0x0052012A identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~WhammyPropMain in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_WhammyPropMain_dtor(WhammyPropMain* p)
{
    p->WhammyPropMain::~WhammyPropMain();
}

// FUNCTION: USA 0x00135F67
// PROTOTYPE: PREVIEW 0x00520157 identical
// NAME: proto-pdb:default.pdb
SwampScript* WhammyPropMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Whammy.h", 54) WhammyPropMain(node);
}
