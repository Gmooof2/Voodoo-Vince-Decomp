// UNIT: src/Apps/Vince/core/Trolley.cpp OBJ: core\XBox\OfficialRelease\Trolley.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
struct AABox;      // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10

    void SetOption(unsigned int, bool, bool);
    const Quaternion& SetRotation(const Quaternion&);

    char field_4[0x40 - 0x4];          // 0x04
    Quaternion _Rotation;              // 0x40
};
class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
};

class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Return(float);
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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class VinceExitTrolley : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    VinceExitTrolley(SwampSceneNode* node);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(VinceExitTrolley, 0x4C);

void* operator new(unsigned int size, const char* file, int line);

// FUNCTION: USA 0x0013043E
// PROTOTYPE: PREVIEW 0x00518E71 identical
// NAME: proto-pdb:default.pdb
void VinceExitTrolley::AnimationFinished()
{
    AnimCam::Return(1.0f);
    InvalidateScript();
}

class VinceBoardTrolley : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    VinceBoardTrolley(SwampSceneNode* node);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_4C[0x64 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(VinceBoardTrolley, 0x64);

// FUNCTION: USA 0x00130485
// PROTOTYPE: PREVIEW 0x00518EB8 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceExitTrolley::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\Trolley.h", 60) VinceExitTrolley(node);
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); not finished: pManager pin is site-corr (provisional)
// FUNCTION: USA 0x001304A9
// PROTOTYPE: PREVIEW 0x00518EDC near-identical
// NAME: proto-pdb:default.pdb
void TrolleyMain::MoveVinceToPad()
{
    Vince* vince = pManager->_vinceCache;
    SwampSceneNode* padnode;

    _signScript->Message(this, 0x15, &padnode); // src proto-pdb:default.pdb MESSAGE_GETPADNODE
    Vector3 pos = padnode->GetWorldPosition();
    vince->SetWorldPosition(pos);
    vince->_startPos = pos;
    vince->SetRotation(padnode->_Rotation);
}
#endif

// FUNCTION: USA 0x00130504
// PROTOTYPE: PREVIEW 0x00518F37 identical
// NAME: proto-pdb:default.pdb
void VinceBoardTrolley::AnimationFinished()
{
    _node->SetOption(2, true, false);
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
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

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    T* _Resource;                      // 0x00
};

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    char field_4C[0x70];               // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

// FUNCTION: USA 0x00130668
// PROTOTYPE: PREVIEW 0x0051909B identical
// NAME: proto-pdb:default.pdb
class TrolleyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF& tdf);
    ResourceHandle<AnimationResource> _arrive;   // 0xBC
    ResourceHandle<AnimationResource> _boarding; // 0xC0
    ResourceHandle<AnimationResource> _depart;   // 0xC4
    ResourceHandle<AnimationResource> _flyby;    // 0xC8
    SwampString _camName;              // 0xCC
    SwampScript* _signScript;          // 0xE8

private:
    void MoveVinceToPad();
};
DECOMP_SIZE_ASSERT(TrolleyMain, 0xEC);

// Retail layout: Vince _startPos 0x354 -> 0x368 (+0x14).
class Vince : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_50[0x368 - 0x50];       // 0x50
    Vector3 _startPos;                 // 0x368
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1TrolleyMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_TrolleyMain_dtor(TrolleyMain* p)
{
    p->TrolleyMain::~TrolleyMain();
}

// FUNCTION: USA 0x00130940
// PROTOTYPE: PREVIEW 0x00519373 identical
// NAME: proto-pdb:default.pdb
void TrolleyMain::Load(const BeepTDF& tdf)
{
    _camName = tdf.GetStringRecord("camera", "camera_trolley");
}

// FUNCTION: USA 0x00130CBF
// PROTOTYPE: PREVIEW 0x005196F2 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceBoardTrolley::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\Trolley.h", 38) VinceBoardTrolley(node);
}
