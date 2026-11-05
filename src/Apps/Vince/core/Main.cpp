// UNIT: src/Apps/Vince/core/Main.cpp OBJ: core\XBox\OfficialRelease\Main.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    void StopRecording();
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];      // 0x00
    SwampInput* _input;     // 0x04
    char field_8[0x60];     // 0x08
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp;

void handledMain(); // src proto-pdb:default.pdb handledMain

// FUNCTION: USA 0x000E0BB0
// PROTOTYPE: PREVIEW 0x004CBF55 identical
// NAME: proto-pdb:default.pdb
int main()
{
    __try {
        handledMain();
    } __except (g_swamp->GetInput().StopRecording(), 0) {
    }
    return 0;
}

#ifdef NON_MATCHING
namespace std { // NAME: proto-pdb:default.pdb
template<class _Elem>
struct char_traits {};

template<class _Ty>
class allocator {};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    ~basic_string() { _Tidy(true); }
    bool empty() const { return (_Mysize == 0); }
    const _Elem& operator[](unsigned int _Off) const { return (_Myptr()[_Off]); }
    const _Elem* _Myptr() const { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    enum { _BUF_SIZE = 16 / sizeof(_Elem) < 1 ? 1 : 16 / sizeof(_Elem) };
    char _Alval;                       // 0x00
    union {
        _Elem _Buf[_BUF_SIZE];
        _Elem* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

void ReadErrorHalt(); // NAME: proto-pdb:default.pdb
#endif

// FUNCTION: USA 0x000E04F0
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match; wall: any added code renumbers _main's pinned scope table $T318 (->$T402)
void WeAreScrewedByAReadError(const SwampString& filename)
{
    if (filename.empty()) {
        ReadErrorHalt();
        return;
    }
    switch (filename[0]) {
    case 'D':
    case 'd':
        ReadErrorHalt();
    }
}
#endif

#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match for ??1VinceLoadScreen@@UAE@XZ; the gate fails _main (masked 1): pin $T318 becomes $T465 (4.3 T16.c6)
class CXBFont; // NAME: proto-pdb:default.pdb

class CXBPackedResource { // NAME: proto-pdb:default.pdb
public:
    ~CXBPackedResource();

    char field_0[0x18];                // 0x00
};
DECOMP_SIZE_ASSERT(CXBPackedResource, 0x18);

class LoadScreen { // NAME: proto-pdb:default.pdb
public:
    virtual ~LoadScreen();             // vtable+0x00

    char field_4[0x2C];                // 0x04
};
DECOMP_SIZE_ASSERT(LoadScreen, 0x30);

// FUNCTION: USA 0x000E03D8
// PROTOTYPE: PREVIEW 0x004CB7AF identical
// NAME: proto-pdb:default.pdb
class VinceLoadScreen : public LoadScreen { // NAME: proto-pdb:default.pdb
public:
    CXBFont* _fontBig;                 // 0x30
    CXBFont* _fontSmall;               // 0x34
    CXBPackedResource _loadScreenResources;            // 0x38
    std::basic_string<unsigned short> _savingMessage;  // 0x50
    std::basic_string<unsigned short> _loadingMessage; // 0x6C
    std::basic_string<unsigned short> _levelName;      // 0x88
    std::basic_string<unsigned short> _areaName;       // 0xA4
    char field_C0[0x18];               // 0xC0
};
DECOMP_SIZE_ASSERT(VinceLoadScreen, 0xD8);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1VinceLoadScreen@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceLoadScreen_dtor(VinceLoadScreen* p)
{
    p->VinceLoadScreen::~VinceLoadScreen();
}
#endif

//   {
//   }
