// HEADER: named for PDB class SearchlightBeam; no file recorded
// Partial: declares only what matched units use so far.
#ifndef SEARCHLIGHTBEAM_H
#define SEARCHLIGHTBEAM_H

#include "../../../../include/decomp.h"
#include "../../../../Libs/SwampShared/SwampVector.h"
#include "../../../../Libs/Swamp/include/FogZone.h"

// A FogZone that follows one bone of its parent node (used as a GatorNode): Update copies the bone's position
// and sets the rotation to the bone's rotation times _baseRotation. With a negative _parentBone, Update does nothing.
// SearchlightBeam: the PDB has two class records (0x148 with _lightSound, 0x144 without); retail's
// ctor ??0SearchlightBeam@@QAE@HABUQuaternion@@@Z matches the later 0x144 record, used here.
class SearchlightBeam : public FogZone { // NAME: proto-pdb:default.pdb
public:
    SearchlightBeam(int parentBone, const Quaternion& baseRotation);
    virtual void Update();                                         // vtable+0x1C

    int _parentBone;                   // 0x130
    Quaternion _baseRotation;          // 0x134
};
DECOMP_SIZE_ASSERT(SearchlightBeam, 0x144);

#endif
