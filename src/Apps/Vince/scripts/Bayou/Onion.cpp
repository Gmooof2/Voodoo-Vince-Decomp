// UNIT: src/Apps/Vince/scripts/Bayou/Onion.cpp OBJ: Xbox\OfficialRelease\Onion.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
}
// VC 7.1 string.h memcpy, wchar.h:791 wmemcpy, iosfwd:268 char_traits<wchar_t> (wchar_t is unsigned short here);
// retail inlines the copy as rep movsw (intrinsic memcpy)
extern "C" void* memcpy(void* _Dst, const void* _Src, unsigned int _Size);
#pragma intrinsic(memcpy)
// !FAKE: c2 intrinsics; file-scope pragma; retail inlines fabs in AnimationControl::SetAnimationSpeed (C0221)
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
inline float fabs(float _X)
{
    return (float)fabs((double)_X);
}
inline unsigned short* wmemcpy(unsigned short* _S1, const unsigned short* _S2, unsigned int _N)
{
    return (unsigned short*)memcpy(_S1, _S2, _N * sizeof(unsigned short));
}
namespace std {
template <> struct char_traits<unsigned short> {
    static void assign(unsigned short& _Left, const unsigned short& _Right) { _Left = _Right; }
    static unsigned short* copy(unsigned short* _First1, const unsigned short* _First2, unsigned int _Count)
    {
        return ::wmemcpy(_First1, _First2, _Count);
    }
};
struct _Nonscalar_ptr_iterator_tag {
};
template <class T> inline void _Destroy(T* _Ptr)
{
    (_Ptr)->~T();
}
}
#pragma warning(disable: 4530) // no /EH in the unit flags; as Init.cpp
// VC 7.1 <new> placement new, <xmemory> _Construct, allocator::allocate/construct (map _Buynode row)
inline void* operator new(unsigned int, void* _Where)
{
    return _Where;
}
namespace std {
template <class T1, class T2> inline void _Construct(T1* _Ptr, const T2& _Val)
{
    new ((void*)_Ptr) T1(_Val);
}
template <class T> class allocator {
public:
    T* allocate(unsigned int _Count) { return (T*)operator new(_Count * sizeof(T)); }
    void construct(T* _Ptr, const T& _Val) { _Construct(_Ptr, _Val); }
    void deallocate(T* _Ptr, unsigned int) { operator delete(_Ptr); }
    void destroy(T* _Ptr) { _Destroy(_Ptr); }
};
template <class T, class A> inline void _Destroy_range(T* _First, T* _Last, A& _Al, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_First)
        _Al.destroy(_First);
}
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E*);
    ~basic_string() { _Tidy(true); }
    const E* c_str() const { return _Myptr(); } // src proto-pdb:default.pdb c_str
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);
    int compare(const E* _Ptr) const;
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }
    E* _Myptr() { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

protected:
    void _Eos(unsigned int _Newsize);
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    A _Alval;                          // 0x00
    union {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
// !FAKE: c1xx front end; decl-only specialization; retail calls the char _Tidy out of line (no row here), keeps it unemitted
template <> void basic_string<char, char_traits<char>, allocator<char> >::_Tidy(bool _Built, unsigned int _Newsize);
template <class E, class T, class A> void basic_string<E, T, A>::_Eos(unsigned int _Newsize)
{
    T::assign(_Myptr()[_Mysize = _Newsize], E());
}
template <class E, class T, class A> void basic_string<E, T, A>::_Tidy(bool _Built, unsigned int _Newsize)
{
    if (!_Built)
        ;
    else if (_BUF_SIZE <= _Myres) {
        E* tmp = _Bx._Ptr; // !FAKE: provenance; stand-in name tmp; VC 7.1 _Ptr, not in the PDB locals
        if (0 < _Newsize)
            T::copy(_Bx._Buf, tmp, _Newsize);
        _Alval.deallocate(tmp, _Myres + 1);
    }
    _Myres = _BUF_SIZE - 1;
    _Eos(_Newsize);
}
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
template <class T1, class T2> struct pair {
    T1 first;                          // 0x00
    T2 second;                         // 0x1C (pair<string,string>)
};
template <class T> struct less;
template <class T, class A> class vector {
public:
    typedef T* iterator;
    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; } // src proto-pdb:default.pdb _Myend // src proto-pdb:default.pdb _Myfirst // src proto-pdb:default.pdb _Mylast
    ~vector() { _Tidy(); }
    iterator begin() { return _Myfirst; }
    iterator end() { return _Mylast; }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class T, class A = allocator<T> > class deque { // src proto-pdb:default.pdb deque
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef pair<const K, T> value_type;
};
// _Tree_nod/_Tree: VC 7.1 <xtree> node layout and _Erase body (as SwampDiary.cpp:93-131, 356-364)
template <class Traits> class _Tree_nod {
public:
    struct _Node;
    typedef _Node* _Genptr;
    struct _Node {
        _Genptr _Left;                 // 0x00
        _Genptr _Parent;               // 0x04
        _Genptr _Right;                // 0x08
        typename Traits::value_type _Myval; // 0x0C
        char _Color;
        char _Isnil;
    };
};
template <class Traits> class _Tree {
public:
    _Tree() { _Init(); } // src proto-pdb:default.pdb _Init
    ~_Tree() { _Tidy(); }

protected:
    typedef typename _Tree_nod<Traits>::_Node _Node;
    typedef _Node* _Nodeptr;
    enum _Redbl { _Red, _Black };
    static char& _Color(_Nodeptr _Pnode) { return ((char&)(*_Pnode)._Color); }
    static char& _Isnil(_Nodeptr _Pnode) { return ((char&)(*_Pnode)._Isnil); }
    static _Nodeptr& _Left(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Left); }
    static _Nodeptr& _Parent(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Parent); }
    static _Nodeptr& _Right(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Right); }
    _Nodeptr _Buynode()
    {
        _Nodeptr _Wherenode = this->_Alnod.allocate(1);
        int _Linkcnt = 0;

        try {
            this->_Alptr.construct(&_Left(_Wherenode), 0);
            ++_Linkcnt;
            this->_Alptr.construct(&_Parent(_Wherenode), 0);
            ++_Linkcnt;
            this->_Alptr.construct(&_Right(_Wherenode), 0);
        } catch (...) {
            if (1 < _Linkcnt)
                this->_Alptr.destroy(&_Parent(_Wherenode));
            if (0 < _Linkcnt)
                this->_Alptr.destroy(&_Left(_Wherenode));
            this->_Alnod.deallocate(_Wherenode, 1);
            throw;
        }
        _Color(_Wherenode) = _Black;
        _Isnil(_Wherenode) = false;
        return (_Wherenode);
    }
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
    void _Tidy();

