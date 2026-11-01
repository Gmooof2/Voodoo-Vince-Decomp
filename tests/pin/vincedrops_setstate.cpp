// compiler-pin test (1.4 T9): VinceDropsInCutscene::SetState, retail 0x000A871D, 1732 B (switch; jump table inside
// the proc). Types per prototype default.pdb (docs/evidence/compiler_pin.md): VinceDropsInCutscene size 0x84 (base
// CutsceneDriver; _vince 0x78, _ethel 0x7C, _wasWhammyReady 0x80); CutsceneDriver size 0x78 (base SwampScript; _cam
// 0x4C, _cancelled 0x50, _autostop 0x51, _state 0x54, _cameraShake 0x58, _cameraName 0x5C); SwampScript size 0x4C
// (vfptr 0x0; ?InvalidateScript@SwampScript@@UAEXXZ is a public virtual in symbols.txt); Swamp size 0x68
// (_animationManager 0x18; GetAnimationManager in its PDB field list, no out-of-line symbol, so inline);
// AnimationManager base ResourceManager<AnimationResource> at 0x0; Vince base Critter base GatorNode, all at 0x0; ResourceBase::_RefCount 0x1C; std::basic_string size 0x1C. Members this body does not
// touch are opaque `char` pads sized by those PDB offsets (D78). SwampSceneNode sits at 0x0 of GatorNode: retail
// calls SetOption with the unadjusted _vince/_ethel pointer. Callees are declared only (retail mangled names).
// Retail strings are not reproduced (D46): each `push imm32` string is an extern array named by its retail VA,
// resolved through config/retail/symbols_pin.txt.

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
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);
    char _pad00[0x1C];  // VC 7.1 _String_val/_Bx/_Mysize/_Myres, opaque (not used by this body)
};

}  // namespace std

class ResourceBase {
public:
    char _Name[0x1C];  // std::basic_string _Name 0x0, opaque here (not used by this body)
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
    char _pad00[0x18];  // Swamp members before _animationManager, opaque (not used by this body)
    // inline accessor named in the PDB Swamp field list (no out-of-line symbol); retail evaluates Load's `this`
    // before the outer call's arguments, as this inline call does (a direct member read loads it last)
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

class SwampScript {
public:
    virtual void InvalidateScript();
    char _pad04[0x4C - 0x4];  // SwampScript members past the vptr, opaque (not used by this body)
};

class CutsceneDriver : public SwampScript {
protected:
    AnimCam* NewCamera(const std::basic_string<char>& name);
public:
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
        // 0x1..0xA: names not in the evidence doc; the case labels below use the values
        STATE_V_ONIT = 0xB,
        STATE_LAST = 0xC
    };
private:
    virtual void SetState(int state);
public:
    Vince* _vince;
    Critter* _ethel;
    bool _wasWhammyReady;
};

// Retail string addresses (push imm32 operands), in body order; contents not reproduced (D46).
extern "C" const char s_001EA324[], s_001EA30C[], s_001EA2FC[], s_001EA2EC[], s_001EA2D0[], s_001EA2AC[],
    s_001EA29C[], s_001EA284[], s_001EA264[], s_001EA254[], s_001EA244[], s_001EA22C[], s_001EA21C[], s_001EA200[],
    s_001EA1DC[], s_001EA1CC[], s_001EA1B0[], s_001EA1A0[], s_001EA180[], s_001EA160[], s_001EA150[], s_001EA138[],
    s_001EA128[], s_001EA108[], s_001EA0F8[], s_001EA0E4[], s_001EA0C8[];

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
