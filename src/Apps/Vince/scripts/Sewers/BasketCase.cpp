// UNIT: src/Apps/Vince/scripts/Sewers/BasketCase.cpp OBJ: Xbox\OfficialRelease\BasketCase.obj OPT: /O1 /Oy
#include <math.h>
#include "../../../../include/decomp.h"

// !FAKE: c2 intrinsics; file-scope pragma; retail inlines fsin in LaundryBitsHud::Render under /O1 /Oy (call _sin without)
#pragma intrinsic(sin)

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
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
template <class T, class A = allocator<T> > class vector { // NAME: proto-pdb:default.pdb
public:
    typedef T* iterator;
    // !FAKE: inliner; hand form of the VC7.1 body; vector() { _Buy(0); } folds to these three stores (C0191)
    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; }
    ~vector() { _Tidy(); }
    iterator begin() { return _Myfirst; }
    iterator end() { return _Mylast; }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
typedef std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > SwampWString;

class BeepTDF;              // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Matrix;              // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    void Normalize();
    float LengthSquared() const { return x * x + y * y + z * z; }
    Vector3& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    float x;                          // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

struct Vector2 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3&, Vector3&) const;
    Quaternion& operator*=(const Quaternion&);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    float GetLength() const { return _length; }

    char field_24[0x8];                // 0x24
    float _length;                     // 0x2C
    char field_30[0x4];                // 0x30
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();

    char field_24[0x68 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class SwampGamePad { // NAME: proto-pdb:default.pdb
public:
    bool WasButtonPressed(int) const;
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }
    float GetGameTime() const { return _gameTimePublic; }
    bool IsGamePaused() const { return _pauseGame; }
    const SwampGamePad* GetPad(int) const;

    float _elapsed;                    // 0x00
    char field_4[0x4];                 // 0x04
    float _gameTimePublic;             // 0x08
    char field_C[0x28];                // 0x0C
    bool _pauseGame;                   // 0x34
    char field_35[0xEB];               // 0x35
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    int GetScreenWidth() const { return _screenWidth; }
    int GetScreenHeight() const { return _screenHeight; }

    char field_0[0x2C];                // 0x00
    int _screenWidth;                  // 0x2C
    int _screenHeight;                 // 0x30
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    SwampRenderer& GetRenderer() const { return *_renderer; }
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0xC];                 // 0x0C
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1,
        DontDraw = 0x2,
        NoCull = 0x4,
        UseAlpha = 0x8,
        SortedRender = 0x10,
        DeferredButNotSorted = 0x20,
        DrawEarly = 0x40,
        Wireframe = 0x80,
        ShowBoundingBox = 0x100,
        Unlit = 0x200,
        UpdateOnlyOnRender = 0x400,
        ReflectMe = 0x800,
        RenderRealTime = 0x1000,
        LocationSorting = 0x2000,
        IsACollidableType = 0x10000000
    };
    SwampSceneNode(int);
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
public:
    void SetOption(unsigned int option, bool on, bool recurse);
    SwampScript* GetScript() const;
    Vector3 GetFacing() const;
    const Quaternion& SetRotation(const Quaternion&);
    const Quaternion& SetFacing(const Vector3&);

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetCollidable(bool collidable) { _collidable = collidable; }

    char field_A4[0x4];                // 0xA4
    bool _collidable;                  // 0xA8
    char field_A9[0x3];                // 0xA9
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    float GetHeight() const { return _height; }

    float _height;                     // 0xAC
    char field_B0[0x4];                // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    ResourceHandle<AnimationControl> TransitionAnimation(ResourceHandle<AnimationResource>, float, bool, bool);
    void SetAnimSpeed(float);
    void GetBoneOrientation(int, Quaternion&, Vector3&) const;
    char field_B4[0x14C];              // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0xC8];              // 0x200
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

