// UNIT: src/Apps/Vince/core/LevelScript.cpp OBJ: core\XBox\OfficialRelease\LevelScript.obj OPT: /O1 /Oy
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
template <class T, class A = allocator<T> > class vector { // src proto-pdb:default.pdb vector
public:
    unsigned int size() const;

private:
    char _pad00[0x10];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString& id, int def) const;
};
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

    void RegisterTimerEvent(unsigned int, float, bool);
    void UnregisterTimerEvent(unsigned int);

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_LOADCOMPLETE = 0x14,
        MESSAGE_NEXT_HINT = 0x15,
        MESSAGE_LEVEL_USERDEFINED = 0x16
    };
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_PROGRESS_HINT = 0x0,
        TIMER_LEVEL_USER = 0x1
    };
    struct ProgressHint;               // NAME: proto-pdb:default.pdb

    virtual void LeavingRegion(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    int _progressHintLevel;            // 0x50
    std::vector<ProgressHint> _progressHints; // 0x54
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

// FUNCTION: USA 0x000EC5D0
// PROTOTYPE: PREVIEW 0x004D78EF identical
// NAME: proto-pdb:default.pdb
void LevelScript::LeavingRegion(SwampSceneNode* node)
{
    if (_inRegion) {
        _inRegion = false;
        if (_isZone)
            UnregisterTimerEvent(TIMER_PROGRESS_HINT);
    }
}

// FUNCTION: USA 0x000EC5E8
// PROTOTYPE: PREVIEW 0x004D7907 identical
// NAME: proto-pdb:default.pdb
void LevelScript::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_NEXT_HINT && _isZone) {
        UnregisterTimerEvent(TIMER_PROGRESS_HINT);
        if (_progressHintLevel < _progressHints.size())
            RegisterTimerEvent(TIMER_PROGRESS_HINT, 0.1f, false);
    }
}

// FUNCTION: USA 0x000EC727
// PROTOTYPE: PREVIEW 0x004D7A2B identical
// NAME: proto-pdb:default.pdb
void LevelScript::Load(const BeepTDF& tdf)
{
    SwampScript::Load(tdf);
    _progressHintLevel = tdf.GetIntRecord("progresshintlevel", 0);
}
