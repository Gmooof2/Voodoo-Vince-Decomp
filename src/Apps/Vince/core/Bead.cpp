// UNIT: src/Apps/Vince/core/Bead.cpp OBJ: core\XBox\OfficialRelease\Bead.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

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
    unsigned int _pad00[7];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
}

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
    enum NodeOptions { UseAlpha = 0x8 };
    enum NodeRenderingSettings { Alpha = 0x0 }; // NAME: proto-pdb:default.pdb
    void SetOption(unsigned int option, bool on, bool recurse);
    void SetRenderingSetting(unsigned int setting, unsigned int value, bool recurse);

    char field_4[0xA4 - 0x4];          // 0x04
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x10];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class AnimationControl; // NAME: proto-pdb:default.pdb

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetGravity() const { return Gravity; }
    float GetMiscBeadSmallVel() const { return _miscBeadSmallVelocity; }
    float GetMiscBeadMedVel() const { return _miscBeadMedVelocity; }
    float GetMiscBeadLargeVel() const { return _miscBeadLargeVelocity; }
    int GetMiscBeadSmallValue() const { return _miscBeadSmallValue; }
    int GetMiscBeadMedValue() const { return _miscBeadMedValue; }
    int GetMiscBeadLargeValue() const { return _miscBeadLargeValue; }

    char field_0[0x8C];                // 0x00
    float Gravity;                     // 0x8C
    char field_90[0x164 - 0x90];       // 0x90
    float _miscBeadLifetime;           // 0x164
    char field_168[0x174 - 0x168];     // 0x168
    int _miscBeadSmallValue;           // 0x174
    int _miscBeadMedValue;             // 0x178
    int _miscBeadLargeValue;           // 0x17C
    float _miscBeadSmallVelocity;      // 0x180
    float _miscBeadMedVelocity;        // 0x184
    float _miscBeadLargeVelocity;      // 0x188
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

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
    void SetRadius(float);
    void SetHeight(float);

    char field_AC[0xB4 - 0xAC];        // 0xAC
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_DUST = 0x10
};

// Retail layout: Critter _wallThreshold 0x254 -> 0x264, _CritterType 0x258 -> 0x268 (+0x10).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40

    char field_200[0x20C - 0x200];     // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x264 - 0x218];     // 0x218
    float _wallThreshold;              // 0x264
    CritterType _CritterType;          // 0x268
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

    void RegisterFrameTick();

    char field_4[0x1C];               // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    Critter* GetCritter() { return (Critter*)_node; }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

class BeadMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_GETBEADSIZE = 0x14
    };
    enum Beadsize { // NAME: proto-pdb:default.pdb
        BEADSIZE_SMALL = 0,
        BEADSIZE_MEDIUM = 1,
        BEADSIZE_LARGE = 2
    };
    BeadMain(SwampSceneNode* node);
    virtual void Message(SwampScript* from, int message, void* data);
    static int GetBeadValue(int size);
    static SwampString GetBeadBaseName(int size);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;

private:
    static SwampScript* creationFunc(SwampSceneNode* node);

protected:
    void SetupVelocity();
    void Blink();

public:
    float _lifeRemaining;              // 0xBC
    float _nextBlinkTime;              // 0xC0
    float _blinkElapsedTime;           // 0xC4
    bool _blinkOn;                     // 0xC8
    bool _gettingSuckedDown;           // 0xC9
    unsigned char _blinkFrames;        // 0xCA
    int _size;                         // 0xCC
};
DECOMP_SIZE_ASSERT(BeadMain, 0xD0);

// FUNCTION: USA 0x001260C4
// PROTOTYPE: PREVIEW 0x0050EBD2 identical
// NAME: proto-pdb:default.pdb
void BeadMain::Message(SwampScript* from, int message, void* data)
{
    if (message == MESSAGE_GETBEADSIZE)
        *(int*)data = _size;
}

// FUNCTION: USA 0x00126140
// PROTOTYPE: PREVIEW 0x0050EC4E identical
// NAME: proto-pdb:default.pdb
void BeadMain::SetupVelocity()
{
    Vector3 v;
    v.x = BeepLib::RandomFloat(-1.5f, 1.5f);
    v.y = BeepLib::RandomFloat(GetCritter()->GetGravity() * 0.25f, gSettings.GetGravity() * 0.5f);
    v.z = BeepLib::RandomFloat(-1.5f, 1.5f);
    switch (_size) {
    case BEADSIZE_SMALL:
        v *= gSettings.GetMiscBeadSmallVel();
        break;
    case BEADSIZE_MEDIUM:
        v *= gSettings.GetMiscBeadMedVel();
        break;
    case BEADSIZE_LARGE:
        v *= gSettings.GetMiscBeadLargeVel();
        break;
    }
    GetCritter()->_Velocity = v;
}

