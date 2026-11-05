// UNIT: src/Apps/Vince/scripts/Bayou/Hannah.cpp OBJ: Xbox\OfficialRelease\Hannah.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <string.h>
#pragma intrinsic(memcpy) // src proto-pdb:default.pdb memcpy
#pragma intrinsic(memcmp) // !FAKE: compiler directive; retail basic_string::rfind inlines char_traits<char>::compare as repe cmpsb (match_one 57.93 -> 100.0)

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <> struct char_traits<char> { // NAME: proto-pdb:default.pdb
    static bool eq(const char& _Left, const char& _Right) { return _Left == _Right; }
    static int compare(const char* _First1, const char* _First2, unsigned int _Count)
    {
        return ::memcmp(_First1, _First2, _Count);
    }
    static char* copy(char* _First1, const char* _First2, unsigned int _Count);
};
template <class T> class allocator;
class _String_base { // NAME: proto-pdb:default.pdb
public:
    void _Xlen() const;
};
template <class E, class T, class A> class basic_string : public _String_base {
public:
    basic_string(const E* ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right); }
    basic_string& assign(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)(-1)); } // npos
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& append(const E* _Ptr, unsigned int _Count);
    unsigned int rfind(const E* _Ptr, unsigned int _Off, unsigned int _Count) const;
    const E* c_str() const { return _Myptr(); }
    E* _Myptr() { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

protected:
    void _Eos(unsigned int _Newsize);
    bool _Grow(unsigned int _Newsize, bool _Trim = false);
    bool _Inside(const E* _Ptr);
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
basic_string<E, T, A>& basic_string<E, T, A>::append(const E* _Ptr, unsigned int _Count)
{
    if (_Inside(_Ptr))
        return append(*this, _Ptr - _Myptr(), _Count);
    if ((unsigned int)(-1) - _Mysize <= _Count) // npos; static left undeclared (C0215)
        _String_base::_Xlen();

    unsigned int _Num;
    if (0 < _Count && _Grow(_Num = _Mysize + _Count)) {
        T::copy(_Myptr() + _Mysize, _Ptr, _Count);
        _Eos(_Num);
    }
    return *this;
}
template <class E, class T, class A>
unsigned int basic_string<E, T, A>::rfind(const E* _Ptr, unsigned int _Off, unsigned int _Count) const
{
    if (_Count == 0)
        return _Off < _Mysize ? _Off : _Mysize;
    if (_Count <= _Mysize) {
        const E* _Uptr = _Myptr() + (_Off < _Mysize - _Count ? _Off : _Mysize - _Count);
        for (;; --_Uptr)
            if (T::eq(*_Uptr, *_Ptr) && T::compare(_Uptr, _Ptr, _Count) == 0)
                return _Uptr - _Myptr();
            else if (_Uptr == _Myptr())
                break;
    }

    return (unsigned int)(-1); // npos
}
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const E* _Right);
template <class T, class A = allocator<T> > class deque { // src proto-pdb:default.pdb deque
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

// FUNCTION: USA 0x000292B0
// PROTOTYPE: PREVIEW 0x00488991 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::basic_string<char,std::char_traits<char>,std::allocator<char> >::rfind(char const *,unsigned int,unsigned int)const;

// FUNCTION: USA 0x000294FC
// PROTOTYPE: PREVIEW 0x004182B9 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> > & std::basic_string<char,std::char_traits<char>,std::allocator<char> >::append(char const *,unsigned int);

class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line);

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3& operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(const Vector3& v) { Set(v); }
    Quaternion(const Vector3& axis, float angle) { Set(axis, angle); }
    void Set(const Vector3&);
    void Set(const Vector3&, float);
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    SwampScript* GetScript() const;
    unsigned int GetZone() const;
    Vector3 GetFacing() const;
    void SetOption(unsigned int, bool, bool);
    bool PopScript();
    void SetScript(const SwampString&);
    const Vector3& SetLocation(const Vector3&);
    const Vector3& GetLocation() const { return _Location; }

    char field_4[0x34 - 0x4];          // 0x04
    Vector3 _Location;                 // 0x34
    char field_40[0xA4 - 0x40];        // 0x40
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C

    char field_A4[0xAC - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0xB4 - 0xAC];        // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    void GetBoneOrientation(const SwampString&, Quaternion&, Vector3&) const;

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

    bool IsOnFloor() const { return _OnFloor; }
    const Vector3& GetVelocity() const { return _Velocity; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }
    void SetSpecies(SwampString);
    Vector3 GetVelocityToTarget(const Vector3&, const Vector3&, float, float);

    char field_200[0x20C - 0x200];     // 0x200
    Vector3 _Velocity;                 // 0x20C
    SwampString _species;              // 0x218
    char field_234[0x250 - 0x234];     // 0x234
    bool _OnFloor;                     // 0x250
    char field_251[0x2C8 - 0x251];     // 0x251
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Create(const SwampString&, const SwampString&, const char*, ResourceHandle<ModelResource>, const Vector3&,
                    const Quaternion&, unsigned int);
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

    void UnRegisterFrameTick();
    void RegisterTimerEvent(unsigned int id, float deltaTime, bool looped);
    SwampSceneNode* GetNode() { return _node; }

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class HannahStartBattleRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void LeavingRegion(SwampSceneNode* node);
    virtual void FrameTick();
};
DECOMP_SIZE_ASSERT(HannahStartBattleRegionMain, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x10C - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    char field_10C[0x11C - 0x10C];     // 0x10C
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class VinceBlowBack : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    VinceBlowBack(SwampSceneNode* node);
    virtual void FrameTick();
    virtual void InvalidateScript();

    Vector3 _controlPoint[4];          // 0xBC
    float _time;                       // 0xEC
    float _totalTime;                  // 0xF0
    GameCam* _vincecam;                // 0xF4
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(VinceBlowBack, 0xF8);

class QuickRand { // NAME: proto-pdb:default.pdb
public:
    QuickRand(unsigned __int64 seed);
    unsigned int GetRandom()
    {
        unsigned __int64 tmp = (unsigned __int64)_Mul * 0x7F13AC69 + _Carry;
        _Mul = (unsigned int)tmp;
        _Carry = (unsigned int)(tmp >> 32);
        return _Mul;
    }
    float GetRandomFloat();

    unsigned int _Mul;                 // 0x00
    unsigned int _Carry;               // 0x04
};
DECOMP_SIZE_ASSERT(QuickRand, 0x8);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(VertexShaderResource, 0x30);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource)
    {
        if (_Resource)
            _Resource->AddRef();
    }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    void CheckForRegionCrossings(SwampSceneNode* node, const Vector3& start, const Vector3& end, bool testEndOnly) const;
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    ModelManager& GetModelManager() const { return *_modelManager; }
    RegionManager& GetRegionManager() const { return *_regionManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x28 - 0x8];          // 0x08
    ModelManager* _modelManager;       // 0x28
    char field_2C[0x30 - 0x2C];        // 0x2C
    RegionManager* _regionManager;     // 0x30
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    T Fetch();

    std::deque<T> _queue;              // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

