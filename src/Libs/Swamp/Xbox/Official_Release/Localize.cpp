// UNIT: src/Libs/Swamp/Xbox/Official_Release/Localize.cpp OBJ: Xbox\Official_Release\Localize.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
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

// FUNCTION: USA 0x000D079E
// PROTOTYPE: PREVIEW 0x004BBA5E identical
// NAME: proto-pdb:default.pdb
SwampWString GetLocalizedTimeString(int hours, int minutes, int seconds)
{
    const unsigned short* v0 = L"";
    if (GetLanguage() == 1) {
        if (hours >= 12) {
            hours -= 12;
            v0 = L" PM";
        } else {
            v0 = L" AM";
        }
        if (hours == 0)
            hours = 12;
    }
    if (seconds >= 0)
        return wformat("%02i:%02i:%02i%s", hours, minutes, seconds, v0);
    return wformat("%02i:%02i%s", hours, minutes, v0);
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

// FUNCTION: USA 0x000D0871
// PROTOTYPE: PREVIEW 0x004BBB2C identical
// NAME: proto-pdb:default.pdb
SwampString GetLangCode()
{
    switch (GetLanguage()) {
    case 1:
        return "en";
    case 2:
        return "ja";
    case 3:
        return "de";
    case 4:
        return "fr";
    case 5:
        return "es";
    case 6:
        return "it";
    default:
        return "crap";
    }
}
