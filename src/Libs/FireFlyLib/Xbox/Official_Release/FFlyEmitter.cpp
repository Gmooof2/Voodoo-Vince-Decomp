// UNIT: src/Libs/FireFlyLib/Xbox/Official_Release/FFlyEmitter.cpp OBJ: Xbox\Official_Release\FFlyEmitter.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <xmmintrin.h>

class FFlyController { // NAME: proto-pdb:default.pdb
public:
    virtual ~FFlyController();                       // vtable+0x00
    virtual void Initialize(float&, float&);         // vtable+0x04
    virtual float Evaluate(double) const = 0;        // vtable+0x08

    char field_4[0x8];                 // 0x04
};
DECOMP_SIZE_ASSERT(FFlyController, 0xC);

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    unsigned int GetARGB() const;

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    void TransformationAboutPivot(float, float, float, const Vector3&, const Vector3&);

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct AABox { // NAME: proto-pdb:default.pdb
    void AddPoint(const Vector3& pt);

    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

class FFlyControllerV3 { // NAME: proto-pdb:default.pdb
public:
    virtual ~FFlyControllerV3();                     // vtable+0x00
    virtual void Initialize(Vector3&, Vector3&);     // vtable+0x04
    virtual void Evaluate(double, Vector3&) const = 0; // vtable+0x08

    char field_4[0x8];                 // 0x04
};
DECOMP_SIZE_ASSERT(FFlyControllerV3, 0xC);

struct FFlyData { // NAME: proto-pdb:default.pdb
    char field_0[0x4];                 // 0x00
    float _param1[2];                  // 0x04
    float _param2[2];                  // 0x0C
    Vector3 _location[2];              // 0x14
    Vector3 _rotation[2];              // 0x2C
    Vector3 _pivot;                    // 0x44
    unsigned int _renderEffects;       // 0x50
    char field_54[0x70 - 0x54];        // 0x54
    unsigned int _maxParticles;        // 0x70
    unsigned int _spawnParticles;      // 0x74
    float _rate[2];                    // 0x78
    float _lifeSpan[2];                // 0x80
    float _angle[2];                   // 0x88
    float _speed[2];                   // 0x90
    Vector3 _color[2];                 // 0x98
    float _alpha[2];                   // 0xB0
    float _texId[2];                   // 0xB8
    float _size[2];                    // 0xC0
    char field_C8[0x2C];               // 0xC8
    Vector3 _renderColor[2];           // 0xF4
    float _renderAlpha[2];             // 0x10C
    float _renderTex[2];               // 0x114
    float _renderSize[2];              // 0x11C
    Vector3 _renderNormal;             // 0x124
    unsigned int _renderColorCtrl;     // 0x130
    unsigned int _renderAlphaCtrl;     // 0x134
    unsigned int _renderTexCtrl;       // 0x138
    unsigned int _renderSizeCtrl;      // 0x13C
    Vector3 _constAccel;               // 0x140
};
DECOMP_SIZE_ASSERT(FFlyData, 0x14C);

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    iterator begin() { return _Myfirst; }
    iterator end() { return _Mylast; }
    unsigned int size() const;
    void clear() { _Tidy(); }
    ~vector() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}

class FFlyEmitter;
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

class FFlyForce { // NAME: proto-pdb:default.pdb
public:
    virtual ~FFlyForce();                            // vtable+0x00
    virtual void Apply(FFlyEmitter&, FFlyParticle*, unsigned int) = 0; // vtable+0x04
};
DECOMP_SIZE_ASSERT(FFlyForce, 0x4);

class FFlyEmitterSurface { // NAME: proto-pdb:default.pdb
public:
    virtual ~FFlyEmitterSurface();                   // vtable+0x00
    virtual int Spawn(FFlyEmitter&, FFlyParticle*, float, float, unsigned int) = 0; // vtable+0x04
};
DECOMP_SIZE_ASSERT(FFlyEmitterSurface, 0x4);

class FFlyRenderer { // NAME: proto-pdb:default.pdb
public:
    virtual void Render(const FFlyEmitter&, const FFlyParticle*, unsigned int) = 0; // vtable+0x00
};
DECOMP_SIZE_ASSERT(FFlyRenderer, 0x4);