private:
    allocator<_Node> _Alnod;           // 0x00
    allocator<_Nodeptr> _Alptr;        // 0x01
    char _pad02[0x4 - 0x2];            // 0x02 (_Alval)
    _Nodeptr _Myhead;                  // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > >
class map : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    struct Record { // NAME: proto-pdb:default.pdb
        ~Record();

        SwampString name;              // 0x00
        SwampString stringdata;        // 0x1C
        int intdata;                   // 0x38
        double floatdata;              // 0x40
        bool booldata;                 // 0x48
    };
};
DECOMP_SIZE_ASSERT(BeepTDF::Record, 0x50);
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }

    T* _Object;                        // 0x00
};
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

extern "C" int __cdecl _strnicmp(const char*, const char*, unsigned int); // src proto-pdb:default.pdb _strnicmp

void* operator new(unsigned int size, const char* file, int line);
struct Matrix;               // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb
class AnimationResource;     // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
public:
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    void RotateJ(float);

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

struct AABox { // NAME: proto-pdb:default.pdb
public:
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

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
    const Vector3& SetLocation(const Vector3& loc);
    Vector3 GetFacing() const;
    void SetScale(float f);
    const SwampString& GetName() const { return _Name; }
    SwampScript* GetScript() const;
    unsigned int GetZone() const;
    void SetOption(unsigned int, bool, bool);
protected:
    void InvalidateCachedWorldPos();
public:
    int _Type;                         // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x34 - 0x24];        // 0x24
    Vector3 _Location;                 // 0x34
    Quaternion _Rotation;              // 0x40
    Vector3 _Scale;                    // 0x50
    char field_5C[0x6C - 0x5C];        // 0x5C
    AABox _BoundingBox;                // 0x6C
    char field_84[0xA4 - 0x84];        // 0x84
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C

    char field_A4[0xAC - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0xB4 - 0xAC];        // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);

    char field_B4[0xC8 - 0xB4];        // 0xB4
    Vector3 _worldPosLast;             // 0xC8
    char field_D4[0x200 - 0xD4];       // 0xD4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    virtual void ReturnToHome();                                   // vtable+0x44
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C
    void PlaySound(const SwampString& name, unsigned int flags, bool bAttach, float minDist3D, float maxDist3D);
    const Vector3& GetVelocity() const { return _Velocity; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }

    char field_200[0x20C - 0x200];     // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x234 - 0x218];     // 0x218
    Vector3 _homePosition;             // 0x234
    Quaternion _homeRotation;          // 0x240
    char field_250[0x2C8 - 0x250];     // 0x250
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

// Retail layout: retail disasm puts _ridingTrolley at 0x317, _targetLocation at 0x3A4, _vehicleName at 0x548
// (proto PDB 0x307, 0x390, 0x518; Vince 0x598); the spans between are opaque.
class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetTargetLocation(const SwampString& s, bool istrolley);
    void SetVehicleName(const SwampString& name);

    char field_2C8[0x317 - 0x2C8];     // 0x2C8
    bool _ridingTrolley;               // 0x317
    char field_318[0x3A4 - 0x318];     // 0x318
    SwampString _targetLocation;       // 0x3A4
    char field_3C0[0x548 - 0x3C0];     // 0x3C0
    SwampString _vehicleName;          // 0x548
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString& name, unsigned int flags, SwampSceneNode* parent, bool bAttach,
                          float minDist3D, float maxDist3D);
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

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

// FUNCTION: USA 0x000181FB
// NAME: proto-pdb:default.pdb
// ??_G?$ResourceHandle<TextureResource>: compiler-generated, emitted by pin_emit_ResourceHandle_TextureResource_delete (5.3 T6.c12)
template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);
    T& operator()() { return *_Resource; }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x2C - 0x24];        // 0x24
    float _length;                     // 0x2C
    char field_30[0x34 - 0x30];        // 0x30
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x60 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(ModelResource, 0x60);

class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(VertexShaderResource, 0x30);

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    std::deque<T> _queue;              // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

class MultiModel { // NAME: proto-pdb:default.pdb
public:
    SwampShuffleQueue<ResourceHandle<ModelResource> > _modelQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiModel, 0x14);

