// UNIT: src/Apps/Vince/core/SignTrolley.cpp OBJ: core\XBox\OfficialRelease\SignTrolley.obj OPT: /O1 /Oy
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

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0012CDF8
// PROTOTYPE: PREVIEW 0x00515827 identical
// NAME: proto-pdb:default.pdb
Vector2 Normalize(const Vector2& vec)
{
    float r = FastReciprocalSquareRoot(vec.u * vec.u + vec.v * vec.v);
    return Vector2(vec.u * r, vec.v * r);
}

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
#ifdef NON_MATCHING // D78, D117: default.pdb records no free Vector3 operator (5.2 T20)
inline Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
#endif

struct AABox; // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb
class SwampScript;

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    SwampScript* GetScript() const;
    void RemoveTreeFromScene();
    void Destroy();

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetBlockName() const { return _blockname; }
    const std::vector<BeepTDF*>& GetBlocks() const { return _blocklist; }
    const BeepTDF* FindBlock(const SwampString& blockname, bool canfail) const;
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
    bool GetBoolRecord(const SwampString& id, bool def) const;

    SwampString _blockname;            // 0x00
    char field_1C[0x3C - 0x1C];        // 0x1C
    std::vector<BeepTDF*> _blocklist;  // 0x3C
    char field_4C[0x58 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString MakeLower(const SwampString& s);
SwampString StripPath(const SwampString& s);
}
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum CollisionType { // NAME: proto-pdb:default.pdb
        HorizontalCollision = 0,
        VerticalCollision = 1
    };
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04
    virtual void Activate();                                       // vtable+0x08
    virtual void Deactivate();                                     // vtable+0x0C
    virtual void FrameTick();                                      // vtable+0x10
    virtual void TimerEvent(unsigned int);                         // vtable+0x14
    virtual void AnimationFinished();                              // vtable+0x18
    virtual void SecondaryAnimationFinished();                     // vtable+0x1C
    virtual void AnimationInterrupted();                           // vtable+0x20
    virtual void SecondaryAnimationInterrupted();                  // vtable+0x24
    virtual void AnimationTrigger(const SwampString&, float, float, unsigned int); // vtable+0x28
    virtual bool LosingControl(const SwampString&);                // vtable+0x2C
    virtual void GainingControl(SwampScript*);                     // vtable+0x30
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    virtual void EnteringRegion(SwampSceneNode*);                  // vtable+0x38
    virtual void LeavingRegion(SwampSceneNode*);                   // vtable+0x3C
    virtual void SoundFinished(const SwampString&);                // vtable+0x40
    virtual void SoundStreamerFinished(const SwampString&);        // vtable+0x44
    virtual void MovieFinished(const SwampString&);                // vtable+0x48
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x50
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual SwampString Save();                                    // vtable+0x58
    virtual void SetNode(SwampSceneNode*);                         // vtable+0x5C
    virtual void InvalidateScript();                               // vtable+0x60
    void RegisterFrameTick();
    void UnRegisterFrameTick();

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x2C - 0x24];        // 0x24
    float _length;                     // 0x2C
    char field_30[0x34 - 0x30];        // 0x30
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

// visible body: cl 13.10 /O1 keeps a handle local unescaped across operator= only with it (C0136; as LevelLoader.cpp)
template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0012D02D
// PROTOTYPE: PREVIEW 0x00515A5C identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012CFBA
// PROTOTYPE: PREVIEW 0x005159E9 identical
// NAME: proto-pdb:default.pdb
class Sprite { // NAME: proto-pdb:default.pdb
public:
    Sprite();
    void SetTexture(const SwampString& name);

