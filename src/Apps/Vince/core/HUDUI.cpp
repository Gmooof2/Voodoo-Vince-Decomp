// UNIT: src/Apps/Vince/core/HUDUI.cpp OBJ: core\XBox\OfficialRelease\HUDUI.obj OPT: /O1 /Oy
// !FAKE: c2 intrinsics; file-scope pragma; retail inlines fsin in PagesUIMain::FrameTick under /O1 /Oy (C0181);
// the CRT sin declared first so the pragma precedes <math.h> (its sin(float) inline then gets fsin, else call _sin)
extern "C" double __cdecl sin(double);
#pragma intrinsic(sin)
#include <math.h>
#include "../../../include/decomp.h"
#include <float.h>
#include <xmmintrin.h>
// !FAKE: c2 intrinsics; file-scope pragma; retail inlines wmemcpy's memcpy as rep movsw in the basic_string<ushort>
// append rows (char_traits<wchar_t>::copy); declared first so the pragma precedes <wchar.h>'s inline wmemcpy (C0181)
extern "C" void* __cdecl memcpy(void*, const void*, size_t);
#pragma intrinsic(memcpy)
#include <wchar.h>

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
template <> struct char_traits<unsigned short> { // src proto-pdb:default.pdb char_traits
    static unsigned short* __cdecl copy(unsigned short* _First1, const unsigned short* _First2, size_t _Count)
    {
        return (::wmemcpy(_First1, _First2, _Count));
    }
};
class _String_base { // src proto-pdb:default.pdb _String_base
public:
    void _Xlen() const; // retail 0x00140DDC
    void _Xran() const; // retail 0x00140D9C
};
template <class T, class A> class basic_string<unsigned short, T, A> : public _String_base {
public:
    typedef unsigned int size_type;
    static const size_type npos;
    basic_string() { _Myres = 7; _Eos(0); } // src proto-pdb:default.pdb _Myres
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const unsigned short* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const unsigned short* _Ptr);
    basic_string& append(const basic_string& _Right, size_type _Roff, size_type _Count)
    {
        if (_Right.size() < _Roff)
            _String_base::_Xran();
        size_type _Num = _Right.size() - _Roff;
        if (_Num < _Count)
            _Count = _Num;
        if (npos - _Mysize <= _Count)
            _String_base::_Xlen();

        if (0 < _Count && _Grow(_Num = _Mysize + _Count)) {
            T::copy(_Myptr() + _Mysize, _Right._Myptr() + _Roff, _Count);
            _Eos(_Num);
        }
        return (*this);
    }
    basic_string& append(const unsigned short* _Ptr, size_type _Count)
    {
        if (_Inside(_Ptr))
            return (append(*this, _Ptr - _Myptr(), _Count));
        if (npos - _Mysize <= _Count)
            _String_base::_Xlen();

        size_type _Num;
        if (0 < _Count && _Grow(_Num = _Mysize + _Count)) {
            T::copy(_Myptr() + _Mysize, _Ptr, _Count);
            _Eos(_Num);
        }
        return (*this);
    }
    unsigned int size() const { return _Mysize; }

    enum { _BUF_SIZE = 16 / sizeof(unsigned short) < 1 ? 1 : 16 / sizeof(unsigned short) };
protected:
    void _Eos(unsigned int _Newsize);
    bool _Grow(size_type _Newsize, bool _Trim = false); // retail 0x000234D1
    bool _Inside(const unsigned short* _Ptr); // retail 0x00023362
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    A _Alval;                          // 0x00
    union _Bxty {
        unsigned short _Buf[_BUF_SIZE];
        unsigned short* _Ptr;
    } _Bx;                             // 0x04

public:
    unsigned short* _Myptr() { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }
    const unsigned short* _Myptr() const { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }

private:
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class T, class A>
const typename basic_string<unsigned short, T, A>::size_type basic_string<unsigned short, T, A>::npos =
    (typename basic_string<unsigned short, T, A>::size_type)(-1);
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

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

Vector3 Normalize(const Vector3&);     // NAME: proto-pdb:default.pdb

struct Vector4 { // NAME: proto-pdb:default.pdb
    void Set(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
public:
    Matrix() {}
    void Translation(const Vector3&);

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct AABox;      // NAME: proto-pdb:default.pdb
class Quaternion; // NAME: proto-pdb:default.pdb

extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawVerticesUP(unsigned long, unsigned int, const void*, unsigned int); // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x248];               // 0x00
    float _uiZoomonTime;               // 0x248
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line);

class SwampScript; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2
    };
    enum NodeRenderingSettings { // NAME: proto-pdb:default.pdb
        Alpha = 0x0
    };

    const Vector3& SetLocation(const Vector3& location);
    const Vector3& GetLocation() const { return _Location; }
    // !FAKE: inliner; hand form of SetScale(float), inlined in retail and the prototype (z, y, x chained store)
    void SetScale(float f)
    {
        _Scale.x = _Scale.y = _Scale.z = f;
        InvalidateCachedWorldPos();
    }
    void SetOption(unsigned int option, bool value, bool recursive);
    SwampScript* GetScript() const;
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    void SetRenderingSetting(unsigned int setting, unsigned int value, bool recurse);
    unsigned int GetRenderingSetting(unsigned int setting) const { return _RenderingSetting[setting]; }
    bool GetRenderingOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    bool IsVisible() const { return !GetRenderingOption(DontDraw); }

protected:
    void InvalidateCachedWorldPos();

public:
    char field_4[0x34 - 0x4];          // 0x04
    Vector3 _Location;                 // 0x34
    char field_40[0x50 - 0x40];        // 0x40
    Vector3 _Scale;                    // 0x50
    char field_5C[0x88 - 0x5C];        // 0x5C
    unsigned int _RenderingOptions;    // 0x88
    unsigned int _RenderingSetting[1]; // 0x8C
    char field_90[0xA4 - 0x90];        // 0x90
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
    void RegisterFrameTick();
protected:
    SwampScript(SwampSceneNode* node);

public:

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

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
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
    ResourceHandle() : _Resource(0) {}
    ResourceHandle<T>& operator=(const ResourceHandle<T>&);
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

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
};

class CXBFont { // NAME: proto-pdb:default.pdb
public:
    long GetTextExtent(const SwampString&, float*, float*, int) const;
    void SetScaleFactors(float, float);
    long DrawText(float, float, unsigned long, const SwampString&, unsigned long, float);
};

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    bool IsOffscreen() const;
    void SetOffscreenLoc(const Vector3& loc);
    void ResetOnscreenTimer();
    void GetOffscreenNow();
    void RenderNumber(int num, float x, float y, float scale, unsigned int color, unsigned int flags) const;
    virtual void Update();

    Vector3 _offScreenLoc;             // 0xA4
    float _onScreenTime;               // 0xB0
    float _onScreenTimer;              // 0xB4
    Vector3 _speed;                    // 0xB8
    Vector3 _accel;                    // 0xC4
    bool _slideVertical;               // 0xD0
    bool _useAlpha;                    // 0xD1
    bool _vanishOffScreen;             // 0xD2
