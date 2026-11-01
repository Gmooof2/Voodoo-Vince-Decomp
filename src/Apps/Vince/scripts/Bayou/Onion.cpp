// UNIT: src/Apps/Vince/scripts/Bayou/Onion.cpp OBJ: Xbox\OfficialRelease\Onion.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    ~basic_string() { _Tidy(true); }
    const E* c_str() const { return _Myptr(); } // src proto-pdb:default.pdb c_str
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

protected:
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
template <class T1, class T2> struct pair;
template <class T> struct less;
template <class T, class A> class vector {
public:
    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; } // src proto-pdb:default.pdb _Myend // src proto-pdb:default.pdb _Myfirst // src proto-pdb:default.pdb _Mylast
    ~vector() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits;
template <class Traits> class _Tree {
public:
    _Tree() { _Init(); } // src proto-pdb:default.pdb _Init
    ~_Tree() { _Tidy(); }

protected:
    void _Init();
    void _Tidy();

private:
    char _pad00[0xC];
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > >
class map : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

extern "C" int __cdecl _strnicmp(const char*, const char*, unsigned int); // src proto-pdb:default.pdb _strnicmp

void* operator new(unsigned int size, const char* file, int line);
class Matrix;                // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
public:
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3&, Vector3&) const;
    // !FAKE: __forceinline; plain inline stays an out-of-line call at /O1, retail inlines it
    __forceinline Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct AABox { // NAME: proto-pdb:default.pdb
public:
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

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
    const Vector3& SetLocation(const Vector3& loc);
    Vector3 GetFacing() const;
    void SetScale(float f);
    const SwampString& GetName() const { return _Name; }
    SwampScript* GetScript() const;
protected:
    void InvalidateCachedWorldPos();
public:
    int _Type;                         // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x34 - 0x24];        // 0x24
    Vector3 _Location;                 // 0x34
    Quaternion _Rotation;              // 0x40
    Vector3 _Scale;                    // 0x50
    char field_5C[0x6C - 0x5C];        // 0x5C
    AABox _BoundingBox;                // 0x6C
    char field_84[0xA4 - 0x84];        // 0x84
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
    void PlaySound(const SwampString& name, unsigned int flags, bool bAttach, float minDist3D, float maxDist3D);

    char field_200[0x2C8 - 0x200];     // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

// Retail layout: retail disasm puts _ridingTrolley at 0x317, _targetLocation at 0x3A4, _vehicleName at 0x548
// (proto PDB 0x307, 0x390, 0x518; Vince 0x598); the spans between are opaque.
class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetTargetLocation(const SwampString& s, bool istrolley);
    void SetVehicleName(const SwampString& name);

    char field_2C8[0x317 - 0x2C8];     // 0x2C8
    bool _ridingTrolley;               // 0x317
    char field_318[0x3A4 - 0x318];     // 0x318
    SwampString _targetLocation;       // 0x3A4
    char field_3C0[0x548 - 0x3C0];     // 0x3C0
    SwampString _vehicleName;          // 0x548
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString& name, unsigned int flags, SwampSceneNode* parent, bool bAttach,
                          float minDist3D, float maxDist3D);
};

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

#ifdef NON_MATCHING
#endif

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    ~AnimationControl();
    void SubRef();

    char field_24[0x44];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

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

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

// FUNCTION: USA 0x00018673
// PROTOTYPE: PREVIEW 0x004078CA identical
// NAME: proto-pdb:default.pdb
class AreaStats { // NAME: proto-pdb:default.pdb
public:
    int _clustersRemaining;            // 0x00
    int _clustersFound;                // 0x04
    int _jugRemaining;                 // 0x08
    int _jugMax;                       // 0x0C
    int _pagesFound;                   // 0x10
    int _pagesRemaining;               // 0x14
    int _boxesFound;                   // 0x18
    int _boxesMax;                     // 0x1C
    int _juneBugsFound;                // 0x20
    int _juneBugsMax;                  // 0x24
    int _whammysFound;                 // 0x28
    int _whammysRemaining;             // 0x2C
    SwampString _name;                 // 0x30
};
DECOMP_SIZE_ASSERT(AreaStats, 0x4C);

// !FAKE: c1xx front end; scaffold delete; retail emits ??_GAreaStats@@QAEPAXI@Z in this unit (no matched function
// deletes an AreaStats); this delete-expression emits it instead (matched_set [[emitter]] row)
void pin_emit_AreaStats_delete(AreaStats* p)
{
    delete p;
}

