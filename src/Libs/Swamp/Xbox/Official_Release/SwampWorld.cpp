// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampWorld.cpp OBJ: Xbox\Official_Release\SwampWorld.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct FloodChunk; // NAME: proto-pdb:default.pdb

namespace std { // NAME: proto-pdb:default.pdb
template<class _Elem> struct char_traits;
template <class T> class allocator {}; // empty, as VC 7.1
// Opaque basic_string (C0090): VC 7.1 layout, _Tidy protected as retail's IAE 0x000111D6 (C0241)
template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    basic_string(const _Elem* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);

    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    enum { _BUF_SIZE = 16 / sizeof(_Elem) < 1 ? 1 : 16 / sizeof(_Elem) };
    _Ax _Alval;                        // 0x00
    union {
        _Elem _Buf[_BUF_SIZE];
        _Elem* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
typedef basic_string<char> string;
template<class _Elem, class _Traits, class _Alloc>
basic_string<_Elem, _Traits, _Alloc> operator+(const basic_string<_Elem, _Traits, _Alloc>& _Left, const _Elem* _Right);

struct input_iterator_tag
	{	// identifying tag for input iterators
	};
struct forward_iterator_tag
	: public input_iterator_tag
	{	// identifying tag for forward iterators
	};
struct bidirectional_iterator_tag
	: public forward_iterator_tag
	{	// identifying tag for bidirectional iterators
	};
struct random_access_iterator_tag
	: public bidirectional_iterator_tag
	{	// identifying tag for random-access iterators
	};
template<class _Category,
	class _Ty,
	class _Diff,
	class _Pointer,
	class _Reference>
		struct iterator
	{	// base type for all iterator classes
	typedef _Category iterator_category;
	typedef _Ty value_type;
	typedef _Diff difference_type;
	typedef _Diff distance_type;	// retained
	typedef _Pointer pointer;
	typedef _Reference reference;
	};
template<class _Ty,
	class _Diff,
	class _Pointer,
	class _Reference>
	struct _Ranit
		: public iterator<random_access_iterator_tag, _Ty, _Diff,
			_Pointer, _Reference>
	{	// base for random-access iterators
	};
template<class _Iter>
	struct iterator_traits
	{	// get traits from iterator _Iter
	typedef typename _Iter::iterator_category iterator_category;
	typedef typename _Iter::value_type value_type;
	typedef typename _Iter::difference_type difference_type;
	typedef difference_type distance_type;	// retained
	typedef typename _Iter::pointer pointer;
	typedef typename _Iter::reference reference;
	};
template<class _Iter> inline
	typename iterator_traits<_Iter>::value_type *_Val_type(_Iter)
	{	// return value type from arbitrary argument
	return (0);
	}
template<class _Iter> inline
	typename iterator_traits<_Iter>::difference_type
		*_Dist_type(_Iter)
	{	// return distance type from arbitrary argument
	return (0);
	}
template<class _Ty> inline
	void swap(_Ty& _Left, _Ty& _Right)
	{	// exchange values stored at _Left and _Right
	_Ty _Tmp = _Left;
	_Left = _Right, _Right = _Tmp;
	}
template<class _FwdIt1,
	class _FwdIt2> inline
	void iter_swap(_FwdIt1 _Left, _FwdIt2 _Right)
	{	// swap *_Left and *_Right
	std::swap(*_Left, *_Right);
	}

template <class T, class A = allocator<T> > class vector {
public:
	typedef int _Dift;
	typedef T *_Tptr;
	typedef const T *_Ctptr;
	typedef T& _Reft;
	typedef const T& const_reference;
	class const_iterator // NAME: proto-pdb:default.pdb
		: public _Ranit<T, _Dift, _Ctptr, const_reference>
		{	// iterator for nonmutable vector
	public:
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef _Dift difference_type;
		typedef _Ctptr pointer;
		typedef const_reference reference;

		const_iterator(_Tptr _Ptr)
			{	// construct with pointer _Ptr
			_Myptr = _Ptr;
			}

		const_reference operator*() const
			{	// return designated object
			return (*_Myptr);
			}

		const_iterator& operator++()
			{	// preincrement
			++_Myptr;
			return (*this);
			}

		const_iterator& operator+=(difference_type _Off)
			{	// increment by integer
			_Myptr += _Off;
			return (*this);
			}

		difference_type operator-(const const_iterator& _Right) const
			{	// return difference of iterators
			return (_Myptr - _Right._Myptr);
			}

		bool operator==(const const_iterator& _Right) const
			{	// test for iterator equality
			return (_Myptr == _Right._Myptr);
			}

		bool operator!=(const const_iterator& _Right) const
			{	// test for iterator inequality
			return (!(*this == _Right));
			}

		bool operator<(const const_iterator& _Right) const
			{	// test if this < _Right
			return (_Myptr < _Right._Myptr);
			}

		_Tptr _Myptr;	// offset of element in vector
		};
	class iterator // NAME: proto-pdb:default.pdb
		: public const_iterator
		{	// iterator for mutable vector
	public:
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef _Dift difference_type;
		typedef _Tptr pointer;
		typedef _Reft reference;

		iterator(pointer _Ptr)
			: const_iterator(_Ptr)
			{	// construct with pointer _Ptr
			}

		reference operator*() const
			{	// return designated object
			return ((reference)**(const_iterator *)this);
			}

		iterator& operator++()
			{	// preincrement
			++this->_Myptr;
			return (*this);
			}

		iterator operator++(int)
			{	// postincrement
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
			}

		iterator& operator--()
			{	// predecrement
			--this->_Myptr;
			return (*this);
			}

		iterator& operator+=(difference_type _Off)
			{	// increment by integer
			this->_Myptr += _Off;
			return (*this);
			}

		iterator operator+(difference_type _Off) const
			{	// return this + integer
			iterator _Tmp = *this;
			return (_Tmp += _Off);
			}

		iterator& operator-=(difference_type _Off)
			{	// decrement by integer
			return (*this += -_Off);
			}

		iterator operator-(difference_type _Off) const
			{	// return this - integer
			iterator _Tmp = *this;
			return (_Tmp -= _Off);
			}

		difference_type operator-(const const_iterator& _Right) const
			{	// return difference of iterators
			return ((const_iterator)*this - _Right);
			}
		};
	vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
	~vector() { _Tidy(); }
	const_reference operator[](unsigned int _Pos) const { return (*(_Myfirst + _Pos)); }
	_Reft operator[](unsigned int _Pos) { return (*(begin() + _Pos)); }
	iterator begin() { return iterator(_Myfirst); }
	const_iterator begin() const { return const_iterator(_Myfirst); }
	iterator end() { return iterator(_Mylast); }
	const_iterator end() const { return const_iterator(_Mylast); }
	unsigned int capacity() const { return (_Myfirst == 0 ? 0 : _Myend - _Myfirst); }
	unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
	void push_back(const T& _Val);

protected:
	void _Tidy();

public:
	A _Alval;                          // 0x00
	T* _Myfirst;                      // 0x04
	T* _Mylast;                        // 0x08
	T* _Myend;                         // 0x0C
};

template <class T, class A = allocator<T> > class list {
public:
	struct _Node { // NAME: proto-pdb:default.pdb
		_Node* _Next;                  // 0x00
		_Node* _Prev;                  // 0x04
		T _Myval;                      // 0x08
	};
	typedef _Node* _Nodeptr;
	class const_iterator { // NAME: proto-pdb:default.pdb
	public:
		const_iterator(_Nodeptr _Pnode) : _Ptr(_Pnode) {}
		const T& operator*() const { return _Ptr->_Myval; }
		const_iterator& operator++() { _Ptr = _Ptr->_Next; return (*this); }
		const_iterator& operator--() { _Ptr = _Prevnode(_Ptr); return (*this); }
		bool operator==(const const_iterator& _Right) const { return (_Ptr == _Right._Ptr); }
		bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
		_Nodeptr _Ptr;
	};
	class iterator : public const_iterator { // NAME: proto-pdb:default.pdb
	public:
		iterator(_Nodeptr _Pnode) : const_iterator(_Pnode) {}
		T& operator*() const { return ((T&)**(const_iterator *)this); }
		iterator& operator--() { --(*(const_iterator *)this); return (*this); }
	};
	static _Nodeptr& _Nextnode(_Nodeptr _Pnode) { return (_Pnode->_Next); }
	static _Nodeptr& _Prevnode(_Nodeptr _Pnode) { return (_Pnode->_Prev); }
	iterator begin() { return (iterator(_Nextnode(_Myhead))); }
	const_iterator begin() const { return (const_iterator(_Nextnode(_Myhead))); }
	iterator end() { return (iterator(_Myhead)); }
	const_iterator end() const { return (const_iterator(_Myhead)); }
	unsigned int size() const { return (_Mysize); }
	T& back() { return (*(--end())); }
	void push_back(const T& _Val) { _Insert(end(), _Val); }
	iterator erase(iterator _First, iterator _Last);
	void _Insert(iterator _Where, const T& _Val);
	void clear()
		{	// erase all
		_Nodeptr _Pnext;
		_Nodeptr _Pnode = _Nextnode(_Myhead);
		_Nextnode(_Myhead) = _Myhead;
		_Prevnode(_Myhead) = _Myhead;
		_Mysize = 0;

		for (; _Pnode != _Myhead; _Pnode = _Pnext)
			{	// delete an element
			_Pnext = _Nextnode(_Pnode);
			operator delete(_Pnode);
			}
		}

	A _Alval;                          // 0x00
	_Nodeptr _Myhead;                  // 0x04
	unsigned int _Mysize;              // 0x08
};

template<class _Ty1, class _Ty2> struct pair { // VC 7.1 <utility>
	pair(const _Ty1& _Val1, const _Ty2& _Val2) : first(_Val1), second(_Val2) {}
	template<class _Other1, class _Other2>
		pair(const pair<_Other1, _Other2>& _Right)
		: first(_Right.first), second(_Right.second)
		{	// construct from compatible pair
		}

	_Ty1 first;
	_Ty2 second;
};
template<class _Ty1, class _Ty2> inline
	pair<_Ty1, _Ty2> make_pair(_Ty1 _Val1, _Ty2 _Val2)
	{	// return pair composed from arguments
	return (pair<_Ty1, _Ty2>(_Val1, _Val2));
	}

// Opaque map (VC 7.1 <xtree>/<map> names; insert declaration only, retail calls it out of line)
template <class T> struct less {};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
	typedef K key_type;
	typedef pair<const K, T> value_type;
	Pr comp;                           // 0x00
};
template <class Traits> class _Tree : public Traits {
public:
	typedef typename Traits::value_type value_type;
	struct _Node { // NAME: proto-pdb:default.pdb
		_Node* _Left;                  // 0x00
		_Node* _Parent;                // 0x04
		_Node* _Right;                 // 0x08
		value_type _Myval;             // 0x0C
		char _Color;
		char _Isnil;
	};
	typedef typename Traits::key_type key_type;
	class const_iterator { // NAME: proto-pdb:default.pdb
	public:
		const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
		const_iterator& operator++() { _Inc(); return (*this); }
		bool operator==(const const_iterator& _Right) const { return (_Ptr == _Right._Ptr); }
		bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
		void _Inc();
		_Node* _Ptr;
	};
	class iterator : public const_iterator { // NAME: proto-pdb:default.pdb
	public:
		iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
		value_type* operator->() const { return (&this->_Ptr->_Myval); }
		iterator& operator++() { ++(*(const_iterator *)this); return (*this); }
	};
	typedef pair<iterator, bool> _Pairib;
	_Tree() { _Init(); }
	~_Tree() { _Tidy(); }
	iterator begin() { return (iterator(_Myhead->_Left)); }
	iterator end() { return (iterator(_Myhead)); }
	_Pairib insert(const value_type& _Val);
	iterator find(const key_type& _Keyval);

protected:
	void _Init();
	void _Tidy();

public:

	_Node* _Myhead;                    // 0x04
	unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > >
class map : public _Tree<_Tmap_traits<K, T, Pr, A, false> > { // NAME: proto-pdb:default.pdb
};

// !FAKE: inliner; decl-only callee; retail calls rotate (0x000B3FE3) out of line, not a row here
template<class _FwdIt> void rotate(_FwdIt _First, _FwdIt _Mid, _FwdIt _Last);
const int _ISORT_MAX = 32;	// maximum size for insertion sort

		// TEMPLATE FUNCTION push_heap WITH PRED
template<class _RanIt,
	class _Diff,
	class _Ty,
	class _Pr> inline
	void _Push_heap(_RanIt _First, _Diff _Hole,
		_Diff _Top, _Ty _Val, _Pr _Pred)
	{	// percolate _Hole to _Top or where _Val belongs, using operator<
	for (_Diff _Idx = (_Hole - 1) / 2;
		_Top < _Hole && _Pred(*(_First + _Idx), _Val);
		_Idx = (_Hole - 1) / 2)
		{	// move _Hole up to parent
		*(_First + _Hole) = *(_First + _Idx);
		_Hole = _Idx;
		}

	*(_First + _Hole) = _Val;	// drop _Val into final hole
	}
		// TEMPLATE FUNCTION pop_heap WITH PRED
template<class _RanIt,
	class _Diff,
	class _Ty,
	class _Pr> inline
	void _Adjust_heap(_RanIt _First, _Diff _Hole, _Diff _Bottom,
		_Ty _Val, _Pr _Pred)
	{	// percolate _Hole to _Bottom, then push _Val, using _Pred
	_Diff _Top = _Hole;
	_Diff _Idx = 2 * _Hole + 2;

	for (; _Idx < _Bottom; _Idx = 2 * _Idx + 2)
		{	// move _Hole down to larger child
		if (_Pred(*(_First + _Idx), *(_First + (_Idx - 1))))
			--_Idx;
		*(_First + _Hole) = *(_First + _Idx), _Hole = _Idx;
		}

	if (_Idx == _Bottom)
		{	// only child at bottom, move _Hole down to it
		*(_First + _Hole) = *(_First + (_Bottom - 1));
		_Hole = _Bottom - 1;
		}
	_Push_heap(_First, _Hole, _Top, _Val, _Pred);
	}
template<class _RanIt,
	class _Diff,
	class _Ty,
	class _Pr> inline
	void _Pop_heap(_RanIt _First, _RanIt _Last, _RanIt _Dest,
		_Ty _Val, _Pr _Pred, _Diff *)
	{	// pop *_First to *_Dest and reheap, using _Pred
	*_Dest = *_First;
	_Adjust_heap(_First, _Diff(0), _Diff(_Last - _First), _Val, _Pred);
	}
template<class _RanIt,
	class _Ty,
	class _Pr> inline
	void _Pop_heap_0(_RanIt _First, _RanIt _Last, _Pr _Pred, _Ty *)
	{	// pop *_First to *(_Last - 1) and reheap, using _Pred
	_Pop_heap(_First, _Last - 1, _Last - 1,
		_Ty(*(_Last - 1)), _Pred, _Dist_type(_First));
	}
template<class _RanIt,
	class _Pr> inline
	void pop_heap(_RanIt _First, _RanIt _Last, _Pr _Pred)
	{	// pop *_First to *(_Last - 1) and reheap, using _Pred
	if (1 < _Last - _First)
		_Pop_heap_0(_First, _Last, _Pred, _Val_type(_First));
	}
		// TEMPLATE FUNCTION make_heap WITH PRED
template<class _RanIt,
	class _Diff,
	class _Ty,
	class _Pr> inline
	void _Make_heap(_RanIt _First, _RanIt _Last, _Pr _Pred, _Diff *, _Ty *)
	{	// make nontrivial [_First, _Last) into a heap, using _Pred
	_Diff _Bottom = _Last - _First;
	for (_Diff _Hole = _Bottom / 2; 0 < _Hole; )
		{	// reheap top half, bottom to top
		--_Hole;
		_Adjust_heap(_First, _Hole, _Bottom,
			_Ty(*(_First + _Hole)), _Pred);
		}
	}
template<class _RanIt,
	class _Pr> inline
	void make_heap(_RanIt _First, _RanIt _Last, _Pr _Pred)
	{	// make [_First, _Last) into a heap, using _Pred
	if (1 < _Last - _First)
		_Make_heap(_First, _Last, _Pred,
			_Dist_type(_First), _Val_type(_First));
	}
		// TEMPLATE FUNCTION sort_heap WITH PRED
template<class _RanIt,
	class _Pr> inline
	void sort_heap(_RanIt _First, _RanIt _Last, _Pr _Pred)
	{	// order heap by repeatedly popping, using _Pred
	for (; 1 < _Last - _First; --_Last)
		std::pop_heap(_First, _Last, _Pred);
	}
		// TEMPLATE FUNCTION sort WITH PRED
template<class _BidIt,
	class _Pr> inline
	void _Insertion_sort(_BidIt _First, _BidIt _Last, _Pr _Pred)
	{	// insertion sort [_First, _Last), using _Pred
	if (_First != _Last)
		for (_BidIt _Next = _First; ++_Next != _Last; )
			if (_Pred(*_Next, *_First))
				{	// found new earliest element, rotate to front
				_BidIt _Next1 = _Next;
				std::rotate(_First, _Next, ++_Next1);
				}
			else
				{	// look for insertion point after first
				_BidIt _Dest = _Next;
				for (_BidIt _Dest0 = _Dest; _Pred(*_Next, *--_Dest0); )
					_Dest = _Dest0;
				if (_Dest != _Next)
					{	// rotate into place
					_BidIt _Next1 = _Next;
					std::rotate(_Dest, _Next, ++_Next1);
					}
				}
	}
template<class _RanIt,
	class _Pr> inline
	void _Med3(_RanIt _First, _RanIt _Mid, _RanIt _Last, _Pr _Pred)
	{	// sort median of three elements to middle
	if (_Pred(*_Mid, *_First))
		std::iter_swap(_Mid, _First);
	if (_Pred(*_Last, *_Mid))
		std::iter_swap(_Last, _Mid);
	if (_Pred(*_Mid, *_First))
		std::iter_swap(_Mid, _First);
	}
template<class _RanIt,
	class _Pr> inline
	void _Median(_RanIt _First, _RanIt _Mid, _RanIt _Last, _Pr _Pred)
	{	// sort median element to middle
	if (40 < _Last - _First)
		{	// median of nine
		int _Step = (_Last - _First + 1) / 8;
		_Med3(_First, _First + _Step, _First + 2 * _Step, _Pred);
		_Med3(_Mid - _Step, _Mid, _Mid + _Step, _Pred);
		_Med3(_Last - 2 * _Step, _Last - _Step, _Last, _Pred);
		_Med3(_First + _Step, _Mid, _Last - _Step, _Pred);
		}
	else
		_Med3(_First, _Mid, _Last, _Pred);
	}
template<class _RanIt,
	class _Pr> inline
	pair<_RanIt, _RanIt> _Unguarded_partition(_RanIt _First, _RanIt _Last,
		_Pr _Pred)
	{	// partition [_First, _Last), using _Pred
	_RanIt _Mid = _First + (_Last - _First) / 2;
	_Median(_First, _Mid, _Last - 1, _Pred);
	_RanIt _Pfirst = _Mid;
	_RanIt _Plast = _Pfirst + 1;

	while (_First < _Pfirst
		&& !_Pred(*(_Pfirst - 1), *_Pfirst)
		&& !_Pred(*_Pfirst, *(_Pfirst - 1)))
		--_Pfirst;
	while (_Plast < _Last
		&& !_Pred(*_Plast, *_Pfirst)
		&& !_Pred(*_Pfirst, *_Plast))
		++_Plast;

	_RanIt _Gfirst = _Plast;
	_RanIt _Glast = _Pfirst;

	for (; ; )
		{	// partition
		for (; _Gfirst < _Last; ++_Gfirst)
			if (_Pred(*_Pfirst, *_Gfirst))
				;
			else if (_Pred(*_Gfirst, *_Pfirst))
				break;
			else
				std::iter_swap(_Plast++, _Gfirst);
		for (; _First < _Glast; --_Glast)
			if (_Pred(*(_Glast - 1), *_Pfirst))
				;
			else if (_Pred(*_Pfirst, *(_Glast - 1)))
				break;
			else
				std::iter_swap(--_Pfirst, _Glast - 1);
		if (_Glast == _First && _Gfirst == _Last)
			return (pair<_RanIt, _RanIt>(_Pfirst, _Plast));

		if (_Glast == _First)
			{	// no room at bottom, rotate pivot upward
			if (_Plast != _Gfirst)
				std::iter_swap(_Pfirst, _Plast);
			++_Plast;
			std::iter_swap(_Pfirst++, _Gfirst++);
			}
		else if (_Gfirst == _Last)
			{	// no room at top, rotate pivot downward
			if (--_Glast != --_Pfirst)
				std::iter_swap(_Glast, _Pfirst);
			std::iter_swap(_Pfirst, --_Plast);
			}
		else
			std::iter_swap(_Gfirst++, --_Glast);
		}
	}
template<class _RanIt,
	class _Diff,
	class _Pr> inline
	void _Sort(_RanIt _First, _RanIt _Last, _Diff _Ideal, _Pr _Pred)
	{	// order [_First, _Last), using _Pred
	_Diff _Count;
	for (; _ISORT_MAX < (_Count = _Last - _First) && 0 < _Ideal; )
		{	// divide and conquer by quicksort
		pair<_RanIt, _RanIt> _Mid =
			_Unguarded_partition(_First, _Last, _Pred);
		_Ideal /= 2, _Ideal += _Ideal / 2;	// allow 1.5 log2(N) divisions

		if (_Mid.first - _First < _Last - _Mid.second)	// loop on larger half
			_Sort(_First, _Mid.first, _Ideal, _Pred), _First = _Mid.second;
		else
			_Sort(_Mid.second, _Last, _Ideal, _Pred), _Last = _Mid.first;
		}

	if (_ISORT_MAX < _Count)
		{	// heap sort if too many divisions
		std::make_heap(_First, _Last, _Pred);
		std::sort_heap(_First, _Last, _Pred);
		}
	else if (1 < _Count)
		_Insertion_sort(_First, _Last, _Pred);	// small, insertion sort
	}
template<class _RanIt,
	class _Pr> inline
	void sort(_RanIt _First, _RanIt _Last, _Pr _Pred)
	{	// order [_First, _Last), using _Pred
	_Sort(_First, _Last, _Last - _First, _Pred);
	}
}

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    operator Vector3() const { return *(const Vector3*)this; }

