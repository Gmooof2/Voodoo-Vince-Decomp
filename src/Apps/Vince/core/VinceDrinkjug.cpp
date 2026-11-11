// UNIT: src/Apps/Vince/core/VinceDrinkjug.cpp OBJ: core\XBox\OfficialRelease\VinceDrinkjug.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }

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
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
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

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    AnimationManager* GetAnimationManager() { return _animationManager; }
    AnimationManager* _animationManager; // 0x18
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class AABox;                 // NAME: proto-pdb:default.pdb
class Quaternion;            // NAME: proto-pdb:default.pdb
class Matrix;                // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    unsigned int _RefCount;            // 0x00
};

class CollisionResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
};

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }

    T* _Object;                        // 0x00
};

class CritterCollision { // NAME: proto-pdb:default.pdb
public:
    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};

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

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C

    char field_A4[0xAC - 0xA4];        // 0xA4
};

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0xB4 - 0xAC];        // 0xAC
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    CritterCollision MoveMyFatAss(float, unsigned int);
    const Vector3& GetVelocity() const { return _Velocity; }
    void SetVelocity(const Vector3& v) { _Velocity = v; }

    char field_200[0x4];               // 0x200
    float _CurSpeed;                   // 0x204
    char field_208[0x4];               // 0x208
    Vector3 _Velocity;                 // 0x20C
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    char field_218[0x2F5 - 0x218];     // 0x218
    bool _inputEnabled;                // 0x2F5
    bool _canUseEyeCam;                // 0x2F6
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

    void RegisterFrameTick();

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    CritterMain(SwampSceneNode* node);
    virtual ~CritterMain();
    Critter* GetCritter() { return (Critter*)_node; }

    char field_4C[0xBC - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class VinceDrinkjug : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    VinceDrinkjug(SwampSceneNode* node);
    virtual void AnimationFinished();
    virtual void FrameTick();
    virtual bool LosingControl(const SwampString&);
    virtual ~VinceDrinkjug();
};
DECOMP_SIZE_ASSERT(VinceDrinkjug, 0xBC);

// FUNCTION: USA 0x00129CEB
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0), 5.5 T8.c7; wall: precheck matched_set `REFUSED
// ?FrameTick@VinceDrinkjug@@UAEXXZ pin has no size in symbols_pin.txt` (:7135 `func -`; retail extent 0x5E)
void VinceDrinkjug::FrameTick()
{
    Vector3 v = GetCritter()->GetVelocity();
    v.z = 0.0f;
    v.x = 0.0f;
    GetCritter()->SetVelocity(v);
    GetCritter()->MoveMyFatAss(GetCritter()->GetGravity(), 0);
}
#endif

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0), 5.5 T8.c6; wall: gate_check `our ranges overlap at 0x000E2D89`:
// the emitted ??_GVinceDrinkjug (vtable 0x001F79D0 slot 0) lands on fold VA 0x000E2D89, which has no ??_G/??_EVinceDrinkjug fold-alias pin (C0250)
// FUNCTION: USA 0x00129D49
// PROTOTYPE: PREVIEW 0x005127BA near-identical
// NAME: proto-pdb:default.pdb
VinceDrinkjug::VinceDrinkjug(SwampSceneNode* node) : CritterMain(node)
{
    Vince* vince = (Vince*)_node;
    vince->_CurSpeed = 0.0f;
    vince->_Velocity = Vector3(0.0f, 0.0f, 0.0f);
    vince->StartAnimation(g_swamp->GetAnimationManager()->Load("vince_knowhow"), false, false);
    RegisterFrameTick();
    vince->_canUseEyeCam = false;
    vince->_inputEnabled = false;
}
#endif