class FFlyEmitter { // NAME: proto-pdb:default.pdb
public:
    void Render(FFlyRenderer* renderer) const;
    void SetSurface(FFlyEmitterSurface* surface);
    void SetParam1Ctrl(FFlyController* ctrl);
    void SetParam2Ctrl(FFlyController* ctrl);
    void SetLocationCtrl(FFlyControllerV3* ctrl);
    void SetRotationCtrl(FFlyControllerV3* ctrl);
    void SetRateCtrl(FFlyController* ctrl);
    void SetLifeSpanCtrl(FFlyController* ctrl);
    void SetAngleCtrl(FFlyController* ctrl);
    void SetSpeedCtrl(FFlyController* ctrl);
    void SetColorCtrl(FFlyControllerV3* ctrl);
    void SetAlphaCtrl(FFlyController* ctrl);
    void SetTexIdCtrl(FFlyController* ctrl);
    void SetSizeCtrl(FFlyController* ctrl);
    void SetRenderColorCtrl(FFlyControllerV3* ctrl);
    void SetRenderAlphaCtrl(FFlyController* ctrl);
    void SetRenderTexCtrl(FFlyController* ctrl);
    void SetRenderSizeCtrl(FFlyController* ctrl);
    void GetParams(float& param1, float& param2) const;
    void GetRenderColor(float life, Vector3& clr) const;
    float GetRenderAlpha(float life, float alpha) const;
    unsigned int GetRenderTex(float life, unsigned int texid) const;
    float GetRenderSize(float life, float size) const;
    void ComputeBounds(AABox& box, float* maxSize) const;
    void ClearForces();
    ~FFlyEmitter();

    void Reset();
    void Step();

private:
    void initialize();

public:
    bool _spawning;                    // 0x00
    char field_1[0x4 - 0x1];           // 0x01
    float _timeStep;                   // 0x04
    float _timeStepSqr;                // 0x08
    float _timeStepScale;              // 0x0C
    double _time;                      // 0x10
    float _genAcc;                     // 0x18
    char field_1C[0x20 - 0x1C];        // 0x1C
    double _lastTime;                  // 0x20
    float _stepAcc;                    // 0x28
    unsigned int _poolLo;              // 0x2C
    unsigned int _poolHi;              // 0x30
    unsigned int _live;                // 0x34
    unsigned int _poolBlks;            // 0x38
    FFlyParticle* _pool;               // 0x3C
    FFlyEmitterSurface* _surface;      // 0x40
    FFlyController* _param1;           // 0x44
    FFlyController* _param2;           // 0x48
    FFlyControllerV3* _location;       // 0x4C
    FFlyControllerV3* _rotation;       // 0x50
    FFlyController* _rate;             // 0x54
    FFlyController* _lifeSpan;         // 0x58
    FFlyController* _angle;            // 0x5C
    FFlyController* _speed;            // 0x60
    FFlyControllerV3* _color;          // 0x64
    FFlyController* _alpha;            // 0x68
    FFlyController* _texId;            // 0x6C
    FFlyController* _size;             // 0x70
    FFlyControllerV3* _renderColor;    // 0x74
    FFlyController* _renderAlpha;      // 0x78
    FFlyController* _renderTex;        // 0x7C
    FFlyController* _renderSize;       // 0x80
    std::vector<FFlyForce*> _forces;   // 0x84
    Matrix _matrix;                    // 0x94
    char field_D4[0x114 - 0xD4];       // 0xD4
    FFlyData _data;                    // 0x114
};
DECOMP_SIZE_ASSERT(FFlyEmitter, 0x260);

class FFlyManager { // NAME: proto-pdb:default.pdb
public:
    enum { PARTICLES_PER_BLOCK = 0x40 };

    FFlyParticle* Alloc(unsigned int);
    void Free(FFlyParticle* block, unsigned int nblocks);
    static unsigned int FitBlocks(unsigned int);

private:
    friend class FFlyEmitter;
    static FFlyManager* s_ffmanager;
};

// FUNCTION: USA 0x0006CB83
// PROTOTYPE: PREVIEW 0x00458E16 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::Reset()
{
    if (_pool) {
        FFlyManager::s_ffmanager->Free(_pool, _poolBlks);
        _pool = 0;
        _poolBlks = 0;
    }
    _poolHi = 0;
    _lastTime = 0.0;
    _poolLo = 0;
    _live = 0;
    _time = 0.0;
    _spawning = true;
    _stepAcc = 0.0f;
    _genAcc = 0.0f;
}

