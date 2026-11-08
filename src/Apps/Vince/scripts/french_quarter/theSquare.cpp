// UNIT: src/Apps/Vince/scripts/french_quarter/theSquare.cpp OBJ: Xbox\OfficialRelease\theSquare.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    int compare(const E* _Ptr) const;

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
template <class T, class A> class deque {
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    int GetIntRecord(const SwampString&, int) const;
    bool GetBoolRecord(const SwampString&, bool) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
struct Vector4 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
struct Matrix { // NAME: proto-pdb:default.pdb
    void Multiply(const Matrix&, Matrix&) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);
struct AABox;                // NAME: proto-pdb:default.pdb
class SwampScript;           // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    float GetLength() const { return _length; }

    char field_24[0x2C - 0x24];        // 0x24 (_anim, _myAnim; not reached)
    float _length;                     // 0x2C
    int _triggers;                     // 0x30
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle& operator=(const ResourceHandle& h);
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    T& operator()() { return *_Resource; }
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationTime(float time) { _time = time; }
    float GetAnimationLength() const { return _anim._Resource->_length; }

    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x38 - 0x28];        // 0x28 (_owner .. _lightEnabled; not reached)
    float _time;                       // 0x38
};

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    void PushBack(T);
    T Fetch();

    std::deque<T, std::allocator<T> > _queue; // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    void Add(ResourceHandle<AnimationResource> anim);
    ResourceHandle<AnimationResource> FetchAnim() { return _animQueue.Fetch(); }

    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1,
        DontDraw = 0x2
    };
    void SetOption(unsigned int option, bool value, bool recursive);
    bool GetOption(unsigned int option) const { return (_RenderingOptions & option) != 0; }
    bool IsVisible() const { return !GetOption(DontDraw); }
    void SetVisible(bool visible, bool recursive);
    SwampScript* GetScript() const;
    const Quaternion& SetFacing(const Vector3&);
    const Vector3& SetLocation(const Vector3&);
    void SetScript(const SwampString&);
    unsigned int GetZone() const;
    virtual ~SwampSceneNode();                                   // vtable+0x00
    virtual const AABox& GetBounds() const;                      // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                    // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                 // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);               // vtable+0x10

    char field_4[0x88 - 0x4];          // 0x04 (not reached)
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0xA4 - 0x8C];        // 0x8C (not reached)
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetDeleteWhenDone(bool b) { _deleteWhenDone = b; }

    char field_A4[0xCE - 0xA4];        // 0xA4 (not reached)
    bool _deleteWhenDone;              // 0xCE
    char field_CF[0xE8 - 0xCF];        // 0xCF (not reached)
};
DECOMP_SIZE_ASSERT(FireFlyNode, 0xE8);

class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    FireFlyNode* Add(const SwampString&, const SwampString&, unsigned int, bool);
};
extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

class Locator { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetLocation() const { return _pos; }
    const Vector3& GetBasisVector(int i) const { return _basis[i]; }

    Vector3 _pos;                      // 0x00
    char field_C[0x1C - 0xC];          // 0x0C (_rot; not reached)
    Vector3 _basis[3];                 // 0x1C
    char field_40[0x48 - 0x40];        // 0x40 (_zone, _script; not reached)
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    Locator* Find(const SwampString&) const;
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    ResourceHandle<AnimationControl> TransitionAnimation(ResourceHandle<AnimationResource>, float, bool, bool);
    ResourceHandle<AnimationControl>& StartSecondaryAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void SetAnimSpeed(float);
    Matrix GetBoneOrientation(const SwampString&) const;
    virtual void Render(const Matrix&) const;

    char field_A4[0x200 - 0xA4];      // 0xA4 (not reached)
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);
    ResourceHandle<AnimationResource> GetDefaultIdleAnim() const;

    char field_200[0x2C8 - 0x200];     // 0x200 (not reached)
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CarriedObject : public Critter { // NAME: proto-pdb:default.pdb
public:
    char field_2C8[0x2E4 - 0x2C8];     // 0x2C8 (not reached)
};
DECOMP_SIZE_ASSERT(CarriedObject, 0x2E4);

