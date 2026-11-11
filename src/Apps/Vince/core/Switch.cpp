// UNIT: src/Apps/Vince/core/Switch.cpp OBJ: core\XBox\OfficialRelease\Switch.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);
    bool empty() const { return _Mysize == 0; }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x14];
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class T, class A = allocator<T> > class vector { // NAME: proto-pdb:default.pdb
public:
    typedef T* iterator;
    // !FAKE: inliner; hand form of the VC7.1 body; vector() { _Buy(0); } folds to these three stores (C0191)
    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; }
    ~vector() { _Tidy(); }
    iterator begin() { return _Myfirst; }
    iterator end() { return _Mylast; }
    unsigned int size() const { return _Myfirst == 0 ? 0 : _Mylast - _Myfirst; }
    bool empty() const { return size() == 0; }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct AABox;      // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                         // vtable+0x00
    virtual const AABox& GetBounds() const;            // vtable+0x04
    virtual Vector3 GetWorldPosition() const;          // vtable+0x08
    virtual Quaternion GetWorldRotation() const;       // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);     // vtable+0x10

    const Quaternion& SetFacing(const Vector3& dir);
    void SetScript(const SwampString& name);
    unsigned int GetZone() const;
    class SwampScript* GetScript() const;

    int _Type;                         // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x80];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x8];                // 0x24
    float _length;                     // 0x2C
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    bool IsValid() const { return _Resource != 0; }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationSpeed(float speed);
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim._Resource->_length; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x10];               // 0x28
    float _time;                       // 0x38
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
    void SetAnimSpeed(float speed);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

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

    void RemoveTimerEvents();
    void RegisterTimerEvent(unsigned int id, float time, bool repeat);

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
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
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    SwitchMain(SwampSceneNode* node);
    virtual ~SwitchMain();
    virtual bool TestActivate() const;
    virtual void Activate();
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void TimerEvent(unsigned int timerId);

    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_TURN_OFF = 0x14,
        MESSAGE_TURN_ON = 0x15,
        MESSAGE_SET_ONEWAYTHROW = 0x16,
        MESSAGE_QUERYSTATE = 0x17,
        MESSAGE_SWITCHSPECIFIC = 0x18
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_OFF = 0,
        STATE_TURNINGON = 1,
        STATE_ON = 2,
        STATE_TURNINGOFF = 3
    };

    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_TURNOFF = 0,
        TIMER_SWITCHSPECIFIC = 1
    };

protected:
    bool NotifyTarget(int message);
    bool ActivateTarget();
public:
    void ResetPopupTimer();
    void SetOnAnimName(const SwampString& name);
    void SetOffAnimName(const SwampString& name);

    SwampString _onAnimName;           // 0xBC
    SwampString _offAnimName;          // 0xD8
    ResourceHandle<AnimationResource> _onAnim; // 0xF4
    ResourceHandle<AnimationResource> _offAnim; // 0xF8
    char field_FC[0x100 - 0xFC];       // 0xFC
    SwampString _activatorName;        // 0x100
    SwampString _target;               // 0x11C
    bool _popupOK;                     // 0x138
    bool _oneWayThrow;                 // 0x139
    bool _multiTarget;                 // 0x13A
    bool field_13B;                    // 0x13B retail only: tested by the retail Message, absent in the proto PDB
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
public:
    BasicWallSwitchMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void AnimationFinished();
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class FloorSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
public:
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(FloorSwitchMain, 0x13C);

class CarriedObject : public Critter { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x2E9 - 0xA4];       // 0xA4
    bool _wearable;                    // 0x2E9
};

class Boomerang : public CarriedObject { // NAME: proto-pdb:default.pdb
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x2FB - 0xA4];       // 0xA4
    bool _readyToUseObject;            // 0x2FB
    char field_2FC[0x4];               // 0x2FC
    CarriedObject* _carriedObject;     // 0x300
    Boomerang* _boomerangPin;          // 0x304
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
    void FindAllMatching(std::vector<Critter*>& matches, const SwampString& name, unsigned int zone);

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0010F1A2
// PROTOTYPE: PREVIEW 0x004F9AC0 near-identical
// NAME: proto-pdb:default.pdb
bool SwitchMain::TestActivate() const
{
    if (_state != STATE_OFF)
        return false;
    CarriedObject* obj = pManager->_vinceCache->_carriedObject;
    if (obj == 0 || obj->_wearable || obj == pManager->_vinceCache->_boomerangPin)
        return true;
    return false;
}

