// UNIT: src/Apps/Vince/SwampD3D.cpp OBJ: Xbox\OfficialRelease\SwampD3D.obj OPT: /O1 /Oy
#include "../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
// FUNCTION: USA 0x000120C1
// PROTOTYPE: PREVIEW 0x004012B5 identical
// NAME: proto-pdb:default.pdb
template <class E, class T, class A> class basic_string {
public:
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[0x1C / 4];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

// !FAKE: c1xx front end; scaffold array delete; retail SwampD3D.obj defines the compiler-generated vector destructor
// iterator ??_I@YGXPAXIHP6EX0@Z@Z with no caller left in the unit; this delete[] makes cl emit it (matched_set [[emitter]] row)
void pin_emit_vector_dtor_iterator(SwampString* p)
{
    delete[] p;
}
