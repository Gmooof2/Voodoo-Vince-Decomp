// UNIT: src/Apps/Vince/scripts/Bayou/CrawdadJimmys.cpp OBJ: Xbox\OfficialRelease\CrawdadJimmys.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
struct _Nonscalar_ptr_iterator_tag {
};
template <class T> inline void _Destroy(T* _Ptr)
{
    (_Ptr)->~T();
}
template <class T> class allocator {
public:
    void destroy(T* _Ptr) { _Destroy(_Ptr); }
};
template <class T, class A> inline void _Destroy_range(T* _First, T* _Last, A& _Al, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_First)
        _Al.destroy(_First);
}
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    unsigned int size() const { return _Mysize; }
    int compare(const basic_string& _Right) const { return compare(0, _Mysize, _Right._Myptr(), _Right.size()); }
    int compare(unsigned int _Off, unsigned int _N0, const E* _Ptr, unsigned int _Count) const;
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    A _Alval;                          // 0x00
    union {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A> inline bool operator<(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right)
{
    return (_Left.compare(_Right) < 0);
}
template <class T> struct less; // src proto-pdb:default.pdb less
template <class T1, class T2> struct pair { // src proto-pdb:default.pdb pair
    T1 first;                          // 0x00 // src proto-pdb:default.pdb first
    T2 second; // src proto-pdb:default.pdb second
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits { // src proto-pdb:default.pdb _Tmap_traits
public:
    typedef K key_type; // src proto-pdb:default.pdb key_type
    typedef pair<const K, T> value_type;
};
template <class Tr> class _Tree { // src proto-pdb:default.pdb _Tree
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
        bool operator==(const const_iterator& _Right) const { return _Ptr == _Right._Ptr; }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
        const typename Tr::value_type* operator->() const { return &this->_Ptr->_Myval; }
        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
        typename Tr::value_type* operator->() const { return &this->_Ptr->_Myval; }
    };
    iterator end() { return iterator(_Myhead); }
    iterator find(const typename Tr::key_type& _Keyval); // src proto-pdb:default.pdb find

private:
    char _Alval;                       // 0x00 // src proto-pdb:default.pdb _Alval
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class multimap // src proto-pdb:default.pdb multimap
    : public _Tree<_Tmap_traits<K, T, Pr, A, true> > {
};
template <class T, class A> class vector {
public:
    ~vector() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x10];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

// FUNCTION: USA 0x00019E98
// PROTOTYPE: PREVIEW 0x004090F9 identical
// NAME: proto-pdb:default.pdb
template int std::basic_string<char,std::char_traits<char>,std::allocator<char> >::compare(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &)const;

// FUNCTION: USA 0x00019EBC
// PROTOTYPE: PREVIEW 0x0040911D identical
// NAME: proto-pdb:default.pdb
template bool std::operator< <char,std::char_traits<char>,std::allocator<char> >(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &);

// FUNCTION: USA 0x0001A50A
// PROTOTYPE: PREVIEW 0x00409787 identical
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >(std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > &,std::_Nonscalar_ptr_iterator_tag);

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1
    };
    const Quaternion& SetRotation(const Quaternion& q);
    void SetVisible(bool b, bool r);
    void SetOption(unsigned int, bool, bool);
    bool GetRenderingOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    bool IsValid() const { return !GetRenderingOption(Invalid); }
protected:
    void InvalidateCachedWorldPos();
public:
    char field_0[0x40];                // 0x00
    Quaternion _Rotation;              // 0x40
    char field_50[0x88 - 0x50];        // 0x50
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

void* operator new(unsigned int size, const char* file, int line);

// FUNCTION: USA 0x00019D86
// PROTOTYPE: PREVIEW 0x00408FE7 identical
// NAME: proto-pdb:default.pdb
const Quaternion& SwampSceneNode::SetRotation(const Quaternion& q)
{
    InvalidateCachedWorldPos();
    return _Rotation = q;
}

// FUNCTION: USA 0x00019DA1
// PROTOTYPE: PREVIEW 0x00409002 identical
// NAME: proto-pdb:default.pdb
void SwampSceneNode::SetVisible(bool b, bool r)
{
    SetOption(2, !b, r);
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

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class CrawdadJimmyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_CHECKTIME = 1,
        STATE_ASK_CRAWDAD = 2,
        STATE_ASK_ONION = 3,
        STATE_ASK_SAUSAGE = 4,
        STATE_V_NOTHING = 5,
        STATE_GOTALL = 6
    };

    CrawdadJimmyMain(SwampSceneNode* node);
    virtual bool TestActivate() const;
    virtual void GainingControl(SwampScript*);
    virtual void SetState(int);

    bool _didintro;                    // 0xBC
    bool _didchallenge;                // 0xBD
    char field_BE[0x6];                // 0xBE

private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    void ResetAmbients();
    void GiveHint(int item);
};
DECOMP_SIZE_ASSERT(CrawdadJimmyMain, 0xC4);

