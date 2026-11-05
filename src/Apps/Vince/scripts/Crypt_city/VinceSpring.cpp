// UNIT: src/Apps/Vince/scripts/Crypt_city/VinceSpring.cpp OBJ: Xbox\OfficialRelease\VinceSpring.obj OPT: unknown
// !FAKE: c2 intrinsics; file-scope pragma; retail inlines fsqrt in VinceSpring::CalcSpeeds under /O1 /Oy (C0213);
// the CRT sqrt declared first so the pragma precedes <math.h> (its sqrt(float) inline then gets fsqrt)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
#include <math.h>
#include "../../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    int compare(const E* _Ptr) const;

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
// VC 7.1 <xutility>/<xmemory>/<memory>/<deque> bodies for the deque rows (C0132); retail calls
// _Uninitialized_copy, _Construct and deque::_Xlen out of line at named fold VAs (0x0007F3FD, 0x000AE5C1,
// 0x0001D5C2): declared only; fill_n (0x00033F35) keeps its inline body, retail calls it out of line anyway
struct _Scalar_ptr_iterator_tag {};
template <class _Ty> inline _Scalar_ptr_iterator_tag _Ptr_cat(_Ty**, _Ty**)
{
    _Scalar_ptr_iterator_tag _Cat;
    return (_Cat);
}
template <class _Ty> inline _Ty* _Allocate(unsigned int _Count, _Ty*)
{
    return ((_Ty*)operator new(_Count * sizeof(_Ty)));
}
template <class _T1, class _T2> void _Construct(_T1* _Ptr, const _T2& _Val);
template <class _Ty> inline void _Destroy(_Ty* _Ptr)
{
    (_Ptr)->~_Ty();
}
template <class _Ty> class allocator {
public:
    typedef _Ty* pointer;
    typedef unsigned int size_type;
    template <class _Other> struct rebind {
        typedef allocator<_Other> other;
    };
    void deallocate(pointer _Ptr, size_type) { operator delete(_Ptr); }
    pointer allocate(size_type _Count) { return (_Allocate(_Count, (pointer)0)); }
    void construct(pointer _Ptr, const _Ty& _Val) { _Construct(_Ptr, _Val); }
    void destroy(pointer _Ptr) { _Destroy(_Ptr); }
    unsigned int max_size() const
    {
        unsigned int _Count = (unsigned int)(-1) / sizeof(_Ty);
        return (0 < _Count ? _Count : 1);
    }
};
template <class _OutIt, class _Diff, class _Ty> inline void fill_n(_OutIt _First, _Diff _Count, const _Ty& _Val)
{
    for (; 0 < _Count; --_Count, ++_First)
        *_First = _Val;
}
template <class _InIt, class _FwdIt, class _Alloc>
_FwdIt _Uninitialized_copy(_InIt _First, _InIt _Last, _FwdIt _Dest, _Alloc& _Al);
template <class _Ty, class _Diff, class _Tval>
inline void _Uninit_fill_n(_Ty* _First, _Diff _Count, const _Tval& _Val, allocator<_Ty>&, _Scalar_ptr_iterator_tag)
{
    fill_n(_First, _Count, _Val);
}
template <class _FwdIt, class _Diff, class _Tval, class _Alloc>
inline void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al)
{
    _Uninit_fill_n(_First, _Count, _Val, _Al, _Ptr_cat(_First, _First));
}
template <class _Ty, class _Alloc>
inline void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al, _Scalar_ptr_iterator_tag)
{
}
template <class _Ty, class _Alloc> inline void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al)
{
    _Destroy_range(_First, _Last, _Al, _Ptr_cat(_First, _Last));
}
template <class _Ty, class _Alloc> class _Deque_map {
protected:
    typedef typename _Alloc::template rebind<_Ty>::other::pointer _Tptr;
    typename _Alloc::template rebind<_Tptr>::other _Almap; // 0x00
};
template <class _Ty, class _Alloc> class _Deque_val : public _Deque_map<_Ty, _Alloc> {
protected:
    typedef typename _Alloc::template rebind<_Ty>::other _Alty;
    _Alty _Alval;                      // 0x01
};
template <class _Ty, class _Ax = allocator<_Ty> > class deque : public _Deque_val<_Ty, _Ax> {
public:
    enum {
        _DEQUEMAPSIZ = 8,
        _DEQUESIZ = sizeof(_Ty) <= 1 ? 16 : sizeof(_Ty) <= 2 ? 8 : sizeof(_Ty) <= 4 ? 4 : sizeof(_Ty) <= 8 ? 2 : 1
    };
    typedef unsigned int size_type;
    typedef _Ty* _Tptr;
    typedef _Tptr* _Mapptr;
    size_type max_size() const { return (this->_Alval.max_size()); }
    bool empty() const { return (_Mysize == 0); }
    void pop_front()
    {
        if (!empty()) {
            size_type _Block = _Myoff / _DEQUESIZ;
            this->_Alval.destroy(_Map[_Block] + _Myoff % _DEQUESIZ);
            if (_Mapsize * _DEQUESIZ <= ++_Myoff)
                _Myoff = 0;
            if (--_Mysize == 0)
                _Myoff = 0;
        }
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

protected:
    // !FAKE: c2 noreturn; retail 0x0001D5C2 (named fold alias) is called out of line and _Growmap does not reload
    // eax after it; the VC 7.1 body (throw length_error) gives that but emits unnamed throw-info data (C0236)
    __declspec(noreturn) void _Xlen() const;
    void _Growmap(size_type _Count)
    {
        if (max_size() / _DEQUESIZ - _Mapsize < _Count)
            _Xlen();

        size_type _Inc = _Mapsize / 2;
        if (_Inc < _DEQUEMAPSIZ)
            _Inc = _DEQUEMAPSIZ;
        if (_Count < _Inc && _Mapsize <= max_size() / _DEQUESIZ - _Inc)
            _Count = _Inc;
        size_type _Myboff = _Myoff / _DEQUESIZ;
        _Mapptr _Newmap = this->_Almap.allocate(_Mapsize + _Count);
        _Mapptr _Myptr = _Newmap + _Myboff;

        _Myptr = _Uninitialized_copy(_Map + _Myboff, _Map + _Mapsize, _Myptr, this->_Almap);
        if (_Myboff <= _Count) {
            _Myptr = _Uninitialized_copy(_Map, _Map + _Myboff, _Myptr, this->_Almap);
            _Uninitialized_fill_n(_Myptr, _Count - _Myboff, (_Tptr)0, this->_Almap);
            _Uninitialized_fill_n(_Newmap, _Myboff, (_Tptr)0, this->_Almap);
        } else {
            _Uninitialized_copy(_Map, _Map + _Count, _Myptr, this->_Almap);
            _Myptr = _Uninitialized_copy(_Map + _Count, _Map + _Myboff, _Newmap, this->_Almap);
            _Uninitialized_fill_n(_Myptr, _Count, (_Tptr)0, this->_Almap);
        }

        _Destroy_range(_Map + _Myboff, _Map + _Mapsize, this->_Almap);
        if (_Map)
            this->_Almap.deallocate(_Map, _Mapsize);

        _Map = _Newmap;
        _Mapsize += _Count;
    }

    _Mapptr _Map;                      // 0x04
    size_type _Mapsize;                // 0x08
    size_type _Myoff;                  // 0x0C
    size_type _Mysize;                 // 0x10
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class AABox;                 // NAME: proto-pdb:default.pdb
class Matrix;                // NAME: proto-pdb:default.pdb
class Quaternion;            // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }
    ResourceHandle& operator=(const ResourceHandle&);

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

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

    char field_4[0xA4 - 0x4];          // 0x04
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

    char field_B4[0x200 - 0xB4];       // 0xB4 (not reached)
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    void SetMaxSpeed(float v) { _MaxSpeed = v; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }

    float _MaxSpeed;                   // 0x200
    char field_204[0x20C - 0x204];     // 0x204
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x2C8 - 0x218];     // 0x218 (not reached)
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetVinceSpringJumpDist() const { return _vinceSpringJumpDist; }
    float GetVinceSpringJumpHeight() const { return _vinceSpringJumpHeight; }

    char field_0[0xB4];                // 0x00
    float _vinceSpringJumpDist;        // 0xB4
    float _vinceSpringJumpHeight;      // 0xB8
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

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

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class SoundResource; // NAME: proto-pdb:default.pdb

// fold-alias owner of deque::_Growmap 0x0005CF1A (C0234); only ReggieTaunt is needed
class ReggieWreckageMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    struct ReggieTaunt { // NAME: proto-pdb:default.pdb
        ResourceHandle<AnimationResource> _anim; // 0x00
        ResourceHandle<SoundResource> _sound;    // 0x04
    };
};

