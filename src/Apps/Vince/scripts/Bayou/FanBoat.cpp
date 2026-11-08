// UNIT: src/Apps/Vince/scripts/Bayou/FanBoat.cpp OBJ: Xbox\OfficialRelease\FanBoat.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as Oven.cpp
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <math.h>
#include <xmmintrin.h>
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class AABox;                 // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
class Quaternion;            // NAME: proto-pdb:default.pdb
class TextureResource;       // NAME: proto-pdb:default.pdb

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    void Normalize();
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct WakeVertex { // NAME: proto-pdb:default.pdb
    Vector3 pos;                       // 0x00
    unsigned long color;               // 0x0C
    Vector2 texcoord;                  // 0x10
};
DECOMP_SIZE_ASSERT(WakeVertex, 0x18);

#ifdef NON_MATCHING // D78, D117: default.pdb records no free Vector3 operator (5.2 T20)
inline Vector3 operator+(const Vector3& a, const Vector3& b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vector3 operator*(const Vector3& a, float s) { return Vector3(a.x * s, a.y * s, a.z * s); }
#endif

class AABox { // NAME: proto-pdb:default.pdb
public:
    void AddPoint(const Vector3&);

    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x630];               // 0x000 (not reached)
    float _fanBoatWakeSegmentLife;     // 0x630
    float _fanBoatWakeWidthStart;      // 0x634
    float _fanBoatWakeWidthEnd;        // 0x638
    char field_63C[0xA0C - 0x63C];     // 0x63C (not reached)
};
DECOMP_SIZE_ASSERT(Globals, 0xA0C);
extern Globals gSettings; // NAME: proto-pdb:default.pdb

class Quaternion { // NAME: proto-pdb:default.pdb
public:
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }
    void Apply(const Vector3&, Vector3&) const;
    void Set(float, float, float);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

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
    Vector2 GetLeftStick() const;

private:
    static void AdjustForDeadzone(Vector2&);

public:
    char field_0[0x2C];                // 0x00
    _XINPUT_GAMEPAD _current;          // 0x2C
    _XINPUT_GAMEPAD _previous;         // 0x3E
    char field_50[0x98 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampGamePad, 0x98);

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
    const Quaternion& GetRotation() const { return _Rotation; }
    const SwampString& GetName() const { return _Name; }
    class SwampScript* GetScript() const;
    void SetScript(const SwampString&);

    char field_4[0x4];                 // 0x04 (_Type; not reached)
    SwampString _Name;                 // 0x08
    char field_24[0x40 - 0x24];        // 0x24 (_Parent .. _Location; not reached)
    Quaternion _Rotation;              // 0x40
    char field_50[0x6C - 0x50];        // 0x50 (_Scale .. _cachedWorldPositionValid; not reached)
    AABox _BoundingBox;                // 0x6C
    char field_84[0xA4 - 0x84];        // 0x84 (_lastRenderFrame .. _scripts; not reached)
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class SoundResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int _RefCount;            // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    char field_4[0x40 - 0x4];          // 0x04 (_tag .. _platformRot; not reached)
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

struct _D3DVERTEXATTRIBUTEFORMAT; // NAME: proto-pdb:default.pdb

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class PixelShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class VertexShaderManager : public ResourceManager<VertexShaderResource> { // NAME: proto-pdb:default.pdb
};

class PixelShaderManager : public ResourceManager<PixelShaderResource> { // NAME: proto-pdb:default.pdb
};

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void SetTexture(unsigned long, const ResourceHandle<TextureResource>&);
    void SetEmptyTexture(unsigned long);
    long SetVertexShader(const ResourceHandle<VertexShaderResource>&, const _D3DVERTEXATTRIBUTEFORMAT*);
    long SetPixelShader(const ResourceHandle<PixelShaderResource>&);
};

extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawVerticesUP(unsigned long, unsigned int, const void*, unsigned int); // NAME: proto-pdb:default.pdb

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    SwampRenderer& GetRenderer() const { return *_renderer; }
    PixelShaderManager& GetPixelShaderManager() const { return *_pixelShaderManager; }
    VertexShaderManager& GetVertexShaderManager() const { return *_vertexShaderManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x2C - 0xC];          // 0x0C (not reached)
    PixelShaderManager* _pixelShaderManager; // 0x2C
    char field_30[0x3C - 0x30];        // 0x30 (not reached)
    VertexShaderManager* _vertexShaderManager; // 0x3C
    char field_40[0x68 - 0x40];        // 0x40 (not reached)
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000307F3
// PROTOTYPE: PREVIEW 0x0041EF3A identical
// NAME: proto-pdb:default.pdb
class FanBoatWake : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    struct WakeSegment { // NAME: proto-pdb:default.pdb
        Vector3 _center;               // 0x00
        Vector3 _side;                 // 0x0C
        float _age;                    // 0x18
        float _texOffset;              // 0x1C
        float _alpha;                  // 0x20
        float _atten;                  // 0x24
    };

    void AddSegment(Vector3, Vector3);
    virtual void Update();
    virtual void Render(const Matrix&) const;

