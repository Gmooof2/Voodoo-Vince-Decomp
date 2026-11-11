// UNIT: src/Apps/Vince/scripts/french_quarter/GasPumps.cpp OBJ: Xbox\OfficialRelease\GasPumps.obj OPT: /O1 /Oy /Ob1
#include "../../../../include/decomp.h"
inline void* operator new(unsigned int, void* _Where) { return _Where; }
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);

    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right);
// VC 7.1 <xmemory>/<deque> bodies for the deque rows (C0132, VinceSpring.cpp form); deque::_Growmap is called
// out of line at the named fold VA 0x0005CF1A (VinceSpring.cpp owns the ReggieTaunt-named row)
template <class _T1, class _T2> inline void _Construct(_T1* _Ptr, const _T2& _Val)
{
    new ((void*)_Ptr) _T1(_Val);
}
template <class _Ty> inline void _Destroy(_Ty* _Ptr)
{
    (_Ptr)->~_Ty();
}
template <class _Ty> inline _Ty* _Allocate(unsigned int _Count, _Ty*)
{
    return ((_Ty*)operator new(_Count * sizeof(_Ty)));
}
template <class _Ty> class allocator {
public:
    typedef _Ty* pointer;
    typedef unsigned int size_type;
    template <class _Other> struct rebind {
        typedef allocator<_Other> other;
    };
    pointer allocate(size_type _Count) { return (_Allocate(_Count, (pointer)0)); }
    void construct(pointer _Ptr, const _Ty& _Val) { _Construct(_Ptr, _Val); }
    void destroy(pointer _Ptr) { _Destroy(_Ptr); }
    void deallocate(pointer _Ptr, size_type) { operator delete(_Ptr); }
};
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
    bool empty() const { return (_Mysize == 0); }
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
    void pop_back()
    {
        if (!empty()) {
            size_type _Newoff = _Mysize + _Myoff - 1;
            size_type _Block = _Newoff / _DEQUESIZ;
            if (_Mapsize <= _Block)
                _Block -= _Mapsize;
            this->_Alval.destroy(_Map[_Block] + _Newoff % _DEQUESIZ);
            if (--_Mysize == 0)
                _Myoff = 0;
        }
    }

protected:
    // !FAKE: c1xx front end; declared only; retail calls the named fold VA 0x0005CF1A out of line (row owned by VinceSpring.cpp)
    void _Growmap(size_type _Count);
    void _Tidy()
    {
        while (!empty())
            pop_back();
        for (size_type _Count = _Mapsize; 0 < _Count;) {
            if (*(_Map + --_Count) != 0)
                this->_Alval.deallocate(*(_Map + _Count), _DEQUESIZ);
        }
        if (_Map != 0)
            this->_Almap.deallocate(_Map, _Mapsize);
        _Mapsize = 0;
        _Map = 0;
    }

    _Mapptr _Map;                      // 0x04
    size_type _Mapsize;                // 0x08
    size_type _Myoff;                  // 0x0C
    size_type _Mysize;                 // 0x10
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString&, int) const;
};
class AABox;                // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
class ModelManager;          // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    void Set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
    Vector3& operator+=(const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    // !FAKE: inliner; hand form of the PDB inline conversion operator; no before/after recorded
    operator Vector3() const { return *(const Vector3*)this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Vector4& operator[](int i) { return row[i]; }

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    void Set(float, float, float);
    void Set(const Vector3&, float);
    Quaternion& operator*=(const Quaternion&);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    int _RefCount;                     // 0x00
};

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int _tag;                 // 0x04
};

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    bool HitSomething() const { return _tag != 0; }
};

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    const T* operator->() const { return _Object; }

    T* _Object;                        // 0x00
};

class CritterCollision { // NAME: proto-pdb:default.pdb
public:
    bool HitWorld() const { return _wall->HitSomething() || _floor->HitSomething(); }

    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};
DECOMP_SIZE_ASSERT(CritterCollision, 0x8);

class SwampScript;            // NAME: proto-pdb:default.pdb

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
    SwampScript* GetScript() const;
    const Vector3& GetLocation() const { return _Location; }
    unsigned int GetZone() const;
    const Quaternion& SetRotation(const Quaternion&);
    const Quaternion& GetRotation() const { return _Rotation; }

    char field_4[0x30];                // 0x04
    Vector3 _Location;                 // 0x34
    Quaternion _Rotation;              // 0x40
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    ModelManager& GetModelManager() const { return *_modelManager; }

    char field_0[4];                   // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x20];                // 0x08
    ModelManager* _modelManager;       // 0x28
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

