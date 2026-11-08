// UNIT: src/Apps/Vince/scripts/Bayou/FanHurricane.cpp OBJ: Xbox\OfficialRelease\FanHurricane.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

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
struct Vector3;              // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

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

    char field_4[0x4];                 // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x80];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

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
    void RegisterFrameTick();
    void UnRegisterFrameTick();

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
protected:
    virtual void SetState(int);                                    // vtable+0x6C
public:

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle& operator=(const ResourceHandle& other);
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
    void SetAnimSpeed(float speed);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
    char field_4[0x11C];               // 0x04
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x10];                // 0x08
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Invalidate();
    void SetPitch(float);
    void Play();
    float GetPitch() const { return _pitch; }

    char field_A4[0x10];               // 0xA4
    float _pitch;                      // 0xB4
    char field_B8[0x18];               // 0xB8
};
DECOMP_SIZE_ASSERT(SoundNode, 0xD0);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static SoundNode* CreateSound(const SwampString&, unsigned int, SwampSceneNode*, bool);
};

// FUNCTION: USA 0x0002B4E8
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0002B504
// PROTOTYPE: PREVIEW 0x0041A028 identical
// NAME: proto-pdb:default.pdb
class FanHurricaneMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_OFF = 0,
        STATE_ON = 1
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_TURNOFF = 0x14,
        MESSAGE_QUERYON = 0x15
    };

    FanHurricaneMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void FrameTick();
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _animRun; // 0xBC
    float _runSpeed;                   // 0xC0
    SwampString _switchName;           // 0xC4
    SoundNode* _loopSound;             // 0xE0
};
DECOMP_SIZE_ASSERT(FanHurricaneMain, 0xE4);

// FUNCTION: USA 0x0002B465
// PROTOTYPE: PREVIEW 0x00419F89 identical
// NAME: proto-pdb:default.pdb
FanHurricaneMain::FanHurricaneMain(SwampSceneNode* node) : CritterMain(node), _loopSound(0)
{
    _animRun = g_swamp->GetAnimationManager()->Load("fanHurricane_on");
    _runSpeed = 0.0f;
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1FanHurricaneMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_FanHurricaneMain_dtor(FanHurricaneMain* p)
{
    p->FanHurricaneMain::~FanHurricaneMain();
}

// FUNCTION: USA 0x0002B52B
// PROTOTYPE: PREVIEW 0x0041A04F identical
// NAME: proto-pdb:default.pdb
void FanHurricaneMain::Activate()
{
    ((GatorNode*)_node)->StartAnimation(_animRun, true, false);
    ((GatorNode*)_node)->SetAnimSpeed(0.0f);
    _loopSound = SoundManager::CreateSound("bhh_fbrun", 9, GetNode(), true);
    _loopSound->SetPitch(-2.0f);
    _loopSound->Play();
    SetState(STATE_ON);
    RegisterFrameTick();
}

// FUNCTION: USA 0x0002B5BB
// PROTOTYPE: PREVIEW 0x0041A0DF identical
// NAME: proto-pdb:default.pdb
void FanHurricaneMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 7:
        _switchName = source->_node->_Name;
        break;
    case MESSAGE_TURNOFF:
        SetState(STATE_OFF);
        pManager->Find(_switchName)->GetScript()->Message(this, 0x14, 0);
        break;
    case MESSAGE_QUERYON:
        *(bool*)data = _state == STATE_ON;
        break;
    }
    CritterMain::Message(source, message, data);
}

// FUNCTION: USA 0x0002B63D
// PROTOTYPE: PREVIEW 0x0041A161 identical
// NAME: proto-pdb:default.pdb
void FanHurricaneMain::FrameTick()
{
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    switch (_state) {
    case STATE_OFF:
        if (_runSpeed > 0.0f) {
            _loopSound->SetPitch((_loopSound->GetPitch() - elapsed < -2.0f) ? -2.0f : _loopSound->GetPitch() - elapsed);
            if ((_runSpeed -= elapsed) <= 0.0f) {
                _runSpeed = 0.0f;
                ((GatorNode*)_node)->StartAnimation(ResourceHandle<AnimationResource>(), false, false);
                UnRegisterFrameTick();
                _loopSound->Invalidate();
                _loopSound = 0;
            }
            ((GatorNode*)_node)->SetAnimSpeed(_runSpeed);
        }
        break;
    case STATE_ON:
        if (_runSpeed < 1.0f) {
            _runSpeed += elapsed;
            if (_runSpeed >= 1.0f) {
                _runSpeed = 1.0f;
                pManager->Find("hannah")->GetScript()->Message(this, MESSAGE_TURNOFF, 0);
            }
            ((GatorNode*)_node)->SetAnimSpeed(_runSpeed);
        }
        _loopSound->SetPitch((elapsed + _loopSound->GetPitch() > -1.0f) ? -1.0f : elapsed + _loopSound->GetPitch());
        break;
    }
}

// FUNCTION: USA 0x0002B7CD
// PROTOTYPE: PREVIEW 0x0041A2F1 identical
// NAME: proto-pdb:default.pdb
SwampScript* FanHurricaneMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\hurricanehanna\\FanHurricane.h", 6) FanHurricaneMain(node);
}
