// UNIT: src/Apps/Vince/scripts/french_quarter/Blender.cpp OBJ: Xbox\OfficialRelease\Blender.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

class Vector3 { // NAME: proto-pdb:default.pdb
public:
    void Zero() { x = 0.0f; y = 0.0f; z = 0.0f; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class Quaternion { // NAME: proto-pdb:default.pdb
public:
    void Identity() { v.Zero(); w = 1.0f; }

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class VinceGiblets { // NAME: proto-pdb:default.pdb
public:
    struct Giblet { // NAME: proto-pdb:default.pdb
        Giblet() {}
        void DeActivate();

        Vector3 loc;                   // 0x00
        Quaternion rot;                // 0x0C
        Vector3 velocity;              // 0x1C
        Vector3 rotaxis;               // 0x28
        Vector3 start;                 // 0x34
        Vector3 current;               // 0x40
        bool active;                   // 0x4C
    };

    virtual ~VinceGiblets();          // vtable+0x00
    void StopEmitting();

    char field_4[0x210 - 0x4];         // 0x004
    bool _emitting;                    // 0x210
};
DECOMP_SIZE_ASSERT(VinceGiblets::Giblet, 0x50);
DECOMP_SIZE_ASSERT(VinceGiblets, 0x214);

// FUNCTION: USA 0x000775B1
// PROTOTYPE: PREVIEW 0x0046361B identical
// NAME: proto-pdb:default.pdb
void VinceGiblets::StopEmitting()
{
    _emitting = false;
}

// FUNCTION: USA 0x000776F9
// PROTOTYPE: PREVIEW 0x0046373D identical
// NAME: proto-pdb:default.pdb
void VinceGiblets::Giblet::DeActivate()
{
    active = false;
    loc.Zero();
    rot.Identity();
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x10];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    T* _Resource;                      // 0x00
};

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

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

    SwampSceneNode* GetNode() { return _node; }

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)GetNode(); }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0xF8 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

class WeaponMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb

    char field_F8[0x180 - 0xF8];       // 0xF8
};
DECOMP_SIZE_ASSERT(WeaponMain, 0x180);

class BlenderWeaponMain : public WeaponMain { // NAME: proto-pdb:default.pdb
public:
    BlenderWeaponMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BlenderWeaponMain, 0x180);

class BlenderMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    BlenderMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_BC[0xE4 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(BlenderMain, 0xE4);

class VinceShared : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual ~VinceShared();
    char field_BC[0x28];               // 0xBC
};
DECOMP_SIZE_ASSERT(VinceShared, 0xE4);

// FUNCTION: USA 0x00078259
// PROTOTYPE: PREVIEW 0x0046429D identical
// NAME: proto-pdb:default.pdb
class VinceInBlender : public VinceShared { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_POPOUT = 0,
        STATE_SHAKEOFF = 1
    };

    virtual void AnimationFinished();

    ResourceHandle<AnimationResource> _popOut;   // 0xE4
    ResourceHandle<AnimationResource> _shakeoff; // 0xE8
    bool _wasBlended;                  // 0xEC
};
DECOMP_SIZE_ASSERT(VinceInBlender, 0xF0);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1VinceInBlender@@UAE@XZ because this unit defines
// the ctor 0x77FD9 (not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_VinceInBlender_dtor(VinceInBlender* p)
{
    p->VinceInBlender::~VinceInBlender();
}

// FUNCTION: USA 0x000778DB
// PROTOTYPE: PREVIEW 0x0046391F identical
// NAME: proto-pdb:default.pdb
void VinceInBlender::AnimationFinished()
{
    switch (_state) {
    case STATE_POPOUT:
        if (_wasBlended) {
            GetCritter()->StartAnimation(_shakeoff, false, false);
            SetState(STATE_SHAKEOFF);
        } else {
            InvalidateScript();
        }
        break;
    case STATE_SHAKEOFF:
        InvalidateScript();
        break;
    }
}

