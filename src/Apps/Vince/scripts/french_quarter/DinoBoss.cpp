// UNIT: src/Apps/Vince/scripts/french_quarter/DinoBoss.cpp OBJ: Xbox\OfficialRelease\DinoBoss.obj OPT: /O1 /Oy /Ob1 /Oi
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const E* _Ptr);
    int compare(const E* _Ptr) const;

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
template <class E, class T, class A>
inline bool operator!=(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return !(_Left == _Right);
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

extern "C" double __cdecl fabs(double);
inline float fabs(float _X)
{
    return (float)fabs((double)_X);
}

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(double);
double RandomFloat(float, float);
}

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    void Normalize();
    Vector3& operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion(const Vector3& v, float f) { Set(v, f); }
    void Set(const Vector3&, float);
    Quaternion& operator*=(const Quaternion&);
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class AABox; // NAME: proto-pdb:default.pdb
class SwampScript; // NAME: proto-pdb:default.pdb
class BeepTDF; // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08

    SwampScript* GetScript() const;
    const Quaternion& SetRotation(const Quaternion&);
    const Quaternion& GetRotation() const { return _Rotation; }

    char field_4[0x3C];                // 0x04
    Quaternion _Rotation;              // 0x40
    char field_50[0x54];               // 0x50
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);
class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x8];                // 0xA4
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
    char field_B4[0x14C];              // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetVelocity() const { return _Velocity; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }
    float GetAngleDiff(const Vector3&);

    char field_200[0xC];               // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0xB0];              // 0x218
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);
class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
    char field_4[0x11C];               // 0x04
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class GameCam { // NAME: proto-pdb:default.pdb
public:
    void SetShakeFactor(float, float);
};

namespace Particle { // NAME: proto-pdb:default.pdb
void LandDust(const Vector3&); // NAME: proto-pdb:default.pdb
}

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;

    char field_0[0xC];                 // 0x00
    SwampString _curCamName;           // 0x0C
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

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
    void RemoveTimerEvents();
    void RegisterFrameTick();
    void UnRegisterFrameTick();

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class DinoBossMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum State { // NAME: proto-pdb:default.pdb
        STATE_WAITFORACTIVATE = 0x0,
        STATE_STILL = 0x1,
        STATE_HOP = 0x2,
        STATE_ROAR = 0x3,
        STATE_CHOMP = 0x4,
        STATE_TAILATTACK = 0x5,
        STATE_DAMAGE = 0x6,
        STATE_DEATH = 0x7
    };
    enum Timer { // NAME: proto-pdb:default.pdb
        TIMER_FINDVINCE = 0x0,
        TIMER_SHAKE = 0x1,
        TIMER_ROAR = 0x2
    };
    enum { // NAME: proto-pdb:default.pdb
        VINCE_ONFLOOR = 0x0,
        VINCE_ONSTAIRS = 0x1,
        VINCE_ONTOP = 0x2
    };

    virtual void AnimationFinished();
    virtual void TimerEvent(unsigned int);
    virtual void FrameTick();
    virtual void Activate();

private:
    void SetState(int, bool);
    bool VinceOnMuseumFloor();
    bool VinceOnUpperFloor();
    void ResetAttackTimer();
    void UpdateVincesWhereabouts();
    void DamageVince(SwampSceneNode*);
    Critter* GetDino() { return static_cast<Critter*>(_node); }

    char field_BC[0x34];               // 0xBC
    bool _spinNow;                     // 0xF0
    char field_F1[0x3];                // 0xF1
    Vector3 _destVince;                // 0xF4
    int _chompCount;                   // 0x100
    float _attackTimer;                // 0x104
    int _wheresVince;                  // 0x108
    float _vinceFloorTimer;            // 0x10C
    char field_110[0x10];              // 0x110
};
DECOMP_SIZE_ASSERT(DinoBossMain, 0x120);

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x2BC];               // 0x00
    float _museumDinoAttackDelay;      // 0x2BC
    float _museumDinoTailAttackLead;   // 0x2C0
    char field_2C4[0xA0C - 0x2C4];     // 0x2C4
};
DECOMP_SIZE_ASSERT(Globals, 0xA0C);

