// UNIT: src/Apps/Vince/scripts/french_quarter/TeslaCoils.cpp OBJ: Xbox\OfficialRelease\TeslaCoils.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
struct Quaternion;           // NAME: proto-pdb:default.pdb

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
    const SwampString& GetName() const { return _Name; }

    char field_4[0x4];                 // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x60 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(ModelResource, 0x60);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_24[0x34 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle& operator=(const ResourceHandle&);
    ~ResourceHandle()
    {
        if (_Resource)
            _Resource->SubRef();
    }

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }
    ModelManager& GetModelManager() const { return *_modelManager; }

    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x28 - 0x1C];        // 0x1C
    ModelManager* _modelManager;       // 0x28
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C

    char field_A4[0xAC - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0xB4 - 0xAC];        // 0xAC
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
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
    SwampSceneNode* GetNode() { return _node; }
protected:
    void RegisterSaveLoadVar(const SwampString&, SwampString*, const char*);
public:

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
protected:
    virtual void SetState(int);                                    // vtable+0x6C
public:

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    void Remove(const SwampString&);
    Critter* Find(const SwampString&);

    char field_0[0x2C];                // 0x00
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    BasicDoorMain(SwampSceneNode* node);
    virtual ~BasicDoorMain();
    virtual void Message(SwampScript*, int, void*);
    virtual void AnimationFinished();
    virtual void Activate();
    virtual void Load(const BeepTDF&);
    virtual SwampString Save();
    virtual int GetDefaultState() const;                           // vtable+0x70
protected:
    virtual void SetState(int);
public:
    Critter* GetBasicDoor() { return (Critter*)GetNode(); }

    ResourceHandle<AnimationResource> _animOpen; // 0xBC
    char field_C0[0x110 - 0xC0];       // 0xC0
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

// FUNCTION: USA 0x0008BFDD
// PROTOTYPE: PREVIEW 0x00477B00 identical
// NAME: proto-pdb:default.pdb
class TeslaCoilPartMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_DESTROY = 0x1A
    };
    enum State { // NAME: proto-pdb:default.pdb
        Closed = 0,
        Opening = 1,
        Open = 2
    };
    TeslaCoilPartMain(SwampSceneNode* node);
    virtual void Message(SwampScript*, int, void*);

    ResourceHandle<AnimationResource> _open; // 0x110
};
DECOMP_SIZE_ASSERT(TeslaCoilPartMain, 0x114);

class TeslaCoilLowerMain : public TeslaCoilPartMain { // NAME: proto-pdb:default.pdb
public:
    TeslaCoilLowerMain(SwampSceneNode* node);
    virtual void Message(SwampScript*, int, void*);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    SwampString _activatorName;        // 0x114
};
DECOMP_SIZE_ASSERT(TeslaCoilLowerMain, 0x130);

// FUNCTION: USA 0x0008C129
// NAME: proto-pdb:default.pdb
// FUNCTION: USA 0x0008C145
// NAME: proto-pdb:default.pdb
class TeslaCoilUpperMain : public TeslaCoilPartMain { // NAME: proto-pdb:default.pdb
public:
    TeslaCoilUpperMain(SwampSceneNode* node);
    virtual void Message(SwampScript*, int, void*);
    virtual void Load(const BeepTDF&);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    SwampString _model;                // 0x114
};
DECOMP_SIZE_ASSERT(TeslaCoilUpperMain, 0x130);

// FUNCTION: USA 0x0008BD7C
// PROTOTYPE: PREVIEW 0x0047789F identical
// NAME: proto-pdb:default.pdb
void TeslaCoilPartMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case 6:
        pManager->Remove("teslaCoilBlowUp");
        pManager->Remove("teslaCoilDestroyed");
        break;
    }
    BasicDoorMain::Message(source, message, data);
}

// FUNCTION: USA 0x0008BDF0
// PROTOTYPE: PREVIEW 0x00477913 identical
// NAME: proto-pdb:default.pdb
void TeslaCoilLowerMain::Message(SwampScript* source, int message, void* data)
{
    TeslaCoilPartMain::Message(source, message, data);
    switch (message) {
    case 6:
        TeslaCoilPartMain::Message(source, message, data);
        if (pManager->Find("DinoBoss"))
            SetState(0);
        break;
    case 7:
        _activatorName = source->GetNode()->GetName();
        break;
    case 0x14:
        SoundManager::PlaySound("qch_pad", 0x20, GetNode(), true, 0.0f, 0.0f);
        break;
    }
}

