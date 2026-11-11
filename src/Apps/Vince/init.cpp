// UNIT: src/Apps/Vince/init.cpp OBJ: Xbox\OfficialRelease\Init.obj OPT: /O1 /Oy
#include "../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as soundnode.cpp
#include <xstring>

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0231 lever 1); the string rows call these out of line (named VAs
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
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x98 - 0x64];        // 0x64 (not reached)
};
DECOMP_SIZE_ASSERT(VoodooShopLevelMain, 0x98);

class FrenchquarterLevelMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    FrenchquarterLevelMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x68 - 0x64];        // 0x64 (not reached)
};
DECOMP_SIZE_ASSERT(FrenchquarterLevelMain, 0x68);

class CryptCityLevelMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    CryptCityLevelMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptCityLevelMain, 0x64);

class BayouLevelMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    BayouLevelMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x6C - 0x64];        // 0x64 (not reached)
};
DECOMP_SIZE_ASSERT(BayouLevelMain, 0x6C);

class SewersLevelMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    SewersLevelMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x6C - 0x64];        // 0x64 (not reached)
};
DECOMP_SIZE_ASSERT(SewersLevelMain, 0x6C);

class CarnivalLevelMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    CarnivalLevelMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x68 - 0x64];        // 0x64 (not reached)
};
DECOMP_SIZE_ASSERT(CarnivalLevelMain, 0x68);

class ManorHouseLevelMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    ManorHouseLevelMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_64[0x6C - 0x64];        // 0x64 (not reached)
};
DECOMP_SIZE_ASSERT(ManorHouseLevelMain, 0x6C);

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
    void Add(const SwampString& name, CreationFunc func);

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

typedef std::_Tree<std::_Tmap_traits<std::string, SwampScriptManager::CreationFunc, std::less<std::string>,
                                     std::allocator<std::pair<const std::string, SwampScriptManager::CreationFunc> >,
                                     false> >
    ScriptTree;
typedef std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,
                                     std::less<SwampScriptManager::ScriptTimerEvent>,
                                     std::allocator<SwampScriptManager::ScriptTimerEvent>, true> >
    TimerTree;
#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0154); retail calls _Init, the list ctor and _Tidy out of
// line (named VAs); their header bodies would emit _Buynode/_Copy __ehhandler$ funclets here (gate objects extra,
// relink .text$x interleave refusal)
template <> void ScriptTree::_Init();
template <> void TimerTree::_Init();
template <> std::list<SwampScriptManager::ScriptTimerEvent>::list();
template <> void ScriptTree::_Tidy();
template <> void TimerTree::_Tidy();
template <> void std::list<SwampScriptManager::ScriptTimerEvent>::_Tidy();
template <> void std::deque<SwampScript*>::_Tidy();
#endif

// !FAKE: c1xx front end; explicit instantiation of the header static member (C0096); retail emits it through callers in this unit
// (not matched yet)
// FUNCTION: USA 0x00011000
// NAME: proto-pdb:default.pdb
template char* std::char_traits<char>::copy(char*, const char*, size_t); // needs unit opt /Oi (C0231)

namespace std {
// FUNCTION: USA 0x00011023
// PROTOTYPE: PREVIEW 0x00400423 identical
// NAME: proto-pdb:default.pdb
// !FAKE: provenance; repeated forward declaration; the class whose array below makes cl emit the row, as a class
// declaration carries a compiler-generated row (CONVENTIONS §1)
template <class T> class allocator;
}

// !FAKE: c1xx front end; scaffold local array; retail Init.obj defines the compiler-generated vector constructor iterator
// ??_H@YGXPAXIHP6EPAX0@Z@Z with no caller left in the unit; this array makes cl emit it (matched_set [[emitter]] row)
void pin_emit_vector_ctor_iterator()
{
    std::allocator<char> v[2];
}

// FUNCTION: USA 0x00011045
// PROTOTYPE: PREVIEW 0x00400445 identical
// NAME: proto-pdb:default.pdb
SwampScript* VoodooShopLevelMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts/voodooShop/voodooShop.h", 22) VoodooShopLevelMain(node);
}

// FUNCTION: USA 0x0001106C
// NAME: proto-pdb:default.pdb
// matched by the 5.4 T6 re-cut (pin route); re-verified by match_one in 5.4 T8
SwampScript* FrenchquarterLevelMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts/french_Quarter/frenchQuarter.h", 13) FrenchquarterLevelMain(node);
}

// FUNCTION: USA 0x00011090
// NAME: proto-pdb:default.pdb
SwampScript* CryptCityLevelMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts/Crypt_City/CryptCity.h", 13) CryptCityLevelMain(node);
}

// FUNCTION: USA 0x000110B4
// NAME: proto-pdb:default.pdb
SwampScript* BayouLevelMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts/Bayou/Bayou.h", 13) BayouLevelMain(node);
}

// FUNCTION: USA 0x000110D8
// NAME: proto-pdb:default.pdb
SwampScript* SewersLevelMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts/Sewers/Sewers.h", 13) SewersLevelMain(node);
}

