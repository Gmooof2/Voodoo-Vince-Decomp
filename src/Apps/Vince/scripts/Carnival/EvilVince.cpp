// UNIT: src/Apps/Vince/scripts/Carnival/EvilVince.cpp OBJ: Xbox\OfficialRelease\EvilVince.obj OPT: /O1 /Oy /EHa

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;
    float y;
    float z;
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetScale(Vector3 v);
protected:
    void InvalidateCachedWorldPos();
public:
    char field_0[0x50];                // 0x00
    Vector3 _Scale;                    // 0x50
};

// FUNCTION: USA 0x000449DE
// PROTOTYPE: PREVIEW 0x00432815 identical
// NAME: proto-pdb:default.pdb
void SwampSceneNode::SetScale(Vector3 v)
{
    _Scale = v;
    InvalidateCachedWorldPos();
}

class FireFlyNode; // NAME: proto-pdb:default.pdb

class EvilVinceMain { // NAME: proto-pdb:default.pdb
public:
    struct EvilWhammy { // NAME: proto-pdb:default.pdb
        bool _started;                 // 0x0
        bool _hitVince;                // 0x1
        FireFlyNode *_effect;          // 0x4
    };
};

typedef unsigned int size_t;
typedef int ptrdiff_t;

inline void *__cdecl operator new(size_t, void *_Where)
{
    return (_Where);
}

namespace std { // NAME: proto-pdb:default.pdb

struct _Nonscalar_ptr_iterator_tag {}; // src proto-pdb:default.pdb _Nonscalar_ptr_iterator_tag

template<class _T1, class _T2> inline
void _Construct(_T1 *_Ptr, const _T2& _Val)
{
    new ((void *)_Ptr) _T1(_Val);
}

template<class _Ty>
class allocator {
public:
    typedef _Ty value_type;
    typedef value_type *pointer;
    typedef value_type& reference;
    typedef const value_type *const_pointer;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    template<class _Other>
    struct rebind { // src proto-pdb:default.pdb rebind
        typedef allocator<_Other> other; // src proto-pdb:default.pdb other
    };

    // FUNCTION: USA 0x00044A73
    // NAME: proto-pdb:default.pdb
    void construct(pointer _Ptr, const _Ty& _Val)
    {
        _Construct(_Ptr, _Val);
    }
};

// FUNCTION: USA 0x00044B6E
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; explicit instantiation; retail emits this template through a caller in this unit (not
// matched yet); the instantiation emits it instead
template<class _FwdIt, class _Diff, class _Tval, class _Alloc> inline
void _Uninit_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    try {
        for (; 0 < _Count; --_Count, ++_First)
            _Al.construct(_First, _Val);
    } catch (...) {
        throw;
    }
}

template void _Uninit_fill_n<Vector3 *, unsigned int, Vector3, allocator<Vector3> >(
    Vector3 *, unsigned int, const Vector3&, allocator<Vector3>&, _Nonscalar_ptr_iterator_tag);

template<class _T1, class _T2> inline
_Nonscalar_ptr_iterator_tag _Ptr_cat(_T1&, _T2&)
{
    _Nonscalar_ptr_iterator_tag _Cat;
    return (_Cat);
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x00044C27
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; explicit instantiation; retail emits this template through a caller in this unit (not
// matched yet); the instantiation emits it instead
template<class _FwdIt, class _Diff, class _Tval, class _Alloc> inline
void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al)
{
    _Uninit_fill_n(_First, _Count, _Val, _Al, _Ptr_cat(_First, _First));
}

template void _Uninitialized_fill_n<Vector3 *, unsigned int, Vector3, allocator<Vector3> >(
    Vector3 *, unsigned int, const Vector3&, allocator<Vector3>&);
#else
template<class _FwdIt, class _Diff, class _Tval, class _Alloc>
void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al);
#endif

// FUNCTION: USA 0x00044995
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; explicit instantiation; retail emits this template through a caller in this unit (not
// matched yet); the instantiation emits it instead
template<class _FwdIt, class _Ty> inline
void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}

template void fill<Vector3 *, Vector3>(Vector3 *, Vector3 *, const Vector3&);

// FUNCTION: USA 0x000449B6
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; explicit instantiation; retail emits this template through a caller in this unit (not
// matched yet); the instantiation emits it instead
template<class _BidIt1, class _BidIt2> inline
_BidIt2 _Copy_backward_opt(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Nonscalar_ptr_iterator_tag)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}

template Vector3 *_Copy_backward_opt<Vector3 *, Vector3 *>(Vector3 *, Vector3 *, Vector3 *,
    _Nonscalar_ptr_iterator_tag);

template<class _Ty, class _Alloc>
class _List_nod { // src proto-pdb:default.pdb _List_nod
protected:
    struct _Node;
    friend struct _Node;
    typedef typename _Alloc::template rebind<_Node>::other::pointer _Genptr;

