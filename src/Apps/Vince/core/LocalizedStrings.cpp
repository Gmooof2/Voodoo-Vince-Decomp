// UNIT: src/Apps/Vince/core/LocalizedStrings.cpp OBJ: core\XBox\OfficialRelease\LocalizedStrings.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

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
    unsigned int _pad00[0x1C / 4];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

extern "C" void* __stdcall XLoadSectionA(const char* pSectionName); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    BeepTDF();
    bool ParseRawData(const SwampString&, const SwampString&, int);
    const BeepTDF* FindBlock(const SwampString&, bool) const;
    int GetIntRecord(const SwampString& id, int def) const;

private:
    unsigned int _pad00[0x58 / 4];
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);

extern "C" const unsigned short* LocString_GetString(void* hblob, unsigned int uId); // NAME: proto-pdb:default.pdb

class LocalizedStrings { // NAME: proto-pdb:default.pdb
    LocalizedStrings();
public:
    const unsigned short* GetGameString(const SwampString& str);

private:
    static const unsigned short* _emptyString;

    void* _systemStrings;           // 0x00
    void* _gameStrings;             // 0x04
    BeepTDF _indirectFile;          // 0x08
    const BeepTDF* _stringBlock;    // 0x60
};
DECOMP_SIZE_ASSERT(LocalizedStrings, 0x64);

// FUNCTION: USA 0x00107DBF
// NAME: proto-pdb:default.pdb
const unsigned short* LocalizedStrings::GetGameString(const SwampString& str)
{
    int tmp = _stringBlock->GetIntRecord(str, -1);
    if (tmp == -1)
        return _emptyString;
    return LocString_GetString(_gameStrings, tmp);
}

// FUNCTION: USA 0x00107ECE
// PROTOTYPE: PREVIEW 0x004F2868 identical
// NAME: proto-pdb:default.pdb
LocalizedStrings::LocalizedStrings()
    : _systemStrings(0), _gameStrings(0)
{
    _indirectFile.ParseRawData(SwampString((const char*)XLoadSectionA("vstr")), "vinceStrings.tdf", 1);
    _stringBlock = _indirectFile.FindBlock("strings", false);
}