// FUNCTION: USA 0x0006CBC8
// PROTOTYPE: PREVIEW 0x00458E5B identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::Render(FFlyRenderer* renderer) const
{
    if (_pool && _poolHi)
        renderer->Render(*this, _pool, _poolHi);
}

// FUNCTION: USA 0x0006CBEA
// PROTOTYPE: PREVIEW 0x00458E7D identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetSurface(FFlyEmitterSurface* surf)
{
    delete _surface;
    _surface = surf;
}

// FUNCTION: USA 0x0006CC05
// PROTOTYPE: PREVIEW 0x00458E98 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetParam1Ctrl(FFlyController* ctrl)
{
    delete _param1;
    _param1 = ctrl;
    if (ctrl)
        ctrl->Initialize(_data._param1[0], _data._param1[1]);
}

// FUNCTION: USA 0x0006CC37
// PROTOTYPE: PREVIEW 0x00458ECA identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetParam2Ctrl(FFlyController* ctrl)
{
    delete _param2;
    _param2 = ctrl;
    if (ctrl)
        ctrl->Initialize(_data._param2[0], _data._param2[1]);
}

// FUNCTION: USA 0x0006CC69
// PROTOTYPE: PREVIEW 0x00458EFC identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetLocationCtrl(FFlyControllerV3* ctrl)
{
    delete _location;
    _location = ctrl;
    if (ctrl)
        ctrl->Initialize(_data._location[0], _data._location[1]);
}

// FUNCTION: USA 0x0006CC9B
// PROTOTYPE: PREVIEW 0x00458F2E identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetRotationCtrl(FFlyControllerV3* ctrl)
{
    delete _rotation;
    _rotation = ctrl;
    if (ctrl)
        ctrl->Initialize(_data._rotation[0], _data._rotation[1]);
}

// FUNCTION: USA 0x0006CCCD
// PROTOTYPE: PREVIEW 0x00458F60 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetRateCtrl(FFlyController* ctrl)
{
    delete _rate;
    _rate = ctrl;
    ctrl->Initialize(_data._rate[0], _data._rate[1]);
}

// FUNCTION: USA 0x0006CCFB
// PROTOTYPE: PREVIEW 0x00458F8E identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetLifeSpanCtrl(FFlyController* ctrl)
{
    delete _lifeSpan;
    _lifeSpan = ctrl;
    ctrl->Initialize(_data._lifeSpan[0], _data._lifeSpan[1]);
}

// FUNCTION: USA 0x0006CD29
// PROTOTYPE: PREVIEW 0x00458FBC identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetAngleCtrl(FFlyController* ctrl)
{
    delete _angle;
    _angle = ctrl;
    ctrl->Initialize(_data._angle[0], _data._angle[1]);
}

// FUNCTION: USA 0x0006CD57
// PROTOTYPE: PREVIEW 0x00458FEA identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetSpeedCtrl(FFlyController* ctrl)
{
    delete _speed;
    _speed = ctrl;
    ctrl->Initialize(_data._speed[0], _data._speed[1]);
}

// FUNCTION: USA 0x0006CD85
// PROTOTYPE: PREVIEW 0x00459018 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetColorCtrl(FFlyControllerV3* ctrl)
{
    delete _color;
    _color = ctrl;
    ctrl->Initialize(_data._color[0], _data._color[1]);
}

// FUNCTION: USA 0x0006CDB3
// PROTOTYPE: PREVIEW 0x00459046 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetAlphaCtrl(FFlyController* ctrl)
{
    delete _alpha;
    _alpha = ctrl;
    ctrl->Initialize(_data._alpha[0], _data._alpha[1]);
}

// FUNCTION: USA 0x0006CDE1
// PROTOTYPE: PREVIEW 0x00459074 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetTexIdCtrl(FFlyController* ctrl)
{
    delete _texId;
    _texId = ctrl;
    ctrl->Initialize(_data._texId[0], _data._texId[1]);
}

// FUNCTION: USA 0x0006CE0F
// PROTOTYPE: PREVIEW 0x004590A2 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetSizeCtrl(FFlyController* ctrl)
{
    delete _size;
    _size = ctrl;
    ctrl->Initialize(_data._size[0], _data._size[1]);
}