private:
    static _D3DVERTEXATTRIBUTEFORMAT _vaf;

public:
    std::deque<WakeSegment> _wakeSegments; // 0xA4
    WakeVertex _wakeVerts0[128];       // 0xB8
    WakeVertex _wakeVerts1[128];       // 0xCB8
    WakeVertex _wakeVerts2[128];       // 0x18B8
    WakeVertex _wakeVerts3[128];       // 0x24B8
    int _numVerts;                     // 0x30B8
    float _texOffsetNext;              // 0x30BC
    ResourceHandle<TextureResource> _wakeTexture; // 0x30C0
    bool _deleteWhenEmpty;             // 0x30C4
    float _age;                        // 0x30C8
};
DECOMP_SIZE_ASSERT(FanBoatWake::WakeSegment, 0x28);
DECOMP_SIZE_ASSERT(FanBoatWake, 0x30CC);

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0154); retail calls these out of line at
// named fold-alias VAs (_Tidy 0x0002E6E8); a header body would emit a second copy of the fold VA here; the string ctor
// and _Tidy (AnimationFinished's temporaries) are other units' rows (precheck relink overlap at 0x000111D6 without)
namespace std {
template <> void deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::_Tidy();
template <> void deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::_Xlen() const; // 0x0001D5C2
template <> void deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::_Growmap(unsigned int); // 0x00030A12, defined below
template <> basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string(const char*); // 0x000117DE
template <> void basic_string<char,std::char_traits<char>,std::allocator<char> >::_Tidy(bool, unsigned int); // 0x000111D6
template <> basic_string<char,std::char_traits<char>,std::allocator<char> >& basic_string<char,std::char_traits<char>,std::allocator<char> >::assign(const basic_string<char,std::char_traits<char>,std::allocator<char> >&, unsigned int, unsigned int); // 0x00011655
template <> void vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >::_Xlen() const; // 0x00139A45
template <> void vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >::_Tidy(); // 0x000C7B97
template <> void _Uninit_fill_n<basic_string<char,std::char_traits<char>,std::allocator<char> > *,unsigned int,basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<basic_string<char,std::char_traits<char>,std::allocator<char> > > >(basic_string<char,std::char_traits<char>,std::allocator<char> > *,unsigned int,const basic_string<char,std::char_traits<char>,std::allocator<char> >&,std::allocator<basic_string<char,std::char_traits<char>,std::allocator<char> > >&,_Nonscalar_ptr_iterator_tag); // 0x00030B31
template <> void _Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Erase(std::_Tree_nod<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Node *); // 0x0003289B, guarded below
}
#endif

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
    void RegisterFrameTick();
protected:
    SwampScript(SwampSceneNode*);
public:

    char field_4[0x1C];              // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    struct ScriptTimerEvent { // NAME: proto-pdb:default.pdb
        SwampScript* script;           // 0x00
        unsigned int id;               // 0x04
        bool looped;                   // 0x08
        float deltaTime;               // 0x0C
        float absTime;                 // 0x10
    };
};
DECOMP_SIZE_ASSERT(SwampScriptManager::ScriptTimerEvent, 0x14);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_VINCEONBOARD = 0x12
};

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetCollidable(bool c) { _collidable = c; }

    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class GatorNode : public CollidableNode { // NAME: proto-pdb:default.pdb (via CollidableCylinder, not reached)
public:
    float _height;                     // 0xAC (CollidableCylinder)
    float _radius;                     // 0xB0 (CollidableCylinder)
    char field_B4[0xD4 - 0xB4];        // 0xB4 (not reached)
    int _posBone;                      // 0xD4
    char field_D8[0xF4 - 0xD8];        // 0xD8 (_blendMode .. _shadowTex; not reached)
    int _shadowBone;                   // 0xF4
    char field_F8[0x200 - 0xF8];       // 0xF8 (not reached)
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void CheckRegions();
    void SetVelocity(const Vector3& vel) { _Velocity = vel; }

    char field_200[0x20C - 0x200];     // 0x200 (not reached)
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x2C8 - 0x218];     // 0x218 (_species .. ; not reached)
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetVehicleName(const SwampString&);
    Critter* GetVehicle() const;
};

class FanBoat : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00 (not reached)
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    float GetFOV() const { return _fov; }

    float _fov;                        // 0xA4
    char field_A8[0x10C - 0xA8];       // 0xA8 (not reached)
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    char field_10C[0x11C - 0x10C];     // 0x10C (not reached)
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void ClearPositionFixed();

    float _speed;                      // 0x11C
    char field_120[0x1];               // 0x120 (not reached)
    bool _triggerEnabled;              // 0x121
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class ActivatorMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0xE0 - 0xBC];        // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(ActivatorMain, 0xE0);

class Activator_FanBoatMain : public ActivatorMain { // NAME: proto-pdb:default.pdb
public:
    Activator_FanBoatMain(SwampSceneNode* node);
    virtual bool TestActivate() const;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Activator_FanBoatMain, 0xE0);

