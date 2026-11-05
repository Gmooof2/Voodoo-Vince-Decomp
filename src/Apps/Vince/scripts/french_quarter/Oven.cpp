// UNIT: src/Apps/Vince/scripts/french_quarter/Oven.cpp OBJ: Xbox\OfficialRelease\Oven.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

#pragma warning(disable: 4530)
#include <string>
#include <deque>
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class SoundNode;             // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> void deque<Vector3,std::allocator<Vector3> >::_Tidy();
// retail 0x0001D5C2 (named fold alias); the VC 7.1 body (throw length_error) emits unnamed throw-info data (C0236)
template <> void deque<Vector3,std::allocator<Vector3> >::_Xlen() const;
template <> Vector3** _Uninitialized_copy(Vector3**, Vector3**, Vector3**, std::allocator<Vector3*>&); // 0x0007F3FD
template <> basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string(const char *);
template <> void basic_string<char,std::char_traits<char>,std::allocator<char> >::_Tidy(bool,unsigned int);
}
#endif

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
    ResourceHandle() : _Resource(0) {}
    ResourceHandle& operator=(const ResourceHandle& other);
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

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

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_STATIC = 0x4
};

// Retail layout: Critter _CritterType 0x258 -> 0x268 (+0x10).
class Critter { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x268];               // 0x00
    CritterType _CritterType;          // 0x268
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

    void RegisterTimerEvent(unsigned int, float, bool);
    void RemoveTimerEvents();

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

// FUNCTION: USA 0x0007B39C
// NAME: proto-pdb:default.pdb
class OvenMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    OvenMain(SwampSceneNode* node);
    virtual void Activate();
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

    ResourceHandle<AnimationResource> _open; // 0xBC
    bool _active;                      // 0xC0
    bool _makeCookies;                 // 0xC1
    bool _noMoreCookies;               // 0xC2
    int _cookieNum;                    // 0xC4
    int _bakeCount;                    // 0xC8
    SoundNode* _beltSound;             // 0xCC
    std::deque<Vector3> _gingerDMLocs; // 0xD0
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(OvenMain, 0xE4);

#ifdef NON_MATCHING
// row USA 0x0007B188, PREVIEW 0x0046718F identical
// best: match_one objdiff 100.0, bytes match; wall: provenance_lint nested operator (C0212): no NAME tag, so
// no FUNCTION tag (4.3 T7.c1)
template Vector3 const & std::deque<Vector3,std::allocator<Vector3> >::const_iterator::operator*()const;
#endif

// FUNCTION: USA 0x0007B602
// PROTOTYPE: PREVIEW 0x00467609 identical
// NAME: proto-pdb:default.pdb
// VC 7.1 <deque> body verbatim as an explicit specialization: the header's _Xlen cannot be redeclared noreturn
// (C2381), so `__assume(0)` restates its throw body's effect (C0237, !FAKE) and no throw-info is emitted
template <> void std::deque<Vector3,std::allocator<Vector3> >::_Growmap(size_type _Count)
{
    if (max_size() / _DEQUESIZ - _Mapsize < _Count) {
        _Xlen();
        __assume(0); // !FAKE: noreturn restated (C0237)
    }

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

// FUNCTION: USA 0x0007B707
// PROTOTYPE: PREVIEW 0x0046770E identical
// NAME: proto-pdb:default.pdb
template void std::deque<Vector3,std::allocator<Vector3> >::push_back(Vector3 const &);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

// FUNCTION: USA 0x0007B15E
// PROTOTYPE: PREVIEW 0x00467165 identical
// NAME: proto-pdb:default.pdb
void OvenMain::Activate()
{
    _active = true;
    RemoveTimerEvents();
    if (!_noMoreCookies)
        RegisterTimerEvent(0, 1.0f, false);
}

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0007B2AF
// PROTOTYPE: PREVIEW 0x004672B6 identical
// NAME: proto-pdb:default.pdb
SwampString OvenMain::Save()
{
    return format("\t\tnomorecookies=%d;\n", _noMoreCookies);
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2 = vptr store, literal)
// FUNCTION: USA 0x0007B2CF
// PROTOTYPE: PREVIEW 0x004672D6 near-identical
// NAME: proto-pdb:default.pdb
OvenMain::OvenMain(SwampSceneNode* node)
    : CritterMain(node), _active(false), _makeCookies(false), _noMoreCookies(false), _cookieNum(0), _bakeCount(0),
      _beltSound(0)
{
    _open = g_swamp->GetAnimationManager()->Load("oven_doorOpen.anim");
    ((Critter*)_node)->_CritterType = CRITTER_TYPE_STATIC;
    _state = 0;
}
#endif

// FUNCTION: USA 0x0007B5A0
// NAME: proto-pdb:default.pdb
void OvenMain::Load(const BeepTDF& tdf)
{
    _noMoreCookies = tdf.GetBoolRecord("nomorecookies", false);
}

// FUNCTION: USA 0x0007B5DB
// PROTOTYPE: PREVIEW 0x004675E2 identical
// NAME: proto-pdb:default.pdb
SwampScript* OvenMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\bakery\\Oven.h", 21) OvenMain(node);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1OvenMain@@UAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_OvenMain_dtor(OvenMain* p)
{
    p->OvenMain::~OvenMain();
}
