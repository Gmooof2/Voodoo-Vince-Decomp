// UNIT: src/Apps/Vince/core/LoadScreens.cpp OBJ: core\XBox\OfficialRelease\LoadScreens.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetSessionTime() const { return _sessionTimePublic; }

    char field_0[0xC];                 // 0x00
    float _sessionTimePublic;          // 0x0C
};

struct Matrix { // NAME: proto-pdb:default.pdb
    Matrix() {}

    char field_0[0x40];                // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void ClearScreen(unsigned int, bool, bool, bool) const;
};

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    float UpdateScene();
    float RenderScene();
};

class SwampCamera; // NAME: proto-pdb:default.pdb

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    SwampRenderer& GetRenderer() const { return *_renderer; }
    SwampScene& GetScene() const { return *_scene; }
    SwampCamera* GetCamera() const { return _currentCamera; }
    SwampCamera* SetCamera(SwampCamera* camera) { _currentCamera = camera; return camera; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    SwampScene* _scene;                // 0x0C
    char field_10[0x40 - 0x10];        // 0x10
    SwampCamera* _currentCamera;       // 0x40
    char field_44[0x68 - 0x44];        // 0x44
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class GameCam; // NAME: proto-pdb:default.pdb

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    void Update() const;
    GameCam* GetBlendedCamera() const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

struct AABox;                // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb

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
};

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class HUDUI : public UINode { // NAME: proto-pdb:default.pdb
public:
    static HUDUI& Instance() { return *_self; }
private:
    static HUDUI* _self;
};

struct D3DResource; // NAME: proto-pdb:default.pdb

class CXBPackedResource { // NAME: proto-pdb:default.pdb
public:
    void* GetData(const char* strName) const;
    D3DResource* GetResource(const char* strName) const;
    D3DResource* RegisterResource(D3DResource* pResource) const;
};

class TextureResource; // NAME: proto-pdb:default.pdb

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _Resource != 0; }
    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

class LoadScreen { // NAME: proto-pdb:default.pdb
public:
    virtual ~LoadScreen();                                         // vtable+0x00
    virtual void RenderGameScene() = 0;                            // vtable+0x04
    virtual void RenderLoadScene() = 0;                            // vtable+0x08

    void RenderPersisted() const;
    bool HasPersistedImage() const { return _persistedScreen.IsValid(); }

    char field_4[0x2C - 0x4];          // 0x04
    ResourceHandle<TextureResource> _persistedScreen; // 0x2C
};
DECOMP_SIZE_ASSERT(LoadScreen, 0x30);

class VinceLoadScreen : public LoadScreen { // NAME: proto-pdb:default.pdb
public:
    virtual ~VinceLoadScreen();        // vtable+0x00
    virtual void RenderGameScene();    // vtable+0x04
    virtual void RenderLoadScene();    // vtable+0x08
    void SetToSaving();
    void SetToLoading();

    char field_30[0xC5 - 0x30];        // 0x30
    bool _bSaving;                     // 0xC5
    char field_C6[0x2];                // 0xC6
    float _saveMsgTimeStarted;         // 0xC8
    char field_CC[0xD8 - 0xCC];        // 0xCC
};
DECOMP_SIZE_ASSERT(VinceLoadScreen, 0xD8);

// FUNCTION: USA 0x00125528
// PROTOTYPE: PREVIEW 0x0050E053 identical
// NAME: proto-pdb:default.pdb
void VinceLoadScreen::SetToLoading()
{
    _bSaving = false;
}

// FUNCTION: USA 0x00125540
// PROTOTYPE: PREVIEW 0x0050E06B identical
// NAME: proto-pdb:default.pdb
void VinceLoadScreen::SetToSaving()
{
    _saveMsgTimeStarted = g_swamp->GetInput().GetSessionTime();
    _bSaving = true;
}

// FUNCTION: USA 0x00125558
// PROTOTYPE: PREVIEW 0x0050E083 identical
// NAME: proto-pdb:default.pdb
D3DResource* CXBPackedResource::GetResource(const char* strName) const
{
    return RegisterResource((D3DResource*)GetData(strName));
}

// FUNCTION: USA 0x001255DB
// PROTOTYPE: PREVIEW 0x0050E106 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 96.19, bytes mismatch (retail reloads g_swamp after the GetCamera test; ours reuses eax)
void VinceLoadScreen::RenderGameScene()
{
    if (HasPersistedImage()) {
        RenderPersisted();
    } else {
        g_swamp->GetRenderer().ClearScreen(0, true, true, true);
        if (g_swamp->GetCamera()) {
            g_swamp->GetScene().UpdateScene();
            g_CameraManager->Update();
            // !FAKE: c2 register allocation; pointer local; retail loads g_swamp into esi before GetBlendedCamera
            Swamp* v0 = g_swamp;
            v0->SetCamera((SwampCamera*)g_CameraManager->GetBlendedCamera());
            HUDUI::Instance().Update();
            g_swamp->GetScene().RenderScene();
            HUDUI::Instance().Render(Matrix());
        }
    }
}
#endif
