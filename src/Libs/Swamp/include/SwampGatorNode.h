// HEADER: proto-pdb:default.pdb file libs\swamp\include\swampgatornode.h
// Partial: declares only what matched units use so far.
#ifndef SWAMPGATORNODE_H
#define SWAMPGATORNODE_H

#include "../../../include/decomp.h"
#include "../../SwampShared/SwampVector.h"
#include "../../SwampShared/SwampResource.h"
#include "SwampSceneNode.h"
#include "SwampScript.h"

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3& start, const Vector3& end, float radius) const; // vtable+0x24
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode* jorge, const Vector3& delta) const; // vtable+0x28
    virtual bool DoesJorgeRepelMe(const SwampSceneNode* jorge, bool bHorizontal) const; // vtable+0x2C

    char field_A4[0xAC - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    float GetHeight() const { return _height; }

    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    virtual void SetModel(const ResourceHandle<ModelResource>& mh); // vtable+0x30
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool looped, bool reverse);
    void SetEdgeGlow(bool edgeGlow) { _edgeGlow = edgeGlow; }
    void GetBoneOrientation(int, Quaternion&, Vector3&) const;

    char field_B4[0xC5 - 0xB4];        // 0xB4 (_model .. _dirtyAnimation; not reached)
    bool _edgeGlow;                    // 0xC5
    char field_C6[0x18C - 0xC6];       // 0xC6 (not reached)
    unsigned int _animTriggerFlags;    // 0x18C
    char field_190[0x200 - 0x190];     // 0x190 (not reached)
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

#endif
