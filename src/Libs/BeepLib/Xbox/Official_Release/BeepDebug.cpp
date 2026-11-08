// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepDebug.cpp OBJ: Xbox\Official_Release\BeepDebug.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as init.cpp
#include <xstring>

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0090, C0302); erase's out_of_range throw calls the string ctor,
// copy ctor and _Tidy out of line in retail; their header bodies would emit other units' string rows here
template <> std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(const char*);
template <> std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(
    const std::basic_string<char, std::char_traits<char>, std::allocator<char> >&);
template <> void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Tidy(bool, unsigned int);
#endif
#include <set>

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0231 lever 1); retail _Init calls _Buynode out of line (named
// 0x0003835D); its header body would emit an __ehhandler$ funclet here
template <> std::_Tree<std::_Tset_traits<unsigned __int64, std::less<unsigned __int64>,
                                         std::allocator<unsigned __int64>, false> >::_Nodeptr
std::_Tree<std::_Tset_traits<unsigned __int64, std::less<unsigned __int64>, std::allocator<unsigned __int64>,
                             false> >::_Buynode();
#endif

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

// FUNCTION: USA 0x00038249
// PROTOTYPE: PREVIEW 0x00426940 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::_Lrotate(std::_Tree_nod<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::_Node *);

// FUNCTION: USA 0x0003828B
// PROTOTYPE: PREVIEW 0x00426982 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::_Rrotate(std::_Tree_nod<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::_Node *);

// FUNCTION: USA 0x000382CF
// PROTOTYPE: PREVIEW 0x004269C6 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::const_iterator::_Inc();

// FUNCTION: USA 0x00038305
// PROTOTYPE: PREVIEW 0x004269FC identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::_Erase(std::_Tree_nod<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::_Node *);

// FUNCTION: USA 0x000383E5
// PROTOTYPE: PREVIEW 0x00426ADC identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::_Init();

// FUNCTION: USA 0x0003840A
// PROTOTYPE: PREVIEW 0x00426B01 identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::iterator std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::erase(std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::iterator);
