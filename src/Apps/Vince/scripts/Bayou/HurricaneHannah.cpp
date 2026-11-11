// UNIT: src/Apps/Vince/scripts/Bayou/HurricaneHannah.cpp OBJ: Xbox\OfficialRelease\HurricaneHannah.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* ptr);
    basic_string(const basic_string& _Right, unsigned int _Roff, unsigned int _Count = (unsigned int)(-1)); // npos (C0215)
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);

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
basic_string<E, T, A>::basic_string(const basic_string& _Right, unsigned int _Roff, unsigned int _Count)
{
    _Myres = _BUF_SIZE - 1; // _Tidy() body inline (C0144)
    _Eos(0);
    assign(_Right, _Roff, _Count);
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

// FUNCTION: USA 0x00027966
// PROTOTYPE: PREVIEW 0x00416755 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string<char,std::char_traits<char>,std::allocator<char> >(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,unsigned int,unsigned int);

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
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
    ResourceHandle() : _Resource(0) {}
    ResourceHandle& operator=(const ResourceHandle& h);
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    void Translation(const Vector3&);
    void Multiply(const Matrix&, Matrix&) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

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

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1,
        DontDraw = 0x2
    };

    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    const SwampString& GetName() const { return _Name; }
    bool IsVisible() const { return !GetRenderingOption(DontDraw); }
    void SetOption(unsigned int option, bool value, bool recursive);
    bool GetRenderingOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    void SetScript(const SwampString& name);

    char field_4[0x4];                 // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x64];               // 0x24
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0x18];               // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x8];                // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0x8];                // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void Render(const Matrix&) const;
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool looped, bool reverse);
    void SetAnimSpeed(float speed);
    float GetAnimSpeed();

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0x2C8 - 0x200];     // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class HannahDebris : public Critter { // NAME: proto-pdb:default.pdb
public:
    virtual void Render(const Matrix& m) const;

    float _gravity;                    // 0x2C8
};
DECOMP_SIZE_ASSERT(HannahDebris, 0x2CC);

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& s);
    void Remove(const SwampString& s);

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
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x10];                // 0x08
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_GAMESTART = 0x6,
    MESSAGE_WHOSYOURDADDY = 0x7
};

class HannahDebrisMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_RESETCRASH = 0
    };

    HannahDebrisMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    char field_BC[0x1C];               // 0xBC
    bool _okToCrash;                   // 0xD8
    float _timeTilNextFlip;            // 0xDC
};
DECOMP_SIZE_ASSERT(HannahDebrisMain, 0xE0);

class LighthouseMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    LighthouseMain(SwampSceneNode* node);
    virtual void FrameTick();
    virtual bool TestActivate() const;
    virtual void Activate();
    virtual void Message(SwampScript* source, int message, void* data);

    ResourceHandle<AnimationResource> _animSpin; // 0xBC
    SwampString _padName;              // 0xC0
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(LighthouseMain, 0xDC);

class VinceWindmillGibletsMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    VinceWindmillGibletsMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(VinceWindmillGibletsMain, 0x4C);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x110 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class EntranceWallMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    EntranceWallMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(EntranceWallMain, 0x110);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode*);
    bool InRegion() const { return _inRegion; }

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_4E[0x64 - 0x4E];        // 0x4E
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class HurricaneHannahRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    HurricaneHannahRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(HurricaneHannahRegionMain, 0x64);

class Locator { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const { return _script; }

    char field_0[0x44];                // 0x00
    SwampScript* _script;              // 0x44
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    Locator* Find(const SwampString& s) const;
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

class CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    static void SetStream(const SwampString& wav, bool looped);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
double RandomFloat(double range);
}

void* operator new(unsigned int size, const char* file, int line);

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x13C - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class SwitchSwampMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
    SwitchSwampMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(SwitchSwampMain, 0x13C);

// FUNCTION: USA 0x0002771E
// PROTOTYPE: PREVIEW 0x00416517 identical
// NAME: proto-pdb:default.pdb
void HannahDebrisMain::TimerEvent(unsigned int id)
{
    if (id == TIMER_RESETCRASH) {
        _okToCrash = true;
    }
}

// FUNCTION: USA 0x0002772F
// PROTOTYPE: PREVIEW 0x00416528 identical
// NAME: proto-pdb:default.pdb
double BeepLib::RandomFloat(double range)
{
    return (double)Random() * range * (1.0 / 4294967296.0);
}

// FUNCTION: USA 0x0002777C
// PROTOTYPE: PREVIEW 0x00416575 identical
// NAME: proto-pdb:default.pdb
bool LighthouseMain::TestActivate() const
{
    return !_node->GetRenderingOption(SwampSceneNode::DontDraw);
}

// FUNCTION: USA 0x000277AF
// PROTOTYPE: PREVIEW 0x004165A8 identical
// NAME: proto-pdb:default.pdb
void HannahDebris::Render(const Matrix& m) const
{
    Matrix trans;
    trans.Translation(Vector3(0.0f, 0.5f, 0.0f));
    // !FAKE: inliner; dummy locals v0, v1; hand form of the inlined by-value product, the PDB lists only m, trans
    // (match_one 100.0 first draft; one temp only 59.08 per docs/evidence/inline_helpers.md:140)
    Matrix v0;
    m.Multiply(trans, v0);
    Matrix v1(v0);
    GatorNode::Render(v1);
}

