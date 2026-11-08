// UNIT: src/Libs/Swamp/Xbox/Official_Release/ProfileBar.cpp OBJ: Xbox\Official_Release\ProfileBar.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as WalkSounds.cpp
#include <string>
#include <vector>
#include <map>
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
}

class ProfileBar { // NAME: proto-pdb:default.pdb
public:
    struct ProfileEntry { // NAME: proto-pdb:default.pdb
        SwampString name;              // 0x00
        unsigned int color;            // 0x1C
        double currentTime;            // 0x20
        double percentOfFrame;         // 0x28
    };

    unsigned int Register(const SwampString& name, unsigned int color);
    void Register(const SwampString& name, unsigned int id, unsigned int color);
    void StartFrame();
    void EndFrame();

    double _frameStartTime;            // 0x00
    double _accumStartTime;            // 0x08
    std::map<unsigned int, ProfileEntry*> _entries; // 0x10
    std::vector<unsigned int> _renderOrder; // 0x1C
    char field_2C[0x40 - 0x2C];        // 0x2C
};
DECOMP_SIZE_ASSERT(ProfileBar::ProfileEntry, 0x30);
DECOMP_SIZE_ASSERT(ProfileBar, 0x40);

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
// _Lbound stays visible: its fold VA 0x000D7CEC is the ZoneNode-alias row below; our ProfileEntry copy is byte-equal and
// merges as a fold duplicate (C0233 fix 1)
namespace std {
template <> std::_Tree<std::_Tmap_traits<unsigned int,ProfileBar::ProfileEntry *,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,ProfileBar::ProfileEntry *> >,0> >::iterator _Tree<std::_Tmap_traits<unsigned int,ProfileBar::ProfileEntry *,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,ProfileBar::ProfileEntry *> >,0> >::insert(std::_Tree<std::_Tmap_traits<unsigned int,ProfileBar::ProfileEntry *,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,ProfileBar::ProfileEntry *> >,0> >::iterator,std::pair<unsigned int const ,ProfileBar::ProfileEntry *> const &);
template <> vector<unsigned int>::iterator vector<unsigned int>::erase(vector<unsigned int>::iterator, vector<unsigned int>::iterator);
// Register: basic_string ctor/assign/compare are called by name (assign 0x00011655 and compare 0x00019E98 are other
// units' rows, C0215); the map's find stays visible: with its body cl keeps end() in edi across the find loop
template <> basic_string<char, char_traits<char>, allocator<char> >::basic_string();
template <> basic_string<char, char_traits<char>, allocator<char> >& basic_string<char, char_traits<char>, allocator<char> >::assign(const basic_string<char, char_traits<char>, allocator<char> >&, size_type, size_type);
template <> int basic_string<char, char_traits<char>, allocator<char> >::compare(const basic_string<char, char_traits<char>, allocator<char> >&) const;
}
#endif

void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000D7E08
// PROTOTYPE: PREVIEW 0x004C335A identical
// NAME: proto-pdb:default.pdb
void ProfileBar::StartFrame()
{
    _accumStartTime = _frameStartTime = BeepLib::GetTime();
    // !FAKE: c2 stack slots; inner block; retail reuses i's slot for erase's result temp (match_one 96.69 -> 100.0)
    {
        for (std::map<unsigned int, ProfileEntry*>::iterator i = _entries.begin(); i != _entries.end(); ++i)
            i->second->currentTime = 0.0;
    }
    _renderOrder.erase(_renderOrder.begin(), _renderOrder.end());
}

// FUNCTION: USA 0x000D7E54
// PROTOTYPE: PREVIEW 0x004C33A6 identical
// NAME: proto-pdb:default.pdb
void ProfileBar::EndFrame()
{
    for (std::map<unsigned int, ProfileEntry*>::iterator i = _entries.begin(); i != _entries.end(); ++i)
        i->second->percentOfFrame = i->second->currentTime * 30.0;
}

// FUNCTION: USA 0x000D816C
// PROTOTYPE: PREVIEW 0x004C36F3 identical
// NAME: proto-pdb:default.pdb
template ProfileBar::ProfileEntry * & std::map<unsigned int,ProfileBar::ProfileEntry *,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,ProfileBar::ProfileEntry *> > >::operator[](unsigned int const &);

// FUNCTION: USA 0x000D81AE
// PROTOTYPE: PREVIEW 0x004C3735 identical
// NAME: proto-pdb:default.pdb
void ProfileBar::Register(const SwampString& name, unsigned int id, unsigned int color)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
    ProfileEntry* e = new("ProfileBar.cpp", 59) ProfileEntry;
    e->name = name;
    e->currentTime = 0.0;
    e->color = color;
    _entries[id] = e;
}

// FUNCTION: USA 0x000D8203
// PROTOTYPE: PREVIEW 0x004C378A identical
// NAME: proto-pdb:default.pdb
unsigned int ProfileBar::Register(const SwampString& name, unsigned int color)
{
    // !FAKE: c2 stack slots; sibling blocks; PDB has eit and id both at bprel -0x4 (match_one 76.95 -> 77.0, slots equal)
    {
        for (std::map<unsigned int, ProfileEntry*>::const_iterator eit = _entries.begin(); eit != _entries.end(); ++eit)
            if (eit->second->name == name)
                return eit->first;
    }
    {
        unsigned int id = 1;
        while (_entries.find(id) != _entries.end())
            id++;
        Register(name, id, color);
        return id;
    }
}

// !FAKE: linker COMDAT folding; ZoneNode alias declaration; the delinked retail object names this fold VA by the ZoneNode alias, objdiff pairs by name (C0234)
class ZoneNode; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000D7CEC
// PROTOTYPE: PREVIEW 0x004C323E identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<unsigned int,ZoneNode *,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,ZoneNode *> >,0> >::_Node * std::_Tree<std::_Tmap_traits<unsigned int,ZoneNode *,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,ZoneNode *> >,0> >::_Lbound(unsigned int const &)const;
