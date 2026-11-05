// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepRand.cpp OBJ: Xbox\Official_Release\BeepRand.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

class QuickRand { // NAME: proto-pdb:default.pdb
public:
    unsigned int GetRandom()
    {
        unsigned __int64 tmp = (unsigned __int64)_Mul * 0x7F13AC69 + _Carry;
        _Mul = (unsigned int)tmp;
        _Carry = (unsigned int)(tmp >> 32);
        return (unsigned int)tmp;
    }

private:
    unsigned int _Mul;                 // 0x00
    unsigned int _Carry;               // 0x04
};
DECOMP_SIZE_ASSERT(QuickRand, 0x8);

class BeepState { // NAME: proto-pdb:default.pdb
public:
    QuickRand* GetRNG() { return _RNG; }

private:
    char field_0[0x40];                // 0x00
    QuickRand* _RNG;                   // 0x40
    char field_44[0xC14];              // 0x44
};
DECOMP_SIZE_ASSERT(BeepState, 0xC58);

extern BeepState beepState; // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00035E1F
// PROTOTYPE: PREVIEW 0x0042458A identical
// NAME: proto-pdb:default.pdb
unsigned int Random()
{
    return beepState.GetRNG()->GetRandom();
}

}
