// UNIT: src/Apps/Vince/scripts/Carnival/ScareHouse.cpp OBJ: Xbox\OfficialRelease\ScareHouse.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
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
template <class E, class T, class A>
basic_string<E, T, A> operator+(const E* _Left, const basic_string<E, T, A>& _Right);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
class SwampScript;           // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(float x, float y, float z, float w_) : v(x, y, z), w(w_) {}
    Quaternion& operator*=(const Quaternion&);
    void Apply(const Vector3&, Vector3&) const;
    Vector3 Apply(const Vector3& v) const
    {
        Vector3 r;
        Apply(v, r);
        return r;
    }

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class Locator { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetLocation() const { return _pos; }
    Quaternion GetRotation() const { return _rot; }

    Vector3 _pos;                      // 0x00
    Quaternion _rot;                   // 0x0C
    char field_1C[0x48 - 0x1C];        // 0x1C
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
class CXBFont;               // NAME: proto-pdb:default.pdb

class FontManager { // NAME: proto-pdb:default.pdb
public:
    CXBFont* Load(const char*);
};
FontManager& GetFontManager(); // NAME: proto-pdb:default.pdb

enum { // NAME: proto-pdb:default.pdb
    BUTTON_A = 8,
    BUTTON_B = 9
};

struct _XINPUT_GAMEPAD { // NAME: proto-pdb:default.pdb
    unsigned short wButtons;           // 0x00
    unsigned char bAnalogButtons[8];   // 0x02
    short sThumbLX;                    // 0x0A
    short sThumbLY;                    // 0x0C
    short sThumbRX;                    // 0x0E
    short sThumbRY;                    // 0x10
};
DECOMP_SIZE_ASSERT(_XINPUT_GAMEPAD, 0x12);

class SwampGamePad { // NAME: proto-pdb:default.pdb
public:
    unsigned char GetButtonState(int b) const;
    bool WasButtonPressed(int b) const;

    char field_0[0x2C];                // 0x00
    _XINPUT_GAMEPAD _current;          // 0x2C
    _XINPUT_GAMEPAD _previous;         // 0x3E
    char field_50[0x98 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampGamePad, 0x98);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }
    bool IsGamePaused() const { return _pauseGame; }
    const SwampGamePad* GetPad(int player) const;

    float _elapsed;                    // 0x00
    char field_4[0x10 - 0x4];          // 0x04
    double _gameTimeInternal;          // 0x10
    char field_18[0x34 - 0x18];        // 0x18
    bool _pauseGame;                   // 0x34
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    class AnimationManager& GetAnimationManager() const { return *_animationManager; }
    class ModelManager& GetModelManager() const { return *_modelManager; }

    void* _hwnd;                       // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x18 - 0x8];          // 0x08
    class AnimationManager* _animationManager; // 0x18
    char field_1C[0x28 - 0x1C];        // 0x1C
    class ModelManager* _modelManager; // 0x28
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

extern "C" int __cdecl _stricmp(const char*, const char*); // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1,
        DontDraw = 0x2
    };
    void SetOption(unsigned int option, bool value, bool recursive);
    bool GetOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    SwampScript* GetScript() const;
    Vector3 GetFacing() const;
    const Quaternion& SetRotation(const Quaternion&);
    const Quaternion& GetRotation() const { return _Rotation; }
    const Vector3& SetLocation(const Vector3&);
    unsigned int GetZone() const;
    void RemoveTreeFromScene();
    void AddChild(SwampSceneNode*);
    virtual ~SwampSceneNode();                                   // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
public:
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    SwampSceneNode(int);

    char field_4[0x8 - 0x4];           // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x40 - 0x24];        // 0x24
    Quaternion _Rotation;              // 0x40
    char field_50[0x88 - 0x50];        // 0x50
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

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
};

class AnimationControl; // NAME: proto-pdb:default.pdb

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    unsigned int _RefCount;            // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int _tag;                 // 0x04
    float _pct;                        // 0x08
    const void* _blindData;            // 0x0C
    char field_10[0x40 - 0x10];        // 0x10
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    bool HitSomething() const { return _tag != 0 || _blindData != 0; }
    const SwampSceneNode* HitObject() const { return (const SwampSceneNode*)_blindData; }
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    T* operator->() { return _Object; }

    T* _Object;                        // 0x00
};

class SwampCollide { // NAME: proto-pdb:default.pdb
public:
    static Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float, const class GatorNode*, unsigned int);
};

class FireFlyNode; // NAME: proto-pdb:default.pdb

class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    FireFlyNode* AddOneShot(const SwampString&, const Vector3&);
};

extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

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

    T* _Resource;                      // 0x00
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode : SwampSceneNode, all at 0x0; only
// SwampSceneNode modelled
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    // src proto-pdb:default.pdb CollidableNode intro slots
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const;          // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const;                         // vtable+0x2C
    virtual void SetModel(const ResourceHandle<class ModelResource>&);                       // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void SetAnimSpeed(float speed);
    void GetBoneOrientation(int, Quaternion&, Vector3&) const;

    char field_A4[0xB0 - 0xA4];        // 0xA4
    float _radius;                     // 0xB0 (src proto-pdb:default.pdb CollidableCylinder::_radius)
    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    virtual void ReturnToHome();                                   // vtable+0x44
    const Vector3& GetVelocity() const { return _Velocity; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }
    void SetHomePosition(const Vector3& pos) { _homePosition = pos; }
    void SetHomeRotation(const Quaternion& rot) { _homeRotation = rot; }

    char field_200[0x20C - 0x200];     // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x234 - 0x218];     // 0x218
    Vector3 _homePosition;             // 0x234
    Quaternion _homeRotation;          // 0x240
};

class NeedleHat : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetVisible(bool visible);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampSceneNode* creationFunc();
};

class Level { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() { return _script; }

    char field_0[0xC];                 // 0x00
    SwampScript* _script;              // 0x0C
};

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }

    class HUDUI* _hudUI;               // 0x00
    Level* _curLevel;                  // 0x04
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Return(float);
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    void SetCurCam(const SwampString& s, float delay);
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void ShowHairPins(bool show);
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
    Vince* GetVince() const { return _vinceCache; }
    Critter* Create(const SwampString&, const SwampString&, const char*, ResourceHandle<ModelResource>, const Vector3&,
                    const Quaternion&, unsigned int);

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

    void RegisterTimerEvent(unsigned int id, float deltaTime, bool looped);

protected:
    SwampScript(SwampSceneNode* node);
    void RegisterSaveLoadVar(const SwampString&, SwampString*, const char*);
    void RegisterSaveLoadVar(const SwampString&, float*, float);
    void RegisterSaveLoadVar(const SwampString&, int*, int);
public:

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
    Critter* GetCritter() { return (Critter*)_node; }
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~BasicDoorMain();
    virtual int GetDefaultState() const;                           // vtable+0x70

    char field_BC[0x110 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class ScareHouseDoor : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_AUTO_CLOSE = 0
    };
    virtual void TimerEvent(unsigned int id);
};
DECOMP_SIZE_ASSERT(ScareHouseDoor, 0x110);

class CarRideMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    CarRideMain(SwampSceneNode* node);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0x0,
        STATE_PATH_SHORT_A = 0x1,
        STATE_PATH_SHORT_B = 0x2,
        STATE_PATH_LONG_A = 0x3,
        STATE_PATH_LONG_B_FAIL = 0x4,
        STATE_PATH_LONG_B_DONE = 0x5,
        STATE_DEAD = 0x6
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_START = 0x14,
        MESSAGE_SWITCH_A_HIT = 0x15,
        MESSAGE_SWITCH_B_HIT = 0x16,
        MESSAGE_HIT = 0x17,
        MESSAGE_DEAD = 0x18
    };
    virtual void FrameTick();
    virtual void AnimationFinished();
    virtual void InvalidateScript();
private:
    virtual void SetState(int state);
public:

    ResourceHandle<AnimationResource> _animShortA;     // 0xBC
    ResourceHandle<AnimationResource> _animShortB;     // 0xC0
    ResourceHandle<AnimationResource> _animLongA;      // 0xC4
    ResourceHandle<AnimationResource> _animLongBFail;  // 0xC8
    ResourceHandle<AnimationResource> _animLongBDone;  // 0xCC
    Critter* _rider;                   // 0xD0
    int _bone;                         // 0xD4
    bool _switch1;                     // 0xD8
    bool _switch2;                     // 0xD9
    bool _firstRide;                   // 0xDA
    bool _doQuip;                      // 0xDB
    float _speed;                      // 0xDC
    double _lastTime;                  // 0xE0
    Vector3 _lastPos;                  // 0xE8
    class SoundNode* _sound;           // 0xF4
};
DECOMP_SIZE_ASSERT(CarRideMain, 0xF8);

class VinceCarRide : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timer { // NAME: proto-pdb:default.pdb
        TIMER_CS_BEGIN = 0x0,
        TIMER_CS_GENERATOR = 0x1
    };
    VinceCarRide(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int id);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    Critter* _carRide;                 // 0xBC
    class ShootingCam* _shootingCam;   // 0xC0
    int _bone;                         // 0xC4
    bool _readyToFire;                 // 0xC8
    bool _overheated;                  // 0xC9
    bool _firstTime;                   // 0xCA
    bool _died;                        // 0xCB
    bool _wasWhammyReady;              // 0xCC
    double _lastFire;                  // 0xD0
    float _gunHeat;                    // 0xD8
    ResourceHandle<AnimationResource> _animRide; // 0xDC
};
DECOMP_SIZE_ASSERT(VinceCarRide, 0xE0);

class NeedleHatMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_CHECKHAIR = 0x14
    };
    NeedleHatMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void CheckHair();
};
DECOMP_SIZE_ASSERT(NeedleHatMain, 0xBC);

class SewingBulletMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    SewingBulletMain(SwampSceneNode* node);
    virtual void FrameTick();

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    float _speed;                      // 0xBC
};
DECOMP_SIZE_ASSERT(SewingBulletMain, 0xC0);

class TargetSwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_HIT = 0x14,
        MESSAGE_SETPARENT = 0x15
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_UP = 0x0,
        STATE_HIT = 0x1
    };
    TargetSwitchMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void HitBy(CollisionType type, SwampSceneNode* other, const Ptr<CollisionResultObject>& result);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);
    Critter* GetSwitch() { return (Critter*)_node; }

    ResourceHandle<AnimationResource> _animHit; // 0xBC
    Critter* _parent;                  // 0xC0
};
DECOMP_SIZE_ASSERT(TargetSwitchMain, 0xC4);

class ScareHouseMonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timer { // NAME: proto-pdb:default.pdb
        TIMER_DEAD = 0x0,
        TIMER_BLINK = 0x1
    };
    virtual void TimerEvent(unsigned int id);
    void SetCreator(class MonsterSpawnLocatorMain* creator) { _creator = creator; }
    static void ResetKillQuip();

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    static int _killsTilQuip;
public:

    int _bone;                         // 0xBC
    class MonsterSpawnLocatorMain* _creator; // 0xC0
};
DECOMP_SIZE_ASSERT(ScareHouseMonsterMain, 0xC4);

// src proto-pdb:default.pdb LevelScript second record (size 0x64, ti 0x1408d): retail reads _inRegion at 0x4D
class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode* node);

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_50[0x64 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class ScareHouseRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    ScareHouseRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ScareHouseRegionMain, 0x64);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double Random(unsigned int, unsigned int); // src proto-pdb:default.pdb Random
}

// FUNCTION: USA 0x0003FA0F
// NAME: proto-pdb:default.pdb
void ScareHouseMonsterMain::ResetKillQuip()
{
    _killsTilQuip = BeepLib::Random(10, 25);
}

// FUNCTION: USA 0x0003FE18
// PROTOTYPE: PREVIEW 0x0042E2C3 ambiguous
// NAME: proto-pdb:default.pdb
// ??_GLocatorMain: compiler-generated with the vtable LocatorMain's ctor emits; calls ??1 out of line (5.3 T5)
class LocatorMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    LocatorMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    Locator* GetLocator() const { return _locator; }

    Locator* _locator;                 // 0x4C
    SwampString _loadName;             // 0x50
    SwampString _instanceName;         // 0x6C
};
DECOMP_SIZE_ASSERT(LocatorMain, 0x88);

// FUNCTION: USA 0x000415E6
// PROTOTYPE: PREVIEW 0x0042FA4D identical
// NAME: proto-pdb:default.pdb
// ??_GMonsterSpawnLocatorMain: compiler-generated with the vtable the ctor emits; inlines the implicit dtor
class MonsterSpawnLocatorMain : public LocatorMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SPAWN = 0x18
    };
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_SPAWN = 0x0
    };
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void TimerEvent(unsigned int id);
    MonsterSpawnLocatorMain(SwampSceneNode* node);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    bool _spawning;                    // 0x88
    int _count;                        // 0x8C
    SwampString _monsterName;          // 0x90
    float _monsterFreq;                // 0xAC
    float _monsterSpeed;               // 0xB0
    int _monsterCount;                 // 0xB4
    int _monsterAnim;                  // 0xB8
    Critter* _carRide;                 // 0xBC
};
DECOMP_SIZE_ASSERT(MonsterSpawnLocatorMain, 0xC0);

class ScareHouseTryAgainMenu : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ScareHouseTryAgainMenu(SwampScript* script);
    virtual void Update();
    virtual void Render(const Matrix&) const;

    bool _ready;                       // 0xA4
    CXBFont* _font;                    // 0xA8
    SwampScript* _returnScript;        // 0xAC
};
DECOMP_SIZE_ASSERT(ScareHouseTryAgainMenu, 0xB0);

// FUNCTION: USA 0x0003F98A
// PROTOTYPE: PREVIEW 0x0042DE3D identical
// NAME: proto-pdb:default.pdb
void CarRideMain::InvalidateScript()
{
    _rider = 0;
    if (_sound)
        _sound->Invalidate();
    CritterMain::InvalidateScript();
}

// FUNCTION: USA 0x0003F9AB
// PROTOTYPE: PREVIEW 0x0047DAF5 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one ?AnimationFinished@TargetSwitchMain bytes match (masked 0), objdiff None; fold-name wall (C0377):
// row 0x0003F9AB is named ?AnimationFinished@RagDollMain (proto FrontPorch.obj), refused `not defined in ScareHouse.obj`;
// lift: the row renamed to the unit's folded ?AnimationFinished@TargetSwitchMain@@UAEXXZ (pin, symbols_pin.txt:4199) + re-cut
void TargetSwitchMain::AnimationFinished()
{
    SetState(STATE_UP);
}
#endif

// FUNCTION: USA 0x0003F9B3
// PROTOTYPE: PREVIEW 0x0042DE5E identical
// NAME: proto-pdb:default.pdb
void ScareHouseDoor::TimerEvent(unsigned int id)
{
    if (id == TIMER_AUTO_CLOSE)
        Message(this, 0x15, 0); // src proto-pdb:default.pdb BasicDoorMain::Message::MESSAGE_CLOSE
}

