// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampScript.cpp OBJ: Xbox\Official_Release\SwampScript.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    // !FAKE: inliner; hand form of the VC 7.1 ctor with _Tidy() inlined; _Tidy(true) stays out of line (C0245)
    basic_string() { _Myres = _BUF_SIZE - 1; _Eos(0); }
    basic_string(const E* ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, (unsigned int)-1); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    const E* c_str() const { return _Myptr(); }
    unsigned int size() const { return _Mysize; }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

protected:
    void _Eos(unsigned int _Newsize);
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
    list();
    ~list() { _Tidy(); }

protected:
    void _Tidy();
    void _Incsize(unsigned int _Count);
    unsigned int max_size() const;

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
    ~vector() { _Tidy(); }
    iterator begin() { return _Myfirst; }
    iterator end() { return _Mylast; }
    void push_back(const T& _Val);
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class T> struct less {
    bool operator()(const T& _Left, const T& _Right) const { return (_Left < _Right); }
};
template <class T1, class T2> struct pair {
    pair() : first(T1()), second(T2()) {}
    pair(const T1& _Val1, const T2& _Val2) : first(_Val1), second(_Val2) {}
    T1 first;
    T2 second;
};
template <class K, class Pr, class A, bool Mfl> class _Tset_traits {
public:
    typedef K key_type;
    typedef K value_type;
    Pr comp;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
    Pr comp;
};
template <class Tr> class _Tree_nod : public Tr { // NAME: proto-pdb:default.pdb
protected:
    struct _Node {
        _Node(_Node* _Larg, _Node* _Parg, _Node* _Rarg, const typename Tr::value_type& _Val, char _Carg)
            : _Left(_Larg), _Parent(_Parg), _Right(_Rarg), _Myval(_Val), _Color(_Carg), _Isnil(false)
        {
        }

        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
        char _Color;
        char _Isnil;
    };
};
template <class Tr> class _Tree : public _Tree_nod<Tr> {
    typedef typename _Tree_nod<Tr>::_Node _Node;
public:
    class const_iterator {
    public:
        const_iterator() {}
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        const typename Tr::value_type& operator*() const { return _Ptr->_Myval; }
        const typename Tr::value_type* operator->() const { return &**this; }
        const_iterator& operator++() { _Inc(); return *this; }
        bool operator==(const const_iterator& _Right) const { return _Ptr == _Right._Ptr; }
        bool operator!=(const const_iterator& _Right) const { return !(*this == _Right); }
        void _Inc()
        {
            if (_Ptr->_Isnil)
                ;
            else if (!_Ptr->_Right->_Isnil)
                _Ptr = _Min(_Ptr->_Right);
            else {
                _Node* _Pnode;
                while (!(_Pnode = _Ptr->_Parent)->_Isnil && _Ptr == _Pnode->_Right)
                    _Ptr = _Pnode;
                _Ptr = _Pnode;
            }
        }

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
    const_iterator end() const { return const_iterator(_Myhead); }
    const_iterator find(const typename Tr::key_type& _Keyval) const;
    pair<iterator, bool> insert(const typename Tr::value_type& _Val);
    iterator erase(iterator _Where);
    iterator erase(iterator _First, iterator _Last);
    void clear();
    iterator upper_bound(const typename Tr::value_type& _Keyval) { return iterator(_Ubound(_Keyval)); }

protected:
    static _Node* _Min(_Node* _Pnode)
    {
        while (!_Pnode->_Left->_Isnil)
            _Pnode = _Pnode->_Left;
        return _Pnode;
    }
    _Node* _Ubound(const typename Tr::value_type& _Keyval) const;
    iterator _Insert(bool _Addleft, _Node* _Wherenode, const typename Tr::value_type& _Val);

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
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map
    : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
public:
    T& operator[](const K& _Keyval);
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

extern "C" int __cdecl sscanf(const char*, const char*, ...); // NAME: proto-pdb:default.pdb
const SwampString format(const char* fmt, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString& id, int def) const;
    double GetFloatRecord(const SwampString& id, double def) const;
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
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

class SwampSceneNode;
class SwampRenderer; // NAME: proto-pdb:default.pdb

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void AddNode(SwampSceneNode* node, unsigned int zone);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    SwampScene& GetScene() const { return *_scene; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    SwampScene* _scene;                // 0x0C
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AABox; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampSceneNode(int type);
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    void SetName(const SwampString& name) { _Name = name; }
    Vector3 GetWorldFacing() const;
    SwampScript* GetScript() const;
    void SetScript(SwampScript* script);
    bool PopScript();

    int _Type;                         // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x80];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

void* operator new(unsigned int size, const char* file, int line);
inline void* operator new(unsigned int, void* p) { return p; }

class SwampScript { // NAME: proto-pdb:default.pdb
    // !FAKE: c1xx front end; friend declaration (access only); GetGenericScript constructs through the protected ctor (friendship not in the PDB TPI)
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
    void QueueTriggerMessage(const SwampString& name, float value, float prev, unsigned int bone);
    void RemoveMatchingQueuedMessages(unsigned int msg);
    void RegisterTimerEvent(unsigned int id, float deltaTime, bool looped);
    void ClearQueuedMessages();
    void UnregisterTimerEvent(unsigned int id);
    void RemoveTimerEvents();
    void RegisterFrameTick();
    void UnRegisterFrameTick();
    bool CheckActivationRange(const Vector3& vincePos, const Vector3& vinceDir, float range) const;

    SwampString _name;                // 0x04
    SwampSceneNode* _node;             // 0x20
    SwampScript* _parent;              // 0x24
    int _frameTicks;                   // 0x28
    bool _valid;                       // 0x2C
    bool _sleeping;                    // 0x2D
    std::list<QueuedMessage> _pendingMessages; // 0x30
    std::vector<SaveLoadVar> _saveLoadVars; // 0x3C
protected:
    static int _scriptCount;
    SwampScript(SwampSceneNode* node);
    void RegisterSaveLoadVar(const SwampString& name, int* addr, int defaultval);
    void RegisterSaveLoadVar(const SwampString& name, float* addr, float defaultval);
    void RegisterSaveLoadVar(const SwampString& name, bool* addr, bool defaultval);
    void RegisterSaveLoadVar(const SwampString& name, SwampString* addr, const char* defaultval);
    void RegisterSaveLoadVar(const SwampString& name, Vector3* addr, Vector3 defaultval);
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
    SwampScript* Create(const SwampString& name, SwampSceneNode* node) const;
    SwampScript* GetGenericScript(SwampSceneNode* node) const;
    static SwampScriptManager& Instance();
    void RegisterScriptTimerEvent(SwampScript* script, unsigned int id, float deltaTime, bool looped);
    void UnregisterScriptTimerEvent(SwampScript* script, unsigned int id);
    void RemoveScriptTimerEvents(SwampScript* script);
    void Remove(SwampScript* script);
    void Cleanup();
    void ProcessTimerEvents();
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

// FUNCTION: USA 0x000AB5CD
// PROTOTYPE: PREVIEW 0x004E05F6 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<SwampScript::SaveLoadVar,std::allocator<SwampScript::SaveLoadVar> >::size()const;

// FUNCTION: USA 0x000AB638
// PROTOTYPE: PREVIEW 0x0049646A identical
// NAME: proto-pdb:default.pdb
// !FAKE: c2 register allocation; inline operator<; inline drops the same-unit register summary, so the _Tree rows 0x000AB73E/0x000AC0F0 save esi/edi across the call as retail does (C0222)
inline bool operator<(const SwampScriptManager::ScriptTimerEvent& t0, const SwampScriptManager::ScriptTimerEvent& t1)
{
    return t0.absTime < t1.absTime;
}

// FUNCTION: USA 0x000AB654
// NAME: proto-pdb:default.pdb
bool SwampScript::CheckActivationRange(const Vector3& vincePos, const Vector3& vinceDir, float range) const
{
    Vector3 objNormal = _node->GetWorldFacing();
    // !FAKE: inliner; named Vector3 v1, pointer local p, float v2; hand form of the plane distance (C0191, C0214)
    Vector3 v1(-objNormal.x, -objNormal.y, -objNormal.z);
    const Vector3* p = &_node->GetWorldPosition();
    float v2 = v1.x * p->x + v1.y * p->y + v1.z * p->z;
    // !FAKE: c2 register allocation; reference local v4; v2 held in st(1) (fadd st(1), fstp st(0)) only with it (probe 232 B -> 234 B)
    const float& v4 = v2;
    // !FAKE: c2 register allocation; product stand-ins v5 and v6; term orders x,z,y and z,y,x only with them (match_one 99.96 -> 100.0)
    float v5 = objNormal.x * vincePos.x;
    if ((v5 + objNormal.z * vincePos.z + objNormal.y * vincePos.y) + v4 <= 0.0f)
        return false;
    float v6 = objNormal.z * vinceDir.z;
    // !FAKE: c2 block layout; split expression (two ifs, no ||); one || put return true before the shared false
    if (v6 + objNormal.y * vinceDir.y + objNormal.x * vinceDir.x > -0.75f)
        return false;
    // !FAKE: inliner; pointer local p and named Vector3 v3; hand form of the distance vector (C0191)
    p = &_node->GetWorldPosition();
    Vector3 v3(vincePos.x - p->x, vincePos.y - p->y, vincePos.z - p->z);
    if (v3.x * v3.x + v3.y * v3.y + v3.z * v3.z > range * range)
        return false;
    return true;
}

// FUNCTION: USA 0x000AB932
// NAME: proto-pdb:default.pdb
void SwampScriptManager::RemoveScriptTimerEvents(SwampScript* script)
{
    std::multiset<ScriptTimerEvent>::iterator i;
    for (i = _timerEvents.begin(); i != _timerEvents.end();) {
        if (script && i->script != script)
            ++i;
        else
            i->script = 0;
    }
    // !FAKE: c1xx front end; neutral local v1; the PDB records no list iterator (register-held)
    for (std::list<ScriptTimerEvent>::iterator v1 = _timerPausedEvents.begin(); v1 != _timerPausedEvents.end();) {
        if (v1->script == script)
            v1 = _timerPausedEvents.erase(v1);
        else
            ++v1;
    }
    for (i = _timerNewEvents.begin(); i != _timerNewEvents.end();) {
        if (script && i->script != script)
            ++i;
        else
            i->script = 0;
    }
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

// FUNCTION: USA 0x000ABC46
// PROTOTYPE: PREVIEW 0x00496A78 identical
// NAME: proto-pdb:default.pdb
void SwampScript::Load(const BeepTDF& tdf)
{
    // !FAKE: c1xx front end; neutral local v1; the PDB records no vector iterator (register-held)
    for (std::vector<SaveLoadVar>::iterator v1 = _saveLoadVars.begin(); v1 != _saveLoadVars.end(); ++v1) {
        SaveLoadVar var = *v1;
        switch (var._type) {
        case SaveLoadVar::TYPE_INT:
            *(int*)var._addr = tdf.GetIntRecord(var._name, *(int*)var._addr);
            break;
        case SaveLoadVar::TYPE_FLOAT:
            *(float*)var._addr = tdf.GetFloatRecord(var._name, *(float*)var._addr);
            break;
        case SaveLoadVar::TYPE_STRING:
            *(SwampString*)var._addr = tdf.GetStringRecord(var._name, *(SwampString*)var._addr);
            break;
        case SaveLoadVar::TYPE_VECTOR3: {
            SwampString str = tdf.GetStringRecord(var._name, "");
            if (str.size())
                sscanf(str.c_str(), "%f %f %f", &((Vector3*)var._addr)->x, &((Vector3*)var._addr)->y,
                    &((Vector3*)var._addr)->z);
            break;
        }
        case SaveLoadVar::TYPE_BOOL:
            *(bool*)var._addr = tdf.GetBoolRecord(var._name, *(bool*)var._addr);
            break;
        }
    }
}

// FUNCTION: USA 0x000ABA7B
// NAME: proto-pdb:default.pdb
SwampString SwampScript::Save()
{
    SwampString str;
    for (std::vector<SaveLoadVar>::iterator it = _saveLoadVars.begin(); it != _saveLoadVars.end(); ++it) {
        SaveLoadVar var = *it;
        str += format("\t\t%s = ", var._name.c_str());
        switch (var._type) {
        case SaveLoadVar::TYPE_INT:
            str += format("%d;\n", *(int*)var._addr);
            break;
        case SaveLoadVar::TYPE_FLOAT:
            str += format("%f;\n", *(float*)var._addr);
            break;
        case SaveLoadVar::TYPE_STRING:
            str += format("%s;\n", ((SwampString*)var._addr)->c_str());
            break;
        case SaveLoadVar::TYPE_VECTOR3:
            str += format("%f %f %f;\n", ((Vector3*)var._addr)->x, ((Vector3*)var._addr)->y,
                ((Vector3*)var._addr)->z);
            break;
        case SaveLoadVar::TYPE_BOOL:
            str += format("%d;\n", *(bool*)var._addr);
            break;
        }
    }
    return str;
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

// FUNCTION: USA 0x000AC65D
// PROTOTYPE: PREVIEW 0x00497530 identical
// NAME: proto-pdb:default.pdb
void SwampScriptManager::ProcessTimerEvents()
{
    ScriptTimerEvent t;
    t.absTime = g_swamp->GetInput().GetGameTime();
    std::multiset<ScriptTimerEvent>::iterator l, it;
    for (l = _timerEvents.begin(); l != _timerEvents.end();) {
        // !FAKE: c2 register allocation; neutral local v1; retail holds the script in edi across GetScript
        SwampScript* v1 = l->script;
        if (v1 && (v1->_sleeping || (v1->_node && v1->_node->GetScript() != v1))) {
            ScriptTimerEvent n = *l;
            n.absTime -= t.absTime;
            _timerPausedEvents.push_back(n);
            l = _timerEvents.erase(l);
        } else
            ++l;
    }
    it = _timerEvents.upper_bound(t);
    for (std::multiset<ScriptTimerEvent>::iterator i = _timerEvents.begin(); i != it; ++i) {
        if (i->script) {
            i->script->TimerEvent(i->id);
            if (i->looped) {
                ScriptTimerEvent n = *i;
                n.absTime += n.deltaTime;
                _timerEvents.insert(n);
                it = _timerEvents.upper_bound(t);
            }
        }
    }
    _timerEvents.erase(_timerEvents.begin(), it);
    for (std::list<ScriptTimerEvent>::iterator it = _timerPausedEvents.begin(); it != _timerPausedEvents.end();) {
        // !FAKE: c2 register allocation; neutral local v1; retail holds the script in esi across GetScript
        SwampScript* v1 = it->script;
        if (v1 && !v1->_sleeping && v1->_node && v1->_node->GetScript() == v1) {
            ScriptTimerEvent n = *it;
            n.absTime += t.absTime;
            _timerEvents.insert(n);
            it = _timerPausedEvents.erase(it);
        } else
            ++it;
    }
    for (std::multiset<ScriptTimerEvent>::iterator i = _timerNewEvents.begin(); i != _timerNewEvents.end(); ++i)
        _timerEvents.insert(*i);
    _timerNewEvents.clear();
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

// FUNCTION: USA 0x000ACBB2
// NAME: proto-pdb:default.pdb
void SwampScript::QueueTriggerMessage(const SwampString& name, float value, float prev, unsigned int bone)
{
    QueuedMessage qm;
    qm.msg = ANIMATION_TRIGGER;
    qm.stringParam = name;
    qm.intParam0 = bone;
    qm.floatParam0 = value;
    qm.floatParam1 = prev;
    _pendingMessages.push_back(qm);
}

// FUNCTION: USA 0x000ACCE4
// NAME: proto-pdb:default.pdb
void SwampScriptManager::Remove(SwampScript* script)
{
    _deadScripts.push(script);
}

// FUNCTION: USA 0x000ACE6A
// PROTOTYPE: PREVIEW 0x00497D10 identical
// NAME: proto-pdb:default.pdb
SwampScript::SwampScript(SwampSceneNode* node)
    : _node(node), _parent(0), _frameTicks(0), _valid(true), _sleeping(false)
{
    _scriptCount++;
}

// FUNCTION: USA 0x000ACEB0
// NAME: proto-pdb:default.pdb
SwampScript::~SwampScript()
{
    _scriptCount--;
}

// FUNCTION: USA 0x000ACEDD
// PROTOTYPE: PREVIEW 0x00497D83 identical
// NAME: proto-pdb:default.pdb
SwampScript* SwampScriptManager::GetGenericScript(SwampSceneNode* node) const
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
    return new("SwampScript.cpp", 495) SwampScript(node);
}

// FUNCTION: USA 0x000ACFA2
// NAME: proto-pdb:default.pdb
void SwampScript::RegisterSaveLoadVar(const SwampString& name, float* addr, float defaultval)
{
    SaveLoadVar var;
    var._type = SaveLoadVar::TYPE_FLOAT;
    var._name = name;
    var._addr = addr;
    *addr = defaultval;
    _saveLoadVars.push_back(var);
}

// FUNCTION: USA 0x000ACFFA
// NAME: proto-pdb:default.pdb
void SwampScript::RegisterSaveLoadVar(const SwampString& name, int* addr, int defaultval)
{
    SaveLoadVar var;
    var._type = SaveLoadVar::TYPE_INT;
    var._name = name;
    *addr = defaultval;
    var._addr = addr;
    _saveLoadVars.push_back(var);
}

// FUNCTION: USA 0x000AD04F
// NAME: proto-pdb:default.pdb
void SwampScript::RegisterSaveLoadVar(const SwampString& name, bool* addr, bool defaultval)
{
    SaveLoadVar var;
    var._type = SaveLoadVar::TYPE_BOOL;
    var._name = name;
    *addr = defaultval;
    var._addr = addr;
    _saveLoadVars.push_back(var);
}

// FUNCTION: USA 0x000AD0A7
// NAME: proto-pdb:default.pdb
void SwampScript::RegisterSaveLoadVar(const SwampString& name, SwampString* addr, const char* defaultval)
{
    SaveLoadVar var;
    var._type = SaveLoadVar::TYPE_STRING;
    var._name = name;
    var._addr = addr;
    *addr = defaultval;
    _saveLoadVars.push_back(var);
}

// FUNCTION: USA 0x000AD102
// NAME: proto-pdb:default.pdb
void SwampScript::RegisterSaveLoadVar(const SwampString& name, Vector3* addr, Vector3 defaultval)
{
    SaveLoadVar var;
    var._type = SaveLoadVar::TYPE_VECTOR3;
    var._name = name;
    var._addr = addr;
    *addr = defaultval;
    _saveLoadVars.push_back(var);
}

// FUNCTION: USA 0x000AD15F
// PROTOTYPE: PREVIEW 0x00498005 identical
// NAME: proto-pdb:default.pdb
void SwampScriptManager::Add(const SwampString& name, CreationFunc creationFunc)
{
    _scripts[BeepLib::MakeLower(name)] = creationFunc;
}

// FUNCTION: USA 0x000AD194
// NAME: proto-pdb:default.pdb
SwampScript* SwampScriptManager::Create(const SwampString& name, SwampSceneNode* node) const
{
    SwampString lookup = BeepLib::MakeLower(name);
    Map::const_iterator it = _scripts.find(lookup);
    // !FAKE: c1xx front end; neutral local v1; retail re-tests the original node after creating one (register-held)
    SwampSceneNode* v1 = node;
    if (!node) {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        v1 = new("SwampScript.cpp", 428) SwampSceneNode(0);
        v1->SetName(name);
        g_swamp->GetScene().AddNode(v1, 0);
    }
    // !FAKE: c1xx front end; neutral local v2; the PDB records no script local (register-held)
    SwampScript* v2;
    if (it != _scripts.end()) {
        v2 = it->second(v1);
        if (v2)
            v2->_name = lookup;
        if (!node && v1->GetScript() != v2)
            v1->SetScript(v2);
    } else
        v2 = GetGenericScript(v1);
    return v2;
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

namespace std {
template <class Tr>
pair<typename _Tree<Tr>::iterator, bool> _Tree<Tr>::insert(const typename Tr::value_type& _Val)
{
    _Node* _Trynode = _Myhead->_Parent;
    _Node* _Wherenode = _Myhead;
    bool _Addleft = true;
    while (!_Trynode->_Isnil) {
        _Wherenode = _Trynode;
        _Addleft = this->comp(_Val, _Trynode->_Myval);
        _Trynode = _Addleft ? _Trynode->_Left : _Trynode->_Right;
    }
    // opaque std: the _Multi (Mfl true) path only; the unit's one _Tree is a multiset
    return (pair<iterator, bool>(_Insert(_Addleft, _Wherenode, _Val), true));
}
template <class Tr> typename _Tree<Tr>::_Node* _Tree<Tr>::_Ubound(const typename Tr::value_type& _Keyval) const
{
    _Node* _Pnode = _Myhead->_Parent;
    _Node* _Wherenode = _Myhead;
    while (!_Pnode->_Isnil)
        if (this->comp(_Keyval, _Pnode->_Myval)) {
            _Wherenode = _Pnode;
            _Pnode = _Pnode->_Left;
        } else
            _Pnode = _Pnode->_Right;
    return (_Wherenode);
}
}

// FUNCTION: USA 0x000AC383
// PROTOTYPE: PREVIEW 0x004971DF identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::iterator std::multiset<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent> >::insert(SwampScriptManager::ScriptTimerEvent const &);

// FUNCTION: USA 0x000AB605
// PROTOTYPE: PREVIEW 0x00496411 identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Node::_Node(std::_Tree_nod<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Node *,std::_Tree_nod<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Node *,std::_Tree_nod<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Node *,SwampScriptManager::ScriptTimerEvent const &,char);

// FUNCTION: USA 0x000AB73E
// PROTOTYPE: PREVIEW 0x00496570 identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Node * std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::_Ubound(SwampScriptManager::ScriptTimerEvent const &)const;

// FUNCTION: USA 0x000AC0F0
// PROTOTYPE: PREVIEW 0x00496F32 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::iterator,bool> std::_Tree<std::_Tset_traits<SwampScriptManager::ScriptTimerEvent,std::less<SwampScriptManager::ScriptTimerEvent>,std::allocator<SwampScriptManager::ScriptTimerEvent>,1> >::insert(SwampScriptManager::ScriptTimerEvent const &);

class exception { // NAME: proto-pdb:default.pdb
public:
    exception();
    exception(const exception& v1);
    virtual ~exception();
    virtual const char* what() const;

private:
    const char* _m_what;               // 0x04
    int _m_doFree;                     // 0x08
};
namespace std { // NAME: proto-pdb:default.pdb
class logic_error : public exception {
public:
    explicit logic_error(const basic_string<char, char_traits<char>, allocator<char> >& v1);
    logic_error(const logic_error& v1);
    virtual ~logic_error();
    virtual const char* what() const;

private:
    basic_string<char, char_traits<char>, allocator<char> > _Str; // 0x0C
};
class length_error : public logic_error {
public:
    explicit length_error(const basic_string<char, char_traits<char>, allocator<char> >& v1) : logic_error(v1) {}
    virtual ~length_error() {}
};
template <class T, class A> inline unsigned int list<T, A>::max_size() const
{
    unsigned int _Count = (unsigned int)(-1) / sizeof(T);
    return 0 < _Count ? _Count : 1;
}
template <class T, class A> void list<T, A>::_Incsize(unsigned int _Count)
{
    if (max_size() - _Mysize < _Count)
        throw length_error("list<T> too long");
    _Mysize += _Count;
}
} // namespace std

// FUNCTION: USA 0x000ABD81
// PROTOTYPE: PREVIEW 0x00496BF8 identical
// NAME: proto-pdb:default.pdb
template void std::list<SwampScript::QueuedMessage,std::allocator<SwampScript::QueuedMessage> >::_Incsize(unsigned int);
