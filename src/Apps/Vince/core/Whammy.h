// HEADER: proto-pdb:default.pdb file apps\vince\core\whammy.h
// Partial: declares only what matched units use so far.
#ifndef WHAMMY_H
#define WHAMMY_H

#include "../../../include/decomp.h"
#include "../../../Libs/SwampShared/SwampString.h"
#include "../../../Libs/SwampShared/SwampResource.h"
#include "../../../Libs/Swamp/include/SwampSceneNode.h"
#include "../../../Libs/Swamp/include/SwampScript.h"
#include "Critter.h"
#include "CritterMain.h"

// Base of the WhammyResult*Main scripts. KillMonsterNow puts _modelRestore back on the node when it is valid,
// sends message 0x14 to _parent, clears the node's edge glow and kills the critter (MakeDead).
class WhammyResultScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultScript(SwampSceneNode* node);
    virtual void KillMonsterNow();                                // vtable+0x64

    ResourceHandle<ModelResource> _modelRestore; // 0x4C
};
DECOMP_SIZE_ASSERT(WhammyResultScript, 0x50);

// No override of its own; beyond WhammyResultScript it holds only unmodelled bytes (field_50).
class WhammyResultFlattenMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultFlattenMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x54 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultFlattenMain, 0x54);

// Holds _animFrozen, _animRecover and _finished. On TIMER_FINISH it clears the edge glow and plays _animRecover
// (with none, it finishes at once); AnimationFinished invalidates the script and sends message 10 to _parent.
class WhammyResultFrozenMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_FINISH = 0x0
    };
    WhammyResultFrozenMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int timerId);
    virtual void AnimationFinished();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    ResourceHandle<AnimationResource> _animFrozen;  // 0x50
    ResourceHandle<AnimationResource> _animRecover; // 0x54
    bool _finished;                    // 0x58
};
DECOMP_SIZE_ASSERT(WhammyResultFrozenMain, 0x5C);

// No override of its own; beyond WhammyResultScript it holds only unmodelled bytes (field_50).
class WhammyResultCutHorizontalMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultCutHorizontalMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultCutHorizontalMain, 0x58);

// No override of its own; beyond WhammyResultScript it holds only unmodelled bytes (field_50).
class WhammyResultCutVerticalMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultCutVerticalMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultCutVerticalMain, 0x58);

// No override of its own; beyond WhammyResultScript it holds only unmodelled bytes (field_50).
class WhammyResultCutMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultCutMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultCutMain, 0x58);

// Adds the timers TIMER_STARTANIM and TIMER_FINISH; its TimerEvent acts on TIMER_FINISH only (AnimationFinished).
class WhammyResultDisintegrateMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_STARTANIM = 0x0,
        TIMER_FINISH = 0x1
    };
    WhammyResultDisintegrateMain(SwampSceneNode* node);
    virtual void TimerEvent(unsigned int timerId);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultDisintegrateMain, 0x58);

// No override of its own; beyond WhammyResultScript it holds only unmodelled bytes (field_50).
class WhammyResultPokeMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultPokeMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x54 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultPokeMain, 0x54);

// No override of its own; beyond WhammyResultScript it holds only unmodelled bytes (field_50).
class WhammyResultJackhammerMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultJackhammerMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x54 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultJackhammerMain, 0x54);

// No override of its own; beyond WhammyResultScript it holds only unmodelled bytes (field_50).
class WhammyResultToxicMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultToxicMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x54 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultToxicMain, 0x54);

// No override of its own; beyond WhammyResultScript it holds only unmodelled bytes (field_50).
class WhammyResultBurstMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultBurstMain(SwampSceneNode* node);
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    char field_50[0x58 - 0x50];        // 0x50 (not reached)
};
DECOMP_SIZE_ASSERT(WhammyResultBurstMain, 0x58);

// Holds _animTwister; AnimationFinished kills the critter (KillMonsterNow) and invalidates the script.
class WhammyResultTwisterMain : public WhammyResultScript { // NAME: proto-pdb:default.pdb
public:
    WhammyResultTwisterMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

public:
    ResourceHandle<AnimationResource> _animTwister; // 0x50
};
DECOMP_SIZE_ASSERT(WhammyResultTwisterMain, 0x54);

// A CritterMain: MESSAGE_SET_WHAMMY_NAME builds a prop critter from the name after "whammy" (CreateProp) as a
// child of the node, MESSAGE_KILL_WHAMMY invalidates the prop and the script; STATE_WHAMMY_ANIM plays _animPowerUp.
class VinceWhammyMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SET_WHAMMY_NAME = 0x14,
        MESSAGE_KILL_WHAMMY = 0x15
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_WHAMMY_ANIM = 0x0,
        STATE_WHAMMY_REACT_ANIM = 0x1
    };
    VinceWhammyMain(SwampSceneNode* node);
    virtual void InvalidateScript();
    virtual void SetState(int state);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void CreateProp();                                     // vtable+0x70
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    Critter* _prop;                    // 0xBC
    ResourceHandle<AnimationResource> _animPowerUp; // 0xC0
    SwampString _whammyName;           // 0xC4
};
DECOMP_SIZE_ASSERT(VinceWhammyMain, 0xE0);

// Script of the prop CreateProp makes ("WhammyProp"). AnimationFinished sets DontDraw on the node and _finished,
// invalidates Vince's script, puts _modelVince back on Vince when _restoreVinceModel is set and sends his script
// MESSAGE_WHAMMY_CLEANUP; with a negative _cameraShake it first sets camera _cameraName's shake to 0.
class WhammyPropMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    WhammyPropMain(SwampSceneNode* node);
    virtual void AnimationFinished();
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    bool _restoreVinceModel;           // 0x4C
    ResourceHandle<ModelResource> _modelVince; // 0x50
    float _cameraShake;                // 0x54
    SwampString _cameraName;           // 0x58
    float _radius;                     // 0x74
    char field_78[0x7C - 0x78];        // 0x78 (_effect; not reached)
    SwampString _scriptName;           // 0x7C
    bool _finished;                    // 0x98
};
DECOMP_SIZE_ASSERT(WhammyPropMain, 0x9C);

#endif
