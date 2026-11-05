// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampBSP.cpp OBJ: Xbox\Official_Release\SwampBSP.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

extern "C" void* memset(void*, int, unsigned int);
// !FAKE: c2 intrinsic; per-file pragma; retail rep stosd header clear in BSP::BSP under /O1 /Oy (C4164 without the decl)
#pragma intrinsic(memset)

void* operator new(unsigned int);
inline void* operator new(unsigned int, void* _Where) { return _Where; }
void operator delete[](void*);
void BeepFree(void* p); // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    void Zero()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    Vector4() {}
    void Zero()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
    float w; // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v; // 0x00
    float w;   // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

    int _RefCount; // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    Ptr(T* p) : _Object(p)
    {
        if (_Object)
            _Object->AddRef();
    }

    T* _Object; // 0x00
};

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int _tag;        // 0x04
    float _pct;               // 0x08
    const void* _blindData;   // 0x0C
    Vector4 _plane;           // 0x10
    unsigned char _bone;      // 0x20
    bool _hitPlatform;        // 0x21
    Vector3 _platformMove;    // 0x24
    Quaternion _platformRot;  // 0x30

private:
    Ptr<BSPQueryResultObject> Reset();
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

struct BSP_HEADER { // NAME: proto-pdb:default.pdb
    unsigned int magic;        // 0x00
    unsigned int version;      // 0x04
    unsigned int totalSize;    // 0x08
    unsigned int numPlanes;    // 0x0C
    unsigned int numTriangles; // 0x10
    unsigned int numVertices;  // 0x14
    unsigned int numIndices;   // 0x18
    unsigned int numNodes;     // 0x1C
};
DECOMP_SIZE_ASSERT(BSP_HEADER, 0x20);

struct BSPTriangleInfo { // NAME: proto-pdb:default.pdb
    unsigned int pt[3];  // 0x00
    unsigned int plane;  // 0x0C
    unsigned int tag;    // 0x10
};
DECOMP_SIZE_ASSERT(BSPTriangleInfo, 0x14);

class BSPnode;

class BSP { // NAME: proto-pdb:default.pdb
public:
    BSP(void* fileImage, bool owns);
    ~BSP();
    Ptr<BSPQueryResultObject> CheckLine(const Vector3& start, const Vector3& end, unsigned int ignoreMask) const;
    Ptr<BSPQueryResultObject> CheckSphere(const Vector3& start, const Vector3& end, float radius, unsigned int ignoreMask) const;
    const BSPnode* GetRoot() const { return _nodes; }

    BSP_HEADER _header;                       // 0x00
    void* _data;                              // 0x20
    const Vector4* _planes;                   // 0x24
    const BSPTriangleInfo* _triangleList;     // 0x28
    const Vector3* _vertexList;               // 0x2C
    const unsigned int* _triangleIndices;     // 0x30
    const BSPnode* _nodes;                    // 0x34
    unsigned int* _triCheckedMarkers;         // 0x38
    unsigned int _currentMarker;              // 0x3C

protected:
    void CalculatePointers(const void* image);
};
DECOMP_SIZE_ASSERT(BSP, 0x40);

class BSPnode { // NAME: proto-pdb:default.pdb
public:
    struct CheckInfo { // NAME: proto-pdb:default.pdb
        CheckInfo(const BSP& bsp_, float radius_, unsigned int ignoreMask_, const Vector3& start, const Vector3& end);

        const BSP& bsp;           // 0x00
        float radius;             // 0x04
        unsigned int ignoreMask;  // 0x08
        const Vector3& origStart; // 0x0C
        const Vector3& origEnd;   // 0x10
    };

    Ptr<BSPQueryResultObject> CheckLine(const Vector3& start, const Vector3& end, const CheckInfo& info) const;
    Ptr<BSPQueryResultObject> CheckSphere(const Vector3& start, const Vector3& end, const CheckInfo& info) const;

    unsigned short _numTris;  // 0x00
    unsigned short _flags;    // 0x02
    unsigned int _offsetTris; // 0x04
    unsigned int _back;       // 0x08
};
DECOMP_SIZE_ASSERT(BSPnode, 0xC);
DECOMP_SIZE_ASSERT(BSPnode::CheckInfo, 0x14);

