// UNIT: src/Apps/Vince/core/HUDUI.cpp OBJ: core\XBox\OfficialRelease\HUDUI.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
#include <float.h>

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class T, class A> class basic_string<unsigned short, T, A> {
public:
    basic_string() { _Myres = 7; _Eos(0); } // src proto-pdb:default.pdb _Myres
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const unsigned short* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const unsigned short* _Ptr);
    unsigned int size() const { return _Mysize; }
protected:
    void _Eos(unsigned int _Newsize);
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x14];                 // 0x00
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
namespace std { // NAME: proto-pdb:default.pdb
template <class T> struct less; // src proto-pdb:default.pdb less
template <class T1, class T2> struct pair { // src proto-pdb:default.pdb pair
    T1 first;                          // 0x00 // src proto-pdb:default.pdb first
    T2 second; // src proto-pdb:default.pdb second
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits { // src proto-pdb:default.pdb _Tmap_traits
public:
    typedef K key_type; // src proto-pdb:default.pdb key_type
    typedef pair<const K, T> value_type;
};
template <class Tr> class _Tree { // src proto-pdb:default.pdb _Tree
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
    };
public:
    class const_iterator {
    public:
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        bool operator==(const const_iterator& _Right) const { return _Ptr == _Right._Ptr; }
        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
        typename Tr::value_type* operator->() const { return &this->_Ptr->_Myval; }
    };
    iterator end() { return iterator(_Myhead); }
    iterator find(const typename Tr::key_type& _Keyval); // src proto-pdb:default.pdb find

private:
    char _Alval;                       // 0x00 // src proto-pdb:default.pdb _Alval
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map
    : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
}

namespace BeepLib { // NAME: proto-pdb:default.pdb
std::basic_string<char, std::char_traits<char>, std::allocator<char> > MakeLower( // src proto-pdb:default.pdb MakeLower
    const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& s);
}

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

void* operator new(unsigned int size, const char* file, int line);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2
    };

    const Vector3& SetLocation(const Vector3& location);
    void SetOption(unsigned int option, bool value, bool recursive);
    virtual ~SwampSceneNode();                                     // vtable+0x00

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

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

    void RegisterTimerEvent(unsigned int id, float deltaTime, bool looped);

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    char field_4C[0xBC - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class UIMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_QUERYEFFECT = 0x14,
        MESSAGE_ADD = 0x15,
        MESSAGE_SUBTRACT = 0x16,
        MESSAGE_SETOWNER = 0x17,
        MESSAGE_BUMPCOUNTER = 0x18,
        MESSAGE_SETCOUNTER = 0x19,
        MESSAGE_FILLERUP = 0x1A,
        MESSAGE_FILLED = 0x1B,
        MESSAGE_UISPECIFIC = 0x1C
    };
};
DECOMP_SIZE_ASSERT(UIMain, 0x4C);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class AnimationControl; // NAME: proto-pdb:default.pdb

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
};

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    bool IsOffscreen() const;
    void GetOffscreenNow();

    Vector3 _offScreenLoc;             // 0xA4
    float _onScreenTime;               // 0xB0
    float _onScreenTimer;              // 0xB4
    Vector3 _speed;                    // 0xB8
    Vector3 _accel;                    // 0xC4
    bool _slideVertical;               // 0xD0
    bool _useAlpha;                    // 0xD1
    bool _vanishOffScreen;             // 0xD2
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

class HealthUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    bool CounterIsCurrent();
    void RestartThrob();

    float _drawnHealth;                // 0xD4
    int _destHealth;                   // 0xD8
    char field_DC[0xE8 - 0xDC];        // 0xDC
    ResourceHandle<TextureResource> _bigBagTex[5];     // 0xE8
    ResourceHandle<TextureResource> _cautionBagTex[4]; // 0xFC
    ResourceHandle<TextureResource> _lilBagEmptyTex;   // 0x10C
    ResourceHandle<TextureResource> _lilBagFullTex;    // 0x110
    char field_114[0x4];               // 0x114
    float _throbSin;                   // 0x118
    char field_11C[0x128 - 0x11C];     // 0x11C
};
DECOMP_SIZE_ASSERT(HealthUI, 0x128);

class ClustersUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    bool IsAnimating();

    char field_D4[0xE4 - 0xD4];        // 0xD4
    GatorNode* _bag;                   // 0xE4
    char field_E8[0xEC - 0xE8];        // 0xE8
    float _throbSin;                   // 0xEC
    bool _readyForMote;                // 0xF0
};
DECOMP_SIZE_ASSERT(ClustersUI, 0xF4);

class LivesUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    void RestartThrob();

    GatorNode* _heart;                 // 0xD4
    char field_D8[0xDC - 0xD8];        // 0xD8
    float _throbSin;                   // 0xDC
};
DECOMP_SIZE_ASSERT(LivesUI, 0xE0);

class SkullUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    static int GetSkullRowWidth();
    void RestartThrob();

    char field_D4[0xE8 - 0xD4];        // 0xD4
    float _throbSin;                   // 0xE8
    char field_EC[0xF4 - 0xEC];        // 0xEC
};
DECOMP_SIZE_ASSERT(SkullUI, 0xF4);

class HealthUIMain : public UIMain { // NAME: proto-pdb:default.pdb
public:
    virtual void FrameTick();
    HealthUI* GetUINode() { return (HealthUI*)_node; }

    char field_4C[0x90 - 0x4C];        // 0x4C
    SwampScript* _scriptToNotify;      // 0x90
};
DECOMP_SIZE_ASSERT(HealthUIMain, 0x94);

class BeadsUIMain : public UIMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_CHECKCOUNTER = 0
    };
    void BurnBeads();

    char field_4C[0x98 - 0x4C];        // 0x4C
    bool _burnBeads;                   // 0x98
};
DECOMP_SIZE_ASSERT(BeadsUIMain, 0x9C);

class UIHealthMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    UIHealthMain(SwampSceneNode* node);

    char field_BC[0xE0 - 0xBC];        // 0xBC
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(UIHealthMain, 0xE0);

class UIBeadMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SETVALUE = 0x1C
    };
    UIBeadMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);

    Vector3 _dest;                     // 0xBC
    Vector3 _start;                    // 0xC8
    float _moveTime;                   // 0xD4
    float _destTime;                   // 0xD8
    SwampScript* _owner;               // 0xDC
    int _val;                          // 0xE0
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(UIBeadMain, 0xE4);

class UIBonusBagMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    UIBonusBagMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void Message(SwampScript* source, int message, void* data);

    Vector3 _dest;                     // 0xBC
    Vector3 _start;                    // 0xC8
    float _moveTime;                   // 0xD4
    float _destTime;                   // 0xD8
    SwampScript* _owner;               // 0xDC
    bool _moveNow;                     // 0xE0
    bool _animDone;                    // 0xE1
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(UIBonusBagMain, 0xE4);

class UIClusterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    UIClusterMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);

    Vector3 _dest;                     // 0x4C
    Vector3 _start;                    // 0x58
    float _moveTime;                   // 0x64
    float _destTime;                   // 0x68
    SwampScript* _owner;               // 0x6C
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(UIClusterMain, 0x70);

class UIBagIconMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    UIBagIconMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(UIBagIconMain, 0xBC);

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x30];                // 0x00
    int _screenHeight;                 // 0x30
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x18 - 0xC];          // 0x0C
    AnimationManager& GetAnimationManager() const { return *_animationManager; }
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x60 - 0x1C];        // 0x1C
    unsigned int _frameID;             // 0x60
    char field_64[0x68 - 0x64];        // 0x64
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

