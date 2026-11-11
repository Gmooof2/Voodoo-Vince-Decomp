// HEADER: named for PDB class CritterMain; no file recorded
// Partial: declares only what matched units use so far.
#ifndef CRITTERMAIN_H
#define CRITTERMAIN_H

#include "../../../include/decomp.h"
#include "../../../Libs/SwampShared/SwampVector.h"
#include "../../../Libs/Swamp/include/SwampScript.h"
#include "Critter.h"

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    Critter* GetCritter() { return (Critter*)_node; }
    virtual ~CritterMain();
    virtual bool CanChaseTo(const Vector3& point);                 // vtable+0x64
    virtual bool CanPatrolTo(const Vector3& point);                // vtable+0x68
    virtual void SetState(int state);                              // vtable+0x6C

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

#endif
