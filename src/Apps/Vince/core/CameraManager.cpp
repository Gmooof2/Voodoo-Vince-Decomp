// UNIT: src/Apps/Vince/core/CameraManager.cpp OBJ: core\XBox\OfficialRelease\CameraManager.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

inline void* operator new(unsigned int, void* _Where) { return _Where; }
class SwampScript; // NAME: proto-pdb:default.pdb

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    int compare(const basic_string& _Right) const;
    unsigned int size() const { return _Mysize; }
    bool empty() const { return _Mysize == 0; }
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x14];                 // 0x00
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A> inline bool operator==(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right)
{
    return (_Left.compare(_Right) == 0);
}
template <class T> struct less;
template <class T1, class T2> struct pair {
    pair(const T1& _Val1, const T2& _Val2) : first(_Val1), second(_Val2) {}
    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
};
template <class Tr> class _Tree {
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
    };
public:
    typedef typename Tr::key_type key_type;
    typedef typename Tr::value_type value_type;
    class const_iterator {
    public:
        const_iterator() {}
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const typename Tr::value_type* operator->() const { return &_Ptr->_Myval; }
        bool operator==(const const_iterator& _Right) const { return _Ptr == _Right._Ptr; }
        bool operator!=(const const_iterator& _Right) const { return _Ptr != _Right._Ptr; }
        const_iterator& operator++()
        {
            _Inc();
            return (*this);
        }
        void _Inc();
        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        iterator() {}
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
        typename Tr::value_type* operator->() const { return &this->_Ptr->_Myval; }
    };
    _Tree() { _Init(); }
    ~_Tree() { _Tidy(); }
    iterator begin() { return iterator(_Myhead->_Left); }
    const_iterator begin() const { return const_iterator(_Myhead->_Left); }
    iterator end() { return iterator(_Myhead); }
    const_iterator end() const { return const_iterator(_Myhead); }
    unsigned int size() const { return _Mysize; }
    bool empty() const { return size() == 0; }
    pair<iterator, bool> insert(const typename Tr::value_type& _Val);
    iterator erase(iterator _Where);
    iterator find(const typename Tr::key_type& _Keyval);
    const_iterator find(const typename Tr::key_type& _Keyval) const;

protected:
    void _Init();
    void _Tidy();

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map
    : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
