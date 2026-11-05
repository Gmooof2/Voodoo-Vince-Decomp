// UNIT: src/Apps/Vince/scripts/Crypt_city/ZombieGuidanceCounselor.cpp OBJ: Xbox\OfficialRelease\ZombieGuidanceCounselor.obj OPT: /O1 /Oy
#include <math.h>
#include "../../../../include/decomp.h"

// !FAKE: c2 intrinsics; file-scope pragma; retail inlines fsin in ZombieGuidanceCounselorHud::Render under /O1 /Oy
#pragma intrinsic(sin)

#pragma warning(disable: 4530)
#include <string>
#include <deque>
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

#ifndef NON_MATCHING
// !FAKE: declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string(const char *);
template <> void basic_string<char,std::char_traits<char>,std::allocator<char> >::_Tidy(bool,unsigned int);
}
#endif

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class AABox;                 // NAME: proto-pdb:default.pdb
class Quaternion;            // NAME: proto-pdb:default.pdb
class SoundResource;         // NAME: proto-pdb:default.pdb
struct Matrix;               // NAME: proto-pdb:default.pdb
class ModelResource;         // NAME: proto-pdb:default.pdb
class AnimationResource;     // NAME: proto-pdb:default.pdb
class AnimationControl;      // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    T* _Resource;                      // 0x00
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                         // vtable+0x00
    virtual const AABox& GetBounds() const;            // vtable+0x04
    virtual Vector3 GetWorldPosition() const;          // vtable+0x08
    virtual Quaternion GetWorldRotation() const;       // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);     // vtable+0x10
    virtual void Invalidate();                         // vtable+0x14
    virtual void UpdateAnimation(unsigned int);        // vtable+0x18
    virtual void Update();                             // vtable+0x1C
    virtual void Render(const Matrix&) const;          // vtable+0x20
    void SetOption(unsigned int option, bool value, bool recursive);

    enum NodeOptions { // NAME: proto-pdb:default.pdb
        DontDraw = 0x2,
        UseAlpha = 0x8,
        Unlit = 0x200
    };

    char field_4[0xA0];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const; // vtable+0x2C
    char field_A4[0x8];                // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0x8];                // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>&);   // vtable+0x30
    bool HasCollisionBSP() const { return _hasCollisionBSP; }
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);
    void SetAnimSpeed(float);
    void OverrideTexture(const SwampString&, const SwampString&);

    char field_B4[0xBE - 0xB4];        // 0xB4
    bool _hasCollisionBSP;             // 0xBE
    char field_BF[0x200 - 0xBF];       // 0xBF
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF&);                             // vtable+0x34
    virtual SwampString Save();                                    // vtable+0x38
    virtual void Stun(float);                                      // vtable+0x3C
    virtual float GetGravity();                                    // vtable+0x40
    virtual void ReturnToHome();                                   // vtable+0x44
    void PlaySound(const SwampString&, unsigned int, bool, float, float);
    const Vector3& GetHomePosition() const { return _homePosition; }

    char field_200[0x234 - 0x200];     // 0x200
    Vector3 _homePosition;             // 0x234
    char field_240[0x2C8 - 0x240];     // 0x240
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(float, float); // NAME: proto-pdb:default.pdb
}

namespace Particle { // NAME: proto-pdb:default.pdb
void MonsterDeath(const Vector3&); // NAME: proto-pdb:default.pdb
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

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    void RegisterFrameTick();
    void UnRegisterFrameTick();
    void RegisterTimerEvent(unsigned int, float, bool);
    void RemoveTimerEvents();

    SwampScript* _parent;              // 0x24
    int _frameTicks;                   // 0x28
    bool _valid;                       // 0x2C
    bool _sleeping;                    // 0x2D
    char field_2E[0x4C - 0x2E];        // 0x2E (_pendingMessages, _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode* node);

    bool _isZone;                      // 0x4C
    bool _inRegion;                    // 0x4D
    char field_4E[0x64 - 0x4E];        // 0x4E (_progressHintLevel, _progressHints; not reached)
};
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

