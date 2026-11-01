// gate fixture (1.5 T2): LocString.obj unit, copied from tests/pin/locstring_getstringid.cpp; built with the unit opt `/O1 /Oy`.
//
// compiler-pin test (1.4 T9): LocString_GetStringID, retail 0x00140B98, 34 B (fpo).
// Types per prototype default.pdb (docs/evidence/compiler_pin.md): NLocString::BLOBDATA size 0x14,
// SECTIONHEADER size 0x8, STRINGENTRY size 0x6 (packed); cdecl C-linkage int (void*, uint), no callees.

class BeepFile;

namespace NLocString {

struct SECTIONHEADER {
    unsigned long m_dwSectionSize;
    unsigned int m_uNumberOfEntries;
};

#pragma pack(push, 1)
struct STRINGENTRY {
    unsigned short m_wValue;
    unsigned long m_dwOfs;
};
#pragma pack(pop)

struct BLOBDATA {
    SECTIONHEADER* m_pshString;
    STRINGENTRY* m_aStringIndex;
    unsigned short* m_pStringData;
    BeepFile* m_blobFile;
    long m_lStringDataInFile;
};

}  // namespace NLocString

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