class Vector4 { // NAME: proto-pdb:default.pdb
public:
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Matrix() {}

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int _RefCount;            // 0x00
};
DECOMP_SIZE_ASSERT(DumbBase, 0x4);

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int _tag;                 // 0x04
    float _pct;                        // 0x08
    const void *_blindData;            // 0x0C
    Vector4 _plane;                    // 0x10
    unsigned char _bone;               // 0x20
    bool _hitPlatform;                 // 0x21
    Vector3 _platformMove;             // 0x24
    Quaternion _platformRot;           // 0x30
};
DECOMP_SIZE_ASSERT(BSPQueryResultObject, 0x40);

typedef unsigned int size_t;
typedef int ptrdiff_t;

inline void *__cdecl operator new(size_t, void *_Where)
{
    return (_Where);
}

namespace std { // NAME: proto-pdb:default.pdb

struct _Nonscalar_ptr_iterator_tag {};

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
    struct rebind {
        typedef allocator<_Other> other;
    };
};

template<class _InIt, class _OutIt> inline
_OutIt _Copy_opt(_InIt _First, _InIt _Last, _OutIt _Dest, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_Dest, ++_First)
        *_Dest = *_First;
    return (_Dest);
}

template<class _BidIt1, class _BidIt2> inline
_BidIt2 _Copy_backward_opt(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Nonscalar_ptr_iterator_tag)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}

template<class _FwdIt, class _Ty> inline
void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}

template<class _FwdIt, class _Diff, class _Tval, class _Alloc>
void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al);

template<class _Ty, class _Alloc>
class _Vector_val {
protected:
    typedef typename _Alloc::template rebind<_Ty>::other _Alty;

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
    };

    size_type capacity() const
    {
        return (_Myfirst == 0 ? 0 : _Myend - _Myfirst);
    }

    iterator begin()
    {
        return (iterator(_Myfirst));
    }

    iterator end()
    {
        return (iterator(_Mylast));
    }

    void resize(size_type _Newsize)
    {
        resize(_Newsize, _Ty());
    }

    void resize(size_type _Newsize, _Ty _Val)
    {
        if (size() < _Newsize)
            _Insert_n(end(), _Newsize - size(), _Val);
        else if (_Newsize < size())
            erase(begin() + _Newsize, end());
    }

    size_type size() const
    {
        return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst);
    }

    iterator erase(iterator _First, iterator _Last);

protected:
    void _Insert_n(iterator _Where, size_type _Count, const _Ty& _Val);

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

// !FAKE: c1xx front end; explicit instantiations of single templates and members; retail emits them through callers
// in this unit (not matched yet); the instantiations emit them instead
// FUNCTION: USA 0x000775B9
// PROTOTYPE: PREVIEW 0x005157E9 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
template vector<Matrix>::size_type vector<Matrix>::size() const;
#endif
// FUNCTION: USA 0x000775CC
// PROTOTYPE: PREVIEW 0x00463623 identical
// NAME: proto-pdb:default.pdb
template vector<VinceGiblets::Giblet>::size_type vector<VinceGiblets::Giblet>::size() const;
// FUNCTION: USA 0x000775E2
// PROTOTYPE: PREVIEW 0x00515814 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
template vector<Matrix>::size_type vector<Matrix>::capacity() const;
#endif
// FUNCTION: USA 0x000775F5
// PROTOTYPE: PREVIEW 0x00463639 identical
// NAME: proto-pdb:default.pdb
template vector<VinceGiblets::Giblet>::size_type vector<VinceGiblets::Giblet>::capacity() const;
// FUNCTION: USA 0x0007760B
// PROTOTYPE: PREVIEW 0x0046364F identical
// NAME: proto-pdb:default.pdb
template void fill<Matrix *, Matrix>(Matrix *, Matrix *, const Matrix&);
// FUNCTION: USA 0x0007762E
// PROTOTYPE: PREVIEW 0x00463672 identical
// NAME: proto-pdb:default.pdb
template void fill<VinceGiblets::Giblet *, VinceGiblets::Giblet>(VinceGiblets::Giblet *, VinceGiblets::Giblet *,
    const VinceGiblets::Giblet&);
