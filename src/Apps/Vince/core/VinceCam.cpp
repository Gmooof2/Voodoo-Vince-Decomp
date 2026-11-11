// UNIT: src/Apps/Vince/core/VinceCam.cpp OBJ: core\XBox\OfficialRelease\VinceCam.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
#ifdef NON_MATCHING
extern "C" double __cdecl fabs(double);
// !FAKE: c1xx front end; file-scope #pragma intrinsic before the overload body; the unit has no /Oi, retail inlines fabs (match_one 87.38 -> 95.92)
#pragma intrinsic(fabs)
#endif
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, npos); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    int compare(const E* _Ptr) const;
    const E* c_str() const { return _Myptr(); }
    unsigned int size() const { return _Mysize; }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);
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
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
template <class E, class T, class A>
inline bool operator!=(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return !(_Left == _Right);
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
    int GetIntRecord(const SwampString& id, int def) const;
    double GetFloatRecord(const SwampString& id, double def) const;
};
extern "C" int __cdecl _stricmp(const char*, const char*); // NAME: proto-pdb:default.pdb
extern "C" int __cdecl sscanf(const char*, const char*, ...); // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08

    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    void Normalize();
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
Vector3 Normalize(const Vector3&); // NAME: proto-pdb:default.pdb
float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

struct AABox;      // NAME: proto-pdb:default.pdb
struct Matrix;     // NAME: proto-pdb:default.pdb
struct Quaternion { // NAME: proto-pdb:default.pdb
    void Set(float, float, float);
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
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

    const Vector3& SetLocation(const Vector3&);
    Vector3 GetFacing() const;
    const Quaternion& SetFacing(const Vector3&);
    const SwampString& GetName() const { return _Name; }
    const Quaternion& GetRotation() const { return _Rotation; }

