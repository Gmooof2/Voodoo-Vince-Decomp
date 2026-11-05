// UNIT: src/Apps/Vince/scripts/french_quarter/Blender.cpp OBJ: Xbox\OfficialRelease\Blender.obj OPT: /O1 /Oy /Ob1
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    void Zero() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(float x_, float y_, float z_, float w_) : v(x_, y_, z_), w(w_) {}
    void Identity() { v.Zero(); w = 1.0f; }
    void Set(const Vector3&, float);
    void Apply(const Vector3&, Vector3&) const;
    Vector3 Apply(const Vector3& a) const { Vector3 r; Apply(a, r); return r; }
    Quaternion& operator*=(const Quaternion&);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class Vector4 { // NAME: proto-pdb:default.pdb
public:
    void Set(const Vector3& v) { x = v.x; y = v.y; z = v.z; w = 1.0f; }
    operator Vector3() const { return *(const Vector3*)this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    Matrix() {}
    Matrix(const Quaternion& q, const Vector3& t) { Rotation(q); row[3].Set(t); }
    void Rotation(const Quaternion&);

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

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
    typedef typename _Alloc::reference reference;
    typedef typename _Alloc::const_reference const_reference;

    class const_iterator {
    public:
        const_iterator(_Tptr _Ptr)
        {
            _Myptr = _Ptr;
        }

        const_iterator& operator++()
        {
            ++_Myptr;
            return (*this);
        }

        const_iterator& operator+=(difference_type _Off)
        {
            _Myptr += _Off;
            return (*this);
        }

        bool operator==(const const_iterator& _Right) const
        {
            return (_Myptr == _Right._Myptr);
        }

        bool operator!=(const const_iterator& _Right) const
        {
            return (!(*this == _Right));
        }

        _Tptr _Myptr;
    };

    class iterator : public const_iterator {
    public:
        iterator(pointer _Ptr)
            : const_iterator(_Ptr)
        {
        }

        reference operator*() const
        {
            return (*this->_Myptr);
        }

        pointer operator->() const
        {
            return (&**this);
        }

        iterator& operator++()
        {
            ++this->_Myptr;
            return (*this);
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
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[4];                   // 0x00
    SwampInput* _input;                // 0x04
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x8C];                // 0x00
    float Gravity;                     // 0x8C
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

class AABox;                 // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2
    };
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
    void SetOption(unsigned int option, bool on, bool recurse);
    SwampScript* GetScript() const;
    void InvalidateAllSubScripts();
    void SetScript(const SwampString&);

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetVolume(float);
    void SetPitch(float);
    float GetVolume() const { return _volume; }
    float GetPitch() const { return _pitch; }

    char field_A4[0xB0 - 0xA4];        // 0xA4
    float _volume;                     // 0xB0
    float _pitch;                      // 0xB4
};

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void StopSpawning();
    void Reset();
    void Play();
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
    static SoundNode* CreateSound(const SwampString&, unsigned int, SwampSceneNode*, bool);
};

class AnimationControl;      // NAME: proto-pdb:default.pdb

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

class ModelResource;         // NAME: proto-pdb:default.pdb

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void CalcBounds(unsigned int) const;
    Matrix GetBoneOrientation(int) const;

    char field_B4[0x16C - 0xB4];       // 0xB4
    std::vector<Matrix> _deformMatrices; // 0x16C
    char field_17C[0x200 - 0x17C];     // 0x17C
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    virtual void ReturnToHome();                                   // vtable+0x44
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    bool HitSomething() const { return _tag != 0; }
};
DECOMP_SIZE_ASSERT(CollisionResultObject, 0x40);

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    T* operator->() { return _Object; }
    const T* operator->() const { return _Object; }

    T* _Object;                        // 0x00
};

class SwampCollide { // NAME: proto-pdb:default.pdb
public:
    static Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float, const GatorNode*, unsigned int);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(double);
double RandomFloat(float, float);
}

class VinceGiblets : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    struct Giblet { // NAME: proto-pdb:default.pdb
        Giblet() {}
        void Init();
        void DeActivate();
        void Update();

        Vector3 loc;                   // 0x00
        Quaternion rot;                // 0x0C
        Vector3 velocity;              // 0x1C
        Vector3 rotaxis;               // 0x28
        Vector3 start;                 // 0x34
        Vector3 current;               // 0x40
        bool active;                   // 0x4C
    };

    virtual void Update();
    void StartEmitting();
    void StopEmitting();
    virtual ~VinceGiblets();

    std::vector<Giblet> _giblets;      // 0x200
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

