// UNIT: src/Apps/Vince/core/xbfont.cpp OBJ: core\XBox\OfficialRelease\xbfont.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

struct GLYPH_ATTR { // NAME: proto-pdb:default.pdb
    float tu1;                         // 0x00
    float tv1;                         // 0x04
    float tu2;                         // 0x08
    float tv2;                         // 0x0C
    short wOffset;                     // 0x10
    short wWidth;                      // 0x12
    short wAdvance;                    // 0x14
};
DECOMP_SIZE_ASSERT(GLYPH_ATTR, 0x18);

struct _D3DDEVICE_CREATION_PARAMETERS { // NAME: proto-pdb:default.pdb
    unsigned int AdapterOrdinal;       // 0x00
    int DeviceType;                    // 0x04 enum _D3DDEVTYPE
    void* hFocusWindow;                // 0x08 HWND__*
    unsigned long BehaviorFlags;       // 0x0C
};
DECOMP_SIZE_ASSERT(_D3DDEVICE_CREATION_PARAMETERS, 0x10);

extern "C" void __stdcall D3DDevice_GetCreationParameters(_D3DDEVICE_CREATION_PARAMETERS* pParameters); // src proto-pdb:default.pdb D3DDevice_GetCreationParameters
void* operator new[](unsigned int size, const char* file, int line);

struct XBRESOURCE;  // NAME: proto-pdb:default.pdb
struct D3DTexture;  // NAME: proto-pdb:default.pdb
struct D3DResource;

extern "C" void __stdcall D3DResource_Register(D3DResource* pThis, void* pBase); // src proto-pdb:default.pdb D3DResource_Register
extern "C" unsigned long __stdcall D3DResource_AddRef(D3DResource* pThis); // src proto-pdb:default.pdb D3DResource_AddRef

struct D3DResource { // NAME: proto-pdb:default.pdb
    unsigned long AddRef() { return D3DResource_AddRef(this); }
    void Register(void* pBase) { D3DResource_Register(this, pBase); }

    unsigned long Common;              // 0x00
    unsigned long Data;                // 0x04
    unsigned long Lock;                // 0x08
};

class CXBPackedResource { // NAME: proto-pdb:default.pdb
public:
    CXBPackedResource();
    ~CXBPackedResource();
    D3DResource* RegisterResource(D3DResource* pResource) const;
    void Destroy();

    unsigned char* m_pSysMemData;      // 0x00
    unsigned long m_dwSysMemDataSize;  // 0x04
    unsigned char* m_pVidMemData;      // 0x08
    unsigned long m_dwVidMemDataSize;  // 0x0C
    XBRESOURCE* m_pResourceTags;       // 0x10
    unsigned long m_dwNumResourceTags; // 0x14
};

DECOMP_SIZE_ASSERT(CXBPackedResource, 0x18);

class CXBFont { // NAME: proto-pdb:default.pdb
public:
    CXBFont();
    ~CXBFont();
    long Destroy();
    long Create(D3DTexture* pFontTexture, void* pFontData);
    long CreateShaders();
    void SetScaleFactors(float fXScaleFactor, float fYScaleFactor);
    void SetBaseScaleFactors(float fXScaleFactor, float fYScaleFactor);
    float GetScalingCode(const unsigned short* str) const;

    float m_fFontHeight;                // 0x00
    float m_fFontTopPadding;            // 0x04
    float m_fFontBottomPadding;         // 0x08
    float m_fFontYAdvance;              // 0x0C
    float m_fXScaleFactor;              // 0x10
    float m_fYScaleFactor;              // 0x14
    float m_fSlantFactor;               // 0x18
    float m_verticalSpacing;            // 0x1C
    float m_fXBaseScaleFactor;          // 0x20
    float m_fYBaseScaleFactor;          // 0x24
    float m_fCursorX;                   // 0x28
    float m_fCursorY;                   // 0x2C
    unsigned short m_cMaxGlyph;         // 0x30
    short* m_TranslatorTable;           // 0x34
    unsigned long m_dwNumGlyphs;        // 0x38
    GLYPH_ATTR* m_Glyphs;               // 0x3C
    int m_bSaveState;                   // 0x40
    unsigned long m_dwSavedState[16];   // 0x44
    CXBPackedResource m_xprResource;    // 0x84
    D3DTexture* m_pFontTexture;         // 0x9C
    unsigned long m_dwNestedBeginCount; // 0xA0
};

DECOMP_SIZE_ASSERT(CXBFont, 0xA4);

// FUNCTION: USA 0x000FF5F3
// PROTOTYPE: PREVIEW 0x004EA1A0 identical
// NAME: proto-pdb:default.pdb
D3DResource* CXBPackedResource::RegisterResource(D3DResource* pResource) const
{
    if (pResource && (pResource->Common & 0xFFFF) == 1) {
        pResource->Register(m_pVidMemData);
        pResource->AddRef();
    }
    return pResource;
}

// FUNCTION: USA 0x000FF61D
// PROTOTYPE: PREVIEW 0x004EA1CA identical
// NAME: proto-pdb:default.pdb
CXBFont::CXBFont()
{
    m_fFontHeight = 36.0f;
    m_fFontTopPadding = 0.0f;
    m_fFontBottomPadding = 0.0f;
    m_fFontYAdvance = 36.0f;
    m_pFontTexture = 0;
    m_dwNumGlyphs = 0;
    m_Glyphs = 0;
    m_cMaxGlyph = 0;
    m_TranslatorTable = 0;
    m_fCursorX = 0.0f;
    m_fCursorY = 0.0f;
    m_fXScaleFactor = 1.0f;
    m_fYScaleFactor = 1.0f;
    m_fSlantFactor = 0.0f;
    m_verticalSpacing = 0.0f;
    m_fXBaseScaleFactor = 1.0f;
    m_fYBaseScaleFactor = 1.0f;
    m_dwNestedBeginCount = 0;
}