template <class _Iter> struct iterator_traits {
    typedef typename _Iter::iterator_category iterator_category;
};
template <class _Iter> inline typename iterator_traits<_Iter>::iterator_category _Iter_cat(const _Iter&)
{
    typename iterator_traits<_Iter>::iterator_category _Cat;
    return (_Cat);
}
template <class _Ty, class _Ax = allocator<_Ty> > class list {
    struct _Node {
        _Node* _Next;                  // 0x00
        _Node* _Prev;                  // 0x04
        _Ty _Myval;                    // 0x08
    };
    static _Node*& _Nextnode(_Node* _Pnode) { return ((_Node*&)(*_Pnode)._Next); }
public:
    typedef list<_Ty, _Ax> _Myt;
    class const_iterator {
    public:
        typedef bidirectional_iterator_tag iterator_category;
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        typedef bidirectional_iterator_tag iterator_category;
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
    };
    _Myt& operator=(const _Myt& _Right)
    {
        if (this != &_Right)
            assign(_Right.begin(), _Right.end());
        return (*this);
    }
    iterator begin() { return (iterator(_Nextnode(_Myhead))); }
    const_iterator begin() const { return (const_iterator(_Nextnode(_Myhead))); }
    const_iterator end() const { return (const_iterator(_Myhead)); }
    template <class _Iter> void assign(_Iter _First, _Iter _Last) { _Assign(_First, _Last, _Iter_cat(_First)); }
    template <class _Iter> void _Assign(_Iter _First, _Iter _Last, input_iterator_tag)
    {
        clear();
        insert(begin(), _First, _Last);
    }
    template <class _Iter> void insert(iterator _Where, _Iter _First, _Iter _Last)
    {
        _Insert(_Where, _First, _Last, _Iter_cat(_First));
    }
    template <class _Iter> void _Insert(iterator _Where, _Iter _First, _Iter _Last, forward_iterator_tag);
    void clear();

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
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
    void construct(pointer _Ptr, const _Ty& _Val) { _Construct(_Ptr, _Val); }
};
struct _Nonscalar_ptr_iterator_tag {};
template <class _T1, class _T2> inline _Nonscalar_ptr_iterator_tag _Ptr_cat(_T1&, _T2&)
{
    _Nonscalar_ptr_iterator_tag _Cat;
    return (_Cat);
}
template <class _InIt, class _OutIt> inline _OutIt _Copy_opt(_InIt _First, _InIt _Last, _OutIt _Dest, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_Dest, ++_First)
        *_Dest = *_First;
    return (_Dest);
}
template <class _InIt, class _OutIt> inline _OutIt copy(_InIt _First, _InIt _Last, _OutIt _Dest)
{
    return (_Copy_opt(_First, _Last, _Dest, _Ptr_cat(_First, _Dest)));
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
        difference_type operator-(const const_iterator& _Right) const
        {
            return (_Right._Myoff <= _Myoff ? _Myoff - _Right._Myoff : -(difference_type)(_Right._Myoff - _Myoff));
        }
        bool operator==(const const_iterator& _Right) const
        {
            return (_Mycont == _Right._Mycont && _Myoff == _Right._Myoff);
        }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
        const_iterator& operator+=(difference_type _Off)
        {
            _Myoff += _Off;
            return (*this);
        }
        const_iterator operator+(difference_type _Off) const
        {
            const_iterator _Tmp = *this;
            return (_Tmp += _Off);
        }

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
    _Myt& operator=(const _Myt& _Right)
    {
        if (this == &_Right)
            ;
        else if (_Right._Mysize == 0)
            clear();
        else if (_Right._Mysize <= _Mysize) {
            iterator _Mid = copy(_Right.begin(), _Right.end(), begin());
            erase(_Mid, end());
        } else {
            const_iterator _Mid = _Right.begin() + _Mysize;
            copy(_Right.begin(), _Mid, begin());
            insert(end(), _Mid, _Right.end());
        }
        return (*this);
    }
    iterator begin() { return (iterator(_Myoff, this)); }
    const_iterator begin() const { return (const_iterator(_Myoff, this)); }
    iterator end() { return (iterator(_Myoff + _Mysize, this)); }
    const_iterator end() const { return (const_iterator(_Myoff + _Mysize, this)); }
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
    template <class _It> void insert(iterator _Where, _It _First, _It _Last)
    {
        _Insert(_Where, _First, _Last, _Iter_cat(_First));
    }
    template <class _It> void _Insert(iterator _Where, _It _First, _It _Last, bidirectional_iterator_tag);
    iterator erase(iterator _First, iterator _Last);
    void clear() { _Tidy(); }

protected:
    void _Growmap(size_type _Count);
    void _Tidy();

    char _Almap;                       // 0x00
    _Ax _Alval;                        // 0x01
    _Mapptr _Map;                      // 0x04
    size_type _Mapsize;                // 0x08
    size_type _Myoff;                  // 0x0C
    size_type _Mysize;                 // 0x10
};
template <class _RanIt, class _Diff> inline void _Distance2(_RanIt _First, _RanIt _Last, _Diff& _Off, random_access_iterator_tag)
{
    _Off += _Last - _First;
}
template <class _Ty, class _Container = deque<_Ty> > class stack { // NAME: proto-pdb:default.pdb
protected:
    _Container c;                      // 0x00
};
template <class _InIt, class _Diff> inline void _Distance(_InIt _First, _InIt _Last, _Diff& _Off)
{
    _Distance2(_First, _Last, _Off, _Iter_cat(_First));
}
template <class _BidIt1, class _BidIt2> inline _BidIt2 _Copy_backward_opt(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Nonscalar_ptr_iterator_tag)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);
Quaternion Slerp(const Quaternion&, const Quaternion&, float); // NAME: proto-pdb:default.pdb

struct AABox { // NAME: proto-pdb:default.pdb
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct Vector4 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

class AnimationControl; // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle& operator=(const ResourceHandle&); // retail ??4?$ResourceHandle@VAnimationControl@@@@QAEAAV0@ABV0@@Z

