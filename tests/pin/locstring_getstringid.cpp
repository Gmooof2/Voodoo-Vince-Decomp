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

extern "C" int LocString_GetStringID(void* hBlob, unsigned int id)
{
    NLocString::BLOBDATA* blob = (NLocString::BLOBDATA*)hBlob;
    if (blob != 0 && id <= blob->m_pshString->m_uNumberOfEntries)
        return blob->m_aStringIndex[id].m_wValue;
    return -1;
}
