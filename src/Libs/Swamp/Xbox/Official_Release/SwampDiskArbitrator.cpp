// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampDiskArbitrator.cpp OBJ: Xbox\Official_Release\SwampDiskArbitrator.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

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
    class iterator { public: T* _Myptr; };
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
template <class RanIt> void push_heap(RanIt _First, RanIt _Last); // src proto-pdb:default.pdb push_heap
template <class RanIt> void pop_heap(RanIt _First, RanIt _Last); // src proto-pdb:default.pdb pop_heap
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
    void Update();

    char field_0[0x1C];                // 0x00
    RequestQueue _queue;               // 0x1C
    unsigned int _maxSize;             // 0x2C
    unsigned int _lastID;              // 0x30
    char field_34[0x78 - 0x34];        // 0x34
    unsigned int _numRequestsPending;  // 0x78
};
DECOMP_SIZE_ASSERT(DiskQueue, 0x7C);

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
