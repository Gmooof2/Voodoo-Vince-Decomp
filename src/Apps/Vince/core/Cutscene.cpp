// UNIT: src/Apps/Vince/core/Cutscene.cpp OBJ: core\XBox\OfficialRelease\Cutscene.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    bool empty() const { return _Mysize == 0; }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x14];                 // 0x00
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class CutsceneAnimationScript; // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line);

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

protected:
    SwampScript(SwampSceneNode* node);
    void RegisterSaveLoadVar(const SwampString&, SwampString*, const char*);
    void RegisterSaveLoadVar(const SwampString&, bool*, bool);

public:
    void RegisterFrameTick();

    char field_4[0x48];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    static SwampScriptManager& Instance();
    SwampScript* Create(const SwampString&, SwampSceneNode*) const;
};

// Retail layout: BeginPlayback tests [_stream + 0xDC] (proto PDB 0x64 = _file._fh, SoundStreamer 0xC0).
class SoundStreamer { // NAME: proto-pdb:default.pdb
public:
    void Update();
    void Play();
    void Stop();
    bool IsValid() const { return field_DC != 0; }
    bool IsValidData() const { return IsValid() && field_11A != 0; }

    char field_0[0xDC];                // 0x00
    int field_DC;                      // 0xDC
    char field_E0[0x11A - 0xE0];       // 0xE0
    unsigned short field_11A;          // 0x11A
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { DontDraw = 0x2 }; // NAME: proto-pdb:default.pdb
    bool PopScript();
    void SetOption(unsigned int option, bool on, bool recurse);
    void SetScript(SwampScript* script);
};

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void AddNode(SwampSceneNode* node, unsigned int);
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
};

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

class AnimCam : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    GatorNode(const ResourceHandle<ModelResource>& mh);

    char field_0[0x200];               // 0x00
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};
class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class MusicManager { // NAME: proto-pdb:default.pdb
public:
    void Suspend();
    void Resume();
};
extern MusicManager* g_MusicManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    void SetMute(bool);
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    double GetSessionTimeAccurate() const { return _sessionTimeInternal; }

    char field_0[0x18];                // 0x00
    double _sessionTimeInternal;       // 0x18
};

class DiskArbitrator { // NAME: proto-pdb:default.pdb
public:
    void Update();
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    DiskArbitrator& GetDiskArbitrator() const { return *_diskArbitrator; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x4];                 // 0x08
    SwampScene* _scene;                // 0x0C
    char field_10[0x4];                // 0x10
    DiskArbitrator* _diskArbitrator;   // 0x14
    char field_18[0x1C];               // 0x18
    SoundManager* _soundManager;       // 0x34
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class HUDUI : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void BeginLetterbox(float, float, float);
    void EndLetterbox(float);
    void ClearSubtitle();
private:
    static HUDUI* _self;
    friend class CutsceneDriver;
};

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CutsceneDriver(SwampSceneNode* node);
    virtual void FrameTick();
    virtual void AnimationFinished(unsigned int);                  // vtable+0x64
    virtual void AnimationTrigger(unsigned int, const SwampString&, float, float, unsigned int); // vtable+0x68
protected:
    virtual void Init();                                           // vtable+0x6C
    virtual void Cleanup();                                       // vtable+0x70
public:
    virtual void SetState(int);                                    // vtable+0x74

private:
    static SoundStreamer* _stream;
    static double _cutsceneStart;
    static double _cameraStart;

protected:
    void StateTransition(unsigned int, int);
    GatorNode* NewNode(unsigned int id);
    CutsceneAnimationScript* NewScript(SwampSceneNode* node, unsigned int id);
    AnimCam* NewCamera(const SwampString& name);

public:
    static void BeginPlayback();
    static void StopStream();

    AnimCam* _cam;                     // 0x4C
    bool _cancelled;                   // 0x50
    bool _autostop;                    // 0x51
    char field_52[0x2];                // 0x52
    int _state;                        // 0x54
    float _cameraShake;                // 0x58
    SwampString _cameraName;           // 0x5C
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

class CutsceneAnimationScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CutsceneAnimationScript(SwampSceneNode*, CutsceneDriver*, unsigned int);
    virtual void AnimationFinished();
    virtual void AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone);

    CutsceneDriver* _cd;              // 0x4C
    unsigned int _id;                  // 0x50
};
DECOMP_SIZE_ASSERT(CutsceneAnimationScript, 0x54);

class CutsceneRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        MESSAGE_QUERYTRIGGERED = 0x14
    };
    CutsceneRegionMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void EnteringRegion(SwampSceneNode* node);
    virtual void Load(const BeepTDF& tdf);

    SwampString _cutsceneName;         // 0x4C
    bool _triggered;                   // 0x68
};
DECOMP_SIZE_ASSERT(CutsceneRegionMain, 0x6C);

// FUNCTION: USA 0x001085BF
// PROTOTYPE: PREVIEW 0x004F2F78 identical
// NAME: proto-pdb:default.pdb
CutsceneAnimationScript::CutsceneAnimationScript(SwampSceneNode* node, CutsceneDriver* cd, unsigned int id)
    : SwampScript(node), _cd(cd), _id(id)
{
}

// FUNCTION: USA 0x001085E5
// PROTOTYPE: PREVIEW 0x004F2F9E identical
// NAME: proto-pdb:default.pdb
void CutsceneAnimationScript::AnimationFinished()
{
    if (_cd)
        _cd->AnimationFinished(_id);
}

// FUNCTION: USA 0x001085F9
// PROTOTYPE: PREVIEW 0x004F2FB2 identical
// NAME: proto-pdb:default.pdb
void CutsceneAnimationScript::AnimationTrigger(const SwampString& name, float value, float prev, unsigned int bone)
{
    if (_cd)
        _cd->AnimationTrigger(_id, name, value, prev, bone);
}