// FUNCTION: USA 0x00019E7A
// PROTOTYPE: PREVIEW 0x004090DB identical
// NAME: proto-pdb:default.pdb
void CrawdadJimmyMain::GainingControl(SwampScript* oldScript)
{
    SetState(STATE_IDLE);
    if (_didintro) {
        ResetAmbients();
    }
}

// FUNCTION: USA 0x00019FD2
// PROTOTYPE: PREVIEW 0x00409233 identical
// NAME: proto-pdb:default.pdb
class ActivatorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    bool _useable;                     // 0xBC
    float _alpha;                      // 0xC0
    SwampString _target;               // 0xC4
};
DECOMP_SIZE_ASSERT(ActivatorMain, 0xE0);

class Activator_FanBoatMain : public ActivatorMain { // NAME: proto-pdb:default.pdb
};
DECOMP_SIZE_ASSERT(Activator_FanBoatMain, 0xE0);

class Activator_FanBoatAMain : public Activator_FanBoatMain { // NAME: proto-pdb:default.pdb
public:
    Activator_FanBoatAMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Activator_FanBoatAMain, 0xE0);

class Activator_FanBoatBMain : public Activator_FanBoatMain { // NAME: proto-pdb:default.pdb
public:
    Activator_FanBoatBMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Activator_FanBoatBMain, 0xE0);

class Activator_FanBoatCMain : public Activator_FanBoatMain { // NAME: proto-pdb:default.pdb
public:
    Activator_FanBoatCMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(Activator_FanBoatCMain, 0xE0);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1ActivatorMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_ActivatorMain_dtor(ActivatorMain* p)
{
    p->ActivatorMain::~ActivatorMain();
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
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) ++_Resource->_RefCount; }
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    ResourceHandle& operator=(const ResourceHandle&);
    bool IsValid() const { return _Resource != 0; }
    T& operator()() { return *_Resource; }
    T* _Resource;                      // 0x00
};

// Retail layout: retail disasm puts _defaultIdleAnim at 0x288 (proto PDB 0x278, Critter 0x2C8).
class Critter { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> GetDefaultIdleAnim() const;
    void SetDefaultIdleAnim(ResourceHandle<AnimationResource> ah);

    char field_0[0x288];               // 0x00
    ResourceHandle<AnimationResource> _defaultIdleAnim; // 0x288
};

// FUNCTION: USA 0x00019E1B
// PROTOTYPE: PREVIEW 0x0040907C near-identical
// NAME: proto-pdb:default.pdb
void Critter::SetDefaultIdleAnim(ResourceHandle<AnimationResource> ah)
{
    _defaultIdleAnim = ah;
}

// FUNCTION: USA 0x00019E39
// PROTOTYPE: PREVIEW 0x0040909A near-identical
// NAME: proto-pdb:default.pdb
ResourceHandle<AnimationResource> Critter::GetDefaultIdleAnim() const
{
    return _defaultIdleAnim;
}

class RegionTrigger; // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString MakeLower(const SwampString& s); // src proto-pdb:default.pdb MakeLower
}

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    typedef std::multimap<SwampString, RegionTrigger*> RegionMap; // NAME: proto-pdb:default.pdb
    RegionTrigger* Find(const SwampString& name);

    RegionMap _regions;                // 0x00
};
DECOMP_SIZE_ASSERT(RegionManager, 0xC);

// FUNCTION: USA 0x0001A04C
// NAME: proto-pdb:default.pdb
RegionTrigger* RegionManager::Find(const SwampString& name)
{
    RegionMap::const_iterator tmp = _regions.find(BeepLib::MakeLower(name));
    return tmp != _regions.end() ? tmp->second : 0;
}

class SoundResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    float GetPlaybackTime() const { return _playbackTime; }

    char field_24[0x38 - 0x24];        // 0x24
    float _playbackTime;               // 0x38
};
DECOMP_SIZE_ASSERT(SoundResource, 0x3C);

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void Play();
    ResourceHandle<SoundResource> GetSound() const { return _sound; }

    char field_A4[0xC4 - 0xA4];        // 0xA4
    ResourceHandle<SoundResource> _sound; // 0xC4
    char field_C8[0xD0 - 0xC8];        // 0xC8
};
DECOMP_SIZE_ASSERT(SoundNode, 0xD0);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static SoundNode* CreateSound(const SwampString&, unsigned int, SwampSceneNode*, bool);
};

class LocalizedStrings { // NAME: proto-pdb:default.pdb
public:
    static LocalizedStrings& Instance();
    const unsigned short* GetGameString(const SwampString&);
};

class HUDUI { // NAME: proto-pdb:default.pdb
public:
    static HUDUI& Instance() { return *_self; }
    void SetSubtitle(const unsigned short*, float);
private:
    static HUDUI* _self;
};

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
};

