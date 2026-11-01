// UNIT: src/Libs/Swamp/Xbox/Official_Release/RegionTrigger.cpp OBJ: Xbox\Official_Release\RegionTrigger.obj OPT: /O1 /Oy
#include <string.h>
#pragma intrinsic(strlen) // src proto-pdb:default.pdb strlen
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    const E* c_str() const { return _Myptr(); }
protected:
    const E* _Myptr() const { return 16 <= _Myres ? _Bx._Ptr : _Bx._Buf; }

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
template <class T> struct less;
template <class T1, class T2> struct pair {
    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
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
    class const_iterator {
    public:
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const typename Tr::value_type* operator->() const { return &_Ptr->_Myval; }
        const_iterator& operator++() { _Inc(); return *this; }
        bool operator!=(const const_iterator& _Right) const { return _Ptr != _Right._Ptr; }
        void _Inc();
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

protected:
    _Tree() { _Init(); }
    void _Init();

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

void* operator new(unsigned int size, const char* file, int line);

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

enum RegionShape { // NAME: proto-pdb:default.pdb
    REGION_AABB = 0,
    REGION_PLANE = 1,
    REGION_SPHERE = 2,
    REGION_OBB = 3,
    REGION_VERT_CYLINDER = 4,
    REGION_OTHER = 5
};

class SwampScript { // NAME: proto-pdb:default.pdb
public:
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
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    const Vector3& SetLocation(const Vector3& location);
    SwampScript* GetScript() const;
    virtual ~SwampSceneNode();                                     // vtable+0x00

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger();

    void TestLineSegmentCollision(SwampSceneNode* node, const Vector3& start, const Vector3& end, bool testEndOnly) const;

protected:
    void Set(const Vector3& origin, float radius);
    void Set(const Vector3& origin, float radius, float height);

public:
    RegionShape _shape;                // 0xA4
    char field_A8[0xC0 - 0xA8];        // 0xA8
    Vector3 _size;                     // 0xC0
    char field_CC[0x124 - 0xCC];       // 0xCC
private:
    static SwampSceneNode* creationFunc();
};
DECOMP_SIZE_ASSERT(RegionTrigger, 0x124);

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    RegionManager();
    void CheckForRegionCrossings(SwampSceneNode* node, const Vector3& start, const Vector3& end, bool testEndOnly) const;
    void NotifyAllRegions(SwampScript* source, int message, void* data);
    void NotifyMatchingRegions(const char* match, SwampScript* source, int message, void* data);

    std::multimap<SwampString, RegionTrigger*> _regions; // 0x00
};
DECOMP_SIZE_ASSERT(RegionManager, 0xC);

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

// FUNCTION: USA 0x000C9007
// PROTOTYPE: PREVIEW 0x004B426C identical
// NAME: proto-pdb:default.pdb
SwampSceneNode* RegionTrigger::creationFunc()
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\libs\\swamp\\include/RegionTrigger.h", 24) RegionTrigger;
}

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

// FUNCTION: USA 0x000CA043
// PROTOTYPE: PREVIEW 0x004B528C identical
// NAME: proto-pdb:default.pdb
RegionManager::RegionManager()
{
}