// FUNCTION: USA 0x000186B2
// PROTOTYPE: PREVIEW 0x00407909 identical
// NAME: proto-pdb:default.pdb
class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    virtual void Load(const BeepTDF&);
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0x84 - 0x50];        // 0x50
    SwampString _wanderChaseTarget;    // 0x84
    char field_A0[0xBC - 0xA0];        // 0xA0
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1CritterMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_CritterMain_dtor(CritterMain* p)
{
    p->CritterMain::~CritterMain();
}

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void DeathFX();                                        // vtable+0x70
    virtual Vector3 GetAttackVectorStraight();                     // vtable+0x74
    virtual Vector3 GetAttackVectorRight();                        // vtable+0x78
    virtual Vector3 GetAttackVectorLeft();                         // vtable+0x7C
    virtual float GetAttackVectorTolerance();                      // vtable+0x80

    char field_BC[0x240 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

class OnionMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_PATROL = 1,
        STATE_SNEAK = 2,
        STATE_DAMAGE = 3,
        STATE_BOUNCEBACK = 4
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_TRAPSTOP = 0x14,
        MESSAGE_SET_UNDER_TRAP = 0x15,
        MESSAGE_QUERY_UNDER_TRAP = 0x16
    };
    virtual void TimerEvent(unsigned int timerId);
    virtual void Load(const BeepTDF& tdf);

    char field_240[0x258 - 0x240];     // 0x240
};
DECOMP_SIZE_ASSERT(OnionMain, 0x258);

class TrapOnionMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_UP = 0,
        STATE_DOWN = 1,
        STATE_FALLING = 2,
        STATE_LIFTING = 3
    };
    TrapOnionMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int timerId);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    char field_BC[0xD0 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(TrapOnionMain, 0xD0);

class OnionTrapRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    OnionTrapRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(OnionTrapRegionMain, 0x4C);

// FUNCTION: USA 0x00018226
// PROTOTYPE: PREVIEW 0x004074E4 identical
// NAME: proto-pdb:default.pdb
SwampScript* OnionTrapRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\belowdecks\\Onion.h", 126) OnionTrapRegionMain(node);
}

// FUNCTION: USA 0x000186F0
// NAME: proto-pdb:default.pdb
void OnionTrapRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (_strnicmp(node->GetName().c_str(), "onion", 5) == 0)
        node->GetScript()->Message(this, OnionMain::MESSAGE_SET_UNDER_TRAP, (void*)true);
}

// FUNCTION: USA 0x00018732
// NAME: proto-pdb:default.pdb
void OnionTrapRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (_strnicmp(node->GetName().c_str(), "onion", 5) == 0)
        node->GetScript()->Message(this, OnionMain::MESSAGE_SET_UNDER_TRAP, (void*)false);
}

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
double RandomFloat(float min, float max);

// FUNCTION: USA 0x00018010
// PROTOTYPE: PREVIEW 0x004072DF identical
// NAME: proto-pdb:default.pdb
double RandomFloat(float min, float max)
{
    return (double)Random() * (max - min) * (1.0 / 4294967296.0) + min;
}
}

// FUNCTION: USA 0x0001803C
// PROTOTYPE: PREVIEW 0x0040730B identical
// NAME: proto-pdb:default.pdb
const Vector3& SwampSceneNode::SetLocation(const Vector3& loc)
{
    InvalidateCachedWorldPos();
    _Location = loc;
    return _Location;
}

// FUNCTION: USA 0x00018056
// PROTOTYPE: PREVIEW 0x00407325 identical
// NAME: proto-pdb:default.pdb
Vector3 SwampSceneNode::GetFacing() const
{
    return _Rotation.Apply(Vector3(0.0f, 0.0f, 1.0f));
}

// FUNCTION: USA 0x0001808E
// PROTOTYPE: PREVIEW 0x0040735D identical
// NAME: proto-pdb:default.pdb
const AABox& SwampSceneNode::GetBounds() const
{
    return _BoundingBox;
}

// FUNCTION: USA 0x00018092
// PROTOTYPE: PREVIEW 0x00407361 identical
// NAME: proto-pdb:default.pdb
void SwampSceneNode::SetScale(float f)
{
    _Scale.x = _Scale.y = _Scale.z = f;
    InvalidateCachedWorldPos();
}

// FUNCTION: USA 0x000180E8
// PROTOTYPE: PREVIEW 0x004073B7 identical
// NAME: proto-pdb:default.pdb
void Critter::PlaySound(const SwampString& name, unsigned int flags, bool bAttach, float minDist3D, float maxDist3D)
{
    SoundManager::PlaySound(name, flags, this, bAttach, minDist3D, maxDist3D);
}