Vector3 Normalize(const Vector3&); // NAME: proto-pdb:default.pdb

class DamageSourceInfo { // NAME: proto-pdb:default.pdb
public:
    DamageSourceInfo();
    ~DamageSourceInfo();

    SwampString sourceType;            // 0x00
    SwampString attackType;            // 0x1C
    float _multiplier;                 // 0x38
    bool _canBlock;                    // 0x3C
};
DECOMP_SIZE_ASSERT(DamageSourceInfo, 0x40);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(double);
unsigned int Random();
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00 (_Name; not reached)
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class SoundResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class AnimationControl { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource)
    {
        if (_Resource)
            _Resource->AddRef();
    }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C
};

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl> TransitionAnimation(ResourceHandle<AnimationResource>, float, bool, bool);
    Matrix GetBoneOrientation(const SwampString&) const;
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void PlaySound(const SwampString&, unsigned int, bool, float, float);
    ResourceHandle<AnimationResource> GetDefaultIdleAnim() const;
    CritterCollision MoveMyFatAss(float, unsigned int);
    bool IsStunned() const;
    void SetVelocity(const Vector3& v) { _Velocity = v; }
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40

    char field_50[0x20C - 0x50];       // 0x50
    Vector3 _Velocity;                 // 0x20C
    SwampString _species;              // 0x218
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Create(const SwampString&, const SwampString&, const char*, ResourceHandle<ModelResource>, const Vector3&, const Quaternion&, unsigned int);
    Critter* Find(const SwampString&);

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_DAMAGE = 0x0
};

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    void PushBack(T rval);

    std::deque<T> _queue;              // 0x00
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

    void RegisterTimerEvent(unsigned int, float, bool);
    void RemoveTimerEvents();

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
    virtual SwampString Save();
    Critter* GetCritter() { return (Critter*)_node; }
    int GetState() const { return _state; }

    int _state;                      // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class WrenchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    WrenchMain(SwampSceneNode* node);
    virtual void FrameTick();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    Vector3 _axis;                     // 0xBC
    float _gravity;                    // 0xC8
    float _angle;                      // 0xCC
    float _dAngle;                     // 0xD0
    int _collisions;                   // 0xD4
    Vector3 _oldVel;                   // 0xD8
};
DECOMP_SIZE_ASSERT(WrenchMain, 0xE4);

// FUNCTION: USA 0x0007F8A4
// PROTOTYPE: PREVIEW 0x0046B7ED identical
// NAME: proto-pdb:default.pdb
class GasPumpMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum State { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_ATTACK = 1,
        STATE_DAMAGE = 2,
        STATE_DEAD = 3
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_VINCE_NEAR = 0x14,
        MESSAGE_VINCE_FAR = 0x15,
        MESSAGE_DEADWRENCH = 0x16,
        MESSAGE_DO_TAUNT = 0x17
    };
    enum TimerEvent { // NAME: proto-pdb:default.pdb
        TIMER_EVENT_BEGIN_ATTACK = 0,
        TIMER_EVENT_RELEASE_WRENCH = 1,
        TIMER_EVENT_DING = 2,
        TIMER_EVENT_RELOAD = 3
    };
    virtual void AnimationFinished();
    virtual void Message(SwampScript*, int, void*);
    virtual void TimerEvent(unsigned int);
    virtual void Activate();
    virtual SwampString Save();
    virtual void SetState(int);
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);

protected:
    void ResetDingTimer();

    static int _hintCount;

public:
    ResourceHandle<AnimationResource> _animThrow;  // 0xBC
    bool _vinceNear;                   // 0xC0
    bool _active;                      // 0xC1
    int _wrenchHits;                   // 0xC4
    float _tauntTimer;                 // 0xC8
    ResourceHandle<AnimationResource> _animDamage; // 0xCC
};
DECOMP_SIZE_ASSERT(GasPumpMain, 0xD0);

