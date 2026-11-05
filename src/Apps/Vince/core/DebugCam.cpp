// UNIT: src/Apps/Vince/core/DebugCam.cpp OBJ: core\XBox\OfficialRelease\DebugCam.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string { // NAME: proto-pdb:default.pdb
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

    char field_0[0x1C];                // 0x00
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
DECOMP_SIZE_ASSERT(SwampString, 0x1C);

enum { // NAME: proto-pdb:default.pdb
    BUTTON_UP = 0,
    BUTTON_DOWN = 1,
    BUTTON_LEFT = 2,
    BUTTON_RIGHT = 3,
    BUTTON_START = 4,
    BUTTON_BACK = 5,
    BUTTON_LEFT_THUMB = 6,
    BUTTON_RIGHT_THUMB = 7,
    BUTTON_A = 8,
    BUTTON_B = 9,
    BUTTON_X = 10,
    BUTTON_Y = 11,
    BUTTON_BLACK = 12,
    BUTTON_WHITE = 13,
    BUTTON_LEFT_TRIGGER = 14,
    BUTTON_RIGHT_TRIGGER = 15
};

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
    Vector2() {}
    Vector2(float u_, float v_) { u = u_; v = v_; }
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    Vector3() {}
    Vector3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    Vector3& operator+=(const Vector3& a) { x += a.x; y += a.y; z += a.z; return *this; }
    Vector3& operator*=(float f) { x *= f; y *= f; z *= f; return *this; }
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
    Quaternion() {}
    void Set(const Vector3&, float);
    void Apply(const Vector3&, Vector3&) const;
    Quaternion& operator*=(const Quaternion&);
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
    Vector2 GetRightStick() const;
    unsigned char GetButtonState(int b) const
    {
        if (b < 8)
            return ~((_current.wButtons >> b & 1) - 1);
        return _current.bAnalogButtons[b - 8];
    }
    unsigned char GetButtonPreviousState(int b) const
    {
        if (b < 8)
            return ~((_previous.wButtons >> b & 1) - 1);
        return _previous.bAnalogButtons[b - 8];
    }

    char field_0[0x2C];                // 0x00
    _XINPUT_GAMEPAD _current;          // 0x2C
    _XINPUT_GAMEPAD _previous;         // 0x3E
    char field_50[0x98 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampGamePad, 0x98);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }
    const SwampGamePad* GetPad(int) const;

    float _elapsed;                    // 0x00
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x68 - 0x8];          // 0x08
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    const Vector3& SetLocation(const Vector3&);
    const Vector3& GetLocation() const { return _Location; }
    const Quaternion& SetRotation(const Quaternion&);
    const Quaternion& GetRotation() const { return _Rotation; }
    Vector3 GetFacing() const;

    char field_4[0x34 - 0x4];          // 0x04
    Vector3 _Location;                 // 0x34
    Quaternion _Rotation;              // 0x40
    char field_50[0xA4 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x10C - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
protected:
    virtual void Update();

public:
    float _shakeFactor;                // 0x10C
    float _shakeDuration;              // 0x110
    float _shakeTimer;                 // 0x114
    bool _isCurrentCamera;             // 0x118
    bool _isAnimated;                  // 0x119
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};
extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class ZoneNode { // NAME: proto-pdb:default.pdb
public:
    static void DebugTrigger();
};

class DebugCam : public GameCam { // NAME: proto-pdb:default.pdb
private:
    virtual void Update();
};
DECOMP_SIZE_ASSERT(DebugCam, 0x11C);

// FUNCTION: USA 0x00137D6F
// PROTOTYPE: PREVIEW 0x00521FA6 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.03, bytes mismatch (frame slot packing of temps; side.x x87 operand form)
void DebugCam::Update()
{
    GameCam::Update();
    if (_isCurrentCamera) {
        // !FAKE: c2 register allocation; dummy local; retail loads the elapsed time once and keeps it in st (no PDB record)
        float v8 = g_swamp->GetInput().GetElapsedTime();
        const float turnrate = v8 * 2.5f;
        const float moverate = v8 * 5.0f;
        const SwampGamePad* pad = g_swamp->GetInput().GetPad(0);
        if (pad) {
            if (pad->GetButtonState(BUTTON_X) >= 0x80 && pad->GetButtonPreviousState(BUTTON_X) < 0x80) {
                // !FAKE: c2 register allocation; dummy local; retail holds the camera in edi, no PDB record (C0223)
                GameCam* p = g_CameraManager->Find("VINCECAM");
                if (p)
                    SetLocation(p->GetLocation());
            }
            if (pad->GetButtonState(BUTTON_BLACK) >= 0x80 && pad->GetButtonPreviousState(BUTTON_BLACK) < 0x80)
                ZoneNode::DebugTrigger();
            Quaternion qh;
            Quaternion qv;
            {
                Vector2 turn = pad->GetRightStick();
                qh.Set(Vector3(0.0f, 1.0f, 0.0f), turn.u * turnrate);
                qv.Set(Vector3(1.0f, 0.0f, 0.0f), turn.v * turnrate);
            }
            // !FAKE: inliner; hand forms of the inlined Quaternion and Vector3 operators (no PDB record, D78); stand-ins v0-v7, v9, v10; blocks give retail's slot sharing (match_one 95.41 -> 98.03)
            Quaternion hrot;
            {
                Quaternion v0(qh);
                v0 *= GetRotation();
                hrot = v0;
            }
            Quaternion newrot;
            {
                Quaternion v1(hrot);
                v1 *= qv;
                newrot = v1;
            }
            Vector3 Z(0.0f, 0.0f, 1.0f);
            {
                Vector3 v2;
                newrot.Apply(Z, v2);
                Vector3 v3(v2);
                const Vector3& v4 = Vector3(v3.y * 0.0f - v3.z, v3.z * 0.0f - v3.x * 0.0f, v3.x - v3.y * 0.0f);
                Vector3 v5;
                hrot.Apply(Z, v5);
                Vector3 v6(v5);
                const Vector3& v7 = Vector3(v6.y * 0.0f - v6.z, v6.z * 0.0f - v6.x * 0.0f, v6.x - v6.y * 0.0f);
                SetRotation(v4.x * v7.x + v4.y * v7.y + v4.z * v7.z > 0.01 ? newrot : hrot);
            }
            {
                Vector2 move = pad->GetLeftStick();
                Vector3 facing = GetFacing();
                Vector3 delta(facing.x * move.v, facing.y * move.v, facing.z * move.v);
                {
                    Vector3 v9(facing.z - facing.y * 0.0f, facing.x * 0.0f - facing.z * 0.0f, facing.y * 0.0f - facing.x);
                    const Vector3& v10 = Vector3(move.u * v9.x, move.u * v9.y, move.u * v9.z);
                    delta += v10;
                }
                delta *= moverate;
                SetLocation(Vector3(delta.x + GetLocation().x, delta.y + GetLocation().y, delta.z + GetLocation().z));
            }
            _shakeFactor = 0.0f;
        }
    }
}
#endif
