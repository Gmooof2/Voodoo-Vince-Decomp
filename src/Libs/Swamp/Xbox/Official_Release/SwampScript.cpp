// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampScript.cpp OBJ: Xbox\Official_Release\SwampScript.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class T, class A = allocator<T> > class list {
    struct _Node {
        _Node* _Next;                  // 0x00
        _Node* _Prev;                  // 0x04
        T _Myval;                      // 0x08
    };
public:
    class iterator {
    public:
        iterator() {}
        iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        T& operator*() const { return _Ptr->_Myval; }
        T* operator->() const { return &**this; }
        iterator& operator++() { _Ptr = _Ptr->_Next; return *this; }
        bool operator==(const iterator& _Right) const { return _Ptr == _Right._Ptr; }
        bool operator!=(const iterator& _Right) const { return !(*this == _Right); }

        _Node* _Ptr;
    };
    iterator begin() { return iterator(_Myhead->_Next); }
    iterator end() { return iterator(_Myhead); }
    void push_back(const T& _Val) { _Insert(end(), _Val); }
    void _Insert(iterator _Where, const T& _Val);
    iterator erase(iterator _Where);
    void clear();

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class T> struct less;
template <class T1, class T2> struct pair {
    pair() : first(T1()), second(T2()) {}
    pair(const T1& _Val1, const T2& _Val2) : first(_Val1), second(_Val2) {}
    T1 first;
    T2 second;
};
template <class K, class Pr, class A, bool Mfl> class _Tset_traits {
public:
    typedef K value_type;
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
        const_iterator() {}
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const_iterator& operator++() { _Inc(); return *this; }
        bool operator==(const const_iterator& _Right) const { return _Ptr == _Right._Ptr; }
        bool operator!=(const const_iterator& _Right) const { return !(*this == _Right); }
        void _Inc();

        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        iterator() {}
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
        typename Tr::value_type& operator*() const { return this->_Ptr->_Myval; }
        typename Tr::value_type* operator->() const { return &**this; }
        iterator& operator++() { ++(*(const_iterator*)this); return *this; }
    };
    iterator begin() { return iterator(_Myhead->_Left); }
    iterator end() { return iterator(_Myhead); }
    pair<iterator, bool> insert(const typename Tr::value_type& _Val);

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class Pr = less<K>, class A = allocator<K> > class multiset // src proto-pdb:default.pdb multiset
    : public _Tree<_Tset_traits<K, Pr, A, true> > {
public:
    typedef _Tree<_Tset_traits<K, Pr, A, true> > _Mybase;
    typename _Mybase::iterator insert(const K& _Val) { return _Mybase::insert(_Val).first; }
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    T& operator[](const K& _Keyval);

private:
    char _Alval;                       // 0x00
    void* _Myhead;                     // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class T, class A = allocator<T> > class deque {
public:
    void push_back(const T& _Val);
    const T& back() const;
    bool empty() const { return _Mysize == 0; }
    void pop_back()
    {
        if (!empty()) {
            if (--_Mysize == 0)
                _Myoff = 0;
        }
    }

private:
    char _Alval;                       // 0x00
    T** _Map;                          // 0x04
    unsigned int _Mapsize;             // 0x08
    unsigned int _Myoff;               // 0x0C
    unsigned int _Mysize;              // 0x10
};
template <class T, class C = deque<T> > class stack {
public:
    void push(const T& _Val) { c.push_back(_Val); }
    bool empty() const { return c.empty(); }
    const T& top() const { return c.back(); }
    void pop() { c.pop_back(); }

protected:
    C c;                               // 0x00
};
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampScript;

namespace BeepLib { // NAME: proto-pdb:default.pdb
std::basic_string<char, std::char_traits<char>, std::allocator<char> > MakeLower(
    const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& s);
}

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetGameTime() const { return _gameTimePublic; }

    char field_0[0x8];                 // 0x00
    float _gameTimePublic;             // 0x08
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;
    bool PopScript();
};

void* operator new(unsigned int size, const char* file, int line);
inline void* operator new(unsigned int, void* p) { return p; }

