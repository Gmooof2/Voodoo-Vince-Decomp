// UNIT: src/Apps/Vince/scripts/Sewers/PropellerRoom.cpp OBJ: Xbox\OfficialRelease\PropellerRoom.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[0x1C / 4];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class SwampScript;

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1,
        DontDraw = 0x2,
        NoCull = 0x4
    };
    SwampScript* GetScript() const;
    void SetOption(unsigned int option, bool on, bool recurse);

    char field_0[0xA4];                // 0x00
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);

    char field_0[0x2C];                // 0x00
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x124 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(RegionTrigger, 0x124);

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger* Find(const SwampString& name);

    char field_0[0xC];                 // 0x00
};
DECOMP_SIZE_ASSERT(RegionManager, 0xC);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    RegionManager& GetRegionManager() const { return *_regionManager; }

    char field_0[0x30];                // 0x00
    RegionManager* _regionManager;     // 0x30
    char field_34[0x68 - 0x34];        // 0x34
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class HeatShimmer : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xE0 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(HeatShimmer, 0xE0);

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

    char field_4[0x48];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class ShimmerRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    HeatShimmer* _shimmer;            // 0x4C
};
DECOMP_SIZE_ASSERT(ShimmerRegionMain, 0x50);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    int GetState() const { return _state; }

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class PropellerMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SHIMMERSWITCH = 0x14,
        MESSAGE_ISSLOWED = 0x15,
        MESSAGE_ISSTOPPED = 0x16
    };

private:
    void ComputeSpeed();
    void SetShimmer(char letter, bool on);

public:
    bool _ventAOff;                    // 0xBC
    bool _ventBOff;                    // 0xBD
    bool _ventCOff;                    // 0xBE
    bool _ventDOff;                    // 0xBF
    bool _showprop;                    // 0xC0
    char field_C1[0xC4 - 0xC1];        // 0xC1
    float _speed;                      // 0xC4
    char field_C8[0xD0 - 0xC8];        // 0xC8
};
DECOMP_SIZE_ASSERT(PropellerMain, 0xD0);

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual bool TestActivate() const;
    virtual void Message(SwampScript* source, int message, void* data);

    char field_BC[0x13C - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished();
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class Switch_PropellerMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    virtual bool TestActivate() const;
    virtual void Message(SwampScript* source, int message, void* data);

    SwampScript* _propeller;           // 0x13C
};
DECOMP_SIZE_ASSERT(Switch_PropellerMain, 0x140);

class Switch_ShimmerMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished();

    char _letter;                      // 0x13C
};
DECOMP_SIZE_ASSERT(Switch_ShimmerMain, 0x140);

// FUNCTION: USA 0x000A3926
// PROTOTYPE: PREVIEW 0x0048EA02 identical
// NAME: proto-pdb:default.pdb
void PropellerMain::ComputeSpeed()
{
    _speed = 0.0f;
    if (!_ventAOff)
        _speed += 0.25f;
    if (!_ventBOff)
        _speed += 0.25f;
    if (!_ventCOff)
        _speed += 0.25f;
    if (!_ventDOff)
        _speed += 0.25f;
    _speed *= 2.0f;
}

// FUNCTION: USA 0x000A3997
// PROTOTYPE: PREVIEW 0x0048EA73 identical
// NAME: proto-pdb:default.pdb
bool Switch_PropellerMain::TestActivate() const
{
    if (_propeller) {
        bool isStopped;
        _propeller->Message(0, PropellerMain::MESSAGE_ISSTOPPED, &isStopped);
        if (!isStopped)
            return false;
    }
    return BasicWallSwitchMain::TestActivate();
}

// FUNCTION: USA 0x000A40F6
// PROTOTYPE: PREVIEW 0x0048F125 identical
// NAME: proto-pdb:default.pdb
void PropellerMain::SetShimmer(char letter, bool on)
{
    // !FAKE: c1xx front end; dummy local; PDB lists no local, the string temp dies before the null test
    RegionTrigger* p = g_swamp->GetRegionManager().Find(format("shimmer%c", letter));
    if (p)
        // !FAKE: inliner; direct member read; GetShimmer() accessor pulls GetScript before the SetOption args (match_one 80.86 -> 100.0)
        ((ShimmerRegionMain*)p->GetScript())->_shimmer->SetOption(SwampSceneNode::DontDraw, !on, false);
}

// FUNCTION: USA 0x000A42F7
// PROTOTYPE: PREVIEW 0x0048F326 identical
// NAME: proto-pdb:default.pdb
void Switch_PropellerMain::Message(SwampScript* source, int message, void* data)
{
    if (message != 6) {
        BasicWallSwitchMain::Message(source, message, data);
        return;
    }
    // !FAKE: c1xx front end; dummy local; PDB lists no local, the string temp dies before the null test
    Critter* p = pManager->Find("propeller");
    if (p)
        _propeller = p->GetScript();
}

// FUNCTION: USA 0x000A43AA
// PROTOTYPE: PREVIEW 0x0048F3D9 identical
// NAME: proto-pdb:default.pdb
void Switch_ShimmerMain::AnimationFinished()
{
    if (GetState() == 1) {
        // !FAKE: c1xx front end; dummy local; PDB lists no local, the string temp dies before GetScript
        Critter* p = pManager->Find("propeller");
        p->GetScript()->Message(this, PropellerMain::MESSAGE_SHIMMERSWITCH, (void*)_letter);
    }
    BasicWallSwitchMain::AnimationFinished();
}
