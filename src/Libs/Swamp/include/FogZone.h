// HEADER: proto-pdb:default.pdb file libs\swamp\include\fogzone.h
// Partial: declares only what matched units use so far.
#ifndef FOGZONE_H
#define FOGZONE_H

#include "../../../include/decomp.h"
#include "../../SwampShared/SwampVector.h"
#include "../../SwampShared/SwampResource.h"
#include "SwampSceneNode.h"

class FogZone : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    FogZone(const ResourceHandle<ModelResource>& volume, const Vector3& color, float visibilityDistance);
    virtual ~FogZone() {}                                          // vtable+0x00
    virtual void Render(const Matrix&) const;                      // vtable+0x20

    ResourceHandle<ModelResource> _model;          // 0xA4
    Vector3 _points[8];                // 0xA8
    char field_108[0x118 - 0x108];     // 0x108  PDB: Vector4 _color; Vector4 not declared yet
    unsigned long _destAlphaBlend;     // 0x118
    float _visibilityDistance;         // 0x11C
    float _noiseSpeed;                 // 0x120
    float _noiseScale;                 // 0x124
    ResourceHandle<PixelShaderResource> _pshader;  // 0x128
    ResourceHandle<VertexShaderResource> _vshader; // 0x12C
};
DECOMP_SIZE_ASSERT(FogZone, 0x130);

#endif