// FUNCTION: USA 0x0008BEBA
// PROTOTYPE: PREVIEW 0x004779DD identical
// NAME: proto-pdb:default.pdb
void TeslaCoilUpperMain::Message(SwampScript* source, int message, void* data)
{
    TeslaCoilPartMain::Message(source, message, data);
    switch (message) {
    case 6:
        TeslaCoilPartMain::Message(source, message, data);
        if (pManager->Find("DinoBoss"))
            SetState(0);
        break;
    case 0x14:
        SoundManager::PlaySound("qch_tesla", 0x20, GetNode(), true, 0.0f, 0.0f);
        break;
    case MESSAGE_DESTROY:
        _model = "teslaCoilDestroyed_base";
        GetBasicDoor()->SetModel(g_swamp->GetModelManager().Load(_model));
        break;
    }
}

// !FAKE: inliner; scaffold call; odr-uses the implicit ??1TeslaCoilPartMain@@UAE@XZ before the ctor's vtable emits
// ??_GTeslaCoilPartMain, so ??_G inlines it as retail does (retail's derived ctors in this unit are unmatched;
// without it ??_G calls ??1 out of line: match_one 68.75 -> 100.0) (matched_set [[emitter]] row)
#ifdef NON_MATCHING
// best: guarded with the ctor below (pin_rows: site-corr pins, C0264); scaffold for ??_G 0x0008BFDD
void pin_emit_TeslaCoilPartMain_dtor(TeslaCoilPartMain* p)
{
    p->TeslaCoilPartMain::~TeslaCoilPartMain();
}
#endif

// 5.3 T2.c5: the ctor's relink refusal below (C0369) is the ctor without the scaffold above: our unmatched ??_G
// (0x0008BFDD, kept via the vtable) then calls ??1 out of line; relink T7 placed ??1's COMDAT from that REL32 site
// (+4, 0x0008BFE1), but retail inlines ??1 there (no call), so the misread rel32 put it at 0x0009D06B, inside
// TrophyRoom's real REL32 field 0x0009D068..0x0009D06C. Not a tool fault. Ctor + scaffold unguarded: precheck
// gate_check rc 0. Stays guarded for pin_rows (below).
// FUNCTION: USA 0x0008BFB5
// PROTOTYPE: PREVIEW 0x00477AD8 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (with ??_G 0x0008BFDD); gate pin_rows refuses: relocates through site-corr pins ??0BasicDoorMain 0x00107C33 (symbols_pin.txt:293), ??_7TeslaCoilPartMain 0x001E2340 (:737) (C0264)
// 5.3 T2.c3 re-try: match_one 100.0 + bytes; precheck gate_check rc 2 `relink: reloc field past its buffer:
// TrophyRoom.obj_0009C34F piece .text offset 0xD19 (site 0x0009D068) + 4 > buffer 3356 B` (C0369), pin_rows as above
TeslaCoilPartMain::TeslaCoilPartMain(SwampSceneNode* node) : BasicDoorMain(node)
{
    SetState(Closed);
}
#endif

// FUNCTION: USA 0x0008C08E
// NAME: proto-pdb:default.pdb
TeslaCoilUpperMain::TeslaCoilUpperMain(SwampSceneNode* node) : TeslaCoilPartMain(node)
{
    _animOpen = g_swamp->GetAnimationManager().Load("TeslaCoilUpper_extend");
    RegisterSaveLoadVar("model", &_model, "teslaCoilUpper_base");
}

// FUNCTION: USA 0x0008C16C
// PROTOTYPE: PREVIEW 0x00477C8F identical
// NAME: proto-pdb:default.pdb
SwampScript* TeslaCoilLowerMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\museum\\TeslaCoils.h", 28) TeslaCoilLowerMain(node);
}

// FUNCTION: USA 0x0008C193
// PROTOTYPE: PREVIEW 0x00477CB6 identical
// NAME: proto-pdb:default.pdb
SwampScript* TeslaCoilUpperMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\french_quarter\\museum\\TeslaCoils.h", 45) TeslaCoilUpperMain(node);
}
