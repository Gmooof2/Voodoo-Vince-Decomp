// UNIT: src/Apps/Vince/core/VinceCam.cpp OBJ: core\XBox\OfficialRelease\VinceCam.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AABox;      // NAME: proto-pdb:default.pdb
struct Quaternion { // NAME: proto-pdb:default.pdb
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

    const Vector3& SetLocation(const Vector3&);
    Vector3 GetFacing() const;
    const Quaternion& SetFacing(const Vector3&);

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetFOV(float);
    float GetFOV() const { return _fov; }

    float _fov;                        // 0xA4
    char field_A8[0x10C - 0xA8];       // 0xA8
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    char field_10C[0x11C - 0x10C];     // 0x10C
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void CopyCamera(GameCam* pcam);
    void TransitionTo(float radius, float elevation, float lookAhead, float delay);
    void SetPositionFixed(const Vector3& position, float delay);
    void SetBaseFOV(float fov, float delay);

    char field_11C[0x128 - 0x11C];     // 0x11C
    Vector3 _fixedDirection;           // 0x128
    bool _positionFixed;               // 0x134
    Vector3 _fixedPosition;            // 0x138
    char field_144[0x154 - 0x144];     // 0x144
    float _orbitRadius;                // 0x154
    char field_158[0x160 - 0x158];     // 0x158
    float _elevation;                  // 0x160
    char field_164[0x168 - 0x164];     // 0x164
    float _adjustOrbitRadius;          // 0x168
    float _adjustElevation;            // 0x16C
    float _adjustLookAhead;            // 0x170
    float _adjustDelay;                // 0x174
    float _adjustTimer;                // 0x178
    float _bumpedAlpha;                // 0x17C
    char field_180[0x1D0 - 0x180];     // 0x180
    float _lookAhead;                  // 0x1D0
    float _baseFOV;                    // 0x1D4
    float _oldFOV;                     // 0x1D8
    float _fovDelay;                   // 0x1DC
    float _fovTime;                    // 0x1E0
    char field_1E4[0x238 - 0x1E4];     // 0x1E4
};
DECOMP_SIZE_ASSERT(VinceCam, 0x238);

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

class CameraHelperRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CameraHelperRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_4C[0x9C - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CameraHelperRegionMain, 0x9C);

// FUNCTION: USA 0x0010BB8E
// PROTOTYPE: PREVIEW 0x004F6499 identical
// NAME: proto-pdb:default.pdb
SwampScript* CameraHelperRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\VinceCam.h", 212) CameraHelperRegionMain(node);
}