// FUNCTION: USA 0x0003FA25
// PROTOTYPE: PREVIEW 0x0042DED0 identical
// NAME: proto-pdb:default.pdb
void CarRideMain::FrameTick()
{
    if (_state == STATE_IDLE || _state == STATE_DEAD)
        return;
    double t = g_swamp->GetInput()._gameTimeInternal;
    if (_lastTime == t)
        return;
    Critter* carride = GetCritter();
    Vector3 pos;
    Quaternion rot;
    carride->GetBoneOrientation(_bone, rot, pos);
    Quaternion v0 = carride->GetWorldRotation();
    {
        // !FAKE: inliner; hand form of the inlined Quaternion free operator*; no PDB record (D78), no before/after recorded
        Quaternion tmp = rot;
        tmp *= v0;
        rot = tmp;
    }
    {
        // !FAKE: inliner; hand form of the inlined Quaternion::Apply(const Vector3&) and Vector3 free operator+; no before/after recorded (D78)
        Vector3 tmp;
        v0.Apply(pos, tmp);
        Vector3 v1 = tmp;
        const Vector3* wp = &carride->GetWorldPosition(); // !FAKE: inliner; pointer local to the call; D117 allowed form, no before/after recorded
        pos = Vector3(v1.x + wp->x, v1.y + wp->y, v1.z + wp->z);
    }
    if (_rider) {
        _rider->SetWorldPosition(pos);
        _rider->SetRotation(rot);
    }
    if (_lastTime != 0.0) {
        // !FAKE: inliner; hand form of the inlined Vector3 free operator- and ApproxLength; no before/after recorded (D78)
        const Vector3* last = &_lastPos; // !FAKE: inliner; pointer local to the inlined operator-'s reference; D117 allowed form, no before/after recorded
        float dx = pos.x - last->x;
        float dy = pos.y - last->y;
        float dz = pos.z - last->z;
        float v2 = dx * dx + dy * dy + dz * dz;
        _speed = FastReciprocalSquareRoot(v2) * v2 / (t - _lastTime);
    }
    _lastTime = t;
    _lastPos = pos;
}

// FUNCTION: USA 0x0003FB95
// PROTOTYPE: PREVIEW 0x0042E040 identical
// NAME: proto-pdb:default.pdb
void CarRideMain::SetState(int state)
{
    Critter* ride = (Critter*)_node;
    switch (state) {
    case STATE_IDLE:
        ride->StartAnimation(ResourceHandle<AnimationResource>(), false, false);
        ride->ReturnToHome();
        break;
    case STATE_PATH_SHORT_A:
        ride->StartAnimation(_animShortA, false, false);
        ride->SetWorldPosition(Vector3(0.0f, 0.0f, 0.0f));
        break;
    case STATE_PATH_SHORT_B:
        ride->StartAnimation(_animShortB, false, false);
        ride->SetWorldPosition(Vector3(0.0f, 0.0f, 0.0f));
        break;
    case STATE_PATH_LONG_A:
        ride->StartAnimation(_animLongA, false, false);
        ride->SetWorldPosition(Vector3(0.0f, 0.0f, 0.0f));
        break;
    case STATE_PATH_LONG_B_FAIL:
        ride->StartAnimation(_animLongBFail, false, false);
        ride->SetWorldPosition(Vector3(0.0f, 0.0f, 0.0f));
        break;
    case STATE_PATH_LONG_B_DONE:
        ride->StartAnimation(_animLongBDone, false, false);
        ride->SetWorldPosition(Vector3(0.0f, 0.0f, 0.0f));
        break;
    case STATE_DEAD:
        ride->SetAnimSpeed(0.0f);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x0003FCD0
// PROTOTYPE: PREVIEW 0x0042E17B identical
// NAME: proto-pdb:default.pdb
void NeedleHatMain::CheckHair()
{
    pManager->_vinceCache->ShowHairPins(_node->GetOption(SwampSceneNode::DontDraw));
}

// FUNCTION: USA 0x0003FCEF
// PROTOTYPE: PREVIEW 0x0042E19A identical
// NAME: proto-pdb:default.pdb
void ScareHouseMonsterMain::TimerEvent(unsigned int timerId)
{
    SwampSceneNode* node = _node;
    switch (timerId) {
    case TIMER_DEAD:
        node->SetOption(SwampSceneNode::DontDraw, true, false);
        InvalidateScript();
        break;
    case TIMER_BLINK:
        node->SetOption(SwampSceneNode::DontDraw, !node->GetOption(SwampSceneNode::DontDraw), false);
        break;
    }
}

// FUNCTION: USA 0x0003FD31
// PROTOTYPE: PREVIEW 0x0042E1DC identical
// NAME: proto-pdb:default.pdb
void TargetSwitchMain::HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr)
{
    if (critterHitBy)
        Message(this, MESSAGE_HIT, 0);
}

// FUNCTION: USA 0x0003FDAB
// PROTOTYPE: PREVIEW 0x0042E256 identical
// NAME: proto-pdb:default.pdb
void NeedleHatMain::Message(SwampScript* source, int message, void* data)
{
    if (message != 6 && message != MESSAGE_CHECKHAIR) {
        CritterMain::Message(source, message, data);
        return;
    }
    CheckHair();
}

// FUNCTION: USA 0x0003FDC9
// PROTOTYPE: PREVIEW 0x0042E274 identical
// NAME: proto-pdb:default.pdb
LocatorMain::LocatorMain(SwampSceneNode* node) : SwampScript(node), _locator(0)
{
}

// FUNCTION: USA 0x0003FDF5
// PROTOTYPE: PREVIEW 0x0042E2A0 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1LocatorMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LocatorMain_dtor(LocatorMain* p)
{
    p->LocatorMain::~LocatorMain();
}

// FUNCTION: USA 0x0003FE34
// PROTOTYPE: PREVIEW 0x0045D94C identical
// NAME: proto-pdb:default.pdb
void NeedleHat::SetVisible(bool b)
{
    SetOption(DontDraw, !b, false);
    if (GetScript())
        GetScript()->Message(GetScript(), NeedleHatMain::MESSAGE_CHECKHAIR, 0);
}

// FUNCTION: USA 0x0003FFD9
// PROTOTYPE: PREVIEW 0x0042E440 identical
// NAME: proto-pdb:default.pdb
void TargetSwitchMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 6:
        SetState(STATE_UP);
        break;
    case MESSAGE_HIT:
        if (_state != STATE_HIT) {
            SetState(STATE_HIT);
            if (_stricmp(_node->_Name.c_str(), "scareSwitchA") == 0)
                _parent->GetScript()->Message(this, 0x15, 0);
            else
                _parent->GetScript()->Message(this, 0x16, 0);
        }
        break;
    case MESSAGE_SETPARENT:
        _parent = (Critter*)data;
        break;
    default:
        CritterMain::Message(source, message, data);
    }
}