// PDB has two records (0x60, 0x64); retail reads _inRegion at 0x4D: the later 0x64 record (C0150)
class ZombieGuidanceCounselorRegionMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ZombieGuidanceCounselorRegionMain, 0x64);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum WanderModes { // NAME: proto-pdb:default.pdb
        WANDER_NONE = 0,
        WANDER_PATROL = 1,
        WANDER_CHASE = 2
    };
    Critter* GetCritter() { return (Critter*)_node; }
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    virtual void Message(SwampScript*, int, void*);
    void SetWanderPatrolCenter(const Vector3& center) { _wanderPatrolCenter = center; }
    void SetWanderPatrolRadius(float radius) { _wanderPatrolRadius = radius; }
    void SetWanderLeashRadius(float radius) { _wanderLeashRadius = radius; }
    void SetWanderSpeed(float speed) { _wanderSpeed = speed; }
    void SetWanderTurnRate(float rate) { _wanderTurnRate = rate; }
    void SetWanderAvoidLedges(bool avoid) { _wanderAvoidLedges = avoid; }

    int _state;                        // 0x4C
    Vector3 _wanderPatrolCenter;       // 0x50
    float _wanderPatrolRadius;         // 0x5C
    float _wanderLeashRadius;          // 0x60
    float _wanderLeashMargin;          // 0x64
    int _wanderMode;                   // 0x68
    char field_6C[0xA0 - 0x6C];        // 0x6C (_wanderDir .. _wanderChaseTarget; not reached)
    float _wanderSpeed;                // 0xA0
    float _wanderTurnRate;             // 0xA4
    bool _wanderAvoidLedges;           // 0xA8
    char field_A9[0xBC - 0xA9];        // 0xA9 (_wanderTurnAdjusting .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

extern "C" int __cdecl sprintf(char*, const char*, ...); // NAME: proto-pdb:default.pdb

class CXBFont { // NAME: proto-pdb:default.pdb
public:
    void SetScaleFactors(float, float);
    long DrawText(float, float, unsigned long, const SwampString&, unsigned long, float);
};

class Sprite { // NAME: proto-pdb:default.pdb
public:
    void Render() const;

    char field_0[0x4C];                // 0x00 (not reached)
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

class HUDUI { // NAME: proto-pdb:default.pdb
public:
    static HUDUI& Instance() { return *_self; }
    bool IsLetterboxed() const { return _letterboxEnabled; }

    char field_0[0xE4];                // 0x00
    bool _letterboxEnabled;            // 0xE4
    char field_E5[0x1CB];              // 0xE5
private:
    static HUDUI* _self;
};
DECOMP_SIZE_ASSERT(HUDUI, 0x2B0);

class ZombieGuidanceCounselorHud : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Render(const Matrix& m) const;

    CXBFont* _font;                    // 0xA4
    Sprite _impHeadIcon;               // 0xA8
    Sprite _vinceHeadIcon;             // 0xF4
    float _throbSinVince;              // 0x140
    float _throbSinImp;                // 0x144
    unsigned int _impScore;            // 0x148
    unsigned int _vinceScore;          // 0x14C
};
DECOMP_SIZE_ASSERT(ZombieGuidanceCounselorHud, 0x150);

class Globals { // NAME: proto-pdb:default.pdb
public:
    float GetZgcZombieSpeed() const { return _zgcZombieSpeed; }
    float GetZgcZombieWanderRange() const { return _zgcZombieWanderRange; }

    char field_0[0x2EC];               // 0x00
    float _zgcZombieSpeed;             // 0x2EC
    char field_2F0[0x2F8 - 0x2F0];     // 0x2F0
    float _zgcZombieWanderRange;       // 0x2F8
};

extern Globals gSettings; // NAME: proto-pdb:default.pdb

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_GAMESTART = 0x6
};

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x240 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

class ImpMain : public MonsterMain { // NAME: proto-pdb:default.pdb

    char field_240[0x300 - 0x240];     // 0x240 (not reached)
};
DECOMP_SIZE_ASSERT(ImpMain, 0x300);

class ZGCImpMain : public ImpMain { // NAME: proto-pdb:default.pdb
public:
    ZGCImpMain(SwampSceneNode* node);
    virtual void DeathFX();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_300[0x304 - 0x300];     // 0x300 (not reached)
};
DECOMP_SIZE_ASSERT(ZGCImpMain, 0x304);

class CryptImpMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    CryptImpMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_BC[0xD0 - 0xBC];        // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(CryptImpMain, 0xD0);

class CarriedObjectMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0xF8 - 0xBC];        // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(CarriedObjectMain, 0xF8);

