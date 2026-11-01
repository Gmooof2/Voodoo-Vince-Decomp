// UNIT: src/Apps/Vince/scripts/Carnival/Kosmo.cpp OBJ: Xbox\OfficialRelease\Kosmo.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
template <class T, class A = allocator<T> > class vector {
public:
    ~vector() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class T, class A = allocator<T> > class deque {
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    std::deque<T> _queue;              // 0x00
};

class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line);

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

    char field_4[0x4C - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();

    char field_4C[0xBC - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class KosmoBotLegsMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);

    char field_BC[0x130 - 0xBC];       // 0xBC
};
DECOMP_SIZE_ASSERT(KosmoBotLegsMain, 0x130);

// FUNCTION: USA 0x00049BD0
// PROTOTYPE: PREVIEW 0x004374D7 identical
// NAME: proto-pdb:default.pdb
void KosmoBotLegsMain::CollidedWith(CollisionType type, SwampSceneNode* nodehitby,
                                    const Ptr<CollisionResultObject>& cr)
{
    HitBy(type, nodehitby, cr);
}

class Camera_elevatorMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Camera_elevatorMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Camera_elevatorMain, 0x4C);

// FUNCTION: USA 0x00049BED
// PROTOTYPE: PREVIEW 0x004374F4 identical
// NAME: proto-pdb:default.pdb
SwampScript* Camera_elevatorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BumperCarBumpoff\\Kosmo.cpp", 18) Camera_elevatorMain(node);
}

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

    T* _Resource;                      // 0x00
};

// FUNCTION: USA 0x0004C9D4
// NAME: proto-pdb:default.pdb
class KosmoBotHeadMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    std::vector<int> _cannonJoint;     // 0xBC
    char field_CC[0xDC - 0xCC];        // 0xCC
    ResourceHandle<AnimationResource> _animIdle;   // 0xDC
    ResourceHandle<AnimationResource> _animAttack; // 0xE0
    ResourceHandle<AnimationResource> _animDamage; // 0xE4
    ResourceHandle<AnimationResource> _animDeath;  // 0xE8
};
DECOMP_SIZE_ASSERT(KosmoBotHeadMain, 0xEC);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1KosmoBotHeadMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_KosmoBotHeadMain_dtor(KosmoBotHeadMain* p)
{
    p->KosmoBotHeadMain::~KosmoBotHeadMain();
}

// FUNCTION: USA 0x0004D2DD
// NAME: proto-pdb:default.pdb
class KosmoBotMidriffMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    std::vector<Vector3> _cannonLocs;  // 0xBC
    SwampShuffleQueue<SwampString> _handSmackSounds; // 0xCC
};
DECOMP_SIZE_ASSERT(KosmoBotMidriffMain, 0xE0);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1KosmoBotMidriffMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_KosmoBotMidriffMain_dtor(KosmoBotMidriffMain* p)
{
    p->KosmoBotMidriffMain::~KosmoBotMidriffMain();
}