// Retail layout: Vince _whammyReady 0x434 -> 0x448 (+0x14).
class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetWhammyReady(bool);
    void SetTargetLocation(const SwampString&, bool);

    char field_2C8[0x448 - 0x2C8];     // 0x2C8
    bool _whammyReady;                 // 0x448
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    void FindAllMatching(std::vector<Critter*>&, const SwampString&, unsigned int);
    Critter* Find(const SwampString&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

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

    SwampSceneNode* GetNode() { return _node; }
    void UnRegisterFrameTick();
protected:
    SwampScript(SwampSceneNode* node);
public:

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    Critter* GetCritter() { return (Critter*)GetNode(); }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

// FUNCTION: USA 0x000A27CE
// NAME: proto-pdb:default.pdb
class VinceLaundryCar : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_RIDING = 1,
        STATE_DYING = 2,
        STATE_DEAD = 3,
        STATE_FINISHED = 4,
        STATE_MENU = 5
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_RIDEOVER_FAIL = 0x14,
        MESSAGE_RIDEOVER_DONE = 0x15,
        MESSAGE_REACHED_BOTTOM = 0x16,
        MESSAGE_MENU_TRYAGAIN = 0x17,
        MESSAGE_MENU_LEAVE = 0x18,
        MESSAGE_CRASHED = 0x19,
        MESSAGE_SWITCHED_LINES = 0x1A,
        MESSAGE_SWITCHED_SEGMENTS = 0x1B,
        MESSAGE_GET_LAUNDRY = 0x1C,
        MESSAGE_RESTART = 0x1D
    };
    VinceLaundryCar(SwampSceneNode* node);
    virtual void FrameTick();
    virtual void AnimationFinished();
    virtual void InvalidateScript();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int);
    void SetupRide();
    static bool _tryagainmenu;
public:
    Critter* _laundryCar;              // 0xBC
    int _resetcam;                     // 0xC0
    bool _reachedBottom;               // 0xC4
    bool _success;                     // 0xC5
    bool _wasWhammyReady;              // 0xC6
    ResourceHandle<AnimationResource> _animRide;  // 0xC8
    ResourceHandle<AnimationResource> _animCrash; // 0xCC
};
DECOMP_SIZE_ASSERT(VinceLaundryCar, 0xD0);

class LaundryCarMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_RIDING = 1,
        STATE_CRASH = 2,
        STATE_DYING = 3,
        STATE_MENU = 4,
        STATE_END = 5
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_START = 0x14,
        MESSAGE_DONE = 0x15,
        MESSAGE_RESET = 0x16,
        MESSAGE_DYING = 0x17,
        MESSAGE_QUERY_LAUNDRYBITS = 0x18
    };
    enum SetLineTransition { // NAME: proto-pdb:default.pdb
        SETLINE_SNAP = 0,
        SETLINE_JUMP = 1,
        SETLINE_CORNER = 2
    };
    LaundryCarMain(SwampSceneNode* node);
    virtual void FrameTick();
    virtual void AnimationFinished();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void ResetLaundry();
    bool SetLine(int segment, int line, int transition);
public:

    int _laundryBits;                  // 0xBC
    SwampSceneNode* _hud;              // 0xC0
    Critter* _rider;                   // 0xC4
    int _bone;                         // 0xC8
    int _segment;                      // 0xCC
    int _line;                         // 0xD0
    float _speed;                      // 0xD4
    float _linet;                      // 0xD8
    float _lineLength;                 // 0xDC
    Vector3 _lineStart;                // 0xE0
    Vector3 _lineDir;                  // 0xEC
    Vector3 _prevLineStart;            // 0xF8
    float _jumpPct;                    // 0x104
    ResourceHandle<AnimationResource> _animRide;        // 0x108
    ResourceHandle<AnimationResource> _animJump;        // 0x10C
    ResourceHandle<AnimationResource> _animCornerRight; // 0x110
    ResourceHandle<AnimationResource> _animCornerLeft;  // 0x114
    char field_118[0x11C - 0x118];     // 0x118
    bool _switchingLines;              // 0x11C
    char field_11D[0x120 - 0x11D];     // 0x11D
};
DECOMP_SIZE_ASSERT(LaundryCarMain, 0x120);

class BasketStopperMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    BasketStopperMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void AnimationFinished();
    GatorNode* GetStopper() { return (GatorNode*)GetNode(); }

    ResourceHandle<AnimationResource> _idle; // 0x4C
    ResourceHandle<AnimationResource> _hit;  // 0x50
};
DECOMP_SIZE_ASSERT(BasketStopperMain, 0x54);

void* operator new(unsigned int size, const char* file, int line);

class BasketStopperPanMain : public BasketStopperMain { // NAME: proto-pdb:default.pdb
public:
    BasketStopperPanMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BasketStopperPanMain, 0x54);

class CXBFont { // NAME: proto-pdb:default.pdb
public:
    void SetScaleFactors(float, float);
    long DrawText(float, float, unsigned long, const SwampString&, unsigned long, float);
    long DrawText(float, float, unsigned long, const unsigned short*, unsigned long, float);
};

class FontManager { // NAME: proto-pdb:default.pdb
public:
    CXBFont* Load(const char*);
};

FontManager& GetFontManager(); // NAME: proto-pdb:default.pdb

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0xC];                // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

class Sprite { // NAME: proto-pdb:default.pdb
public:
    Sprite();
    void SetTexture(const SwampString&);
    void Render() const;
    void SetLocation(int x, int y) { _loc.x = (float)x; _loc.y = (float)y; }

