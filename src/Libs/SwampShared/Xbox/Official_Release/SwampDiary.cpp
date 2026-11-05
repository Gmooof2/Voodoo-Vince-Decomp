// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampDiary.cpp OBJ: Xbox\Official_Release\SwampDiary.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

void BeepFree(void* p); // NAME: proto-pdb:default.pdb

// global scope: retail RTTI is ?AVexception@@ (C0255); NAME: proto-pdb:default.pdb
class exception {
public:
    exception(const exception& v1); // out of line in retail (logic_error copy ctor calls it)
    virtual ~exception();
    virtual const char* what() const;
private:
    const char* _m_what;
    int _m_doFree;
};

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
// VC 7.1 <xmemory> _Destroy / allocator members the _Tree rows inline (4.3 T12.c37)
template <class _Ty> inline void _Destroy(_Ty* _Ptr)
{
    (_Ptr)->~_Ty();
}
template <class T> class allocator {
public:
    template <class _Other> struct rebind {
        typedef allocator<_Other> other;
    };
    void deallocate(T* _Ptr, unsigned int) { operator delete(_Ptr); }
    void destroy(T* _Ptr) { _Destroy(_Ptr); }
    unsigned int max_size() const
    {
        unsigned int _Count = (unsigned int)(-1) / sizeof(T);
        return (0 < _Count ? _Count : 1);
    }
};
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0); // VC 7.1 protected: retail ?_Tidy@...@IAEX_NI@Z 0x000111D6
private:
    char _pad00[0x1C];
};
template <class E, class T, class A>
bool operator<(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right);
template <class T> struct less {
    bool operator()(const T& _Left, const T& _Right) const { return (_Left < _Right); }
};
template <class _Ty> inline void swap(_Ty& _Left, _Ty& _Right)
{
    _Ty _Tmp = _Left;
    _Left = _Right, _Right = _Tmp;
}
#ifdef NON_MATCHING
// VC 7.1 <exception>/<stdexcept> classes thrown inline by _Tree::erase/_Insert; ctor of logic_error 0x0001179C
class logic_error : public exception {
public:
    explicit logic_error(const basic_string<char, char_traits<char>, allocator<char> >& _Message);
    virtual ~logic_error() {}
    virtual const char* what() const;
private:
    basic_string<char, char_traits<char>, allocator<char> > _Str;
};
class length_error : public logic_error {
public:
    explicit length_error(const basic_string<char, char_traits<char>, allocator<char> >& _Message) : logic_error(_Message) {}
    virtual ~length_error() {}
};
class out_of_range : public logic_error {
public:
    explicit out_of_range(const basic_string<char, char_traits<char>, allocator<char> >& _Message) : logic_error(_Message) {}
    virtual ~out_of_range() {}
};
#endif
template <class T1, class T2> struct pair {
    pair(const T1& _Val1, const T2& _Val2) : first(_Val1), second(_Val2) {}
    // implicit copy ctor (retail ??0...@ABU01@@Z) written out: cl 13.10 skips an explicit instantiation of an implicit one
    pair(const pair& p) : first(p.first), second(p.second) {} // !FAKE: implicit copy ctor written member-wise (no PDB local), 4.3 T12
    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
    typedef Pr key_compare;
    typedef typename A::template rebind<value_type>::other allocator_type;
    enum { _Multi = Mfl };
    static const K& _Kfn(const value_type& _Val) { return (_Val.first); }
    Pr comp;
};
template <class Traits> class _Tree_nod : public Traits {
protected:
    typedef typename Traits::value_type value_type;

public:
    struct _Node;
    typedef _Node* _Genptr;
    struct _Node {
        _Node(_Genptr _Larg, _Genptr _Parg, _Genptr _Rarg, const value_type& _Val, char _Carg)
            : _Left(_Larg), _Parent(_Parg), _Right(_Rarg), _Myval(_Val), _Color(_Carg), _Isnil(false)
        {
        }
        _Genptr _Left;                 // 0x00
        _Genptr _Parent;               // 0x04
        _Genptr _Right;                // 0x08
        value_type _Myval;             // 0x0C
        char _Color;
        char _Isnil;
    };
    typename Traits::allocator_type::template rebind<_Node>::other _Alnod;
};
// _Tree: VC 7.1 <xtree> bodies of the SwampDiary rows (xtree:130-289, 476-505, 589-747, 889-1085), 4.3 T12.c37
template <class Tr> class _Tree : public _Tree_nod<Tr> {
public:
    typedef typename Tr::key_type key_type;
    typedef typename Tr::value_type value_type;
    typedef typename Tr::allocator_type allocator_type;

protected:
    typedef typename _Tree_nod<Tr>::_Node _Node;
    typedef _Node* _Nodeptr;
    enum _Redbl { _Red, _Black };
    static char& _Color(_Nodeptr _Pnode) { return ((char&)(*_Pnode)._Color); }
    static char& _Isnil(_Nodeptr _Pnode) { return ((char&)(*_Pnode)._Isnil); }
    static const key_type& _Key(_Nodeptr _Pnode) { return (Tr::_Kfn(_Myval(_Pnode))); }
    static _Nodeptr& _Left(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Left); }
    static _Nodeptr& _Parent(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Parent); }
    static _Nodeptr& _Right(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Right); }
    static value_type& _Myval(_Nodeptr _Pnode) { return ((value_type&)(*_Pnode)._Myval); }

