// UNIT: src/Apps/Vince/core/Locator.cpp OBJ: core\XBox\OfficialRelease\Locator.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const E* _Ptr);
private:
    char _pad00[0x1C];
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
    void clear();

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map
    : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
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

void* operator new(unsigned int size, const char* file, int line);

class Locator; // NAME: proto-pdb:default.pdb

class LocatorMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SETLOCATOR = 0x14,
        MESSAGE_SETLOADNAME = 0x15,
        MESSAGE_SETINSTANCENAME = 0x16,
        MESSAGE_AREA_LOAD_ACTIVATE = 0x17,
        MESSAGE_LOCATOR_USERDEFINED = 0x18
    };
    virtual void Message(SwampScript* source, int message, void* data);

    Locator* _locator;                 // 0x4C
    SwampString _loadName;             // 0x50
    SwampString _instanceName;         // 0x6C
};
DECOMP_SIZE_ASSERT(LocatorMain, 0x88);

class AreaLoadLocatorMain : public LocatorMain { // NAME: proto-pdb:default.pdb
public:
    AreaLoadLocatorMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_88[0xFC - 0x88];        // 0x88
};
DECOMP_SIZE_ASSERT(AreaLoadLocatorMain, 0xFC);

class Locator { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x44];                // 0x00
    SwampScript* _script;              // 0x44
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    typedef std::map<SwampString, Locator*> Map;

    void RemoveAll();
    void NotifyAllLocators(SwampScript* from, unsigned int message, void* data) const;

    Map _locators;                     // 0x00
};
DECOMP_SIZE_ASSERT(LocatorManager, 0xC);

// FUNCTION: USA 0x0010A326
// NAME: proto-pdb:default.pdb
void LocatorManager::NotifyAllLocators(SwampScript* from, unsigned int message, void* data) const
{
    for (Map::const_iterator it = _locators.begin(); it != _locators.end(); ++it) {
        if (it->second && it->second->_script)
            it->second->_script->Message(from, message, data);
    }
}

// FUNCTION: USA 0x0010A845
// PROTOTYPE: PREVIEW 0x004F515B identical
// NAME: proto-pdb:default.pdb
void LocatorMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_SETLOCATOR:
        _locator = (Locator*)data;
        break;
    case MESSAGE_SETLOADNAME:
        _loadName = (const char*)data;
        break;
    case MESSAGE_SETINSTANCENAME:
        _instanceName = (const char*)data;
        break;
    }
}

// FUNCTION: USA 0x0010AB74
// PROTOTYPE: PREVIEW 0x004F548A identical
// NAME: proto-pdb:default.pdb
SwampScript* AreaLoadLocatorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Locator.h", 117) AreaLoadLocatorMain(node);
}

// FUNCTION: USA 0x0010AEBB
// NAME: proto-pdb:default.pdb
void LocatorManager::RemoveAll()
{
    for (Map::iterator lit = _locators.begin(); lit != _locators.end(); ++lit)
        delete lit->second;
    _locators.clear();
}