    float x;                         // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct AABox { // NAME: proto-pdb:default.pdb
    bool Contains(const Vector3& p) const
    {
        return p.x >= min.x && p.y >= min.y && p.z >= min.z && p.x <= max.x && p.y <= max.y && p.z <= max.z;
    }
    Vector3* GeneratePoints(Vector3* pts) const;

    static unsigned short boxIndices[];

    Vector3 min;                     // 0x00
    Vector3 max;                       // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct Matrix { // NAME: proto-pdb:default.pdb
    Matrix() {}
    void Multiply(const Matrix& m, Matrix& result) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct Frustum { // NAME: proto-pdb:default.pdb
    Frustum();
    bool IsVisible(const AABox& box, float tolerance) const;
    void CreateEmptyInverse(const Frustum& f);
    void TrimFrustum(const Frustum& f, const Vector3& eye, const Vector3* pts, unsigned int numPts);

    Vector4 _Plane[5];                 // 0x00
};
DECOMP_SIZE_ASSERT(Frustum, 0x50);

struct D3DResource; // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DResource_Register(D3DResource*, void*); // NAME: proto-pdb:default.pdb

struct D3DResource { // NAME: proto-pdb:default.pdb
    void Register(void* pBase) { D3DResource_Register(this, pBase); }

