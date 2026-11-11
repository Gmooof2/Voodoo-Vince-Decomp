// UNIT: src/Apps/Vince/core/PitFrog.cpp OBJ: core\XBox\OfficialRelease\PitFrog.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string() { _Myres = 15; _Eos(0); }
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, npos); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    const E* c_str() const { return _Myptr(); }
    unsigned int size() const { return _Mysize; }
    int compare(const E* _Ptr) const;
    static const unsigned int npos = (unsigned int)(-1);
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }
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
template <class E, class T, class A>
inline bool operator!=(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return !(_Left == _Right);
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

extern "C" int __cdecl sscanf(const char*, const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
    int GetIntRecord(const SwampString& id, int def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
    void RotateJ(float);

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    Vector3 GetFacing() const;
};

class Critter { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x234];               // 0x00
    Vector3 _homePosition;             // 0x234
    char field_240[0x2C8 - 0x240];     // 0x240
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class Locator { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetLocation() const { return _pos; }

    Vector3 _pos;                      // 0x00
    char field_C[0x48 - 0xC];          // 0x0C
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    Locator* Find(const SwampString& s) const;
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
}

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
    virtual SwampString Save();

    int _state;                        // 0x4C
    Vector3 _wanderPatrolCenter;       // 0x50
    char field_5C[0x6C - 0x5C];        // 0x5C
    Vector3 _wanderDir;                // 0x6C
    char field_78[0xA8 - 0x78];        // 0x78
    bool _wanderAvoidLedges;           // 0xA8
    char field_A9[0xBC - 0xA9];        // 0xA9
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void DeathFX();                                        // vtable+0x70

    char field_BC[0x240 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

class PitFrogMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    PitFrogMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_HIDDEN = 0,
        STATE_IDLE = 3
    };
    virtual void DeathFX();
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
    void SetRandomPatrolDir();
    Critter* GetPitFrog() { return (Critter*)_node; }

    char field_240[0x33C - 0x240];     // 0x240
    Vector3 _landingFacing;            // 0x33C
    Vector3 _pitPos;                   // 0x348
    bool _haspit;                      // 0x354
    char field_355[0x358 - 0x355];     // 0x355
    SwampString _patrolLoc;            // 0x358
    SwampString _pit;                  // 0x374
    char field_390[0x394 - 0x390];     // 0x390
    int _hoverAnimIndex;               // 0x394
    char field_398[0x39C - 0x398];     // 0x398
};
DECOMP_SIZE_ASSERT(PitFrogMain, 0x39C);

// FUNCTION: USA 0x00115C0C
// PROTOTYPE: PREVIEW 0x0050045A identical
// NAME: proto-pdb:default.pdb
void PitFrogMain::SetRandomPatrolDir()
{
    Vector3 patrol = _node->GetFacing();
    patrol.RotateJ(BeepLib::RandomFloat(-1.5707964f, 1.5707964f));
    _wanderDir = patrol;
}

// FUNCTION: USA 0x00115E06
// PROTOTYPE: PREVIEW 0x00500654 identical
// NAME: proto-pdb:default.pdb
void PitFrogMain::DeathFX()
{
    SoundManager::PlaySound("frogdeath", 0, _node, false, 0.0f, 0.0f);
    MonsterMain::DeathFX();
}

// FUNCTION: USA 0x00116807
// PROTOTYPE: PREVIEW 0x0050104B identical
// NAME: proto-pdb:default.pdb
void PitFrogMain::Load(const BeepTDF& tdf)
{
    _pit = tdf.GetStringRecord("pit", "false");
    if (_pit == "true") {
        SetState(STATE_HIDDEN);
        _haspit = true;
        _wanderAvoidLedges = false;
        _landingFacing = _node->GetFacing();
    } else {
        SetState(STATE_IDLE);
        _haspit = false;
    }
    SwampString landFacing;
    landFacing = tdf.GetStringRecord("landFacing", "");
    if (landFacing.size() != 0) {
        sscanf(landFacing.c_str(), "%f %f %f", &_landingFacing.x, &_landingFacing.y, &_landingFacing.z);
    }
    Vector3 patrolCenter = GetPitFrog()->_homePosition;
    if (_haspit) {
        // !FAKE: inliner; pointer locals and hand form of the inlined Vector3 scale and difference; no PDB free
        // operator, D78); direct member reads score 98.05, the C0191 pointer locals 100.0
        const Vector3* v2 = &_landingFacing;
        Vector3 v0(v2->x * 1.75f, v2->y * 1.75f, v2->z * 1.75f);
        const Vector3* v1 = &GetPitFrog()->_homePosition;
        _pitPos = Vector3(v1->x - v0.x, v1->y - v0.y, v1->z - v0.z);
        _patrolLoc = tdf.GetStringRecord("patrolLoc", "");
        if (_patrolLoc != "") {
            // !FAKE: c2 register allocation; dummy local; the PDB records no local for the Find result
            Locator* p = g_locatorManager->Find(_patrolLoc);
            if (p) {
                patrolCenter = p->GetLocation();
            }
        }
        _hoverAnimIndex = tdf.GetIntRecord("hoverAnim", 0);
    }
    _wanderPatrolCenter = patrolCenter;
}

// FUNCTION: USA 0x00116AA5
// PROTOTYPE: PREVIEW 0x005012E9 identical
// NAME: proto-pdb:default.pdb
SwampString PitFrogMain::Save()
{
    SwampString str = MonsterMain::Save();
    str += format("\t\tpatrolLoc = %s;\n", _patrolLoc.c_str());
    str += format("\t\tpit = %s;\n", _pit.c_str());
    str += format("\t\tlandFacing = %f %f %f;\n", _landingFacing.x, _landingFacing.y, _landingFacing.z);
    return str;
}

// FUNCTION: USA 0x00117D11
// PROTOTYPE: PREVIEW 0x00502555 identical
// NAME: proto-pdb:default.pdb
SwampScript* PitFrogMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\PitFrog.h", 13) PitFrogMain(node);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x00117D38
// NAME: proto-pdb:default.pdb
PitFrogMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("PitFrogMain", creationFunc);
}