// FUNCTION: USA 0x0010F1D1
// PROTOTYPE: PREVIEW 0x004F9AEF identical
// NAME: proto-pdb:default.pdb
void SwitchMain::Activate()
{
    if (TestActivate()) {
        SetState(STATE_TURNINGON);
        ((GatorNode*)_node)->StartAnimation(_onAnim, false, false);
    }
}

// FUNCTION: USA 0x0010F1FB
// PROTOTYPE: PREVIEW 0x004F9B19 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100, bytes match; gate relink REFUSED (our ranges overlap at 0x0007EA16: ResetPopupTimer@SwitchMain copy vs SwitchSafe row; else external in replaced range, C0154)
void FloorSwitchMain::HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr)
{
    if (type == VerticalCollision && _popupOK && critterHitBy == pManager->_vinceCache) {
        if (_state == STATE_ON) {
            RemoveTimerEvents();
            ResetPopupTimer();
        }
        if (_state == STATE_OFF)
            Activate();
    }
}
#endif

// SwitchMain::ResetPopupTimer: retail folds it (ICF) into SwitchSafeMain::ResetPopupTimer at 0x0007EA16, the
// SwitchSafe.cpp row; proto_map lists Switch.obj among its modules. This byte-equal copy lands at that VA as a fold
// duplicate in relink (C0233 Fix 1), so the replaced SwitchSafe range no longer refuses this unit's external.
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: byte-equal copy; gate relink REFUSED (our ranges overlap at 0x0007EA16 with the matched SwitchSafeMain row)
void SwitchMain::ResetPopupTimer()
{
    RegisterTimerEvent(TIMER_TURNOFF, 1.0f, false);
}
#endif

// FUNCTION: USA 0x0010F36E
// NAME: proto-pdb:default.pdb
BasicWallSwitchMain::BasicWallSwitchMain(SwampSceneNode* node) : SwitchMain(node)
{
    SetOnAnimName("wallSwitch_down");
    SetOffAnimName("");
}

// FUNCTION: USA 0x0010F3D4
// PROTOTYPE: PREVIEW 0x004F9CC3 near-identical
// NAME: proto-pdb:default.pdb
void BasicWallSwitchMain::Activate()
{
    Critter* activator = pManager->Find(_activatorName);
    SwampSceneNode* node = _node;
    Vince* vince = pManager->_vinceCache;
    // !FAKE: inliner; pointer local to the call, hand form of the inlined Vector3 difference; retail reads both results through eax (probe 53.45 -> byte-equal)
    const Vector3* p = &vince->GetWorldPosition();
    const Vector3* v1 = &node->GetWorldPosition();
    Vector3 diff(v1->x - p->x, v1->y - p->y, v1->z - p->z);
    diff.y = 0.0f;
    vince->SetFacing(diff);
    vince->SetWorldPosition(activator->GetWorldPosition());
    vince->SetScript("VinceSwitch");
    vince->_readyToUseObject = false;
    SwitchMain::Activate();
}

// FUNCTION: USA 0x0010F486
// NAME: proto-pdb:default.pdb
SwampScript* SwitchMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\Switch.h", 14) SwitchMain(node);
}

// FUNCTION: USA 0x0010F4FB
// PROTOTYPE: PREVIEW 0x004F9DEA identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 87.85, bytes mismatch (masked 0); one _Tidy now (shared `return false` after the else); retail lays the _Tidy block after the size test (jne to the loop test), ours after the loop (13 drafts)
// T10.c1 also tried: bool result local (stack slot, 85.43), function-scope result (81.06), size() == 0, if/else, while/do loops, !empty() block, single branch first (43.39): <= 87.85
// T12.c17: scope-exit `return true` after the if/else keeps the _Tidy at the size test but no bl merge (84.72); at-stop 20
bool SwitchMain::NotifyTarget(int message)
{
    if (_target.empty())
        return false;
    if (_multiTarget) {
        // !FAKE: c2 register allocation; dummy local; retail loads pManager into edi before GetZone (match_one 75.53 -> 81.39)
        CritterManager* p = pManager;
        std::vector<Critter*> targets;
        p->FindAllMatching(targets, _target, _node->GetZone());
        if (targets.empty())
            return false;
        for (std::vector<Critter*>::iterator it = targets.begin(); it != targets.end(); ++it)
            (*it)->GetScript()->Message(this, message, 0);
        return true;
    } else {
        // !FAKE: c1xx front end; temporary; Find's result tested then used, no PDB local (stand-in)
        Critter* p = pManager->Find(_target);
        if (p != 0) {
            p->GetScript()->Message(this, message, 0);
            return true;
        }
    }
    return false;
}
#endif

