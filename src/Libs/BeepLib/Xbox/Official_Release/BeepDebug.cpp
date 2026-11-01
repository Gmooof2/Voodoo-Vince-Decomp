// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepDebug.cpp OBJ: Xbox\Official_Release\BeepDebug.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class T> struct less;
template <class T> class allocator;
template <class K, class Pr, class A, bool Mfl> class _Tset_traits { // src proto-pdb:default.pdb _Tset_traits
public:
    typedef K key_type;
};
template <class Tr> class _Tree {
public:
    _Tree() { _Init(); }

protected:
    void _Init();

private:
    char _Alval;                       // 0x00
    void* _Myhead;                     // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class Pr = less<K>, class A = allocator<K> > class set
    : public _Tree<_Tset_traits<K, Pr, A, false> > {
};
}

class BeepState { // NAME: proto-pdb:default.pdb
private:
    class DebugState { // NAME: proto-pdb:default.pdb
    public:
        DebugState();

        std::set<unsigned __int64> _AssertsToIgnore; // 0x00
        int _AssertCount;              // 0x0C
        bool _AssertsDisabled;         // 0x10
    };

    DebugState _DebugState;            // 0x00
    char field_14[0xC58 - 0x14];       // 0x14
};
DECOMP_SIZE_ASSERT(BeepState, 0xC58);

// FUNCTION: USA 0x000386FD
// PROTOTYPE: PREVIEW 0x00426DF4 identical
// NAME: proto-pdb:default.pdb
BeepState::DebugState::DebugState()
{
    _AssertCount = 0;
    _AssertsDisabled = false;
}
