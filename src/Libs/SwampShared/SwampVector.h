// HEADER: proto-pdb:default.pdb file libs\swampshared\swampvector.h
// Partial: declares only what matched units use so far.
#ifndef SWAMPVECTOR_H
#define SWAMPVECTOR_H

#include "../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    void RotateJ(float r);
    float Length() const;

    float x;                          // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(float x, float y, float z) { Set(x, y, z); }
    Quaternion(float x, float y, float z, float _w) : v(x, y, z), w(_w) {}
    void Set(float xr, float yr, float zr);
    Quaternion& operator*=(const Quaternion&);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

#endif
