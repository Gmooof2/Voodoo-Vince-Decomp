// UNIT: src/Libs/Swamp/Xbox/Official_Release/Localize.cpp OBJ: Xbox\Official_Release\Localize.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> >
    SwampWString;

unsigned long GetLanguage();                  // NAME: proto-pdb:default.pdb
const SwampWString wformat(const char*, ...); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall XGetLanguage(); // src proto-pdb:default.pdb XGetLanguage

// FUNCTION: USA 0x000D078B
// PROTOTYPE: PREVIEW 0x004BBA4B identical
// NAME: proto-pdb:default.pdb
unsigned long GetLanguage()
{
    unsigned long lang = XGetLanguage();
    if (lang < 1 || lang > 6) {
        lang = 1;
    }
    return lang;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 3 = the three literals)
// FUNCTION: USA 0x000D080F
// PROTOTYPE: PREVIEW 0x004BBACF near-identical
// NAME: proto-pdb:default.pdb
SwampWString GetLocalizedDateString(int year, int month, int day)
{
    switch (GetLanguage()) {
    case 2:
        return wformat("%04i/%02i/%02i", year, month, day);
    case 3:
        return wformat("%02i.%02i.%04i", day, month, year);
    case 4:
    case 5:
    case 6:
        return wformat("%02i/%02i/%04i", day, month, year);
    default:
        return wformat("%02i/%02i/%04i", month, day, year);
    }
}
#endif
