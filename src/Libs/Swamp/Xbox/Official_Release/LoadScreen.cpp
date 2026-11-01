// UNIT: src/Libs/Swamp/Xbox/Official_Release/LoadScreen.cpp OBJ: Xbox\Official_Release\LoadScreen.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

class TextureResource; // NAME: proto-pdb:default.pdb

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _Resource != 0; }
    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
}

class Swamp { // NAME: proto-pdb:default.pdb
public:
    void BeginScene();
    void EndScene();
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class LoadScreen { // NAME: proto-pdb:default.pdb
public:
    virtual ~LoadScreen();                                         // vtable+0x00
    virtual void RenderGameScene() = 0;                            // vtable+0x04
    virtual void RenderLoadScene() = 0;                            // vtable+0x08
    virtual void Update();                                         // vtable+0x10
    virtual void Update(bool, float);                              // vtable+0x0C
    virtual void EndOfTransition(bool);                            // vtable+0x14

    void DisplayLoadScreen();
    void RenderPersisted() const;
    bool HasPersistedImage() const { return _persistedScreen.IsValid(); }

    float _startDisplayTime;           // 0x04
    float _minimumDisplayTime;         // 0x08
    float _loadTime;                   // 0x0C
    float _loadRate;                   // 0x10
    char field_14[0x2C - 0x14];        // 0x14
    ResourceHandle<TextureResource> _persistedScreen; // 0x2C
private:
    void RenderScreen(float, bool) const;
};
DECOMP_SIZE_ASSERT(LoadScreen, 0x30);

// FUNCTION: USA 0x000DC11B
// PROTOTYPE: PREVIEW 0x004C7653 identical
// NAME: proto-pdb:default.pdb
void LoadScreen::DisplayLoadScreen()
{
    double time = BeepLib::GetTime();
    if (_loadTime > 0.0f && _loadTime + _loadRate > time)
        return;
    _loadTime = time;
    Update();
    g_swamp->BeginScene();
    RenderLoadScene();
    g_swamp->EndScene();
}

// FUNCTION: USA 0x000DC584
// PROTOTYPE: PREVIEW 0x004C7ABC identical
// NAME: proto-pdb:default.pdb
void LoadScreen::RenderPersisted() const
{
    if (HasPersistedImage())
        RenderScreen(1.0f, true);
}