// FUNCTION: USA 0x0010F5BB
// PROTOTYPE: PREVIEW 0x004F9EAA identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 89.77, bytes mismatch (masked 0); one _Tidy now (shared `return false` after the else); retail lays the _Tidy block after the size test (jne to the loop test), ours after the loop (11 drafts)
// T10.c1: same layout wall as NotifyTarget (its forms tried there, none moved the _Tidy block)
// T12.c17: 13 loop/return forms compile identical (89.77); scope-exit `return true` lays the _Tidy at the size test, no bl merge (88.86); at-stop 20
bool SwitchMain::ActivateTarget()
{
    if (_target.empty())
        return false;
    if (_multiTarget) {
        // !FAKE: c2 register allocation; dummy local; retail loads pManager into edi before GetZone (NotifyTarget match_one 75.53 -> 81.39)
        CritterManager* p = pManager;
        std::vector<Critter*> targets;
        p->FindAllMatching(targets, _target, _node->GetZone());
        if (targets.empty())
            return false;
        for (std::vector<Critter*>::iterator it = targets.begin(); it != targets.end(); ++it)
            (*it)->GetScript()->Activate();
        return true;
    } else {
        // !FAKE: c1xx front end; temporary; Find's result tested then used, no PDB local (stand-in)
        Critter* p = pManager->Find(_target);
        if (p != 0) {
            p->GetScript()->Activate();
            return true;
        }
    }
    return false;
}
#endif

// FUNCTION: USA 0x0010F66F
// PROTOTYPE: PREVIEW 0x004F9F5E near-identical
// NAME: proto-pdb:default.pdb
void SwitchMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 6:
        CritterMain::Message(source, 6, data);
        NotifyTarget(7);
        if (field_13B) {
            SetState(STATE_ON);
            ResourceHandle<AnimationControl> anim = ((GatorNode*)_node)->StartAnimation(_onAnim, false, true);
            ((GatorNode*)_node)->SetAnimSpeed(0.0f);
        }
        break;
    case 7:
        _activatorName = source->_node->_Name;
        break;
    case 8:
        _popupOK = true;
        break;
    case MESSAGE_TURN_OFF:
        _popupOK = true;
        ResetPopupTimer();
        break;
    case MESSAGE_SET_ONEWAYTHROW:
        _oneWayThrow = true;
        break;
    case MESSAGE_QUERYSTATE:
        *(int*)data = _state;
        break;
    default:
        CritterMain::Message(source, message, data);
        break;
    }
}

// FUNCTION: USA 0x0010F758
// PROTOTYPE: PREVIEW 0x004F9FF2 identical
// NAME: proto-pdb:default.pdb
void SwitchMain::TimerEvent(unsigned int timerId)
{
    if (timerId == TIMER_TURNOFF) {
        if (!_popupOK) {
            RegisterTimerEvent(TIMER_TURNOFF, 0.1f, false);
            return;
        }
        if (_offAnimName.empty()) {
            // !FAKE: c1xx front end; temporary; the AddRef/IsValid/SubRef copy needs a named handle, no PDB local (stand-in)
            ResourceHandle<AnimationControl> tmp = ((GatorNode*)_node)->StartAnimation(_onAnim, false, true);
            if (tmp.IsValid()) {
                tmp().SetAnimationTime(tmp().GetAnimationLength());
                tmp().SetAnimationSpeed(-1.0f);
            }
        } else
            ((GatorNode*)_node)->StartAnimation(_offAnim, false, false);
        SetState(STATE_TURNINGOFF);
        if (NotifyTarget(8))
            _popupOK = false;
    }
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0010F891
// NAME: proto-pdb:default.pdb
SwitchMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("SwitchMain", creationFunc);
}

// FUNCTION: USA 0x0010F8CD
// NAME: proto-pdb:default.pdb
FloorSwitchMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("FloorSwitchMain", creationFunc);
}

// FUNCTION: USA 0x0010F909
// NAME: proto-pdb:default.pdb
BasicWallSwitchMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BasicWallSwitchMain", creationFunc);
}