    Vector2 _loc;                      // 0x00
    char field_8[0x10];                // 0x08
    ResourceHandle<TextureResource> _texture; // 0x18
    char field_1C[0x30];               // 0x1C
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class HUDUI { // NAME: proto-pdb:default.pdb
public:
    static HUDUI& Instance() { return *_self; }
    bool IsLetterboxed() const { return _letterboxEnabled; }

    char field_0[0xE4];                // 0x00
    bool _letterboxEnabled;            // 0xE4
    char field_E5[0x1CB];              // 0xE5
private:
    static HUDUI* _self;
};
DECOMP_SIZE_ASSERT(HUDUI, 0x2B0);

class GameCam { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x11C];               // 0x00
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void ResetLocation(bool);
    void SetDirectionFixed(const Vector3&, float);
    void ClearDirectionFixed() { _directionFixed = false; }
    void UnholdTarget() { _holdTarget = false; }
    void SetOrbitRadius(float r) { _orbitRadius = r; }
    void EnableTrigger(bool on) { _triggerEnabled = on; }
    void SetHSpeedFactor(float f) { _hspeedFactor = f; }
    void SetVSpeedFactor(float f) { _vspeedFactor = f; }
    void SetTurnSpeedFactor(float f) { _turnspeedFactor = f; }
    void SetHMoveMax(float m) { _hmoveMax = m; }
    void SetVMoveMax(float m) { _vmoveMax = m; }
    void SetTurnMax(float m) { _turnMax = m; }
    void HoldTarget(const Vector3& pos) { _holdTarget = true; _heldTargetPos = pos; }

    char field_11C[0x5];               // 0x11C
    bool _triggerEnabled;              // 0x121
    char field_122[0x2];               // 0x122
    bool _directionFixed;              // 0x124
    char field_125[0x154 - 0x125];     // 0x125
    float _orbitRadius;                // 0x154
    char field_158[0x1A0 - 0x158];     // 0x158
    float _hspeedFactor;               // 0x1A0
    float _vspeedFactor;               // 0x1A4
    float _turnspeedFactor;            // 0x1A8
    float _hmoveMax;                   // 0x1AC
    float _vmoveMax;                   // 0x1B0
    float _turnMax;                    // 0x1B4
    char field_1B8[0x210 - 0x1B8];     // 0x1B8
    bool _holdTarget;                  // 0x210
    char field_211[0x3];               // 0x211
    Vector3 _heldTargetPos;            // 0x214
    char field_220[0x18];              // 0x220
};
DECOMP_SIZE_ASSERT(VinceCam, 0x238);

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class Locator { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetLocation() const { return _pos; }

    Vector3 _pos;                      // 0x00
    char field_C[0x48 - 0xC];          // 0x0C
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    Locator* Find(const SwampString& s) const;
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x87C];               // 0x00
    float _basketCaseMinSpeed;         // 0x87C
    float _basketCaseMaxSpeed;         // 0x880
    float _basketCaseAcceleration;     // 0x884
    float _basketCaseCameraSpeed;      // 0x888
    float _basketCaseCameraDist;       // 0x88C
    float _basketCaseCameraFocusPos;   // 0x890
    float _basketCaseJumpAdjust;       // 0x894
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

class Level { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() { return _script; }

    char field_0[0xC];                 // 0x00
    SwampScript* _script;              // 0x0C
};

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    enum LoadType { // NAME: proto-pdb:default.pdb
        LOAD_VOODOOSHOP = 0,
        LOAD_AREA = 1,
        LOAD_CHAPTER = 2
    };
    void RequestArea(const SwampString&, const SwampString&, bool, float, bool);
    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }
    bool IsChapterLoad() const { return _loadType == LOAD_CHAPTER; }

    char field_0[0x4];                 // 0x00
    Level* _curLevel;                  // 0x04
    char field_8[0x3C];                // 0x08
    LoadType _loadType;                // 0x44
};

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    int _RefCount;                     // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    char field_4[0x40 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }

    T* _Object;                        // 0x00
};

class SwampCollide { // NAME: proto-pdb:default.pdb
public:
    static Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float, const GatorNode*, unsigned int);
};

extern "C" int __cdecl sprintf(char*, const char*, ...); // NAME: proto-pdb:default.pdb

class LocalizedStrings { // NAME: proto-pdb:default.pdb
public:
    static LocalizedStrings& Instance();
    const unsigned short* GetGameString(const SwampString& str);
};

