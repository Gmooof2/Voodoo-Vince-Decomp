// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepDebug.cpp OBJ: Xbox\Official_Release\BeepDebug.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as init.cpp
#include <set>

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specialization (C0231 lever 1); retail _Init calls _Buynode out of line (named
// 0x0003835D); its header body would emit an __ehhandler$ funclet here
template <> std::_Tree<std::_Tset_traits<unsigned __int64, std::less<unsigned __int64>,
                                         std::allocator<unsigned __int64>, false> >::_Nodeptr
std::_Tree<std::_Tset_traits<unsigned __int64, std::less<unsigned __int64>, std::allocator<unsigned __int64>,
                             false> >::_Buynode();
// !FAKE: declaration-only explicit specialization of the guarded erase row (named 0x0003840A, C0233 fix 2); the
// unreferenced ~set -> _Tidy chain would emit its out_of_range throw info and other units' string rows here
template <> std::_Tree<std::_Tset_traits<unsigned __int64, std::less<unsigned __int64>,
                                         std::allocator<unsigned __int64>, false> >::iterator
std::_Tree<std::_Tset_traits<unsigned __int64, std::less<unsigned __int64>, std::allocator<unsigned __int64>,
                             false> >::erase(iterator);
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
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (4.4 T4.c6, with decl-only string ctor/copy ctor/_Tidy); relink wall:
// throw data now pinned, but our .xdata$x __CT/__CTA3/__TI3 (0x001FCEBC..0x001FCF30) and EvilVince.obj .xdata$x
// (0x001FD8A8) enclose retail bytes: "retail bytes between two of our .xdata$x sections" (relink.py:1143-1145)
template std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::iterator std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::erase(std::_Tree<std::_Tset_traits<unsigned __int64,std::less<unsigned __int64>,std::allocator<unsigned __int64>,0> >::iterator);
#endif