namespace std { // NAME: proto-pdb:default.pdb
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
template <class _Category, class _Ty, class _Diff, class _Pointer, class _Reference> struct iterator {
    typedef _Category iterator_category;
};
template <class _Ty, class _Diff, class _Pointer, class _Reference> struct _Ranit
    : public iterator<random_access_iterator_tag, _Ty, _Diff, _Pointer, _Reference> {
};
template <class _Ty> inline _Ty* _Allocate(unsigned int _Count, _Ty*)
{
    return ((_Ty*)operator new(_Count * sizeof(_Ty)));
}
template <class _T1, class _T2> inline void _Construct(_T1* _Ptr, const _T2& _Val)
{
    new ((void*)_Ptr) _T1(_Val);
}
template <class _Ty> class allocator {
public:
    typedef _Ty* pointer;
    pointer allocate(unsigned int _Count) { return (_Allocate(_Count, (pointer)0)); }
    void deallocate(pointer _Ptr, unsigned int) { operator delete(_Ptr); }
    void construct(pointer _Ptr, const _Ty& _Val) { _Construct(_Ptr, _Val); }
    unsigned int max_size() const
    {
        unsigned int _Count = (unsigned int)(-1) / sizeof(_Ty);
        return (0 < _Count ? _Count : 1);
    }
};
// !FAKE: inliner; decl-only callees; retail calls _Uninitialized_copy (0x0007F3FD) and fill_n (0x00033F35) out of line
template <class _InIt, class _FwdIt, class _Alloc> _FwdIt _Uninitialized_copy(_InIt _First, _InIt _Last, _FwdIt _Dest, _Alloc& _Al);
template <class _OutIt, class _Diff, class _Ty> void fill_n(_OutIt _First, _Diff _Count, const _Ty& _Val);
struct _Scalar_ptr_iterator_tag {};
template <class _Ty> inline _Scalar_ptr_iterator_tag _Ptr_cat(_Ty**, _Ty**)
{
    _Scalar_ptr_iterator_tag _Cat;
    return (_Cat);
}
template <class _Ty, class _Diff, class _Tval> inline void _Uninit_fill_n(_Ty* _First, _Diff _Count, const _Tval& _Val, allocator<_Ty>&, _Scalar_ptr_iterator_tag)
{
    fill_n(_First, _Count, _Val);
}
template <class _FwdIt, class _Diff, class _Tval, class _Alloc> inline void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al)
{
    _Uninit_fill_n(_First, _Count, _Val, _Al, _Ptr_cat(_First, _First));
}
template <class _Ty, class _Alloc> inline void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al, _Scalar_ptr_iterator_tag)
{
}
template <class _Ty, class _Alloc> inline void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al)
{
    _Destroy_range(_First, _Last, _Al, _Ptr_cat(_First, _Last));
}
struct _Nonscalar_ptr_iterator_tag {};
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
template <class _FwdIt, class _Ty> inline void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}
template <class _Ty, class _Ax = allocator<_Ty> > class deque {
public:
    enum {
        _DEQUEMAPSIZ = 8,
        _DEQUESIZ = sizeof(_Ty) <= 1 ? 16 : sizeof(_Ty) <= 2 ? 8 : sizeof(_Ty) <= 4 ? 4 : sizeof(_Ty) <= 8 ? 2 : 1
    };
    typedef deque<_Ty, _Ax> _Myt;
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef _Ty* _Tptr;
    typedef _Tptr* _Mapptr;
    class const_iterator;
    friend class const_iterator;
    class const_iterator : public _Ranit<_Ty, difference_type, const _Ty*, const _Ty&> {
    public:
        typedef random_access_iterator_tag iterator_category;
        const_iterator(size_type _Off, const _Myt* _Pdeque)
        {
            _Mycont = _Pdeque;
            _Myoff = _Off;
        }
        const _Ty& operator*() const
        {
            size_type _Block = _Myoff / _DEQUESIZ;
            size_type _Off = _Myoff - _Block * _DEQUESIZ;
            if (_Mycont->_Mapsize <= _Block)
                _Block -= _Mycont->_Mapsize;
            return ((_Mycont->_Map)[_Block][_Off]);
        }
        const_iterator& operator++()
        {
            ++_Myoff;
            return (*this);
        }
        const_iterator& operator--()
        {
            --_Myoff;
            return (*this);
        }
        bool operator==(const const_iterator& _Right) const
        {
            return (_Mycont == _Right._Mycont && _Myoff == _Right._Myoff);
        }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }

    protected:
        const _Myt* _Mycont;
        size_type _Myoff;
    };
    class iterator;
    friend class iterator;
    class iterator : public const_iterator {
    public:
        typedef random_access_iterator_tag iterator_category;
        iterator(size_type _Off, const _Myt* _Pdeque) : const_iterator(_Off, _Pdeque) {}
        _Ty& operator*() const { return ((_Ty&)**(const_iterator*)this); }
        iterator& operator++()
        {
            ++this->_Myoff;
            return (*this);
        }
        iterator& operator--()
        {
            --this->_Myoff;
            return (*this);
        }
    };
    void push_front(const _Ty& _Val)
    {
        if (_Myoff % _DEQUESIZ == 0 && _Mapsize <= (_Mysize + _DEQUESIZ) / _DEQUESIZ)
            _Growmap(1);
        size_type _Newoff = _Myoff != 0 ? _Myoff : _Mapsize * _DEQUESIZ;
        size_type _Block = --_Newoff / _DEQUESIZ;
        if (_Map[_Block] == 0)
            _Map[_Block] = this->_Alval.allocate(_DEQUESIZ);
        this->_Alval.construct(_Map[_Block] + _Newoff % _DEQUESIZ, _Val);
        _Myoff = _Newoff;
        ++_Mysize;
    }
    void push_back(const _Ty& _Val)
    {
        if ((_Myoff + _Mysize) % _DEQUESIZ == 0 && _Mapsize <= (_Mysize + _DEQUESIZ) / _DEQUESIZ)
            _Growmap(1);
        size_type _Newoff = _Myoff + _Mysize;
        size_type _Block = _Newoff / _DEQUESIZ;
        if (_Mapsize <= _Block)
            _Block -= _Mapsize;
        if (_Map[_Block] == 0)
            _Map[_Block] = this->_Alval.allocate(_DEQUESIZ);
        this->_Alval.construct(_Map[_Block] + _Newoff % _DEQUESIZ, _Val);
        ++_Mysize;
    }

    size_type max_size() const { return (this->_Alval.max_size()); }