// Retail layout = the PDB HannahMain record of size 0x138 (ti 0x142b3)
class Settings { // NAME: proto-pdb:default.pdb
public:
    float GetParamFloat(const SwampString&, float) const;

    char field_0[0x34];                // 0x00
};
DECOMP_SIZE_ASSERT(Settings, 0x34);

class Globals : public Settings { // NAME: proto-pdb:default.pdb
public:
    float GetHannahDebrisLobFactor() const { return _hannahDebrisLobbiness; }

    char field_34[0x790 - 0x34];       // 0x34
    float _hannahDebrisLobbiness;      // 0x790
    char field_794[0xA0C - 0x794];     // 0x794
};
DECOMP_SIZE_ASSERT(Globals, 0xA0C);
extern Globals gSettings; // NAME: proto-pdb:default.pdb

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float);
}

class HannahMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_LAUNCHCRAP = 0x0,
        TIMER_TICKSOUND = 0x1,
        TIMER_TIMESUP = 0x2,
        TIMER_IDLEUPDATE = 0x3
    };
    HannahMain(SwampSceneNode* node);
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
    void SetAttackLevel(int level);
private:
    void TossCrap();
    void SetCrapTimer();
public:

    MultiAnimation _multiAnimIdle;     // 0xBC
    MultiAnimation _multiAnimBlowBack; // 0xD0
    ResourceHandle<AnimationResource> _animTaunt;      // 0xE4
    ResourceHandle<AnimationResource> _animDamage;     // 0xE8
    ResourceHandle<AnimationResource> _animAttack[4];  // 0xEC
    int _attackLevel;                  // 0xFC
    char field_100[0x108 - 0x100];     // 0x100
    SwampShuffleQueue<SwampString> _debrisNames; // 0x108
    char field_11C[0x120 - 0x11C];     // 0x11C
    bool _battleOn;                    // 0x120
    float _minCrapTime;                // 0x124
    float _maxCrapTime;                // 0x128
    char field_12C[0x138 - 0x12C];     // 0x12C
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(HannahMain, 0x138);

