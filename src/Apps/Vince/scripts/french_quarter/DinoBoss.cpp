// UNIT: src/Apps/Vince/scripts/french_quarter/DinoBoss.cpp OBJ: Xbox\OfficialRelease\DinoBoss.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampScript();                                        // vtable+0x00

    char field_4[0x48];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    char field_4C[0x70];               // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class DinoBossMain : public CritterMain { // NAME: proto-pdb:default.pdb
private:
    bool VinceOnMuseumFloor();
    bool VinceOnUpperFloor();

    char field_BC[0x4C];               // 0xBC
    int _wheresVince;                  // 0x108
    float _vinceFloorTimer;            // 0x10C
    char field_110[0x10];              // 0x110
};
DECOMP_SIZE_ASSERT(DinoBossMain, 0x120);

// FUNCTION: USA 0x000875A7
// PROTOTYPE: PREVIEW 0x004732FA identical
// NAME: proto-pdb:default.pdb
bool DinoBossMain::VinceOnMuseumFloor()
{
    return _vinceFloorTimer < 2.0f;
}

// FUNCTION: USA 0x000875C1
// PROTOTYPE: PREVIEW 0x00473314 identical
// NAME: proto-pdb:default.pdb
bool DinoBossMain::VinceOnUpperFloor()
{
    return _wheresVince == 2;
}
