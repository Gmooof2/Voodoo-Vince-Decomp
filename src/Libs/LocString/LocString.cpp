// UNIT: src/Libs/LocString/LocString.cpp OBJ: Xbox\OfficialRelease\LocString.obj OPT: /O1 /Oy
#include "../../include/decomp.h"

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    ~BeepFile();
};

void BeepFree(void* p); // NAME: proto-pdb:default.pdb

namespace NLocString { // NAME: proto-pdb:default.pdb

struct SECTIONHEADER { // NAME: proto-pdb:default.pdb
    char field_0[0x4];                 // 0x00
    unsigned int m_uNumberOfEntries;   // 0x04
};
DECOMP_SIZE_ASSERT(SECTIONHEADER, 0x8);

#pragma pack(push, 1)
struct STRINGENTRY { // NAME: proto-pdb:default.pdb
    unsigned short m_wValue;           // 0x00
    char field_2[0x4];                 // 0x02
};
#pragma pack(pop)
DECOMP_SIZE_ASSERT(STRINGENTRY, 0x6);

struct BLOBDATA { // NAME: proto-pdb:default.pdb
    SECTIONHEADER* m_pshString;        // 0x00
    STRINGENTRY* m_aStringIndex;       // 0x04
    char field_8[0x4];                 // 0x08
    BeepFile* m_blobFile;              // 0x0C
    char field_10[0x4];                // 0x10
};
DECOMP_SIZE_ASSERT(BLOBDATA, 0x14);

}

// FUNCTION: USA 0x00140B86
// PROTOTYPE: PREVIEW 0x0052A454 identical
// NAME: proto-pdb:default.pdb
extern "C" int LocString_GetNumberOfString(void* hblob)
{
    NLocString::BLOBDATA* p = (NLocString::BLOBDATA*)hblob;
    if (p != 0)
        return p->m_pshString->m_uNumberOfEntries;
    return -1;
}

// FUNCTION: USA 0x00140B98
// PROTOTYPE: PREVIEW 0x0052A466 identical
// NAME: proto-pdb:default.pdb
extern "C" int LocString_GetStringID(void* hblob, unsigned int uIndex)
{
    NLocString::BLOBDATA* p = (NLocString::BLOBDATA*)hblob;
    if (p != 0 && uIndex <= p->m_pshString->m_uNumberOfEntries)
        return p->m_aStringIndex[uIndex].m_wValue;
    return -1;
}

// FUNCTION: USA 0x00140BD1
// PROTOTYPE: PREVIEW 0x0052A49F identical
// NAME: proto-pdb:default.pdb
extern "C" void LocString_CloseBLOB(void* hblob)
{
    NLocString::BLOBDATA* p = (NLocString::BLOBDATA*)hblob;
    if (p != 0) {
        if (p->m_pshString)
            BeepFree(p->m_pshString);
        if (p->m_blobFile) {
            delete p->m_blobFile;
            p->m_blobFile = 0;
        }
        BeepFree(p);
    }
}
