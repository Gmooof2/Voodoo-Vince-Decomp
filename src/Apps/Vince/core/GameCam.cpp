// UNIT: src/Apps/Vince/core/GameCam.cpp OBJ: core\XBox\OfficialRelease\GameCam.obj OPT: /O1 /Oy /Oi
#include "../../../include/decomp.h"
#include <math.h>

struct Matrix; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    Vector3() {}
    Vector3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    Vector3& operator+=(const Vector3& a) { x += a.x; y += a.y; z += a.z; return *this; }
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
struct Quaternion; // NAME: proto-pdb:default.pdb
struct AABox; // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(double);
}

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
    char field_4[0x34 - 0x4];          // 0x04
    bool _pauseGame;                   // 0x34
    char field_35[0x120 - 0x35];       // 0x35
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x68 - 0x8];          // 0x08
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetFOV() const { return FOV; }

    char field_0[0x38];                // 0x00
    float FOV;                         // 0x38
    char field_3C[0xA0C - 0x3C];       // 0x3C
};
DECOMP_SIZE_ASSERT(Globals, 0xA0C);

extern Globals gSettings; // NAME: proto-pdb:default.pdb

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

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampCamera();
    virtual ~SwampCamera();
    virtual Vector3 GetViewPosition() const;                       // vtable+0x24
    virtual Matrix GetViewMatrix() const;                          // vtable+0x28
    virtual const Matrix& GetProjectionMatrix() const;             // vtable+0x2C
    void SetFOV(float);
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C

    char field_A4[0x10C - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    GameCam();
    virtual ~GameCam() {}
    virtual void Activate();                                       // vtable+0x30
    virtual void LosingControl();                                  // vtable+0x34
    void SetShakeFactor(float shake, float duration);
    virtual Vector3 GetViewPosition() const;                       // vtable+0x24

    float _shakeFactor;                // 0x10C
    float _shakeDuration;              // 0x110
    float _shakeTimer;                 // 0x114
    bool _isCurrentCamera;             // 0x118
    bool _isAnimated;                  // 0x119

protected:
    virtual void Update();                                         // vtable+0x1C
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

// FUNCTION: USA 0x0011269D
// PROTOTYPE: PREVIEW 0x004FCEF4 identical
// NAME: proto-pdb:default.pdb
void GameCam::Update()
{
    SwampCamera::Update();
    if (_shakeTimer > 0.0f && _shakeFactor > 0.0f) {
        _shakeTimer -= g_swamp->GetInput().GetElapsedTime();
        if (_shakeTimer < 0.0f) {
            _shakeTimer = 0.0f;
        }
    }
}

// FUNCTION: USA 0x001126F8
// PROTOTYPE: PREVIEW 0x004FCF4F identical
// NAME: proto-pdb:default.pdb
void GameCam::SetShakeFactor(float shake, float duration)
{
    _shakeFactor = shake;
    if (duration >= 0.0f) {
        _shakeDuration = duration;
    } else {
        _shakeDuration = fabs(shake) * 0.5f;
    }
    _shakeTimer = _shakeDuration;
}

// FUNCTION: USA 0x00112740
// PROTOTYPE: PREVIEW 0x004FCF97 identical
// NAME: proto-pdb:default.pdb
Vector3 GameCam::GetViewPosition() const
{
    Vector3 _baseloc = SwampSceneNode::GetWorldPosition();
    Vector3 newloc = _baseloc;
    if (!g_swamp->GetInput()._pauseGame && _shakeTimer > 0.0f) {
        float shake;
        if (_shakeFactor > 0.0f) {
            shake = _shakeTimer / _shakeDuration * _shakeFactor;
        } else {
            shake = _shakeFactor;
        }
        newloc += Vector3(BeepLib::RandomFloat(0.1f) * shake, BeepLib::RandomFloat(0.1f) * shake, BeepLib::RandomFloat(0.1f) * shake);
    }
    return newloc;
}

// FUNCTION: USA 0x00112824
// PROTOTYPE: PREVIEW 0x004FD07B identical
// NAME: proto-pdb:default.pdb
GameCam::GameCam() : _shakeFactor(0.0f), _shakeDuration(0.0f), _shakeTimer(0.0f), _isCurrentCamera(false), _isAnimated(false)
{
    SetFOV(gSettings.GetFOV());
}
