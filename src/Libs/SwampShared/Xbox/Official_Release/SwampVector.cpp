// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampVector.cpp OBJ: Xbox\Official_Release\SwampVector.obj OPT: /O1 /Oy /Oi
#include <math.h>
#include <xmmintrin.h>
#include "../../../../include/decomp.h"

struct Vector3 { // NAME: proto-pdb:default.pdb
    void RotateJ(float r);
    unsigned int GetRGB() const;
    const float& operator[](int i) const { return (&x)[i]; }

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    void Set(unsigned int argb);

    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
    float w; // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

// FUNCTION: USA 0x000DCD1B
// PROTOTYPE: PREVIEW 0x004C8078 identical
// NAME: proto-pdb:default.pdb
void Vector4::Set(unsigned int argb)
{
    z = (argb & 0xFF) * (1.0f / 255.0f);
    y = ((argb >> 8) & 0xFF) * (1.0f / 255.0f);
    x = ((argb >> 16) & 0xFF) * (1.0f / 255.0f);
    w = (argb >> 24) * (1.0f / 255.0f);
}

// FUNCTION: USA 0x000DCDF6
// PROTOTYPE: PREVIEW 0x004C8153 identical
// NAME: proto-pdb:default.pdb
void Vector3::RotateJ(float r)
{
    float v1 = x;
    float v2 = sin(r);
    float v3 = cos(r);
    x = v3 * x - v2 * z;
    z = v2 * v1 + v3 * z;
}

// FUNCTION: USA 0x000DCE28
// PROTOTYPE: PREVIEW 0x004C8185 identical
// NAME: proto-pdb:default.pdb
unsigned int Vector3::GetRGB() const
{
    unsigned int rgb = 0;
    for (int i = 0; i < 3; i++) {
        float c = (*this)[i];
        if (c < 0.0f)
            c = 0.0f;
        if (c > 1.0f)
            c = 1.0f;
        float v0 = c * 255.5f;
        rgb = (rgb << 8) + _mm_cvtt_ss2si(_mm_load_ss(&v0));
    }
    return rgb | 0xFF000000;
}