namespace std { // NAME: proto-pdb:default.pdb
template <class T, class A> class list {
public:
    struct _Node { _Node* _Next; _Node* _Prev; };
    class iterator { public: _Node* _Ptr; };
    iterator begin() { iterator i; i._Ptr = _Myhead->_Next; return i; }
    iterator end() { iterator i; i._Ptr = _Myhead; return i; }
    unsigned int size() const { return _Mysize; }
    bool empty() const { return _Mysize == 0; }
    void push_back(const T& _Val) { _Insert(end(), _Val); }
    void pop_front() { erase(begin()); }
    iterator erase(iterator where); // retail 0x000E6B59, no accepted name
    void _Insert(iterator _Where, const T& _Val); // retail 0x000E84F7, no accepted name
private:
    char _Alnod[4];                    // 0x00 // src proto-pdb:default.pdb _Alnod
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
}

// Retail layout: retail disasm reads _maxSkulls at 0x404 (proto PDB 0x3F0, Vince 0x598).
class FireFlyNode; // NAME: proto-pdb:default.pdb

class Vince { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x404];               // 0x00
    int _maxSkulls;                    // 0x404
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

// Retail layout: retail disasm shows every member from proto 0xEC up at +4 (proto PDB HUDUI 0x2B0);
// the 4 retail-only bytes sit somewhere in 0xE6..0xF0 and stay inside the opaque span.
class HUDUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    void DisplayMissingControllerMessageThisFrame();
    void EndLetterbox(float fadetime);
    void EndExpandedLetterbox(float fadetime);
    void BloodHaze(float fadetime);
    void SetQuickMessage(const unsigned short* ptr, int fontsize);
    void ClearSubtitle();
    UINode* FindUINode(const SwampString& name);
    typedef std::map<SwampString, UINode*> UINodeMap; // NAME: proto-pdb:default.pdb

    struct Subtitle; // NAME: proto-pdb:default.pdb
    class QuickMessage { // NAME: proto-pdb:default.pdb
    public:
        std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > _wstr; // 0x00
        float _timer;                  // 0x1C
        float _scale;                  // 0x20
        float _dScale;                 // 0x24
        float _y;                      // 0x28
        float _ySinOffset;             // 0x2C
        float _alpha;                  // 0x30
        FireFlyNode* _effect;          // 0x34
        int _fontSize;                 // 0x38
    };

    char field_D4[0xE0 - 0xD4];        // 0xD4
    float _fontHeight;                 // 0xE0
    char field_E4[0xE5 - 0xE4];        // 0xE4
    bool _bloodHazeEnabled;            // 0xE5
    char field_E6[0xF0 - 0xE6];        // 0xE6
    unsigned int _displayMissingControllerMessageOnFrame; // 0xF0
    float _letterboxAlpha;             // 0xF4
    float _letterboxFadeRate;          // 0xF8
    float _letterboxScreenPct;         // 0xFC
    float _letterboxFadeRateEx;        // 0x100
    float _letterboxExPct;             // 0x104
    float _bloodHazeAlpha;             // 0x108
    float _bloodHazeFadeRate;          // 0x10C
    char field_110[0x260 - 0x110];     // 0x110
    UINodeMap _uiNodeMap;              // 0x260
    std::list<QuickMessage, std::allocator<QuickMessage> > _quickMessages; // 0x26C
    std::list<Subtitle, std::allocator<Subtitle> > _subtitleQueue; // 0x278
};

// FUNCTION: USA 0x000E0BEC
// PROTOTYPE: PREVIEW 0x004CBF91 identical
// NAME: proto-pdb:default.pdb
bool UINode::IsOffscreen() const
{
    return _onScreenTimer < 0.0f;
}

// FUNCTION: USA 0x000E0C06
// PROTOTYPE: PREVIEW 0x004CBFAB identical
// NAME: proto-pdb:default.pdb
bool HealthUI::CounterIsCurrent()
{
    return _drawnHealth == _destHealth;
}

// FUNCTION: USA 0x000E0C22
// PROTOTYPE: PREVIEW 0x004CBFC7 identical
// NAME: proto-pdb:default.pdb
bool ClustersUI::IsAnimating()
{
    return _throbSin >= 0.0f;
}

// FUNCTION: USA 0x000E0C73
// PROTOTYPE: PREVIEW 0x004CC004 identical
// NAME: proto-pdb:default.pdb
void HealthUI::RestartThrob()
{
    _throbSin = 0.0f;
}

// FUNCTION: USA 0x000E0C7C
// PROTOTYPE: PREVIEW 0x004CC00D identical
// NAME: proto-pdb:default.pdb
void UIBeadMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case UIMain::MESSAGE_SETOWNER:
        _owner = source;
        break;
    case MESSAGE_SETVALUE:
        _val = *(int*)data;
        break;
    }
}