    char field_4[0x8 - 0x4];           // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x40 - 0x24];        // 0x24
    Quaternion _Rotation;              // 0x40
    char field_50[0xA4 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampCamera();
    virtual Vector3 GetViewPosition() const;                       // vtable+0x24
    virtual Matrix GetViewMatrix() const;                          // vtable+0x28
    virtual const Matrix& GetProjectionMatrix() const;             // vtable+0x2C
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    void SetFOV(float);
    float GetFOV() const { return _fov; }
    float GetNearPlane() const { return _nearPlane; }

    float _fov;                        // 0xA4
    char field_A8[0xAC - 0xA8];        // 0xA8
    float _nearPlane;                  // 0xAC
    char field_B0[0x10C - 0xB0];       // 0xB0
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    virtual ~GameCam() {}
    virtual void Activate();                                       // vtable+0x30
    virtual void LosingControl();                                  // vtable+0x34
    virtual Vector3 GetViewPosition() const;                       // vtable+0x24
    char field_10C[0x11C - 0x10C];     // 0x10C

protected:
    virtual void Update();                                         // vtable+0x1C
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    ~VinceCam();
    void CopyCamera(GameCam* pcam);
    void SetDefaultVals();
    void SetSpeed(float speed) { _speed = speed; }
    float GetSpeed() const { return _speed; }
    float GetOrbitRadius() const { return _orbitRadius; }
    float GetElevation() const { return _elevation; }
    float GetLookAhead() const { return _lookAhead; }
    void SetDirectionFixed(const Vector3& direction, float delay);
    void ResetLocation(bool behind);
    static float GetDefaultOrbitRadius();
    static float GetDefaultElevation();
    static float GetDefaultLookAhead();
    void TransitionTo(float radius, float elevation, float lookAhead, float delay);
    void TransitionToDefault(float delay);
    void ClearDirectionFixed() { _directionFixed = false; }
    void SetPositionFixed(const Vector3& position, float delay);
    void ClearPositionFixed();
    void EnableControl(bool enable) { _controlEnabled = enable; }
    void SetBaseFOV(float fov, float delay);
    void SetCurrentHelperRegion(const SwampSceneNode* region) { _helperRegion = region; }

    float _speed;                      // 0x11C
    bool _controlEnabled;              // 0x120
    char field_121[0x124 - 0x121];     // 0x121
    bool _directionFixed;              // 0x124
    Vector3 _fixedDirection;           // 0x128
    bool _positionFixed;               // 0x134
    Vector3 _fixedPosition;            // 0x138
    char field_144[0x14C - 0x144];     // 0x144
    float _fixedTimer;                 // 0x14C
    char field_150[0x154 - 0x150];     // 0x150
    float _orbitRadius;                // 0x154
    float _minOrbit;                   // 0x158
    char field_15C[0x160 - 0x15C];     // 0x15C
    float _elevation;                  // 0x160
    char field_164[0x168 - 0x164];     // 0x164
    float _adjustOrbitRadius;          // 0x168
    float _adjustElevation;            // 0x16C
    float _adjustLookAhead;            // 0x170
    float _adjustDelay;                // 0x174
    float _adjustTimer;                // 0x178
    float _bumpedAlpha;                // 0x17C
    char field_180[0x1A0 - 0x180];     // 0x180
    float _hspeedFactor;               // 0x1A0
    float _vspeedFactor;               // 0x1A4
    float _turnspeedFactor;            // 0x1A8
    float _hmoveMax;                   // 0x1AC
    float _vmoveMax;                   // 0x1B0
    float _turnMax;                    // 0x1B4
    float _collisionRadius;            // 0x1B8
    char field_1BC[0x1C0 - 0x1BC];     // 0x1BC
    float _moveBehindTimer;            // 0x1C0
    char field_1C4[0x1D0 - 0x1C4];     // 0x1C4
    float _lookAhead;                  // 0x1D0
    float _baseFOV;                    // 0x1D4
    float _oldFOV;                     // 0x1D8
    float _fovDelay;                   // 0x1DC
    float _fovTime;                    // 0x1E0
    bool _peeking;                     // 0x1E4
    char field_1E5[0x1F4 - 0x1E5];     // 0x1E5
    bool _wasInTunnel;                 // 0x1F4
    char field_1F5[0x204 - 0x1F5];     // 0x1F5
    float _lostTimer;                  // 0x204
    char field_208[0x20C - 0x208];     // 0x208
    float _vinceFloatTimer;            // 0x20C
    bool _holdTarget;                  // 0x210
    char field_211[0x214 - 0x211];     // 0x211
    Vector3 _heldTargetPos;            // 0x214
    const SwampSceneNode* _helperRegion; // 0x220
    float _rampUp;                     // 0x224
    char field_228[0x238 - 0x228];     // 0x228
};
DECOMP_SIZE_ASSERT(VinceCam, 0x238);

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetFOV() const { return FOV; }
    float GetCamMinDist() const { return _camMinDist; }
    float GetCamElevation() const { return _camElevation; }
    float GetCamOrbitRadius() const { return _camOrbitRadius; }
    float GetCamLookAhead() const { return _camLookAhead; }