    unsigned long Common;             // 0x00
    unsigned long Data;                // 0x04
    unsigned long Lock;                // 0x08
};
DECOMP_SIZE_ASSERT(D3DResource, 0xC);

struct D3DVertexBuffer : public D3DResource { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(D3DVertexBuffer, 0xC);

enum _D3DPRIMITIVETYPE { D3DPT_TRIANGLELIST = 5, D3DPT_TRIANGLESTRIP = 6 }; // NAME: proto-pdb:default.pdb

extern "C" void __fastcall D3DDevice_SetRenderState_Simple(unsigned long, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_SetStreamSource(unsigned int, D3DVertexBuffer*, unsigned int); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawVertices(_D3DPRIMITIVETYPE, unsigned int, unsigned int); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawIndexedVertices(_D3DPRIMITIVETYPE, unsigned int, const unsigned short*); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_DrawIndexedVerticesUP(_D3DPRIMITIVETYPE, unsigned int, const void*, const void*, unsigned int); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_BeginVisibilityTest(); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_SetRenderState_StencilEnable(unsigned long); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall Sleep(unsigned long); // NAME: proto-pdb:default.pdb

enum _D3DTRANSFORMSTATETYPE { // NAME: proto-pdb:default.pdb
    D3DTS_VIEW = 0
};

struct _D3DMATRIX; // NAME: proto-pdb:default.pdb

extern "C" long __stdcall D3DDevice_GetTransform(_D3DTRANSFORMSTATETYPE, _D3DMATRIX*); // NAME: proto-pdb:default.pdb
extern "C" void __fastcall D3DDevice_SetVertexShaderConstant1(int, const void*); // NAME: proto-pdb:default.pdb
extern "C" void __fastcall D3DDevice_SetVertexShaderConstant4(int, const void*); // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle;
class VertexShaderResource; // NAME: proto-pdb:default.pdb

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    long SetStreamSource(D3DVertexBuffer* vb, unsigned int stride);
    void SetCullMode(unsigned long value);
    unsigned int EndRasterTest() const;
    int GetRasterTestResults(unsigned int handle) const;
    long SetVertexShader(const ResourceHandle<VertexShaderResource>& shader);
    void GetProjectionTransform(Matrix& m) const;
    void SetBasicPixelShader();
    void SetEmptyTexture(unsigned long stage);
    void SetVertexShaderConstant(int regnum, const Vector4* data, int count) const;
};

class MaterialManager; // NAME: proto-pdb:default.pdb
class VertexShaderManager; // NAME: proto-pdb:default.pdb
struct DelayedDraw; // NAME: proto-pdb:default.pdb
class SwampSceneNode; // NAME: proto-pdb:default.pdb
class RootNode; // NAME: proto-pdb:default.pdb
class BSP; // NAME: proto-pdb:default.pdb

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void AddNode(SwampSceneNode* node, unsigned int flags);
    RootNode* GetRootNode() const { return _root; }
    void AddDelayedItem(const DelayedDraw& dd);

    RootNode* _root;                   // 0x00
    char field_4[0x14 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScene, 0x14);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    void SetGlobalBSP(BSP* bsp);
    SwampRenderer& GetRenderer() const { return *_renderer; }
    SwampScene& GetScene() const { return *_scene; }
    MaterialManager& GetMaterialManager() const { return *_materialManager; }
    VertexShaderManager& GetVertexShaderManager() const { return *_vertexShaderManager; }

    char field_0[0x8];                 // 0x00
    SwampRenderer* _renderer;          // 0x08
    SwampScene* _scene;                // 0x0C
    char field_10[0x24 - 0x10];        // 0x10
    MaterialManager* _materialManager; // 0x24
    char field_28[0x3C - 0x28];        // 0x28
    VertexShaderManager* _vertexShaderManager; // 0x3C
    char field_40[0x68 - 0x40];        // 0x40
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

void BeepFree(void*); // NAME: proto-pdb:default.pdb

struct FloodChunk { // NAME: proto-pdb:default.pdb
    // src retail 0x000C4B10: child offsets follow the chunk (chunk + 0x90 + i * 4), relative to the chunk
    FloodChunk& GetChild(unsigned int i) const { return *(FloodChunk*)((char*)this + ((const int*)(this + 1))[i]); }

    unsigned int status;              // 0x00
    unsigned int materialID;           // 0x04
    unsigned int type;                 // 0x08
    unsigned int numVerts;             // 0x0C
    unsigned int numChildren;          // 0x10
    unsigned short baseAddrU;          // 0x14
    unsigned short baseAddrV;          // 0x16
    unsigned short detailAddrU;        // 0x18
    unsigned short detailAddrV;        // 0x1A
    Vector3 center;                    // 0x1C
    unsigned int render_flags;         // 0x28
    float fog_rcp_dist;                // 0x2C
    unsigned int fog_color;            // 0x30
    Vector4 plane;                     // 0x34
    unsigned short* indexBuffer;       // 0x44
    D3DVertexBuffer vertexBuffer;      // 0x48
    D3DVertexBuffer auxBuffer;         // 0x54
    AABox boundsChunk;                 // 0x60
    AABox boundsChildren;              // 0x78
};
DECOMP_SIZE_ASSERT(FloodChunk, 0x90);

struct FloodVertex { // NAME: proto-pdb:default.pdb
    Vector3 loc;                       // 0x00
    unsigned int norm;                 // 0x0C
    unsigned int color;                // 0x10
    unsigned int tex[3];               // 0x14
};
DECOMP_SIZE_ASSERT(FloodVertex, 0x20);

struct FloodAuxVertex { // NAME: proto-pdb:default.pdb
    unsigned int lightavg;             // 0x00
    unsigned int tangent;              // 0x04
};
DECOMP_SIZE_ASSERT(FloodAuxVertex, 0x8);

struct BogTriangle { // NAME: proto-pdb:default.pdb
    unsigned short material;           // 0x00
    unsigned short indices[3];         // 0x02
};
DECOMP_SIZE_ASSERT(BogTriangle, 0x8);

struct BogNode { // NAME: proto-pdb:default.pdb
    // src retail 0x000C4C0A: the triangles follow the node (node + 0x10 + i * 8)
    BogTriangle* GetTriangles() { return (BogTriangle*)(this + 1); }
    // src retail 0x000C5257: child offsets are relative to the node, 0 = none
    BogNode* GetFrontChild() { return offsetFrontChild ? (BogNode*)((char*)this + offsetFrontChild) : 0; }
    BogNode* GetBackChild() { return offsetBackChild ? (BogNode*)((char*)this + offsetBackChild) : 0; }

    unsigned int planeID;              // 0x00
    unsigned short numTrisFront;       // 0x04
    unsigned short numTrisBack;        // 0x06
    int offsetFrontChild;              // 0x08
    int offsetBackChild;               // 0x0C
};
DECOMP_SIZE_ASSERT(BogNode, 0x10);

struct FloodZoneFile { // NAME: proto-pdb:default.pdb
    FloodChunk& GetRootChunk() const { return *(FloodChunk*)((char*)this + offsetRootChunk); }
    FloodChunk* GetPortals() const { return (FloodChunk*)((char*)this + offsetPortals); }
    FloodChunk& GetExtraRoot() const { return *(FloodChunk*)((char*)this + offsetExtraRoot); }

    unsigned int id;                   // 0x00
    unsigned int version;              // 0x04
    int offsetRootChunk;               // 0x08
    unsigned int numPortals;           // 0x0C
    int offsetPortals;                 // 0x10
    bool usesAuxVerts;                 // 0x14
    bool usesExtraChunks;              // 0x15
    bool containsBogs;                 // 0x16
    bool reserved1;                    // 0x17
    int offsetExtraRoot;               // 0x18
    unsigned int reserved2;            // 0x1C
};
DECOMP_SIZE_ASSERT(FloodZoneFile, 0x20);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    // body visible, in-class (pick-any COMDAT, C0270): declaration-only, GetMaterial reloads the temp after the call
    // (92.0); fold VA 0x0001819A, the MaterialResource alias (C0233)
    ResourceHandle& operator=(const ResourceHandle& h)
    {
        if (_Resource)
            _Resource->SubRef();
        _Resource = h._Resource;
        if (_Resource)
            _Resource->AddRef();
        return *this;
    }
    bool IsValid() const { return _Resource != 0; }
    const T& operator()() const { return *_Resource; }

    T* _Resource;                      // 0x00
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    std::string _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    bool UsesAlpha() const;
};

class MaterialResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    const ResourceHandle<TextureResource>& GetBaseTexture() const { return _baseTex; }
    const ResourceHandle<TextureResource>& GetDetailTexture() const { return _detailTex; }

    char field_24[0x28 - 0x24];        // 0x24
    ResourceHandle<TextureResource> _baseTex; // 0x28
    ResourceHandle<TextureResource> _detailTex; // 0x2C
    char field_30[0x58 - 0x30];        // 0x30
};
DECOMP_SIZE_ASSERT(MaterialResource, 0x58);

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class BeepFile; // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    struct Record { // NAME: proto-pdb:default.pdb
        std::string name;              // 0x00
        std::string stringdata;        // 0x1C
        int intdata;                   // 0x38
        char field_3C[0x50 - 0x3C];    // 0x3C
    };

    BeepTDF(BeepFile& file, const std::string& name);
    ~BeepTDF();
    bool IsValid() const { return _valid; }
    const std::vector<Record*>& GetRecords() const { return _recordlist; }
    const std::vector<BeepTDF*>& GetBlocks() const { return _blocklist; }
    const BeepTDF* FindBlock(const std::string& name, bool recurse) const;
    int GetIntRecord(const std::string& name, int def) const;

    std::string _blockname;            // 0x00
    bool _valid;                       // 0x1C
    std::vector<Record*> _recordlist;  // 0x20
    char field_30[0xC];                // 0x30 std::map _recordlookup
    std::vector<BeepTDF*> _blocklist;  // 0x3C
    char field_4C[0xC];                // 0x4C std::map _blocklookup
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);