public:
    class const_iterator;
    friend class const_iterator;
    class const_iterator {
    public:
        const_iterator(_Nodeptr _Pnode) : _Ptr(_Pnode) {}
        const value_type* operator->() const { return &this->_Ptr->_Myval; }
        const_iterator& operator++() { _Inc(); return *this; }
        bool operator==(const const_iterator& _Right) const { return (_Ptr == _Right._Ptr); }
        bool operator!=(const const_iterator& _Right) const { return _Ptr != _Right._Ptr; }
        void _Dec()
        {
            if (_Isnil(_Ptr))
                _Ptr = _Right(_Ptr);
            else if (!_Isnil(_Left(_Ptr)))
                _Ptr = _Max(_Left(_Ptr));
            else {
                _Nodeptr _Pnode;
                while (!_Isnil(_Pnode = _Parent(_Ptr)) && _Ptr == _Left(_Pnode))
                    _Ptr = _Pnode;
                if (!_Isnil(_Pnode))
                    _Ptr = _Pnode;
            }
        }
        void _Inc()
        {
            if (_Isnil(_Ptr))
                ;
            else if (!_Isnil(_Right(_Ptr)))
                _Ptr = _Min(_Right(_Ptr));
            else {
                _Nodeptr _Pnode;
                while (!_Isnil(_Pnode = _Parent(_Ptr)) && _Ptr == _Right(_Pnode))
                    _Ptr = _Pnode;
                _Ptr = _Pnode;
            }
        }
        _Nodeptr _Mynode() const { return (_Ptr); }
        _Nodeptr _Ptr;
    };
    class iterator;
    friend class iterator;
    class iterator : public const_iterator {
    public:
        iterator(_Nodeptr _Pnode) : const_iterator(_Pnode) {}
        value_type* operator->() const { return &this->_Ptr->_Myval; }
        iterator& operator++() { this->_Inc(); return *this; }
        iterator& operator--() { this->_Dec(); return (*this); }
    };
    typedef pair<iterator, bool> _Pairib;
    _Tree() { _Init(); }
    ~_Tree() { _Tidy(); }
    iterator begin() { return iterator(_Lmost()); }
    iterator end() { return iterator(_Myhead); }
    unsigned int max_size() const { return (this->_Alval.max_size()); }
    unsigned int size() const { return (_Mysize); }
    void clear();
    iterator find(const key_type& _Keyval); // retail calls it by name (pinned 0x00151D13)

    _Pairib insert(const value_type& _Val)
    {
        _Nodeptr _Trynode = _Root();
        _Nodeptr _Wherenode = _Myhead;
        bool _Addleft = true;
        while (!_Isnil(_Trynode)) {
            _Wherenode = _Trynode;
            _Addleft = this->comp(Tr::_Kfn(_Val), _Key(_Trynode));
            _Trynode = _Addleft ? _Left(_Trynode) : _Right(_Trynode);
        }

        if (Tr::_Multi)
            return (_Pairib(_Insert(_Addleft, _Wherenode, _Val), true));
        else {
            iterator _Where = iterator(_Wherenode);
            if (!_Addleft)
                ;
            else if (_Where == begin())
                return (_Pairib(_Insert(true, _Wherenode, _Val), true));
            else
                --_Where;

            if (this->comp(_Key(_Where._Mynode()), Tr::_Kfn(_Val)))
                return (_Pairib(_Insert(_Addleft, _Wherenode, _Val), true));
            else
                return (_Pairib(_Where, false));
        }
    }

