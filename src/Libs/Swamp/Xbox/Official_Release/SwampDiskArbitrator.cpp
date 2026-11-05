// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampDiskArbitrator.cpp OBJ: Xbox\Official_Release\SwampDiskArbitrator.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#pragma warning(disable: 4530)
#include <algorithm>

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator;
template <class T, class A> class list {
public:
    struct _Node { _Node* _Next; _Node* _Prev; T _Myval; };
    class iterator { public: T& operator*() const { return _Ptr->_Myval; } _Node* _Ptr; };
    iterator begin() { iterator i; i._Ptr = _Myhead->_Next; return i; }
    unsigned int size() const { return _Mysize; }
    T& front() { return *begin(); }
    void pop_front() { erase(begin()); }
    iterator erase(iterator where);
private:
    char _Alnod[4];                    // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class T, class A> class vector {
public:
    // verbatim VC 7.1 <vector> iterator members the heap rows use (C0132)
    class const_iterator : public _Ranit<T, int, const T*, const T&> {
    public:
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef int difference_type;
        typedef const T* pointer;
        typedef const T& reference;
        const T& operator*() const { return (*_Myptr); }
        const_iterator& operator+=(difference_type _Off) { _Myptr += _Off; return (*this); }
        difference_type operator-(const const_iterator& _Right) const { return (_Myptr - _Right._Myptr); }
        bool operator==(const const_iterator& _Right) const { return (_Myptr == _Right._Myptr); }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
        T* _Myptr;
    };
    class iterator : public const_iterator {
    public:
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef int difference_type;
        typedef T* pointer;
        typedef T& reference;
        reference operator*() const { return ((reference)**(const_iterator *)this); }
        iterator& operator--() { --this->_Myptr; return (*this); }
        iterator& operator+=(difference_type _Off) { this->_Myptr += _Off; return (*this); }
        iterator operator+(difference_type _Off) const { iterator _Tmp = *this; return (_Tmp += _Off); }
        iterator& operator-=(difference_type _Off) { return (*this += -_Off); }
        iterator operator-(difference_type _Off) const { iterator _Tmp = *this; return (_Tmp -= _Off); }
        difference_type operator-(const const_iterator& _Right) const { return ((const_iterator)*this - _Right); }
    };
    iterator begin() { iterator i; i._Myptr = _Myfirst; return i; }
    iterator end() { iterator i; i._Myptr = _Mylast; return i; }
    unsigned int size() const;
    void push_back(const T& _Val);
    bool empty() const { return size() == 0; }
    T& back() { return *(_Mylast - 1); }
    void pop_back()
    {
        if (!empty())
            --_Mylast;
    }
private:
    char _Alval[4];                    // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
// !FAKE: c1xx front end; verbatim VC 7.1 vector::size body, defined out of class so /Ob1 keeps the retail call
template <class T, class A> unsigned int vector<T, A>::size() const
{
    return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst);
}
}

class BeepFile; // NAME: proto-pdb:default.pdb

class DiskQueue { // NAME: proto-pdb:default.pdb
public:
    struct Request { // NAME: proto-pdb:default.pdb
        unsigned int priority;         // 0x00
        BeepFile* f;                   // 0x04
        unsigned int offset;           // 0x08
        void* data;                    // 0x0C
        unsigned int bytes;            // 0x10
        unsigned int* resultLoc;       // 0x14

        // !FAKE: provenance; neutral stand-in parameter, the PDB records no proc; body from retail _Push_heap (cmp priority; jae)
        bool operator<(const Request& v0) const { return priority < v0.priority; }
    };
    class RequestQueue : public std::vector<Request, std::allocator<Request> > { // NAME: proto-pdb:default.pdb
    public:
        void Add(const Request& r);
        void Pop();
    };
    enum { // NAME: proto-pdb:default.pdb
        MAX_PENDING_COUNT = 0x2
    };

    bool AddReadRequest(BeepFile* f, unsigned int offset, void* data, unsigned int bytes, unsigned int* resultLoc,
                        unsigned int priority);
    bool AddWriteRequest(BeepFile* f, unsigned int offset, void* data, unsigned int bytes, unsigned int* resultLoc,
                         unsigned int priority);
    void Update();

    char field_0[0x1C];                // 0x00
    RequestQueue _queue;               // 0x1C
    unsigned int _maxSize;             // 0x2C
    unsigned int _lastID;              // 0x30
    char field_34[0x78 - 0x34];        // 0x34
    unsigned int _numRequestsPending;  // 0x78
};
DECOMP_SIZE_ASSERT(DiskQueue, 0x7C);

// FUNCTION: USA 0x000D2F41
// PROTOTYPE: PREVIEW 0x004BE1E8 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::size()const;

// FUNCTION: USA 0x000D2F57
// PROTOTYPE: PREVIEW 0x004BE22E identical
// NAME: proto-pdb:default.pdb
template void std::_Push_heap<std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,int,DiskQueue::Request>(std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,int,int,DiskQueue::Request);

