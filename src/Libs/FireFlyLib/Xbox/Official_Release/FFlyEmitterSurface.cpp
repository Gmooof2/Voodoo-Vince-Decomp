// UNIT: src/Libs/FireFlyLib/Xbox/Official_Release/FFlyEmitterSurface.cpp OBJ: Xbox\Official_Release\FFlyEmitterSurface.obj OPT: /O1 /Oy /Oi
#include <math.h>
#include "../../../../include/decomp.h"

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    float LengthSquared() const { return x * x + y * y + z * z; }
    Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

inline Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }

struct Vector4 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3& v, Vector3& result) const;
    Vector3 Apply(const Vector3& v) const { Vector3 result; Apply(v, result); return result; }
    void ApplyNormal(const Vector3& v, Vector3& result) const;
    void AffineMultiply(const Matrix& m, Matrix& result) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

class QuickRand { // NAME: proto-pdb:default.pdb
public:
    float GetRandomFloat();
    float GetRandomFloat(float lo, float hi) { return GetRandomFloat() * (hi - lo) + lo; }

    unsigned int _Mul;                 // 0x00
    unsigned int _Carry;               // 0x04
};
DECOMP_SIZE_ASSERT(QuickRand, 0x8);

extern QuickRand g_fflyRand; // NAME: proto-pdb:default.pdb

class FFlyController { // NAME: proto-pdb:default.pdb
public:
    virtual ~FFlyController();                       // vtable+0x00
    virtual void Initialize(float&, float&);         // vtable+0x04
    virtual float Evaluate(double) const = 0;        // vtable+0x08

    char field_4[0x8];                 // 0x04
};
DECOMP_SIZE_ASSERT(FFlyController, 0xC);

struct FFlyParticle { // NAME: proto-pdb:default.pdb
    enum { ALIVE = 0x1, SPAWNING = 0x2 };

    unsigned int _flags;               // 0x00
    float _life;                       // 0x04
    Vector3 _position;                 // 0x08
    Vector3 _lastPosition;             // 0x14
    Vector3 _acceleration;             // 0x20
    unsigned int _color;               // 0x2C
    unsigned int _texId;               // 0x30
    float _size;                       // 0x34
    unsigned int _pad[2];              // 0x38
};
DECOMP_SIZE_ASSERT(FFlyParticle, 0x40);

class FFlyEmitter { // NAME: proto-pdb:default.pdb
public:
    const Matrix& GetTransformMatrix() const { return _matrix; }
    const Matrix& GetWorldMatrix() const { return _worldMatrix; }
    float GetTimeStep() const { return _timeStep; }
    double GetTime() const { return _time; }
    FFlyController* GetAngleCtrl() { return _angle; }
    FFlyController* GetSpeedCtrl() { return _speed; }

    char field_0[0x4];                 // 0x00
    float _timeStep;                   // 0x04
    char field_8[0x10 - 0x8];          // 0x08
    double _time;                      // 0x10
    char field_18[0x5C - 0x18];        // 0x18
    FFlyController* _angle;            // 0x5C
    FFlyController* _speed;            // 0x60
    char field_64[0x94 - 0x64];        // 0x64
    Matrix _matrix;                    // 0x94
    Matrix _worldMatrix;               // 0xD4
    char field_114[0x260 - 0x114];     // 0x114
};
DECOMP_SIZE_ASSERT(FFlyEmitter, 0x260);

void* operator new(unsigned int size, const char* file, int line);

class FFlyEmitterSurface { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        NONE = 0,
        POINT = 1,
        SPHERE = 2,
        ELLIPSE = 3,
        RING = 4,
        PLANE = 5,
        ALIGNED_SPHERE = 6
    };

    virtual ~FFlyEmitterSurface() {}                 // vtable+0x00
    virtual int Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max) = 0; // vtable+0x04
    static FFlyEmitterSurface* Create(unsigned int type);
};
DECOMP_SIZE_ASSERT(FFlyEmitterSurface, 0x4);