// FUNCTION: USA 0x00028C89
// PROTOTYPE: PREVIEW 0x00417ABC identical
// NAME: proto-pdb:default.pdb
float QuickRand::GetRandomFloat()
{
    unsigned int bits = (GetRandom() >> 9) | 0x3F800000;
    return *(float*)&bits - 1.0f;
}

// FUNCTION: USA 0x00028CBE
// PROTOTYPE: PREVIEW 0x00417AF6 identical
// NAME: proto-pdb:default.pdb
void HannahMain::SetAttackLevel(int level)
{
    _attackLevel = level;
}

// FUNCTION: USA 0x00028D58
// PROTOTYPE: PREVIEW 0x00417B90 identical
// NAME: proto-pdb:default.pdb
template ResourceHandle<TextureResource>::ResourceHandle(const ResourceHandle<TextureResource>& h);

// FUNCTION: USA 0x00028D6C
// PROTOTYPE: PREVIEW 0x00417BA4 identical
// NAME: proto-pdb:default.pdb
QuickRand::QuickRand(unsigned __int64 seed)
{
    _Mul = (unsigned int)seed * 2 + 1;
    _Carry = (unsigned int)(seed >> 32) >> 2;
    GetRandom();
}

struct D3DVertexBuffer; // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_SetStreamSource(unsigned int, D3DVertexBuffer*, unsigned int); // src proto-pdb:default.pdb D3DDevice_SetStreamSource
extern "C" void __fastcall D3DDevice_SetVertexShaderConstant1(int, const void*); // src proto-pdb:default.pdb D3DDevice_SetVertexShaderConstant1
extern "C" void __fastcall D3DDevice_SetVertexShaderConstant4(int, const void*); // src proto-pdb:default.pdb D3DDevice_SetVertexShaderConstant4
extern "C" void __fastcall D3DDevice_SetVertexShaderConstantNotInline(int, const void*, unsigned int); // src proto-pdb:default.pdb D3DDevice_SetVertexShaderConstantNotInline

struct Vector4; // NAME: proto-pdb:default.pdb