#ifdef NON_MATCHING
#endif

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    ~AnimationControl();
    void SubRef();
    void SetAnimationSpeed(float);
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim._Resource->_length; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x2C - 0x28];        // 0x28
    float _speed;                      // 0x2C
    char field_30[0x32 - 0x30];        // 0x30
    bool _forward;                     // 0x32
    char field_33[0x38 - 0x33];        // 0x33
    float _time;                       // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

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
    void RegisterTimerEvent(unsigned int, float, bool);
    void RemoveTimerEvents();

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

// FUNCTION: USA 0x00018673
// PROTOTYPE: PREVIEW 0x004078CA identical
// NAME: proto-pdb:default.pdb
class AreaStats { // NAME: proto-pdb:default.pdb
public:
    int _clustersRemaining;            // 0x00
    int _clustersFound;                // 0x04
    int _jugRemaining;                 // 0x08
    int _jugMax;                       // 0x0C
    int _pagesFound;                   // 0x10
    int _pagesRemaining;               // 0x14
    int _boxesFound;                   // 0x18
    int _boxesMax;                     // 0x1C
    int _juneBugsFound;                // 0x20
    int _juneBugsMax;                  // 0x24
    int _whammysFound;                 // 0x28
    int _whammysRemaining;             // 0x2C
    SwampString _name;                 // 0x30
};
DECOMP_SIZE_ASSERT(AreaStats, 0x4C);

// !FAKE: c1xx front end; scaffold delete; retail emits ??_GAreaStats@@QAEPAXI@Z in this unit (no matched function
// deletes an AreaStats); this delete-expression emits it instead (matched_set [[emitter]] row)
void pin_emit_AreaStats_delete(AreaStats* p)
{
    delete p;
}

// FUNCTION: USA 0x00018B19
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<AreaStats, std::allocator<AreaStats> >(AreaStats*, AreaStats*, std::allocator<AreaStats>&,
                                                                        std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x00018696
// PROTOTYPE: PREVIEW 0x004078ED identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // wall-fold-va-callers (C0233): match_one 100, but 0x00018696 also folds ~DamageSourceInfo, which matched DinoBoss 0x0008766B and SafeHookRope 0x0007E64B call as an external (4.3 T13)
BeepTDF::Record::~Record()
{
}
#endif

// FUNCTION: USA 0x000186B2
// PROTOTYPE: PREVIEW 0x00407909 identical
// NAME: proto-pdb:default.pdb
class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    int _RefCount;                     // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    char field_4[0x40 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

class CritterCollision { // NAME: proto-pdb:default.pdb
public:
    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};
DECOMP_SIZE_ASSERT(CritterCollision, 0x8);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
protected:
    virtual void SetState(int);                                    // vtable+0x6C
public:
    virtual void Load(const BeepTDF&);
    virtual void Message(SwampScript*, int, void*);
    Critter* GetCritter() { return (Critter*)_node; }
    CritterCollision MoveMe(float);
    void UpdateWander();

    int _state;                        // 0x4C
    Vector3 _wanderPatrolCenter;       // 0x50
    float _wanderPatrolRadius;         // 0x5C
    float _wanderLeashRadius;          // 0x60
    float _wanderLeashMargin;          // 0x64
    int _wanderMode;                   // 0x68
    Vector3 _wanderDir;                // 0x6C
    Vector3 _wanderDest;               // 0x78
    SwampString _wanderChaseTarget;    // 0x84
    float _wanderSpeed;                // 0xA0
    float _wanderTurnRate;             // 0xA4
    char field_A8[0xBC - 0xA8];        // 0xA8
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1CritterMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_CritterMain_dtor(CritterMain* p)
{
    p->CritterMain::~CritterMain();
}

// FUNCTION: USA 0x000195A1
// PROTOTYPE: PREVIEW 0x00408812 identical
// NAME: proto-pdb:default.pdb
class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void DeathFX();                                        // vtable+0x70
    virtual Vector3 GetAttackVectorStraight();                     // vtable+0x74
    virtual Vector3 GetAttackVectorRight();                        // vtable+0x78
    virtual Vector3 GetAttackVectorLeft();                         // vtable+0x7C
    virtual float GetAttackVectorTolerance();                      // vtable+0x80
    void UpdateKnockBack();
    virtual void Message(SwampScript*, int, void*);

    ResourceHandle<AnimationResource> _animDamage; // 0xBC
    MultiAnimation _multiAnimScared;   // 0xC0
    MultiAnimation _multiAnimDamageSpin; // 0xD4
    MultiAnimation _multiAnimDamagePunch; // 0xE8
    MultiAnimation _multiAnimDamageDeath; // 0xFC
    MultiAnimation _multiAnimDamageHeadSlam; // 0x110
    MultiAnimation _multiAnimFlatten;  // 0x124
    MultiAnimation _multiAnimFrozen;   // 0x138
    MultiAnimation _multiAnimFrozenRecover; // 0x14C
    MultiAnimation _multiAnimTwister;  // 0x160
    MultiAnimation _multiAnimToxic;    // 0x174
    MultiAnimation _multiAnimJackhammer; // 0x188
    MultiAnimation _multiAnimPoked;    // 0x19C
    ResourceHandle<ModelResource> _modelDisintegrate; // 0x1B0
    MultiAnimation _multiAnimDisintegrate; // 0x1B4
    ResourceHandle<ModelResource> _modelCutHorizontal; // 0x1C8
    MultiAnimation _multiAnimCutHorizontal; // 0x1CC
    ResourceHandle<ModelResource> _modelCutVertical; // 0x1E0
    MultiAnimation _multiAnimCutVertical; // 0x1E4
    MultiModel _multiModelGiblets;     // 0x1F8
    MultiAnimation _multiAnimCelebrate; // 0x20C
    std::map<SwampString, SwampString> _texRemaps; // 0x220
    bool _kickClusters;                // 0x22C
    float _knockBackAmount;            // 0x230
    float _knockBackTimer;             // 0x234
    int _whammyType;                   // 0x238
    ResourceHandle<ModelResource> _modelRestore; // 0x23C
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1MonsterMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MonsterMain_dtor(MonsterMain* p)
{
    p->MonsterMain::~MonsterMain();
}

typedef std::_Tmap_traits<SwampString, SwampString, std::less<SwampString>,
                          std::allocator<std::pair<const SwampString, SwampString> >, false>
    TexRemapTraits;

// FUNCTION: USA 0x00018E1A
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): callee ??_G_Node (destroy) at retail 0x000186CC has no symbol; needs that pin (our build also emits ??_G_Node here)
template void std::_Tree<TexRemapTraits>::_Erase(std::_Tree_nod<TexRemapTraits>::_Node*);
#endif

