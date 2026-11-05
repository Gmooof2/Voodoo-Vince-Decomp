// UNIT: src/Apps/Vince/scripts/Carnival/HouseOfMirrors.cpp OBJ: Xbox\OfficialRelease\HouseOfMirrors.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
private:
    unsigned int _pad00[0x1C / 4];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3;              // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

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

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

enum { // NAME: proto-pdb:default.pdb
    MESSAGE_VINCEDIED = 0x2,
    MESSAGE_GAMESTART = 0x6
};

class Level { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xC];                 // 0x00
    SwampScript* _script;              // 0x0C
};

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();

    char field_0[0x4];                 // 0x00
    Level* _curLevel;                  // 0x04
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;
};

// src proto-pdb:default.pdb Critter base GatorNode : ... : SwampSceneNode, all at 0x0; not modelled
class Critter : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    char field_BC[0x13C - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class BasicWallSwitchMain : public SwitchMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Activate();
};
DECOMP_SIZE_ASSERT(BasicWallSwitchMain, 0x13C);

class Switch_GeneratorMain : public BasicWallSwitchMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Activate();
};

class BasicDoorMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Message { // NAME: proto-pdb:default.pdb
        MESSAGE_OPEN_INSTANT = 0x16
    };

    virtual void Message(SwampScript* source, int message, void* data);

    char field_BC[0x110 - 0xBC];       // 0xBC (not reached)
};
DECOMP_SIZE_ASSERT(BasicDoorMain, 0x110);

class BattleDoorsMain : public BasicDoorMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Message(SwampScript* source, int message, void* data);
};

class MirrorMonsterRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_FIND_MONSTER = 0x14
    };

    virtual void Message(SwampScript* source, int message, void* data);

    SwampString _monster;              // 0x4C
    Critter* _child;                   // 0x68
};
DECOMP_SIZE_ASSERT(MirrorMonsterRegionMain, 0x6C);

// FUNCTION: USA 0x0004484A
// NAME: proto-pdb:default.pdb
void Switch_GeneratorMain::Activate()
{
    // 0x18: CarnivalLevelMain::Messages, two PDB records disagree on its name
    LevelLoader::Instance()._curLevel->_script->Message(this, 0x18, 0);
    BasicWallSwitchMain::Activate();
}

// FUNCTION: USA 0x00043057
// NAME: proto-pdb:default.pdb
void BattleDoorsMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_VINCEDIED:
    case MESSAGE_GAMESTART:
        BasicDoorMain::Message(source, message, data);
        BasicDoorMain::Message(this, MESSAGE_OPEN_INSTANT, 0);
        break;
    default:
        BasicDoorMain::Message(source, message, data);
        break;
    }
}

// FUNCTION: USA 0x00043265
// NAME: proto-pdb:default.pdb
void MirrorMonsterRegionMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_GAMESTART:
        if (_child)
            _child->GetScript()->Message(this, 0x15, 0);
        break;
    case MESSAGE_FIND_MONSTER:
        _child = pManager->Find(_monster);
        break;
    }
}
