// UNIT: src/Apps/Vince/scripts/Bayou/SwitchBoat.cpp OBJ: Xbox\OfficialRelease\SwitchBoat.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
public:
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AABox { // NAME: proto-pdb:default.pdb
public:
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

class RegionTrigger { // NAME: proto-pdb:default.pdb
public:
    virtual const AABox& GetBounds() const;

    char field_4[0xA4 - 0x4];          // 0x04
    int _shape;                        // 0xA4 // src proto-pdb:default.pdb RegionShape
    AABox _bounds;                     // 0xA8
    char field_C0[0x124 - 0xC0];       // 0xC0
};
DECOMP_SIZE_ASSERT(RegionTrigger, 0x124);

// FUNCTION: USA 0x000220BF
// PROTOTYPE: PREVIEW 0x004110A0 identical
// NAME: proto-pdb:default.pdb
const AABox& RegionTrigger::GetBounds() const
{
    return _bounds;
}