protected:
    static CXBFont* _font;
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

// FUNCTION: USA 0x000E88E1
// PROTOTYPE: PREVIEW 0x004D3B9F identical
// NAME: proto-pdb:default.pdb
class HealthUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    bool CounterIsCurrent();
    void RestartThrob();

    virtual void Render(const Matrix& m) const;

    float _drawnHealth;                // 0xD4
    int _destHealth;                   // 0xD8
    int _drawnMaxHealth;               // 0xDC
    float _newBagScale;                // 0xE0
    float _newBagAlpha;                // 0xE4
    ResourceHandle<TextureResource> _bigBagTex[5];     // 0xE8
    ResourceHandle<TextureResource> _cautionBagTex[4]; // 0xFC
    ResourceHandle<TextureResource> _lilBagEmptyTex;   // 0x10C
    ResourceHandle<TextureResource> _lilBagFullTex;    // 0x110
    float _fade;                       // 0x114
    float _throbSin;                   // 0x118
    char field_11C[0x128 - 0x11C];     // 0x11C
};
DECOMP_SIZE_ASSERT(HealthUI, 0x128);

class ClustersUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    bool IsAnimating();
    void Add();
    virtual void Update();

    int _drawnMotes;                   // 0xD4
    float _digitsScale;                // 0xD8
    float _clustersScale;              // 0xDC
    char field_E0[0xE4 - 0xE0];        // 0xE0
    GatorNode* _bag;                   // 0xE4
    int _queueMotes;                   // 0xE8
    float _throbSin;                   // 0xEC
    bool _readyForMote;                // 0xF0
};
DECOMP_SIZE_ASSERT(ClustersUI, 0xF4);

class LivesUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    void RestartThrob();
    virtual void Update();

    virtual void Render(const Matrix& m) const;

    GatorNode* _heart;                 // 0xD4
    int _drawnHearts;                  // 0xD8
    float _throbSin;                   // 0xDC
};
DECOMP_SIZE_ASSERT(LivesUI, 0xE0);

class SkullUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    static int GetSkullRowWidth();
    void RestartThrob();

    virtual void Render(const Matrix& m) const;

    // PDB statics WIDTH_SMALL_SKULL / WIDTH_LARGE_SKULL (const int) not declared: an in-class initializer emits an
    // unpinned data COMDAT (C0215); Render uses their retail values 17 / 28 as literals

    int _drawnSkulls;                  // 0xD4
    int _maxSkulls;                    // 0xD8
    ResourceHandle<TextureResource> _bigSkull;      // 0xDC
    ResourceHandle<TextureResource> _lilEmptySkull; // 0xE0
    ResourceHandle<TextureResource> _lilFullSkull;  // 0xE4
    float _throbSin;                   // 0xE8
    float _newSkullScale;              // 0xEC
    float _newSkullAlpha;              // 0xF0
};
DECOMP_SIZE_ASSERT(SkullUI, 0xF4);

class FireFlyNode; // NAME: proto-pdb:default.pdb

class BeadsUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    void SetBeadsScale(float s) { _beadsScale = s; }

    ResourceHandle<TextureResource> _beadsFull;  // 0xD4
    ResourceHandle<TextureResource> _beadsEmpty; // 0xD8
    int _drawnBeads;                   // 0xDC
    int _maxBeads;                     // 0xE0
    FireFlyNode* _effect;              // 0xE4
    float _beadsScale;                 // 0xE8
    bool _wasWhammyReady;              // 0xEC
};
DECOMP_SIZE_ASSERT(BeadsUI, 0xF0);

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
    virtual void FrameTick();
    BeadsUI* GetUINode() { return (BeadsUI*)_node; }

    char field_4C[0x94 - 0x4C];        // 0x4C
    float _throbSin;                   // 0x94
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
    virtual void FrameTick();
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
    virtual void FrameTick();
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
    virtual void FrameTick();
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
    bool EnableDepthBuffering(bool enable);
    long SetVertexShader(unsigned long);
    void SetBasicPixelShader();
    void SetEmptyTexture(unsigned long);
    void SetTexture(unsigned long, const ResourceHandle<TextureResource>&);
    int GetScreenWidth() const { return _screenWidth; }
    int GetScreenHeight() const { return _screenHeight; }
    int GetScreenSafeWidth() const { return _screenSafeWidth; }
    int GetScreenSafeLeft() const { return _screenSafeWidthMargin; }
    int GetScreenSafeTop() const { return _screenSafeHeightMargin; }
    int GetScreenSafeRight() const { return _screenSafeWidth + _screenSafeWidthMargin; }

    char field_0[0x2C];                // 0x00
    int _screenWidth;                  // 0x2C
    int _screenHeight;                 // 0x30
    int _screenSafeWidth;              // 0x34
    int _screenSafeHeight;             // 0x38
    int _screenSafeWidthMargin;        // 0x3C
    int _screenSafeHeightMargin;       // 0x40
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
    SwampRenderer& GetRenderer() const { return *_renderer; }
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

class exception { // NAME: proto-pdb:default.pdb
public:
    exception(const exception& v0);
    virtual ~exception();
    virtual const char* what() const;
private:
    const char* _m_what;               // 0x04
    int _m_doFree;                     // 0x08
};

