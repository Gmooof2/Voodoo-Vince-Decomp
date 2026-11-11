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

#ifdef NON_MATCHING // D78, D117: default.pdb records no free Vector3 operator (5.2 T20)
inline Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
#endif

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

// FUNCTION: USA 0x0006F150
// NAME: proto-pdb:default.pdb
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
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__
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
    // !FAKE: c2 frame slots; stand-in local v0; it stands for retail's unnamed Apply result at [ebp-0x44]
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
            // !FAKE: inliner; stand-in local v1 and member stores, hand form of a Vector3 difference (no free operator in the PDB, D78)
            Vector3 v1;
            v1.x = p->_position.x - v.x;
            v1.y = p->_position.y - v.y;
            v1.z = p->_position.z - v.z;
            p->_lastPosition = v1;
        }
    }
    return count;
}

// FUNCTION: USA 0x0006F2B5
// NAME: proto-pdb:default.pdb
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
            // !FAKE: c2 frame slots and x87 code; stand-in local v1 (retail's first random, y) and per-component pos stores
            float v1 = g_fflyRand.GetRandomFloat(-1.0f, 1.0f);
            Vector3 pos;
            pos.x = g_fflyRand.GetRandomFloat(-1.0f, 1.0f);
            pos.y = v1;
            pos.z = g_fflyRand.GetRandomFloat(-1.0f, 1.0f);
            // !FAKE: inliner; hand form of the ApproxLength body (D117); /O1 keeps the inline call out of line
            float v0 = v1 * v1 + pos.x * pos.x + pos.z * pos.z;
            v0 = FastReciprocalSquareRoot(v0) * v0;
            // !FAKE: c2 x87 code; double stand-in s keeps the scale on st(0) past the z product (fstp st(0), C0323)
            double s = g_fflyRand.GetRandomFloat(param2, param1) / v0;
            pos.x *= s;
            pos.y *= s;
            pos.z *= s;
            mtx.Apply(pos, p->_position);
            float speed = speedCtrl->Evaluate(t) * step;
            float angle1 = g_fflyRand.GetRandomFloat() * 6.2831855f;
            float angle2 = angleCtrl->Evaluate(t);
            Vector3 v;
            // !FAKE: c2 scheduling; stand-in local v3 and per-component stores (Ellipse's form)
            float v3 = sinf(angle2) * speed;
            v.x = sinf(angle1) * v3;
            v.y = cosf(angle2) * speed;
            v.z = cosf(angle1) * v3;
            mtx.ApplyNormal(v, v);
            // !FAKE: inliner; stand-in local v2 and member stores, hand form of a Vector3 difference (no free operator in the PDB, D78)
            Vector3 v2;
            v2.x = p->_position.x - v.x;
            v2.y = p->_position.y - v.y;
            v2.z = p->_position.z - v.z;
            p->_lastPosition = v2;
        }
    }
    return count;
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x0006F7B1
// NAME: proto-pdb:default.pdb
// best: match_one 95.24 under /O1 /Oy /Oi; at-stop 5.2 T12.c8b (20 runs); pos code = retail; only the ApplyNormal
// lea/push: retail after the last velocity store, ours hoisted. Dead: v3/per-component/ctor/Set/double v3/operator[]/
// pointer/comma-arg/label velocity forms, decl moves (91.38-95.24); ctor pos w/o stand-ins parks x (74.60-76.19).
// Sphere's same block moved only with an upstream double stand-in (C0323): look upstream (logs T12.c8, T12.c8b)
// T12.c24 run 20: top-declared speed/angles + v3 Set 93.65; ~300 free probes (pos order/types, decls, velocity) none better
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
            // !FAKE: c2 x87 code; stand-in local v1; retail negates both bounds before the first random
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

// FUNCTION: USA 0x0006F479
// NAME: proto-pdb:default.pdb
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
            // !FAKE: inliner; hand form of a 2D length, len holds the squared length first; retail sums x and z only
            float len = pos.x * pos.x + pos.z * pos.z;
            len = 1.0f / (FastReciprocalSquareRoot(len) * len);
            pos.x = (g_fflyRand.GetRandomFloat() * pos.x) * len * param1;
            pos.z = (g_fflyRand.GetRandomFloat() * pos.z) * len * param2;
            mtx.Apply(pos, p->_position);
            float speed = speedCtrl->Evaluate(t) * step;
            float angle1 = g_fflyRand.GetRandomFloat() * 6.2831855f;
            float angle2 = angleCtrl->Evaluate(t);
            Vector3 v;
            // !FAKE: c2 scheduling; stand-in local v3 and per-component stores; ApplyNormal lea/push placement (match_one 94.41 -> 95.80 -> 100.0)
            float v3 = sinf(angle2) * speed;
            v.x = sinf(angle1) * v3;
            v.y = cosf(angle2) * speed;
            v.z = cosf(angle1) * v3;
            mtx.ApplyNormal(v, v);
            // !FAKE: inliner; stand-in local v0 and member stores, hand form of a Vector3 difference (no free operator in the PDB, D78)
            Vector3 v0;
            v0.x = p->_position.x - v.x;
            v0.y = p->_position.y - v.y;
            v0.z = p->_position.z - v.z;
            p->_lastPosition = v0;
        }
    }
    return count;
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x0006F61F
// NAME: proto-pdb:default.pdb
// best: match_one 98.86 under /O1 /Oy /Oi; retail scales x and z only (y known 0), ours also scales y; explicit
// x/z forms (94.04) lose the scale's st(0) lifetime and the ApplyNormal push placement (wall-x87-lifetime)
// at-stop 5.2 T12.c8 (20 runs) lead: `pos.x = rand; pos.Set(pos.x, 0.0f, rand);` + LengthSquared (y*y folds) +
// `s = ...; pos.x *= s; pos.z *= s;` gives retail's length and scale exactly (92.54); left: the fldz/y store lands
// after the 2nd GetRandomFloat call (retail before it) and the velocity block's lea/push schedule
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
            // !FAKE: inliner; hand form of a 2D length; retail sums x and z only
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