    T* _Resource;                      // 0x00
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString MakeLower(const SwampString& s);
}

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

// FUNCTION: USA 0x00109F5F
// PROTOTYPE: PREVIEW 0x004F4866 identical
// NAME: proto-pdb:default.pdb
class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    const SwampString& GetName() const { return _Name; }
    const Vector3& SetLocation(const Vector3&);
    const Vector3& GetLocation() const { return _Location; }
    const Quaternion& SetRotation(const Quaternion&);
    const Quaternion& GetRotation() const { return _Rotation; }

    int _Type;                         // 0x04
    SwampString _Name;                 // 0x08
    SwampSceneNode* _Parent;           // 0x24
    std::list<SwampSceneNode*> _Children; // 0x28
    Vector3 _Location;                 // 0x34
    Quaternion _Rotation;              // 0x40
    Vector3 _Scale;                    // 0x50
    Vector3 _cachedWorldPosition;      // 0x5C
    bool _cachedWorldPositionValid;    // 0x68
    AABox _BoundingBox;                // 0x6C
    unsigned int _lastRenderFrame;     // 0x84
    unsigned int _RenderingOptions;    // 0x88
    unsigned int _RenderingSetting[1]; // 0x8C
    std::stack<SwampScript*> _scripts; // 0x90
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// FUNCTION: USA 0x0010A006
// PROTOTYPE: PREVIEW 0x004F490D identical
// NAME: proto-pdb:default.pdb
class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampCamera();
    virtual Vector3 GetViewPosition() const;                       // vtable+0x24
    virtual Matrix GetViewMatrix() const;                          // vtable+0x28
    virtual const Matrix& GetProjectionMatrix() const;             // vtable+0x2C
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    void SetFOV(float);
    float GetFOV() const { return _fov; }
    void SetFarPlane(float);

    float _fov;                        // 0xA4
    float _aspectRatio;                // 0xA8
    float _nearPlane;                  // 0xAC
    float _farPlane;                   // 0xB0
    Vector3 _up;                       // 0xB4
    ResourceHandle<AnimationControl> _currentPath; // 0xC0
    float _speed;                      // 0xC4
    bool _projectionValid;             // 0xC8
    Matrix _projection;                // 0xCC
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

// FUNCTION: USA 0x0010A099
// PROTOTYPE: PREVIEW 0x004F49A0 identical
// NAME: proto-pdb:default.pdb
class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    GameCam();
    virtual ~GameCam();
    virtual void Activate();                                       // vtable+0x30
    virtual void LosingControl();                                  // vtable+0x34
    virtual Vector3 GetViewPosition() const;                       // vtable+0x24

    float _shakeFactor;                // 0x10C
    float _shakeDuration;              // 0x110
    float _shakeTimer;                 // 0x114
    bool _isCurrentCamera;             // 0x118
    bool _isAnimated;                  // 0x119

protected:
    virtual void Update();                                         // vtable+0x1C
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Reset();
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
    char field_4[0x120 - 0x4];         // 0x04
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampCamera* GetCamera() const { return _currentCamera; }
    SwampCamera* SetCamera(SwampCamera* camera) { _currentCamera = camera; return camera; }
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x40 - 0x8];          // 0x08
    SwampCamera* _currentCamera;       // 0x40
    char field_44[0x68 - 0x44];        // 0x44
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    CameraManager();
    ~CameraManager();
    void AddCamera(const SwampString& s, GameCam* p);
    void Remove(const SwampString& s);
    GameCam* Find(const SwampString& s) const;
    void SetCurCam(const SwampString& s, float delay);
    void DestroyAll();
    void Update() const;
    GameCam* GetBlendedCamera() const;

    typedef std::map<SwampString, GameCam*> NameMap;

    NameMap _cameras;                  // 0x00
    SwampString _curCamName;           // 0x0C
    SwampString _defaultCamName;       // 0x28
    float _farPlane;                   // 0x44
    SwampString _oldCamName;           // 0x48
    // !FAKE: c1xx front end; mutable; the PDB's const Update writes _delay and _oldCamCopy (CV records no mutable)
    mutable float _delay;              // 0x64
    // !FAKE: c1xx front end; mutable; the PDB's const GetBlendedCamera writes _totalDelay and _blendedCam
    mutable float _totalDelay;         // 0x68
    mutable GameCam* _blendedCam;      // 0x6C
    mutable GameCam* _oldCamCopy;      // 0x70
};
DECOMP_SIZE_ASSERT(CameraManager, 0x74);

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00108D4F
// PROTOTYPE: PREVIEW 0x004F3699 identical
// NAME: proto-pdb:default.pdb
template void std::_Distance<std::deque<SwampScript *,std::allocator<SwampScript *> >::const_iterator,unsigned int>(std::deque<SwampScript *,std::allocator<SwampScript *> >::const_iterator,std::deque<SwampScript *,std::allocator<SwampScript *> >::const_iterator,unsigned int &);