    char field_0[0x38];                // 0x00
    float FOV;                         // 0x38
    char field_3C[0x1D0 - 0x3C];       // 0x3C
    float _camMinDist;                 // 0x1D0
    char field_1D4[0x1D8 - 0x1D4];     // 0x1D4
    float _camLookAhead;               // 0x1D8
    float _camElevation;               // 0x1DC
    float _camOrbitRadius;             // 0x1E0
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

inline float VinceCam::GetDefaultOrbitRadius() { return gSettings.GetCamOrbitRadius(); }
inline float VinceCam::GetDefaultElevation() { return gSettings.GetCamElevation(); }
inline float VinceCam::GetDefaultLookAhead() { return gSettings.GetCamLookAhead(); }

// FUNCTION: USA 0x0010B311
// PROTOTYPE: PREVIEW 0x004F5C27 identical
// NAME: proto-pdb:default.pdb
void VinceCam::TransitionTo(float radius, float elevation, float lookAhead, float delay)
{
    if (delay != 0.0f) {
        _adjustOrbitRadius = _orbitRadius;
        _adjustTimer = _adjustDelay = delay;
        _adjustElevation = _elevation;
        _adjustLookAhead = _lookAhead;
    }
    _orbitRadius = radius;
    _bumpedAlpha = 0.0f;
    _elevation = elevation;
    _lookAhead = lookAhead;
}

// FUNCTION: USA 0x0010B383
// PROTOTYPE: PREVIEW 0x004F5C99 identical
// NAME: proto-pdb:default.pdb
void VinceCam::SetPositionFixed(const Vector3& position, float delay)
{
    _fixedPosition = position;
    _bumpedAlpha = 0.0f;
    _positionFixed = true;
    if (delay != 0.0f) {
        _adjustDelay = _adjustTimer = delay;
        _fixedDirection = GetWorldPosition();
    } else {
        SetWorldPosition(position);
    }
}

// FUNCTION: USA 0x0010B3F2
// PROTOTYPE: PREVIEW 0x004F5D08 identical
// NAME: proto-pdb:default.pdb
void VinceCam::SetDefaultVals()
{
    _minOrbit = gSettings.GetCamMinDist();
    _lookAhead = gSettings.GetCamLookAhead();
    _orbitRadius = gSettings.GetCamOrbitRadius();
    _elevation = gSettings.GetCamElevation();
    _collisionRadius = GetNearPlane() * 8.0f;
    _baseFOV = gSettings.GetFOV();
    SetFOV(_baseFOV);
    _vspeedFactor = 6.0f;
    _vmoveMax = 4.0f;
    _hspeedFactor = 32.0f;
    _turnspeedFactor = 128.0f;
    _hmoveMax = 2.0f;
    _turnMax = 2.0f;
    _bumpedAlpha = 0.0f;
}

// FUNCTION: USA 0x0010B492
// PROTOTYPE: PREVIEW 0x004F5DA8 identical
// NAME: proto-pdb:default.pdb
void VinceCam::TransitionToDefault(float delay)
{
    if (delay != 0.0f) {
        _adjustOrbitRadius = _orbitRadius;
        _adjustTimer = _adjustDelay = delay;
        _adjustElevation = _elevation;
        _adjustLookAhead = _lookAhead;
    }
    _orbitRadius = gSettings.GetCamOrbitRadius();
    _elevation = gSettings.GetCamElevation();
    _lookAhead = gSettings.GetCamLookAhead();
    _adjustTimer = 0.0f;
    _vspeedFactor = 8.0f;
    _vmoveMax = 4.0f;
    _hspeedFactor = 32.0f;
    _turnspeedFactor = 128.0f;
    _hmoveMax = 2.0f;
    _turnMax = 2.0f;
    _bumpedAlpha = 0.0f;
}

// FUNCTION: USA 0x0010B549
// PROTOTYPE: PREVIEW 0x004F5E5F identical
// NAME: proto-pdb:default.pdb
void VinceCam::CopyCamera(GameCam* pcam)
{
    SetLocation(pcam->GetWorldPosition());
    SetFacing(pcam->GetFacing());
    SetFOV(pcam->GetFOV());
    _bumpedAlpha = 0.0f;
}

// FUNCTION: USA 0x0010B59B
// PROTOTYPE: PREVIEW 0x004F5EB1 identical
// NAME: proto-pdb:default.pdb
void VinceCam::SetBaseFOV(float fov, float delay)
{
    if (delay > 0.0f) {
        _oldFOV = GetFOV();
        _fovTime = _fovDelay = delay;
    } else {
        SetFOV(fov);
        _fovTime = 0.0f;
        _fovDelay = 0.0f;
    }
    _baseFOV = fov;
}

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};
class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

#ifdef NON_MATCHING
// math.h fabs(float) overload for the ResetLocation draft below (inline fabs needs /Oi, C0221)
inline float fabs(float _X)
{
    return (float)fabs((double)_X);
}
#endif

