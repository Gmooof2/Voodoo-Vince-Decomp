// UNIT: src/Apps/Vince/core/FireRegion.cpp OBJ: core\XBox\OfficialRelease\FireRegion.obj OPT: /O1 /Oy
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

protected:
    SwampScript(SwampSceneNode* node);
    void RegisterSaveLoadVar(const SwampString& name, float* addr, float defaultval);

public:
    void UnRegisterFrameTick();

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();
};

class Vince : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetOnFire(float, bool);
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetMainstreetFlameTime() const { return _mainstreetFlameTime; }

    char field_0[0x25C];               // 0x00
    float _mainstreetFlameTime;        // 0x25C
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class FireRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    FireRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode*);
    virtual void LeavingRegion(SwampSceneNode*);
    virtual void FrameTick();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    float _flameTime;                  // 0x4C
};
DECOMP_SIZE_ASSERT(FireRegionMain, 0x50);

// FUNCTION: USA 0x001219A7
// PROTOTYPE: PREVIEW 0x0050A651 identical
// NAME: proto-pdb:default.pdb
void FireRegionMain::FrameTick()
{
    pManager->GetVince()->SetOnFire(_flameTime, true);
}

// FUNCTION: USA 0x001219BE
// NAME: proto-pdb:default.pdb
void FireRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (node == pManager->GetVince())
        UnRegisterFrameTick();
}

// FUNCTION: USA 0x001219D5
// PROTOTYPE: PREVIEW 0x0050A67F identical
// NAME: proto-pdb:default.pdb
FireRegionMain::FireRegionMain(SwampSceneNode* node) : SwampScript(node), _flameTime(1.0f)
{
    RegisterSaveLoadVar("flameTime", &_flameTime, gSettings.GetMainstreetFlameTime());
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x00121A2B
// NAME: proto-pdb:default.pdb
SwampScript* FireRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\FireRegion.h", 13) FireRegionMain(node);
}

// FUNCTION: USA 0x00121A4F
// NAME: proto-pdb:default.pdb
FireRegionMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("FireRegionMain", creationFunc);
}
