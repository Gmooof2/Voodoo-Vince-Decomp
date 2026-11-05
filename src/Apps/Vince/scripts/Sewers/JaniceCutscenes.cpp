// UNIT: src/Apps/Vince/scripts/Sewers/JaniceCutscenes.cpp OBJ: Xbox\OfficialRelease\JaniceCutscenes.obj OPT: /O1 /Oy

namespace std {

template<class _Elem>
struct char_traits {};

template<class _Ty>
class allocator {};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    basic_string(const _Elem *_Ptr);
    ~basic_string() { _Tidy(true); }
    int compare(const _Elem *_Ptr) const;
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);
    char _pad00[0x1C];
};

}

class ResourceBase {
public:
    char _Name[0x1C];
    unsigned int _RefCount;
    bool _valid;
};

class AnimationResource : public ResourceBase {
};

class AnimationControl;

template<class T>
class ResourceHandle {
public:
    ~ResourceHandle() { if (_Resource) --_Resource->_RefCount; }
    T* _Resource;
};

template<class T>
class ResourceManager {
public:
    ResourceHandle<T> Load(const std::basic_string<char>& name);
};

class AnimationManager : public ResourceManager<AnimationResource> {
};

class Swamp {
public:
    char _pad00[0x18];
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager;
};

extern Swamp* g_swamp;

class SwampSceneNode {
public:
    void SetOption(unsigned int option, bool a, bool b);
};

class GatorNode : public SwampSceneNode {
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
};

class Critter : public GatorNode {
};

class Vince : public Critter {
};

class AnimCam;

class FireFlyNode { // NAME: proto-pdb:default.pdb
public:
    void Play();
    void StopSpawning();
};

class SwampScript {
public:
    virtual void InvalidateScript();
    char _pad04[0x4C - 0x4];
};

class CutsceneDriver : public SwampScript {
protected:
    AnimCam* NewCamera(const std::basic_string<char>& name);
    void StateTransition(unsigned int anim, int state);
public:
    virtual void AnimationTrigger(unsigned int id, const std::basic_string<char>& name, float value, float prev, unsigned int bone);
    AnimCam* _cam;
    bool _cancelled;
    bool _autostop;
    int _state;
    float _cameraShake;
    std::basic_string<char> _cameraName;
};

class VinceDropsInCutscene : public CutsceneDriver {
public:
    enum States {
        STATE_V_FALLSIN = 0x0,
        // 0x1..0xA: names unknown; the case labels below use the values
        STATE_V_ONIT = 0xB,
        STATE_LAST = 0xC
    };
    VinceDropsInCutscene(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void SetState(int state);
public:
    Vince* _vince;
    Critter* _ethel;
    bool _wasWhammyReady;
};

class JaniceDefeatCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    enum States {
        STATE_N_EXPLODES = 0x0,
        STATE_J_DEATH = 0x1,
        STATE_V_WALKS = 0x2,
        STATE_V_WONDERS = 0x3,
        STATE_V_LOOKING = 0x4,
        STATE_V_HMMM = 0x5,
        STATE_LAST = 0x6
    };
private:
    virtual void SetState(int state);
public:
    Vince* _vince;
    Critter* _janice;
    GatorNode* _debris;
    bool _wasWhammyReady;
};

class LeavingRoachfortCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    enum States {
        STATE_V_INHALE = 0x0,
        STATE_V_FLOATS = 0x1,
        STATE_LAST = 0x2
    };
private:
    virtual void SetState(int state);
public:
    Vince* _vince;
    bool _wasWhammyReady;
};

class PlanGoesAwryCutscene : public CutsceneDriver {
public:
    enum Ids { // NAME: proto-pdb:default.pdb
        ID_VINCE = 0x1
    };
    enum States {
        STATE_E_PULLCORD = 0x0,
        STATE_LAST = 0xE
    };
    virtual void AnimationFinished(unsigned int id);
    virtual void AnimationTrigger(unsigned int id, const std::basic_string<char>& name, float value, float prev, unsigned int bone);

    Vince* _vince;                     // 0x78
    Critter* _ethel;                   // 0x7C
    Critter* _janice;                  // 0x80
    GatorNode* _balloon;               // 0x84
    FireFlyNode* _effect;              // 0x88
    bool _wasWhammyReady;              // 0x8C
};

// Retail string addresses (push imm32 operands), in body order; contents not reproduced.
extern "C" const char s_001EA324[], s_001EA30C[], s_001EA2FC[], s_001EA2EC[], s_001EA2D0[], s_001EA2AC[],
    s_001EA29C[], s_001EA284[], s_001EA264[], s_001EA254[], s_001EA244[], s_001EA22C[], s_001EA21C[], s_001EA200[],
    s_001EA1DC[], s_001EA1CC[], s_001EA1B0[], s_001EA1A0[], s_001EA180[], s_001EA160[], s_001EA150[], s_001EA138[],
    s_001EA128[], s_001EA108[], s_001EA0F8[], s_001EA0E4[], s_001EA0C8[];
