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
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

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
    char field_44[0x34];               // 0x44
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
    char field_124[0x28];              // 0x124
};
DECOMP_SIZE_ASSERT(FFlyData, 0x14C);

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    iterator begin() { return _Myfirst; }
    iterator end() { return _Mylast; }
    void clear() { _Tidy(); }

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
struct FFlyParticle; // NAME: proto-pdb:default.pdb

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
    void ClearForces();

    char field_0[0x10];                // 0x00
    double _time;                      // 0x10
    char field_18[0x30 - 0x18];        // 0x18
    unsigned int _poolHi;              // 0x30
    char field_34[0x8];                // 0x34
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
    char field_94[0x114 - 0x94];       // 0x94
    FFlyData _data;                    // 0x114
};
DECOMP_SIZE_ASSERT(FFlyEmitter, 0x260);

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

// FUNCTION: USA 0x0006DBFD
// NAME: proto-pdb:default.pdb
void FFlyEmitter::ClearForces()
{
    for (std::vector<FFlyForce*>::iterator it = _forces.begin(); it != _forces.end(); ++it)
        delete *it;
    _forces.clear();
}