protected:
    void _Growmap(size_type _Count)
    {
        if (max_size() / _DEQUESIZ - _Mapsize < _Count) {
            // !FAKE: noreturn; retail's inline _Xlen throws, decl-only ours needs __assume(0) (C0268)
            _Xlen();
            __assume(0);
        }
        size_type _Inc = _Mapsize / 2;
        if (_Inc < _DEQUEMAPSIZ)
            _Inc = _DEQUEMAPSIZ;
        if (_Count < _Inc && _Mapsize <= max_size() / _DEQUESIZ - _Inc)
            _Count = _Inc;
        size_type _Myboff = _Myoff / _DEQUESIZ;
        _Mapptr _Newmap = this->_Almap.allocate(_Mapsize + _Count);
        _Mapptr _Myptr = _Newmap + _Myboff;
        _Myptr = _Uninitialized_copy(_Map + _Myboff, _Map + _Mapsize, _Myptr, this->_Almap);
        // !FAKE: c1xx evaluation order; copy nested in a direct fill_n call, retail pushes the fill's temp and count
        // before the copy (VC 7.1 header assigns _Myptr, then _Uninitialized_fill_n)
        if (_Myboff <= _Count) {
            fill_n(_Uninitialized_copy(_Map, _Map + _Myboff, _Myptr, this->_Almap), _Count - _Myboff, (_Tptr)0);
            _Uninitialized_fill_n(_Newmap, _Myboff, (_Tptr)0, this->_Almap);
        } else {
            _Uninitialized_copy(_Map, _Map + _Count, _Myptr, this->_Almap);
            fill_n(_Uninitialized_copy(_Map + _Count, _Map + _Myboff, _Newmap, this->_Almap), _Count, (_Tptr)0);
        }
        _Destroy_range(_Map + _Myboff, _Map + _Mapsize, this->_Almap);
        if (_Map)
            this->_Almap.deallocate(_Map, _Mapsize);
        _Map = _Newmap;
        _Mapsize += _Count;
    }
    void _Xlen() const;

    allocator<_Tptr> _Almap;           // 0x00
    _Ax _Alval;                        // 0x01
    _Mapptr _Map;                      // 0x04
    size_type _Mapsize;                // 0x08
    size_type _Myoff;                  // 0x0C
    size_type _Mysize;                 // 0x10
};
}

// FUNCTION: USA 0x000DE6F2
// PROTOTYPE: PREVIEW 0x004C9AAD identical
// NAME: proto-pdb:default.pdb
void BSP::CalculatePointers(const void* image)
{
    _planes = (const Vector4*)((const char*)image + sizeof(BSP_HEADER));
    _triangleList = (const BSPTriangleInfo*)(_planes + _header.numPlanes);
    _vertexList = (const Vector3*)(_triangleList + _header.numTriangles);
    _triangleIndices = (const unsigned int*)(_vertexList + _header.numVertices);
    _nodes = (const BSPnode*)(_triangleIndices + _header.numIndices);
}

