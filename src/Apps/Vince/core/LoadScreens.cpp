// UNIT: src/Apps/Vince/core/LoadScreens.cpp OBJ: core\XBox\OfficialRelease\LoadScreens.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetSessionTime() const { return _sessionTimePublic; }

    char field_0[0xC];                 // 0x00
    float _sessionTimePublic;          // 0x0C
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct D3DResource; // NAME: proto-pdb:default.pdb

class CXBPackedResource { // NAME: proto-pdb:default.pdb
public:
    void* GetData(const char* strName) const;
    D3DResource* GetResource(const char* strName) const;
    D3DResource* RegisterResource(D3DResource* pResource) const;
};

class VinceLoadScreen { // NAME: proto-pdb:default.pdb
public:
    virtual ~VinceLoadScreen();        // vtable+0x00
    void SetToSaving();
    void SetToLoading();

    char field_4[0xC5 - 0x4];          // 0x04
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
