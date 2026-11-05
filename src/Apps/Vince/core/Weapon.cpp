// UNIT: src/Apps/Vince/core/Weapon.cpp OBJ: core\XBox\OfficialRelease\Weapon.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AABox;      // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                 // vtable+0x00
    virtual const AABox& GetBounds() const;                    // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                  // vtable+0x08
    virtual Quaternion GetWorldRotation() const;               // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);             // vtable+0x10

    void RemoveNodeFromScene();
};

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetDeleteWhenDone(bool d) { _deleteWhenDone = d; }

    char field_4[0xCE - 0x4];          // 0x04
    bool _deleteWhenDone;              // 0xCE
    char field_CF[0xE8 - 0xCF];        // 0xCF
};
DECOMP_SIZE_ASSERT(FireFlyNode, 0xE8);

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void AddNode(SwampSceneNode* node, unsigned int);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xC];                 // 0x00
    SwampScene* _scene;                // 0x0C
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class WeaponMain { // NAME: proto-pdb:default.pdb
protected:
    void DropParticles(FireFlyNode*& node);
};

// FUNCTION: USA 0x001213A9
// PROTOTYPE: PREVIEW 0x0050A06D identical
// NAME: proto-pdb:default.pdb
void WeaponMain::DropParticles(FireFlyNode*& node)
{
    if (node) {
        Vector3 worldPos = node->GetWorldPosition();
        node->RemoveNodeFromScene();
        g_swamp->_scene->AddNode(node, 0);
        node->SetWorldPosition(worldPos);
        node->SetDeleteWhenDone(true);
        node = 0;
    }
}
