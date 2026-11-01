// UNIT: src/Apps/Vince/core/SwingHook.cpp OBJ: core\XBox\OfficialRelease\SwingHook.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;

template <class T, class A> class list { // NAME: proto-pdb:default.pdb
    struct _Node {
        _Node* _Next;                  // 0x00
        _Node* _Prev;                  // 0x04
        T _Myval;                      // 0x08
    };
public:
    class iterator { // NAME: proto-pdb:default.pdb
    public:
        iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        bool operator!=(const iterator& _Right) const { return _Ptr != _Right._Ptr; }
        _Node* _Ptr;
    };
    iterator begin() { return iterator(_Myhead->_Next); }
    iterator end() { return iterator(_Myhead); }
    void push_back(const T& _Val) { _Insert(end(), _Val); }
    void _Insert(iterator _Where, const T& _Val);
    iterator erase(iterator _Where);
    void clear();

    char field_0[0x4];                 // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class SwampScript { // NAME: proto-pdb:default.pdb
public:
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

    char field_4[0x48];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    char field_4C[0x70];               // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class SwingHookTargetMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_QUERYLENGTH = 0x14
    };
    virtual void Message(SwampScript*, int, void*);

    float _ropeLength;                 // 0xBC
};
DECOMP_SIZE_ASSERT(SwingHookTargetMain, 0xC0);

// FUNCTION: USA 0x00126DA5
// PROTOTYPE: PREVIEW 0x0050F8FE identical
// NAME: proto-pdb:default.pdb
void SwingHookTargetMain::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_QUERYLENGTH)
        *(float*)data = _ropeLength;
}

class SwampSceneNode; // NAME: proto-pdb:default.pdb

class SwingHookManager { // NAME: proto-pdb:default.pdb
public:
    void Add(SwampSceneNode* node);
    void RemoveAll();

    std::list<SwampSceneNode*, std::allocator<SwampSceneNode*> > _swingHooks; // 0x00
};
DECOMP_SIZE_ASSERT(SwingHookManager, 0xC);

// FUNCTION: USA 0x00126DD7
// NAME: proto-pdb:default.pdb
void SwingHookManager::RemoveAll()
{
    std::list<SwampSceneNode*, std::allocator<SwampSceneNode*> >::iterator it = _swingHooks.begin();
    while (it != _swingHooks.end())
        it = _swingHooks.erase(it);
    _swingHooks.clear();
}

// FUNCTION: USA 0x00126F52
// PROTOTYPE: PREVIEW 0x0050FABA identical
// NAME: proto-pdb:default.pdb
void SwingHookManager::Add(SwampSceneNode* node)
{
    _swingHooks.push_back(node);
}