class PrimoMain : public GasPumpMain { // NAME: proto-pdb:default.pdb
public:
    PrimoMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void Message(SwampScript*, int, void*);
    virtual void TimerEvent(unsigned int);
    virtual Critter* GetPartner();                                 // vtable+0x70
    Critter* GetPrimo() { return (Critter*)_node; }
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(PrimoMain, 0xD0);

class ReggieMain : public GasPumpMain { // NAME: proto-pdb:default.pdb
public:
    ReggieMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
    virtual Critter* GetPartner();                                 // vtable+0x70
    Critter* GetReggie() { return (Critter*)_node; }
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_D0[0xE4 - 0xD0];        // 0xD0 (not reached)
};
DECOMP_SIZE_ASSERT(ReggieMain, 0xE4);

class ReggieWreckageMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_TAUNT = 0
    };
// FUNCTION: USA 0x0007F3C8
// PROTOTYPE: PREVIEW 0x0046B2F5 identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0007F3DC
// PROTOTYPE: PREVIEW 0x0046B309 identical
// NAME: proto-pdb:default.pdb
    struct ReggieTaunt { // NAME: proto-pdb:default.pdb
        ResourceHandle<AnimationResource> _anim; // 0x00
        ResourceHandle<SoundResource> _sound;    // 0x04
    };
    ReggieWreckageMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void GainingControl(SwampScript*);

    SwampShuffleQueue<ReggieTaunt> _taunts; // 0xBC
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ReggieWreckageMain, 0xD0);

// ??_GPrimoMain 0x0007FAE6, ??_GReggieWreckageMain 0x000810A8 unmatched, no draft (5.3 T6.c16): companion wall: a
// virtual ??_G is emitted only with its class vtable; ??0PrimoMain 0x0008023B and ??0ReggieWreckageMain 0x00080967
// are fail rows (provisional reloc), and neither class has an explicit dtor storing the vptr. Lift: match a ctor.

// FUNCTION: USA 0x0007F1F1
// PROTOTYPE: PREVIEW 0x0046B0FB identical
// NAME: proto-pdb:default.pdb
void ReggieWreckageMain::GainingControl(SwampScript* oldscript)
{
    RemoveTimerEvents();
    AnimationFinished();
}

// FUNCTION: USA 0x0007F238
// PROTOTYPE: PREVIEW 0x0046B165 identical
// NAME: proto-pdb:default.pdb
void GasPumpMain::AnimationFinished()
{
    switch (_state) {
    case STATE_ATTACK:
        if (_vinceNear)
            RegisterTimerEvent(TIMER_EVENT_BEGIN_ATTACK, (float)BeepLib::RandomFloat(1.0), false);
        SetState(STATE_IDLE);
        break;
    case STATE_DAMAGE:
        SetState(STATE_ATTACK);
        break;
    }
}

// FUNCTION: USA 0x0007F392
// PROTOTYPE: PREVIEW 0x0046B2BF identical
// NAME: proto-pdb:default.pdb
void PrimoMain::AnimationFinished()
{
    switch (_state) {
    case STATE_DAMAGE:
        SetState(STATE_IDLE);
        break;
    }
}

// FUNCTION: USA 0x0007F41A
// PROTOTYPE: PREVIEW 0x0046B34B identical
// NAME: proto-pdb:default.pdb
void GasPumpMain::Activate()
{
    if (!_active) {
        ResetDingTimer();
        _active = true;
    }
}

// FUNCTION: USA 0x0007F430
// PROTOTYPE: PREVIEW 0x0046B361 identical
// NAME: proto-pdb:default.pdb
void GasPumpMain::SetState(int state)
{
    if (state != GetState()) {
        switch (state) {
        case STATE_IDLE:
            GetCritter()->TransitionAnimation(GetCritter()->GetDefaultIdleAnim(), 0.25f, true, false);
            break;
        case STATE_ATTACK:
            GetCritter()->TransitionAnimation(_animThrow, 0.25f, false, false);
            RegisterTimerEvent(TIMER_EVENT_RELEASE_WRENCH, 0.4f, false);
            RegisterTimerEvent(TIMER_EVENT_RELOAD, 1.0f, false);
            break;
        case STATE_DAMAGE:
            GetCritter()->TransitionAnimation(_animDamage, 0.25f, false, false);
            break;
        }
    }
    _state = state;
}

