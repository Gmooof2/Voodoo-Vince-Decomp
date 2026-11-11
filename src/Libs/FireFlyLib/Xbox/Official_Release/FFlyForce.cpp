// UNIT: src/Libs/FireFlyLib/Xbox/Official_Release/FFlyForce.cpp OBJ: Xbox\Official_Release\FFlyForce.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"


struct Vector3 { // NAME: proto-pdb:default.pdb
    void Zero() { x = 0.0f; y = 0.0f; z = 0.0f; }
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb
void* operator new(unsigned int size, const char* file, int line);

class FFlyEmitter { // NAME: proto-pdb:default.pdb
public:
    float GetTimeStep() const { return _timeStep; }

    char field_0[0x4];                 // 0x00
    float _timeStep;                   // 0x04
    char field_8[0x260 - 0x8];         // 0x08
};
DECOMP_SIZE_ASSERT(FFlyEmitter, 0x260);

struct FFlyParticle { // NAME: proto-pdb:default.pdb
    char field_0[0x8];                 // 0x00
    Vector3 _position;                 // 0x08
    Vector3 _lastPosition;             // 0x14
    Vector3 _acceleration;             // 0x20
    char field_2C[0x14];               // 0x2C
};
DECOMP_SIZE_ASSERT(FFlyParticle, 0x40);

struct FFlyForceData; // NAME: proto-pdb:default.pdb

class FFlyForce { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        NONE = 0,
        WIND = 1,
        FLOOR = 2,
        VORTEX = 3,
        ATTRACTOR = 4
    };

    virtual ~FFlyForce() {}                                                   // vtable+0x00
    virtual void Apply(FFlyEmitter& emitter, FFlyParticle* p, unsigned int count) = 0; // vtable+0x04
    static FFlyForce* Create(const FFlyForceData& data);
    static FFlyForce* Create(unsigned int type);
};
DECOMP_SIZE_ASSERT(FFlyForce, 0x4);

class FFlyFloor : public FFlyForce { // NAME: proto-pdb:default.pdb
public:
    struct Params { // NAME: proto-pdb:default.pdb
        float coeff;  // 0x00
        float height; // 0x04
    };

    FFlyFloor() { _data.height = 0.0f; _data.coeff = 1.0f; }
    virtual void Apply(FFlyEmitter& emitter, FFlyParticle* p, unsigned int count);
    virtual ~FFlyFloor() {}

    Params _data;                      // 0x04
};
DECOMP_SIZE_ASSERT(FFlyFloor, 0xC);

// FUNCTION: USA 0x0014F5A1
// NAME: proto-pdb:default.pdb
class FFlyWind : public FFlyForce { // NAME: proto-pdb:default.pdb
public:
    struct Params { // NAME: proto-pdb:default.pdb
        Vector3 wind;   // 0x00
        float maxSpeed; // 0x0C
    };

    FFlyWind();
    virtual void Apply(FFlyEmitter& emitter, FFlyParticle* p, unsigned int count);
    virtual ~FFlyWind() {}

    Params _data;                      // 0x04
};
DECOMP_SIZE_ASSERT(FFlyWind, 0x14);

class FFlyVortex : public FFlyForce { // NAME: proto-pdb:default.pdb
public:
    struct Params { // NAME: proto-pdb:default.pdb
        float angle;  // 0x00
        float accel;  // 0x04
        Vector3 axis; // 0x08
        Vector3 apex; // 0x14
    };

    FFlyVortex();
    virtual void Apply(FFlyEmitter& emitter, FFlyParticle* p, unsigned int count);
    virtual ~FFlyVortex() {}

    Params _data;                      // 0x04
};
DECOMP_SIZE_ASSERT(FFlyVortex, 0x24);

class FFlyAttractor : public FFlyForce { // NAME: proto-pdb:default.pdb
public:
    struct Params { // NAME: proto-pdb:default.pdb
        float accel;  // 0x00
        float inner;  // 0x04
        float outer;  // 0x08
        Vector3 loc;  // 0x0C
    };

    FFlyAttractor();
    virtual void Apply(FFlyEmitter& emitter, FFlyParticle* p, unsigned int count);
    virtual ~FFlyAttractor() {}

    Params _data;                      // 0x04
};
DECOMP_SIZE_ASSERT(FFlyAttractor, 0x1C);

