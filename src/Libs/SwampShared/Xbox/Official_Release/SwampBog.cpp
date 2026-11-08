// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampBog.cpp OBJ: Xbox\Official_Release\SwampBog.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <string.h>

struct BogNode; // NAME: proto-pdb:default.pdb
struct BogVertex; // NAME: proto-pdb:default.pdb

struct D3DResource { // NAME: proto-pdb:default.pdb
    unsigned long Common;              // 0x00
    unsigned long Data;                // 0x04
    unsigned long Lock;                // 0x08
};
DECOMP_SIZE_ASSERT(D3DResource, 0xC);

struct D3DVertexBuffer : public D3DResource { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(D3DVertexBuffer, 0xC);

extern "C" void __stdcall D3DResource_Register(D3DResource*, void*);

struct BOG_HEADER { // NAME: proto-pdb:default.pdb
    unsigned int magic;                // 0x00
    unsigned int version;              // 0x04
    unsigned int totalSize;            // 0x08
    unsigned int numNodes;             // 0x0C
    unsigned int numTriangles;         // 0x10
    unsigned int numPlanes;            // 0x14
    unsigned int numVertices;          // 0x18
    unsigned int offsetVertices;       // 0x1C
    unsigned int offsetPlanes;         // 0x20
    unsigned int offsetRootNode;       // 0x24
    unsigned int maxDepth;             // 0x28
    unsigned int flags;                // 0x2C

    BogNode* GetRootNode() { return (BogNode*)((char*)this + offsetRootNode); }
    const BogVertex* GetVertices() const { return (const BogVertex*)((const char*)this + offsetVertices); }
};
DECOMP_SIZE_ASSERT(BOG_HEADER, 0x30);

class Bog { // NAME: proto-pdb:default.pdb
public:
    Bog(void* fileImage);
    BogNode* GetRoot();
    unsigned int GetMaxTriCount() const;
    unsigned int GetFlags() const;
    const BogVertex* GetVertices() const;

private:
    BOG_HEADER* _bog;                  // 0x00
    D3DVertexBuffer _vb;               // 0x04
};
DECOMP_SIZE_ASSERT(Bog, 0x10);

// FUNCTION: USA 0x000DF3B0
// PROTOTYPE: PREVIEW 0x004CA787 identical
// NAME: proto-pdb:default.pdb
BogNode* Bog::GetRoot()
{
    return _bog ? _bog->GetRootNode() : 0;
}

// FUNCTION: USA 0x000DF3BF
// PROTOTYPE: PREVIEW 0x004CA796 identical
// NAME: proto-pdb:default.pdb
unsigned int Bog::GetMaxTriCount() const
{
    return _bog ? _bog->numTriangles : 0;
}

// FUNCTION: USA 0x000DF3CC
// PROTOTYPE: PREVIEW 0x004CA7A3 identical
// NAME: proto-pdb:default.pdb
unsigned int Bog::GetFlags() const
{
    return _bog ? _bog->flags : 0;
}

// FUNCTION: USA 0x000DF3D9
// PROTOTYPE: PREVIEW 0x004CA7B0 identical
// NAME: proto-pdb:default.pdb
const BogVertex* Bog::GetVertices() const
{
    return _bog ? _bog->GetVertices() : 0;
}

// FUNCTION: USA 0x000DF3E8
// PROTOTYPE: PREVIEW 0x004CA7BF identical
// NAME: proto-pdb:default.pdb
// !FAKE: preprocessor; #pragma intrinsic(memset) compiler directive; retail has the inline stosd memset form
#pragma intrinsic(memset)
Bog::Bog(void* fileImage)
{
    _bog = (BOG_HEADER*)fileImage;
    // !FAKE: c1xx front end; stand-in bool; retail lays the else block first and reloads ecx before GetVertices
    // (if/else forms 76.85, 4.3 T16.c5; 5.2 T12.c16 probe: byte-equal)
    bool v0 = _bog->magic == 0x20474F42 && _bog->version == 2;
    if (!v0) {
        _bog = 0;
    } else {
        memset(&_vb, 0, sizeof(_vb));
        _vb.Common = 1;
        D3DResource_Register(&_vb, (void*)GetVertices());
    }
}