class VinceSpring : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    struct HeightSample { // NAME: proto-pdb:default.pdb
        float height;                  // 0x00
        float time;                    // 0x04
    };
    VinceSpring(SwampSceneNode* node);
    virtual void SoundFinished(const SwampString& name);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void CalcSpeeds(float jumpheight);
    void IncreaseJumpHeight();
    void CacheAnims();
    Vince* GetVince() { return (Vince*)_node; }

    ResourceHandle<AnimationResource> _idle;       // 0xBC
    ResourceHandle<AnimationResource> _prejump;    // 0xC0
    ResourceHandle<AnimationResource> _jumpstart;  // 0xC4
    ResourceHandle<AnimationResource> _fall;       // 0xC8
    ResourceHandle<AnimationResource> _land;       // 0xCC
    ResourceHandle<AnimationResource> _grab;       // 0xD0
    ResourceHandle<AnimationResource> _damage;     // 0xD4
    ResourceHandle<AnimationResource> _death;      // 0xD8
    ResourceHandle<AnimationResource> _landstart;  // 0xDC
    ResourceHandle<AnimationResource> _landend;    // 0xE0
    ResourceHandle<AnimationResource> _tumble;     // 0xE4
    char field_E8[0xF4 - 0xE8];        // 0xE8 (_oldModel .. _state; not reached)
    float _jumpHeight;                 // 0xF4
    bool _rejump;                      // 0xF8
    float _jumpDiff;                   // 0xFC
    char field_100[0x128 - 0x100];     // 0x100 (not reached)
    bool _playingRaspberry;            // 0x128
    char field_129[0x148 - 0x129];     // 0x129 (not reached)
};
DECOMP_SIZE_ASSERT(VinceSpring, 0x148);

