// UNIT: src/Apps/Vince/core/Critter.cpp OBJ: core\XBox\OfficialRelease\Critter.obj OPT: /O1 /Oy
// !FAKE: c2 intrinsics; file-scope pragma; retail inlines fpatan/fcos/fsin/fsqrt in Critter::GetVelocityToTarget
// under /O1 /Oy (C0181); the CRT functions declared first so the pragma precedes <math.h>'s float inlines
extern "C" double __cdecl atan(double);
extern "C" double __cdecl cos(double);
extern "C" double __cdecl sin(double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(atan, cos, sin, sqrt)
#include <math.h>
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
    unsigned int find(const basic_string& _Right, unsigned int _Off = 0) const;
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
        value_type& operator*() const { return (_Myval(this->_Ptr)); }
        value_type* operator->() const { return (&**this); }
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
    iterator lower_bound(const key_type& _Keyval) { return (iterator(_Lbound(_Keyval))); }
    iterator insert(iterator _Where, const value_type& _Val)
    {
        iterator _Next;

        if (size() == 0)
            return (_Insert(true, _Myhead, _Val));
        else if (this->_Multi) {
            if (_Where == begin()) {
                if (!this->comp(_Key(_Where._Mynode()), this->_Kfn(_Val)))
                    return (_Insert(true, _Where._Mynode(), _Val));
            } else if (_Where == end()) {
                if (!this->comp(this->_Kfn(_Val), _Key(_Rmost())))
                    return (_Insert(false, _Rmost(), _Val));
            } else if (!this->comp(_Key(_Where._Mynode()), this->_Kfn(_Val))
                       && !this->comp(this->_Kfn(_Val), _Key((--(_Next = _Where))._Mynode()))) {
                if (_Isnil(_Right(_Next._Mynode())))
                    return (_Insert(false, _Next._Mynode(), _Val));
                else
                    return (_Insert(true, _Where._Mynode(), _Val));
            } else if (!this->comp(this->_Kfn(_Val), _Key(_Where._Mynode()))
                       && (++(_Next = _Where) == end() || !this->comp(_Key(_Next._Mynode()), this->_Kfn(_Val)))) {
                if (_Isnil(_Right(_Where._Mynode())))
                    return (_Insert(false, _Where._Mynode(), _Val));
                else
                    return (_Insert(true, _Next._Mynode(), _Val));
            }
        } else {
            if (_Where == begin()) {
                if (this->comp(this->_Kfn(_Val), _Key(_Where._Mynode())))
                    return (_Insert(true, _Where._Mynode(), _Val));
            } else if (_Where == end()) {
                if (this->comp(_Key(_Rmost()), this->_Kfn(_Val)))
                    return (_Insert(false, _Rmost(), _Val));
            } else if (this->comp(this->_Kfn(_Val), _Key(_Where._Mynode()))
                       && this->comp(_Key((--(_Next = _Where))._Mynode()), this->_Kfn(_Val))) {
                if (_Isnil(_Right(_Next._Mynode())))
                    return (_Insert(false, _Next._Mynode(), _Val));
                else
                    return (_Insert(true, _Where._Mynode(), _Val));
            } else if (this->comp(_Key(_Where._Mynode()), this->_Kfn(_Val))
                       && (++(_Next = _Where) == end() || this->comp(this->_Kfn(_Val), _Key(_Next._Mynode())))) {
                if (_Isnil(_Right(_Where._Mynode())))
                    return (_Insert(false, _Where._Mynode(), _Val));
                else
                    return (_Insert(true, _Next._Mynode(), _Val));
            }
        }

        return (insert(_Val).first);
    }

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

    iterator erase(iterator _Where);

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

    void _Init();

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
public:
    typedef _Tree<_Tmap_traits<K, T, Pr, A, false> > _Mybase;
    typedef K key_type;
    typedef T mapped_type;
    typedef typename _Mybase::value_type value_type;
    typedef typename _Mybase::iterator iterator;

    mapped_type& operator[](const key_type& _Keyval)
    {
        iterator _Where = this->lower_bound(_Keyval);
        if (_Where == this->end() || this->comp(_Keyval, this->_Key(_Where._Mynode())))
            _Where = this->insert(_Where, value_type(_Keyval, mapped_type()));
        return ((*_Where).second);
    }
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

    unsigned int _RefCount;            // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    Ptr() : _Object(0) {}
    Ptr(const Ptr<T>& p) : _Object(p._Object)
    {
        if (_Object)
            _Object->AddRef();
    }
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    Ptr<T>& operator=(const Ptr<T>& p);

    T* _Object;                        // 0x00
};

class CollisionResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
};

// FUNCTION: USA 0x000ED080
// PROTOTYPE: PREVIEW 0x004D8383 identical
// NAME: proto-pdb:default.pdb
class SwampCollide { // NAME: proto-pdb:default.pdb
public:
    static Ptr<CollisionResultObject> GetNewResult();
};

class CritterCollision { // NAME: proto-pdb:default.pdb
private:
    CritterCollision();

public:
    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};
DECOMP_SIZE_ASSERT(CritterCollision, 0x8);

// FUNCTION: USA 0x000ED1B3
// PROTOTYPE: PREVIEW 0x004D8474 identical
// NAME: proto-pdb:default.pdb
CritterCollision::CritterCollision()
{
    _wall = SwampCollide::GetNewResult();
    _floor = SwampCollide::GetNewResult();
}

struct AABox;                          // NAME: proto-pdb:default.pdb
struct Quaternion;                     // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    const SwampString& GetName() const { return _Name; }
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1
    };
    unsigned int GetZone() const;
    bool GetRenderingOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    bool IsValid() const { return !GetRenderingOption(Invalid); }

    int _Type;                         // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x88 - 0x24];        // 0x24
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0x200 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    void CheckForRegionCrossings(SwampSceneNode* node, const Vector3& start, const Vector3& end, bool testEndOnly) const;
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    RegionManager& GetRegionManager() const { return *_regionManager; }

    char field_0[0x30];                // 0x00
    RegionManager* _regionManager;     // 0x30
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Stun(float);                                      // vtable+0x3C
    CritterCollision MoveMyFatAss(float, unsigned int);
    void CheckRegions();
    Vector3 GetVelocityToTarget(const Vector3& start, const Vector3& target, float gravity, float lobfactor);

    float _MaxSpeed;                   // 0x200
    float _CurSpeed;                   // 0x204
    float _stunTimer;                  // 0x208
};

// FUNCTION: USA 0x000ECE3A
// PROTOTYPE: PREVIEW 0x004D8134 identical
// NAME: proto-pdb:default.pdb
void Critter::Stun(float time)
{
    _stunTimer = time;
}

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb
Vector3 Normalize(const Vector3&);     // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000ECF71
// PROTOTYPE: PREVIEW 0x004D8274 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 92.15, bytes mismatch; retail passes delta's slot as Normalize's return temp and reads it
// through eax (frame 0x18, ours 0x24), and squares flat x before z; the operator-, operator* and const-ref forms
// stay out of line or add a temp (C0125, C0219)
Vector3 Critter::GetVelocityToTarget(const Vector3& start, const Vector3& target, float gravity, float lobfactor)
{
    Vector3 delta;
    delta.x = target.x - start.x;
    delta.y = target.y - start.y;
    delta.z = target.z - start.z;
    Vector3 flat_delta(delta.x, 0.0f, delta.z);
    float lengthSq = flat_delta.x * flat_delta.x + flat_delta.z * flat_delta.z;
    float distance = lengthSq * FastReciprocalSquareRoot(lengthSq);
    float angle = distance != 0.0f ? (float)atan(delta.y / distance) : 1.5707964f;
    angle += (1.5707963267948966 - angle) * lobfactor;
    float cos_angle = (float)cos(angle);
    float sin_angle = (float)sin(angle);
    float velocity = distance * distance * gravity * -0.5f / ((cos_angle * delta.y - sin_angle * distance) * cos_angle);
    if (velocity < 0.0f)
        return delta;
    velocity = (float)sqrt(velocity);
    Vector3 dir = Normalize(flat_delta);
    return Vector3(cos_angle * dir.x * velocity, velocity * sin_angle, cos_angle * dir.z * velocity);
}
#endif