class BasketCaseTryAgainMenu : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    BasketCaseTryAgainMenu(SwampScript* script);
    virtual void Render(const Matrix&) const;
    virtual void Update();

    CXBFont* _font;                    // 0xA4
    SwampScript* _returnScript;        // 0xA8
};
DECOMP_SIZE_ASSERT(BasketCaseTryAgainMenu, 0xAC);

class LaundryBitsHud : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    LaundryBitsHud();
    virtual void Render(const Matrix&) const;
    virtual void Update();

    CXBFont* _font;                    // 0xA4
    Sprite _laundryIcon;               // 0xA8
    float _throbSin;                   // 0xF4
    int _laundryBits;                  // 0xF8
};
DECOMP_SIZE_ASSERT(LaundryBitsHud, 0xFC);

// FUNCTION: USA 0x000A1500
// NAME: proto-pdb:default.pdb
BasketStopperMain::BasketStopperMain(SwampSceneNode* node)
    : SwampScript(node)
{
}

// FUNCTION: USA 0x000A1520
// NAME: proto-pdb:default.pdb
void BasketStopperMain::Activate()
{
    GetStopper()->StartAnimation(_hit, false, false);
    GetStopper()->SetCollidable(false);
}

// FUNCTION: USA 0x000A153F
// PROTOTYPE: PREVIEW 0x0048C5EE identical
// NAME: proto-pdb:default.pdb
void BasketStopperMain::AnimationFinished()
{
    GetStopper()->StartAnimation(_idle, true, false);
    GetStopper()->SetCollidable(true);
}

// FUNCTION: USA 0x000A155E
// NAME: proto-pdb:default.pdb
void LaundryCarMain::AnimationFinished()
{
    GetCritter()->StartAnimation(_animRide, true, false);
}

// FUNCTION: USA 0x000A1572
// PROTOTYPE: PREVIEW 0x0048C621 identical
// NAME: proto-pdb:default.pdb
void VinceLaundryCar::AnimationFinished()
{
    if (_state == STATE_DYING)
        SetState(STATE_DEAD);
}

// FUNCTION: USA 0x000A1580
// NAME: proto-pdb:default.pdb
void VinceLaundryCar::InvalidateScript()
{
    ((Vince*)_node)->SetWhammyReady(_wasWhammyReady);
    CritterMain::InvalidateScript();
}

// FUNCTION: USA 0x000A159C
// PROTOTYPE: PREVIEW 0x0048C667 identical
// NAME: proto-pdb:default.pdb
bool LaundryCarMain::SetLine(int segment, int line, int transition)
{
    LocatorManager& mgr = *g_locatorManager;
    Locator* start = mgr.Find(format("locator_line%d%c_b", line + 1, segment + 'A'));
    if (!start)
        return false;
    _segment = segment;
    _line = line;
    Locator* end = mgr.Find(format("locator_line%d%c_e", line + 1, segment + 'A'));
    Vector3 prevLineStart = _lineStart;
    _lineStart = start->GetLocation();
    // !FAKE: inliner; pointer locals to the operands (C0191); retail subtracts through ecx/eax bases
    const Vector3* v1 = &end->GetLocation();
    const Vector3* v2 = &_lineStart;
    Vector3 diff(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
    // !FAKE: inliner; hand form of the ApproxLength body (D117); /O1 keeps the inline call out of line
    float v3 = diff.LengthSquared();
    _lineLength = FastReciprocalSquareRoot(v3) * v3;
    diff.Normalize();
    Vector3 oldLineDir = _lineDir;
    // !FAKE: c2 addressing; pointer local; retail reads _lineDir through eax in the corner test (99.97 without it)
    Vector3* v4 = &_lineDir;
    *v4 = diff;
    switch (transition) {
    case SETLINE_SNAP:
        GetCritter()->StartAnimation(_animRide, true, false);
        break;
    case SETLINE_JUMP:
        GetCritter()->TransitionAnimation(_animJump, 0.1f, false, false);
        GetCritter()->SetAnimSpeed(gSettings._basketCaseJumpAdjust);
        _jumpPct = 0.0f;
        _prevLineStart = prevLineStart;
        _switchingLines = true;
        break;
    case SETLINE_CORNER:
        if (oldLineDir.z * v4->x - oldLineDir.x * v4->z > 0.0f)
            GetCritter()->StartAnimation(_animCornerRight, false, false);
        else
            GetCritter()->StartAnimation(_animCornerLeft, false, false);
        if (_switchingLines) {
            _switchingLines = false;
            _jumpPct = 1.0f;
        }
        break;
    }
    return true;
}

// FUNCTION: USA 0x000A179C
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1: vptr ??_7BasketCaseTryAgainMenu@@6B@ unpinned, retail 0x001E84F4)
BasketCaseTryAgainMenu::BasketCaseTryAgainMenu(SwampScript* script)
    : SwampSceneNode(9), _returnScript(script)
{
    _font = GetFontManager().Load("pauseCounters");
    SoundManager::PlaySound("frontdialog", 0, 0, false, 0.0f, 0.0f);
}
#endif