// FUNCTION: USA 0x0005C472
// PROTOTYPE: PREVIEW 0x00448990 identical
// NAME: proto-pdb:default.pdb
void VinceSpring::CalcSpeeds(float jumpheight)
{
    // !FAKE: c2 x87 register allocation; neutral stand-in local; the gravity value is held on the x87 stack (no PDB record)
    float tmp = GetVince()->GetGravity();
    GetVince()->SetMaxSpeed(gSettings.GetVinceSpringJumpDist() / (2.0f * sqrtf(jumpheight / tmp)));
    GetVince()->SetVelocity(Vector3(0.0f, sqrtf(2.0f * (tmp * jumpheight)), 0.0f));
}

// FUNCTION: USA 0x0005C4C5
// PROTOTYPE: PREVIEW 0x004489E3 identical
// NAME: proto-pdb:default.pdb
void VinceSpring::IncreaseJumpHeight()
{
    _jumpHeight = (gSettings.GetVinceSpringJumpHeight() < _jumpHeight + _jumpDiff) ? gSettings.GetVinceSpringJumpHeight() : _jumpHeight + _jumpDiff;
}

// FUNCTION: USA 0x0005C8D5
// PROTOTYPE: PREVIEW 0x00448E42 identical
// NAME: proto-pdb:default.pdb
void VinceSpring::CacheAnims()
{
    _grab = g_swamp->GetAnimationManager()->Load("vince_spring_get");
    _damage = g_swamp->GetAnimationManager()->Load("vince_spring_damage01");
    _death = g_swamp->GetAnimationManager()->Load("vince_spring_death");
    _idle = g_swamp->GetAnimationManager()->Load("vince_spring_idle01");
    _jumpstart = g_swamp->GetAnimationManager()->Load("vince_spring_jumpstart");
    _fall = g_swamp->GetAnimationManager()->Load("vince_spring_jumpfloat");
    _landstart = g_swamp->GetAnimationManager()->Load("vince_spring_jumpland_1");
    _landend = g_swamp->GetAnimationManager()->Load("vince_spring_jumpland_2");
    _tumble = g_swamp->GetAnimationManager()->Load("vince_spring_tumblefall");
}

// FUNCTION: USA 0x0005D02F
// PROTOTYPE: PREVIEW 0x00449487 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceSpring::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\sarcophagushustle\\VinceSpring.h", 11) VinceSpring(node);
}

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specialization (C0132, C0154); push_back calls the named fold VA 0x0005CF1A
// out of line; the body would emit a second copy of the ReggieTaunt-named row here (fill_n stays visible:
// declared only, _Growmap's register allocation changes, 67.6)
namespace std {
template <> void deque<VinceSpring::HeightSample,std::allocator<VinceSpring::HeightSample> >::_Growmap(unsigned int);
}
#endif

// FUNCTION: USA 0x0005C577
// PROTOTYPE: PREVIEW 0x00448A94 identical
// NAME: proto-pdb:default.pdb
template void std::deque<VinceSpring::HeightSample,std::allocator<VinceSpring::HeightSample> >::pop_front();

// FUNCTION: USA 0x0005C59E
// PROTOTYPE: PREVIEW 0x00448B0B identical
// NAME: proto-pdb:default.pdb
void VinceSpring::SoundFinished(const SwampString& name)
{
    if (_playingRaspberry && name == "intnowhammy")
        _playingRaspberry = false;
}

// FUNCTION: USA 0x0005CF1A
// PROTOTYPE: PREVIEW 0x0046C4CB identical
// NAME: proto-pdb:default.pdb
template void std::deque<ReggieWreckageMain::ReggieTaunt,std::allocator<ReggieWreckageMain::ReggieTaunt> >::_Growmap(unsigned int);

// FUNCTION: USA 0x0005D056
// PROTOTYPE: PREVIEW 0x004494AE identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // wall-fold-va-callers (C0233): match_one 100, but its call to the declared-only _Construct<HeightSample> (fold 0x000AE5C1, SwampGatorNode's matched alias) is refused "lies in a replaced range" (4.3 T12)
template void std::deque<VinceSpring::HeightSample,std::allocator<VinceSpring::HeightSample> >::push_back(VinceSpring::HeightSample const &);
#endif