// FUNCTION: USA 0x000ED09F
// PROTOTYPE: PREVIEW 0x004D83A2 identical
// NAME: proto-pdb:default.pdb
void Critter::CheckRegions()
{
    Vector3 pos = GetWorldPosition();
    g_swamp->GetRegionManager().CheckForRegionCrossings(this, pos, pos, true);
}

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    double GetFloatRecord(const SwampString& id, double def) const;
};

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

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

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)_node; }
    CritterCollision MoveMe(float gravity);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

    char field_4C[0x5C - 0x4C];        // 0x4C
    float _wanderPatrolRadius;         // 0x5C
    float _wanderLeashRadius;          // 0x60
    char field_64[0xAC - 0x64];        // 0x64
    float _wanderLeashRadiusOverride;  // 0xAC
    float _wanderPatrolRadiusOverride; // 0xB0
    char field_B4[0xBC - 0xB4];        // 0xB4
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

// FUNCTION: USA 0x000EE88E
// PROTOTYPE: PREVIEW 0x004D9AF7 identical
// NAME: proto-pdb:default.pdb
CritterCollision CritterMain::MoveMe(float gravity)
{
    return GetCritter()->MoveMyFatAss(gravity, 0);
}

// FUNCTION: USA 0x000EEF82
// PROTOTYPE: PREVIEW 0x004DA1EB identical
// NAME: proto-pdb:default.pdb
void CritterMain::Load(const BeepTDF& tdf)
{
    _wanderLeashRadiusOverride = tdf.GetFloatRecord("Leash", 0.0);
    if (_wanderLeashRadiusOverride != 0.0f)
        _wanderLeashRadius = _wanderLeashRadiusOverride;
    _wanderPatrolRadiusOverride = tdf.GetFloatRecord("PatrolRadius", 0.0);
    if (_wanderPatrolRadiusOverride != 0.0f)
        _wanderPatrolRadius = _wanderPatrolRadiusOverride;
    SwampScript::Load(tdf);
}

// FUNCTION: USA 0x000EF03A
// PROTOTYPE: PREVIEW 0x004DA2A3 identical
// NAME: proto-pdb:default.pdb
SwampString CritterMain::Save()
{
    SwampString s;
    if (_wanderPatrolRadiusOverride > 0.0f)
        s += format("\t\tPatrolRadius = %g;\n", _wanderPatrolRadiusOverride);
    if (_wanderLeashRadiusOverride > 0.0f)
        s += format("\t\tLeash = %g;\n", _wanderLeashRadiusOverride);
    s += SwampScript::Save();
    return s;
}

inline void* operator new(unsigned int, void* p) { return p; }

// !FAKE: c1xx front end; scaffold call; retail emits the implicit CritterCollision copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_CritterCollision_copy(void* p, const CritterCollision& v)
{
    new (p) CritterCollision(v);
}

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString MakeLower(const SwampString&);
}

struct DamageInfo { // NAME: proto-pdb:default.pdb
    int damage;                        // 0x00
    float knockBackMin;                // 0x04
    float knockBackMax;                // 0x08
    int beadsSmall;                    // 0x0C
    int beadsMed;                      // 0x10
    int beadsLarge;                    // 0x14
    int motes;                         // 0x18
};
DECOMP_SIZE_ASSERT(DamageInfo, 0x1C);

class Vince; // NAME: proto-pdb:default.pdb

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    typedef std::map<SwampString, Critter*> NameMap; // NAME: proto-pdb:default.pdb

    CritterManager();
    ~CritterManager();
    void Remove(const SwampString& s);
    void RemoveMatching(const SwampString& s);
    void DestroyAll();
    int GetStartHealth(const SwampString& critterType);
    void InvalidateMatching(const char* s, unsigned int zone);
    DamageInfo GetDamageInfo(const SwampString& critterType, const SwampString& attackType);

    NameMap _Critters;                 // 0x00
    bool _disableScripts;              // 0x0C
    std::map<SwampString, int> _startHealth; // 0x10
    std::map<SwampString, std::map<SwampString, DamageInfo> > _damageTable; // 0x1C
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000EF120
// PROTOTYPE: PREVIEW 0x004DA389 identical
// NAME: proto-pdb:default.pdb
void CritterManager::InvalidateMatching(const char* s, unsigned int zone)
{
    SwampString lookup = BeepLib::MakeLower(s);
    NameMap::iterator next;
    // !FAKE: c2 register allocation; neutral stand-in local; the PDB records only next, retail walks the current node in esi
    for (NameMap::iterator v0 = _Critters.begin(); v0 != _Critters.end(); v0 = next) {
        next = v0;
        ++next;
        // !FAKE: c2 register allocation; neutral stand-in local; the PDB records no local for the critter (esi)
        Critter* v1 = (*v0).second;
        if (v1->IsValid() && v1->GetName().find(lookup) == 0) {
            if (zone == 0 || v1->GetZone() == zone)
                v1->Invalidate();
        }
    }
}

