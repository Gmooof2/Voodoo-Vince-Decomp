// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepState.cpp OBJ: Xbox\Official_Release\BeepState.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator {}; // empty, as VC 7.1 (layout 1 byte); _Ufill's fill path selects on allocator<_Ty>&
struct _Scalar_ptr_iterator_tag { // NAME: proto-pdb:default.pdb
};
inline _Scalar_ptr_iterator_tag _Ptr_cat(double *, double *)
	{	// return pointer category from pointer to double arguments
	_Scalar_ptr_iterator_tag _Cat;
	return (_Cat);
	}
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
	typename iterator_traits<_Iter>::iterator_category
		_Iter_cat(const _Iter&)
	{	// return category from iterator argument
	typename iterator_traits<_Iter>::iterator_category _Cat;
	return (_Cat);
	}
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
template<class _Ty1,
	class _Ty2> struct pair
	{	// store a pair of values
	pair(const _Ty1& _Val1, const _Ty2& _Val2)
		: first(_Val1), second(_Val2)
		{	// construct from specified values
		}

	_Ty1 first;	// the first stored value
	_Ty2 second;	// the second stored value
	};
template<class _OutIt,
	class _Diff,
	class _Ty> inline
	void fill_n(_OutIt _First, _Diff _Count, const _Ty& _Val)
	{	// copy _Val _Count times through [_First, ...); retail calls it out of line (0x00035ED3)
	for (; 0 < _Count; --_Count, ++_First)
		*_First = _Val;
	}
template<class _Ty,
	class _Diff,
	class _Tval> inline
	void _Uninit_fill_n(_Ty *_First, _Diff _Count,
		const _Tval& _Val, allocator<_Ty>&, _Scalar_ptr_iterator_tag)
	{	// copy _Count *_Val to raw _First, using _Al, scalar type
	fill_n(_First, _Count, _Val);
	}
