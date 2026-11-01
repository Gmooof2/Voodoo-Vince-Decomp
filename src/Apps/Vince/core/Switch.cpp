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

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
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
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    virtual void Message(SwampScript*, int, void*);

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual bool TestActivate() const;
    virtual void Activate();
    virtual void Message(SwampScript* source, int message, void* data);

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

protected:
    bool NotifyTarget(int message);
public:
    void ResetPopupTimer();

    char field_BC[0xF4 - 0xBC];        // 0xBC
    ResourceHandle<AnimationResource> _onAnim; // 0xF4
    char field_F8[0x100 - 0xF8];       // 0xF8
    SwampString _activatorName;        // 0x100
    char field_11C[0x138 - 0x11C];     // 0x11C
    bool _popupOK;                     // 0x138
    bool _oneWayThrow;                 // 0x139
    bool _multiTarget;                 // 0x13A
    bool field_13B;                    // 0x13B retail only: tested by the retail Message, absent in the proto PDB
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Activate();
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

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

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

#ifdef NON_MATCHING
// best: match_one objdiff 83.67 (control /O1 /Oy; one if-form: /O1 /Oy 68.67, /O2 /Oy 47.33)
// FUNCTION: USA 0x0010F1A2
// PROTOTYPE: PREVIEW 0x004F9AC0 near-identical
// NAME: proto-pdb:default.pdb
bool SwitchMain::TestActivate() const
{
    CarriedObject* obj;
    return _state == 0 && ((obj = pManager->_vinceCache->_carriedObject) == 0 || obj->_wearable ||
                           obj == pManager->_vinceCache->_boomerangPin);
}
#endif

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

#ifdef NON_MATCHING
// best: match_one objdiff 53.45, bytes mismatch (masked 0)
// FUNCTION: USA 0x0010F3D4
// PROTOTYPE: PREVIEW 0x004F9CC3 near-identical
// NAME: proto-pdb:default.pdb
void BasicWallSwitchMain::Activate()
{
    Critter* activator = pManager->Find(_activatorName);
    SwampSceneNode* node = _node;
    Vince* vince = pManager->_vinceCache;
    const Vector3& vpos = vince->GetWorldPosition();
    const Vector3& npos = node->GetWorldPosition();
    Vector3 diff(npos.x - vpos.x, 0.0f, npos.z - vpos.z);
    vince->SetFacing(diff);
    vince->SetWorldPosition(activator->GetWorldPosition());
    vince->SetScript("VinceSwitch");
    vince->_readyToUseObject = false;
    SwitchMain::Activate();
}
#endif

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0; near-corr CritterMain::Message resolves in byte_compare)
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
#endif