class SquareObject : public CarriedObject { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF& tdf);
};
DECOMP_SIZE_ASSERT(SquareObject, 0x2E4);

class Beret : public Critter { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF& tdf);
    virtual void Render(const Matrix& m) const;
    void SetVisible(bool);
};
DECOMP_SIZE_ASSERT(Beret, 0x2C8);

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00 (not reached)
    Vince* _vinceCache;                // 0x28
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class Level { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() { return _script; }

    char field_0[0xC];                 // 0x00 (not reached)
    SwampScript* _script;              // 0x0C
};

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }

    char field_0[0x4];                 // 0x00 (not reached)
    Level* _curLevel;                  // 0x04
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }

    char field_0[0x18];                        // 0x00 (not reached)
    AnimationManager* _animationManager;       // 0x18
    char field_1C[0x68 - 0x1C];                // 0x1C (not reached)
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Return(float);
};

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
    void RegisterTimerEvent(unsigned int, float, bool);
    void RemoveTimerEvents();

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                       // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0xF8 - 0xBC];        // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

class MasqueradeMaskMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    MasqueradeMaskMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(MasqueradeMaskMain, 0xF8);

class MoneyMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    MoneyMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(MoneyMain, 0xF8);

class DialogueRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    SwampString _onscreenText;         // 0x4C
    SwampString _dialogueFilename;     // 0x68
    SwampString _camName;              // 0x84
    bool _hintDone;                    // 0xA0
};
DECOMP_SIZE_ASSERT(DialogueRegionMain, 0xA4);

class StoreDialogueRegionMain : public DialogueRegionMain { // NAME: proto-pdb:default.pdb
public:
    virtual void LeavingRegion(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(StoreDialogueRegionMain, 0xA4);

class DialoguePawnRegionMain : public StoreDialogueRegionMain { // NAME: proto-pdb:default.pdb
public:
    DialoguePawnRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DialoguePawnRegionMain, 0xA4);

class DialogueCostumeRegionMain : public StoreDialogueRegionMain { // NAME: proto-pdb:default.pdb
public:
    DialogueCostumeRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DialogueCostumeRegionMain, 0xA4);

class DialogueBurlesqueRegionMain : public StoreDialogueRegionMain { // NAME: proto-pdb:default.pdb
public:
    DialogueBurlesqueRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DialogueBurlesqueRegionMain, 0xA4);

class DialogueMovieRegionMain : public StoreDialogueRegionMain { // NAME: proto-pdb:default.pdb
public:
    DialogueMovieRegionMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DialogueMovieRegionMain, 0xA4);

class HornMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    HornMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(HornMain, 0xF8);

class PopcornMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    PopcornMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(PopcornMain, 0xF8);

class MuseumGateMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_OPEN = 0x14,
        MESSAGE_QUERY_OPEN = 0x15
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0,
        STATE_OPEN = 1
    };
    MuseumGateMain(SwampSceneNode* node);
    static SwampScript* creationFunc(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();
    virtual void SetState(int state);
    Critter* GetGate() { return (Critter*)_node; }

    ResourceHandle<AnimationResource> _animOpening; // 0xBC
    ResourceHandle<AnimationResource> _animClosing; // 0xC0
    ResourceHandle<AnimationResource> _animClosed;  // 0xC4
};
DECOMP_SIZE_ASSERT(MuseumGateMain, 0xC8);

class VincePlayHorn : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ENDHORN = 0x14
    };
    VincePlayHorn(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    Vince* GetVince() { return (Vince*)_node; }
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(VincePlayHorn, 0x4C);

class ClockPlatformMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SHOWTIME = 0x14,
        MESSAGE_HIDETIME = 0x15,
        MESSAGE_QUERY_TOUCHED_ONCE = 0x16
    };
};

