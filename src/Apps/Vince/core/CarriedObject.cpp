// UNIT: src/Apps/Vince/core/CarriedObject.cpp OBJ: core\XBox\OfficialRelease\CarriedObject.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

class AnimationResource; // NAME: proto-pdb:default.pdb
class AnimationControl;  // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template<class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    bool empty() const { return _Mysize == 0; }
    static const unsigned int npos = (unsigned int)(-1);

private:
    char _pad00[0x14];                 // 0x00
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class T, class A = allocator<T> > class vector {
public:
    unsigned int size() const; // out of line in retail (fold VA 0x001114D9)
    T& operator[](unsigned int _Pos) { return *(_Myfirst + _Pos); }

private:
    char _pad00[0x4];                  // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random();
}

class BeepTDF;               // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb
struct AABox;                // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float x, y, z;
};

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion(float x, float y, float z, float _w) : v(x, y, z), w(_w) {}
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }
    void Apply(const Vector3&, Vector3&) const;

    Vector3 v; // 0x00
    float w;   // 0x0C
};

class DumbBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

    unsigned int _RefCount;            // 0x00
};

class BSPQueryResultObject : public DumbBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int HitWorld() const { return _tag; }
    unsigned int HitTag() const { return _tag; }

    unsigned int _tag;                 // 0x04
    float _pct;                        // 0x08
    const void* _blindData;            // 0x0C
    char field_10[0x40 - 0x10];        // 0x10
};

class CollisionResultObject : public BSPQueryResultObject { // NAME: proto-pdb:default.pdb
public:
    const SwampSceneNode* HitObject() const { return (const SwampSceneNode*)_blindData; }
    bool HitSomething() const { return HitWorld() || HitObject(); }
};

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    ~Ptr()
    {
        if (_Object)
            _Object->SubRef();
    }
    Ptr<T>& operator=(const Ptr<T>& p);
    T* operator->() { return _Object; }

    T* _Object;                        // 0x00
};

class CritterCollision { // NAME: proto-pdb:default.pdb
public:
    Ptr<CollisionResultObject> _wall;  // 0x00
    Ptr<CollisionResultObject> _floor; // 0x04
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        SortedRender = 0x10
    };
    virtual ~SwampSceneNode();                                     // vtable+0x00
    virtual const AABox& GetBounds() const;                        // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                      // vtable+0x08
    virtual Quaternion GetWorldRotation() const;                   // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);                 // vtable+0x10
    virtual void Invalidate();                                     // vtable+0x14
    virtual void UpdateAnimation(unsigned int);                    // vtable+0x18
    virtual void Update();                                         // vtable+0x1C
    virtual void Render(const Matrix&) const;                      // vtable+0x20
    void SetOption(unsigned int option, bool value, bool recursive);
    const Quaternion& SetRotation(const Quaternion& rotation);

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C
    void SetCollidable(bool c) { _collidable = c; }
    bool GetCollidable() const { return _collidable; }

    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
};

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    float GetHeight() const { return _height; }
    float GetRadius() const { return _radius; }

    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void StopAnim();
    void SetSuperEdgeGlowParams(const ResourceHandle<ModelResource>& mh, const Vector3& color, unsigned int bufferWidth,
                                unsigned int bufferHeight, float pulseFactor);

    char field_B4[0xC5 - 0xB4];        // 0xB4
    bool _edgeGlow;                    // 0xC5
    char field_C6[0x200 - 0xC6];       // 0xC6
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

// Retail layout (C0147): Critter grew +0x10 above 0x252 (src/Apps/Vince/core/Critter.cpp), _isEyeItem 0x2B7 -> 0x2C7.
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode* jorgeNode, const Vector3&) const;
    virtual bool DoesJorgeRepelMe(const SwampSceneNode* jorgeNode, bool bHorizontal) const;
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    virtual void ReturnToHome();                                   // vtable+0x44
    CritterCollision MoveMyFatAss(float, unsigned int);
    void SetVelocity(const Vector3& v) { _Velocity = v; }
    ResourceHandle<AnimationResource> GetDefaultIdleAnim() const; // out of line in CrawdadJimmys.cpp (0x19E39)

    char field_200[0x20C - 0x200];     // 0x200
    Vector3 _Velocity;                 // 0x20C
    char field_218[0x250 - 0x218];     // 0x218
    bool _OnFloor;                     // 0x250
    bool _onPlatform;                  // 0x251
    char field_252[0x288 - 0x252];     // 0x252
    ResourceHandle<AnimationResource> _defaultIdleAnim; // 0x288 proto 0x278
    char field_28C[0x2C7 - 0x28C];     // 0x28C
    bool _isEyeItem;                   // 0x2C7 proto 0x2B7
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class SwampCollide { // NAME: proto-pdb:default.pdb
public:
    static Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float, const GatorNode*, unsigned int);
};

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_INWATER = 0x10
};

