// UNIT: src/Apps/Vince/scripts/french_quarter/Courtyard.cpp OBJ: Xbox\OfficialRelease\Courtyard.obj OPT: /O1 /Oy
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

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;
};

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger* Find(const SwampString&);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    RegionManager& GetRegionManager() const { return *_regionManager; }

    char field_0[0x30];                // 0x00
    RegionManager* _regionManager;     // 0x30
    char field_34[0x68 - 0x34];        // 0x34
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    static void SetStream(const SwampString& wav, bool looped);
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

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode*);
    bool InRegion() const { return _inRegion; }

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_4E[0x64 - 0x4E];        // 0x4E
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class CourtyardRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    CourtyardRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x68 - 0x64];        // 0x64
};
DECOMP_SIZE_ASSERT(CourtyardRegionMain, 0x68);

// FUNCTION: USA 0x0006FD79
// PROTOTYPE: PREVIEW 0x0045C026 identical
// NAME: proto-pdb:default.pdb
void CourtyardRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (!InRegion()) {
        LevelScript::EnteringRegion(node);
        RegionTrigger* r = g_swamp->GetRegionManager().Find("firstVoodooIcon");
        bool didicon = false;
        r->GetScript()->Message(this, 0x14, &didicon);
        CutsceneDriver::SetStream(didicon ? "vinceMeetsKosmoCutscene" : "firstVoodooIcon", false);
    }
}

// FUNCTION: USA 0x0006FE24
// PROTOTYPE: PREVIEW 0x0045C0D6 identical
// NAME: proto-pdb:default.pdb
SwampScript* CourtyardRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("courtyard\\Courtyard.cpp", 23) CourtyardRegionMain(node);
}