// FUNCTION: USA 0x0006CE3D
// PROTOTYPE: PREVIEW 0x004590D0 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetRenderColorCtrl(FFlyControllerV3* ctrl)
{
    delete _renderColor;
    _renderColor = ctrl;
    if (ctrl)
        ctrl->Initialize(_data._renderColor[0], _data._renderColor[1]);
}

// FUNCTION: USA 0x0006CE6F
// PROTOTYPE: PREVIEW 0x00459102 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetRenderAlphaCtrl(FFlyController* ctrl)
{
    delete _renderAlpha;
    _renderAlpha = ctrl;
    if (ctrl)
        ctrl->Initialize(_data._renderAlpha[0], _data._renderAlpha[1]);
}

// FUNCTION: USA 0x0006CEA1
// PROTOTYPE: PREVIEW 0x00459134 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetRenderTexCtrl(FFlyController* ctrl)
{
    delete _renderTex;
    _renderTex = ctrl;
    if (ctrl)
        ctrl->Initialize(_data._renderTex[0], _data._renderTex[1]);
}

// FUNCTION: USA 0x0006CED3
// PROTOTYPE: PREVIEW 0x00459166 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::SetRenderSizeCtrl(FFlyController* ctrl)
{
    delete _renderSize;
    _renderSize = ctrl;
    if (ctrl)
        ctrl->Initialize(_data._renderSize[0], _data._renderSize[1]);
}

// FUNCTION: USA 0x0006CF0B
// PROTOTYPE: PREVIEW 0x0045919E identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::GetParams(float& param1, float& param2) const
{
    param1 = _param1 ? _param1->Evaluate(_time) : _data._param1[0];
    param2 = _param2 ? _param2->Evaluate(_time) : _data._param2[0];
}

// FUNCTION: USA 0x0006CF62
// PROTOTYPE: PREVIEW 0x004591F5 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::GetRenderColor(float life, Vector3& clr) const
{
    if (_renderColor) {
        _renderColor->Evaluate((1.0f - life) * _data._lifeSpan[1], clr);
        return;
    }
    float tr = _data._renderColor[0].x;
    float tg = _data._renderColor[0].y;
    float tb = _data._renderColor[0].z;
    clr.x = tr + (clr.x - tr) * life;
    clr.y = tg + (clr.y - tg) * life;
    clr.z = tb + (clr.z - tb) * life;
}

// FUNCTION: USA 0x0006CFD2
// PROTOTYPE: PREVIEW 0x00459265 identical
// NAME: proto-pdb:default.pdb
float FFlyEmitter::GetRenderAlpha(float life, float alpha) const
{
    if (_renderAlpha) {
        float v1 = (1.0f - life) * _data._lifeSpan[1];
        return _renderAlpha->Evaluate(v1);
    }
    float v2 = _data._renderAlpha[0];
    return v2 + (alpha - v2) * life;
}

// FUNCTION: USA 0x0006D008
// PROTOTYPE: PREVIEW 0x0045929B identical
// NAME: proto-pdb:default.pdb
float FFlyEmitter::GetRenderSize(float life, float size) const
{
    if (_renderSize) {
        float v1 = (1.0f - life) * _data._lifeSpan[1];
        return _renderSize->Evaluate(v1);
    }
    float v2 = _data._renderSize[0];
    return v2 + (size - v2) * life;
}

// FUNCTION: USA 0x0006D0AC
// PROTOTYPE: PREVIEW 0x0045933F identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::ComputeBounds(AABox& box, float* maxSize) const
{
    if (_pool && _poolHi) {
        FFlyParticle* p = _pool;
        // !FAKE: c1xx front end; neutral loop counter; the PDB lists no counter local
        for (unsigned int v1 = 0; v1 < _poolHi; v1++, p++) {
            if (p->_flags & 1) {
                float size = p->_size;
                if (_data._renderEffects & 0x80)
                    size = GetRenderSize(p->_life, size);
                if (maxSize)
                    *maxSize = size < *maxSize ? *maxSize : size;
                size *= 0.5f;
                // !FAKE: c2 register allocation; pointer local; retail's second induction pointer (esi = p+0x10,
                // p spilled to [ebp-8]); p->_position direct gives one register p (match_one 88.74 -> 100.0, C0216)
                const Vector3* v2 = &p->_position;
                box.AddPoint(Vector3(v2->x - size, v2->y - size, v2->z - size));
                box.AddPoint(Vector3(size + v2->x, size + v2->y, size + v2->z));
            }
        }
    }
}