// FUNCTION: USA 0x00027990
// PROTOTYPE: PREVIEW 0x0041677F identical
// NAME: proto-pdb:default.pdb
SwampScript* HannahDebrisMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\HurricaneHannah.cpp", 109) HannahDebrisMain(node);
}

// FUNCTION: USA 0x000279B7
// PROTOTYPE: PREVIEW 0x004167A6 identical
// NAME: proto-pdb:default.pdb
void HurricaneHannahRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (!InRegion()) {
        LevelScript::EnteringRegion(node);
        bool didflythru = false;
        // !FAKE: c1xx front end; neutral stand-in local; retail ends the name temp before Message (no PDB local)
        Locator* v1 = g_locatorManager->Find("locator_loadFromCrawdadJimmys");
        v1->GetScript()->Message(this, 0x19, &didflythru); // src proto-pdb:default.pdb MESSAGE_QUERYFLYTHROUGH
        CutsceneDriver::SetStream(didflythru ? "hannaDeath" : "hannaPrelude", false);
    }
}

// FUNCTION: USA 0x000282AB
// NAME: proto-pdb:default.pdb
LighthouseMain::LighthouseMain(SwampSceneNode* node) : CritterMain(node)
{
    _animSpin = g_swamp->GetAnimationManager().Load("lighthouse_spin");
}

// FUNCTION: USA 0x0002831F
// PROTOTYPE: PREVIEW 0x00417152 identical
// NAME: proto-pdb:default.pdb
void LighthouseMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_GAMESTART:
        if (pManager->Find("hannah"))
            _node->SetOption(SwampSceneNode::DontDraw, true, false);
        ((GatorNode*)_node)->StartAnimation(_animSpin, true, false);
        ((GatorNode*)_node)->SetAnimSpeed(0.0f);
        RegisterFrameTick();
        break;
    case MESSAGE_WHOSYOURDADDY:
        _padName = source->_node->GetName();
        break;
    }
}

// FUNCTION: USA 0x000283BD
// PROTOTYPE: PREVIEW 0x004171F0 identical
// NAME: proto-pdb:default.pdb
void LighthouseMain::FrameTick()
{
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    float speed = ((GatorNode*)_node)->GetAnimSpeed();
    if (!_node->IsVisible()) {
        speed = 0.0f;
    } else if (speed < 2.0f) {
        speed += elapsed;
        if (speed >= 2.0f)
            speed = 2.0f;
    }
    if (!pManager->Find("hannah")) {
        pManager->Remove(_padName);
        _padName = "";
        UnRegisterFrameTick();
    }
    ((GatorNode*)_node)->SetAnimSpeed(speed);
}

// FUNCTION: USA 0x0002848D
// PROTOTYPE: PREVIEW 0x004172C0 identical
// NAME: proto-pdb:default.pdb
void LighthouseMain::Activate()
{
    pManager->_vinceCache->SetScript("vinceinwindmill");
}

// FUNCTION: USA 0x000284BF
// PROTOTYPE: PREVIEW 0x004172F2 identical
// NAME: proto-pdb:default.pdb
SwampScript* SwitchSwampMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\HurricaneHannah.cpp", 59) SwitchSwampMain(node);
}

// FUNCTION: USA 0x00028569
// NAME: proto-pdb:default.pdb
SwampScript* VinceWindmillGibletsMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\HurricaneHannah.cpp", 371) VinceWindmillGibletsMain(node);
}

// FUNCTION: USA 0x00028590
// NAME: proto-pdb:default.pdb
SwampScript* LighthouseMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\HurricaneHannah.cpp", 409) LighthouseMain(node);
}

// FUNCTION: USA 0x00028636
// NAME: proto-pdb:default.pdb
SwampScript* EntranceWallMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\HurricaneHannah.cpp", 499) EntranceWallMain(node);
}

// FUNCTION: USA 0x00028660
// NAME: proto-pdb:default.pdb
SwampScript* HurricaneHannahRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\HurricaneHannah.cpp", 25) HurricaneHannahRegionMain(node);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x00028684
// NAME: proto-pdb:default.pdb
HurricaneHannahRegionMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("HurricaneHannahRegionMain", creationFunc);
}

// FUNCTION: USA 0x000286C0
// NAME: proto-pdb:default.pdb
SwitchSwampMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("SwitchSwampMain", creationFunc);
}

// FUNCTION: USA 0x00028738
// NAME: proto-pdb:default.pdb
HannahDebrisMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("HannahDebrisMain", creationFunc);
}

// FUNCTION: USA 0x000287B0
// NAME: proto-pdb:default.pdb
VinceWindmillGibletsMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VinceWindmillGibletsMain", creationFunc);
}

// FUNCTION: USA 0x000287EC
// NAME: proto-pdb:default.pdb
LighthouseMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("LighthouseMain", creationFunc);
}

// FUNCTION: USA 0x00028828
// NAME: proto-pdb:default.pdb
EntranceWallMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("EntranceWallMain", creationFunc);
}
