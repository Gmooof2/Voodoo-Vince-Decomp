// UNIT: src/Apps/Vince/core/FontManager.cpp OBJ: core\XBox\OfficialRelease\FontManager.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

class exception { // NAME: proto-pdb:default.pdb
public:
    exception(const exception& v1); // out of line in retail (logic_error copy ctor calls it)
    virtual ~exception();
    virtual const char* what() const;

private:
    const char* _m_what;               // 0x04
    int _m_doFree;                     // 0x08
};

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string() { _Myres = _BUF_SIZE - 1; _Eos(0); }
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& operator+=(const E* _Ptr) { return append(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, npos); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& append(const E* _Ptr);
    const E* c_str() const { return _Myptr(); }
    bool empty() const { return (_Mysize == 0); }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Eos(unsigned int _Newsize);
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    char _Alval;                       // 0x00
    union {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class T, class A = allocator<T> > class vector {
public:
    class const_iterator {
    public:
        const_iterator(T* _Ptr) : _Myptr(_Ptr) {}
        const T& operator*() const { return *_Myptr; }
        const_iterator& operator++() { ++_Myptr; return *this; }
        bool operator!=(const const_iterator& _Right) const { return _Myptr != _Right._Myptr; }
        T* _Myptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(T* _Ptr) : const_iterator(_Ptr) {}
        T& operator*() const { return *this->_Myptr; }
        T* operator->() const { return (&**this); }
        iterator& operator++() { ++this->_Myptr; return *this; }
    };
    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; }
    iterator begin() { return iterator(_Myfirst); }
    const_iterator begin() const { return const_iterator(_Myfirst); }
    iterator end() { return iterator(_Mylast); }
    const_iterator end() const { return const_iterator(_Mylast); }
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    ~vector() { _Tidy(); }
    void push_back(const T& _Val);

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class T1, class T2> struct pair {
    pair(const T1& _Val1, const T2& _Val2) : first(_Val1), second(_Val2) {}
    T1 first;                          // 0x00
    T2 second;
};
template <class E, class T, class A>
bool operator<(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right);
template <class T> inline void swap(T& _Left, T& _Right)
{
    T _Tmp = _Left;
    _Left = _Right, _Right = _Tmp;
}
class logic_error : public exception { // NAME: proto-pdb:default.pdb
public:
    explicit logic_error(const basic_string<char, char_traits<char>, allocator<char> >& _Message);
    virtual ~logic_error();
    virtual const char* what() const;

private:
    basic_string<char, char_traits<char>, allocator<char> > _Str; // 0x0C
};
class length_error : public logic_error { // NAME: proto-pdb:default.pdb
public:
    explicit length_error(const basic_string<char, char_traits<char>, allocator<char> >& _Message) : logic_error(_Message) {}
    virtual ~length_error() {}
};
class out_of_range : public logic_error { // NAME: proto-pdb:default.pdb
public:
    explicit out_of_range(const basic_string<char, char_traits<char>, allocator<char> >& _Message) : logic_error(_Message) {}
    virtual ~out_of_range() {}
};
template <class T> inline void _Destroy(T* _Ptr) { (_Ptr)->~T(); }
template <class T> class allocator { // VC 7.1 xmemory, the members the _Tree rows use
public:
    typedef T* pointer;
    typedef unsigned int size_type;
    template <class U> struct rebind {
        typedef allocator<U> other;
    };
    void deallocate(pointer _Ptr, size_type) { operator delete(_Ptr); }
    void destroy(pointer _Ptr) { _Destroy(_Ptr); }
    unsigned int max_size() const
    {
        unsigned int _Count = (unsigned int)(-1) / sizeof(T);
        return (0 < _Count ? _Count : 1);
    }
};
struct _Nonscalar_ptr_iterator_tag {};
template <class _Ty, class _Alloc> inline void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_First)
        _Al.destroy(_First);
}
template <class _FwdIt, class _Ty> inline void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}
template <class _InIt, class _OutIt> inline _OutIt _Copy_opt(_InIt _First, _InIt _Last, _OutIt _Dest, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_Dest, ++_First)
        *_Dest = *_First;
    return (_Dest);
}
template <class _BidIt1, class _BidIt2> inline _BidIt2 _Copy_backward_opt(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Nonscalar_ptr_iterator_tag)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}
template <class T> struct less {
    bool operator()(const T& _Left, const T& _Right) const { return (_Left < _Right); }
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
    typedef Pr key_compare;
    typedef typename A::template rebind<value_type>::other allocator_type;
    enum { _Multi = Mfl };
    static const K& _Kfn(const value_type& _Val) { return (_Val.first); }
    Pr comp;                           // 0x00
};
template <class Traits> class _Tree_nod : public Traits {
protected:
    typedef typename Traits::allocator_type allocator_type;
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
    typename allocator_type::template rebind<_Node>::other _Alnod; // 0x01
};
template <class Traits> class _Tree_ptr : public _Tree_nod<Traits> {
protected:
    typedef typename _Tree_nod<Traits>::_Node _Node;
    typedef typename Traits::allocator_type allocator_type;
    typedef typename allocator_type::template rebind<_Node>::other::pointer _Nodeptr;
    typename allocator_type::template rebind<_Nodeptr>::other _Alptr; // 0x02
};
template <class Traits> class _Tree_val : public _Tree_ptr<Traits> {
protected:
    typedef typename Traits::allocator_type allocator_type;
    allocator_type _Alval;             // 0x03
};
template <class Traits> class _Tree : public _Tree_val<Traits> {
public:
    typedef _Tree_val<Traits> _Mybase;
    typedef typename Traits::key_type key_type;
    typedef typename Traits::value_type value_type;
    typedef typename Traits::allocator_type allocator_type;

protected:
    typedef typename _Tree_nod<Traits>::_Node _Node;
    enum _Redbl { _Red, _Black };
    typedef _Node* _Nodeptr;

    static char& _Color(_Nodeptr _Pnode) { return ((char&)(*_Pnode)._Color); }
    static char& _Isnil(_Nodeptr _Pnode) { return ((char&)(*_Pnode)._Isnil); }
    static const key_type& _Key(_Nodeptr _Pnode) { return (_Mybase::_Kfn(_Myval(_Pnode))); }
    static _Nodeptr& _Left(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Left); }
    static _Nodeptr& _Parent(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Parent); }
    static _Nodeptr& _Right(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Right); }
    static value_type& _Myval(_Nodeptr _Pnode) { return ((value_type&)(*_Pnode)._Myval); }