// FUNCTION: USA 0x00019905
// NAME: proto-pdb:default.pdb
// ??_GOnionMain (alias ??_EOnionMain) unmatched, no draft; wall: a virtual ??_G is emitted only with the class vtable, i.e.
// with its ctor ??0OnionMain 0x000196CA (0x1EB, not matched, not a row here) (C0320); match_one: `not defined in Onion.obj`
// FUNCTION: USA 0x00019921
// NAME: proto-pdb:default.pdb
class OnionMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_PATROL = 1,
        STATE_SNEAK = 2,
        STATE_DAMAGE = 3,
        STATE_BOUNCEBACK = 4
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_TRAPSTOP = 0x14,
        MESSAGE_SET_UNDER_TRAP = 0x15,
        MESSAGE_QUERY_UNDER_TRAP = 0x16
    };
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_BORED = 0
    };
    OnionMain(SwampSceneNode* node);
    virtual void FrameTick();
    virtual void AnimationFinished();
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void TimerEvent(unsigned int timerId);
    virtual void Load(const BeepTDF& tdf);
    virtual void SetState(int state);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    ResourceHandle<AnimationResource> _animIdle; // 0x240
    ResourceHandle<AnimationResource> _animDamage; // 0x244
    ResourceHandle<AnimationResource> _animRun; // 0x248
    ResourceHandle<AnimationResource> _animSneaking; // 0x24C
    bool _underTrap;                   // 0x250
    char field_251[0x254 - 0x251];     // 0x251
    int _idleCount;                    // 0x254
};
DECOMP_SIZE_ASSERT(OnionMain, 0x258);

typedef std::_Tmap_traits<SwampString, SwampSceneNode* (*)(), std::less<SwampString>,
                          std::allocator<std::pair<const SwampString, SwampSceneNode* (*)()> >, false>
    NodeMapTraits;

// FUNCTION: USA 0x0001850E
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): only masked target is its own __ehhandler$ funclet (retail 0x00152AF6), unnamed; needs the funclet pinned (C0331), not a source lever
template std::_Tree<NodeMapTraits>::_Nodeptr std::_Tree<NodeMapTraits>::_Buynode();
#endif

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1OnionMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_OnionMain_dtor(OnionMain* p)
{
    p->OnionMain::~OnionMain();
}

class TrapOnionMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_UP = 0,
        STATE_DOWN = 1,
        STATE_FALLING = 2,
        STATE_LIFTING = 3
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_QUERY_ONIONS_REMAINING = 0x14
    };
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_DONE = 0
    };
    TrapOnionMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int timerId);
    virtual void SetState(int state);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

    ResourceHandle<AnimationResource> _animIdle; // 0xBC
    ResourceHandle<AnimationResource> _animUp; // 0xC0
    ResourceHandle<AnimationResource> _animDown; // 0xC4
    int _onionsRemaining;              // 0xC8
    SwampSceneNode* _hud;              // 0xCC
};
DECOMP_SIZE_ASSERT(TrapOnionMain, 0xD0);

class OnionTrapRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    OnionTrapRegionMain(SwampSceneNode* node);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(OnionTrapRegionMain, 0x4C);

// FUNCTION: USA 0x00018226
// PROTOTYPE: PREVIEW 0x004074E4 identical
// NAME: proto-pdb:default.pdb
SwampScript* OnionTrapRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\belowdecks\\Onion.h", 126) OnionTrapRegionMain(node);
}

// FUNCTION: USA 0x000186F0
// NAME: proto-pdb:default.pdb
void OnionTrapRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (_strnicmp(node->GetName().c_str(), "onion", 5) == 0)
        node->GetScript()->Message(this, OnionMain::MESSAGE_SET_UNDER_TRAP, (void*)true);
}

// FUNCTION: USA 0x00018732
// NAME: proto-pdb:default.pdb
void OnionTrapRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (_strnicmp(node->GetName().c_str(), "onion", 5) == 0)
        node->GetScript()->Message(this, OnionMain::MESSAGE_SET_UNDER_TRAP, (void*)false);
}

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
double RandomFloat(float min, float max);

// FUNCTION: USA 0x00018010
// PROTOTYPE: PREVIEW 0x004072DF identical
// NAME: proto-pdb:default.pdb
double RandomFloat(float min, float max)
{
    return (double)Random() * (max - min) * (1.0 / 4294967296.0) + min;
}
}

// FUNCTION: USA 0x0001803C
// PROTOTYPE: PREVIEW 0x0040730B identical
// NAME: proto-pdb:default.pdb
const Vector3& SwampSceneNode::SetLocation(const Vector3& loc)
{
    InvalidateCachedWorldPos();
    _Location = loc;
    return _Location;
}

