// UNIT: src/Apps/Vince/core/WalkSounds.cpp OBJ: core\XBox\OfficialRelease\WalkSounds.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as soundnode.cpp
#include <xstring>

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0090); retail calls the string ctor and _Tidy out of line
// (named VAs); their bodies would emit the _Copy __ehhandler$ funclet here (gate objects extra); declared after
// <xstring>, before <stdexcept> instantiates them (C2908 otherwise)
template <> std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(const char*);
template <> void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Tidy(bool, unsigned int);
#endif

#include <string>
#include <vector>
#include <map>

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class SwampSceneNode; // NAME: proto-pdb:default.pdb

class SoundResource; // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _Resource != 0; }

    T* _Resource; // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
    bool EnableMissingResourceMessages(bool enable)
    {
        bool old = _missingResourceMessages;
        _missingResourceMessages = enable;
        return old;
    }

    char field_0[0x54];                // 0x00
    bool _missingResourceMessages;     // 0x54
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
}

class SoundManager : public ResourceManager<SoundResource> { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const ResourceHandle<SoundResource>& sh, const SwampString& name, unsigned int flags,
                          SwampSceneNode* parent, bool bAttach, float minDist3D, float maxDist3D);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
    SoundManager* _soundManager;       // 0x34
    char field_38[0x30];               // 0x38
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class WalkSounds { // NAME: proto-pdb:default.pdb
public:
    typedef std::map<unsigned int, std::vector<ResourceHandle<SoundResource> > > TagToSoundsMap;

    WalkSounds(const SwampString& characterName);
    void PlayASound(SwampSceneNode* owner) const;
    void SetCurrentSurface(unsigned int tag);

    unsigned int _currentSurface; // 0x00
    TagToSoundsMap _sounds;       // 0x04
};
DECOMP_SIZE_ASSERT(WalkSounds, 0x10);

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0132, C0154); retail calls size out of line (named VA); find stays
// visible (retail keeps tit in esi across the calls; decl-only 92.27) and relinks over its retail copy byte-equal
template <> std::vector<ResourceHandle<SoundResource> >::size_type std::vector<ResourceHandle<SoundResource> >::size() const;
#endif

// FUNCTION: USA 0x001150CB
// PROTOTYPE: PREVIEW 0x004FF8F9 identical
// NAME: proto-pdb:default.pdb
void WalkSounds::SetCurrentSurface(unsigned int tag)
{
    _currentSurface = tag & 0xF0000; // mask: retail disassembly 0x001150CB
}

// FUNCTION: USA 0x00115440
// PROTOTYPE: PREVIEW 0x004FFC47 identical
// NAME: proto-pdb:default.pdb
void WalkSounds::PlayASound(SwampSceneNode* owner) const
{
    TagToSoundsMap::const_iterator tit = _sounds.find(_currentSurface);
    if (tit != _sounds.end()) {
        // !FAKE: c2 block layout; dummy local; retail takes the index before the string temp (match_one 72.92 -> 100.0)
        unsigned int v1 = BeepLib::Random() % tit->second.size();
        SoundManager::PlaySound(tit->second[v1], "Walk sound", 0, owner, true, 0.0f, 0.0f);
    }
}

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >(std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > const &);
}
#endif

// FUNCTION: USA 0x001157E4
// PROTOTYPE: PREVIEW 0x00500032 identical
// NAME: proto-pdb:default.pdb
template std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > >::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > >(unsigned int const &,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > const &);

// FUNCTION: USA 0x00115801
// PROTOTYPE: PREVIEW 0x0050004F identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Node::_Node(std::_Tree_nod<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Node *,std::_Tree_nod<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Node *,std::_Tree_nod<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Node *,std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > const &,char);

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::iterator _Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Insert(bool,std::_Tree_nod<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Node *,std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > const &);
}
#endif

// FUNCTION: USA 0x001150D9
// PROTOTYPE: PREVIEW 0x004FF907 identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Node * std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::_Lbound(unsigned int const &)const;

// FUNCTION: USA 0x00115102
// PROTOTYPE: PREVIEW 0x004FF930 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::const_iterator::_Dec();

// FUNCTION: USA 0x001159F1
// PROTOTYPE: PREVIEW 0x0050023F identical
// NAME: proto-pdb:default.pdb
template std::pair<std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::iterator,bool> std::_Tree<std::_Tmap_traits<unsigned int,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > >,0> >::insert(std::pair<unsigned int const ,std::vector<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > > > const &);

// FUNCTION: USA 0x00115A82
// PROTOTYPE: PREVIEW 0x005002D0 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.92, bytes match; wall (C0155): delink tags the 0x10000/0x50000 table tags as DIR32 relocs
// (that best had ResourceHandle's PDB dtor inline, `if (_Resource) _Resource->SubRef();` = ResourceBase 0x1C `_RefCount--`,
// and SoundResource : ResourceBase; left out unit-wide: it changes the emitted vector _Tidy at fold VA 0x000C7B97)
WalkSounds::WalkSounds(const SwampString& characterName)
    : _currentSurface(0)
{
    struct SurfaceTypes { // NAME: proto-pdb:default.pdb
        unsigned int tag;              // 0x00
        const char* name;              // 0x04
    };
    const SurfaceTypes types[8] = {
        { 0x00000, "step01" },
        { 0x10000, "step02" },
        { 0x20000, "step05" },
        { 0x30000, "step07" },
        { 0x40000, "step03" },
        { 0x50000, "step06" },
        { 0x60000, "steppiano" },
        { 0xFFFFFFFF, 0 },
    };
    bool v0 = g_swamp->_soundManager->EnableMissingResourceMessages(false);
    for (int v1 = 0; types[v1].name; v1++) {
        std::vector<ResourceHandle<SoundResource> > surfaceSounds;
        for (unsigned int j = 1;; j++) {
            SwampString filename = format("%s%s_%02i", characterName.c_str(), types[v1].name, j);
            ResourceHandle<SoundResource> sh = g_swamp->_soundManager->Load(filename);
            if (!sh.IsValid()) {
                break;
            }
            surfaceSounds.push_back(sh);
        }
        // !FAKE: inliner; hand form of the VC 7.1 vector::size body; retail inlines size here but calls it out of
        // line in PlayASound, whose decl-only specialization above keeps the fold-VA body out of this obj
        std::vector<ResourceHandle<SoundResource> >::size_type v2;
        if (surfaceSounds._Myfirst == 0) {
            v2 = 0;
        } else {
            v2 = surfaceSounds._Mylast - surfaceSounds._Myfirst;
        }
        if (v2 > 0) {
            _sounds.insert(TagToSoundsMap::value_type(types[v1].tag, surfaceSounds));
        }
    }
    g_swamp->_soundManager->EnableMissingResourceMessages(v0);
}
#endif