// FUNCTION: USA 0x0006D38E
// PROTOTYPE: PREVIEW 0x00459621 identical
// NAME: proto-pdb:default.pdb
unsigned int FFlyEmitter::GetRenderTex(float life, unsigned int texid) const
{
    // !FAKE: c2 register allocation; local; removal changes the bytes (the PDB lists no such local)
    float v1;
    if (_renderTex) {
        float v3 = (1.0f - life) * _data._lifeSpan[1];
        v1 = _renderTex->Evaluate(v3);
    } else {
        float v2 = _data._renderTex[0];
        v1 = v2 + (texid + 0.9f - v2) * life;
    }
    return _mm_cvtt_ss2si(_mm_load_ss(&v1));
}

// FUNCTION: USA 0x0006D3F6
// PROTOTYPE: PREVIEW 0x00459689 identical
// NAME: proto-pdb:default.pdb
void FFlyEmitter::initialize()
{
    if (_data._maxParticles > 0) {
        // !FAKE: c2 register allocation; neutral local; block count held in ebx (the PDB lists no such local)
        unsigned int v1 = FFlyManager::FitBlocks(_data._maxParticles);
        _pool = FFlyManager::s_ffmanager->Alloc(v1);
        if (!_pool)
            return;
        // !FAKE: c2 register allocation; neutral pointer local; retail walks the pool in ecx; _pool[i] reloads it (match_one 99.42 -> 100.0)
        FFlyParticle* v2 = _pool;
        // !FAKE: c1xx front end; neutral loop counter; the PDB lists no counter local
        for (unsigned int v3 = 0; v3 < v1 * FFlyManager::PARTICLES_PER_BLOCK; v3++, v2++)
            v2->_flags = 0;
        _poolBlks = v1;
        _poolHi = 0;
        _poolLo = 0;
    }
    _time = 0.0;
    if (_location || _rotation) {
        Vector3 rot(0.0f, 0.0f, 0.0f);
        if (_rotation)
            _rotation->Evaluate(0.0, rot);
        Vector3 loc(0.0f, 0.0f, 0.0f);
        if (_location)
            _location->Evaluate(0.0, loc);
        _matrix.TransformationAboutPivot(rot.x, rot.y, rot.z, loc, _data._pivot);
    }
    _genAcc = 0.0f;
    if (!_spawning)
        return;
    unsigned int nspawn = _data._spawnParticles < _data._maxParticles ? _data._spawnParticles : _data._maxParticles;
    if (nspawn > 0) {
        // !FAKE: c2 register allocation; neutral pointer local; pool cursor held in ebx (the PDB lists no such local)
        FFlyParticle* p = _pool;
        float lifeScale;
        if (_data._lifeSpan[1] > 0.0f)
            lifeScale = 1.0f / _data._lifeSpan[1];
        else
            lifeScale = 1e-7f;
        // !FAKE: c1xx front end; neutral loop counter; the PDB lists no counter local
        for (unsigned int v4 = 0; v4 < nspawn; v4++, p++) {
            p->_flags = FFlyParticle::SPAWNING;
            p->_life = _lifeSpan->Evaluate(0.0) * lifeScale;
            p->_position.Set(0.0f, 0.0f, 0.0f);
            p->_lastPosition.Set(0.0f, 0.0f, 0.0f);
            p->_acceleration.Set(0.0f, 0.0f, 0.0f);
            Vector4 clr;
            _color->Evaluate(0.0, (Vector3&)clr);
            clr.w = _alpha->Evaluate(0.0);
            p->_color = clr.GetARGB();
            // !FAKE: c2 register allocation; neutral local; cvttss2si reads the float from a stack slot
            float v5 = _texId->Evaluate(0.0);
            p->_texId = _mm_cvtt_ss2si(_mm_load_ss(&v5));
            p->_size = _size->Evaluate(0.0);
            p->_pad[0] = p->_pad[1] = 0;
        }
        float param1;
        float param2;
        GetParams(param1, param2);
        _surface->Spawn(*this, _pool, param1, param2, nspawn);
        _live = _poolLo = _poolHi = nspawn;
    }
}