// FUNCTION: USA 0x000743A5
// PROTOTYPE: PREVIEW 0x004604B1 identical
// NAME: proto-pdb:default.pdb
// retail ??1 has no vptr store: the class declares no dtor (C0029)
class SkeletonJazzGuyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_IDLE2 = 0
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_GIVEHINT = 1,
        STATE_YOUSTINK = 2,
        STATE_IDLE2 = 3
    };
    SkeletonJazzGuyMain(SwampSceneNode* node);
    virtual void SoundFinished(const SwampString& name);
    virtual void TimerEvent(unsigned int timerId);
    virtual void AnimationFinished();
private:
    virtual void SetState(int state);
    static SwampScript* creationFunc(SwampSceneNode* node);

    bool _playedStink;                // 0xBC
    bool _playedNoOpen;                // 0xBD
    ResourceHandle<AnimationResource> _animIdle2; // 0xC0
    MultiAnimation _animsNoHorn;       // 0xC4
    MultiAnimation _animsBadPlaying;   // 0xD8
};
DECOMP_SIZE_ASSERT(SkeletonJazzGuyMain, 0xEC);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1SkeletonJazzGuyMain@@UAE@XZ here (its ??_G and
// ctor, not matched yet); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_SkeletonJazzGuyMain_dtor(SkeletonJazzGuyMain* p)
{
    p->SkeletonJazzGuyMain::~SkeletonJazzGuyMain();
}

// ??_GDialogueRegionMain 0x000716AB, ??_GVinceKickedOut 0x00073E24, ??_GSkeletonJazzGuyMain 0x00074389 unmatched,
// no draft (5.3 T6.c13): companion wall: a virtual ??_G is emitted only with its class vtable, i.e. the ctor; ??0 of
// DialogueRegionMain (0x001220E8), VinceKickedOut (0x00073D67), SkeletonJazzGuyMain (0x00074151) are not passing rows.
// Lift: match the ctor. ??_GZombieCryptMain 0x00073BE0 unmatched, no draft; fold-name wall: it calls the ??1 fold
// 0x0001E456, whose theSquare class is StoreDoorMain (ctor 0x00073AF7, not a passing row); match_one `not defined in
// theSquare.obj`. Lift: rename the row to ??_GStoreDoorMain, then the companion ctor.

class StoreDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_CLOSED = 0,
        STATE_OPEN = 1
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_QUERY_OPEN = 0x14
    };
    virtual void SetState(int state);
    virtual void Message(SwampScript* source, int message, void* data);
    Critter* GetDoor() { return (Critter*)_node; }

    ResourceHandle<AnimationResource> _animClosed; // 0xBC
    ResourceHandle<AnimationResource> _animOpen;   // 0xC0
};
DECOMP_SIZE_ASSERT(StoreDoorMain, 0xC4);

class BeretMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_CHECKHAIR = 0x14
    };
    virtual void Message(SwampScript* source, int message, void* data);
    void CheckHair();
};
DECOMP_SIZE_ASSERT(BeretMain, 0xBC);

class VinceKickedOut : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SPECIAL_COSTUME = 0x14
    };
    virtual void Message(SwampScript* source, int message, void* data);

    bool _playSpecialCostumeBall;      // 0x4C
    char field_50[0x68 - 0x50];        // 0x50 (_state, _animNames; not reached)
};
DECOMP_SIZE_ASSERT(VinceKickedOut, 0x68);

class CostumeBallRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_ENDSUCCESS = 0,
        TIMER_ENDFAILURE = 1
    };
    virtual void TimerEvent(unsigned int timerId);
};
DECOMP_SIZE_ASSERT(CostumeBallRegionMain, 0x4C);

class PawnShopRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_ENDSUCCESS = 0,
        TIMER_ENDFAILURE = 1
    };
    virtual void TimerEvent(unsigned int timerId);
};
DECOMP_SIZE_ASSERT(PawnShopRegionMain, 0x4C);