// FUNCTION: USA 0x000A1811
// PROTOTYPE: PREVIEW 0x0048C8DC identical
// NAME: proto-pdb:default.pdb
void BasketCaseTryAgainMenu::Update()
{
    if (g_swamp->GetInput().IsGamePaused())
        return;
    // !FAKE: c2 register allocation; neutral stand-in local; the pad pointer held in edi across both tests
    const SwampGamePad* p = g_swamp->GetInput().GetPad(0);
    if (p->WasButtonPressed(8)) {
        SoundManager::PlaySound("frontbutton", 0, 0, false, 0.0f, 0.0f);
        _returnScript->Message(0, VinceLaundryCar::MESSAGE_MENU_TRYAGAIN, 0);
        Invalidate();
    } else if (p->WasButtonPressed(9)) {
        SoundManager::PlaySound("frontbutton", 0, 0, false, 0.0f, 0.0f);
        _returnScript->Message(0, VinceLaundryCar::MESSAGE_MENU_LEAVE, 0);
        Invalidate();
    }
}

// FUNCTION: USA 0x000A18D9
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1: literal ??_C@_0L@ELICBOBH@bctryagain?$AA@ unpinned, retail 0x001E8518)
void BasketCaseTryAgainMenu::Render(const Matrix& m) const
{
    SwampWString s = LocalizedStrings::Instance().GetGameString("bctryagain");
    _font->DrawText(g_swamp->GetRenderer().GetScreenWidth() * 0.5f, g_swamp->GetRenderer().GetScreenHeight() * 0.5f, 0xFFFFFFFF, s.c_str(), 2, 0.0f);
}
#endif

// FUNCTION: USA 0x000A196A
// PROTOTYPE: PREVIEW 0x0048CA35 identical
// NAME: proto-pdb:default.pdb
LaundryBitsHud::LaundryBitsHud()
    : SwampSceneNode(9), _throbSin(-1.0f), _laundryBits(-1)
{
    _font = GetFontManager().Load("counters");
    _laundryIcon.SetTexture("laundryIcon");
    _laundryIcon.SetLocation(448, 368);
}

// FUNCTION: USA 0x000A19ED
// PROTOTYPE: PREVIEW 0x0048CAE6 identical
// NAME: proto-pdb:default.pdb
void LaundryBitsHud::Update()
{
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    // !FAKE: c2 register allocation; neutral stand-in local; the critter pointer held in ebx past the string dtor
    Critter* p = pManager->Find("laundrycar");
    int laundryBits = 0;
    p->GetScript()->Message(0, LaundryCarMain::MESSAGE_QUERY_LAUNDRYBITS, &laundryBits);
    if (_laundryBits > 0 && _laundryBits != laundryBits)
        _throbSin = 0.0f;
    _laundryBits = laundryBits;
    if (_throbSin >= 0.0f) {
        _throbSin += elapsed * 16.0f;
        if (_throbSin > 3.14159265f)
            _throbSin = -1.0f;
    }
}

// FUNCTION: USA 0x000A1AA8
// PROTOTYPE: PREVIEW 0x0048CBA1 identical
// NAME: proto-pdb:default.pdb
void LaundryBitsHud::Render(const Matrix& m) const
{
    if (HUDUI::Instance().IsLetterboxed())
        return;
    _laundryIcon.Render();
    char buffer[128];
    sprintf(buffer, "%i", _laundryBits);
    // !FAKE: c2 register allocation; neutral stand-in local; one x87 value feeds both scale arguments
    float v0 = 1.0f;
    if (_throbSin >= 0.0f)
        // !FAKE: inliner; hand form of math.h sin(float); its header body predates the pragma (objdiff 91.76 -> 100.0)
        v0 = (float)sin((double)_throbSin) * 0.5f + 1.0f;
    _font->SetScaleFactors(v0, v0);
    _font->DrawText(541.0f, 400.0f, 0xFFFFFFFF, buffer, 0, 0.0f);
    _font->SetScaleFactors(1.0f, 1.0f);
}

// FUNCTION: USA 0x000A1B8E
// PROTOTYPE: PREVIEW 0x0048CC87 identical
// NAME: proto-pdb:default.pdb
BasketStopperPanMain::BasketStopperPanMain(SwampSceneNode* node)
    : BasketStopperMain(node)
{
    _idle = g_swamp->GetAnimationManager().Load("basketStopperPan_idle");
    _hit = g_swamp->GetAnimationManager().Load("basketStopperPan_hit");
    GetStopper()->StartAnimation(_idle, true, false);
}