// FUNCTION: USA 0x0001821B
// PROTOTYPE: PREVIEW 0x004074D9 identical
// NAME: proto-pdb:default.pdb
class Sprite { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    ResourceHandle<TextureResource> _texture; // 0x18
    char field_1C[0x4C - 0x1C];        // 0x1C
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1Sprite@@QAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Sprite_dtor(Sprite* p)
{
    p->Sprite::~Sprite();
}

// FUNCTION: USA 0x0001837B
// PROTOTYPE: PREVIEW 0x00407639 identical
// NAME: proto-pdb:default.pdb
void OnionMain::TimerEvent(unsigned int timerId)
{
    switch (_state) {
    case STATE_IDLE:
        SetState(STATE_PATROL);
        break;
    case STATE_PATROL:
        SetState(STATE_IDLE);
        break;
    }
}

// FUNCTION: USA 0x00018394
// PROTOTYPE: PREVIEW 0x00407652 identical
// NAME: proto-pdb:default.pdb
void OnionMain::Load(const BeepTDF& tdf)
{
    CritterMain::Load(tdf);
    GetCritter()->ReturnToHome();
    SetState(STATE_PATROL);
}

// FUNCTION: USA 0x0001840B
// PROTOTYPE: PREVIEW 0x004076C9 identical
// NAME: proto-pdb:default.pdb
void TrapOnionMain::TimerEvent(unsigned int timerId)
{
    switch (_state) {
    case STATE_DOWN:
        SetState(STATE_LIFTING);
        break;
    }
}

// FUNCTION: USA 0x00018438
// PROTOTYPE: PREVIEW 0x004076D9 identical
// NAME: proto-pdb:default.pdb
void AnimationControl::SubRef()
{
    if (--_RefCount == 0)
        delete this;
}

// FUNCTION: USA 0x00019513
// PROTOTYPE: PREVIEW 0x00408784 identical
// NAME: proto-pdb:default.pdb
SwampScript* TrapOnionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (a header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\belowdecks\\Onion.h", 75) TrapOnionMain(node);
}

class HUDUI;                 // NAME: proto-pdb:default.pdb
class Level;                 // NAME: proto-pdb:default.pdb
class LevelStats;            // NAME: proto-pdb:default.pdb
class LoadScreen;            // NAME: proto-pdb:default.pdb
typedef std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > SwampWString;

class Diary { // NAME: proto-pdb:default.pdb
public:
    ~Diary();

    char field_0[0x14C];               // 0x00
};
DECOMP_SIZE_ASSERT(Diary, 0x14C);

// FUNCTION: USA 0x00019B5F
// PROTOTYPE: PREVIEW 0x00408DD0 identical
// NAME: proto-pdb:default.pdb
class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    // FUNCTION: USA 0x00019565
    // PROTOTYPE: PREVIEW 0x004087D6 identical
    // NAME: proto-pdb:default.pdb
    struct Slot { // NAME: proto-pdb:default.pdb
        SwampString level;             // 0x00
        SwampString area;              // 0x1C
        char field_38[0x44 - 0x38];    // 0x38
        std::vector<std::pair<SwampString, SwampString>, std::allocator<std::pair<SwampString, SwampString> > >
            _chaptersFinished;         // 0x44
        SwampString _savePath;         // 0x54
        SwampWString _gameName;        // 0x70
    };

    HUDUI* _hudUI;                     // 0x00
    Level* _curLevel;                  // 0x04
    SwampString _requestLevel;         // 0x08
    SwampString _requestArea;          // 0x24
    char field_40[0x4C - 0x40];        // 0x40
    std::vector<Slot, std::allocator<Slot> > _slots; // 0x4C
    Diary _currSave;                   // 0x5C
    char field_1A8[0x1B0 - 0x1A8];     // 0x1A8
    std::vector<LevelStats, std::allocator<LevelStats> > _levelStatsList; // 0x1B0
    std::map<SwampString, SwampSceneNode*> _regionScriptNodeList; // 0x1C0
};
DECOMP_SIZE_ASSERT(LevelLoader::Slot, 0x8C);
DECOMP_SIZE_ASSERT(LevelLoader, 0x1CC);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1Slot@LevelLoader@@QAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Slot_dtor(LevelLoader::Slot* p)
{
    p->LevelLoader::Slot::~Slot();
}

class SwampNodeManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampSceneNode* (*CreationFunc)();
    SwampNodeManager();
    ~SwampNodeManager();

    std::map<SwampString, CreationFunc> _nodes; // 0x00
    std::vector<SwampSceneNode*, std::allocator<SwampSceneNode*> > _deadNodes; // 0x0C
};
DECOMP_SIZE_ASSERT(SwampNodeManager, 0x1C);

// FUNCTION: USA 0x000196B3
// PROTOTYPE: PREVIEW 0x00408924 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match; gate relink REFUSED
SwampNodeManager::SwampNodeManager()
{
}
#endif

// FUNCTION: USA 0x000198B5
// PROTOTYPE: PREVIEW 0x00408B26 identical
// NAME: proto-pdb:default.pdb
Vector3 MonsterMain::GetAttackVectorStraight()
{
    return Vector3(0.0f, 0.0f, 1.0f);
}

// FUNCTION: USA 0x000198CA
// PROTOTYPE: PREVIEW 0x00408B3B identical
// NAME: proto-pdb:default.pdb
Vector3 MonsterMain::GetAttackVectorRight()
{
    return Vector3(0.7071f, 0.0f, 0.7071f);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1LevelLoader@@QAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LevelLoader_dtor(LevelLoader* p)
{
    p->LevelLoader::~LevelLoader();
}

// FUNCTION: USA 0x00018B34
// PROTOTYPE: PREVIEW 0x00407D79 near-identical
// NAME: proto-pdb:default.pdb
void Vince::SetTargetLocation(const SwampString& s, bool istrolley)
{
    _targetLocation = s;
    _ridingTrolley = istrolley;
}

// FUNCTION: USA 0x00018B58
// PROTOTYPE: PREVIEW 0x00407D9D near-identical
// NAME: proto-pdb:default.pdb
void Vince::SetVehicleName(const SwampString& name)
{
    _vehicleName = name;
}
