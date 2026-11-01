// UNIT: src/Apps/Vince/core/DialogueRegion.cpp OBJ: core\XBox\OfficialRelease\DialogueRegion.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
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

class SwampGamePad; // NAME: proto-pdb:default.pdb

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    const SwampGamePad* GetPad(int player) const;
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    void* _hwnd;                       // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x60];                // 0x08
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class DialogueRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_HINTOFF = 0x14
    };

    virtual void Message(SwampScript* source, int message, void* data);

    char field_4C[0x54];               // 0x4C
    bool _hintDone;                    // 0xA0
};
DECOMP_SIZE_ASSERT(DialogueRegionMain, 0xA4);

class VinceCutScene : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void FrameTick();
};
DECOMP_SIZE_ASSERT(VinceCutScene, 0xBC);

class VinceThinkAloud : public VinceCutScene { // NAME: proto-pdb:default.pdb
public:
    VinceThinkAloud(SwampSceneNode* node);
    virtual void FrameTick();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    char field_BC[0x20];               // 0xBC
};
DECOMP_SIZE_ASSERT(VinceThinkAloud, 0xDC);

// FUNCTION: USA 0x0012208F
// PROTOTYPE: PREVIEW 0x0050AD5D identical
// NAME: proto-pdb:default.pdb
void DialogueRegionMain::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_HINTOFF) {
        _hintDone = true;
    }
}

// FUNCTION: USA 0x001220A0
// PROTOTYPE: PREVIEW 0x0050AD6E identical
// NAME: proto-pdb:default.pdb
void VinceThinkAloud::FrameTick()
{
    g_swamp->_input->GetPad(0);
    VinceCutScene::FrameTick();
}

// FUNCTION: USA 0x00122173
// PROTOTYPE: PREVIEW 0x0050AE41 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceThinkAloud::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\DialogueRegion.h", 44) VinceThinkAloud(node);
}