class SwampScript { // NAME: proto-pdb:default.pdb
    // !FAKE: access only; GetGenericScript constructs through the protected ctor (friendship not in the PDB TPI)
    friend class SwampScriptManager;
public:
    struct QueuedMessage;              // NAME: proto-pdb:default.pdb
    class SaveLoadVar;                 // NAME: proto-pdb:default.pdb

    enum CollisionType { // NAME: proto-pdb:default.pdb
        HorizontalCollision = 0,
        VerticalCollision = 1
    };
    enum { // NAME: proto-pdb:default.pdb
        ANIMATION_FINISHED = 0,
        ANIMATION2_FINISHED = 1,
        ANIMATION_INTERRUPTED = 2,
        ANIMATION2_INTERRUPTED = 3,
        ANIMATION_TRIGGER = 4,
        SOUND_FINISHED = 5,
        SOUND_STREAMER_FINISHED = 6,
        MOVIE_FINISHED = 7,
        TOTAL_MESSAGES = 8
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
    void QueueAnimationMessage(unsigned int msg);
    void QueueSoundFinishedMessage(const SwampString& name);
    void QueueSoundStreamerFinishedMessage(const SwampString& name);
    void QueueMovieFinishedMessage(const SwampString& name);
    void RemoveMatchingQueuedMessages(unsigned int msg);
    void RegisterTimerEvent(unsigned int id, float deltaTime, bool looped);
    void ClearQueuedMessages();
    void UnregisterTimerEvent(unsigned int id);
    void RemoveTimerEvents();
    void RegisterFrameTick();
    void UnRegisterFrameTick();

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    SwampScript* _parent;              // 0x24
    int _frameTicks;                   // 0x28
    bool _valid;                       // 0x2C
    bool _sleeping;                    // 0x2D
    std::list<QueuedMessage> _pendingMessages; // 0x30
    char field_3C[0x4C - 0x3C];        // 0x3C
protected:
    SwampScript(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

// FUNCTION: USA 0x000ABA2B
// PROTOTYPE: PREVIEW 0x0049685D identical
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x000ABF95
// PROTOTYPE: PREVIEW 0x00496DBC identical
// NAME: proto-pdb:default.pdb
class SwampScript::SaveLoadVar { // NAME: proto-pdb:default.pdb
public:
    enum Type { // NAME: proto-pdb:default.pdb
        TYPE_INT = 0,
        TYPE_FLOAT = 1,
        TYPE_STRING = 2,
        TYPE_VECTOR3 = 3,
        TYPE_BOOL = 4
    };
    SwampString _name;                 // 0x00
    Type _type;                        // 0x1C
    void* _addr;                       // 0x20
};
DECOMP_SIZE_ASSERT(SwampScript::SaveLoadVar, 0x24);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit SaveLoadVar copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_SaveLoadVar_copy(void* p, const SwampScript::SaveLoadVar& v)
{
    new (p) SwampScript::SaveLoadVar(v);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit SaveLoadVar::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_SaveLoadVar_assign(SwampScript::SaveLoadVar* p, const SwampScript::SaveLoadVar& v)
{
    *p = v;
}

// FUNCTION: USA 0x000ABA4C
// PROTOTYPE: PREVIEW 0x0049687E identical
// NAME: proto-pdb:default.pdb
struct SwampScript::QueuedMessage { // NAME: proto-pdb:default.pdb
    unsigned int msg;                  // 0x00
    unsigned int intParam0;            // 0x04
    float floatParam0;                 // 0x08
    float floatParam1;                 // 0x0C
    SwampString stringParam;           // 0x10
};
DECOMP_SIZE_ASSERT(SwampScript::QueuedMessage, 0x2C);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit QueuedMessage copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_QueuedMessage_copy(void* p, const SwampScript::QueuedMessage& m)
{
    new (p) SwampScript::QueuedMessage(m);
}

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
    typedef std::map<SwampString, CreationFunc> Map;       // NAME: proto-pdb:default.pdb

    void Add(const SwampString& name, CreationFunc creationFunc);
    SwampScript* GetGenericScript(SwampSceneNode* node) const;
    static SwampScriptManager& Instance();
    void RegisterScriptTimerEvent(SwampScript* script, unsigned int id, float deltaTime, bool looped);
    void UnregisterScriptTimerEvent(SwampScript* script, unsigned int id);
    void RemoveScriptTimerEvents(SwampScript* script);
    void Remove(SwampScript* script);
    void Cleanup();
    void AdjustRegisteredFrameTickCount(int n) { _numRegisteredFrameTicks += n; }

    Map _scripts;                      // 0x00
    std::stack<SwampScript*> _deadScripts; // 0x0C
    std::multiset<ScriptTimerEvent> _timerEvents; // 0x20
    std::multiset<ScriptTimerEvent> _timerNewEvents; // 0x2C
    std::list<ScriptTimerEvent> _timerPausedEvents; // 0x38
    int _numRegisteredFrameTicks;      // 0x44
};
DECOMP_SIZE_ASSERT(SwampScriptManager, 0x48);
DECOMP_SIZE_ASSERT(SwampScriptManager::ScriptTimerEvent, 0x14);

// FUNCTION: USA 0x000AB638
// PROTOTYPE: PREVIEW 0x0049646A identical
// NAME: proto-pdb:default.pdb
bool operator<(const SwampScriptManager::ScriptTimerEvent& t0, const SwampScriptManager::ScriptTimerEvent& t1)
{
    return t0.absTime < t1.absTime;
}

// FUNCTION: USA 0x000AB9CE
// NAME: proto-pdb:default.pdb
void SwampScriptManager::Cleanup()
{
    while (!_deadScripts.empty()) {
        SwampScript* script = _deadScripts.top();
        _deadScripts.pop();
        delete script;
    }
}

// FUNCTION: USA 0x000AC33D
// PROTOTYPE: PREVIEW 0x00497199 identical
// NAME: proto-pdb:default.pdb
void SwampScript::ClearQueuedMessages()
{
    _pendingMessages.clear();
}

// FUNCTION: USA 0x000AC30C
// PROTOTYPE: PREVIEW 0x00497168 identical
// NAME: proto-pdb:default.pdb
void SwampScript::RemoveMatchingQueuedMessages(unsigned int msg)
{
    std::list<QueuedMessage>::iterator it = _pendingMessages.begin();
    while (it != _pendingMessages.end()) {
        if (it->msg == msg)
            it = _pendingMessages.erase(it);
        else
            ++it;
    }
}

// FUNCTION: USA 0x000ACB7A
// PROTOTYPE: PREVIEW 0x00497A20 identical
// NAME: proto-pdb:default.pdb
void SwampScript::QueueAnimationMessage(unsigned int msg)
{
    QueuedMessage qm;
    qm.msg = msg;
    _pendingMessages.push_back(qm);
}

// FUNCTION: USA 0x000ACC0C
// PROTOTYPE: PREVIEW 0x00497AB2 identical
// NAME: proto-pdb:default.pdb
void SwampScript::QueueSoundFinishedMessage(const SwampString& name)
{
    QueuedMessage qm;
    qm.msg = SOUND_FINISHED;
    qm.stringParam = name;
    _pendingMessages.push_back(qm);
}

// FUNCTION: USA 0x000ACC54
// PROTOTYPE: PREVIEW 0x00497AFA identical
// NAME: proto-pdb:default.pdb
void SwampScript::QueueSoundStreamerFinishedMessage(const SwampString& name)
{
    QueuedMessage qm;
    qm.msg = SOUND_STREAMER_FINISHED;
    qm.stringParam = name;
    _pendingMessages.push_back(qm);
}

// FUNCTION: USA 0x000ACC9C
// PROTOTYPE: PREVIEW 0x00497B42 identical
// NAME: proto-pdb:default.pdb
void SwampScript::QueueMovieFinishedMessage(const SwampString& name)
{
    QueuedMessage qm;
    qm.msg = MOVIE_FINISHED;
    qm.stringParam = name;
    _pendingMessages.push_back(qm);
}

// FUNCTION: USA 0x000ACCE4
// NAME: proto-pdb:default.pdb
void SwampScriptManager::Remove(SwampScript* script)
{
    _deadScripts.push(script);
}

// FUNCTION: USA 0x000ACEDD
// PROTOTYPE: PREVIEW 0x00497D83 identical
// NAME: proto-pdb:default.pdb
SwampScript* SwampScriptManager::GetGenericScript(SwampSceneNode* node) const
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
    return new("SwampScript.cpp", 495) SwampScript(node);
}

// FUNCTION: USA 0x000AD15F
// PROTOTYPE: PREVIEW 0x00498005 identical
// NAME: proto-pdb:default.pdb
void SwampScriptManager::Add(const SwampString& name, CreationFunc creationFunc)
{
    _scripts[BeepLib::MakeLower(name)] = creationFunc;
}

// FUNCTION: USA 0x000AD264
// PROTOTYPE: PREVIEW 0x0049810A identical
// NAME: proto-pdb:default.pdb
void SwampScript::RegisterTimerEvent(unsigned int id, float deltaTime, bool looped)
{
    if (_valid)
        SwampScriptManager::Instance().RegisterScriptTimerEvent(this, id, deltaTime, looped);
}

// FUNCTION: USA 0x000AD28A
// PROTOTYPE: PREVIEW 0x00498130 identical
// NAME: proto-pdb:default.pdb
void SwampScript::UnregisterTimerEvent(unsigned int id)
{
    SwampScriptManager::Instance().UnregisterScriptTimerEvent(this, id);
}

// FUNCTION: USA 0x000AD29E
// PROTOTYPE: PREVIEW 0x00498144 identical
// NAME: proto-pdb:default.pdb
void SwampScript::RemoveTimerEvents()
{
    SwampScriptManager::Instance().RemoveScriptTimerEvents(this);
}

// FUNCTION: USA 0x000AD2AC
// PROTOTYPE: PREVIEW 0x00498152 identical
// NAME: proto-pdb:default.pdb
void SwampScript::RegisterFrameTick()
{
    if (_valid) {
        SwampScriptManager::Instance().AdjustRegisteredFrameTickCount(1);
        _frameTicks++;
    }
}

// FUNCTION: USA 0x000AD2C2
// PROTOTYPE: PREVIEW 0x00498168 identical
// NAME: proto-pdb:default.pdb
void SwampScript::UnRegisterFrameTick()
{
    SwampScriptManager& mgr = SwampScriptManager::Instance();
    mgr.AdjustRegisteredFrameTickCount(-_frameTicks);
    _frameTicks = 0;
}

// FUNCTION: USA 0x000AC822
// PROTOTYPE: PREVIEW 0x004976F5 identical
// NAME: proto-pdb:default.pdb
void SwampScriptManager::RegisterScriptTimerEvent(SwampScript* script, unsigned int id, float deltaTime, bool looped)
{
    ScriptTimerEvent t;
    t.script = script;
    t.id = id;
    t.deltaTime = deltaTime;
    t.looped = looped;
    t.absTime = deltaTime + g_swamp->GetInput().GetGameTime();
    _timerNewEvents.insert(t);
}

#ifdef NON_MATCHING
// best: match_one objdiff 99.891304, bytes mismatch 1 (+0x2A) (6 runs; continue, (*i), i++ forms identical)
// FUNCTION: USA 0x000AB8C9
// PROTOTYPE: PREVIEW 0x004966FB identical
// NAME: proto-pdb:default.pdb
void SwampScriptManager::UnregisterScriptTimerEvent(SwampScript* script, unsigned int id)
{
    for (std::multiset<ScriptTimerEvent>::iterator i = _timerEvents.begin(); i != _timerEvents.end();) {
        if (i->id == id && i->script == script)
            i->script = 0;
        else
            ++i;
    }
    for (std::list<ScriptTimerEvent>::iterator v1 = _timerPausedEvents.begin(); v1 != _timerPausedEvents.end();) {
        if (v1->id == id && v1->script == script)
            v1 = _timerPausedEvents.erase(v1);
        else
            ++v1;
    }
}
#endif

// FUNCTION: USA 0x000AD2D6
// PROTOTYPE: PREVIEW 0x0049817C identical
// NAME: proto-pdb:default.pdb
void SwampScript::InvalidateScript()
{
    _valid = false;
    SwampSceneNode* node = _node;
    if (node) {
        if (node->GetScript() != this)
            return;
        node->PopScript();
        SwampScript* script = _node->GetScript();
        if (script)
            script->GainingControl(this);
    }
    SwampScriptManager::Instance().Remove(this);
    UnRegisterFrameTick();
    RemoveTimerEvents();
}