class VinceEnterBoat : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    VinceEnterBoat(SwampSceneNode* node);
    virtual void AnimationFinished();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    int _oldBone;                      // 0x4C
    int _oldShadowBone;                // 0x50
};
DECOMP_SIZE_ASSERT(VinceEnterBoat, 0x54);

class VinceExitBoat : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    VinceExitBoat(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_4C[0x54 - 0x4C];        // 0x4C (not reached)
};
DECOMP_SIZE_ASSERT(VinceExitBoat, 0x54);

class FanBoatExitArea : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    FanBoatExitArea(SwampSceneNode* node);
    virtual ~FanBoatExitArea();
    virtual void FrameTick();
    virtual void Message(SwampScript*, int, void*);

    Quaternion _qstart;                // 0x4C
    Quaternion _qend;                  // 0x5C
    Vector3 _velstart;                 // 0x6C
    Vector3 _velend;                   // 0x78
    float _age;                        // 0x84
};
DECOMP_SIZE_ASSERT(FanBoatExitArea, 0x88);

class FanBoatMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    FanBoatMain(SwampSceneNode* node);
    bool IsInWater();
    Vector3 GetParkingDest(const SwampString&);
    virtual bool TestActivate() const;
    virtual void Activate();
    FanBoat* GetBoat() { return (FanBoat*)_node; }
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    float _waterSurfaceY;              // 0xBC
    char field_C0[0xE8 - 0xC0];        // 0xC0 (_interp .. _destRot; not reached)
    Quaternion _noBankRot;             // 0xE8
    char field_F8[0x148 - 0xF8];       // 0xF8 (_bank .. _baseRot; not reached)
    float _curThrottle;                // 0x148
    float _oldCamSpeed;                // 0x14C
    float _oldFOV;                     // 0x150
    char field_154[0x188 - 0x154];     // 0x154 (_FOVBlend .. _destLoc; not reached)
    SwampString _curPad;               // 0x188
    char field_1A4[0x210 - 0x1A4];     // 0x1A4 (_curParkingPct .. _wake; not reached)
};
DECOMP_SIZE_ASSERT(FanBoatMain, 0x210);

// FUNCTION: USA 0x0002DEA0
// PROTOTYPE: PREVIEW 0x0041C911 identical
// NAME: proto-pdb:default.pdb
Vector3 Normalize(const Vector3& vec)
{
    float tmp = FastReciprocalSquareRoot(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    return Vector3(vec.x * tmp, vec.y * tmp, vec.z * tmp);
}

// FUNCTION: USA 0x0002DEEE
// PROTOTYPE: PREVIEW 0x0041C95F identical
// NAME: proto-pdb:default.pdb
Vector2 SwampGamePad::GetLeftStick() const
{
    Vector2 result;
    result.u = _current.sThumbLX * (1.0f / 32767.0f);
    result.v = _current.sThumbLY * (1.0f / 32767.0f);
    AdjustForDeadzone(result);
    return result;
}

// FUNCTION: USA 0x0002DF5B
// PROTOTYPE: PREVIEW 0x0041C9CC identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; wall-fold-va-callers (C0273): init.obj also defines this alias at the fold VA (gate objects extra)
template void std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Rrotate(std::_Tree_nod<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Node *);
#endif

// FUNCTION: USA 0x0002DF9F
// PROTOTYPE: PREVIEW 0x004005BA identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; wall-fold-va-callers (C0273): init.obj and SwampScript.obj also define this alias at the fold VA (gate objects extra)
template void std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::const_iterator::_Inc();
#endif

// FUNCTION: USA 0x0002E161
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); wall (5.3 T6.c7): denominator gap, 0x0002E161 is no entry in
// derived/xdk functions.jsonl (finish progress.py refuses, C0322); lift: the VA made a function (re-derive)
bool FanBoatMain::TestActivate() const
{
    return _state == 1 ? true : false;
}
#endif

// FUNCTION: USA 0x0002E2E6
// PROTOTYPE: PREVIEW 0x0041CCA9 identical
// NAME: proto-pdb:default.pdb
bool FanBoatMain::IsInWater()
{
    return _node->GetWorldPosition().y - _waterSurfaceY < 0.005f;
}

// FUNCTION: USA 0x0002E31B
// PROTOTYPE: PREVIEW 0x0041CCDE identical
// NAME: proto-pdb:default.pdb
Vector3 FanBoatMain::GetParkingDest(const SwampString& padname)
{
    Vector3 dest;
    Critter* pad = pManager->Find(padname);
    if (pad) {
        // !FAKE: inliner; comma expression; hand form of the PDB by-value Quaternion::Apply(v), out of line at /O1; one
        // full-expression keeps the input temp off dest's slot (match_one 97.99 -> 99.77 two statements -> 100.0)
        // !FAKE: inliner; stand-in pointer locals v1, p (C0191) and the PDB ctor, hand form of a Vector3 sum (no free
        // operator in the PDB, D78); split statements 88.0, `Vector3(tmp) += pos` 99.75 (one slot short)
        Vector3 tmp;
        const Vector3* v1;
        const Vector3* p;
        dest = (pad->GetRotation().Apply(Vector3(-0.261f, 0.0f, 0.562f), tmp), v1 = &Vector3(tmp), p = &pad->GetWorldPosition(),
                Vector3(v1->x + p->x, v1->y + p->y, v1->z + p->z));
    } else
        dest = _node->GetWorldPosition();
    dest.y = _waterSurfaceY;
    return dest;
}