// FUNCTION: USA 0x0010B763
// PROTOTYPE: PREVIEW 0x004F6079 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 96.08 at /O1 /Oy (T12.c5b, 20 attempts), bytes mismatch at +0xFC: frame 0x40 and every slot match retail; retail keeps the radius on x87 for the z product (fld; fmul st(1); fstp st(0)), ours folds fmul [dir.z]; then v1 add operand order and pop edi/fcom scheduling
// T12.c26 lever A (`const float& v` on v4 or _orbitRadius, member stores, z-product stand-in) neutral: 96.08, +0xFC
void VinceCam::ResetLocation(bool behind)
{
    Critter* vince = pManager->GetVince();
    Vector3 viewvect;
    // !FAKE: c2 stack packing; sibling blocks; retail shares lookat/flatvect and dir/myloc slots (PDB), our cl shares named slots only across sibling blocks (frame 0x4C -> 0x40, match_one 95.92 -> 96.08)
    {
        Vector3 lookat = _holdTarget ? _heldTargetPos : vince->GetWorldPosition();
        // !FAKE: inliner; hand form of the inlined Vector3 operators (no PDB record); stand-ins v5, v4, v0, v1, p as pointers to temporaries carried into the sibling block (C0191)
        const Vector3* v5;
        const Vector3* v1;
        {
            Vector3 dir = _directionFixed ? _fixedDirection : vince->GetFacing();
            if (behind && !_directionFixed)
                dir = Vector3(-dir.x, -dir.y, -dir.z);
            _fixedTimer = 0.0f;
            _adjustTimer = 0.0f;
            _positionFixed = false;
            _peeking = false;
            _wasInTunnel = false;
            _moveBehindTimer = 0.0f;
            _bumpedAlpha = 0.0f;
            _lostTimer = 0.0f;
            _rampUp = 1.0f;
            _vinceFloatTimer = 0.0f;
            v5 = &Vector3(0.0f, _elevation, 0.0f);
            float v4 = _orbitRadius;
            Vector3 v0(dir.x * v4, dir.y * v4, dir.z * v4);
            v1 = &Vector3(lookat.x + v0.x, lookat.y + v0.y, lookat.z + v0.z);
        }
        {
            Vector3 myloc = Vector3(v1->x + v5->x, v1->y + v5->y, v1->z + v5->z);
            SetLocation(myloc);
            SetFOV(_baseFOV);
            viewvect.x = lookat.x - myloc.x;
            viewvect.y = lookat.y - myloc.y;
            viewvect.z = lookat.z - myloc.z;
        }
    }
    {
        Vector3 flatvect = viewvect;
        flatvect.y = 0.0f;
        float dist = flatvect.x * flatvect.x + flatvect.z * flatvect.z;
        dist = dist * FastReciprocalSquareRoot(dist);
        // !FAKE: c2 expression reassociation; stand-ins v3, v2; one expression sinks the 0.05f factor past the divide
        float v3 = (fabs(viewvect.y) > 0.001f) ? fabs(viewvect.y) : 0.001f;
        float v2 = _lookAhead * 0.05f;
        dist = v2 / v3 * dist * 0.75f;
        const Vector3* p = &Normalize(flatvect);
        viewvect += Vector3(dist * p->x, dist * p->y, dist * p->z);
    }
    viewvect.Normalize();
    SetFacing(viewvect);
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
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class CameraHelperRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Flags { // NAME: proto-pdb:default.pdb
        FIX_DIRECTION = 0x1,
        ELEVATION = 0x2,
        ORBIT_RADIUS = 0x4,
        LOOKAHEAD = 0x8,
        CONTROL_OFF = 0x10,
        FIX_LOCATION = 0x20,
        SPEED = 0x40,
        DELAY = 0x80
    };
    CameraHelperRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    unsigned int _flags;               // 0x4C
    Vector3 _fixDirection;             // 0x50
    SwampString _fixPosLocator;        // 0x5C
    float _elevation;                  // 0x78
    float _orbitRadius;                // 0x7C
    float _lookAhead;                  // 0x80
    float _speed;                      // 0x84
    float _delay;                      // 0x88
    float _restoreElevation;           // 0x8C
    float _restoreOrbitRadius;         // 0x90
    float _restoreLookAhead;           // 0x94
    float _restoreSpeed;               // 0x98
};
DECOMP_SIZE_ASSERT(CameraHelperRegionMain, 0x9C);