// FUNCTION: USA 0x00018056
// PROTOTYPE: PREVIEW 0x00407325 identical
// NAME: proto-pdb:default.pdb
Vector3 SwampSceneNode::GetFacing() const
{
    // !FAKE: inliner; dummy local v0 for the by-value Apply; retail inlines it, /O1 keeps it out of line (T4.c1 E.GF 100.0)
    Vector3 v0;
    _Rotation.Apply(Vector3(0.0f, 0.0f, 1.0f), v0);
    return v0;
}

// FUNCTION: USA 0x0001808E
// PROTOTYPE: PREVIEW 0x0040735D identical
// NAME: proto-pdb:default.pdb
const AABox& SwampSceneNode::GetBounds() const
{
    return _BoundingBox;
}

// FUNCTION: USA 0x00018092
// PROTOTYPE: PREVIEW 0x00407361 identical
// NAME: proto-pdb:default.pdb
void SwampSceneNode::SetScale(float f)
{
    _Scale.x = _Scale.y = _Scale.z = f;
    InvalidateCachedWorldPos();
}

// FUNCTION: USA 0x000180A7
// NAME: proto-pdb:default.pdb
void AnimationControl::SetAnimationSpeed(float speed)
{
    if (speed != 0.0f)
        _forward = speed > 0.0f;
    _speed = fabs(speed);
}

// FUNCTION: USA 0x000180E8
// PROTOTYPE: PREVIEW 0x004073B7 identical
// NAME: proto-pdb:default.pdb
void Critter::PlaySound(const SwampString& name, unsigned int flags, bool bAttach, float minDist3D, float maxDist3D)
{
    SoundManager::PlaySound(name, flags, this, bAttach, minDist3D, maxDist3D);
}

// FUNCTION: USA 0x00018129
// NAME: proto-pdb:default.pdb
void Critter::ReturnToHome()
{
    SwampSceneNode::SetWorldPosition(_homePosition);
    InvalidateCachedWorldPos();
    _Rotation = _homeRotation;
    _worldPosLast = _homePosition;
    SetOption(2, false, false);
    _Velocity = Vector3(0.0f, 0.0f, 0.0f);
}

// FUNCTION: USA 0x00018190
// PROTOTYPE: PREVIEW 0x00478846 identical
// NAME: proto-pdb:default.pdb
void CritterMain::SetState(int state)
{
    _state = state;
}

// FUNCTION: USA 0x0001819A
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match; gate relink REFUSED (our ranges overlap at 0x0001819A)
template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

template ResourceHandle<VertexShaderResource>& ResourceHandle<VertexShaderResource>::operator=(
    const ResourceHandle<VertexShaderResource>& h);
#endif

// !FAKE: c1xx front end; scaffold delete; retail emits ??_G?$ResourceHandle@VTextureResource@@@@QAEPAXI@Z in this unit
// (no matched function deletes a ResourceHandle); this delete-expression emits it instead (matched_set [[emitter]] row)
void pin_emit_ResourceHandle_TextureResource_delete(ResourceHandle<TextureResource>* p)
{
    delete p;
}

// FUNCTION: USA 0x0001821B
// PROTOTYPE: PREVIEW 0x004074D9 identical
// NAME: proto-pdb:default.pdb
class Sprite { // NAME: proto-pdb:default.pdb
public:
    void Render() const;

    char field_0[0x18];                // 0x00
    ResourceHandle<TextureResource> _texture; // 0x18
    char field_1C[0x4C - 0x1C];        // 0x1C
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1Sprite@@QAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Sprite_dtor(Sprite* p)
{
    p->Sprite::~Sprite();
}

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    void FindAllMatching(std::vector<Critter*, std::allocator<Critter*> >& matches, const SwampString& name, unsigned int zone);

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xCE - 0xA4];        // 0xA4
    bool _deleteWhenDone;              // 0xCE
    char field_CF[0xE8 - 0xCF];        // 0xCF
};
DECOMP_SIZE_ASSERT(FireFlyNode, 0xE8);

class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    FireFlyNode* Add(const SwampString&, const SwampString&, unsigned int, bool);
};

extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

class Onion : public Critter { // NAME: proto-pdb:default.pdb
public:
    virtual bool DoesJorgeRepelMe(const SwampSceneNode* jorgeNode, bool bHorizontal) const;
};

// FUNCTION: USA 0x0001827A
// PROTOTYPE: PREVIEW 0x00407538 identical
// NAME: proto-pdb:default.pdb
bool Onion::DoesJorgeRepelMe(const SwampSceneNode* jorgeNode, bool bHorizontal) const
{
    if (jorgeNode == pManager->_vinceCache)
        return true;
    return Critter::DoesJorgeRepelMe(jorgeNode, bHorizontal);
}

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetGravity() const { return Gravity; }

    char field_0[0x8C];                // 0x00
    float Gravity;                     // 0x8C
    char field_90[0x6C4 - 0x90];       // 0x90
    float _onionPatrolRadius;          // 0x6C4
    float _onionTurnRate;              // 0x6C8
    float _onionIdleTimeMin;           // 0x6CC
    float _onionIdleTimeMax;           // 0x6D0
    float _onionPatrolTimeMin;         // 0x6D4
    float _onionPatrolTimeMax;         // 0x6D8
    char field_6DC[0x6E0 - 0x6DC];     // 0x6DC
    float _onionTrapDownTime;          // 0x6E0
    float _onionWanderSpeed;           // 0x6E4
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000180E1
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match (masked 0); precheck ADD REFUSED: pin has no size in symbols_pin.txt (C0377 unsized pin)
float Critter::GetGravity()
{
    return gSettings.GetGravity();
}
#endif

