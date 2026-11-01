// UNIT: src/Apps/Vince/scripts/Carnival/ShootingCam.cpp OBJ: Xbox\OfficialRelease\ShootingCam.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct AABox;      // NAME: proto-pdb:default.pdb
struct Matrix;     // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb
struct Vector3;    // NAME: proto-pdb:default.pdb

extern "C" void __stdcall D3DDevice_SetVertexShader(unsigned long); // src proto-pdb:default.pdb D3DDevice_SetVertexShader

class AnimationControl { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }
    void AddRef() { _RefCount++; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);

    T* _Resource;                      // 0x00
};

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
};

class Sprite { // NAME: proto-pdb:default.pdb
public:
    Vector2 _loc;                      // 0x00
    int _w;                            // 0x08
    int _h;                            // 0x0C
    Vector2 _scale;                    // 0x10
    ResourceHandle<TextureResource> _texture; // 0x18
    char field_1C[0x4C - 0x1C];        // 0x1C
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    long SetVertexShader(unsigned long handle);

    char field_0[0x2C];                // 0x00
    int _screenWidth;                  // 0x2C
    int _screenHeight;                 // 0x30
    char field_34[0x4C - 0x34];        // 0x34
    ResourceHandle<VertexShaderResource> _CurrentVertexShader; // 0x4C
    const void* _CurrentVertexDeclaration; // 0x50
    char field_54[0x38C - 0x54];       // 0x54
};
DECOMP_SIZE_ASSERT(SwampRenderer, 0x38C);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x8];                 // 0x00
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x68 - 0xC];          // 0x0C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

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

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// FUNCTION: USA 0x000576B9
// PROTOTYPE: PREVIEW 0x00443F34 identical
// NAME: proto-pdb:default.pdb
class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xC0 - 0xA4];        // 0xA4
    ResourceHandle<AnimationControl> _currentPath; // 0xC0
    char field_C4[0x10C - 0xC4];       // 0xC4
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    char field_10C[0x11C - 0x10C];     // 0x10C
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xD4 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

// FUNCTION: USA 0x0005790F
// PROTOTYPE: PREVIEW 0x00444195 identical
// NAME: proto-pdb:default.pdb
class OverheatUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    void SetOverheatIndicator(bool o) { _overheat = o; }
    void SetHeatIndicator(float hl) { _heatLevel = hl; }

    bool _overheat;                    // 0xD4
    float _heatLevel;                  // 0xD8
    Sprite _iconMask;                  // 0xDC
    Sprite _icon;                      // 0x128
};
DECOMP_SIZE_ASSERT(OverheatUI, 0x174);

class ShootingUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    void SetAimPosition(float x, float y) { _x = x; _y = y; }

    float _x;                          // 0xD4
    float _y;                          // 0xD8
    ResourceHandle<TextureResource> _tex; // 0xDC
};
DECOMP_SIZE_ASSERT(ShootingUI, 0xE0);

class ShootingCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    virtual void Invalidate();
    void AimCenter();
    void SetOverheatIndicator(bool o);
    void SetHeatIndicator(float hl);

    float _x;                          // 0x11C
    float _y;                          // 0x120
    char field_124[0x140 - 0x124];     // 0x124
    ShootingUI* _ui;                   // 0x140
    OverheatUI* _heatUI;               // 0x144
};
DECOMP_SIZE_ASSERT(ShootingCam, 0x148);

// FUNCTION: USA 0x00056FA0
// PROTOTYPE: PREVIEW 0x00443915 identical
// NAME: proto-pdb:default.pdb
void ShootingCam::Invalidate()
{
    if (_ui) {
        _ui->Invalidate();
        _ui = 0;
    }
    if (_heatUI) {
        _heatUI->Invalidate();
        _heatUI = 0;
    }
    GameCam::Invalidate();
}

// FUNCTION: USA 0x00056FD5
// PROTOTYPE: PREVIEW 0x0044394A identical
// NAME: proto-pdb:default.pdb
void ShootingCam::SetOverheatIndicator(bool o)
{
    if (_heatUI)
        _heatUI->SetOverheatIndicator(o);
}

// FUNCTION: USA 0x00056FEC
// PROTOTYPE: PREVIEW 0x00443961 identical
// NAME: proto-pdb:default.pdb
void ShootingCam::SetHeatIndicator(float hl)
{
    if (_heatUI)
        _heatUI->SetHeatIndicator(hl);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1SwampCamera@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_SwampCamera_dtor(SwampCamera* p)
{
    p->SwampCamera::~SwampCamera();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1OverheatUI@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_OverheatUI_dtor(OverheatUI* p)
{
    p->OverheatUI::~OverheatUI();
}

// FUNCTION: USA 0x00057200
// PROTOTYPE: PREVIEW 0x00443B47 identical
// NAME: proto-pdb:default.pdb
void ShootingCam::AimCenter()
{
    SwampRenderer* renderer = g_swamp->_renderer;
    _x = renderer->_screenWidth * 0.5f;
    _y = renderer->_screenHeight * 0.5f;
    if (_ui)
        _ui->SetAimPosition(_x, _y);
}

#ifdef NON_MATCHING
template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

// FUNCTION: USA 0x00057242
// PROTOTYPE: PREVIEW 0x00443B89 identical
// NAME: proto-pdb:default.pdb
long SwampRenderer::SetVertexShader(unsigned long handle)
{
    _CurrentVertexShader = ResourceHandle<VertexShaderResource>();
    _CurrentVertexDeclaration = 0;
    D3DDevice_SetVertexShader(handle);
    return 0;
}
#endif
