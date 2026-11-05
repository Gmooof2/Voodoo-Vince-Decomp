// UNIT: src/Apps/Vince/scripts/ManorHouse/CaptainsRoom.cpp OBJ: Xbox\OfficialRelease\CaptainsRoom.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    int compare(const E* _Ptr) const;

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class T, class A = allocator<T> > class deque {
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb
class AnimationResource;     // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle; // NAME: proto-pdb:default.pdb

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
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    SwampScript* GetScript() const;

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode : SwampSceneNode, all at 0x0; only SwampSceneNode modelled
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool restart, bool loop);
    void SetAnimSpeed(float speed);

    char field_A4[0x200 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0x2C8 - 0x200];     // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line);

class ButlerStartRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    ButlerStartRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ButlerStartRegionMain, 0x4C);

class ButlerResetRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    ButlerResetRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ButlerResetRegionMain, 0x4C);

class CaptainBrusqueMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_FINISHED_THANKS = 0x14
    };
    enum {
        STATE_IDLE = 0x0,
        STATE_CRUMPLED = 0x1
    };
    CaptainBrusqueMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Activate();
    virtual void InvalidateScript();
protected:
    // access: the retail symbol is protected (?SetState@CaptainBrusqueMain@@MAEXH@Z)
    virtual void SetState(int);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    Critter* _dolly;                   // 0xBC
};
DECOMP_SIZE_ASSERT(CaptainBrusqueMain, 0xC0);

// FUNCTION: USA 0x0008ECD7
// PROTOTYPE: PREVIEW 0x0047A396 identical
// NAME: proto-pdb:default.pdb
void CaptainBrusqueMain::Message(SwampScript* source, int message, void* data)
{
    if (message != MESSAGE_FINISHED_THANKS) {
        CritterMain::Message(source, message, data);
        return;
    }
    if (_dolly) {
        _dolly->Invalidate();
        _dolly = 0;
    }
    SetState(STATE_CRUMPLED);
}

// FUNCTION: USA 0x0008ED17
// PROTOTYPE: PREVIEW 0x0047A3D6 identical
// NAME: proto-pdb:default.pdb
void CaptainBrusqueMain::InvalidateScript()
{
    if (_dolly) {
        _dolly->Invalidate();
        _dolly = 0;
    }
    CritterMain::InvalidateScript();
}

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x110 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class PassageExitMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    PassageExitMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(PassageExitMain, 0x110);

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
class SoundResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
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

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    T Fetch();

    std::deque<T> _queue;              // 0x00
};

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void Play();
    void SetMinDistance(float);
    void SetMaxDistance(float);
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static SoundNode* CreateSound(const ResourceHandle<SoundResource>&, const SwampString&, unsigned int, SwampSceneNode*, bool);
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

// FUNCTION: USA 0x0008F6CC
// NAME: proto-pdb:default.pdb
class DollyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    DollyMain(SwampSceneNode* node);
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
    virtual void InvalidateScript();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_BC[0xC0 - 0xBC];        // 0xBC
    Critter* _poodle;                  // 0xC0
    char field_C4[0xC8 - 0xC4];        // 0xC4
    ResourceHandle<AnimationResource> _animDollyGiggle;  // 0xC8
    ResourceHandle<AnimationResource> _animPoodleGiggle; // 0xCC
    MultiAnimation _animNotHappy;      // 0xD0
    SwampShuffleQueue<ResourceHandle<SoundResource> > _giggleComments; // 0xE4
};
DECOMP_SIZE_ASSERT(DollyMain, 0xF8);

// FUNCTION: USA 0x0008ED68
// PROTOTYPE: PREVIEW 0x0047A427 identical
// NAME: proto-pdb:default.pdb
void DollyMain::InvalidateScript()
{
    if (_poodle) {
        _poodle->Invalidate();
        _poodle = 0;
    }
    CritterMain::InvalidateScript();
}