// FUNCTION: USA 0x0010BB5F
// NAME: proto-pdb:default.pdb
VinceCam::~VinceCam()
{
}

// FUNCTION: USA 0x0010BB6A
// NAME: proto-pdb:default.pdb
CameraHelperRegionMain::CameraHelperRegionMain(SwampSceneNode* node)
    : SwampScript(node), _flags(0)
{
}

// FUNCTION: USA 0x0010BB8E
// PROTOTYPE: PREVIEW 0x004F6499 identical
// NAME: proto-pdb:default.pdb
SwampScript* CameraHelperRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\VinceCam.h", 212) CameraHelperRegionMain(node);
}

// FUNCTION: USA 0x0010DF74
// PROTOTYPE: PREVIEW 0x004F88BA identical
// NAME: proto-pdb:default.pdb
void CameraHelperRegionMain::Load(const BeepTDF& tdf)
{
    _flags = 0;
    SwampString fixdir = tdf.GetStringRecord("camerafixdirection", "");
    if (fixdir.size() != 0) {
        if (!_stricmp(fixdir.c_str(), "x_axis")) {
            _flags |= FIX_DIRECTION;
            _fixDirection = Vector3(1.0f, 0.0f, 0.0f);
        } else if (!_stricmp(fixdir.c_str(), "z_axis")) {
            _flags |= FIX_DIRECTION;
            _fixDirection = Vector3(0.0f, 0.0f, 1.0f);
        } else if (!_stricmp(fixdir.c_str(), "neg_x_axis")) {
            _flags |= FIX_DIRECTION;
            _fixDirection = Vector3(-1.0f, 0.0f, 0.0f);
        } else if (!_stricmp(fixdir.c_str(), "neg_z_axis")) {
            _flags |= FIX_DIRECTION;
            _fixDirection = Vector3(0.0f, 0.0f, -1.0f);
        } else if (!_stricmp(fixdir.c_str(), "local_x_axis")) {
            _flags |= FIX_DIRECTION;
            // !FAKE: inliner; dummy local; hand form of the PDB by-value Quaternion::Apply(v), which stays out of line at /O1 (C0225; 98.76 -> 99.56)
            Vector3 tmp;
            _node->GetRotation().Apply(Vector3(1.0f, 0.0f, 0.0f), tmp);
            _fixDirection = tmp;
        } else if (!_stricmp(fixdir.c_str(), "local_y_axis")) {
            _flags |= FIX_DIRECTION;
            // !FAKE: inliner; dummy local; hand form of the PDB by-value Quaternion::Apply(v), which stays out of line at /O1 (C0225; 98.76 -> 99.56)
            Vector3 tmp;
            _node->GetRotation().Apply(Vector3(0.0f, 1.0f, 0.0f), tmp);
            _fixDirection = tmp;
        } else if (!_stricmp(fixdir.c_str(), "local_z_axis")) {
            _flags |= FIX_DIRECTION;
            // !FAKE: inliner; dummy local; hand form of the PDB by-value Quaternion::Apply(v), which stays out of line at /O1 (C0225; 98.76 -> 99.56)
            Vector3 tmp;
            _node->GetRotation().Apply(Vector3(0.0f, 0.0f, 1.0f), tmp);
            _fixDirection = tmp;
        }
        if (!_stricmp(fixdir.c_str(), "neg_local_x_axis")) {
            _flags |= FIX_DIRECTION;
            // !FAKE: inliner; dummy local; hand form of the PDB by-value Quaternion::Apply(v), which stays out of line at /O1 (C0225; 98.76 -> 99.56)
            Vector3 tmp;
            _node->GetRotation().Apply(Vector3(-1.0f, 0.0f, 0.0f), tmp);
            _fixDirection = tmp;
        } else if (!_stricmp(fixdir.c_str(), "neg_local_y_axis")) {
            _flags |= FIX_DIRECTION;
            // !FAKE: inliner; dummy local; hand form of the PDB by-value Quaternion::Apply(v), which stays out of line at /O1 (C0225; 98.76 -> 99.56)
            Vector3 tmp;
            _node->GetRotation().Apply(Vector3(0.0f, -1.0f, 0.0f), tmp);
            _fixDirection = tmp;
        } else if (!_stricmp(fixdir.c_str(), "neg_local_z_axis")) {
            _flags |= FIX_DIRECTION;
            // !FAKE: inliner; dummy local; hand form of the PDB by-value Quaternion::Apply(v), which stays out of line at /O1 (C0225; 98.76 -> 99.56)
            Vector3 tmp;
            _node->GetRotation().Apply(Vector3(0.0f, 0.0f, -1.0f), tmp);
            _fixDirection = tmp;
        } else {
            _flags |= FIX_DIRECTION;
            sscanf(fixdir.c_str(), "%f %f %f", &_fixDirection.x, &_fixDirection.y, &_fixDirection.z);
        }
    }
    // !FAKE: c1xx front end; cast; retail multiplies by a float literal (fmul dword), the double product gives fmul qword (match_one 99.557 -> 99.568)
    // !FAKE: c2 instruction scheduling; cast; the outer redundant (float) puts fstp fixangle above the temp dtor's lea ecx (match_one 99.568 -> 100.0)
    float fixangle = (float)((float)tdf.GetFloatRecord("camerafixangle", -1.0) * 0.0174532925f);
    if (fixangle >= 0.0f) {
        _flags |= FIX_DIRECTION;
        Quaternion q;
        q.Set(0.0f, -fixangle, 0.0f);
        // !FAKE: inliner; dummy local; hand form of the PDB by-value Quaternion::Apply(v), which stays out of line at /O1 (C0225; 98.76 -> 99.56)
        Vector3 tmp;
        q.Apply(Vector3(0.0f, 0.0f, 1.0f), tmp);
        _fixDirection = tmp;
    }
    float elev = tdf.GetFloatRecord("cameraelevation", -1.0);
    if (elev >= 0.0f) {
        _flags |= ELEVATION;
        _elevation = elev;
    }
    float orbit = tdf.GetFloatRecord("cameraorbitradius", -1.0);
    if (orbit >= 0.0f) {
        _flags |= ORBIT_RADIUS;
        _orbitRadius = orbit;
    }
    float lookAhead = tdf.GetFloatRecord("cameralookahead", -1.0);
    if (lookAhead >= 0.0f) {
        _flags |= LOOKAHEAD;
        _lookAhead = lookAhead;
    }
    if (tdf.GetIntRecord("cameracontroloff", 0))
        _flags |= CONTROL_OFF;
    _fixPosLocator = tdf.GetStringRecord("cameraposfixed", "");
    if (_fixPosLocator.size() != 0)
        _flags |= FIX_LOCATION;
    _speed = tdf.GetFloatRecord("cameraspeed", 1.0);
    _flags |= SPEED;
    _delay = tdf.GetFloatRecord("cameradelay", 1.0);
    _flags |= DELAY;
}

