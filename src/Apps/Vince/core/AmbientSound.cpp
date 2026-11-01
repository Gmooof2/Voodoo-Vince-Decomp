// UNIT: src/Apps/Vince/core/AmbientSound.cpp OBJ: core\XBox\OfficialRelease\AmbientSound.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float LengthSquared() const { return x * x + y * y + z * z; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x94];                // 0x00
    Vector3 _listenerPos;              // 0x94
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
    SoundManager* _soundManager;       // 0x34
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
    Vector3 _Location;                 // 0x34
};

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_40[0xBC - 0x40];        // 0x40
    float _maxDist;                    // 0xBC
};

class AmbientSound : public SoundNode { // NAME: proto-pdb:default.pdb
private:
    bool IsInRange() const;
};

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes mismatch (masked 0; 3 bytes = the fmul operand address)
// FUNCTION: USA 0x00138088
// PROTOTYPE: PREVIEW 0x005222DB near-identical
// NAME: proto-pdb:default.pdb
bool AmbientSound::IsInRange() const
{
    const Vector3& listener = g_swamp->_soundManager->_listenerPos;
    Vector3 diff(_Location.x - listener.x, _Location.y - listener.y, _Location.z - listener.z);
    float range = _maxDist * 1.1f;
    return diff.LengthSquared() <= range * range;
}
#endif
