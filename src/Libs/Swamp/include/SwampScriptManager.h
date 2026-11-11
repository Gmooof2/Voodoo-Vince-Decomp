// HEADER: named for PDB class SwampScriptManager; no file recorded
// Partial: declares only what matched units use so far.
#ifndef SWAMPSCRIPTMANAGER_H
#define SWAMPSCRIPTMANAGER_H

#include "../../SwampShared/SwampString.h"
#include "SwampSceneNode.h"
#include "SwampScript.h"

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc creationFunc);
};

#endif