template<class _FwdIt,
	class _Diff,
	class _Tval,
	class _Alloc> inline
	void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count,
		const _Tval& _Val, _Alloc& _Al)
	{	// copy _Count *_Val to raw _First, using _Al
	_Uninit_fill_n(_First, _Count, _Val, _Al,
		_Ptr_cat(_First, _First));
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

		const_iterator& operator--()
			{	// predecrement
			--_Myptr;
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

		_Tptr operator->() const
			{	// return pointer to class object
			return (&**this);
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
    iterator begin() { return iterator(_Myfirst); }
    iterator end() { return iterator(_Mylast); }
    unsigned int size() const { return _Myfirst == 0 ? 0 : _Mylast - _Myfirst; }
    unsigned int capacity() const { return _Myfirst == 0 ? 0 : _Myend - _Myfirst; }
    T& operator[](unsigned int _Pos) { return _Myfirst[_Pos]; }
    void push_back(const T& val);

protected:
    void _Tidy();
	T *_Ufill(T *_Ptr, unsigned int _Count, const T &_Val)
		{	// copy initializing _Count * _Val, using allocator
		_Uninitialized_fill_n(_Ptr, _Count, _Val, this->_Alval);
		return (_Ptr + _Count);
		}

private:
    A _Alval;                          // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
const int _ISORT_MAX = 32;	// maximum size for insertion sort
template<class _FwdIt1,
	class _FwdIt2> inline
	void iter_swap(_FwdIt1 _Left, _FwdIt2 _Right)
	{	// swap *_Left and *_Right
	std::swap(*_Left, *_Right);
	}
template<class _RanIt,
	class _Diff,
	class _Ty> inline
	void _Rotate(_RanIt _First, _RanIt _Mid, _RanIt _Last, _Diff *, _Ty *)
	{	// rotate [_First, _Last), random-access iterators
	_Diff _Shift = _Mid - _First;
	_Diff _Count = _Last - _First;

	for (_Diff _Factor = _Shift; _Factor != 0; )
		{	// find subcycle count as GCD of shift count and length
		_Diff _Tmp = _Count % _Factor;
		_Count = _Factor, _Factor = _Tmp;
		}

	if (_Count < _Last - _First)
		for (; 0 < _Count; --_Count)
			{	// rotate each subcycle
			_RanIt _Hole = _First + _Count;
			_RanIt _Next = _Hole;
			_Ty _Holeval = *_Hole;
			_RanIt _Next1 = _Next + _Shift == _Last ? _First : _Next + _Shift;
			while (_Next1 != _Hole)
				{	// percolate elements back around subcycle
				*_Next = *_Next1;
				_Next = _Next1;
				_Next1 = _Shift < _Last - _Next1 ? _Next1 + _Shift
					: _First + (_Shift - (_Last - _Next1));
				}
			*_Next = _Holeval;
			}
	}
template<class _RanIt> inline
	void _Rotate(_RanIt _First, _RanIt _Mid, _RanIt _Last,
			random_access_iterator_tag)
	{	// rotate [_First, _Last), random-access iterators
	_Rotate(_First, _Mid, _Last, _Dist_type(_First), _Val_type(_First));
	}
template<class _FwdIt> inline
	void rotate(_FwdIt _First, _FwdIt _Mid, _FwdIt _Last)
	{	// rotate [_First, _Last)
	if (_First != _Mid && _Mid != _Last)
		_Rotate(_First, _Mid, _Last, _Iter_cat(_First));
	}
template<class _RanIt,
	class _Diff,
	class _Ty> inline
	void _Push_heap(_RanIt _First, _Diff _Hole,
		_Diff _Top, _Ty _Val)
	{	// percolate _Hole to _Top or where _Val belongs, using operator<
	for (_Diff _Idx = (_Hole - 1) / 2;
		_Top < _Hole && *(_First + _Idx) < _Val;
		_Idx = (_Hole - 1) / 2)
		{	// move _Hole up to parent
		*(_First + _Hole) = *(_First + _Idx);
		_Hole = _Idx;
		}

	*(_First + _Hole) = _Val;	// drop _Val into final hole
	}
template<class _RanIt,
	class _Diff,
	class _Ty> inline
	void _Adjust_heap(_RanIt _First, _Diff _Hole, _Diff _Bottom, _Ty _Val)
	{	// percolate _Hole to _Bottom, then push _Val, using operator<
	_Diff _Top = _Hole;
	_Diff _Idx = 2 * _Hole + 2;

	for (; _Idx < _Bottom; _Idx = 2 * _Idx + 2)
		{	// move _Hole down to larger child
		if (*(_First + _Idx) < *(_First + (_Idx - 1)))
			--_Idx;
		*(_First + _Hole) = *(_First + _Idx), _Hole = _Idx;
		}

	if (_Idx == _Bottom)
		{	// only child at bottom, move _Hole down to it
		*(_First + _Hole) = *(_First + (_Bottom - 1));
		_Hole = _Bottom - 1;
		}
	_Push_heap(_First, _Hole, _Top, _Val);
	}
template<class _RanIt,
	class _Diff,
	class _Ty> inline
	void _Pop_heap(_RanIt _First, _RanIt _Last, _RanIt _Dest,
		_Ty _Val, _Diff *)
	{	// pop *_First to *_Dest and reheap, using operator<
	*_Dest = *_First;
	_Adjust_heap(_First, _Diff(0), _Diff(_Last - _First), _Val);
	}
template<class _RanIt,
	class _Ty> inline
	void _Pop_heap_0(_RanIt _First, _RanIt _Last, _Ty *)
	{	// pop *_First to *(_Last - 1) and reheap, using operator<
	_Pop_heap(_First, _Last - 1, _Last - 1,
		_Ty(*(_Last - 1)), _Dist_type(_First));
	}
template<class _RanIt> inline
	void pop_heap(_RanIt _First, _RanIt _Last)
	{	// pop *_First to *(_Last - 1) and reheap, using operator<
	if (1 < _Last - _First)
		_Pop_heap_0(_First, _Last, _Val_type(_First));
	}
template<class _RanIt,
	class _Diff,
	class _Ty> inline
	void _Make_heap(_RanIt _First, _RanIt _Last, _Diff *, _Ty *)
	{	// make nontrivial [_First, _Last) into a heap, using operator<
	_Diff _Bottom = _Last - _First;

	for (_Diff _Hole = _Bottom / 2; 0 < _Hole; )
		{	// reheap top half, bottom to top
		--_Hole;
		_Adjust_heap(_First, _Hole, _Bottom, _Ty(*(_First + _Hole)));
		}
	}
template<class _RanIt> inline
	void make_heap(_RanIt _First, _RanIt _Last)
	{	// make [_First, _Last) into a heap, using operator<
	if (1 < _Last - _First)
		_Make_heap(_First, _Last,
			_Dist_type(_First), _Val_type(_First));
	}
template<class _RanIt> inline
	void sort_heap(_RanIt _First, _RanIt _Last)
	{	// order heap by repeatedly popping, using operator<
	for (; 1 < _Last - _First; --_Last)
		std::pop_heap(_First, _Last);
	}
template<class _BidIt> inline
	void _Insertion_sort(_BidIt _First, _BidIt _Last)
	{	// insertion sort [_First, _Last), using operator<
	if (_First != _Last)
		for (_BidIt _Next = _First; ++_Next != _Last; )
			if (*_Next < *_First)
				{	// found new earliest element, rotate to front
				_BidIt _Next1 = _Next;
				std::rotate(_First, _Next, ++_Next1);
				}
			else
				{	// look for insertion point after first
				_BidIt _Dest = _Next;
				for (_BidIt _Dest0 = _Dest; *_Next < *--_Dest0; )
					_Dest = _Dest0;
				if (_Dest != _Next)
					{	// rotate into place
					_BidIt _Next1 = _Next;
					std::rotate(_Dest, _Next, ++_Next1);
					}
				}
	}
template<class _RanIt> inline
	void _Med3(_RanIt _First, _RanIt _Mid, _RanIt _Last)
	{	// sort median of three elements to middle
	if (*_Mid < *_First)
		std::iter_swap(_Mid, _First);
	if (*_Last < *_Mid)
		std::iter_swap(_Last, _Mid);
	if (*_Mid < *_First)
		std::iter_swap(_Mid, _First);
	}
template<class _RanIt> inline
	void _Median(_RanIt _First, _RanIt _Mid, _RanIt _Last)
	{	// sort median element to middle
	if (40 < _Last - _First)
		{	// median of nine
		int _Step = (_Last - _First + 1) / 8;
		_Med3(_First, _First + _Step, _First + 2 * _Step);
		_Med3(_Mid - _Step, _Mid, _Mid + _Step);
		_Med3(_Last - 2 * _Step, _Last - _Step, _Last);
		_Med3(_First + _Step, _Mid, _Last - _Step);
		}
	else
		_Med3(_First, _Mid, _Last);
	}
template<class _RanIt> inline
	pair<_RanIt, _RanIt> _Unguarded_partition(_RanIt _First, _RanIt _Last)
	{	// partition [_First, _Last), using operator<
	_RanIt _Mid = _First + (_Last - _First) / 2;	// sort median to _Mid
	_Median(_First, _Mid, _Last - 1);
	_RanIt _Pfirst = _Mid;
	_RanIt _Plast = _Pfirst + 1;

	while (_First < _Pfirst
		&& !(*(_Pfirst - 1) < *_Pfirst)
		&& !(*_Pfirst < *(_Pfirst - 1)))
		--_Pfirst;
	while (_Plast < _Last
		&& !(*_Plast < *_Pfirst)
		&& !(*_Pfirst < *_Plast))
		++_Plast;

	_RanIt _Gfirst = _Plast;
	_RanIt _Glast = _Pfirst;

	for (; ; )
		{	// partition
		for (; _Gfirst < _Last; ++_Gfirst)
			if (*_Pfirst < *_Gfirst)
				;
			else if (*_Gfirst < *_Pfirst)
				break;
			else
				std::iter_swap(_Plast++, _Gfirst);
		for (; _First < _Glast; --_Glast)
			if (*(_Glast - 1) < *_Pfirst)
				;
			else if (*_Pfirst < *(_Glast - 1))
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
	class _Diff> inline
	void _Sort(_RanIt _First, _RanIt _Last, _Diff _Ideal)
	{	// order [_First, _Last), using operator<
	_Diff _Count;
	for (; _ISORT_MAX < (_Count = _Last - _First) && 0 < _Ideal; )
		{	// divide and conquer by quicksort
		pair<_RanIt, _RanIt> _Mid = _Unguarded_partition(_First, _Last);
		_Ideal /= 2, _Ideal += _Ideal / 2;	// allow 1.5 log2(N) divisions

		if (_Mid.first - _First < _Last - _Mid.second)	// loop on larger half
			_Sort(_First, _Mid.first, _Ideal), _First = _Mid.second;
		else
			_Sort(_Mid.second, _Last, _Ideal), _Last = _Mid.first;
		}

	if (_ISORT_MAX < _Count)
		{	// heap sort if too many divisions
		std::make_heap(_First, _Last);
		std::sort_heap(_First, _Last);
		}
	else if (1 < _Count)
		_Insertion_sort(_First, _Last);	// small, insertion sort
	}
template<class _RanIt> inline
	void sort(_RanIt _First, _RanIt _Last)
	{	// order [_First, _Last), using operator<
	_Sort(_First, _Last, _Last - _First);
	}
#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0132); retail calls rotate out of line (pin 0x00036475), and its
// header body would emit rotate and the unnamed _Rotate(..., random_access_iterator_tag) it calls
template<> void rotate(vector<double>::iterator, vector<double>::iterator, vector<double>::iterator);
#endif
}

union _LARGE_INTEGER { // NAME: proto-pdb:default.pdb
    __int64 QuadPart;                  // 0x00
};
DECOMP_SIZE_ASSERT(_LARGE_INTEGER, 0x8);

extern "C" void __stdcall Sleep(unsigned long); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall QueryPerformanceCounter(_LARGE_INTEGER*); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall QueryPerformanceFrequency(_LARGE_INTEGER*); // NAME: proto-pdb:default.pdb

class FrequencyCalculator { // NAME: proto-pdb:default.pdb
public:
    struct Sample { // NAME: proto-pdb:default.pdb
        _LARGE_INTEGER performance_timer; // 0x00
        unsigned __int64 tsc;          // 0x08
    };

    FrequencyCalculator();
    Sample TakeSample();
    double Finalize();

    std::vector<Sample> _Samples;      // 0x00
};
DECOMP_SIZE_ASSERT(FrequencyCalculator, 0x10);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    ~BeepTDF();

    class Record { // NAME: proto-pdb:default.pdb
    public:
        ~Record();
    };
};

class FileSystem { // NAME: proto-pdb:default.pdb
public:
    ~FileSystem();
};

class QuickRand { // NAME: proto-pdb:default.pdb
    char field_0[0x8];                 // 0x00
};

class BeepState { // NAME: proto-pdb:default.pdb
public:
    ~BeepState();
    unsigned __int64 GetTSC();

private:
    class DebugState { // NAME: proto-pdb:default.pdb
    public:
        ~DebugState();

        char field_0[0x14];            // 0x00
    };

    class TDFState { // NAME: proto-pdb:default.pdb
    public:
        BeepTDF* _DefaultBlock;        // 0x00
        BeepTDF::Record* _DefaultRecord; // 0x04
    };

    void CheckForMMX();
    void CheckForRDTSC();
    void CheckForSSE();

    DebugState _DebugState;            // 0x00
    TDFState _TDFState;                // 0x14
    FileSystem* _FileSystem;           // 0x1C
    char field_20[0x1C];               // 0x20
    bool _MMXAvailable;                // 0x3C
    bool _RDTSCAvailable;              // 0x3D
    bool _SSEAvailable;                // 0x3E
    QuickRand* _RNG;                   // 0x40
    char field_44[0xC14];              // 0x44
};
DECOMP_SIZE_ASSERT(BeepState, 0xC58);

extern BeepState beepState; // NAME: proto-pdb:default.pdb

// the fold rows 0x00035E68/0x00035E7B carry this alias's name (symbols.txt); its element type only
class TextureResource; // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
    T* _Resource;                      // 0x00
};

// FUNCTION: USA 0x00035E44
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; body is __asm rdtsc (verbatim, never a match; no rdtsc intrinsic in MSVC 7.1)
unsigned __int64 BeepState::GetTSC()
{
    unsigned __int64 tsc;
    __asm {
        rdtsc
        mov dword ptr [tsc], eax
        mov dword ptr [tsc + 4], edx
    }
    return tsc;
}
#endif

// FUNCTION: USA 0x00035E59
// PROTOTYPE: PREVIEW 0x004245C4 identical
// NAME: proto-pdb:default.pdb
void BeepState::CheckForMMX()
{
    _MMXAvailable = true;
}

// FUNCTION: USA 0x00035E5E
// PROTOTYPE: PREVIEW 0x004245C9 identical
// NAME: proto-pdb:default.pdb
void BeepState::CheckForRDTSC()
{
    _RDTSCAvailable = true;
}

// FUNCTION: USA 0x00035E63
// PROTOTYPE: PREVIEW 0x004245CE identical
// NAME: proto-pdb:default.pdb
void BeepState::CheckForSSE()
{
    _SSEAvailable = true;
}

// FUNCTION: USA 0x00035E68
// PROTOTYPE: PREVIEW 0x0045A14F identical
// NAME: proto-pdb:default.pdb
// fold VA: the FFlyCtrlData::Key and FrequencyCalculator::Sample aliases share these bytes (C0233)
template unsigned int std::vector<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > >,std::allocator<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > > >::capacity()const;

// FUNCTION: USA 0x00035E7B
// PROTOTYPE: PREVIEW 0x0045A13C identical
// NAME: proto-pdb:default.pdb
// fold VA: the FFlyCtrlData::Key and FrequencyCalculator::Sample aliases share these bytes (C0233)
template unsigned int std::vector<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > >,std::allocator<std::vector<ResourceHandle<TextureResource>,std::allocator<ResourceHandle<TextureResource> > > > >::size()const;

// FUNCTION: USA 0x00035ED3
// PROTOTYPE: PREVIEW 0x004245F6 identical
// NAME: proto-pdb:default.pdb
template void std::fill_n<double *,unsigned int,double>(double *,unsigned int,double const &);

// FUNCTION: USA 0x00035F14
// PROTOTYPE: PREVIEW 0x00424637 identical
// NAME: proto-pdb:default.pdb
FrequencyCalculator::Sample FrequencyCalculator::TakeSample()
{
    Sample s;
    Sleep(0);
    QueryPerformanceCounter(&s.performance_timer);
    s.tsc = beepState.GetTSC();
    return s;
}

// FUNCTION: USA 0x00035FB4
// PROTOTYPE: PREVIEW 0x004246C2 identical
// NAME: proto-pdb:default.pdb
template void std::_Med3<std::vector<double,std::allocator<double> >::iterator>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator);

// FUNCTION: USA 0x00035FFC
// PROTOTYPE: PREVIEW 0x0042470A identical
// NAME: proto-pdb:default.pdb
template void std::_Push_heap<std::vector<double,std::allocator<double> >::iterator,int,double>(std::vector<double,std::allocator<double> >::iterator,int,int,double);

// FUNCTION: USA 0x00036042
// PROTOTYPE: PREVIEW 0x00424750 identical
// NAME: proto-pdb:default.pdb
template void std::_Rotate<std::vector<double,std::allocator<double> >::iterator,int,double>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator,int *,double *);