// FUNCTION: USA 0x000714CE
// PROTOTYPE: PREVIEW 0x0045D650 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 54.166668, bytes match (cut-extent wall: retail cut symbol runs past the
// row; va_dis no difference; fix is row-end pins + re-cut, source is final; generated, 4.4 T10)
SwampScript* MasqueradeMaskMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 48) MasqueradeMaskMain(node);
}
#endif

// FUNCTION: USA 0x0007158F
// PROTOTYPE: PREVIEW 0x0045D711 identical
// NAME: proto-pdb:default.pdb
SwampScript* MoneyMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 95) MoneyMain(node);
}

// FUNCTION: USA 0x00071769
// PROTOTYPE: PREVIEW 0x0045D8A4 identical
// NAME: proto-pdb:default.pdb
SwampScript* DialoguePawnRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 504) DialoguePawnRegionMain(node);
}

// FUNCTION: USA 0x00071793
// PROTOTYPE: PREVIEW 0x0045D8CE identical
// NAME: proto-pdb:default.pdb
SwampScript* DialogueCostumeRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 505) DialogueCostumeRegionMain(node);
}

// FUNCTION: USA 0x000717BD
// PROTOTYPE: PREVIEW 0x0045D8F8 identical
// NAME: proto-pdb:default.pdb
SwampScript* DialogueBurlesqueRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 506) DialogueBurlesqueRegionMain(node);
}

// FUNCTION: USA 0x000717E7
// PROTOTYPE: PREVIEW 0x0045D922 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 50.0, bytes match (cut-extent wall: retail cut symbol runs past the
// row; va_dis no difference; fix is row-end pins + re-cut, source is final; generated, 4.4 T10)
SwampScript* DialogueMovieRegionMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 507) DialogueMovieRegionMain(node);
}
#endif

// FUNCTION: USA 0x000739B9
// PROTOTYPE: PREVIEW 0x0045FAF7 identical
// NAME: proto-pdb:default.pdb
SwampScript* HornMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 76) HornMain(node);
}

// FUNCTION: USA 0x000739E0
// PROTOTYPE: PREVIEW 0x0045FB1E identical
// NAME: proto-pdb:default.pdb
SwampScript* PopcornMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 113) PopcornMain(node);
}

// FUNCTION: USA 0x00073A07
// PROTOTYPE: PREVIEW 0x0045FB45 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 20.0, bytes match (cut-extent wall: retail cut symbol runs past the
// row; va_dis no difference; fix is row-end pins + re-cut, source is final; generated, 4.4 T10)
SwampScript* MuseumGateMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 129) MuseumGateMain(node);
}
#endif

// FUNCTION: USA 0x00073AD0
// PROTOTYPE: PREVIEW 0x0045FC0E identical
// NAME: proto-pdb:default.pdb
SwampScript* VincePlayHorn::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("theSquare\\theSquare.cpp", 408) VincePlayHorn(node);
}

// FUNCTION: USA 0x00074455
// PROTOTYPE: PREVIEW 0x00460567 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 41.935482, bytes match (cut-extent wall: retail cut symbol runs past the
// row; va_dis no difference; fix is row-end pins + re-cut, source is final; generated, 4.4 T10)
SwampScript* SkeletonJazzGuyMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\thesquare\\theSquare.h", 155) SkeletonJazzGuyMain(node);
}
#endif

// FUNCTION: USA 0x00071282
// NAME: proto-pdb:default.pdb
void SkeletonJazzGuyMain::TimerEvent(unsigned int timerId)
{
    if (timerId == TIMER_IDLE2)
        SetState(STATE_IDLE2);
}

// FUNCTION: USA 0x00071401
// NAME: proto-pdb:default.pdb
void MuseumGateMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_OPEN:
        SetState(STATE_OPEN);
        break;
    case MESSAGE_QUERY_OPEN:
        *(bool*)data = _state == STATE_OPEN;
        break;
    }
}