// FUNCTION: USA 0x00108EA3
// PROTOTYPE: PREVIEW 0x004F37ED identical
// NAME: proto-pdb:default.pdb
template std::deque<SwampScript *,std::allocator<SwampScript *> >::iterator std::_Copy_backward_opt<std::deque<SwampScript *,std::allocator<SwampScript *> >::iterator,std::deque<SwampScript *,std::allocator<SwampScript *> >::iterator>(std::deque<SwampScript *,std::allocator<SwampScript *> >::iterator,std::deque<SwampScript *,std::allocator<SwampScript *> >::iterator,std::deque<SwampScript *,std::allocator<SwampScript *> >::iterator,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x0010912E
// PROTOTYPE: PREVIEW 0x004F3A35 identical
// NAME: proto-pdb:default.pdb
void CameraManager::Update() const
{
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    NameMap::const_iterator cit = _cameras.find(_curCamName);
    for (NameMap::const_iterator it = _cameras.begin(); it != _cameras.end(); ++it) {
        it->second->_isCurrentCamera = (it == cit);
    }
    if (_delay != 0.0f && _totalDelay != 0.0f) {
        _delay -= elapsed;
        if (_delay <= 0.0f) {
            _delay = 0.0f;
            if (_oldCamCopy) {
                delete _oldCamCopy;
                _oldCamCopy = 0;
            }
        }
    }
}

// FUNCTION: USA 0x001091CF
// NAME: proto-pdb:default.pdb
GameCam* CameraManager::Find(const SwampString& s) const
{
    SwampString lookup = BeepLib::MakeLower(s);
    NameMap::const_iterator it = _cameras.find(lookup);
    return it != _cameras.end() ? it->second : 0;
}

// FUNCTION: USA 0x00109217
// NAME: proto-pdb:default.pdb
void CameraManager::SetCurCam(const SwampString& s, float delay)
{
    SwampString lookup = BeepLib::MakeLower(s);
    if (lookup.empty() || lookup == _curCamName) {
        return;
    }
    // !FAKE: c2 register allocation; dummy local; the PDB records no local for the found camera (edi)
    GameCam* p = Find(lookup);
    if (p) {
        p->Activate();
        _oldCamName = _curCamName;
        _curCamName = lookup;
        g_swamp->SetCamera(p);
        _delay = delay;
        p = Find(_oldCamName);
        if (p) {
            p->LosingControl();
        }
        if (_delay != 0.0f && p) {
            _totalDelay = _delay;
            // !FAKE: retail string literal; the original passes its build's __FILE__, ours differs
            _oldCamCopy = new("core\\CameraManager.cpp", 206) GameCam();
            _oldCamCopy->SetLocation(p->GetLocation());
            _oldCamCopy->SetRotation(p->GetRotation());
            _oldCamCopy->SetFOV(p->GetFOV());
        } else {
            _totalDelay = 0.0f;
        }
    }
}

// FUNCTION: USA 0x00109411
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.09, bytes mismatch (retail mov ebx,[ebp+8] reads the find slot; this form reads it via mov ebx,[eax])
void CameraManager::Remove(const SwampString& s)
{
    SwampString lookup = BeepLib::MakeLower(s);
    // !FAKE: c2 register allocation; declared then assigned so the iterator is a copy (ebx), not the find target
    NameMap::iterator it;
    it = _cameras.find(lookup);
    if (it != _cameras.end()) {
        // !FAKE: c2 register allocation; dummy local; the PDB records no local for the erased camera (edi)
        GameCam* p = it->second;
        bool bneedcam = false;
        if (_curCamName == p->GetName()) {
            bneedcam = true;
        }
        _cameras.erase(it);
        p->Invalidate();
        if (bneedcam) {
            if (!_defaultCamName.empty()) {
                SetCurCam(_defaultCamName, 0.0f);
            } else {
                // !FAKE: c1xx front end; dummy local; retail keeps begin() in ecx for the test and the use
                NameMap::iterator v1 = _cameras.begin();
                if (v1 != _cameras.end()) {
                    SetCurCam(v1->second->GetName(), 0.0f);
                }
            }
        }
    }
}
#endif

// FUNCTION: USA 0x001094B0
// PROTOTYPE: PREVIEW 0x004F3DB7 identical
// NAME: proto-pdb:default.pdb
void CameraManager::DestroyAll()
{
    AnimCam::Reset();
    // !FAKE: c2 register allocation; dummy local; retail loads the current camera once into ebp before the loop
    SwampCamera* v1 = g_swamp->GetCamera();
    // !FAKE: c1xx front end; dummy local; the PDB records no locals for the loop iterator
    NameMap::iterator p = _cameras.begin();
    while (p != _cameras.end()) {
        // !FAKE: c2 register allocation; dummy local; the camera stays in ebx across erase
        GameCam* tmp = p->second;
        if (v1 == tmp) {
            g_swamp->SetCamera(0);
        }
        p = _cameras.erase(p);
        tmp->Invalidate();
    }
    _curCamName = "";
    _defaultCamName = "";
}

// FUNCTION: USA 0x00109D2F
// NAME: proto-pdb:default.pdb
void CameraManager::AddCamera(const SwampString& s, GameCam* p)
{
    SwampString lookup = BeepLib::MakeLower(s);
    p->_Name = lookup;
    p->SetFarPlane(_farPlane);
    _cameras.insert(NameMap::value_type(lookup, p));
}

// FUNCTION: USA 0x00109DC5
// PROTOTYPE: PREVIEW 0x004F46CC identical
// NAME: proto-pdb:default.pdb
template void std::list<SwampSceneNode *,std::allocator<SwampSceneNode *> >::_Assign<std::list<SwampSceneNode *,std::allocator<SwampSceneNode *> >::const_iterator>(std::list<SwampSceneNode *,std::allocator<SwampSceneNode *> >::const_iterator,std::list<SwampSceneNode *,std::allocator<SwampSceneNode *> >::const_iterator,std::input_iterator_tag);

// FUNCTION: USA 0x00109DEB
// PROTOTYPE: PREVIEW 0x004F46F2 identical
// NAME: proto-pdb:default.pdb
CameraManager::~CameraManager()
{
    while (!_cameras.empty()) {
        // !FAKE: c1xx front end; dummy local; the PDB records no locals for the erased node
        NameMap::iterator p = _cameras.begin();
        delete p->second;
        _cameras.erase(p);
    }
    delete _blendedCam;
    g_CameraManager = 0;
}

// FUNCTION: USA 0x00109EFB
// PROTOTYPE: PREVIEW 0x004F4802 identical
// NAME: proto-pdb:default.pdb
CameraManager::CameraManager() : _farPlane(100.0f), _delay(0.0f), _totalDelay(0.0f), _blendedCam(0)
{
    g_CameraManager = this;
}

// FUNCTION: USA 0x00109F3A
// PROTOTYPE: PREVIEW 0x004F4841 identical
// NAME: proto-pdb:default.pdb
template std::list<SwampSceneNode *,std::allocator<SwampSceneNode *> > & std::list<SwampSceneNode *,std::allocator<SwampSceneNode *> >::operator=(std::list<SwampSceneNode *,std::allocator<SwampSceneNode *> > const &);

// FUNCTION: USA 0x0010A0EA
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 95.84, bytes mismatch (frame 0x60 vs 0x54: GetWorldPosition results get their own slots, retail packs them into temp/-0x24, as an inlined free Vector3 operator* would, no PDB record (D78); z product spilled in the +=)
GameCam* CameraManager::GetBlendedCamera() const
{
    if (!Find(_curCamName)) {
        return 0;
    }
    if (!_blendedCam) {
        // !FAKE: retail string literal; the original passes its build's __FILE__, ours differs
        _blendedCam = new("core\\CameraManager.cpp", 223) GameCam();
    }
    GameCam* curcam = Find(_curCamName);
    if (curcam) {
        *_blendedCam = *curcam;
    }
    if (_oldCamCopy && _delay != 0.0f) {
        float oldpct = _delay / _totalDelay;
        float newpct = 1.0f - oldpct;
        Quaternion qnew = curcam->GetRotation();
        Quaternion qold = _oldCamCopy->GetRotation();
        Quaternion qblend = Slerp(qold, qnew, newpct);
        _blendedCam->SetRotation(qblend);
        // hand forms of the inlined Vector3 scale (no PDB operator*, D78)
        Vector3 temp;
        {
            // !FAKE: inliner; pointer local to the call and dummy local; the scaled copy retail builds before assigning temp
            const Vector3* p = &curcam->GetWorldPosition();
            Vector3 v1;
            v1.x = newpct * p->x;
            v1.y = newpct * p->y;
            v1.z = newpct * p->z;
            temp = v1;
        }
        {
            // !FAKE: inliner; pointer local to the call; the old camera position retail scales into temp
            const Vector3* p = &_oldCamCopy->GetWorldPosition();
            temp += Vector3(oldpct * p->x, oldpct * p->y, oldpct * p->z);
        }
        _blendedCam->SetLocation(temp);
        _blendedCam->SetFOV(newpct * curcam->GetFOV() + oldpct * _oldCamCopy->GetFOV());
        _oldCamCopy->SetLocation(_blendedCam->GetLocation());
        _oldCamCopy->SetRotation(_blendedCam->GetRotation());
        _oldCamCopy->SetFOV(_blendedCam->GetFOV());
        _totalDelay = _delay;
        return _blendedCam;
    }
    return curcam;
}
#endif

// !FAKE: c1xx front end; scaffold call; retail emits the implicit GameCam::operator= in this unit (its user
// GetBlendedCamera is not matched); this assignment odr-uses it instead (matched_set [[emitter]] row)
// (T16.c9) un-guarded: SwampSceneNode/SwampCamera now carry their PDB members, so the implicit base ??4 copies are memberwise
// (was guarded: ??4GameCam 100.0, relink red as the base ??4 copies were opaque char arrays at 0x10A006/0x109F5F)
void pin_emit_GameCam_assign(GameCam* p, const GameCam& v)
{
    *p = v;
}

// FUNCTION: USA 0x001096BA
// PROTOTYPE: PREVIEW 0x004F3FC1 identical
// NAME: proto-pdb:default.pdb
template void std::deque<SwampScript *,std::allocator<SwampScript *> >::push_front(SwampScript * const &);

// FUNCTION: USA 0x00109DA4
// PROTOTYPE: PREVIEW 0x004F46AB identical
// NAME: proto-pdb:default.pdb
template void std::deque<SwampScript *,std::allocator<SwampScript *> >::insert<std::deque<SwampScript *,std::allocator<SwampScript *> >::const_iterator>(std::deque<SwampScript *,std::allocator<SwampScript *> >::iterator,std::deque<SwampScript *,std::allocator<SwampScript *> >::const_iterator,std::deque<SwampScript *,std::allocator<SwampScript *> >::const_iterator);

// FUNCTION: USA 0x00109E5C
// PROTOTYPE: PREVIEW 0x004F4763 identical
// NAME: proto-pdb:default.pdb
template std::deque<SwampScript *,std::allocator<SwampScript *> > & std::deque<SwampScript *,std::allocator<SwampScript *> >::operator=(std::deque<SwampScript *,std::allocator<SwampScript *> > const &);