// FUNCTION: USA 0x000305AF
// PROTOTYPE: PREVIEW 0x0041ECF6 identical
// NAME: proto-pdb:default.pdb
void FanBoatMain::Activate()
{
    Vince* vince = pManager->GetVince();
    // !FAKE: c2 dead store; unnamed result local; retail makes both virtual position calls into packed temps and
    // drops the result (PDB lists no local); the repeated GetVince() gives retail's `mov eax,edi` object copy
    // (match_one 91.15 two named locals -> 96.35 one expression -> 100.0)
    // !FAKE: inliner; stand-in locals v2 (retail loads GetBoat() before the first call), p, v1 (C0191) and the PDB
    // ctor, hand form of a Vector3 difference (no free operator in the PDB, D78); `-=` on the temp 97.12
    FanBoat* v2 = GetBoat();
    const Vector3* p;
    const Vector3* v1;
    Vector3 delta = (p = &pManager->GetVince()->GetWorldPosition(), v1 = &v2->GetWorldPosition(),
                     Vector3(v1->x - p->x, v1->y - p->y, v1->z - p->z));
    if (_curPad.size() != 0 && vince->GetVehicle() != GetBoat())
        vince->SetScript("VinceEnterBoat");
    vince->SetCollidable(false);
    GetBoat()->SetCollidable(false);
    RegisterFrameTick();
    VinceCam* cam = static_cast<VinceCam*>(g_CameraManager->Find("VINCECAM"));
    _oldCamSpeed = cam->_speed;
    _oldFOV = cam->GetFOV();
    cam->_speed = 5.0f;
    cam->_triggerEnabled = false;
    vince->SetVehicleName(_node->GetName());
    _noBankRot = _node->GetWorldRotation();
    if (_state != 0)
        SetState(2);
    _curThrottle = 0.0f;
    GetBoat()->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
}

// FUNCTION: USA 0x0002E52C
// PROTOTYPE: PREVIEW 0x004993D9 identical
// NAME: proto-pdb:default.pdb
template void std::_Construct<FanBoatWake::WakeSegment,FanBoatWake::WakeSegment>(FanBoatWake::WakeSegment *,FanBoatWake::WakeSegment const &);

