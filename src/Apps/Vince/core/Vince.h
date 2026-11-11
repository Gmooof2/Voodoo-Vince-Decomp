// HEADER: proto-pdb:default.pdb file apps\vince\core\vince.h
// Partial: declares only what matched units use so far.
#ifndef VINCE_H
#define VINCE_H

#include "Critter.h"

class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

// src proto-pdb:default.pdb VinceMain base VinceShared, not modelled; only its Messages enum is reached
class VinceMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_WHAMMY_CLEANUP = 0x1A
    };
};

#endif
