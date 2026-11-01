// UNIT: src/Apps/Vince/core/WaterRegion.cpp OBJ: core\XBox\OfficialRelease\WaterRegion.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string() { _Myres = 15; _Eos(0); }
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);
    void _Eos(unsigned int _Newsize);

private:
    char _pad00[0x18];
    unsigned int _Myres; // 0x18
};
template <class T, class A = allocator<T> > class vector {
public:
    unsigned int size() const;
    void push_back(const T& _Val);

private:
    char _pad00[0x10];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

inline void* operator new(unsigned int, void* p) { return p; }

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x8];                 // 0x00
    SwampString _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
    Vector2() {}
    void Set(float u_, float v_) { u = u_; v = v_; }
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

class PuddleRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    struct Splasher;                   // NAME: proto-pdb:default.pdb

    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void LeavingRegion(SwampSceneNode* node);

private:
    void RemoveCritterFromSplashList(const SwampString&);

    char field_4C[0x68 - 0x4C];        // 0x4C
    std::vector<Splasher> _splashers;  // 0x68
    unsigned int _nextID;              // 0x78
};
DECOMP_SIZE_ASSERT(PuddleRegionMain, 0x7C);

// FUNCTION: USA 0x0012A5D1
// PROTOTYPE: PREVIEW 0x00512FF6 identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012B449
// PROTOTYPE: PREVIEW 0x00513E6E identical
// NAME: proto-pdb:default.pdb
struct PuddleRegionMain::Splasher { // NAME: proto-pdb:default.pdb
    SwampString name;                  // 0x00
    unsigned int id;                   // 0x1C
    Vector2 lastPos;                   // 0x20
};
DECOMP_SIZE_ASSERT(PuddleRegionMain::Splasher, 0x28);

// FUNCTION: USA 0x0012BCB2
// PROTOTYPE: PREVIEW 0x005146D7 identical
// NAME: proto-pdb:default.pdb
void PuddleRegionMain::EnteringRegion(SwampSceneNode* node)
{
    RemoveCritterFromSplashList(node->_Name);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit Splasher copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Splasher_copy(void* p, const PuddleRegionMain::Splasher& v)
{
    new (p) PuddleRegionMain::Splasher(v);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit Splasher::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Splasher_assign(PuddleRegionMain::Splasher* p, const PuddleRegionMain::Splasher& v)
{
    *p = v;
}

// FUNCTION: USA 0x0012BD02
// PROTOTYPE: PREVIEW 0x00514727 identical
// NAME: proto-pdb:default.pdb
void PuddleRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (_splashers.size() == 0) {
        RegisterFrameTick();
    }
    Splasher splasher;
    splasher.name = node->_Name;
    splasher.id = _nextID++;
    splasher.lastPos.Set(3.4028235e38f, 3.4028235e38f);
    _splashers.push_back(splasher);
}