// FUNCTION: USA 0x000EF3C2
// PROTOTYPE: PREVIEW 0x004DA62B identical
// NAME: proto-pdb:default.pdb
int CritterManager::GetStartHealth(const SwampString& critterType)
{
    std::map<SwampString, int>::iterator c = _startHealth.find(BeepLib::MakeLower(critterType));
    if (_startHealth.end() != c)
        return (*c).second;
    return 1;
}

// FUNCTION: USA 0x000EF407
// PROTOTYPE: PREVIEW 0x004DA670 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.96, bytes mismatch (a in the attackType slot [ebp+0x10], retail reuses c's [ebp+0xc]; 20 forms: blocks, stand-ins, branch order)
DamageInfo CritterManager::GetDamageInfo(const SwampString& critterType, const SwampString& attackType)
{
    // !FAKE: c2 register allocation; neutral stand-in local; the PDB records only c and a, retail keeps the node in esi
    std::map<SwampString, std::map<SwampString, DamageInfo> >::iterator v0;
    {
        std::map<SwampString, std::map<SwampString, DamageInfo> >::iterator c = _damageTable.find(BeepLib::MakeLower(critterType));
        v0 = c;
    }
    {
        std::map<SwampString, DamageInfo>::iterator a = (*v0).second.find(BeepLib::MakeLower(attackType));
        if (a == (*v0).second.end()) {
            DamageInfo di;
            di.damage = -1;
            di.knockBackMax = 0.0f;
            di.knockBackMin = 0.0f;
            return di;
        }
        return (*a).second;
    }
}
#endif

// FUNCTION: USA 0x000F05E4
// PROTOTYPE: PREVIEW 0x004DB84D identical
// NAME: proto-pdb:default.pdb
void CritterManager::RemoveMatching(const SwampString& s)
{
    SwampString lookup = BeepLib::MakeLower(s);
    NameMap::iterator next;
    // !FAKE: c2 register allocation; neutral stand-in local; the PDB records only next, retail walks the current node in esi
    for (NameMap::iterator v0 = _Critters.begin(); v0 != _Critters.end(); v0 = next) {
        next = v0;
        ++next;
        // !FAKE: c2 register allocation; neutral stand-in local; the PDB records no local for the erased critter (edi)
        Critter* v1 = (*v0).second;
        if (v1->GetName().find(lookup) == 0) {
            _Critters.erase(v0);
            v1->Invalidate();
        }
    }
}

// FUNCTION: USA 0x000F0654
// PROTOTYPE: PREVIEW 0x004DB8BD identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.74, bytes mismatch (frame 0x20 vs 0x1C: retail's erase result shares the [ebp+8] slot of it; declared-then-assigned it gives 98.42, the CameraManager::Remove wall)
void CritterManager::Remove(const SwampString& s)
{
    SwampString lookup = BeepLib::MakeLower(s);
    NameMap::iterator it = _Critters.find(lookup);
    if (it != _Critters.end()) {
        // !FAKE: c2 register allocation; neutral stand-in local; the PDB records no local for the erased critter (esi)
        Critter* p = it->second;
        _Critters.erase(it);
        p->Invalidate();
    }
}
#endif

// FUNCTION: USA 0x000F2546
// PROTOTYPE: PREVIEW 0x004DD7AF identical
// NAME: proto-pdb:default.pdb
void CritterManager::DestroyAll()
{
    // !FAKE: c2 register allocation; neutral stand-in local; the PDB records no local, retail keeps begin() in eax for the test and the use
    for (NameMap::iterator v0 = _Critters.begin(); _Critters.end() != v0; v0 = _Critters.begin()) {
        // !FAKE: c2 register allocation; neutral stand-in local; the PDB records no local for the erased critter (edi)
        Critter* v1 = (*v0).second;
        _Critters.erase(v0);
        if (v1)
            v1->Invalidate();
    }
    _startHealth.clear();
    _damageTable.clear();
    _vinceCache = 0;
    _disableScripts = false;
}