// FUNCTION: USA 0x00036145
// PROTOTYPE: PREVIEW 0x00424853 identical
// NAME: proto-pdb:default.pdb
template void std::_Median<std::vector<double,std::allocator<double> >::iterator>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator);

// FUNCTION: USA 0x000361CC
// PROTOTYPE: PREVIEW 0x004248DA identical
// NAME: proto-pdb:default.pdb
template void std::_Adjust_heap<std::vector<double,std::allocator<double> >::iterator,int,double>(std::vector<double,std::allocator<double> >::iterator,int,int,double);

// FUNCTION: USA 0x00036243
// PROTOTYPE: PREVIEW 0x00424951 identical
// NAME: proto-pdb:default.pdb
template void std::_Pop_heap<std::vector<double,std::allocator<double> >::iterator,int,double>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator,double,int *);

// FUNCTION: USA 0x0003626E
// PROTOTYPE: PREVIEW 0x0042497C identical
// NAME: proto-pdb:default.pdb
template double * std::vector<double,std::allocator<double> >::_Ufill(double *,unsigned int,double const &);

// FUNCTION: USA 0x000362F9
// PROTOTYPE: PREVIEW 0x00424A07 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator> std::_Unguarded_partition<std::vector<double,std::allocator<double> >::iterator>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator);