// FUNCTION: USA 0x0002E542
// PROTOTYPE: PREVIEW 0x0041CDD2 identical
// NAME: proto-pdb:default.pdb
template std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator std::_Copy_backward_opt<std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator,std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator>(std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator,std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator,std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0002E598
// PROTOTYPE: PREVIEW 0x0041CE28 identical
// NAME: proto-pdb:default.pdb
template std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator std::_Copy_opt<std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator,std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator>(std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator,std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator,std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::iterator,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0002E6C6
// PROTOTYPE: PREVIEW 0x0041CE7E identical
// NAME: proto-pdb:default.pdb
template void std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::pop_front();

// FUNCTION: USA 0x0002E6E8
// PROTOTYPE: PREVIEW 0x00448ABB identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; wall-fold-va-callers (C0273): SwampGatorNode.obj defines the deque<unsigned int>::_Tidy alias at this fold VA (gate objects extra)
template void std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::_Tidy();
#endif

// FUNCTION: USA 0x0002E831
// PROTOTYPE: PREVIEW 0x0041CF99 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 95.62, bytes mismatch at +0x511 (T12.c29: WakeVertex& stand-in fixes the strip base, was 94.54);
// left: in the three `pos = pos = ..; u = u = ..;` pairs cl hoists the two texcoord.u fst above the 2nd pos copy, retail
// after it; tail add ebx/fadd order (u reversed/split/first, pos split, temp: no change). T12.c7: PDB frame slots 93.80,
// alpha int copy 94.54; _mm_set_ss 93.11, by-value helper 93.2, operator- vdelta 89.06 (log T12.c7, T12.c29)
void FanBoatWake::Update()
{
    float wakeLife = gSettings._fanBoatWakeSegmentLife;
    float wakeHalfWidthMin = gSettings._fanBoatWakeWidthStart;
    float offsetAdjust;
    int numSegments;
    int numVerts;
    float minTexOffset;
    float wakeHalfWidthMax = gSettings._fanBoatWakeWidthEnd;
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    std::deque<WakeSegment>::iterator it;
    _age += elapsed;

    numSegments = 0;
    minTexOffset = 3.4028235e38f;
    for (it = _wakeSegments.begin(); it != _wakeSegments.end(); ++it) {
        WakeSegment& segment = *it;
        if (segment._age > wakeLife + elapsed) {
            _wakeSegments.erase(it, _wakeSegments.end());
            break;
        }
        if (segment._texOffset < minTexOffset)
            minTexOffset = segment._texOffset;
        numSegments++;
    }

    if (numSegments <= 1 && _deleteWhenEmpty) {
        Invalidate();
        return;
    }

    numVerts = 0;
    offsetAdjust = 0.0f;
    if (minTexOffset > 1.0f)
        offsetAdjust = floor(minTexOffset);
    _texOffsetNext -= offsetAdjust;

    if (numSegments > 1) {
        WakeSegment* prevSegment = 0;
        float recipWakeLife = 1.0f / wakeLife;
        for (it = _wakeSegments.begin(); it != _wakeSegments.end(); ++it) {
            WakeSegment& segment = *it;
            if (numVerts >= 128)
                break;

            float t = segment._age * (recipWakeLife * (segment._atten * segment._atten));
            float w = (1.0f - t) * wakeHalfWidthMin + t * wakeHalfWidthMax;
            Vector3 offset(segment._side.x * w, segment._side.y * w, segment._side.z * w);
            segment._texOffset -= offsetAdjust;

            float life = segment._age / wakeLife;
            if (1.0f < life)
                life = 1.0f;
            float fadeIn = segment._age * 16.0f;
            if (!(fadeIn < 1.0f))
                fadeIn = 1.0f;
            float alpha = segment._atten * (fadeIn * (1.0f - life)) * 255.0f;
            if (_deleteWhenEmpty && alpha > segment._alpha)
                alpha = segment._alpha;
            segment._alpha = alpha;

            // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand (as HUDUI); no PDB local
            // !FAKE: c2 store forwarding; the operand copied as its bit pattern so the x87 alpha is popped, as retail
            int a = *(int*)&segment._alpha;
            _wakeVerts3[numVerts + 1].color = _wakeVerts2[numVerts + 1].color = _wakeVerts1[numVerts + 1].color =
                _wakeVerts0[numVerts + 1].color = _wakeVerts3[numVerts].color = _wakeVerts2[numVerts].color =
                _wakeVerts1[numVerts].color = _wakeVerts0[numVerts].color = (_mm_cvtt_ss2si(_mm_load_ss((float*)&a)) << 24) + 0xFFFFFF;
            _wakeVerts0[numVerts].texcoord.v = segment._texOffset;
            _wakeVerts1[numVerts].texcoord.v = segment._texOffset;
            _wakeVerts2[numVerts].texcoord.v = segment._texOffset;
            _wakeVerts3[numVerts].texcoord.v = segment._texOffset;
            _wakeVerts0[numVerts + 1].texcoord.v = segment._texOffset;
            _wakeVerts1[numVerts + 1].texcoord.v = segment._texOffset;
            _wakeVerts2[numVerts + 1].texcoord.v = segment._texOffset;
            _wakeVerts3[numVerts + 1].texcoord.v = segment._texOffset;

            _wakeVerts0[numVerts].pos = segment._center - offset;
            _wakeVerts0[numVerts].texcoord.u = 0.0f;
            _BoundingBox.AddPoint(_wakeVerts0[numVerts].pos);
            // !FAKE: c2 strength reduction; stand-in reference; loop base ebx = &_wakeVerts3[n+1].pos.z without it, retail .pos.y (T12.c29 probe: 57 -> 33 diff lines)
            WakeVertex& p = _wakeVerts3[numVerts + 1];
            p.pos = segment._center + offset;
            p.texcoord.u = 1.0f;
            _BoundingBox.AddPoint(p.pos);

            if (prevSegment) {
                // !FAKE: c2 x87 register allocation; neutral x87-resident local (no PDB record); retail keeps the
                // centre delta on the x87 stack across both edge tests
                Vector3 d(segment._center.x - prevSegment->_center.x, segment._center.y - prevSegment->_center.y,
                          segment._center.z - prevSegment->_center.z);
                Vector3 vdelta(_wakeVerts3[numVerts + 1].pos.x - _wakeVerts3[numVerts - 1].pos.x,
                               _wakeVerts3[numVerts + 1].pos.y - _wakeVerts3[numVerts - 1].pos.y,
                               _wakeVerts3[numVerts + 1].pos.z - _wakeVerts3[numVerts - 1].pos.z);
                if (d.y * vdelta.y + d.z * vdelta.z + d.x * vdelta.x < 0.0f)
                    _wakeVerts3[numVerts + 1].pos = _wakeVerts3[numVerts - 1].pos;
                vdelta = Vector3(_wakeVerts0[numVerts].pos.x - _wakeVerts0[numVerts - 2].pos.x,
                                 _wakeVerts0[numVerts].pos.y - _wakeVerts0[numVerts - 2].pos.y,
                                 _wakeVerts0[numVerts].pos.z - _wakeVerts0[numVerts - 2].pos.z);
                if (d.y * vdelta.y + d.z * vdelta.z + d.x * vdelta.x < 0.0f)
                    _wakeVerts0[numVerts].pos = _wakeVerts0[numVerts - 2].pos;
            }

            _wakeVerts0[numVerts + 1].pos = _wakeVerts1[numVerts].pos = _wakeVerts0[numVerts].pos * 0.75f + _wakeVerts3[numVerts + 1].pos * 0.25f;
            _wakeVerts0[numVerts + 1].texcoord.u = _wakeVerts1[numVerts].texcoord.u = 0.25f;
            _wakeVerts1[numVerts + 1].pos = _wakeVerts2[numVerts].pos = _wakeVerts0[numVerts].pos * 0.5f + _wakeVerts3[numVerts + 1].pos * 0.5f;
            _wakeVerts1[numVerts + 1].texcoord.u = _wakeVerts2[numVerts].texcoord.u = 0.5f;
            _wakeVerts2[numVerts + 1].pos = _wakeVerts3[numVerts].pos = _wakeVerts0[numVerts].pos * 0.25f + _wakeVerts3[numVerts + 1].pos * 0.75f;
            _wakeVerts2[numVerts + 1].texcoord.u = _wakeVerts3[numVerts].texcoord.u = 0.75f;

            numVerts += 2;
            prevSegment = &segment;
            segment._age += elapsed;
        }
    }
    _numVerts = numVerts;
}
#endif

// FUNCTION: USA 0x0002FF24
// PROTOTYPE: PREVIEW 0x0041E674 identical
// NAME: proto-pdb:default.pdb
void FanBoatWake::Render(const Matrix& m) const
{
    if (_numVerts < 4)
        return;
    SwampRenderer& r = g_swamp->GetRenderer();
    r.SetTexture(0, _wakeTexture);
    D3DDevice_SetRenderState_Simple(0x40304, 1);
    D3DDevice_SetRenderState_Simple(0x40344, 0x302);
    D3DDevice_SetRenderState_Simple(0x40348, 0x303);
    D3DDevice_SetRenderState_Simple(0x4035C, 0);
    r.SetVertexShader(g_swamp->GetVertexShaderManager().Load("BoatWake"), &_vaf);
    r.SetPixelShader(g_swamp->GetPixelShaderManager().Load("BoatWake"));
    D3DDevice_DrawVerticesUP(6, _numVerts, _wakeVerts0, sizeof(WakeVertex));
    D3DDevice_DrawVerticesUP(6, _numVerts, _wakeVerts1, sizeof(WakeVertex));
    D3DDevice_DrawVerticesUP(6, _numVerts, _wakeVerts2, sizeof(WakeVertex));
    D3DDevice_DrawVerticesUP(6, _numVerts, _wakeVerts3, sizeof(WakeVertex));
    r.SetEmptyTexture(0);
    D3DDevice_SetRenderState_Simple(0x4035C, 1);
}

// FUNCTION: USA 0x000301FA
// PROTOTYPE: PREVIEW 0x0041E941 identical
// NAME: proto-pdb:default.pdb
void VinceEnterBoat::AnimationFinished()
{
    InvalidateScript();
    Critter* boat = pManager->Find("fanboat");
    boat->GetScript()->Message(this, MESSAGE_VINCEONBOARD, 0);
    SoundManager::PlaySound("fanbtstart", 0x20, boat, true, 0.0f, 0.0f);
    ((GatorNode*)_node)->_posBone = _oldBone;
    ((GatorNode*)_node)->_shadowBone = _oldShadowBone;
    static_cast<VinceCam*>(g_CameraManager->Find("vincecam"))->ClearPositionFixed();
    boat->CheckRegions();
}

// FUNCTION: USA 0x000303EA
// PROTOTYPE: PREVIEW 0x0041EB31 ambiguous
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2); wall-vtable-relink (C0203): the vptr store's
// retail vtable (Ghidra PTR_??_GUIClusterMain@@UAEPAXI@Z_*, named for its first slot) and the "noscript" literal (s_noscript_001c9b1c)
// have no symbols.txt/symbols_pin.txt name
FanBoatExitArea::FanBoatExitArea(SwampSceneNode* node) : SwampScript(node)
{
    _qend.Set(0.0f, 0.0f, 1.0f);
    _velend.Set(0.0f, 0.0f, 1.0f);
    _age = 0.0f;
    _qstart = node->GetRotation();
    _velstart = ((Critter*)node)->_Velocity;
    _velstart.y = 0.0f;
    RegisterFrameTick();
    pManager->GetVince()->SetScript("noscript");
}
#endif

// FUNCTION: USA 0x00030491
// PROTOTYPE: PREVIEW 0x0041EBD8 identical
// NAME: proto-pdb:default.pdb
template void std::fill<std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &);

