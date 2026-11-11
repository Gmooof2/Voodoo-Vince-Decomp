// UNIT: src/Apps/Vince/core/BasicDoor.cpp OBJ: core\XBox\OfficialRelease\BasicDoor.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    bool empty() const { return (_Mysize == 0); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x14];                 // 0x00
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class T, class A = allocator<T> > class vector {
public:
    T* begin() { return _Myfirst; }
    T* end() { return _Mylast; }
    void push_back(const T& _Val);

private:
    char _pad00[0x4];                  // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    SwampScript* GetScript() const;
    const SwampString& GetName() const { return _Name; }

    char field_4[0x4];                 // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
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
    char field_24[0x2C - 0x24];        // 0x24
    float _length;                     // 0x2C
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);
    bool IsValid() const { return _Resource != 0; }
    T& operator()() { return *_Resource; }
    bool operator==(const ResourceHandle& h) const { return _Resource == h._Resource; }

    T* _Resource;                      // 0x00
};

// visible body: cl 13.10 /O1 keeps a handle local in a register only with it (C0136, C0330)
template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationSpeed(float);
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim._Resource->_length; }
    const ResourceHandle<AnimationResource>& GetAnimHandle() { return _anim; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x38 - 0x28];        // 0x28
    float _time;                       // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    ResourceHandle<AnimationControl> GetCurrentAnimation() const { return _currentAnim; }

    char field_A4[0x140 - 0xA4];       // 0xA4
    ResourceHandle<AnimationControl> _currentAnim; // 0x140
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    char field_144[0x2F5 - 0x144];     // 0x144
    bool _inputEnabled;                // 0x2F5 proto 0x2E5
};

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Return(float);
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);

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
    SwampSceneNode* GetNode() { return _node; }

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);
    virtual SwampString Save();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum State { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0,
        STATE_OPEN = 1,
        STATE_OPENING = 2,
        STATE_CLOSING = 3,
        STATE_LOCKEDOPEN = 4,
        STATE_BASICDOOR_USER = 5
    };
    enum Message { // NAME: proto-pdb:default.pdb
        MESSAGE_OPEN = 0x14,
        MESSAGE_CLOSE = 0x15,
        MESSAGE_OPEN_INSTANT = 0x16,
        MESSAGE_CLOSE_INSTANT = 0x17,
        MESSAGE_QUERYSTATE = 0x18,
        MESSAGE_STAYOPEN = 0x19,
        MESSAGE_BASICDOOR_USERDEFINED = 0x1A
    };
    virtual void Message(SwampScript*, int, void*);
    virtual void AnimationFinished();
    virtual void Activate();
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
    virtual int GetDefaultState() const;                           // vtable+0x70
    virtual ~BasicDoorMain();

    Critter* GetBasicDoor() { return (Critter*)GetNode(); }

protected:
    virtual void SetState(int);
    void NotifySwitches(int msg);

public:
    ResourceHandle<AnimationResource> _animOpen; // 0xBC
    ResourceHandle<AnimationResource> _animClose; // 0xC0
    std::vector<SwampString> _ownerSwitches; // 0xC4
    SwampString _animCam;              // 0xD4
    SwampString _animCamClose;         // 0xF0
    char field_10C[0x1];               // 0x10C
    bool _locked;                      // 0x10D proto 0x10C
    char field_10E[0x2];               // 0x10E
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

// FUNCTION: USA 0x001078A9
// PROTOTYPE: PREVIEW 0x004F225D near-identical
// NAME: proto-pdb:default.pdb
void BasicDoorMain::AnimationFinished()
{
    switch (_state) {
    case STATE_OPENING:
        if (!_animCam.empty()) {
            AnimCam::Return(0.0f);
            pManager->_vinceCache->_inputEnabled = true;
        }
        SetState(STATE_OPEN);
        break;
    case STATE_CLOSING:
        if (!_animCamClose.empty()) {
            AnimCam::Return(0.0f);
            pManager->_vinceCache->_inputEnabled = true;
        }
        SetState(STATE_CLOSED);
        break;
    }
}

// FUNCTION: USA 0x0010790D
// PROTOTYPE: PREVIEW 0x004F22C1 identical
// NAME: proto-pdb:default.pdb
void BasicDoorMain::NotifySwitches(int msg)
{
    for (SwampString* i = _ownerSwitches.begin(); i != _ownerSwitches.end(); ++i) {
        SwampString name = *i;
        Critter* p = pManager->Find(name);
        if (p)
            p->GetScript()->Message(this, msg, 0);
    }
}

// FUNCTION: USA 0x0010796C
// PROTOTYPE: PREVIEW 0x004F2320 identical
// NAME: proto-pdb:default.pdb
void BasicDoorMain::Activate()
{
    if (_state == STATE_CLOSED)
        SetState(STATE_OPENING);
    else if (_state == STATE_OPEN)
        NotifySwitches(8);
}

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_GAMESTART = 0x6,
    MESSAGE_WHOSYOURDADDY = 0x7,
    MESSAGE_READY = 0x8
};

