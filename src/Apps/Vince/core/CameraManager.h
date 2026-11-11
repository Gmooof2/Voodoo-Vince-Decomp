// HEADER: proto-pdb:default.pdb file apps\vince\core\cameramanager.h
// Partial: declares only what matched units use so far.
#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "../../../Libs/SwampShared/SwampString.h"
#include "GameCam.h"

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    GameCam* Find(const SwampString& s) const;
};
extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

#endif
