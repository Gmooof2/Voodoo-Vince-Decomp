// HEADER: proto-pdb:default.pdb file libs\swampshared\swampresource.h
// Partial: declares only what matched units use so far.
#ifndef SWAMPRESOURCE_H
#define SWAMPRESOURCE_H

#include "../../include/decomp.h"
#include "SwampString.h"

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class PixelShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class VertexShaderResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};
class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

#endif
