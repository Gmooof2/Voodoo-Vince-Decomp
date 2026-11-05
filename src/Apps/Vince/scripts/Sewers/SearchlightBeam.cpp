// UNIT: src/Apps/Vince/scripts/Sewers/SearchlightBeam.cpp OBJ: Xbox\OfficialRelease\SearchlightBeam.obj OPT: /O1 /Oy
// SearchlightBeam: the PDB has two class records (0x148 with _lightSound, 0x144 without); retail's
// ctor ??0SearchlightBeam@@QAE@HABUQuaternion@@@Z matches the later 0x144 record, used here.
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion& operator*=(const Quaternion&);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                     // vtable+0x00

    const Vector3& SetLocation(const Vector3&);
    const Quaternion& SetRotation(const Quaternion&);
    SwampSceneNode* GetParent() { return _Parent; }

    char field_4[0x24 - 0x4];          // 0x04
    SwampSceneNode* _Parent;           // 0x24
    char field_28[0xA4 - 0x28];        // 0x28
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xAC - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    char field_AC[0xB4 - 0xAC];        // 0xAC
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    void GetBoneOrientation(int, Quaternion&, Vector3&) const;

    char field_B4[0x200 - 0xB4];       // 0xB4
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

class FogZone : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~FogZone();                                            // vtable+0x00

    char field_A4[0x130 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(FogZone, 0x130);

class SearchlightBeam : public FogZone { // NAME: proto-pdb:default.pdb
public:
    virtual void Update();
    virtual ~SearchlightBeam();                                    // vtable+0x00

    int _parentBone;                   // 0x130
    Quaternion _baseRotation;          // 0x134
};
DECOMP_SIZE_ASSERT(SearchlightBeam, 0x144);

// FUNCTION: USA 0x000AB463
// PROTOTYPE: PREVIEW 0x00496285 identical
// NAME: proto-pdb:default.pdb
void SearchlightBeam::Update()
{
    if (_parentBone >= 0) {
        Vector3 pos;
        Quaternion rot;
        static_cast<GatorNode*>(GetParent())->GetBoneOrientation(_parentBone, rot, pos);
        // !FAKE: inliner; hand form of the inline Quaternion operator* (copy, *=, by-value result)
        Quaternion v1;
        {
            Quaternion v0 = rot;
            v0 *= _baseRotation;
            v1 = v0;
        }
        SetRotation(v1);
        SetLocation(pos);
    }
}
