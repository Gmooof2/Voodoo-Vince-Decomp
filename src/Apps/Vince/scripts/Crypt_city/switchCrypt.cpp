// UNIT: src/Apps/Vince/scripts/Crypt_city/switchCrypt.cpp OBJ: Xbox\OfficialRelease\switchCrypt.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

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
class Matrix;                // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

Vector3 Normalize(const Vector3&); // NAME: proto-pdb:default.pdb

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    Vector3 GetWorldFacing() const;
    const Vector3& SetLocation(const Vector3&);
    const Quaternion& SetFacing(const Vector3&);
    void SetScript(const SwampString&);
    class SwampScript* GetScript() const;

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableCylinder : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    float GetRadius() const { return _radius; }

    char field_A4[0xB0 - 0xA4];        // 0xA4
    float _radius;                     // 0xB0
};

// FUNCTION: USA 0x0005E3C6
// PROTOTYPE: PREVIEW 0x0044A805 identical
// NAME: proto-pdb:default.pdb
Vector3 SwampSceneNode::GetWorldFacing() const
{
    // !FAKE: inliner; axis local v1, dummy local v0, pointer local p to the returned Quaternion; retail inlines Apply (T4.c1 A.WF.p3 100.0; axis as a temporary 54.26)
    Vector3 v1(0.0f, 0.0f, 1.0f);
    Vector3 v0;
    const Quaternion* p = &GetWorldRotation();
    p->Apply(v1, v0);
    return v0;
}

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
    bool CheckActivationRange(const Vector3&, const Vector3&, float) const;

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line);

class AnimationResource; // NAME: proto-pdb:default.pdb
class AnimationControl;  // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    T* _Resource;                      // 0x00
};

class SwitchCryptMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    SwitchCryptMain(SwampSceneNode* node);
    virtual bool TestActivate() const;
    virtual void Activate();
    virtual void TimerEvent(unsigned int id);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _turnOn; // 0xBC
    int _id;                           // 0xC0
};
DECOMP_SIZE_ASSERT(SwitchCryptMain, 0xC4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void StayIdle(bool);
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString&);

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
};

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0005E406
// PROTOTYPE: PREVIEW 0x0044A845 identical
// NAME: proto-pdb:default.pdb
bool SwitchCryptMain::TestActivate() const
{
    if (_state != 0) // src proto-pdb:default.pdb Off
        return false;
    // !FAKE: c2 register allocation; neutral stand-in local (no PDB local); facing temp lea ecx without it (match_one 99.71 -> 100.0)
    Vince* vince = pManager->_vinceCache;
    if (CheckActivationRange(vince->GetWorldPosition(), vince->GetWorldFacing(), 0.4f))
        return true;
    return false;
}

class CryptBlockMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    CryptBlockMain(SwampSceneNode* node);
    virtual void Activate();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _open; // 0xBC
};
DECOMP_SIZE_ASSERT(CryptBlockMain, 0xC0);

class CryptBlock_AaMain : public CryptBlockMain { // NAME: proto-pdb:default.pdb
private:
    CryptBlock_AaMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBlock_AaMain, 0xC0);

class CryptBlock_AbMain : public CryptBlockMain { // NAME: proto-pdb:default.pdb
private:
    CryptBlock_AbMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBlock_AbMain, 0xC0);

class CryptBlock_BaMain : public CryptBlockMain { // NAME: proto-pdb:default.pdb
private:
    CryptBlock_BaMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBlock_BaMain, 0xC0);

class CryptBlock_BbMain : public CryptBlockMain { // NAME: proto-pdb:default.pdb
private:
    CryptBlock_BbMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBlock_BbMain, 0xC0);

class CryptBlock_CaMain : public CryptBlockMain { // NAME: proto-pdb:default.pdb
private:
    CryptBlock_CaMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBlock_CaMain, 0xC0);

class CryptBlock_CbMain : public CryptBlockMain { // NAME: proto-pdb:default.pdb
private:
    CryptBlock_CbMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBlock_CbMain, 0xC0);

class CryptBlock_DaMain : public CryptBlockMain { // NAME: proto-pdb:default.pdb
private:
    CryptBlock_DaMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBlock_DaMain, 0xC0);

class CryptBlock_DbMain : public CryptBlockMain { // NAME: proto-pdb:default.pdb
private:
    CryptBlock_DbMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBlock_DbMain, 0xC0);