#ifdef NON_MATCHING
    // erase/_Insert: match_one 100, but the inline throw emits unnamed RTTI/__CT data (wall C0212)
    iterator erase(iterator _Where)
    {
        if (_Isnil(_Where._Mynode()))
            throw out_of_range("invalid map/set<T> iterator");
        _Nodeptr _Erasednode = _Where._Mynode();
        ++_Where;

        _Nodeptr _Fixnode;
        _Nodeptr _Fixnodeparent;
        _Nodeptr _Pnode = _Erasednode;

        if (_Isnil(_Left(_Pnode)))
            _Fixnode = _Right(_Pnode);
        else if (_Isnil(_Right(_Pnode)))
            _Fixnode = _Left(_Pnode);
        else {
            _Pnode = _Where._Mynode();
            _Fixnode = _Right(_Pnode);
        }

        if (_Pnode == _Erasednode) {
            _Fixnodeparent = _Parent(_Erasednode);
            if (!_Isnil(_Fixnode))
                _Parent(_Fixnode) = _Fixnodeparent;

            if (_Root() == _Erasednode)
                _Root() = _Fixnode;
            else if (_Left(_Fixnodeparent) == _Erasednode)
                _Left(_Fixnodeparent) = _Fixnode;
            else
                _Right(_Fixnodeparent) = _Fixnode;

            if (_Lmost() == _Erasednode)
                _Lmost() = _Isnil(_Fixnode) ? _Fixnodeparent : _Min(_Fixnode);

            if (_Rmost() == _Erasednode)
                _Rmost() = _Isnil(_Fixnode) ? _Fixnodeparent : _Max(_Fixnode);
        } else {
            _Parent(_Left(_Erasednode)) = _Pnode;
            _Left(_Pnode) = _Left(_Erasednode);

            if (_Pnode == _Right(_Erasednode))
                _Fixnodeparent = _Pnode;
            else {
                _Fixnodeparent = _Parent(_Pnode);
                if (!_Isnil(_Fixnode))
                    _Parent(_Fixnode) = _Fixnodeparent;
                _Left(_Fixnodeparent) = _Fixnode;
                _Right(_Pnode) = _Right(_Erasednode);
                _Parent(_Right(_Erasednode)) = _Pnode;
            }

            if (_Root() == _Erasednode)
                _Root() = _Pnode;
            else if (_Left(_Parent(_Erasednode)) == _Erasednode)
                _Left(_Parent(_Erasednode)) = _Pnode;
            else
                _Right(_Parent(_Erasednode)) = _Pnode;

            _Parent(_Pnode) = _Parent(_Erasednode);
            std::swap(_Color(_Pnode), _Color(_Erasednode));
        }

        if (_Color(_Erasednode) == _Black) {
            for (; _Fixnode != _Root() && _Color(_Fixnode) == _Black; _Fixnodeparent = _Parent(_Fixnode))
                if (_Fixnode == _Left(_Fixnodeparent)) {
                    _Pnode = _Right(_Fixnodeparent);
                    if (_Color(_Pnode) == _Red) {
                        _Color(_Pnode) = _Black;
                        _Color(_Fixnodeparent) = _Red;
                        _Lrotate(_Fixnodeparent);
                        _Pnode = _Right(_Fixnodeparent);
                    }

                    if (_Isnil(_Pnode))
                        _Fixnode = _Fixnodeparent;
                    else if (_Color(_Left(_Pnode)) == _Black && _Color(_Right(_Pnode)) == _Black) {
                        _Color(_Pnode) = _Red;
                        _Fixnode = _Fixnodeparent;
                    } else {
                        if (_Color(_Right(_Pnode)) == _Black) {
                            _Color(_Left(_Pnode)) = _Black;
                            _Color(_Pnode) = _Red;
                            _Rrotate(_Pnode);
                            _Pnode = _Right(_Fixnodeparent);
                        }

                        _Color(_Pnode) = _Color(_Fixnodeparent);
                        _Color(_Fixnodeparent) = _Black;
                        _Color(_Right(_Pnode)) = _Black;
                        _Lrotate(_Fixnodeparent);
                        break;
                    }
                } else {
                    _Pnode = _Left(_Fixnodeparent);
                    if (_Color(_Pnode) == _Red) {
                        _Color(_Pnode) = _Black;
                        _Color(_Fixnodeparent) = _Red;
                        _Rrotate(_Fixnodeparent);
                        _Pnode = _Left(_Fixnodeparent);
                    }
                    if (_Isnil(_Pnode))
                        _Fixnode = _Fixnodeparent;
                    else if (_Color(_Right(_Pnode)) == _Black && _Color(_Left(_Pnode)) == _Black) {
                        _Color(_Pnode) = _Red;
                        _Fixnode = _Fixnodeparent;
                    } else {
                        if (_Color(_Left(_Pnode)) == _Black) {
                            _Color(_Right(_Pnode)) = _Black;
                            _Color(_Pnode) = _Red;
                            _Lrotate(_Pnode);
                            _Pnode = _Left(_Fixnodeparent);
                        }

                        _Color(_Pnode) = _Color(_Fixnodeparent);
                        _Color(_Fixnodeparent) = _Black;
                        _Color(_Left(_Pnode)) = _Black;
                        _Rrotate(_Fixnodeparent);
                        break;
                    }
                }

            _Color(_Fixnode) = _Black;
        }

        this->_Alnod.destroy(_Erasednode);
        this->_Alnod.deallocate(_Erasednode, 1);

        if (0 < _Mysize)
            --_Mysize;

        return (_Where);
    }
