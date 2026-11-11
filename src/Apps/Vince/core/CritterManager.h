// HEADER: named for PDB class CritterManager; no file recorded
// Partial: declares only what matched units use so far.
#ifndef CRITTERMANAGER_H
#define CRITTERMANAGER_H

#include "../../../Libs/SwampShared/SwampString.h"
#include "../../../Libs/SwampShared/SwampVector.h"
#include "../../../Libs/SwampShared/SwampResource.h"
#include "Critter.h"
#include "Vince.h"

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }
    Critter* Find(const SwampString& s);
    Critter* Create(const SwampString& name, const SwampString& scriptname, const char* species, ResourceHandle<ModelResource> mh, const Vector3& loc, const Quaternion& orient, unsigned int zone);

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

#endif