class CarriedObject : public Critter { // NAME: proto-pdb:default.pdb
public:
    void SetQuestGlow(ResourceHandle<ModelResource> mh, float size);
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
    void RegisterFrameTick();
    void UnRegisterFrameTick();
    void RemoveTimerEvents();

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)GetNode(); }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    virtual void Message(SwampScript*, int, void*);

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0x0,
        STATE_CARRIED = 0x1,
        STATE_USE = 0x2,
        STATE_OBJECTSPECIFIC = 0x3
    };

    // Retail values (C0147): retail inserted a message at 0x16 (unnamed, no PDB name), USE/CANCELUSE moved up one.
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_PICKUP = 0x14,
        MESSAGE_DROP = 0x15,
        MESSAGE_USE = 0x17,            // proto 0x16
        MESSAGE_CANCELUSE = 0x18       // proto 0x17
    };

    virtual ~CarriedObjectMain();
    virtual void FrameTick();
    virtual void Message(SwampScript*, int, void*);
    virtual void AnimationFinished();

    ResourceHandle<AnimationResource> _animCarry; // 0xBC
    ResourceHandle<AnimationResource> _animUse;   // 0xC0
    std::vector<SwampString> _vinceAnimNames;     // 0xC4
    Critter* _owner;                   // 0xD4
    bool _persistentFrameTick;         // 0xD8
    bool _useAnimDone;                 // 0xD9
    bool _oldCollidable;               // 0xDA
    bool _oldEdgeGlow;                 // 0xDB
    SwampString _dropSoundName;        // 0xDC
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

// FUNCTION: USA 0x0010E93E
// PROTOTYPE: PREVIEW 0x004F9284 identical
// NAME: proto-pdb:default.pdb
void CarriedObjectMain::AnimationFinished()
{
    if (_state < STATE_OBJECTSPECIFIC) {
        _useAnimDone = true;
        if (_state != STATE_IDLE) {
            SetState(STATE_CARRIED);
            GetCritter()->StartAnimation(_animCarry, true, false);
        }
    }
}

// FUNCTION: USA 0x0010E970
// PROTOTYPE: PREVIEW 0x004F92B6 near-identical
// NAME: proto-pdb:default.pdb
void CarriedObjectMain::FrameTick()
{
    switch (_state) {
    case STATE_IDLE: {
        CritterCollision cr = GetCritter()->MoveMyFatAss(GetCritter()->GetGravity(), 3);
        if (cr._floor->HitTag() & 0x400) {
            GetCritter()->ReturnToHome();
        } else if (GetCritter()->_OnFloor) {
            if (!_dropSoundName.empty())
                SoundManager::PlaySound(_dropSoundName, 0x20, GetCritter(), true, 0.0f, 0.0f);
            GetCritter()->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
            // !FAKE: inline copy; retail inlines GetDefaultIdleAnim here, which CrawdadJimmys.cpp defines out of line (0x19E39)
            GetCritter()->StartAnimation(ResourceHandle<AnimationResource>(GetCritter()->_defaultIdleAnim), true, false);
            if (!GetCritter()->_onPlatform && !_persistentFrameTick)
                UnRegisterFrameTick();
            GetCritter()->SetCollidable(_oldCollidable);
        }
        break;
    }
    case STATE_CARRIED:
        // !FAKE: c2 block layout; dead statement; retail keeps the case-1 test, an empty case drops it (match_one 99.13 -> 100.0)
        GetCritter();
        break;
    case STATE_USE:
        if (_useAnimDone) {
            SetState(STATE_CARRIED);
            GetCritter()->StartAnimation(_animCarry, true, false);
        }
        break;
    }
}

// FUNCTION: USA 0x0010E8E2
// PROTOTYPE: PREVIEW 0x004F9228 near-identical
// NAME: proto-pdb:default.pdb
void CarriedObject::SetQuestGlow(ResourceHandle<ModelResource> mh, float size)
{
    SetSuperEdgeGlowParams(mh, Vector3(1.0f, 1.0f, 0.0f), 0xA0, 0x78, 0.0f);
    _edgeGlow = true;
    SetOption(SortedRender, true, false);
    _isEyeItem = true;
}

