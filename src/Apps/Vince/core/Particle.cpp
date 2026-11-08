// UNIT: src/Apps/Vince/core/Particle.cpp OBJ: core\XBox\OfficialRelease\Particle.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class FireFlyNode; // NAME: proto-pdb:default.pdb
class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    FireFlyNode* AddOneShot(const SwampString&, const Vector3&);
};
extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

namespace Particle { // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0011FBE9
// NAME: proto-pdb:default.pdb
void LandDust(const Vector3& p)
{
    g_particleManager->AddOneShot("landDust", p);
}

// FUNCTION: USA 0x0011FC1C
// NAME: proto-pdb:default.pdb
void MonsterDeath(const Vector3& p)
{
    g_particleManager->AddOneShot("monsterDeath", p);
}

}
