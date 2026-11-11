// UNIT: src/Apps/Vince/scripts/Crypt_city/RatRaceRodeoCutscenes.cpp OBJ: Xbox\OfficialRelease\RatRaceRodeoCutscenes.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string { // NAME: proto-pdb:default.pdb
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

    char field_0[0x1C];                // 0x00
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
DECOMP_SIZE_ASSERT(SwampString, 0x1C);

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
struct AABox;                // NAME: proto-pdb:default.pdb
class Matrix;                // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb
class AnimCam;               // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

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

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<AnimationResource>, 0x4);

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }
    ModelManager& GetModelManager() const { return *_modelManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x28 - 0x1C];        // 0x1C
    ModelManager* _modelManager;       // 0x28
    char field_2C[0x68 - 0x2C];        // 0x2C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class Locator { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetLocation() const { return _pos; }
    const Vector3& GetBasisVector(int i) const { return _basis[i]; }

    Vector3 _pos;                      // 0x00
    Quaternion _rot;                   // 0x0C
    Vector3 _basis[3];                 // 0x1C
    int _zone;                         // 0x40
    SwampScript* _script;              // 0x44
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    Locator* Find(const SwampString&) const;
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

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
    SwampScript* GetScript() const;
    void SetScript(SwampScript*);
    void SetOption(unsigned int, bool, bool);
    const Quaternion& SetFacing(const Vector3&);
};

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C
};

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    void SetEdgeGlow(bool b) { _edgeGlow = b; }

    char field_4[0xC5 - 0x4];          // 0x04
    bool _edgeGlow;                    // 0xC5
    char field_C6[0x200 - 0xC6];       // 0xC6
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

// Retail layout: Vince _whammyReady 0x434 -> 0x448 (+0x14).
class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetWhammyReady(bool);

    char field_200[0x448 - 0x200];     // 0x200
    bool _whammyReady;                // 0x448 (proto 0x434)
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

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

class CutsceneAnimationScript : public SwampScript { // NAME: proto-pdb:default.pdb
};

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished(unsigned int);                  // vtable+0x64
    virtual void AnimationTrigger(unsigned int, const SwampString&, float, float, unsigned int); // vtable+0x68
    static void BeginPlayback();
    static void MakeEvilVision(GatorNode*);
protected:
    virtual void Init();                                           // vtable+0x6C
    virtual void Cleanup();                                        // vtable+0x70
public:
    virtual void SetState(int);                                    // vtable+0x74
protected:
    AnimCam* NewCamera(const SwampString&);
    GatorNode* NewNode(unsigned int);
    CutsceneAnimationScript* NewScript(SwampSceneNode*, unsigned int);

    char field_4C[0x54 - 0x4C];        // 0x4C
    int _state;                        // 0x54
    char field_58[0x78 - 0x58];        // 0x58
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

class RatRacePreludeCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    enum Ids { // NAME: proto-pdb:default.pdb
        ID_VINCE = 0x1,
        ID_RAT = 0x2,
        ID_MRSG = 0x3,
        ID_HAT = 0x4,
        ID_KOSMO = 0x5
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_V_DROPSIN = 0x0,
        STATE_K_MRSG_ALIVE = 0x1,
        STATE_G_SCREAMS = 0x2
    };
    RatRacePreludeCutscene(SwampSceneNode* node);
protected:
    virtual void Init();                                           // vtable+0x6C
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);                              // vtable+0x74

    Vince* _vince;                     // 0x78
    Critter* _rat;                     // 0x7C
    Critter* _mrsg;                    // 0x80
    GatorNode* _hat;                   // 0x84
    GatorNode* _kosmo;                 // 0x88
    bool _wasWhammyReady;              // 0x8C
};
DECOMP_SIZE_ASSERT(RatRacePreludeCutscene, 0x90);

class RatRaceDeathCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    RatRaceDeathCutscene(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);                              // vtable+0x74

    Vince* _vince;                     // 0x78
    Critter* _rat;                     // 0x7C
    Critter* _mrsg;                    // 0x80
    GatorNode* _hat;                   // 0x84
    char field_88[0x8C - 0x88];        // 0x88
};
DECOMP_SIZE_ASSERT(RatRaceDeathCutscene, 0x8C);

