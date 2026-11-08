// UNIT: src/Apps/Vince/core/Locator.cpp OBJ: core\XBox\OfficialRelease\Locator.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& operator+=(const E* _Ptr) { return append(_Ptr); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, (unsigned int)-1); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& append(const E* _Ptr);
    bool empty() const { return _Mysize == 0; }
    unsigned int find(const basic_string& _Right, unsigned int _Off = 0) const;
    const E* c_str() const { return _Myptr(); }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    char _Alval;                       // 0x00
    union {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
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
template <class T> struct less {
    bool operator()(const T& _Left, const T& _Right) const { return (_Left < _Right); }
};
template <class T1, class T2> struct pair {
    // retail calls the converting ctor out of line (fold 0x000EC70C, named): declared only
    template <class U1, class U2> pair(const pair<U1, U2>& _Right);
    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
    static const K& _Kfn(const value_type& _Val) { return (_Val.first); }
    Pr comp;                           // 0x00
};
template <class Tr> class _Tree_nod {
public:
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
        char _Color;
        char _Isnil;
    };
};
template <class Tr> class _Tree : public Tr {
    typedef typename _Tree_nod<Tr>::_Node _Node;
    typedef _Node* _Nodeptr;
    typedef typename Tr::key_type key_type;

    static char& _Isnil(_Nodeptr _Pnode) { return ((char&)(*_Pnode)._Isnil); }
    static const key_type& _Key(_Nodeptr _Pnode) { return (Tr::_Kfn(_Pnode->_Myval)); }
    static _Nodeptr& _Left(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Left); }
    static _Nodeptr& _Parent(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Parent); }
    static _Nodeptr& _Right(_Nodeptr _Pnode) { return ((_Nodeptr&)(*_Pnode)._Right); }
    static typename Tr::value_type& _Myval(_Nodeptr _Pnode) { return ((typename Tr::value_type&)(*_Pnode)._Myval); }
    static _Nodeptr _Min(_Nodeptr _Pnode)
    {
        while (!_Isnil(_Left(_Pnode)))
            _Pnode = _Left(_Pnode);
        return (_Pnode);
    }
    _Nodeptr& _Root() const { return (_Parent(_Myhead)); }

protected:
    // retail calls _Init (0x0010A36A) and _Tidy (0x0010AF92) out of line: declared only
    void _Init();
    void _Tidy();
    _Nodeptr _Lbound(const key_type& _Keyval) const
    {
        _Nodeptr _Pnode = _Root();
        _Nodeptr _Wherenode = _Myhead;

        while (!_Isnil(_Pnode))
            if (this->comp(_Key(_Pnode), _Keyval))
                _Pnode = _Right(_Pnode);
            else {
                _Wherenode = _Pnode;
                _Pnode = _Left(_Pnode);
            }

        return (_Wherenode);
    }

public:
    class const_iterator {
    public:
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const typename Tr::value_type& operator*() const { return (_Myval(_Ptr)); }
        const typename Tr::value_type* operator->() const { return &_Ptr->_Myval; }
        const_iterator& operator++() { _Inc(); return *this; }
        bool operator==(const const_iterator& _Right) const { return (_Ptr == _Right._Ptr); }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
        void _Inc()
        {
            if (_Isnil(_Ptr))
                ;
            else if (!_Isnil(_Right(_Ptr)))
                _Ptr = _Min(_Right(_Ptr));
            else {
                _Nodeptr _Pnode;
                while (!_Isnil(_Pnode = _Parent(_Ptr)) && _Ptr == _Right(_Pnode))
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
    _Tree() { _Init(); }
    ~_Tree() { _Tidy(); }
    // retail calls find out of line (fold 0x001090EB, named): declared only
    const_iterator find(const key_type& _Keyval) const;
    void clear();

private:
    char _Alval;                       // 0x01 (comp at 0x00)
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map
    : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
template <class _InIt, class _Pr> inline _InIt find_if(_InIt _First, _InIt _Last, _Pr _Pred)
{
    for (; _First != _Last; ++_First)
        if (_Pred(*_First))
            break;
    return (_First);
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString MakeLower(const SwampString& s);
}

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

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_LEVELINTRO_FINISHED = 0xF
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    void NotifyAllCritters(SwampScript* from, unsigned int message, void* data);
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class MusicManager { // NAME: proto-pdb:default.pdb
public:
    void PlayTrack(const SwampString& name);
};
extern MusicManager* g_MusicManager; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    const SwampString& GetMiscPostBossMusic() const { return _miscPostBossMusic; }

    char field_0[0x1AC];               // 0x00
    SwampString _miscPostBossMusic;    // 0x1AC
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

class Locator; // NAME: proto-pdb:default.pdb
class VertexShaderResource; // NAME: proto-pdb:default.pdb

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
    const SwampString& GetName() const { return _instanceName; }

    Locator* _locator;                 // 0x4C
    SwampString _loadName;             // 0x50
    SwampString _instanceName;         // 0x6C
};
DECOMP_SIZE_ASSERT(LocatorMain, 0x88);

class AreaLoadLocatorMain : public LocatorMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SETMUSICNAME = 0x18,
        MESSAGE_QUERYFLYTHROUGH = 0x19,
        MESSAGE_SETPOSTBOSSMUSIC = 0x1A,
        MESSAGE_SKIP_INTRO = 0x1B,
        MESSAGE_AREALOCATOR_USERDEFINED = 0x1C
    };
    AreaLoadLocatorMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    static void FinishedLevelIntro(SwampScript* source);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    bool _didFlyThrough;               // 0x88
    bool _chapterStart;                // 0x89
    bool _delayMusic;                  // 0x8A
    SwampString _musicName;            // 0x8C
    SwampString _cutsceneName;         // 0xA8
    SwampString _hintName;             // 0xC4
    SwampString _vinceWalkInName;      // 0xE0

    static SwampString _delayMusicName;
};
DECOMP_SIZE_ASSERT(AreaLoadLocatorMain, 0xFC);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    SwampScript* Create(const SwampString&, SwampSceneNode*) const;
    void Add(const SwampString& name, CreationFunc func);
};