// FUNCTION: USA 0x000E0CA5
// PROTOTYPE: PREVIEW 0x004CC036 identical
// NAME: proto-pdb:default.pdb
void BeadsUIMain::BurnBeads()
{
    _burnBeads = true;
    RegisterTimerEvent(TIMER_CHECKCOUNTER, 0.1f, false);
}

// FUNCTION: USA 0x000E0CC0
// PROTOTYPE: PREVIEW 0x004CC051 identical
// NAME: proto-pdb:default.pdb
void UIBonusBagMain::AnimationFinished()
{
    _animDone = true;
}

// FUNCTION: USA 0x000E0CC8
// PROTOTYPE: PREVIEW 0x004CC059 identical
// NAME: proto-pdb:default.pdb
void UIBonusBagMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case UIMain::MESSAGE_SETOWNER:
        _owner = source;
        break;
    case UIMain::MESSAGE_FILLED:
        _moveNow = true;
        break;
    }
}

// FUNCTION: USA 0x000E0CEC
// PROTOTYPE: PREVIEW 0x004CC07D identical
// NAME: proto-pdb:default.pdb
void UIClusterMain::Message(SwampScript* source, int message, void* data)
{
    if (message == UIMain::MESSAGE_SETOWNER)
        _owner = source;
}

// FUNCTION: USA 0x000E0CFD
// PROTOTYPE: PREVIEW 0x004CC0A2 identical
// NAME: proto-pdb:default.pdb
void LivesUI::RestartThrob()
{
    _throbSin = 0.0f;
}

// FUNCTION: USA 0x000E0D84
// PROTOTYPE: PREVIEW 0x004CC101 near-identical
// NAME: proto-pdb:default.pdb
void HUDUI::EndLetterbox(float fadetime)
{
    if (fadetime > 0.0f)
        _letterboxFadeRate = -1.0f / fadetime;
    else
        _letterboxFadeRate = -FLT_MAX;
}

// FUNCTION: USA 0x000E0DB4
// PROTOTYPE: PREVIEW 0x004CC131 near-identical
// NAME: proto-pdb:default.pdb
void HUDUI::EndExpandedLetterbox(float fadetime)
{
    if (fadetime > 0.0f)
        _letterboxFadeRateEx = -1.0f / fadetime;
    else
        _letterboxFadeRateEx = -FLT_MAX;
}

// FUNCTION: USA 0x000E0DE4
// PROTOTYPE: PREVIEW 0x004CC161 near-identical
// NAME: proto-pdb:default.pdb
void HUDUI::BloodHaze(float fadetime)
{
    _bloodHazeEnabled = true;
    _bloodHazeFadeRate = 1.0f / fadetime;
    _bloodHazeAlpha = 0.0f;
}

// FUNCTION: USA 0x000E0E41
// PROTOTYPE: PREVIEW 0x004CC1BE identical
// NAME: proto-pdb:default.pdb
void SkullUI::RestartThrob()
{
    _throbSin = 0.0f;
}