namespace std { // NAME: proto-pdb:default.pdb
class logic_error : public exception { // src proto-pdb:default.pdb logic_error
public:
    explicit logic_error(const basic_string<char, char_traits<char>, allocator<char> >& _Message); // retail 0x0001179C
    virtual ~logic_error();
    virtual const char* what() const;
private:
    basic_string<char, char_traits<char>, allocator<char> > _Str; // 0x0C
};
class length_error : public logic_error { // src proto-pdb:default.pdb length_error
public:
    explicit length_error(const basic_string<char, char_traits<char>, allocator<char> >& _Message)
        : logic_error(_Message) {}
    virtual ~length_error() {}
};

template <class _Ty> inline _Ty* _Allocate(unsigned int _Count, _Ty*)
{
    return ((_Ty*)operator new(_Count * sizeof(_Ty)));
}
template <class _Ty> class allocator {
public:
    typedef _Ty* pointer;
    typedef unsigned int size_type;
    pointer allocate(size_type _Count) { return (_Allocate(_Count, (pointer)0)); }
    unsigned int max_size() const
    {
        unsigned int _Count = (unsigned int)(-1) / sizeof(_Ty);
        return (0 < _Count ? _Count : 1);
    }
};
template <class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    typedef unsigned int size_type;
    size_type size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    size_type max_size() const { return (this->_Alval.max_size()); }
    _Ty& operator[](size_type _Pos) { return (*(_Myfirst + _Pos)); }
protected:
    bool _Buy(size_type _Capacity)
    {
        _Myfirst = 0, _Mylast = 0, _Myend = 0;
        if (_Capacity == 0)
            return (false);
        else if (max_size() < _Capacity)
            _Xlen();
        else {
            _Myfirst = this->_Alval.allocate(_Capacity);
            _Mylast = _Myfirst;
            _Myend = _Myfirst + _Capacity;
        }
        return (true);
    }
    // !FAKE: c2 noreturn; retail _Xlen 0x00139A45 (named fold alias) is called out of line; the VC 7.1 body
    // (throw length_error) emits unnamed throw-info data (C0236, C0237)
    __declspec(noreturn) void _Xlen() const;
private:
    _Ax _Alval;                        // 0x00
    _Ty* _Myfirst;                     // 0x04
    _Ty* _Mylast;                      // 0x08
    _Ty* _Myend;                       // 0x0C
};

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
    unsigned int max_size() const { unsigned int v0 = (unsigned int)(-1) / sizeof(T); return 0 < v0 ? v0 : 1; }
protected:
    void _Incsize(unsigned int _Count)
    {
        if (max_size() - _Mysize < _Count)
            throw length_error("list<T> too long");
        _Mysize += _Count;
    }
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

class GameUI { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xD8];                // 0x00
    float _realElapsed;                // 0xD8
    char field_DC[0xE0 - 0xDC];        // 0xDC
};
DECOMP_SIZE_ASSERT(GameUI, 0xE0);

extern GameUI* _self; // NAME: proto-pdb:default.pdb

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
    void SetSubtitle(const unsigned short* ptr, float length);
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
    struct Subtitle { // NAME: proto-pdb:default.pdb
        std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > text; // 0x00
        float timeRemaining;           // 0x1C
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

    static HUDUI& Instance() { return *_self; }
private:
    static HUDUI* _self;
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
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
// !FAKE: c2 register allocation; inline keyword; an inline (on-demand COMDAT) callee gives HealthUIMain::FrameTick no
// register summary, so retail keeps _scriptToNotify in edi across the call (out of line: edx, match_one 88.5 -> 100.0)
inline bool HealthUI::CounterIsCurrent()
{
    return _drawnHealth == _destHealth;
}

// FUNCTION: USA 0x000E0C22
// PROTOTYPE: PREVIEW 0x004CBFC7 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c2 register allocation; inline keyword; an inline (on-demand COMDAT) callee gives ClustersUI::Update no
// register summary, so retail keeps _readyForMote in bl across the call (out of line: dl, match_one 97.5 -> 100.0)
inline bool ClustersUI::IsAnimating()
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

// FUNCTION: USA 0x000E0E91
// PROTOTYPE: PREVIEW 0x004CC20E identical
// NAME: proto-pdb:default.pdb
void UINode::SetOffscreenLoc(const Vector3& loc)
{
    // !FAKE: c2 register allocation; pointer local; retail holds &_offScreenLoc in ebx and spills this
    Vector3* v0 = &_offScreenLoc;
    *v0 = loc;
    SetLocation(loc);
    // !FAKE: inliner; hand form of retail's inlined Vector3::Length (out of line in our /O1 build)
    float a = v0->x * v0->x + v0->y * v0->y + v0->z * v0->z;
    a = a * FastReciprocalSquareRoot(a);
    // !FAKE: c2 register allocation; temporary; retail loads _uiZoomonTime once and squares the copy
    float tmp = gSettings._uiZoomonTime;
    a = a / (tmp * tmp * 0.5f);
    if (a != 0.0f) {
        // !FAKE: inliner; hand form of retail's inlined Vector3 scale; pointer local keeps the eax read (C0191)
        const Vector3* p = &Normalize(*v0);
        _accel = Vector3(a * p->x, a * p->y, a * p->z);
    } else {
        _accel = Vector3(0.0f, 0.0f, 0.0f);
    }
}

// FUNCTION: USA 0x000E1023
// PROTOTYPE: PREVIEW 0x004CC2D6 identical
// NAME: proto-pdb:default.pdb
void HealthUIMain::FrameTick()
{
    SwampScript* p = _scriptToNotify;
    if (p && GetUINode()->CounterIsCurrent()) {
        p->Message(this, MESSAGE_FILLED, 0);
        _scriptToNotify = 0;
    }
}

// FUNCTION: USA 0x000E1082
// PROTOTYPE: PREVIEW 0x004CC335 identical
// NAME: proto-pdb:default.pdb
void UIBeadMain::FrameTick()
{
    _moveTime += _self->_realElapsed;
    if (_moveTime >= _destTime) {
        _moveTime = _destTime;
        _owner->Message(this, UIMain::MESSAGE_BUMPCOUNTER, &_val);
        InvalidateScript();
    }
    // !FAKE: inliner; hand form of retail's inlined Vector3 arithmetic (no free operator in the PDB), block-scoped
    // temps for retail's shared stack slots, pointer local for retail's lea of _dest; D117 (match_one 90.6 -> 100.0)
    Vector3 v1;
    {
        const Vector3* p = &_dest;
        Vector3 v0(p->x - _start.x, p->y - _start.y, p->z - _start.z);
        float tmp = _moveTime / _destTime;
        v1.x = v0.x * tmp;
        v1.y = v0.y * tmp;
        v1.z = v0.z * tmp;
    }
    {
        Vector3 loc(v1.x + _start.x, v1.y + _start.y, v1.z + _start.z);
        _node->SetLocation(loc);
    }
}

// FUNCTION: USA 0x000E1151
// PROTOTYPE: PREVIEW 0x004CC404 identical
// NAME: proto-pdb:default.pdb
void BeadsUIMain::FrameTick()
{
    if (_throbSin >= 0.0f) {
        float v0 = 1.0f;
        if (_throbSin >= 0.0f && _throbSin < 3.141592653589793)
            v0 = sin(_throbSin) * 0.5f + 1.0f;
        GetUINode()->SetBeadsScale(v0);
        _throbSin += _self->_realElapsed * 12.566371f;
        if (_throbSin > 3.1415927f)
            _throbSin = -1.0f;
    } else {
        GetUINode()->SetBeadsScale(1.0f);
    }
}

// FUNCTION: USA 0x000E123A
// PROTOTYPE: PREVIEW 0x004CC4ED identical
// NAME: proto-pdb:default.pdb
void UIClusterMain::FrameTick()
{
    _moveTime += _self->_realElapsed;
    if (_moveTime >= _destTime) {
        _moveTime = _destTime;
        _owner->Message(this, UIMain::MESSAGE_BUMPCOUNTER, (void*)1);
        InvalidateScript();
    }
    // !FAKE: inliner; hand form of retail's inlined Vector3 arithmetic (no free operator in the PDB), block-scoped
    // temps for retail's shared stack slots; D117 (match_one 93.8 -> 99.9 -> 100.0)
    Vector3 v1;
    {
        Vector3 v0(_dest.x - _start.x, _dest.y - _start.y, _dest.z - _start.z);
        float tmp = _moveTime / _destTime;
        v1.x = v0.x * tmp;
        v1.y = v0.y * tmp;
        v1.z = v0.z * tmp;
    }
    {
        Vector3 loc(v1.x + _start.x, v1.y + _start.y, v1.z + _start.z);
        _node->SetLocation(loc);
    }
}

class PagesUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    void SetDigitsScale(float s) { _digitsScale = s; }
    void SetPagesScale(float s) { _pagesScale = s; }
    void RestartDefaultAnimations();
    virtual void Render(const Matrix& m) const;

    int _drawnPages;                   // 0xD4
    GatorNode* _page;                  // 0xD8
    GatorNode* _box;                   // 0xDC
    GatorNode* _bug;                   // 0xE0
    float _digitsScale;                // 0xE4
    float _pagesScale;                 // 0xE8
};
DECOMP_SIZE_ASSERT(PagesUI, 0xEC);