class Locator { // NAME: proto-pdb:default.pdb
public:
    void SetScript(const SwampString& scriptname);
    void Save(SwampString& str);
    SwampScript* GetScript() const { return _script; }

    char field_0[0x44];                // 0x00
    SwampScript* _script;              // 0x44
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    typedef std::map<SwampString, Locator*> Map;

    LocatorManager();
    ~LocatorManager();
    void RemoveAll();
    Locator* Find(const SwampString& s) const;
    Locator* Find(bool (*match)(const std::pair<SwampString, Locator*>&)) const;
    void SaveLocators(SwampString& str);
    void Load(const BeepTDF& summary);
    void NotifyAllLocators(SwampScript* from, unsigned int message, void* data) const;
    void NotifyMatchingLocators(const char* match, SwampScript* from, unsigned int message, void* data) const;

    Map _locators;                     // 0x00
};
DECOMP_SIZE_ASSERT(LocatorManager, 0xC);
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0010A326
// NAME: proto-pdb:default.pdb
void LocatorManager::NotifyAllLocators(SwampScript* from, unsigned int message, void* data) const
{
    for (Map::const_iterator it = _locators.begin(); it != _locators.end(); ++it) {
        if (it->second && it->second->_script)
            it->second->_script->Message(from, message, data);
    }
}

// FUNCTION: USA 0x0010A65B
// PROTOTYPE: PREVIEW 0x004F4F71 identical
// NAME: proto-pdb:default.pdb
void Locator::Save(SwampString& str)
{
    // !FAKE: c2 register allocation; neutral stand-in local; retail loads _script once into esi (match_one 88.9 without it)
    LocatorMain* p = (LocatorMain*)_script;
    if (p && !p->GetName().empty()) {
        str += format("\t[%s]\n\t{\n", p->GetName().c_str());
        str += p->Save();
        str += format("\t}\n");
    }
}

// FUNCTION: USA 0x0010A705
// PROTOTYPE: PREVIEW 0x004F501B identical
// NAME: proto-pdb:default.pdb
void LocatorManager::SaveLocators(SwampString& str)
{
    str += "[Locators]\n{\n";
    for (Map::iterator i = _locators.begin(); i != _locators.end(); ++i) {
        if (i->second->GetScript())
            i->second->Save(str);
    }
    str += "}\n";
}

// FUNCTION: USA 0x0010A75A
// PROTOTYPE: PREVIEW 0x004F5070 identical
// NAME: proto-pdb:default.pdb
Locator* LocatorManager::Find(const SwampString& s) const
{
    SwampString lookup = BeepLib::MakeLower(s);
    Map::const_iterator it = _locators.find(BeepLib::MakeLower(lookup));
    return it != _locators.end() ? it->second : 0;
}

// FUNCTION: USA 0x0010A7B9
// PROTOTYPE: PREVIEW 0x004F50CF identical
// NAME: proto-pdb:default.pdb
void LocatorManager::NotifyMatchingLocators(const char* match, SwampScript* from, unsigned int message, void* data) const
{
    SwampString lookup = BeepLib::MakeLower(match);
    for (Map::const_iterator it = _locators.begin(); it != _locators.end(); ++it) {
        // !FAKE: c2 register allocation; neutral stand-in local; retail holds it->second in esi across find (no PDB local)
        Locator* v1 = it->second;
        if (v1 && it->first.find(lookup) == 0 && v1->GetScript())
            v1->GetScript()->Message(from, message, data);
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

// FUNCTION: USA 0x0010AA05
// PROTOTYPE: PREVIEW 0x004F531B identical
// NAME: proto-pdb:default.pdb
void AreaLoadLocatorMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_AREA_LOAD_ACTIVATE:
        _didFlyThrough = true;
        break;
    case MESSAGE_SETMUSICNAME:
        _musicName = (const char*)data;
        break;
    case MESSAGE_QUERYFLYTHROUGH:
        *(bool*)data = _didFlyThrough;
        break;
    case MESSAGE_SETPOSTBOSSMUSIC:
        if (!_musicName.empty())
            _musicName = gSettings.GetMiscPostBossMusic();
        break;
    case MESSAGE_SKIP_INTRO:
        _didFlyThrough = true;
        break;
    default:
        LocatorMain::Message(source, message, data);
    }
}