#else
    iterator erase(iterator _Where);
#endif

protected:
    void _Erase(_Nodeptr _Rootnode)
    {
        for (_Nodeptr _Pnode = _Rootnode; !_Isnil(_Pnode); _Rootnode = _Pnode) {
            _Erase(_Right(_Pnode));
            _Pnode = _Left(_Pnode);
            this->_Alnod.destroy(_Rootnode);
            this->_Alnod.deallocate(_Rootnode, 1);
        }
    }

    void _Init()
    {
        _Myhead = _Buynode();
        _Isnil(_Myhead) = true;
        _Root() = _Myhead;
        _Lmost() = _Myhead, _Rmost() = _Myhead;
        _Mysize = 0;
    }

#ifdef NON_MATCHING
    iterator _Insert(bool _Addleft, _Nodeptr _Wherenode, const value_type& _Val)
    {
        if (max_size() - 1 <= _Mysize)
            throw length_error("map/set<T> too long");
        _Nodeptr _Newnode = _Buynode(_Myhead, _Wherenode, _Myhead, _Val, _Red);

        ++_Mysize;
        if (_Wherenode == _Myhead) {
            _Root() = _Newnode;
            _Lmost() = _Newnode, _Rmost() = _Newnode;
        } else if (_Addleft) {
            _Left(_Wherenode) = _Newnode;
            if (_Wherenode == _Lmost())
                _Lmost() = _Newnode;
        } else {
            _Right(_Wherenode) = _Newnode;
            if (_Wherenode == _Rmost())
                _Rmost() = _Newnode;
        }

        for (_Nodeptr _Pnode = _Newnode; _Color(_Parent(_Pnode)) == _Red;)
            if (_Parent(_Pnode) == _Left(_Parent(_Parent(_Pnode)))) {
                _Wherenode = _Right(_Parent(_Parent(_Pnode)));
                if (_Color(_Wherenode) == _Red) {
                    _Color(_Parent(_Pnode)) = _Black;
                    _Color(_Wherenode) = _Black;
                    _Color(_Parent(_Parent(_Pnode))) = _Red;
                    _Pnode = _Parent(_Parent(_Pnode));
                } else {
                    if (_Pnode == _Right(_Parent(_Pnode))) {
                        _Pnode = _Parent(_Pnode);
                        _Lrotate(_Pnode);
                    }
                    _Color(_Parent(_Pnode)) = _Black;
                    _Color(_Parent(_Parent(_Pnode))) = _Red;
                    _Rrotate(_Parent(_Parent(_Pnode)));
                }
            } else {
                _Wherenode = _Left(_Parent(_Parent(_Pnode)));
                if (_Color(_Wherenode) == _Red) {
                    _Color(_Parent(_Pnode)) = _Black;
                    _Color(_Wherenode) = _Black;
                    _Color(_Parent(_Parent(_Pnode))) = _Red;
                    _Pnode = _Parent(_Parent(_Pnode));
                } else {
                    if (_Pnode == _Left(_Parent(_Pnode))) {
                        _Pnode = _Parent(_Pnode);
                        _Rrotate(_Pnode);
                    }
                    _Color(_Parent(_Pnode)) = _Black;
                    _Color(_Parent(_Parent(_Pnode))) = _Red;
                    _Lrotate(_Parent(_Parent(_Pnode)));
                }
            }

        _Color(_Root()) = _Black;
        return (iterator(_Newnode));
    }
