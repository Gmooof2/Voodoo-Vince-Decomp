// UNIT: src/Apps/Vince/scripts/Carnival/Carnival.cpp OBJ: Xbox\OfficialRelease\Carnival.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, npos); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum CollisionType { // NAME: proto-pdb:default.pdb
        HorizontalCollision = 0,
        VerticalCollision = 1
    };
    SwampScript(SwampSceneNode* node);
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

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    LevelScript(SwampSceneNode* node, bool isZone);
    virtual ~LevelScript();
    virtual void TimerEvent(unsigned int id);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
    virtual bool DoProgressHint(int level, const SwampString& hint); // vtable+0x64

    char field_4C[0x64 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

// Later of the two PDB records (0x68): retail reads _solvedSH at 0x64, _solvedHOM at 0x65.
class CarnivalLevelMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SHSOLVED = 0x16,
        MESSAGE_WASSHSOLVED = 0x17,
        MESSAGE_HOMSOLVED = 0x18,
        MESSAGE_WASHOMSOLVED = 0x19
    };

    CarnivalLevelMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

    bool _solvedSH;  // 0x64
    bool _solvedHOM; // 0x65
};
DECOMP_SIZE_ASSERT(CarnivalLevelMain, 0x68);

// FUNCTION: USA 0x0003AEF0
// PROTOTYPE: PREVIEW 0x004295E7 identical
// NAME: proto-pdb:default.pdb
void CarnivalLevelMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_SHSOLVED:
        _solvedSH = true;
        break;
    case MESSAGE_WASSHSOLVED:
        *(bool*)data = _solvedSH;
        break;
    case MESSAGE_HOMSOLVED:
        _solvedHOM = true;
        break;
    case MESSAGE_WASHOMSOLVED:
        *(bool*)data = _solvedHOM;
        break;
    default:
        LevelScript::Message(source, message, data);
    }
}

// FUNCTION: USA 0x0003AF27
// PROTOTYPE: PREVIEW 0x0042961E identical
// NAME: proto-pdb:default.pdb
void CarnivalLevelMain::Load(const BeepTDF& tdf)
{
    LevelScript::Load(tdf);
    _solvedSH = tdf.GetBoolRecord("solvedSH", false);
    _solvedHOM = tdf.GetBoolRecord("solvedHOM", false);
}

// FUNCTION: USA 0x0003AF91
// PROTOTYPE: PREVIEW 0x00429688 identical
// NAME: proto-pdb:default.pdb
SwampString CarnivalLevelMain::Save()
{
    SwampString str = LevelScript::Save();
    str += format("\t\tsolvedSH = %d;\n", _solvedSH);
    str += format("\t\tsolvedHOM = %d;\n", _solvedHOM);
    return str;
}

// FUNCTION: USA 0x0003B020
// PROTOTYPE: PREVIEW 0x00429717 identical
// NAME: proto-pdb:default.pdb
CarnivalLevelMain::CarnivalLevelMain(SwampSceneNode* node)
    : LevelScript(node, false), _solvedSH(false), _solvedHOM(false)
{
}