    Vector2 _loc;                      // 0x00
    int _w;                            // 0x08
    int _h;                            // 0x0C
    Vector2 _scale;                    // 0x10
    ResourceHandle<TextureResource> _texture; // 0x18
    bool _centered;                    // 0x1C
    float _alpha;                      // 0x20
    Vector2 _uv[4];                    // 0x24
    unsigned int _color;               // 0x44
    float _rotation;                   // 0x48
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

inline void* operator new(unsigned int, void* p) { return p; }

// FUNCTION: USA 0x0012D0FE
// PROTOTYPE: PREVIEW 0x00515B2D identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012D11A
// PROTOTYPE: PREVIEW 0x00515B49 identical
// NAME: proto-pdb:default.pdb
struct LocInfo { // NAME: proto-pdb:default.pdb
    SwampString _levelName;            // 0x00
    SwampString _areaName;             // 0x1C
    SwampString _trolleyName;          // 0x38
};
DECOMP_SIZE_ASSERT(LocInfo, 0x54);

// FUNCTION: USA 0x0012DD61
// PROTOTYPE: PREVIEW 0x005166F3 identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Node::_Node(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Node *,std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Node *,std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Node *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> const &,char);

// FUNCTION: USA 0x0012DAE8
// PROTOTYPE: PREVIEW 0x0051647A identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::iterator std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::erase(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::iterator);

// FUNCTION: USA 0x0012E2D6
// PROTOTYPE: PREVIEW 0x00516D0F identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::iterator std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Insert(bool,std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Node *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> const &);

// FUNCTION: USA 0x0012CEBF
// PROTOTYPE: PREVIEW 0x005158EE identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::const_iterator::_Inc();

// FUNCTION: USA 0x0012CEF5
// PROTOTYPE: PREVIEW 0x00515924 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Lrotate(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Node *);

// FUNCTION: USA 0x0012CF37
// PROTOTYPE: PREVIEW 0x00515966 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Rrotate(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Node *);

// FUNCTION: USA 0x0012CF7B
// PROTOTYPE: PREVIEW 0x005159AA identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::const_iterator::_Dec();

// FUNCTION: USA 0x0012D0D9
// PROTOTYPE: PREVIEW 0x00515B08 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Init();

// FUNCTION: USA 0x0012D1B8
// PROTOTYPE: PREVIEW 0x00515BE7 identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Node * std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Lbound(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &)const;

// FUNCTION: USA 0x0012E034
// PROTOTYPE: PREVIEW 0x005169C6 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Erase(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::_Node *);

// FUNCTION: USA 0x0012E517
// PROTOTYPE: PREVIEW 0x00516F50 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::iterator,bool> std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,LocInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> >,0> >::insert(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,LocInfo> const &);

class TrolleyScreen; // NAME: proto-pdb:default.pdb

class TrolleyManager { // NAME: proto-pdb:default.pdb
public:
    class MapStop;                     // NAME: proto-pdb:default.pdb

    TrolleyManager();
    ~TrolleyManager();
    void Add(const SwampString& stopname, const SwampString& levelname, const SwampString& zonename, const SwampString& trolleyname);
    void Load(const BeepTDF& tdf);
    SwampString Save();

protected:
    void ReadTDF();

public:

    std::map<SwampString, LocInfo> _activeStops; // 0x00
    std::vector<MapStop> _mapStops;    // 0x0C
    TrolleyScreen* _screen;            // 0x1C
};
DECOMP_SIZE_ASSERT(TrolleyManager, 0x20);

extern TrolleyManager* g_TrolleyManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0012D15A
// PROTOTYPE: PREVIEW 0x00515B89 identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012DDD5
// PROTOTYPE: PREVIEW 0x00516767 identical
// NAME: proto-pdb:default.pdb
class TrolleyManager::MapStop { // NAME: proto-pdb:default.pdb
public:
    SwampString _name;                 // 0x00
    Vector2 _screenLoc;                // 0x1C
    SwampString _spriteName;           // 0x24
};
DECOMP_SIZE_ASSERT(TrolleyManager::MapStop, 0x40);

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xD4 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampCamera();
    char field_A4[0x10C - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GatorNode; // NAME: proto-pdb:default.pdb

class RootNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xB0 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(RootNode, 0xB0);

class MusicManager { // NAME: proto-pdb:default.pdb
public:
    void PlayTrack(const SwampString&);
    void EndTrack();
};
extern MusicManager* g_MusicManager; // NAME: proto-pdb:default.pdb

class TrolleyScreen : public UINode { // NAME: proto-pdb:default.pdb
public:
    struct MapInfo;                    // NAME: proto-pdb:default.pdb

    void AddLocation(const SwampString& name, const Vector2& loc, const SwampString& spritename, bool current);
    virtual void Invalidate();

    char field_D4[0xE0 - 0xD4];        // 0xD4
    std::vector<MapInfo> _locations;   // 0xE0
    Sprite _mapTexture;                // 0xF0
    std::vector<Sprite> _areaSelect;   // 0x13C
    int _startLoc;                     // 0x14C
    int _curLoc;                       // 0x150
    char field_154[0x164 - 0x154];     // 0x154
    GatorNode* _trolley;               // 0x164
    RootNode* _rootNode;               // 0x168
    SwampCamera _cam;                  // 0x16C
    char field_278[0x2BC - 0x278];     // 0x278
    SwampString _curTrack;             // 0x2BC
};
DECOMP_SIZE_ASSERT(TrolleyScreen, 0x2D8);

// FUNCTION: USA 0x0012D192
// PROTOTYPE: PREVIEW 0x00515BC1 identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012DD94
// PROTOTYPE: PREVIEW 0x00516726 identical
// NAME: proto-pdb:default.pdb
struct TrolleyScreen::MapInfo { // NAME: proto-pdb:default.pdb
    SwampString _name;                 // 0x00
    SwampWString _screenName;          // 0x1C
    Vector2 _loc;                      // 0x38
    Sprite _sprite;                    // 0x40
};
DECOMP_SIZE_ASSERT(TrolleyScreen::MapInfo, 0x8C);

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_STATIC = 0x4
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim._Resource->_length; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x38 - 0x28];        // 0x28
    float _time;                       // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

// src proto-pdb:default.pdb CollidableCylinder base CollidableNode : SwampSceneNode, at 0x0; CollidableNode not modelled
class CollidableCylinder : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xAC - 0xA4];        // 0xA4
    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
};