enum _D3DRENDERSTATETYPE { // NAME: proto-pdb:default.pdb
    D3DRS_SIMPLE_MAX = 0x5C,
    D3DRS_DEFERRED_MAX = 0x88,
    D3DRS_PSTEXTUREMODES = 0x88,
    D3DRS_VERTEXBLEND = 0x89,
    D3DRS_FOGCOLOR = 0x8A,
    D3DRS_FILLMODE = 0x8B,
    D3DRS_BACKFILLMODE = 0x8C,
    D3DRS_TWOSIDEDLIGHTING = 0x8D,
    D3DRS_NORMALIZENORMALS = 0x8E,
    D3DRS_ZENABLE = 0x8F,
    D3DRS_STENCILENABLE = 0x90,
    D3DRS_STENCILFAIL = 0x91,
    D3DRS_FRONTFACE = 0x92,
    D3DRS_CULLMODE = 0x93,
    D3DRS_TEXTUREFACTOR = 0x94,
    D3DRS_ZBIAS = 0x95,
    D3DRS_LOGICOP = 0x96,
    D3DRS_EDGEANTIALIAS = 0x97,
    D3DRS_MULTISAMPLEANTIALIAS = 0x98,
    D3DRS_MULTISAMPLEMASK = 0x99,
    D3DRS_MULTISAMPLEMODE = 0x9A,
    D3DRS_MULTISAMPLERENDERTARGETMODE = 0x9B,
    D3DRS_SHADOWFUNC = 0x9C,
    D3DRS_LINEWIDTH = 0x9D,
    D3DRS_SAMPLEALPHA = 0x9E,
    D3DRS_DXT1NOISEENABLE = 0x9F,
    D3DRS_YUVENABLE = 0xA0,
    D3DRS_OCCLUSIONCULLENABLE = 0xA1,
    D3DRS_STENCILCULLENABLE = 0xA2,
    D3DRS_ROPZCMPALWAYSREAD = 0xA3,
    D3DRS_ROPZREAD = 0xA4,
    D3DRS_DONOTCULLUNCOMPRESSED = 0xA5,
    D3DRS_MAX = 0xA6,
    D3DRS_FORCE_DWORD = 0x7FFFFFFF
};
extern "C" unsigned long D3DSIMPLERENDERSTATEENCODE[]; // src proto-pdb:default.pdb D3DSIMPLERENDERSTATEENCODE
extern "C" unsigned long D3DDIRTYFROMRENDERSTATE[]; // src proto-pdb:default.pdb D3DDIRTYFROMRENDERSTATE
extern "C" unsigned long D3D__DirtyFlags; // src proto-pdb:default.pdb D3D__DirtyFlags
extern "C" unsigned long D3D__RenderState[]; // src proto-pdb:default.pdb D3D__RenderState
extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_Simple
extern "C" void __stdcall D3DDevice_SetRenderState_PSTextureModes(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_PSTextureModes
extern "C" void __stdcall D3DDevice_SetRenderState_VertexBlend(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_VertexBlend
extern "C" void __stdcall D3DDevice_SetRenderState_FogColor(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_FogColor
extern "C" void __stdcall D3DDevice_SetRenderState_FillMode(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_FillMode
extern "C" void __stdcall D3DDevice_SetRenderState_BackFillMode(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_BackFillMode
extern "C" void __stdcall D3DDevice_SetRenderState_TwoSidedLighting(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_TwoSidedLighting
extern "C" void __stdcall D3DDevice_SetRenderState_NormalizeNormals(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_NormalizeNormals
extern "C" void __stdcall D3DDevice_SetRenderState_ZEnable(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_ZEnable
extern "C" void __stdcall D3DDevice_SetRenderState_StencilEnable(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_StencilEnable
extern "C" void __stdcall D3DDevice_SetRenderState_StencilFail(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_StencilFail
extern "C" void __stdcall D3DDevice_SetRenderState_FrontFace(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_FrontFace
extern "C" void __stdcall D3DDevice_SetRenderState_CullMode(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_CullMode
extern "C" void __stdcall D3DDevice_SetRenderState_TextureFactor(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_TextureFactor
extern "C" void __stdcall D3DDevice_SetRenderState_ZBias(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_ZBias
extern "C" void __stdcall D3DDevice_SetRenderState_LogicOp(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_LogicOp
extern "C" void __stdcall D3DDevice_SetRenderState_EdgeAntiAlias(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_EdgeAntiAlias
extern "C" void __stdcall D3DDevice_SetRenderState_MultiSampleAntiAlias(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_MultiSampleAntiAlias
extern "C" void __stdcall D3DDevice_SetRenderState_MultiSampleMask(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_MultiSampleMask
extern "C" void __stdcall D3DDevice_SetRenderState_MultiSampleMode(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_MultiSampleMode
extern "C" void __stdcall D3DDevice_SetRenderState_MultiSampleRenderTargetMode(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_MultiSampleRenderTargetMode
extern "C" void __stdcall D3DDevice_SetRenderState_ShadowFunc(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_ShadowFunc
extern "C" void __stdcall D3DDevice_SetRenderState_LineWidth(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_LineWidth
extern "C" void __stdcall D3DDevice_SetRenderState_SampleAlpha(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_SampleAlpha
extern "C" void __stdcall D3DDevice_SetRenderState_Dxt1NoiseEnable(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_Dxt1NoiseEnable
extern "C" void __stdcall D3DDevice_SetRenderState_YuvEnable(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_YuvEnable
extern "C" void __stdcall D3DDevice_SetRenderState_OcclusionCullEnable(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_OcclusionCullEnable
extern "C" void __stdcall D3DDevice_SetRenderState_StencilCullEnable(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_StencilCullEnable
extern "C" void __stdcall D3DDevice_SetRenderState_RopZCmpAlwaysRead(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_RopZCmpAlwaysRead
extern "C" void __stdcall D3DDevice_SetRenderState_RopZRead(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_RopZRead
extern "C" void __stdcall D3DDevice_SetRenderState_DoNotCullUncompressed(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetRenderState_DoNotCullUncompressed

extern "C" D3DVertexBuffer* __stdcall D3DDevice_CreateVertexBuffer2(unsigned int length); // src proto-pdb:default.pdb D3DDevice_CreateVertexBuffer2
extern "C" unsigned char* __stdcall D3DVertexBuffer_Lock2(D3DVertexBuffer* vb, unsigned long flags); // src proto-pdb:default.pdb D3DVertexBuffer_Lock2
extern "C" long __stdcall D3DXGetErrorStringA(long hr, char* buffer, unsigned int size); // src proto-pdb:default.pdb D3DXGetErrorStringA

class WindyBits { // NAME: proto-pdb:default.pdb
public:
    struct Floater { // NAME: proto-pdb:default.pdb
        char pos[0xC];                 // 0x00
        unsigned int color;            // 0x0C
    };
};

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    template <class T> D3DVertexBuffer* CreateVertexBuffer(const T* verts, unsigned int num) const;
    long SetStreamSource(D3DVertexBuffer* vb, unsigned int stride);
    void SetVertexShaderConstant(int regnum, const Vector4* data, int count) const;
    void SetRenderState(_D3DRENDERSTATETYPE state, unsigned long value);

    char field_0[0x44];                // 0x00
    D3DVertexBuffer* _CurrentVB;       // 0x44
    char field_48[0x38C - 0x48];       // 0x48
};
DECOMP_SIZE_ASSERT(SwampRenderer, 0x38C);

// FUNCTION: USA 0x00028D96
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetRenderState(_D3DRENDERSTATETYPE state, unsigned long value)
{
    // !FAKE: inliner; hand form of the XDK inline D3DDevice_SetRenderState body (retail expands it here)
    if (state < D3DRS_SIMPLE_MAX)
        D3DDevice_SetRenderState_Simple(D3DSIMPLERENDERSTATEENCODE[state], value);
    else if (state < D3DRS_DEFERRED_MAX) {
        D3D__DirtyFlags |= D3DDIRTYFROMRENDERSTATE[state - D3DRS_SIMPLE_MAX];
        D3D__RenderState[state] = value;
    }
    else if (state == D3DRS_PSTEXTUREMODES)
        D3DDevice_SetRenderState_PSTextureModes(value);
    else if (state == D3DRS_VERTEXBLEND)
        D3DDevice_SetRenderState_VertexBlend(value);
    else if (state == D3DRS_FOGCOLOR)
        D3DDevice_SetRenderState_FogColor(value);
    else if (state == D3DRS_FILLMODE)
        D3DDevice_SetRenderState_FillMode(value);
    else if (state == D3DRS_BACKFILLMODE)
        D3DDevice_SetRenderState_BackFillMode(value);
    else if (state == D3DRS_TWOSIDEDLIGHTING)
        D3DDevice_SetRenderState_TwoSidedLighting(value);
    else if (state == D3DRS_NORMALIZENORMALS)
        D3DDevice_SetRenderState_NormalizeNormals(value);
    else if (state == D3DRS_ZENABLE)
        D3DDevice_SetRenderState_ZEnable(value);
    else if (state == D3DRS_STENCILENABLE)
        D3DDevice_SetRenderState_StencilEnable(value);
    else if (state == D3DRS_STENCILFAIL)
        D3DDevice_SetRenderState_StencilFail(value);
    else if (state == D3DRS_CULLMODE)
        D3DDevice_SetRenderState_CullMode(value);
    else if (state == D3DRS_FRONTFACE)
        D3DDevice_SetRenderState_FrontFace(value);
    else if (state == D3DRS_TEXTUREFACTOR)
        D3DDevice_SetRenderState_TextureFactor(value);
    else if (state == D3DRS_ZBIAS)
        D3DDevice_SetRenderState_ZBias(value);
    else if (state == D3DRS_LOGICOP)
        D3DDevice_SetRenderState_LogicOp(value);
    else if (state == D3DRS_EDGEANTIALIAS)
        D3DDevice_SetRenderState_EdgeAntiAlias(value);
    else if (state == D3DRS_MULTISAMPLEANTIALIAS)
        D3DDevice_SetRenderState_MultiSampleAntiAlias(value);
    else if (state == D3DRS_MULTISAMPLEMASK)
        D3DDevice_SetRenderState_MultiSampleMask(value);
    else if (state == D3DRS_MULTISAMPLEMODE)
        D3DDevice_SetRenderState_MultiSampleMode(value);
    else if (state == D3DRS_MULTISAMPLERENDERTARGETMODE)
        D3DDevice_SetRenderState_MultiSampleRenderTargetMode(value);
    else if (state == D3DRS_SHADOWFUNC)
        D3DDevice_SetRenderState_ShadowFunc(value);
    else if (state == D3DRS_LINEWIDTH)
        D3DDevice_SetRenderState_LineWidth(value);
    else if (state == D3DRS_SAMPLEALPHA)
        D3DDevice_SetRenderState_SampleAlpha(value);
    else if (state == D3DRS_DXT1NOISEENABLE)
        D3DDevice_SetRenderState_Dxt1NoiseEnable(value);
    else if (state == D3DRS_YUVENABLE)
        D3DDevice_SetRenderState_YuvEnable(value);
    else if (state == D3DRS_OCCLUSIONCULLENABLE)
        D3DDevice_SetRenderState_OcclusionCullEnable(value);
    else if (state == D3DRS_STENCILCULLENABLE)
        D3DDevice_SetRenderState_StencilCullEnable(value);
    else if (state == D3DRS_ROPZCMPALWAYSREAD)
        D3DDevice_SetRenderState_RopZCmpAlwaysRead(value);
    else if (state == D3DRS_ROPZREAD)
        D3DDevice_SetRenderState_RopZRead(value);
    else if (state == D3DRS_DONOTCULLUNCOMPRESSED)
        D3DDevice_SetRenderState_DoNotCullUncompressed(value);
}

// FUNCTION: USA 0x00029017
// NAME: proto-pdb:default.pdb
void SwampRenderer::SetVertexShaderConstant(int regnum, const Vector4* data, int count) const
{
    if (count == 1)
        D3DDevice_SetVertexShaderConstant1(regnum + 96, data);
    else if (count == 4)
        D3DDevice_SetVertexShaderConstant4(regnum + 96, data);
    else
        D3DDevice_SetVertexShaderConstantNotInline(regnum + 96, data, count * 4);
}

// FUNCTION: USA 0x0002904B
// PROTOTYPE: PREVIEW 0x00417E83 identical
// NAME: proto-pdb:default.pdb
long SwampRenderer::SetStreamSource(D3DVertexBuffer* vb, unsigned int stride)
{
    if (vb != _CurrentVB) {
        _CurrentVB = vb;
        D3DDevice_SetStreamSource(0, vb, stride);
    }
    return 0;
}

// FUNCTION: USA 0x00029068
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // best 92.91 (T14.c5): x87 spill order in the p1 term (retail keeps t1a.x on the stack, stores y/z; ours stores x/y) after ~25 attempts
void VinceBlowBack::FrameTick()
{
    _time += g_swamp->_input->GetElapsedTime();
    if (_time > _totalTime) {
        _time = _totalTime;
        InvalidateScript();
    }
    // !FAKE: register local; v0 is the x87-held u (no PDB record: register locals on the FP stack are not emitted)
    float v0 = _time / _totalTime;
    float u2 = v0 * v0;
    float u3 = u2 * v0;
    float iu = 1.0f - v0;
    float iu3 = iu * iu * iu;
    // !FAKE: inliner; t3..s012, d, c are hand forms of the inlined Vector3 free operators (no PDB record, D78)
    const Vector3& p3 = _controlPoint[3];
    Vector3 t3;
    t3.x = u3 * p3.x;
    t3.y = u3 * p3.y;
    t3.z = u3 * p3.z;
    Vector3 t2;
    {
        const Vector3& p2 = _controlPoint[2];
        Vector3 t2a;
        t2a.x = p2.x * 3.0f;
        t2a.y = p2.y * 3.0f;
        t2a.z = p2.z * 3.0f;
        Vector3 t2b;
        t2b.x = t2a.x * u2;
        t2b.y = t2a.y * u2;
        t2b.z = t2a.z * u2;
        t2.x = t2b.x * iu;
        t2.y = t2b.y * iu;
        t2.z = t2b.z * iu;
    }
    Vector3 t1;
    {
        const Vector3& p1 = _controlPoint[1];
        Vector3 t1a;
        t1a.x = p1.x * 3.0f;
        t1a.y = p1.y * 3.0f;
        t1a.z = p1.z * 3.0f;
        Vector3 t1b;
        t1b.x = t1a.x * v0;
        t1b.y = t1a.y * v0;
        t1b.z = t1a.z * v0;
        t1.x = t1b.x * (iu * iu);
        t1.y = t1b.y * (iu * iu);
        t1.z = t1b.z * (iu * iu);
    }
    const Vector3& p0 = _controlPoint[0];
    Vector3 t0;
    t0.x = iu3 * p0.x;
    t0.y = iu3 * p0.y;
    t0.z = iu3 * p0.z;
    Vector3 s01;
    s01.x = t0.x + t1.x;
    s01.y = t0.y + t1.y;
    s01.z = t0.z + t1.z;
    Vector3 s012;
    s012.x = s01.x + t2.x;
    s012.y = s01.y + t2.y;
    s012.z = s01.z + t2.z;
    Vector3 newloc;
    newloc.x = s012.x + t3.x;
    newloc.y = s012.y + t3.y;
    newloc.z = s012.z + t3.z;
    Vector3 oldloc = _node->GetLocation();
    _node->SetLocation(newloc);
    g_swamp->GetRegionManager().CheckForRegionCrossings(_node, oldloc, newloc, false);
    {
        Vector3 d;
        d.x = newloc.x - oldloc.x;
        d.y = newloc.y - oldloc.y;
        d.z = newloc.z - oldloc.z;
        Vector3 c;
        c.x = d.x + _vincecam->GetLocation().x;
        c.y = d.y + _vincecam->GetLocation().y;
        c.z = d.z + _vincecam->GetLocation().z;
        _vincecam->SetLocation(c);
    }
}
#endif

// FUNCTION: USA 0x00028CE3
// NAME: proto-pdb:default.pdb
template <class T> D3DVertexBuffer* SwampRenderer::CreateVertexBuffer(const T* verts, unsigned int num) const
{
    unsigned int size = num * sizeof(T);
    D3DVertexBuffer* vb = D3DDevice_CreateVertexBuffer2(size);
    long hr = vb ? 0 : 0x8007000E;
    if (hr < 0) {
        char buff[0x80];
        D3DXGetErrorStringA(hr, buff, sizeof(buff));
    }
    unsigned char* p = D3DVertexBuffer_Lock2(vb, 0x30);
    memcpy(p, verts, size);
    return vb;
}
template D3DVertexBuffer* SwampRenderer::CreateVertexBuffer<WindyBits::Floater>(const WindyBits::Floater*,
    unsigned int) const;

// FUNCTION: USA 0x000292A6
// NAME: proto-pdb:default.pdb
template ResourceHandle<VertexShaderResource>::~ResourceHandle();

// FUNCTION: USA 0x00029448
// PROTOTYPE: PREVIEW 0x00418205 identical
// NAME: proto-pdb:default.pdb
void HannahMain::SetCrapTimer()
{
    _minCrapTime = gSettings.GetParamFloat(format("HANNAH_DEBRIS_FREQUENCY_%d_MIN", _attackLevel), 0.0f);
    _maxCrapTime = gSettings.GetParamFloat(format("HANNAH_DEBRIS_FREQUENCY_%d_MAX", _attackLevel), 2.0f);
    RegisterTimerEvent(TIMER_LAUNCHCRAP, BeepLib::RandomFloat(_minCrapTime, _maxCrapTime), false);
}

// FUNCTION: USA 0x00029821
// PROTOTYPE: PREVIEW 0x004185DE identical
// NAME: proto-pdb:default.pdb
void HannahMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    if (_state == 4 && value > 0.0f) {
        {
            // !FAKE: inliner; stand-in locals, hand form of the inlined SwampSceneNode::SetVisible(false, false);
            // its in-class body emits a COMDAT named like CrawdadJimmys' matched row 0x00019DA1
            SwampString v0("lighthouse");
            Critter* p = pManager->Find(v0);
            p->SetOption(2, true, false); // proto-pdb:default.pdb SwampSceneNode::DontDraw
        }
        GetCritter()->ReturnToHome();
        AnimCam::SwitchTo("camera_vinceBlowback", 0.0f, false, 0.0f);
        pManager->GetVince()->PopScript();
        pManager->GetVince()->SetScript("vinceblowback");
    }
}

// FUNCTION: USA 0x000298F3
// PROTOTYPE: PREVIEW 0x004186B0 identical
// NAME: proto-pdb:default.pdb
void HannahStartBattleRegionMain::FrameTick()
{
    // !FAKE: c1xx front end; stand-in local; ends the string temporary before the test (not a PDB local)
    Critter* tmp = pManager->Find("hannah");
    if (tmp) {
        if (pManager->GetVince()->IsOnFloor()) {
            tmp->GetScript()->Message(this, 0x17, 0); // proto-pdb:default.pdb HannahMain::MESSAGE_BATTLEON
            UnRegisterFrameTick();
        }
    } else {
        UnRegisterFrameTick();
    }
}

// FUNCTION: USA 0x0002995A
// PROTOTYPE: PREVIEW 0x00418717 identical
// NAME: proto-pdb:default.pdb
void HannahStartBattleRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (node == pManager->GetVince()) {
        // !FAKE: c1xx front end; stand-in local; ends the string temporary before the test (not a PDB local)
        Critter* tmp = pManager->Find("hannah");
        if (tmp)
            tmp->GetScript()->Message(this, 0x18, 0); // proto-pdb:default.pdb HannahMain::MESSAGE_BATTLEOFF
        UnRegisterFrameTick();
    }
}

// FUNCTION: USA 0x00029AF2
// PROTOTYPE: PREVIEW 0x004188AF identical
// NAME: proto-pdb:default.pdb
void Critter::SetSpecies(SwampString s)
{
    _species = s;
}

// FUNCTION: USA 0x00029B16
// PROTOTYPE: PREVIEW 0x004188D3 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceBlowBack::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("HurricaneHanna\\Hannah.cpp", 31) VinceBlowBack(node);
}

// FUNCTION: USA 0x0002A0A1
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1HannahMain@@UAE@XZ because this unit defines
// the ctor (not matched); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_HannahMain_dtor(HannahMain* p)
{
    p->HannahMain::~HannahMain();
}

// FUNCTION: USA 0x0002A0FC
// PROTOTYPE: PREVIEW 0x00418EB9 identical
// NAME: proto-pdb:default.pdb
SwampScript* HannahMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\hurricanehanna\\Hannah.h", 48) HannahMain(node);
}

// FUNCTION: USA 0x0002A123
// PROTOTYPE: PREVIEW 0x00418EE0 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 95.75, bytes mismatch (frame slot packing of the inlined Vector3 temps; Create arg regalloc)
void HannahMain::TossCrap()
{
    if (!_battleOn)
        return;
    Vince* vince = pManager->GetVince();
    Vector3 startloc;
    Quaternion rot;
    GetCritter()->GetBoneOrientation("mouth", rot, startloc);
    startloc += GetCritter()->GetWorldPosition();
    Vector3 vinceloc = vince->GetWorldPosition();
    SwampString name = _debrisNames.Fetch();
    Critter* crap = pManager->Create("", "HannahDebris", name.c_str(), g_swamp->GetModelManager().Load(name + "_base"),
                                     startloc, GetCritter()->GetFacing(), GetNode()->GetZone());
    crap->SetSpecies(name);
    Vector3 vel = crap->GetVelocityToTarget(startloc, vinceloc, crap->GetGravity(), gSettings.GetHannahDebrisLobFactor());
    Vector3 diff;
    diff.x = vinceloc.x - startloc.x;
    diff.z = vinceloc.z - startloc.z;
    // !FAKE: inliner; stand-in local; hand form of the inlined x-z length
    float v0 = diff.z * diff.z + diff.x * diff.x;
    float dist = FastReciprocalSquareRoot(v0) * v0;
    diff = vel;
    v0 = diff.z * diff.z + diff.x * diff.x;
    float crapspeed = FastReciprocalSquareRoot(v0) * v0;
    float v1 = dist / crapspeed; // !FAKE: inliner; stand-in local; hand form of the inlined Vector3 * float
    Vector3 vincevel = vince->GetVelocity();
    vincevel.y = 0.0f;
    vinceloc += Vector3(vincevel.x * v1, vincevel.y * v1, vincevel.z * v1);
    diff.x = vinceloc.x - startloc.x;
    diff.y = vinceloc.y - startloc.y;
    diff.z = vinceloc.z - startloc.z;
    Vector3 destloc;
    {
        Vector3 v2; // !FAKE: inliner; stand-in local; hand form of the inlined Vector3 * float
        v2.x = diff.x * 0.5f;
        v2.y = diff.y * 0.5f;
        v2.z = diff.z * 0.5f;
        destloc = Vector3(v2.x + startloc.x, v2.y + startloc.y, v2.z + startloc.z);
    }
    vel = crap->GetVelocityToTarget(startloc, destloc, crap->GetGravity(), gSettings.GetHannahDebrisLobFactor());
    float v3 = (_attackLevel - 1) * 0.062831856f; // !FAKE: c2 register allocation; stand-in local; one angle value
    Quaternion(Vector3(0.0f, 1.0f, 0.0f), BeepLib::RandomFloat(-v3, v3)).Apply(vel, diff);
    vel = diff;
    crapspeed = gSettings.GetParamFloat(format("HANNAH_DEBRIS_SPEED_%d", _attackLevel), 1.0f);
    crap->SetVelocity(Vector3(vel.x * crapspeed, vel.y * crapspeed, vel.z * crapspeed));
    SoundManager::PlaySound("hurrspit", 0x20, GetNode(), true, 0.0f, 0.0f);
}
#endif
