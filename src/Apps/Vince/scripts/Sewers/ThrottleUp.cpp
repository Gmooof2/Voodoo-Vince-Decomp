// UNIT: src/Apps/Vince/scripts/Sewers/ThrottleUp.cpp OBJ: Xbox\OfficialRelease\ThrottleUp.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    typedef basic_string<E, T, A> _Myt;
    basic_string() { _Myres = 15; _Eos(0); }
    basic_string(const _Myt& _Right);
    basic_string(const _Myt& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    _Myt& operator=(const E* _Ptr) { return assign(_Ptr); }
    _Myt& assign(const E* _Ptr);
    int compare(const E* _Ptr) const;
    _Myt substr(unsigned int _Off, unsigned int _Count) const { return _Myt(*this, _Off, _Count); }
protected:
    void _Eos(unsigned int _Newsize);
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x18];                 // 0x00
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
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

struct AABox;                // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableCylinder : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xB4 - 0xA4];        // 0xA4
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    T& operator()() { return *_Resource; }
    const T& operator()() const { return *_Resource; }

    T* _Resource;                      // 0x00
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    float GetLength() const { return _length; }

    char field_24[0x2C - 0x24];        // 0x24
    float _length;                     // 0x2C
    char field_30[0x34 - 0x30];        // 0x30
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim().GetLength(); }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x38 - 0x28];        // 0x28
    float _time;                       // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl> GetCurrentAnimation() const { return _currentAnim; }

    char field_B4[0x140 - 0xB4];       // 0xB4
    ResourceHandle<AnimationControl> _currentAnim; // 0x140
    char field_144[0x184 - 0x144];     // 0x144
    float _animSoundRange;             // 0x184
    char field_188[0x200 - 0x188];     // 0x188
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void PlaySound(const SwampString&, unsigned int, bool, float, float);

    char field_200[0x218 - 0x200];     // 0x200
    SwampString _species;              // 0x218
    char field_234[0x2C8 - 0x234];     // 0x234
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class AirplaneMain : public CritterMain { // NAME: proto-pdb:default.pdb
protected:
    virtual void DoDeath();                                        // vtable+0x70

    char field_BC[0xF0 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(AirplaneMain, 0xF0);

class ThrottleUpAirplaneMain : public AirplaneMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_RESTART = 0x15,
        MESSAGE_MENU_TRY_AGAIN = 0x16,
        MESSAGE_MENU_LEAVE = 0x17
    };
    ThrottleUpAirplaneMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void DoDeath();

    char field_F0[0xF4 - 0xF0];        // 0xF0
    static bool _tryagainmenu;
};
DECOMP_SIZE_ASSERT(ThrottleUpAirplaneMain, 0xF4);

class SwampGamePad { // NAME: proto-pdb:default.pdb
public:
    bool WasButtonPressed(int) const;
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    const SwampGamePad* GetPad(int) const;

    char field_0[0x34];                // 0x00
    bool _pauseGame;                   // 0x34
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class CXBFont;               // NAME: proto-pdb:default.pdb

class ThrottleUpTryAgainMenu : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Update();

    CXBFont* _font;                    // 0xA4
    char field_A8[0xC4 - 0xA8];        // 0xA8 _message (wstring)
    SwampScript* _returnScript;        // 0xC4
};
DECOMP_SIZE_ASSERT(ThrottleUpTryAgainMenu, 0xC8);

class IrisCityMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript* from, int message, void* data);
};
DECOMP_SIZE_ASSERT(IrisCityMain, 0xBC);

class ThrottleUpObstacleMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript* source, int message, void* data);

    float _idleOffset;                 // 0x4C
};
DECOMP_SIZE_ASSERT(ThrottleUpObstacleMain, 0x50);

// FUNCTION: USA 0x000AA631
// PROTOTYPE: PREVIEW 0x0049545E identical
// NAME: proto-pdb:default.pdb
void ThrottleUpAirplaneMain::DoDeath()
{
    _tryagainmenu = true;
    AirplaneMain::DoDeath();
}

// FUNCTION: USA 0x000AA63D
// PROTOTYPE: PREVIEW 0x0049546A identical
// NAME: proto-pdb:default.pdb
void IrisCityMain::Message(SwampScript* from, int message, void* data)
{
    if (message == 6)
        GetCritter()->_animSoundRange = 100.0f;
    CritterMain::Message(from, message, data);
}

// FUNCTION: USA 0x000AA78E
// PROTOTYPE: PREVIEW 0x00495587 identical
// NAME: proto-pdb:default.pdb
SwampScript* ThrottleUpAirplaneMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\sewers\\throttleup\\ThrottleUp.h", 11) ThrottleUpAirplaneMain(node);
}

// FUNCTION: USA 0x000AA812
// PROTOTYPE: PREVIEW 0x004955FA identical
// NAME: proto-pdb:default.pdb
void ThrottleUpObstacleMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 6: {
        // !FAKE: c1xx front end; stand-in local; the PDB records no local, retail keeps _node in esi
        Critter* p = (Critter*)_node;
        // !FAKE: c1xx front end; stand-in local; the PDB records no local, retail holds the handle copy in edi to the end
        ResourceHandle<AnimationControl> tmp = p->GetCurrentAnimation();
        tmp().SetAnimationTime(tmp().GetAnimationLength() * _idleOffset);
        p->_animSoundRange = 10000.0f;
        SwampString name = p->_species;
        SwampString soundname;
        if (name.substr(0, 15) == "tunneltooltorch")
            soundname = "rtu_blowtorchcrane";
        else if (name.substr(0, 12) == "rotatingtrap")
            soundname = "rtu_rotatingblades";
        else
            soundname = "rtu_snippycrane";
        p->PlaySound(soundname, 9, true, 0.0f, 0.0f);
    }
    }
}

// FUNCTION: USA 0x000AA9F0
// PROTOTYPE: PREVIEW 0x00495718 identical
// NAME: proto-pdb:default.pdb
void ThrottleUpTryAgainMenu::Update()
{
    if (!g_swamp->_input->_pauseGame) {
        // !FAKE: c1xx front end; stand-in local; the PDB records no local, retail calls GetPad once and keeps it in edi
        const SwampGamePad* tmp = g_swamp->_input->GetPad(0);
        if (tmp->WasButtonPressed(8)) {
            SoundManager::PlaySound("frontbutton", 0, 0, false, 0.0f, 0.0f);
            _returnScript->Message(0, ThrottleUpAirplaneMain::MESSAGE_MENU_TRY_AGAIN, 0);
            Invalidate();
        } else if (tmp->WasButtonPressed(9)) {
            SoundManager::PlaySound("frontbutton", 0, 0, false, 0.0f, 0.0f);
            _returnScript->Message(0, ThrottleUpAirplaneMain::MESSAGE_MENU_LEAVE, 0);
            Invalidate();
        }
    }
}
