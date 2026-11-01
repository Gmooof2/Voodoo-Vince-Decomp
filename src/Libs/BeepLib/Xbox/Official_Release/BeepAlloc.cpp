// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepAlloc.cpp OBJ: Xbox\Official_Release\BeepAlloc.obj OPT: /O1 /Oy /Ob1
#include "../../../../include/decomp.h"

struct UntrackedAllocationHeader { // NAME: proto-pdb:default.pdb
    void* addr;                        // 0x00
    unsigned int size;                 // 0x04
};
DECOMP_SIZE_ASSERT(UntrackedAllocationHeader, 0x8);

struct AllocationRecord { // NAME: proto-pdb:default.pdb
    enum AllocType { // NAME: proto-pdb:default.pdb
        NEW = 0x0,
        ARRAY = 0x1,
        ALLOC = 0x2
    };
    void Print();

    const char* file;                  // 0x00
    int line;                          // 0x04
    unsigned int size;                 // 0x08
    void* addr;                        // 0x0C
    void* alignedAddr;                 // 0x10
    char field_14[0x24 - 0x14];        // 0x14
};
DECOMP_SIZE_ASSERT(AllocationRecord, 0x24);

class AllocationTracker { // NAME: proto-pdb:default.pdb
public:
    static AllocationTracker& GetAllocationTracker();
    AllocationRecord* RemoveRecord(void* addr);
    void SetCheckpoint()
    {
        _lastCheckpoint++;
        _checkpointAllocCount = 0;
    }

    AllocationRecord* _hash[16384];    // 0x00000
    int _allocCount;                   // 0x10000
    int _maxAllocCount;                // 0x10004
    int _totalAllocCount;              // 0x10008
    int _allocSize;                    // 0x1000C
    int _maxAllocSize;                 // 0x10010
    unsigned int _lastCheckpoint;      // 0x10014
    int _checkpointAllocCount;         // 0x10018
    int _checkpointAllocSize;          // 0x1001C
    bool _ignoreAllLeaks;              // 0x10020

private:
    void Clear();
};
DECOMP_SIZE_ASSERT(AllocationTracker, 0x10024);

extern "C" void* __cdecl malloc(unsigned int); // NAME: proto-pdb:default.pdb
extern "C" void __cdecl free(void*); // NAME: proto-pdb:default.pdb
extern "C" void* __cdecl memset(void*, int, unsigned int); // NAME: proto-pdb:default.pdb
#pragma intrinsic(memset)
extern "C" int __cdecl sprintf(char*, const char*, ...); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall OutputDebugStringA(const char*); // NAME: proto-pdb:default.pdb
void* AllocateMemory(unsigned int size, const char* file, int line, AllocationRecord::AllocType type);

// FUNCTION: USA 0x00033BEB
// PROTOTYPE: PREVIEW 0x00422371 identical
// NAME: proto-pdb:default.pdb
UntrackedAllocationHeader* GetAllocationHeader(const void* m)
{
    return (UntrackedAllocationHeader*)m - 1;
}

// FUNCTION: USA 0x00033BF3
// NAME: proto-pdb:default.pdb
void AllocationRecord::Print()
{
    char outputbuf[1024];
    sprintf(outputbuf, "%s(%d) : %d bytes\n", file, line, size);
    OutputDebugStringA(outputbuf);
}

#ifdef NON_MATCHING
// best: match_one objdiff 99.84, bytes match (masked 0); verdict cut-reloc
// FUNCTION: USA 0x00033D78
// PROTOTYPE: PREVIEW 0x004224FE identical
// NAME: proto-pdb:default.pdb
void AllocationTracker::Clear()
{
    if (_allocCount) {
        for (int i = 0; i < 16384; i++) {
            while (_hash[i]) {
                AllocationRecord* r = RemoveRecord(_hash[i]->alignedAddr);
                void* addr = r->addr;
                free(r);
                free(addr);
            }
        }
    }
}
#endif

// FUNCTION: USA 0x00033DBC
// NAME: proto-pdb:default.pdb
void* AllocateMemory(unsigned int size, const char* file, int line, AllocationRecord::AllocType type)
{
    void* p = malloc(size + 12);
    if (!p) {
        // !FAKE: c1xx front end; volatile local; not match-only: the PDB types x and y `volatile int`
        // (pdb_types locals AllocateMemory), the marker is for the d66 lint
        volatile int x = 0;
        volatile int y = 1 / x; // !FAKE: c1xx front end; volatile local; as x above (PDB-typed, d66 lint marker)
    }
    void* v1 = (void*)(((unsigned int)p + 15) & ~7);
    UntrackedAllocationHeader* v2 = GetAllocationHeader(v1);
    v2->size = size;
    v2->addr = p;
    return v1;
}

// FUNCTION: USA 0x00033DF9
// PROTOTYPE: PREVIEW 0x0042257F identical
// NAME: proto-pdb:default.pdb
void FreeMemory(void* mem, AllocationRecord::AllocType type)
{
    free(GetAllocationHeader(mem)->addr);
}

// FUNCTION: USA 0x00033E0C
// PROTOTYPE: PREVIEW 0x00422592 identical
// NAME: proto-pdb:default.pdb
void* BeepMalloc(unsigned int size, const char* file, int line)
{
    return AllocateMemory(size, file, line, AllocationRecord::ALLOC);
}

// FUNCTION: USA 0x00033E23
// NAME: proto-pdb:default.pdb
void* BeepCalloc(unsigned int num, unsigned int size, const char* file, int line)
{
    unsigned int v1 = num * size;
    void* p = AllocateMemory(v1, file, line, AllocationRecord::ALLOC);
    memset(p, 0, v1);
    return p;
}

// FUNCTION: USA 0x00033E5A
// PROTOTYPE: PREVIEW 0x004225E0 identical
// NAME: proto-pdb:default.pdb
void BeepFree(void* p)
{
    if (p)
        FreeMemory(p, AllocationRecord::ALLOC);
}

// FUNCTION: USA 0x00033E6F
// PROTOTYPE: PREVIEW 0x004225F5 identical
// NAME: proto-pdb:default.pdb
void* operator new(unsigned int size)
{
    return AllocateMemory(size, "BeepAlloc.cpp", 800, AllocationRecord::NEW);
}

// FUNCTION: USA 0x00033E88
// PROTOTYPE: PREVIEW 0x0042260E identical
// NAME: proto-pdb:default.pdb
void* operator new(unsigned int size, const char* file, int line)
{
    return AllocateMemory(size, file, line, AllocationRecord::NEW);
}

// FUNCTION: USA 0x00033E9F
// PROTOTYPE: PREVIEW 0x00422625 identical
// NAME: proto-pdb:default.pdb
void operator delete(void* p)
{
    if (p)
        FreeMemory(p, AllocationRecord::NEW);
}

// FUNCTION: USA 0x00033ECB
// PROTOTYPE: PREVIEW 0x00422651 identical
// NAME: proto-pdb:default.pdb
void operator delete[](void* p)
{
    if (p)
        FreeMemory(p, AllocationRecord::ARRAY);
}

// FUNCTION: USA 0x00033F22
// PROTOTYPE: PREVIEW 0x004226A8 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 97.5, bytes match: the cut body carries DIR32 relocs (XBE_FILE_HEADER_00010000) at the 0x10014/0x10018 displacements; no source emits them
void BeepSetMemoryCheckpoint()
{
    AllocationTracker::GetAllocationTracker().SetCheckpoint();
}
#endif