extern Globals gSettings; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000875A7
// PROTOTYPE: PREVIEW 0x004732FA identical
// NAME: proto-pdb:default.pdb
bool DinoBossMain::VinceOnMuseumFloor()
{
    return _vinceFloorTimer < 2.0f;
}

// FUNCTION: USA 0x000875C1
// PROTOTYPE: PREVIEW 0x00473314 identical
// NAME: proto-pdb:default.pdb
bool DinoBossMain::VinceOnUpperFloor()
{
    return _wheresVince == 2;
}

// FUNCTION: USA 0x000875CE
// PROTOTYPE: PREVIEW 0x00473321 identical
// NAME: proto-pdb:default.pdb
void DinoBossMain::ResetAttackTimer()
{
    _attackTimer = gSettings._museumDinoAttackDelay;
}

// FUNCTION: USA 0x000875DA
// PROTOTYPE: PREVIEW 0x0047332D identical
// NAME: proto-pdb:default.pdb
void DinoBossMain::UpdateVincesWhereabouts()
{
    if (pManager->GetVince()->GetWorldPosition().y < _node->GetWorldPosition().y + pManager->GetVince()->GetHeight()) {
        _wheresVince = VINCE_ONFLOOR;
        _vinceFloorTimer = 0.0f;
    } else {
        _vinceFloorTimer += g_swamp->GetInput().GetElapsedTime();
    }
    if (g_CameraManager->_curCamName == "upperdeckcam")
        _wheresVince = VINCE_ONTOP;
}

// FUNCTION: USA 0x0008766B
// PROTOTYPE: PREVIEW 0x004735A4 identical
// NAME: proto-pdb:default.pdb
void DinoBossMain::DamageVince(SwampSceneNode* pvince)
{
    DamageSourceInfo dsi;
    dsi.sourceType = "dinoboss";
    switch (_state) {
    case STATE_CHOMP:
        dsi.attackType = "chomp";
        break;
    case STATE_TAILATTACK:
        dsi.attackType = "tail";
        break;
    default:
        dsi.attackType = "bump";
        break;
    }
    pvince->GetScript()->Message(this, MESSAGE_DAMAGE, &dsi);
    // !FAKE: inliner; pointer locals + hand form of retail's inlined Vector3 difference (C0191)
    const Vector3* v0 = &_node->GetWorldPosition();
    const Vector3* v1 = &pvince->GetWorldPosition();
    Vector3 diff;
    diff.x = v1->x - v0->x;
    diff.y = v1->y - v0->y;
    diff.z = v1->z - v0->z;
    diff.Normalize();
    diff.x *= 2.0f;
    diff.z *= 2.0f;
    diff.y = 2.0f;
    static_cast<Critter*>(pvince)->SetVelocity(diff);
}

// FUNCTION: USA 0x000880A5
// PROTOTYPE: PREVIEW 0x00474040 identical
// NAME: proto-pdb:default.pdb
void DinoBossMain::Activate()
{
    UnRegisterFrameTick();
    RemoveTimerEvents();
    RegisterFrameTick();
    RegisterTimerEvent(TIMER_FINDVINCE, BeepLib::RandomFloat(1.0f, 3.0f), false);
    SetState(STATE_ROAR, false);
    _destVince = pManager->_vinceCache->GetWorldPosition();
    ResetAttackTimer();
}