// FUNCTION: USA 0x000A1ECF
// PROTOTYPE: PREVIEW 0x0048CFC8 identical
// NAME: proto-pdb:default.pdb
void VinceLaundryCar::FrameTick()
{
    if (_state != STATE_RIDING) {
        UnRegisterFrameTick();
        return;
    }
    // !FAKE: c2 register allocation; neutral stand-in local; the camera pointer held in edi
    VinceCam* v0 = (VinceCam*)g_CameraManager->Find("VINCECAM");
    if (_resetcam) {
        if (--_resetcam) {
            v0->UnholdTarget();
            v0->ResetLocation(false);
        }
        v0->ClearDirectionFixed();
        // !FAKE: inliner; pointer local to the call; retail negates through GetFacing's returned eax (objdiff 88.21 -> 90.0)
        const Vector3* p = &_node->GetFacing();
        // !FAKE: c2 scheduling; named temporary; z,y,x loaded before the stores (objdiff 90.0 -> 100.0)
        Vector3 v1(-p->x, -p->y, -p->z);
        v0->SetDirectionFixed(v1, gSettings._basketCaseCameraSpeed);
    }
}

// FUNCTION: USA 0x000A1F77
// PROTOTYPE: PREVIEW 0x0048D070 identical
// NAME: proto-pdb:default.pdb
void VinceLaundryCar::SetupRide()
{
    // !FAKE: c2 register allocation; neutral stand-in local; the camera pointer held in edi
    VinceCam* v0 = (VinceCam*)g_CameraManager->Find("VINCECAM");
    v0->SetHMoveMax(8.0f);
    v0->EnableTrigger(false);
    v0->SetTurnMax(8.0f);
    v0->SetHSpeedFactor(128.0f);
    v0->SetVSpeedFactor(32.0f);
    v0->SetVMoveMax(16.0f);
    v0->SetTurnSpeedFactor(512.0f);
    v0->SetOrbitRadius(gSettings._basketCaseCameraDist);
    _laundryCar = pManager->Find("laundrycar");
    // !FAKE: c1xx front end; neutral stand-in locals; retail fetches position and rotation before either set
    Vector3 v1 = _laundryCar->GetWorldPosition();
    Quaternion v2 = _laundryCar->GetWorldRotation();
    // !FAKE: c2 register allocation; neutral stand-in local; the node held in edi to the end
    Critter* v3 = GetCritter();
    v3->SetWorldPosition(v1);
    v3->SetRotation(v2);
    v3->StartAnimation(_animRide, true, false);
    _laundryCar->GetScript()->Message(this, LaundryCarMain::MESSAGE_START, v3);
    _resetcam += 3;
    _reachedBottom = false;
}

// FUNCTION: USA 0x000A2095
// PROTOTYPE: PREVIEW 0x0048D18E identical
// NAME: proto-pdb:default.pdb
SwampScript* BasketStopperPanMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BasketCase\\BasketCase.cpp", 205) BasketStopperPanMain(node);
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x000A2885
// NAME: proto-pdb:default.pdb
LaundryCarMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("LaundryCarMain", creationFunc);
}

// FUNCTION: USA 0x000A28C1
// NAME: proto-pdb:default.pdb
VinceLaundryCar::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VinceLaundryCar", creationFunc);
}

// FUNCTION: USA 0x000A28FD
// NAME: proto-pdb:default.pdb
BasketStopperPanMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BasketStopperPanMain", creationFunc);
}

// FUNCTION: USA 0x000A22F9
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1LaundryCarMain@@UAE@XZ because this unit defines
// the ctor 0xA20BC (not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LaundryCarMain_dtor(LaundryCarMain* p)
{
    p->LaundryCarMain::~LaundryCarMain();
}