// FUNCTION: USA 0x0010E48D
// PROTOTYPE: PREVIEW 0x004F8DD3 identical
// NAME: proto-pdb:default.pdb
SwampString CameraHelperRegionMain::Save()
{
    SwampString ret = SwampScript::Save();
    if (_flags & FIX_DIRECTION)
        ret += format("\t\tcamerafixdirection = %f %f %f;\n", _fixDirection.x, _fixDirection.y, _fixDirection.z);
    if (_flags & ELEVATION)
        ret += format("\t\tcameraelevation = %f;\n", _elevation);
    if (_flags & ORBIT_RADIUS)
        ret += format("\t\tcameraorbitradius = %f;\n", _orbitRadius);
    if (_flags & LOOKAHEAD)
        ret += format("\t\tcameralookahead = %f;\n", _lookAhead);
    if (_flags & CONTROL_OFF)
        ret += SwampString("\t\tcameracontroloff = 1;\n");
    if (_flags & FIX_LOCATION)
        ret += format("\t\tcameraposfixed = %s;\n", _fixPosLocator.c_str());
    if (_flags & SPEED)
        ret += format("\t\tcameraspeed = %f;\n", _speed);
    if (_flags & DELAY)
        ret += format("\t\tcameradelay = %f;\n", _delay);
    return ret;
}

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