extern "C" const char s_001EA60C[], s_001EA5F4[], s_001EA62C[], s_001EA61C[], s_001EA654[], s_001EA63C[],
    s_001EA67C[], s_001EA664[], s_001EA68C[], s_001EA6B4[], s_001EA69C[];

void* operator new(unsigned int size, const char* file, int line);

// FUNCTION: USA 0x000A82A9
// PROTOTYPE: PREVIEW 0x004931B7 identical
// NAME: proto-pdb:default.pdb
void PlanGoesAwryCutscene::AnimationFinished(unsigned int id)
{
    StateTransition(id, STATE_LAST);
}

// FUNCTION: USA 0x000A8353
// PROTOTYPE: PREVIEW 0x00493253 identical
// NAME: proto-pdb:default.pdb
void PlanGoesAwryCutscene::AnimationTrigger(unsigned int id, const std::basic_string<char>& name, float value, float prev, unsigned int bone)
{
    CutsceneDriver::AnimationTrigger(id, name, value, prev, bone);
    if (id == ID_VINCE && name.compare("trigger_sizzle") == 0 && _effect) {
        if (value > 0.0f)
            _effect->Play();
        else
            _effect->StopSpawning();
    }
}

// FUNCTION: USA 0x000A871D
// PROTOTYPE: PREVIEW 0x004935E1 identical
// NAME: proto-pdb:default.pdb
void VinceDropsInCutscene::SetState(int state)
{
    switch (state) {
    case STATE_V_FALLSIN:
        NewCamera(s_001EA324);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA30C), false, false);
        break;
    case 0x1:
        NewCamera(s_001EA2FC);
        break;
    case 0x2:
        NewCamera(s_001EA2EC);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA2D0), false, false);
        _ethel->SetOption(2, false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA2AC), false, false);
        break;
    case 0x3:
        NewCamera(s_001EA29C);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA284), false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA264), false, false);
        break;
    case 0x4:
        NewCamera(s_001EA254);
        _vince->SetOption(2, true, false);
        break;
    case 0x5:
        NewCamera(s_001EA244);
        _vince->SetOption(2, false, false);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA22C), false, false);
        break;
    case 0x6:
        NewCamera(s_001EA21C);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA200), false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA1DC), false, false);
        break;
    case 0x7:
        NewCamera(s_001EA1CC);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA1B0), false, false);
        break;
    case 0x8:
        NewCamera(s_001EA1A0);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA180), false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA160), false, false);
        break;
    case 0x9:
        NewCamera(s_001EA150);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA138), false, false);
        break;
    case 0xA:
        NewCamera(s_001EA128);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA108), false, false);
        break;
    case STATE_V_ONIT:
        NewCamera(s_001EA0F8);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA0E4), false, false);
        _ethel->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA0C8), true, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x000A9D72
// PROTOTYPE: PREVIEW 0x00494C06 identical
// NAME: proto-pdb:default.pdb
void JaniceDefeatCutscene::SetState(int state)
{
    switch (state) {
    case STATE_N_EXPLODES:
        NewCamera(s_001EA60C);
        _debris->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA5F4), false, false);
        break;
    case STATE_J_DEATH:
        NewCamera(s_001EA62C);
        _janice->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA61C), false, false);
        break;
    case STATE_V_WALKS:
        NewCamera(s_001EA654);
        _vince->SetOption(2, false, false);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA63C), false, false);
        _janice->SetOption(2, true, true);
        break;
    case STATE_V_WONDERS:
        NewCamera(s_001EA67C);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA664), false, false);
        break;
    case STATE_V_LOOKING:
        NewCamera(s_001EA68C);
        break;
    case STATE_V_HMMM:
        NewCamera(s_001EA6B4);
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load(s_001EA69C), false, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x000AA0E1
// PROTOTYPE: PREVIEW 0x00494F55 identical
// NAME: proto-pdb:default.pdb
void LeavingRoachfortCutscene::SetState(int state)
{
    switch (state) {
    case STATE_V_INHALE:
        NewCamera("camera_00_LR");
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load("vince_janiceDefeated003b"), false, false);
        break;
    case STATE_V_FLOATS:
        NewCamera("camera_01_LR");
        _vince->StartAnimation(g_swamp->GetAnimationManager()->Load("vince_janiceDefeated004"), false, false);
        break;
    }
    _state = state;
}

// FUNCTION: USA 0x000AA4C6
// PROTOTYPE: PREVIEW 0x004952F3 identical
// NAME: proto-pdb:default.pdb
SwampScript* VinceDropsInCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("WrathOfJanice\\JaniceCutscenes.cpp", 20) VinceDropsInCutscene(node);
}