namespace std {
// !FAKE: inliner; decl-only explicit specialization; retail calls size out of line (fold VA 0x000C10C4) in Swamp_LoadWorld
template<> unsigned int vector<BeepTDF*>::size() const;
}

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime(); // NAME: proto-pdb:default.pdb
}

void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const std::string&);
};

class MaterialManager : public ResourceManager<MaterialResource> { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<MaterialResource> CreateMaterial(const BeepTDF* tdf);
    ResourceHandle<MaterialResource> GetDefaultMaterial() const { return _defaultMaterial; }

    char field_0[0x58];                // 0x00
    ResourceHandle<MaterialResource> _defaultMaterial; // 0x58
    char field_5C[0x7C - 0x5C];        // 0x5C
};
DECOMP_SIZE_ASSERT(MaterialManager, 0x7C);

class VertexShaderManager : public ResourceManager<VertexShaderResource> { // NAME: proto-pdb:default.pdb
};

const std::string format(const char*, ...); // NAME: proto-pdb:default.pdb

class SmartBase { // NAME: proto-pdb:default.pdb
public:
    virtual ~SmartBase() {}            // vtable+0x00
    void AddRef() { _RefCount++; }
    void SubRef()
    {
        if (--_RefCount == 0)
            delete this;
    }

    int _RefCount;                     // 0x04
};
DECOMP_SIZE_ASSERT(SmartBase, 0x8);

struct _D3DVERTEXATTRIBUTEFORMAT; // NAME: proto-pdb:default.pdb

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    Ptr& operator=(const Ptr& p);
    bool IsValid() const { return _Object != 0; }
    T* operator->() const { return _Object; }

    T* _Object;                        // 0x00
};

class PixelShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class HottieBundle; // NAME: proto-pdb:default.pdb

class BundleWrapper : public SmartBase { // NAME: proto-pdb:default.pdb
public:
    HottieBundle* GetPtr() { return _bundle; }

    HottieBundle* _bundle;             // 0x08
    unsigned int _size;                // 0x0C
    unsigned int _flags;               // 0x10
};
DECOMP_SIZE_ASSERT(BundleWrapper, 0x14);

class LightNode;   // NAME: proto-pdb:default.pdb

class LightManager { // NAME: proto-pdb:default.pdb
public:
    static LightNode* CreateDefinedLight(const BeepTDF* tdf);
    static LightNode* CreateDirectionalLight(const Vector3& dir, const Vector3& color);
    static void CreateShadowBuffers();
    static void ClearCache();
};

extern "C" void __stdcall D3D_FreeContiguousMemory(void*); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000C254C
// PROTOTYPE: PREVIEW 0x004AD54D ambiguous
// NAME: proto-pdb:default.pdb
// ??_GWorldData: compiler-generated with the vtable this unit emits; calls ??1 out of line (5.3 T5)
class WorldData : public SmartBase { // NAME: proto-pdb:default.pdb
public:
    struct ClipDistances { // NAME: proto-pdb:default.pdb
        float world;                   // 0x00
        float worldMirrored;           // 0x04
        float objects;                 // 0x08
        float objectsMirrored;         // 0x0C
    };

    // !FAKE: provenance; friend declaration; the protected ctor is called by Swamp_LoadWorld (retail 0x000C54D0)
    friend BeepTDF* Swamp_LoadWorld(const std::string& name, Ptr<BundleWrapper> dataBundle, Ptr<BundleWrapper> vertexBundle);

protected:
    WorldData();

public:
    virtual ~WorldData();                                          // vtable+0x00
    static const ClipDistances& GetClipDistances() { return _clipDistances; }
    ResourceHandle<MaterialResource> GetMaterial(unsigned int matID) const;
    ResourceHandle<VertexShaderResource> GetVertexShaderBase(unsigned int i) const { return _vShaderBase[i]; }

    static _D3DVERTEXATTRIBUTEFORMAT vafWorld;

protected:
    static ClipDistances _clipDistances;

public:

    FloodVertex* _vertices;            // 0x08
    FloodAuxVertex* _auxVertices;      // 0x0C
    ResourceHandle<VertexShaderResource> _vShaderBase[3]; // 0x10
    ResourceHandle<VertexShaderResource> _vShaderEnvmap[3]; // 0x1C
    ResourceHandle<VertexShaderResource> _vShaderBump[3]; // 0x28
    ResourceHandle<PixelShaderResource> _basicWorldPixelShader; // 0x34
    Ptr<BundleWrapper> _dataBundle;    // 0x38
    Ptr<BundleWrapper> _vertexBundle;  // 0x3C
    std::vector<LightNode*> _staticLights; // 0x40
};
DECOMP_SIZE_ASSERT(WorldData, 0x50);

class TextureResource;  // NAME: proto-pdb:default.pdb
class AnimationControl; // NAME: proto-pdb:default.pdb

struct Quaternion; // NAME: proto-pdb:default.pdb
class Bog { // NAME: proto-pdb:default.pdb
public:
    BogNode* GetRoot();

    char field_0[0x10];                // 0x00
};
DECOMP_SIZE_ASSERT(Bog, 0x10);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum SwampSceneNodeTypes { // NAME: proto-pdb:default.pdb
        Zone = 0x2
    };

    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3& pos);             // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    int GetType() const { return _Type; }
    void SetName(const std::string& name) { _Name = name; }
    void AddChild(SwampSceneNode* child);

protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

public:

    int _Type;                         // 0x04
    std::string _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class RootNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class FogZone : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    FogZone(const BeepTDF* tdf);

    char field_A4[0x130 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(FogZone, 0x130);

class BSP { // NAME: proto-pdb:default.pdb
public:
    BSP(void* data, bool b);

    char field_0[0x40];                // 0x00
};
DECOMP_SIZE_ASSERT(BSP, 0x40);

struct BOG_COLLECTION { // NAME: proto-pdb:default.pdb
    unsigned int magic;                // 0x00
    unsigned int version;              // 0x04
    unsigned int totalSize;            // 0x08
    unsigned int numBogs;              // 0x0C
};
DECOMP_SIZE_ASSERT(BOG_COLLECTION, 0x10);

class CollidableNode : public SwampSceneNode {         // NAME: proto-pdb:default.pdb
};
class CollidableCylinder : public CollidableNode {     // NAME: proto-pdb:default.pdb
};
class GatorNode : public CollidableCylinder {          // NAME: proto-pdb:default.pdb
};
struct GatorStrip; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000C4884
// PROTOTYPE: PREVIEW 0x004AFB45 ambiguous
// NAME: proto-pdb:default.pdb
// ??_GZoneNode: compiler-generated with the vtable this unit emits; calls ??1 out of line (5.3 T5)
class ZoneNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~ZoneNode();                                           // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    FloodZoneFile* GetZoneData() const { return _zoneData; }
    unsigned int GetID() const { return _zoneID; }
    void RasterCheckChunk(FloodChunk& chunk);
    void RasterCheckPortals(unsigned int visMask);
    void DoDelayedRender(FloodChunk& chunk, const Vector3& camPos, bool disableZWrite) const;
    void RenderYourself(const Frustum& viewFrustum, const Vector3& camPos, bool noEffects) const;
    void ReallyDrawChunk(FloodChunk& chunk) const;

    // !FAKE: provenance; friend declaration; the protected ctor is called by Swamp_LoadWorld (retail 0x000C54D0)
    friend BeepTDF* Swamp_LoadWorld(const std::string& name, Ptr<BundleWrapper> dataBundle, Ptr<BundleWrapper> vertexBundle);

protected:
    ZoneNode(unsigned int id, FloodZoneFile* zoneData, BOG_COLLECTION* bogs, WorldData* world);
    void Init();
    void SetupForRasterCheck() const;
    void CleanupAfterRasterCheck() const;
    void RenderChunks(FloodChunk& chunk, const Frustum& viewFrustum, const Vector3& camPos, bool noEffects) const;
    void ZoneRenderSetup(const Vector3& camPos, bool noEffects) const;
    void ZoneRenderCleanup() const;
    void ChunkRenderSetup(FloodChunk& chunk, const ResourceHandle<MaterialResource>& mat, unsigned int& stateModifications, const _D3DVERTEXATTRIBUTEFORMAT* vaf) const;
    void ChunkRenderCleanup(const FloodChunk& chunk, const unsigned int& stateModifications) const;
    void RenderChunk(FloodChunk& chunk) const;
    void RenderBog(FloodChunk& chunk, const Vector3& camPos) const;
    void RegisterChunkBuffers(FloodChunk& chunk, const FloodVertex* vertexBase, const FloodAuxVertex* auxBase, std::map<unsigned int, unsigned int>& materialMap, unsigned int depth);
    void RemapBogMaterial(BogNode* node, std::map<unsigned int, unsigned int>& materialMap);

public:
    Ptr<WorldData> _world;             // 0xA4
    FloodZoneFile* _zoneData;          // 0xA8
    void* _zoneAllocation;             // 0xAC
    unsigned int _zoneID;              // 0xB0
    std::vector<Bog*> _bogs;           // 0xB4
    std::vector<ResourceHandle<MaterialResource> > _materials; // 0xC4
    std::vector<FloodChunk*> _chunkList; // 0xD4
    char field_E4[0x124 - 0xE4];       // 0xE4
};
DECOMP_SIZE_ASSERT(ZoneNode, 0x124);

namespace std {
// !FAKE: inliner; explicit specialization; retail calls size out of line (fold VA 0x000C10C4) and reuses the key load across it (decl-only 88.70 -> 100.0, RegisterChunkBuffers)
template<> unsigned int vector<ResourceHandle<MaterialResource> >::size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
}

class LightDefinition { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x24];                // 0x00
    float _glowSize;                   // 0x24
    char field_28[0x47 - 0x28];        // 0x28
    bool _castsWorldShadows;           // 0x47
};
DECOMP_SIZE_ASSERT(LightDefinition, 0x48);

class LightNode : public SwampSceneNode, public LightDefinition { // NAME: proto-pdb:default.pdb
public:
    bool NeedsToBeInTheScene() const;
    void UpdateCachedWorldPosition();

    char field_EC[0x158 - 0xEC];       // 0xEC
    ResourceHandle<AnimationControl> _currentPath; // 0x158
    char field_15C[0x160 - 0x15C];     // 0x15C
};
DECOMP_SIZE_ASSERT(LightNode, 0x160);

struct DelayedDraw { // NAME: proto-pdb:default.pdb
    DelayedDraw(const Matrix& trans, const ZoneNode* zone, FloodChunk* chunk);

    Matrix _trans;                     // 0x00
    float _distance;                   // 0x40
    const SwampSceneNode* _node;       // 0x44
    const ZoneNode* _zone;             // 0x48
    FloodChunk* _chunk;                // 0x4C
    unsigned int _rasterTest;          // 0x50
};
DECOMP_SIZE_ASSERT(DelayedDraw, 0x54);

struct HottieFile; // NAME: proto-pdb:default.pdb

struct HottieRawData { // NAME: proto-pdb:default.pdb
    void* header;                      // 0x00
    void* data;                        // 0x04
    unsigned int size;                 // 0x08
};
DECOMP_SIZE_ASSERT(HottieRawData, 0xC);

class HottieBundle { // NAME: proto-pdb:default.pdb
public:
    const HottieFile* GetFileInfo(const std::string& name) const;
    HottieRawData GetRawFileData(const HottieFile* hf) const;
    HottieRawData GetRawFileData(const std::string& name) const;
    BeepFile* OpenBundledFile(const std::string& name) const;
    void CloseBundledFile(BeepFile* file) const;