// FUNCTION: USA 0x0001829A
// PROTOTYPE: PREVIEW 0x00407558 identical
// NAME: proto-pdb:default.pdb
void OnionMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 0:
        MonsterMain::Message(source, message, data);
        SetState(STATE_DAMAGE);
        break;
    case 6:
        RegisterFrameTick();
        _wanderPatrolRadius = gSettings._onionPatrolRadius;
        _wanderLeashRadius = gSettings._onionPatrolRadius;
        _wanderLeashMargin = 0.0f;
        _wanderPatrolCenter = GetCritter()->_homePosition;
        _wanderTurnRate = gSettings._onionTurnRate;
        _wanderSpeed = gSettings._onionWanderSpeed;
        SetState(STATE_IDLE);
        break;
    case 9:
    case 10:
        CritterMain::Message(source, message, data);
        break;
    case MESSAGE_TRAPSTOP:
        SetState(STATE_IDLE);
        break;
    case MESSAGE_SET_UNDER_TRAP:
        // !FAKE: c1xx front end; cast; (bool)data emits cmp/setne, retail copies the low byte (match_one 83.70 -> 92.08)
        _underTrap = *(bool*)&data;
        break;
    case MESSAGE_QUERY_UNDER_TRAP:
        *(bool*)data = _underTrap;
        break;
    default:
        MonsterMain::Message(source, message, data);
        break;
    }
}

// FUNCTION: USA 0x0001837B
// PROTOTYPE: PREVIEW 0x00407639 identical
// NAME: proto-pdb:default.pdb
void OnionMain::TimerEvent(unsigned int timerId)
{
    switch (_state) {
    case STATE_IDLE:
        SetState(STATE_PATROL);
        break;
    case STATE_PATROL:
        SetState(STATE_IDLE);
        break;
    }
}

// FUNCTION: USA 0x00018394
// PROTOTYPE: PREVIEW 0x00407652 identical
// NAME: proto-pdb:default.pdb
void OnionMain::Load(const BeepTDF& tdf)
{
    CritterMain::Load(tdf);
    GetCritter()->ReturnToHome();
    SetState(STATE_PATROL);
}

// FUNCTION: USA 0x0001840B
// PROTOTYPE: PREVIEW 0x004076C9 identical
// NAME: proto-pdb:default.pdb
void TrapOnionMain::TimerEvent(unsigned int timerId)
{
    switch (_state) {
    case STATE_DOWN:
        SetState(STATE_LIFTING);
        break;
    }
}

// FUNCTION: USA 0x00018438
// PROTOTYPE: PREVIEW 0x004076D9 identical
// NAME: proto-pdb:default.pdb
void AnimationControl::SubRef()
{
    if (--_RefCount == 0)
        delete this;
}

// FUNCTION: USA 0x0001844E
// PROTOTYPE: PREVIEW 0x004076EF identical
// NAME: proto-pdb:default.pdb
void OnionMain::FrameTick()
{
    switch (_state) {
    case STATE_IDLE: {
        Vector3 vel = GetCritter()->GetVelocity();
        vel.z = 0.0f;
        vel.x = 0.0f;
        GetCritter()->SetVelocity(vel);
        break;
    }
    case STATE_PATROL:
        UpdateWander();
        break;
    case STATE_DAMAGE:
        UpdateKnockBack();
        break;
    }
    MoveMe(GetCritter()->GetGravity());
}

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000185FF
// PROTOTYPE: PREVIEW 0x00407856 identical
// NAME: proto-pdb:default.pdb
void OnionMain::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    if (value > 0.0f && name == "step") {
        // !FAKE: c2 register allocation; neutral stand-in local; retail loads the node before the string (match_one 88.64 -> 100)
        Critter* p = GetCritter();
        // !FAKE: c1xx front end; neutral stand-in local, comma expression; retail builds the string before the float arguments (match_one 63.61 -> 88.64)
        const SwampString* v1;
        p->PlaySound((v1 = &format("bsh_ostep%02d", BeepLib::Random() % 6 + 1), *v1), 0x28, true, 0.0f, 0.0f);
    }
}

// FUNCTION: USA 0x000187B9
// PROTOTYPE: PREVIEW 0x00407A10 identical
// NAME: proto-pdb:default.pdb
void OnionMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_IDLE:
        GetCritter()->StartAnimation(_animIdle, true, false);
        if (--_idleCount <= 0) {
            GetCritter()->PlaySound("bsh_oidle", 0x28, true, 0.0f, 0.0f);
            _idleCount = (BeepLib::Random() & 3) + 2;
        }
        RegisterTimerEvent(TIMER_BORED, BeepLib::RandomFloat(gSettings._onionIdleTimeMin, gSettings._onionIdleTimeMax), false);
        break;
    case STATE_PATROL: {
        _wanderMode = 1;
        Vector3 newdir = _node->GetFacing();
        newdir.RotateJ(BeepLib::RandomFloat(-1.5707964f, 1.5707964f));
        _wanderDir = newdir;
        GetCritter()->StartAnimation(_animRun, true, false);
        RegisterTimerEvent(TIMER_BORED, BeepLib::RandomFloat(gSettings._onionPatrolTimeMin, gSettings._onionPatrolTimeMax), false);
        break;
    }
    case STATE_DAMAGE:
        RemoveTimerEvents();
        GetCritter()->StartAnimation(_animDamage, false, false);
        break;
    }
}