// FUNCTION: USA 0x00071426
// PROTOTYPE: PREVIEW 0x0045D5A8 identical
// NAME: proto-pdb:default.pdb
void MuseumGateMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_OPEN:
        GetGate()->StartAnimation(_animOpening, false, false);
        GetGate()->SetAnimSpeed(0.1f);
        break;
    case STATE_CLOSED:
        GetGate()->StartAnimation(_animClosed, false, false);
        break;
    }
}

// FUNCTION: USA 0x00071482
// NAME: proto-pdb:default.pdb
void SkeletonJazzGuyMain::AnimationFinished()
{
    switch (_state) {
    case STATE_GIVEHINT:
    case STATE_YOUSTINK:
        AnimCam::Return(1.0f);
        SetState(STATE_IDLE);
        break;
    case STATE_IDLE2:
        SetState(STATE_IDLE);
        break;
    }
}

// FUNCTION: USA 0x000714AF
// NAME: proto-pdb:default.pdb
void StoreDoorMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_QUERY_OPEN:
        *(bool*)data = _state == STATE_OPEN;
        break;
    default:
        CritterMain::Message(source, message, data);
    }
}

// FUNCTION: USA 0x0007153C
// NAME: proto-pdb:default.pdb
void StoreDoorMain::SetState(int state)
{
    if (_state != state) {
        switch (state) {
        case STATE_CLOSED: {
            ResourceHandle<AnimationControl> ach = GetDoor()->StartAnimation(_animClosed, false, false);
            ach().SetAnimationTime(_animClosed().GetLength());
            break;
        }
        case STATE_OPEN: {
            ResourceHandle<AnimationControl> ach = GetDoor()->StartAnimation(_animOpen, false, false);
            ach().SetAnimationTime(_animOpen().GetLength());
            break;
        }
        }
    }
    _state = state;
}

// FUNCTION: USA 0x000715B6
// NAME: proto-pdb:default.pdb
void SkeletonJazzGuyMain::SoundFinished(const SwampString& name)
{
    if (name == "VincePlaysHornBadly")
        SetState(STATE_YOUSTINK);
}

// FUNCTION: USA 0x00071649
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1DialogueRegionMain@@UAE@XZ in this unit (no vptr
// store, so no declared dtor, C0029; no matched function odr-uses it); this qualified call odr-uses it instead
// (matched_set [[emitter]] row)
void pin_emit_DialogueRegionMain_dtor(DialogueRegionMain* p)
{
    p->DialogueRegionMain::~DialogueRegionMain();
}

// FUNCTION: USA 0x0007183B
// NAME: proto-pdb:default.pdb
void BeretMain::CheckHair()
{
    Vince* vince = pManager->_vinceCache;
    if (vince)
        vince->GetScript()->Message(this, 0x18, (void*)_node->GetOption(SwampSceneNode::DontDraw));
}

// FUNCTION: USA 0x0007186A
// NAME: proto-pdb:default.pdb
void BeretMain::Message(SwampScript* source, int message, void* data)
{
    if (message == 6 || (message == MESSAGE_CHECKHAIR && pManager->_vinceCache))
        CheckHair();
}

// FUNCTION: USA 0x0007188B
// NAME: proto-pdb:default.pdb
void SquareObject::Load(const BeepTDF& tdf)
{
    CarriedObject::Load(tdf);
    // !FAKE: c1xx front end; stand-in locals; retail inlines SwampSceneNode::SetVisible (CrawdadJimmys.cpp defines it
    // out of line, an inline copy here fails relink LNK2005); the named string keeps retail's temp lifetime past SetOption
    SwampString tmp("visible");
    bool visible = tdf.GetBoolRecord(tmp, true);
    SetOption(DontDraw, !visible, false);
}

// FUNCTION: USA 0x00071945
// NAME: proto-pdb:default.pdb
void Beret::Load(const BeepTDF& tdf)
{
    Critter::Load(tdf);
    SetVisible(tdf.GetBoolRecord("visible", true));
}

// FUNCTION: USA 0x00071A17
// NAME: proto-pdb:default.pdb
void Beret::Render(const Matrix& m) const
{
    if (IsVisible()) {
        // !FAKE: c1xx front end; stand-in locals for the Vince pointer and the product (PDB records only `head`); the
        // explicit Matrix copy gives retail's by-value temp passed to GatorNode::Render
        Vince* v0 = pManager->GetVince();
        Matrix head = v0->GetBoneOrientation("head");
        Matrix v1;
        head.Multiply(m, v1);
        GatorNode::Render(Matrix(v1));
    } else
        GatorNode::Render(m);
}

template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

// FUNCTION: USA 0x00071F8F
// NAME: proto-pdb:default.pdb
MuseumGateMain::MuseumGateMain(SwampSceneNode* node) : CritterMain(node)
{
    _animClosed = ResourceHandle<AnimationResource>();
    _animOpening = g_swamp->GetAnimationManager().Load("museumGate_open");
    _animClosing = g_swamp->GetAnimationManager().Load("museumGate_close");
    SetState(STATE_CLOSED);
}

// FUNCTION: USA 0x0007205F
// NAME: proto-pdb:default.pdb
void MuseumGateMain::Load(const BeepTDF& tdf)
{
    CritterMain::Load(tdf);
    _state = tdf.GetIntRecord("state", 0);
    if (_state == STATE_OPEN) {
        ResourceHandle<AnimationControl> ach = GetGate()->StartAnimation(_animOpening, false, false);
        ach().SetAnimationTime(ach().GetAnimationLength());
    }
}

// FUNCTION: USA 0x00072136
// NAME: proto-pdb:default.pdb
void VincePlayHorn::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_ENDHORN)
        GetVince()->StartAnimation(g_swamp->GetAnimationManager().Load("vince_playhorn_end"), false, false);
}

