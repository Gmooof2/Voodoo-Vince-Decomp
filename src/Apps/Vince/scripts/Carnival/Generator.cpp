// UNIT: src/Apps/Vince/scripts/Carnival/Generator.cpp OBJ: Xbox\OfficialRelease\Generator.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

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
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct Vector2 { // NAME: proto-pdb:default.pdb
    void Set(float u_, float v_) { u = u_; v = v_; }

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Vector3;               // NAME: proto-pdb:default.pdb
class BeepTDF;                // NAME: proto-pdb:default.pdb
class CollisionResultObject;  // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class SoundManager; // NAME: proto-pdb:default.pdb

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SoundManager& GetSoundManager() const { return *_soundManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x34 - 0x8];          // 0x08
    SoundManager* _soundManager;       // 0x34
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }
    const SwampString& GetName() const { return _Name; }

    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationSpeed(float);
    float GetAnimationSpeed() const { return _speed; }

    char field_24[0x2C - 0x24];        // 0x24
    float _speed;                      // 0x2C
    char field_30[0x68 - 0x30];        // 0x30
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    T& operator()() { return *_Resource; }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    T* Ptr() { return _Resource; }
    ResourceHandle<T>& operator=(const ResourceHandle<T>& h)
    {
        if (_Resource)
            _Resource->SubRef();
        _Resource = h._Resource;
        if (_Resource)
            _Resource->AddRef();
        return *this;
    }

    T* _Resource;                      // 0x00
};

class SoundResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class SoundNode { // NAME: proto-pdb:default.pdb
public:
    void SetPitch(float);
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl> GetCurrentAnimation() const { return _currentAnim; }
    void SetUVScrollSpeed(float u, float v) { _uvScrollingSpeed.Set(u, v); }

    char field_0[0x140];               // 0x00
    ResourceHandle<AnimationControl> _currentAnim; // 0x140
    char field_144[0x1F8 - 0x144];     // 0x144
    Vector2 _uvScrollingSpeed;         // 0x1F8
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class SoundManager : public ResourceManager<SoundResource> { // NAME: proto-pdb:default.pdb
public:
    static SoundNode* CreateSound(const ResourceHandle<SoundResource>&, const SwampString&, unsigned int,
                                  SwampSceneNode*, bool);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    SoundNode* CreateSound(ResourceHandle<SoundResource> sh, unsigned int flags, bool bAttach);
    void PlaySound3d(ResourceHandle<SoundResource>, unsigned int, bool, float, float);
};

class SwampScript { // NAME: proto-pdb:default.pdb
public:
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
    virtual void CollidedWith(int, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void HitBy(int, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x50
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual SwampString Save();                                    // vtable+0x58
    virtual void SetNode(SwampSceneNode*);                         // vtable+0x5C
    virtual void InvalidateScript();                               // vtable+0x60

    void UnRegisterFrameTick();

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class GeneratorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_TURNON = 0x14,
        MESSAGE_TURNOFF = 0x15,
        MESSAGE_SETDELAY = 0x16
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0x0,
        STATE_SPEEDING_UP = 0x1,
        STATE_RUNNING = 0x2,
        STATE_SLOWING_DOWN = 0x3
    };
    GeneratorMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void FrameTick();
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void SetState(int);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:
    char field_BC[0x4];                // 0xBC
    float _delay;                      // 0xC0
    SoundNode* _sound;                 // 0xC4
};
DECOMP_SIZE_ASSERT(GeneratorMain, 0xC8);

// FUNCTION: USA 0x0003B042
// PROTOTYPE: PREVIEW 0x00429739 identical
// NAME: proto-pdb:default.pdb
void GeneratorMain::Activate()
{
    Message(this, MESSAGE_TURNON, 0);
}

// FUNCTION: USA 0x0003B04D
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; explicit member instantiation; retail emits this template member through a caller in this
// unit (not matched yet); the instantiation emits it instead
template ResourceHandle<AnimationControl>& ResourceHandle<AnimationControl>::operator=(
    const ResourceHandle<AnimationControl>&);

// FUNCTION: USA 0x0003B070
// PROTOTYPE: PREVIEW 0x00429767 identical
// NAME: proto-pdb:default.pdb
void GeneratorMain::FrameTick()
{
    switch (_state) {
    case STATE_SPEEDING_UP:
    case STATE_SLOWING_DOWN: {
        float speed;
        // !FAKE: c2 register allocation; dummy locals; elapsed time, node, handle copy and current speed held
        // in st(1)/edi/ebx/st(0) across the body (match_one 83.21 without them)
        float v1 = g_swamp->_input->GetElapsedTime();
        GatorNode* p = (GatorNode*)_node;
        ResourceHandle<AnimationControl> tmp = p->GetCurrentAnimation();
        float v2 = tmp().GetAnimationSpeed();
        if (_state == STATE_SPEEDING_UP) {
            speed = v2 + v1 / _delay;
            if (speed > 1.0f) {
                speed = 1.0f;
                SetState(STATE_RUNNING);
            }
            tmp().SetAnimationSpeed(speed);
            p->SetUVScrollSpeed(speed, speed);
        } else {
            speed = v2 - v1 / _delay;
            if (speed <= 0.0f) {
                SetState(STATE_IDLE);
            } else {
                tmp().SetAnimationSpeed(speed);
                p->SetUVScrollSpeed(speed, speed);
            }
        }
        if (_sound)
            _sound->SetPitch(speed - 1.25f);
        break;
    }
    default:
        UnRegisterFrameTick();
        break;
    }
}

// FUNCTION: USA 0x0003B186
// PROTOTYPE: PREVIEW 0x0042987D identical
// NAME: proto-pdb:default.pdb
SoundNode* Critter::CreateSound(ResourceHandle<SoundResource> sh, unsigned int flags, bool bAttach)
{
    return SoundManager::CreateSound(sh, sh.Ptr() ? sh.Ptr()->GetName() : SwampString(""), flags, this, bAttach);
}

// FUNCTION: USA 0x0003B1E7
// PROTOTYPE: PREVIEW 0x004298DE identical
// NAME: proto-pdb:default.pdb
void GeneratorMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_TURNON:
        if (_state != STATE_RUNNING && _state != STATE_SPEEDING_UP) {
            ResourceHandle<SoundResource> sh = g_swamp->GetSoundManager().Load("chm_genact");
            if (!sh.Ptr())
                sh = g_swamp->GetSoundManager().Load("css_genact");
            ((Critter*)_node)->PlaySound3d(sh, 0x20, true, 0.0f, 0.0f);
            SetState(STATE_SPEEDING_UP);
        }
        break;
    case MESSAGE_TURNOFF:
        if (_state != STATE_IDLE && _state != STATE_SLOWING_DOWN)
            SetState(STATE_SLOWING_DOWN);
        break;
    case MESSAGE_SETDELAY:
        _delay = *(float*)data;
        break;
    }
}

// FUNCTION: USA 0x0003B53D
// PROTOTYPE: PREVIEW 0x00429C35 identical
// NAME: proto-pdb:default.pdb
SwampScript* GeneratorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\Generator.h", 11) GeneratorMain(node);
}