class FFlyPointSurf : public FFlyEmitterSurface { // NAME: proto-pdb:default.pdb
public:
    virtual int Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max);
};
DECOMP_SIZE_ASSERT(FFlyPointSurf, 0x4);

class FFlySphereSurf : public FFlyEmitterSurface { // NAME: proto-pdb:default.pdb
public:
    virtual int Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max);
};
DECOMP_SIZE_ASSERT(FFlySphereSurf, 0x4);

class FFlyEllipseSurf : public FFlyEmitterSurface { // NAME: proto-pdb:default.pdb
public:
    virtual int Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max);
};
DECOMP_SIZE_ASSERT(FFlyEllipseSurf, 0x4);

class FFlyRingSurf : public FFlyEmitterSurface { // NAME: proto-pdb:default.pdb
public:
    virtual int Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max);
};
DECOMP_SIZE_ASSERT(FFlyRingSurf, 0x4);

class FFlyPlaneSurf : public FFlyEmitterSurface { // NAME: proto-pdb:default.pdb
public:
    virtual int Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max);
};
DECOMP_SIZE_ASSERT(FFlyPlaneSurf, 0x4);

class FFlyAlignedSphereSurf : public FFlyEmitterSurface { // NAME: proto-pdb:default.pdb
public:
    virtual int Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max);
};
DECOMP_SIZE_ASSERT(FFlyAlignedSphereSurf, 0x4);

// FUNCTION: USA 0x0006F07F
// NAME: proto-pdb:default.pdb
FFlyEmitterSurface* FFlyEmitterSurface::Create(unsigned int type)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__
    switch (type) {
    case POINT:
        return new("FFlyEmitterSurface.cpp", 24) FFlyPointSurf;
    case SPHERE:
        return new("FFlyEmitterSurface.cpp", 27) FFlySphereSurf;
    case ELLIPSE:
        return new("FFlyEmitterSurface.cpp", 30) FFlyEllipseSurf;
    case RING:
        return new("FFlyEmitterSurface.cpp", 33) FFlyRingSurf;
    case PLANE:
        return new("FFlyEmitterSurface.cpp", 36) FFlyPlaneSurf;
    case ALIGNED_SPHERE:
        return new("FFlyEmitterSurface.cpp", 39) FFlyAlignedSphereSurf;
    }
    return 0;
}

// FUNCTION: USA 0x0006F16D
// NAME: proto-pdb:default.pdb
int FFlyPointSurf::Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max)
{
    int count = 0;
    float step = emitter.GetTimeStep();
    double t = emitter.GetTime();
    FFlyController* speedCtrl = emitter.GetSpeedCtrl();
    FFlyController* angleCtrl = emitter.GetAngleCtrl();
    Matrix mtx;
    float angle1;
    float speed;
    float angle2;
    emitter.GetTransformMatrix().AffineMultiply(emitter.GetWorldMatrix(), mtx);
    Vector3 v(0.0f, 0.0f, 0.0f);
    // !FAKE: stand-in for retail's unnamed Apply result at [ebp-0x44]
    Vector3 v0;
    mtx.Apply(v, v0);
    for (unsigned int i = 0; i < max; i++, p++) {
        if (p->_flags & FFlyParticle::SPAWNING) {
            count++;
            p->_flags = FFlyParticle::ALIVE;
            p->_position = v0;
            speed = speedCtrl->Evaluate(t) * step;
            angle1 = g_fflyRand.GetRandomFloat() * 6.2831855f;
            angle2 = angleCtrl->Evaluate(t);
            v.Set(sinf(angle1) * (sinf(angle2) * speed), cosf(angle2) * speed, cosf(angle1) * (sinf(angle2) * speed));
            mtx.ApplyNormal(v, v);
            p->_lastPosition = p->_position - v;
        }
    }
    return count;
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x0006F2B5
// NAME: proto-pdb:default.pdb
// best: match_one 95.15 under /O1 /Oy /Oi; retail stores the x random straight into pos.x and puts count below pos
// (-0x10); every pos form here (ctor, Set, per-component, y stand-in) keeps a temp or flips the frame (wall-regalloc)
int FFlySphereSurf::Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max)
{
    int count = 0;
    float step = emitter.GetTimeStep();
    double t = emitter.GetTime();
    FFlyController* speedCtrl = emitter.GetSpeedCtrl();
    FFlyController* angleCtrl = emitter.GetAngleCtrl();
    Matrix mtx;
    emitter.GetTransformMatrix().AffineMultiply(emitter.GetWorldMatrix(), mtx);
    for (unsigned int i = 0; i < max; i++, p++) {
        if (p->_flags & FFlyParticle::SPAWNING) {
            count++;
            p->_flags = FFlyParticle::ALIVE;
            Vector3 pos(g_fflyRand.GetRandomFloat(-1.0f, 1.0f), g_fflyRand.GetRandomFloat(-1.0f, 1.0f), g_fflyRand.GetRandomFloat(-1.0f, 1.0f));
            // !FAKE: inliner; hand form of the ApproxLength body (D117); /O1 keeps the inline call out of line
            float v0 = pos.LengthSquared();
            v0 = FastReciprocalSquareRoot(v0) * v0;
            pos *= g_fflyRand.GetRandomFloat(param2, param1) / v0;
            mtx.Apply(pos, p->_position);
            float speed = speedCtrl->Evaluate(t) * step;
            float angle1 = g_fflyRand.GetRandomFloat() * 6.2831855f;
            float angle2 = angleCtrl->Evaluate(t);
            Vector3 v;
            v.Set(sinf(angle1) * (sinf(angle2) * speed), cosf(angle2) * speed, cosf(angle1) * (sinf(angle2) * speed));
            mtx.ApplyNormal(v, v);
            p->_lastPosition = p->_position - v;
        }
    }
    return count;
}
#endif

