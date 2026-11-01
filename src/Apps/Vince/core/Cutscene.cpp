// UNIT: src/Apps/Vince/core/Cutscene.cpp OBJ: core\XBox\OfficialRelease\Cutscene.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
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

    char field_4[0x48];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

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
};

class AnimCam : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

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
    char field_8[0xC];                 // 0x08
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
    CutsceneAnimationScript* NewScript(SwampSceneNode* node, unsigned int id);

public:
    static void BeginPlayback();

    AnimCam* _cam;                     // 0x4C
    bool _cancelled;                   // 0x50
    bool _autostop;                    // 0x51
    char field_52[0x2];                // 0x52
    int _state;                        // 0x54
    char field_58[0x20];               // 0x58
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

class CutsceneAnimationScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CutsceneAnimationScript(SwampSceneNode*, CutsceneDriver*, unsigned int);
    virtual void AnimationFinished();

    CutsceneDriver* _cd;               // 0x4C
    unsigned int _id;                  // 0x50
};
DECOMP_SIZE_ASSERT(CutsceneAnimationScript, 0x54);

class CutsceneRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        MESSAGE_QUERYTRIGGERED = 0x14
    };
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);

    SwampString _cutsceneName;         // 0x4C
    bool _triggered;                   // 0x68
};
DECOMP_SIZE_ASSERT(CutsceneRegionMain, 0x6C);

// FUNCTION: USA 0x001085E5
// PROTOTYPE: PREVIEW 0x004F2F9E identical
// NAME: proto-pdb:default.pdb
void CutsceneAnimationScript::AnimationFinished()
{
    if (_cd)
        _cd->AnimationFinished(_id);
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

// FUNCTION: USA 0x00108BE4
// PROTOTYPE: PREVIEW 0x004F354A identical
// NAME: proto-pdb:default.pdb
void CutsceneRegionMain::Load(const BeepTDF& tdf)
{
    SwampScript::Load(tdf);
    if (_cutsceneName.empty())
        _cutsceneName = tdf.GetStringRecord("cutscene", "");
}