// FUNCTION: USA 0x000304B2
// PROTOTYPE: PREVIEW 0x0041EBF9 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> > * std::_Copy_backward_opt<std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > *>(std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::_Nonscalar_ptr_iterator_tag);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1FanBoatWake@@UAE@XZ (no vptr store, so no
// declared dtor) in this unit and no matched function odr-uses it; this qualified call odr-uses it (matched_set
// [[emitter]] row)
void pin_emit_FanBoatWake_dtor(FanBoatWake* p)
{
    p->FanBoatWake::~FanBoatWake();
}

// FUNCTION: USA 0x000309CC
// PROTOTYPE: PREVIEW 0x0041F113 identical
// NAME: proto-pdb:default.pdb
// VC 7.1 <vector> body verbatim as an explicit specialization: the header's _Xlen cannot be redeclared noreturn
// (C2381), so `__assume(0)` restates its throw body's effect (C0237, !FAKE) and no throw-info is emitted
template <> bool std::vector<SwampString,std::allocator<SwampString> >::_Buy(size_type _Capacity)
{
    _Myfirst = 0, _Mylast = 0, _Myend = 0;
    if (_Capacity == 0)
        return (false);
    else if (max_size() < _Capacity) {
        _Xlen();
        __assume(0); // !FAKE: c2 noreturn; __assume(0) restating noreturn; gen line with decl-only _Xlen reloads eax after the call (match_one 89.29 -> 100.0)
    } else {
        _Myfirst = this->_Alval.allocate(_Capacity);
        _Mylast = _Myfirst;
        _Myend = _Myfirst + _Capacity;
    }
    return (true);
}
// !FAKE: c1xx front end; explicit instantiation of the explicit specialization; the real header defines _Buy in-class, so cl 13.10 keeps the specialization inline and emits nothing unreferenced (match_one rc 2 "not defined" -> 100.0)
template bool std::vector<SwampString,std::allocator<SwampString> >::_Buy(unsigned int);

