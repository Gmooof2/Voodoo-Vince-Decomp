// HEADER: named for PDB class SoundNode; no file recorded
// Partial: declares only what matched units use so far.
#ifndef SOUNDNODE_H
#define SOUNDNODE_H

#include "SwampSceneNode.h"

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual void Invalidate();
    void Stop();
};

#endif
