// compiler-pin test (1.4 T8): std::fill<Vector3 *,Vector3>, retail 0x00044995, 33 B (fpo, loop).
// Types per prototype default.pdb (docs/evidence/compiler_pin.md): Vector3 size 0xC, float x/y/z at 0x0/0x4/0x8;
// fill is cdecl void (Vector3*, Vector3*, const Vector3&), no callees. Body as the VC 7.1 <xutility> fill loop.

struct Vector3 {
    float x;
    float y;
    float z;
};

namespace std {

template<class _FwdIt, class _Ty> inline
void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}

template void fill<Vector3 *, Vector3>(Vector3 *, Vector3 *, const Vector3&);

}  // namespace std
