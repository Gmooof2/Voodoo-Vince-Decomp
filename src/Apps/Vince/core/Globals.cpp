// UNIT: src/Apps/Vince/core/Globals.cpp OBJ: core\XBox\OfficialRelease\Globals.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    void SetAllVolumes(float, float, float, float);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
    SoundManager* _soundManager;       // 0x34
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class Settings { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34]; // 0x00
};
DECOMP_SIZE_ASSERT(Settings, 0x34);

class GameOptions : public Settings { // NAME: proto-pdb:default.pdb
public:
    struct State { // NAME: proto-pdb:default.pdb
        float _musicVolume;            // 0x00
        float _soundVolume;            // 0x04
        float _cinematicVolume;        // 0x08
        int _subtitles;                // 0x0C
        int _reverseHCameraMovement;   // 0x10
        int _reverseVCameraMovement;   // 0x14
    };

    void Set() const;
    void BackupState();
    void RestorePreviousState();

    State _currState;                  // 0x34
    State _prevState;                  // 0x4C
};
DECOMP_SIZE_ASSERT(GameOptions::State, 0x18);
DECOMP_SIZE_ASSERT(GameOptions, 0x64);

// FUNCTION: USA 0x00125F88
// PROTOTYPE: PREVIEW 0x0050EA95 identical
// NAME: proto-pdb:default.pdb
void GameOptions::BackupState()
{
    _prevState = _currState;
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x00125F98
// PROTOTYPE: PREVIEW 0x0050EAA5 near-identical
// NAME: proto-pdb:default.pdb
void GameOptions::Set() const
{
    g_swamp->_soundManager->SetAllVolumes(_currState._soundVolume, _currState._musicVolume,
                                          _currState._cinematicVolume, 1.0f);
}
#endif

// FUNCTION: USA 0x00125FC3
// PROTOTYPE: PREVIEW 0x0050EAD1 identical
// NAME: proto-pdb:default.pdb
void GameOptions::RestorePreviousState()
{
    _currState = _prevState;
    Set();
}