// FUNCTION: USA 0x00030BB8
// PROTOTYPE: PREVIEW 0x0041F2FF identical
// NAME: proto-pdb:default.pdb
template void std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::push_front(FanBoatWake::WakeSegment const &);

// FUNCTION: USA 0x00030A12
// PROTOTYPE: PREVIEW 0x0041F159 identical
// NAME: proto-pdb:default.pdb
// VC 7.1 <deque> body verbatim as an explicit specialization: the header's inline _Xlen throw emits length_error's
// ctor and the basic_string copy ctor body (precheck `our ranges overlap at 0x00011753`); with _Xlen decl-only (it
// cannot be redeclared noreturn, C2381) `__assume(0)` restates its throw body's effect (C0237, !FAKE)
template <> void std::deque<FanBoatWake::WakeSegment,std::allocator<FanBoatWake::WakeSegment> >::_Growmap(size_type _Count)
{	// grow map by _Count pointers
    if (max_size() / _DEQUESIZ - _Mapsize < _Count) {
        _Xlen();	// result too long
        __assume(0); // !FAKE: c2 noreturn; __assume(0) restating noreturn; decl-only _Xlen reloads after the call (match_one 90.80 -> 100.0)
    }

    size_type _Inc = _Mapsize / 2;	// try to grow by 50%
    if (_Inc < _DEQUEMAPSIZ)
        _Inc = _DEQUEMAPSIZ;
    if (_Count < _Inc && _Mapsize <= max_size() / _DEQUESIZ - _Inc)
        _Count = _Inc;
    size_type _Myboff = _Myoff / _DEQUESIZ;
    _Mapptr _Newmap = this->_Almap.allocate(_Mapsize + _Count);
    _Mapptr _Myptr = _Newmap + _Myboff;

    _Myptr = _Uninitialized_copy(_Map + _Myboff,
        _Map + _Mapsize, _Myptr, this->_Almap);	// copy initial to end
    if (_Myboff <= _Count) {	// increment greater than offset of initial block
        _Myptr = _Uninitialized_copy(_Map,
            _Map + _Myboff, _Myptr, this->_Almap);	// copy rest of old
        _Uninitialized_fill_n(_Myptr, _Count - _Myboff,
            (_Tptr)0, this->_Almap);	// clear suffix of new
        _Uninitialized_fill_n(_Newmap, _Myboff,
            (_Tptr)0, this->_Almap);	// clear prefix of new
    } else {	// increment not greater than offset of initial block
        _Uninitialized_copy(_Map,
            _Map + _Count, _Myptr, this->_Almap);	// copy more old
        _Myptr = _Uninitialized_copy(_Map + _Count,
            _Map + _Myboff, _Newmap, this->_Almap);	// copy rest of old
        _Uninitialized_fill_n(_Myptr, _Count,
            (_Tptr)0, this->_Almap);	// clear rest to initial block
    }

    _Destroy_range(_Map + _Myboff, _Map + _Mapsize, this->_Almap);
    if (_Map)
        this->_Almap.deallocate(_Map, _Mapsize);	// free storage for old

    _Map = _Newmap;	// point at new
    _Mapsize += _Count;
}