// FUNCTION: USA 0x000688EA
// PROTOTYPE: PREVIEW 0x00454E4A near-identical
// NAME: proto-pdb:default.pdb
void RatRacePreludeCutscene::Init()
{
    CutsceneDriver::Init();
    Locator* l = g_locatorManager->Find("locator_ratraceprelude");
    _vince = pManager->GetVince();
    _vince->SetScript(NewScript(_vince, ID_VINCE));
    _vince->SetEdgeGlow(false);
    _vince->SetOption(2, false, false);
    _vince->SetWorldPosition(l->GetLocation());
    _vince->SetFacing(l->GetBasisVector(2));
    _wasWhammyReady = _vince->_whammyReady;
    _vince->SetWhammyReady(false);
    _rat = pManager->Find("rodeorat");
    _rat->SetScript(NewScript(_rat, ID_RAT));
    _rat->SetWorldPosition(l->GetLocation());
    _rat->SetFacing(l->GetBasisVector(2));
    _rat->SetOption(2, false, false);
    _mrsg = pManager->Find("mrsg");
    _mrsg->SetScript(NewScript(_mrsg, ID_MRSG));
    _hat = NewNode(ID_HAT);
    _hat->SetModel(g_swamp->GetModelManager().Load("cowboyHat_base"));
    _hat->SetWorldPosition(l->GetLocation());
    _hat->SetFacing(l->GetBasisVector(2));
    _kosmo = NewNode(ID_KOSMO);
    _kosmo->SetModel(g_swamp->GetModelManager().Load("kosmoHead_base"));
    MakeEvilVision(_kosmo);
    SetState(STATE_V_DROPSIN);
    BeginPlayback();
}

// FUNCTION: USA 0x00068C05
// PROTOTYPE: PREVIEW 0x00455165 identical
// NAME: proto-pdb:default.pdb
void RatRacePreludeCutscene::SetState(int state)
{
    switch (state) {
    case 0:
        NewCamera("camera_rrp01");
        _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_ratRacePrelude01"), false, false);
        _rat->StartAnimation(g_swamp->GetAnimationManager().Load("rodeoRat_ratRacePrelude01"), false, false);
        _hat->StartAnimation(g_swamp->GetAnimationManager().Load("cowboyHat_ratRacePrelude01"), false, false);
        _kosmo->StartAnimation(g_swamp->GetAnimationManager().Load("kosmoHead_RRRodeoIntro"), false, false);
        break;
    case 1:
        NewCamera("camera_rrp03");
        _mrsg->StartAnimation(g_swamp->GetAnimationManager().Load("mrsG_comeToLife"), false, false);
        break;
    case 2:
        NewCamera("camera_rrp04");
        _kosmo->SetOption(2, true, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x00069208
// PROTOTYPE: PREVIEW 0x00455738 identical
// NAME: proto-pdb:default.pdb
void RatRaceDeathCutscene::SetState(int state)
{
    switch (state) {
    case 0:
        NewCamera("camera_death01");
        _mrsg->GetScript()->Message(this, 0x19, 0);
        break;
    case 1:
        NewCamera("camera_death02");
        _vince->SetOption(2, false, false);
        _vince->StartAnimation(g_swamp->GetAnimationManager().Load("vince_ratRace_celebration"), false, false);
        _rat->SetOption(2, false, false);
        _rat->StartAnimation(g_swamp->GetAnimationManager().Load("rodeoRat_ratRace_celebration"), false, false);
        _hat->SetOption(2, false, false);
        _hat->StartAnimation(g_swamp->GetAnimationManager().Load("cowboyHat_ratRace_celebration"), false, false);
        break;
    }
    _state = state;
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x000693F4
// PROTOTYPE: PREVIEW 0x00455924 identical
// NAME: proto-pdb:default.pdb
SwampScript* RatRacePreludeCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("RatRaceRodeo\\RatRaceRodeoCutscenes.cpp", 19) RatRacePreludeCutscene(node);
}

// FUNCTION: USA 0x00069445
// NAME: proto-pdb:default.pdb
RatRacePreludeCutscene::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("RatRacePreludeCutscene", creationFunc);
}

// FUNCTION: USA 0x0006941B
// PROTOTYPE: PREVIEW 0x0045594B identical
// NAME: proto-pdb:default.pdb
SwampScript* RatRaceDeathCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("RatRaceRodeo\\RatRaceRodeoCutscenes.cpp", 191) RatRaceDeathCutscene(node);
}

// FUNCTION: USA 0x00069481
// NAME: proto-pdb:default.pdb
RatRaceDeathCutscene::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("RatRaceDeathCutscene", creationFunc);
}