// FUNCTION: USA 0x000DE72B
// PROTOTYPE: PREVIEW 0x004C9AE6 identical
// NAME: proto-pdb:default.pdb
BSP::~BSP()
{
    BeepFree(_data);
    delete[] _triCheckedMarkers;
}

#ifdef NON_MATCHING
// row USA 0x000DE742, PREVIEW 0x004C9AFD identical
// best: match_one objdiff 100.0, bytes match; wall: provenance_lint nested operator (C0212): no NAME tag, so
// no FUNCTION tag (4.4 T11.c8)
template BSPQueryResultObject const & std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::const_iterator::operator*()const;
#endif

// FUNCTION: USA 0x000DE760
// PROTOTYPE: PREVIEW 0x004C9B37 identical
// NAME: proto-pdb:default.pdb
BSP::BSP(void* fileImage, bool owns)
    : _data(owns ? fileImage : 0), _planes(0), _triangleList(0), _vertexList(0), _triangleIndices(0), _nodes(0),
      _triCheckedMarkers(0), _currentMarker(0)
{
    _header = *(const BSP_HEADER*)fileImage;
    // !FAKE: c2 block layout; dummy local; memset branch laid first like retail only with the condition in a local (match_one 78.5 -> 100.0)
    bool v0 = _header.magic == 0x20505342 && _header.version == 4;
    if (!v0)
        memset(&_header, 0, sizeof(_header));
    else
        CalculatePointers(fileImage);
}

// FUNCTION: USA 0x000DE7C2
// PROTOTYPE: PREVIEW 0x004C9B99 identical
// NAME: proto-pdb:default.pdb
Ptr<BSPQueryResultObject> BSP::CheckLine(const Vector3& start, const Vector3& end, unsigned int ignoreMask) const
{
    BSPnode::CheckInfo ci(*this, 0.0f, ignoreMask, start, end);
    return GetRoot()->CheckLine(start, end, ci);
}

// FUNCTION: USA 0x000DE800
// PROTOTYPE: PREVIEW 0x004C9BD7 identical
// NAME: proto-pdb:default.pdb
Ptr<BSPQueryResultObject> BSP::CheckSphere(const Vector3& start, const Vector3& end, float radius, unsigned int ignoreMask) const
{
    BSPnode::CheckInfo ci(*this, radius, ignoreMask, start, end);
    return GetRoot()->CheckSphere(start, end, ci);
}

// FUNCTION: USA 0x000DE85E
// PROTOTYPE: PREVIEW 0x004C9C35 identical
// NAME: proto-pdb:default.pdb
template std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator std::_Copy_backward_opt<std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator>(std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000DE8B4
// PROTOTYPE: PREVIEW 0x004C9C8B identical
// NAME: proto-pdb:default.pdb
template std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator std::_Copy_opt<std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator>(std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000DE90A
// PROTOTYPE: PREVIEW 0x004C9CE1 identical
// NAME: proto-pdb:default.pdb
Ptr<BSPQueryResultObject> BSPQueryResultObject::Reset()
{
    _plane.Zero();
    _tag = 0;
    _pct = 1.0f;
    _blindData = 0;
    _bone = 0;
    _hitPlatform = false;
    _platformMove.Zero();
    return Ptr<BSPQueryResultObject>(this);
}

// FUNCTION: USA 0x000DE99B
// PROTOTYPE: PREVIEW 0x004C9D72 identical
// NAME: proto-pdb:default.pdb
template void std::fill<std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,BSPQueryResultObject>(std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::iterator,BSPQueryResultObject const &);

// FUNCTION: USA 0x000DEA9F
// PROTOTYPE: PREVIEW 0x004C9E76 identical
// NAME: proto-pdb:default.pdb
template void std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::_Growmap(unsigned int);

// FUNCTION: USA 0x000DEBA4
// PROTOTYPE: PREVIEW 0x004C9F7B identical
// NAME: proto-pdb:default.pdb
template void std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::push_front(BSPQueryResultObject const &);

// FUNCTION: USA 0x000DEBF6
// PROTOTYPE: PREVIEW 0x004C9FCD identical
// NAME: proto-pdb:default.pdb
template void std::deque<BSPQueryResultObject,std::allocator<BSPQueryResultObject> >::push_back(BSPQueryResultObject const &);