// FUNCTION: USA 0x00032622
// PROTOTYPE: PREVIEW 0x00420DAC identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; wall-relink: the inline out_of_range throw emits logic_error's ctor
// COMDAT, which instantiates the basic_string copy ctor body (precheck `our ranges overlap at 0x00011753`); a decl-only
// copy ctor is C2908 (<stdexcept> instantiates it first); it also emits ??_G_Node 0x00030F6F (the _Erase wall below)
template std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::iterator std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::erase(std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::iterator);
#endif

// FUNCTION: USA 0x00030C0A
// PROTOTYPE: PREVIEW 0x0041F351 ambiguous
// NAME: proto-pdb:default.pdb
template void std::_Uninitialized_fill_n<std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,unsigned int,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >(std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,unsigned int,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > &);

// FUNCTION: USA 0x00030C3F
// PROTOTYPE: PREVIEW 0x0041F386 identical
// NAME: proto-pdb:default.pdb
void FanBoatWake::AddSegment(Vector3 pos, Vector3 delta)
{
    WakeSegment segment;
    float seglength = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
    seglength *= FastReciprocalSquareRoot(seglength);
    segment._age = 0.0f;
    segment._center = pos;
    // !FAKE: provenance; written-out expression; cross product of up (0,1,0) with delta per the retail x87 sequence, the
    // prototype PDB names no cross helper (D78); no before/after recorded
    segment._side = Vector3(delta.z - delta.y * 0.0f, delta.x * 0.0f - delta.z * 0.0f, delta.y * 0.0f - delta.x);
    segment._side.Normalize();
    segment._texOffset = _texOffsetNext;
    _texOffsetNext -= seglength;
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    // !FAKE: c2 x87 register allocation; neutral local; retail keeps the attenuation in st0 and stores _atten once
    // after the clamp (match_one 94.78 with stores to segment._atten -> 100.0)
    float tmp = 1.0f;
    if (elapsed > 0.0f)
        tmp = seglength / elapsed * 0.06f;
    tmp = tmp * _age * 2.0f;
    if (tmp > 1.0f)
        tmp = 1.0f;
    segment._atten = tmp;
    _wakeSegments.push_front(segment);
}

// FUNCTION: USA 0x0003289B
// PROTOTYPE: PREVIEW 0x00421025 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; also emits the compiler-generated ??_G_Node row 0x00030F6F (100.0,
// guarded with it, no other source form); wall-relink: the ??_G's call to
// vector<ResourceHandle<SoundResource> >::_Tidy (fold VA 0x000C7B97) binds to walksounds.obj's unplaced COMDAT copy
// (precheck .text vsize 0x14517B vs XBE 0x14515C); lift = WalkSounds.cpp declares that _Tidy without a body
template void std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Erase(std::_Tree_nod<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Node *);
#endif

// FUNCTION: USA 0x000328D6
// PROTOTYPE: PREVIEW 0x00421060 ambiguous
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::clear();

// FUNCTION: USA 0x0002FB8E
// PROTOTYPE: PREVIEW 0x0041D650 identical
// NAME: proto-pdb:default.pdb
SwampScript* Activator_FanBoatMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\FanBoat.h", 249) Activator_FanBoatMain(node);
}

// FUNCTION: USA 0x0002FBB8
// PROTOTYPE: PREVIEW 0x0041D67A ambiguous
// NAME: proto-pdb:default.pdb
bool Activator_FanBoatMain::TestActivate() const
{
    Critter* boat = pManager->Find("fanboat");
    // !FAKE: c2 register allocation; pointer local (C0191); retail loads _node into ebx before the boat's call
    // (match_one 95.56 without -> 100.0)
    SwampSceneNode* node = _node;
    const Vector3* b = &boat->GetWorldPosition();
    const Vector3* n = &node->GetWorldPosition();
    // !FAKE: c1xx front end; hand form of the inlined PDB Vector3::Length (D117) on the difference, the PDB records no
    // free operator- (C0365); v1 is a stand-in local, retail copies _radius to a frame slot before the call (96.99
    // without); `if ... return false` gives retail's branch layout (93.19 as one return expression)
    Vector3 d(n->x - b->x, n->y - b->y, n->z - b->z);
    float v1 = boat->_radius;
    float tmp = d.x * d.x + d.y * d.y + d.z * d.z;
    if (tmp * FastReciprocalSquareRoot(tmp) > v1 * 4.0f)
        return false;
    return true;
}

// FUNCTION: USA 0x00030561
// PROTOTYPE: PREVIEW 0x0041ECA8 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceEnterBoat::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\FanBoat.h", 206) VinceEnterBoat(node);
}

// FUNCTION: USA 0x00030588
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); gate pin_rows: ctor pin 0x000302CF is site-corr, relocated through by this row (D102); lift = a disasm-/corr-verified ctor pin (5.3 T5)
SwampScript* VinceExitBoat::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\FanBoat.h", 226) VinceExitBoat(node);
}
#endif

// FUNCTION: USA 0x00032F07
// PROTOTYPE: PREVIEW 0x0042168D identical
// NAME: proto-pdb:default.pdb
SwampScript* FanBoatMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\FanBoat.h", 37) FanBoatMain(node);
}