    char field_0[0x1C];                // 0x00
};
DECOMP_SIZE_ASSERT(HottieBundle, 0x1C);

class DrawLevel { // NAME: proto-pdb:default.pdb
public:
    struct ZoneRenderInfo { // NAME: proto-pdb:default.pdb
        ZoneNode* node;                // 0x00
        Frustum frustum;               // 0x04
    };
    struct MirrorInfo { // NAME: proto-pdb:default.pdb
        const SwampSceneNode* node;    // 0x00
        FloodChunk* chunk;             // 0x04
        const GatorStrip* strip;       // 0x08
        unsigned int stencilID;        // 0x0C
        unsigned int rasterCheck;      // 0x10
    };

    DrawLevel(const Vector3& camPos, const Frustum& viewFrustum, const std::list<SwampSceneNode*>& potentialNodes);
    void InitMirrorStuff();
    static void AddMirrorChunk(const ZoneNode* zone, FloodChunk* chunk);
    static MirrorInfo* BeginGatorMirrorProcessing(const GatorNode* gator, const GatorStrip* strip);
    static void EndGatorMirrorProcessing(MirrorInfo* mi);
    static bool AreWeMirroring() { return _mirroringHappening; }
    static void EnableMirrors(bool enable);
    static bool AreMirrorsEnabled() { return _mirrorsEnabled; }
    void BeginMirrorProcessing();
    void DrawInitialStuff();
    bool DrawPendingStuff(std::vector<ZoneRenderInfo>& zr);

protected:
    bool QueuePortalChecks();

public:

    Vector3 _camPos;                   // 0x00
    const Frustum& _viewFrustum;       // 0x0C
    const std::list<SwampSceneNode*>& _potentialNodes; // 0x10
    std::vector<ZoneRenderInfo> _zonesRendered; // 0x14
    ZoneNode* _zones[16];              // 0x24
    unsigned int _zoneVisibility[16];  // 0x64
    bool _zoneTested[16];              // 0xA4

protected:
    static std::list<MirrorInfo> _mirrors;
    static unsigned int _mirrorStencil;
    static bool _mirroringHappening;
    static bool _renderMirrors;
    static bool _mirrorsEnabled;
};
DECOMP_SIZE_ASSERT(DrawLevel, 0xB4);

struct SwampScript; // NAME: proto-pdb:default.pdb

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    struct ScriptTimerEvent { // NAME: proto-pdb:default.pdb
        SwampScript* script;           // 0x00
        unsigned int id;               // 0x04
        bool looped;                   // 0x08
        float deltaTime;               // 0x0C
        float absTime;                 // 0x10
    };
};
DECOMP_SIZE_ASSERT(SwampScriptManager::ScriptTimerEvent, 0x14);

typedef std::vector<FloodChunk*>::iterator FloodChunkIt;
typedef bool (__cdecl* FloodChunkPred)(FloodChunk*, FloodChunk*);

// FUNCTION: USA 0x000C1053
// PROTOTYPE: PREVIEW 0x004ABFE6 identical
// NAME: proto-pdb:default.pdb
bool ChunkMaterialSorter(FloodChunk* a, FloodChunk* b)
{
    return a->materialID < b->materialID;
}

// FUNCTION: USA 0x000C1066
// PROTOTYPE: PREVIEW 0x004ABFF9 identical
// NAME: proto-pdb:default.pdb
const AABox& ZoneNode::GetBounds() const
{
    return _zoneData->GetRootChunk().boundsChildren;
}

// FUNCTION: USA 0x000C109C
// PROTOTYPE: PREVIEW 0x004AC02F identical
// NAME: proto-pdb:default.pdb
void DrawLevel::EnableMirrors(bool enable)
{
    _mirrorsEnabled = enable;
    if (!enable)
        _mirrorStencil = 0xFFFFFFFF;
}

// FUNCTION: USA 0x000C10B1
// PROTOTYPE: PREVIEW 0x004AC06D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; relink: FFlyController.obj, SwampGatorNode.obj and others reference capacity aliases at this fold VA lie in a replaced range (C0233)
template unsigned int std::vector<ResourceHandle<TextureResource> >::capacity() const;
#endif

// FUNCTION: USA 0x000C10C4
// PROTOTYPE: PREVIEW 0x004AC05A identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; relink: ParticleManager.obj's external ?size@?$vector@PAVFireFlyNode@@...@QBEIXZ at this fold VA lie in a replaced range (C0233)
template unsigned int std::vector<ResourceHandle<TextureResource> >::size() const;
#endif

// FUNCTION: USA 0x000C1169
// PROTOTYPE: PREVIEW 0x004AC132 identical
// NAME: proto-pdb:default.pdb
DelayedDraw::DelayedDraw(const Matrix& trans, const ZoneNode* zone, FloodChunk* chunk)
    : _trans(trans), _distance(0.0f), _node(0), _zone(zone), _chunk(chunk), _rasterTest(0xFFFFFFFF)
{
}

// FUNCTION: USA 0x000C1198
// PROTOTYPE: PREVIEW 0x004AC161 identical
// NAME: proto-pdb:default.pdb
bool LightNode::NeedsToBeInTheScene() const
{
    return _glowSize > 0.0f || _castsWorldShadows || _currentPath.IsValid();
}

// FUNCTION: USA 0x000C11C1
// PROTOTYPE: PREVIEW 0x004AC18A identical
// NAME: proto-pdb:default.pdb
HottieRawData HottieBundle::GetRawFileData(const std::string& name) const
{
    return GetRawFileData(GetFileInfo(name));
}

// FUNCTION: USA 0x000C11E1
// PROTOTYPE: PREVIEW 0x004AC1AA identical
// NAME: proto-pdb:default.pdb
void ZoneNode::CleanupAfterRasterCheck() const
{
    // !FAKE: c2 register allocation; stand-in local; retail loads g_swamp->_renderer once before the render-state calls
    SwampRenderer& v0 = g_swamp->GetRenderer();
    D3DDevice_SetRenderState_Simple(0x4035C, 1);
    D3DDevice_SetRenderState_Simple(0x40358, 0x1010101);
    v0.SetCullMode(0x901);
    v0.SetStreamSource(0, 0);
}

// FUNCTION: USA 0x000C12DD
// PROTOTYPE: PREVIEW 0x004AC2A6 identical
// NAME: proto-pdb:default.pdb
void ZoneNode::ReallyDrawChunk(FloodChunk& chunk) const
{
    g_swamp->GetRenderer().SetStreamSource(&chunk.vertexBuffer, 0x20);
    if (chunk.auxBuffer.Data)
        D3DDevice_SetStreamSource(1, &chunk.auxBuffer, 8);
    switch (chunk.type) {
    case 0:
        D3DDevice_DrawVertices(D3DPT_TRIANGLELIST, 0, chunk.numVerts);
        break;
    case 2:
        D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLESTRIP, chunk.numVerts, chunk.indexBuffer);
        break;
    case 1:
        D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLELIST, chunk.numVerts, chunk.indexBuffer);
        break;
    }
}

// FUNCTION: USA 0x000C1355
// PROTOTYPE: PREVIEW 0x004AC31E identical
// NAME: proto-pdb:default.pdb
void DrawLevel::EndGatorMirrorProcessing(MirrorInfo* mi)
{
    if (mi) {
        mi->rasterCheck = g_swamp->GetRenderer().EndRasterTest();
        D3DDevice_SetRenderState_Simple(0x40358, 0x1010101);
        D3DDevice_SetRenderState_Simple(0x40378, 0);
        D3DDevice_SetRenderState_Simple(0x40368, 0);
    }
}

// FUNCTION: USA 0x000C142E
// PROTOTYPE: PREVIEW 0x004AC45C identical
// NAME: proto-pdb:default.pdb
template void std::_Med3<FloodChunkIt, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkIt, FloodChunkPred);

// FUNCTION: USA 0x000C1481
// PROTOTYPE: PREVIEW 0x004AC4AF identical
// NAME: proto-pdb:default.pdb
template void std::_Push_heap<FloodChunkIt, int, FloodChunk*, FloodChunkPred>(FloodChunkIt, int, int, FloodChunk*, FloodChunkPred);

// FUNCTION: USA 0x000C156A
// PROTOTYPE: PREVIEW 0x004AC598 identical
// NAME: proto-pdb:default.pdb
void ZoneNode::SetupForRasterCheck() const
{
    SwampRenderer& renderer = g_swamp->GetRenderer();
    renderer.SetVertexShader(_world->GetVertexShaderBase(0));
    Matrix view;
    D3DDevice_GetTransform(D3DTS_VIEW, (_D3DMATRIX*)&view);
    Matrix proj;
    renderer.GetProjectionTransform(proj);
    // !FAKE: inliner; hand form of the retail-inlined Matrix product by value: stand-in v0 for its result temp; the PDB records only worldproj
    Matrix v0;
    view.Multiply(proj, v0);
    Matrix worldproj = v0;
    // !FAKE: inliner; hand form of the retail-inlined SwampRenderer::SetVertexShaderConstant (register + 96); Hannah.obj
    // defines that member out of line, so ours stays decl-only (LNK2005)
    D3DDevice_SetVertexShaderConstant4(0 + 96, worldproj.row);
    Vector4 materialInfo;
    materialInfo.x = 0.0f;
    materialInfo.y = 0.0f;
    materialInfo.z = 0.5f;
    materialInfo.w = 1.0f;
    D3DDevice_SetVertexShaderConstant1(8 + 96, &materialInfo);
    Vector4 T0Scaling;
    T0Scaling.x = 0.0f;
    T0Scaling.y = 0.0f;
    T0Scaling.z = 0.0f;
    T0Scaling.w = 16.0f;
    D3DDevice_SetVertexShaderConstant1(6 + 96, &T0Scaling);
    Vector4 T1Scaling;
    T1Scaling.x = 0.0f;
    T1Scaling.y = 0.0f;
    T1Scaling.z = 0.0f;
    T1Scaling.w = 16.0f;
    D3DDevice_SetVertexShaderConstant1(7 + 96, &T1Scaling);
    D3DDevice_SetRenderState_Simple(0x4035C, 0);
    D3DDevice_SetRenderState_Simple(0x40358, 0);
    renderer.SetCullMode(0);
    renderer.SetBasicPixelShader();
    renderer.SetEmptyTexture(0);
}

// FUNCTION: USA 0x000C1686
// PROTOTYPE: PREVIEW 0x004AC6B4 identical
// NAME: proto-pdb:default.pdb
void ZoneNode::RasterCheckChunk(FloodChunk& chunk)
{
    SetupForRasterCheck();
    SwampRenderer& renderer = g_swamp->GetRenderer();
    D3DDevice_BeginVisibilityTest();
    Vector3 pts[8];
    chunk.boundsChildren.GeneratePoints(pts);
    FloodVertex v[8];
    for (int i = 0; i < 8; i++)
        v[i].loc = pts[i];
    D3DDevice_DrawIndexedVerticesUP(D3DPT_TRIANGLESTRIP, 0xE, AABox::boxIndices, v, sizeof(FloodVertex));
    chunk.status = renderer.EndRasterTest();
    CleanupAfterRasterCheck();
}

