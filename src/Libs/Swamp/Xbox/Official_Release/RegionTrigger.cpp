// UNIT: src/Libs/Swamp/Xbox/Official_Release/RegionTrigger.cpp OBJ: Xbox\Official_Release\RegionTrigger.obj OPT: /O1 /Oy
#include <string.h>
#pragma intrinsic(strlen) // src proto-pdb:default.pdb strlen
// !FAKE: c2 intrinsics; file-scope pragma; retail inlines fabs in RegionTrigger::TestLineSegmentCollision (C0221)
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
inline float fabs(float _X)
{
    return (float)fabs((double)_X);
}
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& operator+=(const E* _Ptr) { return append(_Ptr); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, npos); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& append(const E* _Ptr);
    const E* c_str() const { return _Myptr(); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    int compare(const basic_string& _Right) const;
    int compare(const E* _Ptr) const;
    static const unsigned int npos = (unsigned int)(-1);
protected:
    const E* _Myptr() const { return 16 <= _Myres ? _Bx._Ptr : _Bx._Buf; }
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    union _Bxty {
        E _Buf[16];
        E* _Ptr;
    };
    char _Alval;                       // 0x00
    _Bxty _Bx;                         // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class T, class A = allocator<T> > class vector {
public:
    class const_iterator {
    public:
        const_iterator(T* _Ptr) : _Myptr(_Ptr) {}
        const T& operator*() const { return *_Myptr; }
        const_iterator& operator++() { ++_Myptr; return *this; }
        bool operator!=(const const_iterator& _Right) const { return _Myptr != _Right._Myptr; }
        T* _Myptr;
    };
    const_iterator begin() const { return const_iterator(_Myfirst); }
    const_iterator end() const { return const_iterator(_Mylast); }

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class E, class T, class A>
bool operator<(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right);
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const E* _Right);
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right)
{
    return (_Left.compare(_Right) == 0);
}
template <class E, class T, class A> inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return (_Left.compare(_Right) == 0);
}
template <class E, class T, class A> inline bool operator!=(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return (!(_Left == _Right));
}
template <class T> struct less {
    bool operator()(const T& _Left, const T& _Right) const { return (_Left < _Right); }
};
template <class T1, class T2> struct pair {
    pair(const T1& _Val1, const T2& _Val2) : first(_Val1), second(_Val2) {}
    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
    Pr comp;
};
template <class Tr> class _Tree_nod : public Tr { // NAME: proto-pdb:default.pdb
protected:
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
        char _Color;
        char _Isnil;
    };
};
template <class Tr> class _Tree : public _Tree_nod<Tr> {
    typedef typename _Tree_nod<Tr>::_Node _Node;
public:
    class const_iterator {
    public:
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const typename Tr::value_type* operator->() const { return &_Ptr->_Myval; }
        const_iterator& operator++() { _Inc(); return *this; }
        bool operator!=(const const_iterator& _Right) const { return _Ptr != _Right._Ptr; }
        // _Inc body visible: cl then keeps the iterator in edi across RegionManager::Create's loop calls (C0245;
        // Create 92.36 -> 98.36); it stays out of line, emitting this unit's alias at fold VA 0x000ECEA5 (bytes equal)
        void _Inc()
        {
            if (_Ptr->_Isnil)
                ;
            else if (!_Ptr->_Right->_Isnil) {
                // _Ptr = _Min(_Right(_Ptr)), the VC 7.1 static helper written out
                _Node* _Pnode = _Ptr->_Right;
                while (!_Pnode->_Left->_Isnil)
                    _Pnode = _Pnode->_Left;
                _Ptr = _Pnode;
            } else {
                _Node* _Pnode;
                while (!(_Pnode = _Ptr->_Parent)->_Isnil && _Ptr == _Pnode->_Right)
                    _Ptr = _Pnode;
                _Ptr = _Pnode;
            }
        }
        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
        typename Tr::value_type* operator->() const { return &this->_Ptr->_Myval; }
        iterator& operator++() { this->_Inc(); return *this; }
    };
    iterator begin() { return iterator(_Myhead->_Left); }
    const_iterator begin() const { return const_iterator(_Myhead->_Left); }
    iterator end() { return iterator(_Myhead); }
    const_iterator end() const { return const_iterator(_Myhead); }
    pair<iterator, bool> insert(const typename Tr::value_type& _Val);
    iterator lower_bound(const typename Tr::key_type& _Keyval) { return (iterator(_Lbound(_Keyval))); }
    iterator upper_bound(const typename Tr::key_type& _Keyval) { return (iterator(_Ubound(_Keyval))); }

protected:
    _Tree() { _Init(); }
    void _Init();
    void _Lrotate(_Node* _Wherenode);
    _Node* _Lbound(const typename Tr::key_type& _Keyval) const;
    _Node* _Ubound(const typename Tr::key_type& _Keyval) const;

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class multimap // src proto-pdb:default.pdb multimap
    : public _Tree<_Tmap_traits<K, T, Pr, A, true> > {
public:
    multimap() {}
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetBlockName() const { return _blockname; }
    const std::vector<BeepTDF*>& GetBlocks() const { return _blocklist; }
    const BeepTDF* FindBlock(const SwampString& blockname, bool canfail) const;
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;

    SwampString _blockname;            // 0x00
    char field_1C[0x3C - 0x1C];        // 0x1C
    std::vector<BeepTDF*> _blocklist;  // 0x3C
    char field_4C[0x58 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);

void* operator new(unsigned int size, const char* file, int line);

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    float LengthSquared() const { return x * x + y * y + z * z; }

    float x;                          // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

struct AABox { // NAME: proto-pdb:default.pdb
    // retail 0x000C8F3A inlines this: low = __real@7f7fffff x3, high = __real@ff7fffff x3; our /O1 keeps
    // Clear out of line (inlines at /O2 control, or with half the body)
    AABox() { Clear(); }
    void Clear()
    {
        low.z = low.y = low.x = 3.402823466e+38F;
        high.z = high.y = high.x = -3.402823466e+38F;
    }

    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(float x, float y, float z, float w_)
    {
        v.x = x;
        v.y = y;
        v.z = z;
        w = w_;
    }
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

float FastReciprocalSquareRoot(float); // NAME: proto-pdb:default.pdb

enum RegionShape { // NAME: proto-pdb:default.pdb
    REGION_AABB = 0,
    REGION_PLANE = 1,
    REGION_SPHERE = 2,
    REGION_OBB = 3,
    REGION_VERT_CYLINDER = 4,
    REGION_OTHER = 5
};

class SwampSceneNode; // NAME: proto-pdb:default.pdb
struct Matrix; // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

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
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum SwampSceneNodeTypes { // NAME: proto-pdb:default.pdb
        Region = 0xB
    };

    SwampSceneNode(int type);
    const Vector3& SetLocation(const Vector3& location);
    const Vector3& GetLocation() const { return _Location; }
    const Quaternion& SetRotation(const Quaternion& rotation);
    const Quaternion& GetRotation() const { return _Rotation; }
    const SwampString& GetName() const { return _Name; }
    void SetName(const SwampString& name) { _Name = name; }
    unsigned int GetZone() const;
    SwampScript* GetScript() const;
    void SetScript(SwampScript* script);
    void SetScript(const SwampString& name);
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

public:
    char field_4[0x8 - 0x4];           // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x34 - 0x24];        // 0x24
    Vector3 _Location;                 // 0x34
    Quaternion _Rotation;              // 0x40
    char field_50[0xA4 - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class GeometryNode; // NAME: proto-pdb:default.pdb

class SwampDebugGeometry { // NAME: proto-pdb:default.pdb
public:
    GeometryNode* AddBox(const Vector3&, float, float, float, const Vector3&, float);
    GeometryNode* AddRotatedBox(const Vector3&, const Quaternion&, float, float, float, const Vector3&, float);
    GeometryNode* AddSphere(const Vector3&, float, const Vector3&, float);
    GeometryNode* AddCylinder(const Vector3&, float, float, bool, const Vector3&, float);
    GeometryNode* AddPlaneSection(const Vector3&, const Quaternion&, const Vector3&, const Vector3&, float);
};

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void AddNode(SwampSceneNode* node, unsigned int);
};

class SwampNodeManager { // NAME: proto-pdb:default.pdb
public:
    static SwampNodeManager& Instance();
    SwampSceneNode* Create(const SwampString&) const;
    bool CanCreate(const SwampString&) const;
};

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    bool CanCreate(const SwampString&) const;
    static SwampScriptManager& Instance();
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xC];                 // 0x00
    SwampScene* _scene;                // 0x0C
    SwampDebugGeometry* _debugGeometry; // 0x10
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger();
    virtual ~RegionTrigger();

    virtual void Load(const BeepTDF&);                             // vtable+0x24
    virtual void Save(SwampString& str);                           // vtable+0x28
    void TestLineSegmentCollision(SwampSceneNode* node, const Vector3& start, const Vector3& end, bool testEndOnly) const;
    virtual const AABox& GetBounds() const;
    void Enable(bool enable) { _enabled = enable; }
    virtual void Show() const;                                     // vtable+0x2C

protected:
    void Set(const Vector3& origin, const Vector3& size);
    void Set(const Vector3& origin, float radius);
    void Set(const Vector3& origin, float radius, float height);
    void Set(const Vector3& origin, const Quaternion& rotation, const Vector3& size, bool obb);

public:
    RegionShape _shape;                // 0xA4
    AABox _bounds;                     // 0xA8
    Vector3 _size;                     // 0xC0
    Quaternion _unrot;                 // 0xCC
    Vector3 _saveLoc;                  // 0xDC
    Quaternion _saveRot;               // 0xE8
    Vector3 _saveScale;                // 0xF8
    SwampString _saveInstanceName;     // 0x104
    bool _enabled;                     // 0x120
private:
    static SwampSceneNode* creationFunc();
};
DECOMP_SIZE_ASSERT(RegionTrigger, 0x124);

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    RegionManager();
    void Load(const BeepTDF& summary);
    RegionTrigger* Create(const SwampString& className, const SwampString& instanceName, BeepTDF* block);
    void CheckForRegionCrossings(SwampSceneNode* node, const Vector3& start, const Vector3& end, bool testEndOnly) const;
    void NotifyAllRegions(SwampScript* source, int message, void* data);
    void NotifyMatchingRegions(const char* match, SwampScript* source, int message, void* data);

    void SaveRegions(SwampString& str);

    std::multimap<SwampString, RegionTrigger*> _regions; // 0x00
};
DECOMP_SIZE_ASSERT(RegionManager, 0xC);