// FUNCTION: USA 0x0001A386
// PROTOTYPE: PREVIEW 0x00409603 identical
// NAME: proto-pdb:default.pdb
void CrawdadJimmyMain::GiveHint(int item)
{
    // !FAKE: c2 register allocation; stand-in local; the PDB records no local for the sound name (edi in retail)
    const char* v0 = 0;
    const char* camname = 0;
    switch (item) {
    case 0:
        v0 = "jimmy_b_cj_06";
        camname = "camera_hintOnion";
        break;
    case 1:
        v0 = "jimmy_b_cj_07";
        camname = "camera_hintsausage";
        break;
    case 2:
        v0 = "jimmy_b_cj_08";
        camname = "camera_hintcrawdad";
        break;
    }
    // !FAKE: c2 register allocation; stand-in local; the PDB records no local for the sound node (esi in retail)
    SoundNode* v1 = SoundManager::CreateSound(v0, 0x120, GetNode(), true);
    if (v1->IsValid()) {
        v1->Play();
        // !FAKE: c2 register allocation; stand-in local; the PDB records no local for the sound handle (esi in retail)
        ResourceHandle<SoundResource> v2 = v1->GetSound();
        if (v2.IsValid())
            HUDUI::Instance().SetSubtitle(LocalizedStrings::Instance().GetGameString(v0), v2().GetPlaybackTime());
    }
    AnimCam::SwitchTo(camname, 0.0f, false, 0.0f);
}

// FUNCTION: USA 0x0001A48C
// PROTOTYPE: PREVIEW 0x00409709 identical
// NAME: proto-pdb:default.pdb
SwampScript* Activator_FanBoatAMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 560) Activator_FanBoatAMain(node);
}

// FUNCTION: USA 0x0001A4B6
// PROTOTYPE: PREVIEW 0x00409733 identical
// NAME: proto-pdb:default.pdb
SwampScript* Activator_FanBoatBMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 569) Activator_FanBoatBMain(node);
}

// FUNCTION: USA 0x0001A4E0
// PROTOTYPE: PREVIEW 0x0040975D identical
// NAME: proto-pdb:default.pdb
SwampScript* Activator_FanBoatCMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 578) Activator_FanBoatCMain(node);
}

// FUNCTION: USA 0x0001A617
// PROTOTYPE: PREVIEW 0x004098A6 identical
// NAME: proto-pdb:default.pdb
SwampScript* CrawdadJimmyMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\bayou\\crawdadjimmys\\CrawdadJimmys.h", 14) CrawdadJimmyMain(node);
}

// FUNCTION: USA 0x0001A66B
// PROTOTYPE: PREVIEW 0x004098FA identical
// NAME: proto-pdb:default.pdb
class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationResource> _animOpen;   // 0xBC
    ResourceHandle<AnimationResource> _animClose;  // 0xC0
    std::vector<SwampString, std::allocator<SwampString> > _ownerSwitches; // 0xC4
    SwampString _animCam;              // 0xD4
    SwampString _animCamClose;         // 0xF0
    bool _locked;                      // 0x10C
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class GrateExitMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    GrateExitMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(GrateExitMain, 0x110);

class TimberGateMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    TimberGateMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(TimberGateMain, 0x110);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1BasicDoorMain@@UAE@XZ in this unit (no
// matched function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_BasicDoorMain_dtor(BasicDoorMain* p)
{
    p->BasicDoorMain::~BasicDoorMain();
}

// FUNCTION: USA 0x0001A77D
// PROTOTYPE: PREVIEW 0x00409A0C identical
// NAME: proto-pdb:default.pdb
SwampScript* GrateExitMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 535) GrateExitMain(node);
}

// FUNCTION: USA 0x0001A7A7
// PROTOTYPE: PREVIEW 0x00409A36 identical
// NAME: proto-pdb:default.pdb
SwampScript* TimberGateMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("CrawdadJimmys\\CrawdadJimmys.cpp", 521) TimberGateMain(node);
}

class Level { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() { return _script; }

    char field_0[0xC];                 // 0x00
    SwampScript* _script;              // 0x0C
    char field_10[0x140 - 0x10];       // 0x10
};
DECOMP_SIZE_ASSERT(Level, 0x140);

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }

    char field_0[0x4];                 // 0x00
    Level* _curLevel;                  // 0x04
    char field_8[0x1CC - 0x8];         // 0x08
};
DECOMP_SIZE_ASSERT(LevelLoader, 0x1CC);

// FUNCTION: USA 0x0001A939
// PROTOTYPE: PREVIEW 0x00409BC8 identical
// NAME: proto-pdb:default.pdb
bool CrawdadJimmyMain::TestActivate() const
{
    // !FAKE: c2 register allocation; pointer local; retail calls Instance() once and keeps the script in esi
    SwampScript* v0 = LevelLoader::Instance().GetCurLevel()->GetScript();
    bool racewon = false;
    v0->Message(0, 0x1F, &racewon);
    bool onion = false;
    v0->Message(0, 0x17, &onion);
    bool sausage = false;
    v0->Message(0, 0x19, &sausage);
    bool crawdad = false;
    v0->Message(0, 0x1B, &crawdad);
    bool pending = false;
    v0->Message(0, 0x1D, &pending);
    return !onion || !sausage || !crawdad || pending || racewon;
}
