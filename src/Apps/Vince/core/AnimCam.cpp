// UNIT: src/Apps/Vince/core/AnimCam.cpp OBJ: core\XBox\OfficialRelease\AnimCam.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[0x1C / 4];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString AddExtension(const SwampString& s, const SwampString& ext); // NAME: proto-pdb:default.pdb
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle&);

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                        // 0x00
    AnimationManager* _animationManager;       // 0x18
    char field_1C[0x68 - 0x1C];                // 0x1C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04
    virtual void Activate();                                       // vtable+0x08
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    SwampScript* GetScript() const;

    int _Type;                         // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl> StartPlayback(const ResourceHandle<AnimationResource>&, bool, bool);

    char field_A4[0xC4 - 0xA4];        // 0xA4
    float _speed;                      // 0xC4
    char field_C8[0x10C - 0xC8];       // 0xC8
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    virtual void Activate();

    float _shakeFactor;                // 0x10C
    float _shakeDuration;              // 0x110
    float _shakeTimer;                 // 0x114
    bool _isCurrentCamera;             // 0x118
    bool _isAnimated;                  // 0x119
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class AnimCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    virtual void Activate();

    float _holdTime;                               // 0x11C
    float _delay;                                  // 0x120
    ResourceHandle<AnimationControl> _camPath;     // 0x124
};
DECOMP_SIZE_ASSERT(AnimCam, 0x128);

// FUNCTION: USA 0x000FDB05
// PROTOTYPE: PREVIEW 0x004E8961 identical
// NAME: proto-pdb:default.pdb
void AnimCam::Activate()
{
    _delay = _holdTime;
    if (_isAnimated)
    {
        SwampString name = BeepLib::AddExtension(_Name, ".campath");
        ResourceHandle<AnimationResource> path = g_swamp->_animationManager->Load(name);
        _camPath = StartPlayback(path, false, _speed < 0.0f);
    }
    if (GetScript())
        GetScript()->Activate();
}
