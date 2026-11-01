// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampLightning.cpp OBJ: Xbox\Official_Release\SwampLightning.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

class SwampLightningFlash { // NAME: proto-pdb:default.pdb
public:
    void SetScreenLocation(float x, float y);

    char field_0[0xC4];                // 0x00
    float _x;                          // 0xC4
    float _y;                          // 0xC8
    char field_CC[0xD8 - 0xCC];        // 0xCC
};
DECOMP_SIZE_ASSERT(SwampLightningFlash, 0xD8);

// FUNCTION: USA 0x000D08CA
// PROTOTYPE: PREVIEW 0x004BBB85 identical
// NAME: proto-pdb:default.pdb
void SwampLightningFlash::SetScreenLocation(float x, float y)
{
    _x = x;
    _y = y;
}