// FUNCTION: USA 0x00088120
// PROTOTYPE: PREVIEW 0x004740BB identical
// NAME: proto-pdb:default.pdb
void DinoBossMain::FrameTick()
{
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    if (pManager->GetVince() == 0)
        return;
    UpdateVincesWhereabouts();
    if (_attackTimer > 0.0f) {
        _attackTimer -= elapsed;
        if (_attackTimer < 0.0f)
            _attackTimer = 0.0f;
    }
    if (_state == STATE_CHOMP && g_CameraManager->_curCamName != "upperdeckcam") {
        RemoveTimerEvents();
        SetState(STATE_STILL, false);
    }
    if (_spinNow) {
        Vector3 mypos = _node->GetWorldPosition();
        // !FAKE: inliner; pointer locals + hand form of retail's inlined Vector3 differences (C0191)
        const Vector3* v0 = &_destVince;
        Vector3 diff;
        diff.x = v0->x - mypos.x;
        diff.y = v0->y - mypos.y;
        diff.z = v0->z - mypos.z;
        Vector3 pos2face;
        if (VinceOnMuseumFloor()) {
            Vector3 v1;
            v1.x = mypos.x - diff.x;
            v1.y = mypos.y - diff.y;
            v1.z = mypos.z - diff.z;
            pos2face = v1;
        } else {
            pos2face = *v0;
        }
        float maxturn = elapsed * 3.92699075f;
        // !FAKE: c2 register allocation; neutral local; the angle lives on the x87 stack, no PDB local
        float tmp = -GetDino()->GetAngleDiff(pos2face);
        if (!(fabs(tmp) < 0.0314159244f)) {
            if (fabs(tmp) > maxturn)
                tmp = (tmp < 0.0f) ? -maxturn : maxturn;
            if (tmp != 0.0f) {
                Quaternion hrot(Vector3(0.0f, 1.0f, 0.0f), tmp);
                {
                    // !FAKE: inliner; hand form of retail's inlined Quaternion product: inner-block copy, then a temporary (C0191)
                    Quaternion v2 = GetDino()->GetRotation();
                    v2 *= hrot;
                    _node->SetRotation(Quaternion(v2));
                }
            }
        }
    }
}

// FUNCTION: USA 0x000882CB
// PROTOTYPE: PREVIEW 0x00474266 identical
// NAME: proto-pdb:default.pdb
void DinoBossMain::AnimationFinished()
{
    switch (_state) {
    case STATE_DEATH:
        InvalidateScript();
        break;
    case STATE_CHOMP:
    case STATE_TAILATTACK:
        _spinNow = false;
        SetState(STATE_STILL, false);
        ResetAttackTimer();
        break;
    case STATE_ROAR: {
        // !FAKE: inliner; pointer locals + hand form of retail's inlined Vector3 scale and sum (C0191)
        const Vector3* v0 = &pManager->GetVince()->GetVelocity();
        Vector3 v1;
        v1.x = v0->x * 2.0f;
        v1.y = v0->y * 2.0f;
        v1.z = v0->z * 2.0f;
        const Vector3* v2 = &pManager->GetVince()->GetWorldPosition();
        Vector3 v3;
        v3.x = v1.x + v2->x;
        v3.y = v1.y + v2->y;
        v3.z = v1.z + v2->z;
        _destVince = v3;
        if (VinceOnUpperFloor() && _attackTimer <= 0.0f)
            SetState(STATE_CHOMP, false);
        else
            SetState(STATE_STILL, false);
        break;
    }
    case STATE_HOP:
        if (VinceOnMuseumFloor() && _attackTimer <= 0.0f)
            SetState(STATE_TAILATTACK, false);
        else
            SetState(STATE_STILL, false);
        break;
    case STATE_DAMAGE:
        SetState(STATE_STILL, false);
        break;
    }
}

#ifdef NON_MATCHING
extern "C" double __cdecl atan2(double, double);
inline float atan2(float _Y, float _X)
{
    return (float)atan2((double)_Y, (double)_X);
}
#endif