// FUNCTION: USA 0x000FF74D
// PROTOTYPE: PREVIEW 0x004EA2FA identical
// NAME: proto-pdb:default.pdb
long CXBFont::Create(D3DTexture* pFontTexture, void* pFontData)
{
    m_pFontTexture = pFontTexture;
    // !FAKE: c2 register allocation; stand-in local; the data cursor (no PDB local)
    unsigned char* p = (unsigned char*)pFontData + 8;
    // !FAKE: c2 register allocation; stand-in local; the file version (no PDB local)
    unsigned long v0 = *(unsigned long*)p;
    p += 4;
    if (v0 == 4) {
        m_fFontHeight = (float)*(unsigned long*)p;
        p += 4;
        m_fFontYAdvance = m_fFontHeight;
        float fTexWidth = (float)*(unsigned long*)p;
        p += 4;
        float fTexHeight = (float)*(unsigned long*)p;
        p += 4;
        p += 4;
        // !FAKE: c2 register allocation; stand-in local; the low glyph kept in di (no PDB local)
        unsigned short v1 = *(unsigned short*)p;
        p += 2;
        m_cMaxGlyph = *(unsigned short*)p;
        p += 2;
        p += 4;
        m_Glyphs = (GLYPH_ATTR*)p;
        // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
        m_TranslatorTable = new("\\beep\\vendor\\Microsoft\\XbCommon\\xbfont.cpp", 259) short[m_cMaxGlyph + 1];
        // !FAKE: c2 loop; stand-in loop counter; no PDB local
        for (unsigned short v2 = 0; v2 <= m_cMaxGlyph; v2++) {
            if (v2 < v1) {
                m_TranslatorTable[v2] = 0;
            } else {
                m_TranslatorTable[v2] = v2 - v1;
                // !FAKE: c2 register allocation; stand-in local; the glyph kept in eax (no PDB local)
                GLYPH_ATTR* v3 = &m_Glyphs[m_TranslatorTable[v2]];
                v3->wWidth++;
                v3->tu1 *= (fTexWidth - 1.0f) / fTexWidth;
                v3->tv1 *= (fTexHeight - 1.0f) / fTexHeight;
                v3->tu2 *= (fTexWidth - 1.0f) / fTexWidth;
                v3->tv2 *= (fTexHeight - 1.0f) / fTexHeight;
            }
        }
        m_fFontHeight += 1.0f;
    } else if (v0 == 5) {
        m_fFontHeight = *(float*)p;
        p += 4;
        m_fFontTopPadding = *(float*)p;
        p += 4;
        m_fFontBottomPadding = *(float*)p;
        p += 4;
        m_fFontYAdvance = *(float*)p;
        p += 4;
        m_cMaxGlyph = *(unsigned short*)p;
        p += 2;
        m_TranslatorTable = (short*)p;
        p += 2 * (m_cMaxGlyph + 1);
        m_dwNumGlyphs = *(unsigned long*)p;
        p += 4;
        m_Glyphs = (GLYPH_ATTR*)p;
    } else {
        return 0x80004005;
    }
    if (CreateShaders() < 0)
        return 0x80004005;
    _D3DDEVICE_CREATION_PARAMETERS d3dcp;
    D3DDevice_GetCreationParameters(&d3dcp);
    m_bSaveState = 1;
    return 0;
}

// FUNCTION: USA 0x000FF8C4
// PROTOTYPE: PREVIEW 0x004EA471 identical
// NAME: proto-pdb:default.pdb
long CXBFont::Destroy()
{
    m_xprResource.Destroy();
    m_fFontHeight = 0.0f;
    m_fFontTopPadding = 0.0f;
    m_fFontBottomPadding = 0.0f;
    m_fFontYAdvance = 0.0f;
    m_pFontTexture = 0;
    m_dwNumGlyphs = 0;
    m_Glyphs = 0;
    m_cMaxGlyph = 0;
    m_TranslatorTable = 0;
    m_dwNestedBeginCount = 0;
    return 0;
}

// FUNCTION: USA 0x000FF902
// PROTOTYPE: PREVIEW 0x004EA4AF identical
// NAME: proto-pdb:default.pdb
void CXBFont::SetScaleFactors(float fXScaleFactor, float fYScaleFactor)
{
    m_fXScaleFactor = fXScaleFactor;
    m_fYScaleFactor = fYScaleFactor;
}

// FUNCTION: USA 0x000FF913
// PROTOTYPE: PREVIEW 0x004EA4C0 identical
// NAME: proto-pdb:default.pdb
void CXBFont::SetBaseScaleFactors(float fXScaleFactor, float fYScaleFactor)
{
    m_fXBaseScaleFactor = fXScaleFactor;
    m_fYBaseScaleFactor = fYScaleFactor;
}

// FUNCTION: USA 0x000FFB46
// PROTOTYPE: PREVIEW 0x004EA6F3 identical
// NAME: proto-pdb:default.pdb
float CXBFont::GetScalingCode(const unsigned short* str) const
{
    if (str && str[0] && str[1] && str[2] && str[0] >= '0' && str[0] <= '9' && str[1] == '.' && str[2] >= '0'
        && str[2] <= '9')
        return (str[2] - '0') * 0.1f + (double)(str[0] - '0');
    return -1.0f;
}

// FUNCTION: USA 0x000FFBB4
// PROTOTYPE: PREVIEW 0x004EA761 identical
// NAME: proto-pdb:default.pdb
CXBFont::~CXBFont()
{
    Destroy();
}