// FUNCTION: USA 0x000D2FF2
// PROTOTYPE: PREVIEW 0x004BE2C9 identical
// NAME: proto-pdb:default.pdb
template void std::_Adjust_heap<std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,int,DiskQueue::Request>(std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,int,int,DiskQueue::Request);

// FUNCTION: USA 0x000D3140
// PROTOTYPE: PREVIEW 0x004BE424 identical
// NAME: proto-pdb:default.pdb
template void std::_Push_heap_0<std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,int,DiskQueue::Request>(std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,int *,DiskQueue::Request *);

// FUNCTION: USA 0x000D31DD
// PROTOTYPE: PREVIEW 0x004BE4C1 identical
// NAME: proto-pdb:default.pdb
template void std::_Pop_heap<std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,int,DiskQueue::Request>(std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,DiskQueue::Request,int *);

// FUNCTION: USA 0x000D3218
// PROTOTYPE: PREVIEW 0x004BE4FC identical
// NAME: proto-pdb:default.pdb
template void std::push_heap<std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator>(std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator);

// FUNCTION: USA 0x000D327F
// PROTOTYPE: PREVIEW 0x004BE563 identical
// NAME: proto-pdb:default.pdb
template void std::_Pop_heap_0<std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,DiskQueue::Request>(std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,DiskQueue::Request *);

// FUNCTION: USA 0x000D32CB
// PROTOTYPE: PREVIEW 0x004BE5AF identical
// NAME: proto-pdb:default.pdb
template void std::pop_heap<std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator>(std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator,std::vector<DiskQueue::Request,std::allocator<DiskQueue::Request> >::iterator);

// FUNCTION: USA 0x000D32F1
// PROTOTYPE: PREVIEW 0x004BE5D5 identical
// NAME: proto-pdb:default.pdb
void DiskQueue::RequestQueue::Pop()
{
    std::pop_heap(begin(), end());
    pop_back();
}

// FUNCTION: USA 0x000D37A1
// PROTOTYPE: PREVIEW 0x004BEAAB identical
// NAME: proto-pdb:default.pdb
void DiskQueue::RequestQueue::Add(const Request& r)
{
    push_back(r);
    std::push_heap(begin(), end());
}

class DiskArbitrator { // NAME: proto-pdb:default.pdb
public:
    struct MirrorRequest { // NAME: proto-pdb:default.pdb
        bool Update(void*);
    };

    void Update();

    DiskQueue _hardDriveQueue;         // 0x00
    DiskQueue _dvdQueue;               // 0x7C
    std::list<MirrorRequest*, std::allocator<MirrorRequest*> > _mirrorQueue; // 0xF8
    unsigned char _buffer[131072];     // 0x104
};

#ifdef NON_MATCHING
// best: match_one objdiff 89.71, bytes mismatch (masked 1): register allocation only
// FUNCTION: USA 0x000D37C0
// PROTOTYPE: PREVIEW 0x004BEACA near-identical
// NAME: proto-pdb:default.pdb
bool DiskQueue::AddReadRequest(BeepFile* f, unsigned int offset, void* data, unsigned int bytes,
                               unsigned int* resultLoc, unsigned int priority)
{
    if (_queue.size() >= _maxSize) {
        Request& last = _queue.back();
        if ((last.priority & 0xF0000000) < (priority << 28)) {
            if (last.resultLoc)
                *last.resultLoc = 0;
            _queue.pop_back();
        } else {
            *resultLoc = 0;
            return false;
        }
    }
    Request r;
    r.f = f;
    r.offset = offset;
    r.data = data;
    r.bytes = bytes;
    _lastID = (_lastID - 1) & 0x0FFFFFFF;
    r.priority = (priority << 28) + _lastID;
    r.resultLoc = resultLoc;
    _queue.Add(r);
    *resultLoc = 1;
    if (_numRequestsPending < MAX_PENDING_COUNT)
        Update();
    return true;
}
#endif

// FUNCTION: USA 0x000D3871
// PROTOTYPE: PREVIEW 0x004BEB7E identical
// NAME: proto-pdb:default.pdb
bool DiskQueue::AddWriteRequest(BeepFile* f, unsigned int offset, void* data, unsigned int bytes,
                                unsigned int* resultLoc, unsigned int priority)
{
    return AddReadRequest(f, offset, data, bytes | 0x80000000, resultLoc, priority);
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 3 = the two unpinned callees)
// FUNCTION: USA 0x000D3991
// PROTOTYPE: PREVIEW 0x004BECA1 near-identical
// NAME: proto-pdb:default.pdb
void DiskArbitrator::Update()
{
    _hardDriveQueue.Update();
    _dvdQueue.Update();
    if (_mirrorQueue.size()) {
        if (!_mirrorQueue.front()->Update(_buffer)) {
            _mirrorQueue.pop_front();
        }
    }
}
#endif
