// UNIT: src/Apps/Vince/core/xbresource.cpp OBJ: core\XBox\OfficialRelease\xbresource.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
#include <string.h>

struct XBRESOURCE { // NAME: proto-pdb:default.pdb
    char* strName;                     // 0x00
    unsigned long dwOffset;            // 0x04
};
DECOMP_SIZE_ASSERT(XBRESOURCE, 0x8);

class CXBPackedResource { // NAME: proto-pdb:default.pdb
public:
    CXBPackedResource();
    void* GetData(const char* strName) const;

    unsigned char* m_pSysMemData;      // 0x00
    unsigned long m_dwSysMemDataSize;  // 0x04
    unsigned char* m_pVidMemData;      // 0x08
    unsigned long m_dwVidMemDataSize;  // 0x0C
    XBRESOURCE* m_pResourceTags;       // 0x10
    unsigned long m_dwNumResourceTags; // 0x14
};
DECOMP_SIZE_ASSERT(CXBPackedResource, 0x18);

// FUNCTION: USA 0x00125249
// PROTOTYPE: PREVIEW 0x0050DD74 identical
// NAME: proto-pdb:default.pdb
CXBPackedResource::CXBPackedResource()
{
    m_pSysMemData = 0;
    m_dwSysMemDataSize = 0;
    m_pVidMemData = 0;
    m_dwVidMemDataSize = 0;
    m_pResourceTags = 0;
    m_dwNumResourceTags = 0;
}

// FUNCTION: USA 0x0012525F
// PROTOTYPE: PREVIEW 0x0050DD8A identical
// NAME: proto-pdb:default.pdb
void* CXBPackedResource::GetData(const char* strName) const
{
    if (m_pResourceTags == 0 || strName == 0)
        return 0;
    for (unsigned long i = 0; m_pResourceTags[i].strName; i++) {
        if (!_stricmp(strName, m_pResourceTags[i].strName)) // src proto-pdb:default.pdb _stricmp
            return &m_pSysMemData[m_pResourceTags[i].dwOffset];
    }
    return 0;
}