class PagesUIMain : public UIMain { // NAME: proto-pdb:default.pdb
public:
    virtual void FrameTick();
    PagesUI* GetUINode() { return (PagesUI*)_node; }

    char field_4C[0x8C - 0x4C];        // 0x4C
    HUDUI* _hud;                       // 0x8C
    float _throbSin;                   // 0x90
};
DECOMP_SIZE_ASSERT(PagesUIMain, 0x94);

// FUNCTION: USA 0x000E12D5
// PROTOTYPE: PREVIEW 0x004CC588 identical
// NAME: proto-pdb:default.pdb
void PagesUIMain::FrameTick()
{
    if (_throbSin >= 0.0f) {
        // !FAKE: c2 register allocation; dummy locals; retail preloads each 1.0 default on the x87 stack
        float v0 = 1.0f;
        if (_throbSin >= 0.0f && _throbSin < 3.141592653589793)
            v0 = sin(_throbSin) * 0.5f + 1.0f;
        GetUINode()->SetPagesScale(v0);
        _throbSin += _self->_realElapsed * 12.566371f;
        if (_throbSin > 5.497787f)
            _throbSin = -1.0f;
        float v1 = 1.0f;
        if (_throbSin >= 2.356194490192345)
            v1 = sin(_throbSin - 2.356194490192345) * 0.5 + 1.0;
        GetUINode()->SetDigitsScale(v1);
    } else {
        GetUINode()->SetDigitsScale(1.0f);
        GetUINode()->SetPagesScale(1.0f);
    }
}

// FUNCTION: USA 0x000E142A
// PROTOTYPE: PREVIEW 0x004CC7A7 near-identical
// NAME: proto-pdb:default.pdb
void HUDUI::DisplayMissingControllerMessageThisFrame()
{
    _displayMissingControllerMessageOnFrame = g_swamp->_frameID;
}

// FUNCTION: USA 0x000E14C4
// PROTOTYPE: PREVIEW 0x004CC815 near-identical
// NAME: proto-pdb:default.pdb
int SkullUI::GetSkullRowWidth()
{
    return pManager->_vinceCache->_maxSkulls * 18 + 12;
}