// FUNCTION: USA 0x00077720
// PROTOTYPE: PREVIEW 0x00463764 identical
// NAME: proto-pdb:default.pdb
void VinceGiblets::Giblet::Init()
{
    Quaternion zap;
    zap.Set(Vector3(0.0f, 1.0f, 0.0f), BeepLib::RandomFloat(6.283185307179586));
    {
        // !FAKE: inliner; hand form of the by-value Apply in a block (C0191, C0214); neutral stand-in local
        Vector3 tmp;
        zap.Apply(Vector3(0.0f, 0.25f, BeepLib::RandomFloat(0.4f)), tmp);
        loc = tmp;
    }
    velocity.Zero();
    // !FAKE: c2 scheduling; neutral stand-in local; retail copies Gravity to a stack slot before the call
    float v0 = gSettings.Gravity;
    velocity.y = v0 * BeepLib::RandomFloat(0.25f, 0.75f);
    rot = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    rotaxis.x = BeepLib::RandomFloat(-1.0f, 1.0f);
    rotaxis.y = BeepLib::RandomFloat(-1.0f, 1.0f);
    rotaxis.z = BeepLib::RandomFloat(-1.0f, 1.0f);
    active = true;
}

// FUNCTION: USA 0x0007783E
// PROTOTYPE: PREVIEW 0x00463882 identical
// NAME: proto-pdb:default.pdb
void VinceGiblets::Giblet::Update()
{
    if (active) {
        // !FAKE: c2 register allocation; neutral stand-in local (no PDB local); retail keeps elapsed in st(0) throughout
        float tmp = g_swamp->GetInput().GetElapsedTime();
        // !FAKE: inliner; hand form of the scaled velocity (no PDB inline operator*); retail forms all three products first
        Vector3 v0;
        v0.x = tmp * velocity.x;
        v0.y = tmp * velocity.y;
        v0.z = tmp * velocity.z;
        loc += v0;
        if (loc.y <= 0.25f) {
            DeActivate();
        } else {
            velocity.y -= gSettings.Gravity * tmp;
            Quaternion q;
            q.Set(rotaxis, tmp * 1.5f);
            rot *= q;
        }
    }
}

// FUNCTION: USA 0x00077AD4
// PROTOTYPE: PREVIEW 0x00463B18 identical
// NAME: proto-pdb:default.pdb
void VinceGiblets::Update()
{
    Vector3 bpos = GetWorldPosition();
    Quaternion brot = GetWorldRotation();
    int bone = 0;
    // !FAKE: provenance; neutral stand-in name; the PDB records no local for the register-held giblet iterator
    std::vector<Giblet>::iterator p = _giblets.begin();
    std::vector<Matrix>::iterator dit = _deformMatrices.begin();
    for (; p != _giblets.end(); ++p, ++dit, ++bone) {
        p->Update();
        // !FAKE: provenance; neutral stand-in local; retail keeps the pre-Init active flag in a stack byte, no PDB local
        bool tmp = p->active;
        if (!tmp && _emitting)
            p->Init();
        if (bone > 1) {
            // !FAKE: inliner; hand form of the inline Matrix(const Quaternion&, const Vector3&) ctor (C0191); neutral local
            Matrix v2;
            v2.Rotation(p->rot);
            v2.row[3].Set(p->loc);
            *dit = v2;
        }
        // !FAKE: inliner; hand forms of the by-value Apply and operator+ (C0191); neutral stand-in locals
        Matrix v3 = GetBoneOrientation(bone);
        Vector3 v4;
        brot.Apply(v3.row[3], v4);
        Vector3 v0(v4);
        Vector3 v1;
        v1.x = bpos.x + v0.x;
        v1.y = bpos.y + v0.y;
        v1.z = bpos.z + v0.z;
        p->current = v1;
        if (tmp) {
            Ptr<CollisionResultObject> cr = SwampCollide::CheckLine(p->start, p->current, 0.1f, 0, 0);
            if (cr->HitSomething())
                p->DeActivate();
        } else {
            p->start = v1;
        }
    }
    CalcBounds(0);
}