// FUNCTION: USA 0x000F2943
// PROTOTYPE: PREVIEW 0x004DDB9E identical
// NAME: proto-pdb:default.pdb
CritterManager::~CritterManager()
{
    pManager = 0;
}

// FUNCTION: USA 0x000F2965
// PROTOTYPE: PREVIEW 0x004DDBC0 identical
// NAME: proto-pdb:default.pdb
CritterManager::CritterManager()
{
    _vinceCache = 0;
    pManager = this;
    _disableScripts = false;
}

namespace std {
// !FAKE: c1xx front end; decl-only explicit specialization; retail calls this pair ctor out of line (0x000F1441) and it
// copies the map through the unnamed map copy ctor 0x000F13B5; ours copied bitwise (precheck .text 32 bytes at 0x000F1441)
template <> pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > >::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > >(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > const &);
}

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > >,0> >::iterator _Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > >,0> >::_Insert(bool,std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > >,0> >::_Node *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > const &);
template <> std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> >::iterator _Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> >::_Insert(bool,std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> >::_Node *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> const &);
template <> std::pair<std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > >,0> >::iterator,bool> _Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > >,0> >::insert(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > const &);
template <> std::pair<std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> >::iterator,bool> _Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> >::insert(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> const &);
}
#endif

class FontManager { // NAME: proto-pdb:default.pdb
public:
    struct LangInfo; // NAME: proto-pdb:default.pdb
};

// FUNCTION: USA 0x000ECEA5
// PROTOTYPE: PREVIEW 0x004D819F identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; relink red: fold VA, other units' externals to their _Inc aliases (CameraManager GameCam*) lie in a replaced range
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::const_iterator::_Inc();
#endif

// FUNCTION: USA 0x000ECEDB
// PROTOTYPE: PREVIEW 0x004D81D5 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,FontManager::LangInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,FontManager::LangInfo> > > > >,0> >::const_iterator::_Inc();

// FUNCTION: USA 0x000ECF11
// PROTOTYPE: PREVIEW 0x004074A3 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >,0> >::const_iterator::_Inc();

// FUNCTION: USA 0x000EEA6F
// PROTOTYPE: PREVIEW 0x004D9CD8 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,int>::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,int>(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,int const &);

// FUNCTION: USA 0x000EEA8A
// PROTOTYPE: PREVIEW 0x004D9CF3 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo>::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo>(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,DamageInfo const &);

// FUNCTION: USA 0x000EEAAC
// PROTOTYPE: PREVIEW 0x004D9D15 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit pair<const SwampString, DamageInfo> copy ctor in
// this unit (no matched function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_pair_DamageInfo_copy(void* p, const std::pair<const SwampString, DamageInfo>& v)
{
    new (p) std::pair<const SwampString, DamageInfo>(v);
}

// FUNCTION: USA 0x000F0DD6
// PROTOTYPE: PREVIEW 0x004DC03F identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> >::_Copy(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> > const &);

// FUNCTION: USA 0x000F118E
// PROTOTYPE: PREVIEW 0x004DC3F7 identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> >::iterator std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> >::insert(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> >,0> >::iterator,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> const &);

// FUNCTION: USA 0x000F134B
// PROTOTYPE: PREVIEW 0x004DC5B4 identical
// NAME: proto-pdb:default.pdb
template DamageInfo & std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >::operator[](std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &);

// FUNCTION: USA 0x000F23C2
// PROTOTYPE: PREVIEW 0x004DD62B identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > >,0> >::iterator std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > >,0> >::insert(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > >,0> >::iterator,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > const &);

// FUNCTION: USA 0x000F2595
// PROTOTYPE: PREVIEW 0x004DD7FE identical
// NAME: proto-pdb:default.pdb
template std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > & std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,std::map<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,DamageInfo,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,DamageInfo> > > > > >::operator[](std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &);