// FUNCTION: USA 0x0007F61A
// PROTOTYPE: PREVIEW 0x0046B54B identical
// NAME: proto-pdb:default.pdb
void ReggieWreckageMain::AnimationFinished()
{
    RegisterTimerEvent(TIMER_TAUNT, (float)(BeepLib::RandomFloat(2.0) + 2.0), false);
    // !FAKE: c2 register allocation; stand-in local (no PDB record); retail keeps _node in esi across both calls (C0267)
    Critter* p = (Critter*)_node;
    p->TransitionAnimation(p->GetDefaultIdleAnim(), 0.25f, true, false);
}

// FUNCTION: USA 0x0007F69E
// PROTOTYPE: PREVIEW 0x0046B5E7 identical
// NAME: proto-pdb:default.pdb
SwampString GasPumpMain::Save()
{
    return CritterMain::Save();
}

// FUNCTION: USA 0x0007F6AE
// PROTOTYPE: PREVIEW 0x0046B5F7 identical
// NAME: proto-pdb:default.pdb
void WrenchMain::FrameTick()
{
    float elapsed = g_swamp->GetInput().GetElapsedTime();
    _oldVel = GetCritter()->_Velocity;
    CritterCollision cr = GetCritter()->MoveMyFatAss(_gravity, 3);
    // !FAKE: c2 register allocation; stand-in local (no PDB record); retail loads _node before the product
    Critter* v2 = GetCritter();
    // !FAKE: inliner; hand form of retail's inlined axis-angle Quaternion product; retail inlines it, no before/after recorded (C0191)
    Quaternion v0;
    v0.Set(_axis, elapsed * _dAngle);
    Quaternion v1(v0);
    v1 *= v2->GetRotation();
    GetCritter()->SetRotation(Quaternion(v1));
    if (cr.HitWorld() && _oldVel.y < 0.0f) {
        Critter* p = GetCritter();
        // !FAKE: c1xx front end; neutral stand-in local, comma expression; retail builds the string before the float arguments (C0220)
        const SwampString* v1;
        p->PlaySound((v1 = &format("reggimp%02d", BeepLib::Random() % 2 + 1), *v1), 8, false, 0.0f, 0.0f);
        InvalidateScript();
    }
}

// FUNCTION: USA 0x0007F7C3
// PROTOTYPE: PREVIEW 0x0046B70C identical
// NAME: proto-pdb:default.pdb
template void std::deque<ReggieWreckageMain::ReggieTaunt,std::allocator<ReggieWreckageMain::ReggieTaunt> >::pop_front();

// FUNCTION: USA 0x0007F802
// PROTOTYPE: PREVIEW 0x0046B74B identical
// NAME: proto-pdb:default.pdb
template void std::deque<ReggieWreckageMain::ReggieTaunt,std::allocator<ReggieWreckageMain::ReggieTaunt> >::pop_back();

// !FAKE: c1xx front end; scaffold call; retail's ??1GasPumpMain@@UAE@XZ is implicit (an explicit body adds a vptr
// store, 86.6); this qualified call odr-uses it (matched_set [[emitter]] row at the finish, C0266)
void pin_emit_GasPumpMain_dtor(GasPumpMain* p)
{
    p->GasPumpMain::~GasPumpMain();
}

// FUNCTION: USA 0x0007FA7B
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; explicit instantiation of a single member; retail emits it through a caller in this unit
// (not matched yet); the instantiation emits it instead
template void std::deque<ReggieWreckageMain::ReggieTaunt,std::allocator<ReggieWreckageMain::ReggieTaunt> >::_Tidy();

// FUNCTION: USA 0x0007FABF
// PROTOTYPE: PREVIEW 0x0046BA08 identical
// NAME: proto-pdb:default.pdb
SwampScript* WrenchMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\mainstreet\\GasPumps.h", 117) WrenchMain(node);
}