// FUNCTION: USA 0x000E0E60
// PROTOTYPE: PREVIEW 0x004CC1DD identical
// NAME: proto-pdb:default.pdb
void UINode::GetOffscreenNow()
{
    SetLocation(_offScreenLoc);
    _onScreenTimer = -1.0f;
    if (_vanishOffScreen)
        SetOption(DontDraw, true, true);
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x000E1023
// PROTOTYPE: PREVIEW 0x004CC2D6 identical
// NAME: proto-pdb:default.pdb
// best: objdiff 88.5, bytes mismatch
void HealthUIMain::FrameTick()
{
    SwampScript* p = _scriptToNotify;
    if (p && GetUINode()->CounterIsCurrent()) {
        p->Message(this, MESSAGE_FILLED, 0);
        _scriptToNotify = 0;
    }
}
#endif

// FUNCTION: USA 0x000E142A
// PROTOTYPE: PREVIEW 0x004CC7A7 near-identical
// NAME: proto-pdb:default.pdb
void HUDUI::DisplayMissingControllerMessageThisFrame()
{
    _displayMissingControllerMessageOnFrame = g_swamp->_frameID;
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x000E14C4
// PROTOTYPE: PREVIEW 0x004CC815 near-identical
// NAME: proto-pdb:default.pdb
int SkullUI::GetSkullRowWidth()
{
    return pManager->_vinceCache->_maxSkulls * 18 + 12;
}
#endif

class FadeToBlack : public UINode { // NAME: proto-pdb:default.pdb
public:
    virtual void Update();

    float _alpha;                      // 0xD4
    float _dt;                         // 0xD8
};
DECOMP_SIZE_ASSERT(FadeToBlack, 0xDC);

// FUNCTION: USA 0x000E14DA
// PROTOTYPE: PREVIEW 0x004CC82B identical
// NAME: proto-pdb:default.pdb
void FadeToBlack::Update()
{
    if (_dt > 0.0f) {
        _alpha += g_swamp->_input->GetElapsedTime() * _dt;
        if (_alpha >= 1.0f) {
            _alpha = 1.0f;
            _dt = 0.0f;
        }
    }
}

class AreaStats { // NAME: proto-pdb:default.pdb
public:
    AreaStats();
    void Copy(const AreaStats& pstats);

    int _clustersRemaining;            // 0x00
    int _clustersFound;                // 0x04
    int _jugRemaining;                 // 0x08
    int _jugMax;                       // 0x0C
    int _pagesFound;                   // 0x10
    int _pagesRemaining;               // 0x14
    int _boxesFound;                   // 0x18
    int _boxesMax;                     // 0x1C
    int _juneBugsFound;                // 0x20
    int _juneBugsMax;                  // 0x24
    int _whammysFound;                 // 0x28
    int _whammysRemaining;             // 0x2C
    SwampString _name;                 // 0x30
};
DECOMP_SIZE_ASSERT(AreaStats, 0x4C);

// FUNCTION: USA 0x000E2691
// NAME: proto-pdb:default.pdb
AreaStats::AreaStats()
{
    _clustersRemaining = _clustersFound = 0;
    _jugRemaining = _jugMax = 0;
    _pagesFound = _pagesRemaining = 0;
    _boxesFound = _boxesMax = 0;
    _juneBugsFound = _juneBugsMax = 0;
    _whammysFound = _whammysRemaining = 0;
}

inline void* operator new(unsigned int, void* p) { return p; }

// FUNCTION: USA 0x000E2C79
// PROTOTYPE: PREVIEW 0x004CDFEE identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit QuickMessage copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_QuickMessage_copy(void* p, const HUDUI::QuickMessage& v)
{
    new (p) HUDUI::QuickMessage(v);
}

// FUNCTION: USA 0x000E2CE1
// PROTOTYPE: PREVIEW 0x004CE056 identical
// NAME: proto-pdb:default.pdb
SwampScript* UIHealthMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    return new("core\\HUDUI.cpp", 257) UIHealthMain(node);
}

// FUNCTION: USA 0x000E2D0B
// PROTOTYPE: PREVIEW 0x004CE080 identical
// NAME: proto-pdb:default.pdb
SwampScript* UIBeadMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    return new("core\\HUDUI.cpp", 759) UIBeadMain(node);
}

class ClustersUIMain : public UIMain { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished();
    ClustersUI* GetUINode() { return (ClustersUI*)_node; }

    char field_4C[0x8C - 0x4C];        // 0x4C
    HUDUI* _hud;                       // 0x8C
    class Critter* _uiBag;             // 0x90
};
DECOMP_SIZE_ASSERT(ClustersUIMain, 0x94);

// FUNCTION: USA 0x000E2E78
// PROTOTYPE: PREVIEW 0x004CE1D1 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit AreaStats copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_AreaStats_copy(void* p, const AreaStats& v)
{
    new (p) AreaStats(v);
}

// FUNCTION: USA 0x000E2FA1
// PROTOTYPE: PREVIEW 0x004CE2FA identical
// NAME: proto-pdb:default.pdb
void AreaStats::Copy(const AreaStats& pstats)
{
    _boxesFound = pstats._boxesFound;
    _boxesMax = pstats._boxesMax;
    _clustersFound = pstats._clustersFound;
    _clustersRemaining = pstats._clustersRemaining;
    _jugMax = pstats._jugMax;
    _jugRemaining = pstats._jugRemaining;
    _juneBugsFound = pstats._juneBugsFound;
    _juneBugsMax = pstats._juneBugsMax;
    _pagesFound = pstats._pagesFound;
    _pagesRemaining = pstats._pagesRemaining;
    _whammysFound = pstats._whammysFound;
    _whammysRemaining = pstats._whammysRemaining;
    _name = pstats._name;
}

