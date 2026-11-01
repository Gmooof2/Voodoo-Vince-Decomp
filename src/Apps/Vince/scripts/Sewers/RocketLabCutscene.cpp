// UNIT: src/Apps/Vince/scripts/Sewers/RocketLabCutscene.cpp OBJ: Xbox\OfficialRelease\RocketLabCutscene.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

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

class EthelRocketLabCutscene : public CutsceneDriver { // NAME: proto-pdb:default.pdb
public:
    enum States { // NAME: proto-pdb:default.pdb
        STATE_LAST = 0x8
    };
    virtual void AnimationFinished(unsigned int id);

    char field_78[0x84 - 0x78];        // 0x78
};
DECOMP_SIZE_ASSERT(EthelRocketLabCutscene, 0x84);

// FUNCTION: USA 0x0009F088
// PROTOTYPE: PREVIEW 0x0048A12C identical
// NAME: proto-pdb:default.pdb
void EthelRocketLabCutscene::AnimationFinished(unsigned int id)
{
    StateTransition(id, STATE_LAST);
}
