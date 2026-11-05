// UNIT: src/Apps/Vince/core/xbresource.cpp OBJ: core\XBox\OfficialRelease\xbresource.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
#include <string.h>
#pragma intrinsic(memcpy) // src proto-pdb:default.pdb memcpy

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[7];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    BeepFile();
    ~BeepFile();
    bool Open(const SwampString& filename);
    bool Read(void* buffer, unsigned int size);

    char field_0[0x20];                // 0x00
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

long XBUtil_FindMediaFile(char* strPath, const char* strFilename); // NAME: proto-pdb:default.pdb

struct XPR_HEADER { // NAME: proto-pdb:default.pdb
    unsigned long dwMagic;             // 0x00
    unsigned long dwTotalSize;         // 0x04
    unsigned long dwHeaderSize;        // 0x08
};
DECOMP_SIZE_ASSERT(XPR_HEADER, 0xC);

struct XBRESOURCE { // NAME: proto-pdb:default.pdb
    char* strName;                     // 0x00
    unsigned long dwOffset;            // 0x04
};
DECOMP_SIZE_ASSERT(XBRESOURCE, 0x8);

class CXBPackedResource { // NAME: proto-pdb:default.pdb
public:
    CXBPackedResource();
    long Create(const void* mem, unsigned long dwNumResourceTags, XBRESOURCE* pResourceTags);
    long Create(const char* strFilename, unsigned long dwNumResourceTags, XBRESOURCE* pResourceTags);
    void Destroy();
    void* GetData(const char* strName) const;

    unsigned char* m_pSysMemData;      // 0x00
    unsigned long m_dwSysMemDataSize;  // 0x04
    unsigned char* m_pVidMemData;      // 0x08
    unsigned long m_dwVidMemDataSize;  // 0x0C
    XBRESOURCE* m_pResourceTags;       // 0x10
    unsigned long m_dwNumResourceTags; // 0x14
};
DECOMP_SIZE_ASSERT(CXBPackedResource, 0x18);

void operator delete[](void* p);
void* operator new[](unsigned int size, const char* file, int line);
extern "C" void* __stdcall D3D_AllocContiguousMemory(unsigned long Size, unsigned long Alignment); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3D_FreeContiguousMemory(void* pMemory); // NAME: proto-pdb:default.pdb
extern unsigned int s_totalVideoMemoryAllocated; // NAME: proto-pdb:default.pdb

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

// FUNCTION: USA 0x001252A2
// PROTOTYPE: PREVIEW 0x0050DDCD identical
// NAME: proto-pdb:default.pdb
long CXBPackedResource::Create(const void* mem, unsigned long dwNumResourceTags, XBRESOURCE* pResourceTags)
{
    XPR_HEADER xprh;
    memcpy(&xprh, mem, sizeof(XPR_HEADER));
    mem = (const unsigned char*)mem + sizeof(XPR_HEADER);

    int bHasResourceOffsetsTable;
    if (xprh.dwMagic == 0x30525058)
        bHasResourceOffsetsTable = 0;
    else if (xprh.dwMagic == 0x31525058)
        bHasResourceOffsetsTable = 1;
    else
        return (long)0x80070057;

    m_dwSysMemDataSize = xprh.dwHeaderSize - sizeof(XPR_HEADER);
    m_dwVidMemDataSize = xprh.dwTotalSize - xprh.dwHeaderSize;

    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    m_pSysMemData = new("\\beep\\vendor\\Microsoft\\XbCommon\\xbresource.cpp", 217) unsigned char[m_dwSysMemDataSize];
    m_pVidMemData = (unsigned char*)D3D_AllocContiguousMemory(m_dwVidMemDataSize, 0x80);
    s_totalVideoMemoryAllocated += m_dwVidMemDataSize;

    memcpy(m_pSysMemData, mem, m_dwSysMemDataSize);
    memcpy(m_pVidMemData, (const unsigned char*)mem + m_dwSysMemDataSize, m_dwVidMemDataSize);

    if (bHasResourceOffsetsTable) {
        m_dwNumResourceTags = *(unsigned long*)m_pSysMemData;
        m_pResourceTags = (XBRESOURCE*)(m_pSysMemData + 4);
        for (unsigned long i = 0; i < m_dwNumResourceTags; i++)
            m_pResourceTags[i].strName = (char*)((unsigned long)m_pResourceTags[i].strName + (unsigned long)m_pSysMemData);
    }

    if (dwNumResourceTags || pResourceTags) {
        m_pResourceTags = pResourceTags;
        m_dwNumResourceTags = dwNumResourceTags;
    }
    return 0;
}

// FUNCTION: USA 0x0012539A
// PROTOTYPE: PREVIEW 0x0050DEC5 identical
// NAME: proto-pdb:default.pdb
void CXBPackedResource::Destroy()
{
    if (m_pSysMemData != 0)
        delete[] m_pSysMemData;
    m_pSysMemData = 0;
    m_dwSysMemDataSize = 0;
    if (m_pVidMemData != 0) {
        D3D_FreeContiguousMemory(m_pVidMemData);
        s_totalVideoMemoryAllocated -= m_dwVidMemDataSize;
    }
    m_pVidMemData = 0;
    m_dwVidMemDataSize = 0;
    m_pResourceTags = 0;
    m_dwNumResourceTags = 0;
}

// FUNCTION: USA 0x001253DC
// PROTOTYPE: PREVIEW 0x0050DF07 identical
// NAME: proto-pdb:default.pdb
long CXBPackedResource::Create(const char* strFilename, unsigned long dwNumResourceTags, XBRESOURCE* pResourceTags)
{
    char strResourcePath[512];
    if (XBUtil_FindMediaFile(strResourcePath, strFilename) < 0)
        return (long)0x80004005;

    BeepFile file;
    if (!file.Open(strResourcePath))
        return (long)0x80004005;

    XPR_HEADER xprh;
    file.Read(&xprh, sizeof(XPR_HEADER));

    // !FAKE: c1xx front end; stand-in local; the PDB records no local for the offsets-table flag (edi in retail)
    int v1;
    if (xprh.dwMagic == 0x30525058)
        v1 = 0;
    else if (xprh.dwMagic == 0x31525058)
        v1 = 1;
    else
        return (long)0x80070057;

    m_dwSysMemDataSize = xprh.dwHeaderSize - sizeof(XPR_HEADER);
    m_dwVidMemDataSize = xprh.dwTotalSize - xprh.dwHeaderSize;

    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    m_pSysMemData = new("\\beep\\vendor\\Microsoft\\XbCommon\\xbresource.cpp", 150) unsigned char[m_dwSysMemDataSize];
    m_pVidMemData = (unsigned char*)D3D_AllocContiguousMemory(m_dwVidMemDataSize, 0x80);
    s_totalVideoMemoryAllocated += m_dwVidMemDataSize;

    file.Read(m_pSysMemData, m_dwSysMemDataSize);
    file.Read(m_pVidMemData, m_dwVidMemDataSize);

    if (v1) {
        m_dwNumResourceTags = *(unsigned long*)m_pSysMemData;
        m_pResourceTags = (XBRESOURCE*)(m_pSysMemData + 4);
        for (unsigned long i = 0; i < m_dwNumResourceTags; i++)
            m_pResourceTags[i].strName = (char*)((unsigned long)m_pResourceTags[i].strName + (unsigned long)m_pSysMemData);
    }

    if (dwNumResourceTags || pResourceTags) {
        m_pResourceTags = pResourceTags;
        m_dwNumResourceTags = dwNumResourceTags;
    }
    return 0;
}