// FUNCTION: USA 0x00072D16
// PROTOTYPE: PREVIEW 0x0045EE9E identical
// NAME: proto-pdb:default.pdb
void CostumeBallRegionMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_ENDSUCCESS: {
        Vince* vince = pManager->GetVince();
        Critter* money = pManager->Find("Money");
        vince->GetScript()->Message(this, 0x16, money);
        money->SetOption(SwampSceneNode::DontDraw, false, false);
        vince->SetOption(SwampSceneNode::DontDraw, false, false);
        vince->SetScript("VinceLeaveStore1");
        vince->StartSecondaryAnimation(g_swamp->GetAnimationManager().Load("vince_dialog_costumeBall"), false, false);
        Locator* locator = g_locatorManager->Find("locator_costumeBall");
        vince->SetWorldPosition(locator->GetLocation());
        vince->SetFacing(locator->GetBasisVector(2));
        break;
    }
    case TIMER_ENDFAILURE: {
        Vince* vince = pManager->GetVince();
        Locator* locator = g_locatorManager->Find("locator_costumeBall");
        vince->SetWorldPosition(locator->GetLocation());
        vince->SetFacing(locator->GetBasisVector(2));
        vince->SetOption(SwampSceneNode::DontDraw, false, false);
        vince->SetScript("VinceKickedOut");
        vince->GetScript()->Message(this, 0x14, 0);
        SoundManager::PlaySound("qsq_vboot01", 0, _node, true, 0.0f, 0.0f);
        break;
    }
    }
}