// FUNCTION: USA 0x000E40BD
// PROTOTYPE: PREVIEW 0x004CF40D identical
// NAME: proto-pdb:default.pdb
void ClustersUIMain::AnimationFinished()
{
    GetUINode()->_bag->StartAnimation(g_swamp->GetAnimationManager().Load("dustbag_idle"), true, false);
}

class LivesUIMain : public UIMain { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished();
    LivesUI* GetUINode() { return (LivesUI*)_node; }

    char field_4C[0x8C - 0x4C];        // 0x4C
    HUDUI* _hud;                       // 0x8C
};
DECOMP_SIZE_ASSERT(LivesUIMain, 0x90);

// FUNCTION: USA 0x000E470D
// PROTOTYPE: PREVIEW 0x004CFA5D identical
// NAME: proto-pdb:default.pdb
void LivesUIMain::AnimationFinished()
{
    GetUINode()->_heart->StartAnimation(g_swamp->GetAnimationManager().Load("extralife_idle"), true, false);
}

// FUNCTION: USA 0x000E6837
// PROTOTYPE: PREVIEW 0x004D1B89 near-identical
// NAME: proto-pdb:default.pdb
UINode* HUDUI::FindUINode(const SwampString& name)
{
    UINodeMap::iterator it = _uiNodeMap.find(BeepLib::MakeLower(name));
    if (it == _uiNodeMap.end())
        return 0;
    return it->second;
}

// FUNCTION: USA 0x000E6F17
// PROTOTYPE: PREVIEW 0x004D2220 identical
// NAME: proto-pdb:default.pdb
SwampScript* UIBonusBagMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    return new("core\\HUDUI.cpp", 1111) UIBonusBagMain(node);
}

// FUNCTION: USA 0x000E70E7
// PROTOTYPE: PREVIEW 0x004D23F0 identical
// NAME: proto-pdb:default.pdb
SwampScript* UIClusterMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    return new("core\\HUDUI.cpp", 1226) UIClusterMain(node);
}

// FUNCTION: USA 0x000E710E
// PROTOTYPE: PREVIEW 0x004D2417 identical
// NAME: proto-pdb:default.pdb
SwampScript* UIBagIconMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    return new("core\\HUDUI.cpp", 1292) UIBagIconMain(node);
}

#ifdef NON_MATCHING
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1HealthUI@@UAE@XZ in this unit (no matched
// function odr-uses it; the unsymboled FUN_000E892F after it calls ??0UINode); this qualified call odr-uses it instead
// (no matched_set row: neither ??1HealthUI@@UAE@XZ nor this scaffold is listed, so the gate build leaves this out; the
// call stays as the odr-use that emits the dtor in a NON_MATCHING build)
void pin_emit_HealthUI_dtor(HealthUI* p)
{
    p->HealthUI::~HealthUI();
}
#endif

#ifdef NON_MATCHING
// FUNCTION: USA 0x000E912A
// PROTOTYPE: PREVIEW 0x004D43E8 near-identical
// NAME: proto-pdb:default.pdb
void HUDUI::ClearSubtitle()
{
    if (!_subtitleQueue.empty())
        _subtitleQueue.pop_front();
}
#endif

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): the push_back _Insert call 0x000E84F7 is unresolved
// FUNCTION: USA 0x000E92E3
// PROTOTYPE: PREVIEW 0x004D45A1 near-identical
// NAME: proto-pdb:default.pdb
void HUDUI::SetQuickMessage(const unsigned short* ptr, int fontsize)
{
    QuickMessage qm;

    qm._wstr = ptr;
    qm._scale = 0.01f;
    qm._dScale = 4.0f;
    qm._timer = qm._wstr.size() * 0.05f + 2.0f;
    qm._y = _quickMessages.size() * _fontHeight * 2 + g_swamp->_renderer->_screenHeight * 0.5f;
    qm._ySinOffset = 0;
    qm._alpha = 0;
    qm._effect = 0;
    qm._fontSize = fontsize;
    _quickMessages.push_back(qm);
}
#endif