class GbcmlMain : public CarriedObjectMain { // NAME: proto-pdb:default.pdb
public:
    GbcmlMain(SwampSceneNode* node);
    virtual void InvalidateScript();
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_F8[0x114 - 0xF8];       // 0xF8 (_animImpCarry .. _lastAttractGameTime; not reached)
    SwampSceneNode* _hud;              // 0x114
};
DECOMP_SIZE_ASSERT(GbcmlMain, 0x118);

class ZombieMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_PATROL = 1,
        STATE_SEEKCRYPT = 2,
        STATE_ENTERCRYPT = 3,
        STATE_ENTERCRYPT_EVIL = 4,
        STATE_INCRYPT = 5
    };
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_BORED = 0
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_RESET = 0x14
    };
    virtual void SetState(int state);
    virtual void TimerEvent(unsigned int timerId);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void CollidedWith(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr);

    char field_BC[0xDC - 0xBC];        // 0xBC (_speed .. _entry; not reached)
    ResourceHandle<AnimationResource> _animWalk;       // 0xDC
    ResourceHandle<AnimationResource> _animIdle;       // 0xE0
    ResourceHandle<AnimationResource> _animEnterCrypt; // 0xE4
};
DECOMP_SIZE_ASSERT(ZombieMain, 0xE8);

class ZombieHulaGirlMain : public ZombieMain { // NAME: proto-pdb:default.pdb
public:
    ZombieHulaGirlMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ZombieHulaGirlMain, 0xE8);

class ZombieBakerMain : public ZombieMain { // NAME: proto-pdb:default.pdb
public:
    ZombieBakerMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ZombieBakerMain, 0xE8);

class ZombieClownMain : public ZombieMain { // NAME: proto-pdb:default.pdb
public:
    ZombieClownMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ZombieClownMain, 0xE8);

class ZombieBelleMain : public ZombieMain { // NAME: proto-pdb:default.pdb
public:
    ZombieBelleMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ZombieBelleMain, 0xE8);

class ZombieFootballMain : public ZombieMain { // NAME: proto-pdb:default.pdb
public:
    ZombieFootballMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(ZombieFootballMain, 0xE8);

class ZombieCryptMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ABSORB_ZOMBIE = 0x14,
        MESSAGE_ON = 0x15,
        MESSAGE_OFF = 0x16
    };
    virtual void Message(SwampScript* source, int message, void* data);

    ResourceHandle<AnimationResource> _animIdle;   // 0xBC
    ResourceHandle<AnimationResource> _animAbsorb; // 0xC0
    bool _on;                          // 0xC4
};
DECOMP_SIZE_ASSERT(ZombieCryptMain, 0xC8);

class CryptHulaMain : public ZombieCryptMain { // NAME: proto-pdb:default.pdb
public:
    CryptHulaMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptHulaMain, 0xC8);

class CryptBakerMain : public ZombieCryptMain { // NAME: proto-pdb:default.pdb
public:
    CryptBakerMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBakerMain, 0xC8);

class CryptClownMain : public ZombieCryptMain { // NAME: proto-pdb:default.pdb
public:
    CryptClownMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptClownMain, 0xC8);

class CryptBelleMain : public ZombieCryptMain { // NAME: proto-pdb:default.pdb
public:
    CryptBelleMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptBelleMain, 0xC8);

class CryptFootballMain : public ZombieCryptMain { // NAME: proto-pdb:default.pdb
public:
    CryptFootballMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(CryptFootballMain, 0xC8);

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb

    char field_BC[0x110 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class DoorExitZGCMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    DoorExitZGCMain(SwampSceneNode* node);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(DoorExitZGCMain, 0x110);

// FUNCTION: USA 0x000579DE
// PROTOTYPE: PREVIEW 0x00444264 identical
// NAME: proto-pdb:default.pdb
void ZombieGuidanceCounselorRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (!_inRegion)
        LevelScript::EnteringRegion(node);
}

// FUNCTION: USA 0x000579EC
// PROTOTYPE: PREVIEW 0x00444281 identical
// NAME: proto-pdb:default.pdb
void GbcmlMain::InvalidateScript()
{
    if (_hud) {
        _hud->Invalidate();
        _hud = 0;
    }
    CarriedObjectMain::InvalidateScript();
}

#ifdef NON_MATCHING
// row USA 0x00057A0D, PREVIEW 0x004DE953 identical
// best: match_one objdiff 100.0, bytes match; wall: provenance_lint nested operator (C0212): no NAME tag, so
// no FUNCTION tag (4.4 T11.c3)
template ResourceHandle<SoundResource> const & std::deque<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >::const_iterator::operator*()const;
#endif

// FUNCTION: USA 0x000592AA
// PROTOTYPE: PREVIEW 0x0040DF9C identical
// NAME: proto-pdb:default.pdb
// best: match_one objdiff 100.0, bytes match (4.4 T13.c5); wall: fold VA (C0233): CameraManager.obj calls its
// deque<SwampScript*>::_Growmap alias at 0x000592AA, refused `lies in a replaced range`; needs CameraManager.cpp
// to emit that alias inline (C0233 Fix 1, C0270)
#ifdef NON_MATCHING
// !FAKE: declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> ResourceHandle<SoundResource> * * _Uninitialized_copy<ResourceHandle<SoundResource> * *,ResourceHandle<SoundResource> * *,std::allocator<ResourceHandle<SoundResource> *> >(ResourceHandle<SoundResource> * *,ResourceHandle<SoundResource> * *,ResourceHandle<SoundResource> * *,std::allocator<ResourceHandle<SoundResource> *> &);
template <> void deque<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >::_Xlen()const;
}
// VC 7.1 <deque> body verbatim as an explicit specialization: the header's _Xlen cannot be redeclared noreturn
// (C2381), so `__assume(0)` restates its throw body's effect (C0237, !FAKE) and no throw-info is emitted
template <> void std::deque<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >::_Growmap(size_type _Count)
{
    if (max_size() / _DEQUESIZ - _Mapsize < _Count) {
        _Xlen();
        __assume(0); // !FAKE: noreturn restated (C0237)
    }

    size_type _Inc = _Mapsize / 2;
    if (_Inc < _DEQUEMAPSIZ)
        _Inc = _DEQUEMAPSIZ;
    if (_Count < _Inc && _Mapsize <= max_size() / _DEQUESIZ - _Inc)
        _Count = _Inc;
    size_type _Myboff = _Myoff / _DEQUESIZ;
    _Mapptr _Newmap = this->_Almap.allocate(_Mapsize + _Count);
    _Mapptr _Myptr = _Newmap + _Myboff;

    _Myptr = _Uninitialized_copy(_Map + _Myboff, _Map + _Mapsize, _Myptr, this->_Almap);
    if (_Myboff <= _Count) {
        _Myptr = _Uninitialized_copy(_Map, _Map + _Myboff, _Myptr, this->_Almap);
        _Uninitialized_fill_n(_Myptr, _Count - _Myboff, (_Tptr)0, this->_Almap);
        _Uninitialized_fill_n(_Newmap, _Myboff, (_Tptr)0, this->_Almap);
    } else {
        _Uninitialized_copy(_Map, _Map + _Count, _Myptr, this->_Almap);
        _Myptr = _Uninitialized_copy(_Map + _Count, _Map + _Myboff, _Newmap, this->_Almap);
        _Uninitialized_fill_n(_Myptr, _Count, (_Tptr)0, this->_Almap);
    }

    _Destroy_range(_Map + _Myboff, _Map + _Mapsize, this->_Almap);
    if (_Map)
        this->_Almap.deallocate(_Map, _Mapsize);

    _Map = _Newmap;
    _Mapsize += _Count;
}
// !FAKE: emission; no caller of _Growmap in this unit, so cl drops the unreferenced specialization without this
template void std::deque<ResourceHandle<SoundResource>,std::allocator<ResourceHandle<SoundResource> > >::_Growmap(unsigned int);
#endif

// FUNCTION: USA 0x00057AE2
// PROTOTYPE: PREVIEW 0x00444355 identical
// NAME: proto-pdb:default.pdb
void ZombieMain::CollidedWith(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr)
{
    if (!critterHit || ((GatorNode*)critterHit)->HasCollisionBSP())
        CritterMain::CollidedWith(type, critterHit, cr);
}