// FUNCTION: USA 0x0008ED89
// PROTOTYPE: PREVIEW 0x0047A448 identical
// NAME: proto-pdb:default.pdb
SwampScript* ButlerStartRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\manorhouse\\captainsroom\\CaptainsRoom.h", 81) ButlerStartRegionMain(node);
}

// FUNCTION: USA 0x0008EDAD
// PROTOTYPE: PREVIEW 0x0047A46C identical
// NAME: proto-pdb:default.pdb
SwampScript* ButlerResetRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\manorhouse\\captainsroom\\CaptainsRoom.h", 97) ButlerResetRegionMain(node);
}

// FUNCTION: USA 0x0008EE3F
// PROTOTYPE: PREVIEW 0x0047A56F identical
// NAME: proto-pdb:default.pdb
void CaptainBrusqueMain::Activate()
{
    if (_dolly)
        _dolly->GetScript()->Activate();
}

// FUNCTION: USA 0x0008EEE9
// PROTOTYPE: PREVIEW 0x0047A619 identical
// NAME: proto-pdb:default.pdb
void CaptainBrusqueMain::SetState(int state)
{
    // !FAKE: c2 register allocation; neutral pointer local; retail reads _node once into esi (no PDB local)
    GatorNode* p = (GatorNode*)_node;
    _state = state;
    switch (state) {
    case STATE_IDLE:
        p->StartAnimation(g_swamp->GetAnimationManager().Load("captainBrusque_idle"), true, false);
        break;
    case STATE_CRUMPLED:
        p->StartAnimation(g_swamp->GetAnimationManager().Load("captainBrusque_dollyThanksVince"), false, true);
        p->SetAnimSpeed(0.0f);
        break;
    }
}

// FUNCTION: USA 0x0008EFA0
// PROTOTYPE: PREVIEW 0x0047A6D0 identical
// NAME: proto-pdb:default.pdb
void ButlerStartRegionMain::EnteringRegion(SwampSceneNode* node)
{
    // !FAKE: c1xx front end; temporary; retail destroys the "busybutler" string before GetScript, a one-expression chain keeps it to the end (match_one 67.83 -> 100.0)
    Critter* critter = pManager->Find("busybutler");
    critter->GetScript()->Message(this, 0x14, 0); // src proto-pdb:default.pdb MESSAGE_START
}

// FUNCTION: USA 0x0008F131
// PROTOTYPE: PREVIEW 0x0047A861 identical
// NAME: proto-pdb:default.pdb
SwampScript* CaptainBrusqueMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\manorhouse\\captainsroom\\CaptainsRoom.h", 31) CaptainBrusqueMain(node);
}

// FUNCTION: USA 0x0008F158
// PROTOTYPE: PREVIEW 0x0047A888 identical
// NAME: proto-pdb:default.pdb
void DollyMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    if (name == "trigger_giggle") {
        // !FAKE: c1xx front end; neutral stand-in local; the PDB records no local for the sound node
        SoundNode* p = SoundManager::CreateSound(_giggleComments.Fetch(), "dolly_giggle", 10, GetNode(), true);
        p->SetMinDistance(2.0f);
        p->SetMaxDistance(10.0f);
        p->Play();
    }
}

// FUNCTION: USA 0x0008F769
// PROTOTYPE: PREVIEW 0x0047AE99 identical
// NAME: proto-pdb:default.pdb
SwampScript* PassageExitMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\manorhouse\\captainsroom\\CaptainsRoom.h", 68) PassageExitMain(node);
}

// FUNCTION: USA 0x0008F790
// PROTOTYPE: PREVIEW 0x0047AEC0 identical
// NAME: proto-pdb:default.pdb
SwampScript* DollyMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\manorhouse\\captainsroom\\CaptainsRoom.h", 111) DollyMain(node);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1DollyMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_DollyMain_dtor(DollyMain* p)
{
    p->DollyMain::~DollyMain();
}
