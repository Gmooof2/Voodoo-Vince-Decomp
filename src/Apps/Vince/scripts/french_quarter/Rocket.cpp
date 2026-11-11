// UNIT: src/Apps/Vince/scripts/french_quarter/Rocket.cpp OBJ: Xbox\OfficialRelease\Rocket.obj OPT: /O1 /Oy
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
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    // !FAKE: inliner; hand form of the PDB inline conversion operator; no before/after recorded
    operator Vector3() const { return *(const Vector3*)this; }

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
    float w; // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    const Vector4& operator[](int i) const { return row[i]; }

    Vector4 row[4]; // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v; // 0x00
    float w;   // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C

    const Vector3& GetLocation() const { return _Location; }
    const Quaternion& SetFacing(const Vector3&);
    const Vector3& SetLocation(const Vector3&);

    char field_4[0x30];                // 0x04
    Vector3 _Location;                 // 0x34
    char field_40[0x64];               // 0x40
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x8];                // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    float GetRadius() const { return _radius; }

    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    Matrix GetBoneOrientation(int) const;

    char field_B4[0x14C];              // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void PlaySound(const SwampString&, unsigned int, bool, float, float);

    char field_200[0xC8];              // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetOnFire(float, bool);

    char field_2C8[0x2D0];             // 0x2C8
};
DECOMP_SIZE_ASSERT(Vince, 0x598);

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetMainstreetFlameRange() const { return _mainstreetFlameRange; }
    float GetMainstreetFlameDiameter() const { return _mainstreetFlameDiameter; }
    float GetMainstreetFlameTime() const { return _mainstreetFlameTime; }

    char field_0[0x254];               // 0x000
    float _mainstreetFlameRange;       // 0x254
    float _mainstreetFlameDiameter;    // 0x258
    float _mainstreetFlameTime;        // 0x25C
    char field_260[0x7AC];             // 0x260
};
DECOMP_SIZE_ASSERT(Globals, 0xA0C);

extern Globals gSettings; // NAME: proto-pdb:default.pdb

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
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class RocketMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    RocketMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

public:
    virtual void FrameTick();
    virtual void Activate();

    char field_BC[0xC0 - 0xBC];        // 0xBC
    FireFlyNode* _emitter;             // 0xC0
    int _flameJointId;                 // 0xC4
    bool _active;                      // 0xC8
};
DECOMP_SIZE_ASSERT(RocketMain, 0xCC);

// FUNCTION: USA 0x00081BCB
// PROTOTYPE: PREVIEW 0x0046DA8A identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 96.19, radius copied by mov (retail fld/fst feeding radius + diameter); rocket/radius slot -4 vs -0x10
// 5.2 T12.c19 at-stop 20: radius int-copy vs retail fld/fst survives every decl/stand-in/chain form (cl copy-propagates them)
void RocketMain::FrameTick()
{
    Critter* rocket = GetCritter();
    Matrix m = rocket->GetBoneOrientation(_flameJointId);
    Vector3 flamedir = m[2];
    Vector3 flamepos = m[3];
    _emitter->SetFacing(flamedir);
    _emitter->SetLocation(flamepos);
    // !FAKE: c2 register allocation; stand-in v0 for the cached Vince (ebx; no PDB local)
    Vince* v0 = pManager->GetVince();
    if (v0) {
        float RocketRange = gSettings.GetMainstreetFlameRange();
        Quaternion q = rocket->GetWorldRotation();
        Vector3 v = rocket->GetWorldPosition();
        // !FAKE: inliner; hand form of the by-value Quaternion::Apply and the Vector3 operators; no before/after recorded; the operators: sum, difference, scale and
        // LengthSquared (out of line in our /O1 build; no free operator in the PDB): stand-ins v1..v6
        {
            Vector3 v1;
            q.Apply(flamedir, v1);
            flamedir = v1;
        }
        {
            Vector3 v1;
            q.Apply(flamepos, v1);
            Vector3 v2 = v1;
            Vector3 v3 = {v2.x + v.x, v2.y + v.y, v2.z + v.z};
            flamepos = v3;
        }
        float radius = v0->GetRadius();
        float v4 = gSettings.GetMainstreetFlameDiameter() + radius;
        Vector3 vincecenter = v0->GetLocation();
        vincecenter.y += radius;
        Vector3 tovince = {vincecenter.x - flamepos.x, vincecenter.y - flamepos.y, vincecenter.z - flamepos.z};
        float v5 = tovince.z * flamedir.z + tovince.y * flamedir.y + tovince.x * flamedir.x;
        if (v5 > 0.0f && v5 < radius + RocketRange) {
            Vector3 v6 = {flamedir.x * v5, flamedir.y * v5, flamedir.z * v5};
            Vector3 v7 = {tovince.x - v6.x, tovince.y - v6.y, tovince.z - v6.z};
            if (v7.x * v7.x + v7.y * v7.y + v7.z * v7.z < v4 * v4)
                v0->SetOnFire(gSettings.GetMainstreetFlameTime(), true);
        }
    }
}
#endif

// FUNCTION: USA 0x00081EA4
// PROTOTYPE: PREVIEW 0x0046DD63 identical
// NAME: proto-pdb:default.pdb
void RocketMain::Activate()
{
    if (!_active) {
        GetCritter()->PlaySound("qms_rocketburn", 9, true, 0.0f, 0.0f);
        _active = true;
    }
}

// FUNCTION: USA 0x00081EF6
// PROTOTYPE: PREVIEW 0x0046DDB5 identical
// NAME: proto-pdb:default.pdb
SwampScript* RocketMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("mainstreet\\Rocket.cpp", 12) RocketMain(node);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x00081F1D
// NAME: proto-pdb:default.pdb
RocketMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("RocketMain", creationFunc);
}
