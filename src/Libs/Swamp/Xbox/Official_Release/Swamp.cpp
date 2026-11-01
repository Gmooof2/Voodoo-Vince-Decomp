// UNIT: src/Libs/Swamp/Xbox/Official_Release/Swamp.cpp OBJ: Xbox\Official_Release\Swamp.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
}

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    void Update();
};

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    void BeginScene();
};

class SwampDebugGeometry { // NAME: proto-pdb:default.pdb
public:
    void Update();
};

class BSP { // NAME: proto-pdb:default.pdb
public:
    ~BSP();
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    void SetGlobalBSP(BSP* bsp);
    void BeginScene();
    void MarkSceneRenderingStartTime();

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x4];                 // 0x0C
    SwampDebugGeometry* _debugGeometry; // 0x10
    char field_14[0x30];               // 0x14
    BSP* _bsp;                         // 0x44
    double _timeFrameBegan;            // 0x48
    double _timeSceneRenderingBegan;   // 0x50
    char field_58[0x8];                // 0x58
    unsigned int _frameID;             // 0x60
    char field_64[0x4];                // 0x64
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

// FUNCTION: USA 0x000AD32B
// PROTOTYPE: PREVIEW 0x004981D1 identical
// NAME: proto-pdb:default.pdb
void Swamp::MarkSceneRenderingStartTime()
{
    _timeSceneRenderingBegan = BeepLib::GetTime();
}

// FUNCTION: USA 0x000AD338
// PROTOTYPE: PREVIEW 0x004981DE identical
// NAME: proto-pdb:default.pdb
void Swamp::SetGlobalBSP(BSP* bsp)
{
    delete _bsp;
    _bsp = bsp;
}

// FUNCTION: USA 0x000AD5B1
// NAME: proto-pdb:default.pdb
void Swamp::BeginScene()
{
    _timeFrameBegan = BeepLib::GetTime();
    _frameID++;
    _input->Update();
    _debugGeometry->Update();
    _renderer->BeginScene();
}