// FUNCTION: USA 0x000C1705
// PROTOTYPE: PREVIEW 0x004AC733 identical
// NAME: proto-pdb:default.pdb
void ZoneNode::RasterCheckPortals(unsigned int visMask)
{
    SwampRenderer& renderer = g_swamp->GetRenderer();
    SetupForRasterCheck();
    for (unsigned int i = 0; i < _zoneData->numPortals; i++) {
        if (visMask & (1 << i)) {
            FloodChunk& p = _zoneData->GetPortals()[i];
            D3DDevice_BeginVisibilityTest();
            ReallyDrawChunk(p);
            p.status = renderer.EndRasterTest();
        }
    }
    CleanupAfterRasterCheck();
}

// FUNCTION: USA 0x000C1AA9
// PROTOTYPE: PREVIEW 0x0040072B identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; relink: CameraManager.obj's external ?clear@?$list@PAVSwampSceneNode@@...@QAEXXZ at this fold VA lies in a replaced range (C0233)
template void std::list<SwampScriptManager::ScriptTimerEvent>::clear();
#endif

// FUNCTION: USA 0x000C1D7E
// PROTOTYPE: PREVIEW 0x004ACD7F identical
// NAME: proto-pdb:default.pdb
template void std::_Median<FloodChunkIt, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkIt, FloodChunkPred);

// FUNCTION: USA 0x000C1E14
// PROTOTYPE: PREVIEW 0x004ACE15 identical
// NAME: proto-pdb:default.pdb
template void std::_Adjust_heap<FloodChunkIt, int, FloodChunk*, FloodChunkPred>(FloodChunkIt, int, int, FloodChunk*, FloodChunkPred);

// FUNCTION: USA 0x000C1E7D
// PROTOTYPE: PREVIEW 0x004ACE7E identical
// NAME: proto-pdb:default.pdb
template void std::_Pop_heap<FloodChunkIt, int, FloodChunk*, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkIt, FloodChunk*, FloodChunkPred, int*);

// FUNCTION: USA 0x000C1EA6
// PROTOTYPE: PREVIEW 0x004ACEA7 identical
// NAME: proto-pdb:default.pdb
bool DrawLevel::QueuePortalChecks()
{
    bool retval = false;
    for (unsigned int z = 0; z < 16; z++) {
        // !FAKE: c2 register allocation; stand-in local; retail holds _zones[z] in a register (spilled across calls)
        ZoneNode* v0 = _zones[z];
        if (!v0)
            break;
        bool& tested = _zoneTested[z];
        if (_zoneVisibility[z] == 0) {
            tested = false;
            FloodZoneFile* zd = v0->GetZoneData();
            if (zd->numPortals == 0) {
                v0->RasterCheckChunk(zd->GetRootChunk());
                tested = true;
                retval = true;
            } else {
                bool found = false;
                for (unsigned int i = 0; i < zd->numPortals; i++) {
                    FloodChunk& portal = zd->GetPortals()[i];
                    for (std::vector<ZoneRenderInfo>::const_iterator it = _zonesRendered.begin(); it != _zonesRendered.end(); ++it) {
                        if ((*it).node->GetID() == portal.materialID) {
                            if ((*it).frustum.IsVisible(portal.boundsChunk, 0.0f))
                                _zoneVisibility[z] |= 1 << i;
                            found = true;
                        }
                    }
                }
                if (found) {
                    if (_zoneVisibility[z] == 0) {
                        _zoneVisibility[z] = 0xFFFFFFFF;
                    } else {
                        v0->RasterCheckPortals(_zoneVisibility[z]);
                        tested = true;
                        retval = true;
                    }
                }
            }
        }
    }
    return retval;
}

// FUNCTION: USA 0x000C20DE
// PROTOTYPE: PREVIEW 0x004AD0DF identical
// NAME: proto-pdb:default.pdb
template std::pair<FloodChunkIt, FloodChunkIt> std::_Unguarded_partition<FloodChunkIt, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkPred);

// FUNCTION: USA 0x000C2276
// PROTOTYPE: PREVIEW 0x004AD277 identical
// NAME: proto-pdb:default.pdb
template void std::_Make_heap<FloodChunkIt, int, FloodChunk*, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkPred, int*, FloodChunk**);

// FUNCTION: USA 0x000C22AC
// PROTOTYPE: PREVIEW 0x004AD2AD identical
// NAME: proto-pdb:default.pdb
template void std::_Pop_heap_0<FloodChunkIt, FloodChunk*, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkPred, FloodChunk**);

// FUNCTION: USA 0x000C22CA
// NAME: proto-pdb:default.pdb
ResourceHandle<MaterialResource> WorldData::GetMaterial(unsigned int matID) const
{
    std::string name = format("%i", matID);
    ResourceHandle<MaterialResource> mat = g_swamp->GetMaterialManager().Load(name);
    if (!mat.IsValid())
        mat = g_swamp->GetMaterialManager().GetDefaultMaterial();
    return mat;
}

// FUNCTION: USA 0x000C2351
// PROTOTYPE: PREVIEW 0x004AD352 identical
// NAME: proto-pdb:default.pdb
void DrawLevel::InitMirrorStuff()
{
    _mirrors.erase(_mirrors.begin(), _mirrors.end());
    _mirroringHappening = false;
    _renderMirrors = false;
    _mirrorStencil = _mirrorsEnabled ? 0 : 0xFFFFFFFF;
}

// FUNCTION: USA 0x000C23AC
// PROTOTYPE: PREVIEW 0x004AD3AD identical
// NAME: proto-pdb:default.pdb
template void std::make_heap<FloodChunkIt, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkPred);

// FUNCTION: USA 0x000C23D4
// PROTOTYPE: PREVIEW 0x004AD3D5 identical
// NAME: proto-pdb:default.pdb
template void std::_Insertion_sort<FloodChunkIt, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkPred);

// FUNCTION: USA 0x000C2445
// PROTOTYPE: PREVIEW 0x004AD446 identical
// NAME: proto-pdb:default.pdb
template void std::pop_heap<FloodChunkIt, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkPred);

// FUNCTION: USA 0x000C246B
// PROTOTYPE: PREVIEW 0x004AD46C identical
// NAME: proto-pdb:default.pdb
WorldData::~WorldData()
{
    if (!_vertexBundle._Object) {
        if (_vertices)
            D3D_FreeContiguousMemory(_vertices);
        if (_auxVertices)
            D3D_FreeContiguousMemory(_auxVertices);
    }
    LightManager::ClearCache();
    for (std::vector<LightNode*>::const_iterator it = _staticLights.begin(); it != _staticLights.end(); ++it)
        delete *it;
}

// FUNCTION: USA 0x000C2521
// PROTOTYPE: PREVIEW 0x004AD522 identical
// NAME: proto-pdb:default.pdb
template void std::sort_heap<FloodChunkIt, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkPred);

// FUNCTION: USA 0x000C2568
// PROTOTYPE: PREVIEW 0x004AD569 identical
// NAME: proto-pdb:default.pdb
template void std::_Sort<FloodChunkIt, int, FloodChunkPred>(FloodChunkIt, FloodChunkIt, int, FloodChunkPred);

// FUNCTION: USA 0x000C2624
// PROTOTYPE: PREVIEW 0x004AD625 identical
// NAME: proto-pdb:default.pdb
void DrawLevel::BeginMirrorProcessing()
{
    if (_mirrors.size() == 0)
        return;
    unsigned int savedStencilValue = _mirrorStencil;
    _mirrorStencil = 0xFFFFFFFF;
    SwampRenderer& renderer = g_swamp->GetRenderer();
    ResourceHandle<VertexShaderResource> vs = g_swamp->GetVertexShaderManager().Load("WorldBasicC");
    renderer.SetVertexShader(vs);
    Matrix view;
    D3DDevice_GetTransform(D3DTS_VIEW, (_D3DMATRIX*)&view);
    Matrix proj;
    renderer.GetProjectionTransform(proj);
    // !FAKE: inliner; hand form of the retail-inlined Matrix product by value: stand-in v0 for its result temp; the PDB records only worldproj
    Matrix v0;
    view.Multiply(proj, v0);
    Matrix worldproj = v0;
    // !FAKE: inliner; hand form of the retail-inlined SwampRenderer::SetVertexShaderConstant (register + 96); Hannah.obj
    // defines that member out of line, so ours stays decl-only (LNK2005)
    D3DDevice_SetVertexShaderConstant4(0 + 96, worldproj.row);
    D3DDevice_SetRenderState_Simple(0x40358, 0);
    D3DDevice_SetRenderState_StencilEnable(1);
    D3DDevice_SetRenderState_Simple(0x40378, 0x1E01);
    renderer.SetBasicPixelShader();
    renderer.SetEmptyTexture(0);
    for (std::list<MirrorInfo>::iterator it = _mirrors.begin(); it != _mirrors.end(); ++it) {
        D3DDevice_SetRenderState_Simple(0x40368, (*it).stencilID);
        D3DDevice_BeginVisibilityTest();
        ((const ZoneNode*)(*it).node)->ReallyDrawChunk(*(*it).chunk);
        (*it).rasterCheck = renderer.EndRasterTest();
    }
    _mirrorStencil = savedStencilValue;
    renderer.SetStreamSource(0, 0);
    D3DDevice_SetRenderState_Simple(0x40358, 0x1010101);
    D3DDevice_SetRenderState_Simple(0x40378, 0);
    D3DDevice_SetRenderState_Simple(0x40368, 0);
}

// FUNCTION: USA 0x000C28DB
// PROTOTYPE: PREVIEW 0x004ADB9C identical
// NAME: proto-pdb:default.pdb
template void std::sort<FloodChunkIt, FloodChunkPred>(FloodChunkIt, FloodChunkIt, FloodChunkPred);

// FUNCTION: USA 0x000C2F48
// PROTOTYPE: PREVIEW 0x004AE209 identical
// NAME: proto-pdb:default.pdb
void ZoneNode::RenderChunk(FloodChunk& chunk) const
{
    unsigned int stateModifications = 0;
    ChunkRenderSetup(chunk, _materials[chunk.materialID], stateModifications, &WorldData::vafWorld);
    ReallyDrawChunk(chunk);
    ChunkRenderCleanup(chunk, stateModifications);
}

// FUNCTION: USA 0x000C3202
// NAME: proto-pdb:default.pdb
void ZoneNode::DoDelayedRender(FloodChunk& chunk, const Vector3& camPos, bool disableZWrite) const
{
    ZoneRenderSetup(camPos, true);
    if (chunk.type == 3) {
        D3DDevice_SetRenderState_Simple(0x40300, 1);
        D3DDevice_SetRenderState_Simple(0x40340, 0x20);
        D3DDevice_SetRenderState_Simple(0x4033C, 0x204);
        RenderBog(chunk, camPos);
        D3DDevice_SetRenderState_Simple(0x40300, 1);
    } else {
        if (disableZWrite)
            D3DDevice_SetRenderState_Simple(0x4035C, 0);
        RenderChunk(chunk);
        if (disableZWrite)
            D3DDevice_SetRenderState_Simple(0x4035C, 1);
    }
    ZoneRenderCleanup();
}

// FUNCTION: USA 0x000C4333
// PROTOTYPE: PREVIEW 0x004AF5F4 identical
// NAME: proto-pdb:default.pdb
ZoneNode::~ZoneNode()
{
    BeepFree(_zoneAllocation);
}