// Retail layout: Critter _CritterType 0x258 -> 0x268 (+0x10).
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_B4[0x268 - 0xB4];       // 0xB4
    CritterType _CritterType;          // 0x268
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class Level { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xAC];                // 0x00
    SwampString _levelName;            // 0xAC
    SwampString _areaName;             // 0xC8
    char field_E4[0x140 - 0xE4];       // 0xE4
};
DECOMP_SIZE_ASSERT(Level, 0x140);

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    // Slot is 0x8C here (retail size() divides by 0x8C, proto PDB size); body opaque.
    struct Slot { // NAME: proto-pdb:default.pdb
        char field_0[0x8C];            // 0x00
    };

    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }

    char field_0[0x4];                 // 0x00
    Level* _curLevel;                  // 0x04
};

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
    Critter* GetCritter() { return (Critter*)_node; }
    char field_4C[0xBC - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class SignTrolleyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ACTIVATESTOP = 0x14,
        MESSAGE_GETPADNODE = 0x15
    };
    SignTrolleyMain(SwampSceneNode* node);
    virtual bool TestActivate() const;
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    void ActivateStop();
    virtual void FrameTick();
    virtual SwampString Save();

    ResourceHandle<AnimationResource> _animActive; // 0xBC
    Critter* _trolley;                 // 0xC0
    bool _tagged;                      // 0xC4
    SwampString _stopName;             // 0xC8
    SwampString _trolleyName;          // 0xE4
    SwampScript* _padScript;           // 0x100

private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(SignTrolleyMain, 0x104);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

const SwampString format(const char* fmt, ...); // NAME: proto-pdb:default.pdb

