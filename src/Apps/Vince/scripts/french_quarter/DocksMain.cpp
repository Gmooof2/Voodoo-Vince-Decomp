// UNIT: src/Apps/Vince/scripts/french_quarter/DocksMain.cpp OBJ: Xbox\OfficialRelease\DocksMain.obj OPT: /O1 /Oy
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

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    void ActivateMatching(const SwampString&, unsigned int);
    void SetSleepMatching(const SwampString&, bool, unsigned int);
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    static void SetStream(const SwampString& wav, bool looped);
};

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_LOADCOMPLETE = 0x14
    };
    virtual void EnteringRegion(SwampSceneNode*);
    virtual void Message(SwampScript*, int, void*);
    bool InRegion() const { return _inRegion; }

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_4E[0x64 - 0x4E];        // 0x4E
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class BigPigRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    BigPigRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BigPigRegionMain, 0x64);

// FUNCTION: USA 0x000702FF
// PROTOTYPE: PREVIEW 0x0045C49E identical
// NAME: proto-pdb:default.pdb
void BigPigRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (!InRegion()) {
        LevelScript::EnteringRegion(node);
        pManager->ActivateMatching("SafeHookRope", 0);
        pManager->SetSleepMatching("Big", false, 0);
        pManager->SetSleepMatching("SafeHookRope", false, 0);
        CutsceneDriver::SetStream("kosmoPigCutscene", false);
    }
}

// FUNCTION: USA 0x000704A5
// PROTOTYPE: PREVIEW 0x0045C644 identical
// NAME: proto-pdb:default.pdb
SwampScript* BigPigRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("docks\\DocksMain.cpp", 15) BigPigRegionMain(node);
}

class MainstreetRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript* from, int message, void* data);
};

// FUNCTION: USA 0x00070505
// PROTOTYPE: PREVIEW 0x00444272 identical
// NAME: proto-pdb:default.pdb
void MainstreetRegionMain::Message(SwampScript* from, int message, void* data)
{
    if (message != MESSAGE_LOADCOMPLETE)
        LevelScript::Message(from, message, data);
}