// FUNCTION: USA 0x00126201
// NAME: proto-pdb:default.pdb
void BeadMain::Blink()
{
    _node->SetOption(SwampSceneNode::UseAlpha, !_blinkOn, true);
    _blinkFrames = 0;
    _blinkOn = !_blinkOn;
}

// FUNCTION: USA 0x00126234
// PROTOTYPE: PREVIEW 0x0050ED42 identical
// NAME: proto-pdb:default.pdb
int BeadMain::GetBeadValue(int size)
{
    // !FAKE: c2 block layout; temporary; return-per-case form puts the default return 0 before the cases (match_one 72.5 -> 100.0); the proto PDB lists no local
    int tmp = 0;
    switch (size) {
    case BEADSIZE_SMALL:
        tmp = gSettings.GetMiscBeadSmallValue();
        break;
    case BEADSIZE_MEDIUM:
        tmp = gSettings.GetMiscBeadMedValue();
        break;
    case BEADSIZE_LARGE:
        tmp = gSettings.GetMiscBeadLargeValue();
        break;
    }
    return tmp;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0) 5.5 T8.c3; wall: relink: the ctor emits ??_7BeadMain, whose
// slot ?SetState@CritterMain@@UAEXH@Z does not resolve by name (needs a retail VA pin for it)
// FUNCTION: USA 0x00126256
// PROTOTYPE: PREVIEW 0x0050ED64 near-identical
// NAME: proto-pdb:default.pdb
BeadMain::BeadMain(SwampSceneNode* node)
    : CritterMain(node), _blinkOn(false), _gettingSuckedDown(false), _blinkFrames(0), _size(0)
{
    // !FAKE: c2 register allocation; local; retail holds _node in edi across the calls; the proto PDB lists no such local
    Critter* critter = GetCritter();
    critter->_CritterType = CRITTER_TYPE_DUST;
    critter->SetRadius(0.055f);
    critter->SetHeight(0.25f);
    critter->_wallThreshold = 0.174532935f;
    _lifeRemaining = gSettings._miscBeadLifetime;
    _nextBlinkTime = _lifeRemaining < 3.0f ? _lifeRemaining : 3.0f;
    _blinkElapsedTime = 0.25f;
    RegisterFrameTick();
    critter->StartAnimation(g_swamp->GetAnimationManager().Load("bead_idle"), true, false);
    critter->SetRenderingSetting(SwampSceneNode::Alpha, 0x3F, false);
    SetupVelocity();
}
#endif

// FUNCTION: USA 0x0012641F
// NAME: proto-pdb:default.pdb
SwampString BeadMain::GetBeadBaseName(int size)
{
    switch (size) {
    case BEADSIZE_MEDIUM:
        return "bead2";
    case BEADSIZE_LARGE:
        return "bead3";
    default:
        return "bead";
    }
}

// FUNCTION: USA 0x0012644B
// PROTOTYPE: PREVIEW 0x0050EF59 identical
// NAME: proto-pdb:default.pdb
SwampScript* BeadMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\Bead.h", 53) BeadMain(node);
}

class SwampNodeManager { // NAME: proto-pdb:default.pdb
public:
    static SwampNodeManager& Instance();
    void Add(const SwampString& name, SwampSceneNode* (*func)());
};

class Bead2 { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampSceneNode* creationFunc();
};

// FUNCTION: USA 0x00126C5C
// NAME: proto-pdb:default.pdb
Bead2::RegisterMe::RegisterMe()
{
    SwampNodeManager::Instance().Add("Bead2", creationFunc);
}

class Bead3 { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampSceneNode* creationFunc();
};

// FUNCTION: USA 0x00126C98
// NAME: proto-pdb:default.pdb
Bead3::RegisterMe::RegisterMe()
{
    SwampNodeManager::Instance().Add("Bead3", creationFunc);
}

// FUNCTION: USA 0x00126CD4
// NAME: proto-pdb:default.pdb
BeadMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BeadMain", creationFunc);
}

class Bead2Main : public BeadMain { // NAME: proto-pdb:default.pdb
public:
    Bead2Main(SwampSceneNode* node);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Bead2Main, 0xD0);

// FUNCTION: USA 0x001266BB
// PROTOTYPE: PREVIEW 0x0050F070 identical
// NAME: proto-pdb:default.pdb
SwampScript* Bead2Main::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\Bead.h", 87) Bead2Main(node);
}

// FUNCTION: USA 0x00126D10
// NAME: proto-pdb:default.pdb
Bead2Main::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("Bead2Main", creationFunc);
}

class Bead3Main : public BeadMain { // NAME: proto-pdb:default.pdb
public:
    Bead3Main(SwampSceneNode* node);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Bead3Main, 0xD0);

// FUNCTION: USA 0x001266E2
// NAME: proto-pdb:default.pdb
SwampScript* Bead3Main::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\Bead.h", 96) Bead3Main(node);
}

// FUNCTION: USA 0x00126D4C
// NAME: proto-pdb:default.pdb
Bead3Main::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("Bead3Main", creationFunc);
}