// FUNCTION: USA 0x000A2691
// PROTOTYPE: PREVIEW 0x0048D77E identical
// NAME: proto-pdb:default.pdb
SwampScript* LaundryCarMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\basketcase\\BasketCase.h", 16) LaundryCarMain(node);
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1); T15.c5 va_dis byte-equal but the vptr store: needs ??_7VinceLaundryCar@@6B@ 0x001E8798 pinned
// 5.4 wall (T12.1.c3): candidates fail `reloc: 1 unresolved target(s): 0x1E8798` (vptr store target unpinned after
// T11.1); ??_GVinceLaundryCar 0x000A27B2 is emitted only with this ctor's vtable (C0320) and stays unclaimed
// FUNCTION: USA 0x000A26B8
// PROTOTYPE: PREVIEW 0x0048D7A5 near-identical
// NAME: proto-pdb:default.pdb
VinceLaundryCar::VinceLaundryCar(SwampSceneNode* node)
    : CritterMain(node), _laundryCar(0), _resetcam(2), _reachedBottom(false), _success(false), _wasWhammyReady(false)
{
    AnimationManager& mgr = g_swamp->GetAnimationManager();
    _animRide = mgr.Load("vince_idle01");
    _animCrash = mgr.Load("vince_laundryCarFall");
    _tryagainmenu = false;
    _wasWhammyReady = ((Vince*)_node)->_whammyReady;
    ((Vince*)_node)->SetWhammyReady(false);
    SetState(STATE_IDLE);
}
#endif

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1VinceLaundryCar@@UAE@XZ because this unit defines
// the ctor 0xA26B8 (not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceLaundryCar_dtor(VinceLaundryCar* p)
{
    p->VinceLaundryCar::~VinceLaundryCar();
}

// FUNCTION: USA 0x000A27ED
// PROTOTYPE: PREVIEW 0x0048D8DA identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceLaundryCar::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\basketcase\\BasketCase.h", 83) VinceLaundryCar(node);
}

// FUNCTION: USA 0x000A2814
// PROTOTYPE: PREVIEW 0x0048D901 identical
// NAME: proto-pdb:default.pdb
void LaundryCarMain::ResetLaundry()
{
    std::vector<Critter*> lb;
    pManager->FindAllMatching(lb, "laundry", 0);
    for (std::vector<Critter*>::iterator it = lb.begin(); it != lb.end(); ++it)
        (*it)->SetOption(SwampSceneNode::DontDraw, false, false);
    _laundryBits = 100;
}