// FUNCTION: USA 0x000C48A0
// PROTOTYPE: PREVIEW 0x004AFB61 identical
// NAME: proto-pdb:default.pdb
DrawLevel::DrawLevel(const Vector3& camPos, const Frustum& viewFrustum, const std::list<SwampSceneNode*>& potentialNodes)
    : _camPos(camPos), _viewFrustum(viewFrustum), _potentialNodes(potentialNodes)
{
    unsigned int i = 0;
    for (std::list<SwampSceneNode*>::const_iterator it = _potentialNodes.begin(); it != _potentialNodes.end(); ++it) {
        if ((*it)->GetType() == SwampSceneNode::Zone)
            _zones[i++] = (ZoneNode*)*it;
    }
    for (; i < 16; i++)
        _zones[i] = 0;
}

// FUNCTION: USA 0x000C4907
// PROTOTYPE: PREVIEW 0x004AFBC8 identical
// NAME: proto-pdb:default.pdb
void DrawLevel::AddMirrorChunk(const ZoneNode* zone, FloodChunk* chunk)
{
    if (_mirrorStencil != 0xFFFFFFFF) {
        MirrorInfo mi;
        mi.node = zone;
        mi.chunk = chunk;
        mi.strip = 0;
        mi.stencilID = ++_mirrorStencil;
        mi.rasterCheck = 0;
        _mirrors.push_back(mi);
    }
}

// FUNCTION: USA 0x000C494A
// PROTOTYPE: PREVIEW 0x004AFC0B identical
// NAME: proto-pdb:default.pdb
DrawLevel::MirrorInfo* DrawLevel::BeginGatorMirrorProcessing(const GatorNode* gator, const GatorStrip* strip)
{
    if (_mirrorStencil == 0xFFFFFFFF)
        return 0;
    MirrorInfo mi;
    mi.node = gator;
    mi.chunk = 0;
    mi.strip = strip;
    mi.stencilID = ++_mirrorStencil;
    mi.rasterCheck = 0;
    _mirrors.push_back(mi);
    D3DDevice_SetRenderState_Simple(0x40358, 0);
    D3DDevice_SetRenderState_StencilEnable(1);
    D3DDevice_SetRenderState_Simple(0x40378, 0x1E01);
    D3DDevice_SetRenderState_Simple(0x40368, mi.stencilID);
    D3DDevice_BeginVisibilityTest();
    return &_mirrors.back();
}

// FUNCTION: USA 0x000C4B10
// PROTOTYPE: PREVIEW 0x004AFDD1 identical
// NAME: proto-pdb:default.pdb
void ZoneNode::RegisterChunkBuffers(FloodChunk& chunk, const FloodVertex* vertexBase, const FloodAuxVertex* auxBase, std::map<unsigned int, unsigned int>& materialMap, unsigned int depth)
{
    chunk.fog_color = 0xFFFFFFFF;
    _chunkList.push_back(&chunk);
    if (chunk.type == 3)
        return;
    chunk.vertexBuffer.Register((void*)vertexBase);
    if (chunk.auxBuffer.Common != 0)
        chunk.auxBuffer.Register((void*)auxBase);
    if (chunk.indexBuffer)
        chunk.indexBuffer = (unsigned short*)((char*)&chunk + (unsigned char)(unsigned int)chunk.indexBuffer);
    if (chunk.materialID) {
        std::pair<std::map<unsigned int, unsigned int>::iterator, bool> mapper =
            materialMap.insert(std::map<unsigned int, unsigned int>::value_type(chunk.materialID, _materials.size()));
        if (mapper.second) {
            ResourceHandle<MaterialResource> mat = _world->GetMaterial(chunk.materialID);
            _materials.push_back(mat);
        }
        chunk.materialID = mapper.first->second;
    }
    for (unsigned int i = 0; i < chunk.numChildren; i++)
        RegisterChunkBuffers(chunk.GetChild(i), vertexBase, auxBase, materialMap, depth + 1);
}

// FUNCTION: USA 0x000C4C0A
// PROTOTYPE: PREVIEW 0x004AFECB identical
// NAME: proto-pdb:default.pdb
void ZoneNode::RemapBogMaterial(BogNode* node, std::map<unsigned int, unsigned int>& materialMap)
{
    unsigned int triCount = node->numTrisFront + node->numTrisBack;
    if (triCount == 0)
        return;
    for (int i = 0; i < triCount; i++) {
        // !FAKE: T12.c2c loop form; stand-in local; retail recomputes &triangles[i] from i and node each iteration (no strength reduction)
        BogTriangle* v1 = node->GetTriangles();
        if (v1[i].material) {
            std::pair<std::map<unsigned int, unsigned int>::iterator, bool> mapper =
                materialMap.insert(std::map<unsigned int, unsigned int>::value_type(v1[i].material, _materials.size()));
            if (mapper.second) {
                ResourceHandle<MaterialResource> mat = _world->GetMaterial(v1[i].material);
                _materials.push_back(mat);
            }
            v1[i].material = (unsigned short)mapper.first->second;
        }
    }
}

// FUNCTION: USA 0x000C4CB5
// PROTOTYPE: PREVIEW 0x004AFF76 identical
// NAME: proto-pdb:default.pdb
void ZoneNode::RenderChunks(FloodChunk& chunk, const Frustum& viewFrustum, const Vector3& camPos, bool noEffects) const
{
    // !FAKE: T12.c2 register allocation; stand-in bool; retail keeps the visibility result in al with bogChunk's value (T12.c2c: branch on bogChunk)
    const bool bogChunk = chunk.type == 3;
    bool visible = bogChunk;
    if (!bogChunk && chunk.numVerts != 0)
        visible = viewFrustum.IsVisible(chunk.boundsChunk, DrawLevel::AreWeMirroring() ? WorldData::GetClipDistances().worldMirrored : WorldData::GetClipDistances().world);
    if (visible) {
        const ResourceHandle<MaterialResource>& v1 = _materials[chunk.materialID];
        ResourceHandle<TextureResource> baseTex = v1().GetBaseTexture();
        ResourceHandle<TextureResource> detailTex = v1().GetDetailTexture();
        bool deferredRender = bogChunk;
        if (baseTex.IsValid() && baseTex().UsesAlpha())
            deferredRender = true;
        if (chunk.render_flags & 6) {
            if (!noEffects && DrawLevel::AreMirrorsEnabled())
                DrawLevel::AddMirrorChunk(this, &chunk);
            else
                RenderChunk(chunk);
        } else if (deferredRender) {
            if (!noEffects) {
                DelayedDraw dd(Matrix(), this, &chunk);
                Vector3 v2 = viewFrustum._Plane[4];
                Vector3 points[8];
                chunk.boundsChunk.GeneratePoints(points);
                dd._distance = 0.0f;
                for (int i = 0; i < 8; i++) {
                    // !FAKE: c2 strength reduction; stand-in reference; loop pointer based at &points[0].y without it, retail &points[0].z (T12.c29 probe: 1 diff -> byte-equal)
                    const Vector3& p = points[i];
                    Vector3 v;
                    v.x = p.x - camPos.x;
                    v.y = p.y - camPos.y;
                    v.z = p.z - camPos.z;
                    float dist = v.x * v2.x + v.y * v2.y + v.z * v2.z;
                    if (dist > dd._distance)
                        dd._distance = dist;
                }
                g_swamp->GetScene().AddDelayedItem(dd);
            }
        } else {
            RenderChunk(chunk);
        }
    }
}

// FUNCTION: USA 0x000C4E42
// PROTOTYPE: PREVIEW 0x004B0103 identical
// NAME: proto-pdb:default.pdb
void ZoneNode::RenderYourself(const Frustum& viewFrustum, const Vector3& camPos, bool noEffects) const
{
    ZoneRenderSetup(camPos, false);
    for (std::vector<FloodChunk*>::const_iterator it = _chunkList.begin(); it != _chunkList.end(); ++it)
        RenderChunks(**it, viewFrustum, camPos, noEffects);
    ZoneRenderCleanup();
}

// FUNCTION: USA 0x000C4E83
// PROTOTYPE: PREVIEW 0x004B0144 identical
// NAME: proto-pdb:default.pdb
void DrawLevel::DrawInitialStuff()
{
    int priorities[16];
    int bestPriority = 0x913;
    for (unsigned int z = 0; z < 16; z++) {
        // !FAKE: c2 register allocation; stand-in local; retail holds _zones[z] in a register (spilled across calls)
        ZoneNode* v0 = _zones[z];
        if (!v0)
            break;
        priorities[z] = 0;
        for (unsigned int attempt = 1; priorities[z] == 0; attempt++) {
            switch (attempt) {
            // !FAKE: inliner; hand form of the retail-inlined AABox::Contains(_camPos); our /O1 build keeps it out of line (C0125)
            case 1: {
                const AABox& b = v0->GetBounds();
                if (_camPos.x >= b.min.x && _camPos.y >= b.min.y && _camPos.z >= b.min.z &&
                    _camPos.x <= b.max.x && _camPos.y <= b.max.y && _camPos.z <= b.max.z)
                    priorities[z] = 1;
                break;
            }
            case 2:
                if (v0->GetZoneData()->usesExtraChunks) {
                    const AABox& b = v0->GetZoneData()->GetExtraRoot().boundsChildren;
                    if (_camPos.x >= b.min.x && _camPos.y >= b.min.y && _camPos.z >= b.min.z &&
                        _camPos.x <= b.max.x && _camPos.y <= b.max.y && _camPos.z <= b.max.z)
                        priorities[z] = 2;
                }
                break;
            case 3: {
                const AABox& b = v0->GetBounds();
                if (_viewFrustum.IsVisible(b, 0.0f))
                    priorities[z] = 3;
                break;
            }
            case 4:
                if (v0->GetZoneData()->usesExtraChunks) {
                    const AABox& b = v0->GetZoneData()->GetExtraRoot().boundsChildren;
                    if (_viewFrustum.IsVisible(b, 0.0f))
                        priorities[z] = 4;
                }
                break;
            default:
                priorities[z] = -1;
                break;
            }
        }
        if (priorities[z] > 0 && priorities[z] < bestPriority)
            bestPriority = priorities[z];
    }
    for (unsigned int z = 0; z < 16; z++) {
        if (!_zones[z])
            break;
        if (priorities[z] == 3 && bestPriority != 3) {
            _zoneVisibility[z] = 0;
            continue;
        }
        _zoneVisibility[z] = 0xFFFFFFFF;
        if (priorities[z] == bestPriority) {
            // !FAKE: c2 register allocation; stand-in local; retail holds _zones[z] in a register (spilled across calls)
        ZoneNode* v0 = _zones[z];
            v0->RenderYourself(_viewFrustum, _camPos, false);
            ZoneRenderInfo zri;
            zri.node = v0;
            zri.frustum = _viewFrustum;
            _zonesRendered.push_back(zri);
        }
    }
    QueuePortalChecks();
}