    struct _Node {
        // FUNCTION: USA 0x00044974
        // NAME: proto-pdb:default.pdb
        _Node(_Genptr _Nextarg, _Genptr _Prevarg, const _Ty& _Myvalarg)
            : _Next(_Nextarg), _Prev(_Prevarg), _Myval(_Myvalarg)
        {
        }

        _Genptr _Next;
        _Genptr _Prev;
        _Ty _Myval;
    };
};

// !FAKE: c1xx front end; explicit instantiation; retail emits this member through a caller in this unit (not
// matched yet); the instantiation emits it instead
template _List_nod<EvilVinceMain::EvilWhammy, allocator<EvilVinceMain::EvilWhammy> >::_Node::_Node(
    _List_nod<EvilVinceMain::EvilWhammy, allocator<EvilVinceMain::EvilWhammy> >::_Node *,
    _List_nod<EvilVinceMain::EvilWhammy, allocator<EvilVinceMain::EvilWhammy> >::_Node *,
    const EvilVinceMain::EvilWhammy&);

template<class _Ty, class _Alloc>
class _Vector_val { // src proto-pdb:default.pdb _Vector_val
protected:
    typedef typename _Alloc::template rebind<_Ty>::other _Alty; // src proto-pdb:default.pdb _Alty

    _Alty _Alval;
};

template<class _Ty, class _Ax = allocator<_Ty> >
class vector : public _Vector_val<_Ty, _Ax> {
public:
    typedef vector<_Ty, _Ax> _Myt;
    typedef _Vector_val<_Ty, _Ax> _Mybase;
    typedef typename _Mybase::_Alty _Alloc;
    typedef typename _Alloc::size_type size_type;
    typedef typename _Alloc::difference_type _Dift;
    typedef _Dift difference_type;
    typedef typename _Alloc::pointer _Tptr;
    typedef typename _Alloc::const_pointer _Ctptr;
    typedef _Tptr pointer;
    typedef typename _Alloc::const_reference const_reference;

    class const_iterator {
    public:
        const_iterator(_Tptr _Ptr)
        {
            _Myptr = _Ptr;
        }

        const_iterator& operator+=(difference_type _Off)
        {
            _Myptr += _Off;
            return (*this);
        }

        difference_type operator-(const const_iterator& _Right) const
        {
            return (_Myptr - _Right._Myptr);
        }

        _Tptr _Myptr;
    };

    class iterator : public const_iterator {
    public:
        iterator(pointer _Ptr)
            : const_iterator(_Ptr)
        {
        }

        iterator& operator+=(difference_type _Off)
        {
            this->_Myptr += _Off;
            return (*this);
        }

        iterator operator+(difference_type _Off) const
        {
            iterator _Tmp = *this;
            return (_Tmp += _Off);
        }

        difference_type operator-(const const_iterator& _Right) const
        {
            return ((const_iterator)*this - _Right);
        }
    };

    // FUNCTION: USA 0x0004495E
    // NAME: proto-pdb:default.pdb
    size_type capacity() const
    {
        return (_Myfirst == 0 ? 0 : _Myend - _Myfirst);
    }

    iterator begin()
    {
        return (iterator(_Myfirst));
    }

    // FUNCTION: USA 0x00044948
    // NAME: proto-pdb:default.pdb
    size_type size() const
    {
        return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst);
    }

    // FUNCTION: USA 0x000453AF
    // NAME: proto-pdb:default.pdb
    iterator insert(iterator _Where, const _Ty& _Val)
    {
        size_type _Off = size() == 0 ? 0 : _Where - begin();
        _Insert_n(_Where, (size_type)1, _Val);
        return (begin() + _Off);
    }

protected:
    void _Insert_n(iterator _Where, size_type _Count, const _Ty& _Val);

    // FUNCTION: USA 0x00044C5C
    // NAME: proto-pdb:default.pdb
    pointer _Ufill(pointer _Ptr, size_type _Count, const _Ty &_Val)
    {
        _Uninitialized_fill_n(_Ptr, _Count, _Val, this->_Alval);
        return (_Ptr + _Count);
    }

public:
    pointer _Myfirst;
    pointer _Mylast;
    pointer _Myend;
};

// !FAKE: c1xx front end; explicit instantiations of single members; retail emits them through callers in this unit
// (not matched yet); the instantiations emit them instead
template vector<Vector3>::size_type vector<Vector3>::size() const;
template vector<Vector3>::size_type vector<Vector3>::capacity() const;
template vector<Vector3>::pointer vector<Vector3>::_Ufill(pointer, size_type, const Vector3&);
template vector<Vector3>::iterator vector<Vector3>::insert(iterator, const Vector3&);
template void allocator<Vector3>::construct(Vector3 *, const Vector3&);

}