// FUNCTION: USA 0x00019068
// PROTOTYPE: PREVIEW 0x004082AD identical
// NAME: proto-pdb:default.pdb
void TrapOnionMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_UP:
        GetCritter()->StartAnimation(_animIdle, true, false);
        break;
    case STATE_DOWN: {
        RegisterTimerEvent(TIMER_DONE, gSettings._onionTrapDownTime, false);
        ResourceHandle<AnimationControl> ach = GetCritter()->StartAnimation(_animDown, false, false);
        ach().SetAnimationTime(ach().GetAnimationLength());
        ach().SetAnimationSpeed(0.0f);
        std::vector<Critter*, std::allocator<Critter*> > onions;
        pManager->FindAllMatching(onions, "onion", 0);
        std::vector<Critter*, std::allocator<Critter*> >::iterator it;
        for (it = onions.begin(); it != onions.end(); ++it) {
            bool intrap;
            // !FAKE: c2 register allocation; dummy local p; retail loads *it once into edi for the body (T9.c7)
            Critter* p = *it;
            p->GetScript()->Message(this, OnionMain::MESSAGE_QUERY_UNDER_TRAP, &intrap);
            if (intrap) {
                // !FAKE: c2 register allocation; stand-in local v1; retail loads g_particleManager before GetZone and spills it (T9.c7)
                SwampParticleManager* v1;
                FireFlyNode* node = (v1 = g_particleManager)->Add("", "belowDecksCapture02", p->GetZone(), true);
                node->SetLocation(p->GetWorldPosition());
                node->_deleteWhenDone = true;
                p->Invalidate();
                _onionsRemaining--;
            }
        }
        break;
    }
    case STATE_FALLING:
        GetCritter()->StartAnimation(_animDown, false, false);
        break;
    case STATE_LIFTING:
        GetCritter()->StartAnimation(_animUp, false, false);
        break;
    }
}

// FUNCTION: USA 0x00019513
// PROTOTYPE: PREVIEW 0x00408784 identical
// NAME: proto-pdb:default.pdb
SwampScript* TrapOnionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (a header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\belowdecks\\Onion.h", 75) TrapOnionMain(node);
}

class HUDUI;                 // NAME: proto-pdb:default.pdb
class Level;                 // NAME: proto-pdb:default.pdb
class LevelStats;            // NAME: proto-pdb:default.pdb
class LoadScreen;            // NAME: proto-pdb:default.pdb
typedef std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > SwampWString;

// FUNCTION: USA 0x0001841B
// PROTOTYPE: PREVIEW 0x00406C67 identical
// NAME: proto-pdb:default.pdb
template void std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> >::_Eos(unsigned int);

// FUNCTION: USA 0x000184CB
// PROTOTYPE: PREVIEW 0x00406CA8 identical
// NAME: proto-pdb:default.pdb
template void std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> >::_Tidy(bool,unsigned int);

class Diary { // NAME: proto-pdb:default.pdb
public:
    ~Diary();

    char field_0[0x14C];               // 0x00
};
DECOMP_SIZE_ASSERT(Diary, 0x14C);

// FUNCTION: USA 0x00019B5F
// PROTOTYPE: PREVIEW 0x00408DD0 identical
// NAME: proto-pdb:default.pdb
class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    // FUNCTION: USA 0x00019565
    // PROTOTYPE: PREVIEW 0x004087D6 identical
    // NAME: proto-pdb:default.pdb
    struct Slot { // NAME: proto-pdb:default.pdb
        SwampString level;             // 0x00
        SwampString area;              // 0x1C
        char field_38[0x44 - 0x38];    // 0x38
        std::vector<std::pair<SwampString, SwampString>, std::allocator<std::pair<SwampString, SwampString> > >
            _chaptersFinished;         // 0x44
        SwampString _savePath;         // 0x54
        SwampWString _gameName;        // 0x70
    };

    HUDUI* _hudUI;                     // 0x00
    Level* _curLevel;                  // 0x04
    SwampString _requestLevel;         // 0x08
    SwampString _requestArea;          // 0x24
    char field_40[0x4C - 0x40];        // 0x40
    std::vector<Slot, std::allocator<Slot> > _slots; // 0x4C
    Diary _currSave;                   // 0x5C
    char field_1A8[0x1B0 - 0x1A8];     // 0x1A8
    std::vector<LevelStats, std::allocator<LevelStats> > _levelStatsList; // 0x1B0
    std::map<SwampString, SwampSceneNode*> _regionScriptNodeList; // 0x1C0
};
DECOMP_SIZE_ASSERT(LevelLoader::Slot, 0x8C);
DECOMP_SIZE_ASSERT(LevelLoader, 0x1CC);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1Slot@LevelLoader@@QAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Slot_dtor(LevelLoader::Slot* p)
{
    p->LevelLoader::Slot::~Slot();
}

class SwampNodeManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampSceneNode* (*CreationFunc)();
    SwampNodeManager();
    ~SwampNodeManager();

    std::map<SwampString, CreationFunc> _nodes; // 0x00
    std::vector<SwampSceneNode*, std::allocator<SwampSceneNode*> > _deadNodes; // 0x0C
};
DECOMP_SIZE_ASSERT(SwampNodeManager, 0x1C);

// FUNCTION: USA 0x000196B3
// PROTOTYPE: PREVIEW 0x00408924 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match; gate relink REFUSED (unreferenced inline _Tidy@_Tree external, C0126)
SwampNodeManager::SwampNodeManager()
{
}
#endif