// FUNCTION: USA 0x0005E454
// NAME: proto-pdb:default.pdb
void CryptBlockMain::Activate()
{
    pManager->_vinceCache->StayIdle(true);
    ((GatorNode*)_node)->StartAnimation(_open, false, false);
    SetState(1);
}

// FUNCTION: USA 0x0005E519
// PROTOTYPE: PREVIEW 0x0044A958 identical
// NAME: proto-pdb:default.pdb
void SwitchCryptMain::Activate()
{
    if (_state == 0) {
        // !FAKE: c2 register allocation; neutral stand-in local (no PDB local); retail loads the Vince cache into ebx first
        Vince* p = pManager->_vinceCache;
        Vector3 facing = _node->GetWorldFacing();
        facing.y = 0.0f;
        // !FAKE: c2 register allocation; neutral stand-in locals (no PDB local); retail keeps the node in edi, radius at [ebp-4]
        SwampSceneNode* v0 = _node;
        float v1 = ((CollidableCylinder*)v0)->GetRadius();
        // !FAKE: inliner; hand form of the vector scale and sum (no PDB inline operator), pointer locals to the returned temps,
        // one comma expression so the Normalize temp keeps its own slot (frame 0x50 -> 0x5C; T8.c1 draft 6 100.0)
        Vector3 v3;
        const Vector3* v2;
        const Vector3* v4;
        v2 = &Normalize(facing), v3.x = v1 * v2->x, v3.y = v1 * v2->y, v3.z = v1 * v2->z, v4 = &v0->GetWorldPosition();
        p->SetLocation(Vector3(v3.x + v4->x, v3.y + v4->y, v3.z + v4->z));
        p->SetFacing(Vector3(-facing.x, 0.0f, -facing.z));
        p->SetScript("VinceSwitch");
        ((GatorNode*)_node)->StartAnimation(_turnOn, false, false);
        SetState(1);
    }
}

// FUNCTION: USA 0x0005E610
// PROTOTYPE: PREVIEW 0x0044AA4F identical
// NAME: proto-pdb:default.pdb
void SwitchCryptMain::TimerEvent(unsigned int id)
{
    AnimCam::SwitchTo("camera_cryptBlockB2", 3.0f, false, 0.0f);
    // !FAKE: c1xx front end; neutral stand-in locals (no PDB local); retail reads pManager before format, destroys the temp before GetScript
    CritterManager* v0 = pManager;
    Critter* p = v0->Find(format("cryptblock_Bb", id));
    p->GetScript()->Activate();
}

// FUNCTION: USA 0x0005E756
// PROTOTYPE: PREVIEW 0x0044AB95 identical
// NAME: proto-pdb:default.pdb
SwampScript* SwitchCryptMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 15) SwitchCryptMain(node);
}

// FUNCTION: USA 0x0005E9F7
// PROTOTYPE: PREVIEW 0x0044AE36 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBlockMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 48) CryptBlockMain(node);
}

// FUNCTION: USA 0x0005EA1E
// PROTOTYPE: PREVIEW 0x0044AE5D identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBlock_AaMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 255) CryptBlock_AaMain(node);
}

// FUNCTION: USA 0x0005EA48
// PROTOTYPE: PREVIEW 0x0044AE87 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBlock_AbMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 256) CryptBlock_AbMain(node);
}

// FUNCTION: USA 0x0005EA72
// PROTOTYPE: PREVIEW 0x0044AEB1 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBlock_BaMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 257) CryptBlock_BaMain(node);
}

// FUNCTION: USA 0x0005EA9C
// PROTOTYPE: PREVIEW 0x0044AEDB identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBlock_BbMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 258) CryptBlock_BbMain(node);
}

// FUNCTION: USA 0x0005EAC6
// PROTOTYPE: PREVIEW 0x0044AF05 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBlock_CaMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 259) CryptBlock_CaMain(node);
}

// FUNCTION: USA 0x0005EAF0
// PROTOTYPE: PREVIEW 0x0044AF2F identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBlock_CbMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 260) CryptBlock_CbMain(node);
}

// FUNCTION: USA 0x0005EB1A
// PROTOTYPE: PREVIEW 0x0044AF59 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBlock_DaMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 261) CryptBlock_DaMain(node);
}

// FUNCTION: USA 0x0005EB44
// PROTOTYPE: PREVIEW 0x0044AF83 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBlock_DbMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("sarcophagusHustle\\switchCrypt.cpp", 262) CryptBlock_DbMain(node);
}