// FUNCTION: USA 0x00088672
// PROTOTYPE: PREVIEW 0x0047437B identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 96.89, bytes mismatch (code shape matches; stack slot layout of Vector3 locals/temps differs, x87 keeps x not y in the velocity scale)
void DinoBossMain::TimerEvent(unsigned int timerid)
{
    if (pManager->GetVince() == 0 || _state == STATE_DEATH)
        return;
    Vector3 vincepos = pManager->GetVince()->GetWorldPosition();
    Vector3 mypos = _node->GetWorldPosition();
    switch (timerid) {
    case TIMER_FINDVINCE:
        if (VinceOnMuseumFloor()) {
            {
                // !FAKE: inliner; pointer local + hand form of retail's inlined Vector3 scale, block-scoped temp (C0191)
                const Vector3* v0 = &pManager->GetVince()->GetVelocity();
                Vector3 v1(v0->x * 2.0f, v0->y * 2.0f, v0->z * 2.0f);
                vincepos += v1;
            }
            Vector3 diff;
            diff.x = vincepos.x - mypos.x;
            diff.y = vincepos.y - mypos.y;
            diff.z = vincepos.z - mypos.z;
            diff.y = 0.0f;
            // !FAKE: c2 register allocation; neutral locals; the angles live on the x87 stack, no PDB local
            float v2 = gSettings._museumDinoTailAttackLead;
            float v3 = atan2(diff.z, diff.x) - v2;
            if (v3 > 0.0f && v3 < 0.63f)
                v2 -= 0.63f - v3;
            Vector3 loc;
            {
                // !FAKE: inliner; hand form of retail's inlined by-value Quaternion::Apply, block-scoped temp (C0191)
                Vector3 v5;
                Quaternion(Vector3(0.0f, 1.0f, 0.0f), v2).Apply(diff, v5);
                loc = v5;
            }
            {
                // !FAKE: inliner; hand form of retail's inlined Vector3 sum, block-scoped temp (C0191)
                Vector3 v4;
                v4.x = loc.x + mypos.x;
                v4.y = loc.y + mypos.y;
                v4.z = loc.z + mypos.z;
                _destVince = v4;
            }
            if (_attackTimer <= 0.0f)
                SetState(STATE_HOP, false);
            else
                SetState(STATE_ROAR, false);
        } else {
            // !FAKE: c2 register allocation; neutral local; the angle lives on the x87 stack, no PDB local
            float v2 = fabs(GetDino()->GetAngleDiff(vincepos));
            if (v2 > 0.785398185f) {
                SetState(STATE_HOP, false);
                _destVince = pManager->GetVince()->GetWorldPosition();
            } else {
                SetState(STATE_ROAR, false);
            }
        }
        break;
    case TIMER_SHAKE: {
        g_CameraManager->Find(g_CameraManager->_curCamName)->SetShakeFactor(0.35f, 0.5f);
        Vector3 loc = _node->GetWorldPosition();
        Vector3 offset;
        // !FAKE: c1xx front end; neutral loop counter, no PDB local
        for (int v0 = 0; v0 < 20; v0++) {
            // !FAKE: c2 register allocation; neutral local; the angle lives on the x87 stack, no PDB local
            float v2 = BeepLib::RandomFloat(6.283185307179586);
            Quaternion qrot(Vector3(0.0f, 1.0f, 0.0f), v2);
            {
                // !FAKE: inliner; hand form of retail's inlined by-value Quaternion::Apply, block-scoped temp (C0191)
                Vector3 v3;
                qrot.Apply(Vector3(0.0f, 0.0f, GetDino()->GetRadius() * 0.5f), v3);
                offset = v3;
            }
            {
                // !FAKE: inliner; hand form of retail's inlined Vector3 sum, block-scoped temp (C0191)
                Vector3 v1;
                v1.x = loc.x + offset.x;
                v1.y = loc.y + offset.y;
                v1.z = loc.z + offset.z;
                Particle::LandDust(v1);
            }
        }
        break;
    }
    case TIMER_ROAR:
        g_CameraManager->Find(g_CameraManager->_curCamName)->SetShakeFactor(0.35f, 0.5f);
        break;
    }
}
#endif
