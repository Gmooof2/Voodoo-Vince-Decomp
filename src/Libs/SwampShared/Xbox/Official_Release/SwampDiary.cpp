// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampDiary.cpp OBJ: Xbox\Official_Release\SwampDiary.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

void BeepFree(void* p); // NAME: proto-pdb:default.pdb

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
private:
    char _pad00[0x1C];
};
template <class T> struct less;
template <class T1, class T2> struct pair {
    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef pair<const K, T> value_type;
};
template <class Tr> class _Tree {
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
    };
public:
    class const_iterator {
    public:
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        bool operator!=(const const_iterator& _Right) const { return _Ptr != _Right._Ptr; }
        void _Inc();
        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
        typename Tr::value_type* operator->() const { return &this->_Ptr->_Myval; }
        iterator& operator++() { this->_Inc(); return *this; }
    };
    _Tree() { _Init(); }
    ~_Tree() { _Tidy(); }
    iterator begin() { return iterator(_Myhead->_Left); }
    iterator end() { return iterator(_Myhead); }
    void clear();

protected:
    void _Init();
    void _Tidy();

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map
    : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct DiaryFile { // NAME: proto-pdb:default.pdb
    unsigned int magic;                // 0x00
    unsigned int version;              // 0x04
    unsigned int numFiles;             // 0x08
    unsigned int reserved;             // 0x0C
};
DECOMP_SIZE_ASSERT(DiaryFile, 0x10);

class SwampDiary { // NAME: proto-pdb:default.pdb
public:
    struct DiaryEntry { // NAME: proto-pdb:default.pdb
        char field_0[0x40];            // 0x00
        void* data;                    // 0x40
    };

    SwampDiary();
    ~SwampDiary();

protected:
    void CleanUp();

    DiaryFile _currDiary;              // 0x00
    std::map<SwampString, DiaryEntry> _entries; // 0x10
};
DECOMP_SIZE_ASSERT(SwampDiary::DiaryEntry, 0x44);
DECOMP_SIZE_ASSERT(SwampDiary, 0x1C);

// FUNCTION: USA 0x00152367
// PROTOTYPE: PREVIEW 0x0053B9E7 identical
// NAME: proto-pdb:default.pdb
void SwampDiary::CleanUp()
{
    for (std::map<SwampString, DiaryEntry>::iterator eit = _entries.begin(); eit != _entries.end(); ++eit) {
        BeepFree(eit->second.data);
    }
    _entries.clear();
}
// FUNCTION: USA 0x001526CA
// PROTOTYPE: PREVIEW 0x0053BD4A identical
// NAME: proto-pdb:default.pdb
SwampDiary::~SwampDiary()
{
    CleanUp();
}

// FUNCTION: USA 0x001526DB
// PROTOTYPE: PREVIEW 0x0053BD5B identical
// NAME: proto-pdb:default.pdb
SwampDiary::SwampDiary()
{
}