// FUNCTION: USA 0x00040092
// PROTOTYPE: PREVIEW 0x0042E4F9 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 95.40, bytes mismatch (slot packing: retail puts hat in node's param slot, mgr at -4 and the
// this spill at -0x18; ours mgr in node's slot, hat and this share -4; frame 0x84 vs 0x88; at-stop 20, T12.c12: hat/mgr/sw
// block-scoped, hat at use, hat = 0, carride at top, this stand-ins, if-wrap, pManager->GetVince: no change)
void ScareHouseRegionMain::EnteringRegion(SwampSceneNode* node)
{
    NeedleHat* hat;
    if (_inRegion)
        return;
    LevelScript::EnteringRegion(node);
    CritterManager& mgr = *pManager;
    Vince* vince = mgr.GetVince(); // !FAKE: c2 register allocation; stand-in local for the register the PDB drops (ebx); no before/after recorded
    vince->SetHomePosition(vince->GetWorldPosition());
    vince->SetHomeRotation(vince->GetRotation());
    Critter* carride = mgr.Find("carride");
    // !FAKE: c2 register allocation; stand-in for the register the PDB drops (esi); retail ends the name temp before GetScript
    Critter* sw = mgr.Find("scareSwitchA");
    sw->GetScript()->Message(this, 0x15, carride); // src proto-pdb:default.pdb TargetSwitchMain::Messages::MESSAGE_SETPARENT
    sw = mgr.Find("scareSwitchB");
    sw->GetScript()->Message(this, 0x15, carride); // src proto-pdb:default.pdb TargetSwitchMain::Messages::MESSAGE_SETPARENT
    hat = (NeedleHat*)mgr.Find("needlehat");
    if (!hat) {
        hat = (NeedleHat*)mgr.Create("needlehat", "needlehat", "needlehat",
                                     g_swamp->GetModelManager().Load("needlehat_base"), Vector3(0.0f, 0.0f, 0.0f),
                                     Quaternion(0.0f, 0.0f, 0.0f, 1.0f), carride->GetZone());
        hat->RemoveTreeFromScene();
        hat->SetLocation(Vector3(0.0f, 0.0f, 0.0f));
        hat->SetVisible(false);
        vince->AddChild(hat);
    }
    carride->GetScript()->Activate();
}
#endif

// FUNCTION: USA 0x000402AB
// PROTOTYPE: PREVIEW 0x0042E712 identical
// NAME: proto-pdb:default.pdb
ScareHouseTryAgainMenu::ScareHouseTryAgainMenu(SwampScript* script)
    : SwampSceneNode(9), _returnScript(script)
{
    _font = GetFontManager().Load("pauseCounters");
    // !FAKE: inliner; pointer local + hand form of the inlined SwampGamePad::GetButtonState (analog branch, b >= 8), as in Update; no before/after recorded
    const SwampGamePad* p = g_swamp->GetInput().GetPad(0);
    _ready = p->_current.bAnalogButtons[BUTTON_A - 8] < 0x20 && p->_current.bAnalogButtons[BUTTON_B - 8] < 0x20;
    SoundManager::PlaySound("frontdialog", 0, 0, false, 0.0f, 0.0f);
}