struct FFlyForceData { // NAME: proto-pdb:default.pdb
    unsigned int _type;                // 0x00
    Vector3 _constAccel;               // 0x04
    FFlyWind::Params _wind;            // 0x10
    FFlyFloor::Params _floor;          // 0x20
    FFlyVortex::Params _vortex;        // 0x28
    FFlyAttractor::Params _attractor;  // 0x48
};
DECOMP_SIZE_ASSERT(FFlyForceData, 0x60);

// FUNCTION: USA 0x0014F32C
// PROTOTYPE: PREVIEW 0x005389D7 identical
// NAME: proto-pdb:default.pdb
void FFlyFloor::Apply(FFlyEmitter& emitter, FFlyParticle* p, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++, p++) {
        if (p->_position.y < _data.height) {
            p->_lastPosition.y = _data.height + (p->_position.y - p->_lastPosition.y) * _data.coeff;
            p->_position.y = _data.height;
        }
    }
}

// FUNCTION: USA 0x0014F365
// PROTOTYPE: PREVIEW 0x00538A2D identical
// NAME: proto-pdb:default.pdb
void FFlyWind::Apply(FFlyEmitter& emitter, FFlyParticle* p, unsigned int count)
{
    float maxSpeed = emitter.GetTimeStep() * _data.maxSpeed;
    for (unsigned int i = 0; i < count; i++, p++) {
        // !FAKE: inliner; pointer locals + hand form of retail's inlined (p->_position - p->_lastPosition).Length(); match_one 97.85 -> 100.0, because
        // the two operand pointers fix esi at p+0x18 (scalar form: p+0x14, match_one 97.85 -> 100.0, C0191)
        const Vector3* v3 = &p->_position;
        const Vector3* v4 = &p->_lastPosition;
        float v0 = v3->x - v4->x;
        float v1 = v3->y - v4->y;
        float v2 = v3->z - v4->z;
        float tmp = v0 * v0 + v1 * v1 + v2 * v2;
        if (tmp * FastReciprocalSquareRoot(tmp) < maxSpeed) {
            // !FAKE: inliner; hand form of retail's inlined Vector3::operator+=; out of line in our /O1 build
            p->_acceleration.x += _data.wind.x;
            p->_acceleration.y += _data.wind.y;
            p->_acceleration.z += _data.wind.z;
        }
    }
}

// FUNCTION: USA 0x0014F54F
// PROTOTYPE: PREVIEW 0x00538C17 identical
// NAME: proto-pdb:default.pdb
FFlyWind::FFlyWind()
{
    _data.wind.Zero();
    _data.maxSpeed = 1.0f;
}

// FUNCTION: USA 0x0014F56C
// NAME: proto-pdb:default.pdb
FFlyVortex::FFlyVortex()
{
    _data.angle = 0.785398163f;
    _data.accel = 10.0f;
    _data.axis.Set(0.0f, 1.0f, 0.0f);
    _data.apex.Zero();
}

// FUNCTION: USA 0x0014F5BE
// PROTOTYPE: PREVIEW 0x00538C69 identical
// NAME: proto-pdb:default.pdb
FFlyAttractor::FFlyAttractor()
{
    _data.accel = 10.0f;
    _data.inner = 1000.0f;
    _data.outer = 1000.0f;
    _data.loc.Zero();
}

// FUNCTION: USA 0x0014F5E9
// PROTOTYPE: PREVIEW 0x00538C94 identical
// NAME: proto-pdb:default.pdb
FFlyForce* FFlyForce::Create(unsigned int type)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
    switch (type) {
    case WIND:
        return new("FFlyForce.cpp", 21) FFlyWind;
    case FLOOR:
        return new("FFlyForce.cpp", 24) FFlyFloor;
    case VORTEX:
        return new("FFlyForce.cpp", 27) FFlyVortex;
    case ATTRACTOR:
        return new("FFlyForce.cpp", 30) FFlyAttractor;
    }
    return 0;
}

// FUNCTION: USA 0x0014F676
// PROTOTYPE: PREVIEW 0x00538D21 identical
// NAME: proto-pdb:default.pdb
FFlyForce* FFlyForce::Create(const FFlyForceData& data)
{
    FFlyForce* p = Create(data._type);
    if (!p)
        return 0;
    switch (data._type) {
    case WIND:
        static_cast<FFlyWind*>(p)->_data = data._wind;
        break;
    case FLOOR:
        static_cast<FFlyFloor*>(p)->_data = data._floor;
        break;
    case VORTEX:
        static_cast<FFlyVortex*>(p)->_data = data._vortex;
        break;
    case ATTRACTOR:
        static_cast<FFlyAttractor*>(p)->_data = data._attractor;
        break;
    }
    return p;
}