// FUNCTION: USA 0x0010E680
// PROTOTYPE: PREVIEW 0x004F8FC6 identical
// NAME: proto-pdb:default.pdb
void CameraHelperRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (node->GetName() == "vince") {
        // !FAKE: c1xx front end; stand-in local; the PDB records no local for the found camera (edi in retail)
        VinceCam* p = (VinceCam*)g_CameraManager->Find("vincecam");
        _restoreElevation = p->GetElevation();
        _restoreOrbitRadius = p->GetOrbitRadius();
        _restoreLookAhead = p->GetLookAhead();
        _restoreSpeed = p->GetSpeed();
        p->SetCurrentHelperRegion(_node);
        if (_flags & FIX_DIRECTION)
            p->SetDirectionFixed(_fixDirection, _speed);
        if (_flags & (ELEVATION | ORBIT_RADIUS | LOOKAHEAD)) {
            // !FAKE: c2 block layout; stand-in locals; retail evaluates elevation, radius, lookahead in this order (fld st2 copy)
            float v2 = (_flags & ELEVATION) ? _elevation : VinceCam::GetDefaultElevation();
            float v3 = (_flags & ORBIT_RADIUS) ? _orbitRadius : VinceCam::GetDefaultOrbitRadius();
            float v4 = (_flags & LOOKAHEAD) ? _lookAhead : VinceCam::GetDefaultLookAhead();
            p->TransitionTo(v3, v2, v4, _delay);
        }
        if (_flags & CONTROL_OFF)
            p->EnableControl(false);
        if (_flags & SPEED)
            p->SetSpeed(_speed);
        if (_flags & FIX_LOCATION) {
            // !FAKE: c1xx front end; stand-in local; the PDB records no local for the found locator (eax in retail)
            Locator* v1 = g_locatorManager->Find(_fixPosLocator);
            if (v1)
                p->SetPositionFixed(v1->GetLocation(), _delay);
        }
    }
}

// FUNCTION: USA 0x0010E7CA
// PROTOTYPE: PREVIEW 0x004F9110 identical
// NAME: proto-pdb:default.pdb
void CameraHelperRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (node->GetName() == "vince") {
        // !FAKE: c1xx front end; stand-in local; the PDB records no local for the found camera (edi in retail)
        VinceCam* p = (VinceCam*)g_CameraManager->Find("vincecam");
        p->SetCurrentHelperRegion(0);
        if (_flags & FIX_DIRECTION)
            p->ClearDirectionFixed();
        if (_flags & (ELEVATION | ORBIT_RADIUS | LOOKAHEAD))
            p->TransitionTo(_restoreOrbitRadius, _restoreElevation, _restoreLookAhead, _delay);
        if (_flags & CONTROL_OFF)
            p->EnableControl(true);
        if (_flags & SPEED)
            p->SetSpeed(_restoreSpeed);
        if (_flags & FIX_LOCATION) {
            if (g_locatorManager->Find(_fixPosLocator))
                p->ClearPositionFixed();
        }
    }
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};


// FUNCTION: USA 0x0010E8A6
// NAME: proto-pdb:default.pdb
CameraHelperRegionMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("CameraHelperRegionMain", creationFunc);
}