#ifdef NON_MATCHING
// FUNCTION: USA 0x0006F7B1
// NAME: proto-pdb:default.pdb
// best: match_one 95.24 under /O1 /Oy /Oi; only the ApplyNormal `lea/push` pair is scheduled after the velocity
// stores in retail, before them in ours; 5 velocity forms tried (wall-schedule)
int FFlyPlaneSurf::Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max)
{
    int count = 0;
    float step = emitter.GetTimeStep();
    double t = emitter.GetTime();
    FFlyController* speedCtrl = emitter.GetSpeedCtrl();
    FFlyController* angleCtrl = emitter.GetAngleCtrl();
    Matrix mtx;
    emitter.GetTransformMatrix().AffineMultiply(emitter.GetWorldMatrix(), mtx);
    for (unsigned int i = 0; i < max; i++, p++) {
        if (p->_flags & FFlyParticle::SPAWNING) {
            count++;
            p->_flags = FFlyParticle::ALIVE;
            // !FAKE: stand-in; retail negates both bounds before the first random
            float v1 = -param2;
            float v2 = -param1;
            Vector3 pos;
            pos.x = g_fflyRand.GetRandomFloat(v2, param1) * 0.5f;
            pos.y = 0.0f;
            pos.z = g_fflyRand.GetRandomFloat(v1, param2) * 0.5f;
            mtx.Apply(pos, p->_position);
            float speed = speedCtrl->Evaluate(t) * step;
            float angle1 = g_fflyRand.GetRandomFloat() * 6.2831855f;
            float angle2 = angleCtrl->Evaluate(t);
            Vector3 v;
            v.Set(sinf(angle1) * (sinf(angle2) * speed), cosf(angle2) * speed, cosf(angle1) * (sinf(angle2) * speed));
            mtx.ApplyNormal(v, v);
            p->_lastPosition = p->_position - v;
        }
    }
    return count;
}
#endif