// FUNCTION: USA 0x0003643E
// PROTOTYPE: PREVIEW 0x00424B4C identical
// NAME: proto-pdb:default.pdb
template void std::_Make_heap<std::vector<double,std::allocator<double> >::iterator,int,double>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator,int *,double *);

// FUNCTION: USA 0x00036499
// PROTOTYPE: PREVIEW 0x00424BA7 identical
// NAME: proto-pdb:default.pdb
template void std::_Pop_heap_0<std::vector<double,std::allocator<double> >::iterator,double>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator,double *);

// FUNCTION: USA 0x000364ED
// PROTOTYPE: PREVIEW 0x00424BFB identical
// NAME: proto-pdb:default.pdb
template void std::make_heap<std::vector<double,std::allocator<double> >::iterator>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator);

// FUNCTION: USA 0x00036512
// PROTOTYPE: PREVIEW 0x00424C20 identical
// NAME: proto-pdb:default.pdb
template void std::_Insertion_sort<std::vector<double,std::allocator<double> >::iterator>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator);

// FUNCTION: USA 0x00036572
// PROTOTYPE: PREVIEW 0x00424C80 identical
// NAME: proto-pdb:default.pdb
template void std::pop_heap<std::vector<double,std::allocator<double> >::iterator>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator);

