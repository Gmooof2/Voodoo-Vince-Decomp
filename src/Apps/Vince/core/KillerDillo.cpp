// UNIT: src/Apps/Vince/core/KillerDillo.cpp OBJ: core\XBox\OfficialRelease\KillerDillo.obj OPT: unknown
#include "../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[0x1C / 4];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode { // NAME: proto-pdb:default.pdb
};

// src proto-pdb:default.pdb Vince base Critter : GatorNode : CollidableCylinder : CollidableNode : SwampSceneNode, all
// at 0x0; only Critter and SwampSceneNode modelled
class Critter : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void MakeDead(bool);
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

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
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    Critter* GetCritter() { return (Critter*)_node; }

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

namespace BeepLib { // NAME: proto-pdb:default.pdb
unsigned int Random(); // NAME: proto-pdb:default.pdb
}

class MonsterMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_MONSTER_KILLED = 0x14
    };
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&);
    virtual void DeathFX();                                        // vtable+0x70

    char field_BC[0x240 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(MonsterMain, 0x240);

class KillerDilloMain : public MonsterMain { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_BORED = 0x0
    };
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_DILLO_EXPLODE = 0x15
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0x1,
        STATE_IDLE2 = 0x2,
        STATE_PATROL = 0x3,
        STATE_STARTROLL = 0x5,
        STATE_ROLL = 0x6,
        STATE_WINDUP_BITE_STRAIGHT = 0x8,
        STATE_WINDUP_BITE_LEFT = 0x9,
        STATE_WINDUP_BITE_RIGHT = 0xa,
        STATE_ATTACK_BITE_STRAIGHT = 0xb,
        STATE_ATTACK_BITE_LEFT = 0xc,
        STATE_ATTACK_BITE_RIGHT = 0xd,
        STATE_DAMAGE = 0xe,
        STATE_DEAD = 0x10
    };
    KillerDilloMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int timerId);
    virtual void AnimationFinished();
    virtual void HitBy(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr);
    virtual void SetState(int);
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_240[0x324 - 0x240];     // 0x240 (not reached)
public:
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
};
DECOMP_SIZE_ASSERT(KillerDilloMain, 0x324);

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x001234FA
// PROTOTYPE: PREVIEW 0x0050C0A2 identical
// NAME: proto-pdb:default.pdb
void KillerDilloMain::TimerEvent(unsigned int timerId)
{
    if (timerId == TIMER_BORED) {
        switch (_state) {
        case STATE_IDLE:
            if ((BeepLib::Random() & 1) == 0)
                SetState(STATE_IDLE2);
            else
                SetState(STATE_PATROL);
            break;
        case STATE_PATROL:
        case STATE_ROLL:
            SetState(STATE_IDLE);
            break;
        }
    }
}

// FUNCTION: USA 0x00123531
// NAME: proto-pdb:default.pdb
void KillerDilloMain::AnimationFinished()
{
    switch (_state) {
    case STATE_IDLE2:
        SetState(STATE_IDLE);
        break;
    case STATE_STARTROLL:
        SetState(STATE_ROLL);
        break;
    case STATE_ROLL:
        SetState(STATE_IDLE);
        break;
    case STATE_WINDUP_BITE_STRAIGHT:
        SetState(STATE_ATTACK_BITE_STRAIGHT);
        break;
    case STATE_WINDUP_BITE_LEFT:
        SetState(STATE_ATTACK_BITE_LEFT);
        break;
    case STATE_WINDUP_BITE_RIGHT:
        SetState(STATE_ATTACK_BITE_RIGHT);
        break;
    case STATE_ATTACK_BITE_STRAIGHT:
    case STATE_ATTACK_BITE_LEFT:
    case STATE_ATTACK_BITE_RIGHT:
        SetState(STATE_IDLE);
        break;
    case STATE_DAMAGE:
        SetState(STATE_IDLE);
        break;
    case STATE_DEAD:
        Message(this, MESSAGE_MONSTER_KILLED, 0);
        DeathFX();
        GetCritter()->MakeDead(true);
        break;
    }
}

// FUNCTION: USA 0x00123600
// NAME: proto-pdb:default.pdb
void KillerDilloMain::HitBy(CollisionType type, SwampSceneNode* critterHit, const Ptr<CollisionResultObject>& cr)
{
    if (_state == STATE_ROLL && critterHit == pManager->GetVince()) {
        Message(this, MESSAGE_DILLO_EXPLODE, 0);
    } else {
        MonsterMain::HitBy(type, critterHit, cr);
    }
}

// FUNCTION: USA 0x00124AE7
// PROTOTYPE: PREVIEW 0x0050D614 identical
// NAME: proto-pdb:default.pdb
SwampScript* KillerDilloMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\KillerDillo.h", 11) KillerDilloMain(node);
}

// FUNCTION: USA 0x00124B0E
// NAME: proto-pdb:default.pdb
KillerDilloMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("KillerDilloMain", creationFunc);
}