// FUNCTION: USA 0x0006D657
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 92.43, bytes mismatch (cl keeps a zero register in edi for the whole body; retail's esi zero dies after the rotation block)
void FFlyEmitter::Step()
{
    if (!_pool) {
        initialize();
        if (!_pool)
            return;
    }
    _time += _timeStep;
    if (_location || _rotation) {
        Vector3 rot;
        if (_rotation)
            _rotation->Evaluate(_time, rot);
        else
            rot = _data._rotation[0];
        Vector3 loc;
        if (_location)
            _location->Evaluate(_time, loc);
        else
            loc = _data._location[0];
        _matrix.TransformationAboutPivot(rot.x, rot.y, rot.z, loc, _data._pivot);
    }
    if (_forces.size() > 0) {
        {
            int hi = 0;
            // !FAKE: c2 register allocation; neutral pointer local; pool cursor held in ecx (the PDB lists no such local)
            FFlyParticle* v1 = _pool;
            // !FAKE: c1xx front end; neutral loop counter in edx; the PDB lists no counter local
            for (int v2 = 0; v2 < _poolHi; v2++, v1++) {
                if (v1->_flags & FFlyParticle::ALIVE) {
                    v1->_life -= _timeStepScale;
                    if (v1->_life <= 0.0f) {
                        v1->_flags = 0;
                        v1->_life = 0.0f;
                        _live--;
                        if (v2 < _poolLo)
                            _poolLo = v2;
                    } else if (v2 >= hi) {
                        hi = v2 + 1;
                    }
                    v1->_acceleration = _data._constAccel;
                }
            }
            _poolHi = hi;
        }
        // !FAKE: c2 register allocation; neutral iterator local held in esi (the PDB lists no such local)
        for (std::vector<FFlyForce*>::iterator v3 = _forces.begin(); v3 != _forces.end(); ++v3)
            (*v3)->Apply(*this, _pool, _poolHi);
        // !FAKE: c2 register allocation; neutral pointer local; pool cursor held in edx (the PDB lists no such local)
        FFlyParticle* v4 = _pool;
        for (int i = 0; i < _poolHi; i++, v4++) {
            if (v4->_flags & FFlyParticle::ALIVE) {
                Vector3 temp = v4->_position;
                // !FAKE: inliner; hand form of retail's inlined Vector3 operator temps (no PDB free operator, D78);
                // pointer locals for the reference operands, float local for the by-value scale (C0191)
                const Vector3* v5a = &v4->_acceleration;
                float v5s = _timeStepSqr;
                Vector3 v5;
                v5.x = v5a->x * v5s;
                v5.y = v5a->y * v5s;
                v5.z = v5a->z * v5s;
                const Vector3* v6a = &v4->_position;
                const Vector3* v6b = &v4->_lastPosition;
                Vector3 v6;
                v6.x = v6a->x - v6b->x;
                v6.y = v6a->y - v6b->y;
                v6.z = v6a->z - v6b->z;
                Vector3 v7;
                v7.x = v6.x + v5.x;
                v7.y = v6.y + v5.y;
                v7.z = v6.z + v5.z;
                v4->_position.x += v7.x;
                v4->_position.y += v7.y;
                v4->_position.z += v7.z;
                v4->_lastPosition = temp;
            }
        }
    } else {
        unsigned int hi = 0;
        FFlyParticle* p = _pool;
        // !FAKE: c1xx front end; neutral loop counter in edx; the PDB lists no counter local
        for (int v8 = 0; v8 < _poolHi; v8++, p++) {
            if (p->_flags & FFlyParticle::ALIVE) {
                p->_life -= _timeStepScale;
                if (p->_life <= 0.0f) {
                    p->_flags = 0;
                    p->_life = 0.0f;
                    _live--;
                    if (v8 < _poolLo)
                        _poolLo = v8;
                } else if (v8 >= hi) {
                    hi = v8 + 1;
                }
                p->_acceleration = _data._constAccel;
                Vector3 temp = p->_position;
                // !FAKE: inliner; hand form of retail's inlined Vector3 operator temps (no PDB free operator, D78);
                // pointer locals for the reference operands, float local for the by-value scale (C0191)
                const Vector3* v9a = &p->_acceleration;
                float v9s = _timeStepSqr;
                Vector3 v9;
                v9.x = v9a->x * v9s;
                v9.y = v9a->y * v9s;
                v9.z = v9a->z * v9s;
                const Vector3* v10a = &p->_position;
                const Vector3* v10b = &p->_lastPosition;
                Vector3 v10;
                v10.x = v10a->x - v10b->x;
                v10.y = v10a->y - v10b->y;
                v10.z = v10a->z - v10b->z;
                Vector3 v11;
                v11.x = v10.x + v9.x;
                v11.y = v10.y + v9.y;
                v11.z = v10.z + v9.z;
                p->_position.x += v11.x;
                p->_position.y += v11.y;
                p->_position.z += v11.z;
                p->_lastPosition = temp;
            }
        }
        _poolHi = hi;
    }
    if (!_spawning)
        return;
    _genAcc += _rate->Evaluate(_time) * _timeStep;
    // !FAKE: c2 register allocation; neutral local; cvttss2si reads the float from a stack slot
    float v12 = _genAcc;
    int nspawn = _mm_cvtt_ss2si(_mm_load_ss(&v12));
    if (nspawn > 0) {
        int maxSlot = _data._maxParticles < _poolBlks * FFlyManager::PARTICLES_PER_BLOCK ? _data._maxParticles : _poolBlks * FFlyManager::PARTICLES_PER_BLOCK;
        if (_poolLo < maxSlot) {
            FFlyParticle* first = 0;
            int count = 0;
            // !FAKE: c2 register allocation; neutral pointer local; pool cursor held in edi (the PDB lists no such local)
            FFlyParticle* v13 = &_pool[_poolLo];
            float lifeScale;
            if (_data._lifeSpan[1] > 0.0f)
                lifeScale = 1.0f / _data._lifeSpan[1];
            else
                lifeScale = 1e-7f;
            for (int i = _poolLo; i < maxSlot && count < nspawn; i++, v13++) {
                if (!(v13->_flags & FFlyParticle::ALIVE)) {
                    if (!first)
                        first = v13;
                    count++;
                    if (_poolLo <= i)
                        _poolLo = i + 1;
                    if (_poolHi <= i)
                        _poolHi = i + 1;
                    v13->_flags = FFlyParticle::SPAWNING;
                    v13->_life = _lifeSpan->Evaluate(_time) * lifeScale;
                    v13->_position.Set(0.0f, 0.0f, 0.0f);
                    v13->_lastPosition.Set(0.0f, 0.0f, 0.0f);
                    v13->_acceleration.Set(0.0f, 0.0f, 0.0f);
                    Vector4 clr;
                    _color->Evaluate(_time, (Vector3&)clr);
                    clr.w = _alpha->Evaluate(_time);
                    v13->_color = clr.GetARGB();
                    // !FAKE: c2 register allocation; neutral local; cvttss2si reads the float from a stack slot
                    float v14 = _texId->Evaluate(_time);
                    v13->_texId = _mm_cvtt_ss2si(_mm_load_ss(&v14));
                    v13->_size = _size->Evaluate(_time);
                    v13->_pad[0] = v13->_pad[1] = 0;
                }
            }
            if (!first) {
                _poolLo = maxSlot;
                return;
            }
            _genAcc -= (float)count;
            float param1;
            float param2;
            GetParams(param1, param2);
            _live += _surface->Spawn(*this, first, param1, param2, v13 - first);
        }
    }
}
#endif

// FUNCTION: USA 0x0006DBFD
// NAME: proto-pdb:default.pdb
void FFlyEmitter::ClearForces()
{
    for (std::vector<FFlyForce*>::iterator it = _forces.begin(); it != _forces.end(); ++it)
        delete *it;
    _forces.clear();
}

// FUNCTION: USA 0x0006DC2D
// PROTOTYPE: PREVIEW 0x00459EC0 identical
// NAME: proto-pdb:default.pdb
FFlyEmitter::~FFlyEmitter()
{
    if (_pool) {
        FFlyManager::s_ffmanager->Free(_pool, _poolBlks);
        _pool = 0;
    }
    ClearForces();
    delete _surface;
    delete _param1;
    delete _param2;
    delete _location;
    delete _rotation;
    delete _rate;
    delete _lifeSpan;
    delete _angle;
    delete _speed;
    delete _color;
    delete _alpha;
    delete _texId;
    delete _size;
    delete _renderColor;
    delete _renderAlpha;
    delete _renderTex;
    delete _renderSize;
}