public:
    typedef unsigned int size_type;

    class const_iterator;
    friend class const_iterator;

    class const_iterator {
    public:
        const_iterator() : _Ptr(0) {}
        const_iterator(_Nodeptr _Pnode) : _Ptr(_Pnode) {}
        const_iterator& operator++()
        {
            _Inc();
            return (*this);
        }
        const_iterator& operator--()
        {
            _Dec();
            return (*this);
        }
        bool operator==(const const_iterator& _Right) const { return (_Ptr == _Right._Ptr); }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }

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

        const value_type& operator*() const { return (_Myval(_Ptr)); }
        const value_type* operator->() const { return (&**this); }
        _Nodeptr _Mynode() const { return (_Ptr); }

    protected:
        _Nodeptr _Ptr;
    };

    class iterator;
    friend class iterator;

    class iterator : public const_iterator {
    public:
        iterator() {}
        iterator(_Nodeptr _Pnode) : const_iterator(_Pnode) {}
        iterator& operator++()
        {
            ++(*(const_iterator*)this);
            return (*this);
        }
        iterator& operator--()
        {
            --(*(const_iterator*)this);
            return (*this);
        }
    };

    typedef pair<iterator, bool> _Pairib;

    _Tree() { _Init(); }
    ~_Tree() { _Tidy(); }
    iterator begin() { return (iterator(_Lmost())); }
    iterator end() { return (iterator(_Myhead)); }
    size_type size() const { return (_Mysize); }
    size_type max_size() const { return (this->_Alval.max_size()); }
    void clear();
    iterator find(const key_type& _Keyval);

    _Pairib insert(const value_type& _Val)
    {
        _Nodeptr _Trynode = _Root();
        _Nodeptr _Wherenode = _Myhead;
        bool _Addleft = true;
        while (!_Isnil(_Trynode)) {
            _Wherenode = _Trynode;
            _Addleft = this->comp(this->_Kfn(_Val), _Key(_Trynode));
            _Trynode = _Addleft ? _Left(_Trynode) : _Right(_Trynode);
        }

        if (this->_Multi)
            return (_Pairib(_Insert(_Addleft, _Wherenode, _Val), true));
        else {
            iterator _Where = iterator(_Wherenode);
            if (!_Addleft)
                ;
            else if (_Where == begin())
                return (_Pairib(_Insert(true, _Wherenode, _Val), true));
            else
                --_Where;

            if (this->comp(_Key(_Where._Mynode()), this->_Kfn(_Val)))
                return (_Pairib(_Insert(_Addleft, _Wherenode, _Val), true));
            else
                return (_Pairib(_Where, false));
        }
    }

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