// FUNCTION: USA 0x00040347
// PROTOTYPE: PREVIEW 0x0042E7AE identical
// NAME: proto-pdb:default.pdb
void ScareHouseTryAgainMenu::Update()
{
    if (g_swamp->GetInput().IsGamePaused())
        return;
    const SwampGamePad* pad = g_swamp->GetInput().GetPad(0);
    if (!_ready) {
        // !FAKE: inliner; hand form of the inlined SwampGamePad::GetButtonState (analog branch, b >= 8); an in-class
        // body leaves a COMDAT that collides with the cut object's out-of-line copy at relink (C0256)
        _ready = pad->_current.bAnalogButtons[BUTTON_A - 8] < 0x20 && pad->_current.bAnalogButtons[BUTTON_B - 8] < 0x20;
    } else if (pad->WasButtonPressed(BUTTON_A)) {
        SoundManager::PlaySound("frontbutton", 0, 0, false, 0.0f, 0.0f);
        _returnScript->Message(0, 0x1D, 0); // src proto-pdb:default.pdb VinceCarRide::Messages::MESSAGE_MENU_TRYAGAIN
        Invalidate();
    } else if (pad->WasButtonPressed(BUTTON_B)) {
        SoundManager::PlaySound("frontbutton", 0, 0, false, 0.0f, 0.0f);
        _returnScript->Message(0, 0x1E, 0); // src proto-pdb:default.pdb VinceCarRide::Messages::MESSAGE_MENU_LEAVE
        Invalidate();
    }
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1VinceCarRide@@UAE@XZ (0x00042737) in this
// unit (no matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
#ifdef NON_MATCHING
// best: match_one ??1VinceCarRide objdiff 1.30, bytes match (masked 0); cut-extent wall: delink symbol runs to FUN_00042bfd
// (5.4 T12.1.c4: unguarded once, 1.30 again; lift: a pin at the neighbour + re-cut, C0377).
// ??_GVinceCarRide 0x0004271B: no draft; companion wall (C0320): ctor 0x0004261B fails, 3 unresolved reloc targets
void pin_emit_VinceCarRide_dtor(VinceCarRide* p)
{
    p->VinceCarRide::~VinceCarRide();
}
#endif

// FUNCTION: USA 0x00040E9E
// PROTOTYPE: PREVIEW 0x0042F305 identical
// NAME: proto-pdb:default.pdb
void VinceCarRide::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_CS_BEGIN:
        AnimCam::Return(0.0f);
        g_CameraManager->SetCurCam("RideCarCamera", 0.0f);
        _readyToFire = true;
        break;
    case TIMER_CS_GENERATOR:
        AnimCam::Return(0.0f);
        _readyToFire = true;
        break;
    }
}

// FUNCTION: USA 0x0004137B
// PROTOTYPE: PREVIEW 0x0042F7E2 identical
// NAME: proto-pdb:default.pdb
void SewingBulletMain::FrameTick()
{
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    Critter* bullet = GetCritter(); // !FAKE: c2 register allocation; stand-in local for the register the PDB drops (ebx); no before/after recorded
    {
        // !FAKE: inliner; hand form of the inlined Vector3 free operator*; no PDB record (D78), no before/after recorded
        float v0 = _speed;
        const Vector3* facing = &bullet->GetFacing(); // !FAKE: inliner; pointer local to the call; D117 allowed form, no before/after recorded
        bullet->SetVelocity(Vector3(v0 * facing->x, v0 * facing->y, v0 * facing->z));
    }
    Vector3 start = bullet->GetWorldPosition();
    // !FAKE: inliner; hand form of the inlined Vector3 free operator* and operator+; no PDB record (D78), no before/after recorded
    const Vector3& v = bullet->GetVelocity();
    Vector3 v1(elapsed * v.x, elapsed * v.y, elapsed * v.z);
    Vector3 end(v1.x + start.x, v1.y + start.y, v1.z + start.z);
    Ptr<CollisionResultObject> cr = SwampCollide::CheckLine(start, end, bullet->_radius, 0, 0);
    if (!cr->HitSomething() || cr->HitObject() == bullet) {
        bullet->SetWorldPosition(end);
        return;
    }
    SwampSceneNode* hit = (SwampSceneNode*)cr->HitObject();
    if (hit && hit->GetScript())
        hit->GetScript()->HitBy(HorizontalCollision, bullet, cr);
    else {
        // !FAKE: c2 evaluation order; stand-in local for the object pointer, retail loads g_particleManager into edi
        // before the GetWorldPosition call (probe: va_dis first diff +0xC5 -> none, T12.c28)
        SwampParticleManager* p;
        (p = g_particleManager)->AddOneShot("bulletMiss", bullet->GetWorldPosition());
    }
    InvalidateScript();
}

// FUNCTION: USA 0x000414C6
// PROTOTYPE: PREVIEW 0x0042F92D identical
// NAME: proto-pdb:default.pdb
MonsterSpawnLocatorMain::MonsterSpawnLocatorMain(SwampSceneNode* node)
    : LocatorMain(node), _spawning(false), _count(0), _carRide(0)
{
    RegisterSaveLoadVar("monstername", &_monsterName, "");
    RegisterSaveLoadVar("monsterfrequency", &_monsterFreq, 1.0f);
    RegisterSaveLoadVar("monsterspeed", &_monsterSpeed, 1.0f);
    RegisterSaveLoadVar("monstercount", &_monsterCount, 1);
    RegisterSaveLoadVar("monsteranim", &_monsterAnim, 0);
}