// FUNCTION: USA 0x00057B04
// PROTOTYPE: PREVIEW 0x00444377 identical
// NAME: proto-pdb:default.pdb
void ZombieMain::SetState(int state)
{
    _state = state;
    switch (state) {
    case STATE_IDLE:
        RemoveTimerEvents();
        GetCritter()->StartAnimation(_animIdle, true, false);
        RegisterTimerEvent(TIMER_BORED, (float)BeepLib::RandomFloat(1.0f, 1.5f), false);
        break;
    case STATE_PATROL:
        _node->SetOption(SwampSceneNode::Unlit, false, true);
        RemoveTimerEvents();
        GetCritter()->StartAnimation(_animWalk, true, false);
        _wanderMode = WANDER_PATROL;
        RegisterTimerEvent(TIMER_BORED, (float)BeepLib::RandomFloat(2.0f, 3.0f), false);
        break;
    case STATE_INCRYPT:
        _sleeping = true;
        UnRegisterFrameTick();
        RemoveTimerEvents();
        _node->SetOption(SwampSceneNode::DontDraw, true, false);
        _node->SetOption(SwampSceneNode::UseAlpha, false, true);
        break;
    case STATE_SEEKCRYPT:
        RemoveTimerEvents();
        GetCritter()->StartAnimation(_animWalk, true, false);
        _wanderMode = WANDER_CHASE;
        _node->SetOption(SwampSceneNode::Unlit, true, true);
        break;
    case STATE_ENTERCRYPT:
    case STATE_ENTERCRYPT_EVIL:
        RemoveTimerEvents();
        GetCritter()->StartAnimation(_animEnterCrypt, false, false);
        break;
    }
}

// FUNCTION: USA 0x00057C35
// PROTOTYPE: PREVIEW 0x004444A8 identical
// NAME: proto-pdb:default.pdb
void ZombieMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_GAMESTART:
        SetWanderPatrolRadius(gSettings.GetZgcZombieWanderRange());
        SetWanderLeashRadius(1000.0f);
        SetWanderPatrolCenter(GetCritter()->GetHomePosition());
        SetWanderSpeed(gSettings.GetZgcZombieSpeed());
        SetWanderAvoidLedges(true);
        SetWanderTurnRate(2.0f);
        break;
    case MESSAGE_RESET:
        _sleeping = false;
        _node->SetOption(SwampSceneNode::DontDraw, false, false);
        GetCritter()->ReturnToHome();
        SetState(STATE_IDLE);
        RegisterFrameTick();
        break;
    default:
        CritterMain::Message(source, message, data);
        break;
    }
}

// FUNCTION: USA 0x00057EF3
// PROTOTYPE: PREVIEW 0x0044477E identical
// NAME: proto-pdb:default.pdb
SwampScript* ZGCImpMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 286) ZGCImpMain(node);
}

// FUNCTION: USA 0x00058571
// PROTOTYPE: PREVIEW 0x00444D5A identical
// NAME: proto-pdb:default.pdb
void ZGCImpMain::DeathFX()
{
    Particle::MonsterDeath(_node->GetWorldPosition());
    SoundManager::PlaySound("monsterpoof", 0, _node, false, 0.0f, 0.0f);
}

// FUNCTION: USA 0x000589B6
// PROTOTYPE: PREVIEW 0x004451BB identical
// NAME: proto-pdb:default.pdb
void ZombieMain::TimerEvent(unsigned int timerId)
{
    if (timerId == TIMER_BORED) {
        GetCritter()->PlaySound("cczg_zlost", 0x28, true, 0.0f, 0.0f);
        switch (_state) {
        case STATE_IDLE:
            SetState(STATE_PATROL);
            break;
        case STATE_PATROL:
            SetState(STATE_IDLE);
            break;
        }
    }
}

// FUNCTION: USA 0x00058B70
// PROTOTYPE: PREVIEW 0x00445375 identical
// NAME: proto-pdb:default.pdb
void ZombieCryptMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_ABSORB_ZOMBIE:
        GetCritter()->StartAnimation(_animAbsorb, false, false);
        break;
    case MESSAGE_ON:
        GetCritter()->OverrideTexture("crypt331_as_a", "");
        GetCritter()->SetAnimSpeed(1.0f);
        _on = true;
        break;
    case MESSAGE_OFF:
        _on = false;
        GetCritter()->OverrideTexture("crypt331_as_a", "crypt331done_as_a");
        GetCritter()->SetAnimSpeed(0.0f);
        break;
    }
}