// FUNCTION: USA 0x000365B7
// PROTOTYPE: PREVIEW 0x00424CC5 identical
// NAME: proto-pdb:default.pdb
template void std::sort_heap<std::vector<double,std::allocator<double> >::iterator>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator);

// FUNCTION: USA 0x000365DD
// NAME: proto-pdb:default.pdb
BeepState::~BeepState()
{
    delete _TDFState._DefaultRecord;
    delete _TDFState._DefaultBlock;
    delete _RNG;
    delete _FileSystem;
}
// FUNCTION: USA 0x00036632
// PROTOTYPE: PREVIEW 0x00424D40 identical
// NAME: proto-pdb:default.pdb
template void std::_Sort<std::vector<double,std::allocator<double> >::iterator,int>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator,int);

// FUNCTION: USA 0x000366DA
// PROTOTYPE: PREVIEW 0x00424DE8 identical
// NAME: proto-pdb:default.pdb
template void std::sort<std::vector<double,std::allocator<double> >::iterator>(std::vector<double,std::allocator<double> >::iterator,std::vector<double,std::allocator<double> >::iterator);

// FUNCTION: USA 0x00036B75
// NAME: proto-pdb:default.pdb
FrequencyCalculator::FrequencyCalculator()
{
    for (int i = 0; i < 64; i++)
        _Samples.push_back(TakeSample());
}