#else
    iterator _Insert(bool _Addleft, _Nodeptr _Wherenode, const value_type& _Val);
#endif

    _Nodeptr _Lbound(const key_type& _Keyval) const
    {
        _Nodeptr _Pnode = _Root();
        _Nodeptr _Wherenode = _Myhead;

        while (!_Isnil(_Pnode))
            if (this->comp(_Key(_Pnode), _Keyval))
                _Pnode = _Right(_Pnode);
            else {
                _Wherenode = _Pnode;
                _Pnode = _Left(_Pnode);
            }

        return (_Wherenode);
    }

    _Nodeptr& _Lmost() { return (_Left(_Myhead)); }
    _Nodeptr& _Lmost() const { return (_Left(_Myhead)); }

    void _Lrotate(_Nodeptr _Wherenode)
    {
        _Nodeptr _Pnode = _Right(_Wherenode);
        _Right(_Wherenode) = _Left(_Pnode);

        if (!_Isnil(_Left(_Pnode)))
            _Parent(_Left(_Pnode)) = _Wherenode;
        _Parent(_Pnode) = _Parent(_Wherenode);

        if (_Wherenode == _Root())
            _Root() = _Pnode;
        else if (_Wherenode == _Left(_Parent(_Wherenode)))
            _Left(_Parent(_Wherenode)) = _Pnode;
        else
            _Right(_Parent(_Wherenode)) = _Pnode;

        _Left(_Pnode) = _Wherenode;
        _Parent(_Wherenode) = _Pnode;
    }

    static _Nodeptr _Max(_Nodeptr _Pnode)
    {
        while (!_Isnil(_Right(_Pnode)))
            _Pnode = _Right(_Pnode);
        return (_Pnode);
    }

    static _Nodeptr _Min(_Nodeptr _Pnode)
    {
        while (!_Isnil(_Left(_Pnode)))
            _Pnode = _Left(_Pnode);
        return (_Pnode);
    }

    _Nodeptr& _Rmost() { return (_Right(_Myhead)); }
    _Nodeptr& _Rmost() const { return (_Right(_Myhead)); }
    _Nodeptr& _Root() { return (_Parent(_Myhead)); }
    _Nodeptr& _Root() const { return (_Parent(_Myhead)); }

    void _Rrotate(_Nodeptr _Wherenode)
    {
        _Nodeptr _Pnode = _Left(_Wherenode);
        _Left(_Wherenode) = _Right(_Pnode);

        if (!_Isnil(_Right(_Pnode)))
            _Parent(_Right(_Pnode)) = _Wherenode;
        _Parent(_Pnode) = _Parent(_Wherenode);

        if (_Wherenode == _Root())
            _Root() = _Pnode;
        else if (_Wherenode == _Right(_Parent(_Wherenode)))
            _Right(_Parent(_Wherenode)) = _Pnode;
        else
            _Left(_Parent(_Wherenode)) = _Pnode;

        _Right(_Pnode) = _Wherenode;
        _Parent(_Wherenode) = _Pnode;
    }

    _Nodeptr _Buynode();
    _Nodeptr _Buynode(_Nodeptr _Larg, _Nodeptr _Parg, _Nodeptr _Rarg, const value_type& _Val, char _Carg);
    void _Tidy();

    allocator_type _Alval;             // 0x02 (comp 0x00, _Alnod 0x01)
    _Nodeptr _Myhead;                  // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map
    : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct DiaryFile { // NAME: proto-pdb:default.pdb
    struct Entry { // NAME: proto-pdb:default.pdb
        char filename[48];             // 0x00
        unsigned int compressedSize;   // 0x30
        unsigned int uncompressedSize; // 0x34
        unsigned int offset;           // 0x38
        unsigned int reserved;         // 0x3C
    };

    unsigned int magic;                // 0x00
    unsigned int version;              // 0x04
    unsigned int numFiles;             // 0x08
    unsigned int reserved;             // 0x0C
};
DECOMP_SIZE_ASSERT(DiaryFile, 0x10);
DECOMP_SIZE_ASSERT(DiaryFile::Entry, 0x40);