// FUNCTION: USA 0x00108628
// PROTOTYPE: PREVIEW 0x004F2FE1 identical
// NAME: proto-pdb:default.pdb
void CutsceneDriver::StateTransition(unsigned int id, int lastState)
{
    if (id == 0) {
        if (_state + 1 == lastState)
            Cleanup();
        else
            SetState(_state + 1);
    }
}

// FUNCTION: USA 0x00108649
// NAME: proto-pdb:default.pdb
CutsceneAnimationScript* CutsceneDriver::NewScript(SwampSceneNode* node, unsigned int id)
{
    return new("core\\Cutscene.cpp", 225) CutsceneAnimationScript(node, this, id);
}

// FUNCTION: USA 0x0010882D
// PROTOTYPE: PREVIEW 0x004F31C3 identical
// NAME: proto-pdb:default.pdb
GatorNode* CutsceneDriver::NewNode(unsigned int id)
{
    // !FAKE: c1xx front end; neutral local; the PDB records no local for the new node (three uses need one)
    GatorNode* p = new("core\\Cutscene.cpp", 217) GatorNode(ResourceHandle<ModelResource>());
    p->SetScript(NewScript(p, id));
    g_swamp->_scene->AddNode(p, 0);
    return p;
}

// FUNCTION: USA 0x001086AE
// NAME: proto-pdb:default.pdb
void CutsceneDriver::StopStream()
{
    _stream->Stop();
}

#ifdef NON_MATCHING
// best: match_one objdiff 89.09, bytes mismatch (masked 2)
// FUNCTION: USA 0x001086CE
// PROTOTYPE: PREVIEW 0x004F3070 near-identical
// NAME: proto-pdb:default.pdb
void CutsceneDriver::Init()
{
    g_swamp->_soundManager->SetMute(true);
    g_MusicManager->Suspend();
    while (_stream->IsValid() && !_stream->IsValidData()) {
        _stream->Update();
        g_swamp->GetDiskArbitrator().Update();
    }
    HUDUI::_self->SetOption(SwampSceneNode::DontDraw, false, false);
    HUDUI::_self->BeginLetterbox(0.0f, 0.3f, 0.0f);
    _cutsceneStart = g_swamp->GetInput().GetSessionTimeAccurate();
}
#endif

#ifdef NON_MATCHING
// FUNCTION: USA 0x00108762
// PROTOTYPE: PREVIEW 0x004F30FA near-identical
// NAME: proto-pdb:default.pdb
void CutsceneDriver::Cleanup()
{
    _cutsceneStart = 0;
    _cameraStart = 0;
    if (_autostop)
        _stream->Stop();
    g_MusicManager->Resume();
    g_swamp->_soundManager->SetMute(false);
    HUDUI::_self->EndLetterbox(0.25f);
    if (_autostop)
        HUDUI::_self->ClearSubtitle();
    if (_cam)
        _cam->PopScript();
}
#endif

// FUNCTION: USA 0x001087D6
// PROTOTYPE: PREVIEW 0x004F316C identical
// NAME: proto-pdb:default.pdb
AnimCam* CutsceneDriver::NewCamera(const SwampString& name)
{
    if (_cam)
        _cam->PopScript();
    _cam = AnimCam::SwitchTo(name, 0.0f, false, 0.0f);
    _cam->SetScript(NewScript(_cam, 0));
    _cameraStart = g_swamp->GetInput().GetSessionTimeAccurate();
    return _cam;
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x0010892C
// PROTOTYPE: PREVIEW 0x004F32BF near-identical
// NAME: proto-pdb:default.pdb
void CutsceneDriver::BeginPlayback()
{
    if (_stream->field_DC)
        _stream->Play();
}
#endif

// FUNCTION: USA 0x001089AA
// PROTOTYPE: PREVIEW 0x004F333A identical
// NAME: proto-pdb:default.pdb
void CutsceneRegionMain::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_QUERYTRIGGERED)
        *(bool*)data = _triggered;
}

// FUNCTION: USA 0x001089BD
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; wall: precheck relink `our ranges overlap at 0x001089F9`: the ctor emits
// ??_7CutsceneDriver, whose slot 0 ??_GCutsceneDriver folds at 0x001089F9, pinned only as ??_GCameraHelperRegionMain (5.3 T6.c8)
CutsceneDriver::CutsceneDriver(SwampSceneNode* node)
    : SwampScript(node), _cam(0), _cancelled(false), _autostop(true), _state(-1), _cameraShake(0.0f)
{
    RegisterFrameTick();
}
#endif

// FUNCTION: USA 0x00108B62
// PROTOTYPE: PREVIEW 0x004F34C8 identical
// NAME: proto-pdb:default.pdb
CutsceneRegionMain::CutsceneRegionMain(SwampSceneNode* node) : SwampScript(node), _triggered(false)
{
    RegisterSaveLoadVar("cutscenename", &_cutsceneName, "");
    RegisterSaveLoadVar("triggered", &_triggered, false);
}

// FUNCTION: USA 0x00108BE4
// PROTOTYPE: PREVIEW 0x004F354A identical
// NAME: proto-pdb:default.pdb
void CutsceneRegionMain::Load(const BeepTDF& tdf)
{
    SwampScript::Load(tdf);
    if (_cutsceneName.empty())
        _cutsceneName = tdf.GetStringRecord("cutscene", "");
}

// FUNCTION: USA 0x00108CE6
// PROTOTYPE: PREVIEW 0x004F364C identical
// NAME: proto-pdb:default.pdb
void CutsceneRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (node == pManager->_vinceCache && !_triggered) {
        _triggered = true;
        SwampScriptManager::Instance().Create(_cutsceneName, 0);
    }
}