// FUNCTION: USA 0x00036BA9
// PROTOTYPE: PREVIEW 0x004252B7 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 88.51, bytes mismatch +0x6 (T12.c23 at-stop 20): retail keeps frequencies' _Myfirst/_Mylast in
// ebx/esi across the out-of-line sort (cl 13.10 reloads members of any local passed as `this`); iterator stand-ins first/last
// keep them in registers but cl merges the repeated size() null tests and puts first in edi, zero stores stay `and [m],0`;
// retail pretests the sample loop; VC 7.1 operator[] body form 82.52; lever C (T12.c27, visible push_back/_Tidy): identical
double FrequencyCalculator::Finalize()
{
    std::vector<double> frequencies;
    _LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    double performance_frequency = (double)freq.QuadPart;
    for (std::vector<Sample>::iterator it = _Samples.begin(); it != _Samples.end(); ++it) {
        Sample s = TakeSample();
        s.tsc -= it->tsc;
        s.performance_timer.QuadPart -= it->performance_timer.QuadPart;
        double frequency = (double)s.tsc * performance_frequency / (double)s.performance_timer.QuadPart;
        frequencies.push_back(frequency);
    }
    // !FAKE: regalloc; iterator stand-ins first/last and hand forms of size() and operator[] keep the vector bounds in
    // registers across sort, as retail; lo (address of the lower median) sets the fadd operand order
    std::vector<double>::iterator first = frequencies.begin();
    std::vector<double>::iterator last = frequencies.end();
    std::sort(first, last);
    if ((first._Myptr == 0 ? 0 : (unsigned int)(last - first)) & 1)
        return *(first + (first._Myptr == 0 ? 0 : (unsigned int)(last - first)) / 2);
    const double* lo = &*(first + ((first._Myptr == 0 ? 0 : (unsigned int)(last - first)) / 2 - 1));
    return (*(first + (first._Myptr == 0 ? 0 : (unsigned int)(last - first)) / 2) + *lo) * 0.5;
}
#endif