// FUNCTION: USA 0x0007FDA7
// PROTOTYPE: PREVIEW 0x0046BCF0 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 93.74, bytes mismatch +0x19 (T12.c29: C0244 block, strings "Wrench" then "", _node local
// before Set; retail's call is ?Set@Quaternion 0xDC99B). Left: newloc += v3 folds y, z as fadd-memory where retail
// does fld newloc.c; fadd st(1) (+= split/commuted/Set/ctor forms unchanged); string ctors schedule into that block.
// Earlier: 88.36 (4.4 T13.c3), 90.58 with an inline Quaternion(x,y,z) call; T12.c17 at-stop 20 (logs T12.c17, T12.c29)
void GasPumpMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_EVENT_BEGIN_ATTACK:
        SetState(STATE_ATTACK);
        break;
    case TIMER_EVENT_RELEASE_WRENCH: {
        Matrix m = GetCritter()->GetBoneOrientation("wrench");
        ResourceHandle<ModelResource> wrenchModel = g_swamp->GetModelManager().Load("wrench_base");
        Vector3 diff = m[3];
        // !FAKE: inliner; pointer locals + hand form of retail's inlined Vector3 sum, difference and scale; retail inlines them, no before/after recorded (C0191)
        Vector3 newloc(diff.x + _node->_Location.x, diff.y + _node->_Location.y, diff.z + _node->_Location.z);
        const Vector3* v1 = &pManager->_vinceCache->GetWorldPosition();
        diff.Set(v1->x - newloc.x, v1->y - newloc.y, v1->z - newloc.z);
        const Vector3* v2 = &Normalize(diff);
        const Vector3& v3 = Vector3(v2->x * 1.5f, v2->y * 1.5f, v2->z * 1.5f);
        newloc += v3;
        {
            // !FAKE: c1xx temporary order; string-argument temporaries as block locals, built right to left before the Quaternion as retail (T12.c29 probe)
            SwampString v6("Wrench");
            SwampString v5("");
            // !FAKE: c2 register allocation; stand-in local; retail loads _node into edi before the Set call (T12.c29 probe)
            SwampSceneNode* v7 = _node;
            // !FAKE: inliner; hand form of retail's inlined Quaternion(0, 0, 0) temporary; retail inlines it, no before/after recorded (C0191)
            Quaternion v4;
            v4.Set(0.0f, 0.0f, 0.0f);
            pManager->Create(v5, v6, "Wrench", wrenchModel, newloc, v4, v7->GetZone());
        }
        GetCritter()->PlaySound("reggswoosh", 0x28, true, 0.0f, 0.0f);
        break;
    }
    }
}
#endif

// FUNCTION: USA 0x0007FF8F
// PROTOTYPE: PREVIEW 0x0046BED8 identical
// NAME: proto-pdb:default.pdb
void GasPumpMain::HitBy(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr)
{
    if (critterHit == pManager->_vinceCache && !((Critter*)critterHit)->IsStunned()) {
        SoundManager::PlaySound("vbounce", 0x20, _node, true, 0.0f, 0.0f);
        // !FAKE: inliner; pointer locals + hand form of retail's inlined Vector3 difference; retail inlines it, no before/after recorded (C0191)
        const Vector3* v0 = &_node->GetWorldPosition();
        const Vector3* v1 = &critterHit->GetWorldPosition();
        Vector3 v2;
        v2.x = v1->x - v0->x;
        v2.y = v1->y - v0->y;
        v2.z = v1->z - v0->z;
        Vector3 diff = Normalize(v2);
        float v3 = GetCritter()->GetGravity() * 0.1f;
        v2.x = diff.x * 2.0f;
        v2.y = v3 * 2.0f;
        v2.z = diff.z * 2.0f;
        ((Critter*)critterHit)->SetVelocity(v2);
        ((Critter*)critterHit)->Stun(0.5f);
        DamageSourceInfo dsi;
        dsi.attackType = "bump";
        dsi.sourceType = GetCritter()->_species;
        critterHit->GetScript()->Message(this, MESSAGE_DAMAGE, &dsi);
    }
}

// FUNCTION: USA 0x000800E0
// PROTOTYPE: PREVIEW 0x0046C029 identical
// NAME: proto-pdb:default.pdb
void ReggieMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_EVENT_DING:
        GetReggie()->PlaySound("reggding", 0x28, true, 0.0f, 0.0f);
        ResetDingTimer();
        break;
    case TIMER_EVENT_RELOAD:
        GetReggie()->PlaySound("reggwrench", 0x28, true, 0.0f, 0.0f);
        break;
    default:
        GasPumpMain::TimerEvent(timerId);
    }
}

// FUNCTION: USA 0x00080177
// NAME: proto-pdb:default.pdb
Critter* ReggieMain::GetPartner()
{
    return pManager->Find("Primo");
}

// FUNCTION: USA 0x000801AD
// PROTOTYPE: PREVIEW 0x0046C0F6 identical
// NAME: proto-pdb:default.pdb
void ReggieMain::Load(const BeepTDF& tdf)
{
    _hintCount = tdf.GetIntRecord("hintcount", 0);
}

