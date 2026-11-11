// UNIT: src/Apps/Vince/core/DialogueRegion.cpp OBJ: core\XBox\OfficialRelease\DialogueRegion.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    int compare(const basic_string& _Right) const;
    const E* c_str() const { return _Myptr(); }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    char _Alval;                       // 0x00
    union {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right);
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right)
{
    return _Left.compare(_Right) == 0;
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
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
class SoundNode;    // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Return(float);
};

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

    DialogueRegionMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    SwampString _onscreenText;         // 0x4C
    SwampString _dialogueFilename;     // 0x68
    SwampString _camName;              // 0x84
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
    virtual void SoundFinished(const SwampString& name);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    SwampString _name;                 // 0xBC
    SoundNode* _sound;                 // 0xD8
};
DECOMP_SIZE_ASSERT(VinceThinkAloud, 0xDC);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

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

// FUNCTION: USA 0x001220BA
// PROTOTYPE: PREVIEW 0x0050AD88 identical
// NAME: proto-pdb:default.pdb
void VinceThinkAloud::SoundFinished(const SwampString& name)
{
    if (name == _name) {
        InvalidateScript();
        AnimCam::Return(0.0f);
    }
}

// FUNCTION: USA 0x00122122
// NAME: proto-pdb:default.pdb
SwampScript* DialogueRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\DialogueRegion.h", 18) DialogueRegionMain(node);
}

// FUNCTION: USA 0x0012257A
// NAME: proto-pdb:default.pdb
DialogueRegionMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("DialogueRegionMain", creationFunc);
}

// FUNCTION: USA 0x00122173
// PROTOTYPE: PREVIEW 0x0050AE41 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceThinkAloud::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\DialogueRegion.h", 44) VinceThinkAloud(node);
}

// FUNCTION: USA 0x001225B6
// NAME: proto-pdb:default.pdb
VinceThinkAloud::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VinceThinkAloud", creationFunc);
}

// FUNCTION: USA 0x0012219A
// PROTOTYPE: PREVIEW 0x0050AE68 identical
// NAME: proto-pdb:default.pdb
void DialogueRegionMain::Load(const BeepTDF& tdf)
{
    _onscreenText = tdf.GetStringRecord("onscreentext", "");
    _dialogueFilename = tdf.GetStringRecord("dialogue", "");
    _camName = tdf.GetStringRecord("camera", "");
    _hintDone = tdf.GetBoolRecord("hintdone", false);
}

// FUNCTION: USA 0x0012243F
// PROTOTYPE: PREVIEW 0x0050B00F identical
// NAME: proto-pdb:default.pdb
SwampString DialogueRegionMain::Save()
{
    return SwampScript::Save() + format("\t\tonscreentext = %s;\n", _onscreenText.c_str())
        + format("\t\tdialogue = %s;\n", _dialogueFilename.c_str())
        + format("\t\tcamera = %s;\n", _camName.c_str())
        + format("\t\thintdone = %d;\n", _hintDone);
}