// FUNCTION: USA 0x00078D38
// PROTOTYPE: PREVIEW 0x00464D51 identical
// NAME: proto-pdb:default.pdb
template void vector<Matrix>::resize(size_type);
// FUNCTION: USA 0x00078D5F
// PROTOTYPE: PREVIEW 0x00464D78 identical
// NAME: proto-pdb:default.pdb
template void vector<VinceGiblets::Giblet>::resize(size_type);
// FUNCTION: USA 0x00077651
// PROTOTYPE: PREVIEW 0x00463695 identical
// NAME: proto-pdb:default.pdb
template Matrix *_Copy_opt<Matrix *, Matrix *>(Matrix *, Matrix *, Matrix *, _Nonscalar_ptr_iterator_tag);
// FUNCTION: USA 0x0007767B
// PROTOTYPE: PREVIEW 0x004636BF identical
// NAME: proto-pdb:default.pdb
template Matrix *_Copy_backward_opt<Matrix *, Matrix *>(Matrix *, Matrix *, Matrix *, _Nonscalar_ptr_iterator_tag);
// FUNCTION: USA 0x000776A5
// PROTOTYPE: PREVIEW 0x004636E9 identical
// NAME: proto-pdb:default.pdb
template VinceGiblets::Giblet *_Copy_opt<VinceGiblets::Giblet *, VinceGiblets::Giblet *>(VinceGiblets::Giblet *,
    VinceGiblets::Giblet *, VinceGiblets::Giblet *, _Nonscalar_ptr_iterator_tag);
// FUNCTION: USA 0x000776CF
// PROTOTYPE: PREVIEW 0x00463713 identical
// NAME: proto-pdb:default.pdb
template VinceGiblets::Giblet *_Copy_backward_opt<VinceGiblets::Giblet *, VinceGiblets::Giblet *>(
    VinceGiblets::Giblet *, VinceGiblets::Giblet *, VinceGiblets::Giblet *, _Nonscalar_ptr_iterator_tag);
// shares 0x00077A04 with the matched BSPQueryResultObject instantiation below
// FUNCTION: USA 0x00077A04 FOLDED
// PROTOTYPE: PREVIEW 0x00463A48 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
template void _Construct<Matrix, Matrix>(Matrix *, const Matrix&);
#endif
// FUNCTION: USA 0x00077A04 FOLDED
// PROTOTYPE: PREVIEW 0x00463A48 identical
// NAME: proto-pdb:default.pdb
template void _Construct<BSPQueryResultObject, BSPQueryResultObject>(BSPQueryResultObject *,
    const BSPQueryResultObject&);
// FUNCTION: USA 0x00077A1A
// PROTOTYPE: PREVIEW 0x00463A5E identical
// NAME: proto-pdb:default.pdb
template void _Construct<VinceGiblets::Giblet, VinceGiblets::Giblet>(VinceGiblets::Giblet *,
    const VinceGiblets::Giblet&);
// FUNCTION: USA 0x00077F05
// PROTOTYPE: PREVIEW 0x00463F49 identical
// NAME: proto-pdb:default.pdb
template vector<VinceGiblets::Giblet>::pointer vector<VinceGiblets::Giblet>::_Ufill(pointer, size_type,
    const VinceGiblets::Giblet&);
// FUNCTION: USA 0x00078C8B
// PROTOTYPE: PREVIEW 0x00464CA4 identical
// NAME: proto-pdb:default.pdb
template void vector<Matrix>::resize(size_type, Matrix);
// FUNCTION: USA 0x00078CE0
// PROTOTYPE: PREVIEW 0x00464CF9 identical
// NAME: proto-pdb:default.pdb
template void vector<VinceGiblets::Giblet>::resize(size_type, VinceGiblets::Giblet);

}

// FUNCTION: USA 0x00078898
// PROTOTYPE: PREVIEW 0x004648B1 identical
// NAME: proto-pdb:default.pdb
SwampScript* BlenderWeaponMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("bakery\\Blender.cpp", 29) BlenderWeaponMain(node);
}

// FUNCTION: USA 0x00079177
// PROTOTYPE: PREVIEW 0x0046518B identical
// NAME: proto-pdb:default.pdb
SwampScript* BlenderMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\Blender.h", 14) BlenderMain(node);
}