// FUNCTION: USA 0x00041613
// PROTOTYPE: PREVIEW 0x0042FA7A identical
// NAME: proto-pdb:default.pdb
void MonsterSpawnLocatorMain::TimerEvent(unsigned int id)
{
    if (id != TIMER_SPAWN)
        return;
    ResourceHandle<ModelResource> mh = g_swamp->GetModelManager().Load(format("monster%s_base", _monsterName.c_str()));
    const Locator* l = GetLocator();
    Critter* m = pManager->Create(_monsterName, "scarehousemonster", "scarehouse", mh, l->GetLocation(),
                                  l->GetRotation(), 0);
    ((ScareHouseMonsterMain*)m->GetScript())->SetCreator(this);
    ResourceHandle<AnimationResource> ah =
        g_swamp->GetAnimationManager().Load(format("monster%s_attack%d", _monsterName.c_str(), _monsterAnim));
    m->StartAnimation(ah, false, false);
    m->SetAnimSpeed(_monsterSpeed);
    if (++_count < _monsterCount)
        RegisterTimerEvent(TIMER_SPAWN, 1.0f / _monsterFreq, false);
    else
        _spawning = false;
}

// FUNCTION: USA 0x0004183D
// PROTOTYPE: PREVIEW 0x0042FCA4 identical
// NAME: proto-pdb:default.pdb
void TargetSwitchMain::SetState(int state)
{
    switch (state) {
    case STATE_UP:
        GetSwitch()->StartAnimation(ResourceHandle<AnimationResource>(), false, false);
        break;
    case STATE_HIT:
        SoundManager::PlaySound("css_targethit", 0x20, _node, true, 0.0f, 0.0f);
        GetSwitch()->StartAnimation(_animHit, false, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x00041925
// PROTOTYPE: PREVIEW 0x0042FD2D identical
// NAME: proto-pdb:default.pdb
SwampScript* CarRideMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 32) CarRideMain(node);
}

// FUNCTION: USA 0x0004194C
// PROTOTYPE: PREVIEW 0x0042FD54 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 0.0, bytes match (delink wall: retail cut obj carries a local label
// ?creationFunc@NeedleHatMain...+4 inside the push imm32, splitting the symbol; source is final; 1 draft)
SwampScript* NeedleHatMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 169) NeedleHatMain(node);
}
#endif

// FUNCTION: USA 0x00041976
// PROTOTYPE: PREVIEW 0x0042FD7E identical
// NAME: proto-pdb:default.pdb
SwampScript* SewingBulletMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 190) SewingBulletMain(node);
}

// row-end pin 5.4 T26.c1
// FUNCTION: USA 0x000419A0
// PROTOTYPE: PREVIEW 0x0042FDA8 identical
// NAME: proto-pdb:default.pdb
SwampScript* MonsterSpawnLocatorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 209) MonsterSpawnLocatorMain(node);
}

// FUNCTION: USA 0x00041B79
// PROTOTYPE: PREVIEW 0x0042FF81 identical
// NAME: proto-pdb:default.pdb
void MonsterSpawnLocatorMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 6:
        _carRide = pManager->Find("carride");
        break;
    case MESSAGE_SPAWN:
        if (!_spawning) {
            _spawning = true;
            _count = 0;
            SwampString soundname = "css_" + _monsterName;
            SoundManager::PlaySound(soundname, 0, 0, false, 0.0f, 0.0f);
            RegisterTimerEvent(TIMER_SPAWN, 1.0f / _monsterFreq, false);
        }
        break;
    default:
        LocatorMain::Message(source, message, data);
    }
}

// FUNCTION: USA 0x00041C3C
// NAME: proto-pdb:default.pdb
TargetSwitchMain::TargetSwitchMain(SwampSceneNode* node) : CritterMain(node), _parent(0)
{
    _animHit = g_swamp->GetAnimationManager().Load("targetswitch_hit");
    SetState(STATE_UP);
}

// FUNCTION: USA 0x00041CB5
// PROTOTYPE: PREVIEW 0x004300BD identical
// NAME: proto-pdb:default.pdb
SwampScript* TargetSwitchMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 274) TargetSwitchMain(node);
}

// FUNCTION: USA 0x00042202
// NAME: proto-pdb:default.pdb
SwampScript* ScareHouseRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 18) ScareHouseRegionMain(node);
}

// FUNCTION: USA 0x00042226
// NAME: proto-pdb:default.pdb
ScareHouseRegionMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("ScareHouseRegionMain", creationFunc);
}

// FUNCTION: USA 0x00042262
// NAME: proto-pdb:default.pdb
CarRideMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("CarRideMain", creationFunc);
}

// FUNCTION: USA 0x000422DA
// NAME: proto-pdb:default.pdb
NeedleHatMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("NeedleHatMain", creationFunc);
}

// FUNCTION: USA 0x00042316
// NAME: proto-pdb:default.pdb
SewingBulletMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("SewingBulletMain", creationFunc);
}