protected:
    void _Copy(const _Tree& _Right)
    {
        _Root() = _Copy(_Right._Root(), _Myhead);
        _Mysize = _Right.size();
        if (!_Isnil(_Root())) {
            _Lmost() = _Min(_Root());
            _Rmost() = _Max(_Root());
        } else
            _Lmost() = _Myhead, _Rmost() = _Myhead;
    }

    _Nodeptr _Copy(_Nodeptr _Rootnode, _Nodeptr _Wherenode);

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

    _Nodeptr _Myhead;                  // 0x04
    size_type _Mysize;                 // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > >
class map : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
typedef std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > SwampWString;

struct Vector2 { // NAME: proto-pdb:default.pdb
    Vector2() {}
    Vector2(float u_, float v_) { u = u_; v = v_; }

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

class CXBFont { // NAME: proto-pdb:default.pdb
public:
    ~CXBFont();
    long Destroy();
};

class FontManager { // NAME: proto-pdb:default.pdb
public:
    struct LangInfo { // NAME: proto-pdb:default.pdb
        LangInfo();

        Vector2 scaling;               // 0x00
        float verticalSpacing;         // 0x08
    };
    typedef std::map<SwampString, CXBFont*> FontMap;       // NAME: proto-pdb:default.pdb
    typedef std::map<SwampString, LangInfo> LangInfoMap;   // NAME: proto-pdb:default.pdb
    typedef std::map<SwampString, LangInfoMap> LocInfoMap; // NAME: proto-pdb:default.pdb

    FontManager();
    ~FontManager();
    CXBFont* Load(const char*);

    FontMap _fontMap;                  // 0x00
    LocInfoMap _locInfo;               // 0x0C
};
DECOMP_SIZE_ASSERT(FontManager::LangInfo, 0xC);
DECOMP_SIZE_ASSERT(FontManager, 0x18);

// FUNCTION: USA 0x000FDE05
// PROTOTYPE: PREVIEW 0x004E8C61 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> > > > >,0> >::const_iterator::_Dec();

// FUNCTION: USA 0x000FDFDD
// PROTOTYPE: PREVIEW 0x004A2B45 identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> > > > >,0> >::_Node * std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> > > > >,0> >::_Lbound(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &)const;

// FUNCTION: USA 0x000FE9D3
// PROTOTYPE: PREVIEW 0x004E9589 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> >,0> >::_Copy(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> >,0> > const &);

// FUNCTION: USA 0x000FF321
// PROTOTYPE: PREVIEW 0x004E9ECE identical
// NAME: proto-pdb:default.pdb
FontManager::~FontManager()
{
    for (FontMap::iterator it = _fontMap.begin(); it != _fontMap.end(); ++it) {
        it->second->Destroy();
        delete it->second;
    }
    _fontMap.clear();
}