// FUNCTION: USA 0x0010AA69
// PROTOTYPE: PREVIEW 0x004F537F identical
// NAME: proto-pdb:default.pdb
void AreaLoadLocatorMain::Load(const BeepTDF& tdf)
{
    SwampScript::Load(tdf);
    if (_cutsceneName.empty())
        _cutsceneName = tdf.GetStringRecord("cutscene", "");
}

// FUNCTION: USA 0x0010AAE9
// PROTOTYPE: PREVIEW 0x004F53FF identical
// NAME: proto-pdb:default.pdb
void AreaLoadLocatorMain::FinishedLevelIntro(SwampScript* source)
{
    if (!_delayMusicName.empty()) {
        g_MusicManager->PlayTrack(_delayMusicName);
        _delayMusicName = "";
    }
    pManager->NotifyAllCritters(source, MESSAGE_LEVELINTRO_FINISHED, 0);
}

// FUNCTION: USA 0x0010AB25
// PROTOTYPE: PREVIEW 0x004F543B identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,Locator *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,Locator *> >,0> >::const_iterator std::find_if(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,Locator *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,Locator *> >,0> >::const_iterator,std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,Locator *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,Locator *> >,0> >::const_iterator,bool (__cdecl*)(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,Locator *> const &));

// FUNCTION: USA 0x0010AB74
// PROTOTYPE: PREVIEW 0x004F548A identical
// NAME: proto-pdb:default.pdb
SwampScript* AreaLoadLocatorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Locator.h", 117) AreaLoadLocatorMain(node);
}

// FUNCTION: USA 0x0010AB9B
// PROTOTYPE: PREVIEW 0x004F54B1 identical
// NAME: proto-pdb:default.pdb
void LocatorManager::Load(const BeepTDF& summary)
{
    // !FAKE: c1xx front end; neutral stand-in local; the block pointer has no PDB local name
    const BeepTDF* p = summary.FindBlock("Locators", false);
    if (p) {
        // !FAKE: c1xx front end; neutral stand-in local; the iterator has no PDB local name
        for (std::vector<BeepTDF*>::const_iterator it = p->GetBlocks().begin(); it != p->GetBlocks().end(); ++it) {
            SwampString instancename = (*it)->GetBlockName();
            // !FAKE: c1xx front end; neutral stand-in local; the found locator has no PDB local name
            Locator* v1 = Find(instancename);
            if (v1 && v1->GetScript())
                v1->GetScript()->Load(**it);
        }
    }
}

// FUNCTION: USA 0x0010AC1A
// PROTOTYPE: PREVIEW 0x004F5530 identical
// NAME: proto-pdb:default.pdb
Locator* LocatorManager::Find(bool (*match)(const std::pair<SwampString, Locator*>&)) const
{
    Map::const_iterator it = std::find_if(_locators.begin(), _locators.end(), match);
    return it != _locators.end() ? it->second : 0;
}

// FUNCTION: USA 0x0010AEBB
// NAME: proto-pdb:default.pdb
void LocatorManager::RemoveAll()
{
    for (Map::iterator lit = _locators.begin(); lit != _locators.end(); ++lit)
        delete lit->second;
    _locators.clear();
}

// FUNCTION: USA 0x0010B016
// NAME: proto-pdb:default.pdb
LocatorManager::~LocatorManager()
{
    RemoveAll();
    g_locatorManager = 0;
}

// FUNCTION: USA 0x0010B02D
// NAME: proto-pdb:default.pdb
LocatorManager::LocatorManager()
{
    g_locatorManager = this;
}

// FUNCTION: USA 0x0010B03F
// NAME: proto-pdb:default.pdb
AreaLoadLocatorMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("AreaLoadLocatorMain", creationFunc);
}

// FUNCTION: USA 0x0010B07B
// PROTOTYPE: PREVIEW 0x004F5991 identical
// NAME: proto-pdb:default.pdb
void Locator::SetScript(const SwampString& scriptname)
{
    _script = SwampScriptManager::Instance().Create(scriptname, 0);
    _script->Message(0, LocatorMain::MESSAGE_SETLOCATOR, this);
}

// fold VA 0x0010A38F (21 aliases): the delinked retail Locator.obj names the row by this alias (C0234)
// FUNCTION: USA 0x0010A38F
// PROTOTYPE: PREVIEW 0x004CDFB9 identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,VertexShaderResource *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,VertexShaderResource *> >,1> >::_Node * std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,VertexShaderResource *,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,VertexShaderResource *> >,1> >::_Lbound(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &)const;
