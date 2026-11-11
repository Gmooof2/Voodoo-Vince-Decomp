// UNIT: src/Apps/Vince/scripts/Bayou/JimmysFanboatCutscenes.cpp OBJ: Xbox\OfficialRelease\JimmysFanboatCutscenes.obj OPT: /O1 /Oy
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
struct Vector3;              // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
class Matrix;                // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Return(float);
};

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

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
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
    void SetOption(unsigned int, bool, bool);
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
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
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

class RacingOpponentMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_VINCELAP = 0x14,
        MESSAGE_OPPONENTLAP = 0x15,
        MESSAGE_SHOWTIMEDIFF = 0x16,
        MESSAGE_FINISHED_JVD = 0x17
    };
};

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished(unsigned int);                  // vtable+0x64
    virtual void AnimationTrigger(unsigned int, const SwampString&, float, float, unsigned int); // vtable+0x68
    static void SetStream(const SwampString&, bool);
    static void BeginPlayback();
protected:
    virtual void Init();                                           // vtable+0x6C
    virtual void Cleanup();                                        // vtable+0x70
public:
    virtual void SetState(int);                                    // vtable+0x74
protected:
    AnimCam* NewCamera(const SwampString&);
    GatorNode* NewNode(unsigned int);

    char field_4C[0x54 - 0x4C];        // 0x4C
    int _state;                        // 0x54
    char field_58[0x78 - 0x58];        // 0x58
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class JimmyVictoryDanceCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    JimmyVictoryDanceCutscene(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    enum Ids { // NAME: proto-pdb:default.pdb
        ID_JIMMY = 0x1,
        ID_RC_CONTROLLER = 0x2
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_J_DANCE = 0x0,
        STATE_LAST = 0x1
    };
protected:
    virtual void Init();                                           // vtable+0x6C
    virtual void Cleanup();                                        // vtable+0x70
private:
    virtual void SetState(int state);                              // vtable+0x74
    static SwampScript* creationFunc(SwampSceneNode* node);

    GatorNode* _jimmy;                 // 0x78
    GatorNode* _controller;            // 0x7C
};
DECOMP_SIZE_ASSERT(JimmyVictoryDanceCutscene, 0x80);

// FUNCTION: USA 0x00027458
// PROTOTYPE: PREVIEW 0x00416251 identical
// NAME: proto-pdb:default.pdb
void JimmyVictoryDanceCutscene::Init()
{
    CutsceneDriver::Init();
    _jimmy = NewNode(ID_JIMMY);
    _jimmy->SetModel(g_swamp->GetModelManager().Load("crawdadjimmy_base"));
    _jimmy->SetOption(2, false, false);
    _controller = NewNode(ID_RC_CONTROLLER);
    _controller->SetModel(g_swamp->GetModelManager().Load("rccontroller_base"));
    _controller->SetOption(2, false, false);
    SetState(STATE_J_DANCE);
    BeginPlayback();
}

// FUNCTION: USA 0x00027532
// PROTOTYPE: PREVIEW 0x0041632B identical
// NAME: proto-pdb:default.pdb
void JimmyVictoryDanceCutscene::Cleanup()
{
    CutsceneDriver::Cleanup();
    AnimCam::Return(0.0f);
    _jimmy->Invalidate();
    _controller->Invalidate();
    // !FAKE: c2 register allocation; stand-in local; the PDB records no local for the opponent (ebx in retail)
    Critter* p = pManager->Find("racingopponent");
    p->GetScript()->Message(this, RacingOpponentMain::MESSAGE_FINISHED_JVD, 0);
    SetStream("jimmyVictoryDance", false);
    InvalidateScript();
}

// FUNCTION: USA 0x000275CB
// PROTOTYPE: PREVIEW 0x004163C4 identical
// NAME: proto-pdb:default.pdb
void JimmyVictoryDanceCutscene::SetState(int state)
{
    switch (state) {
    case STATE_J_DANCE:
        NewCamera("camera_victoryDance");
        _jimmy->StartAnimation(g_swamp->GetAnimationManager().Load("crawdadJimmy_victoryDance"), false, false);
        _controller->StartAnimation(g_swamp->GetAnimationManager().Load("rcController_victoryDance"), false, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x000276BB
// NAME: proto-pdb:default.pdb
SwampScript* JimmyVictoryDanceCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("JimmysFanboat\\JimmysFanboatCutscenes.cpp", 19) JimmyVictoryDanceCutscene(node);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x000276E2
// NAME: proto-pdb:default.pdb
JimmyVictoryDanceCutscene::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("JimmyVictoryDanceCutscene", creationFunc);
}