// FUNCTION: USA 0x0010EB1A
// PROTOTYPE: PREVIEW 0x004F9460 near-identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 97.87, bytes mismatch (frame 0xf0 vs 0xe4: Vector3 temp slots of the DROP loop; USE case esi/edi swapped)
void CarriedObjectMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_INWATER:
        GetCritter()->ReturnToHome();
        break;
    case MESSAGE_PICKUP:
        _oldCollidable = GetCritter()->GetCollidable();
        SetState(STATE_CARRIED);
        GetCritter()->StartAnimation(_animCarry, true, false);
        if (!_persistentFrameTick)
            UnRegisterFrameTick();
        _owner = (Critter*)source->GetNode();
        _oldEdgeGlow = GetCritter()->_edgeGlow;
        GetCritter()->_edgeGlow = false;
        break;
    case MESSAGE_DROP: {
        RemoveTimerEvents();
        if (_state == STATE_USE && !_persistentFrameTick)
            UnRegisterFrameTick();
        SetState(STATE_IDLE);
        GetCritter()->StopAnim();
        // !FAKE: inline copy; retail inlines GetDefaultIdleAnim here, which CrawdadJimmys.cpp defines out of line (0x19E39)
        GetCritter()->StartAnimation(ResourceHandle<AnimationResource>(GetCritter()->_defaultIdleAnim), true, false);
        const Vector3 directions[4] = {
            Vector3(0.0f, 0.0f, 1.0f),
            Vector3(1.0f, 0.0f, 0.0f),
            Vector3(-1.0f, 0.0f, 0.0f),
            Vector3(0.0f, 0.0f, -1.0f)
        };
        int n;
        const Vector3* v5;
        const Vector3* v6;
        float v10;
        float v11;
        for (n = 0; n < 4; n++) {
            // !FAKE: inliner; hand form of Quaternion::Apply(v) and free Vector3 operators (D78) with pointer locals; out of line at /O1 without it (match_one 87.66 -> 97.87)
            float v0 = _owner->GetRadius();
            float v1 = GetCritter()->GetRadius();
            Vector3 v2;
            const Quaternion* v8 = &_owner->GetWorldRotation();
            v8->Apply(directions[n], v2);
            float v9 = v1 + v0;
            const Vector3* v3 = &Vector3(v2);
            Vector3 todest(v3->x * v9, v3->y * v9, v3->z * v9);
            const Vector3* v4 = &_owner->GetWorldPosition();
            Vector3 end(v4->x + todest.x, v4->y + todest.y, v4->z + todest.z);
            Vector3 start = end;
            start.y += _owner->GetHeight();
            Ptr<CollisionResultObject> cr = SwampCollide::CheckLine(start, end, 0.001f, 0, 0);
            Vector3 dest = cr->HitSomething()
                               ? (v10 = 1.0f - cr->_pct, v5 = &Vector3(start.x * v10, start.y * v10, start.z * v10),
                        v11 = cr->_pct, v6 = &Vector3(v11 * end.x, v11 * end.y, v11 * end.z),
                        Vector3(v6->x + v5->x, v6->y + v5->y, v6->z + v5->z))
                               : end;
            end = dest;
            end.y += GetCritter()->GetHeight() * 0.5f;
            Vector3 v7;
            v7.x = end.x - todest.x;
            v7.y = end.y - todest.y;
            v7.z = end.z - todest.z;
            start = v7;
            cr = SwampCollide::CheckLine(start, end, GetCritter()->GetRadius() * 0.25f, 0, 0);
            if (!cr->HitSomething()) {
                GetCritter()->SetWorldPosition(dest);
                break;
            }
        }
        if (n == 4)
            GetCritter()->SetWorldPosition(_owner->GetWorldPosition());
        RegisterFrameTick();
        _owner = 0;
        GetCritter()->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        GetCritter()->_edgeGlow = _oldEdgeGlow;
        break;
    }
    case 0x16:
        *(bool*)data = _vinceAnimNames.size() > 0;
        break;
    case MESSAGE_USE:
        if (_state == STATE_USE)
            break;
        if (_vinceAnimNames.size()) {
            _useAnimDone = false;
            GetCritter()->StopAnim();
            GetCritter()->StartAnimation(_animUse, false, false);
            if (data)
                *(SwampString*)data = _vinceAnimNames[BeepLib::Random() % _vinceAnimNames.size()];
            SetState(STATE_USE);
        } else {
            _useAnimDone = true;
            if (data)
                *(SwampString*)data = "";
        }
        break;
    case MESSAGE_CANCELUSE:
        RemoveTimerEvents();
        GetCritter()->StopAnim();
        GetCritter()->StartAnimation(_animCarry, false, false);
        if (_state == STATE_USE && !_persistentFrameTick)
            UnRegisterFrameTick();
        if (_state != STATE_IDLE)
            SetState(STATE_CARRIED);
        break;
    default:
        CritterMain::Message(source, message, data);
        break;
    }
}
#endif