class FadeToBlack : public UINode { // NAME: proto-pdb:default.pdb
public:
    virtual void Update();
    virtual void Render(const Matrix& m) const;

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

struct Vector2 { // NAME: proto-pdb:default.pdb
    void Set(float u_, float v_) { u = u_; v = v_; }

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct UIVertex { // NAME: proto-pdb:default.pdb
    UIVertex() {}
    static unsigned int FormatFlags() { return 0x144; }

    Vector4 loc;                       // 0x00
    unsigned int color;                // 0x10
    Vector2 tex;                       // 0x14
};
DECOMP_SIZE_ASSERT(UIVertex, 0x1C);

// FUNCTION: USA 0x000E15BA
// PROTOTYPE: PREVIEW 0x004CC90B identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 95.55, bytes mismatch: renderer in edx not edi (C0128 same-TU wall); retail sinks the x/midy stores past the health clamp
void HealthUI::Render(const Matrix& m) const
{
    SwampRenderer& renderer = g_swamp->GetRenderer();
    if (IsOffscreen())
        return;
    float x = renderer.GetScreenSafeLeft() + GetLocation().x + 28.0f;
    // !FAKE: c2 float conversion; stand-in local for the cvttss2si operands, reloaded per use; no PDB local
    float v1 = _drawnHealth;
    int health = (_mm_cvtt_ss2si(_mm_load_ss(&v1)) < 0) ? 0 : (v1 = _drawnHealth, _mm_cvtt_ss2si(_mm_load_ss(&v1)));
    // !FAKE: c2 constant folding; grouped operands; the ungrouped chain folds 18 + 32 into one fadd
    float midy = 32.0f + (renderer.GetScreenSafeTop() + GetLocation().y + 18.0f);
    int lefttodraw = health;
    int totalbags = _drawnMaxHealth / 4;
    // !FAKE: c2 register allocation; dummy local; retail keeps the vertex color in edi to the last fill
    unsigned int v0 = (GetRenderingSetting(Alpha) << 24) | 0xFFFFFF;
    UIVertex v[4];
    v[0].tex.Set(0.0f, 0.0f);
    v[1].tex.Set(1.0f, 0.0f);
    v[2].tex.Set(1.0f, 1.0f);
    v[3].tex.Set(0.0f, 1.0f);
    // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
    for (int v3 = 0; v3 < 4; v3++)
        v[v3].color = v0;
    bool wasDepth = renderer.EnableDepthBuffering(false);
    renderer.SetVertexShader(UIVertex::FormatFlags());
    renderer.SetBasicPixelShader();
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 0x303);
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    for (int i = 0; i < totalbags; i++) {
        ResourceHandle<TextureResource> tex;
        float scale = 1.0f;
        int width;
        if (lefttodraw > 4) {
            tex = _lilBagFullTex;
            width = 17;
        } else if (lefttodraw == 0) {
            if (health == 0 && i == 0) {
                tex = _bigBagTex[0];
                width = 28;
            } else {
                tex = _lilBagEmptyTex;
                width = 17;
            }
        } else {
            width = 28;
            tex = _bigBagTex[(lefttodraw < 4) ? lefttodraw : 4];
            if (_throbSin >= 0.0f)
                scale = sin(_throbSin) + 1.0f;
        }
        width += 2;
        renderer.SetTexture(0, tex);
        float dy = scale * 32.0f;
        {
            float dx = width * scale * 0.5f;
            // !FAKE: c2 register allocation; stand-in locals; retail holds the quad edges and the u extent on the x87
            // stack before the tex and loc stores (as SkullUI::Render)
            float v5 = width * 0.5f + x - dx;
            float v6 = width * 0.5f + dx + x;
            float v7 = midy - dy;
            float y2 = dy + midy;
            float v8 = width / 64.0f;
            v[0].tex.Set(0.0f, 0.0f);
            v[1].tex.Set(v8, 0.0f);
            v[2].tex.Set(v8, 1.0f);
            v[3].tex.Set(0.0f, 1.0f);
            v[0].loc.Set(v5, v7, 0.0f, 1.0f);
            v[1].loc.Set(v6, v7, 0.0f, 1.0f);
            v[2].loc.Set(v6, y2, 0.0f, 1.0f);
            v[3].loc.Set(v5, y2, 0.0f, 1.0f);
            D3DDevice_DrawVerticesUP(8, 4, v, sizeof(UIVertex));
        }
        if (_drawnHealth > 0.0f && _drawnHealth <= 4.0f && lefttodraw == _drawnHealth) {
            {
                // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand; no PDB local
                float v9 = _fade * 255.0f;
                // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
                for (int v10 = 0; v10 < 4; v10++)
                    v[v10].color = (_mm_cvtt_ss2si(_mm_load_ss(&v9)) << 24) | 0xFFFFFF;
            }
            // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand; no PDB local
            float v11 = _drawnHealth;
            renderer.SetTexture(0, _cautionBagTex[_mm_cvtt_ss2si(_mm_load_ss(&v11)) - 1]);
            D3DDevice_DrawVerticesUP(8, 4, v, sizeof(UIVertex));
            // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
            for (int v12 = 0; v12 < 4; v12++)
                v[v12].color = v0;
        }
        if (i == totalbags - 1 && _newBagAlpha > 0.0f) {
            {
                // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand; no PDB local
                float v13 = _newBagAlpha * 255.0f;
                // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
                for (int v14 = 0; v14 < 4; v14++)
                    v[v14].color = (_mm_cvtt_ss2si(_mm_load_ss(&v13)) << 24) | 0xFFFFFF;
            }
            dy = _newBagScale * 32.0f;
            float dx = _newBagScale * 14;
            float cx = x + 14;
            v[0].loc.Set(cx - dx, midy - dy, 0.0f, 1.0f);
            v[1].loc.Set(cx + dx, midy - dy, 0.0f, 1.0f);
            v[2].loc.Set(cx + dx, dy + midy, 0.0f, 1.0f);
            v[3].loc.Set(cx - dx, dy + midy, 0.0f, 1.0f);
            renderer.SetTexture(0, _bigBagTex[4]);
            D3DDevice_DrawVerticesUP(8, 4, v, sizeof(UIVertex));
            // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
            for (int v15 = 0; v15 < 4; v15++)
                v[v15].color = v0;
        }
        lefttodraw -= (lefttodraw > 4) ? 4 : lefttodraw;
        x += width - 1;
    }
    renderer.EnableDepthBuffering(wasDepth);
}
#endif

// FUNCTION: USA 0x000E1EC9
// PROTOTYPE: PREVIEW 0x004CD21A identical
// NAME: proto-pdb:default.pdb
void SkullUI::Render(const Matrix& m) const
{
    // !FAKE: c2 register allocation; pointer local; retail loads g_swamp->_renderer once into edi before the early out
    SwampRenderer* p = &g_swamp->GetRenderer();
    if (IsOffscreen())
        return;
    int lefttodraw = _drawnSkulls;
    // !FAKE: c2 float reassociation; grouped operands; cl reorders the ungrouped sum (fild width first)
    float x = p->GetScreenSafeWidth() + (GetLocation().x + p->GetScreenSafeLeft()) - 28 - GetSkullRowWidth();
    // !FAKE: c2 constant folding; grouped operands; the ungrouped chain folds 18 + 32 into one fadd
    float midy = 32.0f + (p->GetScreenSafeTop() + GetLocation().y + 18.0f);
    // !FAKE: c2 register allocation; dummy local; retail keeps the vertex color in ebx to the last fill
    unsigned int v0 = (GetRenderingSetting(Alpha) << 24) | 0xFFFFFF;
    UIVertex v[4];
    v[0].tex.Set(0.0f, 0.0f);
    v[1].tex.Set(1.0f, 0.0f);
    v[2].tex.Set(1.0f, 1.0f);
    v[3].tex.Set(0.0f, 1.0f);
    // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
    for (int v1 = 0; v1 < 4; v1++)
        v[v1].color = v0;
    bool wasDepth = p->EnableDepthBuffering(false);
    p->SetVertexShader(UIVertex::FormatFlags());
    p->SetBasicPixelShader();
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 0x303);
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    for (int i = 0; i < _maxSkulls; i++) {
        ResourceHandle<TextureResource> tex;
        float scale = 1.0f;
        int width;
        if (lefttodraw > 1) {
            tex = _lilFullSkull;
            width = 17;
        } else if (lefttodraw == 0) {
            tex = _lilEmptySkull;
            width = 17;
        } else {
            width = 28;
            tex = _bigSkull;
            if (_throbSin >= 0.0f)
                scale = sin(_throbSin) + 1.0f;
        }
        width += 2;
        p->SetTexture(0, tex);
        float dy = scale * 32.0f;
        {
            float dx = width * scale * 0.5f;
            // !FAKE: c2 register allocation; stand-in locals; retail holds the quad edges and the u extent on the x87
            // stack before the tex and loc stores; this operand order also places the &v push (99.20 -> 100)
            float v5 = width * 0.5f + x - dx;
            float v6 = width * 0.5f + dx + x;
            float v7 = midy - dy;
            float y2 = dy + midy;
            float v8 = width / 64.0f;
            v[0].tex.Set(0.0f, 0.0f);
            v[1].tex.Set(v8, 0.0f);
            v[2].tex.Set(v8, 1.0f);
            v[3].tex.Set(0.0f, 1.0f);
            v[0].loc.Set(v5, v7, 0.0f, 1.0f);
            v[1].loc.Set(v6, v7, 0.0f, 1.0f);
            v[2].loc.Set(v6, y2, 0.0f, 1.0f);
            v[3].loc.Set(v5, y2, 0.0f, 1.0f);
            D3DDevice_DrawVerticesUP(8, 4, v, sizeof(UIVertex));
        }
        if (i == _maxSkulls - 1 && _newSkullAlpha > 0.0f) {
            {
                // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand (as FadeToBlack::Render); no PDB local
                float v2 = _newSkullAlpha * 255.0f;
                // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
                for (int v3 = 0; v3 < 4; v3++)
                    v[v3].color = (_mm_cvtt_ss2si(_mm_load_ss(&v2)) << 24) | 0xFFFFFF;
            }
            dy = _newSkullScale * 32.0f;
            float dx = _newSkullScale * 14;
            float cx = x + 14;
            v[0].loc.Set(cx - dx, midy - dy, 0.0f, 1.0f);
            v[1].loc.Set(cx + dx, midy - dy, 0.0f, 1.0f);
            v[2].loc.Set(cx + dx, dy + midy, 0.0f, 1.0f);
            v[3].loc.Set(cx - dx, dy + midy, 0.0f, 1.0f);
            p->SetTexture(0, _bigSkull);
            D3DDevice_DrawVerticesUP(8, 4, v, sizeof(UIVertex));
            // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
            for (int v4 = 0; v4 < 4; v4++)
                v[v4].color = v0;
        }
        if (lefttodraw != 0)
            lefttodraw--;
        x += width - 1;
    }
    p->EnableDepthBuffering(wasDepth);
}