// FUNCTION: USA 0x00042352
// NAME: proto-pdb:default.pdb
MonsterSpawnLocatorMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("MonsterSpawnLocatorMain", creationFunc);
}

// FUNCTION: USA 0x0004238E
// NAME: proto-pdb:default.pdb
ScareHouseMonsterMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("ScareHouseMonsterMain", creationFunc);
}

// FUNCTION: USA 0x000423CA
// NAME: proto-pdb:default.pdb
TargetSwitchMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("TargetSwitchMain", creationFunc);
}

// FUNCTION: USA 0x00042532
// PROTOTYPE: PREVIEW 0x0043093A identical
// NAME: proto-pdb:default.pdb
void CarRideMain::AnimationFinished()
{
    switch (_state) {
    case STATE_PATH_SHORT_A:
        SetState(_switch1 ? STATE_PATH_LONG_A : STATE_PATH_SHORT_B);
        break;
    case STATE_PATH_SHORT_B:
    case STATE_PATH_LONG_B_FAIL:
    case STATE_PATH_LONG_B_DONE: {
        SetState(STATE_IDLE);
        bool solvedSH = false;
        LevelLoader::Instance().GetCurLevel()->GetScript()->Message(this, MESSAGE_HIT, &solvedSH);
        Vince* vince = pManager->_vinceCache;
        vince->GetScript()->Message(this, (_switch2 || solvedSH) ? 0x1C : 0x1B, 0); // src proto-pdb:default.pdb VinceCarRide::Messages::MESSAGE_RIDEOVER_DONE / VinceCarRide::Messages::MESSAGE_RIDEOVER_FAIL
        break;
    }
    case STATE_PATH_LONG_A:
        if (_switch2) {
            SetState(STATE_PATH_LONG_B_DONE);
            bool solvedSH = false;
            LevelLoader::Instance().GetCurLevel()->GetScript()->Message(this, MESSAGE_HIT, &solvedSH);
            if (!solvedSH)
                pManager->_vinceCache->GetScript()->Message(this, 0x1F, 0); // src proto-pdb:default.pdb VinceCarRide::Messages::MESSAGE_GENERATOR
        } else {
            SetState(STATE_PATH_LONG_B_FAIL);
        }
        break;
    }
}

// FUNCTION: USA 0x00042FA8
// PROTOTYPE: PREVIEW 0x004313E7 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceCarRide::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 98) VinceCarRide(node);
}

// FUNCTION: USA 0x00042FCF
// NAME: proto-pdb:default.pdb
VinceCarRide::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VinceCarRide", creationFunc);
}

class SwampNodeManager { // NAME: proto-pdb:default.pdb
public:
    static SwampNodeManager& Instance();
    void Add(const SwampString& name, SwampSceneNode* (*func)());
};

// FUNCTION: USA 0x0004229E
// NAME: proto-pdb:default.pdb
NeedleHat::RegisterMe::RegisterMe()
{
    SwampNodeManager::Instance().Add("NeedleHat", creationFunc);
}

class DoorAMain : public ScareHouseDoor { // NAME: proto-pdb:default.pdb
public:
    DoorAMain(SwampSceneNode* node);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DoorAMain, 0x110);

class DoorBMain : public ScareHouseDoor { // NAME: proto-pdb:default.pdb
public:
    DoorBMain(SwampSceneNode* node);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DoorBMain, 0x110);

class DoorDMain : public ScareHouseDoor { // NAME: proto-pdb:default.pdb
public:
    DoorDMain(SwampSceneNode* node);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DoorDMain, 0x110);

class DoorEMain : public ScareHouseDoor { // NAME: proto-pdb:default.pdb
public:
    DoorEMain(SwampSceneNode* node);
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DoorEMain, 0x110);


class DoorCMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};


// FUNCTION: USA 0x00042406
// NAME: proto-pdb:default.pdb
DoorAMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("DoorAMain", creationFunc);
}

// FUNCTION: USA 0x00042442
// NAME: proto-pdb:default.pdb
DoorBMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("DoorBMain", creationFunc);
}

// FUNCTION: USA 0x0004247E
// NAME: proto-pdb:default.pdb
DoorCMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("DoorCMain", creationFunc);
}

// FUNCTION: USA 0x000424BA
// NAME: proto-pdb:default.pdb
DoorDMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("DoorDMain", creationFunc);
}

// FUNCTION: USA 0x000424F6
// NAME: proto-pdb:default.pdb
DoorEMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("DoorEMain", creationFunc);
}

// FUNCTION: USA 0x00042116
// NAME: proto-pdb:default.pdb
SwampScript* DoorAMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 314) DoorAMain(node);
}

// FUNCTION: USA 0x00042140
// PROTOTYPE: PREVIEW 0x00479907 identical
// NAME: proto-pdb:default.pdb
SwampScript* DoorBMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 322) DoorBMain(node);
}

// FUNCTION: USA 0x00042194
// NAME: proto-pdb:default.pdb
SwampScript* DoorDMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 338) DoorDMain(node);
}

// FUNCTION: USA 0x000421BE
// NAME: proto-pdb:default.pdb
SwampScript* DoorEMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\scarehouse\\ScareHouse.h", 346) DoorEMain(node);
}
