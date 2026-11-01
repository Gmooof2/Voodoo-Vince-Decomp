// UNIT: src/Apps/Vince/core/SignTrolley.cpp OBJ: core\XBox\OfficialRelease\SignTrolley.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[0x1C / 4];
};
template <class T, class A = allocator<T> > class vector {
public:
    ~vector() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x10];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
typedef std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > SwampWString;

struct Vector2 { // NAME: proto-pdb:default.pdb
    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class BeepTDF;               // NAME: proto-pdb:default.pdb
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
    virtual void InvalidateScript();                               // vtable+0x60

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

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

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x34 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& h);

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

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

// FUNCTION: USA 0x0012D02D
// PROTOTYPE: PREVIEW 0x00515A5C identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012CFBA
// PROTOTYPE: PREVIEW 0x005159E9 identical
// NAME: proto-pdb:default.pdb
class Sprite { // NAME: proto-pdb:default.pdb
public:
    Sprite();

    Vector2 _loc;                      // 0x00
    int _w;                            // 0x08
    int _h;                            // 0x0C
    Vector2 _scale;                    // 0x10
    ResourceHandle<TextureResource> _texture; // 0x18
    bool _centered;                    // 0x1C
    float _alpha;                      // 0x20
    Vector2 _uv[4];                    // 0x24
    unsigned int _color;               // 0x44
    float _rotation;                   // 0x48
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

inline void* operator new(unsigned int, void* p) { return p; }

// FUNCTION: USA 0x0012D0FE
// PROTOTYPE: PREVIEW 0x00515B2D identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012D11A
// PROTOTYPE: PREVIEW 0x00515B49 identical
// NAME: proto-pdb:default.pdb
struct LocInfo { // NAME: proto-pdb:default.pdb
    SwampString _levelName;            // 0x00
    SwampString _areaName;             // 0x1C
    SwampString _trolleyName;          // 0x38
};
DECOMP_SIZE_ASSERT(LocInfo, 0x54);

class TrolleyManager { // NAME: proto-pdb:default.pdb
public:
    class MapStop;                     // NAME: proto-pdb:default.pdb
};

// FUNCTION: USA 0x0012D15A
// PROTOTYPE: PREVIEW 0x00515B89 identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012DDD5
// PROTOTYPE: PREVIEW 0x00516767 identical
// NAME: proto-pdb:default.pdb
class TrolleyManager::MapStop { // NAME: proto-pdb:default.pdb
public:
    SwampString _name;                 // 0x00
    Vector2 _screenLoc;                // 0x1C
    SwampString _spriteName;           // 0x24
};
DECOMP_SIZE_ASSERT(TrolleyManager::MapStop, 0x40);

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xD4 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampCamera();
    char field_A4[0x10C - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class TrolleyScreen : public UINode { // NAME: proto-pdb:default.pdb
public:
    struct MapInfo;                    // NAME: proto-pdb:default.pdb

    char field_D4[0xE0 - 0xD4];        // 0xD4
    std::vector<MapInfo> _locations;   // 0xE0
    Sprite _mapTexture;                // 0xF0
    std::vector<Sprite> _areaSelect;   // 0x13C
    char field_14C[0x16C - 0x14C];     // 0x14C
    SwampCamera _cam;                  // 0x16C
    char field_278[0x2BC - 0x278];     // 0x278
    SwampString _curTrack;             // 0x2BC
};
DECOMP_SIZE_ASSERT(TrolleyScreen, 0x2D8);

// FUNCTION: USA 0x0012D192
// PROTOTYPE: PREVIEW 0x00515BC1 identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0012DD94
// PROTOTYPE: PREVIEW 0x00516726 identical
// NAME: proto-pdb:default.pdb
struct TrolleyScreen::MapInfo { // NAME: proto-pdb:default.pdb
    SwampString _name;                 // 0x00
    SwampWString _screenName;          // 0x1C
    Vector2 _loc;                      // 0x38
    Sprite _sprite;                    // 0x40
};
DECOMP_SIZE_ASSERT(TrolleyScreen::MapInfo, 0x8C);

enum CritterType { // NAME: proto-pdb:default.pdb
    CRITTER_TYPE_STATIC = 0x4
};

// Retail layout: Critter _CritterType 0x258 -> 0x268 (+0x10).
class Critter { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x268];               // 0x00
    CritterType _CritterType;          // 0x268
};

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    char field_4C[0xBC - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class SignTrolleyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    SignTrolleyMain(SwampSceneNode* node);
    virtual bool TestActivate() const;

    ResourceHandle<AnimationResource> _animActive; // 0xBC
    Critter* _trolley;                 // 0xC0
    bool _tagged;                      // 0xC4
    SwampString _stopName;             // 0xC8
    SwampString _trolleyName;          // 0xE4
    SwampScript* _padScript;           // 0x100
};
DECOMP_SIZE_ASSERT(SignTrolleyMain, 0x104);

// FUNCTION: USA 0x0012CDB0
// PROTOTYPE: PREVIEW 0x005157B9 identical
// NAME: proto-pdb:default.pdb
bool SignTrolleyMain::TestActivate() const
{
    if (_trolley != 0 && _tagged)
        return true;
    return false;
}

// The compiler generates the Sprite copy constructor here; it has no body in source.
// FUNCTION: USA 0x0012CE60
// PROTOTYPE: PREVIEW 0x0051588F identical
// NAME: proto-pdb:default.pdb

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~MapStop in this unit (no matched function
// odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapStop_dtor(TrolleyManager::MapStop* p)
{
    p->TrolleyManager::MapStop::~MapStop();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit LocInfo default ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LocInfo_ctor(void* p)
{
    new (p) LocInfo;
}

// FUNCTION: USA 0x0012D176
// PROTOTYPE: PREVIEW 0x00515BA5 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapInfo default ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapInfo_ctor(void* p)
{
    new (p) TrolleyScreen::MapInfo;
}

// !FAKE: c1xx front end; scaffold call; retail emits the Sprite scalar deleting dtor in this unit (no matched
// function odr-uses it); this delete odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Sprite_delete(Sprite* p)
{
    delete p;
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~MapInfo in this unit (no matched function
// odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapInfo_dtor(TrolleyScreen::MapInfo* p)
{
    p->TrolleyScreen::MapInfo::~MapInfo();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~LocInfo in this unit (no matched function
// odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LocInfo_dtor(LocInfo* p)
{
    p->LocInfo::~LocInfo();
}

// FUNCTION: USA 0x0012D241
// PROTOTYPE: PREVIEW 0x00515C70 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit LocInfo copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LocInfo_copy(void* p, const LocInfo& v)
{
    new (p) LocInfo(v);
}

// FUNCTION: USA 0x0012D2EA
// PROTOTYPE: PREVIEW 0x00515D19 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapStop copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapStop_copy(void* p, const TrolleyManager::MapStop& v)
{
    new (p) TrolleyManager::MapStop(v);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapStop::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapStop_assign(TrolleyManager::MapStop* p, const TrolleyManager::MapStop& v)
{
    *p = v;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2 = vptr store, literal)
// FUNCTION: USA 0x0012D338
// PROTOTYPE: PREVIEW 0x00515D67 near-identical
// NAME: proto-pdb:default.pdb
SignTrolleyMain::SignTrolleyMain(SwampSceneNode* node) : CritterMain(node), _tagged(false), _padScript(0)
{
    ((Critter*)_node)->_CritterType = CRITTER_TYPE_STATIC;
    _animActive = g_swamp->GetAnimationManager()->Load("signtrolley_activate");
}
#endif

// FUNCTION: USA 0x0012D3EE
// PROTOTYPE: PREVIEW 0x00515E1D identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~SignTrolleyMain in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_SignTrolleyMain_dtor(SignTrolleyMain* p)
{
    p->SignTrolleyMain::~SignTrolleyMain();
}

// FUNCTION: USA 0x0012D26E
// PROTOTYPE: PREVIEW 0x00515C9D identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapInfo copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapInfo_copy(void* p, const TrolleyScreen::MapInfo& v)
{
    new (p) TrolleyScreen::MapInfo(v);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit MapInfo::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_MapInfo_assign(TrolleyScreen::MapInfo* p, const TrolleyScreen::MapInfo& v)
{
    *p = v;
}

// FUNCTION: USA 0x0012F27D
// PROTOTYPE: PREVIEW 0x00517CB0 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~TrolleyScreen in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_TrolleyScreen_dtor(TrolleyScreen* p)
{
    p->TrolleyScreen::~TrolleyScreen();
}
