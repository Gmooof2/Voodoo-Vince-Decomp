// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepZlib.cpp OBJ: Xbox\Official_Release\BeepZlib.obj OPT: /O1 /Oy

typedef void *voidpf;

void BeepFree(void *ptr); // NAME: proto-pdb:default.pdb
void *BeepCalloc(unsigned int num, unsigned int size, const char *file, int line); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0003918E
// PROTOTYPE: PREVIEW 0x00427885 identical
// NAME: proto-pdb:default.pdb
extern "C" voidpf zcalloc(voidpf opaque, unsigned items, unsigned size)
{
    return BeepCalloc(items, size, "BeepZlib.cpp", 35);
}

// FUNCTION: USA 0x000391A6
// PROTOTYPE: PREVIEW 0x0042789D identical
// NAME: proto-pdb:default.pdb
extern "C" void zcfree(voidpf opaque, voidpf ptr)
{
    BeepFree(ptr);
}