// FUNCTION: USA 0x000C50BA
// PROTOTYPE: PREVIEW 0x004B037B identical
// NAME: proto-pdb:default.pdb
bool DrawLevel::DrawPendingStuff(std::vector<ZoneRenderInfo>& zr)
{
    SwampRenderer& renderer = g_swamp->GetRenderer();
    for (unsigned int z = 0; z < 16; z++) {
        // !FAKE: c2 register allocation; stand-in local; retail holds _zones[z] in a register (spilled across calls)
        ZoneNode* v0 = _zones[z];
        if (!v0)
            break;
        if (_zoneVisibility[z] != 0xFFFFFFFF && _zoneTested[z]) {
            const FloodZoneFile* zd = v0->GetZoneData();
            ZoneRenderInfo zri;
            zri.node = v0;
            if (zd->numPortals == 0) {
                bool bVisible = false;
                int res;
                // !FAKE: c2 addressing; stand-in root pointer; retail forms &GetRootChunk() in a register before reading status
                const FloodChunk* root;
                while ((res = renderer.GetRasterTestResults((root = &zd->GetRootChunk())->status)) != 0) {
                    if (res > 0) {
                        bVisible = true;
                        break;
                    }
                    Sleep(0);
                }
                _zoneVisibility[z] = 0xFFFFFFFF;
                if (!bVisible)
                    continue;
                zri.frustum = _viewFrustum;
            } else {
                Frustum portalFrustum;
                portalFrustum.CreateEmptyInverse(_viewFrustum);
                bool bVisible = false;
                for (unsigned int i = 0; i < zd->numPortals && !bVisible; i++) {
                    if (_zoneVisibility[z] & (1 << i)) {
                        FloodChunk& portal = zd->GetPortals()[i];
                        int res;
                        while ((res = renderer.GetRasterTestResults(portal.status)) != 0) {
                            if (res > 0) {
                                bVisible = true;
                                Vector3 pts[8];
                                portal.boundsChunk.GeneratePoints(pts);
                                portalFrustum.TrimFrustum(_viewFrustum, _camPos, pts, 8);
                                break;
                            }
                            Sleep(0);
                        }
                    }
                }
                _zoneVisibility[z] = 0xFFFFFFFF;
                if (!bVisible)
                    continue;
                zri.frustum = portalFrustum;
            }
            v0->RenderYourself(zri.frustum, _camPos, false);
            zr.push_back(zri);
            _zonesRendered.push_back(zri);
        }
    }
    return QueuePortalChecks();
}

// FUNCTION: USA 0x000C5257
// PROTOTYPE: PREVIEW 0x004B0518 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.87, bytes mismatch first diff +0x6D (T12.c2: portal-offset/bit slot at -8 and child at -4; retail (PDB) bit -4, front/back -8; if-decl front/back keeps frame 0x28;
// T12.c2c: function-scope `child` before the bit loop, same 99.87 +0x6D)
void ZoneNode::Init()
{
    std::map<unsigned int, unsigned int> materialMap;
    // !FAKE: c2 register allocation; stand-in locals; retail loads both vertex bases once and keeps them in ebx/edi across the calls
    // !FAKE: T12.c2 register allocation; stand-in ref read before the vertex bases; retail holds _zoneData in ecx for the first call
    FloodChunk& root = _zoneData->GetRootChunk();
    const FloodVertex* v0 = _world->_vertices;
    const FloodAuxVertex* v1 = _world->_auxVertices;
    RegisterChunkBuffers(root, v0, v1, materialMap, 0);
    if (_zoneData->usesExtraChunks)
        RegisterChunkBuffers(_zoneData->GetExtraRoot(), v0, v1, materialMap, 0x10000);
    for (unsigned int i = 0; i < _zoneData->numPortals; i++)
        _zoneData->GetPortals()[i].vertexBuffer.Register((void*)v0);
    for (std::vector<Bog*>::iterator bit = _bogs.begin(); bit != _bogs.end(); ++bit) {
        std::vector<BogNode*> nodes;
        // !FAKE: T12.c2 stack slot packing; one reused stand-in local; retail keeps the root, front and back push_back temps in one slot
        BogNode* child = (*bit)->GetRoot();
        nodes.push_back(child);
        for (unsigned int i = 0; i < nodes.size(); i++) {
            // !FAKE: c2 register allocation; stand-in local; retail reads nodes[i] once and keeps it in edi across the push_back
            BogNode* v2 = nodes[i];
            child = v2->GetFrontChild();
            if (child)
                nodes.push_back(child);
            child = v2->GetBackChild();
            if (child)
                nodes.push_back(child);
        }
        for (std::vector<BogNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
            RemapBogMaterial(*it, materialMap);
    }
    std::sort(_chunkList.begin(), _chunkList.end(), ChunkMaterialSorter);
}
#endif

// FUNCTION: USA 0x000C54D0
// PROTOTYPE: PREVIEW 0x004B0791 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 87.05, bytes mismatch (T12.c2c: zoneNodes in the else-if scope, slot 16 (retail 8); frame 0xc8 vs 0xc4;
// world kept in edi early where retail spills it and holds 1 in esi; vertices/zone hrd at -24, VertsAux 68, retail all 0x44)
// retail 0x000BDC97 (called out of line, home unit not this one, as SwampLoader.cpp); the visible body gives cl
// its summary, so the by-value params stay unescaped across the call (Ptr dtors `dec [ecx+4]; jne` as retail)
// NAME: proto-pdb:default.pdb
template <class T> Ptr<T>& Ptr<T>::operator=(const Ptr<T>& p)
{
    if (_Object)
        _Object->SubRef();
    _Object = p._Object;
    if (_Object)
        _Object->AddRef();
    return *this;
}

BeepTDF* Swamp_LoadWorld(const std::string& name, Ptr<BundleWrapper> dataBundle, Ptr<BundleWrapper> vertexBundle)
{
    WorldData* world;
    const HottieBundle* bundle;
    const BeepTDF* zones;
    const BeepTDF* materials;
    const BeepTDF* fog;
    const BeepTDF* lights;
    const HottieBundle* vertsbundle;
    BeepTDF* indexInfo;
    BeepFile* indexFile;
    BeepLib::GetTime();
    // !FAKE: retail string literal; __FILE__/__LINE__ text; retail pushes "SwampWorld.cpp" and the line to operator new
    world = new("SwampWorld.cpp", 53) WorldData;
    world->_dataBundle = dataBundle;
    world->_vertexBundle = vertexBundle;
    bundle = 0;
    if (dataBundle.IsValid())
        bundle = dataBundle->GetPtr();
    vertsbundle = 0;
    if (vertexBundle.IsValid())
        vertsbundle = vertexBundle->GetPtr();
    if (bundle) {
    indexFile = bundle->OpenBundledFile("Index.tdf");
    if (indexFile) {
    // !FAKE: retail string literal; __FILE__/__LINE__ text; retail pushes "SwampWorld.cpp" and the line to operator new
    indexInfo = new("SwampWorld.cpp", 87) BeepTDF(*indexFile, "Index.tdf");
    bundle->CloseBundledFile(indexFile);
    if (indexInfo) {
    if (!indexInfo->IsValid() || indexInfo->GetBlocks().size() == 0) {
        delete indexInfo;
    } else if (vertsbundle) {
    {
        HottieRawData hrd = vertsbundle->GetRawFileData("vertices.raw");
        world->_vertices = (FloodVertex*)hrd.data;
    }
    {
        HottieRawData hrd = vertsbundle->GetRawFileData("VertsAux.raw");
        if (hrd.size)
            world->_auxVertices = (FloodAuxVertex*)hrd.data;
    }
    materials = indexInfo->FindBlock("Materials", false);
    for (std::vector<BeepTDF*>::const_iterator mit = materials->GetBlocks().begin(); mit != materials->GetBlocks().end(); ++mit)
        g_swamp->GetMaterialManager().CreateMaterial(*mit);
    std::map<unsigned int, ZoneNode*> zoneNodes;
    zones = indexInfo->FindBlock("Zones", false);
    for (std::vector<BeepTDF::Record*>::const_iterator rit = zones->GetRecords().begin(); rit != zones->GetRecords().end(); ++rit) {
        ZoneNode* node;
        BOG_COLLECTION* collection;
        unsigned int zoneID;
        zoneID = (*rit)->intdata;
        std::string zoneBaseName = format("Zone%04i", zoneID);
        std::string zonename = zoneBaseName + ".fld";
        std::string bogname = zoneBaseName + ".bog";
        HottieRawData hrd = bundle->GetRawFileData(zonename);
        if (((FloodZoneFile*)hrd.data)->id != 0x44554C46 || ((FloodZoneFile*)hrd.data)->version != 0x12)
            goto fail;
        collection = 0;
        if (((FloodZoneFile*)hrd.data)->containsBogs) {
            HottieRawData hrd = vertsbundle->GetRawFileData(bogname);
            collection = (BOG_COLLECTION*)hrd.data;
            if (collection->magic != 0x43474F42 || collection->version != 1)
                goto fail;
        }
        // !FAKE: retail string literal; __FILE__/__LINE__ text; retail pushes "SwampWorld.cpp" and the line to operator new
        node = new("SwampWorld.cpp", 226) ZoneNode(zoneID, (FloodZoneFile*)hrd.data, collection, world);
        node->SetName((*rit)->name);
        zoneNodes.insert(std::map<unsigned int, ZoneNode*>::value_type(zoneID, node));
    }
    BeepLib::GetTime();
    {
        HottieRawData hrd = bundle->GetRawFileData("Collision.bsp");
        // !FAKE: retail string literal; __FILE__/__LINE__ text; retail pushes "SwampWorld.cpp" and the line to operator new
        g_swamp->SetGlobalBSP(new("SwampWorld.cpp", 246) BSP(hrd.data, false));
    }
    lights = indexInfo->FindBlock("Lights", true);
    if (lights) {
        for (std::vector<BeepTDF*>::const_iterator lit = lights->GetBlocks().begin(); lit != lights->GetBlocks().end(); ++lit) {
            LightNode* light = LightManager::CreateDefinedLight(*lit);
            if (light->NeedsToBeInTheScene()) {
                unsigned int z = (*lit)->GetIntRecord("Zone", 0);
                std::map<unsigned int, ZoneNode*>::iterator zmi = zoneNodes.find(z);
                zmi->second->AddChild(light);
            } else {
                world->_staticLights.push_back(light);
                light->UpdateCachedWorldPosition();
            }
        }
        LightManager::CreateShadowBuffers();
    } else {
        g_swamp->GetScene().AddNode(LightManager::CreateDirectionalLight(Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f)), 0);
        g_swamp->GetScene().AddNode(LightManager::CreateDirectionalLight(Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.2f, 0.2f, 0.2f)), 0);
    }
    fog = indexInfo->FindBlock("Fog", true);
    if (fog) {
        for (std::vector<BeepTDF*>::const_iterator fit = fog->GetBlocks().begin(); fit != fog->GetBlocks().end(); ++fit) {
            // !FAKE: retail string literal; __FILE__/__LINE__ text; retail pushes "SwampWorld.cpp" and the line to operator new
            unsigned int z;
            FogZone* f = new("SwampWorld.cpp", 308) FogZone(*fit);
            z = (*fit)->GetIntRecord("Zone", 0);
            std::map<unsigned int, ZoneNode*>::iterator zmi = zoneNodes.find(z);
            zmi->second->AddChild(f);
        }
    }
    for (std::map<unsigned int, ZoneNode*>::iterator zit = zoneNodes.begin(); zit != zoneNodes.end(); ++zit)
        g_swamp->GetScene().GetRootNode()->AddChild(zit->second);
    return indexInfo;
    }
    }
    }
    }
fail:
    delete world;
    return 0;
}
#endif
