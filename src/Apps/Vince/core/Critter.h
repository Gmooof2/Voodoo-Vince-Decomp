// HEADER: proto-pdb:default.pdb file apps\vince\core\critter.h
// Partial: declares only what matched units use so far.
#ifndef CRITTER_H
#define CRITTER_H

#include "../../../include/decomp.h"
#include "../../../Libs/Swamp/include/SwampGatorNode.h"

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void MakeDead(bool killedbyvince);

    char field_200[0x2C8 - 0x200];     // 0x200 (not reached)
};
DECOMP_SIZE_ASSERT(Critter, 0x2C8);

#endif
