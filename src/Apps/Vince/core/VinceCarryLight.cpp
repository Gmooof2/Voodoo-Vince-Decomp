// UNIT: src/Apps/Vince/core/VinceCarryLight.cpp OBJ: core\XBox\OfficialRelease\VinceCarryLight.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

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

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

enum VinceAnimState { // NAME: proto-pdb:default.pdb
    STATE_PICKINGUP = 0x15,
    STATE_DROPPING = 0x16
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    SwampSceneNode* GetParent() { return _Parent; }

    char field_4[0x24 - 0x4];          // 0x04
    SwampSceneNode* _Parent;           // 0x24
};

class Critter : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
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

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class VinceShared : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    char field_BC[0xE4 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(VinceShared, 0xE4);

class VinceMain : public VinceShared { // NAME: proto-pdb:default.pdb
public:
    virtual void InvalidateScript();
    virtual void FrameTick();
    virtual void AnimationFinished();

    char field_E4[0x18C - 0xE4];       // 0xE4
};
DECOMP_SIZE_ASSERT(VinceMain, 0x18C);

class VinceCarryLight : public VinceMain { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished();
    virtual void FrameTick();

    SwampString _carriedObject;        // 0x18C
};
DECOMP_SIZE_ASSERT(VinceCarryLight, 0x1A8);

// FUNCTION: USA 0x00129DEE
// PROTOTYPE: PREVIEW 0x0051285F identical
// NAME: proto-pdb:default.pdb
void VinceCarryLight::AnimationFinished()
{
    if (pManager->Find(_carriedObject)->GetParent() != _node)
        InvalidateScript();
    else
        VinceMain::AnimationFinished();
}

// FUNCTION: USA 0x00129E19
// PROTOTYPE: PREVIEW 0x0051288A identical
// NAME: proto-pdb:default.pdb
void VinceCarryLight::FrameTick()
{
    // !FAKE: c1xx front end; stand-in local; the PDB records no local, one Find result is tested then read
    Critter* p = pManager->Find(_carriedObject);
    if (!p || (p->GetParent() != _node && _state != STATE_PICKINGUP && _state != STATE_DROPPING))
        InvalidateScript();
    VinceMain::FrameTick();
}
