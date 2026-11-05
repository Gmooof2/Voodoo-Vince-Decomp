// UNIT: src/Apps/Vince/scripts/Carnival/EvilVince.cpp OBJ: Xbox\OfficialRelease\EvilVince.obj OPT: /O1 /Oy /EHa
#include "../../../../include/decomp.h"

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

namespace std { // NAME: proto-pdb:default.pdb
template<class _Elem> struct char_traits; // src proto-pdb:default.pdb char_traits
template<class _Ty> class allocator;
template<class _Elem, class _Traits, class _Ax> class basic_string;
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x240 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

class EvilVinceMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    struct EvilWhammy { // NAME: proto-pdb:default.pdb
        bool _started;                 // 0x0
        bool _hitVince;                // 0x1
        FireFlyNode *_effect;          // 0x4
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_HIDDEN = 1,
        STATE_APPEARING = 2,
        STATE_INIT_TAUNT = 3,
        STATE_PREATTACK = 4,
        STATE_ATTACK = 5,
        STATE_WHAMMY = 6,
        STATE_VINCEIDLE = 7,
        STATE_PAUSE = 8,
        STATE_JUMPOUT = 9,
        STATE_JUMPIN = 10
    };
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);

    char field_240[0x2C4 - 0x240];     // 0x240
};
DECOMP_SIZE_ASSERT(EvilVinceMain, 0x2C4);

typedef unsigned int size_t;
typedef int ptrdiff_t;

inline void *__cdecl operator new(size_t, void *_Where)
{
    return (_Where);
}

#ifdef NON_MATCHING
class exception { // NAME: proto-pdb:default.pdb
public:
    exception();
    exception(const char *const& _What);
    exception(const exception& _That);
    virtual ~exception();
    virtual const char *what() const;

private:
    const char *_m_what;
    int _m_doFree;
};
#endif

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

    size_type max_size() const
    {
        size_type _Count = (size_type)(-1) / sizeof (_Ty);
        return (0 < _Count ? _Count : 1);
    }

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

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string { // src proto-pdb:default.pdb basic_string
public:
    basic_string(const _Elem *_Ptr);
    int compare(const _Elem *_Ptr) const;

    _Ax _Alval;
    union _Bxty { // src proto-pdb:default.pdb _Bxty
        _Elem _Buf[16];
        _Elem *_Ptr;
    } _Bx;
    size_t _Mysize;
    size_t _Myres;
};

template<class _Elem, class _Traits, class _Alloc>
inline bool operator==(const basic_string<_Elem, _Traits, _Alloc>& _Left, const _Elem *_Right)
{
    return (_Left.compare(_Right) == 0);
}

#ifdef NON_MATCHING
// best: match_one objdiff 96.3, bytes mismatch (masked 1); wall: codegen, length_error(const string&) called out
// of line (retail inlines it: call logic_error ctor 0x0001179C, then stores ??_7length_error; C0255 residual).
// Throw-info data now pinned (4.4 T4); throw() on the dtors/what as in VC 7.1 <stdexcept> changes nothing (96.3,
// 4.4 T4.c5). ~basic_string omitted: declared, it adds an EH frame for the temp that retail lacks (objdiff 77.4)
class logic_error : public ::exception { // src proto-pdb:default.pdb logic_error
public:
    explicit logic_error(const basic_string<char>& _Message);
    virtual ~logic_error();
    virtual const char *what() const;

private:
    basic_string<char> _Str;
};

class length_error : public logic_error { // src proto-pdb:default.pdb length_error
public:
    explicit length_error(const basic_string<char>& _Message)
        : logic_error(_Message)
    {
    }

    virtual ~length_error()
    {
    }
};

template<class _Ty, class _Alloc>
class _List_ptr : public _List_nod<_Ty, _Alloc> { // src proto-pdb:default.pdb _List_ptr
};

template<class _Ty, class _Alloc>
class _List_val : public _List_ptr<_Ty, _Alloc> { // src proto-pdb:default.pdb _List_val
protected:
    typedef typename _Alloc::template rebind<_Ty>::other _Alty; // src proto-pdb:default.pdb _Alty

    _Alty _Alval;
};

template<class _Ty, class _Ax = allocator<_Ty> >
class list : public _List_val<_Ty, _Ax> { // src proto-pdb:default.pdb list
public:
    typedef _List_val<_Ty, _Ax> _Mybase;
    typedef typename _Mybase::_Alty _Alloc;
    typedef typename _Alloc::size_type size_type;
    typedef typename _Mybase::_Genptr _Nodeptr;

    size_type max_size() const
    {
        return (this->_Alval.max_size());
    }

protected:
    // FUNCTION: USA 0x0004515E
    // NAME: proto-pdb:default.pdb
    void _Incsize(size_type _Count)
    {
        if (max_size() - _Mysize < _Count)
            throw length_error("list<T> too long");
        _Mysize += _Count;
    }

    _Nodeptr _Myhead;
    size_type _Mysize;
};

// !FAKE: c1xx front end; explicit instantiation of a single member; retail emits it through callers in this unit
// (not matched yet); the instantiation emits it instead
template void list<EvilVinceMain::EvilWhammy>::_Incsize(size_type);
#endif

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

// FUNCTION: USA 0x00044ADC
// PROTOTYPE: PREVIEW 0x00432913 identical
// NAME: proto-pdb:default.pdb
void EvilVinceMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    if (_state == STATE_ATTACK && name == "trigger_whammy")
        SetState(STATE_WHAMMY);
}
