// UNIT: src/Libs/FireFlyLib/Xbox/Official_Release/FFlyManager.cpp OBJ: Xbox\Official_Release\FFlyManager.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct FFlyParticle; // NAME: proto-pdb:default.pdb

extern "C" void* __stdcall VirtualAlloc(void* lpAddress, unsigned long dwSize, unsigned long flAllocationType, // src proto-pdb:default.pdb VirtualAlloc
                                        unsigned long flProtect);
extern "C" int __stdcall VirtualFree(void* lpAddress, unsigned long dwSize, unsigned long dwFreeType); // src proto-pdb:default.pdb VirtualFree

class FFlyManager { // NAME: proto-pdb:default.pdb
public:
    FFlyManager();
    FFlyParticle* Alloc(unsigned int nblocks);
    void Free(FFlyParticle* block, unsigned int nblocks);
    static unsigned int FitBlocks(unsigned int particles);

    unsigned int _nMaxAllocBlks;       // 0x00
    unsigned int _nAllocBlks;          // 0x04
private:
    static FFlyManager* s_ffmanager;
};
DECOMP_SIZE_ASSERT(FFlyManager, 0x8);

// FUNCTION: USA 0x0006DE22
// PROTOTYPE: PREVIEW 0x0045A0B5 identical
// NAME: proto-pdb:default.pdb
FFlyManager::FFlyManager()
{
    _nMaxAllocBlks = 0;
    _nAllocBlks = 0;
    s_ffmanager = this;
}

// FUNCTION: USA 0x0006DE31
// PROTOTYPE: PREVIEW 0x0045A0C4 identical
// NAME: proto-pdb:default.pdb
FFlyParticle* FFlyManager::Alloc(unsigned int nblocks)
{
    FFlyParticle* block = (FFlyParticle*)VirtualAlloc(0, nblocks << 12, 0x1000, 4);
    if (block) {
        _nAllocBlks += nblocks;
        _nMaxAllocBlks = _nMaxAllocBlks > _nAllocBlks ? _nMaxAllocBlks : _nAllocBlks;
    }
    return block;
}

// FUNCTION: USA 0x0006DE66
// PROTOTYPE: PREVIEW 0x0045A0F9 identical
// NAME: proto-pdb:default.pdb
void FFlyManager::Free(FFlyParticle* block, unsigned int nblocks)
{
    if (block) {
        _nAllocBlks -= nblocks;
        VirtualFree(block, 0, 0x8000);
    }
}

// FUNCTION: USA 0x0006DE87
// PROTOTYPE: PREVIEW 0x0045A11A identical
// NAME: proto-pdb:default.pdb
unsigned int FFlyManager::FitBlocks(unsigned int particles)
{
    if (particles == 0)
        return 1;
    return (particles + 63) >> 6;
}