void* BeepMalloc(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb
extern "C" int uncompress(unsigned char* dest, unsigned long* destLen, const unsigned char* source, unsigned long sourceLen); // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString MakeLower(const SwampString& s);
}

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    BeepFile();
    ~BeepFile();
    bool Open(const SwampString&);
    bool OpenForWrite(const SwampString&);
    bool Read(void*, unsigned int);
    bool Write(const void*, unsigned int);
    unsigned int Size();
    int Pos();
    void Seek(int);

    char field_0[0x20];                // 0x00
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    bool ParseRawData(const SwampString&, const SwampString&, int);
};

class SwampDiary { // NAME: proto-pdb:default.pdb
public:
    struct DiaryEntry { // NAME: proto-pdb:default.pdb
        DiaryFile::Entry summary;      // 0x00
        void* data;                    // 0x40
    };

    SwampDiary();
    ~SwampDiary();
    bool GetEntry(const SwampString& name, BeepTDF& tdf);

protected:
    bool LoadData(const SwampString& filename);
    bool SaveData(const SwampString& filename, int padToSize);
    void CleanUp();

    DiaryFile _currDiary;              // 0x00
    std::map<SwampString, DiaryEntry> _entries; // 0x10
};
DECOMP_SIZE_ASSERT(SwampDiary::DiaryEntry, 0x44);
DECOMP_SIZE_ASSERT(SwampDiary, 0x1C);

// FUNCTION: USA 0x00151D56
// PROTOTYPE: PREVIEW 0x0053B3D6 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry>::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry>(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,SwampDiary::DiaryEntry const &);

// FUNCTION: USA 0x00151D78
// PROTOTYPE: PREVIEW 0x0053B3F8 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry>::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry>(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> const &);

// FUNCTION: USA 0x00151D9A
// PROTOTYPE: PREVIEW 0x0053B41A identical
// NAME: proto-pdb:default.pdb
bool SwampDiary::GetEntry(const SwampString& name, BeepTDF& tdf)
{
    // !FAKE: c1xx naming; neutral stand-ins v0-v2; no PDB local (pdb_types locals lists bufSize, dataString only)
    // !FAKE: c2 register allocation; const_iterator copy of find's result; node kept in esi (match_one 75.17 -> 86.68)
    std::map<SwampString, DiaryEntry>::const_iterator v0 = _entries.find(BeepLib::MakeLower(name));
    if (v0 == _entries.end())
        return false;

    unsigned char* v1 = (unsigned char*)BeepMalloc(v0->second.summary.uncompressedSize, "SwampDiary.cpp", 225);
    unsigned long bufSize = v0->second.summary.uncompressedSize;
    int v2 = uncompress(v1, &bufSize, (const unsigned char*)v0->second.data, v0->second.summary.compressedSize);
    SwampString dataString((const char*)v1);
    BeepFree(v1);
    if (v2 != 0)
        return false;
    if (!tdf.ParseRawData(dataString, name, 0))
        return false;
    return true;
}

// FUNCTION: USA 0x00151E4A
// PROTOTYPE: PREVIEW 0x0053B4CA identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Node::_Node(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Node *,std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Node *,std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Node *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> const &,char);

// FUNCTION: USA 0x001519F7
// PROTOTYPE: PREVIEW 0x0053B077 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Rrotate(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Node *);

// FUNCTION: USA 0x00151A3B
// PROTOTYPE: PREVIEW 0x0053B0BB identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::const_iterator::_Inc();

// FUNCTION: USA 0x00151A71
// PROTOTYPE: PREVIEW 0x0053B0F1 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::const_iterator::_Dec();

// FUNCTION: USA 0x00151AB0
// PROTOTYPE: PREVIEW 0x0053B130 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Lrotate(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Node *);