#ifdef NON_MATCHING
// FUNCTION: USA 0x0006F479
// NAME: proto-pdb:default.pdb
// best: match_one 94.41 under /O1 /Oy /Oi; only the ApplyNormal `lea/push` pair placement inside the velocity
// block differs; 5 velocity forms tried (wall-schedule)
int FFlyEllipseSurf::Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max)
{
    int count = 0;
    float step = emitter.GetTimeStep();
    double t = emitter.GetTime();
    FFlyController* speedCtrl = emitter.GetSpeedCtrl();
    FFlyController* angleCtrl = emitter.GetAngleCtrl();
    Matrix mtx;
    emitter.GetTransformMatrix().AffineMultiply(emitter.GetWorldMatrix(), mtx);
    for (unsigned int i = 0; i < max; i++, p++) {
        if (p->_flags & FFlyParticle::SPAWNING) {
            count++;
            p->_flags = FFlyParticle::ALIVE;
            Vector3 pos;
            pos.x = g_fflyRand.GetRandomFloat(-1.0f, 1.0f);
            pos.y = 0.0f;
            pos.z = g_fflyRand.GetRandomFloat(-1.0f, 1.0f);
            // !FAKE: inliner; hand form of a 2D length (retail sums x and z only), len holds the squared length first
            float len = pos.x * pos.x + pos.z * pos.z;
            len = 1.0f / (FastReciprocalSquareRoot(len) * len);
            pos.x = (g_fflyRand.GetRandomFloat() * pos.x) * len * param1;
            pos.z = (g_fflyRand.GetRandomFloat() * pos.z) * len * param2;
            mtx.Apply(pos, p->_position);
            float speed = speedCtrl->Evaluate(t) * step;
            float angle1 = g_fflyRand.GetRandomFloat() * 6.2831855f;
            float angle2 = angleCtrl->Evaluate(t);
            Vector3 v;
            v.Set(sinf(angle1) * (sinf(angle2) * speed), cosf(angle2) * speed, cosf(angle1) * (sinf(angle2) * speed));
            mtx.ApplyNormal(v, v);
            p->_lastPosition = p->_position - v;
        }
    }
    return count;
}
#endif

#ifdef NON_MATCHING
// FUNCTION: USA 0x0006F61F
// NAME: proto-pdb:default.pdb
// best: match_one 98.86 under /O1 /Oy /Oi; retail scales x and z only (y known 0), ours also scales y; explicit
// x/z forms (94.04) lose the scale's st(0) lifetime and the ApplyNormal push placement (wall-x87-lifetime)
int FFlyRingSurf::Spawn(FFlyEmitter& emitter, FFlyParticle* p, float param1, float param2, unsigned int max)
{
    int count = 0;
    float step = emitter.GetTimeStep();
    double t = emitter.GetTime();
    FFlyController* speedCtrl = emitter.GetSpeedCtrl();
    FFlyController* angleCtrl = emitter.GetAngleCtrl();
    Matrix mtx;
    emitter.GetTransformMatrix().AffineMultiply(emitter.GetWorldMatrix(), mtx);
    for (unsigned int i = 0; i < max; i++, p++) {
        if (p->_flags & FFlyParticle::SPAWNING) {
            count++;
            p->_flags = FFlyParticle::ALIVE;
            Vector3 pos;
            pos.x = g_fflyRand.GetRandomFloat(-1.0f, 1.0f);
            pos.y = 0.0f;
            pos.z = g_fflyRand.GetRandomFloat(-1.0f, 1.0f);
            // !FAKE: inliner; hand form of a 2D length (retail sums x and z only)
            float v0 = pos.x * pos.x + pos.z * pos.z;
            v0 = FastReciprocalSquareRoot(v0) * v0;
            pos *= g_fflyRand.GetRandomFloat(param2, param1) / v0;
            mtx.Apply(pos, p->_position);
            float speed = speedCtrl->Evaluate(t) * step;
            float angle1 = g_fflyRand.GetRandomFloat() * 6.2831855f;
            float angle2 = angleCtrl->Evaluate(t);
            Vector3 v;
            v.Set(sinf(angle1) * (sinf(angle2) * speed), cosf(angle2) * speed, cosf(angle1) * (sinf(angle2) * speed));
            mtx.ApplyNormal(v, v);
            p->_lastPosition = p->_position - v;
        }
    }
    return count;
}
#endif