// FUNCTION: USA 0x0007309C
// PROTOTYPE: PREVIEW 0x0045F224 identical
// NAME: proto-pdb:default.pdb
void PawnShopRegionMain::TimerEvent(unsigned int timerId)
{
    switch (timerId) {
    case TIMER_ENDSUCCESS: {
        Vince* vince = pManager->GetVince();
        Critter* horn = pManager->Find("Horn");
        // !FAKE: c1xx front end; stand-in local; the assignment loads the manager before GetZone as retail
        // (a plain g_particleManager->Add loads it after the argument pushes; C0218)
        SwampParticleManager* p;
        FireFlyNode* node = (p = g_particleManager)->Add("", "hornAway", horn->GetZone(), true);
        node->SetLocation(horn->GetWorldPosition());
        node->SetDeleteWhenDone(true);
        horn->SetOption(SwampSceneNode::DontDraw, false, false);
        vince->GetScript()->Message(this, 0x16, horn);
        vince->SetOption(SwampSceneNode::DontDraw, false, false);
        vince->SetScript("VinceLeaveStore1");
        vince->StartSecondaryAnimation(g_swamp->GetAnimationManager().Load("vince_dialog_pawnShop"), false, false);
        Locator* locator = g_locatorManager->Find("locator_pawnShop");
        vince->SetWorldPosition(locator->GetLocation());
        vince->SetFacing(locator->GetBasisVector(2));
        break;
    }
    case TIMER_ENDFAILURE: {
        Vince* vince = pManager->GetVince();
        Locator* locator = g_locatorManager->Find("locator_pawnShop");
        vince->SetWorldPosition(locator->GetLocation());
        vince->SetFacing(locator->GetBasisVector(2));
        vince->SetOption(SwampSceneNode::DontDraw, false, false);
        vince->SetScript("VinceKickedOut");
        SoundManager::PlaySound("qsq_vboot02", 0, _node, true, 0.0f, 0.0f);
        break;
    }
    }
}

// FUNCTION: USA 0x0007395E
// NAME: proto-pdb:default.pdb
void StoreDialogueRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (node == pManager->GetVince()) {
        // !FAKE: c1xx front end; stand-in local; ends the string temporary before GetScript (BumperTargetMain form)
        Critter* tmp = pManager->Find("clockplatform");
        tmp->GetScript()->Message(this, ClockPlatformMain::MESSAGE_HIDETIME, 0);
    }
}

// FUNCTION: USA 0x00073D46
// NAME: proto-pdb:default.pdb
void MultiAnimation::Add(ResourceHandle<AnimationResource> anim)
{
    _animQueue.PushBack(anim);
}

// FUNCTION: USA 0x00073F1B
// PROTOTYPE: PREVIEW 0x00460055 identical
// NAME: proto-pdb:default.pdb
void SkeletonJazzGuyMain::SetState(int state)
{
    _state = state;
    Vince* vince = pManager->_vinceCache;
    Critter* critter = GetCritter();
    switch (state) {
    case STATE_IDLE:
        RemoveTimerEvents();
        critter->TransitionAnimation(critter->GetDefaultIdleAnim(), 0.1f, true, false);
        RegisterTimerEvent(TIMER_IDLE2, 8.0f, false);
        break;
    case STATE_IDLE2:
        RemoveTimerEvents();
        critter->TransitionAnimation(_animIdle2, 0.1f, false, false);
        break;
    case STATE_GIVEHINT:
        RemoveTimerEvents();
        critter->TransitionAnimation(_animsNoHorn.FetchAnim(), 0.1f, false, false);
        break;
    case STATE_YOUSTINK:
        RemoveTimerEvents();
        if (!_playedStink) {
            ResourceHandle<AnimationResource> ah = g_swamp->GetAnimationManager().Load("skeletonjazzguy_badplaying01");
            critter->TransitionAnimation(ah, 0.1f, false, false);
            _animsBadPlaying.Add(ah);
            _playedStink = true;
        } else {
            critter->TransitionAnimation(_animsBadPlaying.FetchAnim(), 0.1f, false, false);
        }
        vince->GetScript()->Message(this, 0x14, 0);
        break;
    }
}

// FUNCTION: USA 0x00075042
// NAME: proto-pdb:default.pdb
void VinceKickedOut::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_SPECIAL_COSTUME) {
        bool b;
        LevelLoader::Instance().GetCurLevel()->GetScript()->Message(this, 0x17, &b);
        if (!b) {
            LevelLoader::Instance().GetCurLevel()->GetScript()->Message(this, 0x16, 0);
            _playSpecialCostumeBall = true;
        }
    }
}