// FUNCTION: USA 0x000198B5
// PROTOTYPE: PREVIEW 0x00408B26 identical
// NAME: proto-pdb:default.pdb
Vector3 MonsterMain::GetAttackVectorStraight()
{
    return Vector3(0.0f, 0.0f, 1.0f);
}

// FUNCTION: USA 0x000198CA
// PROTOTYPE: PREVIEW 0x00408B3B identical
// NAME: proto-pdb:default.pdb
Vector3 MonsterMain::GetAttackVectorRight()
{
    return Vector3(0.7071f, 0.0f, 0.7071f);
}

// FUNCTION: USA 0x000198E3
// NAME: proto-pdb:default.pdb
Vector3 MonsterMain::GetAttackVectorLeft()
{
    return Vector3(-0.7071f, 0.0f, 0.7071f);
}

// FUNCTION: USA 0x000198FE
// NAME: proto-pdb:default.pdb
float MonsterMain::GetAttackVectorTolerance()
{
    return 0.9f;
}

class LevelStats { // NAME: proto-pdb:default.pdb
public:
    ~LevelStats();

    SwampString _name;                 // 0x00
    std::vector<AreaStats, std::allocator<AreaStats> > _areaStatsList; // 0x1C
};
DECOMP_SIZE_ASSERT(LevelStats, 0x2C);

// FUNCTION: USA 0x0001995A
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<LevelStats, std::allocator<LevelStats> >(LevelStats*, LevelStats*,
                                                                          std::allocator<LevelStats>&,
                                                                          std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000194FB
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0 + bytes match (masked 0); relink: our objects and a cut object both define
// ?_Tidy@?$vector@VAreaStats@@... (0x00019214, a fail row `library: std:: name`); needs _Tidy matched with it
LevelStats::~LevelStats()
{
}
#endif

// FUNCTION: USA 0x0001999E
// PROTOTYPE: PREVIEW 0x00408C0F identical
// NAME: proto-pdb:default.pdb
SwampScript* OnionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\belowdecks\\Onion.h", 9) OnionMain(node);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1LevelLoader@@QAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LevelLoader_dtor(LevelLoader* p)
{
    p->LevelLoader::~LevelLoader();
}

// FUNCTION: USA 0x00018921
// PROTOTYPE: PREVIEW 0x00407B78 identical
// NAME: proto-pdb:default.pdb
void OnionMain::AnimationFinished()
{
    switch (_state) {
    case STATE_DAMAGE:
        _wanderPatrolCenter = _node->GetWorldPosition();
        if ((BeepLib::Random() & 1) == 0)
            GetCritter()->PlaySound("bsh_ofright", 0x28, true, 0.0f, 0.0f);
        else
            GetCritter()->PlaySound("bsh_ocall", 0x28, true, 0.0f, 0.0f);
        SetState(STATE_PATROL);
        break;
    }
}

class HUDUI { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xE4];                // 0x00
    bool _letterboxEnabled;            // 0xE4

private:
    static HUDUI* _self;
    friend class OnionHud;
};

class CXBFont { // NAME: proto-pdb:default.pdb
public:
    long DrawText(float, float, unsigned long, const SwampString&, unsigned long, float);
};

extern "C" int __cdecl sprintf(char*, const char*, ...); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00018A38
// NAME: proto-pdb:default.pdb
// ??1OnionHud: implicit, emitted by pin_emit_OnionHud_dtor (5.3 T6.c12)
class OnionHud : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Render(const Matrix& m) const;

    CXBFont* _font;                    // 0xA4
    Sprite _icon;                      // 0xA8
};
DECOMP_SIZE_ASSERT(OnionHud, 0xF4);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1OnionHud@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_OnionHud_dtor(OnionHud* p)
{
    p->OnionHud::~OnionHud();
}

// FUNCTION: USA 0x00018A4A
// PROTOTYPE: PREVIEW 0x00407C8F identical
// NAME: proto-pdb:default.pdb
void OnionHud::Render(const Matrix& m) const
{
    if (!HUDUI::_self->_letterboxEnabled) {
        char buffer[128];
        int onionsRemaining;
        // !FAKE: c1xx front end; dummy local v0; retail destroys the "traponion" temporary before GetScript (match_one 83.46 -> 100.0)
        Critter* v0 = pManager->Find("traponion");
        v0->GetScript()->Message(0, TrapOnionMain::MESSAGE_QUERY_ONIONS_REMAINING, &onionsRemaining);
        _icon.Render();
        sprintf(buffer, "%i", onionsRemaining);
        _font->DrawText(120.0f, 400.0f, 0xFFFFFFFF, buffer, 0, 0.0f);
    }
}

// FUNCTION: USA 0x00018B34
// PROTOTYPE: PREVIEW 0x00407D79 near-identical
// NAME: proto-pdb:default.pdb
void Vince::SetTargetLocation(const SwampString& s, bool istrolley)
{
    _targetLocation = s;
    _ridingTrolley = istrolley;
}

// FUNCTION: USA 0x00018B58
// PROTOTYPE: PREVIEW 0x00407D9D near-identical
// NAME: proto-pdb:default.pdb
void Vince::SetVehicleName(const SwampString& name)
{
    _vehicleName = name;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1); wall: callee ~pair<string,string> folds at 0x00018696
// (pins ~DamageSourceInfo, ~BeepTDF::Record) with no retail name of its own: needs a pin alias (config)
// FUNCTION: USA 0x00018B88
// PROTOTYPE: PREVIEW 0x00407DCD identical
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > &,std::_Nonscalar_ptr_iterator_tag);
#endif
