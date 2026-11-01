// UNIT: src/Apps/Vince/core/Diary.cpp OBJ: core\XBox\OfficialRelease\Diary.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
#include <stddef.h>

class SwampDiary { // NAME: proto-pdb:default.pdb
public:
    SwampDiary();
    ~SwampDiary();

    char field_0[0x1C]; // 0x00
};
DECOMP_SIZE_ASSERT(SwampDiary, 0x1C);

class Diary : public SwampDiary { // NAME: proto-pdb:default.pdb
public:
    Diary();
    ~Diary();

    char _path[48];                   // 0x1C
    wchar_t _currTitle[128];          // 0x4C
};
DECOMP_SIZE_ASSERT(Diary, 0x14C);

// FUNCTION: USA 0x0010026A
// PROTOTYPE: PREVIEW 0x004EADF2 identical
// NAME: proto-pdb:default.pdb
Diary::Diary()
{
    _currTitle[0] = 0;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); finish refused: pin-only name, not a fold alias
// FUNCTION: USA 0x0010027B
// NAME: proto-pdb:default.pdb
Diary::~Diary()
{
}
#endif