// FUNCTION: USA 0x000801E3
// PROTOTYPE: PREVIEW 0x0046C12C identical
// NAME: proto-pdb:default.pdb
SwampString ReggieMain::Save()
{
    return GasPumpMain::Save() + format("\t\thintcount = %i;\n", _hintCount);
}

// FUNCTION: USA 0x00080333
// PROTOTYPE: PREVIEW 0x0046C27C identical
// NAME: proto-pdb:default.pdb
void PrimoMain::Message(SwampScript* from, int message, void* data)
{
    switch (message) {
    case MESSAGE_VINCE_NEAR:
        break;
    case MESSAGE_VINCE_FAR:
        break;
    default:
        GasPumpMain::Message(from, message, data);
        break;
    }
}

// FUNCTION: USA 0x00080347
// PROTOTYPE: PREVIEW 0x0046C290 identical
// NAME: proto-pdb:default.pdb
void PrimoMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_EVENT_DING:
        GetPrimo()->PlaySound("reggding", 0x28, true, 0.0f, 0.0f);
        break;
    default:
        GasPumpMain::TimerEvent(timerId);
    }
}

// FUNCTION: USA 0x0008039C
// NAME: proto-pdb:default.pdb
Critter* PrimoMain::GetPartner()
{
    return pManager->Find("Reggie");
}

// FUNCTION: USA 0x00080534
// PROTOTYPE: PREVIEW 0x0046C47D identical
// NAME: proto-pdb:default.pdb
SwampScript* PrimoMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\mainstreet\\GasPumps.h", 84) PrimoMain(node);
}

// FUNCTION: USA 0x00080582
// PROTOTYPE: PREVIEW 0x0046C5E0 identical
// NAME: proto-pdb:default.pdb
template void std::deque<ReggieWreckageMain::ReggieTaunt,std::allocator<ReggieWreckageMain::ReggieTaunt> >::push_back(ReggieWreckageMain::ReggieTaunt const &);

// FUNCTION: USA 0x00080692
// PROTOTYPE: PREVIEW 0x0046C6F0 identical
// NAME: proto-pdb:default.pdb
template <class T> void SwampShuffleQueue<T>::PushBack(T rval)
{
    _queue.push_back(rval);
}
template void SwampShuffleQueue<ReggieWreckageMain::ReggieTaunt>::PushBack(ReggieWreckageMain::ReggieTaunt);

// row-end pin 5.4 T26.c1
// FUNCTION: USA 0x00081304
// PROTOTYPE: PREVIEW 0x0046D1F6 identical
// NAME: proto-pdb:default.pdb
SwampScript* ReggieMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\mainstreet\\GasPumps.h", 64) ReggieMain(node);
}

// FUNCTION: USA 0x0008132B
// NAME: proto-pdb:default.pdb
SwampScript* ReggieWreckageMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("mainstreet\\GasPumps.cpp", 623) ReggieWreckageMain(node);
}

class CanGasMain { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x00081385
// NAME: proto-pdb:default.pdb
ReggieMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("ReggieMain", creationFunc);
}

// FUNCTION: USA 0x000813C1
// NAME: proto-pdb:default.pdb
PrimoMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("PrimoMain", creationFunc);
}

// FUNCTION: USA 0x00081439
// NAME: proto-pdb:default.pdb
CanGasMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("CanGasMain", creationFunc);
}

// FUNCTION: USA 0x00081475
// NAME: proto-pdb:default.pdb
WrenchMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("WrenchMain", creationFunc);
}

// FUNCTION: USA 0x000815B9
// NAME: proto-pdb:default.pdb
ReggieWreckageMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("ReggieWreckageMain", creationFunc);
}

class CanGas { // NAME: proto-pdb:default.pdb
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampSceneNode* creationFunc();
};

class SwampNodeManager { // NAME: proto-pdb:default.pdb
public:
    static SwampNodeManager& Instance();
    void Add(const SwampString& name, SwampSceneNode* (*func)());
};

// FUNCTION: USA 0x000813FD
// NAME: proto-pdb:default.pdb
CanGas::RegisterMe::RegisterMe()
{
    SwampNodeManager::Instance().Add("CanGas", creationFunc);
}