// FUNCTION: USA 0x00077C3C
// PROTOTYPE: PREVIEW 0x00463C80 identical
// NAME: proto-pdb:default.pdb
void VinceGiblets::StartEmitting()
{
    Vector3 bpos = GetWorldPosition();
    Quaternion brot = GetWorldRotation();
    int bone = 0;
    std::vector<Giblet>::iterator git = _giblets.begin();
    std::vector<Matrix>::iterator dit = _deformMatrices.begin();
    for (; git != _giblets.end(); ++git, ++dit, ++bone) {
        git->Init();
        if (bone > 1) {
            // !FAKE: inliner; hand form of the inline Matrix(const Quaternion&, const Vector3&) ctor (C0191); neutral local
            Matrix v2;
            v2.Rotation(git->rot);
            v2.row[3].Set(git->loc);
            *dit = v2;
        }
        // !FAKE: inliner; hand forms of the by-value Apply and operator+ (C0191); neutral stand-in locals
        Matrix v3 = GetBoneOrientation(bone);
        Vector3 v4;
        brot.Apply(v3.row[3], v4);
        Vector3 v0(v4);
        Vector3 v1;
        v1.x = bpos.x + v0.x;
        v1.y = bpos.y + v0.y;
        v1.z = bpos.z + v0.z;
        git->current = v1;
        git->start = v1;
    }
    _emitting = true;
}

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
    void RegisterTimerEvent(unsigned int, float, bool);
    void UnRegisterFrameTick();

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
    enum State { // NAME: proto-pdb:default.pdb
        STATE_OFF = 0x0,
        STATE_STARTING = 0x1,
        STATE_ON = 0x2,
        STATE_BLENDING = 0x3
    };

    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_EVENT_POPOUT = 0x0,
        TIMER_BLENDER_CSEND = 0x1,
        TIMER_EVENT_FLOURSACK = 0x2,
        TIMER_AREA_EFFECT = 0x3
    };

    BlenderMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void FrameTick();
    virtual void HitBy(CollisionType type, SwampSceneNode* hitter, const Ptr<CollisionResultObject>& cr);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    bool _vinceBlended;                // 0xBC
    char field_BD[0xC4 - 0xBD];        // 0xBD
    ResourceHandle<AnimationResource> _on; // 0xC4
    ResourceHandle<AnimationResource> _puree; // 0xC8
    SoundNode* _soundCycle;            // 0xCC
    SoundNode* _tickSound;             // 0xD0
    char field_D4[0xD8 - 0xD4];        // 0xD4
    VinceGiblets* _giblets;            // 0xD8
    FireFlyNode* _smokeEmitter;        // 0xDC
    char field_E0[0xE4 - 0xE0];        // 0xE0
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
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_POP_OUT = 0x14,
        MESSAGE_POP_OUT_BLENDED = 0x15
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_POPOUT = 0,
        STATE_SHAKEOFF = 1
    };

    VinceInBlender(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void Message(SwampScript* source, int message, void* data);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:

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

// FUNCTION: USA 0x00078278
// PROTOTYPE: PREVIEW 0x004642BC identical
// NAME: proto-pdb:default.pdb
void VinceInBlender::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_POP_OUT_BLENDED:
        _wasBlended = true;
    case MESSAGE_POP_OUT:
        _node->SetOption(SwampSceneNode::DontDraw, false, false);
        GetCritter()->StartAnimation(_popOut, false, false);
        SoundManager::PlaySound("qb_vincepop", 0x20, _node, true, 0.0f, 0.0f);
        SetState(STATE_POPOUT);
        break;
    }
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

// FUNCTION: USA 0x0007791E
// PROTOTYPE: PREVIEW 0x00463962 identical
// NAME: proto-pdb:default.pdb
void BlenderMain::FrameTick()
{
    if (_state == STATE_STARTING || _state == STATE_ON) {
        float elapsed = g_swamp->GetInput().GetElapsedTime();
        if (_soundCycle) {
            _soundCycle->SetPitch(elapsed + _soundCycle->GetPitch() >= -1.0f ? -1.0f : elapsed + _soundCycle->GetPitch());
            _soundCycle->SetVolume(elapsed + _soundCycle->GetVolume() > 1.0f ? 1.0f : elapsed + _soundCycle->GetVolume());
        }
    }
}

class WallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        TURN_OFF = 0x14
    };
};

