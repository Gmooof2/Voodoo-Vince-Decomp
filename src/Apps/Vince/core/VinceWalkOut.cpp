// UNIT: src/Apps/Vince/core/VinceWalkOut.cpp OBJ: core\XBox\OfficialRelease\VinceWalkOut.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* ptr);
    ~basic_string() { _Tidy(true); }
protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[7];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
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

    char field_4[0x20 - 0x4];          // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24

protected:
    SwampScript(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class ModelResource { // NAME: proto-pdb:default.pdb
public:
    int GetBoneIndex(const SwampString& name, bool) const;
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    T* operator->() const { return _Resource; }

    T* _Resource;                      // 0x00
};

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xB4];                // 0x00
    ResourceHandle<ModelResource> _model; // 0xB4
    char field_B8[0xD4 - 0xB8];        // 0xB8
    int _posBone;                      // 0xD4
    char field_D8[0xF4 - 0xD8];        // 0xD8
    int _shadowBone;                   // 0xF4
    char field_F8[0x200 - 0xF8];       // 0xF8
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    char field_200[0x4];               // 0x200
    float _CurSpeed;                   // 0x204
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void PutOutFire();

    char field_208[0x2F5 - 0x208];     // 0x208
    bool _inputEnabled;                // 0x2F5
    bool _canUseEyeCam;                // 0x2F6
};

class GameCam { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
    Vector3 _Location;                 // 0x34
};

class VinceCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    void SetPositionFixed(const Vector3& pos, float);
};

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString&) const;
};

extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class VinceWalkOut : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    VinceWalkOut(SwampSceneNode* node);
    virtual ~VinceWalkOut();
    virtual void FrameTick();
    virtual bool LosingControl(const SwampString&);
    virtual void Message(SwampScript*, int, void*);
    Vince* GetVince() { return (Vince*)_node; }

    int _oldBone;                      // 0x4C
    SwampString _animName;             // 0x50
    bool _onFloor;                     // 0x6C
};
DECOMP_SIZE_ASSERT(VinceWalkOut, 0x70);

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1 = the vptr store)
// FUNCTION: USA 0x00136386
// PROTOTYPE: PREVIEW 0x00520576 near-identical
// NAME: proto-pdb:default.pdb
VinceWalkOut::VinceWalkOut(SwampSceneNode* node) : SwampScript(node)
{
    _onFloor = false;
    GetVince()->_CurSpeed = 0;
    GetVince()->_canUseEyeCam = false;
    GetVince()->_inputEnabled = false;
    GetVince()->PutOutFire();
    RegisterFrameTick();
    VinceCam* cam = (VinceCam*)g_CameraManager->Find("vincecam");
    cam->SetPositionFixed(cam->_Location, 1.0f);
    _oldBone = GetVince()->_posBone;
    GetVince()->_posBone = GetVince()->_model->GetBoneIndex("move", false);
    GetVince()->_shadowBone = -1;
}
#endif
