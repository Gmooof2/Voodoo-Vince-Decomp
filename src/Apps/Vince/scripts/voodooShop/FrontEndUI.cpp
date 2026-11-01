// UNIT: src/Apps/Vince/scripts/voodooShop/FrontEndUI.cpp OBJ: Xbox\OfficialRelease\FrontEndUI.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

class FrontEndMenu { // NAME: proto-pdb:default.pdb
public:
    virtual ~FrontEndMenu();           // vtable+0x00
    virtual void Invalidate();
    virtual void Update();

    char field_4[0xA4 - 0x4];          // 0x04
    bool _valid;                       // 0xA4
    char field_A5[0xFC - 0xA5];        // 0xA5
};
DECOMP_SIZE_ASSERT(FrontEndMenu, 0xFC);

class FrontEndUI : public FrontEndMenu { // NAME: proto-pdb:default.pdb
public:
    virtual void Update();

    FrontEndMenu* _menu;               // 0xFC
};
DECOMP_SIZE_ASSERT(FrontEndUI, 0x100);

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator;
template <class E> struct char_traits;
template <class E, class T = char_traits<E>, class A = allocator<E> > class basic_string {
public:
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    int compare(const basic_string& _Right) const;

private:
    unsigned int _pad00[7];
};
template <class E, class T, class A>
inline bool operator!=(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right)
{
    return _Left.compare(_Right) != 0;
}
template <class T1, class T2> struct pair {
    T1 first;                          // 0x00
    T2 second;
};
template <class T, class A = allocator<T> > class vector {
public:
    class iterator {
    public:
        iterator(T* _Ptr) : _Myptr(_Ptr) {}
        T& operator*() const { return *_Myptr; }
        T* operator->() const { return _Myptr; }
        iterator& operator++() { ++_Myptr; return *this; }
        iterator operator+(int _Off) const { return iterator(_Myptr + _Off); }
        bool operator!=(const iterator& _Right) const { return _Myptr != _Right._Myptr; }
        T* _Myptr;
    };
    iterator begin() { return iterator(_Myfirst); }
    iterator end() { return iterator(_Mylast); }
    unsigned int size() const;
    T& operator[](unsigned int _Pos) { return _Myfirst[_Pos]; }
    void push_back(const T& _Val);
    void clear() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}

typedef std::basic_string<char> SwampString;

// Retail layout: LevelLoader::Slot is 0xA4 in retail (imul 0xA4; proto PDB 0x8C); _chaptersFinished
// stays at 0x44, the grown span is opaque.
class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    class Slot { // NAME: proto-pdb:default.pdb
    public:
        char field_0[0x44];            // 0x00
        std::vector<std::pair<SwampString, SwampString> > _chaptersFinished; // 0x44
        char field_54[0xA4 - 0x54];    // 0x54
    };

    static LevelLoader& Instance();
    Slot& GetSlot(int slot) { return _slots[slot]; }

    char field_0[0x4C];                // 0x00
    std::vector<Slot> _slots;          // 0x4C
};

class ChaptersMenu : public FrontEndMenu { // NAME: proto-pdb:default.pdb
public:
    class LevelEntry { // NAME: proto-pdb:default.pdb
    public:
        LevelEntry();
        ~LevelEntry();

        SwampString _name;             // 0x00
        std::vector<SwampString> _areas; // 0x1C
    };

private:
    void NextChapter();
    void PrevChapter();
    void BuildChapterList();

    int _slotNum;                      // 0xFC
    char field_100[0x238 - 0x100];     // 0x100
    int _curLevel;                     // 0x238
    int _curArea;                      // 0x23C
    std::vector<LevelEntry> _chapterList; // 0x240
    int _totalChapters;                // 0x250
};
DECOMP_SIZE_ASSERT(ChaptersMenu, 0x254);

// FUNCTION: USA 0x0013D49D
// NAME: proto-pdb:default.pdb
void ChaptersMenu::NextChapter()
{
    int level = ++_curLevel;
    if (level == _chapterList.size())
        _curLevel = 0;
    _curArea = 0;
}

// FUNCTION: USA 0x0013D4C9
// NAME: proto-pdb:default.pdb
void ChaptersMenu::PrevChapter()
{
    if (_curLevel == 0)
        _curLevel = _chapterList.size();
    _curArea = 0;
    --_curLevel;
}

// FUNCTION: USA 0x0013D4F1
// PROTOTYPE: PREVIEW 0x00525E61 identical
// NAME: proto-pdb:default.pdb
void FrontEndUI::Update()
{
    if (!_menu) {
        FrontEndMenu::Update();
    }
}

// FUNCTION: USA 0x0013E055
// PROTOTYPE: PREVIEW 0x005269AD identical
// NAME: proto-pdb:default.pdb
void FrontEndMenu::Invalidate()
{
    _valid = false;
}

#ifdef NON_MATCHING
// best: match_one objdiff 92.81, bytes mismatch (masked 4)
// FUNCTION: USA 0x0013EC2F
// PROTOTYPE: PREVIEW 0x0052754D near-identical
// NAME: proto-pdb:default.pdb
void ChaptersMenu::BuildChapterList()
{
    _totalChapters = 0;
    std::vector<std::pair<SwampString, SwampString> >& chapters = LevelLoader::Instance().GetSlot(_slotNum)._chaptersFinished;
    if (chapters.size() == 0)
        return;

    LevelEntry le;
    le._name = chapters.begin()->first;
    _chapterList.clear();
    for (std::vector<std::pair<SwampString, SwampString> >::iterator it = chapters.begin(); it != chapters.end(); ++it) {
        if (le._name != it->first) {
            _chapterList.push_back(le);
            le._name = it->first;
            le._areas.clear();
        }
        le._areas.push_back(it->second);
        ++_totalChapters;
    }
    _chapterList.push_back(le);
}
#endif