// FUNCTION: USA 0x000110FC
// NAME: proto-pdb:default.pdb
SwampScript* CarnivalLevelMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts/Carnival/Carnival.h", 13) CarnivalLevelMain(node);
}

// FUNCTION: USA 0x00011120
// NAME: proto-pdb:default.pdb
SwampScript* ManorHouseLevelMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts/ManorHouse/ManorHouse.h", 13) ManorHouseLevelMain(node);
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

// FUNCTION: USA 0x0001121F
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): only masked target is its own __ehhandler$ funclet (retail 0x00152ACE), unnamed; needs the funclet pinned (C0331), not a source lever
template std::list<SwampScriptManager::ScriptTimerEvent>::_Nodeptr std::list<SwampScriptManager::ScriptTimerEvent>::_Buynode();
#endif

// FUNCTION: USA 0x000113EB
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): only masked target is its own __ehhandler$ funclet (retail 0x00152AE2), unnamed; needs the funclet pinned (C0331), not a source lever
template std::_Tree<std::_Tmap_traits<std::string, SwampScriptManager::CreationFunc, std::less<std::string>, std::allocator<std::pair<const std::string, SwampScriptManager::CreationFunc> >, false> >::_Nodeptr std::_Tree<std::_Tmap_traits<std::string, SwampScriptManager::CreationFunc, std::less<std::string>, std::allocator<std::pair<const std::string, SwampScriptManager::CreationFunc> >, false> >::_Buynode();
#endif

// FUNCTION: USA 0x00011473
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): only masked target is its own __ehhandler$ funclet (retail 0x00152AEC), unnamed; needs the funclet pinned (C0331), not a source lever
template std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent, std::less<SwampScriptManager::ScriptTimerEvent>, std::allocator<SwampScriptManager::ScriptTimerEvent>, true> >::_Nodeptr std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent, std::less<SwampScriptManager::ScriptTimerEvent>, std::allocator<SwampScriptManager::ScriptTimerEvent>, true> >::_Buynode();
#endif

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

// logic_error's inline members (0x000115FB, 0x00011618 tagged here; 0x0001179C, 0x00011A7B below, split so each NAME
// tag finds code within the lint's window) are emitted through a use: erase's out_of_range throw (its ctor calls the
// logic_error ctor; vtable: dtor, what; throw-info: copy ctor); retail Init.obj has no length_error (C0302)
// FUNCTION: USA 0x000114FB
// PROTOTYPE: PREVIEW 0x0040095E identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x000115FB
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x00011618
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Erase(std::_Tree_nod<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Node *);

// FUNCTION: USA 0x0001179C
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x00011800
// PROTOTYPE: PREVIEW 0x00400C6D identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::iterator std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::erase(std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::iterator);

// FUNCTION: USA 0x00011A7B
// PROTOTYPE: PREVIEW 0x00400EE8 identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x00011B38
// PROTOTYPE: PREVIEW 0x004A925C identical
// NAME: proto-pdb:default.pdb
// best: match_one 100, bytes match (masked 0) (4.4 T4.c5); was walled (5.1 T5.c2): relink refuses `our cameramanager.obj: external ?erase@...GameCam... 0x00011B38 lies in a replaced range`: CameraManager calls the GameCam alias of this fold VA (C0233; lift: CameraManager emits its alias body) -- lifted 5.2 T13.c1: callers emit their alias inline (C0233, C0270)
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::iterator std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::erase(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,void *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,void *> >,0> >::iterator);

// FUNCTION: USA 0x00011DEC
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): callee ScriptTree::_Erase at retail 0x00011DB1 has no symbol; needs that pin (also emits _Erase here, C0132)
template void std::_Tree<std::_Tmap_traits<std::string, SwampScriptManager::CreationFunc, std::less<std::string>, std::allocator<std::pair<const std::string, SwampScriptManager::CreationFunc> >, false> >::clear();
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

// FUNCTION: USA 0x00011F1D
// NAME: proto-pdb:default.pdb
VoodooShopLevelMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("VoodooShopLevelMain", creationFunc);
}

// FUNCTION: USA 0x00011F59
// NAME: proto-pdb:default.pdb
FrenchquarterLevelMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("FrenchquarterLevelMain", creationFunc);
}

// FUNCTION: USA 0x00011F95
// NAME: proto-pdb:default.pdb
CryptCityLevelMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("CryptCityLevelMain", creationFunc);
}

// FUNCTION: USA 0x00011FD1
// NAME: proto-pdb:default.pdb
BayouLevelMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BayouLevelMain", creationFunc);
}

// FUNCTION: USA 0x0001200D
// NAME: proto-pdb:default.pdb
SewersLevelMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("SewersLevelMain", creationFunc);
}

// FUNCTION: USA 0x00012049
// NAME: proto-pdb:default.pdb
CarnivalLevelMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("CarnivalLevelMain", creationFunc);
}

// FUNCTION: USA 0x00012085
// NAME: proto-pdb:default.pdb
ManorHouseLevelMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("ManorHouseLevelMain", creationFunc);
}
