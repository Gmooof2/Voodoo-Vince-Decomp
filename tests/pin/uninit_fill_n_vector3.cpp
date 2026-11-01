// compiler-pin test (1.4 T9): std::_Uninit_fill_n<Vector3 *,unsigned int,Vector3,std::allocator<Vector3> >,
// retail 0x00044B6E, 79 B (eh, loop). Types per prototype default.pdb (docs/evidence/compiler_pin.md): Vector3 size
// 0xC; std::allocator<Vector3> size 0x1, construct thiscall void (Vector3*, const Vector3&), out of line (retail
// 0x00044A73, declared only here). Body as the VC 7.1 <xmemory> _Uninit_fill_n (catch-all, rethrow).

struct Vector3 {
    float x;
    float y;
    float z;
};

namespace std {

struct _Nonscalar_ptr_iterator_tag {};

template<class _Ty>
class allocator {
public:
    void construct(_Ty *_Ptr, const _Ty& _Val);
};

template<class _FwdIt, class _Diff, class _Tval, class _Alloc> inline
void _Uninit_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    try {
        for (; 0 < _Count; --_Count, ++_First)
            _Al.construct(_First, _Val);
    } catch (...) {
        throw;
    }
}

template void _Uninit_fill_n<Vector3 *, unsigned int, Vector3, allocator<Vector3> >(
    Vector3 *, unsigned int, const Vector3&, allocator<Vector3>&, _Nonscalar_ptr_iterator_tag);

}  // namespace std
