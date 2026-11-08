// UNIT: src/Apps/Vince/scripts/Carnival/BumperCarCutscenes.cpp OBJ: Xbox\OfficialRelease\BumperCarCutscenes.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

class SwampSceneNode;        // NAME: proto-pdb:default.pdb

void* operator new(unsigned int size, const char* file, int line);

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampScript();                                        // vtable+0x00

    char field_4[0x4C - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CutsceneDriver : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void AnimationFinished(unsigned int id);

protected:
    void StateTransition(unsigned int id, int state);

public:
    char field_4C[0x78 - 0x4C];        // 0x4C
};
DECOMP_SIZE_ASSERT(CutsceneDriver, 0x78);

class KosmobotPreludeCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_LAST = 0x10
    };
    KosmobotPreludeCutscene(SwampSceneNode* node);
    virtual void AnimationFinished(unsigned int id);

    char field_78[0xB0 - 0x78];        // 0x78
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(KosmobotPreludeCutscene, 0xB0);

// FUNCTION: USA 0x0004EAC8
// PROTOTYPE: PREVIEW 0x0043BA01 identical
// NAME: proto-pdb:default.pdb
void KosmobotPreludeCutscene::AnimationFinished(unsigned int id)
{
    StateTransition(id, STATE_LAST);
}

// FUNCTION: USA 0x0004FAB3
// PROTOTYPE: PREVIEW 0x0043C9EC identical
// NAME: proto-pdb:default.pdb
SwampScript* KosmobotPreludeCutscene::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("BumperCarBumpoff\\BumperCarCutscenes.cpp", 19) KosmobotPreludeCutscene(node);
}