// FUNCTION: USA 0x000E232E
// PROTOTYPE: PREVIEW 0x004CD67F identical
// NAME: proto-pdb:default.pdb
void FadeToBlack::Render(const Matrix& m) const
{
    if (!(_alpha <= 0.0f)) {
        // !FAKE: c2 register allocation; pointer local; retail loads g_swamp->_renderer once into esi
        SwampRenderer* p = &g_swamp->GetRenderer();
        bool wasDepth = p->EnableDepthBuffering(false);
        UIVertex v[4];
        v[0].tex.Set(0.0f, 0.0f);
        v[1].tex.Set(1.0f, 0.0f);
        v[2].tex.Set(1.0f, 1.0f);
        v[3].tex.Set(0.0f, 1.0f);
        {
            // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand (as Sprite.cpp); no PDB local
            float v1 = _alpha * 255.5f;
            // !FAKE: provenance; neutral stand-in loop index; the PDB records no local
            for (int v2 = 0; v2 < 4; v2++)
                v[v2].color = _mm_cvtt_ss2si(_mm_load_ss(&v1)) << 24;
        }
        p->SetEmptyTexture(0);
        p->SetVertexShader(UIVertex::FormatFlags());
        p->SetBasicPixelShader();
        D3DDevice_SetRenderState_Simple(0x40344, 0x302);
        D3DDevice_SetRenderState_Simple(0x40348, 0x303);
        D3DDevice_SetRenderState_Simple(0x40304, 1);
        const int swidth = p->GetScreenWidth();
        const int sheight = p->GetScreenHeight();
        v[0].loc.Set(0.0f, 0.0f, 0.0f, 1.0f);
        v[1].loc.Set(swidth, 0.0f, 0.0f, 1.0f);
        v[2].loc.Set(swidth, sheight, 0.0f, 1.0f);
        v[3].loc.Set(0.0f, sheight, 0.0f, 1.0f);
        D3DDevice_DrawVerticesUP(8, 4, v, sizeof(UIVertex));
        D3DDevice_SetRenderState_Simple(0x40304, 0);
        p->EnableDepthBuffering(wasDepth);
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

// FUNCTION: USA 0x000E0E4A
// PROTOTYPE: PREVIEW 0x004CC1C7 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<AreaStats,std::allocator<AreaStats> >::size()const;

// FUNCTION: USA 0x000E84B1
// PROTOTYPE: PREVIEW 0x004D376F identical
// NAME: proto-pdb:default.pdb
template bool std::vector<AreaStats,std::allocator<AreaStats> >::_Buy(unsigned int);

// FUNCTION: USA 0x000E8A7A
// PROTOTYPE: PREVIEW 0x004D3D38 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1BeadsUI@@UAE@XZ in this unit (no vptr store, so
// no declared dtor, C0029; no matched function odr-uses it); this qualified call odr-uses it instead (matched_set
// [[emitter]] row)
void pin_emit_BeadsUI_dtor(BeadsUI* p)
{
    p->BeadsUI::~BeadsUI();
}

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

// FUNCTION: USA 0x000E26C5
// PROTOTYPE: PREVIEW 0x004CDA16 identical
// NAME: proto-pdb:default.pdb
void UINode::RenderNumber(int num, float x, float y, float scale, unsigned int color, unsigned int flags) const
{
    SwampString s = format("%d", num);
    float h;
    float w;
    _font->GetTextExtent(s, &w, &h, 1);
    _font->SetScaleFactors(scale, scale);
    _font->DrawText(x, y, color, s, flags, 0.0f);
}

// FUNCTION: USA 0x000E2A7B
// PROTOTYPE: PREVIEW 0x004CDDBB identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 97.92, bytes mismatch: retail schedules the loc.z immediate store one slot earlier (before fsub 32)
void LivesUI::Render(const Matrix& m) const
{
    // !FAKE: c2 register allocation; pointer local; retail loads g_swamp->_renderer once into edi before the early out
    SwampRenderer* p = &g_swamp->GetRenderer();
    if (IsOffscreen())
        return;
    const float LIVES_X = p->GetScreenSafeRight() + GetLocation().x - 32.0f;
    const float LIVES_Y = p->GetScreenSafeTop() + GetLocation().y + 260.0f;
    // !FAKE: inliner; hand form of a scaled Vector3 temp; retail keeps the x product on the x87 stack (91.04 -> 97.92)
    Vector3 v1(GetLocation().x * -0.01f, GetLocation().y * -0.01f, GetLocation().z * -0.01f);
    Vector3 loc(v1.x - 1.33f, v1.y - 0.55f, -5.75f);
    _heart->SetLocation(loc);
    bool wasDepth = p->EnableDepthBuffering(false);
    p->SetVertexShader(0x144);
    p->SetBasicPixelShader();
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 0x303);
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    // !FAKE: c2 register allocation; dummy local; retail keeps the Alpha setting in ebx to the last call
    unsigned int v0 = GetRenderingSetting(Alpha);
    RenderNumber(_drawnHearts, LIVES_X, LIVES_Y, 1.0f, (v0 << 24) | 0xFFFFFF, 6);
    p->EnableDepthBuffering(wasDepth);
    Matrix m2;
    m2.Translation(_heart->GetLocation());
    _heart->Render(m2);
    _heart->SetRenderingSetting(Alpha, v0, true);
}
#endif

// FUNCTION: USA 0x000E2BE8
// PROTOTYPE: PREVIEW 0x004CDF28 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> > & std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> >::append(std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> > const &,unsigned int,unsigned int);

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

// FUNCTION: USA 0x000E2DA5
// PROTOTYPE: PREVIEW 0x004CE0FE identical
// NAME: proto-pdb:default.pdb
template std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> > & std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> >::append(unsigned short const *,unsigned int);

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

// FUNCTION: USA 0x000E2FFE
// PROTOTYPE: PREVIEW 0x004CE357 identical
// NAME: proto-pdb:default.pdb
void UINode::ResetOnscreenTimer()
{
    if (HUDUI::Instance().IsVisible()) {
        if (_vanishOffScreen)
            SetOption(DontDraw, false, true);
        if (_onScreenTimer <= 0.0f && _onScreenTimer < 0.0f) {
            SoundManager::PlaySound("intflyon", 0, 0, false, 0.0f, 0.0f);
            // !FAKE: inliner; hand form of retail's inlined Vector3 negate and scale; pointer local for retail's lea
            const Vector3* p = &_accel;
            Vector3 v0(-p->x, -p->y, -p->z);
            _speed = Vector3(gSettings._uiZoomonTime * v0.x, gSettings._uiZoomonTime * v0.y,
                gSettings._uiZoomonTime * v0.z);
        }
        _onScreenTimer = _onScreenTime;
    }
}

// FUNCTION: USA 0x000E3D01
// PROTOTYPE: PREVIEW 0x004CF051 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0) (4.4 T4.c5; vtable now pinned); wall: relink LINK exit 1169,
// Onion.obj LNK2005 ?SetScale@SwampSceneNode@@QAEXM@Z already defined in HUDUI.obj: the inlined SetScale(float) hand
// form leaves a COMDAT referenced only from .debug$F (__forceinline too) beside Onion's out-of-line matched row 0x00018092
UIClusterMain::UIClusterMain(SwampSceneNode* node) : SwampScript(node)
{
    node->SetScale(0.5f);
    ((GatorNode*)node)->StartAnimation(g_swamp->GetAnimationManager().Load("dustBag_idle"), true, false);
    _start = node->GetLocation();
    _moveTime = 0.0f;
    _dest = Vector3(-1.33f, 0.365f, -5.75f);
    _destTime = gSettings._uiZoomonTime;
    RegisterFrameTick();
}
#endif

// FUNCTION: USA 0x000E3F6F
// PROTOTYPE: PREVIEW 0x004CF2BF identical
// NAME: proto-pdb:default.pdb
void ClustersUI::Update()
{
    UINode::Update();
    if (_readyForMote && _queueMotes) {
        _queueMotes--;
        _throbSin = 0.0f;
        _readyForMote = false;
        Add();
        return;
    }
    if (IsAnimating()) {
        _throbSin += _self->_realElapsed * 12.566371f;
        if (_throbSin >= 1.5707964f) {
            if (!_readyForMote) {
                _drawnMotes++;
                _readyForMote = true;
            }
            if (_throbSin > 5.497787f)
                _throbSin = -1.0f;
        }
        float scale = 1.0f;
        if (IsAnimating() && _throbSin < 3.141592653589793)
            scale = sin(_throbSin) * 0.5f + 1.0f;
        _clustersScale = scale;
        scale = 1.0f;
        if (_throbSin >= 1.5707963267948966) {
            // !FAKE: c2 x87 code; temporary; the double difference clamped to pi by the C0220 ternary before fsin
            double tmp = _throbSin - 1.5707963267948966;
            scale = sin(3.141592653589793 < tmp ? 3.141592653589793 : tmp) * 0.5 + 1.0;
        }
        _digitsScale = scale;
    } else {
        _digitsScale = 1.0f;
        _clustersScale = 1.0f;
    }
}

// FUNCTION: USA 0x000E40BD
// PROTOTYPE: PREVIEW 0x004CF40D identical
// NAME: proto-pdb:default.pdb
void ClustersUIMain::AnimationFinished()
{
    GetUINode()->_bag->StartAnimation(g_swamp->GetAnimationManager().Load("dustbag_idle"), true, false);
}

// FUNCTION: USA 0x000E42BE
// PROTOTYPE: PREVIEW 0x004CF60E identical
// NAME: proto-pdb:default.pdb
void PagesUI::RestartDefaultAnimations()
{
    _page->StartAnimation(g_swamp->GetAnimationManager().Load("interfacecharmpage_idle"), true, false);
    _box->StartAnimation(g_swamp->GetAnimationManager().Load("interfacemysterybox_idle"), true, false);
    _bug->StartAnimation(g_swamp->GetAnimationManager().Load("interfacegoldenjunebug_idle"), true, false);
}

// FUNCTION: USA 0x000E447C
// PROTOTYPE: PREVIEW 0x004CF7CC identical
// NAME: proto-pdb:default.pdb
void LivesUI::Update()
{
    UINode::Update();
    if (_throbSin >= 0.0f) {
        _throbSin += _self->_realElapsed * 3.14159265f;
        if (_throbSin > 3.141592653589793)
            _throbSin = -1.0f;
    }
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

// FUNCTION: USA 0x000E6F41
// PROTOTYPE: PREVIEW 0x004D224A identical
// NAME: proto-pdb:default.pdb
void UIBonusBagMain::FrameTick()
{
    float elapsed = g_swamp->_input->GetElapsedTime();
    HUDUI::Instance().FindUINode("health")->ResetOnscreenTimer();
    switch (_state) {
    case 0:
        if (_moveNow && _animDone) {
            SetState(1);
            _moveTime = 0.0f;
            _destTime = gSettings._uiZoomonTime;
        }
        break;
    case 1: {
        _moveTime += elapsed;
        if (_moveTime >= _destTime) {
            _moveTime = _destTime;
            HUDUI::Instance().FindUINode("health")->GetScript()->Message(this, UIMain::MESSAGE_FILLED, 0);
            HUDUI::Instance().FindUINode("clusters")->GetScript()->Message(this, UIMain::MESSAGE_FILLED, 0);
            InvalidateScript();
        }
        // !FAKE: inliner; hand form of retail's inlined Vector3 arithmetic (no free operator in the PDB), block-scoped
        // temps for retail's shared stack slots, pointer local for retail's lea of _dest; D117 (as UIBeadMain::FrameTick)
        Vector3 v1;
        {
            const Vector3* p = &_dest;
            Vector3 v0(p->x - _start.x, p->y - _start.y, p->z - _start.z);
            float tmp = _moveTime / _destTime;
            v1.x = v0.x * tmp;
            v1.y = v0.y * tmp;
            v1.z = v0.z * tmp;
        }
        {
            Vector3 loc(v1.x + _start.x, v1.y + _start.y, v1.z + _start.z);
            _node->SetLocation(loc);
        }
        break;
    }
    }
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

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1HealthUI@@UAE@XZ in this unit (no matched
// function odr-uses it; the unsymboled FUN_000E892F after it calls ??0UINode); this qualified call odr-uses it instead
// (matched_set [[emitter]] row)
void pin_emit_HealthUI_dtor(HealthUI* p)
{
    p->HealthUI::~HealthUI();
}

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

class LocalizedStrings { // NAME: proto-pdb:default.pdb
public:
    static LocalizedStrings& Instance();
    const unsigned short* GetGameString(const SwampString& str);
};

// FUNCTION: USA 0x000E963E
// NAME: proto-pdb:default.pdb
void HUDUISoundTriggerCallback(const SwampString& soundName, float length)
{
    const unsigned short* tmp = LocalizedStrings::Instance().GetGameString(soundName);
    if (*tmp)
        HUDUI::Instance().SetSubtitle(tmp, length);
}

class Level { // NAME: proto-pdb:default.pdb
public:
    int GetPagesFound() const { return _stats._pagesFound; }
    int GetPagesRemaining() const { return _stats._pagesRemaining; }
    const AreaStats& GetAreaStats() const { return _stats; }

    char field_0[0x3C];                // 0x00
    AreaStats _stats;                  // 0x3C
    char field_88[0x140 - 0x88];       // 0x88
};
DECOMP_SIZE_ASSERT(Level, 0x140);

class LevelStats { // NAME: proto-pdb:default.pdb
public:
    LevelStats(const LevelStats&);
    ~LevelStats();
    int GetAreaCount() { return _areaStatsList.size(); }

    SwampString _name;                 // 0x00
    std::vector<AreaStats, std::allocator<AreaStats> > _areaStatsList; // 0x1C
};
DECOMP_SIZE_ASSERT(LevelStats, 0x2C);

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }
    std::vector<LevelStats, std::allocator<LevelStats> >* GetLevelStatsList() { return &_levelStatsList; }

    HUDUI* _hudUI;                     // 0x00
    Level* _curLevel;                  // 0x04
    char field_8[0x1B0 - 0x8];         // 0x08
    std::vector<LevelStats, std::allocator<LevelStats> > _levelStatsList; // 0x1B0
};

// FUNCTION: USA 0x000EA1DD
// PROTOTYPE: PREVIEW 0x004D548D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.26, bytes mismatch: retail schedules the loc.z immediate store before fadd (ours 3 later)
void PagesUI::Render(const Matrix& m) const
{
    // !FAKE: c2 register allocation; pointer locals; retail loads the renderer into edi and the level into ebx once
    SwampRenderer* p = &g_swamp->GetRenderer();
    Level* v0 = LevelLoader::Instance().GetCurLevel();
    if (IsOffscreen())
        return;
    const float PAGES_X = p->GetScreenSafeRight() + GetLocation().x - 32.0f;
    const float PAGES_Y = p->GetScreenSafeTop() + GetLocation().y + 199.0f;
    // !FAKE: inliner; hand form of a scaled Vector3 temp; retail keeps the x product on the x87 stack (C0220 kin)
    Vector3 v1(GetLocation().x * -0.01f, GetLocation().y * -0.01f, GetLocation().z * -0.01f);
    Vector3 loc(v1.x - 1.33f, v1.y - 0.12f, -5.75f);
    _page->SetLocation(loc);
    _box->SetLocation(loc);
    _bug->SetLocation(loc);
    bool wasDepth = p->EnableDepthBuffering(false);
    p->SetVertexShader(0x144);
    p->SetBasicPixelShader();
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 0x303);
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    RenderNumber(v0->GetAreaStats()._juneBugsFound ? 1
        : (v0->GetPagesFound() && !v0->GetPagesRemaining() ? 0 : v0->GetPagesFound()),
        PAGES_X, PAGES_Y, _digitsScale, 0xFFFFFFFF, 6);
    p->EnableDepthBuffering(wasDepth);
    Matrix m2;
    m2.Translation(_page->GetLocation());
    (v0->GetPagesFound() && !v0->GetPagesRemaining() ? _box : _page)->Render(m2);
}
#endif

class PauseUI : public UINode { // NAME: proto-pdb:default.pdb
private:
    void UpdateCounts();

public:
    char field_D4[0x1D8 - 0xD4];       // 0xD4
    int _curLevel;                     // 0x1D8
    int _curArea;                      // 0x1DC
    AreaStats _curStats;               // 0x1E0
    SwampString _areaName;             // 0x22C
    SwampString _levelName;            // 0x248
    std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > _displayAreaName; // 0x264
    std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > _displayLevelName; // 0x280
    float _leftThrobSin;               // 0x29C
    float _rightThrobSin;              // 0x2A0
    int _areaCount;                    // 0x2A4
    int _levelCount;                   // 0x2A8
    char field_2AC[0x2BC - 0x2AC];     // 0x2AC
};
DECOMP_SIZE_ASSERT(PauseUI, 0x2BC);

// FUNCTION: USA 0x000EA619
// PROTOTYPE: PREVIEW 0x004D58C9 identical
// NAME: proto-pdb:default.pdb
void PauseUI::UpdateCounts()
{
    LevelStats lstats = (*LevelLoader::Instance().GetLevelStatsList())[_curLevel];
    _levelName = lstats._name;
    AreaStats astats = lstats._areaStatsList[_curArea];
    _areaName = astats._name;
    _displayAreaName = LocalizedStrings::Instance().GetGameString(_areaName);
    _displayLevelName = LocalizedStrings::Instance().GetGameString(_levelName);
    _curStats.Copy(astats);
    _areaCount = lstats.GetAreaCount();
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0) on both rows (4.4 T4.c5; throw-info now pinned); wall: relink
// refuses `.rdata 0x001FCEF4..0x001FCF80: retail bytes between two of our .xdata$x sections (one LINK group cannot
// interleave with them)`: the length_error throw-info lands in .xdata$x; and ??_7logic_error/??_R0 logic_error COMDAT
// data overlap (0x001C77E8) when Init.obj's out_of_range throw rows are live too (relink places one copy per unit)
// FUNCTION: USA 0x000E6BCF
// PROTOTYPE: PREVIEW 0x004D1EA0 identical
// NAME: proto-pdb:default.pdb
template void std::list<HUDUI::QuickMessage,std::allocator<HUDUI::QuickMessage> >::_Incsize(unsigned int);

// FUNCTION: USA 0x000E6C1F
// PROTOTYPE: PREVIEW 0x004D1F28 identical
// NAME: proto-pdb:default.pdb
template void std::list<HUDUI::Subtitle,std::allocator<HUDUI::Subtitle> >::_Incsize(unsigned int);
#endif