// FUNCTION: USA 0x00078474
// PROTOTYPE: PREVIEW 0x0046448D identical
// NAME: proto-pdb:default.pdb
void BlenderMain::AnimationFinished()
{
    if (_state == STATE_STARTING) {
        GetCritter()->StartAnimation(_on, true, false);
        SetState(STATE_ON);
    } else {
        // !FAKE: provenance; neutral stand-in local; the PDB records no local for the register-held critter
        Critter* p = pManager->Find("WallSwitch");
        if (p)
            p->GetScript()->Message(this, WallSwitchMain::TURN_OFF, 0);
        if (_soundCycle) {
            _soundCycle->Invalidate();
            _soundCycle = 0;
        }
        UnRegisterFrameTick();
        if (_tickSound)
            _tickSound->Invalidate();
        _tickSound = 0;
        _vinceBlended = _state == STATE_BLENDING;
        if (_vinceBlended) {
            RegisterTimerEvent(TIMER_AREA_EFFECT, 0.1f, false);
            RegisterTimerEvent(TIMER_EVENT_POPOUT, 3.0f, false);
            RegisterTimerEvent(TIMER_EVENT_FLOURSACK, 1.0f, false);
            _giblets->StopEmitting();
            if (_smokeEmitter)
                _smokeEmitter->StopSpawning();
        }
        SetState(STATE_OFF);
    }
}

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
};

class GameCam { // NAME: proto-pdb:default.pdb
};

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void ResetLocation(bool);
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00078583
// PROTOTYPE: PREVIEW 0x0046459C identical
// NAME: proto-pdb:default.pdb
void BlenderMain::HitBy(CollisionType type, SwampSceneNode* hitter, const Ptr<CollisionResultObject>& cr)
{
    if (type == VerticalCollision && cr->_tag == 1) {
        if (hitter == pManager->GetVince()) {
            hitter->InvalidateAllSubScripts();
            hitter->SetScript("VinceInBlender");
            if (_state == STATE_OFF) {
                RegisterTimerEvent(TIMER_EVENT_POPOUT, 0.5f, false);
            } else {
                AnimCam::SwitchTo("camera_blender", 0.0f, false, 0.0f);
                if (_tickSound)
                    _tickSound->Invalidate();
                _tickSound = 0;
                if (_soundCycle)
                    _soundCycle->Invalidate();
                _soundCycle = SoundManager::CreateSound("qb_vgrind", 5, GetNode(), true);
                SetState(STATE_BLENDING);
                GetCritter()->StartAnimation(_puree, false, false);
                _giblets->SetOption(SwampSceneNode::DontDraw, false, false);
                _giblets->StartEmitting();
                if (_smokeEmitter) {
                    _smokeEmitter->Reset();
                    _smokeEmitter->Play();
                }
            }
            // !FAKE: c1xx front end; neutral stand-in local; retail destroys the "VINCECAM" temp before the call,
            // the PDB records no local (match_one 95.81 -> 100)
            VinceCam* p = (VinceCam*)g_CameraManager->Find("VINCECAM");
            p->ResetLocation(false);
        } else {
            ((Critter*)hitter)->ReturnToHome();
        }
    }
}

namespace std {

// !FAKE: c1xx front end; explicit instantiations of single templates and members; retail emits them through callers
// in this unit (not matched yet); the instantiations emit them instead
// FUNCTION: USA 0x000775B9
// PROTOTYPE: PREVIEW 0x005157E9 identical
// NAME: proto-pdb:default.pdb
template vector<Matrix>::size_type vector<Matrix>::size() const;
// FUNCTION: USA 0x000775CC
// PROTOTYPE: PREVIEW 0x00463623 identical
// NAME: proto-pdb:default.pdb
template vector<VinceGiblets::Giblet>::size_type vector<VinceGiblets::Giblet>::size() const;
// FUNCTION: USA 0x000775E2
// PROTOTYPE: PREVIEW 0x00515814 identical
// NAME: proto-pdb:default.pdb
template vector<Matrix>::size_type vector<Matrix>::capacity() const;
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

// FUNCTION: USA 0x000788BF
// PROTOTYPE: PREVIEW 0x004648D8 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceInBlender::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("bakery\\Blender.cpp", 234) VinceInBlender(node);
}

// FUNCTION: USA 0x00079177
// PROTOTYPE: PREVIEW 0x0046518B identical
// NAME: proto-pdb:default.pdb
SwampScript* BlenderMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\Blender.h", 14) BlenderMain(node);
}
