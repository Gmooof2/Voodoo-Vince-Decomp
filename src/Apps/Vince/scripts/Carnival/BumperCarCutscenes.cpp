// UNIT: src/Apps/Vince/scripts/Carnival/BumperCarCutscenes.cpp OBJ: Xbox\OfficialRelease\BumperCarCutscenes.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std {

template<class _Elem>
struct char_traits {};

template<class _Ty>
class allocator {};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    basic_string(const _Elem *_Ptr);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);
    char _pad00[0x1C];
};

}

struct AABox;                // NAME: proto-pdb:default.pdb
class Matrix;                // NAME: proto-pdb:default.pdb
class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb
template<class T>
class Ptr;                   // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion(float x, float y, float z, float _w) : v(x, y, z), w(_w) {}

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char _Name[0x1C];                  // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    T* _Resource;                      // 0x00
};

template<class T>
class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const std::basic_string<char>& name);
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

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
    void SetOption(unsigned int option, bool a, bool b);
    void SetScript(SwampScript* script);
    const Quaternion& SetRotation(const Quaternion&);
    const Quaternion& SetFacing(const Vector3&);
};

class RootNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    float GetFogDistance() const { return _fogDistance; }
    float GetFogDensity() const { return _fogDensity; }
    void SetFogDistance(float d) { _fogDistance = d; }
    void SetFogDensity(float d) { _fogDensity = d; }

    char field_4[0xA4 - 0x4];          // 0x04
    unsigned int _fogColor;            // 0xA4
    float _fogDistance;                // 0xA8
    float _fogDensity;                 // 0xAC
};
DECOMP_SIZE_ASSERT(RootNode, 0xB0);

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    RootNode* GetRootNode() const { return _root; }

    RootNode* _root;                   // 0x00
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampScene& GetScene() const { return *_scene; }
    ModelManager& GetModelManager() const { return *_modelManager; }

    char field_0[0xC];                 // 0x00
    SwampScene* _scene;                // 0x0C
    char field_10[0x28 - 0x10];        // 0x10
    ModelManager* _modelManager;       // 0x28
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

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
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

// Retail layout: Vince _whammyReady 0x434 -> 0x448 (+0x14).
class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetWhammyReady(bool);

    char field_4[0x448 - 0x4];         // 0x04
    bool _whammyReady;                 // 0x448 (proto 0x434)
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const std::basic_string<char>&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

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
    Locator* Find(const std::basic_string<char>& s) const;
};

extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

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
    virtual void AnimationTrigger(const std::basic_string<char>&, float, float, unsigned int); // vtable+0x28
    virtual bool LosingControl(const std::basic_string<char>&);    // vtable+0x2C
    virtual void GainingControl(SwampScript*);                     // vtable+0x30
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    virtual void EnteringRegion(SwampSceneNode*);                  // vtable+0x38
    virtual void LeavingRegion(SwampSceneNode*);                   // vtable+0x3C
    virtual void SoundFinished(const std::basic_string<char>&);    // vtable+0x40
    virtual void SoundStreamerFinished(const std::basic_string<char>&); // vtable+0x44
    virtual void MovieFinished(const std::basic_string<char>&);    // vtable+0x48
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x50
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual std::basic_string<char> Save();                        // vtable+0x58
    virtual void SetNode(SwampSceneNode*);                         // vtable+0x5C
    virtual void InvalidateScript();                               // vtable+0x60

    char field_4[0x4C - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CutsceneAnimationScript : public SwampScript { // NAME: proto-pdb:default.pdb
};

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished(unsigned int id);               // vtable+0x64
    virtual void AnimationTrigger(unsigned int id, const std::basic_string<char>& name, float value, float prev, unsigned int bone); // vtable+0x68
    static void BeginPlayback();

protected:
    virtual void Init();                                           // vtable+0x6C
    virtual void Cleanup();                                        // vtable+0x70
public:
    virtual void SetState(int);                                    // vtable+0x74

protected:
    void StateTransition(unsigned int id, int state);
    GatorNode* NewNode(unsigned int id);
    CutsceneAnimationScript* NewScript(SwampSceneNode* node, unsigned int id);

public:
    char field_4C[0x78 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

class KosmobotPreludeCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    enum Ids { // NAME: proto-pdb:default.pdb
        ID_VINCE = 0x1,
        ID_BOT_LEGS = 0x2,
        ID_BOT_MIDRIFF = 0x3,
        ID_BOT_HEAD = 0x4,
        ID_CHOPPER = 0x5,
        ID_KOSMO = 0x6,
        ID_JEB = 0x7,
        ID_FINGERS = 0x8,
        ID_EGG = 0x9,
        ID_LIGHT1 = 0xA,
        ID_LIGHT2 = 0xB
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_V_FLYSIN = 0x0,
        // 0x1..0xF: not used in this unit's bodies
        STATE_LAST = 0x10
    };
    KosmobotPreludeCutscene(SwampSceneNode* node);
    virtual void AnimationFinished(unsigned int id);
protected:
    virtual void Init();
private:
    virtual void SetState(int state);
    static SwampScript* creationFunc(SwampSceneNode* node);
public:
    Vince* _vince;                     // 0x78
    Critter* _legs;                    // 0x7C
    Critter* _midriff;                 // 0x80
    Critter* _head;                    // 0x84
    GatorNode* _chopper;               // 0x88
    GatorNode* _kosmo;                 // 0x8C
    GatorNode* _jeb;                   // 0x90
    GatorNode* _fingers;               // 0x94
    GatorNode* _egg;                   // 0x98
    GatorNode* _light1;                // 0x9C
    GatorNode* _light2;                // 0xA0
    bool _wasWhammyReady;              // 0xA4
    float _fogDist;                    // 0xA8
    float _fogDensity;                 // 0xAC
};
DECOMP_SIZE_ASSERT(KosmobotPreludeCutscene, 0xB0);

// FUNCTION: USA 0x0004EAC8
// PROTOTYPE: PREVIEW 0x0043BA01 identical
// NAME: proto-pdb:default.pdb
void KosmobotPreludeCutscene::AnimationFinished(unsigned int id)
{
    StateTransition(id, STATE_LAST);
}

// FUNCTION: USA 0x0004EAD6
// PROTOTYPE: PREVIEW 0x0043BA0F near-identical
// NAME: proto-pdb:default.pdb
void KosmobotPreludeCutscene::Init()
{
    CutsceneDriver::Init();
    _vince = pManager->GetVince();
    _vince->SetScript(NewScript(_vince, ID_VINCE));
    _vince->SetWorldPosition(Vector3(0.0f, 0.0f, 0.0f));
    _vince->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
    _vince->SetOption(2, false, false);
    _wasWhammyReady = _vince->_whammyReady;
    _vince->SetWhammyReady(false);
    _legs = pManager->Find("kosmoBotLegs");
    _legs->SetScript(NewScript(_legs, ID_BOT_LEGS));
    _midriff = pManager->Find("kosmoBotMidriff");
    _midriff->SetScript(NewScript(_midriff, ID_BOT_MIDRIFF));
    _head = pManager->Find("kosmoBotHead");
    _head->SetScript(NewScript(_head, ID_BOT_HEAD));
    _chopper = NewNode(ID_CHOPPER);
    _chopper->SetModel(g_swamp->GetModelManager().Load("chopper_base"));
    _chopper->SetOption(2, false, false);
    _kosmo = NewNode(ID_KOSMO);
    _kosmo->SetModel(g_swamp->GetModelManager().Load("kosmo_base"));
    _kosmo->SetOption(2, true, false);
    _jeb = NewNode(ID_JEB);
    _jeb->SetModel(g_swamp->GetModelManager().Load("jeb_base"));
    _jeb->SetOption(2, true, false);
    _fingers = NewNode(ID_FINGERS);
    _fingers->SetModel(g_swamp->GetModelManager().Load("fingers_base"));
    _fingers->SetOption(2, true, false);
    Locator* l = g_locatorManager->Find("locator_placefuckingegghere");
    _egg = NewNode(ID_EGG);
    _egg->SetModel(g_swamp->GetModelManager().Load("kosmoegg_base"));
    _egg->SetOption(2, true, false);
    _egg->SetWorldPosition(l->GetLocation());
    _egg->SetFacing(l->GetBasisVector(2));
    _light1 = NewNode(ID_LIGHT1);
    _light1->SetModel(g_swamp->GetModelManager().Load("kosmoPreludeLights_base"));
    _light2 = NewNode(ID_LIGHT2);
    _light2->SetModel(g_swamp->GetModelManager().Load("kosmoPreludeLights_base"));
    // !FAKE: c2 common subexpression; dummy local; root fetched once, chained accessors reload g_swamp (match_one 95.86 -> 100.0)
    RootNode* p = g_swamp->GetScene().GetRootNode();
    _fogDist = p->GetFogDistance();
    _fogDensity = p->GetFogDensity();
    p->SetFogDistance(_fogDist * 4.0f);
    p->SetFogDensity(_fogDensity * 0.25f);
    SetState(STATE_V_FLYSIN);
    BeginPlayback();
}

// FUNCTION: USA 0x0004FAB3
// PROTOTYPE: PREVIEW 0x0043C9EC identical
// NAME: proto-pdb:default.pdb
SwampScript* KosmobotPreludeCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BumperCarBumpoff\\BumperCarCutscenes.cpp", 19) KosmobotPreludeCutscene(node);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const std::basic_string<char>& name, CreationFunc func);
};

// FUNCTION: USA 0x0004FADA
// NAME: proto-pdb:default.pdb
KosmobotPreludeCutscene::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("KosmobotPreludeCutscene", creationFunc);
}