// FUNCTION: USA 0x00058F4F
// PROTOTYPE: PREVIEW 0x00445754 identical
// NAME: proto-pdb:default.pdb
void ZombieGuidanceCounselorHud::Render(const Matrix& m) const
{
    if (HUDUI::Instance().IsLetterboxed())
        return;
    char buffer[256];
    _impHeadIcon.Render();
    sprintf(buffer, "%i", _impScore);
    // !FAKE: c2 register allocation; neutral stand-in local; one x87 value feeds both scale arguments
    float v0 = 1.0f;
    if (_throbSinImp >= 0.0f)
        // !FAKE: inliner; hand form of math.h sin(float); its header body predates the pragma
        v0 = (float)sin((double)_throbSinImp) * 0.5f + 1.0f;
    _font->SetScaleFactors(v0, v0);
    _font->DrawText(577.0f, 400.0f, 0xFFFFFFFF, buffer, 0, 0.0f);
    _vinceHeadIcon.Render();
    sprintf(buffer, "%i", _vinceScore);
    // !FAKE: c2 register allocation; neutral stand-in local; one x87 value feeds both scale arguments
    float v1 = 1.0f;
    if (_throbSinVince >= 0.0f)
        // !FAKE: inliner; hand form of math.h sin(float); its header body predates the pragma
        v1 = (float)sin((double)_throbSinVince) * 0.5f + 1.0f;
    _font->SetScaleFactors(v1, v1);
    _font->DrawText(129.0f, 400.0f, 0xFFFFFFFF, buffer, 0, 0.0f);
    _font->SetScaleFactors(1.0f, 1.0f);
}

// FUNCTION: USA 0x00057FD0
// PROTOTYPE: PREVIEW 0x00444833 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptImpMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 411) CryptImpMain(node);
}

// FUNCTION: USA 0x00059280
// PROTOTYPE: PREVIEW 0x00445AA1 identical
// NAME: proto-pdb:default.pdb
SwampScript* GbcmlMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 355) GbcmlMain(node);
}

// FUNCTION: USA 0x000593C0
// PROTOTYPE: PREVIEW 0x00445ACB identical
// NAME: proto-pdb:default.pdb
SwampScript* ZombieHulaGirlMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 100) ZombieHulaGirlMain(node);
}

// FUNCTION: USA 0x000593E7
// PROTOTYPE: PREVIEW 0x00445AF2 identical
// NAME: proto-pdb:default.pdb
SwampScript* ZombieBakerMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 112) ZombieBakerMain(node);
}

// FUNCTION: USA 0x0005940E
// PROTOTYPE: PREVIEW 0x00445B19 identical
// NAME: proto-pdb:default.pdb
SwampScript* ZombieClownMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 124) ZombieClownMain(node);
}

// FUNCTION: USA 0x00059435
// PROTOTYPE: PREVIEW 0x00445B40 identical
// NAME: proto-pdb:default.pdb
SwampScript* ZombieBelleMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 136) ZombieBelleMain(node);
}

// FUNCTION: USA 0x0005945F
// PROTOTYPE: PREVIEW 0x00445B6A identical
// NAME: proto-pdb:default.pdb
SwampScript* ZombieFootballMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 148) ZombieFootballMain(node);
}

// FUNCTION: USA 0x00059489
// PROTOTYPE: PREVIEW 0x00445B94 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptHulaMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 207) CryptHulaMain(node);
}

// FUNCTION: USA 0x000594B3
// PROTOTYPE: PREVIEW 0x00445BBE identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBakerMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 220) CryptBakerMain(node);
}

// FUNCTION: USA 0x000594DD
// PROTOTYPE: PREVIEW 0x00445BE8 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptClownMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 233) CryptClownMain(node);
}

// FUNCTION: USA 0x00059507
// PROTOTYPE: PREVIEW 0x00445C12 identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptBelleMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 246) CryptBelleMain(node);
}

// FUNCTION: USA 0x00059531
// PROTOTYPE: PREVIEW 0x00445C3C identical
// NAME: proto-pdb:default.pdb
SwampScript* CryptFootballMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 259) CryptFootballMain(node);
}

// FUNCTION: USA 0x0005B9D9
// PROTOTYPE: PREVIEW 0x00447EFA identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100, bytes match; pin_rows refuses: ctor pin 0x0005B952 is site-corr
// (symbols_pin.txt:3583); needs a disasm-verified ctor pin, source is final (generated, 4.4 T10)
SwampScript* DoorExitZGCMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\crypt_city\\zombieguidancecounselor\\ZombieGuidanceCounselor.h", 484) DoorExitZGCMain(node);
}
#endif