// FUNCTION: USA 0x000A2B6B
// PROTOTYPE: PREVIEW 0x0048DC58 identical
// NAME: proto-pdb:default.pdb
void LaundryCarMain::FrameTick()
{
    Critter* car = GetCritter();
    Vector3 carpos;
    const float elapsed = g_swamp->GetInput().GetElapsedTime();
    switch (_state) {
    case STATE_RIDING: {
        // !FAKE: c2 register allocation; neutral stand-in local; the min speed held on the x87 stack
        const float v0 = gSettings._basketCaseMinSpeed;
        const float maxspeed = gSettings._basketCaseMaxSpeed;
        // !FAKE: inliner; hand form of the clamp macro; retail evaluates the inner min twice (x87 ternaries)
        if ((maxspeed - v0)
                    * ((1.0f < -((0.0f < _lineDir.y * 3.3333333f) ? 0.0f : _lineDir.y * 3.3333333f))
                            ? 1.0f
                            : -((0.0f < _lineDir.y * 3.3333333f) ? 0.0f : _lineDir.y * 3.3333333f))
                + v0
            > _speed)
            _speed = (maxspeed < _speed + gSettings._basketCaseAcceleration * elapsed)
                ? maxspeed
                : _speed + gSettings._basketCaseAcceleration * elapsed;
        else
            _speed = (v0 > _speed - gSettings._basketCaseAcceleration * elapsed)
                ? v0
                : _speed - gSettings._basketCaseAcceleration * elapsed;
        // !FAKE: c2 x87 scheduling; neutral stand-in local; retail multiplies elapsed by _speed before the divide
        const float v11 = elapsed * _speed;
        _linet += v11 / _lineLength;
        if (_linet >= 1.0f) {
            if (_segment + 1 >= 16) {
                bool solvedBC = false;
                _linet = 1.0f;
                // !FAKE: c2 register allocation; neutral stand-in local; the level script held in esi
                SwampScript* v0 = LevelLoader::Instance().GetCurLevel()->GetScript();
                v0->Message(this, 0x17, &solvedBC); // retail literal: SewersLevelMain query
                if (_laundryBits == 0 && !solvedBC && !LevelLoader::Instance().IsChapterLoad()) {
                    v0->Message(this, 0x16, 0); // retail literal: SewersLevelMain message
                    pManager->GetVince()->SetTargetLocation("loadInFrontOfEthel", false);
                    LevelLoader::Instance().RequestArea("d:\\levels\\sewers", "wrathofjanice", true,
                        g_swamp->GetInput().GetGameTime(), true);
                } else {
                    pManager->GetVince()->GetScript()->Message(this, VinceLaundryCar::MESSAGE_REACHED_BOTTOM, 0);
                }
                SetState(STATE_MENU);
            } else if (!SetLine(_segment + 1, _line, SETLINE_CORNER)) {
                _linet = 1.0f;
                SoundManager::PlaySound("rbc_post", 0x20, _node, true, 0.0f, 0.0f);
                pManager->GetVince()->GetScript()->Message(this, VinceLaundryCar::MESSAGE_CRASHED, 0);
                SetState(STATE_CRASH);
            } else {
                while (_linet >= 1.0f)
                    _linet -= 1.0f;
                pManager->GetVince()->GetScript()->Message(this, VinceLaundryCar::MESSAGE_SWITCHED_SEGMENTS, 0);
            }
        }
        if (!_switchingLines) {
            // !FAKE: c2 register allocation; neutral stand-in locals; the pad and the line index held in esi
            const SwampGamePad* v1 = g_swamp->GetInput().GetPad(0);
            if (v1->WasButtonPressed(14) || v1->WasButtonPressed(2)) {
                for (int v2 = _line + 1; v2 < 4; v2++) {
                    if (SetLine(_segment, v2, SETLINE_JUMP)) {
                        pManager->GetVince()->GetScript()->Message(this, VinceLaundryCar::MESSAGE_SWITCHED_LINES, 0);
                        break;
                    }
                }
            } else if (v1->WasButtonPressed(15) || v1->WasButtonPressed(3)) {
                for (int v2 = _line - 1; v2 >= 0; v2--) {
                    if (SetLine(_segment, v2, SETLINE_JUMP)) {
                        pManager->GetVince()->GetScript()->Message(this, VinceLaundryCar::MESSAGE_SWITCHED_LINES, 0);
                        break;
                    }
                }
            }
        }
        break;
    }
    case STATE_MENU:
        break;
    default:
        UnRegisterFrameTick();
        return;
    }
    Vector3 oldpos = car->GetWorldPosition();
    // !FAKE: inliner; hand forms of the inline Vector3 operator* and operator+; no before/after recorded (D78, C0223)
    Vector3 travel(_lineDir.x * (_lineLength * _linet), _lineDir.y * (_lineLength * _linet),
        _lineDir.z * (_lineLength * _linet));
    const Vector3* v3 = &_lineStart;
    carpos.x = travel.x + v3->x;
    carpos.y = travel.y + v3->y;
    carpos.z = travel.z + v3->z;
    if (_switchingLines) {
        _jumpPct += elapsed / (_animJump().GetLength() / gSettings._basketCaseJumpAdjust);
        if (_jumpPct > 1.0f) {
            _switchingLines = false;
            _jumpPct = 1.0f;
        }
        const Vector3* v4 = &_prevLineStart;
        Vector3 srcPos(travel.x + v4->x, travel.y + v4->y, travel.z + v4->z);
        carpos *= _jumpPct;
        // !FAKE: c2 x87 register allocation; neutral stand-in local; one blend factor makes retail store srcPos.z
        const float v12 = 1.0f - _jumpPct;
        Vector3 v5(srcPos.x * v12, srcPos.y * v12, srcPos.z * v12);
        carpos.x = v5.x + carpos.x;
        carpos.y = v5.y + carpos.y;
        carpos.z = v5.z + carpos.z;
    }
    car->SetWorldPosition(carpos);
    Vector3 dir(_lineDir.x, 0.0f, _lineDir.z);
    dir.Normalize();
    car->SetFacing(dir);
    Vector3 pos;
    Quaternion rot;
    car->GetBoneOrientation(_bone, rot, pos);
    // !FAKE: inliner; hand forms of the inline Quaternion operator* and by-value Apply; no before/after recorded (C0221, C0225)
    Quaternion v6 = car->GetWorldRotation();
    {
        Quaternion v7 = v6;
        v7 *= rot;
        rot = v7;
    }
    Vector3 v9;
    {
        Vector3 v8;
        v6.Apply(pos, v8);
        v9 = v8;
    }
    pos = Vector3(v9.x + carpos.x, v9.y + carpos.y, v9.z + carpos.z);
    if (_rider) {
        _rider->SetWorldPosition(pos);
        _rider->SetRotation(rot);
    }
    if (_state == STATE_RIDING)
        Ptr<CollisionResultObject> cr = SwampCollide::CheckLine(oldpos, carpos, 0.05f, car, 0x40);
    Vector3 camTargetPos = carpos;
    // !FAKE: c2 scheduling; neutral stand-in local; retail pushes the VINCECAM literal before the y store
    const float v13 = gSettings._basketCaseCameraFocusPos * GetCritter()->GetHeight();
    camTargetPos.y -= v13;
    // !FAKE: c2 register allocation; neutral stand-in local; the camera held in esi past the string dtor (C0223)
    VinceCam* v10 = (VinceCam*)g_CameraManager->Find("VINCECAM");
    v10->HoldTarget(camTargetPos);
}