class LocalizedStrings { // NAME: proto-pdb:default.pdb
public:
    static LocalizedStrings& Instance();
    const unsigned short* GetGameString(const SwampString& id);
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

// FUNCTION: USA 0x0012CDB0
// PROTOTYPE: PREVIEW 0x005157B9 identical
// NAME: proto-pdb:default.pdb
bool SignTrolleyMain::TestActivate() const
{
    if (_trolley != 0 && _tagged)
        return true;
    return false;
}

// The compiler generates the Sprite copy constructor here; it has no body in source.
// FUNCTION: USA 0x0012CE60
// PROTOTYPE: PREVIEW 0x0051588F identical
// NAME: proto-pdb:default.pdb

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~MapStop in this unit (no matched function
// odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapStop_dtor(TrolleyManager::MapStop* p)
{
    p->TrolleyManager::MapStop::~MapStop();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit LocInfo default ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LocInfo_ctor(void* p)
{
    new (p) LocInfo;
}

// FUNCTION: USA 0x0012D176
// PROTOTYPE: PREVIEW 0x00515BA5 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapInfo default ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapInfo_ctor(void* p)
{
    new (p) TrolleyScreen::MapInfo;
}

// !FAKE: c1xx front end; scaffold call; retail emits the Sprite scalar deleting dtor in this unit (no matched
// function odr-uses it); this delete odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Sprite_delete(Sprite* p)
{
    delete p;
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~MapInfo in this unit (no matched function
// odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapInfo_dtor(TrolleyScreen::MapInfo* p)
{
    p->TrolleyScreen::MapInfo::~MapInfo();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~LocInfo in this unit (no matched function
// odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LocInfo_dtor(LocInfo* p)
{
    p->LocInfo::~LocInfo();
}

// FUNCTION: USA 0x0012D241
// PROTOTYPE: PREVIEW 0x00515C70 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit LocInfo copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LocInfo_copy(void* p, const LocInfo& v)
{
    new (p) LocInfo(v);
}

// FUNCTION: USA 0x0012D2EA
// PROTOTYPE: PREVIEW 0x00515D19 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapStop copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapStop_copy(void* p, const TrolleyManager::MapStop& v)
{
    new (p) TrolleyManager::MapStop(v);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapStop::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapStop_assign(TrolleyManager::MapStop* p, const TrolleyManager::MapStop& v)
{
    *p = v;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2 = vptr store, literal)
// FUNCTION: USA 0x0012D338
// PROTOTYPE: PREVIEW 0x00515D67 near-identical
// NAME: proto-pdb:default.pdb
SignTrolleyMain::SignTrolleyMain(SwampSceneNode* node) : CritterMain(node), _tagged(false), _padScript(0)
{
    ((Critter*)_node)->_CritterType = CRITTER_TYPE_STATIC;
    _animActive = g_swamp->GetAnimationManager()->Load("signtrolley_activate");
}
#endif

// FUNCTION: USA 0x0012D3EE
// PROTOTYPE: PREVIEW 0x00515E1D identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~SignTrolleyMain in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_SignTrolleyMain_dtor(SignTrolleyMain* p)
{
    p->SignTrolleyMain::~SignTrolleyMain();
}

// FUNCTION: USA 0x0012D424
// PROTOTYPE: PREVIEW 0x00515E53 identical
// NAME: proto-pdb:default.pdb
SwampString SignTrolleyMain::Save()
{
    SwampString str = CritterMain::Save();
    str += format("\t\ttagged = %d;\n", _tagged);
    str += format("\t\tstopname = %s;\n", _stopName.c_str());
    str += format("\t\ttrolleyname = %s;\n", _trolleyName.c_str());
    return str;
}

// FUNCTION: USA 0x0012D5A6
// PROTOTYPE: PREVIEW 0x00515F32 identical
// NAME: proto-pdb:default.pdb
SwampString TrolleyManager::Save()
{
    SwampString s;
    s += format("[trolleys]\n{\n");
    for (std::map<SwampString, LocInfo>::const_iterator it = _activeStops.begin(); it != _activeStops.end(); ++it) {
        s += format("\t[%s]\n\t{\n", it->first.c_str());
        LocInfo info = it->second;
        s += format("\t\tlevel = %s;\n", info._levelName.c_str());
        s += format("\t\tarea = %s;\n", info._areaName.c_str());
        s += format("\t\ttrolley = %s;\n", info._trolleyName.c_str());
        s += format("\t}\n");
    }
    s += "}\n";
    return s;
}

// FUNCTION: USA 0x0012D26E
// PROTOTYPE: PREVIEW 0x00515C9D identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapInfo copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapInfo_copy(void* p, const TrolleyScreen::MapInfo& v)
{
    new (p) TrolleyScreen::MapInfo(v);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapInfo::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapInfo_assign(TrolleyScreen::MapInfo* p, const TrolleyScreen::MapInfo& v)
{
    *p = v;
}

// FUNCTION: USA 0x0012DFC4
// PROTOTYPE: PREVIEW 0x00516956 identical
// NAME: proto-pdb:default.pdb
SwampScript* SignTrolleyMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\SignTrolley.h", 23) SignTrolleyMain(node);
}

// FUNCTION: USA 0x0012EADE
// PROTOTYPE: PREVIEW 0x00517517 identical
// NAME: proto-pdb:default.pdb
void TrolleyManager::Add(const SwampString& stopname, const SwampString& levelname, const SwampString& zonename, const SwampString& trolleyname)
{
    SwampString lowername = BeepLib::MakeLower(stopname);
    std::map<SwampString, LocInfo>::iterator it = _activeStops.find(lowername);
    LocInfo info;
    info._areaName = zonename;
    info._levelName = BeepLib::StripPath(levelname);
    info._trolleyName = trolleyname;
    _activeStops.insert(std::pair<const SwampString, LocInfo>(lowername, info));
}

// FUNCTION: USA 0x0012EC2D
// PROTOTYPE: PREVIEW 0x00517666 identical
// NAME: proto-pdb:default.pdb
TrolleyManager::~TrolleyManager()
{
    g_TrolleyManager = 0;
}

// FUNCTION: USA 0x0012F05E
// PROTOTYPE: PREVIEW 0x00517A97 identical
// NAME: proto-pdb:default.pdb
TrolleyManager::TrolleyManager()
{
    g_TrolleyManager = this;
}

// FUNCTION: USA 0x0012D75C
// PROTOTYPE: PREVIEW 0x005160E8 identical
// NAME: proto-pdb:default.pdb
void TrolleyScreen::Invalidate()
{
    if (_curTrack.empty())
        g_MusicManager->EndTrack();
    else
        g_MusicManager->PlayTrack(_curTrack);
    _trolley->RemoveTreeFromScene();
    _trolley->Invalidate();
    _trolley = 0;
    _rootNode->Destroy();
    _rootNode = 0;
    _mapTexture.SetTexture("");
    // !FAKE: c1xx front end; neutral stand-in local; the iterator has no PDB local name
    for (std::vector<MapInfo>::iterator it = _locations.begin(); it != _locations.end(); ++it)
        it->_sprite.SetTexture("");
}

// FUNCTION: USA 0x0012F27D
// PROTOTYPE: PREVIEW 0x00517CB0 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~TrolleyScreen in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_TrolleyScreen_dtor(TrolleyScreen* p)
{
    p->TrolleyScreen::~TrolleyScreen();
}

// FUNCTION: USA 0x0012F2C5
// PROTOTYPE: PREVIEW 0x00517CF8 identical
// NAME: proto-pdb:default.pdb
void TrolleyScreen::AddLocation(const SwampString& name, const Vector2& loc, const SwampString& spritename, bool current)
{
    if (current)
        _startLoc = _curLoc = _locations.size();
    MapInfo info;
    info._name = name;
    info._screenName = LocalizedStrings::Instance().GetGameString(name);
    info._loc = loc;
    info._sprite.SetTexture(spritename);
    info._sprite._loc = loc;
    info._sprite._alpha = 0.0f;
    info._sprite._centered = true;
    _locations.push_back(info);
}

// FUNCTION: USA 0x0012F36D
// PROTOTYPE: PREVIEW 0x00517DA0 identical
// NAME: proto-pdb:default.pdb
void TrolleyManager::Load(const BeepTDF& tdf)
{
    ReadTDF();
    _activeStops.clear();
    const BeepTDF* stops = tdf.FindBlock("Trolleys", false);
    // !FAKE: c1xx front end; neutral stand-in local; the iterator has no PDB local name
    for (std::vector<BeepTDF*>::const_iterator it = stops->GetBlocks().begin(); it != stops->GetBlocks().end(); ++it) {
        LocInfo info;
        SwampString stopName = (*it)->GetBlockName();
        SwampString levelName = (*it)->GetStringRecord("level", "");
        SwampString areaName = (*it)->GetStringRecord("area", "");
        SwampString trolleyName = (*it)->GetStringRecord("trolley", "");
        Add(stopName, levelName, areaName, trolleyName);
    }
}

// FUNCTION: USA 0x0012F595
// PROTOTYPE: PREVIEW 0x00517FC8 identical
// NAME: proto-pdb:default.pdb
void SignTrolleyMain::ActivateStop()
{
    _tagged = true;
    UnRegisterFrameTick();
    GetCritter()->StartAnimation(_animActive, true, false);
    // !FAKE: c1xx front end; stand-in local; the PDB records no local here, retail calls Instance() once for both names
    Level* p = LevelLoader::Instance().GetCurLevel();
    g_TrolleyManager->Add(_stopName, p->_levelName, p->_areaName, _trolleyName);
}

// FUNCTION: USA 0x0012F5E7
// PROTOTYPE: PREVIEW 0x0051801A identical
// NAME: proto-pdb:default.pdb
void SignTrolleyMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 6:
        _trolley = pManager->Find(_trolleyName);
        _trolley->GetScript()->Message(this, 7, 0);
        break;
    case 7:
        _padScript = source;
        break;
    case MESSAGE_ACTIVATESTOP:
        if (!_tagged)
            ActivateStop();
        break;
    case MESSAGE_GETPADNODE:
        *(SwampSceneNode**)data = _padScript->_node;
        break;
    }
}

// FUNCTION: USA 0x0012F65C
// PROTOTYPE: PREVIEW 0x0051808F identical
// NAME: proto-pdb:default.pdb
void SignTrolleyMain::Load(const BeepTDF& tdf)
{
    CritterMain::Load(tdf);
    _tagged = tdf.GetBoolRecord("tagged", false);
    _stopName = BeepLib::MakeLower(LevelLoader::Instance().GetCurLevel()->_areaName);
    _trolleyName = tdf.GetStringRecord("trolleyname", "trolley");
    ResourceHandle<AnimationControl> ac;
    if (!_tagged) {
        RegisterFrameTick();
    } else {
        ac = GetCritter()->StartAnimation(_animActive, true, false);
        ac().SetAnimationTime(ac().GetAnimationLength());
    }
}

// FUNCTION: USA 0x00130354
// PROTOTYPE: PREVIEW 0x00518D87 identical
// NAME: proto-pdb:default.pdb
void SignTrolleyMain::FrameTick()
{
    // !FAKE: inliner; pointer locals, hand form of operator- (C0191); /O1 keeps operator- out of line, retail
    // reads Vince before the node call (match_one 68.88 with operator-)
    Vince* v1 = pManager->GetVince();
    const Vector3* v2 = &GetCritter()->GetWorldPosition();
    const Vector3* v3 = &v1->GetWorldPosition();
    Vector3 diff(v3->x - v2->x, v3->y - v2->y, v3->z - v2->z);
    if (diff.y > GetCritter()->_height * -0.5f && diff.y < GetCritter()->_height &&
        diff.x * diff.x + diff.z * diff.z < GetCritter()->_radius * 4.0f) {
        SoundManager::PlaySound("inttrolact", 0x20, _node, true, 0.0f, 0.0f);
        ActivateStop();
    }
}

// FUNCTION: USA 0x0012CDC8
// PROTOTYPE: PREVIEW 0x005157D1 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<LevelLoader::Slot,std::allocator<LevelLoader::Slot> >::size()const;

// FUNCTION: USA 0x0012DE0A
// PROTOTYPE: PREVIEW 0x0051679C identical
// NAME: proto-pdb:default.pdb
template TrolleyManager::MapStop * std::_Copy_opt<TrolleyManager::MapStop *,TrolleyManager::MapStop *>(TrolleyManager::MapStop *,TrolleyManager::MapStop *,TrolleyManager::MapStop *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0012DE2E
// PROTOTYPE: PREVIEW 0x005167C0 identical
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<TrolleyManager::MapStop,std::allocator<TrolleyManager::MapStop> >(TrolleyManager::MapStop *,TrolleyManager::MapStop *,std::allocator<TrolleyManager::MapStop> &,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0012DE8F
// PROTOTYPE: PREVIEW 0x00516821 identical
// NAME: proto-pdb:default.pdb
template TrolleyManager::MapStop * std::_Copy_backward_opt<TrolleyManager::MapStop *,TrolleyManager::MapStop *>(TrolleyManager::MapStop *,TrolleyManager::MapStop *,TrolleyManager::MapStop *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0012E165
// PROTOTYPE: PREVIEW 0x00516AF7 identical
// NAME: proto-pdb:default.pdb
template void std::fill<TrolleyManager::MapStop *,TrolleyManager::MapStop>(TrolleyManager::MapStop *,TrolleyManager::MapStop *,TrolleyManager::MapStop const &);