// FUNCTION: USA 0x00151B7A
// PROTOTYPE: PREVIEW 0x0053B1FA identical
// NAME: proto-pdb:default.pdb
bool SwampDiary::SaveData(const SwampString& filename, int padToSize)
{
    BeepFile f;
    if (!f.OpenForWrite(filename))
        return false;

    DiaryFile header = { 0 };
    header.magic = 0x52414544;
    header.version = 1;
    header.numFiles = _entries.size();
    if (!f.Write(&header, sizeof(header)))
        return false;

    // !FAKE: c1xx naming; neutral stand-ins tmp, v0; no PDB local
    unsigned int tmp = f.Pos() + header.numFiles * sizeof(DiaryFile::Entry);
    for (std::map<SwampString, DiaryEntry>::iterator eit = _entries.begin(); eit != _entries.end(); ++eit) {
        // !FAKE: c2 register allocation; reference local; edi = &summary across the Write (match_one 94.68 -> 100)
        DiaryFile::Entry& v1 = eit->second.summary;
        v1.offset = tmp;
        if (!f.Write(&v1, sizeof(DiaryFile::Entry)))
            return false;
        tmp = (tmp + v1.compressedSize + 3) & ~3;
    }

    unsigned int zero = 0;
    for (std::map<SwampString, DiaryEntry>::const_iterator eit = _entries.begin(); eit != _entries.end(); ++eit) {
        unsigned int v0 = eit->second.summary.offset - f.Pos();
        if (v0 != 0 && !f.Write(&zero, v0))
            return false;
        if (!f.Write(eit->second.data, eit->second.summary.compressedSize))
            return false;
    }

    if (padToSize > (int)f.Size()) {
        f.Seek(padToSize - 1);
        f.Write(&zero, 1);
    }
    return true;
}

// FUNCTION: USA 0x00151CB9
// PROTOTYPE: PREVIEW 0x0053B339 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Init();

// FUNCTION: USA 0x00151CDE
// PROTOTYPE: PREVIEW 0x0053B35E identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Node * std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Lbound(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &)const;

// FUNCTION: USA 0x00151E7D
// PROTOTYPE: PREVIEW 0x0053B4FD identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100, bytes match; relink wall (4.4 T4.c6): throw data now pinned, but ??_7logic_error/??_7out_of_range
// are also emitted by BeepDebug.obj; relink same() does not merge copies whose relocs name a weak ??_E slot (relink.py:1071-1073)
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::iterator std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::erase(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::iterator);
#endif

// FUNCTION: USA 0x001520F6
// PROTOTYPE: PREVIEW 0x0053B776 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Erase(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Node *);

// FUNCTION: USA 0x001521C2
// PROTOTYPE: PREVIEW 0x0053B842 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100, bytes match; relink wall (4.4 T4.c6): throw data now pinned, but ??_7logic_error/??_7out_of_range
// are also emitted by BeepDebug.obj; relink same() does not merge copies whose relocs name a weak ??_E slot (relink.py:1071-1073)
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::iterator std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Insert(bool,std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::_Node *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> const &);
#endif

// FUNCTION: USA 0x0015239E
// PROTOTYPE: PREVIEW 0x0053BA1E identical
// NAME: proto-pdb:default.pdb
template std::pair<std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::iterator,bool> std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,SwampDiary::DiaryEntry,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> >,0> >::insert(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,SwampDiary::DiaryEntry> const &);

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

// FUNCTION: USA 0x0015246A
// PROTOTYPE: PREVIEW 0x0053BAEA identical
// NAME: proto-pdb:default.pdb
bool SwampDiary::LoadData(const SwampString& filename)
{
    CleanUp();
    BeepFile f;
    if (!f.Open(filename))
        return false;
    if (!f.Read(&_currDiary, sizeof(DiaryFile)))
        return false;
    if (_currDiary.magic != 0x52414544 || _currDiary.version != 1)
        return false;

    // !FAKE: c1xx naming; neutral stand-in v0; no PDB local
    for (unsigned int v0 = 0; v0 < _currDiary.numFiles; ++v0) {
        DiaryEntry entry;
        if (!f.Read(&entry.summary, sizeof(DiaryFile::Entry)))
            return false;
        entry.data = BeepMalloc(entry.summary.compressedSize, "SwampDiary.cpp", 79);
        unsigned int fileLoc = f.Pos();
        f.Seek(entry.summary.offset);
        if (!f.Read(entry.data, entry.summary.compressedSize))
            return false;
        f.Seek(fileLoc);
        _entries.insert(std::pair<const SwampString, DiaryEntry>(entry.summary.filename, entry));
    }
    return true;
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

