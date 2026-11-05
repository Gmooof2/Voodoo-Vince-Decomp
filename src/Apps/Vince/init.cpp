// UNIT: src/Apps/Vince/init.cpp OBJ: Xbox\OfficialRelease\Init.obj OPT: /O1 /Oy
#include "../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as soundnode.cpp
#include <xstring>

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specializations (C0231 lever 1); the string rows call these out of line (named VAs
// 0x000112F5, 0x00011289, 0x00011779); _Copy's body would emit an __ehhandler$ funclet; declared after <xstring>,
// before <stdexcept> instantiates them (C2908 otherwise)
template <> void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Copy(unsigned int,
                                                                                             unsigned int);
template <> std::basic_string<char, std::char_traits<char>, std::allocator<char> >&
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::erase(unsigned int, unsigned int);
template <> std::basic_string<char, std::char_traits<char>, std::allocator<char> >&
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::assign(const char*);
#endif

#include <string>
#include <map>
#include <set>
#include <stack>
#include <list>

class SwampSceneNode; // NAME: proto-pdb:default.pdb

typedef std::string SwampString;

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

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb

    char field_4C[0x64 - 0x4C];        // 0x4C (not reached)
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

class VoodooShopLevelMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    VoodooShopLevelMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x98 - 0x64];        // 0x64 (not reached)
};
DECOMP_SIZE_ASSERT(VoodooShopLevelMain, 0x98);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    struct ScriptTimerEvent { // NAME: proto-pdb:default.pdb
        SwampScript* script;           // 0x00
        unsigned int id;               // 0x04
        bool looped;                   // 0x08
        float deltaTime;               // 0x0C
        float absTime;                 // 0x10
    };

    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    typedef std::map<std::string, CreationFunc> Map;       // NAME: proto-pdb:default.pdb

    ~SwampScriptManager();
    static SwampScriptManager& Instance();

private:
    SwampScriptManager();

    Map _scripts;                      // 0x00
    std::stack<SwampScript*> _deadScripts; // 0x0C
    std::multiset<ScriptTimerEvent> _timerEvents; // 0x20
    std::multiset<ScriptTimerEvent> _timerNewEvents; // 0x2C
    std::list<ScriptTimerEvent> _timerPausedEvents; // 0x38
    int _numRegisteredFrameTicks;      // 0x44
};
DECOMP_SIZE_ASSERT(SwampScriptManager, 0x48);
DECOMP_SIZE_ASSERT(SwampScriptManager::ScriptTimerEvent, 0x14);

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specializations (C0132, C0154); retail calls _Init, the list ctor and _Tidy out of
// line (named VAs); their header bodies would emit _Buynode/_Copy __ehhandler$ funclets here (gate objects extra,
// relink .text$x interleave refusal)
typedef std::_Tree<std::_Tmap_traits<std::string, SwampScriptManager::CreationFunc, std::less<std::string>,
                                     std::allocator<std::pair<const std::string, SwampScriptManager::CreationFunc> >,
                                     false> >
    ScriptTree;
typedef std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,
                                     std::less<SwampScriptManager::ScriptTimerEvent>,
                                     std::allocator<SwampScriptManager::ScriptTimerEvent>, true> >
    TimerTree;
template <> void ScriptTree::_Init();
template <> void TimerTree::_Init();
template <> std::list<SwampScriptManager::ScriptTimerEvent>::list();
template <> void ScriptTree::_Tidy();
template <> void TimerTree::_Tidy();
template <> void std::list<SwampScriptManager::ScriptTimerEvent>::_Tidy();
template <> void std::deque<SwampScript*>::_Tidy();
#endif

// !FAKE: explicit instantiation of the header static member (C0096); retail emits it through callers in this unit
// (not matched yet)
// FUNCTION: USA 0x00011000
// NAME: proto-pdb:default.pdb
template char* std::char_traits<char>::copy(char*, const char*, size_t); // needs unit opt /Oi (C0231)

// FUNCTION: USA 0x00011045
// PROTOTYPE: PREVIEW 0x00400445 identical
// NAME: proto-pdb:default.pdb
SwampScript* VoodooShopLevelMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts/voodooShop/voodooShop.h", 22) VoodooShopLevelMain(node);
}

// FUNCTION: USA 0x00011144
// PROTOTYPE: PREVIEW 0x00400544 identical
// NAME: proto-pdb:default.pdb
template bool std::basic_string<char,std::char_traits<char>,std::allocator<char> >::_Inside(char const *);

