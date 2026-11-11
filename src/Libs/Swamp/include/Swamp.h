// HEADER: named for PDB class Swamp; no file recorded
// Partial: declares only what matched units use so far.
#ifndef SWAMP_H
#define SWAMP_H

#include "../../SwampShared/SwampResource.h"
#include "SwampInput.h"

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    AnimationManager& GetAnimationManager() const { return *_animationManager; }
    ModelManager& GetModelManager() const { return *_modelManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x18 - 0x8];          // 0x08
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x28 - 0x1C];        // 0x1C
    ModelManager* _modelManager;       // 0x28
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

#endif
