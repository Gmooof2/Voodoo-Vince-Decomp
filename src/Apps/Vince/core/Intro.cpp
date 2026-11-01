// UNIT: src/Apps/Vince/core/Intro.cpp OBJ: core\XBox\OfficialRelease\Intro.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template<class _Elem> // src proto-pdb:default.pdb _Elem
struct char_traits {};

template<class _Ty>
class allocator {};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
private:
    char _pad00[0x1C];
};

template<class _Ty, class _Ax = allocator<_Ty> >
class list {
public:
    ~list() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0xC];
};
}

class MoviePlayer { // NAME: proto-pdb:default.pdb
public:
    ~MoviePlayer();
    void Render() const;
    bool IsActive() const;

    char field_0[0x8];                 // 0x00
    bool _active;                      // 0x08
    char field_9[0xB - 0x9];           // 0x09
    bool _clear;                       // 0x0B
    bool _cleanup;                     // 0x0C
    char field_D[0x94 - 0xD];          // 0x0D
};
DECOMP_SIZE_ASSERT(MoviePlayer, 0x94);

class VinceIntro { // NAME: proto-pdb:default.pdb
public:
    ~VinceIntro();
    void Render();

private:
    char field_0[0xC];                 // 0x00
    std::list<std::basic_string<char> > _movies; // 0x0C
    MoviePlayer* _player;              // 0x18
};
DECOMP_SIZE_ASSERT(VinceIntro, 0x1C);

// FUNCTION: USA 0x00125C3E
// PROTOTYPE: PREVIEW 0x0050E755 identical
// NAME: proto-pdb:default.pdb
bool MoviePlayer::IsActive() const
{
    return _active || _clear || _cleanup;
}

// FUNCTION: USA 0x00125C53
// PROTOTYPE: PREVIEW 0x0050E76A identical
// NAME: proto-pdb:default.pdb
void VinceIntro::Render()
{
    if (_player)
        _player->Render();
}

// FUNCTION: USA 0x00125E7B
// PROTOTYPE: PREVIEW 0x0050E988 identical
// NAME: proto-pdb:default.pdb
VinceIntro::~VinceIntro()
{
    delete _player;
}