// FUNCTION: USA 0x000111BA
// PROTOTYPE: PREVIEW 0x004005F0 identical
// NAME: proto-pdb:default.pdb
template void std::basic_string<char,std::char_traits<char>,std::allocator<char> >::_Eos(unsigned int);

// FUNCTION: USA 0x000111D6
// PROTOTYPE: PREVIEW 0x0040060C identical
// NAME: proto-pdb:default.pdb
template void std::basic_string<char,std::char_traits<char>,std::allocator<char> >::_Tidy(bool,unsigned int);

// FUNCTION: USA 0x0001152D
// PROTOTYPE: PREVIEW 0x0040099A identical
// NAME: proto-pdb:default.pdb
template bool std::basic_string<char,std::char_traits<char>,std::allocator<char> >::_Grow(unsigned int,bool);

// FUNCTION: USA 0x00011655
// PROTOTYPE: PREVIEW 0x00400AC2 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> > & std::basic_string<char,std::char_traits<char>,std::allocator<char> >::assign(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,unsigned int,unsigned int);

// FUNCTION: USA 0x000116E4
// PROTOTYPE: PREVIEW 0x00400B51 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> > & std::basic_string<char,std::char_traits<char>,std::allocator<char> >::assign(char const *,unsigned int);

// FUNCTION: USA 0x00011753
// PROTOTYPE: PREVIEW 0x00400BC0 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string<char,std::char_traits<char>,std::allocator<char> >(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &);

// FUNCTION: USA 0x000117DE
// PROTOTYPE: PREVIEW 0x00400C4B identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string<char,std::char_traits<char>,std::allocator<char> >(char const *);

// FUNCTION: USA 0x00011178
// PROTOTYPE: PREVIEW 0x00400578 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Lrotate(std::_Tree_nod<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Node *);

// FUNCTION: USA 0x000114FB
// PROTOTYPE: PREVIEW 0x0040095E identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Erase(std::_Tree_nod<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Node *);

// FUNCTION: USA 0x00011800
// PROTOTYPE: PREVIEW 0x00400C6D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // best: match_one 100, bytes match (masked 0) (4.4 T4.c5; throw-info now pinned); wall: relink refuses `.rdata 0x001FCF30..0x001FD8A8: retail bytes between two of our .xdata$x sections (one LINK group cannot interleave with them)` (the out_of_range throw-info in .xdata$x); with HUDUI _Incsize live, ??_7logic_error COMDAT overlap 0x001C77E8
template std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::iterator std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::erase(std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::iterator);
#endif

// FUNCTION: USA 0x00011B38
// PROTOTYPE: PREVIEW 0x004A925C identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // best: match_one 100, bytes match (masked 0) (4.4 T4.c5); wall: as 0x00011800 (.xdata$x interleave; logic_error data overlap), and CameraManager calls the GameCam alias of this fold VA (C0233)
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::iterator std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::erase(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::iterator);
#endif

// !FAKE: logic_error is no template; its inline members are emitted through a use: the real caller chain
// deque<SwampScript*>::_Xlen (pinned 0x0001D5C2) -> length_error ctor -> logic_error ctor, vtable (dtor, what)
// FUNCTION: USA 0x000115FB
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x00011618
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0001179C
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // best: match_one 100, bytes match (masked 0) all 3 (4.4 T4.c5); throw-info and vtable data pinned (4.4 T4); wall: relink refuses Init.obj .rdata: the _Xlen message string literal has no pin (0 VAs from its symbols by name; retail 0x001C94D4, the push in _Xlen 0x0001D5C2)
template void std::deque<SwampScript*>::_Xlen() const;
#endif

// FUNCTION: USA 0x00011A7B
// PROTOTYPE: PREVIEW 0x00400EE8 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // wall as the block above: the implicit logic_error copy ctor comes only from _Xlen's throw-info
// best: match_one objdiff 100, bytes match (masked 0) with the _Xlen instantiation enabled (4.4 T4.c5); repeated here
// as this row's emitter (cl 13.10 accepts the repeat silently)
template void std::deque<SwampScript*>::_Xlen() const;
#endif

// FUNCTION: USA 0x00011E90
// PROTOTYPE: PREVIEW 0x00401084 identical
// NAME: proto-pdb:default.pdb
SwampScriptManager::~SwampScriptManager()
{
}

// FUNCTION: USA 0x00011EBB
// PROTOTYPE: PREVIEW 0x004010AF identical
// NAME: proto-pdb:default.pdb
SwampScriptManager::SwampScriptManager() : _numRegisteredFrameTicks(0)
{
}