// FUNCTION: USA 0x000C8AAF
// PROTOTYPE: PREVIEW 0x004B3D56 identical
// NAME: proto-pdb:default.pdb
void RegionTrigger::Set(const Vector3& origin, const Vector3& size)
{
    _size = size;
    // !FAKE: inliner; hand form of the Vector3 operators: stand-ins v0, v1; retail inlines origin -/+ size * 0.5f (C0191)
    Vector3 v0(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
    _bounds.low = Vector3(origin.x - v0.x, origin.y - v0.y, origin.z - v0.z);
    Vector3 v1(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
    _bounds.high = Vector3(v1.x + origin.x, v1.y + origin.y, v1.z + origin.z);
    SetLocation(origin);
    _shape = REGION_AABB;
}

// FUNCTION: USA 0x000C8B59
// PROTOTYPE: PREVIEW 0x004B3E00 identical
// NAME: proto-pdb:default.pdb
void RegionTrigger::Set(const Vector3& origin, const Quaternion& rotation, const Vector3& size, bool obb)
{
    SetLocation(origin);
    SetRotation(rotation);
    _size = size;
    // !FAKE: inliner; hand form of the by-value Quaternion inverse and norm, which /O1 keeps out of line (C0191);
    // stand-ins p, v1, tmp: p keeps the _size copy ahead of the loads, v1 sets the x87 pops (90.36 -> 100)
    const Quaternion* p = &GetRotation();
    float v1 = p->v.x * p->v.x + p->v.y * p->v.y + p->v.z * p->v.z + p->w * p->w;
    float tmp = FastReciprocalSquareRoot(v1);
    _unrot = Quaternion(-(tmp * p->v.x), -(tmp * p->v.y), -(tmp * p->v.z), tmp * p->w);
    if (obb) {
        _shape = REGION_OBB;
        // !FAKE: inliner; hand form of the Vector3 operators: stand-in v0; retail inlines -size * 0.5f (C0191)
        Vector3 v0(-size.x, -size.y, -size.z);
        _bounds.low = Vector3(v0.x * 0.5f, v0.y * 0.5f, v0.z * 0.5f);
        _bounds.high = Vector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
    } else
        _shape = REGION_PLANE;
}

// FUNCTION: USA 0x000C8C71
// PROTOTYPE: PREVIEW 0x004B3F18 identical
// NAME: proto-pdb:default.pdb
void RegionTrigger::Set(const Vector3& origin, float radius)
{
    SetLocation(origin);
    _size = Vector3(radius, radius, radius);
    _shape = REGION_SPHERE;
}

// FUNCTION: USA 0x000C8CAD
// PROTOTYPE: PREVIEW 0x004B3F54 identical
// NAME: proto-pdb:default.pdb
void RegionTrigger::Set(const Vector3& origin, float radius, float height)
{
    SetLocation(origin);
    _size.x = _size.z = radius;
    _size.y = height * 0.5f;
    _shape = REGION_VERT_CYLINDER;
}

// FUNCTION: USA 0x000C8D29
// PROTOTYPE: PREVIEW 0x004B3F8E identical
// NAME: proto-pdb:default.pdb
void RegionTrigger::Show() const
{
    Vector3 clr;
    if (_strnicmp(GetName().c_str(), "areaload", 8) == 0)
        clr.Set(1.0f, 0.0f, 0.0f);
    else if (_strnicmp(GetName().c_str(), "healthwaypoint", 14) == 0)
        clr.Set(0.0f, 0.0f, 1.0f);
    else
        clr.Set(0.0f, 1.0f, 0.0f);
    // !FAKE: c2 register allocation; stand-in pointer locals; retail holds the location and rotation in ecx/edx
    const Vector3* p = &GetLocation();
    const Quaternion* q = &GetRotation();
    switch (_shape) {
    case REGION_AABB:
        g_swamp->_debugGeometry->AddBox(*p, _size.x, _size.y, _size.z, clr, 0.0f);
        break;
    case REGION_PLANE:
        g_swamp->_debugGeometry->AddPlaneSection(*p, *q, _size, clr, 0.0f);
        break;
    case REGION_SPHERE:
        g_swamp->_debugGeometry->AddSphere(*p, _size.x, clr, 0.0f);
        break;
    case REGION_OBB:
        g_swamp->_debugGeometry->AddRotatedBox(*p, *q, _size.x, _size.y, _size.z, clr, 0.0f);
        break;
    case REGION_VERT_CYLINDER:
        g_swamp->_debugGeometry->AddCylinder(*p, _size.y * 2.0f, _size.x, true, clr, 0.0f);
        break;
    }
}

// FUNCTION: USA 0x000C8F3A
// PROTOTYPE: PREVIEW 0x004B419F identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 63.57, bytes mismatch (AABox::AABox/Clear not inlined at /O1; 4.4 T4.c9: the hand-inlined ctor body, no Clear call, still 63.57); vtable slots now named (SwampSceneNode UpdateAnimation/Update/Render protected, as the PDB)
RegionTrigger::RegionTrigger() : SwampSceneNode(Region), _shape(REGION_OTHER), _enabled(true)
{
}
#endif

// FUNCTION: USA 0x000C8F8D
// PROTOTYPE: PREVIEW 0x004B41F2 identical
// NAME: proto-pdb:default.pdb
RegionTrigger::~RegionTrigger()
{
}

// FUNCTION: USA 0x000C9007
// PROTOTYPE: PREVIEW 0x004B426C identical
// NAME: proto-pdb:default.pdb
SwampSceneNode* RegionTrigger::creationFunc()
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\libs\\swamp\\include/RegionTrigger.h", 24) RegionTrigger;
}

// FUNCTION: USA 0x000C9045
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.13, bytes mismatch (frame 0x48 vs 0x54: retail Vector3 slots from inlined by-value helpers our /O1 keeps out of line, C0125)
void RegionTrigger::TestLineSegmentCollision(SwampSceneNode* node, const Vector3& start, const Vector3& end,
                                             bool testendonly) const
{
    // !FAKE: locals; stand-ins v0, v1 for the start-inside and end-inside flags retail holds in dl/cl and al (no PDB local)
    bool v0;
    bool v1;
    if (!_enabled)
        return;
    switch (_shape) {
    case REGION_AABB:
        // !FAKE: inliner; hand form of AABox::Contains, which /O1 keeps out of line (C0191)
        v0 = !testendonly && start.x >= _bounds.low.x && start.y >= _bounds.low.y && start.z >= _bounds.low.z && start.x <= _bounds.high.x &&
             start.y <= _bounds.high.y && start.z <= _bounds.high.z;
        v1 = end.x >= _bounds.low.x && end.y >= _bounds.low.y && end.z >= _bounds.low.z && end.x <= _bounds.high.x &&
             end.y <= _bounds.high.y && end.z <= _bounds.high.z;
        if (!v0) {
            if (v1)
                GetScript()->EnteringRegion(node);
        } else if (!v1)
            GetScript()->LeavingRegion(node);
        break;
    case REGION_PLANE: {
        // !FAKE: inliner; hand form of the Vector3 operators and the by-value Quaternion::Apply (C0191, C0225); stand-ins tmp, v4, v2, v3
        Vector3 starttome(start.x - GetLocation().x, start.y - GetLocation().y, start.z - GetLocation().z);
        Vector3 movevect(end.x - start.x, end.y - start.y, end.z - start.z);
        Vector3 tmp;
        _unrot.Apply(starttome, tmp);
        Vector3 startrel = tmp;
        Vector3 v4;
        _unrot.Apply(movevect, v4);
        Vector3 moverel = v4;
        float endrely = moverel.y + startrel.y;
        if ((startrel.y < 0.0f && endrely >= 0.0f) || (startrel.y >= 0.0f && endrely < 0.0f)) {
            float v2 = startrel.y / fabs(moverel.y);
            Vector3 v3;
            v3.x = moverel.x * v2;
            v3.z = moverel.z * v2;
            Vector3 intersectrel;
            intersectrel.x = startrel.x - v3.x;
            intersectrel.z = startrel.z - v3.z;
            if (fabs(intersectrel.x) < _size.x * 0.5f && fabs(intersectrel.z) < _size.z * 0.5f) {
                if (startrel.y < 0.0f)
                    GetScript()->EnteringRegion(node);
                else
                    GetScript()->LeavingRegion(node);
            }
        }
        break;
    }
    case REGION_SPHERE: {
        // !FAKE: inliner; hand form of the Vector3 difference (C0191)
        Vector3 startrel(start.x - GetLocation().x, start.y - GetLocation().y, start.z - GetLocation().z);
        Vector3 endrel(end.x - GetLocation().x, end.y - GetLocation().y, end.z - GetLocation().z);
        float rangesq = _size.x * _size.x;
        v0 = !testendonly && startrel.LengthSquared() < rangesq;
        v1 = endrel.LengthSquared() < rangesq;
        if (!v0) {
            if (v1)
                GetScript()->EnteringRegion(node);
        } else if (!v1)
            GetScript()->LeavingRegion(node);
        break;
    }
    case REGION_OBB: {
        // !FAKE: inliner; hand form of the Vector3 differences and the by-value Quaternion::Apply (C0191, C0225); stand-in tmp
        Vector3 starttome(start.x - GetLocation().x, start.y - GetLocation().y, start.z - GetLocation().z);
        Vector3 endtome(end.x - GetLocation().x, end.y - GetLocation().y, end.z - GetLocation().z);
        Vector3 tmp;
        _unrot.Apply(starttome, tmp);
        Vector3 startrel = tmp;
        _unrot.Apply(endtome, tmp);
        Vector3 endrel = tmp;
        // !FAKE: inliner; hand form of AABox::Contains, which /O1 keeps out of line (C0191)
        v0 = !testendonly && startrel.x >= _bounds.low.x && startrel.y >= _bounds.low.y && startrel.z >= _bounds.low.z && startrel.x <= _bounds.high.x &&
             startrel.y <= _bounds.high.y && startrel.z <= _bounds.high.z;
        v1 = endrel.x >= _bounds.low.x && endrel.y >= _bounds.low.y && endrel.z >= _bounds.low.z && endrel.x <= _bounds.high.x &&
             endrel.y <= _bounds.high.y && endrel.z <= _bounds.high.z;
        if (!v0) {
            if (v1)
                GetScript()->EnteringRegion(node);
        } else if (!v1)
            GetScript()->LeavingRegion(node);
        break;
    }
    case REGION_VERT_CYLINDER: {
        // !FAKE: inliner; hand form of the Vector3 difference (C0191)
        Vector3 startrel(start.x - GetLocation().x, start.y - GetLocation().y, start.z - GetLocation().z);
        Vector3 endrel(end.x - GetLocation().x, end.y - GetLocation().y, end.z - GetLocation().z);
        float rangesq = _size.x * _size.x;
        v0 = !testendonly && startrel.y >= -_size.y && startrel.y < _size.y &&
             startrel.x * startrel.x + startrel.z * startrel.z < rangesq;
        v1 = endrel.y >= -_size.y && endrel.y < _size.y && endrel.x * endrel.x + endrel.z * endrel.z < rangesq;
        if (!v0) {
            if (v1)
                GetScript()->EnteringRegion(node);
        } else if (!v1)
            GetScript()->LeavingRegion(node);
        break;
    }
    }
}
#endif

// FUNCTION: USA 0x000C95C3
// NAME: proto-pdb:default.pdb
void RegionManager::CheckForRegionCrossings(SwampSceneNode* node, const Vector3& start, const Vector3& end,
                                            bool testEndOnly) const
{
    for (std::multimap<SwampString, RegionTrigger*>::const_iterator rit = _regions.begin(); rit != _regions.end();
         ++rit)
        rit->second->TestLineSegmentCollision(node, start, end, testEndOnly);
}

// FUNCTION: USA 0x000C95FF
// NAME: proto-pdb:default.pdb
void RegionManager::NotifyAllRegions(SwampScript* source, int message, void* data)
{
    for (std::multimap<SwampString, RegionTrigger*>::iterator rit = _regions.begin(); rit != _regions.end(); ++rit)
        rit->second->GetScript()->Message(source, message, data);
}

// FUNCTION: USA 0x000C963F
// PROTOTYPE: PREVIEW 0x004B4888 identical
// NAME: proto-pdb:default.pdb
void RegionManager::NotifyMatchingRegions(const char* match, SwampScript* source, int message, void* data)
{
    // !FAKE: local; retail holds strlen(match) in ebx across the loop (no PDB local)
    unsigned int len = strlen(match);
    for (std::multimap<SwampString, RegionTrigger*>::iterator rit = _regions.begin(); rit != _regions.end(); ++rit) {
        // !FAKE: local; retail holds rit->second in esi across the compare (no PDB local)
        RegionTrigger* trigger = rit->second;
        if (trigger && _strnicmp(rit->first.c_str(), match, len) == 0)
            trigger->GetScript()->Message(source, message, data);
    }
}

// FUNCTION: USA 0x000C9AE6
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.90, bytes mismatch (frame 0x34 vs 0x38: retail gives the Save() temp typeStrings' dead slot, ours shares the format temps' slot; 6 drafts)
void RegionTrigger::Save(SwampString& s)
{
    const char* typeStrings[6] = { "Cube", "Plane", "Sphere", "Cube", "Cylinder", "Other" };
    s += format("\t\tZone = %d;\n", GetZone());
    s += format("\t\tType = %s;\n", typeStrings[_shape]);
    s += format("\t\tLocation = %f %f %f;\n", _saveLoc.x, _saveLoc.y, _saveLoc.z);
    s += format("\t\tRotation = %f %f %f %f;\n", _saveRot.v.x, _saveRot.v.y, _saveRot.v.z, _saveRot.w);
    s += format("\t\tScale = %f %f %f;\n", _saveScale.x, _saveScale.y, _saveScale.z);
    if (_saveInstanceName != "")
        s += format("\t\tinstancename = %s;\n", _saveInstanceName.c_str());
    s += format("\t\tenabled = %i;\n", _enabled);
    s += GetScript()->Save();
}
#endif

// FUNCTION: USA 0x000C9D08
// NAME: proto-pdb:default.pdb
void RegionManager::SaveRegions(SwampString& str)
{
    str += "[Regions]\n{\n";
    for (std::multimap<SwampString, RegionTrigger*>::iterator rit = _regions.begin(); rit != _regions.end(); ++rit) {
        // !FAKE: c2 register allocation; dummy local; retail holds rit->second in esi across the loop body (no PDB local)
        RegionTrigger* trigger = rit->second;
        str += format("\t[%s]\n\t{\n", trigger->GetName().c_str());
        trigger->Save(str);
        str += format("\t}\n");
    }
    str += "}\n";
}

// FUNCTION: USA 0x000CA043
// PROTOTYPE: PREVIEW 0x004B528C identical
// NAME: proto-pdb:default.pdb
RegionManager::RegionManager()
{
}

// FUNCTION: USA 0x000CA08B
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.36, bytes mismatch (loop GetZone order: retail calls trigger's first into ebx, cl always calls it->second's first; 11 drafts)
RegionTrigger* RegionManager::Create(const SwampString& classname, const SwampString& instancename, BeepTDF* tdf)
{
    // !FAKE: c1xx front end; neutral stand-in local; retail calls SwampNodeManager::Instance once and holds it in edi (no PDB local)
    const SwampNodeManager& v0 = SwampNodeManager::Instance();
    const SwampScriptManager& scriptMgr = SwampScriptManager::Instance();
    SwampString name = instancename + "region";
    if (!v0.CanCreate(name))
        name = classname + "region";
    RegionTrigger* trigger = 0;
    if (v0.CanCreate(name))
        trigger = (RegionTrigger*)v0.Create(name);
    if (!trigger)
        // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__
        trigger = new("RegionTrigger.cpp", 406) RegionTrigger;
    trigger->SetName(classname);
    if (tdf)
        trigger->Load(*tdf);
    else {
        g_swamp->_scene->AddNode(trigger, 0);
        trigger->Enable(false);
    }
    for (std::multimap<SwampString, RegionTrigger*>::iterator it = _regions.lower_bound(instancename);
         it != _regions.upper_bound(instancename); ++it) {
        if (it->second->GetName() == classname && trigger->GetZone() == it->second->GetZone()) {
            trigger->SetScript(it->second->GetScript());
            break;
        }
    }
    if (!trigger->GetScript()) {
        SwampString name = instancename + "regionmain";
        if (scriptMgr.CanCreate(name))
            trigger->SetScript(name);
        else
            trigger->SetScript(classname + "regionmain");
        if (tdf)
            trigger->GetScript()->Load(*tdf);
        trigger->GetScript()->SetNode(trigger);
    }
    _regions.insert(std::pair<const SwampString, RegionTrigger*>(instancename, trigger));
    return trigger;
}
#endif

// FUNCTION: USA 0x000CA2B4
// PROTOTYPE: PREVIEW 0x004B54FD identical
// NAME: proto-pdb:default.pdb
void RegionManager::Load(const BeepTDF& summary)
{
    // !FAKE: c1xx front end; neutral stand-in local; the block pointer has no PDB local name
    const BeepTDF* p = summary.FindBlock("Regions", false);
    // !FAKE: c1xx front end; neutral stand-in local; the iterator has no PDB local name
    for (std::vector<BeepTDF*>::const_iterator v0 = p->GetBlocks().begin(); v0 != p->GetBlocks().end(); ++v0) {
        SwampString className = (*v0)->GetBlockName();
        SwampString instanceName = (*v0)->GetStringRecord("instanceName", "");
        Create(className, instanceName, *v0);
    }
}

namespace std {
template <class Tr> void _Tree<Tr>::_Lrotate(_Node* _Wherenode)
{
    _Node* _Pnode = _Wherenode->_Right;
    _Wherenode->_Right = _Pnode->_Left;

    if (!_Pnode->_Left->_Isnil)
        _Pnode->_Left->_Parent = _Wherenode;
    _Pnode->_Parent = _Wherenode->_Parent;

    if (_Wherenode == _Myhead->_Parent)
        _Myhead->_Parent = _Pnode;
    else if (_Wherenode == _Wherenode->_Parent->_Left)
        _Wherenode->_Parent->_Left = _Pnode;
    else
        _Wherenode->_Parent->_Right = _Pnode;

    _Pnode->_Left = _Wherenode;
    _Wherenode->_Parent = _Pnode;
}
// _Lbound body visible: its fold VA 0x0010A38F is Locator.obj's row; our RegionTrigger-alias COMDAT is the
// byte-equal fold duplicate relink merges (C0233 Fix 1); out of class, so cl keeps it out of line as retail calls it;
// only RegionManager::Create (under NON_MATCHING) instantiates it
template <class Tr> typename _Tree<Tr>::_Node* _Tree<Tr>::_Lbound(const typename Tr::key_type& _Keyval) const
{
    _Node* _Pnode = _Myhead->_Parent;
    _Node* _Wherenode = _Myhead;
    while (!_Pnode->_Isnil)
        if (this->comp(_Pnode->_Myval.first, _Keyval))
            _Pnode = _Pnode->_Right;
        else {
            _Wherenode = _Pnode;
            _Pnode = _Pnode->_Left;
        }
    return (_Wherenode);
}
template <class Tr> typename _Tree<Tr>::_Node* _Tree<Tr>::_Ubound(const typename Tr::key_type& _Keyval) const
{
    _Node* _Pnode = _Myhead->_Parent;
    _Node* _Wherenode = _Myhead;
    while (!_Pnode->_Isnil)
        if (this->comp(_Keyval, _Pnode->_Myval.first)) {
            _Wherenode = _Pnode;
            _Pnode = _Pnode->_Left;
        } else
            _Pnode = _Pnode->_Right;
    return (_Wherenode);
}
}

// FUNCTION: USA 0x000C8CE7
// PROTOTYPE: PREVIEW 0x004A89D3 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::_Lrotate(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::_Node *);

// FUNCTION: USA 0x000C8FAD
// PROTOTYPE: PREVIEW 0x004B4212 identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,RegionTrigger *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,RegionTrigger *> >,1> >::_Node * std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,RegionTrigger *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,RegionTrigger *> >,1> >::_Ubound(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &)const;
