// HEADER: named for PDB class Matrix; no file recorded
// Partial: declares only what matched units use so far.
#ifndef MATRIX_H
#define MATRIX_H

#include "../../include/decomp.h"
#include "SwampVector.h"

struct Matrix { // NAME: proto-pdb:default.pdb
    void Orient(const Vector3& dir, const Vector3& up);
    void Euler(float& xr, float& yr, float& zr) const;

    char field_0[0x40];                // 0x00 (row; not reached)
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

#endif