// FUNCTION: USA 0x00107988
// PROTOTYPE: PREVIEW 0x004F233C near-identical
// NAME: proto-pdb:default.pdb
void BasicDoorMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_CLOSED:
        if (_animOpen.IsValid()) {
            ResourceHandle<AnimationControl> ach = GetBasicDoor()->StartAnimation(_animOpen, false, false);
            ach().SetAnimationTime(0.0f);
            ach().SetAnimationSpeed(0.0f);
        } else if (_animClose.IsValid()) {
            ResourceHandle<AnimationControl> ach = GetBasicDoor()->StartAnimation(_animClose, false, false);
            ach().SetAnimationTime(ach().GetAnimationLength());
            ach().SetAnimationSpeed(0.0f);
        }
        NotifySwitches(MESSAGE_READY);
        break;
    case STATE_OPEN:
        if (_animOpen.IsValid()) {
            ResourceHandle<AnimationControl> ach = GetBasicDoor()->StartAnimation(_animOpen, false, false);
            ach().SetAnimationTime(ach().GetAnimationLength());
            ach().SetAnimationSpeed(0.0f);
        } else if (_animClose.IsValid()) {
            ResourceHandle<AnimationControl> ach = GetBasicDoor()->StartAnimation(_animClose, false, false);
            ach().SetAnimationTime(0.0f);
            ach().SetAnimationSpeed(0.0f);
        }
        NotifySwitches(MESSAGE_READY);
        break;
    case STATE_CLOSING:
        if (_animClose.IsValid() || _animOpen.IsValid()) {
            if (!_animCamClose.empty()) {
                AnimCam::SwitchTo(_animCamClose, 0.0f, false, 0.0f);
                pManager->_vinceCache->_inputEnabled = false;
            }
            if (_animClose.IsValid()) {
                GetBasicDoor()->StartAnimation(_animClose, false, false);
            } else {
                ResourceHandle<AnimationControl> ach = GetBasicDoor()->GetCurrentAnimation();
                if (!ach.IsValid())
                    ach = GetBasicDoor()->StartAnimation(_animOpen, false, true);
                else if (!(_animOpen == ach().GetAnimHandle()))
                    GetBasicDoor()->StartAnimation(_animOpen, false, true);
                ach().SetAnimationSpeed(-1.0f);
            }
        } else {
            SetState(STATE_CLOSED);
        }
        break;
    case STATE_OPENING:
        if (_animOpen.IsValid()) {
            if (!_animCam.empty()) {
                AnimCam::SwitchTo(_animCam, 0.0f, false, 0.0f);
                pManager->_vinceCache->_inputEnabled = false;
            }
            ResourceHandle<AnimationControl> ach = GetBasicDoor()->GetCurrentAnimation();
            if (ach.IsValid() && _animOpen == ach().GetAnimHandle())
                ach().SetAnimationSpeed(1.0f);
            else
                GetBasicDoor()->StartAnimation(_animOpen, false, false);
        } else {
            SetState(STATE_OPEN);
        }
        break;
    }
}

// FUNCTION: USA 0x00107CBA
// PROTOTYPE: PREVIEW 0x004F2659 near-identical
// NAME: proto-pdb:default.pdb
void BasicDoorMain::Message(SwampScript* source, int message, void* data)
{
    CritterMain::Message(source, message, data);
    if (_locked && message != MESSAGE_GAMESTART && message != MESSAGE_QUERYSTATE)
        return;
    switch (message) {
    case MESSAGE_GAMESTART:
        if (_locked)
            Message(this, MESSAGE_OPEN_INSTANT, 0);
        break;
    case MESSAGE_WHOSYOURDADDY:
        _ownerSwitches.push_back(source->GetNode()->GetName());
        break;
    case MESSAGE_READY:
    case MESSAGE_CLOSE:
        if (!_locked) {
            if (_state != STATE_CLOSED && _state != STATE_CLOSING)
                SetState(STATE_CLOSING);
            else
                NotifySwitches(MESSAGE_READY);
        }
        break;
    case MESSAGE_OPEN:
        if (_state != STATE_OPEN && _state != STATE_OPENING)
            SetState(STATE_OPENING);
        break;
    case MESSAGE_OPEN_INSTANT:
        SetState(STATE_OPEN);
        break;
    case MESSAGE_CLOSE_INSTANT:
        if (!_locked)
            SetState(STATE_CLOSED);
        break;
    case MESSAGE_QUERYSTATE:
        if (_locked)
            *(int*)data = STATE_LOCKEDOPEN;
        else
            *(int*)data = _state;
        break;
    case MESSAGE_STAYOPEN:
        Message(this, MESSAGE_OPEN_INSTANT, 0);
        NotifySwitches(MESSAGE_OPEN_INSTANT);
        _locked = true;
        break;
    }
}
