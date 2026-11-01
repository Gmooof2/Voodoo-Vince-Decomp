// UNIT: src/Apps/Vince/scripts/Carnival/Generator.cpp OBJ: Xbox\OfficialRelease\Generator.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string;
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class SwampSceneNode; // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();

    char field_24[0x44];               // 0x24
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T>& operator=(const ResourceHandle<T>& h)
    {
        if (_Resource)
            _Resource->SubRef();
        _Resource = h._Resource;
        if (_Resource)
            _Resource->AddRef();
        return *this;
    }

    T* _Resource;                      // 0x00
};

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

void* operator new(unsigned int size, const char* file, int line);

class GeneratorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_TURNON = 0x14,
        MESSAGE_TURNOFF = 0x15,
        MESSAGE_SETDELAY = 0x16
    };
    GeneratorMain(SwampSceneNode* node);
    virtual void Activate();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
public:
    char field_BC[0xC];                // 0xBC
};
DECOMP_SIZE_ASSERT(GeneratorMain, 0xC8);

// FUNCTION: USA 0x0003B042
// PROTOTYPE: PREVIEW 0x00429739 identical
// NAME: proto-pdb:default.pdb
void GeneratorMain::Activate()
{
    Message(this, MESSAGE_TURNON, 0);
}

// FUNCTION: USA 0x0003B04D
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; explicit member instantiation; retail emits this template member through a caller in this
// unit (not matched yet); the instantiation emits it instead
template ResourceHandle<AnimationControl>& ResourceHandle<AnimationControl>::operator=(
    const ResourceHandle<AnimationControl>&);

// FUNCTION: USA 0x0003B53D
// PROTOTYPE: PREVIEW 0x00429C35 identical
// NAME: proto-pdb:default.pdb
SwampScript* GeneratorMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\carnival\\Generator.h", 11) GeneratorMain(node);
}
