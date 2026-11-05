// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampShadow.cpp OBJ: Xbox\Official_Release\SwampShadow.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator;
template <class T, class A> class vector;
}

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class ModelResource;                   // NAME: proto-pdb:default.pdb
template <class T> class ResourceHandle; // NAME: proto-pdb:default.pdb
struct AABox;                          // NAME: proto-pdb:default.pdb

class ProjectModelShadow { // NAME: proto-pdb:default.pdb
public:
    ProjectModelShadow(const ResourceHandle<ModelResource>& model, const AABox& bounds,
                       const std::vector<bool, std::allocator<bool> >& stripControl);

    Vector3 _lightLoc;                 // 0x00
    Vector3 _lightColor;               // 0x0C
    Vector3 _lightDir;                 // 0x18
    float _lightConeCos;               // 0x24
    const ResourceHandle<ModelResource>& _model; // 0x28
    const AABox& _bounds;              // 0x2C
    const std::vector<bool, std::allocator<bool> >& _stripControl; // 0x30
};
DECOMP_SIZE_ASSERT(ProjectModelShadow, 0x34);

// FUNCTION: USA 0x000D6E93
// PROTOTYPE: PREVIEW 0x004C2424 identical
// NAME: proto-pdb:default.pdb
ProjectModelShadow::ProjectModelShadow(const ResourceHandle<ModelResource>& model, const AABox& bounds,
                                       const std::vector<bool, std::allocator<bool> >& stripControl)
    : _lightDir(0.0f, 0.0f, 1.0f), _lightConeCos(-1.0f), _model(model), _bounds(bounds), _stripControl(stripControl)
{
}
