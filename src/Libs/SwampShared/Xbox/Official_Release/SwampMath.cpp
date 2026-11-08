// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampMath.cpp OBJ: Xbox\Official_Release\SwampMath.obj OPT: /O1 /Oy /Oi

// FUNCTION: USA 0x000DCE88
// PROTOTYPE: PREVIEW 0x004C81E5 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 0.0, bytes mismatch (intrinsics: and esp,-16, xmm0-2; retail hand __asm SSE xmm3/4/6/7, D116 wall)
#include <xmmintrin.h>
float FastReciprocalSquareRoot(float val)
{
    const float Half = 0.5f;
    const float Three = 3.0f;
    const float Min = 1.17549435e-38f;
    float result;
    __m128 x = _mm_max_ss(_mm_load_ss(&val), _mm_load_ss(&Min));
    __m128 r = _mm_rsqrt_ss(x);
    x = _mm_mul_ss(_mm_mul_ss(x, r), r);
    _mm_store_ss(&result, _mm_mul_ss(_mm_mul_ss(_mm_load_ss(&Half), r), _mm_sub_ss(_mm_load_ss(&Three), x)));
    return result;
}
#endif
